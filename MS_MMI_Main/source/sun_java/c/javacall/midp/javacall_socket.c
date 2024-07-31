/*
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

#include "tasks_id.h"
#include "javacall_socket.h"
#include "java_socket.h" //t_sessioncontext
#include "app_tcp_if.h" //af_inet
#include "os_api.h" //sci_trace_low, sci_alloca
//#include "mmiwap.h" //einprogress
#include "javacall_logging.h"
#include "javacall_network.h"
#include "javacall_datagram.h"
#include "javacall_time.h"
#include "java_sigdispatchtask.h"


#ifndef INVALID_SOCKET
#define INVALID_SOCKET	((long) -1)
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR	-1
#endif


extern T_Socket g_SocketList[MAX_SOCKET_NUM];
extern unsigned int g_java_net_id;//pdp并发记录java模块的 net id    /*lint !e18*/

/*****************************************************************************
*	Function		:    java_SocketRead
*	Description	    :    socket read
*	Input    		:    handle:  socket handle
*                        pData :  buffer to receive read data 
*                        len   :  number of bytes to attempt to read 
*	Output       	:    pBytesRead:number of bytes actually read   
*	Return    	 	:     
*	Other    		: 	 None   
******************************************************************************/
static javacall_result java_SocketRead(void *handle,
										unsigned char *pData,
										int len,
										int *pBytesRead,
										int *pBytesLeft)
{
    
    int bytesRead;
    long s;
	int lastError;
	int bytesLeft ;	 
	javacall_int32  return_error = 0;
	javacall_int32 	try_num = 0;
	s = (long)handle;
    
TRY_AGAIN :
    bytesRead = sci_sock_asyncrecv(s, (char*)pData, len, 0, &bytesLeft);  
    if (SOCKET_ERROR != bytesRead) 
    {
        *pBytesRead = bytesRead;
        *pBytesLeft = bytesLeft ;
        return JAVACALL_OK;
    }
    
    lastError = sci_sock_errno(s);    
    SOCK_INFO(LOG_STR("sci_sock_errno socket = %d, error = %d"), s, lastError);
    switch (lastError)
    {
    case EWOULDBLOCK:            
        return_error = JAVACALL_WOULD_BLOCK;
        break;
        
    case ENOTSOCK:            
        return_error =  JAVACALL_INTERRUPTED;
        break;
        
    case ENETUNREACH:            
        return_error =  JAVACALL_CONNECTION_NOT_FOUND;
        break;     
        
	case 0:
		if(0 < *pBytesRead)
		{
			return JAVACALL_OK;
		}
		else
		{	
			try_num++;
    		SOCK_INFO(LOG_STR("error sci_sock_asyncrecv return -1 ,try = %d"), try_num);
			goto TRY_AGAIN;
		}
        break;     
    default:
        return_error =  JAVACALL_FAIL;
        break;
    }
    
    return return_error ;
}

/*****************************************************************************
*	Function		:    java_SocketWrite
*	Description 	:    write data
*	Input    		:    handle:socket handle
*                        pData:	buffer containing data to be written 
*                        len:	number of bytes to attempt to write 
*	Output    	    :    pBytesWritten: number of bytes written after successful write operation
*	Return    	 	:    
*	Other    		: 	  None   
******************************************************************************/
static javacall_result java_SocketWrite(void *handle,
										 char *pData,
										 int len,
										 int *pBytesWritten)
{
    
    long s;
    int bytesSent;
	int lastError;
	javacall_int32  return_error = 0;

	s = (long)handle;
    
    bytesSent = sci_sock_send(s, pData, len, 0);
    if (SOCKET_ERROR != bytesSent) 
    {
        *pBytesWritten = bytesSent;
        return JAVACALL_OK;
    }

    lastError = sci_sock_errno(s);    
    SOCK_INFO(LOG_STR("sci_sock_send socket = %d, error = %d"), s, lastError);
    switch (lastError)
    {
    case EWOULDBLOCK:            
        return_error = JAVACALL_WOULD_BLOCK;
        break;
        
    case ENOTSOCK:            
        return_error =  JAVACALL_INTERRUPTED;
        break;
        
    case ENETUNREACH:            
        return_error =  JAVACALL_CONNECTION_NOT_FOUND;
        break;     
        
    default:
        return_error =  JAVACALL_FAIL;
        break;
    }
    
    return return_error ;
}


