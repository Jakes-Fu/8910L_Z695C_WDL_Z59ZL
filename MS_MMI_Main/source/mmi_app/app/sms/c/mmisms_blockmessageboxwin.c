/****************************************************************************
** File Name:      mmisms_chatreadwin.c                                    *
** Author:         liming deng                                             *
** Date:           07/20/2015                                        *
** Copyright:      2015 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe deleting SMS.              *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2015
**
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "mmisms_blockmessageboxwin.h"
#include "mmiudisk_export.h"
#include "mmimms_export.h"
#include "mmisms_pushmsgboxwin.h"
#include "guictrl_api.h"
#include "mmisms_appcontrol.h"
#include "mmisms_filecontrol.h"
#include "mmialarm_export.h"
#include "mmiidle_statusbar.h"
#include "mmicom_statusarea.h"
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
}MMISMS_MESSAGEBOX_BLOCK_WINDATA_T;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/

#ifdef MMI_SMS_CHAT_SUPPORT
BOOLEAN is_BlockedMessageWindow = FALSE;
#endif
/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//MMISMS_DELMSG_GLOBLE_T g_sms_delmsg_all_info = {0};


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUCTION DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : open read message
//    Global resource dependence :
//  Author:
//    Note:读取内容的窗口(MT,MO Success)
/*****************************************************************************/
LOCAL void HandleMsgBoxOpenReadMessage(MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : open read message
//    Global resource dependence :
//  Author:
//    Note:编辑内容的窗口(DRAFT,MO FAIL)
/*****************************************************************************/
LOCAL void HandleMsgBoxEditSMS(MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr);

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void ButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr);
#endif

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : to handle the message of SMS box window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleChatBoxBlackChildWinMsg(
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

#endif

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenBlockBoxMenuOpt(MMIPUB_MENU_NOTIFY_T *param );


/*****************************************************************************/
//     Description : handle msg box answser sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxDelAll(MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr);

LOCAL void HandleMsgBoxDelAllNoQueryWindow(MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr);


/*****************************************************************************/
//  Description : build win user data
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MsgBoxBuildWinUserData(MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : handle menu key opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxWinOpenMenuKeyOpt(MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr,BOOLEAN is_enable_mark);

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
//     Description : open read message
//    Global resource dependence :
//  Author:rong.gu
//    Note:显示内容的窗口(MT,MO Success)
/*****************************************************************************/
/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/



#ifdef MMI_SMS_CHAT_SUPPORT
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
WINDOW_TABLE( MMISMS_CHATBOX_BLOCK_CHILD_WIN_TAB ) =
{
    WIN_TITLE(STXT_SMS_FILTERED),
    WIN_FUNC((uint32) HandleChatBoxBlackChildWinMsg),
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
    WIN_ID( MMISMS_CHATBOX_BLOCK_CHILD_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_CHATBOX_LISTBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL,TXT_NULL,TXT_COMMON_COMMON_BACK),
    END_WIN
};
#endif


#endif


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : to Enter the list of box
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_EnterBlackSmsBox( MMISMS_BOX_TYPE_E        box_type)
{
    BOOLEAN is_not_inited = FALSE;
    MMI_HANDLE_T win_handle = 0;
    MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr = PNULL;

    if (MMK_IsOpenWin(MMISMS_CHATBOX_BLOCK_CHILD_WIN_ID))   //不可重入
    {
        MMK_CloseWin(MMISMS_SHOW_CHAT_SMS_WIN_ID);
        return;
    }

    win_data_ptr = (MMISMS_MESSAGEBOX_BLOCK_WINDATA_T*)SCI_ALLOCAZ(sizeof(MMISMS_MESSAGEBOX_BLOCK_WINDATA_T));
    if(PNULL == win_data_ptr)
    {
       SCI_TRACE_LOW("mmisms win_data_ptr alloc fail");
       return;
    }
    is_not_inited = MMISMS_IsSimNotInited();

    if (MMISMS_IsOrderOk() && MMISMS_IsMMSOrderReady()) // 增加判断mms是否排序完毕
    {
        win_data_ptr->box_type = MMISMS_BOX_CHAT;
        win_data_ptr->win_id = MMISMS_CHATBOX_BLOCK_CHILD_WIN_ID;
#ifdef MMI_SMS_CHAT_SUPPORT
        win_data_ptr->cur_list_ctrl_id = MMISMS_CHATBOX_LISTBOX_CTRL_ID;
#endif
        MMISMS_SetCurType(box_type);
#ifdef MMI_SMS_CHAT_SUPPORT
        win_handle = MMK_CreateWin((uint32 *)MMISMS_CHATBOX_BLOCK_CHILD_WIN_TAB, PNULL);
 #endif
        if(win_handle == PNULL)
        {
            SCI_FREE(win_data_ptr);
            return;
        }
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
    SCI_TRACE_LOW("mmisms open chat read win cur_order_id = %d",cur_order_id);
    //save chat number
    MMISMS_SetSMSUpdateInfo(cur_order_id);
    MMISMS_SetGlobalOperationOrderId(cur_order_id);
    MMISMS_EnterSMSChatReadWin(cur_order_id);
    MMI_GetLabelTextByLang(STR_NOTE_WAITING, &prompt_str);
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,MMISMS_HandleWaitWinMsg);
}
#endif



/*****************************************************************************/
//     Description : open read message
//    Global resource dependence :
//  Author:rong.gu
//    Note:显示内容的窗口(MT,MO Success)
/*****************************************************************************/
LOCAL void HandleMsgBoxShowSMS(MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr)
{
    MMISMS_READ_MSG_T *read_msg = PNULL;
	MMISMS_ClearOperInfo();
    read_msg = MMISMS_GetReadResult();
    MMISMS_EnterShowSmsWin(read_msg);
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
//  Description : build win user data
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MsgBoxBuildWinUserData(MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr)
{
    win_data_ptr->marked_sms_num = 0;
}

#ifdef MMI_SMS_CHAT_SUPPORT

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgBoxWinOpenChatBoxBlockOpt(MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};

    menuparam.group_id = MENU_SMS_BLOCK_CHAT_MAIN_OPT;

    win_data_ptr->group_id = menuparam.group_id;
    menuparam.win_id =   MMISMS_CHATBOX_BLOCK_CHILD_OPT_WIN_ID;
    menuparam.ctrl_id =  MMISMS_CHATBOX_BLOCK_OPT_MENU_CTRL_ID;
    menuparam.dst_handle = win_data_ptr->win_id;
    menuparam.func.OnMenuOk = HandleMsgWinOpenBlockBoxMenuOpt;
    MMIPUB_OpenPubMenuWin( &menuparam );
}

/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatBoxDateChangeCallBack(MMIEVENT_INFO_T *event_info_ptr)
{
    MMI_WIN_ID_T        win_id = MMISMS_CHATBOX_BLOCK_CHILD_WIN_ID;
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

	/* Messaging Delete multiple Converstation (Mark OptionMenu) */
/*****************************************************************************/
//    Description : define pub-menu window for Marked-Message List
//    Global resource dependence :
//    Author:
//    Note: menu item will be changed according to number of  marked-list item
/*****************************************************************************/
LOCAL void HandleMsgBoxWinOpenChatBoxOptMarkable(MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr)
{
	MMIPUB_MENU_PARAM_T menuparam = {0};
	uint16 sms_totalnum = 0;
        uint16 item_marked_num = GUILIST_GetSelectedItemNum(win_data_ptr->cur_list_ctrl_id);

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
	menuparam.win_id =   MMISMS_CHATBOX_BLOCK_CHILD_OPT_WIN_ID;
	menuparam.ctrl_id =  MMISMS_CHATBOX_BLOCK_OPT_MENU_CTRL_ID;
	menuparam.dst_handle = win_data_ptr->win_id;
	menuparam.func.OnMenuOk = HandleMsgWinOpenBlockBoxMenuOpt;
	MMIPUB_OpenPubMenuWin( &menuparam );
}

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC  void MMISmsAPI_SetBlockedMessageWindow(BOOLEAN  is_BlockedWindow)
{
	is_BlockedMessageWindow = is_BlockedWindow;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMISmsAPI_GetBlockedMessageWindowStatus(  void)
{
	return is_BlockedMessageWindow;
}
#endif

/*****************************************************************************/
//  Description : to handle the message of SMS box window
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleChatBoxBlackChildWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E msg_id,        //IN:
    DPARAM            param        //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_CHATBOX_LISTBOX_CTRL_ID;
    static uint16 selected_index = 0;
    LOCAL BOOLEAN s_have_items = FALSE;
    LOCAL BOOLEAN s_need_update = FALSE;
    GUILIST_MARK_ITEM_T* list_param_ptr = NULL;
    uint16 i = 0;
    uint16 index = 0;
    MMI_STRING_T        item_str = {0};
	BOOLEAN is_need_edit_draft = FALSE;

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr = (MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *)MMK_GetWinUserData(win_id);
#endif

	if(PNULL == win_data_ptr)
	{
		return MMI_RESULT_FALSE;
	}

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
            {
                GUIFORM_CHILD_HEIGHT_T    button_height  = {0};
                button_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
                button_height.add_data = 42;
                GUIFORM_SetChildHeight(MMISMS_CHATBOX_FORM_CTRL_ID, MMISMS_CHATBOX_WRITEMSG_BUTTON_CTRL_ID, &button_height);
                GUIBUTTON_SetTextId(MMISMS_CHATBOX_WRITEMSG_BUTTON_CTRL_ID, TXT_COMMON_WRITE_MESSAGE);
                GUIBUTTON_PermitBorder(MMISMS_CHATBOX_WRITEMSG_BUTTON_CTRL_ID, FALSE);
                GUIBUTTON_SetOwnerDraw(MMISMS_CHATBOX_WRITEMSG_BUTTON_CTRL_ID, ButtonOwnerDrawFun);
            }
#endif
#ifdef MMI_PDA_SUPPORT
            GUIAPICTRL_SetBothRect(ctrl_id, &list_both_rect);
#endif
            MsgBoxBuildWinUserData(win_data_ptr);
            GUILIST_SetNeedPrgbar(ctrl_id, TRUE);

            // load items to list box
            s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_CHAT, ctrl_id );
#if 0  // yuguang.liu for bug 493746
            if (s_have_items)
            {
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
            }
            else
            {
                GUILIST_EMPTY_INFO_T empty_info = {0};
                wchar text_buffer[] = {' ', 0};

                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
                empty_info.text_buffer.wstr_ptr = text_buffer;
                empty_info.text_buffer.wstr_len = MMIAPICOM_Wstrlen(text_buffer);
                GUILIST_SetEmptyInfo(ctrl_id, &empty_info);
            }
#endif
            GUILIST_SetDefaultIcon(MMISMS_CHATBOX_LISTBOX_CTRL_ID, IMAGE_SMS_CHAT_DEFAULT, IMAGE_SMS_CHAT_DEFAULT);

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
            MMISMS_SetCurType(MMISMS_BOX_CHAT);
            win_data_ptr->box_type = MMISMS_BOX_CHAT;
            GUIFORM_SetChildDisplay(MMISMS_CHATBOX_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleChatBoxDateChangeCallBack, TRUE);
            MMK_SetAtvCtrl(win_id, ctrl_id);
            s_need_update = FALSE;
            break;
			
		case MSG_NOTIFY_CONTENT_OVERFLOW:
			GUIWIN_UpdateTitle(win_id);
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
//BUG 505046
# if 0
                    if (0 == win_data_ptr->marked_sms_num)
                    {
                        GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, TRUE);
                    }
#endif
#endif
                }
            }
            break;

        case MMISMS_MSG_NEEDUPDATELIST:
            s_need_update = TRUE;
            break;

        case MSG_GET_FOCUS:
            if (win_data_ptr->box_type != MMISMS_BOX_CHAT) //从其他信箱切换回来
            {
                //清除掉原来的标记
                win_data_ptr->marked_sms_num = 0;
                for (i = 0; i < GUILIST_GetTotalItemNum(ctrl_id); i++)
                {
                    GUILIST_SetSelectedItem(ctrl_id, i, FALSE);
                }
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);
#ifndef MMI_PDA_SUPPORT
                GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, FALSE);
