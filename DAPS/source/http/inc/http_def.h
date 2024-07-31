/*****************************************************************************
** File Name:      http_def.h                                                *
** Author:          li.li                                                    *
** Date:           14/11/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the HTTP define             *
*****************************************************************************/

#ifndef _http_def_H_
#define _http_def_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
//#include "netSystem.h"
#include "http_api.h"
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
#define HTTP_CACHE_UNKNOWN                           0      //unknown type
#define HTTP_CACHE_PUBLIC                            0x001  //public param
#define HTTP_CACHE_PRIVATE                           0x002  //private param
#define HTTP_CACHE_NO_CACHE                          0x004  //no cache param
#define HTTP_CACHE_NO_STORE                          0x008  //no store param
#define HTTP_CACHE_NO_TRANSFORM                      0x010  //no transform param
#define HTTP_CACHE_MUST_REVALIDATE                   0x020  //must revalidate param
#define HTTP_CACHE_PROXY_REVALIDATE                  0x040  //proxy revalidate param
#define HTTP_CACHE_MAX_AGE                           0x080  //max age param
#define HTTP_CACHE_S_MAXAGE                          0x100  //s max age param
#define HTTP_CACHE_EXTENSION                         0x200  //extension param

#define	HTTP_CACHE_INDEX_FILE       "index"  //cache index file name "index"
#define	HTTP_CACHE_DEFAULT_SIZE     4000000 //the max cache size
#define HTTP_CACHE_DEFAULT_MAX_COUNT  100   //the max cache count
#define HTTP_CACHE_MAX_READ_LEN     5000    //the max len when read file

#define HTTP_CACHE_HASH_LEN         16      //the hash buffer len
#define HTTP_CACHE_MAX_INFO_LEN     1024    //the max cache info len
//chunk
#define HTTP_CHUNK_MAXNUM_SIZE      16      //the chunk max num size
//cookie
#define HTTP_COOKIE_MAX_SIZE        (30*1024)   //the max cookie size
#define HTTP_COOKIE_MAX_LEN         (10*1024)   //the max cookie read len
#define HTTP_COOKIE_INVALID_STRING  ""

#define HTTP_HSM_CONNECT_TIMEOUT                30000   //the max connect time
#define HTTP_HSM_SEND_TIMEOUT                   60000   //the max send time
#define HTTP_HSM_RECV_TIMEOUT                   120000   //the max receive time of one packet
#define HTTP_HSM_HANDSHAKE_TIMEOUT              60000   //the handshake time
#define HTTP_HSM_RECV_PROGRESS_TIMEOUT          500
#define HTTP_HSM_RETRY_MAXCOUNT                 2       //the retry connect number
#define HTTP_PROXY_CONNECT_RETRY_MAXCOUNT       2       //the proxy connect retry number
#define HTTP_SENDBUF_MAX_SIZE                   6000    //the max send buffer size

#define HTTP_MAXLEN_DIGEST_REPONSE_PARAM_TOTAL  200     //the max len of digest response param
#define HTTP_MAXLEN_DIGEST_REPONSE_PARAM        50      //the max len of digest response token param
#define HTTP_MAXLEN_AUTHENLEN_BASIC             300     //the max len of basic auth len
#define HTTP_MAXLEN_AUTHENLEN_DIGEST            1000    //the max len of digest auth len
#define HTTP_MAX_REDIRECT_COUNT                 50      //the max redirect count
//hsm

#define HTTP_SLASH_STRING           "/"
#define HTTP_CRLF_STRING            "\r\n"
#define HTTP_CRLFX2_STRING          "\r\n\r\n"
#define HTTP_GET_STRING             "GET"
#define HTTP_POST_STRING            "POST"
#define HTTP_PATCH_STRING           "PATCH"
#define HTTP_HEAD_STRING            "HEAD"
#define HTTP_OPTIONS_STRING         "OPTIONS"
#define HTTP_DELETE_STRING         "DELETE"
#define HTTP_CNONCE_LEN             32
#define HTTP_METHOD_LEN             10
#define HTTP_NC_STRING              "00000000"
#define HTTP_NC_STRING_LEN          8
#define HTTP_MD5_SESS_STRING        "MD5-SESS"
        
