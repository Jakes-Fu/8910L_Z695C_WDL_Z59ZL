/*****************************************************************************
** File Name:      http_def.h                                                *
** Author:          li.li                                                    *
** Date:           14/11/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the HTTP define             *
*****************************************************************************/

#ifndef _HTTPC_DEF_H_
#define _HTTPC_DEF_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
//#include "netSystem.h"
#include "httpc_api.h"
#include "netmgr_api.h"
#include "hsm_api.h"
#include "cfl_list.h"
#include "cfl_timer.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

//cache type
#define HTTPC_CACHE_UNKNOWN                           0      //unknown type
#define HTTPC_CACHE_PUBLIC                            0x001  //public param
#define HTTPC_CACHE_PRIVATE                           0x002  //private param
#define HTTPC_CACHE_NO_CACHE                          0x004  //no cache param
#define HTTPC_CACHE_NO_STORE                          0x008  //no store param
#define HTTPC_CACHE_NO_TRANSFORM                      0x010  //no transform param
#define HTTPC_CACHE_MUST_REVALIDATE                   0x020  //must revalidate param
#define HTTPC_CACHE_PROXY_REVALIDATE                  0x040  //proxy revalidate param
#define HTTPC_CACHE_MAX_AGE                           0x080  //max age param
#define HTTPC_CACHE_S_MAXAGE                          0x100  //s max age param
#define HTTPC_CACHE_EXTENSION                         0x200  //extension param

#define	HTTPC_CACHE_INDEX_FILE       "index"  //cache index file name "index"
#define	HTTPC_CACHE_DEFAULT_SIZE     4000000 //the max cache size
#define HTTPC_CACHE_DEFAULT_MAX_COUNT  100   //the max cache count
#define HTTPC_CACHE_MAX_READ_LEN     5000    //the max len when read file

#define HTTPC_CACHE_HASH_LEN         16      //the hash buffer len
#define HTTPC_CACHE_MAX_INFO_LEN     1024    //the max cache info len
//chunk
#define HTTPC_CHUNK_MAXNUM_SIZE      16      //the chunk max num size
//cookie
#define HTTPC_COOKIE_MAX_SIZE        (30*1024)   //the max cookie size
#define HTTPC_COOKIE_MAX_LEN         (10*1024)   //the max cookie read len
#define HTTPC_COOKIE_INVALID_STRING  ""

#define HTTPC_HSM_CONNECT_TIMEOUT                30000   //the max connect time
#define HTTPC_HSM_SEND_TIMEOUT                   60000   //the max send time
#define HTTPC_HSM_RECV_TIMEOUT                   120000   //the max receive time of one packet
#define HTTPC_HSM_HANDSHAKE_TIMEOUT              60000   //the handshake time
#define HTTPC_HSM_RECV_PROGRESS_TIMEOUT          500
#define HTTPC_HSM_RETRY_MAXCOUNT                 2       //the retry connect number
#define HTTPC_PROXY_CONNECT_RETRY_MAXCOUNT       2       //the proxy connect retry number
#define HTTPC_SENDBUF_MAX_SIZE                   6000    //the max send buffer size

#define HTTPC_MAXLEN_DIGEST_REPONSE_PARAM_TOTAL  200     //the max len of digest response param
#define HTTPC_MAXLEN_DIGEST_REPONSE_PARAM        50      //the max len of digest response token param
#define HTTPC_MAXLEN_AUTHENLEN_BASIC             300     //the max len of basic auth len
#define HTTPC_MAXLEN_AUTHENLEN_DIGEST            1000    //the max len of digest auth len
#define HTTPC_MAX_REDIRECT_COUNT                 50      //the max redirect count
//hsm

#define HTTPC_SLASH_STRING           "/"
#define HTTPC_CRLF_STRING            "\r\n"
#define HTTPC_CRLFX2_STRING          "\r\n\r\n"
#define HTTPC_GET_STRING             "GET"
#define HTTPC_POST_STRING            "POST"
#define HTTPC_PATCH_STRING           "PATCH"
#define HTTPC_HEAD_STRING            "HEAD"
#define HTTPC_OPTIONS_STRING         "OPTIONS"
#define HTTPC_DELETE_STRING         "DELETE"
#define HTTPC_CNONCE_LEN             32
#define HTTPC_METHOD_LEN             10
#define HTTPC_NC_STRING              "00000000"
#define HTTPC_NC_STRING_LEN          8
#define HTTPC_MD5_SESS_STRING        "MD5-SESS"
        
