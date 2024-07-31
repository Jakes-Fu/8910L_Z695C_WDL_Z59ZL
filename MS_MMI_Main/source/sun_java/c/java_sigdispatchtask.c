/****************************************************************************
** File Name:       java_sigdispatchtask.c                                              *
** Author:                                                                 *
** Date:           27/11/2010                                            *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the dm task           *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
**   27/11/2010     ying.xu           this file is singal dispath  task for sun java  
****************************************************************************/


#include "mmi_sun_java_trc.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "sci_service.h"
#include "os_api.h"
#include "mn_api.h"
#include "in_message.h"
#include "app_tcp_if.h"

#include "java_socket.h"
#include "javacall_logging.h"
#include "javacall_lifecycle.h"
#include "javacall_time.h"
#include "javacall_input.h"
#include "javacall_file.h"
#include "javacall_dir.h"
#include "javacall_lcd.h"
#include "javacall_multimedia.h"
#include "javacall_network.h"
#include "javacall_socket.h"
#include "javacall_datagram.h"
#include "socket_types.h" 
#include "priority_app.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define     STACK_SIZE                 (1024)           // task stack size
#define     STACK_QUEUE_NUM            (20)         // task stack queue number
#define     TASK_PRIORITY             PRI_JAVA_SIGDISPATCH_TASK   // (30)               // task priority

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL  uint32           s_java_sigdispatch_task_id = 0;                     // task id

extern char* pcHostAddress;
//extern char * pcHostName;

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
static BOOLEAN  java_HandleTCPIPMessage(void* pSignal);


