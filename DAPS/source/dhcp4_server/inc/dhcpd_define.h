/***************************************************************************
** File Name:      dhcpd_define.h
** Author:         lisa.lin
** Date:           2011/07/26
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.
** Description:    DHCP server type and const definitions
****************************************************************************
**                         Important Edit History                   
** ------------------------------------------------------------------------*
** DATE            NAME             DESCRIPTION                    
** 2011/07/26      lisa.lin         Create
****************************************************************************/

#ifndef  _DHCPD_DEFINE_H_    
#define  _DHCPD_DEFINE_H_


#include "ipport.h"
#include "dhcps_common.h"


/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/*---------------------------------------------------------------------------*
 *                          MACRO DEFINITION                                 *
 *---------------------------------------------------------------------------*/
/* DHCP server macros */
#define DHCPS_TRACE     TCPIP_TRACE
#define DHCPS_DEBUG     TCPIP_DEBUG
#define DHCPS_ASSERT    TCPIP_ASSERT

#define DHCPS_ALLOC     TCPIP_ALLOC
#define DHCPS_FREE      TCPIP_FREE
#define DHCPS_MEMCPY    TCPIP_MEMCPY
#define DHCPS_MEMSET    TCPIP_MEMSET
#define DHCPS_MEMCMP    MEMCMP

#define bb_info_msg     DHCPS_TRACE

#define MAC_ADDR_LEN    6

/* address to send to all hosts. */
#define	INADDR_BROADCAST    ((uint32_t)0xFFFFFFFF)

#define CLIENT_PORT     68

#define PF_INET         AF_INET
#define IPPROTO_UDP     17  /* UDP protocol */


/*---------------------------------------------------------------------------*
 *                          TYPE DEFINITION                                  *
 *---------------------------------------------------------------------------*/
typedef uint8   uint8_t;
typedef uint16  uint16_t;
typedef uint32  uint32_t;
typedef int32   int32_t;

typedef uint8   u_char;
typedef uint16  u_short;
typedef uint32  u_int;
typedef uint32  u_long;


/* ethernet specific structure */
typedef struct ether_addr
{
    uint8_t ether_addr_octet[MAC_ADDR_LEN]; 
}ETHER_ADDR;


/* Internet address. */
struct in_addr {
    uint32  s_addr;
};

#define move_to_unaligned32(u32p, v) do { \
    uint32_t __t = (v); \
    DHCPS_MEMCPY((u32p), &__t, 4); \
} while (0)


/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
#endif /* _DHCPD_DEFINE_H_ */

