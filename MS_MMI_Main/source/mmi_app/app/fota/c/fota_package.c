#ifdef  FOTA_SUPPORT

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
#include "mn.h"
#include "sim_file_structure.h"
#ifndef WIN32
#include "mbedtls/error.h"
#include "mbedtls/ssl.h"
#include "mbedtls/net.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/pk.h"
#include "mbedtls/debug.h"
#include "mbedtls/platform.h"
#include "mbedtls/mbedtls_import.h"
#include "mbedtls/mbedtls_import_dtls.h"
#endif
#include "fota_api.h"
#include "fota_header.h"
#include "fota_nv.h"

#define FOTA_HTTP_URI_LOGIN         "https://test/API/login"
#define FOTA_HTTP_URI_GETDATA         "https://test/API/getData"

#define FOTA_HTTP_CONTENT_TYPE "application/json;charset=UTF-8"
#define FOTA_ACCEPT_STRING  "text/html, application/json, */*"
#define FOTA_USER_AGENT_STRING       "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)"

#define FOTA_BODY_URL_AND  "&"
#define FOTA_BODY_URL_PACKAGE_HEAD  "package="
#define FOTA_BODY_URL_TOKEN_HEAD  "token="
#define FOTA_BODY_URL_TOKENKEY_HEAD  "TokenKey="
#define FOTA_BODY_URL_IP_HEAD  "sourceip="
#define FOTA_BODY_URL_PROJ_HEAD  "project="
#define FOTA_BODY_URL_TIME_HEAD  "datetime="
#define FOTA_BODY_URL_MCC_HEAD  "mcc="
#define FOTA_BODY_URL_MNC_HEAD  "mnc="
#define FOTA_BODY_URL_OSVER_HEAD  "osversion="
#define FOTA_BODY_URL_VER_HEAD  "version="
#define FOTA_BODY_URL_IMEI_HEAD  "imei="
#define FOTA_BODY_URL_SPN_HEAD  "SPN="
#define FOTA_BODY_URL_GID_HEAD  "GID="
#define FOTA_BODY_URL_PNN_HEAD  "PNN="
#define FOTA_BODY_URL_IMSI_HEAD  "imsi="
#define FOTA_BODY_URL_PCODE_HEAD  "pcode="
#define FOTA_BODY_URL_OPERATOR_HEAD  "operatorindex="

#define FOTA_BODY_LEN (1024)

extern PHONE_SERVICE_STATUS_T  g_service_status[MMI_DUAL_SYS_MAX];

LOCAL char *s_token = NULL;
LOCAL char *s_apn_addr = NULL;
LOCAL char *s_cfgnv_addr = NULL;
LOCAL char *s_ims_addr = NULL;
LOCAL char *s_imstoggle_addr = NULL;
LOCAL uint32 s_version = 0;
LOCAL uint32 s_operator_index = 0;

PUBLIC void Fota_RestAllUrl(void)
{
    if (s_token != NULL)
        SCI_Free(s_token);
    s_token = NULL;

    if (s_apn_addr != NULL)
        SCI_Free(s_apn_addr);
    s_apn_addr = NULL;

    if (s_cfgnv_addr != NULL)
        SCI_Free(s_cfgnv_addr);
    s_cfgnv_addr = NULL;

    if (s_ims_addr != NULL)
        SCI_Free(s_ims_addr);
    s_ims_addr = NULL;
}

PUBLIC void Fota_SetVersion(uint32 v) {s_version = v;}
PUBLIC uint32 Fota_GetVersion(void) {return s_version;}

PUBLIC void Fota_SetOperatorIndex(uint32 i) {s_operator_index = i;}
PUBLIC uint32 Fota_GetOperatorIndex(void) {return s_operator_index;}

PUBLIC void Fota_SetToken(char *ptr, int len)
{
    if (ptr == NULL)
        return;

    if (s_token != NULL)
        SCI_Free(s_token);
    
    s_token = SCI_ALLOC_APPZ(len + 1);
    SCI_MEMCPY (s_token, ptr, len);

    s_token[len] = '\0';
    SCI_TraceLow("[FOTA Update] Fota_SetToken =  (%d)", len);
    Fota_LongTrace(s_token, len);
}

PUBLIC char * Fota_GetToken(void)
{
    return s_token;
}

