/****************************************************************************
** File Name:      mmisms_chatreadwin.c                                    *
** Author:         liming deng                                             *
** Date:           1/4/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe deleting SMS.              *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 01/2012        minghu.mao       Create
**
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "mmisms_messageboxwin.h"
#include "mmiudisk_export.h"
#include "mmimms_export.h"
#include "mmisms_pushmsgboxwin.h"
#include "guictrl_api.h"
#include "mmisms_appcontrol.h"
#include "mmisms_filecontrol.h"
#include "mmialarm_export.h"
#include "mmiidle_statusbar.h"
#include "mmicom_statusarea.h"
#include "mmicom_banner.h"
#include "mmisms_receive.h"
#include "mmisms_send.h"
#include "mmisms_read.h"
#include "mmisms_internal.h"
#include "mmisms_export.h"
#include "mmisms_messagecountwin.h"
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
#include "mmisms_securitywin.h"
#endif
#ifdef MMI_OTA_SUPPORT
#include "mmiota_export.h"
#endif
#ifdef BROWSER_SUPPORT
#ifndef NANDBOOT_SUPPORT
#include "mmibrowser_export.h"
#endif
#endif
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#endif
#include "mmismsapp_interface.h"
#include "mmisms_blockmessageboxwin.h"
#include "mmitheme_button.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
typedef struct
{
    MMI_HANDLE_T         win_handle;
    MMI_WIN_ID_T            win_id;
    uint32                  menu_id;
    uint32                  group_id;
    MMISMS_BOX_TYPE_E       box_type;
    MMI_CTRL_ID_T     cur_list_ctrl_id;
    uint32            marked_sms_num;
    MMISMS_ORDER_ID_T cur_focus_order_id;
#ifdef MMI_PDA_SUPPORT
    BOOLEAN is_mark_all;
#endif
    BOOLEAN is_del_all;
    BOOLEAN is_del_chatlist;
}MMISMS_MESSAGEBOX_WINDATA_T;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/
#ifdef MMI_GUI_STYLE_TYPICAL
#ifdef TOUCH_PANEL_SUPPORT
extern MMISMS_GLOBAL_T g_mmisms_global;   // SMS的全局变量
#endif
#endif

BOOLEAN is_exit_from_save_draft = FALSE;
/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//MMISMS_DELMSG_GLOBLE_T g_sms_delmsg_all_info = {0};


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL BOOLEAN is_focus_new_msg_btn = FALSE;
/**--------------------------------------------------------------------------*
 **                         FUCTION DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void ButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr);
/*****************************************************************************/
// 	Description : open draft box
//	Global resource dependence :
//  Author:rong.gu
//	Note:
/*****************************************************************************/
LOCAL void HandleOpenDraftBoxWin(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

#endif


#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxCopyToMe(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//  Description : open the SMS move query window
//  Global resource dependence :
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
//PUBLIC void MsgBoxOpenCopyQueryWinMsg(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);


/*****************************************************************************/
//  Description : to handle the message of move waiting window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMsgBoxCopyWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     );

/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MsgBoxCopySelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);

/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxCopyToSIM(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : to handle the message of delete query window
//    Global resource dependence :
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMsgBoxCopyQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );

#endif


#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxMoveToMe(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxMoveToSIM(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : to handle the message of delete query window
//    Global resource dependence :
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMsgBoxMoveQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );

/*****************************************************************************/
//  Description : to handle the message of move waiting window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMsgBoxMoveWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     );
#endif

/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMtBoxChildWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendSuccBoxChildWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendFailBoxChildWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDraftBoxChildWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : to handle the message of SMS box window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleChatBoxChildWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E msg_id,        //IN:
    DPARAM            param        //IN:
);

/*****************************************************************************/
//  Description : set anim in chat list
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawListItemWithIconData(
    uint16           item_index,     //in:item索引
    uint16           content_index,  //in:item内容索引,text or anim etc.
    MMI_CTRL_ID_T    ctrl_id         //in:control id
);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : handle menu key opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxWinOpenChatMenuKeyOpt(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenChatMenuKeyInit(MMIPUB_MENU_NOTIFY_T *param);
#endif

/*****************************************************************************/
//     Description : handle chat box init
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenChatBoxInit(MMIPUB_MENU_NOTIFY_T *param);


/*****************************************************************************/
//  Discription: Handle  block list textbox msg
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL void   HandleBlockNumberTextBoxWinMsg(
                                         MMI_WIN_ID_T win_id,
                                         MMI_MESSAGE_ID_E msg_id,
                                         DPARAM param
                                         );
#endif

/*****************************************************************************/
//     Description : to handle the message of message main box window
//    Global resource dependence :
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsgBoxMainWindow(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : add all tabs and all child windows to parent window.
//  Global resource dependence :
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void  AddTabsAndChildWindows(MMISMS_MESSAGEBOX_WINDATA_T  *win_data_ptr);

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxWinOpenSendFailOpt(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxWinOpenSendSuccOpt(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenSendSuccInit(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxWinOpenMTOpt(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenBoxMenuOpt(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxWinOpenNoSendOpt(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenNoSendInit(MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenSendFailInit(MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenMTInit(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : handle msg box answser sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxDelAll(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

LOCAL void HandleMsgBoxDelAllNoQueryWindow(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

#if 0
/*****************************************************************************/
//     Description : to handle the message of delete waiting window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MsgBoxHandleDelWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     );
#endif
/*****************************************************************************/
//     Description : handle msg box answser sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxLockItem(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle msg box answser sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxUnLockItem(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  rong.gu
// Note:
/*****************************************************************************/
LOCAL void MsgBoxWinCloseCtrlFunc (MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : to handle the message of delete query window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxDelSameNum(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//  Description : build win user data
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MsgBoxBuildWinUserData(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

#if defined(MAINLCD_SIZE_128X64) || defined(TOUCH_PANEL_SUPPORT)
/*****************************************************************/
//  Description :号码编辑菜单选择联系人的callback
//  Global resource dependence : none
//  Author:
//  Note:
//  Date:
/*****************************************************************/
LOCAL void EditSmsNumSelectContactCallback(MMIPB_HANDLE_T handle);
#endif

#ifdef MMI_GUI_STYLE_TYPICAL
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : pre open toolbar func
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_MTBoxPreEntryToolbar( MMI_HANDLE_T ctrl_handle,void* param_ptr);

/*****************************************************************************/
//  Description : create toolbar
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_CreateToolbar(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id,MMI_CTRL_ID_T attach_ctrl_id);

/*****************************************************************************/
//  Description : update toolbar
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_MTBoxUpdateWinToolbar(void);

/*****************************************************************************/
//  Description : sel item to send sms
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_ToolBarItemSendSMS(MMI_WIN_ID_T    win_id);

/*****************************************************************************/
//  Description :deal sel toolbar item
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_HandleToolBarItemSelect(MMI_WIN_ID_T    win_id, SMS_TOOLBAR_TYPE_E node_id);
#endif
#endif

/*****************************************************************************/
//     Description : handle msg box answser sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxWinSmsKeyCall(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr,MMI_MESSAGE_ID_E msg_id);


#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : handle menu key opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxWinOpenMenuKeyOpt(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr,BOOLEAN is_enable_mark);

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenMenuKeyInit(MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenListMarkInit(MMIPUB_MENU_NOTIFY_T *param);
#endif

/*****************************************************************************/
//     Description : handle msg box forward sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxForwardMsg(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle msg box forward sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
//LOCAL void HandleMsgBoxForwardSMS(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle msg box forward sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxResendSms(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle msg box forward sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxResendMsg(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle msg box edit sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxEditMsg(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);


/*****************************************************************************/
//     Description : to handle the message of delete query window
//    Global resource dependence :
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMessageBoxDelQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );

/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void SMS_ValidatePrivacyEnterSmsBox(BOOLEAN is_permitted, DPARAM param_ptr);

/*****************************************************************************/
//     Description : handle delete finish call back
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_MessageBoxDelFinishCallback(void *user_data,MMISMS_DELETE_DATE_INFO *data_info_ptr) ;


/******************************************************************************
//  Description : Callback of read sms
//	Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL BOOLEAN MessageboxWinReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T *callback_data);


/*****************************************************************************/
//     Description : handle delete finish call back
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_MessageBoxMoveFinishCallback(void *user_data,MMISMS_MOVE_RESULT_INFO *data_info_ptr);

/*****************************************************************************/
//     Description : open read message
//    Global resource dependence :
//  Author:rong.gu
//    Note:显示内容的窗口(MT,MO Success)
/*****************************************************************************/
LOCAL void HandleMsgBoxEditSMS(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr);
/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//     Description : New message Button callback function
//    Global resource dependence :
//  Author:yuguang.liu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MIDButtonCallBack(void)
{
	MMI_RESULT_E result  = MMI_RESULT_TRUE;

    MMISMS_CreateNewMessageWin();
	return result;
}
#endif

// the window for message box
WINDOW_TABLE(MMISMS_MSGBOX_MAIN_WIN_TAB) =
{
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_FUNC( (uint32)HandleMsgBoxMainWindow),
    WIN_ID( MMISMS_MSGBOX_MAIN_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
#if defined(MMI_GUI_STYLE_TYPICAL) || defined(MMI_SMS_MINI_SUPPORT) || defined(MMI_GUI_STYLE_MINISCREEN)
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NULL | GUITAB_STATE_NONEED_SCROLL, MMISMS_MSGBOX_TAB_CTRL_ID),
#else
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL | GUITAB_STATE_SINGLE_LINE, MMISMS_MSGBOX_TAB_CTRL_ID),
#endif
    END_WIN
};


// the window of  2.2.0 and 2.3.0  for message box
WINDOW_TABLE( MMISMS_MTBOX_CHILD_WIN_TAB ) =
{
    WIN_FUNC((uint32) HandleMtBoxChildWinMsg),
    WIN_ID( MMISMS_MTBOX_CHILD_WIN_ID ),

#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISMS_MTBOX_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMISMS_MTBOX_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISMS_MTBOX_LISTBOX_CTRL_ID, MMISMS_MTBOX_FORM_CTRL_ID),
#else
#ifndef MMI_GUI_STYLE_TYPICAL
    WIN_SOFTKEY(STXT_OPTION, TXT_VIEW, STXT_RETURN ),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_MTBOX_LISTBOX_CTRL_ID),
#endif
    END_WIN
};

WINDOW_TABLE( MMISMS_SENDSUCCBOX_CHILD_WIN_TAB ) =
{
    WIN_FUNC((uint32) HandleSendSuccBoxChildWinMsg),
    WIN_ID( MMISMS_SENDSUCCBOX_CHILD_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISMS_SENDSUCCBOX_FORM_CTRL_ID),

    CHILD_SOFTKEY_CTRL(TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMISMS_SENDSUCCBOX_FORM_CTRL_ID),

    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISMS_SENDSUCCBOX_LISTBOX_CTRL_ID, MMISMS_SENDSUCCBOX_FORM_CTRL_ID),
#else
#ifndef MMI_GUI_STYLE_TYPICAL
    WIN_SOFTKEY(STXT_OPTION, TXT_VIEW, STXT_RETURN ),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
#endif

    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_SENDSUCCBOX_LISTBOX_CTRL_ID),
#endif
    END_WIN
};

WINDOW_TABLE( MMISMS_SENDFAILBOX_CHILD_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleSendFailBoxChildWinMsg),
    WIN_ID( MMISMS_SENDFAILBOX_CHILD_WIN_ID ),

#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISMS_SENDFAILBOX_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMISMS_SENDFAILBOX_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISMS_SENDFAILBOX_LISTBOX_CTRL_ID, MMISMS_SENDFAILBOX_FORM_CTRL_ID),
#else
#ifndef MMI_GUI_STYLE_TYPICAL
    WIN_SOFTKEY(STXT_OPTION, TXT_VIEW, STXT_RETURN ),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_SENDFAILBOX_LISTBOX_CTRL_ID),
#endif
    END_WIN
};

WINDOW_TABLE( MMISMS_NOSENDBOX_CHILD_WIN_TAB ) =
{
    WIN_FUNC((uint32) HandleDraftBoxChildWinMsg),
    WIN_ID( MMISMS_NOSENDBOX_CHILD_WIN_ID ),
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    WIN_TITLE(TXT_COMM_DRAFT),
#endif
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISMS_DRAFTBOX_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMISMS_DRAFTBOX_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISMS_NOSENDBOX_LISTBOX_CTRL_ID, MMISMS_DRAFTBOX_FORM_CTRL_ID),
#else
#ifndef MMI_GUI_STYLE_TYPICAL
    WIN_SOFTKEY(STXT_OPTION, TXT_VIEW, STXT_RETURN ),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_NOSENDBOX_LISTBOX_CTRL_ID),
#endif
    END_WIN
};

#ifdef MMI_SMS_CHAT_SUPPORT
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
WINDOW_TABLE( MMISMS_CHATBOX_CHILD_WIN_TAB ) =
{
    WIN_FUNC((uint32) HandleChatBoxChildWinMsg),
    WIN_ID( MMISMS_CHATBOX_CHILD_WIN_ID ),
//     WIN_TITLE(TXT_COMMON_WRITE_MESSAGE),
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISMS_CHATBOX_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE, IMAGE_SMS_BUTTON_NORMAL, MMISMS_CHATBOX_WRITEMSG_BUTTON_CTRL_ID , MMISMS_CHATBOX_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISMS_CHATBOX_LISTBOX_CTRL_ID, MMISMS_CHATBOX_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_DELETE, TXT_NULL, STXT_CANCEL , MMICOMMON_SOFTKEY_CTRL_ID, MMISMS_CHATBOX_FORM_CTRL_ID),
#else

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISMS_CHATBOX_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISMS_CHATBOX_CHILD_BUTTON_FORM_CTRL_ID,MMISMS_CHATBOX_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE, PNULL, MMISMS_CHATBOX_NEWMSG_BUTTON_CTRL_ID , MMISMS_CHATBOX_CHILD_BUTTON_FORM_CTRL_ID),

    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISMS_CHATBOX_LISTBOX_CTRL_ID, MMISMS_CHATBOX_FORM_CTRL_ID),
   // CHILD_TEXT_CTRL(FALSE,MMISMS_CHATBOX_NEWMSG_BTEXT_CTRL_ID,MMISMS_CHATBOX_FORM_CTRL_ID),
    CHILD_TEXT_CTRL(FALSE,MMISMS_CHATBOX_NEWMSG_STEXT_CTRL_ID,MMISMS_CHATBOX_FORM_CTRL_ID),
    WIN_SOFTKEY(STXT_SOFTKEY_MENU_MK, STXT_SOFTKEY_OK_MK, TXT_COMMON_COMMON_BACK),
#endif

    END_WIN
};


#else
WINDOW_TABLE( MMISMS_CHATBOX_CHILD_WIN_TAB ) =
{
    WIN_FUNC((uint32) HandleChatBoxChildWinMsg),
    WIN_ID( MMISMS_CHATBOX_CHILD_WIN_ID ),
    WIN_TITLE(TXT_COMMON_WRITE_MESSAGE),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISMS_CHATBOX_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_NULL, TXT_COMMON_WRITE_MESSAGE, TXT_NULL, MMICOMMON_SOFTKEY_CTRL_ID, MMISMS_CHATBOX_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISMS_CHATBOX_LISTBOX_CTRL_ID, MMISMS_CHATBOX_FORM_CTRL_ID),
#else
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_CHATBOX_LISTBOX_CTRL_ID),
#ifndef MMI_GUI_STYLE_TYPICAL
    WIN_SOFTKEY(STXT_OPTION, TXT_VIEW, STXT_RETURN ),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
#endif
#endif

    END_WIN
};
#endif

WINDOW_TABLE(MMISMS_BLOCKNUMBER_TEXT_BOX_WIN_TAB) =
{
    WIN_FUNC((uint32) HandleBlockNumberTextBoxWinMsg ),
    WIN_ID(MMISMS_BLOCKNUMBER_WIN_ID),
    WIN_TITLE(TXT_SET_BLACKLIST_LIST),
    CREATE_TEXT_CTRL(MMISMS_SHOW_BLOCKNUMBER_TEXT_CTRL_ID),
    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, TXT_COMMON_COMMON_BACK),
    END_WIN
};

#endif


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : select sim card callback
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  SendSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinAddDataPtr(MMK_GetFocusWinId());

    if (isReturnOK && win_data_ptr != PNULL)
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if (!MMIAPICC_IsSimServiceAvail(dual_sys))
        {
            MMIAPICC_OpenSIMCallingPrompt();
            return recode;
        }
#endif

        MMISMS_ResendSMSbyOrderID(win_data_ptr->cur_focus_order_id,dual_sys);
    }
    return recode;
}
#endif


#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//     Description : to Enter the list of box
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_EnterSmsBox(
    MMISMS_BOX_TYPE_E        box_type
)
{
    BOOLEAN is_not_inited = FALSE;
    MMI_HANDLE_T win_handle = 0;
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = PNULL;

    if (MMK_IsOpenWin(MMISMS_CHATBOX_CHILD_WIN_ID))   //不可重入
    {
        //for cr122772 minghu.mao
        MMK_CloseWin(MMISMS_SHOW_CHAT_SMS_WIN_ID);
        return;
    }

    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T*)SCI_ALLOCAZ(sizeof(MMISMS_MESSAGEBOX_WINDATA_T));
    if(PNULL == win_data_ptr)
    {
       SCI_TRACE_LOW("mmisms win_data_ptr alloc fail");
       return;
    }
    is_not_inited = MMISMS_IsSimNotInited();

    if (MMISMS_IsOrderOk() && MMISMS_IsMMSOrderReady()) // 增加判断mms是否排序完毕
    {
        win_data_ptr->box_type = MMISMS_BOX_CHAT;
        win_data_ptr->win_id = MMISMS_CHATBOX_CHILD_WIN_ID;
#ifdef MMI_SMS_CHAT_SUPPORT
        win_data_ptr->cur_list_ctrl_id = MMISMS_CHATBOX_LISTBOX_CTRL_ID;
#endif
        MMISMS_SetCurType(box_type);
#ifdef MMI_SMS_CHAT_SUPPORT
        win_handle = MMK_CreateWin((uint32 *)MMISMS_CHATBOX_CHILD_WIN_TAB, PNULL);
 #endif
        if(win_handle == PNULL)
        {
            SCI_TRACE_LOW("mmisms MMISMS_SHOW_CHAT_SMS_WIN_TAB create error");
            SCI_FREE(win_data_ptr);
            return;
        }
        SCI_TRACE_LOW("mmisms MMISMS_SHOW_CHAT_SMS_WIN_TAB create success");
        win_data_ptr->win_handle = win_handle;
        MMK_SetWinUserData(win_handle, (ADD_DATA)win_data_ptr);

    }
    else if (is_not_inited)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_INIT);
    }
    else
    {
        // 需要开启等待窗口
        MMISMS_SetBoxType(box_type);
        MMISMS_OpenInitWaitingWindow(MMISMS_OPEN_BOX);
    }
}
#else
/*****************************************************************************/
//     Description : to Enter the list of box
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_EnterSmsBox(
    MMISMS_BOX_TYPE_E        box_type
)
{
    BOOLEAN is_not_inited = FALSE;
    MMI_HANDLE_T win_handle = 0;
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T*)SCI_ALLOCAZ(sizeof(MMISMS_MESSAGEBOX_WINDATA_T));
    if (MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID))
    {
        //for cr122772 minghu.mao
#ifdef MMI_SMS_CHAT_SUPPORT
        MMK_CloseWin(MMISMS_SHOW_CHAT_SMS_WIN_ID);
#endif
        return;
    }

    if(PNULL == win_data_ptr)
	{
	    SCI_TRACE_LOW("mmisms MMISMS_MSGBOX_MAIN_WIN_ID create success");
        return;
	}

    is_not_inited = MMISMS_IsSimNotInited();
    win_data_ptr->box_type = box_type;
    MMISMS_SetCurType(box_type);
    if (MMISMS_IsOrderOk() && MMISMS_IsMMSOrderReady()) // 增加判断mms是否排序完毕
    {

        win_data_ptr->win_id = MMISMS_MSGBOX_MAIN_WIN_ID;
        win_handle = MMK_CreateParentWin((uint32 *)MMISMS_MSGBOX_MAIN_WIN_TAB, PNULL);
        if(win_handle == PNULL)
        {
            SCI_TRACE_LOW("mmisms MMISMS_MSGBOX_MAIN_WIN_ID create error");
            SCI_FREE(win_data_ptr);
            return;
        }
        SCI_TRACE_LOW("mmisms MMISMS_MSGBOX_MAIN_WIN_ID create success");
        win_data_ptr->win_handle = win_handle;
        MMK_SetWinUserData(win_handle, (ADD_DATA)win_data_ptr);

#ifdef MMI_SMS_CHAT_SUPPORT
        MMK_CreateChildWin(MMISMS_MSGBOX_MAIN_WIN_ID, (uint32 *)MMISMS_CHATBOX_CHILD_WIN_TAB, NULL);
#endif
        MMK_CreateChildWin(MMISMS_MSGBOX_MAIN_WIN_ID, (uint32 *)MMISMS_MTBOX_CHILD_WIN_TAB, NULL);
        MMK_CreateChildWin(MMISMS_MSGBOX_MAIN_WIN_ID, (uint32 *)MMISMS_SENDFAILBOX_CHILD_WIN_TAB, NULL);
        MMK_CreateChildWin(MMISMS_MSGBOX_MAIN_WIN_ID, (uint32 *)MMISMS_NOSENDBOX_CHILD_WIN_TAB, NULL);
        MMK_CreateChildWin(MMISMS_MSGBOX_MAIN_WIN_ID, (uint32 *)MMISMS_SENDSUCCBOX_CHILD_WIN_TAB, NULL);

        switch (box_type)
        {
            case MMISMS_BOX_MT:
                MMK_SetChildWinFocus(MMISMS_MSGBOX_MAIN_WIN_ID, MMISMS_MTBOX_CHILD_WIN_ID);
                break;
            case MMISMS_BOX_SENDSUCC:
                MMK_SetChildWinFocus(MMISMS_MSGBOX_MAIN_WIN_ID, MMISMS_SENDSUCCBOX_CHILD_WIN_ID);
                break;
            case MMISMS_BOX_SENDFAIL:
                MMK_SetChildWinFocus(MMISMS_MSGBOX_MAIN_WIN_ID, MMISMS_SENDFAILBOX_CHILD_WIN_ID);
                break;
            case MMISMS_BOX_NOSEND:
                MMK_SetChildWinFocus(MMISMS_MSGBOX_MAIN_WIN_ID, MMISMS_NOSENDBOX_CHILD_WIN_ID);
                break;

#ifdef MMI_SMS_CHAT_SUPPORT
            case MMISMS_BOX_CHAT:
                MMK_SetChildWinFocus(MMISMS_MSGBOX_MAIN_WIN_ID, MMISMS_CHATBOX_CHILD_WIN_ID);
                break;
#endif
            default:
                MMK_SetChildWinFocus(MMISMS_MSGBOX_MAIN_WIN_ID, MMISMS_MTBOX_CHILD_WIN_ID);
                break;
        }

        if ( MMISMS_BOX_MT == box_type )
        {
            MMK_CloseWin( MMISMS_NEWMSG_WIN_ID );
        }
    }
    else if (is_not_inited)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_INIT);
    }
    else
    {
        // 需要开启等待窗口
        MMISMS_OpenInitWaitingWindow(MMISMS_OPEN_BOX);
    }
}
#endif

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL void MMISMS_OpenChatReadWin(MMISMS_ORDER_ID_T cur_order_id)
{
    MMI_STRING_T   prompt_str = {0};
#ifdef MMI_TIMERMSG_SUPPORT
    if (MMISMS_GetIsTimerMsg())
    {
        MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);

        return;
    }
#endif
    if(PNULL == cur_order_id)
    {
        return;
    }
    SCI_TRACE_LOW("mmisms MMISMS_OpenChatReadWin: open chat read win cur_order_id = %d",cur_order_id);
    //save chat number
    MMISMS_SetSMSUpdateInfo(cur_order_id);
    MMISMS_SetGlobalOperationOrderId(cur_order_id);
    MMISMS_EnterSMSChatReadWin(cur_order_id);
    MMI_GetLabelTextByLang( STR_NOTE_WAITING, &prompt_str );
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,MMISMS_HandleWaitWinMsg);
}
#endif

/*****************************************************************************/
//     Description : open read message
//    Global resource dependence :
//  Author:rong.gu
//    Note:读取内容的窗口(MT,MO Success)
/*****************************************************************************/
LOCAL void HandleMsgBoxOpenReadMessage(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    MMISMS_BOX_TYPE_E state_box_type = MMISMS_BOX_NONE;
    MMI_STRING_T   prompt_str = {0};
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
#ifdef MMI_TIMERMSG_SUPPORT
    if (MMISMS_GetIsTimerMsg())
    {
        MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);

        return;
    }
#endif
    cur_order_id = win_data_ptr->cur_focus_order_id;
    if(PNULL == cur_order_id)
    {
        return;
    }

    state_box_type = MMISMS_GetSmsBoxtype(cur_order_id);
    MMISMS_SetSMSUpdateInfo(cur_order_id);
    MMISMS_SetGlobalOperationOrderId(cur_order_id);
    switch(cur_order_id->flag.msg_type)
    {
        case MMISMS_TYPE_SMS:
#ifdef MMI_TIMERMSG_SUPPORT
            if (MMISMS_GetIsTimerMsg())
            {
                MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
                break;
            }
#endif
            if (MMISMS_IsSmsBusy(cur_order_id))
            {
                MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
                break;
            }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            if(MMISMS_BOX_NOSEND == state_box_type)
            {
                MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT_SMS);
            }
#endif
            MMI_GetLabelTextByLang( STR_NOTE_WAITING, &prompt_str );
            MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,MMISMS_HandleWaitWinMsg);

            MMISMSAPP_ReadOneSms(
                cur_order_id,
                MessageboxWinReadSmsCallback,
                PNULL,
                (void*)win_data_ptr->win_handle);

            break;

        case MMISMS_TYPE_MMS:
			//解决cr MS00150274,U盘使用过程中不能看MMS
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                break;
            }

#if defined (BROWSER_SUPPORT) && defined (MMS_SUPPORT)
#ifndef NANDBOOT_SUPPORT
            if (MMIAPIBROWSER_IsRunning())
            {
                MMIPUB_OpenAlertWarningWin(TXT_MMS_WEB_IN_USING);
                break;
            }
#endif
#endif
#ifdef MET_MEX_SUPPORT
            if (MMIMEX_ConflictPromt())
            {
                // mex kernel is running and may result in mem conflict
                break;
            }
#endif
            MMISMS_ClearOperInfo();
            switch(state_box_type)
            {
            case MMISMS_BOX_MT:
            case MMISMS_BOX_SENDSUCC:
            case MMISMS_BOX_SENDFAIL:
                MMIAPIMMS_ReadMMS((uint32)cur_order_id->record_id);
                break;
            case MMISMS_BOX_NOSEND:
                MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT);
                MMIAPIMMS_DraftEditMMS((uint32)cur_order_id->record_id);
                break;
            default:
                break;
            }

            //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
            if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
            {
                MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
            }
            if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
            {
                MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
            {
                MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
            }
            break;

        case MMISMS_TYPE_WAP_PUSH:
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            }
            else
            {

                MMISMS_ClearOperInfo();
#ifdef MMI_OTA_SUPPORT
                MMIAPIOTA_WapPushEnter((uint16)cur_order_id->record_id);
#endif
                //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
                if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
                {
                    MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
                }
                if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
                {
                    MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
                }
            }
            break;

        case MMISMS_TYPE_WAP_OTA:
        case MMISMS_TYPE_MMS_OTA:
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            }
            else
            {
                MMISMS_ClearOperInfo();
#ifdef MMI_OTA_SUPPORT
                MMIAPIOTA_OTAEnter((uint16)cur_order_id->record_id);
#endif
                //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
                if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
                {
                    MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
                }
                if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
                {
                    MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
                }
            }
            break;
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //Added by yuanl  2010.8.19
        case MMISMS_TYPE_MAIL:
            //读取邮件
            mail_ReadMailByRecordId((uint16)cur_order_id->record_id);
            break;
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */
        default:
            break;
    }

}

/*****************************************************************************/
//     Description : open read message
//    Global resource dependence :
//  Author:rong.gu
//    Note:显示内容的窗口(MT,MO Success)
/*****************************************************************************/
LOCAL void HandleMsgBoxShowSMS(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    MMISMS_READ_MSG_T *read_msg = PNULL;
	MMISMS_ClearOperInfo();
    read_msg = MMISMS_GetReadResult();
    MMISMS_EnterShowSmsWin(read_msg);
    MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
}


/*****************************************************************************/
//     Description : open read message
//    Global resource dependence :
//  Author:rong.gu
//    Note:编辑内容的窗口(DRAFT,MO FAIL)
/*****************************************************************************/
LOCAL void HandleMsgBoxEditSMS(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    MMI_STRING_T msg_content = {0};
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_ORDER_ID_T order_id = PNULL;
#endif
    uint8 num_len = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
	uint8 number[MMISMS_MAX_USERNUMBER_LEN + 2] = {0};
    // set the init SMS content
    MMISMS_GetSMSContent(&msg_content);
#ifdef MMI_SMS_CHAT_SUPPORT
    MMISMS_SetDraftOperationOrderId(win_data_ptr->cur_focus_order_id);
#endif
    num_len = MMISMS_GetCurNumByOrder(win_data_ptr->cur_focus_order_id,&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);

    // open edit window
    MMISMS_WriteNewMessage(
        MN_DUAL_SYS_MAX,
        &msg_content,
        FALSE,
        number,
        num_len
        );

    #ifdef MMI_TIMERMSG_SUPPORT
    if (MMISMS_BOX_SENDFAIL == win_data_ptr->box_type)
    {
        MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr = PNULL;

        MMISMS_SetCurOrderArrIndex(0);
        order_id = MMISMS_GetCurOperationOrderId();

        event_fast_info_ptr = MMISMS_TIMERMSG_GetEventByOrder(order_id);

        if (PNULL != event_fast_info_ptr)
        {
            MMISMS_SetIsTimerMode(TRUE);
            MMISMS_SetIsTimerMsgReplace(TRUE);
            MMISMS_TIMERMSG_SetCurEvent(event_fast_info_ptr);
        }
    }
    #endif

    if (PNULL != msg_content.wstr_ptr)
    {
        SCI_FREE(msg_content.wstr_ptr);
        msg_content.wstr_ptr = PNULL;
    }

    MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);

}

