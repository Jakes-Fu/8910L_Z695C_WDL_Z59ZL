/*****************************************************************************
** File Name:      mmiidle_mstyle.c                                          *
** Author:                                                                   *
** Date:           09/09/2015                                                *
** Copyright:      2015 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe microsoft style idle screen       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 9/2015     grant.peng              Creat
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmi_app_idle_trc.h"
#include "mmiidle_mstyle.h"
#include "mmk_type.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiset_export.h"
#include "mmifm_export.h"
#include "mmiset_text.h"
#include "guires.h"
#include "mmiidle_func.h"
#include "mmicom_statusarea.h"
#include "mmicountedtime_export.h"
#include "mmiidle_func.h"
#include "mmi_id.h"
#include "mmi_appmsg.h"
#include "mmimp3_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "guistatusbar.h"
#include "mmi_image.h"
#include "guictrl_api.h"
#include "guilabel.h"
#include "mmicom_banner.h"
#include "window_parse.h"
#include "mmicl_export.h"
#include "mmipub.h"
#include "mmiset_nv.h"
#include "mmisrv_cameraroll_export.h"
#include "mmiflashlight_export.h"
#include "mmiset_id.h"
#include "mmiconnection_text.h"
#ifdef MMI_SMART_DUAL_SIM_SUPPORT
#include "smartdualsim_id.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


#define IDLE_MP3_TIMER_PERIOD       500 //idle下mp3名字显示的定时器周期
#define IDLE_LOCKED_FROM_MAIN_MENU_TIMER_PERIOD 30000
#define IDLE_UNLOCK_CONFIRM_TIME             5000
#ifndef UNISOC_VERSION
#define UNISOC_VERSION
#endif

#define FORM_BOTTOM_MAGRIN_HEIGHT 6
#define FORM_CHILD_BUTTON_HEIGHT 32

#define FORM_CHILD_FM_HEIGHT 72
#define FORM_CHILD_FM_FRQ_HEIGHT 30
#define FORM_CHILD_FM_NAVI_WIDTH 84

#define FORM_CHILD_MUSIC_HEIGHT 100//98+2
#define FORM_CHILD_MUSIC_NAME_HEIGHT 24
#define FORM_CHILD_MUSIC_NAME_WIDTH 228
#define FORM_CHILD_MUSIC_PROGRESS_HEIGHT 3
#define FORM_CHILD_MUSIC_NAVI_HEIGHT 35
#define FORM_CHILD_MUSIC_NAVI_WIDTH 228
#define FORM_CHILD_MUSIC_TIMEINFO_HEIGHT 24
#define FORM_CHILD_MUSIC_TIMEINFO_WIDTH 40
#define FORM_CHILD_MUSIC_BUTTON_HEIGHT 24
#define FORM_CHILD_MUSIC_BUTTON_WIDTH 24
#define FORM_CHILD_MUSIC_BUTTON_TOP_SPACE 11//space between progress and button 11
#define FORM_CHILD_MUSIC_BUTTON_PLAY_LEFT_SPACE 102//40+62
#define FORM_CHILD_MUSIC_BUTTON_NEXT_LEFT_SPACE 132//60+62+24+6

#define MMI_IDLE_NTF_LIST_FORM_HEIGHT_MAX (64*3 + FORM_BOTTOM_MAGRIN_HEIGHT)
//#define MMI_IDLE_NTF_LIST_FORM_HEIGHT_MAX (164 + FORM_BOTTOM_MAGRIN_HEIGHT)


#define MMI_SECURITY_CODE_MAX_LEN  8

#if defined (MAINLCD_SIZE_128X160)
#define MMI_IDLE_TIME_TOP 18
#define MMI_IDLE_TIME_MARGIN 6
#define MMI_IDLE_TIME_HEIGHT 58
#define MMI_IDLE_TIME_INTERVAL 4

#define MMI_IDLE_DATE_HEIGHT 24


#define MMI_IDLE_INTERVAL_MARGIN 1

#define MMI_IDLE_SIM_STATUS_TOP (MMI_IDLE_TIME_TOP + MMI_IDLE_TIME_HEIGHT/2 + MMI_IDLE_TIME_MARGIN)

#define MMI_IDLE_SIM_STATUS_HEIGHT 18
#define MMI_IDLE_SIM_STATUS_WIDTH 120
#define MMI_IDLE_SIM_ICON_HEIGHT 14
#define MMI_IDLE_SIM_ICON_WIDTH 14

#else
#define MMI_IDLE_TIME_TOP 24
#define MMI_IDLE_TIME_MARGIN 6
#define MMI_IDLE_TIME_HEIGHT 68
#define MMI_IDLE_TIME_INTERVAL 4

#define MMI_IDLE_DATE_HEIGHT 30


#define MMI_IDLE_INTERVAL_MARGIN 4

#define MMI_IDLE_SIM_STATUS_TOP (MMI_IDLE_TIME_TOP + MMI_IDLE_TIME_HEIGHT + MMI_IDLE_DATE_HEIGHT + 5 + 5 + 70)

#define MMI_IDLE_SIM_STATUS_HEIGHT 30
#define MMI_IDLE_SIM_STATUS_WIDTH 228
#define MMI_IDLE_SIM_ICON_HEIGHT 20
#define MMI_IDLE_SIM_ICON_WIDTH 20
#endif

#define  MMI_IDLE_NTF_INDICATOR_MAX  8

//notification indicator layout
#define MMI_IDLE_NTF_TOP_MARGIN 4
#define MMI_IDLE_NTF_LEFT_MARGIN 6
#define MMI_IDLE_NTF_INTERVAL_MARGIN 4
#define MMI_IDLE_NTF_WIDTH 26
#define MMI_IDLE_NTF_HEIGHT 26

#ifdef MMI_MULTI_SIM_SYS_DUAL
#define MMI_IDLE_NTF_AREA_TOP (MMI_IDLE_SIM_STATUS_TOP + MMI_IDLE_SIM_STATUS_HEIGHT*2 + MMI_IDLE_INTERVAL_MARGIN*2 + 2)
#else
#define MMI_IDLE_NTF_AREA_TOP (MMI_IDLE_SIM_STATUS_TOP + MMI_IDLE_SIM_STATUS_HEIGHT + MMI_IDLE_INTERVAL_MARGIN + 2)
#endif

#define MMI_IDLE_NTF_AREA_WIDTH 128
#define MMI_IDLE_NTF_AREA_HEIGHT 64


#ifdef  MMI_UNISOC_IDLE_EMERGENCY_CALL_SUPPORT
#define MMIIDLE_EMERGENCY_CALL_BG_ALPHA 255*0.8
#define MMIIDLE_EMERGENCY_CALL_LEN      20
#endif

#ifdef MAINLCD_SIZE_128X160 //arvin zhang added for idle softkey icon display
#define MMIIDLE_SOFTKEY_ICON_MAX        2//left icon plus right icon
#define MMIIDLE_SOFTKEY_MARGIN_WIDTH    5//margin space
#endif

#if defined MMI_MSTYLE_SUPPORT
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


#ifdef UI_MULTILAYER_SUPPORT
LOCAL GUI_LCD_DEV_INFO  s_common_idle_paint_layer_handle = {0, UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_common_idle_stb_layer_handle = {0, UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_common_idle_notification_layer_handle = {0, UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_common_idle_privacy_mode_layer_handle = {0, UILAYER_NULL_HANDLE};
#endif


#ifdef MAINLCD_SIZE_128X160 
#define MMI_COMMON_IDLE_ITEM_MAX_NUM 5
#else
#define MMI_COMMON_IDLE_ITEM_MAX_NUM 6
#endif

LOCAL BOOLEAN s_notification_form_auto_size = FALSE;
LOCAL GUI_RECT_T s_bottom_rect = {0}; //显示区域

LOCAL MMI_IDLE_LOCK_STATUS_E s_idle_lock_status = IDLE_UNLOCKED;
LOCAL BOOLEAN s_is_unlocked_in_30s = FALSE;
LOCAL BOOLEAN s_is_allow_locked_operation = FALSE;
LOCAL uint8 s_locked_from_main_menu_timer = 0;
LOCAL uint8 s_idle_unlock_timer_id = 0;

LOCAL int16 s_idle_privacy_mode_ntf_height = 0;
#ifdef CALENDAR_SUPPORT
LOCAL int16 calendar_bottom = 0;
#endif
LOCAL MMI_WIN_ID_T    s_active_app_win_id = 0;
LOCAL MMI_WIN_ID_T    s_active_app_previous_win_id = 0;

//LOCAL BOOLEAN s_is_hotkey_turnon_light = FALSE;

LOCAL BOOLEAN s_is_keydown_in_idle = FALSE;

LOCAL MMISET_SECURITY_CODE_T s_is_security_code = {0};

LOCAL BOOLEAN   s_is_need_input_securitycode = FALSE;

#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (KURO_SUPPORT)|| defined (FM_SUPPORT)

LOCAL IDLE_MP3_NAME_INFO    g_idle_cur_mp3_name = {0}; //当前播放的MP3名称

#endif
#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)                                        
LOCAL uint8 g_mp3_name_timer_id = 0; //Idle界面循环显示MP3歌词的定时器
#endif

//
//#define PHONE_LOCK_PS_FAST_SLEEP_TIMEOUT 3
//#define PHONE_UNLOCK_PS_FAST_SLEEP_TIMEOUT 12

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
extern BOOLEAN g_is_dtag_version;
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : create idle items paint layer
//  Global resource dependence :
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void CreateCommonIdlePaintLayer(
    MMI_WIN_ID_T      win_id
);

/*****************************************************************************/
//  Description : get idle items paint layer ptr
//  Global resource dependence :
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO *GetCommonIdlePaintLcdDev(void);

/*****************************************************************************/
//  Description : release idle paint layer
//  Global resource dependence :
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdlePaintLayer(void);

/*****************************************************************************/
//  Description : create status bar layer
//  Global resource dependence :
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void CreateCommonIdleStbLayer(
    MMI_WIN_ID_T      win_id
);

/*****************************************************************************/
//  Description : release idle paint layer
//  Global resource dependence :
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdleStbLayer(MMI_WIN_ID_T win_id);


/*****************************************************************************/
//  Description : get idle items paint layer ptr
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO *GetCommonIdleNotiLcdDev(void);

/*****************************************************************************/
//  Description : create notification layer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateCommonIdleNotiLayer(
    MMI_WIN_ID_T      win_id
);

/*****************************************************************************/
//  Description : release notification layer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdleNotiLayer(
    MMI_WIN_ID_T      win_id
);

/*****************************************************************************/
//  Description : create privacy mode notification indicator layer
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CreateCommonIdlePrivacyModeLayer(
    MMI_WIN_ID_T      win_id
);

/*****************************************************************************/
//  Description : release privacy mode notification indicator  layer
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdlePrivacyModeLayer(
    MMI_WIN_ID_T      win_id
);


/*****************************************************************************/
//  Description : apend list item
//  Global resource dependence :
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL void UpdateIdleWinPushNotificationList(
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UpdateIdleFmRadioNotification(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
/*****************************************************************************/
LOCAL void UpdateIdleMusicNotification(void);

/*****************************************************************************/
//  Description : display the  content
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void OutIdleWinContent(void);

PUBLIC void DisplayIdleWinSoftkey(void);

/*****************************************************************************/
//  Description : cal idle string start point
//  Global resource dependence :
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL GUI_POINT_T CalStartPoint(MMI_IDLE_DISPLAY_T disp_style, uint16 str_width);


/*****************************************************************************/
//  Description : output date
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void  OutputIdleWinWeekAndDate(void);

/*****************************************************************************/
//  Description : output counted time
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinCountedTime(BOOLEAN is_highlight);

#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
/*****************************************************************************/
//  Description : output idle win words
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinWords(void);
#endif

#ifdef CALENDAR_SUPPORT
/*****************************************************************************/
//  Description : output calendar
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinCalendar(void);
#endif

#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)
 /*****************************************************************************/
 //  Description : display the mp3 name or not
 //  Global resource dependence : 
 //  Author: Jassmine.Meng
 //  Note: 
 /*****************************************************************************/
 LOCAL void DisplayIdleMp3Name(void);
 
 /*****************************************************************************/
 //  Description : display the cycle mp3 name
 //  Global resource dependence : 
 //  Author: Jassmine.Meng
 //  Note: 
 /*****************************************************************************/
 LOCAL void DisplayIdleCycleMp3Name(void);
#endif


 /*****************************************************************************/
//  Description : handle press * to unlock
//  Global resource dependence :
//  Author: jianma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePressStarToUnlockWinMsg(
                    MMI_WIN_ID_T     win_id, 
                    MMI_MESSAGE_ID_E    msg_id, 
                    DPARAM              param);
 /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN InputSecurityCodeWinSoftkey(
                                      wchar         *str_ptr,   //in
                                      uint16        str_len,    //in
                                      MMI_TEXT_ID_T *text_id_ptr//in/out:
                                      );
/*****************************************************************************/
//  Description : handle input security code unlock
//  Global resource dependence :
//  Author: jianma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSecurityCodeWinMsg(
                    MMI_WIN_ID_T     win_id, 
                    MMI_MESSAGE_ID_E    msg_id, 
                    DPARAM              param);
#ifdef MMI_UNISOC_IDLE_EMERGENCY_CALL_SUPPORT
/*****************************************************************************/
//  Description : handle emergency call in idle 
//  Global resource dependence :
//  Author: jianma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsIdleEmergencyCallWinMsg(
                    MMI_WIN_ID_T     win_id, 
                    MMI_MESSAGE_ID_E    msg_id, 
                    DPARAM              param);

#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 获取shortcut app name 更新status bar
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetShortcutAppNameID(MMISET_FUNCKEY_E  index);


/*****************************************************************************/
//  Description : set notification list defaut index
//  Global resource dependence :
//  Author: Grant.Peng
//  Note:
/*****************************************************************************/
LOCAL void SetListBottomIndex(MMI_CTRL_ID_T ctrl_id);

/*
LOCAL MMI_RESULT_E  HandleLockBackgroundWinMsg(
                    MMI_WIN_ID_T     win_id, 
                    MMI_MESSAGE_ID_E    msg_id, 
                    DPARAM              param);


LOCAL BOOLEAN MMIAPIIDLE_LockPhoneEx(void);
*/

WINDOW_TABLE( MMIIDLE_INPUT_SECURITY_CODE_WINTAB ) =
{
        WIN_FUNC( (uint32)HandleSecurityCodeWinMsg ),    
        WIN_ID( MMIIDLE_SECURITY_CODE_WIN_ID ),
        WIN_STATUSBAR,
        //CREATE_ANIM_CTRL(MMI_DEVICELOCK_SECURITY_ANIM_ID,MMI_DEVICELOCK_SECURITY_WIN_ID),
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMIIDLE_SECURITY_CODE_LABEL_CTRL_ID),
        CREATE_EDIT_PASSWORD_CTRL(MMI_SECURITY_CODE_MAX_LEN,MMIIDLE_SECURITY_CODE_CTRL_ID),
        WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_OK_MK, TXT_NULL),
        END_WIN
};

#ifdef MMI_UNISOC_IDLE_EMERGENCY_CALL_SUPPORT
//ms锁屏/idle界面紧急呼叫窗口
WINDOW_TABLE( MMIIDLE_EMERGENCY_WIN_TAB ) = 
{
    
    WIN_FUNC((uint32)HandleMsIdleEmergencyCallWinMsg),    
    WIN_ID(MMIIDLE_EMERGENCY_CALL_WIN_ID),
    WIN_STATUSBAR,
    
    CREATE_EDIT_PHONENUM_CTRL(MMIIDLE_EMERGENCY_CALL_LEN, MMIIDLE_EMERGENCY_PHONENUM_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_CALL_MK, TXT_NULL),
    END_WIN
};
#endif

/*
WINDOW_TABLE( MMIIDLE_LOCK_BACKGROUND_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleLockBackgroundWinMsg),    
    WIN_ID(MMIIDLE_LOCK_BACKGROUND_WIN_ID),
    END_WIN
};
*/

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T GetNotificationIndicator(MMI_IDLE_PUSH_MSG_TYPE_E n_type)
{
    MMI_IMAGE_ID_T          image_id    =   IMAGE_NULL;
#ifdef UIX8910_ORG_IDLE_STYLE
    switch(n_type)
    {
    case MMI_IDLE_PUSH_MSG_SMS:
        image_id = IMAGE_IDLE_SCREEN_N_MESSAGE;
        break;
    case MMI_IDLE_PUSH_MSG_MISSED_CALL:
        image_id = IMAGE_IDLE_SCREEN_N_MISSED_CALL;
        break;
    case MMI_IDLE_PUSH_MSG_SNOOZED_ALARM:
        image_id = IMAGE_IDLE_SCREEN_N_SNOOZE;
        break;
    case MMI_IDLE_PUSH_MSG_SNOOZED_CALENDER:
        image_id = IMAGE_IDLE_SCREEN_N_CALENDAR;
        break;
    case MMI_IDLE_PUSH_MSG_VOICE_MAIL:
        image_id = IMAGE_IDLE_SCREEN_N_VOICEMAIL;
        break;
    case MMI_IDLE_PUSH_MSG_MAIL:
        image_id = IMAGE_IDLE_SCREEN_N_EMAIL;
        break;
    case MMI_IDLE_PUSH_MSG_OPERATER_SETTING:
        image_id = IMAGE_IDLE_SCREEN_N_MESSAGE;
        break;
        
    default:
        image_id = IMAGE_IDLE_SCREEN_N_GENERIC;
        break;
    }
#endif
    return image_id;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void GetNotificationIconRect(int index, GUI_RECT_T *rect_ptr)
{
    int one_line_num = MMI_IDLE_NTF_INDICATOR_MAX/2;
    if(PNULL != rect_ptr && index < MMI_IDLE_NTF_INDICATOR_MAX)
    {
        rect_ptr->top = MMI_IDLE_NTF_AREA_TOP + MMI_IDLE_NTF_INTERVAL_MARGIN 
                                    + (MMI_IDLE_NTF_INTERVAL_MARGIN + MMI_IDLE_NTF_HEIGHT) * (index / one_line_num);
        rect_ptr->left = MMI_IDLE_NTF_LEFT_MARGIN + MMI_IDLE_NTF_WIDTH * (index % one_line_num)
                                  + MMI_IDLE_NTF_INTERVAL_MARGIN * (index % one_line_num);

        rect_ptr->bottom = rect_ptr->top + MMI_IDLE_NTF_HEIGHT;
        rect_ptr->right= rect_ptr->left + MMI_IDLE_NTF_WIDTH;
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void DisplayOneNtfIconByImgId(int index, MMI_IMAGE_ID_T image_id)
{
    if(index < MMI_IDLE_NTF_INDICATOR_MAX)
    {
        GUIANIM_CTRL_INFO_T       control_info = {0};
        GUIANIM_DATA_INFO_T       data_info = {0};
        GUIANIM_DISPLAY_INFO_T display_info = {0};
       
        GUI_RECT_T               anim_rect = {0};
      

        GetNotificationIconRect(index, &anim_rect);

        //set anim param
        control_info.is_ctrl_id = TRUE;
        control_info.ctrl_id = MMIIDLE_NTF_ANIM_CTRL_ID_1 + index;

        data_info.img_id = image_id;

        display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
        display_info.is_play_once      = FALSE;
        display_info.is_syn_decode   = TRUE;
        display_info.is_update            = FALSE;
        display_info.is_disp_one_frame = TRUE;
    
        //set anim ctrl rect
        GUIANIM_SetCtrlRect(control_info.ctrl_id, &anim_rect);

        GUIANIM_SetDisplayRect(control_info.ctrl_id, &anim_rect, FALSE);

        GUIANIM_SetParam(&control_info, &data_info, PNULL, &display_info);

        GUIANIM_SetVisible(control_info.ctrl_id, TRUE, FALSE);
    }

}


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void DisplayOneNtfIconByImgFilePath(int index, GUIANIM_FILE_INFO_T *anim_path)
{
    if(PNULL != anim_path && index < MMI_IDLE_NTF_INDICATOR_MAX)
    {
        GUIANIM_CTRL_INFO_T       control_info = {0};
        GUIANIM_DISPLAY_INFO_T display_info = {0};
       
        GUI_RECT_T               anim_rect = {0};

        GetNotificationIconRect(index, &anim_rect);

        //set anim param
        control_info.is_ctrl_id = TRUE;
        control_info.ctrl_id = MMIIDLE_NTF_ANIM_CTRL_ID_1 + index;

        display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;

        //set anim ctrl rect
        GUIANIM_SetCtrlRect(control_info.ctrl_id, &anim_rect);

        GUIANIM_SetDisplayRect(control_info.ctrl_id, &anim_rect, FALSE);

        GUIANIM_SetParam(&control_info, PNULL,anim_path, &display_info);

        GUIANIM_SetVisible(control_info.ctrl_id, TRUE, FALSE);

    }

}


/******************************************************************************/
//  Description : Clear control img data.
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL void ClearNotificationAnimData(void)
{
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_FILE_INFO_T file_info  = {0};
    GUIANIM_DISPLAY_INFO_T display_info = {0};
    MMI_CTRL_ID_T ctrl_id = 0;
    uint32 ctrl_idx = 0;

    ctrl_info.is_wallpaper_ctrl = FALSE;
    ctrl_info.is_ctrl_id        = TRUE;
    ctrl_info.ctrl_id           = ctrl_id;
    ctrl_info.ctrl_ptr          = PNULL;

    file_info.full_path_wstr_ptr = PNULL;
    file_info.full_path_wstr_len = 0;

    display_info.align_style       = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_disp_one_frame = FALSE;
    display_info.is_crop_rect      = TRUE;
    display_info.is_free_buf_lose_focus = TRUE;

    for(ctrl_idx = 0; ctrl_idx < MMI_IDLE_NTF_INDICATOR_MAX; ctrl_idx++)
    {         
        ctrl_id = MMIIDLE_NTF_ANIM_CTRL_ID_1 + ctrl_idx;
        ctrl_info.ctrl_id           = ctrl_id;

        GUIANIM_SetParam(&ctrl_info,
                                            PNULL,
                                            &file_info,
                                            &display_info);
        GUIANIM_SetVisible(ctrl_id, FALSE, FALSE);
    }

}


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void DisplayNotifactionIndicator(void)
{
    IDLE_PUSH_MSG_NODE_T  *idle_push_msg_list_ptr[MMI_IDLE_NTF_INDICATOR_MAX] = {PNULL};
    BOOLEAN have_new_notification[MMI_IDLE_PUSH_MSG_TYPE_MAX] = {FALSE};
    IDLE_PUSH_MSG_NODE_T *tmp_msg_node_ptr = PNULL;
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    int msg_count = 0;
    int index = 0;
    int i = 0;
    
    for(i = MMI_IDLE_PUSH_MSG_MAX_COUNT; i > 0; i --)
    {
        tmp_msg_node_ptr = MMIIDLE_GetPushMsgNode(i - 1);
        if(PNULL != tmp_msg_node_ptr
            && MMI_IDLE_PUSH_MSG_STATE_VALID == tmp_msg_node_ptr->msg_state)
        {
            MMI_IDLE_PUSH_MSG_TYPE_E e_type = tmp_msg_node_ptr->idle_push_msg_info.e_push_msg_type;
            if(!have_new_notification[e_type] && MMI_IDLE_PUSH_MSG_ONGOING_CALL != e_type)
            {
                msg_count++;
                have_new_notification[e_type] = TRUE;
                idle_push_msg_list_ptr[MMI_IDLE_NTF_INDICATOR_MAX - msg_count] = tmp_msg_node_ptr;
                if(MMI_IDLE_NTF_INDICATOR_MAX == msg_count)
                {
                    break;
                }
            }
        }
    }

    for(i = MMI_IDLE_NTF_INDICATOR_MAX - msg_count; i < MMI_IDLE_NTF_INDICATOR_MAX; i ++)
    {
        tmp_msg_node_ptr = idle_push_msg_list_ptr[i];
        if(PNULL != tmp_msg_node_ptr
            && MMI_IDLE_PUSH_MSG_STATE_VALID == tmp_msg_node_ptr->msg_state)
        {
            MMI_IDLE_PUSH_MSG_TYPE_E e_type = tmp_msg_node_ptr->idle_push_msg_info.e_push_msg_type;
            if(MMI_IDLE_PUSH_MSG_WNS == e_type)
            {
                if(MMI_IDLE_ICON_DATA_ANIM_PATH == tmp_msg_node_ptr->idle_push_msg_info.icon_data_type)
                {
                    GUIANIM_FILE_INFO_T   anim_path = {0};
                    anim_path.full_path_wstr_len = MMIAPICOM_Wstrlen(tmp_msg_node_ptr->idle_push_msg_info.icon_data.full_path_wstr_ptr);
                    anim_path.full_path_wstr_ptr = tmp_msg_node_ptr->idle_push_msg_info.icon_data.full_path_wstr_ptr;
                    DisplayOneNtfIconByImgFilePath(index, &anim_path);
                }
                else
                {
                    image_id = GetNotificationIndicator(e_type);
                    DisplayOneNtfIconByImgId(index, image_id);
                }
            }
            else
            {
                image_id = GetNotificationIndicator(e_type);
                DisplayOneNtfIconByImgId(index, image_id);
            }
            index++;
        }
    }

    if(0 == msg_count)
    {
        s_idle_privacy_mode_ntf_height = 0;
    }
    else if(msg_count <= MMI_IDLE_NTF_INDICATOR_MAX/2)
    {
        s_idle_privacy_mode_ntf_height = MMI_IDLE_NTF_HEIGHT + MMI_IDLE_NTF_TOP_MARGIN;
    }
    else /* if(msg_count > MMI_IDLE_NTF_INDICATOR_MAX/2)*/
    {
        s_idle_privacy_mode_ntf_height = (MMI_IDLE_NTF_HEIGHT + MMI_IDLE_NTF_TOP_MARGIN)*2;
    }

}



/*****************************************************************************/
//  Description :Is security code enabled
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNotificationPrivacyMode(void)
{
    MN_RETURN_RESULT_E   return_value = MN_RETURN_FAILURE;
    BOOLEAN IsPrivacyMode = FALSE;

    MMINV_READ(MMINV_SET_PRIVACY_MODE, &IsPrivacyMode, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_PRIVACY_MODE, &IsPrivacyMode);
    }

    return IsPrivacyMode;
}


/*****************************************************************************/
//  Description :Is security code enabled
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_IsUnlockedIn30s(void)
{
    return s_is_unlocked_in_30s;
}

/*****************************************************************************/
//  Description :Is security code enabled
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_SetUnLockedIn30s(BOOLEAN is_unlocked_in_30s)
{
    s_is_unlocked_in_30s = is_unlocked_in_30s;
}

/*****************************************************************************/
//  Description :Is security code enabled
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_IsAllowLockedOperation(void)
{
    return s_is_allow_locked_operation;
}

/*****************************************************************************/
//  Description :Is security code enabled
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_SetAllowLockedOperation(BOOLEAN is_allow_locked_operation)
{
    s_is_allow_locked_operation = is_allow_locked_operation;
}

/*****************************************************************************/
//  Description : get win status
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IDLE_LOCK_STATUS_E MMIIDLE_GetLockStatus(void)
{
    return s_idle_lock_status;
}


/*****************************************************************************/
//  Description : set win status
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_SetLockStatus(MMI_IDLE_LOCK_STATUS_E status)
{
    s_idle_lock_status = status;
}

/*****************************************************************************/
//  Description : set idle lock mode
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_SetLockFromMainMenuMode(void)
{
    //MMIIDLE_SetUnLockedIn30s(TRUE);
}



/*****************************************************************************/
//  Description :Is security code enabled
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSecurityCodeEnabled(void)
{
    //read status from nv
#ifdef UNISOC_VERSION
    BOOLEAN b_ret = FALSE;
    MMISET_LOCKSCREEN_SECURITY_CODE_T devicelock_code = {0};
    b_ret = MMIDEVICELOCKSCREEN_ReadPwd(&devicelock_code);
    if(b_ret)
    {
        return devicelock_code.is_enable;
    }
    else
    {
        return FALSE;
    }
#else //#ifdef UNISOC_VERSION

    MN_RETURN_RESULT_E   return_value = MN_RETURN_FAILURE;
    MMINV_READ(MMINV_SET_SECURITY_CODE,&s_is_security_code,return_value);
    if(MN_RETURN_SUCCESS == return_value)
    {
        return s_is_security_code.is_enable;
    }
    else
    {
        return FALSE;
    }
#endif //#ifdef UNISOC_VERSION
    
}

/*****************************************************************************/
//  Description :Unlock phone to main menu
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UlockPhoneToActiveApp(void *para1, void *para2)
{

    MMI_HANDLE_T win_handle = MMK_ConvertIdToHandle(MAIN_IDLE_WIN_ID);

    MMK_WinResumeWindowFocus(win_handle);

    SCI_TRACE_LOW("[UlockPhoneToActiveApp] s_active_app_win_id = 0x%08x", s_active_app_win_id);
    SCI_TRACE_LOW("[UlockPhoneToActiveApp] s_active_app_previous_win_id = 0x%08x", s_active_app_previous_win_id);

    if(!MMK_IsOpenWin(MAIN_MAINMENU_WIN_ID))
    {
        if( MMK_IsOpenWin(PHONE_LANGUAGE_FIRST_POWER_WIN_ID) ||
            MMK_IsOpenWin(PHONE_WARRANTY_FIRST_POWER_WIN_ID) ||
            MMK_IsOpenWin(PHONE_TRANSPORT_FIRST_POWER_WIN_ID) ||
            MMK_IsOpenWin(PHONE_FIRST_POWER_DATE_SET_WIN_ID) ||
            MMK_IsOpenWin(MMISET_SET_DATE_TIME_LIST_WIN_ID)
#ifdef MMI_SMART_DUAL_SIM_SUPPORT
            || MMK_IsOpenWin(MMI_SMARTDUALSIM_PREFERRED_SIM_WIN_ID)
#endif
            )
        {
            if((0 != s_active_app_win_id) && MMK_IsOpenWin(s_active_app_win_id))
            {
                win_handle = MMK_ConvertIdToHandle(s_active_app_win_id);
                MMK_WinGrabFocus(s_active_app_win_id);            
                s_active_app_win_id = 0;
                s_active_app_previous_win_id = 0;
                return;
            }
            else if((0 != s_active_app_previous_win_id) && MMK_IsOpenWin(s_active_app_previous_win_id))
            {
                win_handle = MMK_ConvertIdToHandle(s_active_app_previous_win_id);
                MMK_WinGrabFocus(s_active_app_previous_win_id);
                s_active_app_win_id = 0;
                s_active_app_previous_win_id = 0;
                return;
            }
        }

        if(0 != s_active_app_win_id && MMK_IsOpenWin(s_active_app_win_id))
        {
            MMK_CloseWin(s_active_app_win_id);
        }
        
        if(0 != s_active_app_previous_win_id && MMK_IsOpenWin(s_active_app_previous_win_id))
        {
            MMK_CloseWin(s_active_app_previous_win_id);
        }
        
        MMIAPIMENU_CreatMainMenu();
        s_active_app_win_id = 0;
        s_active_app_previous_win_id = 0;
        return;
    }

    if(0 != s_active_app_win_id)
    {
        if(MMK_IsOpenWin(s_active_app_win_id))
        {
            win_handle = MMK_ConvertIdToHandle(s_active_app_win_id);
            MMK_WinGrabFocus(s_active_app_win_id);            
            s_active_app_win_id = 0;
            s_active_app_previous_win_id = 0;
            return;
        }
        else if(0 != s_active_app_previous_win_id && MMK_IsOpenWin(s_active_app_previous_win_id))
        {
            win_handle = MMK_ConvertIdToHandle(s_active_app_previous_win_id);
            MMK_WinGrabFocus(s_active_app_previous_win_id);
            s_active_app_win_id = 0;
            s_active_app_previous_win_id = 0;
            return;
        }
    }
    
    s_active_app_win_id = 0;
    s_active_app_previous_win_id = 0;
    
    if(MMK_IsOpenWin(MAIN_MAINMENU_WIN_ID))
    {
        win_handle = MMK_ConvertIdToHandle(MAIN_MAINMENU_WIN_ID);
        MMK_WinGrabFocus(MAIN_MAINMENU_WIN_ID);
    }
    else
    {
        MMIAPIMENU_CreatMainMenu();
    }
}


/*****************************************************************************/
//  Description :Unlock phone by left shortcut
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UlockPhoneByLeftShortcut(void *para1, void *para2)
{
    MMI_TEXT_ID_T shortcut_name_id = GetShortcutAppNameID(MMISET_FUNCKEY_LEFT);
    
    if (MMK_IsOpenWin(s_active_app_win_id))
    {
        MMI_HANDLE_T  win_handle = MMK_ConvertIdToHandle( s_active_app_win_id );

        //s_active_app_win_id = 0;
    }  
    
    MMIAPISET_OpenFuncKeyWin(MAIN_IDLE_WIN_ID, MMISET_FUNCKEY_LEFT);

    if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(shortcut_name_id))
    {
        GUIWIN_UpdateStb();
    }
}

/*****************************************************************************/
//  Description :Unlock phone by right shortcut
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UlockPhoneByRightShortcut(void *para1, void *para2)
{
    MMI_TEXT_ID_T shortcut_name_id = GetShortcutAppNameID(MMISET_FUNCKEY_RIGHT);

    if (MMK_IsOpenWin(s_active_app_win_id))
    {
        MMI_HANDLE_T   win_handle = MMK_ConvertIdToHandle( s_active_app_win_id );

        //s_active_app_win_id = 0;
    }  
    MMIAPISET_OpenFuncKeyWin(MAIN_IDLE_WIN_ID, MMISET_FUNCKEY_RIGHT);

    if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(shortcut_name_id))
    {
        GUIWIN_UpdateStb();
    }
}


