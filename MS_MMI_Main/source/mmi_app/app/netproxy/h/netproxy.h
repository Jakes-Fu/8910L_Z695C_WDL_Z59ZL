#ifndef _NET_PROXY_H_    
#define _NET_PROXY_H_   

#include "sci_types.h"
#include "os_api.h"



#ifdef __cplusplus
extern "C" {
#endif
typedef void (*DATA_RECEIVED)(int resulet, const char *data, uint32 data_len, void *user_data);
typedef int (*NETPOST)(uint32 net_id, const char *request, const char *data, void *user_data, DATA_RECEIVED data_recv);
typedef int (*NETGET)(uint32 net_id, const char *request, const char *data, void *user_data, DATA_RECEIVED data_revc);
typedef void (*OPENBROWSER)(const char *url, uint32 netId);
typedef void (*EXITBROWSER)(uint16 exit_type);

typedef enum
{
    NET_SUCCESS = 0,
    NET_ERROR_BUSY,
    NET_ERROR_FAILD,
    NET_ERROR_BAD_PARAM,
    NET_ERROR_CANCELED,
    NET_ERROR_NO_MEM,

    NET_ERROR_MAX,
}_NET_ERROR_E;

typedef enum
{
    NET_METHOD_POST = 0,
    NET_METHOD_GET,

    NET_METHOD_MAX,
}NET_METHOD_E;

typedef struct _NET_MANAGER_
{
    NETPOST     http_post;
    NETGET      http_get;
    OPENBROWSER open_browser;
    EXITBROWSER exit_browser;
}NET_MANAGER_T;

typedef struct _NET_REQUEST_PARAM_
{
    uint32 net_id;
    int method;
    char *uri;
    char *body;
    int body_len;
    int uri_len;
    void *user_data;
    DATA_RECEIVED callback;
}NET_REQUEST_PARAM_T;

typedef struct _NetproxySignal
{
    SIGNAL_VARS
    void *sig;				/*!< signal data */
}NetproxySignal;

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC NET_MANAGER_T * MMINETPROXY_NetManager(void);

#ifdef   __cplusplus
    }
#endif
#endif