/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMTBoxDateChangeCallBack(MMIEVENT_INFO_T *event_info_ptr)
{
    MMI_WIN_ID_T        win_id = MMISMS_MTBOX_CHILD_WIN_ID;
    if(PNULL == event_info_ptr)
    {
       SCI_TRACE_LOW("mmisms MTBox event ptr PNULL");
       return;
    }
    SCI_TRACE_LOW("mmisms HandleMTBoxDateChangeCallBack ");
    if(SMS_DATA_CHANGE_EVENT == event_info_ptr->event_type)
    {
        MMK_SetWinDirty(win_id, TRUE);
        if(MMK_IsFocusWin(win_id))
        {
           MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
    }
}

/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMtBoxChildWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM            param        //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_MTBOX_LISTBOX_CTRL_ID;
    uint16 selected_index = 0;
    uint16 total_num = 0;
    LOCAL BOOLEAN s_have_items = FALSE;
    LOCAL BOOLEAN s_need_update = FALSE;
    LOCAL BOOLEAN s_need_close_new_msg_win   = FALSE;
    GUILIST_MARK_ITEM_T *list_param_ptr = NULL;
    uint16 i = 0;
    uint16 index = 0;
    uint16 top_index = 0;
    uint16 page_num = 0;
	MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMK_GetParentWinHandle(win_id));
    // GUI_RECT_T  list_rect = {0};
#ifndef MMI_PDA_SUPPORT
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#endif
#ifdef MMI_PDA_SUPPORT
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
#endif
	if(PNULL == win_data_ptr)
	{
		//SCI_PASSERT(0,("mmisms win_data_ptr PNULL"));
		return MMI_RESULT_FALSE;
	}
    //SCI_TRACE_LOW:"MMISMS: HandleMtBoxChildWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_6858_112_2_18_2_54_34_744, (uint8*)"d", msg_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
#ifdef MMI_GUI_STYLE_TYPICAL
#ifdef  TOUCH_PANEL_SUPPORT
            MMISMS_CreateToolbar(win_id, MMISMS_TOOLBAR_MTBOX_CTRL_ID, ctrl_id);
#endif
#endif
#ifndef MMI_PDA_SUPPORT
            GUIAPICTRL_SetBothRect(ctrl_id, &list_both_rect);
#endif
            MsgBoxBuildWinUserData(win_data_ptr);

            s_need_update = FALSE;
            s_need_close_new_msg_win = FALSE;

            GUILIST_SetNeedPrgbar(ctrl_id, TRUE);

            // load items to list box
            s_have_items = MMISMS_LoadSmsListFromOrder(MMISMS_BOX_MT, ctrl_id );
            if (s_have_items)
            {
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
            }
            else
            {
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
            }

#ifdef MMI_GUI_STYLE_TYPICAL
#ifdef  TOUCH_PANEL_SUPPORT
        if(MMISMS_BOX_MT == MMISMS_GetCurBoxType())
        {
            MMISMS_MTBoxUpdateWinToolbar();
        }
#endif
#endif

#ifdef MMI_PDA_SUPPORT
            child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
            GUIFORM_SetChildHeight(MMISMS_MTBOX_FORM_CTRL_ID, ctrl_id, &child_height);
            GUIFORM_IsSlide(MMISMS_MTBOX_FORM_CTRL_ID, FALSE);
            GUIFORM_PermitChildBg(MMISMS_MTBOX_FORM_CTRL_ID, FALSE);
            GUILIST_PermitBorder(ctrl_id, FALSE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIFORM_SetChildDisplay(MMISMS_MTBOX_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
            GUILIST_SetDefaultIcon(MMISMS_MTBOX_LISTBOX_CTRL_ID, IMAGE_SMS_CHAT_DEFAULT, IMAGE_SMS_CHAT_DEFAULT);
            win_data_ptr->is_mark_all = FALSE;
#endif
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleMTBoxDateChangeCallBack, TRUE);
            MMK_SetAtvCtrl(win_id, ctrl_id);
#ifdef MMI_GUI_STYLE_TYPICAL
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
#endif
            break;

#ifdef MMI_GUI_STYLE_TYPICAL
#ifdef  TOUCH_PANEL_SUPPORT
    case MSG_LCD_SWITCH:
        {
            MMISMS_MTBoxUpdateWinToolbar();
        }
        break;
#endif
#endif
        case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;

            if (PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:"MMISMS:MSG_CTL_LIST_NEED_ITEM_DATA need_item_data_ptr = PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_6925_112_2_18_2_54_34_745, (uint8*)"");
                break;
            }
            index = need_item_data_ptr->item_index;
            MMISMS_SetListItemData(MMISMS_BOX_MT, ctrl_id, index);
        }
        break;

#ifndef MMI_GUI_STYLE_TYPICAL
        case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if (PNULL != need_item_content_ptr)
            {
                MMISMS_DrawListBoxItemWithIconData(need_item_content_ptr->item_index, need_item_content_ptr->item_content_index, ctrl_id, MMISMS_BOX_MT);
            }
            break;
        }
#endif

        case MMISMS_MSG_NEEDUPDATELIST:
            s_need_update = TRUE;
            break;

        case MSG_LOSE_FOCUS:

		#ifdef HERO_ENGINE_TTS_SUPPORT 
			MMIAPITTS_StopPlayTextForControl();
		#endif

            if (s_need_close_new_msg_win)
            {
                MMK_CloseWin(MMISMS_NEWMSG_WIN_ID);
                s_need_close_new_msg_win = FALSE;
            }
            break;

        case MSG_GET_FOCUS:
            if (win_data_ptr->box_type != MMISMS_BOX_MT) //从其他信箱切换回来
            {
                //清除掉原来的标记
                win_data_ptr->marked_sms_num = 0;
                for (i = 0; i < GUILIST_GetTotalItemNum(ctrl_id); i++)
                {
                    GUILIST_SetSelectedItem(ctrl_id, i, FALSE);
                }
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);
#ifndef MMI_PDA_SUPPORT
				if(s_have_items == 0)
				{
					 GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);
				}
				else
				{
                GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, FALSE);
				}
#endif
                MMISMS_SetAllMsgMarked(MMISMS_BOX_MT, FALSE);

#ifdef MMI_PDA_SUPPORT
                GUIFORM_SetChildDisplay(MMISMS_MTBOX_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
                //重新load信息list
                MMISMS_SetCurType(MMISMS_BOX_MT);
                win_data_ptr->box_type = MMISMS_BOX_MT;

                if (MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID))
                {
#ifdef MMI_SMS_CHAT_SUPPORT
                    GUIMENU_SetSelectedItem(MMISMS_MENU_CTRL_ID, (uint16)MMISMS_MAIN_MENU_NODE_INBOX - MMISMS_MAIN_MENU_NODE_CHAT);
#else
                    GUIMENU_SetSelectedItem(MMISMS_MENU_CTRL_ID, (uint16)MMISMS_MAIN_MENU_NODE_INBOX - MMISMS_MAIN_MENU_NODE_WRITE_NEW_MSG);
#endif
                }
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_MT, ctrl_id );
                if (s_have_items)
                {
                    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
                }
                else
                {
                    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
                }
                GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                s_need_update = FALSE;
#ifdef MMI_PDA_SUPPORT
                win_data_ptr->is_mark_all = FALSE;
#endif
            }
            else
            {
#ifndef MMI_PDA_SUPPORT
				if(s_have_items == 0)
				{
					 GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);
				}
				else
				{
	                if (GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
	                {
	                    GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
	                }
	                else
	                {
	                    GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, FALSE);
	                }
				}
				
#endif

                if (s_need_update || MMISMS_IsNeedSort())
                {
                    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                    s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_MT, ctrl_id );
                    if (s_have_items)
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
                    }
                    else
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
                    }
                    GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                    s_need_update = FALSE;
                }
            }
            win_data_ptr->cur_list_ctrl_id = ctrl_id;
            GUILIST_ValidCurItemPos(ctrl_id);
#ifdef MMI_GUI_STYLE_TYPICAL
#ifdef  TOUCH_PANEL_SUPPORT
            MMISMS_MTBoxUpdateWinToolbar();
#endif
#endif
            MMK_CloseWin(MMISMS_NEWMSG_WIN_ID);
#ifdef MMI_PDA_SUPPORT
            if ( s_have_items )
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            }
            else
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            }

#endif
            break;

        case MSG_FULL_PAINT:
            if(MMK_GetWinDirty(win_id))
            {
                if (MMK_IsFocusWin(win_id))
                {
                    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                    total_num = GUILIST_GetTotalItemNum(ctrl_id);
                    top_index = GUILIST_GetTopItemIndex(ctrl_id);
                    page_num = GUILIST_GetPageItemNum(ctrl_id);

                    s_have_items = MMISMS_LoadSmsListFromOrder(MMISMS_BOX_MT, ctrl_id);
                    if (s_have_items)
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
                    }
                    else
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
                    }

                    if (total_num < GUILIST_GetTotalItemNum(ctrl_id)) //有增加新信息
                    {
                        if ((top_index == 0) && (selected_index != (page_num - 1)))
                        {
                            GUILIST_SetCurItemIndex(ctrl_id, (uint16)(selected_index + 1));
                            GUILIST_SetTopItemIndex(ctrl_id, top_index);
                        }
                        else
                        {
                            GUILIST_SetCurItemIndex(ctrl_id, (uint16)(selected_index + 1));
                            GUILIST_SetTopItemIndex(ctrl_id, top_index + 1);
                        }
                    }

                    else
                    {
                        if (selected_index >= GUILIST_GetTotalItemNum(ctrl_id)) // 删除的 情况
                        {
                            GUILIST_SetCurItemIndex(ctrl_id, 0);
                            GUILIST_SetTopItemIndex(ctrl_id, 0);
                        }
                        else
                        {
                            GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                            GUILIST_SetTopItemIndex(ctrl_id, top_index);
                        }
                    }
        #ifdef MMI_GUI_STYLE_TYPICAL
        #ifdef  TOUCH_PANEL_SUPPORT
                    MMISMS_MTBoxUpdateWinToolbar();
        #endif
        #endif
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                    MMK_SetWinDirty(win_id,FALSE);
                }
            }
            break;

#ifdef MMI_GUI_STYLE_TYPICAL
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_NOTIFY_LIST_MOVEBOTTOM:
        case MSG_NOTIFY_LIST_MOVETOP:
        case MSG_NOTIFY_LIST_MOVEDOWN:
        case MSG_NOTIFY_LIST_MOVEUP:
        case MSG_NOTIFY_LIST_PREPAGE:
        case MSG_NOTIFY_LIST_NXTPAGE:
        {
            GUITOOLBAR_SetVisible(MMISMS_TOOLBAR_MTBOX_CTRL_ID, TRUE);
            GUITOOLBAR_SetShowType(MMISMS_TOOLBAR_MTBOX_CTRL_ID, GUITOOLBAR_SHRINK);
            MMISMS_MTBoxUpdateWinToolbar();
        }
        break;
        case MSG_CTL_LIST_SLIDE_STATE_CHANGE:
        {
            GUILIST_SLIDE_STATE_E  state = *(GUILIST_SLIDE_STATE_E*)param;
            if (GUILIST_START_SLIDE == state)
            {
                GUITOOLBAR_SetVisible(MMISMS_TOOLBAR_MTBOX_CTRL_ID, FALSE);
            }
            else if (GUILIST_END_SLIDE == state)
            {
                MMISMS_MTBoxUpdateWinToolbar();
            }
        }
        break;
#endif
#endif
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
        {

            #ifdef MMI_GUI_STYLE_TYPICAL
            #ifdef TOUCH_PANEL_SUPPORT
            MMI_NOTIFY_T notify = {0};
            uint16       current_index = 0;
            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
            if (MMISMS_TOOLBAR_MTBOX_CTRL_ID == notify.src_id)
            {
                SMS_TOOLBAR_TYPE_E node_id = SMS_TOOLBAR_TYPE_INIT;
                node_id = (SMS_TOOLBAR_TYPE_E)GUITOOLBAR_GetItemUserDate(MMISMS_TOOLBAR_MTBOX_CTRL_ID, GUITOOLBAR_GetClickItemIndex(MMISMS_TOOLBAR_MTBOX_CTRL_ID));
                current_index = GUILIST_GetCurItemIndex(ctrl_id);
                MMISMS_SetCurOperationOrderId(MMISMS_GetCurOperationType(), current_index);
                MMISMS_HandleToolBarItemSelect(win_id,node_id);
            }
            else
            #endif
            #endif
            {
#ifdef MMI_PDA_SUPPORT
                if (GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
                {
                    uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
                    switch (src_id)
                    {
                        case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                        {
                            win_data_ptr->is_mark_all = MMISMS_HandleMarkAll(win_data_ptr->is_mark_all, win_id, ctrl_id, win_data_ptr->box_type,&win_data_ptr->marked_sms_num);
                        }
                        break;

                        case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                            if (win_data_ptr->marked_sms_num > 0 )
                            {
                                if (MMISMS_IsUpdateOk())
                                {
                                    //MsgBoxOpenDelteQueryWin(win_data_ptr);
                                    MMISMS_OpenDelteQueryWin(MMISMS_DELETE_PART,FALSE,win_data_ptr->cur_focus_order_id,win_data_ptr->box_type,HandleMessageBoxDelQueryWinMsg);
                                }
                            }
                            else
                            {
                                MMISMS_OpenAlertWarningWin(TXT_SMS_SELECT_ONT_PROMPT);
                            }
                            break;

                        case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                        {
                            win_data_ptr->marked_sms_num = 0;
                            win_data_ptr->is_mark_all = FALSE;
                            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                            MMISMS_SetAllMsgMarked(MMISMS_BOX_MT, FALSE);

                            GUIFORM_SetChildDisplay(MMISMS_MTBOX_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
                            if (MMK_IsFocusWin(win_id))
                            {
                                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
                            }
                        }
                        break;

                        default:
                            break;
                    }
                    break;
                }
#endif
                if (s_have_items)
                {
                    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                    win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_MT, selected_index);
                    //MMISMS_OpenMsgInSmsList(win_data_ptr->cur_focus_order_id,win_data_ptr->box_type);
                    HandleMsgBoxOpenReadMessage(win_data_ptr);
                }
            }
            break;
        }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef MMI_PDA_SUPPORT
        case MSG_APP_MENU:
            {
                 BOOLEAN  is_enable_mark = FALSE;
                 is_enable_mark = GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK);
                 if(!is_enable_mark)
                 {
                    win_data_ptr->is_mark_all = FALSE;
                 }
                 win_data_ptr->marked_sms_num = GUILIST_GetSelectedItemNum(ctrl_id);
                 //init softkey
                 GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE,STXT_CANCEL,TRUE);
                 HandleMsgBoxWinOpenMenuKeyOpt(win_data_ptr,is_enable_mark);
            }
                 break;
        case MSG_CTL_LIST_LONGOK:
#endif
        case MSG_CTL_OK:
        case MSG_APP_OK:
            {
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_MT, selected_index);
				win_data_ptr->marked_sms_num = GUILIST_GetSelectedItemNum(ctrl_id);
                if(s_have_items)
                {
                    HandleMsgBoxWinOpenMTOpt(win_data_ptr);
                }
            }
            break;

        case MSG_SMS_FINISH_READ:
            //MMISMS_HandleSmsFinishRead(win_data_ptr->cur_focus_order_id,win_data_ptr->box_type,ctrl_id,&win_data_ptr->marked_sms_num);
            HandleMsgBoxShowSMS(win_data_ptr);
            break;

        case MSG_KEYUP_GREEN:
#ifdef VT_SUPPORT
        case MSG_KEYLONG_GREEN:
#endif
#ifndef MMI_MULTI_GREEN_KEY_SINGLE
        case MSG_KEYUP_CALL2:
        case MSG_KEYUP_CALL3:
        case MSG_KEYUP_CALL4:
#endif
            {
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_MT, selected_index);
                HandleMsgBoxWinSmsKeyCall(win_data_ptr,msg_id);
            }
            break;

        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MMISMS_MSG_READ_MSG:
            if (s_have_items && (win_data_ptr->marked_sms_num == 0) )
            {
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_MT, selected_index);
                //MMISMS_OpenMsgInSmsList(win_data_ptr->cur_focus_order_id,win_data_ptr->box_type);
                HandleMsgBoxOpenReadMessage(win_data_ptr);
            }
            break;

        case MSG_CTL_LIST_MARK_ITEM:
            //to get the info of current selected item
            list_param_ptr = (GUILIST_MARK_ITEM_T*)param;

            //MMISMS_SetCurOperationOrderId(MMISMS_BOX_MT, list_param_ptr->item_idx);

            win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_MT,list_param_ptr->item_idx);

#ifdef MMI_PDA_SUPPORT
            MMISMS_SetSoftkeyAfterMarkOpera(win_id, ctrl_id, &win_data_ptr->is_mark_all);
#endif

            if (ctrl_id == list_param_ptr->ctrl_id && win_data_ptr->cur_focus_order_id != PNULL)
            {
                if (list_param_ptr->had_maked)
                {
                    MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,TRUE);
                    win_data_ptr->marked_sms_num++;
                }
                else
                {
                    MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
                    win_data_ptr->marked_sms_num--;
#ifndef MMI_PDA_SUPPORT
                    // 如果标记数为零，关闭标记状态
                    if (0 == win_data_ptr->marked_sms_num)
                    {
                        GUILIST_SetTextListMarkable(ctrl_id, FALSE);
#ifdef MMI_GUI_STYLE_TYPICAL
#ifdef TOUCH_PANEL_SUPPORT
                        MMISMS_MTBoxUpdateWinToolbar();
#endif
#else
                        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, TRUE);
#endif
                    }
#endif
                }
            }
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseParentWin(MMISMS_MSGBOX_MAIN_WIN_ID);
            break;

        case MSG_CLOSE_WINDOW:
            win_data_ptr->marked_sms_num = 0;
            MMISMS_SetAllMsgMarked(MMISMS_BOX_MT, FALSE);
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleMTBoxDateChangeCallBack, FALSE);
            if (s_need_close_new_msg_win)
            {
                MMK_CloseWin(MMISMS_NEWMSG_WIN_ID);
                MMK_CloseWin(MMICMSBRW_NEWPUSH_WIN_ID);

                MMIAPIMMS_CloseNewMMSPromptWin();

                s_need_close_new_msg_win = FALSE;
            }
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSendSuccBoxDateChangeCallBack(MMIEVENT_INFO_T *event_info_ptr)
{
    MMI_WIN_ID_T win_id = MMISMS_SENDSUCCBOX_CHILD_WIN_ID;
    if(PNULL == event_info_ptr)
    {
       SCI_TRACE_LOW("mmisms MTBox event ptr PNULL");
       return;
    }
    SCI_TRACE_LOW("mmisms HandleSendSuccBoxDateChangeCallBack ");
    if(SMS_DATA_CHANGE_EVENT == event_info_ptr->event_type)
    {
        MMK_SetWinDirty(win_id, TRUE);
        if(MMK_IsFocusWin(win_id))
        {
           MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
    }
}
/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendSuccBoxChildWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E msg_id,        //IN:
    DPARAM            param        //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SENDSUCCBOX_LISTBOX_CTRL_ID;
    uint16 selected_index = 0;
    LOCAL BOOLEAN s_have_items = FALSE;
    LOCAL BOOLEAN s_need_update = FALSE;
    GUILIST_MARK_ITEM_T *list_param_ptr = NULL;
    uint16 i = 0;
    uint16 index = 0;
    // GUI_RECT_T  list_rect = {0};
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifdef MMI_PDA_SUPPORT
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
#endif
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMK_GetParentWinHandle(win_id));
	if(PNULL == win_data_ptr)
	{
		//SCI_PASSERT(0,("mmisms win_data_ptr PNULL"));
		return MMI_RESULT_FALSE;
	}

    //SCI_TRACE_LOW:"MMISMS: HandleSendSuccBoxChildWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_7377_112_2_18_2_54_35_746, (uint8*)"d", msg_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIAPICTRL_SetBothRect(ctrl_id, &list_both_rect);

            GUILIST_SetNeedPrgbar(ctrl_id, TRUE);
            MsgBoxBuildWinUserData(win_data_ptr);

            // load items to list box
            s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_SENDSUCC, ctrl_id );
            if (s_have_items)
            {
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
            }
            else
            {
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
            }

#ifdef MMI_PDA_SUPPORT
            child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
            GUIFORM_SetChildHeight(MMISMS_SENDSUCCBOX_FORM_CTRL_ID, ctrl_id, &child_height);
            GUIFORM_IsSlide(MMISMS_SENDSUCCBOX_FORM_CTRL_ID, FALSE);
            GUIFORM_PermitChildBg(MMISMS_SENDSUCCBOX_FORM_CTRL_ID, FALSE);
            GUILIST_PermitBorder(ctrl_id, FALSE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIFORM_SetChildDisplay(MMISMS_SENDSUCCBOX_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
            GUILIST_SetDefaultIcon(MMISMS_SENDSUCCBOX_LISTBOX_CTRL_ID, IMAGE_SMS_CHAT_DEFAULT, IMAGE_SMS_CHAT_DEFAULT);
            win_data_ptr->is_mark_all = FALSE;
#endif
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleSendSuccBoxDateChangeCallBack, TRUE);
            MMK_SetAtvCtrl(win_id, ctrl_id);
#ifdef MMI_GUI_STYLE_TYPICAL
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
#endif
            s_need_update = FALSE;
            break;

        case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;

            if (PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:"MMISMS:MSG_CTL_LIST_NEED_ITEM_DATAa need_item_data_ptr = PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_7421_112_2_18_2_54_35_747, (uint8*)"");
                break;
            }
            index = need_item_data_ptr->item_index;
            MMISMS_SetListItemData(MMISMS_BOX_SENDSUCC, ctrl_id, index);
        }
        break;
#ifndef MMI_GUI_STYLE_TYPICAL
        case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if (PNULL != need_item_content_ptr)
            {
                MMISMS_DrawListBoxItemWithIconData(need_item_content_ptr->item_index, need_item_content_ptr->item_content_index, ctrl_id, MMISMS_BOX_SENDSUCC);
            }
            break;
        }
#endif
        case MSG_CTL_LIST_MARK_ITEM:
            //to get the info of current selected item
            list_param_ptr = (GUILIST_MARK_ITEM_T*)param;

            win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_SENDSUCC,list_param_ptr->item_idx);
#ifdef MMI_PDA_SUPPORT
            MMISMS_SetSoftkeyAfterMarkOpera(win_id, ctrl_id, &win_data_ptr->is_mark_all);
#endif
            if (ctrl_id == list_param_ptr->ctrl_id && win_data_ptr->cur_focus_order_id != PNULL)
            {
                if (list_param_ptr->had_maked)
                {
                    MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,TRUE);
                    win_data_ptr->marked_sms_num++;
                }
                else
                {
                    MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
                    win_data_ptr->marked_sms_num--;
#ifndef MMI_PDA_SUPPORT
                    // 如果标记数为零，关闭标记状态
                    if (0 == win_data_ptr->marked_sms_num)
                    {
                        GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, TRUE);
                    }
#endif
                }
            }
            break;

        case MMISMS_MSG_NEEDUPDATELIST:
            s_need_update = TRUE;
            break;

        case MSG_GET_FOCUS:
            if (win_data_ptr->box_type != MMISMS_BOX_SENDSUCC) //从其他信箱切换回来
            {
                //清除掉原来的标记
                win_data_ptr->marked_sms_num = 0;

                for (i = 0; i < GUILIST_GetTotalItemNum(ctrl_id); i++)
                {
                    GUILIST_SetSelectedItem(ctrl_id, i, FALSE);
                }
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);
#ifndef MMI_PDA_SUPPORT
				if(s_have_items == 0)
				{
					 GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);
				}
				else
				{
                GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, FALSE);
				}
#endif
                MMISMS_SetAllMsgMarked(MMISMS_BOX_SENDSUCC, FALSE);
#ifdef MMI_PDA_SUPPORT
                GUIFORM_SetChildDisplay(MMISMS_SENDSUCCBOX_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif

                //重新load信息list
                MMISMS_SetCurType(MMISMS_BOX_SENDSUCC);
                win_data_ptr->box_type = MMISMS_BOX_SENDSUCC;
                win_data_ptr->cur_list_ctrl_id = ctrl_id;
                if (MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID))
                {
#ifdef MMI_SMS_CHAT_SUPPORT
                    GUIMENU_SetSelectedItem(MMISMS_MENU_CTRL_ID, (uint16)MMISMS_MAIN_MENU_NODE_SENTBOX - MMISMS_MAIN_MENU_NODE_CHAT);
#else
                    GUIMENU_SetSelectedItem(MMISMS_MENU_CTRL_ID, (uint16)MMISMS_MAIN_MENU_NODE_SENTBOX - MMISMS_MAIN_MENU_NODE_WRITE_NEW_MSG);
#endif
                }
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_SENDSUCC, ctrl_id );
                if (s_have_items)
                {
                    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
                }
                else
                {
                    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
                }
                GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                s_need_update = FALSE;
#ifdef MMI_PDA_SUPPORT
                win_data_ptr->is_mark_all = FALSE;
#endif
            }
            else
            {
#ifndef MMI_PDA_SUPPORT
				if(s_have_items == 0)
				{
					 GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);
				}
				else
				{
	                if (GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
	                {
	                    GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
	                }
	                else
	                {
	                    GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, FALSE);
	                }
				}
#endif

                if (s_need_update || MMISMS_IsNeedSort())
                {
                    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                    s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_SENDSUCC, ctrl_id );
                    if (s_have_items)
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
                    }
                    else
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
                    }
                    GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                    s_need_update = FALSE;
                }
            }
            win_data_ptr->cur_list_ctrl_id = ctrl_id;
            GUILIST_ValidCurItemPos(ctrl_id);
#ifdef MMI_PDA_SUPPORT
            if ( s_have_items )
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            }
            else
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            }

#endif
            break;

        case MSG_FULL_PAINT:
            if(MMK_GetWinDirty(win_id))
            {
                if (MMK_IsFocusWin(win_id))
                {
                    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                    s_have_items = MMISMS_LoadSmsListFromOrder(MMISMS_BOX_SENDSUCC, ctrl_id);
                    if (s_have_items)
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
                    }
                    else
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
                    }
                    GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                    MMK_SetWinDirty(win_id, FALSE);
                }
            }
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT
            if (GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
            {
                uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
                switch (src_id)
                {
                    case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                    {
                        win_data_ptr->is_mark_all = MMISMS_HandleMarkAll(win_data_ptr->is_mark_all, win_id, ctrl_id, win_data_ptr->box_type,&win_data_ptr->marked_sms_num);
                    }
                    break;

                    case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                        if (win_data_ptr->marked_sms_num > 0 )
                        {
                            if (MMISMS_IsUpdateOk())
                            {
                                //MsgBoxOpenDelteQueryWin(win_data_ptr);
                                MMISMS_OpenDelteQueryWin(MMISMS_DELETE_PART,FALSE,win_data_ptr->cur_focus_order_id,win_data_ptr->box_type,HandleMessageBoxDelQueryWinMsg);
                            }
                        }
                        else
                        {
                            MMISMS_OpenAlertWarningWin(TXT_SMS_SELECT_ONT_PROMPT);
                        }
                        break;

                    case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                    {
                        win_data_ptr->marked_sms_num = 0;
                        win_data_ptr->is_mark_all = FALSE;
                        GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                        MMISMS_SetAllMsgMarked(MMISMS_BOX_SENDSUCC, FALSE);

                        GUIFORM_SetChildDisplay(MMISMS_SENDSUCCBOX_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
                        if (MMK_IsFocusWin(win_id))
                        {
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
                        }
                    }
                    break;

                    default:
                        break;
                }
                break;
            }
#endif
            if (s_have_items)
            {
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_SENDSUCC, selected_index);
                //MMISMS_OpenMsgInSmsList(win_data_ptr->cur_focus_order_id,win_data_ptr->box_type);
                HandleMsgBoxOpenReadMessage(win_data_ptr);
            }
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527


#ifdef MMI_PDA_SUPPORT
        case MSG_APP_MENU:
             {
                 BOOLEAN  is_enable_mark = FALSE;
                 is_enable_mark = GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK);
                 if(!is_enable_mark)
                 {
                    win_data_ptr->is_mark_all = FALSE;
                 }
                 win_data_ptr->marked_sms_num = GUILIST_GetSelectedItemNum(ctrl_id);
                 //init softkey
                 GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE,STXT_CANCEL,TRUE);
                 HandleMsgBoxWinOpenMenuKeyOpt(win_data_ptr,is_enable_mark);
                 break;
            }

        case MSG_CTL_LIST_LONGOK:
#endif
        case MSG_CTL_OK:
        case MSG_APP_OK:
            {
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_SENDSUCC, selected_index);
                win_data_ptr->marked_sms_num = GUILIST_GetSelectedItemNum(ctrl_id);
                if(s_have_items)
                {
                    HandleMsgBoxWinOpenSendSuccOpt(win_data_ptr);
                }
            }
            break;
        case MSG_SMS_FINISH_READ:
            {
                HandleMsgBoxShowSMS(win_data_ptr);
            }
            break;
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MMISMS_MSG_READ_MSG:
            if (s_have_items && (win_data_ptr->marked_sms_num == 0) )
            {
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_SENDSUCC, selected_index);
                //MMISMS_OpenMsgInSmsList(win_data_ptr->cur_focus_order_id,win_data_ptr->box_type);
                HandleMsgBoxOpenReadMessage(win_data_ptr);
            }
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseParentWin(MMISMS_MSGBOX_MAIN_WIN_ID);
            break;

        case MSG_CLOSE_WINDOW:
            win_data_ptr->marked_sms_num = 0;
            MMISMS_SetAllMsgMarked(MMISMS_BOX_SENDSUCC, FALSE);
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleSendSuccBoxDateChangeCallBack, FALSE);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}