/*****************************************************************************/
//  Description :Unlock phone to music player
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/

LOCAL void UlockPhoneToMusicPlayer(void *para1, void *para2)
{
    MMIAPIMAINWIN_EnterPlayer();
//    MMIAPIMP3_OpenMp3Player();
}

/*****************************************************************************/
//  Description :Unlock phone to FM
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UlockPhoneToFM(void *para1, void *para2)
{
    MMIAPIFM_OpenMainWin();
}

/*****************************************************************************/
//  Description :Unlock phone to Notify application
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UlockPhoneToNtfApp(void *para1, void *para2)
{
    IDLE_PUSH_MSG_NODE_T *idle_push_msg_ptr = (IDLE_PUSH_MSG_NODE_T *)para1;

    SCI_TraceLow("[SKY] UlockPhoneToNtfApp :: XXXX");

    if(PNULL != idle_push_msg_ptr 
#if 0
        && PNULL != idle_push_msg_ptr->idle_push_msg_info.app_entry_func_ptr)
#else
        && IDLE_APP_ENTRY_NULL != idle_push_msg_ptr->idle_push_msg_info.app_entry_e)
#endif
    {
        void *tmp_para1 = PNULL;
        void *tmp_para2 = PNULL;
        MMI_IDLE_PUSH_MSG_TYPE_E push_msg_t = idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type;

        if (MMI_IDLE_PUSH_MSG_WNS == push_msg_t
            || MMI_IDLE_PUSH_MSG_SNOOZED_ALARM == push_msg_t
            || MMI_IDLE_PUSH_MSG_SNOOZED_CALENDER == push_msg_t)
        {
            tmp_para1 = (void *)idle_push_msg_ptr->idle_push_msg_info.appname;
        }
        else if(
                    MMI_IDLE_PUSH_MSG_MISSED_CALL == push_msg_t
                    || MMI_IDLE_PUSH_MSG_ONGOING_CALL == push_msg_t
                    )
        {
            tmp_para1 = (void *)&idle_push_msg_ptr->idle_push_msg_info.cc_number;
            tmp_para2 = (void *)idle_push_msg_ptr->idle_push_msg_info.recv_num;
        }
        else if(MMI_IDLE_PUSH_MSG_SMS == push_msg_t)
        {
            tmp_para1 = (void *)&idle_push_msg_ptr->idle_push_msg_info.pb_bcd;            
        }
#if 0
        idle_push_msg_ptr->idle_push_msg_info.app_entry_func_ptr(tmp_para1, tmp_para2);
#else
        MMIIDLE_ExecutePushMsgEntryFunc(idle_push_msg_ptr->idle_push_msg_info.app_entry_e, tmp_para1, tmp_para2);
#endif
            if(MMI_IDLE_PUSH_MSG_SMS != push_msg_t)
            {
                MMIIDLE_RemovePushMsg(push_msg_t,
                      idle_push_msg_ptr->idle_push_msg_info.appname,
                      &idle_push_msg_ptr->idle_push_msg_info.cc_number);
            }
            else
            {
                MMIIDLE_RemovePushMsgEx(push_msg_t,
                      idle_push_msg_ptr->idle_push_msg_info.appname,
                      &idle_push_msg_ptr->idle_push_msg_info.pb_bcd);
            }
        }
    
}

/*****************************************************************************/
//  Description :Unlock phone to FM
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UlockPhoneToNtfRightAction(void *para1, void *para2)
{
    IDLE_PUSH_MSG_NODE_T *idle_push_msg_ptr = (IDLE_PUSH_MSG_NODE_T *)para1;

    if(PNULL != idle_push_msg_ptr 
#if 0
        && PNULL != idle_push_msg_ptr->idle_push_msg_info.app_right_focus_action_func_ptr)
#else
        && IDLE_APP_ENTRY_NULL != idle_push_msg_ptr->idle_push_msg_info.app_right_focus_action_e)
#endif
    {
        void *tmp_para1 = PNULL;
        void *tmp_para2 = PNULL;

        if(MMI_IDLE_PUSH_MSG_SMS == idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type)
        {
            tmp_para1 = (void *)&idle_push_msg_ptr->idle_push_msg_info.pb_bcd;
        }
        else if(MMI_IDLE_PUSH_MSG_MISSED_CALL == idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type
            || MMI_IDLE_PUSH_MSG_ONGOING_CALL == idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type)
        {
            tmp_para1 = (void *)&idle_push_msg_ptr->idle_push_msg_info.cc_number;
            tmp_para2 = (void *)idle_push_msg_ptr->idle_push_msg_info.recv_num;
        }
#if 0
        idle_push_msg_ptr->idle_push_msg_info.app_right_focus_action_func_ptr(tmp_para1, tmp_para2);
#else
        MMIIDLE_ExecutePushMsgEntryFunc(idle_push_msg_ptr->idle_push_msg_info.app_right_focus_action_e, tmp_para1, tmp_para2);
#endif
        if(MMI_IDLE_PUSH_MSG_SMS != idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type)
        {
            MMIIDLE_RemovePushMsg(idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type,
                  idle_push_msg_ptr->idle_push_msg_info.appname,
                  &idle_push_msg_ptr->idle_push_msg_info.cc_number);
        }
        else
        {
            MMIIDLE_RemovePushMsgEx(idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type,
                  idle_push_msg_ptr->idle_push_msg_info.appname,
                  &idle_push_msg_ptr->idle_push_msg_info.pb_bcd);
        }
    }

}


//[in idle,press the call button should enter call log]
LOCAL void UlockPhoneToCalllog(void *para1, void *para2)
{
    /*uint8           exist_sim_count = 0;
    MN_DUAL_SYS_E   dual_sys = MN_DUAL_SYS_1;

    for (dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
    {
        if (SIM_STATUS_NO_SIM != MMIAPIPHONE_GetSimStatus(dual_sys))
        {
            exist_sim_count++;
            break;
        }
    }

    if (0 == exist_sim_count)
    {
        MMIAPICC_MakeCall(MN_DUAL_SYS_MAX, (uint8 *)"112", 3, PNULL, PNULL, CC_CALL_SIM_MAX, CC_CALL_NORMAL_CALL, PNULL);
        return;
    }*/

    MMIAPICL_OpenCallsListWindow(MMICL_CALL_DIALED);
}


/*****************************************************************************/
//  Description :Unlock phone to Browser
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UlockPhoneToBrowser(void *para1, void *para2)
{
    MMIIDLE_StartBrowser();
}

/*****************************************************************************/
//  Description :Unlock phone to Browser
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UlockPhoneToSpeedDial(void *para1, void *para2)
{
    uint32 msg_id = (void *)para1;

    MMIAPIIDLE_HandleSpeedDialFunction((MMI_MESSAGE_ID_E)msg_id);
}

/*****************************************************************************/
//  Description :Unlock phone to Voice Mail Function
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UlockPhoneToVoiceMailFunction(void *para1, void *para2)
{
#ifndef FLYMODE_ECC_SUPPORT
    if (MMIAPISET_GetFlyMode())
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return ;
    }
#endif	
    MMIAPIIDLE_HandleVoiceMailFunction();
}

/******************************************************************************/
//  Description : get unlock timer id
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL uint8 GetUnlockTimerID(void)
{
    return s_idle_unlock_timer_id;
}

/*****************************************************************************/
//  Description : stop unlock confirm timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StopUnlockConfirmTimer(void)
{
    BOOLEAN     result  =   FALSE;

    if (0 < s_idle_unlock_timer_id)
    {
        MMK_StopTimer(s_idle_unlock_timer_id);
        s_idle_unlock_timer_id = 0;

        result = TRUE;
    }

    return (result);
}


/******************************************************************************/
//  Description : start unlock confirm timer
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartUnlockConfirmTimer(
                                   MMI_WIN_ID_T     win_id
                                   )
{   
    StopUnlockConfirmTimer();
    if (0 == s_idle_unlock_timer_id)
    {
        s_idle_unlock_timer_id = MMK_CreateWinTimer(win_id, IDLE_UNLOCK_CONFIRM_TIME, FALSE);
    }
}


/*****************************************************************************/
//  Description :Unlock phone by star
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_UlockPhoneByStar(IDLE_LOCK_DATA *idle_lock_data_ptr)
{
        MMI_STRING_T  text1_str ={0};
        MMI_TEXT_ID_T text_id = STXT_PRESS_MENU_STAR_EXT01;
        MMI_WIN_ID_T alert_win_id = MMI_DEVICELOCK_PROGRESS_WIN_ID;

        IDLE_LOCK_DATA *temp_ptr = PNULL;

        temp_ptr = SCI_ALLOC_APP(sizeof(IDLE_LOCK_DATA));
        if(PNULL != temp_ptr)
        {
            temp_ptr->e_phone_status = idle_lock_data_ptr->e_phone_status;
            temp_ptr->app_entry_func_ptr = idle_lock_data_ptr->app_entry_func_ptr;
            temp_ptr->para1 = idle_lock_data_ptr->para1;
            temp_ptr->para2 = idle_lock_data_ptr->para2;
        }
        
        MMIIDLE_SetLockStatus(IDLE_UNLOCKED_ONCE);
        StartUnlockConfirmTimer(MAIN_IDLE_WIN_ID);
        
        //提示按*解锁
        MMI_GetLabelTextByLang(text_id, &text1_str);
 
        //MMIPUB_OpenProgressWinByTextPtr(&text1_str, &alert_win_id, PNULL, MMIPUB_SOFTKEY_DIALOG_NONE, HandlePressStarToUnlockWinMsg);
		//MMIPUB_OpenAlertWinByTextPtrEx(MMK_GetFirstAppletHandle(),
        //    PNULL, 
        //    &text1_str, 
        //    PNULL, 
        //    IMAGE_NULL, 
        //    &alert_win_id, 
        //    PNULL, 
        //    MMIPUB_SOFTKEY_DIALOG_NONE, 
        //    HandlePressStarToUnlockWinMsg,
        //    temp_ptr);

        MMIPUB_OpenProgressTextOnlyWinEx(
            MMK_GetFirstAppletHandle(),
            &text1_str, 
            &alert_win_id, 
            PNULL, 
            MMIPUB_SOFTKEY_DIALOG_NONE, 
            HandlePressStarToUnlockWinMsg,
            temp_ptr);
 

        MMK_SetWinUserData(alert_win_id, (void *)temp_ptr);
}

/*****************************************************************************/
//  Description :Unlock phone by security code
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_UlockPhoneBySecurityCode(IDLE_LOCK_DATA *idle_lock_data_ptr)
{
    IDLE_LOCK_DATA *temp_ptr = PNULL;

    temp_ptr = SCI_ALLOC_APP(sizeof(IDLE_LOCK_DATA));
    if(PNULL != temp_ptr)
    {
        temp_ptr->e_phone_status = idle_lock_data_ptr->e_phone_status;
        temp_ptr->app_entry_func_ptr = idle_lock_data_ptr->app_entry_func_ptr;
        temp_ptr->para1 = idle_lock_data_ptr->para1;
        temp_ptr->para2 = idle_lock_data_ptr->para2;
    }

    //创建解锁窗口
    if(s_is_need_input_securitycode ||(MMI_IDLE_PHONE_START_UP ==idle_lock_data_ptr->e_phone_status))
    {
        if(MMK_IsOpenWin(MMIIDLE_SECURITY_CODE_WIN_ID))
        {
            MMK_CloseWin(MMIIDLE_SECURITY_CODE_WIN_ID);
        }
        MMK_CreateWin((uint32 *)MMIIDLE_INPUT_SECURITY_CODE_WINTAB, temp_ptr);
    }
    else         
    {
        idle_lock_data_ptr->app_entry_func_ptr(idle_lock_data_ptr->para1, idle_lock_data_ptr->para2);
        s_is_need_input_securitycode = TRUE;
    }
    SCI_TRACE_LOW("MMIIDLE_UlockPhoneBySecurityCode s_is_need_input_securitycode =%d",s_is_need_input_securitycode);
}


/*****************************************************************************/
//  Description :Unlock phone by security code
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_UlockPhoneBySecurityMode(IDLE_LOCK_DATA *idle_lock_data_ptr)
{
    if(MMIIDLE_IsAllowLockedOperation())
    {
        MMIIDLE_SetAllowLockedOperation(FALSE);
        if(!IsSecurityCodeEnabled())
        {
            if(PNULL != idle_lock_data_ptr && PNULL != idle_lock_data_ptr->app_entry_func_ptr)
            {
                idle_lock_data_ptr->app_entry_func_ptr(idle_lock_data_ptr->para1, idle_lock_data_ptr->para2);
            }
        }
        else
        {
            MMIIDLE_UlockPhoneBySecurityCode(idle_lock_data_ptr);
        }
        return;
    }

    if((MMIIDLE_IsUnlockedIn30s() || (IDLE_UNLOCKED == MMIIDLE_GetLockStatus())) && (FALSE == MMIDEFAULT_IsKeylockByStarKey()))
    {
        MMIAPIIDLE_UnlockPhone();
        if(PNULL != idle_lock_data_ptr && PNULL != idle_lock_data_ptr->app_entry_func_ptr)
        {
            idle_lock_data_ptr->app_entry_func_ptr(idle_lock_data_ptr->para1, idle_lock_data_ptr->para2);
        }
        return;
    }

    if(!IsSecurityCodeEnabled())
    {
	#ifdef UIX8910_ORG_IDLE_STYLE
        MMIIDLE_UlockPhoneByStar(idle_lock_data_ptr);
	#endif
    }
    else
    {
        MMIIDLE_UlockPhoneBySecurityCode(idle_lock_data_ptr);
    }
}

#ifdef UIX8910_ORG_IDLE_STYLE
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 获取shortcut图片
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetSoftkeyIcon(MMISET_FUNCKEY_E  index)
{
    MMI_IMAGE_ID_T shortcut_img_id = IMAGE_NULL;
    MMISET_SHORTCUT_TYPE_E shortcut = MMIAPISET_GetFuncKeyType(index);
   
    switch(shortcut)
    {
        case MMISET_SHORTCUT_PB:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_CONTACTS;
            break;

        case MMISET_SHORTCUT_CL:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_PHONE;
            break;

        case MMISET_SHORTCUT_MESSAGE:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_MESSAGING;
            break;

#ifdef MMI_AUDIO_PLAYER_SUPPORT
        case MMISET_SHORTCUT_MP3:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_MUSIC;
            break;
#endif

#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
        case MMISET_SHORTCUT_MEDIA_PLAY:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_VIDEOS;
            break;
#endif
#endif

#if defined(ZDT_PRODUCT_FM)
#ifdef FM_SUPPORT
        case MMISET_SHORTCUT_FM_RADIO:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_FM_RADIO;
            break;
#endif
#endif

#ifdef  CAMERA_SUPPORT
        case MMISET_SHORTCUT_CAMERA:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_CAMERA;
            break;
#endif    

        /*
        case MMISET_SHORTCUT_SETTINGS:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_SETTINGS;
            break;
       */     
        case MMISET_SHORTCUT_DOCUMENT:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_FILE_EXPLORER;
            break;
            
        case MMISET_SHORTCUT_ALARM:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_ALARM_CLOCK;
            break;

#ifdef PIC_VIEWER_SUPPORT
        case MMISET_SHORTCUT_PIC:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_PHOTOS;
            break;
#endif

        case MMISET_SHORTCUT_CALENDAR:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_CALENDAR;
            break;

        case MMISET_SHORTCUT_CALC:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_CALCULATOR;
            break;

#ifdef MMI_RECORD_SUPPORT
        case MMISET_SHORTCUT_RECORDER:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_RECORDER;
            break;
#endif

        /*            
        case MMISET_SHORTCUT_EXTRAS:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_EXTRAS;
            break;
            
        case MMISET_SHORTCUT_COUNTERS:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_COUNTERS;
            break;
        */

#ifdef BROWSER_SUPPORT
        case MMISET_SHORTCUT_WEB:
				shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_INTERNET;		
            break;
#endif
	//	case MMISET_SHORTCUT_SETTINGS:
      //      shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_SETTINGS;
      //      break;
		//case MMISET_SHORTCUT_EXTRAS:
           // shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_EXTRAS;
#if 0
def DATACOUNTER_SUPPORT
		case MMISET_SHORTCUT_COUNTERS:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_COUNTERS;
            break;
#endif
/*
#ifdef BROWSER_SUPPORT
		case MMISET_SHORTCUT_STORE:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_OPERA_STORE;
            break;
        case MMISET_SHORTCUT_FACEBOOK:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_FB;
            break;
		case MMISET_SHORTCUT_TWITTER:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_TWITTER;
			break;
		case MMISET_SHORTCUT_GAMELOFT:
            shortcut_img_id = IMAGE_GAMELOFT;
			break;
#endif
*/
        /*
        case MMISET_SHORTCUT_ONENOTE:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_ONENOTE;
            break;
            
        case MMISET_SHORTCUT_OUTLOOK:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_OUTLOOK;
            break;

        case MMISET_SHORTCUT_BING:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_BING;
            break;
        */
        /*
        case MMISET_SHORTCUT_GROUPME:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_GROUPME;
            break;
            
        case MMISET_SHORTCUT_FB:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_FB;
            break;
            
        case MMISET_SHORTCUT_MY_APPLICATION:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_MY_APPLICATION;
            break;
            
        case MMISET_SHORTCUT_OPERA_STORE:
            shortcut_img_id = IMAGE_IDLE_SCREEN_SHORTCUT_OPERA_STORE;
            break;
        */
        
        default:
            break;
    }

    return shortcut_img_id;
}
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 获取shortcut app name 更新status bar
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetShortcutAppNameID(MMISET_FUNCKEY_E  index)
{
    MMI_TEXT_ID_T shortcut_name_id = TXT_NULL;
    MMISET_SHORTCUT_TYPE_E shortcut = MMIAPISET_GetFuncKeyType(index);
   
    switch(shortcut)
    {
        case MMISET_SHORTCUT_PB:
            shortcut_name_id = STXT_HEADER_PHONEBOOK;
            break;

        case MMISET_SHORTCUT_CL:
            shortcut_name_id = STR_MENU_HDR_LOG_EXT02;
            break;

        case MMISET_SHORTCUT_MESSAGE:
            shortcut_name_id = STXT_HEADER_MESSAGES;
            break;

#ifdef MMI_AUDIO_PLAYER_SUPPORT
        case MMISET_SHORTCUT_MP3:
            shortcut_name_id = STR_HDR_MUSIC;
            break;
#endif

#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
        case MMISET_SHORTCUT_MEDIA_PLAY:
            shortcut_name_id = STR_MAINMENU_VIDEOS;
            break;
#endif
#endif

#if defined(ZDT_PRODUCT_FM)
#ifdef FM_SUPPORT
        case MMISET_SHORTCUT_FM_RADIO:
            shortcut_name_id = STR_RADIO_HDR_FMRADIO;
            break;
#endif
#endif

#ifdef  CAMERA_SUPPORT
        case MMISET_SHORTCUT_CAMERA:
            shortcut_name_id = STR_MENU_CAMERA;
            break;
#endif    

        /*
        case MMISET_SHORTCUT_SETTINGS:
            shortcut_name_id = STXT_HEADER_SETTINGS;
            break;
       */     
        case MMISET_SHORTCUT_DOCUMENT:
            shortcut_name_id = STR_MAINMENU_FILES;
            break;
            
        case MMISET_SHORTCUT_ALARM:
            shortcut_name_id = STXT_HEADER_ALARM_CLOCK;
            break;

#ifdef PIC_VIEWER_SUPPORT
        case MMISET_SHORTCUT_PIC:
            shortcut_name_id = STR_HDR_PHOTOS;
            break;
#endif

        case MMISET_SHORTCUT_CALENDAR:
            shortcut_name_id = STXT_CALENDAR;
            break;

        case MMISET_SHORTCUT_CALC:
            shortcut_name_id = STR_CALC_HDR_CALCULATOR_EXT02;
            break;

#ifdef MMI_RECORD_SUPPORT
        case MMISET_SHORTCUT_RECORDER:
            shortcut_name_id = STXT_HEADER_VOICE_REC;
            break;
#endif

        /*            
        case MMISET_SHORTCUT_EXTRAS:
            shortcut_name_id = STR_HDR_APPS_EXTRAS;
            break;
            
        case MMISET_SHORTCUT_COUNTERS:
            shortcut_name_id = STR_HDR_COUNTERS;
            break;
        */

#ifdef BROWSER_SUPPORT
        case MMISET_SHORTCUT_WEB:
            shortcut_name_id = STR_OPERA_BROWSER_EXT01;
            break;

        /*            
        case MMISET_SHORTCUT_BING:
            shortcut_name_id = STR_HDR_BING;
            break;
            
        case MMISET_SHORTCUT_TWITTER:
            shortcut_name_id = STR_HDR_TWITTER;
            break;
            
        case MMISET_SHORTCUT_FACEBOOK:
            shortcut_name_id = STR_HDR_FACEBOOK;
            break;
        */

        
#endif


        /*            
        case MMISET_SHORTCUT_ONENOTE:
            shortcut_name_id = TEXT_NULL;
            break;
            
        case MMISET_SHORTCUT_OUTLOOK:
            shortcut_name_id = TXT_NULL;
            break;
            
        case MMISET_SHORTCUT_GROUPME:
            shortcut_name_id = TXT_NULL;
            break;
            
            
        case MMISET_SHORTCUT_MY_APPLICATION:
            shortcut_name_id = STXT_HEADER_MYAPP;
            break;
            
        case MMISET_SHORTCUT_OPERA_STORE:
            shortcut_name_id = STR_OPERA_STORE_EXT01;
            break;
        */
        
        default:
            break;
    }

    return shortcut_name_id;
}


#ifdef UIX8910_ORG_IDLE_STYLE
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 只能画shortcut icon
/*****************************************************************************/
LOCAL void DisplaySoftkeyShortcut(void)
{
    
    MMI_HANDLE_T idle_win_handle = MMK_ConvertIdToHandle(MAIN_IDLE_WIN_ID);
    MMI_IMAGE_ID_T          idle_softkey_icon[2] = {0};

  
    idle_softkey_icon[0] = GetSoftkeyIcon(MMISET_FUNCKEY_LEFT);
    idle_softkey_icon[1] = GetSoftkeyIcon(MMISET_FUNCKEY_RIGHT);

   
    GUIWIN_SeSoftkeytButtonIconId(idle_win_handle, idle_softkey_icon[0], 0, TRUE);
    GUIWIN_SeSoftkeytButtonIconId(idle_win_handle, idle_softkey_icon[1], 2, TRUE);

}
#endif
 


/*****************************************************************************/
//  Description :Is notification on focus
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNotificationFocus(void)
{
    MMI_HANDLE_T idle_win_handle = MMK_ConvertIdToHandle(MAIN_IDLE_WIN_ID);
    MMI_CTRL_ID_T  ctrl_id = MMK_GetActiveCtrlId(idle_win_handle);

    SCI_TRACE_LOW("[grant]:IsNotificationFocus active ctrl_id(%d)", ctrl_id);

     if(MMIIDLE_PUSH_MSG_LIST_CTRL_ID !=  ctrl_id 
        && MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID  != ctrl_id
        && MMIIDLE_MUSIC_FORM_CTRL_ID  != ctrl_id
        && MMIIDLE_FM_PALY_ANIM_CTRL_ID  != ctrl_id
        && MMIIDLE_FM_FORM_CTRL_ID != ctrl_id)
    {
        return FALSE;
    }

     return TRUE;
}

LOCAL void ResetDisplayNum(void);
LOCAL uint16 GetDisplayNum(void);
LOCAL void SetDisplayNum(void);
LOCAL uint8 s_display_num = 0;

