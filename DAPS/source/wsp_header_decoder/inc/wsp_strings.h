#ifndef WSP_STRINGS_H
#define WSP_STRINGS_H

#include "Octstr.h"

#define _MMS_D      0x04    /* digit */
#define _MMS_S      0x80    /* space (0x20) */

extern const uint8 g_ascii_table[];
#define WSP_HEADER_ISSPACE(c)  ((c>=0 && c<256)&&((g_ascii_table)[(uint8)(c)] & (_MMS_S)))
#define WSP_HEADER_ISDIGIT(c)  ((c>=0 && c<256)&&((g_ascii_table)[(uint8)(c)] & (_MMS_D)))



struct wsp_element
{
    char *str;
    long number;
};

void wsp_strings_init(void);
void wsp_strings_shutdown(void);

Octstr* wsp_content_type_to_string(long number);
unsigned char*   wsp_content_type_to_cstr(long number);
long    wsp_string_to_content_type(Octstr *ostr);

Octstr* wsp_parameter_to_string(long number);
unsigned char*   wsp_parameter_to_cstr(long number);
long    wsp_string_to_parameter(Octstr *parameter);

Octstr* wsp_language_to_string(long number);
unsigned char*   wsp_language_to_cstr(long number);
long    wsp_string_to_language(Octstr *ostr);

Octstr* wsp_charset_to_string(long number);
unsigned char*   wsp_charset_to_cstr(long number);
long    wsp_string_to_charset(Octstr *ostr);

Octstr* wsp_cache_control_to_string(long number);
unsigned char*   wsp_cache_control_to_cstr(long number);
long    wsp_string_to_cache_control(Octstr *ostr);

Octstr* wsp_connection_to_string(long number);
unsigned char*   wsp_connection_to_cstr(long number);
long    wsp_string_to_connection(Octstr *ostr);

Octstr* wsp_encoding_to_string(long number);
unsigned char*   wsp_encoding_to_cstr(long number);
long    wsp_string_to_encoding(Octstr *ostr);

Octstr* wsp_header_to_string(long number);
unsigned char*   wsp_header_to_cstr(long number);
long    wsp_string_to_header(Octstr *ostr);

Octstr* wsp_method_to_string(long number);
unsigned char*   wsp_method_to_cstr(long number);
long    wsp_string_to_method(Octstr *ostr);

Octstr* wsp_ranges_to_string(long number);
unsigned char*   wsp_ranges_to_cstr(long number);
long    wsp_string_to_ranges(Octstr *ostr);

Octstr* wsp_transfer_encoding_to_string(long number);
unsigned char*   wsp_transfer_encoding_to_cstr(long number);
long    wsp_string_to_transfer_encoding(Octstr *ostr);

Octstr* wsp_push_application_to_string(long number);
unsigned char*   wsp_push_application_id_to_cstr(long number);
long    wsp_string_to_push_application_id(Octstr *ostr);

unsigned char*   wsp_disposition_to_cstr(long number);
long    wsp_string_to_disposition(Octstr *ostr);


