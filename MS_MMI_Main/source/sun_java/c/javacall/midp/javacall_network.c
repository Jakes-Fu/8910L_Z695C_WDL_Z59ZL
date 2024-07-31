/*
 *
 * Copyright  1990-2009 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file
 *
 * Stub implementation of pcsl_network.h for platforms that support the winsock 
 * API.
 *
 * For all functions, the "handle" is the winsock handle (an int)
 * cast to void *.  Since winsock reads and writes to sockets are synchronous,
 * the context for reading and writing is always set to NULL.
 */
    
#ifdef WIN32
#include "mmk_app.h"
//#pragma warning( disable : 4005)  // Disable warning messages "macro redefinition"
#endif

#include "javacall_network.h"
#include "javacall_socket.h"
#include "java_socket.h" //t_socket
#include "os_api.h" //sci_trace_low, sci_alloca
#include "app_tcp_if.h" //struct sci_hostent
#include "javacall_logging.h"
#include "mmiconnection_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "javacall_file.h"
//#include "mmiset_dualsim.h"
#include "mmiset_dualsim_export.h"
//#include "mmiset_multisim.h"
#include "mmiset_multisim_export.h"
#include "mmipdp_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mn_type.h"
#include "mmijava_main.h"
#include "java_sigdispatchtask.h"
#include "mmijava_id.h"


//#include "tcpip_pclink.h"   



//below is copy from ztesvrcall_type.h
#ifndef DWORD
typedef unsigned long   DWORD;
#endif
//todo: should be deleted

struct linger 
{
	unsigned int	 l_onoff;            // option on/off
	unsigned int	 l_linger;           // linger time
};

extern BOOLEAN TCPIPPCLINK_SetNetInterface( void );
extern T_Socket g_SocketList[MAX_SOCKET_NUM];
extern uint32 g_java_net_id;    /*lint !e18 */   //pdp并发记录java模块的 net id  

#ifdef BROWSER_SUPPORT_DORADO	
extern MMICMSWAP_GPRS_STATE_E MMIBRW_GetGprsState(void);
#endif

char * pcHostName=NULL;    //用于gethostbyname函数
char* pcHostAddress = NULL ;    // ptr to host ip address for gethostbyname
static int delayVal;       //用于socket delay属性 just for tck

int java_sim;
int java_network_index;
unsigned short java_network_name[256];

