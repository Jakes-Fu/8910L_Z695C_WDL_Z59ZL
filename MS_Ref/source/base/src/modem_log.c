/******************************************************************************
 ** File Name:      dsp_log.c                                                     *
 ** Author:         Fei Zhang                                              *
 ** DATE:           07/04/2011                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/04/2010     fei.zhang        Add DSP USB Log function       *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
#include "modem_log.h"
#include "chip_plf_export.h"
#include "ref_param.h"
#include "mem_prod.h"
#include "ref_outport.h"
#include "sfs.h"
#include "priority_system.h"
#include "load_modem.h"
#include "modem_log.h"
#include "cfw.h"

/**---------------------------------------------------------------------------*
 **                                                 Compiler Flag                                                               *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/*-----------------------------SIO DEBUG MACOR-------------------------------*/

#ifdef MODEM_LOG_TRA
#define SIO_MODEM_TRACE   SCI_TraceLow
#else
#define SIO_MODEM_TRACE(...) 
#endif

#define  TASK_MODEM_CP_LOG_PRI   (PRI_APP)   //31

/*-----------------------------END--------------------------------------------*/

/**---------------------------------------------------------------------------*
 **  Static Variables & functions
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **  external Variables
 **---------------------------------------------------------------------------*/
extern SCI_MUTEX_PTR   comm_mutex;
extern uint32 s_sio_tx_dma_is_start;
//extern uint32 armLogSendContinueCnt;  //arm log continue send out count,must be cleared to 0 when other log be sent out
extern uint32 dspLogSendContinueCnt;   //dsp log continue send out count,must be cleared to 0 when other log be sent out
extern SIO_INFO_S      sio_port[MAX_LOGICAL_SIO_PORT_NUM];           // SIO information

PUBLIC uint32 SIO_GetTxFifoCnt (uint32 port);

/**---------------------------------------------------------------------------*
 **  Protocol functions
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **  User port variables & functions
 **---------------------------------------------------------------------------*/
#define MODME_LOG_COM_PORT      COM_DEBUG2
#define MODEM_LOG_USB_VIR_COM         VIR_COM4
#define SIO_MODEM_LOG_SEND_CP  0
#define SIO_MODEM_LOG_SEND_DSP  1

#define MODEM_CP_LOG_MIN_SIZE  1536
#define MODEM_DSP_LOG_MIN_SIZE  16
#define MODEM_CP_LOG_MAX_SIZE  (16*1026)
#define MODEM_DSP_LOG_MAX_SIZE  (8*1024)
#define MODEM_LOG_ALIGN_4   0xFFFFFFFC

#define MODEM_USB_DISCARD_THRESHOLD  100

#define MODEM_LOG_USB_TIMER            500
#define MODEM_LOG_SDC_TIMER             80

LOCAL uint32  v_sio_modem_log_timer_value = 0; 


PUBLIC SCI_SEMAPHORE_PTR modem_cp_log_semaphore = SCI_NULL;
LOCAL BLOCK_ID modem_cp_log_thread_id = SCI_INVALID_BLOCK_ID;
PUBLIC SCI_SEMAPHORE_PTR modem_dsp_log_semaphore = SCI_NULL;

LOCAL BLOCK_ID modem_dsp_log_thread_id = SCI_INVALID_BLOCK_ID;
LOCAL uint16 modem_dsp_logfile_num = 0, modem_dsp_deletefile_num = 0;
LOCAL uint16 modem_cp_logfile_num = 0, modem_cp_deletefile_num = 0;

LOCAL SFS_HANDLE  sfs_modem_dsp_log = NULL; 
LOCAL SFS_HANDLE  sfs_modem_cp_log = NULL; 
PUBLIC T_TRA_MODEM_LOG * p_modem_cp_log_buff_info = NULL; 
PUBLIC T_TRA_MODEM_LOG * p_modem_dsp_log_buff_info = NULL; 


LOCAL uint8  v_sio_modem_log_tx_is_start = SCI_FALSE; 
LOCAL uint8  v_sio_modem_cp_log_sdc_tx_is_start = SCI_FALSE; 
LOCAL uint8  v_sio_modem_dsp_log_sdc_tx_is_start = SCI_FALSE; 
LOCAL uint32  v_sio_modem_cp_log_lost_cnt = 0; 
LOCAL uint32  v_sio_modem_dsp_log_lost_cnt = 0; 
LOCAL uint8  v_sio_modem_log_usb_cp_threshold_cnt = 0;   
LOCAL uint8  v_sio_modem_log_usb_dsp_threshold_cnt = 0;  

LOCAL uint8  v_sio_modem_tx_type = SIO_MODEM_LOG_SEND_CP;  //0,send cp;1 send dsp log

#ifdef MODEM_LOG_USB_ZERO_COPY

#define MODEM_LOG_USB_FINISH    0x10
#define MODEM_LOG_USB_WORK    0x1
#define MODEM_LOG_USB_IDLE    0x0
//0: usb-send is idle ; 1: usb-send  is working; 0x10: usb-send  has finished
//avoid the send api to be reentried in the critical region when not completed  the last time .
LOCAL uint8  v_sio_modem_log_usb_flag = MODEM_LOG_USB_IDLE; 
LOCAL uint32  v_sio_modem_cp_log_sending_sz = 0; 
LOCAL uint32  v_sio_modem_dsp_log_sending_sz = 0; 
LOCAL uint32  v_sio_modem_cp_log_sending_total_sz = 0; 
LOCAL uint32  v_sio_modem_dsp_log_sending_total_sz = 0; 

#endif
LOCAL uint32  v_sio_modem_log_usb_fifo_full_cnt = 0;   // when usb fifo is full, and disacrd the log to free the log buffer


PUBLIC uint8  v_sio_modem_log_tx_status = SIO_MODEM_LOG_USB_TX_DISABLE; 
PUBLIC uint8  v_sio_modem_log_sdc_tx_status = SIO_MODEM_LOG_SDC_TX_DISABLE; 



static SCI_TIMER_PTR s_sio_modem_log_usb_check_timer = PNULL;  

LOCAL void SIO_SendModemCpLog(void);
LOCAL void SIO_SendModemDspLog(void);

LOCAL void SIO_SdcSendModemCpLog(uint32 send_size);
LOCAL void SIO_SdcSendModemDspLog(uint32 send_size);

