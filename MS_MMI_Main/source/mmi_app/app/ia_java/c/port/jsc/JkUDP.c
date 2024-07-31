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
#ifdef JAVA_SUPPORT_IA
#include "jktypes.h"
#include "jkudp.h"
#include "udpresource.h"
#include "udptaskprocess.h"
#include "jwe_log.h"
#include "mmijava_internal.h"
#include "amudpevent.h"
#include "mmk_timer.h"
#ifndef WIN32
#ifdef PCLINK_SUPPORT
#include "tcpip_api.h"
#endif
#endif
#ifdef WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define IPPROTO_UDP 0
#define KJAVA_UDP_LINT_TIMER_OUT 200
#define KJAVA_UDP_LINK_TIMES 200

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern uint8 g_java_net_id;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : kjava_connectProc
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL int32 getHostByName( const JKSint8 *name, JKSint32 nameLength, uint32 *ip );

//******************************************************************************
//  Description:  Jkudp_initiate_connect
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 Jkudp_initiate_connect(int32 id);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get the max sessions for udp to use.
//  Global resource dependence :
//  Author: 
//  Note:return MAX_DATAGRAM_SOCKET
/*****************************************************************************/
JKSint32 JkUdpGetMaxSessions( void )
{   
    JWE_LOG(JkUdpGetMaxSessions, ("Max socket sessions: %d", MAX_DATAGRAM_SOCKET))
        
    return (JKSint32)MAX_DATAGRAM_SOCKET;
}

/*****************************************************************************/
//  Description : open the socket.
//  Global resource dependence :
//  Author: 
//  Note:* @param    address
//          the purpose address
// @param    addressLength
//           the length of address
// @param    port
//           the port will be used
// @return   
//       return Handle id means successful
//        return JK_INVALID_HANDLE means failfull
/*****************************************************************************/
JKUdpHandle JkUdpOpen( const JKSint8* address, JKSint32 addressLength, JKSint32 port )
{
    UDPResource             *resource = PNULL;
    int32                   id = 0;  
    MMIJAVA_NETWORK_TYPE_E  net_type = MMIJAVA_GetNetworkType();
    
    JWE_LOG( JkUdpOpen,("[JkUdpOpen] TRACE: address = %08xH, addressLength = %d, port = %d\n", address, addressLength,port) );
	if(net_type <JAVA_NETWORK_TYPE_WIFI)
    {
    	if (!MMIAPIPHONE_IsSimAvailable((MN_DUAL_SYS_E)(net_type)))
        {
            return JK_INVALID_HANDLE;
        }
    }
    
    Ajava_Initiliztion_UDP();

    if ( (address != NULL) && (addressLength <= 0) ) 
    {
        JWE_LOG(JkUdpOpen,("[JkUdpOpen] ERROR: invalid address length"));
        return JK_INVALID_HANDLE;
    }    
 
    resource = UDPResourceAlloc( &id );

    if ( resource == NULL ) 
    {
        JWE_LOG(JkUdpOpen,("[JkUdpOpen] ERROR: insufficient resource.\n"));
        return JK_INVALID_HANDLE;
    }
    resource->port = port;

    JWE_LOG( JkUdpOpen,("[JkUdpOpen] TRACE: allocated resource ID:%d\n", id ) );

   
    return (JKUdpHandle)id;   
    
}



