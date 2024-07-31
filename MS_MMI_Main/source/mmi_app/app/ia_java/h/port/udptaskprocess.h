/*
 * Copyright 2004 Aplix Corporation. All rights reserved.
 */

#ifndef __UDP_TASK_PROCESS_H
#define __UDP_TASK_PROCESS_H

/*
 * UDP Connection task processing.
 */

#include "jkudp.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
//  Description : Initialization all tasks.
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
// int32 UDPTaskProcessInitialize( void );

/*****************************************************************************/
//  Description : Finalization all tasks.
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
//int32 UDPTaskProcessFinalize( void );

/*****************************************************************************/
//  Description : Initiate asynchronous processing in opening datagram socket.
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
//int32 UDPTaskProcessInitiateConnecting( JKUdpHandle socket );

/*****************************************************************************/
//  Description : UDPTaskProcessInitiateSending
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
//int32 UDPTaskProcessInitiateSending( JKUdpHandle socket, const JKUint8 *data, JKSint32 dataLength,
 //                               const JKSint8 *address, JKSint32 addressLength, JKSint32 port );
/*****************************************************************************/
//  Description : Initiate receiving from a datagram socket.
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
//int32 UDPTaskProcessInitiateReceiving( JKUdpHandle socket, JKUint8 *buffer, JKSint32 bufferLength,
 //                               JKSint8 *addressBuf, JKSint32 addressBufLength );

/*****************************************************************************/
//  Description : Initiate closing a datagram socket.
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
//int32 UDPTaskProcessInitiateClosing( JKUdpHandle socket );

/*****************************************************************************/
//  Description : Initiate querying a host name.
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
//int32 UDPTaskProcessInitiateQuerying( const JKSint8 *host, JKSint32 hostLength );


#ifdef __cplusplus
}
#endif

#endif /*  __UDP_TASK_PROCESS_H */