/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSendFailBoxDateChangeCallBack(MMIEVENT_INFO_T *event_info_ptr)
{
    MMI_WIN_ID_T win_id = MMISMS_SENDFAILBOX_CHILD_WIN_ID;
    if(PNULL == event_info_ptr)
    {
       SCI_TRACE_LOW("mmisms MTBox event ptr PNULL");
       return;
    }
    SCI_TRACE_LOW("mmisms HandleSendFailBoxDateChangeCallBack ");
    if(SMS_DATA_CHANGE_EVENT == event_info_ptr->event_type)
    {
        MMK_SetWinDirty(win_id, TRUE);
        if(MMK_IsFocusWin(win_id))
        {
           MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
    }
}
/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendFailBoxChildWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E msg_id,        //IN:
    DPARAM            param        //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SENDFAILBOX_LISTBOX_CTRL_ID;
    uint16 selected_index = 0;
    LOCAL BOOLEAN s_have_items = FALSE;
    LOCAL BOOLEAN s_need_update = FALSE;
    GUILIST_MARK_ITEM_T* list_param_ptr = NULL;
    uint16 i = 0;
    uint16 index = 0;
    // GUI_RECT_T      list_rect = {0};
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);

#ifdef MMI_PDA_SUPPORT
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
#endif
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMK_GetParentWinHandle(win_id));
	if(PNULL == win_data_ptr)
	{
		//SCI_PASSERT(0,("mmisms win_data_ptr PNULL"));
		return MMI_RESULT_FALSE;
	}
    //SCI_TRACE_LOW:"MMISMS: HandleSendFailBoxChildWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_7757_112_2_18_2_54_36_748, (uint8*)"d", msg_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIAPICTRL_SetBothRect(ctrl_id, &list_both_rect);

            GUILIST_SetNeedPrgbar(ctrl_id, TRUE);

            MsgBoxBuildWinUserData(win_data_ptr);
            // load items to list box
            s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_SENDFAIL, ctrl_id );
            if (s_have_items)
            {
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
            }
            else
            {
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
            }
            //GUILIST_SetNeedTitleArrow(ctrl_id,TRUE);//@baokun fix cr101674

#ifdef MMI_PDA_SUPPORT
            child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
            GUIFORM_SetChildHeight(MMISMS_SENDFAILBOX_FORM_CTRL_ID, ctrl_id, &child_height);
            GUIFORM_IsSlide(MMISMS_SENDFAILBOX_FORM_CTRL_ID, FALSE);
            GUIFORM_PermitChildBg(MMISMS_SENDFAILBOX_FORM_CTRL_ID, FALSE);
            GUILIST_PermitBorder(ctrl_id, FALSE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIFORM_SetChildDisplay(MMISMS_SENDFAILBOX_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
            GUILIST_SetDefaultIcon(MMISMS_SENDFAILBOX_LISTBOX_CTRL_ID, IMAGE_SMS_CHAT_DEFAULT, IMAGE_SMS_CHAT_DEFAULT);
            win_data_ptr->is_mark_all = FALSE;
#endif
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleSendFailBoxDateChangeCallBack, TRUE);
            MMK_SetAtvCtrl(win_id, ctrl_id);
#ifdef MMI_GUI_STYLE_TYPICAL
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
#endif
            s_need_update = FALSE;
            break;

        case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;

            if (PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:"MMISMS:MSG_CTL_LIST_NEED_ITEM_DATA need_item_data_ptr = PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_7802_112_2_18_2_54_36_749, (uint8*)"");
                break;
            }
            index = need_item_data_ptr->item_index;
            MMISMS_SetListItemData(MMISMS_BOX_SENDFAIL, ctrl_id, index);
        }
        break;

#ifndef MMI_GUI_STYLE_TYPICAL
        case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if (PNULL != need_item_content_ptr)
            {
                MMISMS_DrawListBoxItemWithIconData(need_item_content_ptr->item_index, need_item_content_ptr->item_content_index, ctrl_id, MMISMS_BOX_SENDFAIL);
            }
            break;
        }
#endif

        case MSG_CTL_LIST_MARK_ITEM:
            //to get the info of current selected item
            list_param_ptr = (GUILIST_MARK_ITEM_T*)param;

            //MMISMS_SetCurOperationOrderId(MMISMS_BOX_SENDFAIL, list_param_ptr->item_idx);
            win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_SENDFAIL,list_param_ptr->item_idx);
#ifdef MMI_PDA_SUPPORT
            MMISMS_SetSoftkeyAfterMarkOpera(win_id, ctrl_id, &win_data_ptr->is_mark_all);
#endif

            if (ctrl_id == list_param_ptr->ctrl_id)
            {
                if (list_param_ptr->had_maked)
                {
                    MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,TRUE);
                    win_data_ptr->marked_sms_num++;
                }
                else
                {
                    MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
                    win_data_ptr->marked_sms_num--;
                    // 如果标记数为零，关闭标记状态
#ifndef MMI_PDA_SUPPORT
                    if (0 == win_data_ptr->marked_sms_num)
                    {
                        GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, TRUE);
                    }

#endif
                }
            }
            break;

        case MMISMS_MSG_NEEDUPDATELIST:
            // MMISMS_SetCurType(MMISMS_BOX_SENDFAIL);
            s_need_update = TRUE;
            break;

        case MSG_GET_FOCUS:
            if (win_data_ptr->box_type != MMISMS_BOX_SENDFAIL) //从其他信箱切换回来
            {
                //清除掉原来的标记
                win_data_ptr->marked_sms_num = 0;
                for (i = 0; i < GUILIST_GetTotalItemNum(ctrl_id); i++)
                {
                    GUILIST_SetSelectedItem(ctrl_id, i, FALSE);
                }
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);
#ifndef MMI_PDA_SUPPORT
				if(s_have_items == 0)
					{
						 GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);
					}
					else
					{
                GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, FALSE);
					}
#endif
                MMISMS_SetAllMsgMarked(MMISMS_BOX_SENDFAIL, FALSE);
#ifdef MMI_PDA_SUPPORT
                GUIFORM_SetChildDisplay(MMISMS_SENDFAILBOX_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
                //重新load信息list
                win_data_ptr->box_type = MMISMS_BOX_SENDFAIL;
                MMISMS_SetCurType(MMISMS_BOX_SENDFAIL);

                if (MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID))
                {
#ifdef MMI_SMS_CHAT_SUPPORT
                    GUIMENU_SetSelectedItem(MMISMS_MENU_CTRL_ID, (uint16)MMISMS_MAIN_MENU_NODE_OUTBOX - MMISMS_MAIN_MENU_NODE_CHAT);
#else
                    GUIMENU_SetSelectedItem(MMISMS_MENU_CTRL_ID, (uint16)MMISMS_MAIN_MENU_NODE_OUTBOX - MMISMS_MAIN_MENU_NODE_WRITE_NEW_MSG);
#endif
                }
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_SENDFAIL, ctrl_id );
                if (s_have_items)
                {
                    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
                }
                else
                {
                    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
                }
                GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                s_need_update = FALSE;
#ifdef MMI_PDA_SUPPORT
                win_data_ptr->is_mark_all = FALSE;
#endif
            }
            else
            {
#ifndef MMI_PDA_SUPPORT
				if(s_have_items == 0)
				{
					 GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);
				}
				else
				{
	                if (GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
	                {
	                    GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
	                }
	                else
	                {
	                    GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, FALSE);
	                }
				}
#endif

                if (s_need_update || MMISMS_IsNeedSort())
                {
                    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                    s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_SENDFAIL, ctrl_id );
                    if (s_have_items)
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
                    }
                    else
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
                    }
                    GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                    s_need_update = FALSE;
                }
            }
            win_data_ptr->cur_list_ctrl_id = ctrl_id;
            GUILIST_ValidCurItemPos(ctrl_id);
#ifdef MMI_PDA_SUPPORT
            if ( s_have_items )
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            }
            else
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            }

#endif
            break;

        case MSG_FULL_PAINT:
            if(MMK_GetWinDirty(win_id))
            {
                if (MMK_IsFocusWin(win_id))
                {
                    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                    s_have_items = MMISMS_LoadSmsListFromOrder(MMISMS_BOX_SENDFAIL, ctrl_id);
                    if (s_have_items)
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
                    }
                    else
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
                    }
                    GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
                    MMK_SetWinDirty(win_id, FALSE);
                }
            }
            break;


#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT
            if (GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
            {
                uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
                switch (src_id)
                {
                    case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                    {
                        win_data_ptr->is_mark_all = MMISMS_HandleMarkAll(win_data_ptr->is_mark_all, win_id, ctrl_id, win_data_ptr->box_type,&win_data_ptr->marked_sms_num);
                    }
                    break;
                    case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                        if (win_data_ptr->marked_sms_num > 0 )
                        {
                            if (MMISMS_IsUpdateOk())
                            {
                                //MsgBoxOpenDelteQueryWin(win_data_ptr);
                                MMISMS_OpenDelteQueryWin(MMISMS_DELETE_PART,FALSE,win_data_ptr->cur_focus_order_id,win_data_ptr->box_type,HandleMessageBoxDelQueryWinMsg);
                            }
                        }
                        else
                        {
                            MMISMS_OpenAlertWarningWin(TXT_SMS_SELECT_ONT_PROMPT);
                        }
                        break;

                    case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                    {
                        win_data_ptr->marked_sms_num = 0;
                        win_data_ptr->is_mark_all = FALSE;
                        GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                        MMISMS_SetAllMsgMarked(MMISMS_BOX_SENDFAIL, FALSE);

                        GUIFORM_SetChildDisplay(MMISMS_SENDFAILBOX_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
                        if (MMK_IsFocusWin(win_id))
                        {
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
                        }
                    }
                    break;

                    default:
                        break;
                }
                break;
            }
#endif
            if (s_have_items)
            {
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_SENDFAIL, selected_index);
                HandleMsgBoxOpenReadMessage(win_data_ptr);
            }
            break;
 //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

#ifdef MMI_PDA_SUPPORT
        case MSG_APP_MENU:
             {
                 BOOLEAN  is_enable_mark = FALSE;
                 is_enable_mark = GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK);
                 if(!is_enable_mark)
                 {
                    win_data_ptr->is_mark_all = FALSE;
                 }
                 win_data_ptr->marked_sms_num = GUILIST_GetSelectedItemNum(ctrl_id);
                 //init softkey
                 GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE,STXT_CANCEL,TRUE);
                 HandleMsgBoxWinOpenMenuKeyOpt(win_data_ptr,is_enable_mark);
                 break;
            }

        case MSG_CTL_LIST_LONGOK:
#endif
        case MSG_CTL_OK:
        case MSG_APP_OK:
            {
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_SENDFAIL, selected_index);
                win_data_ptr->marked_sms_num = GUILIST_GetSelectedItemNum(ctrl_id);
                if(s_have_items)
                {
                    HandleMsgBoxWinOpenSendFailOpt(win_data_ptr);
                }
            }
            break;
        case MSG_SMS_FINISH_READ:
            {
                //MMISMS_HandleSmsFinishRead(win_data_ptr->cur_focus_order_id,win_data_ptr->box_type,ctrl_id,&win_data_ptr->marked_sms_num);
                HandleMsgBoxEditSMS(win_data_ptr);
            }

            break;

        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MMISMS_MSG_READ_MSG:
            if (s_have_items && (win_data_ptr->marked_sms_num == 0) )
            {
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_SENDFAIL, selected_index);
                HandleMsgBoxOpenReadMessage(win_data_ptr);
            }
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseParentWin(MMISMS_MSGBOX_MAIN_WIN_ID);
            break;

        case MSG_CLOSE_WINDOW:
            win_data_ptr->marked_sms_num = 0;
            MMISMS_SetAllMsgMarked(MMISMS_BOX_SENDFAIL, FALSE);
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleSendFailBoxDateChangeCallBack, FALSE);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleDraftBoxDateChangeCallBack(MMIEVENT_INFO_T *event_info_ptr)
{
    MMI_WIN_ID_T win_id = MMISMS_NOSENDBOX_CHILD_WIN_ID;
    if(PNULL == event_info_ptr)
    {
       SCI_TRACE_LOW("mmisms MTBox event ptr PNULL");
       return;
    }
    SCI_TRACE_LOW("mmisms HandleDraftBoxDateChangeCallBack ");
    if(SMS_DATA_CHANGE_EVENT == event_info_ptr->event_type)
    {
        MMK_SetWinDirty(win_id, TRUE);
        if(MMK_IsFocusWin(win_id))
        {
           MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
    }
}

/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDraftBoxChildWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E msg_id,        //IN:
    DPARAM            param        //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_NOSENDBOX_LISTBOX_CTRL_ID;
    uint16 selected_index = 0;
    LOCAL BOOLEAN s_have_items = FALSE;
    LOCAL BOOLEAN s_need_update = FALSE;
    GUILIST_MARK_ITEM_T* list_param_ptr = NULL;
    uint16 i = 0;
    uint16 index = 0;
    // GUI_RECT_T  list_rect = {0};
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifdef MMI_PDA_SUPPORT
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
#endif

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    MMI_HANDLE_T win_add_handle = (MMI_HANDLE_T)MMK_GetWinAddDataPtr(win_id);
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(win_add_handle);
#else
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMK_GetParentWinHandle(win_id));
#endif

	if(PNULL == win_data_ptr)
	{
		//SCI_PASSERT(0,("mmisms win_data_ptr PNULL"));
		return MMI_RESULT_FALSE;
	}

    //SCI_TRACE_LOW:"MMISMS: HandleDraftBoxChildWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_8132_112_2_18_2_54_37_750, (uint8*)"d", msg_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIAPICTRL_SetBothRect(ctrl_id, &list_both_rect);

            GUILIST_SetNeedPrgbar(ctrl_id, TRUE);
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            MsgBoxBuildWinUserData(win_data_ptr);
            // load items to list box
            s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_NOSEND, ctrl_id );
            if (s_have_items)
            {
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
            }
            else
            {
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
            }
#ifdef MMI_PDA_SUPPORT
            child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
            GUIFORM_SetChildHeight(MMISMS_DRAFTBOX_FORM_CTRL_ID, ctrl_id, &child_height);
            GUIFORM_IsSlide(MMISMS_DRAFTBOX_FORM_CTRL_ID, FALSE);
            GUIFORM_PermitChildBg(MMISMS_DRAFTBOX_FORM_CTRL_ID, FALSE);
            GUILIST_PermitBorder(ctrl_id, FALSE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIFORM_SetChildDisplay(MMISMS_DRAFTBOX_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
            GUILIST_SetDefaultIcon(MMISMS_NOSENDBOX_LISTBOX_CTRL_ID, IMAGE_SMS_CHAT_DEFAULT, IMAGE_SMS_CHAT_DEFAULT);
            win_data_ptr->is_mark_all = FALSE;
#endif
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleDraftBoxDateChangeCallBack, TRUE);
            MMK_SetAtvCtrl(win_id, ctrl_id);
#ifdef MMI_GUI_STYLE_TYPICAL
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
#endif
            s_need_update = FALSE;
            break;

        case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;

            if (PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:"MMISMS:MSG_CTL_LIST_NEED_ITEM_DATA need_item_data_ptr = PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_8174_112_2_18_2_54_37_751, (uint8*)"");
                break;
            }
            index = need_item_data_ptr->item_index;
            MMISMS_SetListItemData(MMISMS_BOX_NOSEND, ctrl_id, index);
        }
        break;

#ifndef MMI_GUI_STYLE_TYPICAL
        case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if (PNULL != need_item_content_ptr)
            {
                MMISMS_DrawListBoxItemWithIconData(need_item_content_ptr->item_index, need_item_content_ptr->item_content_index, ctrl_id, MMISMS_BOX_NOSEND);
            }
            break;
        }
#endif

        case MSG_CTL_LIST_MARK_ITEM:
            //to get the info of current selected item
            list_param_ptr = (GUILIST_MARK_ITEM_T*)param;

            win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_NOSEND,list_param_ptr->item_idx);

#ifdef MMI_PDA_SUPPORT
            MMISMS_SetSoftkeyAfterMarkOpera(win_id, ctrl_id, &win_data_ptr->is_mark_all);
#endif

            if (ctrl_id == list_param_ptr->ctrl_id)
            {
                if (list_param_ptr->had_maked)
                {
                    MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,TRUE);
                    win_data_ptr->marked_sms_num++;
                }
                else
                {
                    MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
                    win_data_ptr->marked_sms_num--;
                    // 如果标记数为零，关闭标记状态
#ifndef MMI_PDA_SUPPORT
                    if (0 == win_data_ptr->marked_sms_num)
                    {
                        GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, TRUE);
                    }
#endif
                }
            }
            break;

        case MMISMS_MSG_NEEDUPDATELIST:
            //  MMISMS_SetCurType(MMISMS_BOX_NOSEND);
            s_need_update = TRUE;
            break;

        case MSG_GET_FOCUS:
            if (win_data_ptr->box_type != MMISMS_BOX_NOSEND) //从其他信箱切换回来
            {
                //清除掉原来的标记
                win_data_ptr->marked_sms_num = 0;

                for (i = 0; i < GUILIST_GetTotalItemNum(ctrl_id); i++)
                {
                    GUILIST_SetSelectedItem(ctrl_id, i, FALSE);
                }
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);

#ifndef MMI_PDA_SUPPORT
				if(s_have_items == 0)
				{
					 GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);
				}
				else
				{
                GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, FALSE);
				}
#endif

                MMISMS_SetAllMsgMarked(MMISMS_BOX_NOSEND, FALSE);
#ifdef MMI_PDA_SUPPORT
                GUIFORM_SetChildDisplay(MMISMS_DRAFTBOX_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
                //重新load信息list
                MMISMS_SetCurType(MMISMS_BOX_NOSEND);
                win_data_ptr->box_type = MMISMS_BOX_NOSEND;
                win_data_ptr->cur_list_ctrl_id = ctrl_id;
                if (MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID))
                {
#ifdef MMI_SMS_CHAT_SUPPORT
                    GUIMENU_SetSelectedItem(MMISMS_MENU_CTRL_ID, (uint16)MMISMS_MAIN_MENU_NODE_DRAFTBOX - MMISMS_MAIN_MENU_NODE_CHAT);
#else
                    GUIMENU_SetSelectedItem(MMISMS_MENU_CTRL_ID, (uint16)MMISMS_MAIN_MENU_NODE_DRAFTBOX - MMISMS_MAIN_MENU_NODE_WRITE_NEW_MSG);
#endif
                }
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_NOSEND, ctrl_id );
                if (s_have_items)
                {
                    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
                }
                else
                {
                    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
                }
                GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                s_need_update = FALSE;
#ifdef MMI_PDA_SUPPORT
                win_data_ptr->is_mark_all = FALSE;
#endif
            }
            else
            {
#ifndef MMI_PDA_SUPPORT
				if(s_have_items == 0)
				{
					 GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);
				}
				else
				{
	                if (GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
	                {
	                    GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
	                }
	                else
	                {
	                    GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, FALSE);
	                }
				}
#endif

                if (s_need_update || MMISMS_IsNeedSort())
                {
                    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                    s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_NOSEND, ctrl_id );
                    if (s_have_items)
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
                    }
                    else
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
                    }
                    GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                    s_need_update = FALSE;
                }
            }
            win_data_ptr->cur_list_ctrl_id = ctrl_id;
            GUILIST_ValidCurItemPos(ctrl_id);
#ifdef MMI_PDA_SUPPORT
            if ( s_have_items )
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            }
            else
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            }

#endif
            break;

        case MSG_FULL_PAINT:
            if(MMK_GetWinDirty(win_id))
            {
                if (MMK_IsFocusWin(win_id))
                {
                    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                    s_have_items = MMISMS_LoadSmsListFromOrder(MMISMS_BOX_NOSEND, ctrl_id);
                    if (s_have_items)
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
                    }
                    else
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
                    }
                    GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
                    MMK_SetWinDirty(win_id,FALSE);
                }
            }
            break;


#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT
            if (GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
            {
                uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
                switch (src_id)
                {
                    case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                    {
                        win_data_ptr->is_mark_all = MMISMS_HandleMarkAll(win_data_ptr->is_mark_all, win_id, ctrl_id, win_data_ptr->box_type,&win_data_ptr->marked_sms_num);
                    }
                    break;
                    case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                        if (win_data_ptr->marked_sms_num > 0)
                        {
                            if (MMISMS_IsUpdateOk())
                            {
                                //MsgBoxOpenDelteQueryWin(win_data_ptr);
                                MMISMS_OpenDelteQueryWin(MMISMS_DELETE_PART,FALSE,win_data_ptr->cur_focus_order_id,win_data_ptr->box_type,HandleMessageBoxDelQueryWinMsg);
                            }
                        }
                        else
                        {
                            MMISMS_OpenAlertWarningWin(TXT_SMS_SELECT_ONT_PROMPT);
                        }
                        break;

                    case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                    {
                        win_data_ptr->marked_sms_num = 0;
                        win_data_ptr->is_mark_all = FALSE;
                        GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                        MMISMS_SetAllMsgMarked(MMISMS_BOX_NOSEND, FALSE);
                        GUIFORM_SetChildDisplay(MMISMS_DRAFTBOX_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
                        if (MMK_IsFocusWin(win_id))
                        {
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
                        }
                    }
                    break;

                    default:
                        break;
                }
                break;
            }
#endif
            if (s_have_items)
            {
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_NOSEND, selected_index);
                HandleMsgBoxOpenReadMessage(win_data_ptr);
            }
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527



#ifdef MMI_PDA_SUPPORT
        case MSG_APP_MENU:
             {
                 BOOLEAN  is_enable_mark = FALSE;
                 is_enable_mark = GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK);
                 if(!is_enable_mark)
                 {
                    win_data_ptr->is_mark_all = FALSE;
                 }
                 win_data_ptr->marked_sms_num = GUILIST_GetSelectedItemNum(ctrl_id);
                 //init softkey
                 GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE,STXT_CANCEL,TRUE);
                 HandleMsgBoxWinOpenMenuKeyOpt(win_data_ptr,is_enable_mark);
                 break;
            }

        case MSG_CTL_LIST_LONGOK:
#endif
        case MSG_CTL_OK:
        case MSG_APP_OK:
            {
                BOOLEAN  is_enable_mark = FALSE;
                is_enable_mark = GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK);
                if(is_enable_mark && msg_id == MSG_CTL_LIST_LONGOK)  //mark 情况下不处理longok
                {
                    break;
                }
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_NOSEND, selected_index);
                win_data_ptr->marked_sms_num = GUILIST_GetSelectedItemNum(ctrl_id);
                if(s_have_items)
                {
                   HandleMsgBoxWinOpenNoSendOpt(win_data_ptr);
                }
            }
            break;
        case MSG_SMS_FINISH_READ:
            //MMISMS_HandleSmsFinishRead(win_data_ptr->cur_focus_order_id,win_data_ptr->box_type,ctrl_id,&win_data_ptr->marked_sms_num);
            HandleMsgBoxEditSMS(win_data_ptr);
            break;

        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MMISMS_MSG_READ_MSG:
            if (GUILIST_GetItemState((GUILIST_ITEM_T*) GUILIST_GetItemPtrByIndex (ctrl_id, GUILIST_GetCurItemIndex(ctrl_id)), GUIITEM_STATE_LEFT_FOCUS))
            {
                //quick delete
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_NOSEND, selected_index);
                MMISMS_OpenDelteQueryWin(MMISMS_DELETE_SINGLE,FALSE,win_data_ptr->cur_focus_order_id,MMISMS_BOX_NOSEND,HandleMessageBoxDelQueryWinMsg);
            }
            else
            {
                if (s_have_items && (win_data_ptr->marked_sms_num == 0) )
                {
                    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                    win_data_ptr->cur_focus_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_NOSEND, selected_index);
                    HandleMsgBoxOpenReadMessage(win_data_ptr);
                }
            }
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            if (GUILIST_GetItemState((GUILIST_ITEM_T*) GUILIST_GetItemPtrByIndex (ctrl_id, GUILIST_GetCurItemIndex(ctrl_id)), GUIITEM_STATE_LEFT_FOCUS))
            {
                MMK_SendMsg(win_id, MSG_APP_RIGHT, PNULL);
            }
            else
            {
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
                MMK_CloseWin(win_id);
#else
                MMK_CloseParentWin(MMISMS_MSGBOX_MAIN_WIN_ID);
#endif
             }
        break;

        case MSG_CLOSE_WINDOW:
            win_data_ptr->marked_sms_num = 0;
            MMISMS_SetAllMsgMarked(MMISMS_BOX_NOSEND, FALSE);
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleDraftBoxDateChangeCallBack, FALSE);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : build win user data
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MsgBoxBuildWinUserData(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    win_data_ptr->marked_sms_num = 0;
}

#ifdef MMI_SMS_CHAT_SUPPORT
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenChatMenuKeyInit(MMIPUB_MENU_NOTIFY_T *param)
{
    MMI_MENU_EANBLE_GRAY_T menu_enable_t = {0};
    uint16 list_item_num = 0;
    list_item_num = GUILIST_GetTotalItemNum(MMISMS_CHATBOX_LISTBOX_CTRL_ID);
    if(0 == list_item_num)
    {
        GUIMENU_SetItemGrayed(MMISMS_CHAT_MENUKEY_OPT_MENU_CTRL_ID, MENU_SMS_CHAT_MENUKEY_OPT, MMISMS_MENU_MARK_ITEM_ID, TRUE);
    }

    MMISMS_SetSortMenuFocusItem(MMISMS_CHAT_MENUKEY_OPT_MENU_CTRL_ID,MENU_SMS_CHATBOX_SORT_OPT);

    menu_enable_t.group_id = MENU_SMS_CHAT_MENUKEY_OPT;
    menu_enable_t.win_id = MMISMS_CHAT_MENUKEY_WIN_ID;
    MMISMS_HandleSortMenuEnableGray(&menu_enable_t);
}

