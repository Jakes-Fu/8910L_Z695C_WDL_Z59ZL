
#ifdef  FOTA_SUPPORT

#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "lcd_cfg.h"
#include "mmk_app.h"
#include "mmiphone_export.h"
#include "mmi_filemgr.h"
#include "http_api.h"
#include "version.h"
#include "socket_api.h"
#include "mmipdp_export.h"
#include "in_message.h"
#include "mmieng_internal.h"
#include "mmisms_app.h"
#include "mmieng_nv.h"
#include "mmiconnection_export.h"
#include "mmipdp_internal.h"
#include "mmi_module.h"
#include "dal_time.h"
#include "mmk_authen.h"
#include "mmisms_set.h"
#include "mmi_nv.h"
#include "fota_api.h"
#include "fota_header.h"

#define HTTP_SIGNAL_ANALYTIC_EX(param,signal,app_param,http_context_ptr,app_module_id,sig_type) \
do{\
    Fota_GetSignalStruct(param, &signal, sizeof(sig_type));\
    if(signal.module_id != app_module_id)\
    {\
        return MMI_RESULT_FALSE;\
    }\
    app_param = signal.app_param;\
    http_context_ptr->context_id   = signal.context_id;\
    http_context_ptr->app_instance = 0;\
    http_context_ptr->request_id = 0;\
    http_context_ptr->module_id    = signal.module_id;\
}while(0)\

#define HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,sig_type) \
do{\
    Fota_GetSignalStruct(param, &signal, sizeof(sig_type));\
    if(signal.context_id != http_context_ptr->context_id)\
    {\
        return MMI_RESULT_FALSE;\
    }\
}while(0)\

LOCAL void Fota_GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size)
{
    if((PNULL != param)&&(PNULL != signal_struct_ptr)&&(struct_size >= sizeof(xSignalHeaderRec)))
    {
        SCI_MEMCPY((uint8 *)((uint32)signal_struct_ptr), param, (struct_size ));
    }
}

#define SEC_RETMSG "\"message\":"
#define SEC_RET_OK "\"ok\""
#define SEC_TOKEN "\"token\":"
#define SEC_VERSION "\"version\":"
#define SEC_OPERATOR_IDX "\"operatorindex\":"

LOCAL void Fota_ParseTokenData(const char *_data, int _len)
{
    int i;
    char *p, *ps, *pe;
    char buff[32];
    SCI_TraceLow("[FOTA Update] Fota_ParseTokenData");
    if (_data[0] != '{' || _data[_len - 1] != '}')
    {
        SCI_TraceLow("[FOTA Update] Token Data not json");
        return;
    }
    _data++; _len -= 2;

    p = strstr(_data, SEC_RETMSG);
    if (p == NULL) 
    {
        SCI_TraceLow("[FOTA Update] Token Data no message");
        return;
    }
    p += strlen(SEC_RETMSG);
    if (memcmp(p, SEC_RET_OK, strlen(SEC_RET_OK)) != 0)
    {
        SCI_TraceLow("[FOTA Update] Token Data err msg != ok");
        return;
    }

    p = strstr(_data, SEC_TOKEN);
    if (p == NULL) 
    {
        SCI_TraceLow("[FOTA Update] Token Data no token");
        return;
    }
    p += strlen(SEC_TOKEN);

    if(*p != '\"')
    {
        SCI_TraceLow("[FOTA Update] Token Data no token data");
        return;
    }
    ps = p + 1;
    pe = strchr(ps, '\"');

    Fota_SetToken(ps, pe - ps);
}

#define SEC_APN "\"apnurl\":"
#define SEC_CFGNV "\"carrierconfigurl\":"
#define SEC_IMS "\"imsurl\":"
#define SEC_TOGGLE "\"customizationurl\":"

