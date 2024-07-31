/*****************************************************************************
 **                         Important Edit History                            *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/2004      maryxiao       Create
 ******************************************************************************/

#define _MMISET_PHONEWIN_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmiphone_export.h"
#include "mmiset_id.h"
#include "mmiset_menutable.h"
#include "mmiconnection_export.h"
#include "mmienvset_export.h"
#include "mmipub.h"
#include "mmiset_text.h"
#include "mmi_appmsg.h"
#include "mmiset_display.h"
#include "mmiset_internal.h"
#include "guires.h"
#include "mmiset_nv.h"
#include "mmiset_wintab.h"
#include "mmiudisk_export.h"
#include "guiownerdraw.h"
#include "mmiset_image.h"
#include "mmiset_text.h"
#include "mmicc_export.h"
#include "guisetlist.h"
#include "mmk_tp.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmiset_func.h"
#include "mmi_resource.h"
#include "mmiset_security.h"
#include "mmiim.h"
#include "mmiset_shortcut.h"
#include "tp_export.h"
#include "mmialarm_export.h"

#include "msensor_drv.h"
#include "guictrl_api.h"
#include "mmiim_im_switching.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef MMI_DUAL_BATTERY_SUPPORT
#include "dualbat_drvapi.h"
#endif
#include "mmiidle_statusbar.h"
#include "mmicom_statusarea.h"
#ifdef WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#ifdef JAVA_SUPPORT_IA
#include "mmijava_export.h"
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif
#ifdef SCREENSAVER_SUPPORT
#include "mmiss_export.h"
#endif
#include "mmi_menutable.h"
#include "mmiset_export.h"
#ifdef BROWSER_SUPPORT
#include "browser_manager_text.h"
#endif
#include "mmieng_export.h"
#include "mmi_nv.h"
#ifdef BLUETOOTH_SUPPORT
#include "mmibt_export.h"
#endif

#include "mmiset_notifications.h"
#include "mmiset_about.h"
#include "mmiset_personaliation.h"
#ifdef IKEYBACKUP_SUPPORT
#include "mmiikeybackup_api.h"
#endif
#ifdef NPS_SUPPORT
#include "nps_export.h"
#endif
#include "mmipicview_export.h"
#include "mmimms_export.h"
#include "mmimms_setting.h"
#ifdef RECORD_SUPPORT
#include "mmirecord_main.h"
#endif
#ifdef MSA_SUPPORT
#include "mmimsa_export.h"
#endif
#include "mmicom_banner.h"
#include "variant_config_nv.h"
#ifdef FOTA_SUPPORT_ABUP
#include "guitext.h"
typedef enum  {   
	FOTA_STATE_CV,    
	FOTA_STATE_DL,    
	FOTA_STATE_RD,    
	FOTA_STATE_RU,
	FOTA_STATE_RG,
	FOTA_STATE_ERROR,
	FOTA_STATE_LAST_VER
}FotaDisplayState;
LOCAL MMI_RESULT_E CheckUpdateWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
/**--------------------------------------------------------------------------*
 **                function declaration                                       *
 **--------------------------------------------------------------------------*/
LOCAL uint8  s_setting_cur_colortheme_index = 0;
LOCAL GUI_RECT_T   color_theme_rect[MMITHEME_ACCENT_COLOR_MAX] = {0};
LOCAL MMISET_SECURITY_CODE_T s_setting_security_code = {0};
LOCAL BOOLEAN   s_setting_privacy_mode = FALSE;
LOCAL BOOLEAN   s_setting_is_pwd_right = FALSE;
extern PUBLIC void SetCurColorIndex(uint8 cur_color_theme_index);

/*****************************************************************************/
//  Description : to handle select language
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetLanguageWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );
//add by nhl for LanguageWindow
LOCAL MMI_RESULT_E  HandleSetLanguageWindow2(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );
/*****************************************************************************/
//  Description :显示语言设置的窗口处理函数
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetLanguageContentWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

/*****************************************************************************/
//  Description :输入语言设置的窗口处理函数
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetLanguageInputContentWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

#if 0
/*****************************************************************************/
//  Description : 编码方式设置界面
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetCodeType(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );
#endif

#ifdef LCD_SLIDE_SUPPORT

/*****************************************************************************/
//  Description : to handle slide operation window message
//  Global resource dependence :
//  Author:jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSlideOperationWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

/*****************************************************************************/
//  Description : slide open
//  Global resource dependence :
//  Author:jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetSlideOpenTypeWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

/*****************************************************************************/
//  Description : slide close
//  Global resource dependence :
//  Author:jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetSlideCloseTypeWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

/*****************************************************************************/
//  Description : keylock switch
//  Global resource dependence :
//  Author:jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetKeylockSwitchTypeWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

#endif

#ifdef MOTION_SENSOR_TYPE
/*****************************************************************************/
//  Description : open set sensor win
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void OpenSetSensorWin(void);

/*****************************************************************************/
//  Description : Append the item of sensor list
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void AppendSensorListItem(
        MMI_CTRL_ID_T    ctrl_id //in
        );

/*****************************************************************************/
//  Description : handle set sensor windows messages
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetSensorWindow(
        MMI_WIN_ID_T      win_id,
        MMI_MESSAGE_ID_E  msg_id,
        DPARAM            param
        );
/*****************************************************************************/

//  Description :
//  Global resource dependence :
//  Author:maryxiao
//  Note:
/*****************************************************************************/
//CR227614  maryxiao
LOCAL MMI_RESULT_E HandleSetSensorAutorotaionWindow(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

#endif

/*****************************************************************************/
//  Description : handle input reset factory password win msg
//  Global resource dependence :
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInputResetFactoryPwd(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );
#ifndef MMI_PHONESET_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle prompt win of clean user data
//  Global resource dependence :
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePromptCompleteWin(
        MMI_WIN_ID_T    win_id, // 窗口的ID
        MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
        DPARAM             param   // 相应消息的参数
        );

#endif
/*****************************************************************************/
//  Description : 处理恢复出厂设置和清楚用户数据的等待窗口
//  Global resource dependence :
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleResetOrCleanDataWaitWin(
        MMI_WIN_ID_T    win_id, // 窗口的ID
        MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
        DPARAM             param   // 相应消息的参数
        );

#ifndef FLASH_SUPPORT
/*****************************************************************************/
//  Description : handle prompt win of theme
//  Global resource dependence :
//  Author: applezhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePromptCompleteThemeWin(
        MMI_WIN_ID_T    win_id, // 窗口的ID
        MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
        DPARAM             param   // 相应消息的参数
        );
#endif



/*****************************************************************************/
//  Description : set the language settings form ctrl param
//  Global resource dependence :
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void SetLanguageFormCtrlParam(void);
#ifndef MMI_PHONESET_MINI_SUPPORT
/*****************************************************************************/
//  Description : 编码方式选项:1、GB(简体中文) 2、Big5(繁体中文)
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void AppendCodeTypeListItem(
        MMI_CTRL_ID_T      ctrl_id
        );



/*****************************************************************************/
//  Description : 设置编码方式
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetCodeType(
        MMI_CTRL_ID_T      ctrl_id
        );
/*****************************************************************************/
//  Description : to handle phone setting window message
//  Global resource dependence :
//  Author:xiangjie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetPhoneWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );
#endif
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
/*****************************************************************************/
//  Description :HandleSetPowerSaveWindow
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetPowerSaveWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

/*****************************************************************************/
//  Description :HandlePowerSaveScheduleWindow
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePowerSaveScheduleWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

/*****************************************************************************/
//  Description :SetPowerSaveParam
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL void SetPowerSaveParam(void);

/*****************************************************************************/
//  Description :AppendPowerSavingOnOffList
//  Global resource dependence :
//  Author:juan.wu
//  Note: append 省电模式列表
/*****************************************************************************/
LOCAL void AppendTwoTextCheckList(MMI_CTRL_ID_T ctrl_id, BOOLEAN is_update);
/*****************************************************************************/
//  Description :AppendPowerSavingSettingList
//  Global resource dependence :
//  Author:juan.wu
//  Note: 省电模式设置列表
/*****************************************************************************/
LOCAL void AppendOneTextOneIconList(
        MMI_CTRL_ID_T ctrl_id,
        MMI_TEXT_ID_T text_id,
        MMI_IMAGE_ID_T image_id
        );

/*****************************************************************************/
//  Description :HandlePowerSaveSettingWindow
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePowerSaveSettingWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

/*****************************************************************************/
//  Description :AppendTwoTextOneIconList
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL void AppendTwoTextOneIconList(
        MMI_CTRL_ID_T ctrl_id,
        MMI_TEXT_ID_T text1,
        MMI_TEXT_ID_T text2,
        MMI_IMAGE_ID_T  image_id,
        MMI_WIN_ID_T   win_id,
        uint16  pos,
        BOOLEAN is_update
        );
/*****************************************************************************/
//  Description :GetIconID
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T  GetIconID(BOOLEAN is_checked);

/*****************************************************************************/
//  Description :GetTxtID
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T   GetTxtID(MMISET_POWER_SAVING_SETTING_E index);

/*****************************************************************************/
//  Description :UpdateStatusBar
//  Global resource dependence :
//  Author:juan.wu
//  Note: 刷新状态栏
/*****************************************************************************/
LOCAL void UpdateStatusBar(BOOLEAN is_open_ps_mode);

/*****************************************************************************/
//  Description :DealWithListCheckMsg
//  Global resource dependence :
//  Author:juan.wu
//  Note: 处理list 的check消息
/*****************************************************************************/
LOCAL void DealWithListCheckMsg(MMI_CTRL_ID_T ctrl_id);
#endif
/*****************************************************************************/

//  Description :
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetPhoneSettingWindow(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetAddNewMSAWindow(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

/*****************************************************************************/
//  Discription: append one line text radio list item
//  Global resource dependence: none
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void MMISET_AppenTwoLineTextListItem(
        MMI_CTRL_ID_T    ctrl_id,
        MMI_TEXT_ID_T    text_id_1,
        MMI_TEXT_ID_T    text_id_2,
        MMI_STRING_T     *str_1,
        MMI_STRING_T     *str_2,
        uint16           pos
        );

/*****************************************************************************/
//  Discription: append one line text radio list item
//  Global resource dependence: none
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void MMISET_AppenOneLineTextListItem(
        MMI_CTRL_ID_T    ctrl_id,
        MMI_TEXT_ID_T    text_id,
        uint16           pos
        );

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL void MMISET_AppenOneLineTextToggleListItem(
        MMI_CTRL_ID_T    ctrl_id,
        MMI_TEXT_ID_T    text_id,
        uint16           pos
        );

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL void MMISET_AppendSetMSAItem(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetMSAWinMsg(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

/*****************************************************************************/
//  Description : handle list preview option win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetMSAlistOptWinMsg(
        MMI_WIN_ID_T        win_id,
        MMI_MESSAGE_ID_E    msg_id,
        DPARAM              param
        );
#ifdef MMI_GUI_STYLE_MINISCREEN
/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence :
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void AppendPdaPhoneSettingsItem1Line(
        MMI_CTRL_ID_T  ctrl_id,    //控件ID
        MMI_TEXT_ID_T  text_1,     //主文本
        MMI_IMAGE_ID_T image_id,   //icon
        uint32         menu_id   //用户数据
        );
/*****************************************************************************/
//  Description : InitPdaCallSettingsCtrl
//  Global resource dependence :
//  Author:yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void  InitPdaPhoneSettingsCtrl(void);
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetMainWindow(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetSystemSettingWindow(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );
/*****************************************************************************/
//  Description :HandleSetTimeLanguageWindow
//  Global resource dependence :
//  Author:tangting
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetTimeLanguageWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetPowerManagerWindow(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetBatteryStatusWindow(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetGprsCrWindow(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetStorageWindow(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetPreferStorageWindow(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );
/*****************************************************************************/
//  Description :start stk
//  Global resource dependence :
//  Note:
/*****************************************************************************/

LOCAL void MMISET_StartSTK (void);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetPhoneLockWindow(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );

#ifdef UNISOC_TEST_ENTRY_FOR_TESTER
/*****************************************************************************/
//  Description : temp test entry for tester
//  Global resource dependence :
//  Author:pan.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTestEntryWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        );
/*****************************************************************************/
//  Description : temp test entry for tester
//  Global resource dependence : 
//  Author:pan.yang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetSecurityCodeWindow(
        MMI_WIN_ID_T    win_id, 
        MMI_MESSAGE_ID_E   msg_id, 
        DPARAM             param
        );
#endif
/*****************************************************************************/
//  Description : to handle display color theme setting window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetDisplayColorThemeWindow(
        MMI_WIN_ID_T    win_id, 
        MMI_MESSAGE_ID_E   msg_id, 
        DPARAM             param
        );


#ifdef MMI_VOLTE_SUPPORT
LOCAL MMI_RESULT_E  HandleSetVoLTEWindow(
        MMI_WIN_ID_T    win_id, 
        MMI_MESSAGE_ID_E   msg_id, 
        DPARAM             param
        );
#endif
#if defined(MMI_2G_4G_SUPPORT)
LOCAL MMI_RESULT_E Set2G4GNetWorkWinHandleMsg(
        MMI_WIN_ID_T    win_id, 
        MMI_MESSAGE_ID_E   msg_id, 
        DPARAM             param
        );
LOCAL MMI_RESULT_E  HandleSimWinHandleMsg(
        MMI_WIN_ID_T    win_id, 
        MMI_MESSAGE_ID_E   msg_id, 
        DPARAM             param
        );
#endif

/*****************************************************************************/
//  Description : get token cb func
//  Global resource dependence :
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void MMISET_MSA_Get_Token_Func( int result, char*  buf, uint16 buflen);

/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
#ifdef MMI_COMPILE_E
WINDOW_TABLE( MMISET_DISPLAY_COLOR_THEME_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetDisplayColorThemeWindow ),    
    WIN_ID( MMISET_DISPLAY_COLOR_THEME_ID ),
    //WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

//the window for input reset factory setting password
WINDOW_TABLE( MMISET_INPUT_RESET_FACTORY_PWD_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleInputResetFactoryPwd),
    WIN_ID( MMISET_INPUT_RESET_FACTORY_PWD_WIN_ID ),
    WIN_TITLE( TXT_INPUT_PHONE_PASSWORD ),//cr227782 maryxiao
    CREATE_EDIT_PASSWORD_CTRL(MMISET_POWER_ON_PWD_MAX_LEN,MMISET_INPUT_RESET_FACTORY_PWD_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
    END_WIN
};

#ifdef MOTION_SENSOR_TYPE
WINDOW_TABLE( MMISET_SET_SENSOR_AUTOROTATION_WIN_TAB ) = //CR227614 MARYXIAO
{
    WIN_ID(MMISET_SET_SENSOR_AUTOROTATION_WIN_ID),
    WIN_FUNC((uint32) HandleSetSensorAutorotaionWindow),
    WIN_TITLE(TXT_SET_SENSOR_LANDSCAPE),
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMISET_SET_SENSOR_AUTOROTATION_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_FINISH),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_FINISH),
#endif
    END_WIN
};
#ifdef DPHONE_SUPPORT
WINDOW_TABLE( MMISET_SET_SENSOR_WIN_TAB ) =
{
    WIN_ID(MMISET_SET_SENSOR_WIN_ID),
    WIN_FUNC((uint32) HandleSetSensorWindow),
    WIN_TITLE(TXT_SET_SENSOR_RESPONDENT),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMISET_SET_SENSOR_CTRL_ID),
    END_WIN
};
#else
WINDOW_TABLE( MMISET_SET_SENSOR_WIN_TAB ) = //CR 227614 maryxiao
{
    WIN_STATUSBAR,
    WIN_ID(MMISET_SET_SENSOR_WIN_ID),
    WIN_FUNC((uint32) HandleSetSensorWindow),
    WIN_TITLE(TXT_SET_SENSOR_RESPONDENT),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_SENSOR_FORM_CTRL_ID),
#ifdef MMI_RES_ORIENT_BOTH
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SENSOR_AUTO_ROTATE_FORM_CTRL_ID,MMISET_SENSOR_FORM_CTRL_ID),//AUTOMATIC ROTATE
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_SENSOR_AUTO_ROTATE_LABEL_CTRL_ID,MMISET_SENSOR_AUTO_ROTATE_FORM_CTRL_ID),
#endif

    END_WIN
};
#endif
#endif
#ifdef LCD_SLIDE_SUPPORT

// the window for slide operation
WINDOW_TABLE( MMISET_SLIDE_OPERATION_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSlideOperationWindow ),
    WIN_ID( MMISET_SET_SLIDE_OPERATION_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_SET_SLIDE_OPERATION_OPTION, MMISET_SET_SLIDE_OPERATION_MENU_CTRL_ID),
    END_WIN
};

//the window for slide open
WINDOW_TABLE(MMISET_SET_SLIDE_OPEN_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleSetSlideOpenTypeWindow ),
    WIN_ID(MMISET_SET_SLIDE_OPEN_WIN_ID),
    WIN_TITLE( TXT_SET_SLIDE_OPEN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_SLIDE_OPEN_TYPE_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the window for slide close
WINDOW_TABLE(MMISET_SET_SLIDE_CLOSE_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleSetSlideCloseTypeWindow ),
    WIN_ID(MMISET_SET_SLIDE_CLOSE_WIN_ID),
    WIN_TITLE( TXT_SET_SLIDE_CLOSE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_SLIDE_CLOSE_TYPE_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the window for keylock switch
WINDOW_TABLE(MMISET_SET_KEYLOCK_SWITCH_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleSetKeylockSwitchTypeWindow ),
    WIN_ID(MMISET_SET_KEYLOCK_SWITCH_WIN_ID),
    WIN_TITLE( TXT_SET_KEYLOCK_SWITCH),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_KEYLOCK_SWITCH_TYPE_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

#endif

/*WINDOW_TABLE( MMISET_SET_TXT_CODE_TYPE_WIN_TAB ) =
  {
//CLEAR_LCD,
//WIN_PRIO( WIN_ONE_LEVEL ),
WIN_FUNC( (uint32)HandleSetCodeType ),
WIN_ID( MMISET_SET_CODE_TYPE_WIN_ID ),
WIN_TITLE( TXT_CODING_TYPE ),
CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_TXT_CODE_TYPE_CTRL_ID),
WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
END_WIN
};*/


WINDOW_TABLE( MMISET_SET_INPUT_LANGUAGE_CONTENT_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
    WIN_FUNC( (uint32)HandleSetLanguageInputContentWindow),
    WIN_ID( MMISET_SET_INPUT_LANGUAGE_CONTENT_WIN_ID ),
    WIN_TITLE( TXT_INPUT_LANGUAGE ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_INPUT_LANGUAGE_CONTENT_LIST_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};


WINDOW_TABLE( MMISET_SET_SHOW_LANGUAGE_CONTENT_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
    WIN_FUNC( (uint32)HandleSetLanguageContentWindow),
    WIN_ID( MMISET_SET_SHOW_LANGUAGE_CONTENT_WIN_ID ),
    WIN_TITLE( TXT_SET_SHOW_LANGUAGE ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_SHOW_LANGUAGE_CONTENT_LIST_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

// the window for set language
WINDOW_TABLE( MMISET_SET_LANGUAGE_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
    WIN_FUNC( (uint32)HandleSetLanguageWindow ),
    WIN_ID( MMISET_SET_LANGUAGE_WIN_ID ),
    WIN_TITLE( TXT_SET_LANGUAGE_SETTINGS ),
#ifdef DPHONE_SUPPORT
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_LANGUAGE_CTRL_ID),
#else
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_LANGUAGE_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_LANGUAGE_SHOW_FORM_CTRL_ID,MMISET_LANGUAGE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_LANGUAGE_SHOW_LABEL_CTRL_ID,MMISET_LANGUAGE_SHOW_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE, MMISET_LANGUAGE_SHOW_LABEL_CONTENT_CTRL_ID, MMISET_LANGUAGE_SHOW_FORM_CTRL_ID),

    #if 0//delete by James li
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_LANGUAGE_INPUT_FORM_CTRL_ID,MMISET_LANGUAGE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_LANGUAGE_INPUT_LABEL_CTRL_ID,MMISET_LANGUAGE_INPUT_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE, MMISET_LANGUAGE_INPUT_LABEL_CONTENT_CTRL_ID, MMISET_LANGUAGE_INPUT_FORM_CTRL_ID),
    #endif
#endif

    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    END_WIN
};
//add by nhl for settings language
WINDOW_TABLE( MMISET_SET_LANGUAGE_WIN2_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),

    WIN_FUNC( (uint32)HandleSetLanguageWindow2 ),
    WIN_ID( MMISET_SET_LANGUAGE2_WIN_ID ),
    //WIN_TITLE( TXT_SET_LANGUAGE_SETTINGS ),

    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_LANGUAGE_FORM_CTRL_ID2),
    //modify by nhl for softkey style
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    END_WIN
};
#ifndef MMI_PHONESET_MINI_SUPPORT
// the window for phone setting
   WINDOW_TABLE( MMISET_SET_PHONE_WIN_TAB ) =
   {
//CLEAR_LCD,
//WIN_PRIO( WIN_ONE_LEVEL ),
WIN_STATUSBAR,
WIN_FUNC( (uint32)HandleSetPhoneWindow ),
WIN_ID( MMISET_SET_PHONE_WIN_ID ),
WIN_TITLE(TXT_NULL),
WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
CREATE_MENU_CTRL(MENU_SET_PHONE, MMISET_SET_PHONE_MENU_CTRL_ID),
END_WIN
};
#if 0
//remove title
WINDOW_TABLE( MMISET_SET_PHONE_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetMainWindow ),
    WIN_ID( MMISET_SET_PHONE_WIN_ID ),
#ifdef MMI_SUPPORT_CONTROLPANEL//Queen project no need
    WIN_SOFTKEY(STXT_MAIN_SETTING, STXT_SELECT, TXT_COMMON_COMMON_BACK),
#else
    WIN_SOFTKEY(NULL, STXT_SELECT, TXT_COMMON_COMMON_BACK),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_MAIN_LIST_CTRL_ID),
    END_WIN
};
//end
#endif
// the window for phone setting
WINDOW_TABLE( MMISET_ICON_PHONE_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetPhoneWindow ),
    WIN_ID( MMISET_SET_PHONE_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_ICON_SET_PHONE, MMISET_SET_PHONE_MENU_CTRL_ID),
    END_WIN
};
#endif
// the window for phone setting
WINDOW_TABLE( MMISET_PHONE_SETTING_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
    WIN_FUNC( (uint32)HandleSetPhoneSettingWindow ),
    WIN_ID( MMISET_PHONE_SETTING_WIN_ID ),
#ifdef MMI_GUI_STYLE_MINISCREEN
    WIN_TITLE(TXT_SET_PHONE),
#else
    WIN_TITLE(TXT_NULL),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#ifdef MMI_GUI_STYLE_MINISCREEN
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_PHONE_SETTING_MENU_CTRL_ID),
#else
    CREATE_MENU_CTRL(MENU_SET_PHONE_SETTING, MMISET_PHONE_SETTING_MENU_CTRL_ID),
#endif
    END_WIN
};
#ifdef MSA_SUPPORT 
WINDOW_TABLE( MMISET_MSA_LIST_OPT_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSetMSAlistOptWinMsg),
    WIN_ID(MMISET_MSA_LIST_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT|WS_DISABLE_COMMON_BG),
    CREATE_POPMENU_CTRL(MENU_SET_MSA_LIST_OPT,MMISET_MSA_LIST_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SELECT,STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMISET_SET_ADD_NEW_MSA_WIN_TAB ) =
{
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
    WIN_FUNC( (uint32)HandleSetAddNewMSAWindow ),
    WIN_ID( MMISET_ADD_NEW_MSA_WIN_ID ),

    CREATE_BUTTON_CTRL(PNULL, MMISET_ADD_NEW_MSA_CTRL_ID),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),

    END_WIN
};

WINDOW_TABLE( MMISET_SET_MSA_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSetMSAWinMsg),
    WIN_ID( MMISET_MSA_WIN_ID ),

    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_MSA_MENU_CTRL_ID),

//    WIN_SOFTKEY(STXT_SOFTKEY_MENU_MK ,STXT_SELECT,TXT_COMMON_COMMON_BACK),
    WIN_SOFTKEY(STXT_OPTION, STXT_SELECT, STXT_RETURN),
    END_WIN
};
#endif
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
WINDOW_TABLE( MMISET_POWER_SAVE_WIN_TAB ) =
{
    WIN_STATUSBAR,
    WIN_FUNC((uint32)HandleSetPowerSaveWindow),
    WIN_ID(MMISET_POWER_SAVE_WIN_ID),
    WIN_TITLE(TXT_ENERGY_SAVING),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_POWER_SAVE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_POWER_SAVE_TITLE_CTRL_ID, MMISET_POWER_SAVE_FORM_CTRL_ID),
    //CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISET_POWER_SAVE_ONOFF_FORM_CTRL_ID, MMISET_POWER_SAVE_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISET_POWER_SAVE_ONOFF_LIST_CTRL_ID, MMISET_POWER_SAVE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_POWER_TITLE_LABEL_CTRL_ID, MMISET_POWER_SAVE_FORM_CTRL_ID),

#ifndef MMI_BATTERY_PERCENT_IN_STATUSBAR//双电池 电池容量分为主副电池
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMISET_POWER_LEFT_FORM_CTRL_ID, MMISET_POWER_SAVE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_POWER_LEFT_TITLE_CTRL_ID, MMISET_POWER_LEFT_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_POWER_CONTENT_CTRL_ID, MMISET_POWER_LEFT_FORM_CTRL_ID),
#ifdef MMI_DUAL_BATTERY_SUPPORT
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMISET_AUX_POWER_LEFT_FORM_CTRL_ID, MMISET_POWER_SAVE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_AUX_POWER_LEFT_TITLE_CTRL_ID, MMISET_AUX_POWER_LEFT_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_AUX_POWER_CONTENT_CTRL_ID, MMISET_AUX_POWER_LEFT_FORM_CTRL_ID),
#endif
#else
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISET_POWER_PERCENT_DIS_LIST_CTRL_ID, MMISET_POWER_SAVE_FORM_CTRL_ID),
#endif
    END_WIN
};

WINDOW_TABLE( MMISET_POWER_SAVE_SETTING_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandlePowerSaveSettingWindow),
    WIN_ID(MMISET_POWER_SAVE_SETTING_WIN_ID),
    WIN_TITLE(TXT_SET_POWER_SAVING_SEETING),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#else
    WIN_STATUSBAR,
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_POWER_SAVE_SETTING_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISET_POWER_SAVE_SETTING_LIST1_CTRL_ID, MMISET_POWER_SAVE_SETTING_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISET_POWER_SAVE_SETTING_BACKLIGHT_FORM_CTRL_ID, MMISET_POWER_SAVE_SETTING_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_POWER_SAVE_SETTING_BACKLIGHT_TITLE_CTRL_ID, MMISET_POWER_SAVE_SETTING_BACKLIGHT_FORM_CTRL_ID),
    CHILD_OWNDRAW_CTRL(TRUE, MMISET_POWER_SAVE_SETTING_BACKLIGHT_ANIM_CTRL_ID, MMISET_POWER_SAVE_SETTING_BACKLIGHT_FORM_CTRL_ID, (uint32)MMISET_ContrastBarCtrlCallbackFun),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISET_POWER_SAVE_SETTING_LIST2_CTRL_ID, MMISET_POWER_SAVE_SETTING_FORM_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMISET_POWER_SAVE_SCHEDULE_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandlePowerSaveScheduleWindow),
    WIN_ID(MMISET_POWER_SAVE_SCHEDULE_WIN_ID),
    WIN_TITLE(TXT_POWER_SAVING_MODE_SCHEDULE),
    //WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),

    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_POWER_SAVE_SCHEDULE_LIST_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    END_WIN
};
#endif

//dong.chunguang add the window for system setting
WINDOW_TABLE( MMISET_SYSTEM_SETTING_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetSystemSettingWindow ),
    WIN_ID( MMISET_SYSTEM_SETTING_WIN_ID ),
//     WIN_TITLE(TXT_SET_SYSTEM),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_SYSTEM_LIST_CTRL_ID),
    //CREATE_MENU_CTRL(MENU_SET_SYSTEM_SETTING, MMISET_SYSTEM_SETTING_MENU_CTRL_ID),
    END_WIN
};
//2015-7-13 tangting add the window for time & language
WINDOW_TABLE( MMISET_TIME_LANGUAGE_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSetTimeLanguageWindow),
    WIN_ID( MMISET_TIME_LANGUAGE_WIN_ID),
    //nhl modify softkey
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_TIME_LANGUAGE_CTRL_ID),
    END_WIN
};
WINDOW_TABLE( MMISET_POWER_MANAGER_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetPowerManagerWindow ),
    WIN_ID( MMISET_POWER_MANAGER_WIN_ID ),
//     WIN_TITLE(TXT_SET_SYSTEM),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_POWER_MANAGER_LIST_CTRL_ID),
    END_WIN
};
WINDOW_TABLE( MMISET_BATTERY_STATUS_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetBatteryStatusWindow ),
    WIN_ID( MMISET_BATTERY_STATUS_WIN_ID ),
//     WIN_TITLE(TXT_SET_SYSTEM),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_BATTERY_STATUS_LIST_CTRL_ID),
    END_WIN
};
WINDOW_TABLE( MMISET_STORAGE_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetStorageWindow ),
    WIN_ID( MMISET_STORAGE_WIN_ID ),
//     WIN_TITLE(TXT_SET_SYSTEM),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_STORAGE_CTRL_ID),
    END_WIN
};
WINDOW_TABLE( MMISET_PREFER_STORAGE_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetPreferStorageWindow ),
    WIN_ID( MMISET_PREFER_STORAGE_WIN_ID ),
//     WIN_TITLE(TXT_SET_SYSTEM),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_PREFER_STORAGE_CTRL_ID),
    END_WIN
};
WINDOW_TABLE( MMISET_PHONE_LOCK_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetPhoneLockWindow ),
    WIN_ID( MMISET_PHONE_LOCK_WIN_ID ),
//     WIN_TITLE(TXT_SET_SYSTEM),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_PHONE_LOCK_CTRL_ID),
    END_WIN
};

#ifdef MMI_VOLTE_SUPPORT
WINDOW_TABLE(MMISET_VOLTE_WIN_ID) =
{
    WIN_FUNC((uint32) HandleSetVoLTEWindow),
    WIN_ID(MMISET_SET_VOLTE_WIN_ID),
    WIN_TITLE( TXT_LIST_SET_VOLTE ),
    WIN_SOFTKEY(STXT_SOFTKEY_CHANGE, TXT_NULL, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_VOLTE_LIST_CTRL_ID),
    END_WIN	
};
#endif
#if defined(MMI_2G_4G_SUPPORT)

WINDOW_TABLE( MMISET_2G_4G_SIM_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSimWinHandleMsg ),
    WIN_ID( MMISET_2G4G_SIM_WIN_ID),
    WIN_TITLE( TXT_NETWORK_TYPE ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_2G4G_SIM_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMISET_SET_2G_4G_WIN_TAB ) = 
{
    WIN_FUNC( (uint32) Set2G4GNetWorkWinHandleMsg),    
    WIN_ID( MMISET_SET_2G4G_WIN_ID ),
    WIN_TITLE(STR_SETT_LIST_NET_TYPE_EXT01),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_2G4G_CTRL_ID),
    END_WIN
};
#endif

