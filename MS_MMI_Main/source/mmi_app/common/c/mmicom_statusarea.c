/*****************************************************************************
** File Name:      mmicom_statusarea.c                                       *
** Author:                                                                   *
** Date:           06/07/2015                                                *
** Copyright:      2015 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2015                      Creat
******************************************************************************/

#ifndef _MMICOM_STATUSAREA_C_
#define _MMICOM_STATUSAREA_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_idle_trc.h"
#include "mmicom_statusarea.h"
#include "mmialarm_export.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "mmialarm_id.h"
#include "mms_image.h"
#include "mmisms_id.h"
#include "mmisms_image.h"
#include "mmicl_export.h"
#include "mmicc_id.h"

#include "mmiset_id.h"
#include "mmiidle_func.h"
#include "mmidisplay_data.h"
#include "mmi_id.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiota_image.h"
#include "mmiidle_cstyle.h"
#include "mmimtv_export.h"
#include "mmiatv_export.h"
#include "mmimms_export.h"
#include "mmiidle_subwintab.h"
//#ifdef MMI_DUAL_BATTERY_SUPPORT
#include "dualbat_drvapi.h"
//#endif
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "guistatusbar_scrollmsg.h"
#include "mmicl_internal.h"
#include "mmisms_read.h"
#ifndef WIN32
#include "gpio_prod_api.h"
#endif
#include "mmipub.h"

#if defined(MMI_SMSCB_SUPPORT)
#include "mmisms_export.h"
#endif

#include "mmiconnection_export.h"

#include "mmi_event_api.h"
#include "mmisms_receive.h"
#include "mmi_mainmenu_matrix.h"
#include "mmiphone_export.h"

#if defined(TORCH_SUPPORT)
#include "mmiflashlight_export.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifndef MMI_MULTI_SIM_SYS_SINGLE
typedef uint8  SIM_GLOBAL_SELECTION_TYPE;
#define SIM1_SELECTED  MN_DUAL_SYS_1//SIM1
#define SIM2_SELECTED   MN_DUAL_SYS_2 //SIM2
#define ALWAYS_ASK   MN_DUAL_SYS_MAX//always ask
#endif
#ifdef MMI_2G_4G_SUPPORT
#define  SIM_STATUS_ICON_MAX   2  //sim data connect status
#endif
#define MMI_SIGNAL_IDLE_MODE_TIMEOUT 2000 //信号跳变平滑显示Timer 2000ms
#define MMI_SIGNAL_DEDICATED_MODE_TIMEOUT 1000 //信号跳变平滑显示Timer 1000
#define MMICOM_CHARGE_FULL_CAP        100
#define MMI_BATTERY_CHARGE_WARNNING_TIMEOUT 5000 //正在充电状态显示Timer 5000ms
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef PRODUCT_DM
LOCAL BOOLEAN s_is_gprs_attach[MMI_DUAL_SYS_MAX] = {0};
#endif
#if defined CMCC_UI_STYLE || defined NET_ICON_UI_STYLE
LOCAL MMIPDP_NOTIFY_EVENT_E s_pdp_event[MMI_DUAL_SYS_MAX] = {0};
#endif
LOCAL uint8 s_signal_bound_timer[MMI_DUAL_SYS_MAX] = {0};
LOCAL BOOLEAN s_is_withheader = FALSE;
LOCAL BOOLEAN s_is_lockscreen = FALSE;
LOCAL uint8 s_charge_warnning_timer_id = 0;
typedef struct
{
    uint16 mcc;
    uint16 mnc;
    char icon_2Gtype;
    char icon_4Gtype;
}MMI_RATICON_G_PLMN_T;

LOCAL const MMI_RATICON_G_PLMN_T s_raticon_table[] = 
{
    #include "raticon_list.h"
};

#define MAX_RATICON_NUM (ARR_SIZE(s_raticon_table))
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// Description : 显示图标
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
LOCAL void SetStatusAreaInfo(
    uint32 item_index
);

/*****************************************************************************/
// Description : 显示信号图标
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
LOCAL void StatusAreaSetSimInfo(
    MN_DUAL_SYS_E dual_sys,
    uint32 item_index,
    BOOLEAN is_append
);

/*****************************************************************************/
// Description : 显示volte 信号图标
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
#ifdef MMI_VOLTE_SUPPORT
LOCAL void StatusAreaSetVolteInfo(MN_DUAL_SYS_E dual_sys,uint32 item_index, BOOLEAN is_append);
#endif
/*****************************************************************************/
// Description : 显示漫游图标
// Global resource dependence :
// Author: wancan.you
// Note:
/*****************************************************************************/
LOCAL void StatusAreaSetRoamingInfo(uint32 item_index, BOOLEAN is_append);

/*****************************************************************************/
// Description : 获取环境设置的图标
// Global resource dependence :
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T StatusAreaGetEvnSetIconId(void);


/*****************************************************************************/
// Description : 获取线路设置的图标
// Global resource dependence :
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T StatusAreaGetLineCallForwardIconId(void);

/*****************************************************************************/
//  Description : 判断是否时号码快速查询
//  Global resource dependence :
//  Author: bown.zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T StatusAreaGetMessageDispImageId(void);

/*****************************************************************************/
// Description : 显示图标
// Global resource dependence :
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL void StatusAreaSetGprsInfo(
    MN_DUAL_SYS_E dual_sys,
    MMIPDP_NOTIFY_EVENT_E pdp_event,
    BOOLEAN is_append
);

/*****************************************************************************/
//  Description : handle rxlevel bound timer
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void StatusAreaHandleRxLevelTimer(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : handle rxlevel bound timer
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StatusAreaStartRxLevelTimer(
    MN_DUAL_SYS_E dual_sys,
    BOOLEAN is_need_bound
);

/*****************************************************************************/
// Description : ????????
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenBtryCapWinFunc(void);

/*****************************************************************************/
// Description :
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
LOCAL void SetStatusAreaInfo(
    uint32 item_index
)
{
    MMI_IMAGE_ID_T          image_id        =   IMAGE_NULL;
    GUI_LCD_DEV_INFO        lcd_dev_info    =   {GUI_LCD_0, GUI_BLOCK_0};
    GUIWIN_STBDATA_ITEM_DATA_T  item_data   =   {0};

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;

    item_data.icon_info.icon_num = 1;
    item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;
    item_data.icon_info.is_display = TRUE;
    item_data.cur_frame = 0;
    item_data.is_handle_callback = TRUE;

    switch(item_index)
    {
#ifdef STATUS_TEXT_SUPPORT
    case MMICOM_WIN_TEXT_INFO:
    {
        item_data.icon_info.is_display = FALSE;

        GUIWIN_AppendStbItemData(&item_data);
        GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_INFO, FALSE);
    }
    break;
#endif

    case MMICOM_WIN_ICON_USB_MODE:
    {
        image_id = IMAGE_STATUS_USB_CONNECTED;
        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);

        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_USB_MODE, FALSE);
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_USB_MODE, FALSE);
    }
    break;

#ifdef BLUETOOTH_SUPPORT
    case MMICOM_WIN_ICON_BLUE_TOOTH: //  蓝牙
        if ( ON_AND_VISIBLE == MMIIDLE_GetIdleWinInfo()->bluetooth_state)
        {
            image_id = (MMI_IMAGE_ID_T)IMAGE_IDLE_TITLE_ICON_BT_VISIBLE;
            //blue tooth is power on and visible
        }
        else if ( ON_AND_HIDDEN == MMIIDLE_GetIdleWinInfo()->bluetooth_state)
        {
            image_id = (MMI_IMAGE_ID_T)IMAGE_IDLE_TITLE_ICON_BT_VISIBLE;
            //blue tooth is power on and hidden
        }
        else
        {
            image_id = (MMI_IMAGE_ID_T)IMAGE_NULL;
        }
        if(ON_AND_HEADSETCONNECT == MMIIDLE_GetIdleWinInfo()->bluetooth_state)
        {
            image_id = (MMI_IMAGE_ID_T)IMAGE_BT_HEADSET_CONNECT;
        }

        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = PNULL;

        GUIWIN_AppendStbItemData(&item_data);

        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_BLUE_TOOTH, FALSE);
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_BLUE_TOOTH, FALSE);
        break;
#endif

    case MMICOM_WIN_ICON_STOPWATCH:
    {
        image_id = IMAGE_STATUS_STOPWATCH;
        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);

        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_STOPWATCH, FALSE);
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_STOPWATCH, FALSE);
    }
    break;
    case MMICOM_WIN_ICON_COUNTDOWN_TIMER:
    {
        image_id = IMAGE_STATUS_COUNTDOWN;
        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);

        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_COUNTDOWN_TIMER, FALSE);
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_COUNTDOWN_TIMER, FALSE);
    }
    break;
    case MMICOM_WIN_ICON_CALENDAR_REMINDER:
    {
        image_id = IMAGE_STATUS_REMINDER;
        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);

        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALENDAR_REMINDER, FALSE);
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_CALENDAR_REMINDER, FALSE);
    }
    break;

    case MMICOM_WIN_ICON_ALARM:  // 闹钟 2
        image_id = IMAGE_IDLE_TITLE_ICON_CLOCK;
        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);

        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ALARM, FALSE);
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_ALARM, FALSE);
        break;

    case MMICOM_WIN_ICON_ENVSET:     // 情景模式

        image_id = StatusAreaGetEvnSetIconId();

        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);

        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ENVSET, FALSE);
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_ENVSET, FALSE);
        break;

    case MMICOM_WIN_ICON_EARPHONE:
        item_data.icon_info.icon_array[0] = IMAGE_IDLE_TITLE_ICON_EARPHONE;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);
        
        if(GPIO_CheckHeadsetStatus())
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_EARPHONE, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_EARPHONE, FALSE);
        }
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_EARPHONE, FALSE);
        break;

    case MMICOM_WIN_ICON_APP_NOTICE:
    {
        image_id = IMAGE_STATUS_GENERAL_APPLICATION_NOTIFICATION;
        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);

        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_APP_NOTICE, FALSE);
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_APP_NOTICE, FALSE);
    }
    break;

    case MMICOM_WIN_ICON_MISSED_CALL:
        item_data.icon_info.icon_array[0] = IMAGE_IDLE_TITLE_ICON_MISSED_CALL;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);

        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_MISSED_CALL, FALSE);
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_MISSED_CALL, FALSE);
        break;

    case MMICOM_WIN_ICON_MESSAGE: // 消息
        image_id = StatusAreaGetMessageDispImageId();
        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);

        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_MESSAGE, FALSE);
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_MESSAGE, FALSE);
        break;

    case MMICOM_WIN_ICON_VOICEMAIL:
    {
        image_id = IMAGE_IDLE_ICON_NEW_VM;
        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);

        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_VOICEMAIL, FALSE);
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_VOICEMAIL, FALSE);
    }
    break;

    case MMICOM_WIN_ICON_CALL_FORWARD: // 呼叫转移
        image_id = StatusAreaGetLineCallForwardIconId();
        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);

        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALL_FORWARD, FALSE);
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_CALL_FORWARD, FALSE);
        break;

    case MMICOM_WIN_ICON_CALLING:
        item_data.icon_info.icon_array[0] = IMAGE_IDLE_TITLE_ICON_CALLING;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);

        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALLING, FALSE);
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_CALLING, FALSE);
        break;

#if defined(TORCH_SUPPORT)
    case MMICOM_WIN_ICON_TORCH: //Torch
        item_data.icon_info.icon_array[0] = IMAGE_IDLE_TITLE_ICON_FLASHLIGHT;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);
        if(TRUE == MMIIDLE_GetTorchStatus())
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_TORCH, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_TORCH, FALSE);
        }
        break; 
#endif

#ifdef PRODUCT_DM
#if !defined CMCC_UI_STYLE && !defined NET_ICON_UI_STYLE
        //    case MMICOM_WIN_ICON_GPRS:
        //        StatusAreaSetGprsInfo(MN_DUAL_SYS_1, MMIPDP_NOTIFY_EVENT_DETTACH, TRUE);
        //        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_GPRS, TRUE);
        //        break;
#endif
#endif

    case MMICOM_WIN_ICON_ROAMING:
        StatusAreaSetRoamingInfo(MMICOM_WIN_ICON_ROAMING, TRUE);
        break;
#ifndef MMI_MULTI_SIM_SYS_SINGLE		
    case MMICOM_WIN_ICON_ROAMING_2:
        StatusAreaSetRoamingInfo(MMICOM_WIN_ICON_ROAMING_2, TRUE);
        break;		
#endif
#ifdef MMI_VOLTE_SUPPORT
    case MMICOM_WIN_ICON_VOLTE:
        StatusAreaSetVolteInfo(MN_DUAL_SYS_1,MMICOM_WIN_ICON_VOLTE, TRUE);
        break;
#endif

    case MMICOM_WIN_ICON_SIGNAL:
        StatusAreaSetSimInfo(MN_DUAL_SYS_1, MMICOM_WIN_ICON_SIGNAL, TRUE);
        break;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
#ifdef MMI_VOLTE_SUPPORT
    case MMICOM_WIN_ICON_VOLTE_2:
        StatusAreaSetVolteInfo(MN_DUAL_SYS_2,MMICOM_WIN_ICON_VOLTE_2, TRUE);
        break;
#endif
    case MMICOM_WIN_ICON_SIGNAL_2:
        StatusAreaSetSimInfo(MN_DUAL_SYS_2, MMICOM_WIN_ICON_SIGNAL_2, TRUE);        //MAIN LCD sim2信号量提示
        break;
