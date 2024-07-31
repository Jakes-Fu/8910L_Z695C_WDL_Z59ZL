/* vi: set sw=4 ts=4: */
/*
 * Packet ops
 *
 * Rewrite by Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */

#include "in_message.h"
#include "socket_types.h"
#include "socket_api.h"
#include "osport.h"
#include "dhcps_common.h"
#include "dhcpd.h"
#include "dhcpd_define.h"


#define MAX_BUFFER_SIZE (2*1024)


void udhcp_init_header(struct dhcp_packet *packet, char type)
{
    if(PNULL == packet)
        return;
    DHCPS_MEMSET(packet,0,sizeof(struct dhcp_packet));
    packet->op = BOOTREQUEST; /* if client to a server */
	switch (type) {
	case DHCPOFFER:
	case DHCPACK:
	case DHCPNAK:
		packet->op = BOOTREPLY; /* if server to client */
	}
	packet->htype = 1; /* ethernet */
	packet->hlen = 6;
	packet->cookie = htonl(DHCP_MAGIC);
//#if(0 != DHCP_END)
	packet->options[0] = DHCP_END;
//#endif
	udhcp_add_simple_option(packet, DHCP_MESSAGE_TYPE, type);
}

#if defined CONFIG_UDHCP_DEBUG && CONFIG_UDHCP_DEBUG >= 2
void udhcp_dump_packet(struct dhcp_packet *packet)
{
	char buf[sizeof(packet->chaddr)*2 + 1];

    if (dhcp_verbose < 2)||(PNULL == packet)
		return;
    

	bb_info_msg(
		//" op %x"
		//" htype %x"
		" hlen %x"
		//" hops %x"
		" xid %x"
		//" secs %x"
		//" flags %x"
		" ciaddr %x"
		" yiaddr %x"
		" siaddr %x"
		" giaddr %x"
		//" chaddr %s"
		//" sname %s"
		//" file %s"
		//" cookie %x"
		//" options %s"
		//, packet->op
		//, packet->htype
		, packet->hlen
		//, packet->hops
		, packet->xid
		//, packet->secs
		//, packet->flags
		, packet->ciaddr
		, packet->yiaddr
		, packet->siaddr_nip
		, packet->gateway_nip
		//, packet->chaddr[16]
		//, packet->sname[64]
		//, packet->file[128]
		//, packet->cookie
		//, packet->options[]
	);
  *bin2hex(buf, (void *) packet->chaddr, sizeof(packet->chaddr)) = '\0';
	bb_info_msg(" chaddr %s", buf);
}
#endif

/* Read a packet from socket fd, return -1 on read error, -2 on packet error */
int udhcp_recv_kernel_packet(struct dhcp_packet *packet, int fd)
{
    int bytes = 0, left = 0;
    unsigned char* vendor = NULL;
    char buffer[MAX_BUFFER_SIZE] = {0};
    struct sci_sockaddr from_addr = {0};
    SOCKET_READ_EVENT_IND_SIG_T* sig_ptr = NULL;
    
    if( PNULL == packet )
        return bytes;

    DHCPS_MEMSET(packet, 0, sizeof(struct dhcp_packet));
    DHCPS_MEMSET(buffer, 0, MAX_BUFFER_SIZE);

    /* receive socket data */
    bytes = sci_sock_asyncrecvfrom(fd, buffer, MAX_BUFFER_SIZE, 0, &from_addr, &left);
    if( bytes < 0 )
    {
        DHCPS_TRACE("tcpip: dhcps socket - asyncrecvfrom error %d", sci_sock_errno(fd));
        return bytes;   /* returns -1 */
    }

    /* if still data left, send more SOCKET_READ_EVENT_IND to itself */
    if( left > 0 )
    {
        sig_ptr = TCPIP_ALLOC(sizeof(SOCKET_READ_EVENT_IND_SIG_T));
        if( NULL != sig_ptr )
        {
            TCPIP_MEMSET(sig_ptr, 0, sizeof(SOCKET_READ_EVENT_IND_SIG_T));
            sig_ptr->Sender = THIS_TASK;
            sig_ptr->SignalCode = SOCKET_READ_EVENT_IND;
            sig_ptr->SignalSize = sizeof(SOCKET_READ_EVENT_IND_SIG_T);
            sig_ptr->socket_id = fd;
            SCI_SendSignal((xSignalHeader)sig_ptr, THIS_TASK);
        }
        else
        {
            DHCPS_TRACE("tcpip: dhcps socket - alloc READ signal failed");
        }
    }
    
    DHCPS_MEMCPY(packet, buffer, sizeof(struct dhcp_packet));
    if( packet->cookie != htonl(DHCP_MAGIC) )
    {
        DHCPS_TRACE("tcpip: dhcps socket - packet with bad magic");
        return (-2);
    }
    
    DHCPS_TRACE("tcpip: dhcps - received packet, len = %d", bytes);
    
#if defined CONFIG_UDHCP_DEBUG && CONFIG_UDHCP_DEBUG >= 2
    udhcp_dump_packet(packet);
#endif

    if( packet->op == BOOTREQUEST )
    {
        vendor = udhcp_get_option(packet, DHCP_VENDOR);
        if( PNULL != vendor )
        {
#if 0
            static const char broken_vendors[][8] = {
                "MSFT 98",
                ""
            };
            int i;
            for (i = 0; broken_vendors[i][0]; i++)
            {
                if (vendor[OPT_LEN - OPT_DATA] == (uint8_t)strlen(broken_vendors[i])
                    && strncmp((char*)vendor, broken_vendors[i], vendor[OPT_LEN - OPT_DATA]) == 0
                {
                    log1("Broken client (%s), forcing broadcast replies",
                        broken_vendors[i]);
                    packet->flags |= htons(BROADCAST_FLAG);
                }
            }
#else
            if( vendor[OPT_LEN - OPT_DATA] == (uint8_t)(sizeof("MSFT 98")-1) &&
                0 == DHCPS_MEMCMP(vendor, "MSFT 98", sizeof("MSFT 98")-1) )
            {
                DHCPS_TRACE("tcpip:DHCPS - udhcp_recv_kernel_packet-Broken client (%s), forcing broadcast replies", "MSFT 98");
                packet->flags |= htons(BROADCAST_FLAG);
            }
#endif
        }
    }

    return bytes;
}

uint16_t udhcp_checksum(void *addr, int count)
{
	/* Compute Internet Checksum for "count" bytes
	 * beginning at location "addr".
	 */
	int32_t sum = 0;
	uint16_t *source = (uint16_t *) addr;

    if(PNULL == addr)
        return sum;
    
	while (count > 1)  {
		/*  This is the inner loop */
		sum += *source++;
		count -= 2;
	}

	/*  Add left-over byte, if any */
	if (count > 0) {
		/* Make sure that the left-over byte is added correctly both
		 * with little and big endian hosts */
		uint16_t tmp = 0;
		*(uint8_t*)&tmp = *(uint8_t*)source;
		sum += tmp;
	}
	/*  Fold 32-bit sum to 16 bits */
	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);

	return ~sum;
}

