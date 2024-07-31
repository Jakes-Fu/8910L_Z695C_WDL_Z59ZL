/******************************************************************************
 ** File Name:      gps_api.c                                                 *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           07/26/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of GPS   *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/26/2007     Liangwen.Zhen/David.Jia    Create.                         *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#include "ms_customize_trc.h"
#include "gps_drv.h"
#include "gps_cfg.h"
#include "os_api.h"
#include "nv_item_id.h"
#include "layer1_engineering.h"
#include "gps_nmea.h"
#include "threadx_os.h"
#include "threadx_assert.h"
#include "priority_system.h"
#include "sci_types.h"
#include "sci_api.h"
#include "nvitem.h"

#if defined(GPS_CHIP_VER_GREENEYE2)
#include "gps_interface.h"
#include "greeneye2_common.h"
#include "env_gps_signal.h"
#include "mn_gps_signal_def.h"
#ifdef GPS_TCARD_LOG_SUPPORT
#include "greeneye2_tcard_log.h"
#endif
#ifdef GPS_AGPS_SUPPORT_QX
#include "qx_supl_sdk.h"
#endif
#include "mn_type.h"
#endif

#define DEBUG_GPS_DRV
#ifdef  DEBUG_GPS_DRV
	#define GPS_TRACE   SCI_TRACE_LOW 
#else
	#define GPS_TRACE( _format_string )  
#endif


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#if defined(GPS_CHIP_VER_GREENEYE2)

LOCAL GPS_STATUS_E 		s_gps_status 			= GPS_STATUS_NOT_INIT;
LOCAL GPS_OPERATIONS_T*	s_gps_operations_ptr	= PNULL;
LOCAL GPS_MODE_E		s_gps_mode				= GPS_MODE_MAX;
LOCAL uint32 fac_init_count = 0;
LOCAL uint32 fac_restart_count = 0;

BLOCK_ID s_gps_interface_thread_id = SCI_INVALID_BLOCK_ID;
#define GPS_INTERFACE_THREAD_STACK_SIZE (10*1024)
#define s_gps_interface_thread_task_priority     PRI_GPS_INTERFACE_TASK    //31
#define GPS_INTERFACE_THREAD_QUEUE_NUM    0x20
BLOCK_ID s_sender_id = SCI_INVALID_BLOCK_ID;
BLOCK_ID mn_sender_id = SCI_INVALID_BLOCK_ID;
APP_MN_NCELL_RSSI_CNF_T        ncell;
MN_IMSI_T          agps_imsi_str;
BOOLEAN gps_report_agps_flag = SCI_FALSE;
BOOLEAN gps_agps_init_flag = SCI_FALSE;

static MN_DUAL_SYS_E agps_dual_sys;
static uint8     agps_netid; //value 5~11.   0:invalid

#ifdef GPS_AGPS_SUPPORT_QX
QX_report_data_t  request;
QX_agnssdata_client_t response;
QX_user_config_t config;

BLOCK_ID s_agps_interface_thread_id = SCI_INVALID_BLOCK_ID;
#define AGPS_INTERFACE_THREAD_STACK_SIZE (10*1024)
#define s_agps_interface_thread_task_priority     PRI_AGPS_INTERFACE_TASK    //31
#define AGPS_INTERFACE_THREAD_QUEUE_NUM    0x20
#endif

extern MN_PHONE_CURRENT_PLMN_INFO_T  MNPHONE_GetCurrentPLMNInfoEx(uint32   dual_sys);
extern ERR_MNPHONE_CODE_E MNPHONE_GetMeasReportEx(MN_DUAL_SYS_E dual_sys,APP_MN_NCELL_RSSI_CNF_T *ncell_meas_ptr,
            MN_PHONE_CELL_MEAS_T    *scell_meas_ptr);
extern MN_IMSI_T MNSIM_GetImsiEx(MN_DUAL_SYS_E dual_sys);

/**---------------------------------------------------------------------------*
 **                    LOCAL Function Prototypes                            *
 **---------------------------------------------------------------------------*/
LOCAL void GPS_Start_Interface_Thread(void);
LOCAL void GPS_Interface_Task_Deinit(void);
LOCAL void GPS_Interface_Task_Init(void);
LOCAL void GPS_Interface_Task_Entry(uint32 argc, void *argv);
LOCAL uint32 GPS_RequestDataFrom_Agps();
LOCAL uint32 GPS_ReportDataTo_Agps();
LOCAL void GPS_QXAgps_Init();
LOCAL void GPS_QXAgps_Release();

#ifdef GPS_AGPS_SUPPORT_QX
LOCAL void AGPS_Interface_Task_Entry(uint32 argc, void *argv);
#endif

/**---------------------------------------------------------------------------*
 **                     Function Prototypes                            *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    GPS_Interface_Task_Init
//  Author:         
//  Note:   
/*****************************************************************************/
LOCAL void GPS_Interface_Task_Init(void)
{
    GPS_Start_Interface_Thread();
}

/*****************************************************************************/
//  Description:    GPS_Interface_Task_Deinit
//  Author:         
//  Note:   
/*****************************************************************************/
LOCAL void GPS_Interface_Task_Deinit(void)
{
    if(s_gps_interface_thread_id != SCI_INVALID_BLOCK_ID)
    {
        SCI_TerminateThread(s_gps_interface_thread_id);
        SCI_DeleteThread(s_gps_interface_thread_id);
        s_gps_interface_thread_id = SCI_INVALID_BLOCK_ID;
    }    
}

/*****************************************************************************/
//  Description:    GPS_GetInterfaceThreadId
//  Author:         
//  Note:   
/*****************************************************************************/
PUBLIC BLOCK_ID GPS_GetInterfaceThreadId()
{
    return s_gps_interface_thread_id;
}
/*****************************************************************************/
//  Description:    GPS_Init_Request_Factory
//  Author:         
//  Note:   
/*****************************************************************************/
void GPS_Init_Request_Factory(void)
{
    xSignalHeader signal = PNULL;

    SCI_CREATE_SIGNAL(
                        signal,
                        GPS_FACTORY_INIT_REQ,
                        sizeof (xSignalHeaderRec),
                        SCI_IDENTIFY_THREAD());

    SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, s_gps_interface_thread_id); 
}

/*****************************************************************************/
//  Description:    GPS_Reset_Request_Factory
//  Author:         
//  Note:   
/*****************************************************************************/
void GPS_Reset_Request_Factory(uint8 mode)
{
    MN_GPS_RESET_REQ_T *signal = PNULL;

    SCI_CREATE_SIGNAL(
                        signal,
                        GPS_FACTORY_RESTART_REQ,
                        sizeof (MN_GPS_RESET_REQ_T),
                        SCI_IDENTIFY_THREAD());
    ((MN_GPS_RESET_REQ_T *)signal)->reset_mode = mode;

    SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, s_gps_interface_thread_id); 
}

/*****************************************************************************/
//  Description:    GPS_Reset_Request
//  Author:         
//  Note:   
/*****************************************************************************/
void GPS_Reset_Request(uint8 mode)
{
#if 0
    MN_GPS_RESET_REQ_T *signal = PNULL;

    SCI_CREATE_SIGNAL(
                        signal,
                        MN_GPS_RESET_REQ,
                        sizeof (MN_GPS_RESET_REQ_T),
                        SCI_IDENTIFY_THREAD());
    ((MN_GPS_RESET_REQ_T *)signal)->reset_mode = mode;

    SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, s_gps_interface_thread_id); 
#else
    GPS_SetRestart(mode);
#endif
}

/*****************************************************************************/
//  Description:    GPS_Interface_Task_Entry
//  Author:         
//  Note:   
/*****************************************************************************/
#ifdef GE2_ASSERT_DEBUG

#define INTERFACE_RECORD_SIZE 256

typedef struct ge2_interface_record_debug_tag
{
    uint32 tick;
    uint16 value;
} ge2_interface_record_debug_tag_t;

ge2_interface_record_debug_tag_t ge2_interface_record_debug[INTERFACE_RECORD_SIZE] = {0};
uint32 interface_record_index = 0;
ge2_interface_record_debug_tag_t ge2_agps_interface_record_debug[INTERFACE_RECORD_SIZE] = {0};
uint32 agps_interface_record_index = 0;

#endif

LOCAL void gps_interface_record_debug(uint16 value)
{
#ifdef GE2_ASSERT_DEBUG
	ge2_interface_record_debug[interface_record_index].tick = SCI_GetTickCount();
	ge2_interface_record_debug[interface_record_index].value = value;
	interface_record_index++;
	if(interface_record_index >= INTERFACE_RECORD_SIZE)
		interface_record_index = 0x0;
#endif
}

