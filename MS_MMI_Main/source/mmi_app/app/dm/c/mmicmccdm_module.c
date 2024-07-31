#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "mmk_app.h"

#include "version.h"
#include "mmipdp_export.h"  
//#include "in_message.h"
#include "mmipdp_internal.h"
#include "mmi_module.h"
#include "mmiphone_export.h"
#include "mmicmccdm_header.h"
#include "mmicmccdm_nv.h"

#include <stdio.h>
#include "mmk_timer.h"
#include "mmicmccdm_lwm2m.h"
#ifdef LWM2M_SUPPORT
#include "lwm2msdk.h"
#endif
#include "mn_type.h"
#include "mmiutil.h"
#include "mmicmccdm_main.h"
#include "mmicc_export.h"
#include "mmipb_export.h"
#include "mmipb_common.h"
#include "mmiconnection_export.h"

MMI_APPLICATION_T g_cmccdm_app = {0};
CMCCDM_LINKER g_cmccdm_linker = {0};
#if defined LWM2M_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define RETRY_TIME_INTERVAL 60000 //milliseconds
#define MAX_RETRY_TIMES  3


/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
uint8 s_register_timer;
uint8 s_retry_times;
BOOLEAN s_run_result = FALSE;
BOOLEAN s_in_register = FALSE;

/**--------------------------------------------------------------------------*
**                                                      *
**--------------------------------------------------------------------------*/
extern PUBLIC int MMICMCCDM_UEReportInfo(int resId, char **outbuff);
extern PUBLIC Options MMICMCCDM_DMCoapRegDate(void);
extern PUBLIC void MMICMCCDM_PostUEReportDate(void);
LOCAL void HandleCMCCDMRegPdpMsg(MMIPDP_CNF_INFO_T *msg_ptr);

#ifdef MMI_GPRS_SUPPORT

LOCAL void HandleCMCCDMRegPdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr || msg_ptr->app_handler != MMI_MODULE_CMCCDM)
    {
        return;
    }
    SCI_TraceLow("[CMCC DM] HandleCMCCDMRegPdpMsg -- msg = %d", msg_ptr->msg_id);

    switch(msg_ptr->msg_id) 
    {
        case MMIPDP_ACTIVE_CNF:     //PDP激活成功
            SCI_TraceLow("[CMCC DM] MMIPDP_ACTIVE_CNF--- result = %d", msg_ptr->result);
            if(MMIPDP_RESULT_SUCC == msg_ptr->result)
            {
              SCI_TraceLow("[CMCC DM] MMIPDP_ACTIVE_CNF--- net id  = %d", msg_ptr->nsapi);
              socket_SetNetId(msg_ptr->nsapi);
#ifdef LWM2M_SUPPORT
              CMCCDMReg_StartDMCoapRequest();
#endif
            }
            else
            {
                MMICMCCDM_ReTry(10);
            }
            break;

        case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功
            SCI_TraceLow("[CMCC DM] MMIPDP_DEACTIVE_CNF");
            g_cmccdm_linker.net_id = 0;
            break;

        case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活
            SCI_TraceLow("[CMCC DM] MMIPDP_DEACTIVE_IND");
            g_cmccdm_linker.net_id = 0;
            MMIAPIPDP_Deactive(MMI_MODULE_CMCCDM);
            break;	        

        default:
            break;
    }
}

#endif

