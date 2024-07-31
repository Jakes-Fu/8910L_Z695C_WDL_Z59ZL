/*****************************************************************************
** File Name:      mmicom_statusarea.h                                       *
** Author:                                                                   *
** Date:           11/07/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2011        jian.ma              Creat
******************************************************************************/
#ifndef _MMICOM_STATUSAREA_H_
#define _MMICOM_STATUSAREA_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_font.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guistring.h"
#include "mn_type.h"
#include "mmi_signal_ext.h"
#include "mmipdp_export.h"
#include "mmiidle_func.h"
#include "guistatusbar_scrollmsg.h"
#include "dualbat_drvapi.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef enum
{
    //øø◊Ûœ‘ 
#ifdef STATUS_TEXT_SUPPORT
    MMICOM_WIN_TEXT_INFO,          // view header
#endif

#ifdef MMI_VOLTE_SUPPORT
    MMICOM_WIN_ICON_VOLTE,        // MAIN LCD VOLTE
#endif
    MMICOM_WIN_ICON_SIGNAL,        // MAIN LCD sim1–≈∫≈¡øÃ· æ
#ifndef MMI_MULTI_SIM_SYS_SINGLE
#ifdef MMI_VOLTE_SUPPORT
    MMICOM_WIN_ICON_VOLTE_2,
#endif
    MMICOM_WIN_ICON_SIGNAL_2,        //MAIN LCD sim2–≈∫≈¡øÃ· æ
#endif

#if !defined CMCC_UI_STYLE && !defined NET_ICON_UI_STYLE
#ifdef PRODUCT_DM
    //  MMICOM_WIN_ICON_GPRS,          // 3G gprs∑˛ŒÒø™∆Ù
#endif
#endif

    MMICOM_WIN_ICON_CALLING,       // MAIN LCD’˝‘⁄Õ®ª∞÷–
    MMICOM_WIN_ICON_MISSED_CALL,   // MAIN LCDŒ¥Ω”¿¥µÁ
    MMICOM_WIN_ICON_MESSAGE,       // MAIN LCD∂Ã–≈Ã· æ
    MMICOM_WIN_ICON_VOICEMAIL,       // MAIN LCD VoicemailÃ· æ
    MMICOM_WIN_ICON_CALL_FORWARD,  // MAIN LCD∫ÙΩ–◊™“∆ 

#if defined(TORCH_SUPPORT)
    MMICOM_WIN_ICON_TORCH,         // Torch ICON
#endif
    
    //øø”“œ‘ æ
    MMICOM_WIN_ICON_USB_MODE,         // MAIN LCD Stopwatch running
#ifdef BLUETOOTH_SUPPORT
    MMICOM_WIN_ICON_BLUE_TOOTH,    // MAIN LCD Accessory connected(headset)
#endif
    MMICOM_WIN_ICON_STOPWATCH,         // MAIN LCD Stopwatch running
    MMICOM_WIN_ICON_COUNTDOWN_TIMER,         // MAIN LCD Countdown Timer(when is set)
    MMICOM_WIN_ICON_CALENDAR_REMINDER,         // MAIN LCD Calendar Reminder(Reminder snoozed)
    MMICOM_WIN_ICON_ALARM,         // MAIN LCDƒ÷÷”Ã· æ
    MMICOM_WIN_ICON_ENVSET,        // MAIN LCD«Èæ∞ƒ£ Ω:æ≤“Ù,¡Â…˘,’Ò∂ØÃ· æ

    MMICOM_WIN_ICON_APP_NOTICE,   // MAIN LCD General application notification

    MMICOM_WIN_ICON_ROAMING,       // MAIN LCD¬˛”ŒÃ· æ
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMICOM_WIN_ICON_ROAMING_2,   // MAIN LCD sim2¬˛”ŒÃ· æ
#endif   

    MMICOM_WIN_ICON_EARPHONE,      // MAIN LCD∂˙ª˙Ã· æ

    MMICOM_WIN_ICON_BATTERY,       // MAIN LCDµÁ≥ÿÃ· æ
    MMICOM_WIN_TEXT_TIME,          // MAIN LCD ±º‰
    // end
    MMICOM_WIN_ICON_MAX
}
MMICOM_WIN_ICON_E;
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// Description : Status area init
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaInit(void);

/*****************************************************************************/
//  Description :  …Ë÷√Œƒ±æƒ⁄»›
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: Œƒ±æƒ⁄»›Œ™ ±º‰
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetDispTime(void);//PUBLIC void MAIN_SetStbDispTime(void);