LOCAL void agps_interface_record_debug(uint16 value)
{
#ifdef GE2_ASSERT_DEBUG
	ge2_agps_interface_record_debug[agps_interface_record_index].tick = SCI_GetTickCount();
	ge2_agps_interface_record_debug[agps_interface_record_index].value = value;
	agps_interface_record_index++;
	if(agps_interface_record_index >= INTERFACE_RECORD_SIZE)
		agps_interface_record_index = 0x0;
#endif
}

LOCAL void GPS_Interface_Task_Entry(uint32 argc, void *argv)
{
    xSignalHeader signal_req = NULL;
    GPS_ERR_E err_val = GPS_ERR_NONE;
    uint32 data_Size = 0;
    uint32 byte_to_read = 1024;
    Ge2PositionParam_t *gps_buf_ptr = NULL;
    char *read_buf_ptr = NULL;
    MN_GPS_RESET_MODE_E mode = 0;
    MN_GPS_STATUS_E status = 0;
    uint32 output_mode = 0;
    uint16 time_interval = 0;
    
    do
    {
        signal_req = SCI_GetSignal (s_gps_interface_thread_id);
        s_sender_id = signal_req->Sender;
        err_val == GPS_ERR_NONE;
		TM_SendTestPointRequest(signal_req->SignalCode,s_sender_id);
		gps_interface_record_debug(signal_req->SignalCode);
        switch((mngps_signal_id_t)(signal_req->SignalCode))
            {
                case MN_GPS_DOWNLOAD_REQ:
		    mn_sender_id = s_sender_id;
                    err_val = GPS_Download();
                    {
                        MN_GPS_DOWNLOAD_CNF_T *signal = PNULL;

                        SCI_CREATE_SIGNAL(
                        signal,
                        MN_GPS_DOWNLOAD_CNF,
                        sizeof (MN_GPS_DOWNLOAD_CNF_T),
                        SCI_IDENTIFY_THREAD());
                        if(GPS_ERR_NONE == err_val)
                            signal->is_ok= SCI_TRUE;
                        else
                            signal->is_ok= SCI_FALSE;

                        SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, s_sender_id); 
                    }
                    break;
                case MN_GPS_INIT_REQ:
		    GPS_DebugLog_Enable(0x1);
                    err_val = GPS_Init_Greeneye2();
                    {
                        MN_GPS_INIT_CNF_T *signal = PNULL;

                        SCI_CREATE_SIGNAL(
                        signal,
                        MN_GPS_INIT_CNF,
                        sizeof (MN_GPS_INIT_CNF_T),
                        SCI_IDENTIFY_THREAD());
                        if(GPS_ERR_NONE == err_val)
                            signal->is_ok= SCI_TRUE;
                        else
                            signal->is_ok= SCI_FALSE;

                        SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, s_sender_id); 
                    }
                    break;
                case MN_GPS_START_REQ:
		     GPS_NmeaLog_Enable(0x1);
                     err_val = GPS_Open();
                     {
                        MN_GPS_START_CNF_T *signal = PNULL;

                        SCI_CREATE_SIGNAL(
                        signal,
                        MN_GPS_START_CNF,
                        sizeof (MN_GPS_START_CNF_T),
                        SCI_IDENTIFY_THREAD());
                        if(GPS_ERR_NONE == err_val)
                            signal->is_ok= SCI_TRUE;
                        else
                            signal->is_ok= SCI_FALSE;

                        SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, s_sender_id); 
                    }
                    break;
                case MN_GPS_STOP_REQ:
		    GPS_NmeaLog_Enable(0x0);
                    err_val = GPS_Close();
                    {
                        MN_GPS_STOP_CNF_T *signal = PNULL;

                        SCI_CREATE_SIGNAL(
                        signal,
                        MN_GPS_STOP_CNF,
                        sizeof (MN_GPS_STOP_CNF_T),
                        SCI_IDENTIFY_THREAD());
                        if(GPS_ERR_NONE == err_val)
                            signal->is_ok= SCI_TRUE;
                        else
                            signal->is_ok= SCI_FALSE;

                        SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, s_sender_id); 
                    }
                    break;
                case MN_GPS_RESET_REQ:
		    GPS_DebugLog_Enable(0x1);
                    mode = ((MN_GPS_RESET_REQ_T *)signal_req)->reset_mode;
                    err_val = GPS_SetRestart(mode);
                    {
                        MN_GPS_RESET_CNF_T *signal = PNULL;

                        SCI_CREATE_SIGNAL(
                        signal,
                        MN_GPS_RESET_CNF,
                        sizeof (MN_GPS_RESET_CNF_T),
                        SCI_IDENTIFY_THREAD());
                        if(GPS_ERR_NONE == err_val)
                            signal->is_ok= SCI_TRUE;
                        else
                            signal->is_ok= SCI_FALSE;

                        SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, s_sender_id); 
			 s_sender_id = mn_sender_id;
                    }
                    break;
                case MN_GPS_READ_STATUS_REQ:
                    if(GPS_STATUS_OPEN == GPS_GetStatus())
                    {
                        data_Size = GPS_ReadInfoData(&read_buf_ptr, byte_to_read);
                        if(!data_Size)
                        {
                            status = MN_GPS_NOT_FIX;
                        }
                        else
                        {
                            gps_buf_ptr = (Ge2PositionParam_t *)read_buf_ptr;
                            status = (MN_GPS_STATUS_E)(gps_buf_ptr->fix_mode);
                        }
                    }
                    else
                    {
                        status = MN_GPS_NOT_RUN;
                    }
                    {
                        MN_GPS_STATUS_CNF_T *signal = PNULL;

                        SCI_CREATE_SIGNAL(
                        signal,
                        MN_GPS_READ_STATUS_CNF,
                        sizeof (MN_GPS_STATUS_CNF_T),
                        SCI_IDENTIFY_THREAD());

                        signal->status = status;

                        SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, s_sender_id); 
                    }
                    break;
                case MN_GPS_READ_INFO_REQ:
                    data_Size = GPS_ReadInfoData(&read_buf_ptr, byte_to_read);
                    {
                        MN_GPS_READ_INFO_CNF_T *signal = PNULL;

                        SCI_CREATE_SIGNAL(
                        signal,
                        MN_GPS_READ_INFO_CNF,
                        sizeof (MN_GPS_READ_INFO_CNF_T),
                        SCI_IDENTIFY_THREAD());
                        if(data_Size > 0)
                        {
                            gps_buf_ptr = (Ge2PositionParam_t *)read_buf_ptr;
                            signal->gps_info.longitude = gps_buf_ptr->pos.longitude;
                            signal->gps_info.latitude = gps_buf_ptr->pos.latitude;
                            signal->gps_info.altitude= gps_buf_ptr->pos.altitude;
                            signal->gps_info.utc_time.wYear = gps_buf_ptr->time.wYear;
                            signal->gps_info.utc_time.wMonth = gps_buf_ptr->time.wMonth;
                            signal->gps_info.utc_time.wDay = gps_buf_ptr->time.wDay;
                            signal->gps_info.utc_time.wHour = gps_buf_ptr->time.wHour;
                            signal->gps_info.utc_time.wMinute = gps_buf_ptr->time.wMinute;
                            signal->gps_info.utc_time.wSecond = gps_buf_ptr->time.wSecond;
                            signal->gps_info.ttff_time = gps_buf_ptr->ttff;
                            signal->gps_info.satellite_num = gps_buf_ptr->pos.satellites;
                            signal->gps_info.speed = gps_buf_ptr->velocity.horizontalSpeed;
                            signal->gps_info.course = gps_buf_ptr->velocity.bearing;

                        }
                        SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, s_sender_id); 
                    }
                    break;
                case MN_GPS_OUTPUT_REQ:
                    output_mode = ((MN_GPS_OUTPUT_REQ_T *)signal_req)->type;
                    time_interval = ((MN_GPS_OUTPUT_REQ_T *)signal_req)->time_interval;
                    err_val = GPS_SetOutputMode(output_mode,time_interval,s_sender_id);
                    {
                        MN_GPS_OUTPUT_CNF_T *signal = PNULL;

                        SCI_CREATE_SIGNAL(
                        signal,
                        MN_GPS_OUTPUT_CNF,
                        sizeof (MN_GPS_OUTPUT_CNF_T),
                        SCI_IDENTIFY_THREAD());
                        if(GPS_ERR_NONE == err_val)
                            signal->is_ok= SCI_TRUE;
                        else
                            signal->is_ok= SCI_FALSE;

                        SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, s_sender_id); 
                    }
                    break;
                case MN_GPS_PDP_INFO_REQ:
                    agps_dual_sys = ((MN_GPS_PDP_INFO_REQ_T *)signal_req)->dual_sys;
                    agps_netid = ((MN_GPS_PDP_INFO_REQ_T *)signal_req)->netid;
		    {
			 #ifdef GPS_AGPS_SUPPORT_QX
			 GPS_AGPSDATA_INIT_T *signal = PNULL;

	                 SCI_CREATE_SIGNAL(
	                        signal,
	                        GPS_AGPSDATA_INIT_REQ,
	                        sizeof (GPS_AGPSDATA_INIT_T),
	                        SCI_IDENTIFY_THREAD());
	                        SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, AGPS_GetInterfaceThreadId());
			 #endif
                    }	
                    break;
                case MN_GPS_POWEROFF_REQ:
                    err_val = GPS_PowerOff();
                    {
                        MN_GPS_POWEROFF_CNF_T *signal = PNULL;

                        SCI_CREATE_SIGNAL(
                        signal,
                        MN_GPS_POWEROFF_CNF,
                        sizeof (MN_GPS_POWEROFF_CNF_T),
                        SCI_IDENTIFY_THREAD());
                        if(GPS_ERR_NONE == err_val)
                            signal->is_ok= SCI_TRUE;
                        else
                            signal->is_ok= SCI_FALSE;

                        SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, s_sender_id); 
                    }
                    break;
                case GPS_GET_EPHDATA_REQ:
		    s_sender_id = mn_sender_id;
                    GPS_Send_EphData();
                    break;
                case GPS_FIX_SUCCESS_REQ:
		    s_sender_id = mn_sender_id;		
                    {
                         MN_GPS_FIX_IND_T *signal = PNULL;
			
                        SCI_CREATE_SIGNAL(
                        signal,
                        MN_GPS_FIX_IND,
                        sizeof (MN_GPS_FIX_IND_T),
                        SCI_IDENTIFY_THREAD());

                        data_Size = GPS_ReadInfoData(&read_buf_ptr, byte_to_read);

                        if(data_Size > 0)
                        {
                            gps_buf_ptr = (Ge2PositionParam_t *)read_buf_ptr;
                            signal->longitude = gps_buf_ptr->pos.longitude;
                            signal->latitude = gps_buf_ptr->pos.latitude;
                            signal->ttff_time = gps_buf_ptr->ttff;
                        }
                        SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, mn_sender_id); 
                    }
		    {
			#ifdef GPS_AGPS_SUPPORT_QX	
			GPS_REPORT_AGPSDATA_T *signal = PNULL;

	                 SCI_CREATE_SIGNAL(
	                        signal,
	                        GPS_REPORT_AGPSDATA_REQ,
	                        sizeof (GPS_REPORT_AGPSDATA_T),
	                        SCI_IDENTIFY_THREAD());
	                        SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, AGPS_GetInterfaceThreadId());
			#endif
                    }
		    break;
		case GPS_WATCHDOG_REQ:
			s_sender_id = mn_sender_id;
			GPS_Send_Watchdog();
			break;
		case GPS_CREATE_DUMP_REQ:
			s_sender_id = mn_sender_id;
