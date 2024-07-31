/*****************************************************************************
** File Name:       dhcps_api.c
** Author:          lisa.lin
** Date:            2011/07/26
** Copyright:       2011 Spreadtrum, Incorporated. All Rights Reserved.      
** Description:     dhcp server interface
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME                DESCRIPTION                          
** 2011/07/26       lisa.lin            create
** 2011/08/25       yifeng.wang         improve interfaces and robustness
******************************************************************************/

/*---------------------------------------------------------------------------*
 *                          Include Files                                    *
 *---------------------------------------------------------------------------*/
#include "in_message.h"
#include "app_tcp_if.h"
#include "tcpip_types.h"
#include "ipport.h"
#include "osport.h"
#include "net.h"
#include "dhcps_common.h"
#include "dhcpd.h"
#include "dhcps_api.h"


/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*---------------------------------------------------------------------------*
 *                          MACRO DEFINITION                                 *
 *---------------------------------------------------------------------------*/
/* DHCP serever task parameters */
#define DHCPS_STACK_SIZE        0x1400      /* 0x1400 = 5120 */
#define DHCPS_QUEUE_NUM         32
#define DHCPS_TASK_PRIORITY     NET_PRIORITY

#define DHCPS_STR_SUBNET        "subnet"
#define DHCPS_STR_DNS           "dns"
#define DHCPS_STR_ROUTER        "router"
#define DHCPS_STR_LEASE         "lease"


/*---------------------------------------------------------------------------*
 *                          TYPE DEFINITION                                  *
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
 *                          GLOBAL DEFINITION                                *
 *---------------------------------------------------------------------------*/
/* DHCP server task ID */
LOCAL BLOCK_ID s_dhcps_task_id = 0;

/* DHCP server net interface */
LOCAL NET s_dhcps_netif_ptr = PNULL;

LOCAL BOOLEAN s_is_dhcps_open = FALSE;

/*---------------------------------------------------------------------------*
 *                          LOCAL FUNCTION DECLARE                           *
 *---------------------------------------------------------------------------*/
LOCAL void SetIpField(TCPIP_IPADDR_T start_ip, TCPIP_IPADDR_T end_ip);
LOCAL BOOLEAN SetSubnetMask(TCPIP_IPADDR_T subnet_mask);
LOCAL BOOLEAN SetDNS(TCPIP_IPADDR_T dns1,TCPIP_IPADDR_T dns2);
LOCAL BOOLEAN SetRouter(TCPIP_IPADDR_T router);
LOCAL BOOLEAN SetLeaseTime(uint32 t_sec);
LOCAL void SetServerIp(TCPIP_IPADDR_T server_ip);
LOCAL BOOLEAN IsNeedCleanLeaseInfo(const DHCP_SERVER_CONFIG_T* server_param_ptr);


/*---------------------------------------------------------------------------*
 *                          FUNCTION DEFINITIONS                             *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description: create DHCP server
// Global : none
// Author : lisa.lin, yifeng.wang
// Note :         
/*****************************************************************************/
PUBLIC TCPIP_ERROR_E DHCPS_Open( NET netif_ptr )
{
    /* check input */
    if( PNULL == netif_ptr )
    {
        DHCPS_TRACE("tcpip: DHCPS_Open - invalid netif");
        return TCPIP_ERROR_INVALPARAM;
    }

    /* check server task */
    if( 0 != s_dhcps_task_id && INVALID_TASK != s_dhcps_task_id )
    {
        DHCPS_TRACE("tcpip: DHCPS_Open - server task is already exist %x", s_dhcps_task_id);
        return TCPIP_ERROR_LOGICAL;
    }

    /* set DHCP server net interface */
    s_dhcps_netif_ptr = netif_ptr;
    DHCPS_SetOpenStatus(FALSE);
    
    /* create DHCP server task */
    s_dhcps_task_id = SCI_CreateAppThread( 
        "T_DHCP_SERVER", 
        "Q_DHCP_SERVER", 
        udhcpd_main, 
        0, 
        0, 
        DHCPS_STACK_SIZE, 
        DHCPS_QUEUE_NUM, 
        DHCPS_TASK_PRIORITY, 
        SCI_PREEMPT, 
        SCI_AUTO_START);
        
    /* check task create result */
    if( INVALID_TASK != s_dhcps_task_id )
    {
        DHCPS_TRACE("tcpip: create DHCP server task with ID : %d", s_dhcps_task_id);
        DHCPS_TRACE("tcpip: priority of DHCP server is : %d\n", DHCPS_TASK_PRIORITY);
        DHCPS_SetOpenStatus(TRUE);
        return TCPIP_ERROR_OK;
    }
    else
    {
        DHCPS_TRACE("tcpip: create DHCP server task failed!");
        s_dhcps_netif_ptr = PNULL;
        DHCPS_SetOpenStatus(FALSE);
        return TCPIP_ERROR_LOGICAL;
    }
}