PUBLIC void Fota_SetApnUrl(char *ptr, int len)
{
    if (ptr == NULL)
        return;

    if (s_apn_addr != NULL)
        SCI_Free(s_apn_addr);
    
    s_apn_addr = SCI_ALLOC_APPZ(len + 1);
    SCI_MEMCPY (s_apn_addr, ptr, len);

    s_apn_addr[len] = '\0';
    SCI_TraceLow("[FOTA Update] Fota_SetApnUrl =  (%d) %s", len, s_apn_addr);
}

PUBLIC char * Fota_GetApnUrl(void)
{
    return s_apn_addr;
}

PUBLIC void Fota_SetCfgNvUrl(char *ptr, int len)
{
    if (ptr == NULL)
        return;

    if (s_cfgnv_addr != NULL)
        SCI_Free(s_cfgnv_addr);
    
    s_cfgnv_addr = SCI_ALLOC_APPZ(len + 1);
    SCI_MEMCPY (s_cfgnv_addr, ptr, len);

    s_cfgnv_addr[len] = '\0';
    SCI_TraceLow("[FOTA Update] Fota_SetCfgNvUrl =  (%d) %s", len, s_cfgnv_addr);
}

PUBLIC char * Fota_GetCfgNvUrl(void)
{
    return s_cfgnv_addr;
}

PUBLIC void Fota_SetIMSUrl(char *ptr, int len)
{
    if (ptr == NULL)
        return;

    if (s_ims_addr != NULL)
        SCI_Free(s_ims_addr);
    
    s_ims_addr = SCI_ALLOC_APPZ(len + 1);
    SCI_MEMCPY (s_ims_addr, ptr, len);

    s_ims_addr[len] = '\0';
    SCI_TraceLow("[FOTA Update] Fota_SetIMSUrl =  (%d) %s", len, s_ims_addr);
}

PUBLIC char * Fota_GetIMSUrl(void)
{
    return s_ims_addr;
}

PUBLIC void Fota_SetIMSToggleUrl(char *ptr, int len)
{
    if (ptr == NULL)
        return;

    if (s_imstoggle_addr != NULL)
        SCI_Free(s_imstoggle_addr);
    
    s_imstoggle_addr = SCI_ALLOC_APPZ(len + 1);
    SCI_MEMCPY (s_imstoggle_addr, ptr, len);

    s_imstoggle_addr[len] = '\0';
    SCI_TraceLow("[FOTA Update] Fota_SetIMSUrl =  (%d) %s", len, s_imstoggle_addr);
}

PUBLIC char * Fota_GetIMSToggleUrl(void)
{
    return s_imstoggle_addr;
}

#ifndef WIN32
LOCAL int Fota_GetIMSI(char *buff, int buff_len)
{
    MN_IMSI_T tmp_imsi = MNSIM_GetImsiEx(FOTA_UPDATE_SIM);
    
    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)tmp_imsi.imsi_val, MN_MAX_IMSI_ARR_LEN<<1, buff);

    return strlen(buff);
}

LOCAL int Fota_GetIMEI(char *buff, int buff_len)
{
    int i;
    char imei_str[18]={0};
    MN_IMEI_T tmp_imei;

    if (MNNV_GetIMEIEx(FOTA_UPDATE_SIM, tmp_imei))
    {
        for (i = 0; i < 8; i++) 
        {
            imei_str[2 * i] = (tmp_imei[i]) & 0x0F;
            imei_str[2 * i + 1] = tmp_imei[i] >> 4;
        }
    }
    sprintf(buff,
            "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",
            imei_str[1], imei_str[2], imei_str[3], imei_str[4],
            imei_str[5], imei_str[6], imei_str[7], imei_str[8],
            imei_str[9], imei_str[10], imei_str[11], imei_str[12],
            imei_str[13], imei_str[14], imei_str[15]);

    return strlen(buff);
}

LOCAL int Fota_GetMCC(char *buff, int buff_len)
{
    MMIAPICOM_Int2Str(g_service_status[FOTA_UPDATE_SIM].plmn.mcc, buff, buff_len);
    return strlen(buff);
}

LOCAL int Fota_GetMNC(char *buff, int buff_len)
{
    MMIAPICOM_Int2Str(g_service_status[FOTA_UPDATE_SIM].plmn.mnc, buff, buff_len);
    return strlen(buff);
}