#define HTTP_AUTH_BASIC             "Basic "
#define HTTP_AUTH_DIGEST            "Digest "
#define HTTP_AUTH_USERNMAE          "username=\""
#define HTTP_AUTH_REALM             "\", realm=\""
#define HTTP_AUTH_QOP               "\", qop=\""
#define HTTP_AUTH_ALG               "\", algorithm=\""
#define HTTP_AUTH_URI               "\", uri=\""
#define HTTP_AUTH_NONCE             "\", nonce=\""
#define HTTP_AUTH_NC                "\", nc=\""
#define HTTP_AUTH_CNONCE            "\", cnonce=\""
#define HTTP_AUTH_OPAQUE            "\", opaque=\""
#define HTTP_AUTH_RESPONSE          "\", response=\""

#define HTTP_COOKIE_PARAM_PORT      "PORT"
#define HTTP_COOKIE_PARAM_PATH      "PATH"
#define HTTP_COOKIE_PARAM_DOMAIN    "DOMAIN"
#define HTTP_COOKIE_PARAM_VERSION   "VERSION"
#define HTTP_COOKIE_PARAM_MAXAGE    "MAX-AGE"
#define HTTP_COOKIE_PARAM_EXPIRES   "EXPIRES"
#define HTTP_COOKIE_PARAM_SECURE    "SECURE"
#define HTTP_COOKIE_PARAM_HTTPONLY  "httponly"

#define HTTP_VERSION_STRING                     " HTTP/1.1"
#define HTTP_CONNECT_STRING                     "CONNECT "
#define HTTPS_STRING                            "https://"
#define HTTPS_PORT_STRING                       "443"
#define HTTP_USER_AGENT_STRING                  "User-Agent"
#define HTTP_USER_AGENT_COLON_STRING            "User-Agent: "
#define HTTP_HOST_COLON_STRING                  "Host: "
#define HTTP_PORT_LEN                           10
#define HTTP_VERSION10_STRING                   "HTTP/1.0 200"
#define HTTP_VERSION11_STRING                   "HTTP/1.1 200"
#define HTTP_HTTPS_STRING                       "https"
#define HTTP_CONNECTION_CLOSE_STRING            "close"
#define HTTP_CONNECTION_KEEP_ALIVE_STRING       "keep-alive"
#define HTTP_ENCODING_GZIP_STRING               "gzip"

#define HTTP_GET_SPACE_STRING                   "GET "
#define HTTP_POST_SPACE_STRING                  "POST "
#define HTTP_PATCH_SPACE_STRING                  "PATCH "
#define HTTP_HEAD_SPACE_STRING                  "HEAD "
#define HTTP_OPTIONS_SPACE_STRING               "OPTIONS "
#define HTTP_DELETE_SPACE_STRING               "DELETE "
#define HTTP_DEFAULT_PORT                       80
#define HTTP_CONTENT_TYPE_STRING                "Content-Type"
#define HTTP_COOKIE_COLON_STRING                "Cookie:"
#define HTTP_PROXY_AUTH_STRING                  "Proxy-Authorization: "
#define HTTP_ETAG_COLON_STRING                  "If-None-Match: "
#define HTTP_LAST_MODIFIED_COLON_STRING         "If-Modified-Since: "
#define HTTP_AUTH_COLON_STRING                  "Authorization: "
#define HTTP_CONTENT_LENGTH_LEN                 10
#define HTTP_CONTENT_TYPE_COLON_STRING          "Content-Type: "
#define HTTP_CONTENT_LENGTH_COLON_STRING        "Content-Length: "
#define HTTP_HTTP_STRING                        "HTTP"
#define HTTP_CHUNKED_STRING                     "CHUNKED"
#define HTTP_HTTP_COLON_STRING                  "HTTP:"
#define HTTP_HTTP_COLON_SLASH_STRING            "http://"
#define HTTP_HTTPS_COLON_SLASH_STRING           "https://"
#define HTTP_DIGEST_STRING                      "DIGEST"
#define HTTP_RANGE_STRING                       "Range"
#define HTTP_NOT_EXPIRE_STRING                  "not-expire"