extern uint32 UCOM_TxIsIdle(uint32 port_num);
LOCAL uint32 SIO_GetModemLogSize (void *p_ctrl_info)
{
	T_TRA_MODEM_LOG *p_tra_info = (T_TRA_MODEM_LOG *)p_ctrl_info;
	uint32 v_len = 0;

	//bug1112466, assure the rdIdex is 4Bytes alignment
       if((p_tra_info->rdIndex) & (~(MODEM_LOG_ALIGN_4)))
       {
  		SCI_TraceLow("modemLog:rdInx unaligned 4B,port %d, head 0x%x, wr0x%x,rd0x%x,ofi0x%x",SIO_GetModemTracePort(),p_tra_info->head,p_tra_info->wrIndex,p_tra_info->rdIndex,p_tra_info->overflowIndex);
       	p_tra_info->rdIndex = p_tra_info->rdIndex & MODEM_LOG_ALIGN_4;
    	}

	 if(p_tra_info->wrIndex >= p_tra_info->rdIndex)
            return  ((p_tra_info->wrIndex -  p_tra_info->rdIndex) & MODEM_LOG_ALIGN_4);
        else
        {
        	v_len = (p_tra_info->overflowIndex - p_tra_info->rdIndex);
	     // avoid the tail log size less than 4
            if(v_len < 4)
	    {	 
	    	return v_len;
            }
	     else
               return (v_len & MODEM_LOG_ALIGN_4);
        }
}

LOCAL uint32 SIO_GetModemLogTotalSize (void *p_ctrl_info)
{
	T_TRA_MODEM_LOG *p_tra_info = (T_TRA_MODEM_LOG *)p_ctrl_info;

	 if(p_tra_info->wrIndex >= p_tra_info->rdIndex)
            return  p_tra_info->wrIndex -  p_tra_info->rdIndex;
        else
            return p_tra_info->length- p_tra_info->rdIndex +  p_tra_info->wrIndex;
}

/*****************************************************************************/
// Description: Dsp log process Task
// Global resource dependence: sio_dsplog_trans_buf, dsp_log_semaphore, sio_dsp_log_headInfo_ptr
// Author: fei.zhang
// Note:
/*****************************************************************************/
LOCAL void SIO_ModemCpLogThread (uint32 id, void *pdata)
{
    uint32	v_log_size,v_len,v_i,v_sended_size;
    T_TRA_MODEM_LOG * p_log_buff_info =NULL;
    T_TRA_MODEM_LOG * p_dsp_log_buff_info =NULL;
    uint8 v_port;

    

    for (; ;)
    {
        SCI_GetSemaphore (modem_cp_log_semaphore,SCI_WAIT_FOREVER); //SCI_WAIT_FOREVER

	   if((SIO_MODEM_UART == SIO_GetModemTracePort()) || (SIO_MODEM_PORT_NULL == SIO_GetModemTracePort()))
	   {
	   	SIO_SetModemTracePort(COM3);
	   }
	   
        v_port =  SIO_ModemTracePortNotify();
	
        if(NULL == p_modem_cp_log_buff_info)
        {
            p_modem_cp_log_buff_info = (T_TRA_MODEM_LOG *)GET_MemoryStartAddressAndLen(MEM_CP_DEBUG_NAME,&v_len);
        }

	 if(NULL == p_modem_dsp_log_buff_info)
	 {
		 p_modem_dsp_log_buff_info = (T_TRA_MODEM_LOG *)GET_MemoryStartAddressAndLen(MEM_ZSP_DEBUG_NAME,&v_len);
	 }

	 p_log_buff_info = p_modem_cp_log_buff_info;
	 p_dsp_log_buff_info = p_modem_dsp_log_buff_info;

        SIO_MODEM_TRACE ("modemLog:send cp log,port %d",v_port);
	 v_len = 0;

	SCI_TraceLow("modemLog:thread,port %d, CP wr0x%x,rd0x%x,ofi0x%x,DSP  wr0x%x,rd0x%x,ofi0x%x",v_port,p_modem_cp_log_buff_info->wrIndex,p_modem_cp_log_buff_info->rdIndex,p_modem_cp_log_buff_info->overflowIndex,p_modem_dsp_log_buff_info->wrIndex,p_modem_dsp_log_buff_info->rdIndex,p_modem_dsp_log_buff_info->overflowIndex);

  	        
         switch(v_port)
          {
                case SIO_MODEM_AP_DSK:
             {
			#if 1 

                     //first, send dsp log
          		for(v_i = 0; v_i < 2; v_i++)
		      {	
                            v_log_size = SIO_GetModemLogSize((void*)p_dsp_log_buff_info);

				if(v_log_size)
				{
					SIO_SdcSendModemDspLog(v_log_size);                      
				}
				else
				 	break;
	                }
		 	  //v_log_size =  (p_dsp_log_buff_info->length + p_dsp_log_buff_info->wrIndex -  p_dsp_log_buff_info->rdIndex) & (p_dsp_log_buff_info->length - 1);
	
	 		 //SIO_MODEM_TRACE ("modemLog:DSK send DSP log finish,rd 0x%x,wr 0x%x",p_dsp_log_buff_info->rdIndex,p_dsp_log_buff_info->wrIndex);			 

                     //Must finish the total dsp log to send
			if(SIO_GetModemLogTotalSize(p_dsp_log_buff_info))
				break;

			 
			//Sec, send cp log
			for(v_i = 0; v_i < 2; v_i++)
	 	      {	
                            v_log_size = SIO_GetModemLogSize((void*)p_log_buff_info);
	 
	 			if(v_log_size)
	 			{
					SIO_SdcSendModemCpLog(v_log_size);
				}
	 			else
	 			 	break;
	                 }
	 		 //v_log_size =  (p_log_buff_info->length + p_log_buff_info->wrIndex -  p_log_buff_info->rdIndex) & (p_log_buff_info->length - 1);
	 		  
	 		 //SIO_MODEM_TRACE ("modemLog:DSK send CP log finish,rd 0x%x,wr 0x%x",p_log_buff_info->rdIndex,p_log_buff_info->wrIndex );


			#else

     			v_log_size = SIO_GetModemLogSize((void*)p_dsp_log_buff_info);
     
     			if(v_log_size)
     			{
     				SIO_SdcSendModemDspLog(v_log_size);                      
     			}

     			v_log_size = SIO_GetModemLogSize((void*)p_log_buff_info);
     
     			if(v_log_size)
     			{
     				SIO_SdcSendModemCpLog(v_log_size);
     			}
     
			#endif
			

			if((MODEM_LOG_SDC_TIMER != v_sio_modem_log_timer_value) && (s_sio_modem_log_usb_check_timer))
			{
				SCI_DeleteTimer(s_sio_modem_log_usb_check_timer);
				s_sio_modem_log_usb_check_timer = PNULL;
			}
			 break;
                	}

		case SIO_MODEM_AP_USB:
         	{

			SIO_SendModemCpLog();	// first, handle cp log
			SIO_SendModemDspLog();	//second, handle dsp log


			if((MODEM_LOG_USB_TIMER != v_sio_modem_log_timer_value) && (s_sio_modem_log_usb_check_timer)) 
			{
				SCI_DeleteTimer(s_sio_modem_log_usb_check_timer);
				s_sio_modem_log_usb_check_timer = PNULL;
			}
		       break;
		}	
		
		default:
			break;
      }
    }
}