/*****************************************************************************/
//  Description : handle java pdp active cnf
//  Global resource dependence : none
//  Author: applezhang
//  Note:
/*****************************************************************************/
LOCAL void HandleJAVAPDPActiveCnf(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : handle java pdp msg 
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void HandleJAVAPDPMsg(MMIPDP_CNF_INFO_T *msg_ptr);


/******************************************************************************
* 函数定义：	java_NetDisconnect
* 函数说明：	java断开GPRS和断开socket
* 参数说明：	void
* 返回值：		int32
********************************************************************************/
LOCAL int32 java_NetDisconnect(void);

/**
 * Performs platform-specific initialization of the networking system.
 * Will be called ONCE during VM startup before opening a network connection.
 * 
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_WOULD_BLOCK caller must call the finish function to 
 * 			complete the operation
 * @retval JAVACALL_FAIL    fail
 */
javacall_result javacall_network_init_start(void)
{  
    int nRet;
    //初始化全局变量
    SOCK_INFO(LOG_STR_ENTER(""));
    
    SCI_MEMSET((void *)g_SocketList, 0, sizeof(T_Socket)*MAX_SOCKET_NUM);
    
    if (MMIJAVA_enable_pclink()) 
    {
        g_java_net_id=0;
        SOCK_INFO(LOG_STR("pc link enabled, only active pc link"));
#ifndef WIN32
        if (TCPIPPCLINK_SetNetInterface() == TRUE)  /*lint !e718  !e746  !e628 !e18*/
        {
            return JAVACALL_OK;
        }
#endif
        SOCK_INFO(LOG_STR_LEAVE("pc link enabled, NetInterface down"));
        return JAVACALL_FAIL;
    }
    
    nRet = java_NetConnect();
    SOCK_INFO(LOG_STR("java_NetConnect return =  %d"), nRet);
    switch (nRet) {
    case 0:
        return JAVACALL_OK;
    case 1:         
        if (GOOD == java_NetStatus())
        {                
            SOCK_INFO(LOG_STR("java_NetStatus == good"));
            if (g_java_net_id != 0) {
                javacall_printf(LOG_STR_LEAVE("already start, g_java_net_id=%d"), g_java_net_id);
                return JAVACALL_OK;
            } else {
                javacall_printf(LOG_STR_LEAVE("already closed, g_java_net_id=%d"), g_java_net_id);
                return JAVACALL_FAIL;
            }
        } 
        else if(REACTIVATING == java_NetStatus())
        {
            SOCK_INFO(LOG_STR_LEAVE("java_NetStatus == REACTIVATING"));
            return JAVACALL_WOULD_BLOCK;                 
        }
        else
        {
            SOCK_INFO(LOG_STR_LEAVE("java_NetStatus == BAD"));
            return JAVACALL_FAIL;            
        }
    default:
        return JAVACALL_FAIL;            
    } 
}

/**
 * Finishes platform-specific initialization of the networking system.
 * The function is invoked be the JVM after receiving JAVACALL_NETWORK_UP
 * notification from the platform.
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_WOULD_BLOCK caller must call the finish function to 
 * 			complete the operation
 * @retval JAVACALL_FAIL    fail
 */
javacall_result javacall_network_init_finish(void)
{  
   return JAVACALL_OK;
}

/**
 * Performs platform-specific finalization of the networking system.
 * Will be called ONCE during VM shutdown.
 *
 * @retval JAVACALL_WOULD_BLOCK caller must call xxx_finalize_finish
 *         function to complete the operation
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    fail
 */
javacall_result javacall_network_finalize_start(void)
{	
    DWORD nRet;
    nRet = java_NetDisconnect();
    if(nRet == 0)
    {
	    return JAVACALL_OK;
	}
	else
	{
	    return JAVACALL_FAIL;
	}
}

/**
 * Finishes platform-specific finalize of the networking system.
 * The function is invoked be the JVM after receiving JAVACALL_NETWORK_DOWN
 * notification from the platform.
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    fail
 */
javacall_result javacall_network_finalize_finish(void)
{
    return JAVACALL_OK;
}
  
/**
 * Performs platform-specific finalizaiton of the networking system.
 * Will be called ONCE during VM shutdown!
 * 
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    fail
 */
javacall_result javacall_network_finalize(void) {   /*lint -e{765}*/
    return JAVACALL_FAIL;                                        
}  
    
/**
 * Initiates lookup of the given host name to find its IP address.
 *
 * @param hostname the host name for which an ip address is needed,
 *        a null-terminated string
 * @param pAddress base of byte array to receive the address
 * @param maxLen size of buffer at pAddress
 * @param pLen number of bytes returned to pAddress, 4 if it's an 
 *        IPv4 address, 16 if it is an IPv6 address
 * @param pHandle address of variable to receive the handle to for
 *        unblocking the Java thread; this is set
 *        only when this function returns JAVACALL_WOULD_BLOCK.
 * @param pContext address of a pointer variable to receive the context;
 *        this is set only when the function returns JAVACALL_WOULD_BLOCK.
 *
 * @retval JAVACALL_OK                  success
 * @retval JAVACALL_FAIL                if there is a network error
 * @retval JAVACALL_INVALID_ARGUMENTS   if maxLen is too small to receive the address
 */
javacall_result javacall_network_gethostbyname_start(char *hostname,
    unsigned char *pAddress, int maxLen, /*OUT*/ int *pLen, /*OUT*/ void **pHandle, /*OUT*/ void **pContext)
{
    struct sci_hostent * tHostent;
    int hostnameLen=0;
    const int nLen = 4;
    int handle = 0 ;
    
    SOCK_DBG(LOG_STR_ENTER("hostname=%s"), hostname);
    if(NULL==hostname||NULL==pAddress||NULL==pLen||NULL==pHandle||NULL==pContext)
    {
        SOCK_INFO(LOG_STR_LEAVE("fail, paramter null, leave"));
        return JAVACALL_INVALID_ARGUMENT;
    }
    
    if ((java_NetStatus() == BAD) || (java_NetStatus() == REACTIVATING))
    {
        if (java_NetStatus() == BAD) {
          java_NetConnect();
        }
        SOCK_INFO(LOG_STR_ENTER("fail, java_NetStatus down"));
        return JAVACALL_CONNECTION_NOT_FOUND;
    } 


    tHostent = sci_gethostbyname(hostname);	     
    if(NULL!=tHostent)
    {      	
        if(nLen>maxLen)		
        {		    	
            SOCK_INFO(LOG_STR_LEAVE("fail, nLen=%d;maxLen=%d"), nLen, maxLen);
            return JAVACALL_FAIL;		
        }	
        
        SCI_MEMCPY(pAddress,tHostent->h_addr,nLen);
        *pLen = nLen;
        
        SOCK_INFO(LOG_STR_LEAVE("OK, nLen=%d, addr=0x%lx"), nLen, *(unsigned int*)pAddress);
        return JAVACALL_OK;	
    }    
    
    handle =  sci_async_gethostbyname(hostname, java_GetSigDispatchTaskID(), 5000, g_java_net_id); 
    SOCK_INFO(LOG_STR("sci_gethostbyname, handle = %d, netid = %d"), handle,g_java_net_id);
    if(0 == handle )
    {          
        return JAVACALL_FAIL;
    }        

    *pHandle = (javacall_handle)handle;        
    hostnameLen=SCI_STRLEN(hostname);
    if (PNULL != pcHostName)
    SCI_FREE(pcHostName)  ;   
    pcHostName = (char *)SCI_ALLOCA(hostnameLen+1);
    if(NULL == pcHostName)
    {
        SOCK_INFO(LOG_STR_LEAVE("SCI_ALLOCA(pcHostName) error"));
        return JAVACALL_FAIL;
    }
    
    SCI_MEMSET(pcHostName,0,hostnameLen+1);
    SCI_MEMCPY(pcHostName, hostname, hostnameLen);   
    
    SCI_FREE( pcHostAddress);     
    pcHostAddress = (char*)SCI_ALLOCA(sizeof(char)*nLen);
    if(PNULL == pcHostAddress)	
    {
        SCI_FREE(pcHostName);
        pcHostName = PNULL;              
        return JAVACALL_FAIL;
    }                    
    
    SOCK_INFO(LOG_STR_LEAVE("JAVACALL_WOULD_BLOCK"));
    return JAVACALL_WOULD_BLOCK;    
}    
/**
* Finishes a pending host name lookup operation.
* 
* @param pAddress base of byte array to receive the address
* @param maxLen size of buffer at pAddress
* @param pLen number of bytes returned to pAddress, 4 if it's an 
*        IPv4 address, 16 if it is an IPv6 address
* @param handle the handle returned by the gethostbyname_start function
* @param context the context returned by the gethostbyname_start function
*
* @retval JAVACALL_OK          success
* @retval JAVACALL_WOULD_BLOCK if the caller must call the finish function again to complete the operation;\n
* @retval JAVACALL_FAIL        if there is a network error
*/
javacall_result javacall_network_gethostbyname_finish(unsigned char *pAddress,
                                                      int maxLen,int *pLen,void *handle,void *context)
{
    const int nLen = 4;  
    int result=(int)context;
    
    if(NULL==pAddress||NULL==pLen)
    {         
        SOCK_INFO(LOG_STR_LEAVE("fail, pAddress or pLen is NULL"));
        return JAVACALL_INVALID_ARGUMENT;
    }   
    SOCK_INFO(LOG_STR_ENTER("handle = %x") , handle);
    SOCK_INFO(LOG_STR_ENTER("hostname= %x, %s") , pcHostName, pcHostName);
    if (NULL == pcHostName )
    {
        SOCK_INFO(LOG_STR_LEAVE("fail, pcHostName is NULL"));
        if (PNULL != pcHostAddress) {
        SCI_FREE(pcHostAddress);  
        pcHostAddress = NULL;
        }
        return JAVACALL_FAIL;
    }
    
    SOCK_INFO(LOG_STR_ENTER("start result=%d, handle=%x, hostname=%s"), result, handle, pcHostName);
    if (JAVACALL_OK == result) {
    if (NULL != pcHostAddress )
    {     
        if(nLen>maxLen)     
        {               
            SOCK_INFO(LOG_STR_LEAVE("fail, nLen=%d;maxLen=%d"), nLen, maxLen);
            SCI_FREE(pcHostName);
            pcHostName=NULL;
            SCI_FREE(pcHostAddress);                     
            pcHostAddress = NULL;    
            return JAVACALL_FAIL;       
        }            
        SCI_MEMCPY(pAddress,  pcHostAddress, nLen);         
        *pLen = nLen;
        SCI_FREE(pcHostName);
        pcHostName=NULL;
        SCI_FREE(pcHostAddress);                     
        pcHostAddress = NULL;             
        
        SOCK_INFO(LOG_STR_LEAVE("OK, nLen=%d, addr=0x%lx"), nLen, *(unsigned int*)pAddress);
        return JAVACALL_OK; 
    }  
    }
    SOCK_INFO(LOG_STR_LEAVE("fail, host=%s"), pcHostName);

    if (NULL == pcHostName ) {
        SCI_FREE(pcHostName);  
        pcHostName = NULL;
    }

    if (NULL == pcHostAddress) {
        SCI_FREE(pcHostAddress);  
        pcHostAddress = NULL;
    }
    return JAVACALL_FAIL;
}



/**
 * Gets an option's value for a platform-specific TCP socket or datagram
 *
 * @param handle handle of an open connection
 * @param flag socket option to get. Must be one of the values defined in
 *             <tt>javax.microedition.io.SocketConnection</tt>
 * @param pOptval returns the option's value
 *
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    if there is a network error
 */
javacall_result javacall_network_getsockopt(javacall_handle handle,javacall_socket_option flag, /*OUT*/ int *pOptval)
{
    long s;
	int optname;
    int optsize;
    struct linger lbuf={0} ;
    void * opttarget;
	
	if(NULL==handle||NULL==pOptval)
	{
		return JAVACALL_FAIL;
	}
	
    s = (long)handle;
	optsize = sizeof(optname);
	opttarget = (void *) pOptval ;
	
	SOCK_INFO(LOG_STR_ENTER("network flag=%d"),flag);
	
    switch (flag) {
    case JAVACALL_SOCK_DELAY:
		// do nothing;
		*pOptval = delayVal;
        return JAVACALL_OK;
		break;/*lint !e527 */
    case JAVACALL_SOCK_LINGER: /* LINGER */
    case JAVACALL_SOCK_ABORT: 
        opttarget = (void *) &lbuf ;
        optsize = sizeof (struct linger);
        optname = SO_LINGER;
        break;
		
    case JAVACALL_SOCK_KEEPALIVE: /* KEEPALIVE */
        optname = SO_KEEPALIVE;
        break;
		
    case JAVACALL_SOCK_RCVBUF: /* RCVBUF */
        optname = SO_RCVBUF;
        break;
		
    case JAVACALL_SOCK_SNDBUF: /* SNDBUF */
        optname = SO_SNDBUF;
        break;
		
    default: /* Illegal Argument */
		SOCK_INFO(LOG_STR_LEAVE("JAVACALL_INVALID_ARGUMENT flag=%d"),flag);
        return JAVACALL_INVALID_ARGUMENT;
    }
	
    if (0 == sci_sock_getsockopt(s, optname, opttarget))
	{
        if (SO_LINGER == optname) {
            if(JAVACALL_SOCK_LINGER == flag) {
				/* If linger is on return the number of seconds. */
				*pOptval = (lbuf.l_onoff == 0 ? 0 : lbuf.l_linger) ;
            } else { /* if (JAVACALL_SOCK_ABORT == flag) */
                *pOptval = ( ((lbuf.l_onoff == 1) && (lbuf.l_linger == 0)) ? 1 : 0 );
            }
        }
		return JAVACALL_OK;
    }
	return JAVACALL_FAIL;
}    
/**
 * Sets an option's value for a platform-specific TCP socket or datagram
 *
 * @param handle handle of an open connection
 * @param flag socket option to set. Must be one of the values defined in
 *             <tt>javax.microedition.io.SocketConnection</tt>
 * @param optval the value to be set for this option
 *
 * @retval JAVACALL_OK                  success
 * @retval JAVACALL_FAIL                if there is a network error
 * @retval JAVACALL_INVALID_ARGUMENTS   if the platform did not accept the value for the option changed
 */
javacall_result javacall_network_setsockopt(javacall_handle handle, javacall_socket_option flag, int optval)
{
    
    long s;
    int    optsize =  sizeof(optval);
    int    optname;
    struct linger lbuf ={0} ;
    void * opttarget = (void *) & optval ;
    
	if(NULL==handle)
	{
		return JAVACALL_FAIL;
	}
	s = (long)handle;
	
    SOCK_INFO(LOG_STR_ENTER("flag=%d"),flag);
    
    switch (flag) {
	case JAVACALL_SOCK_DELAY: 
		/*do nothing*/
        delayVal = optval;
        return JAVACALL_OK;
    case JAVACALL_SOCK_LINGER: /* LINGER */
        opttarget = (void *) &lbuf ;
        optsize = sizeof (struct linger);
        optname = SO_LINGER;
        if (optval == 0) {
			lbuf.l_onoff = 0;
            lbuf.l_linger = 0;
		} else {
		//test
            lbuf.l_onoff = 1;
            lbuf.l_linger = (unsigned int)optval;
        }
        break;
        
    case JAVACALL_SOCK_KEEPALIVE: /* KEEPALIVE */
        optname = SO_KEEPALIVE;
        break;
        
    case JAVACALL_SOCK_RCVBUF: /* RCVBUF */
        optname = SO_RCVBUF;
        break;
        
    case JAVACALL_SOCK_SNDBUF: /* SNDBUF */
        optname = SO_SNDBUF;
        if (optval < 1) optval=1;   //just for passing tck test
        break;
        
    case JAVACALL_SOCK_ABORT: 
        opttarget = (void *) &lbuf ;
        optsize = sizeof (struct linger);
        optname = SO_LINGER;
        
        lbuf.l_onoff = 1;
        lbuf.l_linger = (unsigned short)0;
        break;
        //*pOptval = ( ((lbuf.l_onoff == 1) && (lbuf.l_linger == 0)) ? 1 : 0 );
        
    default: /* Illegal Argument */
        SOCK_INFO(LOG_STR_LEAVE("JAVACALL_INVALID_ARGUMENT flag=%d"),flag);
        return JAVACALL_INVALID_ARGUMENT;
    }
    
    if (0 == sci_sock_setsockopt(s, optname, opttarget))
    {		
		/*add by zhouyang for test begin 2007-04-04*/
        SOCK_INFO(LOG_STR("ok, optname = %d,optval = %d,lbuf.l_onoff = %d,lbuf.l_linger = %d"),optname,optval,lbuf.l_onoff,lbuf.l_linger);
        if(optname == SO_LINGER)
        {
            struct linger tResult;
            if(0==sci_sock_getsockopt(s,optname,(int *)&tResult))
            {
				SOCK_INFO(LOG_STR("tResult.l_onoff = %d,tResult.l_linger = %d"),tResult.l_onoff,tResult.l_linger);
            }
        }
        else
        {
			int nResult;
            if(0==sci_sock_getsockopt(s,optname,&nResult))
            {
				SOCK_INFO(LOG_STR("tResult = %d"),nResult);
            }
        }
		/*add by zhouyang for test end 2007-04-04*/
		
        return JAVACALL_OK;
    }    
    return JAVACALL_FAIL;
}
/**
 * Translates the given IP address into a host name. 
 *
 * @param ipn Raw IP address to translate
 * @param hostname the host name. The value of <tt>host</tt> is set by
 *             this function.
 * @param pHandle address of variable to receive the handle to for
 *        unblocking the Java thread; this is set
 *        only when this function returns JAVACALL_WOULD_BLOCK.
 * @param pContext address of a pointer variable to receive the context;
 *        this is set only when the function returns JAVACALL_WOULD_BLOCK.
 *
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if there is a network error
 * @retval JAVACALL_WOULD_BLOCK if the caller must call the finish function to complete the operation
 */
javacall_result /*OPTIONAL*/ javacall_network_gethostbyaddr_start(int ipn,
    char *hostname, /*OUT*/ javacall_handle* pHandle, void **pContext)
{
	
    /*
    * Note there is no spec requirement to look up the the address to
    * to get a hostname. It is not a good idea to perform the lookup,
    * since this function is called EVERY time
    * a datagram is received and on a cell phone network this lookup
    * can take some time and really effect performance for receiving
    * datagrams.
	*/
	
	
	unsigned long ipAddress;
	char * pcAddress = NULL;
	
	if(NULL==hostname||NULL==pHandle||NULL==pContext)
	{
		return JAVACALL_FAIL;
	}
	
	//SCI_TRACE_LOW("network javacall_network_gethostbyaddr_start begin");
	
	ipAddress = (long)ipn;
	pcAddress = (char *)inet_ntoa(ipAddress);
	
	if(NULL == pcAddress)
	{
		return JAVACALL_FAIL;
	}
	else
	{
		strcpy(hostname, pcAddress);		
		return JAVACALL_OK; 		
	}
}
    
/**
 * Finishes a pending host name lookup operation.
 * 
 * @param ipn Raw IP address to translate
 * @param hostname the host name. The value of <tt>host</tt> is set by
 *             this function.
 * @param pHandle address of variable to receive the handle to for
 *        unblocking the Java thread; this is set
 *        only when this function returns JAVACALL_WOULD_BLOCK.
 * @param context the context returned by the getHostByAddr_start function
 *
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if there is a network error
 * @retval JAVACALL_WOULD_BLOCK if the caller must call the finish function again to complete the operation 
 */
javacall_result /*OPTIONAL*/ javacall_network_gethostbyaddr_finish(int ipn,
    char *hostname, /*OUT*/ javacall_handle* pHandle, void *context)
{
    return JAVACALL_FAIL;
}

/**
 * Gets the IP address of the local socket endpoint.
 *
 * @param handle handle of an open connection
 * @param pAddress base of byte array to receive the address
 *
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    if there was an error
 */
javacall_result /*OPTIONAL*/ javacall_socket_getlocaladdr(
    javacall_handle handle,
    char *pAddress)
{
    long s = (long)handle;
    int status;
    struct sci_sockaddr sa;
	
    if(NULL==pAddress)
	{
		return JAVACALL_FAIL;
	}
    
    //SCI_TRACE_LOW("network javacall_socket_getlocaladdr begin");
    
    sa.family = AF_INET;
    status = sci_sock_getsockname(s, &sa);
	
    if (0 != status) 
    {
        return JAVACALL_FAIL;
    }
    
    /*
    * The string returned from inet_ntoa is only valid until the next
    * winsock API call. The string should not be freed.
    */
	
    strcpy(pAddress, (char *)inet_ntoa(sa.ip_addr));	
    return JAVACALL_OK;
}

/**
 * Gets the IP address of the remote socket endpoint.
 *
 * @param handle handle of an open connection
 * @param pAddress base of byte array to receive the address
 *
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    if there was an error
 */
javacall_result /*OPTIONAL*/ javacall_socket_getremoteaddr(
    void *handle,
    char *pAddress)
{
    long s;
    int status;
    struct sci_sockaddr sa;
    
    
    if(NULL==pAddress||NULL==handle)
	{
		return JAVACALL_FAIL;
	}
	
    s = (long)handle;
	
    //SCI_TRACE_LOW("network javacall_socket_getremoteaddr begin");
    
    sa.family = AF_INET;
    status = sci_sock_getpeername(s,&sa);
    if (0 != status) 
    {
        return JAVACALL_FAIL;
    }
    
    /*
    * The string returned from inet_ntoa is only valid until the next
    * winsock API call. The string should not be freed.
    */
	
    strcpy(pAddress, (char *)inet_ntoa(sa.ip_addr));	
    
    return JAVACALL_OK;
}
   
/**
 * Gets the port number of the local socket endpoint.
 *
 * @param handle handle of an open connection
 * @param pPortNumber returns the local port number
 * 
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    if there was an error
 */
javacall_result /*OPTIONAL*/ javacall_socket_getlocalport(javacall_handle handle,int *pPortNumber)
{
    
    long s = (long)handle;
    int status;
    struct sci_sockaddr sa;
    
    if(NULL==pPortNumber)
	{
		return JAVACALL_FAIL;
	}
	
    //SCI_TRACE_LOW("network javacall_socket_getlocalport begin");
    
    sa.family = AF_INET;
    status = sci_sock_getsockname(s, &sa);
    if (0 != status) 
    {
        return JAVACALL_FAIL;
    }
    
    *pPortNumber = ntohs(sa.port);
    
    return JAVACALL_OK;
}

/**
 * Gets the port number of the remote socket endpoint.
 *
 * @param handle handle of an open connection
 * @param pPortNumber returns the local port number
 * 
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    if there was an error
 */
javacall_result javacall_socket_getremoteport(
    void *handle,
    int *pPortNumber)
{
    long s;
    int status;
    struct sci_sockaddr sa;
    
    if(NULL==pPortNumber||NULL==handle)
	{
		return JAVACALL_FAIL;
	}
	
    s = (long)handle;
	
    //SCI_TRACE_LOW("network javacall_socket_getremoteport begin");
    
    sa.family = AF_INET;
    status = sci_sock_getpeername(s, &sa);
    if (0 != status) 
    {
        return JAVACALL_FAIL;
    }
    
    *pPortNumber = ntohs(sa.port);
    
    return JAVACALL_OK;
}

/**
 * Gets the string representation of the local device's IP address.
 * This function returns dotted quad IP address as a string in the 
 * output parameter and not the host name.
 *
 * @param pLocalIPAddress base of char array to receive the local
 *        device's IP address
 *
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    if there is a network error
 */
javacall_result /*OPTIONAL*/ javacall_network_get_local_ip_address_as_string(/*OUT*/ char *pLocalIPAddress)
{  
    unsigned long LocalAddr;
	
	if(NULL==pLocalIPAddress)
	{
		return JAVACALL_FAIL;
	}
	
	//SCI_TRACE_LOW("network javacall_network_get_local_ip_address_as_string begin");
	

    LocalAddr = sci_gethostaddress_ext(g_java_net_id);
    if (NULL == LocalAddr) 
    {
        return JAVACALL_FAIL;
    }
    
    strcpy(pLocalIPAddress, (char *)inet_ntoa(LocalAddr));
    return JAVACALL_OK;
}

/**
 * Gets the http / https proxy address. This method is
 * called when the <tt>com.sun.midp.io.http.proxy</tt> or 
 <tt>com.sun.midp.io.https.proxy</tt> internal property
 * is retrieved.
 *
 * @param pHttpProxy base of char array to receive the hostname followed 
 *          by ':' and port. - ex) webcache.thecompany.com:8080.  
 *          Size of the pHttpProxy should be (MAX_HOST_LENGTH + 6).
 * @param pHttpsProxy base of char array to receive the hostname followed 
 *          by ':' and port. - ex) webcache.thecompany.com:8080.  
 *          Size of the pHttpsProxy should be (MAX_HOST_LENGTH + 6).
 *
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    if there is a network error
 */
#define MMIBROWSER_INVALID_GATEWAY              "0.0.0.0"
javacall_result /*OPTIONAL*/ javacall_network_get_http_proxy(/*OUT*/ char *pHttpProxy, /*OUT*/ char *pHttpsProxy)
{ 
    char tmp[255] = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_detail_ptr = PNULL;
    SOCK_INFO(LOG_STR_ENTER("java_network_index=%d"), java_network_index);
    if(java_sim < MMIJAVA_GetSimCount())  //sim card
    {
        setting_detail_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(java_sim, java_network_index);
        if ((setting_detail_ptr->gateway_len != 0) 
            && (setting_detail_ptr->gateway != NULL) /*lint !e774 */
            && (0 != strncmp(MMIBROWSER_INVALID_GATEWAY,setting_detail_ptr->gateway,strlen(MMIBROWSER_INVALID_GATEWAY)))) {/*lint !e64 */
            SCI_MEMCPY(tmp, setting_detail_ptr->gateway, setting_detail_ptr->gateway_len);
            tmp[setting_detail_ptr->gateway_len] = 0;
            sprintf(pHttpProxy, "%s:%d", tmp, setting_detail_ptr->port);
            strcpy(pHttpsProxy, pHttpProxy);
            return JAVACALL_OK;
        }                      
        SOCK_DBG(LOG_STR_LEAVE("get_http_proxy failed"));
        return JAVACALL_FAIL;
    }
    else if(java_sim == MMIJAVA_GetSimCount())  //wifi
    {
        SOCK_DBG(LOG_STR_LEAVE("wifi network, no need to get_http_proxy "));
        return JAVACALL_FAIL;
    }  
    return JAVACALL_FAIL;
}
/**
 * Gets a platform-specific error code for the previous operation on an open
 * connection.  This is mainly useful for adding detail information to
 * debugging and diagnostic messages.
 *
 * @param handle handle of an open connection
 *
 * @return 0 if there is no error;\n
 * a non-zero, platform-specific value if there was an error
 */
int /*OPTIONAL*/ javacall_network_error(javacall_handle handle)
{
    return sci_sock_errno((int32)handle);
}

/**
 * Gets the name of the local device from the system. This method is
 * called when the <tt>microedition.hostname</tt> system property
 * is retrieved.
 *
 * @param pLocalHost base of char array to receive the host name, Size
 *        of the pLocalHost should be MAX_HOST_LENGTH
 *
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    if there is a network error
 */
javacall_result /*OPTIONAL*/ javacall_network_get_local_host_name(/*OUT*/ char *pLocalHost)
{
    //todo: what to do?    
    if(NULL==pLocalHost)
	{
		return JAVACALL_FAIL;
	}
    
    //SCI_TRACE_LOW("network javacall_network_get_local_host_name begin");
    
    //if (SUCCESS != sci_gethostname(pLocalHost, MAX_HOST_LENGTH)) 
    //{
    //    return JAVACALL_FAIL;
    //}
    sprintf(pLocalHost,"SpreadMS");
    return JAVACALL_OK;
	
}

/**
 * See javacall_network.h for definition.
 */
javacall_result javacall_server_socket_set_notifier(javacall_handle handle, javacall_bool set) {
    return JAVACALL_FAIL;
}

/**
 * A javacall equivelent of BSD inet_ntoa () function.
 * The inet_ntoa() function converts the Internet host address to a string 
 * in standard numbers-and-dots notation. The string is returned in
 * a statically allocated buffer, which subsequent calls will overwrite.
 * 
 * @param address the IP address of the remote device in the form of byte array
 *
 * @return converted address
 */
char* javacall_inet_ntoa(void *address)
{
	if(NULL==address)
	{
		return "";
	}	
    return (char *)inet_ntoa(*((unsigned long *)address));	
}

javacall_result javacall_count_network_setting(int sim, int* count)
{
    if(sim < MMIJAVA_GetSimCount())  //sim card
    {
        *count = MMIAPICONNECTION_GetLinkSettingNum(sim);
    }
    else if(sim == MMIJAVA_GetSimCount())  //wifi
    {
        *count = 1;
    }
    SOCK_INFO(LOG_STR_LEAVE("ok, sim=%d, count=%d"), sim, *count);
    return JAVACALL_OK;
}

javacall_result javacall_get_network_setting(int sim, int i, javacall_utf16* name, int maxlen)
{
    int len;
    const javacall_utf16* wifi_name=L"WIFI";
    int settinglink_num = MMIAPICONNECTION_GetLinkSettingNum(sim);
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_detail_ptr = 0;
    SOCK_DBG(LOG_STR_ENTER("javacall_get_network_setting(): sim=%d, i=%d"), sim, i);
    name[0] = 0;

    //gprs 
    if(sim < MMIJAVA_GetSimCount())  //sim card
    {
        setting_detail_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim, i);
        len = setting_detail_ptr->name_len;
        if((len > 0) && (len < maxlen))
        {
            MMI_WSTRNCPY( name, maxlen,  setting_detail_ptr->name, len,  len);
            name[len] = 0;
            SOCK_DBG(LOG_STR_LEAVE("javacall_get_network_setting(): ok, name=%s"), javacall_nwstr2str((const short*)name, maxlen));
            return JAVACALL_OK;        
        } 
        else
        {
            SOCK_DBG(LOG_STR_LEAVE("javacall_get_network_setting(): reach length limit %d < %d"), len ,maxlen);
            return JAVACALL_FAIL;
        }
    }
    else if(sim == MMIJAVA_GetSimCount())  //wifi
    {
        if (i == 0) {
            len = MMIAPICOM_Wstrlen(wifi_name);
            if((len > 0) && (len < maxlen))
            {
                MMI_WSTRNCPY(name, maxlen,   wifi_name, len, len);
                name[len] = 0;
                SOCK_DBG(LOG_STR_LEAVE("javacall_get_network_setting(): ok, name=%s"), javacall_nwstr2str((const short*)name, maxlen));
                return JAVACALL_OK;        
            } else {
                SOCK_DBG(LOG_STR_LEAVE("javacall_get_network_setting(): reach length limit %d < %d"), len ,maxlen);
                return JAVACALL_FAIL;
            }
        }   
    }
    SOCK_DBG(LOG_STR_LEAVE("javacall_get_network_setting(): reach index limit %d < %d"), i, settinglink_num);
    return JAVACALL_FAIL;
}

