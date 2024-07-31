/****************************************************************************
** File Name:      netproxy.c                                            *
** Author:                                                                    *
** Date:           03/26/2015                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:                                                               *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 03/2015      Jiaoyou.wu        Create
** 
****************************************************************************/



/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "netproxy.h"
#include "http_api.h"
#include "mmi_appmsg.h"

#include <string.h>
#include "sci_types.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmi_modu_main.h"
#include "mmi_default.h"
#include "mmk_msg.h"
#include "in_message.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#if defined(BROWSER_SUPPORT_WEBKIT) && (!defined(WIN32) || (defined(_MSC_VER) && (_MSC_VER >= 1400)))
#include "mmiwk.h"
#endif
//#include "../../browser_manager/mmibrowser_api.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define JASON_DATA_LENGTH_MAX 5*1024
#define NETPROXY_COOKIE_FILE_NAME    L"E:\\http\\cookie"
#define NETPROXY_CACHE_PATH_NAME     L"E:\\http\\cache"
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct MMINETPROXY_HTTP_CONTEXT_T_
{
    HTTP_APP_MODULE_ID_T module_id;
    HTTP_CONTEXT_ID_T    context_id;
    HTTP_REQUEST_ID_T    request_id;
    HTTP_APP_INSTANCE_T  app_instance; //鐢ㄤ簬鍖哄垎涓嶅悓鐨凥TTP璇锋眰銆傚悓鏃跺彧鏈変竴涓狧TTP璇锋眰鏃跺彲浠ヤ负浠绘剰鍊笺�?    void * user_data;
    uint8 * data_ptr;
    uint32 data_len;
    uint32 recv_len;
    int result;
}MMINETPROXY_HTTP_CONTEXT_T;

MMI_APPLICATION_T           g_net_proxy_app       = {0};
NET_MANAGER_T               s_net_mgr = {0};
MMINETPROXY_HTTP_CONTEXT_T  s_net_context      = {0};
NET_REQUEST_PARAM_T         s_request = {0};

/**-------------------------------------------------------------------------*
**            LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :   NetProxy handler
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

/*****************************************************************************/
//  Description :   NetProxy handler
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

/*****************************************************************************/
//  Description : HTTP request
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
int http_request(NET_METHOD_E method, int net_id, const char *uri, const char *body, void *user_data, DATA_RECEIVED data_recv);

/*****************************************************************************/
//  Description : POST request
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
int http_post(uint32 net_id, const char *uri, const char *body, void *user_data, DATA_RECEIVED data_recv);

/*****************************************************************************/
//  Description : GET request
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
int http_get(uint32 net_id, const char *uri, const char *body, void *user_data, DATA_RECEIVED data_recv);

/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size);

/*****************************************************************************/
//  Description : Open Browser
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
void open_browser(const char * url, uint32 netId);

/*****************************************************************************/
//  Description : Exit Browser
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
void exit_browser(uint16 exit_type );