#if 0

/*****************************************************************************/
// Description: Dsp log process Task
// Global resource dependence: sio_dsplog_trans_buf, dsp_log_semaphore, sio_dsp_log_headInfo_ptr
// Author: fei.zhang
// Note:
/*****************************************************************************/
LOCAL void SIO_ModemDspLogThread (uint32 id, void *pdata)
{
    uint32	v_log_size,v_len,v_i,v_sended_size;
    T_TRA_MODEM_LOG * p_log_buff_info =NULL;
    uint8 v_port;


    for (; ;)
    {
        SCI_GetSemaphore (modem_dsp_log_semaphore,SCI_WAIT_FOREVER); //SCI_WAIT_FOREVER

      v_port =  SIO_ModemTracePortNotify();

        if(NULL == p_modem_dsp_log_buff_info)
        {
            p_modem_dsp_log_buff_info = (T_TRA_MODEM_LOG *)GET_MemoryStartAddressAndLen(MEM_ZSP_DEBUG_NAME,&v_len);
        }
	 
	 p_log_buff_info = p_modem_dsp_log_buff_info;

        SIO_MODEM_TRACE ("modemLog:send DSP log,port %d",v_port);
	 v_len = 0;

	  if(p_log_buff_info)
         {   	        
               switch(v_port)
              {
                    case SIO_MODEM_AP_DSK:
	             {
              		for(v_i = 0; v_i < 2; v_i++)
			      {	
				      if(p_log_buff_info->wrIndex >= p_log_buff_info->rdIndex)
		 	                  v_log_size = p_log_buff_info->wrIndex -  p_log_buff_info->rdIndex;
		                    else
		                        v_log_size = p_log_buff_info->length - p_log_buff_info->rdIndex;

					if(v_log_size)
					{
					  //SIO_MODEM_TRACE ("modemLog:DSK send DSP log,rd 0x%x,sending 0x%x",p_log_buff_info->rdIndex,v_log_size);			 
			                 v_len = LogSaveOutput_Send_DSP_LOG((uint8*)((p_log_buff_info->head << 1) + p_log_buff_info->rdIndex),v_log_size);
					   SIO_MODEM_TRACE ("modemLog:DSK send DSP log,wr 0x%x,rd 0x%x,sending 0x%x,sended 0x%x",p_log_buff_info->wrIndex,p_log_buff_info->rdIndex,v_log_size,v_len);			 
                  			   //update the rd index
					  p_log_buff_info->rdIndex =  ( p_log_buff_info->rdIndex + v_len) & (p_log_buff_info->length - 1);		 
					}
					else
					 	break;
		                }
		 		  v_log_size =  (p_log_buff_info->length + p_log_buff_info->wrIndex -  p_log_buff_info->rdIndex) & (p_log_buff_info->length - 1);
		
		 		 //SIO_MODEM_TRACE ("modemLog:DSK send DSP log finish,rd 0x%x,remaind 0x%x",p_log_buff_info->rdIndex,v_log_size);		
				 break;
                    	}

			case SIO_MODEM_AP_USB:
			{
			    //SIO_SendModemDspLog();
			     break;
			}	
					
			default:
				break;
	      }
         }
    }
}
#endif

/*****************************************************************************/
// Description: 
// Global resource dependence: 
// Author:
// Note:
/*****************************************************************************/
LOCAL void SIO_ModemLogUsbCheckTimerExpired(void)
{
	SIO_MODEM_TRACE("modemLog:timer, CP wr0x%x,rd0x%x,DSP  wr0x%x,rd0x%x",p_modem_cp_log_buff_info->wrIndex,p_modem_cp_log_buff_info->rdIndex,p_modem_dsp_log_buff_info->wrIndex,p_modem_dsp_log_buff_info->rdIndex);

	//fix the converity38990. keep the original processing  and remove the length contraint 
      //if((SIO_GetModemLogTotalSize((void*)p_modem_cp_log_buff_info) >= MODEM_CP_LOG_MIN_SIZE)  || \
	//  	(SIO_GetModemLogTotalSize((void*)p_modem_dsp_log_buff_info) >= MODEM_DSP_LOG_MIN_SIZE) )
	//{
		SCI_PutSemaphore (modem_cp_log_semaphore);
      	//}


}