PUBLIC BOOLEAN MMICMCCDM_ConnectPDP(void)
{
#if defined(MMI_GPRS_SUPPORT)
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    MMICONNECTION_APNTYPEINDEX_T *apn_idx_arr;
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;

    SCI_TraceLow("[CMCC DM] MMICMCCDM_ConnectPDP");

    if (FALSE == MMIAPIPHONE_GetDataServiceSIM(&g_cmccdm_linker.sim_sys))
    {
        SCI_TraceLow("[CMCC DM] MMICMCCDM_ConnectPDP [SIM ERROR]");
        return FALSE;
    }

    active_info.app_handler         = MMI_MODULE_CMCCDM;
    active_info.dual_sys            = g_cmccdm_linker.sim_sys;
    active_info.apn_ptr             = NULL;
    active_info.user_name_ptr       = NULL;
    active_info.psw_ptr             = NULL;
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_TD_PREFER;
    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
    active_info.handle_msg_callback = HandleCMCCDMRegPdpMsg;
    active_info.ps_service_type     = BROWSER_E;
    active_info.storage             = MN_GPRS_STORAGE_ALL;

    apn_idx_arr = MMIAPICONNECTION_GetApnTypeIndex(g_cmccdm_linker.sim_sys);
    if (apn_idx_arr == NULL)
    {
        SCI_TraceLow("[CMCC DM] MMICMCCDM_ConnectPDP [APN IDX ERROR]");
        return FALSE;
    }
    SCI_TraceLow("[CMCC DM] MMICMCCDM_ConnectPDP [sim = %d, Net Setting = %d]", g_cmccdm_linker.sim_sys, apn_idx_arr->internet_index[g_cmccdm_linker.sim_sys].index);
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(g_cmccdm_linker.sim_sys, apn_idx_arr->internet_index[g_cmccdm_linker.sim_sys].index);

    if(PNULL != linksetting && 0 != linksetting->apn_len)
    {
        active_info.apn_ptr = (char*)linksetting->apn;
        active_info.user_name_ptr = (char*)linksetting->username;
        active_info.psw_ptr = (char*)linksetting->password;
#ifdef IPVERSION_SUPPORT_V4_V6
        active_info.ip_type   = linksetting->ip_type;
#endif
        if(MMIAPIPDP_Active(&active_info))
        {
            return TRUE;
        }
    }
    SCI_TraceLow("[CMCC DM] MMICMCCDM_ConnectPDP [Net Setting ERROR]");
#endif
    return FALSE;
}

LOCAL MMI_RESULT_E CMCCDMReg_HandleModuleMsg(PWND app_ptr, uint16 msg_id,
        DPARAM param)
{
    SCI_TraceLow("[CUCC DM] CMCCDMReg_HandleModuleMsg msg id = %d", msg_id);
    switch (msg_id)
    {
        default:
            break;
    }
    return MMI_RESULT_FALSE;
}
LOCAL void  HandleLwM2MNotifyMsg(OptNotifyParam *optNotifyParam)
{
    if (optNotifyParam == NULL)
        return ;

    SCI_TRACE_LOW("[CMCC DM lwm2m]MMICMCCDM_LwM2MNotifyMsg:type=%0x,code=%d,msg=%s\n",
                  optNotifyParam->notify_type, optNotifyParam->notify_code,
                  optNotifyParam->notify_msg);
    switch (optNotifyParam->notify_type)
    {
        case NOTIFY_TYPE_SYS:
            if (s_run_result && NOTIFY_CODE_OK == optNotifyParam->notify_code)
            {
                MMICMCCDM_Update(5);     //Heartbeat registration cycle 5min(test)    1440min(release)
            }
            break;

        case NOTIFY_TYPE_REGISTER:
            if (NOTIFY_CODE_OK == optNotifyParam->notify_code)
            {
                s_run_result = TRUE;
            }
            else
            {
                s_run_result = FALSE;
                if (s_retry_times++ < MAX_RETRY_TIMES)
                {
                    MMICMCCDM_ReTry(60);
                }
                else
                {
                    s_retry_times = 0;
                    MMICMCCDM_ReTry(300);  //300s(test)  //3600s(release)
                }
            }
            break;

        case NOTIFY_TYPE_DEREGISTER://HeartBeat result
            s_run_result = FALSE;
            break;

        case NOTIFY_TYPE_REG_UPDATE:
            if (NOTIFY_CODE_OK == optNotifyParam->notify_code)
            {
                s_run_result = TRUE;
            }
            else
            {
                s_run_result = FALSE;
                MMICMCCDM_ReTry(300);   //300s(test)   //3600s(release)
            }
            break;

        default:
            break;
    }
}