/*****************************************************************************/
//  Description : cal idle string start point
//  Global resource dependence :
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL GUI_POINT_T CalStartPoint(MMI_IDLE_DISPLAY_T disp_style, uint16 str_width)
{
    GUI_POINT_T     point   =   {0};
    GUI_RECT_T      mainlcd_rect = MMITHEME_GetFullScreenRect();


    switch(disp_style.text_align)
    {
    case IDLE_ALIGN_LEFT:
        point.x = disp_style.text_frame_x;
        point.y = disp_style.text_frame_top;
        break;

    case IDLE_ALIGN_RIGHT:
        point.x = disp_style.text_frame_x - str_width;
        point.y = disp_style.text_frame_top;
        break;

    case IDLE_ALIGN_X_CENTER:
        point.x = (mainlcd_rect.right - str_width) / 2;
        point.y = disp_style.text_frame_top;
        break;

    case IDLE_ALIGN_CUSTOM:
        point.x = disp_style.text_frame_x;
        point.y = disp_style.text_frame_top;
        break;

    default:
        break;
    }

    return point;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_NotificationFillBottomRect(void)
{
    GUI_LCD_DEV_INFO        lcd_dev_info             = *GetCommonIdlePaintLcdDev();//*GetCommonIdleNotiLcdDev();
    LCD_FillArgbRect(&lcd_dev_info, s_bottom_rect, MMITHEME_GetTransparenceColor(), (uint8)MMI_IDLE_NTF_LIST_BG_ALPHA);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_NotificationUpdateSoftkey(void)
{
    MMI_HANDLE_T idle_win_handle = MMK_ConvertIdToHandle(MAIN_IDLE_WIN_ID);
    MMI_CTRL_ID_T  ctrl_id = MMK_GetActiveCtrlId(idle_win_handle);
    MMI_SOFTKEY_STYLE_T      softkey_style = {0};
    GUIFORM_CHILD_DISPLAY_E   child_list_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_music_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_fm_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_PUSH_MSG_LIST_CTRL_ID, &child_list_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_FM_FORM_CTRL_ID, &child_fm_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_MUSIC_FORM_CTRL_ID, &child_music_display_type);


    MMITHEME_GetSoftkeyStyle(&softkey_style);
    GUIWIN_SetSoftkeyStyle(idle_win_handle,
                           softkey_style.font_color,
                           softkey_style.bg_type,
                           softkey_style.bg_color,
                           IMAGE_NULL);


    if(GUIFORM_CHILD_DISP_HIDE == child_list_display_type
        && GUIFORM_CHILD_DISP_HIDE == child_music_display_type
        && GUIFORM_CHILD_DISP_HIDE == child_fm_display_type
      )
    {
         MMK_WinInactiveCtrl(MAIN_IDLE_WIN_ID, FALSE);   

        if((MMIIDLE_IsUnlockedIn30s() || (IDLE_UNLOCKED == MMIIDLE_GetLockStatus())) && (FALSE == MMIDEFAULT_IsKeylockByStarKey()))
        {
		#ifdef UIX8910_ORG_IDLE_STYLE
		GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, STXT_SOFTKEY_MENU_MK, MIDDLE_BUTTON, TRUE);
		//显示shortcut
		DisplaySoftkeyShortcut();		
		#else
		GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, STXT_SOFTKEY_MENU_MK, LEFT_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, STXT_SOFTKEY_PB_MK, RIGHT_BUTTON, TRUE);	
		#endif
        }
        else
        {
		#ifdef UIX8910_ORG_IDLE_STYLE
		GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, STXT_SOFTKEY_UNLOCK_MK, MIDDLE_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, TXT_NULL, LEFT_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, TXT_NULL, RIGHT_BUTTON, TRUE);
		#endif
        }           
    }
    else if(!IsNotificationFocus())
    {
        if((MMIIDLE_IsUnlockedIn30s() || (IDLE_UNLOCKED == MMIIDLE_GetLockStatus())) && (FALSE == MMIDEFAULT_IsKeylockByStarKey()))
        {
		#ifdef UIX8910_ORG_IDLE_STYLE		
		GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, STXT_SOFTKEY_MENU_MK, MIDDLE_BUTTON, TRUE);
		DisplaySoftkeyShortcut();
		#else
		GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, STXT_SOFTKEY_MENU_MK, LEFT_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, STXT_SOFTKEY_PB_MK, RIGHT_BUTTON, TRUE);
		#endif
        }
        else
        {
		#ifdef UIX8910_ORG_IDLE_STYLE	
		GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, STXT_SOFTKEY_UNLOCK_MK, MIDDLE_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, TXT_NULL, LEFT_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, TXT_NULL, RIGHT_BUTTON, TRUE);
		#endif
        }        
    }
    else if((GUIFORM_CHILD_DISP_HIDE != child_music_display_type && MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID == ctrl_id)
            ||(GUIFORM_CHILD_DISP_HIDE != child_music_display_type && MMIIDLE_MUSIC_FORM_CTRL_ID == ctrl_id)
            || (GUIFORM_CHILD_DISP_HIDE != child_fm_display_type && MMIIDLE_FM_PALY_ANIM_CTRL_ID == ctrl_id)
            ||(GUIFORM_CHILD_DISP_HIDE != child_fm_display_type && MMIIDLE_FM_FORM_CTRL_ID == ctrl_id)
           )
    {
		GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, TXT_NULL, LEFT_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, TXT_NULL, RIGHT_BUTTON, TRUE);    
    }
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_NotificationRectCalculate(void)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint16          client_height = 0;
    uint16          form_height = 0;
    uint16          form_relative_height = 0;
    GUI_BOTH_RECT_T  parent_both_rect  = {0};
    GUIFORM_CHILD_HEIGHT_T      child_form_height = {0, GUIFORM_CHILD_HEIGHT_AUTO};
    GUIFORM_CHILD_HEIGHT_T list_form_height = {0, GUIFORM_CHILD_HEIGHT_FIXED};
    GUIFORM_CHILD_HEIGHT_T button_form_height = {FORM_CHILD_BUTTON_HEIGHT, GUIFORM_CHILD_HEIGHT_FIXED};
    GUIFORM_CHILD_HEIGHT_T fm_form_height = {FORM_CHILD_FM_HEIGHT, GUIFORM_CHILD_HEIGHT_FIXED};
    GUIFORM_CHILD_HEIGHT_T music_form_height = {FORM_CHILD_MUSIC_HEIGHT, GUIFORM_CHILD_HEIGHT_FIXED};
    GUIFORM_CHILD_DISPLAY_E   child_list_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_music_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_fm_display_type = GUIFORM_CHILD_DISP_MAX;

    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_PUSH_MSG_LIST_CTRL_ID, &child_list_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_FM_FORM_CTRL_ID, &child_fm_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_MUSIC_FORM_CTRL_ID, &child_music_display_type);

    //list height
    if(GUIFORM_CHILD_DISP_HIDE != child_list_display_type)
    {
        THEMELIST_ITEM_STYLE_T *my_style = THEMELIST_GetStyle(GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_MS);
        uint16 list_item_num = GUILIST_GetTotalItemNum(MMIIDLE_PUSH_MSG_LIST_CTRL_ID);
        list_form_height.add_data = list_item_num * my_style->height;
        form_relative_height += list_form_height.add_data;
    }
    //child fm from height
    if(GUIFORM_CHILD_DISP_HIDE != child_fm_display_type)
    {
        form_relative_height += fm_form_height.add_data;
    }
    //child music from height
    if(GUIFORM_CHILD_DISP_HIDE != child_music_display_type)
    {
        form_relative_height += music_form_height.add_data;
    }


    //child fm from rect
    if(GUIFORM_CHILD_DISP_HIDE != child_fm_display_type)
    {
        uint16 ver_space = 6;
        uint16 hor_space = 0;
        GUIFORM_CHILD_HEIGHT_T fm_navi_lable_form_height = {FORM_CHILD_FM_FRQ_HEIGHT, GUIFORM_CHILD_HEIGHT_FIXED};
        GUIFORM_CHILD_WIDTH_T fm_navi_form_width = {FORM_CHILD_FM_NAVI_WIDTH, GUIFORM_CHILD_WIDTH_FIXED};

        GUIFORM_SetChildHeight(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_FM_FORM_CTRL_ID, &fm_form_height);
        GUIFORM_SetSpace(MMIIDLE_FM_FORM_CTRL_ID, &hor_space, &ver_space);
        GUIFORM_SetMargin(MMIIDLE_FM_FORM_CTRL_ID, 6);

        GUIFORM_SetChildHeight(MMIIDLE_FM_FORM_CTRL_ID, MMIIDLE_FM_FREQ_LABLE_CTRL_ID, &fm_navi_lable_form_height);
        GUIFORM_SetChildWidth(MMIIDLE_FM_FORM_CTRL_ID, MMIIDLE_FM_CHIL_NAVI_FORM_CTRL_ID, &fm_navi_form_width);
    }
    
    //child music from rect
    if(GUIFORM_CHILD_DISP_HIDE != child_music_display_type)
    {
        GUIFORM_CHILD_HEIGHT_T music_lable_form_height = {FORM_CHILD_MUSIC_NAME_HEIGHT, GUIFORM_CHILD_HEIGHT_FIXED};
        GUIFORM_CHILD_HEIGHT_T music_progress_height = {FORM_CHILD_MUSIC_PROGRESS_HEIGHT, GUIFORM_CHILD_HEIGHT_FIXED};
        GUIFORM_CHILD_HEIGHT_T music_navi_form_height = {FORM_CHILD_MUSIC_NAVI_HEIGHT, GUIFORM_CHILD_HEIGHT_FIXED};
        GUIFORM_CHILD_WIDTH_T music_navi_form_width = {FORM_CHILD_MUSIC_NAVI_WIDTH, GUIFORM_CHILD_WIDTH_FIXED};

        GUIFORM_SetChildHeight(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_MUSIC_FORM_CTRL_ID, &music_form_height);
        GUIFORM_SetMargin(MMIIDLE_MUSIC_FORM_CTRL_ID, 6);

        GUIFORM_SetChildHeight(MMIIDLE_MUSIC_FORM_CTRL_ID, MMIIDLE_MUSIC_NAME_LABLE_CTRL_ID, &music_lable_form_height);
        GUIFORM_SetChildHeight(MMIIDLE_MUSIC_FORM_CTRL_ID, MMIIDLE_MUSIC_ARTIST_LABLE_CTRL_ID, &music_lable_form_height);
        GUIFORM_SetChildHeight(MMIIDLE_MUSIC_FORM_CTRL_ID, MMIIDLE_MUSIC_PROGRESSBAR_CTRL_ID, &music_progress_height);
        GUIFORM_SetChildHeight(MMIIDLE_MUSIC_FORM_CTRL_ID, MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID, &music_navi_form_height);
        GUIFORM_SetChildWidth(MMIIDLE_MUSIC_FORM_CTRL_ID, MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID, &music_navi_form_width);


    }


    if (GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height))
    {
        //client_height = (uint16)(lcd_height - MMI_STATUSBAR_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT);
    }

    client_height = MMI_IDLE_NTF_LIST_FORM_HEIGHT_MAX;
    
    if(client_height <= form_relative_height)
    {
        s_notification_form_auto_size = TRUE;
    }
    else
    {
        s_notification_form_auto_size = FALSE;
    }


    if(s_notification_form_auto_size)
    {
        uint16 bottom_margin = FORM_BOTTOM_MAGRIN_HEIGHT;
        parent_both_rect = MMITHEME_GetWinClientBothRect(MAIN_IDLE_WIN_ID);

        parent_both_rect.v_rect.top = parent_both_rect.v_rect.bottom - MMI_IDLE_NTF_LIST_FORM_HEIGHT_MAX + 1;
        parent_both_rect.h_rect.top = parent_both_rect.h_rect.bottom - MMI_IDLE_NTF_LIST_FORM_HEIGHT_MAX + 1;
        parent_both_rect.v_rect.bottom -= bottom_margin;
        parent_both_rect.h_rect.bottom -= bottom_margin;
        GUIAPICTRL_SetBothRect(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, &parent_both_rect);

        //list rect
        if(GUIFORM_CHILD_DISP_HIDE != child_list_display_type)
        {
            GUIFORM_SetChildHeight(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_PUSH_MSG_LIST_CTRL_ID, &child_form_height);
        }
    }
    else
    {
        GUI_BOTH_RECT_T  both_rect  = {0};

        //list rect
        if(GUIFORM_CHILD_DISP_HIDE != child_list_display_type)
        {
            GUIFORM_SetChildHeight(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_PUSH_MSG_LIST_CTRL_ID, &list_form_height);

            GUIAPICTRL_GetBothRect(MMIIDLE_PUSH_MSG_LIST_CTRL_ID, &both_rect);
            both_rect.v_rect.bottom -= fm_form_height.add_data + music_form_height.add_data;
            both_rect.h_rect.bottom -= fm_form_height.add_data + music_form_height.add_data;
            GUIAPICTRL_SetBothRect(MMIIDLE_PUSH_MSG_LIST_CTRL_ID, &both_rect);
        }
        //parent form rect
        if(GUIFORM_CHILD_DISP_HIDE != child_music_display_type
                || GUIFORM_CHILD_DISP_HIDE != child_fm_display_type
                || GUIFORM_CHILD_DISP_HIDE != child_list_display_type)
        {
            GUIAPICTRL_GetBothRect(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, &parent_both_rect);
            parent_both_rect.v_rect.top = parent_both_rect.v_rect.bottom - form_relative_height;
            parent_both_rect.h_rect.top = parent_both_rect.h_rect.bottom - form_relative_height;
            GUIAPICTRL_SetBothRect(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, &parent_both_rect);
        }
    }
    
    if(0 < form_relative_height)
    {
        s_bottom_rect.top = parent_both_rect.v_rect.bottom;
        s_bottom_rect.bottom = lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
        s_bottom_rect.left = parent_both_rect.v_rect.left;
        s_bottom_rect.right  = parent_both_rect.v_rect.right;

        CTRLFORM_SetVisible(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, TRUE, FALSE);
        GUIFORM_ForceLayout(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID);
    }
    else
    {
        s_bottom_rect.top = 0;
        s_bottom_rect.bottom = 0;
        s_bottom_rect.left = 0;
        s_bottom_rect.right  = 0;

        CTRLFORM_SetVisible(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, FALSE, FALSE);
    }

    if(!IsNotificationFocus())
    {
        MMK_WinInactiveCtrl(MAIN_IDLE_WIN_ID, FALSE);   
    }

}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_UpdateFmMusicNotification(void)
{

    MMK_WinInactiveCtrl(MAIN_IDLE_WIN_ID, FALSE);   

    GUIFORM_SetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_FM_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    GUIFORM_SetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_MUSIC_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);

    if(FM_PLAYING == MMIAPIFM_GetStatus())
    {
        GUIFORM_SetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_FM_FORM_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
        UpdateIdleFmRadioNotification();
        MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_FM_PALY_ANIM_CTRL_ID);
    }    
    else if(MMIAPIMP3_IsPlayerActive()
           &&MMIAPIMP3_IsPlayerPlaying())
    {
        GUIFORM_SetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_MUSIC_FORM_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
        UpdateIdleMusicNotification();
        MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID);
    }

    //MMIIDLE_NotificationRectCalculate();
}


/*****************************************************************************/
//  Description :Idle screen更新List item的图标数据
//  Global resource dependence :
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void UpdateNotificationListItemIconData(GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr)
{
    //add anim into list item
    if(PNULL != need_item_content_ptr && MMIIDLE_PUSH_MSG_LIST_CTRL_ID == need_item_content_ptr->ctrl_id)
    {
        uint32 user_data = 0;
        GUIITEM_CONTENT_T  item_content = {0};      /*lint !e64*/
        IDLE_PUSH_MSG_NODE_T *idle_push_msg_ptr = PNULL;

        GUILIST_GetItemData(need_item_content_ptr->ctrl_id, need_item_content_ptr->item_index, &user_data);
        idle_push_msg_ptr = (IDLE_PUSH_MSG_NODE_T *)user_data;


        if(PNULL != idle_push_msg_ptr)
        {
            if(MMI_IDLE_ICON_DATA_ANIM_PATH == idle_push_msg_ptr->idle_push_msg_info.icon_data_type)
            {
                GUIANIM_FILE_INFO_T   anim_path = {0};

                anim_path.full_path_wstr_len = MMIAPICOM_Wstrlen(idle_push_msg_ptr->idle_push_msg_info.icon_data.full_path_wstr_ptr);
                anim_path.full_path_wstr_ptr = idle_push_msg_ptr->idle_push_msg_info.icon_data.full_path_wstr_ptr;

                item_content.item_data_type = GUIITEM_DATA_ANIM_PATH;
                item_content.item_data.anim_path_ptr = &anim_path;
                GUILIST_SetItemContent(need_item_content_ptr->ctrl_id,
                                       &item_content,
                                       need_item_content_ptr->item_index,
                                       need_item_content_ptr->item_content_index);
            }
        }
    }
}


/*****************************************************************************/
//  Description :处理Idle screen 控件对OK键的响应
//  Global resource dependence :
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void HandleIdleScreenCtrlMidSK( MMI_WIN_ID_T win_id)
{
    uint16 selected_index = 0;
    GUIFORM_CHILD_DISPLAY_E   child_music_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_fm_display_type = GUIFORM_CHILD_DISP_MAX;
    MMI_CTRL_ID_T  ctrl_id = MMK_GetActiveCtrlId(win_id);

    if(MMIIDLE_PUSH_MSG_LIST_CTRL_ID == ctrl_id)
    {
        GUILIST_ITEM_T  *item_ptr  = PNULL;
        IDLE_PUSH_MSG_NODE_T *idle_push_msg_ptr = PNULL;
        uint32 user_data = 0;

        selected_index = GUILIST_GetCurItemIndex(MMIIDLE_PUSH_MSG_LIST_CTRL_ID);
        item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, selected_index);
        GUILIST_GetItemData(ctrl_id, selected_index, &user_data);
        idle_push_msg_ptr = (IDLE_PUSH_MSG_NODE_T *)user_data;

        SCI_TraceLow("[SKY] HandleIdleScreenCtrlMidSK ::");
        if(PNULL != item_ptr && PNULL != idle_push_msg_ptr)
        {
            if (GUILIST_GetItemState(item_ptr, GUIITEM_STATE_LEFT_FOCUS))
            {
                //当前item处于左键focus状态
                SCI_TraceLow("[SKY] HandleIdleScreenCtrlMidSK :: GUIITEM_STATE_LEFT_FOCUS");
#if 0
                if(PNULL != idle_push_msg_ptr->idle_push_msg_info.app_left_focus_action_func_ptr)
#else
                if(IDLE_APP_ENTRY_NULL != idle_push_msg_ptr->idle_push_msg_info.app_left_focus_action_e)
#endif
                {
                    IDLE_LOCK_DATA idle_lock_data = {0};

                    void *para1 = PNULL;
                    void *para2 = PNULL;

                    if(MMI_IDLE_PUSH_MSG_WNS == idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type)
                    {
                        para1 = (void *)idle_push_msg_ptr->idle_push_msg_info.appname;
                    }
                    else if(MMI_IDLE_PUSH_MSG_SMS == idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type)
                    {
                        para1 = (void *)&idle_push_msg_ptr->idle_push_msg_info.pb_bcd;
                    }
                    else if(MMI_IDLE_PUSH_MSG_MISSED_CALL == idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type
                            || MMI_IDLE_PUSH_MSG_ONGOING_CALL == idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type
                           )
                    {
                        para1 = (void *)&idle_push_msg_ptr->idle_push_msg_info.cc_number;
                    }

                    idle_lock_data.e_phone_status = MMI_IDLE_PHONE_NORMAL;
#if 0
                    idle_lock_data.app_entry_func_ptr =  idle_push_msg_ptr->idle_push_msg_info.app_left_focus_action_func_ptr;
#else
                    idle_lock_data.app_entry_func_ptr = MMIIDLE_GetPushMsgEntryFunc(idle_push_msg_ptr->idle_push_msg_info.app_left_focus_action_e);
#endif
                    idle_lock_data.para1 = para1;
                    idle_lock_data.para2 = para2;

                    MMIIDLE_SetAllowLockedOperation(TRUE);

                    MMIIDLE_UlockPhoneBySecurityMode(&idle_lock_data);
                }
                else
                {
                    if(MMI_IDLE_PUSH_MSG_SMS != idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type)
                    {
                        MMIIDLE_RemovePushMsg(idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type,
                                              idle_push_msg_ptr->idle_push_msg_info.appname,
                                              &idle_push_msg_ptr->idle_push_msg_info.cc_number);
                    }
                    else
                    {
                        MMIIDLE_RemovePushMsgEx(idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type,
                                  idle_push_msg_ptr->idle_push_msg_info.appname,
                                  &idle_push_msg_ptr->idle_push_msg_info.pb_bcd);                        
                    }
                }
            }
            else if (GUILIST_GetItemState(item_ptr, GUIITEM_STATE_RIGHT_FOCUS))
            {
                //当前item处理右键focus状态
                SCI_TraceLow("[SKY] HandleIdleScreenCtrlMidSK :: GUIITEM_STATE_RIGHT_FOCUS");

#if 0
                if(PNULL != idle_push_msg_ptr->idle_push_msg_info.app_right_focus_action_func_ptr)
#else
                if(IDLE_APP_ENTRY_NULL != idle_push_msg_ptr->idle_push_msg_info.app_right_focus_action_e)
#endif
                {
                    IDLE_LOCK_DATA idle_lock_data = {0};
                    void *para1 = PNULL;
                    void *para2 = PNULL;

                    idle_lock_data.e_phone_status = MMI_IDLE_PHONE_NORMAL;
                    idle_lock_data.app_entry_func_ptr =  UlockPhoneToNtfRightAction;
                    idle_lock_data.para1 = idle_push_msg_ptr;
                    idle_lock_data.para2 = para2;

                    MMIIDLE_SetAllowLockedOperation(TRUE);

                    MMIIDLE_UlockPhoneBySecurityMode(&idle_lock_data);
                }
            }
            else
            {
                SCI_TraceLow("[SKY] HandleIdleScreenCtrlMidSK :: XXXX");
#if 0
                if(PNULL != idle_push_msg_ptr->idle_push_msg_info.app_entry_func_ptr)
#else
                if(IDLE_APP_ENTRY_NULL != idle_push_msg_ptr->idle_push_msg_info.app_entry_e)
#endif
                {
                    IDLE_LOCK_DATA idle_lock_data = {0};
                    void *para1 = PNULL;
                    void *para2 = PNULL;

                    idle_lock_data.e_phone_status = MMI_IDLE_PHONE_NORMAL;
                    idle_lock_data.app_entry_func_ptr =  UlockPhoneToNtfApp;
                    idle_lock_data.para1 = idle_push_msg_ptr;
                    idle_lock_data.para2 = para2;

                    MMIIDLE_SetAllowLockedOperation(TRUE);

                    MMIIDLE_UlockPhoneBySecurityMode(&idle_lock_data);
                }
            }
        }
    }
    else if((GUIFORM_CHILD_DISP_HIDE != child_music_display_type && MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID == ctrl_id)
                ||(GUIFORM_CHILD_DISP_HIDE != child_music_display_type && MMIIDLE_MUSIC_FORM_CTRL_ID == ctrl_id)
                )
    {
#ifdef MMI_AUDIO_PLAYER_SUPPORT
        if(MMIAPIMP3_IsPlayerActive()
           &&MMIAPIMP3_IsPlayerPlaying())
        {
            IDLE_LOCK_DATA idle_lock_data = {0};
            idle_lock_data.e_phone_status = MMI_IDLE_PHONE_NORMAL;
            idle_lock_data.app_entry_func_ptr =  UlockPhoneToMusicPlayer;
            idle_lock_data.para1 = PNULL;
            idle_lock_data.para2 = PNULL;

            MMIIDLE_SetAllowLockedOperation(TRUE);

            MMIIDLE_UlockPhoneBySecurityMode(&idle_lock_data);
        }
#endif
    }
    else if((GUIFORM_CHILD_DISP_HIDE != child_fm_display_type && MMIIDLE_FM_PALY_ANIM_CTRL_ID == ctrl_id)
                ||(GUIFORM_CHILD_DISP_HIDE != child_fm_display_type && MMIIDLE_FM_FORM_CTRL_ID == ctrl_id)
                )
    {
#ifdef FM_SUPPORT
        if(FM_PLAYING == MMIAPIFM_GetStatus())
        {
            IDLE_LOCK_DATA idle_lock_data = {0};
            idle_lock_data.e_phone_status = MMI_IDLE_PHONE_NORMAL;
            idle_lock_data.app_entry_func_ptr =  UlockPhoneToFM;
            idle_lock_data.para1 = PNULL;
            idle_lock_data.para2 = PNULL;

            MMIIDLE_SetAllowLockedOperation(TRUE);

            MMIIDLE_UlockPhoneBySecurityMode(&idle_lock_data);
        }
#endif
    }

}

/*****************************************************************************/
//  Description :处理Idle screen 控件对navi-left键的响应
//  Global resource dependence :
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void HandleIdleScreenCtrlNaviLeft( MMI_WIN_ID_T win_id)
{
    GUIFORM_CHILD_DISPLAY_E   child_music_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_fm_display_type = GUIFORM_CHILD_DISP_MAX;
    MMI_CTRL_ID_T  ctrl_id = MMK_GetActiveCtrlId(win_id);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_MUSIC_FORM_CTRL_ID, &child_music_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_FM_FORM_CTRL_ID, &child_fm_display_type);

    if((GUIFORM_CHILD_DISP_HIDE != child_music_display_type && MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID == ctrl_id)
        || (GUIFORM_CHILD_DISP_HIDE != child_music_display_type && MMIIDLE_MUSIC_FORM_CTRL_ID == ctrl_id)
        )
    {
#ifdef MMI_AUDIO_PLAYER_SUPPORT
        if(MMIMP3_AUDIO_PLAYING == MMIAPIMP3_GetAudioState())
        {
            MMIAPIMP3_PauseMp3(FALSE);
            //MMK_WinInactiveCtrl(MAIN_IDLE_WIN_ID, FALSE);   
            //GUIFORM_SetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_MUSIC_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        }
        else if(MMIMP3_AUDIO_PAUSED == MMIAPIMP3_GetAudioState())
        {
            MMIAPIMP3_ResumeMp3(FALSE);
            UpdateIdleMusicNotification();
            MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID);
        }
        else if(MMIMP3_PLAYER_STATE_NONE == MMIAPIMP3_GetPlayerState())
        {
            uint32 offset = MMIMP3_ZERO_OFFSET;

            offset = MMIAPIAP_GetCurPlayOffset(offset);

            MMIAPIMP3_PlayAudioPlayer(offset);

            UpdateIdleMusicNotification();
            MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID);
        }

        if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
        {
            MMK_PostMsg(MAIN_IDLE_WIN_ID, MSG_IDLE_UPDATE_MP3, PNULL, 0);
        }
#endif
    }
    else if((GUIFORM_CHILD_DISP_HIDE != child_fm_display_type && MMIIDLE_FM_PALY_ANIM_CTRL_ID == ctrl_id)
                ||(GUIFORM_CHILD_DISP_HIDE != child_fm_display_type && MMIIDLE_FM_FORM_CTRL_ID == ctrl_id)
                )
    {
#ifdef FM_SUPPORT
        if(FM_PLAYING == MMIAPIFM_GetStatus())
        {
            //MMIAPIFM_Pause(TRUE);
            MMIAPIFM_Exit();
        }
        else if(FM_SUSPENDED == MMIAPIFM_GetStatus())
        {
            MMIAPIFM_Resume(TRUE);
            UpdateIdleFmRadioNotification();
            MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID);
        }

        if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
        {
            MMK_PostMsg(MAIN_IDLE_WIN_ID, MSG_IDLE_UPDATE_MP3, PNULL, 0);
        }
#endif
    }
}

/*****************************************************************************/
//  Description :处理Idle screen 控件对navi-right键的响应
//  Global resource dependence :
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void HandleIdleScreenCtrlNaviRight( MMI_WIN_ID_T win_id)
{
    GUIFORM_CHILD_DISPLAY_E   child_music_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_fm_display_type = GUIFORM_CHILD_DISP_MAX;
    MMI_CTRL_ID_T  ctrl_id = MMK_GetActiveCtrlId(win_id);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_MUSIC_FORM_CTRL_ID, &child_music_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_FM_FORM_CTRL_ID, &child_fm_display_type);

    if((GUIFORM_CHILD_DISP_HIDE != child_music_display_type && MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID == ctrl_id)
        ||(GUIFORM_CHILD_DISP_HIDE != child_music_display_type && MMIIDLE_MUSIC_FORM_CTRL_ID == ctrl_id)
        )
    {
#ifdef MMI_AUDIO_PLAYER_SUPPORT

#ifdef UNISOC_VERSION
        if(MMIMP3_GetNextAvailable())
#endif
        {
            MMIAPIMP3_GoToPreOrNextMp3(FALSE);

#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
            MMIAPIMP3_UpdateAllAlbumInfo(FALSE);
#endif    

#ifdef MMIAP_F_RELEASE_BG_AUDIO
            MMIAPIAP_ReleaseAudioHandle();
#endif
            UpdateIdleMusicNotification();
            //MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID);
            MMIAPSETWIN_CloseMp3QueueWin();
            if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
            {
                MMK_PostMsg(MAIN_IDLE_WIN_ID, MSG_IDLE_UPDATE_MP3, PNULL, 0);
            }
        }
#endif
    }
    else if((GUIFORM_CHILD_DISP_HIDE != child_fm_display_type && MMIIDLE_FM_PALY_ANIM_CTRL_ID == ctrl_id)
                ||(GUIFORM_CHILD_DISP_HIDE != child_fm_display_type && MMIIDLE_FM_FORM_CTRL_ID == ctrl_id)
            )
    {
#ifdef FM_SUPPORT
        if(MMIFM_GetValidChannelCount() > 1)
        {
            MMIFM_AdjustChannel((int)1);
            UpdateIdleFmRadioNotification();
            //MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_FM_PALY_ANIM_CTRL_ID);
            if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
            {
                MMK_PostMsg(MAIN_IDLE_WIN_ID, MSG_IDLE_UPDATE_MP3, PNULL, 0);
            }
        }
#endif
    }
}


/*****************************************************************************/
//  Description :处理Idle screen 控件对navi-up键的响应
//  Global resource dependence :
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void HandleIdleScreenCtrlNaviUp( MMI_WIN_ID_T win_id)
{
    GUIFORM_CHILD_DISPLAY_E   child_list_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_music_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_fm_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_PUSH_MSG_LIST_CTRL_ID, &child_list_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_FM_FORM_CTRL_ID, &child_fm_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_MUSIC_FORM_CTRL_ID, &child_music_display_type);

    if(!IsNotificationFocus())
    {
        if(GUIFORM_CHILD_DISP_HIDE != child_music_display_type)
        {
            MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID);
            MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_MUSIC_FORM_CTRL_ID);
        }
        else if(GUIFORM_CHILD_DISP_HIDE != child_fm_display_type)
        {
            MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_FM_PALY_ANIM_CTRL_ID);
            MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_FM_FORM_CTRL_ID);
        }
        else if(GUIFORM_CHILD_DISP_HIDE != child_list_display_type)
        {
            MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_PUSH_MSG_LIST_CTRL_ID);
        }
        SetListBottomIndex(MMIIDLE_PUSH_MSG_LIST_CTRL_ID);

        MMIIDLE_NotificationRectCalculate();
    }

}


/*****************************************************************************/
//  Discription: Stop create channel timer
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL void StopUnLockedIn30sTimer(void)
{
    SCI_TRACE_LOW("[idle]:StopUnLockedIn30sTimer enter");

    if(0 != s_locked_from_main_menu_timer)
    {
        MMK_StopTimer(s_locked_from_main_menu_timer);
        s_locked_from_main_menu_timer = 0;
    }
}


/*****************************************************************************/
//  Discription: Start create channel timer
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL void StartUnlockedIn30sTimer(void)
{
    MMISET_BACKLIGHT_SET_T      backlight_info = {0};

    SCI_TRACE_LOW("[idle]:StartUnlockedIn30sTimer enter");

    StopUnLockedIn30sTimer();
    MMIAPISET_GetBackLightInfo(&backlight_info);

    s_locked_from_main_menu_timer = MMK_CreateWinTimer(
                        MAIN_IDLE_WIN_ID, 
                        backlight_info.backlight_time, 
                        FALSE
                        );

}


PUBLIC BOOLEAN MMIAPIIDLE_LockPhone(void)
{
    MMI_WIN_ID_T    idle_win_id  = MAIN_IDLE_WIN_ID;
    MMI_WIN_ID_T    main_menu_win_id  = MAIN_MAINMENU_WIN_ID;
    MMI_HANDLE_T    idle_win_handle = MMK_ConvertIdToHandle(idle_win_id);
    MMI_WIN_PRIORITY_E win_priority = MMK_GetWinPriority(idle_win_handle);
    MMI_WIN_ID_T    focus_win_id = MMK_GetFocusWinId();

    if(!MMIAPIIDLE_IsPhoneLocked() 
        #ifdef BLUETOOTH_SUPPORT
        && !MMIAPIBT_IsSendingOrReceiving()
        #endif
        )
    {
        MMIIDLE_SetLockStatus(IDLE_LOCKED);
        s_is_need_input_securitycode = TRUE;
        StopUnLockedIn30sTimer();
        MMIIDLE_SetUnLockedIn30s(FALSE);
        if(focus_win_id != idle_win_id)
        {
            MMK_SetWinPriority(idle_win_handle, WIN_THREE_LEVEL);
            MMK_WinGrabFocus(idle_win_id);
            MMK_SetWinPriority(idle_win_handle, win_priority);
            if(focus_win_id != main_menu_win_id)
            {
                if (MMK_GetWinDisplayStyleState(focus_win_id, WS_HAS_PROMPT_WIN))
                {
                    SCI_TRACE_LOW("[MMIAPIIDLE_LockPhone] focus_win_id =0x%08x, main_menu_win_id = 0x%08x", focus_win_id, main_menu_win_id);
                    s_active_app_previous_win_id = MMK_GetPrevWinId(focus_win_id);
                }
                s_active_app_win_id = focus_win_id;
                SCI_TRACE_LOW("[MMIAPIIDLE_LockPhone] s_active_app_win_id =0x%08x", s_active_app_win_id);
                SCI_TRACE_LOW("[MMIAPIIDLE_LockPhone] s_active_app_previous_win_id =0x%08x", s_active_app_previous_win_id);
            }
            else
            {
                s_active_app_win_id = 0;
            }
        }
        SCI_TRACE_LOW("[grant]:MMIAPIIDLE_LockPhone(): Locked");
        //SetPsFastSleepTimer(PHONE_LOCK_PS_FAST_SLEEP_TIMEOUT);
    }
    
    return TRUE;
}
 