/*****************************************************************************/
// Description: usb send cp log api
// Global resource dependence: 
// Author:
// Note:
/*****************************************************************************/
LOCAL void SIO_SendModemCpLog(void)
{
	uint32 v_log_size=0;
	uint32 v_sended_size=0;
	uint32 v_fifo_size=0;
	uint32 v_sending_size = 0;	 
	uint32 v_send_flag=0;
	T_TRA_MODEM_LOG * p_log_buff_info =p_modem_cp_log_buff_info;

	if(SIO_MODEM_LOG_USB_TX_DISABLE == v_sio_modem_log_tx_status)
	 {
	 	return;
	}
	

	if((NULL == p_log_buff_info) || (MODEM_LOG_BUFF_OK != p_log_buff_info->status))
	{
		v_sio_modem_tx_type = SIO_MODEM_LOG_SEND_DSP;
		SCI_TraceLow("modemLog:USB send,cp buff is uninitialized");
		return;
	}


	  
	
	SCI_DisableIRQ();

	if(SCI_TRUE == v_sio_modem_log_tx_is_start)
	{	   
	   SCI_RestoreIRQ();
	   return;
	}
	
	v_sio_modem_log_tx_is_start = SCI_TRUE;	
	SCI_RestoreIRQ();

	if(SIO_MODEM_LOG_SEND_CP == v_sio_modem_tx_type)
	{	
		p_log_buff_info = p_modem_cp_log_buff_info;

#ifdef MODEM_LOG_USB_ZERO_COPY
		SCI_DisableIRQ();

		if(MODEM_LOG_USB_WORK == v_sio_modem_log_usb_flag)
		{
			v_sio_modem_log_usb_fifo_full_cnt++;  // to disard the log and free the log buffer
			if(v_sio_modem_log_usb_fifo_full_cnt < MODEM_USB_DISCARD_THRESHOLD)
			{
				v_sio_modem_log_tx_is_start = SCI_FALSE; 
				v_sio_modem_tx_type = SIO_MODEM_LOG_SEND_DSP;
				SCI_RestoreIRQ();
				return;
			}
			else
			{
				SCI_TraceLow("modemLog:USB send cp  log timerout and fail ");
			}
		}

		v_sio_modem_log_usb_fifo_full_cnt=0;
		v_sio_modem_log_usb_flag = MODEM_LOG_USB_WORK;
		SCI_RestoreIRQ();
		
		p_log_buff_info->rdIndex =  (p_log_buff_info->rdIndex + v_sio_modem_cp_log_sending_sz) & (p_log_buff_info->length - 1); 		

		if(!v_sio_modem_cp_log_sending_total_sz)
			v_sio_modem_cp_log_sending_total_sz = SIO_GetModemLogSize((void*)p_log_buff_info);

              //avoid the v_sio_modem_cp_log_sending_total_sz /  MODEM_CP_LOG_MAX_SIZE  remainder is less than  MODEM_CP_LOG_MIN_SIZE.
              //Distinguish between the first size  and "v_sio_modem_cp_log_sending_total_sz /  MODEM_CP_LOG_MAX_SIZE  remainder" less than MODEM_CP_LOG_MIN_SIZE
		if(v_sio_modem_cp_log_sending_total_sz > (MODEM_CP_LOG_MAX_SIZE + MODEM_CP_LOG_MIN_SIZE))
			v_sio_modem_cp_log_sending_sz = MODEM_CP_LOG_MAX_SIZE;
		else
			v_sio_modem_cp_log_sending_sz = v_sio_modem_cp_log_sending_total_sz;


		v_sending_size = v_sio_modem_cp_log_sending_sz;
		if((v_sending_size) && \
			((v_sending_size > MODEM_CP_LOG_MIN_SIZE) || (p_log_buff_info->rdIndex > p_log_buff_info->wrIndex)))
		{
			v_send_flag = 1;
			v_sio_modem_log_usb_cp_threshold_cnt=0;
		}
		else
		{
			v_send_flag=0;
			if(v_sending_size)
				v_sio_modem_log_usb_cp_threshold_cnt++;
			
			if(v_sio_modem_log_usb_cp_threshold_cnt > 3)
			{
				v_send_flag = 1;
				v_sio_modem_log_usb_cp_threshold_cnt = 0;
			}
			else
			{						
				v_sio_modem_cp_log_sending_sz = 0;
				v_sio_modem_cp_log_sending_total_sz = 0;
			}

		}


		if(v_send_flag)
		{		    
			v_sended_size = ((sio_port[MODME_LOG_COM_PORT].sio_op.write) (SIO_PhyPortfromIndex (MODEM_LOG_USB_VIR_COM), \
					(uint8 *)( p_log_buff_info->rdIndex+p_log_buff_info->head),v_sending_size));	
			SIO_MODEM_TRACE ("modemLog:USB send cp log,rd 0x%x,sending 0x%x,sended 0x%x,total 0x%x",p_log_buff_info->rdIndex,v_sending_size,v_sended_size,v_log_size);		   

                     if(v_sended_size)
                     {
                     	v_sio_modem_cp_log_sending_sz = v_sended_size;
				v_sio_modem_cp_log_sending_total_sz -= v_sended_size;		
				SCI_DisableIRQ();
				v_sio_modem_log_tx_is_start = SCI_FALSE; 
				SCI_RestoreIRQ();
                     }
			else
			{     
			       // USB abnormal,discard the log and update the read inedx with v_log_size
			       v_sending_size = v_sio_modem_cp_log_sending_total_sz;
			       p_log_buff_info->rdIndex =  (p_log_buff_info->rdIndex + v_sending_size) & (p_log_buff_info->length - 1); 		
                            v_sio_modem_cp_log_sending_sz = 0; // clear sending size. 
                            v_sio_modem_cp_log_sending_total_sz = 0;
				v_sio_modem_cp_log_lost_cnt++;
				SCI_DisableIRQ();
				v_sio_modem_tx_type = SIO_MODEM_LOG_SEND_DSP; // next send the dsp log	
				v_sio_modem_log_tx_is_start = SCI_FALSE; 
				v_sio_modem_log_usb_flag = MODEM_LOG_USB_IDLE;
				SCI_RestoreIRQ();
				SCI_TraceLow("modemLog:USB send CP log fail,lost 0x%x,wr0x%x,rd0x%x,ofi0x%x",(v_sending_size),p_log_buff_info->wrIndex,p_log_buff_info->rdIndex,p_log_buff_info->overflowIndex);
	
			}
		}
		else	   
		{			
			SCI_DisableIRQ();
		 	v_sio_modem_tx_type = SIO_MODEM_LOG_SEND_DSP; // next send the dsp log	
		 	v_sio_modem_log_tx_is_start = SCI_FALSE; 	
			v_sio_modem_log_usb_flag = MODEM_LOG_USB_IDLE;
			SCI_RestoreIRQ();
		}
#else
		v_log_size = SIO_GetModemLogSize((void*)p_log_buff_info);

		v_fifo_size = SIO_GetTxFifoCnt (MODME_LOG_COM_PORT);
		
		if(v_log_size >= v_fifo_size)	  
			v_sending_size = v_fifo_size;
		else
			v_sending_size = v_log_size;


		if(v_log_size  && v_fifo_size && \
			((v_log_size > MODEM_CP_LOG_MIN_SIZE) || (p_log_buff_info->rdIndex > p_log_buff_info->wrIndex)))
		{
			v_send_flag = 1;
		}
		else
			v_send_flag=0;

		if(v_fifo_size)
		{
			v_sio_modem_log_usb_fifo_full_cnt = 0;
		}
		else
		{
			v_sio_modem_log_usb_fifo_full_cnt++;  // to disard the log and free the log buffer
			if(v_sio_modem_log_usb_fifo_full_cnt < MODEM_USB_DISCARD_THRESHOLD)
			{
				v_send_flag=0;
			}
			else
				v_send_flag=1;
		}

		if((v_send_flag))
		{		    
			v_sended_size = ((sio_port[MODME_LOG_COM_PORT].sio_op.write) (SIO_PhyPortfromIndex (MODEM_LOG_USB_VIR_COM), \
					(uint8 *)( p_log_buff_info->rdIndex+p_log_buff_info->head),v_sending_size));	
			SIO_MODEM_TRACE ("modemLog:USB send cp log,rd 0x%x,sending 0x%x,sended 0x%x,total 0x%x",p_log_buff_info->rdIndex,v_sending_size,v_sended_size,v_log_size);		   

                     if(v_sended_size)
			{
				p_log_buff_info->rdIndex =  (p_log_buff_info->rdIndex + v_sending_size) & (p_log_buff_info->length - 1); 		
				v_log_size -=  v_sending_size;
                     }
			else
			{     
			       // USB abnormal,discard the log and update the read inedx with v_log_size
				p_log_buff_info->rdIndex =  (p_log_buff_info->rdIndex + v_log_size) & (p_log_buff_info->length - 1); 
				v_sio_modem_cp_log_lost_cnt++;
				SCI_TraceLow("modemLog:USB send CP log fail,lost 0x%x Bytes,wr0x%x,rd0x%x",(v_log_size),p_log_buff_info->wrIndex,p_log_buff_info->rdIndex);
				v_log_size = 0;
			}

			SCI_DisableIRQ();
			if(0 == v_log_size) //finish cp send 
			{
				v_sio_modem_tx_type = SIO_MODEM_LOG_SEND_DSP; //next send the dsp log 
			}
			v_sio_modem_log_tx_is_start = SCI_FALSE; 
			SCI_RestoreIRQ();
		}
		else	   
		{			
			SCI_DisableIRQ();
		 	v_sio_modem_tx_type = SIO_MODEM_LOG_SEND_DSP; // next send the dsp log	
		 	v_sio_modem_log_tx_is_start = SCI_FALSE; 	
			SCI_RestoreIRQ();
		}
 #endif
      }
	else
	{		
		SCI_DisableIRQ();
		v_sio_modem_tx_type=SIO_MODEM_LOG_SEND_DSP;  // next send the dsp log	
		v_sio_modem_log_tx_is_start = SCI_FALSE; 	
		SCI_RestoreIRQ();		
	}
}