/*****************************************************************************
*	Function		:    java_GetSocketList
*	Description 	:    get a position in array g_SocketList
*	Input    		:    s: socket handle                     
*	Return    	 	:    position of array
*	Other    		: 	  None   
*      author           :      ying.xu
*     date               :      20101119
******************************************************************************/
int32 java_GetSocketList(int32 s, int32 port, int32 socket_type)
{
    int i = 0;
    //SOCK_INFO(LOG_STR_ENTER("socket = %d, port = %d"),s, port);
    for(i=0;i<MAX_SOCKET_NUM;i++)
    {
        if(g_SocketList[i].nSocket == 0)
        {
            g_SocketList[i].nSocket = s;
            g_SocketList[i].nPort = port;
            g_SocketList[i].block = NONE_BLOCED;
            g_SocketList[i].nType = socket_type;
            break;
        }
    }
   // SOCK_INFO(LOG_STR_LEAVE(" postion = %d"), i);
    return i;
}

int32 java_FindSocketList(int32 s)
{
    int i = 0;
    //SOCK_INFO(LOG_STR_ENTER(" socket = %d"));
    for(i=0;i<MAX_SOCKET_NUM;i++)
    {
        if(g_SocketList[i].nSocket == s)
        {
            break;
        }
    }
    //SOCK_INFO(LOG_STR_LEAVE(" postion = %d"), i);
    return i;
}

int32 java_SocketListSetBlock(int32 s, int32 type)
{
    int i = 0;
    //SOCK_INFO(LOG_STR_ENTER(" socket = %d"));
    for(i=0;i<MAX_SOCKET_NUM;i++)
    {
        if(g_SocketList[i].nSocket == s)
        {
            g_SocketList[i].block=type;
            break;
        }
    }
    //SOCK_INFO(LOG_STR_LEAVE(" postion = %d"), i);
        return i;
}

int32 java_SocketListClearBlock(int32 s)
{
    int i = 0;
    //SOCK_INFO(LOG_STR_ENTER(" socket = %d"));
    for(i=0;i<MAX_SOCKET_NUM;i++)
    {
        if(g_SocketList[i].nSocket == s)
        {
            g_SocketList[i].block=NONE_BLOCED;
            break;
        }
    }
    //SOCK_INFO(LOG_STR_LEAVE(" postion = %d"), i);
        return i;
}

int32 java_DelSocketListBySocket(int32 s)
{
    int32 i = 0;
    //SOCK_INFO(LOG_STR_ENTER(" socket = %d"),s);
    for(i=0;i<MAX_SOCKET_NUM;i++)
	{
		if(g_SocketList[i].nSocket == s)
		{
            SCI_MEMSET(&g_SocketList[i], 0, sizeof(T_Socket));
			break;
		}
	}
	//SOCK_INFO(LOG_STR_LEAVE(" postion = %d"), i);
	return i;
}

int32 java_DelSocketListByIndex(int32 index)
{
    int32  s =  0;
    s =  g_SocketList[index].nSocket;
    SCI_MEMSET(&g_SocketList[index], 0, sizeof(T_Socket));
     return s;
}
int32 java_GetSocketTypeBySocket(int32 s)
{
    int32 i = 0;
    int32 type = 0;
     for(i=0;i<MAX_SOCKET_NUM;i++)
     {
         if(g_SocketList[i].nSocket == s)
         {
             type = g_SocketList[i].nType;
             break;
         }
     }
     return type;
}


/*
 * notify all java blocked java network operation to retry
 */