LOCAL void Fota_ParseDownloadData(char *_data, int _len)
{
    int i;
    char *p, *ps, *pe;
    char buff[32];
    SCI_TraceLow("[FOTA Update] Fota_ParseDownloadData");
    if (_data[0] != '{' || _data[_len - 1] != '}')
    {
        SCI_TraceLow("[FOTA Update] Download Data not json");
        return;
    }
    _data++; _len -= 2;

    p = strstr(_data, SEC_RETMSG);
    if (p == NULL) 
    {
        SCI_TraceLow("[FOTA Update] Download Data no message");
        return;
    }
    p += strlen(SEC_RETMSG);
    if (memcmp(p, SEC_RET_OK, strlen(SEC_RET_OK)) != 0)
    {
        SCI_TraceLow("[FOTA Update] Download Data err msg != ok");
        return;
    }

    p = strstr(_data, SEC_VERSION);
    if (p != NULL)
    {
        int v = 0;
        p += strlen(SEC_VERSION);
	 //save version
	 while (*p != ',')
	 {
	     if (v != 0)
	         v *= 10;
	     v += (*p) - '0';
	     p++;
	 }
	 Fota_SetVersion(v);
	 SCI_TraceLow("[FOTA Update] Download Data version = %d", v);
    }

    p = strstr(_data, SEC_OPERATOR_IDX);
    p += strlen(SEC_OPERATOR_IDX);
    if (*p == '\"')
    {
        int oi;
        ps = p + 1;
        pe = strchr(ps, '\"');
	 //save operator index
	 for (oi = 0; ps <=pe; ps++, oi *= 10)
	 {
	     oi += *ps;
	 }
	 Fota_SetOperatorIndex(oi);
	 SCI_TraceLow("[FOTA Update] Download Data operator index = %d", oi);
    }

    p = strstr(_data, SEC_APN);
    p += strlen(SEC_APN);
    if (*p == '\"')
    {
        ps = p + 1;
        pe = strchr(ps, '\"');
        Fota_SetApnUrl(ps, pe - ps);
        SCI_TraceLow("[FOTA Update] Apn ==");
        Fota_LongTrace(Fota_GetApnUrl(), pe - ps);
    }

    p = strstr(_data, SEC_CFGNV);
    p += strlen(SEC_CFGNV);
    if (*p == '\"')
    {
        ps = p + 1;
        pe = strchr(ps, '\"');
        Fota_SetCfgNvUrl(ps, pe - ps);
        SCI_TraceLow("[FOTA Update] Config nv ==");
        Fota_LongTrace(Fota_GetCfgNvUrl(), pe - ps);
    }

    p = strstr(_data, SEC_IMS);
    p += strlen(SEC_IMS);
    if (*p == '\"')
    {
        ps = p + 1;
        pe = strchr(ps, '\"');
        Fota_SetIMSUrl(ps, pe - ps);
        SCI_TraceLow("[FOTA Update] IMS ==");
        Fota_LongTrace(Fota_GetIMSUrl(), pe - ps);
    }

    p = strstr(_data, SEC_TOGGLE);
    p += strlen(SEC_TOGGLE);
    if (*p == '\"')
    {
        ps = p + 1;
        pe = strchr(ps, '\"');
        Fota_SetIMSToggleUrl(ps, pe - ps);
        SCI_TraceLow("[FOTA Update] IMS Toggle ==");
        Fota_LongTrace(Fota_GetIMSToggleUrl(), pe - ps);
    }
}

PUBLIC void Fota_StartHttpRequest(void)
{
    HTTP_INIT_PARAM_T* init_param_ptr = PNULL;
    init_param_ptr = SCI_ALLOC_APPZ(sizeof(HTTP_INIT_PARAM_T));
    init_param_ptr->is_cookie_enable = FALSE;
    init_param_ptr->is_cache_enable = FALSE;

    SCI_TraceLow("[FOTA Update] Fota_StartHttpRequest");

    if (HTTP_SUCCESS != HTTP_InitRequest(MMI_MODULE_FOTA, 1, init_param_ptr))
    {
        CHECK_VERSION_RETURN(FOTA_INIT_REQ_ERR);
    }
    else 
    {
        Fota_SM_Set(FOTA_SM_NULL);
    }

    SCI_FREE(init_param_ptr);
}