#ifdef WIN32
#define HTTP_RAND_MAX_NUM                       0x7fff
#else
#define HTTP_RAND_MAX_NUM                       0x7fffffff
#endif
#define HTTP_BLANK_NUM                          0x20
#define HTTP_STATE_CODE_LEN                     4
#define HTTP_SLASH_0_STRING                     "/1.0"
#define HTTP_SLASH_1_STRING                     "/1.1"
#define HTTP_TWENTY_K                           (20*1024)
#define HTTP_PROGRESS_COUNT                     100
#define HTTP_PROGRESS_TIME                      1000
#define HTTP_TRACE                              SCI_TRACE_LOW
	
#define HTTP_400_STRING                         "<html><head><title>http 400 bad request</title><body>http 400 bad request</body></head></html>"
#define HTTP_402_STRING                         "<html><head><title>http 402 payment required</title><body>http 400 payment required</body></head></html>"
#define HTTP_403_STRING                         "<html><head><title>http 403 forbidden</title><body>http 403 forbidden</body></head></html>"
#define HTTP_404_STRING                         "<html><head><title>http 404 not found</title><body>http 404 not found</body></head></html>"
#define HTTP_405_STRING                         "<html><head><title>http 405 method not allowed</title><body>http 405 method not allowed</body></head></html>"
#define HTTP_406_STRING                         "<html><head><title>http 406 not acceptable</title><body>http 406 not acceptable</body></head></html>"
#define HTTP_408_STRING                         "<html><head><title>http 408 request timeout</title><body>http 408 request timeout</body></head></html>"
#define HTTP_409_STRING                         "<html><head><title>http 409 conflict</title><body>http 409 conflict</body></head></html>"
#define HTTP_410_STRING                         "<html><head><title>http 410 gone</title><body>http 410 gone</body></head></html>"
#define HTTP_411_STRING                         "<html><head><title>http 411 length required</title><body>http 411 length required</body></head></html>"
#define HTTP_412_STRING                         "<html><head><title>http 412 precondition failed</title><body>http 412 precondition failed</body></head></html>"
#define HTTP_413_STRING                         "<html><head><title>http 413 request entity too large</title><body>http 413 request entity too large</body></head></html>"
#define HTTP_414_STRING                         "<html><head><title>http 414 request url too long</title><body>http 414 request url too long</body></head></html>"
#define HTTP_415_STRING                         "<html><head><title>http 415 unsupported media type</title><body>http 415 unsupported media type</body></head></html>"
#define HTTP_416_STRING                         "<html><head><title>http 416 requested range not satisfiable</title><body>http 416 requested range not satisfiable</body></head></html>"
#define HTTP_417_STRING                         "<html><head><title>http 417 exceptation failed</title><body>http 417 exceptation failed</body></head></html>"
#define HTTP_422_STRING                         "<html><head><title>http 422 unprocessable request</title><body>http 422 unprocessable request</body></head></html>"
#define HTTP_500_STRING                         "<html><head><title>http 500 internal server error</title><body>http 500 internal server error</body></head></html>"
#define HTTP_501_STRING                         "<html><head><title>http 501 not implemented</title><body>http 501 not implemented</body></head></html>"
#define HTTP_502_STRING                         "<html><head><title>http 502 bad gateway</title><body>http 502 bad gateway</body></head></html>"
#define HTTP_503_STRING                         "<html><head><title>http 503 service unavailable</title><body>http 503 service unavailable</body></head></html>"
#define HTTP_504_STRING                         "<html><head><title>http 504 gateway timeout</title><body>http 504 gateway timeout</body></head></html>"
#define HTTP_505_STRING                         "<html><head><title>http 505 HTTP version not supported</title><body>http 505 HTTP version not supported</body></head></html>"

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
//event
typedef enum 
{
    HTTP_EVENT_TYPE_NULL = HSM_USER_SIG + 1,
    HTTP_REQ,
    HTTP_CONNECT_SERVER,            //connect server
    HTTP_CONNECT_SUCC,              //connect success
    HTTP_CONNECT_FAIL,              //connect fail
    HTTP_HANDSHAKE,                 //handshake
    HTTP_PROXY_CONNECT,             //proxy connect
    HTTP_SEND_DATA,                 //send data
    HTTP_READ_DATA,                 //read data
    HTTP_READ_DATA_PROGRESS,
    HTTP_HANDSHAKE_SUCC,            //handshake success
    HTTP_HANDSHAKE_FAIL,            //handshake fail
    HTTP_READ_SUCC,                 //read success
    HTTP_READ_FAIL,                 //read fail
    HTTP_PROXY_CONNECT_SUCC,        //proxy connect success
    HTTP_PROXY_CONNECT_FAIL,        //proxy connect fail
    HTTP_SEND_SUCC,                 //send success
    HTTP_SEND_FAIL,                 //send fail
    HTTP_SERVER_CLOSE,              //server close
    HTTP_SERVER_EXCEPTION_CLOSE,    //server exception close
    HTTP_TIME_OUT,                  //time out
    HTTP_RECV_PROGRESS_TIME_OUT,
    HTTP_HANDSHAKE_TIME_OUT,
    HTTP_SSL_SEND_END,              //ssl send end
    HTTP_ERROR,                     //error
    HTTP_READ_CACHE,                //read cache
    HTTP_READ_CACHE_SUCC,           //read cache success
    HTTP_READ_CACHE_FAIL,           //read cache fail
    HTTP_DESTROY_HSM,               //destroy hsm
    HTTP_SSL_CANCEL,                //ssl cancel
    HTTP_EVENT_MAX
} HTTP_EVENT_TYPE_E;

