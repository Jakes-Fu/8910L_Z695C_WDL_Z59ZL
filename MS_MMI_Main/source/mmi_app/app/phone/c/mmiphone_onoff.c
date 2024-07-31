/*****************************************************************************
** File Name:      mmiphone_onoff.c                                          *
** Author:                                                                   *
** Date:           12/2011                                                   *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe phone power on/off         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011        jian.ma          Create                                    *
******************************************************************************/

#define _MMIPHONE_ONOFF_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_phone_trc.h"
#include "mmiphone_onoff.h"
#include "mmiphone_internal.h"
#include "mmiphone_export.h"
#include "window_parse.h"
#include "guiblock.h"
#include "mmiidle_subwintab.h"
#include "mmiidle_export.h"
#include "mmi_id.h"
#ifdef TOUCH_PANEL_SUPPORT
#include "tp_export.h"
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
//#include "mmienvset.h"
#include "mmienvset_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmipub.h"
#include "mmisd_export.h"
#ifdef FLASH_SUPPORT
#include "mmiacc_id.h"
#endif
#include "guitext.h"   //2015.5.27
#include "ctrlrichtext_export.h"
#include "guirichtext.h"
#ifdef MSA_SUPPORT
#include "mmimsa_export.h"
#endif
#if defined(BROWSER_SUPPORT_WEBKIT) && ((!defined(WIN32)) || (defined(_MSC_VER) && (_MSC_VER >= 1400)))
#include "mmiwk.h" 
#endif
#include "variant_config_nv.h"
#include "mmi_menutable.h"
#include "autocit_lcd.h"
#ifdef SIMLOCK_PROTECT_DATA
#include "simlock.h"
#ifdef SIMLOCK_PROTECT_DATA_DEBUG_SCN
#include "nvitem.h"
#endif
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define ALM_3SEC_TIME                   10000
#ifndef  WIN32
#define PHONE_STARTUP_DURING_TIME           8000//12000    // 以毫秒为单位的值
#else
#define PHONE_STARTUP_DURING_TIME           3000//12000    // ò?oá???aμ￥??μ??μ
#endif
#define  PHONE_OPERATOR_PROMO_TIME           3000 
//#define PHONE_FIRST_ANIM_TIME               3000     
#define PHONE_POWER_OFF_DURING_TIME         8000//12000
#define  LANGUAGE_WELCOME_TEXT_RECT_LEFT    6
#define  LANGUAGE_WELCOME_TEXT_RECT_RIGHT   6
#define  LANGUAGE_WELCOME_TEXT_RECT_HIGHT   30
#define  LANGUAGE_THANK_TEXT_RECT_LEFT      6
#define  LANGUAGE_THANK_TEXT_RECT_RIGHT     6
#define  LANGUAGE_THANK_TEXT_RECT_TOP       36
#define  LANGUAGE_THANK_TEXT_RECT_HIGHT     54
#define  LANGUAGE_LIST_RECT_TOP             90
#define  SPINNING_DISPLAY_TIME   1000
#define  DISPLAY_OPERATOR_LOGO        1  //nv配置NV_VARIANT_CONFIG_OTHER = 1时只显示operator logo
#define  DISPLAY_OPERATOR_LOGO_RING    2 //nv配置NV_VARIANT_CONFIG_OTHER = 2时显示operator logo AND ring

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL PHONE_STARTUP_CONDITION_E   s_startup_condition;    // 记录开机的原因
//LOCAL BOOLEAN       s_is_play_default = TRUE;   //TRUE:play default animation,FALSE:play file
LOCAL BOOLEAN   s_is_play_power_on_mp4; //TRUE:power on mp4, FALSE: power off MP4
LOCAL BOOLEAN   s_is_playing_mp4 = FALSE; //TRUE:playing mp4 as poweron/off anim, FALSE: NONE
LOCAL uint8 s_3sec_timer_id = 0; 
LOCAL uint8 s_phone_startup_timer_id = 0;// 正常开机时开机界面的持续时间的ID号   
#if defined(MMI_POWERONOFFMP4_ENABLE)
LOCAL uint8 * s_poweronoff_buffer_ptr = PNULL;
#endif
//LOCAL uint8  s_first_anim_timer_id = 0;
//LOCAL uint8  s_power_off_anim_timer_id = 0; //1??ú?ˉ?-?¨ê±?÷
LOCAL uint8  s_operator_timer_id = 0;  
LOCAL uint8  s_spinning_diaplay_timer = 0;
BOOLEAN    s_fly_window_open_flag = FALSE;
BOOLEAN    s_first_language_select_flag = FALSE;
BOOLEAN    s_sim2_ok_remain_flag = FALSE;
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
#ifdef MULTI_SIM_GSM_CTA_SUPPORT
extern BOOLEAN g_is_sim_only_check_status[MMI_DUAL_SYS_MAX];
#endif
extern MMISET_PWD_OPERATE_TYPE_E g_mmiset_pwd_type;

#ifdef SIMLOCK_PROTECT_DATA_DEBUG_SCN
static simlock_protect_data_t   s_sim_protect_cur_data = {0};
static simlock_protect_data_t   s_sim_protect_flash_data = {0};
static uint8 s_protect_data_log[256] = {0};
#endif
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 处理正常开机界面的消息(除PS之外)
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNormalStartupWindow(
                                             MMI_WIN_ID_T    win_id,    // STARTUP NORMAL窗口的ID
                                             MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                             DPARAM             param       // 相应消息的参数
                                             );
/*****************************************************************************/
//  Description :  [MS]开机operator promo显示窗口处理函数
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOperatorPromoStartupWindow(
                                             MMI_WIN_ID_T    win_id,    // STARTUP NORMAL窗口的ID
                                             MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                             DPARAM             param       // 相应消息的参数
                                             );
/*****************************************************************************/
//  Description : [MS]开机operator logo显示窗口处理函数
//  Global resource dependence : 
//  Author: 
//  Note:zhenxing.chen
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStartupOperatorLogoWindow(
                                             MMI_WIN_ID_T    win_id,    // STARTUP NORMAL窗口的ID
                                             MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                             DPARAM             param       // 相应消息的参数
                                             );
/*****************************************************************************/
//  Description : 处理闹铃启动开机界面的消息(除PS之外)
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlarmStartupWindow (
                                             MMI_WIN_ID_T    win_id,    // STARTUP ALARM窗口的ID
                                             MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                             DPARAM             param       // 相应消息的参数
                                             );

/*****************************************************************************/
//  Description : 关机闹钟后，是否开机询问窗口处理函数
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStartOrNotWindow( 
                                          MMI_WIN_ID_T     win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description : play power on mp4
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PlayPowerOnOffMP4(BOOLEAN is_power_on);

#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : HandleVideoPowerOnOffMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void HandleVideoPowerOnOffMsg(VP_CALLBACK_T *msg_ptr);
#endif
#ifndef MMI_POWERONOFFMP4_ENABLE
/*****************************************************************************/
//  Description : PlayDefaultPowerOnOffAnimation
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN PlayDefaultPowerOnOffAnimation(BOOLEAN is_power_on_off);    //TRUE:power on, FALSE:power off

#if 0
/*****************************************************************************/
//  Description : PlayOperatorPowerOnOffAnimation
//  Global resource dependence : 
//  Author: Cheney.Wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN PlayOperatorPowerOnOffAnimation(BOOLEAN is_power_on_off);    //TRUE:power on, FALSE:power off
#endif

LOCAL BOOLEAN DisplaySecondAnim(
								 MMI_CTRL_ID_T  ctrl_id,
								 MMI_IMAGE_ID_T  image_id 
								 );
#endif
/*****************************************************************************/
//  Description : call back function of PlayPowerOnOffMP4
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
#ifdef VIDEO_PLAYER_SUPPORT
void PlayPowerOnOffMP4CallBack(VP_PLAY_END_RESULT_E result);
#else
void PlayPowerOnOffMP4CallBack(void);
#endif


/*****************************************************************/
//  Description :play more power on or off animation
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************/
LOCAL BOOLEAN PlayMorePowerOnOffAnimation(
                                          BOOLEAN is_power_on_off,  //TRUE:power on, FALSE:power off
                                          MMISET_POWER_ON_OFF_ANIM_INFO_T *power_on_off_anim_info
                                          );
/*****************************************************************/
//  Description :language select in first power on
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************/
LOCAL MMI_RESULT_E HandleLanguageFirstPowerOnWindow(
												    MMI_WIN_ID_T     win_id,    // STARTUP NORMAL′°?úμ?ID
												    MMI_MESSAGE_ID_E    msg_id,     // ′°?úμ??ú2????￠ID
													DPARAM              param       // ?àó|???￠μ?2?êy
												   );
/*****************************************************************/
//  Description :window for warranty in first power on
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************/
LOCAL MMI_RESULT_E HandleWarrantyFirstPowerOnWindow(
												    MMI_WIN_ID_T     win_id,    // STARTUP NORMAL′°?úμ?ID
												    MMI_MESSAGE_ID_E    msg_id,     // ′°?úμ??ú2????￠ID
													DPARAM              param       // ?àó|???￠μ?2?êy
												   );

/*****************************************************************/
//  Description :window for warranty in first power on
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************/
LOCAL MMI_RESULT_E HandleTransportFirstPowerOnWindow(
                                                    MMI_WIN_ID_T     win_id,    // STARTUP NORMAL′°?úμ?ID
                                                    MMI_MESSAGE_ID_E    msg_id,     // ′°?úμ??ú2????￠ID
                                                    DPARAM              param       // ?àó|???￠μ?2?êy
                                                   );

/*****************************************************************************/
//  Description :创建日期设置dialog窗口
//  Global resource dependence : 
//  Author:
//  Note:2015.6.20
/*****************************************************************************/
LOCAL void MMIPHONE_CreateDateTimeDialogWindow(void);

/*****************************************************************/
//  Description :window for startup spinning
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************/
LOCAL MMI_RESULT_E HandleStartupSpinningWindow(
												    MMI_WIN_ID_T     win_id,    // STARTUP NORMAL′°?úμ?ID
												    MMI_MESSAGE_ID_E    msg_id,     // ′°?úμ??ú2????￠ID
													DPARAM              param       // ?àó|???￠μ?2?êy
												   );

/*****************************************************************************/
//  Description : 处理关机界面
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerOffWindow(
                                        MMI_WIN_ID_T     win_id,    // STARTUP NORMAL窗口的ID
                                        MMI_MESSAGE_ID_E    msg_id,     // 窗口的内部消息ID
                                        DPARAM              param       // 相应消息的参数
                                        );

/*****************************************************************/
//  Description :window for warranty in first power on
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************/
LOCAL MMI_RESULT_E HandleWarrantyOptMenuWinMsg(
                                               MMI_WIN_ID_T     win_id,    // STARTUP NORMAL′°?úμ?ID
                                               MMI_MESSAGE_ID_E    msg_id,     // ′°?úμ??ú2????￠ID
                                               DPARAM              param       // ?àó|???￠μ?2?êy
                                               );

/*****************************************************************/
//  Description :window for warranty in first power on
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************/
LOCAL MMI_RESULT_E HandlePrivacyPolicyWindow(
                                             MMI_WIN_ID_T     win_id,    // STARTUP NORMAL′°?úμ?ID
                                             MMI_MESSAGE_ID_E    msg_id,     // ′°?úμ??ú2????￠ID
                                             DPARAM              param       // ?àó|???￠μ?2?êy
                                             );

// the window for startup normal

WINDOW_TABLE( PHONE_STARTUP_NORMAL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    WIN_PRIO( WIN_THREE_LEVEL ),
    WIN_FUNC( (uint32)HandleNormalStartupWindow ),    
    WIN_ID( PHONE_STARTUP_NORMAL_WIN_ID ),
    WIN_HIDE_STATUS,
    WIN_STYLE(WS_DISABLE_HWICON),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    END_WIN
};

// the window for operator promo [MS]
WINDOW_TABLE( PHONE_STARTUP_OPERATOR_PROMO_WIN_TAB ) = 
{
    WIN_PRIO( WIN_THREE_LEVEL ),
    WIN_FUNC( (uint32)HandleOperatorPromoStartupWindow ),    
    WIN_ID( PHONE_STARTUP_OPERATOR_WIN_ID),
    WIN_HIDE_STATUS,
	CREATE_TEXT_CTRL(PHONE_STARTUP_OPERATOR_PROMOL_TEXT_CTRL_ID),
    WIN_STYLE(WS_DISABLE_HWICON),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    END_WIN
};
//the window for operator logo[MS]
WINDOW_TABLE( PHONE_STARTUP_OPERATOR_LOGO_WIN_TAB) = 
{
	WIN_PRIO( WIN_THREE_LEVEL ),
    WIN_FUNC( (uint32)HandleStartupOperatorLogoWindow ),    
    WIN_ID( PHONE_STARTUP_OPERATOR_LOGO_WIN_ID),
    WIN_HIDE_STATUS,
	CREATE_ANIM_CTRL(PHONE_STARTUP_OPERATOR_LOGO_CTRL_ID, PHONE_STARTUP_OPERATOR_LOGO_WIN_ID),
    WIN_STYLE(WS_HAS_ANIM_BG),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    END_WIN
};
// the window for startup alarm
WINDOW_TABLE( PHONE_STARTUP_ALARM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleAlarmStartupWindow ),    
    WIN_ID( PHONE_STARTUP_ALARM_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};

// the window for power off
WINDOW_TABLE( PHONE_POWER_OFF_WIN_TAB ) = 
{
    //CLEAR_LCD,
    WIN_PRIO( WIN_THREE_LEVEL ),
    WIN_FUNC( (uint32)HandlePowerOffWindow ),
//    WIN_STYLE(WS_DISABLE_LCD_SWITCH),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_ID( PHONE_POWER_OFF_WIN_ID ),
    WIN_STYLE(WS_DISABLE_HWICON),
    WIN_HIDE_STATUS,
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    END_WIN
};
// the window for language select in first power on 
WINDOW_TABLE( PHONE_LANGUAGE_FIRST_POWER_WIN_TAB ) = 
{	
    WIN_PRIO( WIN_TWO_LEVEL ),
	WIN_FUNC( (uint32)HandleLanguageFirstPowerOnWindow ),    
	WIN_ID( PHONE_LANGUAGE_FIRST_POWER_WIN_ID ),
	WIN_STYLE(WS_DISABLE_RETURN_WIN),
	WIN_STATUSBAR,
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, PHONE_WELCOME_LABEL_CTRL_ID),
    CREATE_TEXT_CTRL(PHONE_THANKS_TEXTBOX_CTRL_ID),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, PHONE_LANGUAGE_LISTBOX_CTRL_ID),
	WIN_SOFTKEY(TXT_NULL,STXT_SOFTKEY_SELECT_MK,TXT_NULL), //TXT_CHOOSE_FOR_SOFTKEY
	END_WIN
};
// the window for warranty in first power on
WINDOW_TABLE( PHONE_WARRANTY_FIRST_POWER_WIN_TAB ) = 
{	
	//WIN_PRIO( WIN_LOWEST_LEVEL ),
	WIN_FUNC( (uint32)HandleWarrantyFirstPowerOnWindow ),    
	WIN_ID( PHONE_WARRANTY_FIRST_POWER_WIN_ID ),
	WIN_STYLE(WS_DISABLE_RETURN_WIN),
    WIN_TITLE( STR_START_WARR_HDR_EXT02 ),
	WIN_STATUSBAR,
    CREATE_RICHTEXT_CTRL(PHONE_WARRANTY_RICHTEXT_CTRL_ID),
	WIN_SOFTKEY(STXT_SOFTKEY_MENU_MK, STXT_SOFTKEY_CONTINUE_MK,TXT_NULL), //oóD?textDèòa???acontinue
	END_WIN
};

WINDOW_TABLE( PHONE_TRANSPORT_FIRST_POWER_WIN_TAB ) = 
{	
	WIN_PRIO( WIN_LOWEST_LEVEL ),
	WIN_FUNC( (uint32)HandleTransportFirstPowerOnWindow ),    
	WIN_ID( PHONE_TRANSPORT_FIRST_POWER_WIN_ID ),
	WIN_STYLE(WS_DISABLE_RETURN_WIN),
	END_WIN
};

// the window for fly query in startup
WINDOW_TABLE( PHONE_STARTUP_FLY_QUERY_WIN_TAB ) = 
{
    WIN_ID( PHONE_STARTUP_FLY_QUERY_WIN_ID ),
	WIN_STYLE(WS_DISABLE_RETURN_WIN),
    END_WIN
};
// the window for startup spinning
WINDOW_TABLE( PHONE_STARTUP_SPINNING_WIN_TAB) =
{
	WIN_FUNC((uint32)HandleStartupSpinningWindow),
	WIN_ID(PHONE_STARTUP_SPINNING_WIN_ID),
	WIN_HIDE_STATUS,
	WIN_STYLE(WS_DISABLE_RETURN_WIN),
	CREATE_SPINNER_CTRL(PHONE_STARTUP_SPINNING_CTRL_ID), 
	END_WIN
};
// the window for 
WINDOW_TABLE( PHONE_FIRST_POWER_DATE_SET_WIN_TAB) =
{
	WIN_ID(PHONE_FIRST_POWER_DATE_SET_WIN_ID),
	WIN_STYLE(WS_DISABLE_RETURN_WIN),
	END_WIN
};

WINDOW_TABLE( PHONE_FIRST_POWER_WARRANTY_OPT_MENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWarrantyOptMenuWinMsg),
        WIN_ID(PHONE_FIRST_POWER_WARRANTY_OPTMENU_WIN_ID),
        CREATE_POPMENU_CTRL(WARRANTY_OPT,PHONE_WARRANTY_RICHTEXT_CTRL_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        WIN_SOFTKEY(TXT_NULL, TXT_COMMON_CHOOSE, TXT_COMMON_COMMON_BACK),
        END_WIN
};

