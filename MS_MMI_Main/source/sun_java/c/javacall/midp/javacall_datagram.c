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

#include "tasks_id.h"
#include "javacall_datagram.h" 
#include "java_socket.h"
#include "os_api.h" //sci_trace_low, sci_alloca
#include "app_tcp_if.h" //sci_sock_???
#include "javacall_time.h" //javacall_callback_func()
#include "javacall_socket.h" //javanotify_socket_event()
#include "javacall_network.h" //javanotify_network_event()
#include "javacall_logging.h"
#include "javacall_lifecycle.h"
#include "java_sigdispatchtask.h"

T_Socket g_SocketList[MAX_SOCKET_NUM];


uint32  g_java_net_id = 0;  /*lint !e18*/   //pdp并发记录java模块的 net id    


/**
 * Opens a datagram socket
 *
 * @param port The local port to attach to
 * @param pHandle address of variable to receive the handle; this is set
 *        only when this function returns JAVACALL_OK.
 *
 * @return JAVACALL_OK if the function completes successfully
 *         JAVACALL_FAIL if there was an IO error and IOException needs to be thrown;
 */
javacall_result javacall_datagram_open(
    int port,
    javacall_handle *pHandle) {
    long socket;
    struct sci_sockaddr  tSOCKAdd;
    int nPos;
    int nRet;
    char cNonBlock = 1;
    int lastError;

    /*检查入参*/
    if ((NULL == pHandle) || (port <0))
    {        
        SOCK_INFO(LOG_STR_LEAVE("fail, null handle or port %d < 0"), port);
        return JAVACALL_INVALID_ARGUMENT;
    }

    
    if ((java_NetStatus() == BAD) || (java_NetStatus() == REACTIVATING))
    {
        SOCK_INFO(LOG_STR_LEAVE("fail, java_NetStatus down"));
        return JAVACALL_CONNECTION_NOT_FOUND;
    } 

    /*如果没有指定端口号，则由平台分配一个可用的端口*/
    
    /*创建一个指定网络接口的 socket*/
    socket = sci_sock_socket(AF_INET,SOCK_DGRAM,0,g_java_net_id);
    SOCK_INFO(LOG_STR("sci_sock_socket socket = %d"),socket );
    if(socket == -1)
    {
        /*test start*/        
        lastError = sci_sock_errno(socket);
        SOCK_INFO("fail, sci_sock_socket with netid %d failed, error=%d", g_java_net_id, lastError);
        /*test end*/
        return JAVACALL_FAIL;
    }
       
    /*设置非阻塞*/
    nRet = sci_sock_setsockopt(socket,SO_NBIO,&cNonBlock);
    if(nRet < 0)
    {        
        lastError = sci_sock_errno(socket);
        sci_sock_socketclose(socket);
        SOCK_INFO(LOG_STR_LEAVE("fail, on %d, sci_sock_socket(SO_NBIO) error=%d, netid=%d"), socket, lastError, g_java_net_id);
        return JAVACALL_FAIL;
    }
    /*保存socket*/
    nPos = java_GetSocketList(socket, port, SOCK_DGRAM);
    if(MAX_SOCKET_NUM <= nPos)
    {
        sci_sock_socketclose(socket);
        SOCK_INFO(LOG_STR_LEAVE("fail, on %d, too many sockets"), socket);
        return JAVACALL_FAIL;
    }

      nRet  = sci_sock_asyncselect(socket,  java_GetSigDispatchTaskID(), AS_READ|AS_WRITE);              
    if(0 != nRet)
    {         
        SOCK_DBG(LOG_STR_LEAVE("fail to create new message handle"));
        sci_sock_socketclose(socket);
        java_DelSocketListBySocket(socket);
        return JAVACALL_FAIL;
    }
    
	SCI_MEMSET(&tSOCKAdd,0,sizeof(struct sci_sockaddr));
    tSOCKAdd.family = AF_INET;
	tSOCKAdd.port = htons(port);
	nRet = sci_sock_bind((long)socket,(struct sci_sockaddr*)&tSOCKAdd,sizeof(tSOCKAdd));
    if(nRet != 0)
    {
        lastError = sci_sock_errno(socket);
        sci_sock_socketclose(socket);
        java_DelSocketListBySocket(socket);
        SOCK_INFO(LOG_STR_LEAVE("fail, on %d, sci_sock_bind error=%d, netid=%d"), socket, lastError, g_java_net_id);
        return JAVACALL_FAIL;
    }
    *pHandle = (void *)socket;
    SOCK_DBG(LOG_STR_LEAVE("ok, handle=%d, port=%d"), socket, port);
    return JAVACALL_OK;    
}