#define HTTPC_AUTH_BASIC             "Basic "
#define HTTPC_AUTH_DIGEST            "Digest "
#define HTTPC_AUTH_USERNMAE          "username=\""
#define HTTPC_AUTH_REALM             "\", realm=\""
#define HTTPC_AUTH_QOP               "\", qop=\""
#define HTTPC_AUTH_ALG               "\", algorithm=\""
#define HTTPC_AUTH_URI               "\", uri=\""
#define HTTPC_AUTH_NONCE             "\", nonce=\""
#define HTTPC_AUTH_NC                "\", nc=\""
#define HTTPC_AUTH_CNONCE            "\", cnonce=\""
#define HTTPC_AUTH_OPAQUE            "\", opaque=\""
#define HTTPC_AUTH_RESPONSE          "\", response=\""

#define HTTPC_COOKIE_PARAM_PORT      "PORT"
#define HTTPC_COOKIE_PARAM_PATH      "PATH"
#define HTTPC_COOKIE_PARAM_DOMAIN    "DOMAIN"
#define HTTPC_COOKIE_PARAM_VERSION   "VERSION"
#define HTTPC_COOKIE_PARAM_MAXAGE    "MAX-AGE"
#define HTTPC_COOKIE_PARAM_EXPIRES   "EXPIRES"
#define HTTPC_COOKIE_PARAM_SECURE    "SECURE"
#define HTTPC_COOKIE_PARAM_HTTPONLY  "httponly"

#define HTTPC_VERSION_STRING                     " HTTP/1.1"
#define HTTPC_CONNECT_STRING                     "CONNECT "
#define HTTPS_STRING                            "https://"
#define HTTPS_PORT_STRING                       "443"
#define HTTPC_USER_AGENT_STRING                  "User-Agent"
#define HTTPC_USER_AGENT_COLON_STRING            "User-Agent: "
#define HTTPC_HOST_COLON_STRING                  "Host: "
#define HTTPC_PORT_LEN                           10
#define HTTPC_VERSION10_STRING                   "HTTP/1.0 200"
#define HTTPC_VERSION11_STRING                   "HTTP/1.1 200"
#define HTTPC_HTTPS_STRING                       "https"
#define HTTPC_CONNECTION_CLOSE_STRING            "close"
#define HTTPC_CONNECTION_KEEP_ALIVE_STRING       "keep-alive"
#define HTTPC_ENCODING_GZIP_STRING               "gzip"

#define HTTPC_GET_SPACE_STRING                   "GET "
#define HTTPC_POST_SPACE_STRING                  "POST "
#define HTTPC_PATCH_SPACE_STRING                  "PATCH "
#define HTTPC_HEAD_SPACE_STRING                  "HEAD "
#define HTTPC_OPTIONS_SPACE_STRING               "OPTIONS "
#define HTTPC_DELETE_SPACE_STRING               "DELETE "
#define HTTPC_DEFAULT_PORT                       80
#define HTTPC_CONTENT_TYPE_STRING                "Content-Type"
#define HTTPC_COOKIE_COLON_STRING                "Cookie:"
#define HTTPC_PROXY_AUTH_STRING                  "Proxy-Authorization: "
#define HTTPC_ETAG_COLON_STRING                  "If-None-Match: "
#define HTTPC_LAST_MODIFIED_COLON_STRING         "If-Modified-Since: "
#define HTTPC_AUTH_COLON_STRING                  "Authorization: "
#define HTTPC_CONTENT_LENGTH_LEN                 10
#define HTTPC_CONTENT_TYPE_COLON_STRING          "Content-Type: "
#define HTTPC_CONTENT_LENGTH_COLON_STRING        "Content-Length: "
#define HTTPC_HTTP_STRING                        "HTTP"
#define HTTPC_CHUNKED_STRING                     "CHUNKED"
#define HTTPC_HTTP_COLON_STRING                  "HTTP:"
#define HTTPC_HTTP_COLON_SLASH_STRING            "http://"
#define HTTPC_HTTPS_COLON_SLASH_STRING           "https://"
#define HTTPC_DIGEST_STRING                      "DIGEST"
#define HTTPC_RANGE_STRING                       "Range"
#define HTTPC_NOT_EXPIRE_STRING                  "not-expire"