// the window for warranty in first power on
WINDOW_TABLE( PHONE_FIRST_POWER_PRIVACY_POLICY_WIN_TAB ) = 
{	
   // WIN_PRIO( WIN_LOWEST_LEVEL ),
        WIN_FUNC( (uint32)HandlePrivacyPolicyWindow ),    
        WIN_ID( PHONE_FIRST_POWER_PRIVACY_POLICY_WIN_ID ),
        WIN_STYLE(WS_DISABLE_RETURN_WIN),
        WIN_TITLE( STR_PRIVACY_HDR_WE_CARE_EXT02 ),
        WIN_STATUSBAR,
        CREATE_RICHTEXT_CTRL(PHONE_PRIVACY_POLICY_RICHTEXT_CTRL_ID),
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_COMMON_COMMON_BACK), //oóD?textDèòa???acontinue
        END_WIN
};

#ifdef SIMLOCK_PROTECT_DATA_DEBUG_SCN
/*****************************************************************************/
//  Description : get SIM lock check result, for whether disable SIM or BT.
//  Global resource dependence : none
//  Author: wubin.zou
//  Note:
/*****************************************************************************/
uint16 MMIPHONE_GetSIMLockProtectDataCheckItem(uint8 *info)
{
    SIMLOCK_ERROR_TYPE ret = SIMLOCK_SUCCESS;
    uint16 result = 0;
    uint8 temp_log[64] = {0};

    ret = Simlock_ReadProtectData(&s_sim_protect_flash_data);
    Simlock_GetCurProtectData(&s_sim_protect_cur_data);
#if 1
    if (SCI_MEMCMP(&(s_sim_protect_cur_data.bt_addr), &(s_sim_protect_flash_data.bt_addr), sizeof(s_sim_protect_flash_data.bt_addr)) != 0) {
        result |= SIMLOCK_ERR_RESTORE_BT;
    }
#endif
    if (SCI_MEMCMP(s_sim_protect_cur_data.IMEI1, s_sim_protect_flash_data.IMEI1, sizeof(s_sim_protect_flash_data.IMEI1)) != 0) {
        result |= SIMLOCK_ERR_RESTORE_IMEI1;
    }
    if (SCI_MEMCMP(s_sim_protect_cur_data.IMEI2, s_sim_protect_flash_data.IMEI2, sizeof(s_sim_protect_flash_data.IMEI2)) != 0) {
        result |= SIMLOCK_ERR_RESTORE_IMEI2;
    }
    if (s_sim_protect_cur_data.rf_crc != s_sim_protect_flash_data.rf_crc) {
        result |= SIMLOCK_ERR_RESTORE_RF;
    }

    SCI_MEMSET(s_protect_data_log, 0, sizeof(s_protect_data_log));
#if 1
    /* BT */
    SCI_MEMSET(temp_log, 0, sizeof(temp_log));
    sprintf(temp_log, "\nBT nv: %02x%02x %02x%02x %02x%02x\nflash: %02x%02x %02x%02x %02x%02x",
        s_sim_protect_cur_data.bt_addr.bd_addr.addr[0], s_sim_protect_cur_data.bt_addr.bd_addr.addr[1],
        s_sim_protect_cur_data.bt_addr.bd_addr.addr[2], s_sim_protect_cur_data.bt_addr.bd_addr.addr[3],
        s_sim_protect_cur_data.bt_addr.bd_addr.addr[4], s_sim_protect_cur_data.bt_addr.bd_addr.addr[5],
        s_sim_protect_flash_data.bt_addr.bd_addr.addr[0], s_sim_protect_flash_data.bt_addr.bd_addr.addr[1],
        s_sim_protect_flash_data.bt_addr.bd_addr.addr[2], s_sim_protect_flash_data.bt_addr.bd_addr.addr[3],
        s_sim_protect_flash_data.bt_addr.bd_addr.addr[4], s_sim_protect_flash_data.bt_addr.bd_addr.addr[5]);
    strcpy(s_protect_data_log, temp_log);
#endif
#if 1
    /* IMEI */
    SCI_MEMSET(temp_log, 0, sizeof(temp_log));
    sprintf(temp_log, "\nIMEI1: %02x%02x %02x%02x %02x%02x %02x%02x\nflash: %02x%02x %02x%02x %02x%02x %02x%02x",
        s_sim_protect_cur_data.IMEI1[0], s_sim_protect_cur_data.IMEI1[1],
        s_sim_protect_cur_data.IMEI1[2], s_sim_protect_cur_data.IMEI1[3],
        s_sim_protect_cur_data.IMEI1[4], s_sim_protect_cur_data.IMEI1[5],
        s_sim_protect_cur_data.IMEI1[6], s_sim_protect_cur_data.IMEI1[7],
        s_sim_protect_flash_data.IMEI1[0], s_sim_protect_flash_data.IMEI1[1],
        s_sim_protect_flash_data.IMEI1[2], s_sim_protect_flash_data.IMEI1[3],
        s_sim_protect_flash_data.IMEI1[4], s_sim_protect_flash_data.IMEI1[5],
        s_sim_protect_flash_data.IMEI1[6], s_sim_protect_flash_data.IMEI1[7]);
    strcat(s_protect_data_log, temp_log);

    /* IMEI2 */
    SCI_MEMSET(temp_log, 0, sizeof(temp_log));
    sprintf(temp_log, "\nIMEI2: %02x%02x %02x%02x %02x%02x %02x%02x\nflash: %02x%02x %02x%02x %02x%02x %02x%02x",
        s_sim_protect_cur_data.IMEI2[0], s_sim_protect_cur_data.IMEI2[1],
        s_sim_protect_cur_data.IMEI2[2], s_sim_protect_cur_data.IMEI2[3],
        s_sim_protect_cur_data.IMEI2[4], s_sim_protect_cur_data.IMEI2[5],
        s_sim_protect_cur_data.IMEI2[6], s_sim_protect_cur_data.IMEI2[7],
        s_sim_protect_flash_data.IMEI2[0], s_sim_protect_flash_data.IMEI2[1],
        s_sim_protect_flash_data.IMEI2[2], s_sim_protect_flash_data.IMEI2[3],
        s_sim_protect_flash_data.IMEI2[4], s_sim_protect_flash_data.IMEI2[5],
        s_sim_protect_flash_data.IMEI2[6], s_sim_protect_flash_data.IMEI2[7]);
    strcat(s_protect_data_log, temp_log);
#endif
#if 1
    /* RF */
    SCI_MEMSET(temp_log, 0, sizeof(temp_log));
    //sprintf(temp_log, "\nRF CRC:%x\nflash: %x",
    //    s_sim_protect_cur_data.rf_crc,
    //    s_sim_protect_flash_data.rf_crc);
    sprintf(temp_log, "\nRF CRC:%04x %02x%02x %02x%02x %02x%02x %02x%02x\nflash: %04x %02x%02x %02x%02x %02x%02x %02x%02x",
        s_sim_protect_cur_data.rf_crc,
        s_sim_protect_cur_data.rf_debug[0], s_sim_protect_cur_data.rf_debug[1],
        s_sim_protect_cur_data.rf_debug[2], s_sim_protect_cur_data.rf_debug[3],
        s_sim_protect_cur_data.rf_debug[4], s_sim_protect_cur_data.rf_debug[5],
        s_sim_protect_cur_data.rf_debug[6], s_sim_protect_cur_data.rf_debug[7],
        s_sim_protect_flash_data.rf_crc,
        s_sim_protect_flash_data.rf_debug[0], s_sim_protect_flash_data.rf_debug[1],
        s_sim_protect_flash_data.rf_debug[2], s_sim_protect_flash_data.rf_debug[3],
        s_sim_protect_flash_data.rf_debug[4], s_sim_protect_flash_data.rf_debug[5],
        s_sim_protect_flash_data.rf_debug[6], s_sim_protect_flash_data.rf_debug[7]);
    strcat(s_protect_data_log, temp_log);
#endif
#if 0
    /* UID */
    {
        uint8 uid[32] = {0};
        SCI_MEMSET(temp_log, 0, sizeof(temp_log));
        Simlock_TestUid(uid);
        sprintf(temp_log, "\nUID: %x%x %x%x %x%x %x%x %x%x %x%x %x%x %x%x",
            uid[0], uid[1],  uid[2],  uid[3],  uid[4],  uid[5],  uid[6],  uid[7],
            uid[8], uid[9], uid[10], uid[11], uid[12], uid[13], uid[14], uid[15]);
        strcat(s_protect_data_log, temp_log);
        SCI_MEMSET(temp_log, 0, sizeof(temp_log));
        sprintf(temp_log, "\n   : %x%x %x%x %x%x %x%x %x%x %x%x %x%x %x%x",
            uid[16], uid[17], uid[18], uid[19], uid[20], uid[21], uid[22], uid[23],
            uid[24], uid[25], uid[26], uid[27], uid[28], uid[29], uid[30], uid[31]);
        strcat(s_protect_data_log, temp_log);
    }
#endif
#if 0
    /* SHA1 use 20 bytes, SHA2 use 32 bytes */
    {
        SCI_MEMSET(temp_log, 0, sizeof(temp_log));
        sprintf(temp_log, "\nSHA: %x%x %x%x %x%x %x%x %x%x %x%x %x%x %x%x %x%x %x%x",
            s_sim_protect_cur_data.debug[0],  s_sim_protect_cur_data.debug[1],  s_sim_protect_cur_data.debug[2],  s_sim_protect_cur_data.debug[3],
            s_sim_protect_cur_data.debug[4],  s_sim_protect_cur_data.debug[5],  s_sim_protect_cur_data.debug[6],  s_sim_protect_cur_data.debug[7],
            s_sim_protect_cur_data.debug[8],  s_sim_protect_cur_data.debug[9],  s_sim_protect_cur_data.debug[10], s_sim_protect_cur_data.debug[11],
            s_sim_protect_cur_data.debug[12], s_sim_protect_cur_data.debug[13], s_sim_protect_cur_data.debug[14], s_sim_protect_cur_data.debug[15],
            s_sim_protect_cur_data.debug[16], s_sim_protect_cur_data.debug[17], s_sim_protect_cur_data.debug[18], s_sim_protect_cur_data.debug[19]);
        strcat(s_protect_data_log, temp_log);

        SCI_MEMSET(temp_log, 0, sizeof(temp_log));
        sprintf(temp_log, "\nFlash: %x%x %x%x %x%x %x%x %x%x %x%x %x%x %x%x %x%x %x%x",
            s_sim_protect_flash_data.debug[0],  s_sim_protect_flash_data.debug[1],  s_sim_protect_flash_data.debug[2],  s_sim_protect_flash_data.debug[3],
            s_sim_protect_flash_data.debug[4],  s_sim_protect_flash_data.debug[5],  s_sim_protect_flash_data.debug[6],  s_sim_protect_flash_data.debug[7],
            s_sim_protect_flash_data.debug[8],  s_sim_protect_flash_data.debug[9],  s_sim_protect_flash_data.debug[10], s_sim_protect_flash_data.debug[11],
            s_sim_protect_flash_data.debug[12], s_sim_protect_flash_data.debug[13], s_sim_protect_flash_data.debug[14], s_sim_protect_flash_data.debug[15],
            s_sim_protect_flash_data.debug[16], s_sim_protect_flash_data.debug[17], s_sim_protect_flash_data.debug[18], s_sim_protect_flash_data.debug[19]);
        strcat(s_protect_data_log, temp_log);
    }
#endif
#if 0
    SCI_MEMSET(temp_log, 0, sizeof(temp_log));
    SCI_MEMSET(&s_sim_lock_customize_data, 0x0, sizeof(MN_SIM_LOCK_CUSTOMIZE_DATA_T));
    ret = Simlock_ReadCustomiseData(&s_sim_lock_customize_data);
    sprintf(temp_log, "\nlos:%x \nnwlo:%x",
        s_sim_lock_customize_data.SIM_lock_status, s_sim_lock_customize_data.network_locks);
    strcat(s_protect_data_log, temp_log);
#endif

    strcpy(info, s_protect_data_log);
    return result;
}

/*****************************************************************************/
//  Description : get SIM lock protect error info.
//  Global resource dependence : none
//  Author: wubin.zou
//  Note:
/*****************************************************************************/
void MMIPHONE_GetSIMLockProtectDataErrInfo(uint32 *info)
{
    Simlock_GetProtectDataErrInfo(info);
}
#endif

/*****************************************************************************/
//  Description : 闹铃启动开机界面的初始化
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMIAPIPHONE_AlarmInit(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    // 记录开机方式
    s_startup_condition = STARTUP_ALARM;
    MMIDEFAULT_InitDefault(FALSE); //关机闹钟响，lcd背光默认是关闭的
    
    if(!MMIALMSVC_StartService()/*MMIAPIALM_StartAlarm(FALSE)*/)
    {
        //关机
        MMIDEFAULT_TurnOffBackLight();
        POWER_PowerOff();
    }
#ifndef SUBLCD_SIZE_NONE
    else
    {
        //设置小屏显示是关机状态，刷新字屏显示关机充电
        MMISUB_SetMsStatus(FALSE);
        MMISUB_SetSubLcdDisplay(TRUE,TRUE,SUB_CONTENT_ALARM,PNULL);
        MMISUB_IsPermitUpdate(TRUE);
    }
#endif
}


/*****************************************************************************/
//  Description : ASSERT启动的初始化,直接进入idle界面
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMIAPIPHONE_AssertInit(void)
{
    // 记录开机方式,虽然是ASSERT开机，但是除开机流程不一样之外，其余都一样的
    s_startup_condition = STARTUP_NORMAL;//STARTUP_ASSERT;
#ifndef SUBLCD_SIZE_NONE
    MMISUB_IsPermitUpdate(TRUE);
#endif
    // 进入idle界面
    MMIAPIIDLE_OpenIdleWin();

    MMIDEFAULT_AllowTurnOffBackLight(TRUE);
}


/*****************************************************************************/
//     Description : 正常开机校准后界面的初始化
//    Global resource dependence : 
//  Author: Robert
//    Note:
/*****************************************************************************/
void MMIAPIPHONE_NormalInitAfterCoordinate(void)
{    

    MMIAPIPHONE_NormalInitAfterPwd();
    if(MMIAPISET_IsPowerOnSecruityCodeEnable()){
		MMIAPISET_OpenPowerOnPwdWin();
	}
 /*   else
    {
        if (MMIAPISET_GetFlyModeSetPowerOn())
        {
            MMIAPISET_SetFlyModeOpenPhone();
        }
        else
        {
            MMIAPIPHONE_NormalInitAfterPwd();
        }
    }*/
}

/*****************************************************************************/
//  Description : init a virtual window
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_VirtualWinInit(void)
{
    MMI_APPLET_START_T start ={0};
    
    start.guid = SPRD_IDLE_APPLET_ID;
    
    MMK_StartApplet( &start );
}

/*****************************************************************************/
//  Description : create operator logo window
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_CreateOperatorLogoWindow(void)
{
    MMK_CreateWin((uint32*)PHONE_STARTUP_OPERATOR_LOGO_WIN_TAB, PNULL);//operator window
}

/*****************************************************************************/
//  Description : 正常开机界面的初始化
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMIAPIPHONE_NormalInit(void)
{
#ifdef SIMLOCK_PROTECT_DATA_DEBUG_SCN
	uint16 protect_chk = 0;
	uint32 err_val = 0;
#endif
	// 记录开机方式
	s_startup_condition = STARTUP_NORMAL;
#ifndef SUBLCD_SIZE_NONE    
	//子屏开机图画,不允许刷新   SUBLCD刷新必须在初始化之后
	MMISUB_DisplayIdlePic();
	MMISUB_IsPermitUpdate(TRUE);
#endif    
#ifdef MMI_SIM_LOCK_SUPPORT
	if(!MNPHONE_SIMLockDataIntegrityCheck())
	{
#ifdef SIMLOCK_PROTECT_DATA_DEBUG_SCN
		uint8 err_log[256] = {0};
		uint8 val_log[64] = {0};

		protect_chk = MMIPHONE_GetSIMLockProtectDataCheckItem(err_log);
		MMIPHONE_GetSIMLockProtectDataErrInfo(&err_val);
		sprintf(val_log, "\nVarify err: %x", err_val);
		strcat(err_log, val_log);
		if ((err_val & (!SIMLOCK_ERR_SIGN_VERI)) != 0)  // for example, get UID fail, or read key fail
		{
			MMIPHONE_SIMLockedDisableNone(err_log);
			return;
		}
		SCI_MEMSET(val_log, 0, 64);
		sprintf(val_log, "\nprotect chk: %x", protect_chk);
		strcat(err_log, val_log);
		if ((SIMLOCK_ERR_RESTORE_SIM & protect_chk) != 0) // or no simlock data or default
		{
			MMIPHONE_SIMLockedDisableSIM(err_log);
			return;
		}
		if ((SIMLOCK_ERR_RESTORE_BT & protect_chk) == SIMLOCK_ERR_RESTORE_BT)   // not else if
		{
			MMIPHONE_SIMLockedDisableBT(err_log);
			return;
		}
		MMIPHONE_SIMLockedDisableNone(err_log);
		return;
#else
		/*sim lock data error,the ME alert "sim Locked" then power off*/
		MMIPHONE_SIMLockedOpenShutOffWin();
		//return;
#endif
	}
	else
	{
	#ifdef SIMLOCK_PROTECT_DATA_DEBUG_SCN
		uint8 err_log[256] = {0};
		protect_chk = MMIPHONE_GetSIMLockProtectDataCheckItem(err_log);
		MMIPHONE_SIMLockedOK(err_log);
		return;
	#else
		MN_SIM_LOCK_STATUS_T* sim_unlock_data = MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
		
		/*初始化SIM Lock Data*/
		SCI_MEMSET(sim_unlock_data,0x00,sizeof(MN_SIM_LOCK_STATUS_T));
		MMIAPIPHONE_UpdateSIMUnlockData();
	#endif
	}
#endif
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && defined(MMI_SIM_LOCK_TYPE2_UNLOCK_SUPPORT)
    MMIAPIPHONE_CheckSimLockStatus();
#endif
#ifndef SIMLOCK_PROTECT_DATA_DEBUG_SCN
#if defined TOUCH_PANEL_SUPPORT && !defined TOUCHPANEL_TYPE_MULTITP
	// 判定是否第一次开机， 是打开校准窗口
	if (MMIAPITP_IsPowerOnFirst() && !POWER_IsAutoTestMode())
	{
		MMIAPITP_COORDINATE_OpenWin(TRUE);
	}
	else
#endif
	{        		 
		//dong.chunguang move phone lock before bootanim
#ifndef WIN32
        if(RM_GetCalibrationPostMode())
        {
            //MMIAPIPHONE_NormalInitAfterPwd();
            MMIAPIPHONE_StartUpPsAccordingToSetting(TRUE, TRUE);
            AUTOCIT_OpenMainWin();
            return;
        }
#endif
		if (MMIAPISET_GetPowerOnPwdStatus())
		{
			g_mmiset_pwd_type = MMISET_PWD_NORMAL;
			MMIAPISET_OpenPhoneLockPwdWin();
			// MMIAPISET_OpenPowerOnPwdWin();
		}
		else
		{
			if (MMIAPISET_GetFlyModeSetPowerOn() || MMIAPISET_GetFlightMode())
			{
				// MMIAPISET_SetFlyModeOpenPhone();
				MMIPHONE_CreateFlyQueryDialogWindow();
			}
			else // if (FALSE == MMIAPISET_IsPowerOnSecruityCodeEnable())
			{
				//在既没有飞行模式,也没有security code
				MMIAPIPHONE_NormalInitAfterPwd();
			}
#ifdef FOTA_SUPPORT
#ifdef MMI_UPDATE_TIME_SUPPORT
			if (!MMIAPISET_GetIsAutoUpdateTime())
#endif
			{
				MMIFOTA_Start();
			}
#endif
		}
	}
#endif
}