#endif

    case MMICOM_WIN_ICON_BATTERY:  // 电池电量
    {
        uint32  icon_index = 0;
        uint32  charge_icon_id[MMISTATUSBAR_ITEM_ICON_FRAME] =   {
#ifdef ZDT_PRODUCT_Z685S
			IMAGE_IDLE_TITLE_ICON_BATTERY1,
			IMAGE_IDLE_TITLE_ICON_BATTERY2,
			IMAGE_IDLE_TITLE_ICON_BATTERY3,
			IMAGE_IDLE_TITLE_ICON_BATTERY4,
			IMAGE_IDLE_TITLE_ICON_BATTERY5,
			IMAGE_IDLE_TITLE_ICON_BATTERY6
#else
			IMAGE_IDLE_TITLE_ICON_BATTERY1,
			IMAGE_IDLE_TITLE_ICON_BATTERY2,
			IMAGE_IDLE_TITLE_ICON_BATTERY3,
			IMAGE_IDLE_TITLE_ICON_BATTERY3,
			IMAGE_IDLE_TITLE_ICON_BATTERY4,
			IMAGE_IDLE_TITLE_ICON_BATTERY4,
			IMAGE_IDLE_TITLE_ICON_BATTERY5,
			IMAGE_IDLE_TITLE_ICON_BATTERY5,
			IMAGE_IDLE_TITLE_ICON_BATTERY6,
			IMAGE_IDLE_TITLE_ICON_BATTERY6
#endif
		};
        MMIIDLE_GetIdleWinInfo()->batlevel = MMIAPIPHONE_GetBatCapacity();

        for (icon_index = 0; icon_index < MMISTATUSBAR_ITEM_ICON_FRAME; icon_index++)
        {
            item_data.icon_info.icon_array[icon_index] = charge_icon_id[icon_index];
        }

        item_data.app_handle_func = OpenBtryCapWinFunc;
        item_data.icon_info.icon_num = MMISTATUSBAR_ITEM_ICON_FRAME;
        item_data.cur_frame = MMIIDLE_GetIdleWinInfo()->batlevel;

        if (MMIIDLE_GetIdleWinInfo()->is_charge)
        {
            item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_ANIM;
        }
        else
        {
            item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;
        }

        GUIWIN_AppendStbItemData(&item_data);
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_BATTERY, TRUE);
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_BATTERY, TRUE);
    }
    break;

    case MMICOM_WIN_TEXT_TIME:
    {
        uint16 w_str[MMIIDLE_TIME_LEN] = {0};
        // uint16 wstr_len = 0;

        MMIAPICOM_StatusAreaGetSysTime(w_str, MMIIDLE_TIME_LEN);
        item_data.icon_info.is_display = FALSE;
        item_data.text_info.is_display = TRUE;
        item_data.app_handle_func = PNULL;
        item_data.text_info.wstr_len = (uint32)MMIAPICOM_Wstrlen(w_str);

        // 取最小值，防止越界
        item_data.text_info.wstr_len = MIN(item_data.text_info.wstr_len, MMIIDLE_TIME_LEN);
        item_data.text_info.wstr_len = MIN(item_data.text_info.wstr_len, MMISTATUSBAR_ITEM_TEXT_LEN);

        // cp字符串
        SCI_MEMCPY(item_data.text_info.wstr_ptr, w_str, sizeof(wchar)*item_data.text_info.wstr_len);

        item_data.text_info.font_color = MMI_WHITE_COLOR;//MMI_GRAY_WHITE_COLOR; CRNEWMS00203710
        item_data.text_info.align = ALIGN_RVMIDDLE;//ALIGN_HVMIDDLE; //居中 edit for CRNEWMS00135505
        item_data.text_info.font_type = MMI_STATUS_FONT_TYPE;

        GUIWIN_AppendStbItemData(&item_data);

        GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_TIME, FALSE);
        GUIWIN_SetStbItemCenterAlign(MMICOM_WIN_TEXT_TIME, FALSE);
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_TEXT_TIME, TRUE);
    }
    break;

    default:
        break;
    }
}

