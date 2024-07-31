/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_ia_java_trc.h"
#include "std_header.h"
#include "jblend_config.h"
#ifdef JAVA_SUPPORT_IA
#ifdef JAVA_SUPPORT_SHRINK_RAM
#include "kjava_shrink_ram.h"
#endif
#include "mmijava_internal.h"

#include "kjava_sys_sock.h"
#include "kjava_sys_sock_action.h"

#include "app_tcp_if.h" //socket structure
#ifndef WIN32
#ifdef PCLINK_SUPPORT
#include "tcpip_api.h"
#endif
#endif
#if defined BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmipdp_export.h"      
#include "mmiconnection_export.h"

#include "jblend_platform.h"
#include "ajms_cfg.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#ifdef WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define KJAVA_MAX_SESSIONS     10
#define KJAVA_MAX_HOSTNAME_LENGTH   128
#define KJAVA_SOCKET_INVALD_ID			-1
#define KJAVA_SOCKET_LINK_POLLING_TIME_PEROID   	    100
#define KJAVA_SOCKET_CONN_POLLING_TIME_PEROID   	    200
#ifdef JAVA_LOW_POWER_ENABLE 
#define KJAVA_SOCKET_SEND_POLLING_TIME_PEROID   	    2000
#define KJAVA_SOCKET_RECV_POLLING_TIME_PEROID   	    2000
#else
#define KJAVA_SOCKET_SEND_POLLING_TIME_PEROID   	    200
#define KJAVA_SOCKET_RECV_POLLING_TIME_PEROID   	    200
#endif
#define KJAVA_SOCKET_CLOS_POLLING_TIME_PEROID   	    100
#define KJAVA_SOCKET_LINK_POLLING_TIME_COUNT   	    100
#define KJAVA_SOCKET_CONN_POLLING_TIME_COUNT   	    200
#define KJAVA_SOCKET_SEND_POLLING_TIME_COUNT   	    200

#define KJAVA_SOCKET_RECV_POLLING_TIME_COUNT   	    10000
#define KJAVA_SOCKET_CLOS_POLLING_TIME_COUNT   	    100
#define JAVA_MAX_URL_LEN   MMIENG_MAX_URL_LEN
#define JAVA_MAX_PROXY_URL_LEN   KJAVA_MAX_HOSTNAME_LENGTH

/**-------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct _KJAVA_TCP_ConnectionTag
{
    int32                       used;
    int32                       socketID;
    int32                       secure;
    char                       	hostname[KJAVA_MAX_HOSTNAME_LENGTH];
    uint32               		ipaddr;
    uint32              		port;
	int32 						state;	
	struct sci_sockaddr	      	sockAddr;
	KJavaLocalInfo *			localInfo;
	uint8          *			recv_buf;
	int32						recv_bufsize;
	int32 						timer_link_cnt;
	int32 						timer_conn_cnt;
	int32 						timer_recv_cnt;
	int32 						timer_send_cnt;	
	int32 						timer_link_id;
	int32 						timer_conn_id;
	int32 						timer_recv_id;
	int32 						timer_send_id;
    uint8*                      send_buf;
    int32                       send_buflen;
    int32                       send_total_size;
	KJavaNetworkCallbackProc  	eventHandler;
}KJAVA_TCP_Connection;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
static int32					linger_value = 0;
static int32					delay_value = 0;
#ifndef JAVA_SUPPORT_SHRINK_RAM
static KJAVA_TCP_Connection socketConn[KJAVA_MAX_SESSIONS] = {0};
static	AJ_U8                   Proxy_addr[JAVA_MAX_PROXY_URL_LEN + 1];
static	AJ_U8                   java_ua[JAVA_MAX_URL_LEN + 1];
#else
DEFINE_STATIC_SHRINK_VAR_1_DIM(KJAVA_TCP_Connection,socketConn)
DEFINE_STATIC_SHRINK_VAR_1_DIM(AJ_U8,Proxy_addr)
DEFINE_STATIC_SHRINK_VAR_1_DIM(AJ_U8,java_ua)
#endif
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
#ifdef NCI_TCK
extern int32 isWmaOpen;
extern int g_runningTckLevel;
#endif

extern BOOLEAN g_jvm_is_auto_tck_mode;

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
uint8 g_java_net_id=0; //gprs socket id 

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
//******************************************************************************
//  Description:  kjava_socket_find_free_slot
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_socket_find_free_slot(void);

//******************************************************************************
//  Description:  kjava_sock_link_polling
//  Author:huangjc
//  Date:2007-09-03
//  Note: 1 linked/ 0 not linked
//*********************************************************************************
LOCAL int32 kjava_sock_link_polling(int32 slot);
//******************************************************************************
//  Description:  kjava_sock_link_finish
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_sock_link_finish(int32 slot);
//******************************************************************************
//  Description:  kjava_sock_conn_start
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_sock_conn_start(int32 slot);
//******************************************************************************
//  Description:  kjava_sock_conn_polling
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_sock_conn_polling(int32 slot);
//******************************************************************************
//  Description:  kjava_sock_conn_finish
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_sock_conn_finish(int32 slot);
//******************************************************************************
//  Description:  kjava_sock_recv_start
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_sock_recv_start(int32 slot);
//******************************************************************************
//  Description:  kjava_sock_recv_polling
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_sock_recv_polling(int32 slot);
//******************************************************************************
//  Description:  kjava_sock_recv_finish
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_sock_recv_finish(int32 slot);
//******************************************************************************
//  Description:  kjava_sock_send_polling
//  Author:hanker.zhou
//  Date:2010-09-09
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_sock_send_polling(int32 slot);
//******************************************************************************
//  Description:  kjava_sock_recv_finish
//  Author:hanker.zhou
//  Date:2010-09-09
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_sock_send_finish(int32 slot);

/***************************************************************************/
// Description:socketEventCallbackProc
// Date:2007-06-02
// Author:huangjc
// Note:J
//*************************************************************************/
LOCAL void socketEventCallbackProc(int32 connID, int32 type, int32 result);

/***************************************************************************/
// Description:kjava_socket_Jblendia_WapSetting
// Date:2010-03-08
// Author:
// Note:add gateway,port,ua setting when enter java application
//*************************************************************************/
LOCAL void  kjava_socket_Jblendia_WapSetting(void);


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifdef JAVA_SUPPORT_SHRINK_RAM
BEGIN_DEFINE_SHRINK_VAR_INIT(kjava_sys_sock)
	SHRINK_VAR_MEMORY_1_DIM(KJAVA_TCP_Connection, socketConn, KJAVA_MAX_SESSIONS)
	SHRINK_VAR_MEMORY_1_DIM(AJ_U8,Proxy_addr,JAVA_MAX_PROXY_URL_LEN + 1)
	SHRINK_VAR_MEMORY_1_DIM(AJ_U8,java_ua,JAVA_MAX_URL_LEN + 1)
END_DEFINE_SHRINK_VAR_INIT

BEGIN_DEFINE_SHRINK_VAR_FINI(kjava_sys_sock)
	FREE_SHRINK_VAR_MEMORY(socketConn)
	FREE_SHRINK_VAR_MEMORY(Proxy_addr)
	FREE_SHRINK_VAR_MEMORY(java_ua)