/*****************************************************************************/
//  Description :  ªÒ»°◊¥Ã¨¿∏timer «∑Òø…º˚
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: Œƒ±æƒ⁄»›Œ™ ±º‰
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_StatusAreaIsDispTime(void);//PUBLIC BOOLEAN MAIN_IsStbDispTime(void);

/*****************************************************************************/
//  Description :  …Ë÷√◊¥Ã¨¿∏ «∑Òœ‘ ætimer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: Œƒ±æƒ⁄»›Œ™ ±º‰
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetTimeVisible(//PUBLIC void MAIN_SetStbTimeVisible(
    BOOLEAN is_true
);
/*****************************************************************************/
//  Description :  set status bar time info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_StatusAreaSetDispTimeinfo(wchar *time_buf, uint16 time_len); //PUBLIC BOOLEAN MAIN_SetStbDispTimeinfo(wchar *time_buf,uint16 time_len);

/*****************************************************************************/
// Description : ªÒµ√ ±º‰◊÷∑˚¥Æ
// Global resource dependence :
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaGetSysTime(//PUBLIC void MAIN_StatusbarGetSysTime(
    wchar *time_wstr,
    uint16 wlen
    );

/*****************************************************************************/
//  Description : set msg state
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetMsgState(BOOLEAN is_draw);//PUBLIC void MAIN_SetMsgState(BOOLEAN is_draw);

/*****************************************************************************/
//  Description : set the call forward state of the idle window
//  Global resource dependence :
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetForwardState(//PUBLIC void MAIN_SetIdleForwardState(
    MN_DUAL_SYS_E dual_sys,
    BOOLEAN forward_state
    );

/*****************************************************************************/
//  Description : set the Alarm state of the idle window
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaUpdateEarPhoneState(void);

/*****************************************************************************/
//  Description : update the envset state of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaUpdateEnvsetState(void);//PUBLIC void MAIN_UpdateIdleEnvsetState(void);

/*****************************************************************************/
//  Description : set the roaming state of the idle window
//  Global resource dependence :
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetRoamingState(//PUBLIC void MAIN_SetIdleRoamingState(
    MN_DUAL_SYS_E	e_dual_sys,
    BOOLEAN is_roaming
    );

/*****************************************************************************/
// Description : œ‘ æÕº±Í
// Global resource dependence :
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetGprsInfo(//PUBLIC void MMIAPIIDLE_SetGprsInfo(
    MN_DUAL_SYS_E dual_sys,
    MMIPDP_NOTIFY_EVENT_E pdp_event,
    BOOLEAN is_append
    );
/*****************************************************************************/
//  Description : set the gprs state of the idle window
//  Global resource dependence :
//  Author: figo.feng
//  Note: DUAL_MODEœ¬÷ª◊ˆπÿ±’Õº±Í”√
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetGprsState(//PUBLIC void MAIN_SetIdleGprsState(
    MN_DUAL_SYS_E    e_dual_sys,
    BOOLEAN          gprs_state
    );
/*****************************************************************************/
//  Description : set the rx level of the idle window
//    is_direct:TRUE÷±Ω”œ‘ æ–≈∫≈Ã¯±‰£ªFALSE∆Ωª¨œ‘ æ–≈∫≈Ã¯±‰
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetRxLevel(MN_DUAL_SYS_E e_dual_sys, uint8 rx_level, BOOLEAN is_direct);//PUBLIC void MAIN_SetIdleRxLevel(MN_DUAL_SYS_E e_dual_sys, uint8 rx_level, BOOLEAN is_direct);

/*****************************************************************************/
//  Description : get the rx level
//
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICOM_StatusAreaGetRxLevel(MN_DUAL_SYS_E e_dual_sys);//PUBLIC uint8 MMIMAIN_GetRxLevel(MN_DUAL_SYS_E e_dual_sys);

/*****************************************************************************/
//  Description : set the battery level of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetBatLevel(//PUBLIC void MAIN_SetIdleBatLevel(
    uint8 bat_level
    );

/*****************************************************************************/
//  Description : set the charge state
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetChargeState(//PUBLIC void MAIN_SetIdleChargeState(
    BOOLEAN is_charge
    );

/*****************************************************************************/
//  Description : set the blue tooth state of when power on
//  Global resource dependence :
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetBTStateWhenPoweron(//PUBLIC void MAIN_SetIdleBTStateWhenPowerOn(
    BLUETOOTH_STATE_TYPE  bluetooth_state
    );

