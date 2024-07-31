/* vi: set sw=4 ts=4: */
/*
 * Rewrite by Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */

#include "socket_types.h"
#include "os_api.h"
#include "socket_api.h"
#include "dhcps_common.h"


#if defined CONFIG_UDHCP_DEBUG && CONFIG_UDHCP_DEBUG >= 1
unsigned dhcp_verbose;
#endif

#define INVALID_OPITON_END  ((uint32_t)(-1))

#define _tolower(_c) ((_c)|((char) 0x20))

#ifndef isdigit
#define isdigit(_c)  ((_c >= '0' && _c <= '9') ? 1:0)
#endif

#ifndef isspace
#define isspace(_c)  ((_c==' '||_c=='\t'||_c=='\n'||_c=='\v'||_c=='\f'||_c=='\r')? 1:0)
#endif

char* hex2bin(char *dst, const char *str, int count);

const uint8_t MAC_BCAST_ADDR[MAC_ADDR_LEN] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

/* Supported options are easily added here.
 * See RFC2132 for more options.
 * OPTION_REQ: these options are requested by udhcpc (unless -o).
 */
const struct dhcp_optflag dhcp_optflags[] = {
    /* flags                                    code */
    { OPTION_IP                   | OPTION_REQ, 0x01 }, /* DHCP_SUBNET        */
    { OPTION_S32                              , 0x02 }, /* DHCP_TIME_OFFSET   */
    { OPTION_IP | OPTION_LIST     | OPTION_REQ, 0x03 }, /* DHCP_ROUTER        */
//    OPTION_IP | OPTION_LIST                 , 0x04 }, /* DHCP_TIME_SERVER   */
//    OPTION_IP | OPTION_LIST                 , 0x05 }, /* DHCP_NAME_SERVER   */
    { OPTION_IP | OPTION_LIST     | OPTION_REQ, 0x06 }, /* DHCP_DNS_SERVER    */
//    OPTION_IP | OPTION_LIST                 , 0x07 }, /* DHCP_LOG_SERVER    */
//    OPTION_IP | OPTION_LIST                 , 0x08 }, /* DHCP_COOKIE_SERVER */
    { OPTION_IP | OPTION_LIST                 , 0x09 }, /* DHCP_LPR_SERVER    */
    { OPTION_STRING               | OPTION_REQ, 0x0c }, /* DHCP_HOST_NAME     */
    { OPTION_U16                              , 0x0d }, /* DHCP_BOOT_SIZE     */
    { OPTION_STRING               | OPTION_REQ, 0x0f }, /* DHCP_DOMAIN_NAME   */
    { OPTION_IP                               , 0x10 }, /* DHCP_SWAP_SERVER   */
    { OPTION_STRING                           , 0x11 }, /* DHCP_ROOT_PATH     */
    { OPTION_U8                               , 0x17 }, /* DHCP_IP_TTL        */
    { OPTION_U16                              , 0x1a }, /* DHCP_MTU           */
    { OPTION_IP                   | OPTION_REQ, 0x1c }, /* DHCP_BROADCAST     */
    { OPTION_IP_PAIR | OPTION_LIST            , 0x21 }, /* DHCP_ROUTES        */
    { OPTION_STRING                           , 0x28 }, /* DHCP_NIS_DOMAIN    */
    { OPTION_IP | OPTION_LIST                 , 0x29 }, /* DHCP_NIS_SERVER    */
    { OPTION_IP | OPTION_LIST     | OPTION_REQ, 0x2a }, /* DHCP_NTP_SERVER    */
    { OPTION_IP | OPTION_LIST                 , 0x2c }, /* DHCP_WINS_SERVER   */
    { OPTION_U32                              , 0x33 }, /* DHCP_LEASE_TIME    */
    { OPTION_IP                               , 0x36 }, /* DHCP_SERVER_ID     */
    { OPTION_STRING                           , 0x38 }, /* DHCP_ERR_MESSAGE   */
//TODO: must be combined with 'sname' and 'file' handling:
    { OPTION_STRING                           , 0x42 }, /* DHCP_TFTP_SERVER_NAME */
    { OPTION_STRING                           , 0x43 }, /* DHCP_BOOT_FILE     */
//TODO: not a string, but a set of LASCII strings:
//    OPTION_STRING                           , 0x4D }, /* DHCP_USER_CLASS    */
#ifdef ENABLE_FEATURE_UDHCP_RFC3397
    { OPTION_DNS_STRING | OPTION_LIST         , 0x77 }, /* DHCP_DOMAIN_SEARCH */
    { OPTION_SIP_SERVERS                      , 0x78 }, /* DHCP_SIP_SERVERS   */
#endif
    { OPTION_STATIC_ROUTES                    , 0x79 }, /* DHCP_STATIC_ROUTES */
    { OPTION_STATIC_ROUTES                    , 0xf9 }, /* DHCP_MS_STATIC_ROUTES */
    { OPTION_STRING                           , 0xfc }, /* DHCP_WPAD          */

    /* Options below have no match in dhcp_option_strings[],
     * are not passed to dhcpc scripts, and cannot be specified
     * with "option XXX YYY" syntax in dhcpd config file.
     * These entries are only used internally by udhcp[cd]
     * to correctly encode options into packets.
     */

    { OPTION_IP                               , 0x32 }, /* DHCP_REQUESTED_IP  */
    { OPTION_U8                               , 0x35 }, /* DHCP_MESSAGE_TYPE  */
    { OPTION_U16                              , 0x39 }, /* DHCP_MAX_SIZE      */
    { OPTION_U32                              , 0x3a }, /* DHCP_RENEWAL_TIME */
    { OPTION_U32                              , 0x3b }, /* DHCP_REBINDING_TIME */
//looks like these opts will work just fine even without these defs:
//    OPTION_STRING                           , 0x3c }, /* DHCP_VENDOR        */
//  * not really a string: */
//    OPTION_STRING                           , 0x3d }, /* DHCP_CLIENT_ID     */
    { 0, 0 } /* zeroed terminating entry */
};