END_DEFINE_SHRINK_VAR_FINI
#endif
//******************************************************************************
//  Description:Gets the number of maximum concurrent connections.
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//*********************************************************************************
int kjava_sock_getMaxSessions(void)
{
    JWE_LOG(kjava_sock_getMaxSessions, ("Max socket sessions: %d", KJAVA_MAX_SESSIONS))
    return KJAVA_MAX_SESSIONS;
}

//******************************************************************************
//  Description:Opens a new connection handle
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//*********************************************************************************

#ifdef FS_PROFILE		
extern int g_vm_exit;
#endif
int kjava_sock_open(const unsigned char *addr, int port, int secure, int *connectionID_p,
                    KJavaNetworkCallbackProc callback)
{
	int32 slot = 0;

   	JWE_LOG(kjava_sock_open, ("addr:%s, port:%d", addr, port))
	slot = kjava_socket_find_free_slot();
#ifdef FS_PROFILE		
     if(g_vm_exit <2)
    {
	    kjava_profiler_startCount(3);
    }	
#endif

	if(-1 == slot)
    {
		JWE_LOG(kjava_sock_open,("ERROR! no free slot "))
		return -1;
	}

    strncpy(socketConn[slot].hostname, (char*)addr, KJAVA_MAX_HOSTNAME_LENGTH);
	socketConn[slot].used 	    = 1;
	socketConn[slot].port 	    = port;
	socketConn[slot].secure 		= secure;
	socketConn[slot].socketID		= KJAVA_SOCKET_INVALD_ID;
	socketConn[slot].state	    = KJAVA_SOCKET_ACTION_OPEN;	
    socketConn[slot].send_buflen  = -1;  
    socketConn[slot].send_buf     = NULL;
    socketConn[slot].send_total_size = -1;

	socketConn[slot].eventHandler	= callback;
    
    *connectionID_p 			    = slot;

#ifdef NCI_TCK
	if(isWmaOpen && g_runningTckLevel)
    {
	    JWE_LOG(kjava_sock_open, ("sleep 1 minutes for nci test"))
    	SCI_Sleep(1000);
	}
#endif

    return 0;
}


//******************************************************************************
//  Description:Establish a connection using a specified connection handle. 
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//*********************************************************************************
int kjava_sock_connect(int connectionID, KJavaLocalInfo *localInfo_p)
{
	int32 nResult = 0;
	int32 slot = 0;
    MMIPDP_CNF_INFO_T pdp_cnf_info;
    MMIJAVA_NETWORK_TYPE_E  net_type = MMIJAVA_GetNetworkType();
    JWE_LOG(kjava_sock_connect, ("connectionID =%d", connectionID))
	slot = connectionID;

	// check the sim card state
	if(net_type < JAVA_NETWORK_TYPE_WIFI)
	{
    	if (!MMIAPIPHONE_IsSimAvailable((MN_DUAL_SYS_E)(net_type)))
    	{
    		JWE_LOG(kjava_sock_connect, ("sim card is not ready"));
            pdp_cnf_info.msg_id = MMIPDP_ACTIVE_CNF;
            pdp_cnf_info.result = MMIPDP_RESULT_FAIL;
            MMIJAVAHandlePDPMsg(&pdp_cnf_info);
    		return -1;
    	}
	}
#ifdef WIFI_SUPPORT
	else if(net_type == JAVA_NETWORK_TYPE_WIFI)
	{	    
        if(MMIWIFI_STATUS_CONNECTED != MMIAPIWIFI_GetStatus())
        {
           	JWE_LOG(kjava_sock_connect, ("wifi is not connected"));
            pdp_cnf_info.msg_id = MMIPDP_ACTIVE_CNF;
            pdp_cnf_info.result = MMIPDP_RESULT_FAIL;
            MMIJAVAHandlePDPMsg(&pdp_cnf_info);
    		return -1; 
        }
	}
#endif	
	// set the parameter
	socketConn[slot].state = KJAVA_SOCKET_ACTION_CONNECT;
	socketConn[slot].localInfo = localInfo_p;

	// check if gprs linked
	if(net_type < JAVA_NETWORK_TYPE_WIFI)
    {
#ifdef PCLINK_SUPPORT
        #ifndef WIN32
        //lint -esym(628, TCPIPPCLINK_IsPclinkEnable)
        if(TCPIPPCLINK_IsPclinkEnable())/*lint !e718  !e746  !e628*/
        {
            //lint -esym(628, TCPIPPCLINK_SetNetInterface)
            if (TCPIPPCLINK_SetNetInterface() == TRUE)/*lint !e718  !e746  !e628 !e18*/
            {

            }     
            else
            {
                //SCI_TRACE_LOW:"kjava_sock_connect pclink set interface is wrong"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_SOCK_330_112_2_18_2_23_26_399,(uint8*)"");
                return -1;
            }
        }    
        else if(MMIPDP_LINK_STATE_ACTIVED!= MMIAPIPDP_GetLinkState(MMI_MODULE_JAVA))
        #endif
#else
        if(MMIPDP_LINK_STATE_ACTIVED!= MMIAPIPDP_GetLinkState(MMI_MODULE_JAVA))
#endif
        {
            JWE_LOG(kjava_sock_connect, ("send MSG_JAVA_LINK_TIMER"));
            MMIAPIJAVA_SendSignal(MSG_JAVA_LINK_TIMER,slot);
            return 0;
        }
    }
	
	// do the socket connect
	nResult = kjava_sock_conn_start(slot);
	JWE_LOG(kjava_sock_connect, ("result = %d", nResult));
	return 0;
}
  

//******************************************************************************
//  Description:  Sends data over a connection.  
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//*********************************************************************************
int kjava_sock_send(int connectionID, const unsigned char *data, int datalen)
{
    int32 slot = 0;
    int32 result = 0;
    int32 e = 0;

    JWE_LOG(kjava_sock_send,("connectionID=%d, data=%08x, datalen=%d", 
	connectionID, data, datalen))
   
    slot = connectionID;
    JWE_LOG(kjava_sock_send,("socket=%d", socketConn[slot].socketID))

	if( socketConn[slot].socketID == KJAVA_SOCKET_INVALD_ID)
    {    
		return -1;
    }
    
    if ( socketConn[slot].state == KJAVA_SOCKET_ACTION_RECEIVE)
    {
        socketConn[slot].state = KJAVA_SOCKET_ACTION_SEN_RCV;
    }
    else
    {
        socketConn[slot].state = KJAVA_SOCKET_ACTION_SEND;
    }

	//socketConn[slot].state = KJAVA_SOCKET_ACTION_SEND;
    socketConn[slot].send_total_size = datalen;
    result = sci_sock_send(socketConn[slot].socketID, (char *)data, datalen, 0);
    JWE_LOG(kjava_sock_send,("sci_sock_send return result(%d)",result))

	if(result < 0)
    {
		JWE_LOG(kjava_sock_send,("[ERROR] sci_sock_send return unaccept result (%d)",result))
		e = sci_sock_errno(socketConn[slot].socketID);
		if ( EWOULDBLOCK == e)
		{
            JWE_LOG(kjava_sock_send, ("do send later"));
            MMIAPIJAVA_SendSignal(MSG_JAVA_SEND_TIMER,slot);
            socketConn[slot].send_buflen = datalen;
            socketConn[slot].send_buf =( uint8 * ) data;  
            return 0;
		}
		else
		{
    	    JWE_LOG(kjava_sock_send,("errno=%d",e))		
            socketEventCallbackProc(slot,KJAVA_SOCKET_EVENT_TYPE_SEND,-1);
            return -1;
		}
    }
    else
    {
        JWE_LOG(kjava_sock_send,("[OK] send ,slot (%d) datalen (%d),datalen =%d", slot, result,datalen))

    	if(result < datalen)
        {
             MMIAPIJAVA_SendSignal(MSG_JAVA_SEND_TIMER,slot);
            socketConn[slot].send_buflen = datalen - result;
            socketConn[slot].send_buf =( uint8 * ) (data + result) ;  
            JWE_LOG(kjava_sock_send,("send left,slot (%d) datalen (%d)", slot, socketConn[slot].send_buflen))
            return 0;
        }
        else
        {
            if ( socketConn[slot].state == KJAVA_SOCKET_ACTION_SEN_RCV )
            {
                socketConn[slot].state = KJAVA_SOCKET_ACTION_RECEIVE;
            }
            else
            {
                socketConn[slot].state = KJAVA_SOCKET_ACTION_NULL;
            }  				

            //socketConn[slot].state = KJAVA_SOCKET_ACTION_NULL;
            socketEventCallbackProc(slot,KJAVA_SOCKET_EVENT_TYPE_SEND, socketConn[slot].send_total_size);
        }
    }

#ifdef NCI_TCK
	if(isWmaOpen && g_runningTckLevel)
    {
    	JWE_LOG(kjava_sock_send, ("sleep 1 minutes for nci test"))
    	SCI_Sleep(1000);
	}
#endif

    return 0; 	
}