/*****************************************************************************/
// Description: usb send DSP log api
// Global resource dependence: 
// Author:
// Note:
/*****************************************************************************/
LOCAL void SIO_SendModemDspLog(void)
{
	uint32 v_log_size=0;
	uint32 v_sended_size=0;
	uint32 v_fifo_size=0;
	uint32 v_sending_size = 0;	
	uint32 v_send_flag=0;
	T_TRA_MODEM_LOG * p_log_buff_info =p_modem_dsp_log_buff_info;

	if(SIO_MODEM_LOG_USB_TX_DISABLE == v_sio_modem_log_tx_status)
	 {
	 	return;
	}


	if((NULL == p_log_buff_info) || (MODEM_LOG_BUFF_OK != p_log_buff_info->status))
	{
		v_sio_modem_tx_type = SIO_MODEM_LOG_SEND_CP;
		SCI_TraceLow("modemLog:USB send,dsp buff is uninitialized");
		return;
	}

	SCI_DisableIRQ();

	if(SCI_TRUE == v_sio_modem_log_tx_is_start)
	{	   
	   SCI_RestoreIRQ();
	   return;
	}
	
	v_sio_modem_log_tx_is_start = SCI_TRUE; 	

	SCI_RestoreIRQ();


	if(SIO_MODEM_LOG_SEND_DSP == v_sio_modem_tx_type)
	{	
		p_log_buff_info = p_modem_dsp_log_buff_info;

#ifdef MODEM_LOG_USB_ZERO_COPY
		SCI_DisableIRQ();

		if(MODEM_LOG_USB_WORK == v_sio_modem_log_usb_flag)
		{	
			v_sio_modem_log_usb_fifo_full_cnt++;
			if(v_sio_modem_log_usb_fifo_full_cnt < MODEM_USB_DISCARD_THRESHOLD)
			{
				v_sio_modem_log_tx_is_start = SCI_FALSE; 
				v_sio_modem_tx_type = SIO_MODEM_LOG_SEND_CP;
				SCI_RestoreIRQ();
				return;
			}
			else
			{
				SCI_TraceLow("modemLog:USB send dsp log timerout and fail ");
			}
		}

		v_sio_modem_log_usb_fifo_full_cnt=0;
		v_sio_modem_log_usb_flag = MODEM_LOG_USB_WORK;
		SCI_RestoreIRQ();

		p_log_buff_info->rdIndex =  (p_log_buff_info->rdIndex + v_sio_modem_dsp_log_sending_sz) & (p_log_buff_info->length - 1); 	

		if(!v_sio_modem_dsp_log_sending_total_sz)
			v_sio_modem_dsp_log_sending_total_sz = SIO_GetModemLogSize((void*)p_log_buff_info);

              //avoid the v_sio_modem_dsp_log_sending_total_sz /  MODEM_DSP_LOG_MAX_SIZE  remainder is less than  MODEM_DSP_LOG_MIN_SIZE.
              //Distinguish between the first size  and "v_sio_modem_dsp_log_sending_total_sz /  MODEM_DSP_LOG_MAX_SIZE  remainder" less than MODEM_DSP_LOG_MIN_SIZE
		if(v_sio_modem_dsp_log_sending_total_sz > (MODEM_DSP_LOG_MAX_SIZE + MODEM_DSP_LOG_MIN_SIZE))
			v_sio_modem_dsp_log_sending_sz = MODEM_DSP_LOG_MAX_SIZE;
		else
			v_sio_modem_dsp_log_sending_sz = v_sio_modem_dsp_log_sending_total_sz;


		
		v_sending_size = v_sio_modem_dsp_log_sending_sz;
		if(v_sending_size && \
			((v_sending_size > MODEM_DSP_LOG_MIN_SIZE) || (p_log_buff_info->rdIndex > p_log_buff_info->wrIndex)))
		{
			v_send_flag = 1;
			v_sio_modem_log_usb_dsp_threshold_cnt = 0;
		}
		else
		{
			v_send_flag=0;
			
			if(v_sending_size)
				v_sio_modem_log_usb_dsp_threshold_cnt++;
			
			if(v_sio_modem_log_usb_dsp_threshold_cnt > 3)
			{
				v_send_flag =1;
				v_sio_modem_log_usb_dsp_threshold_cnt = 0;
			}
			else
			{
				v_sio_modem_dsp_log_sending_sz = 0;
				v_sio_modem_dsp_log_sending_total_sz = 0;
			}
		}
		

		if((v_send_flag ))
		{			
			v_sended_size = ((sio_port[MODME_LOG_COM_PORT].sio_op.write) (SIO_PhyPortfromIndex (MODEM_LOG_USB_VIR_COM), \
					(uint8 *)( p_log_buff_info->rdIndex+(p_log_buff_info->head << 1)),v_sending_size));	
			SIO_MODEM_TRACE ("modemLog:USB send dsp log,rd 0x%x,sending 0x%x,sended 0x%x,total 0x%x",p_log_buff_info->rdIndex,v_sending_size,v_sended_size,v_log_size);		   

                     if(v_sended_size)
                     {
                     	v_sio_modem_dsp_log_sending_sz = v_sended_size;
				v_sio_modem_dsp_log_sending_total_sz -= v_sended_size;
				SCI_DisableIRQ();
				v_sio_modem_log_tx_is_start = SCI_FALSE;
				SCI_RestoreIRQ();
	
                     }
			else
			{     
			       // USB abnormal,discard the log and update the read inedx with v_log_size
			       v_sending_size = v_sio_modem_dsp_log_sending_total_sz;
				p_log_buff_info->rdIndex =  (p_log_buff_info->rdIndex + v_sending_size) & (p_log_buff_info->length - 1); 		
			       v_sio_modem_dsp_log_sending_sz = 0; // clear sending size. 
			       v_sio_modem_dsp_log_sending_total_sz = 0;
				v_sio_modem_dsp_log_lost_cnt++;
				
				SCI_DisableIRQ();
				v_sio_modem_tx_type = SIO_MODEM_LOG_SEND_CP; //usb callback send the cp log 				
				v_sio_modem_log_tx_is_start = SCI_FALSE;
				v_sio_modem_log_usb_flag = MODEM_LOG_USB_IDLE;
				SCI_RestoreIRQ();

				SCI_TraceLow("modemLog:USB send DSP log fail,lost 0x%x,wr0x%x,rd0x%x,ofi0x%x",v_sending_size,p_log_buff_info->wrIndex,p_log_buff_info->rdIndex,p_log_buff_info->overflowIndex);

			}
			

		}
		else	   
		{			
			SCI_DisableIRQ();
			v_sio_modem_tx_type = SIO_MODEM_LOG_SEND_CP; // next send the cp log				 
			v_sio_modem_log_tx_is_start = SCI_FALSE;	
			v_sio_modem_log_usb_flag = MODEM_LOG_USB_IDLE;
			SCI_RestoreIRQ();
		}
#else
		v_log_size = SIO_GetModemLogSize((void*)p_log_buff_info);

		v_fifo_size = SIO_GetTxFifoCnt (MODME_LOG_COM_PORT);

		
		if(v_log_size >= v_fifo_size)	  
			v_sending_size = v_fifo_size;
		else
			v_sending_size = v_log_size;

		

		if(v_log_size && v_fifo_size && \
			((v_log_size > MODEM_DSP_LOG_MIN_SIZE) || (p_log_buff_info->rdIndex > p_log_buff_info->wrIndex)))
		{
			v_send_flag = 1;
		}
		else
			v_send_flag=0;
	
		if(v_fifo_size)
		{
			v_sio_modem_log_usb_fifo_full_cnt = 0;
		}
		else
		{
			v_sio_modem_log_usb_fifo_full_cnt++;  // to disard the log and free the log buffer
			if(v_sio_modem_log_usb_fifo_full_cnt < MODEM_USB_DISCARD_THRESHOLD)
			{
				v_send_flag=0;
			}
			else
				v_send_flag=1;
		}
		
		if((v_send_flag))
		{			
			v_sended_size = ((sio_port[MODME_LOG_COM_PORT].sio_op.write) (SIO_PhyPortfromIndex (MODEM_LOG_USB_VIR_COM), \
					(uint8 *)( p_log_buff_info->rdIndex+(p_log_buff_info->head << 1)),v_sending_size));	
			SIO_MODEM_TRACE ("modemLog:USB send dsp log,rd 0x%x,sending 0x%x,sended 0x%x,total 0x%x",p_log_buff_info->rdIndex,v_sending_size,v_sended_size,v_log_size);		   

                     if(v_sended_size)
			{
				p_log_buff_info->rdIndex =  (p_log_buff_info->rdIndex + v_sending_size) & (p_log_buff_info->length - 1); 		
				v_log_size -=  v_sending_size;
                     }
			else
			{     
			       // USB abnormal,discard the log and update the read inedx with v_log_size
				p_log_buff_info->rdIndex =  (p_log_buff_info->rdIndex + v_log_size) & (p_log_buff_info->length - 1); 		
				v_sio_modem_dsp_log_lost_cnt++;
				SCI_TraceLow("modemLog:USB send DSP log fail,lost 0x%x Bytes,wr0x%x,rd0x%x",v_log_size,p_log_buff_info->wrIndex,p_log_buff_info->rdIndex);
				v_log_size = 0;
			}
			
			SCI_DisableIRQ();
			if(0 == v_log_size) //finish dsp send 
			{
				v_sio_modem_tx_type = SIO_MODEM_LOG_SEND_CP; //next send the cp log 				
			}
			v_sio_modem_log_tx_is_start = SCI_FALSE;
			SCI_RestoreIRQ();

		}
		else	   
		{			
			SCI_DisableIRQ();
			v_sio_modem_tx_type = SIO_MODEM_LOG_SEND_CP; // next send the cp log				 
			v_sio_modem_log_tx_is_start = SCI_FALSE;	
			SCI_RestoreIRQ();
		}
#endif
	  }
	else
	{		
		SCI_DisableIRQ();
		v_sio_modem_tx_type=SIO_MODEM_LOG_SEND_CP;  // next send the cp log 
		v_sio_modem_log_tx_is_start = SCI_FALSE;			
		SCI_RestoreIRQ();		
	}
}