LOCAL const char *Fota_GetSourceIPString(void)
{
    return  inet_ntoa(sci_gethostaddress_ext(g_fota_linker.net_id));
}

LOCAL int Fota_GetSaveOperatorIndex(char *buff, int buff_len)
{
    uint32 io;

    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_FOTA_OPERATOR_IDX, &io) || io == (uint32)-1)
        return 0;

    sprintf(buff, "%d", io);

    return strlen(buff);
}

LOCAL int Fota_GetSaveVersion(char *buff, int buff_len)
{
    uint32 v;

    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_FOTA_VERSION, &v) || v == (uint32)-1 || v == 0)
    {
        SCI_TraceLow("[FOTA Update] get -- last version [0]");
        return 0;
    }
    SCI_TraceLow("[FOTA Update] get -- last version [%d]", v);

    sprintf(buff, "%d", v);

    return strlen(buff);
}

LOCAL const char * fota_rsa_key_buff = 
"-----BEGIN PUBLIC KEY-----\n\
MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDDq4PTKBt2fHLFZajPyBxnc7tR\n\
H2WBp4aYVOwmqNvj5gqBu4c/gS63Sc/B30MZmbGGWb6g27DAzLkmfgWfyrU8h4E2\n\
YjvYADSaQED3KDT2o/3wrAbMcGgjJabOwyKqRBOtVK1JEgoersq67kI4/hcJUUEP\n\
CviY9VELTfH5fWfqKwIDAQAB\n\
-----END PUBLIC KEY-----";

static int _fota_random(void *p_rng, unsigned char *output, size_t output_len)
{
    uint32_t rnglen = output_len;
    uint8_t   rngoffset = 0;

    while (rnglen > 0) {
        *(output + rngoffset) = (unsigned char)rand();
        rngoffset++;
        rnglen--;
    }
    return 0;
}


LOCAL BOOLEAN is_unreserved_char(uint8 c)
{
    if( (c>='0'&&c<='9') ||
        (c>='a'&&c<='z') ||
        (c>='A'&&c<='Z') ||
        c=='-' || c=='_' || c=='.' || c=='/'/*|| c=='!' || c=='~' || c=='*' || c=='\''  || c=='(' || c==')'*/) 
    {
        return TRUE;
    }
    return FALSE;
}

LOCAL uint32 encode_url(uint8 *dst_ptr, uint32 dst_len, const uint8 *url_ptr)
{
    uint8 c = 0;
    uint32 ret_len = 0;

    if(PNULL == dst_ptr || PNULL == url_ptr)
    {
        return 0;
    }

    while (0 != (c = *url_ptr++))
    {
        if (is_unreserved_char(c))
        {
            if (ret_len + 1 <= dst_len)
            {
                dst_ptr[ret_len ++] = c;
            }
            else
            {
                break;
            }
        }
        else if (' ' == c)
        {
            if (ret_len + 1 <= dst_len)
            {
                dst_ptr[ret_len ++] = '+';
            }
            else
            {
                break;
            }
        }
        else
        {
            char str[10] = {0};
            uint16 len = 0;

            sprintf(str, "%02X", c);
            len = strlen(str);

            if (ret_len + len <= dst_len)
            {
                dst_ptr[ret_len ++] = '%';

                SCI_MEMCPY(dst_ptr+ ret_len, str, len);

                ret_len += len;
            }
            else
            {
                break;
            }
        }
    }

    return ret_len;
}