PUBLIC uint32 java_SocketListNotifyAll(void)
{
    int i = 0;
    javacall_handle socket_handle = 0;
    //SOCK_INFO(LOG_STR_ENTER(" socket = %d"));
    for(i=0;i<MAX_SOCKET_NUM;i++)
    {
        if(g_SocketList[i].block != NONE_BLOCED)
        {
            socket_handle = (javacall_handle)(g_SocketList[i].nSocket);
            SOCK_INFO(LOG_STR("notify %d for %d"), g_SocketList[i].nSocket, g_SocketList[i].block);
            switch (g_SocketList[i].block) {
                case DATAGRAM_RECVFROM_BLOCED:
                    javanotify_datagram_event(JAVACALL_EVENT_DATAGRAM_RECVFROM_COMPLETED, socket_handle,  JAVACALL_OK);
                    break;
                case DATAGRAM_SENDTO_BLOCED:
                    javanotify_datagram_event(JAVACALL_EVENT_DATAGRAM_SENDTO_COMPLETED, socket_handle,  JAVACALL_OK);
                    break;
                case SOCKET_OPEN_BLOCED:
                    javanotify_socket_event(JAVACALL_EVENT_SOCKET_OPEN_COMPLETED, socket_handle,  JAVACALL_OK);
                    break;
                case SOCKET_CLOSE_BLOCED:
                    javanotify_socket_event(JAVACALL_EVENT_SOCKET_CLOSE_COMPLETED, socket_handle,  JAVACALL_OK);
                    break;
                case SOCKET_READ_BLOCED:
                    javanotify_socket_event(JAVACALL_EVENT_SOCKET_RECEIVE, socket_handle,  JAVACALL_OK);
                    break;
                case SOCKET_WRITE_BLOCED:
                    javanotify_socket_event(JAVACALL_EVENT_SOCKET_SEND, socket_handle,  JAVACALL_OK);
                    break;
                case NETWORK_GETHOSTBYNAME_BLOCED:
                    javanotify_socket_event(JAVACALL_EVENT_NETWORK_GETHOSTBYNAME_COMPLETED, socket_handle,  JAVACALL_OK);
                    break;
                default:
                    break;
            }
            g_SocketList[i].block == NONE_BLOCED;
        }
    }
    //SOCK_INFO(LOG_STR_LEAVE(" postion = %d"), i);
    return i;
}

/**
 * Initiates the connection of a client socket.
 *
 * @param ipBytes IP address of the local device in the form of byte array
 * @param port number of the port to open
 * @param pHandle address of variable to receive the handle; this is set
 *        only when this function returns JAVACALL_WOULD_BLOCK or JAVACALL_OK.
 * @param pContext address of a pointer variable to receive the context;
 *        this is set only when the function returns JAVACALL_WOULDBLOCK.
 *
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if there was an IO error   
 * @retval JAVACALL_WOULD_BLOCK  if the caller must call the finish function to complete the operation
 * @retval JAVACALL_CONNECTION_NOT_FOUND when there was some other error (Connection not found exception case)
 */