/*****************************************************************************/
// Description: send  cp log  with SDC 
// Global resource dependence: 
// Author:
// Note:
/*****************************************************************************/
LOCAL void SIO_SdcSendModemCpLog(uint32 send_size)
{
	uint32 v_len=0;	 
	T_TRA_MODEM_LOG * p_log_buff_info =p_modem_cp_log_buff_info;

	if((SIO_MODEM_LOG_SDC_TX_DISABLE == v_sio_modem_log_sdc_tx_status) || (NULL == p_log_buff_info))
	 {
	 	return;
	}


	if((NULL == p_log_buff_info) || (MODEM_LOG_BUFF_OK != p_log_buff_info->status))
		return;

	
	SCI_DisableIRQ();

	if(SCI_TRUE == v_sio_modem_cp_log_sdc_tx_is_start)
	{	   
	   SCI_RestoreIRQ();
	   return;
	}
	
	v_sio_modem_cp_log_sdc_tx_is_start = SCI_TRUE;	
	SCI_RestoreIRQ();

	if(send_size)
	{
		//SIO_MODEM_TRACE ("modemLog:DSK send CP log,rd 0x%x,sending 0x%x",p_log_buff_info->rdIndex,v_log_size);			 
		 v_len = LogSaveOutput_Send_CP_LOG((uint8*)(p_log_buff_info->head + p_log_buff_info->rdIndex),send_size);
		//update the rd index
		if(v_len)
		{
			p_log_buff_info->rdIndex =  ( p_log_buff_info->rdIndex + v_len) & (p_log_buff_info->length - 1);
			v_sio_modem_cp_log_lost_cnt=0;
		}
		else
		{
			v_sio_modem_cp_log_lost_cnt++;
			if((v_sio_modem_cp_log_lost_cnt & 0x4) == 0x4)
			{
			    //send fail, discard the log and update the read index with send_size
			     p_log_buff_info->rdIndex =  ( p_log_buff_info->rdIndex + send_size) & (p_log_buff_info->length - 1);
			     SCI_TraceLow("modemLog:DSK send CP log fail %d lost 0x%x,wr0x%x,rd0x%x,ofi0x%x",v_sio_modem_cp_log_lost_cnt,(send_size),p_log_buff_info->wrIndex,p_log_buff_info->rdIndex,p_log_buff_info->overflowIndex);
			}

			
		}
	
		SIO_MODEM_TRACE ("modemLog:DSK send CP log,wr 0x%x,rd 0x%x,sending 0x%x,sended 0x%x",p_log_buff_info->wrIndex,p_log_buff_info->rdIndex,send_size,v_len);			 
	}

	SCI_DisableIRQ();
	v_sio_modem_cp_log_sdc_tx_is_start = SCI_FALSE;	
	SCI_RestoreIRQ();

	
}


