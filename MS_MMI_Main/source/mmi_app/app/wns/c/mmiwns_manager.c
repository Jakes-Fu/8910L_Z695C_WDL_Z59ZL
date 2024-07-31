/****************************************************************************
** File Name:      mmiwns_manager.c                                            *
** Author:         grant.peng                                              *
** Date:           24/07/2012                                               *
** Copyright:      2015 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the wns interface for java application.       *
*****************************************************************************
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 24/07/2012    grant.peng      Create
****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "version.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "mmiconnection_export.h"
#include "mmiphone_export.h"
#include "mmipdp_export.h"
#include "mmi_common.h"
#include "mmi_modu_main.h"
#include "mmiidle_export.h"
#include "mmibrowser_export.h"
#include "mmiphone_export.h"
#include "mmicc_export.h"
#include "mmienvset_export.h"
#include "mmiwns_export.h"
#include "mmiwns_nv.h"
#include "mmiwns_manager.h"

/**-------------------------------------------------------------------------*
**                         MICRO DEFINITION                                *
**--------------------------------------------------------------------------*/

#define WNS_CREATE_CHANNEL_TIMEOUT 10000
#define WNS_CONNECT_WAIT_TIMEOUT 30000
#define WNS_DISCONNECT_WAIT_TIMEOUT 30000

#define WNS_CONNECT_WAIT_INIT_TIMEOUT 3000
#define WNS_CONNECT_RETRY_INC_TIMEOUT 10000
#define WNS_CONNECT_RETRY_MAX_TIMEOUT 60000

#define WNS_PDP_ACTIVE_TIMEOUT       20000  
#define WNS_PDP_RETRY_TIMEOUT        3000 //重新尝试连接pdp，换卡连接

#define WNS_NOTIFICATION_RING_DUATION     600  
#if 0
#define WNS_NOTIFICATION_RING_ID  R_GENERIC_ALERT //(8 + MMI_SMS_RING_START_ID - 1)
#else
#define WNS_NOTIFICATION_RING_ID (MMI_WNS_RING_START_ID)
#endif

//      char time[];
//      follow WNP document, the format is UTC time
//      "YYYY-MM-DDTHH:MM:SSZ", 
//      like "2013-02-07T02:56:32Z"
#define TIME_MIN_LEN 20

#ifdef WIN32
#define OS_VERSION     "MOCOR_13A_MS_W15.32.1_Release"
#endif //WIN32



/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/


typedef struct  pns_mgr_app_list_tag
{
    BLOCK_ID wns_task_id;
    uint8 available_channel_num;
    uint8 is_wns_registered;
    uint8   create_channel_timer_id[WNS_APP_MAX_NUM];
    uint32 net_id;
    uint8   pdp_timer_id;
    uint8   pdp_retry_timer_id;
    uint8   wns_connect_timer_id;
    uint8   wns_connect_retry_count;    
    uint32 wns_connect_retry_time_out;  
    uint8 is_connect_by_app;
    uint8 is_create_channel_ongoing;
    uint16 create_channel_app_index;
    MN_DUAL_SYS_E         cur_sys_num;
    NET_CONNECT_STATUS_E net_connect_status;
    WNS_CONNECT_STATUS_E wns_connect_status;
    get_push_notification_channel_cb get_channel_info_func_cb;
    PNS_MGR_APP_INFO_T *pns_mgr_app_info[WNS_APP_MAX_NUM];
}PNS_MGR_APP_LIST_T;

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
MMI_APPLICATION_T    g_wns_app = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
static PNS_MGR_APP_LIST_T s_pns_mgr_app_list = {0};

/*****************************************************************************/
//  Description : handle socket connect timeout
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL void HandleWnsPdpActiveTimer(
                                            uint8 timer_id,
                                            uint32 param
                                            );

LOCAL void HandleWnsPdpMsg(MMIPDP_CNF_INFO_T *msg_ptr);

LOCAL void HandleWnsPdpRetryTimer(
                                uint8 timer_id,
                                uint32 param
                                );

LOCAL void HandleWnsCreateChannelTimer(
                                            uint8 timer_id,
                                            uint32 param
                                            );

/*****************************************************************************/
//     Description : tell java app create channel failed by callback function
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
LOCAL void CreateChannelFailed(char* appname, char* appkey);

/*****************************************************************************/
//  Discription: Stop wns connect timer
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL void StopWnsConnectTimer(void);

/*****************************************************************************/
//  Discription: Init wns connect timer
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL void InitWnsConnectTimer(void);

/*****************************************************************************/
//  Discription: Start wns connect timer
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL void StartWnsConnectRetryTimer(void);

/*****************************************************************************/
//  Description : handle wns connect timeout
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL void HandleWnsConnectTimer(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : test code
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL void MMIWNS_TestPushNotification(void);

/*****************************************************************************/
//     Description : free app info
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
LOCAL void FreePnsMgrAppInfo(
                            PNS_MGR_APP_INFO_T *app_ptr
                            );

/*****************************************************************************/
//     Description : stop wns service at poweroff
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
LOCAL void MMIWNS_Stop(BOOLEAN is_force_stop);

/*****************************************************************************/
//     Description : start wns 
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
LOCAL void MMIWNS_Start(void);

/*****************************************************************************/
//     Description : to set the available channel number to NV
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIWNS_SaveAppInfo(
                            PNS_MGR_APP_INFO_T *app_ptr
                            );


/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/
PUBLIC BOOLEAN MMIWNS_IsRegistered(void)
{
    return s_pns_mgr_app_list.is_wns_registered;
}

PUBLIC void MMIWNS_SetRegistered(BOOLEAN is_wns_register)
{
    s_pns_mgr_app_list.is_wns_registered = (uint16)is_wns_register;
}

PUBLIC uint32 MMIWNS_GetNetId(void)
{
    return s_pns_mgr_app_list.net_id;
}


PUBLIC void MMIWNS_SetNetId(uint32 net_id)
{
    s_pns_mgr_app_list.net_id = net_id;
}

PUBLIC uint16 MMIWNS_GetCreateChannelOngoing(void)
{
    return s_pns_mgr_app_list.is_create_channel_ongoing;
}


PUBLIC void MMIWNS_SetCreateChannelOngoing(uint16 is_create_channel_ongoing)
{
    s_pns_mgr_app_list.is_create_channel_ongoing = is_create_channel_ongoing;
}


PUBLIC uint16 MMIWNS_GetIsConnectByApp(void)
{
    return s_pns_mgr_app_list.is_connect_by_app;
}


PUBLIC void MMIWNS_SetIsConnectByApp(uint16 is_connect_by_app)
{
    s_pns_mgr_app_list.is_connect_by_app = is_connect_by_app;
}


PUBLIC uint16 MMIWNS_GetCreateChannelAppIndex(void)
{
    return s_pns_mgr_app_list.create_channel_app_index;
}

PUBLIC void MMIWNS_SetCreateChannelAppIndex(uint16 create_channel_app_index)
{
    s_pns_mgr_app_list.create_channel_app_index = create_channel_app_index;
}

PUBLIC WNS_CONNECT_STATUS_E MMIWNS_GetNetConnStatus(void)
{
    return s_pns_mgr_app_list.net_connect_status;
}

PUBLIC void MMIWNS_SetNetConnStatus(NET_CONNECT_STATUS_E net_connect_status)
{
    s_pns_mgr_app_list.net_connect_status = net_connect_status;
}


PUBLIC WNS_CONNECT_STATUS_E MMIWNS_GetWnsConnStatus(void)
{
    return s_pns_mgr_app_list.wns_connect_status;
}

PUBLIC void MMIWNS_SetWnsConnStatus(WNS_CONNECT_STATUS_E wns_connect_status)
{
    s_pns_mgr_app_list.wns_connect_status = wns_connect_status;
}



/*****************************************************************************/
//     Description : get the available channel number 
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIWNS_GetAvailableChnNum(void)
{
    return s_pns_mgr_app_list.available_channel_num;
}

/*****************************************************************************/
//     Description : to set the available channel number to NV
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIWNS_SetAvailableChnNum(
                                  uint16 available_channel_num
                                  )
{
    MMI_WriteNVItem(MMIWNS_NV_AVALAIBLE_CHANNEL_NUM, &available_channel_num);    
    
    s_pns_mgr_app_list.available_channel_num = available_channel_num;
}


PUBLIC BLOCK_ID MMIWNS_GetTaskId(void)
{
    if(0 == s_pns_mgr_app_list.wns_task_id)
    {
#ifdef MMIWNS_TEST_SUPPORT
        s_pns_mgr_app_list.wns_task_id = P_APP;
#else
        //s_pns_mgr_app_list.wns_task_id = MMIWNS_Init();
#endif
    }

    return s_pns_mgr_app_list.wns_task_id;
}

LOCAL void MMIWNS_SetTaskId(BLOCK_ID wns_task_id)
{
    s_pns_mgr_app_list.wns_task_id = wns_task_id;
}


/*****************************************************************************/
//  Description : Send msg to self
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWNS_SendMsgToSelf(uint32 sig_id)
{
    MmiSignalS *signal = PNULL;
    
    SCI_TRACE_LOW("[wns]:MMIWNS_SendMsgToSelf---sig_id = 0x%04x ---");

    //creat signal
    MMI_CREATE_SIGNAL(signal,(uint16)sig_id, sizeof(MmiSignalS), P_APP);

    //send signal
    MMI_SEND_SIGNAL(signal, P_APP);
}


/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StartWnsPdpActiveTimer(void)
{
    SCI_TRACE_LOW("[wns]:StartWnsPdpActiveTimer(), enter.");

    if(0 != s_pns_mgr_app_list.pdp_timer_id)
    {
        MMK_StopTimer(s_pns_mgr_app_list.pdp_timer_id);
        s_pns_mgr_app_list.pdp_timer_id = 0;
    }
    
    s_pns_mgr_app_list.pdp_timer_id = MMK_CreateTimerCallback(WNS_PDP_ACTIVE_TIMEOUT, 
                            HandleWnsPdpActiveTimer, 
                            PNULL, 
                            FALSE);
}

/*****************************************************************************/
//  Discription: Stop socket connect timer
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL void StopWnsPdpActiveTimer(void)
{
    SCI_TRACE_LOW("[wns]:StopWnsPdpActiveTimer");

    if(0 != s_pns_mgr_app_list.pdp_timer_id)
    {
        MMK_StopTimer(s_pns_mgr_app_list.pdp_timer_id);
        s_pns_mgr_app_list.pdp_timer_id = 0;
    }
}


/*****************************************************************************/
//  Discription: Wns get apn str 
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL MMICONNECTION_LINKSETTING_DETAIL_T* GetWnsLinkSetting(MN_DUAL_SYS_E dual_sys)
{
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    char*   apn_str = PNULL;
    uint8   index   = 0;

#ifdef BROWSER_SUPPORT
    index = MMIAPIBROWSER_GetNetSettingIndex(dual_sys);               
    SCI_TRACE_LOW("[wns]:GetWnsLinkSetting(): net setting index(%d).", index);
#endif

    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);

    return linksetting;
}

/*****************************************************************************/
//  Discription: Wns get apn str 
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL char* GetWnsApnStr(MMICONNECTION_LINKSETTING_DETAIL_T* linksetting)
{
    char*   apn_str = PNULL;

    if(PNULL != linksetting && 0 != linksetting->apn_len)
    {
        apn_str = (char*)linksetting->apn;
    }

    return apn_str;
    
}

/*****************************************************************************/
//  Discription: Wns get apn user name 
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL char* GetWnsUserNameStr(MMICONNECTION_LINKSETTING_DETAIL_T* linksetting)
{
    char*   user_name_str = PNULL;

    if(PNULL != linksetting && 0 != linksetting->username_len)
    {
        user_name_str = (char*)linksetting->username;
    }
    
    return user_name_str;
}

/*****************************************************************************/
//  Discription: Get apn password 
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL char* GetWnsPasswordStr(MMICONNECTION_LINKSETTING_DETAIL_T* linksetting)
{
    char*   password_str = PNULL;

    if(PNULL != linksetting && 0 != linksetting->password_len)
    {
        password_str = (char*)linksetting->password;
    }
    
    return password_str;
}

/*****************************************************************************/
//  Description : is pdp permit using,G ner and in phone can not use pdp
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIWNS_IsPdpPermitUsing(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_permit_used = FALSE;
    MN_PHONE_PLMN_STATUS_E net_status = PLMN_NO_SERVICE;
    net_status = MMIAPIPHONE_GetPlmnStatus(dual_sys);
    SCI_TRACE_LOW("[wns]:MMIWNS_IsPdpPermitUsing net_status = %d", net_status);

    switch(net_status)
    {
        case PLMN_NORMAL_GPRS_ONLY:
        //case PLMN_EMERGENCY_GPRS_ONLY:
        case PLMN_FULL_PS_SERVICE:
                is_permit_used = TRUE;
                break;
        case PLMN_NORMAL_GSM_GPRS_BOTH:
        case PLMN_NORMAL_GSM_ONLY:
                is_permit_used = TRUE;
                if (MMI_GMMREG_RAT_GPRS == MMIAPIPHONE_GetTDOrGsm(dual_sys) 
                    && MMIAPICC_IsInState(CC_IN_CALL_STATE))
                {
                    is_permit_used = FALSE;
                }
                break;
        default:
                is_permit_used = FALSE;
                break;
    }

    return is_permit_used;
}