//******************************************************************************
//  Description:  kjava_sock_link_polling
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
PUBLIC void MMIAPIJAVA_StartUDPLink(int32 id)
{
//    int32 err = java_NetConnect();
	//int32 err =0;
    UDPResource     *resource = PNULL;
	if(MMIJAVA_PDPActive())
    {
        resource = UDPResourceGet(id);   
        if (PNULL != resource)
        {
            resource->timer_times = KJAVA_UDP_LINK_TIMES;
            resource->timer_id = MMK_CreateTimerCallback(KJAVA_UDP_LINT_TIMER_OUT, MMIAPIJAVA_UDP_Timer_Process, NULL, SCI_TRUE);
            MMK_StartTimerCallback(resource->timer_id, KJAVA_UDP_LINT_TIMER_OUT,  MMIAPIJAVA_UDP_Timer_Process, NULL, SCI_TRUE);
        }
    }
}
//******************************************************************************
//  Description:  Jkudp_link_polling
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
//extern MMICMSWAP_GPRS_STATE_E MMIBRW_GetGprsState(void);
PUBLIC int32 Jkudp_link_polling(int32 id)
{
	// int32 ret = 0;
    UDPResource     *resource = PNULL;
       
    JWE_LOG(Jkudp_link_polling,("id:%d", id))
	   	
    resource = UDPResourceGet(id);   
    if (PNULL == resource)
    {
        return -1;
    }
	if(resource->timer_times <= 0)    
	{
	    JWE_LOG(Jkudp_link_polling,("timeout"))
	   	if (resource->timer_id != 0)
        {
            MMK_StopTimer(resource->timer_id);
            resource->timer_id = 0;
        }
		return -1;
	}	

	resource->timer_times--;

//	ret = MMIBRW_GetGprsState();
	#if 0
	if((ret == MMICMSWAP_GPRS_ACTIVE_OK) || (ret == MMICMSWAP_GPRS_ACEIVE_FAILED))
	{
        if (resource->timer_id != 0)
        {
            MMK_StopTimer(resource->timer_id);
            resource->timer_id = 0;
        }

	}
	#endif
	if(MMIPDP_LINK_STATE_ACTIVED==MMIAPIPDP_GetLinkState(MMI_MODULE_JAVA) )
	{
        if (resource->timer_id != 0)
        {
            MMK_StopTimer(resource->timer_id);
            resource->timer_id = 0;
        }
	 Jkudp_initiate_connect( id);   //@Ivy.su connect UDP

	}
	return 0;
}


//******************************************************************************
//  Description:  Jkudp_link_polling
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
LOCAL int32 Jkudp_initiate_connect(int32 id)
{
/*    int32                   socket = 0;
    UDPResource*            resource = PNULL;
    struct sci_sockaddr    addr = {0};
    int32                   ret = 0;
    TCPIP_NETID_T           java_net_id = g_java_net_id;
    int                     is_reuse = 1;        
    
    JWE_LOG(Jkudp_initiate_connect,("TRACE:id=%d\n", id));
    #ifdef WIFI_SUPPORT
    if(MMIJAVA_GetNetworkType() == JAVA_NETWORK_TYPE_WIFI)
    {
        java_net_id = MMIAPIWIFI_GetNetId();
    }
    #endif
    socket = sci_sock_socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP, java_net_id);
    if ( socket < 0 ) 
    {
        JWE_LOG(Jkudp_initiate_connect,("ERROR: SocketOpen() returned:%d\n", socket ));
        UDPResourceFree( id );
        AmNotifyUdpEvent((JKUdpHandle)id, JK_UDP_EVENT_TYPE_CONNECT, JK_UDP_ERROR_OTHER );
        return JK_UDP_ERROR_OTHER;
    }
    if (sci_sock_setsockopt(socket, SO_NBIO, NULL)==-1)
    {
    	   JWE_LOG(Jkudp_initiate_connect,("set no block socket error!"));
    }
    else
    {
        JWE_LOG(Jkudp_initiate_connect,("set no block socket OK!"));
    }

    //new add 20100124 -murphy.xie
    if (sci_sock_setsockopt(socket, SO_REUSEADDR, &is_reuse)==-1)
    {
    	   JWE_LOG(Jkudp_initiate_connect,("set reuse address socket error!"));
    }
    else
    {
        JWE_LOG(Jkudp_initiate_connect,("set reuse address socket OK!"));
    }
    
    resource = UDPResourceGet(id);
    if (PNULL == resource)
    {
        return JK_UDP_ERROR_OTHER;
    }
    addr.ip_addr = sci_gethostaddress_ext(java_net_id);
	addr.port = resource->port & 0xffff;//murphy Constant '-1' out of range for operator
    //addr.port = (resource->port!= -1)?(unsigned short)( resource->port & 0xffff ):0;
    ret = sci_sock_bind( socket, (struct sci_sockaddr *)(&(addr)), sizeof( addr )); 
    if ( ret != 0 ) 
    {
        JWE_LOG(Jkudp_initiate_connect,("ERROR: SocketBind() returned:%d\n", ret ));
        sci_sock_socketclose( socket );
        UDPResourceFree( id  );
        return JK_UDP_ERROR_OTHER;
    }

    JWE_LOG( Jkudp_initiate_connect,("TRACE: addr.ip_addr:%d\n", addr.ip_addr ) );
    
    resource->socket = socket;
    resource->reference++;    
    
    JWE_LOG( Jkudp_initiate_connect,("TRACE: Socket:%d was bound with resource:%d\n", socket, id ) );
    
    AmNotifyUdpEvent((JKUdpHandle)id, JK_UDP_EVENT_TYPE_CONNECT, JK_UDP_SUCCESS );*/
    return JK_UDP_SUCCESS;
      
}