/*****************************************************************************/
// Description : ????????
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenBtryCapWinFunc(void)
{
    BOOLEAN     result  =   FALSE;

    MMIAPIPHONE_OpenBtryCapWin();
    if (MMK_IsOpenWin(PHONE_STARTUP_BTRY_CAP_WIN_ID))
    {
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
// Description : Status area init
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaInit(void)
{
    uint32      i   =   0;

#if defined CMCC_UI_STYLE || defined NET_ICON_UI_STYLE
    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        s_pdp_event[i] = MMIPDP_NOTIFY_EVENT_MAX;
    }
#endif

    for(i = 0; i < MMICOM_WIN_ICON_MAX; i++)
    {
        SetStatusAreaInfo(i);
    }
}

LOCAL BOOLEAN DisplaySetSysmodeFunc(void)
{
#ifdef MCARE_V31_SUPPORT
    if(MCareV31_Exit_Confirm())
    {
        return FALSE;
    }
#endif
#ifdef MMI_MULTI_SIM_SYS_SINGLE
    MMIAPISET_OpenSetFlyMode();
#else
    if(MMIAPISET_GetFlyMode())
    {
        MMIAPISET_OpenSetFlyMode();
    }
    else
    {
        MMIAPISET_OpenMultiSimSettingsWindow();
    }
#endif

    return TRUE;
}

LOCAL void StatusAreaSetSimInfo(
                                MN_DUAL_SYS_E dual_sys,
                                uint32 item_index,
                                BOOLEAN is_append
                                )
{
#if defined CMCC_UI_STYLE
    const MMI_IMAGE_ID_T service_image[NET_SERVICE_TYPE_MAX][MMIPDP_NOTIFY_EVENT_MAX + 1]=
    {
        {IMAGE_IDLE_TITLE_ICON_G_DATAING_1,IMAGE_IDLE_TITLE_ICON_G_1,IMAGE_IDLE_TITLE_ICON_G_1,IMAGE_IDLE_TITLE_ICON_G_1, IMAGE_IDLE_TITLE_ICON_G_1},
        {IMAGE_IDLE_TITLE_ICON_E_DATAING_1,IMAGE_IDLE_TITLE_ICON_E_1,IMAGE_IDLE_TITLE_ICON_E_1,IMAGE_IDLE_TITLE_ICON_E_1, IMAGE_IDLE_TITLE_ICON_E_1},
        {IMAGE_IDLE_TITLE_ICON_3G_DATAING_1,IMAGE_IDLE_TITLE_ICON_3G_1,IMAGE_IDLE_TITLE_ICON_3G_1,IMAGE_IDLE_TITLE_ICON_3G_1, IMAGE_IDLE_TITLE_ICON_3G_1},
        {IMAGE_IDLE_TITLE_ICON_4G_DATAING_1, IMAGE_IDLE_TITLE_ICON_4G_1, IMAGE_IDLE_TITLE_ICON_4G_1, IMAGE_IDLE_TITLE_ICON_4G_1, IMAGE_IDLE_TITLE_ICON_4G_1},
    };
#endif

#if defined NET_ICON_UI_STYLE
    MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;
#ifdef MMI_MULTI_SIM_SYS_SINGLE
    const MMI_IMAGE_ID_T emergency_service_start_id = IMAGE_IDLE_TITLE_ICON_X_SIM1_1;

    const MMI_IMAGE_ID_T service_image[NET_SERVICE_TYPE_MAX][MMIPDP_NOTIFY_EVENT_MAX + 1]=
    {
        {IMAGE_IDLE_TITLE_ICON_G_DATAING_1,IMAGE_IDLE_TITLE_ICON_G_1,IMAGE_IDLE_TITLE_ICON_G_1,IMAGE_IDLE_TITLE_ICON_G_1, IMAGE_IDLE_TITLE_ICON_G_1},
        {IMAGE_IDLE_TITLE_ICON_E_DATAING_1,IMAGE_IDLE_TITLE_ICON_E_1,IMAGE_IDLE_TITLE_ICON_E_1,IMAGE_IDLE_TITLE_ICON_E_1, IMAGE_IDLE_TITLE_ICON_E_1},
        {IMAGE_IDLE_TITLE_ICON_3G_DATAING_1,IMAGE_IDLE_TITLE_ICON_3G_1,IMAGE_IDLE_TITLE_ICON_3G_1,IMAGE_IDLE_TITLE_ICON_3G_1, IMAGE_IDLE_TITLE_ICON_3G_1},
        {IMAGE_IDLE_TITLE_ICON_4G_DATAING_1,IMAGE_IDLE_TITLE_ICON_4G_1,IMAGE_IDLE_TITLE_ICON_4G_1,IMAGE_IDLE_TITLE_ICON_4G_1, IMAGE_IDLE_TITLE_ICON_4G_1},
    };
#elif defined MULTI_SIM_SYS_DUAL	
    const MMI_IMAGE_ID_T emergency_service_start_id[MMI_DUAL_SYS_MAX]  = {IMAGE_IDLE_TITLE_ICON_X_SIM1_1, IMAGE_IDLE_TITLE_ICON_X_SIM2_1};

    const MMI_IMAGE_ID_T service_image_sim1[NET_SERVICE_TYPE_MAX][MMIPDP_NOTIFY_EVENT_MAX + 1]=
    {
        {IMAGE_IDLE_TITLE_ICON_G_DATAING_SIM1_1,IMAGE_IDLE_TITLE_ICON_G_SIM1_1,IMAGE_IDLE_TITLE_ICON_G_SIM1_1,IMAGE_IDLE_TITLE_ICON_G_SIM1_1, IMAGE_IDLE_TITLE_ICON_G_SIM1_1},
        {IMAGE_IDLE_TITLE_ICON_E_DATAING_SIM1_1,IMAGE_IDLE_TITLE_ICON_E_SIM1_1,IMAGE_IDLE_TITLE_ICON_E_SIM1_1,IMAGE_IDLE_TITLE_ICON_E_SIM1_1, IMAGE_IDLE_TITLE_ICON_E_SIM1_1},
        {IMAGE_IDLE_TITLE_ICON_3G_DATAING_SIM1_1,IMAGE_IDLE_TITLE_ICON_3G_SIM1_1,IMAGE_IDLE_TITLE_ICON_3G_SIM1_1,IMAGE_IDLE_TITLE_ICON_3G_SIM1_1, IMAGE_IDLE_TITLE_ICON_3G_SIM1_1},
        {IMAGE_IDLE_TITLE_ICON_4G_DATAING_SIM1_1,IMAGE_IDLE_TITLE_ICON_4G_SIM1_1,IMAGE_IDLE_TITLE_ICON_4G_SIM1_1,IMAGE_IDLE_TITLE_ICON_4G_SIM1_1, IMAGE_IDLE_TITLE_ICON_4G_SIM1_1},
    };

    const MMI_IMAGE_ID_T service_image_sim2[NET_SERVICE_TYPE_MAX][MMIPDP_NOTIFY_EVENT_MAX + 1]=
    {
        {IMAGE_IDLE_TITLE_ICON_G_DATAING_SIM2_1,IMAGE_IDLE_TITLE_ICON_G_SIM2_1,IMAGE_IDLE_TITLE_ICON_G_SIM2_1,IMAGE_IDLE_TITLE_ICON_G_SIM2_1, IMAGE_IDLE_TITLE_ICON_G_SIM2_1},
        {IMAGE_IDLE_TITLE_ICON_E_DATAING_SIM2_1,IMAGE_IDLE_TITLE_ICON_E_SIM2_1,IMAGE_IDLE_TITLE_ICON_E_SIM2_1,IMAGE_IDLE_TITLE_ICON_E_SIM2_1, IMAGE_IDLE_TITLE_ICON_E_SIM2_1},
        {IMAGE_IDLE_TITLE_ICON_3G_DATAING_SIM2_1,IMAGE_IDLE_TITLE_ICON_3G_SIM2_1,IMAGE_IDLE_TITLE_ICON_3G_SIM2_1,IMAGE_IDLE_TITLE_ICON_3G_SIM2_1, IMAGE_IDLE_TITLE_ICON_3G_SIM2_1},
        {IMAGE_IDLE_TITLE_ICON_4G_DATAING_SIM2_1,IMAGE_IDLE_TITLE_ICON_4G_SIM2_1,IMAGE_IDLE_TITLE_ICON_4G_SIM2_1,IMAGE_IDLE_TITLE_ICON_4G_SIM2_1, IMAGE_IDLE_TITLE_ICON_4G_SIM2_1},
    };
#endif
#endif

#ifdef MMI_MULTISIM_COLOR_SUPPORT_STATUSICON
        GUISTBDATA_ICON_T icon_info = {0};
#else
        const MMI_IMAGE_ID_T signal_gsm_start_id[MMI_DUAL_SYS_MAX] = 
        {
    #ifdef MMI_MULTI_SIM_SYS_SINGLE
            IMAGE_IDLE_TITLE_ICON_SIGNAL1
    #else
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_SIM1,
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_SIM2,
        #if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_SIM3,
        #endif
        #if defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_SIM4
        #endif
    #endif
        };
        
#ifdef MMI_GPRS_SUPPORT     
        const MMI_IMAGE_ID_T signal_gprs_start_id[MMI_DUAL_SYS_MAX] = {
    #ifdef MMI_MULTI_SIM_SYS_SINGLE
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_GPRS
    #else
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_GPRS_SIM1,
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_GPRS_SIM2,
        #if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_GPRS_SIM3,
        #endif
        #if defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_GPRS_SIM4
        #endif
    #endif
        };
#endif    
    
//#ifdef PRODUCT_DM  /* lint_lai */
        const MMI_IMAGE_ID_T signal_td_start_id[MMI_DUAL_SYS_MAX] = {
    #ifdef MMI_MULTI_SIM_SYS_SINGLE
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_T
    #else
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_T_SIM1,
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_T_SIM2,
        #if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_T_SIM3,
        #endif
        #if defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_T_SIM4
        #endif
    #endif
        };   
//#endif    

#endif /* end of MMI_MULTISIM_COLOR_SUPPORT_STATUSICON */

    BOOLEAN is_fly_mode_on = FALSE;
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    GUIWIN_STBDATA_ITEM_DATA_T item_data = {0};
    MMI_IMAGE_ID_T signal_start_id = IMAGE_NULL;
    MN_PLMN_T cur_plmn = {0};
    BOOLEAN isGmust = FALSE;
    BOOLEAN isLTEmust = FALSE;
    BOOLEAN isRaticonable = FALSE;
    MMIPHONE_NET_SERVICE_TYPE_E  service_type = NET_SERVICE_TYPE_MAX;
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    item_data.icon_info.icon_num = 1;
    item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;
    item_data.icon_info.is_display = TRUE;
    item_data.cur_frame = 0;
    item_data.is_handle_callback = TRUE;
    
    is_fly_mode_on = MMIAPISET_GetFlyMode();

//#ifdef PRODUCT_DM
    if (is_fly_mode_on)
    {
        if (MN_DUAL_SYS_1 == dual_sys)//飞行模式只显示一个图标
        {
            image_id = IMAGE_IDLE_TITLE_ICON_FLYMODE;
        }
        else
        {
            image_id = IMAGE_NULL;
        }
    }
    else if (MMIAPIPHONE_GetSimExistedStatus(dual_sys))
    {
        MMI_GMMREG_RAT_E    network_status_rat = MMI_GMMREG_RAT_GPRS;
        MN_DUAL_SYS_E cc_dual_sys = MMIAPICC_GetCallingSIM();
        network_status_rat = MMIAPIPHONE_GetTDOrGsm(dual_sys);
        isRaticonable = MMIAPICOM_IsExistInRatIconTable(&cur_plmn, &isGmust, &isLTEmust);
        SCI_TRACE_LOW("StatusAreaSetSimInfo dual_sys:%d ,network_status_rat:%d,data_service_type:%d,cc_dual_sys=%d", dual_sys, network_status_rat, MMIAPIPHONE_GetDataServiceType(dual_sys), cc_dual_sys);

#ifdef MMI_MULTISIM_COLOR_SUPPORT_STATUSICON
        switch (network_status_rat) 
        {
        case MMI_GMMREG_RAT_GPRS:
            #ifdef MMI_GPRS_SUPPORT            
            signal_start_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_GPRS;
            #else            
            signal_start_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1;
            #endif            
            break;
            
        case MMI_GMMREG_RAT_3G:
            signal_start_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_T;
            break;
        case MMI_GMMREG_RAT_LTE:            
        case MMI_GMMREG_RAT_UNKNOWN:
            signal_start_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1;
            break;
            
        default:
            signal_start_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1;
            break;
        }
#else
        switch (network_status_rat) 
        {
        case MMI_GMMREG_RAT_GPRS:
    #if defined CMCC_UI_STYLE
            signal_start_id = service_image[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
    #elif defined NET_ICON_UI_STYLE
        #ifdef MMI_MULTI_SIM_SYS_SINGLE
            signal_start_id = service_image[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
        #elif defined MULTI_SIM_SYS_DUAL	
            if (MN_DUAL_SYS_1 == dual_sys)
            {
                signal_start_id = service_image_sim1[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
            }
            else if (MN_DUAL_SYS_2 == dual_sys)
            {
                signal_start_id = service_image_sim2[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
            }
        #endif
    #else
            signal_start_id = signal_gprs_start_id[dual_sys];
    #endif
            break;
            
        case MMI_GMMREG_RAT_3G:
    #if defined CMCC_UI_STYLE
            signal_start_id = service_image[NET_SERVICE_TYPE_3G][s_pdp_event[dual_sys]];
    #elif defined NET_ICON_UI_STYLE
        #ifdef MMI_MULTI_SIM_SYS_SINGLE
            signal_start_id = service_image[NET_SERVICE_TYPE_3G][s_pdp_event[dual_sys]];
        #elif defined MULTI_SIM_SYS_DUAL	
            if (MN_DUAL_SYS_1 == dual_sys)
            {
                signal_start_id = service_image_sim1[NET_SERVICE_TYPE_3G][s_pdp_event[dual_sys]];
            }
            else if (MN_DUAL_SYS_2 == dual_sys)
            {
                signal_start_id = service_image_sim2[NET_SERVICE_TYPE_3G][s_pdp_event[dual_sys]];
            }
        #endif
    #else
            signal_start_id = signal_td_start_id[dual_sys];
    #endif
            break;
            
        case MMI_GMMREG_RAT_4G:
            service_type = NET_SERVICE_TYPE_4G;
        #if 0
            if (TRUE == isRaticonable)
            {
                if (TRUE == isLTEmust)
                {
                    service_type = NET_SERVICE_TYPE_LTE;
                }
                else
                {
                    service_type = NET_SERVICE_TYPE_4G;
                }
            }
            else
            {
                if (MMIAPIPHONE_4g_icon_display_style() == 1)
                {
                    service_type = NET_SERVICE_TYPE_LTE;
                }
                else
                {
                    service_type = NET_SERVICE_TYPE_4G;
                }
            }
        #endif
    #if defined CMCC_UI_STYLE
                    signal_start_id = service_image[service_type][s_pdp_event[dual_sys]];
    #elif defined NET_ICON_UI_STYLE
        #ifdef MMI_MULTI_SIM_SYS_SINGLE
                    signal_start_id = service_image[service_type][s_pdp_event[dual_sys]];
        #elif defined MULTI_SIM_SYS_DUAL    
                    if (MN_DUAL_SYS_1 == dual_sys)
                    {
                        signal_start_id = service_image_sim1[service_type][s_pdp_event[dual_sys]];
                    }
                    else if (MN_DUAL_SYS_2 == dual_sys)
                    {
                        signal_start_id = service_image_sim2[service_type][s_pdp_event[dual_sys]];
                    }           
        #endif
    #else
                    signal_start_id = signal_gprs_start_id[dual_sys];
    #endif
            break;
            
        case MMI_GMMREG_RAT_UNKNOWN:
    #if defined CMCC_UI_STYLE
            signal_start_id = service_image[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
    #elif defined NET_ICON_UI_STYLE
        #ifdef MMI_MULTI_SIM_SYS_SINGLE
            signal_start_id = service_image[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
        #elif defined MULTI_SIM_SYS_DUAL	
            if(MN_DUAL_SYS_1 == dual_sys)
            {
                signal_start_id = service_image_sim1[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
            }
            else if(MN_DUAL_SYS_2 == dual_sys)
            {
                signal_start_id = service_image_sim2[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
            }
        #endif
    #else
            signal_start_id = signal_gsm_start_id[dual_sys];
    #endif
            break;
            
        default:
    #if defined CMCC_UI_STYLE
            signal_start_id = service_image[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
    #elif defined NET_ICON_UI_STYLE
        #ifdef MMI_MULTI_SIM_SYS_SINGLE
            signal_start_id = service_image[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
        #elif defined MULTI_SIM_SYS_DUAL	
            if(MN_DUAL_SYS_1 == dual_sys)
            {
                signal_start_id = service_image_sim1[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
            }
            else if(MN_DUAL_SYS_2 == dual_sys)
            {
                signal_start_id = service_image_sim2[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
            }
        #endif
    #else
            signal_start_id = signal_gsm_start_id[dual_sys];
    #endif
            break;
        }
#endif /* end of MMI_MULTISIM_COLOR_SUPPORT_STATUSICON */

#if defined CMCC_UI_STYLE
        if (EDGE_DATA_SERVICE_TYPE == MMIAPIPHONE_GetDataServiceType(dual_sys))
        {
            signal_start_id = service_image[NET_SERVICE_TYPE_E][s_pdp_event[dual_sys]];
        }
#endif

#if defined NET_ICON_UI_STYLE
        if (EDGE_DATA_SERVICE_TYPE == MMIAPIPHONE_GetDataServiceType(dual_sys))
        {
    #ifdef MMI_MULTI_SIM_SYS_SINGLE
            signal_start_id = service_image[NET_SERVICE_TYPE_E][s_pdp_event[dual_sys]];
    #elif defined MULTI_SIM_SYS_DUAL	
            if(MN_DUAL_SYS_1 == dual_sys)
            {
                signal_start_id = service_image_sim1[NET_SERVICE_TYPE_E][s_pdp_event[dual_sys]];
            }
            else if(MN_DUAL_SYS_2 == dual_sys)
            {
                signal_start_id = service_image_sim2[NET_SERVICE_TYPE_E][s_pdp_event[dual_sys]];
            }
    #endif
        }
        plmn_status = MMIAPIPHONE_GetPlmnStatus(dual_sys);
        if (PLMN_NO_SERVICE == plmn_status
            || PLMN_EMERGENCY_ONLY == plmn_status 
            || PLMN_EMERGENCY_SIM_INVALID == plmn_status
            || PLMN_EMERGENCY_GPRS_ONLY == plmn_status 
            || PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY == plmn_status
            || PLMN_REGISTER_SERVICE == plmn_status
            || PLMN_REGISTER_GPRS_ONLY == plmn_status)
        {
    #ifdef MMI_MULTI_SIM_SYS_SINGLE
            signal_start_id = emergency_service_start_id;
    #elif defined MULTI_SIM_SYS_DUAL	
            signal_start_id = emergency_service_start_id[dual_sys];
    #endif        
        }            
#endif /* end of NET_ICON_UI_STYLE */

	if(MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys]>0 && MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys]< 5)
	{
		MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys]++;
	}

        if (MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting(dual_sys))
        {
            if (SIM_STATUS_PIN_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys)
                || SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys))
            {
                image_id = signal_start_id;
            }
            else
            {
                image_id = MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys] + signal_start_id;
            }
        }
        else
        {
            image_id = signal_start_id;
        }
    }
    else
    {
        image_id = IMAGE_IDLE_ICON_NO_SIM;
    }

/*#else
    if (is_fly_mode_on)
    {
        if (MN_DUAL_SYS_1 == dual_sys)//飞行模式只显示一个图标
        {
            image_id = IMAGE_IDLE_TITLE_ICON_FLYMODE;
        }
        else
        {
            image_id = IMAGE_NULL;
        }
    }
//    else if( 0 == MMIAPIPHONE_GetSimExistedNum(PNULL, 0))
//   {
        //如果没有插入sim卡，只需要显示1个没有卡的图标就可以了。
//        if(MN_DUAL_SYS_1 == dual_sys)
//        {
//            image_id = IMAGE_IDLE_ICON_NO_SIM;
//        }
//        else
//        {
//            image_id = IMAGE_NULL;
//        }
//    }
    else if(SIM_STATUS_NO_SIM == MMIAPIPHONE_GetSimStatus(dual_sys))
	{
        image_id = IMAGE_IDLE_ICON_NO_SIM;
	}
    else
    {
        if (MMIAPIPHONE_GetSimExistedStatus(dual_sys))
        {
        #ifdef MMI_MULTISIM_COLOR_SUPPORT_STATUSICON
            if (MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting(dual_sys))
            {
                if (SIM_STATUS_PIN_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys)
                    || SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys))
                {
                    image_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1;
                }
                else
                {
                    if (MMIIDLE_GetIdleWinInfo()->is_gprs[dual_sys])
                    {
                        if (MMIAPICONNECTION_isGPRSSwitchedOff())
                        {
                            signal_start_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1;
                        }
            #ifdef MMI_GPRS_SUPPORT                        
                        else
                        {
                            signal_start_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_GPRS;
                        }
            #endif                        

                    }
                    else
                    {
                        signal_start_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1;
                    }

                    image_id = MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys] + signal_start_id;
                }
            }
            else
            {
                image_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1;
            }
        #else // MMI_MULTISIM_COLOR_SUPPORT_STATUSICON
            if (MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting(dual_sys))
            {
                if (SIM_STATUS_PIN_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys)
                    || SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys))
                {
                    image_id = signal_gsm_start_id[dual_sys];
                }
                else
                {
                    if (MMIIDLE_GetIdleWinInfo()->is_gprs[dual_sys])
                    {
                        if (MMIAPICONNECTION_isGPRSSwitchedOff())
                        {
                            signal_start_id = signal_gsm_start_id[dual_sys];
                        }
            #ifdef MMI_GPRS_SUPPORT 
                        else
                        {
                            signal_start_id = signal_gprs_start_id[dual_sys];
                        }
            #endif
                    }
                    else
                    {
                        signal_start_id = signal_gsm_start_id[dual_sys];
                    }

                    image_id = MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys] + signal_start_id;
                }
            }
            else
            {
                image_id = signal_gsm_start_id[dual_sys];
            }
        #endif // end of MMI_MULTISIM_COLOR_SUPPORT_STATUSICON
        }
        else
        {
            image_id = IMAGE_NULL;
        }
    }
#endif // end of PRODUCT_DM */

#ifdef MMI_VOLTE_SUPPORT
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (MN_DUAL_SYS_2 == dual_sys)
    {
        item_index = MMICOM_WIN_ICON_SIGNAL_2;
    }
#endif
#endif

#ifdef MMI_MULTISIM_COLOR_SUPPORT_STATUSICON
    if ((image_id == IMAGE_IDLE_TITLE_ICON_FLYMODE ) || (image_id == IMAGE_IDLE_ICON_NO_SIM ) || (image_id == IMAGE_NULL))
    {
        GUISTBDATA_GetItemIcon(item_index, &icon_info);
        icon_info.is_color_exchange	= FALSE;
        GUISTBDATA_SetItemIcon(item_index, &icon_info);
    }
    else
    {
        GUISTBDATA_GetItemIcon(item_index, &icon_info);
        icon_info.is_color_exchange = TRUE;
        GUISTBDATA_SetItemIcon(item_index, &icon_info);
    }
#endif    
    item_data.icon_info.icon_array[0] = image_id;
    item_data.app_handle_func = DisplaySetSysmodeFunc;

    if (is_append)
    {
        GUIWIN_AppendStbItemData(&item_data);
    }
    else
    {
        GUIWIN_SetStbItemIconId(item_index, image_id);
    }

    if (image_id == IMAGE_NULL)
    {
        GUIWIN_SetStbItemVisible(item_index, FALSE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(item_index, TRUE);
    }
    
    GUIWIN_SetStbItemRightAlign(item_index, FALSE);

    return;
}

#ifdef MMI_VOLTE_SUPPORT
LOCAL void StatusAreaSetVolteInfo(MN_DUAL_SYS_E dual_sys,uint32 item_index, BOOLEAN is_append)
{
    GUIWIN_STBDATA_ITEM_DATA_T item_data = {0};
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;
    BOOLEAN is_emergency_service = FALSE;
    BOOLEAN is_ps_ready = FALSE;
    BOOLEAN is_fly_mode_on = FALSE;
    BOOLEAN is_volte_icon_on =FALSE;
    BOOLEAN is_vodafone_icon_off =FALSE;	

    SCI_TRACE_LOW("StatusAreaSetVolteInfo dual_sys=%d,index=%d",dual_sys, item_index);
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

#ifdef MMI_MULTI_SIM_SYS_DUAL	
    if(dual_sys == MN_DUAL_SYS_2)
		item_index = MMICOM_WIN_ICON_VOLTE_2;
#endif
    is_fly_mode_on = MMIAPISET_GetFlyMode();

    if (is_fly_mode_on){
        image_id = IMAGE_NULL;
    }
    else
    {
        if(MMIAPIPHONE_volte_icon_display_set(dual_sys) == 1)
        {
            if (MMIAPIPHONE_GetImsStatus(dual_sys))
            {
                if(MMIAPIPHONE_volte_icon_display_style(dual_sys) == 1)
                {
                    image_id = IMAGE_STATUS_HD;	
                }
                else
                {
                    image_id = IMAGE_STATUS_VOLTE;		
                }
            }		
            else
            {
                image_id = IMAGE_NULL;
            }
        }
        else
        {
            image_id = IMAGE_NULL;
        }
    }
    plmn_status = MMIAPIPHONE_GetPlmnStatus(dual_sys);
    is_ps_ready = MMIAPIPHONE_GetIsPsReady(dual_sys);
    SCI_TRACE_LOW("StatusAreaSetVolteInfo plmn_status =%d,is_ps_ready =%d,sim_status = %d ",plmn_status,is_ps_ready,MMIAPIPHONE_GetSimStatus(dual_sys));
    if(PLMN_NO_SERVICE == plmn_status
            || PLMN_EMERGENCY_ONLY == plmn_status
            || PLMN_EMERGENCY_SIM_INVALID == plmn_status
            || PLMN_EMERGENCY_GPRS_ONLY == plmn_status
            || PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY == plmn_status
            || PLMN_REGISTER_SERVICE == plmn_status
            || PLMN_REGISTER_GPRS_ONLY == plmn_status
            || !is_ps_ready)
    {
        is_emergency_service = TRUE;
        SCI_TRACE_LOW("StatusAreaSetVolteInfo is_emergency_service = %d",is_emergency_service);
    }
    
    if ((MMIAPISET_IsVodafoneVersion()) && 
        (MMICONNECTION_SETTING_GPRS_SWITCH_OFF == MMIAPICONNECTION_GetGPRSSwitchStatus()))
    {
        is_vodafone_icon_off = TRUE;
        SCI_TRACE_LOW("StatusAreaSetVolteInfo is_vodafone_icon_off =%d",is_vodafone_icon_off);	
    }
    is_volte_icon_on = (MMIAPISET_GetVolteInfo() )& (1<<dual_sys);
    if (SIM_STATUS_PIN_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys)
            || SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys)
            || !is_volte_icon_on
            || is_emergency_service
            || is_vodafone_icon_off)
    {
            image_id = IMAGE_NULL;
    }

    item_data.icon_info.icon_array[0] = image_id;
    item_data.icon_info.icon_num = 1;
    item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;
    item_data.icon_info.is_display = TRUE;
    item_data.cur_frame = 0;
    item_data.app_handle_func = PNULL;
    item_data.is_handle_callback = FALSE;
if(image_id != IMAGE_NULL)	
	SCI_TraceLow(" StatusAreaSetVolteInfo  image_id=%x",image_id);
else
	SCI_TraceLow(" StatusAreaSetVolteInfo  image_id= NULL");
	
    if (is_append)
    {
        GUIWIN_AppendStbItemData(&item_data);
        GUIWIN_SetStbItemVisible(item_index, FALSE);
    }
    else
    {
        GUIWIN_SetStbItemIconId(item_index, image_id);

        if (image_id == IMAGE_NULL)
        {
            GUIWIN_SetStbItemVisible(item_index, FALSE);
        }
        else
        {
            if(!s_is_lockscreen)
            {
                GUIWIN_SetStbItemVisible(item_index, TRUE);
            }
            else
            {
                GUIWIN_SetStbItemVisible(item_index, FALSE);
            }
        }
    }
	 GUIWIN_SetStbItemRightAlign(item_index, FALSE);
}


/*****************************************************************************/
//  Description : set the VOLTE state of the idle window
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetIdleVolteState(MN_DUAL_SYS_E dual_sys)
{
    StatusAreaSetVolteInfo(dual_sys, MMICOM_WIN_ICON_VOLTE, FALSE);
    GUIWIN_UpdateStb();
}
#endif
/*****************************************************************************/
//  Description :  设置文本内容
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 文本内容为时间
/*****************************************************************************/
LOCAL BOOLEAN StatusbarSetDispTime(void)
{
    uint16 w_str[MMIIDLE_TIME_LEN] = {0};
    GUIWIN_STBDATA_TEXT_T text_info = {0};

    MMIAPICOM_StatusAreaGetSysTime(w_str, MMIIDLE_TIME_LEN);
    text_info.is_display = TRUE;
    text_info.wstr_len = MMIAPICOM_Wstrlen(w_str);//MMIIDLE_TIME_LEN;

    SCI_MEMCPY(text_info.wstr_ptr, w_str,
               sizeof(wchar)*MIN(text_info.wstr_len, MMIIDLE_TIME_LEN));
    text_info.font_color = MMI_WHITE_COLOR;//MMI_GRAY_WHITE_COLOR; CRNEWMS00203710
    text_info.align = ALIGN_RVMIDDLE;
    text_info.font_type = MMI_STATUS_FONT_TYPE;

    GUIWIN_SetStbItemText(MMICOM_WIN_TEXT_TIME, &text_info);

    return TRUE;
}
/*****************************************************************************/
//  Description :  设置文本内容
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 文本内容为时间
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetDispTime(void)
{
    if (StatusbarSetDispTime())
    {
        SCI_TRACE_LOW("MMIAPICOM_StatusAreaSetDispTime Set Time OK!");
        GUIWIN_UpdateStb();
    }
}

/*****************************************************************************/
//  Description :  获取状态栏timer是否可见
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 文本内容为时间
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_StatusAreaIsDispTime(void)
{
    return GUIWIN_IsStbItemVisible(MMICOM_WIN_TEXT_TIME);
}

/*****************************************************************************/
//  Description :  设置状态栏是否显示timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 文本内容为时间
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetTimeVisible(
    BOOLEAN is_true
)
{
    if(s_is_lockscreen)//(MMIAPIKL_IsPhoneLocked())//lockscreen不显示
    {
        SCI_TRACE_LOW("MMIAPICOM_StatusAreaSetTimeVisible lock screen should not display timer info");
        return;
    }
    if(MMIAPIIDLE_IdleWinIsOpen() && MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
        SCI_TRACE_LOW("MMIAPICOM_StatusAreaSetTimeVisible idle screen should not display timer info");
        return;
    }
    GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_TIME, is_true);
}


/*****************************************************************************/
//  Description :  set status bar time info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_StatusAreaSetDispTimeinfo(wchar *time_buf, uint16 time_len)
{
    BOOLEAN result = FALSE;
    if (PNULL != time_buf && 0 != time_len)
    {
        GUIWIN_STBDATA_TEXT_T text_info = {0};
        text_info.is_display = TRUE;
        text_info.wstr_len = time_len;//MMIIDLE_TIME_LEN;

        SCI_MEMCPY(text_info.wstr_ptr, time_buf,
                   sizeof(wchar)*MIN(text_info.wstr_len, 10));
        text_info.font_color = MMI_WHITE_COLOR;//MMI_GRAY_WHITE_COLOR; CRNEWMS00203710
        text_info.align = ALIGN_HVMIDDLE;//ALIGN_LEFT;
        text_info.font_type = MMI_STATUS_FONT_TYPE;

        GUIWIN_SetStbItemText(MMICOM_WIN_TEXT_TIME, &text_info);
        result = TRUE;
    }
    return result;
}
/*****************************************************************************/
// Description : 获得时间字符串
// Global resource dependence :
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaGetSysTime(
    wchar *time_wstr,
    uint16 wlen
)
{
    uint8 time_str[MMIIDLE_TIME_LEN]  = {0};
    uint16 time_wstr_len = 0;
    SCI_TIME_T  sys_time = {0};
    // BOOLEAN                     is_12hour       = FALSE;
    // BOOLEAN                     is_pm           = FALSE;

    if (PNULL == time_wstr)
    {
        //MMI_ASSERT_WARNING("PNULL != time_wstr");
        SCI_TRACE_LOW("MMIAPICOM_StatusAreaGetSysTime ERROR! PNULL == time_wstr");
        return;
    }
    //get system time
    TM_GetSysTime(&sys_time);

    MMIAPISET_FormatTimeStrByTime(sys_time.hour, sys_time.min, (uint8 *)time_str, MMIIDLE_TIME_LEN);
    time_wstr_len = strlen((char *)time_str);
    time_wstr_len = MIN(time_wstr_len, wlen);
    //convert string
    MMI_STRNTOWSTR(time_wstr, MMIIDLE_TIME_LEN, time_str, MMIIDLE_TIME_LEN, time_wstr_len);

    return;
}

/*****************************************************************************/
// Description : 显示漫游图标
// Global resource dependence :
// Author: wancan.you
// Note:
/*****************************************************************************/
LOCAL void StatusAreaSetRoamingInfo(uint32 item_index, BOOLEAN is_append)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    GUIWIN_STBDATA_ITEM_DATA_T item_data = {0};
    uint32 i = 0;
    uint16 roaming_index = 0;
    BOOLEAN is_display = FALSE;

    const MMI_IMAGE_ID_T roaming_icon[] =
    {
        IMAGE_NULL,
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        IMAGE_IDLE_ROAM
#else
        //--------------------SIM4 SIM3 SIM2 SIM1
        IMAGE_IDLE_ROAM_SIM1,//0x01------0001
        IMAGE_IDLE_ROAM_SIM2,//0x02------0010
        IMAGE_IDLE_ROAM_SIM1_SIM2,//0x03------0011
#endif
    };
    i = item_index -MMICOM_WIN_ICON_ROAMING ;
  //  for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
  //  {
        if (MMIPHONE_ROAMING_STATUS_NO_ROMING != MMIAPIPHONE_GetRoamingStatus(i)
                && MMIIDLE_GetIdleWinInfo()->is_roaming[i]
                /*&& MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting(i)*/)
        {
            MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;
            BOOLEAN is_ps_ready = FALSE;
            
            plmn_status = MMIAPIPHONE_GetPlmnStatus(i);
            is_ps_ready = MMIAPIPHONE_GetIsPsReady(i);
            if(PLMN_NO_SERVICE == plmn_status
                || PLMN_EMERGENCY_ONLY == plmn_status
                || PLMN_EMERGENCY_SIM_INVALID == plmn_status
                || PLMN_EMERGENCY_GPRS_ONLY == plmn_status
                || PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY == plmn_status
                || PLMN_REGISTER_SERVICE == plmn_status
                || PLMN_REGISTER_GPRS_ONLY == plmn_status
                || !is_ps_ready)
            {
                roaming_index |= (0x01 << i);
                is_display = FALSE;
            }
            else
            {
                roaming_index |= (0x01 << i);
                is_display = TRUE;
            }
        }
   // }

    SCI_TRACE_LOW("StatusAreaSetRoamingInfo is_display=%d, roaming_index=%d", is_display, roaming_index);

    if (is_display && roaming_index < sizeof(roaming_icon) / sizeof(roaming_icon[0]))
    {
        image_id = roaming_icon[roaming_index];
    }
    else
    {
        image_id = IMAGE_NULL;
    }

    item_data.icon_info.icon_num = 1;
    item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;
    item_data.icon_info.is_display = TRUE;
    item_data.cur_frame = 0;

    item_data.icon_info.icon_array[0] = image_id;
    item_data.app_handle_func = PNULL;
    item_data.is_handle_callback = FALSE;
    if (is_append)
    {
        GUIWIN_AppendStbItemData(&item_data);
        GUIWIN_SetStbItemVisible(item_index, FALSE);
    }
    else
    {
        GUIWIN_SetStbItemIconId(item_index, image_id);
        if (IMAGE_NULL != image_id)
        {
            if(!s_is_lockscreen)
            {
                GUIWIN_SetStbItemVisible(item_index, TRUE);
            }
            else
            {
                GUIWIN_SetStbItemVisible(item_index, FALSE);
            }
        }
        else
        {
            GUIWIN_SetStbItemVisible(item_index, FALSE);
        }
    }

    GUIWIN_SetStbItemRightAlign(item_index, FALSE);
}

/*****************************************************************************/
// Description : 获取线路设置的图标
// Global resource dependence :
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T StatusAreaGetLineCallForwardIconId(void)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    uint32 i = 0;
    uint16 forward_index = 0;
    BOOLEAN is_display = FALSE;

    const MMI_IMAGE_ID_T forward_icon[] =
    {
        IMAGE_NULL,
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        IMAGE_IDLE_TITLE_ICON_FORWARD
#else
        //---------------------------------SIM2 SIM1
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM1,//IMAGE_IDLE_TITLE_ICON_FORWARD_SIM1,//0x01------0001
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM2,//IMAGE_IDLE_TITLE_ICON_FORWARD_SIM2,//0x02------0010
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM1_SIM2,//IMAGE_IDLE_TITLE_ICON_FORWARD_SIM1_SIM2,//0x03------0011
#endif
    };

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (1 == MMIIDLE_GetIdleWinInfo()->is_forward[i])//只有在线路2下在显示
        {
            forward_index |= (0x01 << i);
            is_display = TRUE;
        }
    }

    SCI_TRACE_LOW("StatusAreaGetLineCallForwardIconId is_display=%d, forward_index=%d", is_display, forward_index);

    if (is_display && forward_index < sizeof(forward_icon) / sizeof(forward_icon[0]))
    {
        image_id = forward_icon[forward_index];
    }
    else
    {
        image_id = IMAGE_NULL;
    }

    return image_id;
}

/*****************************************************************************/
// Description : 获取环境设置的图标
// Global resource dependence :
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T StatusAreaGetEvnSetIconId(void)
{
    MMI_IMAGE_ID_T  image_id        = IMAGE_NULL;
    uint8           envset_mode     = 0;

    envset_mode = MMIAPIENVSET_GetActiveModeId();
    switch(envset_mode)
    {
    case MMIENVSET_STANDARD_MODE:  // 普通环境（常规）
        image_id = (MMI_IMAGE_ID_T)IMAGE_NULL;
        break;

    case MMIENVSET_SILENT_MODE:     // 安静环境（静音）
        image_id = (MMI_IMAGE_ID_T)IMAGE_IDLE_TITLE_ICON_SILENT;
        break;

    #ifdef ZDT_PRODUCT_VIBRATE
    case MMIENVSET_MEETING_MODE:     // 会议环境（震动）
        image_id = (MMI_IMAGE_ID_T)IMAGE_IDLE_TITLE_ICON_VIBRATION;
        break;

    case MMIENVSET_INSIDE_MODE:     // 室内环境（低音）
        image_id = (MMI_IMAGE_ID_T)IMAGE_IDLE_TITLE_ICON_LOW_RING;
        break;

    case MMIENVSET_NOISY_MODE:       //户外环境（高音震动）
        image_id = (MMI_IMAGE_ID_T)IMAGE_IDLE_TITLE_ICON_RING_VIBRA;
        break;
    #endif

    default:
        image_id = (MMI_IMAGE_ID_T)IMAGE_NULL;
        break;
    };

    return image_id;
}

/*****************************************************************************/
//  Description : set the roaming state of the idle window
//  Global resource dependence :
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetRoamingState(
    MN_DUAL_SYS_E    e_dual_sys,
    BOOLEAN is_roaming
)
{
    MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;
    BOOLEAN is_ps_ready = FALSE;
    BOOLEAN is_emergency_service = FALSE;

    if (MMI_DUAL_SYS_MAX <= e_dual_sys)
    {
        //MMI_ASSERT_WARNING("e_dual_sys < MMI_DUAL_SYS_MAX");
        SCI_TRACE_LOW("MMIAPICOM_StatusAreaSetRoamingState ERROR! MMI_DUAL_SYS_MAX <= e_dual_sys,e_dual_sys = %d", e_dual_sys);
        return;
    }
    plmn_status = MMIAPIPHONE_GetPlmnStatus(e_dual_sys);
    is_ps_ready = MMIAPIPHONE_GetIsPsReady(e_dual_sys);
    if(PLMN_NO_SERVICE == plmn_status
        || PLMN_EMERGENCY_ONLY == plmn_status
        || PLMN_EMERGENCY_SIM_INVALID == plmn_status
        || PLMN_EMERGENCY_GPRS_ONLY == plmn_status
        || PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY == plmn_status
        || PLMN_REGISTER_SERVICE == plmn_status
        || PLMN_REGISTER_GPRS_ONLY == plmn_status
        || !is_ps_ready)
    {
        is_emergency_service = TRUE;
    }	
    SCI_TRACE_LOW("MMIAPICOM_StatusAreaSetRoamingState: e_dual_sys=%d, is_roaming = %d,is_emergency_service=%d", e_dual_sys, is_roaming,is_emergency_service);

    if((is_roaming != MMIIDLE_GetIdleWinInfo()->is_roaming[e_dual_sys]) || is_roaming)
    {
        MMIIDLE_GetIdleWinInfo()->is_roaming[e_dual_sys] = is_roaming;

        //SPEC中目前没有R图标
#if 1
        StatusAreaSetRoamingInfo(MMICOM_WIN_ICON_ROAMING+e_dual_sys, FALSE);
#endif
#if 0
        StatusAreaSetSimInfo(e_dual_sys, (uint32)MMICOM_WIN_ICON_SIGNAL + ((uint32)e_dual_sys - MN_DUAL_SYS_1), FALSE);
#endif

        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，Roaming状态
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}

/*****************************************************************************/
//  Description : set the Alarm state of the idle window
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaUpdateEarPhoneState(void)
{
    if(GPIO_CheckHeadsetStatus())
    {   
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_EARPHONE, TRUE); 
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_EARPHONE, FALSE);
    }
    GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_EARPHONE, FALSE);

    // update
    //GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
    //刷新小屏的显示，短信状态
    //MMISUB_UpdateDisplaySubLcd();
#endif
}