//main setting
//#define TEST_MAIN_MENU
#ifdef TEST_MAIN_MENU
enum{
    ID_SET_LIST_SYSTEM,
    ID_SET_LIST_PHONE,
    ID_SET_LIST_PERSIONALIATION,
    ID_SET_LIST_MDISPLAY,
    ID_SET_LIST_TIME_LANGUAGE,
    ID_SET_LIST_SECURITY,
    ID_SET_LIST_MCALL,
    ID_SET_LIST_CONNECTION,
    ID_SET_LIST_STORAGE,
#ifdef MMI_INDIAN_SOS_SUPPORT
    ID_SET_LIST_SOS
#endif
};
#else
enum{
    ID_SET_LIST_SYSTEM,
    ID_SET_LIST_CONNECTION,
    ID_SET_LIST_PERSIONALIATION,
#ifdef MMI_SHOW_MSA
    ID_SET_LIST_MS_ACCOUNT,
#endif
    ID_SET_LIST_TIME_LANGUAGE,
    ID_SET_LIST_SECURITY,
    ID_SET_LIST_STK,
    ID_SET_LIST_STORAGE,
#ifdef JAVA_SUPPORT 	
    ID_SET_LIST_INSTALLED_APPS,
#endif	
#ifdef MMI_VOLTE_SUPPORT
    ID_SET_LIST_VOLTE,
#endif
#ifdef MMI_2G_4G_SUPPORT    
    ID_SET_2G_4G_NETWORK,
#endif    
#ifdef MMI_INDIAN_SOS_SUPPORT
    ID_SET_LIST_SOS
#endif
};
#endif
//system setting
enum{
    ID_SET_LIST_CALL,
    ID_SET_LIST_MESSAGE,
    ID_SET_LIST_DISPLAY,
    ID_SET_LIST_NOTIFICATIONS,
    //ID_SET_LIST_CONFSERVICE,
    ID_SET_LIST_UPDATE,
    ID_SET_LIST_ABOUT
};
//Power managerment
enum{
    ID_SET_LIST_AUTO_POWER,
    ID_SET_LIST_BATTERY_STATUS
};
//battery status
enum{
    ID_SET_LIST_BATTERY_USAGE,
    ID_SET_LIST_BATTERY_TIME,
    ID_SET_LIST_LAST_FULL_CHARGE
};
//backup
enum{
    ID_SET_LIST_CREATE_BACKUP,
    ID_SET_LIST_RESTORE_BACKUP,
    ID_SET_LIST_PREFER_STORAGE
};
//Prefer storage
enum{
    ID_SET_LIST_STORAGE_PHONE,
    ID_SET_LIST_STORAGE_MEMORY_CARD
};
//end
#ifdef UNISOC_TEST_ENTRY_FOR_TESTER
WINDOW_TABLE( MMISET_SECURITY_CODE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetSecurityCodeWindow ),
    WIN_ID( MMISET_TEST_SECURITY_CODE_ID),
    WIN_SOFTKEY(TXT_COMMON_CHOOSE, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SECURITY_LIST_CTRL_ID),
    END_WIN
};
WINDOW_TABLE( MMISET_TEST_ENTRY_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleTestEntryWindow ),
    WIN_ID( MMISET_TEST_ENTRY_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_TEST_ENTRY, MMISET_TEST_ENTRY_MENU_CTRL_ID),
    END_WIN
};
#endif
#ifdef FOTA_SUPPORT_ABUP
WINDOW_TABLE(MMISET_PHONE_FOTA_WIN_TAB) =
{
	WIN_FUNC((uint32)CheckUpdateWinHandleMsg),
	WIN_ID(MMISET_PHONE_SETTING_FOTA_WIN_ID),
	WIN_TITLE(TXT_SETTINGS_ABUP_FOTA_CHECK),
    CREATE_TEXT_CTRL(MMISET_SETTINGS_CHECK_UPDATE_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifndef MMI_PHONESET_MINI_SUPPORT
/*****************************************************************************/
//  Description : to handle phone setting window message
//  Global resource dependence :
//  Author:xiangjie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetPhoneWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    //BOOLEAN                     udisk_status    =   TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            // fix cr89287
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            MMK_SetAtvCtrl(win_id,MMISET_SET_PHONE_MENU_CTRL_ID);
            break;


#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            GUIMENU_GetId(MMISET_SET_PHONE_MENU_CTRL_ID,&group_id,&menu_id);
            switch (menu_id)
            {
                //        case ID_SET_TIME_DATE:
                //            MMISET_CreateDateTimeWin();
                //            break;
                //
                //        case ID_SET_LANGUAGE://语言设置
                //            MMK_CreateWin((uint32*)MMISET_SET_LANGUAGE_WIN_TAB,PNULL);
                //            break;
                //
                ////sogou不支持模糊音，非触摸屏不支持手写，如果两者同时成立时，则导致没有输入法设置项
                //#if (defined TOUCH_PANEL_SUPPORT || ((!defined IM_ENGINE_SOGOU) && (defined IM_SIMP_CHINESE_SUPPORT) && (defined IM_TRAD_CHINESE_SUPPORT)))
                //        case ID_SET_INPUT_MODE://输入法设置
                //            MMIAPIIM_OpenSetWin();
                //            break;
                //#endif
                //
                //        case ID_SET_SHORTCUT:    //快捷键设置
                //            MMISET_EnterSettingShortcutWin();
                //            break;
                //
                //        case ID_SET_TXT_CODE_TYPE:
                //            MMK_CreateWin((uint32*)MMISET_SET_TXT_CODE_TYPE_WIN_TAB,PNULL);
                //            break;
                //
                //
                //        case ID_SET_COORDINATE://校正触摸屏
                //            if(!POWER_IsAutoTestMode())
                //                MMIAPITP_COORDINATE_OpenWin(FALSE);
                //            break;
                //
                //        case ID_SET_AUTO_POWERONOFF://自动开关机
                //            MMIAPIAUTOPOWER_OpenMainWin();
                //            break;
                //
                //#ifdef LCD_SLIDE_SUPPORT
                //        case ID_SET_SLIDE_OPERATION://滑盖操作
                //            MMK_CreateWin((uint32*)MMISET_SLIDE_OPERATION_WIN_TAB, PNULL);
                //            break;
                //#endif
                //
                //        case ID_SET_POWER_MANAGEMENT:
                //            MMIAPIPHONE_OpenBtryCapWin();
                //            break;
                //
                //#ifdef MMI_POWER_SAVING_MODE_SUPPORT
                //        case ID_SET_SAVE_ELECTRICITY:
                //            MMK_CreateWin((uint32 *) MMISET_POWER_SAVE_WIN_TAB, PNULL);
                //            break;
                //#endif
                //
                //        case ID_SET_DM_SERVICE:
                //            MMIAPIDM_OpenMainMenuWin();
                //            break;
                //#ifdef MOTION_SENSOR_TYPE
                //        case ID_SET_SENSOR:
                //            OpenSetSensorWin();
                //            break;
                //#endif
                //        case ID_SET_RESET_FACTORY://恢复出厂设置
                //            MMIAPISET_OpenInputResetPwdWin(FALSE);
                //            break;
                //
                //#ifdef BROWSER_SUPPORT
                //        case ID_SET_SELECT_BROWSER_TYPE:
                //            MMIAPIBROWSER_OpenSetBrowserWin();
                //            break;
                //#endif
                case ID_SETTINGS_PHONE_SETTING:
                    MMK_CreateWin((uint32 *) MMISET_PHONE_SETTING_WIN_TAB, PNULL);
                    break;

                case ID_SETTINGS_DISPLAY:
                    MMIAPISET_EnterDisplaySettingWin();
                    break;

                case ID_SETTINGS_SECURITY:
                    MMIAPISET_EnterSecuritySettingWin();
                    break;

                case ID_SETTINGS_CALL:
                    MMIAPISET_OpenCallSettingsWindow();
                    break;

#ifdef MMI_INDIAN_SOS_SUPPORT
                case ID_SETTINGS_SOS:
                    MMIAPISET_EnterSOSSettingWin();
                    break;
#endif

#if defined (MMI_TVOUT_ENABLE) || defined (MMI_GPRS_SUPPORT) || defined (BLUETOOTH_SUPPORT)
                case ID_SETTINGS_LINK:
                    MMIAPICONNECTION_OpenMenuWin();
                    break;
#endif


#ifdef CMCC_UI_STYLE
                case ID_SET_PHONE_VERSION:
                    MMIAPIENG_CreateShowVersionWin();
                    break;
#endif

#ifdef UNISOC_TEST_ENTRY_FOR_TESTER
                case ID_SET_FUN_TEST_ENTRY:
                    MMK_CreateWin((uint32 *) MMISET_TEST_ENTRY_WIN_TAB, PNULL);
                    break;
#endif
#ifdef FOTA_SUPPORT_ABUP
                case ID_SETTINGS_PHONE_ABUP_FOTA: 
                    MMK_CreateWin((uint32 *) MMISET_PHONE_FOTA_WIN_TAB, PNULL);
                    break;
#endif
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
#endif

/*****************************************************************************/
//  Description : to handle select language
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetLanguageWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T    ctrl_id = 0;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
#ifdef DPHONE_SUPPORT
            {
                GUILIST_SetMaxItem(MMISET_SET_LANGUAGE_CTRL_ID,MMISET_GetValidLanguageNum(), FALSE );//max item 33
                MMISET_AppendLanguageListItem(MMISET_SET_LANGUAGE_CTRL_ID);
                MMK_SetAtvCtrl(win_id,MMISET_SET_LANGUAGE_CTRL_ID);
            }
#else
#ifndef MMI_PDA_SUPPORT
            GUIFORM_SetCircularHandleUpDown(MMISET_LANGUAGE_FORM_CTRL_ID,TRUE);
#endif
            SetLanguageFormCtrlParam();
            MMISET_SetLanguageCtrlContent();
            MMISET_SetInputLanguageCtrlContent();
            MMK_SetAtvCtrl(win_id,MMISET_LANGUAGE_SHOW_LABEL_CONTENT_CTRL_ID);
#endif
            break;

        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_NOTIFY_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef DPHONE_SUPPORT
            if (!GUILIST_IsCurrItemInvalid(MMISET_SET_LANGUAGE_CTRL_ID))
            {
                MMISET_SetSelectLanguage(MMISET_SET_LANGUAGE_CTRL_ID);
#ifndef SUBLCD_SIZE_NONE
                //需要立即重新刷新小屏，否则小屏的内容不会改变
                MMISUB_UpdateDisplaySubLcd();
#endif
                //success!
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                MMK_CloseWin( win_id );
            }
#else
            {
#ifdef TOUCH_PANEL_SUPPORT
                if(MSG_CTL_PENOK == msg_id)
                {
                    ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
                }
                else
#endif
                {
                    ctrl_id = MMK_GetActiveCtrlId(win_id);
                }
                switch(ctrl_id)
                {
                    case MMISET_LANGUAGE_SHOW_LABEL_CONTENT_CTRL_ID:
                        MMK_CreatePubListWin((uint32*)MMISET_SET_SHOW_LANGUAGE_CONTENT_WIN_TAB, PNULL);
                        break;

                    case MMISET_LANGUAGE_INPUT_LABEL_CONTENT_CTRL_ID:
                        MMK_CreatePubListWin( (uint32*)MMISET_SET_INPUT_LANGUAGE_CONTENT_WIN_TAB, PNULL);
                        break;

                    default:

                        break;
                }
            }
#endif
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

//add by nhl for set language ui HandleSetLanguageWindow2

LOCAL MMI_RESULT_E  HandleSetLanguageWindow2(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_STRING_T show_str={0};
    MMI_STRING_T input_str={0};

    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T    ctrl_id = 0;
    MMI_STRING_T	temp_str = {0};

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUILIST_SetMaxItem(MMISET_LANGUAGE_FORM_CTRL_ID2,MMISET_GetValidLanguageNum(), FALSE );//max item 33
            MMISET_AppendLanguageListItem(MMISET_LANGUAGE_FORM_CTRL_ID2);
//             MMI_GetLabelTextByLang(STXT_HEADER_LANGUAGE, &temp_str);
//             if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len))
//             {
//                 GUIWIN_UpdateStb();
//             }
            MMK_SetAtvCtrl(win_id,MMISET_LANGUAGE_FORM_CTRL_ID2);
            break;
            //case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_NOTIFY_MIDSK:
            MMISET_SetSelectLanguage(MMISET_LANGUAGE_FORM_CTRL_ID2);
            MMK_CloseWin( win_id );
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

/*****************************************************************************/
//  Description :显示语言设置的窗口处理函数
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetLanguageContentWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    MMI_STRING_T show_str={0};
    MMI_STRING_T input_str={0};
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUILIST_SetMaxItem(MMISET_SET_SHOW_LANGUAGE_CONTENT_LIST_CTRL_ID,MMISET_GetValidLanguageNum(), FALSE );//max item 33
            MMISET_AppendLanguageListItem(MMISET_SET_SHOW_LANGUAGE_CONTENT_LIST_CTRL_ID);
            MMK_SetAtvCtrl(win_id,MMISET_SET_SHOW_LANGUAGE_CONTENT_LIST_CTRL_ID);
            break;

        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
        case MSG_NOTIFY_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            MMISET_SetSelectLanguage(MMISET_SET_SHOW_LANGUAGE_CONTENT_LIST_CTRL_ID);

            MMI_GetLabelTextByLang(TXT_SET_SHOW_LANGUAGE,&show_str);
            GUILABEL_SetText(MMISET_LANGUAGE_SHOW_LABEL_CTRL_ID, &show_str, FALSE);
            MMI_GetLabelTextByLang(TXT_INPUT_LANGUAGE,&input_str);
            GUILABEL_SetText(MMISET_LANGUAGE_INPUT_LABEL_CTRL_ID, &input_str, FALSE);

            MMISET_SetLanguageCtrlContent();
            GUIWIN_SetTitleTextId(MMISET_SET_LANGUAGE_WIN_ID, TXT_SET_LANGUAGE_SETTINGS, FALSE);
            MMK_CloseWin( win_id );
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}



/*****************************************************************************/
//  Description :输入语言设置的窗口处理函数
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetLanguageInputContentWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    MMISET_LANGUAGE_TYPE_E  cur_input_language_type = MMISET_LANGUAGE_ENGLISH;
    GUIIM_LANGUAGE_T        cur_input_language_index   =   0;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            cur_input_language_index = MMIAPIIM_GetLanguage();
            cur_input_language_type = (MMISET_LANGUAGE_TYPE_E)MMIAPIIM_GetSetLangByGUILangIdx(cur_input_language_index);
            GUILIST_SetMaxItem(MMISET_SET_INPUT_LANGUAGE_CONTENT_LIST_CTRL_ID,MMISET_GetValidLanguageNum(), FALSE );//max item 33
            MMIAPISET_AppendInputLanguageListItem(MMISET_SET_INPUT_LANGUAGE_CONTENT_LIST_CTRL_ID,cur_input_language_type);
            MMK_SetAtvCtrl(win_id,MMISET_SET_INPUT_LANGUAGE_CONTENT_LIST_CTRL_ID);
            break;

        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
        case MSG_NOTIFY_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            MMISET_SetInputLanguage(MMISET_SET_INPUT_LANGUAGE_CONTENT_LIST_CTRL_ID);
            MMISET_SetInputLanguageCtrlContent();
            MMK_CloseWin( win_id );
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;

}

#if 0
/*****************************************************************************/
//  Description : 编码方式设置界面
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetCodeType(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMISET_SET_TXT_CODE_TYPE_CTRL_ID;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUILIST_SetMaxItem(ctrl_id,MMISET_TXT_CODE_TYPE_MAX, FALSE );
            AppendCodeTypeListItem(ctrl_id);
            MMK_SetAtvCtrl(win_id,ctrl_id);
            break;

        case MSG_APP_WEB:
        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
            SetCodeType(ctrl_id);
            //success!
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            MMK_CloseWin( win_id );
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
#endif
#ifdef LCD_SLIDE_SUPPORT

/*****************************************************************************/
//  Description : to handle slide operation window message
//  Global resource dependence :
//  Author:jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSlideOperationWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_MENU_GROUP_ID_T         group_id = 0;
    MMI_MENU_ID_T               menu_id = 0;
    MMI_RESULT_E                recode = MMI_RESULT_TRUE;


    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            break;

        case MSG_FULL_PAINT:

            MMK_SetAtvCtrl(win_id, MMISET_SET_SLIDE_OPERATION_MENU_CTRL_ID);
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            GUIMENU_GetId(MMISET_SET_SLIDE_OPERATION_MENU_CTRL_ID,&group_id,&menu_id);
            switch (menu_id)
            {
                case ID_SET_SLIDE_OPEN://ID_SET_SLIDE_OPEN
                    MMK_CreateWin((uint32*)MMISET_SET_SLIDE_OPEN_WIN_TAB,PNULL);
                    break;

                case ID_SET_SLIDE_CLOSE://ID_SET_SLIDE_CLOSE
                    MMK_CreateWin((uint32*)MMISET_SET_SLIDE_CLOSE_WIN_TAB,PNULL);

                    break;

                case ID_SET_KEYLOCK_SWITCH://ID_SET_KEYLOCK_SWITCH
                    MMK_CreateWin((uint32*)MMISET_SET_KEYLOCK_SWITCH_WIN_TAB,PNULL);
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
//  Description : slide open
//  Global resource dependence :
//  Author:jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetSlideOpenTypeWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:

            //设置列表项数
            GUILIST_SetMaxItem(MMISET_SET_SLIDE_OPEN_TYPE_CTRL_ID,2, FALSE );//max item 2
            //显示列表
            MMISET_AppendSlideOpenTypeListItem(MMISET_SET_SLIDE_OPEN_TYPE_CTRL_ID);
            //激活控件
            MMK_SetAtvCtrl(win_id,MMISET_SET_SLIDE_OPEN_TYPE_CTRL_ID);
            break;

        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_WEB:
        case MSG_APP_OK:
            {
                //设置打开滑盖的模式为当前模式
                uint16      cur_selection = 0;
                MMISET_SLIDE_OPEN_TYPE_E slide_open_type = 0;

                GUILIST_GetSelectedItemIndex(MMISET_SET_SLIDE_OPEN_TYPE_CTRL_ID, &cur_selection, sizeof(cur_selection));
                //SCI_TRACE_LOW:"HandleSetSlideOpenTypeWindow cur_selection = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_PHONEWIN_1126_112_2_18_2_51_33_255,(uint8*)"d",cur_selection);
                if (0 == cur_selection)
                {
                    slide_open_type = MMISET_RECEIVE_PHONE_TYPE;
                }
                else
                {
                    slide_open_type = MMISET_REJECT_PHONE_TYPE;
                }
                MMISET_SetSelectSlideOpenType(slide_open_type);
                //成功
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                //关闭当前窗口
                MMK_CloseWin( win_id );
            }
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}


/*****************************************************************************/
//  Description : slide close
//  Global resource dependence :
//  Author:jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetSlideCloseTypeWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:

            //设置列表项数
            GUILIST_SetMaxItem(MMISET_SET_SLIDE_CLOSE_TYPE_CTRL_ID,2, FALSE );//max item 2
            //显示列表
            MMISET_AppendSlideCloseTypeListItem(MMISET_SET_SLIDE_CLOSE_TYPE_CTRL_ID);
            //激活控件
            MMK_SetAtvCtrl(win_id,MMISET_SET_SLIDE_CLOSE_TYPE_CTRL_ID);
            break;

        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_WEB:
        case MSG_APP_OK:
            {
                //设置合上滑盖的模式为当前模式
                uint16      cur_selection = 0;
                MMISET_SLIDE_CLOSE_TYPE_E slide_close_type = 0;

                GUILIST_GetSelectedItemIndex(MMISET_SET_SLIDE_CLOSE_TYPE_CTRL_ID,&cur_selection,sizeof(cur_selection));
                //SCI_TRACE_LOW:"HandleSetSlideCloseTypeWindow cur_selection = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_PHONEWIN_1191_112_2_18_2_51_33_256,(uint8*)"d",cur_selection);
                if (0 == cur_selection)
                {
                    slide_close_type = MMISET_STOP_OPERATION_TYPE;
                }
                else
                {
                    slide_close_type = MMISET_CONTINUE_OPERATION_TYPE;
                }
                MMISET_SetSelectSlideCloseType(slide_close_type);
                //成功
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                //关闭当前窗口
                MMK_CloseWin( win_id );
            }
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

/*****************************************************************************/
//  Description : keylock switch
//  Global resource dependence :
//  Author:jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetKeylockSwitchTypeWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:

            //设置列表项数
            GUILIST_SetMaxItem(MMISET_SET_KEYLOCK_SWITCH_TYPE_CTRL_ID,2, FALSE );//max item 2
            //显示列表
            MMISET_AppendKeylockSwitchTypeListItem(MMISET_SET_KEYLOCK_SWITCH_TYPE_CTRL_ID);
            //激活控件
            MMK_SetAtvCtrl(win_id,MMISET_SET_KEYLOCK_SWITCH_TYPE_CTRL_ID);
            break;

        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_WEB:
        case MSG_APP_OK:
            {
                //设置选中的时间显示格式为当前显示格式
                uint16      cur_selection = 0;
                MMISET_KEYLOCK_SWITCH_TYPE_E keylock_switch_type = 0;

                GUILIST_GetSelectedItemIndex(MMISET_SET_KEYLOCK_SWITCH_TYPE_CTRL_ID,&cur_selection,sizeof(cur_selection));
                //SCI_TRACE_LOW:"HandleSetKeylockSwitchTypeWindow cur_selection = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_PHONEWIN_1255_112_2_18_2_51_33_257,(uint8*)"d",cur_selection);
                if (0 == cur_selection)
                {
                    keylock_switch_type = MMISET_KEYLOCK_SWITCH_ON_TYPE;
                }
                else
                {
                    keylock_switch_type = MMISET_KEYLOCK_SWITCH_OFF_TYPE;
                }
                MMISET_SetSelectKeylockSwitchType(keylock_switch_type);
                //成功
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                //关闭当前窗口
                MMK_CloseWin( win_id );
            }
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

#endif


#ifdef MOTION_SENSOR_TYPE
/*****************************************************************************/
//  Description : open set sensor win
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void OpenSetSensorWin(void)
{
    MMK_CreateWin((uint32*)MMISET_SET_SENSOR_WIN_TAB,PNULL);
}
/*****************************************************************************/
//  Description : Append the item of sensor auto rotation list
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
LOCAL void AppendSensorAutorotationListItem(
        MMI_CTRL_ID_T    ctrl_id, //in
        uint32 sensor_open_flag
        )
{
    uint8 item_num=0;
    //    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    //    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    MMI_TEXT_ID_T text_id[MMISET_SENSOR_AUTOROTATION_MAX] ={
        TXT_SET_SENSOR_90,
        TXT_SET_SENSOR_180,
        TXT_SET_SENSOR_270,
        //TXT_SET_SENSOR_VIDEO_FULLSCREEN,
    };

    //GUILIST_SetOwnSofterKey(ctrl_id, TRUE);

    GUILIST_ChangeCheckListDefSoftkeyID(ctrl_id, STXT_CANCEL, STXT_SELECT, STXT_FINISH);
    //add items to listbox
    for(item_num = 0; item_num < MMISET_SENSOR_AUTOROTATION_MAX; item_num++)
    {
        /* item_t.item_style    = GUIITEM_STYLE_ONE_LINE_CHECK;
           item_t.item_data_ptr = &item_data;

           item_data.softkey_id[0] = STXT_OPTION;
           item_data.softkey_id[1] = TXT_NULL;
           item_data.softkey_id[2] = STXT_FINISH;


           item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
           item_data.item_content[0].item_data.text_id = text_id[item_num];

           GUILIST_AppendItem( ctrl_id, &item_t );*/
        if ( (sensor_open_flag&(1<<(5-item_num)))!= 0)
        {
            MMIAPISET_AppendListItemByTextIdExt(text_id[item_num],
                    STXT_SELECT,
                    TXT_NULL,
                    STXT_FINISH,
                    ctrl_id,
                    GUIITEM_STYLE_ONE_LINE_CHECK
                    );
            GUILIST_SetSelectedItem(ctrl_id, item_num, TRUE);
        }
        else
        {
            MMIAPISET_AppendListItemByTextIdExt(text_id[item_num],
                    STXT_CANCEL,
                    TXT_NULL,
                    STXT_FINISH,
                    ctrl_id,
                    GUIITEM_STYLE_ONE_LINE_CHECK
                    );
        }
    }
}

/*****************************************************************************/

//  Description :
//  Global resource dependence :
//  Author:maryxiao
//  Modify:wei.zhou, for wre use
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_ReturnToAngleSupport(uint32 sensor_open_flag)//cr234883
{
    LCD_ANGLE_E cur_angle = LCD_ANGLE_0;

    cur_angle = MMK_GetScreenAngle();
    if((cur_angle == LCD_ANGLE_90 && !(sensor_open_flag&0x20))
            || (cur_angle == LCD_ANGLE_180 && !(sensor_open_flag&0x10))
            || (cur_angle == LCD_ANGLE_270 && !(sensor_open_flag&0x08)))
    {
        MMK_SetScreenAngle(LCD_ANGLE_0);
    }
}
/*****************************************************************************/

//  Description :
//  Global resource dependence :
//  Author:maryxiao
//  Note:
/*****************************************************************************/
//cr227614 maryxiao
LOCAL MMI_RESULT_E HandleSetSensorAutorotaionWindow(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id     =   MMISET_SET_SENSOR_AUTOROTATION_CTRL_ID;
    uint16      cur_selection[MMISET_SENSOR_AUTOROTATION_MAX]   =  {0};
    uint16      cur_num       =   0;
    int32       loop=0;
    static  uint32      sensor_open_flag=0;
    uint32      flag = 0;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUILIST_SetMaxItem(ctrl_id, MMISET_SENSOR_AUTOROTATION_MAX, FALSE);
            GUILIST_SetMaxSelectedItem(ctrl_id, MMISET_SENSOR_AUTOROTATION_MAX);
            sensor_open_flag = MMISET_GetIsSensorOpenFlag();
            MMK_SetAtvCtrl(win_id,ctrl_id);
            break;
        case MSG_FULL_PAINT:
            AppendSensorAutorotationListItem(ctrl_id,sensor_open_flag);
            break;

        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_WEB:
        case MSG_APP_OK:
            break;
        case MSG_LCD_SWITCH:
            cur_num=GUILIST_GetSelectedItemIndex(ctrl_id, cur_selection, MMISET_SENSOR_AUTOROTATION_MAX);
            for(loop=0;loop<cur_num;loop++)
            {
                flag|=1<<(2-cur_selection[loop]);

            }
            sensor_open_flag&=0x7;
            sensor_open_flag|=flag<<3;;
            break;
        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );
            break;

        case MSG_CLOSE_WINDOW:
            sensor_open_flag = MMISET_GetIsSensorOpenFlag();
            cur_num=GUILIST_GetSelectedItemIndex(ctrl_id, cur_selection, MMISET_SENSOR_AUTOROTATION_MAX);
            for(loop=0;loop<cur_num;loop++)
            {
                flag|=1<<(2-cur_selection[loop]);

            }
            sensor_open_flag&=0x7;
            sensor_open_flag|=flag<<3;
            MMISET_SetIsSensorOpenFlag(sensor_open_flag);
            MMISET_ReturnToAngleSupport(sensor_open_flag);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
/*****************************************************************************/
//  Description : Append the item of sensor list
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void AppendSensorListItem(
        MMI_CTRL_ID_T    ctrl_id //in
        )
{
    uint8 item_num=0;
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    MMI_TEXT_ID_T text_id[MMISET_SENSOR_TYPE_MAX] ={
#ifdef MMI_RES_ORIENT_BOTH
        TXT_SET_SENSOR_LANDSCAPE,
#endif
        TXT_SET_SENSOR_CALL_SILENCE,
        TXT_SET_SENSOR_REPLACE_WALLPAPER,
        TXT_SET_SENSOR_NEXT_SONG,
        //TXT_SET_SENSOR_VIDEO_FULLSCREEN,
    };
    uint32 sensor_open_flag=MMISET_GetIsSensorOpenFlag();

    //add items to listbox
    for(item_num = 0; item_num < MMISET_SENSOR_TYPE_MAX; item_num++)
    {
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_CHECK;
        item_t.item_data_ptr = &item_data;

        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = text_id[item_num];

        GUILIST_AppendItem( ctrl_id, &item_t );
#ifdef MMI_RES_ORIENT_BOTH
        if ( (sensor_open_flag&(1<<item_num))!= 0)
#else
            if ( (sensor_open_flag&(1<<(item_num+1)))!= 0)
#endif
            {
                GUILIST_SetSelectedItem(ctrl_id, item_num, TRUE);
            }
    }
}
#ifdef MMI_RES_ORIENT_BOTH
LOCAL void SetSensorAutoRotateParam(void)
{
    MMI_STRING_T label_name  = {0};
#if !defined MMI_GUI_STYLE_TYPICAL
    GUIFORM_SetType(MMISET_SENSOR_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif
    GUIFORM_SetStyle(MMISET_SENSOR_AUTO_ROTATE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_SENSOR_LANDSCAPE, &label_name);
    GUILABEL_SetText(MMISET_SENSOR_AUTO_ROTATE_LABEL_CTRL_ID, &label_name, FALSE);
}
#endif
LOCAL void SetSensorCallMuteParam(void)
{
    uint32 loop = 0;
    uint16 cur_selection = 0;
    MMI_STRING_T label_name  = {0};
    uint32 sensor_open_flag = 0;


    uint32 text_id[2]=
    {
        TXT_OPEN,
        STR_POC_ILIST_OFF,
    };
#if !defined MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_SENSOR_CALL_MUTE_SETLIST_CTRL_ID,ALIGN_LEFT);
    GUISETLIST_SetTitleTextId(MMISET_SENSOR_CALL_MUTE_SETLIST_CTRL_ID, TXT_SET_SENSOR_CALL_SILENCE);
#endif

    GUIFORM_SetStyle(MMISET_SENSOR_CALL_MUTE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_SENSOR_CALL_SILENCE, &label_name);
    GUILABEL_SetText(MMISET_SENSOR_CALL_MUTE_LABEL_CTRL_ID, &label_name, FALSE);


    sensor_open_flag=MMISET_GetIsSensorOpenFlag();

    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_SENSOR_CALL_MUTE_SETLIST_CTRL_ID, text_id[loop]);
    }

    if ((sensor_open_flag&(1<<2))!= 0)
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }

    GUISETLIST_SetCurIndex(MMISET_SENSOR_CALL_MUTE_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetCtrlState(MMISET_SENSOR_CALL_MUTE_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
}

LOCAL void SetSensorChangeWallpaperParam(void)
{
    uint32 loop = 0;
    uint16 cur_selection = 0;
    MMI_STRING_T label_name  = {0};
    uint32 sensor_open_flag = 0;

    uint32 text_id[2]=
    {
        TXT_OPEN,
        STR_POC_ILIST_OFF,
    };

    GUIFORM_SetStyle(MMISET_SENSOR_CHANGE_WALLPAPER_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_SENSOR_REPLACE_WALLPAPER, &label_name);
    GUILABEL_SetText(MMISET_SENSOR_CHANGE_WALLPAPER_LABEL_CTRL_ID, &label_name, FALSE);
#if !defined MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_SENSOR_CHANGE_WALLPAPER_SETLIST_CTRL_ID,ALIGN_LEFT);
    GUISETLIST_SetTitleTextId(MMISET_SENSOR_CHANGE_WALLPAPER_SETLIST_CTRL_ID, TXT_SET_SENSOR_REPLACE_WALLPAPER);
#endif


    sensor_open_flag=MMISET_GetIsSensorOpenFlag();

    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_SENSOR_CHANGE_WALLPAPER_SETLIST_CTRL_ID, text_id[loop]);
    }

    if ((sensor_open_flag&(1<<1))!= 0)
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }

    GUISETLIST_SetCurIndex(MMISET_SENSOR_CHANGE_WALLPAPER_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetCtrlState(MMISET_SENSOR_CHANGE_WALLPAPER_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
}

LOCAL void SetSensorPlayNextTrackParam(void)
{
    uint32 loop = 0;
    uint16 cur_selection = 0;
    MMI_STRING_T label_name  = {0};
    uint32 sensor_open_flag = 0;

    uint32 text_id[2]=
    {
        TXT_OPEN,
        STR_POC_ILIST_OFF,
    };

    GUIFORM_SetStyle(MMISET_SENSOR_PLAY_NEXTTRACK_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_SENSOR_NEXT_SONG, &label_name);
    GUILABEL_SetText(MMISET_SENSOR_PLAY_NEXTTRACK_LABEL_CTRL_ID, &label_name, FALSE);

#if !defined MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_SENSOR_PLAY_NEXTTRACK_SETLIST_CTRL_ID,ALIGN_LEFT);
    GUISETLIST_SetTitleTextId(MMISET_SENSOR_PLAY_NEXTTRACK_SETLIST_CTRL_ID, TXT_SET_SENSOR_NEXT_SONG);
#endif

    sensor_open_flag=MMISET_GetIsSensorOpenFlag();

    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_SENSOR_PLAY_NEXTTRACK_SETLIST_CTRL_ID, text_id[loop]);
    }

    if ((sensor_open_flag&1)!= 0)
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }

    GUISETLIST_SetCurIndex(MMISET_SENSOR_PLAY_NEXTTRACK_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetCtrlState(MMISET_SENSOR_PLAY_NEXTTRACK_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
}
LOCAL void SaveSensorCallMuteParam(void)
{
    uint16 cur_selection = 0;
    uint32 sensor_open_flag=MMISET_GetIsSensorOpenFlag();
    cur_selection = GUISETLIST_GetCurIndex(MMISET_SENSOR_CALL_MUTE_SETLIST_CTRL_ID);
    if(cur_selection == 0)
    {
        sensor_open_flag|=(1<<2);
    }
    else
    {
        sensor_open_flag&=~(1<<2);
    }
    MMISET_SetIsSensorOpenFlag(sensor_open_flag);
}
LOCAL void SaveSensorChangeWallpaperParam(void)
{
    uint16 cur_selection = 0;
    uint32 sensor_open_flag=MMISET_GetIsSensorOpenFlag();
    cur_selection = GUISETLIST_GetCurIndex(MMISET_SENSOR_CHANGE_WALLPAPER_SETLIST_CTRL_ID);
    if(cur_selection == 0)
    {
        sensor_open_flag|=(1<<1);
    }
    else
    {
        sensor_open_flag&=~(1<<1);
    }
    MMISET_SetIsSensorOpenFlag(sensor_open_flag);
}
LOCAL void SaveSensorPlayNextTrackParam(void)
{
    uint16 cur_selection = 0;
    uint32 sensor_open_flag=MMISET_GetIsSensorOpenFlag();
    cur_selection = GUISETLIST_GetCurIndex(MMISET_SENSOR_PLAY_NEXTTRACK_SETLIST_CTRL_ID);
    if(cur_selection == 0)
    {
        sensor_open_flag|=1;
    }
    else
    {
        sensor_open_flag&=~(1<<0);
    }
    MMISET_SetIsSensorOpenFlag(sensor_open_flag);
}


/*****************************************************************************/
//  Description : append form
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void AppendFormChild(
        MMI_HANDLE_T     win_handle,
        MMI_HANDLE_T     form_handle,
        MMI_HANDLE_T     ctrl_handle
        )
{
    GUIFORM_DYNA_CHILD_T    child_data = {0};
    GUIFORM_INIT_DATA_T     init_data = {0};

    //init data
    init_data.layout_type = GUIFORM_LAYOUT_ORDER;

    child_data.is_get_active = TRUE;
    child_data.guid          = SPRD_GUI_FORM_ID;
    child_data.child_handle  = ctrl_handle;
    child_data.init_data_ptr = &init_data;

    //create
    GUIFORM_CreatDynaChildCtrl(win_handle,
            form_handle,
            &child_data);

    GUIFORM_SetStyle(ctrl_handle,GUIFORM_STYLE_UNIT);
}

/*****************************************************************************/
//  Description : append label
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void AppendFormLabel(MMI_HANDLE_T win_handle,
        MMI_HANDLE_T form_handle,
        MMI_CTRL_ID_T label_id,
        BOOLEAN       is_get_active
        )
{
    GUILABEL_INIT_DATA_T    init_data = {0};
    GUIFORM_DYNA_CHILD_T    child_data = {0};

    child_data.is_bg = FALSE;
    child_data.is_get_active = is_get_active;
    child_data.guid = SPRD_GUI_LABEL_ID;
    child_data.init_data_ptr = &init_data;
    child_data.child_handle = label_id;
    //edit_ptr = MMK_CreateControl( &create );
    GUIFORM_CreatDynaChildCtrl(win_handle, form_handle, &child_data);
}

/*****************************************************************************/
//  Description : append list
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void AppendFormSetList(MMI_HANDLE_T win_handle,
        MMI_HANDLE_T form_handle,
        MMI_CTRL_ID_T setlist_id,
        BOOLEAN       is_get_active
        )
{
    GUISETLIST_INIT_DATA_T    init_data = {0};
    GUIFORM_DYNA_CHILD_T    child_data = {0};

    child_data.is_bg = FALSE;
    child_data.is_get_active = is_get_active;
    child_data.guid = SPRD_GUI_SETLIST_ID;
    child_data.init_data_ptr = &init_data;
    child_data.child_handle = setlist_id;
    //edit_ptr = MMK_CreateControl( &create );
    GUIFORM_CreatDynaChildCtrl(win_handle, form_handle, &child_data);

}

/*****************************************************************************/
//  Description : handle set sensor windows messages
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
#ifdef DPHONE_SUPPORT
LOCAL MMI_RESULT_E  HandleSetSensorWindow(
        MMI_WIN_ID_T      win_id,
        MMI_MESSAGE_ID_E  msg_id,
        DPARAM            param
        )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id     =   MMISET_SET_SENSOR_CTRL_ID;
    uint16      cur_selection[MMISET_SENSOR_TYPE_MAX]   =  {0};
    uint16      cur_num       =   0;
    int32       loop=0;
    uint32      sensor_open_flag=0;


    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUILIST_SetMaxItem(ctrl_id, MMISET_SENSOR_TYPE_MAX, FALSE);
            GUILIST_SetMaxSelectedItem(ctrl_id, MMISET_SENSOR_TYPE_MAX);
            AppendSensorListItem(ctrl_id);
            MMK_SetAtvCtrl(win_id,ctrl_id);
            break;

        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_WEB:
        case MSG_APP_OK:
            break;

        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );
            break;

        case MSG_CLOSE_WINDOW:
            cur_num=GUILIST_GetSelectedItemIndex(ctrl_id, cur_selection, MMISET_SENSOR_TYPE_MAX);
            for(loop=0;loop<cur_num;loop++)
            {
#ifdef MMI_RES_ORIENT_BOTH
                sensor_open_flag|=(1<<(cur_selection[loop]));
#else
                sensor_open_flag|=(1<<(cur_selection[loop]+1));
#endif
            }
            MMISET_SetIsSensorOpenFlag(sensor_open_flag);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
