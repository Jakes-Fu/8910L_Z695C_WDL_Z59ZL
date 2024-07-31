/****************************************************************************
** File Name:      mmi_ping.c                                             *
** Author:                                                                      *
** Date:           09/2020                                              *
** Description:     contactsync interact with FTP.                            *

****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifdef MMIENG_PING_SUPPORT
#include "os_api.h"
#include "priority_app.h"
#include "mmipdp_export.h"
#include "mmiphone_export.h"
#include "mmiping_text.h"
#include "tcpip_types.h"
#include "app_tcp_if.h"
#include "in_message.h"
#include "mmipub.h"
#include "mmiping_internal.h"
#include "mmiping_id.h"

#define TIMERMSG_LONG_PING_TIME_OUT (1000)

static uint32 ping_count = 0;
static uint32 ping_times = 0;
static uint32 ping_fail = 0;
static uint32 s_ping_net_id = 0;
LOCAL BOOLEAN  s_is_manual_quit = FALSE;
LOCAL RES_PING_STR g_mmi_ping_rsp_str = {0};
LOCAL MMIPDP_RESULT_E g_ping_pdp_result = MMIPDP_RESULT_SUCC;

LOCAL uint8 s_mmiping_timer_id = 0;
LOCAL void MMIPING_PingCallBack(TCPIP_PING_RESULT_E     res,            /* ping result, 0 - succeed; other - fail */
                                uint32                  time_delay,     /* ping time delay, only valid when success, unit: ms */
                                TCPIP_PING_HANDLE       ping_handle,  /* ping handle */
                                uint8                   ttl,
                                char*                   ipaddr);
PUBLIC BOOLEAN MMIAPIPING_GetIsManualQuit(void)
{
    SCI_TraceLow("[PING]:MMIAPIPING_GetIsManualQuit s_is_manual_quit=%d", s_is_manual_quit);
    return s_is_manual_quit;
}
PUBLIC void MMIAPIPING_SetIsManualQuit(BOOLEAN is_manual_quit)
{
    s_is_manual_quit = is_manual_quit;
    SCI_TraceLow("[PING]:MMIAPIPING_SetIsManualQuit s_is_manual_quit=%d", s_is_manual_quit);
}
LOCAL void HandleLongPingTimerMsgTimeout(uint8 timer_id, uint32 param)
{
    MMIPING_SETTING_INFO_T* p_setting_info = PNULL;
    
    p_setting_info = MMIAPIPING_GetSettingInfoBySIMID();
    if (PNULL != p_setting_info)
    {
        if (MMICONNECTION_IP_V4 == p_setting_info->ip_type)
        {
            sci_ping4_request(p_setting_info->homepage, 0, 8000, 1, MMIPING_PingCallBack, s_ping_net_id);
        }
        else if (MMICONNECTION_IP_V6 == p_setting_info->ip_type)
        {
            sci_ping6_request(p_setting_info->homepage, 0, 8000, 1, MMIPING_PingCallBack, s_ping_net_id);
        }
        else
        {
            sci_ping_request_ext(p_setting_info->homepage, 0, 8000, 1, MMIPING_PingCallBack, s_ping_net_id);
        }
    }
    SCI_TraceLow("[PING]:HandleLongPingTimerMsgTimeout s_mmiping_timer_id=%d,s_is_manual_quit=%d",s_mmiping_timer_id, s_is_manual_quit);
    
    if (0 != s_mmiping_timer_id && s_mmiping_timer_id == timer_id)
    {
        MMK_StopTimer(s_mmiping_timer_id);        
        s_mmiping_timer_id = 0;
    }

#ifndef WIN32
    SCI_TraceLow("[PING]:HandleLongPingTimerMsgTimeout");
#endif
}
PUBLIC uint8 MMIAPIPING_GetLongPingTimerID(void)
{
    return s_mmiping_timer_id;
}
PUBLIC void MMIAPIPING_SetLongPingTimerID(uint8 timer_id)
{
    s_mmiping_timer_id = timer_id;
}