javacall_result javacall_socket_open_start(unsigned char *ipBytes,int port,
                                           void **pHandle, void **pContext)
{
    long s;
    int falsebuf = 0;
    int status;
    struct sci_sockaddr addr;
    unsigned long nonBlockingFlag = 1;
    int lastError;
    int nPos;
    
    if(ipBytes == NULL || NULL==pContext || NULL==pHandle)
    {
        return JAVACALL_INVALID_ARGUMENT;
    }
    SOCK_INFO(LOG_STR_ENTER("ipBytes=%d.%d.%d.%d ,port=%d"), ipBytes[0], ipBytes[1], ipBytes[2], ipBytes[3], port);
    if ((java_NetStatus() == BAD) || (java_NetStatus() == REACTIVATING))
    {
        if (java_NetStatus() == BAD) {
            java_NetConnect();
        }
        SOCK_INFO(LOG_STR_LEAVE("fail, java_NetStatus down"));
        return JAVACALL_CONNECTION_NOT_FOUND;
    }  
    
    /* 创建指定网络接口的 socket */
    s = sci_sock_socket(AF_INET, SOCK_STREAM, 0, g_java_net_id);
    SOCK_INFO(LOG_STR("sci_sock_socket, socket =%d"), s);
    if (INVALID_SOCKET == s) 
    {	
        SOCK_INFO(LOG_STR_LEAVE("fail, sci_sock_socket with netid %d failed"), g_java_net_id);
        return JAVACALL_FAIL;
    }

    nPos = java_GetSocketList(s, port, SOCK_STREAM);
    if(MAX_SOCKET_NUM <= nPos )
    {
        sci_sock_socketclose(s);
        SOCK_INFO(LOG_STR_LEAVE("fail, on %d, too many sockets"), s);
        return JAVACALL_FAIL;
    }

    status = sci_sock_setsockopt(s,SO_REUSEADDR,(char*)&falsebuf);

    if (0 != status) 
    {
        sci_sock_socketclose(s); 
        java_DelSocketListBySocket(s);
        SOCK_INFO(LOG_STR_LEAVE("fail, on %d, sci_sock_socket(SO_REUSEADDR) return =%d"), s, status);
        return JAVACALL_FAIL;
    }
    
    status = sci_sock_setsockopt(s, SO_NBIO, &nonBlockingFlag);        
    if(status !=0) 
    {
        SOCK_INFO(LOG_STR("fail, on %d sci_sock_setsockopt(SO_NBIO), return = %d"), s, status);
        sci_sock_socketclose(s);
        java_DelSocketListBySocket(s);
        return JAVACALL_FAIL;
    }
    
    status  = sci_sock_asyncselect(s,  java_GetSigDispatchTaskID(), AS_CONNECT|AS_CLOSE|AS_READ|AS_WRITE);              
    if(0 != status)
    {         
        SOCK_DBG(LOG_STR_LEAVE("fail, on %d java_GetSigDispatchTaskID return = %d"), s, status);
        sci_sock_socketclose(s);
        java_DelSocketListBySocket(s);
        return JAVACALL_FAIL;
    }

    SCI_MEMSET(&addr, 0, sizeof(addr));
    addr.family      = AF_INET;
    addr.port        = htons((unsigned short)port);
    
    SCI_MEMCPY(&addr.ip_addr, ipBytes, sizeof(addr.ip_addr));
    SOCK_DBG(LOG_STR("before call sci_sock_connect time=%d"), javacall_time_get_seconds_since_1970());
    status = sci_sock_connect(s, (struct sci_sockaddr *)&addr, sizeof(addr));
    SOCK_DBG(LOG_STR("after call sci_sock_connect return %d, time=%d"), status,javacall_time_get_seconds_since_1970());

    *pHandle = (void*)s;
    *pContext = NULL;
    
    if (status == 0) 
	{         
        SOCK_DBG(LOG_STR_LEAVE("ok, on %d, port=%d"), s, port);
        return JAVACALL_OK;
    } /* end of connect() is OK */
	
    lastError = sci_sock_errno(s);
    SOCK_DBG(LOG_STR("sci_sock_errno, return  = %d"),lastError);
    if (EINPROGRESS == lastError)
    {                
        java_SocketListSetBlock(s, SOCKET_OPEN_BLOCED);
        SOCK_DBG(LOG_STR_LEAVE("block, on %d"), s);
		
        return JAVACALL_WOULD_BLOCK;
    }
    
    java_DelSocketListBySocket(s);
 	sci_sock_socketclose(s);
    SOCK_INFO(LOG_STR_LEAVE("fail, on %d, sci_sock_connect error=%d, netid=%d"), s, lastError, g_java_net_id);
    return JAVACALL_CONNECTION_NOT_FOUND;
}
    
