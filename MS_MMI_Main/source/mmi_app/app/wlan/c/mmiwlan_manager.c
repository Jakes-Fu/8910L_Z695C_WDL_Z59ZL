/*****************************************************************************
** File Name:      mmiwifi_manager.c                                         *
** Author:         li.li                                                     *
** Date:           2009.11.13                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009.11.13     li.li            Create                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_wlan_trc.h"
#include "wifisupp_api.h"
#include "cfl_list.h"
#include "mmiwlan_manager.h"
#include "in_message.h"
#include "mmiwlan_profile.h"
#include "mmiwlan_nv.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiwlan_text.h"
#include "mmipub.h"
#include "mmi_image.h"
#include "os_api.h"
#include "mmiwlan_win.h"
#include "app_tcp_if.h"
#include "tcpip_api.h"
#include "mmipdp_export.h"
#include "mmk_type.h"
#include "mmiwlan_portal.h"
#include "mmiidle_export.h"
#include "mmiidle_statusbar.h"
#ifdef MMI_WIDGET_SHORTCUT_SET
#include "mmiwidget_export.h"
#endif
#include "mmiwlan_image.h"
/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIWIFI_DHCP_TIMEOUT 30000 //��λ����
#define MMIWIFI_RSSI_MIN (-1000)
#define MMIWIFI_MINISEC_PER_SEC (1000)
#define MMIWIFI_DISCONNECT_CMCC_DELAY (1000)

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef struct MMIWIFI_CONTEXT
{
    MMIWIFI_STATUS_E    status;
    MMIWIFI_PROFILE_T* current_profile_ptr;
    MMIWIFI_PROC_FUNC   msg_proc_func;
    TCPIP_NETID_T       net_id;
    CFL_LIST_HANDLE     wifi_subscriber_list;
    BOOLEAN             is_need_notify_find_ap;
    BOOLEAN             is_support_roam;
    BOOLEAN             is_support_protocol[WIFISUPP_ENCRYP_PROTOCOL_MAX];
    BOOLEAN             is_support_eap[WIFISUPP_WPA_EAP_TYPE_MAX];
    BOOLEAN             is_support_inner_eap[WIFISUPP_INNER_CREDENTIAL_TYPE_MAX];
    BOOLEAN             is_initialized;
    BOOLEAN             is_specified;
    BOOLEAN             is_scanning;
    BOOLEAN             is_prompt_ok;
    BOOLEAN             is_off_req_handled;
    MMIWIFI_AUTO_CONNECT_STATUS_E             auto_connect_status;
}MMIWIFI_CONTEXT_T;

typedef void    (*WIFIFSM_DISPATCH)(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index, uint32 msg_id, void *msg_ptr);

LOCAL void WifiDisconnected(MMIWIFI_PROFILE_T* profile_ptr, MMIWIFI_INDEX_T index,uint32 msg_id,void *msg_ptr);
LOCAL void WifiConnecting(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index,uint32 msg_id, void *msg_ptr);
LOCAL void WifiConnected(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index,uint32 msg_id, void *msg_ptr);
LOCAL void WifiDhcping(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index,uint32 msg_id, void *msg_ptr);
LOCAL void WifiDhcped(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index,uint32 msg_id, void *msg_ptr);
LOCAL void WifiDisconnecting(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index,uint32 msg_id, void *msg_ptr);
static const WIFIFSM_DISPATCH  wifi_fsm_dispatch[MMIWIFI_PROFILE_STATE_MAX] = {
                                                        WifiDisconnected,
                                                        WifiConnecting,
                                                        WifiConnected,
                                                        WifiDhcping,
                                                        WifiDhcped,
                                                        WifiDisconnecting,
                                                        };
/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
LOCAL MMIWIFI_CONTEXT_T s_mmiwifi_context = {0};

LOCAL uint8   s_auto_scan_timer_id = 0; //�Զ�������ʱ��

/**-------------------------------------------------------------------------*
**                         STATIC CLASS DEFINITION                          *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
#if (defined MMIWIDGET_SUPPORT)
extern void MMIAPIWIDGET_Shortcutset_ResetWifiState(void);
#endif

/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/
/****************************************************************************/
//  Description : wifi set status function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiSetStatus(MMIWIFI_STATUS_E status);

/****************************************************************************/
//  Description : wifi handle supplicant msg WIFISUPP_SIG_ON_CNF
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiHandleSuppSigOnCnf(WIFISUPP_ON_CNF_T* wifi_on_cnf_ptr);

/****************************************************************************/
//  Description : wifi handle supplicant msg WIFISUPP_SIG_SCAN_IND
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiHandleSuppSigScanInd(WIFISUPP_SCAN_IND_T* wifi_scan_ind_ptr);

/****************************************************************************/
//  Description : wifi handle supplicant msg WIFISUPP_SIG_CONNECT_CNF
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiHandleSuppSigConnectCnf(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index,WIFISUPP_CONNECT_CNF_T* wifi_connect_cnf_ptr);

/****************************************************************************/
//  Description : wifi handle supplicant msg WIFISUPP_SIG_DISCONNECT_CNF
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiHandleSuppSigDisconnectCnf(WIFISUPP_DISCONNECT_CNF_T* wifi_disconnect_cnf_ptr);

/****************************************************************************/
//  Description : wifi handle supplicant msg WIFISUPP_SIG_DISCONNECT_IND
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiHandleSuppSigDisconnectInd(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index, WIFISUPP_DISCONNECT_IND_T* wifi_disconnect_ind_ptr);

/****************************************************************************/
//  Description : wifi handle supplicant msg WIFISUPP_SIG_BSSID_CHANGED_IND
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiHandleSuppSigBssidChandedInd(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index,WIFISUPP_BSSID_CHANGED_IND_T * wifi_bssid_changed_ind_ptr);

/****************************************************************************/
//  Description : wifi handle supplicant msg WIFISUPP_SIG_RSSI_CHANGED_IND
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiHandleSuppSigRssiChandedInd(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index,WIFISUPP_RSSI_CHANGED_IND_T * wifi_rssi_changed_ind_ptr, MMIWIFI_RSSI_CHANGED_IND_T* rssi_changed_ptr);

/****************************************************************************/
//  Description : wifi find subscriber by module id
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL CFL_LIST_NODE_HANDLE WifiFindSubscriberByModuleId(uint16 module_id);

/****************************************************************************/
//  Description : the callback of wifi find subscriber by module id
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiFindSubscriberByModuleIdCb(CFL_NODE_OBJECT data, uint16 module_id);


LOCAL BOOLEAN WifiHandleTcpSigDhcpInd(TCPIP_DHCP_RESULT_IND_SIG_T *wifi_dhcp_ind_ptr);

/****************************************************************************/
//  Description : Is wifi connecting
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/****************************************************************************/
LOCAL BOOLEAN WifiIsConnecting(void);

/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/

/****************************************************************************/
//  Description : wifi ui reg msg handle function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_RegHandleMsg(MMIWIFI_PROC_FUNC HandleMsgFunc)
{
    BOOLEAN result = TRUE;
    
    if(0 != HandleMsgFunc)
    {
        s_mmiwifi_context.msg_proc_func = HandleMsgFunc;
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    return result;
}

/****************************************************************************/
//  Description : APP reg notify function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIAPIWIFI_RegNotifyInfo(MMIWIFI_NOTIFY_INFO_T* notify_info_ptr)
{
    CFL_LIST_NODE_HANDLE node_found = 0;

    if(PNULL == notify_info_ptr)
    {
        //SCI_TRACE_LOW:"WIFI:error:Manager:%s: notify_info_ptr is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_202_112_2_18_3_9_30_0,(uint8*)"s", __func__);
        return ;
    }

    //if notify list is null, create notify list
    if(0 == s_mmiwifi_context.wifi_subscriber_list)
    {
        s_mmiwifi_context.wifi_subscriber_list = CFL_ListCreat(sizeof(MMIWIFI_NOTIFY_INFO_T),0);
        if (0 == s_mmiwifi_context.wifi_subscriber_list)
        {
            //SCI_TRACE_LOW:"WIFI:error:Manager:%s: wifi_subscriber_list create failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_212_112_2_18_3_9_30_1,(uint8*)"s", __func__);
            return;
        }
    }

    node_found = WifiFindSubscriberByModuleId(notify_info_ptr->module_id);
    if(0 == node_found)
    {
        (void)CFL_ListInsertNode(s_mmiwifi_context.wifi_subscriber_list, (CFL_LIST_NODE_HANDLE)0, notify_info_ptr, sizeof(MMIWIFI_NOTIFY_INFO_T));
        //SCI_TRACE_LOW:"WIFI:info:Manager:%s:app %d reg inserted"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_221_112_2_18_3_9_30_2,(uint8*)"sd", __func__, notify_info_ptr->module_id);
    }
}

/****************************************************************************/
//  Description : APP dereg notify function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIAPIWIFI_DeRegNotifyInfo(uint32 module_id)
{
    CFL_LIST_NODE_HANDLE node_found = 0;
    
    //find the node need delete
    node_found = WifiFindSubscriberByModuleId(module_id);
    if(0!=node_found)
    {
        //delete the node from list
        CFL_ListDeleteNode(s_mmiwifi_context.wifi_subscriber_list,node_found);
    }
}

/****************************************************************************/
//  Description : wifi on function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_On(void)
{
#ifndef WIN32
    return WIFISUPP_OnReq(SCI_IdentifyThread());
#else
    return FALSE;
#endif
}

/****************************************************************************/
//  Description : wifi off function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_Off(void)
{
    uint32 num = 0;
    MMIWIFI_PROFILE_T* profile_ptr = PNULL;

    num = MMIWIFI_GetProfileNum();
    while(num != 0)
    {
        profile_ptr = MMIWIFI_GetProfileByIndex(num-1);
        if (PNULL != profile_ptr && (profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTED) &&(profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTING))
        {
            MMIWIFI_DisConnect(profile_ptr);
        }
        num --;
    }

    if(s_auto_scan_timer_id  != 0)
    {
        MMK_StopTimer(s_auto_scan_timer_id);  
        s_auto_scan_timer_id = 0;
    }
    MMIWIFI_SetIsWlanOffReqHandled(FALSE);
    MMIWIFI_SetAutoConnectStatus(MMIWIFI_AUTO_CONNECT_STATUS_NONE);

#if (defined MMIWIDGET_SUPPORT)
    MMIAPIWIDGET_Shortcutset_ResetWifiState(); /*lint !e718*/
    #endif
#ifndef WIN32    
    return WIFISUPP_OffReq();
#else
    return FALSE;
#endif
}

/****************************************************************************/
//  Description : wifi scan function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_Scan(uint8 *ssid)
{
    return MMIWIFI_ScanEx(ssid, TRUE);
}

/****************************************************************************/
//  Description : wifi scan function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_ScanEx(uint8 *ssid, BOOLEAN is_prompt)
{
    uint32 num = 0;
    MMIWIFI_PROFILE_T* profile_ptr = PNULL;
    WIFISUPP_SCAN_REQ_INFO_T scan_req_info = {0};

    
    if(WifiIsConnecting() || !MMIWIFI_GetIsWlanOffReqHandled())
    {
        return FALSE;
    }

    if(MMIWIFI_GetScanStatus())
    {
        if (is_prompt)
        {
            MMIWIFI_OpenScanWaitWin();
        }

        return FALSE;
    }
    if(PNULL != ssid)
    {
        uint16 len = 0;

        len = strlen((char *)ssid);
        SCI_MEMCPY(scan_req_info.ssid.ssid, ssid, MIN(len, WIFISUPP_SSID_MAX_LEN));
        scan_req_info.ssid.ssid_len = len;
        MMIWIFI_SetSpecifiedScanStatus(TRUE);
    }
    else
    {
        MMIWIFI_SetSpecifiedScanStatus(FALSE);
    }

    num = MMIWIFI_GetProfileNum();
    while(num != 0)
    {
        profile_ptr = MMIWIFI_GetProfileByIndex(num-1);

        if (PNULL == profile_ptr)
        {
            continue;
        }
        
        if (!MMIWIFI_IsEditing(profile_ptr) && !MMIWIFI_IsTrustedProfile(profile_ptr) && (profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTED ||profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTING))
        {
            MMIWIFI_DeleteProfileByIndex(num - 1);
        }
        else
        {
            if(profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTED ||profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTING)
            {
                profile_ptr->is_on_air = FALSE;
                profile_ptr->rssi = MMIWIFI_RSSI_MIN;
            }
        }
        num --;
    }

    num = MMIWIFI_GetProfileNum();
    if (MMIWIFI_MAX_PROFILE_NUM == num)
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_LIST_NUM_FULL);
        return FALSE;
    }

    MMIWIFI_SetAutoConnectStatus(MMIWIFI_AUTO_CONNECT_STATUS_NONE);
    if (is_prompt)
    {
        MMIWIFI_OpenScanWaitWin();
    }
    MMIWIFI_SetScanStatus(TRUE);
#ifndef WIN32        
    return WIFISUPP_ScanReq(&scan_req_info);
#else
    return FALSE;
#endif
}

