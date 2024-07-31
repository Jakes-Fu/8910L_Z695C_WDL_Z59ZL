/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/

#include "opcfg.h"
#include "opdevinternal.h"

#include "opimport.h"


/** Host resolve will block 7 seconds and may try several times */
#define HOST_RESOLVE_TIMEOUT    (7*5*1000)

#define RET_ON_SOCKET_ERR(errCode) case errCode: return (#errCode);
#define RET_ON_UNKNOW_ERR()        default: return "unknow error";
static char* OD_GetSocketErrMsg(int err)
{
    switch(err)
    {
        RET_ON_SOCKET_ERR(ENOBUFS);
        RET_ON_SOCKET_ERR(ETIMEDOUT);
        RET_ON_SOCKET_ERR(EISCONN);
        RET_ON_SOCKET_ERR(EOPNOTSUPP);
        RET_ON_SOCKET_ERR(ECONNABORTED);
        RET_ON_SOCKET_ERR(EWOULDBLOCK);
        RET_ON_SOCKET_ERR(ECONNREFUSED);
        RET_ON_SOCKET_ERR(ECONNRESET);
        RET_ON_SOCKET_ERR(ENOTCONN);
        RET_ON_SOCKET_ERR(EALREADY);
        RET_ON_SOCKET_ERR(EINVAL);
        RET_ON_SOCKET_ERR(EMSGSIZE);
        RET_ON_SOCKET_ERR(EPIPE);
        RET_ON_SOCKET_ERR(EDESTADDRREQ);
        RET_ON_SOCKET_ERR(ESHUTDOWN);
        RET_ON_SOCKET_ERR(ENOPROTOOPT);
        RET_ON_SOCKET_ERR(EHAVEOOB);
        RET_ON_SOCKET_ERR(ENOMEM);
        RET_ON_SOCKET_ERR(EADDRNOTAVAIL);
        RET_ON_SOCKET_ERR(EADDRINUSE);
        RET_ON_SOCKET_ERR(EAFNOSUPPORT);
        RET_ON_SOCKET_ERR(EINPROGRESS);
        RET_ON_SOCKET_ERR(ELOWER);
        RET_ON_SOCKET_ERR(ENOTSOCK);
        RET_ON_SOCKET_ERR(EIEIO);
        RET_ON_SOCKET_ERR(ETOOMANYREFS);
        RET_ON_SOCKET_ERR(EFAULT);
        RET_ON_SOCKET_ERR(ENETUNREACH);
        RET_ON_UNKNOW_ERR();
    }
}

static OD_STATUS _OD_TranslateSocketErr(int err)
{
	switch(err)
	{
	case 0:        
		return OD_SUCCESS;
	case ETIMEDOUT:
	case EISCONN:
	case EOPNOTSUPP:
	case ECONNABORTED:
		return OD_ERROR;
	case EWOULDBLOCK:
	case ENOBUFS:
	case EINPROGRESS:
		return OD_SOCKET_WOULDBLOCK;
	case ECONNREFUSED:
		return OD_ERROR;
	case ECONNRESET:  
		return OD_SOCKET_CONNRESET;
	case ENOTCONN:    
		return OD_ERROR; 
	case EALREADY: 
		return OD_SOCKET_ALREADY;
	case EINVAL:
	case EMSGSIZE:
	case EPIPE:
	case EDESTADDRREQ:
	case ESHUTDOWN:
	case ENOPROTOOPT:
	case EHAVEOOB:
		return OD_ERROR;
	case ENOMEM:    
		return OD_NO_MEMORY;
	case EADDRNOTAVAIL:
	case EADDRINUSE:
	case EAFNOSUPPORT:
		return OD_ERROR;
	case ELOWER:
		return OD_ERROR;
	case ENOTSOCK:
		return OD_SOCKET_INVALID_SOCKET;
	case EIEIO:
		return OD_ERROR;
	case ETOOMANYREFS:
	case EFAULT: 
	case ENETUNREACH:
		return OD_ERROR;
	}

    return OD_ERROR;
}

OD_STATUS OD_HostResolve(OD_Context *context, const char* url, OD_HostResolveHandle* hostHandle)
{
	OD_STATUS		status = OD_ERROR;
	TCPIP_NETID_T	netID = OD_NetGetID(context); //Current activate NetID
	TCPIP_HOST_HANDLE handle = 0;
	
	OD_RETURNIFNOCTX(context);
	OD_CHECK_ARGS_RETURN_STATUS(IsValidContext(context) && "Invalid OD_Context");
	OD_CHECK_ARGS_RETURN_STATUS(url && "url should not be null");
	OD_CHECK_ARGS_RETURN_STATUS(hostHandle && "HostHandle should not be null");

    handle = sci_async_gethostbyname((char*)url,
                                     (BLOCK_ID)OD_GetMMITaskID(context),
                                     HOST_RESOLVE_TIMEOUT,
                                     netID);

    OD_TRACE_LOW("net: OD_HostResolve, url = %s, handle = 0x%x", url, handle);

    if(handle)
    {
        status = OD_SUCCESS;
        *hostHandle = handle;
    }

    return status;
}

