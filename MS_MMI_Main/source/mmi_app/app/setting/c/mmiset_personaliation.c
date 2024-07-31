/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004      maryxiao       Create
******************************************************************************/

#define _MMISET_PERSONALIATION_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_app_setting_trc.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmiphone_export.h"
#include "mmiset_id.h"
//#include "mmiset_menutable.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmipub.h"
#include "mmiset_text.h"
#include "mmi_appmsg.h"
#include "mmiset_personaliation.h"
#include "guires.h"
#include "mmiset_nv.h"
#include "mmiset_wintab.h"
#include "mmiudisk_export.h"
#include "guiownerdraw.h"
#include "mmiset_image.h"
#include "mmiset_text.h"
#include "guisetlist.h"
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmi_livewallpaper_text.h"
#endif
#include "mmk_tp.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmiset_func.h"
#include "mmi_resource.h"
#include "mmiset_export.h"
#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_text.h"
#include "mmiqbtheme_export.h"
#endif
#ifdef UI_P3D_SUPPORT
#include "mmi_winse.h"
#endif
#include "mmiset_internal.h"
#include "mmiset_lock_effect_internal.h"

//#ifdef PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM
#include "mmiset_mainmenu_export.h"
//#endif

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#include "mmiset_lock_effect_export.h"
#endif
#include "mmicom_panel.h"
#include "mmicc_export.h"
#include "mmipicview_export.h"
#include "mmienvset_internal.h"
#include "mn_type.h"
#include "guitext.h"
#include "mmicom_banner.h"
/*----------------------------------------------------------------------------*/
/*                         Data/Variables                                     */
/*----------------------------------------------------------------------------*/

//personaliation setting
enum {
    ID_PERSONALIATION_LIST_WELCOME,
    ID_PERSONALIATION_LIST_SOUNDS,
    ID_PERSONALIATION_LIST_LOCKSCREEN_BG,
#ifdef MMI_SETTING_POWERON_ANIM
    ID_PERSONALIATION_LIST_POWERON_ANIM,
    ID_PERSONALIATION_LIST_POWEROFF_ANIM,
#endif
#ifdef MMI_COMPILE_E
    ID_PERSONALIATION_LIST_THEME,
#endif 
    ID_PERSONALIATION_LIST_KEYPAD
};
//sound
enum {
    ID_PERSONALIATION_VIBRATE,
    ID_PERSONALIATION_NOTIFICATION_ALERT,
    ID_PERSONALIATION_WARNING_TONE
};
//background
enum {
    ID_PERSONALIATION_WALLPAPERS,
    ID_PERSONALIATION_CHOOSE_FROM_GALLERY
};

//keypad
enum {
    ID_PERSONALIATION_SPEED_DIAL,
    ID_PERSONALIATION_LOCKSCREEN_SHORTCUT
};

LOCAL uint16 lcd_width = 0;
LOCAL uint16 lcd_height = 0;
LOCAL GUI_RECT_T PHONE_VOLUME_BAR_RECT = {0};
LOCAL GUI_RECT_T VOLUME_BAR_RECT = {0};

static int16 volumebar_value = 0;
static int16 volumebar_pos = 0;
static int16 phone_volumebar_value = 0;
static int16 phone_volumebar_pos = 0;

#ifdef MMI_SETTING_POWERON_ANIM
LOCAL BOOLEAN     s_fmm_poweron_call = FALSE;
LOCAL BOOLEAN     s_fmm_poweroff_call = FALSE;
#endif

static BOOLEAN is_control_panel_open = FALSE;
#define SET_VIBRATE_TIMER_OUT                1000    //振动
extern uint8 g_ringtone_max;
extern uint8 g_alert_max;
extern uint8 g_alert_default_id;
extern uint8 g_ringtone_default_id;
/**--------------------------------------------------------------------------*
**                function declaration                                       *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetPersonaliationWindow(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

LOCAL MMI_RESULT_E HandleSetSoundsWindow(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

LOCAL MMI_RESULT_E HandleSetBackgroundWindow(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

LOCAL MMI_RESULT_E HandleSetKeypadWindow(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);


LOCAL void SettingVolumeCB(MMICOM_PANEL_CALLBACK_T *para_ptr);
LOCAL void HandleVolumeBar(BOOLEAN is_increase);
LOCAL void AddVolumeBar();
LOCAL void SetPhoneVolumeCB(MMICOM_PANEL_CALLBACK_T *para_ptr);
LOCAL void AddPhoneVolumeBar();
LOCAL void HandlePhoneVolumeBar(BOOLEAN is_increase);
LOCAL void UpdateKeyVolumeBar(void);
/*****************************************************************************/
//  Description :set vibrate
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL void MMISet_SetVibrateAll(MMI_WIN_ID_T    win_id,BOOLEAN is_on);

/*****************************************************************************/
// the window for select font
WINDOW_TABLE( MMISET_SET_PERSONALIATION_WIN_TAB ) = {
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetPersonaliationWindow ),
    WIN_ID( MMISET_SET_PERSONALIATION_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_PERSONALIATION_CTRL_ID),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),

    END_WIN
};

WINDOW_TABLE( MMISET_SET_SOUNDS_WIN_TAB ) = {
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetSoundsWindow ),
    WIN_ID( MMISET_SET_SOUNDS_WIN_ID ),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_SET_SOUNDS_FORM_CTRL_ID),
    //CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER, MMISET_SET_SOUNDS_FORM_SUB_CTRL_ID,MMISET_SET_SOUNDS_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISET_SET_SOUNDS_LIST0_CTRL_ID, MMISET_SET_SOUNDS_FORM_CTRL_ID),
	CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISET_SET_SOUNDS_LIST1_CTRL_ID, MMISET_SET_SOUNDS_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISET_SET_SOUNDS_LIST2_CTRL_ID, MMISET_SET_SOUNDS_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISET_SET_SOUNDS_LIST3_CTRL_ID, MMISET_SET_SOUNDS_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISET_SET_SOUNDS_LIST4_CTRL_ID, MMISET_SET_SOUNDS_FORM_CTRL_ID),
    //CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_SOUNDS_LIST1_CTRL_ID),
    //CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_SOUNDS_LIST2_CTRL_ID),
    CHILD_PRGBOX_CTRL(TRUE,GUIPRGBOX_STYLE_VOLUME_BAR, MMISET_SET_PHONE_VOL_PROGRESS_CTRL_ID,MMISET_SET_SOUNDS_FORM_CTRL_ID),
#ifndef WIN32
    CHILD_PRGBOX_CTRL(TRUE,GUIPRGBOX_STYLE_KEYPAD_VOLUME_BAR, MMISET_SET_VOL_PROGRESS_CTRL_ID,MMISET_SET_SOUNDS_FORM_CTRL_ID),
#endif
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),

    END_WIN
};

WINDOW_TABLE( MMISET_SET_BACKGROUND_WIN_TAB ) = {
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetBackgroundWindow ),
    WIN_ID( MMISET_SET_BACKGROUND_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_BACKGROUND_LIST_CTRL_ID),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),

    END_WIN
};

WINDOW_TABLE( MMISET_SET_KEYPAD_WIN_TAB ) = {
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetKeypadWindow ),
    WIN_ID( MMISET_SET_KEYPAD_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_KEYPAD_LIST_CTRL_ID),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),

    END_WIN
};

/*****************************************************************************/
//  Description : restore confirm
//  Global resource dependence : none
//  Author: dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWelcomeDialogWin(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
	MMI_STRING_T    	temp_str = {0};
	MMI_HANDLE_T      	ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID );
	char 				str_welcome[256] = {0};
	wchar   			welcome_wstr[127+1] = {0};
	MMICOM_BANNER_INFO_T 	banner_info = {0};
	GUI_POINT_T 			pin_alert_start_point = {0, 0};
	MMI_STRING_T      text_content = {0};

	switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OK_MK, MIDDLE_BUTTON, TRUE);
		//MMIPUB_SetDialogBgLayerStyle(win_id,DIALOG_FULL_BG_SCREEN);
		MMISet_GetWelcomeNote(welcome_wstr);
		GUIEDIT_SetMaxLen(MMIPUB_INPUTFIELD_CTRL_ID,127);//max len
		GUIEDIT_SetString (MMIPUB_INPUTFIELD_CTRL_ID, welcome_wstr,MMIAPICOM_Wstrlen(welcome_wstr));
		break;
	case MSG_FULL_PAINT:
		GUIEDIT_GetString(MMIPUB_INPUTFIELD_CTRL_ID,&text_content);
		if(0 == text_content.wstr_len) {
			GUIWIN_SetSoftkeyBtnState(win_id,1,FALSE,FALSE);
		}
		MMIPUB_SetDialogBgLayerAlpha(win_id, 127);
		GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OK_MK, MIDDLE_BUTTON, TRUE);
		break;
	case MSG_KEYUP_OK:
	case MSG_CTL_OK:
	case MSG_APP_OK:
		MMK_CloseWin(win_id);
		break;
	case MSG_CTL_MIDSK:
		//GUITEXT_GetString(ctrl_handle,&temp_str);
		GUIEDIT_GetString(MMIPUB_INPUTFIELD_CTRL_ID,&temp_str);
		if(temp_str.wstr_ptr!=NULL){
			MMIAPICOM_WstrToStr((wchar*)temp_str.wstr_ptr, (char*)str_welcome);
			SCI_TRACE_LOW("dcg get input text %s",str_welcome);
			MMISet_SetWelcomeNote(temp_str.wstr_ptr);
		}
	    {
			banner_info.banner_type = MMI_BANNER_TEXT;
			banner_info.text1_id = STR_SETT_WELCOME_UPDATED_EXT01;
			banner_info.start_point = pin_alert_start_point;
			MMIAPICOM_OpenBannerWin(win_id, banner_info);

	    }
		MMK_CloseWin(win_id);
		break;
	case MSG_NOTIFY_CANCEL:
    case MSG_APP_CANCEL:
        //MMK_CloseWin(win_id);
        break;
    default:
        result = MMIPUB_HandleDialogInputFieldWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : open personaliation setting shortcut window