/****************************************************************************/
//  Description : wifi handle auto scan
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
void MMIWIFI_HandleAutoScan(uint8 timer_id, uint32 param)
{     
     MMIWIFI_AUTOSCAN_STATUS_E autoscan_status = (MMIWIFI_AUTOSCAN_STATUS_E)param;
     if(s_auto_scan_timer_id  != 0)
     {
         MMK_StopTimer(s_auto_scan_timer_id);  
         s_auto_scan_timer_id = 0;
     }
     MMIWIFI_AutoScanByTimer(autoscan_status);
}
/****************************************************************************/
//  Description :  auto scan use timer
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_AutoScanByTimer(MMIWIFI_AUTOSCAN_STATUS_E autoscan_status)
{
    WIFISUPP_SCAN_REQ_INFO_T scan_req_info = {0};
    uint32 scan_interval = 0;
    MMIWIFI_PROFILE_T *profile_ptr = NULL;
    uint32 num = 0;
    
    MMIWIFI_SetSpecifiedScanStatus(FALSE);
    
    if(autoscan_status && MMIAPIWIFI_GetStatus())
    {
        scan_interval = autoscan_status * MMIWIFI_MINISEC_PER_SEC;
    }
    else
    {
        return FALSE;
    }
    if(s_auto_scan_timer_id != 0)
    {
         MMK_StopTimer(s_auto_scan_timer_id);  
         s_auto_scan_timer_id = 0;
    }

    s_auto_scan_timer_id = MMK_CreateTimerCallback(                     
        scan_interval ,
        MMIWIFI_HandleAutoScan,
        autoscan_status, 
        FALSE);
    
    //SCI_TRACE_LOW:"wifi scan status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_421_112_2_18_3_9_30_3,(uint8*)"d",MMIWIFI_GetScanStatus());

    if(WifiIsConnecting() || !MMIWIFI_GetIsWlanOffReqHandled())
    {
        return FALSE;
    }

    if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus() && !MMIWIFI_IsListWinOpen())
    {
        return FALSE;
    }

    if(MMIWIFI_IsOperating())
    {
        return FALSE;
    }

    if (!MMIWIFI_GetScanStatus() )
    {
        num = MMIWIFI_GetProfileNum();
        while(num != 0)
        {
            profile_ptr = MMIWIFI_GetProfileByIndex(num-1);

            if (PNULL != profile_ptr)
            {
                if (!MMIWIFI_IsEditing(profile_ptr) && !MMIWIFI_IsTrustedProfile(profile_ptr) 
                    &&(profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTED ||profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTING))
                {//�Զ�����ʱ����ָ�������Ľ��
                    MMIWIFI_DeleteProfileByIndex(num - 1);
                }
                else
                {
                    if((profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTED ||profile_ptr->state == MMIWIFI_PROFILE_STATE_DISCONNECTING))
                    {
                        profile_ptr->is_on_air = FALSE;
                        profile_ptr->rssi = MMIWIFI_RSSI_MIN;
                    }
                }
            }
            num --;
        }
        MMIWIFI_SetScanStatus(TRUE);
#ifndef WIN32        
        return WIFISUPP_ScanReq(&scan_req_info);
#else
        return FALSE;
#endif        
    }
    return FALSE;
}

/****************************************************************************/
//  Description : MMIWIFI_DeepSleepCMD
//  Global resource dependence : 
//  Author:
//  Note: ����is_on = FALSE����ʾWAKEUP��is_on = TRUE����ʾdeepsleep
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_DeepSleepCMD(BOOLEAN  is_enter)
{
    return FALSE;
}

/****************************************************************************/
//  Description : MMIAPIWIFI_HandleDeepSleep,��Ļ����״̬�仯ʱ��WLAN���ܵĴ���
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
PUBLIC void MMIAPIWIFI_HandleDeepSleep(BOOLEAN  is_deepsleep)
{
    if (!MMIWIFI_GetIsWlanOn())
    {
        return;
    }

    if (is_deepsleep)
    {
        MMIWIFI_StopAutoScan();
        MMIWIFI_DeepSleepCMD(TRUE);
    }
    else
    {
        MMIWIFI_DeepSleepCMD(FALSE);
        MMIWIFI_StartAutoScan();
    }
}

/*****************************************************************************
 Prototype    : MmiWifiConnect
 Description  : MMI��ʵ��ִ��WiFi���ӵĹ��ܺ���
 Input        : WIFISUPP_SSID_CONFIG_T *pstConfig_info  
 Output       : None
 Return Value : PUBLIC
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2010/7/20
    Author       : George.Liu 01526
    Modification : Created function

*****************************************************************************/
PUBLIC uint32 MMIWIFI_DirectConnect(WIFISUPP_SSID_CONFIG_T *pstConfig_info)
{
    uint32 result = 0;
    uint32 num = 0;
    MMIWIFI_PROFILE_T  *profile_ptr = PNULL;
    
    num = MMIWIFI_GetProfileNum();
    while(num != 0)
    {
        profile_ptr = MMIWIFI_GetProfileByIndex(num-1);
        if (PNULL != profile_ptr && profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTED && profile_ptr->state != MMIWIFI_PROFILE_STATE_DISCONNECTING)
        {
            if(MMIWIFI_PROFILE_STATE_CONNECTING == profile_ptr->state)
            {
                MMIWIFI_DisConnect(profile_ptr);
            }
            else if ((MMIWIFI_PROFILE_STATE_DHCPING == profile_ptr->state) ||
                (MMIWIFI_PROFILE_STATE_DHCPED  == profile_ptr->state))
            {
                MMIWIFI_DisDhcp(profile_ptr);
            }
            else
            {
                //SCI_TRACE_LOW:"MMIWIFI_DirectConnect curr profile state = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_529_112_2_18_3_9_30_4,(uint8*)"d", profile_ptr->state);
            }
            
            profile_ptr->state = MMIWIFI_PROFILE_STATE_DISCONNECTING;
        }
        num --;
    }

    MMIWIFI_SetAutoConnectStatus(MMIWIFI_AUTO_CONNECT_STATUS_NONE);
#ifndef WIN32            
    result = (uint32)WIFISUPP_ConnectReq(pstConfig_info);
#endif
    return result;
}

/****************************************************************************/
//  Description : wifi connect function
//  Global resource dependence : 
//  Author:li.li 
//  Note:  Modified by George.Liu 01526, 2010/9/2
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_Connect(void *profile_ptr)
{
    MMIWIFI_OpenConnectWaitWin();

    return MMIWIFI_Connect((MMIWIFI_PROFILE_T *)profile_ptr);
}