/*****************************************************************************/
//  Description : set the Alarm state of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaUpdateEnvsetState(void)
{
    MMI_IMAGE_ID_T      image_id    = IMAGE_NULL;

    image_id = StatusAreaGetEvnSetIconId();

    GUIWIN_SetStbItemIconId(MMICOM_WIN_ICON_ENVSET, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMICOM_WIN_ICON_ENVSET, 0);



    if (image_id != IMAGE_NULL)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ENVSET, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ENVSET, FALSE);
    }
    GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_ENVSET, FALSE);

    // update
    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
    //刷新小屏的显示，短信状态
    MMISUB_UpdateDisplaySubLcd();
#endif
}


/*****************************************************************************/
//  Description : set the call forward state of the idle window
//  Global resource dependence :
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetForwardState(
    MN_DUAL_SYS_E dual_sys,
    BOOLEAN forward_state
)
{
    BOOLEAN             is_update   = FALSE;
    MMI_IMAGE_ID_T      image_id    = IMAGE_NULL;

    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        if(forward_state != MMIIDLE_GetIdleWinInfo()->is_forward[dual_sys])
        {
            MMIIDLE_GetIdleWinInfo()->is_forward[dual_sys] = forward_state;
            is_update = TRUE;
        }
    }

    // 更改图标
    image_id = StatusAreaGetLineCallForwardIconId();
    GUIWIN_SetStbItemIconId(MMICOM_WIN_ICON_CALL_FORWARD, image_id);

    // 设置item可见性

    if (image_id != IMAGE_NULL)
    {
        if(!s_is_lockscreen)
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALL_FORWARD, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALL_FORWARD, FALSE);
        }
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALL_FORWARD, FALSE);
    }
    GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_CALL_FORWARD, FALSE);

    if (is_update)
    {
        // 刷新控件
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，闹钟状态
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}