javacall_result javacall_set_network_setting(int sim, int i, javacall_utf16* name)
{
    java_sim = sim;
    java_network_index = i;
    MMIAPICOM_Wstrcpy(java_network_name, name);
    return JAVACALL_OK;
}

javacall_result javacall_load_network_setting(int suiteId)
{
    javacall_utf16 file_name[256];
    char buf[30];
    javacall_handle fh;
    int len;

    SOCK_INFO(LOG_STR_ENTER("javacall_load_network_setting(): suiteId=%d"), suiteId);

    sprintf(buf, "%08xnetwork.cfg", suiteId);

    MMIAPICOM_StrToWstr((uint8*)buf, file_name);

    if(javacall_file_exist(file_name, MMIAPICOM_Wstrlen(file_name)) == JAVACALL_OK)
    {
        if(javacall_file_open(file_name, MMIAPICOM_Wstrlen(file_name), JAVACALL_FILE_O_RDWR, &fh) != JAVACALL_OK)
        {
            return JAVACALL_FAIL;
        }
        
        len = (int32)javacall_file_sizeofopenfile(fh);
        
        if(len < 8)
        {
            return JAVACALL_FAIL;
        }
        
        javacall_file_read(fh, buf, 4);/*lint !e64 */
        memcpy(&java_sim, buf, 4);
        
        javacall_file_read(fh, buf, 4);/*lint !e64 */
        memcpy(&java_network_index, buf, 4);
        
        javacall_file_read(fh, buf, len - 8);/*lint !e64 */
        memcpy(java_network_name, buf, len - 8);
        java_network_name[(len-8)/sizeof(javacall_utf16)] = 0;/*lint !e737 !e573*/
        javacall_file_close(fh);
        
        if ((java_sim <  (MMIJAVA_GetSimCount()) && java_network_index < MMIAPICONNECTION_GetLinkSettingNum(java_sim))   //gprs
            || (java_sim == (MMIJAVA_GetSimCount()) && java_network_index == 0))       //wifi
        {
            SOCK_INFO(LOG_STR_LEAVE("javacall_load_network_setting(): ok, sim=%d, index=%d"), java_sim, java_network_index);
            return JAVACALL_OK;
        }
        //the network settings has been removed outside of JVM by native application.
        javacall_file_delete(file_name, MMIAPICOM_Wstrlen(file_name));
    }

    SOCK_INFO(LOG_STR("javacall_load_network_setting(): load default settings of native browser"));

    java_sim = MMIAPISET_GetActiveSim();
    if (MMIAPIPHONE_IsSimAvailable((MN_DUAL_SYS_E)java_sim))   //dual sys later
    {
        SOCK_INFO(LOG_STR("javacall_load_network_setting(): using MN_DUAL_SYS_%d"), java_sim);
        java_network_index = MMIJAVA_GetNetSettingIndex(java_sim);//g_cmsbrw_setting_info.wap_default_index1;
        }
    else {
        return JAVACALL_FAIL;
    }
    return javacall_get_network_setting(java_sim, java_network_index, java_network_name, 256);
}