/*****************************************************************************/
//     Description : handle menu key opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxWinOpenChatMenuKeyOpt(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
     MMIPUB_MENU_PARAM_T menuparam = {0};
     menuparam.group_id = MENU_SMS_CHAT_MENUKEY_OPT;
     menuparam.win_id =   MMISMS_CHAT_MENUKEY_WIN_ID;
     menuparam.ctrl_id =  MMISMS_CHAT_MENUKEY_OPT_MENU_CTRL_ID;
     menuparam.dst_handle = win_data_ptr->win_id;
     menuparam.func.OnMenuOk = HandleMsgWinOpenBoxMenuOpt;
     menuparam.func.OnMenuInit = HandleMsgWinOpenChatMenuKeyInit;
     MMIPUB_OpenPubMenuWin( &menuparam );
}
#endif

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxWinOpenChatBoxOpt(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};

    menuparam.group_id = MENU_SMS_CHAT_MAIN_OPT;

    win_data_ptr->group_id = menuparam.group_id;
    menuparam.win_id =   MMISMS_CHATBOX_CHILD_OPT_WIN_ID;
    menuparam.ctrl_id =  MMISMS_CHATBOX_OPT_MENU_CTRL_ID;
    menuparam.dst_handle = win_data_ptr->win_id;
    menuparam.func.OnMenuOk = HandleMsgWinOpenBoxMenuOpt;
    menuparam.func.OnMenuInit = HandleMsgWinOpenChatBoxInit;
    MMIPUB_OpenPubMenuWin( &menuparam );
}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenChatBoxInit(MMIPUB_MENU_NOTIFY_T *param)
{
    uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    uint32 number_len = 0;
    uint16 count = 0;
    MMI_WIN_ID_T win_id = MMISMS_CHATBOX_CHILD_OPT_WIN_ID;
    uint32 common_group_id = 0;
    MMI_MENU_EANBLE_GRAY_T menu_enable_t = {0};
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);

    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    menu_enable_t.cur_order_id = win_data_ptr->cur_focus_order_id;
    menu_enable_t.box_type = win_data_ptr->box_type;
    menu_enable_t.marked_num = win_data_ptr->marked_sms_num;
	menu_enable_t.win_id = win_id;
    common_group_id = win_data_ptr->group_id;

    if(menu_enable_t.cur_order_id  != PNULL && 0 == strlen(menu_enable_t.cur_order_id ->number.sender)) // number is union struct, so just need check sender length
    {
        MMIAPICOM_EnableGrayed(win_id, MENU_SMS_CHAT_MAIN_OPT,MMISMS_MENU_EXTRACT_ADDTO_BLACKLIST_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(win_id, MENU_SMS_CHAT_MAIN_OPT,MMISMS_MENU_ADDNUMTOPB_ITEM_ID,TRUE);

        MMIAPICOM_StrToWstr(menu_enable_t.cur_order_id ->number.sender, wnumber);
        number_len = MMIAPICOM_Wstrlen(wnumber);
    }
    if (URL_IsMailtoDefault((URL_CHAR_T const *)wnumber, (int32 *)&number_len))
    {
        MMIAPICOM_EnableGrayed(win_id, MENU_SMS_CHAT_MAIN_OPT,MMISMS_MENU_ADDNUMTOPB_ITEM_ID,TRUE);
    }
    count = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_CHAT);
    if(count > 0)
    {
        menu_enable_t.group_id = MENU_SMS_CHATBOX_MARK_OPT;
        MMISMS_HandleChatMarkMenuEnableGray(&menu_enable_t);

    #ifdef MMI_PDA_SUPPORT
        menu_enable_t.group_id = common_group_id;
    #else
        #if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
        menu_enable_t.group_id = MENU_SMS_MTLIST_CALL_OPT;
        #endif
    #endif
        MMISMS_HandleDialMenuEnableGray(&menu_enable_t);

    #ifndef MMI_PDA_SUPPORT
    #ifdef MMS_SUPPORT
    #ifndef MMIMMS_SMS_IN_1_SUPPORT
        menu_enable_t.group_id = MENU_SMS_MTLIST_ANSWER_OPT;
    #endif
    #endif
    #endif
        if(menu_enable_t.group_id != MENU_SMS_MTLIST_ANSWER_OPT)
        {
            menu_enable_t.group_id = common_group_id;
        }

        MMISMS_HandleAnswerMsgMenuEnableGray(&menu_enable_t);

        menu_enable_t.group_id = common_group_id;
        MMISMS_HandleCommMenuEnableGray(&menu_enable_t);

        menu_enable_t.group_id = common_group_id;
        MMISMS_HandleDeleteMenuEnableGray(&menu_enable_t);

        menu_enable_t.group_id = common_group_id;
        MMISMS_HandleSortMenuEnableGray(&menu_enable_t);

        MMISMS_SetSortMenuFocusItem(MMISMS_CHATBOX_OPT_MENU_CTRL_ID,MENU_SMS_CHATBOX_SORT_OPT);

    }
    else
    {
        MMIAPICOM_EnableGrayed(win_id, MENU_SMS_CHAT_MAIN_OPT,MMISMS_MENU_DIAL_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(win_id, MENU_SMS_CHAT_MAIN_OPT,MMISMS_MENU_ADDNUMTOPB_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(win_id, MENU_SMS_CHAT_MAIN_OPT,MMISMS_MENU_CHAT_OPT_MARK_ITEM_ID,TRUE);
#if defined(MMI_BLACKLIST_SUPPORT)
        MMIAPICOM_EnableGrayed(win_id, MENU_SMS_CHAT_MAIN_OPT,MMISMS_MENU_EXTRACT_ADDTO_BLACKLIST_ITEM_ID,TRUE);
#endif
    }
    if(0 == GUILIST_GetTotalItemNum(MMISMS_CHATBOX_LISTBOX_CTRL_ID))
    {
        MMIAPICOM_EnableGrayed(win_id, MENU_SMS_CHAT_MAIN_OPT,MMISMS_MENU_CHAT_OPT_MARK_ITEM_ID,TRUE);
    }

    if(is_focus_new_msg_btn)
    {
#if defined(MMI_BLACKLIST_SUPPORT)
        MMIAPICOM_EnableGrayed(win_id, MENU_SMS_CHAT_MAIN_OPT,MMISMS_MENU_EXTRACT_ADDTO_BLACKLIST_ITEM_ID,TRUE);
#endif
        MMIAPICOM_EnableGrayed(win_id, MENU_SMS_CHAT_MAIN_OPT,MMISMS_MENU_DIAL_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(win_id, MENU_SMS_CHAT_MAIN_OPT,MMISMS_MENU_ADDNUMTOPB_ITEM_ID,TRUE);
    }
    else
    {
#if defined(MMI_BLACKLIST_SUPPORT)
        MMIAPICOM_EnableGrayed(win_id, MENU_SMS_CHAT_MAIN_OPT,MMISMS_MENU_EXTRACT_ADDTO_BLACKLIST_ITEM_ID,FALSE);
#endif
        MMIAPICOM_EnableGrayed(win_id, MENU_SMS_CHAT_MAIN_OPT,MMISMS_MENU_DIAL_ITEM_ID,FALSE);
    }
    if(menu_enable_t.cur_order_id  == PNULL || 0 == strlen(menu_enable_t.cur_order_id ->number.sender)) // number is union struct, so just need check sender length
    {
        MMIAPICOM_EnableGrayed(win_id, MENU_SMS_CHAT_MAIN_OPT,MMISMS_MENU_DIAL_ITEM_ID,TRUE);
#if defined(MMI_BLACKLIST_SUPPORT)
        MMIAPICOM_EnableGrayed(win_id, MENU_SMS_CHAT_MAIN_OPT,MMISMS_MENU_EXTRACT_ADDTO_BLACKLIST_ITEM_ID,TRUE);
#endif
    }
}


/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatBoxDateChangeCallBack(MMIEVENT_INFO_T *event_info_ptr)
{
    MMI_WIN_ID_T        win_id = MMISMS_CHATBOX_CHILD_WIN_ID;
    if(PNULL == event_info_ptr)
    {
       SCI_TRACE_LOW("mmisms chatbox event ptr PNULL");
       return;
    }
    SCI_TRACE_LOW("mmisms HandleChatBoxDateChangeCallBack ");
    if(SMS_DATA_CHANGE_EVENT == event_info_ptr->event_type)
    {
        MMK_SetWinDirty(win_id, TRUE);
        if(MMK_IsFocusWin(win_id))
        {
           MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
    }
}

#if 1	/* Messaging Delete multiple Converstation (Mark OptionMenu) */
/*****************************************************************************/
//    Description : define pub-menu window for Marked-Message List
//    Global resource dependence :
//    Author:
//    Note: menu item will be changed according to number of  marked-list item
/*****************************************************************************/
LOCAL void HandleMsgBoxWinOpenChatBoxOptMarkable(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
	MMIPUB_MENU_PARAM_T menuparam = {0};
	uint16 sms_totalnum = 0;
        uint16 item_marked_num =  GUILIST_GetSelectedItemNum(win_data_ptr->cur_list_ctrl_id);

	if(item_marked_num == 0 )
	{
		menuparam.group_id = MENU_SMS_CHAT_MAIN_OPT_ONE;
	}
	else
	{
		sms_totalnum = GUILIST_GetTotalItemNum(win_data_ptr->cur_list_ctrl_id);
		if(item_marked_num== sms_totalnum)
		{
			menuparam.group_id = MENU_SMS_CHAT_MAIN_OPT_THREE;
		}
		else
		{
			menuparam.group_id = MENU_SMS_CHAT_MAIN_OPT_TWO;
		}
	}
	win_data_ptr->group_id = menuparam.group_id;
	menuparam.win_id =   MMISMS_CHATBOX_CHILD_OPT_WIN_ID;
	menuparam.ctrl_id =  MMISMS_CHATBOX_OPT_MENU_CTRL_ID;
	menuparam.dst_handle = win_data_ptr->win_id;
	menuparam.func.OnMenuOk = HandleMsgWinOpenBoxMenuOpt;
	MMIPUB_OpenPubMenuWin( &menuparam );
}
#endif

/*****************************************************************************/
//  Function Name: CompareSMSNum
//  Description  : compare two sms num
//  Global resource dependence : s_mmisms_chat_linked_order
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CompareSMSNum(
                            MMIPB_BCD_NUMBER_T  *compare_number_ptr1,
                            MMIPB_BCD_NUMBER_T *compare_number_ptr2
                           )
{
    if(0 == MMIAPIPB_BCDCmp(compare_number_ptr1,compare_number_ptr2))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#ifdef HERO_ENGINE_TTS_SUPPORT
LOCAL void heroTtsPlayNewMsg()
{
	MMI_STRING_T new_message = {0};
	MMI_STRING_T wstr[1] = {0};
	int copySize = 0;
	static char tempBuf[128] = {0};

	memset(tempBuf, 0, sizeof(tempBuf));

	MMI_GetLabelTextByLang(STR_MSG_NEW_MSG_EXT01, &new_message);
	copySize = MIN(sizeof(tempBuf) - 2, ((new_message.wstr_len)<<1));
	memcpy(tempBuf, new_message.wstr_ptr, copySize);

	tempBuf[copySize] = 0;
	tempBuf[copySize+1] = 0;

	wstr[0].wstr_ptr = tempBuf;
	wstr[0].wstr_len = copySize;
	MMIAPITTS_PlayTextForControl(wstr, 1, MMISRVAUD_ROUTE_AUTO);
}
#endif
/*****************************************************************************/
//  Description : to handle the message of SMS box window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleChatBoxChildWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E msg_id,        //IN:
    DPARAM            param        //IN:
)
{
	GUI_BG_T form_bg_info = {0};
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_CHATBOX_LISTBOX_CTRL_ID;
    uint16 selected_index = 0;
    LOCAL BOOLEAN s_have_items = FALSE;
    LOCAL BOOLEAN s_need_update = FALSE;
    GUILIST_MARK_ITEM_T* list_param_ptr = NULL;
    uint16 i = 0;
    uint16 index = 0;

	uint16 hor_space = 0;
    uint16 ver_space = 0;

 #ifdef MMI_PDA_SUPPORT
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
    GUIFORM_CHILD_HEIGHT_T    child_height  = {0};
#endif

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(win_id);
#else
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMK_GetParentWinHandle(win_id));
#endif

    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T bg_rect = MMITHEME_GetClientRect();
    //GUI_COLOR_T font_color = MMI_WHITE_COLOR;
    GUI_FONT_T font_size_note = SONG_FONT_16;  //need change font size
    GUI_COLOR_T   font_color = MMIAPI_GetOpactityColorValue(60, MMI_GRAY_COLOR, MMI_BLACK_COLOR);

    MMI_STRING_T str_msg = {0};
    MMI_STRING_T empty_text = {0};
    MMI_STRING_T new_message = {0};

    MMI_HANDLE_T form_ctrl_id = NULL;
    MMI_HANDLE_T form_child_button_ctrl_id = NULL;
    MMI_HANDLE_T list_ctr_id = NULL;
    MMI_HANDLE_T button_ctr_id = NULL;
    MMI_HANDLE_T empty_text_ctrl_id = NULL;
    MMI_HANDLE_T empty_text_ctrl_note_id = NULL;
    MMISMS_ORDER_ID_T current_order_id = PNULL;
    GUIFORM_CHILD_HEIGHT_T height_info = {0};
    GUISTBDATA_TEXT_T stb_data_text = {0};

    form_ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMISMS_CHATBOX_FORM_CTRL_ID);
    form_child_button_ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_CHATBOX_CHILD_BUTTON_FORM_CTRL_ID);
    list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_CHATBOX_LISTBOX_CTRL_ID);
    button_ctr_id = MMK_GetCtrlHandleByWin(win_id,MMISMS_CHATBOX_NEWMSG_BUTTON_CTRL_ID);
    empty_text_ctrl_note_id = MMK_GetCtrlHandleByWin(win_id,MMISMS_CHATBOX_NEWMSG_STEXT_CTRL_ID);

    form_bg_info.bg_type = GUI_BG_COLOR;
    form_bg_info.color = MMI_WHITE_COLOR;

    GUIFORM_SetBg(form_ctrl_id,&form_bg_info);

    if(PNULL == win_data_ptr)
    {
    	//SCI_PASSERT(0,("mmisms win_data_ptr PNULL"));
    	return MMI_RESULT_FALSE;
    }

    //SCI_TRACE_LOW:"MMISMS: HandleChatBoxChildWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_27500_112_2_18_2_55_17_847, (uint8*)"d", msg_id);

    MMI_GetLabelTextByLang(STR_HDR_MESSAGING, &str_msg);
    MMI_GetLabelTextByLang(STR_MSG_NEW_MSG_EXT01, &new_message);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
            {
                GUIFORM_CHILD_HEIGHT_T    button_height  = {0};
                MMITHEME_BUTTON_T btn_theme;
                button_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
                button_height.add_data = CHILD_FORM_BUTTON_HIGHT;
                is_exit_from_save_draft  = FALSE;
                MMISMS_SetIsDraftDeleting(FALSE);
#if 0 //yuguang.liu
             GUIFORM_SetChildHeight(MMISMS_CHATBOX_FORM_CTRL_ID, MMISMS_CHATBOX_WRITEMSG_BUTTON_CTRL_ID, &button_height);
             GUIBUTTON_SetTextId(MMISMS_CHATBOX_WRITEMSG_BUTTON_CTRL_ID, TXT_COMMON_WRITE_MESSAGE);
             GUIBUTTON_PermitBorder(MMISMS_CHATBOX_WRITEMSG_BUTTON_CTRL_ID, FALSE);
             GUIBUTTON_SetOwnerDraw(MMISMS_CHATBOX_WRITEMSG_BUTTON_CTRL_ID, ButtonOwnerDrawFun);
#endif
                GUIFORM_SetMargin(form_ctrl_id,0);
                GUIFORM_SetMargin(form_child_button_ctrl_id,0);

                GUIFORM_SetChildHeight(form_child_button_ctrl_id, button_ctr_id, &button_height);
                GUIFORM_SetSpace(form_ctrl_id, &hor_space, &ver_space);
                GUIFORM_SetChildSpace(form_ctrl_id,form_child_button_ctrl_id,&hor_space,&ver_space);
                GUIFORM_SetChildSpace(form_ctrl_id,list_ctr_id,&hor_space,&ver_space);
                GUIFORM_SetChildSpace(form_child_button_ctrl_id,button_ctr_id,&hor_space,&ver_space);
                GUIBUTTON_PermitBorder(button_ctr_id,FALSE);
                GUIFORM_PermitChildFontColor(form_ctrl_id, TRUE);
                CTRLBUTTON_SetLocation(button_ctr_id, 0, 0);
                CTRLBUTTON_SetIsRectButton(button_ctr_id, FALSE);
                MMITHEME_GetButtonTheme(&btn_theme);
                btn_theme.font.font = CAF_FONT_20;
                CTRLBUTTON_SetFont(button_ctr_id, &btn_theme.font);
                CTRLBUTTON_SetText(button_ctr_id, new_message.wstr_ptr,new_message.wstr_len);
                CTRLBUTTON_SetButtonIcon(button_ctr_id, IMAGE_COMMON_PLUS_SMALL);
                GUIBUTTON_SetCallBackFunc(button_ctr_id,MIDButtonCallBack);
                //	CTRLBASE_SetCanActive(MMISMS_CHATBOX_NEWMSG_BUTTON_CTRL_ID, TRUE);
                MMK_SetActiveCtrl(button_ctr_id, TRUE);

				#ifdef HERO_ENGINE_TTS_SUPPORT
				{
					heroTtsPlayNewMsg();
				}
				#endif

                height_info.type=GUIFORM_CHILD_HEIGHT_LEFT;
                GUIFORM_SetChildHeight(form_ctrl_id,list_ctr_id,&height_info);
                GUILIST_PermitBorder(list_ctr_id, FALSE);
                GUILIST_SetMargin(list_ctr_id,0, 0);
                GUILIST_SetUserBg(list_ctr_id, FALSE);
#if 0
                current_order_id = MMISMS_GetChatLinkedArrayPtrIdInList(index);
                if (NULL == current_order_id)
                {
                    GUIFORM_SetChildDisplay(form_ctrl_id, list_ctr_id, GUIFORM_CHILD_DISP_HIDE);
                    GUIFORM_SetChildDisplay(form_ctrl_id, empty_text_ctrl_id, GUIFORM_CHILD_DISP_NORMAL);
                    GUIFORM_SetChildDisplay(form_ctrl_id, empty_text_ctrl_note_id, GUIFORM_CHILD_DISP_NORMAL);

                }
                else
                {
                    GUIFORM_SetChildDisplay(form_ctrl_id, empty_text_ctrl_id, GUIFORM_CHILD_DISP_HIDE);
                    GUIFORM_SetChildDisplay(form_ctrl_id, empty_text_ctrl_note_id, GUIFORM_CHILD_DISP_HIDE);
                    GUIFORM_SetChildDisplay(form_ctrl_id, list_ctr_id, GUIFORM_CHILD_DISP_NORMAL);
                }
#endif
            }
#endif
#ifdef MMI_PDA_SUPPORT
            GUIAPICTRL_SetBothRect(ctrl_id, &list_both_rect);
#endif

            MsgBoxBuildWinUserData(win_data_ptr);
            GUILIST_SetNeedPrgbar(ctrl_id, TRUE);
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            // load items to list box
            s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_CHAT, ctrl_id );
            if (s_have_items)
            {
                GUIFORM_SetChildDisplay(form_ctrl_id, empty_text_ctrl_note_id, GUIFORM_CHILD_DISP_HIDE);
                GUIFORM_SetChildDisplay(form_ctrl_id, list_ctr_id, GUIFORM_CHILD_DISP_NORMAL);
            }
            else
            {
                GUIFORM_SetChildDisplay(form_ctrl_id, list_ctr_id, GUIFORM_CHILD_DISP_HIDE);
                GUIFORM_SetChildDisplay(form_ctrl_id, empty_text_ctrl_note_id, GUIFORM_CHILD_DISP_NORMAL);
            }
            GUILIST_SetDefaultIcon(MMISMS_CHATBOX_LISTBOX_CTRL_ID, IMAGE_SMS_CHAT_DEFAULT, IMAGE_SMS_CHAT_DEFAULT);

#ifdef MMI_PDA_SUPPORT
            child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
            GUIFORM_SetChildHeight(MMISMS_CHATBOX_FORM_CTRL_ID, ctrl_id, &child_height);
            GUIFORM_IsSlide(MMISMS_CHATBOX_FORM_CTRL_ID, FALSE);
            GUIFORM_PermitChildBg(MMISMS_CHATBOX_FORM_CTRL_ID, FALSE);
            GUILIST_PermitBorder(ctrl_id, FALSE);
            win_data_ptr->is_mark_all = FALSE;
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
            MMISMS_SetCurType(MMISMS_BOX_CHAT);
            win_data_ptr->box_type = MMISMS_BOX_CHAT;
            GUIFORM_SetChildDisplay(MMISMS_CHATBOX_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleChatBoxDateChangeCallBack, TRUE);
            MMK_SetAtvCtrl(win_id, button_ctr_id);
#ifdef MMI_GUI_STYLE_TYPICAL
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
#endif
            s_need_update = FALSE;
            break;

        case MSG_NOTIFY_CONTENT_OVERFLOW:
            //string bitmap has overlaid this button, redraw it
            CTRLBUTTON_Update(button_ctr_id);
            break;

        case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;

            if (PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:"MMISMS:MSG_CTL_LIST_NEED_ITEM_DATA need_item_data_ptr = PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_27560_112_2_18_2_55_17_848, (uint8*)"");
                break;
            }
            index = need_item_data_ptr->item_index;
            MMISMS_SetListItemData(MMISMS_BOX_CHAT, ctrl_id, index);
        }
        break;

        case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if (PNULL != need_item_content_ptr)
            {
                DrawListItemWithIconData(need_item_content_ptr->item_index, need_item_content_ptr->item_content_index, ctrl_id);
            }
            break;
        }

        case MSG_CTL_LIST_MARK_ITEM:
            //to get the info of current selected item
            list_param_ptr = (GUILIST_MARK_ITEM_T*)param;

            MMISMS_SetCurOperationOrderId(MMISMS_BOX_CHAT, list_param_ptr->item_idx);
#ifdef MMI_PDA_SUPPORT
            MMISMS_SetSoftkeyAfterMarkOpera(win_id, ctrl_id, &win_data_ptr->is_mark_all);
#endif
            if (ctrl_id == list_param_ptr->ctrl_id)
            {
                if (list_param_ptr->had_maked)
                {
                    MMISMS_SetChatMsgMarkedInList(list_param_ptr->item_idx, TRUE);
                    win_data_ptr->marked_sms_num++;
                }
                else
                {
                    MMISMS_SetChatMsgMarkedInList(list_param_ptr->item_idx, FALSE);
                    win_data_ptr->marked_sms_num--;
                    // 如果标记数为零，关闭标记状态
#ifndef MMI_PDA_SUPPORT
//BUG 493957 Tong Yongli
#if 0
                    if (0 == win_data_ptr->marked_sms_num)
                    {
                        GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                        GUIFORM_SetChildDisplayOnly(MMISMS_CHATBOX_FORM_CTRL_ID,MMISMS_CHATBOX_CHILD_BUTTON_FORM_CTRL_ID,GUIFORM_CHILD_DISP_NORMAL);
                        //IWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, TRUE);
                    }
#endif
#endif
                }
                if(MMK_IsFocusWin(win_id))
                {
                   MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                }
            }
            break;

        case MMISMS_MSG_NEEDUPDATELIST:
            //  MMISMS_SetCurType(MMISMS_BOX_NOSEND);
            s_need_update = TRUE;
            break;

        case MSG_GET_FOCUS:
            current_order_id = MMISMS_GetChatLinkedArrayPtrIdInList(index);
            if (NULL == current_order_id)
            {
                GUIFORM_SetChildDisplay(form_ctrl_id, list_ctr_id, GUIFORM_CHILD_DISP_HIDE);
                //GUIFORM_SetChildDisplay(form_ctrl_id, empty_text_ctrl_id, GUIFORM_CHILD_DISP_NORMAL);
                GUIFORM_SetChildDisplay(form_ctrl_id, empty_text_ctrl_note_id, GUIFORM_CHILD_DISP_NORMAL);
            }
            else
            {
                //GUIFORM_SetChildDisplay(form_ctrl_id, empty_text_ctrl_id, GUIFORM_CHILD_DISP_HIDE);
                GUIFORM_SetChildDisplay(form_ctrl_id, empty_text_ctrl_note_id, GUIFORM_CHILD_DISP_HIDE);
                GUIFORM_SetChildDisplay(form_ctrl_id, list_ctr_id, GUIFORM_CHILD_DISP_NORMAL);
            }

            if (win_data_ptr->box_type != MMISMS_BOX_CHAT) //从其他信箱切换回来
            {
                uint16 item_totalnum = GUILIST_GetTotalItemNum(ctrl_id);

                //清除掉原来的标记
                win_data_ptr->marked_sms_num = 0;
                for (i = 0; i < GUILIST_GetTotalItemNum(ctrl_id); i++)
                {
                    GUILIST_SetSelectedItem(ctrl_id, i, FALSE);
                }
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                GUIFORM_SetChildDisplay(MMISMS_CHATBOX_FORM_CTRL_ID,MMISMS_CHATBOX_CHILD_BUTTON_FORM_CTRL_ID,GUIFORM_CHILD_DISP_NORMAL);
                GUILIST_SetListState(ctrl_id, GUILIST_STATE_CYC_SCROLL_ITEM, FALSE);
#ifndef MMI_PDA_SUPPORT
                //IWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, FALSE);
#endif

                MMISMS_SetAllMsgMarked(MMISMS_BOX_CHAT, FALSE);
#ifdef MMI_PDA_SUPPORT
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
                MMISMS_SetBoxSoftkeyDisplay(MMISMS_BOX_CHAT, GUIFORM_CHILD_DISP_HIDE);
                GUIFORM_SetChildDisplay(MMISMS_CHATBOX_FORM_CTRL_ID, MMISMS_CHATBOX_WRITEMSG_BUTTON_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
#else
                GUISOFTKEY_SetTextId(MMISMS_CHATBOX_CHILD_WIN_ID, MMICOMMON_SOFTKEY_CTRL_ID, TXT_NULL, TXT_COMMON_WRITE_MESSAGE, TXT_NULL, FALSE);
#endif
#endif

                //重新load信息list
                MMISMS_SetCurType(MMISMS_BOX_CHAT);
                win_data_ptr->box_type = MMISMS_BOX_CHAT;

                if (MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID))
                {
                    GUIMENU_SetSelectedItem(MMISMS_MENU_CTRL_ID, 0);
                    // GUIMENU_SetFirstAndSelectedItem(0, (uint16)MMISMS_MAIN_MENU_NODE_DRAFTBOX - MMISMS_MAIN_MENU_NODE_WRITE_NEW_MSG,MMISMS_MENU_CTRL_ID);
                }
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_CHAT, ctrl_id );
                if (s_have_items)
                {
                    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
                }
                else
                {
                    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
                }
                GUILIST_SetCurItemIndex(ctrl_id, selected_index);

                if(item_totalnum > 0)
                {
                      if(!GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
                      {
                          GUILIST_SetHandleLeftRightEx(ctrl_id, TRUE, FALSE);
                      }
                      else
                      {
                          GUILIST_SetHandleLeftRightEx(ctrl_id, FALSE, FALSE);
                      }
                }
                else
                {
                      GUILIST_SetHandleLeftRightEx(ctrl_id, FALSE, FALSE);
                }

                s_need_update = FALSE;
#ifdef MMI_PDA_SUPPORT
                win_data_ptr->is_mark_all = FALSE;
#endif
            }
            else
            {
                MMI_STRING_T        item_str = {0};
                uint16 item_totalnum = GUILIST_GetTotalItemNum(ctrl_id);
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
                MMISMS_SetCurType(MMISMS_BOX_CHAT);
#endif

#ifndef MMI_PDA_SUPPORT
                if(item_totalnum>0)
                {
                    if (GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
                    {
                        //IWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
                        MMI_GetLabelTextByLang(STXT_HEADER_MEMCLEAR_ALL_OR_ONE, &item_str);
                    }
                    else
                    {
                        //IWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, FALSE);
                        MMI_GetLabelTextByLang(STR_HDR_MESSAGING, &item_str);
                    }
                }
                else
                {
                    MMI_GetLabelTextByLang(STR_HDR_MESSAGING, &item_str);
                }
#endif

                if (s_need_update || MMISMS_IsNeedSort())
                {
                    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                    s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_CHAT, ctrl_id );
                    if (s_have_items)
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
                    }
                    else
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
                    }
                    GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                    s_need_update = FALSE;
                }
                if(item_totalnum > 0)
                {
                      if(!GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
                      {
                          GUILIST_SetHandleLeftRightEx(ctrl_id, TRUE, FALSE);
                      }
                      else
                      {
                          GUILIST_SetHandleLeftRightEx(ctrl_id, FALSE, FALSE);
                      }
                }
                else
                {
                      GUILIST_SetHandleLeftRightEx(ctrl_id, FALSE, FALSE);
                }
            }
            win_data_ptr->cur_list_ctrl_id = ctrl_id;
            GUILIST_ValidCurItemPos(ctrl_id);

#ifdef MMI_PDA_SUPPORT
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
            if (!GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK)) //标记状态下option不显示，都在softkey上
#else
            if ( s_have_items )
#endif
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            }
            else
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            }

#endif

#ifdef HERO_ENGINE_TTS_SUPPORT
		if(MMK_IsActiveCtrl(button_ctr_id))
		{
			heroTtsPlayNewMsg();
		}
#endif
            break;

        case MSG_FULL_PAINT:
            {
                if(MMK_GetWinDirty(win_id))
                {
                    if (MMK_IsFocusWin(win_id))
                    {
                        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                        s_have_items = MMISMS_LoadSmsListFromOrder(MMISMS_BOX_CHAT, ctrl_id);
                        if (s_have_items)
                        {
                            //GUIFORM_SetChildDisplay(form_ctrl_id, empty_text_ctrl_id, GUIFORM_CHILD_DISP_HIDE);
                            GUIFORM_SetChildDisplay(form_ctrl_id, empty_text_ctrl_note_id, GUIFORM_CHILD_DISP_HIDE);
                            GUIFORM_SetChildDisplay(form_ctrl_id, list_ctr_id, GUIFORM_CHILD_DISP_NORMAL);
                            if(!GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
                            {
                                GUILIST_SetHandleLeftRightEx(ctrl_id, TRUE, FALSE);
                            }
                            else
                            {
                                GUILIST_SetHandleLeftRightEx(ctrl_id, FALSE, FALSE);
                            }
                            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
                        }
                        else
                        {
                            GUIFORM_SetChildDisplay(form_ctrl_id, list_ctr_id, GUIFORM_CHILD_DISP_HIDE);
                            //GUIFORM_SetChildDisplay(form_ctrl_id, empty_text_ctrl_id, GUIFORM_CHILD_DISP_NORMAL);
                            GUIFORM_SetChildDisplay(form_ctrl_id, empty_text_ctrl_note_id, GUIFORM_CHILD_DISP_NORMAL);
                            GUILIST_SetHandleLeftRightEx(ctrl_id, FALSE, FALSE);
                            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
                        }

                        if(is_exit_from_save_draft)
                        {
                            GUILIST_SetCurItemIndex(ctrl_id, 0);
                            MMK_SetActiveCtrl(ctrl_id, TRUE);
                            is_exit_from_save_draft = FALSE;
                        }
                        else
                        {
                            GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                        }

                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
                        MMK_SetWinDirty(win_id,FALSE);
                    }
                }

                LCD_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR);
#if 0
                if(!GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
                {
                    MMI_GetLabelTextByLang(STR_HDR_MESSAGING, &del_msg);
                    MMIAPICOM_StatusAreaSetViewHeaderinfo(del_msg.wstr_ptr,  del_msg.wstr_len);
                    //GUILIST_SetShowLRTriangleIcon(ctrl_id, TRUE, FALSE);
                }
                else
                {
                    uint16 item_num = 0;
                    item_num = GUILIST_GetSelectedItemNum(ctrl_id);
                    if(item_num == 0)
                    {
                        MMI_GetLabelTextByLang(STXT_HEADER_MEMCLEAR_ALL_OR_ONE, &del_msg);
                        MMIAPICOM_StatusAreaSetViewHeaderinfo(del_msg.wstr_ptr,  del_msg.wstr_len);
                    }
                    else
                    {
                        wchar result_wchar[20] = {0};
                        del_msg.wstr_ptr = result_wchar;
                        MMIAPICOM_CustomGenerateNumReplaceStrByTextId(STR_MSG_HDR_DELETE_MULTI_EXT01,
                           L"%N",
                           item_num,
                           20,
                           &del_msg);
                        MMIAPICOM_StatusAreaSetViewHeaderinfo(del_msg.wstr_ptr,  del_msg.wstr_len);
                    }
                    //GUILIST_SetShowLRTriangleIcon(ctrl_id, FALSE, FALSE);
                }
#endif
            }
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef MMI_PDA_SUPPORT

        case MSG_CTL_PENOK:
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            switch (src_id)
            {
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
                case MMISMS_CHATBOX_WRITEMSG_BUTTON_CTRL_ID:
                    MMISMS_ClearDestNum();
                    MMISMS_WriteNewMessage(MN_DUAL_SYS_MAX, PNULL, FALSE, PNULL, 0);
                    break;
#endif

                case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                {
                    win_data_ptr->is_mark_all = MMISMS_HandleMarkAll(win_data_ptr->is_mark_all, win_id, ctrl_id, win_data_ptr->box_type,&win_data_ptr->marked_sms_num);
                }
                break;

                case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                    if (GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
                    {
                        if (win_data_ptr->marked_sms_num > 0)
                        {
                            if (MMISMS_IsUpdateOk())
                            {
                                //MsgBoxOpenDelteQueryWin(win_data_ptr);
                                MMISMS_OpenDelteQueryWin(MMISMS_DELETE_PART,FALSE,win_data_ptr->cur_focus_order_id,win_data_ptr->box_type,HandleMessageBoxDelQueryWinMsg);
                            }
                        }
                        else
                        {
                            MMISMS_OpenAlertWarningWin(TXT_SMS_SELECT_ONT_PROMPT);
                        }
                    }
                    else
                    {
                        MMISMS_WriteNewMessage(MN_DUAL_SYS_MAX, PNULL, FALSE, PNULL, 0);
                    }
                    break;

                case  MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                    win_data_ptr->marked_sms_num = 0;
                    win_data_ptr->is_mark_all = FALSE;
                    GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                    MMISMS_SetAllMsgMarked(MMISMS_BOX_CHAT, FALSE);
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
                    MMISMS_SetBoxSoftkeyDisplay(MMISMS_BOX_CHAT, GUIFORM_CHILD_DISP_HIDE);
                    GUIFORM_SetChildDisplay(MMISMS_CHATBOX_FORM_CTRL_ID, MMISMS_CHATBOX_WRITEMSG_BUTTON_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
                    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
                    GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID,
                                         TXT_NULL, TXT_COMMON_WRITE_MESSAGE, TXT_NULL,
                                         FALSE);
#endif

                    if (MMK_IsFocusWin(win_id))
                    {
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
                    }
                    break;

                default:
                    if (s_have_items)
                    {
                        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                        win_data_ptr->cur_focus_order_id = MMISMS_GetNumNodeByIndexInList(selected_index);;
                        //MMISMS_OpenMsgInSmsList(win_data_ptr->cur_focus_order_id,win_data_ptr->box_type);
                        MMISMS_OpenChatReadWin(win_data_ptr->cur_focus_order_id);
                    }
                    break;
            }

            break;
        }
 //TOUCH_PANEL_SUPPORT //IGNORE9527
#else
        case MSG_CTL_PENOK:
            if (s_have_items)
            {
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                win_data_ptr->cur_focus_order_id = MMISMS_GetNumNodeByIndexInList(selected_index);
                //MMISMS_OpenMsgInSmsList(win_data_ptr->cur_focus_order_id,win_data_ptr->box_type);
                MMISMS_OpenChatReadWin(win_data_ptr->cur_focus_order_id);

            }
            break;
#endif
#endif

#ifdef MMI_PDA_SUPPORT
        case MSG_APP_MENU:
             {
                 BOOLEAN  is_enable_mark = FALSE;
                 is_enable_mark = GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK);
                 if(!is_enable_mark)
                 {
                    win_data_ptr->is_mark_all = FALSE;
                    HandleMsgBoxWinOpenChatMenuKeyOpt(win_data_ptr);
                 }
                 break;
            }

        case MSG_CTL_LIST_LONGOK:
#endif
        case MSG_CTL_OK:
        case MSG_APP_OK:
            {
                //if(!MMK_IsActiveCtrl(button_ctr_id))  //revert this change
                {
                    if (MMK_IsActiveCtrl(button_ctr_id))
                    {
                        is_focus_new_msg_btn = TRUE;
                    }
                    else
                   {
                          is_focus_new_msg_btn = FALSE;
                    }
                    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                    win_data_ptr->cur_focus_order_id = MMISMS_GetNumNodeByIndexInList(selected_index);
                    if (GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
                    {
                        HandleMsgBoxWinOpenChatBoxOptMarkable(win_data_ptr);
                    }
                    else
                    {
                        if (GUILIST_GetItemState((GUILIST_ITEM_T*) GUILIST_GetItemPtrByIndex (ctrl_id, GUILIST_GetCurItemIndex(ctrl_id)), GUIITEM_STATE_LEFT_FOCUS))
                        {
                            //GUILIST_SetCurItemLeftFocus(ctrl_id, FALSE);
                            break;
                        }
                        HandleMsgBoxWinOpenChatBoxOpt(win_data_ptr);
                    }
                }
            }
            break;


        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MMISMS_MSG_READ_MSG:
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            if (GUILIST_GetItemState((GUILIST_ITEM_T*) GUILIST_GetItemPtrByIndex (ctrl_id, GUILIST_GetCurItemIndex(ctrl_id)), GUIITEM_STATE_LEFT_FOCUS))
            {
                win_data_ptr->is_del_chatlist = TRUE;
                win_data_ptr->cur_focus_order_id = MMISMS_GetNumNodeByIndexInList(selected_index);
                HandleMsgBoxDelAll(win_data_ptr);
            }
            else if (s_have_items && (win_data_ptr->marked_sms_num == 0 || !GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK)) )
            {
                BOOLEAN is_need_edit_draft = FALSE;

                win_data_ptr->cur_focus_order_id = MMISMS_GetNumNodeByIndexInListEx(selected_index, &is_need_edit_draft);
                // deleting draft can not be open
                if(MMISMS_GetIsDraftDeleting())
                {
					MMISMS_OpenMsgBox(TXT_SMS_BUSY, IMAGE_PUBWIN_WARNING);
					break;
#if 0
                    MMISMS_ORDER_ID_T del_order_id = PNULL;
                    MMISMS_ORDER_ID_T focus_order_id = win_data_ptr->cur_focus_order_id;
                    MMIPB_BCD_NUMBER_T pb_bcd1 = {0};
                    MMIPB_BCD_NUMBER_T pb_bcd2 = {0};
                    MN_NUMBER_TYPE_E number_type = MN_NUM_TYPE_UNKNOW;

                    del_order_id = MMISMS_GetCurOperationOrderId();
                    if(del_order_id != PNULL && focus_order_id != PNULL)
                    {
                        if (MMISMS_TYPE_SMS == focus_order_id->flag.msg_type)
                        {
                            pb_bcd1 = MMISMS_SmsBCDToPbBCD(focus_order_id->number.bcd_number);
                            pb_bcd2 = MMISMS_SmsBCDToPbBCD(del_order_id->number.bcd_number);
                            if(CompareSMSNum(&pb_bcd1,&pb_bcd2))
                            {
                                break;
                            }
                        }
                        else
                        {
                            if(strlen((char *)focus_order_id->number.sender) > 1 && (0x2B == focus_order_id->number.sender[0]))
                            {
                                number_type = MN_NUM_TYPE_INTERNATIONAL;
                                MMIAPICOM_StrToBcd(PACKED_LSB_FIRST,
                                    (char*)&focus_order_id->number.sender[1],
                                    pb_bcd1.number);
                                pb_bcd1.number_len =  (strlen((char *)&focus_order_id->number.sender[1]) + 1)/2;
                            }
                            else
                            {
                                MMIAPICOM_StrToBcd(PACKED_LSB_FIRST,
                                    (char*)focus_order_id->number.sender,
                                    pb_bcd1.number);
                                pb_bcd1.number_len =  (strlen((char *)focus_order_id->number.sender) + 1)/2;
                            }
                            pb_bcd1.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(number_type, MN_NUM_PLAN_UNKNOW);
                            pb_bcd2 = MMISMS_SmsBCDToPbBCD(del_order_id->number.bcd_number);

                            if(CompareSMSNum(&pb_bcd1,&pb_bcd2))
                            {
                                break;
                            }
                        }
                    }
#endif
                }

                //only one draft OR draft is mms type
                if(is_need_edit_draft)
                {
                    MMISMS_SetDraftOperationOrderId(win_data_ptr->cur_focus_order_id);
                    HandleMsgBoxOpenReadMessage(win_data_ptr);
                }
                else
                {
                    MMISMS_OpenChatReadWin(win_data_ptr->cur_focus_order_id);
                }
            }
            break;
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
        case MSG_SMS_FINISH_READ:
            HandleMsgBoxEditSMS(win_data_ptr);
            /*
            if(MMK_IsOpenWin(MMISMS_NOSENDBOX_CHILD_WIN_ID))
            {
                MMK_SendMsg(
                    MMISMS_NOSENDBOX_CHILD_WIN_ID,
                    MSG_SMS_FINISH_READ,
                    PNULL);
            }
            */
            break;
#endif
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        if (GUILIST_GetItemState((GUILIST_ITEM_T*) GUILIST_GetItemPtrByIndex (ctrl_id, GUILIST_GetCurItemIndex(ctrl_id)), GUIITEM_STATE_LEFT_FOCUS))
        {
            //GUILIST_SetItemState((GUILIST_ITEM_T*) GUILIST_GetItemPtrByIndex (ctrl_id, GUILIST_GetCurItemIndex(ctrl_id)),GUIITEM_THEME_STATE_FOCUS,TRUE);
            //GUILIST_SetItemState((GUILIST_ITEM_T*) GUILIST_GetItemPtrByIndex (ctrl_id, GUILIST_GetCurItemIndex(ctrl_id)),GUIITEM_STATE_LEFT_FOCUS,FALSE);
            MMK_SendMsg(win_id, MSG_APP_RIGHT, PNULL);
        }
        else
        {
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
		if (GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
		{
                    uint16 item_marked_num = GUILIST_GetSelectedItemNum(ctrl_id);
                    if(item_marked_num>0)
                    {
                        win_data_ptr->is_del_chatlist = TRUE;
                        HandleMsgBoxDelAll(win_data_ptr);
                    }
                    else
                    {
                        uint16 item_totalnum = GUILIST_GetTotalItemNum(ctrl_id);

                        GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                        if(item_totalnum)
                        {
                            GUILIST_SetHandleLeftRightEx(ctrl_id, TRUE, FALSE);
                        }
                        else
                        {
                            GUILIST_SetHandleLeftRightEx(ctrl_id, FALSE, FALSE);
                        }
                        GUIFORM_SetChildDisplayOnly(MMISMS_CHATBOX_FORM_CTRL_ID,MMISMS_CHATBOX_CHILD_BUTTON_FORM_CTRL_ID,GUIFORM_CHILD_DISP_NORMAL);
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
                        win_data_ptr->is_del_chatlist = FALSE;
                        GUILIST_SetListState(ctrl_id, GUILIST_STATE_CYC_SCROLL_ITEM, FALSE);
/*
                        GUILIST_SetTextListMarkable(win_data_ptr->cur_list_ctrl_id, FALSE);
                        GUILIST_SetListState(win_data_ptr->cur_list_ctrl_id, GUILIST_STATE_CYC_SCROLL_ITEM, FALSE);
                        GUIFORM_SetChildDisplayOnly(MMISMS_CHATBOX_FORM_CTRL_ID,MMISMS_CHATBOX_CHILD_BUTTON_FORM_CTRL_ID,GUIFORM_CHILD_DISP_NORMAL);
                        MMISMS_SetAllMsgMarked(MMISMS_BOX_CHAT, FALSE);
                        win_data_ptr->marked_sms_num = 0;// added by Tong Yongli
                        MMISMS_CancelMarkAllItem(win_data_ptr->cur_list_ctrl_id,win_data_ptr->box_type,&win_data_ptr->marked_sms_num,FALSE);
*/
                    }
		}
		else
		{
			MMK_CloseWin(win_id);
		}
#else
            MMK_CloseParentWin(MMISMS_MSGBOX_MAIN_WIN_ID);
#endif
        }
            break;

        case MSG_CLOSE_WINDOW:
            win_data_ptr->marked_sms_num = 0;
            MMISMS_SetAllMsgMarked(MMISMS_BOX_CHAT, FALSE);
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleChatBoxDateChangeCallBack, FALSE);
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
            SCI_FREE(win_data_ptr);
#endif
            break;

    	case MSG_APP_DOWN:
        case MSG_KEYREPEAT_DOWN:

			if ((GUILIST_GetCurItemIndex(list_ctr_id) == GUILIST_GetTotalItemNum(list_ctr_id) - 1) && !GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
			{
				MMK_SetActiveCtrl(button_ctr_id, TRUE);

				GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, STXT_SOFTKEY_OK_MK,TXT_NULL , TRUE);
				GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_SOFTKEY_MENU_MK,0,TRUE);
				GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_COMMON_COMMON_BACK,2,TRUE);
				#ifdef HERO_ENGINE_TTS_SUPPORT
				{
					heroTtsPlayNewMsg();
				}
				#endif
			}
			GUILIST_SetCurItemIndex(list_ctr_id,0);

			recode = MMI_RESULT_TRUE;
			break;

		case MSG_APP_UP:
		case MSG_KEYREPEAT_UP:
            current_order_id = MMISMS_GetChatLinkedArrayPtrIdInList(index);

			if (NULL != current_order_id )
			{
		    	if (0 == GUILIST_GetCurItemIndex(list_ctr_id))
				{
					GUILIST_SetCurItemIndex(list_ctr_id,GUILIST_GetTotalItemNum(list_ctr_id) - 1);
				}
		     	MMK_SetActiveCtrl(list_ctr_id, TRUE);
				recode = MMI_RESULT_TRUE;
			}
	    	break;

		case MSG_NOTIFY_GET_ACTIVE :
		case MSG_END_FULL_PAINT:
			if (MMK_IsActiveCtrl(button_ctr_id))
			{
				GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, STXT_SOFTKEY_OK_MK,TXT_NULL , TRUE);
				GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_SOFTKEY_MENU_MK,0,TRUE);
				GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_COMMON_COMMON_BACK,2,TRUE);
			
				#ifdef HERO_ENGINE_TTS_SUPPORT
				{
					heroTtsPlayNewMsg();
				}
				#endif
			}

			break;

		case MSG_NOTIFY_LOSE_ACTIVE:

			break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
/*****************************************************************************/
//  Discription:
//  Global resource dependence: none
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenBlocknumberTextBoxWin(MMIPB_BCD_NUMBER_T pb_bcd)
{
	MMI_HANDLE_T                win_handle = 0;
	BOOLEAN                     res = MMI_RESULT_FALSE;
	MMIPB_BCD_NUMBER_T  *win_param_ptr=PNULL;

	if(MMK_IsOpenWin(MMISMS_BLOCKNUMBER_WIN_ID))
	{
		MMK_CloseWin(MMISMS_BLOCKNUMBER_WIN_ID);
	}
	 win_param_ptr = (MMIPB_BCD_NUMBER_T *)SCI_ALLOC_APPZ(sizeof(MMIPB_BCD_NUMBER_T));

	win_handle = MMK_CreateWin((uint32 *)MMISMS_BLOCKNUMBER_TEXT_BOX_WIN_TAB, (ADD_DATA)win_param_ptr);
	if (!win_handle || (MMI_INVALID_ID == win_handle))
	{
	    res = MMI_RESULT_FALSE;
	}
	else
	{
            win_param_ptr->npi_ton = pb_bcd.npi_ton;
            SCI_MEMCPY(win_param_ptr->number,pb_bcd.number,pb_bcd.number_len);
            win_param_ptr->number_len =pb_bcd.number_len;
            MMK_SetWinUserData(win_handle,(void *)win_param_ptr);
            res = MMI_RESULT_TRUE;
	}


    return res;
}

/*****************************************************************************/
//  Discription: Handle  block list textbox msg
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL void   HandleBlockNumberTextBoxWinMsg(
                                         MMI_WIN_ID_T win_id,
                                         MMI_MESSAGE_ID_E msg_id,
                                         DPARAM param
                                         )
{
    MMI_CTRL_ID_T     ctrl_id = MMISMS_SHOW_BLOCKNUMBER_TEXT_CTRL_ID;
    MMI_STRING_T        content_str = {0};
    MMI_STRING_T mmi_pb_name = {0};
    uint8  num_ton = 0;
    BOOLEAN is_plus_symbol = FALSE;
    MMIPB_BCD_NUMBER_T 	*block_number=(MMIPB_BCD_NUMBER_T*)MMK_GetWinUserData(win_id);
    uint8               tele_len = 0;
    uint8               temp_num[MMISMS_PBNUM_MAX_LEN + 1]= {0};
    wchar               tele_num[MMISMS_PBNUM_MAX_LEN + 1] = {0};

    MMISET_BLACKLIST_ERROR_TYPE_E add_blacklist_result = MMISET_BLACKLIST_ERROR_MAX;
    MMICOM_BANNER_INFO_T banner_info = {0};
    GUI_POINT_T start_point = {0, 0};

    num_ton = block_number->npi_ton >> 4;
    if ((0 == block_number->number_len) && (MN_NUM_TYPE_INTERNATIONAL == num_ton))
    {
        is_plus_symbol = TRUE;
    }

    if ((block_number->number_len > 0) || is_plus_symbol)
    {
        mmi_pb_name.wstr_ptr = SCI_ALLOCA((MMIPB_NAME_MAX_LEN + 1) * sizeof(wchar));

        if (PNULL != mmi_pb_name.wstr_ptr)
        {
            SCI_MEMSET(mmi_pb_name.wstr_ptr, 0x00, ((MMIPB_NAME_MAX_LEN + 1)));
        }
        else
        {
            return ;
        }

#if 0
        MMIAPIPB_GetNameByNumber(block_number,
                                 &mmi_pb_name,
                                 MMIPB_NAME_MAX_LEN,
                                 FALSE  //use default name,"no name"
                                );
#endif


    }
    switch( msg_id )
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetTitleTextId( win_id,  TXT_SET_BLACKLIST_LIST,  TRUE);
	//GUIWIN_SetSoftkeyTextId( win_id,  TXT_NULL, STXT_SOFTKEY_OK_MK,STXT_RETURN ,TRUE);
	//GUIWIN_SeSoftkeytButtonTextId( win_id,  STXT_SOFTKEY_OK_MK,  1,  TRUE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_FULL_PAINT:
        tele_len = MMIAPIPB_BCDNumberToString(block_number,   (MMISMS_PBNUM_MAX_LEN + 2),   temp_num);
        MMI_STRNTOWSTR(tele_num, MMISMS_PBNUM_MAX_LEN + 1, (uint8 *)temp_num, MMISMS_PBNUM_MAX_LEN + 1, MMISMS_PBNUM_MAX_LEN + 1);
        GUITEXT_SetString(ctrl_id, tele_num, tele_len,TRUE);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
	case MSG_CTL_MIDSK:
        MMIAPISET_SetAllBlacklistSetting(TRUE);
        add_blacklist_result = MMIAPISET_AddBlacklistFromExt(&mmi_pb_name,block_number);
        if(MMISET_BLACKLIST_ERROR_NONE == add_blacklist_result)
        {
            MMK_SendMsg(MMISMS_CHATBOX_CHILD_WIN_ID, MMISMS_MSG_NEEDUPDATELIST, PNULL);
            banner_info.banner_type = MMI_BANNER_TEXT;
            banner_info.text1_id = STR_MSG_NUMBER_BLOCKED_EXT01;
            banner_info.start_point = start_point;
            MMIAPICOM_OpenBannerWin(MMISMS_CHATBOX_CHILD_WIN_ID, banner_info);
        }
        else if(MMISET_BLACKLIST_ERROR_BEYOND_MAX_NUM == add_blacklist_result)
        {
            banner_info.banner_type = MMI_BANNER_TEXT;
            banner_info.text1_id = STR_CL_BLOCK_LIST_FULL;
            banner_info.start_point = start_point;
            MMIAPICOM_OpenBannerWin(MMISMS_CHATBOX_CHILD_WIN_ID, banner_info);
        }
        SCI_SLEEP(200);
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        //SCI_FREE(mmi_pb_name.wstr_ptr);  //create every time ,but freed only in MSG_CLOSE_WINDOW, memory leak
        SCI_FREE(block_number);  //alloc in OpenBlocknumberTextBoxWin, free here
        break;
    default:
        break;
    }
    SCI_FREE(mmi_pb_name.wstr_ptr);
}

#endif



/*****************************************************************************/
//     Description : to handle tab parent window message
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsgBoxMainWindow(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T child_win_id = 0;
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T*)MMK_GetWinUserData(win_id);
    if(PNULL == win_data_ptr)
    {
        return MMI_RESULT_TRUE;
    }
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id, MMISMS_MSGBOX_TAB_CTRL_ID);
            AddTabsAndChildWindows(win_data_ptr);
            break;

        case MSG_FULL_PAINT:
            break;

        case MMISMS_MSG_NEEDUPDATELIST:
            child_win_id = MMISMS_GetSmsFocusWinId(win_data_ptr->box_type);
            MMK_SendMsg(child_win_id, MMISMS_MSG_NEEDUPDATELIST, PNULL);
            break;

        case MMISMS_MSG_READ_MSG:
            child_win_id = MMISMS_GetSmsFocusWinId(win_data_ptr->box_type);
            MMK_SendMsg(child_win_id, MMISMS_MSG_READ_MSG, PNULL);
            break;

        case MSG_SMS_FINISH_READ:
            child_win_id = MMISMS_GetSmsFocusWinId(win_data_ptr->box_type);
            MMK_SendMsg(child_win_id, MSG_SMS_FINISH_READ, PNULL);
            break;

        case MMISMS_MSG_UPDATELIST:
            child_win_id = MMISMS_GetSmsFocusWinId(win_data_ptr->box_type);
            if (MMK_IsOpenWin(child_win_id))
            {
                MMK_SendMsg(child_win_id, MMISMS_MSG_UPDATELIST, PNULL);
            }
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseParentWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
            MsgBoxWinCloseCtrlFunc(win_data_ptr);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : add all tabs and all child windows to parent window.
//  Global resource dependence :
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void  AddTabsAndChildWindows(MMISMS_MESSAGEBOX_WINDATA_T  *win_data_ptr)
{
    MMI_STRING_T str_text = {0};
    uint8 index = 0;
    MMI_IMAGE_ID_T sms_pda_tab[][2] =
    {
#ifdef MMI_SMS_CHAT_SUPPORT
        {IMAGE_SMS_CHATBOX_TAB_ON_ICON, IMAGE_SMS_CHATBOX_TAB_OFF_ICON},
#endif
        {IMAGE_SMS_INBOX_TAB_ON_ICON, IMAGE_SMS_INBOX_TAB_OFF_ICON},
        {IMAGE_SMS_SENDFAILBOX_TAB_ON_ICON, IMAGE_SMS_SENDFAILBOX_TAB_OFF_ICON},
        {IMAGE_SMS_DRAFTBOX_TAB_ON_ICON, IMAGE_SMS_DRAFTBOX_TAB_OFF_ICON},
        {IMAGE_SMS_SENDSUCCBOX_TAB_ON_ICON, IMAGE_SMS_SENDSUCCBOX_TAB_OFF_ICON}
    };

#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
    MMI_IMAGE_ID_T istyle_sms_pda_tab[][2] =
    {
#ifdef MMI_SMS_CHAT_SUPPORT
        {ISTYLE_IMAGE_SMS_CHATBOX_TAB_ON_ICON, ISTYLE_IMAGE_SMS_CHATBOX_TAB_OFF_ICON},
#endif
        {ISTYLE_IMAGE_SMS_INBOX_TAB_ON_ICON, ISTYLE_IMAGE_SMS_INBOX_TAB_OFF_ICON},
        {ISTYLE_IMAGE_SMS_SENDFAILBOX_TAB_ON_ICON, ISTYLE_IMAGE_SMS_SENDFAILBOX_TAB_OFF_ICON},
        {ISTYLE_IMAGE_SMS_DRAFTBOX_TAB_ON_ICON, ISTYLE_IMAGE_SMS_DRAFTBOX_TAB_OFF_ICON},
        {ISTYLE_IMAGE_SMS_SENDSUCCBOX_TAB_ON_ICON, ISTYLE_IMAGE_SMS_SENDSUCCBOX_TAB_OFF_ICON}
    };
#endif

    MMI_IMAGE_ID_T (*pda_tab_ptr)[2] = {PNULL};

#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
    if (MMITHEME_IsIstyle())
    {
        pda_tab_ptr = istyle_sms_pda_tab;
    }
    else
#endif
    {
        pda_tab_ptr = sms_pda_tab;
    }

#ifdef MMI_SMS_CHAT_SUPPORT
    GUITAB_SetMaxItemNum(MMISMS_MSGBOX_TAB_CTRL_ID, 5);
#else
    GUITAB_SetMaxItemNum(MMISMS_MSGBOX_TAB_CTRL_ID, 4);
#endif
    //GUITAB_SetStyle(MMISMS_MSGBOX_TAB_CTRL_ID, GUI_TAB_ITEM_MIDDLE);

#ifdef MMI_SMS_CHAT_SUPPORT
    MMI_GetLabelTextByLang(TXT_SMS_CHAT , &str_text);
    GUITAB_AppendSimpleItem(
        MMISMS_MSGBOX_TAB_CTRL_ID,
        &str_text,
        pda_tab_ptr[index][0],
        pda_tab_ptr[index][1]);
    GUITAB_AddChildWindow(
        MMISMS_MSGBOX_TAB_CTRL_ID,
        MMISMS_CHATBOX_CHILD_WIN_ID,
        index);
    index++;
#endif
    MMI_GetLabelTextByLang(TXT_INBOX , &str_text);
    GUITAB_AppendSimpleItem(
        MMISMS_MSGBOX_TAB_CTRL_ID,
        &str_text,
        pda_tab_ptr[index][0],
        pda_tab_ptr[index][1]);
    GUITAB_AddChildWindow(
        MMISMS_MSGBOX_TAB_CTRL_ID,
        MMISMS_MTBOX_CHILD_WIN_ID,
        index);
    index++;

    MMI_GetLabelTextByLang(TXT_COMM_OUTBOX, &str_text);
    GUITAB_AppendSimpleItem(
        MMISMS_MSGBOX_TAB_CTRL_ID,
        &str_text,
        pda_tab_ptr[index][0],
        pda_tab_ptr[index][1]);
    GUITAB_AddChildWindow(
        MMISMS_MSGBOX_TAB_CTRL_ID,
        MMISMS_SENDFAILBOX_CHILD_WIN_ID,
        index);
    index++;

    MMI_GetLabelTextByLang(TXT_COMM_DRAFT, &str_text);
    GUITAB_AppendSimpleItem(
        MMISMS_MSGBOX_TAB_CTRL_ID,
        &str_text,
        pda_tab_ptr[index][0],
        pda_tab_ptr[index][1]);
    GUITAB_AddChildWindow(
        MMISMS_MSGBOX_TAB_CTRL_ID,
        MMISMS_NOSENDBOX_CHILD_WIN_ID,
        index);
    index++;

    MMI_GetLabelTextByLang(TXT_SMS_SENTBOX, &str_text);
    GUITAB_AppendSimpleItem(
        MMISMS_MSGBOX_TAB_CTRL_ID,
        &str_text,
        pda_tab_ptr[index][0],
        pda_tab_ptr[index][1]);
    GUITAB_AddChildWindow(
        MMISMS_MSGBOX_TAB_CTRL_ID,
        MMISMS_SENDSUCCBOX_CHILD_WIN_ID,
        index);
    index++;

    switch (win_data_ptr->box_type)
    {
#ifdef MMI_SMS_CHAT_SUPPORT
        case MMISMS_BOX_CHAT:
            GUITAB_SetCurSel(MMISMS_MSGBOX_TAB_CTRL_ID, 0);
            break;

        case MMISMS_BOX_MT:
            GUITAB_SetCurSel(MMISMS_MSGBOX_TAB_CTRL_ID, 1);
            break;

        case MMISMS_BOX_SENDFAIL:
            GUITAB_SetCurSel(MMISMS_MSGBOX_TAB_CTRL_ID, 2);
            break;

        case MMISMS_BOX_NOSEND:
            GUITAB_SetCurSel(MMISMS_MSGBOX_TAB_CTRL_ID, 3);
            break;

        case MMISMS_BOX_SENDSUCC:
            GUITAB_SetCurSel(MMISMS_MSGBOX_TAB_CTRL_ID, 4);
            break;
#else
        case MMISMS_BOX_MT:
            GUITAB_SetCurSel(MMISMS_MSGBOX_TAB_CTRL_ID, 0);
            break;

        case MMISMS_BOX_SENDFAIL:
            GUITAB_SetCurSel(MMISMS_MSGBOX_TAB_CTRL_ID, 1);
            break;

        case MMISMS_BOX_NOSEND:
            GUITAB_SetCurSel(MMISMS_MSGBOX_TAB_CTRL_ID, 2);
            break;

        case MMISMS_BOX_SENDSUCC:
            GUITAB_SetCurSel(MMISMS_MSGBOX_TAB_CTRL_ID, 3);
            break;
#endif
        default:
            GUITAB_SetCurSel(MMISMS_MSGBOX_TAB_CTRL_ID, 0);
            break;
    }
}

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : set anim in chat list
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawListItemWithIconData(
    uint16           item_index,     //in:item索引
    uint16           content_index,  //in:item内容索引,text or anim etc.
    MMI_CTRL_ID_T    ctrl_id         //in:control id
)
{
    GUIITEM_CONTENT_T       item_content = {0};      /*lint !e64*/
    MMIPB_BCD_NUMBER_T pb_num = {0};
    wchar custom_photo_name[MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN + 2] = {0};
    uint32 custom_photo_name_len = 0;
    uint32 photo_file_size = 0;
    GUIANIM_FILE_INFO_T anim_path = {0};
    BOOLEAN result = FALSE;
    MMISMS_ORDER_ID_T current_order_id = PNULL;

    current_order_id = MMISMS_GetChatLinkedArrayPtrIdInList(item_index);
    if ( PNULL == current_order_id )
    {
        return FALSE;
    }
    if (current_order_id->flag.msg_type == MMISMS_TYPE_SMS)
    {
        pb_num.number_len = current_order_id->number.bcd_number.number_len;
        pb_num.npi_ton = current_order_id->number.bcd_number.npi_ton;
        if (pb_num.number_len > 0)
        {
            SCI_MEMCPY(&pb_num.number, current_order_id->number.bcd_number.number, pb_num.number_len);
        }
    }
    else
    {
        MMISMS_MMSNumberToBcd((char*)current_order_id->number.sender, pb_num.number, &pb_num.number_len);
    }

    if (MMIAPIPB_GetCustomPhotoPathByNumber(&pb_num, custom_photo_name, &custom_photo_name_len, &photo_file_size))
    {
        item_content.item_data_type = GUIITEM_DATA_ANIM_PATH;
        item_content.item_data.anim_path_ptr = &anim_path;
        anim_path.full_path_wstr_ptr = custom_photo_name;
        anim_path.full_path_wstr_len = custom_photo_name_len;

        result = GUILIST_SetItemContent(ctrl_id, &item_content, item_index, content_index);
    }
    else
    {
        GUILIST_SetDefaultIcon(MMISMS_CHATBOX_LISTBOX_CTRL_ID, IMAGE_SMS_CHAT_DEFAULT, IMAGE_SMS_CHAT_DEFAULT);
    }

    return result;
}
#endif

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxWinOpenNoSendOpt(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};
    MMISMS_ORDER_ID_T cur_order_id = win_data_ptr->cur_focus_order_id;

    if ((win_data_ptr->marked_sms_num == 0 && cur_order_id != PNULL && cur_order_id->flag.is_locked)
    ||(win_data_ptr->marked_sms_num > 0 && PNULL == MMISMS_FindValidMoSMSForDelAll(MMISMS_BOX_NOSEND)))
    {
        menuparam.group_id = MENU_SMS_NOSEND_LIST_UNLOCK_OPT;
    }
    else
    {
        menuparam.group_id = MENU_SMS_NOSEND_LIST_OPT;
    }

    win_data_ptr->group_id = menuparam.group_id;
    menuparam.win_id =   MMISMS_MOBOX_WIN_ID;
    menuparam.ctrl_id =  MMISMS_DRAFTBOX_OPT_MENU_CTRL_ID;
    menuparam.dst_handle = win_data_ptr->win_id;
    menuparam.func.OnMenuOk = HandleMsgWinOpenBoxMenuOpt;
    menuparam.func.OnMenuInit = HandleMsgWinOpenNoSendInit;
    MMIPUB_OpenPubMenuWin( &menuparam );
}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenNoSendInit(MMIPUB_MENU_NOTIFY_T *param)
{
    uint32 common_group_id = 0;
    MMI_MENU_EANBLE_GRAY_T menu_enable_t = {0};
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);

    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    menu_enable_t.cur_order_id = win_data_ptr->cur_focus_order_id;
    menu_enable_t.box_type = win_data_ptr->box_type;
    menu_enable_t.marked_num = win_data_ptr->marked_sms_num;

    menu_enable_t.win_id = MMISMS_MOBOX_WIN_ID;

    common_group_id = win_data_ptr->group_id;

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleCommMenuEnableGray(&menu_enable_t);

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleDeleteMenuEnableGray(&menu_enable_t);