#ifdef WIN32
#define HTTPC_RAND_MAX_NUM                       0x7fff
#else
#define HTTPC_RAND_MAX_NUM                       0x7fffffff
#endif
#define HTTPC_BLANK_NUM                          0x20
#define HTTPC_STATE_CODE_LEN                     4
#define HTTPC_SLASH_0_STRING                     "/1.0"
#define HTTPC_SLASH_1_STRING                     "/1.1"
#define HTTPC_TWENTY_K                           (20*1024)
#define HTTPC_PROGRESS_COUNT                     100
#define HTTPC_PROGRESS_TIME                      1000
#define HTTPC_TRACE                              SCI_TRACE_LOW
	
#define HTTPC_400_STRING                         "<html><head><title>http 400 bad request</title><body>http 400 bad request</body></head></html>"
#define HTTPC_402_STRING                         "<html><head><title>http 402 payment required</title><body>http 400 payment required</body></head></html>"
#define HTTPC_403_STRING                         "<html><head><title>http 403 forbidden</title><body>http 403 forbidden</body></head></html>"
#define HTTPC_404_STRING                         "<html><head><title>http 404 not found</title><body>http 404 not found</body></head></html>"
#define HTTPC_405_STRING                         "<html><head><title>http 405 method not allowed</title><body>http 405 method not allowed</body></head></html>"
#define HTTPC_406_STRING                         "<html><head><title>http 406 not acceptable</title><body>http 406 not acceptable</body></head></html>"
#define HTTPC_408_STRING                         "<html><head><title>http 408 request timeout</title><body>http 408 request timeout</body></head></html>"
#define HTTPC_409_STRING                         "<html><head><title>http 409 conflict</title><body>http 409 conflict</body></head></html>"
#define HTTPC_410_STRING                         "<html><head><title>http 410 gone</title><body>http 410 gone</body></head></html>"
#define HTTPC_411_STRING                         "<html><head><title>http 411 length required</title><body>http 411 length required</body></head></html>"
#define HTTPC_412_STRING                         "<html><head><title>http 412 precondition failed</title><body>http 412 precondition failed</body></head></html>"
#define HTTPC_413_STRING                         "<html><head><title>http 413 request entity too large</title><body>http 413 request entity too large</body></head></html>"
#define HTTPC_414_STRING                         "<html><head><title>http 414 request url too long</title><body>http 414 request url too long</body></head></html>"
#define HTTPC_415_STRING                         "<html><head><title>http 415 unsupported media type</title><body>http 415 unsupported media type</body></head></html>"
#define HTTPC_416_STRING                         "<html><head><title>http 416 requested range not satisfiable</title><body>http 416 requested range not satisfiable</body></head></html>"
#define HTTPC_417_STRING                         "<html><head><title>http 417 exceptation failed</title><body>http 417 exceptation failed</body></head></html>"
#define HTTPC_422_STRING                         "<html><head><title>http 422 unprocessable request</title><body>http 422 unprocessable request</body></head></html>"
#define HTTPC_500_STRING                         "<html><head><title>http 500 internal server error</title><body>http 500 internal server error</body></head></html>"
#define HTTPC_501_STRING                         "<html><head><title>http 501 not implemented</title><body>http 501 not implemented</body></head></html>"
#define HTTPC_502_STRING                         "<html><head><title>http 502 bad gateway</title><body>http 502 bad gateway</body></head></html>"
#define HTTPC_503_STRING                         "<html><head><title>http 503 service unavailable</title><body>http 503 service unavailable</body></head></html>"
#define HTTPC_504_STRING                         "<html><head><title>http 504 gateway timeout</title><body>http 504 gateway timeout</body></head></html>"
#define HTTPC_505_STRING                         "<html><head><title>http 505 HTTP version not supported</title><body>http 505 HTTP version not supported</body></head></html>"

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/



//auth
typedef enum 
{
	HTTPC_RSP_DIGEST_REALM = 0,  //the realm
	HTTPC_RSP_DIGEST_DOMAIN,     //the domain
	HTTPC_RSP_DIGEST_NONCE,      //the nonce
	HTTPC_RSP_DIGEST_QOP,        //the qop
	HTTPC_RSP_DIGEST_OPAQUE,     //the opaque
	HTTPC_RSP_DIGEST_STALE,      //the stale
	HTTPC_RSP_DIGEST_ALGORITHM,  //the algorithm
    HTTPC_RSP_DIGEST_MAX
}HTTPC_RSP_DIGEST_E;

