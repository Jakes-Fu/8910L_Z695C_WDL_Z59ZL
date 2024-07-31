/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */

#ifndef KJAVA_SYS_DEVICE_HEADER
#define KJAVA_SYS_DEVICE_HEADER

/**
 * @file kjava_sys_device.h
 * Device Control KSI
 */

#ifdef __cplusplus
extern "C"{
#endif

/** @addtogroup device_control_ksi
 *  @{
 */

/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/

/** 
 * @addtogroup Device_Control_Alarm_Sound Alarm Sound
 * @{
 */

/**
 *  \brief An alarm type sound.
 *
 *  This type corresponds to that of the <i>javax.microedition.lcdui.AlertType.ALARM</i> field.
 */
#define KJAVA_DEVICE_ALERT_TYPE_ALARM          0

/**
 *  \brief A confirmation type sound.
 *
 *  This type corresponds to that of the 
 *  <i>javax.microedition.lcdui.AlertType.CONFIRMATION</i> field.
 */
#define KJAVA_DEVICE_ALERT_TYPE_CONFIRM        1

/**
 *  \brief An error beep type sound.
 *
 *  This type corresponds to that of the 
 *  <i>javax.microedition.lcdui.AlertType.ERROR</i> field.
 */
#define KJAVA_DEVICE_ALERT_TYPE_ERROR          2

/**
 *  \brief An infomational type sound.
 *
 *  This type corresponds to that of the 
 *  <i>javax.microedition.lcdui.AlertType.INFO</i> field.
 */
#define KJAVA_DEVICE_ALERT_TYPE_INFO           3

/**
 *  \brief A warning type sound.
 *
 *  This type corresponds to that of the
 *  <i>javax.microedition.lcdui.AlertType.WARNING</i> field.
 */
#define KJAVA_DEVICE_ALERT_TYPE_WARNING        4

/** @} */

/** 
 * @addtogroup Device_Control_Command_Key_Mapping Command Key Mapping
 * @{
 */
 
/**
 * \brief Structure holding the key code for different function keys.
 */
typedef struct _KJavaDeviceCommandKeysTag
{
    /** \brief The key code for "BACK" */
    int back;
    /** \brief The key code for "CANCEL" */
    int cancel;
    /** \brief The key code for "HELP" */
    int help;
    /** \brief The key code for "EXIT" */
    int exit;
    /** \brief The key code for "OK" */
    int ok;
    /** \brief The key code for "STOP" */
    int stop;
    /** \brief The key code for "Menu" */
    int menu;
}
KJavaDeviceCommandKeys;

/** @} */

/** 
 * @addtogroup Device_Control_Game_Key_Mapping Game Key Mapping
 * @{
 */
/** \brief The Up key. This is the Up key bit in the value returned by #kjava_device_getKeyState(). */
#define KJAVA_DEVICE_GAME_KEY_UP           (0x2)
/** \brief The Left key. This is the Left key bit in the value returned by #kjava_device_getKeyState(). */
#define KJAVA_DEVICE_GAME_KEY_LEFT         (0x4)
/** \brief The Right key. This is the Right key bit in the value returned by #kjava_device_getKeyState(). */
#define KJAVA_DEVICE_GAME_KEY_RIGHT        (0x20)
/** \brief The Down key. This is the Down key bit in the value returned by #kjava_device_getKeyState(). */
#define KJAVA_DEVICE_GAME_KEY_DOWN         (0x40)
/** \brief The Select key. This is the Select key bit in the value returned by #kjava_device_getKeyState(). */
#define KJAVA_DEVICE_GAME_KEY_FIRE         (0x100)
/** \brief The A key. This is the A key bit in the value returned by #kjava_device_getKeyState(). */
#define KJAVA_DEVICE_GAME_KEY_A            (0x200)
/** \brief  The B key. This is the B key bit in the value returned by #kjava_device_getKeyState(). */
#define KJAVA_DEVICE_GAME_KEY_B            (0x400)
/** \brief The C key. This is the C key bit in the value returned by #kjava_device_getKeyState(). */
#define KJAVA_DEVICE_GAME_KEY_C            (0x800)
/** \brief The D key. This is the D key bit in the value returned by #kjava_device_getKeyState(). */
#define KJAVA_DEVICE_GAME_KEY_D            (0x1000)

/**
 *  \brief Game action type A.
 *
 *  This type corresponds to that of the <i>javax.microedition.lcdui.Canvas.GAME_A</i> field.
 */
#define KJAVA_DEVICE_GAME_ACTION_GAME_A        9

/**
 *  \brief Game action type B.
 *
 *  This type corresponds to that of the <i>javax.microedition.lcdui.Canvas.GAME_B</i> field.
 */
#define KJAVA_DEVICE_GAME_ACTION_GAME_B        10

/**
 *  \brief Game action type C.
 *
 *  This type corresponds to that of the <i>javax.microedition.lcdui.Canvas.GAME_C</i> field.
 */
#define KJAVA_DEVICE_GAME_ACTION_GAME_C        11

/**
 *  \brief Game action type D.
 *
 *  This type corresponds to that of the <i>javax.microedition.lcdui.Canvas.GAME_D</i> field.
 */
#define KJAVA_DEVICE_GAME_ACTION_GAME_D        12

/**
 *  \brief Game action type UP.
 *
 *  This type corresponds to that of the <i>javax.microedition.lcdui.Canvas.UP</i> field.
 */
#define KJAVA_DEVICE_GAME_ACTION_UP            1

/**
 *  \brief Game action type LEFT.
 *
 *  This type corresponds to that of the <i>javax.microedition.lcdui.Canvas.LEFT</i> field.
 */
#define KJAVA_DEVICE_GAME_ACTION_LEFT          2

/**
 *  \brief Game action type DOWN.
 *
 *  This type corresponds to that of the <i>javax.microedition.lcdui.Canvas.DOWN</i> field.
 */
#define KJAVA_DEVICE_GAME_ACTION_DOWN          6

/**
 *  \brief Game action type RIGHT.
 *
 *  This type corresponds to that of the <i>javax.microedition.lcdui.Canvas.RIGHT</i> field.
 */
#define KJAVA_DEVICE_GAME_ACTION_RIGHT         5

/**
 *  \brief Game action type FIRE.
 *
 *  This type corresponds to that of the <i>javax.microedition.lcdui.Canvas.FIRE</i> field.
 */
#define KJAVA_DEVICE_GAME_ACTION_FIRE          8


/**
 * \brief Structure holding the key code for different game action keys.
 */
typedef struct _KJavaDeviceGameActionTag
{
    /** \brief The game action mapped to the 0 key. */
    int key_num0;
    /** \brief The game action mapped to the 1 key. */
    int key_num1;
    /** \brief The game action mapped to the 2 key. */
    int key_num2;
    /** \brief The game action mapped to the 3 key. */
    int key_num3;
    /** \brief The game action mapped to the 4 key. */
    int key_num4;
    /** \brief The game action mapped to the 5 key. */
    int key_num5;
    /** \brief The game action mapped to the 6 key. */
    int key_num6;
    /** \brief The game action mapped to the 7 key. */
    int key_num7;
    /** \brief The game action mapped to the 8 key. */
    int key_num8;
    /** \brief The game action mapped to the 9 key. */
    int key_num9;
    /** \brief The game action mapped to the asterisk (*) key. */
    int key_asterisk;
    /** \brief The game action mapped to the pound (#) key. */
    int key_pound;
    /** \brief The game action mapped to the Left key. */
    int key_left;
    /** \brief The game action mapped to the Up key. */
    int key_up;
    /** \brief The game action mapped to the Right key. */
    int key_right;
    /** \brief The game action mapped to the Down key. */
    int key_down;
    /** \brief The game action mapped to the Select key. */
    int key_select;
}
KJavaDeviceGameAction;

/** @} */

/*-----------------------------------------------------------------------------
 * Global Function(s) Prototype
 *----------------------------------------------------------------------------*/

/** 
 * @addtogroup Device_Control_Alarm_Sound
 * @{
 */

/**
 * \brief Alerts the user by playing a sound.
 * @param alertType 
 *        The type of sound used to alert the user.
 *        One of the following is designated.
 *        - #KJAVA_DEVICE_ALERT_TYPE_ALARM
 *        - #KJAVA_DEVICE_ALERT_TYPE_CONFIRM
 *        - #KJAVA_DEVICE_ALERT_TYPE_ERROR
 *        - #KJAVA_DEVICE_ALERT_TYPE_INFO
 *        - #KJAVA_DEVICE_ALERT_TYPE_WARNING
 *        .
 * @return 
 *        Return 1 if the operation succeeds. <br>
 *        In case of failure, (e.g. sound support is lacking or the 
 *        sound resource is in use, etc) return 0.
 *
 * This function corresponds to the 
 * <i>javax.microedition.lcdui.AlertType.playSound</i> method.
 */
int kjava_device_alertBySound(int alertType);

/**
 * \brief Terminates the alert sound.
 *
 * Stop the alert sound started by #kjava_device_alertBySound.
 * If the sound is not playing when this function is called, exit this
 * function without doing anything.
 */
void kjava_device_stopAlertSound(void);

/** @} */

/** 
 * @addtogroup Device_Control_Back_Light Backlight Flash Control
 * @{
 */

/**
 * \brief Starts backlight flashing.
 * @param duration  
 *        The duration (in milliseconds) of backlight flashing.
 * @return 
 *        Return 1 if the operation succeeds.<br>
 *        If backlight support is lacking or the control failed, return 0.
 *
 * Flash the backlight for the designated duration. The blink cycle
 * (durations of on and off periods) is platform-dependent.
 *
 * If this function is called while the backlight is flashing,
 * cancel the previous flashing duration designation and flash for
 * the newly designated duration, starting from the instant this function
 * was called. Enable the flashing to be stopped forcibly when
 * #kjava_device_stopBacklightFlash is called.
 *
 * This function is called when the flash duration is designated as a
 * parameter in the <i>javax.microedition.lcdui.Display.flashBacklight</i> method.
 */
int kjava_device_startBacklightFlash(int duration);

/**
 * \brief Stops backlight flashing.
 * @return 
 *        Return 1 if the operation succeeds. <br>
 *        If backlight support is lacking or the control failed, return 0.
 *
 * Stop the backlight flashing. 
 *
 * If this function is called when #kjava_device_startBacklightFlash has
 * not been called or the duration designated in
 * #kjava_device_startBacklightFlash has elapsed, return 0 without
 * performing any operation. 
 *
 * This function is called when 0 was designated in a parameter of the
 * <i>javax.microedition.lcdui.Display.flashBacklight</i> method.
 */
int kjava_device_stopBacklightFlash(void);

/** @} */

/** 
 * @addtogroup Device_Control_Vibration Vibration Control
 * @{
 */

/**
 * \brief Starts vibration.
 * @param duration  
 *        The duration of the vibration (in milliseconds).
 * @return 
 *        Return 1 if the operation succeeds.<br>
 *        If vibration support is lacking or the control failed, return 0.
 *
 * Vibrate the target device for the designated duration. 
 *
 * If this function
 * is called while the target device is vibrating, cancel the previous
 * duration designation and vibrate for the newly designated duration,
 * starting from the instant this function was called.
 *
 * Enable vibration to be stopped forcibly when #kjava_device_stopVibration is
 * called. 
 *
 * This function is called when the vibration duration is
 * designated in a parameter of the 
 * <i>javax.microedition.lcdui.Display.vibrate</i> method.
 *
 */
int kjava_device_startVibration(int duration);

/**
 * \brief Stops vibration.
 *
 * @return 
 *        Return 1 if the operation succeeds.<br>
 *        If vibration support is lacking or the control failed, return 0.
 *
 * Stop the target device vibration.
 * 
 * If this function is called when #kjava_device_startVibration has
 * not been called or the duration designated in
 * #kjava_device_startVibration has elapsed, return 0 without
 * performing any operation.
 *
 * This function is called when 0 was
 * designated in a parameter of the 
 * <i>javax.microedition.lcdui.Display.vibrate</i> method.
 *
 */
int kjava_device_stopVibration(void);

/** @} */

/** 
 * @addtogroup Device_Control_Command_Key_Mapping Command Key Mapping
 * @{
 */
 
/**
 *   \brief Gets key code information for mapping to command keys.
 *
 *          This function is used to mapping the several command keys to
 *          the target keypad. Usually, all the command keys should mapping
 *          to one of the follows AJSC key code:
 *          - #AJSC_KEYCODE_SOFT1: Indicate the command key is mapping to first soft key.
 *          - #AJSC_KEYCODE_SOFT2: Indicate the command key is mapping to second soft key.
 *          - #AJSC_KEYCODE_SOFT3: Indicate the command key is mapping to third soft key. The
 *                                 third sokt key might not exist in all target. 
 *          - <b>-1</b>: Indicate the command key is not supported on target.
 *          .
 *          
 *          A typical implementation is as follows:
 *    <pre>
 *    commandkeys->back    = AJSC_KEYCODE_SOFT2;
 *    commandkeys->cancel  = AJSC_KEYCODE_SOFT2;
 *    commandkeys->help    = -1;
 *    commandkeys->exit    = AJSC_KEYCODE_SOFT2;
 *    commandkeys->ok      = AJSC_KEYCODE_SOFT1;
 *    commandkeys->stop    = AJSC_KEYCODE_SOFT2;
 *    commandkeys->menu    = AJSC_KEYCODE_SOFT1;
 *    </pre>
 *
 *    Please check <i>javax.microedition.lcdui.Command</i> for more information.
 *
 *   @param commandkeys A pointer to the #KJavaDeviceCommandKeys structure
 *                      for storing command key mapping information.
 *
 *
 */
void kjava_device_getCommandKey( KJavaDeviceCommandKeys* commandkeys );

/** @} */

/** 
 * @addtogroup Device_Control_Game_Key_Mapping Game Key Mapping
 * @{
 */
/**
 *  \brief Get the game key states from the game input device.
 *
 *  \return
 *   Return game key states as per the bit assignments below.<br>
 *   #KJAVA_DEVICE_GAME_KEY_UP<br>
 *   #KJAVA_DEVICE_GAME_KEY_DOWN<br>
 *   #KJAVA_DEVICE_GAME_KEY_LEFT<br>
 *   #KJAVA_DEVICE_GAME_KEY_RIGHT<br>
 *   #KJAVA_DEVICE_GAME_KEY_FIRE<br>
 *   #KJAVA_DEVICE_GAME_KEY_A<br>
 *   #KJAVA_DEVICE_GAME_KEY_B<br>
 *   #KJAVA_DEVICE_GAME_KEY_C<br>
 *   #KJAVA_DEVICE_GAME_KEY_D
 *
 *  <p>
 *  Return the game key states. The purpose of this function is to prevent the key information
 *  used by Java applications from lagging behind the actual hardware states.<br>
 *  </p><p>
 *  Set bits in the return value as in the table below.
 *  </p>
 *  <table>
 *   <tr>
 *    <td><b>Key state</b></td>
 *    <td><b>Return value bits</b></td>
 *   </tr>
 *   <tr>
 *    <td> Currently pressed, or pressed at least once since the previous time #kjava_device_getKeyState() was called </td>
 *    <td> 1 </td>
 *   </tr>
 *   <tr>
 *    <td> Currently released, and not pressed since the previous time #kjava_device_getKeyState() was called. </td>
 *    <td> 0 </td>
 *   </tr>
 *  </table>
 *
 *  <p>The first time #kjava_device_getKeyState() is called, set this information based only on the state at that point in time.</p>
 *  The mapping of game key bits to actual keys is dependent on the native system specifications.<br>
 *  This function corresponds to the javax.microedition.lcdui.game.GameCanvas class getKeyStates() method.
 *
 */
unsigned long kjava_device_getKeyState(void); 

/**
 *   \brief Gets game action information for mapping to key codes.
 *
 *          This function is used to mapping all the several game actions to
 *          the target keypad. All the target key inside #KJavaDeviceGameAction
 *          should be mapping to one of the follows:
 *          - #KJAVA_DEVICE_GAME_ACTION_GAME_A
 *          - #KJAVA_DEVICE_GAME_ACTION_GAME_B
 *          - #KJAVA_DEVICE_GAME_ACTION_GAME_C
 *          - #KJAVA_DEVICE_GAME_ACTION_GAME_D
 *          - #KJAVA_DEVICE_GAME_ACTION_UP
 *          - #KJAVA_DEVICE_GAME_ACTION_LEFT
 *          - #KJAVA_DEVICE_GAME_ACTION_DOWN
 *          - #KJAVA_DEVICE_GAME_ACTION_RIGHT
 *          - #KJAVA_DEVICE_GAME_ACTION_FIRE
 *          - <b>0</b>: Indicate the key is not mapping to any game action.
 *          .
 *
 *          
 *          A typical implementation is as follows:
 *    <pre>
 *    gameAction->key_num0     = KJAVA_DEVICE_GAME_ACTION_GAME_A ;
 *    gameAction->key_num1     = KJAVA_DEVICE_GAME_ACTION_GAME_B ;
 *    gameAction->key_num2     = KJAVA_DEVICE_GAME_ACTION_UP;
 *    gameAction->key_num3     = 0;
 *    gameAction->key_num4     = KJAVA_DEVICE_GAME_ACTION_LEFT;
 *    gameAction->key_num5     = KJAVA_DEVICE_GAME_ACTION_FIRE;
 *    gameAction->key_num6     = KJAVA_DEVICE_GAME_ACTION_RIGHT;
 *    gameAction->key_num7     = 0;
 *    gameAction->key_num8     = KJAVA_DEVICE_GAME_ACTION_DOWN;
 *    gameAction->key_num9     = 0;
 *    gameAction->key_asterisk = KJAVA_DEVICE_GAME_ACTION_GAME_C ;
 *    gameAction->key_pound    = KJAVA_DEVICE_GAME_ACTION_GAME_D ;
 *    gameAction->key_left     = KJAVA_DEVICE_GAME_ACTION_LEFT ;
 *    gameAction->key_right    = KJAVA_DEVICE_GAME_ACTION_RIGHT ;
 *    gameAction->key_up       = KJAVA_DEVICE_GAME_ACTION_UP ;
 *    gameAction->key_down     = KJAVA_DEVICE_GAME_ACTION_DOWN ;
 *    gameAction->key_select   = KJAVA_DEVICE_GAME_ACTION_FIRE;
 *    </pre>
 *
 *    Please check <i>javax.microedition.lcdui.Canvas</i> for more information.
 *
 *   @param gameAction A pointer to the #KJavaDeviceGameAction structure
 *                      for storing key to game action mapping information.
 */
void kjava_device_getGameAction( KJavaDeviceGameAction *gameAction );

/**
  * @description	Activates and deactivates the lights on the device. 
  * @para num	is the number or id for light, 0 is used for display backlight, other numbers are currently left unspecified.
  * @para level	the lighting level 0-100. 0 means "off" or some other minimum lighting setting. For many devices, a value greater than 0 means "light on". 
  */
void  kjava_device_setLight(int num, int level);
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* KJAVA_SYS_DEVICE_HEADER */
