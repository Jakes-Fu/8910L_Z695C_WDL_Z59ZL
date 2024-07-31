/******************************************************************************
 ** File Name:      gps_diag.c                                                *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           11/29/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of GPS   *
 ** 				Diag communication										  *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/29/2007     Liangwen.Zhen    Create.                         		  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#include "ms_customize_trc.h"
#include "os_api.h"
#include "cmddef.h"
#include "gps_diag.h"
#include "gps_drv.h"
#include "gps_interface.h"

#define DEBUG_GPS_DIAG
#ifdef  DEBUG_GPS_DIAG
	#define DIAG_TRACE   SCI_TRACE_LOW 
#else
	#define DIAG_TRACE( _format_string )  
#endif


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define GPS_API_ENABLE
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
#if defined(GPS_SUPPORT) && !defined(GPS_CHIP_VER_GREENEYE2) 
LOCAL GPS_DIAG_MODE_E s_gps_diag_mode = GPS_DIAG_MODE_MAX;
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

#if defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)

/*****************************************************************************/
//  Description:    GPS_StartBy_Diag   
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL GPS_ERR_E GPS_StartBy_Diag()
{
    GPS_ERR_E err_val = GPS_ERR_NONE;
	
	GPS_PowerOff();//don't jundje return value for nemog
    err_val = GPS_Download();
    if(GPS_ERR_NONE != err_val)
    {
        SCI_TRACE_LOW("[GE2]%s download fail",__FUNCTION__);
        return err_val;
    }

    err_val = GPS_Init_Greeneye2();
    if(GPS_ERR_NONE != err_val)
    {
        SCI_TRACE_LOW("[GE2]%s init fail",__FUNCTION__);
        return err_val;
    }
    
    GPS_TestMode_Enable(0x1);

    return err_val;
}