/* Used for converting options from incoming packets to env variables
 * for udhcpc stript, and for setting options for udhcpd via
 * "opt OPTION_NAME OPTION_VALUE" directives in udhcpd.conf file.
 */
/* Must match dhcp_optflags[] order */
//const char dhcp_option_strings[] ALIGN1 =

const char dhcp_option_strings[] =
    "subnet" "\0"      /* DHCP_SUBNET         */
    "timezone" "\0"    /* DHCP_TIME_OFFSET    */
    "router" "\0"      /* DHCP_ROUTER         */
//  "timesrv" "\0"     /* DHCP_TIME_SERVER    */
//  "namesrv" "\0"     /* DHCP_NAME_SERVER    */
    "dns" "\0"         /* DHCP_DNS_SERVER     */
//  "logsrv" "\0"      /* DHCP_LOG_SERVER     */
//  "cookiesrv" "\0"   /* DHCP_COOKIE_SERVER  */
    "lprsrv" "\0"      /* DHCP_LPR_SERVER     */
    "hostname" "\0"    /* DHCP_HOST_NAME      */
    "bootsize" "\0"    /* DHCP_BOOT_SIZE      */
    "domain" "\0"      /* DHCP_DOMAIN_NAME    */
    "swapsrv" "\0"     /* DHCP_SWAP_SERVER    */
    "rootpath" "\0"    /* DHCP_ROOT_PATH      */
    "ipttl" "\0"       /* DHCP_IP_TTL         */
    "mtu" "\0"         /* DHCP_MTU            */
    "broadcast" "\0"   /* DHCP_BROADCAST      */
    "routes" "\0"      /* DHCP_ROUTES         */
    "nisdomain" "\0"   /* DHCP_NIS_DOMAIN     */
    "nissrv" "\0"      /* DHCP_NIS_SERVER     */
    "ntpsrv" "\0"      /* DHCP_NTP_SERVER     */
    "wins" "\0"        /* DHCP_WINS_SERVER    */
    "lease" "\0"       /* DHCP_LEASE_TIME     */
    "serverid" "\0"    /* DHCP_SERVER_ID      */
    "message" "\0"     /* DHCP_ERR_MESSAGE    */
    "tftp" "\0"        /* DHCP_TFTP_SERVER_NAME */
    "bootfile" "\0"    /* DHCP_BOOT_FILE      */