/*****************************************************************************/
//  Description : pdp retry if it can't active
//  Global resource dependence : 
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WnsPDPTry(MN_DUAL_SYS_E sim_sys)
{
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    BOOLEAN                 return_val      = FALSE;
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = GetWnsLinkSetting(sim_sys);

    active_info.app_handler         = MMI_MODULE_WNS;
    active_info.dual_sys            = sim_sys;
    active_info.apn_ptr             = GetWnsApnStr(linksetting);
    active_info.user_name_ptr       = GetWnsUserNameStr(linksetting);
    active_info.psw_ptr             = GetWnsPasswordStr(linksetting);
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_UNSPECIFIED;
    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
    active_info.handle_msg_callback = HandleWnsPdpMsg;
    active_info.ps_service_type = BROWSER_E;
    active_info.storage = MN_GPRS_STORAGE_ALL;
#ifdef IPVERSION_SUPPORT_V4_V6
    active_info.ip_type   = linksetting->ip_type;
#endif

    if(MMIAPIPDP_Active(&active_info))
    {
        return_val = TRUE;
    }
    SCI_TRACE_LOW("[wns]:WnsPDPTry return_val = %d",return_val);
    return return_val;
}


/*****************************************************************************/
//  Description : Pdp Active, 
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWNS_PdpActive(void)
{
    BOOLEAN                 return_val      = FALSE;
    MN_DUAL_SYS_E           dual_sys        = MN_DUAL_SYS_MAX;
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};

    SCI_TRACE_LOW("[wns]: MMIWNS_PdpActive(): enter.");


#ifdef WIFI_SUPPORT    
    //优先wifi
    if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
    {
        active_info.app_handler         = MMI_MODULE_WNS;
        active_info.handle_msg_callback = HandleWnsPdpMsg;
        active_info.ps_interface        = MMIPDP_INTERFACE_WIFI;
        if(MMIAPIPDP_Active(&active_info))
        {
            StartWnsPdpActiveTimer();
            return_val = TRUE;
        }
        else
        {
            MMIWNS_SendMsgToSelf(MSG_PNS_NET_CONN_FAILED_CNF);
        }
    }
    else
#endif //WIFI_SUPPORT
    {
        //uint32 sim_ok_num = 0;
        uint16 sim_ok = MN_DUAL_SYS_1;
        BOOLEAN b_ret = FALSE;

        s_pns_mgr_app_list.cur_sys_num   = MN_DUAL_SYS_1;
        //sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);/*lint !e64*/
        b_ret = MMIAPIPHONE_GetDataServiceSIM((MN_DUAL_SYS_E *)(&sim_ok));
        s_pns_mgr_app_list.cur_sys_num = (MN_DUAL_SYS_E)sim_ok;
        SCI_TRACE_LOW("[wns]:b_ret = %d,sim_ok = %d", b_ret, sim_ok);
        
        if(b_ret && MMIWNS_IsPdpPermitUsing(s_pns_mgr_app_list.cur_sys_num))
        {
            return_val = WnsPDPTry(s_pns_mgr_app_list.cur_sys_num);//第一张有效卡

            if(FALSE == return_val)
            {
                //去掉wns换sim卡PDP重连的机制
                /*
                if(0 != s_pns_mgr_app_list.pdp_retry_timer_id)
                {
                    MMK_StopTimer(s_pns_mgr_app_list.pdp_retry_timer_id);
                    s_pns_mgr_app_list.pdp_retry_timer_id = 0;
                }
                s_pns_mgr_app_list.pdp_retry_timer_id = MMK_CreateTimerCallback(
                                                                    WNS_PDP_RETRY_TIMEOUT, 
                                                                    HandleWnsPdpRetryTimer, 
                                                                    PNULL, 
                                                                    FALSE
                                                                    );
                */
                MMIWNS_SendMsgToSelf(MSG_PNS_NET_CONN_FAILED_CNF);
            }
        }
        else
        {
            MMIWNS_SendMsgToSelf(MSG_PNS_NET_CONN_FAILED_CNF);
        }
    
    }

    return return_val;

}

/*****************************************************************************/
//  Description : Wns deactive Pdp
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWNS_PdpDeactive(void)
{
    BOOLEAN return_val = FALSE;
    
    SCI_TRACE_LOW("[wns]: MMIWNS_PdpDeactive(): enter.");
    
    return_val = MMIAPIPDP_Deactive(MMI_MODULE_WNS);

    return return_val;
}



PUBLIC MSG_WNS_STATUS_E MMIWNS_RegService(void)
{
    WNS_REG_SIGNAL_T *sendSignal = PNULL;
    BLOCK_ID wns_task_id = MMIWNS_Init();
    MSG_WNS_STATUS_E ret = WNS_STATUS_FAILED;
    SCI_TRACE_LOW("[wns]:MMIWNS_RegService(): enter, wns_task_id(%d).", wns_task_id);

    if(0 != wns_task_id)
    {
        MMIWNS_SetTaskId(wns_task_id);
        //creat signal
        MMI_CREATE_SIGNAL(sendSignal, 
                                                (uint16)MSG_WNS_REG_WNS, 
                                                sizeof(WNS_REG_SIGNAL_T), 
                                                P_APP);  
        
        sendSignal->task_id = P_APP;
        
        //send signal
        MMI_SEND_SIGNAL(sendSignal, wns_task_id);

        ret = WNS_STATUS_SUCCESS;
    }

    return ret;
}


PUBLIC MSG_WNS_STATUS_E MMIWNS_ConnectService(void)
{
    MSG_WNS_STATUS_E ret = WNS_STATUS_FAILED;
    WNS_CONNECT_SIGNAL_T *sendSignal = PNULL;
    BLOCK_ID wns_task_id = MMIWNS_GetTaskId();
    char *os_version = PNULL;
    uint16 version_len =0;

    SCI_TRACE_LOW("[wns]: MMIWNS_ConnectService(): enter.");

    if(!MMIWNS_IsRegistered())
    {
        SCI_TRACE_LOW("[wns]: MMIWNS_ConnectService(): wns is not register.");
        return WNS_STATUS_NOTREGISTERED;
    }

    StopWnsConnectTimer();

    if(NET_DISCONNECTED == MMIWNS_GetNetConnStatus())
    {
        BOOLEAN b_ret = MMIWNS_PdpActive();
        if(!b_ret)
        {
            SCI_TRACE_LOW("[wns]: MMIWNS_ConnectService(): MMIWNS_PdpActive faild.");
            InitWnsConnectTimer();
        }
        else
        {
            MMIWNS_SetNetConnStatus(NET_CONNECTTING);
        }
    }
    else if(NET_CONNECTTING == MMIWNS_GetNetConnStatus())
    {
        //InitWnsConnectTimer();
    }
    else if(NET_CONNECTED == MMIWNS_GetNetConnStatus())
    {
        if(0 != wns_task_id)
        {
            //creat signal
            MMI_CREATE_SIGNAL(sendSignal, 
                                                    (uint16)MSG_WNS_CONNECT_WNS, 
                                                    sizeof(WNS_CONNECT_SIGNAL_T), 
                                                    P_APP);        

            sendSignal->net_id = MMIWNS_GetNetId();
            MMIAPICOM_GetOsName(sendSignal->agent_info.agent_os, AGENT_OS_MAX_LEN);
#ifdef WIN32
            os_version = OS_VERSION;
#else
            os_version = COMMON_GetVersionInfo(PLATFORM_VERSION);
#endif //WIN32
            version_len = strlen(os_version);
            version_len = MIN(version_len, AGENT_OS_VERSION_MAX_LEN);
            strncpy(sendSignal->agent_info.agent_os_version, os_version, version_len);
            MMIAPICOM_GetProcessorModel(sendSignal->agent_info.agent_processor, AGENT_PROCESSOR_MAX_LEN);
            MMIAPICOM_GetLanguagePackID(sendSignal->agent_info.agent_locale_id, AGENT_LOCALE_ID_MAX_LEN);

            //send signal
            MMI_SEND_SIGNAL(sendSignal, wns_task_id);

            MMIWNS_SetWnsConnStatus(WNS_CONNECTTING);
            s_pns_mgr_app_list.wns_connect_retry_count++;

            ret = WNS_STATUS_SUCCESS;
        }
    }

    return ret;
}

LOCAL PNS_MGR_APP_INFO_T *GetExistPnsMgrAppInfo(const char *app_key)
{
    int i = 0;
    
    for(i = 0; i < WNS_APP_MAX_NUM; i++)
    {
        if(PNULL != s_pns_mgr_app_list.pns_mgr_app_info[i])
        {
                if(0 == strcmp(s_pns_mgr_app_list.pns_mgr_app_info[i]->app_key, app_key))
                {
                    return s_pns_mgr_app_list.pns_mgr_app_info[i];
                }
        }
    }

    return PNULL;
}

LOCAL PNS_MGR_APP_INFO_T *GetCurCreateChannelAppInfo(void)
{
    uint16 app_index = MMIWNS_GetCreateChannelAppIndex();

    if(app_index < WNS_APP_MAX_NUM && MMIWNS_GetCreateChannelOngoing())    
    {
        if(PNULL != s_pns_mgr_app_list.pns_mgr_app_info[app_index]
           && WNS_CHANNEL_STATUS_CREATE_ONGOING
            == s_pns_mgr_app_list.pns_mgr_app_info[app_index]->channel_status)
        {
            return s_pns_mgr_app_list.pns_mgr_app_info[app_index];
        }
    }
    
    return PNULL;
}



LOCAL PNS_MGR_APP_INFO_T *GetNewPnsMgrAppInfo(void)
{

    PNS_MGR_APP_INFO_T *tmp_pns_mgr_app_info_ptr = PNULL;
    uint32 app_info_size = sizeof(PNS_MGR_APP_INFO_T) - sizeof(WNS_PUSH_NOTIFICATION_INFO_T);
    int i = 0;
    
    SCI_TRACE_LOW("[wns]:GetNewPnsMgrAppInfo(): enter");
    
    for(i = 0; i < WNS_APP_MAX_NUM; i++)
    {
        if(PNULL == s_pns_mgr_app_list.pns_mgr_app_info[i])
        {
            SCI_TRACE_LOW("[wns]:GetNewPnsMgrAppInfo(): app info size(%d).", app_info_size);
            tmp_pns_mgr_app_info_ptr = (PNS_MGR_APP_INFO_T *)SCI_ALLOC_APP(app_info_size);
            if(PNULL == tmp_pns_mgr_app_info_ptr)
            {
                SCI_TRACE_LOW("[wns]:GetNewPnsMgrAppInfo(): SCI_ALLOC_APP failed.");
                break;
            }
            SCI_MEMSET(tmp_pns_mgr_app_info_ptr, 0x0, app_info_size);
            s_pns_mgr_app_list.pns_mgr_app_info[i] = tmp_pns_mgr_app_info_ptr;
            tmp_pns_mgr_app_info_ptr->app_index = i;
            break;
        }
    }

    if(WNS_APP_MAX_NUM == i)
    {
        SCI_TRACE_LOW("[wns]:GetNewPnsMgrAppInfo(): the app list is full.");
    }

    return tmp_pns_mgr_app_info_ptr;
}

/*****************************************************************************/
//  Discription: Stop create channel timer
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL void StopWnsCreateChannelTimer(uint16 app_index)
{
    if(0 != s_pns_mgr_app_list.create_channel_timer_id[app_index])
    {
        MMK_StopTimer(s_pns_mgr_app_list.create_channel_timer_id[app_index]);
        s_pns_mgr_app_list.create_channel_timer_id[app_index] = 0;
    }
}

/*****************************************************************************/
//  Discription: Start create channel timer
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL void StartWnsCreateChannelTimer(PNS_MGR_APP_INFO_T *tmp_ptr)
{
    SCI_TRACE_LOW("[wns]:StartWnsCreateChannelTimer enter");

    if(PNULL != tmp_ptr)
    {
        StopWnsCreateChannelTimer(tmp_ptr->app_index);
        
        s_pns_mgr_app_list.create_channel_timer_id[tmp_ptr->app_index] = MMK_CreateTimerCallback(
                            WNS_CREATE_CHANNEL_TIMEOUT, 
                            HandleWnsCreateChannelTimer, 
                            tmp_ptr, 
                            FALSE);
    }
}


/*****************************************************************************/
//  Discription: Stop wns connect timer
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL void StopWnsConnectTimer(void)
{
    if(0 != s_pns_mgr_app_list.wns_connect_timer_id)
    {
        MMK_StopTimer(s_pns_mgr_app_list.wns_connect_timer_id);
        s_pns_mgr_app_list.wns_connect_timer_id = 0;
    }
}

/*****************************************************************************/
//  Discription: Init wns connect timer
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL void InitWnsConnectTimer(void)
{
    //SCI_TRACE_LOW("[wns]:InitWnsConnectTimer enter");

    StopWnsConnectTimer();

    if(0 != MMIWNS_GetAvailableChnNum())
    {
        s_pns_mgr_app_list.wns_connect_retry_time_out = WNS_CONNECT_WAIT_INIT_TIMEOUT;
        s_pns_mgr_app_list.wns_connect_retry_count = 0;
        s_pns_mgr_app_list.wns_connect_timer_id = MMK_CreateTimerCallback(
                             s_pns_mgr_app_list.wns_connect_retry_time_out, 
                            HandleWnsConnectTimer, 
                            PNULL, 
                            FALSE);

    }
    else
    {
        SCI_TRACE_LOW("[wns]:InitWnsConnectTimer(): there is no available channel, so needn't connect.");
    }
}