/*****************************************************************************/
//  Description : 正常开机输入开机密码后界面的初始化
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMIAPIPHONE_NormalInitAfterPwd(void)
{    
#ifdef MULTI_SIM_GSM_CTA_SUPPORT
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        g_is_sim_only_check_status[i] = TRUE;
    }
#ifndef SUBLCD_SIZE_NONE
    //子屏开机图画,不允许刷新
    MMISUB_DisplayIdlePic();
    MMISUB_IsPermitUpdate(TRUE);
#endif
    MMIAPIPHONE_StartUpPsAccordingToSetting(TRUE, FALSE);
#else
	MMIAPIPHONE_StartUpPsAccordingToSetting(TRUE, TRUE);
#ifndef SUBLCD_SIZE_NONE
    //子屏开机图画,不允许刷新
    MMISUB_DisplayIdlePic();
    MMISUB_IsPermitUpdate(TRUE);
#endif    
	MMIAPIPHONE_OpenNormalStartUpWin();
#endif
}


/*****************************************************************************/
//  Description : get phone start up condition
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:  
/*****************************************************************************/
PUBLIC PHONE_STARTUP_CONDITION_E MMIAPIPHONE_GetStartUpCondition(void)
{
    return s_startup_condition;
}

/*****************************************************************************/
//  Description : get operator logo and ring display or not
//  Global resource dependence : 
//  Author: 
//  Note:  
/*****************************************************************************/
PUBLIC char MMIAPIPHONE_IsOperatorLogoRingDisplay(void)
{
	char     operator_mode  =  0;
	uint16    nv_len = 0;
	nv_len = NVITEM_GetLength(OTHER_NV_OPERATOR_LOGO);
	EFS_NvitemRead(OTHER_NV_OPERATOR_LOGO, nv_len, &operator_mode);
#ifdef WIN32
//	operator_mode = DISPLAY_OPERATOR_LOGO;
#endif
	SCI_TRACE_LOW("[Power on] MMIAPIPHONE_IsOperatorLogoRingDisplay operator_mode=%d", (uint8)operator_mode);
	if(operator_mode == DISPLAY_OPERATOR_LOGO)
	{
		SCI_TRACE_LOW("[Power on] MMIAPIPHONE_IsOperatorLogoRingDisplay DISPLAY_OPERATOR_LOGO");
	}
	else if(operator_mode == DISPLAY_OPERATOR_LOGO_RING)
	{
        SCI_TRACE_LOW("[Power on] MMIAPIPHONE_IsOperatorLogoRingDisplay DISPLAY_OPERATOR_LOGO_RING");
	}
    return  operator_mode;
}
/*****************************************************************************/
//  Description : set phone start up condition
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetStartUpCondition(PHONE_STARTUP_CONDITION_E e_start_cond)
{
    if(e_start_cond >= STARTUP_MAX)
    {
        //SCI_TRACE_LOW:"[POWERONOFF]:MMIAPIPHONE_IsNetworkSelecting e_start_cond = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_ONOFF_381_112_2_18_2_45_25_223,(uint8*)"d",e_start_cond);
        return ;
    }
    
    s_startup_condition = e_start_cond;
}

/*****************************************************************************/
//  Description : 创建首次开机语言选择窗口
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
PUBLIC void MMIAPIHONE_CreateFirstLanguageSelectWindow(void)
{
	MMK_CreateWin((uint32*)PHONE_LANGUAGE_FIRST_POWER_WIN_TAB, NULL); //创建首次开机语言选择窗口
	s_first_language_select_flag = TRUE;
}

/*****************************************************************************/
//  Description : create Microsoft Account Creation screen
//  Global resource dependence : 
//  Author:zhenxing
//  Note:  
/*****************************************************************************/
PUBLIC void MMIAPIHONE_CreateMicrosoftAccoutWindow(void)
{
#ifdef MMI_SHOW_MSA
#if defined(BROWSER_SUPPORT_WEBKIT) && ((!defined(WIN32)) || (defined(_MSC_VER ) && (_MSC_VER >= 1400)))
      MMIAPIMSA_get_token(TRUE , MSA_APP_TYPE_CAMERA_ROLL , MMIAPIPHONE_MSA_Get_Token_Func);
#else 
      MMK_CreateWin((uint32*)PHONE_STARTUP_SPINNING_WIN_TAB, PNULL);
#endif

#else

      MMK_CreateWin((uint32*)PHONE_STARTUP_SPINNING_WIN_TAB, PNULL);

#endif
}

/*****************************************************************************/
//  Description : select moble data sim
//  Global resource dependence : 
//  Author:zhenxing
//  Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_OpenSelectMobleDataSim(void)
{
#ifdef MMI_MULTI_SIM_SYS_SINGLE
#ifdef MMI_SHOW_MSA
     MMIAPIHONE_CreateMicrosoftAccoutWindow();
#endif
#else
#ifdef UNISOC_VERSION
     if(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)&&MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))//dual sim existed,open the sim select window
      {//SETTING 接口，用户选择数据连接sim
          MMISmartDS_PreferredSimWin(TRUE); 
          SCI_TRACE_LOW("[First Startup] MMIAPIPHONE_OpenSelectMobleDataSim DUAL SIM EXISTED");
      }
     else
#endif 
      {
#ifdef MMI_SHOW_MSA
          MMIAPIHONE_CreateMicrosoftAccoutWindow();
#endif
      }
#endif
     SCI_TRACE_LOW("[First Startup] MMIAPIPHONE_OpenSelectMobleDataSim success");
}

/*****************************************************************************/
//  Description : restore factory setting
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SettingfactoryForStartup(void)
{
	BOOLEAN                 is_first_power_on = TRUE;
	MMINV_WRITE(MMINV_PHONE_FIRST_POWER_ON, &is_first_power_on);
	SCI_TRACE_LOW("[First Startup] MMIAPIPHONE_SettingfactoryForStartup setting startup success");
}
/*****************************************************************************/
//  Description : 判断是否首次开机
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
PUBLIC void CreateLanguageSelectFirstPowerOn(void)
{ 
    BOOLEAN                 is_first_power_on = FALSE;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    MMINV_READ(MMINV_PHONE_FIRST_POWER_ON, &is_first_power_on, return_value);//读NV
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_first_power_on = TRUE;
        MMINV_WRITE(MMINV_PHONE_FIRST_POWER_ON, &is_first_power_on);
    }
    else if( is_first_power_on == TRUE )
    {        
//        is_first_power_on = FALSE;
//        MMINV_WRITE(MMINV_PHONE_FIRST_POWER_ON, &is_first_power_on);  
        MMIAPIHONE_CreateFirstLanguageSelectWindow();
    }
}

/*****************************************************************************/
//  Description : 首次开机语言选择窗口处理函数
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
LOCAL void MMIPHONE_SetLanguageWindowCtrl(GUI_RECT_T   bg_rect)
{
    MMI_CTRL_ID_T  welcome_label_id = PHONE_WELCOME_LABEL_CTRL_ID;
	MMI_CTRL_ID_T  thanks_textbox_id = PHONE_THANKS_TEXTBOX_CTRL_ID;
	GUI_RECT_T   welcome_text_rect = {0};
	GUI_RECT_T   thanks_text_rect = {0};
	GUI_RECT_T   language_list_rect = {0}; 
	MMI_STRING_T  thanks_string = {0};
	GUI_BG_T  	thanks_text_backgrand = {0};
	GUI_FONT_T   thanks_text_font = MMI_DEFAULT_NORMAL_FONT_SIZE;  //TEXT文字大小medium:16Px regular MMI_DEFAULT_NORMAL_FONT_SIZE
	GUI_COLOR_T   thanks_text_color = MMIAPI_GetOpactityColorValue(60, MMI_WHITE_COLOR, MMI_BLACK_COLOR);//MMI_WHITE_COLOR; //text??×???é?
    welcome_text_rect = bg_rect;  //
	welcome_text_rect.left = welcome_text_rect.left + LANGUAGE_WELCOME_TEXT_RECT_LEFT;// 6
	welcome_text_rect.right = welcome_text_rect.right - LANGUAGE_WELCOME_TEXT_RECT_RIGHT;// 6
	welcome_text_rect.bottom = welcome_text_rect.top + LANGUAGE_WELCOME_TEXT_RECT_HIGHT; // 30
	thanks_text_rect = bg_rect;	 
	thanks_text_rect.left = thanks_text_rect.left + LANGUAGE_THANK_TEXT_RECT_LEFT; //  6
	thanks_text_rect.right = thanks_text_rect.right - LANGUAGE_THANK_TEXT_RECT_RIGHT;//   6
	thanks_text_rect.top = thanks_text_rect.top + LANGUAGE_THANK_TEXT_RECT_TOP;  //  36
	thanks_text_rect.bottom = thanks_text_rect.top + LANGUAGE_THANK_TEXT_RECT_HIGHT;// 54
	MMI_GetLabelTextByLang(STR_START_LANGUAGE_QUERY_EXT02, &thanks_string);
	thanks_text_backgrand.bg_type = GUI_BG_COLOR; //éè??±3?°ààDíê???é?
	thanks_text_backgrand.color = MMI_BLACK_COLOR;
	language_list_rect = bg_rect;
	language_list_rect.top = language_list_rect.top + LANGUAGE_LIST_RECT_TOP;//  90
	GUILABEL_SetFont(welcome_label_id, MMI_DEFAULT_EXTRA_BIG_FONT_SIZE, MMI_WHITE_COLOR);//font:extra large:24Px regular
	GUILABEL_SetBackgroundColor(welcome_label_id, MMI_BLACK_COLOR);
    GUILABEL_SetRect(welcome_label_id, &welcome_text_rect, FALSE);
	GUILABEL_SetTextById(welcome_label_id, STR_START_WELCOME_HDR_EXT02, FALSE); 
	GUITEXT_SetMargin(thanks_textbox_id, 0x0);
	GUITEXT_SetRect(thanks_textbox_id, &thanks_text_rect);
	GUITEXT_SetBg(thanks_textbox_id, &thanks_text_backgrand);
	GUITEXT_SetFont(thanks_textbox_id, &thanks_text_font, &thanks_text_color);
	GUITEXT_SetString(thanks_textbox_id, thanks_string.wstr_ptr, thanks_string.wstr_len, FALSE);
	GUILIST_SetMaxItem(PHONE_LANGUAGE_LISTBOX_CTRL_ID,MMISET_GetValidLanguageNum(), FALSE );
	GUILIST_SetRect(PHONE_LANGUAGE_LISTBOX_CTRL_ID, &language_list_rect);
	GUILIST_SetBgColor(PHONE_LANGUAGE_LISTBOX_CTRL_ID, MMI_BLACK_COLOR);
	GUILIST_SetTextFont(PHONE_LANGUAGE_LISTBOX_CTRL_ID, CAF_FONT_20, MMI_WHITE_COLOR);
	GUILIST_SetNeedSplidLine(PHONE_LANGUAGE_LISTBOX_CTRL_ID, FALSE);//2?éè????è?listóD??????
	GUILIST_SetOwnSofterKey(PHONE_LANGUAGE_LISTBOX_CTRL_ID, TRUE);
}

/*****************************************************************************/
//  Description : 首次开机语言选择窗口处理函数
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLanguageFirstPowerOnWindow(
                                                    MMI_WIN_ID_T     win_id,    // STARTUP NORMAL′°?úμ?ID
                                                    MMI_MESSAGE_ID_E    msg_id,     // ′°?úμ??ú2????￠ID
                                                    DPARAM              param       // ?àó|???￠μ?2?êy
                                                    )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   bg_rect = {0}; 
    GUI_BOTH_RECT_T both_client_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMISET_LANGUAGE_TYPE_E  prefer_language =   0;
    uint32   sys_sim  =  MN_DUAL_SYS_1;
    BOOLEAN    sim_existed_flag  =  FALSE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(MMITHEME_IsMainScreenLandscape())
        {
            bg_rect = both_client_rect.h_rect;
        }
        else
        {
            bg_rect = both_client_rect.v_rect;
        }
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_WITHOUT_HEADER, TRUE);
        MMISET_GetLanguageFirstPowerOn(&prefer_language);  //??μ???è?????  
        MMIAPISET_SetLanguageType(prefer_language);//???μí3??ê?ó?????DDéè??
        MMIPHONE_SetLanguageWindowCtrl(bg_rect);
        MMISET_AppendLanguageFirstPowerOnListItem(PHONE_LANGUAGE_LISTBOX_CTRL_ID, prefer_language);
        MMK_SetAtvCtrl(win_id,PHONE_LANGUAGE_LISTBOX_CTRL_ID);
        break;
    case MSG_FULL_PAINT:
        if(MMITHEME_IsMainScreenLandscape())
        {
            bg_rect = both_client_rect.h_rect;
        }
        else
        {
            bg_rect = both_client_rect.v_rect;
        }
        GUI_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR); //ó?LCD_FillRectê?ò??ùμ?￡?GUI??°üá?ò?2?
        break;  
    case MSG_GET_FOCUS:
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_WITHOUT_HEADER, TRUE);
        MMIAPICOM_StatusAreaIdlescreenSet();
        break;
    case MSG_LOSE_FOCUS:
        break;
//  case MSG_APP_OK:
    case MSG_APP_WEB:
//  case MSG_CTL_OK:
    case MSG_NOTIFY_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif 
        MMISET_SetSelectLanguageFirstPowerOn(PHONE_LANGUAGE_LISTBOX_CTRL_ID);
        MMK_CloseWin( win_id );
        break;
    case MSG_NOTIFY_LIST_SELECT_QUERY:
        {
            GUILIST_QRERY_SELECT_ITEM_T * mark_item_ptr = (GUILIST_QRERY_SELECT_ITEM_T *)param;
            mark_item_ptr->is_check_select = TRUE;
        }
        break;
    case MSG_CLOSE_WINDOW:

//        MMK_CreateWin((uint32*)PHONE_WARRANTY_FIRST_POWER_WIN_TAB, FALSE);//′′?¨·t??ì???′°?ú
        for(sys_sim=MN_DUAL_SYS_1; sys_sim<MMI_DUAL_SYS_MAX; sys_sim++)
        {
            SCI_TRACE_LOW("[startup date and time] HandleLanguageFirstPowerOnWindow sim_sys = %d", sys_sim);
            if(MMIAPIPHONE_GetSimExistedStatus(sys_sim))//óD?¨2?è?
            {               
                SCI_TRACE_LOW("[startup date and time] HandleLanguageFirstPowerOnWindow sim_sys_in = %d", sys_sim);
                sim_existed_flag = TRUE;
            }
        }
        
        if(sim_existed_flag == TRUE)
        {
            SCI_TRACE_LOW("[startup date and time] HandleLanguageFirstPowerOnWindow  sim existed");
#ifdef MMI_MULTI_SIM_SYS_SINGLE
            if((SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1))
                && (1 == MMIAPISET_GetVodafoneSimConfig()))
            {
                MMIPHONE_CreateStartupVodafoneSelectWindow(); 
            }
#else  
            if((SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1))
                && (1 == MMIAPISET_GetVodafoneSimConfig()))
            {
                MMIPHONE_CreateStartupVodafoneSelectWindow(); 
            }
            if((SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_2))
                && (1 == MMIAPISET_GetVodafoneSimConfig()))
            {
                s_sim2_ok_remain_flag = TRUE;
            }
#endif
            if(FALSE == MMIAPISET_GetIsAutoUpdateTime())
            {
                SCI_TRACE_LOW("[startup date and time] HandleLanguageFirstPowerOnWindow atuo update time close");
                MMIPHONE_CreateDateTimeDialogWindow(); 
                MMIAPIPHONE_SetFirstPowerOnNV();
            }
            else
            {
#ifdef MMI_ORTHODOX_VERSION
                MMIPHONE_CreateDateTimeDialogWindow(); 
                MMIAPIPHONE_SetFirstPowerOnNV();
#else
                if(MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID) 
                    || MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID)
                    || MMK_IsOpenWin(MMI_VODAFONE_SELECT_WIN_ID))
                {
                    MMK_CreateWin((uint32*)PHONE_TRANSPORT_FIRST_POWER_WIN_TAB, FALSE);
                }
                else
                {
                    MMIAPIPHONE_OpenSelectMobleDataSim();
                }                          
                MMIAPIPHONE_SetFirstPowerOnNV();