#else
//cr227614 maryxiao
LOCAL MMI_RESULT_E  HandleSetSensorWindow(
        MMI_WIN_ID_T      win_id,
        MMI_MESSAGE_ID_E  msg_id,
        DPARAM            param
        )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id     =   MMISET_SET_SENSOR_CTRL_ID;
    BOOLEAN         is_sensor_mute    =  FALSE;
    BOOLEAN         is_sensor_change_wallpaper = FALSE;
    uint32          sensor_data        =   0;


    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
#if defined (MOTION_SENSOR_TYPE)&& !defined(FLASH_SUPPORT)
#ifndef WIN32
            MSensor_ioctl(MSENSOR_TYPE_ACCELEROMETER, MSENSOR_IOCTL_GET_CAP, &sensor_data);
            is_sensor_mute = sensor_data & MSENSOR_MOTION_CAP_Z_AXIS;
            is_sensor_change_wallpaper = sensor_data & MSENSOR_MOTION_CAP_SHAKE;
            //SCI_TRACE_LOW:"HandleSetSensorWindow sensor_data = %d,is_sensor_mute = %d,is_sensor_change_wallpaper = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_PHONEWIN_1823_112_2_18_2_51_34_258,(uint8*)"ddd",sensor_data,is_sensor_mute,is_sensor_change_wallpaper);
#else
            is_sensor_mute = TRUE;
            is_sensor_change_wallpaper = TRUE;
#endif
#endif
            if(is_sensor_mute)
            {
                AppendFormChild(win_id, MMISET_SENSOR_FORM_CTRL_ID, MMISET_SENSOR_CALL_MUTE_FORM_CTRL_ID);
                AppendFormLabel(win_id, MMISET_SENSOR_CALL_MUTE_FORM_CTRL_ID, MMISET_SENSOR_CALL_MUTE_LABEL_CTRL_ID,FALSE);
                AppendFormSetList(win_id, MMISET_SENSOR_CALL_MUTE_FORM_CTRL_ID,MMISET_SENSOR_CALL_MUTE_SETLIST_CTRL_ID, TRUE);
            }
            if(is_sensor_change_wallpaper)
            {
                AppendFormChild(win_id, MMISET_SENSOR_FORM_CTRL_ID, MMISET_SENSOR_CHANGE_WALLPAPER_FORM_CTRL_ID);
                AppendFormLabel(win_id, MMISET_SENSOR_CHANGE_WALLPAPER_FORM_CTRL_ID, MMISET_SENSOR_CHANGE_WALLPAPER_LABEL_CTRL_ID,FALSE);
                AppendFormSetList(win_id, MMISET_SENSOR_CHANGE_WALLPAPER_FORM_CTRL_ID,MMISET_SENSOR_CHANGE_WALLPAPER_SETLIST_CTRL_ID, TRUE);

                AppendFormChild(win_id, MMISET_SENSOR_FORM_CTRL_ID, MMISET_SENSOR_PLAY_NEXTTRACK_FORM_CTRL_ID);
                AppendFormLabel(win_id, MMISET_SENSOR_PLAY_NEXTTRACK_FORM_CTRL_ID, MMISET_SENSOR_PLAY_NEXTTRACK_LABEL_CTRL_ID,FALSE);
                AppendFormSetList(win_id, MMISET_SENSOR_PLAY_NEXTTRACK_FORM_CTRL_ID,MMISET_SENSOR_PLAY_NEXTTRACK_SETLIST_CTRL_ID, TRUE);
            }

#ifdef MMI_RES_ORIENT_BOTH
            SetSensorAutoRotateParam();
#endif
            if(is_sensor_mute)
            {
                SetSensorCallMuteParam();
            }
            if(is_sensor_change_wallpaper)
            {
                SetSensorChangeWallpaperParam();
                SetSensorPlayNextTrackParam();
            }
            MMK_SetAtvCtrl(win_id, MMISET_SENSOR_FORM_CTRL_ID);
            break;
        case MSG_CTL_SETLIST_SWITCH:
            ctrl_id= MMK_GetActiveCtrlId(win_id);

            switch (ctrl_id)
            {

                case MMISET_SENSOR_CALL_MUTE_SETLIST_CTRL_ID:
                    SaveSensorCallMuteParam();
                    break;

                case MMISET_SENSOR_CHANGE_WALLPAPER_SETLIST_CTRL_ID:
                    SaveSensorChangeWallpaperParam();
                    break;

                case MMISET_SENSOR_PLAY_NEXTTRACK_SETLIST_CTRL_ID:
                    SaveSensorPlayNextTrackParam();
                    break;

                default:
                    break;

            }
            break;
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_WEB:
        case MSG_APP_OK:

            ctrl_id= MMK_GetActiveCtrlId(win_id);
            if(msg_id==MSG_CTL_PENOK)
            {
                ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
            }
            switch (ctrl_id)
            {
#ifdef MMI_RES_ORIENT_BOTH
                case MMISET_SENSOR_AUTO_ROTATE_LABEL_CTRL_ID:
                    MMK_CreatePubListWin((uint32*)MMISET_SET_SENSOR_AUTOROTATION_WIN_TAB,PNULL);
                    break;
#endif

                case MMISET_SENSOR_CALL_MUTE_SETLIST_CTRL_ID:
                    SaveSensorCallMuteParam();
                    break;

                case MMISET_SENSOR_CHANGE_WALLPAPER_SETLIST_CTRL_ID:
                    SaveSensorChangeWallpaperParam();
                    break;

                case MMISET_SENSOR_PLAY_NEXTTRACK_SETLIST_CTRL_ID:
                    SaveSensorPlayNextTrackParam();
                    break;

                default:
                    break;

            }
            break;

        case MSG_CTL_CANCEL:
        case MSG_CLOSE_WINDOW:
            MMK_CloseWin( win_id );
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
#endif
#endif

/*****************************************************************************/
//  Description : handle input reset factory password win msg
//  Global resource dependence :
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInputResetFactoryPwd(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_STRING_T                old_pwd_value           =   {0};
    MMI_RESULT_E                result                  =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id                 =   MMISET_INPUT_RESET_FACTORY_PWD_CTRL_ID;
    uint32                      is_delete_all_data      =   FALSE;
    uint32                      time_out                =   0;
    MMI_WIN_ID_T                wait_win_id             =   0;
    GUI_BOTH_RECT_T edit_both_rect = {0};

    is_delete_all_data = (uint32)(MMK_GetWinAddDataPtr(win_id));

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
            MMIAPISET_SetPwTitleRect(win_id);
            edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
            edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif
            GUIAPICTRL_SetBothRect(ctrl_id, &edit_both_rect);

            //set left softkey
            GUIEDIT_SetSoftkey(ctrl_id,0,MMISET_PRIVACY_PROTECT_PWD_MIN_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;

        case MSG_FULL_PAINT:
            MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
            break;
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        case MSG_APP_MENU:
            // get the value about PIN
            GUIEDIT_GetString(ctrl_id,&old_pwd_value);

            if (PRIVACY_PROTECT_PWD_LEN_IS_IN_RANGE(old_pwd_value.wstr_len))
            {
                //密码是否正确
                if (MMISET_IsCorrectPrivacyProtectPwd( old_pwd_value.wstr_ptr,old_pwd_value.wstr_len))
                {
                    s_setting_is_pwd_right = TRUE;
                    if (is_delete_all_data)
                    {
                        //询问是否清楚用户数据!
                        MMIPUB_OpenQueryWinByTextId(TXT_SET_CLEAN_USER_DATA,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                        //MMIPUB_OpenConfirmationDialogByTextId(PNULL,TXT_SET_CLEAN_USER_DATA,PNULL,PNULL,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,PNULL);
                    }
                    else
                    {
                        //询问是否恢复出厂设置!!
                        MMIPUB_OpenQueryWinByTextId(TXT_COMMON_RESTORE_FACTORY_SETTINGS,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                        //MMIPUB_OpenConfirmationDialogByTextId(PNULL,TXT_COMMON_RESTORE_FACTORY_SETTINGS,PNULL,PNULL,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,PNULL);
                    }

                    //MMIDEFAULT_EnableKeyRing(FALSE);
                    //MMIDEFAULT_EnableTpRing(FALSE);
                    //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_RESET_FACT, FALSE);

                }
                else
                {
                    s_setting_is_pwd_right = FALSE;
                    //清空password, yuanl modify 2009.9.4 for cr151094
                    GUIEDIT_ClearAllStr(ctrl_id);

                    //提示密码错误!
                    MMIPUB_OpenAlertWarningWin(TXT_KL_PSW_WRONG_PSW);
#if 0//Del by yuanl  2009.9.4 for cr151094
                    //清空password
                    GUIEDIT_ClearAllStr(ctrl_id);
#endif
                }
            }
            break;

        case MSG_SET_CLEAN_DATE_IND:
#if defined(MSA_SUPPORT)		
            if(MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL))
            {
                MMIAPIMSA_sign_off(MSA_APP_TYPE_CAMERA_ROLL);
                MMK_CloseWin(win_id);
            }
            else
#endif
            {

		#if 0
                if (is_delete_all_data)
                {
                    //清除所有数据
                    MMISET_CleanUserData();
                    MMISET_ResetFactorySetting();
                    MMK_CloseWin(win_id);
                }
                else
		#endif
                {
                    //进行恢复出厂设置
                    MMISET_CleanUserData();
                    MMISET_ResetFactorySetting();
                    MMK_CloseWin(win_id);
                }
            }
            break;

        case MSG_PROMPTWIN_OK:
            if(s_setting_is_pwd_right)
            {
                s_setting_is_pwd_right = FALSE;
                //关闭询问窗口
                MMIPUB_CloseQuerytWin(PNULL);

                //进入等待界面
                wait_win_id = MMISET_RESET_FACTORY_WAITING_WIN_ID;
                MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,
                        &wait_win_id,PNULL,MMIPUB_SOFTKEY_NONE,
                        HandleResetOrCleanDataWaitWin);

                MMK_PostMsg(win_id, MSG_SET_CLEAN_DATE_IND, PNULL,PNULL);
            }
            else
            {
                //关闭密码错误提示
                MMIPUB_CloseAlertWin();
            }
            break;

        case MSG_PROMPTWIN_CANCEL:
            //关闭询问窗口
            //MMIDEFAULT_EnableKeyRing(TRUE);
            //MMIDEFAULT_EnableTpRing(TRUE);
            //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_REEEESET_FACT, TRUE);
            MMIPUB_CloseQuerytWin(PNULL);
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
            //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_RESET_FACT, TRUE);
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}
#ifndef MMI_PHONESET_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle prompt win of clean user data
//  Global resource dependence :
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePromptCompleteWin(
        MMI_WIN_ID_T    win_id, // 窗口的ID
        MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
        DPARAM             param   // 相应消息的参数
        )
{
    MMI_RESULT_E    result  =   MMI_RESULT_FALSE;

    switch(msg_id)
    {
        case MSG_CLOSE_WINDOW:
            //清除数据后需要重启
            MMIAPIPHONE_PowerReset();
            break;

        default:
            break;
    }

    result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
    return (result);
}
#endif
/*****************************************************************************/
//  Description :open input reset factory password window
//  Global resource dependence :
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenInputResetPwdWin(BOOLEAN      is_clean_all_data)
{
    uint32  tmp     =   is_clean_all_data;
    MMK_CreatePubEditWin((uint32*)MMISET_INPUT_RESET_FACTORY_PWD_WIN_TAB,(ADD_DATA)tmp);
}


#if !defined MMI_GUI_STYLE_TYPICAL
PUBLIC void MMIAPISET_OpenImInputLangSettingWin(void)
{
    MMK_CreatePubListWin( (uint32*)MMISET_SET_INPUT_LANGUAGE_CONTENT_WIN_TAB, PNULL);
}
#endif


/*****************************************************************************/
//  Description : 处理恢复出厂设置和清楚用户数据的等待窗口
//  Global resource dependence :
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleResetOrCleanDataWaitWin(
        MMI_WIN_ID_T    win_id, // 窗口的ID
        MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
        DPARAM             param   // 相应消息的参数
        )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    static uint32   s_waitting_num = 0;
    static BOOLEAN is_cleandata =FALSE;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            s_waitting_num = 0;
            //recode  =   MMI_RESULT_FALSE;
            recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
            break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
        case MSG_KEYDOWN_FLIP:
            MMIDEFAULT_HandleFlipKey(FALSE);
            break;
#endif

        case MSG_APP_RED:
            break;
        case MSG_SET_RESET_NEED_WAIT_IND:
            //add one wait action
            s_waitting_num++;
            SCI_TRACE_LOW("mmiset_phonewin.c HandleResetOrCleanDataWaitWin: add s_waitting_num=%d", s_waitting_num);
            break;
        case MSG_SET_RESET_FACTORY_OVER_IND:
            if(s_waitting_num > 0)
            {
                //finished one, wait number -1
                s_waitting_num --;
            }
            SCI_TRACE_LOW("mmiset_phonewin.c HandleResetOrCleanDataWaitWin: minus s_waitting_num=%d", s_waitting_num);
            //no one to wait, finished
            if(s_waitting_num == 0)
            {
                if(is_cleandata)
                {
                    //MMK_CloseWin(win_id);
                    MMIAPIPHONE_PowerReset();
                }
                else
                {
                    //MMK_CloseWin(win_id);
                    MMK_ReturnIdleWin(); //crMS00187558 @murphy.xie
                    //success!
#ifndef FLASH_SUPPORT
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePromptCompleteThemeWin);
#endif
                }
            }
            break;

        case MSG_SET_CLEAN_USER_DATA_OVER_IND:
            is_cleandata = TRUE;
            break;

        case MSG_CLOSE_WINDOW:
            //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_RESET_FACT, TRUE);
#ifdef BLUETOOTH_SUPPORT
            MMIAPIBT_ClearAllDevice();
#endif
            recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
            break;
        default:
            recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
            break;
    }

    return recode;
}

#ifndef FLASH_SUPPORT
/*****************************************************************************/
//  Description : handle prompt win of theme
//  Global resource dependence :
//  Author: applezhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePromptCompleteThemeWin(
        MMI_WIN_ID_T    win_id, // 窗口的ID
        MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
        DPARAM             param   // 相应消息的参数
        )
{
    MMI_RESULT_E    result  =   MMI_RESULT_FALSE;

    switch(msg_id)
    {
        case MSG_CLOSE_WINDOW:
            if(MMK_IsOpenWin(MMISET_INPUT_RESET_FACTORY_PWD_WIN_ID))
            {
                MMK_CloseWin(MMISET_INPUT_RESET_FACTORY_PWD_WIN_ID);
            }
#ifdef MULTI_THEME_SUPPORT
            else if(MMK_IsOpenWin(MMISET_SET_THEME_PREVIEW_WIN_ID))
            {
                MMK_CloseWin(MMISET_SET_THEME_PREVIEW_WIN_ID);
            }
            //        else if(MMK_IsOpenWin(MMISET_SET_THEME_WIN_ID))
            //        {
            //            MMK_CloseWin(MMISET_SET_THEME_WIN_ID);
            //         }
#endif
            break;

        default:
            break;
    }

    result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
    return (result);
}
#endif

#ifndef MMI_PHONESET_MINI_SUPPORT
/*****************************************************************************/
//  Description : 设置编码方式
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SetCodeType(
        MMI_CTRL_ID_T      ctrl_id
        )
{
    uint16      cur_selection = 0;
    MMISET_TXT_CODE_TYPE_TYPE_E code_type = MMISET_TXT_CODE_TYPE_GB;

    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);

    switch (cur_selection)
    {
        case MMISET_TXT_CODE_TYPE_GB:
            code_type = MMISET_TXT_CODE_TYPE_GB;
            break;

        case MMISET_TXT_CODE_TYPE_BIG5:
            code_type = MMISET_TXT_CODE_TYPE_BIG5;
            break;

        default:
            break;
    }

    MMINV_WRITE(MMINV_SET_TXT_CODE_TYPE,&code_type);
}


/*****************************************************************************/
//  Description : 编码方式选项:1、GB(简体中文) 2、Big5(繁体中文)
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void AppendCodeTypeListItem(
        MMI_CTRL_ID_T      ctrl_id
        )
{
    uint16                      cur_selection = 0;
    MMISET_TXT_CODE_TYPE_TYPE_E code_type = MMISET_TXT_CODE_TYPE_GB;

    MMIAPISET_AppendListItemByTextId( TXT_CODING_TYPE_GB, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_CODING_TYPE_BIG5, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );

    //get current methed
    code_type = MMIAPISET_GetCodeType();

    //the status is open
    cur_selection = code_type;

    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}

/*****************************************************************************/
//     Description : 获得编码方式
// Global resource dependence :
//  Author: xingdong.li
// Note:
/*****************************************************************************/
PUBLIC MMISET_TXT_CODE_TYPE_TYPE_E MMIAPISET_GetCodeType( void )
{
    MMISET_TXT_CODE_TYPE_TYPE_E code_type = MMISET_TXT_CODE_TYPE_GB;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;


    MMINV_READ(MMINV_SET_TXT_CODE_TYPE,&code_type,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;


        MMIAPISET_GetLanguageType(&lang_type);

        //#if defined BIG5_SUPPORT

        if( MMISET_LANGUAGE_TRAD_CHINESE == lang_type )
        {
            code_type = MMISET_TXT_CODE_TYPE_BIG5;
        }
        else

            //#endif

        {
            code_type = MMISET_TXT_CODE_TYPE_GB;
        }
        MMINV_WRITE(MMINV_SET_TXT_CODE_TYPE,&code_type);
    }

    return (code_type);
}

#endif