#endif

                MMISMS_SetAllMsgMarked(MMISMS_BOX_CHAT, FALSE);

                //重新load信息list
                MMISMS_SetCurType(MMISMS_BOX_CHAT);
                win_data_ptr->box_type = MMISMS_BOX_CHAT;

                if (MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID))
                {
                    GUIMENU_SetSelectedItem(MMISMS_MENU_CTRL_ID, 0);
                }
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_CHAT, ctrl_id );
                if (s_have_items)
                {
                    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_NONE );
                }
                else
                {
                    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_NONE );
                }
                GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                s_need_update = FALSE;
#ifdef MMI_PDA_SUPPORT
                win_data_ptr->is_mark_all = FALSE;
#endif
            }
            else
            {
                    //uint16 item_totalnum = GUILIST_GetTotalItemNum(ctrl_id);
				s_have_items = MMISMS_LoadSmsListFromOrder(MMISMS_BOX_CHAT, ctrl_id);

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
                MMISMS_SetCurType(MMISMS_BOX_CHAT);
#endif

#ifndef MMI_PDA_SUPPORT
                if(s_have_items>0)
                {
                    if (GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
                    {
                        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, STXT_SOFTKEY_MARK_MK, STXT_RETURN, FALSE);
                    }
                    else
                    {
                        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_VIEW, STXT_RETURN, FALSE);
                    }
                }
                else
                {
                    //GUIWIN_SetSoftkeyTextId(win_id,  IMAGE_NULL, TXT_NULL, (MMI_IMAGE_ID_T)IMAGE_COMMON_BACK,  TRUE);
                    GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL,TXT_NULL , TRUE);
					GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
					GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, TRUE);
                }