/**
 * Initiates a read from a platform-specific datagram.
 *  
 * <p>
 * <b>NOTE:</b> The parameter <tt>buffer</tt> must be pre-allocated
 * prior to calling this function.
 *
 * @param handle handle of an open connection
 * @param pAddress base of byte array to receive the address
 * @param port pointer to the port number of the remote location
 *             that sent the datagram. <tt>port</tt> is set by
 *             this function.
 * @param buffer data received from the remote location. The contents
 *               of <tt>buffer</tt> are set by this function.
 * @param length the length of the buffer
 * @param pBytesRead returns the number of bytes actually read; it is
 *        set only when this function returns JAVACALL_OK
 * @param pContext address of pointer variable to receive the context;
 *        it is set only when this function returns JAVACALL_WOULD_BLOCK
 *
 * @return JAVACALL_OK for successful read operation
 *         JAVACALL_WOULD_BLOCK if the operation would block
 *         JAVACALL_INTERRUPTED for an Interrupted IO Exception
 *         JAVACALL_FAIL for all other errors
 */
int javacall_datagram_recvfrom_start(
    javacall_handle handle,
    unsigned char *pAddress,
    int *port, 
    char *buffer,
    int length,
    int *pBytesRead,
    void **pContext)
{
    struct sci_sockaddr tSCIAdd;
    int nBytesLeft = 0;
    int nRet = 0;
    
     if ((java_NetStatus() == BAD) || (java_NetStatus() == REACTIVATING))
    {
        SOCK_INFO(LOG_STR_LEAVE("fail, on %d, java_NetStatus down"), handle);
        return JAVACALL_CONNECTION_NOT_FOUND;
    } 
    
	SCI_MEMSET(&tSCIAdd,0,sizeof(struct sci_sockaddr));
    *pBytesRead = sci_sock_asyncrecvfrom((int32)handle, buffer, length, 
        0,(struct sci_sockaddr *)&tSCIAdd, &nBytesLeft);    
    SOCK_INFO(LOG_STR("socket = %d, pBytesRead = %d"),(int32)handle,*pBytesRead);
    if (*pBytesRead > 0) //case SOCKET_SUCCESS:
    {
        tSCIAdd.ip_addr=ntohl(tSCIAdd.ip_addr);
        *port = ntohs(tSCIAdd.port);
        SCI_MEMCPY(pAddress,&(tSCIAdd.ip_addr),sizeof(DWORD));        
        SOCK_DBG(LOG_STR_LEAVE("ok, recv %d on %d"), *pBytesRead, handle);
        return JAVACALL_OK;
    }
    nRet = sci_sock_errno((int32)handle);    
    SOCK_INFO(LOG_STR("sci_sock_errno = %d"), nRet);
    switch(nRet)
    {
    case EWOULDBLOCK:   
        java_SocketListSetBlock((int32)handle, DATAGRAM_RECVFROM_BLOCED);
        SOCK_DBG(LOG_STR_LEAVE("block, on %d"), handle);
        return JAVACALL_WOULD_BLOCK;
        
    case EIEIO:
        SOCK_INFO(LOG_STR_LEAVE("fail, io interrupted on %d"), handle);
        return JAVACALL_INTERRUPTED;
        
    default:
        SOCK_INFO(LOG_STR_LEAVE("fail, ret=%d, handle=%d"), nRet, handle);
        return JAVACALL_FAIL;
    }
}
/**
 * Finishes a pending read operation.
 *
 * @param handle handle of an open connection
 * @param pAddress base of byte array to receive the address
 * @param port pointer to the port number of the remote location
 *             that sent the datagram. <tt>port</tt> is set by
 *             this function.
 * @param buffer data received from the remote location. The contents
 *               of <tt>buffer</tt> are set by this function.
 * @param length the length of the buffer
 * @param pBytesRead returns the number of bytes actually read; it is
 *        set only when this function returns JAVACALL_OK
 * @param context the context returned by javacall_datagram_recvfrom_start
 *
 * @return JAVACALL_OK for successful read operation;
 *         JAVACALL_WOULD_BLOCK if the caller must call the finish function again to complete the operation;
 *         JAVACALL_INTERRUPTED for an Interrupted IO Exception
 *         JAVACALL_FAIL for all other errors
 */