/*****************************************************************************/
//  Discription: Start wns connect  retry timer
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL void StartWnsConnectRetryTimer(void)
{
    SCI_TRACE_LOW("[wns]:StartWnsConnectRetryTimer enter");

    StopWnsConnectTimer();

    s_pns_mgr_app_list.wns_connect_retry_time_out += 
            (s_pns_mgr_app_list.wns_connect_retry_count * WNS_CONNECT_RETRY_INC_TIMEOUT);
    if(WNS_CONNECT_RETRY_MAX_TIMEOUT < s_pns_mgr_app_list.wns_connect_retry_time_out)
    {
        s_pns_mgr_app_list.wns_connect_retry_time_out = WNS_CONNECT_RETRY_MAX_TIMEOUT;
    }

    SCI_TRACE_LOW("[wns]:StartWnsConnectRetryTimer timeout(%ud)", 
                                    s_pns_mgr_app_list.wns_connect_retry_time_out);

    if(0 != MMIWNS_GetAvailableChnNum())
    {
        s_pns_mgr_app_list.wns_connect_timer_id = MMK_CreateTimerCallback(
                            s_pns_mgr_app_list.wns_connect_retry_time_out, 
                            HandleWnsConnectTimer, 
                            PNULL, 
                            FALSE);
    }
    else
    {
        SCI_TRACE_LOW("[wns]:StartWnsConnectRetryTimer(): there is no available channel, so needn't connect.");
    }

}


/*****************************************************************************/
//     Description : tell java app create channel failed by callback function
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
LOCAL void CreateChannelFailed(char* appname, char* appkey)
{
    PNS_MGR_APP_INFO_T *app_info_ptr = PNULL;

    SCI_TRACE_LOW("[wns]: CreateChannelFailed(): Enter.");

    if(PNULL != s_pns_mgr_app_list.get_channel_info_func_cb && PNULL != appname && PNULL != appkey)
    {
        SCI_TRACE_LOW("[wns]: CreateChannelFailed(): appname(%s), appkey(%s).", appname, appkey);
        s_pns_mgr_app_list.get_channel_info_func_cb(appname, appkey, PNULL);
    }

    app_info_ptr = MMIWNS_GetAppInfoByAppName(appname);

    FreePnsMgrAppInfo(app_info_ptr);
   
    MMIWNS_SetIsConnectByApp(FALSE);
    MMIWNS_SetCreateChannelOngoing(FALSE);

}

