/*****************************************************************************
** File Name:		http_api.h                                               *
** Author:			li.li                                                    *
** Date:			10/28/2008                                               *
** Copyright:		2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:		This file is used to describe the interface              *
*****************************************************************************/

#ifndef _HTTPC_API_H_
#define _HTTPC_API_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sfs.h"
#include "os_api.h"
#include "sci_types.h"
#ifdef SSL_SUPPORT
#include "ssl_api.h"
#endif
 /**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
#define  HTTPC_TRACE SCI_TRACE_LOW
#define HTTPC_CONTEXT_ID 44344300
#define HTTPC_REQUEST_ID 44344301
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define HTTPC_MAX_FILE_NAME_LEN      SFS_MAX_PATH	//max file name string length
#define HTTPC_INVALID_CONTEXT_ID     0				//invalid context id
#define HTTPC_INVALID_REQUEST_ID     0				//invalid request id   

#define HTTPC_RESPONSE_CODE_UNKNOW                               0
#define HTTPC_RESPONSE_CODE_CONTINUE                             100
#define HTTPC_RESPONSE_CODE_SWITCHING_PROTOCOLS                  101
#define HTTPC_RESPONSE_CODE_CREATED                              201
#define HTTPC_RESPONSE_CODE_OK                                   200
#define HTTPC_RESPONSE_CODE_ACCEPTED                             202
#define HTTPC_RESPONSE_CODE_NON_AUTHORITATIVE                    203
#define HTTPC_RESPONSE_CODE_NO_CONTENT                           204
#define HTTPC_RESPONSE_CODE_RESET_CONTENT                        205
#define HTTPC_RESPONSE_CODE_PARTIAL_CONTENT                      206
#define HTTPC_RESPONSE_CODE_MULTIPLE_CHOICES                     300
#define HTTPC_RESPONSE_CODE_MOVED_PERMANENTLY                    301
#define HTTPC_RESPONSE_CODE_FOUND                                302
#define HTTPC_RESPONSE_CODE_SEE_OTHER                            303
#define HTTPC_RESPONSE_CODE_NOT_MODIFIED                         304	
#define HTTPC_RESPONSE_CODE_USE_PROXY                            305
#define HTTPC_RESPONSE_CODE_TEMPORARY_REDIRECT                   307
#define HTTPC_RESPONSE_CODE_BAD_REQUEST                          400
#define HTTPC_RESPONSE_CODE_UNAUTHORIZED                         401
#define HTTPC_RESPONSE_CODE_PAYMENT_REQUIRED                     402
#define HTTPC_RESPONSE_CODE_FORBIDDEN                            403
#define HTTPC_RESPONSE_CODE_NOT_FOUND                            404
#define HTTPC_RESPONSE_CODE_METHOD_NOT_ALLOWED                   405
#define HTTPC_RESPONSE_CODE_NOT_ACCEPTABLE                       406
#define HTTPC_RESPONSE_CODE_PROXY_AUTHENTICATION_REQUIRED	     407
#define HTTPC_RESPONSE_CODE_REQUEST_TIMEOUT                      408
#define HTTPC_RESPONSE_CODE_CONFLICT                             409
#define HTTPC_RESPONSE_CODE_GONE                                 410		
#define HTTPC_RESPONSE_CODE_LENGTH_REQUIRED                      411
#define HTTPC_RESPONSE_CODE_PRECONDITION_FAILED                  412
#define HTTPC_RESPONSE_CODE_REQUEST_ENTITY_TOO_LARGE			 413
#define HTTPC_RESPONSE_CODE_REQUEST_URI_TOO_LONG				 414
#define HTTPC_RESPONSE_CODE_UNSUPPORTED_MEDIA_TYPE				 415
#define HTTPC_RESPONSE_CODE_REQUESTED_RANGE_NOT_SATISFIABLE	     416
#define HTTPC_RESPONSE_CODE_EXPECTATION_FAILED					 417
#define HTTPC_RESPONSE_CODE_UNPROCESSABLE_REQUEST				 422
#define HTTPC_RESPONSE_CODE_INTERNAL_SERVER_ERROR				 500
#define HTTPC_RESPONSE_CODE_NOT_IMPLEMENTED                      501
#define HTTPC_RESPONSE_CODE_BAD_GATEWAY                          502
#define HTTPC_RESPONSE_CODE_SERVICE_UNAVAILABLE                  503
#define HTTPC_RESPONSE_CODE_GATEWAY_TIMEOUT                      504
#define HTTPC_RESPONSE_CODE_VERSION_NOT_SUPPORTED                505

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/



typedef uint32 HTTPC_APP_MODULE_ID_T;    //module id of app
typedef uint32 HTTPC_CONTEXT_ID_T;       //context id that HTTP assigned for app
typedef uint32 HTTPC_REQUEST_ID_T;       //the request id that http assigned for app request
typedef uint32 HTTPC_APP_INSTANCE_T;     //the app instance
typedef uint16 HTTPC_UNICODE_T;          //the unicode encoding
typedef uint32 HTTPC_APP_PARAM_T;        //the app param

typedef enum
{
    HTTPC_SUCCESS = 0,                   //request success
    HTTPC_ERROR_FAILED,                  //request failed
    HTTPC_ERROR_INVALID_INPUT,           //input param is invalid
    HTTPC_ERROR_NO_MEMORY,               //alloc memory failed
    HTTPC_ERROR_FILE_OPEN,               //file open error
    HTTPC_ERROR_FILE_WRITE,              //file write error
    HTTPC_ERROR_FILE_NOT_FOUND,          //not found HTTPC file
    HTTPC_ERROR_FILE_DELETE,             //delete file error
    HTTPC_ERROR_FILE_GET_SIZE,           //get file size error
    HTTPC_ERROR_FILE_READ,               //read file error
    HTTPC_ERROR_NET_DATA_ERROR,          //the data from server is error
    HTTPC_ERROR_PROXY_CONNECT,           //proxy connect error
    HTTPC_ERROR_TIMEOUT,                 //time out
    HTTPC_ERROR_SERVER_CLOSE,            //server close
    HTTPC_ERROR_UNKNOWN,                 //unknown error
    HTTPC_ERROR_CONNECT,                 //connect error
    HTTPC_ERROR_HANDSHAKE,               //handshake error
    HTTPC_ERROR_PROXY_HANDSHAKE,         //proxy handshake error
    HTTPC_ERROR_RECV,                    //receive error
    HTTPC_ERROR_SEND,                    //send error
    HTTPC_ERROR_FILE_NO_SPACE,           //no space when deal with file
    HTTPC_ERROR_MAX
}HTTPC_ERROR_E;

typedef enum
{
    HTTPC_ENCODING_TYPE_NULL = 0,
    HTTPC_ENCODING_TYPE_GZIP,          //accept encoding header value: gzip
    HTTPC_ENCODING_TYPE_UNSUPPORT      //other is not supported
}HTTPC_ENCODING_TYPE_E;

typedef enum
{
    HTTPC_CONNECTION_DEFAULT = 0,//the default connection value, in this HTTPC, the default value is "keep-alive"
    HTTPC_CONNECTION_CLOSE,      //connection header value: close
    HTTPC_CONNECTION_KEEP_ALIVE, //connection header value: keep-alive
    HTTPC_CONNECTION_UNSUPPORT   //other is not support
}HTTPC_CONNECTION_E;


typedef enum
{
    HTTPC_DATA_TRANS_STYLE_UNKNOW, //APP don't know if use buffer or file
    HTTPC_DATA_TRANS_STYLE_BUFFER, //use buffer
    HTTPC_DATA_TRANS_STYLE_FILE	  //use file
}HTTPC_DATA_TRANS_STYLE_E;

typedef enum
{
    HTTPC_AUTH_TYPE_WWW,     //server need auth
    HTTPC_AUTH_TYPE_PROXY    //proxy need auth
}HTTPC_AUTH_TYPE_E;          //auth type

typedef struct _HTTPC_AUTH_PARAM
{
    char*   user_name_ptr;  //the user name that app set for auth
    char*   password_ptr;   //the password that app set for auth
    uint8*   entity_ptr;    //the entity param
    uint32  entity_len;     //the entity length
}HTTPC_AUTH_PARAM_T;

typedef struct _HTTPC_URI
{
    char*   uri_ptr;    //the resource uri that app set
}HTTPC_URI_T;            //start with "HTTPC://" or "https://"

typedef struct _HTTPC_ACCEPT_HEADER
{
    char*   accept_ptr; //the accept header value after Accept: 
}HTTPC_ACCEPT_HEADER_T;  //example: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, */*

typedef struct _HTTPC_ACCEPT_CHARSET_HEADER
{
    char*   accept_charset_ptr; //the accept charset header value after Accept-Charset:
}HTTPC_ACCEPT_CHARSET_HEADER_T;  //example: iso-8859-5 

typedef struct _HTTPC_ACCEPT_LANGUAGE_HEADER
{
    char*   accept_language_ptr;    //the accept language header value Accept-Language:
}HTTPC_ACCEPT_LANGUAGE_HEADER_T;     //example: zh-cn

typedef struct _HTTPC_CONTENT_TYPE_HEADER
{
    char*   content_type_ptr;       //the content-type header value content-type:
}HTTPC_CONTENT_TYPE_HEADER_T;     //example: application/json

typedef struct _HTTPC_IF_MATCH_HEADER
{
    char*   if_match_ptr;      //the IF-MATCH header value if_match:
}HTTPC_IF_MATCH_HEADER_T;     //example: "123456"

typedef struct _HTTPC_UA_PROFILE_HEADER
{
    char*   ua_profile_ptr; //the ua profile header value after x-wap-profile:
}HTTPC_UA_PROFILE_HEADER_T;  //example: HTTP://nds1.nds.nokia.com/uaprof/N6230r200.xml

typedef struct _HTTPC_USER_AGENT_HEADER
{
    char*   user_agent_ptr; //the user agent header value after User-Agent:
}HTTPC_USER_AGENT_HEADER_T;  //example: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)

typedef struct _HTTPC_REFERER_HEADER_T
{
    char*   referer_ptr;    //the referer header value after referer:  
}HTTPC_REFERER_HEADER_T;     //example: HTTP;//www.google.com

typedef struct _HTTPC_OTHER_HEADER
{
    char*   header_name_ptr;    //other header name, example: If-None-Match
    char*   header_value_ptr;   //other header value, example: "9ad948a3:1905"
}HTTPC_OTHER_HEADER_T;

typedef struct _HTTPC_EXTERN_HEADER
{
    HTTPC_OTHER_HEADER_T*    other_header_ptr;   //all other extern header
    uint32                  header_num;         //the other header number, if the header_num is 2, we should alloc other_header_ptr = sizeof(HTTP_OTHER_HEADER_T)*2  
}HTTPC_EXTERN_HEADER_T;                          //example: If-None-Match: "9ad948a3:1905"

typedef struct _HTTPC_GET_DATA
{
    HTTPC_DATA_TRANS_STYLE_E style; //the data trans style, if this set file, the content_file_name must set
    BOOLEAN 		    is_header_use_file;   //if this flag is set TRUE, the header will save as file, the header_file_name should set; if set FALSE, will use buffer
    HTTPC_UNICODE_T	    content_file_name[HTTPC_MAX_FILE_NAME_LEN + 1];
    HTTPC_UNICODE_T	    header_file_name[HTTPC_MAX_FILE_NAME_LEN + 1];
}HTTPC_GET_DATA_T;                                     

typedef struct _HTTPC_POST_FILE
{
    HTTPC_UNICODE_T  file_name[HTTPC_MAX_FILE_NAME_LEN + 1];  //the file name that the data will post
    uint32          file_size;                              //the file size    
}HTTPC_POST_FILE_T;

typedef struct _HTTPC_PATCH_FILE
{
    HTTPC_UNICODE_T  file_name[HTTPC_MAX_FILE_NAME_LEN + 1];  //the file name that the data will patch
    uint32          file_size;                              //the file size    
}HTTPC_PATCH_FILE_T;

typedef struct _HTTPC_DELETE_FILE
{
    HTTPC_UNICODE_T  file_name[HTTPC_MAX_FILE_NAME_LEN + 1];  //the file name that the data will delete
    uint32          file_size;                              //the file size    
}HTTPC_DELETE_FILE_T;


typedef struct _HTTPC_POST_BUFFER
{
    uint8*  buffer_ptr;         //the post data pointer
    uint32  buffer_len;         //the post data length
    BOOLEAN is_copied_by_httpc;  //if this flag is set TRUE, HTTP will copy the data of buffer_ptr, 
}HTTPC_POST_BUFFER_T;            //and app can free buffer_ptr after using the relative interface; 
                                //if this flag is set FALSE, HTTP will not copy the data of buffer_ptr,
                                //and app can not free buffer_ptr until APP receives HTTP_SIG_POST_CNF

typedef struct _HTTPC_PATCH_BUFFER
{
    uint8*  buffer_ptr;         //the PATCH data pointer
    uint32  buffer_len;         //the PATCH data length
    BOOLEAN is_copied_by_httpc;  //if this flag is set TRUE, HTTP will copy the data of buffer_ptr, 
}HTTPC_PATCH_BUFFER_T;        //and app can free buffer_ptr after using the relative interface; 
                                //if this flag is set FALSE, HTTP will not copy the data of buffer_ptr,
                                //and app can not free buffer_ptr until APP receives HTTP_SIG_PATCH_CNF

typedef struct _HTTPC_DELETE_BUFFER
{
    uint8*  buffer_ptr;         //the delete data pointer
    uint32  buffer_len;         //the delte data length
    BOOLEAN is_copied_by_httpc;  //if this flag is set TRUE, HTTP will copy the data of buffer_ptr, 
}HTTPC_DELETE_BUFFER_T;        //and app can free buffer_ptr after using the relative interface; 
                                //if this flag is set FALSE, HTTP will not copy the data of buffer_ptr,
                                //and app can not free buffer_ptr until APP receives HTTP_SIG_DELETE_CNF
								

typedef struct _HTTPC_POST_BODY
{
    BOOLEAN             is_use_file;        //if this flag is set TRUE, the post data use file style
    char*               body_type_ptr;      //post body type, example: text/html
    union
    {
        HTTPC_POST_FILE_T    post_file;      //the post data use file style
        HTTPC_POST_BUFFER_T  post_buffer;    //the post data use buffer style
    }u;
}HTTPC_POST_BODY_T;

typedef struct _HTTPC_PATCH_BODY
{
    BOOLEAN             is_use_file;        //if this flag is set TRUE, the PATCH data use file style
    char*               body_type_ptr;      //PATCH body type, example: application/json
    union
    {
        HTTPC_PATCH_FILE_T    patch_file;      //the PATCH data use file style
        HTTPC_PATCH_BUFFER_T  patch_buffer;    //the PATCH data use buffer style
    }u;
}HTTPC_PATCH_BODY_T;

typedef struct _HTTPC_DELETE_BODY
{
    BOOLEAN             is_use_file;        //if this flag is set TRUE, the delete data use file style
    char*               body_type_ptr;      //delete body type, example: text/html
    union
    {
        HTTPC_DELETE_FILE_T    delete_file;      //the delete data use file style
        HTTPC_DELETE_BUFFER_T  delete_buffer;    //the delete data use buffer style
    }u;
}HTTPC_DELETE_BODY_T;
								

typedef struct _HTTPC_HEAD_TRANS_STYLE
{
    BOOLEAN         is_use_file;                                  //if this flag is set TRUE, the header will save as file, the header_file_name should set; if set FALSE, will use buffer
    HTTPC_UNICODE_T	header_file_name[HTTPC_MAX_FILE_NAME_LEN + 1];
}HTTPC_HEAD_TRANS_STYLE_T;