#ifdef GPS_TCARD_LOG_SUPPORT
    			Gps_CardLogClose(GPS_TCARDLOG_TYPE_ALL);
#endif
			err_val = GPS_Download_Fdl();
			if(err_val != GPS_ERR_NONE)
			{
			    xSignalHeaderRec *signal = NULL;
			    GE2GPS_LOGD("GreenEye2 fdl download err occurs");
			    SCI_CREATE_SIGNAL(
			        signal,
			        MN_GPS_ERR_IND,
			        sizeof (xSignalHeaderRec),
			        SCI_IDENTIFY_THREAD());
			    SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, s_sender_id);
			}
			break;
		case MN_GPS_ASSERT_REQ:
			GPS_Set_Assert(((MN_GPS_ASSERT_REQ_T *)signal_req)->type);
			break;
		case GPS_ENTER_SLEEP_REQ:
			s_sender_id = mn_sender_id;
			GPS_Enter_Sleep();
			break;
		case GPS_FACTORY_INIT_REQ:
			fac_init_count++;
			GPS_Init_Greeneye2();
			break;
		case GPS_FACTORY_RESTART_REQ:
			fac_restart_count++;
                    mode = ((MN_GPS_RESET_REQ_T *)signal_req)->reset_mode;
                    err_val = GPS_SetRestart(mode);
			break;
                default:
                    break;
            }
	gps_interface_record_debug(0xFF);
        SCI_FREE(signal_req);
        signal_req = NULL;
    }while(1);
}

/*****************************************************************************/
//  Description:    GPS_Start_Interface_Thread
//  Author:         
//  Note:   
/*****************************************************************************/
LOCAL void GPS_Start_Interface_Thread(void)
{
	
    s_gps_interface_thread_id = SCI_CreateThread("T_GPS_INTERFACE_SCHED", "Q_GPS_INTERFACE_SCHED", GPS_Interface_Task_Entry, 0, NULL, GPS_INTERFACE_THREAD_STACK_SIZE, 
									GPS_INTERFACE_THREAD_QUEUE_NUM, s_gps_interface_thread_task_priority, SCI_PREEMPT, SCI_AUTO_START);

    SCI_ASSERT(SCI_INVALID_BLOCK_ID != s_gps_interface_thread_id);

#ifdef GPS_AGPS_SUPPORT_QX

    s_agps_interface_thread_id = SCI_CreateThread("T_AGPS_INTERFACE_SCHED", "Q_AGPS_INTERFACE_SCHED", AGPS_Interface_Task_Entry, 0, NULL, AGPS_INTERFACE_THREAD_STACK_SIZE, 
									AGPS_INTERFACE_THREAD_QUEUE_NUM, s_agps_interface_thread_task_priority, SCI_PREEMPT, SCI_AUTO_START);
	
    SCI_ASSERT(SCI_INVALID_BLOCK_ID != s_agps_interface_thread_id);
#endif	
}

/*****************************************************************************/
//  Description:    GreenEye2_GetNmeaData 
//  Author:         
//  Note:           
/*****************************************************************************/
GPS_ERR_E GPS_GetNmeaData(uint32 mode,uint8 *buff)
{
    BOOLEAN ret;
    if(PNULL != s_gps_operations_ptr->gps_getnmeadata)
    {
        ret = s_gps_operations_ptr->gps_getnmeadata(mode,buff);
        if(ret)
            return GPS_ERR_NONE;
        else
            return GPS_ERR_NO_MODULE;
    }
    
    return GPS_ERR_NO_MODULE;
}

/*****************************************************************************/
//  Description:    GreenEye2_SetOutputMode   
//  Author:         
//  Note:           
/*****************************************************************************/
GPS_ERR_E GPS_SetOutputMode(uint32 mode,uint32 time_interval,BLOCK_ID sender_id)
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;
    if(PNULL != s_gps_operations_ptr->gps_getnmeadata)
    {
        err_val = s_gps_operations_ptr->gps_setoutputmode(mode,time_interval,sender_id);
    }
    return err_val;
}

/*****************************************************************************/
//  Description:    This function is used to set GPS module status    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _GPS_SetStatus(GPS_STATUS_E status)
{	
	SCI_TRACE_LOW("[GE2]_GPS_SetStatus status=%d",status);
	s_gps_status = status;		
}

/*****************************************************************************/
//  Description:    This function is used to get GPS status    
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC GPS_STATUS_E GPS_GetStatus(void)
{
	SCI_TRACE_LOW("[GE2]GPS_GetStatus s_gps_status=%d",s_gps_status);
	return s_gps_status;
}

/*****************************************************************************/
//  Description:    This function is used to init GPS mode and global variable    
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Init(void)
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;

    if(PNULL != s_gps_operations_ptr)
    {		
        // must Identify 
        if(PNULL != s_gps_operations_ptr->gps_identify)
        {
            if(s_gps_operations_ptr->gps_identify())
            {
                err_val = GPS_ERR_NONE;						
            }
        }
        else
        {
            SCI_PASSERT(0, ("NO GPS identify!"));   /*assert verified*/
        }		
    }
    else
    {	
        // Second: to find the right operation in the table
        s_gps_operations_ptr = *(GPS_OPERATIONS_T**)GPS_GetOperationTab();

        while(PNULL != s_gps_operations_ptr)
        {		
            // must Identify 
            if(PNULL != s_gps_operations_ptr->gps_identify)
            {
                if(s_gps_operations_ptr->gps_identify())
                {					
                    err_val = GPS_ERR_NONE;
                    break;			
                }
            }
            else
            {
                SCI_PASSERT(0, ("NO GPS identify!"));   /*assert verified*/
            }		

            // Find next operation
            s_gps_operations_ptr++;
        }	
    }

    GPS_Interface_Task_Init();
    _GPS_SetStatus(GPS_STATUS_POWEROFF);
    GPS_DIAG_RegDiagCmdRoutine();

    return err_val;
}

