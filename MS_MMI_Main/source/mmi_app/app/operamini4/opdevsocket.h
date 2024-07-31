/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_SOCKET_H
#define OPDEV_SOCKET_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The max number of entries for host resolving.
 * If it can get more entries, only the first max entries will be used.
 */
#define OD_SOCKET_HOST_MAX_ENTRY (4)

/** 
* @brief Async socket event post from network 
*/
typedef enum _OD_SocketEventType
{
	OD_SOCKET_EVENT_READ	= 0x01,  /* Notify for read */
	OD_SOCKET_EVENT_WRITE	= 0x02,  /* Notify for write */
	OD_SOCKET_EVENT_ACCEPT	= 0x04,  /* Notify for accept */
	OD_SOCKET_EVENT_CONNECT	= 0x08,  /* Notify for connect */
	OD_SOCKET_EVENT_CLOSE	= 0x10,  /* Notify for close */
	OD_SOCKET_EVENT_DNS		= 0x20,  /* Notify for DNS result */

	/* 
	 * Some platforms ONLY
	 * @Note when getting the first READ event, we need receive 
	 *  all the incoming data. when getting the second READ event, 
	 *  we need setup a 500ms timer. If the timer is out or an 
	 *  OD_SOCKET_EVENT_READ_BUF_HALF_FULL event comes, we should
	 *  start to read the incoming data.
	 */
	OD_SOCKET_EVENT_READ_BUF_HALF_FULL = 0x40
} OD_SocketEventType;

typedef struct _OD_SocketAddress
{
	unsigned int ip;
	unsigned short port;
} OD_SocketAddress;

typedef struct _OD_SocketEvent
{
	OD_SocketEventType type;
	OD_STATUS result;	/** Error codes for this event */

	union
	{
		OD_Socket socket;
		struct 
		{
			OD_HostResolveHandle hostHandle;
			OD_NetID netID;
			unsigned int hostIP[OD_SOCKET_HOST_MAX_ENTRY];
		} hostResolve;
	} data;
} OD_SocketEvent;

/**
 * Async resolve the url.
 * @param context Opera context
 * @param url: the url need DNS lookup
 * @param handle: the host resolve handle return for later use in the async callback.
 * @return OD_SUCCESS if request applied. The result will send by via sig msg
 */
OD_ALL_TASK OD_STATUS OD_HostResolve(OD_Context *context, const char* url, OD_HostResolveHandle* handle);

/** 
 * Create a non-block socket.
 * @param context Opera context
 * @param pSocket [out] pointer to an address which will hold the new created socekt handler.
 * @return OD_SUCCESS created success.
 *         OD_ERROR if failure
 */
OD_ALL_TASK OD_STATUS OD_SocketCreate(OD_Context *context, OD_Socket * pSocket);

/**
 * Connect to peer
 * @param context Opera context
 * @param socket the socket handler
 * @param pSocketAddress romote address will connect.
 * @return OD_SUCCESS,OD_SOCKET_WOULDBLOCK if it success.
 *         OD_ERROR if failure
 */
OD_ALL_TASK OD_STATUS OD_SocketConnect(OD_Context *context, OD_Socket socket, const OD_SocketAddress* pSocketAddress);

/**
 * Close the socket
 * @param context Opera context
 * @param socket the socket handler which will be closed.
 * @return OD_SUCCESS if closed, otherwise OD_ERROR
 */
OD_ALL_TASK OD_STATUS OD_SocketClose(OD_Context *context, OD_Socket socket);

/**
 * Send data to peer
 * @param context Opera context
 * @param socket socket handler
 * @param buffer the buffer which hold the data need send
 * @param len the data length in bytes.
 * @param bytes_written, the number bytes wrote after this call.
 * @return OD_SUCCESS if sent
 *         OD_ERROR if failure
 */
OD_ALL_TASK OD_STATUS OD_SocketSend(OD_Context *context, OD_Socket socket, const void* buffer, int len, int* bytes_written);

/**
 * Receive socket data on socket read event
 * @param context Opera context
 * @param socket
 * @param buffer: buffer to store data
 * @param len: buffer length
 * @param bytes_received: should not be NULL, for design reason.
 * @param bytes_left indicators the left byte in the socket buffer which need receive again.
 *		  if len equal to bytes_received, it indicates we have more data need to read.
 *		  we need to read it soon later. otherwise the read event will not come any more.
 * @return : OD_SUCCESS, receive successfully. OD_SOCKET_WOULDBLOCK, try it later.
 */
OD_ALL_TASK OD_STATUS OD_SocketReceive(OD_Context *context, OD_Socket socket, void* buffer, int len, int* bytes_received, int* bytes_left);

/**
 * Parses a dotted-decimal Internet address
 * @Param addr, xxx.xxx.xxx.xxx
 * @Param ip[out], the result
 * @return OD_SUCCESS if done, OD_ERROR if failured
 */
OD_ALL_TASK OD_STATUS od_inet_aton(char* addr, unsigned int *ip);

/**
 * Formats an Internet address into its printable dotted-decimal representation.
 * @Param ip, the ip address need to format
 * @addr[out],a dotted-decimal Internet address
 * @return OD_SUCCESS if done, OD_ERROR if failured
 */
OD_ALL_TASK OD_STATUS od_inet_ntoa(unsigned int ip, char* addr);

#ifdef __cplusplus
};
#endif

#endif /* OPDEV_SOCKET_H */