LOCAL int Fota_RsaEncrypt(char *in, int in_len, char *out, int out_len)
{
    int ret, out_ret_len;
    mbedtls_pk_context _ctx;
    int _out_rsa_len=0;
    int _out_base64_len=0;
    char _out_rsa[512], _out_base64[512];

    SCI_MEMSET(_out_rsa, 0, 512);

    mbedtls_pk_init(&_ctx);
    ret = mbedtls_pk_parse_public_key(&_ctx, fota_rsa_key_buff,  strlen(fota_rsa_key_buff) + 1);

    SCI_TraceLow("[FOTA Update] RSA key : ret = %d (len = %d, type = %d, name = %s) ", ret,
		mbedtls_pk_get_len(&_ctx), 
		mbedtls_pk_get_type(&_ctx), 
		mbedtls_pk_get_name(&_ctx));

    if (ret == 0 && mbedtls_pk_can_do(&_ctx, MBEDTLS_PK_RSA))
    {
        SCI_TraceLow("[FOTA Update] mbedtls_pk_can_do  ");

        ret = mbedtls_pk_encrypt(&_ctx,
                            in, in_len,
                            _out_rsa, &_out_rsa_len, 512,
                            _fota_random, NULL); 
        if (ret != 0)
        {
            SCI_TraceLow("[FOTA Update] mbedtls_pk_encrypt : error( return = %d)", ret);
            return 0;
        }
        SCI_TraceLow("[FOTA Update] mbedtls_pk_encrypt : ret = %d (rsa_ret_len = %d)", ret, _out_rsa_len);

        ret = mbedtls_base64_encode(_out_base64, 512, &_out_base64_len, _out_rsa, _out_rsa_len);
        if (ret != 0)
        {
            SCI_TraceLow("[FOTA Update] mbedtls_base64_encode : error( return = %d)", ret);
            return 0;
        }
	
        out_ret_len = encode_url(out, out_len, _out_base64);
    }
    mbedtls_pk_free(&_ctx);

    SCI_TraceLow("[FOTA Update] RSA in : (%d)[%s] ", strlen(in), in);
    SCI_TraceLow("[FOTA Update] RSA out (base64): len = (%d) ", _out_base64_len);
    Fota_LongTrace(_out_base64, _out_base64_len);

    SCI_TraceLow("[FOTA Update] RSA out (url): len = (%d) ", strlen(out));
    Fota_LongTrace(out, strlen(out));

    return strlen(out) + 1;
}

LOCAL int Fota_Make_Login_Package(char *ptr, int ptr_len)
{
    char buff[128], prod_buff[32];
    SCI_DATE_T _date = {31, 12, 2004, 6};
    SCI_TIME_T _time = {59, 59, 23};
    int priv_key, len;
    char *ip_string;

    TM_GetSysDate(&_date);
    TM_GetSysTime(&_time);

    priv_key = ((_time.hour * 100 + _time.min) * 100 + _time.sec) / _date.mday;

    strcpy(prod_buff, "QUE");

    len = strlen(prod_buff);
    if (prod_buff[len - 1] == '\n')
        prod_buff[len - 1] = '\0';

    ip_string = Fota_GetSourceIPString();

    SCI_TraceLow("[FOTA Update] ip_source : [%s] ", ip_string);

    snprintf(buff, 128, "%d,%s,%s,%4d%02d%02d%02d%02d%04d", 
		priv_key, prod_buff, ip_string, 
		_date.year, _date.mon, _date.mday,
		_time.hour, _time.min, _time.sec * 1000);

    return Fota_RsaEncrypt(buff, strlen(buff), ptr, ptr_len);
}

