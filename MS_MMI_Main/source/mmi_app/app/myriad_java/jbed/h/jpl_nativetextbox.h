
/**
 * @file
 * Jbed Porting Layer: Native TextBox Support
 */

#ifndef __JBED_JPL_NATIVETEXTBOX_H__
#define __JBED_JPL_NATIVETEXTBOX_H__

#include <jbed_global.h>
#include <jbed_jbni.h>
#include <javainstance.h>
#include <javastatic.h>

#if defined(__cplusplus)
extern "C" {
#endif

/** Typedef for Java class java.lang.String */
typedef struct INSTANCE_java_lang_String_s* JbedString;

/** Typedef for Java class javax.microedition.lcdui.TextBox */
typedef struct INSTANCE_javax_microedition_lcdui_TextBox_s* LcduiTextBox;

/** Typedef for Java class javax.microedition.lcdui.Command */
typedef struct INSTANCE_javax_microedition_lcdui_Command_s* LcduiCommand;

#define NATIVE_TEXT_CHANGED \
        STATIC_com_jbed_midp_lcdui_toolkit_peer_NativeTextBoxDispatcherImpl_NATIVE_TEXT_CHANGED

#define NATIVE_COMMAND_FIRED \
        STATIC_com_jbed_midp_lcdui_toolkit_peer_NativeTextBoxDispatcherImpl_NATIVE_COMMAND_FIRED

#define NATIVE_WANT_CLOSE \
        STATIC_com_jbed_midp_lcdui_toolkit_peer_NativeTextBoxDispatcherImpl_NATIVE_WANT_CLOSE

#define JPL_TEXTFIELD_CONSTRAINT_CONSTRAINT_MASK \
STATIC_javax_microedition_lcdui_TextField_CONSTRAINT_MASK

#define JPL_TEXTFIELD_CONSTRAINT_URL \
STATIC_javax_microedition_lcdui_TextField_URL

/**
  * @name Command types:
  * Used with #LcduiCommand
  * @{
  */
/** @hideinitializer Command type: SCREEN */
#define NATIVEUI_COMMAND_SCREEN STATIC_javax_microedition_lcdui_Command_SCREEN
/** @hideinitializer Command type: BACK */
#define NATIVEUI_COMMAND_BACK STATIC_javax_microedition_lcdui_Command_BACK
/** @hideinitializer Command type: CANCEL */
#define NATIVEUI_COMMAND_CANCEL STATIC_javax_microedition_lcdui_Command_CANCEL
/** @hideinitializer Command type: OK */
#define NATIVEUI_COMMAND_OK STATIC_javax_microedition_lcdui_Command_OK
/** @hideinitializer Command type: HELP */
#define NATIVEUI_COMMAND_HELP STATIC_javax_microedition_lcdui_Command_HELP
/** @hideinitializer Command type: STOP */
#define NATIVEUI_COMMAND_STOP STATIC_javax_microedition_lcdui_Command_STOP
/** @hideinitializer Command type: EXIT */
#define NATIVEUI_COMMAND_EXIT STATIC_javax_microedition_lcdui_Command_EXIT
/** @hideinitializer Command type: ITEM */
#define NATIVEUI_COMMAND_ITEM STATIC_javax_microedition_lcdui_Command_ITEM
/*@}*/

/**
  * @name TextField/TextBox constraint flags:
  * @{
  */
/** @hideinitializer TextField/TextBox constraint: ANY */
#define NATIVEUI_TEXTFIELD_ANY STATIC_javax_microedition_lcdui_TextField_ANY
/** @hideinitializer TextField/TextBox constraint: EMAILADDR */
#define NATIVEUI_TEXTFIELD_EMAILADDR STATIC_javax_microedition_lcdui_TextField_EMAILADDR
/** @hideinitializer TextField/TextBox constraint: NUMERIC */
#define NATIVEUI_TEXTFIELD_NUMERIC STATIC_javax_microedition_lcdui_TextField_NUMERIC
/** @hideinitializer TextField/TextBox constraint: PHONENUMBER */
#define NATIVEUI_TEXTFIELD_PHONENUMBER STATIC_javax_microedition_lcdui_TextField_PHONENUMBER
/** @hideinitializer TextField/TextBox constraint: URL */
#define NATIVEUI_TEXTFIELD_URL STATIC_javax_microedition_lcdui_TextField_URL
/** @hideinitializer TextField/TextBox constraint: DECIMAL */
#define NATIVEUI_TEXTFIELD_DECIMAL STATIC_javax_microedition_lcdui_TextField_DECIMAL
/** @hideinitializer TextField/TextBox constraint: PASSWORD */
#define NATIVEUI_TEXTFIELD_PASSWORD STATIC_javax_microedition_lcdui_TextField_PASSWORD
/** @hideinitializer TextField/TextBox constraint: UNEDITABLE */
#define NATIVEUI_TEXTFIELD_UNEDITABLE STATIC_javax_microedition_lcdui_TextField_UNEDITABLE
/** @hideinitializer TextField/TextBox constraint: SENSITIVE */
#define NATIVEUI_TEXTFIELD_SENSITIVE STATIC_javax_microedition_lcdui_TextField_SENSITIVE
/** @hideinitializer TextField/TextBox constraint: NON_PREDICTIVE */
#define NATIVEUI_TEXTFIELD_NON_PREDICTIVE STATIC_javax_microedition_lcdui_TextField_NON_PREDICTIVE
/** @hideinitializer TextField/TextBox constraint: INITIAL_CAPS_WORD */
#define NATIVEUI_TEXTFIELD_INITIAL_CAPS_WORD STATIC_javax_microedition_lcdui_TextField_INITIAL_CAPS_WORD
/** @hideinitializer TextField/TextBox constraint: INITIAL_CAPS_SENTENCE */
#define NATIVEUI_TEXTFIELD_INITIAL_CAPS_SENTENCE STATIC_javax_microedition_lcdui_TextField_INITIAL_CAPS_SENTENCE
/** @hideinitializer TextField/TextBox constraint: CONSTRAINT_MASK */
#define NATIVEUI_TEXTFIELD_CONSTRAINT_MASK STATIC_javax_microedition_lcdui_TextField_CONSTRAINT_MASK
/*@}*/

/**
 * Gets the label #JbedString of the command.
 * If the command is the List.SELECT_COMMAND or the Alert.TXT_DISMISS_COMMAND,
 * a I18N translated text will be returned.
 * @param command the #LcduiCommand to query
 * @return the label text #JbedString or NULL if no label text
 * @hideinitializer
 */
#define LcduiCommand_getLabel(command) ((command)->fLabel)

/**
 * Gets the long label #JbedString of the command.
 * @param command the #LcduiCommand to query
 * @return the long label text #JbedString or NULL if no long label text
 * @hideinitializer
 */
#define LcduiCommand_getLongLabel(command) ((command)->fLongLabel)

/**
 * Gets the type of the command.
 * @param command the #LcduiCommand to query
 * @return the type of the command;
 * One of the following:
 *   @arg #NATIVEUI_COMMAND_SCREEN
 *   @arg #NATIVEUI_COMMAND_BACK
 *   @arg #NATIVEUI_COMMAND_CANCEL
 *   @arg #NATIVEUI_COMMAND_OK
 *   @arg #NATIVEUI_COMMAND_HELP
 *   @arg #NATIVEUI_COMMAND_STOP
 *   @arg #NATIVEUI_COMMAND_EXIT
 *   @arg #NATIVEUI_COMMAND_ITEM
 * @hideinitializer
 */
#define LcduiCommand_getType(command) ((command)->fType)
/**
 * Gets the priority of the command.
 * @param command the #LcduiCommand to query
 * @return the priority of the command;
 * smaller values are higher priority
 * @hideinitializer
 */
#define LcduiCommand_getPriority(command) ((command)->fPriority)
/**
 * Gets the native id of the command.
 * @param command the #LcduiCommand to query
 * @return the native id
 * @hideinitializer
 */
#define LcduiCommand_getNativeId(command) ((void*)(((struct INSTANCE_java_lang_Integer_s *)((command)->fData))->value))
/*@}*/

 /**
 * Gets the title string of the displayable.
 * @param displayable the #LcduiDisplayable to query
 * @return the title string or NULL if no title
 * @hideinitializer
 */
#define LcduiDisplayable_getTitle(displayable) ((displayable)->fTitle)
/**
 * Gets the ticker string of the displayable.
 * @param displayable the #LcduiDisplayable to query
 * @return the ticker string or NULL if no ticker
 * @hideinitializer
 */
#define LcduiDisplayable_getTicker(displayable) ((displayable)->fTicker ? (displayable)->fTicker->fText : NULL)
/**
 * Gets the number of commands of the displayable.
 * @param displayable the #LcduiDisplayable to query
 * @return the number of commands
 * @hideinitializer
 */
#define LcduiDisplayable_getCommandCount(displayable) ((displayable)->fCommands->count)
/**
 * Gets the command with the specified index of the displayable.
 * @param displayable the #LcduiDisplayable to query
 * @param index the index of the command
 * @return the #LcduiCommand with the given index
 * @hideinitializer
 */
#define LcduiDisplayable_getCommand(displayable, index) ((LcduiCommand) ((displayable)->fCommands->elements->data[(index)]))

/**
 * @name Query LcduiDisplayable subclasses:
 * Can be used with: #LcduiCanvas
 * @internal
 * @{
 */
/**
 * Gets the title string of the displayable subclass.
 * @param displayable the Displayable subclass to query
 * @return the title string or NULL if no title
 * @hideinitializer
 */
#define LcduiDisplayableSubclass_getTitle(displayable) LcduiDisplayable_getTitle(&(displayable)->super)
/**
 * Gets the ticker string of the subclass of #LcduiDisplayable.
 * @param displayable the Displayable subclass to query
 * @return the ticker string or NULL if no ticker
 * @hideinitializer
 */
#define LcduiDisplayableSubclass_getTicker(displayable) LcduiDisplayable_getTicker(&(displayable)->super)
/**
 * Gets the number of commands of the subclass of #LcduiDisplayable.
 * @param displayable the Displayable subclass to query
 * @return the number of commands
 * @hideinitializer
 */
#define LcduiDisplayableSubclass_getCommandCount(displayable) LcduiDisplayable_getCommandCount(&(displayable)->super)
/**
 * Gets the command with the specified index of the subclass of #LcduiDisplayable.
 * @param displayable the Displayable subclass to query
 * @param index the index of the command
 * @return the #LcduiCommand with the given index
 * @hideinitializer
 */
#define LcduiDisplayableSubclass_getCommand(displayable, index) LcduiDisplayable_getCommand(&(displayable)->super, (index))
/*@}*/

/**
 * @name Query LcduiScreen subclasses:
 * Can be used with: #LcduiAlert, #LcduiForm, #LcduiList, #LcduiTextBox
 * @internal
 * @{
 */
/**
 * Gets the title string of the subclass of Screen.
 * @param screen the Screen to query
 * @return the title string or NULL if no title
 * @hideinitializer
 */
#define LcduiScreenSubclass_getTitle(screen) LcduiDisplayable_getTitle(&(screen)->super.super)
/**
 * Gets the ticker string of the subclass of Screen.
 * @param screen the Screen to query
 * @return the ticker string or NULL if no ticker
 * @hideinitializer
 */
#define LcduiScreenSubclass_getTicker(screen) LcduiDisplayable_getTicker(&(screen)->super.super)
/**
 * Gets the number of commands of the subclass of Screen.
 * @param screen the Screen to query
 * @return the number of commands
 * @hideinitializer
 */
#define LcduiScreenSubclass_getCommandCount(screen) LcduiDisplayable_getCommandCount(&(screen)->super.super)
/**
 * Gets the command with the specified index of the subclass of Screen.
 * @param screen the Screen to query
 * @param index the index of the command
 * @return the #LcduiCommand with the given index
 * @hideinitializer
 */
#define LcduiScreenSubclass_getCommand(screen, index) LcduiDisplayable_getCommand(&(screen)->super.super, (index))

/**
 * @name Query LcduiTextBox:
 * Can be used with: #LcduiTextBox
 * @{
 */
/**
 * Gets the title string of the textbox.
 * @param textbox the #LcduiTextBox to query
 * @return the title string or NULL if no title
 * @hideinitializer
 */
#define LcduiTextBox_getTitle(textbox) LcduiScreenSubclass_getTitle(textbox)
/**
 * Gets the ticker #JbedString of the textbox.
 * @param textbox the #LcduiTextBox to query
 * @return the ticker #JbedString or NULL if no ticker
 * @hideinitializer
 */
#define LcduiTextBox_getTicker(textbox) LcduiScreenSubclass_getTicker(textbox)
/**
 * Gets the number of commands of the textbox.
 * @param textbox the #LcduiTextBox to query
 * @return the number of commands
 * @hideinitializer
 */
#define LcduiTextBox_getCommandCount(textbox) LcduiScreenSubclass_getCommandCount(textbox)
/**
 * Gets the command with the specified index of the textbox.
 * @param textbox the #LcduiTextBox to query
 * @param index the index of the command
 * @return the #LcduiCommand with the given index
 * @hideinitializer
 */
#define LcduiTextBox_getCommand(textbox, index) LcduiScreenSubclass_getCommand((textbox), (index))
/**
 * Gets the maximal size (contained characters) of the textbox.
 * @param textbox the #LcduiTextBox to query
 * @return the maximal size
 * @hideinitializer
 */
#define LcduiTextBox_getMaxSize(textbox) ((textbox)->fBackText->fBuffer->length)
/**
 * Gets the constraints flags of the textbox.
 * @param textbox the #LcduiTextBox to query
 * @return the constraints flags;
 * A bitwise or combination of the following:
 *   @arg #NATIVEUI_TEXTFIELD_ANY
 *   @arg #NATIVEUI_TEXTFIELD_EMAILADDR
 *   @arg #NATIVEUI_TEXTFIELD_NUMERIC
 *   @arg #NATIVEUI_TEXTFIELD_PHONENUMBER
 *   @arg #NATIVEUI_TEXTFIELD_URL
 *   @arg #NATIVEUI_TEXTFIELD_DECIMAL
 *   @arg #NATIVEUI_TEXTFIELD_PASSWORD
 *   @arg #NATIVEUI_TEXTFIELD_UNEDITABLE
 *   @arg #NATIVEUI_TEXTFIELD_SENSITIVE
 *   @arg #NATIVEUI_TEXTFIELD_NON_PREDICTIVE
 *   @arg #NATIVEUI_TEXTFIELD_INITIAL_CAPS_WORD
 *   @arg #NATIVEUI_TEXTFIELD_INITIAL_CAPS_SENTENCE
 * @hideinitializer
 */
#define LcduiTextBox_getConstraints(textbox) ((textbox)->fConstraints)
/**
 * Gets the initial input mode of the textbox.
 * @param textbox the #LcduiTextBox to query
 * @return the initial input mode #JbedString
 * @hideinitializer
 */
#define LcduiTextBox_getInitialInputMode(textbox) ((textbox)->fInitialInputMode)


/**
*********************************************
*/
/*
 *copyMmiStr
 * new a MMI_STRING_T copy passed in MMI_STRING_T str
 * return the new MMI_STRING_T if malloc memory success,
 * otherwise return NULL
*/
MMI_STRING_T * copyMmiStr(MMI_STRING_T * str) ;
/*
 *freeMmiStr
 * free passed in MMI_STRING_T memory
*/
void freeMmiStr(MMI_STRING_T * mmiStr);
/*
 *clearNativeTextBox
 * clear native text box content,title,tick str
  * and free all text  box command list
*/
void clearNativeTextBox();

/*
 *check dot char is in the string
 * MMI_STRING_T  passed in
 * return 1 if decimal type need set '*' to '.'
 * otherwise return 0
*/
int  needDecimalSetDot(MMI_STRING_T * thisString);

#if defined(__cplusplus)
}
#endif

#endif /* __JBED_JPL_NATIVETEXTBOX_H__ */