LOCAL MMI_RESULT_E Fota_HandleHttpSigInitCnf (FOTA_HTTP_CONTEXT *http_context_ptr, DPARAM param);
LOCAL MMI_RESULT_E Fota_HandleHttpSigRequestIdInd (FOTA_HTTP_CONTEXT *http_context_ptr, DPARAM param);
LOCAL MMI_RESULT_E Fota_HandleHttpSigRedriectInd (FOTA_HTTP_CONTEXT *http_context_ptr, DPARAM param);
LOCAL MMI_RESULT_E Fota_HandleHttpSigErrorInd (FOTA_HTTP_CONTEXT *http_context_ptr, DPARAM param);
LOCAL MMI_RESULT_E Fota_HandleHttpSigHeaderInd (FOTA_HTTP_CONTEXT *http_context_ptr, DPARAM param);
LOCAL MMI_RESULT_E Fota_HandleHttpSigDataInd (FOTA_HTTP_CONTEXT *http_context_ptr, DPARAM param);
LOCAL MMI_RESULT_E Fota_HandleHttpSigPostCnf (FOTA_HTTP_CONTEXT *http_context_ptr, DPARAM param);
LOCAL MMI_RESULT_E Fota_HandleHttpSigGetCnf (FOTA_HTTP_CONTEXT *http_context_ptr, DPARAM param);
LOCAL MMI_RESULT_E Fota_HandleHttpSigCloseCnf (FOTA_HTTP_CONTEXT *http_context_ptr, DPARAM param);

PUBLIC MMI_RESULT_E Fota_HandleHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    FOTA_HTTP_CONTEXT *http_context_ptr = &g_fota_linker.http_ctx;

    switch(msg_id)
    {
        case HTTP_SIG_INIT_CNF:
            SCI_TraceLow("[FOTA Update] http : HTTP_SIG_INIT_CNF");
            result = Fota_HandleHttpSigInitCnf(http_context_ptr, param);
            break;
        case HTTP_SIG_REQUEST_ID_IND:
            SCI_TraceLow("[FOTA Update] http : HTTP_SIG_REQUEST_ID_IND");
            result = Fota_HandleHttpSigRequestIdInd(http_context_ptr, param);
            break;
        case HTTP_SIG_REDIRECT_IND:
            SCI_TraceLow("[FOTA Update] http : HTTP_SIG_REDIRECT_IND");
            result = Fota_HandleHttpSigRedriectInd(http_context_ptr, param);
            break;
        case HTTP_SIG_ERROR_IND:
            SCI_TraceLow("[FOTA Update] http : HTTP_SIG_ERROR_IND");
            result = Fota_HandleHttpSigErrorInd(http_context_ptr, param);
            break;
        case HTTP_SIG_HEADER_IND:
            SCI_TraceLow("[FOTA Update] http : HTTP_SIG_HEADER_IND");
            result = Fota_HandleHttpSigHeaderInd(http_context_ptr, param);
            break;
        case HTTP_SIG_DATA_IND:
            SCI_TraceLow("[FOTA Update] http : HTTP_SIG_DATA_IND");
            result = Fota_HandleHttpSigDataInd(http_context_ptr, param);
            break;
        case HTTP_SIG_POST_CNF:
            SCI_TraceLow("[FOTA Update] http : HTTP_SIG_POST_CNF");
            result = Fota_HandleHttpSigPostCnf(http_context_ptr, param);
            break;
        case HTTP_SIG_GET_CNF:
            SCI_TraceLow("[FOTA Update] http : HTTP_SIG_GET_CNF");
            result = Fota_HandleHttpSigGetCnf(http_context_ptr, param);
            break;	
        case HTTP_SIG_CLOSE_CNF:
            SCI_TraceLow("[FOTA Update] http : HTTP_SIG_CLOSE_CNF");
            result = Fota_HandleHttpSigCloseCnf(http_context_ptr, param);
            break;	 	
        default:
            break;
    }
    return result;
}

LOCAL MMI_RESULT_E Fota_HandleHttpSigInitCnf (FOTA_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    HTTP_INIT_CNF_SIG_T * cnf = (HTTP_INIT_CNF_SIG_T*)param;
    HTTP_INIT_CNF_SIG_T signal = {0};
    HTTP_APP_PARAM_T app_param = 0;

    if(cnf->module_id != MMI_MODULE_FOTA || 1 != cnf->app_param || http_context_ptr == NULL)
    {
        return MMI_RESULT_FALSE;
    }

    HTTP_SIGNAL_ANALYTIC_EX(param, signal, app_param, http_context_ptr, MMI_MODULE_FOTA, HTTP_INIT_CNF_SIG_T);

    if (signal.result != HTTP_SUCCESS)
    {
        SCI_TraceLow("[FOTA Update] Fota_HandleHttpSigInitCnf <error = %d> ", signal.result);
        CHECK_VERSION_RETURN(FOTA_INIT_CNF_ERR);
        return MMI_RESULT_FALSE;
    }

    Fota_SM_Set(FOTA_SM_INIT);
    Fota_SM_Process();

    return MMI_RESULT_TRUE; 
}

