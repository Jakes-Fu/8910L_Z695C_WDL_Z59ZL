/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "ajsc_platform.h"
#include "kjava_sys_device.h"
#include "mmijava_internal.h"
#include "jkmediatone.h"


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL SCI_TIMER_PTR s_vibration_timer = SCI_NULL;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL void Vibrationtimer_func(void);


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/**
* Alerts the user by playing a sound.
*
* This function corresponds to the javax.microedition.lcdui.AlertType 
* class playSound() method. 
*
* \param alterType The type of sound used to alert the user.
*
*                  One of the following is designated.
*                  <ul>
*                      <li>KJAVA_DEVICE_ALERT_TYPE_ALARM</li>
*                      <li>KJAVA_DEVICE_ALERT_TYPE_CONFIRM</li>
*                      <li>KJAVA_DEVICE_ALERT_TYPE_ERROR</li>
*                      <li>KJAVA_DEVICE_ALERT_TYPE_INFO</li>
*                      <li>KJAVA_DEVICE_ALERT_TYPE_WARNING</li>
*                  </ul>
*
* \return if operation is successful, returns 1. Otherwise (e.g. sound support 
*         is lacking or the sound resource is in use, etc), returns 0.
*/
int kjava_device_alertBySound(int alertType)
{
    JWE_LOG(kjava_device_alertBySound, ("[kjava_device_alertBySound] (alertType) = (%d)", alertType));        
    JkMediaTonePlayNote(alertType*2 + 60, 500, 100);
    return 1;
}


/**
* Terminates the alert sound. 
*
* Stop the alert sound started by JkDeviceAlertBySound(). 
* If the sound is not playing when this function is called, exit this 
* function without doing anything. 
*/
void kjava_device_stopAlertSound(void)
{
    JWE_LOG(kjava_device_stopAlertSound, ("[kjava_device_stopAlertSound] enter."))        
    JkMediaToneStopAllNotes();
}


/**
* Starts backlight flashing.
*
* Flash the backlight for the designated duration. The blink cycle 
* (durations of on and off periods) is platform-dependent.
*
* If this function is called while the backlight is flashing, 
* cancel the previous flashing duration designation and flash for 
* the newly designated duration, starting from the instant this function 
* was called. Enable the flashing to be stopped forcibly when 
* kjava_device_stopBacklightFlash() is called. 
*
* This function is called when the flash duration is designated as a 
* parameter in the javax.microedition.lcdui.Display class flashBacklight() 
* method. 
*
* \param duration  The duration (in milliseconds) of backlight flashing  
*
* \return if the operation succeeds, returns 1. If backlight support is 
*         lacking or the control failed, return 0.
*/
int kjava_device_startBacklightFlash(int duration)
{   
    JWE_LOG(kjava_device_startBacklightFlash, ("[kjava_device_startBacklightFlash] (duration) = (%d)", duration))
     MMIJAVA_FlashBackLightStart(duration/300,150);      
     return 1;
}

/**
* Stops backlight flashing. 
*
* This function is called when 0 was designated in a parameter of the 
* javax.microedition.lcdui.Display class flashBacklight() method.
*
* \return If this function is called when kjava_device_startBacklightFlash() 
*         has not been called or the duration designated in 
*         kjava_device_startBacklightFlash() has elapsed, return 0 without
*         performing any operation. Otherwise, return 1.
*/
int kjava_device_stopBacklightFlash(void)
{          
     JWE_LOG(kjava_device_stopBacklightFlash, ("[kjava_device_stopBacklightFlash] enter."))
     MMIJAVA_FlashBackLightStop();
     return 0; /* In the default, platform doesn't support blacklight flash */
}

/**
* Starts vibration.
*
* Vibrate the target device for the designated duration. If this function 
* is called while the target device is vibrating, cancel the previous 
* duration designation and vibrate for the newly designated duration, 
* starting from the instant this function was called.
*
* Enable vibration to be stopped forcibly when kjava_device_stopVibration() is 
* called. This function is called when the vibration duration is 
* designated in a parameter of the javax.microedition.lcdui.Display class 
* vibrate() method. 
*
* \param duration  The duration of the vibration (in milliseconds)  
*
* \return if the operation succeeds, returns 1. If vibration support is 
*         lacking or the control failed, return 0.
*/
int kjava_device_startVibration(int duration)
{
    JWE_LOG(kjava_device_startVibration, ("[kjava_device_startVibration] (duration) = (%d)", duration))
        
    if(s_vibration_timer != SCI_NULL)
    {
        if(SCI_IsTimerActive(s_vibration_timer) == TRUE)
        {
            SCI_DeactiveTimer(s_vibration_timer);
        }
        SCI_DeleteTimer(s_vibration_timer);
        s_vibration_timer = SCI_NULL;
    }
    
    MMIAPISET_SetVibrator(TRUE, MMISET_RING_TYPE_OTHER);
    s_vibration_timer = SCI_CreateTimer("Vibration timer",(TIMER_FUN)Vibrationtimer_func, 0, duration, SCI_AUTO_ACTIVATE);
    
    return 1;
}

