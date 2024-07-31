/* vi: set sw=4 ts=4: */
/*
 * udhcp server
 * Copyright (C) 1999 Matthew Ramsay <matthewr@moreton.com.au>
 *            Chris Trew <ctrew@moreton.com.au>
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

#include "os_api.h"
#include "app_tcp_if.h"
#include "in_message.h"
#include "osport.h"
#include "dhcps_common.h"
#include "dhcpd.h"
#include "dhcps_api.h"
#include "arp.h"

#if 0
/* globals */
uint32 s_dhcp_renewal_time   = 0xA8C0;
uint32 s_dhcp_rebinding_time = 0x12750;
#endif

LOCAL void ReleaseConfigInfo(void);

/* Send a packet to a specific mac address and ip address by creating our own ip packet */
static void send_packet_to_client(struct dhcp_packet *dhcp_pkt, int force_broadcast)
{
    const uint8_t* chaddr = NULL;
    uint32_t ciaddr = 0;
    struct sci_sockaddr dst_addr = {0};

    // Was:
    //if (force_broadcast) { /* broadcast */ }
    //else if (dhcp_pkt->ciaddr) { /* unicast to dhcp_pkt->ciaddr */ }
    //else if (dhcp_pkt->flags & htons(BROADCAST_FLAG)) { /* broadcast */ }
    //else { /* unicast to dhcp_pkt->yiaddr */ }
    // But this is wrong: yiaddr is _our_ idea what client's IP is
    // (for example, from lease file). Client may not know that,
    // and may not have UDP socket listening on that IP!
    // We should never unicast to dhcp_pkt->yiaddr!
    // dhcp_pkt->ciaddr, OTOH, comes from client's request packet,
    // and can be used.

    if( PNULL == dhcp_pkt )
    {
        DHCPS_TRACE("tcpip dhcps: send_packet_to_client - invalid input");
        return;
    }

    if( PNULL != force_broadcast
        || (dhcp_pkt->flags & htons(BROADCAST_FLAG))
        /*|| dhcp_pkt->ciaddr == 0 */)
    {
        DHCPS_TRACE("tcpip dhcps: send_packet_to_client - broadcasting packet to client");
        ciaddr = INADDR_BROADCAST;
        chaddr = MAC_BCAST_ADDR;
    }
    else
    {
        DHCPS_TRACE("tcpip dhcps: send_packet_to_client - unicasting packet to client ciaddr");
        if(TCPIP_IP_NULL_ADDR == dhcp_pkt->ciaddr)
        {
            ciaddr = dhcp_pkt->yiaddr;
        }
        else
        {
            ciaddr = dhcp_pkt->ciaddr;
        }
        chaddr = dhcp_pkt->chaddr;
        
        /* asume bptr is 0, fill an arp entry to make it could send to the client */
        fill_arp_entry(ciaddr, chaddr, DHCPS_GetNetif());
    }

    DHCPS_MEMSET(&dst_addr, 0, sizeof(dst_addr));
    dst_addr.family = AF_INET;
    dst_addr.port = htons(CLIENT_PORT);
    dst_addr.ip_addr = ciaddr;

    DHCPS_TRACE("tcpip dhcps: send_packet_to_client - dst addr %x", dst_addr.ip_addr);
    
    if( TCPIP_SOCKET_ERROR == sci_sock_sendto(
        udhcp_get_socketid(), (char*)dhcp_pkt, (int32)sizeof(struct dhcp_packet)- (int32)(DHCP_OPTIONS_BUFSIZE + CONFIG_UDHCPC_SLACK_FOR_BUGGY_SERVERS) + (int32)udhcp_end_option(dhcp_pkt->options) + 1, 0, &dst_addr) )
    {
       DHCPS_TRACE("tcpip dhcps: send_packet_to_client - send error %d", sci_sock_errno(udhcp_get_socketid()));
    }

}

/* Send a packet to gateway_nip using the kernel ip stack */
static void send_packet_to_relay(struct dhcp_packet *dhcp_pkt)
{
    if( PNULL == dhcp_pkt )
    {
        DHCPS_TRACE("tcpip dhcps: send_packet_to_relay - invalid input");
        return;
    }
    
    DHCPS_TRACE("tcpip dhcps: send_packet_to_relay - forwarding packet to relay");
    
    udhcp_send_kernel_packet(
        dhcp_pkt,
        server_config.server_nip,
        SERVER_PORT,
        dhcp_pkt->gateway_nip,
        SERVER_PORT);
}