typedef enum 
{
	HTTPC_AUTH_SCHEME_UNKNOWN = 0,
	HTTPC_AUTH_SCHEME_BASIC,         //the basic scheme
	HTTPC_AUTH_SCHEME_DIGEST         //the digest scheme
} HTTPC_AUTH_SCHEME_E;


typedef struct RESPONSE_DIGEST
{
	uint8* realm_ptr;       //the realm param
	uint8* domain_ptr;      //the domain param
	uint8* nonce_ptr;       //the nonce param
	uint8* qop_ptr;         //the qop param
	uint8* opaques_ptr;     //the opaques param
	uint8* stale_ptr;       //the stale param
	uint8* algorithm_ptr;   //the algorithm param
}HTTPC_RESPONSE_DIGEST_T;

typedef struct RESOPNSE_DIGEST_AUTHEN_MAP
{
	char*					header_ptr; //the header 
	HTTPC_RSP_DIGEST_E		type;       //the response digest type
}HTTPC_RESOPNSE_DIGEST_AUTHEN_MAP_T;
//cache
typedef struct CACHE_DIRECTIVE
{
    uint32 cache_type;      //the cache type
    int32  delta_seconds;   //the delta seconds
}HTTPC_CACHE_DIRECTIVE_T;

typedef struct HTTPC_CACHE_MAP
{
    char*   string_ptr; //the string
    uint32  cache_type; //the cache type
}HTTPC_CACHE_MAP_T;

typedef struct _CACHE
{
	CFL_LIST_HANDLE     cache_list_handle;                      //the cache list handle
	BOOLEAN             is_cache_enable;                        //if cache is enable
    HTTPC_UNICODE_T      index_file[HTTPC_MAX_FILE_NAME_LEN + 1]; //the cache index file name
	HTTPC_UNICODE_T      save_path[HTTPC_MAX_FILE_NAME_LEN + 1];  //the cache save path
	uint32				max_size;                               //the max cache size
	uint32				max_count;                              //the max cache count
	uint32				curr_count;                             //the current cache count
	uint32				curr_size;                              //the current cache size
}HTTPC_CACHE_T;

typedef struct CACHE_NODE
{
    char*       file_name_ptr;      //the cache file name
	uint32      size;               //the cache size
	int32       expires;            //the expires of the cache
	BOOLEAN     is_cache_revalidate;//if the cache is revalidate
	char*       etag_ptr;           //the etag
	char*       last_modified_ptr;  //the last modified
	char*       content_type_ptr;   //the content type
}HTTPC_CACHE_NODE_T;

//chunk
typedef enum 
{
    HTTPC_CHUNK_STATE_FIRST = 0, 
    HTTPC_CHUNK_STATE_HEX,       //the hex state
    HTTPC_CHUNK_STATE_POSTHEX,   //the post hex state
    HTTPC_CHUNK_STATE_CR,        //the cr state
    HTTPC_CHUNK_STATE_DATA,      //the data state
    HTTPC_CHUNK_STATE_POSTCR,    //the post cr state
    HTTPC_CHUNK_STATE_POSTLF,    //the post lf state
    HTTPC_CHUNK_STATE_STOP,      //the stop state
    HTTPC_CHUNK_STATE_LAST 
} HTTPC_CHUNK_STATE_E;