typedef struct _HTTP_EVENT_NODE
{
    HTTP_EVENT_TYPE_E           event;      //the http event
    struct _HTTP_EVENT_NODE*    next_ptr;   //the next
} HTTP_EVENT_NODE_T;

typedef struct _HTTP_EVENT_QUEUE
{
    HTTP_EVENT_NODE_T* front_ptr;   //the front
    HTTP_EVENT_NODE_T* rear_ptr;    //the rear
} HTTP_EVENT_QUEUE_T;
//auth
typedef enum 
{
	HTTP_RSP_DIGEST_REALM = 0,  //the realm
	HTTP_RSP_DIGEST_DOMAIN,     //the domain
	HTTP_RSP_DIGEST_NONCE,      //the nonce
	HTTP_RSP_DIGEST_QOP,        //the qop
	HTTP_RSP_DIGEST_OPAQUE,     //the opaque
	HTTP_RSP_DIGEST_STALE,      //the stale
	HTTP_RSP_DIGEST_ALGORITHM,  //the algorithm
    HTTP_RSP_DIGEST_MAX
}HTTP_RSP_DIGEST_E;

typedef enum 
{
	HTTP_AUTH_SCHEME_UNKNOWN = 0,
	HTTP_AUTH_SCHEME_BASIC,         //the basic scheme
	HTTP_AUTH_SCHEME_DIGEST         //the digest scheme
} HTTP_AUTH_SCHEME_E;


typedef struct RESPONSE_DIGEST
{
	uint8* realm_ptr;       //the realm param
	uint8* domain_ptr;      //the domain param
	uint8* nonce_ptr;       //the nonce param
	uint8* qop_ptr;         //the qop param
	uint8* opaques_ptr;     //the opaques param
	uint8* stale_ptr;       //the stale param
	uint8* algorithm_ptr;   //the algorithm param
}HTTP_RESPONSE_DIGEST_T;

typedef struct RESOPNSE_DIGEST_AUTHEN_MAP
{
	char*					header_ptr; //the header 
	HTTP_RSP_DIGEST_E		type;       //the response digest type
}HTTP_RESOPNSE_DIGEST_AUTHEN_MAP_T;
//cache
typedef struct CACHE_DIRECTIVE
{
    uint32 cache_type;      //the cache type
    int32  delta_seconds;   //the delta seconds
}HTTP_CACHE_DIRECTIVE_T;