/*****************************************************************************/
//  Description:    This function is diag command routine function about GPS    
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL uint32 _GPS_DIAG_CmdRoutine(
    uint8 **	dest_ptr,       // Pointer of the response message.
    uint16 *	dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   	// Pointer of the input message. 
    uint16 		src_len)        // Size of the source buffer in uint8. 
{
 
    MSG_HEAD_T *msg_head = NULL;
    static uint32 nmealog_seq_num = 0x0;
    static uint32 debuglog_seq_num = 0x0;
    static BOOLEAN gps_init_flag = SCI_FALSE;
    static BOOLEAN gps_download_flag = SCI_FALSE;
    GPS_ERR_E err_val = GPS_ERR_NONE;
    static BOOLEAN gps_tcxo_mode = SCI_FALSE;
    static BOOLEAN gps_rf_mode = SCI_FALSE;
    static uint32 tcxo_seq_num = 0x0;

    msg_head = (MSG_HEAD_T *)(src_ptr);
    
    //DIAG_TRACE:"_GPS_DIAG_CmdRoutine() begin: cmd=%d, num=%d, val_list=%s"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_DIAG_78_112_2_18_0_33_6_1551,(uint8*)"dds", comm_ptr->cmd, comm_ptr->param_num, comm_ptr->param_val_list);
  
    //deal with GPS calibration command

    switch (msg_head->subtype)
    {
        case GPS_DIAG_HOT_START:
            if(!gps_init_flag)
            {
                err_val = GPS_StartBy_Diag();
		if(GPS_ERR_NONE == err_val)
		{
			gps_init_flag = SCI_TRUE;
		}
            }
            if(GPS_ERR_NONE == err_val)
            {
                GPS_DebugLog_Enable(0x1);
                GPS_NmeaLog_Enable(0x1);
                GPS_Reset_Request(GPS_HOT_MODE);
            }
            *dest_ptr 	  = SCI_ALLOC_APP(sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T));
            *dest_len_ptr = sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T);
            SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
            if(GPS_ERR_NONE == err_val)
            {
                ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_SUCCESS;
            }
            else
            {
                ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_FAIL;
            }
            ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
            break;

        case GPS_DIAG_WARM_START:
            if(!gps_init_flag)
            {
                err_val = GPS_StartBy_Diag();
		if(GPS_ERR_NONE == err_val)
		{
			gps_init_flag = SCI_TRUE;
		}
            }
            if(GPS_ERR_NONE == err_val)
            {
                GPS_DebugLog_Enable(0x1);
                GPS_NmeaLog_Enable(0x1);
                GPS_Reset_Request(GPS_WARM_MODE);
            }
            *dest_ptr 	  = SCI_ALLOC_APP(sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T));
            *dest_len_ptr = sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T);
            SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
            if(GPS_ERR_NONE == err_val)
            {
                ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_SUCCESS;
            }
            else
            {
                ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_FAIL;
            }
            ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
            break;

        case GPS_DIAG_COLD_START:
            if(!gps_init_flag)
            {
                err_val = GPS_StartBy_Diag();
		if(GPS_ERR_NONE == err_val)
		{
			gps_init_flag = SCI_TRUE;
		}
            }
            if(GPS_ERR_NONE == err_val)
            {
                GPS_DebugLog_Enable(0x1);
                GPS_NmeaLog_Enable(0x1);
                GPS_Reset_Request(GPS_COLD_MODE);
            }
            *dest_ptr 	  = SCI_ALLOC_APP(sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T));
            *dest_len_ptr = sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T);
            SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
            if(GPS_ERR_NONE == err_val)
            {
                ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_SUCCESS;
            }
            else
            {
                ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_FAIL;
            }
            ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
            break;

        case GPS_DIAG_STOP:
            GPS_NmeaLog_Enable(0x0);
            err_val = GPS_Close();
            //gps_init_flag = SCI_FALSE;
            gps_tcxo_mode = SCI_FALSE;
            gps_rf_mode = SCI_FALSE;
            *dest_ptr 	  = SCI_ALLOC_APP(sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T));
            *dest_len_ptr = sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T);
            SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
            if(GPS_ERR_NONE == err_val)
            {
                ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_SUCCESS;
            }
            else
            {
                ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_FAIL;
            }
            ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
            break;

        case GPS_DIAG_DEBUGLOG_ENABLE:
            err_val = GPS_DebugLog_Enable(0x1);
            debuglog_seq_num = msg_head->seq_num;
            *dest_ptr 	  = SCI_ALLOC_APP(sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T));
            *dest_len_ptr = sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T);
            SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
            if(GPS_ERR_NONE == err_val)
            {
                ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_SUCCESS;
            }
            else
            {
                ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_FAIL;
            }
            ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
            break;

        case GPS_DIAG_NMEALOG_OUTPUT:
            *dest_ptr 	  = SCI_ALLOC_APP(sizeof(GPS_NMEALOG_OUTPUT_T));
            msg_head->seq_num = nmealog_seq_num;
            *dest_len_ptr = msg_head->len;
            SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(GPS_NMEALOG_OUTPUT_T));
            ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
            break;

        case GPS_DIAG_DEBUGLOG_OUTPUT:
            break;

        case GPS_DIAG_NMEALOG_ENABLE:
            GPS_DebugLog_Enable(0x1);
            err_val = GPS_NmeaLog_Enable(0x1);
            nmealog_seq_num = msg_head->seq_num;
            *dest_ptr 	  = SCI_ALLOC_APP(sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T));
            *dest_len_ptr = sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T);
            SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
            if(GPS_ERR_NONE == err_val)
            {
                ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_SUCCESS;
            }
            else
            {
                ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_FAIL;
            }
            ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
            break;
        case GPS_DIAG_TXCO:
            if(!gps_tcxo_mode)
            {
                if(!gps_init_flag)
                {
	                err_val = GPS_StartBy_Diag();
			if(GPS_ERR_NONE == err_val)
			{
				gps_init_flag = SCI_TRUE;
			}
            	}
                if(GPS_ERR_NONE == err_val)
                {
                    GPS_DebugLog_Enable(0x1);
                    GPS_NmeaLog_Enable(0x1);
                    GPS_Reset_Request(GPS_TCXO_MODE);
                }
                tcxo_seq_num = msg_head->seq_num;
                *dest_ptr 	  = SCI_ALLOC_APP(sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T));
                *dest_len_ptr = sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T);
                SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
                if(GPS_ERR_NONE == err_val)
                {
                ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_SUCCESS;
                }
                else
                {
                ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_FAIL;
                }
                ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
                gps_tcxo_mode = SCI_TRUE;
            }
            else
            {
                    *dest_ptr 	  = SCI_ALLOC_APP(sizeof(GPS_NMEALOG_OUTPUT_T));
                    msg_head->seq_num = tcxo_seq_num;
                    *dest_len_ptr = msg_head->len;
                    SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(GPS_NMEALOG_OUTPUT_T));
                    ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
            }
            break;
            case GPS_DIAG_RF_GPS:
            if(!gps_rf_mode)
            {
                if(!gps_init_flag)
		{
			err_val = GPS_StartBy_Diag();
			if(GPS_ERR_NONE == err_val)
			{
				gps_init_flag = SCI_TRUE;
			}
		}

                if(GPS_ERR_NONE == err_val)
                {
                    GPS_DebugLog_Enable(0x1);
                    GPS_NmeaLog_Enable(0x1);
                    GPS_Set_RFTestMode(GPS_RF_GPS);
                    GPS_Reset_Request(GPS_RF_MODE);
                }
                tcxo_seq_num = msg_head->seq_num;
                *dest_ptr 	  = SCI_ALLOC_APP(sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T));
                *dest_len_ptr = sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T);
                SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
                if(GPS_ERR_NONE == err_val)
                {
                    ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_SUCCESS;
                }
                else
                {
                    ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_FAIL;
                }
                ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
                gps_rf_mode = SCI_TRUE;
            }
            else
            {
                *dest_ptr 	  = SCI_ALLOC_APP(sizeof(GPS_NMEALOG_OUTPUT_T));
                msg_head->seq_num = tcxo_seq_num;
                *dest_len_ptr = msg_head->len;
                SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(GPS_NMEALOG_OUTPUT_T));
                ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
            }
            break;
        case GPS_DIAG_RF_GLO:
        if(!gps_rf_mode)
        {
            if(!gps_init_flag)
	    {
		err_val = GPS_StartBy_Diag();
		if(GPS_ERR_NONE == err_val)
		{
			gps_init_flag = SCI_TRUE;
		}
	    }

            if(GPS_ERR_NONE == err_val)
            {
                GPS_DebugLog_Enable(0x1);
                GPS_NmeaLog_Enable(0x1);
                GPS_Set_RFTestMode(GPS_RF_GLO);
                GPS_Reset_Request(GPS_RF_MODE);
            }
            tcxo_seq_num = msg_head->seq_num;
            *dest_ptr 	  = SCI_ALLOC_APP(sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T));
            *dest_len_ptr = sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T);
            SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
            if(GPS_ERR_NONE == err_val)
            {
                ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_SUCCESS;
            }
            else
            {
                ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_FAIL;
            }
            ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
            gps_rf_mode = SCI_TRUE;
        }
        else
        {
            *dest_ptr 	  = SCI_ALLOC_APP(sizeof(GPS_NMEALOG_OUTPUT_T));
            msg_head->seq_num = tcxo_seq_num;
            *dest_len_ptr = msg_head->len;
            SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(GPS_NMEALOG_OUTPUT_T));
            ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
        }
        break;

            case GPS_DIAG_RF_BD:
            if(!gps_rf_mode)
            {
                if(!gps_init_flag)
		{
			err_val = GPS_StartBy_Diag();
			if(GPS_ERR_NONE == err_val)
			{
				gps_init_flag = SCI_TRUE;
			}
		}

                if(GPS_ERR_NONE == err_val)
                {
                    GPS_DebugLog_Enable(0x1);
                    GPS_NmeaLog_Enable(0x1);
                    GPS_Set_RFTestMode(GPS_RF_BDS);
                    GPS_Reset_Request(GPS_RF_MODE);
                }
                tcxo_seq_num = msg_head->seq_num;
                *dest_ptr 	  = SCI_ALLOC_APP(sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T));
                *dest_len_ptr = sizeof(MSG_HEAD_T)+sizeof(GPS_DIAG_RESPONSE_T);
                SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
                if(GPS_ERR_NONE == err_val)
                {
                    ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_SUCCESS;
                }
                else
                {
                    ((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd = GPS_DIAG_RESPONSE_FAIL;
                }
                ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
                gps_rf_mode = SCI_TRUE;
            }
            else
            {
                *dest_ptr 	  = SCI_ALLOC_APP(sizeof(GPS_NMEALOG_OUTPUT_T));
                msg_head->seq_num = tcxo_seq_num;
                *dest_len_ptr = msg_head->len;
                SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(GPS_NMEALOG_OUTPUT_T));
                ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
            }
            break;

        default:
            break;    
    }
    //SCI_TRACE_LOW("[GE2]%s,sub_type=%d,data=%d,len=%d",__FUNCTION__,msg_head->subtype,((GPS_DIAG_RESPONSE_PTR)(*dest_ptr + sizeof(MSG_HEAD_T)))->cmd,((MSG_HEAD_T *) (*dest_ptr))->len);

    return SCI_TRUE;
} 