/*****************************************************************************/
//  Description : connect the socket.
//  Global resource dependence :
//  Author: 
//  Note:* @param    socket
//          the socket id which will be connected
/*****************************************************************************/
JKSint32 JkUdpConnect( JKUdpHandle id )
{   
    // int32                   socket = 0;
    // int32                   pdp_active_state = 0;
    // int32                   i = 0;
    // struct sci_sockaddr		addr = {0};
    // UDPResource*			resource = PNULL;
    // int32               	ret = 0;
/*    MMIJAVA_NETWORK_TYPE_E  net_type = MMIJAVA_GetNetworkType();
	
    JWE_LOG( JkUdpConnect,("[JkUdpConnect] TRACE: id = %d\n", id ) );

	if(net_type < JAVA_NETWORK_TYPE_WIFI)
    {
#ifdef PCLINK_SUPPORT
    	#ifndef WIN32
        if(TCPIPPCLINK_IsPclinkEnable())
        {
            if (TCPIPPCLINK_SetNetInterface() == TRUE)
            {
                
            }     
            else
            {
                //SCI_TRACE_LOW:"JkUdpConnect pclink set interface error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKUDP_307_112_2_18_2_22_41_335,(uint8*)"");
                UDPResourceFree( (int32)id );
                AmNotifyUdpEvent( id, JK_UDP_EVENT_TYPE_CONNECT, JK_UDP_ERROR_OTHER );
                return JK_UDP_ERROR_OTHER;
            }
            
        } 
        else
    	#endif
#endif      
       {    
            if (MMIPDP_LINK_STATE_ACTIVED != MMIAPIPDP_GetLinkState(MMI_MODULE_JAVA))
            {
                JWE_LOG(JkUdpOpen, ("send MSG_JAVA_UDP_LINK"));
                MMIAPIJAVA_SendSignal(MSG_JAVA_UDP_LINK,(uint32)id);
                return JK_UDP_SUCCESS;
            }        
            	
        }
    }
    Jkudp_initiate_connect((int32)id);*/
	return JK_UDP_SUCCESS;
}

/*****************************************************************************/
//  Description : close  the connect.
//  Global resource dependence :
//  Author: 
//  Note:* @param    socket
//          the socket id which will be close
/*****************************************************************************/
JKSint32 JkUdpClose( JKUdpHandle id )
{
  /*  int32             ret = 0;
    UDPResource     *resource = PNULL;  
    
    JWE_LOG( JkUdpClose,("[JkUdpClose] TRACE: socket = %d\n", id ) );    
   
    resource = UDPResourceGet((int32)id );
    if ( resource == NULL ) 
    {        
        AmNotifyUdpEvent(id, JK_UDP_EVENT_TYPE_CLOSE, JK_UDP_ERROR_OTHER );
        UDPResourceFree((int32)id );
        return JK_UDP_ERROR_OTHER;
    }
    
    resource->reference--;
    if ( resource->reference > 0 ) 
    {
        AmNotifyUdpEvent(id, JK_UDP_EVENT_TYPE_CLOSE, JK_UDP_SUCCESS );       
        return JK_UDP_SUCCESS;
    }
    
    ret = sci_sock_socketclose( resource->socket );
    if ( ret != 0 ) 
    {
        JWE_LOG(JkUdpClose,("[UDPTaskProcess:closeProc] ERROR: SocketClose() returned:%d\n", ret ));
        AmNotifyUdpEvent(id, JK_UDP_EVENT_TYPE_CLOSE, JK_UDP_ERROR_OTHER );
        UDPResourceFree((int32)id); 
        return JK_UDP_ERROR_OTHER;
    }
    
    JWE_LOG( JkUdpClose,("[UDPTaskProcess:closeProc] TRACE: posting event.\n") );
    AmNotifyUdpEvent(id, JK_UDP_EVENT_TYPE_CLOSE, JK_UDP_SUCCESS );
    JWE_LOG( JkUdpClose,("[UDPTaskProcess:closeProc] TRACE: normally done.\n") );   

    UDPResourceFree((int32)id);  */
    
    return JK_UDP_SUCCESS;
}