PUBLIC BOOLEAN MMIAPIPING_StartLongPingTimer(void)
{
    if (0 != s_mmiping_timer_id)
    {
        MMK_StopTimer(s_mmiping_timer_id);

        s_mmiping_timer_id = 0;
    }

    s_mmiping_timer_id = MMK_CreateTimerCallback(TIMERMSG_LONG_PING_TIME_OUT, HandleLongPingTimerMsgTimeout, NULL, FALSE);
    return TRUE;
}

PUBLIC RES_PING_STR* MMIPING_GetDisplayString(void)
{
    return &g_mmi_ping_rsp_str;
}
LOCAL void MMIPING_PingCallBack(TCPIP_PING_RESULT_E     res,            /* ping result, 0 - succeed; other - fail */
                                uint32                  time_delay,     /* ping time delay, only valid when success, unit: ms */
                                TCPIP_PING_HANDLE       ping_handle,  /* ping handle */
                                uint8                   ttl,
                                char*                   ipaddr)
{
    MMIPING_SETTING_INFO_T* p_setting_info = PNULL;
    MMIPING_SELECT_MODE_E select_mode = MMIPING_SELECT_NORMAL;
    
    //SCI_MEMSET(g_mmi_ping_rsp_str, 0, sizeof(g_mmi_ping_rsp_str));
    select_mode = MMIAPIPING_GetSelectPingMode();
    
    SCI_TraceLow("[PING] MMIPING_PingCallBack res = %d  ", res);
    if (MMIPING_SELECT_NORMAL == select_mode)
    {
    if (PINGRES_SUCCESS != res)
    {
        if (PINGRES_DNS_TIMEOUT == res)
        {
             ping_count = 0;
             ping_times = 0;
             ping_fail = 0;
             if (MMK_IsOpenWin(MMIPING_WAIT_WIN_ID))
             {
                 MMK_CloseWin(MMIPING_WAIT_WIN_ID);
             }
             MMIPUB_OpenAlertWarningWin(TXT_COMMON_NETWORK_CONNECT_FAIL);
             MMIAPIPDP_Deactive(MMI_MODULE_PING);
             return;
        }
        SCI_TraceLow("[PING] MMIPING_PingCallBack ping error!!!! res is %d", res);
        ping_fail++;
    }
    else
    {
        ping_count++;
        SCI_TraceLow("[PING] MMIPING_PingCallBack ping time_delay %d, g_mmi_ping_rsp_str %s, strlen = %d", 
                     time_delay, g_mmi_ping_rsp_str, strlen(g_mmi_ping_rsp_str));
        SCI_TraceLow("[PING] MMIPING_PingCallBack ttl %d ttl    ipaddr %s", ttl, ipaddr);
        //sprintf((char *)g_mmi_ping_rsp_str + strlen(g_mmi_ping_rsp_str), "PING:from:%s response time_delay=%d  ttl=%d\n", ipaddr, time_delay, ttl);
    }
    
    p_setting_info = MMIAPIPING_GetSettingInfoBySIMID();
    if (PNULL != p_setting_info)
    {
        ping_times = p_setting_info->count;
    }
    
    if ((ping_count + ping_fail) == ping_times)
    {
        sprintf((char *)g_mmi_ping_rsp_str, "PING ipaddr [%s] Finish, Packets: Sent=%d, Received=%d,Lost=%d\n", ipaddr, ping_times, ping_count, ping_fail);
        SCI_TraceLow("[PING] MMIPING_PingCallBack ping ping_count = %d, ping_times = %d, ping_fail=%d", ping_count, ping_times, ping_fail);
        SCI_TraceLow("[PING] MMIPING_PingCallBack g_mmi_ping_rsp_str %s, strlen = %d", 
                     g_mmi_ping_rsp_str, strlen(g_mmi_ping_rsp_str));

        if (MMK_IsOpenWin(MMIPING_WAIT_WIN_ID))
        {
            MMK_CloseWin(MMIPING_WAIT_WIN_ID);
        }
        
        if (!MMIAPIPING_IsPacketShowWinOpened())
        {
            MMIAPIENG_CreateShowPacketInfoWin();
        }
        MMIAPIPDP_Deactive(MMI_MODULE_PING);
        ping_count = 0;
        ping_times = 0;
        ping_fail = 0;
    }
    }
    else
    {
        SCI_MEMSET(g_mmi_ping_rsp_str, 0, sizeof(g_mmi_ping_rsp_str));
        if (PINGRES_SUCCESS != res)
        {
            if (PINGRES_DNS_TIMEOUT == res)
            {
                 ping_count = 0;
                 ping_times = 0;
                 ping_fail = 0;
                 if (MMK_IsOpenWin(MMIPING_WAIT_WIN_ID))
                 {
                     MMK_CloseWin(MMIPING_WAIT_WIN_ID);
                 }
                 MMIPUB_OpenAlertWarningWin(TXT_COMMON_NETWORK_CONNECT_FAIL);
                 MMIAPIPDP_Deactive(MMI_MODULE_PING);
                 return;
            }
            SCI_TraceLow("[PING] MMIPING_PingCallBack ping error!!!! res is %d", res);
            sprintf((char *)g_mmi_ping_rsp_str, "PING from:%s response: Time out! time_delay=%dms", ipaddr, time_delay);
            ping_fail++;
        }
        else
        {
            ping_count++;
            sprintf((char *)g_mmi_ping_rsp_str, "PING from:%s response: Byte=%d  time_delay=%dms  ttl=%d\n", ipaddr, 32, time_delay, ttl);
            SCI_TraceLow("[PING] MMIPING_PingCallBack ttl %d, ipaddr %s", ttl, ipaddr);
        }
        ping_times ++;
        
        SCI_TraceLow("[PING] MMIPING_PingCallBack ping time_delay %d, g_mmi_ping_rsp_str %s, strlen = %d", 
                     time_delay, g_mmi_ping_rsp_str, strlen(g_mmi_ping_rsp_str));
        if (TRUE == s_is_manual_quit)
        {
            SCI_MEMSET(g_mmi_ping_rsp_str, 0, sizeof(g_mmi_ping_rsp_str));
            sprintf((char *)g_mmi_ping_rsp_str, "PING ipaddr [%s] Finish, Packets: Sent=%d, Received=%d, Lost=%d\n", ipaddr, ping_times, ping_count, ping_fail);
            MMIAPIPDP_Deactive(MMI_MODULE_PING);
            ping_count = 0;
            ping_times = 0;
            ping_fail = 0;
        }
        else
        {
            MMIAPIPING_StartLongPingTimer();
        }
        
        if (MMK_IsOpenWin(MMIPING_WAIT_WIN_ID))
        {
            MMK_CloseWin(MMIPING_WAIT_WIN_ID);
        }
        
        if (!MMIAPIPING_IsPacketShowWinOpened())
        {
            MMIAPIENG_CreateShowPacketInfoWin();
        }
        else
        {
            MMIAPIPING_UpdatePacketShowWin();
        }

    }

    return;
}