/*****************************************************************************/
// Description:send  dsp log  with SD 
// Global resource dependence: 
// Author:
// Note:
/*****************************************************************************/
LOCAL void SIO_SdcSendModemDspLog(uint32 send_size)
{
	uint32 v_len=0;	 
	T_TRA_MODEM_LOG * p_log_buff_info =p_modem_dsp_log_buff_info;

	if((SIO_MODEM_LOG_SDC_TX_DISABLE == v_sio_modem_log_sdc_tx_status) || (NULL == p_log_buff_info))
	 {
	 	return;
	}

	if((NULL == p_log_buff_info) || (MODEM_LOG_BUFF_OK != p_log_buff_info->status))
		return;

	
	SCI_DisableIRQ();

	if(SCI_TRUE == v_sio_modem_dsp_log_sdc_tx_is_start)
	{	   
	   SCI_RestoreIRQ();
	   return;
	}
	
	v_sio_modem_dsp_log_sdc_tx_is_start = SCI_TRUE;	
	SCI_RestoreIRQ();

	if(send_size)
	{
		//SIO_MODEM_TRACE ("modemLog:DSK send dsp log,rd 0x%x,sending 0x%x",p_log_buff_info->rdIndex,v_log_size);			 
		 v_len = LogSaveOutput_Send_CP_LOG((uint8*)((p_log_buff_info->head << 1) + p_log_buff_info->rdIndex),send_size);
		//update the rd index
		if(v_len)
		{
			p_log_buff_info->rdIndex =  ( p_log_buff_info->rdIndex + v_len) & (p_log_buff_info->length - 1);	
			v_sio_modem_dsp_log_lost_cnt=0;
		}
		else
		{	
			v_sio_modem_dsp_log_lost_cnt++;
			if((v_sio_modem_dsp_log_lost_cnt & 0x4) == 0x4)
			{
				//send fail, discard the log and update the read index with send_size
				p_log_buff_info->rdIndex =  ( p_log_buff_info->rdIndex + send_size) & (p_log_buff_info->length - 1);	
				SCI_TraceLow("modemLog:DSK send DSP log fail %d,lost 0x%x,wr0x%x,rd0x%x,ofi0x%x",v_sio_modem_dsp_log_lost_cnt,send_size,p_log_buff_info->wrIndex,p_log_buff_info->rdIndex,p_log_buff_info->overflowIndex);
			} 
		}
		SIO_MODEM_TRACE ("modemLog:DSK send dsp log,wr 0x%x,rd 0x%x,sending 0x%x,sended 0x%x",p_log_buff_info->wrIndex,p_log_buff_info->rdIndex,send_size,v_len);			 
	}

	SCI_DisableIRQ();
	v_sio_modem_dsp_log_sdc_tx_is_start = SCI_FALSE;	
	SCI_RestoreIRQ();

	
}

/*****************************************************************************/
// Description:callback to send the  the dsp log and cp log with the same usb port
// Global resource dependence: 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void SIO_ModemLogUsbSendCallBack(void)
{
	uint32 v_log_size=0,v_sended_size;   
	T_TRA_MODEM_LOG * p_log_buff_info =NULL;

	   
	if((NULL == p_modem_cp_log_buff_info) || (NULL == p_modem_dsp_log_buff_info))
	{
           v_sio_modem_tx_type = SIO_MODEM_LOG_SEND_CP; // send the cp log 	       
           SCI_PutSemaphore (modem_cp_log_semaphore);
	    return;
	}


#ifdef MODEM_LOG_USB_ZERO_COPY
	SCI_DisableIRQ();
	v_sio_modem_log_usb_flag = MODEM_LOG_USB_FINISH; 
	SCI_RestoreIRQ();
#endif
	   
	SIO_SendModemCpLog();    // first, handle cp log

	SIO_SendModemDspLog();  //second, handle dsp log


	return;
}