/*****************************************************************************/
// Description: close DHCP server
// Global : none
// Author : lisa.lin, yifeng.wang
// Note :         
/*****************************************************************************/
PUBLIC void DHCPS_Close( void )
{ 
    xSignalHeaderRec *sig_ptr = PNULL;

    /* check DHCP server task */
    if( 0 == s_dhcps_task_id || INVALID_TASK == s_dhcps_task_id )
    {
        DHCPS_TRACE("tcpip: DHCPS_Close - server task is already closed");
        return;
    }
    
    /* create signal */
    sig_ptr = (xSignalHeaderRec *)DHCPS_ALLOC(sizeof(xSignalHeaderRec));
    if( PNULL == sig_ptr )
    {
        DHCPS_TRACE("tcpip: DHCPS_Close - alloc signal failed");
        return;
    }

    /* close server socket */
    DHCPS_TRACE("tcpip: DHCPS_Close - close DHCP server socket");
    sci_sock_socketclose( udhcp_get_socketid() );

    /* send signal(SOCKET_FULL_CLOSED_EVENT_IND)manully to dhcp server task to exit */
    DHCPS_MEMSET(sig_ptr, 0, sizeof(xSignalHeaderRec));
    sig_ptr->SignalCode = SOCKET_FULL_CLOSED_EVENT_IND;
    sig_ptr->SignalSize = sizeof(xSignalHeaderRec);
    sig_ptr->Sender     = THIS_TASK;
    SCI_SendSignal(sig_ptr, s_dhcps_task_id);
}

/*****************************************************************************/
// Description : init DHCP server - dummy function
// Global : none
// Author : yifeng.wang
// Note :   RETURN: 0 - OK ; else - error
//          this function is called at TCPIP stack start-up
/*****************************************************************************/
PUBLIC int DHCPS_Init( void )
{
    return 0;
}

/*****************************************************************************/
// Description : clean-up DHCP server - dummy function
// Global : none
// Author : yifeng.wang
// Note :   close DHCP server at system power down or stack exit
/*****************************************************************************/
PUBLIC void DHCPS_CleanUp( void )
{
    return;
}

/*****************************************************************************/
// Description : set DHCP server task ID
// Global : none
// Author : yifeng.wang
// Note :         
/*****************************************************************************/
PUBLIC BOOLEAN DHCPS_SetTaskId( BLOCK_ID task_id )
{
    if( 0 == task_id || INVALID_TASK == task_id )
        return FALSE;

    s_dhcps_task_id = task_id;
    return TRUE;
}

/*****************************************************************************/
// Description : Get DHCP server task ID
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
PUBLIC BLOCK_ID DHCPS_GetTaskId( void )
{
    return s_dhcps_task_id;
}

/*****************************************************************************/
// Description : reset DHCP server task ID
// Global : none
// Author : yifeng.wang
// Note :         
/*****************************************************************************/
PUBLIC void DHCPS_ResetTaskId(void)
{
    s_dhcps_task_id = 0;
}

/*****************************************************************************/
// Description : Get DHCP server net interface pointer
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
PUBLIC NET DHCPS_GetNetif( void )
{
    return s_dhcps_netif_ptr;
}

/*****************************************************************************/
// Description : reset DHCP server net interface
// Global : none
// Author : yifeng.wang
// Note :         
/*****************************************************************************/
PUBLIC void DHCPS_ResetNetif( void )
{
    s_dhcps_netif_ptr = PNULL;
}