//******************************************************************************
//  Description:  MMIAPIJAVA_StartSendTimer
//  Author:murphyxie
//  Date:2010-09-13
//  Note: start send polling
//*********************************************************************************
PUBLIC void MMIAPIJAVA_StartSendTimer(int32 slot)
{
    JWE_LOG(MMIAPIJAVA_StartSendTimer, ("slot=%d", slot));
    
    if(0 != socketConn[slot].timer_send_id )
    {
        MMK_StopTimer(socketConn[slot].timer_send_id );
    }
    
    socketConn[slot].timer_send_cnt = KJAVA_SOCKET_SEND_POLLING_TIME_COUNT;
    socketConn[slot].timer_send_id = MMK_CreateTimerCallback( KJAVA_SOCKET_SEND_POLLING_TIME_PEROID, kjava_socket_polling_timeOut_process,NULL,SCI_TRUE);
    MMK_StartTimerCallback(socketConn[slot].timer_send_id, KJAVA_SOCKET_SEND_POLLING_TIME_PEROID, kjava_socket_polling_timeOut_process,NULL,SCI_TRUE);
}

//******************************************************************************
//  Description:  kjava_sock_send_polling
//  Author:murphyxie
//  Date:2010-09-13
//  Note: send date to dest url timer out process
//*********************************************************************************
LOCAL int32 kjava_sock_send_polling(int32 slot)
{
    int32 e = 0,result = -1;
    
	JWE_LOG(kjava_sock_send_polling, ("slot=%d,count =%d", slot,socketConn[slot].timer_send_cnt));
    
	if(socketConn[slot].timer_send_cnt <= 0)    
	{	
        JWE_LOG(kjava_sock_send_polling,("timeout"))
        kjava_sock_send_finish(slot);
        socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_SEND,-1);
		return -1;
	}	
    
    socketConn[slot].timer_send_cnt--;
    result = sci_sock_send(socketConn[slot].socketID, (char *)socketConn[slot].send_buf, socketConn[slot].send_buflen , 0);

	if( result > 0) // send ok
	{
        kjava_sock_send_finish(slot);
        JWE_LOG(kjava_sock_send_polling,("[OK] send ,slot (%d) datalen (%d)", slot, result))
	    if(result <  socketConn[slot].send_buflen)
        {
            MMIAPIJAVA_SendSignal(MSG_JAVA_SEND_TIMER,slot);
            socketConn[slot].send_buflen = socketConn[slot].send_buflen - result;
            socketConn[slot].send_buf =( uint8 * ) (socketConn[slot].send_buf + result) ;  
            JWE_LOG(kjava_sock_send_polling,("send again ,slot (%d) datalen (%d)", slot, socketConn[slot].send_buflen))
            return 0;
        }
        else
        {
            if ( socketConn[slot].state == KJAVA_SOCKET_ACTION_SEN_RCV )
            {
                socketConn[slot].state = KJAVA_SOCKET_ACTION_RECEIVE;
            }
            else
            {
                socketConn[slot].state = KJAVA_SOCKET_ACTION_NULL;
            }  				

            //socketConn[slot].state = KJAVA_SOCKET_ACTION_NULL;
            socketEventCallbackProc(slot,KJAVA_SOCKET_EVENT_TYPE_SEND, socketConn[slot].send_total_size);		
        }
	}
    else if(result == 0)	// socket closed by other side
	{
		JWE_LOG(kjava_sock_send_polling, ("receive finished"));
		kjava_sock_send_finish(slot);
		socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_SEND, 0);
	}
	else
	{
		JWE_LOG(kjava_sock_send_polling,("[ERROR] sci_sock_send return unaccept result (%d)",result))
		e = sci_sock_errno(socketConn[slot].socketID);
		if ( EWOULDBLOCK == e)
		{
			JWE_LOG(kjava_sock_send_polling, ("do send later"));
	        return 0;
		}
		else
		{
            JWE_LOG(kjava_sock_send_polling,("errno=%d",e))
            kjava_sock_send_finish(slot);
            socketEventCallbackProc(slot,KJAVA_SOCKET_EVENT_TYPE_SEND,-1);
            return -1;
		}
     }
	
	return 0;
}


//******************************************************************************
//  Description:  kjava_sock_send_finish
//  Author:murphyxie
//  Date:2010-09-13
//  Note: send date to dest url
//*********************************************************************************
LOCAL int32 kjava_sock_send_finish(int32 slot)
{
	JWE_LOG(kjava_sock_send_finish, ("slot=%d", slot));
	if(socketConn[slot].timer_send_id)
	{
		MMK_StopTimer(socketConn[slot].timer_send_id);
		socketConn[slot].timer_send_id = 0;
	}
	return 0;
}

//******************************************************************************
//  Description:  Receives data from a connection
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//*********************************************************************************
int kjava_sock_receive(int connectionID, unsigned char *data, int datalen)
{
	int32 slot = 0;
   	JWE_LOG(kjava_sock_receive,("connectionID=%d, data=%08x, datalen=%d", 
		connectionID, data, datalen));

	slot = connectionID;
    
    if ( socketConn[slot].state == KJAVA_SOCKET_ACTION_SEND)
    {
        socketConn[slot].state = KJAVA_SOCKET_ACTION_SEN_RCV;
    }
    else
    {
        socketConn[slot].state = KJAVA_SOCKET_ACTION_RECEIVE;
    }

	socketConn[slot].recv_buf 		= data;
	socketConn[slot].recv_bufsize	= datalen;
	
	return kjava_sock_recv_start(slot);		
}