/**
 * Finishes a pending open operation.
 *
 * @param handle the handle returned by the open_start function
 * @param context the context returned by the open_start function
 *
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if an error occurred  
 * @retval JAVACALL_WOULD_BLOCK  if the caller must call the finish function again to complete the operation
 */
 javacall_result javacall_socket_open_finish(void *handle, void *context)
 {
     long s;
     int nPos = 0 ;

     SOCK_INFO(LOG_STR_ENTER("hande = %d, context = %d"), handle, context);
     if(NULL==handle)
     {
        SOCK_INFO(LOG_STR_LEAVE("fail, null handle"), handle);
        return JAVACALL_INVALID_ARGUMENT;
     }
     s = (long)handle;    
     nPos = java_SocketListClearBlock(s);
     if( MAX_SOCKET_NUM <= nPos)
     {
         sci_sock_socketclose(s);
         SOCK_INFO(LOG_STR_LEAVE("fail, on %d, can not find"), handle);
         return JAVACALL_FAIL;
     }
     
     if (g_SocketList[nPos].blockResult != JAVACALL_OK) {
         SOCK_INFO(LOG_STR_LEAVE("fail, on %d, result=%d"), handle, g_SocketList[nPos].blockResult);
         return JAVACALL_FAIL;
     }
     SOCK_DBG(LOG_STR_LEAVE("ok, on %d"), handle);
     return JAVACALL_OK; 
}    
    
/**
 * Initiates a read from a platform-specific TCP socket.
 *  
 * @param handle handle of an open connection
 * @param pData base of buffer to receive read data
 * @param len number of bytes to attempt to read
 * @param pBytesRead returns the number of bytes actually read; it is
 *        set only when this function returns JAVACALL_OK
 * @param pContext address of pointer variable to receive the context;
 *        it is set only when this function returns JAVACALL_WOULDBLOCK
 * 
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if there was an error   
 * @retval JAVACALL_WOULD_BLOCK  if the operation would block
 * @retval JAVACALL_INTERRUPTED for an Interrupted IO Exception
 */
 javacall_result javacall_socket_read_start(void *handle,unsigned char *pData,int len, 
     int *pBytesRead, void **pContext)
 {
     javacall_result tStatus;
     int nBytesLeft;     
     javacall_int32 socket = (javacall_int32)handle;
    if(NULL==pBytesRead||NULL==handle||NULL==pData||NULL==pContext)
    {
        SOCK_INFO(LOG_STR_LEAVE("fail, on %d, null input parameter"), socket);
        return JAVACALL_INVALID_ARGUMENT;
    }

    if ((java_NetStatus() == BAD) || (java_NetStatus() == REACTIVATING))
    {
        SOCK_INFO(LOG_STR_LEAVE("fail, on %d, java_NetStatus down"), socket);
        return JAVACALL_CONNECTION_NOT_FOUND;
    }      
    
     tStatus = java_SocketRead(handle, pData, len, pBytesRead, &nBytesLeft);
     if( JAVACALL_OK == tStatus)
     {
         SOCK_DBG(LOG_STR_LEAVE("read %d of %d on %d, return =%d"), *pBytesRead, len, socket, tStatus);
         return tStatus;
     }
     if(JAVACALL_WOULD_BLOCK == tStatus )
     {
        java_SocketListSetBlock(socket, SOCKET_READ_BLOCED);
        SOCK_DBG(LOG_STR_LEAVE("block, read %d on %d"), len, handle);
        return JAVACALL_WOULD_BLOCK;
     }
         SOCK_INFO(LOG_STR_LEAVE("fail, read %d of %d on %d, return =%d"), *pBytesRead, len, socket, tStatus);
     return tStatus;
 }    