#endif
            }
        }
        else
        {
            SCI_TRACE_LOW("[startup date and time] HandleLanguageFirstPowerOnWindow no_sim");
            MMIPHONE_CreateDateTimeDialogWindow(); 
            MMIAPIPHONE_SetFirstPowerOnNV();
        }
        break;
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
    case MSG_APP_OK:
    case MSG_APP_RED:
    case MSG_KEYUP_RED:
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}


/*****************************************************************************/
//  Description :显示服务条款界面text
//  Global resource dependence : 
//  Author:
//  Note:2015.6.20
/*****************************************************************************/
LOCAL void DisplayWarrantyScreenText(MMI_CTRL_ID_T  ctrl_id)
{
    GUIRICHTEXT_ITEM_T   item_data = {0};
    uint16        index  =  0;
//    GUI_RECT_T    warranty_text_rect = {0};
//    GUI_BG_T      warranty_text_backgrand = {0};
//    GUI_FONT_T    warranty_text_font = CAF_FONT_14;  //TEXT??×?′óD?
    GUI_COLOR_T   warranty_text_color = MMI_WHITE_COLOR; //text??×???é?
    wchar         *warranty_text_buffer_ptr = PNULL;
    MMI_STRING_T  warranty_string1 = {0};
    MMI_STRING_T  warranty_string2 = {0};
    MMI_STRING_T  warranty_string3 = {0};
    MMI_STRING_T  warranty_string4 = {0};

    MMI_STRING_T    string = {0};
    MMI_STRING_T    			result_string = {0};
    wchar 						result_wchar[200] = {0};
    MMI_TEXT_ID_T label_1=STR_PRIVACY_POLICY_1_HM;
    MMI_TEXT_ID_T label_2=STR_PRIVACY_POLICY_2_HM;
    MMI_TEXT_ID_T label_3=STR_PRIVACY_POLICY_3_HM;
    MMI_TEXT_ID_T label_4=STR_PRIVACY_POLICY_4_HM;
	
    result_string.wstr_ptr 	= result_wchar;
    
    item_data.tag_type = GUIRICHTEXT_TAG_NORMAL;
    CTRLRICHTEXT_SetBorderFocusColor(ctrl_id, MMI_BLACK_COLOR);
    GUIRICHTEXT_SetFontColor(ctrl_id, warranty_text_color);
    
    MMI_GetLabelTextByLang(STR_CHERRY_HELLO_MT_MSG_RM, &string);

    MMIAPICOM_CustomGenerateReplaceStr(&string, L"%U", L"UIS8910 4G", 200, &result_string);
    label_1=STR_START_WELCOME_HDR_EXT02;
    label_2=STXT_SOFTKEY_OK_MK;
    label_3=STXT_SOFTKEY_OK_MK;
    label_4=STXT_SOFTKEY_OK_MK;	

    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.text_set_font =TRUE;
    //item_data.setting_font = MMI_DEFAULT_EXTRA_BIG_FONT_SIZE;//×?ì?,extra large
    item_data.setting_font = MMI_DEFAULT_NORMAL_FONT_SIZE; //×?ì?éè??ò?′?￡?oó????éúD§,medium
    item_data.text_data.buf.str_ptr = result_string.wstr_ptr;
    item_data.text_data.buf.len = result_string.wstr_len;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    MMI_GetLabelTextByLang(label_1, &warranty_string1);
    MMI_GetLabelTextByLang(label_2, &warranty_string2);
    MMI_GetLabelTextByLang(label_3, &warranty_string3);
    MMI_GetLabelTextByLang(label_4, &warranty_string4);

    item_data.text_data.buf.len = warranty_string1.wstr_len +
        warranty_string2.wstr_len +
        warranty_string3.wstr_len +
        warranty_string4.wstr_len + 16;

    warranty_text_buffer_ptr = SCI_ALLOCAZ((item_data.text_data.buf.len) * sizeof(wchar));
    //SCI_MEMSET(warranty_text_buffer_ptr, 0x00, (item_data.text_data.buf.len) * sizeof(wchar));

    if(PNULL == warranty_text_buffer_ptr)
    {
        item_data.text_type = GUIRICHTEXT_TEXT_RES;
        item_data.text_set_font =TRUE;
        item_data.setting_font = MMI_DEFAULT_NORMAL_FONT_SIZE; //×?ì?éè??ò?′?￡?oó????éúD§,medium
        item_data.text_data.res.id = label_1;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
        item_data.text_type = GUIRICHTEXT_TEXT_RES;
        item_data.text_data.res.id = label_2;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
        item_data.text_type = GUIRICHTEXT_TEXT_RES;
        item_data.text_data.res.id = label_3;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
        item_data.text_type = GUIRICHTEXT_TEXT_RES;
        item_data.text_data.res.id = label_4;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    }
    else
    {
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;
        item_data.text_set_font =TRUE;
        item_data.setting_font = MMI_DEFAULT_NORMAL_FONT_SIZE; //×?ì?éè??ò?′?￡?oó????éúD§,medium
        memcpy(warranty_text_buffer_ptr, warranty_string1.wstr_ptr, warranty_string1.wstr_len*2);
        item_data.text_data.buf.len = MMIAPICOM_Wstrlen(warranty_text_buffer_ptr);
        warranty_text_buffer_ptr[item_data.text_data.buf.len] = '\r';
        warranty_text_buffer_ptr[item_data.text_data.buf.len+1] = '\n';
        warranty_text_buffer_ptr[item_data.text_data.buf.len+2] = '\r';
        warranty_text_buffer_ptr[item_data.text_data.buf.len+3] = '\n';
        
        MMIAPICOM_Wstrncat(warranty_text_buffer_ptr, warranty_string2.wstr_ptr, warranty_string2.wstr_len);
        item_data.text_data.buf.len = MMIAPICOM_Wstrlen(warranty_text_buffer_ptr);
        warranty_text_buffer_ptr[item_data.text_data.buf.len] = '\r';
        warranty_text_buffer_ptr[item_data.text_data.buf.len+1] = '\n';
        warranty_text_buffer_ptr[item_data.text_data.buf.len+2] = '\r';
        warranty_text_buffer_ptr[item_data.text_data.buf.len+3] = '\n';
        
        MMIAPICOM_Wstrncat(warranty_text_buffer_ptr, warranty_string3.wstr_ptr, warranty_string3.wstr_len);
        item_data.text_data.buf.len = MMIAPICOM_Wstrlen(warranty_text_buffer_ptr);
        warranty_text_buffer_ptr[item_data.text_data.buf.len] = '\r';
        warranty_text_buffer_ptr[item_data.text_data.buf.len+1] = '\n';
        warranty_text_buffer_ptr[item_data.text_data.buf.len+2] = '\r';
        warranty_text_buffer_ptr[item_data.text_data.buf.len+3] = '\n';
        
        MMIAPICOM_Wstrncat(warranty_text_buffer_ptr, warranty_string4.wstr_ptr, warranty_string4.wstr_len);
        item_data.text_data.buf.len = MMIAPICOM_Wstrlen(warranty_text_buffer_ptr);
        item_data.text_data.buf.str_ptr = warranty_text_buffer_ptr;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
        SCI_FREE(warranty_text_buffer_ptr);
    }


// 	item_data.text_data.res.id = STR_PRIVACY_POLICY_1_EXT01;
// 	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
// 	item_data.text_type = GUIRICHTEXT_TEXT_RES;
// 	item_data.text_data.res.id = STR_PRIVACY_POLICY_2_EXT01;
// 	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
// 	item_data.text_type = GUIRICHTEXT_TEXT_RES;
// 	item_data.text_data.res.id = STR_PRIVACY_POLICY_3_EXT01;
// 	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
// 	item_data.text_type = GUIRICHTEXT_TEXT_RES;
// 	item_data.text_data.res.id = STR_PRIVACY_POLICY_4_EXT01;
// 	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
// 	item_data.text_type = GUIRICHTEXT_TEXT_RES;
// 	item_data.text_data.res.id = STR_PRIVACY_POLICY_5_EXT01;
// 	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
// 	item_data.text_type = GUIRICHTEXT_TEXT_RES;
// 	item_data.text_data.res.id = STR_PRIVACY_POLICY_6_EXT01;
// 	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
// 	item_data.text_type = GUIRICHTEXT_TEXT_RES;
// 	item_data.text_data.res.id = STR_PRIVACY_POLICY_7_EXT01;
// 	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
//	item_data.text_type = GUIRICHTEXT_TEXT_RES;
//	item_data.text_data.res.id = STR_PRIVACY_POLICY_7_mk;
//	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
//	item_data.text_type = GUIRICHTEXT_TEXT_RES;
//	item_data.text_data.res.id = STR_PRIVACY_POLICY_8_RM;
//	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
}

/*****************************************************************************/
//  Description :服务条款窗口处理函数 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWarrantyFirstPowerOnWindow(
												    MMI_WIN_ID_T     win_id,    // STARTUP NORMAL′°?úμ?ID
												    MMI_MESSAGE_ID_E    msg_id,     // ′°?úμ??ú2????￠ID
													DPARAM              param       // ?àó|???￠μ?2?êy
												   )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   bg_rect = {0};
    GUI_BOTH_RECT_T both_client_rect = MMITHEME_GetWinClientBothRect(win_id);	 
    MMI_CTRL_ID_T  ctrl_id = PHONE_WARRANTY_RICHTEXT_CTRL_ID;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(MMITHEME_IsMainScreenLandscape())
        {
            bg_rect = both_client_rect.h_rect;
        }
        else
        {
            bg_rect = both_client_rect.v_rect;
        }
        GUI_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR);
//        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_WITHOUT_HEADER, TRUE);
        DisplayWarrantyScreenText(ctrl_id);
#ifdef MMI_ORTHODOX_VERSION
              InitSet2G4GMode(MN_DUAL_SYS_1); 
#ifdef MMI_MULTI_SIM_SYS_DUAL
              InitSet2G4GMode(MN_DUAL_SYS_2);
#endif
#endif

        GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
        GUIRICHTEXT_SetFocusItem(ctrl_id, 0);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_FULL_PAINT:
       	if(MMITHEME_IsMainScreenLandscape())
        {
            bg_rect = both_client_rect.h_rect;
        }
        else
        {
            bg_rect = both_client_rect.v_rect;
        }
        GUI_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR); 
        break;	
    case MSG_GET_FOCUS:
//        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_WITHOUT_HEADER, TRUE);
        MMIAPICOM_StatusAreaIdlescreenSet();
        break;
 //    case MSG_LOSE_FOCUS:
 //        break;
 //    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMK_CreateWin((uint32*)PHONE_FIRST_POWER_WARRANTY_OPT_MENU_WIN_TAB, PNULL);
        break;
        
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif

        MMK_CloseWin(win_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        break;
		
    case MSG_APP_RED:
		       break;
    case MSG_KEYUP_RED:
		MMK_CloseWin(win_id);
		break;
    case MSG_CLOSE_WINDOW:
#if defined(BROWSER_SUPPORT_WEBKIT) && ((!defined(WIN32)) || (defined(_MSC_VER ) && (_MSC_VER >= 1400))) && defined(MSA_SUPPORT)
		MMIAPIMSA_get_token(TRUE , MSA_APP_TYPE_CAMERA_ROLL , MMIAPIPHONE_MSA_Get_Token_Func);
#else 
		MMK_CreateWin((uint32*)PHONE_STARTUP_SPINNING_WIN_TAB, PNULL);
#endif
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************/
//  Description :window for warranty in first power on
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************/
LOCAL MMI_RESULT_E HandleTransportFirstPowerOnWindow(
                                                    MMI_WIN_ID_T     win_id,    // STARTUP NORMAL′°?úμ?ID
                                                    MMI_MESSAGE_ID_E    msg_id,     // ′°?úμ??ú2????￠ID
                                                    DPARAM              param       // ?àó|???￠μ?2?êy
                                                   )
{
    
     MMI_RESULT_E    result = MMI_RESULT_TRUE;
     GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
     GUI_RECT_T   bg_rect = {0};
     GUI_BOTH_RECT_T both_client_rect = MMITHEME_GetWinClientBothRect(win_id);  
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(MMITHEME_IsMainScreenLandscape())
        {
            bg_rect = both_client_rect.h_rect;
        }
        else
        {
            bg_rect = both_client_rect.v_rect;
        }
        GUI_FillRect(&lcd_dev_info, bg_rect, MMI_WHITE_COLOR);        
        break;
    case MSG_FULL_PAINT: 
        break;  
    case MSG_GET_FOCUS:
        MMK_CloseWin(win_id);        
        break;
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_NOTIFY_MIDSK:
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
        break;        
    case MSG_CLOSE_WINDOW:
         MMIAPIPHONE_OpenSelectMobleDataSim();
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description :首次开机时间日期设置询问窗口处理函数 
//  Global resource dependence : 
//  Author:
//  Note:2015.6.20
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimeAndDateSettingQuery(
                                                 MMI_WIN_ID_T win_id,
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param
                                                 )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE; 
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   bg_rect = {0};
    GUI_BOTH_RECT_T both_client_rect = MMITHEME_GetWinClientBothRect(win_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(MMITHEME_IsMainScreenLandscape())
        {
            bg_rect = both_client_rect.h_rect;
        }
        else
        {
            bg_rect = both_client_rect.v_rect;
        }
        GUI_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR);
        break;
    case MSG_GET_FOCUS:
        break;
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:
#ifdef MMI_ORTHODOX_VERSION
         MMK_CreateWin((uint32*)PHONE_WARRANTY_FIRST_POWER_WIN_TAB, FALSE);
#endif
//          MMISET_CreatePowerOnDateTimeWin(); 

        MMK_CloseWin(win_id);
        break;
    case MSG_APP_CANCEL:
//        MMIAPISET_SetIsAutoUpdateTime(TRUE);//auto update time and date
       
#ifdef MMI_ORTHODOX_VERSION
       MMK_CreateWin((uint32*)PHONE_WARRANTY_FIRST_POWER_WIN_TAB, FALSE);
#endif
	MMIAPIPHONE_OpenSelectMobleDataSim();
       MMK_CloseWin(win_id);       
        break;
    case MSG_CLOSE_WINDOW:
        break;
    case MSG_APP_RED:
    case MSG_KEYUP_RED:
            break;		
    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}

/*****************************************************************************/
//  Description :创建日期设置dialog窗口
//  Global resource dependence : 
//  Author:
//  Note:2015.6.20
/*****************************************************************************/
LOCAL void MMIPHONE_CreateDateTimeDialogWindow(void)
{
	 MMI_WIN_PRIORITY_E      win_priority_dialog = WIN_ONE_LEVEL;
	 MMI_WIN_ID_T     win_id_query = PHONE_FIRST_POWER_DATE_SET_WIN_ID;
	 MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_START_DATE_TIME_EXT01, PNULL, IMAGE_NULL, &win_id_query,&win_priority_dialog, MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,HandleTimeAndDateSettingQuery);
}

/*****************************************************************************/
//  Description :set the first power nv
//  Global resource dependence : 
//  Author:zhenxing
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetFirstPowerOnNV(void)
{
    BOOLEAN                 is_first_power_on = FALSE;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;

    SCI_TRACE_LOW("[POWERONOFF] MMIAPIPHONE_SetFirstPowerOnNV s_first_language_select_flag = %d, is_first_power_on = %d",s_first_language_select_flag, is_first_power_on);
    if(TRUE == s_first_language_select_flag)
    {
        //for first power on for bug507123  
        MMINV_READ(MMINV_PHONE_FIRST_POWER_ON, &is_first_power_on, return_value);//读NV
        if (MN_RETURN_SUCCESS != return_value)
        {
            is_first_power_on = TRUE;
            MMINV_WRITE(MMINV_PHONE_FIRST_POWER_ON, &is_first_power_on);
        }
        else if( is_first_power_on == TRUE ) // the first power on is end
        {
            is_first_power_on = FALSE;
            MMINV_WRITE(MMINV_PHONE_FIRST_POWER_ON, &is_first_power_on);
            SCI_TRACE_LOW("MMIAPIPHONE_SetFirstPowerOnNV set the nv success");
        } 
        SCI_TRACE_LOW("MMIAPIPHONE_SetFirstPowerOnNV is the first power on");
    }
    else
    {
        SCI_TRACE_LOW("MMIAPIPHONE_SetFirstPowerOnNV is not the first power on");
    }       
}

/*****************************************************************************/
//  Description :spinning窗口处理函数
//  Global resource dependence : 
//  Author:
//  Note:2015.6.20
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStartupSpinningWindow(
                                                    MMI_WIN_ID_T     win_id,    // STARTUP NORMAL′°?úμ?ID
                                                    MMI_MESSAGE_ID_E    msg_id,     // ′°?úμ??ú2????￠ID
                                                    DPARAM              param       // ?àó|???￠μ?2?êy
                                                   )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   bg_rect = {0};
    GUI_BOTH_RECT_T both_client_rect = MMITHEME_GetWinClientBothRect(win_id);
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    MMI_STRING_T            spinner_text = {0};
    uint8        i   =    MN_DUAL_SYS_1;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:  
        ctrl_info.ctrl_id = PHONE_STARTUP_SPINNING_CTRL_ID;
        ctrl_info.is_ctrl_id = TRUE;
        data_info.anim_id = ANIM_PUBWIN_WAIT;
        display_info.align_style    = GUIANIM_ALIGN_NONE;
        display_info.is_play_once   = FALSE;
        display_info.is_bg_buf      = TRUE;
        display_info.bg.bg_type     = GUI_BG_COLOR;
        display_info.bg.color       = MMI_WHITE_COLOR;
        GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
        CTRLANIM_SetSpinnerTextById( ctrl_info.ctrl_id, STR_START_PHONE_STARTING_EXT01);

        //bug507123
        MMIAPIPHONE_SetFirstPowerOnNV();
        if( s_spinning_diaplay_timer != 0 ) 
        {
            MMK_StopTimer(s_spinning_diaplay_timer);
            s_spinning_diaplay_timer = 0;
        } 
        s_spinning_diaplay_timer = MMK_CreateTimer(SPINNING_DISPLAY_TIME, FALSE);
        break;
    case MSG_FULL_PAINT:
        if(MMITHEME_IsMainScreenLandscape())
        {
            bg_rect = both_client_rect.h_rect;
        }
        else
        {
            bg_rect = both_client_rect.v_rect;
        }
        GUI_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR); 
        break;
    case MSG_TIMER:
        if (s_spinning_diaplay_timer == *((uint8 *) param))
        {
            MMK_StopTimer(s_spinning_diaplay_timer);
            s_spinning_diaplay_timer = 0;
//          for(i=MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
//          {
//          SCI_TRACE_LOW("[startup date and time] HandleStartupSpinningWindow sim_sys = %c", i);
//               if(MMIAPIPHONE_GetSimExistedStatus(i))//óD?¨2?è?
//               {
//                 SCI_TRACE_LOW("[startup date and time] HandleStartupSpinningWindow sim_sys_in = %c", i);
//                 MMK_CloseWin(win_id);
//                 result = MMI_RESULT_FALSE;
//                 return (result);
//               }
//          }
//         SCI_TRACE_LOW("[startup date and time] HandleStartupSpinningWindow no_sim");
//         MMIPHONE_CreateDateTimeDialogWindow();   
           MMK_CloseWin(win_id);
           result = MMI_RESULT_FALSE;
        }
        break;
    case MSG_GET_FOCUS:
        break;
    case MSG_LOSE_FOCUS:
        break;
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_NOTIFY_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT 
    case MSG_CTL_PENOK:
#endif 
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description :飞行模式询问窗口处理函数 
//  Global resource dependence : 
//  Author:
//  Note:2015.6.20
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFlyQueryDialogWindow(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE; 
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   bg_rect = {0};
    GUI_BOTH_RECT_T both_client_rect = MMITHEME_GetWinClientBothRect(win_id);
    BOOLEAN continue_fly_mode_on = TRUE;
    BOOLEAN continue_fly_mode_off = FALSE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
	{
		MMI_STRING_T prompt_str = {0};
		
		if (MMIAPISET_GetFlyModeSetPowerOn() == FALSE && MMIAPISET_GetFlightMode())
		{
			// 在settings设置飞行模式，并没有设置开机询问，直接以飞行模式继续开机
			MMIAPISET_SetFlyMode(continue_fly_mode_on);	
			MMK_CloseWin(win_id);
			break;
		}
        if(MMITHEME_IsMainScreenLandscape())
        {
            bg_rect = both_client_rect.h_rect;
        }
        else
        {
            bg_rect = both_client_rect.v_rect;
		}
		GUI_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR); 
		// GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_WITHOUT_HEADER, TRUE);
		MMI_GetLabelTextByLang(STR_FLIGHT_QUERY_EXT02, &prompt_str);
		MMIPUB_SetAlertWinTextByPtr(win_id, &prompt_str, PNULL, TRUE);
		break;
	}
    case MSG_APP_OK:
	case MSG_CTL_OK:
	case MSG_APP_WEB:
	case MSG_NOTIFY_MIDSK:
        MMIAPISET_SetFlyMode(continue_fly_mode_on);	
	    MMK_CloseWin(win_id);
		break;
	case MSG_GET_FOCUS:
//		GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_WITHOUT_HEADER, TRUE);
//		MMIAPICOM_StatusAreaIdlescreenSet();
		break;
    case MSG_APP_CANCEL:
        MMIAPISET_SetFlyMode(continue_fly_mode_off);		
		MMK_CloseWin(win_id);
		break;
	case MSG_CLOSE_WINDOW:		
		if(MMIAPISET_IsPowerOnSecruityCodeEnable()){
			MMIAPISET_OpenPowerOnPwdWin();
		}
		else
		{
            MMIAPIPHONE_NormalInitAfterPwd();
		}
		break;
    case MSG_APP_RED:
    case MSG_KEYUP_RED:
        if(MMIAPISET_IsPowerOnSecruityCodeEnable()){	
            MMK_CloseWin(win_id);
            break;
        }
    default:
		recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}

/*****************************************************************************/
//  Description :飞行模式询问设置dialog窗口
//  Global resource dependence : 
//  Author:
//  Note:2015.6.20
/*****************************************************************************/
PUBLIC void MMIPHONE_CreateFlyQueryDialogWindow(void)
{
	 MMI_WIN_PRIORITY_E      win_priority_dialog = WIN_ONE_LEVEL;
	 MMI_WIN_ID_T     win_id_query = PHONE_STARTUP_FLY_QUERY_WIN_ID;
	 MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_FLIGHT_QUERY_EXT02, PNULL, IMAGE_NULL, &win_id_query, PNULL, MMIPUB_SOFTKEY_DIALOG_OK_CLOSE, HandleFlyQueryDialogWindow);
     s_fly_window_open_flag = TRUE;
 }

/*****************************************************************************/
//  Description : get token cb func
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_MSA_Get_Token_Func( int result, char*  buf, uint16 buflen)
{
#ifdef MSA_SUPPORT
	MMIAPIMSA_Exit();
#endif
#if 0	
#if defined(BROWSER_SUPPORT_WEBKIT) && ((!defined(WIN32)) || (defined(_MSC_VER) && (_MSC_VER >= 1400)))
	if(MMK_IsOpenWin(MMIWK_WIN_ID))
	{
		MMK_CloseWin(MMIWK_WIN_ID);
	}
#endif	
#endif
	MMIAPIPHONE_OpenStartupSpinningWin();
}