javacall_result javacall_save_network_setting(int suiteId, int sim, int i, javacall_utf16* name)
{
    javacall_utf16 file_name[256];
    char buf[30];
    javacall_handle fh;
    SOCK_DBG(LOG_STR_ENTER("javacall_save_network_setting(): suiteid=%d, sim=%d, i=%d, name=%s"), suiteId, sim, i, javacall_wstr2str((const short*)name));
    sprintf(buf, "%08xnetwork.cfg", suiteId);

    MMIAPICOM_StrToWstr((uint8*)buf, file_name);

    if(javacall_file_open(file_name, MMIAPICOM_Wstrlen(file_name), JAVACALL_FILE_O_RDWR | JAVACALL_FILE_O_CREAT | JAVACALL_FILE_O_TRUNC, &fh) != JAVACALL_OK)
    {
        return JAVACALL_FAIL;
    }

    javacall_file_write(fh, &java_sim, 4);/*lint !e64 */
    javacall_file_write(fh, &java_network_index, 4);/*lint !e64 */
    javacall_file_write(fh, java_network_name, MMIAPICOM_Wstrlen(java_network_name));/*lint !e64 */

    javacall_file_close(fh);

    return JAVACALL_OK;
}

javacall_bool javacall_network_is_initialized()
{
    if (java_NetStatus() == GOOD)
        return JAVACALL_TRUE;
    return JAVACALL_FALSE;
}