/*****************************************************************************/
//  Description : close  the connect.
//  Global resource dependence :
//  Author: 
//  Note:* @param    socket
//          the socket id which will be close
/*****************************************************************************/
LOCAL int32 getHostByName( const JKSint8 *name, JKSint32 nameLength, uint32 *ip )
{
    char    *asciiz = PNULL;    /* '\0' terminated string. */
    int32     ret = 0;
    struct sci_hostent *hostent = PNULL;
    
    JWE_LOG( getHostByName,("[getHostByName] TRACE: name:%08xH nameLength:%08xH ip:%08xH\n",
        name, nameLength, ip ) );
    
    asciiz = SCI_ALLOC_APP( nameLength + 1 );
    if ( asciiz == NULL ) 
    {
        JWE_LOG(getHostByName,("[getHostByName] ERROR: Insufficient memory.\n"));
        return -1;
    }
    
    SCI_MEMCPY( asciiz, name, nameLength );
    asciiz[ nameLength ] = '\0';
    
    JWE_LOG( getHostByName,("[getHostByName] TRACE: destination name:%s\n", asciiz ) ); 
    
    hostent = sci_gethostbyname( asciiz );
    if ( hostent == 0 ) 
    {
        JWE_LOG(getHostByName,("[getHostByName] ERROR: failed to resolve address:%s, SocketGetHostByName returned:%d\n",
            asciiz, ret ));
        SCI_FREE( asciiz );
        return -2;
    }
    
    *ip = ntohl(*((unsigned long*)*hostent->h_addr_list));
    JWE_LOG(getHostByName,("ip is: %d",*ip));
    
    SCI_FREE( asciiz );
    return 0;
}
/*****************************************************************************/
//  Description : send  the information.
//  Global resource dependence :
//  Author: 
// @param    socket
//           the socket id
// @param    data
//           the data will be sent
// @param    dataLength
//           the data length
// @param    address
//           the purpose address
// @param    addressLength
//           the length of address
// @param    port
//           the port use to send
// @return   
//       return JK_UDP_ERROR_OTHER means error
//       return  UDPTaskProcessInitiateSending means sucessful
/*****************************************************************************/
JKSint32 JkUdpSendTo( JKUdpHandle id, const JKUint8 *data, JKSint32 dataLength,
                     const JKSint8 *address, JKSint32 addressLength, JKSint32 port )
{
  /*  int32                       ret = 0;
    struct sci_sockaddr        addr = {0};
    UDPResource                 *resource = PNULL;

    JWE_LOG( JkUdpSendTo,("[JkUdpSendTo] TRACE: socket:%d\n", id) );
    
    if ( (data == JK_NULL) || (dataLength < 0) ||      
        (address == JK_NULL) || (addressLength <= 0) ) 
    {
        JWE_LOG(JkUdpSendTo,("[JkUdpSendTo] ERROR: Invalid parameter\n"));
        return JK_UDP_ERROR_OTHER;
    }  
    
    resource = UDPResourceGet((int32)id);
    if ( resource == NULL ) 
    {      
        AmNotifyUdpEvent(id, JK_UDP_EVENT_TYPE_SENDTO, JK_UDP_ERROR_OTHER );    
        return JK_UDP_ERROR_OTHER;
    }
    
    ret = getHostByName(address, addressLength, &addr.ip_addr );
    if ( ret != 0 ) 
    {
        JWE_LOG(JkUdpSendTo,("[UDP:sendProc] ERROR: failed to resolve address.\n"));
        AmNotifyUdpEvent(id, JK_UDP_EVENT_TYPE_SENDTO, JK_UDP_ERROR_OTHER );
        return JK_UDP_ERROR_OTHER;
    }

    addr.port = (unsigned short)(port & 0xffff );
    
    ret = sci_sock_sendto(resource->socket, (char*)data, dataLength, 0, &addr);
    if ( ret != dataLength ) 
    {
        JWE_LOG(JkUdpSendTo,("[UDP:sendProc] ERROR: failed to send data, ret = %d\n", ret));
        AmNotifyUdpEvent(id, JK_UDP_EVENT_TYPE_SENDTO, JK_UDP_ERROR_OTHER );
        return JK_UDP_ERROR_OTHER;
    }    
    
    JWE_LOG( JkUdpSendTo,("[UDP:sendProc] TRACE: posting event.\n") );
    
    AmNotifyUdpEvent(id, JK_UDP_EVENT_TYPE_SENDTO, JK_UDP_SUCCESS );
    
    JWE_LOG( JkUdpSendTo,("[UDP:sendProc] TRACE: normally done.\n") );   */

    return JK_UDP_SUCCESS;    
}

