/*****************************************************************************
** File Name:      mmijava_imwin.h
** Author:         murphy.xie
** Date:           2012/08/15
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe call native text edit function
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE              NAME           DESCRIPTION                              *
** 2012/08/15        murphy.xie     Create                                 *
******************************************************************************/

#ifndef  _MMIJAVA_IMWIN_H_
#define  _MMIJAVA_IMWIN_H_

#include "mmk_type.h"
#include "guiim_base.h"
#include "guiedit.h"


#define SHORT_LABLE_LEN  32

//errors in Im module.
typedef enum
{
    MMIJAVA_IM_ERR_OK = 1,
    MMIJAVA_IM_ERR_PARAM,
    MMIJAVA_IM_ERR_RESOURCE,
    MMIJAVA_IM_ERR_SYSTEM,
    MMIJAVA_IM_ERR_MAX
}MMIJAVA_IM_ERR_E;

// callback types
typedef enum
{
    MMIJAVA_IM_CALLBACK_TYPE_OK = 0,  // user selected ok option
    MMIJAVA_IM_CALLBACK_TYPE_CANCEL,  // user selected cancel option
    MMIJAVA_IM_CALLBACK_TYPE_PREEMP,  // EXIT BY ANOTHER IM client CALLING
    MMIJAVA_IM_CALLBACK_TYPE_EXIT,    // EXIT BY API PROVIDED IN IM
    MMIJAVA_IM_CALLBACK_TYPE_SYS,
    MMIJAVA_IM_CALLBACK_TYPE_TEXT_CHANGED,
    MMIJAVA_IM_CALLBACK_TYPE_COMMAND_FIRED,
    MMIJAVA_IM_CALLBACK_TYPE_WANT_CLOSE,
    MMIJAVA_IM_CALLBACK_TYPE_MAX
}MMIJAVA_IM_CALLBACK_TYPE_E;

typedef enum _Command_Type_Prio
{
    CMD_HELP = 0,
    CMD_ITEM,
    CMD_SCREEN,
    CMD_OK,
    CMD_STOP,
    CMD_EXIT,
    CMD_BACK,
    CMD_CANCEL
}CMDTypePrio;

typedef struct _NativeCommand
{
    uint16    lable[SHORT_LABLE_LEN + 1];
    int     type;
    /**
     * typePrio used to decide which command
     * will be add to right soft button
     */
    CMDTypePrio typePrio;
    int     priority;
    int     nativeID;
    BOOLEAN    isListCmd;
    BOOLEAN    isCmdValid;
    struct _NativeCommand * nextCmd;
}NativeCommand;

typedef void (* MMIJAVA_IM_CALLBACK_FUNC)(MMIJAVA_IM_CALLBACK_TYPE_E type, int cmdIdx, MMI_STRING_T * str_ptr);

typedef struct
{
    MMI_STRING_T * text;
    MMI_STRING_T * title;
    MMI_STRING_T * ticker;
    uint16 max_ucs2_len;
    uint16 max_ascii_len;
    GUIIM_TYPE_T allow_inputmethod;
    GUIIM_TYPE_T cur_inputmethod;
    GUIEDIT_TYPE_E edit_type;
    BOOLEAN isTextBox;
    int cmdNum;
    int timer_id;
    NativeCommand * cmdList;
    MMIJAVA_IM_CALLBACK_FUNC callbackfunc;
    MMI_WIN_ID_T winId;
}MMIJAVA_TEXTEDITBOX_INIT_DATA_T;

/*****************************************************************************/
// 	Description : MMIJAVA_CallNativeTextEdit
//	Global resource dependence :
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC MMIJAVA_IM_ERR_E MMIJAVA_CallNativeTextEdit
(
     MMI_STRING_T  * title_str_ptr,        //[in] title str in editbox
     MMI_STRING_T  * def_str_ptr,        //[in] default str in editbox
     uint16          max_ucs2_len,       //[in] max uss2 str len
     uint16          max_ascii_len,    //[in] max asscii str len
     GUIIM_TYPE_T    allow_inputmethod,  //[in] allowed input modes
     GUIIM_TYPE_T    cur_inputmethod, //[in] init input mode
     GUIEDIT_TYPE_E  edit_type,      //[in] edit style
     MMIJAVA_IM_CALLBACK_FUNC callback   //[in] callback function
);

PUBLIC MMIJAVA_IM_ERR_E MMIJAVA_CallNativeTextEditEx
(
    MMI_STRING_T  * title_str_ptr,        //[in] title str in editbox
    MMI_STRING_T  * def_str_ptr,        //[in] default str in editbox
    uint16          max_ucs2_len,       //[in] max uss2 str len
    uint16          max_ascii_len,    //[in] max asscii str len
    GUIIM_TYPE_T    allow_inputmethod,  //[in] allowed input modes
    GUIIM_TYPE_T    cur_inputmethod, //[in] init input mode
    GUIEDIT_TYPE_E  edit_type,      //[in] edit style
    int cmdNum,
    NativeCommand * cmdList,
    MMIJAVA_IM_CALLBACK_FUNC callback   //[in] callback function
);

// 	Description : MMIJAVA_OpenTextEditWin
//	Global resource dependence :
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_EnterTextEditWin(void);
PUBLIC BOOLEAN MMIJAVA_GetTextEditorForegroundStatus(void);
PUBLIC void MMIJAVA_SetTextEditorForegroundStatus(BOOLEAN isForeground);

#endif //_MMIJAVA_IMWIN_H_