/****************************************************************************/
//  Description : wifi connect function
//  Global resource dependence : 
//  Author:li.li 
//  Note:  Modified by George.Liu 01526, 2010/9/2
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_Connect(MMIWIFI_PROFILE_T *profile_ptr)
{
    BOOLEAN                 result = TRUE;
    SFS_HANDLE          file_handle = 0;
    uint16              *file_name = PNULL;
    uint32              buf_len = 0;
    uint32              read_len = 0;
        
    if(PNULL == profile_ptr)
    {
        return FALSE;
    }

    //SCI_TRACE_LOW:"MMIWIFI_Connect: protocol = %d, credential = %d, status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_573_112_2_18_3_9_30_5,(uint8*)"ddd",profile_ptr->config_info.encryp_protocol, profile_ptr->config_info.credential.wapi_credential.credential_type, profile_ptr->state);

    if (MMIWIFI_PROFILE_STATE_DISCONNECTED != profile_ptr->state && MMIWIFI_PROFILE_STATE_DISCONNECTING != profile_ptr->state)
    {
        //SCI_TRACE_LOW:"MMIWIFI_Connect %s is connecting (%d)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_577_112_2_18_3_9_30_6,(uint8*)"sd", profile_ptr->config_info.ssid.ssid, profile_ptr->state);
        return TRUE;
    }

    if (MMIWIFI_GetScanStatus())
    {
        //SCI_TRACE_LOW:"MMIWIFI_Connect Scaning"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_583_112_2_18_3_9_30_7,(uint8*)"");
        return FALSE;
    }

    MMIWIFI_SetCurrProfile(profile_ptr);
    if(!(profile_ptr->is_trusted))
    {
        if ((WIFISUPP_ENCRYP_PROTOCOL_WPA  == profile_ptr->config_info.encryp_protocol) || 
            (WIFISUPP_ENCRYP_PROTOCOL_WPA2 == profile_ptr->config_info.encryp_protocol))
        {
            if (WIFISUPP_WPA_CREDENTIAL_TYPE_PSK == profile_ptr->config_info.credential.wpa_credential.credential_type)
            {
                MMIWIFI_OpenWPAForConnWin();//WPA/WPA2
                return FALSE;
            }
        }
        else if (profile_ptr->config_info.encryp_protocol == WIFISUPP_ENCRYP_PROTOCOL_WEP)
        {
            MMIWIFI_OpenWepForConnWin();//WEP
            return FALSE;
        }
        else if(WIFISUPP_ENCRYP_PROTOCOL_WAPI == profile_ptr->config_info.encryp_protocol)
        {
            if (WIFISUPP_WAPI_CREDENTIAL_TYPE_PSK == profile_ptr->config_info.credential.wapi_credential.credential_type)
            {
                MMIWIFI_OpenWAPIPskForConnWin();
                return FALSE;
            }
            else if (WIFISUPP_WAPI_CREDENTIAL_TYPE_CER == profile_ptr->config_info.credential.wapi_credential.credential_type)
            {
                MMIWIFI_OpenWAPICertificateForConnWin();
                return FALSE;
            }
        }
#ifdef WLAN_EAP_SUPPORT
        else if(WIFISUPP_ENCRYP_PROTOCOL_EAP == profile_ptr->config_info.encryp_protocol)
        {
            profile_ptr->is_editing = TRUE;
            MMIWLAN_OpenEAPTypeSelectWin(profile_ptr);
            return FALSE;
        }
#endif /* WLAN_EAP_SUPPORT */
    }

#ifdef WLAN_EAP_SUPPORT
    if(WIFISUPP_ENCRYP_PROTOCOL_EAP == profile_ptr->config_info.encryp_protocol)
    {
		if (WIFISUPP_WPA_EAP_TYPE_SIM == profile_ptr->config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_type)
		{
			if (!MMIAPIPHONE_IsSimAvailable(profile_ptr->config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_info.eapsim_info.simcard_num))
			{
				MMIAPIPHONE_AlertSimNorOKStatusII(profile_ptr->config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_info.eapsim_info.simcard_num);
				return FALSE;
			}
		}
    }
#endif /* WLAN_EAP_SUPPORT */

    if(WIFISUPP_ENCRYP_PROTOCOL_WAPI == profile_ptr->config_info.encryp_protocol && WIFISUPP_WAPI_CREDENTIAL_TYPE_CER == profile_ptr->config_info.credential.wapi_credential.credential_type)
    {
        file_name = profile_ptr->client_file_name;//�ͻ���֤�����
        file_handle = SFS_CreateFile(file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
        if(SFS_ERROR_NONE == SFS_GetFileSize(file_handle,&buf_len) && buf_len > 0)
        {
            profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_ptr= (uint8 *)SCI_ALLOC(buf_len);
            SCI_MEMSET(profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_ptr, 0, buf_len);
            if(SFS_ERROR_NONE == SFS_ReadFile(file_handle, (char*)(profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_ptr), buf_len, (uint32*)(&read_len), PNULL))
            {
                if (read_len == buf_len)
                {
                    profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_len = read_len;
                }
                SFS_CloseFile(file_handle);
            }
            else
            {
                SFS_CloseFile(file_handle);
            }
        }
        else
        {
            SFS_CloseFile(file_handle);
        }
        file_name = PNULL;

        file_name = profile_ptr->parent_file_name;//��֤�����
        file_handle = SFS_CreateFile(file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
        if(SFS_ERROR_NONE == SFS_GetFileSize(file_handle,&buf_len) && buf_len > 0)
        {
            profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_ptr= (uint8 *)SCI_ALLOC(buf_len);
            SCI_MEMSET(profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_ptr, 0, buf_len);
            if(SFS_ERROR_NONE == SFS_ReadFile(file_handle, (char*)(profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_ptr), buf_len, (uint32*)(&read_len), PNULL))
            {
                if (read_len == buf_len)
                {
                    profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_len = read_len;
                }
                SFS_CloseFile(file_handle);
            }
            else
            {
                SFS_CloseFile(file_handle);
            }
        }
        else
        {
            SFS_CloseFile(file_handle);
        }
    }

    result = MMIWIFI_DirectConnect(&profile_ptr->config_info);
    profile_ptr->state = MMIWIFI_PROFILE_STATE_CONNECTING;
    if(WIFISUPP_ENCRYP_PROTOCOL_WAPI == profile_ptr->config_info.encryp_protocol
        && WIFISUPP_WAPI_CREDENTIAL_TYPE_CER == profile_ptr->config_info.credential.wapi_credential.credential_type)
    {
        SCI_FREE(profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.asue_certificate_buf_ptr);
        SCI_FREE(profile_ptr->config_info.credential.wapi_credential.credential_info.wapi_cer_info.as_certificate_buf_ptr);
    }

    return result;
}

/*****************************************************************************
 Prototype    : MMIWIFI_DisDhcp
 Description  : ֹͣdhcp
 Input        : MMIWIFI_PROFILE_T* profile_ptr  
 Output       : None
 Return Value : PUBLIC
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2010/10/2
    Author       : George.Liu 01526
    Modification : Created function

*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_DisDhcp(MMIWIFI_PROFILE_T* profile_ptr)
{
    TCPIP_NETIF_IPADDR_T addrs = {0};

    if(PNULL == profile_ptr)
    {
        //SCI_TRACE_LOW:"mmiwifi error MMIWIFI_DisDhcp: profile_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_731_112_2_18_3_9_31_8,(uint8*)"");
        return FALSE;
    }

    if (profile_ptr->ip_get.ip_get_type != MMIWIFI_IPGET_AUTO)
    {
        //SCI_TRACE_LOW:"mmiwifi error MMIWIFI_DisDhcp: ip type is self define"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_737_112_2_18_3_9_31_9,(uint8*)"");
        return TRUE;
    }

    sci_dhcp_cancel(MMIAPIWIFI_GetNetId());

    SCI_MEMSET(&(profile_ptr->ip_get.ip), 0, sizeof(profile_ptr->ip_get.ip));
    profile_ptr->state = MMIWIFI_PROFILE_STATE_CONNECTED;
    profile_ptr->is_dhcp_success = FALSE;
   
    MMIWIFI_SetIpAddress(&addrs);

    return TRUE;
}


/****************************************************************************/
//  Description : wifi disconnect function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_DisConnect(MMIWIFI_PROFILE_T *profile_ptr)
{
    BOOLEAN                         result = FALSE;
    WIFISUPP_DISCONNECT_REQ_INFO_T  disconnect_info = {0};

    if(PNULL == profile_ptr)
    {
        //SCI_TRACE_LOW:"mmiwifi error MMIWIFI_DisConnect: profile_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_764_112_2_18_3_9_31_10,(uint8*)"");
        return FALSE;
    }

#ifdef WLAN_PORTAL_SUPPORT
    if (MMIWIFI_APIsCMCC(profile_ptr))
    {
        MMIWIFI_CMCCLogoff();
        //SCI_TRACE_LOW:"MMIWIFI_DisConnect:%d\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_772_112_2_18_3_9_31_11,(uint8*)"d", SCI_GetTickCount());
        SCI_Sleep(MMIWIFI_DISCONNECT_CMCC_DELAY);
        //SCI_TRACE_LOW:"MMIWIFI_DisConnect:%d\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_774_112_2_18_3_9_31_12,(uint8*)"d", SCI_GetTickCount());
    }
#endif

    if (MMIWIFI_IPGET_AUTO == profile_ptr->ip_get.ip_get_type)
    {
        (void)MMIWIFI_DisDhcp(profile_ptr);
    }
    
    disconnect_info.ssid = profile_ptr->config_info.ssid;
    profile_ptr->state = MMIWIFI_PROFILE_STATE_DISCONNECTING;
    profile_ptr->is_dhcp_success = FALSE;
    MMIWIFI_SetAutoConnectStatus(MMIWIFI_AUTO_CONNECT_STATUS_NONE);
#ifndef WIN32          
    result = WIFISUPP_DisconnectReq(&disconnect_info);
#endif    
    return result;
}

/****************************************************************************/
//  Description : wifi DHCP function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_Dhcp(MMIWIFI_PROFILE_T* profile_ptr)
{
    int32 ret = 0;
    
    if(PNULL == profile_ptr)
    {
        //SCI_TRACE_LOW:"\n mmiwifi error MMIWIFI_Dhcp profile_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_802_112_2_18_3_9_31_13,(uint8*)"");
        return FALSE;
    }

    //SCI_TRACE_LOW:"\n mmiwifi info MMIWIFI_Dhcp begin dhcp progress!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_806_112_2_18_3_9_31_14,(uint8*)"");
    
    (void)MMIWIFI_DisDhcp(profile_ptr);
    
    profile_ptr->state = MMIWIFI_PROFILE_STATE_DHCPING;
    ret = sci_async_dhcp(MMIWIFI_DHCP_TIMEOUT, SCI_IdentifyThread(), MMIAPIWIFI_GetNetId());
    if (ret != 0)
    {
        //SCI_TRACE_LOW:"\n mmiwifi error MMIWIFI_Dhcp sci_async_dhcp return %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_814_112_2_18_3_9_31_15,(uint8*)"d", ret);
        return FALSE;
    } 
    
    return TRUE;
}

/****************************************************************************/
//  Description : wifi get status function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC MMIWIFI_STATUS_E MMIAPIWIFI_GetStatus(void)
{
    return s_mmiwifi_context.status;
}

/****************************************************************************/
//  Description : wifi get net id function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC TCPIP_NETID_T MMIAPIWIFI_GetNetId(void)
{
    return s_mmiwifi_context.net_id;
}

/****************************************************************************/
//  Description : wifi set status function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiSetStatus(MMIWIFI_STATUS_E status)
{
    s_mmiwifi_context.status = status;
}

/****************************************************************************/
//  Description : wifi set current Profile ptr
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_SetCurrProfile(MMIWIFI_PROFILE_T* profile_ptr)
{
    //SCI_TRACE_LOW:"MMIWIFI_GetCurrProfile profile_ptr=%p"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_854_112_2_18_3_9_31_16,(uint8*)"d", profile_ptr);

    s_mmiwifi_context.current_profile_ptr = profile_ptr;
}

/****************************************************************************/
//  Description : wifi get current Profile ptr
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC MMIWIFI_PROFILE_T*  MMIWIFI_GetCurrProfile(void)
{
    //SCI_TRACE_LOW:"MMIWIFI_GetCurrProfile current_profile_ptr=%p"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_865_112_2_18_3_9_31_17,(uint8*)"d", s_mmiwifi_context.current_profile_ptr);

    return s_mmiwifi_context.current_profile_ptr;
}

/****************************************************************************/
//  Description : wifi handle supplicant msg function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_HandleSuppMsg(uint32 msg_id, void* param_ptr)
{
    BOOLEAN result = TRUE;
    MMIWIFI_INDEX_T index = 0;
    uint32 num = 0;
    MMIWIFI_PROFILE_T* profile_ptr = PNULL;
    void* original_param=(void*)((uint32)param_ptr);

    if ((MMIWIFI_STATUS_OFF == MMIAPIWIFI_GetStatus()) && (msg_id != WIFISUPP_SIG_ON_CNF))
    {
        //SCI_TRACE_LOW:"mmiwifi error MMIWIFI_HandleSuppMsg: wrong msg(%d)!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_884_112_2_18_3_9_31_18,(uint8*)"d", msg_id - WIFISUPP_SIG_START);
        return FALSE;
    }

    //SCI_TRACE_LOW:"MMIWIFI_HandleSuppMsg msg = (%d)!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_888_112_2_18_3_9_31_19,(uint8*)"d", msg_id - WIFISUPP_SIG_START);
    
    switch(msg_id)
    {
    case WIFISUPP_SIG_ON_CNF:
        {
            MMIWIFI_ON_CNF_T wifi_on = {0};
            WIFISUPP_ON_CNF_T* wifi_on_cnf_ptr= (WIFISUPP_ON_CNF_T*) original_param;
            
            //SCI_TRACE_LOW:"\n mmiwifi recv WIFISUPP_SIG_ON_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_897_112_2_18_3_9_31_20,(uint8*)"");
            
            //handle wifi on cnf sig
            if(WifiHandleSuppSigOnCnf(wifi_on_cnf_ptr))
            {
                if (!MMIWIFI_GetIsInitialized())
                {
                    //initialize wifi profile
                    MMIWIFI_ProfileInitialize();
                    MMIWIFI_SetIsInitialized(TRUE);
                }
                
                //change wifi status
                WifiSetStatus(MMIWIFI_STATUS_ON);
                wifi_on.result = MMIWIFI_RESULT_SUCC;
            }
            else
            {
                wifi_on.result = MMIWIFI_RESULT_FAIL;
            }
            //ind wifi ui
            if (PNULL != s_mmiwifi_context.msg_proc_func)
            {
                s_mmiwifi_context.msg_proc_func(MMIWIFI_MSG_ON_CNF,&wifi_on,sizeof(MMIWIFI_ON_CNF_T));
            }
        }
        break;
    case WIFISUPP_SIG_OFF_CNF:
        {
            MMIWIFI_OFF_CNF_T wifi_off = {0};
            WIFISUPP_OFF_CNF_T* wifi_off_cnf_ptr = (WIFISUPP_OFF_CNF_T*) original_param;
            MMIWIFI_NOTIFY_PARAM_T notify ={0};
            
            //SCI_TRACE_LOW:"\n mmiwifi recv WIFISUPP_SIG_OFF_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_930_112_2_18_3_9_31_21,(uint8*)"");
            if(WIFISUPP_RESULT_SUCC == wifi_off_cnf_ptr->off_result.wifi_off_result)
            {
                //change wifi status
                WifiSetStatus(MMIWIFI_STATUS_OFF);
                wifi_off.result = MMIWIFI_RESULT_SUCC;
                //MMIWIFI_ProfileFinalize();
                MMIWIFI_SetIsInitialized(FALSE);
                MMIWIFI_DeleteOnAirProfile();
#ifdef MMI_WIDGET_SHORTCUT_SET
                MMIAPIWIDGET_Shortcutset_SetWifiState();
#endif
            }
            else
            {
                wifi_off.result = MMIWIFI_RESULT_FAIL;
            }

            //ind wifi ui
            if (PNULL != s_mmiwifi_context.msg_proc_func)
            {
                s_mmiwifi_context. msg_proc_func(MMIWIFI_MSG_OFF_CNF,&wifi_off,sizeof(MMIWIFI_OFF_CNF_T));
            }

            notify.result = wifi_off_cnf_ptr->off_result.wifi_off_result;
            MMIWIFI_NotifyApp(MMIWIFI_NOTIFY_EVENT_OFF, &notify);
        }
        break;
        
    case WIFISUPP_SIG_SCAN_CNF:
        {
            MMIWIFI_SCAN_CNF_T wifi_scan = {0};
            WIFISUPP_SCAN_CNF_T* wifi_scan_cnf_ptr = (WIFISUPP_SCAN_CNF_T*) original_param;
            MMIWIFI_NOTIFY_PARAM_T notify ={0};

            //SCI_TRACE_LOW:"\n mmiwifi recv WIFISUPP_SIG_SCAN_CNF, result=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_962_112_2_18_3_9_31_22,(uint8*)"d", wifi_scan_cnf_ptr->scan_result);

            MMIWIFI_SetScanStatus(FALSE);
            
            if(WIFISUPP_RESULT_SUCC == wifi_scan_cnf_ptr->scan_result)
            {
                wifi_scan.result = MMIWIFI_RESULT_SUCC;
#ifndef WIFI_SUPPORT_UNISOC_RX
                MAIN_SetIdleWifiIcon(IMAGE_WIFI_ON_ICON);
#endif
            }
            else
            {
                wifi_scan.result = MMIWIFI_RESULT_FAIL;
            }


            //ind wifi ui
            if (PNULL != s_mmiwifi_context.msg_proc_func)
            {
                s_mmiwifi_context.msg_proc_func(MMIWIFI_MSG_SCAN_CNF,&wifi_scan,sizeof(MMIWIFI_SCAN_CNF_T));
            }

            MMIWIFI_SetSpecifiedScanStatus(FALSE);
            //check if need notify app
            notify.result = wifi_scan_cnf_ptr->scan_result;
            MMIWIFI_NotifyApp(MMIWIFI_NOTIFY_EVENT_SCAN_CNF, &notify);
        }
        break;

    case WIFISUPP_SIG_AUTOSCAN_CNF:
        MMIWIFI_SetSpecifiedScanStatus(FALSE);
        break;

    case WIFISUPP_SIG_SCAN_BEGIN_IND:
        {
            MMIWIFI_SCAN_CNF_T wifi_scan = {0};
            //SCI_TRACE_LOW:"mmiwifi recv WIFISUPP_SIG_SCANBEGIN_IND sig"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_996_112_2_18_3_9_31_23,(uint8*)"");

            MMIWIFI_SetSpecifiedScanStatus(FALSE);

            if (PNULL != s_mmiwifi_context.msg_proc_func)
            {
                s_mmiwifi_context.msg_proc_func(MMIWIFI_MSG_SCAN_BEGIN_IND,&wifi_scan,sizeof(MMIWIFI_SCAN_CNF_T));
            }
        }
        break;

    case WIFISUPP_SIG_SCAN_IND:
        {
            WIFISUPP_SCAN_IND_T* wifi_scan_ind_ptr = (WIFISUPP_SCAN_IND_T*)original_param;
            
            //SCI_TRACE_LOW:"\n mmiwifi recv WIFISUPP_SIG_SCAN_IND, ssid_len=%d,ssid=%s,signal_qua=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1011_112_2_18_3_9_31_24,(uint8*)"dsd", wifi_scan_ind_ptr->ap_info.ssid.ssid_len, wifi_scan_ind_ptr->ap_info.ssid.ssid, wifi_scan_ind_ptr->ap_info.signal_qua);

            if(0 == MMIWIFI_GetProfileNum())
            {
                s_mmiwifi_context.is_need_notify_find_ap = TRUE;
            }
            else
            {
                s_mmiwifi_context.is_need_notify_find_ap = FALSE;
            }
            //handle wifi scan ind
            result = WifiHandleSuppSigScanInd(wifi_scan_ind_ptr);

            if (PNULL != s_mmiwifi_context.msg_proc_func)
            {
                s_mmiwifi_context.msg_proc_func(MMIWIFI_MSG_SCAN_IND,wifi_scan_ind_ptr,sizeof(WIFISUPP_SCAN_IND_T));
            }           

        }
        break;

     case TCPIP_DHCP_RESULT_IND:
        {
            TCPIP_DHCP_RESULT_IND_SIG_T *wifi_dhcp_ind_ptr = (TCPIP_DHCP_RESULT_IND_SIG_T *)original_param;
            
            //SCI_TRACE_LOW:"WIFI:info:Manager:recv TCPIP_DHCP_RESULT_IND %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1036_112_2_18_3_9_31_25,(uint8*)"d", wifi_dhcp_ind_ptr->result);
            (void)WifiHandleTcpSigDhcpInd(wifi_dhcp_ind_ptr);

            result = MMI_RESULT_FALSE;
        }
        break;
    case WIFISUPP_SIG_DISCONNECT_CNF:
        {
            WIFISUPP_DISCONNECT_CNF_T* wifi_disconnect_cnf_ptr = (WIFISUPP_DISCONNECT_CNF_T*)original_param;

            //SCI_TRACE_LOW:"WIFI:info:Manager:recv WIFISUPP_SIG_DISCONNECT_CNF %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1046_112_2_18_3_9_31_26,(uint8*)"d", wifi_disconnect_cnf_ptr->disconnect_result.result);
            (void)WifiHandleSuppSigDisconnectCnf(wifi_disconnect_cnf_ptr);
        }
        break;
        
    default:
        //SCI_TRACE_LOW:"\n mmiwifi recv miscellaneous message, msg_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1052_112_2_18_3_9_31_27,(uint8*)"d", msg_id);
        num = MMIWIFI_GetProfileNum();
        for (index = 0; index < num; index ++) /* Trusted list is ahead */
        {
            profile_ptr = MMIWIFI_GetProfileByIndex(index);

            if (PNULL != profile_ptr)
            {
                wifi_fsm_dispatch[profile_ptr->state](profile_ptr, index, msg_id, original_param);
            }
        }
        break;
    }
    return result;
}

/*****************************************************************************
 Prototype    : WifiHandleTcpSigDhcpInd
 Description  : MMI���ܴ����ֶ�dhcpCNF��Ϣ����Ӧ����
 Input        : MMIWIFI_DHCP_CNF_T *wifi_dhcp_cnf_ptr  
 Output       : None
 Return Value : LOCAL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2010/9/13
    Author       : George.Liu 01526
    Modification : Created function

*****************************************************************************/
LOCAL BOOLEAN WifiHandleTcpSigDhcpInd(TCPIP_DHCP_RESULT_IND_SIG_T *wifi_dhcp_ind_ptr)
{
    uint32         num = 0;
    uint32           i = 0;
    char       *ip_ptr = PNULL;
    uint16  ip_str_len = 0;
    MMIWIFI_PROFILE_T *profile_ptr = PNULL;
    MMIWIFI_DHCP_CNF_T dhcp_cnf    = {0};
    MMIWIFI_NOTIFY_PARAM_T  notify = {0};
    
    if (PNULL == wifi_dhcp_ind_ptr)
    {
        return FALSE;
    }

    dhcp_cnf.netif  = wifi_dhcp_ind_ptr->addrs;
    dhcp_cnf.result = (MMIWIFI_RESULT_E)wifi_dhcp_ind_ptr->result;

    num = MMIWIFI_GetProfileNum();
    for (i = 0; i < num; i++)
    {
        profile_ptr = MMIWIFI_GetProfileByIndex(i);
        if (PNULL == profile_ptr)
        {
            //SCI_TRACE_LOW:"WIFI:error:%s profile_ptr is PNULL!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1106_112_2_18_3_9_32_28,(uint8*)"s", __func__);
            dhcp_cnf.result= MMIWIFI_RESULT_FAIL;
            break;
        }
        
        if ((MMIWIFI_PROFILE_STATE_DHCPING == profile_ptr->state) ||
            (MMIWIFI_PROFILE_STATE_DHCPED  == profile_ptr->state))
        {
            dhcp_cnf.index = i;

            if (wifi_dhcp_ind_ptr->result != DHCPRES_SUCCESS)
            {
                //SCI_TRACE_LOW:"WIFI:info:%s: result(%d) is failed!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1118_112_2_18_3_9_32_29,(uint8*)"sd", __func__, wifi_dhcp_ind_ptr->result);
                MMIWIFI_DisConnect(profile_ptr);
                break;
            }

            //SCI_TRACE_LOW:"WIFI:info:Manager:%s host IP      :%0x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1123_112_2_18_3_9_32_30,(uint8*)"sd", __func__, wifi_dhcp_ind_ptr->addrs.ipaddr);
            //SCI_TRACE_LOW:"WIFI:info:Manager:%s subnet mask  :%0x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1124_112_2_18_3_9_32_31,(uint8*)"sd", __func__, wifi_dhcp_ind_ptr->addrs.snmask);
            //SCI_TRACE_LOW:"WIFI:info:Manager:%s gateway      :%0x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1125_112_2_18_3_9_32_32,(uint8*)"sd", __func__, wifi_dhcp_ind_ptr->addrs.gateway);
            //SCI_TRACE_LOW:"WIFI:info:Manager:%s primary DNS  :%0x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1126_112_2_18_3_9_32_33,(uint8*)"sd", __func__, wifi_dhcp_ind_ptr->addrs.dns1);
            //SCI_TRACE_LOW:"WIFI:info:Manager:%s secondary DNS:%0x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1127_112_2_18_3_9_32_34,(uint8*)"sd", __func__, wifi_dhcp_ind_ptr->addrs.dns2);
            //SCI_TRACE_LOW:"WIFI:info:Manager:%s DHCP server  :%0x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1128_112_2_18_3_9_32_35,(uint8*)"sd", __func__, wifi_dhcp_ind_ptr->dhcp_addr);
            //SCI_TRACE_LOW:"WIFI:info:Manager:%s netid        :%0x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1129_112_2_18_3_9_32_36,(uint8*)"sd", __func__, wifi_dhcp_ind_ptr->netid);
            
            MMIWIFI_SetIpAddress(&wifi_dhcp_ind_ptr->addrs);
            //save ip addr
            ip_ptr = inet_ntoa(wifi_dhcp_ind_ptr->addrs.ipaddr);
            if(PNULL != ip_ptr)
            {
                ip_str_len = SCI_STRLEN(ip_ptr);
                strncpy(profile_ptr->ip_get.ip.ip_addr, ip_ptr, MIN(ip_str_len, MMIWIFI_IP_LEN));
            }

            //save sub mask
            ip_ptr = inet_ntoa(wifi_dhcp_ind_ptr->addrs.snmask);
            if(PNULL != ip_ptr)
            {
                ip_str_len = SCI_STRLEN(ip_ptr);
                strncpy(profile_ptr->ip_get.ip.subnet_mask, ip_ptr, MIN(ip_str_len, MMIWIFI_IP_LEN));
            }

            //save gateway
            ip_ptr = inet_ntoa(wifi_dhcp_ind_ptr->addrs.gateway);
            if(PNULL != ip_ptr)
            {
                ip_str_len = SCI_STRLEN(ip_ptr);
                strncpy(profile_ptr->ip_get.ip.default_gateway, ip_ptr, MIN(ip_str_len, MMIWIFI_IP_LEN));
            }

            //save dns1
            ip_ptr = inet_ntoa(wifi_dhcp_ind_ptr->addrs.dns1);
            if(PNULL != ip_ptr)
            {
                ip_str_len =SCI_STRLEN(ip_ptr);
                strncpy(profile_ptr->ip_get.ip.first_dns, ip_ptr, MIN(ip_str_len, MMIWIFI_IP_LEN));
            }

            //save dns2
            ip_ptr = inet_ntoa(wifi_dhcp_ind_ptr->addrs.dns2);
            if(PNULL != ip_ptr)
            {
                ip_str_len  = SCI_STRLEN(ip_ptr);
                strncpy(profile_ptr->ip_get.ip.second_dns, ip_ptr, MIN(ip_str_len, MMIWIFI_IP_LEN));
            }

            profile_ptr->state = MMIWIFI_PROFILE_STATE_DHCPED;
            WifiSetStatus(MMIWIFI_STATUS_CONNECTED);
            
#ifdef    WLAN_RESELECT_SUPPORT
            MMIWIFI_OpenSelectServiceWin();
#endif

            profile_ptr->is_dhcp_success = TRUE;

            if (!profile_ptr->is_trusted)
            {
                if(MMIWIFI_MAX_TRUSTED_LIST_NUM > MMIWIFI_GetTrustedProfileNum())
                {
                    profile_ptr->is_trusted= TRUE;
                    profile_ptr->is_auto_connect = TRUE;
                }
                else
                {
                    //Todo: Replace one
                    ;
                }

                MMIWIFI_UpdateProfileFile();
            }

            break;
        }
    }

    if ((0 == num)||(i >= num))
    {
        //SCI_TRACE_LOW:"WIFI:error:%s not find profile_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1203_112_2_18_3_9_32_37,(uint8*)"s", __func__);

        dhcp_cnf.result= MMIWIFI_RESULT_FAIL;
    }

    if (s_mmiwifi_context.msg_proc_func != PNULL)
    {
        s_mmiwifi_context.msg_proc_func(MMIWIFI_MSG_DHCP_CNF,  &dhcp_cnf, sizeof(MMIWIFI_DHCP_CNF_T));
    }

#ifdef WLAN_PORTAL_SUPPORT
    if (WIFISUPP_RESULT_SUCC == dhcp_cnf.result && MMIWIFI_APIsCMCC(profile_ptr))
    {
        MMIWIFI_CMCCAutoLogin();
    }
#endif
    //notify app
    notify.result = (WIFISUPP_RESULT_E)dhcp_cnf.result;
    MMIWIFI_NotifyApp(MMIWIFI_NOTIFY_EVENT_CONNECT, &notify);

    return TRUE;
}

/****************************************************************************/
//  Description : wifi handle supplicant msg WIFISUPP_SIG_ON_CNF
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiHandleSuppSigOnCnf(WIFISUPP_ON_CNF_T* wifi_on_cnf_ptr)
{
    BOOLEAN                 result = FALSE;
    uint32                  i = 0;
    MMIWIFI_NOTIFY_PARAM_T  notify = {0};
    
    if(PNULL != wifi_on_cnf_ptr)
    {
        //save wifi on cnf param to context
        if(WIFISUPP_RESULT_SUCC  == wifi_on_cnf_ptr->on_result.wifi_on_result)
        {
            s_mmiwifi_context.net_id = wifi_on_cnf_ptr->on_result.net_id;
            s_mmiwifi_context.is_support_roam = wifi_on_cnf_ptr->on_result.is_support_roam;
            for(i = 0; i< WIFISUPP_ENCRYP_PROTOCOL_MAX; i++)
            {
                s_mmiwifi_context.is_support_protocol[i] = wifi_on_cnf_ptr->on_result.is_support_protocol[i];
            }
            for(i = 0; i< WIFISUPP_WPA_EAP_TYPE_MAX; i++)
            {
                s_mmiwifi_context.is_support_eap[i] = wifi_on_cnf_ptr->on_result.is_support_eap[i];
            }
            for(i = 0; i< WIFISUPP_INNER_CREDENTIAL_TYPE_MAX; i++)
            {
                s_mmiwifi_context.is_support_inner_eap[i] = wifi_on_cnf_ptr->on_result.is_support_inner_eap[i];
            }
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
        
        notify.result = wifi_on_cnf_ptr->on_result.wifi_on_result;
        MMIWIFI_NotifyApp(MMIWIFI_NOTIFY_EVENT_ON, &notify);
    }
    else
    {
        result = FALSE;
        //SCI_TRACE_LOW:"WIFI:error:Manager:%s wifi_on_cnf_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1268_112_2_18_3_9_32_38,(uint8*)"s", __func__);
    }

    return result;
}

/****************************************************************************/
//  Description : wifi handle supplicant msg WIFISUPP_SIG_SCAN_IND
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiHandleSuppSigScanInd(WIFISUPP_SCAN_IND_T* wifi_scan_ind_ptr)
{
    uint8 essid[WIFISUPP_SSID_MAX_LEN + 1] = {0};
    MMIWIFI_NOTIFY_PARAM_T notify = {0};
    MMIWIFI_PROFILE_T profile = {0};
    MMIWIFI_PROFILE_T* profile_ptr = PNULL;
    uint32    num = 0;
    uint32  index = 0;
    WIFISUPP_WAPI_CREDENTIAL_TYPE_E wapi_type = WIFISUPP_WAPI_CREDENTIAL_TYPE_PSK;  

    if (PNULL == wifi_scan_ind_ptr)
    {
        return FALSE;
    }

#ifndef WIFI_SUPPORT_UNISOC_RX
    if (0 == wifi_scan_ind_ptr->ap_info.ssid.ssid_len)
    {
        //SCI_TRACE_LOW:"WIFI:info:%s scan indication hide SSID AP"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1295_112_2_18_3_9_32_39,(uint8*)"s", __func__);
        return TRUE;
    }
#endif

    SCI_MEMCPY(essid, wifi_scan_ind_ptr->ap_info.ssid.ssid, WIFISUPP_SSID_MAX_LEN);
    //SCI_TRACE_LOW:"WIFI:info:Manager:%s ESSID =%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1300_112_2_18_3_9_32_40,(uint8*)"ss", __func__, essid);
    //SCI_TRACE_LOW:"WIFI:info:Manager:%s network_mode =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1301_112_2_18_3_9_32_41,(uint8*)"sd", __func__, wifi_scan_ind_ptr->ap_info.network_mode);
    //SCI_TRACE_LOW:"WIFI:info:Manager:%s encryp_protocol =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1302_112_2_18_3_9_32_42,(uint8*)"sd", __func__, wifi_scan_ind_ptr->ap_info.encryp_protocol);
    //SCI_TRACE_LOW:"WIFI:info:Manager:%s credential_type =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1303_112_2_18_3_9_32_43,(uint8*)"sd", __func__, wifi_scan_ind_ptr->ap_info.credential_type);
    //SCI_TRACE_LOW:"WIFI:info:Manager:%s chanel =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1304_112_2_18_3_9_32_44,(uint8*)"sd", __func__, wifi_scan_ind_ptr->ap_info.channel);
    //SCI_TRACE_LOW:"WIFI:info:Manager:%s pairwise_cipher =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1305_112_2_18_3_9_32_45,(uint8*)"sd", __func__, wifi_scan_ind_ptr->ap_info.pairwise_cipher);
    //SCI_TRACE_LOW:"WIFI:info:Manager:%s group_cipher =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1306_112_2_18_3_9_32_46,(uint8*)"sd", __func__, wifi_scan_ind_ptr->ap_info.group_cipher);
    //SCI_TRACE_LOW:"WIFI:info:Manager:%s rssi =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1307_112_2_18_3_9_32_47,(uint8*)"sd", __func__, wifi_scan_ind_ptr->ap_info.signal_qua);
    //SCI_TRACE_LOW:"WIFI:info:Manager:%s rate =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1308_112_2_18_3_9_32_48,(uint8*)"sd", __func__, wifi_scan_ind_ptr->ap_info.rate);
    
    if (WIFISUPP_ENCRYP_PROTOCOL_WAPI == wifi_scan_ind_ptr->ap_info.encryp_protocol)
    {
        if (WIFISUPP_CREDENTIAL_TYPE_PSK == wifi_scan_ind_ptr->ap_info.credential_type)
        {
            wapi_type = WIFISUPP_WAPI_CREDENTIAL_TYPE_PSK;
        }
        else
        {
            wapi_type = WIFISUPP_WAPI_CREDENTIAL_TYPE_CER;
        }
    }

#ifndef WIFI_SUPPORT_UNISOC_RX
    num = MMIWIFI_GetProfileNum();
    while(num != 0)
    {
        profile_ptr = MMIWIFI_GetProfileByIndex(num-1);
        if (PNULL != profile_ptr
        &&(profile_ptr->config_info.ssid.ssid_len == wifi_scan_ind_ptr->ap_info.ssid.ssid_len)
        && (!memcmp(profile_ptr->config_info.ssid.ssid, wifi_scan_ind_ptr->ap_info.ssid.ssid, wifi_scan_ind_ptr->ap_info.ssid.ssid_len))
        && (profile_ptr->config_info.encryp_protocol == wifi_scan_ind_ptr->ap_info.encryp_protocol)
        && (profile_ptr->config_info.network_mode == wifi_scan_ind_ptr->ap_info.network_mode)
        &&((WIFISUPP_ENCRYP_PROTOCOL_WAPI != wifi_scan_ind_ptr->ap_info.encryp_protocol) || (WIFISUPP_ENCRYP_PROTOCOL_WAPI == wifi_scan_ind_ptr->ap_info.encryp_protocol &&  profile_ptr->config_info.credential.wapi_credential.credential_type == wapi_type)))
        {
            profile_ptr->is_on_air = TRUE;
            if(MMIWIFI_IsNeedRedrawWifiIcon(wifi_scan_ind_ptr->ap_info.signal_qua,profile_ptr->rssi))    
            {
                 if(MMIWIFI_PROFILE_STATE_DHCPED == profile_ptr->state || (MMIWIFI_PROFILE_STATE_CONNECTED == profile_ptr->state && MMIWIFI_IPGET_SELFDEFINED == profile_ptr->ip_get.ip_get_type))         
                 {
                     MMI_IMAGE_ID_T image = 0;
                     if (WIFISUPP_ENCRYP_PROTOCOL_OPENSYS == profile_ptr->config_info.encryp_protocol)
                     {                       
                         image = WifiGetConnectedRssiImage(wifi_scan_ind_ptr->ap_info.signal_qua,FALSE);
                     }
                     else
                     {
                         image = WifiGetConnectedRssiImage(wifi_scan_ind_ptr->ap_info.signal_qua,TRUE);                        
                     }    
#ifndef WIFI_SUPPORT_UNISOC_RX
                     MAIN_SetIdleWifiIcon(image);
#endif
                 }
            }
            
            if (wifi_scan_ind_ptr->ap_info.signal_qua < profile_ptr->rssi)
            {
                //SCI_TRACE_LOW:"WIFI:info:%s Not Update AP info for weak rssi, profile.rssi is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1353_112_2_18_3_9_32_49,(uint8*)"sd", __func__, profile_ptr->rssi);
                return TRUE;
            }
            else
            {
                //SCI_TRACE_LOW:"WIFI:info:%s Update AP info"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1358_112_2_18_3_9_32_50,(uint8*)"s", __func__);
            }
            
            if ((MMIWIFI_PROFILE_STATE_DISCONNECTED == profile_ptr->state) || (MMIWIFI_PROFILE_STATE_DISCONNECTING == profile_ptr->state))
            {
                profile_ptr->config_info.bssid_info      = wifi_scan_ind_ptr->ap_info.bssid_info;
                profile_ptr->config_info.channel         = wifi_scan_ind_ptr->ap_info.channel;
                profile_ptr->config_info.pairwise_cipher = wifi_scan_ind_ptr->ap_info.pairwise_cipher;
                profile_ptr->config_info.group_cipher    = wifi_scan_ind_ptr->ap_info.group_cipher;

                for (index = 0; index < MMIWIFI_MAX_WEP_NUM; index++)
                {
                    profile_ptr->is_wep_ascii[index] = TRUE;
                }
                profile_ptr->config_info.ssid = wifi_scan_ind_ptr->ap_info.ssid;
                profile_ptr->config_info.bssid_info = wifi_scan_ind_ptr->ap_info.bssid_info;
                profile_ptr->config_info.network_mode = wifi_scan_ind_ptr->ap_info.network_mode;
                profile_ptr->config_info.encryp_protocol = wifi_scan_ind_ptr->ap_info.encryp_protocol;
#if 0
                if (WIFISUPP_ENCRYP_PROTOCOL_WAPI == wifi_scan_ind_ptr->ap_info.encryp_protocol)
                {
                    if (WIFISUPP_CREDENTIAL_TYPE_PSK == wifi_scan_ind_ptr->ap_info.credential_type)
                    {
                        profile_ptr->config_info.credential.wapi_credential.credential_type = WIFISUPP_WAPI_CREDENTIAL_TYPE_PSK;
                    }
                    else
                    {
                        profile_ptr->config_info.credential.wapi_credential.credential_type = WIFISUPP_WAPI_CREDENTIAL_TYPE_CER;
                    }
                }
#endif
                profile_ptr->config_info.channel = wifi_scan_ind_ptr->ap_info.channel;
                profile_ptr->config_info.pairwise_cipher = wifi_scan_ind_ptr->ap_info.pairwise_cipher;
                profile_ptr->config_info.group_cipher = wifi_scan_ind_ptr->ap_info.group_cipher;
                profile_ptr->rssi = wifi_scan_ind_ptr->ap_info.signal_qua;
                profile_ptr->rate = wifi_scan_ind_ptr->ap_info.rate;
                profile_ptr->is_on_air = TRUE;
                if(WIFISUPP_ENCRYP_PROTOCOL_WEP == profile_ptr->config_info.encryp_protocol)
                {
                    profile_ptr->config_info.credential.wep_credential.is_open_mode = TRUE;
                }
            }
            profile_ptr->rssi = wifi_scan_ind_ptr->ap_info.signal_qua;
            profile_ptr->rate = wifi_scan_ind_ptr->ap_info.rate;
            return TRUE;
        }
        num --;
    }
#endif

    for (index = 0; index < MMIWIFI_MAX_WEP_NUM; index++)
    {
        profile.is_wep_ascii[index] = TRUE;
    }

    profile.config_info.ssid.ssid_len = wifi_scan_ind_ptr->ap_info.ssid.ssid_len;
    SCI_MEMCPY(profile.config_info.ssid.ssid, wifi_scan_ind_ptr->ap_info.ssid.ssid, profile.config_info.ssid.ssid_len);
    profile.config_info.bssid_info = wifi_scan_ind_ptr->ap_info.bssid_info;
    profile.config_info.network_mode = wifi_scan_ind_ptr->ap_info.network_mode;
    profile.config_info.encryp_protocol = wifi_scan_ind_ptr->ap_info.encryp_protocol;

    if (WIFISUPP_ENCRYP_PROTOCOL_WAPI == wifi_scan_ind_ptr->ap_info.encryp_protocol)
    {
        profile.config_info.credential.wapi_credential.credential_type = wapi_type;
    }

    profile.config_info.channel = wifi_scan_ind_ptr->ap_info.channel;
    profile.config_info.pairwise_cipher = wifi_scan_ind_ptr->ap_info.pairwise_cipher;
    profile.config_info.group_cipher = wifi_scan_ind_ptr->ap_info.group_cipher;
    profile.rssi = wifi_scan_ind_ptr->ap_info.signal_qua;
    profile.rate = wifi_scan_ind_ptr->ap_info.rate;
    profile.is_on_air = TRUE;
    profile.is_auto_connect = TRUE;

    if(WIFISUPP_ENCRYP_PROTOCOL_WEP == profile.config_info.encryp_protocol)
    {
        profile.config_info.credential.wep_credential.is_open_mode = TRUE;
    }

    //SCI_TRACE_LOW:"WIFI:info:%s New AP info added"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1440_112_2_18_3_9_32_51,(uint8*)"s", __func__);
#ifdef WLAN_PORTAL_SUPPORT
    if (MMIWIFI_APIsCMCC(&profile))
    {
        profile.is_trusted = TRUE;
        if(MMIWIFI_MAX_TRUSTED_LIST_NUM <= MMIWIFI_GetTrustedProfileNum())
        {
            MMIWIFI_DeleteLastTrustedProfile();//�����б�����ɾ�����һ��
        }

        if (MMIWIFI_InsertProfileToList(MMIWIFI_MAX_PROFILE_NUM, &profile))//��CMCC���뵽��һ��
        {
            MMIWIFI_UpdateProfileFile();
        }
        else
        {
            //SCI_TRACE_LOW:"mmiwifi error %s: AddProfileToList failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1456_112_2_18_3_9_32_52,(uint8*)"s", __func__);
            return FALSE;
        }
    }
    else
#endif
    {
        if (!MMIWIFI_AddProfileToList(&profile))
        {
            //SCI_TRACE_LOW:"mmiwifi error %s: AddProfileToList failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1465_112_2_18_3_9_32_53,(uint8*)"s", __func__);
            return FALSE;
        }
    }
    
    //check if need notify app
    if(s_mmiwifi_context.is_need_notify_find_ap)
    {
        MMIWIFI_NotifyApp(MMIWIFI_NOTIFY_EVENT_FIND_AP, &notify);
    }
    
    return TRUE;
}

/****************************************************************************/
//  Description : wifi handle supplicant msg WIFISUPP_SIG_CONNECT_CNF
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiHandleSuppSigConnectCnf(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index,WIFISUPP_CONNECT_CNF_T* wifi_connect_cnf_ptr)
{
    MMIWIFI_CONNECT_CNF_T   wifi_connect = {0};
    MMIWIFI_NOTIFY_PARAM_T  notify = {0};
    TCPIP_NETIF_IPADDR_T    ip_addr = {0};
    uint32  ret = 0;

    if((PNULL == wifi_connect_cnf_ptr) || (PNULL == profile_ptr))
    {
        return ;
    }
    
    //check if the connect wifi is that which we request connect
    if(!memcmp(profile_ptr->config_info.ssid.ssid, wifi_connect_cnf_ptr->connect_result.ssid.ssid, wifi_connect_cnf_ptr->connect_result.ssid.ssid_len))
    {
        wifi_connect.index = index;
        wifi_connect.result = (MMIWIFI_RESULT_E)(wifi_connect_cnf_ptr->connect_result.result);

        if(WIFISUPP_RESULT_SUCC == wifi_connect_cnf_ptr->connect_result.result)
        {
            profile_ptr->state = MMIWIFI_PROFILE_STATE_CONNECTED;
            memcpy(profile_ptr->config_info.bssid_info.bssid,wifi_connect_cnf_ptr->connect_result.bssid_info.bssid, WIFISUPP_BSSID_LEN);
            //profile_ptr->config_info.channel = wifi_connect_cnf_ptr->connect_result.channel;
            //profile_ptr->rssi                = wifi_connect_cnf_ptr->connect_result.rssi;
            
            //check if need dhcp
            if(MMIWIFI_IPGET_AUTO == profile_ptr->ip_get.ip_get_type)
            {
                MMIWIFI_Dhcp(profile_ptr);
            }            
            else if(MMIWIFI_IPGET_SELFDEFINED == profile_ptr->ip_get.ip_get_type)
            {
                 //set ip to tcpip
                inet_aton(profile_ptr->ip_get.ip.ip_addr,&(ip_addr.ipaddr)); 
                inet_aton(profile_ptr->ip_get.ip.subnet_mask,&(ip_addr.snmask));
                inet_aton(profile_ptr->ip_get.ip.default_gateway,&(ip_addr.gateway));
                inet_aton(profile_ptr->ip_get.ip.first_dns,&(ip_addr.dns1));
                inet_aton(profile_ptr->ip_get.ip.second_dns,&(ip_addr.dns2));
                
                //SCI_TRACE_LOW:"WIFI:info:MMI:ip address   - %s"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1522_112_2_18_3_9_32_54,(uint8*)"s", profile_ptr->ip_get.ip.ip_addr);
                //SCI_TRACE_LOW:"WIFI:info:MMI:subnet mask  - %s"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1523_112_2_18_3_9_32_55,(uint8*)"s", profile_ptr->ip_get.ip.subnet_mask);
                //SCI_TRACE_LOW:"WIFI:info:MMI:gateway      - %s"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1524_112_2_18_3_9_32_56,(uint8*)"s", profile_ptr->ip_get.ip.default_gateway);
                //SCI_TRACE_LOW:"WIFI:info:MMI:first dns    - %s"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1525_112_2_18_3_9_32_57,(uint8*)"s", profile_ptr->ip_get.ip.first_dns);
                //SCI_TRACE_LOW:"WIFI:info:MMI:second dns   - %s"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1526_112_2_18_3_9_32_58,(uint8*)"s", profile_ptr->ip_get.ip.second_dns);
                
                ret = TCPIP_SetIpAddress(&ip_addr, MMIAPIWIFI_GetNetId());
                if (ret != TCPIP_ERROR_OK)
                {
                    //SCI_TRACE_LOW:"mmiwifi error T_P_APP WifiHandleSuppSigConnectCnf: TCPIP_SetIpAddress failed(%d)!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1531_112_2_18_3_9_32_59,(uint8*)"d", ret);
                }

                WifiSetStatus(MMIWIFI_STATUS_CONNECTED);
                
#ifdef WLAN_RESELECT_SUPPORT
                MMIWIFI_OpenSelectServiceWin();
#endif

                if (!profile_ptr->is_trusted)
                {
                    if(MMIWIFI_MAX_TRUSTED_LIST_NUM > MMIWIFI_GetTrustedProfileNum())
                    {
                        profile_ptr->is_trusted= TRUE;
                        profile_ptr->is_auto_connect = TRUE;
                    }
                    else
                    {
                        //Todo: Replace one
                        ;
                    }

                    MMIWIFI_UpdateProfileFile();
                }

                if (MMIWIFI_IsConnectByAutoConnecting())
                {
                    MMIWIFI_SetAutoConnectStatus(MMIWIFI_AUTO_CONNECT_STATUS_CONNECTED);
                }
#ifdef WLAN_PORTAL_SUPPORT
                if (MMIWIFI_APIsCMCC(profile_ptr))
                {
                    MMIWIFI_CMCCAutoLogin();
                }
#endif
            }
        }
        else
        {
            profile_ptr->state = MMIWIFI_PROFILE_STATE_DISCONNECTED;
            if (profile_ptr->is_ssid_hide)
            {
                profile_ptr->is_on_air = FALSE;
            }
        }

        //ind wifi ui
        if (PNULL != s_mmiwifi_context.msg_proc_func)
        {
            s_mmiwifi_context.msg_proc_func(MMIWIFI_MSG_CONNECT_CNF,&wifi_connect,sizeof(MMIWIFI_CONNECT_CNF_T));
        }

        //notify app
        if(MMIWIFI_IPGET_AUTO != profile_ptr->ip_get.ip_get_type)
        {
            notify.result = wifi_connect_cnf_ptr->connect_result.result;
            MMIWIFI_NotifyApp(MMIWIFI_NOTIFY_EVENT_CONNECT, &notify);
        }
    }
}

/****************************************************************************/
//  Description : wifi handle supplicant msg WIFISUPP_SIG_DISCONNECT_CNF
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiHandleSuppSigDisconnectCnf(WIFISUPP_DISCONNECT_CNF_T* wifi_disconnect_cnf_ptr)
{
    MMIWIFI_DISCONNECT_CNF_T    wifi_disconnect = {0};
    MMIWIFI_NOTIFY_PARAM_T      notify = {0};
    MMIWIFI_PROFILE_T           *profile_ptr = PNULL;
    uint32                      num = 0;
    uint32                      i = 0;

    if (PNULL == wifi_disconnect_cnf_ptr)
    {
        return;
    }

    wifi_disconnect.result = (MMIWIFI_RESULT_E)(wifi_disconnect_cnf_ptr->disconnect_result.result);

    num = MMIWIFI_GetProfileNum();
    for (i = 0; i < num; i++)
    {
        profile_ptr = MMIWIFI_GetProfileByIndex(i);
        if (PNULL == profile_ptr)
        {
            //SCI_TRACE_LOW:"WIFI:error:Manager:%s profile_ptr is PNULL!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1613_112_2_18_3_9_33_60,(uint8*)"s", __func__);
            break;
        }
        
        if (MMIWIFI_PROFILE_STATE_DISCONNECTING == profile_ptr->state)
        {
            // check if the disconnect wifi is that which we request disconnect 
            if(memcmp(profile_ptr->config_info.ssid.ssid, wifi_disconnect_cnf_ptr->disconnect_result.ssid.ssid, wifi_disconnect_cnf_ptr->disconnect_result.ssid.ssid_len))
            {
                continue;
            }
            
            profile_ptr->state = MMIWIFI_PROFILE_STATE_DISCONNECTED;
            wifi_disconnect.index = i;
            if (profile_ptr->is_ssid_hide)
            {
                profile_ptr->is_on_air = FALSE;
            }
            break;
        }
    }

    if(WIFISUPP_RESULT_SUCC == wifi_disconnect_cnf_ptr->disconnect_result.result)
    {
        //change wifi status
        WifiSetStatus(MMIWIFI_STATUS_ON);
        MMIAPIPDP_WifiDeactiveInd();
        if (profile_ptr != PNULL)
        {
            if (MMIWIFI_IPGET_AUTO == profile_ptr->ip_get.ip_get_type)
            {
                SCI_MEMSET(&(profile_ptr->ip_get.ip),0,sizeof(profile_ptr->ip_get.ip));
                profile_ptr->is_dhcp_success = FALSE;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"WIFI:info:Manager:%s profile_ptr is PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1646_112_2_18_3_9_33_61,(uint8*)"s", __func__);
        }
    }

    if (s_mmiwifi_context.msg_proc_func != PNULL)
    {
        s_mmiwifi_context.msg_proc_func(MMIWIFI_MSG_DISCONNECT_CNF,  &wifi_disconnect, sizeof(MMIWIFI_DISCONNECT_CNF_T));
    }

    //notify app
    notify.result = wifi_disconnect_cnf_ptr->disconnect_result.result;
    MMIWIFI_NotifyApp(MMIWIFI_NOTIFY_EVENT_DISCONNECT, &notify);
}

/****************************************************************************/
//  Description : wifi handle supplicant msg WIFISUPP_SIG_DISCONNECT_IND
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiHandleSuppSigDisconnectInd(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index,WIFISUPP_DISCONNECT_IND_T* wifi_disconnect_ind_ptr)
{
    MMIWIFI_NOTIFY_PARAM_T notify = {0};
    
    if((PNULL == wifi_disconnect_ind_ptr) || (PNULL == profile_ptr))
    {
         return;
    }
    
    //check if the disconnect wifi is current wifi
    if(!memcmp(profile_ptr->config_info.ssid.ssid, wifi_disconnect_ind_ptr->disc_info.ssid.ssid, wifi_disconnect_ind_ptr->disc_info.ssid.ssid_len))
    {
        if (MMIWIFI_IPGET_AUTO == profile_ptr->ip_get.ip_get_type)
        {
            MMIWIFI_DisDhcp(profile_ptr);
        }
        
        WifiSetStatus(MMIWIFI_STATUS_ON);
        profile_ptr->state = MMIWIFI_PROFILE_STATE_DISCONNECTED;
        
        if (profile_ptr->is_ssid_hide)
        {
            profile_ptr->is_on_air = FALSE;
        }
//        MMIAPIPDP_WifiDeactiveInd();
        
        //ind wifi ui
        if (PNULL != s_mmiwifi_context.msg_proc_func)
        {
            s_mmiwifi_context.msg_proc_func(MMIWIFI_MSG_DISCONNECT_IND,&index,sizeof(MMIWIFI_INDEX_T));
        }

        //notify app
        notify.result = WIFISUPP_RESULT_SUCC;
        MMIWIFI_NotifyApp(MMIWIFI_NOTIFY_EVENT_DISCONNECT, &notify);
    }

    return;
}

/****************************************************************************/
//  Description : wifi handle supplicant msg WIFISUPP_SIG_BSSID_CHANGED_IND
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiHandleSuppSigBssidChandedInd(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index,WIFISUPP_BSSID_CHANGED_IND_T * wifi_bssid_changed_ind_ptr)
{
    BOOLEAN result = TRUE;

    if (PNULL == profile_ptr || PNULL == wifi_bssid_changed_ind_ptr)
    {
        result = FALSE;
    }
    else
    {
        if (!memcmp(profile_ptr->config_info.ssid.ssid,wifi_bssid_changed_ind_ptr->bssid_changed_info.ssid.ssid,wifi_bssid_changed_ind_ptr->bssid_changed_info.ssid.ssid_len))
        {
            profile_ptr->config_info.bssid_info = wifi_bssid_changed_ind_ptr->bssid_changed_info.new_bssid_info;
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
    }
    return result;
}

/****************************************************************************/
//  Description : wifi is need redraw wifi icon
//  Global resource dependence : 
//  Author:rong.gu
//  Note: �źŲ����С�Ļ������ܴ���һ���ȼ�������Ҫ�ػ�
/****************************************************************************/
LOCAL BOOLEAN MMIWIFI_IsNeedRedrawWifiIcon(int32 new_rssi, int32 old_rssi)
{
    MMI_IMAGE_ID_T image_new = 0;
    MMI_IMAGE_ID_T image_old = 0;

    if(new_rssi >= 0 || old_rssi >= 0 )
    {
        return FALSE;
    }

    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1737_112_2_18_3_9_33_62,(uint8*)"dd",new_rssi, old_rssi);

    image_new = WifiGetConnectedRssiImage(new_rssi,FALSE);
    image_old = WifiGetConnectedRssiImage(old_rssi,FALSE);
    return (image_new != image_old);
}

/****************************************************************************/
//  Description : wifi handle supplicant msg WIFISUPP_SIG_RSSI_CHANGED_IND
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiHandleSuppSigRssiChandedInd(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index,WIFISUPP_RSSI_CHANGED_IND_T * wifi_rssi_changed_ind_ptr, MMIWIFI_RSSI_CHANGED_IND_T* rssi_changed_ptr)
{
    BOOLEAN result = TRUE;

    if(PNULL == profile_ptr || PNULL == wifi_rssi_changed_ind_ptr || PNULL == rssi_changed_ptr || !(MMIWIFI_PROFILE_STATE_DHCPED == profile_ptr->state || (MMIWIFI_PROFILE_STATE_CONNECTED == profile_ptr->state && MMIWIFI_IPGET_SELFDEFINED == profile_ptr->ip_get.ip_get_type)))
    {
        result = FALSE;
    }
    else
    {
        //SCI_TRACE_LOW:"WLAN profile bssid = %s, rssi bssid = %s,ssid = %s,old_sig = %d,new_sig = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1759_112_2_18_3_9_33_63,(uint8*)"sssdd",profile_ptr->config_info.bssid_info.bssid, wifi_rssi_changed_ind_ptr->rssi_changed_info.bssid_info.bssid ,profile_ptr->config_info.ssid.ssid,profile_ptr->rssi,wifi_rssi_changed_ind_ptr->rssi_changed_info.new_signal_qua);
        if(!memcmp(profile_ptr->config_info.bssid_info.bssid,wifi_rssi_changed_ind_ptr->rssi_changed_info.bssid_info.bssid, WIFISUPP_BSSID_LEN))
        {
            //SCI_TRACE_LOW:"WLAN found connected bssid equal"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1762_112_2_18_3_9_33_64,(uint8*)"");
         
             if(MMIWIFI_IsNeedRedrawWifiIcon(wifi_rssi_changed_ind_ptr->rssi_changed_info.new_signal_qua,profile_ptr->rssi))         
             {
                 MMI_IMAGE_ID_T image = 0;
                 if(WIFISUPP_ENCRYP_PROTOCOL_OPENSYS == profile_ptr->config_info.encryp_protocol)
                 {                       
                     image = WifiGetConnectedRssiImage(wifi_rssi_changed_ind_ptr->rssi_changed_info.new_signal_qua,FALSE);
                 }
                 else
                 {
                     image = WifiGetConnectedRssiImage(wifi_rssi_changed_ind_ptr->rssi_changed_info.new_signal_qua,TRUE);                        
                 }   
#ifndef WIFI_SUPPORT_UNISOC_RX
                 MAIN_SetIdleWifiIcon(image);
#endif
             }
             else 
             {
                 return FALSE;                 
             }

            profile_ptr->rssi = wifi_rssi_changed_ind_ptr->rssi_changed_info.new_signal_qua;
            rssi_changed_ptr->index = index;
            rssi_changed_ptr->new_rssi = wifi_rssi_changed_ind_ptr->rssi_changed_info.new_signal_qua;
            result = TRUE;
        }
    }
    return result;
}

/****************************************************************************/
//  Description : wifi notify event to app
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_NotifyApp(MMIWIFI_NOTIFY_EVENT_E event_id, MMIWIFI_NOTIFY_PARAM_T* param_ptr)
{
    CFL_LIST_NODE_HANDLE node_handler = CFL_ListGetHead(s_mmiwifi_context.wifi_subscriber_list);
    MMIWIFI_NOTIFY_INFO_T *notify_info_ptr = 0;
    
    //notify all app that is subscribed
    while(node_handler!=0)
    {
        notify_info_ptr = CFL_ListGetNodeData(node_handler);
        if(PNULL != notify_info_ptr && PNULL != notify_info_ptr->notify_func)
        {
            notify_info_ptr->notify_func(event_id, param_ptr);
        }
        
        node_handler = CFL_ListGetNextNode(node_handler);
    }
}

/****************************************************************************/
//  Description : wifi find subscriber by module id
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL CFL_LIST_NODE_HANDLE WifiFindSubscriberByModuleId(uint16 module_id)
{
    CFL_LIST_NODE_HANDLE    node_found = 0;
    node_found = CFL_ListFindNode(s_mmiwifi_context.wifi_subscriber_list,module_id,WifiFindSubscriberByModuleIdCb);/*lint !e64*/
    return node_found;
}

/****************************************************************************/
//  Description : the callback of wifi find subscriber by module id
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL BOOLEAN WifiFindSubscriberByModuleIdCb(CFL_NODE_OBJECT data, uint16 module_id)
{
    if(0==data)
    {
        return FALSE;
    }
    return (((MMIWIFI_NOTIFY_INFO_T*)data)->module_id == module_id);
}
/****************************************************************************/
//  Description : handle wifi disconnected
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiDisconnected(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index, uint32 msg_id,void *msg_ptr)
{
    if (PNULL == profile_ptr || PNULL == msg_ptr)
    {
        return;
    }
    switch(msg_id) 
    {
    case WIFISUPP_SIG_RSSI_CHANGED_IND:
        {
            WIFISUPP_RSSI_CHANGED_IND_T* wifi_rssi_changed_ind_ptr = (WIFISUPP_RSSI_CHANGED_IND_T*) msg_ptr;
            MMIWIFI_RSSI_CHANGED_IND_T rssi_changed = {0};

            //SCI_TRACE_LOW:"\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1853_112_2_18_3_9_33_65,(uint8*)"");
            
            //handle wifi rssid change ind
            if(WifiHandleSuppSigRssiChandedInd(profile_ptr,index,wifi_rssi_changed_ind_ptr,& rssi_changed))
            {
                //ind wifi ui
                if (PNULL != s_mmiwifi_context.msg_proc_func)
                {
                    s_mmiwifi_context.msg_proc_func(MMIWIFI_MSG_RSSI_CHANGED_IND,& rssi_changed,sizeof(MMIWIFI_RSSI_CHANGED_IND_T));
                }
            }
        }
        
        break;
    default:
        break;
    }
}
/****************************************************************************/
//  Description : handle wifi connecting
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiConnecting(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index,uint32 msg_id, void *msg_ptr)
{
    if (PNULL == profile_ptr || PNULL == msg_ptr)
    {
        return;
    }
    switch(msg_id) 
    {
    case WIFISUPP_SIG_CONNECT_CNF:
        {
            WIFISUPP_CONNECT_CNF_T* wifi_connect_cnf_ptr = ( WIFISUPP_CONNECT_CNF_T*)msg_ptr;

            //SCI_TRACE_LOW:"\n mmiwifi dispatch WIFISUPP_SIG_CONNECT_CNF, result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1887_112_2_18_3_9_33_66,(uint8*)"d", wifi_connect_cnf_ptr->connect_result.result);
            WifiHandleSuppSigConnectCnf(profile_ptr,index,msg_ptr);
        }
        
        break;
        
    case WIFISUPP_SIG_RSSI_CHANGED_IND:
        {
            WIFISUPP_RSSI_CHANGED_IND_T* wifi_rssi_changed_ind_ptr = (WIFISUPP_RSSI_CHANGED_IND_T*) msg_ptr;
            MMIWIFI_RSSI_CHANGED_IND_T rssi_changed = {0};

            //SCI_TRACE_LOW:"\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1898_112_2_18_3_9_33_67,(uint8*)"");
            //handle wifi rssid change ind
            if(WifiHandleSuppSigRssiChandedInd(profile_ptr,index,wifi_rssi_changed_ind_ptr,& rssi_changed))
            {
                //ind wifi ui
                if (PNULL != s_mmiwifi_context.msg_proc_func)
                {
                    s_mmiwifi_context.msg_proc_func(MMIWIFI_MSG_RSSI_CHANGED_IND,& rssi_changed,sizeof(MMIWIFI_RSSI_CHANGED_IND_T));
                }
            }
        }
        
        break;
    default:
        break;
    }
}

/****************************************************************************/
//  Description : handle wifi connected
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiConnected(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index,uint32 msg_id, void *msg_ptr)
{
    if (PNULL == profile_ptr || PNULL == msg_ptr)
    {
        return;
    }
    switch(msg_id) 
    {
    case WIFISUPP_SIG_DISCONNECT_IND:
        {
            WIFISUPP_DISCONNECT_IND_T* wifi_disconnect_ind_ptr = (WIFISUPP_DISCONNECT_IND_T*) msg_ptr;

            //SCI_TRACE_LOW:"\n mmiwifi dispatch WIFISUPP_SIG_DISCONNECT_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1932_112_2_18_3_9_33_68,(uint8*)"");
            WifiHandleSuppSigDisconnectInd(profile_ptr,index,wifi_disconnect_ind_ptr);
        }
        
        break;
    case WIFISUPP_SIG_BSSID_CHANGED_IND:
        {
            WIFISUPP_BSSID_CHANGED_IND_T * wifi_bssid_changed_ind_ptr = (WIFISUPP_BSSID_CHANGED_IND_T*)msg_ptr;

            //SCI_TRACE_LOW:"\n mmiwifi dispatch WIFISUPP_SIG_BSSID_CHANGED_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1941_112_2_18_3_9_33_69,(uint8*)"");
            WifiHandleSuppSigBssidChandedInd(profile_ptr,index,wifi_bssid_changed_ind_ptr);
        }
        break;
    case WIFISUPP_SIG_RSSI_CHANGED_IND:
        {
            WIFISUPP_RSSI_CHANGED_IND_T* wifi_rssi_changed_ind_ptr = (WIFISUPP_RSSI_CHANGED_IND_T*) msg_ptr;
            MMIWIFI_RSSI_CHANGED_IND_T rssi_changed = {0};

            //SCI_TRACE_LOW:"\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1950_112_2_18_3_9_33_70,(uint8*)"");
            //handle wifi rssid change ind
            if(WifiHandleSuppSigRssiChandedInd(profile_ptr,index,wifi_rssi_changed_ind_ptr,& rssi_changed))
            {
                //ind wifi ui
                if (PNULL != s_mmiwifi_context.msg_proc_func)
                {
                    s_mmiwifi_context.msg_proc_func(MMIWIFI_MSG_RSSI_CHANGED_IND,& rssi_changed,sizeof(MMIWIFI_RSSI_CHANGED_IND_T));
                }
            }
        }
        
        break;
    default:
        break;
    }
}
/****************************************************************************/
//  Description : handle wifi dhcping
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiDhcping(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index,uint32 msg_id, void *msg_ptr)
{
    if (PNULL == profile_ptr || PNULL == msg_ptr)
    {
        return;
    }
    switch(msg_id) 
    {
    case WIFISUPP_SIG_DISCONNECT_IND:
        {
            WIFISUPP_DISCONNECT_IND_T* wifi_disconnect_ind_ptr = (WIFISUPP_DISCONNECT_IND_T*) msg_ptr;

            //SCI_TRACE_LOW:"\n mmiwifi dispatch WIFISUPP_SIG_DISCONNECT_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1983_112_2_18_3_9_33_71,(uint8*)"");
            WifiHandleSuppSigDisconnectInd(profile_ptr,index,wifi_disconnect_ind_ptr);
        }
        
        break;
    case WIFISUPP_SIG_BSSID_CHANGED_IND:
        {
            WIFISUPP_BSSID_CHANGED_IND_T * wifi_bssid_changed_ind_ptr = (WIFISUPP_BSSID_CHANGED_IND_T*)msg_ptr;

            //SCI_TRACE_LOW:"\n mmiwifi dispatch WIFISUPP_SIG_BSSID_CHANGED_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_1992_112_2_18_3_9_33_72,(uint8*)"");
            WifiHandleSuppSigBssidChandedInd(profile_ptr,index,wifi_bssid_changed_ind_ptr);
        }
        break;
    case WIFISUPP_SIG_RSSI_CHANGED_IND:
        {
            WIFISUPP_RSSI_CHANGED_IND_T* wifi_rssi_changed_ind_ptr = (WIFISUPP_RSSI_CHANGED_IND_T*) msg_ptr;
            MMIWIFI_RSSI_CHANGED_IND_T rssi_changed = {0};

            //SCI_TRACE_LOW:"\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2001_112_2_18_3_9_33_73,(uint8*)"");
            //handle wifi rssid change ind
            if(WifiHandleSuppSigRssiChandedInd(profile_ptr,index,wifi_rssi_changed_ind_ptr,& rssi_changed))
            {
                //ind wifi ui
                if (PNULL != s_mmiwifi_context.msg_proc_func)
                {
                    s_mmiwifi_context.msg_proc_func(MMIWIFI_MSG_RSSI_CHANGED_IND,& rssi_changed,sizeof(MMIWIFI_RSSI_CHANGED_IND_T));
                }
            }
        }
        
        break;
    default:
        break;
    }
}

/****************************************************************************/
//  Description : handle wifi dhcped
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiDhcped(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index,uint32 msg_id, void *msg_ptr)
{
    if (PNULL == profile_ptr || PNULL == msg_ptr)
    {
        return;
    }
    switch(msg_id) 
    {
    case WIFISUPP_SIG_DISCONNECT_IND:
        {
            WIFISUPP_DISCONNECT_IND_T* wifi_disconnect_ind_ptr = (WIFISUPP_DISCONNECT_IND_T*) msg_ptr;

            //SCI_TRACE_LOW:"\n mmiwifi dispatch WIFISUPP_SIG_DISCONNECT_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2035_112_2_18_3_9_33_74,(uint8*)"");
            WifiHandleSuppSigDisconnectInd(profile_ptr,index,wifi_disconnect_ind_ptr);
        }
        
        break;
    case WIFISUPP_SIG_BSSID_CHANGED_IND:
        {
            WIFISUPP_BSSID_CHANGED_IND_T * wifi_bssid_changed_ind_ptr = (WIFISUPP_BSSID_CHANGED_IND_T*)msg_ptr;

            //SCI_TRACE_LOW:"\n mmiwifi dispatch WIFISUPP_SIG_BSSID_CHANGED_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2044_112_2_18_3_9_34_75,(uint8*)"");
            WifiHandleSuppSigBssidChandedInd(profile_ptr,index,wifi_bssid_changed_ind_ptr);
        }
        break;
    case WIFISUPP_SIG_RSSI_CHANGED_IND:
        {
            WIFISUPP_RSSI_CHANGED_IND_T* wifi_rssi_changed_ind_ptr = (WIFISUPP_RSSI_CHANGED_IND_T*) msg_ptr;
            MMIWIFI_RSSI_CHANGED_IND_T rssi_changed = {0};

            //SCI_TRACE_LOW:"\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2053_112_2_18_3_9_34_76,(uint8*)"");
            //handle wifi rssid change ind
            if(WifiHandleSuppSigRssiChandedInd(profile_ptr,index,wifi_rssi_changed_ind_ptr,& rssi_changed))
            {
                //ind wifi ui
                if (PNULL != s_mmiwifi_context.msg_proc_func)
                {
                    s_mmiwifi_context.msg_proc_func(MMIWIFI_MSG_RSSI_CHANGED_IND,& rssi_changed,sizeof(MMIWIFI_RSSI_CHANGED_IND_T));
                }
            }
        }
        
        break;
    default:
        break;
    }
}

/****************************************************************************/
//  Description : handle wifi disconnecting
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
LOCAL void WifiDisconnecting(MMIWIFI_PROFILE_T* profile_ptr,MMIWIFI_INDEX_T index,uint32 msg_id, void *msg_ptr)
{
    if (PNULL == profile_ptr || PNULL == msg_ptr)
    {
        return;
    }
    switch(msg_id) 
    {
    case WIFISUPP_SIG_DISCONNECT_IND:
        {
            WIFISUPP_DISCONNECT_IND_T* wifi_disconnect_ind_ptr = (WIFISUPP_DISCONNECT_IND_T*) msg_ptr;
            
            //SCI_TRACE_LOW:"\n mmiwifi dispatch WIFISUPP_SIG_DISCONNECT_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2087_112_2_18_3_9_34_77,(uint8*)"");
            WifiHandleSuppSigDisconnectInd(profile_ptr,index,wifi_disconnect_ind_ptr);
        }
        break;

    case WIFISUPP_SIG_RSSI_CHANGED_IND:
        {
            WIFISUPP_RSSI_CHANGED_IND_T* wifi_rssi_changed_ind_ptr = (WIFISUPP_RSSI_CHANGED_IND_T*) msg_ptr;
            MMIWIFI_RSSI_CHANGED_IND_T rssi_changed = {0};
            
            //SCI_TRACE_LOW:"\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2097_112_2_18_3_9_34_78,(uint8*)"");

            //handle wifi rssid change ind
            if(WifiHandleSuppSigRssiChandedInd(profile_ptr,index,wifi_rssi_changed_ind_ptr,& rssi_changed))
            {
                //ind wifi ui
                if (PNULL != s_mmiwifi_context.msg_proc_func)
                {
                    s_mmiwifi_context.msg_proc_func(MMIWIFI_MSG_RSSI_CHANGED_IND,& rssi_changed,sizeof(MMIWIFI_RSSI_CHANGED_IND_T));
                }
            }
        }
        break;

    default:
        break;
    }
}


/****************************************************************************/
//  Description : set wifi is prompt
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_SetPromptResult(BOOLEAN is_prompt_ok)
{
    s_mmiwifi_context.is_prompt_ok = is_prompt_ok;
}

/****************************************************************************/
//  Description : get wifi is prompt
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetPromptResult(void)
{
    return s_mmiwifi_context.is_prompt_ok;
}


/****************************************************************************/
//  Description : set wifi is initialized
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_SetIsInitialized(BOOLEAN is_initialized)
{
    s_mmiwifi_context.is_initialized = is_initialized;
}

/****************************************************************************/
//  Description : get wifi is initialized
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetIsInitialized(void)
{
    return s_mmiwifi_context.is_initialized;
}

/****************************************************************************/
//  Description : Set wifi  auto connect status
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_SetAutoConnectStatus(MMIWIFI_AUTO_CONNECT_STATUS_E auto_connect_status)
{
    s_mmiwifi_context.auto_connect_status = auto_connect_status;
}

/****************************************************************************/
//  Description : Get wifi auto connect status
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC MMIWIFI_AUTO_CONNECT_STATUS_E MMIWIFI_GetAutoConnectStatus(void)
{
    return s_mmiwifi_context.auto_connect_status;
}

/****************************************************************************/
//  Description : Set wifi spicified scan status
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_SetScanStatus(BOOLEAN is_scanning)
{
    if (s_mmiwifi_context.is_scanning!= is_scanning)
    {
        s_mmiwifi_context.is_scanning = is_scanning;
    }
}

/****************************************************************************/
//  Description : Set wifi  scan status
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetScanStatus(void)
{
    return s_mmiwifi_context.is_scanning;
}

/****************************************************************************/
//  Description : Set wifi spicified scan status
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_SetSpecifiedScanStatus(BOOLEAN is_specified)
{
    if (s_mmiwifi_context.is_specified != is_specified)
    {
        s_mmiwifi_context.is_specified = is_specified;
    }
}

/****************************************************************************/
//  Description : Set wifi spicified scan status
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetSpecifiedScanStatus(void)
{
    return s_mmiwifi_context.is_specified;
}

/****************************************************************************/
//  Description : Check if the connect is started by auto
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC  BOOLEAN MMIWIFI_IsConnectByAutoConnecting(void)
{
    return MMIWIFI_AUTO_CONNECT_STATUS_CONNECTING == MMIWIFI_GetAutoConnectStatus();
}

/****************************************************************************/
//  Description : Auto connect function
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_AutoConnect(void)
{
    uint32 index = 0;
    uint32 num = 0;
    static uint32 from_index = 0;
    MMIWIFI_PROFILE_T* profile_ptr = PNULL;

    //SCI_TRACE_LOW:"MMIWIFI_AutoConnect "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2234_112_2_18_3_9_34_79,(uint8*)"");

    if (MMIAPISET_GetFlyMode() || !MMIWIFI_GetIsWlanOn()|| !MMIWIFI_GetIsAutoConnect())
    {
        //SCI_TRACE_LOW:"MMIWIFI_AutoConnect : Fly Mode or OFF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2238_112_2_18_3_9_34_80,(uint8*)"");
        return FALSE;
    }

    if (!MMIWIFI_GetIsWlanOffReqHandled())
    {
        SCI_TRACE_LOW("WIFI is shuting down");
        return FALSE;
    }

    num = MMIWIFI_GetProfileNum();

    //SCI_TRACE_LOW:"Index Name   ON_AIR TRUSTED STATUS AUTO_CON"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2244_112_2_18_3_9_34_81,(uint8*)"");

    for (index = 0; index < num; index ++)
    {
        profile_ptr = MMIWIFI_GetProfileByIndex(index);
        if (PNULL != profile_ptr)
        {
            //SCI_TRACE_LOW:"%d    %s    %d    %d    %d    %d  "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2251_112_2_18_3_9_34_82,(uint8*)"dsdddd", index, profile_ptr->config_info.ssid.ssid, profile_ptr->is_on_air, profile_ptr->is_trusted, profile_ptr->state, profile_ptr->is_auto_connect);
        }
    }

    for (index = 0 ; index < num; index++)
    {
        profile_ptr = MMIWIFI_GetProfileByIndex(index);
        if (PNULL != profile_ptr && MMIWIFI_PROFILE_STATE_DISCONNECTING != profile_ptr->state && MMIWIFI_PROFILE_STATE_DISCONNECTED != profile_ptr->state)
        {
            //SCI_TRACE_LOW:"MMIWIFI_AutoConnect %s is connecting or connected"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2260_112_2_18_3_9_34_83,(uint8*)"s", profile_ptr->config_info.ssid.ssid);
            return FALSE;
        }
    }

    index = 0;

    if (!MMIWIFI_IsConnectByAutoConnecting())
    {
        from_index = 0;
    }

    if (PNULL != profile_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIFI_AutoConnect %s is connecting or connected from_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2274_112_2_18_3_9_34_84,(uint8*)"sd", profile_ptr->config_info.ssid.ssid, from_index);
    }

    for (index = from_index ; index < num; index++)
    {
        profile_ptr = MMIWIFI_GetProfileByIndex(index);

        if (PNULL != profile_ptr && profile_ptr->is_trusted && (profile_ptr->is_on_air || profile_ptr->is_ssid_hide) && profile_ptr->is_auto_connect)
        {
#ifdef WLAN_EAP_SUPPORT
            if(WIFISUPP_ENCRYP_PROTOCOL_EAP == profile_ptr->config_info.encryp_protocol)
            {
                if (!MMIAPIPHONE_IsSimAvailable(profile_ptr->config_info.credential.wpa_credential.credential_info.wpa_eap_info.eap_info.eapsim_info.simcard_num))
                {
                    continue;
                }
            }
#endif /* WLAN_EAP_SUPPORT */
            from_index = index + 1; /* Connect next index next time */
            MMIWIFI_SetCurrProfile(profile_ptr);
            (void)MMIWIFI_Connect(profile_ptr);
            MMIWIFI_SetAutoConnectStatus(MMIWIFI_AUTO_CONNECT_STATUS_CONNECTING);
            //SCI_TRACE_LOW:"MMIWIFI_AutoConnect Connect -> %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2297_112_2_18_3_9_34_85,(uint8*)"s", profile_ptr->config_info.ssid.ssid);

            profile_ptr->state = MMIWIFI_PROFILE_STATE_CONNECTING;
            return TRUE;
        }
        from_index++;
    }

    from_index = 0;
    MMIWIFI_SetAutoConnectStatus(MMIWIFI_AUTO_CONNECT_STATUS_NONE);

    return FALSE;
}

/****************************************************************************/
//  Description : Start AutoScan
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_StartAutoScan(void)
{
    MMIWIFI_AUTOSCAN_STATUS_E autoscan_status = 0;

    if (!MMIWIFI_GetIsWlanOn() || !MMIWIFI_GetWlanAutoScanStatus())
    {
        //SCI_TRACE_LOW:"MMIWIFI_StartAutoScan Wlan Or AutoScan OFF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2321_112_2_18_3_9_34_86,(uint8*)"");
        return FALSE;
    }

/*
    if(MMIAPIWIFI_IsChildListWinFocus())
    {
        autoscan_status = MMIWIIF_AUTOSCAN_PERIOD_TEN_SECONDS;
    }
    else
*/
#ifdef MMI_WLAN_TAB_STYLE
    {

        switch(MMIWIFI_GetWlanAutoScanStatus())
        {
            case 1:
                autoscan_status = MMIWIFI_AUTOSCAN_PERIOD_ONE_MINUTE;
                break;
            case 2:
                autoscan_status = MMIWIFI_AUTOSCAN_PERIOD_TWO_MINUTES;
                break;
            case 3:
                autoscan_status = MMIWIFI_AUTOSCAN_PERIOD_FIVE_MINUTES;
                break;
            case 4:
                autoscan_status = MMIWIFI_AUTOSCAN_PERIOD_TEN_MINUTES;
                break;
            default:
                break;
        }
    }
#else
    autoscan_status = MMIWIIF_AUTOSCAN_PERIOD_TEN_SECONDS;
#endif /* MMI_WLAN_TAB_STYLE */

    //SCI_TRACE_LOW:"MMIWIFI_StartAutoScan autoscan_status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2356_112_2_18_3_9_34_87,(uint8*)"d", autoscan_status);

     if(s_auto_scan_timer_id  != 0)
     {     
         MMK_StopTimer(s_auto_scan_timer_id);  
         s_auto_scan_timer_id = 0;
     }
     
     return MMIWIFI_AutoScanByTimer(autoscan_status);

}

/****************************************************************************/
//  Description : Stop AutoScan
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_StopAutoScan(void)
{
    //SCI_TRACE_LOW:"MMIWIFI_StopAutoScan"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2374_112_2_18_3_9_34_88,(uint8*)"");

    if(s_auto_scan_timer_id  != 0)
    {
        MMK_StopTimer(s_auto_scan_timer_id);  
        s_auto_scan_timer_id = 0;
    }
    
    if (!MMIWIFI_GetIsWlanOn() || !MMIWIFI_GetWlanAutoScanStatus())
    {
        //SCI_TRACE_LOW:"MMIWIFI_StartAutoScan Wlan Or AutoScan OFF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2384_112_2_18_3_9_34_89,(uint8*)"");
        return TRUE;
    }

    return TRUE;
    
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/****************************************************************************/
LOCAL BOOLEAN WifiIsConnecting(void)
{
    uint32                  num = 0;
    BOOLEAN                 ret = FALSE;
    MMIWIFI_PROFILE_T       *profile_ptr = NULL;

    num = MMIWIFI_GetProfileNum();

    while(num != 0)
    {
        profile_ptr = MMIWIFI_GetProfileByIndex(num - 1);

        if(PNULL != profile_ptr && (profile_ptr->state == MMIWIFI_PROFILE_STATE_CONNECTING || profile_ptr->state == MMIWIFI_PROFILE_STATE_DHCPING))
        {
            ret = TRUE;
            break;
        }

        num --;
    }

    //SCI_TRACE_LOW:"MMIWIFI_IsConnecting ret = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_MANAGER_2417_112_2_18_3_9_34_90,(uint8*)"d", ret);
    
    return ret;
}

/****************************************************************************/
//  Description :MMIWIFI_SetIsWlanOffReqHandled
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/****************************************************************************/
PUBLIC void MMIWIFI_SetIsWlanOffReqHandled(BOOLEAN is_handled)
{
    s_mmiwifi_context.is_off_req_handled = is_handled;
}

/*****************************************************************************/
//  Description : MMIWIFI_GetIsWlanOffReqHandled
//  Global resource dependence : 
//  Author: 
//  Note: 0Ϊ�رգ�1Ϊ��
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetIsWlanOffReqHandled(void)
{
    return s_mmiwifi_context.is_off_req_handled;
}