//  "userclass" "\0"   /* DHCP_USER_CLASS     */
#ifdef ENABLE_FEATURE_UDHCP_RFC3397
    "search" "\0"      /* DHCP_DOMAIN_SEARCH  */
// doesn't work in udhcpd.conf since OPTION_SIP_SERVERS
// is not handled yet by "string->option" conversion code:
    "sipsrv" "\0"      /* DHCP_SIP_SERVERS    */
#endif
// doesn't work in udhcpd.conf since OPTION_STATIC_ROUTES
// is not handled yet by "string->option" conversion code:
    "staticroutes" "\0"     /* DHCP_STATIC_ROUTES  */
    "msstaticroutes""\0"    /* DHCP_MS_STATIC_ROUTES */
    "wpad" "\0"             /* DHCP_WPAD           */
    ;

/* Lengths of the option types in binary form.
 * Used by:
 * udhcp_str2optset: to determine how many bytes to allocate.
 * xmalloc_optname_optval: to estimate string length
 * from binary option length: (option[LEN] / dhcp_option_lengths[opt_type])
 * is the number of elements, multiply in by one element's string width
 * (len_of_option_as_string[opt_type]) and you know how wide string you need.
 */
const uint8_t dhcp_option_lengths[][2] = {
    {OPTION_IP,4},
    {OPTION_IP_PAIR,8},
//  [OPTION_BOOLEAN] = 1,
    {OPTION_STRING,1},  /* ignored by udhcp_str2optset */
#ifdef ENABLE_FEATURE_UDHCP_RFC3397
    {OPTION_DNS_STRING,1},  /* ignored by both udhcp_str2optset and xmalloc_optname_optval */
    {OPTION_SIP_SERVERS,1},
#endif
    {OPTION_U8 ,1},
    {OPTION_U16,2},
//  [OPTION_S16] =     2,
    {OPTION_U32,4},
    {OPTION_S32,4},
    /* Just like OPTION_STRING, we use minimum length here */
    {OPTION_STATIC_ROUTES,5}
};


#if defined CONFIG_UDHCP_DEBUG && CONFIG_UDHCP_DEBUG >= 2
static void log_option(const char *pfx, const uint8_t *opt)
{
    if (dhcp_verbose >= 2)
    {
        char buf[256 * 2 + 2];
        *bin2hex(buf, (void*) (opt + OPT_DATA), opt[OPT_LEN]) = '\0';
        bb_info_msg("%s: 0x%02x %s", pfx, opt[OPT_CODE], buf);
    }
}
#else
#define log_option(pfx, opt) ((void)0)
#endif

int index_in_strings(const char *strings, const char *key)
{
    int idx = 0;

    if( NULL == strings )
        return (-1);
    
    while( 0x00 != *strings )
    {
        if( 0 == strcmp(strings, key) )
        {
            return idx;
        }
        strings += strlen(strings) + 1; /* skip NULL */
        idx++;
    }
    return (-1);
}

/**
 * strnlen - Find the length of a length-limited string
 * @s: The string to be sized
 * @count: The maximum number of bytes to search
 */
size_t strnlen(const char *s, size_t count)
{
    const char *sc = NULL;

    for (sc = s; count-- && *sc != '\0'; ++sc)
        /* nothing */;
    return (sc - s);
}

/* Like strncpy but make sure the resulting string is always 0 terminated. */
char* safe_strncpy(char *dst, const char *src, size_t size)
{
    if (!size) return dst;
    dst[--size] = '\0';
    return strncpy(dst, src, size);
}