enum enum_Header_Field_Name_Assignments{
        WSP_HEADER_ACCEPT = 0,
        WSP_HEADER_ACCEPT_CHARSET,
        WSP_HEADER_ACCEPT_ENCODING,
        WSP_HEADER_ACCEPT_LANGUAGE,
        WSP_HEADER_ACCEPT_RANGES,
        WSP_HEADER_AGE,
        WSP_HEADER_ALLOW,
        WSP_HEADER_AUTHORIZATION,
        WSP_HEADER_CACHE_CONTROL,
        WSP_HEADER_CONNECTION,
        WSP_HEADER_CONTENT_BASE,
        WSP_HEADER_CONTENT_ENCODING,
        WSP_HEADER_CONTENT_LANGUAGE,
        WSP_HEADER_CONTENT_LENGTH,
        WSP_HEADER_CONTENT_LOCATION,
        WSP_HEADER_CONTENT_MD5,
        WSP_HEADER_CONTENT_RANGE,
        WSP_HEADER_CONTENT_TYPE,
        WSP_HEADER_DATE,
        WSP_HEADER_ETAG,
        WSP_HEADER_EXPIRES,
        WSP_HEADER_FROM,
        WSP_HEADER_HOST,
        WSP_HEADER_IF_MODIFIED_SINCE,
        WSP_HEADER_IF_MATCH,
        WSP_HEADER_IF_NONE_MATCH,
        WSP_HEADER_IF_RANGE,
        WSP_HEADER_IF_UNMODIFIED_SINCE,
        WSP_HEADER_LOCATION,
        WSP_HEADER_LAST_MODIFIED,
        WSP_HEADER_MAX_FORWARDS,
        WSP_HEADER_PRAGMA,
        WSP_HEADER_PROXY_AUTHENTICATE,
        WSP_HEADER_PROXY_AUTHORIZATION,
        WSP_HEADER_PUBLIC,
        WSP_HEADER_RANGE,
        WSP_HEADER_REFERER,
        WSP_HEADER_RETRY_AFTER,
        WSP_HEADER_SERVER,
        WSP_HEADER_TRANSFER_ENCODING,
        WSP_HEADER_UPGRADE,
        WSP_HEADER_USER_AGENT,
        WSP_HEADER_VARY,
        WSP_HEADER_VIA,
        WSP_HEADER_WARNING,
        WSP_HEADER_WWW_AUTHENTICATE,
        WSP_HEADER_CONTENT_DISPOSITION,
        WSP_HEADER_X_WAP_APPLICATION_ID,
        WSP_HEADER_X_WAP_CONTENT_URI,
        WSP_HEADER_X_WAP_INITIATOR_URI,
        WSP_HEADER_ACCEPT_APPLICATION,
        WSP_HEADER_BEARER_INDICATION,
        WSP_HEADER_PUSH_FLAG,
        WSP_HEADER_PROFILE,
        WSP_HEADER_PROFILE_DIFF,
        WSP_HEADER_PROFILE_WARNING,
        WSP_HEADER_EXPECT,
        WSP_HEADER_TE,
        WSP_HEADER_TRAILER,
        WSP_HEADER_ACCEPT_CHARSET_V13,
        WSP_HEADER_ACCEPT_ENCODING_V13,
        WSP_HEADER_CACHE_CONTROL_V13,
        WSP_HEADER_CONTENT_RANGE_V13,
        WSP_HEADER_X_WAP_TOD,
        WSP_HEADER_CONTENT_ID,
        WSP_HEADER_SET_COOKIE,
        WSP_HEADER_COOKIE,
        WSP_HEADER_ENCODING_VERSION,
        WSP_HEADER_PROFILE_WARNING_V14,
        WSP_HEADER_CONTENT_DISPOSOTION,
        WSP_HEADER_X_WAP_SECURITY,
        WSP_HEADER_CACHE_CONTROL_V14,
        header_dummy
};

enum enum_cache_control{
        WSP_CACHE_CONTROL_NO_CACHE = 0,          //"no-cache"
        WSP_CACHE_CONTROL_NO_STORE,          //"no-store"
        WSP_CACHE_CONTROL_MAX_AGE,           //"max-age"
        WSP_CACHE_CONTROL_MAX_STALE,         //"max-stale"
        WSP_CACHE_CONTROL_MIN_FRESH,         //"min-fresh"
        WSP_CACHE_CONTROL_ONLY_IF_CACHED,    //"only-if-cached"
        WSP_CACHE_CONTROL_PUBLIC,            //"public"
        WSP_CACHE_CONTROL_PRIVATE,           //"private"
        WSP_CACHE_CONTROL_NO_TRANSFORM,      //"no-transform"
        WSP_CACHE_CONTROL_MUST_REVALIDATE,   //"must-revalidate"
        WSP_CACHE_CONTROL_PROXY_REVALIDATE,  //"proxy-revalidate"
        cache_control_dummy
};

#endif