//******************************************************************************
//  Description:  Gets the size of available data in the buffer of the protocol stack. 
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//*********************************************************************************
int kjava_sock_available(int connectionID)
{
	int32 aval_size = 0;
    int32 slot = 0;

   	JWE_LOG(kjava_sock_available,("connectionID=%d", connectionID))
	slot = connectionID;

	if( socketConn[slot].socketID == KJAVA_SOCKET_INVALD_ID)
    {    
		return -1;
    }

	if(sci_sock_getsockopt(socketConn[slot].socketID, SO_RXDATA,&aval_size) == 0)
	{
		JWE_LOG(kjava_sock_available,("available RXDATA size is %d",aval_size));
	}
	else
	{
		JWE_LOG(kjava_sock_available,("get avail data size failed"));
	}

	return aval_size;
}

//******************************************************************************
//  Description:  Shuts down a connection. 
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//*********************************************************************************
int kjava_sock_shutdown(int connectionID, int method)
{
    /* non-blocking operation */ 
	int32 slot = 0;
    int32 how = 0;
    
    slot = connectionID;

    JWE_LOG(kjava_sock_shutdown,("slot (%d) handle(%d) method (%d)",
	                    slot, socketConn[slot].socketID, method))
    if ( socketConn[slot].state == KJAVA_SOCKET_ACTION_RECEIVE )
    {
        socketEventCallbackProc( slot, KJAVA_SOCKET_EVENT_TYPE_RECEIVE, 0 );
    }
    else if ( socketConn[slot].state == KJAVA_SOCKET_ACTION_SEND )
    {
        socketEventCallbackProc( slot, KJAVA_SOCKET_EVENT_TYPE_SEND, 0 );
    }
	else if(socketConn[slot].state == KJAVA_SOCKET_ACTION_SEN_RCV)
	{
        JWE_LOG(kjava_sock_shutdown,("RECEIVE and SEND state "))	
        socketEventCallbackProc( slot, KJAVA_SOCKET_EVENT_TYPE_RECEIVE, KJAVA_SOCKET_ERROR_OTHER );
        socketEventCallbackProc( slot, KJAVA_SOCKET_EVENT_TYPE_SEND, KJAVA_SOCKET_ERROR_OTHER );

	}
    socketConn[slot].state = KJAVA_SOCKET_ACTION_SHUTDOWN;
    if(KJAVA_SOCKET_SHUTDOWN_RECEIVE == method)
    {        
        how = 0;
    }
	else if (KJAVA_SOCKET_SHUTDOWN_SEND == method)
    {        
        how = 1;
    }
	else
    {        
        how = 2;
    }

    if( socketConn[slot].socketID != KJAVA_SOCKET_INVALD_ID)
	{
	    sci_sock_shutdown(socketConn[slot].socketID, how);
	}

	socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_SHUTDOWN,KJAVA_SOCKET_SUCCESS);
    return 0;	
}

//******************************************************************************
//  Description:  MMIAPIJAVA_StopSockTimer
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
PUBLIC void MMIAPIJAVA_StopSockTimer(int32 slot)
{
    kjava_sock_link_finish(slot);
	kjava_sock_conn_finish(slot);
	kjava_sock_recv_finish(slot);
    kjava_sock_send_finish(slot);

}
//******************************************************************************
//  Description:  Closes the connection.
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//*********************************************************************************
int kjava_sock_close(int connectionID)
{
    int32 slot = 0;
    int32 result = 0;

    JWE_LOG(kjava_sock_close,("connectionID=%d, sockId=%d",connectionID, socketConn[connectionID].socketID))

	slot = connectionID;
    socketConn[slot].state = KJAVA_SOCKET_ACTION_CLOSE;
#ifdef FS_PROFILE		
     if(g_vm_exit <2)
    {
	    kjava_profiler_endCount(3);
    }	
#endif

	// stop link/conn/recv timer  if necessary
	MMIAPIJAVA_SendSignal(MSG_JAVA_STOP_SOCKET_TIMER,slot);

	if(socketConn[slot].socketID != KJAVA_SOCKET_INVALD_ID)
	{
	    result = sci_sock_socketclose(socketConn[slot].socketID);
	}
	else
	{
		result = 0;
	}
    JWE_LOG(kjava_sock_close,("sci_sock_socketclose return result(%d)",result))
    socketConn[slot].state = KJAVA_SOCKET_ACTION_CLOSE;

    if(0 == result)
    {
	    JWE_LOG(kjava_sock_close,("[OK] close ,slot (%d)",slot))

        socketConn[slot].used = 0;    
	    socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_CLOSE, KJAVA_SOCKET_SUCCESS);
    }
	else
    {
       	 JWE_LOG(kjava_sock_close,("[ERROR] sci_sock_socketclose return unaccept result (%d)",result))

       	 socketConn[slot].used = 0;                
	     socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_CLOSE, 0);
	}	

#ifdef NCI_TCK
	if(isWmaOpen && g_runningTckLevel)
    {
    	JWE_LOG(kjava_sock_close, ("sleep 5 minutes for nci test"))
    	SCI_Sleep(5000);
	}
#endif
    return 0;
}

//******************************************************************************
//  Description:  Sets a socket option of the connection
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//*********************************************************************************
int kjava_sock_setoption(int connectionID, int option, int value)
{     
	int32 nResult = 0;
	long sockID = 0;
    
    JWE_LOG(kjava_sock_setoption, ("connectionID=%d, option=%d, value=%d", connectionID, option, value))

	sockID = socketConn[connectionID].socketID;

	if( sockID  == KJAVA_SOCKET_INVALD_ID)
    {    
		return -1;
    }

	switch(option)
	{
	case KJAVA_SOCKET_OPTION_DELAY:	        
		delay_value = value;
        nResult = 0;
        break;

	case KJAVA_SOCKET_OPTION_KEEPALIVE:
		nResult = sci_sock_setsockopt(sockID, SO_KEEPALIVE, (char *)&value);
        break;

    case KJAVA_SOCKET_OPTION_LINGER:     
        linger_value =value;
        nResult = 0;
        break;

    case KJAVA_SOCKET_OPTION_RCVBUF:
        nResult = sci_sock_setsockopt(sockID, SO_RCVBUF, (char *)&value);
        break;

    case KJAVA_SOCKET_OPTION_SNDBUF:
        nResult = sci_sock_setsockopt(sockID, SO_SNDBUF, (char *)&value);
        break;

	default:
       break;
	}    

    JWE_LOG(kjava_sock_setoption, ("nResult=%d", nResult))		
	return nResult;
}

//******************************************************************************
//  Description:  Gets the socket option of a connection
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//*********************************************************************************
int kjava_sock_getoption(int connectionID, int option)
{
	int32 nResult = 0;
	long sockID = 0;
    
    JWE_LOG(kjava_sock_getOption, ("connectionID=%d, option=%d", connectionID, option))

	sockID = socketConn[connectionID].socketID;
	if( sockID  == KJAVA_SOCKET_INVALD_ID)
    {   
		return -1;
    }


	switch(option)
	{
	case KJAVA_SOCKET_OPTION_DELAY:
		nResult = delay_value;
		break;

	case KJAVA_SOCKET_OPTION_KEEPALIVE:
		sci_sock_getsockopt(sockID, SO_KEEPALIVE, (char *)&nResult);
		break;

	case KJAVA_SOCKET_OPTION_LINGER:                  
		nResult = linger_value;
		break;

	case KJAVA_SOCKET_OPTION_RCVBUF:
		sci_sock_getsockopt(sockID, SO_RCVBUF, (char *)&nResult );
		break;

	case KJAVA_SOCKET_OPTION_SNDBUF:
		sci_sock_getsockopt(sockID, SO_SNDBUF, (char *)&nResult);
		break;
    default:
        break;
	}
	
    	
    JWE_LOG(kjava_sock_getoption, ("nResult=%d", nResult))			
	return nResult;
}