LOCAL void Fota_MakeGetDataURL(char *ptr)
{
    int len;
    char buff[128];
    SCI_DATE_T _date = {31, 12, 2004, 6};
    SCI_TIME_T _time = {59, 59, 23};
    MN_PLMN_T sim_hplmn = {0};
    // url sever
    strcpy(ptr, FOTA_HTTP_URI_GETDATA);
    strcat(ptr, "?");
    // MCC
    MMIAPIPHONE_GetSimHPLMN(&sim_hplmn, FOTA_UPDATE_SIM);      
    SCI_MEMSET(buff, 0, 128);
    MMIAPICOM_Int2Str(sim_hplmn.mcc, buff, 128);
    strcat(ptr, FOTA_BODY_URL_MCC_HEAD);
    strcat(ptr, buff);
    strcat(ptr, FOTA_BODY_URL_AND);
    // MNC
    SCI_MEMSET(buff, 0, 128);
    snprintf(buff, 128, "%02d", sim_hplmn.mnc);
    strcat(ptr, FOTA_BODY_URL_MNC_HEAD);
    strcat(ptr, buff);
    strcat(ptr, FOTA_BODY_URL_AND);
    // IMEI
    SCI_MEMSET(buff, 0, 128);
    Fota_GetIMEI(buff, 128);
    strcat(ptr, FOTA_BODY_URL_IMEI_HEAD);
    strcat(ptr, buff);
    strcat(ptr, FOTA_BODY_URL_AND);
    // IMSI
    SCI_MEMSET(buff, 0, 128);
    Fota_GetIMSI(buff, 128);
    strcat(ptr, FOTA_BODY_URL_IMSI_HEAD);
    strcat(ptr, buff);
    strcat(ptr, FOTA_BODY_URL_AND);
    // source ip
    strcat(ptr, FOTA_BODY_URL_IP_HEAD);
    strcat(ptr, Fota_GetSourceIPString());
    strcat(ptr, FOTA_BODY_URL_AND);
    // Project
    SCI_MEMSET(buff, 0, 128);
    strcpy(buff, "QUE");

    len = strlen(buff);
    if (buff[len - 1] == '\n') buff[len - 1] = '\0';
    strcat(ptr, FOTA_BODY_URL_PROJ_HEAD);
    strcat(ptr, buff);
    strcat(ptr, FOTA_BODY_URL_AND);
    // Date Time
    TM_GetSysDate(&_date);
    TM_GetSysTime(&_time);
    SCI_MEMSET(buff, 0, 128);
    snprintf(buff, 128, "%4d%02d%02d%02d%02d%04d", 
        _date.year, _date.mon, _date.mday,
        _time.hour, _time.min, _time.sec * 1000);
    strcat(ptr, FOTA_BODY_URL_TIME_HEAD);
    strcat(ptr, buff);
    strcat(ptr, FOTA_BODY_URL_AND);
    // OS Version
    strcat(ptr, FOTA_BODY_URL_OSVER_HEAD);
    //strcat(ptr, "10.0.0.1");
    strcat(ptr, VERSION_GetInfo(PROJECT_VERSION));
    strcat(ptr, FOTA_BODY_URL_AND);
    // PCode
    strcat(ptr, FOTA_BODY_URL_PCODE_HEAD);
    SCI_MEMSET(buff, 0, 128);
    PRODUCT_CODE_Read(&buff);
    buff[11] = 0;
    strcat(ptr, buff);
    strcat(ptr, FOTA_BODY_URL_AND);

    // Version
    if (Fota_GetSaveVersion(buff, 128) == 0 || MMIFOTA_NVIsIMSIChanged())
    {
        SCI_TraceLow("[FOTA package ] get -- save version [ ERROR]");

        strcat(ptr, FOTA_BODY_URL_VER_HEAD);
        strcat(ptr, "1");
        strcat(ptr, FOTA_BODY_URL_AND);
    }
    else
    {
        SCI_TraceLow("[FOTA package ] get -- save version [%s] SUCC", buff);

        strcat(ptr, FOTA_BODY_URL_VER_HEAD);
        strcat(ptr, buff);
        strcat(ptr, FOTA_BODY_URL_AND);
    }

    // token
    strcat(ptr, FOTA_BODY_URL_TOKEN_HEAD);
    strcat(ptr, Fota_GetToken());
}
#endif
PUBLIC MMI_RESULT_E Fota_SendGetDataRequest(void)
{
#ifndef WIN32
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    char *url_ptr = NULL;
    HTTP_GET_PARAM_T* get_param_ptr= NULL;

    get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));
    if(get_param_ptr == NULL) return result;
    SCI_MEMSET(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));

    url_ptr = SCI_ALLOC_APPZ(FOTA_BODY_LEN);
    if(url_ptr == NULL) return result;
    SCI_MEMSET(url_ptr, 0, FOTA_BODY_LEN);

    Fota_MakeGetDataURL(url_ptr);

    get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
    get_param_ptr->need_net_prog_ind = FALSE;
    get_param_ptr->accept.accept_ptr = FOTA_ACCEPT_STRING;
    get_param_ptr->user_agent.user_agent_ptr = FOTA_USER_AGENT_STRING;
    get_param_ptr->uri.uri_ptr = url_ptr;
    get_param_ptr->get_data.is_header_use_file = FALSE;
    get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_UNKNOW;

    if(HTTP_SUCCESS == HTTP_GetRequest(g_fota_linker.http_ctx.context_id, get_param_ptr, MMI_MODULE_FOTA))
    {
        result = MMI_RESULT_TRUE;
    }

    SCI_TraceLow("[FOTA Update] Fota_SendGetDataRequest url : ");
    Fota_LongTrace(get_param_ptr->uri.uri_ptr, strlen (get_param_ptr->uri.uri_ptr));

    SCI_Free(get_param_ptr);
    SCI_Free(url_ptr);

    if (!result) 
    {
        GET_URL_RETURN(FOTA_GET_URL_REQ_ERR);
    }
    return result;