#if 0
/*****************************************************************************/
//  Description : lock phone
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
//Open Lockbackground win to avoid foucs win is closed by others
PUBLIC BOOLEAN MMIAPIIDLE_LockPhoneEx(void)
{
    if(!MMIAPIIDLE_IsPhoneLocked())
    {
        if(MMK_IsOpenWin(MMIIDLE_LOCK_BACKGROUND_WIN_ID))
        {
            MMK_CloseWin(MMIIDLE_LOCK_BACKGROUND_WIN_ID);            
        }
        MMK_CreateWin((uint32*)MMIIDLE_LOCK_BACKGROUND_WIN_TAB, PNULL);
    }
    return TRUE;
}
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IsPhoneLocked(void)
{
    if(MMIIDLE_IsUnlockedIn30s())
    {
        return FALSE;
    }
    
    if(IDLE_LOCKED == MMIIDLE_GetLockStatus()
    || IDLE_UNLOCKED_ONCE == MMIIDLE_GetLockStatus())
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : unlock phone
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_UnlockPhone(void)
{
    MMI_HANDLE_T win_handle = MMK_ConvertIdToHandle(MAIN_IDLE_WIN_ID);

    MMK_WinResumeWindowFocus(win_handle);

    MMIIDLE_SetLockStatus(IDLE_UNLOCKED);

    StopUnLockedIn30sTimer();
    MMIIDLE_SetUnLockedIn30s(TRUE);
    //StartUnlockedIn30sTimer();

    MMIDEFAULT_SetKeylockByStarKey(FALSE);
    SCI_TRACE_LOW("[grant]:MMIAPIIDLE_UnlockPhone(): Unlocked");

    //SetPsFastSleepTimer(PHONE_UNLOCK_PS_FAST_SLEEP_TIMEOUT);

    return TRUE;
}

/*****************************************************************************/
//  Description : unlock phone
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IsDeviceLockWinOnTop(void)
{
    return MMK_IsFocusWin(MAIN_IDLE_WIN_ID);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_IsDisplayEmergencyNum(BOOLEAN is_display)
{
    GUI_LCD_DEV_INFO        lcd_dev_info             = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T display_rect = {0};  
    GUI_RECT_T client_rect = MMITHEME_GetClientRect();  
    GUISTR_STYLE_T      str_style = {0};
    GUISTR_STATE_T state = {0};
    MMI_STRING_T string = {0};
    wchar  wstr_buf[30]        = {0};
    int result = 0;
    
    str_style.align = ALIGN_HVMIDDLE;
    str_style.font  = MMI_DEFAULT_NORMAL_FONT;
    
    str_style.font_color = MMI_WHITE_COLOR;
    //state = GUISTR_STATE_BOLD | GUISTR_STATE_ALIGN;

   
    display_rect.left = 0;
    display_rect.top = client_rect.bottom - 42;
    display_rect.right = client_rect.right;
    display_rect.bottom = client_rect.bottom;
    
    LCD_FillRect(&lcd_dev_info, display_rect, MMI_BLACK_COLOR);

    string.wstr_ptr = wstr_buf;
    result = MMIIDLE_GetSimAndServiceString(&string);
    
    if(is_display && IDLE_DISPLAY_NO_SERVICE != result)
    {
        MMI_GetLabelTextByLang(STXT_EMERGENCY_CALL_EXT01, &string);
    }
    else if(IDLE_DISPLAY_LIMITED == result)
    {
        MMI_GetLabelTextByLang(STR_START_EMERG_CALL_EXT01, &string);
    }
  
    if(0 != string.wstr_len)
    {
        GUISTR_DrawTextToLCDInRect( 
                        &lcd_dev_info,
                        (const GUI_RECT_T *)&display_rect,       //the fixed display area
                        (const GUI_RECT_T *)&display_rect,       //用户要剪切的实际区域
                        (const MMI_STRING_T *)&string,
                        &str_style,
                        state,
                        GUISTR_TEXT_DIR_AUTO
                    ); 

    }

}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_IsCorrectCode(const wchar *code_ptr, uint16 pwd_len)
{
    BOOLEAN                 result = FALSE;
#ifdef UNISOC_VERSION  
    BOOLEAN b_ret = FALSE;
    uint8 adv_temp_arr[] = {"25815"};
    uint16  adv_len = 0;
    MMISET_LOCKSCREEN_SECURITY_CODE_T devicelock_code = {0};

    adv_len = strlen(adv_temp_arr);
	//just keep 25815 for ms design temp
    b_ret = MMIDEVICELOCKSCREEN_ReadPwd(&devicelock_code);
    if(b_ret)
    {
        if( pwd_len == devicelock_code.pwd_len
            && (0 == MMIAPICOM_WstrStrncmp( code_ptr, (uint8 *)devicelock_code.pwd, pwd_len ) ))
        {
            result = TRUE;
        }
    }
    //dong.chunguang add advance code temp
    /*//remove it
    else if(pwd_len == adv_len
		&&(0 == MMIAPICOM_WstrStrncmp( code_ptr, (uint8 *)adv_temp_arr, pwd_len )))
		{
			result = TRUE;
		}
		*/
		//end
#else //#ifdef UNISOC_VERSION  

    uint8 temp_arr[] = {"1234"};
    uint16  len = 0;
    uint8 adv_temp_arr[] = {"25815"};
    uint16  adv_len = 0;
    if(PNULL == code_ptr)
    {
        return result;
    }
    len = strlen(temp_arr);
    adv_len = strlen(adv_temp_arr);
    if( pwd_len == len
        && (0 == MMIAPICOM_WstrStrncmp( code_ptr, (uint8 *)temp_arr, pwd_len ) ))
    {
        result = TRUE;
    }
    //dong.chunguang add advance code temp
    else if(pwd_len == adv_len
        &&(0 == MMIAPICOM_WstrStrncmp( code_ptr, (uint8 *)adv_temp_arr, pwd_len )))
    {
        result = TRUE;
    }
    //end
#endif
    return (result);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_EnterControlPanel(void *para1, void *para2)
{
#ifdef MMI_SUPPORT_CONTROLPANEL
    MMIAPICP_OpenMainWin();
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_SwtichTorchOnOff(void)
{
    MMIACC_SwtichTorch(MMIACC_FLASHLIGHT_MODE_SWITCH);
}

/*****************************************************************************/
// Description : 
// Global resource dependence : 
//  Author: 
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN InputSecurityCodeWinSoftkey(
                                      wchar         *str_ptr,   //in
                                      uint16        str_len,    //in
                                      MMI_TEXT_ID_T *text_id_ptr//in/out:
                                      )
{
    BOOLEAN  result = FALSE;
    BOOLEAN is_input_emergency_call = FALSE;
    uint16 emergency_call_len = 0;
    MMI_STRING_T str_info = {0};
    
    //set string info
    str_info.wstr_ptr = str_ptr;
    str_info.wstr_len = str_len;
    
    is_input_emergency_call = MMIIdle_IsInputEmergencyCall(&str_info, &emergency_call_len, MN_DUAL_SYS_1);//开机密码界面，未启动SIM卡，输入卡1即可
    if ((is_input_emergency_call) &&
        (str_len == emergency_call_len))
    {
        *text_id_ptr = STXT_SOFTKEY_CALL_MK;
        result = TRUE;
    }
    else if (str_len >= 1)
    {
        *text_id_ptr = STXT_SOFTKEY_OK_MK;
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : handle press * to unlock
//  Global resource dependence :
//  Author: jianma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSecurityCodeWinMsg(
                    MMI_WIN_ID_T     win_id, 
                    MMI_MESSAGE_ID_E    msg_id, 
                    DPARAM              param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_RECT_T main_rect  = MMITHEME_GetClientRect();
    GUI_RECT_T edit_rect = {0};
    GUI_RECT_T label_rect = {0};
    uint16 emergency_call_len = 0;
    GUI_BG_T security_code_label_bg = {0};
    MMI_STRING_T pwd_value = {0};
    IDLE_LOCK_DATA    *idle_lock_data_ptr = (IDLE_LOCK_DATA *)MMK_GetWinAddDataPtr(win_id);

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            //security_code label rect
            label_rect = main_rect;
            label_rect.top += 21;
            label_rect.left += 84;
            label_rect.right -= 84;
            label_rect.bottom = label_rect.top + 72;
            security_code_label_bg.bg_type = GUI_BG_IMG;
            security_code_label_bg.img_id = IMAGE_IDLE_SCREEN_DEVICE_LOCK;
                
            edit_rect = main_rect;
            edit_rect.top = label_rect.bottom + 6;
            edit_rect.bottom = edit_rect.top + 66;

            GUILABEL_SetRect(MMIIDLE_SECURITY_CODE_LABEL_CTRL_ID,&label_rect,FALSE);
            GUILABEL_SetBg(MMIIDLE_SECURITY_CODE_LABEL_CTRL_ID,&security_code_label_bg);

            GUIEDIT_SetHandleRedKey(FALSE, MMIIDLE_SECURITY_CODE_CTRL_ID);
                
            GUIEDIT_SetStyle(MMIIDLE_SECURITY_CODE_CTRL_ID,GUIEDIT_STYLE_MULTI_DYN_DOWN);
            CTRLBASEEDIT_SetLabelId(win_id,MMIIDLE_SECURITY_CODE_CTRL_ID,STR_SECUR_CODE_QUERY_EXT02,FALSE);
            GUIAPICTRL_SetRect(MMIIDLE_SECURITY_CODE_CTRL_ID, &edit_rect);
            GUIEDIT_SetIm (MMIIDLE_SECURITY_CODE_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
            GUIEDIT_SetSoftkey(MMIIDLE_SECURITY_CODE_CTRL_ID,1,1,TXT_NULL,STXT_SOFTKEY_OK_MK,InputSecurityCodeWinSoftkey);

            MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);
            
            if(MMI_IDLE_PHONE_START_UP == idle_lock_data_ptr->e_phone_status)
            {
                GUIEDIT_SetSoftkeyEx(MMIIDLE_SECURITY_CODE_CTRL_ID, 
                    2, 
                    1,
                    TXT_COMMON_COMMON_BACK,
                    TXT_COMMON_COMMON_BACK, 
                    PNULL,
                    PNULL,
                    EDIT_BUTTON_TEXT_ID, 
                    PNULL
                    );
            }
            else
            {
                GUIEDIT_SetSoftkeyEx(MMIIDLE_SECURITY_CODE_CTRL_ID, 
                    2, 
                    0,
                    TXT_COMMON_COMMON_BACK,
                    TXT_COMMON_COMMON_BACK, 
                    PNULL,
                    PNULL,
                    EDIT_BUTTON_TEXT_ID, 
                    PNULL
                    );
            }
            MMK_SetAtvCtrl(win_id,MMIIDLE_SECURITY_CODE_CTRL_ID);
        }   
            break;
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T rect = MMITHEME_GetClientRect();
            GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
            
            rect.top -= 5;
            LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
            GUIWIN_SetStbItemTextValid(0, FALSE);
            GUIEDIT_GetString(MMIIDLE_SECURITY_CODE_CTRL_ID,&pwd_value);
            if(MMIIdle_IsInputEmergencyCall(&pwd_value, &emergency_call_len, MN_DUAL_SYS_MAX)
                && pwd_value.wstr_len == emergency_call_len)
            {
                CTRLBASEEDIT_SetDispPassword(win_id,MMIIDLE_SECURITY_CODE_CTRL_ID,TRUE,TRUE);
                MMIIDLE_IsDisplayEmergencyNum(TRUE);
            }
            else
            {
                CTRLBASEEDIT_SetDispPassword(win_id,MMIIDLE_SECURITY_CODE_CTRL_ID,FALSE,TRUE);
                MMIIDLE_IsDisplayEmergencyNum(FALSE);
            }
        }
            break;
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        case MSG_APP_GREEN:    
            {
                GUIEDIT_GetString(MMIIDLE_SECURITY_CODE_CTRL_ID,&pwd_value);
                if(MMIIdle_IsInputEmergencyCall(&pwd_value, &emergency_call_len, MN_DUAL_SYS_MAX)
                    && pwd_value.wstr_len == emergency_call_len)
                {
                    uint8 temp_str[CC_MAX_TELE_NUM_LEN + 1] = {0};
                    uint32 multi_sys_num = 0;
                    uint16 multi_sys = MN_DUAL_SYS_1;
                    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
                    
                    MMI_WSTRNTOSTR((uint8 *)temp_str, 
                        pwd_value.wstr_len,
                        pwd_value.wstr_ptr,
                        pwd_value.wstr_len,
                        pwd_value.wstr_len);
                    multi_sys_num = MMIAPISET_GetMultiSysSettingNum(&multi_sys, 1);
                    
                    if( 0 == multi_sys_num)
                    {
                        dual_sys = MN_DUAL_SYS_1;
                    }
                    else
                    {
                        dual_sys = (MN_DUAL_SYS_E)multi_sys;
                    }
                    
                    MMIAPICC_MakeCall(
                        dual_sys,
                        temp_str,
                        (uint8)(pwd_value.wstr_len),
                        PNULL,
                        PNULL,
                        MMIAPICC_GetCallSimType(dual_sys),
                        CC_CALL_NORMAL_CALL,
                        PNULL
                        );
                    
                    if(PNULL != idle_lock_data_ptr && MMI_IDLE_PHONE_START_UP == idle_lock_data_ptr->e_phone_status)
                    {
                        //Do nothing.
                    }
                    else
                    {
                        MMK_CloseWin(win_id);
                    }
                }
                else
                {
                    if((GetStartupLockStatus()) && MMIAPICC_IsInState(CC_IN_CALL_STATE))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
                        break;
                    }
                    if (MMIIDLE_IsCorrectCode(pwd_value.wstr_ptr, pwd_value.wstr_len)&&(pwd_value.wstr_len>0))
                    {
                        MMICOM_BANNER_INFO_T banner_info = {0};
                        MMIAPIIDLE_UnlockPhone();
                        //解码正确时，直接进入mainmenu或者active app
                        if(PNULL != idle_lock_data_ptr && PNULL != idle_lock_data_ptr->app_entry_func_ptr)
                        {
                            idle_lock_data_ptr->app_entry_func_ptr(idle_lock_data_ptr->para1, idle_lock_data_ptr->para2);
                        }
                        
                        MMK_CloseWin(win_id);
                        banner_info.banner_type = MMI_BANNER_TEXT;
                        banner_info.start_point.x = 0;
                        banner_info.start_point.y = 0;
                        banner_info.text1_id = STR_LOCK_CODE_APPR_EXT01;
                        MMIAPICOM_OpenBannerWin(win_id, banner_info);
#ifdef NPS_SUPPORT
                        MMIAPINPS_PromptNPSJudgeFunction();
#endif
                                               
                    }
                    else if(pwd_value.wstr_len>0)
                    {
                        MMICOM_BANNER_INFO_T banner_info = {0};
                        banner_info.banner_type = MMI_BANNER_TEXT;
                        banner_info.start_point.x = 0;
                        banner_info.start_point.y = 0;
                        banner_info.text1_id = STR_LOCK_CODE_REJE_EXT01;
                        GUIEDIT_ClearAllStr(MMIIDLE_SECURITY_CODE_CTRL_ID);
                        MMIAPICOM_OpenBannerWin(win_id, banner_info);
                    }
                }
            }
            break;
        case MSG_CTL_EDITBOX_UPDATE_STRNUM:
            GUIEDIT_GetString(MMIIDLE_SECURITY_CODE_CTRL_ID,&pwd_value);
            if(MMIIdle_IsInputEmergencyCall(&pwd_value, &emergency_call_len, MN_DUAL_SYS_MAX)
                && pwd_value.wstr_len == emergency_call_len)
            {
                CTRLBASEEDIT_SetDispPassword(win_id,MMIIDLE_SECURITY_CODE_CTRL_ID,TRUE,TRUE);
                MMIIDLE_IsDisplayEmergencyNum(TRUE);
            }
            else
            {
                CTRLBASEEDIT_SetDispPassword(win_id,MMIIDLE_SECURITY_CODE_CTRL_ID,FALSE,TRUE);
                MMIIDLE_IsDisplayEmergencyNum(FALSE);
            }
            break;
        case MSG_APP_DOWN:
        case MSG_APP_UP:
            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        case MSG_KEYUP_RED:
            if(PNULL != idle_lock_data_ptr && MMI_IDLE_PHONE_START_UP == idle_lock_data_ptr->e_phone_status)
            {
                //Do nothing.
            }
            else
            {
                MMK_CloseWin(win_id);
            }
            break;

        case MSG_CLOSE_WINDOW:
            if(PNULL!= idle_lock_data_ptr)
            {
                MMK_FreeWinAddData(win_id);
            }
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}

 /*****************************************************************************/
//  Description : handle press * to unlock
//  Global resource dependence :
//  Author: jianma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePressStarToUnlockWinMsg(
                    MMI_WIN_ID_T     win_id, 
                    MMI_MESSAGE_ID_E    msg_id, 
                    DPARAM              param)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    IDLE_LOCK_DATA    *idle_lock_data_ptr = (IDLE_LOCK_DATA *)MMK_GetWinUserData(win_id);
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:            
            break;
        case MSG_FULL_PAINT:
            MMIPUB_HandleProgressTextOnlyWinMsg(win_id, msg_id, param);
            break;
        ////mainmenu响应MSG_KEYUP_STAR消息，如果此处响应MSG_KEYDOWN_STAR消息的话，
        ////由idle进入mainmenu就会多出一个MSG_KEYUP_STAR消息响应
        //mainmenu里已经做好相应的处理，这里需要改回MSG_KEYDOWN_STAR
        //因为自动锁屏功能会根据up键来处理，平台收到MSG_KEYUP_STAR的时候判断
        //是否启动定时器，这个时候恰好还没有走到这里解锁，
        //导致自动锁屏定时器在解锁后不能开启；
        //因为很多应用处理按键的动作很不规范，这里还是改回MSG_KEYUP_STAR
        //自动解锁的定时器在设定时去掉当前是否锁屏的判断，在定时器触发时再判断
        case MSG_KEYUP_STAR:
            StopUnlockConfirmTimer();
            MMIAPIIDLE_UnlockPhone();
            if(PNULL != idle_lock_data_ptr && PNULL != idle_lock_data_ptr->app_entry_func_ptr)
            {
                idle_lock_data_ptr->app_entry_func_ptr(idle_lock_data_ptr->para1, idle_lock_data_ptr->para2);
            }
            MMK_CloseWin(win_id);
#ifdef NPS_SUPPORT
            MMIAPINPS_PromptNPSJudgeFunction();
#endif
            break;
        case MSG_KEYLONG_POWER:   //IF power off,close win
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
            if(PNULL != idle_lock_data_ptr)
            {
                SCI_FREE(idle_lock_data_ptr);
            }
            break;
            
       case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;
         
       case MSG_KEYLONG_RED:
           if (IDLE_UNLOCKED == MMIIDLE_GetLockStatus())
           {
               result = MMI_RESULT_FALSE;
           }
           break;

       default:            
            //result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
            result = MMIPUB_HandleProgressTextOnlyWinMsg( win_id, msg_id, param );
            break;
    }
    return result;
}




/*****************************************************************************/
//  Description : set lock status
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetLockStatusByLockMode(void)
{
    if(MMIIDLE_IsUnlockedIn30s())
    {
        StartUnlockedIn30sTimer();
    }

    if(TRUE == MMIAPISET_GetKeyguardSwitch())
    {
        MMIIDLE_SetLockStatus(IDLE_LOCKED);
    }
}

#ifdef MMI_UNISOC_IDLE_EMERGENCY_CALL_SUPPORT
/*****************************************************************************/
//  Description : handle emergency call in idle 
//  Global resource dependence :
//  Author: jianma
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIIdle_MsgidToAscii(
                                   MMI_MESSAGE_ID_E msg_id
                                   )
{
    uint8 result = 0;
    
    switch(msg_id)
    {
        //press key 1-9
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
        result = msg_id - (MSG_KEYDOWN_1) + '1';
        break;
        
        
        //press key *
    case MSG_KEYDOWN_STAR:

        result = '*';
        break;
        
        //press key 0
    case MSG_KEYDOWN_0:

        result = '0';
        break;
        
        //press key #
    case MSG_KEYDOWN_HASH:
        result = '#';
        break;
        
    default:
        result = 0;
        break;
    }
    
    return result;
}
/*****************************************************************************/
//  Description : handle emergency call in idle 
//  Global resource dependence :
//  Author: jianma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIdle_IsEmergencyCallOnly(MMI_MESSAGE_ID_E msg_id)
{
    BOOLEAN is_mergencycall = FALSE;
    MMI_STRING_T key_str = {0};
    wchar key_code = 0;
    uint16 str_len = 0;

    key_str.wstr_ptr = &key_code;
    
    switch(msg_id)
    {
    case MSG_APP_1:
    case MSG_APP_2:
    case MSG_APP_3:
    case MSG_APP_4:
    case MSG_APP_5:
    case MSG_APP_6:
    case MSG_APP_7:
    case MSG_APP_8:
    case MSG_APP_9:
    case MSG_APP_0:
    case MSG_APP_HASH:
    case MSG_APP_STAR:
        key_code = MMIAPIIdle_MsgidToAscii(msg_id);
        key_str.wstr_len = sizeof(key_code)/sizeof(wchar);
        if(MMIIdle_IsInputEmergencyCall(&key_str, &str_len, MMI_DUAL_SYS_MAX))
        {
            is_mergencycall = TRUE;
        }
        break;
        
    default:
        is_mergencycall = FALSE;
        break;
    }
    
    return is_mergencycall;
}

/*
LOCAL MMI_RESULT_E  HandleLockBackgroundWinMsg(
                    MMI_WIN_ID_T     win_id, 
                    MMI_MESSAGE_ID_E    msg_id, 
                    DPARAM              param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMIAPIIDLE_LockPhoneEx();
            break;
       case MSG_GET_FOCUS:
            MMK_CloseWin(win_id);
            break;
        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;
        default:
            break;
    }
    return result;
}
*/


/*****************************************************************************/
//  Description : handle emergency call in idle 
//  Global resource dependence :
//  Author: jianma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsIdleEmergencyCallWinMsg(
                    MMI_WIN_ID_T     win_id, 
                    MMI_MESSAGE_ID_E    msg_id, 
                    DPARAM              param)

{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
        
    MMI_CTRL_ID_T ctrl_id = MMIIDLE_EMERGENCY_PHONENUM_CTRL_ID;
    MMI_STRING_T str_info = {0};
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    uint32 multi_sys_num = 0;
    uint16 multi_sys = MN_DUAL_SYS_1;
    GUI_RECT_T  edit_rect = {0};
    GUI_LCD_DEV_INFO                    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T  rect = MMITHEME_GetClientRect();
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {
                GUI_BG_T        edit_bg = {GUI_BG_COLOR,GUI_SHAPE_RECT,0,MMI_BLACK_COLOR,FALSE};
                GUI_BORDER_T    edit_border = {1,MMI_BACKGROUND_COLOR,GUI_BORDER_NONE};
                GUI_COLOR_T     edit_font_color = 0xffff;
                GUIEDIT_FONT_T  editfont ={0};

#ifdef MMI_ETWS_SUPPORT
                if (MMK_IsOpenWin(MMISMS_ETWS_NEWMSG_WIN_ID))
                {
                    MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
                MMIAPISMSCB_SetIsNeedRedisplay(TRUE);
                }
#endif                
                edit_rect = rect;
                edit_rect.bottom = edit_rect.top + 45;
                GUIAPICTRL_SetRect(ctrl_id, &edit_rect);

                GUIEDIT_SetStyle(ctrl_id, GUIEDIT_STYLE_SINGLE);
                edit_bg.color=MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT);

                GUIEDIT_SetBg(ctrl_id, &edit_bg);
                GUIEDIT_SetBorder(ctrl_id, &edit_border);
                GUIEDIT_SetFontColor(ctrl_id, edit_font_color);
                
                editfont.is_valid = TRUE;
                editfont.big_font = SONG_FONT_36;
                editfont.mid_font = SONG_FONT_22;
                editfont.small_font = SONG_FONT_14;


                GUIEDIT_SetFontEx(MMK_GetCtrlHandleByWin(win_id, ctrl_id), &editfont);
                GUIEDIT_SetDialEditAdaptive(MMK_GetCtrlHandleByWin(win_id, ctrl_id), TRUE, TRUE);

                GUIEDIT_SetFont(ctrl_id, SONG_FONT_29);
                GUIEDIT_SetHandleRedKey(FALSE, ctrl_id);


                GUIEDIT_SetSoftkeyEx(ctrl_id, 2, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, PNULL, PNULL, EDIT_BUTTON_TEXT_ID, PNULL);
                
                MMK_SetAtvCtrl(win_id,ctrl_id);
                if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STXT_EMERGENCY_CALL_EXT01))
                {
                    GUIWIN_UpdateStb();
                }
            }
            break;
        case MSG_CTL_EDITBOX_UPDATE_STRNUM:
            GUIEDIT_GetString(ctrl_id, &str_info);
            
            if (0 == str_info.wstr_len)
            {
                MMK_CloseWin(win_id);
#ifdef MMI_ETWS_SUPPORT                
                if(MMISMSCB_ETWSGetisNeedRedisplay())
                {
                    MMIAPISMSCB_RedisplayETWSMsg();
                }   
#endif                
            }
            else
            {
                BOOLEAN is_input_emergency_call = FALSE;
                uint16 emergency_call_len = 0;
                
                is_input_emergency_call = MMIIdle_IsInputEmergencyCall(&str_info, &emergency_call_len, MMI_DUAL_SYS_MAX);
                
                if (!is_input_emergency_call)
                {
                    MMK_CloseWin(win_id);
#ifdef MMI_ETWS_SUPPORT                    
                    if(MMISMSCB_ETWSGetisNeedRedisplay())
                    {
                        MMIAPISMSCB_RedisplayETWSMsg();
                    }   
#endif                    
                }   

            }
            break;
        case MSG_FULL_PAINT:
            if(MMK_IsFocusWin(win_id))
            {
                LCD_FillArgbRect((const GUI_LCD_DEV_INFO*)&lcd_dev_info,
                                 MMITHEME_GetFullScreenRect(),
                                 MMI_BACKGROUND_COLOR,
                                 MMIIDLE_EMERGENCY_CALL_BG_ALPHA);
            }
            break;
        case MSG_CTL_MIDSK:             
        case MSG_CTL_OK:
        case MSG_APP_WEB:
        case MSG_APP_GREEN:    
        {
            uint8 temp_str[CC_MAX_TELE_NUM_LEN + 1] = {0};
            BOOLEAN is_input_emergency_call = FALSE;
            uint16 emergency_call_len = 0;
            
            GUIEDIT_GetString(ctrl_id, &str_info);
            
            is_input_emergency_call = MMIIdle_IsInputEmergencyCall(&str_info, &emergency_call_len, MMI_DUAL_SYS_MAX);
            
            if (is_input_emergency_call && (emergency_call_len == str_info.wstr_len))
            {
                MMI_WSTRNTOSTR((uint8 *)temp_str, 
                    str_info.wstr_len,
                    str_info.wstr_ptr,
                    str_info.wstr_len,
                    str_info.wstr_len);
                multi_sys_num = MMIAPISET_GetMultiSysSettingNum(&multi_sys, 1);

                if( 0 == multi_sys_num)
                {
                    dual_sys = MN_DUAL_SYS_1;
                }
                else
                {
                    dual_sys = (MN_DUAL_SYS_E)multi_sys;
                }
                
                MMIAPICC_MakeCall(
                    dual_sys,
                    temp_str,
                    (uint8)(str_info.wstr_len),
                    PNULL,
                    PNULL,
                    MMIAPICC_GetCallSimType(dual_sys),
                    CC_CALL_NORMAL_CALL,
                    PNULL
                    );

#ifdef MMI_ETWS_SUPPORT        
           if( TRUE == MMIAPISMSCB_IsETWSAlert() )
	    {
		MMIAPISMSCB_SetEMCallStatus(TRUE);
		MMIAPISET_StopSOSVibrate();
	    }
#endif
                MMK_CloseWin(win_id);
            }
        }
            break;
        
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;

#ifdef MMI_ETWS_SUPPORT        
    case MSG_CLOSE_WINDOW:
		{
    // display etws prompt
			if(  (FALSE == MMIAPICC_IsInState(CC_IN_CALL_STATE)) 
				&& (TRUE == MMIAPISMSCB_GetIsNeedRedisplay()) )
			{
				MMIAPISMSCB_ShowNewETWSPrompt();
			}
		}
        break;
#endif

        default:
            break;
    }
    return result;
}

/*****************************************************************************/
//  Description : handle  idle emergency call message
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIdle_OpenEmergencyWin(MMI_MESSAGE_ID_E msg_id)
{
    BOOLEAN is_mergencycall = FALSE;
    MMI_STRING_T key_str = {0};
    wchar key_code = 0;
    uint16 str_len = 0;

    key_str.wstr_ptr = &key_code;
    
    switch(msg_id)
    {
    case MSG_APP_1:
    case MSG_APP_2:
    case MSG_APP_3:
    case MSG_APP_4:
    case MSG_APP_5:
    case MSG_APP_6:
    case MSG_APP_7:
    case MSG_APP_8:
    case MSG_APP_9:
    case MSG_APP_0:
    case MSG_APP_HASH:
    case MSG_APP_STAR:
        key_code = MMIAPIIdle_MsgidToAscii(msg_id);
        key_str.wstr_len = sizeof(key_code)/sizeof(wchar);
        if(MMIIdle_IsInputEmergencyCall(&key_str, &str_len, MMI_DUAL_SYS_MAX))
        {
            is_mergencycall = TRUE;
            MMK_CreateWin((uint32*)MMIIDLE_EMERGENCY_WIN_TAB, PNULL);

            if (0 != key_code)
            {
                GUIEDIT_SetString(
                    MMIIDLE_EMERGENCY_PHONENUM_CTRL_ID,
                    &key_code,
                    1);
            }
        }
        break;
        
    default:
        is_mergencycall = FALSE;
        break;
    }
    
    return is_mergencycall;
}
#endif

/*****************************************************************************/
//  Description : set notification list defaut index
//  Global resource dependence :
//  Author: Grant.Peng
//  Note:
/*****************************************************************************/
LOCAL void SetListBottomIndex(MMI_CTRL_ID_T ctrl_id)
{
    uint16 cur_index = 0;
    uint16 list_item_num = CTRLLIST_GetTotalItemNum(ctrl_id);

    if(list_item_num != 0)
    {
        cur_index = list_item_num - 1;
        GUILIST_SetCurItemIndex(ctrl_id, cur_index);
    }
}