//******************************************************************************
//  Description:  kjava_sock_link_start
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//*********************************************************************************
PUBLIC void MMIAPIJAVA_StartLinkTimer(int32 slot)
{
    JWE_LOG(MMIAPIJAVA_StartLinkTimer, ("slot=%d", slot));

	if(MMIJAVA_PDPActive())
	{
	    if(0 !=socketConn[slot].timer_link_id)
        {
        	MMK_StopTimer(socketConn[slot].timer_link_id);
        }
        
        socketConn[slot].timer_link_cnt = KJAVA_SOCKET_LINK_POLLING_TIME_COUNT;
        socketConn[slot].timer_link_id = MMK_CreateTimerCallback( KJAVA_SOCKET_LINK_POLLING_TIME_PEROID, kjava_socket_polling_timeOut_process, NULL, SCI_TRUE);
        MMK_StartTimerCallback(socketConn[slot].timer_link_id, KJAVA_SOCKET_LINK_POLLING_TIME_PEROID, kjava_socket_polling_timeOut_process, NULL, SCI_TRUE);
        JWE_LOG(MMIAPIJAVA_StartLinkTimer,("ok"))
	}
	else
	{
	    JWE_LOG(MMIAPIJAVA_StartLinkTimer,("fail"))
	}
    
}

//******************************************************************************
//  Description:  kjava_sock_link_polling
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
//extern MMICMSWAP_GPRS_STATE_E MMIBRW_GetGprsState(void);
LOCAL int32 kjava_sock_link_polling(int32 slot)
{
	// int32 ret = 0;
       
    JWE_LOG(kjava_sock_link_polling,("slot:%d,polling count =%d", slot,socketConn[slot].timer_link_cnt))
	   	
	if(socketConn[slot].timer_link_cnt <= 0)    
	{
	    JWE_LOG(kjava_sock_link_polling,("timeout"))
	   	kjava_sock_link_finish(slot);
		socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_CONNECT, KJAVA_SOCKET_ERROR_TIMEOUT);
		return -1;
	}	

	socketConn[slot].timer_link_cnt--;	

	if( MMIPDP_LINK_STATE_ACTIVED == MMIAPIPDP_GetLinkState(MMI_MODULE_JAVA))
	{
		kjava_sock_link_finish(slot);
		kjava_sock_conn_start(slot);
	}
	else 
	{
		JWE_LOG(kjava_sock_link_polling,("pdp active timeout"))
		return -1;
	}
	return 0;
}

//******************************************************************************
//  Description:  kjava_sock_link_finish
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_sock_link_finish(int32 slot)
{
       
    JWE_LOG(kjava_sock_link_finish,("slot:%d", slot))
	if(socketConn[slot].timer_link_id)
	{
		MMK_StopTimer(socketConn[slot].timer_link_id);
		socketConn[slot].timer_link_id = 0;
	}
	return 0;
}

