/******************************************************************************
 ** File Name:      greeneye2_log.c                                                     *
 ** Author:                                                     *
 ** DATE:                                                           *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/04/2010     fei.zhang        Add BT USB Log function       *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
#include "cmddef.h"
#include "ref_param.h"
#include "mem_prod.h"
#include "ref_outport.h"
#include "sfs.h"
#include "priority_system.h"
#include "dal_time.h"
#include "greeneye2.h"
#include "greeneye2_tcard_log.h"
#include "greeneye2_common.h"
#include "env_gps_signal.h"

/**---------------------------------------------------------------------------*
 **                                                 Compiler Flag                                                               *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#define GPSLOG_DEFAULT_MAX_SIZE      (50*1024*1024) 
#define TCARD_TX_BUF_MAX_SIZE           (30 * 1024)

#define GPS_CARDLOG_THREAD_STACK_SIZE (5*1024)
#define GPS_CARDLOG_THREAD_PRIORITY     (PRI_GPS_LOG_TASK+2)    //31
#define GPS_CARDLOG_THREAD_QUEUE_NUM    0x20

typedef enum
{
    GPS_CARD_LOG_OPEN = MN_GPS_END_SIGNAL + 10,
    GPS_CARD_LOG_CLOSE,
    GPS_CARD_LOG_WIRTE,
    GPS_CARD_LOG_END
}gps_cardlog_signal_id_t;

typedef struct _GPS_TCARD_BUFFER_S{
    BOOLEAN is_busy;
    uint32 tx_in_ptr;
    uint8  tx_buf[TCARD_TX_BUF_MAX_SIZE];
}GPS_TCARD_BUFFER_S;

typedef struct _GPS_TCARD_LOG_INFO_S{
    SFS_HANDLE sfs_log_handle;
    uint32 log_total_size;
    uint8 is_current;
    GPS_TCARD_BUFFER_S buf[2];
}GPS_TCARD_LOG_INFO_S;

BOOLEAN g_tcard_enable = FALSE;
BLOCK_ID g_gps_cardlog_thread_id = SCI_INVALID_BLOCK_ID;

GPS_TCARD_LOG_INFO_S  g_tcard_data[GPS_TCARDLOG_TYPE_MAX];

LOCAL uint16 Gps_gb2ucs(					//return the number of UCS2 code that have been converted.
						uint16 *ucs2_ptr, 	            //destination: string pointer to UCS2 string.
						const uint8 *gb_ptr,              //source: string pointer to GB string
						uint16 len			                  //the length to convert from GB to UCS2.
						)
{
    uint16 tmp_gb, count = 0;

    SCI_ASSERT(PNULL != gb_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != ucs2_ptr); /*assert verified*/

    while((len > 0) && (len < 0xfff))
    {
        len--;
        count += 2;
        tmp_gb = (uint16)(*gb_ptr++);

        if (tmp_gb < 0x80)
        {
            *ucs2_ptr++ = tmp_gb;
        }
    }

    return count;
}

LOCAL BOOLEAN Gps_CreatSfsFile(
                SFS_HANDLE*     fd_ptr,
                uint16*         filename,
                uint16*         device_name
                )
{
    SFS_ERROR_E device_result = SFS_ERROR_NO_SPACE;
    BOOLEAN return_result = FALSE;  
        
    device_result = SFS_GetDeviceStatus(device_name);   

    if(SFS_ERROR_NONE == device_result)
    {       
        *fd_ptr = SFS_CreateFile(filename, SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, NULL, NULL);
        if(NULL != *fd_ptr)     
        {
            GREENEYE2_LOGD(("[GE2]GPSLOG: SFS_CreateFile success!"));
            return_result = TRUE;
        }
        else
        {
            GREENEYE2_LOGE(("[GE2]GPSLOG: SFS_CreateFile fail!"));
            return_result = FALSE;
        }      
    }    
    else
    {
        GREENEYE2_LOGE(("[GE2]GPSLOG: device status(%d) error!", device_result));      
        return_result = FALSE;
    }
    
    return return_result;
}