/*****************************************************************************/
//  Description:    GPS_Download    
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Download(void)
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;

    if(GPS_STATUS_POWEROFF != GPS_GetStatus())
    {
        return err_val;
    }

     //call download
    if(PNULL != s_gps_operations_ptr->gps_download)
    {
        err_val = s_gps_operations_ptr->gps_download();
    }

    if(GPS_ERR_NONE != err_val)
    {
        _GPS_SetStatus(GPS_STATUS_POWEROFF);
        //SCI_PASSERT(0, ("GPS download fail!"));
    }
    else
    {
	_GPS_SetStatus(GPS_STATUS_NOT_INIT);
    }
    return err_val;
}

/*****************************************************************************/
//  Description:    GPS_Download_Fdl    
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Download_Fdl(void)
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;

     //call download
    if(PNULL != s_gps_operations_ptr->gps_downloadfdl)
    {
        err_val = s_gps_operations_ptr->gps_downloadfdl();
    }

    return err_val;
}

/*****************************************************************************/
//  Description:    GPS_Init_Greeneye2    
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Init_Greeneye2()
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;
    
    if(GPS_STATUS_NOT_INIT != GPS_GetStatus())
    {
        return err_val;
    }

    _GPS_SetStatus(GPS_STATUS_INIT);		

    //call init
    if(PNULL != s_gps_operations_ptr->gps_init)
    {
        err_val = s_gps_operations_ptr->gps_init();		
    }

    if(GPS_ERR_NONE != err_val)
    {
        _GPS_SetStatus(GPS_STATUS_NOT_INIT);
        //SCI_PASSERT(0, ("GPS init fail!"));
    }

    return err_val;
}

/*****************************************************************************/
//  Description:    This function is used to open GPS module by the special mode    
//  Author:         
//  Note:           
//	input:			GPS_MODE_E
//	Output:			GPS_ERR_E
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Open()
{
	GPS_ERR_E err_val = GPS_ERR_NO_MODULE;	
        if(GPS_STATUS_OPEN == GPS_GetStatus())
        {
            return GPS_ERR_NO_MODULE;
        }
	if(PNULL != s_gps_operations_ptr->gps_open)
	{
		SCI_ASSERT(PNULL != s_gps_operations_ptr->gps_open);/*assert verified*/	
		err_val = s_gps_operations_ptr->gps_open(s_sender_id);
		if(GPS_ERR_NONE == err_val)
		{
			_GPS_SetStatus(GPS_STATUS_OPEN);
		}
	}
	
	return err_val;
}

/*****************************************************************************/
//  Description:    This function is used to Close GPS module    
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Close(void)
{
	GPS_ERR_E err_val = GPS_ERR_NO_MODULE;
        if(GPS_STATUS_CLOSE == GPS_GetStatus())
        {
            return GPS_ERR_NO_MODULE;
        }
	if(PNULL != s_gps_operations_ptr->gps_close)
	{
		SCI_ASSERT(PNULL != s_gps_operations_ptr->gps_close);/*assert verified*/
		err_val = s_gps_operations_ptr->gps_close();
		if(GPS_ERR_NONE == err_val)
		{
			_GPS_SetStatus(GPS_STATUS_CLOSE);
		}
	}
	
	return err_val;
}

/*****************************************************************************/
//  Description:    GPS_SetRestart  
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC GPS_MODE_E GPS_SetRestart(uint32 mode)
{
    	GPS_ERR_E err_val = GPS_ERR_NO_MODULE;
	
	if(PNULL != s_gps_operations_ptr->gps_setrestart)
	{
		SCI_ASSERT(PNULL != s_gps_operations_ptr->gps_setrestart);/*assert verified*/
		err_val = s_gps_operations_ptr->gps_setrestart(mode);
                if(GPS_ERR_NONE == err_val)
                {
                    _GPS_SetStatus(GPS_STATUS_OPEN);
                }
	}
	
	return err_val;
}

/*****************************************************************************/
//  Description:    GPS_PowerOff  
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC GPS_MODE_E GPS_PowerOff()
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;

    if(PNULL != s_gps_operations_ptr->gps_poweroff)
    {
        err_val = s_gps_operations_ptr->gps_poweroff();
	if(GPS_ERR_NONE == err_val)	
	{
		_GPS_SetStatus(GPS_STATUS_POWEROFF);
	}
    }

    return err_val;
}

/*****************************************************************************/
//  Description:    GPS_ReadInfoData  
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPS_ReadInfoData(char **read_buf_ptr, uint32 byte_to_read)
{
    uint32 data_size = 0;
    if(PNULL != s_gps_operations_ptr->gps_readinfodata)
    {
        data_size = s_gps_operations_ptr->gps_readinfodata(read_buf_ptr,byte_to_read);
    }
    return  data_size;
}

/*****************************************************************************/
//  Description:    This function is used to read data from GPS module    
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPS_ReadData(uint8 * read_buf_ptr, uint32 byte_to_read)
{
    uint32 data_size = 0;
    if(PNULL != s_gps_operations_ptr->gps_readdata)
    {
        data_size = s_gps_operations_ptr->gps_readdata(read_buf_ptr,byte_to_read);
    }
    return  data_size;
	
}

/*****************************************************************************/
//  Description:    This function is used to write data to GPS module    
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPS_WriteData(uint8 * write_buf_ptr, uint32 byte_to_write)
{
    return 0;
}

/*****************************************************************************/
//  Description:    This function is used to write agps data to GPS module    
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC GPS_MODE_E GPS_WriteAgpsData(char *agps_data, uint32 length)
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;
    
    if(PNULL != s_gps_operations_ptr->gps_writeagpsdata)
    {
	err_val = s_gps_operations_ptr->gps_writeagpsdata(agps_data,length);
    }
    return err_val;
}

/*****************************************************************************/
//  Description:    This function is used to get information about GPS engine and MAP    
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC uint8* GPS_GetAgpsNVInfo(void)
{
	 uint32  status = 0;
	 uint8 data_buff[0xc00];
    
    // Read REF parameter initial values from NVItem.
    status = EFS_NvitemRead(NV_GPS_AGPS_PARAM, 0xc00, (uint8 *)(data_buff));    
    // If the initial values don't exist.
    if (NVERR_NOT_EXIST == status)
    {
    	 SCI_PASSERT(0, ("[GE2]Please download new NVItem !"));   /*assert verified*/
    }
    
    return data_buff;
}

/*****************************************************************************/
//  Description:    This function is used to write information about GPS engine and MAP to nv
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPS_WriteAgpsNVInfo(uint8* data_buff)
{
	uint32 status = 0;
	status = EFS_NvitemWrite(NV_GPS_AGPS_PARAM, 0xc00, (uint8 *)(data_buff), 0);
	 if(NVERR_NONE == status)
	 	return SCI_TRUE;
	 else
	 	return SCI_FALSE;
}

/*****************************************************************************/
//  Description:    GPS_Partition
//  Author:        
//  Note:           
/*****************************************************************************/
LOCAL int GPS_Partition(MN_PHONE_CELL_MEAS_T *data,int low,int high)
{
    MN_PHONE_CELL_MEAS_T cell_data;
    cell_data = data[low];
    while(low < high)
    { 
	while(low < high && data[high].rxlev<= cell_data.rxlev)
		high--;
	
	data[low] = data[high];
	
	while(low < high && data[low].rxlev >= cell_data.rxlev)
		low++;
	
	data[high] = data[low];
    }
    data[low] = cell_data;
    return low;
}

/*****************************************************************************/
//  Description:    GPS_Quicksort
//  Author:        
//  Note: //快排每趟进行时的枢轴要重新确定，由此进 //一步确定每个待排小记录的low及high的值
/*****************************************************************************/
LOCAL void GPS_Quicksort(MN_PHONE_CELL_MEAS_T *data,int low,int high) 
{
    int pivotloc = 0;

    if(low >= high)
	return ;
    pivotloc = GPS_Partition(data,low,high);
    GPS_Quicksort(data,low,pivotloc-1);
    GPS_Quicksort(data,pivotloc+1,high);
}

#ifdef GPS_AGPS_SUPPORT_QX
/*****************************************************************************/
//  Description:    AGPS_GetInterfaceThreadId
//  Author:         
//  Note:   
/*****************************************************************************/
PUBLIC BLOCK_ID AGPS_GetInterfaceThreadId()
{
    return s_agps_interface_thread_id;
}