typedef struct _HTTPC_PROXY
{
    BOOLEAN is_use_proxy;   //if use proxy
    char*   user_name_ptr;  //the proxy user name
    char*   password_ptr;   //the proxy password
    char*   proxy_host_ptr; //the proxy ip address, example: 10.0.0.172
    uint16  HTTPC_port;      //the HTTPC proxy port, example: 80
}HTTPC_PROXY_T;

typedef struct _HTTPC_INIT_PARAM
{
    HTTPC_PROXY_T    proxy;           //the proxy param
	uint32			net_id;			//the net id for socket binding
	float           time_zone;      //the time zone
    BOOLEAN         is_cache_enable; //if enable cache, if this flag is set TRUE, the cache_path must be set
    BOOLEAN         is_cookie_enable;//if enable cookie, if this flag is set TRUE, the cookie_file_name must be set
    HTTPC_UNICODE_T  cache_path[HTTPC_MAX_FILE_NAME_LEN+1];
    HTTPC_UNICODE_T  cookie_file_name[HTTPC_MAX_FILE_NAME_LEN+1];
}HTTPC_INIT_PARAM_T;

typedef struct _HTTPC_GET_PARAM
{
    HTTPC_URI_T                      uri;                    //the resource uri
    HTTPC_ACCEPT_HEADER_T            accept;                 //accept header value
    HTTPC_ACCEPT_CHARSET_HEADER_T    accept_charset;         //accept charset header value
    HTTPC_ACCEPT_LANGUAGE_HEADER_T   accept_language;        //accept language header value
    HTTPC_CONNECTION_E               connection;             //connection header value
    HTTPC_USER_AGENT_HEADER_T        user_agent;             //user agent header value
    HTTPC_UA_PROFILE_HEADER_T        ua_profile;             //ua profile header value
    HTTPC_REFERER_HEADER_T           referer;                //referer header value
    HTTPC_EXTERN_HEADER_T            extern_header;          //extern header
    HTTPC_AUTH_PARAM_T               auth;                   //auth param
    HTTPC_GET_DATA_T                 get_data;               //get data style: file or buffer
    HTTPC_ENCODING_TYPE_E            accept_encoding;        //accept encoding header value
    BOOLEAN                         need_net_prog_ind;      //if the app need net progress indication, set this flag TRUE
    BOOLEAN                         is_use_relative_url;    //if use relative url 
	uint32                          recv_timeout;           //ms
}HTTPC_GET_PARAM_T;
typedef HTTPC_GET_PARAM_T HTTPC_OPTIONS_PARAM_T;

typedef struct _HTTPC_PATCH_PARAM
{
    HTTPC_URI_T                      uri;                    //the resource uri
    HTTPC_ACCEPT_HEADER_T            accept;                 //accept header value
    HTTPC_ACCEPT_CHARSET_HEADER_T    accept_charset;         //accept charset header value
    HTTPC_ACCEPT_LANGUAGE_HEADER_T   accept_language;        //accept language header value
    HTTPC_IF_MATCH_HEADER_T          if_match;               //if match header value
    HTTPC_CONNECTION_E               connection;             //connection header value
    HTTPC_USER_AGENT_HEADER_T        user_agent;             //user agent header value
    HTTPC_UA_PROFILE_HEADER_T        ua_profile;             //ua profile header value
    HTTPC_REFERER_HEADER_T           referer;                //referer header value
    HTTPC_EXTERN_HEADER_T            extern_header;          //extern header
    HTTPC_AUTH_PARAM_T               auth;                   //auth param
    HTTPC_GET_DATA_T                 get_data;               //get data style: file or buffer
    HTTPC_PATCH_BODY_T				patch_body;			    //patch body style:  file or buffer
    HTTPC_ENCODING_TYPE_E            accept_encoding;        //accept encoding header value
    BOOLEAN                         need_net_prog_ind;      //if the app need net progress indication, set this flag TRUE
    BOOLEAN                         is_use_relative_url;    //if use relative url 
    BOOLEAN                         is_use_patch_after_redirect; //is still use patch method after redirect
	uint32                          recv_timeout;           //ms
}HTTPC_PATCH_PARAM_T;

typedef struct _HTTPC_DELETE_PARAM
{
	HTTPC_URI_T                      uri;                    //the resource uri
    HTTPC_ACCEPT_HEADER_T            accept;                 //accept header value
    HTTPC_ACCEPT_CHARSET_HEADER_T    accept_charset;         //accept charset header value
    HTTPC_ACCEPT_LANGUAGE_HEADER_T   accept_language;        //accept language header value
    HTTPC_CONNECTION_E               connection;             //connection header value
    HTTPC_USER_AGENT_HEADER_T        user_agent;             //user agent header value
    HTTPC_UA_PROFILE_HEADER_T        ua_profile;             //ua profile header value
    HTTPC_REFERER_HEADER_T           referer;                //referer header value
    HTTPC_EXTERN_HEADER_T            extern_header;          //extern header
    HTTPC_AUTH_PARAM_T               auth;                   //auth param
    HTTPC_GET_DATA_T                 get_data;               //get data style: file or buffer
    HTTPC_DELETE_BODY_T				delete_body;			//delete body style:  file or buffer
    HTTPC_ENCODING_TYPE_E            accept_encoding;        //accept encoding header value
    BOOLEAN                         need_net_prog_ind;      //if the app need net progress indication, set this flag TRUE
    BOOLEAN                         is_use_relative_url;    //if use relative url 
	uint32                          recv_timeout;           //ms
}HTTPC_DELETE_PARAM_T;