/*****************************************************************************/
//  Description : 判断是否时号码快速查询
//  Global resource dependence :
//  Author: bown.zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T StatusAreaGetMessageDispImageId(void)
{
    MMI_IMAGE_ID_T          image_id    =   IMAGE_NULL;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPISMS_IsOrderOk())
    {
        image_id = IMAGE_NULL;
    }
#if 0
    else if (MMIAPISMS_IsSMSFull()) //短信满
    {
        image_id = IMAGE_NULL;//IMAGE_IDLE_TITLE_ICON_FULL_MESSAGE;
    }
    //     else if (MMIAPIMMS_IsMMSFull()) //彩信满
    //     {
    //         image_id = IMAGE_IDLE_TITLE_ICON_FULL_MMS_DUA;
    //     }
#ifndef MONOCHROME_LCD_SUPPORT
    else if (MMIAPISMS_IsSendingSMS())
    {
        image_id = IMAGE_NULL;//IMAGE_IDLE_TITLE_ICON_MESSAGE_SEND;
    }
#endif
    else if(MMIAPIMMS_IsSendingMMS())//彩信下载中
    {
        image_id = IMAGE_NULL;//IMAGE_IDLE_TITLE_ICON_MMSSEND;
    }
    else if(MMIAPIMMS_IsDownloadingMMS())//彩信下载中
    {
        image_id = IMAGE_NULL;//IMAGE_IDLE_TITLE_ICON_MMS;
    }
#endif
    else if (0 < MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_SMS)) //未读短信
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MESSAGE;
    }
    else if (0 < MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_MAX))//未下载彩信
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MESSAGE;
    }
    else if (0 < MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_MMS)) //未读彩信
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MESSAGE;
    }
    //else if ( MMICMSBRW_IsPushFull()) //push信箱满
    //{
    //        image_id = IMAGE_CMSBRW_PUSH_FULL;
    //}
    else if (0 < MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_WAP_PUSH)) //未读push消息
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MESSAGE;
    }
    else if (0 < MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_WAP_OTA)) //未读WAP OTA
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MESSAGE;
    }
#else
    if (!MMIAPISMS_IsOrderOk())
    {
        image_id = IMAGE_NULL;
    }
#if 0
    else if (MMIAPISMS_IsSMSFull())//短信满
    {
        image_id = IMAGE_NULL;//IMAGE_IDLE_TITLE_ICON_FULL_MESSAGE;
    }
#ifndef MONOCHROME_LCD_SUPPORT
    else if (MMIAPISMS_IsSendingSMS())
    {
        image_id = IMAGE_NULL;//IMAGE_IDLE_TITLE_ICON_MESSAGE_SEND;
    }
#endif
    else if(MMIAPIMMS_IsSendingMMS())//彩信下载中
    {
        image_id = IMAGE_NULL;//IMAGE_IDLE_TITLE_ICON_MMSSEND;
    }
    else if(MMIAPIMMS_IsDownloadingMMS())//彩信下载中
    {
        image_id = IMAGE_NULL;//IMAGE_IDLE_TITLE_ICON_MMS;
    }
#endif
    else if (0 < MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_1, MMISMS_TYPE_SMS)) //未读短信
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MESSAGE;
    }
    else if (0 < MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_1))//未下载彩信
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MESSAGE;
    }
    else if (0 < MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_1, MMISMS_TYPE_MMS)) //未读彩信
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MESSAGE;
    }
    //else if( MMICMSBRW_IsPushFull()) //push信箱满
    //{
    //        image_id = IMAGE_CMSBRW_PUSH_FULL;
    //}
    else if (0 < MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_1, MMISMS_TYPE_WAP_PUSH)) //未读push消息
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MESSAGE;
    }
    else if (0 < MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_1, MMISMS_TYPE_WAP_OTA)) //未读WAP OTA
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MESSAGE;
    }
#endif

    return image_id;
}


/*****************************************************************************/
//  Description : set msg state
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetMsgState(BOOLEAN is_draw)
{
    MMI_IMAGE_ID_T      image_id    =   IMAGE_NULL;

    image_id = StatusAreaGetMessageDispImageId();

    GUIWIN_SetStbItemIconId(MMICOM_WIN_ICON_MESSAGE, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMICOM_WIN_ICON_MESSAGE, 0);

    if (image_id != IMAGE_NULL)
    {
        if(!s_is_lockscreen)
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_MESSAGE, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_MESSAGE, FALSE);
        }
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_MESSAGE, FALSE);
    }
    GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_MESSAGE, FALSE);

    if (is_draw)
    {
        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，短信状态
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}
/*****************************************************************************/
// Description : 显示图标
// Global resource dependence :
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetGprsInfo(
    MN_DUAL_SYS_E dual_sys,
    MMIPDP_NOTIFY_EVENT_E pdp_event,
    BOOLEAN is_append
)
{
    StatusAreaSetGprsInfo(dual_sys, pdp_event, is_append);
}

/*****************************************************************************/
// Description : 显示图标
// Global resource dependence :
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL void StatusAreaSetGprsInfo(
    MN_DUAL_SYS_E dual_sys,
    MMIPDP_NOTIFY_EVENT_E pdp_event,
    BOOLEAN is_append
)
{
    uint32 i = 0;
    if (MMI_DUAL_SYS_MAX <= dual_sys)
    {
        SCI_TRACE_LOW("StatusAreaSetGprsInfo error dual_sys: %d ", dual_sys);
        return;
    }
#if defined CMCC_UI_STYLE || defined NET_ICON_UI_STYLE
    SCI_TRACE_LOW("StatusAreaSetGprsInfo: s_pdp_event[%d]=%d, pdp_event = %d", dual_sys, s_pdp_event[dual_sys],pdp_event);
    s_pdp_event[dual_sys] = (pdp_event <= MMIPDP_NOTIFY_EVENT_MAX) ? pdp_event : MMIPDP_NOTIFY_EVENT_MAX;
#endif
    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        StatusAreaSetSimInfo((MN_DUAL_SYS_E)i, MMICOM_WIN_ICON_SIGNAL + i, FALSE);
    }
}

/*****************************************************************************/
//  Description : set the gprs state of the idle window
//  Global resource dependence :
//  Author: figo.feng
//  Note: DUAL_MODE下只做关闭图标用
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetGprsState(
    MN_DUAL_SYS_E    e_dual_sys,
    BOOLEAN          gprs_state
)
{

    if (MMI_DUAL_SYS_MAX <= e_dual_sys)
    {
        //MMI_ASSERT_WARNING("e_dual_sys < MMI_DUAL_SYS_MAX");
        SCI_TRACE_LOW("MMIAPICOM_StatusAreaSetGprsState ERROR! MMI_DUAL_SYS_MAX <= e_dual_sys,e_dual_sys = %d", e_dual_sys);
        return;
    }
    SCI_TRACE_LOW("MMIAPICOM_StatusAreaSetGprsState: e_dual_sys=%d, gprs_state = %d", e_dual_sys, gprs_state);
    if (gprs_state)
    {
        if(MMIAPIPDP_PsIsActivedPdpLinkExistEx(e_dual_sys))  /*@CR255469 modify 2011.08.23*/
        {
            StatusAreaSetGprsInfo(e_dual_sys, MMIPDP_NOTIFY_EVENT_ACTIVED, FALSE);
        }
        else
        {
            StatusAreaSetGprsInfo(e_dual_sys, MMIPDP_NOTIFY_EVENT_ATTACH, FALSE);
        }
    }
    else
    {
        StatusAreaSetGprsInfo(e_dual_sys, MMIPDP_NOTIFY_EVENT_DETTACH, FALSE);
    }
    // update
    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
    //刷新小屏的显示，Gprs状态
    MMISUB_UpdateDisplaySubLcd();
#endif
}

/*****************************************************************************/
//  Description : handle rxlevel bound timer
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StatusAreaStartRxLevelTimer(MN_DUAL_SYS_E dual_sys, BOOLEAN is_need_bound)
{
    BOOLEAN     is_need_start_timer     = FALSE;

    s_signal_bound_timer[dual_sys] = 0;

    if (MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys] < MMIIDLE_GetIdleWinInfo()->rxlevel[dual_sys])
    {
        is_need_start_timer = TRUE;

        if (is_need_bound)
        {
            MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys]++;
        }
    }
    else if (MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys] > MMIIDLE_GetIdleWinInfo()->rxlevel[dual_sys])
    {
        is_need_start_timer = TRUE;

        if (is_need_bound)
        {
            MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys]--;
        }
    }

    if (is_need_start_timer)
    {
        uint32 time_out = MMI_SIGNAL_IDLE_MODE_TIMEOUT;

        if (MMIAPICC_IsInState(CC_IN_CALL_STATE)
#ifdef BROWSER_SUPPORT
                || MMIAPIBROWSER_IsRunning()
#endif
           )
        {
            time_out = MMI_SIGNAL_DEDICATED_MODE_TIMEOUT;
        }
        else
        {
            time_out = MMI_SIGNAL_IDLE_MODE_TIMEOUT;
        }

        s_signal_bound_timer[dual_sys] = MMK_CreateTimerCallback(time_out, StatusAreaHandleRxLevelTimer , NULL, FALSE);
    }

    return is_need_start_timer;
}

/*****************************************************************************/
//  Description : handle rxlevel bound timer
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void StatusAreaHandleRxLevelTimer(uint8 timer_id, uint32 param)
{
    BOOLEAN is_refresh = FALSE;
    uint32 i = 0;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (timer_id == s_signal_bound_timer[i])
        {
            is_refresh = StatusAreaStartRxLevelTimer(i, TRUE);

            if (is_refresh)
            {
                StatusAreaSetSimInfo((MN_DUAL_SYS_E)i, MMICOM_WIN_ICON_SIGNAL + i, FALSE);
            }
        }

    }

    if (is_refresh)
    {
        if (MMIDEFAULT_IsBacklightOn())
        {
            //update
            GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
            //刷新小屏的显示，刷新信号
            MMISUB_UpdateDisplaySubLcd();
#endif
        }
    }
}