int udhcp_option_idx(const char *name)
{
    int n = index_in_strings(dhcp_option_strings, name);
    if (n >= 0)
        return n;
    
    {
        char buf[sizeof(dhcp_option_strings)];
        char *d = buf;
        const char *s = dhcp_option_strings;
        while (s < dhcp_option_strings + sizeof(dhcp_option_strings) - 2)
        {
            *d++ = (*s == '\0' ? ' ' : *s);
            s++;
        }
        *d = '\0';

        DHCPS_TRACE("tcpip: DHCPS udhcp_option_idx - unknown option '%s', known options: %s", name, buf);
        return -1;
    }
}

/* Get an option with bounds checking (warning, result is not aligned) */
uint8_t* udhcp_get_option(struct dhcp_packet *packet, int code)
{
    uint8_t*    optionptr = NULL;
    int         len = 0;
    int         rem = 0;
    int         overload = 0;
    
    enum
    {
        FILE_FIELD101  = FILE_FIELD  * 0x101,
        SNAME_FIELD101 = SNAME_FIELD * 0x101
    };
    
    /* option bytes: [code][len][data1][data2]..[dataLEN] */
    optionptr = packet->options;
    rem = sizeof(packet->options);
    while (1)   /*lint !e716*/
    {
        if( rem <= 0 )
        {
            DHCPS_TRACE("tcpip dhcps: udhcp_get_option - bad pkt, malformed option field");
            return NULL;
        }
        
        if( optionptr[OPT_CODE] == DHCP_PADDING )
        {
            rem--;
            optionptr++;
            continue;
        }
        
        if( optionptr[OPT_CODE] == DHCP_END )
        {
            if ((overload & FILE_FIELD101) == FILE_FIELD)
            {
                /* can use packet->file, and didn't look at it yet */
                overload |= FILE_FIELD101; /* "we looked at it" */
                optionptr = packet->file;
                rem = sizeof(packet->file);
                continue;
            }
            if ((overload & SNAME_FIELD101) == SNAME_FIELD)
            {
                /* can use packet->sname, and didn't look at it yet */
                overload |= SNAME_FIELD101; /* "we looked at it" */
                optionptr = packet->sname;
                rem = sizeof(packet->sname);
                continue;
            }
            break;
        }
        
        len = 2 + optionptr[OPT_LEN];
        rem -= len;
        if( rem < 0 )
            continue; /* complain and return NULL */

        if( optionptr[OPT_CODE] == code )
        {
            log_option("Option found", optionptr);
            return optionptr + OPT_DATA;
        }

        if( optionptr[OPT_CODE] == DHCP_OPTION_OVERLOAD )
        {
            overload |= optionptr[OPT_DATA];
            /* fall through */
        }
        optionptr += len;
    }

    /* log3 because udhcpc uses it a lot - very noisy */
    DHCPS_DEBUG("tcpip dhcps: udhcp_get_option - option 0x%02x not found", code);
    return NULL;
}

/*****************************************************************************/
// Description : get DHCP option end position
// Global : 
// Author : third-party
// Note : PARAM : option_ptr (IN) -- option buffer pointer
//        RETURN: the relative position of the 'end' option (no bounds checking)
/*****************************************************************************/
uint32_t udhcp_end_option(uint8_t *option_ptr)
{
    uint32_t i = 0;

    if( NULL == option_ptr )
    {
        return INVALID_OPITON_END;
    }
    
    while( DHCP_END != option_ptr[i] )
    {
        if( option_ptr[i] != DHCP_PADDING )
            i += option_ptr[i + OPT_LEN] + OPT_DATA - 1;
        i++;
    }
    return i;
}

/*****************************************************************************/
// Description : add an option (supplied in binary form) to the options
// Global : 
// Author : third-party
// Note : PARAM1: packet (IN) -- packet buffer pointer
//        PARAM2: addopt (IN) -- option buffer pointer
//        Option format: [code][len][data(1)][data(2)]..[data(len)]
/*****************************************************************************/
void udhcp_add_binary_option(struct dhcp_packet *packet, uint8_t *addopt)
{
    uint32_t len = 0, end = 0;
    uint8_t* option_ptr = NULL;

    if( NULL == packet || NULL == addopt )
        return;

    /* get and check option end */
    option_ptr = packet->options;
    end = udhcp_end_option(option_ptr);
    if( INVALID_OPITON_END == end )
        return;

    /* calculate new option length */
    len = OPT_DATA + addopt[OPT_LEN];

    /* check if option buffer is big enough */
    /* end position + (option code/length + addopt length) + end option */
    if( (end + len + 1) >= DHCP_OPTIONS_BUFSIZE )
    {
        //TODO: learn how to use overflow option if we exhaust packet->options[]
        DHCPS_TRACE("tcpip dhcps: option 0x%02x did not fit into the packet", addopt[OPT_CODE]);
        return;
    }
    log_option("Adding option", addopt);

    /* set the new option */
    DHCPS_MEMCPY(option_ptr + end, addopt, len);
    option_ptr[end + len] = DHCP_END;
}

/* Add an one to four byte option to a packet */
void udhcp_add_simple_option(struct dhcp_packet *packet, uint8_t code, uint32_t data)
{
    const struct dhcp_optflag* dh = dhcp_optflags;
    uint8_t option[6] = {0};
    uint8_t len = 0;
    
    for( ; 0x00 != dh->code; dh++ )
    {
        if( dh->code == code )
        {
            DHCPS_MEMSET(option, 0, sizeof(option));
            option[OPT_CODE] = code;        
            len = udhcp_get_optionlen(dh->flags & OPTION_TYPE_MASK);
            option[OPT_LEN] = len;
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
            data <<= 8 * (4 - len);
#endif
            /* assignment is unaligned! */
            move_to_unaligned32(&option[OPT_DATA], data);
            udhcp_add_binary_option(packet, option);
            return;
        }
    }

    DHCPS_TRACE("tcpip dhcps: udhcp_add_simple_option - can't add option 0x%02x", code);
}

/* Find option 'code' in opt_list */
struct option_set* udhcp_find_option(struct option_set *opt_list, uint8_t code)
{
    while( NULL != opt_list && opt_list->data[OPT_CODE] < code )
        opt_list = opt_list->next;

    if( NULL != opt_list && opt_list->data[OPT_CODE] == code )
        return opt_list;
    
    return NULL;
}

/* Parse string to IP in network order */
int udhcp_str2nip(const char *str, void *arg)
{
    TCPIP_IPADDR_T ipaddr = 0;
    char* temp_str = PNULL;

    if((PNULL == str)||(PNULL == arg))
        return 0;
    
    temp_str =(char *)DHCPS_ALLOC(strlen(str)+1);
    if(PNULL == temp_str)
        return 0;
    DHCPS_MEMSET(temp_str,0,strlen(str)+1);
    DHCPS_MEMCPY(temp_str,str,strlen(str));

    if(!inet_aton(temp_str,&ipaddr)) /*lint !e605*/
    {
        DHCPS_FREE(temp_str);   
        temp_str = PNULL;
        return 0;	
    }
    *(uint32_t*)arg = ipaddr;
    DHCPS_FREE(temp_str); 
    temp_str = PNULL;
    return 1;
}

/* udhcp_str2optset:
 * Parse string option representation to binary form and add it to opt_list.
 * Called to parse "udhcpc -x OPTNAME:OPTVAL"
 * and to parse udhcpd.conf's "opt OPTNAME OPTVAL" directives.
 */
