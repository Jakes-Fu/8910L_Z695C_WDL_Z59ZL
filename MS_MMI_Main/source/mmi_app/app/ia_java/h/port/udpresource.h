/*
 * Copyright 2004 Aplix Corporation. All rights reserved.
 */

#ifndef ___UDP_RESOURCE_H
#define ___UDP_RESOURCE_H

/*
 * Management the UDP Connection resources.
 */
#include "jblend_platform.h"
#include "app_tcp_if.h"
#include "jktypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_DATAGRAM_SOCKET     6
#define MAX_HOSTNAME_LEN        255

/*
 * Structure of a conneciton resource.
 */
typedef struct UDPResource {
    int             		isAllocated;
    int             		socket;
    unsigned short  		port;
    struct sci_sockaddr 	localAddress;
    int             		reference;
    JKUTF8Char*     		url;
    JKUTF8Char*     		filter;
    int32                   timer_id;
    int32                   timer_times;
} UDPResource;

/*****************************************************************************/
//  Description : Initialization of connection resource control.
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
int32 UDPResourceInitialize(void);

/*****************************************************************************/
//  Description : Finalization of connection resource control.
//  Global resource dependence :
//  Author: 
//  Note:return 0 means successful
/*****************************************************************************/
int32 UDPResourceFinalize(void);

/*****************************************************************************/
//  Description : Allocate a resource for UDP connection.
//  Global resource dependence :
//  Author: id: the id of the udo resource to control the max datagram socket
//  Note:return 0 means successful
/*****************************************************************************/
UDPResource *UDPResourceAlloc(int32 *id);

/*****************************************************************************/
//  Description : Free a resource for connection.
//  Global resource dependence :
//  Author: id: the id of the udo resource to control the max datagram socket
//  Note:return 0 means successful
/*****************************************************************************/
int32 UDPResourceFree(int32 id);

/*****************************************************************************/
//  Description : Get address of resource by the ID of connection.
//  Global resource dependence :
//  Author: id: the id of the udo resource to control the max datagram socket
//  Note:return 0 means successful
/*****************************************************************************/
UDPResource *UDPResourceGet(int32 id);
//******************************************************************************
//  Description:  Jkudp_link_polling
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
PUBLIC int32 Jkudp_link_polling(int32 id);


#ifdef __cplusplus
}
#endif

#endif /* ___UDP_RESOURCE_H */