typedef struct _HTTPC_POST_PARAM
{
    HTTPC_URI_T                      uri;                        //the resource uri
    HTTPC_ACCEPT_HEADER_T            accept;                     //accept header value
    HTTPC_ACCEPT_CHARSET_HEADER_T    accept_charset;             //accept charset header value
    HTTPC_ACCEPT_LANGUAGE_HEADER_T   accept_language;            //accept language header value
    HTTPC_CONNECTION_E               connection;                 //connection header value
    HTTPC_USER_AGENT_HEADER_T        user_agent;                 //user agent header value
    HTTPC_UA_PROFILE_HEADER_T        ua_profile;                 //ua profile header value
    HTTPC_REFERER_HEADER_T           referer;                    //referer header value
    HTTPC_EXTERN_HEADER_T            extern_header;              //extern header
    HTTPC_AUTH_PARAM_T               auth;                       //auth param
    HTTPC_GET_DATA_T                 get_data;                   //get data style: file or buffer
    HTTPC_POST_BODY_T                post_body;                  //post body: file or buffer
    HTTPC_ENCODING_TYPE_E            accept_encoding;            //accept encoding header value
    BOOLEAN                         need_net_prog_ind;          //if the app need net progress indication, set this flag TRUE
    BOOLEAN                         is_use_relative_url;        //if use relative url
    BOOLEAN                         is_use_post_after_redirect; //is still use post method after redirect
	uint32                          recv_timeout;               //ms
}HTTPC_POST_PARAM_T;

typedef struct _HTTPC_HEAD_PARAM
{
    HTTPC_URI_T                      uri;                    //the resource uri
    HTTPC_ACCEPT_HEADER_T            accept;                 //accept header value
    HTTPC_ACCEPT_CHARSET_HEADER_T    accept_charset;         //accept charset header value
    HTTPC_ACCEPT_LANGUAGE_HEADER_T   accept_language;        //accept language header value
    HTTPC_CONNECTION_E               connection;             //connection header value
    HTTPC_USER_AGENT_HEADER_T        user_agent;             //user agent header value
    HTTPC_UA_PROFILE_HEADER_T        ua_profile;             //ua profile header value
    HTTPC_REFERER_HEADER_T           referer;                //referer header value
    HTTPC_EXTERN_HEADER_T            extern_header;          //extern header
    HTTPC_AUTH_PARAM_T               auth;                   //auth param
    HTTPC_HEAD_TRANS_STYLE_T         head_trans_style;       //head trans style: file or buffer
    HTTPC_ENCODING_TYPE_E            accept_encoding;        //accept encoding header value
    BOOLEAN                         need_net_prog_ind;      //if the app need net progress indication, set this flag TRUE
    BOOLEAN                         is_use_relative_url;    //if use relative url
	uint32                          recv_timeout;           //ms
}HTTPC_HEAD_PARAM_T;

typedef struct _HTTPC_RSP_HEADER_INFO
{
    uint32                  response_code;          //HTTPC response code, for example: 200, 302
    uint32                  refresh_time;           //refresh time: seconds
    int32                   content_length;         //HTTPC response content length
    char*                   content_type_ptr;       //HTTPC response content type       
    char*                   charset_ptr;            //HTTPC response charset
    char*                   refresh_url_ptr;        //HTTPC refresh url
	char*                   session_id_ptr;              //add for BITS-Session-Id
    HTTPC_ENCODING_TYPE_E    content_encoding_type;  //HTTPC response content encoding
}HTTPC_RSP_HEADER_INFO_T;

typedef struct _HTTPC_INIT_REQ_SIG
{
    _SIGNAL_VARS
    HTTPC_APP_MODULE_ID_T    module_id;  //module id of app
    HTTPC_APP_PARAM_T        app_param;  //app param
    HTTPC_INIT_PARAM_T       init_param; //init parameter that app set
}HTTPC_INIT_REQ_SIG_T;

typedef struct _HTTPC_GET_REQ_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;     //context id that HTTPC assigned for app
    HTTPC_GET_PARAM_T    get_param;      //get parameter that app set
    HTTPC_APP_INSTANCE_T app_instance;   //the app instance
}HTTPC_GET_REQ_SIG_T;

typedef struct _HTTPC_POST_REQ_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;     //context id that HTTPC assigned for app
    HTTPC_POST_PARAM_T   post_param;     //post parameter that app set
    HTTPC_APP_INSTANCE_T app_instance;   //the app instance
}HTTPC_POST_REQ_SIG_T;

typedef struct _HTTPC_PATCH_REQ_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;     //context id that HTTPC assigned for app
    HTTPC_PATCH_PARAM_T   patch_param;   //patch parameter that app set
    HTTPC_APP_INSTANCE_T app_instance;   //the app instance
}HTTPC_PATCH_REQ_SIG_T;

typedef struct _HTTPC_HEAD_REQ_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;     //context id that HTTPC assigned for app
    HTTPC_HEAD_PARAM_T   head_param;     //head parameter that HTTPC set
    HTTPC_APP_INSTANCE_T app_instance;   //the app instance
}HTTPC_HEAD_REQ_SIG_T;