LOCAL OptFuncs MMICMCCDM_InitLwM2MCallback(void)
{
    OptFuncs optfuncs = {NULL, NULL};
    optfuncs.NotifyMsg  = HandleLwM2MNotifyMsg;
    optfuncs.DMReadInfo = MMICMCCDM_UEReportInfo;

    return optfuncs;
}


PUBLIC void CMCCDMReg_StartDMCoapRequest(void)
{
    SCI_TRACE_LOW(" [CMCC DM ]SlfReg_StartDMCoapRequest ...");
    //MMISFR_ResetFlags();
    s_register_timer = 0;
    s_in_register = TRUE;
    s_retry_times = 0;
    s_run_result = FALSE;
    if (LWM2M_SDK_IS_RUN())
    {
        s_in_register = FALSE;
        LWM2M_SDK_UPDATE_REG();
    }
    else
    {
        Options opt = MMICMCCDM_DMCoapRegDate();
        OptFuncs optfuncs = MMICMCCDM_InitLwM2MCallback();
        int ret = LWM2M_SDK_INIT(&opt, &optfuncs);
        MMICMCCDM_PostUEReportDate();
    }
}


LOCAL void CMCCDMReg_HandlePdpEvent(MN_DUAL_SYS_E dual_sys, MMIPDP_NOTIFY_EVENT_E notify_event)
{
    {    if(g_cmccdm_linker.flags == FALSE)

        if(MMIPDP_NOTIFY_EVENT_ACTIVED == notify_event)
        {
            SCI_TraceLow("[CMCC DM] MMIPDP_NOTIFY_EVENT_ACTIVED");

            g_cmccdm_linker.flags = TRUE;
            g_cmccdm_linker.sim_sys = dual_sys;
        }
    }
}

LOCAL void HandleCMCCDMNotifyEventCallback(MN_DUAL_SYS_E dual_sys, MMIPHONE_NOTIFY_EVENT_E notify_event, uint32 param)
{
    SCI_TraceLow("[CMCC DM]: HandleCMCCDMNotifyEventCallback dual_sys=%d, notify_event=%d, param=%d", dual_sys, notify_event, param);

    //无网恢复有网时
    if(MMIPHONE_NOTIFY_EVENT_CS_AVAIL == notify_event)
    {
        
    }
}

PUBLIC void MMICMCCDM_DMInitModule(void)
{
    MMIPHONE_NOTIFY_INFO_T notify_info = {0};    
    MMIPDP_NOTIFY_EVENT_E pdp_event[MMIPDP_NOTIFY_EVENT_MAX] = {
        MMIPDP_NOTIFY_EVENT_ACTIVED,
        MMIPDP_NOTIFY_EVENT_DEACTIVED,
        MMIPDP_NOTIFY_EVENT_ATTACH,
        MMIPDP_NOTIFY_EVENT_DETTACH
    };

    SCI_TraceLow("[CMCC DM] MMICMCCDM_DMInitModule");

    MMIAPIPDP_SubscribeEvent(MMI_MODULE_CMCCDM, 
            CMCCDMReg_HandlePdpEvent,
            MMIPDP_NOTIFY_EVENT_MAX,
            (MMIPDP_NOTIFY_EVENT_E *)pdp_event);

    g_cmccdm_app.ProcessMsg = CMCCDMReg_HandleModuleMsg;
    
    MMICMCCDM_RegNv();

    notify_info.module_id = MMI_MODULE_CMCCDM;
    notify_info.notify_func = HandleCMCCDMNotifyEventCallback;

    MMIAPIPHONE_RegisterNotifyInfo(&notify_info);
}
#endif