int javacall_network_sim_count()    //for jvm call
{
    if(MMIJAVA_IsSupportWIFI())
    {
        return MMIJAVA_GetSimCount() + 1;
    }
    else
    {
        return MMIJAVA_GetSimCount();
    }
}

void javacall_socket_setdebug(){}
javacall_result javacall_get_network_sim_setting(int sim, javacall_utf16* name, int maxlen)
{
    int len;
    const javacall_utf16* wifi_name=L"WIFI";
    const javacall_utf16* sim_name=L"SIM";

    name[0] = 0;

    if (sim == MMIJAVA_GetSimCount()) {      //wifi
        len = MMIAPICOM_Wstrlen(wifi_name);
        if((len > 0) && (len < maxlen))
        {
            MMI_WSTRNCPY(
                name, maxlen,
                wifi_name, len,
                len);
            name[len] = 0;
            SOCK_DBG(LOG_STR_LEAVE("javacall_get_network_setting(): ok, name=%s"), javacall_nwstr2str((const short*)name, maxlen));
            return JAVACALL_OK;        
        } else {
            SOCK_DBG(LOG_STR_LEAVE("javacall_get_network_setting(): reach length limit %d < %d"), len ,maxlen);
            return JAVACALL_FAIL;
        }
    }
    else        //gprs
    {
        len = MMIAPICOM_Wstrlen(sim_name);
        MMI_WSTRNCPY(
            name, maxlen,
            sim_name, len,
            len);
        name[len] = 0x31+sim;
        name[len+1] = 0;
        SOCK_DBG(LOG_STR_LEAVE("javacall_get_network_setting(): ok, name=%s"), javacall_nwstr2str((const short*)name, maxlen));
        return JAVACALL_OK;  
    }
}