//  Global resource dependence :
//  Author: dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_EnterSettingPersonalationWin(void) {
    MMK_CreateWin((uint32 *)MMISET_SET_PERSONALIATION_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :HandleSetSystemSettingWindow
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleSetPersonaliationWindow(
    MMI_WIN_ID_T    win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
) {
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
#ifdef MMI_SETTING_POWERON_ANIM
    MMI_TEXT_ID_T               str_id[] = {STXT_USER_PRG_WAKEUP_MSG,STR_SETT_LIST_SOUNDS_EXT01,STR_SETT_LIST_LOCK_BKGD_EXT01,TXT_SET_POWER_ON_ANIMATION,TXT_SET_POWER_OFF_ANIMATION,
                                            STR_SETT_LIST_THEME_EXT01,STR_SETT_LIST_KEYPAD_EXT01
                                           };
#else
    MMI_TEXT_ID_T               str_id[] = {STXT_USER_PRG_WAKEUP_MSG,STR_SETT_LIST_SOUNDS_EXT01,STR_SETT_LIST_LOCK_BKGD_EXT01,
                                            STR_SETT_LIST_KEYPAD_EXT01
                                           };
#endif
    int16                       i = 0;
    GUILIST_ITEM_T              item_t = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    uint16                      max_items = 0;
    uint16                      cur_item = 0;
    MMI_CTRL_ID_T               ctrl_id = MMISET_SET_PERSONALIATION_CTRL_ID;
    uint32                      time_out = 0;
    MMI_WIN_ID_T                dialog_win_id = MMISET_SET_WELCOME_WIN_ID;
    switch(msg_id) {
        case MSG_OPEN_WINDOW:

            MMK_SetAtvCtrl(win_id,ctrl_id);

#ifdef MMI_SETTING_POWERON_ANIM
            max_items = 7;
#else
            max_items = 4;
#endif
            cur_item  = 0;
            //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d"
            GUILIST_SetMaxItem(ctrl_id,max_items, FALSE );
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
            item_t.item_data_ptr = &item_data;

            for(i = 0; i < max_items; i++) {
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id = str_id[i];

                if(!GUILIST_InsertItem(ctrl_id, &item_t, i)) {
                    break;
                }
            }

            GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, cur_item);
            break;
//         case MSG_FULL_PAINT: {
//             //reset status bar
//             MMI_STRING_T    temp_str = {0};
//             MMI_GetLabelTextByLang(STR_SETT_LIST_PERSONALISATION_EXT01, &temp_str);
//             if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len)) {
//                 GUIWIN_UpdateStb();
//             }
//         }
//         break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        //case MSG_CTL_OK:
        //case MSG_APP_OK:
        //case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            switch (cur_item) {
                case ID_PERSONALIATION_LIST_WELCOME:
                    {
						MMI_STRING_T    temp_str = {0};
						MMI_WIN_PRIORITY_E 			win_priority = WIN_ONE_LEVEL;
						MMI_GetLabelTextByLang(STXT_USER_PRG_WAKEUP_MSG, &temp_str);
						MMIPUB_OpenDialogInputFieldWinByTextPtr(&temp_str,&dialog_win_id,&win_priority,MMIPUB_SOFTKEY_CUSTOMER,HandleWelcomeDialogWin);
                    }
                    break;

                case ID_PERSONALIATION_LIST_SOUNDS://声音设置
                    //to do.....
                    MMK_CreateWin((uint32 *)MMISET_SET_SOUNDS_WIN_TAB, PNULL);
                    break;

                case ID_PERSONALIATION_LIST_LOCKSCREEN_BG:    //背景设置
                    //to do.....
                    MMK_CreateWin((uint32 *)MMISET_SET_BACKGROUND_WIN_TAB, PNULL);
                    break;

#ifdef MMI_SETTING_POWERON_ANIM
                case ID_PERSONALIATION_LIST_POWERON_ANIM:    //背景设置
                    //to do.....
                    MMISet_OpenSetPowerOnAnimWin();
                    break;
                    
                case ID_PERSONALIATION_LIST_POWEROFF_ANIM:    //背景设置
                    //to do.....
                    MMISet_OpenSetPowerOffAnimWin();
                    break;
#endif
#ifdef MMI_COMPILE_E                    
                case ID_PERSONALIATION_LIST_THEME://主题
                    //need MULTI_THEME_SUPPORT
                    //MMISET_CreateThemePubWin();
                    MMISet_OpenThemeWin();
					//GUIMAINMENU_SetColorThemeMode(ctrl_id, TRUE);
                    break;
#endif
                case ID_PERSONALIATION_LIST_KEYPAD:
                    MMK_CreateWin((uint32 *)MMISET_SET_KEYPAD_WIN_TAB, PNULL);
                    break;

                default:
                    break;
            }
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;

}
LOCAL void AddPhoneVolumeBar()
{
    uint16  layer_width = 0;
    uint16  layer_height = 0;

    phone_volumebar_value = MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_MAX, CALL_RING_VOL);

    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &layer_width, &layer_height);

    PHONE_VOLUME_BAR_RECT.left = 0;
    PHONE_VOLUME_BAR_RECT.top = 0;
    PHONE_VOLUME_BAR_RECT.right = layer_width;
    PHONE_VOLUME_BAR_RECT.bottom = PHONE_VOLUME_BAR_RECT.top + 16;

    if(phone_volumebar_value==0 && MMIAPIENVSET_GetActiveModeId()==MMIENVSET_SILENT_MODE)
    {
        phone_volumebar_pos = 0;
    }
#ifdef ZDT_PRODUCT_VIBRATE    
    else if(phone_volumebar_value==0 && MMIAPIENVSET_GetActiveModeId()==MMIENVSET_MEETING_MODE)
    {
        phone_volumebar_pos = 1;
    }
#endif    
    else
    {
        phone_volumebar_pos = phone_volumebar_value + 1;
    }
    GUIPRGBOX_SetRect(MMISET_SET_PHONE_VOL_PROGRESS_CTRL_ID,&PHONE_VOLUME_BAR_RECT);   
    GUIPRGBOX_SetParamByCtrl(MMISET_SET_PHONE_VOL_PROGRESS_CTRL_ID, MMISRVAUD_VOLUME_LEVEL_MAX + 2, MMISRVAUD_VOLUME_LEVEL_MAX + 2);
    GUIPRGBOX_SetPosByCtrl(FALSE,phone_volumebar_pos,phone_volumebar_pos,MMISET_SET_PHONE_VOL_PROGRESS_CTRL_ID);
}
LOCAL void AddVolumeBar() {
    uint16  layer_width = 0;
    uint16  layer_height = 0;

    volumebar_value = MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_MAX, KEY_RING_VOL);

    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &layer_width, &layer_height);

    VOLUME_BAR_RECT.left = 0;
    VOLUME_BAR_RECT.top = 0;
    VOLUME_BAR_RECT.right = layer_width;
    VOLUME_BAR_RECT.bottom = VOLUME_BAR_RECT.top + 16;

    volumebar_pos = volumebar_value;
    GUIPRGBOX_SetRect(MMISET_SET_VOL_PROGRESS_CTRL_ID,&VOLUME_BAR_RECT);
    //CTRLPROGRESSBAR_SetParam(MMISET_SET_VOL_PROGRESS_CTRL_ID,MMISRVAUD_VOLUME_LEVEL_MAX,1);
    GUIPRGBOX_SetParamByCtrl(MMISET_SET_VOL_PROGRESS_CTRL_ID, MMISRVAUD_VOLUME_LEVEL_MAX + 1, MMISRVAUD_VOLUME_LEVEL_MAX + 1);
    GUIPRGBOX_SetPosByCtrl(TRUE,volumebar_pos,volumebar_pos,MMISET_SET_VOL_PROGRESS_CTRL_ID);
    //GUIPRGBOX_SetBgColor(MMISET_SET_VOL_PROGRESS_CTRL_ID, MMI_WINDOW_BACKGROUND_COLOR);
}