static void send_packet(struct dhcp_packet *dhcp_pkt, int force_broadcast)
{
    if( PNULL == dhcp_pkt )
    {
        DHCPS_TRACE("tcpip dhcps: send_packet - invalid input");
        return;
    }
    
    if( 0 != dhcp_pkt->gateway_nip )
    {
        send_packet_to_relay(dhcp_pkt);
    }
    else
    {
        send_packet_to_client(dhcp_pkt, force_broadcast);
    }
}

static void init_packet(struct dhcp_packet *packet, struct dhcp_packet *oldpacket, char type)
{
    if( PNULL == packet || PNULL == oldpacket )
    {
        DHCPS_TRACE("tcpip dhcps: init_packet - invalid input");
        return;
    }
    
    /* sets op, htype, hlen, cookie fields and adds DHCP_MESSAGE_TYPE option */
    udhcp_init_header(packet, type);
    packet->xid = oldpacket->xid;
    DHCPS_MEMCPY(packet->chaddr, oldpacket->chaddr, sizeof(oldpacket->chaddr));
    packet->flags = oldpacket->flags;
    packet->gateway_nip = oldpacket->gateway_nip;
    packet->ciaddr = oldpacket->ciaddr;
    udhcp_add_simple_option(packet, DHCP_SERVER_ID, server_config.server_nip);
}

/* Fill options field, siaddr_nip, and sname and boot_file fields.
 * TODO: teach this code to use overload option.
 */
static void add_server_options(struct dhcp_packet *packet)
{
    struct option_set *curr = server_config.options;

    if( PNULL == packet )
    {
        DHCPS_TRACE("tcpip dhcps: add_server_options - invalid input");
        return;
    }   

    while( NULL != curr )
    {
        if (curr->data[OPT_CODE] != DHCP_LEASE_TIME)
            udhcp_add_binary_option(packet, curr->data);
        curr = curr->next;
    }

    packet->siaddr_nip = server_config.siaddr_nip;

    if( PNULL != server_config.sname )
        strncpy((char*)packet->sname, server_config.sname, sizeof(packet->sname) - 1);
    if( PNULL != server_config.boot_file )
        strncpy((char*)packet->file, server_config.boot_file, sizeof(packet->file) - 1);
}

static uint32_t select_lease_time(struct dhcp_packet *packet)
{
    uint32_t lease_time_sec = server_config.max_lease_sec;
    uint8_t *lease_time_opt = PNULL;

    if( PNULL == packet )
    {
        DHCPS_TRACE("tcpip dhcps: select_lease_time - invalid input");
        return lease_time_sec;
    }
    
    lease_time_opt = udhcp_get_option(packet, DHCP_LEASE_TIME);
    if( PNULL != lease_time_opt )
    {
        move_from_unaligned32(lease_time_sec, lease_time_opt);
        lease_time_sec = ntohl(lease_time_sec);
        if (lease_time_sec > server_config.max_lease_sec)
            lease_time_sec = server_config.max_lease_sec;
        if (lease_time_sec < server_config.min_lease_sec)
            lease_time_sec = server_config.min_lease_sec;
    }
    DHCPS_TRACE("tcpip dhcps: select_lease_time - lease_time %ds", lease_time_sec);
    return lease_time_sec;
}