/*****************************************************************************/
//  Description:    AGPS_Interface_Task_Entry
//  Author:        
//  Note:           
/*****************************************************************************/
LOCAL void AGPS_Interface_Task_Entry(uint32 argc, void *argv)
{
    xSignalHeader signal_req = NULL;
    
    do
    {

	signal_req = SCI_GetSignal (s_agps_interface_thread_id);
	agps_interface_record_debug(signal_req->SignalCode);
        switch((GPS_INTERFACE_REQ_E)(signal_req->SignalCode))
        {
        	case GPS_AGPSDATA_INIT_REQ:
		    GPS_QXAgps_Init();
		    break;
                case GPS_GET_AGPSDATA_REQ:
                    GPS_RequestDataFrom_Agps();
                    break;
		case GPS_REPORT_AGPSDATA_REQ:
		    GPS_ReportDataTo_Agps();
		    break;
		default:
			 break;
        }
	agps_interface_record_debug(0xFF);
	SCI_FREE(signal_req);
        signal_req = NULL;
    }while(1);
}

/*****************************************************************************/
// Description : This function convert the bcd code to digital string
// Global resource dependence : none
// Author:      
// Note:
/*****************************************************************************/
void GPS_ConvertBcdToDigitalStr(
                uint8         length,
                uint8         *bcd_ptr,      // in: the bcd code string
                uint8         *digital_ptr   // out: the digital string
                )
{
    int32               i = 0;
    uint8         temp = 0;

    // get the first digital
    temp = ((*bcd_ptr >> 4) &0x0f);
    if (temp >= 0x0a)
    {
        *digital_ptr = (temp - 0x0a) + 'A';
    }
    else
    {
        *digital_ptr = temp + '0';
    }
    
    bcd_ptr++;
    digital_ptr++;


    for (i=0; i<(length - 1); i++)
    {
        temp = *bcd_ptr;
        // get the low 4 bits
        temp &= 0x0f;
        // A -- F
        if (temp >= 0x0a)
        {
            *digital_ptr = (temp - 0x0a) + 'A';
        }
        else
        {
            // 1 -- 9
            *digital_ptr = temp + '0';
        }
        digital_ptr++;

        temp = *bcd_ptr;
        // get the high 4 bits
        temp = (temp & 0xf0) >> 4;  
        
        if ((temp == 0x0f) && (i == (length -1)))
        {
            *digital_ptr = '\0';    
            return;
        }
        // A -- F
        else if (temp>=0x0a)
        {
            *digital_ptr = (temp - 0x0a) + 'A';
        }
        else
        {
            // 1 -- 9
            *digital_ptr = temp + '0';
        }
        digital_ptr++;
        bcd_ptr++;
    }
    *digital_ptr = '\0';
}

/*****************************************************************************/
//  Description:    GPS_GetPlmn_Status
//  Author:        
//  Note:           
/*****************************************************************************/
 LOCAL BOOLEAN GPS_GetPlmn_Status()
{

    MN_PHONE_CURRENT_PLMN_INFO_T plmn_info;

    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(agps_dual_sys);

    if(PLMN_NO_SERVICE == plmn_info.plmn_status)
    {
    	SCI_TRACE_LOW("[GE2][AGPS]plmn no service");
	return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description:    GPS_GetPdp_Status
//  Author:        
//  Note:           
/*****************************************************************************/
 LOCAL BOOLEAN GPS_GetPdp_Status()
{
    MN_GPRS_CON_STATE_T *pdp_con_state_arr  = PNULL;
    ERR_MNGPRS_CODE_E    ret     = ERR_MNGPRS_NO_ERR;
    uint32               pdp_con_num = 0;
    uint32               i;    

    ret = MNGPRS_ReadPdpContextStateEx(agps_dual_sys, &pdp_con_state_arr, &pdp_con_num);
    if(ret != ERR_MNGPRS_NO_ERR)
    {
        if(pdp_con_state_arr != PNULL)
        {
            SCI_FREE(pdp_con_state_arr);
            pdp_con_state_arr = PNULL;
        }
        SCI_TRACE_LOW("[GE2][AGPS]no pdp is activated ret %d", ret);
        return FALSE;
    }
    
    for(i = 0; i < pdp_con_num; i++)
    {
        if(pdp_con_state_arr[i].pdp_state == MN_CONTEXT_ACTIVATED)
        {
            break;
        }
    }

    if(pdp_con_state_arr != PNULL)
    {
        SCI_FREE(pdp_con_state_arr);
        pdp_con_state_arr = PNULL;
    }

    if (i == pdp_con_num)
    {
        SCI_TRACE_LOW("[GE2][AGPS]no pdp is activated");
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}

/*****************************************************************************/
//  Description:    GPS_QXAgps_Init
//  Author:        
//  Note:           
/*****************************************************************************/
LOCAL void GPS_QXAgps_Init()
{
    int ret = 0;
    uint8 imsi_str[20];

    SCI_MEMSET(&config,0,sizeof(QX_user_config_t));

    strcpy(config.appKey,"10207");
    strcpy(config.secret,"c0bbff81a7bef4ab713402903935ed5295dda848450f4951fa12b9bb64aaee94");

    agps_imsi_str = MNSIM_GetImsiEx(agps_dual_sys);
    GPS_ConvertBcdToDigitalStr(agps_imsi_str.imsi_len, agps_imsi_str.imsi_val, imsi_str);
    strcpy(config.deviceId,imsi_str);
    SCI_TRACE_LOW("[GE2][AGPS]Request imei_str=%s,deviceId=%s\n",imsi_str,config.deviceId);	
    //strcpy(config.deviceId,"sdk_mt2503_3");
    //strcpy(config.deviceType,"sdk_mt2503");
    config.deviceType = DID_IMSI;
    config.timeout = 7000;
    //config.netId = 0x5;
    config.netId = agps_netid;

    //SCI_TRACE_LOW("[GE2][AGPS]Request agps_netid=%d,agps_dual_sys=%d\n",agps_netid,agps_dual_sys);
    
    ret = QX_supl_sdk_initialize(&config);
    SCI_TRACE_LOW("[GE2][AGPS]Request QX_supl_sdk_initialize ret=%d\n",ret);
    if(ret == 0)
    {
    	gps_agps_init_flag = SCI_TRUE;
    }
}

/*****************************************************************************/
//  Description:    GPS_QXAgps_Release
//  Author:        
//  Note:           
/*****************************************************************************/
LOCAL void GPS_QXAgps_Release()
{
    int ret = 0;

    ret = QX_supl_sdk_release();
    SCI_TRACE_LOW("[GE2][AGPS]Request QX_supl_sdk_release ret=%d\n",ret);
    if(ret == 0)
    {
    	    GPS_WriteAgpsData((char *)(&response),sizeof(QX_agnssdata_client_t));
    }

}

/*****************************************************************************/
//  Description:    GPS_RequestDataFrom_Agps
//  Author:        
//  Note:           
/*****************************************************************************/
LOCAL uint32 GPS_RequestDataFrom_Agps()
{
    int ret = 0;
#ifdef GPS_MODE_GPS_BD
    int reqType=QX_REQ_REF_TIME|QX_REQ_GPS_EPH|QX_REQ_BDS_EPH|QX_REQ_REF_LOC;
#else
    int reqType=QX_REQ_REF_TIME|QX_REQ_GPS_EPH|QX_REQ_GLO_EPH|QX_REQ_REF_LOC;
#endif
    MN_PHONE_CELL_MEAS_T           scell;
    int i = 0;
    int j = 0;

    SCI_TRACE_LOW("[GE2][AGPS]GPS_RequestDataFrom_Agps");

   if(!gps_agps_init_flag)
   {
   	SCI_TRACE_LOW("[GE2][AGPS]AGPS Not Init");
	return 0;
   }

   if(!GPS_GetPlmn_Status() || !GPS_GetPdp_Status())
   {
   	SCI_TRACE_LOW("[GE2][AGPS]gprs can not use!");
	return 0;
   }

    gps_report_agps_flag = SCI_FALSE;

    SCI_MEMSET(&request,0,sizeof(QX_report_data_t));
    SCI_MEMSET(&response,0,sizeof(QX_agnssdata_client_t));
    MNPHONE_GetMeasReportEx(agps_dual_sys, &ncell, &scell);

    GPS_Quicksort(ncell.ncell_power,0,MN_MAX_NCELL_NUM-1);

    for(i = 0;i < (MN_MAX_NCELL_NUM - 1);i ++)
    {
    	if(0 != ncell.ncell_power[i].cell_id)
    	{
	    	for(j = (i+1);j < MN_MAX_NCELL_NUM;j ++)
	    	{
	    		if(ncell.ncell_power[i].cell_id == ncell.ncell_power[j].cell_id)
					ncell.ncell_power[j].cell_id = 0;
	    	}
    	}
    }

    i = 0;
    j = 0;
    for(i = 0;i < (MN_MAX_NCELL_NUM - 1);i ++)
    {
    	if(0 != ncell.ncell_power[i].cell_id)
    	{
		request.nonCDMACell[j].cellType = LOCID_TYPE_GSM;
		request.nonCDMACell[j].cid = ncell.ncell_power[i].cell_id;
		request.nonCDMACell[j].lac = ncell.ncell_power[i].lai.lac;
		request.nonCDMACell[j].mcc = ncell.ncell_power[i].lai.mcc;
		request.nonCDMACell[j].mnc = ncell.ncell_power[i].lai.mnc;
		j++;
    	}
    }
    request.nonCDMACellNum = j;

    request.locType = LOCID_TYPE_GSM;
    
    ret = QX_supl_sdk_get_agnss_data(reqType,&request,&response);
    SCI_TRACE_LOW("[GE2][AGPS]Request QX_supl_sdk_get_agnss_data ret=%d\n",ret);

    if(ret == 0)
    {
    	    GPS_WriteAgpsData((char *)(&response),sizeof(QX_agnssdata_client_t));
    }


    gps_report_agps_flag = SCI_TRUE;

    return 1;
}

/*****************************************************************************/
//  Description:    GPS_ReportDataTo_Agps
//  Author:        
//  Note:           
/*****************************************************************************/
LOCAL uint32 GPS_ReportDataTo_Agps()
{
    int ret = 0;
    int reqType = QX_REQ_REPORT_LOCATION;
    Ge2PositionParam_t *gps_buf_ptr = NULL;
    char *read_buf_ptr = NULL;
    uint32 data_Size = 0;
    uint32 byte_to_read = 1024;
    int i = 0;
    int j = 0;

    SCI_TRACE_LOW("[GE2][AGPS]GPS_ReportDataTo_Agps\n");	

    if(!gps_report_agps_flag)
    return 0;

    gps_report_agps_flag = SCI_FALSE;

   if(!GPS_GetPlmn_Status() || !GPS_GetPdp_Status())
   {
   	SCI_TRACE_LOW("[GE2][AGPS]gprs can not use!");
	return 0;
   }

    SCI_MEMSET(&request,0,sizeof(QX_report_data_t));
    SCI_MEMSET(&response,0,sizeof(QX_agnssdata_client_t));

    /**
    * 上报的位置信息
    */
    request.lla.valid = 1;          /* 填写1 表示已经设置               */
    data_Size = GPS_ReadInfoData(&read_buf_ptr, byte_to_read);

    if(data_Size > 0)
    {
        gps_buf_ptr = (Ge2PositionParam_t *)read_buf_ptr;
        request.lla.lat = gps_buf_ptr->pos.latitude;
        request.lla.lon = gps_buf_ptr->pos.longitude;
        request.lla.height = gps_buf_ptr->pos.altitude;
    }
    request.lla.unc = 25;            /* 填写设备当前位置不确定度,填1即可 */

    /**
    * 与位置相关的附近cell信息
    */
    j  = 0;
    for(i = 0;i < (MN_MAX_NCELL_NUM);i ++)
    {
    if(0 != ncell.ncell_power[i].cell_id)
    {
    	request.nonCDMACell[j].cellType = LOCID_TYPE_GSM;
    	request.nonCDMACell[j].cid = ncell.ncell_power[i].cell_id;
    	request.nonCDMACell[j].lac = ncell.ncell_power[i].lai.lac;
    	request.nonCDMACell[j].mcc = ncell.ncell_power[i].lai.mcc;
    	request.nonCDMACell[j].mnc = ncell.ncell_power[i].lai.mnc;
    	j++;
    }
    }

    request.nonCDMACellNum = j;

    request.locType = LOCID_TYPE_GSM;

    ret = QX_supl_sdk_get_agnss_data(reqType,&request,&response);
    SCI_TRACE_LOW("[GE2][AGPS]Report QX_supl_sdk_get_agnss_data ret=%d\n",ret);

    return 1;
}
#else
/*****************************************************************************/
//  Description:    GPS_RequestDataFrom_Agps
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPS_RequestDataFrom_Agps()
{
    return 0;
}

/*****************************************************************************/
//  Description:    GPS_ReportDataTo_Agps
//  Author:        
//  Note:           
/*****************************************************************************/
LOCAL uint32 GPS_ReportDataTo_Agps()
{
    return 0;
}
#endif
/*****************************************************************************/
//  Description:    Enable debug log output to diag
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC GPS_MODE_E GPS_DebugLog_Enable(uint8 status)
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;

    if(PNULL != s_gps_operations_ptr->gps_debuglogenable)
    {
        err_val = s_gps_operations_ptr->gps_debuglogenable(status);
    }
    return err_val;
}

/*****************************************************************************/
//  Description:    enable nmea log output to diag
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC GPS_MODE_E GPS_NmeaLog_Enable(uint8 status)
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;

    if(PNULL != s_gps_operations_ptr->gps_nmealogenable)
    {
        err_val = s_gps_operations_ptr->gps_nmealogenable(status);
    }

    return err_val;
}

/*****************************************************************************/
//  Description:    GPS_Set_RFTestMode
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC GPS_MODE_E GPS_Set_RFTestMode(GPS_RF_TEST_MODE_E rf_mode)
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;

    if(PNULL != s_gps_operations_ptr->gps_setrftestmode)
    {
        err_val = s_gps_operations_ptr->gps_setrftestmode(rf_mode);
    }

    return err_val;
}