LOCAL void SettingVolumeCB(MMICOM_PANEL_CALLBACK_T *para_ptr) {
    if(PNULL == para_ptr) return;

    MMIENVSET_SetCurModeOptValue(KEY_RING_VOL, para_ptr->cur_value, MN_DUAL_SYS_MAX);
    MMIENVSET_SetActiveModeOptValue(KEY_RING_TYPE, MMISET_OTHER_RING1, 0);
    return;
}
LOCAL void SetPhoneVolumeCB(MMICOM_PANEL_CALLBACK_T *para_ptr) 
{
    if(PNULL == para_ptr)
    {
        return;
    }

    if(para_ptr->last_value > 0 && para_ptr->cur_value == 0)
    {
        MMIENVSET_SetPreActiveMode(MMIAPIENVSET_GetActiveModeId());
#ifdef ZDT_PRODUCT_VIBRATE        
        MMIAPIENVSET_ActiveMode(MMIENVSET_MEETING_MODE);
        MMIENVSET_SetCurModeId(MMIENVSET_MEETING_MODE);
#else
        MMIAPIENVSET_ActiveMode(MMIENVSET_SILENT_MODE);
        MMIENVSET_SetCurModeId(MMIENVSET_SILENT_MODE);
#endif
        MMIENVSET_SetCurModeOptValue(CALL_RING_VOL, 0, MN_DUAL_SYS_MAX);
        MMISet_SetNotificationAlert(FALSE);
        UpdateKeyVolumeBar();
#ifdef MMI_MULTI_SIM_SYS_DUAL
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA,MN_DUAL_SYS_2);
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_2);
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_2);
#else
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA,MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA,MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA,MMIAPISET_GetActiveSim());
#endif
    }
    else if(para_ptr->cur_value==0 && MMIAPIENVSET_GetActiveModeId()==MMIENVSET_SILENT_MODE)
    {
        MMIENVSET_SetPreActiveMode(MMIAPIENVSET_GetActiveModeId());
#ifdef ZDT_PRODUCT_VIBRATE        
        MMIAPIENVSET_ActiveMode(MMIENVSET_MEETING_MODE);
        MMIENVSET_SetCurModeId(MMIENVSET_MEETING_MODE);
#else
        MMIAPIENVSET_ActiveMode(MMIENVSET_SILENT_MODE);
        MMIENVSET_SetCurModeId(MMIENVSET_SILENT_MODE);
#endif
        MMIENVSET_SetCurModeOptValue(CALL_RING_VOL, 0, MN_DUAL_SYS_MAX);
        MMISet_SetNotificationAlert(FALSE);
        UpdateKeyVolumeBar();
#ifdef MMI_MULTI_SIM_SYS_DUAL
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA,MN_DUAL_SYS_2);
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_2);
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_2);
#else
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA,MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA,MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA,MMIAPISET_GetActiveSim());
#endif
    }
    #ifdef ZDT_PRODUCT_VIBRATE
    else if(para_ptr->cur_value==0 && MMIAPIENVSET_GetActiveModeId()==MMIENVSET_MEETING_MODE)
    {
        MMIENVSET_SetPreActiveMode(MMIAPIENVSET_GetActiveModeId());
        MMIAPIENVSET_ActiveMode(MMIENVSET_SILENT_MODE);
        MMIENVSET_SetCurModeId(MMIENVSET_SILENT_MODE);
        MMIENVSET_SetCurModeOptValue(CALL_RING_VOL, 0, MN_DUAL_SYS_MAX);
        CTRLLIST_SetToggleState(MMISET_SET_SOUNDS_LIST2_CTRL_ID,0,FALSE);
        UpdateKeyVolumeBar();
#ifdef MMI_MULTI_SIM_SYS_DUAL
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_SILENT,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_SILENT,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_SILENT,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_SILENT,MN_DUAL_SYS_2);
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_SILENT,MN_DUAL_SYS_2);
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_SILENT,MN_DUAL_SYS_2);
#else
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_SILENT,MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_SILENT,MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_SILENT,MMIAPISET_GetActiveSim());
#endif
    }
    #endif
    else
    {
        /*if(para_ptr->last_value == 0 && para_ptr->cur_value > 0 && MMIAPIENVSET_GetActiveModeId() == MMIENVSET_MEETING_MODE)
        {
            MMIENVSET_SetPreActiveMode(MMIAPIENVSET_GetActiveModeId());
            MMIAPIENVSET_ActiveMode(MMIENVSET_STANDARD_MODE);
        }*/
        MMIAPIENVSET_ActiveMode(MMIENVSET_STANDARD_MODE);
        MMIENVSET_SetCurModeId(MMIENVSET_STANDARD_MODE);
        if(MMISet_GetNotificationAlert()==FALSE&&para_ptr->last_value == 0)
        {
            MMISet_SetNotificationAlert(TRUE);
            UpdateKeyVolumeBar();
        }
        //MMIENVSET_SetCurModeOptValue(CALL_RING_VOL, para_ptr->cur_value, MN_DUAL_SYS_MAX);
#ifdef MMI_MULTI_SIM_SYS_DUAL
        MMIENVSET_SetCurModeOptValue(CALL_RING_VOL, para_ptr->cur_value, MN_DUAL_SYS_MAX);
        MMIENVSET_SetCurModeOptValue(MSG_RING_VOL, para_ptr->cur_value, MN_DUAL_SYS_MAX);
        MMIENVSET_SetCurModeOptValue(ALARM_RING_VOL, para_ptr->cur_value, MN_DUAL_SYS_MAX);
        if(MMISET_CALL_VIBRA == MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_1,CALL_RING_TYPE)
            ||MMISET_CALL_VIBRA == MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_2,CALL_RING_TYPE)
            ||MMISET_CALL_VIBRA_AND_RING == MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_1,CALL_RING_TYPE)
            ||MMISET_CALL_VIBRA_AND_RING == MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_2,CALL_RING_TYPE))
        {
            MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA_AND_RING,MN_DUAL_SYS_1);
            MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MN_DUAL_SYS_1);
            MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MN_DUAL_SYS_1);
            MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA_AND_RING,MN_DUAL_SYS_2);
            MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MN_DUAL_SYS_2);
            MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MN_DUAL_SYS_2);
        }
        else
        {
            MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_RING,MN_DUAL_SYS_1);
            MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_RING,MN_DUAL_SYS_1);
            MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_RING,MN_DUAL_SYS_1);
            MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_RING,MN_DUAL_SYS_2);
            MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_RING,MN_DUAL_SYS_2);
            MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_RING,MN_DUAL_SYS_2);
        }
#else
        MMIENVSET_SetCurModeOptValue(CALL_RING_VOL, para_ptr->cur_value, MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(MSG_RING_VOL, para_ptr->cur_value, MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(ALARM_RING_VOL, para_ptr->cur_value, MMIAPISET_GetActiveSim());
        if(MMISET_CALL_VIBRA == MMIAPIENVSET_GetCurModeOptValue(MMIAPISET_GetActiveSim(),CALL_RING_TYPE)
            ||MMISET_CALL_VIBRA_AND_RING == MMIAPIENVSET_GetCurModeOptValue(MMIAPISET_GetActiveSim(),CALL_RING_TYPE))
        {
            MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA_AND_RING,MMIAPISET_GetActiveSim());
            MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MMIAPISET_GetActiveSim());
            MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MMIAPISET_GetActiveSim());
        }
        else
        {
            MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_RING,MMIAPISET_GetActiveSim());
            MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_RING,MMIAPISET_GetActiveSim());
            MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_RING,MMIAPISET_GetActiveSim());
        }
#endif
    }
    return;
}