/*****************************************************************************/
// Description: CP notify AP send log with mailbox irq
// Global resource dependence: 
// Author:
// Note:
/*****************************************************************************/
PUBLIC void SIO_SendModemLogNotify(void)
{
	uint8 v_port;
	uint32	v_log_size,v_len,v_i,v_sended_size;

	v_port =  SIO_GetModemTracePort();

	if((NULL == p_modem_cp_log_buff_info) || (NULL == p_modem_dsp_log_buff_info))
	{
           v_sio_modem_tx_type = SIO_MODEM_LOG_SEND_CP; // send the cp log 	       
           SCI_PutSemaphore (modem_cp_log_semaphore);
	    return;
	}

	
	switch(v_port)
	{
		case SIO_MODEM_AP_USB:
		{                

			if (PNULL == s_sio_modem_log_usb_check_timer)
			{
				v_sio_modem_log_timer_value = MODEM_LOG_USB_TIMER;
				s_sio_modem_log_usb_check_timer = SCI_CreatePeriodTimer("MdLogUsbCheck",
			                                 SIO_ModemLogUsbCheckTimerExpired, 0, MODEM_LOG_USB_TIMER, SCI_AUTO_ACTIVATE);
			}
			
			SIO_SendModemCpLog();    // first, handle cp log

			SIO_SendModemDspLog();  //second, handle dsp log

			break;
		}	

		case SIO_MODEM_AP_DSK:
		{

			if (PNULL == s_sio_modem_log_usb_check_timer)
			{
				v_sio_modem_log_timer_value = MODEM_LOG_SDC_TIMER;
				s_sio_modem_log_usb_check_timer = SCI_CreatePeriodTimer("MdLogUsbCheck",
			                                 SIO_ModemLogUsbCheckTimerExpired, 0, MODEM_LOG_SDC_TIMER, SCI_AUTO_ACTIVATE);
			}

		       //first,send dsp log
			 v_log_size = SIO_GetModemLogSize((void*)p_modem_dsp_log_buff_info);
 			if(v_log_size)
 			{
 			   //limit the log size in the hirs
 			   if(v_log_size > 2048)
                            v_log_size = 2048;
			   
			   SIO_SdcSendModemDspLog(v_log_size);
 			}

			if(0 == SIO_GetModemLogTotalSize(p_modem_cp_log_buff_info))
			{
				 //Sec,send cp log
				 v_log_size = SIO_GetModemLogSize((void*)p_modem_cp_log_buff_info);
	 			if(v_log_size)
	 			{
	 			   //limit the log size in the hirs
	 			   if(v_log_size > 3072)
	                            v_log_size = 3072;
				   
				   SIO_SdcSendModemCpLog(v_log_size);
	 			}
			}


			SCI_PutSemaphore (modem_cp_log_semaphore);
			break;
		}

		default:
			break;
	}



}

//status: 0--disable usb tx 1--enable usb tx
PUBLIC void SIO_ModemLogReset(uint8 status)
{    
	uint32 v_len;

	if(status)
	{  		
		SIO_MODEM_TRACE ("modemLog:enable reset");
        }
        else
    	{	

        	SIO_MODEM_TRACE ("modemLog:disable reset");
	 }


	if(NULL == p_modem_cp_log_buff_info)
	{
		p_modem_cp_log_buff_info = (T_TRA_MODEM_LOG *)GET_MemoryStartAddressAndLen(MEM_CP_DEBUG_NAME,&v_len);
	}

	p_modem_cp_log_buff_info->rdIndex = ( p_modem_cp_log_buff_info->rdIndex + 4) & (p_modem_cp_log_buff_info->length - 1); 	 

	if(NULL == p_modem_dsp_log_buff_info)
	{
        		p_modem_dsp_log_buff_info = (T_TRA_MODEM_LOG *)GET_MemoryStartAddressAndLen(MEM_ZSP_DEBUG_NAME,&v_len);
	}

	p_modem_dsp_log_buff_info->rdIndex = ( p_modem_dsp_log_buff_info->rdIndex + 4) & (p_modem_dsp_log_buff_info->length - 1); 	 		
}

/*****************************************************************************/
// Description: Dsp log task creat
// Global resource dependence: dsp_log_semaphore, dsp_log_thread_id, s_sio_dsp_frame_ctrl
// Author: fei.zhang
// Note:
/*****************************************************************************/

PUBLIC void SIO_CreateModemLogThread (void)
{

    if (SCI_NULL == modem_cp_log_semaphore)
    {
        modem_cp_log_semaphore = SCI_CreateSemaphore ("MODEM_CP_LOG_SEMAPHORE", 0);
    }

    SCI_PASSERT ( (modem_cp_log_semaphore != SCI_NULL), ("Create MODEM_CP_LOG_SEMAPHORE failed!"));/*assert verified*/


    if (SCI_INVALID_BLOCK_ID == modem_cp_log_thread_id)
    {
        modem_cp_log_thread_id = SCI_CreateThread ("MODEM_CP_LOG_Task", "MODEM_CP_LOG_QUEUE",
                                              SIO_ModemCpLogThread/*void(* entry)(uint32,void *)*/,
                                              0, NULL, 2048, 1,
                                              TASK_MODEM_CP_LOG_PRI,  // same priority as sme
                                              SCI_PREEMPT,
                                              SCI_AUTO_START);
    }

    SCI_PASSERT ( (modem_cp_log_thread_id != SCI_INVALID_BLOCK_ID), ("Create MODEM CP LOG task failed!"));/*assert verified*/

    SCI_TRACE_LOW ("!!!Create SIO_ModemCpLogThread successfully!!!");

#if 0
    if (SCI_NULL == modem_dsp_log_semaphore)
    {
        modem_dsp_log_semaphore = SCI_CreateSemaphore ("MODEM_DSP_LOG_SEMAPHORE", 0);
    }

    SCI_PASSERT ( (modem_dsp_log_semaphore != SCI_NULL), ("Create MODEM_DSP_LOG_SEMAPHORE failed!"));/*assert verified*/


    if (SCI_INVALID_BLOCK_ID == modem_dsp_log_thread_id)
    {
        modem_dsp_log_thread_id = SCI_CreateThread ("MODEM_DSP_LOG_Task", "MODEM_DSP_LOG_QUEUE",
                                              SIO_ModemDspLogThread/*void(* entry)(uint32,void *)*/,
                                              0, NULL, 2048, 1,
                                              TASK_MODEM_DSP_LOG_PRI,  // same priority as sme
                                              SCI_PREEMPT,
                                              SCI_AUTO_START);
    }

    SCI_PASSERT ( (modem_dsp_log_thread_id != SCI_INVALID_BLOCK_ID), ("Create MODEM DSP LOG task failed!"));/*assert verified*/
#endif
    SCI_TRACE_LOW ("!!!Create SIO_ModemDspLogThread successfully!!!");
	

}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif /* End of sio.c*/