/* helper for the helper */
static char *allocate_tempopt_if_needed(
		const struct dhcp_optflag *optflag,
		char *buffer,
		int *length_p)
{
	char *allocated = PNULL;
	if ((optflag->flags & OPTION_TYPE_MASK) == OPTION_BIN) {     
		const char *end;        
        
        allocated = (char *)DHCPS_ALLOC(strlen(buffer)+1);
        if(PNULL == allocated)
            return allocated;       
        strcpy(allocated,buffer);        
		end = hex2bin(allocated, buffer, 255);
		DHCPS_TRACE("tcpip:DHCPS- allocate_tempopt_if_needed - malformed hex string '%s'", buffer);
		*length_p = end - allocated;
	}
	return allocated;
}
/* helper: add an option to the opt_list */
static void attach_option(
    struct option_set**         opt_list,
    const struct dhcp_optflag*  optflag,
    char*                       buffer,
    unsigned                    length)
{
    struct option_set *existing, *new_opt, **curr;
    char *allocated = NULL;

    existing = udhcp_find_option(*opt_list, optflag->code);
    if( PNULL == existing )
    {
        DHCPS_TRACE("tcpip dhcps: attach_option - attach option %02x to list", optflag->code);
        allocated = allocate_tempopt_if_needed(optflag, buffer, (int*)&length);
#ifdef ENABLE_FEATURE_UDHCP_RFC3397
        if ((optflag->flags & OPTION_TYPE_MASK) == OPTION_DNS_STRING)
        {
            /* reuse buffer and length for RFC1035-formatted string */
            allocated = buffer = (char *)dname_enc(NULL, 0, buffer, (int*)&length);
        }
#endif
        /* make a new option */
        //new = xmalloc(sizeof(*new));
        //new->data = xmalloc(length + OPT_DATA);
        new_opt = (struct option_set *)DHCPS_ALLOC(sizeof(*new_opt));
        new_opt->data = (uint8_t *)DHCPS_ALLOC(length + OPT_DATA);
        DHCPS_MEMSET(new_opt->data,0,length + OPT_DATA);
        new_opt->data[OPT_CODE] = optflag->code;
        new_opt->data[OPT_LEN] = length;
        DHCPS_MEMCPY(new_opt->data + OPT_DATA, (allocated ? allocated : buffer), length);

        curr = opt_list;
        while (*curr && (*curr)->data[OPT_CODE] < optflag->code)
            curr = &(*curr)->next;

        new_opt->next = *curr;
        *curr = new_opt;
        goto ret;
    }

    if( optflag->flags & OPTION_LIST )
    {
        unsigned old_len = 0;

        /* add it to an existing option */
        //log2("Attaching option %02x to existing member of list", optflag->code);
        DHCPS_TRACE("tcpip dhcps: attach_option - attaching option %02x to existing member of list", optflag->code);
        allocated = allocate_tempopt_if_needed(optflag, buffer,(int*)&length);
        old_len = existing->data[OPT_LEN];
#ifdef ENABLE_FEATURE_UDHCP_RFC3397
        if ((optflag->flags & OPTION_TYPE_MASK) == OPTION_DNS_STRING)\
        {
            /* reuse buffer and length for RFC1035-formatted string */
            allocated = buffer = (char *)dname_enc(existing->data + OPT_DATA, old_len, buffer, &length);
        }
#endif
        if( old_len + length < 255 )
        {
            /* actually 255 is ok too, but adding a space can overlow it */
            existing ->data = DHCPS_ReAlloc(
                existing->data, OPT_DATA+old_len, OPT_DATA + 1 + old_len + length);
            if ((optflag->flags & OPTION_TYPE_MASK) == OPTION_STRING)
            {
                /* add space separator between STRING options in a list */
                existing->data[OPT_DATA + old_len] = ' ';
                old_len++;
            }
            DHCPS_MEMCPY(existing->data + OPT_DATA + old_len, buffer, length);
            existing->data[OPT_LEN] = old_len + length;
        }
        /* else, ignore the data, we could put this in a second option in the future */
    } 
    /* else, ignore the new data */

ret:
    DHCPS_FREE(allocated);
    allocated = PNULL;
}