typedef struct HTTP_CACHE_MAP
{
    char*   string_ptr; //the string
    uint32  cache_type; //the cache type
}HTTP_CACHE_MAP_T;

typedef struct _CACHE
{
	CFL_LIST_HANDLE     cache_list_handle;                      //the cache list handle
	BOOLEAN             is_cache_enable;                        //if cache is enable
    HTTP_UNICODE_T      index_file[HTTP_MAX_FILE_NAME_LEN + 1]; //the cache index file name
	HTTP_UNICODE_T      save_path[HTTP_MAX_FILE_NAME_LEN + 1];  //the cache save path
	uint32				max_size;                               //the max cache size
	uint32				max_count;                              //the max cache count
	uint32				curr_count;                             //the current cache count
	uint32				curr_size;                              //the current cache size
}HTTP_CACHE_T;

typedef struct CACHE_NODE
{
    char*       file_name_ptr;      //the cache file name
	uint32      size;               //the cache size
	int32       expires;            //the expires of the cache
	BOOLEAN     is_cache_revalidate;//if the cache is revalidate
	char*       etag_ptr;           //the etag
	char*       last_modified_ptr;  //the last modified
	char*       content_type_ptr;   //the content type
}HTTP_CACHE_NODE_T;

//chunk
typedef enum 
{
    HTTP_CHUNK_STATE_FIRST = 0, 
    HTTP_CHUNK_STATE_HEX,       //the hex state
    HTTP_CHUNK_STATE_POSTHEX,   //the post hex state
    HTTP_CHUNK_STATE_CR,        //the cr state
    HTTP_CHUNK_STATE_DATA,      //the data state
    HTTP_CHUNK_STATE_POSTCR,    //the post cr state
    HTTP_CHUNK_STATE_POSTLF,    //the post lf state
    HTTP_CHUNK_STATE_STOP,      //the stop state
    HTTP_CHUNK_STATE_LAST 
} HTTP_CHUNK_STATE_E;

typedef enum 
{
    HTTP_CHUNK_OK = 0,              //ok
    HTTP_CHUNK_ERROR_PARAM,         //the param error
    HTTP_CHUNK_ERROR_TOO_LONG_HEX,  //the hex is too long
    HTTP_CHUNK_ERROR_ILLEGAL_HEX,   //the hex is illegal
    HTTP_CHUNK_ERROR_BAD_CHUNK,     //the chunk is bad
    HTTP_CHUNK_ERROR_WRITE,         //write chunk error
    HTTP_CHUNK_ERROR_STATE,         //the chunk state is error
    HTTP_CHUNK_ERROR_STOP,          //the chunk stop
    HTTP_CHUNK_ERROR_LAST
} HTTP_CHUNK_ERROR_E;
//hsm
typedef enum 
{
	HTTP_METHOD_INVALID  	= 0x00, //the invalid method
	HTTP_METHOD_OPTIONS	    = 0x01, //the options method
	HTTP_METHOD_GET		    = 0x02, //the get method
	HTTP_METHOD_HEAD	    = 0x04, //the head method   
	HTTP_METHOD_POST	    = 0x08, //the post method    
	HTTP_METHOD_PUT		    = 0x10, //the put method
	HTTP_METHOD_DELETE	    = 0x11, //the delete method
	HTTP_METHOD_TRACE	    = 0x12, //the trace method
	HTTP_METHOD_PATCH       = 0x13, //the patch method
	HTTP_METHOD_CONNECT	    = 0x14, //the connect method
    HTTP_METHOD_MAX		    =0x18
}HTTP_METHOD_E;

typedef enum 
{
	HTTP_TRANSFER_ENCODING_UNKNOWN = 0, //the unknown transfer encoding
	HTTP_TRANSFER_ENCODING_CHUNKED,     //the chunked transfer encoding
    TRANSFER_ENCODING_MAX
}HTTP_TRANSFER_ENCODING_E;