/*****************************************************************************/
//  Description : handle common idle message
//  Global resource dependence :
//  Author: jianma
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIIDLE_MStyleHandleMsg ( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
#ifdef MMI_INDIAN_SOS_SUPPORT
    BOOLEAN  is_indiasos_supported = MMIAPIPHONE_IsIndiaSOSSupport();
#endif

    switch (msg_id)
    {
    case MSG_FULL_PAINT:
        OutIdleWinContent();
        DisplayIdleWinSoftkey();
        // NOTE: update status bar here for Bug 1379649 
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_WITHOUT_HEADER, TRUE);
        MMIAPICOM_StatusAreaWithheaderSet();
        //MMITHEME_UpdateRect();
        break;

    case MSG_BACKLIGHT_TURN_OFF:
        s_is_need_input_securitycode = TRUE;
        break;
        
    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        MMIIDLE_NotificationUpdateSoftkey();
        break;

    case MSG_APP_WEB:
        {
            SCI_TraceLow("[SKY] MMIIDLE_MStyleHandleMsg :: MSG_APP_WEB");
#ifdef UIX8910_ORG_IDLE_STYLE
            if(MMIIDLE_IsUnlockedIn30s())
            {
                MMIAPIIDLE_UnlockPhone();
                UlockPhoneToActiveApp(PNULL, PNULL);
            }
            else
            {
                IDLE_LOCK_DATA idle_lock_data = {0};

                idle_lock_data.e_phone_status = MMI_IDLE_PHONE_NORMAL;
                idle_lock_data.app_entry_func_ptr = UlockPhoneToActiveApp;
                idle_lock_data.para1 = PNULL;
                idle_lock_data.para2 = PNULL;
                MMIIDLE_UlockPhoneBySecurityMode(&idle_lock_data);
            }
#else
		MMIAPIMENU_CreatMainMenu();
#endif
        }
        break;
     
    case MSG_APP_OK:
        {
		SCI_TraceLow("[SKY] MMIIDLE_MStyleHandleMsg :: MSG_APP_OK");
#ifdef UIX8910_ORG_IDLE_STYLE
		if((MMIIDLE_IsUnlockedIn30s() || (IDLE_UNLOCKED == MMIIDLE_GetLockStatus())) && (FALSE == MMIDEFAULT_IsKeylockByStarKey()))
		{
			
		    IDLE_LOCK_DATA idle_lock_data = {0};
		    idle_lock_data.e_phone_status = MMI_IDLE_PHONE_NORMAL;
		    idle_lock_data.app_entry_func_ptr =  UlockPhoneByLeftShortcut;
		    idle_lock_data.para1 = PNULL;
		    idle_lock_data.para2 = PNULL;

		    MMIIDLE_UlockPhoneBySecurityMode(&idle_lock_data);
		}
		else
		{
		    IDLE_LOCK_DATA idle_lock_data = {0};

		    idle_lock_data.e_phone_status = MMI_IDLE_PHONE_NORMAL;
		    idle_lock_data.app_entry_func_ptr = UlockPhoneToActiveApp;
		    idle_lock_data.para1 = PNULL;
		    idle_lock_data.para2 = PNULL;
		    MMIIDLE_UlockPhoneBySecurityMode(&idle_lock_data);
		}
#else
		MMIAPIMENU_CreatMainMenu();
#endif
    	}
        break;

    case MSG_CTL_OK:
        break;

    case MSG_CTL_MIDSK:
        SCI_TraceLow("[SKY] MMIIDLE_MStyleHandleMsg :: MSG_CTL_MIDSK");
        HandleIdleScreenCtrlMidSK(win_id);
        break;

    case MSG_APP_CANCEL:
#ifdef UIX8910_ORG_IDLE_STYLE
        {
		if((MMIIDLE_IsUnlockedIn30s() || (IDLE_UNLOCKED == MMIIDLE_GetLockStatus())) && (FALSE == MMIDEFAULT_IsKeylockByStarKey()))
		{
			IDLE_LOCK_DATA idle_lock_data = {0};
			idle_lock_data.e_phone_status = MMI_IDLE_PHONE_NORMAL;
			idle_lock_data.app_entry_func_ptr =  UlockPhoneByRightShortcut;
			idle_lock_data.para1 = PNULL;
			idle_lock_data.para2 = PNULL;

			MMIIDLE_UlockPhoneBySecurityMode(&idle_lock_data);
		}
		else
		{
			IDLE_LOCK_DATA idle_lock_data = {0};

			idle_lock_data.e_phone_status = MMI_IDLE_PHONE_NORMAL;
			idle_lock_data.app_entry_func_ptr = UlockPhoneToActiveApp;
			idle_lock_data.para1 = PNULL;
			idle_lock_data.para2 = PNULL;
			MMIIDLE_UlockPhoneBySecurityMode(&idle_lock_data);
		}       
    	}
#endif	
        break;
        
    case MSG_CTL_CANCEL:    
        break;
        
#ifdef MMI_UNISOC_IDLE_EMERGENCY_CALL_SUPPORT
    case MSG_KEYDOWN_0:
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
    case MSG_KEYDOWN_HASH:
        s_is_keydown_in_idle = TRUE;
        break;
    case MSG_KEYDOWN_STAR:
    case MSG_KEYUP_STAR: 
        if((MMIIDLE_IsUnlockedIn30s()) || (IDLE_UNLOCKED == MMIIDLE_GetLockStatus()))
        {
            MMIAPIIDLE_UnlockPhone();
            MMIAPIIDLE_OpenDialWin(msg_id, TRUE);
        }
        else
        {
            //判断输入是否为紧急号码，如果是则打开dial_win，否则关闭
            MMIAPIIdle_OpenEmergencyWin( msg_id) ;
        }
        break;
    case MSG_KEYUP_0:
    case MSG_KEYUP_1:
    case MSG_KEYUP_2:
    case MSG_KEYUP_3:
    case MSG_KEYUP_4:
    case MSG_KEYUP_5:
    case MSG_KEYUP_6:
    case MSG_KEYUP_7:
    case MSG_KEYUP_8:
    case MSG_KEYUP_9:
    case MSG_KEYUP_HASH:
        if(s_is_keydown_in_idle)
        {
            //拨号盘响应KEYDOWN消息，所以此处需要将KEYUP消息转换为KEYDOWN消息后再传给拨号盘
            msg_id = msg_id + (MSG_KEYDOWN - MSG_KEYUP);

            if((MMIIDLE_IsUnlockedIn30s()) || (IDLE_UNLOCKED == MMIIDLE_GetLockStatus()))
            {
                MMIAPIIDLE_UnlockPhone();
                MMIAPIIDLE_OpenDialWin(msg_id, TRUE);
            }
            else
            {
                //判断输入是否为紧急号码，如果是则打开dial_win，否则关闭
                MMIAPIIdle_OpenEmergencyWin( msg_id) ;
            }
            s_is_keydown_in_idle = FALSE;
        }
        break;
#endif


    case MSG_KEYLONG_0:
		MMIIDLE_SwtichTorchOnOff();
#if 0//def  BROWSER_SUPPORT
        if((MMIIDLE_IsUnlockedIn30s()) || (IDLE_UNLOCKED == MMIIDLE_GetLockStatus()))
        {
            MMIAPIIDLE_UnlockPhone();
            UlockPhoneToBrowser(PNULL, PNULL);
        }
        else
        {
            IDLE_LOCK_DATA idle_lock_data = {0};
            idle_lock_data.e_phone_status = MMI_IDLE_PHONE_NORMAL;
            idle_lock_data.app_entry_func_ptr =  UlockPhoneToBrowser;
            idle_lock_data.para1 = PNULL;
            idle_lock_data.para2 = PNULL;
            MMIIDLE_UlockPhoneBySecurityMode(&idle_lock_data);
        }
#endif
        break;
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
    case MSG_KEYLONG_1:
        if((MMIIDLE_IsUnlockedIn30s()) || (IDLE_UNLOCKED == MMIIDLE_GetLockStatus()))
        {
            MMIAPIIDLE_UnlockPhone();
            //拨号盘voice mail接口
            UlockPhoneToVoiceMailFunction(PNULL, PNULL);
        }
        else
        {
            IDLE_LOCK_DATA idle_lock_data = {0};
            idle_lock_data.e_phone_status = MMI_IDLE_PHONE_NORMAL;
            idle_lock_data.app_entry_func_ptr =  UlockPhoneToVoiceMailFunction;
            idle_lock_data.para1 = PNULL;
            idle_lock_data.para2 = PNULL;
            MMIIDLE_UlockPhoneBySecurityMode(&idle_lock_data);
        }
        break;
#endif
    case MSG_KEYLONG_5:
#ifdef MMI_INDIAN_SOS_SUPPORT
        if (TRUE == is_indiasos_supported)
        {
            MMIAPISET_StartSOSProcedures();
            break;
        }
#endif
    case MSG_KEYLONG_2:
    case MSG_KEYLONG_3:
    case MSG_KEYLONG_4:
    case MSG_KEYLONG_6:
    case MSG_KEYLONG_7:
    case MSG_KEYLONG_8:
    case MSG_KEYLONG_9:
        if((MMIIDLE_IsUnlockedIn30s()) || (IDLE_UNLOCKED == MMIIDLE_GetLockStatus()))
        {
            //拨号盘speed dial接口
            MMIAPIIDLE_HandleSpeedDialFunction(msg_id);
        }
        else
        {
            IDLE_LOCK_DATA idle_lock_data = {0};
            
            idle_lock_data.e_phone_status = MMI_IDLE_PHONE_NORMAL;
            idle_lock_data.app_entry_func_ptr = UlockPhoneToSpeedDial;
            idle_lock_data.para1 = (IDLE_APP_PARA_T)msg_id;
            idle_lock_data.para2 = PNULL;
            MMIIDLE_UlockPhoneBySecurityMode(&idle_lock_data);
        }       
        break;

    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        UpdateNotificationListItemIconData((GUILIST_NEED_ITEM_CONTENT_T *)param);
        break;

    case MSG_KEYREPEAT_UP:
    case MSG_KEYUP_UP:
#ifdef UIX8910_ORG_IDLE_STYLE
        HandleIdleScreenCtrlNaviUp(win_id);
        MMIIDLE_NotificationUpdateSoftkey();
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
#else
	  MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_UP);//audio player
#endif
        break;

    case MSG_KEYREPEAT_DOWN:
    case MSG_APP_DOWN:
#ifdef UIX8910_ORG_IDLE_STYLE
        MMK_WinInactiveCtrl(MAIN_IDLE_WIN_ID, FALSE);   
        MMIIDLE_NotificationUpdateSoftkey();
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
#else
	  MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_DOWN);//DUALSIM_SETTING
#endif
        break;

    case MSG_KEYREPEAT_LEFT:
    case MSG_KEYUP_LEFT:
#ifdef UIX8910_ORG_IDLE_STYLE
        HandleIdleScreenCtrlNaviLeft(win_id);
#else
	  MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_LEFT);//edit new sms
#endif
        break;

    case MSG_KEYREPEAT_RIGHT:
    case MSG_KEYUP_RIGHT:
#ifdef UIX8910_ORG_IDLE_STYLE
        HandleIdleScreenCtrlNaviRight(win_id);
#else
        MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_RIGHT);//env. PROFILE
#endif
        break;

    case MSG_OPEN_WINDOW:
    case MSG_GET_FOCUS:
#ifdef UI_MULTILAYER_SUPPORT
        if (UILAYER_IsLayerActive(MMITHEME_GetDefaultLcdDev()))
        {
            UILAYER_Clear(MMITHEME_GetDefaultLcdDev());
        }
        CreateCommonIdlePaintLayer(win_id);
        CreateCommonIdleStbLayer(win_id);
        CreateCommonIdlePrivacyModeLayer(win_id);
        CreateCommonIdleNotiLayer(win_id);
#endif
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_WITHOUT_HEADER, TRUE);
        MMIAPICOM_StatusAreaWithheaderSet();
#ifdef UIX8910_ORG_IDLE_STYLE
        UpdateIdleWinPushNotificationList(MMIIDLE_PUSH_MSG_LIST_CTRL_ID);

		MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_PUSH_MSG_LIST_CTRL_ID);
        MMIIDLE_UpdateFmMusicNotification();
        MMIIDLE_NotificationRectCalculate();
        SetListBottomIndex(MMIIDLE_PUSH_MSG_LIST_CTRL_ID);
        MMIIDLE_NotificationUpdateSoftkey();
#endif

        //SetLockStatusByLockMode();
        MMK_WinInactiveCtrl(MAIN_IDLE_WIN_ID, FALSE);   
    #ifdef CAMERAROLL_SUPPORT    
        MMISRV_CAMERAROLL_Check_auto_upload_photo();
    #endif
        //MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;

    case MSG_CLOSE_WINDOW:
    case MSG_LOSE_FOCUS:
#ifdef UI_MULTILAYER_SUPPORT
        ReleaseCommonIdlePaintLayer();
        ReleaseCommonIdleStbLayer(win_id);
        ReleaseCommonIdleNotiLayer(win_id);
        ReleaseCommonIdlePrivacyModeLayer(win_id);
#endif
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_WITHOUT_HEADER, FALSE);
        MMIAPICOM_StatusAreaWithheaderSet();
        GUIWIN_SetStbItemTextValid(0, FALSE);
        break;

    case MSG_IDLE_UPDATE_DATETIME:
    case MSG_IDLE_UPDATE_COUNTEDTIME:
    case MSG_STK_SET_UP_IDLE_MODE:
#ifdef UI_MULTILAYER_SUPPORT
        OutIdleWinContent();
#else
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
#endif
        break;
    
    case MSG_IDLE_UPDATE_MP3:
    case MSG_IDLE_UPDATE_NOTIFICATION_LIST:
#ifdef UIX8910_ORG_IDLE_STYLE
        if(MSG_IDLE_UPDATE_NOTIFICATION_LIST == msg_id)
        {
            UpdateIdleWinPushNotificationList(MMIIDLE_PUSH_MSG_LIST_CTRL_ID);
        }
        else
        {
            MMIIDLE_UpdateFmMusicNotification();
        }
        MMIIDLE_NotificationRectCalculate();
#endif
        if(MMK_IsFocusWin(win_id))
        {
            #ifdef UI_MULTILAYER_SUPPORT
            OutIdleWinContent();
            #else
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            #endif
        }
        break;

#if 0
#ifdef MMI_SUPPORT_CONTROLPANEL//Queen project no need
    case MSG_KEYLONG_HASH:
        if((MMIIDLE_IsUnlockedIn30s()) || (IDLE_UNLOCKED == MMIIDLE_GetLockStatus()))
        {
            MMIAPIIDLE_UnlockPhone();
            MMIIDLE_EnterControlPanel(PNULL, PNULL);
        }
        else
        {
            //长按#键打开control panel
            IDLE_LOCK_DATA idle_lock_data = {0};
            idle_lock_data.e_phone_status = MMI_IDLE_PHONE_NORMAL;
            idle_lock_data.app_entry_func_ptr =  MMIIDLE_EnterControlPanel;
            idle_lock_data.para1 = PNULL;
            idle_lock_data.para2 = PNULL;
            MMIIDLE_UlockPhoneBySecurityMode(&idle_lock_data);
        }
        break;
#endif
#endif

    case MSG_KEYLONG_HASH:
		IDLE_HASH_Switch_Slient(win_id);
        break;
    case MSG_TIMER:
        if (GetUnlockTimerID() == *(uint8*)param)
        {
            //stop timer
            StopUnlockConfirmTimer();

            if (IDLE_UNLOCKED_ONCE == MMIIDLE_GetLockStatus())
            {
                MMIIDLE_SetLockStatus(IDLE_LOCKED);
                if(MMK_IsOpenWin(MMI_DEVICELOCK_PROGRESS_WIN_ID))
                {
                    MMK_CloseWin(MMI_DEVICELOCK_PROGRESS_WIN_ID);
                }
            }
        }
        else if(s_locked_from_main_menu_timer == *(uint8*)param)
        {
            StopUnLockedIn30sTimer();
            if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
            {
                MMIIDLE_SetUnLockedIn30s(FALSE);
            }
        }
        break;

    //[in idle,press the call button should enter call log]
    //绿键已拨电话
    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:
        {
	#ifdef UIX8910_ORG_IDLE_STYLE	
            IDLE_LOCK_DATA idle_lock_data = {0};
            idle_lock_data.e_phone_status = MMI_IDLE_PHONE_NORMAL;
            idle_lock_data.app_entry_func_ptr =  UlockPhoneToCalllog;
            idle_lock_data.para1 = PNULL;
            idle_lock_data.para2 = PNULL;
            MMIIDLE_UlockPhoneBySecurityMode(&idle_lock_data);
	#else
			MMIAPICL_OpenCallsListWindow(MMICL_CALL_DIALED);
	#endif
        }
        break;
    //[end]

    case MSG_KEYLONG_RED:
        if (IDLE_UNLOCKED == MMIIDLE_GetLockStatus())
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result ;
}



/*****************************************************************************/
//  Description :在Idle screen上显示时间
//  Global resource dependence :
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void OutputIdleWinTime(void)
{
    GUI_POINT_T                point               = {0};
    GUISTR_STYLE_T         time_text_style = {0};
    uint8 time_buf[MMIIDLE_TIME_LEN]  = {0};
    uint16 time_str_width = 0;
    uint16 time_str_height = 0;
    uint16 time_type_height = 0;
    uint16 time_type_width = 0;

    uint8 time_hour = 0;
    uint8 time_min = 0;

    GUISTR_INFO_T str_info = {0};

    MMI_STRING_T time_string = {0};
    MMI_STRING_T time_type_str = {0};
    wchar                   time_wstr_buf[MMIIDLE_TIME_LEN]        = {0};
    SCI_TIME_T         time = {0};
    MMI_IDLE_DISPLAY_T          idle_disp_style     = {0};
    GUI_RECT_T display_rect = {0};

    GUI_LCD_DEV_INFO            lcd_dev_info        = *GetCommonIdlePaintLcdDev();
    MMISET_TIME_DISPLAY_TYPE_E  time_type = MMISET_TIME_12HOURS;
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;

    //显示时间类型:
    time_type = MMIAPISET_GetTimeDisplayType();
    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_TIME);
    MMIIDLE_GetIdleTextStyle(&time_text_style, idle_disp_style.text_font, MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_TIME));

    //get system time
    TM_GetSysTime(&time);

    time_min = time.min;
    time_hour = time.hour;

    if(time_type == MMISET_TIME_12HOURS)
    {
        if ((MMISET_TIME_PM_HOUR > time.hour))//AM
        {
            if ((MMISET_TIME_1_HOUR > time.hour))//00:00-1:00
            {
                time_hour = time.hour + MMISET_TIME_PM_HOUR;
            }
        }
        else//PM
        {
            if ((MMISET_TIME_PM_HOUR <= time.hour) && (MMISET_TIME_13_HOUR > time.hour))//12:00-13:00
            {
            }
            else
            {
                time_hour = time.hour -MMISET_TIME_PM_HOUR;
            }
        }
    }

    sprintf((char *)time_buf, "%02d:%02d", time_hour, time_min);

    MMIAPICOM_StrToWstr(time_buf, time_wstr_buf);

    time_string.wstr_ptr = time_wstr_buf;
    time_string.wstr_len = MMIAPICOM_Wstrlen(time_string.wstr_ptr);

    text_state |=GUISTR_STATE_BOLD;
    GUISTR_GetStringInfo( &time_text_style, &time_string, text_state, &str_info );

    time_str_width = str_info.width;
    time_str_height = str_info.height;


    if(time_type == MMISET_TIME_12HOURS)
    {
        if ((MMISET_TIME_PM_HOUR > time.hour))//AM
        {
            MMI_GetLabelTextByLang(STR_CLK_IDLE_AM, &time_type_str);
        }
        else//PM
        {
            MMI_GetLabelTextByLang(STR_CLK_IDLE_PM, &time_type_str);
        }
        
        time_text_style.font = SONG_FONT_24;
        time_text_style.align = ALIGN_BOTTOM;
        GUISTR_GetStringInfo( &time_text_style, &time_type_str, text_state, &str_info );
        time_type_height = str_info.height;
        time_type_width = str_info.width;
    }

    point = CalStartPoint(idle_disp_style, (time_str_width + time_type_width + MMI_IDLE_TIME_INTERVAL));

#if defined (MAINLCD_SIZE_240X320)
    point.y = MMI_IDLE_TIME_TOP + 2;
#endif

    time_text_style.font  = idle_disp_style.text_font;

    display_rect.left = point.x;
    display_rect.top = point.y;
    display_rect.right = point.x + time_str_width;
    display_rect.bottom = point.y + time_str_height;

    GUISTR_DrawTextToLCDInRect(
        &lcd_dev_info,
        (const GUI_RECT_T *)&display_rect,       //the fixed display area
        (const GUI_RECT_T *)&display_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T *)&time_string,
        &time_text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
    );


    //12小时制
    if(time_type == MMISET_TIME_12HOURS)
    {
        display_rect.top = point.y;
        display_rect.left = point.x + time_str_width + MMI_IDLE_TIME_INTERVAL;
        display_rect.right = display_rect.left + time_type_width;
        display_rect.bottom = point.y + time_type_height/2;
        time_text_style.font = SONG_FONT_14;

        GUISTR_DrawTextToLCDInRect(
                &lcd_dev_info,
                (const GUI_RECT_T *)&display_rect,       //the fixed display area
                (const GUI_RECT_T *)&display_rect,       //用户要剪切的实际区域
                (const MMI_STRING_T *)&time_type_str,
                &time_text_style,
                text_state,
                GUISTR_TEXT_DIR_AUTO
    );

    }
    
}




/*****************************************************************************/
//  Description :在Idle screen上显示SIM卡状态信息
//  Global resource dependence :
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void OutputIdleSimStatus(void)
{

    MMI_IDLE_DISPLAY_T          idle_disp_style     = {0};
    GUI_POINT_T                 point               = {0};
    wchar                               wstr_buf[30]        = {0};
    GUI_FONT_T                  font                = SONG_FONT_24;
    GUI_COLOR_T                 color               = 0;
    uint16                      str_width           = 0;
    MMI_STRING_T            display_string   = {0};
    GUI_LCD_DEV_INFO            lcd_dev_info        = *GetCommonIdlePaintLcdDev();
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ELLIPSIS;
    GUI_RECT_T                   display_rect = {0};

    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_DATE);
    font = idle_disp_style.text_font;
    color = MMI_WHITE_COLOR;

    text_style.region_num = 0;
    MMIIDLE_GetIdleTextStyle(&text_style, font, color);

    display_string.wstr_ptr = wstr_buf;

    MMIIDLE_GetSimAndServiceString(&display_string);

    if(0 != display_string.wstr_len)
    {
        str_width = GUISTR_GetStringWidth(&text_style, &display_string, text_state);

        point.x = MMI_IDLE_TIME_MARGIN;
        point.y = MMI_IDLE_NTF_AREA_TOP + s_idle_privacy_mode_ntf_height + MMI_IDLE_NTF_TOP_MARGIN;

        display_rect.top = point.y;
        display_rect.left = point.x;
        display_rect.right= point.x + str_width;
        display_rect.bottom = point.y + MMI_IDLE_NTF_AREA_HEIGHT;


        GUISTR_DrawTextToLCDInRect(
                &lcd_dev_info,
                (const GUI_RECT_T *)&display_rect,       //the fixed display area
                (const GUI_RECT_T *)&display_rect,       
                (const MMI_STRING_T *)&display_string,
                &text_style,
                text_state,
                GUISTR_TEXT_DIR_AUTO);

    }

}

LOCAL void DisplaySimStatus(MN_DUAL_SYS_E dual_sys)
{

    wchar                       wstr_buf[MMIPHONE_MAX_OPER_NAME_LEN]        = {0};
#if defined (MAINLCD_SIZE_128X160)	
    GUI_FONT_T                  font                = SONG_FONT_14;
#else
    GUI_FONT_T                  font                = SONG_FONT_16;
#endif
    GUI_COLOR_T                 color               = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_NETWORKNAME);
    uint16                      str_width           = 0;
    MMI_STRING_T                display_string      = {0};
    GUI_LCD_DEV_INFO            lcd_dev_info        = *GetCommonIdlePaintLcdDev();
    GUISTR_STYLE_T              text_style = {0};/*lint !e64*/
    GUISTR_STATE_T              text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    GUI_RECT_T                  string_display_rect = {0};
    GUI_RECT_T                  sim_icon_rect = {0};
    MMI_IMAGE_ID_T              img_id = 0;
    MMISET_IDLE_DISPLAY_T   *idle_mode  =   PNULL;
    idle_mode = MMIAPISET_GetIdleDisplayMode();

    if(dual_sys == MN_DUAL_SYS_1)
    {
        if(idle_mode->visible_flag.item_flag.is_show_network_name)
        {
            sim_icon_rect.top = MMI_IDLE_SIM_STATUS_TOP + MMI_IDLE_INTERVAL_MARGIN;
            sim_icon_rect.left = MMI_IDLE_TIME_MARGIN;
            sim_icon_rect.right= sim_icon_rect.left + MMI_IDLE_SIM_ICON_WIDTH;
            sim_icon_rect.bottom = sim_icon_rect.top + MMI_IDLE_SIM_ICON_HEIGHT;

#ifdef MMI_MULTI_SIM_SYS_SINGLE
            img_id = IMAGE_IDLE_SCREEN_SINGLE_SIM;
#else
            img_id = IMAGE_CUSTOM_SIM1;
#endif
            SetDisplayNum();
	 }
    }
#ifdef MMI_MULTI_SIM_SYS_DUAL
    else if(dual_sys == MN_DUAL_SYS_2)
    {
        if(idle_mode->visible_flag.item_flag.is_show_network_name2)
        {
            if(!idle_mode->visible_flag.item_flag.is_show_network_name)
            {
	         sim_icon_rect.top = MMI_IDLE_SIM_STATUS_TOP + MMI_IDLE_INTERVAL_MARGIN;
            }
            else
            {
                sim_icon_rect.top = MMI_IDLE_SIM_STATUS_TOP + MMI_IDLE_SIM_STATUS_HEIGHT + MMI_IDLE_INTERVAL_MARGIN*2;
            }
            sim_icon_rect.left = MMI_IDLE_TIME_MARGIN;
            sim_icon_rect.right= sim_icon_rect.left + MMI_IDLE_SIM_ICON_WIDTH;
            sim_icon_rect.bottom = sim_icon_rect.top + MMI_IDLE_SIM_ICON_HEIGHT;
            img_id = IMAGE_CUSTOM_SIM2;
            SetDisplayNum();
        }
    }        
#endif
    else
    {
        SCI_TRACE_LOW("[idle]: DisplaySimStatus para error, dual_sys = %d", dual_sys);
        return;
    }

    //string_display_rect.top = MMI_IDLE_SIM_STATUS_TOP + MMI_IDLE_SIM_STATUS_HEIGHT + MMI_IDLE_INTERVAL_MARGIN*2;
    //string_display_rect.left = MMI_IDLE_TIME_MARGIN;
    //string_display_rect.right= string_display_rect.left + MMI_IDLE_SIM_STATUS_WIDTH;
    //string_display_rect.bottom = string_display_rect.top + MMI_IDLE_SIM_STATUS_HEIGHT;

    //LCD_FillArgbRect(&lcd_dev_info, display_rect, MMITHEME_GetTransparenceColor(), (uint8)255*0.5);

    GUIRES_DisplayImg(PNULL,
                         &sim_icon_rect,
                         PNULL,
                         MAIN_IDLE_WIN_ID,
                         img_id,
                         &lcd_dev_info);
    

    text_style.region_num = 0;
    MMIIDLE_GetIdleTextStyle(&text_style, font, color);
    text_style.align = ALIGN_VMIDDLE;

    display_string.wstr_ptr = wstr_buf;

    if(MMIAPISET_GetFlyMode())
    {
        MMI_GetLabelTextByLang(TXT_SET_FLY_MODE, &display_string);
    } else {
        MMIAPIPHONE_GetSimAndServiceString(dual_sys, &display_string);
    }

    if(0 != display_string.wstr_len)
    {
        str_width = GUISTR_GetStringWidth(&text_style, &display_string, text_state);

        string_display_rect.top = sim_icon_rect.top + 2;
        string_display_rect.bottom = sim_icon_rect.bottom;

        string_display_rect.left = sim_icon_rect.right + MMI_IDLE_INTERVAL_MARGIN;
        string_display_rect.right = string_display_rect.left + str_width + MMI_IDLE_INTERVAL_MARGIN;
		string_display_rect.right = string_display_rect.right > MMITHEME_GetClientRectWidth() ? MMITHEME_GetClientRectWidth() : string_display_rect.right ;
        
        GUISTR_DrawTextToLCDInRect(
                &lcd_dev_info,
                (const GUI_RECT_T *)&string_display_rect,       //the fixed display area
                (const GUI_RECT_T *)&string_display_rect,       
                (const MMI_STRING_T *)&display_string,
                &text_style,
                text_state,
                GUISTR_TEXT_DIR_AUTO);

    }
}



/*****************************************************************************/
//  Description :在Idle screen上显示SIM卡状态信息
//  Global resource dependence :
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void OutputIdleSimStatusEx(void)
{

    DisplaySimStatus(MN_DUAL_SYS_1);

#ifdef MMI_MULTI_SIM_SYS_DUAL

    DisplaySimStatus(MN_DUAL_SYS_2);

#endif

}


LOCAL void  OutputIdleWinWeekAndDate(void)
{

    MMI_IDLE_DISPLAY_T          idle_disp_style     = {0};
    GUI_POINT_T                 point               = {0};
    uint8                       datestr[30]         = {0};
    wchar                       date_wstr[30]        = {0};
    wchar                       wstr_buf[30]        = {0};
    //wchar                       wstr_split[]        = {L", "};
    GUI_FONT_T                  font                = SONG_FONT_16;
    GUI_COLOR_T                 color               = 0;
    uint16                      str_width           = 0;
    MMI_STRING_T                week_string            = {0};
    MMI_STRING_T            display_string   = {0};
    GUI_LCD_DEV_INFO            lcd_dev_info        = *GetCommonIdlePaintLcdDev();
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    SCI_DATE_T date = {0};
    GUI_RECT_T display_rect = {0};
#if defined (MAINLCD_SIZE_128X160)
    MMI_TEXT_ID_T week_table[] =
    {
       TXT_SHORT_IDLE_SUNDAY,
       TXT_SHORT_IDLE_MONDAY,
       TXT_SHORT_IDLE_TUESDAY,
       TXT_SHORT_IDLE_WEDNESDAY,
       TXT_SHORT_IDLE_THURSDAY,
       TXT_SHORT_IDLE_FRIDAY,
       TXT_SHORT_IDLE_SATURDAY
    };
#else
    MMI_TEXT_ID_T week_table[] =
    {
        STXT_SUNDAY,//       TXT_SHORT_IDLE_SUNDAY,
        STXT_MONDAY,//       TXT_SHORT_IDLE_MONDAY,
        STXT_TUESDAY,//      TXT_SHORT_IDLE_TUESDAY,
        STXT_WEDNESDAY,//       TXT_SHORT_IDLE_WEDNESDAY,
        STXT_THURSDAY,//       TXT_SHORT_IDLE_THURSDAY,
        STXT_FRIDAY,//       TXT_SHORT_IDLE_FRIDAY,
        STXT_SATURDAY,//       TXT_SHORT_IDLE_SATURDAY
    };
#endif
    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_DATE);
    if(MMITHEME_IsMainScreenLandscape())
    {
#if defined (MAINLCD_SIZE_240X320) || defined (MAINLCD_SIZE_128X160)
        idle_disp_style.text_frame_top = IDLE_DATE_TOP_H;
        idle_disp_style.text_frame_x = MMITHEME_GetClientRectWidth()-IDLE_DATE_LEFT_MARGIN;
#elif defined MAINLCD_SIZE_176X220 
        idle_disp_style.text_frame_top = IDLE_DATE_TOP_H;
        idle_disp_style.text_frame_x = IDLE_DATE_LEFT_H;
#else
        //idle_disp_style.text_frame_x = IDLE_DATE_LEFT_H ;
#endif
    }
    font = idle_disp_style.text_font;
    color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_DATE);

    text_style.region_num = 0;
    MMIIDLE_GetIdleTextStyle(&text_style, font, color);
    text_style.align = ALIGN_VMIDDLE;

    TM_GetSysDate(&date);
    MMI_GetLabelTextByLang(week_table[date.wday], &week_string);
    MMIAPISET_GetCurrentDateStr(datestr, '-', sizeof(datestr));
    MMIAPICOM_StrToWstr(datestr, date_wstr);

    display_string.wstr_ptr = wstr_buf;

    //MMIAPICOM_Wstrncpy(display_string.wstr_ptr, week_string.wstr_ptr, week_string.wstr_len);
    //MMIAPICOM_Wstrncat(display_string.wstr_ptr, wstr_split, MMIAPICOM_Wstrlen(wstr_split));
    MMIAPICOM_Wstrncat(display_string.wstr_ptr, date_wstr, MMIAPICOM_Wstrlen(date_wstr));

    display_string.wstr_len = MMIAPICOM_Wstrlen(display_string.wstr_ptr);

    str_width = GUISTR_GetStringWidth(&text_style, &display_string, text_state);

    point = CalStartPoint(idle_disp_style, str_width);

#if defined (MAINLCD_SIZE_240X320)
    point.y = MMI_IDLE_TIME_TOP;
#endif

    display_rect.top = point.y;
    display_rect.left = point.x;
    display_rect.right= point.x + str_width;
    display_rect.bottom = point.y + MMI_IDLE_DATE_HEIGHT;

    GUISTR_DrawTextToLCDInRect(
        &lcd_dev_info,
        (const GUI_RECT_T *)&display_rect,       //the fixed display area
        (const GUI_RECT_T *)&display_rect,       
        (const MMI_STRING_T *)&display_string,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO);

	idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_WEEK);
        if(MMITHEME_IsMainScreenLandscape())
        {
#if defined MAINLCD_SIZE_240X320 
            idle_disp_style.text_frame_top = IDLE_WEEK_TOP_H;
            idle_disp_style.text_frame_x = MMITHEME_GetClientRectWidth() - 6;
#elif defined MAINLCD_SIZE_176X220  
            idle_disp_style.text_frame_top = IDLE_WEEK_TOP_H;
            idle_disp_style.text_frame_x = IDLE_WEEK_LEFT_H;
#else
            //idle_disp_style.text_frame_x = IDLE_WEEK_LEFT_H;
#endif
        }
	font = idle_disp_style.text_font;
	color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_WEEK);

	text_style.region_num = 0;
	MMIIDLE_GetIdleTextStyle(&text_style, font, color);
#if defined (MAINLCD_SIZE_240X320)
	text_style.align = ALIGN_RIGHT;
#else
	text_style.align = ALIGN_VMIDDLE;
#endif

	str_width = GUISTR_GetStringWidth(&text_style, &week_string, text_state);

	point = CalStartPoint(idle_disp_style, str_width);

#if defined (MAINLCD_SIZE_240X320)
	display_rect.top = point.y - 4;
#endif
	display_rect.left = point.x;

	display_rect.right= point.x + str_width;
	display_rect.bottom = point.y + MMI_IDLE_DATE_HEIGHT;

	GUISTR_DrawTextToLCDInRect(
		&lcd_dev_info,
		(const GUI_RECT_T *)&display_rect,		 //the fixed display area
		(const GUI_RECT_T *)&display_rect,		 
		(const MMI_STRING_T *)&week_string,
		&text_style,
		text_state,
		GUISTR_TEXT_DIR_AUTO);

    
}