/*****************************************************************************/
//  Description : set the blue tooth state of the idle window
//  Global resource dependence :
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetBlueToothState(//PUBLIC void MAIN_SetIdleBlueToothState(
    BLUETOOTH_STATE_TYPE  bluetooth_state
    );
/*****************************************************************************/
//  Description : set the charge state
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetNetWorkName(void);//PUBLIC void MAIN_SetIdleNetWorkName(void);

/*****************************************************************************/
//  Description : set the charge state
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaUpdateSignalState(void);//PUBLIC void MAIN_UpdateIdleSignalState(void);
#ifdef MMI_VOLTE_SUPPORT
PUBLIC void MMIAPICOM_StatusAreaSetIdleVolteState(MN_DUAL_SYS_E dual_sys);
#endif
/*****************************************************************************/
//  Description : set missed call state
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetCallingState(BOOLEAN is_calling);//PUBLIC void MAIN_SetCallingState(BOOLEAN is_calling);

/*****************************************************************************/
//  Description : set missed call state
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetMissedCallState(BOOLEAN is_missed_call);//PUBLIC void MAIN_SetMissedCallState(BOOLEAN is_missed_call);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetAllAppCallbackValid(//PUBLIC void MAIN_SetAllStbAppCallbackValid(
    BOOLEAN is_valid
    );

/*****************************************************************************/
//  Description : set the Alarm state of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetAlarmState(//PUBLIC void MAIN_SetIdleAlarmState(
    BOOLEAN alarm_state
    );


/*****************************************************************************/
//  Description : set the java state of the idle window
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaUpdateJavaState(BOOLEAN java_state);//PUBLIC void MAIN_UpdateIdleJavaState(BOOLEAN java_state);

/*****************************************************************************/
//  Description : set the vibrator state of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetShakeState(//PUBLIC void MAIN_SetIdleShakeState(
    BOOLEAN shake_state
    );

/*****************************************************************************/
//  Description : if is silent of call ring in the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetSlientState(//PUBLIC void MAIN_SetIdleSilentState(
    BOOLEAN silent_state
    );

/*****************************************************************************/
//  Description : …Ë÷√–¬”Ô“Ùœ˚œ¢Õº±Í
//  Global resource dependence :
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetVMFlagState( BOOLEAN vm_status);//PUBLIC void MAIN_SetIdleVMFlagState( BOOLEAN vm_status);

/*****************************************************************************/
//  Description : set the Stopwatch running state of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetStopwatchRunningState(
    BOOLEAN stopwatch_running_state
    );

/*****************************************************************************/
//  Description : set the Countdown Timer(when is set) state of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetCountdownTimerState(
    BOOLEAN countdown_timer_state
    );

/*****************************************************************************/
//  Description : set the Calendar Reminder(Reminder snoozed) state of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetReminderState(
    BOOLEAN reminder_state
    );


/*****************************************************************************/
//  Description : set the General Application Notification state of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetAppNoticeState(
    BOOLEAN app_notice_state
    );

/*****************************************************************************/
//  Description : set the usb mode of the idle window
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetUSBConnectedState(
    BOOLEAN usb_connected_state
    );

#if defined(TORCH_SUPPORT)
/*****************************************************************************/
//  Description : set the td state of the idle window
//  Global resource dependence : 
//  Author: Cheney.Wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StatusAreaSetTorchState(BOOLEAN is_torch_on);
#endif

/*****************************************************************************/
//  Description :  set status bar viewheader info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_StatusAreaSetViewHeaderinfo(wchar *item_str, uint16 item_len);

/*****************************************************************************/
//  Description :  set status bar viewheader info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(MMI_TEXT_ID_T text_id);

//Sam.hua
//Original status header info update function
PUBLIC BOOLEAN MMIAPICOM_StatusAreaSetViewHeaderinfoEx(wchar *item_str, uint16 item_len);
PUBLIC BOOLEAN MMIAPICOM_StatusAreaSetViewHeaderinfoByTextIdEx(MMI_TEXT_ID_T text_id);


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
PUBLIC void MMIAPICOM_StatusAreaLockscreenSet(void);

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
PUBLIC void MMIAPICOM_StatusAreaIdlescreenSet(void);

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
PUBLIC void MMIAPICOM_StatusAreaWithheaderSet(void);
/*****************************************************************************/
//  Description :  get rat icon table info
//  Global resource dependence :
//  Author:zhongping.shi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsExistInRatIconTable(MN_PLMN_T * cur_plmn_ptr, char * icon_2Gtype, char * icon_4Gtype);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif
