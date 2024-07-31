
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

#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#endif
#include "gui_ucs2b_converter.h"

#include "fota_api.h"
#include "fota_header.h"
#include "fota_nv.h"

MMI_APPLICATION_T g_fota_app = {0};
FOTA_LINKER g_fota_linker = {0};

PUBLIC void MMIFOTA_Check_Version(RESULT_FUNC call_back)
{
    g_fota_linker.s_check_version_cb = call_back;

    SCI_TraceLow("[FOTA Update] MMIFOTA_Check_Version sm = %d", Fota_SM_Get());

    Fota_RestAllUrl();

    if (Fota_SM_Get() == FOTA_SM_CONNECT)
    {
        Fota_StartHttpRequest();
    }
    else
    {
        CHECK_VERSION_RETURN(FOTA_STATUS_ERR);
    }
}

PUBLIC void MMIFOTA_Get_URL(RESULT_FUNC call_back)
{
    g_fota_linker.s_get_url_cb = call_back;

    if (Fota_SM_Get() == FOTA_SM_REQUEST)
    {
        Fota_SM_Set(FOTA_SM_GET_URL);
        Fota_SM_Process();
    }
    else
    {
        GET_URL_RETURN(FOTA_STATUS_ERR);
    }
}

PUBLIC void MMIFOTA_DownLoad_Package(RESULT_FUNC call_back)
{
    SCI_TraceLow("[FOTA Update] MMIFOTA_DownLoad_Package");

    g_fota_linker.s_down_package_cb = call_back;

    if (Fota_SM_Get() == FOTA_SM_PRE_DOWNLOAD)
    {
        if (Fota_GetApnUrl() != NULL)
        {
            SCI_TraceLow("[FOTA Update] MMIFOTA_DownLoad_Package --apn");
            Fota_SM_Set(FOTA_SM_APN_DOWNLOAD);
        }
        else if (Fota_GetCfgNvUrl() != NULL)
        {
            SCI_TraceLow("[FOTA Update] MMIFOTA_DownLoad_Package --cnv");
            Fota_SM_Set(FOTA_SM_CNV_DOWNLOAD);
        }
        else if (Fota_GetIMSUrl() != NULL)
        {
            SCI_TraceLow("[FOTA Update] MMIFOTA_DownLoad_Package --ims");
            Fota_SM_Set(FOTA_SM_IMS_DOWNLOAD);
        }
        else if (Fota_GetIMSToggleUrl() != NULL)
        {
            SCI_TraceLow("[FOTA Update] MMIFOTA_DownLoad_Package --tog");
            Fota_SM_Set(FOTA_SM_TOG_DOWNLOAD);
        }
        else 
        {
            SCI_TraceLow("[FOTA Update] MMIFOTA_DownLoad_Package --error");
            DOWN_PACKAGE_RETURN(FOTA_GET_APN_ERR);
            return;
        }
        Fota_SM_Process();
    }
    else
    {
        DOWN_PACKAGE_RETURN(FOTA_STATUS_ERR);
    }
}

PUBLIC void MMIFOTA_Update_Finish(void)
{
    SCI_TraceLow("[FOTA Update] MMIFOTA_Update_Finish");

    if (Fota_SM_Get() == FOTA_SM_UPDATING)
    {
        Fota_SM_Set(FOTA_SM_FINISH);
        Fota_SM_Process();
    }

    g_fota_linker.s_down_package_cb = NULL;
    g_fota_linker.s_down_package_cb = NULL;

    Fota_SM_Set(FOTA_SM_NULL);
    Fota_RestAllUrl();

    MMIAPIPDP_Deactive(MMI_MODULE_FOTA);
    SCI_TraceLow("[FOTA Update] MMIFOTA_Update_Finish :: MMIAPIPDP_Deactive :: MMI_MODULE_FOTA");
}

LOCAL void HandleFotaPdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    SCI_TraceLow("[FOTA Update] HandleFotaPdpMsg");
    if(PNULL == msg_ptr)
    {
        return;
    }

    switch(msg_ptr->msg_id) 
    {
        case MMIPDP_ACTIVE_CNF:     //PDP激活成功
            SCI_TraceLow("[FOTA Update] MMIPDP_ACTIVE_CNF");
            if(MMIPDP_RESULT_SUCC == msg_ptr->result)
            {
                g_fota_linker.net_id = msg_ptr->nsapi;
                //Fota_StartHttpRequest();
                Fota_SM_Set(FOTA_SM_CONNECT);
            }
#if defined(MMI_GPRS_SUPPORT) || defined(WIFI_SUPPORT)
            else
            {
                MMIAPIPDP_Deactive(MMI_MODULE_FOTA);
            }
#endif
            break;

        case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功
            SCI_TraceLow("[FOTA Update] MMIPDP_DEACTIVE_CNF");
            g_fota_linker.net_id = 0;
            break;

        case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活
            SCI_TraceLow("[FOTA Update] MMIPDP_DEACTIVE_CNF");
            g_fota_linker.net_id = 0;
#if defined(MMI_GPRS_SUPPORT) || defined(WIFI_SUPPORT)
            MMIAPIPDP_Deactive(MMI_MODULE_FOTA);
#endif
            break;	        

        default:
            break;
    }
}