/*****************************************************************************/
//     Description : change time fomat from TM to char[], 
//                            follow WNP document, the format is UTC time
//                            "YYYY-MM-DDTHH:MM:SSZ", 
//                            like "2013-02-07T02:56:32Z"
//                           tm->UTC     year need plus 1900, month need plus 1, 
//                           this function is considered as asctime() in <time.h>
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN ChangeTimeFomat(char *time_buf, uint16 buf_len, MSG_WNS_TM tm)
{
    BOOLEAN b_ret = FALSE;
    SCI_TRACE_LOW("[wns]: ChangeTimeFomat(): Enter.");
    /* 
    struct tm 
    {  
        int tm_sec;
        int tm_min;
        int tm_hour;
        int tm_mday;  
        int tm_mon;
        int tm_year;  
        int tm_wday;  
        int tm_yday;  
        int tm_isdst;  
        long int tm_gmtoff;
        const char *tm_zone;  
    };
    */
    if(PNULL != time_buf && buf_len > TIME_MIN_LEN)
    {
        SCI_MEMSET(time_buf, 0, buf_len);
        sprintf(time_buf, "%04d-%02d-%02dT%02d:%02d:%02dZ", 
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

        SCI_TRACE_LOW("[wns]: ChangeTimeFomat(): time_buf(%s).", time_buf);

        b_ret = TRUE;
    }

    return b_ret;

}

/*****************************************************************************/
//  Discription: Print the infomation about pns application manager
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL void PrintPnsMgrAppInfo(PNS_MGR_APP_INFO_T *app_info_ptr)
{
    int i = 0;
    SCI_TRACE_LOW("[wns]:PrintPnsMgrAppInfo(): enter===========================");

    if(PNULL != app_info_ptr)
    {
        SCI_TRACE_LOW("[wns]: appname = (%s).", app_info_ptr->appname);
        SCI_TRACE_LOW("[wns]: appkey = (%s).", app_info_ptr->app_key);

        SCI_TRACE_LOW("[wns]: channel_id = (%s).", app_info_ptr->channel_id);
        SCI_TRACE_LOW("[wns]: channel_uri = (%s).", app_info_ptr->channel_uri);
        SCI_TRACE_LOW("[wns]: channel_status = (%d).", app_info_ptr->channel_status);
        SCI_TRACE_LOW("[wns]: expiry_time = (%s).", app_info_ptr->expiry_time);

        SCI_TRACE_LOW("[wns]: app_index = (%d).", app_info_ptr->app_index);
        SCI_TRACE_LOW("[wns]: push_notification_count = (%d).", app_info_ptr->push_notification_count);
        for(i = 0; i < app_info_ptr->push_notification_count; i++)
        {
            SCI_TRACE_LOW("[wns]:push_notification_list[%d].notification_type = (%d).", 
                                            i, app_info_ptr->push_notification_list[i].notification_type);
            SCI_TRACE_LOW("[wns]:push_notification_list[%d].time = (%s).", 
                                i, app_info_ptr->push_notification_list[i].time);
            SCI_TRACE_LOW("[wns]:push_notification_list[%d].contents = (%s).", 
                                            i, app_info_ptr->push_notification_list[i].contents);

        }
    }
    SCI_TRACE_LOW("[wns]:PrintPnsMgrAppInfo(): left===========================");
}


/*****************************************************************************/
//  Discription: Print the infomation about pns application manager
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL void PrintAllPnsMgrAppInfo(void)
{
    int i = 0;
    for(i = 0; i < WNS_APP_MAX_NUM; i++)
    {
        if(PNULL != s_pns_mgr_app_list.pns_mgr_app_info[i])
        {
            PrintPnsMgrAppInfo(s_pns_mgr_app_list.pns_mgr_app_info[i]);
        }
    }

}

/*****************************************************************************/
//  Discription: Get app info by app key
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
PUBLIC PNS_MGR_APP_INFO_T *MMIWNS_GetAppInfoByAppKey(const char *app_key)
{
    int i = 0;

    if(PNULL == app_key )
    {
        return PNULL;
    }
    
    for(i = 0; i < WNS_APP_MAX_NUM; i++)
    {
        if(PNULL != s_pns_mgr_app_list.pns_mgr_app_info[i])
        {
            if(0 == stricmp(s_pns_mgr_app_list.pns_mgr_app_info[i]->app_key, app_key))
            {
                return s_pns_mgr_app_list.pns_mgr_app_info[i];
            }
        }
    }

    return PNULL;
}

/*****************************************************************************/
//  Discription: Get app info by app name
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
PUBLIC PNS_MGR_APP_INFO_T *MMIWNS_GetAppInfoByAppName(const char *appname)
{
    int i = 0;

    if(PNULL == appname )
    {
        return PNULL;
    }
    
    for(i = 0; i < WNS_APP_MAX_NUM; i++)
    {
        if(PNULL != s_pns_mgr_app_list.pns_mgr_app_info[i])
        {
            if(0 == stricmp(s_pns_mgr_app_list.pns_mgr_app_info[i]->appname, appname))
            {
                return s_pns_mgr_app_list.pns_mgr_app_info[i];
            }
        }
    }

    return PNULL;
}



/*****************************************************************************/
//  Discription: Get app info by channel id
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
PUBLIC PNS_MGR_APP_INFO_T *MMIWNS_GetAppInfoByChannel(const char *channel_id)
{
    int i = 0;

    SCI_TRACE_LOW("[wns]: MMIWNS_GetAppInfoByChannel(): enter");

    if(PNULL == channel_id)
    {
        SCI_TRACE_LOW("[wns]: MMIWNS_GetAppInfoByChannel(): para is error.");
        return PNULL;
    }

    SCI_TRACE_LOW("[wns]: MMIWNS_GetAppInfoByChannel(): channel_id = (%s).", 
                                    channel_id);
    
    for(i = 0; i < WNS_APP_MAX_NUM; i++)
    {
        if(PNULL != s_pns_mgr_app_list.pns_mgr_app_info[i] 
            && WNS_CHANNEL_STATUS_ALREADY_CREATED == s_pns_mgr_app_list.pns_mgr_app_info[i]->channel_status)
        {
            if(0 == strcmp(s_pns_mgr_app_list.pns_mgr_app_info[i]->channel_id, channel_id))
            {
                return s_pns_mgr_app_list.pns_mgr_app_info[i];
            }
        }
    }
    
    SCI_TRACE_LOW("[wns]: MMIWNS_GetAppInfoByChannel(): failed, channel has not been created.");
    
    return PNULL;
}


/*****************************************************************************/
//  Discription: Get channel id by app key
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL BOOLEAN MMIWNS_GetChannelIdByAppKey(const char *app_key, char *channel_buf, uint16 buf_len)
{
    int i = 0;
    uint16 channel_len = 0;
    PNS_MGR_APP_INFO_T *app_info_ptr = PNULL;

    if(PNULL != app_key && PNULL != channel_buf)
    {
        app_info_ptr = MMIWNS_GetAppInfoByAppKey(app_key);
        if(PNULL != app_info_ptr && 
            WNS_CHANNEL_STATUS_ALREADY_CREATED == app_info_ptr->channel_status)
        {
            channel_len = MIN(CHANNEL_ID_MAX_LEN, buf_len);
            SCI_MEMCPY(channel_buf, app_info_ptr->channel_id, channel_len);
            return TRUE;
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Discription: Get channel id by app name
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL BOOLEAN MMIWNS_GetChannelIdByAppName(const char *appname, char *channel_buf, uint16 buf_len)
{
    int i = 0;
    uint16 channel_len = 0;
    PNS_MGR_APP_INFO_T *app_info_ptr = PNULL;

    if(PNULL != appname && PNULL != channel_buf)
    {
        app_info_ptr = MMIWNS_GetAppInfoByAppName(appname);
        if(PNULL != app_info_ptr && 
            WNS_CHANNEL_STATUS_ALREADY_CREATED == app_info_ptr->channel_status)
        {
            channel_len = MIN(CHANNEL_ID_MAX_LEN, buf_len);
            SCI_MEMCPY(channel_buf, app_info_ptr->channel_id, channel_len);
            return TRUE;
        }
    }

    return FALSE;
}


/*****************************************************************************/
//  Discription: Get channel id by app key
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
PUBLIC int MMIWNS_GetNotificationCount(const char *app_key)
{
    int notification_count = 0;
    PNS_MGR_APP_INFO_T *app_info_ptr = MMIWNS_GetAppInfoByAppKey(app_key);;

    if(PNULL != app_info_ptr)
    {
        notification_count = app_info_ptr->push_notification_count;
    }

    return notification_count;
}

/*****************************************************************************/
//     Description : wns create channel
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC MSG_WNS_STATUS_E MMIWNS_CreateChannel(PNS_MGR_APP_INFO_T *tmp_ptr)
{
    MSG_WNS_STATUS_E ret = WNS_STATUS_FAILED;
    BLOCK_ID wns_task_id = MMIWNS_GetTaskId();
    WNS_CREATE_CHANNEL_SIGNAL_T *sendSignal = PNULL;

    SCI_TRACE_LOW("[wns]: MMIWNS_CreateChannel(): enter.");

    if(PNULL == tmp_ptr)
    {
        return WNS_STATUS_BADPARAM;
    }

    if(WNS_DISCONNECTED == MMIWNS_GetWnsConnStatus())
    {
        ret = MMIWNS_ConnectService();
        if(WNS_STATUS_SUCCESS == ret)
        {
            MMIWNS_SetIsConnectByApp(TRUE);
            StartWnsCreateChannelTimer(tmp_ptr);
        }
        else
        {
            SCI_TRACE_LOW("[wns]: MMIWNS_CreateChannel(): MMIWNS_ConnectService faild, ret(%d).", ret);
        }
    }
    else if(WNS_CONNECTTING == MMIWNS_GetWnsConnStatus() 
        || MMIWNS_GetCreateChannelOngoing())
    {
        if(WNS_CHANNEL_STATUS_WAIT_TO_CREATED == tmp_ptr->channel_status)
        {
            //其他应用正在创建Channel中，暂时等待
            StartWnsCreateChannelTimer(tmp_ptr);
            ret = WNS_STATUS_SUCCESS;
        }
    }
    else if(WNS_CONNECTED == MMIWNS_GetWnsConnStatus())
    {
        if(WNS_CHANNEL_STATUS_WAIT_TO_CREATED == tmp_ptr->channel_status)
        {
            do
            {
                //creat signal
                MMI_CREATE_SIGNAL(sendSignal, 
                                                        (uint16)MSG_WNS_CREATE_CHANNEL, 
                                                        sizeof(WNS_CREATE_CHANNEL_SIGNAL_T), 
                                                        P_APP);        

                SCI_MEMCPY(sendSignal->app_key, tmp_ptr->app_key, APP_KEY_MAX_LEN);
                if(0 != strlen(tmp_ptr->channel_id))
                {
                    sendSignal->channel_id_length = strlen(tmp_ptr->channel_id);
                    sendSignal->channel_id = SCI_ALLOC_APP(sendSignal->channel_id_length);
                    if(PNULL == sendSignal->channel_id)
                    {
                        ret = WNS_STATUS_NOMEMORY;
                        SCI_TRACE_LOW("[wns]: MMIWNS_CreateChannel(): SCI_ALLOC_APP faild, ret(%d).", ret);
                        break;
                    }
                    SCI_MEMSET(sendSignal->channel_id, 0x0, sendSignal->channel_id_length);
                    SCI_MEMCPY(sendSignal->channel_id, tmp_ptr->channel_id, sendSignal->channel_id_length);
                }
                
                if(0 != wns_task_id)
                {
                    //send signal
                    MMI_SEND_SIGNAL(sendSignal, wns_task_id);

                    tmp_ptr->channel_status = WNS_CHANNEL_STATUS_CREATE_ONGOING;
                    MMIWNS_SetCreateChannelOngoing(TRUE);
                    MMIWNS_SetCreateChannelAppIndex(tmp_ptr->app_index);
                    StartWnsCreateChannelTimer(tmp_ptr);

                    ret = WNS_STATUS_SUCCESS;
                }
                else
                {
                    SCI_TRACE_LOW("[wns]: MMIWNS_CreateChannel(): faild, wns_task_id is 0.");
                }
            }while(0);
        }
        else if(WNS_CHANNEL_STATUS_CREATE_ONGOING == tmp_ptr->channel_status)
        {
            //没有收到WNS Task的响应。
            SCI_TRACE_LOW("[wns]: MMIWNS_CreateChannel(): No response from WNS Task.");
            ret = WNS_STATUS_FAILED;
        }
        else if(WNS_CHANNEL_STATUS_ALREADY_CREATED == tmp_ptr->channel_status)
        {
            //创建成功。
            SCI_TRACE_LOW("[wns]: MMIWNS_CreateChannel(): Success.");
            ret = WNS_STATUS_SUCCESS;
        }
        else if(WNS_CHANNEL_STATUS_FAILED == tmp_ptr->channel_status)
        {
            //创建失败。
            SCI_TRACE_LOW("[wns]: MMIWNS_CreateChannel(): Failed.");
            ret = WNS_STATUS_FAILED;
        }

    }

    if(WNS_STATUS_SUCCESS != ret)
    {
        if(PNULL != sendSignal)
        {
            if(PNULL != sendSignal->channel_id)
            {
                SCI_FREE(sendSignal->channel_id);
            }
            MmiDestroySignal((MmiSignalS**)&sendSignal);
        }

        StopWnsCreateChannelTimer(tmp_ptr->app_index);
        
        CreateChannelFailed(tmp_ptr->appname, tmp_ptr->app_key);
    }

    return ret;
}

/*****************************************************************************/
//     Description : handle java create channel request
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC MSG_WNS_STATUS_E MMIWNS_HandleCreateChannelReq(PNS_CREATE_CHANNEL_SIGNAL_T *signal_ptr)
{
    MSG_WNS_STATUS_E ret = WNS_STATUS_FAILED;
    PNS_MGR_APP_INFO_T *tmp_ptr = PNULL;

    SCI_TRACE_LOW("[wns]: MMIWNS_HandleCreateChannelReq(): enter.");

    if(PNULL == signal_ptr)
    {
        SCI_TRACE_LOW("[wns]: MMIWNS_HandleCreateChannelReq(): PNULL == signal_ptr.");
        return WNS_STATUS_BADPARAM;
    }

    if(!get_push_notification_setting())
    {
        CreateChannelFailed(signal_ptr->appname, signal_ptr->app_key);
        SCI_TRACE_LOW("[wns]: User have closed the push notification switch.");
        return;
    }

    
    tmp_ptr = GetExistPnsMgrAppInfo(signal_ptr->app_key);
    if(PNULL == tmp_ptr)
    {
        tmp_ptr = GetNewPnsMgrAppInfo();
        if(PNULL == tmp_ptr)
        {
            SCI_TRACE_LOW("[wns]: MMIWNS_HandleCreateChannelReq(): GetNewPnsMgrAppInfo faild.");

            CreateChannelFailed(signal_ptr->appname, signal_ptr->app_key);
            return WNS_STATUS_NOMEMORY;
        }
    }

    //Save application create channel req info
    s_pns_mgr_app_list.get_channel_info_func_cb = signal_ptr->get_channel_info_func_cb;
    SCI_MEMCPY(tmp_ptr->app_key, signal_ptr->app_key, APP_KEY_MAX_LEN);
    SCI_MEMCPY(tmp_ptr->appname, signal_ptr->appname, WNS_APP_NAME_MAX_LEN);
    tmp_ptr->channel_status = WNS_CHANNEL_STATUS_WAIT_TO_CREATED;

    if(!MMIWNS_IsRegistered())
    {
        ret = MMIWNS_RegService();
        if(WNS_STATUS_SUCCESS == ret)
        {
            StartWnsCreateChannelTimer(tmp_ptr);
        }
        else
        {
            SCI_TRACE_LOW("[wns]: MMIWNS_HandleCreateChannelReq(): MMIWNS_RegService faild.");
            CreateChannelFailed(signal_ptr->appname, signal_ptr->app_key);
            //FreePnsMgrAppInfo(tmp_ptr);
        }
    }
    else
    {
        MMIWNS_CreateChannel(tmp_ptr);
    }

    return WNS_STATUS_SUCCESS;
}





/*****************************************************************************/
//     Description : send revoke channel request to wns task
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
LOCAL MSG_WNS_STATUS_E SendRevokeChannelToWnsTask(const char *channel_id)
{
    MSG_WNS_STATUS_E ret = WNS_STATUS_FAILED;
    BLOCK_ID wns_task_id = MMIWNS_GetTaskId();
    WNS_REVOKE_CHANNEL_SIGNAL_T *sendSignal = PNULL;

    SCI_TRACE_LOW("[wns]: SendRevokeChannelToWnsTask(): enter, channel_id(%s).", channel_id);

    if(PNULL == channel_id)
    {
        SCI_TRACE_LOW("[wns]: SendRevokeChannelToWnsTask(): para error.");
        return WNS_STATUS_BADPARAM;
    }

    do
    {
        //creat signal
        MMI_CREATE_SIGNAL(sendSignal, 
                                                (uint16)MSG_WNS_REVOKE_CHANNEL, 
                                                sizeof(WNS_REVOKE_CHANNEL_SIGNAL_T), 
                                                P_APP);        

        sendSignal->channel_id = SCI_ALLOC_APP(CHANNEL_ID_MAX_LEN);
        if(PNULL == sendSignal->channel_id)
        {
            SCI_TRACE_LOW("[wns]: SendRevokeChannelToWnsTask(): SCI_ALLOC_APP failed.");
            ret = WNS_STATUS_NOMEMORY;
            break;
        }
        SCI_MEMSET(sendSignal->channel_id, 0x0, CHANNEL_ID_MAX_LEN);

        sendSignal->channel_id_length = strlen(channel_id);
        sendSignal->channel_id_length = MIN(sendSignal->channel_id_length, CHANNEL_ID_MAX_LEN);

        SCI_MEMCPY(sendSignal->channel_id, channel_id, sendSignal->channel_id_length);
        
        if(0 != wns_task_id)
        {
            //send signal
            MMI_SEND_SIGNAL(sendSignal, wns_task_id);

            ret = WNS_STATUS_SUCCESS;
            break;
        }
        
    }while(0);
    
    if(WNS_STATUS_SUCCESS != ret)
    {
        //if(PNULL != sendSignal)
        //{
            if(PNULL != sendSignal->channel_id)
            {
                SCI_FREE(sendSignal->channel_id);
            }
            MmiDestroySignal((MmiSignalS**)&sendSignal);
        //}
    }

    return ret;
}

/*****************************************************************************/
//     Description : send revoke channel request to wns task
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
LOCAL MSG_WNS_STATUS_E SendDisconnectToWnsTask(int32 reason)
{
    WNS_DISCONNECT_SIGNAL_T *sendDisconnectSignal = PNULL;

    SCI_TRACE_LOW("[wns]: SendDisconnectToWnsTask(): enter.");

    if(MMIWNS_IsRegistered())
    {
        BLOCK_ID wns_task_id = MMIWNS_GetTaskId();
        if(0 != wns_task_id)
        {
            
            //creat signal
            MMI_CREATE_SIGNAL(sendDisconnectSignal, 
                                                    (uint16)MSG_WNS_DISCONNECT_WNS, 
                                                    sizeof(WNS_DISCONNECT_SIGNAL_T), 
                                                    P_APP);  
            sendDisconnectSignal->reason = reason;

            //send signal
            MMI_SEND_SIGNAL(sendDisconnectSignal, wns_task_id);

            //Stop connect timer
            StopWnsConnectTimer();
            MMIWNS_SetWnsConnStatus(WNS_DISCONNECTED);

        }
    }
    

    return WNS_STATUS_SUCCESS;
}

/*****************************************************************************/
//     Description : free app info
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
LOCAL void FreePnsMgrAppInfo(
                            PNS_MGR_APP_INFO_T *app_ptr
                            )
{
    SCI_TRACE_LOW("[wns]:FreePnsMgrAppInfo(): enter---");

    PrintPnsMgrAppInfo(app_ptr);
    
    if(PNULL != app_ptr)
    {
            PNS_MGR_APP_NV_INFO_T *app_nv_info_ptr = PNULL;
         
            app_nv_info_ptr = (PNS_MGR_APP_NV_INFO_T *)SCI_ALLOC_APP(sizeof(PNS_MGR_APP_NV_INFO_T));
            if(PNULL != app_nv_info_ptr)
            {
                SCI_MEMSET(app_nv_info_ptr, 0x0, sizeof(PNS_MGR_APP_NV_INFO_T));
                MMI_WriteNVItem(MMIWNS_NV_PNS_DATA_APP1 + app_ptr->app_index, app_nv_info_ptr);

                SCI_FREE(app_nv_info_ptr);
                app_nv_info_ptr = PNULL;
            }
            else
            {
                SCI_TRACE_LOW("[wns]:FreePnsMgrAppInfo(): SCI_ALLOC_APP failed.");
            }

            if(WNS_CHANNEL_STATUS_ALREADY_CREATED == app_ptr->channel_status)
            {
                s_pns_mgr_app_list.available_channel_num--;
                if(WNS_APP_MAX_NUM < s_pns_mgr_app_list.available_channel_num)
                {
                    s_pns_mgr_app_list.available_channel_num = 0;
                }
                MMIWNS_SetAvailableChnNum(s_pns_mgr_app_list.available_channel_num);
            }
            
            s_pns_mgr_app_list.pns_mgr_app_info[app_ptr->app_index] = PNULL;

            SCI_FREE(app_ptr);

            if(0 == s_pns_mgr_app_list.available_channel_num)
            {
                 MMIAPIWNS_Stop(); 
            }

    }
        
    
}


/*****************************************************************************/
//     Description : free app info
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
LOCAL void FreePnsMgrAppList(void)
{
    int i = 0;

    SCI_TRACE_LOW("[wns]:FreePnsMgrAppList(): enter---");

    for(i = 0; i < WNS_APP_MAX_NUM; i ++)
    {
        if(PNULL != s_pns_mgr_app_list.pns_mgr_app_info[i])
        {
            SCI_FREE(s_pns_mgr_app_list.pns_mgr_app_info[i]);
            s_pns_mgr_app_list.pns_mgr_app_info[i] = PNULL;
        }
    }

    s_pns_mgr_app_list.available_channel_num = 0;
    
    SCI_TRACE_LOW("[wns]:FreePnsMgrAppList(): left---");

}

/*****************************************************************************/
//     Description : clear pns app data
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
LOCAL void ClearPnsMgrAppData(void)
{
    PNS_MGR_APP_NV_INFO_T *app_nv_info_ptr = PNULL;
    int i = 0;

    SCI_TRACE_LOW("[wns]:ClearPnsMgrAppData(): enter---");
         
    app_nv_info_ptr = (PNS_MGR_APP_NV_INFO_T *)SCI_ALLOC_APP(sizeof(PNS_MGR_APP_NV_INFO_T));
    if(PNULL != app_nv_info_ptr)
    {
        SCI_MEMSET(app_nv_info_ptr, 0x0, sizeof(PNS_MGR_APP_NV_INFO_T));
        for(i = 0; i < WNS_APP_MAX_NUM; i ++)
        {
            MMI_WriteNVItem(MMIWNS_NV_PNS_DATA_APP1 + i, app_nv_info_ptr);
        }
        SCI_FREE(app_nv_info_ptr);
        app_nv_info_ptr = PNULL;
    }
    else
    {
        SCI_TRACE_LOW("[wns]:ClearPnsMgrAppData(): SCI_ALLOC_APP failed.");
    }

    SCI_TRACE_LOW("[wns]:ClearPnsMgrAppData(): left---");

}



/*****************************************************************************/
//     Description : handle java close channel request
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC MSG_WNS_STATUS_E MMIWNS_HandleCloseChannelReq(PNS_CLOSE_CHANNEL_SIGNAL_T *signal_ptr)
{
    MSG_WNS_STATUS_E ret = WNS_STATUS_FAILED;
    WNS_REVOKE_CHANNEL_SIGNAL_T *sendSignal = PNULL;
    PNS_MGR_APP_INFO_T *app_info_ptr = PNULL;
    char channel_id[CHANNEL_ID_MAX_LEN] = {0};

    SCI_TRACE_LOW("[wns]: MMIWNS_HandleCloseChannelReq(): enter.");

    if(PNULL == signal_ptr)
    {
        SCI_TRACE_LOW("[wns]: MMIWNS_HandleCloseChannelReq(): para error.");
        return WNS_STATUS_BADPARAM;
    }

    if(MMIWNS_GetChannelIdByAppKey(signal_ptr->app_key, channel_id, CHANNEL_ID_MAX_LEN))
    {
        ret = SendRevokeChannelToWnsTask(channel_id);
    }

    app_info_ptr = MMIWNS_GetAppInfoByAppKey(signal_ptr->app_key);
    if(PNULL != app_info_ptr)
    {
        MMIIDLE_RemovePushMsg(MMI_IDLE_PUSH_MSG_WNS, app_info_ptr->appname, PNULL);

        FreePnsMgrAppInfo(app_info_ptr);
    }

    return ret;
}



/*****************************************************************************/
//     Description : handle java close channel request
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC MSG_WNS_STATUS_E MMIWNS_HandleAppRemovedEvent(PNS_APP_REMOVED_SIGNAL_T *signal_ptr)
{
    MSG_WNS_STATUS_E ret = WNS_STATUS_FAILED;
    WNS_REVOKE_CHANNEL_SIGNAL_T *sendSignal = PNULL;
    PNS_MGR_APP_INFO_T *app_info_ptr = PNULL;
    char channel_id[CHANNEL_ID_MAX_LEN] = {0};

    SCI_TRACE_LOW("[wns]: MMIWNS_HandleAppRemovedEvent(): enter.");

    if(PNULL == signal_ptr)
    {
        SCI_TRACE_LOW("[wns]: MMIWNS_HandleAppRemovedEvent(): para error.");
        return WNS_STATUS_BADPARAM;
    }

    if(MMIWNS_GetChannelIdByAppName(signal_ptr->appname, channel_id, CHANNEL_ID_MAX_LEN))
    {
        ret = SendRevokeChannelToWnsTask(channel_id);
    }

    app_info_ptr = MMIWNS_GetAppInfoByAppName(signal_ptr->appname);
    MMIIDLE_RemovePushMsg(MMI_IDLE_PUSH_MSG_WNS, signal_ptr->appname, PNULL);

    FreePnsMgrAppInfo(app_info_ptr);

    return ret;
}


/*****************************************************************************/
//     Description : handle java close channel request
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC MSG_WNS_STATUS_E MMIWNS_ClearNotificationByAppName(char *appname)
{
    MSG_WNS_STATUS_E ret = WNS_STATUS_SUCCESS;
    PNS_MGR_APP_INFO_T *app_info_ptr = PNULL;

    if(PNULL != appname)
    {
        SCI_TRACE_LOW("[wns]: MMIWNS_ClearNotificationByAppName(): appname=(%s)", appname);
        
        MMIIDLE_RemovePushMsg(MMI_IDLE_PUSH_MSG_WNS, appname, PNULL);
        
        app_info_ptr = MMIWNS_GetAppInfoByAppName(appname);
        if(PNULL != app_info_ptr)
        {
            app_info_ptr->push_notification_count = 0;
            MMIWNS_SaveAppInfo(app_info_ptr);
        }
    }
    
    return WNS_STATUS_SUCCESS;
}



/*****************************************************************************/
//     Description : handle java close channel request
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC MSG_WNS_STATUS_E MMIWNS_HandleAppClearNotification(PNS_CLEAR_NOTIFICATION_SIGNAL_T *signal_ptr)
{
    MSG_WNS_STATUS_E ret = WNS_STATUS_SUCCESS;

    SCI_TRACE_LOW("[wns]: MMIWNS_HandleAppClearNotification(): enter.");

    if(PNULL == signal_ptr)
    {
        SCI_TRACE_LOW("[wns]: MMIWNS_HandleAppClearNotification(): para error.");
        return WNS_STATUS_BADPARAM;
    }

    ret = MMIWNS_ClearNotificationByAppName(signal_ptr->appname);
    
    return WNS_STATUS_SUCCESS;
}


/*****************************************************************************/
//     Description : handle wns register response
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIWNS_HandleRegResp(WNS_STATUS_RESPONSE_SIGNAL_T* signal_ptr)
{
    SCI_TRACE_LOW("[wns]: MMIWNS_HandleRegResp(): enter.");

    if(PNULL == signal_ptr)
    {
        SCI_TRACE_LOW("[wns]: MMIWNS_HandleRegResp(): para error.");
        return;
    }
    
    if(WNS_STATUS_SUCCESS == signal_ptr->status)
    {
        MMIWNS_SetRegistered(TRUE);
        SCI_TRACE_LOW("[wns]: MMIWNS_HandleRegResp(): success.");
        MMIWNS_ConnectService();
    }
    else
    {
        MMIWNS_SetRegistered(FALSE);
        SCI_TRACE_LOW("[wns]: MMIWNS_HandleRegResp(): failed.");
    }
}

/*****************************************************************************/
//     Description : handle wns connect response
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIWNS_HandleConnResp(WNS_STATUS_RESPONSE_SIGNAL_T* signal_ptr)
{
    SCI_TRACE_LOW("[wns]: MMIWNS_HandleConnResp(): enter.");
    if(PNULL == signal_ptr)
    {
        SCI_TRACE_LOW("[wns]: MMIWNS_HandleConnResp(): para error.");
        return;
    }
    
    if(WNS_STATUS_SUCCESS == signal_ptr->status)
    {
        MMIWNS_SetWnsConnStatus(WNS_CONNECTED);
        MMIWNS_SetIsConnectByApp(FALSE);
        SCI_TRACE_LOW("[wns]: MMIWNS_HandleConnResp(): success.");
        //InitWnsConnectTimer();
    }
    else
    {
        MMIWNS_SetWnsConnStatus(WNS_DISCONNECTED);
        SCI_TRACE_LOW("[wns]: MMIWNS_HandleConnResp(): failed.");
        StartWnsConnectRetryTimer();
    }
}


/*****************************************************************************/
//     Description : to set the available channel number to NV
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIWNS_SaveAppInfo(
                            PNS_MGR_APP_INFO_T *app_ptr
                            )
{
    SCI_TRACE_LOW("[wns]:MMIWNS_SaveAppInfo(): enter---");

    PrintPnsMgrAppInfo(app_ptr);
    
    if(PNULL != app_ptr)
    {
            PNS_MGR_APP_NV_INFO_T *app_nv_info_ptr = PNULL;
            uint32 app_info_size = sizeof(PNS_MGR_APP_INFO_T) + (app_ptr->push_notification_count - 1)*sizeof(WNS_PUSH_NOTIFICATION_INFO_T);

            s_pns_mgr_app_list.pns_mgr_app_info[app_ptr->app_index] = app_ptr;
            
            app_nv_info_ptr = (PNS_MGR_APP_NV_INFO_T *)SCI_ALLOC_APP(sizeof(PNS_MGR_APP_NV_INFO_T));
            if(PNULL != app_nv_info_ptr)
            {
                SCI_MEMSET(app_nv_info_ptr, 0x0, sizeof(PNS_MGR_APP_NV_INFO_T));
                SCI_MEMCPY(app_nv_info_ptr, app_ptr, app_info_size);
                MMI_WriteNVItem(MMIWNS_NV_PNS_DATA_APP1 + app_ptr->app_index, app_nv_info_ptr);

                SCI_FREE(app_nv_info_ptr);
            }
            else
            {
                SCI_TRACE_LOW("[wns]:MMIWNS_SaveAppInfo(): SCI_ALLOC_APP failed.");
            }
    }
        
    
}

PUBLIC void MMIWNS_HandleCreatChannelResp(WNS_CREATE_CHANNEL_RESPONSE_SIGNAL_T* signal_ptr)
{
    PNS_MGR_APP_INFO_T *tmp_ptr = GetCurCreateChannelAppInfo();
    BOOLEAN is_create_channel_ok = FALSE;
    uint16 available_channel_num = MMIWNS_GetAvailableChnNum();

    SCI_TRACE_LOW("[wns]:MMIWNS_HandleCreatChannelResp(): enter---");

    if(PNULL == signal_ptr)
    {
        SCI_TRACE_LOW("[wns]:MMIWNS_HandleCreatChannelResp(): para error.");
        return;
    }

    if(PNULL != tmp_ptr)
    {
        StopWnsCreateChannelTimer(tmp_ptr->app_index);

        if(WNS_STATUS_SUCCESS == signal_ptr->status)
        {
            uint32 channel_id_length = MIN(signal_ptr->channel_id_length, CHANNEL_ID_MAX_LEN);
            uint32 channel_uri_length = MIN(signal_ptr->channel_uri_length, CHANNEL_URI_MAX_LEN);

            SCI_MEMSET(tmp_ptr->channel_id, 0, CHANNEL_ID_MAX_LEN);
            SCI_MEMCPY(tmp_ptr->channel_id, signal_ptr->channel_id, channel_id_length);
            SCI_MEMSET(tmp_ptr->channel_uri, 0, CHANNEL_URI_MAX_LEN);
            SCI_MEMCPY(tmp_ptr->channel_uri, signal_ptr->channel_uri, channel_uri_length);

            SCI_MEMSET(tmp_ptr->expiry_time, 0, TIME_MAX_LEN);
            ChangeTimeFomat(tmp_ptr->expiry_time, TIME_MAX_LEN, signal_ptr->expiry_time);

            available_channel_num++;
            MMIWNS_SetAvailableChnNum(available_channel_num);
            
            tmp_ptr->channel_status = WNS_CHANNEL_STATUS_ALREADY_CREATED;

            MMIWNS_SaveAppInfo(tmp_ptr);
            
            if(PNULL != s_pns_mgr_app_list.get_channel_info_func_cb)
            {
                PUSH_NOTIFICATION_CHANNEL_T pns_channel = {0};
                char channel_uri[CHANNEL_URI_MAX_LEN] = {0};
                
                SCI_MEMCPY(channel_uri, signal_ptr->channel_uri, channel_uri_length);
                pns_channel.channel_uri = channel_uri;
                pns_channel.channel_uri_length = channel_uri_length;
                SCI_MEMCPY(pns_channel.channel_id, signal_ptr->channel_id, channel_id_length);
                SCI_MEMCPY(pns_channel.expiry_time, tmp_ptr->expiry_time, TIME_MAX_LEN);

                SCI_TRACE_LOW("[wns]:MMIWNS_HandleCreatChannelResp appname = (%s).", tmp_ptr->appname);
                SCI_TRACE_LOW("[wns]:MMIWNS_HandleCreatChannelResp appkey = (%s).", tmp_ptr->app_key);
                SCI_TRACE_LOW("[wns]:MMIWNS_HandleCreatChannelResp channel_id = (%s).", pns_channel.channel_id);
                SCI_TRACE_LOW("[wns]:MMIWNS_HandleCreatChannelResp channel_uri = (%s).", pns_channel.channel_uri);
                SCI_TRACE_LOW("[wns]:MMIWNS_HandleCreatChannelResp channel_uri_length = (%d).", pns_channel.channel_uri_length);
                SCI_TRACE_LOW("[wns]:MMIWNS_HandleCreatChannelResp expiry_time = (%s).", pns_channel.expiry_time);

                s_pns_mgr_app_list.get_channel_info_func_cb(tmp_ptr->appname, 
                    tmp_ptr->app_key, &pns_channel);
                SCI_TRACE_LOW("[wns]:MMIWNS_HandleCreatChannelResp create channel success.");
                
                is_create_channel_ok = TRUE;
            }
        }

        if(!is_create_channel_ok)
        {
            tmp_ptr->channel_status = WNS_CHANNEL_STATUS_FAILED;
            CreateChannelFailed(tmp_ptr->appname,  tmp_ptr->app_key);
        }

        MMIWNS_SetCreateChannelOngoing(FALSE);
        MMIWNS_SetCreateChannelAppIndex(WNS_APP_MAX_NUM);
    }


    if(PNULL != signal_ptr->channel_id)
    {
        SCI_FREE(signal_ptr->channel_id);
        signal_ptr->channel_id = PNULL;
    }
    
    if(PNULL != signal_ptr->channel_uri)
    {
        SCI_FREE(signal_ptr->channel_uri);
        signal_ptr->channel_uri = PNULL;
    }

}

#ifdef MMISRV_AUDIO_SUPPORT

LOCAL void CallBackFunction(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)
{
    MMIAPISET_AlertRingPlayResult(result, param);

    MMIAPISET_StopVibrator(MMISET_RING_TYPE_MSG);
    MMIENVSET_StopOnetimeVibTimer();
    MMIAPIENVSET_SetVibStatus(FALSE);
}


/*****************************************************************************/
// Description : Audio call back function called by audio service.
// Global resource dependence : 
//   Author: Yintang.Ren
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleAudioCallBack(
            MMISRV_HANDLE_T handle, 
            MMISRVMGR_NOTIFY_PARAM_T *param
            )
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    SCI_TRACE_LOW("[wns]: HandleAudioCallBack() entry, handle=0x%x, param=0x%x",
                                   handle, param);
    
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        
        if(report_ptr != PNULL)
        {            
            MMISRV_TRACE_LOW("[wns]: HandleNotify(), report_ptr->report=%d", report_ptr->report);
        
            switch(report_ptr->report)
            {
            case MMISRVAUD_REPORT_END:
                CallBackFunction(MMISRVAUD_REPORT_RESULT_SUCESS, param); 
                break;
            default:
                break;
            }
        }
    }
    return TRUE;
}

LOCAL void PlayFixedRing(void)
{
    MMIAUD_RING_DATA_INFO_T     ring_data = {MMISRVAUD_RING_FMT_MIDI, 0, NULL};/*lint !e64*/
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN result = FALSE;
    uint32        play_times = 1;
    uint32          ring_id   =   0;
    uint32          ring_vol  =   0 ;

    SCI_TRACE_LOW("[wns]: PlayFixedRing(): enter");


    ring_id = WNS_NOTIFICATION_RING_ID;
    ring_vol = MMISET_VOL_THREE;

    MMI_GetRingInfo(ring_id, &ring_data);

    if (PNULL != ring_data.data_ptr)
    {
        handle = MMISRVAUD_GetHandleByName("TONE SRV");
        if(handle > 0)
        {
            SCI_TRACE_LOW("Free tone handle when play key ring !!");
            MMISRVMGR_Free(handle);
        }

        req.is_auto_free = TRUE;
        req.notify = HandleAudioCallBack;
        req.pri = MMISRVAUD_PRI_NORMAL;

        audio_srv.play_times = play_times;
        //audio_srv.duation = WNS_NOTIFICATION_RING_DUATION;
        audio_srv.volume = ring_vol;
        audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF;
        audio_srv.info.ring_buf.fmt = ring_data.type;
        audio_srv.info.ring_buf.data = ring_data.data_ptr;
        audio_srv.info.ring_buf.data_len = ring_data.data_len;
        audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;

        handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
        if(handle > 0)
        {
            result = MMISRVAUD_Play(handle, 0);       
            
            if(!result)
            {
                MMISRVMGR_Free(handle);
                CallBackFunction(MMISRVAUD_REPORT_RESULT_ERROR, PNULL);
            }
        }
        else
        {
            CallBackFunction(MMISRVAUD_REPORT_RESULT_NOT_SUPPORT, PNULL);
        }
    }
    else
    {
        SCI_TRACE_LOW("[wns]:PlayFixedRing(): ring file fomat error");
    } 
    
}
#endif


/*****************************************************************************/
//  Discription: Play notification ring
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL void PlayNotificationRing(void)
{
    BOOLEAN b_is_vibrator = FALSE;
    uint8 mode_id = MMIAPIENVSET_GetPlayRingModeId();

    SCI_TRACE_LOW("[wns]: PlayNotificationRing(): enter, mode_id(%d)", mode_id);

    if (get_push_notification_alert_setting())
    {
#ifdef JAVA_SUPPORT
        if(MMIAPIJAVA_IsJavaRuning())
        {
            b_is_vibrator = TRUE;
        }
        else
#endif
#ifdef WRE_SUPPORT
        if(WRE_IsFocusRunWin())
        {
            b_is_vibrator = TRUE;
        }
        else
#endif
        if (MMIAPICC_IsInState(CC_IN_CALL_STATE) && !MMIAPIUDISK_UdiskIsRun())
        {
            b_is_vibrator = TRUE;
        }

        if(MMIENVSET_SILENT_MODE == mode_id
            || MMIENVSET_MEETING_MODE == mode_id)
        {
            b_is_vibrator = TRUE;
        }

        SCI_TRACE_LOW("[wns]: PlayNotificationRing(): b_is_vibrator(%d)", b_is_vibrator);
        
        if(b_is_vibrator)
        {
            MMIAPISET_StopAllVibrator();
            MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,WNS_NOTIFICATION_RING_DUATION);
        }
        else
        {
#ifdef MMISRV_AUDIO_SUPPORT
            PlayFixedRing();
#endif
        }

        if(!MMIDEFAULT_GetBackLightState())
        {
            MMIDEFAULT_TurnOnBackLight();
        }
            
    }
}

PUBLIC void MMIWNS_HandleRecvNotification(WNS_PUSH_NOTIFICATION_SIGNAL_T* signal_ptr)
{
    PNS_MGR_APP_INFO_T *tmp_ptr = PNULL; 
    PNS_MGR_APP_INFO_T *app_info_ptr = PNULL; 
    char channel_id[CHANNEL_ID_MAX_LEN] = {0};
    uint32 channel_id_length = 0;
    int app_info_size = 0;
    int contents_len = 0;
    int i = 0;

    SCI_TRACE_LOW("[wns]:MMIWNS_HandleRecvNotification(): enter---");

    if(PNULL == signal_ptr)
    {
        SCI_TRACE_LOW("[wns]:MMIWNS_HandleRecvNotification para error.");
        return;
    }

    SCI_TRACE_LOW("[wns]: MMIWNS_HandleRecvNotification(): channel_id = (%s).", 
                                    signal_ptr->channel_id);
    SCI_TRACE_LOW("[wns]: MMIWNS_HandleRecvNotification(): channel_id_length = (%d).", 
                                    signal_ptr->channel_id_length);
    SCI_TRACE_LOW("[wns]: MMIWNS_HandleRecvNotification(): contents = (%s).", 
                                    signal_ptr->contents);
    SCI_TRACE_LOW("[wns]: MMIWNS_HandleRecvNotification(): content_length = (%d).", 
                                    signal_ptr->content_length);
    for(i = 0; i < signal_ptr->content_length; )
    {
        SCI_TRACE_LOW("0x%02x 0x%02x 0x%02x 0x%02x    0x%02x 0x%02x 0x%02x 0x%02x", 
            signal_ptr->contents[i],
            signal_ptr->contents[i + 1],
            signal_ptr->contents[i + 2],
            signal_ptr->contents[i + 3],
            signal_ptr->contents[i + 4],
            signal_ptr->contents[i + 5],
            signal_ptr->contents[i + 6],
            signal_ptr->contents[i + 7]);
        i += 8;
    }

    SCI_TRACE_LOW("[wns]: MMIWNS_HandleRecvNotification(): notification_type = (%d).", 
                                    signal_ptr->notification_type);

    if(!get_push_notification_setting())
    {
        SCI_TRACE_LOW("[wns]: [wns]:MMIWNS_HandleRecvNotification(): User have closed the push notification switch.");
        return;
    }

    channel_id_length = MIN(signal_ptr->channel_id_length, CHANNEL_ID_MAX_LEN);
            
    SCI_MEMCPY(channel_id, signal_ptr->channel_id, channel_id_length);

    tmp_ptr = MMIWNS_GetAppInfoByChannel(channel_id);
    if(PNULL != tmp_ptr)
    {
        tmp_ptr->push_notification_count++;
        if(WNS_NOTIFICATION_MAX_NUM < tmp_ptr->push_notification_count)
        {
            tmp_ptr->push_notification_count = WNS_NOTIFICATION_MAX_NUM;
        }
        app_info_size = sizeof(PNS_MGR_APP_INFO_T) 
            + (tmp_ptr->push_notification_count - 1)*sizeof(WNS_PUSH_NOTIFICATION_INFO_T);
        app_info_ptr = SCI_REALLOC(tmp_ptr, app_info_size);
        if(PNULL != app_info_ptr)
        {
            for(i =  app_info_ptr->push_notification_count - 1; i > 0; i--)
            {
                SCI_MEMCPY(&app_info_ptr->push_notification_list[i],
                                         &app_info_ptr->push_notification_list[i - 1],
                                         sizeof(WNS_PUSH_NOTIFICATION_INFO_T));
            }
            
            app_info_ptr->push_notification_list[0].notification_type = signal_ptr->notification_type;
            
            contents_len = signal_ptr->content_length;
            contents_len = MIN(contents_len, CONTENT_MAX_LEN);
            SCI_MEMSET(app_info_ptr->push_notification_list[0].contents, 0, CONTENT_MAX_LEN);
            SCI_MEMCPY(app_info_ptr->push_notification_list[0].contents, signal_ptr->contents, contents_len);

            SCI_MEMSET(app_info_ptr->push_notification_list[0].time, 0, TIME_MAX_LEN);
            ChangeTimeFomat(app_info_ptr->push_notification_list[0].time, TIME_MAX_LEN, signal_ptr->time);
            
            MMIWNS_SaveAppInfo(app_info_ptr);

            if(!MMIWNS_IsAppOnFgState(app_info_ptr->appname))
            {
                MMIWNS_ParsePushMsg(app_info_ptr, 0);
                PlayNotificationRing();
            }
        }
    }

    if(PNULL != signal_ptr->channel_id)
    {
        SCI_FREE(signal_ptr->channel_id);
        signal_ptr->channel_id = PNULL;
    }
    
    if(PNULL != signal_ptr->contents)
    {
        SCI_FREE(signal_ptr->contents);
        signal_ptr->contents = PNULL;
    }

}

/*****************************************************************************/
//     Description : load available channel info
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIWNS_LoadChannelInfo(uint16 available_channel_num)
{
    int i = 0;
    int index = 0;
    BOOLEAN b_ret = FALSE;
    MN_RETURN_RESULT_E ret_value = MN_RETURN_SUCCESS;
    PNS_MGR_APP_NV_INFO_T *app_nv_info_ptr = PNULL;

    SCI_TRACE_LOW("[wns]:MMIWNS_LoadChannelInfo(): enter.");

    if(0 == available_channel_num || WNS_APP_MAX_NUM < available_channel_num)
    {
        SCI_TRACE_LOW("[wns]:MMIWNS_LoadChannelInfo(): para error. available_channel_num(%d).", 
                                        available_channel_num);
        return;
    }

    do
    {
        app_nv_info_ptr = (PNS_MGR_APP_NV_INFO_T *)SCI_ALLOC_APP(sizeof(PNS_MGR_APP_NV_INFO_T));
        if(PNULL == app_nv_info_ptr)
        {
            break;
        }
        SCI_MEMSET(app_nv_info_ptr, 0x0, sizeof(PNS_MGR_APP_NV_INFO_T));

        for(i = 0; i < available_channel_num; i++)
        {
            ret_value = MMI_ReadNVItem(MMIWNS_NV_PNS_DATA_APP1 + i, app_nv_info_ptr);
            if (MN_RETURN_SUCCESS == ret_value)
            {
                uint32 app_info_size = 0;
                PNS_MGR_APP_INFO_T *app_info_ptr = PNULL;
                if(app_nv_info_ptr->pns_mgr_app_info.push_notification_count > WNS_NOTIFICATION_MAX_NUM)
                {
                    app_nv_info_ptr->pns_mgr_app_info.push_notification_count = 0;
                }
                app_info_size = sizeof(PNS_MGR_APP_INFO_T) 
                    + (app_nv_info_ptr->pns_mgr_app_info.push_notification_count - 1) * sizeof(WNS_PUSH_NOTIFICATION_INFO_T);
                app_info_ptr = (PNS_MGR_APP_INFO_T *)SCI_ALLOC_APP(app_info_size);
                if(PNULL != app_info_ptr)
                {
                    SCI_MEMSET(app_info_ptr, 0x0, app_info_size);
                    SCI_MEMCPY(app_info_ptr, app_nv_info_ptr, app_info_size); 
                    s_pns_mgr_app_list.pns_mgr_app_info[i] = app_info_ptr;

                    PrintPnsMgrAppInfo(app_info_ptr);
                }
            }
        }

    }while(0);

    if(PNULL != app_nv_info_ptr)
    {
        SCI_FREE(app_nv_info_ptr);
        app_nv_info_ptr = PNULL;
    }
    
}

/*****************************************************************************/
//     Description : handle wns stop response
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
LOCAL void MMIWNS_HandleStopResp(void)
{
    BLOCK_ID wns_task_id = MMIWNS_GetTaskId();
    xSignalHeader sig_ptr = PNULL;

    SCI_TRACE_LOW("[wns]:MMIWNS_HandleStopResp(): enter, wns_task_id(%d).", wns_task_id);

    if(0 != wns_task_id)
    {
        SCI_TRACE_LOW("[wns]:MMIWNS_HandleStopResp(): Suspend Thread");
        SCI_SuspendThread(wns_task_id);

        //maybe there is signal in the queue
        sig_ptr = SCI_PeekSignal(wns_task_id);
        while (PNULL != sig_ptr)
        {
            SCI_TRACE_LOW("[wns]:MMIWNS_HandleStopResp(): Free signal in the queue");
            SCI_FREE(sig_ptr);
            sig_ptr = PNULL;
            sig_ptr = SCI_PeekSignal(wns_task_id);
        }

        SCI_TRACE_LOW("[wns]:MMIWNS_HandleStopResp(): Terminate Thread");
        SCI_TerminateThread(wns_task_id);

        SCI_TRACE_LOW("[wns]:MMIWNS_HandleStopResp(): Delete Thread");
        SCI_DeleteThread(wns_task_id);

        MMIWNS_SetTaskId(0);
    }

}


/*****************************************************************************/
//  Description : handle wns create channel timeout
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL void HandleWnsCreateChannelTimer(
                                            uint8 timer_id,
                                            uint32 param
                                            )
{
    PNS_MGR_APP_INFO_T *tmp_ptr = (PNS_MGR_APP_INFO_T *)param;
    
    SCI_TRACE_LOW("[wns]:HandleWnsCreateChannelTimer(): enter.");

    if(PNULL == tmp_ptr)
    {
        SCI_TRACE_LOW("[wns]:HandleWnsCreateChannelTimer(): para error.");
        return;
    }

    if(!MMIWNS_IsRegistered())
    {
        SCI_TRACE_LOW("[wns]:HandleWnsCreateChannelTimer(): wns register failed.");
        CreateChannelFailed(tmp_ptr->appname, tmp_ptr->app_key);
    }
    else if(WNS_DISCONNECTED == MMIWNS_GetWnsConnStatus()
        && s_pns_mgr_app_list.wns_connect_retry_count > 0
        && s_pns_mgr_app_list.is_connect_by_app)
    {
        SCI_TRACE_LOW("[wns]:HandleWnsCreateChannelTimer(): wns connect failed.");
        if(0 == MMIWNS_GetAvailableChnNum())
        {
            StopWnsConnectTimer();
        }
        CreateChannelFailed(tmp_ptr->appname, tmp_ptr->app_key);
    }
    else
    {
        MMIWNS_CreateChannel(tmp_ptr);
    }
}

/*****************************************************************************/
//  Description : handle wns connect timeout
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL void HandleWnsConnectTimer(uint8 timer_id, uint32 param)
{
    //SCI_TRACE_LOW("[wns]:HandleWnsConnectTimer(): enter.");

    if(NET_CONNECTED == MMIWNS_GetNetConnStatus())
    {
        if(WNS_DISCONNECTED == MMIWNS_GetWnsConnStatus())
        {
            MMIWNS_ConnectService();
        }
        else if(WNS_CONNECTTING == MMIWNS_GetWnsConnStatus())
        {

        }
        else if(WNS_CONNECTED == MMIWNS_GetWnsConnStatus())
        {
            //InitWnsConnectTimer();
        }
    }
    else if(NET_DISCONNECTED == MMIWNS_GetNetConnStatus())
    {
        BOOLEAN b_ret = MMIWNS_PdpActive();
        if(!b_ret)
        {
            SCI_TRACE_LOW("[wns]: MMIWNS_ConnectService(): MMIWNS_PdpActive faild.");
            InitWnsConnectTimer();
        }
        else
        {
            MMIWNS_SetNetConnStatus(NET_CONNECTTING);
        }
    }
    else if(NET_CONNECTTING == MMIWNS_GetNetConnStatus())
    {
        InitWnsConnectTimer();
    }

}


/*****************************************************************************/
//  Description : handle socket connect timeout
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL void HandleWnsPdpActiveTimer(
                                            uint8 timer_id,
                                            uint32 param
                                            )
{
    SCI_TRACE_LOW("[wns]:HandleWnsPdpActiveTimer():---Time out error!---");
    
    MMIWNS_PdpDeactive();

    MMIWNS_SendMsgToSelf(MSG_PNS_NET_CONN_FAILED_CNF);
}

/*****************************************************************************/
//  Description :HandleWnsPdpRetryTimer
//  Global resource dependence : 
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL void HandleWnsPdpRetryTimer(
                                uint8 timer_id,
                                uint32 param
                                )
{
    MN_DUAL_SYS_E sys_sim = 0;
    BOOLEAN       sim_ok =  FALSE;

    SCI_TRACE_LOW("[wns]:HandlePdpRetryTimer(): enter");
    
    if(timer_id == s_pns_mgr_app_list.pdp_retry_timer_id && 0 != s_pns_mgr_app_list.pdp_retry_timer_id)
    {
        MMK_StopTimer(s_pns_mgr_app_list.pdp_retry_timer_id);
        s_pns_mgr_app_list.pdp_retry_timer_id = 0;
    }
    s_pns_mgr_app_list.cur_sys_num++;
    if( s_pns_mgr_app_list.cur_sys_num < MMI_DUAL_SYS_MAX)
    {
        for(sys_sim =  s_pns_mgr_app_list.cur_sys_num; sys_sim < MMI_DUAL_SYS_MAX; sys_sim++)
        {
            if(MMIPHONE_IsSimOk(sys_sim))
            {
                 s_pns_mgr_app_list.cur_sys_num = sys_sim;
                sim_ok = TRUE;
                break;
            }
        }
        
        if(sim_ok  && MMIWNS_IsPdpPermitUsing(s_pns_mgr_app_list.cur_sys_num))
        {
            if(!WnsPDPTry(s_pns_mgr_app_list.cur_sys_num))
            {
                s_pns_mgr_app_list.pdp_retry_timer_id = MMK_CreateTimerCallback(
                                                                                                WNS_PDP_RETRY_TIMEOUT, 
                                                                                                HandleWnsPdpRetryTimer, 
                                                                                                PNULL, 
                                                                                                FALSE
                                                                                                );
            }
        }
        else
        {
            //MMIAPIPDP_Deactive(MMI_MODULE_WNS);
            MMIWNS_SendMsgToSelf(MSG_PNS_NET_CONN_FAILED_CNF);
        }
    }
    else//找不到卡了
    {
        //MMIAPIPDP_Deactive(MMI_MODULE_WNS);
        MMIWNS_SendMsgToSelf(MSG_PNS_NET_CONN_FAILED_CNF);
    }
}



/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
*****************************************************************************/
LOCAL void HandleWnsPdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    
    if(PNULL == msg_ptr)
    {
        return;
    }

    SCI_TRACE_LOW("[wns]:HandleWnsPdpMsg--- msg_id=%d", (uint8*)"d",msg_ptr->msg_id);

    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:     //PDP激活成功
        SCI_TRACE_LOW("[wns]:HandleWnsPdpMsg--- MMIPDP_ACTIVE_CNF");
        StopWnsPdpActiveTimer();
        if(MMIPDP_RESULT_SUCC == msg_ptr->result)
        {
            MMIWNS_SetNetId(msg_ptr->nsapi);

            MMIWNS_SendMsgToSelf(MSG_PNS_PDP_ACTIVE_CNF);
        }
        else if (MMIPDP_RESULT_FAIL == msg_ptr->result)
        {
            MMIWNS_PdpDeactive();
            if(0 != s_pns_mgr_app_list.pdp_retry_timer_id)
            {
                MMK_StopTimer(s_pns_mgr_app_list.pdp_retry_timer_id);
                s_pns_mgr_app_list.pdp_retry_timer_id = 0;
            }
            s_pns_mgr_app_list.pdp_retry_timer_id = MMK_CreateTimerCallback(
                                                            WNS_PDP_RETRY_TIMEOUT, 
                                                            HandleWnsPdpRetryTimer, 
                                                            PNULL, 
                                                            FALSE
                                                            );
        }
        else 
        {
            MMIWNS_PdpDeactive();

            MMIWNS_SendMsgToSelf(MSG_PNS_PDP_ACTIVE_FAILED_CNF);
        }        
        break;
        
    case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功。
        SCI_TRACE_LOW("[wns]:HandleWnsPdpMsg--- MMIPDP_DEACTIVE_CNF");
        MMIWNS_SetNetId(0);
        MMIWNS_SetNetConnStatus(NET_DISCONNECTED);
        break;
        
    case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活。
        SCI_TRACE_LOW("[wns]:HandleWnsPdpMsg--- MMIPDP_DEACTIVE_IND");
        MMIWNS_SetNetId(0);
        MMIWNS_PdpDeactive();
        MMIWNS_SetNetConnStatus(NET_DISCONNECTED);
        SendDisconnectToWnsTask(1);
        InitWnsConnectTimer();
        break;
        
    default:
        break;
    }
   
}