/* We got a DHCP DISCOVER. Send an OFFER. */
/* NOINLINE: limit stack usage in caller */
static void send_offer(struct dhcp_packet *oldpacket, uint32_t static_lease_nip, struct dyn_lease *lease)
{
    struct dhcp_packet packet;
    uint32_t lease_time_sec;
    struct in_addr addr;

    init_packet(&packet, oldpacket, DHCPOFFER);

    /* if it is a static lease, use its IP */
    packet.yiaddr = static_lease_nip;
    /* else: */
    if( 0 == static_lease_nip )
    {
        /* we have no static lease for client's chaddr */
        uint32_t req_nip = 0;
        uint8_t* req_ip_opt = PNULL;
        const char* p_host_name = PNULL;

        if( PNULL != lease )
        {
            /* We have a dynamic lease for client's chaddr.
             * Reuse its IP (even if lease is expired).
             * Note that we ignore requested IP in this case.
             */
            packet.yiaddr = lease->lease_nip;
            DHCPS_TRACE("tcpip dhcps: send_offer - dynamic lease for client's chaddr");
        }
        /* Or: if client has requested an IP */
        else if (((req_ip_opt = udhcp_get_option(oldpacket, DHCP_REQUESTED_IP)) != NULL)
         /* (read IP) */
         && (move_from_unaligned32(req_nip, req_ip_opt), 1)
         /* and the IP is in the lease range */
         && (ntohl(req_nip) >= server_config.start_ip)  /*lint !e506*/
         && (ntohl(req_nip) <= server_config.end_ip)
         && (PNULL == is_nip_reserved(server_config.static_leases, req_nip)) /* this ip is not for static lease */
         /* and */
         && (  !(lease = find_lease_by_nip(req_nip)) /* is not already taken */
            || is_expired_lease(lease)/*lint !e820*/ /* or is taken, but expired */ 
            ))/*lint !e774*/ 
        {
            DHCPS_TRACE("tcpip dhcps: send_offer - have a request ip");
            packet.yiaddr = req_nip;
        }
        else
        {
            /* Otherwise, find a free IP */
            DHCPS_TRACE("tcpip dhcps: send_offer - to find a free IP");
            packet.yiaddr = find_free_or_expired_nip(oldpacket->chaddr);
        }

        if( !packet.yiaddr )
        {
            DHCPS_TRACE("tcpip dhcps: send_offer - invalid yiaddr");
            return;
        }
        /* Reserve the IP for a short time hoping to get DHCPREQUEST soon */
        p_host_name = (const char*) udhcp_get_option(oldpacket, DHCP_HOST_NAME);
        lease = add_lease(
            packet.chaddr,
            packet.yiaddr,
            server_config.offer_time,
            p_host_name,
            (PNULL != p_host_name)? (uint8_t)p_host_name[OPT_LEN - OPT_DATA] : 0
            );
        if( PNULL == lease )
        {
            DHCPS_TRACE("tcpip dhcps: send_offer - no free IP addresses. OFFER abandoned");
            return;
        }
    }

    lease_time_sec = select_lease_time(oldpacket);
    DHCPS_TRACE("tcpip dhcps: send_offer - lease time %ds", lease_time_sec);
    udhcp_add_simple_option(&packet, DHCP_LEASE_TIME, htonl(lease_time_sec));
#if 0
    // add DHCP_RENEWAL_TIME
    udhcp_add_simple_option(&packet, DHCP_RENEWAL_TIME, htonl(s_dhcp_renewal_time));
    
    // add DHCP_REBINDING_TIME
    udhcp_add_simple_option(&packet, DHCP_REBINDING_TIME, htonl(s_dhcp_rebinding_time));
#endif
    add_server_options(&packet);

    addr.s_addr = packet.yiaddr;
    // bb_info_msg("Sending OFFER of %s", inet_ntoa(addr));

    /* send_packet emits error message itself if it detects failure */
    send_packet(&packet, /*force_bcast:*/ 0);
}

/* NOINLINE: limit stack usage in caller */
static void send_NAK(struct dhcp_packet *oldpacket)
{
    struct dhcp_packet packet = {0};

    if( PNULL == oldpacket )
    {
        DHCPS_TRACE("tcpip dhcps: send_NAK - invalid input");
        return;
    }
    
    DHCPS_MEMSET(&packet, 0, sizeof(struct dhcp_packet));
    init_packet(&packet, oldpacket, DHCPNAK);
    DHCPS_TRACE("tcpip dhcps: send_NAK - to send an NAK");
    send_packet(&packet, /*force_bcast:*/1);
}

/* NOINLINE: limit stack usage in caller */
static void send_ACK(struct dhcp_packet *oldpacket, uint32_t yiaddr)
{
    struct dhcp_packet packet;
    uint32_t lease_time_sec;
    struct in_addr addr;
    const char *p_host_name;

    if( PNULL == oldpacket )
    {
        DHCPS_TRACE("tcpip dhcps: send_ACK - invalid input");
        return;
    }
    
    DHCPS_MEMSET(&packet,0,sizeof(struct dhcp_packet));
    init_packet(&packet, oldpacket, DHCPACK);
    packet.yiaddr = yiaddr;

    lease_time_sec = select_lease_time(oldpacket);
    udhcp_add_simple_option(&packet, DHCP_LEASE_TIME, htonl(lease_time_sec));

    add_server_options(&packet);

    addr.s_addr = yiaddr;
    send_packet(&packet, /*force_bcast:*/ 1);  //to be changed

    p_host_name = (const char*) udhcp_get_option(oldpacket, DHCP_HOST_NAME);
    add_lease(packet.chaddr, packet.yiaddr,
        lease_time_sec,
        p_host_name,
        p_host_name ? (unsigned char)p_host_name[OPT_LEN - OPT_DATA] : 0
    );
   
}