/* Construct a ip/udp header for a packet, send packet */
int udhcp_send_raw_packet(struct dhcp_packet *dhcp_pkt,
		uint32_t source_nip, int source_port,
		uint32_t dest_nip, int dest_port, const uint8_t *dest_arp,
		int ifindex)
{
    return 0;
}

/* Let the kernel do all the work for packet generation */
int udhcp_send_kernel_packet(struct dhcp_packet *dhcp_pkt,
		uint32_t source_nip, int source_port,
		uint32_t dest_nip, int dest_port)
{
	//struct sockaddr_in client;
    struct sci_sockaddr client = {0};
	unsigned padding;
	int fd;
	int result = -1;
	const char *msg;
    int status,error;

    if(PNULL == dhcp_pkt)
        return result;
        
    fd = sci_sock_socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP,0);
	if (fd < 0) {
		msg = "socket(%s)";
		goto ret_msg;
	}
	
	status = sci_sock_setsockopt(fd,SO_REUSEADDR,PNULL);

    if(status !=0) 
    {
        error = sci_sock_errno(fd);
        DHCPS_TRACE("tcpip:DHCPS - udhcp_send_kernel_packet-sci_sock_setsockopt error ***************%d", error);
        goto ret_msg;
    }

	DHCPS_MEMSET(&client, 0, sizeof(struct sci_sockaddr));
    client.family = AF_INET;
    client.port = htons(source_port);
    client.ip_addr =source_nip;

	if(sci_sock_bind(fd,&client,sizeof(struct sci_sockaddr)))
	 {
		msg = "bind(%s)";
		goto ret_close;
	}

	DHCPS_MEMSET(&client, 0, sizeof(struct sci_sockaddr));
    client.family = AF_INET;
	client.port = htons(dest_port);
	client.ip_addr = dest_nip; 

	if(sci_sock_connect(fd, (struct sci_sockaddr*)&client, sizeof(client)) == -1)
	{
		msg = "connect";
		goto ret_close;
	}

#if defined CONFIG_UDHCP_DEBUG && CONFIG_UDHCP_DEBUG >= 2

	udhcp_dump_packet(dhcp_pkt);
#endif

	padding = DHCP_OPTIONS_BUFSIZE - 1 - udhcp_end_option(dhcp_pkt->options);
	msg = "write";
 ret_close:	
	sci_sock_socketclose(fd);
	if (result < 0) {
 ret_msg:		
		DHCPS_TRACE("tcpip:DHCPS - udhcp_send_kernel_packet-socket(%s)","UDP");
	}
	return result;
}