#ifdef MMI_SMS_MOVE_SUPPORT
    menu_enable_t.group_id = MENU_SMS_SECURITY_BOX_OPT;
    MMISMS_HandleMoveMenuEnableGray(&menu_enable_t);
#endif

#ifdef MMI_SMS_COPY_SUPPORT
    menu_enable_t.group_id = MENU_SMS_COPY_OPT;
    MMISMS_HandleCopyMenuEnableGray(&menu_enable_t);
#endif

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleSortMenuEnableGray(&menu_enable_t);

    menu_enable_t.group_id = MENU_SMS_MTLIST_MARK_OPT;
    MMISMS_HandleMarkMenuEnableGray(&menu_enable_t);

    if(0 == menu_enable_t.marked_num && (PNULL != win_data_ptr->cur_focus_order_id) && (win_data_ptr->cur_focus_order_id->flag.msg_type  != MMISMS_TYPE_SMS))
    {
        MMIAPICOM_EnableGrayed(menu_enable_t.win_id, common_group_id, MMISMS_MENU_COPY_ITEM_ID, TRUE);
    }

    MMISMS_SetSortMenuFocusItem(MMISMS_DRAFTBOX_OPT_MENU_CTRL_ID,MENU_SMS_MTLIST_SORT_OPT);
}
/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxWinOpenSendFailOpt(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};
    MMISMS_ORDER_ID_T cur_order_id = win_data_ptr->cur_focus_order_id;

    if ((win_data_ptr->marked_sms_num == 0 && cur_order_id != PNULL && cur_order_id->flag.is_locked)
    ||(win_data_ptr->marked_sms_num > 0 && PNULL == MMISMS_FindValidMoSMSForDelAll(MMISMS_BOX_SENDFAIL)))
    {
        menuparam.group_id = MENU_SMS_SENDFAIL_LIST_UNLOCK_OPT;
    }
    else
    {
        menuparam.group_id = MENU_SMS_SENDFAIL_LIST_OPT;
    }

    win_data_ptr->group_id = menuparam.group_id;
    menuparam.win_id =   MMISMS_MOBOX_WIN_ID;
    menuparam.ctrl_id =  MMISMS_SENDFAILBOX_OPT_MENU_CTRL_ID;
    menuparam.dst_handle = win_data_ptr->win_id;
    menuparam.func.OnMenuOk = HandleMsgWinOpenBoxMenuOpt;
    menuparam.func.OnMenuInit = HandleMsgWinOpenSendFailInit;
    MMIPUB_OpenPubMenuWin( &menuparam );
}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenSendFailInit(MMIPUB_MENU_NOTIFY_T *param)
{
    uint32 common_group_id = 0;
    MMI_MENU_EANBLE_GRAY_T menu_enable_t = {0};
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);

    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    menu_enable_t.cur_order_id = win_data_ptr->cur_focus_order_id;
    menu_enable_t.box_type = win_data_ptr->box_type;
    menu_enable_t.marked_num = win_data_ptr->marked_sms_num;

    //menu_enable_t.marked_mms_num= MMISMS_GetMarkedMsgNumByType(win_data_ptr->box_type, MMISMS_TYPE_MMS);
    //menu_enable_t.marked_sms_num = MMISMS_GetMarkedMsgNumByType(win_data_ptr->box_type, MMISMS_TYPE_SMS);

    menu_enable_t.win_id = MMISMS_MOBOX_WIN_ID;

    common_group_id = win_data_ptr->group_id;

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleCommMenuEnableGray(&menu_enable_t);

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleDeleteMenuEnableGray(&menu_enable_t);

#ifdef MMI_SMS_MOVE_SUPPORT
    menu_enable_t.group_id = MENU_SMS_SECURITY_BOX_OPT;
    MMISMS_HandleMoveMenuEnableGray(&menu_enable_t);
#endif

#ifdef MMI_SMS_COPY_SUPPORT
    menu_enable_t.group_id = MENU_SMS_COPY_OPT;
    MMISMS_HandleCopyMenuEnableGray(&menu_enable_t);
#endif

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleSortMenuEnableGray(&menu_enable_t);
    MMISMS_HandleSendMenuEnableGray(&menu_enable_t);

    menu_enable_t.group_id = MENU_SMS_MTLIST_MARK_OPT;
    MMISMS_HandleMarkMenuEnableGray(&menu_enable_t);

    if(0 == menu_enable_t.marked_num && (PNULL != win_data_ptr->cur_focus_order_id) && (win_data_ptr->cur_focus_order_id->flag.msg_type  != MMISMS_TYPE_SMS))
    {
        MMIAPICOM_EnableGrayed(menu_enable_t.win_id, common_group_id, MMISMS_MENU_COPY_ITEM_ID, TRUE);
    }

    MMISMS_SetSortMenuFocusItem(MMISMS_SENDFAILBOX_OPT_MENU_CTRL_ID,MENU_SMS_MTLIST_SORT_OPT);
}

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxWinOpenSendSuccOpt(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};
    if(PNULL == win_data_ptr->cur_focus_order_id || !win_data_ptr->cur_focus_order_id->flag.is_locked)
    {
        menuparam.group_id = MENU_SMS_SENDSUCC_LIST_OPT;
    }
    else
    {
        menuparam.group_id = MENU_SMS_SENDSUCC_LIST_UNLOCK_OPT;
    }
    win_data_ptr->group_id = menuparam.group_id;
    menuparam.win_id =   MMISMS_SENDSUCCBOX_WIN_ID;
    menuparam.ctrl_id =  MMISMS_SENDSUCCBOX_OPT_MENU_CTRL_ID;
    menuparam.dst_handle = win_data_ptr->win_id;
    menuparam.func.OnMenuOk = HandleMsgWinOpenBoxMenuOpt;
    menuparam.func.OnMenuInit = HandleMsgWinOpenSendSuccInit;
    MMIPUB_OpenPubMenuWin( &menuparam );
}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenSendSuccInit(MMIPUB_MENU_NOTIFY_T *param)
{
    uint32 common_group_id = 0;
    MMI_MENU_EANBLE_GRAY_T menu_enable_t = {0};
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);

    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    menu_enable_t.cur_order_id = win_data_ptr->cur_focus_order_id;
    menu_enable_t.box_type = win_data_ptr->box_type;
    menu_enable_t.marked_num = win_data_ptr->marked_sms_num;
    menu_enable_t.win_id = MMISMS_SENDSUCCBOX_WIN_ID;

    common_group_id = win_data_ptr->group_id;
#ifdef MMI_PDA_SUPPORT
    menu_enable_t.group_id = common_group_id;
#else
    #if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
    menu_enable_t.group_id = MENU_SMS_MTLIST_CALL_OPT;
    #endif
#endif

    MMISMS_HandleDialMenuEnableGray(&menu_enable_t);
#ifndef MMI_PDA_SUPPORT
#ifdef MMS_SUPPORT
#ifndef MMIMMS_SMS_IN_1_SUPPORT
    menu_enable_t.group_id = MENU_SMS_MTLIST_ANSWER_OPT;
#endif
#endif
#endif
    if(menu_enable_t.group_id != MENU_SMS_MTLIST_ANSWER_OPT)
    {
        menu_enable_t.group_id = common_group_id;
    }
    MMISMS_HandleAnswerMsgMenuEnableGray(&menu_enable_t);

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleCommMenuEnableGray(&menu_enable_t);

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleDeleteMenuEnableGray(&menu_enable_t);

#ifdef MMI_SMS_MOVE_SUPPORT
    menu_enable_t.group_id = MENU_SMS_SECURITY_BOX_OPT;
    MMISMS_HandleMoveMenuEnableGray(&menu_enable_t);
#endif

#ifdef MMI_SMS_COPY_SUPPORT
    menu_enable_t.group_id = MENU_SMS_COPY_OPT;
    MMISMS_HandleCopyMenuEnableGray(&menu_enable_t);
#endif

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleForwardMenuEnableGray(&menu_enable_t);
    MMISMS_HandleSortMenuEnableGray(&menu_enable_t);

    menu_enable_t.group_id = MENU_SMS_MTLIST_MARK_OPT;
    MMISMS_HandleMarkMenuEnableGray(&menu_enable_t);
    if(0 == menu_enable_t.marked_num && (PNULL != win_data_ptr->cur_focus_order_id) && (win_data_ptr->cur_focus_order_id->flag.msg_type  != MMISMS_TYPE_SMS))
    {
        MMIAPICOM_EnableGrayed(menu_enable_t.win_id, common_group_id, MMISMS_MENU_COPY_ITEM_ID, TRUE);
    }

    MMISMS_SetSortMenuFocusItem(MMISMS_SENDSUCCBOX_OPT_MENU_CTRL_ID,MENU_SMS_MTLIST_SORT_OPT);
}

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxWinOpenMTOpt(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};
    if(PNULL == win_data_ptr->cur_focus_order_id || !win_data_ptr->cur_focus_order_id->flag.is_locked)
    {
        menuparam.group_id = MENU_SMS_MTLIST_OPT;
    }
    else
    {
        menuparam.group_id = MENU_SMS_MTLIST_UNLOCK_OPT;
    }
    win_data_ptr->group_id = menuparam.group_id;

    menuparam.win_id =   MMISMS_MTBOX_WIN_ID;
    menuparam.ctrl_id =  MMISMS_MTBOX_OPT_MENU_CTRL_ID;
    menuparam.dst_handle = win_data_ptr->win_id;
    menuparam.func.OnMenuOk = HandleMsgWinOpenBoxMenuOpt;
    menuparam.func.OnMenuInit = HandleMsgWinOpenMTInit;
    MMIPUB_OpenPubMenuWin( &menuparam );
}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenMTInit(MMIPUB_MENU_NOTIFY_T *param)
{
    uint32 common_group_id = 0;
    MMI_MENU_EANBLE_GRAY_T menu_enable_t = {0};
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    menu_enable_t.cur_order_id = win_data_ptr->cur_focus_order_id;
    menu_enable_t.box_type = win_data_ptr->box_type;
    menu_enable_t.marked_num = win_data_ptr->marked_sms_num;
    menu_enable_t.win_id = MMISMS_MTBOX_WIN_ID;

    if((PNULL == win_data_ptr->cur_focus_order_id) || (win_data_ptr->cur_focus_order_id != PNULL && !win_data_ptr->cur_focus_order_id->flag.is_locked))
    {
        common_group_id = MENU_SMS_MTLIST_OPT;
    }
    else
    {
        common_group_id = MENU_SMS_MTLIST_UNLOCK_OPT;
    }

#ifdef MMI_PDA_SUPPORT
    menu_enable_t.group_id = common_group_id;
#else
    #if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
    menu_enable_t.group_id = MENU_SMS_MTLIST_CALL_OPT;
    #endif
#endif
    MMISMS_HandleDialMenuEnableGray(&menu_enable_t);
#ifndef MMI_PDA_SUPPORT
#ifdef MMS_SUPPORT
#ifndef MMIMMS_SMS_IN_1_SUPPORT
    menu_enable_t.group_id = MENU_SMS_MTLIST_ANSWER_OPT;
#endif
#endif
#endif
    if(menu_enable_t.group_id != MENU_SMS_MTLIST_ANSWER_OPT)
    {
        menu_enable_t.group_id = common_group_id;
    }
    MMISMS_HandleAnswerMsgMenuEnableGray(&menu_enable_t);

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleCommMenuEnableGray(&menu_enable_t);

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleDeleteMenuEnableGray(&menu_enable_t);

#ifdef MMI_SMS_MOVE_SUPPORT
    menu_enable_t.group_id = MENU_SMS_SECURITY_BOX_OPT;
    MMISMS_HandleMoveMenuEnableGray(&menu_enable_t);
#endif

#ifdef MMI_SMS_COPY_SUPPORT
    menu_enable_t.group_id = MENU_SMS_COPY_OPT;
    MMISMS_HandleCopyMenuEnableGray(&menu_enable_t);
#endif

    menu_enable_t.group_id = common_group_id;
    MMISMS_HandleSortMenuEnableGray(&menu_enable_t);

    MMISMS_HandleAddPBMenuEnableGray(&menu_enable_t);

    menu_enable_t.group_id = MENU_SMS_MTLIST_MARK_OPT;
    MMISMS_HandleMarkMenuEnableGray(&menu_enable_t);
    if(0 == menu_enable_t.marked_num && (PNULL != win_data_ptr->cur_focus_order_id) && (win_data_ptr->cur_focus_order_id->flag.msg_type  != MMISMS_TYPE_SMS))
    {
        MMIAPICOM_EnableGrayed(menu_enable_t.win_id, common_group_id, MMISMS_MENU_COPY_ITEM_ID, TRUE);
    }
    MMISMS_SetSortMenuFocusItem(MMISMS_MTBOX_OPT_MENU_CTRL_ID,MENU_SMS_MTLIST_SORT_OPT);
}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenBoxMenuOpt(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 group_id, menu_id = 0;

    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }

    group_id = param->group_id;
    menu_id = param->menu_id;

    win_data_ptr->menu_id = menu_id;
    win_data_ptr->group_id = group_id;
    switch(menu_id)
    {
        case MMISMS_MENU_ANSWER_SMS_ITEM_ID:
            MMISMS_AnswerSms(win_data_ptr->cur_focus_order_id,win_data_ptr->box_type,win_data_ptr->cur_list_ctrl_id,&win_data_ptr->marked_sms_num);
            break;

        case MMISMS_MENU_ANSWER_MMS_ITEM_ID:
            MMISMS_AnswerMMS(win_data_ptr->cur_focus_order_id,win_data_ptr->box_type,win_data_ptr->cur_list_ctrl_id,&win_data_ptr->marked_sms_num);
            break;

        case MMISMS_MENU_SIM1_SEND_ITEM_ID:
            HandleMsgBoxResendMsg(win_data_ptr);
            break;
        case MMISMS_MENU_EDIT_ITEM_ID:
            HandleMsgBoxEditMsg(win_data_ptr);
            break;
        case MMISMS_MENU_FORWARD_ITEM_ID:
            HandleMsgBoxForwardMsg(win_data_ptr);
            break;

        case MMISMS_MENU_DEL_ITEM_ID:
         //  HandleMsgBoxDelAll(win_data_ptr);
            win_data_ptr->is_del_all = FALSE;
			HandleMsgBoxDelAll(win_data_ptr); 
		     //HandleMsgBoxDelAllNoQueryWindow(win_data_ptr);
            break;

        case MMISMS_MENU_DELBOX_ALL_ITEM_ID:
            win_data_ptr->is_del_all = TRUE;

            HandleMsgBoxDelAll(win_data_ptr);
            break;

        case MMISMS_MENU_DIAL_ITEM_ID:
            MMISMS_ReplyCall(win_data_ptr->cur_focus_order_id,CC_CALL_NORMAL_CALL);
            break;
#ifdef VT_SUPPORT
        case MMISMS_MENU_VIDEOCALL_ITEM_ID:
            MMISMS_ReplyCall(win_data_ptr->cur_focus_order_id,CC_CALL_VIDEO_CALL);
            break;
#endif
#ifdef MMI_IP_CALL_SUPPORT
        case MMISMS_MENU_IPDIAL_ITEM_ID:
#endif
            MMISMS_ReplyCall(win_data_ptr->cur_focus_order_id,CC_CALL_IP_CALL);
            break;

        case MMISMS_MENU_LOCK_ITEM_ID:
            HandleMsgBoxLockItem(win_data_ptr);
            break;

        case MMISMS_MENU_UNLOCK_ITEM_ID:
            HandleMsgBoxUnLockItem(win_data_ptr);
            break;

#ifdef MMI_SMS_MOVE_SUPPORT
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMISMS_MENU_BOX_SECURITY_ITEM_ID:
        case MMISMS_MENU_BOX_PREVIEW_ITEM_ID:
            HandleMsgBoxMoveToMe(win_data_ptr);
            break;
#endif

        case MMISMS_MENU_BOX_MOVETOME_ITEM_ID:
            HandleMsgBoxMoveToMe(win_data_ptr);
            break;

        case MMISMS_MENU_BOX_MOVETOSIM_ITEM_ID:
            HandleMsgBoxMoveToSIM(win_data_ptr);
            break;
#endif

#ifdef MMI_SMS_COPY_SUPPORT
        case MMISMS_MENU_BOX_COPYTOME_ITEM_ID:
            HandleMsgBoxCopyToMe(win_data_ptr);
            break;

        case MMISMS_MENU_BOX_COPYTOSIM_ITEM_ID:
            HandleMsgBoxCopyToSIM(win_data_ptr);
            break;
#endif

#ifdef MMI_SMS_CHAT_SUPPORT
    case MMISMS_MENU_CHAT_OPT_MARK_ITEM_ID:
            win_data_ptr->is_del_chatlist = TRUE;
            MMISMS_MarkChatItem(win_data_ptr->cur_focus_order_id,win_data_ptr->cur_list_ctrl_id,&win_data_ptr->marked_sms_num);
            break;

    case MMISMS_MENU_CHAT_CANCEL_MARK_ITEM_ID:
            MMISMS_CancelMarkChatItem(win_data_ptr->cur_focus_order_id,win_data_ptr->cur_list_ctrl_id,&win_data_ptr->marked_sms_num);
            break;

    case MMISMS_MENU_NEW_CHAT_ITEM_ID:
    		MMISMS_ClearDestNum();
            MMISMS_WriteNewMessage(MN_DUAL_SYS_MAX, PNULL, FALSE, PNULL, 0);
            if(win_data_ptr->marked_sms_num > 0)
            {
                MMISMS_CancelMarkAllItem(win_data_ptr->cur_list_ctrl_id,win_data_ptr->box_type,&win_data_ptr->marked_sms_num,FALSE);
            }
            break;
#endif

        case MMISMS_MENU_OPT_MARK_ITEM_ID:
            MMISMS_MarkItem(win_data_ptr->cur_focus_order_id,win_data_ptr->cur_list_ctrl_id,&win_data_ptr->marked_sms_num);
            break;

        case MMISMS_MENU_CANCEL_MARK_ITEM_ID:
            MMISMS_CancelMarkItem(win_data_ptr->cur_focus_order_id,win_data_ptr->cur_list_ctrl_id,&win_data_ptr->marked_sms_num);
            break;

        case MMISMS_MENU_MARKALL_ITEM_ID:
            MMISMS_MarkAllItem(win_data_ptr->cur_list_ctrl_id,win_data_ptr->box_type,&win_data_ptr->marked_sms_num,FALSE);
            break;

        case MMISMS_MENU_CANCLE_MARKALL_ITEM_ID:
            MMISMS_CancelMarkAllItem(win_data_ptr->cur_list_ctrl_id,win_data_ptr->box_type,&win_data_ptr->marked_sms_num,FALSE);
            break;

        case MMISMS_MENU_MARK_NUM_ITEM_ID:
            MMISMS_MarkSameNumItem(win_data_ptr->cur_list_ctrl_id,win_data_ptr->box_type,&win_data_ptr->marked_sms_num);
            break;

        case MMISMS_MENU_CANCEL_MARK_NUM_ITEM_ID:
            MMISMS_CancelMarkSameNumItem(win_data_ptr->cur_list_ctrl_id,win_data_ptr->box_type,&win_data_ptr->marked_sms_num);
            break;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        case MMISMS_MENU_MARK_SIM1_SMS_ITEM_ID:
        case MMISMS_MENU_MARK_SIM2_SMS_ITEM_ID:
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMISMS_MENU_MARK_SIM3_SMS_ITEM_ID:
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMISMS_MENU_MARK_SIM4_SMS_ITEM_ID:
#endif
#else
        case MMISMS_MENU_MARK_SIM_SMS_ITEM_ID:
#endif
        {
            MN_DUAL_SYS_E mark_sim_id = MN_DUAL_SYS_1;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
            mark_sim_id = MN_DUAL_SYS_1+(menu_id-MMISMS_MENU_MARK_SIM1_SMS_ITEM_ID);
#else
            mark_sim_id = MN_DUAL_SYS_1;
#endif
            MMISMS_MarkSimItem(win_data_ptr->cur_list_ctrl_id,win_data_ptr->box_type,mark_sim_id,&win_data_ptr->marked_sms_num);
        }
        break;

#ifdef MMI_PDA_SUPPORT
        case MMISMS_MENU_MARK_ITEM_ID:
            MMISMS_MarkOpt(win_data_ptr->cur_list_ctrl_id,win_data_ptr->box_type,&win_data_ptr->marked_sms_num);
            break;
#endif

    case MMISMS_MENU_TIME_SORT_ITEM_ID:
        MMISMS_SetOrderType( MMISMS_ORDER_BY_TIME);
        break;

    case MMISMS_MENU_NAME_SORT_ITEM_ID:
        MMISMS_SetOrderType( MMISMS_ORDER_BY_NAME);
        break;

    case MMISMS_MENU_SUBJECT_SORT_ITEM_ID:
        MMISMS_SetOrderType( MMISMS_ORDER_BY_CONTENT);
        break;

    case MMISMS_MENU_TYPE_SORT_ITEM_ID:
        MMISMS_SetOrderType( MMISMS_ORDER_BY_TYPE);
        break;
#if defined(MMI_SMS_EXPORT_SUPPORT)
    case MMISMS_MENU_EXPORT_ITEM_ID:
        MMISMS_ExportItem(win_data_ptr->cur_focus_order_id,win_data_ptr->cur_list_ctrl_id,win_data_ptr->box_type,&win_data_ptr->marked_sms_num);
        break;
#endif

    case MMISMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
        {
            MN_DUAL_SYS_E  dual_sys = MN_DUAL_SYS_MAX;
            uint8 number_arr[MMISMS_MAX_USERNUMBER_LEN + 2] = {0};
            MMISMS_GetCurNumByOrder(win_data_ptr->cur_focus_order_id, &dual_sys, (uint8*)number_arr, MMISMS_PBNUM_MAX_LEN + 2);
            MMISMS_AddToPbNew(win_data_ptr->cur_focus_order_id,(uint8*)number_arr);
        }
        break;

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
    case MMISMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
        {
            MN_DUAL_SYS_E  dual_sys = MN_DUAL_SYS_MAX;
            uint8 number_arr[MMISMS_MAX_USERNUMBER_LEN + 2] = {0};
            MMISMS_GetCurNumByOrder(win_data_ptr->cur_focus_order_id, &dual_sys, (uint8*)number_arr, MMISMS_PBNUM_MAX_LEN + 2);
            MMISMS_AddToPbExist(win_data_ptr->cur_focus_order_id,(uint8*)number_arr);
        }
        break;
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
    case MMISMS_MENU_EXTRACT_ADDTO_BLACKLIST_ITEM_ID:
        {
            MMIPB_BCD_NUMBER_T    pb_bcd = {0};
            MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};
            MN_DUAL_SYS_E  dual_sys = MN_DUAL_SYS_MAX;
            uint8 number_arr[MMISMS_MAX_USERNUMBER_LEN + 2] = {0};
            MMISMS_GetCurNumByOrder(win_data_ptr->cur_focus_order_id, &dual_sys, (uint8*)number_arr, MMISMS_PBNUM_MAX_LEN + 2);
            MMIAPICOM_GenPartyNumber(number_arr,(uint16)SCI_STRLEN((char *)number_arr), &party_num);
            pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
            pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
            SCI_MEMCPY(   pb_bcd.number,  party_num.bcd_num, pb_bcd.number_len  );
            //MMISMS_AddSenderToBlackList(pb_bcd);
            //OpenBlocknumberTextBoxWin(pb_bcd);
	#ifdef MMI_SMS_CHAT_SUPPORT
			  OpenBlocknumberTextBoxWin(pb_bcd);
	#else
            MMISMS_AddSenderToBlackList(pb_bcd);
           // OpenBlocknumberTextBoxWin(pb_bcd);
	#endif            
        }
		break;
	case MMISMS_MENU_OPEN_BLOCK_MESSAGE_WIN_ITEM_ID:
        {
#ifdef MMI_SMS_CHAT_SUPPORT
            MMISmsAPI_SetBlockedMessageWindow(TRUE);
#endif
            //MMISMSAPI_OpenBlockedMessageBox(MMISMS_BOX_CHAT);
            MMISMS_EnterBlackSmsBox(MMISMS_BOX_CHAT);
        }
            break;
        case MMISMS_MENU_CHAT_DELETE_CONVERSATION_CANCEL_ID:
            GUILIST_SetTextListMarkable(win_data_ptr->cur_list_ctrl_id, FALSE);
            GUILIST_SetListState(win_data_ptr->cur_list_ctrl_id, GUILIST_STATE_CYC_SCROLL_ITEM, FALSE);

            GUIFORM_SetChildDisplayOnly(MMISMS_CHATBOX_FORM_CTRL_ID,MMISMS_CHATBOX_CHILD_BUTTON_FORM_CTRL_ID,GUIFORM_CHILD_DISP_NORMAL);
            MMISMS_SetAllMsgMarked(MMISMS_BOX_CHAT, FALSE);
            win_data_ptr->marked_sms_num = 0;// added by Tong Yongli
            MMISMS_CancelMarkAllItem(win_data_ptr->cur_list_ctrl_id,win_data_ptr->box_type,&win_data_ptr->marked_sms_num,FALSE);
            MMK_PostMsg(win_data_ptr->win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
            break;
#endif
    case MMISMS_MENU_DELBOX_SAMENUM_ITEM_ID:
        {
            HandleMsgBoxDelSameNum(win_data_ptr);
        }
        break;

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    case MMISMS_MENU_DRAFTBOX_ID_ITEM_ID:
        HandleOpenDraftBoxWin(win_data_ptr);
        break;
	case MMISMS_MENU_OPT_FREQWORD_ITEM_ID:
		//常用语fuc win
		MMISMS_EnterWordListWin();
		break;

    case MMISMS_MENU_MEMSTATE_ID_ITEM_ID:
		//mem state func win
		MMISMS_EnterMessageCountWin();
		break;

    case  MMISMS_MENU_SETTING_ID_ITEM_ID:
        MMISMS_OpenSettingTabWin(); // to do
		break;

#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
    case MMISMS_MENU_VOICEMAIL:
        MMIAPISMS_OpenVoicemailWin();
        break;
#endif

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_MENU_SECURITY_ID_ITEM_ID:
        MMIAPISMS_OpenValidateSecruityBox(PNULL);
		break;
#endif

#if defined (MMS_SUPPORT)
	case MMISMS_MENU_PUSH_MSG_ID_ITEM_ID:
		MMISMS_EnterPushMessageWin();
		break;
#endif

#ifdef MMI_SMS_DELALL_SUPPORT
    case MMISMS_MENU_DELALL_MSG_ITEM_ID:
        MMISMS_EnterDelAllWin();
        break;
#endif

#if defined(MMI_SMSCB_SUPPORT)
    case MMISMS_MENU_BROADCAST_ID:
        MMIAPISMSCB_OpenSettingWin();
        break;
#endif

#endif
	case MMISMS_MENU_CALL_ITEM_ID:
		MMISMS_ReplyCall(win_data_ptr->cur_focus_order_id,CC_CALL_NORMAL_CALL);
		break;

	case MMISMS_MENU_SETTING_MESSAGES_ITEM_ID:
		MMISMS_OpenMessageSetting();
		break;

	default:
		break;
    }
}

/*****************************************************************************/
//     Description : to handle the message of delete query window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxDelSameNum(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    MMI_CTRL_ID_T cur_ctrl_id = 0;
    uint16 cur_index = 0;
    BOOLEAN is_mark = FALSE;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    uint8 num_len = 0;
    uint16 i = 0;
    cur_ctrl_id = win_data_ptr->cur_list_ctrl_id;
    GUILIST_SetTextListMarkable(cur_ctrl_id, TRUE);
    GUILIST_SetMaxSelectedItem(cur_ctrl_id, MMISMS_GetMaxSmsNumInOrder());
    cur_index = GUILIST_GetCurItemIndex(cur_ctrl_id);
    num_len = MMISMS_GetOriginNumFromOrder(win_data_ptr->box_type,(uint8 *)number,cur_index);
    //SCI_TRACE_LOW:"MMISMS:Mark num all , num is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_14911_112_2_18_2_54_52_774,(uint8*)"d",GUILIST_GetTotalItemNum(cur_ctrl_id));

    for (i = 0; i< GUILIST_GetTotalItemNum(cur_ctrl_id); i++)
    {
        is_mark = MMISMS_SetPointedMsgNumMarked(win_data_ptr->box_type, (uint8 *)number, num_len, i);

        if (is_mark)
        {
            GUILIST_SetSelectedItem(cur_ctrl_id, i, TRUE);
            win_data_ptr->marked_sms_num++;
        }
    }
    #ifdef MMI_PDA_SUPPORT     //同号删除为标记状态
    GUIFORM_SetChildDisplay(MMISMS_MTBOX_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
    #endif
    if (MMISMS_IsUpdateOk())
    {
        //MsgBoxOpenDelteQueryWin(win_data_ptr);
        MMISMS_OpenDelteQueryWin(MMISMS_DELETE_PART,FALSE,win_data_ptr->cur_focus_order_id,win_data_ptr->box_type,HandleMessageBoxDelQueryWinMsg);
    }
}

/*****************************************************************************/
//     Description : to handle the message of delete query window
//    Global resource dependence :
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMessageBoxDelQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = PNULL;
    MMISMS_OPERATE_DATA_INFO_T data_info = {0};
	uint16  sms_totaldelenum =0;
#ifdef MMI_PDA_SUPPORT
    MMI_WIN_ID_T child_win_id = 0;
#endif
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_CHATBOX_CHILD_WIN_ID);
#else
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_MSGBOX_MAIN_WIN_ID);
#endif

	if(PNULL == win_data_ptr)
	{
		return MMI_RESULT_FALSE;
	}

    switch (msg_id)
    {
#if 1  // do not handle OK key
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif
    case MSG_APP_WEB:
    #ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_GetIsTimerMsg())
        {
            MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);

            return recode;
        }
    #endif
        if(win_data_ptr->is_del_all)
        {
            MMISMS_SetAllMsgMarked(win_data_ptr->box_type,TRUE);
            win_data_ptr->is_del_all = FALSE;
        }
        else
        {
            if(0 == win_data_ptr->marked_sms_num)
            {
                MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,TRUE);
            }
        }
        data_info.box_type = win_data_ptr->box_type;
	    //sms_totaldelenum =MMISMS_GetSmsBoxDeleteAvailNum(win_data_ptr->box_type);
	    //according to ui spec , should count how many conversation marked, not sms num
        if(0 == win_data_ptr->marked_sms_num && win_data_ptr->is_del_chatlist)
        {
            win_data_ptr->marked_sms_num = 1;
        }
        MMISMS_OpenDelProgressWin(&data_info,MMISMS_MessageBoxDelFinishCallback,win_data_ptr->marked_sms_num,win_data_ptr->is_del_chatlist);