/**************************************************************************/
// Description : set DHCP server config parameters
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
PUBLIC BOOLEAN DHCPS_SetServerParam(const DHCP_SERVER_CONFIG_T* cfg_ptr)
{
    BOOLEAN is_need_clean = FALSE;
    BOOLEAN ret_val = FALSE;
    
    /* check input */
    if( PNULL == cfg_ptr )
    {
        DHCPS_TRACE("tcpip: DHCPS_SetServerParam - input is NULL");
        return FALSE;
    }        

    /* start_ip should less than end_ip */
    if( ntohl(cfg_ptr->start_ip) > ntohl(cfg_ptr->end_ip) )
    {
        DHCPS_TRACE("tcpip: DHCPS_SetServerParam - start_ip > end_ip!");
        return FALSE;
    }

    LOCK_NET_RESOURCE(NET_RESID);
    
    /* check if need to clear lease information */
    is_need_clean = IsNeedCleanLeaseInfo(cfg_ptr);
    DHCPS_TRACE("tcpip: DHCPS_SetServerParam - is_need_clean = %d", is_need_clean);

    if( is_need_clean )
    {
        struct server_config_t* server_cfg_ptr = DHCPS_GetServerConfig();
        struct dyn_lease* leases_info_ptr = DHCPS_GetLeaseInfo();
        release_static_leases(&(server_cfg_ptr->static_leases));
        if( PNULL != leases_info_ptr )
        {
           DHCPS_MEMSET(leases_info_ptr, 0, server_cfg_ptr->max_leases * sizeof(leases_info_ptr[0]));
        }
    }
 
    
    /* set start ip and end ip */
    SetIpField(cfg_ptr->start_ip, cfg_ptr->end_ip);
    /* set server ip */
    SetServerIp(cfg_ptr->server_ip);
    /* set subnet mask */
    if(!SetSubnetMask(cfg_ptr->subnet_mask))
        goto END;
    /* set dns servers */    
    if(!SetDNS(cfg_ptr->dns1, cfg_ptr->dns2))
        goto END;
    /* set router */
    if(!SetRouter(cfg_ptr->router))
        goto END;
    /* set default DHCP lease time */
    SetLeaseTime(cfg_ptr->lease_time);   

    DHCPS_TRACE("tcpip: DHCPS_SetServerParam - server_ip = %s", inet_ntoa(cfg_ptr->server_ip));
    DHCPS_TRACE("tcpip: - client_start = %s, client_end = %s",
        inet_ntoa(cfg_ptr->start_ip), inet_ntoa(cfg_ptr->end_ip));
    DHCPS_TRACE("tcpip: - subnet_mask = %s, router = %s",
        inet_ntoa(cfg_ptr->subnet_mask), inet_ntoa(cfg_ptr->router));
    DHCPS_TRACE("tcpip: - dns1 = %s, dns2 = %s", 
        inet_ntoa(cfg_ptr->dns1), inet_ntoa(cfg_ptr->dns2));
    DHCPS_TRACE("tcpip: - lease_time = %ds", cfg_ptr->lease_time);
    ret_val = TRUE;

END:
    UNLOCK_NET_RESOURCE(NET_RESID);
    return ret_val;
}