//******************************************************************************
//  Description:  kjava_sock_conn_start
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_sock_conn_start(int32 slot)
{
	unsigned long address = 0;
	struct sci_hostent *hostent = PNULL;
	int32 sockID = 0;
	int32 nResult = 0, e = 0;
	TCPIP_NETID_T   java_net_id = g_java_net_id;
    #ifdef WIFI_SUPPORT
	if(MMIJAVA_GetNetworkType() == JAVA_NETWORK_TYPE_WIFI)
		java_net_id = MMIAPIWIFI_GetNetId();
    #endif
      
    JWE_LOG(kjava_sock_conn_start,("slot=%d", slot));
	sci_parse_host_ext(socketConn[slot].hostname, &address, 0,java_net_id);
       
    JWE_LOG(kjava_sock_conn_start, ("hostname=%s, address=%08x", 
	   	socketConn[slot].hostname, address));

       
    hostent = sci_gethostbyname_ext(socketConn[slot].hostname,java_net_id);
	if (hostent == NULL)
	{
		nResult = KJAVA_SOCKET_ERROR_OTHER;
		JWE_LOG(kjava_sock_conn_start,("sci_gethostbyname is error!"));
		socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_CONNECT, nResult);
		return nResult;
	}
	socketConn[slot].ipaddr = ntohl(*((unsigned long*)*hostent->h_addr_list));
	JWE_LOG(kjava_sock_conn_start,("socketConn[slot].ipaddr is 0x%08x, %s", 
		socketConn[slot].ipaddr, inet_ntoa(socketConn[slot].ipaddr)));

	//sockAddr第一次赋值
   socketConn[slot].sockAddr.family	= AF_INET;
   socketConn[slot].sockAddr.ip_addr	= htonl(socketConn[slot].ipaddr);
   socketConn[slot].sockAddr.port	= htons(socketConn[slot].port);

	/* allocate and get the native socket id */
    sockID = sci_sock_socket(AF_INET, SOCK_STREAM, 0,java_net_id);//本地API函数获得真正的socket 句柄
	if (sockID == -1)
	{
		JWE_LOG(kjava_sock_conn_start, ("ERROR! sockID= INVALID_SOCKET"));
		nResult = KJAVA_SOCKET_ERROR_OTHER;
		socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_CONNECT, nResult);
		return nResult;
	}
	else
	{
		socketConn[slot].socketID = sockID;
		JWE_LOG(kjava_sock_conn_start,("sockID is %08x",sockID));
	}
    if(sci_sock_setsockopt(sockID,SO_NBIO,NULL) == -1)
	{
		JWE_LOG(kjava_sock_conn_start,("set no block socket error!"));
	}
	else
	{
		JWE_LOG(kjava_sock_conn_start,("set no block socket OK!"));
	}	

    nResult = sci_sock_connect((long)sockID, (struct sci_sockaddr*)(&(socketConn[slot].sockAddr)), sizeof(socketConn[slot].sockAddr));
	JWE_LOG(kjava_sock_conn_start,("sci_sock_connect result=%d", nResult));

	if(nResult < 0)
	{
		e = sci_sock_errno((long)sockID);
		JWE_LOG(kjava_sock_conn_start,("errno =%d", e));
		if( EINPROGRESS == e || 0 == e)	// connect failed, reconnect
		{
            MMIAPIJAVA_SendSignal(MSG_JAVA_CONNECT_TIMER,slot);			
		}
		else
		{
			JWE_LOG(kjava_sock_conn_start,("connect failed, callback to jvm"));
			socketEventCallbackProc(slot,KJAVA_SOCKET_EVENT_TYPE_CONNECT,-1);	
			return nResult;
		}
	}
	else
	{
		JWE_LOG(kjava_sock_conn_start,("connect OK"));
	    sci_sock_getsockname(sockID, (struct sci_sockaddr*)&socketConn[slot].sockAddr);
		SCI_MEMSET(socketConn[slot].localInfo->localAddress, 0, sizeof(socketConn[slot].localInfo->localAddress));
		strcpy(socketConn[slot].localInfo->localAddress, inet_ntoa(socketConn[slot].sockAddr.ip_addr));
		socketConn[slot].localInfo->localPort = ntohs(socketConn[slot].sockAddr.port);
		JWE_LOG(kjava_sock_conn_start,  	
			("addr %s, port=%d", socketConn[slot].localInfo->localAddress, socketConn[slot].localInfo->localPort));
		socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_CONNECT, nResult);
	}	
	
	return nResult;	
}
//******************************************************************************
//  Description:  kjava_sock_conn_polling
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
PUBLIC void MMIAPIJAVA_StartConnectTimer(int32 slot)
{
    JWE_LOG(MMIAPIJAVA_StartConnectTimer, ("slot=%d", slot));

    if(socketConn[slot].timer_conn_id != 0)
    {
    	MMK_StopTimer(socketConn[slot].timer_conn_id);
    }
    
    socketConn[slot].timer_conn_cnt = KJAVA_SOCKET_CONN_POLLING_TIME_COUNT;
    socketConn[slot].timer_conn_id = MMK_CreateTimerCallback(KJAVA_SOCKET_CONN_POLLING_TIME_PEROID, kjava_socket_polling_timeOut_process, NULL,SCI_TRUE);
    MMK_StartTimerCallback(socketConn[slot].timer_conn_id, KJAVA_SOCKET_CONN_POLLING_TIME_PEROID, kjava_socket_polling_timeOut_process, NULL, SCI_TRUE);

}
//******************************************************************************
//  Description:  kjava_sock_conn_polling
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_sock_conn_polling(int32 slot)
{
	int32 nResult = 0, e = 0;
	int32 sockID = 0;
    JWE_LOG(kjava_sock_conn_polling,("slot:%d,polling count =%d", slot,socketConn[slot].timer_conn_cnt))
	if(socketConn[slot].timer_conn_cnt <= 0)    
	{
	       
        JWE_LOG(kjava_sock_conn_polling,("timeout"))
	   	kjava_sock_conn_finish(slot);
		socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_CONNECT,-1);
		return -1;
	}	
	socketConn[slot].timer_conn_cnt--;

	sockID = socketConn[slot].socketID;	
	nResult = sci_sock_connect((long)sockID, (struct sci_sockaddr*)(&(socketConn[slot].sockAddr)), sizeof(socketConn[slot].sockAddr));
	JWE_LOG(kjava_sock_conn_polling,("sci_sock_connect result=%d", nResult));

	if(nResult < 0)
	{
		e = sci_sock_errno((long)sockID);
		JWE_LOG(kjava_sock_conn_polling,("errno =%d", e));
		if( EINPROGRESS == e || 0 == e)	// connect failed, reconnect
		{
			JWE_LOG(kjava_sock_conn_polling,("reconnect later"));
		}
		else
		{
			JWE_LOG(kjava_sock_conn_polling,("connect failed, callback to jvm"));
			kjava_sock_conn_finish(slot);
			socketEventCallbackProc(slot,KJAVA_SOCKET_EVENT_TYPE_CONNECT,nResult);		
		}
	}
	else
	{
		JWE_LOG(kjava_sock_conn_polling,("connect OK"));
	       
        sci_sock_getsockname(sockID, (struct sci_sockaddr*)&socketConn[slot].sockAddr);
		SCI_MEMSET(socketConn[slot].localInfo->localAddress, 0, sizeof(socketConn[slot].localInfo->localAddress));
		strcpy(socketConn[slot].localInfo->localAddress, inet_ntoa(socketConn[slot].sockAddr.ip_addr));
		socketConn[slot].localInfo->localPort = ntohs(socketConn[slot].sockAddr.port);
		JWE_LOG(kjava_sock_conn_polling,  	
			("addr %s, port=%d", socketConn[slot].localInfo->localAddress, socketConn[slot].localInfo->localPort));
		kjava_sock_conn_finish(slot);		
		socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_CONNECT, nResult);
	}
	
	return 0;
}
//******************************************************************************
//  Description:  kjava_sock_conn_finish
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_sock_conn_finish(int32 slot)
{
	JWE_LOG(kjava_sock_conn_finish, ("slot=%d", slot));
	if(socketConn[slot].timer_conn_id)
	{
		MMK_StopTimer(socketConn[slot].timer_conn_id);
		socketConn[slot].timer_conn_id = 0;
	}
	return 0;
}

//******************************************************************************
//  Description:  kjava_sock_recv_start
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_sock_recv_start(int32 slot)
{
	long sockID = 0;
	int32 inbuflen = 0, e = 0;
	JWE_LOG(kjava_sock_recv_start, ("slot=%d", slot));

	sockID = socketConn[slot].socketID;
	inbuflen = sci_sock_recv(sockID, (char*)socketConn[slot].recv_buf, socketConn[slot].recv_bufsize, 0);
	
	if( inbuflen > 0) // recv ok
	{
		JWE_LOG(kjava_sock_recv_start, ("received %d bytes", inbuflen));
	//	print_header((char*)socketConn[slot].recv_buf, inbuflen);
		socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_RECEIVE, inbuflen);
	}
	else if(inbuflen == 0)	// socket closed by other side
	{
		JWE_LOG(kjava_sock_recv_start, ("receive finished"));
		socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_RECEIVE, 0);
	}
	else
	{
		JWE_LOG(kjava_sock_recv_start, ("do not get any data"));
		e = sci_sock_errno(sockID);
		JWE_LOG(kjava_sock_recv_start, ("errno %d", e));
		if(EPIPE == e)	// socket is closed by remote 
		{
			JWE_LOG(kjava_sock_recv_start, ("socket is closed by remote"));
			socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_RECEIVE, -1);
		}
		else if(EWOULDBLOCK == e)	// redo receive later
		{
			JWE_LOG(kjava_sock_recv_start, ("do receive later"));
            MMIAPIJAVA_SendSignal(MSG_JAVA_RECEIVE_TIMER,slot);		
		}
		
	}

	return 0;
}
//******************************************************************************
//  Description:  kjava_sock_recv_polling
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
PUBLIC void MMIAPIJAVA_StartRecvTimer(int32 slot)
{
    JWE_LOG(MMIAPIJAVA_StartRecvTimer, ("slot=%d", slot));
    
    if(0 !=socketConn[slot].timer_recv_id)
    {
        MMK_StopTimer(socketConn[slot].timer_recv_id);
    }

    socketConn[slot].timer_recv_cnt = KJAVA_SOCKET_RECV_POLLING_TIME_COUNT;
    socketConn[slot].timer_recv_id = MMK_CreateTimerCallback(KJAVA_SOCKET_RECV_POLLING_TIME_PEROID, kjava_socket_polling_timeOut_process, NULL, SCI_TRUE);
    MMK_StartTimerCallback(socketConn[slot].timer_recv_id, KJAVA_SOCKET_RECV_POLLING_TIME_PEROID, kjava_socket_polling_timeOut_process, NULL, SCI_TRUE);

}