/*****************************************************************************/
//  Description :   task handlement
//  Global resource dependence : 
//  Author:ying.xu
//  Note: 
/*****************************************************************************/
LOCAL void java_SigDispatchTask(uint32 argc, void* argv)
{
    void*                      signal_ptr = PNULL; 
    //SCI_TRACE_LOW:"java_SigDispatchTask : task start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JAVA_SIGDISPATCHTASK_61_112_2_18_3_26_33_0,(uint8*)"");
    
    while(1)/*lint !e716 */
    {
        signal_ptr = SCI_GetSignal(SCI_IdentifyThread());
        if (PNULL != signal_ptr)
        {
            //SCI_TRACE_LOW:"java_SigDispatchTask, get signal code =0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,JAVA_SIGDISPATCHTASK_68_112_2_18_3_26_33_1,(uint8*)"d",((xSignalHeader)signal_ptr)->SignalCode );
            java_HandleTCPIPMessage(signal_ptr);           
        }        
        SCI_FREE(signal_ptr); 
        signal_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : create  task
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/*****************************************************************************/
PUBLIC uint32 java_CreateSigDispatchTask(void)
{
    if (0 != s_java_sigdispatch_task_id)
    {
        //之前创建的task还没有关闭!
        //SCI_TRACE_LOW:"java_CreateSigDispatchTask has been created"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,JAVA_SIGDISPATCHTASK_85_112_2_18_3_26_33_2,(uint8*)"");
        return 0;
    }
    
    s_java_sigdispatch_task_id = SCI_CreateThread( 
        "T_P_JAVA_SIGDISPATCH", 
        "T_Q_JAVA_SIGDISPATCH", 
        java_SigDispatchTask, 
        0, 
        0, 
        STACK_SIZE, 
        STACK_QUEUE_NUM, 
        TASK_PRIORITY, 
        SCI_PREEMPT, 
        SCI_AUTO_START);
    
    //SCI_TRACE_LOW:"java_CreateSigDispatchTask created, task id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JAVA_SIGDISPATCHTASK_101_112_2_18_3_26_33_3,(uint8*)"d",s_java_sigdispatch_task_id);
    return s_java_sigdispatch_task_id;
}

/*****************************************************************************/
//  Description : get  task id             
//  Global resource dependence : 
//  Author: ying.xu
//  Note:   
/*****************************************************************************/
PUBLIC uint32 java_GetSigDispatchTaskID(void)
{
    return s_java_sigdispatch_task_id;
}

/*****************************************************************************/
//  Description : get  task id             
//  Global resource dependence : 
//  Author: ying.xu
//  Note:   
/*****************************************************************************/

static BOOLEAN  java_HandleTCPIPMessage(void* pSignal)
{   
    int32 socket_id     =   0;
    int32 socket_type   = 0;
    int32 msg_id        =  (int32)(((xSignalHeader)pSignal)->SignalCode) ;
    javacall_result java_result = JAVACALL_OK;
    javacall_printf(LOG_STR_ENTER("msg_id = 0x%x"),msg_id );
    
    switch (msg_id)
    {        
    case SOCKET_READ_EVENT_IND:
        socket_id = ((SOCKET_READ_EVENT_IND_SIG_T*)pSignal)->socket_id;    
        socket_type = java_GetSocketTypeBySocket(socket_id);
        javacall_printf(LOG_STR("socket_id = %d, type = %d"),socket_id ,socket_type);
        if(SOCK_STREAM  == socket_type)
        {            
            javanotify_socket_event(JAVACALL_EVENT_SOCKET_RECEIVE, (javacall_handle)socket_id,  JAVACALL_OK);
        }
        else if(SOCK_DGRAM == socket_type)
        {
            javanotify_datagram_event(JAVACALL_EVENT_DATAGRAM_RECVFROM_COMPLETED, (javacall_handle)socket_id, JAVACALL_OK);
        }        
        break;
        
        //    case SOCKET_READ_BUFFER_STATUS_EVENT_IND:
        //        socket_id = ((SOCKET_READ_BUFFER_STATUS_EVENT_IND_SIG_T*)pSingal)->socket_id;
        //        break;
        
    case SOCKET_WRITE_EVENT_IND:
        socket_id = ((SOCKET_WRITE_EVENT_IND_SIG_T*)pSignal)->socket_id;
        socket_type = java_GetSocketTypeBySocket(socket_id);
        javacall_printf(LOG_STR("socket_id = %d, type = %d"),socket_id ,socket_type);
        if(SOCK_STREAM  == socket_type)
        {            
            javanotify_socket_event(JAVACALL_EVENT_SOCKET_SEND, (javacall_handle)socket_id,  JAVACALL_OK);
        }
        else  if(SOCK_DGRAM  == socket_type)
        {
            javanotify_datagram_event(JAVACALL_EVENT_DATAGRAM_SENDTO_COMPLETED, (javacall_handle)socket_id, JAVACALL_OK);
        }
        break;
        
    case SOCKET_CONNECT_EVENT_IND:
        socket_id = ((SOCKET_CONNECT_EVENT_IND_SIG_T*)pSignal)->socket_id;      
        socket_type = java_GetSocketTypeBySocket(socket_id);
        if( 0 ==  ((SOCKET_CONNECT_EVENT_IND_SIG_T*)pSignal)->error_code)
        {
            java_result = JAVACALL_OK;            
        }
        else
        {
            java_result = JAVACALL_FAIL;            
        }
        javacall_printf(LOG_STR("socket_id = %d, type = %d, error = %d"),socket_id ,socket_type, java_result);
        if(SOCK_STREAM  == socket_type)
        {             
            javanotify_socket_event(JAVACALL_EVENT_SOCKET_OPEN_COMPLETED, (javacall_handle)socket_id, java_result);
        }        
        break;
    case SOCKET_CONNECTION_CLOSE_EVENT_IND:
        socket_id = ((SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T*)pSignal)->socket_id;     
        socket_type = java_GetSocketTypeBySocket(socket_id);
        if( 0 ==  ((SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T*)pSignal)->error_code)
        {
            java_result = JAVACALL_OK;            
        }
        else
        {
            java_result = JAVACALL_FAIL;            
        } 
        javacall_printf(LOG_STR(" socket_id = %d, type = %d, error = %d"),socket_id ,socket_type, java_result);
        if(SOCK_STREAM  == socket_type)
        {                       
            javanotify_socket_event(JAVACALL_EVENT_SOCKET_REMOTE_DISCONNECTED, (javacall_handle)socket_id, java_result);
        }
        break;
        
    case SOCKET_ACCEPT_EVENT_IND:
        socket_id = ((SOCKET_ACCEPT_EVENT_IND_SIG_T*)pSignal)->socket_id;      
        socket_type = java_GetSocketTypeBySocket(socket_id);
        javacall_printf(LOG_STR(" socket_id = %d, type = %d, error = %d"),socket_id ,socket_type, java_result);
        if(SOCK_STREAM  == socket_type)
        {                         
            javanotify_socket_event(JAVACALL_EVENT_SOCKET_OPEN_COMPLETED, (javacall_handle)socket_id,  JAVACALL_OK);
        }
        break;
        
    case SOCKET_FULL_CLOSED_EVENT_IND:
        socket_id = ((SOCKET_FULL_CLOSED_EVENT_IND_SIG_T*)pSignal)->socket_id;   
        socket_type = java_GetSocketTypeBySocket(socket_id);
        javacall_printf(LOG_STR(" socket_id = %d, type = %d, error = %d"),socket_id ,socket_type, java_result);
        if(SOCK_STREAM  == socket_type)
        {                         
            javanotify_socket_event(JAVACALL_EVENT_SOCKET_CLOSE_COMPLETED, (javacall_handle)socket_id,  JAVACALL_OK);
        }
        
        break;
    case SOCKET_ASYNC_GETHOSTBYNAME_CNF:   
        socket_id = ((ASYNC_GETHOSTBYNAME_CNF_SIG_T*)pSignal)->request_id;              
        javacall_printf(LOG_STR("request_id=%d, error = %d"),socket_id, ((ASYNC_GETHOSTBYNAME_CNF_SIG_T*)pSignal)->error_code);
        if(0 == ((ASYNC_GETHOSTBYNAME_CNF_SIG_T*)pSignal)->error_code)
        {
            java_result = JAVACALL_OK;            
            if(PNULL != pcHostAddress) 
            {      
                const int nLen = 4;	              
                SCI_MEMCPY(pcHostAddress ,((ASYNC_GETHOSTBYNAME_CNF_SIG_T*)pSignal)->hostent.h_addr, nLen);
            }
        }
        else
        {
            java_result = JAVACALL_FAIL;            
            //javanotify_network_event(JAVACALL_EVENT_NETWORK_GETHOSTBYNAME_COMPLETED);   /*lint !e64*/
        }
        javanotify_socket_event(JAVACALL_EVENT_NETWORK_GETHOSTBYNAME_COMPLETED, (javacall_handle)socket_id, java_result);
        break;
        
    default:
        break;           
    }    
    return TRUE;
    }
    