/*****************************************************************************/
// Description: DHCPS Add static leases
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
PUBLIC BOOLEAN DHCPS_AddStaticLease(uint8 *mac_ptr, uint32 nip)
{
    struct static_lease*    st_lease_ptr = PNULL;
    struct server_config_t* server_config_ptr = PNULL;
    TCPIP_IPADDR_T          server_ip = 0;
    TCPIP_IPADDR_T          subnet_mask = 0;
    BOOLEAN                 ret = FALSE;

    if( PNULL == mac_ptr || TCPIP_IP_NULL_ADDR == nip )
    {
        DHCPS_TRACE("tcpip dhcps: DHCPS_AddStaticLease - invalid input");
        return FALSE;
    }
    
    LOCK_NET_RESOURCE(NET_RESID);

    /* get current server configuration */
    server_config_ptr = DHCPS_GetServerConfig();   
    st_lease_ptr    = server_config_ptr->static_leases;
    server_ip       = server_config_ptr->server_nip;
    subnet_mask     = server_config_ptr->subnet_mask;

    /* new lease IP should be the same subnet address as sever IP */
    if( (server_ip & subnet_mask) != (nip & subnet_mask) )
    {
        DHCPS_TRACE("tcpip dhcps: DHCPS_AddStaticLease - ip %s is illegal", inet_ntoa(nip));
        goto END;
    }

    /* replace the old one */
    while( PNULL != st_lease_ptr )
    {
        /* if MAC exist, update IP address */
        if( 0 == DHCPS_MEMCMP(st_lease_ptr->mac, mac_ptr, MAC_ADDR_LEN) )
        {
            DHCPS_TRACE("tcpip dhcps: replace lease - MAC %02X:%02X:%02X:%02X:%02X:%02X, IP %s",
                mac_ptr[0], mac_ptr[1], mac_ptr[2], mac_ptr[3], mac_ptr[4], mac_ptr[5], inet_ntoa(nip));
            st_lease_ptr->nip = nip;
            goto END;
        }

        /* if NIP exist, but MAC not match, return fail */
        if( st_lease_ptr->nip == nip )
        {
            mac_ptr = st_lease_ptr->mac;
            DHCPS_TRACE("tcpip dhcps: IP %s is already bound to MAC %02X:%02X:%02X:%02X:%02X:%02X",
                inet_ntoa(nip), mac_ptr[0], mac_ptr[1], mac_ptr[2], mac_ptr[3], mac_ptr[4], mac_ptr[5]);
            goto END;
        }
        st_lease_ptr = st_lease_ptr->next;
    }

    /* add a new lease */
    add_static_lease(&(server_config_ptr->static_leases), mac_ptr, nip);
    ret = TRUE;

END:
    UNLOCK_NET_RESOURCE(NET_RESID);
    return ret;
}

/*****************************************************************************/
// Description: DHCPS delete static leases
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
PUBLIC BOOLEAN DHCPS_DelStaticLease(uint8 *mac_ptr)
{
    BOOLEAN ret = FALSE;
    struct server_config_t* server_config_ptr = PNULL; 
    
    if( PNULL == mac_ptr )
        return FALSE;
    
    LOCK_NET_RESOURCE(NET_RESID);
    server_config_ptr = DHCPS_GetServerConfig();
    DHCPS_TRACE("tcpip dhcps: delete lease by MAC %02X:%02X:%02X:%02X:%02X:%02x",
        mac_ptr[0], mac_ptr[1], mac_ptr[2], mac_ptr[3], mac_ptr[4], mac_ptr[5]);
    ret = delete_static_lease_by_mac(&(server_config_ptr->static_leases), mac_ptr);
    UNLOCK_NET_RESOURCE(NET_RESID);
    return ret;
}


/*****************************************************************************/
// Description : set DHCP server parameters : start ip and end ip
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
LOCAL void SetIpField(TCPIP_IPADDR_T start_ip, TCPIP_IPADDR_T end_ip)
{
    struct server_config_t* cfg_ptr = DHCPS_GetServerConfig();
    if( NULL != cfg_ptr )
    {
        cfg_ptr->start_ip = start_ip;
        cfg_ptr->end_ip   = end_ip;   
    }
}