#if 0  // UNISOC_MMI_Delete
        MMISMS_EnterDelWaitWin(&data_info,MMISMS_MessageBoxDelFinishCallback,(void*)win_data_ptr->win_handle);
#endif

        //for BUG 520611 & 520542
        /*if(win_data_ptr->marked_sms_num > 0)
        {
            GUILIST_SetCurItemIndex(win_data_ptr->cur_list_ctrl_id, 0);
            GUILIST_SetTopItemIndex(win_data_ptr->cur_list_ctrl_id, 0);
        }*/

        //for 126318
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        win_data_ptr->is_del_all = FALSE;

        MMISMS_CancelMarkAllItem(win_data_ptr->cur_list_ctrl_id,win_data_ptr->box_type, &win_data_ptr->marked_sms_num, FALSE);
#ifdef MMI_PDA_SUPPORT
        child_win_id = MMISMS_GetSmsFocusWinId(win_data_ptr->box_type);
        GUISOFTKEY_SetTextId(child_win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE,STXT_CANCEL,FALSE);
        win_data_ptr->is_mark_all = FALSE;
#endif
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}



LOCAL void HandleMsgBoxDelAllNoQueryWindow(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
#ifdef MMI_SMS_CHAT_SUPPORT
	uint16 cur_index = GUILIST_GetCurItemIndex(win_data_ptr->cur_list_ctrl_id);
#endif
	MMISMS_DELETE_TYPE_E delete_type = MMISMS_DELETE_MAX;
	MMISMS_OPERATE_DATA_INFO_T data_info = {0};

#ifdef MMI_SMS_CHAT_SUPPORT
	if((win_data_ptr->box_type == MMISMS_BOX_CHAT)
	    && (0 == win_data_ptr->marked_sms_num)
	    && !win_data_ptr->is_del_all)
	{
	    MMISMS_SetChatMsgMarkedInList(cur_index,TRUE);
	}
#endif

	if (MMISMS_IsUpdateOk())
	{
		if(win_data_ptr->is_del_all)
		{
		    delete_type = MMISMS_DELETE_ALL;
		}
		else
		{
#ifdef MMI_SMS_CHAT_SUPPORT
		    if(win_data_ptr->box_type == MMISMS_BOX_CHAT)
		    {
		        delete_type = MMISMS_DELETE_PART;
		    }
		    else
#endif
		    {
		        delete_type = (0 == win_data_ptr->marked_sms_num)?MMISMS_DELETE_SINGLE:MMISMS_DELETE_PART;
		    }
		}

		if(win_data_ptr->is_del_all)
		{
		    MMISMS_SetAllMsgMarked(win_data_ptr->box_type,TRUE);
		    win_data_ptr->is_del_all = FALSE;
		}
		else
		{
		    if(0 == win_data_ptr->marked_sms_num)
		    {
		        MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,TRUE);
		    }
		}
		data_info.box_type = win_data_ptr->box_type;
		MMISMS_EnterDelWaitWin(&data_info,MMISMS_MessageBoxDelFinishCallback,(void*)win_data_ptr->win_handle);

		if(win_data_ptr->marked_sms_num > 0)
		{
		    GUILIST_SetCurItemIndex(win_data_ptr->cur_list_ctrl_id, 0);
		    GUILIST_SetTopItemIndex(win_data_ptr->cur_list_ctrl_id, 0);
		}
	}
}



/*****************************************************************************/
//     Description : handle msg box answser sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxDelAll(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
#ifdef MMI_SMS_CHAT_SUPPORT
    uint16 cur_index = GUILIST_GetCurItemIndex(win_data_ptr->cur_list_ctrl_id);
#endif
    MMISMS_DELETE_TYPE_E delete_type = MMISMS_DELETE_MAX;
#ifdef MMI_SMS_CHAT_SUPPORT
    if((win_data_ptr->box_type == MMISMS_BOX_CHAT)
        && (0 == win_data_ptr->marked_sms_num)
        && !win_data_ptr->is_del_all)
    {
        MMISMS_SetChatMsgMarkedInList(cur_index,TRUE);
    }
#endif

    if (MMISMS_IsUpdateOk())
    {
        if(win_data_ptr->is_del_all)
        {
            delete_type = MMISMS_DELETE_ALL;
        }
        else
        {
            delete_type = (0 == win_data_ptr->marked_sms_num)?MMISMS_DELETE_SINGLE:MMISMS_DELETE_PART;
        }
        MMISMS_OpenDelteQueryWin(delete_type,FALSE,win_data_ptr->cur_focus_order_id,win_data_ptr->box_type,HandleMessageBoxDelQueryWinMsg);
    }
}

/*****************************************************************************/
//     Description : handle msg box lock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxLockItem(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    MMISMS_SetGlobalOperationOrderId(win_data_ptr->cur_focus_order_id);
    if (0 == win_data_ptr->marked_sms_num )
    {
        MMISMS_SetCurMsgLocked(TRUE);//锁定当前信息
    }
    else
    {
        MMISMS_SetMarkedMsgLock(win_data_ptr->box_type,TRUE);//锁定标记信息
        win_data_ptr->marked_sms_num = 0;//锁定后去掉标记
        GUILIST_SetTextListMarkable(win_data_ptr->cur_list_ctrl_id, FALSE);
    #ifdef MMI_PDA_SUPPORT
        MMISMS_SetBoxSoftkeyDisplay(win_data_ptr->box_type, GUIFORM_CHILD_DISP_HIDE);
    #endif
        MMISMS_SetAllMsgMarked(win_data_ptr->box_type, FALSE);
    }
    MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_UPDATE);
}

/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxUnLockItem(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    MMISMS_SetGlobalOperationOrderId(win_data_ptr->cur_focus_order_id);
    if (0 == win_data_ptr->marked_sms_num)
    {
        MMISMS_SetCurMsgLocked(FALSE);//解锁当前信息
    }
    else
    {
        MMISMS_SetMarkedMsgLock(win_data_ptr->box_type,FALSE);//解锁标记信息
        win_data_ptr->marked_sms_num = 0;
        GUILIST_SetTextListMarkable(win_data_ptr->cur_list_ctrl_id, FALSE);
#ifdef MMI_PDA_SUPPORT
        MMISMS_SetBoxSoftkeyDisplay(win_data_ptr->box_type, GUIFORM_CHILD_DISP_HIDE);
#endif
        MMISMS_SetAllMsgMarked(win_data_ptr->box_type, FALSE);
    }
    MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_UPDATE);
}

#ifdef MMI_SMS_MOVE_SUPPORT

/*****************************************************************************/
//     Description : handle move finish call back
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_MessageBoxMoveFinishCallback(void *user_data,MMISMS_MOVE_RESULT_INFO *data_info_ptr)
{
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)user_data;
    MMI_CTRL_ID_T cur_ctrl_id = MMISMS_GetCurActiveListCtrlId();
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms message box pnull");
        return MMI_RESULT_FALSE;
    }

    SCI_TRACE_LOW("mmisms MMISMS_MessageBoxMoveFinishCallback call back");
    win_data_ptr->marked_sms_num = 0;
    if (!MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID)
#ifdef MMI_SMS_CHAT_SUPPORT
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
        &&(MMK_IsOpenWin(MMISMS_CHATBOX_CHILD_WIN_ID)
        || MMK_IsOpenWin(MMISMS_PUSH_MESSAGE_WIN_ID)
        || MMK_IsOpenWin(MMISMS_NOSENDBOX_CHILD_WIN_ID))
#endif
#else
        && MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID)
#endif
        )
    {
        GUILIST_SetTextListMarkable(cur_ctrl_id, FALSE);
#ifdef MMI_PDA_SUPPORT
        MMISMS_SetBoxSoftkeyDisplay(win_data_ptr->box_type, GUIFORM_CHILD_DISP_HIDE);
#endif
        MMISMS_SetAllMsgMarked(win_data_ptr->box_type, FALSE);
    }

    MMISMS_SetMoveAllFlag(FALSE);
    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
    MMIAPIALM_CheckAndStartDelayedAlarm();
    MMIAPICOM_StatusAreaSetMsgState(TRUE);
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif

    if(MMISMS_NO_ERR != data_info_ptr->ret_value)
    {
        if(data_info_ptr->ret_value == MMISMS_SYSTEM_BUSY)
        {
            MMIPUB_OpenAlertFailWin(TXT_SMS_BUSY);
        }
        else if(data_info_ptr->ret_value == MMISMS_SMS_READ_FAIL)
        {
            MMIPUB_OpenAlertFailWin(TXT_SMS_READ_ERR);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
        }
    }

    MMISMS_WriteFolderType();
    MMISMS_ClearOperInfo();
    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MsgBoxMoveSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 move_sim_num = 0;
    uint32 move_mark_num = 0;
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = PNULL;

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_CHATBOX_CHILD_WIN_ID);
#else
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_MSGBOX_MAIN_WIN_ID);
#endif

    if(isReturnOK)
    {
        move_sim_num = MMISMS_GetMarkedStorageMsgNumByDualsys(win_data_ptr->box_type,MN_SMS_STORAGE_SIM,(MN_DUAL_SYS_E)dual_sys);
        move_mark_num = MMISMS_GetMarkedStorageMsgNum(win_data_ptr->box_type,MN_SMS_STORAGE_ALL);

        if(!MMIAPIPHONE_IsSimCardOk((MN_DUAL_SYS_E)dual_sys))
        {
            MMIPUB_OpenAlertWarningWin(TXT_SIM_UNAVAILABLE);
            if(0 == win_data_ptr->marked_sms_num)
            {
                MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
            }
        }
        else
        {
            if(move_sim_num == move_mark_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_SMS_NO_MSG_MOVE);
                if(0 == win_data_ptr->marked_sms_num)
                {
                    MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
                }
            }
            else
            {
                MMISMS_SetCurSaveDualSys((MN_DUAL_SYS_E)dual_sys);
                //MsgBoxOpenMoveQueryWinMsg(win_data_ptr);
                MMISMS_OpenMoveQueryWinMsg(win_data_ptr->menu_id,win_data_ptr->box_type,win_data_ptr->marked_sms_num,win_data_ptr->cur_focus_order_id,HandleMsgBoxMoveQueryWinMsg);
            }
        }
    }

    return recode;
}


/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxMoveToSIM(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    if (MMISMS_IsUpdateOk())
    {
        uint16 sim_num = 0;
        uint16 sim_ok = 0;
        MN_DUAL_SYS_E move_dual_sys = MN_DUAL_SYS_1;
        uint32 move_sim_num = 0;
        uint32 move_mark_num = 0;

        sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);

        if(win_data_ptr->marked_sms_num == 0)
        {
            MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,TRUE);
        }

        MMISMS_SetMoveOperStatus(MMISMS_MOVE_SMS_TO_SIM);

        if (0 == sim_num)
        {
            MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
            MMIPUB_OpenAlertWarningWin(TXT_SIM_UNAVAILABLE);
        }
        else if (1 == sim_num)
        {
            move_dual_sys = (MN_DUAL_SYS_E)sim_ok;

            move_sim_num = MMISMS_GetMarkedStorageMsgNumByDualsys(win_data_ptr->box_type,MN_SMS_STORAGE_SIM,move_dual_sys);
            move_mark_num = MMISMS_GetMarkedStorageMsgNum(win_data_ptr->box_type,MN_SMS_STORAGE_ALL);

            if(move_sim_num == move_mark_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_SMS_NO_MSG_MOVE);
                MMISMS_SetCurMsgMarked(FALSE);
            }
            else
            {
                MMISMS_SetCurSaveDualSys(move_dual_sys);
                //MMISMS_OperateMove();
                //MsgBoxOpenMoveQueryWinMsg(win_data_ptr);
                MMISMS_OpenMoveQueryWinMsg(win_data_ptr->menu_id,win_data_ptr->box_type,win_data_ptr->marked_sms_num,win_data_ptr->cur_focus_order_id,HandleMsgBoxMoveQueryWinMsg);
            }
        }
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
        else
        {
            MMISMS_SelectSIMFunc(MsgBoxMoveSelectSimCallback,(ADD_DATA)win_data_ptr,FALSE);
            //MMK_CreateWin((uint32 *)MMISMS_MOVE_SELECT_SIM_WIN_TAB, PNULL);
        }
    #endif
    }
}


/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxMoveToMe(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    if (MMISMS_IsUpdateOk())
    {
        MMISMS_OpenMoveQueryWinMsg(win_data_ptr->menu_id,win_data_ptr->box_type,win_data_ptr->marked_sms_num,win_data_ptr->cur_focus_order_id,HandleMsgBoxMoveQueryWinMsg);
    }
}

/*****************************************************************************/
//     Description : to handle the message of delete query window
//    Global resource dependence :
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMsgBoxMoveQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = PNULL;
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    MMISMS_MOVE_DATA_INFO_T move_data = {0};
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_CHATBOX_CHILD_WIN_ID);
#else
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_MSGBOX_MAIN_WIN_ID);
#endif
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return recode;
    }
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //MMISMS_OperateMove(win_data_ptr->cur_focus_order_id,win_data_ptr->marked_sms_num,HandleMsgBoxMoveWaitWinMsg);
        move_data.box_type = win_data_ptr->box_type;
        move_data.oper_status = MMISMS_GetCurrentMoveOperStatus();
        MMISMS_EnterMoveWaitWin(&move_data,MMISMS_MessageBoxMoveFinishCallback,win_data_ptr);

        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        if(0 == win_data_ptr->marked_sms_num)
        {
            MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
        }
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : to handle the message of move waiting window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMsgBoxMoveWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T cur_ctrl_id = MMISMS_GetCurActiveListCtrlId();
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = PNULL;
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_CHATBOX_CHILD_WIN_ID);
#else
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_MSGBOX_MAIN_WIN_ID);
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
        break;

    case MSG_CLOSE_WINDOW:
        win_data_ptr->marked_sms_num = 0;
        if (!MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID)
#ifdef MMI_SMS_CHAT_SUPPORT
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
            &&(MMK_IsOpenWin(MMISMS_CHATBOX_CHILD_WIN_ID)
            || MMK_IsOpenWin(MMISMS_PUSH_MESSAGE_WIN_ID)
            || MMK_IsOpenWin(MMISMS_NOSENDBOX_CHILD_WIN_ID))
#endif
#else
            && MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID)
#endif
            )
        {
            GUILIST_SetTextListMarkable(cur_ctrl_id, FALSE);
#ifdef MMI_PDA_SUPPORT
            MMISMS_SetBoxSoftkeyDisplay(win_data_ptr->box_type, GUIFORM_CHILD_DISP_HIDE);
#endif
            MMISMS_SetAllMsgMarked(win_data_ptr->box_type, FALSE);
        }

        if(!MMISMS_GetMoveSingleFlag())
        {
            MMISMS_ClearOperInfo();
        }
        MMISMS_SetMoveAllFlag(FALSE);
        //全部删除完毕，将未处理链表中的sms插入到相应的链表中
        MMISMS_InsertUnhandledSmsToOrder();
        MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
        MMIAPIALM_CheckAndStartDelayedAlarm();
        MMIAPICOM_StatusAreaSetMsgState(TRUE);
#ifdef MMI_TIMERMSG_SUPPORT
        MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //SCI_TRACE_LOW:"move cancel"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_24333_112_2_18_2_55_11_833,(uint8*)"");
        MMISMS_SetMoveAllFlag(FALSE);
        if(!MMISMS_GetMoveSingleFlag())
        {
            MMK_CloseWin(win_id);
        }
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

#endif

#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//     Description : handle move finish call back
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_MessageBoxCopyFinishCallback(void *user_data,MMISMS_COPY_RESULT_INFO *data_info_ptr)
{
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)user_data;
    MMI_CTRL_ID_T cur_ctrl_id = MMISMS_GetCurActiveListCtrlId();
    MMISMS_OPER_ERR_E ret_value = MMISMS_MAX_ERR;
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms message box pnull");
        return MMI_RESULT_FALSE;
    }
    SCI_TRACE_LOW("mmisms MMISMS_MessageBoxCopyFinishCallback call back");
    if(data_info_ptr != PNULL)
    {
        ret_value = data_info_ptr->ret_value;
    }
    win_data_ptr->marked_sms_num = 0;
    if (!MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID)
#ifdef MMI_SMS_CHAT_SUPPORT
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
        &&(MMK_IsOpenWin(MMISMS_CHATBOX_CHILD_WIN_ID)
        || MMK_IsOpenWin(MMISMS_PUSH_MESSAGE_WIN_ID)
        || MMK_IsOpenWin(MMISMS_NOSENDBOX_CHILD_WIN_ID))
#endif
#else
        && MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID)
#endif
        )
    {
        GUILIST_SetTextListMarkable(cur_ctrl_id, FALSE);
#ifdef MMI_PDA_SUPPORT
        MMISMS_SetBoxSoftkeyDisplay(win_data_ptr->box_type, GUIFORM_CHILD_DISP_HIDE);
#endif
        MMISMS_SetAllMsgMarked(win_data_ptr->box_type, FALSE);
    }
    MMISMS_SetMoveAllFlag(FALSE);
    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
    MMIAPIALM_CheckAndStartDelayedAlarm();
    MMIAPICOM_StatusAreaSetMsgState(TRUE);
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
    if(MMISMS_NO_ENOUGH_SPACE == ret_value)
    {
        MMISMS_OpenMsgBox(STR_NO_SPACE_EXT01, IMAGE_PUBWIN_WARNING);
    }
    else if(MMISMS_FINISH_OPER_ERR == ret_value)
    {
        MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
    }

    MMISMS_WriteFolderType();
    MMISMS_ClearOperInfo();
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MsgBoxCopySelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 copy_sim_num = 0;
    uint32 copy_mark_num = 0;
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = PNULL;

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_CHATBOX_CHILD_WIN_ID);
#else
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_MSGBOX_MAIN_WIN_ID);
#endif

    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return recode;
    }

    if (isReturnOK)
    {
        if(!MMIAPIPHONE_IsSimCardOk((MN_DUAL_SYS_E)dual_sys))
        {
            MMIPUB_OpenAlertWarningWin(TXT_SIM_UNAVAILABLE);
            if(0 == win_data_ptr->marked_sms_num)
            {
                MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
            }
        }
        else
        {
            copy_sim_num = MMISMS_GetMarkedStorageMsgNumByDualsys(win_data_ptr->box_type,MN_SMS_STORAGE_SIM,(MN_DUAL_SYS_E)dual_sys);
            copy_mark_num = MMISMS_GetMarkedStorageMsgNum(win_data_ptr->box_type,MN_SMS_STORAGE_ALL);

            if(copy_sim_num == copy_mark_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_SMS_NO_MSG_COPY);
                if(0 == win_data_ptr->marked_sms_num)
                {
                    MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
                }
            }
            else
            {
                MMISMS_SetCurSaveDualSys((MN_DUAL_SYS_E)dual_sys);
                //MMISMS_OperateCopy();
                //MsgBoxOpenCopyQueryWinMsg(win_data_ptr);
                MMISMS_OpenCopyQueryWinMsg(win_data_ptr->menu_id,win_data_ptr->box_type, win_data_ptr->marked_sms_num, win_data_ptr->cur_focus_order_id, HandleMsgBoxCopyQueryWinMsg);
            }
        }
    }

    return recode;

}

/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxCopyToSIM(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    if (MMISMS_IsUpdateOk())
    {
        uint16 sim_num = 0;
        uint16 sim_ok = 0;
        MN_DUAL_SYS_E copy_dual_sys = MN_DUAL_SYS_1;
        uint32 copy_sim_num = 0;
        uint32 copy_mark_num = 0;

        sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);

        if(0 == win_data_ptr->marked_sms_num)
        {
            MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,TRUE);
        }

        MMISMS_SetOperStatus(MMISMS_COPY_SMS_TO_SIM);

        if (0 == sim_num)
        {
            MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
            MMIPUB_OpenAlertWarningWin(TXT_SIM_UNAVAILABLE);
        }
        else if (1 == sim_num)
        {
            copy_dual_sys = (MN_DUAL_SYS_E)sim_ok;
            copy_sim_num = MMISMS_GetMarkedStorageMsgNumByDualsys(win_data_ptr->box_type,MN_SMS_STORAGE_SIM,copy_dual_sys);
            copy_mark_num = MMISMS_GetMarkedStorageMsgNum(win_data_ptr->box_type,MN_SMS_STORAGE_ALL);

            if(copy_sim_num == copy_mark_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_SMS_NO_MSG_COPY);
                MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
            }
            else
            {
                MMISMS_SetCurSaveDualSys(copy_dual_sys);
                //MMISMS_OperateCopy();
                //MsgBoxOpenCopyQueryWinMsg(win_data_ptr);
                MMISMS_OpenCopyQueryWinMsg(win_data_ptr->menu_id,win_data_ptr->box_type, win_data_ptr->marked_sms_num, win_data_ptr->cur_focus_order_id, HandleMsgBoxCopyQueryWinMsg);
            }
        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        else
        {
            MMISMS_SelectSIMFunc(MsgBoxCopySelectSimCallback,PNULL,FALSE);
        }
#endif
    }
}

/*****************************************************************************/
//     Description : handle msg box unlock item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxCopyToMe(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    if (MMISMS_IsUpdateOk())
    {
        MMISMS_SetOperStatus(MMISMS_COPY_SMS_TO_ME);
        //MsgBoxOpenCopyQueryWinMsg(win_data_ptr);
        MMISMS_OpenCopyQueryWinMsg(win_data_ptr->menu_id,win_data_ptr->box_type, win_data_ptr->marked_sms_num, win_data_ptr->cur_focus_order_id, HandleMsgBoxCopyQueryWinMsg);
    }
}

/*****************************************************************************/
//     Description : to handle the message of delete query window
//    Global resource dependence :
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMsgBoxCopyQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = PNULL;
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    MMISMS_COPY_DATA_INFO_T copy_data = {0};
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_CHATBOX_CHILD_WIN_ID);
#else
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_MSGBOX_MAIN_WIN_ID);
#endif
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return recode;
    }
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //MMISMS_OperateCopy(win_data_ptr->cur_focus_order_id,win_data_ptr->marked_sms_num,HandleMsgBoxCopyWaitWinMsg);
        copy_data.box_type = win_data_ptr->box_type;

        if(MMISMS_MENU_BOX_COPYTOSIM_ITEM_ID == win_data_ptr->menu_id)
        {
            copy_data.oper_status = MMISMS_COPY_SMS_TO_SIM;
        }
        else if(MMISMS_MENU_BOX_COPYTOME_ITEM_ID == win_data_ptr->menu_id)
        {
            copy_data.oper_status = MMISMS_COPY_SMS_TO_ME;
        }

        MMISMS_EnterCopyWaitWin(&copy_data,MMISMS_MessageBoxCopyFinishCallback,win_data_ptr);
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        if(0 == win_data_ptr->marked_sms_num)
        {
            MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,FALSE);
        }
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : to handle the message of move waiting window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMsgBoxCopyWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T cur_ctrl_id = MMISMS_GetCurActiveListCtrlId();
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = PNULL;
   #ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_CHATBOX_CHILD_WIN_ID);
#else
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_MSGBOX_MAIN_WIN_ID);
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
        break;

    case MSG_CLOSE_WINDOW:
        win_data_ptr->marked_sms_num = 0;
        if (!MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID)
#ifdef MMI_SMS_CHAT_SUPPORT
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
            &&(MMK_IsOpenWin(MMISMS_CHATBOX_CHILD_WIN_ID)
            || MMK_IsOpenWin(MMISMS_PUSH_MESSAGE_WIN_ID)
            || MMK_IsOpenWin(MMISMS_NOSENDBOX_CHILD_WIN_ID))
#endif
#else
            && MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID)
#endif
            )
        {
            GUILIST_SetTextListMarkable(cur_ctrl_id, FALSE);
#ifdef MMI_PDA_SUPPORT
            MMISMS_SetBoxSoftkeyDisplay(win_data_ptr->box_type, GUIFORM_CHILD_DISP_HIDE);
#endif
            MMISMS_SetAllMsgMarked(win_data_ptr->box_type, FALSE);
        }

        if(!MMISMS_GetMoveSingleFlag())
        {
            MMISMS_ClearOperInfo();
        }
        MMISMS_SetMoveAllFlag(FALSE);
        //全部删除完毕，将未处理链表中的sms插入到相应的链表中
        MMISMS_InsertUnhandledSmsToOrder();
        MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
        MMIAPIALM_CheckAndStartDelayedAlarm();
        MMIAPICOM_StatusAreaSetMsgState(TRUE);
#ifdef MMI_TIMERMSG_SUPPORT
        MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //SCI_TRACE_LOW:"move cancel"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_24333_112_2_18_2_55_11_833,(uint8*)"");
        MMISMS_SetMoveAllFlag(FALSE);
        if(!MMISMS_GetMoveSingleFlag())
        {
            MMK_CloseWin(win_id);
        }
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}


#endif

/*****************************************************************************/
//     Description : handle msg box answser sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxWinSmsKeyCall(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr,MMI_MESSAGE_ID_E msg_id)
{
#ifdef MMI_MULTI_GREEN_KEY_SINGLE
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
#else
    CC_CALL_SIM_TYPE_E sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);
#endif
    uint32 call_type = CC_CALL_NORMAL_CALL;
    uint8 num_len = 0;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MN_DUAL_SYS_E     dual_sys = MN_DUAL_SYS_MAX;

#ifdef VT_SUPPORT
    if(MSG_KEYLONG_GREEN == msg_id)
    {
        call_type = CC_CALL_VIDEO_CALL;
    }
#endif
    if(PNULL == win_data_ptr->cur_focus_order_id)
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
        return;
    }

    if (MMISMS_TYPE_WAP_OTA  == win_data_ptr->cur_focus_order_id->flag.msg_type ||
        MMISMS_TYPE_MMS_OTA  == win_data_ptr->cur_focus_order_id->flag.msg_type ||
        MMISMS_TYPE_WAP_PUSH  == win_data_ptr->cur_focus_order_id->flag.msg_type)
    {
        return;
    }
    num_len = MMISMS_GetCurNumByOrder(win_data_ptr->cur_focus_order_id,&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);
    if (num_len > 0)
    {
        MMIAPICC_MakeCall(
            dual_sys,
            number,
            num_len,
            PNULL,
            PNULL,
            sim_type,
            call_type,
            PNULL
            );
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
    }
}



/*****************************************************************************/
//     Description : handle delete finish call back
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_MessageBoxDelFinishCallback(void *user_data,MMISMS_DELETE_DATE_INFO *data_info_ptr)
{
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_CHATBOX_CHILD_WIN_ID);

    MMI_CTRL_ID_T cur_ctrl_id = MMISMS_GetCurActiveListCtrlId();
    if(PNULL == win_data_ptr || PNULL == data_info_ptr)
    {
       return MMI_RESULT_FALSE;
    }
    SCI_TRACE_LOW("mmisms MMISMS_MessageBoxDelFinishCallback ");
    win_data_ptr->marked_sms_num = 0;
    if (
		!MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID)
#ifdef MMI_SMS_CHAT_SUPPORT
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
        && MMK_IsOpenWin(MMISMS_CHATBOX_CHILD_WIN_ID)
        || MMK_IsOpenWin(MMISMS_NOSENDBOX_CHILD_WIN_ID)
#endif
#else
        ||MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID)