/*****************************************************************************/
//  Description:    enable test mode
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC GPS_MODE_E GPS_TestMode_Enable(uint8 status)
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;

    if(PNULL != s_gps_operations_ptr->gps_testmodeenable)
    {
        err_val = s_gps_operations_ptr->gps_testmodeenable(status);
    }

    return err_val;
}

/*****************************************************************************/
//  Description:    get gps fix status
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC uint8 GPS_Get_FixStatus()
{
    uint32 data_Size = 0;
    Ge2PositionParam_t *gps_buf_ptr = NULL;
    char *read_buf_ptr = NULL;
    uint32 byte_to_read = 1024;
    MN_GPS_STATUS_E fixstatus = 0;

    data_Size = GPS_ReadInfoData(&read_buf_ptr, byte_to_read);
    if(!data_Size)
    {
        fixstatus = MN_GPS_NOT_FIX;
    }
    else
    {
        gps_buf_ptr = (Ge2PositionParam_t *)read_buf_ptr;
        fixstatus = (MN_GPS_STATUS_E)(gps_buf_ptr->fix_mode);
    }

    return fixstatus;
}

/*****************************************************************************/
//  Description:    GPS_Send_EphData
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Send_EphData()
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;

    if(PNULL != s_gps_operations_ptr->gps_sendephdata)
    {
        err_val = s_gps_operations_ptr->gps_sendephdata();
    }

    return err_val;
}

/*****************************************************************************/
//  Description:    GPS_Send_Watchdog
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Send_Watchdog()
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;

    if(PNULL != s_gps_operations_ptr->gps_sendwatchdog)
    {
        err_val = s_gps_operations_ptr->gps_sendwatchdog();
    }

    return err_val;
}

/*****************************************************************************/
//  Description:    GPS_Clear_EphData
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Clear_EphData()
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;

    if(PNULL != s_gps_operations_ptr->gps_clearephdata)
    {
        err_val = s_gps_operations_ptr->gps_clearephdata();
    }

    return err_val;
}

/*****************************************************************************/
//  Description:    GPS_Clear_AlmData
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Clear_AlmData()
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;

    if(PNULL != s_gps_operations_ptr->gps_clearalmdata)
    {
        err_val = s_gps_operations_ptr->gps_clearalmdata();
    }

    return err_val;
}


/*****************************************************************************/
//  Description:    get gps fix satellites
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC uint8 GPS_Get_FixSatellites()
{
    uint32 data_Size = 0;
    Ge2PositionParam_t *gps_buf_ptr = NULL;
    char *read_buf_ptr = NULL;
    uint32 byte_to_read = 1024;
    uint8 satellites = 0;

    data_Size = GPS_ReadInfoData(&read_buf_ptr, byte_to_read);
    if(!data_Size)
    {
        satellites = 0x0;
    }
    else
    {
        gps_buf_ptr = (Ge2PositionParam_t *)read_buf_ptr;
        satellites = (MN_GPS_STATUS_E)(gps_buf_ptr->pos.satellites);
    }

    return satellites;
}

/*****************************************************************************/
//  Description:    get gps svinfo
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC void GPS_Get_Svinfo(GpsSvStatus *svinfo)
{
    if(PNULL != s_gps_operations_ptr->gps_getsvinfo)
    {
        s_gps_operations_ptr->gps_getsvinfo(svinfo);
    }
}

/*****************************************************************************/
//  Description:    GPS_Set_Assert
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Set_Assert(uint8 assertmode)
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;

    if(PNULL != s_gps_operations_ptr->gps_setassert)
    {
        s_gps_operations_ptr->gps_setassert(assertmode);
    }

    return err_val;
}