#endif
                if (s_need_update || MMISMS_IsNeedSort())
                {
                    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                    s_have_items = MMISMS_LoadSmsListFromOrder( MMISMS_BOX_CHAT, ctrl_id );
                    if (s_have_items)
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_NONE );
                    }
                    else
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_NONE );
                    }
                    s_need_update = FALSE;
                }
            }

            GUILIST_SetCurItemIndex(ctrl_id, selected_index);
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
            break;

        case MSG_FULL_PAINT:
            if(MMK_GetWinDirty(win_id))
            {
                if (MMK_IsFocusWin(win_id))
                {
                    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                    s_have_items = MMISMS_LoadSmsListFromOrder(MMISMS_BOX_CHAT, ctrl_id);
                    if (s_have_items)
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_NONE );
                    }
                    else
                    {
                        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_NONE );
                    }
                    GUILIST_SetCurItemIndex(ctrl_id, selected_index);

                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
                    MMK_SetWinDirty(win_id,FALSE);
                }
            }
            if(!GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
            {
                GUILIST_SetHandleLeftRightEx(ctrl_id, TRUE, FALSE);
            }
            else
            {
                GUILIST_SetHandleLeftRightEx(ctrl_id, FALSE, FALSE);
            }
            break;
        case MSG_CTL_OK:
        case MSG_APP_OK:
            {
                uint16 item_totalnum = GUILIST_GetTotalItemNum(ctrl_id);
                if(item_totalnum>0)
                {
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
							break;
						}
                        HandleMsgBoxWinOpenChatBoxBlockOpt(win_data_ptr);
                    }
                }
            }
            break;
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MMISMS_MSG_READ_MSG:
			selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            win_data_ptr->cur_focus_order_id = MMISMS_GetNumNodeByIndexInListEx(selected_index, &is_need_edit_draft);

            if (GUILIST_GetItemState((GUILIST_ITEM_T*) GUILIST_GetItemPtrByIndex (ctrl_id, GUILIST_GetCurItemIndex(ctrl_id)), GUIITEM_STATE_LEFT_FOCUS))
            {
                win_data_ptr->is_del_all = FALSE;
                HandleMsgBoxDelAll(win_data_ptr);
            }
            else if (s_have_items && (win_data_ptr->marked_sms_num == 0) )
            {
                //only one draft OR draft is mms type
                if(is_need_edit_draft)
                {
                    MMISMS_SetDraftOperationOrderId(win_data_ptr->cur_focus_order_id);
                    HandleMsgBoxOpenReadMessage(win_data_ptr);
                }
                else
                {
                    win_data_ptr->cur_focus_order_id = MMISMS_GetNumNodeByIndexInList(selected_index);
					MMISMS_OpenChatReadWin(win_data_ptr->cur_focus_order_id);
                }
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
		if (GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
		{
            uint16 item_marked_num = GUILIST_GetSelectedItemNum(ctrl_id);
            if(item_marked_num>0)
            {
                HandleMsgBoxDelAll(win_data_ptr);
            }
            else
            {
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);
				GUIFORM_SetChildDisplayOnly(MMISMS_CHATBOX_FORM_CTRL_ID,MMISMS_CHATBOX_CHILD_BUTTON_FORM_CTRL_ID,GUIFORM_CHILD_DISP_NORMAL);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
            }
        }
		else
		{
            if (GUILIST_GetItemState((GUILIST_ITEM_T*) GUILIST_GetItemPtrByIndex (ctrl_id, GUILIST_GetCurItemIndex(ctrl_id)), GUIITEM_STATE_LEFT_FOCUS))
            {
                MMK_SendMsg(win_id, MSG_APP_RIGHT, PNULL);
            }
            else
            {
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                GUIFORM_SetChildDisplayOnly(MMISMS_CHATBOX_FORM_CTRL_ID,MMISMS_CHATBOX_CHILD_BUTTON_FORM_CTRL_ID,GUIFORM_CHILD_DISP_NORMAL);
                MMK_CloseWin(win_id);
            }
		}
#endif
            break;

        case MSG_SMS_FINISH_READ:
            HandleMsgBoxEditSMS(win_data_ptr);

            break;

        case MSG_CLOSE_WINDOW:
            win_data_ptr->marked_sms_num = 0;
            MMISMS_SetAllMsgMarked(MMISMS_BOX_CHAT, FALSE);
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleChatBoxDateChangeCallBack, FALSE);
            MMISmsAPI_SetBlockedMessageWindow(FALSE);
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
            SCI_FREE(win_data_ptr);