//******************************************************************************
//  Description:  kjava_sock_recv_polling
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_sock_recv_polling(int32 slot)
{
	long sockID = 0;
	int32 inbuflen = 0, e = 0;

	JWE_LOG(kjava_sock_recv_polling, ("slot=%d,polling count =%d", slot,socketConn[slot].timer_recv_cnt--));

	if(socketConn[slot].timer_recv_cnt <= 0)    
	{
	       
        JWE_LOG(kjava_sock_recv_polling,("timeout"))
	   	kjava_sock_recv_finish(slot);
		socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_RECEIVE,-1);
		return -1;
	}	
	socketConn[slot].timer_recv_cnt--;
	sockID = socketConn[slot].socketID;
	inbuflen = sci_sock_recv(sockID, (char*)socketConn[slot].recv_buf, socketConn[slot].recv_bufsize, 0);
	
	if( inbuflen > 0) // recv ok
	{
		JWE_LOG(kjava_sock_recv_polling, ("received %d bytes", inbuflen));
	//	print_header((char*)socketConn[slot].recv_buf, inbuflen);		
		kjava_sock_recv_finish(slot);
		socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_RECEIVE, inbuflen);
	}
	else if(inbuflen == 0)	// socket closed by other side
	{
		JWE_LOG(kjava_sock_recv_polling, ("receive finished"));
		kjava_sock_recv_finish(slot);
		socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_RECEIVE, 0);
	}
	else
	{
		JWE_LOG(kjava_sock_recv_polling, ("do not get any data"));
		e = sci_sock_errno(sockID);
		JWE_LOG(kjava_sock_recv_polling, ("errno %d", e));
		if(EPIPE == e)	// socket is closed by remote 
		{
			JWE_LOG(kjava_sock_recv_polling, ("socket is closed by remote"));
			kjava_sock_recv_finish(slot);
			socketEventCallbackProc(slot, KJAVA_SOCKET_EVENT_TYPE_RECEIVE, inbuflen);
		}
		else if(EWOULDBLOCK == e)	// redo receive later
		{
			JWE_LOG(kjava_sock_recv_polling, ("do receive later"));
		}		
	}
	
	return 0;
}

//******************************************************************************
//  Description:  kjava_sock_recv_finish
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_sock_recv_finish(int32 slot)
{
	JWE_LOG(kjava_sock_recv_finish, ("slot=%d", slot));
	if(socketConn[slot].timer_recv_id)
	{
		MMK_StopTimer(socketConn[slot].timer_recv_id);
		socketConn[slot].timer_recv_id = 0;
	}
	return 0;
}
//******************************************************************************
//  Description:  kjava_socket_find_free_slot
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 kjava_socket_find_free_slot(void)
{
    int32 i = 0;

    for(i = 0; i < KJAVA_MAX_SESSIONS; i++)
    {
	    if(0 == socketConn[i].used)
        {
	        return i;
        }
    }
    return -1;
}
/***************************************************************************/
// Description:MMIJAVA_NET_init  
// Date:2007-06-02
// Author:huangjc
// Note:
//*************************************************************************/
PUBLIC BOOLEAN MMIJAVA_NET_init(void)
{
	SCI_MEMSET(socketConn, 0, (KJAVA_MAX_SESSIONS * sizeof(KJAVA_TCP_Connection)));
	kjava_socket_Jblendia_WapSetting();
	return TRUE;
}
/***************************************************************************/
// Description:free allocated resources function.
// Date:2007-06-02
// Author:huangjc
// Note:
//*************************************************************************/
PUBLIC void MMIJAVA_NET_fini(void)
{
#ifdef PCLINK_SUPPORT
	#ifndef WIN32
    if(TCPIPPCLINK_IsPclinkEnable())/*lint !e718*/
    {    
        TCPIPPCLINK_ResetNetInterface();/*lint !e718 !e18*/
    }
    else
	#endif
#endif
    {   
         //  MMIAPIJAVA_PDPDeactive();       
         MMIAPIJAVA_SendSignal(MSG_JAVA_DEACTIVE_PDP,PNULL);		
    }
}
/***************************************************************************/
// Description:socketEventCallbackProc
// Date:2007-06-02
// Author:huangjc
// Note:J
//*************************************************************************/
LOCAL void socketEventCallbackProc(int32 connID, int32 type, int32 result)
{    
    
	JWE_LOG(socketEventCallbackProc,
                 ("connID = 0x%04x, type = 0x%x, result = 0x%04x", connID, type, result))
 

	if(socketConn[connID].eventHandler)
    {    
		socketConn[connID].eventHandler(connID,  type,  result);
    }
}
/***************************************************************************/
// Description:kjava_socket_polling_timeOut_process
// Date:2007-06-02
// Author:huangjc
// Note:J
//*************************************************************************/
PUBLIC void kjava_socket_polling_timeOut_process(uint8 timerID, uint32 param)
{
	int32 i = 0;
	JWE_LOG(kjava_socket_polling_timeOut_process, ("timer id =%d", timerID))
		
	for( i = 0; i<KJAVA_MAX_SESSIONS; i++)
	{
		if(socketConn[i].timer_link_id == timerID)
		{
			kjava_sock_link_polling(i);
			return;
		}
		if(socketConn[i].timer_conn_id == timerID)
		{
			kjava_sock_conn_polling(i);
			return;
		}
		else if(socketConn[i].timer_recv_id == timerID)
		{
			kjava_sock_recv_polling(i);
			return ;			
		}
		else if(socketConn[i].timer_send_id == timerID)
		{
			kjava_sock_send_polling(i);
			return ;			
		}
	}
	return;
}

/*****************************************************************************/
//  Description : active pdp for JAVA 
//  Global resource dependence : none
//  Author: Ivy.Su
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_PDPActive(void)
{
    BOOLEAN return_val = FALSE;
    uint8 setting_index = 0;
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;
    MN_DUAL_SYS_E dual_sys = MMIJAVA_GetAcitveSim();
#ifdef MMI_JAVA_NW_V2
	if(!g_jvm_is_auto_tck_mode){
		 setting_index = MMIJAVA_GetNetSettingIndex();
		 JWE_LOG(MMIJAVA_PDPActive, ("!g_jvm_is_auto_tck_mode setting_index = %d", setting_index))
	}
	else
	{
        #ifdef BROWSER_SUPPORT
            setting_index = MMIAPIBROWSER_GetNetSettingIndex(MN_DUAL_SYS_1);		
        #endif
            JWE_LOG(MMIJAVA_PDPActive, ("g_jvm_is_auto_tck_mode setting_index = %d", setting_index))
	}
#else
    setting_index = MMIJAVA_GetNetSettingIndex(); 
#endif
    setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, setting_index);
    JWE_LOG(MMIJAVA_PDPActive, ("setting_item_ptr=%x",setting_item_ptr))
    if(setting_item_ptr)
    {
        app_info.app_handler = MMI_MODULE_JAVA;
        app_info.auth_type = setting_item_ptr->auth_type;
        app_info.apn_ptr = (char*)setting_item_ptr->apn;
        app_info.user_name_ptr = (char*)setting_item_ptr->username;
        app_info.psw_ptr = (char*)setting_item_ptr->password;
        app_info.dual_sys = dual_sys;
        app_info.priority = 3;
        //app_info.ps_service_rat = MN_TD_PREFER;
        app_info.handle_msg_callback = MMIJAVAHandlePDPMsg;
        app_info.ps_service_type = BROWSER_E;
        app_info.storage = MN_GPRS_STORAGE_ALL;

	 JWE_LOG(MMIJAVA_PDPActive, ("dual_sys = %d app_info.apn_ptr=%s", dual_sys,app_info.apn_ptr))
        return_val = MMIAPIPDP_Active(&app_info);
    }

    return return_val;
}