PUBLIC void IDLE_HASH_Switch_Slient( MMI_WIN_ID_T win_id)
{
		
	uint8 active_mode = MMIAPIENVSET_GetActiveModeId();
	uint8       mode_id     =   0;
	SET_NV_ITEM_E           nv_item_id                  =   MMIENVSET_SETTING_BEGIN;     
	MMIENVSET_SETTING_T     *mode_setting_ptr           =   PNULL;

	#ifdef MMI_MULTI_SIM_SYS_SINGLE
	MN_DUAL_SYS_E        dual_sys = MMIAPISET_GetActiveSim();
	#endif
	
	// 返回当前的环境模式id 
	mode_setting_ptr = SCI_ALLOC_APP(sizeof(MMIENVSET_SETTING_T));
	SCI_MEMSET(mode_setting_ptr,0,sizeof(MMIENVSET_SETTING_T));
	switch (active_mode)
	{
		case MMIENVSET_STANDARD_MODE:  // 普通环境
		#ifdef ZDT_PRODUCT_VIBRATE
		case MMIENVSET_MEETING_MODE:
		#endif
			MMIENVSET_SetPreActiveMode(active_mode);
			MMIAPIENVSET_ActiveMode(MMIENVSET_SILENT_MODE);
			MMIENVSET_SetCurModeId(MMIENVSET_SILENT_MODE);
			MMISet_SetNotificationAlert(FALSE);
			mode_id = MMIENVSET_GetCurModeId();
			MMIAPIENVSET_GetModeValue(mode_id,mode_setting_ptr);
			mode_setting_ptr->call_ring_vol = 0;
			mode_setting_ptr->key_ring_vol = 0;
			
			mode_setting_ptr->alarm_ring_type = MMISET_MSG_SILENT;
			mode_setting_ptr->low_voltage_warn = MMIENVSET_VOLTAGE_WARN_SWITCH_SILENT_MODE;
#ifdef MMI_MULTI_SIM_SYS_DUAL//dong.chunguang add for dual sim
			mode_setting_ptr->call_ring_type[MN_DUAL_SYS_1] = MMISET_CALL_SILENT;
			mode_setting_ptr->call_ring_type[MN_DUAL_SYS_2] = MMISET_CALL_SILENT;
			mode_setting_ptr->msg_ring_type[MN_DUAL_SYS_1] = MMISET_MSG_SILENT;
			mode_setting_ptr->msg_ring_type[MN_DUAL_SYS_2] = MMISET_MSG_SILENT;
#else
			mode_setting_ptr->call_ring_type[dual_sys] = MMISET_CALL_SILENT;
			mode_setting_ptr->msg_ring_type[dual_sys] = MMISET_MSG_SILENT;
#endif
		
		    break;

		case MMIENVSET_SILENT_MODE:      // 安静环境

			MMIENVSET_SetPreActiveMode(active_mode);
			MMIAPIENVSET_ActiveMode(MMIENVSET_STANDARD_MODE);				
			MMIENVSET_SetCurModeId(MMIENVSET_STANDARD_MODE);
			MMISet_SetNotificationAlert(TRUE);
			mode_id = MMIENVSET_GetCurModeId();
			MMIAPIENVSET_GetModeValue(mode_id,mode_setting_ptr);
			
			mode_setting_ptr->call_ring_vol = MMIENVSET_CALL_RING_VOL_STANDARD_MODE;
			mode_setting_ptr->msg_ring_vol = MMIENVSET_MSG_RING_VOL_STANDARD_MODE;
			mode_setting_ptr->alarm_ring_vol = MMIENVSET_ALARM_RING_VOL_STANDARD_MODE;
			mode_setting_ptr->key_ring_vol = MMIENVSET_KEY_RING_VOL_STANDARD_MODE;

			mode_setting_ptr->alarm_ring_type = MMISET_MSG_VIBRA_AND_RING;
			mode_setting_ptr->low_voltage_warn = MMIENVSET_VOLTAGE_WARN_SWITCH_STANDARD_MODE;
			
#ifdef MMI_MULTI_SIM_SYS_DUAL
			mode_setting_ptr->call_ring_type[MN_DUAL_SYS_1] = MMISET_CALL_VIBRA_AND_RING;
			mode_setting_ptr->call_ring_type[MN_DUAL_SYS_2] = MMISET_CALL_VIBRA_AND_RING;
			mode_setting_ptr->msg_ring_type[MN_DUAL_SYS_1] = MMISET_MSG_VIBRA_AND_RING;
			mode_setting_ptr->msg_ring_type[MN_DUAL_SYS_2] = MMISET_MSG_VIBRA_AND_RING;
#else
			mode_setting_ptr->call_ring_type[dual_sys] = MMISET_CALL_VIBRA_AND_RING;
			mode_setting_ptr->msg_ring_type[dual_sys] = MMISET_MSG_VIBRA_AND_RING;
#endif
				
		    break;
	}

	 nv_item_id = MMIENVSET_SETTING_BEGIN + mode_id;/*lint !e64*/

	MMINV_WRITE(nv_item_id,mode_setting_ptr);
	MMIAPI_UpdateCurrentSetingInfo(mode_id,mode_setting_ptr);
	if (PNULL != mode_setting_ptr)/*lint !e774 */
	{
	    SCI_FREE(mode_setting_ptr);
	    mode_setting_ptr = PNULL;
	}

	MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
		
}

LOCAL void UpdatePhoneVolumeBar(void)
{
    phone_volumebar_value = MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_MAX, CALL_RING_VOL);

    if(phone_volumebar_value==0 && MMIAPIENVSET_GetActiveModeId()==MMIENVSET_SILENT_MODE)
    {
        phone_volumebar_pos = 0;
    }
#ifdef ZDT_PRODUCT_VIBRATE    
    else if(phone_volumebar_value==0 && MMIAPIENVSET_GetActiveModeId()==MMIENVSET_MEETING_MODE)
    {
        phone_volumebar_pos = 1;
    }
#endif    
    else
    {
        phone_volumebar_pos = phone_volumebar_value + 1;
    }
    GUIPRGBOX_SetPosByCtrl(FALSE,phone_volumebar_pos,phone_volumebar_pos,MMISET_SET_PHONE_VOL_PROGRESS_CTRL_ID);
    CTRLPROGRESSBAR_Update(MMISET_SET_PHONE_VOL_PROGRESS_CTRL_ID);
    //SetPhoneVolumeCB(&para);
}
LOCAL void UpdateKeyVolumeBar(void)
{
    volumebar_value = MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_MAX, KEY_RING_VOL);

    volumebar_pos = volumebar_value;
    //CTRLPROGRESSBAR_SetParam(MMISET_SET_VOL_PROGRESS_CTRL_ID,MMISRVAUD_VOLUME_LEVEL_MAX,1);
    GUIPRGBOX_SetPosByCtrl(TRUE,volumebar_pos,volumebar_pos,MMISET_SET_VOL_PROGRESS_CTRL_ID);
    CTRLPROGRESSBAR_Update(MMISET_SET_VOL_PROGRESS_CTRL_ID);
    if(MMK_IsOpenWin(MMISET_SET_SOUNDS_WIN_ID)&&
       MMK_GetActiveCtrlId(MMISET_SET_SOUNDS_WIN_ID) == MMISET_SET_PHONE_VOL_PROGRESS_CTRL_ID)
    {
        GUIWIN_SeSoftkeytButtonTextId(MMISET_SET_SOUNDS_WIN_ID, TXT_NULL, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(MMISET_SET_SOUNDS_WIN_ID, TXT_NULL, MIDDLE_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(MMISET_SET_SOUNDS_WIN_ID, STXT_RETURN, RIGHT_BUTTON, TRUE);
    }else if((MMK_IsOpenWin(MMISET_SET_SOUNDS_WIN_ID)&&
       MMK_GetActiveCtrlId(MMISET_SET_SOUNDS_WIN_ID) == MMISET_SET_SOUNDS_LIST2_CTRL_ID))
    {
        GUIWIN_UpdateSoftkey(MMISET_SET_SOUNDS_WIN_ID);
    }
}
LOCAL void HandlePhoneVolumeBar(BOOLEAN is_increase)
{
    MMICOM_PANEL_CALLBACK_T para = {0};
    BOOLEAN should_handle = FALSE;
    para.last_value = MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_MAX, CALL_RING_VOL);
    para.cur_value = para.last_value;
    if(is_increase)
    {
        if (phone_volumebar_pos < MMISRVAUD_VOLUME_LEVEL_MAX + 1)
        {
            should_handle = TRUE;
            phone_volumebar_pos++;
            if(MMIAPIENVSET_GetActiveModeId() == MMIENVSET_SILENT_MODE)
            {
                phone_volumebar_value = 0;
            }
            else
            {
                phone_volumebar_value++;
            }
            para.cur_value = phone_volumebar_value;//para.last_value + 1;
        }
    }
    else
    {
        if (phone_volumebar_pos > 0)
        {
            should_handle = TRUE;
            phone_volumebar_pos--;
            if(phone_volumebar_value > 0)
            {
                phone_volumebar_value--;
            }
            para.cur_value = phone_volumebar_value;//para.last_value - 1;
        }
    }
    if(should_handle)
    {
        GUIPRGBOX_SetPosByCtrl(FALSE,phone_volumebar_pos,phone_volumebar_pos,MMISET_SET_PHONE_VOL_PROGRESS_CTRL_ID);
        CTRLPROGRESSBAR_Update(MMISET_SET_PHONE_VOL_PROGRESS_CTRL_ID);
        SetPhoneVolumeCB(&para);
    }
}
LOCAL void HandleVolumeBar(BOOLEAN is_increase) {
    MMICOM_PANEL_CALLBACK_T para = {0};
    para.last_value = MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_MAX, KEY_RING_VOL);
    para.cur_value = para.last_value;
    SCI_TRACE_LOW("HandleVolumeBar is_increase=%d, volumebar_value=%d, cur_value=%d", is_increase, volumebar_value, para.cur_value);
    if(is_increase) {
        if (volumebar_pos < MMISRVAUD_VOLUME_LEVEL_MAX) {
            volumebar_pos++;
            volumebar_value++;
            para.cur_value = volumebar_value;//para.last_value + 1;
        }
    } else {
        if (volumebar_pos > 0) {
            volumebar_pos--;
            if(volumebar_value > 0) {
                volumebar_value--;
            }
            para.cur_value = volumebar_value;//para.last_value - 1;
        }
    }
    GUIPRGBOX_SetPosByCtrl(FALSE,volumebar_pos,volumebar_pos,MMISET_SET_VOL_PROGRESS_CTRL_ID);
    CTRLPROGRESSBAR_Update(MMISET_SET_VOL_PROGRESS_CTRL_ID);

    SettingVolumeCB(&para);
}