int udhcp_str2optset(const char *const_str, void *arg)
{
    struct option_set **opt_list = arg;
    char *opt, *val, *endptr;
    char *str;
    const struct dhcp_optflag *optflag;
    int retval, length = 0;
    char buffer[8];
    uint16_t *result_u16 = (uint16_t *) buffer;
    uint32_t *result_u32 = (uint32_t *) buffer;
    int index = 0;
    
    str = (char*)DHCPS_ALLOC(strlen(const_str)+1);
    if( PNULL == str )
    {
        DHCPS_TRACE("tcpip dhcps: udhcp_str2optset - alloc memory failed");
        return 0;  
    }
    DHCPS_MEMSET(str, 0, strlen(const_str)+1);
    DHCPS_MEMCPY(str, const_str, strlen(const_str));
    
    /* cheat, the only *const* str possible is "" */
    opt = strtok(str, " \t=");    
    if( NULL == opt )
    {
        DHCPS_TRACE("tcpip dhcps: udhcp_str2optset - opt is null");
        DHCPS_FREE(str);
        str = PNULL;
        return 0;
    }
    
#if 0   /*deleted by lisa @2010-7-24*/
    optcode = bb_strtou(opt, NULL, 0);

    if( !errno && optcode < 255 )
    {
        /* Raw (numeric) option code */
        bin_optflag.flags = OPTION_BIN;
        bin_optflag.code = optcode;
        optflag = &bin_optflag;
    }
    else
    {
        optflag = &dhcp_optflags[udhcp_option_idx(opt)];
    }
#endif

    index = udhcp_option_idx(opt);
    if( index < 0 )
    {   
        DHCPS_TRACE("tcpip dhcps: udhcp_str2optset - index invalid");
        DHCPS_FREE(str);       
        str = PNULL;
        return 0;    
    }
    
    optflag = &dhcp_optflags[index];    
    retval = 0;

    do
    {
        val = strtok(NULL, ", \t");
        if (!val)
            break;
        //length = dhcp_option_lengths[optflag->flags & OPTION_TYPE_MASK];
        length = udhcp_get_optionlen(optflag->flags & OPTION_TYPE_MASK);
        retval = 0;
        opt = buffer; /* new meaning for variable opt */
        switch (optflag->flags & OPTION_TYPE_MASK)
        {
        case OPTION_IP:
            retval = udhcp_str2nip(val, buffer);
            break;
        
        case OPTION_IP_PAIR:
            retval = udhcp_str2nip(val, buffer);
            val = strtok(NULL, ", \t/-");
            if (!val)
                retval = 0;
            if (retval)
                retval = udhcp_str2nip(val, buffer + 4);
            break;
        
        case OPTION_STRING:
#ifdef ENABLE_FEATURE_UDHCP_RFC3397
        case OPTION_DNS_STRING:
#endif
            length = strnlen(val, 254);
            if (length > 0) {
                opt = val;
                retval = 1;
            }
            break;
         
//      case OPTION_BOOLEAN:
//      {
//          static const char no_yes[] ALIGN1 = "no\0yes\0";
//          buffer[0] = retval = index_in_strings(no_yes, val);
//          retval++; /* 0 - bad; 1: "no" 2: "yes" */
//          break;
//      }

        case OPTION_U8:
            buffer[0] = strtoul(val, &endptr, 0);
            retval = (endptr[0] == '\0');
            break;
        
        /* htonX are macros in older libc's, using temp var
         * in code below for safety */
        /* TODO: use bb_strtoX? */
        case OPTION_U16:
        {
            unsigned long tmp = strtoul(val, &endptr, 0);
            *result_u16 = htons(tmp);
            retval = (endptr[0] == '\0' /*&& tmp < 0x10000*/);
            break;
        }
        
//      case OPTION_S16:
//      {
//          long tmp = strtol(val, &endptr, 0);
//          *result_u16 = htons(tmp);
//          retval = (endptr[0] == '\0');
//          break;
//      }
        
        case OPTION_U32:
        {
            unsigned long tmp = strtoul(val, &endptr, 0);
            *result_u32 = htonl(tmp);
            retval = (endptr[0] == '\0');
            break;
        }
        
        case OPTION_S32:
        {
            long tmp = strtol(val, &endptr, 0);
            *result_u32 =(uint32)(htonl(tmp)); /*lint !e737*/
            retval = (endptr[0] == '\0');
            break;
        }
        
        case OPTION_BIN: /* handled in attach_option() */
            opt = val;
            retval = 1;
            break;
        
        default:
            break;
        }
        
        if( 0 != retval )
        {
            attach_option(opt_list, optflag, opt, length);
        }
    } while( retval && optflag->flags & OPTION_LIST );

    DHCPS_FREE(str);
    str = PNULL;
    return retval;
}