/*****************************************************************************/
//  Description : Handle wns app Msg
//  Global resource dependence : 
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWnsAppMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    SCI_TRACE_LOW("[wns]---HandleWnsAppMsg---:msg_id = 0x%04x", msg_id);

    switch(msg_id)
    {
    case MSG_PNS_PDP_ACTIVE_CNF: 
        {
            SCI_TRACE_LOW("[wns]---MSG_PNS_PDP_ACTIVE_CNF---");
            MMIWNS_SetNetConnStatus(NET_CONNECTED);
            MMIWNS_ConnectService();
        }
        break;
    case MSG_PNS_PDP_ACTIVE_FAILED_CNF:
    case MSG_PNS_NET_CONN_FAILED_CNF:
        {
            SCI_TRACE_LOW("[wns]---MSG_PNS_PDP_ACTIVE_FAILED_CNF---");
            MMIWNS_SetNetConnStatus(NET_DISCONNECTED);
            InitWnsConnectTimer();
        }
        break; 
    case MSG_PNS_CREATE_CHANNEL_REQ:
        {
            SCI_TRACE_LOW("[wns]---MSG_PNS_CREATE_CHANNEL_REQ---");
            MMIWNS_HandleCreateChannelReq((PNS_CREATE_CHANNEL_SIGNAL_T *)param);
        }
        break;
    case MSG_PNS_CLOSE_CHANNEL_REQ:
        {
            SCI_TRACE_LOW("[wns]---MSG_PNS_CLOSE_CHANNEL_REQ---");
            MMIWNS_HandleCloseChannelReq((PNS_CLOSE_CHANNEL_SIGNAL_T *)param);
        }
        break;
    case MSG_PNS_APP_IS_REMOVED:
        {
            SCI_TRACE_LOW("[wns]---MSG_PNS_APP_IS_REMOVED---");
            MMIWNS_HandleAppRemovedEvent((PNS_APP_REMOVED_SIGNAL_T *)param);
        }
        break;
    case MSG_PNS_CLEAR_NOTIFICATION:
        {
            SCI_TRACE_LOW("[wns]---MSG_PNS_CLEAR_NOTIFICATION---");
            MMIWNS_HandleAppClearNotification((PNS_CLEAR_NOTIFICATION_SIGNAL_T *)param);
        }
        break;
    case MSG_PNS_START_WNS:
        MMIWNS_Start();
        break;
    case MSG_PNS_STOP_WNS:
        MMIWNS_Stop(FALSE);
        break;
    default:
        {
            SCI_TRACE_LOW("[wns]---HandleWnsAppMsg----default---");
        }
        break;
        
    }
    
    return TRUE;
}