/*****************************************************************************/
//  Description :HandleSetSystemSettingWindow
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleSetSoundsWindow(
    MMI_WIN_ID_T    win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
) {
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    
    MMI_TEXT_ID_T               str_id[] = {STXT_RINGING_TYPE,STXT_VIBRA_OPTIONS,STR_SETT_LIST_NOTIF_ALERT_EXT01,
                                            STXT_WARNING_TONES,STR_SETT_KEYPAD_VOLUME_EXT01};
    int16                       i = 0;
    GUILIST_ITEM_T              item_t = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    uint16                      max_items = 0;
    uint16                      cur_item = 0;
    int16                       ring_id = 0;
    MMI_CTRL_ID_T               ctrl_id = MMISET_SET_SOUNDS_FORM_CTRL_ID;
    MMI_CTRL_ID_T               ctrl0_id = MMISET_SET_SOUNDS_LIST0_CTRL_ID;
    MMI_CTRL_ID_T               ctrl1_id = MMISET_SET_SOUNDS_LIST1_CTRL_ID;
    MMI_CTRL_ID_T               ctrl2_id = MMISET_SET_SOUNDS_LIST2_CTRL_ID;
    MMI_CTRL_ID_T               ctrl3_id = MMISET_SET_SOUNDS_LIST3_CTRL_ID;
    MMI_CTRL_ID_T               ctrl4_id = MMISET_SET_SOUNDS_LIST4_CTRL_ID;
    MMI_CTRL_ID_T               ctrl5_id = MMISET_SET_PHONE_VOL_PROGRESS_CTRL_ID;
    MMI_CTRL_ID_T               ctrl6_id = MMISET_SET_VOL_PROGRESS_CTRL_ID;
    MMI_CTRL_ID_T               active_ctl = MMISET_SET_SOUNDS_LIST0_CTRL_ID;
    GUIFORM_CHILD_HEIGHT_T      child_height = {0};
    MMISET_CALL_RING_T          ring_info        = {MMISET_CALL_RING_FIXED, 0, 0};
    MMISET_MSG_RING_T			msg_ring_info    = {MMISET_MSG_RING_FIXED, 0, 0};
    uint16                      *ring_id_ptr = PNULL;
    uint32 						text_id[MMISET_RING_FIXED_DEFAUT_NUM] = {TXT_COMMON_RING_RING1,TXT_COMMON_RING_RING2,TXT_COMMON_RING_RING3,TXT_COMMON_RING_RING4};
    wchar						*text_wstr[MMISET_RING_FIXED_DEFAUT_NUM] = {RING_NAMES};
    wchar						*text_msg_wstr[MMISET_MSG_FIXED_RING_MAX_ITEM] = {ALERT_NAMES};
    wchar						*text_wstr_ex[MMISET_RING_FIXED_DEFAUT_NUM_EX] = {RING_NAMES_EX};
	wchar						*text_msg_wstr_ex[MMISET_MSG_FIXED_RING_MAX_ITEM_EX] = {ALERT_NAMES_EX};
	MN_DUAL_SYS_E     			mn_dual_sys = MMIAPISET_GetActiveSim();
	BOOLEAN 					is_on;
	uint16                      v_space = 0;
	GUI_RECT_T      			form_rect   = MMITHEME_GetClientRectEx(win_id);
    GUI_BG_T 					normal_bg = {0};
    MMISET_RING_TITLE_INFO_T    orthodox_text_ring[MMISET_ORTHODOX_RING_FIXED_DEFAUT_NUM] = {ORTHODOX_RING_NAMES};

    SCI_TRACE_LOW("HandleSetSoundsWindow msg_id=0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        //MMISET_SET_SOUNDS_LIST0_CTRL_ID
        GUILIST_SetMaxItem(ctrl0_id,1, FALSE);
        GUILIST_SetListState(ctrl0_id, GUILIST_STATE_SPLIT_LINE, FALSE);
        item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
        item_t.item_data_ptr = &item_data;
        GUILIST_AppendItem(ctrl0_id, &item_t);
        //MMISET_SET_SOUNDS_LIST1_CTRL_ID
        
        SCI_MEMSET(&item_t, 0x00,sizeof(item_t));
        SCI_MEMSET(&item_data, 0x00, sizeof(item_data));
        GUILIST_SetMaxItem(ctrl1_id,1, FALSE);
        GUILIST_SetListState(ctrl1_id, GUILIST_STATE_SPLIT_LINE, FALSE);
        item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
        item_t.item_data_ptr = &item_data;
        GUILIST_AppendItem(ctrl1_id, &item_t);
        
        //MMISET_SET_SOUNDS_LIST2_CTRL_ID
        SCI_MEMSET(&item_t, 0x00,sizeof(item_t));
        SCI_MEMSET(&item_data, 0x00, sizeof(item_data));
        GUILIST_SetMaxItem(ctrl2_id,1, FALSE );
        GUILIST_SetListState(ctrl2_id, GUILIST_STATE_SPLIT_LINE, FALSE);
        item_t.item_style    = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
        item_t.item_data_ptr = &item_data;
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = str_id[1];
        item_data.softkey_id[0] = STXT_SOFTKEY_CHANGE;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;
        GUILIST_AppendItem(ctrl2_id, &item_t);
        
        SCI_MEMSET(&item_t, 0x00,sizeof(item_t));
        SCI_MEMSET(&item_data, 0x00, sizeof(item_data));
        GUILIST_SetMaxItem(ctrl3_id,1, FALSE );
        GUILIST_SetListState(ctrl3_id, GUILIST_STATE_SPLIT_LINE, FALSE);
        item_t.item_style    = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
        item_t.item_data_ptr = &item_data;
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = str_id[2];
        item_data.softkey_id[0] = STXT_SOFTKEY_CHANGE;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;
        GUILIST_AppendItem(ctrl3_id, &item_t);
        
        SCI_MEMSET(&item_t, 0x00,sizeof(item_t));
        SCI_MEMSET(&item_data, 0x00, sizeof(item_data));
        GUILIST_SetMaxItem(ctrl4_id,1, FALSE );
        GUILIST_SetListState(ctrl4_id, GUILIST_STATE_SPLIT_LINE, FALSE);
        item_t.item_style    = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
        item_t.item_data_ptr = &item_data;
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = str_id[3];
        item_data.softkey_id[0] = STXT_SOFTKEY_CHANGE;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;
        GUILIST_AppendItem(ctrl4_id, &item_t);
        AddPhoneVolumeBar();//phone volume bar
#ifndef WIN32//for simulator
        AddVolumeBar();//key volume bar
#endif
        GUIFORM_IsSlide(ctrl_id,TRUE);
        GUIFORM_SetType(ctrl_id,GUIFORM_TYPE_COMMON);
        GUIFORM_PermitChildBg(ctrl_id, FALSE);
        GUIFORM_PermitChildBorder(ctrl_id, FALSE);
        GUIFORM_SetMargin(ctrl_id, 0);
        GUIFORM_SetDividingLine(ctrl_id,0,PNULL);
        child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        child_height.add_data=68;
        GUIFORM_SetChildHeight(ctrl_id, ctrl5_id, &child_height);
        child_height.add_data=64;
        GUIFORM_SetChildHeight(ctrl_id, ctrl6_id, &child_height);
#if 0
        normal_bg.bg_type = GUI_BG_COLOR;
        normal_bg.color = MMI_WINDOW_BACKGROUND_COLOR;
        CTRLFORM_SetBg(ctrl_id,&normal_bg);
#endif
        CTRLFORM_SetDividingLine(ctrl_id,0,MMI_WINDOW_BACKGROUND_COLOR);
        GUIFORM_SetSpace(ctrl_id, PNULL, &v_space);
        GUIFORM_SetCircularHandleUpDown(ctrl_id,TRUE);
        GUILIST_SetSelectedItem(ctrl0_id, cur_item, TRUE);
        GUILIST_SetCurItemIndex(ctrl0_id, cur_item);
        break;
    case MSG_GET_FOCUS:
        //UpdatePhoneVolumeBar();
        //UpdateKeyVolumeBar();
        break;
        
    case MSG_LOSE_FOCUS:
        break;
    case MSG_FULL_PAINT:
        {
            //reset status bar
//             MMI_STRING_T    temp_str = {0};
//             MMI_GetLabelTextByLang(STR_SETT_LIST_SOUNDS_EXT01, &temp_str);
//             if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len))
//             {
//                 GUIWIN_UpdateStb();
//             }
            item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
            item_t.item_data_ptr = &item_data;
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id = str_id[0];
            ring_id = (uint8)MMIAPIENVSET_GetFixCallRingId(mn_dual_sys,MMIENVSET_GetCurModeId());
            item_data.softkey_id[0] = STXT_SELECT;
            item_data.softkey_id[1] = TXT_NULL;
            item_data.softkey_id[2] = STXT_RETURN;

            item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
            ring_info = MMIAPIENVSET_GetCallRingInfo(mn_dual_sys,MMIENVSET_GetCurModeId());/*lint !e64*/
            if(ring_info.call_ring_type == MMISET_CALL_RING_MORE_RING)
            {
                wchar       not_suffix_name[MMIFILE_FILE_NAME_MAX_LEN + 1] 	= {0};
                uint16      not_suffix_name_len 							= MMIFMM_FILENAME_LEN;
                uint16 		suffix_wstr[MMIFMM_FILENAME_LEN + 1] 			= {0};
                uint16 		suffix_len                         				= MMIFMM_FILENAME_LEN;
                wchar   	abs_preview_path[MMIFMM_FILE_FULL_PATH_MAX_LEN+1]={0};
                uint16   	abs_preview_path_length = MMIFMM_FILE_FULL_PATH_MAX_LEN;
                //MMIAPIFMM_SplitFullPath(ring_info.more_ring_info.name_wstr,ring_info.more_ring_info.name_wstr_len,
                //						PNULL,PNULL,PNULL,PNULL,not_suffix_name,&not_suffix_name_len);
                if(MMIAPIFMM_IsFileExist(ring_info.more_ring_info.name_wstr,ring_info.more_ring_info.name_wstr_len))
                {
                    MMIAPIFMM_SplitFullPathExt(ring_info.more_ring_info.name_wstr,ring_info.more_ring_info.name_wstr_len,
                        abs_preview_path, &abs_preview_path_length, not_suffix_name, &not_suffix_name_len, suffix_wstr, &suffix_len);
                    item_data.item_content[1].item_data.text_buffer.wstr_ptr = not_suffix_name;
                    item_data.item_content[1].item_data.text_buffer.wstr_len = not_suffix_name_len;
                }
                else
                {//more ring file not exist,reset default ring
#ifdef MMI_MULTI_SIM_SYS_SINGLE
                    MMIAPIENVSET_SetFixedCallRingId(mn_dual_sys,g_ringtone_default_id,MMIENVSET_GetCurModeId());
#else
                    MMIAPIENVSET_SetBothSIMFixedCallRingId(g_ringtone_default_id);
#endif			
                    if(g_ringtone_max == MMISET_RING_FIXED_DEFAUT_NUM)
                    {
                        item_data.item_content[1].item_data.text_buffer.wstr_ptr = text_wstr[g_ringtone_default_id-1];
                        item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(text_wstr[g_ringtone_default_id-1]);
                    }
                    else
                    {
                        item_data.item_content[1].item_data.text_buffer.wstr_ptr = text_wstr_ex[MMISET_RING_DEFAULT_ID_EX-1];
                        item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(text_wstr_ex[MMISET_RING_DEFAULT_ID_EX-1]);
                    }
                }
            }
            else if(ring_info.call_ring_type == MMISET_CALL_RING_FIXED)
            {
                if(g_ringtone_max >= MMISET_RING_FIXED_DEFAUT_NUM)
                {
                    if( (ring_id -1) <= MMISET_RING_FIXED_DEFAUT_NUM)
                    {
                        item_data.item_content[1].item_data.text_buffer.wstr_ptr = text_wstr[ring_id-1];
                        item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(text_wstr[ring_id-1]);
                    }
                    else
                    {
                        if(MMIAPISET_IsOrthodoxVariant())
                        {
                            item_data.item_content[1].item_data.text_buffer.wstr_ptr = orthodox_text_ring[ring_id - MMISET_RING_FIXED_DEFAUT_NUM - 1].title_id;
                            item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(orthodox_text_ring[ring_id - MMISET_RING_FIXED_DEFAUT_NUM - 1].title_id);
                        }
                    }
                }
                else
                {
                    item_data.item_content[1].item_data.text_buffer.wstr_ptr = text_wstr_ex[ring_id-1];
                    item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(text_wstr_ex[ring_id-1]);
                }
            }
            else
            {
                if(g_ringtone_max == MMISET_RING_FIXED_DEFAUT_NUM)
                {
                    item_data.item_content[1].item_data.text_buffer.wstr_ptr = text_wstr[g_ringtone_default_id-1];
                    item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(text_wstr[g_ringtone_default_id-1]);
                }
                else
                {
                    item_data.item_content[1].item_data.text_buffer.wstr_ptr = text_wstr_ex[MMISET_RING_DEFAULT_ID_EX-1];
                    item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(text_wstr_ex[MMISET_RING_DEFAULT_ID_EX-1]);
                }
            }
            item_data.item_content[1].is_opacity = TRUE;
//             GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);
            GUILIST_ReplaceItem(ctrl0_id, &item_t,0);
            
            //MMISET_SET_SOUNDS_LIST1_CTRL_ID
            SCI_MEMSET(&item_t, 0x00,sizeof(item_t));
            SCI_MEMSET(&item_data, 0x00, sizeof(item_data));
            item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
            item_t.item_data_ptr = &item_data;
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id = STR_MESSAGE_ALERT_EXT01;
            ring_id = (uint8)MMIAPIENVSET_GetFixMsgRingId(mn_dual_sys,MMIENVSET_GetCurModeId());
            item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
            msg_ring_info = MMIAPIENVSET_GetMsgRingInfo(mn_dual_sys,MMIENVSET_GetCurModeId());
            if(msg_ring_info.msg_ring_type == MMISET_MSG_RING_MORE_RING)
            {
                wchar       not_suffix_name[MMIFILE_FILE_NAME_MAX_LEN + 1] 	= {0};
                uint16      not_suffix_name_len 							= MMIFMM_FILENAME_LEN;
                uint16 		suffix_wstr[MMIFMM_FILENAME_LEN + 1] 			= {0};
                uint16 		suffix_len                         				= MMIFMM_FILENAME_LEN;
                wchar   	abs_preview_path[MMIFMM_FILE_FULL_PATH_MAX_LEN+1]={0};
                uint16   	abs_preview_path_length = MMIFMM_FILE_FULL_PATH_MAX_LEN;
                //MMIAPIFMM_SplitFullPath(ring_info.more_ring_info.name_wstr,ring_info.more_ring_info.name_wstr_len,
                //						PNULL,PNULL,PNULL,PNULL,not_suffix_name,&not_suffix_name_len);
                if(MMIAPIFMM_IsFileExist(msg_ring_info.more_ring_info.name_wstr,msg_ring_info.more_ring_info.name_wstr_len))
                {
                    MMIAPIFMM_SplitFullPathExt(msg_ring_info.more_ring_info.name_wstr,msg_ring_info.more_ring_info.name_wstr_len,
                        abs_preview_path, &abs_preview_path_length, not_suffix_name, &not_suffix_name_len, suffix_wstr, &suffix_len);
                    item_data.item_content[1].item_data.text_buffer.wstr_ptr = not_suffix_name;
                    item_data.item_content[1].item_data.text_buffer.wstr_len = not_suffix_name_len;
                }
                else
                {//more ring file not exist,reset default ring
#ifdef MMI_MULTI_SIM_SYS_SINGLE
                    MMIAPIENVSET_SetFixedMsgRingId(mn_dual_sys,g_alert_default_id,MMIENVSET_GetCurModeId());
#else
                    MMIAPIENVSET_SetBothSIMFixMsgRingId(g_alert_default_id);
#endif
                    if(g_alert_max == MMISET_MSG_FIXED_RING_MAX_ITEM)
                    {
                        item_data.item_content[1].item_data.text_buffer.wstr_ptr = text_msg_wstr[g_alert_default_id-1];
                        item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(text_msg_wstr[g_alert_default_id-1]);
                    }
                    else
                    {
                        item_data.item_content[1].item_data.text_buffer.wstr_ptr = text_msg_wstr_ex[g_alert_default_id-1];
                        item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(text_msg_wstr_ex[g_alert_default_id-1]);
                    }
                }
            }
            else if(msg_ring_info.msg_ring_type == MMISET_MSG_RING_FIXED&&
                ring_id>0&&(ring_id -1)<=/*MMISET_MSG_FIXED_RING_MAX_ITEM*/g_alert_max)
            {
                if(g_alert_max == MMISET_MSG_FIXED_RING_MAX_ITEM)
                {
                    item_data.item_content[1].item_data.text_buffer.wstr_ptr = text_msg_wstr[ring_id-1];
                    item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(text_msg_wstr[ring_id-1]);
                }
                else
                {
                    item_data.item_content[1].item_data.text_buffer.wstr_ptr = text_msg_wstr_ex[ring_id-1];
                    item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(text_msg_wstr_ex[ring_id-1]);
                }
            }
            else
            {
                if(g_alert_max == MMISET_MSG_FIXED_RING_MAX_ITEM)
                {
                    item_data.item_content[1].item_data.text_buffer.wstr_ptr = text_msg_wstr[g_alert_default_id-1];
                    item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(text_msg_wstr[g_alert_default_id-1]);
                }
                else
                {
                    item_data.item_content[1].item_data.text_buffer.wstr_ptr = text_msg_wstr_ex[g_alert_default_id-1];
                    item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(text_msg_wstr_ex[g_alert_default_id-1]);
                }
            }
            item_data.softkey_id[0] = STXT_SELECT;
            item_data.softkey_id[1] = TXT_NULL;
            item_data.softkey_id[2] = STXT_RETURN;
            item_data.item_content[1].is_opacity = TRUE;
//             GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);
            GUILIST_ReplaceItem(ctrl1_id, &item_t,0);
            GUICTRL_SetProgress(MMK_GetCtrlPtr(ctrl_id),FALSE);//hide scroll bar
            //GUILIST_AppendItem(ctrl1_id, &item_t);
            //GUIFORM_SetVisible(ctrl3_id, TRUE, FALSE);
            if(MMISET_CALL_VIBRA_AND_RING ==(MMISET_CALL_RING_TYPE_E)MMIAPIENVSET_GetActiveModeOptValue(MMIAPISET_GetActiveSim(), CALL_RING_TYPE)||
                MMISET_CALL_VIBRA ==(MMISET_CALL_RING_TYPE_E)MMIAPIENVSET_GetActiveModeOptValue(MMIAPISET_GetActiveSim(), CALL_RING_TYPE))
            {
                CTRLLIST_SetToggleState(ctrl2_id,0,TRUE);
            }
            else
            {
                CTRLLIST_SetToggleState(ctrl2_id,0,FALSE);
            }
            if(MMISet_GetNotificationAlert())//notification alert
            {
                CTRLLIST_SetToggleState(ctrl3_id,0,TRUE);
            }
            else
            {
                CTRLLIST_SetToggleState(ctrl3_id,0,FALSE);
            }
			/*
            if(MMISET_MSG_VIBRA_AND_RING ==(MMISET_CALL_RING_TYPE_E)MMIAPIENVSET_GetActiveModeOptValue(MMIAPISET_GetActiveSim(), MSG_RING_TYPE)){
				CTRLLIST_SetToggleState(ctrl3_id,0,TRUE);
			}else{
				CTRLLIST_SetToggleState(ctrl3_id,0,FALSE);
			}
			*/
            if(MMIAPISET_GetIsOpenVoltageWarning())
            {
                CTRLLIST_SetToggleState(ctrl4_id,0,TRUE);
            }
            else
            {
                CTRLLIST_SetToggleState(ctrl4_id,0,FALSE);
            }
            
            if(MMK_GetActiveCtrlId(win_id) == MMISET_SET_VOL_PROGRESS_CTRL_ID||
                MMK_GetActiveCtrlId(win_id) == MMISET_SET_PHONE_VOL_PROGRESS_CTRL_ID)
            {
                GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
                GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, TRUE);
                GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_RETURN, RIGHT_BUTTON, TRUE);
            }

            if(TRUE == MMIAPISET_IsDisableShareViaOption())	
            {
                GUIFORM_SetChildDisplay(MMK_GetCtrlHandleByWin(win_id, MMISET_SET_SOUNDS_FORM_CTRL_ID), MMK_GetCtrlHandleByWin(win_id, MMISET_SET_SOUNDS_LIST1_CTRL_ID), GUIFORM_CHILD_DISP_HIDE);
            }
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        //case MSG_CTL_OK:
        //case MSG_APP_OK:
        //case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            active_ctl = MMK_GetActiveCtrlId(win_id);
            switch (active_ctl)
            {
            case MMISET_SET_SOUNDS_LIST1_CTRL_ID:
                //todo ....
                msg_ring_info = MMIAPIENVSET_GetMsgRingInfo(mn_dual_sys,MMIENVSET_GetCurModeId());
                MMIAPISET_OpenFixMsgRingListWin(mn_dual_sys,win_id,msg_ring_info.fix_ring_index);
                break;
                
            case MMISET_SET_SOUNDS_LIST0_CTRL_ID:
                //todo ....
                ring_info = MMIAPIENVSET_GetCallRingInfo(mn_dual_sys,MMIENVSET_GetCurModeId());
                MMIAPISET_OpenFixCallRingListWin(mn_dual_sys,win_id,ring_info.fix_ring_index);
                break;
                
            case MMISET_SET_SOUNDS_LIST2_CTRL_ID:
                //to do.....
                cur_item = GUILIST_GetCurItemIndex(active_ctl);
                CTRLLIST_GetToggleState(active_ctl,cur_item,&is_on);
                //MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_CALL);
                //MMIENVSET_StopVibraAndRing(CALL_RING_TYPE);
                MMISet_SetVibrateAll(win_id,is_on);
                break;
            case MMISET_SET_SOUNDS_LIST3_CTRL_ID:
                //to do.....
                cur_item = GUILIST_GetCurItemIndex(active_ctl);
                CTRLLIST_GetToggleState(active_ctl,cur_item,&is_on);
                //notification alert,need sprd provide api
                MMISet_SetNotificationAlert(is_on);//notification
                break;
            case MMISET_SET_SOUNDS_LIST4_CTRL_ID:
                //to do.....
                cur_item = GUILIST_GetCurItemIndex(active_ctl);
                CTRLLIST_GetToggleState(active_ctl,cur_item,&is_on);
                if(is_on == TRUE)
                    MMIENVSET_SetCurModeOptValue(VOLTAGE_WARN,MMIENVSET_VOLTAGE_WARN_SWITCH_OPEN, MMIAPISET_GetActiveSim());
                else
                    MMIENVSET_SetCurModeOptValue(VOLTAGE_WARN,MMIENVSET_VOLTAGE_WARN_SWITCH_CLOSE, MMIAPISET_GetActiveSim());
                break;
                
            case MMISET_SET_VOL_PROGRESS_CTRL_ID:
                //to do.....
                break;
            case MMISET_SET_PHONE_VOL_PROGRESS_CTRL_ID:
                break;
            default:
                break;
            }
            break;
        case MSG_APP_UP:
        case MSG_KEYUP_UP:
        case MSG_APP_DOWN:
        case MSG_KEYUP_DOWN:
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
            break;
        case MSG_APP_RIGHT:
            active_ctl = MMK_GetActiveCtrlId(win_id);
            if(active_ctl==MMISET_SET_VOL_PROGRESS_CTRL_ID)
            {
                HandleVolumeBar(TRUE);
            }
            else if(active_ctl==MMISET_SET_PHONE_VOL_PROGRESS_CTRL_ID)
            {
                HandlePhoneVolumeBar(TRUE);
                MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
            }
            break;
        case MSG_APP_LEFT:
            active_ctl = MMK_GetActiveCtrlId(win_id);
            if(active_ctl==MMISET_SET_VOL_PROGRESS_CTRL_ID)
            {
                HandleVolumeBar(FALSE);
            }
            else if(active_ctl==MMISET_SET_PHONE_VOL_PROGRESS_CTRL_ID)
            {
                HandlePhoneVolumeBar(FALSE);
                MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
            }
            break;
        case MSG_SET_SELECT_FIX_CALL_RING_RETURN:
            if(*(uint16 *)param>0)
            {
                ring_id_ptr = (uint16 *)param;
#ifdef MMI_MULTI_SIM_SYS_SINGLE
                MMIAPIENVSET_SetFixedCallRingId(mn_dual_sys,*ring_id_ptr,MMIENVSET_GetCurModeId());
#else
                MMIAPIENVSET_SetBothSIMFixedCallRingId(*ring_id_ptr);
#endif
            }
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            is_control_panel_open = FALSE;
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;

}