/* Convert "[x]x[:][x]x[:][x]x[:][x]x" hex string to binary, no more than COUNT bytes */
char* hex2bin(char *dst, const char *str, int count)
{
	//errno = EINVAL;
	while (*str && count) {
		uint8_t val;
		uint8_t c = *str++;
		if (isdigit(c))
			val = c - '0';
		else if ((c|0x20) >= 'a' && (c|0x20) <= 'f')
			val = (c|0x20) - ('a' - 10);
		else
			return NULL;
		val <<= 4;
		c = *str;
		if (isdigit(c))
			val |= c - '0';
		else if ((c|0x20) >= 'a' && (c|0x20) <= 'f')
			val |= (c|0x20) - ('a' - 10);
		else if (c == ':' || c == '\0')
			val >>= 4;
		else
			return NULL;

		*dst++ = val;
		if (c != '\0')
			str++;
		if (*str == ':')
			str++;
		count--;
	}
	//errno = (*str ? ERANGE : 0);
	return dst;
} /*lint !e429*/


uint8_t udhcp_get_optionlen(uint8_t flag)
{
    uint32 i =0;

    for(i=0;i<sizeof(dhcp_option_lengths)/(2*sizeof(uint8_t));i++)
    {
        if(flag == dhcp_option_lengths[i][0])
        {
            return dhcp_option_lengths[i][1];
        }
    }
    return 0;
}
struct ether_addr * ether_aton_r (const char *asc, struct ether_addr *addr)   
{                                                                              
    size_t cnt; 

    if((PNULL == asc)||(PNULL == addr))
        return PNULL;

    for (cnt = 0; cnt < 6; ++cnt)
    {                                                                          
        unsigned int number;   
        char ch;           

        ch = _tolower (*asc++);  
        if ((ch < '0' || ch > '9') && (ch < 'a' || ch > 'f')) 
            return NULL;                             
        number = isdigit (ch) ? (ch - '0') : (ch - 'a' + 10); 
        ch = _tolower (*asc); 
        if ((cnt < 5 && ch != ':') || (cnt == 5 && ch != '\0' && !isspace (ch))) /*lint !e774*/
        {  
            ++asc;      
            if ((ch < '0' || ch > '9') && (ch < 'a' || ch > 'f'))    
                return NULL;           
            number <<= 4;         
            number += (isdigit (ch) ? (ch - '0') : (ch - 'a' + 10));      /*lint !e737*/             
            ch = *asc;               
            if (cnt < 5 && ch != ':')    
                return NULL;         
        }                                                                          
        /** Store result.  */       
        addr->ether_addr_octet[cnt] = (unsigned char) number; 
        /** Skip ':'.  */                           
        ++asc;                              
    }           
    return addr;                                                               
}                                                                              

PUBLIC void* DHCPS_ReAlloc(void *old_mem_ptr, uint32 old_size,uint32 new_size)
{
	void *new_mem_ptr = 0;

	if (PNULL == old_mem_ptr || 0 == new_size)
	{
		DHCPS_TRACE("tcpip: DCHPS -DHCPS_ReAlloc- enter, input invalid!");
		return NULL;
	}

	if(new_size < old_size)
	{
		new_mem_ptr = old_mem_ptr;
	}
	else
	{
		new_mem_ptr = DHCPS_ALLOC(new_size);
		if( NULL != new_mem_ptr )
		{
            DHCPS_MEMSET(new_mem_ptr,0,new_size);
            DHCPS_MEMCPY(new_mem_ptr,old_mem_ptr,old_size);
            DHCPS_FREE(old_mem_ptr);
            old_mem_ptr = PNULL;
        }
	}
	return new_mem_ptr;
}