typedef struct _HTTPC_OPTIONS_REQ_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;     //context id that HTTPC assigned for app
    HTTPC_OPTIONS_PARAM_T   options_param;     //options parameter that HTTPC set
    HTTPC_APP_INSTANCE_T app_instance;   //the app instance
}HTTPC_OPTIONS_REQ_SIG_T;

typedef struct _HTTPC_DELETE_REQ_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;     //context id that HTTPC assigned for app
    HTTPC_DELETE_PARAM_T   delete_param;     //options parameter that HTTPC set
    HTTPC_APP_INSTANCE_T app_instance;   //the app instance
}HTTPC_DELETE_REQ_SIG_T;

typedef struct _HTTPC_CANCEL_REQ_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;     //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T	request_id;     //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T app_instance;   //the app instance
}HTTPC_CANCEL_REQ_SIG_T;

typedef struct _HTTPC_CLOSE_REQ_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id; //context id that HTTPC assigned for app
}HTTPC_CLOSE_REQ_SIG_T;

typedef struct _HTTPC_COOKIE_CLEAR_REQ_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id; //context id that HTTPC assigned for app
}HTTPC_COOKIE_CLEAR_REQ_SIG_T;

typedef struct _HTTPC_CACHE_CLEAR_REQ_SIG
{
    _SIGNAL_VARS
	HTTPC_UNICODE_T file_name[HTTPC_MAX_FILE_NAME_LEN + 1];
}HTTPC_CACHE_CLEAR_REQ_SIG_T;

typedef struct _HTTPC_AUTH_IND_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;     //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T   request_id;     //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T app_instance;   //the app instance
    HTTPC_AUTH_TYPE_E    auth_type;      //the auth type: www or proxy
}HTTPC_AUTH_IND_SIG_T;

typedef struct _HTTPC_AUTH_RSP_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;     //context id that HTTPC assigned for app
    HTTPC_AUTH_PARAM_T   auth;           //auth parameter that app set
    HTTPC_REQUEST_ID_T   request_id;     //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T app_instance;   //the app instance
}HTTPC_AUTH_RSP_SIG_T;

typedef struct _HTTPC_INIT_CNF_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T       context_id; //context id that HTTPC assigned for app
    HTTPC_APP_MODULE_ID_T    module_id;  //module id of app
    HTTPC_APP_PARAM_T        app_param;  //app param
    HTTPC_ERROR_E            result;     //request result
}HTTPC_INIT_CNF_SIG_T;

typedef struct _HTTPC_GET_CNF_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T       context_id;         //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T       request_id;         //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T     app_instance;       //the app instance
    HTTPC_ERROR_E            result;             //request result
    HTTPC_RSP_HEADER_INFO_T  rsp_header_info;    //response header information
}HTTPC_GET_CNF_SIG_T;

typedef struct _HTTPC_POST_CNF_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T       context_id;         //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T       request_id;         //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T     app_instance;       //the app instance
    HTTPC_ERROR_E            result;             //request result
    HTTPC_RSP_HEADER_INFO_T  rsp_header_info;    //response header information
}HTTPC_POST_CNF_SIG_T;

typedef struct _HTTPC_PATCH_CNF_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T       context_id;         //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T       request_id;         //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T     app_instance;       //the app instance
    HTTPC_ERROR_E            result;             //request result
    HTTPC_RSP_HEADER_INFO_T  rsp_header_info;    //response header information
}HTTPC_PATCH_CNF_SIG_T;

typedef struct _HTTPC_HEAD_CNF_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T       context_id;         //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T       request_id;         //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T     app_instance;       //the app instance
    HTTPC_ERROR_E            result;             //request result
    HTTPC_RSP_HEADER_INFO_T  rsp_header_info;    //response header information
}HTTPC_HEAD_CNF_SIG_T;
typedef struct _HTTPC_OPTIONS_CNF_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T       context_id;         //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T       request_id;         //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T     app_instance;       //the app instance
    HTTPC_ERROR_E            result;             //request result
    HTTPC_RSP_HEADER_INFO_T  rsp_header_info;    //response header information
}HTTPC_OPTIONS_CNF_SIG_T;

typedef struct _HTTPC_DELETE_CNF_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T       context_id;         //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T       request_id;         //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T     app_instance;       //the app instance
    HTTPC_ERROR_E            result;             //request result
    HTTPC_RSP_HEADER_INFO_T  rsp_header_info;    //response header information
}HTTPC_DELETE_CNF_SIG_T;

typedef struct _HTTPC_HEADER_IND_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T       context_id;         //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T       request_id;         //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T     app_instance;       //the app instance
    HTTPC_RSP_HEADER_INFO_T  rsp_header_info;    //response header information
    char*                   header_ptr;         //response header pointer
}HTTPC_HEADER_IND_SIG_T;

typedef struct _HTTPC_HEADER_RSP_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T       context_id;         //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T       request_id;         //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T     app_instance;       //the app instance
    HTTPC_DATA_TRANS_STYLE_E style;		//the data trans style, if this set file, the content_file_name must set
    HTTPC_UNICODE_T          content_file_name[HTTPC_MAX_FILE_NAME_LEN + 1];
}HTTPC_HEADER_RSP_SIG_T;