/*****************************************************************************/
//  Description : get the rx level
//
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICOM_StatusAreaGetRxLevel(MN_DUAL_SYS_E e_dual_sys)
{
    return MMIIDLE_GetIdleWinInfo()->rxlevel[e_dual_sys];
}

/*****************************************************************************/
//  Description : set the rx level of the idle window
//    is_direct:TRUE直接显示信号跳变；FALSE平滑显示信号跳变
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetRxLevel(MN_DUAL_SYS_E e_dual_sys, uint8 rx_level, BOOLEAN is_direct)
{
    if (MMI_DUAL_SYS_MAX <= e_dual_sys)
    {
        //MMI_ASSERT_WARNING("e_dual_sys < MMI_DUAL_SYS_MAX");
        SCI_TRACE_LOW("MMIAPICOM_StatusAreaSetRxLevel ERROR! MMI_DUAL_SYS_MAX <= e_dual_sys,e_dual_sys = %d", e_dual_sys);
        return;
    }
    if (rx_level != MMIIDLE_GetIdleWinInfo()->rxlevel_step[e_dual_sys])
    {
        MMIIDLE_GetIdleWinInfo()->rxlevel[e_dual_sys] = rx_level;

        SCI_TRACE_LOW("MMIAPICOM_StatusAreaSetRxLevel %d,%d,%d, %d", rx_level, MMIIDLE_GetIdleWinInfo()->rxlevel[e_dual_sys], MMIIDLE_GetIdleWinInfo()->rxlevel_step[e_dual_sys], is_direct);


        if (is_direct)
        {
            if (0 != s_signal_bound_timer[e_dual_sys])//stop timer
            {
                MMK_StopTimer(s_signal_bound_timer[e_dual_sys]);
                s_signal_bound_timer[e_dual_sys] = 0;
            }            
            MMIIDLE_GetIdleWinInfo()->rxlevel_step[e_dual_sys] = MMIIDLE_GetIdleWinInfo()->rxlevel[e_dual_sys];
        }
        else
        {
            if (0 == s_signal_bound_timer[e_dual_sys])
            {
                StatusAreaStartRxLevelTimer(e_dual_sys, FALSE);
            } 
            else
            {
                return;
            }
        }

        StatusAreaSetSimInfo(e_dual_sys, (uint32)MMICOM_WIN_ICON_SIGNAL + ((uint32)e_dual_sys - MN_DUAL_SYS_1), FALSE);

        if (MMIDEFAULT_IsBacklightOn())
        {
            //update
            GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
            //刷新小屏的显示，刷新信号
            MMISUB_UpdateDisplaySubLcd();
#endif
        }
    }
    else
    {
        if (0 != s_signal_bound_timer[e_dual_sys])
        {
            MMK_StopTimer(s_signal_bound_timer[e_dual_sys]);
            s_signal_bound_timer[e_dual_sys] = 0;
        }
    }
}

/*****************************************************************************/
//  Description : set the blue tooth state of when power on
//  Global resource dependence :
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetBTStateWhenPoweron(
    BLUETOOTH_STATE_TYPE  bluetooth_state
)
{
    SCI_TRACE_LOW("MMIAPICOM_StatusAreaSetBTStateWhenPoweron %d", bluetooth_state);
    MMIAPICOM_StatusAreaSetBlueToothState(bluetooth_state);
}

/*****************************************************************************/
//  Description : set the blue tooth state of the idle window
//  Global resource dependence :
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetBlueToothState(
    BLUETOOTH_STATE_TYPE  bluetooth_state
)
{
#ifdef  BLUETOOTH_SUPPORT /* lint_lai */
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;

    SCI_TRACE_LOW("MMIAPICOM_StatusAreaSetBlueToothState %d", bluetooth_state);
    if (bluetooth_state != MMIIDLE_GetIdleWinInfo()->bluetooth_state)
    {
        MMIIDLE_GetIdleWinInfo()->bluetooth_state = bluetooth_state;

        if ( ON_AND_VISIBLE == MMIIDLE_GetIdleWinInfo()->bluetooth_state)
        {
            image_id = (MMI_IMAGE_ID_T)IMAGE_IDLE_TITLE_ICON_BT_VISIBLE;
            //blue tooth is power on and visible
        }
        else if ( ON_AND_HIDDEN == MMIIDLE_GetIdleWinInfo()->bluetooth_state)
        {
            image_id = (MMI_IMAGE_ID_T)IMAGE_IDLE_TITLE_ICON_BT_VISIBLE;
            //blue tooth is power on and hidden
        }
        else
        {
            image_id = (MMI_IMAGE_ID_T)IMAGE_NULL;
        }
        if(ON_AND_HEADSETCONNECT == MMIIDLE_GetIdleWinInfo()->bluetooth_state)
        {
            image_id = (MMI_IMAGE_ID_T)IMAGE_BT_HEADSET_CONNECT;
        }
        GUIWIN_SetStbItemIconId(MMICOM_WIN_ICON_BLUE_TOOTH, image_id);

        if (image_id != IMAGE_NULL)
        {
            if(!s_is_lockscreen)
            {
                GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_BLUE_TOOTH, TRUE);
            }
            else
            {
                GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_BLUE_TOOTH, FALSE);
            }
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_BLUE_TOOTH, FALSE);
        }
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_BLUE_TOOTH, FALSE);

        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
#endif
}
/*****************************************************************************/
//  Description : set the charge state
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetNetWorkName(void)
{
#if 0
    uint32 i = 0;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        StatusAreaSetSimInfo((MN_DUAL_SYS_E)i, MMICOM_WIN_ICON_SIGNAL + i, FALSE);
    }

    MMIIDLE_SendMsgToIdleWin(MSG_IDLE_UPDATE_NETWORK, PNULL);
#else
    SCI_TRACE_LOW("[idle]: MMIAPICOM_StatusAreaSetNetWorkName(): Net work status update");
    if(MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
    {
        MMK_PostMsg(MAIN_IDLE_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
    }
#endif
}

/*****************************************************************************/
//  Description : set the charge state
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaUpdateSignalState(void)
{
    uint32 i = 0;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        StatusAreaSetSimInfo((MN_DUAL_SYS_E)i, MMICOM_WIN_ICON_SIGNAL + i, FALSE);
#ifdef MMI_VOLTE_SUPPORT
        StatusAreaSetVolteInfo((MN_DUAL_SYS_E)i, MMICOM_WIN_ICON_VOLTE + i, FALSE);
#endif
    }

    // update
    GUIWIN_UpdateStb();
}

/*****************************************************************************/
//  Description : set the charge state
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetChargeState(
    BOOLEAN is_charge
)
{
    GUIWIN_STBDATA_ICON_STYLE_E icon_type = GUIWIN_STBDATA_ICON_NONE;
    MMI_IMAGE_ID_T icon_id = IMAGE_NULL;

    // 这个函数需要处理多种情况：Idle是否处于Focus, Charge的变化情况
    // 根据当前Idle窗口是否处于Focus来显示相关的东东！
    //if (is_charge != MMIIDLE_GetIdleWinInfo()->is_charge)//NEWMS00195807
    {
        MMIIDLE_GetIdleWinInfo()->is_charge = is_charge;

        MMIIDLE_GetIdleWinInfo()->batlevel = MMIAPIPHONE_GetBatCapacity();

        if (is_charge)
        {
            icon_type = GUIWIN_STBDATA_ICON_ANIM;
        }
        else
        {
            icon_type = GUIWIN_STBDATA_ICON_NORMAL;
        }
        SCI_TRACE_LOW("MMIAPICOM_StatusAreaSetChargeState %d, %d, %d, %d", MMIIDLE_GetIdleWinInfo()->batlevel, MMIIDLE_GetIdleWinInfo()->is_charge, icon_type, MMIIDLE_GetIdleWinInfo()->cur_batlevel);

        GUIWIN_SetStbItemIconStyle(MMICOM_WIN_ICON_BATTERY, icon_type);
        GUIWIN_SetStbItemIconCurFrame(MMICOM_WIN_ICON_BATTERY, MMIIDLE_GetIdleWinInfo()->batlevel);
        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
        MMISUB_SetBatteryLevel();
        //刷新小屏的显示，充电
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}

/*****************************************************************************/
//  Description : set the battery level of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetBatLevel(
    uint8 bat_level
)
{
    GUIWIN_STBDATA_ICON_STYLE_E     icon_type   =    GUIWIN_STBDATA_ICON_NONE;
    MMI_IMAGE_ID_T icon_id = IMAGE_NULL;

    SCI_TRACE_LOW("MMIAPICOM_StatusAreaSetBatLevel bat_level = %d,idle level =%d", bat_level, MMIIDLE_GetIdleWinInfo()->batlevel);
    if(bat_level != MMIIDLE_GetIdleWinInfo()->batlevel)
    {
        MMIIDLE_GetIdleWinInfo()->batlevel = bat_level;

        GUIWIN_GetStbItemIconStyle(MMICOM_WIN_ICON_BATTERY, &icon_type);
        GUIWIN_SetStbItemIconCurFrame(MMICOM_WIN_ICON_BATTERY, MMIIDLE_GetIdleWinInfo()->batlevel);
        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，刷新电量
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}


/*****************************************************************************/
//  Description : set missed call state
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetCallingState(BOOLEAN is_calling)
{
    MMI_IMAGE_ID_T      image_id    =   IMAGE_IDLE_TITLE_ICON_CALLING;

    if (MMIAPICC_IsExistUncipheringCall())
    {
        image_id = IMAGE_IDLE_TITLE_ICON_CALLING;//IMAGE_IDLE_TITLE_ICON_UNCIPHERING_CALLING;
    }
    GUIWIN_SetStbItemIconId(MMICOM_WIN_ICON_CALLING, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMICOM_WIN_ICON_CALLING, 0);

    MMIIDLE_GetIdleWinInfo()->is_calling = is_calling;

    if((MMIIDLE_GetIdleWinInfo()->is_calling))
    {
        if(!s_is_lockscreen)
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALLING, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALLING, FALSE);
        }
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALLING, FALSE);
    }
    GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_CALLING, FALSE);

    // update
    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
    //刷新小屏的显示，闹钟状态
    MMISUB_UpdateDisplaySubLcd();
#endif
}

/*****************************************************************************/
//  Description : set missed call state
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetMissedCallState(BOOLEAN is_missed_call)
{
    MMI_IMAGE_ID_T      image_id    =   IMAGE_IDLE_TITLE_ICON_MISSED_CALL;
    GUIWIN_SetStbItemIconId(MMICOM_WIN_ICON_MISSED_CALL, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMICOM_WIN_ICON_MISSED_CALL, 0);

    if(is_missed_call != MMIIDLE_GetIdleWinInfo()->is_missed_call)
    {
        MMIIDLE_GetIdleWinInfo()->is_missed_call = is_missed_call;



        if((MMIIDLE_GetIdleWinInfo()->is_missed_call))
        {
            if(!s_is_lockscreen)
            {
                GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_MISSED_CALL, TRUE);
            }
            else
            {
                GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_MISSED_CALL, FALSE);
            }
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_MISSED_CALL, FALSE);
        }
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_MISSED_CALL, FALSE);

        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，闹钟状态
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetAllAppCallbackValid(
    BOOLEAN is_valid
)
{
    uint32      i   = 0;

    for (i = 0; i < MMICOM_WIN_ICON_MAX; i++)
    {
        GUIWIN_SetItemHandleFuncValid(i, is_valid);
    }
}

/*****************************************************************************/
//  Description : set the Alarm state of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetAlarmState(
    BOOLEAN alarm_state
)
{
    if(alarm_state != MMIIDLE_GetIdleWinInfo()->is_alarm)
    {
        MMIIDLE_GetIdleWinInfo()->is_alarm = alarm_state;



        if((TRUE == MMIIDLE_GetIdleWinInfo()->is_alarm))
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ALARM, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ALARM, FALSE);
        }
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_ALARM, FALSE);

        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，闹钟状态
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}

/*****************************************************************************/
//  Description : set the java state of the idle window
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaUpdateJavaState(BOOLEAN java_state)
{
#if 0//UNISOC_MMI_Delete
#if !defined(JAVA_SUPPORT_TYPE_SVM) || (defined(JAVA_SUPPORT_TYPE_SVM) && defined(SUPPORT_BACKGROUND_RUN))
#if ( !defined(DPHONE_SUPPORT) && defined(JAVA_SUPPORT))
    if(java_state)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_JAVA, TRUE);

#ifdef PDA_UI_DROPDOWN_WIN
        MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_JAVA);//cr00115796 renwei modify
#endif
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_JAVA, FALSE);
#ifdef PDA_UI_DROPDOWN_WIN
        MMIDROPDOWNWIN_DelNotifyRecord(DROPDOWN_RUNNING_JAVA);//cr00115796 renwei modify
#endif
    }
    GUIWIN_SetStbItemRightAlign(MMICOM_WIN_JAVA, FALSE);
    // update
    //GUIWIN_UpdateStb();
    //刷新小屏的显示，短信状态
    //MMISUB_UpdateDisplaySubLcd();
#endif
#endif
#endif
}

/*****************************************************************************/
//  Description : set the vibrator state of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetShakeState(
    BOOLEAN shake_state
)
{
    MMIIDLE_GetIdleWinInfo()->is_shake = shake_state;

    //SCI_PASSERT((FALSE == s_idlewin_info.is_shake) || (FALSE == s_idlewin_info.is_silent), ("MMIAPICOM_StatusAreaSetShakeState vibrator and silent are true!"));
    if((TRUE == MMIIDLE_GetIdleWinInfo()->is_shake) && (TRUE == MMIIDLE_GetIdleWinInfo()->is_silent))
    {
        SCI_TRACE_LOW("MMIAPICOM_StatusAreaSetShakeState vibrator and silent are true!");
        return;
    }
    MMIAPICOM_StatusAreaUpdateEnvsetState();
}

