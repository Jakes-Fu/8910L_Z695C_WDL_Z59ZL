/*****************************************************************************
** File Name:       dhcps_api.h
** Author:          lisa.lin
** Date:            2011/07/26
** Copyright:       2011 Spreadtrum, Incorporated. All Rights Reserved.      
** Description:     dhcp server interface
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME                DESCRIPTION                          
** 2011/07/26       lisa.lin            create
** 2011/08/25       yifeng.wang         improve interfaces
******************************************************************************/

#ifndef  _DHCPS_API_H_
#define  _DHCPS_API_H_

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*
 *                          Include Files                                    *
 *---------------------------------------------------------------------------*/
#include "ipport.h"
#include "tcpip_types.h"
#include "app_tcp_if.h"
#include "net.h"

typedef struct TCPIP_DHCP_SERVER_CONFIG
{
    TCPIP_IPADDR_T server_ip;
    TCPIP_IPADDR_T start_ip;
    TCPIP_IPADDR_T end_ip;
    TCPIP_IPADDR_T subnet_mask;
    TCPIP_IPADDR_T dns1;
    TCPIP_IPADDR_T dns2;
    TCPIP_IPADDR_T router;
    uint32_t lease_time;
} TCPIP_DHCP_SERVER_CONFIG_T;

#define TCPIP_DHCPS_HOST_NAME_LEN_MAX 128


typedef struct TCPIP_DHCPS_HOST_INFO
{
    uint32_t hostip;
    char hostname[TCPIP_DHCPS_HOST_NAME_LEN_MAX];
} TCPIP_DHCPS_HOST_INFO_T;



/*---------------------------------------------------------------------------*
 *                          TYPE DEFINITION                                  *
 *---------------------------------------------------------------------------*/
typedef TCPIP_DHCP_SERVER_CONFIG_T  DHCP_SERVER_CONFIG_T;

/*---------------------------------------------------------------------------*
 *                          FUNCTION DEFINITIONS                             *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : create DHCP server
// Global : none
// Author : lisa.lin, yifeng.wang
// Note :         
/*****************************************************************************/
PUBLIC TCPIP_ERROR_E DHCPS_Open( NET netif_ptr );

/*****************************************************************************/
// Description : close DHCP server
// Global : none
// Author : lisa.lin, yifeng.wang
// Note :   
/*****************************************************************************/
PUBLIC void DHCPS_Close( void );

/*****************************************************************************/
// Description : init DHCP server
// Global : none
// Author : yifeng.wang
// Note :   RETURN: 0 - OK ; else - error
//          this function is called at TCPIP stack start-up
/*****************************************************************************/
PUBLIC int DHCPS_Init( void );

/*****************************************************************************/
// Description : clean-up DHCP server
// Global : none
// Author : yifeng.wang
// Note :   close DHCP server at system power down or stack exit
/*****************************************************************************/
PUBLIC void DHCPS_CleanUp( void );

/*****************************************************************************/
// Description : set DHCP server task ID
// Global : none
// Author : yifeng.wang
// Note :         
/*****************************************************************************/
PUBLIC BOOLEAN DHCPS_SetTaskId( BLOCK_ID task_id );

/*****************************************************************************/
// Description : get DHCP server task ID
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
PUBLIC BLOCK_ID DHCPS_GetTaskId(void);

/*****************************************************************************/
// Description : reset DHCP server task ID
// Global : none
// Author : yifeng.wang
// Note :         
/*****************************************************************************/
PUBLIC void DHCPS_ResetTaskId(void);

/*****************************************************************************/
// Description : get DHCP server net interface pointer
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
PUBLIC NET DHCPS_GetNetif( void );

/*****************************************************************************/
// Description : reset DHCP server net interface
// Global : none
// Author : yifeng.wang
// Note :         
/*****************************************************************************/
PUBLIC void DHCPS_ResetNetif( void );

/*****************************************************************************/
// Description : set DHCP server config parameters
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
PUBLIC BOOLEAN DHCPS_SetServerParam(const DHCP_SERVER_CONFIG_T* server_param_ptr);

/*****************************************************************************/
// Description : DHCPS Add static leases
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
PUBLIC BOOLEAN DHCPS_AddStaticLease(uint8 *mac_ptr,uint32 nip);

/*****************************************************************************/
// Description : DHCPS delete static leases
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
PUBLIC BOOLEAN DHCPS_DelStaticLease(uint8 *mac_ptr);

/*****************************************************************************/
// Description : Get host info (such as ip & host name)by mac address
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
PUBLIC int32 DHCPS_GetHostInfoByMac(uint8 *mac_ptr, TCPIP_DHCPS_HOST_INFO_T* host_info_ptr);

/*****************************************************************************/
// Description : get dhcp server open status
// Global : none
// Author : 
// Note :         
/*****************************************************************************/
PUBLIC BOOLEAN DHCPS_GetOpenStatus(void);

/*****************************************************************************/
// Description : set dhcp server open status
// Global : none
// Author : 
// Note :         
/*****************************************************************************/
PUBLIC void DHCPS_SetOpenStatus(BOOLEAN is_open);

/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef __cplusplus
};  /* end of extern "C" */
#endif
///////////////////////////////////////////////////////////////////////////////
#endif  /* _DHCPS_API_H_ */