/*****************************************************************************/
// 	Description : open normal start up Spinning window
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_OpenStartupSpinningWin(void)
{
	MMK_CreateWin((uint32*)PHONE_STARTUP_SPINNING_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description : open normal start up WarrantyFirstPowerOn window
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void 	MMIAPIPHONE_OpenWarrantyFirstPowerOnWin(void)
{
	MMK_CreateWin((uint32*)PHONE_WARRANTY_FIRST_POWER_WIN_TAB, PNULL);
}


/*****************************************************************************/
//  Description : 处理正常开机界面的消息(除PS之外)
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNormalStartupWindow(
                                             MMI_WIN_ID_T    win_id,    // STARTUP NORMAL窗口的ID
                                             MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                             DPARAM             param       // 相应消息的参数
                                             )
{   
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMISET_POWER_ON_OFF_ANIM_INFO_T power_on_off_anim_info = {0};
    wchar *welcome_wstr = PNULL;
// 	wchar   welcome_wstr[127+1] = {0};
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
        {
            GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
            GUI_RECT_T   rect = {0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};

            LCD_FillRect((const GUI_LCD_DEV_INFO*)&lcd_dev_info, rect, MMI_WHITE_COLOR);    
       }
#endif 
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        MMIDEFAULT_TurnOnBackLight();
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetMainLayerType(DATA_TYPE_RGB565);
#endif
        //s_is_play_default = TRUE;
#ifndef MMI_POWERONOFFMP4_ENABLE
        s_is_play_power_on_mp4 = TRUE; 
        SCI_MEMSET(&power_on_off_anim_info,0,sizeof( MMISET_POWER_ON_OFF_ANIM_INFO_T));
        MMIAPISET_GetPowerOnOffAnimInfo(&power_on_off_anim_info, TRUE);

        //SCI_TRACE_LOW:"[POWERONOFF]:HandleNormalStartupWindow type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_ONOFF_421_112_2_18_2_45_25_224,(uint8*)"d",power_on_off_anim_info.anim_type);

        if (MMISET_POWER_ON_OFF_ANIM_DEFAULT == power_on_off_anim_info.anim_type)
        {
            PlayDefaultPowerOnOffAnimation(TRUE);
#if 0
			if (s_first_anim_timer_id != 0)
			{
				MMK_StopTimer(s_first_anim_timer_id);
				s_first_anim_timer_id = 0;
			}
			s_first_anim_timer_id = MMK_CreateTimer(PHONE_FIRST_ANIM_TIME, FALSE);//?ˉ?-×ê?′?ó??oóDèòaμ÷??ê±??
#endif
        }
        else if((MMISET_POWER_ON_OFF_ANIM_MORE == power_on_off_anim_info.anim_type) 
                || (MMISET_POWER_ON_OFF_PIC_MORE == power_on_off_anim_info.anim_type))
        {
            if(!MMIAPIFMM_IsFileExist(power_on_off_anim_info.more_anim_info.name_wstr,power_on_off_anim_info.more_anim_info.name_wstr_len))
            {
                PlayDefaultPowerOnOffAnimation(TRUE);
            }
            else
            {    
                uint16 suffix_wstr[MMIFMM_FILENAME_LEN + 1]        = {0};
                uint16 suffix_len = MMIFMM_FILENAME_LEN;
                MMIAPIFMM_SplitFileName(power_on_off_anim_info.more_anim_info.name_wstr, 
                    power_on_off_anim_info.more_anim_info.name_wstr_len, 
                    PNULL, PNULL, suffix_wstr, &suffix_len);
				//bug 1247143
                //if(MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_ConvertFileType(suffix_wstr,suffix_len))
                if(MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_ConvertFileType(suffix_wstr,suffix_len)
#ifdef DRM_SUPPORT
                         || MMIFMM_FILE_TYPE_DRM == MMIAPIFMM_ConvertFileType(suffix_wstr,suffix_len)
#endif
                   )					
                {
                    PlayMorePowerOnOffAnimation(TRUE, &power_on_off_anim_info);
                }
            }
        }
#endif
        if (s_phone_startup_timer_id != 0)
        {
            MMK_StopTimer(s_phone_startup_timer_id);
            s_phone_startup_timer_id = 0;
        }
        s_phone_startup_timer_id = MMK_CreateTimer(PHONE_STARTUP_DURING_TIME, FALSE);
        PlayPowerOnOffMP4(TRUE); 
        break;

    case MSG_TIMER:
#if 0
		if(s_first_anim_timer_id == *((uint8 *) param))
		{
		if(CHGMNG_IsChargeConnect())  //Bug 462319
		DisplaySecondAnim(PHONE_STARTUP_NORMAL_ANIMATEBOX_CTRL_ID, IMAGE_STARTUP_CHARGER_CONNECTED);//?a?ú?ˉ?-×ê?′2
			MMK_StopTimer(s_first_anim_timer_id);  
			s_first_anim_timer_id = 0;			
		}
#endif
        if (s_phone_startup_timer_id == *((uint8 *) param))
        { 
            s_phone_startup_timer_id = 0;
            
            // 检查Alarm的情况
            MMIAPIALM_CheckAndStartDelayedAlarm();            
            
            // 关闭本窗口
            //MMK_CloseWin(win_id);
            
            SCI_MEMSET(&power_on_off_anim_info, 0, sizeof(MMISET_POWER_ON_OFF_ANIM_INFO_T));
            MMIAPISET_GetPowerOnOffAnimInfo(&power_on_off_anim_info, TRUE);
#ifdef VIDEO_PLAYER_SUPPORT 
            if((MMISET_POWER_ON_OFF_ANIM_MORE == power_on_off_anim_info.anim_type)
                || (MMISET_POWER_ON_OFF_PIC_MORE == power_on_off_anim_info.anim_type))
            {
                uint16 suffix_wstr[MMIFMM_FILENAME_LEN + 1]        = {0};
                uint16 suffix_len = MMIFMM_FILENAME_LEN;
                MMIAPIFMM_SplitFileName(power_on_off_anim_info.more_anim_info.name_wstr, 
                    power_on_off_anim_info.more_anim_info.name_wstr_len, 
                    PNULL, PNULL, suffix_wstr, &suffix_len);
                if(MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_ConvertFileType(suffix_wstr,suffix_len))
                {
                    PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
                }
                else
                {
#ifdef MMI_POWERONOFFMP4_ENABLE
                    MMIAPIVP_ExitPlay();
#else
                    //if(s_is_play_default)//cr151078
                    //{
                    PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
                    //}
#endif
                }
            }
            else if(MMISET_POWER_ON_OFF_ANIM_DEFAULT == power_on_off_anim_info.anim_type)
            {

#ifdef MMI_POWERONOFFMP4_ENABLE
                MMIAPIVP_ExitPlay();
#else
                PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
#endif

            }
#else
            PlayPowerOnOffMP4CallBack();
#endif
            // 打开IDLE界面 
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
     
    case MSG_LOSE_FOCUS: 
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetMainLayerType(MMITHEME_GetMainLayerDefaultType());
#endif
#ifdef VIDEO_PLAYER_SUPPORT
         MMIAPIVP_ExitPlay();
#endif
        //cr124655, 当开机窗口被打断，需要中止开机铃声
        MMIAPISET_StopRing(MMISET_RING_TYPE_POWER);
        MMIAPIIDLE_OpenIdleWin();
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetMainLayerType(MMITHEME_GetMainLayerDefaultType());
#endif
        // 关闭开机窗口定时器
        if( 0 != s_phone_startup_timer_id )
        {
            // 检查Alarm的情况
            MMIAPIALM_CheckAndStartDelayedAlarm();
            MMK_StopTimer(s_phone_startup_timer_id);
            s_phone_startup_timer_id = 0;
            MMIAPIIDLE_OpenIdleWin();
        }
#ifdef VIDEO_PLAYER_SUPPORT
        MMIAPIVP_ExitPlay();
#endif
#if defined(MMI_POWERONOFFMP4_ENABLE)
		if (PNULL != s_poweronoff_buffer_ptr)
		{
			SCI_FREE(s_poweronoff_buffer_ptr);
			s_poweronoff_buffer_ptr = PNULL;
		}
#endif
        //cr124655, 当开机窗口被打断，需要中止开机铃声
        MMIAPISET_StopRing(MMISET_RING_TYPE_POWER);
        MMIAPIPHONE_SetPowerAnimMpeg4Flag(FALSE);
#ifdef MSDC_CARD_SUPPORT        
        MMIAPISD_CheckAllDelayedDevice();
#endif
        //MMIAPIPHONE_ResumeAllDelaiedEvent();
//        if (MMIAPIUdisk_IsUSBDelay())
//        {        
//            MMIAPIUdisk_OpenUsbOperWindow();
//            MMIAPIUdisk_SetUSBDelay(FALSE);     
//         }
        //MMIDEFAULT_EnableKeyRing(TRUE);
        //MMIDEFAULT_EnableTpRing(TRUE);
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        MMI_RestoreFreq();
#if 0   
		MMISet_InitVibrate();//dong.chunguang add for turn on vib as ms design

		MMISet_GetWelcomeNote(welcome_wstr);

		if(MMIAPIPHONE_IsOperatorLogoRingDisplay())	//	bug486712: new requirement for operator logo
		{
			MMK_CreateWin((uint32*)PHONE_STARTUP_OPERATOR_LOGO_WIN_TAB, PNULL);//operator window
		}
		else if(MMIAPICOM_Wstrlen(welcome_wstr)!=0)//if(0)//MMISET_GetWelcomeNote(&welcome_wstr);
		{
			MMK_CreateWin((uint32*)PHONE_STARTUP_OPERATOR_PROMO_WIN_TAB, PNULL);
		}
		else
		{
			// CreateLanguageSelectFirstPowerOn();
			if (MMIAPISET_GetFlyModeSetPowerOn())	 //2015.6.22
			{
#ifdef ENG_SUPPORT
				if(!MMIAPIENGTestMode_IsSet())
#endif
				{
					MMIPHONE_CreateFlyQueryDialogWindow();
				}
			}
			else
			{
				CreateLanguageSelectFirstPowerOn();
			}
		}
#endif
        break;
       
#ifdef LCD_SLIDE_SUPPORT
    case MSG_KEYDOWN_SLIDE:
    case MSG_KEYUP_SLIDE:
#endif     
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
    case MSG_APP_RED:
    case MSG_KEYLONG_RED:
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : [MS]开机operator promo显示窗口处理函数
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOperatorPromoStartupWindow(
                                                      MMI_WIN_ID_T    win_id,    // STARTUP NORMAL窗口的ID
                                                      MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                      DPARAM             param       // 相应消息的参数
                                                    )
{   
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T   operator_ctrl_id= PHONE_STARTUP_OPERATOR_PROMOL_TEXT_CTRL_ID;
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_RECT_T  ctrl_rect = MMITHEME_GetFullScreenRectEx(win_id);
	GUI_BG_T    bg_color_ptr = {GUI_BG_COLOR, 0, PNULL, MMI_BLACK_COLOR, PNULL};
	GUI_FONT_T   font_ptr = MMI_DEFAULT_NORMAL_FONT_SIZE;
	GUI_COLOR_T  font_color = MMI_WHITE_COLOR;
	MMI_STRING_T  operator_string = {0};
//	wchar       welcome_wstr = 0;
	wchar  welcome_wstr[127+1] = {0};
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
//       MMISET_GetWelcomeNote(&welcome_wstr);
//		 operator_string.wstr_len = MMIAPICOM_Wstrlen(&welcome_wstr);
		 //MMI_GetLabelTextByLang(STR_START_WELCOME_HDR_EXT02, &operator_string);
		MMISet_GetWelcomeNote(welcome_wstr);
		operator_string.wstr_len = MMIAPICOM_Wstrlen(welcome_wstr);
		operator_string.wstr_ptr = welcome_wstr;

	     GUITEXT_SetMargin(operator_ctrl_id, 0x0);
	     GUITEXT_SetRect(operator_ctrl_id, &ctrl_rect);
	     GUITEXT_SetBg(operator_ctrl_id, &bg_color_ptr);
	     GUITEXT_SetFont(operator_ctrl_id, &font_ptr, &font_color);
		 GUITEXT_SetAlign(operator_ctrl_id, ALIGN_HVMIDDLE);
	     GUITEXT_SetString(operator_ctrl_id, operator_string.wstr_ptr, operator_string.wstr_len, FALSE);
//		 MMK_SetAtvCtrl(win_id, operator_ctrl_id);
		 if (s_operator_timer_id != 0)
        {
            MMK_StopTimer(s_operator_timer_id);
            s_operator_timer_id = 0;
        }
        s_operator_timer_id = MMK_CreateTimer(PHONE_OPERATOR_PROMO_TIME, FALSE);
        break;
    case MSG_TIMER:
		if(s_operator_timer_id == *((uint8 *) param))
		{
			MMK_StopTimer(s_operator_timer_id);  
			s_operator_timer_id = 0;
			MMK_CloseWin(win_id);
		}
        break;
    case MSG_CLOSE_WINDOW:
//		 CreateLanguageSelectFirstPowerOn();	//′′?¨ê×′??a?úó???′°?ú
            if (MMIAPISET_GetFlightMode())	 //2015.6.22
			{ 	
			#ifdef ENG_SUPPORT
				if(!MMIAPIENGTestMode_IsSet())
			#endif
			      MMIPHONE_CreateFlyQueryDialogWindow();
			}
		    else if(MMIAPISET_IsPowerOnSecruityCodeEnable()){
				MMIAPISET_OpenPowerOnPwdWin();
			}
			else
			{
				CreateLanguageSelectFirstPowerOn();
			}
        break;
#ifdef LCD_SLIDE_SUPPORT
    case MSG_KEYDOWN_SLIDE:
    case MSG_KEYUP_SLIDE:
#endif     
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
    case MSG_APP_RED:
    case MSG_KEYLONG_RED:
        break;
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#if 0
/*****************************************************************************/
//  Description : [MS]开机operator logo play ring
//  Global resource dependence : 
//  Author: 
//  Note:zhenxing.chen
/*****************************************************************************/
LOCAL void PlayOperatorLogoRing(void)
{
	BOOLEAN   is_earfree = TRUE;
    uint16    ring_id  =  PROMO_OPERATOR_RING;  //注意下面对ring id的处理
    uint32    play_times = 1;
    MMISET_ALL_RING_TYPE_E    ring_type = MMISET_RING_TYPE_POWER;
//	MMIAPISET_StopAppRing();
	MMIAPISET_PlayRingById(is_earfree, (ring_id - MMI_POWER_RING_START_ID + 1), play_times, ring_type, PNULL);
}
#endif
/*****************************************************************************/
//  Description : [MS]开机operator logo显示窗口处理函数
//  Global resource dependence : 
//  Author: 
//  Note:zhenxing.chen
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStartupOperatorLogoWindow(
                                                   MMI_WIN_ID_T    win_id,    // STARTUP NORMAL窗口的ID
                                                   MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                   DPARAM             param       // 相应消息的参数
                                                   )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T    ctrl_id =  PHONE_STARTUP_OPERATOR_LOGO_CTRL_ID;
    GUIANIM_CTRL_INFO_T    ctrl_info = {0};
    GUIANIM_DATA_INFO_T    data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    GUI_RECT_T    anim_rect = MMITHEME_GetFullScreenRect();
    uint8     display_mode = MMIAPIPHONE_IsOperatorLogoRingDisplay();
    wchar  welcome_wstr[127+1] = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW: 		 
        //set anim param
        ctrl_info.ctrl_id = ctrl_id;
        ctrl_info.is_ctrl_id = TRUE;
#ifdef MMI_ORTHODOX_VERSION
        data_info.img_id = IMAGE_WALLPAPER_1;
#else
        data_info.img_id = IMAGE_CIRCULAR_LOADER;   //anim id
#endif
        display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
        display_info.is_play_once = TRUE;
        display_info.is_need_end_ind = TRUE;
        display_info.is_update = FALSE; 
        display_info.bg.bg_type  =  GUI_BG_COLOR;
        display_info.bg.color  =  MMI_WINDOW_BACKGROUND_COLOR;
        GUIANIM_SetDisplayRect(ctrl_id, &anim_rect, FALSE);
        GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
#if 0
        if( display_mode == DISPLAY_OPERATOR_LOGO_RING)
        {
            PlayOperatorLogoRing();
        }
#endif
        MMK_SetAtvCtrl(win_id,ctrl_id);

        if (s_operator_timer_id != 0)
        {
            MMK_StopTimer(s_operator_timer_id);
            s_operator_timer_id = 0;
        }
        s_operator_timer_id = MMK_CreateTimer(PHONE_OPERATOR_PROMO_TIME, FALSE);
        break;

    case MSG_NOTIFY_ANIM_END_IND: //GIF play one time
#if 0
        if( display_mode == DISPLAY_OPERATOR_LOGO_RING)
        {
            MMIAPISET_StopRing(MMISET_RING_TYPE_POWER);
        }
#endif
//        MMK_CloseWin(win_id);
        break;

    case MSG_TIMER:
        if(s_operator_timer_id == *((uint8 *) param))
        {
            MMK_StopTimer(s_operator_timer_id);  
            s_operator_timer_id = 0;
            MMK_CloseWin(win_id);
        }
        break;
    
    
    case MSG_LOSE_FOCUS:
        break;
    case MSG_CLOSE_WINDOW:
        MMISet_GetWelcomeNote(welcome_wstr);
        if(MMIAPICOM_Wstrlen(welcome_wstr)!=0)//if(0)//MMISET_GetWelcomeNote(&welcome_wstr);
        {
            MMK_CreateWin((uint32*)PHONE_STARTUP_OPERATOR_PROMO_WIN_TAB, PNULL);//welcome note window
        }
        else
        {
            if (MMIAPISET_GetFlightMode())	
            {
             #ifdef ENG_SUPPORT
			if(!MMIAPIENGTestMode_IsSet())
			#endif
                MMIPHONE_CreateFlyQueryDialogWindow();
            }
            else
            {
                CreateLanguageSelectFirstPowerOn();
            }
        }
        break;
#ifdef LCD_SLIDE_SUPPORT
    case MSG_KEYDOWN_SLIDE:
    case MSG_KEYUP_SLIDE:
#endif     
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
    case MSG_APP_RED:
    case MSG_KEYLONG_RED:
        break;
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
/*****************************************************************************/
//  Description : 处理闹铃启动开机界面的消息(除PS之外)
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlarmStartupWindow (
                                             MMI_WIN_ID_T    win_id,    // STARTUP ALARM窗口的ID
                                             MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                             DPARAM             param       // 相应消息的参数
                                             )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_StopTimer(s_3sec_timer_id);
        s_3sec_timer_id = 0;
        break;

    case MSG_FULL_PAINT:
        //设置闹钟背景图
        if(MMK_IsOpenWin(MMIPUB_QUERY_WIN_ID))
        {
            MMK_CloseWin(MMIPUB_QUERY_WIN_ID);
        }
        //询问是否开机!!
        //MMIPUB_OpenQueryWinByTextId(TXT_STARTUP_OR_NOT,IMAGE_PUBWIN_QUERY,PNULL,HandleStartOrNotWindow);

        MMIPUB_OpenConfirmationDialogByTextId(PNULL,TXT_STARTUP_OR_NOT,PNULL,
                IMAGE_NULL,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,HandleStartOrNotWindow);

        if (s_3sec_timer_id != 0)
        {
            MMK_StopTimer(s_3sec_timer_id);
            s_3sec_timer_id = 0;
        }
        s_3sec_timer_id = MMK_CreateTimer(ALM_3SEC_TIME, FALSE);
        break;

    case MSG_TIMER:
        if (s_3sec_timer_id == *((uint8 *)param))
        {
            MMK_StopTimer(s_3sec_timer_id);
            s_3sec_timer_id = 0;

            //关闭询问窗口
            //[show alarm expired win when start up charging]
            //MMIPUB_CloseQuerytWin(PNULL);
            MMIPUB_CloseAlertWin();
            //[end]

            //destroy闹铃界面
            MMK_CloseWin(PHONE_STARTUP_ALARM_WIN_ID);

            if (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition())
            {
                //POWER_PowerOff()之前关闭背光
                MMIDEFAULT_TurnOffBackLight();
                MMIDEFAULT_SetKeybadBackLight(FALSE);

                // 关机
                POWER_PowerOff();
            }
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    case MSG_PROMPTWIN_OK:
    case MSG_KEYLONG_RED:
        MMK_StopTimer(s_3sec_timer_id);
        s_3sec_timer_id = 0;
        //关闭询问窗口
        //[show alarm expired win when start
        // up charging]
        //MMIPUB_CloseQuerytWin(PNULL);
        MMIPUB_CloseAlertWin();
        //[end]

        //destroy闹铃界面
        MMK_CloseWin(PHONE_STARTUP_ALARM_WIN_ID);

        // 开机
        MMIAPIPHONE_PowerReset();
        // 记录开机方式
        MMIAPIPHONE_SetStartUpCondition(STARTUP_NORMAL);
        break;

    case MSG_PROMPTWIN_CANCEL:
    case MSG_APP_RED:
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.    
    case MSG_APP_FLIP:
#endif
        MMK_StopTimer(s_3sec_timer_id);
        s_3sec_timer_id = 0;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
        if(MSG_APP_FLIP == msg_id)
        {
            MMIDEFAULT_HandleFlipKey(FALSE);
        }
#endif

        //关闭询问窗口
        //[show alarm expired win when start up charging]
        //MMIPUB_CloseQuerytWin(PNULL);
        MMIPUB_CloseAlertWin();
        //[end]

        //destroy闹铃界面
        MMK_CloseWin(PHONE_STARTUP_ALARM_WIN_ID);

        if (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition())
        {
            //POWER_PowerOff()之前关闭背光
            MMIDEFAULT_TurnOffBackLight();
            MMIDEFAULT_SetKeybadBackLight(FALSE);

            // 关机
            POWER_PowerOff();
        }
        break;
    case MSG_CLOSE_WINDOW:
        if(0!=s_3sec_timer_id)
        {
            MMK_StopTimer(s_3sec_timer_id);
            s_3sec_timer_id = 0;
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

//[modify power off alarm function]
LOCAL void SetWinBackgroundColor()
{
    GUI_RECT_T   rect = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};

    LCD_FillRect(&lcd_dev_info, rect, MMI_BACKGROUND_COLOR);
}
//[end]

/*****************************************************************************/
//  Description : 关机闹钟后，是否开机询问窗口处理函数
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStartOrNotWindow(
                                          MMI_WIN_ID_T     win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    //[modify power off alarm function]
    case MSG_OPEN_WINDOW:
        //MMIPUB_SetDialogBgLayerStyle(win_id,DIALOG_FULL_BG_SCREEN);
        SetWinBackgroundColor();
        break;
    //[end]
    //[in alarm start up window, press mid softkey can power on]
    //case MSG_APP_WEB:
    //[end]
    case MSG_KEYLONG_RED:
        //关闭询问窗口
        //[show alarm expired win when start up charging]
        //MMIPUB_CloseQuerytWin(PNULL);
        MMIPUB_CloseAlertWin();
        //[end]

        //destroy闹铃界面
        MMK_CloseWin(PHONE_STARTUP_ALARM_WIN_ID);

        // 开机
        MMIAPIPHONE_PowerReset();
        // 记录开机方式
        MMIAPIPHONE_SetStartUpCondition(STARTUP_NORMAL);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        //关闭询问窗口
        //[show alarm expired win when start up charging]
        //MMIPUB_CloseQuerytWin(PNULL);
        MMIPUB_CloseAlertWin();
        //[end]

        //destroy闹铃界面
        MMK_CloseWin(PHONE_STARTUP_ALARM_WIN_ID);

        if (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition())
        {
            // 关机
            MMIDEFAULT_TurnOffBackLight();
            MMIDEFAULT_SetKeybadBackLight(FALSE);
            POWER_PowerOff();
        }
        break;
#endif

    //[don't return to mainmenu win
    // when press end key in power off alarm win]
    case MSG_KEYUP_RED:
        result = MMI_RESULT_TRUE;
        break;
    //[end]

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : 处理关机界面
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerOffWindow(
                                        MMI_WIN_ID_T     win_id,    // STARTUP NORMAL窗口的ID
                                        MMI_MESSAGE_ID_E    msg_id,     // 窗口的内部消息ID
                                        DPARAM              param       // 相应消息的参数
                                        )
{   
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    static uint8    s_power_off_timer_id = 0;             // 关机界面持续时间的ID号
#if 0
    static uint8    s_operator_logo_power_off_timer_id = 0;             // 关机界面持续时间的ID号
#endif
    MMISET_POWER_ON_OFF_ANIM_INFO_T power_on_off_anim_info = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetMainLayerType(DATA_TYPE_RGB565);
#endif
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/12
        {
            GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
            GUI_RECT_T   rect = {0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};

            LCD_FillRect((const GUI_LCD_DEV_INFO*)&lcd_dev_info, rect, MMI_WHITE_COLOR);    
       }
#endif 
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        MMIAPISET_StopAllRing(TRUE);  
        //s_is_play_default = TRUE;
#ifndef MMI_POWERONOFFMP4_ENABLE
        s_is_play_power_on_mp4 = FALSE; 
        SCI_MEMSET(&power_on_off_anim_info,0,sizeof( MMISET_POWER_ON_OFF_ANIM_INFO_T));
        MMIAPISET_GetPowerOnOffAnimInfo(&power_on_off_anim_info, FALSE);
        if (MMISET_POWER_ON_OFF_ANIM_DEFAULT == power_on_off_anim_info.anim_type)
        {
            PlayDefaultPowerOnOffAnimation(FALSE);
        }
        else if((MMISET_POWER_ON_OFF_ANIM_MORE == power_on_off_anim_info.anim_type) 
                || (MMISET_POWER_ON_OFF_PIC_MORE == power_on_off_anim_info.anim_type))
        {
            if(!MMIAPIFMM_IsFileExist(power_on_off_anim_info.more_anim_info.name_wstr,power_on_off_anim_info.more_anim_info.name_wstr_len))
            {
                PlayDefaultPowerOnOffAnimation(FALSE);
            }
            else
            {    
                uint16 suffix_wstr[MMIFMM_FILENAME_LEN + 1]        = {0};
                uint16 suffix_len = MMIFMM_FILENAME_LEN;
                MMIAPIFMM_SplitFileName(power_on_off_anim_info.more_anim_info.name_wstr, 
                    power_on_off_anim_info.more_anim_info.name_wstr_len, 
                    PNULL, PNULL, suffix_wstr, &suffix_len);
				//bug 1247143
                //if(MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_ConvertFileType(suffix_wstr,suffix_len))
                if(MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_ConvertFileType(suffix_wstr,suffix_len)
#ifdef DRM_SUPPORT
                         || MMIFMM_FILE_TYPE_DRM == MMIAPIFMM_ConvertFileType(suffix_wstr,suffix_len)
#endif
                    )
				{
                    PlayMorePowerOnOffAnimation(FALSE, &power_on_off_anim_info);
                }
            }

        }
#endif
#if 0
		 if (s_power_off_anim_timer_id != 0) //1??úμúò????ˉ?-2￥·??¨ê±?÷￡?ê±??μ?￡?2￥·?μú?t???ˉ?-
        {
            MMK_StopTimer(s_power_off_anim_timer_id);
            s_power_off_anim_timer_id = 0;
        }
        s_power_off_anim_timer_id = MMK_CreateTimer(PHONE_FIRST_ANIM_TIME, FALSE);
#endif
        if (s_power_off_timer_id != 0)
        {
            MMK_StopTimer(s_power_off_timer_id);
            s_power_off_timer_id = 0;
        }
#if 0
        if(MMIAPIPHONE_IsOperatorLogoRingDisplay())
        {
            s_power_off_timer_id = MMK_CreateTimer(PHONE_OPERATOR_PROMO_TIME, FALSE);
        }
        else
#endif
        {
            s_power_off_timer_id = MMK_CreateTimer(PHONE_POWER_OFF_DURING_TIME, FALSE);
        }
        break;
        
    case MSG_FULL_PAINT:

        #ifdef VIDEO_PLAYER_SUPPORT 
        MMIAPIVP_ExitPlay();//andy.he_crNEWMS00116228
        #endif
        
        PlayPowerOnOffMP4(FALSE);
        break;

    case MSG_TIMER:
        if (s_power_off_timer_id == *((uint8 *) param))
        {
            // 关闭关机窗口定时器
            MMK_StopTimer(s_power_off_timer_id);
            s_power_off_timer_id = 0;                    
            SCI_MEMSET(&power_on_off_anim_info, 0, sizeof(MMISET_POWER_ON_OFF_ANIM_INFO_T));
            MMIAPISET_GetPowerOnOffAnimInfo(&power_on_off_anim_info, FALSE);
#if 0
            if(MMIAPIPHONE_IsOperatorLogoRingDisplay())
            {
                PlayOperatorPowerOnOffAnimation(FALSE);
                s_operator_logo_power_off_timer_id = MMK_CreateTimer(PHONE_OPERATOR_PROMO_TIME, FALSE);
            }
#endif
#ifdef VIDEO_PLAYER_SUPPORT               
            if((MMISET_POWER_ON_OFF_ANIM_MORE == power_on_off_anim_info.anim_type)
                || (MMISET_POWER_ON_OFF_PIC_MORE == power_on_off_anim_info.anim_type))
            {
                uint16 suffix_wstr[MMIFMM_FILENAME_LEN + 1]        = {0};
                uint16 suffix_len = MMIFMM_FILENAME_LEN;
                MMIAPIFMM_SplitFileName(power_on_off_anim_info.more_anim_info.name_wstr, 
                    power_on_off_anim_info.more_anim_info.name_wstr_len, 
                    PNULL, PNULL, suffix_wstr, &suffix_len);
                if(MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_ConvertFileType(suffix_wstr,suffix_len))
                {
                    PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
                }
                else
                {
#ifdef MMI_POWERONOFFMP4_ENABLE
                    MMIAPIVP_ExitPlay();
#else
                    //if(s_is_play_default)//cr151078
                    //{
                    PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
                    //}
#endif
                }

            }
            else if(MMISET_POWER_ON_OFF_ANIM_DEFAULT == power_on_off_anim_info.anim_type)
            {
#ifdef MMI_POWERONOFFMP4_ENABLE
                MMIAPIVP_ExitPlay();
#else
#if 0
                if(!MMIAPIPHONE_IsOperatorLogoRingDisplay())
#endif
                {
                    PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
                }
#endif
            }
#else
            PlayPowerOnOffMP4CallBack();
#endif
            
        }
#if 0
		else if(s_power_off_anim_timer_id == *((uint8 *) param))
		{
			MMK_StopTimer(s_power_off_anim_timer_id);
			s_power_off_anim_timer_id = 0; 
			DisplaySecondAnim(PHONE_POWER_OFF_ANIMATEBOX_CTRL_ID, IMAGE_POWER_OFF);//1??úμú?t???ˉ?-×ê?′
		}
#endif
#if 0
        else if(s_operator_logo_power_off_timer_id == *((uint8 *) param))
        {
            MMK_StopTimer(s_operator_logo_power_off_timer_id);
            s_operator_logo_power_off_timer_id = 0; 
            PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
        }
#endif
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_GET_FOCUS:  
        MMK_CloseWin(win_id);        
        break;
        
    case MSG_LOSE_FOCUS: 
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetMainLayerType(MMITHEME_GetMainLayerDefaultType());
#endif
#ifdef VIDEO_PLAYER_SUPPORT
       MMIAPIVP_ExitPlay();
#endif
        break;
        
    case MSG_CLOSE_WINDOW:
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetMainLayerType(MMITHEME_GetMainLayerDefaultType());
#endif
        // 关闭关机窗口定时器
        if (s_power_off_timer_id != 0)
        {
            MMK_StopTimer(s_power_off_timer_id);
            s_power_off_timer_id = 0;
        }

#ifdef VIDEO_PLAYER_SUPPORT
        MMIAPIVP_ExitPlay();
#endif
#if defined(MMI_POWERONOFFMP4_ENABLE)
		if (PNULL != s_poweronoff_buffer_ptr)
		{
			SCI_FREE(s_poweronoff_buffer_ptr);
			s_poweronoff_buffer_ptr = PNULL;
		}
#endif
        //关机
        MMIDEFAULT_TurnOffBackLight();
        MMIDEFAULT_SetKeybadBackLight(FALSE);
        //any way
        //MMIDEFAULT_EnableKeyRing(TRUE);
        //MMIDEFAULT_EnableTpRing(TRUE);
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        //为了消除耳机模式情况下出现的pu音
        MMIAPISET_StopAllRing(TRUE);
        /* 睡眠会导致adc读不出来，先禁止睡眠 */
        SCI_MMI_EnableDeepSleep(0);
        SCI_Sleep(2000);
#ifdef ENG_SUPPORT
        MMIAPIENG_SaveLastTime();
#endif
        POWER_PowerOff();
        break;
        
#ifdef LCD_SLIDE_SUPPORT
    case MSG_KEYDOWN_SLIDE:
    case MSG_KEYUP_SLIDE:
#endif
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
    case MSG_APP_RED:
    case MSG_KEYLONG_RED:
	case MSG_KEYUP_RED:
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : Play mpeg4 as power on off animation.
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: Modified by apple.zhang
/*****************************************************************************/
LOCAL BOOLEAN PlayPowerOnOffMP4(BOOLEAN is_power_on_off)    //TRUE:power on, FALSE:power off
{
#ifdef VIDEO_PLAYER_SUPPORT   
    MMIAUD_RING_DATA_INFO_T         file_info = {0};/*lint !e64*/ 
    uint8                           power_midi_volume = MMISET_VOL_ZERO;
    VP_PLAY_PARAM_T                     play_param  = {0};
    MMISET_POWER_ON_OFF_ANIM_INFO_T power_on_off_anim_info = {0};
    GUI_RECT_T                     full_rect = {0};
   

    s_is_play_power_on_mp4 = is_power_on_off;
    //SCI_TRACE_LOW:"[POWERONOFF]:PlayPowerOnOffMP4:is_power_on=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_ONOFF_970_112_2_18_2_45_26_225,(uint8*)"d", is_power_on_off);
    
    SCI_MEMSET(&file_info, 0, sizeof(MMIAUD_RING_DATA_INFO_T));  
    SCI_MEMSET(&power_on_off_anim_info,0,sizeof(MMISET_POWER_ON_OFF_ANIM_INFO_T));
    
    MMIAPISET_GetPowerOnOffAnimInfo(
        &power_on_off_anim_info, 
        is_power_on_off
        );
    
    if(0 == MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1,POWER_RING_SWITCH))
    {
        power_midi_volume = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_POWER);
    }
    MMIAPIPHONE_SetPowerAnimMpeg4Flag(TRUE);

    SCI_MEMSET(&play_param,0,sizeof(VP_PLAY_PARAM_T));
    play_param.dual_sys = MN_DUAL_SYS_1;
    play_param.is_random_play = FALSE;
    play_param.mode = VP_REPEAT_PLAY_MODE_CLOSE;
    play_param.url_ptr = PNULL;
    play_param.display_param.audioSupport = 1;
    play_param.display_param.videoSupport = 1;
    play_param.is_scal_up = TRUE;
    play_param.display_param.RotateAngle = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID); 
    full_rect = MMITHEME_GetFullScreenRect();

    if (LCD_ANGLE_0 == play_param.display_param.RotateAngle)
    {
        play_param.display_param.RotateAngle = LCD_ANGLE_90;
        play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;           
        play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
    }
    else if(LCD_ANGLE_180 == play_param.display_param.RotateAngle)
    {
        play_param.display_param.RotateAngle = LCD_ANGLE_270;
        play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;           
        play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
    }
    else
    {
        play_param.display_param.disp_rect.dx = full_rect.right - full_rect.left + 1;           
        play_param.display_param.disp_rect.dy = full_rect.bottom - full_rect.top + 1;
    }
                
    

    play_param.display_param.target_rect.x = play_param.display_param.disp_rect.x;
    play_param.display_param.target_rect.y = play_param.display_param.disp_rect.y;
    play_param.display_param.target_rect.dx = play_param.display_param.disp_rect.dx;
    play_param.display_param.target_rect.dy = play_param.display_param.disp_rect.dy;

    play_param.display_param.disp_mode = DPLAYER_DISP_CUSTOMIZE;

    switch (power_on_off_anim_info.anim_type)
    {
    case MMISET_POWER_ON_OFF_ANIM_DEFAULT:
#if defined(MMI_POWERONOFFMP4_ENABLE)
        MMI_GetRingInfo(OPEN_MP4, &file_info); 
        play_param.play_type = VP_PLAY_FROM_BUFFER;
        play_param.video_buf_info.type = MMICOM_VIDEO_TYPE_3GP;
        play_param.video_buf_info.video_buf_len = file_info.data_len;
        play_param.video_buf_info.video_buf_ptr = SCI_ALLOCA(file_info.data_len);
		if (PNULL == play_param.video_buf_info.video_buf_ptr)
		{
			PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
			break;
		}
		SCI_MEMSET(play_param.video_buf_info.video_buf_ptr,0,file_info.data_len);
		SCI_MEMCPY(play_param.video_buf_info.video_buf_ptr,file_info.data_ptr,file_info.data_len);
		s_poweronoff_buffer_ptr = play_param.video_buf_info.video_buf_ptr;    
        play_param.vol = power_midi_volume;
        if(!MMIAPIVP_Play(&play_param, HandleVideoPowerOnOffMsg, 0))
        {
            PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
        }
#endif
        break;


    case MMISET_POWER_ON_OFF_ANIM_MORE:
    case MMISET_POWER_ON_OFF_PIC_MORE:
        if(MMIAPIFMM_IsFileExist(power_on_off_anim_info.more_anim_info.name_wstr,power_on_off_anim_info.more_anim_info.name_wstr_len))
        {   
            uint16 suffix_wstr[MMIFMM_FILENAME_LEN + 1]        = {0};
            uint16 suffix_len = MMIFMM_FILENAME_LEN;
            MMIAPIFMM_SplitFileName(power_on_off_anim_info.more_anim_info.name_wstr, 
                power_on_off_anim_info.more_anim_info.name_wstr_len, 
                PNULL, PNULL, suffix_wstr, &suffix_len);
			//bug 1247143
            //if(MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_ConvertFileType(suffix_wstr,suffix_len))
            if(MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_ConvertFileType(suffix_wstr,suffix_len)
#ifdef DRM_SUPPORT
                 || MMIFMM_FILE_TYPE_DRM == MMIAPIFMM_ConvertFileType(suffix_wstr,suffix_len)
#endif
                )				
            {
                PlayMorePowerOnOffAnimation(is_power_on_off, &power_on_off_anim_info);
            }
            else
            {
                play_param.play_type = VP_PLAY_FROM_FILE;
                play_param.full_path_len = power_on_off_anim_info.more_anim_info.name_wstr_len;
                play_param.full_path_ptr = power_on_off_anim_info.more_anim_info.name_wstr; 
                play_param.vol = power_midi_volume;
                //s_is_play_default = FALSE;
                if(!MMIAPIVP_Play(&play_param, HandleVideoPowerOnOffMsg, 0))
                {
                    PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
                }  
            }
        }
        else
        {
            #if defined(MMI_POWERONOFFMP4_ENABLE)
            MMI_GetRingInfo(OPEN_MP4, &file_info); 
            play_param.play_type = VP_PLAY_FROM_BUFFER;
            play_param.video_buf_info.type = MMICOM_VIDEO_TYPE_3GP;
            play_param.video_buf_info.video_buf_len = file_info.data_len;
			play_param.video_buf_info.video_buf_ptr = SCI_ALLOCA(file_info.data_len);
			if (PNULL == play_param.video_buf_info.video_buf_ptr)
			{
				PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
				break;
			}
			SCI_MEMSET(play_param.video_buf_info.video_buf_ptr,0,file_info.data_len);
			SCI_MEMCPY(play_param.video_buf_info.video_buf_ptr,file_info.data_ptr,file_info.data_len);
			s_poweronoff_buffer_ptr = play_param.video_buf_info.video_buf_ptr;    
              
            play_param.vol = power_midi_volume;
            if(!MMIAPIVP_Play(&play_param, HandleVideoPowerOnOffMsg, 0))
            {
                PlayPowerOnOffMP4CallBack(VP_PLAY_END_END_SUCCESS);
            }            
            #endif
        }
        break;

    default:
        //SCI_TRACE_LOW:"[POWERONOFF]:PlayPowerOnOffMP4:anim_type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_ONOFF_1086_112_2_18_2_45_27_226,(uint8*)"d", power_on_off_anim_info.anim_type);
        break;
    }
    
#endif
    return TRUE;

}
#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : call back function of PlayPowerOnOffMP4
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
void PlayPowerOnOffMP4CallBack(VP_PLAY_END_RESULT_E result)
{   
    //SCI_TRACE_MID:"mmiphone_onoff.c, PlayPowerOnMP4CallBack result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_ONOFF_1101_112_2_18_2_45_27_227,(uint8*)"d",result);

    /*是否处理待定*/
    //*
#if defined(MMI_POWERONOFFMP4_ENABLE)
	if (PNULL != s_poweronoff_buffer_ptr)
	{
		SCI_FREE(s_poweronoff_buffer_ptr);
		s_poweronoff_buffer_ptr = PNULL;
	}
#endif
    if(VP_PLAY_END_ERROR == result)
    {
        MMIAPISET_RetPowerOnOffAnimInfo(s_is_play_power_on_mp4);
    }
    
    if (s_is_play_power_on_mp4)
    {        
        MMK_CloseWin(PHONE_STARTUP_NORMAL_WIN_ID);
        
        if(!MMIAPIIDLE_IdleWinIsOpen())
        {
            #ifdef FLASH_SUPPORT
            if(MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)||//pin 码
               MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID)||//PIN Block
               MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID)||//puk 码
               MMK_IsOpenWin(MMIMSD_INIT_INPUT_PSW_WIN_ID)||//防盗
               MMK_IsOpenWin(MMIUDISK_USB_OPER_SELECT_WIN_ID)//USB
           #ifdef MMI_SIM_LOCK_SUPPORT
               ||MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
               ||MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
           #endif      
               )
            {
                if(MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)||//pin 码
                    MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID)||//PIN Block
                    MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID)//puk 码
           #ifdef MMI_SIM_LOCK_SUPPORT
                           ||MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
                           ||MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
           #endif      
                    )
                {
                    //如果需要输入pin码，那么此处将不进入IDLE界面
                    //由pin码输入确认后在进入IDLE
                    MMIAPIPHONE_SetOpenIdleFlag(TRUE);
                }
                
                if(MMK_IsOpenWin(MMIMSD_INIT_INPUT_PSW_WIN_ID))//防盗
                {
                   MMIAPIPHONE_SetScrDelCallBack(MMIMSD_INIT_INPUT_PSW_WIN_ID, MMIAPIPHONE_AtemptEnterIdleForLock);  
                }
                if(MMK_IsOpenWin(MMIUDISK_USB_OPER_SELECT_WIN_ID))//USB
                {
                   MMIAPIPHONE_SetScrDelCallBack(MMIUDISK_USB_OPER_SELECT_WIN_ID, MMIAPIPHONE_AtemptEnterIdleForUsb);  
                }
            }
            else
            {
               MMIAPIIDLE_OpenIdleWin();//无特殊窗口，正常进入IDLE
            }
            #else
            MMIAPIIDLE_OpenIdleWin();
            #endif
        }
        
        MMI_FreeRes(VIRTUAL_WIN_ID);//释放开始动画资源
    }
    else
    {
        MMIDEFAULT_TurnOffBackLight();
        MMIDEFAULT_SetKeybadBackLight(FALSE);
        MMK_CloseWin(PHONE_POWER_OFF_WIN_ID);
    }
    //MMI_Enable3DMMI(TRUE);
}