int javacall_datagram_recvfrom_finish(
      javacall_handle handle,
    unsigned char *pAddress,
    int *port, 
    char *buffer,
    int length,
    int *pBytesRead,
    void *context)
{
    struct sci_sockaddr tSCIAdd;
    int nRet;
     int nBytesLeft = 0;
     
	SCI_MEMSET(&tSCIAdd,0,sizeof(struct sci_sockaddr));
    *pBytesRead = sci_sock_asyncrecvfrom ((int32)handle, buffer,length, 0,
        (struct sci_sockaddr *)&tSCIAdd, &nBytesLeft);
    SOCK_INFO(LOG_STR("socket = %d, pBytesRead = %d"),(int32)handle,*pBytesRead);
    if (*pBytesRead > 0)
    { 
        tSCIAdd.ip_addr=ntohl(tSCIAdd.ip_addr);
        *port = ntohs(tSCIAdd.port);
        SCI_MEMCPY(pAddress,&(tSCIAdd.ip_addr),sizeof(DWORD));     
        SOCK_DBG(LOG_STR_LEAVE("ok, recv %d on %d"), *pBytesRead, handle);
        return JAVACALL_OK;
    }

    nRet = sci_sock_errno((int32)handle);
    SOCK_INFO(LOG_STR("sci_sock_errno = %d"), nRet);
    switch(nRet)
    {
    case EWOULDBLOCK:
        SOCK_DBG(LOG_STR_LEAVE("block, on %d"), handle);
        return JAVACALL_WOULD_BLOCK;
        
    case EIEIO:
        java_SocketListClearBlock((int32)handle);
        SOCK_INFO(LOG_STR_LEAVE("fail, io interrupted on %d"), handle);
        return JAVACALL_INTERRUPTED;
        
    default:
        java_SocketListClearBlock((int32)handle);
        SOCK_INFO(LOG_STR_LEAVE("fail, ret=%d, handle=%d"), nRet, handle);
        return JAVACALL_FAIL;
    }
}

/**
 * Initiates a write to a platform-specific datagram
 *
 * @param handle handle of an open connection
 * @param pAddress base of byte array to receive the address
 * @param port port number of the remote location to send the datagram
 * @param buffer data to send to the remote location
 * @param length amount of data, in bytes, to send to the remote
 *               location
 * @param pBytesWritten returns the number of bytes written after
 *        successful write operation; only set if this function returns
 *        JAVACALL_OK
 * @param pContext address of a pointer variable to receive the context;
 *      it is set only when this function returns JAVACALL_WOULD_BLOCK
 *
 * @return JAVACALL_OK for successful write operation;
 *         JAVACALL_WOULD_BLOCK if the operation would block,
 *         JAVACALL_INTERRUPTED for an Interrupted IO Exception
 *         JAVACALL_FAIL for all other errors
 */