typedef enum 
{
    HTTPC_CHUNK_OK = 0,              //ok
    HTTPC_CHUNK_ERROR_PARAM,         //the param error
    HTTPC_CHUNK_ERROR_TOO_LONG_HEX,  //the hex is too long
    HTTPC_CHUNK_ERROR_ILLEGAL_HEX,   //the hex is illegal
    HTTPC_CHUNK_ERROR_BAD_CHUNK,     //the chunk is bad
    HTTPC_CHUNK_ERROR_WRITE,         //write chunk error
    HTTPC_CHUNK_ERROR_STATE,         //the chunk state is error
    HTTPC_CHUNK_ERROR_STOP,          //the chunk stop
    HTTPC_CHUNK_ERROR_LAST
} HTTPC_CHUNK_ERROR_E;
//hsm
typedef enum 
{
	HTTPC_METHOD_INVALID  	= 0x00, //the invalid method
	HTTPC_METHOD_OPTIONS	    = 0x01, //the options method
	HTTPC_METHOD_GET		    = 0x02, //the get method
	HTTPC_METHOD_HEAD	    = 0x04, //the head method   
	HTTPC_METHOD_POST	    = 0x08, //the post method    
	HTTPC_METHOD_PUT		    = 0x10, //the put method
	HTTPC_METHOD_DELETE	    = 0x11, //the delete method
	HTTPC_METHOD_TRACE	    = 0x12, //the trace method
	HTTPC_METHOD_PATCH       = 0x13, //the patch method
	HTTPC_METHOD_CONNECT	    = 0x14, //the connect method
    HTTPC_METHOD_MAX		    =0x18
}HTTPC_METHOD_E;

typedef enum 
{
	HTTPC_TRANSFER_ENCODING_UNKNOWN = 0, //the unknown transfer encoding
	HTTPC_TRANSFER_ENCODING_CHUNKED,     //the chunked transfer encoding
    TRANSFER_ENCODING_MAX
}HTTPC_TRANSFER_ENCODING_E;

typedef struct CHUNK
{
    uint8				hex_buffer[HTTPC_CHUNK_MAXNUM_SIZE + 1]; //the hex buffer of the chunk
	uint32				hex_index;                              //the hex index
	HTTPC_CHUNK_STATE_E  state;                                  //the chunk state
	uint32				data_size;                              //the data size of the chunk
	uint32				data_left;                              //the data left of the chunk
} HTTPC_CHUNK_T;
//cookie
typedef struct _COOKIE_PARAM
{
    char*           domain_ptr;     //the domain param
    NETMGR_PORT_T   port;           //the port param
    char*           path_ptr;       //the path param
    char*           name_ptr;       //the name param
    char*           value_ptr;      //the value param
    char*           expire_str_ptr; //the expire param
    char*           other_name_ptr;
    char*           other_value_ptr;
    BOOLEAN         is_secure;      //the secure param
}HTTPC_COOKIE_PARAM_T;

typedef struct STR_TOKEN
{
    char* str_token_ptr;
}HTTPC_STR_TOKEN_T;

typedef enum{
    COOKIE_DOMAIN,
    COOKIE_PORT,
    COOKIE_PATH,
    COOKIE_FIELD,
    COOKIE_OTHER
} COOKIE_NODE_TYPE;

typedef struct COOKIE_NODE
{
	int32               expires;        //the expires of this cookie   
	BOOLEAN             is_tail_match;  //if the cookie is tail match
	char*               version_ptr;    //the cookie version 
	int32               max_age;        //the max age of the cookie
	HTTPC_COOKIE_PARAM_T cookie_param;   //the cookie param
	COOKIE_NODE_TYPE    node_type;

	struct COOKIE_NODE* parent_ptr;
	struct COOKIE_NODE* next_ptr;
	struct COOKIE_NODE* child_list_ptr;
	uint32              child_count;
}HTTPC_COOKIE_NODE_T;

typedef struct _COOKIE
{
    struct COOKIE_NODE* cookie_list_ptr;                        //the domain cookie list
	BOOLEAN			    is_enable;                              //if the cookie is enable
	BOOLEAN             is_init;
	HTTPC_UNICODE_T      file_name[HTTPC_MAX_FILE_NAME_LEN + 1];  //the cookie file name
	uint32              max_size;                               //the max cookie size
	uint32              max_count;                              //the max cookie count
	uint32              cookies_of_domain;                      //the cookies of one domain
	uint32              curr_count;                             //the current cookie count
	uint32              curr_domain_count;                      //the current domain count
	uint32              curr_size;                              //the current cookie size
	uint32              ref_count;
	SCI_MUTEX_PTR       cookie_mutex;                           //a mutex pointer 
}HTTPC_COOKIE_T;
//header
typedef HTTPC_ERROR_E (*HttpHeaderParserCallback)(void* data_ptr, char* token_ptr, char* value_ptr);

typedef struct HEADER_HANDLER
{
    struct HEADER_HANDLER*      next_ptr;   //the next header handler
    char*                       token_ptr;  //the header name
    HttpHeaderParserCallback    function;   //the callback function
}HTTPC_HEADER_HANDLER_T;