/*****************************************************************************/
//  Description : set the language settings form ctrl param
//  Global resource dependence :
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void SetLanguageFormCtrlParam(void)
{
    MMI_STRING_T show_str={0};
    MMI_STRING_T input_str={0};
#if !defined MMI_GUI_STYLE_TYPICAL
    GUIFORM_SetType(MMISET_LANGUAGE_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif

    GUIFORM_SetStyle(MMISET_LANGUAGE_SHOW_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMISET_LANGUAGE_INPUT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_SET_SHOW_LANGUAGE,&show_str);
    GUILABEL_SetText(MMISET_LANGUAGE_SHOW_LABEL_CTRL_ID, &show_str, FALSE);

    MMI_GetLabelTextByLang(TXT_INPUT_LANGUAGE,&input_str);
    GUILABEL_SetText(MMISET_LANGUAGE_INPUT_LABEL_CTRL_ID, &input_str, FALSE);

}


/*****************************************************************************/
//  Description : open phone setting window
//  Global resource dependence :
//  Author: xiangjie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterPhoneSettingWin(void)
{
    MMIMAINMENU_TYPE_E     mainmenu_style  =   MMIAPISET_GetMainmenuStyle();

    //get main menu style
    switch (mainmenu_style)
    {
        case MMIMAINMENU_MATRIX_E:
        case MMIMAINMENU_MATRIX_TITLE_E:
#if 1//def  MMI_GUI_STYLE_MINISCREEN
            MMK_CreateWin((uint32 *) MMISET_PHONE_SETTING_WIN_TAB, PNULL);
#else
            MMK_CreateWin((uint32 *)MMISET_SET_PHONE_WIN_TAB, PNULL);
#endif
            break;
#ifndef MMI_PHONESET_MINI_SUPPORT
        case MMIMAINMENU_ISTYLE_E:
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
        case MMIMAINMENU_GO_SLIDE_E:
#endif
#ifdef MENU_SWEETS_SUPPORT
        case  MMIMAINMENU_SWEETS_E:
#endif
            MMK_CreateWin((uint32 *)MMISET_ICON_PHONE_WIN_TAB, PNULL);
            break;
#endif

        default:
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_PHONEWIN_2393_112_2_18_2_51_35_259,(uint8*)"d",mainmenu_style);
            break;
    }
}
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
/*****************************************************************************/
//  Description :open power saving setting
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPISET_OpenPowerSavingSetting(void)
{
    MMK_CreateWin((uint32*)MMISET_POWER_SAVE_SETTING_WIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description :HandleSetPowerSaveWindow
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetPowerSaveWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{

    MMI_RESULT_E             result = MMI_RESULT_TRUE;
    GUIFORM_CHILD_HEIGHT_T   label_height = {0};
    MMI_CTRL_ID_T            form_ctrl = MMISET_POWER_SAVE_FORM_CTRL_ID;
    MMI_CTRL_ID_T            list_ctrl = MMISET_POWER_SAVE_ONOFF_LIST_CTRL_ID;
    //BOOLEAN                  is_switch = FALSE;
    uint16  hor_space = 0;
    uint16  ver_space = 0;
    uint16  cur_index = 0;
    //MMISET_POWER_SETTING_ITEM_T power_sav_set = MMIAPISET_GetPowerSavingSetting();

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            label_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            label_height.add_data = GUI_ConvertGUIFontToSize(MMI_DEFAULT_SMALL_FONT) + 3;
            GUILIST_SetMaxItem(list_ctrl,2, FALSE);
            GUILIST_SetMaxItem(MMISET_POWER_PERCENT_DIS_LIST_CTRL_ID,1,FALSE);
            GUIFORM_SetSpace(form_ctrl, &hor_space, &ver_space);
            GUIFORM_SetMargin(form_ctrl, 0);
            GUIFORM_PermitChildBg(form_ctrl, FALSE);
            GUIFORM_PermitChildBorder(form_ctrl, FALSE);
            GUIFORM_SetChildHeight(form_ctrl, MMISET_POWER_SAVE_TITLE_CTRL_ID, &label_height);
            GUIFORM_SetChildHeight(form_ctrl, MMISET_POWER_TITLE_LABEL_CTRL_ID, &label_height);
            SetPowerSaveParam();
            AppendTwoTextCheckList(list_ctrl, FALSE);
            AppendOneTextOneIconList(list_ctrl,TXT_SET_POWER_SAVING_SEETING,IMAGE_COMMON_RIGHT_ARROW);
#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
            AppendTwoTextCheckList(MMISET_POWER_PERCENT_DIS_LIST_CTRL_ID, FALSE);
#endif
            MMK_SetAtvCtrl(win_id,  MMISET_POWER_SAVE_ONOFF_LIST_CTRL_ID);
            break;

        case MSG_NOTIFY_LIST_CHECK_CONTENT:
            {
                if(PNULL != param)
                {
                    MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

                    DealWithListCheckMsg(ctrl_id);
                }
            }
            break;

        case MSG_SET_UPDATE_POWER_SAVE_SETTING:
            AppendTwoTextCheckList(list_ctrl, TRUE);
            break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
            {
                if(PNULL != param)
                {
                    MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
                    if(MMISET_POWER_SAVE_ONOFF_LIST_CTRL_ID == ctrl_id)
                    {
                        cur_index = GUILIST_GetCurItemIndex(MMISET_POWER_SAVE_ONOFF_LIST_CTRL_ID);
                        if(1 == cur_index)//点中的是省电模式设置项
                        {
                            MMK_CreateWin((uint32*)MMISET_POWER_SAVE_SETTING_WIN_TAB,PNULL);
                        }
                        else
                        {
                            DealWithListCheckMsg(ctrl_id);
                        }
                    }
#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
                    else if(MMISET_POWER_PERCENT_DIS_LIST_CTRL_ID == ctrl_id)
                    {
                        DealWithListCheckMsg(ctrl_id);
                    }
#endif
                }
            }
            break;
#endif

        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            {
                MMI_CTRL_ID_T active_ctrl_id = MMK_GetActiveCtrlId(win_id);

                if(MMISET_POWER_SAVE_ONOFF_LIST_CTRL_ID == active_ctrl_id)
                {
                    cur_index = GUILIST_GetCurItemIndex(active_ctrl_id);
                    if(0 == cur_index)//点中的是省电模式设置项
                    {
                        DealWithListCheckMsg(active_ctrl_id);
                    }
                    if(1 == cur_index)//点中的是省电模式设置项
                    {
                        MMK_CreateWin((uint32*)MMISET_POWER_SAVE_SETTING_WIN_TAB,PNULL);
                    }
                }
                else if(MMISET_POWER_PERCENT_DIS_LIST_CTRL_ID == active_ctrl_id)
                {
                    DealWithListCheckMsg(active_ctrl_id);
                }
            }
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}
/*****************************************************************************/
//  Description :SetPowerSaveParam
//  Global resource dependence :
//  Author:juan.wu
//  Note: 设置省电模式参数
/*****************************************************************************/
LOCAL void SetPowerSaveParam(void)
{
    GUI_BG_T        title_bg = {0};
    GUIFORM_CHILD_WIDTH_T  child_width  = {0};
#ifdef MMI_DUAL_BATTERY_SUPPORT
    MMI_STRING_T        mtext_str    = {0};
    MMI_STRING_T        atext_str    = {0};
    uint8               char_num_len    = 0;
    uint8               num_str[16] = {0};
    DBAT_STATE_INFO_T*  dbat_info = DBAT_GetState();
#else
#ifndef MMI_BATTERY_PERCENT_IN_STATUSBAR
    MMI_STRING_T        bat_cap     = {0};
    uint32               bat_level   =  0;
    uint8                num_str[16] = {0};
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
    bat_level = p_chgmng_info->bat_remain_cap;
#endif
#endif
    child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
    child_width.add_data = 30;
    GUIFORM_SetChildWidth(MMISET_POWER_LEFT_FORM_CTRL_ID, MMISET_POWER_CONTENT_CTRL_ID, &child_width);

    title_bg.bg_type = GUI_BG_COLOR;
    title_bg.color   = 0xdedb;
    GUILABEL_SetTextById(MMISET_POWER_SAVE_TITLE_CTRL_ID, TXT_SET_POWER_SAVING, FALSE);
    GUILABEL_SetFont(MMISET_POWER_SAVE_TITLE_CTRL_ID, MMI_DEFAULT_SMALL_FONT, MMI_THEME1_FONT_COLOR);
    GUILABEL_SetBg(MMISET_POWER_SAVE_TITLE_CTRL_ID, &title_bg);

    GUILABEL_SetTextById(MMISET_POWER_TITLE_LABEL_CTRL_ID, TXT_SET_POWER, FALSE);
    GUILABEL_SetFont(MMISET_POWER_TITLE_LABEL_CTRL_ID, MMI_DEFAULT_SMALL_FONT, MMI_THEME1_FONT_COLOR);
    GUILABEL_SetBg(MMISET_POWER_TITLE_LABEL_CTRL_ID, &title_bg);

#ifdef MMI_DUAL_BATTERY_SUPPORT
    //主电池显示:
    if(PNULL == dbat_info->p_bat[DBAT_MAIN_BAT])
    {
        GUIFORM_SetVisible(MMISET_POWER_LEFT_FORM_CTRL_ID, FALSE, FALSE);
    }
    else
    {
        SCI_MEMSET(num_str,0,sizeof(char)*16);
        mtext_str.wstr_ptr = SCI_ALLOCA(sizeof(wchar)*5);
        if(PNULL == mtext_str.wstr_ptr)
        {
            //SCI_TRACE_LOW:"SetPowerSaveParam PNULL == mtext_str.wstr_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_PHONEWIN_2550_112_2_18_2_51_35_260,(uint8*)"");
        }
        GUIFORM_SetVisible(MMISET_POWER_LEFT_FORM_CTRL_ID, TRUE, FALSE);
        GUILABEL_SetTextById(MMISET_POWER_LEFT_TITLE_CTRL_ID, TXT_DUAL_BTRY_MAIN_CAP_LABEL_TEXT, FALSE);
        if(DBAT_MAIN_BAT == dbat_info->cur_bat)
        {
            sprintf((char*)num_str, "%d%s", (int)dbat_info->chgmng_info.bat_remain_cap, "%");
        }
        else
        {
            sprintf((char*)num_str, "%d%s", (int)dbat_info->p_bat[DBAT_MAIN_BAT]->remain_cap, "%");
        }
        MMIAPICOM_StrToWstr(num_str, mtext_str.wstr_ptr);
        mtext_str.wstr_len = MMIAPICOM_Wstrlen(mtext_str.wstr_ptr);
        GUILABEL_SetText(MMISET_POWER_CONTENT_CTRL_ID, &mtext_str, FALSE);
        SCI_FREE(mtext_str.wstr_ptr);
    }
    //副电池显示:
    if(PNULL == dbat_info->p_bat[DBAT_AUX_BAT])
    {
        GUIFORM_SetVisible(MMISET_AUX_POWER_LEFT_FORM_CTRL_ID, FALSE, FALSE);
    }
    else
    {
        GUIFORM_SetChildWidth(MMISET_AUX_POWER_LEFT_FORM_CTRL_ID, MMISET_AUX_POWER_CONTENT_CTRL_ID, &child_width);
        SCI_MEMSET(num_str,0,sizeof(char)*16);
        atext_str.wstr_ptr = SCI_ALLOCA(sizeof(wchar)*5);
        if(PNULL == atext_str.wstr_ptr)
        {
            //SCI_TRACE_LOW:"SetPowerSaveParam PNULL == mtext_str.wstr_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_PHONEWIN_2579_112_2_18_2_51_35_261,(uint8*)"");
        }

        GUIFORM_SetVisible(MMISET_AUX_POWER_LEFT_FORM_CTRL_ID, TRUE, FALSE);
        GUILABEL_SetTextById(MMISET_AUX_POWER_LEFT_TITLE_CTRL_ID, TXT_DUAL_BTRY_AUX_CAP_LABEL_TEXT, FALSE);
        if(DBAT_AUX_BAT == dbat_info->cur_bat)
        {
            sprintf((char*)num_str, "%d%s", (int)dbat_info->chgmng_info.bat_remain_cap, "%");
        }
        else
        {
            sprintf((char*)num_str, "%d%s", (int)dbat_info->p_bat[DBAT_AUX_BAT]->remain_cap, "%");
        }
        MMIAPICOM_StrToWstr(num_str, atext_str.wstr_ptr);
        atext_str.wstr_len = MMIAPICOM_Wstrlen(atext_str.wstr_ptr);
        GUILABEL_SetText(MMISET_AUX_POWER_CONTENT_CTRL_ID, &atext_str, FALSE);
        SCI_FREE(atext_str.wstr_ptr);
    }
#else
#ifndef MMI_BATTERY_PERCENT_IN_STATUSBAR
    SCI_MEMSET(num_str,0,sizeof(char)*16);
    sprintf((char*)num_str, "%d%s", (int)bat_level, "%");
    bat_cap.wstr_ptr = SCI_ALLOCA(sizeof(wchar)*5);
    if(PNULL == bat_cap.wstr_ptr)
    {
        //SCI_TRACE_LOW:"SetPowerSaveParam SCI_ALLOCA error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_PHONEWIN_2603_112_2_18_2_51_35_262,(uint8*)"");
        return ;
    }
    MMIAPICOM_StrToWstr(num_str, bat_cap.wstr_ptr);
    bat_cap.wstr_len = MMIAPICOM_Wstrlen(bat_cap.wstr_ptr);
    GUILABEL_SetText(MMISET_POWER_CONTENT_CTRL_ID, &bat_cap, FALSE);
    GUILABEL_SetTextById(MMISET_POWER_LEFT_TITLE_CTRL_ID, TXT_BTRY_CAP_LABEL_TEXT, FALSE);
    SCI_FREE(bat_cap.wstr_ptr);
#endif
#endif
}


/*****************************************************************************/
//  Description :AppendPowerSavingOnOffList
//  Global resource dependence :
//  Author:juan.wu
//  Note: append 省电模式列表
/*****************************************************************************/
LOCAL void AppendTwoTextCheckList(MMI_CTRL_ID_T ctrl_id,  BOOLEAN is_update)
{
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};

    item_t.item_data_ptr = &item_data;
    item_t.item_state |= GUIITEM_STATE_CONTENT_CHECK;
    if(MMISET_POWER_SAVE_ONOFF_LIST_CTRL_ID == ctrl_id)
    {
        item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = TXT_ENERGY_SAVING;
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[2].item_data.text_id = TXT_SET_START_POWER_SAVING_AS_SETTING;
        item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
        if(MMIAPISET_GetPowerSavingSwitch())
        {
            item_data.item_content[1].item_data.image_id= IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {
            item_data.item_content[1].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;
        }
    }
#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
    else if(MMISET_POWER_PERCENT_DIS_LIST_CTRL_ID == ctrl_id)
    {
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = TXT_SET_BATTERY_PERCENTAGE;
        item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
        if(MMIAPISET_GetBatteryPercentDisSwitch())
        {
            item_data.item_content[1].item_data.image_id= IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {
            item_data.item_content[1].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;
        }
    }
#endif
    if(is_update)
    {
        GUILIST_ReplaceItem(ctrl_id, &item_t, 0);
        MMK_SendMsg(MMISET_POWER_SAVE_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
    else
    {
        GUILIST_AppendItem(ctrl_id, &item_t);
    }
}
/*****************************************************************************/
//  Description :AppendPowerSavingSettingList
//  Global resource dependence :
//  Author:juan.wu
//  Note: 省电模式设置列表
/*****************************************************************************/
LOCAL void AppendOneTextOneIconList(
        MMI_CTRL_ID_T ctrl_id,
        MMI_TEXT_ID_T text_id,
        MMI_IMAGE_ID_T image_id
        )
{
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};

    item_t.item_data_ptr = &item_data;
    if(IMAGE_NULL != image_id)
    {
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
        item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = image_id;
    }
    else
    {
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_RADIO;
    }
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    GUILIST_AppendItem(ctrl_id, &item_t);
}
/*****************************************************************************/
//  Description :MMIAPISET_SetPowerSavingSwitch
//  Global resource dependence :
//  Author:juan.wu
//  Note:设置省电模式
/*****************************************************************************/
PUBLIC void MMIAPISET_SetPowerSavingSwitch(BOOLEAN is_switch)
{
    MMINV_WRITE(MMINV_SET_POWER_SAVING_SWITCH, &is_switch);
}
/*****************************************************************************/
//  Description :MMIAPISET_GetPowerSavingSwitch
//  Global resource dependence :
//  Author:juan.wu
//  Note: 当前省电模式是否开启,TRUE为开启，FALSE 为关闭
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetPowerSavingSwitch(void)
{
    MN_RETURN_RESULT_E          return_value    =   MN_RETURN_FAILURE;
    BOOLEAN                     is_switch          =   FALSE;

    MMINV_READ(MMINV_SET_POWER_SAVING_SWITCH, &is_switch, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_switch= FALSE;
        MMINV_WRITE(MMINV_SET_POWER_SAVING_SWITCH, &is_switch);
    }
    return is_switch;
}
/*****************************************************************************/
//  Description :MMIAPISET_GetPowerSavingSwitch
//  Global resource dependence :
//  Author:juan.wu
//  Note: 当前省电模式设置
/*****************************************************************************/
PUBLIC MMISET_POWER_SETTING_ITEM_T MMIAPISET_GetPowerSavingSetting(void)
{
    MN_RETURN_RESULT_E              return_value    =   MN_RETURN_FAILURE;
    MMISET_POWER_SETTING_ITEM_T     power_sav_set       =   {0};
    SET_NV_ITEM_E                   nv_item   =  MMINV_SET_POWER_SAVING_SETTING;

    //SCI_TRACE_LOW:"MMIAPISET_GetPowerSavingSetting nv_item = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_PHONEWIN_2715_112_2_18_2_51_36_263,(uint8*)"d",nv_item);
    MMINV_READ(MMINV_SET_POWER_SAVING_SETTING, &power_sav_set, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        power_sav_set.is_active = FALSE;
        power_sav_set.is_fb_bg_data= TRUE;
#ifdef WIFI_SUPPORT
        power_sav_set.is_fb_wlan= TRUE;
#endif
#ifdef BLUETOOTH_SUPPORT
        power_sav_set.is_fb_bluetooth= TRUE;
#endif
        power_sav_set.is_fb_anim= TRUE;
        power_sav_set.is_fb_ss= TRUE;
        power_sav_set.is_fb_keypad_light= TRUE;
        power_sav_set.is_fb_anim_wallpaper= TRUE;
        power_sav_set.is_fb_alert_ring = TRUE;
        power_sav_set.lcd_contrast = MMISET_CONTRAST_THREE;
        power_sav_set.schedule_setting  =  MMISET_POWER_SAVING_SETTING_TWE;
        MMINV_WRITE(MMINV_SET_POWER_SAVING_SETTING, &power_sav_set);
    }
    return power_sav_set;
}

/*****************************************************************************/
//  Description :MMIAPISET_GetPowerSavingSwitch
//  Global resource dependence :
//  Author:juan.wu
//  Note: 设置省电模式参数
/*****************************************************************************/
PUBLIC void MMIAPISET_SetPowerSavingSetting(MMISET_POWER_SETTING_ITEM_T power_sav_set)
{
    MMINV_WRITE(MMINV_SET_POWER_SAVING_SETTING, &power_sav_set);
    UpdateStatusBar(power_sav_set.is_active);
}

/*****************************************************************************/
//  Description :HandlePowerSaveSettingWindow
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePowerSaveSettingWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_CTRL_ID_T    list1_ctrl_id = MMISET_POWER_SAVE_SETTING_LIST1_CTRL_ID;
    MMI_CTRL_ID_T    list2_ctrl_id = MMISET_POWER_SAVE_SETTING_LIST2_CTRL_ID;
    MMI_IMAGE_ID_T   icon_id       = 0;
    MMI_RESULT_E     result  = MMI_RESULT_TRUE;
    MMISET_POWER_SETTING_ITEM_T   power_sav_setting = {0};
    MMISET_PS_SETTING_LIST1_E           list1_index = MMISET_PS_SETTING_SCH;
    MMISET_PS_SETTING_LIST2_E           list2_index = MMISET_PS_SETTING_ANIM;
    uint16           bar_width = 0;
    uint16           bar_height = 0;
    power_sav_setting = MMIAPISET_GetPowerSavingSetting();

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,power_sav_setting.lcd_contrast);
            GUIFORM_SetType(MMISET_POWER_SAVE_SETTING_FORM_CTRL_ID,GUIFORM_TYPE_TP);
            GUIFORM_PermitChildBg(MMISET_POWER_SAVE_SETTING_FORM_CTRL_ID, FALSE);
            GUIFORM_PermitChildBorder(MMISET_POWER_SAVE_SETTING_FORM_CTRL_ID, FALSE);
            GUIFORM_SetMargin(MMISET_POWER_SAVE_SETTING_FORM_CTRL_ID, 0);
            GUIFORM_SetStyle(MMISET_POWER_SAVE_SETTING_BACKLIGHT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_PermitChildBg(MMISET_POWER_SAVE_SETTING_BACKLIGHT_FORM_CTRL_ID, FALSE);
            GUIFORM_PermitChildBorder(MMISET_POWER_SAVE_SETTING_BACKLIGHT_FORM_CTRL_ID, FALSE);
            GUILABEL_SetTextById(MMISET_POWER_SAVE_SETTING_BACKLIGHT_TITLE_CTRL_ID, TXT_SET_CONTRAST, FALSE);

#ifndef MMI_GUI_STYLE_MINISCREEN
            GUIRES_GetImgWidthHeight(&bar_width,&bar_height,IMAGE_SET_CONTRAST_BAR_FG,win_id);
#else
            GUIRES_GetImgWidthHeight(&bar_width,&bar_height,IMAGE_SET_CONTRAST_BAR_FG_NEW,win_id);
#endif

            GUIOWNDRAW_SetHeight(MMISET_POWER_SAVE_SETTING_BACKLIGHT_ANIM_CTRL_ID, bar_height + 10);

            GUILIST_SetMaxItem(list1_ctrl_id,4, FALSE);
            GUILIST_SetMaxItem(list2_ctrl_id,5, FALSE);
            AppendTwoTextOneIconList(list1_ctrl_id,  TXT_POWER_SAVING_MODE_SCHEDULE,  GetTxtID(power_sav_setting.schedule_setting),  IMAGE_COMMON_RIGHT_ARROW, win_id, 0, FALSE);

            icon_id = GetIconID(power_sav_setting.is_fb_bg_data);
            AppendTwoTextOneIconList(list1_ctrl_id,  TXT_POWER_SAVING_MODE_FORBIDDEN_BG_DATA,  TXT_POWER_SAVING_MODE_FORBIDDEN_BG_DATA_EX,  icon_id, win_id, 0, FALSE);
#ifdef WIFI_SUPPORT
            icon_id = GetIconID(power_sav_setting.is_fb_wlan);
            AppendTwoTextOneIconList(list1_ctrl_id,  TXT_POWER_SAVING_MODE_FORBIDDEN_WLAN,  TXT_POWER_SAVING_MODE_FORBIDDEN_WLAN_EX,  icon_id, win_id, 0, FALSE);
#endif
#ifdef BLUETOOTH_SUPPORT
            icon_id = GetIconID(power_sav_setting.is_fb_bluetooth);
            AppendTwoTextOneIconList(list1_ctrl_id,  TXT_POWER_SAVING_MODE_FORBIDDEN_BLUETOOTH,  TXT_POWER_SAVING_MODE_FORBIDDEN_BLUETOOTH_EX,  icon_id, win_id, 0, FALSE);
#endif
            icon_id = GetIconID(power_sav_setting.is_fb_anim);
            AppendTwoTextOneIconList(list2_ctrl_id,  TXT_POWER_SAVING_MODE_FORBIDDEN_ANIM,  TXT_POWER_SAVING_MODE_FORBIDDEN_ANIM_EX,  icon_id, win_id, 0, FALSE);
#ifdef SCREENSAVER_SUPPORT
            icon_id = GetIconID(power_sav_setting.is_fb_ss);
            AppendTwoTextOneIconList(list2_ctrl_id,  TXT_POWER_SAVING_MODE_FORBIDDEN_SCREEN_SAVER,  TXT_POWER_SAVING_MODE_FORBIDDEN_SCREEN_SAVER_EX,  icon_id, win_id, 0, FALSE);
#endif
            icon_id = GetIconID(power_sav_setting.is_fb_keypad_light);
            AppendTwoTextOneIconList(list2_ctrl_id,  TXT_POWER_SAVING_MODE_KEYPAD_SETTING,  TXT_POWER_SAVING_MODE_KEYPAD_SETTING_EX,  icon_id, win_id, 0, FALSE);
            icon_id = GetIconID(power_sav_setting.is_fb_anim_wallpaper);
            AppendTwoTextOneIconList(list2_ctrl_id,  TXT_COMMON_SET_WALLPAPER,  TXT_POWER_SAVING_MODE_WALLPAPER_SETTING_EX,  icon_id, win_id, 0, FALSE);
            icon_id = GetIconID(power_sav_setting.is_fb_alert_ring);
            AppendTwoTextOneIconList(list2_ctrl_id,  TXT_TONE,  TXT_POWER_SAVING_MODE_ALERT_SETTING_EX,  icon_id, win_id, 0, FALSE);
            MMK_SetAtvCtrl(win_id, MMISET_POWER_SAVE_SETTING_FORM_CTRL_ID);
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
            if(MMISET_POWER_SAVE_SETTING_BACKLIGHT_ANIM_CTRL_ID == MMK_GetActiveCtrlId(win_id))
            {
                GUI_POINT_T     tp_point = {0};
                MMI_TP_STATUS_E tp_status = MMI_TP_NONE;

                MMK_GetMultiTPMsg(&tp_status,&tp_point);

                MMISET_HandleContrastTPDown(
                        &tp_point,
                        win_id,
                        MMISET_POWER_SAVE_SETTING_BACKLIGHT_ANIM_CTRL_ID,
                        TRUE);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_NOTIFY_LIST_CHECK_CONTENT:
            if(list1_ctrl_id == MMK_GetActiveCtrlId(win_id))
            {
                list1_index = GUILIST_GetCurItemIndex(list1_ctrl_id);/*lint !e64*/
                switch(list1_index)
                {
                    case MMISET_PS_SETTING_SCH:
                        MMK_CreatePubListWin((uint32*)MMISET_POWER_SAVE_SCHEDULE_WIN_TAB,PNULL);
                        break;

                    case MMISET_PS_SETTING_BG:
                        power_sav_setting.is_fb_bg_data = !power_sav_setting.is_fb_bg_data;
                        AppendTwoTextOneIconList(list1_ctrl_id,  TXT_POWER_SAVING_MODE_FORBIDDEN_BG_DATA,  TXT_POWER_SAVING_MODE_FORBIDDEN_BG_DATA_EX, GetIconID(power_sav_setting.is_fb_bg_data), win_id, list1_index, TRUE);
                        break;

#ifdef WIFI_SUPPORT
                    case MMISET_PS_SETTING_WLAN:
                        power_sav_setting.is_fb_wlan= !power_sav_setting.is_fb_wlan;
                        AppendTwoTextOneIconList(list1_ctrl_id,  TXT_POWER_SAVING_MODE_FORBIDDEN_WLAN,  TXT_POWER_SAVING_MODE_FORBIDDEN_WLAN_EX, GetIconID(power_sav_setting.is_fb_wlan), win_id, list1_index, TRUE);
                        break;
#endif
#ifdef BLUETOOTH_SUPPORT
                    case MMISET_PS_SETTING_BT:
                        power_sav_setting.is_fb_bluetooth= !power_sav_setting.is_fb_bluetooth;
                        AppendTwoTextOneIconList(list1_ctrl_id,  TXT_POWER_SAVING_MODE_FORBIDDEN_BLUETOOTH,  TXT_POWER_SAVING_MODE_FORBIDDEN_BLUETOOTH_EX, GetIconID(power_sav_setting.is_fb_bluetooth), win_id, list1_index, TRUE);
                        break;
#endif
                    default:
                        //SCI_TRACE_LOW:"HandlePowerSaveSettingWindow list1 index = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_PHONEWIN_2850_112_2_18_2_51_36_264,(uint8*)"d",list1_index);
                        break;

                }
            }
            else if(list2_ctrl_id == MMK_GetActiveCtrlId(win_id))
            {
                list2_index = GUILIST_GetCurItemIndex(list2_ctrl_id);/*lint !e64*/

                switch(list2_index)
                {
                    case MMISET_PS_SETTING_ANIM:
                        power_sav_setting.is_fb_anim= !power_sav_setting.is_fb_anim;
                        AppendTwoTextOneIconList(list2_ctrl_id,  TXT_POWER_SAVING_MODE_FORBIDDEN_ANIM,  TXT_POWER_SAVING_MODE_FORBIDDEN_ANIM_EX,  GetIconID(power_sav_setting.is_fb_anim), win_id, list2_index, TRUE);
                        break;

#ifdef SCREENSAVER_SUPPORT
                    case MMISET_PS_SETTING_SS:
                        power_sav_setting.is_fb_ss= !power_sav_setting.is_fb_ss;
                        AppendTwoTextOneIconList(list2_ctrl_id,  TXT_POWER_SAVING_MODE_FORBIDDEN_SCREEN_SAVER,  TXT_POWER_SAVING_MODE_FORBIDDEN_SCREEN_SAVER_EX,  GetIconID(power_sav_setting.is_fb_ss), win_id, list2_index, TRUE);
                        break;
#endif

                    case MMISET_PS_SETTING_KEYPAD:
                        power_sav_setting.is_fb_keypad_light= !power_sav_setting.is_fb_keypad_light;
                        AppendTwoTextOneIconList(list2_ctrl_id,  TXT_POWER_SAVING_MODE_KEYPAD_SETTING,  TXT_POWER_SAVING_MODE_KEYPAD_SETTING_EX,  GetIconID(power_sav_setting.is_fb_keypad_light), win_id, list2_index, TRUE);
                        break;

                    case MMISET_PS_SETTING_WALLPAPER:
                        power_sav_setting.is_fb_anim_wallpaper= !power_sav_setting.is_fb_anim_wallpaper;
                        AppendTwoTextOneIconList(list2_ctrl_id,  TXT_COMMON_SET_WALLPAPER,  TXT_POWER_SAVING_MODE_WALLPAPER_SETTING_EX,  GetIconID(power_sav_setting.is_fb_anim_wallpaper), win_id, list2_index, TRUE);
                        break;

                    case MMISET_PS_SETTING_ALERT:
                        power_sav_setting.is_fb_alert_ring= !power_sav_setting.is_fb_alert_ring;
                        AppendTwoTextOneIconList(list2_ctrl_id,  TXT_TONE,  TXT_POWER_SAVING_MODE_ALERT_SETTING_EX,  GetIconID(power_sav_setting.is_fb_alert_ring), win_id, list2_index, TRUE);
                        break;

                    default:
                        //SCI_TRACE_LOW:"HandlePowerSaveSettingWindow list2 index = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_PHONEWIN_2887_112_2_18_2_51_36_265,(uint8*)"d",list2_index);
                        break;
                }
            }
            MMIAPISET_SetPowerSavingSetting(power_sav_setting);
#ifdef MMI_VIDEOWALLPAPER_SUPPORT//NEWMS00162005
            if(power_sav_setting.is_fb_anim_wallpaper && power_sav_setting.is_active && MMIAPISETVWP_IsOpen())
            {
                MMIAPISET_ResetDefaultWallpaper();
            }
#endif
            break;

        case MSG_APP_LEFT:
        case MSG_APP_RIGHT:
            {
                if(MMISET_POWER_SAVE_SETTING_BACKLIGHT_ANIM_CTRL_ID == MMK_GetActiveCtrlId(win_id))
                {
                    uint8 lcd_contrast = power_sav_setting.lcd_contrast;
                    int16           display_x = 0;
                    int16           display_y = 0;
                    //uint16          bar_width = 0;
                    //uint16          bar_height = 0;
                    GUI_RECT_T      display_rect = {0};
                    GUI_RECT_T      ctrl_display_rect = {0};
                    GUI_RECT_T      img_rect = {0};

                    if(msg_id== MSG_APP_LEFT)
                    {
                        if(lcd_contrast>0)
                        {
                            lcd_contrast--;
                        }
                    }
                    else if(msg_id== MSG_APP_RIGHT)
                    {
                        if(lcd_contrast<MMISET_CONTRAST_SEVEN)
                        {
                            lcd_contrast++;
                        }
                    }

#ifndef MMI_GUI_STYLE_MINISCREEN
                    GUIRES_GetImgWidthHeight(&bar_width,
                            &bar_height,
                            IMAGE_SET_CONTRAST_BAR_FG,
                            win_id);
#else
                    GUIRES_GetImgWidthHeight(&bar_width,
                            &bar_height,
                            IMAGE_SET_CONTRAST_BAR_FG_NEW,
                            win_id);
#endif

                    GUIOWNDRAW_GetRect(MMISET_POWER_SAVE_SETTING_BACKLIGHT_ANIM_CTRL_ID,&display_rect);
                    display_x = display_rect.left;
                    display_y = display_rect.top;
                    GUIANIM_AdjustDisplayPosition(&display_x,
                            &display_y,
                            bar_width,
                            bar_height,
                            display_rect,
                            GUIANIM_ALIGN_HVMIDDLE);

                    //set display rect
                    display_rect.left   = display_x;
                    display_rect.top    = display_y;
                    display_rect.right  = (int16)(display_rect.left + bar_width - 1);
                    display_rect.bottom = (int16)(display_rect.top + bar_height - 1);

                    GUIOWNDRAW_GetDisplayRect(MMISET_POWER_SAVE_SETTING_BACKLIGHT_ANIM_CTRL_ID,&ctrl_display_rect);

                    if (GUI_IntersectRect(&display_rect,display_rect,ctrl_display_rect))
                    {
                        //set image rect
                        img_rect.left   = (int16)(display_rect.left - display_x);
                        img_rect.top    = (int16)(display_rect.top - display_y);
                        img_rect.right  = (int16)(display_rect.right - display_x);
                        img_rect.bottom = (int16)(display_rect.bottom - display_y);

#ifndef MMI_GUI_STYLE_MINISCREEN
                        //display bg
                        GUIRES_DisplayImg(PNULL,
                                &display_rect,
                                &img_rect,
                                win_id,
                                IMAGE_SET_CONTRAST_BAR_BG,
                                MMITHEME_GetDefaultLcdDev());
#else
                        GUIRES_DisplayImg(PNULL,
                                &display_rect,
                                &img_rect,
                                win_id,
                                IMAGE_SET_CONTRAST_BAR_BG_NEW,
                                MMITHEME_GetDefaultLcdDev());
#endif

                        //set image rect
                        img_rect.right = img_rect.left + (lcd_contrast*bar_width/MMISET_CONTRAST_SEVEN) - 1;

#ifndef MMI_GUI_STYLE_MINISCREEN
                        //display fg
                        GUIRES_DisplayImg(PNULL,
                                &display_rect,
                                &img_rect,
                                win_id,
                                IMAGE_SET_CONTRAST_BAR_FG,
                                MMITHEME_GetDefaultLcdDev());
#else
                        GUIRES_DisplayImg(PNULL,
                                &display_rect,
                                &img_rect,
                                win_id,
                                IMAGE_SET_CONTRAST_BAR_FG_NEW,
                                MMITHEME_GetDefaultLcdDev());
#endif

                        power_sav_setting.lcd_contrast = lcd_contrast;
                        MMIAPISET_SetPowerSavingSetting(power_sav_setting);
                        MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,lcd_contrast);
                        if(power_sav_setting.is_active)
                        {
                            MMIAPISET_SetCurrentContrast(lcd_contrast);
                        }
                        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                    }
                }
            }
            break;

        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            if(list1_ctrl_id == MMK_GetActiveCtrlId(win_id) )
            {
                if(MMISET_PS_SETTING_SCH == GUILIST_GetCurItemIndex(list1_ctrl_id))
                {
                    //MMK_SendMsg(win_id,MSG_CTL_PENOK, 0);
                    MMK_CreatePubListWin((uint32*)MMISET_POWER_SAVE_SCHEDULE_WIN_TAB,PNULL);
                }
                else
                {
                    MMK_SendMsg(win_id,MSG_NOTIFY_LIST_CHECK_CONTENT, 0);
                }
            }
            else if(list2_ctrl_id == MMK_GetActiveCtrlId(win_id))
            {
                MMK_SendMsg(win_id,MSG_NOTIFY_LIST_CHECK_CONTENT, 0);
            }
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            {
                uint8 cur_contrast = 0;

                if(power_sav_setting.is_active)
                {
                    cur_contrast = power_sav_setting.lcd_contrast;
                }
                else
                {
                    cur_contrast = MMIAPISET_GetCurrentContrast();
                }

                MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,cur_contrast);
            }
            break;


        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}

/*****************************************************************************/
//  Description :HandlePowerSaveScheduleWindow
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePowerSaveScheduleWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_RESULT_E   result = MMI_RESULT_TRUE;
    MMISET_POWER_SAVING_SETTING_E         cur_sch = 0;
    MMISET_POWER_SAVING_SETTING_E          i = MMISET_POWER_SAVING_SETTING_TWE;
    MMI_CTRL_ID_T  ctrl_id = MMISET_POWER_SAVE_SCHEDULE_LIST_CTRL_ID;
    MMISET_POWER_SETTING_ITEM_T   power_sav_setting = {0};

    power_sav_setting = MMIAPISET_GetPowerSavingSetting();
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUILIST_SetMaxItem( ctrl_id, MMISET_POWER_SAVING_SETTING_MAX, FALSE);
            for(i = 0; i< MMISET_POWER_SAVING_SETTING_MAX; i++)
            {
                AppendOneTextOneIconList(ctrl_id,GetTxtID(i),IMAGE_NULL);
            }
            GUILIST_SetSelectedItem(ctrl_id, power_sav_setting.schedule_setting, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id,power_sav_setting.schedule_setting);
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            MMK_SetAtvCtrl(win_id,MMISET_POWER_SAVE_SCHEDULE_LIST_CTRL_ID);
            break;
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            cur_sch = GUILIST_GetCurItemIndex(ctrl_id);/*lint !e64*/
            power_sav_setting.schedule_setting = cur_sch;
            MMIAPISET_SetPowerSavingSetting(power_sav_setting);
            AppendTwoTextOneIconList(MMISET_POWER_SAVE_SETTING_LIST1_CTRL_ID, TXT_POWER_SAVING_MODE_SCHEDULE, GetTxtID(cur_sch),  IMAGE_COMMON_RIGHT_ARROW, MMISET_POWER_SAVE_SETTING_WIN_ID, 0,TRUE );
            MMK_CloseWin(win_id);
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;

    }
    return result;
}
/*****************************************************************************/
//  Description :AppendTwoTextOneIconList
//  Global resource dependence :
//  Author:juan.wu
//  Note: 如果is_update为FALSE,那么win_id ,pos两个参数都可以不用关注，填0也行
/*****************************************************************************/
LOCAL void AppendTwoTextOneIconList(
        MMI_CTRL_ID_T ctrl_id,
        MMI_TEXT_ID_T text1,
        MMI_TEXT_ID_T text2,
        MMI_IMAGE_ID_T  image_id,
        MMI_WIN_ID_T  win_id,
        uint16  pos,
        BOOLEAN is_update
        )
{
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};

    item_t.item_data_ptr = &item_data;
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_state |= GUIITEM_STATE_CONTENT_CHECK;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text1;
    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = image_id;
    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[2].item_data.text_id = text2;

    if(is_update)
    {
        GUILIST_ReplaceItem(ctrl_id, &item_t, pos);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
    else
    {
        GUILIST_AppendItem(ctrl_id, &item_t);
    }


}
/*****************************************************************************/
//  Description :GetIconID
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T  GetIconID(BOOLEAN is_checked)
{
    MMI_IMAGE_ID_T icon_id= IMAGE_CHECK_UNSELECTED_ICON;

    if(is_checked)
    {
        icon_id = IMAGE_CHECK_SELECTED_ICON;
    }
    return icon_id;
}

/*****************************************************************************/
//  Description :GetTxtID
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T   GetTxtID(MMISET_POWER_SAVING_SETTING_E index)
{
    MMI_TEXT_ID_T text_id[] = {
        TXT_POWER_SAVING_LEFT_TWE,
        TXT_POWER_SAVING_LEFT_FOURTY,
        TXT_POWER_SAVING_LEFT_SIXTY
    };

    return text_id[index];

}

/*****************************************************************************/
//  Description :MMIAPISET_IsProgramRunning
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 检查各个后台程序是否开启
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsProgramRunning()
{
    BOOLEAN is_ret = FALSE;

    //关闭蓝牙
#ifdef BLUETOOTH_SUPPORT
    is_ret = MMIAPIBT_IsBtOn();
    if(is_ret)
    {
        //SCI_TRACE_LOW:"MMIAPISET_IsProgramRunning bt is open."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_PHONEWIN_3163_112_2_18_2_51_37_266,(uint8*)"");
        return is_ret;
    }
#endif

    //关闭WIFI
#ifdef WIFI_SUPPORT
    is_ret = MMIAPIWIFI_GetIsWlanOn();
    if(is_ret)
    {
        //SCI_TRACE_LOW:"MMIAPISET_IsProgramRunning wifi is open."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_PHONEWIN_3173_112_2_18_2_51_37_267,(uint8*)"");
        return is_ret;
    }
#endif

#ifdef JAVA_SUPPORT_IA//目前只有IA JAVA 有对外的退出接口，MYRIAD以后再加上
    is_ret = MMIAPIJAVA_IsJavaRuning();
    if(is_ret)
    {
        //SCI_TRACE_LOW:"MMIAPISET_IsProgramRunning java is open."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_PHONEWIN_3182_112_2_18_2_51_37_268,(uint8*)"");
        return is_ret;
    }
#endif

#ifdef BROWSER_SUPPORT
    is_ret = MMIAPIBROWSER_IsRunning();
    if(is_ret)
    {
        //SCI_TRACE_LOW:"MMIAPISET_IsProgramRunning browser is open."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_PHONEWIN_3191_112_2_18_2_51_37_269,(uint8*)"");
        return is_ret;
    }
#endif

    return is_ret;
}