LOCAL BOOLEAN Gps_CreateFile(GPS_TCARDLOG_TYPE_E type)
{
    BOOLEAN        ret_create_file;
    char        GPS_log_file_name_gb[SFS_MAX_PATH+1] = {0};
    uint16      GPS_log_file_name[SFS_MAX_PATH+1] = {0};
    uint16      GPS_log_file_name_len = 0;
    uint16      tcard_name[2] = {'E', 0};    
    SCI_TM_T realtime;

    TM_GetTime(&realtime);
    GPS_log_file_name_len = sprintf(GPS_log_file_name_gb,"E:\\GPSLOG-%04d%02d%02d-%02d%02d%02d.log", realtime.tm_year,realtime.tm_mon,realtime.tm_mday,realtime.tm_hour,realtime.tm_mon,realtime.tm_sec);
    Gps_gb2ucs(GPS_log_file_name,  (uint8 *) GPS_log_file_name_gb, GPS_log_file_name_len);  

    /* create new log file in sfs, original file */        
    ret_create_file = Gps_CreatSfsFile(&(g_tcard_data[type].sfs_log_handle), GPS_log_file_name, tcard_name);
    if(FALSE == ret_create_file)      
    {        
        GREENEYE2_LOGD(("[GE2]GPSLOG: create log file %s fail, finish_time %d", GPS_log_file_name_gb,  SCI_GetTickCount()));               
        return FALSE;      
    }  

    g_tcard_data[type].log_total_size = 0;
    GREENEYE2_LOGD(("[GE2]GPSLOG: create log file %s suc, finish_time %d", GPS_log_file_name_gb,  SCI_GetTickCount()));    

    return TRUE;
}



LOCAL BOOLEAN Gps_WriteFile(
                GPS_TCARDLOG_TYPE_E   type, 
                uint8*              write_buff_ptr,
                uint32              write_length
                )
{
    
    uint32  wrtlen;
    BOOLEAN return_result = FALSE;
    SFS_ERROR_E result = SFS_ERROR_DEVICE;
    GpsState*  state = _gps_state; 
    
#ifndef MODEM_PLATFORM
    if(state->cmcc || !Cardlog_Pal_init())
#endif
    {
        return return_result;
    }

    if(write_buff_ptr == PNULL || write_length == 0)
    {
        return return_result;
    }

    result = SFS_WriteFile(g_tcard_data[type].sfs_log_handle, (void*)write_buff_ptr, write_length, &wrtlen, NULL);
    switch( result )
    {
        case SFS_ERROR_NONE:/*0:success*/
        {
            GREENEYE2_LOGD(("[GE2]GPSLOG:  SFS_FlushFile"));
            SFS_FlushFile(g_tcard_data[type].sfs_log_handle, NULL);   

            /***** verified if need to recreate log file ****/
            g_tcard_data[type].log_total_size += write_length;
            if( g_tcard_data[type].log_total_size >= GPSLOG_DEFAULT_MAX_SIZE)
            {				
                GREENEYE2_LOGD(("[GE2]GPSLOG:  log_total_size(%d) > 50M", g_tcard_data[type].log_total_size));

                SFS_CloseFile(g_tcard_data[type].sfs_log_handle);  
                g_tcard_data[type].sfs_log_handle = NULL;
                Gps_CreateFile(type);
                g_tcard_data[type].log_total_size = 0;
            }

            return_result = TRUE; 
            break ; 
        }
        case SFS_ERROR_NO_SPACE:
        {
            GREENEYE2_LOGE(("[GE2]GPSLOG: SFS_WriteFile  SFS_ERROR_NO_SPACE"));			
            SFS_CloseFile(g_tcard_data[type].sfs_log_handle); 
            g_tcard_data[type].sfs_log_handle = NULL;
            return_result = FALSE; 
            break ;
        }
        case SFS_ERROR_DEVICE:
        case SFS_ERROR_INVALID_PARAM:
        {
            /*In UDisk mode, SFS will be unmounted, and return 3 */
            GREENEYE2_LOGE(("[GE2]GPSLOG: SFS_WriteFile SFS_ERROR_DEVICE or SFS_ERROR_INVALID_PARAM result = %d",  result));
            SFS_CloseFile(g_tcard_data[type].sfs_log_handle);     
            g_tcard_data[type].sfs_log_handle = NULL;
            Gps_CreateFile(type);
            return_result = FALSE; 
            break;
        }
        
        default:
        {
            GREENEYE2_LOGE(("[GE2]GPSLOG: SFS_WriteFile other error result = %d", result));
            SFS_CloseFile(g_tcard_data[type].sfs_log_handle);  
            g_tcard_data[type].sfs_log_handle = NULL;
            return_result = FALSE;   
            break ;
        } 				
    }	 

    return return_result;
}