typedef struct HEADER_PARSER_CALLBACK
{
    char*                       string_ptr;     //the string
	HttpHeaderParserCallback    header_handler; //the header handler
} HTTPC_HEADER_PARSER_CALLBACK_T;

typedef struct HTTPC_HEADER_NODE
{
    struct HTTPC_HEADER_NODE*	next_ptr;   //the next header
	char*						name_ptr;   //the header name
	char*						value_ptr;  //the header value
} HTTPC_HEADER_NODE_T;

typedef struct _HEADER_FIELD
{
    char* token_ptr;    //header name 
    char* value_ptr;    //header value
}HTTPC_HEADER_FIELD_T;
//request
typedef struct REQUEST_DIGEST
{
    char* username_ptr; //the user name param
    char* password_ptr; //the password param
    char* realm_ptr;    //the realm param
    char* qop_ptr;      //the qop param
    char* opaques_ptr;  //the opaques param
    char* response_ptr; //the response param
    char* uri_ptr;      //the uri param
    char* cnonce_ptr;   //the cnonce param
    char* nonce_ptr;    //the nonce param
    char* nc_ptr;       //the nc param
    char* method_ptr;   //the method param
    char* algorithm_ptr;//the algorithm param
} HTTPC_REQUEST_DIGEST_T;

typedef struct HTTPC_DIGEST_CALC_A1
{
    char* username_ptr; //the user name param
    char* password_ptr; //the password param
    char* realm_ptr;    //the realm param
    char* cnonce_ptr;   //the cnonce param
    char* nonce_ptr;    //the nonce param
    char* algorithm_ptr;//the algorithm param
}HTTPC_DIGEST_CALC_A1;

typedef struct HTTPC_DIGEST_CALC_RESPONSE
{
    char* qop_ptr;      //the qop param
    char* uri_ptr;      //the uri param
    char* cnonce_ptr;   //the cnonce param
    char* nonce_ptr;    //the nonce param
    char* nc_ptr;       //the nc param
    char* method_ptr;   //the method param
}HTTPC_DIGEST_CALC_RESPONSE;

typedef struct _REQUEST_PARAM
{
    HTTPC_APP_INSTANCE_T		app_instance;   //the app instance
    HTTPC_CONTEXT_ID_T       context_id;     //the context id
    BLOCK_ID				sender;         //the app task id
}HTTPC_REQUEST_PARAM_T;