/*****************************************************************************/
//  Description :MMIAPISET_OpenPowerSettingMode
//  Global resource dependence :
//  Author:juan.wu
//  Note: 开启省电模式
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenPowerSettingMode(void)
{
    MMISET_KEYBOARD_LIGHT_SET_T  keyborad_setting = {0};
    MMISET_POWER_SETTING_ITEM_T power_sav_setting = MMIAPISET_GetPowerSavingSetting();
#ifdef SCREENSAVER_SUPPORT
    MMISET_SCREENSAVER_INFO_T    ssaver_info = {0};
#endif
    uint8                        lcd_contrast = 0;

    power_sav_setting.is_active = TRUE;
    MMIAPISET_SetPowerSavingSetting(power_sav_setting);
    keyborad_setting.keyboard_type = MMISET_KEYBOARD_LIGHT_CLOSE;
    //关闭蓝牙
#ifdef BLUETOOTH_SUPPORT
    if(power_sav_setting.is_fb_bluetooth)
    {
        MMIAPIBT_OpenBlueTooth(FALSE);
    }
#endif
    //关闭WIFI
#ifdef WIFI_SUPPORT
    if(power_sav_setting.is_fb_wlan)
    {
        MMIAPIWIFI_SetInfo(FALSE);
        MMIAPIWIFI_Off();
    }
#endif
    //退出后台运行
    if(power_sav_setting.is_fb_bg_data)
    {
#ifdef JAVA_SUPPORT_IA//目前只有IA JAVA 有对外的退出接口，MYRIAD以后再加上
        if(MMIAPIJAVA_IsJavaRuning())
        {
            MMIAPIJAVA_ExitJAVA();
        }
#endif

#ifdef BROWSER_SUPPORT
        if (MMIAPIBROWSER_IsRunning())
        {
            MMIAPIBROWSER_Close();
        }
#endif
    }
    //关闭动画
    if(power_sav_setting.is_fb_anim)
    {
        MMISET_CloseAllAnimSet();
    }
    //关闭屏保
#ifdef SCREENSAVER_SUPPORT
    if(power_sav_setting.is_fb_ss)
    {
        //        MMIAPIIDLESS_StopTimer();
        MMIAPISET_SetScreenSaverInfo(&ssaver_info);
    }
#endif
    //关闭按键背光
    if(power_sav_setting.is_fb_keypad_light)
    {
        MMISET_SetKeyBoardLightTime(keyborad_setting);
        MMIDEFAULT_SetKeybadBackLight(FALSE);
    }
    //恢复成默认墙纸
    if(power_sav_setting.is_fb_anim_wallpaper)
    {
        MMIAPISET_ResetDefaultWallpaper();
    }
    //关闭当前情景模式中的其它铃声
    if(power_sav_setting.is_fb_alert_ring)
    {
        MMIAPIENVSET_CloseActModeOherRingSet();
    }
    //刷新状态栏
    UpdateStatusBar(TRUE);
    //修改背光亮度
    lcd_contrast = MMIAPISET_GetCurrentContrast();
    if(lcd_contrast != power_sav_setting.lcd_contrast)
    {
        MMIAPISET_SetCurrentContrast(power_sav_setting.lcd_contrast);
        MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,power_sav_setting.lcd_contrast);
        MMIAPISET_SetMainLCDContrast();
    }
}

/*****************************************************************************/
//  Description :MMIAPISET_ResetPowerSavSetting
//  Global resource dependence :
//  Author:juan.wu
//  Note: 恢复出厂设置
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetPowerSavSetting(void)
{
    MMISET_POWER_SETTING_ITEM_T power_sav_set = {0};

    MMIAPISET_SetPowerSavingSwitch(FALSE);
    power_sav_set.is_active = FALSE;
    power_sav_set.is_fb_bg_data= TRUE;
#ifdef WIFI_SUPPORT
    power_sav_set.is_fb_wlan= TRUE;
#endif
#ifdef BLUETOOTH_SUPPORT
    power_sav_set.is_fb_bluetooth= TRUE;
#endif
    power_sav_set.is_fb_anim= TRUE;
    power_sav_set.is_fb_ss= TRUE;
    power_sav_set.is_fb_keypad_light= TRUE;
    power_sav_set.is_fb_anim_wallpaper= TRUE;
    power_sav_set.is_fb_alert_ring = TRUE;
    power_sav_set.lcd_contrast  = MMISET_CONTRAST_THREE;

    power_sav_set.schedule_setting  =  MMISET_POWER_SAVING_SETTING_TWE;

    MMIAPISET_SetPowerSavingSetting(power_sav_set);
    MMIAPIENVSET_ResetActModeOtherRingSet();
}
/*****************************************************************************/
//  Description :UpdateStatusBar
//  Global resource dependence :
//  Author:juan.wu
//  Note: 刷新状态栏
/*****************************************************************************/
LOCAL void UpdateStatusBar(BOOLEAN is_open_ps_mode)
{
    GUIWIN_STBDATA_ICON_T        icon_info   = {0};
    uint8  bat_level    = MMIAPIPHONE_GetBatCapacity();

    icon_info.is_display  = TRUE;
    icon_info.icon_num  = MMISTATUSBAR_ITEM_ICON_FRAME;
    if (MMIIDLE_GetIdleWinInfo()->is_charge)
    {
        icon_info.icon_type = GUIWIN_STBDATA_ICON_ANIM;
    }
    else
    {
        icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;
    }

    if(is_open_ps_mode)
    {
        icon_info.icon_array[0] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY1;
        icon_info.icon_array[1] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY2;
        icon_info.icon_array[2] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY3;
        icon_info.icon_array[3] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY4;
        icon_info.icon_array[4] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY5;
        icon_info.icon_array[5] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY6;
    }
    else
    {
#ifdef USE_IMAGE_OPEN
        icon_info.icon_array[0] = IMAGE_IDLE_TITLE_ICON_BATTERY1;
        icon_info.icon_array[1] = IMAGE_IDLE_TITLE_ICON_BATTERY2;
        icon_info.icon_array[2] = IMAGE_IDLE_TITLE_ICON_BATTERY3;
        icon_info.icon_array[3] = IMAGE_IDLE_TITLE_ICON_BATTERY4;
        icon_info.icon_array[4] = IMAGE_IDLE_TITLE_ICON_BATTERY5;
        icon_info.icon_array[5] = IMAGE_IDLE_TITLE_ICON_BATTERY6;
#endif
    }
    GUIWIN_SetStbItemIcon(MMI_WIN_ICON_BATTERY,&icon_info);
    MMIAPICOM_StatusAreaSetBatLevel(bat_level);
}

/*****************************************************************************/
//  Description :DealWithListCheckMsg
//  Global resource dependence :
//  Author:juan.wu
//  Note: 处理list 的check消息
/*****************************************************************************/
LOCAL void DealWithListCheckMsg(MMI_CTRL_ID_T ctrl_id)
{
    BOOLEAN is_switch = FALSE;
    MMISET_POWER_SETTING_ITEM_T power_sav_set = MMIAPISET_GetPowerSavingSetting();

    if(MMISET_POWER_SAVE_ONOFF_LIST_CTRL_ID == ctrl_id)
    {
        is_switch = MMIAPISET_GetPowerSavingSwitch();
        is_switch = !is_switch;

        MMIAPISET_SetPowerSavingSwitch(is_switch);
        if(is_switch)//开启省电模式
        {
            //MMIAPISET_SetPowerSavingSwitch(TRUE);
#ifdef MMI_VIDEOWALLPAPER_SUPPORT//NEWMS00162005
            {
                if(power_sav_set.is_fb_anim_wallpaper && power_sav_set.is_active)
                {
                    MMIAPISET_ResetDefaultWallpaper();
                }
            }
#endif
        }
        else//关闭省电模式
        {
            power_sav_set.is_active = FALSE;
            MMIAPISET_SetPowerSavingSetting(power_sav_set);
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
            MMIAPIENVSET_ResetActModeOtherRingSet();
        }
        AppendTwoTextCheckList(ctrl_id,TRUE);
#else
    }
#endif
}
#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
else if(MMISET_POWER_PERCENT_DIS_LIST_CTRL_ID == ctrl_id)
{
    GUIWIN_STBDATA_TEXT_T  text_info = {0};
    BOOLEAN is_percent_dis = MMIAPISET_GetBatteryPercentDisSwitch();

    is_percent_dis = !is_percent_dis;
    text_info = MMIAPIIDLE_GetBatteryPercentStbTxt();
    MMIAPISET_SetBatteryPercentDisSwitch(is_percent_dis);
    if(is_percent_dis)
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_BATTERY_PERCENT, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_BATTERY_PERCENT, FALSE);
    }
    GUIWIN_SetStbItemText(MMI_WIN_ICON_BATTERY_PERCENT,&text_info);
    AppendTwoTextCheckList(ctrl_id,TRUE);
    GUIWIN_UpdateStb();
}
#endif
}
#endif
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:juan.wu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISet_GetPreviewColorThemeIndex(void)
{
    return s_setting_cur_colortheme_index;
}
#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
/*****************************************************************************/
//  Description :MMIAPISET_SetBatteryDisSwitch
//  Global resource dependence :
//  Author:juan.wu
//  Note:设置电池电量在status bar上显示
/*****************************************************************************/
PUBLIC void MMIAPISET_SetBatteryPercentDisSwitch(BOOLEAN is_switch)
{
    MMINV_WRITE(MMINV_SET_BATTERY_PERCENT_DIS_SWITCH, &is_switch);
}
/*****************************************************************************/
//  Description :MMIAPISET_GetPowerSavingSwitch
//  Global resource dependence :
//  Author:juan.wu
//  Note: 获得是否要在status bar上显示电池电量百分比
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetBatteryPercentDisSwitch(void)
{
    MN_RETURN_RESULT_E          return_value    =   MN_RETURN_FAILURE;
    BOOLEAN                     is_switch          =   FALSE;

    MMINV_READ(MMINV_SET_BATTERY_PERCENT_DIS_SWITCH, &is_switch, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_switch= TRUE;
        MMINV_WRITE(MMINV_SET_BATTERY_PERCENT_DIS_SWITCH, &is_switch);
    }
    return is_switch;
}
#endif

/*****************************************************************************/
//  Discription: append one line text radio list item
//  Global resource dependence: none
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void MMISET_AppenTwoLineTextListItem(
        MMI_CTRL_ID_T    ctrl_id,
        MMI_TEXT_ID_T    text_id_1,
        MMI_TEXT_ID_T    text_id_2,
        MMI_STRING_T     *str_1,
        MMI_STRING_T     *str_2,
        uint16           pos
        )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
    GUI_COLOR_T color = MMI_WHITE_COLOR;

    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
    // 	item_t.item_state |= GUIITEM_STATE_CONTENT_CHECK;
    item_t.item_data_ptr = &item_data;
    item_data.softkey_id[0] = STXT_OPTION;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;

    if(0 != text_id_1)
    {
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = text_id_1;
    }
    else if (PNULL != str_1)
    {
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer.wstr_len = str_1->wstr_len;
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = str_1->wstr_ptr;
    }

    if(0 != text_id_2)
    {
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[1].item_data.text_id = text_id_2;
    }
    else if (PNULL != str_2)
    {
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = str_2->wstr_len;
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = str_2->wstr_ptr;
    }

    GUILIST_SetItemState(&item_t, GUIITEM_STATE_COSTMER_FONT_COLOR, TRUE);
    GUILIST_SetItemContentCustomFontColor(ctrl_id, 1, &color, 255*0.40, &color, 255*0.40);

    GUILIST_AppendItem( ctrl_id, &item_t );
}
/*****************************************************************************/
//  Discription: append one line text radio list item
//  Global resource dependence: none
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void MMISET_AppenOneLineTextListItem(
        MMI_CTRL_ID_T    ctrl_id,
        MMI_TEXT_ID_T    text_id,
        uint16           pos
        )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_t.item_data_ptr = &item_data;

    item_data.softkey_id[0] = STXT_SELECT;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;

    GUILIST_AppendItem( ctrl_id, &item_t );
}

/*****************************************************************************/
//  Discription: append one line text toggle list item
//  Global resource dependence: none
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void MMISET_AppenOneLineTextToggleListItem(
        MMI_CTRL_ID_T    ctrl_id,
        MMI_TEXT_ID_T    text_id,
        uint16           pos
        )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

    item_t.item_style = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
    // 	item_t.item_state |= GUIITEM_STATE_CONTENT_CHECK;
    item_t.item_data_ptr = &item_data;

    item_data.softkey_id[0] = STXT_SOFTKEY_CHANGE;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;

    GUILIST_AppendItem( ctrl_id, &item_t );
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL void MMISET_AppendSetMSAItem(void)
{
#if defined(CAMERAROLL_SUPPORT)
    MMIAPIMSA_USER_INFO_T * user_info =MMIAPIMSA_get_user_info();
    wchar user_email[MAX_MSA_USER_INFO_STR_SIZE+1] = {0};
#endif
    MMI_STRING_T user_info_str = {0};
    BOOLEAN b_has_network = TRUE;
    BOOLEAN b_has_user_info = FALSE;

    if(NO_DATA_SERVICE_TYPE == MMIAPIPHONE_GetDataServiceType(MMIAPISET_GetActiveSim())
#ifdef MMI_GPRS_SWITCH_SUPPORT
            ||(TRUE == MMIAPICONNECTION_isGPRSSwitchedOff())
#endif  
      )
    {
        b_has_network = FALSE;
    }

    GUILIST_SetMaxItem(MMISET_MSA_MENU_CTRL_ID, 3, FALSE);

#if defined(CAMERAROLL_SUPPORT)
    if(TRUE == MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL))
    {
        if(0 != strlen(user_info->email))
        {
            MMI_STRNTOWSTR(user_email , MAX_MSA_USER_INFO_STR_SIZE , user_info->email , MAX_MSA_USER_INFO_STR_SIZE , strlen(user_info->email));
            user_info_str.wstr_ptr = user_email;
            user_info_str.wstr_len = strlen(user_info->email) ; 
            b_has_user_info = TRUE;
        }

        if(b_has_network)
        {		
            if(TRUE == b_has_user_info)
                MMISET_AppenTwoLineTextListItem(MMISET_MSA_MENU_CTRL_ID, 0 , STR_MSA_CONNECTED_EXT01,&user_info_str,PNULL,ID_SET_MSA_INDEX_ACCOUNT);		
            else
                MMISET_AppenTwoLineTextListItem(MMISET_MSA_MENU_CTRL_ID, 0 , STR_MSA_CONNECTED_EXT01,PNULL,PNULL,ID_SET_MSA_INDEX_ACCOUNT);		
        }
        else
        {
            if(TRUE == b_has_user_info)
                MMISET_AppenTwoLineTextListItem(MMISET_MSA_MENU_CTRL_ID, 0 , STR_MSA_NOT_CONNECTED_EXT01,&user_info_str,PNULL,ID_SET_MSA_INDEX_ACCOUNT);		
            else
                MMISET_AppenTwoLineTextListItem(MMISET_MSA_MENU_CTRL_ID, 0 , STR_MSA_NOT_CONNECTED_EXT01,PNULL,PNULL,ID_SET_MSA_INDEX_ACCOUNT);		
        }

    }	
    else
#endif
    {
        MMISET_AppenTwoLineTextListItem(MMISET_MSA_MENU_CTRL_ID, TXT_COMMON_WWW_ACCOUNT , STR_MSA_VALIDATION_FAILED_EXT01,PNULL,PNULL,ID_SET_MSA_INDEX_ACCOUNT);		
    }

    MMISET_AppenOneLineTextListItem(MMISET_MSA_MENU_CTRL_ID, STR_ONEDRIVE_LIST_PHOTOS_EXT01, ID_SET_MSA_INDEX_ONEDRIVE_PHOTO);
#ifdef MMIPB_SYNC_WITH_O365 
    MMISET_AppenOneLineTextToggleListItem(MMISET_MSA_MENU_CTRL_ID, STR_MSA_ILIST_CONTA_SYNC_EXT01, ID_SET_MSA_INDEX_CONTACTS_SYNC);
#endif
    //check network status 
    if(!b_has_network)
    {
        SCI_TRACE_LOW("[MSA] MMISET_AppendSetMSAItem switch off the item");
        GUILIST_SetItemGray(MMISET_MSA_MENU_CTRL_ID, ID_SET_MSA_INDEX_ONEDRIVE_PHOTO, TRUE);
        GUILIST_SetItemInvalid(MMISET_MSA_MENU_CTRL_ID, ID_SET_MSA_INDEX_ONEDRIVE_PHOTO, TRUE);  
#ifdef MMIPB_SYNC_WITH_O365 		
        GUILIST_SetItemGray(MMISET_MSA_MENU_CTRL_ID, ID_SET_MSA_INDEX_CONTACTS_SYNC, TRUE);
        GUILIST_SetItemInvalid(MMISET_MSA_MENU_CTRL_ID, ID_SET_MSA_INDEX_CONTACTS_SYNC, TRUE); 
#endif
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
#ifdef MSA_SUPPORT 

LOCAL MMI_RESULT_E HandleSetMSAWinMsg(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_RESULT_E   result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_MSA_MENU_CTRL_ID;
    uint16   cur_index = 0;
#ifdef MMIPB_SYNC_WITH_O365 
    BOOLEAN     is_sync_on = FALSE;
    is_sync_on = MMIPB_SYNC_NV_Is_HeartBeat_On();
    GUILIST_SetToggleState(MMISET_MSA_MENU_CTRL_ID, ID_SET_MSA_INDEX_CONTACTS_SYNC, is_sync_on);

    SCI_TRACE_LOW("[MMIPBSYNC] HandleSetMSAWinMsg is_sync_on = %d", is_sync_on);

#endif       
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMISET_AppendSetMSAItem();
            MMK_SetAtvCtrl(win_id,ctrl_id);
            break;
//         case MSG_FULL_PAINT: 
//             {
//                 //reset status bar
//                 MMI_STRING_T    temp_str = {0};
//                 MMI_GetLabelTextByLang(STR_MSA_HDR_ACCOUNT, &temp_str);
//                 if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len)) {
//                     GUIWIN_UpdateStb();
//                 }
//             }
//             break;
        case MSG_CTL_MIDSK:
            cur_index =  GUILIST_GetCurItemIndex(MMISET_MSA_MENU_CTRL_ID);
            switch(cur_index)
            {
                case ID_SET_MSA_INDEX_ONEDRIVE_PHOTO:
                    MMIPIC_CreateMSAOneDrivePhotoWin();
                    break;
#ifdef MMIPB_SYNC_WITH_O365 
                case ID_SET_MSA_INDEX_CONTACTS_SYNC:
                    {
                        //                                  MMK_CreateWin((uint32*)MMIPICVIEW_CHECK_UPDATE_WIN_TAB, PNULL);
                        //            MMIPIC_CreateOneDriveConnectTextDialogWin();
                    }
                    break;
#endif
                default:
                    break;
            }

            break;
        case MSG_CTL_OK:
            cur_index =  GUILIST_GetCurItemIndex(MMISET_MSA_MENU_CTRL_ID);
            if(ID_SET_MSA_INDEX_ACCOUNT == cur_index)
                MMK_CreateWin((uint32 *)MMISET_MSA_LIST_OPT_WIN_TAB, PNULL);
            break;
        case MSG_CTRL_TOGGLE_CHANGED:
#if 0
            if (is_sync_on)
            {
                // Contacts FTU flow
            }
            else
            {
                // TO DO
            }
#endif
#ifdef MMIPB_SYNC_WITH_O365     
            SCI_TRACE_LOW("[MMIPBSYNC] HandleSetMSAWinMsg MMIPB_SYNC_NV_Set_HeartBeat %d",!is_sync_on);
            MMIPB_SYNC_NV_Set_HeartBeat(!is_sync_on);
            is_sync_on = MMIPB_SYNC_NV_Is_HeartBeat_On();
            if (TRUE == is_sync_on)
            {
                SCI_TRACE_LOW("[MMIPBSYNC] HandleSetMSAWinMsg -2- is_sync_on = %d", is_sync_on);
                MMIPB_SYNC_Check_And_Start_FirstSync();
            }
#endif
            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}


#if 0
LOCAL MMI_RESULT_E HandleSetMSAWinMsg(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_RESULT_E	result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	ctrl_id = MMISET_MSA_MENU_CTRL_ID;
    uint16	cur_index = 0;
    BOOLEAN	is_sync_on = FALSE;
    BOOLEAN	need_heavy_sync = FALSE;

#ifdef MMIPB_SYNC_WITH_O365     
    is_sync_on = MMIPB_SYNC_NV_Is_HeartBeat_On();
    GUILIST_SetToggleState(MMISET_MSA_MENU_CTRL_ID, ID_SET_MSA_INDEX_CONTACTS_SYNC, is_sync_on);
    if (is_sync_on == TRUE)
    {
        SCI_TRACE_LOW("[MMIPBSYNC] HandleSetMSAWinMsg is_sync_on = %d", is_sync_on);
        need_heavy_sync = MMIPB_SYNC_Check_And_Start_FirstSync();
    }
#endif


    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMISET_AppendSetMSAItem();
            MMK_SetAtvCtrl(win_id,ctrl_id);	
            break;
//         case MSG_FULL_PAINT: 
//             {
//                 //reset status bar
//                 MMI_STRING_T    temp_str = {0};
//                 MMI_GetLabelTextByLang(STR_MSA_HDR_ACCOUNT, &temp_str);
//                 if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len)) {
//                     GUIWIN_UpdateStb();
//                 }
//             }
//             break;
        case MSG_CTL_MIDSK:
            cur_index =  GUILIST_GetCurItemIndex(MMISET_MSA_MENU_CTRL_ID);
            switch(cur_index)
            {
                case ID_SET_MSA_INDEX_ONEDRIVE_PHOTO:
                    MMIPIC_CreateMSAOneDrivePhotoWin();
                    break;

                case ID_SET_MSA_INDEX_CONTACTS_SYNC:
                    {
                        // 				MMK_CreateWin((uint32*)MMIPICVIEW_CHECK_UPDATE_WIN_TAB, PNULL);
                        // 	      MMIPIC_CreateOneDriveConnectTextDialogWin();
                    }
                    break;

                default:
                    break;
            }

            break;
        case MSG_CTL_OK:
            cur_index =  GUILIST_GetCurItemIndex(MMISET_MSA_MENU_CTRL_ID);
            if(ID_SET_MSA_INDEX_ACCOUNT == cur_index)
                MMK_CreateWin((uint32 *)MMISET_MSA_LIST_OPT_WIN_TAB, PNULL);
            break;
        case MSG_CTRL_TOGGLE_CHANGED:

            if (is_sync_on)
            {
                // Contacts FTU flow
            }
            else
            {
                // TO DO
            }
#ifdef MMIPB_SYNC_WITH_O365     
            SCI_TRACE_LOW("[MMIPBSYNC] HandleSetMSAWinMsg GUILIST_SetToggleState is_sync_on = %d",!is_sync_on);
            MMIPB_SYNC_NV_Set_HeartBeat(!is_sync_on);
            if ((TRUE == need_heavy_sync)&&(TRUE == is_sync_on))
            {
                SCI_TRACE_LOW("[MMIPBSYNC] HandleSetMSAWinMsg Start Heavy Sync.",!is_sync_on);
                MMIPB_SYNC_Start_HeavySync(FALSE);
            }
#endif
            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}
#endif
/*****************************************************************************/
//  Description : Handle Pic One Drive Sync DIALOG window
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetRemoveMSAWinMsg(
        MMI_WIN_ID_T     win_id,
        MMI_MESSAGE_ID_E msg_id,
        DPARAM           param
        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:

            break;

        case MSG_APP_WEB:
            //MMIAPIMSA_sign_off(MSA_APP_TYPE_CAMERA_ROLL);
            MMIAPISET_OpenFactoryResetFromMSA();
            MMK_CloseWin(win_id);
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
        default:
            recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
            break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : Create one drive Auto upload text dialog window
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_CreateRemoveMSATextDialogWin(void)
{
    MMI_WIN_ID_T     win_id = MMISET_MSA_REMOVE_DIALOG_WIN_ID;
    GUI_LCD_DEV_INFO lcd_dev_info_ptr = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T       win_rect = {0};
    uint16           logic_width = 0;
    uint16           logic_height = 0;

    MMIPUB_OpenConfirmationDialogByTextId ( PNULL,
            STR_MSA_NOTE_REMOVE_ACCOUNT,
            TXT_NULL,
            PNULL,
            &win_id,
            PNULL,
            MMIPUB_SOFTKEY_OKCANCEL,
            HandleSetRemoveMSAWinMsg);
}

/*****************************************************************************/
//  Description : handle list preview option win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetMSAlistOptWinMsg(
        MMI_WIN_ID_T        win_id,
        MMI_MESSAGE_ID_E    msg_id,
        DPARAM              param
        )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMISET_MSA_LIST_OPT_MENU_CTRL_ID;
    MMI_MENU_ID_T       menu_id = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id,ctrl_id);
            break;

        case MSG_CTL_OK:
            break;

        case MSG_CTL_MIDSK:
#ifdef MSA_SUPPORT    	
            GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
            switch (menu_id)
            {
                case ID_SET_MSA_REMOVE_ACCOUNT:
                    MMISET_CreateRemoveMSATextDialogWin();
                    //remove account flow

                    break;

                case ID_SET_MSA_EDIT_ACCOUNT:
                    {
#ifdef BROWSER_SUPPORT
                        uint8 *url_ptr = PNULL;
                        uint16 str_len = (uint16)(MMIAPICOM_Wstrlen(MSA_EDIT_ACCOUNT_URL) * 3 + 1);
                        BOOLEAN bl_browser_type = 0;	  
                        MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/			 

                        url_ptr = SCI_ALLOCA(str_len * sizeof(uint8));

                        SCI_MEMSET(url_ptr, 0x00, (str_len * sizeof(uint8)));

                        GUI_WstrToUTF8(url_ptr, str_len, MSA_EDIT_ACCOUNT_URL, MMIAPICOM_Wstrlen(MSA_EDIT_ACCOUNT_URL));

                        EFS_NvitemRead(APP_NV_DEFAULT_BROWSER_APP, sizeof(BOOLEAN), &bl_browser_type);
                        if(bl_browser_type)
                        {
                            entry_param.browser_type = MMIBROWSER_TYPE_DORADO ; 
                        }
                        else
                        {
#ifdef OPERA_MINI_SUPPORT
                            entry_param.browser_type = MMIBROWSER_TYPE_OPERA_MINI ; 
#endif
                        }
                        entry_param.type = MMIBROWSER_ACCESS_URL;
                        entry_param.dual_sys = MN_DUAL_SYS_MAX;
                        entry_param.url_ptr = (char *)url_ptr;
                        entry_param.user_agent_ptr = PNULL;
                        entry_param.browser_type = 

                            MMIAPIBROWSER_Entry(&entry_param);

                        SCI_FREE(url_ptr);
                        url_ptr = PNULL;      
#endif
                    }
                    break;

                default:
                    break;
            }
#endif
            MMK_CloseWin(win_id);
            break;

        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            //GUI_INVALID_PARAM(param);
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}

/*****************************************************************************/

//  Description :
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetAddNewMSAWindow(
        MMI_WIN_ID_T     win_id,
        MMI_MESSAGE_ID_E msg_id,
        DPARAM           param
        )
{
    MMI_RESULT_E      recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO  lcd_dev_info_ptr = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T        win_rect = {0};
    uint16            logic_width = 0;
    uint16            logic_height = 0;
    GUI_RECT_T        button_rect = {0};
    GUI_RECT_T        lab_rect = {0};
    GUI_FONT_ALL_T  str_font          = {0};

    str_font.font             = MMI_DEFAULT_BIG_FONT;
    str_font.color            = MMI_DEFAULT_TEXT_COLOR;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:

            //Label button 
            CTRLBUTTON_SetLocation(MMISET_ADD_NEW_MSA_CTRL_ID, 0, 24);        
            CTRLBUTTON_SetIsRectButton(MMISET_ADD_NEW_MSA_CTRL_ID, TRUE);        
            CTRLBUTTON_SetTextId(MMISET_ADD_NEW_MSA_CTRL_ID, STR_MSA_ILIST_ADD_ACCOUNT_EXT01);
            CTRLBUTTON_SetButtonIcon(MMISET_ADD_NEW_MSA_CTRL_ID, IMAGE_COMMON_PLUS_SMALL);
            GUIBUTTON_SetFont(MMISET_ADD_NEW_MSA_CTRL_ID,&str_font);

            //Set initially focused
            CTRLBASE_SetCanActive(MMISET_ADD_NEW_MSA_CTRL_ID, TRUE);   //--> base is null
            MMK_SetActiveCtrl(MMISET_ADD_NEW_MSA_CTRL_ID, TRUE);
            MMK_SetAtvCtrl(win_id,MMISET_ADD_NEW_MSA_CTRL_ID);

            //         CountActiveButtons();
            break;

        case MSG_GET_FOCUS:
            break;

        case MSG_LOSE_FOCUS:
            break;

        case MSG_FULL_PAINT:        
            {
                //reset status bar
                MMI_STRING_T    temp_str = {0};
                //Draw background 
                GUILCD_GetLogicWidthHeight(lcd_dev_info_ptr.lcd_id, &logic_width, &logic_height);        
                win_rect.bottom = win_rect.top + logic_height;
                win_rect.right = win_rect.left + logic_width;
                GUI_FillRect((const GUI_LCD_DEV_INFO*) &lcd_dev_info_ptr,win_rect, MMI_BLACK_COLOR);

//                 MMI_GetLabelTextByLang(STR_MSA_HDR_ACCOUNT, &temp_str);
//                 if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len))
//                     GUIWIN_UpdateStb();
            }	
            break;            
        case MSG_CTL_MIDSK:
#if defined(CAMERAROLL_SUPPORT)	
            MMIAPIMSA_get_token(TRUE ,MSA_APP_TYPE_CAMERA_ROLL ,  MMISET_MSA_Get_Token_Func);
#endif
            break;

        case MSG_KEYDOWN_UP:
            break;

        case MSG_KEYDOWN_DOWN:
            break;

        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : get token cb func
//  Global resource dependence :
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void MMISET_MSA_Get_Token_Func( int result, char*  buf, uint16 buflen)
{
    MMIAPIMSA_Exit();	

    if(MSA_OK == result)
    {
        if(MMK_IsOpenWin(MMISET_ADD_NEW_MSA_WIN_ID))
        {
            MMK_CloseWin(MMISET_ADD_NEW_MSA_WIN_ID);
        }
        MMK_CreateWin((uint32*)MMISET_SET_MSA_WIN_TAB,PNULL);
    }
    else if(MSA_FAILED == result)
        MMIPUB_OpenAlertFailWin(STR_MSA_VALIDATION_FAILED_EXT01);	
}

/*****************************************************************************/
// 	Description : set font size for UI DIsplay
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenFactoryResetFromMSA(void)
{
    MMI_WIN_ID_T  wait_win_id = MMISET_RESET_FACTORY_WAITING_WIN_ID;
    uint32 time_out = 0;


    if (MMIAPISET_GetFlyMode())
    {
        MMIAPISET_AlertFlyMode();
        return;
    }
#if defined(CAMERAROLL_SUPPORT)
    MMIAPIMSA_sign_off(MSA_APP_TYPE_CAMERA_ROLL);
#endif
    MMIPUB_OpenAlertWinByTextId(&time_out,STR_NOTE_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,
            &wait_win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandleResetOrCleanDataWaitWin);	
}
#endif
/*****************************************************************************/
//  Description :MMISet_GetPreferDisk
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMISet_GetPreferDisk(void)
{
    MMIFILE_DEVICE_E nv_setting = MMI_DEVICE_SDCARD;
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SETTING_PREFER_DISK, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        //nv_setting = MMI_DEVICE_SYSTEM;
        nv_setting = MMI_DEVICE_SDCARD;
        MMINV_WRITE(MMINV_SETTING_PREFER_DISK, &nv_setting);
    }

    return nv_setting;
}

/*****************************************************************************/
//  Description :MMISet_GetBackgroundWin
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMISet_GetBackgroundWin(void)
{
    return MMISET_SET_BACKGROUND_WIN_ID;
}

#ifndef MMI_GUI_STYLE_MINISCREEN
/*****************************************************************************/