typedef struct CHUNK
{
    uint8				hex_buffer[HTTP_CHUNK_MAXNUM_SIZE + 1]; //the hex buffer of the chunk
	uint32				hex_index;                              //the hex index
	HTTP_CHUNK_STATE_E  state;                                  //the chunk state
	uint32				data_size;                              //the data size of the chunk
	uint32				data_left;                              //the data left of the chunk
} HTTP_CHUNK_T;
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
}HTTP_COOKIE_PARAM_T;

typedef struct STR_TOKEN
{
    char* str_token_ptr;
}HTTP_STR_TOKEN_T;

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
	HTTP_COOKIE_PARAM_T cookie_param;   //the cookie param
	COOKIE_NODE_TYPE    node_type;

	struct COOKIE_NODE* parent_ptr;
	struct COOKIE_NODE* next_ptr;
	struct COOKIE_NODE* child_list_ptr;
	uint32              child_count;
}HTTP_COOKIE_NODE_T;

typedef struct _COOKIE
{
    struct COOKIE_NODE* cookie_list_ptr;                        //the domain cookie list
	BOOLEAN			    is_enable;                              //if the cookie is enable
	BOOLEAN             is_init;
	HTTP_UNICODE_T      file_name[HTTP_MAX_FILE_NAME_LEN + 1];  //the cookie file name
	uint32              max_size;                               //the max cookie size
	uint32              max_count;                              //the max cookie count
	uint32              cookies_of_domain;                      //the cookies of one domain
	uint32              curr_count;                             //the current cookie count
	uint32              curr_domain_count;                      //the current domain count
	uint32              curr_size;                              //the current cookie size
	uint32              ref_count;
	SCI_MUTEX_PTR       cookie_mutex;                           //a mutex pointer 
}HTTP_COOKIE_T;
//header
typedef HTTP_ERROR_E (*HttpHeaderParserCallback)(void* data_ptr, char* token_ptr, char* value_ptr);

typedef struct HEADER_HANDLER
{
    struct HEADER_HANDLER*      next_ptr;   //the next header handler
    char*                       token_ptr;  //the header name
    HttpHeaderParserCallback    function;   //the callback function
}HTTP_HEADER_HANDLER_T;

typedef struct HEADER_PARSER_CALLBACK
{
    char*                       string_ptr;     //the string
	HttpHeaderParserCallback    header_handler; //the header handler
} HTTP_HEADER_PARSER_CALLBACK_T;

typedef struct HTTP_HEADER_NODE
{
    struct HTTP_HEADER_NODE*	next_ptr;   //the next header
	char*						name_ptr;   //the header name
	char*						value_ptr;  //the header value
} HTTP_HEADER_NODE_T;

typedef struct _HEADER_FIELD
{
    char* token_ptr;    //header name 
    char* value_ptr;    //header value
}HTTP_HEADER_FIELD_T;
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
} HTTP_REQUEST_DIGEST_T;

typedef struct HTTP_DIGEST_CALC_A1
{
    char* username_ptr; //the user name param
    char* password_ptr; //the password param
    char* realm_ptr;    //the realm param
    char* cnonce_ptr;   //the cnonce param
    char* nonce_ptr;    //the nonce param
    char* algorithm_ptr;//the algorithm param
}HTTP_DIGEST_CALC_A1;

typedef struct HTTP_DIGEST_CALC_RESPONSE
{
    char* qop_ptr;      //the qop param
    char* uri_ptr;      //the uri param
    char* cnonce_ptr;   //the cnonce param
    char* nonce_ptr;    //the nonce param
    char* nc_ptr;       //the nc param
    char* method_ptr;   //the method param
}HTTP_DIGEST_CALC_RESPONSE;