/*****************************************************************************/
//  Description : output counted time
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinCountedTime(BOOLEAN is_highlight)
{
#ifdef COUNTEDTIME_SUPPORT
    MMI_IDLE_DISPLAY_T          idle_disp_style = {0};
    GUI_POINT_T                 point           = {0};
    SCI_TIME_T                  time            = {0};
    uint8                       timestr[20]     = {0};
    //wchar                         disptime[20] = {0};
    uint8                       disptime_len    = 0;
    MMI_STRING_T                tmp_prompt      = {0};
    MMI_STRING_T                disp_str        = {0};

    GUI_FONT_T                  font            = SONG_FONT_8;
    GUI_COLOR_T                 color           = 0;
    uint16                      str_width       = 0;
    GUI_LCD_DEV_INFO            lcd_dev_info    = *GetCommonIdlePaintLcdDev();
    GUISTR_STYLE_T              text_style = {0};/*lint !e64*/
    GUISTR_STATE_T              text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
//@for light function modify
#if defined MAINLCD_SIZE_240X320
    uint16                      icon_width = 0;
#endif

    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());
    font = idle_disp_style.text_font;
    color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_COUNTTIME);

    //"倒计时"
    MMI_GetLabelTextByLang(TXT_TOOLS_COUNTEDTIME, &tmp_prompt);

    //XX:XX
    time = MMIAPICT_GetCurTime();
//     if (MMIAPICT_IsLessThanOnemin())
//     {
//         sprintf((char*)timestr,": %02d:%02d", time.min, time.sec);//小于1分钟时显示"分分:秒秒"      
//     }
//     else
//     {
//         sprintf((char*)timestr,": %02d:%02d", time.hour, time.min);//大于等于1分钟时显示"时时:分分"
//     }
    sprintf((char*)timestr,": %02d:%02d:%02d", time.hour,time.min, time.sec);
    disptime_len = strlen((char *)timestr); 

    // the total string length
    disp_str.wstr_len =  disptime_len + tmp_prompt.wstr_len ;

    // allocate memory
    disp_str.wstr_ptr = SCI_ALLOC_APP((disp_str.wstr_len + 1) * sizeof(wchar) );
    
    if (PNULL == disp_str.wstr_ptr)
    {
        //SCI_TRACE_LOW:"MMIIDLE OutputIdleWinCountedTime PNULL == disp_str.wstr_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_CSTYLE_988_112_2_18_2_23_42_21,(uint8*)"");
        return;
    }
    //disp_str.is_ucs2 = tmp_prompt.is_ucs2;
    
    //copy the string
    MMIAPICOM_Wstrncpy(
        disp_str.wstr_ptr, 
        tmp_prompt.wstr_ptr,  
        tmp_prompt.wstr_len
                );

    // copy the num
    MMI_STRNTOWSTR(
                &disp_str.wstr_ptr[tmp_prompt.wstr_len], 
                disptime_len,
                timestr,  
                disptime_len,
                disptime_len
                );

    if (is_highlight)
    {
        MMIIDLE_GetIdleTextStyle(&text_style, font, MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_COUNTTIME));
    }
    else
    {
        MMIIDLE_GetIdleTextStyle(&text_style, font, color);
    }

    str_width = GUISTR_GetStringWidth(&text_style,&disp_str,text_state);
    
    point = CalStartPoint(idle_disp_style, str_width);
    point.x = point.x -5;

//@for light function modify
#if defined MAINLCD_SIZE_240X320 
   //display icon
    GUIRES_DisplayImg(&point,
      PNULL,
      PNULL,
      MMK_GetFirstAppletHandle(),
      IMAGE_IDLE_PROMPT_COUNT_DOWN,
      GetCommonIdlePaintLcdDev());
    GUIRES_GetImgWidthHeight(&icon_width,PNULL, IMAGE_IDLE_PROMPT_COUNT_DOWN,MMK_GetFirstAppletHandle());
    point.x = point.x + icon_width + 5;
#endif
#if defined TOUCH_PANEL_SUPPORT
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].type        = MMI_MAIN_LCD_COUNTEDTIME;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.left   = point.x;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.top    = point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.right  = point.x+str_width;
	s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.bottom = point.y+GUI_GetFontHeight(font,UNICODE_HANZI);
#endif

#if 0
    if (is_highlight)
    {
        GUISTR_DrawTextToLCDSingleLine( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_POINT_T      *)&point,
            (const MMI_STRING_T     *)&disp_str,
            &text_style,
            text_state
            );
    }
    else
    {
        GUISTR_DrawTextToLCDSingleLine( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_POINT_T      *)&point,
            (const MMI_STRING_T     *)&disp_str,
            &text_style,
            text_state
            );
    }
#endif

    GUISTR_DrawTextToLCDSingleLine( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_POINT_T      *)&point,
            (const MMI_STRING_T     *)&disp_str,
            &text_style,
            text_state
            );

    if (PNULL != disp_str.wstr_ptr)
    {
        SCI_FREE(disp_str.wstr_ptr);
        disp_str.wstr_ptr = PNULL;
    }
	SetDisplayNum();
#endif
}


#ifdef CALENDAR_SUPPORT
/*****************************************************************************/
//  Description : output calendar
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinCalendar(void)
{
    MMI_IDLE_DISPLAY_T			idle_disp_style = {0};   
    GUI_POINT_T		            point           = {0};
    GUI_FONT_T                  font            = SONG_FONT_8;
    uint16                      str_width       = 0;
	GUI_RECT_T                  mainlcd_rect = MMITHEME_GetFullScreenRect();
//@for light function modify
#if defined TOUCH_PANEL_SUPPORT || defined MAINLCD_SIZE_240X320
    uint16                      icon_width = 0;/*lint !e529*/
#endif
    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());
    font = idle_disp_style.text_font; 
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
    point.x = idle_disp_style.text_frame_x - 4;
    point.y = idle_disp_style.text_frame_top + 2;

    GUIRES_DisplayImg(&point,
        PNULL,
        PNULL,
        MMK_GetFirstAppletHandle(),
        IMAGE_IDLE_PROMPT_SCHEDULE,
        GetCommonIdlePaintLcdDev());

    GUIRES_GetImgWidthHeight(&icon_width,PNULL, IMAGE_IDLE_PROMPT_SCHEDULE,MMK_GetFirstAppletHandle());
    point.x = idle_disp_style.text_frame_x + icon_width;
    point.y = idle_disp_style.text_frame_top;
   
    str_width = MMIIDLE_DisplaySchedule(GetCommonIdlePaintLcdDev(),MMI_DIRECTION_LEFT,mainlcd_rect.right,&point,font);
#else
    point.x = idle_disp_style.text_frame_x - 1;
    point.y = idle_disp_style.text_frame_top;

    str_width = MMIIDLE_DisplaySchedule(GetCommonIdlePaintLcdDev(),MMI_DIRECTION_HMIDDLE,mainlcd_rect.right,&point,font);
#endif

#if defined TOUCH_PANEL_SUPPORT
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].type        = MMI_MAIN_LCD_CALENDAR;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].rect.left   = idle_disp_style.text_frame_x+ icon_width+ 7;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].rect.top    = point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].rect.right  = point.x+str_width;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].rect.bottom = point.y+GUI_GetFontHeight(font,UNICODE_HANZI);
#endif
    SetDisplayNum();
}
#endif

#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)
/*****************************************************************************/
//  Description : display the mp3 name or not
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
LOCAL void DisplayIdleMp3Name(void)
{
//    IDLE_WIN_INFO_T       *idlewin_info_ptr = MMIIDLE_GetIdleWinInfo();
    MMISET_IDLE_DISPLAY_T   *idle_set_ptr  =   PNULL;
    idle_set_ptr = MMIAPISET_GetIdleDisplayMode();
//    if (idlewin_info_ptr->is_display_mp3)//显示mp3 name
//    {
        //display mp3 name
        if (g_idle_cur_mp3_name.is_scroll)
        {
            MMIIDLECOM_StartIdleMp3Timer();
            DisplayIdleCycleMp3Name();
        }
        else
        {
            MMI_IDLE_DISPLAY_T  idle_disp_style = {0};
            GUI_FONT_T          font            = SONG_FONT_8;
            GUI_COLOR_T         color           = 0;
            GUI_POINT_T         point           = {0};
            GUI_LCD_DEV_INFO    lcd_dev_info    = *GetCommonIdlePaintLcdDev();
            uint8               string_width    =0;
            GUISTR_STYLE_T      text_style = {0};/*lint !e64*/
            GUISTR_STATE_T      text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS_EX;
            MMI_STRING_T        text = {0};
            uint16              lcd_width = 0; // lcd宽度
            uint16              lcd_height = 0; // lcd高度
            uint16              music_name_max_width = 0; //MMI_MAINSCREEN_MP3_WIDTH;
            wchar               mp3_name[MMIMP3_NAME_MAX_LEN + 1]={0};
//@for light function modify
#if defined MAINLCD_SIZE_240X320
            MMI_IMAGE_ID_T      icon_id  = IMAGE_IDLE_PROMPT_PLAY;/*lint !e529*/
            uint16              icon_width = 0;/*lint !e529*/
#endif
            text.wstr_ptr = mp3_name; 
			idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());

            if (MMIAPIMP3_IsPlayerActive()
				&& idle_set_ptr->visible_flag.item_flag.is_show_mp3&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
            {
                #ifdef FM_SUPPORT
                if ((FM_PLAYING == MMIAPIFM_GetStatus())
					&& idle_set_ptr->visible_flag.item_flag.is_show_fm&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
                {
                    //display fm
                    MMIAPIFM_GetCurFreqName(&text, MMIMP3_NAME_MAX_LEN);
                }
                else
                #endif
                {
                    MMIAPIMP3_GetCurMp3Name(&text, MMIMP3_NAME_MAX_LEN);
                }
                 SetDisplayNum();   
            }  
            else
            { 
#ifdef FM_SUPPORT
                if ((FM_STOP != MMIAPIFM_GetStatus())
					&& idle_set_ptr->visible_flag.item_flag.is_show_fm&& (GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
                {
                    //display fm
                    MMIAPIFM_GetCurFreqName(&text, MMIMP3_NAME_MAX_LEN);
                    SetDisplayNum();
                }
				
#endif
            }
            GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
            music_name_max_width = (lcd_width-MMI_MAINSCREEN_MP3_LEFT-MMI_MAINSCREEN_MP3_LEFT);
           
            font = idle_disp_style.text_font;
            color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_MP3);
            //text.wstr_len = g_idle_cur_mp3_name.length;
            //text.wstr_ptr = g_idle_cur_mp3_name.mp3_name;
            g_idle_cur_mp3_name.str_width = GUI_CalculateStringPiexlNum(text.wstr_ptr,
                text.wstr_len,
                                                                    font,0);
            MMIIDLE_GetIdleTextStyle(&text_style, font, color);
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
        if(idle_set_ptr->visible_flag.item_flag.is_show_mp3 && (MMIMP3_PLAYER_STATE_NONE!=MMIAPIMP3_GetPlayerState()) 
#ifdef FM_SUPPORT
        ||(idle_set_ptr->visible_flag.item_flag.is_show_fm && (FM_STOP != MMIAPIFM_GetStatus()))
#endif
          )
        {
            point.x = idle_disp_style.text_frame_x -1;
            point.y = idle_disp_style.text_frame_top;
            if(MMIAPIMP3_IsPlayerPlaying() || (FM_PLAYING == MMIAPIFM_GetStatus()))
            {
                icon_id = IMAGE_IDLE_PROMPT_PLAY;
            }
            else if(MMIAPIMP3_IsPlayerActive() || (FM_SUSPENDED == MMIAPIFM_GetStatus()))
            {
                icon_id = IMAGE_IDLE_PROMPT_PAUSE;
            }
            //display icon
            GUIRES_DisplayImg(&point,
                PNULL,
                PNULL,
                MMK_GetFirstAppletHandle(),
                icon_id,
                GetCommonIdlePaintLcdDev());
            GUIRES_GetImgWidthHeight(&icon_width,PNULL, IMAGE_IDLE_PROMPT_SCHEDULE,MMK_GetFirstAppletHandle());
            point.x = point.x + icon_width + 2;
        }
#else
            point.x = ((music_name_max_width - g_idle_cur_mp3_name.str_width)>>1)+MMI_MAINSCREEN_MP3_LEFT ;    
            point.y = idle_disp_style.text_frame_top;
#endif
#if defined TOUCH_PANEL_SUPPORT            
            s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].type = MMI_MAIN_LCD_MP3;
            
            s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.left = point.x;
#endif
            string_width = GUISTR_GetStringWidth(&text_style,&text,text_state);
#if defined TOUCH_PANEL_SUPPORT
            s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.right  = point.x + string_width;
            s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.top    = point.y;
            s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.bottom = point.y + GUI_GetFontHeight(font,UNICODE_HANZI);
#endif
            GUISTR_DrawTextToLCDSingleLine( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_POINT_T      *)&point,
                (const MMI_STRING_T     *)&text,
                &text_style,
                text_state
                );
			
        }

//    }
}

/*****************************************************************************/
//  Description : display the cycle mp3 name
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
LOCAL void DisplayIdleCycleMp3Name(void)
{
    uint16              str_index           = 0;
    MMI_IDLE_DISPLAY_T  idle_disp_style     = {0};
    GUI_RECT_T          rect                = {0};
    GUI_FONT_T          font                = SONG_FONT_8;
    GUI_COLOR_T         color               = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info        = *GetCommonIdlePaintLcdDev();
    GUISTR_STYLE_T      text_style          = {0};/*lint !e64*/
//#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT)	
 //   GUISTR_STATE_T      text_state          = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE |GUISTR_STATE_EFFECT;
//#else
    GUISTR_STATE_T      text_state          = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT;
//#endif		
    MMI_STRING_T        text                = {0};
	GUI_RECT_T                  mainlcd_rect = MMITHEME_GetFullScreenRect();
    wchar               mp3_name[MMIMP3_NAME_MAX_LEN + 1]={0};
    MMISET_IDLE_DISPLAY_T   *idle_set_ptr  =   PNULL;
    idle_set_ptr = MMIAPISET_GetIdleDisplayMode();
    text.wstr_ptr = mp3_name;
	
	MMIIDLE_GetIdleTextStyle(&text_style, font, color);	
#ifdef FM_SUPPORT    
	if (FM_PLAYING == MMIAPIFM_GetStatus() && idle_set_ptr->visible_flag.item_flag.is_show_fm&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
	{
		MMIAPIFM_GetCurFreqName(&text, MMIMP3_NAME_MAX_LEN);
		idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());
		font = idle_disp_style.text_font;
		color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_FM);
             SetDisplayNum();
	}
	else 
#endif     
    if(MMIAPIMP3_IsPlayerActive() && idle_set_ptr->visible_flag.item_flag.is_show_mp3&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
    {
		MMIAPIMP3_GetCurMp3Name(&text, MMIMP3_NAME_MAX_LEN);
		idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());
		font = idle_disp_style.text_font;
		color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_MP3);
        SetDisplayNum();
    }

    //显示mp3 name,防止定时器到时之前，停止mp3播放，Mp3名字长度为0，导致出错!
    if (MMIIDLE_GetIdleWinInfo()->is_display_mp3)
    {
        //set rect
        rect.left = MMI_MAINSCREEN_MP3_LEFT;
        rect.right = mainlcd_rect.right - MMI_MAINSCREEN_MP3_LEFT;
        rect.top = idle_disp_style.text_frame_top;
        rect.bottom = rect.top + GUI_GetStringHeight(font, text.wstr_ptr, 
                            text.wstr_len);
#if defined TOUCH_PANEL_SUPPORT
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].type     = MMI_MAIN_LCD_MP3;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.left=rect.left;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.right=rect.right;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.top=rect.top;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.bottom=rect.bottom;
#endif


        str_index = g_idle_cur_mp3_name.str_index;

        if (str_index < MMIMP3_NAME_MAX_LEN)
        {
            text.wstr_len = (uint16)(text.wstr_len-str_index);
            text.wstr_ptr = text.wstr_ptr+str_index;

            text_style.font = font ;

            GUISTR_DrawTextToLCDInRect( 
                                       (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                       (const GUI_RECT_T      *)&rect,       //the fixed display area
                                       (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                                       (const MMI_STRING_T     *)&text,
                                       &text_style,
                                       text_state,
                                       GUISTR_TEXT_DIR_AUTO
                                       );
        }
    }
      // SetDisplayNum();
}
#endif

#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
/*****************************************************************************/
//  Description : output idle win words
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinWords(void)
{
    MMI_IDLE_DISPLAY_T      idle_disp_style = {0};
    GUI_POINT_T             point           = {0};
    GUI_FONT_T              font            = SONG_FONT_8;
    GUI_COLOR_T             color           = 0;
    uint16                  str_width       = 0;
    MMISET_IDLE_DISPLAY_T   *idle_mode      = PNULL;
    GUI_LCD_DEV_INFO        lcd_dev_info    = *GetCommonIdlePaintLcdDev();
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_EFFECT;
    MMI_STRING_T            text   = {0};
    MMI_STRING_T            idle_words   = {0};
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
    uint16                  icon_width = 0;/*lint !e529*/
#endif
    
    idle_mode = MMIAPISET_GetIdleDisplayMode();

    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());
    font = idle_disp_style.text_font;
    color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_IDLEWORD);    

    MMIIDLE_GetIdleTextStyle(&text_style, font, color);
    
    //个性词语内容为空时显示默认个性词语
    if(idle_mode->wstr_len == 0)
    {
		MMI_GetLabelTextByLang(TXT_SET_CUSTOM_WORD, &idle_words);			

		//默认内容的长度超过idle_mode->idle_words的最大长度
        text.wstr_ptr = idle_words.wstr_ptr;
        text.wstr_len = idle_words.wstr_len;
    }
    else
    {
        text.wstr_len = idle_mode->wstr_len;
        text.wstr_ptr = idle_mode->idle_words;
    }

    str_width = GUISTR_GetStringWidth(&text_style,&text,text_state);
	
    point = CalStartPoint(idle_disp_style, str_width);
    point.x = point.x -4;
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
   //display icon
    GUIRES_DisplayImg(&point,
      PNULL,
      PNULL,
      MMK_GetFirstAppletHandle(),
      IMAGE_IDLE_PROMPT_WORDS,
      GetCommonIdlePaintLcdDev());
    GUIRES_GetImgWidthHeight(&icon_width,PNULL, IMAGE_IDLE_PROMPT_WORDS,MMK_GetFirstAppletHandle());
    point.x = point.x + icon_width + 5;
#endif
#if defined TOUCH_PANEL_SUPPORT
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].type = MMI_MAIN_LCD_CUSTOM_WORD;    
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.left=point.x;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.top=point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.right=point.x+str_width;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.bottom=point.y+GUI_GetFontHeight(font,UNICODE_HANZI);
#endif    
    GUISTR_DrawTextToLCDSingleLine( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&point,
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state
        );
	SetDisplayNum();
}
#endif

LOCAL void ResetDisplayNum(void)
{
	s_display_num = 0;

}

LOCAL uint16 GetDisplayNum(void)
{
    return s_display_num;
}

LOCAL void SetDisplayNum(void)
{
	if (s_display_num<=MMI_COMMON_IDLE_ITEM_MAX_NUM)
	{
		s_display_num++;
	}
}

/*****************************************************************************/
//  Description : Display the progress according to dynamic playing info..
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MusicDisplayProgress(void)
{
    GUI_LCD_DEV_INFO        lcd_dev_info             = *GetCommonIdleNotiLcdDev();
    GUI_RECT_T          bg_rect = {0};
    GUI_RECT_T          progress_rect = {0};
    MMI_CTRL_ID_T       active_ctrl_id = MMK_GetActiveCtrlId(MAIN_IDLE_WIN_ID);
    MMI_CTRL_ID_T       ctrl_id = MMIIDLE_MUSIC_PROGRESSBAR_CTRL_ID;
    GUI_COLOR_T         progress_color = MMI_WHITE_COLOR;
    GUI_COLOR_T         bg_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_1);


    if(MMIAPIMP3_IsPlayerActive()
           &&MMIAPIMP3_IsPlayerPlaying()
      )
    {
        MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};
        MMIAPIMP3_GetPlayingDynInfo(&dyn_info, FALSE);

        if(MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID == active_ctrl_id 
            ||MMIIDLE_MUSIC_FORM_CTRL_ID == active_ctrl_id)
        {
            bg_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_1);
            progress_color = MMI_WHITE_COLOR;
        }
        else
        {
            bg_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_3);
            progress_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT);
        }
        //draw bg
        GUIOWNDRAW_GetDisplayRect(ctrl_id, &bg_rect);
        GUI_FillRect((const GUI_LCD_DEV_INFO *) &lcd_dev_info, bg_rect, bg_color);

        progress_rect = bg_rect;
        if(dyn_info.total_length != 0)
        {
            progress_rect.right = progress_rect.left
                                  + (uint32)((progress_rect.right - progress_rect.left) * ((uint64_t)dyn_info.cur_offset) / dyn_info.total_length); /*lint !e737*/
        }
        else
        {
            progress_rect.right = progress_rect.left;
        }
        GUI_FillRect((const GUI_LCD_DEV_INFO *) &lcd_dev_info, progress_rect, progress_color);
    }
}

/*****************************************************************************/
//  Description : Display the timeinfo according to dynamic playing info..
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MusicDisplayTimeInfo(void)
{
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T    cur_str_t   = {0};
    MMI_STRING_T    total_str_t = {0};
    wchar        cur_time_wstr[32]   = {0};
    wchar        total_time_wstr[32] = {0};
    char         cur_time_str[32] = {0};
    char         total_time_str[32] = {0};
    GUI_LCD_DEV_INFO        lcd_dev_info    = *GetCommonIdleNotiLcdDev();
    GUI_RECT_T parent_display_rect = {0};
    GUI_RECT_T curtime_display_rect = {0};
    GUI_RECT_T totaltime_display_rect = {0};
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};

    GUIAPICTRL_GetDisplayRect(MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID, &parent_display_rect);

    text_style.font = MMI_DEFAULT_NORMAL_FONT;
    text_style.font_color = MMI_WHITE_COLOR;

    if (MMIAPIMP3_IsPlayerActive()
           &&MMIAPIMP3_IsPlayerPlaying()
       )
    {
        MMIAPIMP3_GetPlayingDynInfo(&dyn_info, FALSE);
        curtime_display_rect.left = parent_display_rect.left;
        curtime_display_rect.right = parent_display_rect.left + FORM_CHILD_MUSIC_TIMEINFO_WIDTH;
        totaltime_display_rect.left = parent_display_rect.right - FORM_CHILD_MUSIC_TIMEINFO_WIDTH;
        totaltime_display_rect.right = totaltime_display_rect.left + FORM_CHILD_MUSIC_TIMEINFO_WIDTH;
        if(dyn_info.total_time / 60 < 10)//0:00
        {
            cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.1ld:%.2ld", dyn_info.cur_time / 60, dyn_info.cur_time % 60);
            total_str_t.wstr_len = sprintf((char*)total_time_str, "%.1ld:%.2ld", dyn_info.total_time / 60, dyn_info.total_time % 60);
        }
        else if(dyn_info.total_time/60 < 60)//00:00
        {
            cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld", dyn_info.cur_time/60, dyn_info.cur_time%60);
            total_str_t.wstr_len = sprintf((char*)total_time_str, "%.2ld:%.2ld", dyn_info.total_time/60, dyn_info.total_time%60);
        }
        else if(dyn_info.total_time / 60 < 60*10)//0:00:00
        {
            cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.1ld:%.2ld:%.2ld", (dyn_info.cur_time / 60) / 60,
                                         (dyn_info.cur_time / 60) % 60,
                                         (dyn_info.cur_time % 60));
            total_str_t.wstr_len = sprintf((char*)total_time_str, "%.1ld:%.2ld:%.2ld", (dyn_info.total_time / 60) / 60,
                                           (dyn_info.total_time / 60) % 60,
                                           dyn_info.total_time % 60);
            curtime_display_rect.right = parent_display_rect.left + FORM_CHILD_MUSIC_TIMEINFO_WIDTH + 12;
            totaltime_display_rect.left = parent_display_rect.right - FORM_CHILD_MUSIC_TIMEINFO_WIDTH -12;
            totaltime_display_rect.right = totaltime_display_rect.left + FORM_CHILD_MUSIC_TIMEINFO_WIDTH+12;
        }
        else//00:00:00
        {
            cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld:%.2ld", (dyn_info.cur_time/60)/60, 
                (dyn_info.cur_time/60)%60, 
                (dyn_info.cur_time%60));
            total_str_t.wstr_len = sprintf((char*)total_time_str, "%.2ld:%.2ld:%.2ld", (dyn_info.total_time/60)/60,
                (dyn_info.total_time/60)%60, 
                dyn_info.total_time%60);
            curtime_display_rect.right = parent_display_rect.left + FORM_CHILD_MUSIC_TIMEINFO_WIDTH + 20;
            totaltime_display_rect.left = parent_display_rect.right - FORM_CHILD_MUSIC_TIMEINFO_WIDTH -20;
            totaltime_display_rect.right = totaltime_display_rect.left + FORM_CHILD_MUSIC_TIMEINFO_WIDTH+20;
        }

        cur_str_t.wstr_ptr = cur_time_wstr;
        total_str_t.wstr_ptr = total_time_wstr;
        MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8 *)cur_time_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
        MMI_STRNTOWSTR(total_str_t.wstr_ptr, total_str_t.wstr_len, (uint8 *)total_time_str, total_str_t.wstr_len, total_str_t.wstr_len);

        //cur time info
        curtime_display_rect.top = parent_display_rect.top;
        curtime_display_rect.bottom = curtime_display_rect.top + FORM_CHILD_MUSIC_TIMEINFO_HEIGHT;

        if (curtime_display_rect.bottom > parent_display_rect.bottom)
            curtime_display_rect.bottom = parent_display_rect.bottom;

        //curtime_display_rect.left = parent_display_rect.left;
        //curtime_display_rect.right = curtime_display_rect.left + FORM_CHILD_MUSIC_TIMEINFO_WIDTH;
        text_style.align = ALIGN_LVMIDDLE;
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &curtime_display_rect,
            &curtime_display_rect,
            &cur_str_t,
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
        );

        //total time info
        totaltime_display_rect.top = parent_display_rect.top;
        totaltime_display_rect.bottom = totaltime_display_rect.top + FORM_CHILD_MUSIC_TIMEINFO_HEIGHT;

        if (totaltime_display_rect.bottom > parent_display_rect.bottom)
            totaltime_display_rect.bottom = parent_display_rect.bottom;

        //totaltime_display_rect.left = parent_display_rect.right - FORM_CHILD_MUSIC_TIMEINFO_WIDTH;
        //totaltime_display_rect.right = totaltime_display_rect.left + FORM_CHILD_MUSIC_TIMEINFO_WIDTH;
        text_style.align = ALIGN_RVMIDDLE;
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &totaltime_display_rect,
            &totaltime_display_rect,
            &total_str_t,
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
        );
    }
}

/*****************************************************************************/
//  Description : Display Button
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MusicDisplayButton(void)
{
    GUI_LCD_DEV_INFO        lcd_dev_info    = *GetCommonIdleNotiLcdDev();
    GUI_RECT_T              parent_display_rect = {0};
    GUI_RECT_T              playbtn_display_rect = {0};
    GUI_RECT_T              nextbtn_display_rect = {0};
    MMI_IMAGE_ID_T         img_id = IMAGE_NULL;
    GUIAPICTRL_GetDisplayRect(MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID, &parent_display_rect);

    if(MMIAPIMP3_IsPlayerPlaying())
    {
        img_id = IMAGE_IDLE_SCREEN_PLAYER_PAUSE;
    }
    else //if(MMIAPIMP3_IsPlayerPaused())
    {
        img_id = IMAGE_IDLE_SCREEN_PLAYER_PLAY;
    }
    playbtn_display_rect.top = parent_display_rect.top + FORM_CHILD_MUSIC_BUTTON_TOP_SPACE;
    playbtn_display_rect.bottom = playbtn_display_rect.top + FORM_CHILD_MUSIC_BUTTON_HEIGHT;
    playbtn_display_rect.left = parent_display_rect.left + FORM_CHILD_MUSIC_BUTTON_PLAY_LEFT_SPACE;
    playbtn_display_rect.right = playbtn_display_rect.left + FORM_CHILD_MUSIC_BUTTON_WIDTH;

    GUIRES_DisplayImg(PNULL,
                      &playbtn_display_rect,
                      PNULL,
                      MAIN_IDLE_WIN_ID,
                      img_id,
                      &lcd_dev_info);

    //set next anim
    nextbtn_display_rect.top = parent_display_rect.top + FORM_CHILD_MUSIC_BUTTON_TOP_SPACE;
    nextbtn_display_rect.bottom = nextbtn_display_rect.top + FORM_CHILD_MUSIC_BUTTON_HEIGHT;
    nextbtn_display_rect.left = parent_display_rect.left + FORM_CHILD_MUSIC_BUTTON_NEXT_LEFT_SPACE;
    nextbtn_display_rect.right = nextbtn_display_rect.left + FORM_CHILD_MUSIC_BUTTON_WIDTH;

    GUIRES_DisplayImg(PNULL,
                      &nextbtn_display_rect,
                      PNULL,
                      MAIN_IDLE_WIN_ID,
                      IMAGE_IDLE_SCREEN_PLAYER_NEXT,
                      &lcd_dev_info);
}

/*****************************************************************************/
//  Description : DrawProgressOwnerDrawCtrl
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UpdateIdleMusicNotiProgressOwnerDrawCtrl(
    GUIOWNDRAW_INFO_T *owner_draw_ptr
)
{
    MusicDisplayProgress();
}

/*****************************************************************************/
//  Description : DrawtimeinfoOwnerDrawCtrl
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UpdateIdleMusicNotitimeinfoOwnerDrawCtrl(
    GUIOWNDRAW_INFO_T *owner_draw_ptr
)
{
    MusicDisplayTimeInfo();
}

/*****************************************************************************/
//  Description : Draw play next button OwnerDrawCtrl
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UpdateIdleMusicNotiPlayNextbtnOwnerDrawCtrl(
    GUIOWNDRAW_INFO_T *owner_draw_ptr
)
{
    MusicDisplayButton();
}