//  Description :
//  Global resource dependence :
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetPhoneSettingWindow(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
#if defined (MOTION_SENSOR_TYPE)&& !defined(FLASH_SUPPORT)
    BOOLEAN                   is_sensor_mute    =  FALSE;
    BOOLEAN          is_sensor_change_wallpaper =  FALSE;
    uint32                   sensor_data        =   0;
#endif

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIMENU_GetId(MMISET_PHONE_SETTING_MENU_CTRL_ID,&group_id,&menu_id);
#if defined (MOTION_SENSOR_TYPE)&& !defined(FLASH_SUPPORT)
#ifndef WIN32
            MSensor_ioctl(MSENSOR_TYPE_ACCELEROMETER, MSENSOR_IOCTL_GET_CAP, &sensor_data);
            is_sensor_mute = sensor_data & MSENSOR_MOTION_CAP_Z_AXIS;
            is_sensor_change_wallpaper = sensor_data & MSENSOR_MOTION_CAP_SHAKE;
            SCI_TRACE_LOW("HandleSetPhoneSettingWindow sensor_data = %d,is_sensor_mute = %d,is_sensor_change_wallpaper = %d");

            //不支持来电静音，切换墙纸，切换到下一首,隐藏方向感应菜单，否则，隐藏横竖屏切换菜单
            if(!is_sensor_mute && !is_sensor_change_wallpaper)
            {
                GUIMENU_SetItemVisible(MMISET_PHONE_SETTING_MENU_CTRL_ID, group_id, ID_SET_SENSOR, FALSE);
            }
            else
            {
                GUIMENU_SetItemVisible(MMISET_PHONE_SETTING_MENU_CTRL_ID, group_id, ID_SET_SENSOR_AUTO_ROTATE, FALSE);
            }
#endif
#endif
#ifdef BROWSER_SUPPORT
            if (MMIBROWSER_TYPE_MAX <= 1)/*lint !e506 !e774*/
            {
                GUIMENU_SetItemVisible(MMISET_PHONE_SETTING_MENU_CTRL_ID, group_id, ID_SET_SELECT_BROWSER_TYPE, FALSE);
            }
#endif
            if (MMISET_IDLE_STYLE_SMART == MMIAPISET_GetIdleStyle())
            {
                GUIMENU_SetItemVisible(MMISET_PHONE_SETTING_MENU_CTRL_ID, group_id, ID_SET_SHORTCUT, FALSE);
            }
            MMK_SetAtvCtrl(win_id, MMISET_PHONE_SETTING_MENU_CTRL_ID);
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            GUIMENU_GetId(MMISET_PHONE_SETTING_MENU_CTRL_ID,&group_id,&menu_id);
            switch (menu_id)
            {
            #ifdef PHONE_NUMBER_VOICE
				case ID_ZDT_TTS:
					 MMIAPIST_OpenTTSWin();
				  break;
				///case ID_ZDT_POINTTIME:
		
					// MMIAPIST_PontTimeWin();
					//break;
#endif
#ifdef  SOS_KEYLONGWEB	
		case ID_ZDT_SOS:
	
			MMIAPIST_OpenSOSWin();
			break;
#endif

                case ID_SET_TIME_DATE:
                    MMISET_CreateDateTimeWin();
                    break;

                case ID_SET_LANGUAGE://语言设置
                    MMK_CreateWin((uint32*)MMISET_SET_LANGUAGE_WIN_TAB,PNULL);
                    break;

                    // [MS]
#ifdef MSA_SUPPORT
                case ID_SET_MSA:
                    if(MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL))
                        MMK_CreateWin((uint32*)MMISET_SET_MSA_WIN_TAB,PNULL);
                    else
                    {
                        MMIAPIMSA_Clean_Invalid_Account();
                        MMK_CreateWin((uint32*)MMISET_SET_ADD_NEW_MSA_WIN_TAB,PNULL);
                    }

                    break;
#endif
                    //sogou不支持模糊音，非触摸屏不支持手写，如果两者同时成立时，则导致没有输入法设置项
                    //#if (defined TOUCH_PANEL_SUPPORT || ((!defined IM_ENGINE_SOGOU) && ((defined IM_SIMP_CHINESE_SUPPORT) || (defined IM_TRAD_CHINESE_SUPPORT))))
                case ID_SET_INPUT_MODE://输入法设置
                    MMIAPIIM_OpenSetWin();
                    break;          
                    //#endif


                case ID_SET_SHORTCUT:    //快捷键设置
                    MMISET_EnterSettingShortcutWin();
                    break;

                    //        case ID_SET_TXT_CODE_TYPE:
                    //            MMK_CreateWin((uint32*)MMISET_SET_TXT_CODE_TYPE_WIN_TAB,PNULL);
                    //            break;


                case ID_SET_COORDINATE://校正触摸屏
                    if(!POWER_IsAutoTestMode())
                        MMIAPITP_COORDINATE_OpenWin(FALSE);
                    break;

                case ID_SET_AUTO_POWERONOFF://自动开关机
                    MMIAPIAUTOPOWER_OpenMainWin();
                    break;

#ifdef LCD_SLIDE_SUPPORT
                case ID_SET_SLIDE_OPERATION://滑盖操作
                    MMK_CreateWin((uint32*)MMISET_SLIDE_OPERATION_WIN_TAB, PNULL);
                    break;
#endif

#ifdef NPS_SUPPORT
                case ID_SET_POWER_MANAGEMENT:
                    //           MMIAPIPHONE_OpenBtryCapWin();
                    MMIAPINPS_OpenMainMenuWindow(FALSE);
                    break;
#else
                case ID_SET_POWER_MANAGEMENT:
                    MMIAPIPHONE_OpenBtryCapWin();
                    break;
#endif

#ifdef MMI_POWER_SAVING_MODE_SUPPORT
                case ID_SET_SAVE_ELECTRICITY:
                    MMK_CreateWin((uint32 *) MMISET_POWER_SAVE_WIN_TAB, PNULL);
                    break;
#endif

#ifdef MOTION_SENSOR_TYPE
                case ID_SET_SENSOR:
                    OpenSetSensorWin();
                    break;
#endif
                case ID_SET_RESET_FACTORY://恢复出厂设置
                    MMIAPISET_OpenInputResetPwdWin(TRUE);
                    break;

#ifdef BROWSER_SUPPORT
                case ID_SET_SELECT_BROWSER_TYPE:
                    MMIAPIBROWSER_OpenSetBrowserWin();
                    break;
#endif

#ifdef MOTION_SENSOR_TYPE
                case ID_SET_SENSOR_AUTO_ROTATE:
                    MMK_CreatePubListWin((uint32*)MMISET_SET_SENSOR_AUTOROTATION_WIN_TAB,PNULL);
                    break;
#endif

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
#else
/*****************************************************************************/

//  Description :
//  Global resource dependence :
//  Author:yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetPhoneSettingWindow(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;

    uint16 index = 0;
    MMI_CTRL_ID_T ctrl_id = MMISET_PHONE_SETTING_MENU_CTRL_ID;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            InitPdaPhoneSettingsCtrl();
            MMK_SetAtvCtrl(win_id, ctrl_id);

            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, index, &menu_id);
            switch (menu_id)
            {
                case ID_SET_TIME_DATE:
                    MMISET_CreateDateTimeWin();
                    break;

                case ID_SET_LANGUAGE://语言设置
                    MMK_CreateWin((uint32*)MMISET_SET_LANGUAGE_WIN_TAB,PNULL);
                    break;

                    //sogou不支持模糊音，非触摸屏不支持手写，如果两者同时成立时，则导致没有输入法设置项
                    //#if (defined TOUCH_PANEL_SUPPORT || ((!defined IM_ENGINE_SOGOU) && ((defined IM_SIMP_CHINESE_SUPPORT) || (defined IM_TRAD_CHINESE_SUPPORT))))
                case ID_SET_INPUT_MODE://输入法设置
                    MMIAPIIM_OpenSetWin();

                    break;          
                    //#endif


                case ID_SET_SHORTCUT:    //快捷键设置
                    MMISET_EnterSettingShortcutWin();
                    break;

                    //        case ID_SET_TXT_CODE_TYPE:
                    //            MMK_CreateWin((uint32*)MMISET_SET_TXT_CODE_TYPE_WIN_TAB,PNULL);
                    //            break;


                case ID_SET_COORDINATE://校正触摸屏
                    if(!POWER_IsAutoTestMode())
                        MMIAPITP_COORDINATE_OpenWin(FALSE);
                    break;

                case ID_SET_AUTO_POWERONOFF://自动开关机
                    MMIAPIAUTOPOWER_OpenMainWin();
                    break;

#ifdef LCD_SLIDE_SUPPORT
                case ID_SET_SLIDE_OPERATION://滑盖操作
                    MMK_CreateWin((uint32*)MMISET_SLIDE_OPERATION_WIN_TAB, PNULL);
                    break;
#endif

                case ID_SET_POWER_MANAGEMENT:
                    MMIAPIPHONE_OpenBtryCapWin();
                    break;

#ifdef MMI_POWER_SAVING_MODE_SUPPORT
                case ID_SET_SAVE_ELECTRICITY:
                    MMK_CreateWin((uint32 *) MMISET_POWER_SAVE_WIN_TAB, PNULL);
                    break;
#endif

#ifdef MOTION_SENSOR_TYPE
                case ID_SET_SENSOR:
                    OpenSetSensorWin();
                    break;
#endif
                case ID_SET_RESET_FACTORY://恢复出厂设置
#ifdef BLUETOOTH_SUPPORT
                    if(MMIAPIBT_IsOpenFileWin())
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_BT_CLOSE_WARNING);
                    }
                    else
#endif
                    {
                        MMIAPISET_OpenInputResetPwdWin(TRUE);
                    }
                    break;

#ifdef BROWSER_SUPPORT
                case ID_SET_SELECT_BROWSER_TYPE:
                    MMIAPIBROWSER_OpenSetBrowserWin();
                    break;
#endif

#ifdef MOTION_SENSOR_TYPE
                case ID_SET_SENSOR_AUTO_ROTATE:
                    MMK_CreatePubListWin((uint32*)MMISET_SET_SENSOR_AUTOROTATION_WIN_TAB,PNULL);
                    break;
#endif

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
//  Description : append settings item
//  Global resource dependence :
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void AppendPdaPhoneSettingsItem1Line(
        MMI_CTRL_ID_T  ctrl_id,    //控件ID
        MMI_TEXT_ID_T  text_1,     //主文本
        MMI_IMAGE_ID_T image_id,   //icon
        uint32         menu_id   //用户数据
        )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    //icon
    // item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;//CR191890
    // item_data.item_content[1].item_data.text_id = image_id;

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = menu_id;

    GUILIST_AppendItem(ctrl_id, &item_t);
}
/*****************************************************************************/
//  Description : InitPdaCallSettingsCtrl
//  Global resource dependence :
//  Author:yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void  InitPdaPhoneSettingsCtrl(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMISET_PHONE_SETTING_MENU_CTRL_ID;
    uint16 list_max = 4;
#if defined (MOTION_SENSOR_TYPE)&& !defined(FLASH_SUPPORT)
    BOOLEAN                   is_sensor_mute    =  FALSE;
    BOOLEAN          is_sensor_change_wallpaper =  FALSE;
    uint32                   sensor_data        =   0;
#ifndef WIN32
    MSensor_ioctl(MSENSOR_TYPE_ACCELEROMETER, MSENSOR_IOCTL_GET_CAP, &sensor_data);
    is_sensor_mute = sensor_data & MSENSOR_MOTION_CAP_Z_AXIS;
    is_sensor_change_wallpaper = sensor_data & MSENSOR_MOTION_CAP_SHAKE;
#endif
#endif

    //#if (defined TOUCH_PANEL_SUPPORT || ((!defined IM_ENGINE_SOGOU) && ((defined IM_SIMP_CHINESE_SUPPORT) || (defined IM_TRAD_CHINESE_SUPPORT))))
    list_max++;
    //#endif
#if defined TOUCH_PANEL_SUPPORT && !defined TOUCHPANEL_TYPE_MULTITP
    list_max++;
#endif
#ifdef LCD_SLIDE_SUPPORT
    list_max++;
#endif
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
    list_max++;
#endif
#ifndef MMI_PHONESET_MINI_SUPPORT
    list_max++;
#endif
#ifdef DM_SUPPORT
    list_max++;
#endif
#if defined (MOTION_SENSOR_TYPE)&& !defined(FLASH_SUPPORT)
    list_max++;
#endif
#ifdef BROWSER_SUPPORT
    if (MMIBROWSER_TYPE_MAX > 1 )
    {
        list_max++;
    }
#endif
#if !defined CMCC_UI_STYLE && !defined FLASH_SUPPORT
#ifndef MMI_DISABLE_SHORTCUT_SET_SUPPORT
    if (MMISET_IDLE_STYLE_SMART != MMIAPISET_GetIdleStyle())
    {
        list_max++;
    }
#endif
#endif
    GUILIST_SetMaxItem(ctrl_id, list_max, FALSE );

    AppendPdaPhoneSettingsItem1Line(ctrl_id, TXT_SET_TIME_DATE, IMAGE_NULL, ID_SET_TIME_DATE);
    AppendPdaPhoneSettingsItem1Line(ctrl_id, TXT_SET_LANGUAGE_SETTINGS, IMAGE_NULL, ID_SET_LANGUAGE);
#if (defined TOUCH_PANEL_SUPPORT || ((!defined IM_ENGINE_SOGOU) ))
#if !((defined MAINLCD_DEV_SIZE_320X240) && (defined KEYPAD_TYPE_QWERTY_KEYPAD))
    AppendPdaPhoneSettingsItem1Line(ctrl_id, TXT_SET_INPUT_MODE, IMAGE_NULL, ID_SET_INPUT_MODE);
#endif
#endif

#if !defined CMCC_UI_STYLE && !defined FLASH_SUPPORT
#ifndef MMI_DISABLE_SHORTCUT_SET_SUPPORT
    if (MMISET_IDLE_STYLE_SMART != MMIAPISET_GetIdleStyle())
    {
        AppendPdaPhoneSettingsItem1Line(ctrl_id, TXT_COMMON_SPEEDKEY_SET, IMAGE_NULL, ID_SET_SHORTCUT);
    }
#endif
#endif
#if defined TOUCH_PANEL_SUPPORT && !defined TOUCHPANEL_TYPE_MULTITP
    AppendPdaPhoneSettingsItem1Line(ctrl_id, TXT_SET_COORDINATE, IMAGE_NULL, ID_SET_COORDINATE);
#endif
    AppendPdaPhoneSettingsItem1Line(ctrl_id, TXT_AUTO_POWERONOFF, IMAGE_NULL, ID_SET_AUTO_POWERONOFF);
#ifdef LCD_SLIDE_SUPPORT
    AppendPdaPhoneSettingsItem1Line(ctrl_id, TXT_SET_SLIDE_OPERATION, IMAGE_NULL, ID_SET_SLIDE_OPERATION);
#endif
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
    AppendPdaPhoneSettingsItem1Line(ctrl_id, TXT_ENERGY_SAVING, IMAGE_NULL, ID_SET_SAVE_ELECTRICITY);
#endif
#ifndef MMI_PHONESET_MINI_SUPPORT
    AppendPdaPhoneSettingsItem1Line(ctrl_id, TXT_SET_POWER_MANAGEMENT, IMAGE_NULL, ID_SET_POWER_MANAGEMENT);
#endif
#ifdef DM_SUPPORT
    AppendPdaPhoneSettingsItem1Line(ctrl_id, TXT_DM_SOFTWARE_UPDATE_SERVICE, IMAGE_NULL, ID_SET_DM_SERVICE);
#endif
#if defined (MOTION_SENSOR_TYPE)&& !defined(FLASH_SUPPORT)
    //不支持来电静音，切换墙纸，切换到下一首,隐藏方向感应菜单，否则，隐藏横竖屏切换菜单
    if(!is_sensor_mute && !is_sensor_change_wallpaper)
    {
        AppendPdaPhoneSettingsItem1Line(ctrl_id, TXT_SET_SENSOR_LANDSCAPE, IMAGE_NULL, ID_SET_SENSOR_AUTO_ROTATE);
    }
    else
    {
        AppendPdaPhoneSettingsItem1Line(ctrl_id, TXT_SET_SENSOR_RESPONDENT, IMAGE_NULL, ID_SET_SENSOR);
    }
#endif
#ifdef BROWSER_SUPPORT
    if (MMIBROWSER_TYPE_MAX > 1 )
    {
        AppendPdaPhoneSettingsItem1Line(ctrl_id, TXT_BM_SET_SELECT_BROWSER, IMAGE_NULL, ID_SET_SELECT_BROWSER_TYPE);
    }
#endif
    AppendPdaPhoneSettingsItem1Line(ctrl_id, TXT_COMMON_RESTORE_FACTORY_SETTINGS, IMAGE_NULL, ID_SET_RESET_FACTORY);
}
#endif

/*****************************************************************************/
//  Description : to handle display color theme setting window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
#ifdef MMI_COMPILE_E
LOCAL MMI_RESULT_E  HandleSetDisplayColorThemeWindow(
        MMI_WIN_ID_T    win_id, 
        MMI_MESSAGE_ID_E   msg_id, 
        DPARAM             param
        )
{
    MMI_RESULT_E            recode      =   MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO     lcd_dev_info    = {0};
    MMICOM_BANNER_INFO_T banner_info = {0};
    GUI_POINT_T start_point = {0, 0};
    GUI_RECT_T          title_rect = {0};
    GUI_RECT_T          color_rect = {0};
    GUI_RECT_T          disp_img_rect = {0};
    GUI_RECT_T          img_rect = {0};
    int                index = 0;
    uint16             width = 0;
    uint16             height = 0;
    GUI_COLOR_T                     color_theme = 0;
    GUI_COLOR_T     font_color = 0;
    GUISTR_STYLE_T  str_style = {0};
    MMI_STRING_T    text = {0};
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ELLIPSIS;
    GUI_RECT_T win_rect = MMITHEME_GetFullScreenRect();
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            //初始化color rect
            {
                s_setting_cur_colortheme_index = MMITHEME_GetAccentId();
                for(index = 0;index < MMITHEME_ACCENT_COLOR_MAX;index++)
                {
                    color_theme_rect[index].left = 41* index;
                    color_theme_rect[index].right = color_theme_rect[index].left + 35;
                    color_theme_rect[index].top = MMI_STATUSBAR_HEIGHT;
                    color_theme_rect[index].bottom = color_theme_rect[index].top + 35;

                }
                title_rect.left = 6;
                title_rect.right = 236;
                title_rect.top = MMI_STATUSBAR_HEIGHT + 35+ 6;
                title_rect.bottom = title_rect.top + 24;
            }
            break;
        case MSG_FULL_PAINT:
//             {//add for reset status bar
//                 MMI_STRING_T    temp_str = {0};
//                 MMI_GetLabelTextByLang(STR_SETT_LIST_THEME_EXT01, &temp_str);
//                 if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len))
//                 {
//                     GUIWIN_UpdateStb();
//                 }
//             }
            //end
            //画rect/title
            GUI_FillRect(&lcd_dev_info,win_rect,MMI_BLACK_COLOR);
            //依次画每个color
            for(index = 0;index < MMITHEME_ACCENT_COLOR_MAX; index++)
            {
                color_rect = color_theme_rect[index];
                color_theme = MMITHEME_GetAccentColor(index,MMITHEME_OFFSET_COLOR_DEFAULT);
                //LCD_FillRect(&lcd_dev_info, color_rect, color_theme); 
                LCD_FillArgbRect(&lcd_dev_info,color_rect,color_theme,255);
            }
            disp_img_rect = color_theme_rect[s_setting_cur_colortheme_index];
            GUIRES_GetImgWidthHeight(&width, &height, IMAGE_COLORTHEME_FOCUSFRAME, win_id);
            img_rect.right = width - 1;
            img_rect.bottom = height - 1;
            GUIRES_DisplayImg(PNULL,
                    &disp_img_rect,
                    &img_rect,
                    win_id,
                    IMAGE_COLORTHEME_FOCUSFRAME,
                    &lcd_dev_info);
            font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT);
            color_theme = MMITHEME_GetAccentColor(s_setting_cur_colortheme_index,MMITHEME_OFFSET_COLOR_DEFAULT);
            //画preview
            str_style.align = ALIGN_LVMIDDLE;
            str_style.font = MMI_DEFAULT_SMALL_FONT;
            str_style.font_color = MMI_WHITE_COLOR;


            title_rect.left = 6;
            title_rect.right = 236;
            title_rect.top = MMI_STATUSBAR_HEIGHT + 35+ 6;
            title_rect.bottom = title_rect.top + 24;

            MMI_GetLabelTextByLang(STR_MAIN_PREVIEW_EXT01, &text);

            GUISTR_DrawTextToLCDInRect(&lcd_dev_info,
                    (const GUI_RECT_T*)&title_rect, 
                    (const GUI_RECT_T*)&title_rect, 
                    (const MMI_STRING_T*)&text, 
                    &str_style, 
                    str_state, 
                    GUISTR_TEXT_DIR_AUTO);
            //画图片
            GUIRES_GetImgWidthHeight(&width, &height, IMAGE_COLORTHEME_PREVIEW_BLUE, win_id);
            disp_img_rect = win_rect;
            disp_img_rect.top = title_rect.bottom;
            img_rect.right = width - 1;
            img_rect.bottom = height - 1;

            GUIRES_DisplayImg(PNULL,
                    &disp_img_rect,
                    &img_rect,
                    win_id,
                    IMAGE_COLORTHEME_PREVIEW_VIOLET+s_setting_cur_colortheme_index,
                    &lcd_dev_info);
#ifdef MMI_ORIG_UNISOC_SUPPORT
            GUIWIN_SetSoftkeyStyle(win_id,
                    font_color,
                    GUI_SOFTKEY_BG_COLOR_ONLY,
                    color_theme,
                    IMAGE_NULL);
#endif
            GUIWIN_SetSoftkeyBgAlpha(win_id,255*0.65);
            break;
        case MSG_APP_LEFT:
        case MSG_KEYREPEAT_LEFT:
            {
                if(s_setting_cur_colortheme_index > 0)
                {
                    s_setting_cur_colortheme_index--;
                }
                else
                {
                    s_setting_cur_colortheme_index = 0;
                }
                MMIAPICOM_StatusAreaUpdateSignalState();
                MMK_SendMsg(win_id, MSG_FULL_PAINT,PNULL);
            }
            break;
        case MSG_APP_RIGHT:
        case MSG_KEYREPEAT_RIGHT:
            {
                if(s_setting_cur_colortheme_index < (MMITHEME_ACCENT_COLOR_MAX-1))
                {
                    s_setting_cur_colortheme_index++;
                }
                else
                {
                    s_setting_cur_colortheme_index = MMITHEME_ACCENT_COLOR_MAX - 1;
                }
                MMIAPICOM_StatusAreaUpdateSignalState();
                MMK_SendMsg(win_id, MSG_FULL_PAINT,PNULL);
            }
            break;
        case MSG_APP_WEB:
            MMITHEME_SetAccentId(s_setting_cur_colortheme_index);
            SetCurColorIndex(s_setting_cur_colortheme_index);
            banner_info.banner_type = MMI_BANNER_TEXT;
            banner_info.text1_id = STR_SETT_THEME_UPDATED_EXT01;
            banner_info.start_point = start_point;
            MMIAPICOM_OpenBannerWin(win_id, banner_info);
            MMK_CloseWin(win_id);
            MMK_UpdateColorTheme();
            MMIAPICOM_StatusAreaUpdateSignalState();
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            MMIAPICOM_StatusAreaUpdateSignalState();
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}
#endif
/*****************************************************************************/
//  Description :HandleSetMainWindow
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetMainWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMI_RESULT_E    			result 			=   MMI_RESULT_TRUE;
#ifdef TEST_MAIN_MENU
    MMI_TEXT_ID_T       str_id[] = {
        STR_SETT_LIST_SYSTEM_EXT01,
        TXT_SET_PHONE,
        STR_SETT_LIST_PERSONALISATION_EXT01,
        TXT_SETTINGS_DISPLAY,
        STR_SETT_LIST_TIME_LANGUAGE_EXT01,
        TXT_COMM_SECURITY_SETTING,
        TXT_SETTINGS_CALL,
        TXT_SETTINGS_LINK,
        STR_SETT_LIST_STORAGE_EXT01
    };
    MMI_TEXT_ID_T       img_id[] = {
        IMAGE_SETTING_SYSTEM_ICON,
        IMAGE_SECMENU_ICON_SET_PHONE,
        IMAGE_SETTING_PERSONALISATION_ICON,
        IMAGE_SECMENU_ICON_SET_DISPLAY,
        IMAGE_SETTING_TIME_LANGUAGE_ICON,
        IMAGE_SETTING_SECURITY_ICON,
        IMAGE_SECMENU_ICON_SET_CALL,
        IMAGE_SETTING_CONNECTIVITY_ICON,
        IMAGE_SETTING_STORAGE_ICON
    };
    uint16  			max_items = 9;
#else
#ifdef MMI_SHOW_MSA
    MMI_TEXT_ID_T       str_id[] = {
        STR_SETT_LIST_SYSTEM_EXT01,
        STR_ILIST_CONNECTIVITY_MENU,
        STR_SETT_LIST_PERSONALISATION_EXT01,
        STR_SETT_ILIST_MSA_ACCOUNT,
        STR_SETT_LIST_TIME_LANGUAGE_EXT01,
        STR_SETT_LIST_SECURITY_EXT01,
        TXT_COMMON_MAIN_STK,
        STR_SETT_LIST_STORAGE_EXT01,
        STR_INSTALLED_APPS_EXT01
    };
    MMI_TEXT_ID_T       img_id[] = {
        IMAGE_SETTING_SYSTEM_ICON,
        IMAGE_SETTING_CONNECTIVITY_ICON,
        IMAGE_SETTING_PERSONALISATION_ICON,
        IMAGE_SETTING_MS_ACCOUNT_ICON,
        IMAGE_SETTING_TIME_LANGUAGE_ICON,
        IMAGE_SETTING_SECURITY_ICON,
        IMAGE_SETTING_OPERATOR_ICON,
        IMAGE_SETTING_STORAGE_ICON,
        IMAGE_MY_APPLICATIONS_MYAPP
    };
    uint16  			max_items = 9;
#else
MMI_TEXT_ID_T       str_id[] = {
        STR_SETT_LIST_SYSTEM_EXT01,
        STR_ILIST_CONNECTIVITY_MENU,
        STR_SETT_LIST_PERSONALISATION_EXT01,
        STR_SETT_LIST_TIME_LANGUAGE_EXT01,
        STR_SETT_LIST_SECURITY_EXT01,
        TXT_COMMON_MAIN_STK,
        STR_SETT_LIST_STORAGE_EXT01
#ifdef JAVA_SUPPORT 
        ,STR_INSTALLED_APPS_EXT01
#endif
#ifdef MMI_VOLTE_SUPPORT
        ,STR_LIST_SETT_VOLTE
#endif
        // temporarily hide sos setting menu, need to confirm the request
//#ifdef MMI_INDIAN_SOS_SUPPORT
//     ,TXT_SETTINGS_SOS
//#endif
    };

    MMI_TEXT_ID_T       img_id[] = {
        IMAGE_SETTING_SYSTEM_ICON,
        IMAGE_SETTING_CONNECTIVITY_ICON,
        IMAGE_SETTING_PERSONALISATION_ICON,
        IMAGE_SETTING_TIME_LANGUAGE_ICON,
        IMAGE_SETTING_SECURITY_ICON,
        IMAGE_SETTING_OPERATOR_ICON,
        IMAGE_SETTING_STORAGE_ICON
#ifdef JAVA_SUPPORT                                      
        ,IMAGE_MY_APPLICATIONS_MYAPP
#endif                                        
#ifdef MMI_VOLTE_SUPPORT
        ,IMAGE_SETTING_SYSTEM_ICON
#endif
    };
    // temporarily hide sos setting menu, need to confirm the request
    //#ifdef MMI_INDIAN_SOS_SUPPORT
    //    uint16  			max_items = 9;
    //#else
    uint16  			max_items = 7
#ifdef JAVA_SUPPORT
        +1
#endif
#ifdef MMI_VOLTE_SUPPORT
        +1
#endif
        ;
    //#endif
#endif
#endif
    int16               i = 0;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16   cur_item = 0;
    MMI_CTRL_ID_T               ctrl_id = MMISET_SET_MAIN_LIST_CTRL_ID;
    uint16 item_index = 0;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id,ctrl_id);
            cur_item  = 0;
#ifdef MMI_VOLTE_SUPPORT
            if(MMIAPIPHONE_volte_menu_display_set(MN_DUAL_SYS_1) == 0
		#ifdef MMI_MULTI_SIM_SYS_DUAL
		&& (MMIAPIPHONE_volte_menu_display_set(MN_DUAL_SYS_2) == 0)
		#endif
            )
                max_items = max_items -1;
#endif
            //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d"
            GUILIST_SetMaxItem(ctrl_id,max_items, FALSE );
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
            item_t.item_data_ptr = &item_data;

            for(i = 0; i < max_items; i++)
            {
                //item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
                //item_data.item_content[0].item_data.text_id = str_id[i];
                //item_data.item_content[0].icon_id = img_id[i];
                item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[0].item_data.image_id = img_id[i];

                item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[1].item_data.text_id = str_id[i];

                GUILIST_AppendItem(ctrl_id, &item_t);
            }
            GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, cur_item);
            break;
        case MSG_FULL_PAINT:
            {//reset status bar
//                 MMI_STRING_T    temp_str = {0};
//                 MMI_GetLabelTextByLang(STXT_HEADER_SETTINGS, &temp_str);
//                 if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len))
//                 {
//                     GUIWIN_UpdateStb();
//                 }
                SCI_MEMSET(&item_t, 0x00,sizeof(item_t));
                SCI_MEMSET(&item_data, 0x00, sizeof(item_data));
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
                item_t.item_data_ptr = &item_data;
                for(i = 0; i < max_items; i++)
                {
                    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
                    item_data.item_content[0].item_data.image_id = img_id[i];

                    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
                    item_data.item_content[1].item_data.text_id = str_id[i];

                    GUILIST_ReplaceItem(ctrl_id, &item_t,i);
                }
                //if no sim or flymode stk is gray
#ifdef MMI_SHOW_MSA
                if (0 == MMIAPISTK_GetStkCardNum (PNULL, 0)||MMIAPISET_GetFlyMode())
                {
                    //CTRLLIST_SetItemStateById(ctrl_id,6,GUIITEM_STATE_GRAY,TRUE);
                    CTRLLIST_SetItemInvalid(ctrl_id,6,TRUE);
                    CTRLLIST_SetItemGray(ctrl_id,6,TRUE);
                }else{
                    CTRLLIST_SetItemInvalid(ctrl_id,6,FALSE);
                    CTRLLIST_SetItemGray(ctrl_id,6,FALSE);
                }
#else
                if (0 == MMIAPISTK_GetStkCardNum (PNULL, 0)||MMIAPISET_GetFlyMode())
                {
                    //CTRLLIST_SetItemStateById(ctrl_id,6,GUIITEM_STATE_GRAY,TRUE);
                    CTRLLIST_SetItemInvalid(ctrl_id,5,TRUE);
                    CTRLLIST_SetItemGray(ctrl_id,5,TRUE);
                }else{
                    CTRLLIST_SetItemInvalid(ctrl_id,5,FALSE);
                    CTRLLIST_SetItemGray(ctrl_id,5,FALSE);
                }
#endif


		if(TRUE == MMIAPISET_IsDisableShareViaOption())
		{
#ifdef TEST_MAIN_MENU
			CTRLLIST_RemoveItem(ctrl_id,8);  // STR_SETT_LIST_STORAGE_EXT01
#else
#ifdef MMI_SHOW_MSA
			CTRLLIST_RemoveItem(ctrl_id,7);  // STR_SETT_LIST_STORAGE_EXT01
#else
			SCI_TRACE_LOW("func [%s] line [%d] ",__FUNCTION__,__LINE__);	
#ifdef MMI_VOLTE_SUPPORT
			if(MMIAPIPHONE_volte_menu_display_set(MN_DUAL_SYS_1) != 0
			#ifdef MMI_MULTI_SIM_SYS_DUAL
			|| (MMIAPIPHONE_volte_menu_display_set(MN_DUAL_SYS_2) != 0)
			#endif
			)
			{
#ifdef JAVA_SUPPORT
				item_index = 8;
#else
				item_index = 7;
#endif 
				CTRLLIST_RemoveItem(ctrl_id,item_index);  // STR_LIST_SETT_VOLTE
			
			}
#endif
			CTRLLIST_RemoveItem(ctrl_id,6);  // STR_SETT_LIST_STORAGE_EXT01
#endif
#endif
		}
		else
		{
#ifdef MMI_VOLTE_SUPPORT
                if(MMIAPIPHONE_volte_menu_display_set(MN_DUAL_SYS_1) != 0
		#ifdef MMI_MULTI_SIM_SYS_DUAL
		|| (MMIAPIPHONE_volte_menu_display_set(MN_DUAL_SYS_2) != 0)
		#endif
            ){
#ifdef JAVA_SUPPORT
                    item_index = 8;
#else
                    item_index = 7;
#endif 
                    if (0 == MMIAPIPHONE_GetSimAvailableNum (PNULL, 0)||MMIAPISET_GetFlyMode())
                    {
                        CTRLLIST_SetItemInvalid(ctrl_id,item_index,TRUE);
                        CTRLLIST_SetItemGray(ctrl_id,item_index,TRUE);
                    }else{
                        CTRLLIST_SetItemInvalid(ctrl_id,item_index,FALSE);
                        CTRLLIST_SetItemGray(ctrl_id,item_index,FALSE);
                    }
                }
#endif
		}

                if(MMIAPICP_IsMainWinOpen())
                {
                    GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_NULL,LEFT_BUTTON,TRUE);
                }