/*****************************************************************************/
// Description : set DHCP server parameters : subnet mask
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
LOCAL BOOLEAN SetSubnetMask(TCPIP_IPADDR_T subnet_mask)
{  
    struct option_set*      opt_ptr = PNULL;
    struct server_config_t* cfg_ptr = PNULL;
    int                     data_len = 0;
   
    cfg_ptr = DHCPS_GetServerConfig();
    opt_ptr = udhcp_find_option(cfg_ptr->options, DHCP_SUBNET);
    
    if( PNULL == opt_ptr )
    {
        /* construct option line */
        char* str_ptr = PNULL;
        uint8 str_len = 0;

        /* string format sample : "subnet 255.255.255.0" */
        /* string length : add 2 bytes = 1 space + terminal char */
        str_len = strlen(DHCPS_STR_SUBNET) + strlen(inet_ntoa(subnet_mask)) + 2;

        /* allocate string buffer */
        str_ptr = (char*)DHCPS_ALLOC(str_len);
        if( PNULL == str_ptr )
        {
            DHCPS_TRACE("tcpip dhcps: SetSubnetMask - alloc memory failed");
            return FALSE;
        }

        /* format subnet mask string */
        DHCPS_MEMSET(str_ptr, 0, str_len);
         _snprintf(str_ptr, str_len, "%s %s", DHCPS_STR_SUBNET, inet_ntoa(subnet_mask));
        str_ptr[str_len-1] = 0;

        /* save the string */
        udhcp_str2optset(str_ptr, &(cfg_ptr->options));
        
        DHCPS_FREE(str_ptr);       
        str_ptr = PNULL;
    }
    else
    {            
        data_len = opt_ptr->data[OPT_LEN];            
        DHCPS_MEMSET(opt_ptr->data+OPT_DATA, 0, data_len);
        DHCPS_MEMCPY(opt_ptr->data+OPT_DATA, (uint8*)&subnet_mask, data_len);    
    }   

    cfg_ptr->subnet_mask = subnet_mask;
    
    return TRUE;
}

/*****************************************************************************/
// Description : set DHCP server parameters : DNS servers
// Global : none
// Author : lisa.lin
// Note :        
/*****************************************************************************/
LOCAL BOOLEAN SetDNS(TCPIP_IPADDR_T dns1, TCPIP_IPADDR_T dns2)
{   
    struct option_set*      opt_ptr = PNULL;
    struct server_config_t* cfg_ptr = PNULL;
    int                     data_len = 0;

    cfg_ptr = DHCPS_GetServerConfig();
    opt_ptr = udhcp_find_option(cfg_ptr->options, DHCP_DNS_SERVER);

    if( PNULL == opt_ptr )
    {
        /* construct option line */
        char* dns_ptr = PNULL;
        uint8 str_len = 0;

        /* string format sample : "dns 192.168.1.1 192.168.1.2" */
        /* string length : add 3 bytes = 2 space + terminal char */
        str_len = strlen(DHCPS_STR_DNS) + 
                  strlen(inet_ntoa(dns1)) + strlen(inet_ntoa(dns2)) + 3;

        /* allocate string buffer */
        dns_ptr = (char*)DHCPS_ALLOC(str_len);
        if( PNULL == dns_ptr )
        {
            DHCPS_TRACE("tcpip dhcps: SetDNS - alloc memory failed");
            return FALSE;
        }

        /* format DNS string */
        DHCPS_MEMSET(dns_ptr, 0, str_len);
        _snprintf(dns_ptr, str_len, "%s %s %s", DHCPS_STR_DNS, inet_ntoa(dns1), inet_ntoa(dns2));
        dns_ptr[str_len-1] = 0;
        
        /* save DNS string */
        udhcp_str2optset(dns_ptr, &(cfg_ptr->options));
        
        DHCPS_FREE(dns_ptr);
        dns_ptr = PNULL;
    }
    else
    {   
        data_len = opt_ptr->data[OPT_LEN];            
        DHCPS_MEMSET(opt_ptr->data+OPT_DATA, 0, data_len);
        DHCPS_MEMCPY(opt_ptr->data+OPT_DATA, (uint8*)&dns1, OPT_IP_FIELD_LEN);                  
        
        DHCPS_MEMSET(opt_ptr->data+OPT_DATA+OPT_IP_FIELD_LEN, 0, OPT_IP_FIELD_LEN);
        DHCPS_MEMCPY(opt_ptr->data+OPT_DATA+OPT_IP_FIELD_LEN, (uint8*)&dns2, OPT_IP_FIELD_LEN);       
    }
    
    return TRUE;
}