/**
* Stops vibration.
* 
* Stop the target device vibration. This function is called when 0 was 
* designated in a parameter of the javax.microedition.lcdui.Display class 
* vibrate() method. 
* 
* \return If this function is called when kjava_device_startVibration() has 
*         not been called or the duration designated in 
*         kjava_device_startVibration() has elapsed, return 0 without 
*         performing any operation.
*/
int kjava_device_stopVibration(void)
{
    JWE_LOG(kjava_device_stopVibration, ("[kjava_device_stopVibration] enter."))
        
    MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_OTHER);
    if(s_vibration_timer != SCI_NULL)
    {
        if(SCI_IsTimerActive(s_vibration_timer) == TRUE)
        {
            SCI_DeactiveTimer(s_vibration_timer);
        }
        SCI_DeleteTimer(s_vibration_timer);
        s_vibration_timer = SCI_NULL;
    }
    
    return 1;
}

//***********************************************************************************************
//  Description:Gets key code information for mapping to command keys. 
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
void kjava_device_getCommandKey( KJavaDeviceCommandKeys* commandkeys )
{
    commandkeys->back    = AJSC_KEYCODE_SOFT2;
    commandkeys->cancel  = AJSC_KEYCODE_SOFT2;
    commandkeys->help    = -1;
    commandkeys->exit    = AJSC_KEYCODE_SOFT2;
    commandkeys->ok      = AJSC_KEYCODE_SOFT1;
    commandkeys->stop    = AJSC_KEYCODE_SOFT2;
    commandkeys->menu    = AJSC_KEYCODE_SOFT1;   
    
}

//***********************************************************************************************
//  Description:Get the game key states from the game input device.
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
unsigned long kjava_device_getKeyState(void)
{
    return MMIJAVA_GetKeyState();
}

//***********************************************************************************************
//  Description:Gets game action information for mapping to key codes.
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
void kjava_device_getGameAction( KJavaDeviceGameAction *gameAction )
{
    gameAction->key_num0     = KJAVA_DEVICE_GAME_ACTION_GAME_A ;
    gameAction->key_num1     = KJAVA_DEVICE_GAME_ACTION_GAME_B ;
    gameAction->key_num2     = KJAVA_DEVICE_GAME_ACTION_UP;
    gameAction->key_num3     = 0;
    gameAction->key_num4     = KJAVA_DEVICE_GAME_ACTION_LEFT;
    gameAction->key_num5     = KJAVA_DEVICE_GAME_ACTION_FIRE;
    gameAction->key_num6     = KJAVA_DEVICE_GAME_ACTION_RIGHT;
    gameAction->key_num7     = 0;
    gameAction->key_num8     = KJAVA_DEVICE_GAME_ACTION_DOWN;
    gameAction->key_num9     = 0;
    gameAction->key_asterisk = KJAVA_DEVICE_GAME_ACTION_GAME_C ;
    gameAction->key_pound    = KJAVA_DEVICE_GAME_ACTION_GAME_D ;
    gameAction->key_left     = KJAVA_DEVICE_GAME_ACTION_LEFT ;
    gameAction->key_right    = KJAVA_DEVICE_GAME_ACTION_RIGHT ;
    gameAction->key_up       = KJAVA_DEVICE_GAME_ACTION_UP ;
    gameAction->key_down     = KJAVA_DEVICE_GAME_ACTION_DOWN ;
    gameAction->key_select   = KJAVA_DEVICE_GAME_ACTION_FIRE;  
}

//***********************************************************************************************
//  Description:Vibrationtimer_func
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
LOCAL void Vibrationtimer_func(void)
{
    JWE_LOG(Vibrationtimer_func, ("timeout, stop vibration"));
    
    MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
    s_vibration_timer = SCI_NULL;
}
//***********************************************************************************************
//  Description:ctivates and deactivates the lights on the device. 
//  Author:
//  Date:2007-09-03
//  Note: @para num	is the number or id for light, 0 is used for display backlight, other numbers are currently left unspecified.
//        level	the lighting level 0-100. 0 means "off" or some other minimum lighting setting. For many devices, a value greater than 0 means "light on". 
//***********************************************************************************************
void  kjava_device_setLight(int num, int level)
{
    
}
#endif