/*****************************************************************************/
//  Description : if is silent of call ring in the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetSlientState(
    BOOLEAN silent_state
)
{
    MMIIDLE_GetIdleWinInfo()->is_silent = silent_state;

    //SCI_PASSERT((FALSE == s_idlewin_info.is_shake) || (FALSE == s_idlewin_info.is_silent), ("MMIAPICOM_StatusAreaSetSlientState vibrator and silent are true!"));
    if((TRUE == MMIIDLE_GetIdleWinInfo()->is_shake) && (TRUE == MMIIDLE_GetIdleWinInfo()->is_silent))
    {
        SCI_TRACE_LOW("MMIAPICOM_StatusAreaSetSlientState vibrator and silent are true!");
        return;
    }
    MMIAPICOM_StatusAreaUpdateEnvsetState();
}

/*****************************************************************************/
//  Description : 设置新语音消息图标
//  Global resource dependence :
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetVMFlagState( BOOLEAN vm_status)
{
    MMI_IMAGE_ID_T      image_id    =   IMAGE_IDLE_ICON_NEW_VM;
    if ( vm_status != MMIIDLE_GetIdleWinInfo()->is_new_vm )
    {
        MMIIDLE_GetIdleWinInfo()->is_new_vm = vm_status;
    }
    // update
    GUIWIN_SetStbItemIconId(MMICOM_WIN_ICON_VOICEMAIL, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMICOM_WIN_ICON_VOICEMAIL, 0);

    if((MMIIDLE_GetIdleWinInfo()->is_new_vm))
    {
        if(!s_is_lockscreen)
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_VOICEMAIL, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_VOICEMAIL, FALSE);
        }
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_VOICEMAIL, FALSE);
    }
    GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_VOICEMAIL, FALSE);

    // update
    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
    //刷新小屏的显示，闹钟状态
    MMISUB_UpdateDisplaySubLcd();
#endif
}

/*****************************************************************************/
//  Description : set the Stopwatch running state of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetStopwatchRunningState(
    BOOLEAN stopwatch_running_state
)
{
    MMI_IMAGE_ID_T      image_id    =   IMAGE_STATUS_STOPWATCH;
    GUIWIN_SetStbItemIconId(MMICOM_WIN_ICON_STOPWATCH, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMICOM_WIN_ICON_STOPWATCH, 0);

    if(stopwatch_running_state != MMIIDLE_GetIdleWinInfo()->is_stopwatch_running)
    {
        MMIIDLE_GetIdleWinInfo()->is_stopwatch_running = stopwatch_running_state;

        if((TRUE == MMIIDLE_GetIdleWinInfo()->is_stopwatch_running))
        {
            if(!s_is_lockscreen)
            {
                GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_STOPWATCH, TRUE);
            }
            else
            {
                GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_STOPWATCH, FALSE);
            }
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_STOPWATCH, FALSE);
        }
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_STOPWATCH, FALSE);

        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，闹钟状态
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}

/*****************************************************************************/
//  Description : set the Countdown Timer(when is set) state of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetCountdownTimerState(
    BOOLEAN countdown_timer_state
)
{
    MMI_IMAGE_ID_T      image_id    =   IMAGE_STATUS_COUNTDOWN;
    GUIWIN_SetStbItemIconId(MMICOM_WIN_ICON_COUNTDOWN_TIMER, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMICOM_WIN_ICON_COUNTDOWN_TIMER, 0);

    if(countdown_timer_state != MMIIDLE_GetIdleWinInfo()->is_countdown_timer)
    {
        MMIIDLE_GetIdleWinInfo()->is_countdown_timer = countdown_timer_state;

        if((TRUE == MMIIDLE_GetIdleWinInfo()->is_countdown_timer))
        {
            if(!s_is_lockscreen)
            {
                GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_COUNTDOWN_TIMER, TRUE);
            }
            else
            {
                GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_COUNTDOWN_TIMER, FALSE);
            }
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_COUNTDOWN_TIMER, FALSE);
        }
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_COUNTDOWN_TIMER, FALSE);

        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，闹钟状态
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}

/*****************************************************************************/
//  Description : set the Calendar Reminder(Reminder snoozed) state of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetReminderState(
    BOOLEAN reminder_state
)
{
    MMI_IMAGE_ID_T      image_id    =   IMAGE_STATUS_REMINDER;
    GUIWIN_SetStbItemIconId(MMICOM_WIN_ICON_CALENDAR_REMINDER, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMICOM_WIN_ICON_CALENDAR_REMINDER, 0);

    if(reminder_state != MMIIDLE_GetIdleWinInfo()->is_reminder)
    {
        MMIIDLE_GetIdleWinInfo()->is_reminder = reminder_state;

        if((TRUE == MMIIDLE_GetIdleWinInfo()->is_reminder))
        {
            if(!s_is_lockscreen)
            {
                GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALENDAR_REMINDER, TRUE);
            }
            else
            {
                GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALENDAR_REMINDER, FALSE);
            }
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALENDAR_REMINDER, FALSE);
        }
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_CALENDAR_REMINDER, FALSE);

        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，闹钟状态
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}

/*****************************************************************************/
//  Description : set the usb mode of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetUSBConnectedState(
    BOOLEAN usb_connected_state
)
{
    MMI_IMAGE_ID_T      image_id    =   IMAGE_STATUS_USB_CONNECTED;
    GUIWIN_SetStbItemIconId(MMICOM_WIN_ICON_USB_MODE, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMICOM_WIN_ICON_USB_MODE, 0);

    if(usb_connected_state != MMIIDLE_GetIdleWinInfo()->is_usb_mode)
    {
        MMIIDLE_GetIdleWinInfo()->is_usb_mode = usb_connected_state;

        if((TRUE == MMIIDLE_GetIdleWinInfo()->is_usb_mode))
        {
            if(!s_is_lockscreen)
            {
                GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_USB_MODE, TRUE);
            }
            else
            {
                GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_USB_MODE, FALSE);
            }
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_USB_MODE, FALSE);
        }
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_USB_MODE, FALSE);

        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，闹钟状态
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}

/*****************************************************************************/
//  Description : set the General Application Notification state of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetAppNoticeState(
    BOOLEAN app_notice_state
)
{
    MMI_IMAGE_ID_T      image_id    =   IMAGE_STATUS_GENERAL_APPLICATION_NOTIFICATION;
    GUIWIN_SetStbItemIconId(MMICOM_WIN_ICON_APP_NOTICE, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMICOM_WIN_ICON_APP_NOTICE, 0);

    if(app_notice_state != MMIIDLE_GetIdleWinInfo()->is_app_notice)
    {
        MMIIDLE_GetIdleWinInfo()->is_app_notice = app_notice_state;

        if((TRUE == MMIIDLE_GetIdleWinInfo()->is_app_notice))
        {
            if(!s_is_lockscreen)
            {
                GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_APP_NOTICE, TRUE);
            }
            else
            {
                GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_APP_NOTICE, FALSE);
            }
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_APP_NOTICE, FALSE);
        }
        GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_APP_NOTICE, FALSE);

        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，闹钟状态
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}


/*****************************************************************************/
//  Description : set torch state
//  Global resource dependence : 
//  Author: Cheney.Wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetTorchState(BOOLEAN is_torch_on)
{
#if defined(TORCH_SUPPORT)
    MMI_IMAGE_ID_T      image_id    =   IMAGE_IDLE_TITLE_ICON_FLASHLIGHT;
    GUIWIN_SetStbItemIconId(MMICOM_WIN_ICON_TORCH, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMICOM_WIN_ICON_TORCH, 0);
    
        
    if(is_torch_on)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_TORCH, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_TORCH, FALSE);
    }
    
    GUIWIN_SetStbItemRightAlign(MMICOM_WIN_ICON_TORCH, FALSE);
    
    // update 
    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE        
    //刷新小屏的显示，闹钟状态
    MMISUB_UpdateDisplaySubLcd();  
#endif
#endif
}


/*****************************************************************************/
//  Description :  set status bar viewheader info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_StatusAreaSetViewHeaderinfo(wchar *item_str, uint16 item_len)
{
#ifdef STATUS_TEXT_SUPPORT
    MMI_HANDLE_T win_handle= MMK_GetFocusWinHandle();

    if (PNULL == item_str || 0 == item_len)
    {
        return FALSE;
    }

   if(win_handle == 0)
    {
         MMIAPICOM_StatusAreaSetViewHeaderinfoEx(item_str,item_len);
         return TRUE;
    }
    if(MMK_GetWinDisplayStyleState( win_handle, WS_HAS_PROMPT_WIN ))
    {
         MMIAPICOM_StatusAreaSetViewHeaderinfoEx(item_str,item_len);
         return TRUE;
    }
    
    if (GUIWIN_HasTitle(win_handle) 
#ifdef MMI_ETWS_SUPPORT
		&& (!MMK_IsOpenWin(MMISMS_CB_READ_WIN_ID))
#endif
		)
        {
            GUIWIN_SetTitleText(win_handle, item_str, item_len,TRUE);
        }
    else
        {   //Don't have title
            if(GUIWIN_CreateTitleDyna(win_handle,TXT_NULL))
            {      
                GUIWIN_SetTitleText(win_handle, item_str, item_len,TRUE);
            }
            else
            {
                //Create Title failed
                MMIAPICOM_StatusAreaSetViewHeaderinfoEx(item_str,item_len);
            }
        }

    return TRUE;



#if 0
    BOOLEAN result = FALSE;
    if (PNULL != item_str && 0 != item_len)
    {
    
        GUIWIN_STBDATA_TEXT_T text_info = {0};

        text_info.is_display = TRUE;
        text_info.wstr_len = item_len;

        SCI_MEMCPY(text_info.wstr_ptr, item_str,
                   sizeof(wchar)*MIN(text_info.wstr_len, MMISTATUSBAR_ITEM_TEXT_LEN));
        text_info.font_color = MMI_WHITE_COLOR;
        text_info.align = ALIGN_LVMIDDLE;//ALIGN_LEFT;
        text_info.font_type = MMI_STATUS_FONT_TYPE;

        GUIWIN_SetStbItemText(MMICOM_WIN_TEXT_INFO, &text_info);
        if(!s_is_lockscreen)
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_INFO, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_INFO, FALSE);
        }


        result = TRUE;
    }
    return result;
    #endif
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description :  set status bar viewheader info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(MMI_TEXT_ID_T text_id)
{
#ifdef STATUS_TEXT_SUPPORT
    MMI_HANDLE_T win_handle= MMK_GetFocusWinHandle();

   if(win_handle == 0)
    {
         MMIAPICOM_StatusAreaSetViewHeaderinfoByTextIdEx(text_id);
         return TRUE;
    }
    if(MMK_GetWinDisplayStyleState( win_handle, WS_HAS_PROMPT_WIN ))
        {
         MMIAPICOM_StatusAreaSetViewHeaderinfoByTextIdEx(text_id);
         return TRUE;
    }

    if (GUIWIN_HasTitle(win_handle))
        {
            GUIWIN_SetTitleTextId(win_handle, text_id, TRUE);
        }
    else
        {
            if(GUIWIN_CreateTitleDyna(win_handle,text_id))
                {
                    GUIWIN_UpdateTitle(win_handle);
                }
            else
                {       //Create Title failed,use original api interface
                MMIAPICOM_StatusAreaSetViewHeaderinfoByTextIdEx(text_id);
            }
    }
    return TRUE;

#if 0  
    MMI_STRING_T text_str = {0};
    GUIWIN_STBDATA_TEXT_T text_info = {0};
    MMI_GetLabelTextByLang(text_id,&text_str);

    text_info.is_display = TRUE;
    text_info.wstr_len = text_str.wstr_len;

    SCI_MEMCPY(text_info.wstr_ptr, text_str.wstr_ptr,
               sizeof(wchar)*MIN(text_info.wstr_len, MMISTATUSBAR_ITEM_TEXT_LEN));
    text_info.font_color = MMI_WHITE_COLOR;
    text_info.align = ALIGN_LVMIDDLE;//ALIGN_LEFT;
    text_info.font_type = MMI_STATUS_FONT_TYPE;

    GUIWIN_SetStbItemText(MMICOM_WIN_TEXT_INFO, &text_info);
    if(!s_is_lockscreen)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_INFO, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_INFO, FALSE);
    }
   
    return TRUE;
#endif
    
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description :  set status bar viewheader info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_StatusAreaSetViewHeaderinfoEx(wchar *item_str, uint16 item_len)
{
#ifdef STATUS_TEXT_SUPPORT
    BOOLEAN result = FALSE;
    if (PNULL != item_str && 0 != item_len)
    {
        GUIWIN_STBDATA_TEXT_T text_info = {0};

        text_info.is_display = TRUE;
        text_info.wstr_len = item_len;

        SCI_MEMCPY(text_info.wstr_ptr, item_str,
                   sizeof(wchar)*MIN(text_info.wstr_len, MMISTATUSBAR_ITEM_TEXT_LEN));
        text_info.font_color = MMI_WHITE_COLOR;
        text_info.align = ALIGN_LVMIDDLE;//ALIGN_LEFT;
        text_info.font_type = MMI_STATUS_FONT_TYPE;

        GUIWIN_SetStbItemText(MMICOM_WIN_TEXT_INFO, &text_info);

        if(MMIIDLE_GetLockStatus()!=1)       // if(!s_is_lockscreen)
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_INFO, FALSE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_INFO, FALSE);
        }
        result = TRUE;
    }
    return result;
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description :  set status bar viewheader info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_StatusAreaSetViewHeaderinfoByTextIdEx(MMI_TEXT_ID_T text_id)
{
#ifdef STATUS_TEXT_SUPPORT
    MMI_STRING_T text_str = {0};
    GUIWIN_STBDATA_TEXT_T text_info = {0};
    MMI_GetLabelTextByLang(text_id,&text_str);

    text_info.is_display = TRUE;
    text_info.wstr_len = text_str.wstr_len;

    SCI_MEMCPY(text_info.wstr_ptr, text_str.wstr_ptr,
               sizeof(wchar)*MIN(text_info.wstr_len, MMISTATUSBAR_ITEM_TEXT_LEN));
    text_info.font_color = MMI_WHITE_COLOR;
    text_info.align = ALIGN_LVMIDDLE;//ALIGN_LEFT;
    text_info.font_type = MMI_STATUS_FONT_TYPE;

    GUIWIN_SetStbItemText(MMICOM_WIN_TEXT_INFO, &text_info);
    if(!s_is_lockscreen)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_INFO, FALSE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_INFO, FALSE);
    }
    return TRUE;
#else
    return FALSE;
#endif
}