/*****************************************************************************/
//  Description : receive  the information.
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/            
/* @param    socket
*           the socket id
* @param    data
*           in order to save the data which is received
* @param    dataLength
*           the received data length
* @param    address
*           the source address
* @param    addressLength
*           the length of address
* @return   
*       return JK_UDP_ERROR_OTHER means error
*       return  UDPTaskProcessInitiateReceiving means sucessful
*/
JKSint32 JkUdpReceiveFrom( JKUdpHandle id, JKUint8 *data, JKSint32 dataLength,
                          JKSint8 *address, JKSint32 addressLength)
{
  /*  int32                 ret = 0;
    int32                 size = 0;
    struct sci_sockaddr        from = {0};
    int32                 fromLen = 0;
    UDPResource         *resource = PNULL;
    char     tmp_data[8] = {0};
    uint32 ip = 0;
    // int32  e = 0;

    JWE_LOG( JkUdpReceiveFrom,("[JkUdpReceiveFrom] TRACE: id:%d\n", id) );
    
    if ( (data == JK_NULL) || (dataLength <= 0) ||
        (address == JK_NULL) || (addressLength <= 0) ) 
    {
        JWE_LOG(JkUdpReceiveFrom,("[JkUdpReceiveFrom] ERROR: Invalid parameter\n" ));
        return JK_UDP_ERROR_OTHER;
    }  

    resource = UDPResourceGet((int32)id);
    if ( resource == NULL ) 
    {
        JWE_LOG(JkUdpReceiveFrom,("[UDP:receiveProc] ERROR: Invalid handle:%08xH\n", id ));
        AmNotifyUdpReceived(id, JK_UDP_ERROR_OTHER, 0, -1);       
        return JK_UDP_ERROR_OTHER;
    }
    
    fromLen = sizeof(from);
    ret = sci_sock_recvfrom( resource->socket, (char*)data, (int)dataLength, 0, &from);
    JWE_LOG(JkUdpReceiveFrom,("[UDP:receiveProc]: SocketRecvfrom returned:%d\n", ret ));
    if ( ret < 0 ) 
    {
        JWE_LOG(JkUdpReceiveFrom,("[UDP:receiveProc] ERROR: SocketRecvfrom returned:%d\n", ret ));
        AmNotifyUdpReceived(id, JK_UDP_ERROR_OTHER, 0, -1);
        return JK_UDP_ERROR_OTHER;
    }
    size = ret;
    ip= from.ip_addr;
    tmp_data[3] = from.ip_addr & 0x000000ff;
    tmp_data[2] = (from.ip_addr & 0x0000ff00) >> 8;
    tmp_data[1] = (from.ip_addr & 0x00ff0000) >> 16;
    tmp_data[0] = (from.ip_addr & 0xff000000) >> 24; 
    
    sprintf((char *)(address), "%d.%d.%d.%d",
        tmp_data[0],
        tmp_data[1],
        tmp_data[2],
        tmp_data[3]);
    
    JWE_LOG(JkUdpReceiveFrom,("ip: %ld.",from.ip_addr) );
    JWE_LOG(JkUdpReceiveFrom,("ip: %s.",(char *)(address)) );
    
    if ( ret < 0 ) 
    {
        AmNotifyUdpReceived(id, JK_UDP_ERROR_OTHER, 0, -1);
        return JK_UDP_ERROR_OTHER;
    }
    else 
    {
        JWE_LOG( JkUdpReceiveFrom,("[UDP:receiveProc] TRACE: posting event.\n") );
        AmNotifyUdpReceived(id, size, addressLength, from.port);
        JWE_LOG( JkUdpReceiveFrom,("[UDP:receiveProc] TRACE: normally done.\n") );
    }   */
    return JK_UDP_SUCCESS;
}

