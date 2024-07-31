/* vi: set sw=4 ts=4: */
/*
 * Storing and retrieving data for static leases
 *
 * Wade Berrier <wberrier@myrealbox.com> September 2004
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */

//#include "ipport.h"
#include "dhcps_common.h"
#include "dhcpd.h"
#include "socket_api.h"

/* Takes the address of the pointer to the static_leases linked list,
 * address to a 6 byte mac address,
 * 4 byte IP address */
void add_static_lease(struct static_lease **st_lease_pp,
        uint8_t *mac,
        uint32_t nip)
{
    struct static_lease *st_lease;

    /* Find the tail of the list */
    while( NULL != (st_lease = *st_lease_pp) )
    {
        st_lease_pp = &st_lease->next;
    }

    /* Add new node */
    *st_lease_pp = st_lease =(struct static_lease*)DHCPS_ALLOC(sizeof(*st_lease));
    if( PNULL == st_lease )
        return;

    DHCPS_MEMSET(st_lease, 0, sizeof(sizeof(*st_lease)));
    DHCPS_MEMCPY(st_lease->mac, mac, MAC_ADDR_LEN);
    st_lease->nip = nip;
    st_lease->next = NULL;
    DHCPS_TRACE("tcpip dhcps: add static lease, MAC %02X:%02X:%02X:%02X:%02X:%02X, IP %s",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], inet_ntoa(nip));
}


/* Find static lease IP by mac */
uint32_t get_static_nip_by_mac(struct static_lease *st_lease, void *mac)
{
    if( (PNULL == st_lease) || (PNULL == mac) )
        return 0;
        
    while (st_lease)
    {
        if( 0 == DHCPS_MEMCMP(st_lease->mac, mac, MAC_ADDR_LEN) )
            return st_lease->nip;
        st_lease = st_lease->next;
    }
    return 0;
}


/* Check to see if an IP is reserved as a static IP */
int is_nip_reserved(struct static_lease *st_lease, uint32_t nip)
{
    while (st_lease)
    {
        if (st_lease->nip == nip)
            return 1;
        st_lease = st_lease->next;
    }
    return 0;
}


#if defined CONFIG_UDHCP_DEBUG && CONFIG_UDHCP_DEBUG >= 2
/* Print out static leases just to check what's going on */
/* Takes the address of the pointer to the static_leases linked list */
void log_static_leases(struct static_lease **st_lease_pp)
{
    struct static_lease *cur;

    if (dhcp_verbose < 2)
        return;

    cur = *st_lease_pp;
    while (cur)
    {
        DHCPS_TRACE("tcpip dhcps: print static lease, MAC %02X:%02X:%02X:%02X:%02X:%02X, IP %s",
            cur->mac[0], cur->mac[1], cur->mac[2], cur->mac[3], cur->mac[4], cur->mac[5],
            inet_ntoa(cur->nip));
        cur = cur->next;
    }
}
#endif


/* delete static lease IP by mac */
BOOLEAN delete_static_lease_by_mac(struct static_lease **st_lease, void *mac)
{
    struct static_lease* p = PNULL;
    struct static_lease* q = PNULL;

    p = *st_lease;
    if( (PNULL == p) || (PNULL == mac) )
        return FALSE;

    q = p->next;

    if( 0 == DHCPS_MEMCMP(p->mac, mac, MAC_ADDR_LEN) )
    {
        DHCPS_TRACE("tcpip dhcps: del static lease, MAC %02X:%02X:%02X:%02X:%02X:%02X, IP %s",
            p->mac[0], p->mac[1], p->mac[2], p->mac[3], p->mac[4], p->mac[5], inet_ntoa(p->nip));
        DHCPS_FREE(p);        
        *st_lease = q;
        p = PNULL;
        return TRUE;      
    }    
        
    while( NULL != q )
    {
        if( 0 == DHCPS_MEMCMP(q->mac, mac, MAC_ADDR_LEN) )
        {
            DHCPS_TRACE("tcpip dhcps: del static lease, MAC %02X:%02X:%02X:%02X:%02X:%02X, IP %s",
                q->mac[0], q->mac[1], q->mac[2], q->mac[3], q->mac[4], q->mac[5], inet_ntoa(q->nip));
            p->next = q->next;           
            DHCPS_FREE(q);
            q = PNULL;
            return TRUE;    
        }
        p = q;
        q = q->next;
    }

    return FALSE;
}

/*release all static leases*/
void release_static_leases(struct static_lease **st_lease)
{
     struct static_lease * cur,*next;

     cur = *st_lease;     

     while(cur)
     {
        next = cur->next;        
        DHCPS_FREE(cur);
        cur = next;
     }

     DHCPS_TRACE("tcpip dhcps: release all static lease");
     *st_lease = PNULL;
}