OD_STATUS OD_SocketCreate(OD_Context *context, OD_Socket * pSocket)
{
    TCPIP_SOCKET_T socket_id = TCPIP_SOCKET_INVALID;
    TCPIP_NETID_T netID = OD_NetGetID(context); //Current activate NetID
    BLOCK_ID taskID = (BLOCK_ID)OD_GetMMITaskID(context); //MMI task ID; ?
    OD_STATUS status = OD_ERROR;

	OD_RETURNIFNOCTX(context);
	OD_CHECK_ARGS_RETURN_STATUS(IsValidContext(context) && "Invalid OD_Context");
	OD_CHECK_ARGS_RETURN_STATUS(pSocket && "pSocket should not be null");
	OD_CHECK_ARGS_RETURN_STATUS((taskID) && ("Please call OD_InitMMITaskContext() in the MMI task first"));

    socket_id = sci_sock_socket(AF_INET, SOCK_STREAM, 0, netID);
    if(socket_id != TCPIP_SOCKET_INVALID)
    {
        /** The sci_sock_asyncselect will automatically set the socket in no-block mode */
        int err = sci_sock_asyncselect(socket_id, taskID, AS_READ|AS_WRITE|AS_CONNECT|AS_CLOSE);
        if( TCPIP_SOCKET_ERROR == err )
        {
            err = sci_sock_errno(socket_id);
            OD_TRACE_LOW("net: OD_SocketCreate err = %s", OD_GetSocketErrMsg(err));
            sci_sock_socketclose(socket_id);
        }else
        {
            *pSocket = (OD_Socket)socket_id;
            status = OD_SUCCESS;
        }
    }

    OD_TRACE_LOW("net: OD_SocketCreate, netID = %d, socket_id = %d, status = %d",
        netID, socket_id, status);


    return status;
}

OD_STATUS OD_SocketConnect(OD_Context *context, OD_Socket socket, const OD_SocketAddress* pSocketAddress)
{
    TCPIP_SOCKET_T socket_id = (TCPIP_SOCKET_T)socket;
    struct sci_sockaddr addr;
    OD_STATUS status = OD_SUCCESS;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(IsValidContext(context) && "Invalid OD_Context");
	OD_CHECK_ARGS_RETURN_STATUS((OD_INVALID_SOCKET != socket) && "invalid socket");
	OD_CHECK_ARGS_RETURN_STATUS(pSocketAddress && "pSocketAddress should not be null");

    addr.family = AF_INET;
    addr.port = htons(pSocketAddress->port);
    addr.ip_addr = htonl(pSocketAddress->ip);

    if(TCPIP_SOCKET_ERROR == sci_sock_connect(socket_id, &addr, sizeof(addr)))
    {
        int err = sci_sock_errno(socket_id);
        OD_TRACE_LOW("net: OD_SocketConnect err = %s", OD_GetSocketErrMsg(err));
        status = _OD_TranslateSocketErr(err);
    }

    OD_TRACE_LOW("net: OD_SocketConnect, socket_id = %d, ip = %d.%d.%d.%d, port = %d, status = %d",
        socket_id, 
        (pSocketAddress->ip >> 24) & 0xff,
        (pSocketAddress->ip >> 16) & 0xff,
        (pSocketAddress->ip >> 8) & 0xff,
        (pSocketAddress->ip) & 0xff,
        pSocketAddress->port, status);

    return status;
}

OD_STATUS OD_SocketClose(OD_Context *context, OD_Socket socket)
{
	TCPIP_SOCKET_T socket_id = (TCPIP_SOCKET_T)socket;
	OD_STATUS status;
	BLOCK_ID taskID;

	OD_CHECK_ARGS_RETURN_STATUS(context && "Invalid OD_Context");
	OD_CHECK_ARGS_RETURN_STATUS((OD_INVALID_SOCKET != socket) && ("invalid socket"));

	status = OD_SUCCESS;
	taskID = (BLOCK_ID)OD_GetMMITaskID(context);
	if (sci_sock_asyncselect(socket_id, taskID, AS_NULL) == TCPIP_SOCKET_ERROR)
	{
		status = OD_ERROR;
	}
	if (sci_sock_shutdown(socket_id, SD_BOTH) == TCPIP_SOCKET_ERROR)
	{
		status = OD_ERROR;
	}
	if (sci_sock_socketclose(socket) == TCPIP_SOCKET_ERROR)
	{
		status = OD_ERROR;
	}

	OD_TRACE_LOW("net: OD_SocketClose, socket_id = %d, status = %d",
		socket, status);

	return status;
}