/*****************************************************************************/
//  Description : Handle Http Init Cnf
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpInitCnf(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Request Id Ind
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpRequestIdInd(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Get Cnf
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpGetCnf(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Head Cnf
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpHeadCnf(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Data Ind
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpDataInd(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Header Ind
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpHeaderInd(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Error Ind
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpErrorInd(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Close Cnf
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpCloseCnf(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Cancel Cnf
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpCancelCnf(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Redirect Ind
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpPostCnf(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param);

/*****************************************************************************/
//  Description : Handle Portal Get Or Post Cnf
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void HandleNetProxyReceivedData(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr);

/*****************************************************************************/
//  Description : Initialize http request
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void StartHttpRequest(void);

/*****************************************************************************/
//  Description : Initialize http request
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void CancelHttpRequest(void);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMINETPROXY_InitApp(void)
{
    g_net_proxy_app.ProcessMsg = HandleNetProxyMsg;
    s_net_mgr.http_post = http_post;
    s_net_mgr.http_get = http_get;
    s_net_mgr.open_browser = open_browser;
    s_net_mgr.exit_browser = exit_browser;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMINETPROXY_ResetApp(void)
{
    SCI_MEMSET(&s_net_mgr, 0, sizeof(NET_MANAGER_T));
    return;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC NET_MANAGER_T * MMINETPROXY_NetManager(void)
{
    return &s_net_mgr;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    if ((HTTP_SIGNAL_BEG <= msg_id && msg_id <= HTTP_SIGNAL_END) || (OAUTH_SIG_BEGIN <= msg_id && msg_id <= OAUTH_SIG_END))
    {
        result = HandleNetProxyHttpMsg(app_ptr, msg_id, param);
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL MMINETPROXY_HTTP_CONTEXT_T* GetNetProxyContextPtr(void)
{
    return &s_net_context;
}

/*****************************************************************************/
//  Description : Initialize http request
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void StartHttpRequest(void)
{

    HTTP_INIT_PARAM_T* init_param_ptr = PNULL;

    init_param_ptr = SCI_ALLOC_APPZ(sizeof(HTTP_INIT_PARAM_T));
    init_param_ptr->net_id = s_request.net_id;
    init_param_ptr->is_cookie_enable = TRUE;
    init_param_ptr->is_cache_enable = TRUE;
	memcpy(init_param_ptr->cache_path, NETPROXY_CACHE_PATH_NAME, 28);
    memcpy(init_param_ptr->cookie_file_name, NETPROXY_COOKIE_FILE_NAME, 28);

	//init_param_ptr->proxy.is_use_proxy = TRUE;
    //init_param_ptr->proxy.http_port = 8888;
    //init_param_ptr->proxy.proxy_host_ptr ="10.0.33.135";
    HTTP_InitRequest(MMI_MODULE_NET_PROXY, 1, init_param_ptr);
    SCI_FREE(init_param_ptr);
    init_param_ptr = PNULL;
}

/*****************************************************************************/
//  Description : Cancel http request
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void CancelHttpRequest(void)
{
    MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr = GetNetProxyContextPtr();

    if(PNULL != http_context_ptr)
    {

        if (NULL == s_request.uri)
        {
            SCI_TRACE_LOW("[NetProxy]:CancelHttpRequest No Request");
        }
    
        HTTP_CancelRequest(http_context_ptr->context_id,http_context_ptr->app_instance,http_context_ptr->request_id); 
        //http_context_ptr->result = 
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
	
    MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr = GetNetProxyContextPtr();

	app_ptr = app_ptr;

    switch (msg_id)
    {
    case HTTP_SIG_INIT_CNF:
        SCI_TRACE_LOW("[NetProxy]:HandleNetProxyHttpMsg receive HTTP_SIG_INIT_CNF");
        result = HandleNetProxyHttpInitCnf(http_context_ptr, param);
        break;

    case HTTP_SIG_REQUEST_ID_IND:
        SCI_TRACE_LOW("[NetProxy]:HandleNetProxyHttpMsg receive HTTP_SIG_REQUEST_ID_IND");
        result = HandleNetProxyHttpRequestIdInd(http_context_ptr, param);
        break;

    case HTTP_SIG_GET_CNF:
        SCI_TRACE_LOW("[NetProxy]:HandleNetProxyHttpMsg receive HTTP_SIG_GET_CNF");
        result = HandleNetProxyHttpGetCnf(http_context_ptr, param);
        break;

    case HTTP_SIG_HEAD_CNF:
        SCI_TRACE_LOW("[NetProxy]:HandleNetProxyHttpMsg receive HTTP_SIG_HEAD_CNF");
        result = HandleNetProxyHttpHeadCnf(http_context_ptr, param);
        break;

    case HTTP_SIG_DATA_IND:
        SCI_TRACE_LOW("[NetProxy]:HandleNetProxyHttpMsg receive HTTP_SIG_GET_CNF");
        result = HandleNetProxyHttpDataInd(http_context_ptr, param);
        break;

    case OAUTH_SIG_TOKEN_IND:
        result = HandleNetProxyHttpDataInd(http_context_ptr, param);
        break;

    case HTTP_SIG_POST_CNF:
        SCI_TRACE_LOW("[NetProxy]:HandleNetProxyHttpMsg receive HTTP_SIG_POST_CNF");
        result = HandleNetProxyHttpPostCnf(http_context_ptr, param);
        break;

    case HTTP_SIG_HEADER_IND:
        SCI_TRACE_LOW("[NetProxy]:HandleNetProxyHttpMsg receive HTTP_SIG_HEADER_IND");
        result = HandleNetProxyHttpHeaderInd(http_context_ptr, param);
        break;

    case HTTP_SIG_ERROR_IND:
        SCI_TRACE_LOW("[NetProxy]:HandleNetProxyHttpMsg receive HTTP_SIG_ERROR_IND");
        result = HandleNetProxyHttpErrorInd(http_context_ptr, param);
        break;

    case HTTP_SIG_CLOSE_CNF:
        SCI_TRACE_LOW("[NetProxy]:HandleNetProxyHttpMsg receive HTTP_SIG_CLOSE_CNF");
        result = HandleNetProxyHttpCloseCnf(http_context_ptr, param);
        break;

    case HTTP_SIG_CANCEL_CNF:
        SCI_TRACE_LOW("[NetProxy]:HandleNetProxyHttpMsg receive HTTP_SIG_CANCEL_CNF");
        result = HandleNetProxyHttpCancelCnf(http_context_ptr, param);
        break;
#if defined(BROWSER_SUPPORT_WEBKIT) && (!defined(WIN32) || (defined(_MSC_VER) && (_MSC_VER >= 1400)))
    case OAUTH_SIG_WK_302:
	SCI_TRACE_LOW("[NetProxy]:HandleNetProxyHttpMsg receive OAUTH_SIG_WK_302");
        {
            char* url = ((NetproxySignal*)param)->sig;
            if(NULL == url)
            {
                break;
            }
            OAUTH_CheckAccessCode(url);
            if(NULL != url)
            {
                SCI_FREE(url);            
            }
        }
        break;
#endif
    default:
        SCI_TRACE_LOW("[NetProxy]:HandleNetProxyHttpMsg Not handle msg_id = %x", msg_id);
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
int http_request(NET_METHOD_E method, int net_id, const char *uri, const char *body, void *user_data, DATA_RECEIVED data_recv)
{
    if (NULL != s_request.uri)
    {
        return NET_ERROR_BUSY;
    }

    if (NULL == uri || NULL == data_recv)
    {
        SCI_TRACE_LOW("[NetProxy]:http_request  uri is NULL or data_rcv is null");
        return NET_ERROR_BAD_PARAM;
    }

    s_request.uri_len = strlen(uri);

    s_request.uri = SCI_ALLOCA(s_request.uri_len + 1);

    if (NULL == s_request.uri)
    {
        SCI_TRACE_LOW("[NetProxy]:http_request  s_request.uri is null");
        return NET_ERROR_NO_MEM;
    }

    strncpy(s_request.uri, uri, s_request.uri_len);
    s_request.uri[s_request.uri_len] = 0;

    if (NULL != body)
    {
        s_request.body_len = strlen(body);
        s_request.body = SCI_ALLOCA(s_request.body_len + 1);

        if (NULL == s_request.body)
        {
            SCI_TRACE_LOW("[NetProxy]:http_request  s_request.body is null");
            SCI_FREE(s_request.uri);
            s_request.uri = NULL;
            return NET_ERROR_NO_MEM;
        }

        strncpy(s_request.body, body, s_request.body_len);
        s_request.body[s_request.body_len] = 0;
    }


    s_request.net_id = net_id;
	s_request.method = method;
	s_request.user_data = user_data;
    s_request.callback = data_recv;

	StartHttpRequest();

	return TRUE;
}

/*****************************************************************************/
//  Description : POST request
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
int http_post(uint32 net_id, const char *uri, const char *body, void *user_data, DATA_RECEIVED data_recv)
{
    return http_request(NET_METHOD_POST, net_id, uri, body, user_data, data_recv);
}

/*****************************************************************************/
//  Description : GET request
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
int http_get(uint32 net_id, const char *uri, const char *body, void *user_data, DATA_RECEIVED data_recv)
{
    SCI_TRACE_LOW(" [NetProxy] http_get  uri is %s", uri);
    return http_request(NET_METHOD_GET, net_id, uri, body, user_data, data_recv);
}

/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size)
{
    if (PNULL == param || PNULL == signal_struct_ptr || struct_size < sizeof(xSignalHeaderRec))
    {
        return;
    }
    
    SCI_MEMCPY((uint8 *)((uint32)signal_struct_ptr + sizeof(xSignalHeaderRec)), param, (struct_size - sizeof(xSignalHeaderRec)));
}

/*****************************************************************************/
//  Description : Handle Http Init Cnf
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpInitCnf(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    HTTP_INIT_CNF_SIG_T* signal = NULL;
    HTTP_APP_PARAM_T app_param = 0;

    signal = (HTTP_INIT_CNF_SIG_T*)param;

    if (PNULL == http_context_ptr || MMI_MODULE_NET_PROXY != signal->module_id)
    {
        return MMI_RESULT_FALSE;
    }

    app_param = signal->app_param;   //可以传app需要的参数过来。暂未用到。

    http_context_ptr->context_id   = signal->context_id;
    http_context_ptr->app_instance = 0;
    http_context_ptr->module_id    = signal->module_id;

    if (NET_METHOD_POST == s_request.method)
    {
        HTTP_POST_PARAM_T  param = {0};
        
        param.connection = HTTP_CONNECTION_DEFAULT;
        param.need_net_prog_ind = FALSE;
        param.post_body.is_use_file = FALSE;
        param.uri.uri_ptr = s_request.uri;
        param.get_data.is_header_use_file = FALSE; 

        if (PNULL != s_request.body)
        {
            param.post_body.u.post_buffer.buffer_ptr = (uint8 *)s_request.body;
            param.post_body.u.post_buffer.buffer_len = s_request.body_len;
            param.post_body.u.post_buffer.is_copied_by_http = TRUE;
        }

        param.accept_encoding = HTTP_ENCODING_TYPE_NULL;
        param.get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;   

        HTTP_PostRequest(http_context_ptr->context_id, &param, http_context_ptr->app_instance); 
    }
    else if (NET_METHOD_GET == s_request.method)
    {
        uint8 * url_ptr  = PNULL;
        uint32 url_len = 0;
        HTTP_GET_PARAM_T  param = {0};

        param.connection = HTTP_CONNECTION_DEFAULT;

        if (PNULL != s_request.body)
        {
            url_len = s_request.uri_len + s_request.body_len + 1;// "?"
            url_ptr = SCI_ALLOCA(url_len + 1);

            if (PNULL != url_ptr)
            {
                SCI_MEMSET(url_ptr, 0x00, (url_len + 1));
                SCI_MEMCPY(url_ptr, s_request.uri, s_request.uri_len);
                url_ptr[s_request.uri_len] = '?';
                SCI_MEMCPY(url_ptr + s_request.uri_len + 1, s_request.body, s_request.body_len);
                param.uri.uri_ptr = (char *)url_ptr;
            }
            else
            {
                SCI_TRACE_LOW(" HandleNetProxyHttpInitCnf no memory");
                return MMI_RESULT_FALSE;
            }
        }
        else
        {
            param.uri.uri_ptr = (char *)s_request.uri;
        }

        param.accept_encoding = HTTP_ENCODING_TYPE_NULL;
        param.get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
        param.get_data.is_header_use_file = FALSE;
        param.need_net_prog_ind = FALSE;

        HTTP_GetRequest(http_context_ptr->context_id, &param, http_context_ptr->app_instance); 
    }

    return result;

}

/*****************************************************************************/
//  Description : Handle Http Request Id Ind
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpRequestIdInd(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    //HTTP_REQUEST_ID_IND_SIG_T signal = {0};
	HTTP_REQUEST_ID_IND_SIG_T* signal = NULL;
    signal = (HTTP_REQUEST_ID_IND_SIG_T*)param;
    //GetSignalStruct(param, &signal, sizeof(HTTP_REQUEST_ID_IND_SIG_T));

    if (PNULL == http_context_ptr || http_context_ptr->context_id != signal->context_id)
    {
        return MMI_RESULT_FALSE;
    }

    http_context_ptr->request_id = signal->request_id;
    http_context_ptr->app_instance = signal->app_instance;

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Get Cnf
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpGetCnf(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
	HTTP_GET_CNF_SIG_T* signal = NULL;
   // HTTP_GET_CNF_SIG_T signal = {0};
    signal = (HTTP_GET_CNF_SIG_T*)param;
    //GetSignalStruct(param, &signal, sizeof(HTTP_GET_CNF_SIG_T));

    if (PNULL == http_context_ptr || http_context_ptr->context_id != signal->context_id)
    {
        return MMI_RESULT_FALSE;
    }

    if(signal->result != HTTP_SUCCESS)
    {
        http_context_ptr->result = NET_ERROR_FAILD;
    }
    else
    {
        http_context_ptr->result = NET_SUCCESS;
    }

    HandleNetProxyReceivedData(http_context_ptr);

    HTTP_CloseRequest(http_context_ptr->context_id);

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Head Cnf
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpHeadCnf(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_HEAD_CNF_SIG_T* signal = NULL;
    signal = (HTTP_HEAD_CNF_SIG_T*)param;
    //GetSignalStruct(param, &signal, sizeof(HTTP_HEAD_CNF_SIG_T));

    if (PNULL == http_context_ptr || http_context_ptr->context_id != signal->context_id)
    {
        return MMI_RESULT_FALSE;
    }

    HTTP_CloseRequest(http_context_ptr->context_id);

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Data Ind
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpDataInd(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_DATA_IND_SIG_T* signal = NULL;
    uint32 response_code;  //response code
    signal = (HTTP_DATA_IND_SIG_T*)param;
    //GetSignalStruct(param, &signal, sizeof(HTTP_DATA_IND_SIG_T));

    if (PNULL == http_context_ptr || http_context_ptr->context_id != signal->context_id)
    {
        return MMI_RESULT_FALSE;
    }

    response_code = signal->response_code;

    SCI_TRACE_LOW("HandleNetProxyHttpDataInd recv_len=%d, total_len=%d, data_len=%d, response_code=%d",
        http_context_ptr->recv_len, http_context_ptr->data_len, signal->data_len, response_code);

    if (200 == response_code || 206 == response_code || 301 == response_code
        || 302 == response_code || 307 == response_code || 401 == response_code || 407 == response_code)
    {
        if (http_context_ptr->recv_len + signal->data_len <= http_context_ptr->data_len)
        {
            SCI_MEMCPY(http_context_ptr->data_ptr + http_context_ptr->recv_len, signal->data_ptr, signal->data_len);
            http_context_ptr->recv_len += signal->data_len;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Header Ind
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpHeaderInd(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_HEADER_IND_SIG_T* signal = NULL;
    signal = (HTTP_HEADER_IND_SIG_T*)param;
    //GetSignalStruct(param, &signal, sizeof(HTTP_HEADER_IND_SIG_T));

    if (PNULL == http_context_ptr || http_context_ptr->context_id != signal->context_id)
    {
        return MMI_RESULT_FALSE;
    }

    SCI_TRACE_LOW("HandleNetProxyHttpHeaderInd response_code=%d, content_len=%d", signal->rsp_header_info.response_code, signal->rsp_header_info.content_length);

    if (PNULL != http_context_ptr->data_ptr)
    {
        SCI_TRACE_LOW("HandleNetProxyHttpHeaderInd warrning ");
        SCI_FREE(http_context_ptr->data_ptr);
    }

    http_context_ptr->recv_len = 0;
    http_context_ptr->data_len = MIN(signal->rsp_header_info.content_length, JASON_DATA_LENGTH_MAX);
    http_context_ptr->data_ptr = SCI_ALLOCA(http_context_ptr->data_len + 1);

    if (PNULL != http_context_ptr->data_ptr)
    {
        SCI_MEMSET(http_context_ptr->data_ptr, 0x00, (http_context_ptr->data_len + 1));
    }
    else
    {
        SCI_TRACE_LOW("HandleNetProxyHttpHeaderInd No Memory");
        HTTP_CloseRequest(http_context_ptr->context_id);
    }

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Error Ind
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpErrorInd(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_ERROR_IND_SIG_T* signal = NULL;
    signal = (HTTP_ERROR_IND_SIG_T*)param;
   // GetSignalStruct(param, &signal, sizeof(HTTP_ERROR_IND_SIG_T));

    if (PNULL == http_context_ptr || http_context_ptr->context_id != signal->context_id)
    {
        return MMI_RESULT_FALSE;
    }

    if(signal->result != HTTP_SUCCESS)
    {
        http_context_ptr->result = NET_ERROR_FAILD;
    }
    else
       http_context_ptr->result = NET_SUCCESS;

    HTTP_CloseRequest(http_context_ptr->context_id);

     return result; 
}

/*****************************************************************************/
//  Description : Handle Http Close Cnf
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpCloseCnf(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_CLOSE_CNF_SIG_T* signal = NULL;
    signal = (HTTP_CLOSE_CNF_SIG_T*)param;
    //GetSignalStruct(param, &signal, sizeof(HTTP_CLOSE_CNF_SIG_T));

    if (PNULL == http_context_ptr || http_context_ptr->context_id != signal->context_id)
    {
        return MMI_RESULT_FALSE;
    }

    http_context_ptr->app_instance = 0;
    http_context_ptr->context_id   = 0;
    http_context_ptr->request_id   = 0;
    http_context_ptr->module_id    = 0;

    if (s_request.body)
    {
        SCI_FREE(s_request.body);
    }
    if (s_request.uri)
    {
        SCI_FREE(s_request.uri);
    }

    SCI_MEMSET(&s_request, 0, sizeof(NET_REQUEST_PARAM_T));

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Cancel Cnf
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpCancelCnf(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_CANCEL_CNF_SIG_T* signal = NULL;
    signal = (HTTP_CANCEL_CNF_SIG_T*)param;
    //GetSignalStruct(param, &signal, sizeof(HTTP_CANCEL_CNF_SIG_T));

    if (PNULL == http_context_ptr || http_context_ptr->context_id != signal->context_id)
    {
        return MMI_RESULT_FALSE;
    }
    HTTP_CloseRequest(http_context_ptr->context_id);
//http_context_ptr->result = NET_CANCEL;
    return result;   
}

/*****************************************************************************/
//  Description : Handle Http Post Cnf
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetProxyHttpPostCnf(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_POST_CNF_SIG_T* signal = NULL;
    signal = (HTTP_POST_CNF_SIG_T*)param;
    if (PNULL == http_context_ptr || http_context_ptr->context_id !=signal->context_id)
    {
        return MMI_RESULT_FALSE;
    }

    if(signal->result != HTTP_SUCCESS)
    {
        http_context_ptr->result = NET_ERROR_FAILD;
    }
    else
    {
        http_context_ptr->result = NET_SUCCESS;
    }

    HandleNetProxyReceivedData(http_context_ptr);

    HTTP_CloseRequest(http_context_ptr->context_id);

    return result;
}

/*****************************************************************************/
//  Description : Handle net data
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void HandleNetProxyReceivedData(MMINETPROXY_HTTP_CONTEXT_T *http_context_ptr)
{
       if(PNULL == http_context_ptr)
       {
            SCI_TRACE_LOW("[NetProxy]:HandleNetProxyReceivedData invalid input param");
	     return;
	 }
	if (NULL != s_request.callback)
	{
		(*s_request.callback)(http_context_ptr->result, (const char *)http_context_ptr->data_ptr, http_context_ptr->data_len, s_request.user_data);
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
void open_browser(const char * url, uint32 netId)
{
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
    entry_param.type = MMIBROWSER_ACCESS_URL;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.url_ptr = url;
    entry_param.user_agent_ptr = PNULL;
    entry_param.net_id = netId;
#if defined(BROWSER_SUPPORT_WEBKIT) && (!defined(WIN32) || (defined(_MSC_VER) && (_MSC_VER >= 1400)))
    SCI_TRACE_LOW("[NetProxy]:open_browser");
    MMIWK_Entry(&entry_param);
#else 
    MMIBROWSER_Entry(&entry_param);
#endif
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
void exit_browser(uint16 exit_type)
{
#ifdef BROWSER_SUPPORT
#if defined(BROWSER_SUPPORT_WEBKIT) && (!defined(WIN32) || (defined(_MSC_VER) && (_MSC_VER >= 1400)))
    SCI_TRACE_LOW("[NetProxy]:exit_browser");
    MMIWK_Exit(exit_type);
if(exit_type == 1)
{
   if (s_request.body)
    {
        SCI_FREE(s_request.body);
    }
    if (s_request.uri)
    {
        SCI_FREE(s_request.uri);
    }

    SCI_MEMSET(&s_request, 0, sizeof(NET_REQUEST_PARAM_T));
}
#else
    MMIBROWSER_Close();
#endif
#endif
}