#ifdef MMI_SUPPORT_CONTROLPANEL//Queen project no need
                else
                {
                    GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_MAIN_SETTING,LEFT_BUTTON,TRUE);
                }
#endif
            }
            break;
#ifdef MMI_SUPPORT_CONTROLPANEL//Queen project no need
        case MSG_CTL_OK:
        case MSG_APP_OK:
            if(!MMIAPICP_IsMainWinOpen())
            {
                MMIAPICP_OpenMainWin();
            }
            break;
#endif
        case MSG_KEYUP_OK:
            if(GUILIST_GetCurItemIndex(ctrl_id)==ID_SET_LIST_STK)
            {
                if(!MMIAPICP_IsMainWinOpen())
                {
                    MMIAPICP_OpenMainWin();
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
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            if(TRUE == MMIAPISET_IsDisableShareViaOption())
            {
                SCI_TRACE_LOW(" func [%s] line [%d] ",__FUNCTION__,__LINE__);
                if(cur_item  == ID_SET_LIST_STORAGE)
                {
                    cur_item+=1;
                }
            }
            switch (cur_item)
            {
                case ID_SET_LIST_SYSTEM:
                    MMK_CreateWin((uint32 *) MMISET_SYSTEM_SETTING_WIN_TAB, PNULL);
                    break;
#ifdef TEST_MAIN_MENU
                case ID_SET_LIST_PHONE:
                    //to do.....
                    MMK_CreateWin((uint32 *) MMISET_PHONE_SETTING_WIN_TAB, PNULL);
                    break;

                case ID_SET_LIST_MDISPLAY:
                    MMIAPISET_EnterDisplaySettingWin();
                    break;

                case ID_SET_LIST_MCALL:
                    MMIAPISET_OpenCallSettingsWindow();
                    break;
#endif
                case ID_SET_LIST_SECURITY:
                    //modify by nhl for security setting
                    MMIAPISET_EnterNewSecuritySettingWin();
                    //to do.....
                    //MMIAPISET_EnterSecuritySettingWin();
                    //add end by nhl
                    break;

                case ID_SET_LIST_CONNECTION:
                    MMIAPICONNECTION_OpenMenuWin();
                    break;
                case ID_SET_LIST_PERSIONALIATION:
                    MMISET_EnterSettingPersonalationWin();
                    break;
                case ID_SET_LIST_TIME_LANGUAGE:
                    MMK_CreateWin((uint32 *) MMISET_TIME_LANGUAGE_WIN_TAB, PNULL);
                    break;
                case ID_SET_LIST_STORAGE:
                    MMK_CreateWin((uint32 *) MMISET_STORAGE_WIN_TAB, PNULL);
                    break;
#ifndef TEST_MAIN_MENU
#ifdef MMI_SHOW_MSA
                case ID_SET_LIST_MS_ACCOUNT:
                    if(MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL))
                        MMK_CreateWin((uint32*)MMISET_SET_MSA_WIN_TAB,PNULL);
                    else
                    {//add as sprd patch for bug513679
                        MMIAPIMSA_Clean_Invalid_Account();
                        MMK_CreateWin((uint32*)MMISET_SET_ADD_NEW_MSA_WIN_TAB,PNULL);
                    }
                    break;
#endif
#ifdef JAVA_SUPPORT
                case ID_SET_LIST_INSTALLED_APPS:
                    MMIAPIJAVA_OpenMainMenu();
                    break;
#endif
#endif
                case ID_SET_LIST_STK:
                    MMISET_StartSTK();
                    break;
#ifdef MMI_VOLTE_SUPPORT
                case ID_SET_LIST_VOLTE:
                    MMK_CreateWin((uint32 *) MMISET_VOLTE_WIN_ID, PNULL);
                    break;
#endif

#ifdef MMI_INDIAN_SOS_SUPPORT
                case ID_SET_LIST_SOS:
                    MMIAPISET_EnterSOSSettingWin();
                    break;
#endif
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

PUBLIC void MMIAPIENG_SHOWVOLTE_WINDOW(void)
{
#ifdef MMI_VOLTE_SUPPORT
	MMK_CreateWin((uint32 *) MMISET_VOLTE_WIN_ID, PNULL);
#endif
}

/*****************************************************************************/
//  Description :HandleSetTimeLanguageWindow
//  Global resource dependence :
//  Author:tangting
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetTimeLanguageWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;

    MMI_CTRL_ID_T               ctrl_id = MMISET_TIME_LANGUAGE_CTRL_ID;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16   cur_item = 0;
    MMI_STRING_T	temp_str = {0};

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id,ctrl_id);
            GUILIST_SetMaxItem(ctrl_id,2, FALSE );
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);

            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
            item_t.item_data_ptr = &item_data;
            item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id = STR_ILIST_TIMEDATE_SETTINGS;

            GUILIST_AppendItem( ctrl_id, &item_t );

            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
            item_t.item_data_ptr = &item_data;
            item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id = STXT_LANGUAGE_SELECT;

            GUILIST_AppendItem( ctrl_id, &item_t );

            GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, cur_item);
            break;
//         case MSG_FULL_PAINT:
// 
//             MMI_GetLabelTextByLang(STR_SETT_HDR_TIME_LANGUAGE_EXT01, &temp_str);
//             if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len))
//             {
//                 GUIWIN_UpdateStb();
//             }
//             break;
            //case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            switch (cur_item)
            {
                case 0:
                    MMISET_CreateDateTimeWin();
                    break;
                case 1:
                    //nhl add windows create func
                    MMK_CreateWin((uint32*)MMISET_SET_LANGUAGE_WIN2_TAB,PNULL);
                    //Language
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
//  Description :HandleSetSystemSettingWindow
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetSystemSettingWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
#if defined (MOTION_SENSOR_TYPE)&& !defined(FLASH_SUPPORT)
    BOOLEAN                   is_sensor_mute    =  FALSE;
    BOOLEAN          is_sensor_change_wallpaper =  FALSE;
    uint32                   sensor_data        =   0;
#endif
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    MMI_TEXT_ID_T str_id[] = {
        STXT_CALL_SETTINGS,
        STR_ILIST_MESSAGE_EXT01,
        STR_SETTINGS_BACKLIGHT,
        STR_OPT_NFS_NOTIFICATION,
        STR_LIST_SW_UPD_OFFINE,
        STR_SETT_LIST_ABOUT_EXT01
    };
    int16               i = 0;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_T      item4_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    GUILIST_ITEM_DATA_T item4_data = {0};
    uint16  max_items = 0;
    uint16   cur_item = 0;
    MMI_CTRL_ID_T               ctrl_id = MMISET_SET_SYSTEM_LIST_CTRL_ID;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:

            MMK_SetAtvCtrl(win_id,ctrl_id);

            max_items = 6;
            cur_item  = 0;
            //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d"
            GUILIST_SetMaxItem(ctrl_id,max_items, FALSE );
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
            item_t.item_data_ptr = &item_data;

            //item_data.softkey_id[0] = TXT_COMMON_OK;
            //item_data.softkey_id[1] = TXT_COMMON_CHOOSE;
            //item_data.softkey_id[2] = STXT_RETURN;
            GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_COMMON_COMMON_BACK,RIGHT_BUTTON,TRUE);

            for(i = 0; i < max_items; i++)
            {
                //if(i==4)//the CONFSERVICE is tongle style
                //continue;
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id = str_id[i];
                if(!GUILIST_InsertItem(ctrl_id, &item_t, i))
                {
                    break;
                }
            }
            /*
               {
            //the CONFSERVICE is tongle style
            SCI_MEMSET(&item_t, 0x00,sizeof(item_t));
            SCI_MEMSET(&item_data, 0x00, sizeof(item_data));
            item_t.item_style    = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
            item_t.item_data_ptr = &item_data;
            item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id = str_id[4];
            //GUILIST_InsertItem(ctrl_id, &item_t, 4);
            GUILIST_ReplaceItem( ctrl_id, &item_t, 4 );
            }
             */
            if(TRUE == MMIAPISET_IsDisableShareViaOption())
            {
                CTRLLIST_RemoveItem(ctrl_id,4);
                CTRLLIST_RemoveItem(ctrl_id,1);
            }
            GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, cur_item);
            break;
//         case MSG_FULL_PAINT:
//             {//reset status bar
//                 MMI_STRING_T    temp_str = {0};
//                 MMI_GetLabelTextByLang(STR_SETT_LIST_SYSTEM_EXT01, &temp_str);
//                 if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len))
//                 {
//                     GUIWIN_UpdateStb();
//                 }
//             }
//             break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            //case MSG_CTL_OK:
            //case MSG_APP_OK:
            //case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            if(TRUE == MMIAPISET_IsDisableShareViaOption())
            {
                if(cur_item>0)
                {
                    cur_item+=1;
                }
                if(cur_item == ID_SET_LIST_UPDATE)
                {
                     cur_item +=1 ;
                }
            }
            switch (cur_item)
            {
                case ID_SET_LIST_CALL:
                    MMIAPISET_OpenCallSettingsWindow();
                    break;

                case ID_SET_LIST_MESSAGE://信息设置
                    //to do.....
                    //MMIAPIIM_OpenSetWin();
                    //MMISMS_CreateSmsSettingWin();
                    MMISMS_OpenMessageSetting();
                    break;

                case ID_SET_LIST_NOTIFICATIONS://通知栏
                    MMISET_EnterSettingNotificationsWin();
                    break;

                    //case ID_SET_LIST_CONFSERVICE:    //快捷键设置
                    //to do.....
                    //MMISET_EnterSettingShortcutWin();
                    //break;

                case ID_SET_LIST_DISPLAY://显示
                    MMIAPISET_EnterDisplaySettingWin();
                    break;

                case ID_SET_LIST_UPDATE:
                    /* Do not upgrade if cell phone is in USB mass storage mode */
                    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                        break;
                    }
                    if(FALSE == MMIAPIFMM_GetDeviceTypeStatus( MMI_DEVICE_SDCARD))
                    {
                        MMICOM_BANNER_INFO_T banner_info = {0};

                        banner_info.banner_type = MMI_BANNER_TEXT;
                        banner_info.start_point.x = 0;
                        banner_info.start_point.y = 0;
                        banner_info.text1_id = STR_NOTE_NO_CARD;

                        MMIAPICOM_OpenBannerWin(win_id, banner_info);
                    }
                    else 
                    {
                        MMISET_EnterSettingUpdateWin();
                    }
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
//  Description :HandleSetPowerManagerWindow
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetPowerManagerWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
#if defined (MOTION_SENSOR_TYPE)&& !defined(FLASH_SUPPORT)
    BOOLEAN                   is_sensor_mute    =  FALSE;
    BOOLEAN          is_sensor_change_wallpaper =  FALSE;
    uint32                   sensor_data        =   0;
#endif
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    MMI_TEXT_ID_T       str_id[] = {STR_SETT_LIST_AUTO_POWER_EXT01,STR_SETT_LIST_BATTERY_STATUS_EXT01};

    int16               i = 0;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_T      item4_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    GUILIST_ITEM_DATA_T item4_data = {0};
    uint16  max_items = 0;
    uint16   cur_item = 0;
    MMI_CTRL_ID_T               ctrl_id = MMISET_POWER_MANAGER_LIST_CTRL_ID;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:

            MMK_SetAtvCtrl(win_id,ctrl_id);
            max_items = 1;
            cur_item  = 0;
            //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d"
            GUILIST_SetMaxItem(ctrl_id,max_items, FALSE );
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
            item_t.item_data_ptr = &item_data;

            GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_COMMON_COMMON_BACK,RIGHT_BUTTON,TRUE);
            for(i = 0; i < max_items; i++)
            {
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id = str_id[i];
                if(!GUILIST_InsertItem(ctrl_id, &item_t, i))
                {
                    break;
                }
            }
            GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, cur_item);
            break;
//         case MSG_FULL_PAINT:
//             {//reset status bar
//                 MMI_STRING_T    temp_str = {0};
//                 MMI_GetLabelTextByLang(STR_SETT_HDR_POWER_MANAGE_EXT01, &temp_str);
//                 if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len))
//                 {
//                     GUIWIN_UpdateStb();
//                 }
//             }
//             break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            switch (cur_item)
            {
                case ID_SET_LIST_AUTO_POWER:
                    //to do....
                    break;

                case ID_SET_LIST_BATTERY_STATUS:
                    //to do.....
                    MMK_CreateWin((uint32*)MMISET_BATTERY_STATUS_WIN_TAB, PNULL);
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
//  Description :HandleSetBatteryStatusWindow
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetBatteryStatusWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
#if defined (MOTION_SENSOR_TYPE)&& !defined(FLASH_SUPPORT)
    BOOLEAN                   is_sensor_mute    =  FALSE;
    BOOLEAN          is_sensor_change_wallpaper =  FALSE;
    uint32                   sensor_data        =   0;
#endif
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    MMI_TEXT_ID_T       str_id[] = {STR_SETT_LIST_BATTERY_USAGE_EXT01,STR_SETT_LIST_BATTERY_TIME_EXT01,STR_SETT_LAST_FULL_CHARGE_EXT01};

    int16               i = 0;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_T      item4_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    GUILIST_ITEM_DATA_T item4_data = {0};
    uint16  max_items = 0;
    uint16   cur_item = 0;
    MMI_CTRL_ID_T               ctrl_id = MMISET_BATTERY_STATUS_LIST_CTRL_ID;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:

            MMK_SetAtvCtrl(win_id,ctrl_id);
            max_items = 3;
            cur_item  = 0;
            //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d"
            GUILIST_SetMaxItem(ctrl_id,max_items, FALSE );
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
            item_t.item_data_ptr = &item_data;

            for(i = 0; i < max_items; i++)
            {
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id = str_id[i];
                if(i==0){
                    uint8          num_str[16] = {0};
                    wchar          wnum_str[8] = {0};
                    uint8  bat_level = MMIAPIPHONE_GetBatCapacity();
                    sprintf((char*)num_str, "%d%s",
                            (int)bat_level, "%");
                    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                    MMIAPICOM_StrToWstr(num_str, wnum_str);
                    item_data.item_content[1].item_data.text_buffer.wstr_ptr = wnum_str;
                    item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wnum_str);
                }else if(i==1){
                    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                    item_data.item_content[1].item_data.text_buffer.wstr_ptr = L"3 hours";
                    item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(L"3 hours");
                }else if(i==2){
                    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                    item_data.item_content[1].item_data.text_buffer.wstr_ptr = L"1 hour ago";
                    item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(L"1 hour ago");
                }
                if(!GUILIST_InsertItem(ctrl_id, &item_t, i))
                {
                    break;
                }
            }

            GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, cur_item);
            break;
//         case MSG_FULL_PAINT:
//             {//reset status bar
//                 MMI_STRING_T    temp_str = {0};
//                 MMI_GetLabelTextByLang(STR_SETT_LIST_BATTERY_STATUS_EXT01, &temp_str);
//                 if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len))
//                 {
//                     GUIWIN_UpdateStb();
//                 }
//             }
//             break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
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

PUBLIC void MMISet_SetPreferDisk(MMIFILE_DEVICE_E device_type)
{
    MMIFILE_DEVICE_E nv_setting = MMI_DEVICE_UDISK;

    if(device_type < MMI_DEVICE_NUM)
    {
        nv_setting = device_type;
        MMINV_WRITE(MMINV_SETTING_PREFER_DISK, &nv_setting);
    }
#ifdef MMS_SUPPORT_DORADO
    MMIMMS_Setting_SetPreferDisk(device_type);
#endif
#ifdef RECORD_SUPPORT
    MMIRECORD_SetRecordStorage(device_type);
#endif
#ifdef MMIFM_SUPPORT_FM_RECORD
    MMIFMRECORD_SetRecordStorage(device_type);//tangting add
#endif
}

LOCAL void MMISET_StartSTK (void)
{
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;

    sim_num = MMIAPISTK_GetStkCardNum (&sim_sys, 1);

    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
        MMIAPISTK_EnterToplevelMenu ( (MN_DUAL_SYS_E) sim_sys);
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    else
    {
        MMIAPISTK_SelectSIMFunc();//Sam.hua	use SelectSimFunc
    }
#endif

    return;
}
#if defined (MMI_VOLTE_SUPPORT) || defined(MMI_2G_4G_SUPPORT)
LOCAL uint8 wait_timer_id = 0;
LOCAL void HandleWaitWinTimer(uint8 timer_id, uint32 param)
{
    if (wait_timer_id == timer_id)
    {
        if (MMK_IsOpenWin( MAIN_WAIT_WIN_ID ))
        {
            MMIDEFAULT_SetBackLight(TRUE);
            MMK_CloseWin(MAIN_WAIT_WIN_ID);
        }
        wait_timer_id= 0;		   
    }
}
LOCAL void ShowWaitingWin(void)
{
    MMI_STRING_T wait_text = {0};

    if(MMK_IsOpenWin(MAIN_WAIT_WIN_ID))
    {
        MMK_CloseWin(MAIN_WAIT_WIN_ID);
    }
    MMI_GetLabelTextByLang(STR_NOTE_WAITING, &wait_text);
    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MAIN_WAIT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,NULL);
    wait_timer_id = MMK_CreateTimerCallback(1000,HandleWaitWinTimer, PNULL,FALSE);				
}
#endif
#ifdef MMI_VOLTE_SUPPORT
#ifdef MMI_MULTI_SIM_SYS_DUAL
#define VOLTE_MAX_ITEMS  2
#else
#define VOLTE_MAX_ITEMS  1
#endif
LOCAL void SetVolteItems(BOOLEAN isUpdate)
{
    MMI_CTRL_ID_T               ctrl_id = MMISET_SET_VOLTE_LIST_CTRL_ID;
    GUILIST_ITEM_T              item_t = {0};
    MMISET_SIM_NAME_T           sim_name = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    BOOLEAN                     toggleState = FALSE;
    uint16                      curItemIndex = 0;
    int i = 0;
    uint8 volte_info;
    BOOLEAN volte_on[MMI_DUAL_SYS_MAX] = {0};
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();	

    item_t.item_data_ptr = &item_data;

    if(!isUpdate)
    {
        GUILIST_SetMaxItem(ctrl_id, VOLTE_MAX_ITEMS,FALSE);
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
    }
    volte_info = MMIAPISET_GetVolteInfo();
    for(i = 0; i <MMI_DUAL_SYS_MAX; i++)
    {
        if(volte_info & (1<<i))
            volte_on[i] = TRUE;
        else
            volte_on[i] = FALSE;
    } 
    curItemIndex = 0;
    if(MMIAPIPHONE_volte_menu_display_set(MN_DUAL_SYS_1) == 1)
    {
	    if(!isUpdate)
	    {
	        item_t.item_style    = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
	        item_data.softkey_id[0] = TXT_NULL;
	        item_data.softkey_id[1] = STXT_SOFTKEY_CHANGE;
	        item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;

	        item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
	        item_data.item_content[0].item_data.text_id = STR_VOLTE_SIM_ONE;
	        GUILIST_InsertItem(ctrl_id, &item_t, curItemIndex);
	    }

	    if (!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1) || (!(e_dualsys_setting & (0x01 << MN_DUAL_SYS_1))) 
			||(SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1))
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
			|| (SIM_STATUS_LOCKED_INVALID == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1))
#endif
			)
	    {
	        CTRLLIST_SetItemGray(ctrl_id, 0, TRUE);
	        CTRLLIST_SetItemInvalid(ctrl_id,0, TRUE);
	        volte_on[0] = FALSE;	 
	    }	
	    CTRLLIST_SetToggleState(ctrl_id, curItemIndex, volte_on[0]);
    }	
#ifdef MMI_MULTI_SIM_SYS_DUAL
    if(MMIAPIPHONE_volte_menu_display_set(MN_DUAL_SYS_1) == 1)
        curItemIndex++;
    if(MMIAPIPHONE_volte_menu_display_set(MN_DUAL_SYS_2) == 1)
    {
	    if(!isUpdate)
	    {
	        item_t.item_style    = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
	        item_data.softkey_id[0] = TXT_NULL;
	        item_data.softkey_id[1] = STXT_SOFTKEY_CHANGE;
	        item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;

	        item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
	        item_data.item_content[0].item_data.text_id = STR_VOLTE_SIM_TWO;

	        GUILIST_InsertItem(ctrl_id, &item_t, curItemIndex);
	    }

	    CTRLLIST_SetToggleState(ctrl_id, curItemIndex, volte_on[1]);

	    if (!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2) || (!(e_dualsys_setting & (0x01 << MN_DUAL_SYS_2))) 
			||(SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_2))
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
			|| (SIM_STATUS_LOCKED_INVALID == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_2))
#endif
			)
	    {
	        CTRLLIST_SetItemGray(ctrl_id, 1, TRUE);
	        CTRLLIST_SetItemInvalid(ctrl_id,1, TRUE);
	        volte_on[1] = FALSE;
	    }	
	    CTRLLIST_SetToggleState(ctrl_id, curItemIndex, volte_on[1]);
    }
#endif	
}
PUBLIC BOOLEAN MMIAPICC_IsInState(CC_CALL_STATE_FOR_USING_E call_state);
PUBLIC BOOLEAN MMIAPIPHONE_GetImsStatus(MN_DUAL_SYS_E dual_sys);

LOCAL MMI_RESULT_E  HandleSetVoLTEWindow(
        MMI_WIN_ID_T    win_id, 
        MMI_MESSAGE_ID_E   msg_id, 
        DPARAM             param
        )
{
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
    GUILIST_ITEM_T      item_t      =   {0};
    GUILIST_ITEM_DATA_T item_data   =   {0};
    MMI_CTRL_ID_T               ctrl_id = 0;
    uint16                      v_space = 0;
    GUI_BG_T    bg = {0};
    MMI_STRING_T    temp_str = {0};

    switch(msg_id) {
        case MSG_OPEN_WINDOW:
            SetVolteItems(FALSE);
            MMK_SetAtvCtrl(win_id, MMISET_SET_VOLTE_LIST_CTRL_ID);
            break;
        case MSG_FULL_PAINT: 
            {
                 GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
                 GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
                 LCD_FillRect(&lcd_dev_info, full_rect,MMI_BLACK_COLOR);
//                                  MMI_GetLabelTextByLang(STR_LIST_SETT_VOLTE, &temp_str);
//                                  if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len)) {
//                                      GUIWIN_UpdateStb();
//                                  }
                 SetVolteItems(TRUE);	
             }
             break;

        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            {
                uint16          curItemIndex = 0;
                 uint16 dual_sys = MN_DUAL_SYS_1;	 
                 MMI_MENU_ID_T menu_id = 0;
                 BOOLEAN is_call_state ;
                 BOOLEAN is_volte_state ;
                 curItemIndex = GUILIST_GetCurItemIndex(MMISET_SET_VOLTE_LIST_CTRL_ID);
                 if(curItemIndex == 0 && (MMIAPIPHONE_volte_menu_display_set(MN_DUAL_SYS_1) == 0))
                     dual_sys = MN_DUAL_SYS_2;
                 else
                     dual_sys = curItemIndex;
                 is_call_state =MMIAPICC_IsInState(CC_IN_CALL_STATE);
                 is_volte_state =MMIAPIPHONE_GetImsStatus((MN_DUAL_SYS_E) dual_sys);
                 SCI_TraceLow("fzb HandleSetVoLTEWindow is_call_state(%d)  is_volte_state %d ",is_call_state,is_volte_state);
                 if(  is_call_state)
                 {
                     MMIPUB_OpenAlertWarningWin(STR_IN_CALL_UNAVAILABLE);
                 }
                 else
                 {
                     BOOLEAN enable = FALSE;
                     CTRLLIST_GetToggleState(MMISET_SET_VOLTE_LIST_CTRL_ID, curItemIndex, &enable);
                     MMIAPISET_SetVolteInfo(dual_sys, enable); 
                     ShowWaitingWin();
                     MMIAPICOM_StatusAreaSetIdleVolteState((MN_DUAL_SYS_E) dual_sys);
                 }

                }
                break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            MMK_CloseWin(win_id);
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;

    }
    return recode;
}
#endif
#ifdef MMI_2G_4G_SUPPORT
#define RAT_AUTO  0
#define RAT_ONLY_2G  2
#define RAT_ONLY_4G  4
#define MODE_MENU_MAX 3
#define MODE_MENU_STYLE  5
uint8 g_mode_flag[MMI_DUAL_SYS_MAX]={0};
static BOOLEAN  g_networktype_initial = FALSE;
MMI_TEXT_ID_T mode_menu_textid[MODE_MENU_STYLE][MODE_MENU_MAX] = {
    {STR_OPT_NET_TYPE_AUTO,TXT_SET_4G_ONLY,TXT_SET_2G_ONLY},
    {TXT_SET_RECOMMEND,TXT_SET_4G,TXT_SET_2G},
    {TXT_SET_LTE_AUTO,TXT_SET_LTE_ONLY,TXT_SET_2G_ONLY},
    {TXT_SET_LTE_RECOMMEND,TXT_SET_LTE,TXT_SET_2G},
    {STR_OPT_NET_TYPE_4PLUS_ENABLE, TXT_SET_4G, STR_OPT_NET_TYPE_4PLUS_DISABLE}};
typedef enum
{
    AUTO,
  //  ONLY_4G,
    ONLY_2G,
}MODE_LOCK_E;
typedef enum
{
    MENU_STYLE_0,
    MENU_STYLE_1,
    MENU_STYLE_2,
    MENU_STYLE_3,
    MENU_STYLE_4
}MODE_MENU_STYLE_E;

MODE_MENU_STYLE_E g_menu_style = 0;
PUBLIC MMI_TEXT_ID_T GetNetworkTypeStringId(void)
{
    MMI_TEXT_ID_T    text_id = TXT_NULL;
	
    if(g_mode_flag[MN_DUAL_SYS_1] == RAT_ONLY_2G)
        text_id = mode_menu_textid[g_menu_style][2];
   // else if(g_mode_flag[MN_DUAL_SYS_1] == RAT_ONLY_4G)
       // text_id = mode_menu_textid[g_menu_style][1];
    else
        text_id =  mode_menu_textid[g_menu_style][0];
     return text_id;	
}
PUBLIC void CreateNetworkTypeWin(void)
{
    uint16 sim_sys = MN_DUAL_SYS_1;
    MMK_CreateWin((uint32*)MMISET_SET_2G_4G_WIN_TAB, (ADD_DATA)sim_sys);
}
PUBLIC void InitSet2G4GMode(MN_DUAL_SYS_E dual_sys)
{
    uint8  mode = 0;
    uint16 return_value = 0;
    uint32 item_id = 0;
    
    if (MN_DUAL_SYS_1 == dual_sys)
    {
        item_id = MMINV_SET_2G_4G_NETWORK_SIM1;
    }
    else
    {
        item_id = MMINV_SET_2G_4G_NETWORK_SIM2;
    }
    
    MMINV_READ(item_id, &mode, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
#ifdef MMI_ORTHODOX_VERSION
        mode = RAT_ONLY_2G;
#else
        mode = RAT_AUTO;
#endif
        MNPHONE_SetRat(mode, dual_sys);
        MMINV_WRITE(item_id, &mode);
    }
    
    return;
}

PUBLIC void Reset2G4GMode(void)
{
    uint32 i = 0;
#ifdef MMI_ORTHODOX_VERSION
    uint8 mode = RAT_ONLY_2G;
#else
    uint8 mode = RAT_AUTO;
#endif

#ifndef WIN32
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIAPIPHONE_IsSimAvailable(i))
        {
            MNPHONE_SetRat(mode, i);
        }
    }	
#endif
    MMINV_WRITE(MMINV_SET_2G_4G_NETWORK_SIM1,&mode);
    MMINV_WRITE(MMINV_SET_2G_4G_NETWORK_SIM2,&mode);  
}
uint16  Get2G4GMode(uint32 dual_sys)
{
    uint16   return_value = 0;
    uint8   mode = 0;
    if(dual_sys ==0)
        MMINV_READ(MMINV_SET_2G_4G_NETWORK_SIM1,&mode,return_value);
    else
        MMINV_READ(MMINV_SET_2G_4G_NETWORK_SIM2,&mode,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
#ifdef MMI_ORTHODOX_VERSION
    uint8 mode = RAT_ONLY_2G;
#else
    uint8 mode = RAT_AUTO;
#endif
        if(dual_sys ==0)		
            MMINV_WRITE(MMINV_SET_2G_4G_NETWORK_SIM1,&mode);
        else
            MMINV_WRITE(MMINV_SET_2G_4G_NETWORK_SIM2,&mode);
    }
    return mode;
}
void  Set2G4GMode(uint32 dual_sys,uint8   mode)
{
    g_mode_flag[dual_sys] = mode;

    if (dual_sys==0)
        MMINV_WRITE(MMINV_SET_2G_4G_NETWORK_SIM1,&mode);
    else
        MMINV_WRITE(MMINV_SET_2G_4G_NETWORK_SIM2,&mode);

}
PUBLIC void Init2G4GMode(MN_DUAL_SYS_E dual_sys)
{
    uint8 curmode = 0;
    
#ifndef WIN32
    curmode = MNPHONE_GetRat(dual_sys);
#endif
    Set2G4GMode(dual_sys,curmode);
    g_mode_flag[dual_sys] = Get2G4GMode(dual_sys);
    
    if (MMIAPIPHONE_4g_icon_display_style() == 1)
    {
        if(MMIAPIPHONE_prefer_network_display_style() == 1) 
        {
            g_menu_style = MENU_STYLE_3;
        }
        else 
        {
            g_menu_style = MENU_STYLE_2;
        }
    }
    else if (MMIAPIPHONE_4g_icon_display_style() == 2)
    {
        g_menu_style = MENU_STYLE_4;
    }
    else
    {
        if(MMIAPIPHONE_prefer_network_display_style() == 1) 
        {
            g_menu_style = MENU_STYLE_1;
        }
        else 
        {
            g_menu_style = MENU_STYLE_0;
        }
    }
}

PUBLIC void NetworkType2G4GInit(void)
{
    uint16 i=0;
    if(g_networktype_initial == TRUE)
        return;
    SCI_TraceLow("NetworkTypeInit ");	
    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        Init2G4GMode(i);
    }
    g_networktype_initial = TRUE;
}

PUBLIC void MMIAPISET_Open2G4GSettingsWindow(void)
{
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);	
    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else
    {
        MMK_CreateWin((uint32*)MMISET_2G_4G_SIM_WIN_TAB,PNULL);
    }
}
LOCAL void MMIsetSimItems(BOOLEAN isUpdate)
{
    MMI_CTRL_ID_T               ctrl_id = MMISET_2G4G_SIM_CTRL_ID;
    GUILIST_ITEM_T              item_t = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    MMI_STRING_T    title_str = {0};
    uint16                      curItemIndex = 0;
    MMI_TEXT_ID_T    text_id = TXT_NULL;
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();	

    if(!isUpdate)
    {
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        GUILIST_SetMaxItem(ctrl_id,1, FALSE );
#else
        GUILIST_SetMaxItem(ctrl_id,2, FALSE );
#endif
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
    }

    if (g_mode_flag[MN_DUAL_SYS_1] == RAT_ONLY_2G)
        text_id = mode_menu_textid[g_menu_style][2];
    //else if(g_mode_flag[MN_DUAL_SYS_1] == RAT_ONLY_4G)
       // text_id = mode_menu_textid[g_menu_style][1];
    else
        text_id =  mode_menu_textid[g_menu_style][0];
    GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
//     MMIRES_GetText(STR_SIM_HDR_SELECTION_EXT01, PNULL, &title_str);
//     MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str.wstr_ptr, title_str.wstr_len);
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
    item_t.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[0].item_data.text_id = STR_ILIST_SIM_ONE;
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id = text_id;
    item_data.item_content[1].is_opacity = TRUE;
//     GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);
    item_data.softkey_id[0] = STXT_SELECT;
    item_data.softkey_id[1] = COMMON_TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    curItemIndex=0;
    if(isUpdate)
    {
        GUILIST_ReplaceItem(ctrl_id, &item_t, curItemIndex);
    }
    else
    {
        GUILIST_InsertItem(ctrl_id, &item_t, curItemIndex);
    }
    if (!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1) || (!(e_dualsys_setting & (0x01 << MN_DUAL_SYS_1))) 
		|| (SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1))
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
		|| (SIM_STATUS_LOCKED_INVALID == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1))