/*****************************************************************************/
// Description : set DHCP server parameters : router
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
LOCAL BOOLEAN SetRouter(TCPIP_IPADDR_T router)
{
    struct option_set*      opt_ptr = PNULL;
    struct server_config_t* cfg_ptr = PNULL;
    int                     data_len = 0;

    cfg_ptr = DHCPS_GetServerConfig();        
    opt_ptr = udhcp_find_option(cfg_ptr->options, DHCP_ROUTER);
    
    if( PNULL == opt_ptr )
    {
        /* construct option line */
        char* str_ptr = PNULL;  
        uint8 str_len = 0;

        /* string format : router 192.168.1.1 */
        /* string length : add 2 bytes = 1 space + terminal char */
        str_len = strlen(DHCPS_STR_ROUTER) + strlen((char*)inet_ntoa(router)) + 2;
        str_ptr = (char*)DHCPS_ALLOC(str_len);        
        if( PNULL == str_ptr )
        {
            DHCPS_TRACE("tcpip dhcps: SetRouter - alloc memory failed");
            return FALSE;
        }
        
        /* format router string */
        DHCPS_MEMSET(str_ptr, 0, str_len);
        _snprintf(str_ptr, str_len, "%s %s", DHCPS_STR_ROUTER, inet_ntoa(router));
        str_ptr[str_len-1] = 0;

        /* save router string */
        udhcp_str2optset(str_ptr, &(cfg_ptr->options));
        
        DHCPS_FREE(str_ptr);   
        str_ptr = PNULL;
    }
    else
    {      
        data_len = opt_ptr->data[OPT_LEN];            
        DHCPS_MEMSET(opt_ptr->data+OPT_DATA, 0, data_len);
        DHCPS_MEMCPY(opt_ptr->data+OPT_DATA, (uint8*)&router, data_len);       
    }
    
    return TRUE;
}

/*****************************************************************************/
// Description: set DHCP server parameters : default lease time
// Global : none
// Author : 
// Note :         
/*****************************************************************************/
LOCAL BOOLEAN SetLeaseTime(uint32 t_sec)
{   
    struct option_set*      opt_ptr = PNULL;
    struct server_config_t* cfg_ptr = PNULL;
    int                     data_len = 0;

    cfg_ptr = DHCPS_GetServerConfig();
    opt_ptr = udhcp_find_option(cfg_ptr->options, DHCP_LEASE_TIME);
    
    if( PNULL == opt_ptr )
    {
        /* construct option line */
        char* str_ptr = PNULL;
        uint8 str_len = 0;
        char  sec_str[20]={0};

        sprintf(sec_str,"%d",t_sec);

        /* string format : lease 2345 */
        /* string length : add 2 bytes = 1 space + terminal char */
        str_len = strlen(DHCPS_STR_LEASE) + strlen(sec_str) + 2;
        str_ptr = (char*)DHCPS_ALLOC(str_len);
        if( PNULL == str_ptr )
        {
            DHCPS_TRACE("tcpip dhcps: SetLeaseTime - alloc memory failed");
            return FALSE;
        }

        /* format lease string */
        DHCPS_MEMSET(str_ptr, 0, str_len);
        _snprintf(str_ptr, str_len, "%s %s", DHCPS_STR_LEASE, sec_str);
        str_ptr[str_len-1] = 0;

        /* save lease string */
        udhcp_str2optset(str_ptr, &(cfg_ptr->options));
        
        DHCPS_FREE(str_ptr);
        str_ptr = PNULL;
    }
    else
    {      
        data_len = opt_ptr->data[OPT_LEN];
        DHCPS_MEMSET(opt_ptr->data+OPT_DATA, 0, data_len);
        DHCPS_MEMCPY(opt_ptr->data+OPT_DATA, (uint8*)&t_sec, data_len);
    }

    return TRUE;
}

/*****************************************************************************/
// Description: Set Server Ip
// Global : none
// Author : 
// Note :         
/*****************************************************************************/
LOCAL void SetServerIp(TCPIP_IPADDR_T server_ip)
{   
    struct server_config_t* server_config_ptr = PNULL;
    
    server_config_ptr = DHCPS_GetServerConfig();
    server_config_ptr->server_nip = server_ip;
}