/*****************************************************************************/
//  Description :HandleSetBackgroundWindow
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleSetBackgroundWindow(
    MMI_WIN_ID_T    win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
) {
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
#ifndef MMISET_GALLERY_WALLPAPER_NO_SUPPORT    
    MMI_TEXT_ID_T               str_id[] = {STR_WP_LIST_WALLPAPER_EXT01,STR_SETT_LIST_CHOOSE_GALLERY_EXT01};
    uint16    max_items = 2;
#else
    MMI_TEXT_ID_T               str_id[] = {STR_WP_LIST_WALLPAPER_EXT01};
    uint16    max_items = 1;
#endif
    int16                       i = 0;
    GUILIST_ITEM_T              item_t = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    uint16                      cur_item = 0;
    MMI_CTRL_ID_T               ctrl_id = MMISET_SET_BACKGROUND_LIST_CTRL_ID;
    uint32                      time_out = 0;
    MMI_WIN_ID_T                dialog_win_id = MMIPUB_EDIT_WIN_ID;
    switch(msg_id) {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id,ctrl_id);
            cur_item  = 0;
            //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d"
            GUILIST_SetMaxItem(ctrl_id,max_items, FALSE );
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
            item_t.item_data_ptr = &item_data;

            for(i = 0; i < max_items; i++) {
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id = str_id[i];

                if(!GUILIST_InsertItem(ctrl_id, &item_t, i)) {
                    break;
                }
            }

            GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, cur_item);
            break;