#endif
		)
	{
        CTRLLIST_SetItemGray(ctrl_id, 0, TRUE);
        CTRLLIST_SetItemInvalid(ctrl_id,0, TRUE);
    }		

#ifdef MMI_MULTI_SIM_SYS_DUAL
    if(g_mode_flag[MN_DUAL_SYS_2] == RAT_ONLY_2G)
        text_id = mode_menu_textid[g_menu_style][2];
   // else if(g_mode_flag[MN_DUAL_SYS_2] == RAT_ONLY_4G)
     //   text_id = mode_menu_textid[g_menu_style][1];
    else
        text_id = mode_menu_textid[g_menu_style][0];
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
    item_t.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[0].item_data.text_id = STR_ILIST_SIM_TWO;
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id = text_id;
    item_data.item_content[1].is_opacity = TRUE;
//     GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);
    item_data.softkey_id[0] = STXT_SELECT;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    curItemIndex++;
    if(isUpdate)
    {
        GUILIST_ReplaceItem(ctrl_id, &item_t, curItemIndex);
    }
    else
    {
        GUILIST_InsertItem(ctrl_id, &item_t, curItemIndex);
    }
    if (!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2) || (!(e_dualsys_setting & (0x01 << MN_DUAL_SYS_2))) 
		|| (SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_2))
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
		|| (SIM_STATUS_LOCKED_INVALID == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_2))
#endif
		)
    {
        CTRLLIST_SetItemGray(ctrl_id, 1, TRUE);
        CTRLLIST_SetItemInvalid(ctrl_id,1, TRUE);
    }	
#endif
}

LOCAL MMI_RESULT_E  HandleSimWinHandleMsg(
        MMI_WIN_ID_T    win_id, 
        MMI_MESSAGE_ID_E   msg_id, 
        DPARAM             param
        )
{

    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMISET_2G4G_SIM_CTRL_ID;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16   cur_item = 0;
    int i=0;
    MMI_STRING_T    title_str = {0};
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMI_TEXT_ID_T    text_id = TXT_NULL;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMIsetSimItems(FALSE);
            MMK_SetAtvCtrl(win_id,ctrl_id);

            break;
        case MSG_FULL_PAINT:
            MMIsetSimItems(TRUE);
            break;
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            switch (cur_item)
            {
                case 0:
                    MMK_CreateWin((uint32*)MMISET_SET_2G_4G_WIN_TAB, (ADD_DATA)cur_item);
                    break;
#ifdef MMI_MULTI_SIM_SYS_DUAL		 
                case 1:
                    MMK_CreateWin((uint32*)MMISET_SET_2G_4G_WIN_TAB, (ADD_DATA)cur_item);
                    break;
#endif		 
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
LOCAL MMI_RESULT_E Set2G4GNetWorkWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_2G4G_CTRL_ID;
    uint16 cur_selection = 0;
    uint8 mode=0;
	MN_PLMN_T sim_hplmn = {0};
    uint32  dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            //Init2G4GMode(dual_sys);
            MMIAPIPHONE_GetSimHPLMN(&sim_hplmn, dual_sys);
            if (MMIAPISET_IsVodafoneVersion() && (268 == sim_hplmn.mcc) && (1 == sim_hplmn.mnc))
			{
                GUILIST_SetMaxItem(ctrl_id, 1, FALSE);
                GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
                MMIAPISET_AppendListItemByTextIdExt(mode_menu_textid[g_menu_style][0], STXT_SELECT, TXT_NULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO); 
                cur_selection = AUTO;
            }
            else 
			{
                GUILIST_SetMaxItem(ctrl_id,2, FALSE);
                GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
                MMIAPISET_AppendListItemByTextIdExt(mode_menu_textid[g_menu_style][0], STXT_SELECT, TXT_NULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO); 
                MMIAPISET_AppendListItemByTextIdExt(mode_menu_textid[g_menu_style][2], STXT_SELECT, TXT_NULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);  

                if (g_mode_flag[dual_sys] == RAT_ONLY_2G)
                    cur_selection = ONLY_2G;
                // else if(g_mode_flag[dual_sys] == RAT_ONLY_4G)
                    //   cur_selection =ONLY_4G;
                else
                    cur_selection = AUTO;
            }	

            //set selected item
            GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

            //set current item
            GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
            MMK_SetAtvCtrl(win_id,ctrl_id);
            break;

        case MSG_FULL_PAINT:
            break;

        case MSG_KEYDOWN_CANCEL:
        case MSG_KEYDOWN_OK:
            break;

        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

#ifdef TOUCH_PANEL_SUPPORT 
        case MSG_CTL_PENOK:
#endif 
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
            if(cur_selection == ONLY_2G)
                mode = RAT_ONLY_2G;
            //else if(cur_selection == ONLY_4G)
               // mode = RAT_ONLY_4G;
            else
                mode = RAT_AUTO;
            SCI_TraceLow("Set2G4GNetWorkWinHandleMsg == g_mode_flag[%d]=%d,mode=%d",dual_sys,g_mode_flag[dual_sys],mode);
            if(g_mode_flag[dual_sys]  != mode)
            {
                MNPHONE_ChangeRat(mode,dual_sys);
                Set2G4GMode(dual_sys,mode);
                GUIWIN_UpdateStb();
                MMISET_SetSwitchRatState(TRUE);
                ShowWaitingWin();
            }
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}
#endif

/*****************************************************************************/
//  Description :HandleSetStorageWindow
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetStorageWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
#if defined (MOTION_SENSOR_TYPE)&& !defined(FLASH_SUPPORT)
    BOOLEAN                   is_sensor_mute    =  FALSE;
    BOOLEAN          is_sensor_change_wallpaper =  FALSE;
    uint32                   sensor_data        =   0;
#endif
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    MMI_TEXT_ID_T       str_id[] = {STR_LIST_BACKUP,STR_LIST_RESTORE
#ifdef UMEM_SUPPORT
        ,STR_LIST_PREFERRED_STORE_EXT01
#endif		
    };

    int16               i = 0;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_T      item4_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    GUILIST_ITEM_DATA_T item4_data = {0};
    uint16  max_items = 0;
    uint16   cur_item = 0;
    MMI_CTRL_ID_T               ctrl_id = MMISET_SET_STORAGE_CTRL_ID;
    const CTRLLIST_ITEM_T *list_item_ptr = PNULL;
    MMICOM_BANNER_INFO_T banner_info = {0};
    GUI_POINT_T pin_alert_start_point = {0, 0};

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:

            MMK_SetAtvCtrl(win_id,ctrl_id);
#ifdef UMEM_SUPPORT
            max_items = 3;
#else
            max_items = 2;
#endif

            cur_item  = 0;
            //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d"
            GUILIST_SetMaxItem(ctrl_id,max_items, FALSE );
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
            item_t.item_data_ptr = &item_data;

            for(i = 0; i < 2; i++)
            {
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id = str_id[i];
                if(!GUILIST_InsertItem(ctrl_id, &item_t, i))
                {
                    break;
                }
            }
#ifdef UMEM_SUPPORT
            SCI_MEMSET(&item_t, 0x00,sizeof(item_t));
            SCI_MEMSET(&item_data, 0x00, sizeof(item_data));
            item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
            item_t.item_data_ptr = &item_data;
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id = str_id[2];
            item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_ID;
            if(MMISet_GetPreferDisk()==MMI_DEVICE_SDCARD){
                item_data.item_content[1].item_data.text_id  = STR_ILIST_MEMORY_CARD_EXT01;
            }else{
                item_data.item_content[1].item_data.text_id  = STR_ILIST_PHONE_EXT01;
            }
            item_data.item_content[1].is_opacity = TRUE;
//             GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);
            GUILIST_AppendItem(ctrl_id, &item_t);
#endif
            GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, cur_item);
            break;
        case MSG_FULL_PAINT:
            {//reset status bar
//                 MMI_STRING_T    temp_str = {0};
//                 MMI_GetLabelTextByLang(STR_SETT_HDR_STORAGE_EXT01, &temp_str);
//                 if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len))
//                 {
//                     GUIWIN_UpdateStb();
//                 }
#ifdef UMEM_SUPPORT	
                list_item_ptr = CTRLLIST_GetItem(ctrl_id, 2);
                if(MMISet_GetPreferDisk()==MMI_DEVICE_SDCARD){
                    list_item_ptr->data_ptr->item_content[1].item_data.text_id  = STR_ILIST_MEMORY_CARD_EXT01;
                }else{
                    list_item_ptr->data_ptr->item_content[1].item_data.text_id  = STR_ILIST_PHONE_EXT01;
                }
                if(!MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_SDCARD))
                {
                    CTRLLIST_SetItemInvalid(ctrl_id,0,TRUE);
                    CTRLLIST_SetItemGray(ctrl_id,0,TRUE);
                    CTRLLIST_SetItemInvalid(ctrl_id,1,TRUE);
                    CTRLLIST_SetItemGray(ctrl_id,1,TRUE);
                }
#endif	
            }
            break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
            //case MSG_CTL_OK:
            //case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            //case MSG_KEYUP_WEB:
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            switch (cur_item)
            {
                case ID_SET_LIST_CREATE_BACKUP:
                    if(!MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_SDCARD))
                    {
                        banner_info.banner_type = MMI_BANNER_TEXT;
                        banner_info.text1_id = STR_NOTE_NO_CARD;
                        banner_info.start_point = pin_alert_start_point;
                        MMIAPICOM_OpenBannerWin(win_id, banner_info);
                        break;
                    }
                    MMIIKB_EnterBackupWin();
                    //to do....
                    break;

                case ID_SET_LIST_RESTORE_BACKUP:
                    if(!MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_SDCARD))
                    {
                        banner_info.banner_type = MMI_BANNER_TEXT;
                        banner_info.text1_id = STR_NOTE_NO_CARD;
                        banner_info.start_point = pin_alert_start_point;
                        MMIAPICOM_OpenBannerWin(win_id, banner_info);
                        break;
                    }
                    MMIIKB_EnterRestoreWin();
                    break;
#ifdef UMEM_SUPPORT		
                case ID_SET_LIST_PREFER_STORAGE:
                    MMK_CreateWin((uint32 *)MMISET_PREFER_STORAGE_WIN_TAB, PNULL);
                    break;
#endif		
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
//  Description :HandleSetStorageWindow
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetPreferStorageWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
#if defined (MOTION_SENSOR_TYPE)&& !defined(FLASH_SUPPORT)
    BOOLEAN                   is_sensor_mute    =  FALSE;
    BOOLEAN          is_sensor_change_wallpaper =  FALSE;
    uint32                   sensor_data        =   0;
#endif
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    int16               i = 0;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16  max_items = 0;
    uint16   cur_item = 0;
    MMI_CTRL_ID_T               ctrl_id = MMISET_SET_PREFER_STORAGE_CTRL_ID;
    MMICONNECTION_GPRS_SETTINGS_E grps_status = 0;
    MMICOM_BANNER_INFO_T banner_info = {0};
    GUI_POINT_T pin_alert_start_point = {0, 0};
    BOOLEAN udisk_exit;
    BOOLEAN sd_exit;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:

            MMK_SetAtvCtrl(win_id,ctrl_id);

            udisk_exit = MMIAPIFMM_GetDeviceTypeStatus (MMI_DEVICE_UDISK);
            sd_exit = MMIAPIFMM_GetDeviceTypeStatus (MMI_DEVICE_SDCARD);

            max_items = (udisk_exit ? 1 : 0) + (sd_exit ? 1 : 0);
            cur_item  = 0;
            //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d"
            GUILIST_SetMaxItem(ctrl_id,max_items, FALSE );
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            item_t.item_style    = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
            item_t.item_data_ptr = &item_data;

            if (udisk_exit)
            {
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id  = STR_ILIST_PHONE_EXT01;
                if(!GUILIST_InsertItem(ctrl_id, &item_t, 0)) {
                    break;
                }
            }

            if(sd_exit)
            {
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id  = STR_ILIST_MEMORY_CARD_EXT01;
                if(!GUILIST_InsertItem(ctrl_id, &item_t, 1)) {
                    break;
                }
            }
            //if (!MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD),
            //    MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD)))
            //{
            //    GUILIST_SetItemGray(ctrl_id, 1, TRUE);
            //	GUILIST_SetSelectedItem(ctrl_id, 0, TRUE);
            //}
            if(!MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_SDCARD))
            {
                CTRLLIST_SetItemInvalid(ctrl_id,1,TRUE);
                CTRLLIST_SetItemGray(ctrl_id,1,TRUE);
                GUILIST_SetSelectedItem(ctrl_id, 0, TRUE);
                GUILIST_SetCurItemIndex(ctrl_id, 0);
                MMISet_SetPreferDisk(MMI_DEVICE_UDISK);
            }
            else if(MMISet_GetPreferDisk()==MMI_DEVICE_SDCARD)
            {
                GUILIST_SetSelectedItem(ctrl_id, 1, TRUE);
                GUILIST_SetCurItemIndex(ctrl_id, 1);
            }
            else{
                GUILIST_SetSelectedItem(ctrl_id, 0, TRUE);
                GUILIST_SetCurItemIndex(ctrl_id, 0);
            }
            //GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
            break;
//         case MSG_FULL_PAINT:
//             {//reset status bar
//                 MMI_STRING_T    temp_str = {0};
//                 MMI_GetLabelTextByLang(STR_HDR_PREFERRED_STORE_EXT01, &temp_str);
//                 if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len))
//                 {
//                     GUIWIN_UpdateStb();
//                 }
//             }
//             break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
            //case MSG_CTL_OK:
            //case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            //case MSG_KEYUP_WEB:
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            udisk_exit = MMIAPIFMM_GetDeviceTypeStatus (MMI_DEVICE_UDISK);
            sd_exit = MMIAPIFMM_GetDeviceTypeStatus (MMI_DEVICE_SDCARD);
            if (udisk_exit)
            {
                if(cur_item == 0)
                    MMISet_SetPreferDisk(MMI_DEVICE_UDISK);
                else if(sd_exit && cur_item == 1)
                    MMISet_SetPreferDisk(MMI_DEVICE_SDCARD);
            }
            else if (sd_exit)
            {
                if(cur_item == 0)
                    MMISet_SetPreferDisk(MMI_DEVICE_SDCARD);
            }
            {
                banner_info.banner_type = MMI_BANNER_TEXT;
                banner_info.text1_id = STR_PREFERRED_STORAGE_EXT01;
                banner_info.start_point = pin_alert_start_point;
                MMIAPICOM_OpenBannerWin(PNULL, banner_info);
            }
            MMK_CloseWin(win_id);
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
//  Description :MMISet_OpenThemeWin
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC void MMISet_OpenThemeWin(void)
{
#ifdef MMI_COMPILE_E
    MMK_CreateWin((uint32 *)MMISET_DISPLAY_COLOR_THEME_WIN_TAB, PNULL);
#endif
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetPhoneLockWindow(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
#if defined (MOTION_SENSOR_TYPE)&& !defined(FLASH_SUPPORT)
    BOOLEAN                   is_sensor_mute    =  FALSE;
    BOOLEAN          is_sensor_change_wallpaper =  FALSE;
    uint32                   sensor_data        =   0;
#endif
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    int16               i = 0;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16  max_items = 0;
    uint16   cur_item = 0;
    MMI_CTRL_ID_T               ctrl_id = MMISET_SET_PHONE_LOCK_CTRL_ID;
    BOOLEAN status = FALSE;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:

            MMK_SetAtvCtrl(win_id,ctrl_id);

            max_items = 2;
            cur_item  = 0;
            //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d"
            GUILIST_SetMaxItem(ctrl_id,max_items, FALSE );
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            item_t.item_style    = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
            item_t.item_data_ptr = &item_data;

            item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = L"Open";
            item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(L"Open");
            if(!GUILIST_InsertItem(ctrl_id, &item_t, 0)) {
                break;
            }

            item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = L"Close";
            item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(L"Close");
            if(!GUILIST_InsertItem(ctrl_id, &item_t, 1)) {
                break;
            }

            status = MMIAPISET_GetPowerOnPwdStatus();
            if(status == TRUE){
                GUILIST_SetSelectedItem(ctrl_id, 0, TRUE);
                GUILIST_SetCurItemIndex(ctrl_id, 0);
            }else{
                GUILIST_SetSelectedItem(ctrl_id, 1, TRUE);
                GUILIST_SetCurItemIndex(ctrl_id, 1);
            }
            //GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
            //GUILIST_SetCurItemIndex(ctrl_id, cur_item);
            break;
//         case MSG_FULL_PAINT:
//             {//reset status bar
//                 //MMI_STRING_T    temp_str = {0};
//                 //MMI_GetLabelTextByLang(TXT_GPRS, &temp_str);
//                 if(MMIAPICOM_StatusAreaSetViewHeaderinfo(L"Phone Lock", MMIAPICOM_Wstrlen(L"Phone Lock")))
//                 {
//                     GUIWIN_UpdateStb();
//                 }
//             }
//             break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            switch(cur_item)
            {
                case 0:
                    MMISET_SetPowerOnPwdStatus(TRUE);
                    break;
                case 1:
                    MMISET_SetPowerOnPwdStatus(FALSE);
                    break;
                default:
                    break;
            }
            MMK_CloseWin(win_id);
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
//  Description : open phone Lock setting window
//  Global resource dependence :
//  Author: dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_PhoneLockSettingWin(void)
{
    MMK_CreateWin((uint32 *)MMISET_PHONE_LOCK_WIN_TAB, PNULL);
}
/*****************************************************************************/
//  Description : if the setting main win is open
//  Global resource dependence :
//  Author: dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsMainWinOpen(void)
{
    return MMK_IsOpenWin(MMISET_SET_PHONE_WIN_ID);
}
#ifdef UNISOC_TEST_ENTRY_FOR_TESTER
/*****************************************************************************/
//  Description : temp test entry for tester
//  Global resource dependence :
//  Author:pan.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTestEntryWindow(
        MMI_WIN_ID_T    win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            MMK_SetAtvCtrl(win_id,MMISET_TEST_ENTRY_MENU_CTRL_ID);
            break;

        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            GUIMENU_GetId(MMISET_TEST_ENTRY_MENU_CTRL_ID,&group_id,&menu_id);
            switch (menu_id)
            {

                case ID_TEST_ENTRY_STK:
                    {
                        uint16 sim_sys = MN_DUAL_SYS_1;
                        uint32 sim_num = 0;

                        if (MMIAPISET_GetFlyMode())
                        {
                            MMIAPIPHONE_AlertSimNorOKStatus();
                            break;
                        }

                        sim_num = MMIAPISTK_GetStkCardNum(&sim_sys, 1);
                        if (sim_num == 0)
                        {
                            MMIAPIPHONE_AlertSimNorOKStatus();
                        }
                        else if (sim_num == 1)
                        {
                            MMIAPISTK_EnterToplevelMenu((MN_DUAL_SYS_E)sim_sys);
                        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                        else
                        {
                            MMIAPISTK_SelectSIMFunc();
                        }
#endif
                    }
                    break;

                case ID_TEST_ENTRY_PROFILE:
                    MMIENVSET_OpenMainMenuWindow();
                    break;
#ifdef BLUETOOTH_SUPPORT
                case  ID_TEST_ENTRY_BT:
                    MMIMAINMENU_StartBT();
                    break;
#endif
                case ID_TEST_ENTRY_DORADO:
                    {
#ifdef BROWSER_SUPPORT
                        MMIBROWSER_ENTRY_PARAM entry_param = {0};

                        entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
                        entry_param.dual_sys = MN_DUAL_SYS_MAX;
                        entry_param.url_ptr = PNULL;
                        entry_param.user_agent_ptr = PNULL;
#ifdef BROWSER_SUPPORT_DORADO  
                        entry_param.browser_type = MMIBROWSER_TYPE_DORADO;
#elif defined (OPERA_MINI_SUPPORT)
						entry_param.browser_type = MMIBROWSER_TYPE_OPERA_MINI;
#else
#error "Either BROWSER_SUPPORT_DORADO or OPERA_MINI_SUPPORT has to be defined here."
#endif

                        MMIAPIBROWSER_Entry(&entry_param);
#endif
                    }
                    break;
#ifdef MMI_COMPILE_E
                case ID_TEST_ENTRY_COLOR_THEME:
                    MMK_CreateWin((uint32 *)MMISET_DISPLAY_COLOR_THEME_WIN_TAB, PNULL);
                    break;
#endif
                case ID_TEST_ENTRY_SET_SECURITY_CODE:
                    //MMK_CreateWin((uint32 *)MMISET_SECURITY_CODE_WIN_TAB, PNULL);
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
//  Description : temp set security code for tester
//  Global resource dependence : 
//  Author:pan.yang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_ReadSecurityCode(void)
{
    MN_RETURN_RESULT_E   return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_SECURITY_CODE,&s_setting_security_code,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        return FALSE;
    }
    return TRUE;
}
/*****************************************************************************/
//  Description : temp set security code for tester
//  Global resource dependence : 
//  Author:pan.yang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_GetPrivacyEnable(void)
{
    MN_RETURN_RESULT_E   return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_PRIVACY_MODE, &s_setting_privacy_mode, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_PRIVACY_MODE, &s_setting_privacy_mode);
    }

    return s_setting_privacy_mode;
}
/*****************************************************************************/
//  Description : temp set security code for tester
//  Global resource dependence : 
//  Author:pan.yang
//  Note: 
/*****************************************************************************/
LOCAL void SetWinListParam(MMI_WIN_ID_T    win_id,MMI_CTRL_ID_T ctrl_id )
{


    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16   cur_item = 0;
    int i=0;
    //MMI_TEXT_ID_T               str_id[] = {TXT_SET_ON,TXT_SET_OFF};


    GUILIST_RemoveAllItems(ctrl_id);
    GUILIST_SetMaxItem(ctrl_id,2, FALSE );
    GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
    cur_item = 0;

    item_t.item_style    = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[0].item_data.text_id = STR_SETT_ILIST_SECURITY_CODE_EXT01;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = TXT_COMMON_CHOOSE;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    GUILIST_AppendItem( ctrl_id, &item_t );
    if(MMISET_ReadSecurityCode())
    {
        GUILIST_SetToggleState(ctrl_id, cur_item, s_setting_security_code.is_enable);                
    }
    else
    {
        GUILIST_SetToggleState(ctrl_id, cur_item, FALSE);
    }
    cur_item++;
    item_t.item_style    = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[0].item_data.text_id = STR_SETT_ILIST_PRIVACY_MODE_EX01;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = TXT_COMMON_CHOOSE;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    GUILIST_AppendItem( ctrl_id, &item_t );
    if(MMISET_GetPrivacyEnable())
    {
        GUILIST_SetToggleState(ctrl_id, cur_item, s_setting_privacy_mode);                
    }
    else
    {
        GUILIST_SetToggleState(ctrl_id, cur_item, FALSE);
    }

}
/*****************************************************************************/
//  Description : temp set security code for tester
//  Global resource dependence : 
//  Author:pan.yang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_WriteSecurityCode(const wchar *code_ptr, uint16 code_len)
{
    if(PNULL == code_ptr)
    {
        return FALSE;
    }
    s_setting_security_code.code_len = (uint8)(code_len & 0x00FF);
    MMI_WSTRNTOSTR((uint8 *)s_setting_security_code.code,MMISET_SECURITY_CODE_LEN_MAX,code_ptr,code_len,code_len);
    MMINV_WRITE(MMINV_SET_SECURITY_CODE,&s_setting_security_code);
    return TRUE;
}
/*****************************************************************************/
//  Description : temp set security code for tester
//  Global resource dependence : 
//  Author:pan.yang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetSecurityCodeEnable(BOOLEAN is_enable)
{
    s_setting_security_code.is_enable = is_enable;
    MMINV_WRITE(MMINV_SET_SECURITY_CODE,&s_setting_security_code);
    return TRUE;
}
/*****************************************************************************/
//  Description : temp set security code for tester
//  Global resource dependence : 
//  Author:pan.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetPrivacyEnable(BOOLEAN is_enable)
{
    s_setting_privacy_mode = is_enable;
    MMINV_WRITE(MMINV_SET_PRIVACY_MODE,&s_setting_privacy_mode);
    return;
}
/*****************************************************************************/
//  Description : temp set security code for tester
//  Global resource dependence : 
//  Author:pan.yang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNewSecurityCodeDialogWindow(
        MMI_WIN_ID_T    win_id, 
        MMI_MESSAGE_ID_E   msg_id, 
        DPARAM             param
        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_STRING_T str_t = {0};
    MMI_CTRL_ID_T       input_field_ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID );

    MMIPUB_INFO_T *add_data_ptr = PNULL;
    BOOLEAN* is_init_set_ptr = PNULL;

    add_data_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if(PNULL==add_data_ptr)
    {
        recode = MMI_RESULT_FALSE;
        return recode;
    }
    is_init_set_ptr= (BOOLEAN *)add_data_ptr->user_data;
    if(PNULL==is_init_set_ptr)
    {
        recode = MMI_RESULT_FALSE;
        return recode;
    }

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIEDIT_SetTextMaxLen(input_field_ctrl_id,MMISET_SECURITY_CODE_LEN_MAX, MMISET_SECURITY_CODE_LEN_MAX);
            GUIEDIT_SetIm(input_field_ctrl_id,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
            break;
        case MSG_APP_OK:
        case MSG_CTL_OK:
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
            {
                MMI_STRING_T   str = {0};
                GUIEDIT_GetString(input_field_ctrl_id,&str);
                if(str.wstr_len < 4)
                {

                    GUIEDIT_ClearAllStr(input_field_ctrl_id);
                }
                else
                {
                    //MMISET_OpenCheckNewSecurityCodeDialogWin(&str, *is_init_set_ptr);
                    MMISET_WriteSecurityCode(str.wstr_ptr, str.wstr_len);

                    MMISET_SetSecurityCodeEnable(TRUE);
                    MMK_CloseWin(win_id);
                }

            }
            break;
        case MSG_CLOSE_WINDOW:
            if(PNULL != add_data_ptr->user_data)
            {
                SCI_FREE(add_data_ptr->user_data);
            }
            MMK_FreeWinAddData(win_id);
            break;
            break;
        default:
            recode = MMIPUB_HandleDialogInputFieldWinMsg(win_id, msg_id, param);
            break;
    }
    return (recode);
}
/*****************************************************************************/
//  Description : temp set security code for tester
//  Global resource dependence : 
//  Author:pan.yang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_InputNewSecurityCodeDialogWin(BOOLEAN is_input_code)
{
    MMI_WIN_ID_T win_id = MMISET_NEW_SECURITY_CODE_WIN_ID;
    MMI_STRING_T title = {0};
    BOOLEAN* is_init_set_ptr =  (BOOLEAN*)SCI_ALLOCA(sizeof(BOOLEAN));
    SCI_MEMSET(is_init_set_ptr, 0x00, sizeof(BOOLEAN));
    *is_init_set_ptr = is_input_code;
    MMI_GetLabelTextByLang(STR_SETT_INPUT_NEW_SECUR_EX01, &title );
    MMIPUB_OpenDialogInputFieldWinByTextPtrEx(
            MMK_GetFirstAppletHandle(),
            &title , 
            &win_id,
            PNULL,
            MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
            HandleNewSecurityCodeDialogWindow,
            (uint32)is_init_set_ptr
            );

    return TRUE;
}

/*****************************************************************************/
//  Description : temp set security code for tester
//  Global resource dependence : 
//  Author:pan.yang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetSecurityCodeWindow(
        MMI_WIN_ID_T    win_id, 
        MMI_MESSAGE_ID_E   msg_id, 
        DPARAM             param
        )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMISET_SECURITY_LIST_CTRL_ID;
    uint16   cur_item = 0;
    int i=0;
    MMISET_SECURITY_CODE_T device_lock_pwd = {0};
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            SetWinListParam(win_id,ctrl_id);
            MMK_SetAtvCtrl(win_id,ctrl_id);
            break;
        case MSG_FULL_PAINT:
            if(PNULL !=param && TRUE == *(BOOLEAN*)param)
            {
                SetWinListParam(win_id, ctrl_id);
                if(MMISET_ReadSecurityCode())
                {
                    GUILIST_SetToggleState(ctrl_id, 1, s_setting_security_code.is_enable);
                }
                else
                {
                    GUILIST_SetToggleState(ctrl_id, 1, FALSE);
                }
                //GUILIST_SetCurItemIndex(ctrl_id, 1);
            }
            break;
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            {
                cur_item = GUILIST_GetCurItemIndex(ctrl_id);
                switch(cur_item)
                {
                    case 0:
                        if(s_setting_security_code.is_enable)
                        {
                            MMISET_SetSecurityCodeEnable(FALSE);
                        }
                        else
                        {
                            //MMISET_InputNewSecurityCodeDialogWin(TRUE);
                            MMISET_SetSecurityCodeEnable(TRUE);
                        }
                        break;
                    case 1:
                        if(s_setting_privacy_mode)
                        {
                            MMISET_SetPrivacyEnable(FALSE);
                        }
                        else
                        {
                            MMISET_SetPrivacyEnable(TRUE);
                        }
                        break;
                    default:
                        break;
                }

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
#endif

#ifdef FOTA_SUPPORT_ABUP
void ShowUpdateInfo(FotaDisplayState state)
{
    MMI_STRING_T    text_s = {0};
    if(MMISET_PHONE_SETTING_FOTA_WIN_ID != MMK_GetFocusWinId())
    {
        return;
    }
    switch(state)
    {
        case FOTA_STATE_CV:            
            MMI_GetLabelTextByLang(TXT_SETTINGS_ABUP_FOTA_CHECK_VER, &text_s);
            break;
        case FOTA_STATE_DL:
            MMI_GetLabelTextByLang(TXT_SETTINGS_ABUP_FOTA_STATE_DL, &text_s);
            break;
        case FOTA_STATE_RD:
            MMI_GetLabelTextByLang(TXT_SETTINGS_ABUP_FOTA_STATE_RD, &text_s);
            break;
        case FOTA_STATE_RU:
            MMI_GetLabelTextByLang(TXT_SETTINGS_ABUP_FOTA_STATE_RU, &text_s);
            break;
        case FOTA_STATE_RG:
            MMI_GetLabelTextByLang(TXT_SETTINGS_ABUP_FOTA_FOTA_STATE_RG, &text_s);
            break;
        case FOTA_STATE_ERROR:
            MMI_GetLabelTextByLang(TXT_SETTINGS_ABUP_FOTA_FOTA_STATE_ERROR, &text_s);
            break;
        case FOTA_STATE_LAST_VER:
            MMI_GetLabelTextByLang(TXT_SETTINGS_ABUP_FOTA_FOTA_STATE_LAST_VER, &text_s);
            break;
        default:
            MMI_GetLabelTextByLang(TXT_NULL, &text_s);
            break;
    }
    GUITEXT_SetString(MMISET_SETTINGS_CHECK_UPDATE_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, TRUE);
    MMK_SendMsg(MMISET_SETTINGS_CHECK_UPDATE_CTRL_ID,MSG_CTL_PAINT,PNULL);
}
LOCAL MMI_RESULT_E CheckUpdateWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T           ctrl_id = MMISET_SETTINGS_CHECK_UPDATE_CTRL_ID;
    MMI_STRING_T    text_s = {0};
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMI_GetLabelTextByLang(TXT_SETTINGS_ABUP_FOTA_CHECK_VER, &text_s);            
            GUITEXT_SetString(MMISET_SETTINGS_CHECK_UPDATE_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, FALSE);
        	abup_get_new_version_ex();//check version for update
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
        case MSG_FULL_PAINT:       
            break;
         case MSG_KEYDOWN_OK:
         case MSG_CTL_OK:
         {     
             MMK_CloseWin(win_id);     
             break;
         }
         case MSG_KEYDOWN_CANCEL:
		 case MSG_CTL_CANCEL:
         {
             MMK_CloseWin(win_id);
             break;
         }
        case MSG_CLOSE_WINDOW:
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return (result);
}
#endif
/*Edit by script, ignore 16 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527