LOCAL void Gps_SwitchBuffer(GPS_TCARDLOG_TYPE_E type)
{
    GPS_TCARD_BUFFER_S* buffer = PNULL;
    g_tcard_data[type].is_current = (g_tcard_data[type].is_current+1)%2;
    buffer = &(g_tcard_data[type].buf[g_tcard_data[type].is_current]);
    if(buffer->is_busy)
        GREENEYE2_LOGD(("[GE2]GPSLOG: Notice the current buffer is busy, cannot to switch!"));
    else
        GREENEYE2_LOGD(("[GE2]GPSLOG: Switch the current buffer successfully!"));
        
    buffer->tx_in_ptr = 0;
}

LOCAL uint32 Gps_WriteBuffer(GPS_TCARDLOG_TYPE_E type, uint8 *src_buf, uint16 num_to_send)
{
    uint32 buff_left_space;
    uint32 buff_need_write; 
    uint8 *buff_write_point;

    GPS_TCARD_BUFFER_S* buffer = &(g_tcard_data[type].buf[g_tcard_data[type].is_current]);
    
    if(SCI_NULL == src_buf || 0 == num_to_send)
        return 0;

    buff_left_space = (TCARD_TX_BUF_MAX_SIZE - buffer->tx_in_ptr);
    if(buff_left_space == 0)
        return 0;
    
    buff_write_point = &(buffer->tx_buf[buffer->tx_in_ptr]);
    buff_need_write = num_to_send < buff_left_space ?  num_to_send : buff_left_space;

    SCI_MEMCPY((uint8 *)buff_write_point, (uint8 *)src_buf, buff_need_write);

    buffer->tx_in_ptr += buff_need_write;

    //GREENEYE2_LOGD(("[GE2]GPSLOG: write %d bytes to buffer[%d] = %d", buff_need_write, g_tcard_data[type].is_current, buffer->tx_in_ptr));
    
    return buff_need_write;
}

LOCAL void Gps_CardLogOpen_Handle(GPS_TCARDLOG_TYPE_E type)
{

    if(g_tcard_data[type].sfs_log_handle == NULL)
        Gps_CreateFile(type);

    return;
}

LOCAL void Gps_CardLogClose_Handle (GPS_TCARDLOG_TYPE_E type)
{
    /* Flush the log data in current buffer */
    GPS_TCARD_BUFFER_S* buffer = &(g_tcard_data[type].buf[g_tcard_data[type].is_current]);
    Gps_WriteFile(type, (uint8 *)buffer->tx_buf, buffer->tx_in_ptr);
    GREENEYE2_LOGE(("[GE2]GPSLOG: Gps CardLog Write to File with  %d bytes", buffer->tx_in_ptr));
    buffer->tx_in_ptr = 0;
    

    /* Close the log file */
    if(g_tcard_data[type].sfs_log_handle){
        SFS_CloseFile( g_tcard_data[type].sfs_log_handle);
        g_tcard_data[type].sfs_log_handle = NULL;
    }
    
    GREENEYE2_LOGD(("[GE2]GPSLOG: Close Log file success!"));
    
    return;
}

LOCAL void Gps_CardLogWrite_Handle(GPS_TCARDLOG_TYPE_E type)
{  
    if(g_tcard_enable)
    {
        uint16 num = (g_tcard_data[type].is_current+1)%2;
        GPS_TCARD_BUFFER_S* buffer = &(g_tcard_data[type].buf[num]);
        buffer->is_busy = TRUE;
        Gps_WriteFile(type, (uint8 *)buffer->tx_buf, buffer->tx_in_ptr);
        buffer->is_busy = FALSE;
        GREENEYE2_LOGE(("[GE2]GPSLOG: Gps CardLog Write to File with  %d bytes", buffer->tx_in_ptr));
    }
}
        