//         case MSG_FULL_PAINT: {
//             //reset status bar
//             MMI_STRING_T    temp_str = {0};
//             MMI_GetLabelTextByLang(STR_SETT_HDR_LOCK_BKGD_EXT01, &temp_str);
//             if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len)) {
//                 GUIWIN_UpdateStb();
//             }
//         }
//         break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        //case MSG_CTL_OK:
        //case MSG_APP_OK:
        //case MSG_APP_WEB:
        case MSG_CTL_MIDSK:

        #ifdef MMISET_GALLERY_WALLPAPER_NO_SUPPORT
            MMIAPISET_SetWallpaper();
        #else
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            switch (cur_item) {
                case ID_PERSONALIATION_WALLPAPERS:
                    MMIAPISET_SetWallpaper();
                    break;

                case ID_PERSONALIATION_CHOOSE_FROM_GALLERY://信息设置
                    //to do.....
                    //MMK_CreateWin((uint32 *)MMISET_SET_SOUNDS_WIN_TAB, PNULL);
                    //MMIAPIPICVIEW_EnterMyPhotos();
                   if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
                   {
                       MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                   }
                   else
                   {
                       MMIAPIPICVIEW_EnterOnlyPhotos();
                   }
                    break;

                default:
                    break;
            }
           #endif
            break;
    case MSG_MULTIM_SELECTED_RETURN:
        {
            MULTIM_SELECT_RETURN_T          *selected_info = (MULTIM_SELECT_RETURN_T *)param;
#ifndef MMI_PIC_CROP_SUPPORT
            MMI_STRING_T wallpaper_label_name = {0};
#endif

#ifdef MMI_PIC_CROP_SUPPORT
            MMIAPIFMM_SetPictureToWallpaperEx(FALSE,selected_info->file_name_ptr, selected_info->file_name_len);

#else
            if (MMIAPIFMM_SetPictureToWallpaper(FALSE,selected_info->file_name_ptr, selected_info->file_name_len))
            {
                wallpaper_label_name.wstr_ptr = selected_info->file_name_ptr;
                wallpaper_label_name.wstr_len = selected_info->file_name_len;
                GUILABEL_SetText(MMISET_DISPLAY_WALLPAPER_LABEL_EDIT_CTRL_ID, &wallpaper_label_name, FALSE);

                MMK_CloseWin(win_id);
            }
            else
            {
                //prompt fail
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
#endif
        }
        break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
              MMIAPISET_SetWallpaperCall(FALSE);
              MMK_CloseWin(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;

}
/*****************************************************************************/
//  Description :HandleSetKeypadWindow
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleSetKeypadWindow(
    MMI_WIN_ID_T    win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
) {
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T               str_id[] = {STR_SETT_LIST_SPEED_DIAL_EXT01,STR_SETT_LIST_LOCK_STCT_EXT01};
    int16                       i = 0;
    GUILIST_ITEM_T              item_t = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    uint16                      max_items = 0;
    uint16                      cur_item = 0;
    MMI_CTRL_ID_T               ctrl_id = MMISET_SET_KEYPAD_LIST_CTRL_ID;
    switch(msg_id) {
        case MSG_OPEN_WINDOW:

            MMK_SetAtvCtrl(win_id,ctrl_id);

            max_items = 2;
            cur_item  = 0;
            //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d"
            GUILIST_SetMaxItem(ctrl_id,max_items, FALSE );
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
            item_t.item_data_ptr = &item_data;

            if(TRUE == MMIAPISET_IsDisableShareViaOption())
            {
                max_items = 1;
            }
            for(i = 0; i < max_items; i++) {
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id = str_id[i];

                if(!GUILIST_InsertItem(ctrl_id, &item_t, i)) {
                    break;
                }
            }

            GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, cur_item);
            break;
//         case MSG_FULL_PAINT: {
//             //reset status bar
//             MMI_STRING_T    temp_str = {0};
//             MMI_GetLabelTextByLang(STR_SETT_LIST_KEYPAD_EXT01, &temp_str);
//             if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len)) {
//                 GUIWIN_UpdateStb();
//             }
//         }
//         break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        //case MSG_CTL_OK:
        //case MSG_APP_OK:
        //case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            switch (cur_item) {
                case ID_PERSONALIATION_SPEED_DIAL://信息设置
                    //to do.....
//                    MMIAPICC_OpenSetSpeedDial(PNULL, 0, 0);
                    MMIAPICC_OpenSpeedDialSettingsWin();
                    break;

                case ID_PERSONALIATION_LOCKSCREEN_SHORTCUT://信息设置
                    //to do.....
//                     MMISET_EnterSoftkeyShortcutWin();
                    break;

                default:
                    break;
            }
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;

}
/*****************************************************************************/
//  Description :get welcome note nv
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/