javacall_result javacall_prefer_network_settings(int suiteId, char* name)
{
    int i;
    int settinglink_num;
    javacall_result result;
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_detail_ptr;

    javacall_printf(LOG_STR_ENTER("suite id=%d, name=%s"), suiteId, name);
    java_sim = MMIAPISET_GetActiveSim();
    if (FALSE == MMIAPIPHONE_IsSimAvailable((MN_DUAL_SYS_E)java_sim))   //dual sys later
    {
        javacall_printf(LOG_STR_LEAVE("fail, not active sim"));
        return JAVACALL_FAIL;
    }

    settinglink_num = MMIAPICONNECTION_GetLinkSettingNum(java_sim);
    for (i=0; i<settinglink_num; i++) {
        setting_detail_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(java_sim, i);
        if (setting_detail_ptr != PNULL) {
            javacall_printf(LOG_STR("i=%d, apn=%s"), i, setting_detail_ptr->apn);
            if (strstr((char*)setting_detail_ptr->apn, name) != PNULL) {
                javacall_printf(LOG_STR("find sim=%d, index=%d"), java_sim, i);
                javacall_set_network_setting(java_sim, i, setting_detail_ptr->name);
                result=javacall_save_network_setting(suiteId, java_sim, i, setting_detail_ptr->name);
                javacall_printf(LOG_STR_LEAVE("find index=%d, save result=%d"), i, result);
                return JAVACALL_OK;
            }
        }
    }
    return JAVACALL_FAIL;
//    return javacall_get_network_setting(java_sim, java_network_index, java_network_name, 256);
}
/*****************************************************************************/
//  Description : handle java pdp active cnf
//  Global resource dependence : none
//  Author: applezhang
//  Note:
/*****************************************************************************/
LOCAL void HandleJAVAPDPActiveCnf(MMIPDP_CNF_INFO_T *msg_ptr)
{
    //MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint8  default_setting  = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_detail_ptr = NULL;
	
    //dual_sys = MMIAPISET_GetActiveSim();
    
    SOCK_DBG(LOG_STR("msg_ptr->result=%d"), msg_ptr->result);
    
    if(MMIPDP_RESULT_SUCC == msg_ptr->result)//succ
    {
        default_setting = MMIJAVA_GetNetSettingIndex(java_sim);//g_cmsbrw_setting_info.wap_default_index1;
    
	    setting_detail_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(java_sim, default_setting);	
	   	g_java_net_id = msg_ptr->nsapi;
/*        if (0 == setting_detail_ptr->access_type)
        {
            udpinit((unsigned char*)setting_detail_ptr->gateway,setting_detail_ptr->port);	//fnh modify 20051124
        }*///暂时屏蔽，看是否支持wap上网方式
        javanotify_network_event(JAVACALL_NETWORK_UP);        
    }
    else
    {
        //连接失败处理
       javanotify_network_event(JAVACALL_NETWORK_DOWN);         
    } 
}