/*****************************************************************************/
//  Description : display the mp3 name or not
//  Global resource dependence :
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void UpdateIdleFmRadioNotification(void)
{
    MMIFM_CUR_INFO_T *cur_inof_ptr = PNULL;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    MMI_STRING_T        freq_string = {0};
    wchar               wstr_old[]        = {L"%S"};
    wchar               fm_name[MMIFM_CHANNEL_NAME_LEN + MMIFM_FREQ_MAX_LEN + 1] = {0};
    char                  freq_num_str[MMIFM_FREQ_MAX_LEN + 1] = {0};
    wchar               freq_num_wstr[MMIFM_FREQ_MAX_LEN + 1] = {0};
    MMI_STRING_T            label_str = {0};
    GUI_FONT_T      font_type = MMI_DEFAULT_NORMAL_FONT;    /*!<字体风格*/
    GUI_COLOR_T     font_color = MMI_WHITE_COLOR;   /*!<字体颜色*/

    GUIFORM_PermitChildFont(MMIIDLE_FM_FORM_CTRL_ID, FALSE);
    GUILABEL_SetFont(MMIIDLE_FM_FREQ_LABLE_CTRL_ID, font_type, font_color);

#ifdef FM_SUPPORT
    cur_inof_ptr = MMIFM_GetFMCurrentInfo();
#endif
    if (PNULL != cur_inof_ptr)
    {
        MMIAPIFM_ConvertFreqToString(cur_inof_ptr->cur_freq, (char*) freq_num_str);
    }

    MMIAPICOM_StrToWstr(freq_num_str, freq_num_wstr);

    MMI_GetLabelTextByLang(STR_RADIO_FM_FREQ_INDICATOR, &freq_string);//"%S MHz"

    MMIAPICOM_Wstrncpy(fm_name, freq_string.wstr_ptr, freq_string.wstr_len);
    MMIAPICOM_WstrReplace(fm_name, sizeof(fm_name), wstr_old, freq_num_wstr);
    
    label_str.wstr_ptr = (wchar *)fm_name;
    label_str.wstr_len = (uint16)MMIAPICOM_Wstrlen(fm_name);
    GUILABEL_SetText(MMIIDLE_FM_FREQ_LABLE_CTRL_ID, &label_str, FALSE);

    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_ptr = PNULL;

    //set play/pause anim
    control_info.ctrl_id = MMIIDLE_FM_PALY_ANIM_CTRL_ID;
    if(FM_PLAYING == MMIAPIFM_GetStatus())
    {
        data_info.img_id = IMAGE_COMMON_STOP;//IMAGE_IDLE_SCREEN_PLAYER_PAUSE;
    }
    else if(FM_SUSPENDED == MMIAPIFM_GetStatus())
    {
        data_info.img_id = IMAGE_IDLE_SCREEN_PLAYER_PLAY;
    }
    display_info.align_style = GUIANIM_ALIGN_HMIDDLE_BOTTOM;
    GUIANIM_SetParam(&control_info, &data_info, PNULL, &display_info);

    //set next anim
    control_info.ctrl_id = MMIIDLE_FM_NEXT_ANIM_CTRL_ID;
    data_info.img_id = IMAGE_IDLE_SCREEN_PLAYER_NEXT;
    display_info.align_style = GUIANIM_ALIGN_RIGHT_BOTTOM;
    GUIANIM_SetParam(&control_info, &data_info, PNULL, &display_info);

    //GUIAPICTRL_SetState(MMIIDLE_FM_FORM_CTRL_ID, GUICTRL_STATE_INVISIBLE, FALSE);
}

/*****************************************************************************/
//  Description : display the mp3 name or not
//  Global resource dependence :
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void UpdateIdleMusicNotification(void)
{
    wchar               music_name[MMIMP3_NAME_MAX_LEN + 1] = {0};
    MMI_STRING_T            name_str = {0};
    MMI_STRING_T            artist_str = {0};
    MMIMP3_ALBUM_INFO_T album_info  = {0};
    GUI_FONT_T      font_type = MMI_DEFAULT_NORMAL_FONT;    /*!<字体风格*/
    GUI_COLOR_T     font_color = MMI_WHITE_COLOR;   /*!<字体颜色*/
    GUI_RECT_T rect = {0};
    MMI_STRING_T    tmp_str_t = {0};
    wchar           tmp_wstr[32]   = {0};
    uint32   tmp_str_width = 0;
    uint32   display_width = 0;
    uint16   str_crop_num = 0;

    GUIFORM_PermitChildFont(MMIIDLE_MUSIC_FORM_CTRL_ID, FALSE);

    //song name
    GUILABEL_SetFont(MMIIDLE_MUSIC_NAME_LABLE_CTRL_ID, font_type, font_color);
    name_str.wstr_ptr = music_name;
    MMIAPIMP3_GetCurMp3Name(&name_str, MMIMP3_NAME_MAX_LEN);

    tmp_str_t.wstr_ptr = tmp_wstr;
    tmp_str_t.wstr_len = 4;
    MMI_STRNTOWSTR(tmp_str_t.wstr_ptr, tmp_str_t.wstr_len,"...", tmp_str_t.wstr_len, tmp_str_t.wstr_len);
    GUILABEL_GetStrRect(MMIIDLE_MUSIC_NAME_LABLE_CTRL_ID,&rect);
    if (rect.right-rect.left == 0)
    {
        display_width = FORM_CHILD_MUSIC_NAME_WIDTH;
    }
    else
    {
        display_width = rect.right-rect.left;
    }

    if (MMIAPICOM_GetStrNumForDisplayWidth(name_str.wstr_ptr, &name_str.wstr_len, 0, display_width, font_type) <  name_str.wstr_len)
    {
        tmp_str_width = MMIAPICOM_GetStrWidth(tmp_str_t.wstr_ptr, &tmp_str_t.wstr_len, 1, display_width, font_type);
        str_crop_num = MMIAPICOM_GetStrNumForDisplayWidth(name_str.wstr_ptr, &name_str.wstr_len, 1, 
             display_width-tmp_str_width, font_type);
        if (name_str.wstr_len > str_crop_num)
        {
            MMI_WSTRNCPY((name_str.wstr_ptr+str_crop_num),4,tmp_str_t.wstr_ptr,4,4);
            name_str.wstr_len = str_crop_num+4;
        }
    }
	
    GUILABEL_SetText(MMIIDLE_MUSIC_NAME_LABLE_CTRL_ID, &name_str, FALSE);

#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
    //artist
    GUILABEL_SetFont(MMIIDLE_MUSIC_ARTIST_LABLE_CTRL_ID, font_type, font_color);
    MMIAPIMP3_GetCurMp3AlbumInfo(&album_info);
    artist_str.wstr_ptr = album_info.artist;
    artist_str.wstr_len = (uint16)MMIAPICOM_Wstrlen(artist_str.wstr_ptr);
    GUILABEL_SetText(MMIIDLE_MUSIC_ARTIST_LABLE_CTRL_ID, &artist_str, FALSE);
#endif
    //GUIAPICTRL_SetState(MMIIDLE_MUSIC_FORM_CTRL_ID, GUICTRL_STATE_INVISIBLE, FALSE);
}

/*****************************************************************************/
//  Description : apend list item
//  Global resource dependence :
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL void UpdateIdleWinPushNotificationList(
    MMI_CTRL_ID_T ctrl_id
)
{
    int i = 0;
    uint16 msg_count = 0;
    uint16 cur_index = GUILIST_GetCurItemIndex(ctrl_id);
    //颜色值
    GUI_COLOR_T color = GUI_RGB2RGB565(255, 255, 255); 
    GUILIST_POS_INFO_T pos_info = {0};
    
    CTRLLIST_GetCurPosInfo(ctrl_id, &pos_info);
SCI_TraceLow("UpdateIdleWinPushNotificationList ");

    GUILIST_RemoveAllItems(ctrl_id);
	//dong.chunguang mdofiy show icon only
    if(/*IsNotificationPrivacyMode()*/MMISet_GetNotificationShowType()==1)
    {
        ClearNotificationAnimData();
        DisplayNotifactionIndicator();
    }
    else
    {
        for(i = 0; i < MMI_IDLE_NTF_INDICATOR_MAX; i++)
        {         
            GUIANIM_SetVisible(MMIIDLE_NTF_ANIM_CTRL_ID_1 + i, FALSE, FALSE);
        }
        s_idle_privacy_mode_ntf_height = 0;
    }

    for(i = 0; i < MMI_IDLE_PUSH_MSG_MAX_COUNT; i ++)
    {
        IDLE_PUSH_MSG_NODE_T *idle_push_msg_node_ptr = PNULL;

        idle_push_msg_node_ptr = MMIIDLE_GetPushMsgNode(i);
	SCI_TraceLow("UpdateIdleWinPushNotificationList =====");
	
        if(PNULL != idle_push_msg_node_ptr
                && MMI_IDLE_PUSH_MSG_STATE_VALID == idle_push_msg_node_ptr->msg_state)
        {
			//dong.chunguang mdofiy show icon only
            if(/*!IsNotificationPrivacyMode()*/MMISet_GetNotificationShowType()==0
                || MMI_IDLE_PUSH_MSG_ONGOING_CALL == idle_push_msg_node_ptr->idle_push_msg_info.e_push_msg_type)
            {
                int content_index = 0;
                GUILIST_ITEM_T          item_t     = {0};
                GUILIST_ITEM_DATA_T  item_data = {0};
                GUIANIM_FILE_INFO_T   anim_path = {0};

                item_t.item_style       = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_MS;
                item_t.left_item_style  = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS;
                item_t.right_item_style = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_WITH_RIGHT_FOCUS_MS;
                item_t.item_data_ptr = &item_data;
                item_t.user_data = idle_push_msg_node_ptr;

                if(MMI_IDLE_ICON_DATA_ANIM_PATH == idle_push_msg_node_ptr->idle_push_msg_info.icon_data_type)
                {
                    anim_path.full_path_wstr_len = MMIAPICOM_Wstrlen(idle_push_msg_node_ptr->idle_push_msg_info.icon_data.full_path_wstr_ptr);
                    anim_path.full_path_wstr_ptr = idle_push_msg_node_ptr->idle_push_msg_info.icon_data.full_path_wstr_ptr;
                    item_data.item_content[content_index].item_data_type     = GUIITEM_DATA_ANIM_PATH;
                    item_data.item_content[content_index].item_data.anim_path_ptr = &anim_path;
                }
                else if(MMI_IDLE_ICON_DATA_IMAGE_ID == idle_push_msg_node_ptr->idle_push_msg_info.icon_data_type)
                {
                    item_data.item_content[content_index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
                    item_data.item_content[content_index].item_data.image_id =
                        idle_push_msg_node_ptr->idle_push_msg_info.icon_data.image_id;
                }
                else
                {
                    item_data.item_content[content_index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
                    item_data.item_content[content_index].item_data.image_id =
                        MMIIDLE_GetPushMsgIcon(idle_push_msg_node_ptr->idle_push_msg_info.e_push_msg_type);
                }

                content_index++;

                item_data.item_content[content_index].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[content_index].item_data.text_buffer.wstr_ptr =
                    idle_push_msg_node_ptr->idle_push_msg_info.title;
                item_data.item_content[content_index].item_data.text_buffer.wstr_len =
                    MMIAPICOM_Wstrlen(idle_push_msg_node_ptr->idle_push_msg_info.title);
                content_index++;

                item_data.item_content[content_index].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[content_index].item_data.text_buffer.wstr_ptr =
                    idle_push_msg_node_ptr->idle_push_msg_info.contents;
                item_data.item_content[content_index].item_data.text_buffer.wstr_len =
                    MMIAPICOM_Wstrlen(idle_push_msg_node_ptr->idle_push_msg_info.contents);

                MMIIDLE_SetPushMsgSoftkey(idle_push_msg_node_ptr->idle_push_msg_info.e_push_msg_type, &item_data);

        		//设置item状态为自定义字体颜色
                GUILIST_SetItemState(&item_t, GUIITEM_STATE_COSTMER_FONT_COLOR, TRUE);
                //设置第1个content的字体颜色、透明度,包括高亮状态下和正常状态下
                GUILIST_SetItemContentCustomFontColor(ctrl_id, 2, &color, 255*0.80, &color, 255*0.80);//设置第1个content的字体颜色,包括高亮状态下和正常状态下

                GUILIST_AppendItem(ctrl_id, &item_t);
                msg_count++;
		SCI_TraceLow("UpdateIdleWinPushNotificationList =====msg_count=%d",msg_count);		

            }
        }
        else
        {
            break;
        }
    }

    if(0 == msg_count)
    {
        GUIFORM_SetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, ctrl_id, GUIFORM_CHILD_DISP_HIDE);
    }
    else
    {

        THEMELIST_ITEM_STYLE_T *my_style = THEMELIST_GetStyle(GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_MS);
        if(PNULL != my_style)
        {
            GUI_COLOR_T bg_color = MMITHEME_GetTransparenceColor();
            
            //删除最后一条通知
            if(cur_index > msg_count -1)
            {
                cur_index = msg_count -1; 
            }

            if(!CTRLLIST_GetListState(ctrl_id, GUILIST_STATE_ACTIVE))
            {
                cur_index = msg_count -1; 
            }
            else
            {
                CTRLLIST_SetCurPosInfo(ctrl_id, &pos_info);
            }
            
            GUILIST_SetCurItemIndex(ctrl_id, cur_index);
            GUILIST_SetSelectedItem(ctrl_id, cur_index, TRUE);

            GUILIST_SetListState(ctrl_id, GUILIST_STATE_CYC_SCROLL_ITEM, FALSE);
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);

            //设置左focus时显示的左Quick图标、设置右focus时显示的右Quick图标
            //设置需要响应左右键处理
            MMIIDLE_SetPushListItemLRAction(ctrl_id, msg_count);

            GUIFORM_SetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, ctrl_id, GUIFORM_CHILD_DISP_NORMAL);
        }
        else
        {
            SCI_TRACE_LOW("UpdateIdleWinPushNotificationList form theme list style not exist!!!");
        }

    }


}

/*****************************************************************************/
//  Description : Handle clear all button click event on idle
//  Global resource dependence :
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleClearAllBtnClick(void)
{
    MMIIDLE_RemoveAllPushMsg();
    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description : update idle
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_UpdateMusicProgressTimeinfo(void)
{
    if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
    {
        MMI_CTRL_ID_T   ctrl_id1 =  MMIIDLE_MUSIC_PROGRESSBAR_CTRL_ID;
        MMI_CTRL_ID_T   ctrl_id2 =  MMIIDLE_MUSIC_CURTIME_OWNDRAW_CTRL_ID;
        MMI_CTRL_ID_T   ctrl_id3 =  MMIIDLE_MUSIC_TOTALTIME_OWNDRAW_CTRL_ID;

        GUIOWNDRAW_Update(ctrl_id1);
        GUIOWNDRAW_Update(ctrl_id2);
        GUIOWNDRAW_Update(ctrl_id3);
    }
}
/******************************************************************************/
//  Description : Create softkey.
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_CreateSoftkey(void)
{
    MMI_HANDLE_T idle_win_handle = MMK_ConvertIdToHandle(MAIN_IDLE_WIN_ID);
    //create softkey
    GUISOFTKEY_INIT_DATA_T softkey_data = {0};
    MMI_CONTROL_CREATE_T softkey_create = {0};
    MMI_CTRL_ID_T softkey_ctrl_id = 0;

    softkey_data.leftsoft_id = IMAGE_NULL;
    softkey_data.midsoft_id = TXT_NULL;
    softkey_data.rightsoft_id = IMAGE_NULL;

    softkey_ctrl_id = MMITHEME_GetSoftkeyCtrlId();
    softkey_create.ctrl_id = softkey_ctrl_id;
    softkey_create.guid    = SPRD_GUI_SOFTKEY_ID;
    softkey_create.parent_win_handle = idle_win_handle;
    softkey_create.init_data_ptr = &softkey_data;

    MMK_SetWinDisplayStyleState(idle_win_handle, WS_HAS_BUTTON_SOFTKEY | WS_DISABLE_SOFTKEY_TO_TITLE, TRUE );

    MMK_CreateControl( &softkey_create );
    MMK_SetWinSoftkeyCtrlId(idle_win_handle, softkey_ctrl_id);
    GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, IMAGE_NULL, MIDDLE_BUTTON, FALSE);
    GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, STXT_SOFTKEY_MENU_MK, LEFT_BUTTON, FALSE);
    GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, STXT_SOFTKEY_PB_MK, RIGHT_BUTTON, FALSE);
}

#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)
/*****************************************************************************/
//  Description : get idle mp3 info
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC IDLE_MP3_NAME_INFO* MMIIDLECOM_GetIdleMp3Info(void)
{
    return &g_idle_cur_mp3_name;
}
#endif

PUBLIC void DisplayIdleWinSoftkey(void)
{
    GUI_POINT_T             point                = {0};
	GUI_LCD_DEV_INFO        lcd_dev_info             = *GetCommonIdlePaintLcdDev();
	GUI_RECT_T              mainlcd_rect             = MMITHEME_GetFullScreenRect();
    GUI_RECT_T      softkey_rect = {0};
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    MMI_STRING_T    text = {0};


#if defined (MMI_RES_ORIENT_PORTRAIT) && defined (MAINLCD_SIZE_128X160)//arvin zhang added for idle softkey icon    	
	uint16                  idle_softkey_icon_width  = 0;
    uint16                  idle_softkey_icon_height = 0; 
	uint32                  i                        = 0;
	MMI_IMAGE_ID_T          idle_softkey_icon[MMIIDLE_SOFTKEY_ICON_MAX] = {0};
#ifndef MMI_COMMON_IDLE_IMG_SOFTKEY_SUPPORT	
#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
    if (0 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))
    {
		idle_softkey_icon[0] = IMAGE_IDLE_ICON_MENU;
		idle_softkey_icon[1] = IMAGE_IDLE_ICON_SOS;
    }
	else
#endif
	{
		idle_softkey_icon[0] = IMAGE_IDLE_ICON_MENU;
		idle_softkey_icon[1] = IMAGE_IDLE_ICON_PB;
    }
#else

#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
    if (0 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))
    {
		idle_softkey_icon[0] = IMAGE_IDLE_SHORTCUT_MAINMENU;
		idle_softkey_icon[1] = IMAGE_IDLE_ICON_SOS;
    }
	else
#endif

	{
		idle_softkey_icon[0] = IMAGE_IDLE_SHORTCUT_MAINMENU;
		idle_softkey_icon[1] = IMAGE_IDLE_SHORTCUT_PB;
    }
#endif
#ifdef MMI_COMMON_IDLE_IMG_SOFTKEY_SUPPORT
#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
    if (0 != MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))
#endif
	{
     MMIAPISET_GetFSMainIMGId(&idle_softkey_icon[0],&idle_softkey_icon[MMIIDLE_SOFTKEY_ICON_MAX-1]);
	}

#endif
    GUIRES_GetImgWidthHeight(&idle_softkey_icon_width, &idle_softkey_icon_height, idle_softkey_icon[0], MAIN_IDLE_WIN_ID);

	for (i = 0; i < MMIIDLE_SOFTKEY_ICON_MAX; i++)
	{
		
	    //set softkey icon display dis_point
		if (0 == i)
		{
			point.x = mainlcd_rect.left + MMIIDLE_SOFTKEY_MARGIN_WIDTH;//left icon		  
		}
		else if ((MMIIDLE_SOFTKEY_ICON_MAX-1) == i)
		{
			point.x = mainlcd_rect.right  - idle_softkey_icon_width - MMIIDLE_SOFTKEY_MARGIN_WIDTH;//right icon		    
		}
        else
        {
            point.x = (mainlcd_rect.right>>1 )- idle_softkey_icon_width + 2*MMIIDLE_SOFTKEY_MARGIN_WIDTH; 
        }
		point.y = mainlcd_rect.bottom - idle_softkey_icon_height - MMIIDLE_SOFTKEY_MARGIN_WIDTH;// the same height
		//display softkey icon
		GUIRES_DisplayImg(&point,
			PNULL,
			PNULL,
			MAIN_IDLE_WIN_ID,
			idle_softkey_icon[i],
		    &lcd_dev_info);
	}
#else 
    MMI_STRING_T            mess            = {0};	
    uint16                  str_pixel       = 0;
    uint16                  font_height     = 0; 
    uint16                  str_width       = 0;
    wchar                   *full_str       = PNULL;
    uint16                  full_len        = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
#ifndef TOUCH_PANEL_SUPPORT
    GUI_RECT_T              right_softkey_display_rect ={0};
    GUI_RECT_T              left_softkey_display_rect ={0};
#endif	
    //按照移动定制，左软键硬按键绑定为主菜单

#ifdef BAR_PHONE_WIDGET 
    MMI_TEXT_ID_T           left_key_id     = TXT_COMMON_MAINMENU;
     MMI_TEXT_ID_T           right_key_id    = TXT_IDLE_BUSINESS_MODE;
#else   
    MMI_TEXT_ID_T           left_key_id     = STXT_SOFTKEY_MENU_MK;//STXT_MAIN_MESSAGE;
    MMI_TEXT_ID_T           right_key_id    = STXT_MAIN_PB;
#endif    
    //MMI_TEXT_ID_T           middle_key_id    = TXT_COMM_OPEN;
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
	
#ifdef CMCC_UI_STYLE
    GUI_POINT_T             dis_point       = {0};
    uint16                  mid_icon_width = 0;
    uint16                  mid_icon_height = 0;
#endif
    full_str = SCI_ALLOCA(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);
    SCI_MEMSET(full_str,0,(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL));
    MMIAPISET_GetFSMainTextId(&left_key_id,&right_key_id);
    
    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    softkey_rect.left = 0;
    softkey_rect.top = (int16)(lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - 1);
    softkey_rect.right = (int16)lcd_width;
    softkey_rect.bottom = (int16)(lcd_height - 1);

    LCD_FillRect(&lcd_dev_info, softkey_rect, MMI_BLACK_COLOR);

    //显示左软键
    font_height = GUI_GetFontHeight(MMI_FULLSCREEN_SOFTKEY_TEXT_SMALL_FONT, UNICODE_HANZI);
    
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)left_key_id,&mess);
    
#if defined(BAR_PHONE_WIDGET)    
    point.y =  mainlcd_rect.bottom - MMITHEME_GetSoftkeyHeight()/2 - font_height/2 + 1;/*lint !e737*/
#else
    point.y =  mainlcd_rect.bottom - MMITHEME_GetSoftkeyHeight()/2 - font_height/2;/*lint !e737*/
#endif    
#ifdef  DPHONE_SUPPORT
    point.x = 1;
#elif defined(BAR_PHONE_WIDGET)    
    point.x = 2;
#else    
    point.x = 1;
#endif
  
    MMIIDLE_GetIdleTextStyle(&text_style, MMI_FULLSCREEN_SOFTKEY_TEXT_SMALL_FONT,  MMI_WHITE_COLOR);
    
    str_pixel = GUISTR_GetStringWidth(&text_style,&mess,text_state);
    MMIIDLE_GetDisPlayString(mess.wstr_ptr,mess.wstr_len,(mainlcd_rect.right -1)/2,MMI_FULLSCREEN_SOFTKEY_TEXT_SMALL_FONT,full_str,&full_len);
	//使用...处理后回传的str参数重新计算宽度
	mess.wstr_ptr = full_str;
	mess.wstr_len = full_len;
	str_pixel = GUISTR_GetStringWidth(&text_style,&mess,text_state);
	
#if defined TOUCH_PANEL_SUPPORT
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].type        = MMI_MAIN_LCD_LEFT_SOFTKEY;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.left   = point.x;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.top    = point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.right  = point.x+str_pixel+1;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.bottom = point.y+font_height;
    
    str_width = MMIIDLE_DisplayIdleText(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T *)&point,
        MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,
        MMI_WHITE_COLOR,
        full_str, 
        full_len
        );
#ifdef BAR_PHONE_TOUCH_WIDGET
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.top    = s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.top-MMIIDLE_SOFTKEY_ADD_HEIGHT;
#endif
#else

        left_softkey_display_rect.left   = MMI_SOFTKEY_SPACE_H - 1;
        left_softkey_display_rect.top = (int16)(lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT + MMI_SOFTKEY_SPACE_H - 1);//320-38+6
        left_softkey_display_rect.right = MMI_SOFTKEY_SPACE_H + 72 -1;
        left_softkey_display_rect.bottom = (int16)(lcd_height - MMI_SOFTKEY_SPACE_H -1);//320-6

    MMIIDLE_GetIdleTextStyle(&text_style, MMI_FULLSCREEN_SOFTKEY_TEXT_SMALL_FONT, MMI_WHITE_COLOR);
    text_style.align = ALIGN_LVMIDDLE;
        
    text.wstr_len = full_len;
    text.wstr_ptr = full_str;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&left_softkey_display_rect,
        (const GUI_RECT_T      *)&left_softkey_display_rect,
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );
#endif
    //显示右软键
    SCI_MEMSET(full_str,0,(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL));
    full_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;

#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
    if (0 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_SOS,&mess);
    }
    else
#endif
	{
	    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)right_key_id,&mess);  
	}

    str_pixel = GUISTR_GetStringWidth(&text_style,&mess,text_state);
    MMIIDLE_GetDisPlayString(mess.wstr_ptr,mess.wstr_len,(mainlcd_rect.right -1)/2,MMI_FULLSCREEN_SOFTKEY_TEXT_SMALL_FONT,full_str,&full_len);
	//使用...处理后回传的str参数重新计算宽度
	mess.wstr_ptr = full_str;
	mess.wstr_len = full_len;
	str_pixel = GUISTR_GetStringWidth(&text_style,&mess,text_state);
#ifdef  DPHONE_SUPPORT
#if defined TOUCH_PANEL_SUPPORT
	point.x = MAX((mainlcd_rect.right - str_pixel-1),(s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.right+1));
#else
	point.x = MAX((mainlcd_rect.right - str_pixel-1),(left_softkey_display_rect.right+1));
#endif
#elif defined(BAR_PHONE_WIDGET)    
#if defined TOUCH_PANEL_SUPPORT
    point.x = MAX((mainlcd_rect.right - str_pixel-4),(s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.right+3));
#else
    point.x = MAX((mainlcd_rect.right - str_pixel-4),(left_softkey_display_rect.right+3));
#endif
//#elif defined (MAINLCD_SIZE_128X160)
//    point.x = MAX((mainlcd_rect.right - str_pixel-4),(s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.right));
#else    
#if defined TOUCH_PANEL_SUPPORT
    point.x = MAX((mainlcd_rect.right - str_pixel-4),(s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.right+10));
#else 
    point.x = MAX((mainlcd_rect.right - str_pixel-4),(left_softkey_display_rect.right+10));
#endif
#endif
#if defined(BAR_PHONE_WIDGET)    
    point.y = mainlcd_rect.bottom - MMITHEME_GetSoftkeyHeight()/2 - font_height/2 + 1;/*lint !e737*/
#else
    point.y = mainlcd_rect.bottom - MMITHEME_GetSoftkeyHeight()/2 - font_height/2;/*lint !e737*/
#endif    
#if defined TOUCH_PANEL_SUPPORT
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].type        = MMI_MAIN_LCD_RIGHT_SOFTKEY;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.left   = point.x;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.top    = point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.right  = MIN((point.x+str_pixel+1),mainlcd_rect.right);
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.bottom = point.y+font_height;
    
    str_width = MMIIDLE_DisplayIdleText(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T *)&point,
        MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,
        MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_SOFTKEY),
        full_str, 
        full_len
        );
#ifdef BAR_PHONE_TOUCH_WIDGET
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.top    = s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.top-MMIIDLE_SOFTKEY_ADD_HEIGHT;
#endif
#else

        right_softkey_display_rect.left   = (int16)(lcd_width - 72 - MMI_SOFTKEY_SPACE_H -1);
        right_softkey_display_rect.top    = (int16)(lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT + MMI_SOFTKEY_SPACE_H - 1);//320-38+6
        right_softkey_display_rect.right  = (int16)(lcd_width  - MMI_SOFTKEY_SPACE_H - 1);//240-6
        right_softkey_display_rect.bottom = (int16)(lcd_height - MMI_SOFTKEY_SPACE_H -1);//320-6

    MMIIDLE_GetIdleTextStyle(&text_style, MMI_FULLSCREEN_SOFTKEY_TEXT_SMALL_FONT, MMI_WHITE_COLOR);
    text_style.align = ALIGN_RVMIDDLE;
    
    text.wstr_len = full_len;
    text.wstr_ptr = full_str;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&right_softkey_display_rect,
        (const GUI_RECT_T      *)&right_softkey_display_rect,
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );

#endif
    SCI_FREE(full_str);
    full_str = PNULL;
#ifdef CMCC_UI_STYLE
#ifndef MMI_IDLE_MINI_SUPPORT
    GUIRES_GetImgWidthHeight(&mid_icon_width, &mid_icon_height,IMAGE_IDLE_MIDDLE_SOFTKEY_ICON , MAIN_IDLE_WIN_ID);

    dis_point.x = (mainlcd_rect.right>>1 )- mid_icon_width;
    dis_point.y = mainlcd_rect.bottom - mid_icon_height - 2;
    //display middle softkey icon
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        MAIN_IDLE_WIN_ID,
        IMAGE_IDLE_MIDDLE_SOFTKEY_ICON,
        &lcd_dev_info);
#endif    
#endif
#endif//#ifdef MAINLCD_SIZE_128X160
}