PUBLIC MMI_RESULT_E HandleWnsTaskMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    SCI_TRACE_LOW("[wns]---HandleWnsTaskMsg---:msg_id = 0x%04x", msg_id);

    if(MSG_WNS_RESPONSE == msg_id)
    {
        WNS_STATUS_RESPONSE_SIGNAL_T *signal_ptr = (WNS_STATUS_RESPONSE_SIGNAL_T *)param;
        if(PNULL == signal_ptr)
        {
            SCI_TRACE_LOW("[wns]---HandleWnsTaskMsg---: param is PNULL");
            return MMI_RESULT_FALSE;
        }

        SCI_TRACE_LOW("[wns]---HandleWnsTaskMsg---:response_which_message = %d", 
                                        signal_ptr->response_which_message);
        switch (signal_ptr->response_which_message)
        {
            case MSG_WNS_REG_WNS:
            {
                MMIWNS_HandleRegResp((WNS_STATUS_RESPONSE_SIGNAL_T*)param);
                result = MMI_RESULT_TRUE;
                break;
            }
            case MSG_WNS_CONNECT_WNS:
            {
                MMIWNS_HandleConnResp((WNS_STATUS_RESPONSE_SIGNAL_T*)param);
                result = MMI_RESULT_TRUE;
                break;
            }
            case MSG_WNS_CREATE_CHANNEL:
            {
                MMIWNS_HandleCreatChannelResp((WNS_CREATE_CHANNEL_RESPONSE_SIGNAL_T*) param);
                result = MMI_RESULT_TRUE;
                break;
            }
            case MSG_WNS_STOP_WNS:
            {
                MMIWNS_HandleStopResp();
                result = MMI_RESULT_TRUE;
                break;
            }
            case MSG_WNS_DISCONNECT_WNS:
            {
                MMIWNS_SetWnsConnStatus(WNS_DISCONNECTED);
                result = MMI_RESULT_TRUE;
                break;
            }
            case MSG_WNS_REVOKE_CHANNEL:
            {
                result = MMI_RESULT_TRUE;
                break;
            }
            
            default:
                break;

        }
    }
    else if(MSG_WNS_PUSH_NOTIFICATION == msg_id)
    {
        MMIWNS_HandleRecvNotification((WNS_PUSH_NOTIFICATION_SIGNAL_T *)param);
        result = MMI_RESULT_TRUE;
    }
    else if(MSG_WNS_CONNECTION_CLOSED == msg_id)
    {
        MMIWNS_SetWnsConnStatus(WNS_DISCONNECTED);
        InitWnsConnectTimer();
        result = MMI_RESULT_TRUE;
    } 
    else if(MSG_WNS_COMMON_ERROR == msg_id)
    {
        MMIWNS_SetWnsConnStatus(WNS_DISCONNECTED);
        InitWnsConnectTimer();
        result = MMI_RESULT_TRUE;
    }