#else
/*****************************************************************************/
//  Description:    This function is diag command routine function about GPS    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _GPS_DIAG_CmdRoutine(
    uint8 **	dest_ptr,       // Pointer of the response message.
    uint16 *	dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   	// Pointer of the input message. 
    uint16 		src_len)        // Size of the source buffer in uint8. 
{
 
    GPS_DIAG_MSG_PTR 	comm_ptr 		 = PNULL;
    GPS_DIAG_MSG_PTR	res_ptr  		 = PNULL;
#ifndef GPS_API_ENABLE	
    COM_OBJ 			*pCom 	 		 = &com1_ins;
#endif
    GPS_NV_INFO_T		*gps_nv_info_ptr = PNULL;
	GPS_INFO_T_PTR		gps_info_ptr	 = PNULL;
    
    comm_ptr = (GPS_DIAG_MSG_PTR)(src_ptr + sizeof(MSG_HEAD_T));
    
    //DIAG_TRACE:"_GPS_DIAG_CmdRoutine() begin: cmd=%d, num=%d, val_list=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_DIAG_78_112_2_18_0_33_6_1551,(uint8*)"dds", comm_ptr->cmd, comm_ptr->param_num, comm_ptr->param_val_list);
  
    //deal with GPS calibration command
    switch (comm_ptr->cmd)
    {
        case GPS_DIAG_CMD_SEND:  
			#ifdef GPS_API_ENABLE
			comm_ptr->param_num = GPS_WriteData( comm_ptr->param_val_list, comm_ptr->param_num );
			#else
            UART_Tx_Int_Enable(pCom->port, FALSE);
            comm_ptr->param_num = QueueInsert(&Output_Q, comm_ptr->param_val_list, comm_ptr->param_num);
            UART_Tx_Int_Enable(pCom->port, TRUE);
			#endif
            break;
		
        case GPS_DIAG_CMD_RECEIVE:
			#ifdef GPS_API_ENABLE
			comm_ptr->param_num = GPS_ReadData( comm_ptr->param_val_list, comm_ptr->param_num );
			#else
            UART_Rx_Int_Enable(pCom->port, FALSE);
            comm_ptr->param_num = QueueInsert(&Input_Q, comm_ptr->param_val_list, comm_ptr->param_num);
            UART_Rx_Int_Enable(pCom->port, TRUE);
			#endif
            break;
			
        case GPS_DIAG_CMD_OPEN:
			#ifdef GPS_API_ENABLE
			s_gps_diag_mode = comm_ptr->param_val_list[0];
			if((GPS_DIAG_MODE_WR_DATA == s_gps_diag_mode) && (GPS_DIAG_MODE_GET_INFO == s_gps_diag_mode))
			{
				GPS_Open(GPS_MODE_NORMAL);
				GPS_WriteData((uint8 *)"GPSCalCmd open.\r\n", 17);
			}
			#else
            QueueClean(&Input_Q);
            QueueClean(&Output_Q);
            GPS_ComInit();
            GPS_ComWrite((uint8 *)"GPSCalCmd open.\r\n", 17);
			#endif
            break;
		
        case GPS_DIAG_CMD_CLOSE:
			#ifdef GPS_API_ENABLE
			if((GPS_DIAG_MODE_WR_DATA == s_gps_diag_mode) && (GPS_DIAG_MODE_GET_INFO == s_gps_diag_mode))
			{
				GPS_Close();
			}
			s_gps_diag_mode = GPS_DIAG_MODE_MAX;
			#else
            GPS_ComClose();
			#endif
            break;
		
        case GPS_DIAG_CMD_WRITE_NV:
        	SCI_ASSERT(sizeof(GPS_NV_INFO_T) == comm_ptr->param_num);/*assert verified*/
        	GPS_WriteNVInfo((GPS_NV_INFO_T*)comm_ptr->param_val_list);
	        break;
			
	    case GPS_DIAG_CMD_READ_NV:
	    	SCI_ASSERT(sizeof(GPS_NV_INFO_T) == comm_ptr->param_num);/*assert verified*/
	    	gps_nv_info_ptr = GPS_GetNVInfo();
	    	SCI_ASSERT(PNULL != gps_nv_info_ptr);/*assert verified*/
	    	SCI_MEMCPY(comm_ptr->param_val_list, gps_nv_info_ptr, sizeof(GPS_NV_INFO_T));
	    	break;

		case GPS_DIAG_CMD_GET_GPS_INFO:
			SCI_ASSERT(sizeof(GPS_INFO_T) == comm_ptr->param_num);/*assert verified*/
	    	gps_info_ptr = GPS_GetGpsInfo();
	    	SCI_ASSERT(PNULL != gps_info_ptr);/*assert verified*/
	    	SCI_MEMCPY(comm_ptr->param_val_list, gps_info_ptr, sizeof(GPS_INFO_T));
			break;
        default:
            break;    
    }

	// Build response to channel server
    *dest_ptr 	  = SCI_ALLOC_APP(sizeof(MSG_HEAD_T) + sizeof(GPS_DIAG_MSG_T));
    *dest_len_ptr = sizeof(MSG_HEAD_T) + sizeof(GPS_DIAG_MSG_T);
    SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T)+ sizeof(GPS_DIAG_MSG_T));
    ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
    
    //DIAG_TRACE:"GPSCalCmd end: cmd=%d, num=%d, val_list=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_DIAG_159_112_2_18_0_33_7_1552,(uint8*)"dds", res_ptr->cmd, res_ptr->param_num, res_ptr->param_val_list);
	
    return SCI_TRUE;
} 
#endif

/**---------------------------------------------------------------------------*
 **                     PUBLIC Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is register diag command routine
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC void GPS_DIAG_RegDiagCmdRoutine(void)
{    
    //DIAG_TRACE:"GPS: GPS_DIAG_RegDiagCmdRoutine"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_DIAG_172_112_2_18_0_33_7_1553,(uint8*)"" );
	
    DIAG_RegisterCmdRoutine(DIAG_GPS_F, _GPS_DIAG_CmdRoutine);
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of gsp_diag.c