#endif
        )
    {
        GUILIST_SetTextListMarkable(cur_ctrl_id, FALSE);
		GUILIST_SetListState(cur_ctrl_id, GUILIST_STATE_CYC_SCROLL_ITEM, FALSE);

        GUIFORM_SetChildDisplayOnly(MMISMS_CHATBOX_FORM_CTRL_ID,MMISMS_CHATBOX_CHILD_BUTTON_FORM_CTRL_ID,GUIFORM_CHILD_DISP_NORMAL);
        if(!data_info_ptr->is_del_fail)
        {
            MMK_SetAtvCtrl(MMISMS_CHATBOX_CHILD_WIN_ID, MMISMS_CHATBOX_CHILD_BUTTON_FORM_CTRL_ID);
        }

#ifdef MMI_PDA_SUPPORT
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
        MMISMS_SetBoxSoftkeyDisplay(win_data_ptr->box_type, GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildDisplay(MMISMS_CHATBOX_FORM_CTRL_ID, MMISMS_CHATBOX_WRITEMSG_BUTTON_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
#else
#ifdef MMI_SMS_CHAT_SUPPORT
        if(MMISMS_BOX_CHAT == win_data_ptr->box_type)
        {
            GUISOFTKEY_SetTextId(MMISMS_CHATBOX_CHILD_WIN_ID,MMICOMMON_SOFTKEY_CTRL_ID, TXT_NULL, TXT_COMMON_WRITE_MESSAGE, TXT_NULL, FALSE);
        }
        else
#endif
        {
            MMISMS_SetBoxSoftkeyDisplay(win_data_ptr->box_type, GUIFORM_CHILD_DISP_HIDE);
        }
#endif
#endif
        MMISMS_SetAllMsgMarked(win_data_ptr->box_type, FALSE);

    }
        //finish the del
    if(data_info_ptr->is_mms_del)
    {
        MMIAPIMMS_SaveListInfo();
    }

    if ((MMISMS_BOX_SENDFAIL == win_data_ptr->box_type || MMISMS_BOX_MT == win_data_ptr->box_type) && data_info_ptr->mms_is_sendrecv)
    {
        SCI_TRACE_LOW("BOX_TYPE = %d, mms_is_sendrecv = %d",win_data_ptr->box_type,data_info_ptr->mms_is_sendrecv);
        MMISMS_OpenMsgBox(TXT_SMS_BUSY, IMAGE_PUBWIN_FAIL);
    }
    else
    {
#ifndef MMI_PDA_SUPPORT
#if 0  //UNISOC_MMI_Delete
        MMISMS_OpenMsgBox(TXT_DELETED, IMAGE_PUBWIN_SUCCESS);
#endif
#endif
    }

//for 126318
    MMK_CloseWin(MMISMS_QUERY_WIN_ID);
    MMK_CloseWin(MMISMS_DELETE_ANIM_WIN_ID);

    MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
    MMK_CloseWin(MMISMS_MTBOX_WIN_ID);
    MMK_CloseWin(MMISMS_SENDSUCCBOX_WIN_ID);
    MMK_CloseWin(MMISMS_MOBOX_WIN_ID);
    MMIAPICOM_StatusAreaSetMsgState(TRUE);
    MMISMS_SetDelAllFlag(FALSE);
    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif

    return MMI_RESULT_TRUE;
}

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
// 	Description : open draft box
//	Global resource dependence :
//  Author:rong.gu
//	Note:
/*****************************************************************************/
LOCAL void HandleOpenDraftBoxWin(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    MMK_CreateWin((uint32 *)MMISMS_NOSENDBOX_CHILD_WIN_TAB, (ADD_DATA)win_data_ptr->win_id);
    win_data_ptr->box_type = MMISMS_BOX_NOSEND;
    MMISMS_SetCurType(MMISMS_BOX_NOSEND);
    win_data_ptr->cur_list_ctrl_id = MMISMS_NOSENDBOX_LISTBOX_CTRL_ID;
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:rong.gu
//	Note:
/*****************************************************************************/
LOCAL void ButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr)
{
    uint32 ctrl_id=0;
    GUI_RECT_T rect={0};//max_rect与owner_draw_ptr->display_rect相交区域，也即图片最后显示区域
    GUI_RECT_T max_rect={0};//图片可以显示的最大区域
    GUI_RECT_T img_rect={0};//图片裁剪区
    MMI_HANDLE_T win_handle = 0;
    MMI_WIN_ID_T    win_id=0;
    uint16 image_w = 0;
    uint16 iamge_h = 0;
    GUI_RECT_T ctrl_rect={0};

    if(PNULL == owner_draw_ptr)
    {
        return;
    }
    ctrl_id=MMK_GetCtrlId(owner_draw_ptr->ctrl_handle);
    win_handle=MMK_GetWinHandleByCtrl(owner_draw_ptr->ctrl_handle);
    win_id=MMK_GetWinId(win_handle);
#ifdef MMI_PDA_SUPPORT
    GUIRES_GetImgWidthHeight(&image_w, &iamge_h, IMAGE_SMS_WRITE_SMS_ADD_CONTACT, win_id);
#endif
    GUICTRL_GetRect(MMK_GetCtrlPtr(owner_draw_ptr->ctrl_handle),&ctrl_rect);

    max_rect.left=owner_draw_ptr->display_x + 2;
    max_rect.top= owner_draw_ptr->display_y + (ctrl_rect.bottom - ctrl_rect.top) / 2 - iamge_h / 2 ;
    max_rect.right=ctrl_rect.right;
    max_rect.bottom=max_rect.top + iamge_h;

    if(!GUI_IntersectRect(&rect,max_rect,owner_draw_ptr->display_rect))
    {//没有相交区域，返回
    	return ;
    }

    img_rect.left = 0;
    img_rect.top = 0;//(ctrl_rect.bottom + ctrl_rect.top-iamge_h) /2;
    img_rect.right =  image_w;
    img_rect.bottom = iamge_h;

    if(GUI_IsInvalidRect(img_rect)||GUI_IsInvalidRect(rect))//无效矩形不要画了
    {
    	return ;
    }
#ifdef MMI_PDA_SUPPORT
    GUIRES_DisplayImg(PNULL, &rect, &img_rect, win_handle, IMAGE_SMS_WRITE_SMS_ADD_CONTACT, &owner_draw_ptr->lcd_dev);
#endif
}
#endif

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  rong.gu
// Note:
/*****************************************************************************/
LOCAL void MsgBoxWinCloseCtrlFunc (MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    if (win_data_ptr != PNULL)
    {
        SCI_FREE(win_data_ptr);
    }
}

#ifdef MMI_GUI_STYLE_TYPICAL
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : pre enter mt toolbar
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_MTBoxPreEntryToolbar(
                                 MMI_HANDLE_T        ctrl_handle,
                                 void*               param_ptr
                                 )
{
    MMI_CTRL_ID_T           ctrl_id = (MMI_CTRL_ID_T)param_ptr;
    uint16 cur_index= GUILIST_GetCurItemIndex(MMISMS_MTBOX_LISTBOX_CTRL_ID);
    MMISMS_ORDER_ID_T current_order_id = PNULL;
    uint32 cur_recor_id = 0;
    uint32 avail_del_num = 0;
    current_order_id = MMISMS_GetLinkedArrayPtrIdInList(MMISMS_BOX_MT ,cur_index);
    if(PNULL == current_order_id)
    {
        return;
    }
    MMISMS_SetCurOperationOrderId(MMISMS_BOX_MT ,cur_index);
    cur_recor_id = MMISMS_CurMsgRecordId();
    avail_del_num = MMISMS_GetSmsBoxDeleteAvailNum(MMISMS_BOX_MT);

    if((MMISMS_TYPE_WAP_PUSH == current_order_id->flag.msg_type)
        ||(MMISMS_TYPE_WAP_OTA == current_order_id->flag.msg_type)
        ||(MMISMS_TYPE_SMS == MMISMS_GetMsgType() && MN_NUM_TYPE_ALPHANUMERIC == MMIPB_GetNumberTypeFromUint8(current_order_id->number.bcd_number.npi_ton))
        )
    {
        GUITOOLBAR_SetItemGray(ctrl_id, 0, TRUE);
        GUITOOLBAR_SetItemGray(ctrl_id, 1, TRUE);
    }
    else
    {
         GUITOOLBAR_SetItemGray(ctrl_id, 0, FALSE);
         GUITOOLBAR_SetItemGray(ctrl_id, 1, FALSE);
    }
    if ( MMISMS_CurMsgIsLocked()
        || ((  MMISMS_TYPE_WAP_PUSH == MMISMS_GetMsgType()
        || MMISMS_TYPE_WAP_OTA == MMISMS_GetMsgType()
        || MMISMS_TYPE_MMS == MMISMS_GetMsgType()
        || MMISMS_TYPE_MMS_OTA == MMISMS_GetMsgType())
        && MMIAPIUDISK_UdiskIsRun())
        || MMIAPIMMS_IsProcessMMSById(cur_recor_id)
        )
    {
        GUITOOLBAR_SetItemGray(ctrl_id, 2, TRUE);
    }
    else
    {
        GUITOOLBAR_SetItemGray(ctrl_id, 2, FALSE);
    }

}


/*****************************************************************************/
//  Description : create toolbar
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_CreateToolbar(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id,MMI_CTRL_ID_T attach_ctrl_id)
{
#ifdef GUIF_TOOLBAR
    GUITOOLBAR_INIT_DATA_T init_data = {0};
    MMK_GetBothRect(attach_ctrl_id, &init_data.both_rect);

    GUITOOLBAR_CreateDynamic(win_id, ctrl_id, &init_data);
    if(MMISMS_MTBOX_CHILD_WIN_ID == win_id)
    {
        GUITOOLBAR_ITEM_T cl_toolbar_inbox_item_a [] =
        {
            {SMS_TOOLBAR_DIAL_AUDIO, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_SMS_CALL, IMAGE_TOOLBAR_SMS_CALL_GRAY,0},/*lint !e785*/
            {SMS_TOOLBAR_SEND_SMS, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_SMS_REPLY, IMAGE_TOOLBAR_SMS_REPLY_GRAY,0},/*lint !e785*/
            {SMS_TOOLBAR_SMS_DELETE, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_SMS_DEL, IMAGE_TOOLBAR_SMS_DEL_GRAY,0},/*lint !e785*/
        };

        GUITOOLBAR_SetAllItem(ctrl_id,
        cl_toolbar_inbox_item_a,
        sizeof(cl_toolbar_inbox_item_a)/sizeof(GUITOOLBAR_ITEM_T));

        GUITOOLBAR_SetPreExpandFunc(ctrl_id, (TOOLBAR_PRE_EXPAND_FUNC)MMISMS_MTBoxPreEntryToolbar,(void*)ctrl_id);
    }
#endif
}

/*****************************************************************************/
//  Description : update toolbar
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_MTBoxUpdateWinToolbar(void)
{
    uint16  icon_width = 0;
    uint16  icon_height = 0;
    uint16  cur_index = 0;
    GUI_RECT_T cur_rect = {0};
    GUI_POINT_T pt_bar = {0};
    int16 count = 0;
    BOOLEAN  bMark = FALSE;

    count = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_MT);
    bMark = GUILIST_GetListStateById( MMISMS_MTBOX_LISTBOX_CTRL_ID, GUILIST_STATE_ENABLE_MARK );

    if(0 == count || bMark)
    {
        GUITOOLBAR_SetVisible(MMISMS_TOOLBAR_MTBOX_CTRL_ID, FALSE);
        return;
    }
    else
    {
        GUITOOLBAR_SetVisible(MMISMS_TOOLBAR_MTBOX_CTRL_ID, TRUE);
    }

    cur_index = GUILIST_GetCurItemIndex(MMISMS_MTBOX_LISTBOX_CTRL_ID);

    GUILIST_GetItemRect(MMISMS_MTBOX_LISTBOX_CTRL_ID, cur_index, &cur_rect);
    GUITOOLBAR_GetIconWidthHeight(MMISMS_TOOLBAR_MTBOX_CTRL_ID, &icon_width, &icon_height);

    pt_bar.x = cur_rect.left;
    if(0 != cur_index)
    {
        pt_bar.y = cur_rect.bottom - MMISMS_TOOLBAR_PANEL_GAP + 1; //向上
        GUITOOLBAR_SetPanelPopForm(MMISMS_TOOLBAR_MTBOX_CTRL_ID, GUITOOLBAR_DIR_DOWN);
    }
    else
    {
        pt_bar.y = cur_rect.bottom + MMISMS_TOOLBAR_PANEL_GAP - 1; //向下
        GUITOOLBAR_SetPanelPopForm(MMISMS_TOOLBAR_MTBOX_CTRL_ID, GUITOOLBAR_DIR_UP);
    }

    GUITOOLBAR_SetPanelBasePt(MMISMS_TOOLBAR_MTBOX_CTRL_ID, &pt_bar, FALSE);

    cur_rect.right -= MMISMS_TOOLBAR_ICON_GAP_RIGHT;
    cur_rect.left = cur_rect.right - icon_width + 1;
    cur_rect.top += MMISMS_TOOLBAR_ICON_GAP_TOP;
    cur_rect.bottom = cur_rect.top + icon_height - 1;
    GUITOOLBAR_SetIconRect(MMISMS_TOOLBAR_MTBOX_CTRL_ID, &cur_rect, TRUE);

}
/*****************************************************************************/
//  Description : send sms func
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_ToolBarItemSendSMS(MMI_WIN_ID_T    win_id)
{
    MMI_STRING_T edit_string = {0};

    SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
    GUIEDIT_GetString(MMISMS_EDIT_SMS_TEXT_CTRL_ID, &edit_string);
    // to save the information
    MMISMS_SetMessageContent(edit_string.wstr_len, edit_string.wstr_ptr, TRUE, &g_mmisms_global.edit_content);

    GetNumberFromEditbox(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID,FALSE);
    // open the input number window
    if (0 < MMISMS_GetDestCount())
    {
        HandleMessageboxSendMsg(MMISMS_GetDestDualSys(),win_id);
    }
    else
    {
        uint16 num_count  = 0;
        uint16 max_selected_num = 0;
        num_count = GUIEDIT_GetListItemNum(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID);
        if(num_count == 0)
        {
            //MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
            //Add by huang fengming for cr148999
            max_selected_num= MMISMS_DEST_ADDR_MAX_NUM - MMISMS_GetDestCount();

           /* MMIAPIPB_OpenNumberList(
				MMIPB_LIST_FOR_SMS,
				MMIPB_LIST_CHECK,
				max_selected_num,
				PNULL
				);*/
                MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_NUMBER,
                        max_selected_num,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        EditSmsNumSelectContactCallback
                        );
        }
    }
}

/*****************************************************************************/
//  Description :hand toolbar item
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_HandleToolBarItemSelect(MMI_WIN_ID_T    win_id, SMS_TOOLBAR_TYPE_E node_id)
{
    uint8 num_len = 0;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};

    switch(node_id)
    {
        case SMS_TOOLBAR_DIAL_AUDIO:
            {
                MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
                num_len = MMISMS_GetOriginNumByPos(&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);

                if (num_len > 0)
                {
                      MMIAPICC_MakeCall(
                                                                        dual_sys,
                                                                        number,
                                                                        num_len,
                                                                        PNULL,
                                                                        PNULL,
                                                                        CC_CALL_SIM_MAX,
                                                                        CC_CALL_NORMAL_CALL,
                                                                        PNULL);
                }
                else
                {
                     MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
                }
            }
            break;

        case SMS_TOOLBAR_SEND_SMS:
            {
                MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
                //打开写消息窗口
                if (0 == s_marked_sms_num)
                {
                    // get the address of the SMS message
                    num_len = MMISMS_GetOriginNumByPos(&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);

                    //open edit window and set the address to the deliver address
                    MMISMS_WriteNewMessage(
                                            dual_sys,
                                            PNULL,
                                            FALSE,
                                            number,
                                            num_len
                                            );
                }
                else
                {
                    MN_DUAL_SYS_E origin_dual_sys = MMISMS_GetMtBoxMarkedOriginCard();
                    MMISMS_DEST_ADDR_INFO_T dest_info = {0};
                    //open edit window and set the address to the deliver address
                    MMISMS_GetMtBoxMarkedDestNum(&dest_info);
                    MMISMS_SendGroupSms( origin_dual_sys, PNULL, &dest_info);

                    s_marked_sms_num = 0;//回复后去掉标记
                    GUILIST_SetTextListMarkable(MMISMS_MTBOX_LISTBOX_CTRL_ID, FALSE);
                    MMISMS_SetAllMsgMarked(MMISMS_BOX_MT, FALSE);
                }
            }
            break;

        case SMS_TOOLBAR_SMS_DELETE:
            {
                if (MMISMS_IsUpdateOk())
                {
                   OpenDelteQueryWin((uint32)node_id);
                }
            }
            break;

        case SMS_TOOLBAR_WRITESMS_SEND:
            {
                MMISMS_ToolBarItemSendSMS(win_id);
            }
            break;

        case SMS_TOOLBAR_WRITESMS_CONTACT:
            {
                uint16 max_selected_num= 0;
                GetNumberFromEditbox(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID,TRUE);
                max_selected_num= MMISMS_DEST_ADDR_MAX_NUM - MMISMS_GetDestCount();

                /*MMIAPIPB_OpenNumberList(
                 MMIPB_LIST_FOR_SMS,
                 MMIPB_LIST_CHECK,
                 max_selected_num,
                 PNULL
                 );*/
                MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_NUMBER,
                        max_selected_num,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        EditSmsNumSelectContactCallback
                        );
            }
            break;

        case SMS_TOOLBAR_WRITESMS_TEMPLATE:
            {
                //MMK_CreateWin((uint32 *)MMISMS_SELECTWORD_WIN_TAB, PNULL);
                MMISMS_CreateSelectWordWin(MMISMS_EDIT_SMS_TEXT_CTRL_ID);
                MMK_SetAtvCtrl(win_id, MMISMS_EDIT_SMS_TEXT_CTRL_ID);
            }
            break;

       default:
	    break;
    }
}
#endif
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : handle menu key opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxWinOpenMenuKeyOpt(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr,BOOLEAN is_enable_mark)
{
     MMIPUB_MENU_PARAM_T menuparam = {0};
     if(is_enable_mark)
     {
         if(0 == win_data_ptr->marked_sms_num)
         {
             if(win_data_ptr->cur_focus_order_id != PNULL && win_data_ptr->cur_focus_order_id->flag.is_locked)
             {
                 menuparam.group_id = MENU_SMS_MARK_UNLOCK_MENU_OPT;
             }
             else
             {
                 menuparam.group_id = MENU_SMS_MARK_MENU_OPT;
             }
         }
         else
         {
             if( (win_data_ptr->box_type == MMISMS_BOX_MT && PNULL == MMISMS_FindValidMTSMSForDelAll())
             || (win_data_ptr->box_type != MMISMS_BOX_MT && PNULL == MMISMS_FindValidMoSMSForDelAll(win_data_ptr->box_type))
             )
             {
                 menuparam.group_id = MENU_SMS_MARK_UNLOCK_MENU_OPT;
             }
             else
             {
                 menuparam.group_id = MENU_SMS_MARK_MENU_OPT;
             }
         }
         win_data_ptr->group_id = menuparam.group_id;
         menuparam.win_id =   MMISMS_MARK_OPT_WIN_ID;
         menuparam.ctrl_id =  MMISMS_MARK_MENU_OPT_MENU_CTRL_ID;
         menuparam.dst_handle = win_data_ptr->win_id;
         menuparam.func.OnMenuOk = HandleMsgWinOpenBoxMenuOpt;
         menuparam.func.OnMenuInit = HandleMsgWinOpenListMarkInit;
         MMIPUB_OpenPubMenuWin( &menuparam );
     }
     else
     {
         menuparam.group_id = MENU_SMS_MTLIST_MENUKEY_OPT;
         menuparam.win_id =   MMISMS_MTBOX_MENUKEY_WIN_ID;
         menuparam.ctrl_id =  MMISMS_MTBOX_MENUKEY_OPT_MENU_CTRL_ID;
         menuparam.dst_handle = win_data_ptr->win_id;
         menuparam.func.OnMenuOk = HandleMsgWinOpenBoxMenuOpt;
         menuparam.func.OnMenuInit = HandleMsgWinOpenMenuKeyInit;
         MMIPUB_OpenPubMenuWin( &menuparam );
     }
}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenMenuKeyInit(MMIPUB_MENU_NOTIFY_T *param)
{
    MMI_MENU_EANBLE_GRAY_T menu_enable_t = {0};
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = PNULL;
    MMI_STRING_T box_str = {0};
    MMI_TEXT_ID_T      text_id = IMAGE_NULL;
    uint16 list_item_num = 0;

    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if(PNULL == win_data_ptr)
    {
        return;
    }
    MMISMS_SetSortMenuFocusItem(MMISMS_MTBOX_MENUKEY_OPT_MENU_CTRL_ID,MENU_SMS_MTLIST_SORT_OPT);

#if 0
    switch(win_data_ptr->box_type)
    {
    case MMISMS_BOX_MT:
        text_id = TXT_INBOX;
        break;
    case MMISMS_BOX_SENDSUCC:
        text_id = TXT_SMS_SENTBOX;
        break;
    case MMISMS_BOX_SENDFAIL:
        text_id = TXT_COMM_OUTBOX;
        break;
    case MMISMS_BOX_NOSEND:
        text_id = TXT_COMM_DRAFT;
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BOX_SECURITY:
        text_id = TXT_SMS_SECURITY;
        break;
#endif
    default:
        break;
    }
#else
    text_id = TXT_COMMON_SORT;
#endif
    MMI_GetLabelTextByLang(text_id , &box_str);
    GUIMENU_SetMenuTitle(&box_str, MMISMS_MTBOX_MENUKEY_OPT_MENU_CTRL_ID);
    menu_enable_t.group_id = MENU_SMS_MTLIST_MENUKEY_OPT;
    menu_enable_t.win_id = MMISMS_MTBOX_MENUKEY_WIN_ID;
    MMISMS_HandleSortMenuEnableGray(&menu_enable_t);

    list_item_num = GUILIST_GetTotalItemNum(win_data_ptr->cur_list_ctrl_id);
    if(0 == list_item_num)
    {
        GUIMENU_SetItemGrayed(MMISMS_MTBOX_MENUKEY_OPT_MENU_CTRL_ID, MENU_SMS_MTLIST_MENUKEY_OPT, MMISMS_MENU_MARK_ITEM_ID, TRUE);
    }

}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenListMarkInit(MMIPUB_MENU_NOTIFY_T *param)
{
    MMI_MENU_EANBLE_GRAY_T menu_enable_t = {0};
    MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_MESSAGEBOX_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);

    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    menu_enable_t.cur_order_id = win_data_ptr->cur_focus_order_id;
    menu_enable_t.box_type = win_data_ptr->box_type;
    menu_enable_t.marked_num = win_data_ptr->marked_sms_num;
    menu_enable_t.win_id = MMISMS_MARK_OPT_WIN_ID;
    menu_enable_t.group_id = win_data_ptr->group_id;

    MMISMS_HandleMarkMenuEnableGray(&menu_enable_t);
    if(0 == win_data_ptr->marked_sms_num)
    {
    #ifdef MMI_SMS_MOVE_SUPPORT
        MMIAPICOM_EnableGrayed(menu_enable_t.win_id, menu_enable_t.group_id,MMISMS_MENU_MOVE_ITEM_ID,TRUE);
    #endif
    #ifdef MMI_SMS_COPY_SUPPORT
        MMIAPICOM_EnableGrayed(menu_enable_t.win_id, menu_enable_t.group_id,MMISMS_MENU_COPY_ITEM_ID,TRUE);
    #endif
        MMIAPICOM_EnableGrayed(menu_enable_t.win_id, menu_enable_t.group_id,MMISMS_MENU_LOCK_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(menu_enable_t.win_id, menu_enable_t.group_id,MMISMS_MENU_UNLOCK_ITEM_ID,TRUE);
    }
    else if(MMISMS_GetMarkedMsgNumByType(win_data_ptr->box_type,MMISMS_TYPE_MMS) == win_data_ptr->marked_sms_num)
    {
    #ifdef MMI_SMS_COPY_SUPPORT
        MMIAPICOM_EnableGrayed(menu_enable_t.win_id, menu_enable_t.group_id,MMISMS_MENU_COPY_ITEM_ID,TRUE);
    #endif
    }
#ifdef MMI_SMS_COPY_SUPPORT
    MMISMS_SetCopyMenuTitle();
#endif

}


#endif


#if defined(MAINLCD_SIZE_128X64) || defined(TOUCH_PANEL_SUPPORT)
/*****************************************************************/
//  Description :号码编辑菜单选择联系人的callback
//  Global resource dependence : none
//  Author:
//  Note:
//  Date:
/*****************************************************************/
LOCAL void EditSmsNumSelectContactCallback(MMIPB_HANDLE_T handle)
{
#if defined(MAINLCD_SIZE_128X64)
    MMK_SendMsg(MMISMS_EDIT_SMS_NUMBER_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);
#endif
}
#endif

/******************************************************************************
//  Description : Callback of read sms
//	Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL BOOLEAN MessageboxWinReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T *callback_data)
{
    if (callback_data == PNULL)
    {
        SCI_TRACE_LOW("mmisms callback_data pnull");
        return TRUE;
    }
    SCI_TRACE_LOW("mmisms enter MessageboxWinReadSmsCallback");
    if(callback_data->result)
    {
        if (!MMK_SendMsg((MMI_HANDLE_T)callback_data->user_data,MSG_SMS_FINISH_READ,PNULL))
        {
            MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
        }

    }
    else
    {
        MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
    }
    return TRUE;
}

/*****************************************************************************/
//     Description : handle msg box edit sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxEditMsg(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    MMISMS_ORDER_ID_T cur_order_id = win_data_ptr->cur_focus_order_id;
	MMI_STRING_T   prompt_str = {0};
    if(PNULL == cur_order_id)
    {
        return;
    }
    MMISMS_SetSMSUpdateInfo(cur_order_id);
    MMISMS_SetGlobalOperationOrderId(cur_order_id);

    if (MMISMS_TYPE_SMS == cur_order_id->flag.msg_type) //IS SMS
    {
#ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_GetIsTimerMsg())
        {
            MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
            return;
        }
#endif

       MMI_GetLabelTextByLang( STR_NOTE_WAITING, &prompt_str );
       MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
           ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,MMISMS_HandleWaitWinMsg);

       // to read the pointed SMS
//       MMISMS_ReadPointedSMS();
       if(!MMISMSAPP_ReadOneSms(cur_order_id,
                            MessageboxWinReadSmsCallback,
                            PNULL,
                            (void*)win_data_ptr->win_handle))
        {
           //Error handler
        }


    }
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //Added by yuanl  2010.8.20
    else if (MMISMS_TYPE_MAIL == cur_order_id->flag.msg_type)
    {
        mail_EditMailByRecordId((uint32)cur_order_id->record_id);

    }
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */
    else //IS MMS
    {
        MMIAPIMMS_DraftEditMMS((uint32)cur_order_id->record_id);
    }
}

/*****************************************************************************/
//     Description : handle msg box forward sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxResendMsg(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    MMISMS_ORDER_ID_T  cur_order_id = win_data_ptr->cur_focus_order_id;

    if(PNULL == cur_order_id)
    {
        return;
    }
    MMISMS_SetSMSUpdateInfo(cur_order_id);
    MMISMS_SetGlobalOperationOrderId(cur_order_id);

    if (MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
    {
        if(MMISMS_IsSmsBusy(cur_order_id))
        {
            MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
            return;
        }
        HandleMsgBoxResendSms(win_data_ptr);
    }
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
    else if(MMISMS_TYPE_MAIL == cur_order_id->flag.msg_type)
    {
        MMIAPIMAIL_SendMailById((uint32)mail_getMsgIdByRecordId(cur_order_id->record_id));
    }
#endif
    else //is mms
    {
        MMIAPIMMS_OutboxSendMMS((uint32)cur_order_id->record_id);
    }
}
/*****************************************************************************/
//     Description : handle msg box forward sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxResendSms(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MN_DUAL_SYS_E my_dualsys = MN_DUAL_SYS_1;
    uint16 sim_num = 0;
    uint16 sim_ok = 0;

    sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);

    if (0 == sim_num)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (1 == sim_num)
    {
        my_dualsys = (MN_DUAL_SYS_E)sim_ok;
        MMISMS_ResendSMSbyOrderID(win_data_ptr->cur_focus_order_id,my_dualsys);
    }
    else
    {
        MMISET_MAJOR_SIM_SEL_TYPE_E major_select_sim = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;
        major_select_sim=MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_SMS);
        if(major_select_sim<MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
        {
            my_dualsys = MN_DUAL_SYS_1+((MN_DUAL_SYS_E)major_select_sim-MMISET_MAJOR_SIM_SEL_SIM1);
        }
        else
        {
#ifdef MMI_SMS_FIX_SEND_SIM
            if (MMISMS_GetFixSIMSendingOnOFF())
            {
                my_dualsys = (MN_DUAL_SYS_E)MMISMS_GetFixSIMSendingSIMSYS();
            }
            else
#endif
            {
                MMISMS_SelectSIMFunc(SendSelectSimCallback,(ADD_DATA)win_data_ptr,TRUE);
                return;
            }

        }

        MMISMS_ResendSMSbyOrderID(win_data_ptr->cur_focus_order_id,my_dualsys);
    }
#else
    if (!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1))
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else
    {
        MMISMS_ResendSMSbyOrderID(win_data_ptr->cur_focus_order_id,MN_DUAL_SYS_1);
    }
#endif

}
/*****************************************************************************/
//     Description : handle msg box forward sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxForwardMsg(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    MMISMS_ORDER_ID_T  cur_order_id = win_data_ptr->cur_focus_order_id;
//	MMI_STRING_T   prompt_str = {0};
    if(PNULL == cur_order_id)
    {
        return;
    }
    if (MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
    {

       MMISMS_ForwardSMSbyOrderID(cur_order_id);
    }
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //Added by yuanl  2010.8.20
    else if (MMISMS_TYPE_MAIL == cur_order_id->flag.msg_type)
    {
        mail_ForwardMailByRecordId((uint32)cur_order_id->record_id);
        MMK_CloseWin( win_id );
    }
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */
    else //IS MMS, not OTA and PUSH
    {
        MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_SENDBOX_LIST_FARWARD);
        MMIAPIMMS_FarwardMMS((uint32)cur_order_id->record_id);

        //MMIAPIMMS_OpMMS((uint32)cur_order_id->record_id, MMIMMS_OP_FOWARD, win_id);
        //MMK_CloseWin(win_id);
    }
}

#if 0
/*****************************************************************************/
//     Description : handle msg box forward sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxForwardSMS(MMISMS_MESSAGEBOX_WINDATA_T *win_data_ptr)
{
    MMI_STRING_T msg_content = {0};

    // set the init SMS content
    MMISMS_GetSMSContent(&msg_content);

    // open edit window
    MMISMS_WriteNewMessage(
        MN_DUAL_SYS_MAX,
        &msg_content,
        FALSE,
        PNULL,
        0
        );

    MMK_CloseWin(MMISMS_SENDSUCCBOX_WIN_ID);
    if(MMK_IsOpenWin(MMISMS_READSMS_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
    }
    if (PNULL != msg_content.wstr_ptr)
    {
        SCI_FREE(msg_content.wstr_ptr);
        msg_content.wstr_ptr = PNULL;
    }
}
#endif

//shipeng.lv end of cr150245
/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void SMS_ValidatePrivacyEnterSmsBox(BOOLEAN is_permitted, DPARAM param_ptr)
{
    MMISMS_BOX_TYPE_E box_type = MMISMS_BOX_NONE;

    if (is_permitted)
    {
        if (PNULL != param_ptr)
        {
            box_type = *(MMISMS_BOX_TYPE_E*)param_ptr;
        }
        MMISMS_EnterSmsBox(box_type);
    }
}

/*****************************************************************************/
//     Description : to open the list of box
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_OpenSmsBox(
                       MMISMS_BOX_TYPE_E box_type
                       )
{
    BOOLEAN is_not_inited = FALSE;

    is_not_inited = MMISMS_IsSimNotInited();

    if(MMISMS_IsOrderOk() && MMISMS_IsMMSOrderReady()) // 增加判断mms是否排序完毕
    {
        //隐私保护
        if (MMIAPISET_IsOpenPrivacyProtect( MMISET_PROTECT_SMS )
            && !MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID)
            && !MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID)
            && !MMIAPISET_IsValidatePrivacyWinOpen(MMISET_PROTECT_SMS))
        {
            MMIAPISET_ValidatePrivacy( MMISET_PROTECT_SMS, SMS_ValidatePrivacyEnterSmsBox, &box_type, sizeof(box_type));
        }
        else
        {
            MMISMS_EnterSmsBox(box_type);
        }
    }
    else if(is_not_inited)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_INIT);
    }
    else
    {
        // 需要开启等待窗口
        MMISMS_SetBoxType(box_type);
        MMISMS_OpenInitWaitingWindow(MMISMS_OPEN_BOX);
    }
}