/**
 * Finishes a pending read operation.
 *
 * @param handle handle of an open connection
 * @param pData base of buffer to receive read data
 * @param len number of bytes to attempt to read
 * @param pBytesRead returns the number of bytes actually read; it is
 *        set only when this function returns JAVACALL_OK
 * @param context the context returned by read_start
 * 
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if there was an error   
 * @retval JAVACALL_WOULD_BLOCK  if the caller must call the finish function again to complete the operation
 * @retval JAVACALL_INTERRUPTED for an Interrupted IO Exception
 */
 javacall_result javacall_socket_read_finish(void *handle,unsigned char *pData,int len,int *pBytesRead,void *context)
 {
     
     javacall_result tStatus;
     int nBytesLeft;
     javacall_int32 socket = (javacall_int32)handle;
     if(NULL==pBytesRead||NULL==handle||NULL==pData)    //||NULL==context)
     {
        SOCK_INFO(LOG_STR_LEAVE("fail, on %d, null input parameter"), handle);
        return JAVACALL_INVALID_ARGUMENT;
     }
     
     tStatus = java_SocketRead(handle, pData, len, pBytesRead, &nBytesLeft);

     if( JAVACALL_OK == tStatus)
     {
        java_SocketListClearBlock(socket);
         SOCK_DBG(LOG_STR_LEAVE("ok, read %d of %d on %d, return =%d"), *pBytesRead, len, socket, tStatus);
         return tStatus;
     }
     if(JAVACALL_WOULD_BLOCK == tStatus )
     {
         SOCK_DBG(LOG_STR_LEAVE("block, read %d on %d"), len, socket);
         return JAVACALL_WOULD_BLOCK;
     }
     java_SocketListClearBlock(socket);
     SOCK_INFO(LOG_STR_LEAVE("fail, read %d of %d on %ld, return tStatus=%d"), *pBytesRead, len, socket, tStatus);
     return tStatus;
 }

/**
 * Initiates a write to a platform-specific TCP socket.
 *
 * @param handle handle of an open connection
 * @param pData base of buffer containing data to be written
 * @param len number of bytes to attempt to write
 * @param pBytesWritten returns the number of bytes written after
 *        successful write operation; only set if this function returns
 *        JAVACALL_OK
 * @param pContext address of a pointer variable to receive the context;
 *	  it is set only when this function returns JAVACALL_WOULDBLOCK
 *
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if there was an error   
 * @retval JAVACALL_WOULD_BLOCK  if the operation would block
 * @retval JAVACALL_INTERRUPTED for an Interrupted IO Exception
 */
javacall_result javacall_socket_write_start(void *handle,char *pData,int len,int *pBytesWritten,void **pContext)
{
    javacall_result tStatus;
    javacall_int32 socket = (javacall_int32)handle;
    if(NULL==pBytesWritten||NULL==handle||NULL==pData||NULL==pContext)
    {
        SOCK_INFO(LOG_STR_LEAVE("fail, on %d, null input parameter"), handle);
        return JAVACALL_INVALID_ARGUMENT;
    }

    if ((java_NetStatus() == BAD) || (java_NetStatus() == REACTIVATING))
    {
        SOCK_INFO(LOG_STR_LEAVE("fail, on %d, java_NetStatus down"), socket);
        return JAVACALL_CONNECTION_NOT_FOUND;
    } 	
    
    tStatus = java_SocketWrite(handle, pData, len, pBytesWritten);
    if( JAVACALL_OK == tStatus)
    {
        SOCK_DBG(LOG_STR_LEAVE("write %d of %d on %d, return =%d"), *pBytesWritten, len, socket, tStatus);
        return tStatus;
    }
     
	if(JAVACALL_WOULD_BLOCK == tStatus)
	{
        java_SocketListSetBlock(socket, SOCKET_WRITE_BLOCED);
        SOCK_DBG(LOG_STR_LEAVE("block, write %d on %d"), len, socket);
        return JAVACALL_WOULD_BLOCK;
	}
	SOCK_INFO(LOG_STR_LEAVE("fail, write %d of %d on %d, return tStatus=%d"), *pBytesWritten, len, socket, tStatus);
    return tStatus;
}    
/**
 * Finishes a pending write operation.
 *
 * @param handle handle of an open connection
 * @param pData base of buffer containing data to be written
 * @param len number of bytes to attempt to write
 * @param pBytesWritten returns the number of bytes written after
 *        successful write operation; only set if this function returns
 *        JAVACALL_OK
 * @param context the context returned by write_start
 *
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if there was an error   
 * @retval JAVACALL_WOULD_BLOCK  if the caller must call the finish function again to complete the operation
 * @retval JAVACALL_INTERRUPTED for an Interrupted IO Exception
 */