#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)
/*****************************************************************************/
//  Description : start mp3 name idle timer
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIIDLECOM_StartIdleMp3Timer(void)
{
    BOOLEAN     result  =   FALSE;

    if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
        if ((0 == g_mp3_name_timer_id) &&
            (MMIIDLECOM_GetIdleMp3Info()->is_scroll))
        {
            g_mp3_name_timer_id = MMK_CreateWinTimer(MAIN_IDLE_WIN_ID, IDLE_MP3_TIMER_PERIOD, FALSE);
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : stop mp3 name idle timer
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLECOM_StopIdleMp3Timer(void)
{
    BOOLEAN     result  =   FALSE;

    if (0 != g_mp3_name_timer_id)
    {
        MMK_StopTimer(g_mp3_name_timer_id);
        g_mp3_name_timer_id = 0;
        result = TRUE;
    }

    return (result);
}
/*****************************************************************************/
// Description : UpdateMp3NameStartIndex
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void MMIIDLECOM_UpdateMp3NameStartIndex(void)
{  
    MMI_STRING_T        text = {0};
    wchar               mp3_name[MMIMP3_NAME_MAX_LEN + 1]={0};
    text.wstr_ptr = mp3_name;

    if(FM_PLAYING == MMIAPIFM_GetStatus())
    {
        MMIAPIFM_GetCurFreqName(&text, MMIMP3_NAME_MAX_LEN);
    }
    else
    {
        MMIAPIMP3_GetCurMp3Name(&text, MMIMP3_NAME_MAX_LEN);
    }
    
    MMIIDLECOM_GetIdleMp3Info()->str_index += 1;
    if (0 == text.wstr_len)//防错处理，防止没有文件名的Mp3
    {
        MMIIDLECOM_GetIdleMp3Info()->str_index = 0;
    }
    else
    {
        MMIIDLECOM_GetIdleMp3Info()->str_index %= text.wstr_len;
    }
    g_mp3_name_timer_id = 0;
}
/*****************************************************************************/
// Description : MMIIDLECOM_GetMp3NameTimerId
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC uint8 MMIIDLECOM_GetMp3NameTimerId(void)
{
    return g_mp3_name_timer_id;
}


#endif

/******************************************************************************/
//  Description : Create control dynamically.
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL void CreateMusicNotificationCtrl(void)
{
    MMI_HANDLE_T idle_win_handle = MMK_ConvertIdToHandle(MAIN_IDLE_WIN_ID);
    uint16 ver_space = 0;
    uint16 hor_space = 0;
    //Music Parent Form
    {
        GUIFORM_DYNA_CHILD_T    child_data = {0};
        GUIFORM_INIT_DATA_T     init_data = {0};
        MMITHEME_FORM_T         form_theme = {0};
        //init data
        init_data.layout_type = GUIFORM_LAYOUT_ORDER;

        child_data.is_bg = FALSE;
        child_data.is_get_active = TRUE;
        child_data.guid          = SPRD_GUI_FORM_ID;
        child_data.child_handle  = MMIIDLE_MUSIC_FORM_CTRL_ID;
        child_data.init_data_ptr = &init_data;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
                                   MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,
                                   &child_data);
        MMITHEME_GetFormTheme(&form_theme);
        form_theme.form_active_child.bg.shape = GUI_SHAPE_RECT;
        CTRLFORM_SetFormActiveChildTheme(MMK_ConvertIdToHandle (MMIIDLE_MUSIC_FORM_CTRL_ID), &form_theme.form_active_child.bg, PNULL);
        GUIFORM_SetStyle(MMIIDLE_MUSIC_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
        GUIFORM_SetChildSpace(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_MUSIC_FORM_CTRL_ID, &hor_space, &ver_space);
        //GUICTRL_SetProgress(MMK_GetCtrlPtr(MMIIDLE_MUSIC_FORM_CTRL_ID),FALSE);
    }
    //child song name label
    {
        GUILABEL_INIT_DATA_T    init_data = {0};
        GUIFORM_DYNA_CHILD_T    child_data = {0};

        //init data
        init_data.align = GUILABEL_ALIGN_MIDDLE;

        child_data.is_bg = FALSE;
        child_data.is_get_active = FALSE;
        child_data.guid = SPRD_GUI_LABEL_ID;
        child_data.init_data_ptr = &init_data;
        child_data.child_handle = MMIIDLE_MUSIC_NAME_LABLE_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
                                   MMIIDLE_MUSIC_FORM_CTRL_ID,
                                   &child_data);
    }
    //child artist label
    {
        GUILABEL_INIT_DATA_T    init_data = {0};
        GUIFORM_DYNA_CHILD_T    child_data = {0};

        //init data
        init_data.align = GUILABEL_ALIGN_MIDDLE;

        child_data.is_bg = FALSE;
        child_data.is_get_active = FALSE;
        child_data.guid = SPRD_GUI_LABEL_ID;
        child_data.init_data_ptr = &init_data;
        child_data.child_handle = MMIIDLE_MUSIC_ARTIST_LABLE_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
                                   MMIIDLE_MUSIC_FORM_CTRL_ID,
                                   &child_data);
    }
    //child progressbar
    {
        GUIOWNDRAW_INIT_DATA_T      init_data = {0};
        GUIFORM_DYNA_CHILD_T    child_data = {0};

        init_data.OwnDrawFunc = UpdateIdleMusicNotiProgressOwnerDrawCtrl;

        child_data.is_bg = FALSE;
        child_data.is_get_active = FALSE;
        child_data.guid = SPRD_GUI_OWNDRAW_ID;
        child_data.init_data_ptr = &init_data;
        child_data.child_handle = MMIIDLE_MUSIC_PROGRESSBAR_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
                                   MMIIDLE_MUSIC_FORM_CTRL_ID,
                                   &child_data);
    }
    //child NAVI form
    {
        GUIFORM_DYNA_CHILD_T    child_data = {0};
        GUIFORM_INIT_DATA_T     init_data = {0};

        //init data
        init_data.layout_type = GUIFORM_LAYOUT_SBS;

        child_data.is_get_active = TRUE;
        child_data.guid          = SPRD_GUI_FORM_ID;
        child_data.child_handle  = MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID;
        child_data.init_data_ptr = &init_data;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
                                   MMIIDLE_MUSIC_FORM_CTRL_ID,
                                   &child_data);

        GUIFORM_SetChildAlign(MMIIDLE_MUSIC_FORM_CTRL_ID, MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID, GUIFORM_CHILD_ALIGN_HMIDDLE);
        GUIFORM_SetChildSpace(MMIIDLE_MUSIC_FORM_CTRL_ID, MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID, &hor_space, &ver_space);
        GUIFORM_SetStyle(MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
    }
    //child crutime
    {
        GUIOWNDRAW_INIT_DATA_T      init_data = {0};
        GUIFORM_DYNA_CHILD_T    child_data = {0};

        init_data.OwnDrawFunc = UpdateIdleMusicNotitimeinfoOwnerDrawCtrl;

        child_data.is_bg = FALSE;
        child_data.is_get_active = FALSE;
        child_data.guid = SPRD_GUI_OWNDRAW_ID;
        child_data.init_data_ptr = &init_data;
        child_data.child_handle = MMIIDLE_MUSIC_CURTIME_OWNDRAW_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
                                   MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID,
                                   &child_data);
    }
    //child Play/Pause
    {
        GUIOWNDRAW_INIT_DATA_T      init_data = {0};
        GUIFORM_DYNA_CHILD_T    child_data = {0};

        init_data.OwnDrawFunc = UpdateIdleMusicNotiPlayNextbtnOwnerDrawCtrl;


        child_data.is_bg = FALSE;
        child_data.is_get_active = TRUE;
        child_data.guid = SPRD_GUI_OWNDRAW_ID;
        child_data.init_data_ptr = &init_data;
        child_data.child_handle = MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
                                   MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID,
                                   &child_data);
    }
    //child Forward
    {
        GUIOWNDRAW_INIT_DATA_T      init_data = {0};
        GUIFORM_DYNA_CHILD_T    child_data = {0};

        init_data.OwnDrawFunc = UpdateIdleMusicNotiPlayNextbtnOwnerDrawCtrl;

        child_data.is_bg = FALSE;
        child_data.is_get_active = FALSE;
        child_data.guid = SPRD_GUI_OWNDRAW_ID;
        child_data.init_data_ptr = &init_data;
        child_data.child_handle = MMIIDLE_MUSIC_NEXT_OWNDRAW_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
                                   MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID,
                                   &child_data);
    }
    //child totaltime
    {
        GUIOWNDRAW_INIT_DATA_T      init_data = {0};
        GUIFORM_DYNA_CHILD_T    child_data = {0};

        init_data.OwnDrawFunc = UpdateIdleMusicNotitimeinfoOwnerDrawCtrl;

        child_data.is_bg = FALSE;
        child_data.is_get_active = FALSE;
        child_data.guid = SPRD_GUI_OWNDRAW_ID;
        child_data.init_data_ptr = &init_data;
        child_data.child_handle = MMIIDLE_MUSIC_TOTALTIME_OWNDRAW_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
                                   MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID,
                                   &child_data);
    }
}

/******************************************************************************/
//  Description : Create control dynamically.
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL void CreateFmNotificationCtrl(void)
{
    MMI_HANDLE_T idle_win_handle = MMK_ConvertIdToHandle(MAIN_IDLE_WIN_ID);
    uint16 ver_space = 0;
    uint16 hor_space = 0;
    //FM Parent Form
    {
        GUIFORM_DYNA_CHILD_T    child_data = {0};
        GUIFORM_INIT_DATA_T     init_data = {0};
        MMITHEME_FORM_T         form_theme = {0};
        //init data
        init_data.layout_type = GUIFORM_LAYOUT_ORDER;

        child_data.is_bg = FALSE;
        child_data.is_get_active = TRUE;
        child_data.guid          = SPRD_GUI_FORM_ID;
        child_data.child_handle  = MMIIDLE_FM_FORM_CTRL_ID;
        child_data.init_data_ptr = &init_data;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
                                   MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,
                                   &child_data);
        MMITHEME_GetFormTheme(&form_theme);
        form_theme.form_active_child.bg.shape = GUI_SHAPE_RECT;
        CTRLFORM_SetFormActiveChildTheme(MMK_ConvertIdToHandle (MMIIDLE_FM_FORM_CTRL_ID), &form_theme.form_active_child.bg, PNULL);
        GUIFORM_SetStyle(MMIIDLE_FM_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
        GUIFORM_SetChildSpace(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_FM_FORM_CTRL_ID, &hor_space, &ver_space);
    }
    //child label
    {
        GUILABEL_INIT_DATA_T    init_data = {0};
        GUIFORM_DYNA_CHILD_T    child_data = {0};

        //init data
        init_data.align = GUILABEL_ALIGN_MIDDLE;

        child_data.is_bg = FALSE;
        child_data.is_get_active = FALSE;
        child_data.guid = SPRD_GUI_LABEL_ID;
        child_data.init_data_ptr = &init_data;
        child_data.child_handle = MMIIDLE_FM_FREQ_LABLE_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
                                   MMIIDLE_FM_FORM_CTRL_ID,
                                   &child_data);

        GUIFORM_SetChildAlign(MMIIDLE_FM_FORM_CTRL_ID, MMIIDLE_FM_FREQ_LABLE_CTRL_ID, GUIFORM_CHILD_ALIGN_HMIDDLE);
    }
    //child NAVI form
    {
        GUIFORM_DYNA_CHILD_T    child_data = {0};
        GUIFORM_INIT_DATA_T     init_data = {0};

        //init data
        init_data.layout_type = GUIFORM_LAYOUT_SBS;

        child_data.is_get_active = TRUE;
        child_data.guid          = SPRD_GUI_FORM_ID;
        child_data.child_handle  = MMIIDLE_FM_CHIL_NAVI_FORM_CTRL_ID;
        child_data.init_data_ptr = &init_data;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
                                   MMIIDLE_FM_FORM_CTRL_ID,
                                   &child_data);

        GUIFORM_SetChildAlign(MMIIDLE_FM_FORM_CTRL_ID, MMIIDLE_FM_CHIL_NAVI_FORM_CTRL_ID, GUIFORM_CHILD_ALIGN_HMIDDLE);
        GUIFORM_SetChildSpace(MMIIDLE_FM_FORM_CTRL_ID, MMIIDLE_FM_CHIL_NAVI_FORM_CTRL_ID, &hor_space, &ver_space);
        GUIFORM_SetStyle(MMIIDLE_FM_CHIL_NAVI_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
    }
    //child Null
    {
        GUIANIM_INIT_DATA_T    init_data = {0};
        GUIFORM_DYNA_CHILD_T    child_data = {0};

        //init data
        init_data.is_form_bg = FALSE;

        child_data.is_bg = FALSE;
        child_data.is_get_active = FALSE;
        child_data.guid = SPRD_GUI_BUTTON_ID;
        child_data.init_data_ptr = &init_data;
        child_data.child_handle = MMIIDLE_FM_NULL_ANIM_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
                                   MMIIDLE_FM_CHIL_NAVI_FORM_CTRL_ID,
                                   &child_data);
    }
    //child Play/Pause
    {
        GUIANIM_INIT_DATA_T    init_data = {0};
        GUIFORM_DYNA_CHILD_T    child_data = {0};

        //init data
        init_data.is_form_bg = FALSE;

        child_data.is_bg = FALSE;
        child_data.is_get_active = TRUE;
        child_data.guid = SPRD_GUI_ANIM_ID;
        child_data.init_data_ptr = &init_data;
        child_data.child_handle = MMIIDLE_FM_PALY_ANIM_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
                                   MMIIDLE_FM_CHIL_NAVI_FORM_CTRL_ID,
                                   &child_data);
    }
    //child Forward
    {
        GUIANIM_INIT_DATA_T    init_data = {0};
        GUIFORM_DYNA_CHILD_T    child_data = {0};

        //init data
        init_data.is_form_bg = FALSE;

        child_data.is_bg = FALSE;
        child_data.is_get_active = FALSE;
        child_data.guid = SPRD_GUI_ANIM_ID;
        child_data.init_data_ptr = &init_data;
        child_data.child_handle = MMIIDLE_FM_NEXT_ANIM_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
                                   MMIIDLE_FM_CHIL_NAVI_FORM_CTRL_ID,
                                   &child_data);
    }
}



/******************************************************************************/
//  Description : Create control dynamically.
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL void CreateNotificationAnimCtrl(void)
{
    MMI_HANDLE_T idle_win_handle = MMK_ConvertIdToHandle(MAIN_IDLE_WIN_ID);
    int i = 0;

    for(i = 0; i < MMI_IDLE_NTF_INDICATOR_MAX; i++)
    {
        GUIANIM_INIT_DATA_T init_data = {0};
        MMI_CONTROL_CREATE_T create   = {0};           
        MMI_CTRL_ID_T ctrl_id = 0;
      
        ctrl_id = MMIIDLE_NTF_ANIM_CTRL_ID_1 + i;

        create.ctrl_id = ctrl_id;
        create.guid = SPRD_GUI_ANIM_ID;
        create.parent_win_handle = idle_win_handle;
        create.parent_ctrl_handle = 0;
        create.init_data_ptr = &init_data;
        
        MMK_CreateControl(&create);
    }

}



/******************************************************************************/
//  Description : Create control dynamically.
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
LOCAL void CreatePushMsgNotificationCtrl(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIIDLE_PUSH_MSG_LIST_CTRL_ID;
    MMI_HANDLE_T idle_win_handle = MMK_ConvertIdToHandle(MAIN_IDLE_WIN_ID);
    GUI_BOTH_RECT_T  both_rect  = MMITHEME_GetWinClientBothRect(idle_win_handle);
    //Form msg list
    {
        GUILIST_INIT_DATA_T    init_data = {0};
        GUIFORM_DYNA_CHILD_T    child_data = {0};

        both_rect.v_rect.top = both_rect.v_rect.bottom;
        both_rect.h_rect.top = both_rect.h_rect.bottom;

        init_data.both_rect = both_rect;
        init_data.is_dynamic_item_data = FALSE;
        init_data.max_item_num = MMI_IDLE_PUSH_MSG_MAX_COUNT;
        init_data.max_selected_num = init_data.max_item_num;
        init_data.scroll_bar_type = GUIPRGBOX_STYLE_VERTICAL_SCROLL;
        init_data.type = GUILIST_TEXTLIST_E;

        child_data.is_bg = FALSE;
        child_data.is_get_active = TRUE;
        child_data.guid = SPRD_GUI_LIST_ID;
        child_data.init_data_ptr = &init_data;
        child_data.child_handle = ctrl_id;

        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
                                   MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,
                                   &child_data);
        GUILIST_PermitBorder(ctrl_id, FALSE);
        GUILIST_SetIsPermitBottomMargin(ctrl_id, FALSE);
        
        //设置List背景半透明
        CTRLLIST_SetBgColor(ctrl_id, MMITHEME_GetTransparenceColor());
        GUILIST_SetBgTransparent(ctrl_id, TRUE, (uint8)MMI_IDLE_NTF_LIST_BG_ALPHA);
    }
}

/******************************************************************************/
//  Description : Create control dynamically.
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_CreateDynamicCtrl(MMI_WIN_ID_T win_id)
{
    MMI_HANDLE_T idle_win_handle = MMK_ConvertIdToHandle(win_id);
    GUI_BOTH_RECT_T  both_rect  = MMITHEME_GetWinClientBothRect(win_id);
    uint16 bottom_margin = FORM_BOTTOM_MAGRIN_HEIGHT;
    uint16 ver_space = 0;
    uint16 hor_space = 0;

    both_rect.v_rect.bottom -= bottom_margin;
    both_rect.h_rect.bottom -= bottom_margin;
    //parent form
    {
        GUI_BG_T bg = {0};

        GUIFORM_CreatDynaCtrl(idle_win_handle, MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, GUIFORM_LAYOUT_ORDER);
        bg.bg_type = GUI_BG_HALF_TRANS;
        bg.color = MMITHEME_GetTransparenceColor();
        bg.shape = GUI_SHAPE_RECT;
        GUIFORM_SetBg(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, &bg);
        GUIFORM_PermitChildBg(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,FALSE);
        GUIFORM_PermitChildBorder(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, FALSE);
        GUIFORM_PermitChildFont(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, FALSE);
        //GUIFORM_SetCircularHandleUpDown(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,TRUE);
        GUIFORM_SetType(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, GUIFORM_TYPE_TP);
        GUIAPICTRL_SetBothRect(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, &both_rect);
        GUIFORM_SetDividingLine(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, 0, MMITHEME_GetTransparenceColor());
        GUIFORM_SetMargin(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, 0);
        GUIFORM_SetSpace(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, &hor_space, &ver_space);
        GUIFORM_IsSlide(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, FALSE);
    }

    CreateNotificationAnimCtrl();
    CreatePushMsgNotificationCtrl();
    CreateFmNotificationCtrl();
    CreateMusicNotificationCtrl();
    
}


#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (KURO_SUPPORT)|| defined (FM_SUPPORT)
/*****************************************************************************/
//  Description : set idle mp3 name
//  Global resource dependence :
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleBgPlayName(
    BOOLEAN     is_display,
    MMIBGPLAY_TYPE_E bg_type
)
{

    MMI_IDLE_DISPLAY_T  idle_disp_style = {0};
    GUI_FONT_T          font            = SONG_FONT_8;
    MMI_STRING_T        text            = {0};
    uint16              lcd_width = 0; // lcd宽度
    uint16              lcd_height = 0; // lcd高度
    uint16              music_name_max_width = 0;
    wchar               mp3_name[MMIMP3_NAME_MAX_LEN + 1] = {0};


    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_MP3);
    font = idle_disp_style.text_font;
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    music_name_max_width = lcd_width - MMI_MAINSCREEN_MP3_LEFT - MMI_MAINSCREEN_MP3_LEFT;

    //显示当前播放的mp3 name,包括更换当前的mp3 name

    if (is_display)
    {
        MMIIDLE_GetIdleWinInfo()->is_display_mp3 = is_display;

        text.wstr_ptr = mp3_name;

        if(MMIBGPLAY_TYPE_FM == bg_type)
        {
            //get fm name
            MMIAPIFM_GetCurFreqName(&text, MMIMP3_NAME_MAX_LEN);
        }
#ifdef KURO_SUPPORT
        else if(MMIBGPLAY_TYPE_KURO == bg_type)
        {
            MMIAPIKUR_GetCurKurName(&text, MMIMP3_NAME_MAX_LEN);
        }
#endif
#ifdef MMI_AUDIO_PLAYER_SUPPORT
        else if(MMIBGPLAY_TYPE_MP3 == bg_type)
        {
            MMIAPIMP3_GetCurMp3Name(&text, MMIMP3_NAME_MAX_LEN);
        }
#endif
        //set global variable
        //g_idle_cur_mp3_name.is_ucs2 = text.is_ucs2;
        //g_idle_cur_mp3_name.length = text.wstr_len;
        g_idle_cur_mp3_name.str_index = 0;

        //is scroll
        g_idle_cur_mp3_name.str_width = GUI_CalculateStringPiexlNum(text.wstr_ptr,
                                        text.wstr_len,
                                        font, 0);
        if (music_name_max_width < g_idle_cur_mp3_name.str_width * 1.2)
        {
            g_idle_cur_mp3_name.is_scroll = TRUE;
        }
        else
        {
            g_idle_cur_mp3_name.is_scroll = FALSE;
        }
    }
    else
    {
        if((MMIBGPLAY_TYPE_FM == bg_type && !MMIAPIMP3_IsPlayerActive() && !MMIAPIMP3_IsPlayerPlaying() /*&& !MMIAPIKUR_IsPlayerActive()*/)
                || (MMIBGPLAY_TYPE_MP3 == bg_type && FM_PLAYING != MMIAPIFM_GetStatus() /*&& !MMIAPIKUR_IsPlayerActive()*/)
                || (/*MMIBGPLAY_TYPE_KURO == bg_type && */FM_PLAYING != MMIAPIFM_GetStatus() && !MMIAPIMP3_IsPlayerActive()))
        {
            //no bg play
            MMIIDLE_GetIdleWinInfo()->is_display_mp3 = is_display;
            SCI_MEMSET(&g_idle_cur_mp3_name, 0, sizeof(IDLE_MP3_NAME_INFO));
        }
    }

    //if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
    {
        MMK_PostMsg(MAIN_IDLE_WIN_ID, MSG_IDLE_UPDATE_MP3, PNULL, 0);
    }

}
#endif
/*****************************************************************************/
//  Description : display the  content
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void OutIdleWinContent(void)
{
    MMISET_IDLE_DISPLAY_T   *idle_mode  =   PNULL;
#ifdef UI_MULTILAYER_SUPPORT
    if(UILAYER_IsLayerActive(&s_common_idle_paint_layer_handle))
    {
        UILAYER_Clear(&s_common_idle_paint_layer_handle);
    }
    if(UILAYER_IsLayerActive(&s_common_idle_notification_layer_handle))
    {
        UILAYER_Clear(&s_common_idle_notification_layer_handle);
    }
#endif
    idle_mode = MMIAPISET_GetIdleDisplayMode();
    ResetDisplayNum();
    //show time and date
    if ( idle_mode->visible_flag.item_flag.is_show_time_and_date)
    {
        OutputIdleWinTime();
        OutputIdleWinWeekAndDate();
    }
    OutputIdleSimStatusEx();
#ifdef CALENDAR_SUPPORT
     if (idle_mode->visible_flag.item_flag.is_show_today_schedule&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
    {
        OutputIdleWinCalendar();
     }
#endif
#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)
	if ((
#ifdef MMI_AUDIO_PLAYER_SUPPORT
	(idle_mode->visible_flag.item_flag.is_show_mp3)
#else
	0
#endif
#ifdef FM_SUPPORT
	||(idle_mode->visible_flag.item_flag.is_show_fm)
#endif		
    )&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
    {
        DisplayIdleMp3Name();
    }
#endif
    //show counted time
#ifdef COUNTEDTIME_SUPPORT
    if (MMIAPICT_IsShowIdleCoutime()&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM)&&idle_mode->visible_flag.item_flag.is_show_countdown)
    {
        OutputIdleWinCountedTime(FALSE);
    }
#endif

    //show idle words
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT        
    if ( idle_mode->visible_flag.item_flag.is_show_idle_words && (GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
    {
        OutputIdleWinWords();
    }
#endif  

    //show stk mode text
#ifdef MMI_IDLE_STKTEXT_SUPPORT
    if(idle_mode->visible_flag.item_flag.is_show_stk_idle_text)
    {
        GUI_LCD_DEV_INFO    lcd_dev_info    = *GetCommonIdlePaintLcdDev();
        MAIN_DrawStkIdleModeText(lcd_dev_info.block_id);
    }
#endif

#ifdef UIX8910_ORG_IDLE_STYLE
    MMIIDLE_NotificationUpdateSoftkey();
    MMIIDLE_NotificationFillBottomRect();
    MMK_DrawAllChildCtrl(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID);
#endif
}



/*****************************************************************************/
//  Description : get idle items paint layer ptr
//  Global resource dependence :
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO *GetCommonIdlePaintLcdDev(void)
{
    GUI_LCD_DEV_INFO            *lcd_dev_info_ptr = PNULL;
#ifdef UI_MULTILAYER_SUPPORT
    if(UILAYER_IsLayerActive(&s_common_idle_paint_layer_handle))
    {
        lcd_dev_info_ptr = &s_common_idle_paint_layer_handle;
    }
    else
#endif
    {
        lcd_dev_info_ptr = MMITHEME_GetDefaultLcdDev(); /*lint !e605*/
    }
    return lcd_dev_info_ptr;
}
/*****************************************************************************/
//  Description : get idle items paint layer ptr
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO *GetCommonIdleNotiLcdDev(void)
{
    GUI_LCD_DEV_INFO            *lcd_dev_info_ptr = PNULL;
#ifdef UI_MULTILAYER_SUPPORT
    if(UILAYER_IsLayerActive(&s_common_idle_notification_layer_handle))
    {
        lcd_dev_info_ptr = &s_common_idle_notification_layer_handle;
    }
    else
#endif
    {
        lcd_dev_info_ptr = MMITHEME_GetDefaultLcdDev(); /*lint !e605*/
    }
    return lcd_dev_info_ptr;
}

#ifdef UI_MULTILAYER_SUPPORT

/*****************************************************************************/
//  Description : create idle items paint layer
//  Global resource dependence :
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void CreateCommonIdlePaintLayer(
    MMI_WIN_ID_T      win_id
)
{
    int i = 0;
    uint16   layer_width = 0;
    uint16   layer_height = 0;
    UILAYER_CREATE_T    create_info = {0};
    UILAYER_APPEND_BLT_T        append_layer = {0};

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_common_idle_paint_layer_handle))
    {
        //get tips layer width height
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &layer_width, &layer_height);

        //creat layer
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = layer_width;
        create_info.height = layer_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        UILAYER_CreateLayer(
            &create_info,
            &s_common_idle_paint_layer_handle);

        //set color key
        UILAYER_SetLayerColorKey(&s_common_idle_paint_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);

        append_layer.lcd_dev_info = s_common_idle_paint_layer_handle;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id, MMICOMMON_SOFTKEY_CTRL_ID), &s_common_idle_paint_layer_handle);
        
    }

    if (UILAYER_IsLayerActive(&s_common_idle_paint_layer_handle))
    {
        UILAYER_Clear(&s_common_idle_paint_layer_handle);
    }

}

/*****************************************************************************/
//  Description : release idle paint layer
//  Global resource dependence :
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdlePaintLayer(void)
{
    int i = 0;
    // 释放层
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_common_idle_paint_layer_handle))
    {
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(MAIN_IDLE_WIN_ID,
                              MMICOMMON_SOFTKEY_CTRL_ID),  MMITHEME_GetDefaultLcdDev()); /*lint !e605*/
        
        UILAYER_RELEASELAYER(&s_common_idle_paint_layer_handle); /*lint !e506 !e774 !e831*/
    }
}

/*****************************************************************************/
//  Description : create status bar layer
//  Global resource dependence :
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void CreateCommonIdleStbLayer(
    MMI_WIN_ID_T      win_id
)
{
    uint16 width = 0;
    uint16 height = 0;
    UILAYER_CREATE_T create_info = {0};
    GUI_RECT_T main_rect = MMITHEME_GetFullScreenRect();
    UILAYER_APPEND_BLT_T        append_layer = {0};

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_common_idle_stb_layer_handle))
    {
        width  = main_rect.right - main_rect.left + 1;
        height = MMI_STATUSBAR_HEIGHT;

        create_info.lcd_id          = GUI_MAIN_LCD_ID;
        create_info.owner_handle    = win_id;
        create_info.offset_x        = 0;
        create_info.offset_y        = 0;
        create_info.width           = width;
        create_info.height          = height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format          = UILAYER_MEM_DOUBLE_COPY;

        UILAYER_CreateLayer(&create_info, &s_common_idle_stb_layer_handle);

        UILAYER_SetLayerColorKey(&s_common_idle_stb_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        append_layer.lcd_dev_info = s_common_idle_stb_layer_handle;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);

        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id, MMICOMMON_STATUSBAR_CTRL_ID), &s_common_idle_stb_layer_handle);
        GUISTATUSBAR_SetBgIsDesplay(win_id, TRUE);
    }

    if (UILAYER_IsLayerActive(&s_common_idle_stb_layer_handle))
    {
        UILAYER_Clear(&s_common_idle_stb_layer_handle);
    }
}

/*****************************************************************************/
//  Description : release idle paint layer
//  Global resource dependence :
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdleStbLayer(
    MMI_WIN_ID_T      win_id
)
{
    // 释放层
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_common_idle_stb_layer_handle))
    {
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id,
                              MMICOMMON_STATUSBAR_CTRL_ID),  MMITHEME_GetDefaultLcdDev()); /*lint !e605*/
        UILAYER_RELEASELAYER(&s_common_idle_stb_layer_handle);/*lint !e506 !e774 !e831*/
    }
}

/*****************************************************************************/
//  Description : create notification layer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateCommonIdleNotiLayer(
    MMI_WIN_ID_T      win_id
)
{
    int i = 0;
    uint16   layer_width = 0;
    uint16   layer_height = 0;
    UILAYER_CREATE_T create_info = {0};
    UILAYER_APPEND_BLT_T        append_layer = {0};
    GUI_BOTH_RECT_T  both_rect  = {0};

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_common_idle_notification_layer_handle))
    {
        both_rect = MMITHEME_GetWinClientBothRect(MAIN_IDLE_WIN_ID);
        layer_width = both_rect.v_rect.right - both_rect.v_rect.left + 1;
        layer_height = both_rect.v_rect.bottom - both_rect.v_rect.top;

        create_info.lcd_id          = GUI_MAIN_LCD_ID;
        create_info.owner_handle    = win_id;
        create_info.offset_x        = both_rect.v_rect.left;
        create_info.offset_y        = both_rect.v_rect.top;
        create_info.width           = layer_width;
        create_info.height          = layer_height - FORM_BOTTOM_MAGRIN_HEIGHT;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;
        //create_info.format          = UILAYER_MEM_DOUBLE_COPY;

        UILAYER_CreateLayer(&create_info, &s_common_idle_notification_layer_handle);

        //UILAYER_SetLayerAlpha(&s_common_idle_notification_layer_handle,165);
        append_layer.lcd_dev_info = s_common_idle_notification_layer_handle;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);

        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_NOTI_PARENT_FORM_CTRL_ID), &s_common_idle_notification_layer_handle);
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_PUSH_MSG_LIST_CTRL_ID), &s_common_idle_notification_layer_handle);

    }

    if (UILAYER_IsLayerActive(&s_common_idle_notification_layer_handle))
    {
        UILAYER_Clear(&s_common_idle_notification_layer_handle);
    }
}

/*****************************************************************************/
//  Description : release notification layer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdleNotiLayer(
    MMI_WIN_ID_T      win_id
)
{
    int i = 0;

    // 释放层
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_common_idle_notification_layer_handle))
    {
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id,
                              MMIIDLE_PUSH_MSG_LIST_CTRL_ID),  MMITHEME_GetDefaultLcdDev()); /*lint !e605*/
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id,
                              MMIIDLE_NOTI_PARENT_FORM_CTRL_ID),  MMITHEME_GetDefaultLcdDev()); /*lint !e605*/
        UILAYER_RELEASELAYER(&s_common_idle_notification_layer_handle);/*lint !e506 !e774 !e831*/
    }
}


/*****************************************************************************/
//  Description : create privacy mode notification indicator layer
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CreateCommonIdlePrivacyModeLayer(
    MMI_WIN_ID_T      win_id
)
{
    int i = 0;
    uint16 width = 0;
    uint16 height = 0;
    UILAYER_CREATE_T create_info = {0};
    UILAYER_APPEND_BLT_T        append_layer = {0};

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_common_idle_privacy_mode_layer_handle))
    {
        width  = MMI_IDLE_NTF_AREA_WIDTH;
        height = MMI_IDLE_NTF_AREA_HEIGHT;

        create_info.lcd_id          = GUI_MAIN_LCD_ID;
        create_info.owner_handle    = win_id;
        create_info.offset_x        = 0;
        create_info.offset_y        = MMI_IDLE_NTF_AREA_TOP;
        create_info.width           = width;
        create_info.height          = height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;
        //create_info.format          = UILAYER_MEM_DOUBLE_COPY;

        UILAYER_CreateLayer(&create_info, &s_common_idle_privacy_mode_layer_handle);

        UILAYER_SetLayerColorKey(&s_common_idle_privacy_mode_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);

        append_layer.lcd_dev_info = s_common_idle_privacy_mode_layer_handle;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);

        for(i = 0; i < MMI_IDLE_NTF_INDICATOR_MAX; i++)
        {
            MMI_HANDLE_T    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIIDLE_NTF_ANIM_CTRL_ID_1 + i);
            MMK_SetCtrlLcdDevInfo(ctrl_handle, &s_common_idle_privacy_mode_layer_handle);
        }        

    }

    if (UILAYER_IsLayerActive(&s_common_idle_privacy_mode_layer_handle))
    {
        UILAYER_Clear(&s_common_idle_privacy_mode_layer_handle);
    }

}


/*****************************************************************************/
//  Description : release privacy mode notification indicator  layer
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdlePrivacyModeLayer(MMI_WIN_ID_T win_id)
{
    int i = 0;
    // 释放层
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_common_idle_privacy_mode_layer_handle))
    {
        for(i = 0; i < MMI_IDLE_NTF_INDICATOR_MAX; i++)
        {
            MMI_HANDLE_T    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIIDLE_NTF_ANIM_CTRL_ID_1 + i);
            MMK_SetCtrlLcdDevInfo(ctrl_handle, MMITHEME_GetDefaultLcdDev());
        }
        
        UILAYER_RELEASELAYER(&s_common_idle_privacy_mode_layer_handle); /*lint !e506 !e774 !e831*/
    }
}

#endif


#endif //defined MMI_MSTYLE_SUPPORT



/*Edit by script, ignore 2 case. Thu Apr 26 19:00:55 2012*/ //IGNORE9527