#endif
}
#ifndef WIN32
LOCAL void Fota_MakeLoginURL(char *ptr)
{
    int len;
    char buff[128];
    char pkg_buff[1024];

    // package
    SCI_MEMSET(pkg_buff, 0, 1024);
    len = Fota_Make_Login_Package(pkg_buff, 1024);
    SCI_TraceLow("[FOTA Update] package : (%d)[%s] ", len, pkg_buff);
    
    // url sever
    strcpy(ptr, FOTA_HTTP_URI_LOGIN);
    strcat(ptr, "?");

    // package
    strcat(ptr, FOTA_BODY_URL_PACKAGE_HEAD);
    ptr += strlen(ptr);
    SCI_MEMCPY(ptr, pkg_buff, len);
    ptr += len;
    *ptr = '\0';
}
#endif
PUBLIC MMI_RESULT_E Fota_SendLoginRequest(void)
{
#ifndef WIN32
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    char *url_ptr = NULL;
    HTTP_GET_PARAM_T* get_param_ptr= NULL;

    get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));
    if(get_param_ptr == NULL) return result;
    SCI_MEMSET(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));

    url_ptr = SCI_ALLOC_APPZ(FOTA_BODY_LEN);
    if(url_ptr == NULL) return result;
    SCI_MEMSET(url_ptr, 0, FOTA_BODY_LEN);

    Fota_MakeLoginURL(url_ptr);

    get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
    get_param_ptr->need_net_prog_ind = FALSE;
    get_param_ptr->accept.accept_ptr = FOTA_ACCEPT_STRING;
    get_param_ptr->user_agent.user_agent_ptr = FOTA_USER_AGENT_STRING;
    get_param_ptr->uri.uri_ptr = url_ptr;
    get_param_ptr->get_data.is_header_use_file = FALSE;
    get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_UNKNOW;

    if(HTTP_SUCCESS == HTTP_GetRequest(g_fota_linker.http_ctx.context_id, get_param_ptr, MMI_MODULE_FOTA))
    {
        result = MMI_RESULT_TRUE;
    }
    else 
    {
        CHECK_VERSION_RETURN(FOTA_LOGIN_REQ_ERR);
    }

    SCI_TraceLow("[FOTA Update] Fota_SendLoginRequest url : ");
    Fota_LongTrace(get_param_ptr->uri.uri_ptr, strlen (get_param_ptr->uri.uri_ptr));

    SCI_Free(get_param_ptr);
    SCI_Free(url_ptr);

    return result;
#endif
}

PUBLIC MMI_RESULT_E Fota_SendDownloadRequest(char *url, wchar *save_path, wchar *file_name)
{
#ifndef WIN32
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    HTTP_GET_PARAM_T* get_param_ptr= NULL;

    get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));
    if(get_param_ptr == NULL) return result;
    SCI_MEMSET(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));

    get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
    get_param_ptr->need_net_prog_ind = FALSE;
    get_param_ptr->accept.accept_ptr = FOTA_ACCEPT_STRING;
    get_param_ptr->user_agent.user_agent_ptr = FOTA_USER_AGENT_STRING;
    get_param_ptr->uri.uri_ptr = url;
    get_param_ptr->get_data.is_header_use_file = FALSE;
    get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_FILE;

    MMIAPICOM_Wstrcpy(get_param_ptr->get_data.content_file_name, save_path);
    MMIAPICOM_Wstrcat(get_param_ptr->get_data.content_file_name, file_name);

    if(!HTTP_GetRequest(g_fota_linker.http_ctx.context_id, get_param_ptr, MMI_MODULE_FOTA))
    {
        result = MMI_RESULT_TRUE;
    }

    SCI_TraceLow("[FOTA Update] Fota_SendDownloadRequest url : [%s]", get_param_ptr->uri.uri_ptr);

    SCI_Free(get_param_ptr);

    return result;
#endif
}

PUBLIC MMI_RESULT_E Fota_CloseRequest(void)
{
#ifndef WIN32
    MMI_RESULT_E        result = MMI_RESULT_FALSE;

    if(!HTTP_CloseRequest(g_fota_linker.http_ctx.context_id))
    {
        result = MMI_RESULT_TRUE;
    }	
    SCI_TraceLow("[FOTA Update] Fota_CloseRequest ");
    return result;
#endif
}

#endif