LOCAL void Gps_Cardlog_Thread_Entry(uint32 argc, void *argv){
    
    xSignalHeaderRec *signal = PNULL;
    GREENEYE2_LOGD(("[GE2]GPSLOG: Gps_Cardlog_Thread_Entry: enter"));

    do{
        signal = (xSignalHeaderRec *) SCI_GetSignal (g_gps_cardlog_thread_id);

        switch((gps_cardlog_signal_id_t)(signal->SignalCode))
        {
            case GPS_CARD_LOG_OPEN:
                GREENEYE2_LOGD(("[GE2]GPSLOG: SignalCode: GPS_CARD_LOG_OPEN"));
                Gps_CardLogOpen_Handle(GPS_TCARDLOG_TYPE_ALL);
                break;

            case GPS_CARD_LOG_CLOSE:
                GREENEYE2_LOGD(("[GE2]GPSLOG: SignalCode: GPS_CARD_LOG_CLOSE"));
                Gps_CardLogClose_Handle(GPS_TCARDLOG_TYPE_ALL);
                break;

           case GPS_CARD_LOG_WIRTE:
                GREENEYE2_LOGD(("[GE2]GPSLOG: SignalCode: GPS_CARD_LOG_WIRTE"));
                Gps_CardLogWrite_Handle(GPS_TCARDLOG_TYPE_ALL);
                break;
        }

        SCI_FREE(signal);
        signal = PNULL;
        
    }while(1);

    GREENEYE2_LOGD(("[GE2]GPSLOG: Gps_Cardlog_Thread_Entry: leave"));
}


PUBLIC void Gps_CardLogWrite(GPS_TCARDLOG_TYPE_E type, const uint8 *buffer, uint32 length)
{
    uint32 had_written_len = 0;
    xSignalHeaderRec  *psig = PNULL;

    if(length == 0)
        return;

    if(g_tcard_enable)
    {
        had_written_len = Gps_WriteBuffer(GPS_TCARDLOG_TYPE_ALL,  (uint8 *)buffer, (uint32)length);
        if(had_written_len < length){
            Gps_SwitchBuffer(GPS_TCARDLOG_TYPE_ALL);
            Gps_WriteBuffer(GPS_TCARDLOG_TYPE_ALL,  (uint8 *)buffer+had_written_len, (uint32)(length-had_written_len));
            SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig, GPS_CARD_LOG_WIRTE, sizeof(xSignalHeaderRec),  SCI_IdentifyThread());
            SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,  g_gps_cardlog_thread_id);
        }
    }

    return;
}

PUBLIC void Gps_CardLogOpen(GPS_TCARDLOG_TYPE_E type)
{
    xSignalHeaderRec  *psig = PNULL;

    g_tcard_enable =  TRUE;
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig, GPS_CARD_LOG_OPEN, sizeof(xSignalHeaderRec),  SCI_IdentifyThread());
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,  g_gps_cardlog_thread_id);
}

PUBLIC void Gps_CardLogClose(GPS_TCARDLOG_TYPE_E type)
{
    xSignalHeaderRec  *psig = PNULL;
    
    g_tcard_enable =  FALSE;
    SCI_SLEEP(200);
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig, GPS_CARD_LOG_CLOSE, sizeof(xSignalHeaderRec),  SCI_IdentifyThread());
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,  g_gps_cardlog_thread_id);
}

PUBLIC void Gps_Cardlog_task_init(void)
{
	
	if((SCI_INVALID_BLOCK_ID == g_gps_cardlog_thread_id) || (SCI_NULL == g_gps_cardlog_thread_id))
	{
    	g_gps_cardlog_thread_id = SCI_CreateThread("T_GPS_CARDLOG_SCHED", "Q_GPS_CARDLOG_SCHED", Gps_Cardlog_Thread_Entry, 0, NULL, GPS_CARDLOG_THREAD_STACK_SIZE, 
									GPS_CARDLOG_THREAD_QUEUE_NUM, GPS_CARDLOG_THREAD_PRIORITY, SCI_PREEMPT, SCI_AUTO_START);

	    SCI_ASSERT(SCI_INVALID_BLOCK_ID != g_gps_cardlog_thread_id);
	}
}

PUBLIC void Gps_Cardlog_task_deinit(void)
{
    if(g_gps_cardlog_thread_id != SCI_INVALID_BLOCK_ID)
    {
        SCI_TerminateThread(g_gps_cardlog_thread_id);
        SCI_DeleteThread(g_gps_cardlog_thread_id);
        g_gps_cardlog_thread_id = SCI_INVALID_BLOCK_ID;
    }    
}
#ifdef   __cplusplus
}
#endif 