/**             
* \brief  get local address.
* @param    socket
*           the socket id
* @param    address
*           the local address
* @param    addressLength
*           the length of local address
* @return   
*       return JK_UDP_ERROR_OTHER means error
*       return  nonzero means sucessful
*/
JKSint32 JkUdpGetLocalAddress( JKUdpHandle socket, JKSint8 *address, JKSint32 addressLength )
{
    UDPResource     *resource = PNULL;
    int32             ret = 0;
    uint32           ip = 0;
    int32           i = 0, p = 0;
    char                hostbuf[256] = {0};
    struct sci_hostent*     hostent = PNULL;
    
    JWE_LOG( JkUdpGetLocalAddress,("[JkUdpGetLocalAddress] TRACE: socket:%d address:%08xH addressLength:%d\n",
        socket, address, addressLength ) );
    
    resource = UDPResourceGet( (int32)socket );
    if ( resource == NULL ) 
    {
        JWE_LOG(JkUdpGetLocalAddress,("[JkUdpGetLocalAddress] ERROR: Invalid handle:%08xH\n", socket ));
        return JK_UDP_ERROR_OTHER;
    }
    JWE_LOG( JkUdpGetLocalAddress,("[JkUdpGetLocalAddress] TRACE: resource->localAddress.ip_addr:%08x\n", resource->localAddress.ip_addr) );
    
    /*
    * IP address long to string
    */  
    ret = sci_gethostname(hostbuf, sizeof(hostbuf));
    if ( ret != 0 ) 
    {
        JWE_LOG(JkUdpGetLocalAddress,("[connectProc] ERROR: failed to get host name, sci_gethostname() returned:%s\n", ret));   
        
        return JK_UDP_ERROR_OTHER;
    }
    else
    {
        JWE_LOG(JkUdpGetLocalAddress,("TRACE: host name: %s\n", hostbuf));
    }
    
    hostent = sci_gethostbyname(hostbuf);
    if ( hostent == NULL ) 
    {
        JWE_LOG(JkUdpGetLocalAddress,("[connectProc] ERROR: failed to get host ipaddr, sci_gethostbyname() falid\n"));        
        return JK_UDP_ERROR_OTHER;
    }
    else
    {
        resource->localAddress.ip_addr = ntohl(*((unsigned long*)*hostent->h_addr_list));
        JWE_LOG(JkUdpGetLocalAddress,("TRACE: host ipaddr: 0x%08x\n", resource->localAddress.ip_addr));
    }
    ip = resource->localAddress.ip_addr;        
    p = 0;
    for (i = 3; i >= 0; i--) 
    {                          // e.g. 0xAABBCCDD                  
        int n = (ip >> (8 * i)) & 0xff;                 //        -> 0xAA, 0xBB, 0xCC, 0xDD 
        
        if ((n >= 100) && (p < addressLength)) 
        {        // e.g. 0xAA(170) 
            *(address + p++) = '0' + (n / 100);         //        -> '1'  
        }
        if ((n >=  10) && (p < addressLength)) 
        {
            *(address + p++) = '0' + (n / 10 % 10);     //        -> '7'  
        }
        if (p < addressLength) {
            *(address + p++) = '0' + (n % 10);          //        -> '0'  
        } 
        else 
        {
            return JK_UDP_ERROR_OTHER;
        }
        if ((i > 0) && (p < addressLength)) 
        {
            *(address + p++) = '.';
        }
    }
    ret = p;
    
    
    JWE_LOG( JkUdpGetLocalAddress,("[JkUdpGetLocalAddress] TRACE: address:%s\n", address) );
    JWE_LOG( JkUdpGetLocalAddress,("[JkUdpGetLocalAddress] TRACE: returned:%d\n", ret) );
    
    return ret;
}