#endif
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
#endif

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
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleMsgWinOpenBlockBoxMenuOpt(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 group_id, menu_id = 0;

    MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);

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
        case MMISMS_MENU_DEL_ITEM_ID:
         //   HandleMsgBoxDelAll(win_data_ptr);
			HandleMsgBoxDelAllNoQueryWindow(win_data_ptr);
            break;

        case MMISMS_MENU_DELBOX_ALL_ITEM_ID:
            win_data_ptr->is_del_all = TRUE;
            HandleMsgBoxDelAll(win_data_ptr);
            break;

#ifdef MMI_SMS_CHAT_SUPPORT
    case MMISMS_MENU_CHAT_OPT_MARK_ITEM_ID:
            MMISMS_MarkChatItem(win_data_ptr->cur_focus_order_id,win_data_ptr->cur_list_ctrl_id,&win_data_ptr->marked_sms_num);
            break;

    case MMISMS_MENU_CHAT_CANCEL_MARK_ITEM_ID:
            MMISMS_CancelMarkChatItem(win_data_ptr->cur_focus_order_id,win_data_ptr->cur_list_ctrl_id,&win_data_ptr->marked_sms_num);
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
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT

#ifdef MMI_SMS_DELALL_SUPPORT
    case MMISMS_MENU_DELALL_MSG_ITEM_ID:
        MMISMS_EnterDelAllWin();
        break;