/* NOINLINE: limit stack usage in caller */
static void send_inform(struct dhcp_packet *oldpacket)
{
    struct dhcp_packet packet;

    /* "If a client has obtained a network address through some other means
     * (e.g., manual configuration), it may use a DHCPINFORM request message
     * to obtain other local configuration parameters.  Servers receiving a
     * DHCPINFORM message construct a DHCPACK message with any local
     * configuration parameters appropriate for the client without:
     * allocating a new address, checking for an existing binding, filling
     * in 'yiaddr' or including lease time parameters.  The servers SHOULD
     * unicast the DHCPACK reply to the address given in the 'ciaddr' field
     * of the DHCPINFORM message.
     * ...
     * The server responds to a DHCPINFORM message by sending a DHCPACK
     * message directly to the address given in the 'ciaddr' field
     * of the DHCPINFORM message.  The server MUST NOT send a lease
     * expiration time to the client and SHOULD NOT fill in 'yiaddr'."
     */

    //TODO: do a few sanity checks: is ciaddr set?
    //Better yet: is ciaddr == IP source addr?
    if( PNULL == oldpacket )
    {
        DHCPS_TRACE("tcpip dhcps: send_inform - invalid input");
        return;
    }
    
    init_packet(&packet, oldpacket, DHCPACK);
    add_server_options(&packet);
    send_packet(&packet, /*force_bcast:*/ 1);
}


/* globals */
struct dyn_lease *g_leases = PNULL;
struct server_config_t server_config = {0};
 