/**             
* \brief  get local port.
* @param    socket
*           the socket id
* @return   
*       return JK_UDP_ERROR_OTHER means error
*       return  nonzero means sucessful
*/
JKSint32 JkUdpGetLocalPort( JKUdpHandle socket )
{
    UDPResource     *resource = PNULL;
    int32                 ret = 0;
    struct sci_sockaddr addr = {0};
    
    JWE_LOG( JkUdpGetLocalPort,("[JkUdpGetLocalPort] TRACE: socket:%d\n", socket) );
    
    resource = UDPResourceGet( (int32)socket );
    if ( resource == NULL ) 
    {
        JWE_LOG(JkUdpGetLocalPort,("[JkUdpGetLocalPort] ERROR: Invalid handle:%08xH\n", socket ));
        return JK_UDP_ERROR_OTHER;
    }
    
    ret = sci_sock_getsockname( (long)resource->socket, &addr );
    if ( ret != 0 ) 
    {
        JWE_LOG(JkUdpGetLocalPort,("[connectProc] ERROR: failed to get local address, sci_sock_getsockname() returned:%d\n", ret));
        return JK_UDP_ERROR_OTHER;
    }
    else
    {
        resource->localAddress.port = ntohs(addr.port);
        JWE_LOG(JkUdpGetLocalPort,("TRACE: Local Port is:%d\n", resource->localAddress.port));
    }
    
    JWE_LOG( JkUdpGetLocalPort,("[JkUdpGetLocalPort] TRACE: returned:%d\n", resource->localAddress.port) );
    
    return resource->localAddress.port;
}

/**             
* \brief  get no minal  length.
* @param    socket
*           the socket id
* @return   
*       return JK_UDP_ERROR_OTHER means error
*       return  nonzero means sucessful
*/
JKSint32 JkUdpGetNominalLength( JKUdpHandle socket )
{
    int32 ret = 0;
    int32 optval = 0;
    UDPResource     *resource = PNULL;
    
    resource = UDPResourceGet( (int32)socket );
    if ( resource == NULL ) 
    {
        JWE_LOG(JkUdpGetNominalLength,("[JkUdpGetNominalLength] ERROR: Invalid handle:%08xH\n", socket ));
        return JK_UDP_ERROR_OTHER;
    }
    
    ret =sci_sock_getsockopt( resource->socket, SO_SNDBUF, (char*)&optval);
    JWE_LOG( JkUdpGetNominalLength,("[JkUdpGetNominalLength] ret=%d\n", ret) );
    if ( ret < 0 ) 
    {
        return JK_UDP_ERROR_OTHER;
    }
    
    return optval;
}

/**             
* \brief  get maximum  length.
* @param    socket
*           the socket id
* @return   
*       return JK_UDP_ERROR_OTHER means error
*       return  nonzero means sucessful
*/
JKSint32 JkUdpGetMaximumLength( JKUdpHandle socket )
{
    int32 ret = 0;
    int32 optval = 0;
    UDPResource     *resource = PNULL;
    
    resource = UDPResourceGet( (int32)socket );
    if ( resource == NULL ) 
    {
        JWE_LOG(JkUdpGetMaximumLength,("[JkUdpGetMaximumLength] ERROR: Invalid handle:%08xH\n", socket ));
        return JK_UDP_ERROR_OTHER;
    }
    
    ret = sci_sock_getsockopt( resource->socket, SO_SNDBUF, (char*)&optval);
    JWE_LOG( JkUdpGetMaximumLength,("[JkUdpGetMaximumLength] ret=%d\n", ret) );
    if ( ret < 0 ) 
    {
        return JK_UDP_ERROR_OTHER;
    }
    
    return optval;
}

/**             
* \brief  query the host name.
* @param    host
*           the information of the host
* @param    hostLength
*           the length of host
* @return   
*       return JK_UDP_ERROR_OTHER means error
*       return  UDPTaskProcessInitiateQuerying means sucessful
*/
JKSint32 JkUdpQueryHostname( const JKSint8 *host, JKSint32 hostLength )
{
/*    int32             ret = 0;
    uint32   dummyIP = 0;
    JKBool          result = JK_FALSE;
    JWE_LOG( JkUdpQueryHostname,("[JkUdpQueryHostname] TRACE: host:%08xH hostLength:%d.\n",
        host, hostLength ) );
    
    if ( (host == JK_NULL) || (hostLength <= 0) ) 
    {
        JWE_LOG(JkUdpQueryHostname,("[JkUdpQueryHostname] ERROR: Invalid parameter\n"));
        return JK_UDP_ERROR_OTHER;
    }
    
    ret = getHostByName( host, hostLength, &dummyIP );
    result = ( ( ret == 0 ) ? JK_TRUE : JK_FALSE );
    
    JWE_LOG( JkUdpQueryHostname,("[UDPTaskProcess:queryProc] TRACE: posting result:%d.\n", result) );
    AmPostUdpQueryResult( result );*/
    return JK_UDP_SUCCESS;
}

#endif