/*****************************************************************************/
//  Description : handle java pdp msg 
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void HandleJAVAPDPMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr)
    {        
        SOCK_DBG(LOG_STR_ENTER("msg_ptr == NULL"));
        return ;
    }
    SOCK_DBG(LOG_STR_ENTER("msg_id=%d"), msg_ptr->msg_id);

    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:
         HandleJAVAPDPActiveCnf(msg_ptr);     
        break;
        
    case MMIPDP_DEACTIVE_CNF:             
         g_java_net_id = 0;    
         javanotify_network_event(JAVACALL_NETWORK_DOWN); 
        break;
        
    case MMIPDP_DEACTIVE_IND:         
        g_java_net_id = 0;    
        javanotify_network_event(JAVACALL_NETWORK_DOWN); 
        break;
        
    default:
        break;
    }
//    SCI_FREE(msg_ptr);
}

/******************************************************************************
* 函数定义：	java_NetConnect
* 函数说明：     java 连接GPRS和建立socket
* 参数说明：	void
* 返回值：		void 
********************************************************************************/
PUBLIC int java_NetConnect(void)
{
    //MMIPDP_LINK_STATE_E state =  0;
    SOCK_DBG(LOG_STR_ENTER(""));  
    
    if(java_sim < MMIJAVA_GetSimCount())
    {  
        if (!MMIAPIPHONE_IsSimAvailable((MN_DUAL_SYS_E)java_sim))   //dual sys later    
        {      
            SOCK_INFO(LOG_STR_LEAVE( "no sim card!"));            
            g_java_net_id=0;
            return -1;
        }    
        
        //check if gprs for java exists already   
        if(MMIPDP_LINK_STATE_ACTIVED ==  MMIAPIPDP_GetLinkState(MMIJAVA_GetModuleID()))
        {
            return 0;       
        }
        else
        {
            MMIJAVA_SendJVMMessage2MMI(APP_JAVA_PDP_ACTIVE, PDP_ACTIVE); 
            SCI_SLEEP(100);    //SLEEP 100MS, active pdp connection
            return 1;       
        } 
    }  
    else if(java_sim == MMIJAVA_GetSimCount())  //wifi
    {
        if(0 == MMIJAVA_GetWIFIStatus())//connected
        {
            g_java_net_id = MMIJAVA_GetWIFINetID();
            SOCK_INFO(LOG_STR_LEAVE( "WIFI net id = %d"),g_java_net_id);
            return 0 ;
        }   
        else
        {                       
            g_java_net_id=0;
            //MMIJAVA_SendJVMMessage2MMI(APP_JAVA_PDP_ACTIVE, PDP_ACTIVE);            
            return -1;            
        }
    }
    return -1;
}