/* struct server_config_t server_config is in bb_common_bufsiz1 */
void udhcpd_main(uint32 argc, void *argv)
{
    int server_socket = -1 ;// retval = -1;
    struct dhcp_packet packet = {0};
    uint8_t *state = PNULL;
    uint32_t static_lease_nip = 0;
    unsigned num_ips = 0;
    struct option_set *option = PNULL;
    struct dyn_lease *lease = PNULL, fake_lease ={0};
    xSignalHeaderRec* sig_ptr = 0; 
    BOOLEAN is_exit = FALSE;

    DHCPS_SetTaskId(THIS_TASK);
    DHCPS_TRACE("tcpip dhcps: udhcpd_main - startup DHCP server task entry");
 
    read_config();
    
    option = udhcp_find_option(server_config.options, DHCP_LEASE_TIME);
    server_config.max_lease_sec = DEFAULT_LEASE_TIME;
    if(PNULL != option )
    {
        move_from_unaligned32(server_config.max_lease_sec, option->data + OPT_DATA);
        server_config.max_lease_sec = ntohl(server_config.max_lease_sec);
    }

    /* Sanity check */
    num_ips = server_config.end_ip - server_config.start_ip + 1;
    if( server_config.max_leases > num_ips )
    {
        DHCPS_TRACE("tcpip dhcps: udhcpd_main - adjust max_leases %u -> %u",
            server_config.max_leases, num_ips);
        server_config.max_leases = num_ips;
    }
    
    g_leases =(struct dyn_lease*)DHCPS_ALLOC(server_config.max_leases*sizeof(g_leases[0]));
    if( PNULL == g_leases )
    {
        goto ret;
    }
    
    DHCPS_MEMSET(g_leases, 0, server_config.max_leases*sizeof(g_leases[0]));    

    if( udhcp_read_interface(
            server_config.inf,
            &server_config.ifindex,
            &server_config.server_nip,
            server_config.server_mac) )
    {
        goto ret;
    }

    /* create and init server socket */
    server_socket = udhcp_listen_socket(/*INADDR_ANY,*/ SERVER_PORT, server_config.inf); 
    if( TCPIP_SOCKET_INVALID == server_socket )
    {      
        goto ret;
    }
    
    DHCPS_TRACE("tcpip dhcps: udhcpd_main - server default lease time = %d", server_config.max_lease_sec);
 
    while( !is_exit )    /* loop until universe collapses */
    {
        int bytes = 0;     
        uint8_t *server_id_opt = PNULL;
        uint8_t *requested_opt = PNULL;
        uint32_t requested_nip = 0;// = requested_nip; /* for compiler */

        sig_ptr = SCI_GetSignal(DHCPS_GetTaskId());
        DHCPS_MEMSET(&packet, 0, sizeof(struct dhcp_packet));

        switch( sig_ptr->SignalCode )
        {
        case SOCKET_READ_EVENT_IND:
            bytes = udhcp_recv_kernel_packet(&packet,server_socket);
            break;

        case SOCKET_WRITE_EVENT_IND:
            DHCPS_TRACE("tcpip dhcps: udhcpd_main - receive SOCKET_WRITE_EVENT_IND msg");
            break;

        /* when received FULL_CLOSED, shut-down DHCP server task */
        case SOCKET_FULL_CLOSED_EVENT_IND:
            is_exit = TRUE;
            DHCPS_FREE(sig_ptr);
            sig_ptr = PNULL;
            goto ret;
             
        default:
            break;
        }

        DHCPS_FREE(sig_ptr);        
        sig_ptr = PNULL;
        
        if( packet.hlen != 6 )
        {
            DHCPS_TRACE("tcpip dhcps: udhcpd_main - MAC length != 6, ignoring packet");
            continue;
        }
        if( packet.op != BOOTREQUEST )
        {
            DHCPS_TRACE("tcpip dhcps: udhcpd_main - not a REQUEST, ignoring packet");
            continue;
        }
        state = udhcp_get_option(&packet, DHCP_MESSAGE_TYPE);
        if( PNULL == state || state[0] < DHCP_MINTYPE || state[0] > DHCP_MAXTYPE)
        {
            DHCPS_TRACE("tcpip dhcps: udhcpd_main - no or bad message type option, ignoring packet");
            continue;
        }

        /* Look for a static/dynamic lease */
        static_lease_nip = get_static_nip_by_mac(server_config.static_leases, packet.chaddr);
        if (static_lease_nip)
        {
            bb_info_msg("tcpip dhcps: udhcpd_main - found static lease IP: %x mac: %02X:%02X:%02X:%02X:%02X:%02X", 
                ntohl(static_lease_nip), packet.chaddr[0], packet.chaddr[1], packet.chaddr[2],
                packet.chaddr[3], packet.chaddr[4], packet.chaddr[5]);
            DHCPS_MEMSET(&fake_lease,0,sizeof(struct dyn_lease));
            DHCPS_MEMCPY(fake_lease.lease_mac, packet.chaddr, MAC_ADDR_LEN);
            fake_lease.lease_nip = static_lease_nip;
            fake_lease.expires = 0;
            lease = &fake_lease;
        }
        else
        {
            lease = find_lease_by_mac(packet.chaddr);

            if(NULL != lease)
            {
                bb_info_msg("tcpip dhcps: udhcpd_main - found lease IP: %x mac: %02X:%02X:%02X:%02X:%02X:%02X", 
                    ntohl(lease->lease_nip), lease->lease_mac[0], lease->lease_mac[1], lease->lease_mac[2],
                    lease->lease_mac[3], lease->lease_mac[4], lease->lease_mac[5]);
                /* if nip in old lease is reserved, so set lease to zero */
                if(PNULL != is_nip_reserved(server_config.static_leases, lease->lease_nip))
                {
                    DHCPS_TRACE("tcpip dhcps: udhcp_main - found lease IP %x has reserved for static lease", ntohl(lease->lease_nip));
                    lease = NULL;
                }
            }
        }

        /* Get REQUESTED_IP and SERVER_ID if present */
        server_id_opt = udhcp_get_option(&packet, DHCP_SERVER_ID);
        if( PNULL != server_id_opt )
        {
            uint32_t server_id_net;
            move_from_unaligned32(server_id_net, server_id_opt);
            if (server_id_net != server_config.server_nip)
            {
                /* client talks to somebody else */
                DHCPS_TRACE("tcpip dhcps: udhcpd_main - server ID doesn't match, ignoring");
                continue;
            }
        }
        
        requested_opt = udhcp_get_option(&packet, DHCP_REQUESTED_IP);
        if( PNULL != requested_opt )
        {
            move_from_unaligned32(requested_nip, requested_opt);
        }

        switch( state[0] )
        {

        case DHCPDISCOVER:
            DHCPS_TRACE("tcpip dhcps: udhcpd_main - received DISCOVER");
            send_offer(&packet, static_lease_nip, lease);
            break;

        case DHCPREQUEST:
            DHCPS_TRACE("tcpip dhcps: udhcpd_main - received REQUEST");

/* RFC 2131:

o DHCPREQUEST generated during SELECTING state:

   Client inserts the address of the selected server in 'server
   identifier', 'ciaddr' MUST be zero, 'requested IP address' MUST be
   filled in with the yiaddr value from the chosen DHCPOFFER.

   Note that the client may choose to collect several DHCPOFFER
   messages and select the "best" offer.  The client indicates its
   selection by identifying the offering server in the DHCPREQUEST
   message.  If the client receives no acceptable offers, the client
   may choose to try another DHCPDISCOVER message.  Therefore, the
   servers may not receive a specific DHCPREQUEST from which they can
   decide whether or not the client has accepted the offer.

o DHCPREQUEST generated during INIT-REBOOT state:

   'server identifier' MUST NOT be filled in, 'requested IP address'
   option MUST be filled in with client's notion of its previously
   assigned address. 'ciaddr' MUST be zero. The client is seeking to
   verify a previously allocated, cached configuration. Server SHOULD
   send a DHCPNAK message to the client if the 'requested IP address'
   is incorrect, or is on the wrong network.

   Determining whether a client in the INIT-REBOOT state is on the
   correct network is done by examining the contents of 'giaddr', the
   'requested IP address' option, and a database lookup. If the DHCP
   server detects that the client is on the wrong net (i.e., the
   result of applying the local subnet mask or remote subnet mask (if
   'giaddr' is not zero) to 'requested IP address' option value
   doesn't match reality), then the server SHOULD send a DHCPNAK
   message to the client.

   If the network is correct, then the DHCP server should check if
   the client's notion of its IP address is correct. If not, then the
   server SHOULD send a DHCPNAK message to the client. If the DHCP
   server has no record of this client, then it MUST remain silent,
   and MAY output a warning to the network administrator. This
   behavior is necessary for peaceful coexistence of non-
   communicating DHCP servers on the same wire.

   If 'giaddr' is 0x0 in the DHCPREQUEST message, the client is on
   the same subnet as the server.  The server MUST broadcast the
   DHCPNAK message to the 0xffffffff broadcast address because the
   client may not have a correct network address or subnet mask, and
   the client may not be answering ARP requests.

   If 'giaddr' is set in the DHCPREQUEST message, the client is on a
   different subnet.  The server MUST set the broadcast bit in the
   DHCPNAK, so that the relay agent will broadcast the DHCPNAK to the
   client, because the client may not have a correct network address
   or subnet mask, and the client may not be answering ARP requests.

o DHCPREQUEST generated during RENEWING state:

   'server identifier' MUST NOT be filled in, 'requested IP address'
   option MUST NOT be filled in, 'ciaddr' MUST be filled in with
   client's IP address. In this situation, the client is completely
   configured, and is trying to extend its lease. This message will
   be unicast, so no relay agents will be involved in its
   transmission.  Because 'giaddr' is therefore not filled in, the
   DHCP server will trust the value in 'ciaddr', and use it when
   replying to the client.

   A client MAY choose to renew or extend its lease prior to T1.  The
   server may choose not to extend the lease (as a policy decision by
   the network administrator), but should return a DHCPACK message
   regardless.

o DHCPREQUEST generated during REBINDING state:

   'server identifier' MUST NOT be filled in, 'requested IP address'
   option MUST NOT be filled in, 'ciaddr' MUST be filled in with
   client's IP address. In this situation, the client is completely
   configured, and is trying to extend its lease. This message MUST
   be broadcast to the 0xffffffff IP broadcast address.  The DHCP
   server SHOULD check 'ciaddr' for correctness before replying to
   the DHCPREQUEST.

   The DHCPREQUEST from a REBINDING client is intended to accommodate
   sites that have multiple DHCP servers and a mechanism for
   maintaining consistency among leases managed by multiple servers.
   A DHCP server MAY extend a client's lease only if it has local
   administrative authority to do so.
*/
            if (PNULL == requested_opt)
            {
                requested_nip = packet.ciaddr;
                if (requested_nip == 0)
                {
                    DHCPS_TRACE("tcpip dhcps: udhcpd_main - no requested IP and no ciaddr, ignoring");
                    break;
                }
            }
            
            /* this ip is reserved for static ip and the static ip is not match the ip that searched by mac */
            /* the second is mac is reserved for static ip and the requeset ip is not match is static ip that searched by mac */
            if(((PNULL != is_nip_reserved(server_config.static_leases, requested_nip))&&(requested_nip != static_lease_nip))||
                ((NULL != static_lease_nip) && (requested_nip != static_lease_nip)))//static ip
            {
                DHCPS_TRACE("tcpip dhcps: udhcpd_main - client requested IP %x has matches the static lease but doesn't match the IP %x searched by mac", ntohl(requested_nip), ntohl(static_lease_nip));
                send_NAK(&packet);
                break;
            }

            /* parms reserved in the server_config such as server_config.start_ip or server_config.end_ip are host byte order,
               but requested_nip is network byte order, so use the ntohl change the byte order from network to host.*/
            if((ntohl(requested_nip) < server_config.start_ip)||(ntohl(requested_nip) > server_config.end_ip))//not in the ip feild
            {
                DHCPS_TRACE("tcpip dhcps: udhcpd_main - client requested IP %x has out of the range that server could assign from %x to %x", ntohl(requested_nip), server_config.start_ip ,server_config.end_ip);
                send_NAK(&packet);
                break;
            }

            if(ntohl(requested_nip) == server_config.server_nip)//is server ip
            {
                DHCPS_TRACE("tcpip dhcps: udhcpd_main - client requested IP %x is Client Server IP", ntohl(requested_nip));
                send_NAK(&packet);
                break;
            }

            if((0 != server_config.subnet_mask)&&((~server_config.subnet_mask) == ((~server_config.subnet_mask)&ntohl(requested_nip))))//broadcast ip
            {
                DHCPS_TRACE("tcpip dhcps: udhcpd_main - client requested IP %x is a broadcast IP and submask =%x", ntohl(requested_nip), server_config.subnet_mask);
                send_NAK(&packet);
                break;
            }

            if ((PNULL != lease) && (requested_nip == lease->lease_nip))
            {
                DHCPS_TRACE("tcpip dhcps: udhcpd_main - client requested or configured ip %x matches the lease", ntohl(requested_nip));
                /* client requested or configured IP matches the lease.
                 * ACK it, and bump lease expiration time. */
                send_ACK(&packet, lease->lease_nip);
                break;
            }

            /* to resolve the win7 could not connect dhcp server problem. because the win7 send 
            dhcp request first instead of dhcp discover*/
            if((PNULL != find_lease_by_nip(requested_nip))&&(find_lease_by_nip(packet.ciaddr) != find_lease_by_mac(packet.chaddr)))//used
            {
                DHCPS_TRACE("tcpip dhcps: udhcpd_main - client requested IP %x has already been used", ntohl(requested_nip));
                send_NAK(&packet);
                break;
            }
            
            if (PNULL != server_id_opt)
            {
                /* client was talking specifically to us.
                 * "No, we don't have this IP for you". */
                DHCPS_TRACE("tcpip dhcps: udhcpd_main - client was talking specifically to us");
                send_NAK(&packet);
                break;
            }

            /* the request ip is not used and it is not reserved for static ip*/
            send_ACK(&packet, requested_nip);
            DHCPS_TRACE("tcpip dhcps: udhcpd_main - client requested IP %x is not used, so you can use it", ntohl(requested_nip));
            break;

        case DHCPDECLINE:
            /* RFC 2131:
             * "If the server receives a DHCPDECLINE message,
             * the client has discovered through some other means
             * that the suggested network address is already
             * in use. The server MUST mark the network address
             * as not available and SHOULD notify the local
             * sysadmin of a possible configuration problem."
             *
             * SERVER_ID must be present,
             * REQUESTED_IP must be present,
             * chaddr must be filled in,
             * ciaddr must be 0 (we do not check this)
             */
            DHCPS_TRACE("tcpip dhcps: udhcpd_main - received DECLINE");
            if ((PNULL != server_id_opt)
             && (PNULL != requested_opt)
             && (PNULL != lease)  /* chaddr matches this lease */
             && (requested_nip == lease->lease_nip)
            )
            {
                DHCPS_MEMSET(lease->lease_mac, 0, sizeof(lease->lease_mac));            
                lease->expires = SCI_GetCurrentTime() + server_config.decline_time;
            }
            break;

        case DHCPRELEASE:
            /* "Upon receipt of a DHCPRELEASE message, the server
             * marks the network address as not allocated."
             *
             * SERVER_ID must be present,
             * REQUESTED_IP must not be present (we do not check this),
             * chaddr must be filled in,
             * ciaddr must be filled in
             */
            DHCPS_TRACE("tcpip dhcps: udhcpd_main - received RELEASE");
            if ((PNULL != server_id_opt)
             && (PNULL != lease)  /* chaddr matches this lease */
             && (packet.ciaddr == lease->lease_nip)
            )
            {
                lease->expires = SCI_GetCurrentTime();
            }
            break;

        case DHCPINFORM:
            DHCPS_TRACE("tcpip dhcps: udhcpd_main - received INFORM");
            /* modified dhcp inform to suit more cases. */
            if(NULL == packet.ciaddr)
            {
                DHCPS_TRACE("tcpip dhcps: udhcpd_main - no inform ciaddr, ignoring");
                break;
            }

            /* parms reserved in the server_config such as server_config.start_ip or server_config.end_ip are host byte order,
               but requested_nip is network byte order, so use the ntohl change the byte order from network to host.*/
            if((ntohl(packet.ciaddr) < server_config.start_ip)||(ntohl(packet.ciaddr) > server_config.end_ip))//not in the ip feild
            {
                DHCPS_TRACE("tcpip dhcps: udhcpd_main - client inform IP %x has out of the range that server could assign from %x to %x", ntohl(packet.ciaddr), server_config.start_ip ,server_config.end_ip);
                send_NAK(&packet);
                break;
            }

            if(ntohl(packet.ciaddr) == server_config.server_nip)//is server ip
            {
                DHCPS_TRACE("tcpip dhcps: udhcpd_main - client inform IP %x is Client Server IP", ntohl(packet.ciaddr));
                send_NAK(&packet);
                break;
            }

            if((0 != server_config.subnet_mask)&&((~server_config.subnet_mask) == ((~server_config.subnet_mask)&ntohl(packet.ciaddr))))//broadcast ip
            {
                DHCPS_TRACE("tcpip dhcps: udhcpd_main - client inform IP %x is a broadcast IP and submask =%x", ntohl(packet.ciaddr), server_config.subnet_mask);
                send_NAK(&packet);
                break;
            }

            /* this ip has already used by the other user.*/
            if((PNULL != find_lease_by_nip(packet.ciaddr))&&(find_lease_by_nip(packet.ciaddr) != find_lease_by_mac(packet.chaddr)))//used  
            {
                DHCPS_TRACE("tcpip dhcps: udhcpd_main - client inform IP %x has already been used", ntohl(packet.ciaddr));
                send_NAK(&packet);
                break;
            }

            /* the ip is reserved for static ip and the current ip is not equal the static ip */
            /* the second is mac is reserved for static ip and the requeset ip is not match is static ip that searched by mac */
            if(((PNULL != is_nip_reserved(server_config.static_leases, packet.ciaddr)) &&(requested_nip != static_lease_nip))||
                ((NULL != static_lease_nip) && (requested_nip != static_lease_nip)))//static ip
            {
                DHCPS_TRACE("tcpip dhcps: udhcpd_main - client inform IP %x has matches the static lease but does'nt match the IP %x searched by mac", ntohl(packet.ciaddr), ntohl(static_lease_nip));
                send_NAK(&packet);
                break;
            }

            /* find the release by packet mac, and get a address, if it is equal send inform msg back to client*/
            if ((PNULL != lease)&&(packet.ciaddr == lease->lease_nip))
            {
                DHCPS_TRACE("tcpip dhcps: udhcpd_main - inform ciaddr %x has matches the lease", ntohl(packet.ciaddr));
                send_inform(&packet);
                break;
            }
            
            send_inform(&packet);
            DHCPS_TRACE("tcpip dhcps: udhcpd_main - client inform IP %x is not used, so you can use it", ntohl(packet.ciaddr));
            break;

        default:
            DHCPS_TRACE("tcpip dhcps: udhcpd_main - unknown DHCP state %d", state[0]);
            break;
        }   
    }

ret:
    /* Empty the signal queue */
    while (1)/*lint !e716*/
    {
        sig_ptr = (void*)SCI_PeekSignal(DHCPS_GetTaskId());
        if (PNULL != sig_ptr)
        {
            DHCPS_FREE(sig_ptr);
            sig_ptr = PNULL;
        }
        else
        {
            break;
        }
    }

    ReleaseConfigInfo();
    ReleaseLeaseInfo();
    DHCPS_ResetNetif();
    DHCPS_ResetTaskId();
    DHCPS_SetOpenStatus(FALSE);
    DHCPS_TRACE("tcpip dhcps: udhcpd_main - dhcp server task is end");
    SCI_ThreadExit();
}

