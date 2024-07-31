/* vi: set sw=4 ts=4: */
/*
 * DHCP server config and lease file manipulation
 *
 * Rewrite by Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */
//#include <netinet/ether.h>

#include <string.h>
#include "ipport.h"
#include "dhcps_common.h"
#include "dhcpd.h"
#include "dhcpd_define.h"


#define ARRAY_SIZE(a)	(sizeof(a) / sizeof((a)[0]))

/* on these functions, make sure your datatype matches */
static int read_str(const char *line, void *arg)
{
    char **dest = arg;

    if((PNULL == line)||(PNULL == arg))
    {   
        DHCPS_TRACE("tcpip:DHCPS -read_str - arg is null\n ");
        return 0;
    }

    DHCPS_FREE(*dest);
    *dest =(char *)DHCPS_ALLOC(strlen(line)+1);
    if(PNULL == *dest)
    {   
        DHCPS_TRACE("tcpip:DHCPS -read_str - alloc failed\n ");
        return 0;
    }
    
    DHCPS_MEMSET(*dest,0,strlen(line)+1);
    strcpy(*dest,line);    
    return 1;
}

static int read_u32(const char *line, void *arg)
{
    if((PNULL == line)||(PNULL == arg))
    {
        DHCPS_TRACE("tcpip:DHCPS -read_u32 - arg is null\n ");
        return 0;
    }
    *(uint32_t*)arg = atoi(line);
    return 1;
}

static int read_staticlease(const char *const_line, void *arg)
{
    char *line;
    char *mac_string;
    char *ip_string;
    ETHER_ADDR mac_bytes;
    uint32_t nip;

    if((PNULL == const_line)||(PNULL == arg))
    {
        DHCPS_TRACE("tcpip:DHCPS -read_staticlease - arg is null\n ");
        return 0;
    }
    /* Read mac */
    line = (char*)DHCPS_ALLOC(strlen(const_line)+1);
    if(line ==  PNULL)
        return 0;

    DHCPS_MEMSET(line,0,strlen(const_line)+1);    
    DHCPS_MEMCPY(line,const_line,strlen(const_line));   
    mac_string = strtok(line, " \t");

    if ((PNULL == mac_string) || (PNULL == ether_aton_r(mac_string, &mac_bytes)))
    {
        DHCPS_TRACE("tcpip:DHCPS -read_staticlease - not valid mac string\n ");
        DHCPS_FREE(line);
        line = PNULL;
        return 0;
    }

    /* Read ip */	
    ip_string = strtok(NULL, " \t");
    if ((PNULL == ip_string) || (0 == udhcp_str2nip(ip_string, &nip)))
    {
        DHCPS_FREE(line);
        line = PNULL;
        return 0;
     }
    add_static_lease(arg, (uint8_t*) &mac_bytes, nip);
    log_static_leases(arg);
    DHCPS_FREE(line);
    line = PNULL;
    return 1;
}


struct config_keyword {
	const char *keyword;
	int (*handler)(const char *line, void *var);
	void *var;
	const char *def;
};

static const struct config_keyword keywords[] = {
	/* keyword        handler           variable address               default */
#ifdef WIN32
	{"start"        , udhcp_str2nip   , &server_config.start_ip     , "172.16.0.20"},
	{"end"          , udhcp_str2nip   , &server_config.end_ip       , "172.16.0.254"},
#else
	{"start"        , udhcp_str2nip   , &server_config.start_ip     , "192.168.1.20"},
	{"end"          , udhcp_str2nip   , &server_config.end_ip       , "192.168.1.254"},
#endif
	{"interface"    , read_str        , &server_config.inf    , "eth0"},
	/* Avoid "max_leases value not sane" warning by setting default
	 * to default_end_ip - default_start_ip + 1: */
	{"max_leases"   , read_u32        , &server_config.max_leases   , "235"},
	{"auto_time"    , read_u32        , &server_config.auto_time    , "7200"},
	{"decline_time" , read_u32        , &server_config.decline_time , "3600"},
	{"conflict_time", read_u32        , &server_config.conflict_time, "3600"},
	{"offer_time"   , read_u32        , &server_config.offer_time   , "60"},
	{"min_lease"    , read_u32        , &server_config.min_lease_sec, "60"},
	{"lease_file"   , read_str        , &server_config.lease_file   , LEASES_FILE},
	{"pidfile"      , read_str        , &server_config.pidfile      , "/var/run/udhcpd.pid"},
	{"siaddr"       , udhcp_str2nip   , &server_config.siaddr_nip   , "0.0.0.0"},
	/* keywords with no defaults must be last! */
	{"option"       , udhcp_str2optset, &server_config.options      , "subnet 255.255.255.0"},
#ifdef WIN32
	{"opt"          , udhcp_str2optset, &server_config.options      , "dns 172.16.0.134 172.16.0.135"},
    {"option"       , udhcp_str2optset, &server_config.options      , "router 172.16.18.1"},  
#else
    {"opt"          , udhcp_str2optset, &server_config.options      , "dns 192.168.1.1 192.168.1.2"},
    {"option"       , udhcp_str2optset, &server_config.options      , "router 192.168.1.1"},   
#endif
    {"notify_file"  , read_str        , &server_config.notify_file  , ""},
	{"sname"        , read_str        , &server_config.sname        , ""},
	{"boot_file"    , read_str        , &server_config.boot_file    , ""},	
#ifdef WIN32
    	{"static_lease" , read_staticlease, &server_config.static_leases, "00:50:08:10:ED:5F 172.16.1.53"},
	{"static_lease" , read_staticlease, &server_config.static_leases, "00:60:08:11:CE:4E 172.16.1.54"},
#else
	{"static_lease" , read_staticlease, &server_config.static_leases, "00:50:08:10:ED:5F 192.168.1.53"},
	{"static_lease" , read_staticlease, &server_config.static_leases, "00:60:08:11:CE:4E 192.168.1.54"},
#endif

   // {"static_lease" , read_staticlease, &server_config.static_leases, "20:59:a0:00:04:51 192.168.0.54"},
	
};

enum { KWS_WITH_DEFAULTS = ARRAY_SIZE(keywords) - 8/*6*/ };

void read_config(void)
{

	unsigned i = 0;
	
	for (i = 2; i <KWS_WITH_DEFAULTS ; i++)
		keywords[i].handler(keywords[i].def, keywords[i].var);

	server_config.start_ip = ntohl(server_config.start_ip);
	server_config.end_ip = ntohl(server_config.end_ip);
	
}