#ifdef MMIWNS_TEST_SUPPORT
    else if(MSG_WNS_REG_WNS == msg_id)
    {
        WNS_STATUS_RESPONSE_SIGNAL_T *signal_ptr = PNULL;
        //creat signal
        MMI_CREATE_SIGNAL(signal_ptr, 
                                                (uint16)MSG_WNS_RESPONSE, 
                                                sizeof(WNS_STATUS_RESPONSE_SIGNAL_T), 
                                                P_APP);        
        signal_ptr->status = WNS_STATUS_SUCCESS;
        signal_ptr->response_which_message = MSG_WNS_REG_WNS;
        //send signal
        MMI_SEND_SIGNAL(signal_ptr, P_APP);
        result = MMI_RESULT_TRUE;
    }
    else if(MSG_WNS_CONNECT_WNS == msg_id)
    {
        WNS_STATUS_RESPONSE_SIGNAL_T *signal_ptr = PNULL;
        //creat signal
        MMI_CREATE_SIGNAL(signal_ptr, 
                                                (uint16)MSG_WNS_RESPONSE, 
                                                sizeof(WNS_STATUS_RESPONSE_SIGNAL_T), 
                                                P_APP);        
        signal_ptr->status = WNS_STATUS_SUCCESS;
        signal_ptr->response_which_message = MSG_WNS_CONNECT_WNS;
        //send signal
        MMI_SEND_SIGNAL(signal_ptr, P_APP);
        result = MMI_RESULT_TRUE;
    }
    else if(MSG_WNS_CREATE_CHANNEL == msg_id)
    {
        char* channel_id = "12345678901236";
        char* channel_uri = "ahdfjahuefakfsdfnhfjauhjkhdfkncljhhuekkkllllllljsfadfiuefnafhfhflkjkjkjdslkfgj";
        BOOLEAN b_ret = FALSE;
        WNS_CREATE_CHANNEL_RESPONSE_SIGNAL_T *signal_ptr = PNULL;
        //creat signal
        MMI_CREATE_SIGNAL(signal_ptr, 
                                                (uint16)MSG_WNS_RESPONSE, 
                                                sizeof(WNS_CREATE_CHANNEL_RESPONSE_SIGNAL_T), 
                                                P_APP);
        signal_ptr->status = WNS_STATUS_SUCCESS;
        signal_ptr->response_which_message = MSG_WNS_CREATE_CHANNEL;
        do
        {
            signal_ptr->channel_id_length = strlen(channel_id);
            signal_ptr->channel_id = SCI_ALLOC_APP(signal_ptr->channel_id_length + 1);
            if(PNULL == signal_ptr->channel_id)
            {
                b_ret = FALSE;
                break;
            }
            SCI_MEMSET(signal_ptr->channel_id, 0, signal_ptr->channel_id_length + 1);
            strcpy(signal_ptr->channel_id, channel_id);

            signal_ptr->channel_uri_length = strlen(channel_uri);
            signal_ptr->channel_uri = SCI_ALLOC_APP(signal_ptr->channel_uri_length + 1);
            if(PNULL == signal_ptr->channel_uri)
            {
                b_ret = FALSE;
                break;
            }
            SCI_MEMSET(signal_ptr->channel_uri, 0, signal_ptr->channel_uri_length + 1);
            strcpy(signal_ptr->channel_uri, channel_uri);

            //send signal
            MMI_SEND_SIGNAL(signal_ptr, P_APP);

            b_ret = TRUE;
        }while(0);

        if(!b_ret)
        {
            if(PNULL != signal_ptr->channel_id)
            {
                SCI_FREE(signal_ptr->channel_id);
                signal_ptr->channel_id = PNULL;
            }
            if(PNULL != signal_ptr->channel_uri)
            {
                SCI_FREE(signal_ptr->channel_uri);
                signal_ptr->channel_uri = PNULL;
            }
            MmiDestroySignal((MmiSignalS**)&signal_ptr);
        }
        result = MMI_RESULT_TRUE;
    }