javacall_result javacall_socket_write_finish(void *handle,char *pData,int len,int *pBytesWritten,void *context)
{
    javacall_result tStatus;
    javacall_int32 socket = (javacall_int32)handle;
    if(NULL==pBytesWritten||NULL==handle||NULL==pData)
    {
        SOCK_INFO(LOG_STR_LEAVE("fail, on %d, null input parameter"), socket);
        return JAVACALL_INVALID_ARGUMENT;
    }

    tStatus = java_SocketWrite(handle, pData, len, pBytesWritten);
    if( JAVACALL_OK == tStatus)
    {
        java_SocketListClearBlock(socket);
        SOCK_DBG(LOG_STR_LEAVE("ok, write %d of %d on %d, return =%d"), *pBytesWritten, len, socket, tStatus);
       	return tStatus;
    }
    if(JAVACALL_WOULD_BLOCK == tStatus)
    {
        SOCK_DBG(LOG_STR_LEAVE("block, write %d on %d"), len, socket);
        return JAVACALL_WOULD_BLOCK;
    }
    java_SocketListClearBlock(socket);
    SOCK_INFO(LOG_STR_LEAVE("fail, write %d of %d on %d, return tStatus=%d"), *pBytesWritten, len, socket, tStatus);
    return tStatus;
}

/**
 * Initiates the closing of a platform-specific TCP socket.
 *
 * @param handle handle of an open connection
 * @param pContext address of a pointer variable to receive the context;
 *	  it is set only when this function returns JAVACALL_WOULDBLOCK
 *
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if there was an error
 * @retval JAVACALL_WOULD_BLOCK  if the operation would block 
 */
javacall_result javacall_socket_close_start(void *handle,void **pContext)
{
    
    long s;
    int status;
	int lastError;
    int nPos = 0;
	
	if(NULL==handle||NULL==pContext)
	{
        SOCK_INFO(LOG_STR_LEAVE("fail, on %d, null input parameter"), handle);
    	return JAVACALL_INVALID_ARGUMENT;
	}
	
	s = (long)handle;
    nPos = java_FindSocketList(s);
    SOCK_DBG(LOG_STR("java_FindSocketList, Position = %d"), nPos);
    if(MAX_SOCKET_NUM <= nPos )
    {
         return JAVACALL_FAIL;
    }

    status = sci_sock_socketclose(s);
    if (0 == status) 
    {	
    java_DelSocketListBySocket( s);
        SOCK_INFO(LOG_STR_LEAVE("ok, close %d"),s); 
        return JAVACALL_OK;
    }      
   
     lastError = sci_sock_errno(s);       
    if (lastError == EWOULDBLOCK) 
    {        
        SOCK_DBG(LOG_STR_LEAVE("block, close %d"), handle);
        return JAVACALL_WOULD_BLOCK;
    }
    java_DelSocketListBySocket(s);
    SOCK_INFO(LOG_STR_LEAVE("ok, close %d, result=%d"), handle, lastError);
    return JAVACALL_OK;
}

/**
 * Initiates the closing of a platform-specific TCP socket.
 *
 * @param handle handle of an open connection
 * @param context the context returned by close_start
 *
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if there was an error   
 * @retval JAVACALL_WOULD_BLOCK  if the caller must call the finish function again to complete the operation
 */
javacall_result javacall_socket_close_finish(void *handle,void *context)
{    
    if(NULL==handle)
    {    
        SOCK_INFO(LOG_STR_LEAVE("fail, on %d, null input parameter"), handle);
        return JAVACALL_INVALID_ARGUMENT;
    }	
    java_DelSocketListBySocket((javacall_int32)handle);
    SOCK_DBG(LOG_STR_LEAVE("ok, on %d"), handle);
    return JAVACALL_OK;
 }    