LOCAL void ReleaseConfigInfo(void)
{   
    struct option_set *cur,*next;
    
    if(PNULL != server_config.inf)
    {
        DHCPS_FREE(server_config.inf);  
        server_config.inf = PNULL;
    }
    
    if(PNULL != server_config.lease_file)
    {
        DHCPS_FREE(server_config.lease_file); 
        server_config.lease_file = PNULL;
    }
    //realse options info
    cur = server_config.options;
    while(cur)
    {
        next = cur->next;
        DHCPS_FREE(cur);       
        cur = next;
    }
    server_config.options =  PNULL;   
    //realse pidfile    
    if(PNULL != server_config.pidfile)
    {
        DHCPS_FREE(server_config.pidfile);   
        server_config.pidfile = PNULL;
    }
    
    if(PNULL != server_config.sname)
    {
        DHCPS_FREE(server_config.sname);  
        server_config.sname = PNULL;
    }

    if(PNULL != server_config.boot_file)
    {
        DHCPS_FREE(server_config.boot_file);   
        server_config.boot_file = PNULL;
    }
    
    release_static_leases(&server_config.static_leases);
}

PUBLIC struct server_config_t* DHCPS_GetServerConfig(void)
{
    return &server_config;
}

PUBLIC struct dyn_lease* DHCPS_GetLeaseInfo(void)
{

    return g_leases;
}
    