OD_STATUS OD_SocketSend(OD_Context *context, OD_Socket socket, const void* buffer, int len, int* bytes_written)
{
    TCPIP_SOCKET_T socket_id = (TCPIP_SOCKET_T)socket;
    OD_STATUS status = OD_SUCCESS;
    int sent = 0;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(IsValidContext(context) && "Invalid OD_Context");
	OD_CHECK_ARGS_RETURN_STATUS((OD_INVALID_SOCKET != socket) && "invalid socket");
	OD_CHECK_ARGS_RETURN_STATUS(buffer && "sent buffer should not be null");
	OD_CHECK_ARGS_RETURN_STATUS(len>0 && "sent buffer len should greater than zero");
	OD_CHECK_ARGS_RETURN_STATUS(bytes_written && "bytes_writen should not be null");

    sent = sci_sock_send(socket_id, (char*)buffer, len, 0);

    if(sent == TCPIP_SOCKET_ERROR)
    {
        int error = sci_sock_errno(socket_id);
        status = _OD_TranslateSocketErr(error);
        sent = 0;
    }else if(0 == sent)
	{
		OD_TRACE_LOW("Socket %d already closed\n", socket_id);
		status = OD_SOCKET_CLOSED;
	}

    if(bytes_written) *bytes_written = sent;

    OD_TRACE_LOW("net: OD_SocketSend, socket_id = %d, len = %d, sent = %d, status = %d",
        socket, len, sent, status);

    return status;
}

OD_STATUS OD_SocketReceive(OD_Context *context, OD_Socket socket, void* buffer, int len, int* bytes_received, int* bytes_left)
{
    TCPIP_SOCKET_T socket_id = (TCPIP_SOCKET_T)socket;
    OD_STATUS status = OD_SUCCESS;
    int received = 0;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(IsValidContext(context) && "Invalid OD_Context");
	OD_CHECK_ARGS_RETURN_STATUS((OD_INVALID_SOCKET != socket) && "invalid socket");
	OD_CHECK_ARGS_RETURN_STATUS(buffer && "receive buffer should not be null");
	OD_CHECK_ARGS_RETURN_STATUS(bytes_received && "bytes_received should not be null");
	OD_CHECK_ARGS_RETURN_STATUS(bytes_left && "bytes_left should not be null");

    received = sci_sock_asyncrecv(socket_id, (char*)buffer, len, 0, bytes_left);

    if(TCPIP_SOCKET_ERROR == received)
    {
        int err = sci_sock_errno(socket);
        status = _OD_TranslateSocketErr(err);
        OD_TRACE_LOW("%s", OD_GetSocketErrMsg(err));
        received = 0;
    } else if(0 == received)
    {
        status = OD_SOCKET_CLOSED;
        OD_TRACE_LOW("Socket %d already closed\n", socket_id);
    }
	
    if(bytes_received) *bytes_received = received;

    OD_TRACE_LOW("net: OD_SocketReceive, socket_id = %d, len = %d, received = %d, bytes_left = %d, status = %d",
        socket, len, received, *bytes_left, status);

    return status;
}