/*****************************************************************************/
//  Description : HandleVideoPowerOnOffMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void HandleVideoPowerOnOffMsg(VP_CALLBACK_T *msg_ptr)
{
    //VP_PLAY_END_IND_T  *play_end_ind_ptr = PNULL;
    VP_PLAY_END_RESULT_E play_end_result = 0;
    
    if (PNULL == msg_ptr)
    {
        return;
    }

    //SCI_TRACE_LOW:"[POWERONOFF]:HandleVideoPowerOnOffMsg msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_ONOFF_1185_112_2_18_2_45_27_228,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id) 
    {  
    case VP_STOP_IND: 
        play_end_result = VP_PLAY_END_END_SUCCESS;       
        PlayPowerOnOffMP4CallBack(play_end_result);
        break;   
  
    default:
        break;
    }
    
    
}
#else
/*****************************************************************************/
//  Description : call back function of PlayPowerOnOffMP4
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
void PlayPowerOnOffMP4CallBack(void)
{       
#if defined(MMI_POWERONOFFMP4_ENABLE)
	if (PNULL != s_poweronoff_buffer_ptr)
	{
		SCI_FREE(s_poweronoff_buffer_ptr);
		s_poweronoff_buffer_ptr = PNULL;
	}
#endif
    if (s_is_play_power_on_mp4)
    {        
        MMK_CloseWin(PHONE_STARTUP_NORMAL_WIN_ID);
        
        if(!MMIAPIIDLE_IdleWinIsOpen())
        {
            #ifdef FLASH_SUPPORT
            if(MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)||//pin 码
               MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID)||//PIN Block
               MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID)||//puk 码
               MMK_IsOpenWin(MMIMSD_INIT_INPUT_PSW_WIN_ID)||//防盗
               MMK_IsOpenWin(MMIUDISK_USB_OPER_SELECT_WIN_ID)//USB
           #ifdef MMI_SIM_LOCK_SUPPORT
               ||MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
               ||MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
           #endif      
               )
            {
                if(MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)||//pin 码
                    MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID)||//PIN Block
                    MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID)//puk 码
           #ifdef MMI_SIM_LOCK_SUPPORT
                       ||MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
                       ||MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
           #endif      
                    )
                {
                    //如果需要输入pin码，那么此处将不进入IDLE界面
                    //由pin码输入确认后在进入IDLE
                    MMIAPIPHONE_SetOpenIdleFlag(TRUE);
                }
                
                if(MMK_IsOpenWin(MMIMSD_INIT_INPUT_PSW_WIN_ID))//防盗
                {
                    MMIAPIPHONE_SetScrDelCallBack(MMIMSD_INIT_INPUT_PSW_WIN_ID, MMIAPIPHONE_AtemptEnterIdleForLock);  
                }
                if(MMK_IsOpenWin(MMIUDISK_USB_OPER_SELECT_WIN_ID))//USB
                {
                    MMIAPIPHONE_SetScrDelCallBack(MMIUDISK_USB_OPER_SELECT_WIN_ID, MMIAPIPHONE_AtemptEnterIdleForUsb);  
                }
            }
            else
            {
               MMIAPIIDLE_OpenIdleWin();//无特殊窗口，正常进入IDLE
            }
            #else
            MMIAPIIDLE_OpenIdleWin();
            #endif
        }
        
        MMI_FreeRes(VIRTUAL_WIN_ID);//释放开始动画资源
    }
    else
    {        
        MMIDEFAULT_TurnOffBackLight();
        MMIDEFAULT_SetKeybadBackLight(FALSE);
        MMK_CloseWin(PHONE_POWER_OFF_WIN_ID);
    }
    //MMI_Enable3DMMI(TRUE);
}
#endif