/*****************************************************************************/
//  Description:    GPS_Enter_Sleep
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Enter_Sleep()
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;

    if(PNULL != s_gps_operations_ptr->gps_entersleep)
    {
        s_gps_operations_ptr->gps_entersleep();
    }

    return err_val;
}

/*****************************************************************************/
//  Description:    GPS_Lte_Open
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC int GPS_Lte_Open(uint8 lte_open)
{
    GPS_ERR_E err_val = GPS_ERR_NO_MODULE;

    if(PNULL != s_gps_operations_ptr->gps_lteopen)
    {
        s_gps_operations_ptr->gps_lteopen(lte_open);
    }

    return err_val;
}

/*****************************************************************************/
//  Description:    GPS_Get_Uartport
//  Author:        
//  Note:           
/*****************************************************************************/
PUBLIC int GPS_Get_Uartport()
{
	return GPS_COM;
}

#else
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define GPS_SLEEP_TIME_MAX	(2*60*60*1000)	//ms, 在GPS模块处于sleep状态超过这个时间不唤醒，GPS就会丢失定位信息，进入LOST状态
#define	GPS_IDLE_TIME_MAX	(20 * 1000)		//ms, 在GPS模块处于idle状态超过这个时间不使用，就让GPS进入SLEEP状态
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
#ifndef _SP7560_P1_
	#define _GPS_CIPHER
#endif
/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL GPS_STATUS_E 		s_gps_status 			= GPS_STATUS_NOT_INIT;
LOCAL GPS_OPERATIONS_T*	s_gps_operations_ptr	= PNULL;
LOCAL uint32			s_gps_idle_time_count	= 0;
LOCAL uint32			s_gps_sleep_time_count  = 0;
LOCAL GPS_NV_INFO_T		s_gps_nv_info			= {0};
#ifdef _GPS_CIPHER
LOCAL GPS_CIPHER_REPORT_T s_gps_cipher			= {0};
#endif
LOCAL BOOLEAN 			s_is_get_gps_cipher		= SCI_FALSE;
LOCAL GPS_MODE_E		s_gps_mode				= GPS_MODE_MAX;
//LOCAL GPS_INFO_T		s_gps_info				= {0};

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to set GPS module idle time    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _GPS_SetIdleTime(void)
{
	s_gps_idle_time_count = SCI_GetTickCount();
}

/*****************************************************************************/
//  Description:    This function is used to get GPS module idle time   
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _GPS_GetIdleTime(void)
{
	return s_gps_idle_time_count;
}

/*****************************************************************************/
//  Description:    This function is used to set GPS module sleep time    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _GPS_SetSleepTime(void)
{
	s_gps_sleep_time_count = SCI_GetTickCount();
}

/*****************************************************************************/
//  Description:    This function is used to get GPS module sleep time    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _GPS_GetSleepTime(void)
{
	return s_gps_sleep_time_count;
}

/*****************************************************************************/
//  Description:    This function is used to set GPS module status    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _GPS_SetStatus(GPS_STATUS_E status)
{	
	s_gps_status = status;	
	if(GPS_STATUS_OPEN == status)
	{
		_GPS_SetIdleTime();
	}
	else if(GPS_STATUS_SLEEP == status)
	{
		_GPS_SetSleepTime();
	}	
	
}

/*****************************************************************************/
//  Description:    This function is used to manage GPS module power    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _GPS_PowerManager(void)
{
	uint32 time = 0;
	uint32 cur_time = SCI_GetTickCount();
	
	if(GPS_STATUS_OPEN == GPS_GetStatus())
	{
		time =  _GPS_GetIdleTime();
		if(cur_time > time)
		{
			time = cur_time - time;
		}
		else
		{
			time = 0xFFFFFFFF - time + cur_time;	
		}

		if(time >= GPS_IDLE_TIME_MAX)
		{
			GPS_EnterSleep(SCI_TRUE);	
		}
	}
	else if(GPS_STATUS_SLEEP == GPS_GetStatus())
	{
		time = _GPS_GetSleepTime();
		if(cur_time > time)
		{
			time = cur_time - time;
		}
		else
		{
			time = 0xFFFFFFFF - time + cur_time;	
		}

		if(time >= GPS_SLEEP_TIME_MAX)
		{
			_GPS_SetStatus(GPS_STATUS_LOST);
		}
	}
}

#ifdef _GPS_CIPHER
/*****************************************************************************/
//  Description:    This function is used to get GPS status    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _GPS_ValidationCallback(GPS_CIPHER_REPORT_T GPS_cipher)
{
	//GPS_TRACE:"_GPS_ValidationCallback: high_cipher 0x%08x, low_cipher 0x%08x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_DRV_171_112_2_18_0_33_7_1554,(uint8*)"dd", GPS_cipher.high_level, GPS_cipher.low_level);
	SCI_MEMCPY(&s_gps_cipher, &GPS_cipher, sizeof(GPS_CIPHER_REPORT_T));

	s_is_get_gps_cipher = SCI_TRUE;
}
#endif

/*****************************************************************************/
//  Description:    This function is used to get GPS mode    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL GPS_MODE_E _GPS_GetMode(void)
{
	return s_gps_mode;
}

/*****************************************************************************/
//  Description:    This function is used to get GPS mode    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _GPS_SetMode(GPS_MODE_E mode)
{
	s_gps_mode = mode;
	//GPS_TRACE:"_GPS_SetMode: s_gps_mode %d, mode %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_DRV_192_112_2_18_0_33_7_1555,(uint8*)"dd", s_gps_mode, mode);
}
/**---------------------------------------------------------------------------*
 **                     Global Function Prototypes                            *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to get GPS status    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC GPS_STATUS_E GPS_GetStatus(void)
{
	return s_gps_status;
}
/*****************************************************************************/
//  Description:    This function is used to init GPS mode and global variable    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Init(void)
{
	GPS_ERR_E err_val = GPS_ERR_NO_MODULE;
	
	if(PNULL != s_gps_operations_ptr)
	{		
		// must Identify 
		if(PNULL != s_gps_operations_ptr->gps_identify)
		{
			if(s_gps_operations_ptr->gps_identify())
			{
				err_val = GPS_ERR_NONE;						
			}
		}
		else
		{
			SCI_PASSERT(0, ("NO GPS identify!"));   /*assert verified*/
		}		
		
	}
	else
	{	
		// Second: to find the right operation in the table
		s_gps_operations_ptr = *(GPS_OPERATIONS_T**)GPS_GetOperationTab();
		
		while(PNULL != s_gps_operations_ptr)
		{		
			// must Identify 
			if(PNULL != s_gps_operations_ptr->gps_identify)
			{
				if(s_gps_operations_ptr->gps_identify())
				{					
					err_val = GPS_ERR_NONE;
					break;			
				}
			}
			else
			{
				SCI_PASSERT(0, ("NO GPS identify!"));   /*assert verified*/
			}		
			
			// Find next operation
			s_gps_operations_ptr++;
		}	
	}
	
	
	if(GPS_ERR_NONE != err_val)
	{
	    //GPS_TRACE:"GPS_Init: Identify fail !! status %d"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_DRV_255_112_2_18_0_33_7_1556,(uint8*)"d", err_val);
		
	}
	else
	{
		_GPS_SetStatus(GPS_STATUS_INIT);		
		//GPS_TRACE:"GPS_Init: Identify successful !!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_DRV_261_112_2_18_0_33_7_1557,(uint8*)"");
		
		//call init
		if(PNULL != s_gps_operations_ptr->gps_init)
		{
			s_gps_operations_ptr->gps_init();		
		}
	}	
	
	GPS_DIAG_RegDiagCmdRoutine();
	
	return err_val;
}

/*****************************************************************************/
//  Description:    This function is used to open GPS module by the special mode    
//  Author:         Liangwen.Zhen
//  Note:           
//	input:			GPS_MODE_E
//	Output:			GPS_ERR_E
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Open(GPS_MODE_E mode)
{
	GPS_ERR_E err_val = GPS_ERR_NONE;	
	
	if(PNULL != s_gps_operations_ptr)
	{
		SCI_ASSERT(PNULL != s_gps_operations_ptr->gps_open);/*assert verified*/	
		err_val = s_gps_operations_ptr->gps_open(mode);
		if(GPS_ERR_NONE == err_val)
		{
			_GPS_SetStatus(GPS_STATUS_OPEN);	
			//DoIdle_RegisterCallback(_GPS_PowerManager);
			if( GPS_MODE_TEST_NMEA == mode )
			{				
				NMEA_Init();
			}

			_GPS_SetMode(mode);
		}
	}
	else
	{
		err_val = GPS_ERR_NO_MODULE;
	}
	
	return err_val;
}