/*****************************************************************************/
//  Description : deactive pdp for JAVA 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_PDPDeactive(void)
{
    BOOLEAN return_val = FALSE;
    JWE_LOG(MMIAPIJAVA_PDPDeactive, ("enter"))
    return_val = MMIAPIPDP_Deactive(MMI_MODULE_JAVA);    
    return return_val;
}

/*****************************************************************************/
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVAHandlePDPMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr)
    {
        return;
    }
    
    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:
	    //SCI_TRACE_LOW:"[MMIJAVA]:MMIJAVAHandlePDPMsg msg_ptr->result=%d"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_SOCK_1444_112_2_18_2_23_29_400,(uint8*)"d",msg_ptr->result);
        if(MMIPDP_RESULT_SUCC == msg_ptr->result)
        {
	        JWE_LOG(MMIJAVAHandlePDPMsg, ("pdp active success!"))
            g_java_net_id = msg_ptr->nsapi; 
        }
        else 
        {
	        JWE_LOG(MMIJAVAHandlePDPMsg, ("pdp active fail!"))
        }
        break;
        
    case MMIPDP_DEACTIVE_CNF:
        break;
        
    case MMIPDP_DEACTIVE_IND:
        //SCI_TRACE_LOW:"MMIJAVAHandlePDPMsg: MMIPDP_DEACTIVE_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_SOCK_1460_112_2_18_2_23_29_401,(uint8*)"");
        break;
        
    default:
        break;
    }
    //SCI_FREE(msg_ptr);
}

/***************************************************************************/
// Description:kjava_socket_Jblendia_WapSetting
// Date:2010-03-08
// Author:
// Note:add gateway,port,ua setting when enter java application
//*************************************************************************/
LOCAL void  kjava_socket_Jblendia_WapSetting(void)
{
    AJMS_ProxyDescriptor                Jblend_Proxy_Dec;
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting_item_ptr = PNULL;
    uint8                               java_setting_index = 0;
    int                                 set_result = -1;
    MMIENG_UA_T                         ua = {0};	
    MMIJAVA_NETWORK_TYPE_E              net_type = MMIJAVA_GetNetworkType();
    uint16                              ua_len = 0;
    uint16                              proxy_len = 0;
    MN_DUAL_SYS_E dual_sys = MMIJAVA_GetAcitveSim();
    
 	SCI_MEMSET(Proxy_addr, 0, sizeof(Proxy_addr));
	SCI_MEMSET(java_ua, 0, sizeof(java_ua));

    if(net_type < JAVA_NETWORK_TYPE_WIFI)
    {
        java_setting_index = MMIJAVA_GetNetSettingIndex();
        linksetting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, java_setting_index);

        JWE_LOG(kjava_socket_Jblendia_WapSetting, ("gateway_len = %d", (linksetting_item_ptr->gateway_len)))

        if((linksetting_item_ptr->gateway_len== 0) || (strcmp((char*)linksetting_item_ptr->gateway, (char*)"0.0.0.0") == 0))
        {
            strcpy((char *)Proxy_addr, (const char *)"0.0.0.0");
            Jblend_Proxy_Dec.type = -1; // -1 stands for no proxy is needed
            Jblend_Proxy_Dec.serverPort = 0;
            Jblend_Proxy_Dec.serverAddress = Proxy_addr;
            Jblend_Proxy_Dec.dynamicProxyHandler = NULL;
            JWE_LOG(kjava_socket_Jblendia_WapSetting, ("use net style,Proxy_addr =%s",Proxy_addr))
        }

        else
        {
            proxy_len = MIN(JAVA_MAX_PROXY_URL_LEN, linksetting_item_ptr->gateway_len);
            SCI_MEMCPY((char *)Proxy_addr, (char*)(linksetting_item_ptr->gateway), proxy_len);	// cmwap
            Jblend_Proxy_Dec.type = AJMS_PROXY_DESCRIPTOR_TYPE_BASIC;
            Jblend_Proxy_Dec.serverPort = linksetting_item_ptr->port;//80;
            Jblend_Proxy_Dec.serverAddress = Proxy_addr;
            Jblend_Proxy_Dec.dynamicProxyHandler = NULL;
            JWE_LOG(kjava_socket_Jblendia_WapSetting, ("use proxy"));
            JWE_LOG(kjava_socket_Jblendia_WapSetting, ("use wap  style Proxy_addr=%s",Proxy_addr))
        }
    }
    else
    {
        #ifdef WIFI_SUPPORT
        MMIWLAN_PROXY_SETTING_DETAIL_T detail_struct = {0};
        MMIAPIWIFI_GetProxySettingDetail(&detail_struct);
        if(detail_struct.is_use_proxy)
        {
            proxy_len = MIN(JAVA_MAX_PROXY_URL_LEN, detail_struct.gateway_len);
            SCI_MEMCPY((char *)Proxy_addr, (char*)(detail_struct.gateway), proxy_len);
            Jblend_Proxy_Dec.type = AJMS_PROXY_DESCRIPTOR_TYPE_BASIC;
            Jblend_Proxy_Dec.serverPort = detail_struct.port;
            Jblend_Proxy_Dec.serverAddress = Proxy_addr;
            Jblend_Proxy_Dec.dynamicProxyHandler = NULL;
        }
        else
        #endif
        {
            strcpy((char *)Proxy_addr, (const char *)"0.0.0.0");
            Jblend_Proxy_Dec.type = -1; // -1 stands for no proxy is needed
            Jblend_Proxy_Dec.serverPort = 0;
            Jblend_Proxy_Dec.serverAddress = Proxy_addr;
            Jblend_Proxy_Dec.dynamicProxyHandler = NULL;
        }
    }
    
    ajms_cfg_setHttpImpl(AJSC_HTTP_PROFILE_DEFAULT);
	
    MMIAPIENG_GetUa(&ua);
    ua_len =  SCI_STRLEN((char*)(ua.user_agent));

    if (0 != SCI_STRLEN((char*)(ua.user_agent)))
    {
      MMI_MEMCPY((char*)java_ua,JAVA_MAX_URL_LEN, ua.user_agent, ua_len, ua_len);
    }
    else
    {
         ua_len =  SCI_STRLEN((char*)MMIAPICOM_GetUserAgent());
         MMI_MEMCPY((char*)java_ua,JAVA_MAX_URL_LEN, MMIAPICOM_GetUserAgent(), ua_len, ua_len);
    }
	
	ajms_cfg_setHttpUserAgent((AJ_BYTE*)java_ua);
	JWE_LOG(kjava_socket_Jblendia_WapSetting, ("UserAgent = %s", java_ua))
	set_result = ajms_cfg_setHttpProxy(&Jblend_Proxy_Dec);
	JWE_LOG(kjava_socket_Jblendia_WapSetting, ("Leave! set_result = %d", set_result))
	JWE_LOG(kjava_socket_Jblendia_WapSetting, ("gateway = %s", Jblend_Proxy_Dec.serverAddress))
	JWE_LOG(kjava_socket_Jblendia_WapSetting, ("port = %d", Jblend_Proxy_Dec.serverPort))
	
}

#endif
/*-----------------------------------------------------------------------------
* Local functions
*----------------------------------------------------------------------------*/