PUBLIC void MMISet_GetWelcomeNote(wchar * wstr_welcome)
{
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_WELCOME_NOTE, wstr_welcome, nv_result);
    if (MN_RETURN_SUCCESS != nv_result)
    {           
        *wstr_welcome = 0;
        MMINV_WRITE(MMINV_SET_WELCOME_NOTE, "");
    }
}
/*****************************************************************************/
//  Description :set welcome note nv
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/

PUBLIC void MMISet_SetWelcomeNote(wchar * wstr_welcome)
{
    if(wstr_welcome!=PNULL&&MMIAPICOM_Wstrlen(wstr_welcome)>0)
    {
        MMINV_WRITE(MMINV_SET_WELCOME_NOTE, wstr_welcome);
    }
	else
	{
		//set error
        MMINV_WRITE(MMINV_SET_WELCOME_NOTE, "");
	}
}
/*****************************************************************************/
//  Description :get Notification Alert nv
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISet_GetNotificationAlert(void)
{
    MN_RETURN_RESULT_E          return_value    =   MN_RETURN_FAILURE;
    BOOLEAN                     is_on          =   FALSE;
    SET_NV_ITEM_E               nv_item   =  MMINV_SET_NOTIFICATION_ALERT;

    MMINV_READ(nv_item, &is_on, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_on= TRUE;
        MMINV_WRITE(nv_item, &is_on);
    }
    return is_on;
}
/*****************************************************************************/
//  Description :set Notification Alert nv
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC void MMISet_SetNotificationAlert(BOOLEAN is_on)
{
    MN_RETURN_RESULT_E          return_value    =   MN_RETURN_FAILURE;
    BOOLEAN                     is_switch          =   FALSE;
    SET_NV_ITEM_E               nv_item   =  MMINV_SET_NOTIFICATION_ALERT;

    MMINV_WRITE(nv_item, &is_on);
}
/*****************************************************************************/
//  Description :set vibrate
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL void MMISet_SetVibrateAll(MMI_WIN_ID_T    win_id,BOOLEAN is_on)
{
	if (is_on == TRUE&&MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_CALL))
	{
        if(
#ifdef ZDT_PRODUCT_VIBRATE            
            MMIAPIENVSET_GetActiveModeId()==MMIENVSET_MEETING_MODE||
#endif            
            MMIAPIENVSET_GetActiveModeId()==MMIENVSET_SILENT_MODE)
        {
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        MMIAPISET_SetCallRingType(MMISET_CALL_VIBRA, MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA,MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA,MMIAPISET_GetActiveSim());
#else
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA,MN_DUAL_SYS_2);
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_2);
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_2);
#endif
        if(MMIAPIENVSET_GetActiveModeId()==MMIENVSET_SILENT_MODE)
        {
            HandlePhoneVolumeBar(TRUE);
            if(win_id!=NULL)
                MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
        }
        }else{
#ifdef MMI_MULTI_SIM_SYS_SINGLE
		MMIAPISET_SetCallRingType(MMISET_CALL_VIBRA_AND_RING, MMIAPISET_GetActiveSim());
		MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MMIAPISET_GetActiveSim());
		MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MMIAPISET_GetActiveSim());
#else
		//IAPISET_SetCallRingType(MMISET_CALL_VIBRA_AND_RING,MN_DUAL_SYS_1);
		MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA_AND_RING,MN_DUAL_SYS_1);
		MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MN_DUAL_SYS_1);
		MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MN_DUAL_SYS_1);
		MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA_AND_RING,MN_DUAL_SYS_2);
		//IAPISET_SetCallRingType(MMISET_CALL_VIBRA_AND_RING,MN_DUAL_SYS_2);
		MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MN_DUAL_SYS_2);
		MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MN_DUAL_SYS_2);
#endif
        }
		MMIAPIENVSET_SetVibStatus(TRUE);
	}else{
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        if(
#ifdef ZDT_PRODUCT_VIBRATE            
            MMIAPIENVSET_GetActiveModeId()==MMIENVSET_MEETING_MODE||
#endif            
            MMIAPIENVSET_GetActiveModeId()==MMIENVSET_SILENT_MODE)
        {
            MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE, MMISET_CALL_SILENT, MMIAPISET_GetActiveSim());
            MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE, MMISET_MSG_SILENT, MMIAPISET_GetActiveSim());
            MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE, MMISET_MSG_SILENT, MMIAPISET_GetActiveSim());
            HandlePhoneVolumeBar(FALSE);
            if(win_id!=NULL)
                MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
        }
        else
        {
		MMIAPISET_SetCallRingType(MMISET_CALL_RING, MMIAPISET_GetActiveSim());
		MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_RING,MMIAPISET_GetActiveSim());
		MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_RING,MMIAPISET_GetActiveSim());
        }
#else
        if(
#ifdef ZDT_PRODUCT_VIBRATE            
            MMIAPIENVSET_GetActiveModeId()==MMIENVSET_MEETING_MODE||
#endif            
            MMIAPIENVSET_GetActiveModeId()==MMIENVSET_SILENT_MODE)
        {
            MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE, MMISET_CALL_SILENT, MN_DUAL_SYS_1);
            MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE, MMISET_MSG_SILENT, MN_DUAL_SYS_1);
            MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE, MMISET_MSG_SILENT, MN_DUAL_SYS_1);
            MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE, MMISET_CALL_SILENT, MN_DUAL_SYS_2);
            MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE, MMISET_MSG_SILENT, MN_DUAL_SYS_2);
            MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE, MMISET_MSG_SILENT, MN_DUAL_SYS_2);
            HandlePhoneVolumeBar(FALSE);
            if(win_id!=NULL)
                MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
        }
        else
        {
		//IAPISET_SetCallRingType(MMISET_CALL_RING,MN_DUAL_SYS_1);
		MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_RING,MN_DUAL_SYS_1);
		MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_RING,MN_DUAL_SYS_1);
		MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_RING,MN_DUAL_SYS_1);
		//IAPISET_SetCallRingType(MMISET_CALL_RING,MN_DUAL_SYS_2);
		MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_RING,MN_DUAL_SYS_2);
		MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_RING,MN_DUAL_SYS_2);
		MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_RING,MN_DUAL_SYS_2);
        }
#endif
		MMIAPIENVSET_SetVibStatus(FALSE);
	}
}
/*****************************************************************************/
//  Description :MMISet_InitVibrate
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC void MMISet_InitVibrate(void)
{
	BOOLEAN                 is_first_power_on = FALSE;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
	MMINV_READ(MMINV_PHONE_FIRST_POWER_ON, &is_first_power_on, return_value);//读NV
	//first power on,set vibrate is true
	if(MN_RETURN_SUCCESS != return_value||is_first_power_on == TRUE){
		MMISet_SetVibrateAll(NULL,TRUE);
	}
}