#ifndef MMI_POWERONOFFMP4_ENABLE
/*****************************************************************************/
//  Description : PlayDefaultPowerOnOffAnimation
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN PlayDefaultPowerOnOffAnimation(BOOLEAN is_power_on_off)    //TRUE:power on, FALSE:power off
{
    GUI_LCD_DEV_INFO                lcd_dev_info = {0};
    GUI_RECT_T                      rect = MMITHEME_GetFullScreenRect();
    MMI_WIN_ID_T                    win_id = 0;
    GUIANIM_INIT_DATA_T             anim_init = {0};
    MMI_CONTROL_CREATE_T            anim_ctrl = {0};
    GUIANIM_CTRL_INFO_T             ctrl_info = {0};
    GUIANIM_DATA_INFO_T             data_info = {0};
    GUIANIM_DISPLAY_INFO_T          display_info = {0};
    uint8 active_mode_value =MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1,POWER_RING_SWITCH);
    //SCI_TRACE_LOW:"[POWERONOFF]:PlayDefaultPowerOnOffAnimation active_mode_value=%d is_power_on_off=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_ONOFF_1285_112_2_18_2_45_27_229,(uint8*)"dd",active_mode_value,is_power_on_off);

    /* if power off, we should stop all the sound, and do not resume it. 
        so we request a virtual handl to stop them, and do not free the virtual handle */
    if(!is_power_on_off)
    {
        MMISRVAUD_ReqVirtualHandle("POWER OFF", MMISRVAUD_PRI_NORMAL);
    }
    if(active_mode_value==0)
    {
#ifdef CMCC_UI_STYLE
        if(is_power_on_off)
        {
            MMIAPISET_PlayRing(MN_DUAL_SYS_1,FALSE,0,1,MMISET_RING_TYPE_POWER,PNULL);
        }
        else
        {
            MMIAPISET_PlayRing(MN_DUAL_SYS_1,FALSE,0,1,MMISET_RING_TYPE_POWER_OFF,PNULL);
        }
#else     
        MMIAPISET_PlayRing(MN_DUAL_SYS_1,FALSE,0,1,MMISET_RING_TYPE_POWER,PNULL);
#endif
    }
    
    ctrl_info.is_ctrl_id = TRUE;

    //先填充黑色，再播放动画小图标
    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
    
    if(is_power_on_off)
    {
        win_id = PHONE_STARTUP_NORMAL_WIN_ID;
        ctrl_info.ctrl_id =PHONE_STARTUP_NORMAL_ANIMATEBOX_CTRL_ID;	//  MMK_GetCtrlHandleByWin(win_id, PHONE_STARTUP_NORMAL_ANIMATEBOX_CTRL_ID);
// #if defined (MMI_PDA_SUPPORT) && defined (MAINLCD_SIZE_320X480) // xiaoqing
//         data_info.anim_id = IMAGE_POWER_ON;
// #else
          data_info.img_id = IMAGE_TEMPORARY_BOOTUP;//IMAGE_CIRCULAR_LOADER_1;//IMAGE_TEMPORARY_BOOTUP;//IMAGE_POWER_ON;
//#endif
        
    }
    else
    {
        win_id = PHONE_POWER_OFF_WIN_ID;
        ctrl_info.ctrl_id = PHONE_POWER_OFF_ANIMATEBOX_CTRL_ID;
#ifdef MMI_ORTHODOX_VERSION
        data_info.img_id = IMAGE_WALLPAPER_1;
#else
        data_info.img_id = IMAGE_TEMPORARY_BOOTUP;//IMAGE_POWER_OFF;
#endif
        
    }
        display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
 //       display_info.is_handle_transparent = TRUE;
        display_info.is_play_once = TRUE;
        display_info.is_bg_buf      = TRUE;
        display_info.bg.bg_type     = GUI_BG_COLOR;
        display_info.bg.color       = MMI_WINDOW_BACKGROUND_COLOR;

	    display_info.is_need_end_ind = TRUE;//ó?óú2￥·￠ò?′??ˉ?-·￠?í???￠
      	display_info.is_update = FALSE; //2?á￠?′?üD?
    //set anim control init param
        anim_init.both_rect.h_rect = anim_init.both_rect.v_rect = MMITHEME_GetFullScreenRect();
//    anim_init.is_spinner = TRUE;

    //creat anim
    anim_ctrl.ctrl_id           = ctrl_info.ctrl_id;
    anim_ctrl.guid              = SPRD_GUI_ANIM_ID;
    anim_ctrl.init_data_ptr     = &anim_init;
    anim_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&anim_ctrl);
    
    GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);

    return TRUE;
}

#if 0
/*****************************************************************************/
//  Description : PlayOperatorPowerOnOffAnimation
//  Global resource dependence : 
//  Author: Cheney.Wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN PlayOperatorPowerOnOffAnimation(BOOLEAN is_power_on_off)    //TRUE:power on, FALSE:power off
{
    GUI_LCD_DEV_INFO                lcd_dev_info = {0};
    GUI_RECT_T                      rect = MMITHEME_GetFullScreenRect();

    MMI_WIN_ID_T                    win_id = 0;
    GUIANIM_INIT_DATA_T             anim_init = {0};
    MMI_CONTROL_CREATE_T            anim_ctrl = {0};
    GUIANIM_CTRL_INFO_T             ctrl_info = {0};
    GUIANIM_DATA_INFO_T             data_info = {0};
    GUIANIM_DISPLAY_INFO_T          display_info = {0};
    uint8 active_mode_value =MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1,POWER_RING_SWITCH);
    //SCI_TRACE_LOW:"[POWERONOFF]:PlayDefaultPowerOnOffAnimation active_mode_value=%d is_power_on_off=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_ONOFF_1285_112_2_18_2_45_27_229,(uint8*)"dd",active_mode_value,is_power_on_off);

    /* if power off, we should stop all the sound, and do not resume it. 
        so we request a virtual handl to stop them, and do not free the virtual handle */
    if(!is_power_on_off)
    {
        MMISRVAUD_ReqVirtualHandle("POWER OFF", MMISRVAUD_PRI_NORMAL);
    }
    if(active_mode_value==0)
    {
#ifdef CMCC_UI_STYLE
        if(is_power_on_off)
        {
            MMIAPISET_PlayRing(MN_DUAL_SYS_1,FALSE,0,1,MMISET_RING_TYPE_POWER,PNULL);
        }
        else
        {
            MMIAPISET_PlayRing(MN_DUAL_SYS_1,FALSE,0,1,MMISET_RING_TYPE_POWER_OFF,PNULL);
        }
#else        
		if(is_power_on_off)
        {
            MMIAPISET_PlayRing(MN_DUAL_SYS_1,FALSE,0,1,MMISET_RING_TYPE_POWER,PNULL);
        }
#endif
    }
    
    ctrl_info.is_ctrl_id = TRUE;
    
    //先填充黑色，再播放动画小图标
    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
    
    if(is_power_on_off)
    {
        win_id = PHONE_STARTUP_NORMAL_WIN_ID;
        ctrl_info.ctrl_id =PHONE_STARTUP_NORMAL_ANIMATEBOX_CTRL_ID;	//  MMK_GetCtrlHandleByWin(win_id, PHONE_STARTUP_NORMAL_ANIMATEBOX_CTRL_ID);
        data_info.img_id = IMAGE_PROMO_OPERATOR_LOGO;//IMAGE_CIRCULAR_LOADER_1;//IMAGE_TEMPORARY_BOOTUP;//IMAGE_POWER_ON;
    }
    else
    {
        win_id = PHONE_POWER_OFF_WIN_ID;
        ctrl_info.ctrl_id = PHONE_POWER_OFF_ANIMATEBOX_CTRL_ID;
        data_info.img_id = IMAGE_PROMO_OPERATOR_LOGO;//IMAGE_POWER_OFF;
    }
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_play_once = FALSE;
    display_info.is_bg_buf      = TRUE;
    display_info.bg.bg_type     = GUI_BG_COLOR;
    display_info.bg.color       = MMI_WINDOW_BACKGROUND_COLOR;
    
    display_info.is_need_end_ind = TRUE;//ó?óú2￥·￠ò?′??ˉ?-·￠?í???￠
    display_info.is_update = TRUE; //2?á￠?′?üD?
    anim_init.both_rect.h_rect = anim_init.both_rect.v_rect = MMITHEME_GetFullScreenRect();
    
    //creat anim
    GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
    return TRUE;
}
#endif

LOCAL BOOLEAN DisplaySecondAnim(
								 MMI_CTRL_ID_T  ctrl_id,
								 MMI_IMAGE_ID_T  image_id 
								 )   
{
    GUI_LCD_DEV_INFO                lcd_dev_info = {0};
    GUI_RECT_T                      rect = MMITHEME_GetFullScreenRect();
    GUIANIM_INIT_DATA_T             anim_init = {0};
    MMI_CONTROL_CREATE_T            anim_ctrl = {0};
    GUIANIM_CTRL_INFO_T             ctrl_info = {0};
    GUIANIM_DATA_INFO_T             data_info = {0};
    GUIANIM_DISPLAY_INFO_T          display_info = {0};
    ctrl_info.is_ctrl_id = TRUE;
	ctrl_info.ctrl_id = ctrl_id;
    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
    data_info.img_id = image_id;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_play_once = TRUE;
	display_info.is_need_end_ind = FALSE;//ó?óú2￥·￠ò?′??ˉ?-·￠?í???￠
	display_info.is_update = TRUE; //á￠?′?üD?
    anim_init.both_rect.h_rect = anim_init.both_rect.v_rect = MMITHEME_GetFullScreenRect();
    
    GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);

    return TRUE;
}
#endif
/*****************************************************************************/
//  Description : open normal start up window
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_OpenNormalStartUpWin(void)
{
    if (!MMK_IsOpenWin(MAIN_IDLE_WIN_ID))
    {
        MMK_CreateWin((uint32*)PHONE_STARTUP_NORMAL_WIN_TAB,PNULL);
    }
}
/*****************************************************************************/
//  Description : Set the mpeg4 playing flag as power anim.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetPowerAnimMpeg4Flag(
                    BOOLEAN is_playing
                    )
{
    s_is_playing_mp4 = is_playing;
}

/*****************************************************************************/
//  Description : Set the mpeg4 playing flag as power anim.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetPowerAnimMpeg4Flag(
                    void
                    )
{
    return s_is_playing_mp4;
}

/*****************************************************************************/
//  Description : get power on status when mp4 is playing
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetPowerOnStatus(void)
{
    return (s_is_play_power_on_mp4);
}

/*****************************************************************/
//  Description :play more power on or off animation
//  Global resource dependence : none
//  Author: jian.ma
//  Note: 
/*****************************************************************/
LOCAL BOOLEAN PlayMorePowerOnOffAnimation(
                                          BOOLEAN is_power_on_off,  //TRUE:power on, FALSE:power off
                                          MMISET_POWER_ON_OFF_ANIM_INFO_T *power_on_off_anim_info
                                          )  
{
    MMI_WIN_ID_T                    win_id = 0;
    GUIANIM_INIT_DATA_T             anim_init = {0};
    MMI_CONTROL_CREATE_T            anim_ctrl = {0};
    GUIANIM_CTRL_INFO_T             ctrl_info = {0};
    GUIANIM_FILE_INFO_T             file_info = {0};
    GUIANIM_DISPLAY_INFO_T          display_info = {0};
    GUI_LCD_DEV_INFO                lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    GUI_RECT_T                      rect = MMITHEME_GetFullScreenRect();//{0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};

    if(MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1,POWER_RING_SWITCH)==0)
    {
#ifdef CMCC_UI_STYLE
        if(is_power_on_off)
        {
            MMIAPISET_PlayRing(MN_DUAL_SYS_1,FALSE,0,1,MMISET_RING_TYPE_POWER,PNULL);
        }
        else
        {
            MMIAPISET_PlayRing(MN_DUAL_SYS_1,FALSE,0,1,MMISET_RING_TYPE_POWER_OFF,PNULL);
        }
#else        
        MMIAPISET_PlayRing(MN_DUAL_SYS_1,FALSE,0,1,MMISET_RING_TYPE_POWER,PNULL);
#endif
    }
    
    ctrl_info.is_ctrl_id = TRUE;

    LCD_FillRect((const GUI_LCD_DEV_INFO*)&lcd_dev_info, rect, MMI_WHITE_COLOR);
    
    file_info.full_path_wstr_ptr = power_on_off_anim_info->more_anim_info.name_wstr;
    file_info.full_path_wstr_len = power_on_off_anim_info->more_anim_info.name_wstr_len;

    if(is_power_on_off)
    {
        ctrl_info.ctrl_id = PHONE_STARTUP_NORMAL_ANIMATEBOX_CTRL_ID;
        win_id = PHONE_STARTUP_NORMAL_WIN_ID;
    }
    else
    {
        ctrl_info.ctrl_id = PHONE_POWER_OFF_ANIMATEBOX_CTRL_ID;
        win_id = PHONE_POWER_OFF_WIN_ID;
    }
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_play_once = FALSE;

    //set anim control init param
    anim_init.both_rect.h_rect = anim_init.both_rect.v_rect = MMITHEME_GetFullScreenRect();

    //creat anim
    anim_ctrl.ctrl_id           = ctrl_info.ctrl_id;
    anim_ctrl.guid              = SPRD_GUI_ANIM_ID;
    anim_ctrl.init_data_ptr     = &anim_init;
    anim_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&anim_ctrl);
    
    GUIANIM_SetParam(&ctrl_info,PNULL,&file_info,&display_info);
    GUIANIM_SetDefaultIcon(anim_ctrl.ctrl_id, IMAGE_NULL, IMAGE_NULL);

    return TRUE;
}

/*****************************************************************/
//  Description :window for warranty in first power on
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************/
LOCAL MMI_RESULT_E HandleWarrantyOptMenuWinMsg(
                                               MMI_WIN_ID_T     win_id,    // STARTUP NORMAL′°?úμ?ID
                                               MMI_MESSAGE_ID_E    msg_id,     // ′°?úμ??ú2????￠ID
                                               DPARAM              param       // ?àó|???￠μ?2?êy
                                               )
{
    MMI_RESULT_E  result  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MAIN_MAINMENU_OPT_CTRL_ID;
    uint32 node_id        = 0;

    SCI_TRACE_LOW("HandleWarrantyOptMenuWinMsg msg_id = 0x%x", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
        MMK_CreateWin((uint32*)PHONE_FIRST_POWER_PRIVACY_POLICY_WIN_TAB, PNULL);
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        MMIAPIPHONE_SetFirstPowerOnNV();
        break;
    case MSG_APP_RED:
		       break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
    case MSG_APP_UP:
    case MSG_APP_DOWN:
        break;

    default :
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description :显示服务条款界面text
//  Global resource dependence : 
//  Author:
//  Note:2015.6.20
/*****************************************************************************/
LOCAL void DisplayPrivacyPolicyScreenText(MMI_CTRL_ID_T  ctrl_id)
{
  	 GUIRICHTEXT_ITEM_T   item_data = {0};
     uint16      index  =  0;
     GUI_RECT_T   warranty_text_rect = {0};
     GUI_BG_T  	warranty_text_backgrand = {0};
     MMI_STRING_T  warranty_string = {0};
     GUI_FONT_T   warranty_text_font = CAF_FONT_14;  //TEXT??×?′óD?
     GUI_COLOR_T   warranty_text_color = MMI_WHITE_COLOR; //text??×???é?
     
     MMI_STRING_T string = {0};
     
     item_data.tag_type = GUIRICHTEXT_TAG_NORMAL;
     CTRLRICHTEXT_SetBorderFocusColor(ctrl_id, MMI_BLACK_COLOR);
     GUIRICHTEXT_SetFontColor(ctrl_id, warranty_text_color);
     
     item_data.text_type = GUIRICHTEXT_TEXT_RES;
     item_data.text_set_font =TRUE;
     item_data.setting_font = MMI_DEFAULT_NORMAL_FONT_SIZE; //×?ì?éè??ò?′?￡?oó????éúD§,medium
     item_data.text_data.res.id = STR_PRIVACY_POLICY_CONTENT_1;
     GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
     item_data.text_type = GUIRICHTEXT_TEXT_RES;
     item_data.text_data.res.id = STR_PRIVACY_POLICY_CONTENT_2;
     GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
     item_data.text_type = GUIRICHTEXT_TEXT_RES;
     item_data.text_data.res.id = STR_PRIVACY_POLICY_CONTENT_3;
     GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
     item_data.text_type = GUIRICHTEXT_TEXT_RES;
     item_data.text_data.res.id = STR_PRIVACY_POLICY_CONTENT_4;
     GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
     item_data.text_type = GUIRICHTEXT_TEXT_RES;
     item_data.text_data.res.id = STR_PRIVACY_POLICY_CONTENT_5;
     GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
     item_data.text_type = GUIRICHTEXT_TEXT_RES;
     item_data.text_data.res.id = STR_PRIVACY_POLICY_CONTENT_6;
     GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
     item_data.text_type = GUIRICHTEXT_TEXT_RES;
     item_data.text_data.res.id = STR_PRIVACY_POLICY_CONTENT_7;
     GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
     item_data.text_type = GUIRICHTEXT_TEXT_RES;
     item_data.text_data.res.id = STR_PRIVACY_POLICY_CONTENT_8;
     GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
     
}

/*****************************************************************/
//  Description :window for warranty in first power on
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************/
LOCAL MMI_RESULT_E HandlePrivacyPolicyWindow(
                                             MMI_WIN_ID_T     win_id,    // STARTUP NORMAL′°?úμ?ID
                                             MMI_MESSAGE_ID_E    msg_id,     // ′°?úμ??ú2????￠ID
                                             DPARAM              param       // ?àó|???￠μ?2?êy
                                             )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   bg_rect = {0};
    GUI_BOTH_RECT_T both_client_rect = MMITHEME_GetWinClientBothRect(win_id);	 
    MMI_CTRL_ID_T  ctrl_id = PHONE_PRIVACY_POLICY_RICHTEXT_CTRL_ID;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(MMITHEME_IsMainScreenLandscape())
        {
            bg_rect = both_client_rect.h_rect;
        }
        else
        {
            bg_rect = both_client_rect.v_rect;
        }
        GUI_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR);
        DisplayPrivacyPolicyScreenText(ctrl_id);
        GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_NONE);
        //GUIRICHTEXT_SetFocusItem(ctrl_id, 0);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_FULL_PAINT:
       	if(MMITHEME_IsMainScreenLandscape())
        {
            bg_rect = both_client_rect.h_rect;
        }
        else
        {
            bg_rect = both_client_rect.v_rect;
        }
        GUI_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR); 
        break;	
    case MSG_GET_FOCUS:
        MMIAPICOM_StatusAreaIdlescreenSet();
        break;
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_NOTIFY_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif
        //        MMK_CloseWin(win_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
	
    case MSG_APP_RED:
		       break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
 
    case MSG_CLOSE_WINDOW:
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : 判断是否首次开机
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_IsFirstPowerOn(void)
{ 
    BOOLEAN				is_first_power_on = FALSE;
    MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
    
	MMINV_READ(MMINV_PHONE_FIRST_POWER_ON, &is_first_power_on, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_first_power_on = TRUE;
        MMINV_WRITE(MMINV_PHONE_FIRST_POWER_ON, &is_first_power_on);
    }

	return is_first_power_on;
}