#endif

#endif
        case MMISMS_MENU_CHAT_DELETE_CONVERSATION_CANCEL_ID:
            win_data_ptr->marked_sms_num = 0;
            MMISMS_SetAllMsgMarked(MMISMS_BOX_CHAT, FALSE);
            GUILIST_SetTextListMarkable(win_data_ptr->cur_list_ctrl_id, FALSE);
            MMK_PostMsg(win_data_ptr->win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
            break;

	default:
		break;
    }
}


/*****************************************************************************/
//     Description : to handle the message of delete query window
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMessageBoxDelQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr = PNULL;
    MMISMS_OPERATE_DATA_INFO_T data_info = {0};
	uint16  sms_totaldelenum =0;
#ifdef MMI_PDA_SUPPORT
    MMI_WIN_ID_T child_win_id = 0;
#endif
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    win_data_ptr = (MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *)MMK_GetWinUserData(MMISMS_CHATBOX_BLOCK_CHILD_WIN_ID);
#endif

	if(PNULL == win_data_ptr)
	{
		return MMI_RESULT_FALSE;
	}

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
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
	sms_totaldelenum =MMISMS_GetSmsBoxDeleteAvailNum(win_data_ptr->box_type);
	MMISMS_OpenDelProgressWin(&data_info,MMISMS_MessageBoxDelFinishCallback,sms_totaldelenum,FALSE);

        if(win_data_ptr->marked_sms_num > 0)
        {
            GUILIST_SetCurItemIndex(win_data_ptr->cur_list_ctrl_id, 0);
            GUILIST_SetTopItemIndex(win_data_ptr->cur_list_ctrl_id, 0);
        }

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