BOOLEAN MMIFOTA_ConnectPDP(void)
{
#if defined(MMI_GPRS_SUPPORT) || defined(WIFI_SUPPORT)
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    MMICONNECTION_APNTYPEINDEX_T *apn_idx_arr;
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;

    SCI_TraceLow("[FOTA Update] MMIFOTA_ConnectPDP");

    if (FALSE == MMIAPIPHONE_GetDataServiceSIM(&g_fota_linker.sim_sys))
    {
        SCI_TraceLow("[FOTA Update] MMIFOTA_ConnectPDP [SIM ERROR]");
        return FALSE;
    }

    active_info.app_handler         = MMI_MODULE_FOTA;
    active_info.dual_sys            = g_fota_linker.sim_sys;
    active_info.apn_ptr             = NULL;
    active_info.user_name_ptr       = NULL;
    active_info.psw_ptr             = NULL;
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_UNSPECIFIED;
    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
    active_info.handle_msg_callback = HandleFotaPdpMsg;
    active_info.ps_service_type = BROWSER_E;
    active_info.storage = MN_GPRS_STORAGE_ALL;

    apn_idx_arr = MMIAPICONNECTION_GetApnTypeIndex(g_fota_linker.sim_sys);
    if (apn_idx_arr == NULL)
    {
        SCI_TraceLow("[FOTA Update] MMIFOTA_ConnectPDP [APN IDX ERROR]");
        return FALSE;
    }
    SCI_TraceLow("[FOTA Update] MMIFOTA_ConnectPDP [Net Setting = %d]", apn_idx_arr->internet_index[g_fota_linker.sim_sys].index);
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(g_fota_linker.sim_sys, apn_idx_arr->internet_index[g_fota_linker.sim_sys].index);

    if(PNULL != linksetting && 0 != linksetting->apn_len)
    {
        active_info.apn_ptr = (char*)linksetting->apn;
#ifdef IPVERSION_SUPPORT_V4_V6
        active_info.ip_type   = linksetting->ip_type;
#endif

        if(MMIAPIPDP_Active(&active_info))
        {
            return TRUE;
        }
    }
    SCI_TraceLow("[FOTA Update] MMIFOTA_ConnectPDP [Net Setting ERROR]");
#endif
    return FALSE;
}

LOCAL MMI_RESULT_E Fota_HandleModuleMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    if (HTTP_SIGNAL_BEG <= msg_id && msg_id <= HTTP_SIGNAL_END)
    {
        SCI_TraceLow("[FOTA Update] Fota_HandleModuleMsg msg_id = %d [HTTP_SIGNAL_BEG = %d]", msg_id, HTTP_SIGNAL_BEG);
        result = Fota_HandleHttpMsg(app_ptr,msg_id,param);		
    }
    return result;
}

LOCAL void Fota_HandlePdpEvent(MN_DUAL_SYS_E dual_sys, MMIPDP_NOTIFY_EVENT_E notify_event)
{
    if(g_fota_linker.flags == FALSE)
    {
        if(MMIPDP_NOTIFY_EVENT_ACTIVED == notify_event)
        {
            SCI_TraceLow("[FOTA Update] MMIPDP_NOTIFY_EVENT_ACTIVED");

            g_fota_linker.flags = TRUE;
            g_fota_linker.sim_sys = dual_sys;
        }
    }
}

LOCAL const MMIPDP_NOTIFY_EVENT_E s_fota_pdp_event[MMIPDP_NOTIFY_EVENT_MAX] = {
    MMIPDP_NOTIFY_EVENT_ACTIVED,
    MMIPDP_NOTIFY_EVENT_DEACTIVED,
    MMIPDP_NOTIFY_EVENT_ATTACH,
    MMIPDP_NOTIFY_EVENT_DETTACH
};

PUBLIC void MMIFOTA_InitModule(void)
{
    SCI_TraceLow("[FOTA Update] MMIFOTA_InitModule");

    MMIAPIPDP_SubscribeEvent(MMI_MODULE_FOTA, 
            Fota_HandlePdpEvent,
            MMIPDP_NOTIFY_EVENT_MAX,
            (MMIPDP_NOTIFY_EVENT_E *)s_fota_pdp_event);

    g_fota_app.ProcessMsg = Fota_HandleModuleMsg;

    MMIFOTA_RegNv();
}

#endif