/*****************************************************************************/
// Description :
//
//Charging icon (either charger plugged or battery full)
//Profile indicator, if sound profile is set (e.g. silent)
//Alarm on indicator (active alarm or snoozed alarm)
//Charging, profile and alarm are displayed from right to left.
//
//-------------------------------------------------------------------
//|Name                         |   Alignment       |   Priority    |
//|-----------------------------|-------------------|---------------|
//|Charging                     |   Right           |   0           |
//|-----------------------------|-------------------|---------------|
//|Profile indicator            |   Right           |0              |
//|-----------------------------|-------------------|---------------|
//|Alarm                        |   Right           |0              |
//-------------------------------------------------------------------
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaLockscreenSet(void)
{
    uint32      i   =   0;
    s_is_withheader = FALSE;
    s_is_lockscreen = TRUE;
    //先将所有的item设置为not visible状态
    for(i = 0; i < MMICOM_WIN_ICON_MAX; i++)
    {
        GUIWIN_SetStbItemVisible(i, FALSE);
    }

    //Charging
#if 0
    if (MMIIDLE_GetIdleWinInfo()->is_charge)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_BATTERY, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_BATTERY, FALSE); 
    }
#endif
    //Profile indicator
    if(IMAGE_NULL != StatusAreaGetEvnSetIconId())
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ENVSET, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ENVSET, FALSE);
    }
    //Alarm
    if(MMIIDLE_GetIdleWinInfo()->is_alarm)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ALARM, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ALARM, FALSE);
    }

    MMIAPICOM_StatusAreaUpdateEarPhoneState();

}

/*****************************************************************************/
// Description :
//
//- Status area with all indicators
//Used in device startup and on top of idle screen
//
//Status area of idle screen includes indicator
//area in XX px width. Between each indicator
//there is a 2px margin. For visual details or
//indicator icon, refer to Status area UI screens
//and visual guideline.
//If there are more indicators to be shown
//than fit to indicator area only the highest
//priority ones will be visible.
//
//---------------------------------------------------------------------------------------
//|Name                                     |   Alignment       |   Priority    |
//|-----------------------------------------|-------------------|-----------------------|
//|Battery indicator                        |   Right           |   0 (Always visible)  |
//|-----------------------------------------|-------------------|-----------------------|
//|Flight mode is on                        |   Right           |   0                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Network status icons for both SIM cards  |   Right           |   1                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Background call ongoing                  |   Right           |   2                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Call forwarding                          |   Right           |   3                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Voice Mail                               |   Right           |   4                   |
//|-----------------------------------------|-------------------|-----------------------|
//|New Message(s)                           |   Right           |   5                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Missed Call                              |   Right           |   5                   |
//|-----------------------------------------|-------------------|-----------------------|
//|General Applicat ion Notification        |   Right           |   5                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Alarm Clock(Alarm is set)                |   Right           |   6                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Calendar Reminder(Reminder snoozed)      |   Right           |   7                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Countdown Timer(when is set)             |   Right           |   8                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Stopwatch running                        |   Right           |   9                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Accessory connected(headset)             |   Right           |   10                  |
//---------------------------------------------------------------------------------------
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaIdlescreenSet(void)
{
    MMI_IMAGE_ID_T          image_id    =   IMAGE_NULL;
    uint32      i   =   0;
    s_is_withheader = FALSE;
    s_is_lockscreen = FALSE;

    //先将所有的item设置为visible状态
    for(i = 0; i < MMICOM_WIN_ICON_MAX; i++)
    {
        GUIWIN_SetStbItemVisible(i, FALSE);
    }

    //Battery indicator
    GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_BATTERY, TRUE);
    //Flight mode is on
    //Network status icons for both SIM cards
    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        StatusAreaSetSimInfo((MN_DUAL_SYS_E)i, MMICOM_WIN_ICON_SIGNAL + i, FALSE);
#ifdef MMI_VOLTE_SUPPORT
        StatusAreaSetVolteInfo((MN_DUAL_SYS_E)i,MMICOM_WIN_ICON_VOLTE+i, FALSE);
#endif
#ifndef WIN32
        if(MMIPHONE_ROAMING_STATUS_NO_ROMING != MMIAPIPHONE_GetRoamingStatus(i)
            && MMIIDLE_GetIdleWinInfo()->is_roaming[i])
        {
            StatusAreaSetRoamingInfo(MMICOM_WIN_ICON_ROAMING+i, FALSE);
        }
#endif
    }

    //Background call ongoing
    if(MMIIDLE_GetIdleWinInfo()->is_calling)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALLING, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALLING, FALSE);
    }
    //Call forwarding
    {
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (1 == MMIIDLE_GetIdleWinInfo()->is_forward[i])
            {
                GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALL_FORWARD, TRUE);
            }
        }
    }
    //Voice Mail
    if(MMIIDLE_GetIdleWinInfo()->is_new_vm)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_VOICEMAIL, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_VOICEMAIL, FALSE);
    }
    //New Message(s)
    image_id = StatusAreaGetMessageDispImageId();
    if (image_id != IMAGE_NULL)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_MESSAGE, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_MESSAGE, FALSE);
    }
    //Missed Call
    if(MMIIDLE_GetIdleWinInfo()->is_missed_call)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_MISSED_CALL, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_MISSED_CALL, FALSE);
    }
    //General Applicat ion Notification
    if(MMIIDLE_GetIdleWinInfo()->is_app_notice)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_APP_NOTICE, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_APP_NOTICE, FALSE);
    }
    //Profile indicator

    if(IMAGE_NULL != StatusAreaGetEvnSetIconId())
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ENVSET, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ENVSET, FALSE);
    }
    //Alarm Clock(Alarm is set)
    if(MMIIDLE_GetIdleWinInfo()->is_alarm)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ALARM, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ALARM, FALSE);
    }
    //Calendar Reminder(Reminder snoozed)
    if(MMIIDLE_GetIdleWinInfo()->is_reminder)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALENDAR_REMINDER, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALENDAR_REMINDER, FALSE);
    }
    //Countdown Timer(when is set)
    if(MMIIDLE_GetIdleWinInfo()->is_countdown_timer)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_COUNTDOWN_TIMER, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_COUNTDOWN_TIMER, FALSE);
    }
    //Stopwatch running
    if(MMIIDLE_GetIdleWinInfo()->is_stopwatch_running)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_STOPWATCH, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_STOPWATCH, FALSE);
    }
#ifdef BLUETOOTH_SUPPORT
    if(MMIIDLE_GetIdleWinInfo()->bluetooth_state != OFF_BLUETOOTH)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_BLUE_TOOTH, TRUE);
    }
#endif
    //Accessory connected(headset)
#ifdef BLUETOOTH_SUPPORT
    if(MMIIDLE_GetIdleWinInfo()->bluetooth_state != OFF_BLUETOOTH)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_BLUE_TOOTH, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_BLUE_TOOTH, FALSE);
    }
#endif

    //USB mode(USB cable)
    if(MMIIDLE_GetIdleWinInfo()->is_usb_mode)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_USB_MODE, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_USB_MODE, FALSE);
    }

#if defined(TORCH_SUPPORT)
    if(TRUE == MMIIDLE_GetTorchStatus())
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_TORCH, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_TORCH, FALSE);
    }
#endif

    MMIAPICOM_StatusAreaUpdateEarPhoneState();

}

/*****************************************************************************/
// Description :
//
//- Status area with header
//Used on top of main menu and all applications
//
//Status area on Main Menu is divided on to 2
//sections: Header area and indicator area.
//Header area contains a header text and is 122
//px in width. Indicator area is 106 px in width.
//Between each indicator there is a 2px margin.
//For visual details or indicator icon, refer to
//Status area UI screens and visual guideline.
//
//If there are more indicators to be shown than
//fit to indicator area only the highest priority
//ones will be visible.
//
//The network status indicator will be shown
//only if there is no mobile network available (on
//DS product if both networks are out of reach)
//
//---------------------------------------------------------------------------------------
//|Name                                     |   Alignment       |   Priority    |
//|-----------------------------------------|-------------------|-----------------------|
//|Time                                     |   Right           |   0 (Always visible)  |
//|-----------------------------------------|-------------------|-----------------------|
//|Battery indicator                        |   Right           |   0 (Always visible)  |
//|-----------------------------------------|-------------------|-----------------------|
//|View header                              |   Left            |   0 (Always visible)  |
//|-----------------------------------------|-------------------|-----------------------|
//|Flight mode is on                        |   Right           |   0                   |
//|-----------------------------------------|-------------------|-----------------------|
//|No Network                               |                   |                       |
//|(if both SIM-cards are without network)  |   Right           |   1                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Ongoing mobile network traffic           |   Right           |   1                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Background call ongoing                  |   Right           |   2                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Call forwarding                          |   Right           |   3                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Voice Mail                               |   Right           |   4                   |
//|-----------------------------------------|-------------------|-----------------------|
//|New Message(s)                           |   Right           |   5                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Missed Call                              |   Right           |   5                   |
//|-----------------------------------------|-------------------|-----------------------|
//|General Applicat ion Notification        |   Right           |   5                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Alarm Clock(Alarm is set)                |   Right           |   6                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Calendar Reminder(Reminder snoozed)      |   Right           |   7                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Countdown Timer(when is set)             |   Right           |   8                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Stopwatch running                        |   Right           |   9                   |
//|-----------------------------------------|-------------------|-----------------------|
//|Accessory connected(headset)             |   Right           |   10                  |
//---------------------------------------------------------------------------------------
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaWithheaderSet(void)
{
    MMI_IMAGE_ID_T          image_id    =   IMAGE_NULL;
    uint32      i   =   0;

    s_is_withheader = TRUE;
    s_is_lockscreen = FALSE;

    //先将所有的item设置为visible状态
    for(i = 0; i < MMICOM_WIN_ICON_MAX; i++)
    {
        GUIWIN_SetStbItemVisible(i, FALSE);
    }
    //View header
    {
#ifdef STATUS_TEXT_SUPPORT
        GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_INFO, FALSE);
#endif
    }
#if defined (MAINLCD_SIZE_240X320)	
    //Time
    {
        StatusbarSetDispTime();
        GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_TIME, FALSE);
    }
#endif
    //Battery indicator
    GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_BATTERY, TRUE);
    //Flight mode is on
    //Network status icons for both SIM cards
    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        StatusAreaSetSimInfo((MN_DUAL_SYS_E)i, MMICOM_WIN_ICON_SIGNAL + i, FALSE);
#ifdef MMI_VOLTE_SUPPORT
        StatusAreaSetVolteInfo((MN_DUAL_SYS_E)i,MMICOM_WIN_ICON_VOLTE+i, FALSE);
#endif
#ifndef WIN32
        if(MMIPHONE_ROAMING_STATUS_NO_ROMING != MMIAPIPHONE_GetRoamingStatus(i)
            && MMIIDLE_GetIdleWinInfo()->is_roaming[i])
        {
            StatusAreaSetRoamingInfo(MMICOM_WIN_ICON_ROAMING+i, FALSE);
        }
#endif
    }

    //Background call ongoing
    if((TRUE == MMIIDLE_GetIdleWinInfo()->is_calling))
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALLING, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALLING, FALSE);
    }
    //Call forwarding
    {
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (1 == MMIIDLE_GetIdleWinInfo()->is_forward[i])
            {
                GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALL_FORWARD, TRUE);
            }
        }

    }
    //Voice Mail
    if(MMIIDLE_GetIdleWinInfo()->is_new_vm)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_VOICEMAIL, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_VOICEMAIL, FALSE);
    }
    //New Message(s)
    image_id = StatusAreaGetMessageDispImageId();
    if (image_id != IMAGE_NULL)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_MESSAGE, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_MESSAGE, FALSE);
    }
    //Missed Call
    if(MMIIDLE_GetIdleWinInfo()->is_missed_call)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_MISSED_CALL, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_MISSED_CALL, FALSE);
    }
    //General Applicat ion Notification
    if(MMIIDLE_GetIdleWinInfo()->is_app_notice)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_APP_NOTICE, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_APP_NOTICE, FALSE);
    }
    //Profile indicator
    if(IMAGE_NULL != StatusAreaGetEvnSetIconId())
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ENVSET, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ENVSET, FALSE);
    }
    //Alarm Clock(Alarm is set)
    if(MMIIDLE_GetIdleWinInfo()->is_alarm)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ALARM, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_ALARM, FALSE);
    }
    //Calendar Reminder(Reminder snoozed)
    if(MMIIDLE_GetIdleWinInfo()->is_reminder)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALENDAR_REMINDER, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_CALENDAR_REMINDER, FALSE);
    }
    //Countdown Timer(when is set)
    if(MMIIDLE_GetIdleWinInfo()->is_countdown_timer)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_COUNTDOWN_TIMER, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_COUNTDOWN_TIMER, FALSE);
    }
    //Stopwatch running
    if(MMIIDLE_GetIdleWinInfo()->is_stopwatch_running)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_STOPWATCH, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_STOPWATCH, FALSE);
    }
    //Accessory connected(headset)
#ifdef BLUETOOTH_SUPPORT
    if(MMIIDLE_GetIdleWinInfo()->bluetooth_state != OFF_BLUETOOTH)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_BLUE_TOOTH, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_BLUE_TOOTH, FALSE);
    }
#endif

    //USB mode(USB cable)
    if(MMIIDLE_GetIdleWinInfo()->is_usb_mode)
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_USB_MODE, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_USB_MODE, FALSE);
    }

#if defined(TORCH_SUPPORT)
    if(TRUE == MMIIDLE_GetTorchStatus())
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_TORCH, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMICOM_WIN_ICON_TORCH, FALSE);
    }
#endif

    MMIAPICOM_StatusAreaUpdateEarPhoneState();

}

/*****************************************************************************/
//  Description :  get rat icon table info
//  Global resource dependence :
//  Author:zhongping.shi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsExistInRatIconTable(MN_PLMN_T * cur_plmn_ptr, char *icon_2Gtype, char *icon_4Gtype)
{
    uint16 i = 0;
    BOOLEAN isRaticonable = FALSE;
    
    for (i = 0 ; i < MAX_RATICON_NUM ; i++)
    {
        if ((s_raticon_table[i].mnc == cur_plmn_ptr->mnc)
            && (s_raticon_table[i].mcc == cur_plmn_ptr->mcc))
        {
            *icon_2Gtype = s_raticon_table[i].icon_2Gtype;
            *icon_4Gtype = s_raticon_table[i].icon_4Gtype;
            isRaticonable = TRUE;
            
            break;
        }
    }

    return isRaticonable;
}
#endif