typedef struct _HTTPC_DATA_IND_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;     //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T   request_id;     //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T app_instance;   //the app instance
    uint8*              data_ptr;       //response data pointer
    uint32              data_len;       //response data len
    uint32              response_code;  //response code
}HTTPC_DATA_IND_SIG_T;

typedef struct _HTTPC_ERROR_IND_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;     //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T   request_id;     //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T app_instance;   //the app instance
    HTTPC_ERROR_E        result;         //HTTPC error
}HTTPC_ERROR_IND_SIG_T;


typedef struct _HTTPC_CANCEL_CNF_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;     //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T   request_id;     //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T app_instance;   //the app instance
    HTTPC_ERROR_E        result;         //request result
}HTTPC_CANCEL_CNF_SIG_T;

typedef struct _HTTPC_CLOSE_CNF_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id; //context id that HTTPC assigned for app
    HTTPC_ERROR_E        result;     //request result
}HTTPC_CLOSE_CNF_SIG_T;

typedef struct _HTTPC_CLEAR_CACHE_CNF_SIG
{
    _SIGNAL_VARS
    HTTPC_ERROR_E        result;     //request result
}HTTPC_CLEAR_CACHE_CNF_SIG_T;

typedef struct _HTTPC_COOKIE_CLEAR_CNF_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id; //context id that HTTPC assigned for app
    HTTPC_ERROR_E        result;     //request result
}HTTPC_COOKIE_CLEAR_CNF_SIG_T;

typedef struct _HTTPC_REDIRECT_IND_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;         //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T   request_id;         //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T app_instance;       //the app instance
    char*               redirect_ptr;       //redirect uri
}HTTPC_REDIRECT_IND_SIG_T;

typedef struct _HTTPC_CACHE_DELETE_REQ_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;         //context id that HTTPC assigned for app
    char*               url_ptr;            //redirect uri
}HTTPC_CACHE_DELETE_REQ_SIG_T;

typedef struct _HTTPC_CACHE_DELETE_CNF_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;         //context id that HTTPC assigned for app
    HTTPC_ERROR_E        result;             //request result
}HTTPC_CACHE_DELETE_CNF_SIG_T;

typedef struct _HTTPC_REQUEST_ID_IND_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;         //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T   request_id;         //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T app_instance;       //the app instance
}HTTPC_REQUEST_ID_IND_SIG_T;

typedef struct _HTTPC_READ_CACHE_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;         //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T   request_id;         //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T app_instance;       //the app instance
}HTTPC_READ_CACHE_SIG_T;

typedef struct _HTTPC_COOKIE_SET_PARAM
{
    HTTPC_CONTEXT_ID_T   context_id;         //context id that HTTPC assigned for app
    HTTPC_APP_INSTANCE_T app_instance;       //the app instance
    char*               url_ptr;            //the request url
    char*               cookie_value_ptr;   //the cookie value
}HTTPC_COOKIE_SET_PARAM_T;

typedef struct _HTTPC_COOKIE_SET_REQ_SIG
{
    _SIGNAL_VARS
    HTTPC_COOKIE_SET_PARAM_T cookie_set_param; //the cookie set param
}HTTPC_COOKIE_SET_REQ_SIG_T;

typedef struct _HTTPC_COOKIE_SET_CNF_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;     //context id that HTTPC assigned for app
    HTTPC_APP_INSTANCE_T app_instance;   //the app instance
    HTTPC_ERROR_E        result;         //request result
}HTTPC_COOKIE_SET_CNF_SIG_T;

#ifdef SSL_SUPPORT
typedef struct _HTTPC_SSL_CERT_UNTRUST_IND_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;         //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T   request_id;         //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T app_instance;       //the app instance
}HTTPC_SSL_CERT_UNTRUST_IND_SIG_T;

typedef struct _HTTPC_SSL_CERT_UNTRUST_RSP_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;         //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T   request_id;         //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T app_instance;       //the app instance
    BOOLEAN             is_trust_cert;      //if trust cert
}HTTPC_SSL_CERT_UNTRUST_RSP_SIG_T;

typedef struct _HTTPC_SSL_GET_CERT_REQ_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;         //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T   request_id;         //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T app_instance;       //the app instance
}HTTPC_SSL_GET_CERT_REQ_SIG_T;

typedef struct _HTTPC_SSL_GET_CERT_CNF_SIG
{
    _SIGNAL_VARS
    HTTPC_CONTEXT_ID_T   context_id;         //context id that HTTPC assigned for app
    HTTPC_REQUEST_ID_T   request_id;         //the request id that HTTPC assigned for app request
    HTTPC_APP_INSTANCE_T app_instance;       //the app instance
    SSL_CERTINFO_T      ssl_cert_info;	    //ssl cert info
}HTTPC_SSL_GET_CERT_CNF_SIG_T;
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init HTTPC session request
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
HTTPC_ERROR_E HTTPC_InitRequest(
			                        HTTPC_APP_MODULE_ID_T module_id, 
			                        HTTPC_APP_PARAM_T app_param, 
			                        const HTTPC_INIT_PARAM_T* init_param_ptr
			                        );

/*****************************************************************************/
//  Description : get HTTPC data request
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTPC_ERROR_E HTTPC_GetRequest(
                                    HTTPC_CONTEXT_ID_T       context_id,
                                    const HTTPC_GET_PARAM_T* get_param_ptr,
                                    HTTPC_APP_INSTANCE_T     app_instance
                                    );