#endif //MMIWNS_TEST_SUPPORT
    return result;
}




/*****************************************************************************/
//  Description : Handle wns app Msg
//  Global resource dependence : 
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMsaSignOff(void)
{
    SCI_TRACE_LOW("[wns]---HandleMsaSignOff---");

    MMIWNS_Stop(TRUE);
    ClearPnsMgrAppData();
    
    return TRUE;
}


/*****************************************************************************/
//  Description : handle wns msg
//  Global resource dependence : 
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWnsMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 time_out = 0;

    SCI_TRACE_LOW("[wns]:HandleWnsMsg msg_id = %d",msg_id);

    if (MSG_WNS_RESPONSE <= msg_id && msg_id <= MSG_WNS_COMMON_ERROR)
    {
        result = HandleWnsTaskMsg(app_ptr, msg_id, param);
    }
    else if (MSG_PNS_NET_CONN_FAILED_CNF <= msg_id && msg_id < MSG_WNS_END)
    {
        result = HandleWnsAppMsg(app_ptr, msg_id, param);
    }
    else if(MSG_MS_ACCOUNT_SIGNOFF_CNF == msg_id)
    {
        result = HandleMsaSignOff();
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }

    return result;
}



/*****************************************************************************/
//     Description : stop wns service at poweroff
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
LOCAL void MMIWNS_Stop(BOOLEAN is_force_stop)
{
    WNS_STOP_SIGNAL_T *sendStopSignal = PNULL;

    SCI_TRACE_LOW("[wns]: MMIWNS_Stop(): enter.");

    if(get_push_notification_setting() && !is_force_stop)
    {
        SCI_TRACE_LOW("[wns]: MMIWNS_Stop(): User have open the push notification switch.");
        return;
    }

    if(MMIWNS_IsRegistered())
    {
        BLOCK_ID wns_task_id = MMIWNS_GetTaskId();
        if(0 != wns_task_id)
        {
            SendDisconnectToWnsTask(0);

            //disconnect network
            MMIWNS_PdpDeactive();
            MMIWNS_SetNetId(0);
            MMIWNS_SetNetConnStatus(NET_DISCONNECTED);


            //creat signal
            MMI_CREATE_SIGNAL(sendStopSignal, 
                                                    (uint16)MSG_WNS_STOP_WNS, 
                                                    sizeof(WNS_STOP_SIGNAL_T), 
                                                    P_APP);  
          
            //send signal
            MMI_SEND_SIGNAL(sendStopSignal, wns_task_id);

            MMIWNS_SetRegistered(FALSE);

            MMIWNS_SetIsConnectByApp(FALSE);
            MMIWNS_SetCreateChannelOngoing(FALSE);
            MMIWNS_SetCreateChannelAppIndex(WNS_APP_MAX_NUM);

            FreePnsMgrAppList();
          
            //MMIWNS_SetTaskId(0);
        }
    }
    
    SCI_TRACE_LOW("[wns]: MMIWNS_Stop(): left.");

    return;
}    


/*****************************************************************************/
//     Description : stop wns service at poweroff
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPIWNS_Stop(void)
{
    SCI_TRACE_LOW("[wns]: MMIAPIWNS_Stop(): enter.");

    MMIWNS_SendMsgToSelf(MSG_PNS_STOP_WNS);
    
    SCI_TRACE_LOW("[wns]: MMIAPIWNS_Stop(): left.");

    return;
}    






/*****************************************************************************/
//     Description : start wns 
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
LOCAL void MMIWNS_Start(void)
{
    uint16 available_channel_num;
    MN_RETURN_RESULT_E ret_value = MN_RETURN_SUCCESS;

    SCI_TRACE_LOW("[wns]: MMIWNS_Start(): enter.");

    if(!get_push_notification_setting())
    {
        SCI_TRACE_LOW("[wns]: User have closed the push notification switch.");
        return;
    }
    
    ret_value = MMI_ReadNVItem(MMIWNS_NV_AVALAIBLE_CHANNEL_NUM, &available_channel_num);
    
    if (MN_RETURN_SUCCESS != ret_value)
    {
        // set default value to NV
        available_channel_num = 0;
        MMIWNS_SetAvailableChnNum(available_channel_num);
    }
    else
    {
        s_pns_mgr_app_list.available_channel_num = available_channel_num;
    }

    if(0 == available_channel_num)
    {
        // there is not available channel, don't need to start WNS service.
        SCI_TRACE_LOW("[wns]: MMIWNS_Start(): There is not available channel, don't need to start WNS service");
        return;
    }

    MMIWNS_LoadChannelInfo(available_channel_num);

    MMIWNS_SetIsConnectByApp(FALSE);

    MMIWNS_RegService();
    
    SCI_TRACE_LOW("[wns]: MMIAPIWNS_Init(): left.");

}


/*****************************************************************************/
//     Description : init wns api
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPIWNS_Init(void)
{

    SCI_TRACE_LOW("[wns]: MMIAPIWNS_Init(): enter.");

    MMIWNS_SendMsgToSelf(MSG_PNS_START_WNS);
    
    SCI_TRACE_LOW("[wns]: MMIAPIWNS_Init(): left.");

}


/*****************************************************************************/
//     Description : init wns module
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIWNS_InitModule(void)
{
    g_wns_app.ProcessMsg = HandleWnsMsg;

    MMI_RegWnsNv();
}

///======================================================================//
///        TEST CODE
///======================================================================//

void MMIWNS_TestPushNotification(void)
{
/*
[wns]:PrintPnsMgrAppInfo(): enter===========================
[wns]: appname = (GroupMe.groupme-beta).
[wns]: appkey = (CN=C850B326-07E1-423C-A444-4B19B3018051).
[wns]: channel_id = (1;17874603817070456337).
[wns]: channel_uri = (https://cy2-int5.notify.windows-int.com/?token=AwIAAABV3DQWzbfia).
[wns]: channel_status = (3).
[wns]: expiry_time = (0114-10-30T00:00:00Z).
[wns]: app_index = (1).
[wns]: push_notification_count = (0).
*/
    WNS_PUSH_NOTIFICATION_SIGNAL_T *signal_ptr = PNULL;
    BOOLEAN b_ret = FALSE;
    char *app_key = "CN=C850B326-07E1-423C-A444-4B19B3018051";
    char channel_id[CHANNEL_ID_MAX_LEN] = {0};
    //<?xml version=\"1.0\" encoding=\"utf-16\"?>
    char contents[] = 
    "<toast>\r\n" \
"    <visual>\r\n" \
"        <binding template=\"ToastText02\">\r\n" \
"            <text id=\"1\">headlineText:&amp;&lt;&gt;&apos;&quot;Hello guys.</text>\r\n" \
"            <text id=\"2\">bodyText:Have fun.</text>\r\n" \
"        </binding>\r\n" \
"    </visual>\r\n" \
"</toast>";


    PNS_MGR_APP_INFO_T app_info = {0};
    
    strcpy(app_info.appname, "Grant");
    strcpy(app_info.app_key, "Peng");
    strcpy(app_info.channel_id, "love");
    strcpy(app_info.channel_uri, "Cindy");
    strcpy(app_info.expiry_time, "2099-10-30T00:00:00Z");
    strcpy(app_info.push_notification_list[0].contents, contents);
    app_info.push_notification_list[0].notification_type = 0;
    app_info.app_index = 0;
    app_info.channel_status = WNS_CHANNEL_STATUS_ALREADY_CREATED;
    app_info.push_notification_count = 1;
    
    strcpy(app_info.push_notification_list[0].time, "2015-10-28T17:17:00Z");

    MMIWNS_ParsePushMsg(&app_info, 0);

    PlayNotificationRing();

#if 0
    MMIWNS_GetChannelIdByAppKey(app_key, channel_id, CHANNEL_ID_MAX_LEN);

    do
    {
        //creat signal
        MMI_CREATE_SIGNAL(signal_ptr, 
                                                (uint16)MSG_WNS_PUSH_NOTIFICATION, 
                                                sizeof(WNS_PUSH_NOTIFICATION_SIGNAL_T), 
                                                P_APP);
        signal_ptr->channel_id_length = strlen(channel_id);
        signal_ptr->channel_id = SCI_ALLOC_APP(signal_ptr->channel_id_length + 1);
        if(PNULL ==  signal_ptr->channel_id)
        {
            b_ret = FALSE;
            break;
        }
        SCI_MEMSET(signal_ptr->channel_id, 0, signal_ptr->channel_id_length + 1);
        SCI_MEMCPY(signal_ptr->channel_id, channel_id, signal_ptr->channel_id_length);

        signal_ptr->content_length = strlen(contents);
        signal_ptr->contents = SCI_ALLOC_APP(signal_ptr->content_length + 1);
        if(PNULL ==  signal_ptr->channel_id)
        {
            b_ret = FALSE;
            break;
        }
        SCI_MEMSET(signal_ptr->contents, 0, signal_ptr->content_length + 1);
        SCI_MEMCPY(signal_ptr->contents, contents, signal_ptr->content_length);

        signal_ptr->notification_type = WNS_TYPE_TOAST;

        b_ret = TRUE;
        //send signal
        MMI_SEND_SIGNAL(signal_ptr, P_APP);

    }while(0);

    if(!b_ret)
    {
        if(PNULL !=  signal_ptr->channel_id)
        {
            SCI_FREE(signal_ptr->channel_id);
            signal_ptr->channel_id = PNULL;
        }
        if(PNULL !=  signal_ptr->contents)
        {
            SCI_FREE(signal_ptr->contents);
            signal_ptr->contents = PNULL;
        }

        MmiDestroySignal((MmiSignalS **)&signal_ptr);
    }
#endif
}    


void MMIWNS_TestGetPnsChannel(
                                    char *appname, 
                                    char *appkey, 
                                    PUSH_NOTIFICATION_CHANNEL_T *channel)
{
    SCI_TRACE_LOW("[wns]:MMIWNS_TestGetPnsChannel enter");

    SCI_TRACE_LOW("[wns]:MMIWNS_TestGetPnsChannel appname = (%s).", appname);
    SCI_TRACE_LOW("[wns]:MMIWNS_TestGetPnsChannel appkey = (%s).", appkey);
    if(PNULL != channel)
    {
        SCI_TRACE_LOW("[wns]:MMIWNS_TestGetPnsChannel channel_id = (%s).", channel->channel_id);
        SCI_TRACE_LOW("[wns]:MMIWNS_TestGetPnsChannel channel_uri = (%s).", channel->channel_uri);
        SCI_TRACE_LOW("[wns]:MMIWNS_TestGetPnsChannel channel_uri_length = (%d).", channel->channel_uri_length);
        SCI_TRACE_LOW("[wns]:MMIWNS_TestGetPnsChannel expiry_time = (%s).", channel->expiry_time);
    }
    else
    {
        SCI_TRACE_LOW("[wns]:MMIWNS_TestGetPnsChannel Failed");
    }
    SCI_TRACE_LOW("[wns]:MMIWNS_TestGetPnsChannel end");

    return;
}


PUBLIC void MMIWNS_TestStart(void)
{
    
    MSG_WNS_STATUS_E ret = WNS_STATUS_FAILED;
    SCI_TRACE_LOW("[wns]:MMIWNS_TestStart enter");

    {
       /* 
        char appname[] = "GroupMe.groupme-beta";
        char appkey[] = "spreadtrum520";
        ret = create_push_notification_channel_for_applicationasync(
                                                                appname, 
                                                                appkey, 
                                                                MMIWNS_TestGetPnsChannel);
        */
    }
    
    MMIWNS_TestPushNotification();
    //HandleMsaSignOff();
    //MMIIdle_AddTestData();

    //remove_app_event_handler(appnane);

    PrintAllPnsMgrAppInfo();

    
    SCI_TRACE_LOW("[wns]:MMIWNS_TestStart end");

    return;
}