LOCAL MMI_RESULT_E Fota_HandleHttpSigRequestIdInd (FOTA_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    HTTP_REQUEST_ID_IND_SIG_T signal = {0};
    HTTP_REQUEST_ID_IND_SIG_T *cnf = (HTTP_REQUEST_ID_IND_SIG_T*)param;

    HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_REQUEST_ID_IND_SIG_T);

    if (MMI_MODULE_FOTA == signal.app_instance)
    {
        http_context_ptr->context_id = signal.context_id;
        http_context_ptr->request_id = signal.request_id;
        SCI_TraceLow("[FOTA Update] Fota_HandleHttpSigRequestIdInd request_id:%ld,http_context_ptr->context_id:%d",signal.request_id,http_context_ptr->context_id);

        return MMI_RESULT_TRUE;
    }

    return MMI_RESULT_FALSE;
}

LOCAL MMI_RESULT_E Fota_HandleHttpSigRedriectInd (FOTA_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    HTTP_REDIRECT_IND_SIG_T signal = {0};
    HTTP_REDIRECT_IND_SIG_T *cnf = (HTTP_REDIRECT_IND_SIG_T*)param;

    HTTP_SIGNAL_ANALYTIC(param, signal, http_context_ptr, HTTP_REDIRECT_IND_SIG_T);

    SCI_TraceLow("[FOTA Update] Fota_HandleHttpSigRedriectInd redirect url :%s", signal.redirect_ptr);

    if (http_context_ptr->context_id != signal.context_id || http_context_ptr->request_id != signal.request_id)
    {
        return MMI_RESULT_FALSE;
    }
    //TODO
    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E Fota_HandleHttpSigErrorInd (FOTA_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    HTTP_ERROR_IND_SIG_T signal = {0};
    HTTP_ERROR_IND_SIG_T *cnf = (HTTP_ERROR_IND_SIG_T*)param;

    HTTP_SIGNAL_ANALYTIC(param, signal, http_context_ptr, HTTP_ERROR_IND_SIG_T);

    SCI_TraceLow("[FOTA Update] Fota_HandleHttpSigErrorInd request_id : %d,  error :%d, context request_id : %d", 
		signal.request_id, signal.result, http_context_ptr->request_id);

    if (http_context_ptr->context_id != signal.context_id || http_context_ptr->request_id != signal.request_id)
    {
        return MMI_RESULT_FALSE;
    }

    //TODO
    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E Fota_HandleHttpSigHeaderInd (FOTA_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    HTTP_HEADER_IND_SIG_T signal = {0};
    HTTP_HEADER_IND_SIG_T *cnf = (HTTP_HEADER_IND_SIG_T*)param;
    FOTA_SM_ENUM sm = Fota_SM_Get();

    HTTP_SIGNAL_ANALYTIC(param, signal, http_context_ptr, HTTP_HEADER_IND_SIG_T);

    SCI_TraceLow("[FOTA Update] Fota_HandleHttpSigHeaderInd header_ptr");
    SCI_TraceLow("[FOTA Update]  header_ptr :%s", signal.header_ptr);
    SCI_TraceLow("[FOTA Update]  rsp_header_info.charset_ptr :%s", signal.rsp_header_info.charset_ptr);
    SCI_TraceLow("[FOTA Update]  rsp_header_info.content_encoding_type :%d", signal.rsp_header_info.content_encoding_type);
    SCI_TraceLow("[FOTA Update]  rsp_header_info.content_length :%d", signal.rsp_header_info.content_length);
    SCI_TraceLow("[FOTA Update]  rsp_header_info.content_length :%s", signal.rsp_header_info.content_type_ptr);
    SCI_TraceLow("[FOTA Update]  rsp_header_info.response_code :%d", signal.rsp_header_info.response_code);
    SCI_TraceLow("[FOTA Update]  rsp_header_info.session_id_ptr :%s", signal.rsp_header_info.session_id_ptr);

    if (http_context_ptr->context_id != signal.context_id || http_context_ptr->request_id != signal.request_id)
    {
        return MMI_RESULT_FALSE;
    }

    if(signal.rsp_header_info.response_code == HTTP_RESPONSE_CODE_OK)
    {
        SCI_TraceLow("[FOTA Update] Fota_HandleHttpSigHeaderInd SM = %d", sm);

        if (sm  == FOTA_SM_APN_DOWNLOAD || sm == FOTA_SM_CNV_DOWNLOAD || sm == FOTA_SM_IMS_DOWNLOAD || sm == FOTA_SM_TOG_DOWNLOAD)
        {
            SCI_TraceLow("[FOTA Update]  == HTTP_HeaderResponse <file>");

            return HTTP_HeaderResponse(http_context_ptr->context_id, http_context_ptr->app_instance,
    					http_context_ptr->request_id, HTTP_DATA_TRANS_STYLE_FILE, PNULL, 0);
        }
	 else
	 {
            SCI_TraceLow("[FOTA Update]  == HTTP_HeaderResponse <buffer>");

            return HTTP_HeaderResponse(http_context_ptr->context_id, http_context_ptr->app_instance,
    					http_context_ptr->request_id, HTTP_DATA_TRANS_STYLE_BUFFER, PNULL, 0);
	 }
    }
    else
    {
        if (sm == FOTA_SM_INIT)
		CHECK_VERSION_RETURN(FOTA_LOGIN_ERR);
	 else if (sm == FOTA_SM_PRE_DOWNLOAD)
		DOWN_PACKAGE_RETURN(FOTA_GET_URL_ERR);
	 else if (sm == FOTA_SM_APN_DOWNLOAD)
		DOWN_PACKAGE_RETURN(FOTA_GET_APN_ERR);
	 else if (sm == FOTA_SM_CNV_DOWNLOAD)
		DOWN_PACKAGE_RETURN(FOTA_GET_CNV_ERR);
	 else if (sm == FOTA_SM_IMS_DOWNLOAD)
		DOWN_PACKAGE_RETURN(FOTA_GET_IMS_ERR);
	 else if (sm == FOTA_SM_TOG_DOWNLOAD)
		DOWN_PACKAGE_RETURN(FOTA_GET_TOG_ERR);
    }

    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E Fota_HandleHttpSigDataInd (FOTA_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    HTTP_DATA_IND_SIG_T signal = {0};
    HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;

    HTTP_SIGNAL_ANALYTIC(param, signal, http_context_ptr, HTTP_DATA_IND_SIG_T);

    SCI_TraceLow("[FOTA Update] Fota_HandleHttpSigDataInd response_code :%d, data_len : %d", cnf->response_code, cnf->data_len);

    if (http_context_ptr->context_id != signal.context_id || http_context_ptr->request_id != signal.request_id)
    {
        return MMI_RESULT_FALSE;
    }

    SCI_TraceLow("[FOTA Update] Fota_HandleHttpSigDataInd :");
    Fota_LongTrace(cnf->data_ptr, cnf->data_len);

    if (cnf->data_len > 0)
    {
        FOTA_SM_ENUM sm = Fota_SM_Get();
        SCI_TraceLow("[FOTA Update] Fota_HandleHttpSigDataInd Fota_SM_Get = %d", sm);
        if(PNULL != signal.data_ptr)
        {
            if (sm  == FOTA_SM_INIT)
            {
                Fota_ParseTokenData(signal.data_ptr, signal.data_len );
            }
            else if (sm == FOTA_SM_GET_URL)
            {
                Fota_ParseDownloadData(signal.data_ptr, signal.data_len );
            }
        }
    }

    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E Fota_HandleHttpSigPostCnf (FOTA_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    HTTP_POST_CNF_SIG_T signal = {0};
    HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)param;

    HTTP_SIGNAL_ANALYTIC(param, signal, http_context_ptr, HTTP_POST_CNF_SIG_T);

    SCI_TraceLow("[FOTA Update] Fota_HandleHttpSigPostCnf result :%d", signal.result);
    
    if(signal.result == HTTP_SUCCESS)
    {
        // TODO
        return MMI_RESULT_FALSE;
    }

    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E Fota_HandleHttpSigGetCnf (FOTA_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    HTTP_GET_CNF_SIG_T signal = {0};
    HTTP_GET_CNF_SIG_T *cnf = (HTTP_GET_CNF_SIG_T*)param;

    HTTP_SIGNAL_ANALYTIC(param, signal, http_context_ptr, HTTP_GET_CNF_SIG_T);

    SCI_TraceLow("[FOTA Update] Fota_HandleHttpSigGetCnfresult :%d", signal.result);
    
    if(signal.result == HTTP_SUCCESS)
    {
        //
        // Come here, the request send to http ok
        //
        FOTA_SM_ENUM sm = Fota_SM_Get();

        SCI_TraceLow("[FOTA Update] Fota_HandleHttpSigGetCnfresult SM = %d", sm);
        if (sm  == FOTA_SM_INIT)
        {
            Fota_SM_Set(FOTA_SM_REQUEST);
            Fota_SM_Process();
        }
        else if (sm == FOTA_SM_GET_URL)
        {
            Fota_SM_Set(FOTA_SM_PRE_DOWNLOAD);
            Fota_SM_Process();
        }
        else if (sm == FOTA_SM_PRE_DOWNLOAD)
        {
            if (Fota_GetApnUrl() != NULL)
                Fota_SM_Set(FOTA_SM_APN_DOWNLOAD);
            else if (Fota_GetCfgNvUrl() != NULL)
                Fota_SM_Set(FOTA_SM_CNV_DOWNLOAD);
            else if (Fota_GetIMSUrl() != NULL)
                Fota_SM_Set(FOTA_SM_IMS_DOWNLOAD);
            else if (Fota_GetIMSToggleUrl() != NULL)
                Fota_SM_Set(FOTA_SM_TOG_DOWNLOAD);
            else 
                Fota_SM_Set(FOTA_SM_DOWN_ERR);// it will retry to get url data ??
            Fota_SM_Process();
        }
        else if (sm  == FOTA_SM_APN_DOWNLOAD)
        {
            if (Fota_GetCfgNvUrl() != NULL)
                Fota_SM_Set(FOTA_SM_CNV_DOWNLOAD);
            else if (Fota_GetIMSUrl() != NULL)
                Fota_SM_Set(FOTA_SM_IMS_DOWNLOAD);
            else if (Fota_GetIMSToggleUrl() != NULL)
                Fota_SM_Set(FOTA_SM_TOG_DOWNLOAD);
            else
                Fota_SM_Set(FOTA_SM_UPDATING);
            Fota_SM_Process();
        }
        else if (sm == FOTA_SM_CNV_DOWNLOAD)
        {
            if (Fota_GetIMSUrl() != NULL)
                Fota_SM_Set(FOTA_SM_IMS_DOWNLOAD);
            else if (Fota_GetIMSToggleUrl() != NULL)
                Fota_SM_Set(FOTA_SM_TOG_DOWNLOAD);
            else 
                Fota_SM_Set(FOTA_SM_UPDATING);
            Fota_SM_Process();
        }
        else if (sm == FOTA_SM_IMS_DOWNLOAD)
        {
            if (Fota_GetIMSToggleUrl() != NULL)
                Fota_SM_Set(FOTA_SM_TOG_DOWNLOAD);
            else 
                Fota_SM_Set(FOTA_SM_UPDATING);
            Fota_SM_Process();
        }
	else if (sm == FOTA_SM_TOG_DOWNLOAD)
        {
            Fota_SM_Set(FOTA_SM_UPDATING);
            Fota_SM_Process();
        }
        // TODO
        // else if ....
        else 
            return MMI_RESULT_FALSE;
    }

    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E Fota_HandleHttpSigCloseCnf (FOTA_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    HTTP_CLOSE_CNF_SIG_T signal = {0};
    HTTP_CLOSE_CNF_SIG_T *cnf = (HTTP_CLOSE_CNF_SIG_T*)param;

    HTTP_SIGNAL_ANALYTIC(param, signal, http_context_ptr, HTTP_CLOSE_CNF_SIG_T);

    SCI_TraceLow("[FOTA Update] Fota_HandleHttpSigCloseCnf result :%d", signal.result);
    //TODO

    Fota_SM_Set(FOTA_SM_NULL);
    Fota_SM_Process();

    return result;
}



#endif