LOCAL void MMIPING_StartPingReq()
{
    MMIPING_SETTING_INFO_T* p_setting_info = PNULL;
    MMIPING_SELECT_MODE_E select_mode = MMIPING_SELECT_NORMAL;

    ping_count = 0;
    ping_times = 0;
    ping_fail = 0;
    SCI_MEMSET(g_mmi_ping_rsp_str, 0, sizeof(g_mmi_ping_rsp_str));
    
    select_mode = MMIAPIPING_GetSelectPingMode();
    if (MMIPING_SELECT_NORMAL == select_mode)
    {
        p_setting_info = MMIAPIPING_GetSettingInfoBySIMID();
        if (PNULL != p_setting_info)
        {
            if (MMICONNECTION_IP_V4 == p_setting_info->ip_type)
            {
                sci_ping4_request(p_setting_info->homepage, 0, 8000, p_setting_info->count, MMIPING_PingCallBack, s_ping_net_id);
            }
            else if (MMICONNECTION_IP_V6 == p_setting_info->ip_type)
            {
                sci_ping6_request(p_setting_info->homepage, 0, 8000, p_setting_info->count, MMIPING_PingCallBack, s_ping_net_id);
            }
            else
            {
                sci_ping_request_ext(p_setting_info->homepage, 0, 8000, p_setting_info->count, MMIPING_PingCallBack, s_ping_net_id);
            }
        }
    }
    else 
    {
        MMIAPIPING_StartLongPingTimer();
    }
    //open information window to display ping information
}
LOCAL void MMIPING_HandlePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if (PNULL == msg_ptr) 
    {
        return;
    }

    SCI_TRACE_LOW("[PING]:MMIPBSYNC_HandlePdpMsg--- msg_id=%d, result=%d", msg_ptr->msg_id, msg_ptr->result);

    switch (msg_ptr->msg_id) 
    {
        case MMIPDP_ACTIVE_CNF:     
            SCI_TRACE_LOW("[PING]:HandlePdpMsg--- MMIPDP_ACTIVE_CNF");
            g_ping_pdp_result = msg_ptr->result;
            if ((MMIPDP_RESULT_SUCC == msg_ptr->result) && (FALSE == s_is_manual_quit)) 
            {
                s_ping_net_id = msg_ptr->nsapi;
                MMIPING_StartPingReq();
                SCI_TRACE_LOW("[PING]:HandlePdpMsg  MMIPDP_RESULT_SUCC g_pbsync_nsapi %d ", s_ping_net_id);
            } 
            else if (MMIPDP_RESULT_FDN_NOT_PERMIT == msg_ptr->result)
            {
                if (MMK_IsOpenWin(MMIPING_WAIT_WIN_ID))
                {
                    MMK_CloseWin(MMIPING_WAIT_WIN_ID);
                }
                MMIPUB_OpenAlertWarningWin(TXT_CC_FDN_ONLY);
                SCI_TRACE_LOW("[PING]:HandlePdpMsg FDN ONLY");
                MMIAPIPDP_Deactive(MMI_MODULE_PING);
            }
            else if (MMIPDP_RESULT_FAIL == msg_ptr->result) 
            {
                if (MMK_IsOpenWin(MMIPING_WAIT_WIN_ID))
                {
                    MMK_CloseWin(MMIPING_WAIT_WIN_ID);
                }
                MMIAPIPDP_Deactive(MMI_MODULE_PING);
                SCI_TRACE_LOW("[PING]:HandlePdpMsg active fail!");
            } 
            else
            {
                if (MMK_IsOpenWin(MMIPING_WAIT_WIN_ID))
                {
                    MMK_CloseWin(MMIPING_WAIT_WIN_ID);
                }
                MMIAPIPDP_Deactive(MMI_MODULE_PING);
            }
            break;

        case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功。
            SCI_TRACE_LOW("[PING]:HandlePdpMsg--- MMIPDP_DEACTIVE_CNF");
            if (MMK_IsOpenWin(MMIPING_WAIT_WIN_ID))
            {
                MMK_CloseWin(MMIPING_WAIT_WIN_ID);
            }
            s_ping_net_id = 0;
            break;

        case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活。
            SCI_TRACE_LOW("[PING]:HandlePdpMsg--- MMIPDP_DEACTIVE_IND");
            if (MMK_IsOpenWin(MMIPING_WAIT_WIN_ID))
            {
                MMK_CloseWin(MMIPING_WAIT_WIN_ID);
            }
            s_ping_net_id = 0;
            MMIAPIPDP_Deactive(MMI_MODULE_PING);
            break;

        default:
            break;
    }
    return;
}