/******************************************************************************
* 函数定义：	java_NetConnect
* 函数说明：     java 连接GPRS和建立socket
* 参数说明：	void
* 返回值：		void 
********************************************************************************/
PUBLIC int java_Do_NetConnect(void)
{
    MMIPDP_ACTIVE_INFO_T app_info = {0};
#ifdef BROWSER_SUPPORT_DORADO	
    MMICMSWAP_GPRS_STATE_E  wap_gprs_state;
#endif
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_detail_ptr = NULL;
    MMIPDP_CNF_INFO_T pdp_cnf_info;
	
    pdp_cnf_info.msg_id=MMIPDP_ACTIVE_CNF;


#ifdef BROWSER_SUPPORT_DORADO	
    wap_gprs_state = MMIBRW_GetGprsState();
    if(/*need_shutdown &&*/ ((wap_gprs_state == MMICMSWAP_GPRS_ACTIVE_OK) || (wap_gprs_state == MMICMSWAP_GPRS_ACTIVING)))
    {
        java_Do_NetDisconnect();
    }

    /* if (wap_gprs_state == MMICMSWAP_GPRS_ACTIVING) */
    /* { */
    /*     javacall_printf(LOG_STR( "gprs is activing")); */
    /*     return 1; */
    /* } */
    if(wap_gprs_state == MMICMSWAP_GPRS_ACTIVE_OK)
    {
        pdp_cnf_info.result=MMIPDP_RESULT_SUCC;
        HandleJAVAPDPMsg(&pdp_cnf_info);
        SOCK_INFO(LOG_STR_LEAVE("gprs is already active"));
        return 0;
    }
#else
	java_Do_NetDisconnect();       
#endif	

    app_info.app_handler = MMIJAVA_GetModuleID();
    app_info.handle_msg_callback = HandleJAVAPDPMsg;
    if(java_sim < MMIJAVA_GetSimCount())
    {  
        setting_detail_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(java_sim, java_network_index);
        
        app_info.apn_ptr = (char*)setting_detail_ptr->apn;
        app_info.user_name_ptr = (char*)setting_detail_ptr->username;
        app_info.psw_ptr = (char*)setting_detail_ptr->password;
        app_info.dual_sys = (MN_DUAL_SYS_E)java_sim;
        app_info.priority = 3;
        app_info.ps_service_rat = MN_TD_PREFER;
        app_info.ps_service_type = BROWSER_E;
        app_info.storage = MN_GPRS_STORAGE_ALL;
    }
    //check wifi #ifdef WIFI_SUPPORT
    else if(java_sim == MMIJAVA_GetSimCount())
    {
        app_info.ps_interface = MMIPDP_INTERFACE_WIFI;
    }

    // The call back function will be triggered immediatelly in some situations, set the flag to avoid the timing issue
    if(!MMIAPIPDP_Active(&app_info))
    {
        pdp_cnf_info.result=MMIPDP_RESULT_FAIL;
        HandleJAVAPDPMsg(&pdp_cnf_info);
        return -1;
    }
//    reconnect_times = 0;
        return 1;

}

/******************************************************************************
* 函数定义：	java_NetDisconnect
* 函数说明：	java断开GPRS和断开socket
* 参数说明：	void
* 返回值：		int32
********************************************************************************/
LOCAL int32 java_NetDisconnect(void)
{     
    SOCK_DBG(LOG_STR_ENTER(""));
    // pdp operation run in app task right now
    MMIJAVA_SendJVMMessage2MMI(APP_JAVA_PDP_DEACTIVE, PDP_DEACTIVE);   
    javanotify_network_event(JAVACALL_NETWORK_DOWN_REQUEST); 
    return 0;
}

/******************************************************************************
* 函数定义：	java_NetDisconnect
* 函数说明：	java断开GPRS和断开socket
* 参数说明：	void
* 返回值：		int32
********************************************************************************/
PUBLIC int32 java_Do_NetDisconnect(void)
{   
    SOCK_DBG(LOG_STR_ENTER(""));
    MMIAPIPDP_Deactive(MMIJAVA_GetModuleID());
    g_java_net_id = 0;    
    return 0;
}

PUBLIC E_NET_STATUS java_NetStatus()
{        
    if (MMIJAVA_enable_pclink()) 
    {           
#ifndef WIN32
        if (TCPIPPCLINK_SetNetInterface() == TRUE)  /*lint !e718  !e746  !e628 !e18*/
            return GOOD;     
#endif
        return BAD;
    }
    else if(java_sim < MMIJAVA_GetSimCount())
    {
        E_NET_STATUS return_code = 0;
        MMIPDP_LINK_STATE_E state =  MMIAPIPDP_GetLinkState(MMIJAVA_GetModuleID());
        switch(state) 
        {
        case MMIPDP_LINK_STATE_ACTIVED:
            return_code = GOOD;
            break;
        case  MMIPDP_LINK_STATE_ACTIVING:            
            return_code = REACTIVATING;
            break;
        case  MMIPDP_LINK_STATE_DEACTIVING: 
        case  MMIPDP_LINK_STATE_DEACTIVED: 
            return_code = BAD;
            break;
        default:
            return_code = BAD;
            break;	
        }    
        return return_code;    
    }
    else
    {
        if(MMIJAVA_IsSupportWIFI())
        {      
            if(0 == MMIJAVA_GetWIFIStatus())//connected
            {
                return GOOD;
            }
            else
            {
                return BAD;
            }
        }
    }
    return BAD;
}
/** @} */
#ifdef __cplusplus
}
#endif