/*****************************************************************************/
//  Description:    This function is used to Close GPS module    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Close(void)
{
	GPS_ERR_E err_val = GPS_ERR_NONE;
	
	if(PNULL != s_gps_operations_ptr)
	{
		SCI_ASSERT(PNULL != s_gps_operations_ptr->gps_close);/*assert verified*/
		err_val = s_gps_operations_ptr->gps_close();
		if(GPS_ERR_NONE == err_val)
		{
			_GPS_SetStatus(GPS_STATUS_CLOSE);
		}

		if( GPS_MODE_TEST_NMEA == _GPS_GetMode() )
		{				
			NMEA_Close();
		}

		_GPS_SetMode( GPS_MODE_MAX );
	}
	else
	{
		err_val = GPS_ERR_NO_MODULE;
	}
	
	return err_val;
}

/*****************************************************************************/
//  Description:    This function is used to Close GPS module    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_EnterSleep(BOOLEAN is_sleep)
{
	GPS_ERR_E err_val = GPS_ERR_NONE;
	
	if(PNULL != s_gps_operations_ptr)
	{
		if((!is_sleep && (GPS_STATUS_SLEEP == GPS_GetStatus()))
		 || (!is_sleep && (GPS_STATUS_LOST == GPS_GetStatus()))
		 || (is_sleep && (GPS_STATUS_OPEN == GPS_GetStatus()))
		 )
		{
			SCI_ASSERT(PNULL != s_gps_operations_ptr->gps_sleep);/*assert verified*/
			err_val = s_gps_operations_ptr->gps_sleep(is_sleep);
			if(GPS_ERR_NONE == err_val)
			{
				if(is_sleep)
				{					
					_GPS_SetStatus(GPS_STATUS_SLEEP);					
				}
				else
				{
					if(GPS_STATUS_LOST == GPS_GetStatus())
					{
						SCI_ASSERT(PNULL != s_gps_operations_ptr->gps_reflash);/*assert verified*/
						s_gps_operations_ptr->gps_reflash();
					}					
					_GPS_SetStatus(GPS_STATUS_OPEN);
				}
			}
		}
	}
	
	return err_val;
	
}

/*****************************************************************************/
//  Description:    This function is used to read data from GPS module    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPS_ReadData(uint8 * read_buf_ptr, uint32 byte_to_read)
{
	uint32 data_size = 0;
	
	if((GPS_STATUS_SLEEP == GPS_GetStatus()) || (GPS_STATUS_LOST == GPS_GetStatus()))
	{
		GPS_EnterSleep(SCI_FALSE);
	}
	
	if(GPS_STATUS_OPEN == GPS_GetStatus())
	{
		SCI_ASSERT(PNULL != s_gps_operations_ptr);/*assert verified*/
		SCI_ASSERT(PNULL != s_gps_operations_ptr->gps_readdata);/*assert verified*/
		_GPS_SetIdleTime();

		data_size = s_gps_operations_ptr->gps_readdata(read_buf_ptr, byte_to_read);
		if(GPS_MODE_TEST_NMEA == _GPS_GetMode())
		{
			// Snatch NMEA frame from data stream
			NMEA_SnatchFramesFromStream((uint8*)read_buf_ptr, data_size);
			// Flush the frames
			while(NMEA_ERR_NONE == NMEA_GetFrame( PNULL, NULL, NULL));
		}
		
		return  data_size;
	}
	
}

/*****************************************************************************/
//  Description:    This function is used to write data to GPS module    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPS_WriteData(uint8 * write_buf_ptr, uint32 byte_to_write)
{
	if((GPS_STATUS_SLEEP == GPS_GetStatus()) || (GPS_STATUS_LOST == GPS_GetStatus()))
	{
		GPS_EnterSleep(SCI_FALSE);
	}
	
	if(GPS_STATUS_OPEN == GPS_GetStatus())
	{
		SCI_ASSERT(PNULL != s_gps_operations_ptr);/*assert verified*/
		SCI_ASSERT(PNULL != s_gps_operations_ptr->gps_writedata);/*assert verified*/
		_GPS_SetIdleTime();
		return  s_gps_operations_ptr->gps_writedata(write_buf_ptr, byte_to_write);
	}
}

/*****************************************************************************/
//  Description:    This function is used to test GPS module
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPS_Test(void* ptr, uint32 param)
{
	if((GPS_STATUS_SLEEP == GPS_GetStatus()) || (GPS_STATUS_LOST == GPS_GetStatus()))
	{
		GPS_EnterSleep(SCI_FALSE);
	}
	
	if(GPS_STATUS_OPEN == GPS_GetStatus())
	{
		SCI_ASSERT(PNULL != s_gps_operations_ptr);/*assert verified*/
		SCI_ASSERT(PNULL != s_gps_operations_ptr->gps_test);/*assert verified*/
		_GPS_SetIdleTime();
		return  s_gps_operations_ptr->gps_test(ptr, param);
	}
}

/*****************************************************************************/
//  Description:    This function is used to test GPS module
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC GPS_INFO_T_PTR GPS_GetGpsInfo (void)
{
	if(GPS_STATUS_OPEN == GPS_GetStatus())
	{
		return (GPS_INFO_T_PTR)NMEA_GetGpsInfo();
	}
	else
	{
		return PNULL;
	}
}

/*****************************************************************************/
//  Description:    This function is used to get information about GPS engine and MAP    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC GPS_NV_INFO_T* GPS_GetNVInfo(void)
{
	 uint32  status = 0;
    
    // Read REF parameter initial values from NVItem.
    status = EFS_NvitemRead(NV_GPS_PARAM, sizeof(s_gps_nv_info), (uint8 *)(&s_gps_nv_info));    
    // If the initial values don't exist.
    if (NVERR_NOT_EXIST == status)
    {
#ifdef WIN32
        EFS_NvitemWrite(NV_GPS_PARAM, sizeof(s_gps_nv_info), (uint8 *)(&s_gps_nv_info), 1);
#else
        if (!NVITEM_IsSimulate())
        {
    	    SCI_PASSERT(0, ("Please download new NVItem !"));   /*assert verified*/
        }
#endif  // WIN32        
    }
    
    return &s_gps_nv_info;

}

/*****************************************************************************/
//  Description:    This function is used to write information about GPS engine and MAP to nv
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPS_WriteNVInfo(GPS_NV_INFO_T* nv_info_ptr)
{
	SCI_MEMCPY(&s_gps_nv_info, nv_info_ptr, sizeof(GPS_NV_INFO_T));
	EFS_NvitemWrite(NV_GPS_PARAM, sizeof(GPS_NV_INFO_T), (uint8 *)(&s_gps_nv_info), 1);
	 
	 return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to get validation cipher from DSP
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPS_GetValidationCipher(
		uint32						input_serial, 
		GPS_VALIDATION_CIPHER_T * 	output_validation_cipher_ptr)
{
	uint32 count = 0;
#ifdef _GPS_CIPHER	
	//GPS_TRACE:"GPS_GetValidationCipher: input_serial 0x%08x, start time %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_DRV_509_112_2_18_0_33_8_1558,(uint8*)"dd", input_serial, SCI_GetTickCount());
	
	s_is_get_gps_cipher = SCI_FALSE;
	
	L1API_StartGPSValidation(input_serial, _GPS_ValidationCallback);
	
	while(!(s_is_get_gps_cipher || (count > 200)))
	{
		SCI_Sleep(10);
		count++;
	}
	
	if(count < 200)
	{
		output_validation_cipher_ptr->high_cipher = s_gps_cipher.high_level;
		output_validation_cipher_ptr->low_cipher  = s_gps_cipher.low_level;
		//GPS_TRACE:"GPS_GetValidationCipher OK  end time %d!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_DRV_525_112_2_18_0_33_8_1559,(uint8*)"d", SCI_GetTickCount());
		return SCI_SUCCESS;
	}
	else
	{
		output_validation_cipher_ptr->high_cipher = 0;
		output_validation_cipher_ptr->low_cipher  = 0;
		//GPS_TRACE:"GPS_GetValidationCipher timer out, end time %d!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_DRV_532_112_2_18_0_33_8_1560,(uint8*)"d", SCI_GetTickCount());
		
		return SCI_ERROR;
	}
#else
	//extern uint16 g_cipherBuffer[];
	
	output_validation_cipher_ptr->high_cipher = 0x1122;
	output_validation_cipher_ptr->low_cipher  = 0x33445566;
	//g_cipherBuffer[0] = 0;
	//g_cipherBuffer[1] = 0x5566;
	//g_cipherBuffer[2] = 0x3344;
	//g_cipherBuffer[3] = 0x1122;
	
	return SCI_SUCCESS;
#endif		
	
}
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of gsp_drv.c