LOCAL MMI_RESULT_E HandlePingModeWaitWindow(
                                               MMI_WIN_ID_T          win_id,     // 绐楀彛鐨処D
                                               MMI_MESSAGE_ID_E      msg_id,     // 绐楀彛鐨勫唴閮ㄦ秷鎭疘D
                                               DPARAM                param       // 鐩稿簲娑堟伅鐨勫弬鏁?
                                               )
{
    MMI_RESULT_E    result  =   MMI_RESULT_TRUE;
    uint8 timer_id = 0;
    
    switch (msg_id)
    {
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        if (MMIPING_SELECT_LONG_MODE == MMIAPIPING_GetSelectPingMode())
        {
            s_is_manual_quit = TRUE;
            timer_id = MMIAPIPING_GetLongPingTimerID();
            if (0 != timer_id)
            {
                MMK_StopTimer(timer_id);
                MMIAPIPING_SetLongPingTimerID(timer_id);
            }
        }
        break;

    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }

    return (result);
}

PUBLIC BOOLEAN MMIAPIPING_ActivePDP(MN_DUAL_SYS_E dual_sys)
{

    uint8 index = 0;
    BOOLEAN return_val = FALSE;
    MMI_STRING_T wait_text = {0};
    MMIPDP_ACTIVE_INFO_T active_info = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T *setting_item_ptr = PNULL;
    MMICONNECTION_APNTYPEINDEX_T *apntype_index = PNULL;

    g_ping_pdp_result = MMIPDP_RESULT_SUCC;
    MMIAPIPING_SetIsManualQuit(FALSE);
    apntype_index = MMIAPICONNECTION_GetApnTypeIndex(dual_sys);
    if ((PNULL != apntype_index) && (TRUE == apntype_index->internet_index[dual_sys].is_valid)) 
    {
        index = apntype_index->internet_index[dual_sys].index;
    }
    setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);
    if (setting_item_ptr != PNULL) 
    {
        active_info.app_handler = MMI_MODULE_PING;
        active_info.apn_ptr = ( char * )setting_item_ptr->apn;
        active_info.user_name_ptr = ( char * )setting_item_ptr->username;
        active_info.psw_ptr = ( char * )setting_item_ptr->password;
        active_info.dual_sys = dual_sys;
        active_info.priority = 3;
        active_info.ps_service_rat = MN_TD_PREFER;
        active_info.handle_msg_callback = MMIPING_HandlePdpMsg;
        active_info.ps_service_type = BROWSER_E;
        active_info.storage = MN_GPRS_STORAGE_ALL;

#ifdef IPVERSION_SUPPORT_V4_V6
        active_info.ip_type = setting_item_ptr->ip_type;
#endif
        if (MMIAPIPDP_Active(&active_info)) 
        {
            if ((MMIPDP_RESULT_FDN_NOT_PERMIT != g_ping_pdp_result) &&
            #ifdef DATA_ROAMING_SUPPORT
                (MMIPDP_RESULT_NOT_PERMIT_ROAMING != g_ping_pdp_result) &&
            #endif
            #ifndef GPRSMPDP_SUPPORT
                (MMIPDP_RESULT_AT_IS_ON != g_ping_pdp_result) &&
            #endif
                (MMIPDP_RESULT_DIFFER_DUALSYS != g_ping_pdp_result))
            {
                MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text); 
                MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIPING_WAIT_WIN_ID,PNULL,
                                   ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePingModeWaitWindow);
            }
            return_val = TRUE;
        }
    }

    SCI_TRACE_LOW("[MMIPBSYNC]:MMIPBSYCN_PDPTry return_val = %d", return_val);
    return return_val;
}
#endif