typedef struct _REQUEST_PARAM
{
    HTTP_APP_INSTANCE_T		app_instance;   //the app instance
    HTTP_CONTEXT_ID_T       context_id;     //the context id
    BLOCK_ID				sender;         //the app task id
}HTTP_REQUEST_PARAM_T;

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
    HTTP_METHOD_E           method;                     //the http method
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
    HTTP_AUTH_PARAM_T*      auth_ptr;                   //the auth param
    HTTP_AUTH_SCHEME_E      scheme;                     //the auth scheme
    HTTP_AUTH_TYPE_E        auth_type;                  //the auth type
    CFL_LIST_HANDLE         header_list_handle;         //the header list handle
    HTTP_REQUEST_DIGEST_T*  request_digest_ptr;         //the request's digest
    HTTP_GET_DATA_T         get_data_param;             //the get data param that app set
    HTTP_POST_BODY_T        post_body;                  //the post body that app set
    HTTP_PATCH_BODY_T       patch_body;                 //the patch body that app set
    HTTP_HEAD_TRANS_STYLE_T head_trans_style;           //the head trans style
    HTTP_REQUEST_PARAM_T    request_param;              //the request param
    SFS_HANDLE              content_file_handle;        //the content file handle
    SFS_HANDLE              cache_file_handle;          //the cache file handle
    char*					recv_data_ptr;              //the receive data
    uint16					retry;                      //the retry times
	uint16                  proxy_connect_retry;        //the proxy connect retry times
    void*                   hsm_ptr;                    //the hsm ptr
    NETMGR_REQUEST_HANDLE   net_request_handle;         //the net request handle
    NETMGR_HOST_HANDLE      host_handle;                //the host handle
}HTTP_REQUEST_T;
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
    HTTP_RESPONSE_DIGEST_T*     digest_ptr;             //the response digest param
	HTTP_HEADER_HANDLER_T*      header_handler_ptr;     //the header handler
	BOOLEAN					    is_header_parsed;       //if the header has been parsed 
	BOOLEAN					    is_header_handled;      //if the header has been handled
	BOOLEAN					    is_cache_need_save;     //if the cache need to save
	BOOLEAN					    is_cache_revalidate;    //if the cache is revalidate
    BOOLEAN                     has_data;               //have response data
    BOOLEAN                     is_expired;             //if the response is expired
    BOOLEAN                     is_connect_header_received;
	HTTP_CACHE_NODE_T*          cache_ptr;              //the cache node ptr
    HTTP_HEADER_NODE_T*         header_node_ptr;        //the header node ptr
    HTTP_RSP_HEADER_INFO_T      rsp_header_info;        //the response header info
	HTTP_TRANSFER_ENCODING_E    transfer_encoding;      //the transfer encoding
    HTTP_CACHE_DIRECTIVE_T*     cache_directive_ptr;    //the cache directive ptr
	HTTP_CHUNK_T*               chunk_ptr;              //the chunk ptr
	void*                       hsm_ptr;                //the hsm ptr
    BOOLEAN                     is_connection_close;//if the server's connection is close
    uint32                      progress_time; //seconds
    int32                       expires; //seconds
}HTTP_RESPONSE_T;
//session
    
typedef struct HTTP_SESSION
{
	BOOLEAN          is_pre_req_security;        //if the pre request is security
	BOOLEAN          is_first_req;
    HTTP_APP_MODULE_ID_T    module_id;  //module id of service,eg, webkit, dorado, dac
    HTTP_CACHE_T*    cache_ptr;
} HTTP_SESSION_T;

typedef struct _HTTP_REQUEST_LIST_NODE
{
    uint32 hsm_id;
    uint32 request_id;
    uint32 request_id_address;
}HTTP_REQUEST_LIST_NODE_T;

//the struct of http hsm
SUBCLASS (HTTP_HSM_T,HSM_C)
BOOLEAN             is_busy;                    //if the hsm is busy
BOOLEAN             is_in_idle;                 //if the hsm is in idle state
HTTP_EVENT_QUEUE_T* event_queue_ptr;            //the event queue of hsm
HTTP_REQUEST_T*     request_ptr;                //the request of hsm
HTTP_RESPONSE_T*    response_ptr;               //the response of hsm
HTTP_SESSION_T*     session_ptr;                //the session of hsm
CFLTMR_HANDLE_T     timer_handle;               //the timer handle of hsm
CFLTMR_HANDLE_T     recv_progress_timer_handle; //the timer handle of hsm
CFLTMR_HANDLE_T     handshake_timer_handle;     //the handshake timer handle of hsm
NETMGR_HOST_HANDLE  host_handle;                //the host handle
uint32		    security_handle;            //the security handle
METHODS
END_CLASS

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
