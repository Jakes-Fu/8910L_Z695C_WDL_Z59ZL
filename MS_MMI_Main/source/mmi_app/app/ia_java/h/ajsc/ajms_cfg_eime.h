/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */
/**
 * @file ajms_cfg_eime.h
 * External Input Method ACI
 */

#ifndef AJMS_CFG_EIME_HEADER
#define AJMS_CFG_EIME_HEADER

#ifdef __cplusplus
extern "C"{
#endif

#include "aj_type.h"

/** @addtogroup input_method_aci
 *  @{ 
 */

/*
 *
 *  Interfaces listed in the group are used for external input method, 
 *    including these functionalities:
 *
 *    - Registry the EIME launch and terminate function.
 *    - Function for EIME to return the user input data.
 *    .
 */

/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/

/**
 * @addtogroup External_Input_Method_ACI External Input Method ACI
 * @{
 */

/** \brief Acceptable character type: All (default) */
#define AJMS_EIME_CONSTRAINTS_ANY          (1)
/** \brief Acceptable character type: Email address */
#define AJMS_EIME_CONSTRAINTS_EMAILADDR    (2)
/** \brief Acceptable character type: Numerals */
#define AJMS_EIME_CONSTRAINTS_NUMERIC      (3)
/** \brief Acceptable character type: Telephone numbers */
#define AJMS_EIME_CONSTRAINTS_PHONENUMBER  (4)
/** \brief Acceptable character type: URL */
#define AJMS_EIME_CONSTRAINTS_URL          (5)
/** \brief Acceptable character type: Decimal string */
#define AJMS_EIME_CONSTRAINTS_DECIMAL      (6)

/** \brief Capitalization mode: Undefined (default) */
#define AJMS_EIME_CAPS_UNDEFINE            (1)
/** \brief Capitalization mode: First letter of word */
#define AJMS_EIME_CAPS_WORD                (2)
/** \brief Capitalization mode: First letter of sentence */
#define AJMS_EIME_CAPS_SENTENCE            (3)

/** \brief Result state: User cancel the EIME data */
#define AJMS_EIME_USER_CANCEL              (0)
/** \brief Result state: User confirm the EIME data */
#define AJMS_EIME_USER_CONFIRM             (1)

/** 
 * \brief The data structure type for launch EIME usage.
 */
typedef struct _AJMS_EimeData
{
    /** \brief An USC2 enconding character string for initialize the text
     *         in EIME dialog box. 
     * 
     *         The character string is not terminated by null-character; 
     *         furthermore, note that there may be null-character in the middle 
     *         of the character string.
     */
    AJ_U16   *initText;
    /** \brief The number of characters in the text designated by the initText member. */
    AJ_S32    initTextLen;
    /** \brief The maximum number of acceptable characters. */
    AJ_S32    maxSize;
    /** \brief The input constraint type.
     *
     * The input constrant type could be one of the follows:
     *      - #AJMS_EIME_CONSTRAINTS_ANY
     *      - #AJMS_EIME_CONSTRAINTS_EMAILADDR
     *      - #AJMS_EIME_CONSTRAINTS_NUMERIC
     *      - #AJMS_EIME_CONSTRAINTS_PHONENUMBER
     *      - #AJMS_EIME_CONSTRAINTS_URL
     *      - #AJMS_EIME_CONSTRAINTS_DECIMAL
     */
    AJ_S32    constraints;
    /** \brief A flag which indicates if line-break can be entered. */
    AJ_BOOL   isAllowedLineBreak;
    /** \brief A flag which indicates whether the text input should be in the password mode. */
    AJ_BOOL   isPasswordMode;
    /** \brief A flag which indicates whether the dictionary entry mode is disabled. 
     *
     *  Ignore this field if dictionary search is not supported. For details, see 
     *  javax.microedition.lcdui.TextField.SENSITIVE.
     */
    AJ_BOOL   isSensitive;
    /** \brief A flag which indicates whether the predictive mode is enabled.
     *
     *  Ignore this field if dictionary search is not supported. For details, see 
     *  javax.microedition.lcdui.TextField.NON_PREDICTIVE.
     */
    AJ_BOOL   isPredictive;
    /** \brief The automatic capitalization mode.
     *
     *  The automatic capitalization mode could be one of the follows:
     *       - #AJMS_EIME_CAPS_UNDEFINE
     *       - #AJMS_EIME_CAPS_WORD
     *       - #AJMS_EIME_CAPS_SENTENCE
     *       .
     *  Ignore this field if automatic capitalization is not supported. For details, see 
     *  javax.microedition.lcdui.TextField.INITIAL_CAPS_WORD and
     *  javax.microedition.lcdui.TextField.INITIAL_CAPS_SENTENCE.
     */
    AJ_S32    capitalMode;
    /** \brief An UCS2 character string of the initial input mode.
     *
     *  Ignore this field if target-dependence input mode is not supported. 
     *  For details, see 
     *  javax.microedition.lcdui.TextField.setInitialInputMode(String).
     */
    AJ_U16   *inputModeText;
    /** \brief The number of characters in the text designated by the inputModeText member. */
    AJ_S32    inputModeTextLen;
    /** \brief An UCS2 character string of the EIME dialog title.
     *
     *  Ignore this field if target EIME dialog does not support title. 
     *  For details, see 
     *  the label field for javax.microedition.lcdui.TextField class and
     *  the title field javax.microedition.lcdui.TextBox class.
     */
    AJ_U16   *titleText;
    /** \brief The number of characters in the text designated by the inputModeText member. */
    AJ_S32    titleTextLen;
    /** \brief The X corrdinate of the character drawing position. */
    AJ_S32    contentX;
    /** \brief The Y corrdinate of the character drawing position. */
    AJ_S32    contentY;
    /** \brief The width of the character drawing area. */
    AJ_S32    contentWidth;
    /** \brief The height of the character drawing area. */
    AJ_S32    contentHeight;
}
AJMS_EimeData;

/**
 * \brief  The function type for external input method startup function.
 *
 * If use of the native input method is desired for character entry on GUI components, 
 * implement a function of this callback function type to launch the native input method, 
 * and register the function through a parameter of #ajms_cfg_registerEimeHandler.
 * 
 * When an input method is launched from a GUI component, the <i>contentX</i>, <i>contentY</i>,
 * <i>contentWidth</i> and <i>contentHeight</i> parameters in <i>data</i>
 * indicate the virtual screen coordinates of the GUI component.
 * If the native system input method can be drawn over the GUI component, 
 * the native system may draw it on the real screen portion that corersponds to the virtual 
 * screen area indicated by  <i>contentX</i>, <i>contentY</i>,
 * <i>contentWidth</i> and <i>contentHeight</i> parameters in <i>data</i> structure.
 * Using of the <i>contentX</i>, <i>contentY</i>, <i>contentWidth</i> and 
 * <i>contentHeight</i> parameter is optional. That is, the native system may 
 * use the full-screen regardless of these parameters.
 *
 * This function should request launching of the native input method only and 
 * return as soon as possible to avoid blocking of JBlendia processing long time.
 * The input method must be launched in a separate thread.       
 *
 * @param data The information for launch external input method procedure. 
 *
 * @see ajms_cfg_registerEimeHandler
 */
typedef void (*AJMS_EimeLaunch)( AJMS_EimeData* data );

/**
 * \brief The callback function type for finishing the native input method.
 *
 * If use of the native input method is desired for character entry on GUI components, 
 * implement a function of this callback function type to finish the native input method, 
 * and register the function through a parameter of #ajms_cfg_registerEimeHandler.
 *
 * The callback function is called to forcibly terminate the input method.
 * The native system must free the native system resources used for the input method when it it 
 * finished.
 *
 * When the native input method is terminated in response to the callback funciton, 
 * the native system should not call #ajms_cfg_finalizeEime to notify JBlendia of completion
 * of the input method.
 *
 * @see ajms_cfg_registerEimeHandler
 */
typedef void (*AJMS_EimeTerminate)( void );

/*-----------------------------------------------------------------------------
 * Global Function Prototype
 *----------------------------------------------------------------------------*/

/**
 * \brief Registers callback functions for native input method.
 *
 * If use of the native input method is desired for character entry on GUI components, 
 * implement callback functions for starting and finishing the native input method, 
 * and register them through this function with JBlendia.
 * 
 * Perform the registration by this function before launching the Java application 
 * by #ajms_arc_runApp and #ajms_arc_runAppEx1; namely, it must be performed for each launch of Java application.
 * 
 * Only a set of callback functions can be used.
 * If this function is called multiple times, the callback functions passed in the last call are used.
 * 
 * If AJ_NULL is passed in the <i>launchCb</i> parameter, the default input method implemented in JBlendia 
 * is used. 
 *
 * @param launchCb
 *        The address of the callback function for starting the native input method.
 * @param terminateCb
 *        The address of the callback function for finishing the native input method.
 * 
 *
 * @see AJMS_EimeLaunch and AJMS_EimeTerminate
 */
void ajms_cfg_registerEimeHandler( AJMS_EimeLaunch    launchCb, 
                                   AJMS_EimeTerminate terminateCb);

/**
 * \brief Notifies JBlendia of completion of the native input method. 
 *
 * When the user finishes or cancels the native input method, end the input method and 
 * call this function to notify JBlendia of the result. 
 *
 * @param resultStr          
 *        A pointer to the buffer holding the result character string of user input.
 *        It must be in UCS-2 encoding.
 * @param resultStrLen       
 *        The length of the string indicated by <i>resultStr</i>.
 * @param resultState        
 *        The result of user input.
 *        Pass one of the following values.
 *        - #AJMS_EIME_USER_CONFIRM
 *        - #AJMS_EIME_USER_CANCEL
 *        .
 *
 * @see AJMS_EimeLaunch and AJMS_EimeTerminate
 */

void ajms_cfg_finalizeEime(AJ_U16* resultStr, AJ_S32 resultStrLen, AJ_S32 resultState);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AJMS_EIM_HEADER */
