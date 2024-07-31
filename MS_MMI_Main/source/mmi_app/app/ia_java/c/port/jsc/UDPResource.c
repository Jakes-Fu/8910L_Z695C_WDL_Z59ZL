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
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "os_api.h"
#include "jwe_log.h"
#include "jblend_config.h"
#include "udpresource.h"

#ifdef JAVA_SUPPORT_SHRINK_RAM
#include "kjava_shrink_ram.h"
#endif
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifndef JAVA_SUPPORT_SHRINK_RAM
static UDPResource  udpResource[MAX_DATAGRAM_SOCKET];
#else
DEFINE_STATIC_SHRINK_VAR_1_DIM(UDPResource,udpResource)
#endif

#ifdef JAVA_SUPPORT_SHRINK_RAM
BEGIN_DEFINE_SHRINK_VAR_INIT(UDPResource)
	SHRINK_VAR_MEMORY_1_DIM(UDPResource,udpResource,MAX_DATAGRAM_SOCKET)
END_DEFINE_SHRINK_VAR_INIT

BEGIN_DEFINE_SHRINK_VAR_FINI(UDPResource)
	FREE_SHRINK_VAR_MEMORY(udpResource)
END_DEFINE_SHRINK_VAR_FINI
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Initialization of connection resource control.
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
int32 UDPResourceInitialize(void)
{
    int32 i = 0;
    
    for (i = 0; i < MAX_DATAGRAM_SOCKET; i++) 
    {
        udpResource[i].isAllocated = 0;
    }
    
    JWE_LOG( UDPResourceInitialize,("UDPResourceInitialize() finished.\n") );
    return 0;
}

/*****************************************************************************/
//  Description : Finalization of connection resource control.
//  Global resource dependence :
//  Author: 
//  Note:return 0 means successful
/*****************************************************************************/
int32 UDPResourceFinalize(void)
{
   // UDPTaskProcessFinalize();
    JWE_LOG( UDPResourceFinalize,("UDPResourceFinalize() finished.\n") );
  
    
    return 0;
}

/*****************************************************************************/
//  Description : Allocate a resource for UDP connection.
//  Global resource dependence :
//  Author: id: the id of the udo resource to control the max datagram socket
//  Note:return 0 means successful
/*****************************************************************************/
UDPResource *UDPResourceAlloc(int32 *id)
{
    int32 i = 0;
    
   // SCI_GetMutex( resourceMutex,SCI_WAIT_FOREVER );
    for (i = 0; i < MAX_DATAGRAM_SOCKET; i++) 
    {
        if (udpResource[i].isAllocated == 0) 
        {
            udpResource[i].isAllocated = 1;
            udpResource[i].timer_id = 0;
            *id = i;
            break;
        }
    }
    //SCI_PutMutex( resourceMutex );
    if (i >= MAX_DATAGRAM_SOCKET) 
    {
        *id = -1;
        return NULL;
    }
    return &udpResource[i];
}

/*****************************************************************************/
//  Description : Free a resource for connection.
//  Global resource dependence :
//  Author: id: the id of the udo resource to control the max datagram socket
//  Note:return 0 means successful
/*****************************************************************************/
int32 UDPResourceFree(int32 id)
{
    if ((id < 0) || (id >= MAX_DATAGRAM_SOCKET)) 
    {
        return -1;
    }
    
   // SCI_GetMutex( resourceMutex,SCI_WAIT_FOREVER );
    udpResource[id].isAllocated = 0;
    if (PNULL != udpResource[id].url)
    {
        SCI_FREE(udpResource[id].url);
    }
   // SCI_PutMutex( resourceMutex );
    
    return 0;
}

/*****************************************************************************/
//  Description : Get address of resource by the ID of connection.
//  Global resource dependence :
//  Author: id: the id of the udo resource to control the max datagram socket
//  Note:return 0 means successful
/*****************************************************************************/
UDPResource *UDPResourceGet(int32 id)
{
    UDPResource *resource = NULL;
    
    if ((id < 0) || (id >= MAX_DATAGRAM_SOCKET)) 
    {
        return NULL;
    }
   // SCI_GetMutex( resourceMutex,SCI_WAIT_FOREVER );
    if ( udpResource[id].isAllocated == 1 ) 
    {
        resource = &udpResource[id];
    }
   // SCI_PutMutex( resourceMutex );
    
    return resource;
}

/*****************************************************************************/
//  Description : Initialization of connection resource control.
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_UDP_Timer_Process(uint8 timer_id, uint32 param)
{
    int32 i = 0;
    
    for (i = 0; i < MAX_DATAGRAM_SOCKET; i++) 
    {
        if (udpResource[i].timer_id == timer_id)
        {
            Jkudp_link_polling(i);          
        }
        
    }
}
#endif