int javacall_datagram_sendto_start(
    javacall_handle handle,
    unsigned char *pAddress,
    int port,
    char *buffer,
    int length,
    int *pBytesWritten,
    void **pContext)
{
    struct sci_sockaddr tSCIAdd;
    int nRet;

    if ((java_NetStatus() == BAD) || (java_NetStatus() == REACTIVATING))
    {
        SOCK_INFO(LOG_STR_LEAVE("fail, on %d, java_NetStatus down"), handle);
        return JAVACALL_CONNECTION_NOT_FOUND;
    } 

    SCI_MEMSET(&tSCIAdd,'\0',sizeof(struct sci_sockaddr));
    tSCIAdd.family= AF_INET;
	tSCIAdd.port = htons(port);	
	SCI_MEMCPY(&tSCIAdd.ip_addr, pAddress, sizeof(DWORD));
	
  	*pBytesWritten = sci_sock_sendto ((int32)handle, buffer,length, 0,(struct sci_sockaddr *)(&tSCIAdd));
     SOCK_INFO(LOG_STR("socket = %d ,bytes send =  %d"), (int32)handle, *pBytesWritten);
    if (*pBytesWritten > 0)
    {   
        SOCK_DBG(LOG_STR_LEAVE("ok, send %d on %d"), *pBytesWritten, handle);
        return JAVACALL_OK;
    }
    
    nRet = sci_sock_errno((int32)handle);
    SOCK_INFO(LOG_STR("sci_sock_errno, nRet=%d"), nRet);
    switch(nRet)
    {
    case EWOULDBLOCK:
        java_SocketListSetBlock((int32)handle, DATAGRAM_SENDTO_BLOCED);
        SOCK_DBG(LOG_STR_LEAVE("block, on %d"), handle);
        return JAVACALL_WOULD_BLOCK;
        
    case EIEIO:
        SOCK_INFO(LOG_STR_LEAVE("fail, io interrupted on %d"), handle);
        return JAVACALL_INTERRUPTED;
    case EMSGSIZE:
        SOCK_INFO(LOG_STR_LEAVE("fail, size=%d too large on %d"), length, handle);
        return JAVACALL_FAIL;
    default:
        SOCK_INFO(LOG_STR_LEAVE("fail, ret=%d, handle=%d"), nRet, handle);
        return JAVACALL_FAIL;
    }
}
/**
 * Finishes a pending write operation.
 *
 * @param handle handle of an open connection
 * @param pAddress base of byte array to receive the address
 * @param port port number of the remote location to send the datagram
 * @param buffer data to send to the remote location
 * @param length amount of data, in bytes, to send to the remote
 *               location
 * @param pBytesWritten returns the number of bytes written after
 *        successful write operation; only set if this function returns
 *        JAVACALL_OK
 * @param context the context returned by javacall_datagram_sendto_start
 *
 * @return JAVACALL_OK for successful write operation;
 *         JAVACALL_WOULD_BLOCK if the caller must call the finish function again to complete the operation;
 *         JAVACALL_INTERRUPTED for an Interrupted IO Exception
 *         JAVACALL_FAIL for all other errors
 */
int javacall_datagram_sendto_finish(
      javacall_handle handle,
    unsigned char *pAddress,
    int port,
    char *buffer,
    int length,
    int *pBytesWritten,
    void *context)
{
   	struct sci_sockaddr tSCIAdd;
    int nRet;

	SCI_MEMSET(&tSCIAdd,'\0',sizeof(struct sci_sockaddr));
	tSCIAdd.family= AF_INET;
   	tSCIAdd.port = htons(port);
	SCI_MEMCPY(&tSCIAdd.ip_addr, pAddress, sizeof(DWORD));

	*pBytesWritten = sci_sock_sendto ((int32)handle, buffer, length, 0,(struct sci_sockaddr *)(&tSCIAdd));
    SOCK_INFO(LOG_STR("socket = %d ,bytes send =  %d"), (int32)handle, *pBytesWritten);
    if (*pBytesWritten > 0)
    {        
        SOCK_DBG(LOG_STR_LEAVE("ok, send %d on %d"), *pBytesWritten, handle);
        return JAVACALL_OK;
    }

    nRet = sci_sock_errno((int32)handle);
    SOCK_INFO(LOG_STR("sci_sock_errno, error = %d"), nRet);
    switch(nRet)
    {
    case EWOULDBLOCK:
        SOCK_DBG(LOG_STR_LEAVE("block, on %d"), handle);
        return JAVACALL_WOULD_BLOCK;
        
    case EIEIO:           
        java_SocketListClearBlock((int32)handle);
        SOCK_INFO(LOG_STR_LEAVE("fail, io interrupted on %d"), handle);
        return JAVACALL_INTERRUPTED;
        
    default:       
        java_SocketListClearBlock((int32)handle);
        SOCK_INFO(LOG_STR_LEAVE("fail, ret=%d, handle=%d"), nRet, handle);
        return JAVACALL_FAIL;
    }
}

/**
 * Initiates the closing of a platform-specific datagram socket.
 *
 * @param handle handle of an open connection
 *
 * @return JAVACALL_OK upon success
 *         JAVACALL_FAIL for an error
 */
int javacall_datagram_close(javacall_handle handle)
{
    int nRet;
    long nPos;
    nPos = java_DelSocketListBySocket((int32)handle);
    SOCK_INFO(LOG_STR_ENTER("g_SocketList positon = %d"), nPos);
    nRet = sci_sock_socketclose((int32)handle);   
    if(0 == nRet)
    {
        SOCK_DBG(LOG_STR_LEAVE("ok, close %d"), handle);
		return JAVACALL_OK;
    }
    else
    {
        nRet = sci_sock_errno((int32)handle);
        SOCK_DBG(LOG_STR("sci_sock_errno, error = %d"), nRet);
        SOCK_INFO(LOG_STR_LEAVE("fail, close %d"), handle);
        return JAVACALL_FAIL;
    }
}