static OD_BOOL OD_SocketEventDispatcher(OD_Context* context, OD_SYS_Event* sysEvt)
{
	int err = 0;
	OD_BOOL isSocketEvt = OD_TRUE;
    OD_STATUS status = OD_SUCCESS;
    xSignalHeaderRec* evt = (xSignalHeaderRec*)sysEvt;
    OD_SocketEvent socketEvt;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(IsValidContext(context) && "Invalid OD_Context");
	OD_CHECK_ARGS_RETURN_STATUS(sysEvt && "sysEvt should not be null");

    switch (evt->SignalCode)
    {
        case SOCKET_CONNECT_EVENT_IND:
        {
            socketEvt.type = OD_SOCKET_EVENT_CONNECT;
            socketEvt.data.socket = ((SOCKET_CONNECT_EVENT_IND_SIG_T*)evt)->socket_id;
            socketEvt.result = _OD_TranslateSocketErr(((SOCKET_CONNECT_EVENT_IND_SIG_T*)evt)->error_code);
            break;
        }
        case SOCKET_CONNECTION_CLOSE_EVENT_IND:
        {
            socketEvt.type = OD_SOCKET_EVENT_CLOSE;
            socketEvt.data.socket = ((SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T*)evt)->socket_id;
            socketEvt.result = _OD_TranslateSocketErr(((SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T*)evt)->error_code);
            break;
        }
        case SOCKET_READ_EVENT_IND:
        {
            socketEvt.type = OD_SOCKET_EVENT_READ;
            socketEvt.data.socket = ((SOCKET_READ_EVENT_IND_SIG_T*)evt)->socket_id;
            socketEvt.result = OD_SUCCESS;
            break;
        }
        case SOCKET_READ_BUFFER_STATUS_EVENT_IND:
        {
            socketEvt.type = OD_SOCKET_EVENT_READ_BUF_HALF_FULL;
            socketEvt.data.socket = ((SOCKET_READ_BUFFER_STATUS_EVENT_IND_SIG_T*)evt)->socket_id;
            socketEvt.result = OD_SUCCESS;
            break;
        }
        case SOCKET_WRITE_EVENT_IND:
        {
            socketEvt.type = OD_SOCKET_EVENT_WRITE;
            socketEvt.data.socket = ((SOCKET_WRITE_EVENT_IND_SIG_T*)evt)->socket_id;
            socketEvt.result = OD_SUCCESS;
            break;
        }
        case SOCKET_ASYNC_GETHOSTBYNAME_CNF:
        {
            ASYNC_GETHOSTBYNAME_CNF_SIG_T* dns = (ASYNC_GETHOSTBYNAME_CNF_SIG_T*)evt;
            socketEvt.type = OD_SOCKET_EVENT_DNS;
            socketEvt.data.hostResolve.hostHandle = dns->request_id;
            socketEvt.result = (0 == dns->error_code) ? OD_SUCCESS : OD_ERROR;

            OD_TRACE_LOW("net: OD_SocketEventDispatcher, DNS event, requestID = %d, result = %d",
                socketEvt.data.hostResolve.hostHandle, socketEvt.result);

            if (socketEvt.result == OD_SUCCESS)
            {
                int i = 0;
                OD_ASSERT(dns->hostent.h_length == 4);

                od_memset(socketEvt.data.hostResolve.hostIP, 0, sizeof(socketEvt.data.hostResolve.hostIP));
                for(i = 0; (i<OD_SOCKET_HOST_MAX_ENTRY)&&(dns->hostent.h_addr_list[i]); i++)
                {
                    od_memcpy(&socketEvt.data.hostResolve.hostIP[i],
                                dns->hostent.h_addr_list[i],
                                dns->hostent.h_length);
                    socketEvt.data.hostResolve.netID = dns->netid;
                    OD_TRACE_LOW("net: OD_SocketEventDispatcher, Host resolved:%s", inet_ntoa(socketEvt.data.hostResolve.hostIP[i]));
                    socketEvt.data.hostResolve.hostIP[i] = ntohl(socketEvt.data.hostResolve.hostIP[i]);
                }
            }
            break;
        }
        default:
        {
            isSocketEvt = OD_FALSE;
            break;
        }
    }

    if ( socketEvt.type != OD_SOCKET_EVENT_DNS)
    {
        /* The DNS event has been logged */
        OD_TRACE_LOW("net: OD_SocketEventDispatcher, event type = %d, socket_id = %d, result =%d",
            socketEvt.type, socketEvt.data.socket, socketEvt.result);
    }

    if (isSocketEvt)
    {
        OD_ASSERT(context);

        return context->callback.app.handleSocketEvent(&socketEvt) == OD_SUCCESS ? OD_TRUE : OD_FALSE;
    }

	return OD_FALSE;
}

OD_STATUS od_inet_aton(char* addr, unsigned int* ip)
{
	OD_CHECK_ARGS_RETURN_STATUS(addr);
	OD_CHECK_ARGS_RETURN_STATUS(ip);

    if(ip) *ip  = 0;

    if(inet_aton(addr, ip))
    {
        *ip = ntohl(*ip);
        return OD_SUCCESS;
    }

    return OD_ERROR;
}

OD_STATUS od_inet_ntoa(unsigned int ip, char* addr)
{
	OD_CHECK_ARGS_RETURN_STATUS(ip);
	OD_CHECK_ARGS_RETURN_STATUS(addr);

    od_snprintf(addr, 16,
        "%d.%d.%d.%d",
        (unsigned char)(ip >> 24),
        (unsigned char)(ip >> 16),
        (unsigned char)(ip >> 8),
        (unsigned char)ip);
    addr[15] = 0;

    return OD_SUCCESS;
}