/**
 * Gets the number of bytes available to be read from the platform-specific
 * socket without causing the system to block. If it is not possible to find
 * out the actual number of available bytes then the resulting number is 0.
 * <b>IMPORTANT</b>: Some features of the product (not convered by spec but still essential,
 * for example java debugging) might not work if this function does not return
 * actual value.
 *
 * @param handle handle of an open connection
 * @param pBytesAvailable returns the number of available bytes
 *
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    if there was an error 
 */
javacall_result /* OPTIONAL*/ javacall_socket_available(javacall_handle handle,int *pBytesAvailable) {
    long s;
	int to_recv_len = 0;
	int status = 0;

	if(NULL==handle)
	{
		return JAVACALL_FAIL;
	}
	s = (long)handle;
	
	status = sci_sock_getsockopt(s,SO_RXDATA,&to_recv_len);

	if (0 == status)
	{
		*pBytesAvailable = to_recv_len;

		//SOCK_DBG(LOG_STR_LEAVE("OK %d bytes available"), *pBytesAvailable);	    
        return JAVACALL_OK;
	}
	else
	{
	    *pBytesAvailable = 0;
        SOCK_DBG(LOG_STR_LEAVE("fail, sock opt error, %d"), sci_sock_errno(s));
	    return JAVACALL_FAIL;
	}

}
    
/******************************************************************************
 ******************************************************************************
 ******************************************************************************
    OPTIONAL FUNCTIONS
 ******************************************************************************
 ******************************************************************************
 ******************************************************************************/
    
/**
 * Shuts down the output side of a platform-specific TCP socket.
 * Further writes to this socket are disallowed.
 *
 * Note: a function to shut down the input side of a socket is
 * explicitly not provided.
 *
 * @param handle handle of an open connection
 *
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    if there was an error
 */
javacall_result /*OPTIONAL*/ javacall_socket_shutdown_output(javacall_handle handle)
{
    long s;
    int status;
	int lastError;
	javacall_int32  return_error = 0;
	if(NULL==handle)
	{
		return JAVACALL_FAIL;
	}
	s = (long)handle;
	
    SOCK_DBG(LOG_STR_ENTER("socket = %x"), s);
	//ZTE_Trace(PRN_JAVA,PRN_NORMAL,"network javacall_socket_shutdown_output begin");
	
    status = sci_sock_shutdown(s, 1);
    if(0 == status)
    {
	return JAVACALL_OK;
    }
    lastError = sci_sock_errno(s);    
    SOCK_DBG(LOG_STR("socket = %d, error = %d"), s, lastError);
    switch (lastError)
    {
    case EWOULDBLOCK:            
        return_error = JAVACALL_WOULD_BLOCK;
        break;
        
    case ENOTSOCK:            
        return_error =  JAVACALL_INTERRUPTED;
        break;
        
    case ENETUNREACH:            
        return_error =  JAVACALL_CONNECTION_NOT_FOUND;
        break;     
        
    default:
        return_error =  JAVACALL_FAIL;
        break;  
    }
    
    return return_error ;
}

javacall_result /*OPTIONAL*/ javacall_server_socket_open_start(
	int port, 
	void **pHandle, 
	void **pContext) 
{
    return JAVACALL_FAIL;
}

/**
 * See pcsl_network.h for definition.
 */
javacall_result javacall_server_socket_open_finish(void *handle,void *context){

    	return JAVACALL_FAIL;
}


/**
 * See javacall_socket.h for definition.
 */
javacall_result /*OPTIONAL*/ javacall_server_socket_accept_start(
      javacall_handle handle, 
      javacall_handle *pNewhandle) {
   
     	return JAVACALL_FAIL;
}

/**
 * See javacall_socket.h for definition.
 */
javacall_result /*OPTIONAL*/ javacall_server_socket_accept_finish(
	javacall_handle handle, 
	javacall_handle *pNewhandle) {

    return JAVACALL_FAIL;
}

#ifdef __cplusplus
}
#endif