typedef struct _REQUEST
{
    BOOLEAN                 is_finished;                //if the request is finished
    BOOLEAN                 need_net_prog_ind;          //if the request need net prog ind
    BOOLEAN                 is_body_send;               //if the request's body has been send
    BOOLEAN                 is_security;                //if the request is security
    BOOLEAN                 read_flag;                  //the read data flag
    BOOLEAN                 close_flag;                 //the close flag
    BOOLEAN                 is_received_header_rsp;     //if the data has handled
    BOOLEAN                 is_deal_auth;               //if has deal auth
    BOOLEAN                 is_proxy_connected;         //if proxy has connected
    BOOLEAN                 is_use_relative_url;        //is use relative url
    BOOLEAN                 read_success_flag;          //read success flag
    BOOLEAN                 is_use_post_after_redirect; //is still use post after redirect
    BOOLEAN                 is_use_patch_after_redirect; //is still use patch after redirect
    BOOLEAN                 is_post_method;             //if the method is post
    BOOLEAN                 is_patch_method;            //if the method is patch
    BOOLEAN                 has_read_event;             //if has read event
    BOOLEAN                 has_range;                  //if has range header
    BOOLEAN                 has_location;               //if has location header
    HTTPC_METHOD_E           method;                     //the http method
    uint32					uri_str_len;                //the request uri len
    uint32					format_data_len;            //the format data len
    uint32                  send_recv_data_len;         //the send or receive data len
    uint32                  state_code;                 //the state code
    uint32                  nc;                         //the nc of digest
    uint32                  left_cache_data;            //the cache data that not read
    uint32                  has_read_cache_data;        //the cache data that has read
    uint32                  current_cache_read;         //the current cache read bytes
    uint32                  cache_size;                 //the cache file size
    uint32                  total_send_data_len;        //the total send data length
    uint32                  request_id;                 //the request id
	uint32                  redirect_count;             //the redirect count
	uint32                  recv_timeout;
    char*					uri_str_ptr;                //the request uri
    char*					format_data_ptr;            //the format data
    char*                   cache_data_ptr;             //the cache data
    NETMGR_URL_T const*     url_ptr;                    //the url has parsed
    HTTPC_AUTH_PARAM_T*      auth_ptr;                   //the auth param
    HTTPC_AUTH_SCHEME_E      scheme;                     //the auth scheme
    HTTPC_AUTH_TYPE_E        auth_type;                  //the auth type
    CFL_LIST_HANDLE         header_list_handle;         //the header list handle
    HTTPC_REQUEST_DIGEST_T*  request_digest_ptr;         //the request's digest
    HTTPC_GET_DATA_T         get_data_param;             //the get data param that app set
    HTTPC_POST_BODY_T        post_body;                  //the post body that app set
    HTTPC_PATCH_BODY_T       patch_body;                 //the patch body that app set
    HTTPC_HEAD_TRANS_STYLE_T head_trans_style;           //the head trans style
    HTTPC_REQUEST_PARAM_T    request_param;              //the request param
    SFS_HANDLE              content_file_handle;        //the content file handle
    SFS_HANDLE              cache_file_handle;          //the cache file handle
    char*					recv_data_ptr;              //the receive data
    uint16					retry;                      //the retry times
	uint16                  proxy_connect_retry;        //the proxy connect retry times
    void*                   hsm_ptr;                    //the hsm ptr
    NETMGR_REQUEST_HANDLE   net_request_handle;         //the net request handle
    NETMGR_HOST_HANDLE      host_handle;                //the host handle
}HTTPC_REQUEST_T;
//response
typedef struct RESPONSE
{
    uint32                      content_buf_size;       //the content buffer len 
	uint32                      current_len;            //the current len of receive data 
    uint32                      max_content_len;        //the max content len
	uint32                      content_total_len;      //the total len of content
	uint32                      header_len;             //the header len
    uint32                      net_progress_data_len;  //the data len for net progress
	char*                       header_ptr;             //the header
    char*                       content_ptr;            //the content 
    char*                       max_content_ptr;        //the max content buf
    char*                       etag_ptr;
    char*                       last_modified_ptr;
    char*                       connect_buf_ptr;
    HTTPC_RESPONSE_DIGEST_T*     digest_ptr;             //the response digest param
	HTTPC_HEADER_HANDLER_T*      header_handler_ptr;     //the header handler
	BOOLEAN					    is_header_parsed;       //if the header has been parsed 
	BOOLEAN					    is_header_handled;      //if the header has been handled
	BOOLEAN					    is_cache_need_save;     //if the cache need to save
	BOOLEAN					    is_cache_revalidate;    //if the cache is revalidate
    BOOLEAN                     has_data;               //have response data
    BOOLEAN                     is_expired;             //if the response is expired
    BOOLEAN                     is_connect_header_received;
	HTTPC_CACHE_NODE_T*          cache_ptr;              //the cache node ptr
    HTTPC_HEADER_NODE_T*         header_node_ptr;        //the header node ptr
    HTTPC_RSP_HEADER_INFO_T      rsp_header_info;        //the response header info
	HTTPC_TRANSFER_ENCODING_E    transfer_encoding;      //the transfer encoding
    HTTPC_CACHE_DIRECTIVE_T*     cache_directive_ptr;    //the cache directive ptr
	HTTPC_CHUNK_T*               chunk_ptr;              //the chunk ptr
	void*                       hsm_ptr;                //the hsm ptr
    BOOLEAN                     is_connection_close;//if the server's connection is close
    uint32                      progress_time; //seconds
    int32                       expires; //seconds
}HTTPC_RESPONSE_T;
//session
    
typedef struct HTTPC_SESSION
{
	BOOLEAN          is_pre_req_security;        //if the pre request is security
	BOOLEAN          is_first_req;
    HTTPC_APP_MODULE_ID_T    module_id;  //module id of service,eg, webkit, dorado, dac
    HTTPC_CACHE_T*    cache_ptr;
} HTTPC_SESSION_T;

typedef struct _HTTPC_REQUEST_LIST_NODE
{
    uint32 hsm_id;
    uint32 request_id;
    uint32 request_id_address;
}HTTPC_REQUEST_LIST_NODE_T;


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
