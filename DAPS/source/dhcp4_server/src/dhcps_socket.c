/* vi: set sw=4 ts=4: */
/*
 * DHCP server client/server socket creation
 *
 * udhcp client/server
 * Copyright (C) 1999 Matthew Ramsay <matthewr@moreton.com.au>
 *			Chris Trew <ctrew@moreton.com.au>
 *
 * Rewrite by Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
//#include <net/if.h>
//#if (defined(__GLIBC__) && __GLIBC__ >= 2 && __GLIBC_MINOR__ >= 1) || defined _NEWLIB_VERSION
//# include <netpacket/packet.h>
//# include <net/ethernet.h>
//#else
//# include <asm/types.h>
//# include <linux/if_packet.h>
//# include <linux/if_ether.h>
//#endif

#include "socket_api.h"
#include "dhcps_common.h"
#include "dhcps_api.h"


TCPIP_SOCKET_T dhcps_sock_fd = 0;

PUBLIC int udhcp_read_interface(const char *inf, int *ifindex, uint32_t *nip, uint8_t *mac)
{
    /* get server MAC address */
    if( PNULL != mac )
    {
        if( !TCPIPNETIF_GetMac(mac, MAC_ADDR_LEN, DHCPS_GetNetif()) )
        {
            return ENP_LOGIC;
        } 
    }

    return 0;
}


/* 1. None of the callers expects it to ever fail */
/* 2. ip was always INADDR_ANY */
PUBLIC int udhcp_listen_socket(int port, const char *inf)
{   
    int result = 0; 
    int is_reuse = 1;
    int is_broadcast = 1;
    struct sci_sockaddr addr = {0};
    TCPIP_SOCKET_T sock = TCPIP_SOCKET_INVALID;

    /* create DHCP server socket */
    sock = sci_sock_socket(AF_INET, SOCK_DGRAM, 0, (TCPIP_NETID_T)DHCPS_GetNetif());
    if( TCPIP_SOCKET_ERROR == sock )
    {
        DHCPS_TRACE("tcpip dhcps: udhcp_listen_socket - create socket failed");       
        goto ERROR_END;
    }

    /* set socket to none-block mode */
    if( TCPIP_SOCKET_ERROR == sci_sock_setsockopt(sock, SO_NBIO, NULL) )
    {
        DHCPS_TRACE("tcpip dhcps: udhcp_listen_socket - set socketopt NBIO error %d", sci_sock_errno(sock));  
        goto ERROR_END;
    }

    /* set socket source address to be reused */
    if( TCPIP_SOCKET_ERROR == sci_sock_setsockopt(sock, SO_REUSEADDR, &is_reuse) )
    {
        DHCPS_TRACE("tcpip dhcps: udhcp_listen_socket - set socketopt REUSEADDR error %d", sci_sock_errno(sock));
        goto ERROR_END;
    }

    /* set socket to be broadcast */
    if( TCPIP_SOCKET_ERROR == sci_sock_setsockopt(sock, SO_BROADCAST, &is_broadcast) )
    {
        DHCPS_TRACE("tcpip dhcps: udhcp_listen_socket - set socketopt BROADCAST error %d", sci_sock_errno(sock));
        goto ERROR_END;
    }

    /* bind socket */
    DHCPS_MEMSET(&addr, 0, sizeof(addr));
    addr.family = AF_INET;
    addr.ip_addr = 0L;     /* 0 means to receive broadcast packet */
    addr.port = (uint16)port;
    addr.port = htons(addr.port);
    result = sci_sock_bind(sock, &addr, sizeof(addr));
    if( TCPIP_SOCKET_ERROR == result )
    {
        DHCPS_TRACE("tcpip dhcps: udhcp_listen_socket - bind socket error %d", sci_sock_errno(sock));
        goto ERROR_END;
    }
    
    /* register socket event */
    result = sci_sock_asyncselect(sock, DHCPS_GetTaskId(), AS_READ|AS_WRITE);              
    if( TCPIP_SOCKET_ERROR == result )
    {         
        DHCPS_TRACE("tcpip dhcps: udhcp_listen_socket - asyncselect error %d", sci_sock_errno(sock));     
        goto ERROR_END;
    }

    DHCPS_TRACE("tcpip dhcps: create server socket OK- %x", sock);
    dhcps_sock_fd = sock;
    return dhcps_sock_fd;

ERROR_END:
    sci_sock_socketclose(sock);
    dhcps_sock_fd = TCPIP_SOCKET_INVALID;
    return TCPIP_SOCKET_INVALID;
}

PUBLIC TCPIP_SOCKET_T udhcp_get_socketid(void)
{
    return dhcps_sock_fd;
}