/*****************************************************************************/
//  Description : patch HTTPC data request
//  Global resource dependence : 
//  Author: mingzhe.jin
//  Note: 
/*****************************************************************************/
PUBLIC HTTPC_ERROR_E HTTPC_PatchRequest(
                                    HTTPC_CONTEXT_ID_T          context_id,
                                    const HTTPC_PATCH_PARAM_T*  patch_param_ptr,
                                    HTTPC_APP_INSTANCE_T        app_instance
                                    );

/*****************************************************************************/
//  Description : post HTTPC data request
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTPC_ERROR_E HTTPC_PostRequest(
                                    HTTPC_CONTEXT_ID_T          context_id,
                                    const HTTPC_POST_PARAM_T*   post_param_ptr,
                                    HTTPC_APP_INSTANCE_T        app_instance
                                    );

/*****************************************************************************/
//  Description : HTTPC head request
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTPC_ERROR_E HTTPC_HeadRequest(
                                    HTTPC_CONTEXT_ID_T          context_id,
                                    const HTTPC_HEAD_PARAM_T*   head_param_ptr,
                                    HTTPC_APP_INSTANCE_T        app_instance
                                    );

/*****************************************************************************/
//  Description : HTTPC options request
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTPC_ERROR_E HTTPC_OptionsRequest(
                                    HTTPC_CONTEXT_ID_T          context_id,
                                    const HTTPC_OPTIONS_PARAM_T*   options_param_ptr,
                                    HTTPC_APP_INSTANCE_T        app_instance
                                    );
//  Description : create and send auth signal
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/

/*****************************************************************************/
//  Description : HTTPC delete request
//  Global resource dependence : 
//  Author: zhaodi.chen
//  Note: 
/*****************************************************************************/
PUBLIC HTTPC_ERROR_E HTTPC_DeleteRequest(
                                    HTTPC_CONTEXT_ID_T          context_id,
                                    const HTTPC_DELETE_PARAM_T*   delete_param_ptr,
                                    HTTPC_APP_INSTANCE_T        app_instance
                                    );
//  Description : create and send auth signal
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/


PUBLIC HTTPC_ERROR_E HTTPC_AuthResponse(
                                    HTTPC_CONTEXT_ID_T         context_id,
                                    const HTTPC_AUTH_PARAM_T*  auth_param_ptr,
                                    HTTPC_APP_INSTANCE_T       app_instance,
                                    HTTPC_REQUEST_ID_T         request_id
                                    );

/*****************************************************************************/
//  Description : close HTTPC request
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTPC_ERROR_E HTTPC_CloseRequest(HTTPC_CONTEXT_ID_T context_id);

/*****************************************************************************/
//  Description : cancel HTTPC request
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTPC_ERROR_E HTTPC_CancelRequest(
                                        HTTPC_CONTEXT_ID_T   context_id,
                                        HTTPC_APP_INSTANCE_T app_instance,
                                        HTTPC_REQUEST_ID_T   request_id
                                      );

/*****************************************************************************/
//  Description : create and send header rsp signal
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTPC_ERROR_E HTTPC_HeaderResponse(
                              HTTPC_CONTEXT_ID_T         context_id,
                              HTTPC_APP_INSTANCE_T       app_instance,
                              HTTPC_REQUEST_ID_T         request_id,
                              HTTPC_DATA_TRANS_STYLE_E   style,
                              HTTPC_UNICODE_T*           file_name_ptr,
                              uint32                    file_name_len
                              );

/*****************************************************************************/
//  Description : clear HTTPC cookie
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTPC_ERROR_E HTTPC_CookieClear(HTTPC_CONTEXT_ID_T context_id,HTTPC_UNICODE_T* cookie_file_name_ptr);

/*****************************************************************************/
//  Description : clear HTTPC cache
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTPC_ERROR_E HTTPC_CacheClear (HTTPC_UNICODE_T* cache_path_ptr);

/*****************************************************************************/
//  Description : create and send cache delete to HTTPC 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HTTPC_CacheDelete(
                                   HTTPC_CONTEXT_ID_T context_id,    //[IN]
                                   char* url_ptr
                                   );

/*****************************************************************************/
//  Description : create and send cookie set signal
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTPC_ERROR_E HTTPC_CookieSetReq(
                                        HTTPC_COOKIE_SET_PARAM_T* cookie_ptr
                                        );

/*****************************************************************************/
//  Description : delete cookie file
//  Global resource dependence :
//  Author: zhaodi.chen
//  Note:
/*****************************************************************************/
PUBLIC HTTPC_ERROR_E HTTPCDeleteCookieFile();


#ifdef SSL_SUPPORT
/*****************************************************************************/
//  Description : create and send ssl cert signal
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTPC_ERROR_E HTTPC_SslCertResponse(
                                        HTTPC_CONTEXT_ID_T         context_id,
                                        HTTPC_APP_INSTANCE_T       app_instance,
                                        HTTPC_REQUEST_ID_T         request_id,
                                        BOOLEAN                   is_trust_cert
                                        );

/*****************************************************************************/
//  Description : create and send ssl get cert signal
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTPC_ERROR_E HTTPC_SslGetCertRequest(
                                        HTTPC_CONTEXT_ID_T         context_id,
                                        HTTPC_APP_INSTANCE_T       app_instance,
                                        HTTPC_REQUEST_ID_T         request_id
                                        );
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