/*****************************************************************************/
// Description : Get host info (such as ip & host name)by mac address
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
PUBLIC int32 DHCPS_GetHostInfoByMac(
    uint8 *mac_ptr, TCPIP_DHCPS_HOST_INFO_T* host_info_ptr)
{
    struct dyn_lease* lease_ptr = PNULL; 
    
    if( PNULL == mac_ptr || PNULL == host_info_ptr )
        return ENP_PARAM;
    
    LOCK_NET_RESOURCE(NET_RESID);
    /* get host info from lease table */
    lease_ptr = find_lease_by_mac(mac_ptr);
    if( PNULL != lease_ptr )
    {
        host_info_ptr->hostip = lease_ptr->lease_nip;
        DHCPS_MEMSET(host_info_ptr->hostname, 0x0, sizeof(host_info_ptr->hostname));
        strncpy(host_info_ptr->hostname, lease_ptr->hostname, TCPIP_DHCPS_HOST_NAME_LEN_MAX);            
        UNLOCK_NET_RESOURCE(NET_RESID);
        return SUCCESS;
    }
    UNLOCK_NET_RESOURCE(NET_RESID);
    return ENP_NOT_ALLOWED;//ENP_NO_EXIST;
}


/*****************************************************************************/
// Description : set dhcp server open status
// Global : none
// Author : 
// Note :         
/*****************************************************************************/
PUBLIC void DHCPS_SetOpenStatus(BOOLEAN is_open)
{
    s_is_dhcps_open = is_open;
}
/*****************************************************************************/
// Description : get dhcp server open status
// Global : none
// Author : 
// Note :         
/*****************************************************************************/
PUBLIC BOOLEAN DHCPS_GetOpenStatus(void)
{
    return s_is_dhcps_open;
}

/*****************************************************************************/
// Description : check if need to clean DHCP server leases information
// Global : none
// Author : 
// Note :         
/*****************************************************************************/
LOCAL BOOLEAN IsNeedCleanLeaseInfo(const DHCP_SERVER_CONFIG_T* server_param_ptr)
{
    BOOLEAN                 is_need_clean = FALSE;
    struct option_set*      existing_ptr = PNULL;
    struct server_config_t* server_config_ptr = PNULL;    

    if( PNULL == server_param_ptr )
        return FALSE;
    
    server_config_ptr = DHCPS_GetServerConfig();

    /* check IP address */
    if( (server_config_ptr->start_ip   != server_param_ptr->start_ip) ||
        (server_config_ptr->end_ip     != server_param_ptr->end_ip)   ||
        (server_config_ptr->server_nip != server_param_ptr->server_ip))
    {
        is_need_clean = TRUE;
    }

    /* check if subnet mask changed */
    existing_ptr = udhcp_find_option(server_config_ptr->options, DHCP_SUBNET);
    if( PNULL != existing_ptr )
    {          
        if( 0 != DHCPS_MEMCMP((existing_ptr->data + OPT_DATA),
                              (uint8*)&server_param_ptr->subnet_mask,
                              existing_ptr->data[OPT_LEN]) )
        {
            is_need_clean = TRUE;  
        }
    }
    else
    {   
        is_need_clean = TRUE;
    }

    /* check if DNS changed */ 
    existing_ptr = udhcp_find_option(server_config_ptr->options, DHCP_DNS_SERVER);
    if( PNULL != existing_ptr )
    {
        if( 0 != DHCPS_MEMCMP(existing_ptr->data+OPT_DATA,
                              (uint8*)&server_param_ptr->dns1,
                              OPT_IP_FIELD_LEN) 
         || 0 != DHCPS_MEMCMP(existing_ptr->data+OPT_DATA+OPT_IP_FIELD_LEN,
                              (uint8*)&server_param_ptr->dns2,
                              OPT_IP_FIELD_LEN))
        {
            is_need_clean = TRUE;  
        }
    }
    else
    {   
        is_need_clean = TRUE;
    }

    /* check if router changed */  
    existing_ptr = udhcp_find_option(server_config_ptr->options, DHCP_ROUTER);
    if( PNULL != existing_ptr )
    {           
        if(0 != DHCPS_MEMCMP(existing_ptr->data+OPT_DATA,
                             (uint8 *)&server_param_ptr->router,
                             existing_ptr->data[OPT_LEN]))
        {
            is_need_clean = TRUE;  
        }
    }
    else
    {   
        is_need_clean = TRUE;
    }
    
    return is_need_clean;
}


/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

