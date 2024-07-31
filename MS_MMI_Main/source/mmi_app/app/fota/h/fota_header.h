#ifndef __FOTA_HEADER_H__
#define __FOTA_HEADER_H__

#ifdef FOTA_SUPPORT

typedef struct _FOTA_HTTP_CONTEXT
{
    HTTP_APP_MODULE_ID_T module_id;
    HTTP_CONTEXT_ID_T    context_id;
    HTTP_REQUEST_ID_T    request_id;
    HTTP_APP_INSTANCE_T  app_instance; 
} FOTA_HTTP_CONTEXT;

typedef struct _FOTA_LINKER
{
    BOOLEAN flags;
    MN_DUAL_SYS_E sim_sys;
    uint32 net_id;
    FOTA_HTTP_CONTEXT http_ctx;
    RESULT_FUNC s_check_version_cb;
    RESULT_FUNC s_get_url_cb;
    RESULT_FUNC s_down_package_cb;
} FOTA_LINKER;

typedef enum {
    FOTA_CMD_101,
    FOTA_CMD_103,
    FOTA_CMD_105,
    FOTA_CMD_MAX
} FOTA_CMD_ENUM;

typedef enum {
    FOTA_SM_NULL,
    FOTA_SM_CONNECT,
    FOTA_SM_INIT,
    FOTA_SM_REQUEST,
    FOTA_SM_GET_URL,
    FOTA_SM_PRE_DOWNLOAD,
    FOTA_SM_APN_DOWNLOAD,
    FOTA_SM_CNV_DOWNLOAD,
    FOTA_SM_IMS_DOWNLOAD,
    FOTA_SM_TOG_DOWNLOAD,
    FOTA_SM_UPDATING,
    FOTA_SM_DOWN_ERR,
    FOTA_SM_FINISH
}FOTA_SM_ENUM;

#define FOTA_UPDATE_SIM  0

extern FOTA_LINKER g_fota_linker;


#define CHECK_VERSION_RETURN(result)  if (g_fota_linker.s_check_version_cb != NULL ) g_fota_linker.s_check_version_cb(result)
#define GET_URL_RETURN(result)  if (g_fota_linker.s_get_url_cb != NULL ) g_fota_linker.s_get_url_cb(result)
#define DOWN_PACKAGE_RETURN(result)  if (g_fota_linker.s_down_package_cb != NULL ) g_fota_linker.s_down_package_cb(result)

PUBLIC MMI_RESULT_E Fota_MakeAndPostHttpRequest(FOTA_CMD_ENUM cmd);

PUBLIC void Fota_StartHttpRequest(void);

PUBLIC MMI_RESULT_E Fota_HandleHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

#endif // FOTA_SUPPORT

#endif // __FOTA_HEADER_H__