LOCAL void HandleMsgBoxDelAllNoQueryWindow(MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr)
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
LOCAL void HandleMsgBoxDelAll(MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr)
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
        MMISMS_OpenDelteQueryWin(delete_type,FALSE,win_data_ptr->cur_focus_order_id,win_data_ptr->box_type,HandleMessageBoxDelQueryWinMsg);
    }
}

/*****************************************************************************/
//     Description : handle delete finish call back
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_MessageBoxDelFinishCallback(void *user_data,MMISMS_DELETE_DATE_INFO *data_info_ptr)
{
    //MMI_HANDLE_T win_handle =  (MMI_HANDLE_T)user_data;
    MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr = (MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *)MMK_GetWinUserData(MMISMS_CHATBOX_BLOCK_CHILD_WIN_ID);
    MMI_CTRL_ID_T cur_ctrl_id = MMISMS_GetCurActiveListCtrlId();
    if(win_data_ptr != PNULL )
    {
        win_data_ptr->marked_sms_num = 0;
    }
    if (
		!MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID)
#ifdef MMI_SMS_CHAT_SUPPORT
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
        && MMK_IsOpenWin(MMISMS_CHATBOX_BLOCK_CHILD_WIN_ID)
#endif
#endif
        )
    {
        GUILIST_SetTextListMarkable(cur_ctrl_id, FALSE);
		GUILIST_SetListState(cur_ctrl_id, GUILIST_STATE_CYC_SCROLL_ITEM, FALSE);
        GUIFORM_SetChildDisplayOnly(MMISMS_CHATBOX_FORM_CTRL_ID,MMISMS_CHATBOX_CHILD_BUTTON_FORM_CTRL_ID,GUIFORM_CHILD_DISP_NORMAL);
        MMK_SetAtvCtrl(MMISMS_CHATBOX_CHILD_WIN_ID, MMISMS_CHATBOX_CHILD_BUTTON_FORM_CTRL_ID);
		//MMK_SetAtvCtrl(MMISMS_CHATBOX_CHILD_WIN_ID, MMISMS_CHATBOX_CHILD_BUTTON_FORM_CTRL_ID);
        MMISMS_SetAllMsgMarked(MMISMS_BOX_CHAT, FALSE);
    }
        //finish the del
    if(data_info_ptr->is_mms_del)
    {
        MMIAPIMMS_SaveListInfo();
    }


//for 126318
    MMK_CloseWin(MMISMS_QUERY_WIN_ID);
    MMK_CloseWin(MMISMS_DELETE_ANIM_WIN_ID);

    MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
    MMIAPICOM_StatusAreaSetMsgState(TRUE);
    MMISMS_SetDelAllFlag(FALSE);
    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
//    MMK_CloseWin(MMISMS_CHATBOX_BLOCK_CHILD_WIN_ID);
    return MMI_RESULT_TRUE;
}

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT


/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:
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

/******************************************************************************
//  Description : Callback of read sms
//	Global resource dependence :
//  Author:
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
//     Description : open read message
//    Global resource dependence :
//  Author:
//    Note:读取内容的窗口(MT,MO Success)
/*****************************************************************************/
LOCAL void HandleMsgBoxOpenReadMessage(MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr)
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
//  Author:
//    Note:编辑内容的窗口(DRAFT,MO FAIL)
/*****************************************************************************/
LOCAL void HandleMsgBoxEditSMS(MMISMS_MESSAGEBOX_BLOCK_WINDATA_T *win_data_ptr)
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
    MMISMS_SetDraftOperationOrderId(win_data_ptr->cur_focus_order_id);
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

