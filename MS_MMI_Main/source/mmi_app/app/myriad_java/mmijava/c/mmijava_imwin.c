/****************************************************************************
** File Name:      mmijava_imwin.c
** Author:
** Date:           2012/8/15
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.
** Description:    This file is used to describe functions of permission set
****************************************************************************/

#include "window_parse.h"
#include "mmijava_id.h"
#include "java_mmi_text.h"
#include "mmijava_imwin.h"
#include "mmidisplay_data.h"
#include "mmijava_runwin.h"
#include "mmitheme_pos.h"
#include "mmk_type.h"
#include "ctrllabel_export.h"
#include "mmi_appmsg.h"
#include "mmi_string.h"
#include "guilabel.h"
#include "mmiim.h"
#include "cafcontrol.h"
#include <jbed_exported_defines.h>
#include <jpl_nativetextbox.h>

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
#define JAVA_EDITOR_TEXTBOX_OPT_MAX 15
LOCAL BOOLEAN s_java_textbox_in_foreground = FALSE;
LOCAL BOOLEAN s_is_txt_edit_win_used  =  FALSE;
LOCAL MMIEvent * s_callback_mmi_event_head_ptr = NULL;
LOCAL MMIJAVA_TEXTEDITBOX_INIT_DATA_T init_data = {0};
LOCAL GUI_RECT_T s_editbox_rect = {0x0, };
LOCAL BOOLEAN s_is_editor_url = FALSE;
LOCAL int editor_csk_functionality_num = 1; // tells the index of the command to be executed from the commandlist in 'options' in editor

typedef enum
{
    JAVA_EDITOR_OPT_MIN = JAVA_EDITOR_TEXTBOX_OPT_MAX,
    JAVA_EDITOR_OPT_PREDICT_MODE,
    JAVA_EDITOR_OPT_WRITING_LANGUAGE,
    JAVA_EDITOR_OPT_MAX
}JAVA_EDITOR_OPT_MENU_CMD;

LOCAL MMI_RESULT_E HandleJavaTextEditWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleJavaTextEditMenuMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param);
LOCAL void MMIJAVA_IM_CloseTextEditWindow();
LOCAL void IMCallbackProcess(
    MMIJAVA_IM_CALLBACK_TYPE_E type,
    int cmdIdx,
    MMI_STRING_T * editbox_str_ptr,
    MMIJAVA_IM_CALLBACK_FUNC callbackfun);

MMIJAVA_IM_ERR_E  MMIJAVA_SendSignalToAppThread( uint16 SignalMsg);
PUBLIC void MMIJAVA_SetEditorTypeURL(BOOLEAN is_url);
BOOLEAN  HandleMmiJavaSig(PWND app_ptr,uint16 msg_id,DPARAM param);

WINDOW_TABLE(JBED_MMI_TEXT_EDIT_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleJavaTextEditWinMsg),
    WIN_ID(JBED_IM_EDITBOX_WIN_ID),
#if (defined __MMI_PARAGON_UX__)
   WIN_STATUSBAR,
#endif
    //WIN_TITLE(TXT_NULL),
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    #ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
    #else
   WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
   #endif
    END_WIN
};

WINDOW_TABLE(JBED_MMI_TEXT_EDIT_MENU_TAB) =
{
    WIN_FUNC((uint32)HandleJavaTextEditMenuMsg),
    WIN_ID(JBED_IM_EDITBOX_MENU_ID),
    //WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_SOFTKEY_SELECT_MK, TXT_NULL, STXT_SOFTKEY_DONE_MK),
    END_WIN
};

PUBLIC BOOLEAN MMIJAVA_IsNativeTextEditorOpened(void)
{
    return s_is_txt_edit_win_used;
}

LOCAL void createJavaTextEditorMenu(MMI_WIN_ID_T   win_id,MMI_CTRL_ID_T  ctrl_id)
{
    MMI_STRING_T tstring = {0};
    MMI_STRING_T menu_string = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    NativeCommand * nativeCmd =  init_data.cmdList;
    int idx = 0;

    GUIMENU_CreatDynamic(PNULL, win_id, ctrl_id, GUIMENU_STYLE_POPUP);

    //GUIMENU_SetDynamicMenuSoftkey(ctrl_id, STXT_SELECT, TXT_NULL, STXT_RETURN);
    if (init_data.isTextBox)
    {
        while (init_data.cmdNum > 0 && nativeCmd)
        {
            menu_string.wstr_ptr = nativeCmd->lable;
            menu_string.wstr_len = MMIAPICOM_Wstrlen(menu_string.wstr_ptr);
            node_item.item_text_ptr = &menu_string;
            node_item.select_icon_id = 0;
            GUIMENU_InsertNode(idx, nativeCmd->nativeID, 0, &node_item, ctrl_id);
            idx++;
            nativeCmd = nativeCmd->nextCmd;
        }
    }
    else
    {
        MMI_GetLabelTextByLang(STXT_SOFTKEY_OK_MK, &menu_string);
        node_item.item_text_ptr = &menu_string;
        GUIMENU_InsertNode(idx, JAVA_EDITOR_OPT_MIN, 0, &node_item, ctrl_id);    //JAVA_EDITOR_OPT_MIN for text filed OK function
        idx++;
    }

}

LOCAL MMI_RESULT_E HandleJavaTextEditMenuMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMI_CONTROL_CREATE_T create = {0};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        createJavaTextEditorMenu(win_id, MMIJAVA_IM_TEXT_EDIT_BOX_MENU_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIJAVA_IM_TEXT_EDIT_BOX_MENU_CTRL_ID);
        break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
     case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_KEYDOWN_OK, PNULL);
                break;

            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_KEYDOWN_CANCEL, PNULL);
                break;
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT
#else
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT
#endif
    case MSG_CTL_OK:
    case MSG_KEYDOWN_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            int cmdId = GUIMENU_GetCurNodeId(MMIJAVA_IM_TEXT_EDIT_BOX_MENU_CTRL_ID);
            MMI_STRING_T editbox_str_info = {0};

            //if (cmdId > init_data.cmdNum)
            if(!isTextBoxCommand(cmdId))
            {
                if (cmdId == JAVA_EDITOR_OPT_MIN)
                {
                    GUIEDIT_GetString(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, &editbox_str_info);
                    IMCallbackProcess(MMIJAVA_IM_CALLBACK_TYPE_OK, 0, &editbox_str_info, init_data.callbackfunc);
                    s_is_txt_edit_win_used = FALSE;
                    MMK_CloseWin(JBED_IM_EDITBOX_WIN_ID);
                }
                MMK_CloseWin(win_id);
            }
            else
            {
                GUIEDIT_GetString(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, &editbox_str_info);
                IMCallbackProcess(MMIJAVA_IM_CALLBACK_TYPE_COMMAND_FIRED, cmdId, NULL, init_data.callbackfunc);
                MMK_CloseWin(win_id);
            }
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_KEYDOWN_RED:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        break;

    case MSG_BACKLIGHT_TURN_ON:
        MMK_SendMsg(JBED_IM_EDITBOX_WIN_ID, MSG_BACKLIGHT_TURN_ON, PNULL);
        break;

    case MSG_BACKLIGHT_TURN_OFF:
        MMK_SendMsg(JBED_IM_EDITBOX_WIN_ID, MSG_BACKLIGHT_TURN_OFF, PNULL);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

void JavaTextEditDecimalDotNullSwitch(BOOLEAN isDot)
{
    wchar                   dot_str[] = {'.'};
    if(MMIJAVA_IsNativeTextEditorOpened())
    {
        if(isDot)
        {
            GUIEDIT_ConfigImCustomKey(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
        }
        else
        {
            GUIEDIT_ConfigImCustomKey(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 0);
        }
    }
}

void clearTextBoxInitData()
{
    SCI_TRACE_LOW("NativeTextBoxPeer  clear init data");
    freeMmiStr(init_data.text);
    freeMmiStr(init_data.title);
    freeMmiStr(init_data.ticker);
}

LOCAL MMI_RESULT_E HandleJavaTextEditWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMI_STRING_T  editbox_str_info = {0};
    MMI_CONTROL_CREATE_T create = {0};
    MMIJAVA_TEXTEDITBOX_INIT_DATA_T* init_data_ptr = (MMIJAVA_TEXTEDITBOX_INIT_DATA_T*)MMK_GetWinAddDataPtr(win_id);
    GUIEDIT_INIT_DATA_T edit_init_data = {0};
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    BOOLEAN hasTicker = FALSE;
    wchar                   dot_str[] = {'.'};
    #define TICKER_HEIGHT 20
    if(PNULL == init_data_ptr)
        return MMI_RESULT_TRUE;

    SCI_MEMSET(&editbox_str_info, 0, sizeof(MMI_STRING_T));
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        init_data.winId = win_id;
        if (init_data_ptr->ticker && init_data_ptr->ticker->wstr_ptr && init_data_ptr->ticker->wstr_len > 0) {
            hasTicker = TRUE;
        }

        edit_init_data.type = init_data_ptr->edit_type;

        //due to no GUIEDIT_TYPE_DECIMAL type use GUIEDIT_TYPE_MAX instead
        if (GUIEDIT_TYPE_NULL == edit_init_data.type
             || GUIEDIT_TYPE_MAX == edit_init_data.type
             || GUIEDIT_TYPE_DIGITAL == edit_init_data.type
             || GUIEDIT_TYPE_PHONENUM == edit_init_data.type) {
            edit_init_data.type = GUIEDIT_TYPE_TEXT;
        }

        edit_init_data.both_rect = both_rect;
        if (hasTicker) {
            edit_init_data.both_rect.h_rect.bottom -= TICKER_HEIGHT;
            edit_init_data.both_rect.v_rect.bottom -= TICKER_HEIGHT;
        }
        edit_init_data.str_max_len = init_data_ptr->max_ucs2_len;
        create.ctrl_id = MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID;
        create.guid    = SPRD_GUI_EDITBOX_ID;
        create.parent_win_handle = win_id;
        create.init_data_ptr = &edit_init_data;

        MMK_CreateControl( &create );
        GUIEDIT_SetStyle(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, GUIEDIT_STYLE_MULTI_UP);
        s_is_txt_edit_win_used = TRUE;
        GUIEDIT_SetMaxLen(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, init_data_ptr->max_ascii_len);
        GUIEDIT_SetTextMaxLen(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, init_data_ptr->max_ascii_len,init_data_ptr->max_ucs2_len);
        GUIEDIT_SetIm(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, init_data_ptr->allow_inputmethod, init_data_ptr->cur_inputmethod);

        //due to not GUIEDIT_TYPE_DECIMAL type use GUIEDIT_TYPE_MAX instead.
        if( GUIEDIT_TYPE_MAX ==  init_data_ptr->edit_type)
        {
            GUIEDIT_SetIm(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
            GUIEDIT_SetImTag(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, GUIIM_TAG_DOT_NUM);
            GUIEDIT_ConfigImCustomKey(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 0);
            GUIEDIT_SetImTag(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, GUIIM_TAG_DOT_NUM);
        }

        if(init_data_ptr->edit_type == GUIEDIT_TYPE_PASSWORD )
        {
            if (init_data_ptr->allow_inputmethod == GUIIM_TYPE_DIGITAL)
            {
                GUIEDIT_SetPasswordStyle(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID,GUIEDIT_PASSWORD_STYLE_DIGITAL);
                GUIEDIT_SetIm(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID,GUIIM_TYPE_DIGITAL,init_data_ptr->cur_inputmethod);
            }
            else
            {
                GUIEDIT_SetPasswordStyle(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID,GUIEDIT_PASSWORD_STYLE_ALL);
                GUIEDIT_SetIm(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID,GUIIM_TYPE_MULTITAP | GUIIM_TYPE_DIGITAL | GUIIM_TYPE_ABC,init_data_ptr->cur_inputmethod);
            }
        }
        else
        {
            GUIEDIT_SetIm(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, init_data_ptr->allow_inputmethod,init_data_ptr->cur_inputmethod);
        }

        if (init_data_ptr->text) {
            GUIEDIT_SetString(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, init_data_ptr->text->wstr_ptr, (uint16)init_data_ptr->text->wstr_len);
        }

        if (init_data_ptr->title) {
            //GUIWIN_SetTitleText(win_id, init_data_ptr->title->wstr_ptr, (uint16)init_data_ptr->title->wstr_len, TRUE);
            //For UI Spec cannot have spreadtrum style title,remove it and update status bar string.
            if(MMIAPICOM_StatusAreaSetViewHeaderinfo(init_data_ptr->title->wstr_ptr, (uint16)init_data_ptr->title->wstr_len)) {
                GUIWIN_UpdateStb();
            }
        }else{
            //for null title, we need to set empty str to update status bar, if set NULL will set failed.
            if(MMIAPICOM_StatusAreaSetViewHeaderinfo(L" ", 1)) {
                GUIWIN_UpdateStb();
            }
        }

        GUIEDIT_SetHandleRedKey(FALSE, MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID);

        if (hasTicker) {
            GUILABEL_INIT_DATA_T   init_data = {0};
            MMI_CONTROL_CREATE_T    ctrl_create_label = {0};

            init_data.both_rect = both_rect;
            init_data.both_rect.h_rect.top = both_rect.h_rect.bottom - TICKER_HEIGHT;
            init_data.both_rect.v_rect.top = both_rect.v_rect.bottom - TICKER_HEIGHT;
            init_data.align = GUILABEL_ALIGN_LEFT;

            //set control param
            ctrl_create_label.ctrl_id           = MMIJAVA_IM_TEXT_EDIT_BOX_TICKER_CTRL_ID;
            ctrl_create_label.guid              = SPRD_GUI_LABEL_ID;
            ctrl_create_label.init_data_ptr     = &init_data;
            ctrl_create_label.parent_win_handle = win_id;

            //creat Label control
            MMK_CreateControl(&ctrl_create_label);

            MMK_SendMsg(MMIJAVA_IM_TEXT_EDIT_BOX_TICKER_CTRL_ID, MSG_CTL_GET_ACTIVE, PNULL);
            GUILABEL_SetBackgroundColor(MMIJAVA_IM_TEXT_EDIT_BOX_TICKER_CTRL_ID, 0xF800);
            GUILABEL_SetText(MMIJAVA_IM_TEXT_EDIT_BOX_TICKER_CTRL_ID, init_data_ptr->ticker, TRUE);
            GUILABEL_SetIsTicker(MMIJAVA_IM_TEXT_EDIT_BOX_TICKER_CTRL_ID,TRUE);
        }

        if (GUIEDIT_TYPE_NULL == init_data_ptr->edit_type) {
            GUIAPICTRL_SetState(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
        } else {
            MMK_SetAtvCtrl(win_id, MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID);
        }
#if defined(SNMP_REGION_SEA)
        if(GUIIM_LANG_VIETNAMESE != MMIAPIIM_GetInputLanguage())
#endif
        //seting edit soft key display,
        //textfiled shows OK and backspace,back;
        //textbox shows menu, and back space, cannot show OK, textbox need to handled by itself
        if(init_data_ptr->isTextBox && init_data_ptr->cmdNum>0)
        {
            GUIEDIT_SetSoftkeyEx(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID,0,0,STXT_SOFTKEY_MENU_MK,STXT_SOFTKEY_MENU_MK, PNULL, PNULL, EDIT_BUTTON_TEXT_ID,PNULL);
        }
        if(!init_data_ptr->isTextBox)
        {
            GUIEDIT_SetSoftkey(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID,1,0,STXT_SOFTKEY_OK_MK,STXT_SOFTKEY_OK_MK,PNULL);
            GUIEDIT_SetSoftkeyEx(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID,2,1,TXT_COMMON_COMMON_BACK,STR_LIST_DELETE, PNULL, PNULL, EDIT_BUTTON_TEXT_ID,PNULL);
        }
        else
        {
            GUIEDIT_SetSoftkeyEx(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID,2,1,STR_LIST_DELETE,STR_LIST_DELETE,PNULL,PNULL,EDIT_BUTTON_TEXT_ID,PNULL);
        }

        break;

#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
     case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_KEYDOWN_OK, PNULL);
                break;

            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_KEYDOWN_CANCEL, PNULL);
                break;
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT
#else
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT
#endif
    case MSG_CTL_OK:
    case MSG_KEYDOWN_OK:
        if (init_data.isTextBox && init_data.cmdNum>0){
            MMK_CreateWin((uint32 *)JBED_MMI_TEXT_EDIT_MENU_TAB, NULL);
        }
        break;

    case MSG_CTL_MIDSK:
        GUIEDIT_GetString(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, &editbox_str_info);
        if (!init_data.isTextBox)
        {
            IMCallbackProcess(MMIJAVA_IM_CALLBACK_TYPE_OK, 0, &editbox_str_info, init_data_ptr->callbackfunc);
            s_is_txt_edit_win_used = FALSE;
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_CANCEL:
        GUIEDIT_GetString(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, &editbox_str_info);
        if (!init_data.isTextBox)
        {
            IMCallbackProcess(MMIJAVA_IM_CALLBACK_TYPE_CANCEL, 0, &editbox_str_info, init_data_ptr->callbackfunc);
            s_is_txt_edit_win_used = FALSE;
            MMK_CloseWin(win_id);
        }
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
        else
        {
            IMCallbackProcess(MMIJAVA_IM_CALLBACK_TYPE_COMMAND_FIRED, editor_cancel_functionality_num, &editbox_str_info, init_data_ptr->callbackfunc);
            MMK_CloseWin(win_id);

        }
#endif
        break;

    case MSG_KEYDOWN_RED:
        //accroding to spec, red key should not be handled by IME,should map to midlet.
        if (!init_data.isTextBox)
        {
            MMK_CloseWin(win_id);
        }
        MMK_SendMsg(JBED_RUNNING_APP_WIN_ID, MSG_KEYDOWN_RED, PNULL);
        break;

    case MSG_CLOSE_WINDOW:
        if(s_is_txt_edit_win_used)
        {
            IMCallbackProcess(MMIJAVA_IM_CALLBACK_TYPE_SYS, 0, PNULL, init_data_ptr->callbackfunc);
            s_is_txt_edit_win_used = FALSE;
        }
        MMIJAVA_SetEditorTypeURL(FALSE);
        clearTextBoxInitData();
        //remove clear native text box data here because text box hide not means text box have been released
        break;
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        GUIEDIT_GetString(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, &editbox_str_info);
        IMCallbackProcess(MMIJAVA_IM_CALLBACK_TYPE_TEXT_CHANGED, 0, &editbox_str_info, init_data_ptr->callbackfunc);
        break;

    case MSG_EXIT_TEXTEDIT_WIN:
        //delay to exit to wait java screen updating.
        init_data_ptr->timer_id = MMK_CreateTimer(200, FALSE);
        break;
    case MSG_TIMER:
        if(init_data_ptr->timer_id == *(uint8*)param)
        {
            MMK_StopTimer(init_data_ptr->timer_id);
            init_data_ptr->timer_id = 0;

            MMK_CloseWin(win_id);
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

    case MSG_BACKLIGHT_TURN_ON:
        MMK_SendMsg(JBED_RUNNING_APP_WIN_ID, MSG_BACKLIGHT_TURN_ON, PNULL);
        break;

    case MSG_BACKLIGHT_TURN_OFF:
        MMK_SendMsg(JBED_RUNNING_APP_WIN_ID, MSG_BACKLIGHT_TURN_OFF, PNULL);
        if (!init_data.isTextBox)
        {
            MMK_CloseWin(JBED_IM_EDITBOX_WIN_ID);
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

void setTextBoxTitle(MMI_STRING_T * title)
{
    if (init_data.winId && MMK_IsOpenWin(JBED_IM_EDITBOX_WIN_ID)) {
        MMI_STRING_T * newTitle = copyMmiStr(title);
        if (newTitle) {
            freeMmiStr(init_data.title);
            init_data.title = newTitle;
            //GUIWIN_SetTitleText(init_data.winId, init_data.title->wstr_ptr, (uint16)init_data.title->wstr_len, TRUE);
            //update status bar instead
            if(MMIAPICOM_StatusAreaSetViewHeaderinfo(init_data.title->wstr_ptr, (uint16)init_data.title->wstr_len)) {
                GUIWIN_UpdateStb();
            }
        }
    }
}

void setTextBoxTicker(MMI_STRING_T * ticker)
{
    if (init_data.winId && MMK_IsOpenWin(JBED_IM_EDITBOX_WIN_ID)) {
        MMI_STRING_T * newTicker = copyMmiStr(ticker);
        if (newTicker) {
            freeMmiStr(init_data.ticker);
            init_data.ticker = newTicker;
        }
    }
}

void setTextBoxText(MMI_STRING_T * text)
{
    if (init_data.winId && MMK_IsOpenWin(JBED_IM_EDITBOX_WIN_ID))  {
        MMI_STRING_T * newText = copyMmiStr(text);
        if (newText) {
            freeMmiStr(init_data.text);
            init_data.text = newText;
            GUIEDIT_SetString(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, init_data.text->wstr_ptr, (uint16)init_data.text->wstr_len);
            MMK_SendMsg(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, MSG_CTL_PAINT, PNULL);
        }
    }
}

void setTextBoxMenu(int cmdNum, NativeCommand * cmdList)
{
    if (init_data.winId && MMK_IsOpenWin(JBED_IM_EDITBOX_WIN_ID)) {
        init_data.cmdNum = cmdNum;
        init_data.cmdList = cmdList;
        if (init_data.cmdNum > 0 && init_data.cmdList){
            GUIEDIT_SetSoftkeyEx(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID,0,0,STXT_SOFTKEY_MENU_MK,STXT_SOFTKEY_MENU_MK,PNULL,PNULL,EDIT_BUTTON_TEXT_ID,PNULL);
        } else {
            GUIEDIT_SetSoftkeyEx(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID,0,0,TXT_NULL,TXT_NULL,PNULL,PNULL,EDIT_BUTTON_TEXT_ID,PNULL);
        }
    }
}

void setTextBoxConstraints(GUIIM_TYPE_T constraints)
{
    if (init_data.winId && MMK_IsOpenWin(JBED_IM_EDITBOX_WIN_ID)) {
        init_data.allow_inputmethod = constraints;
        GUIEDIT_SetIm(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, init_data.allow_inputmethod, init_data.cur_inputmethod);
    }
}

void setTextBoxInputMethod(GUIIM_TYPE_T inputmethod)
{
    if (init_data.winId && MMK_IsOpenWin(JBED_IM_EDITBOX_WIN_ID)) {
        init_data.cur_inputmethod = inputmethod;
        GUIEDIT_SetIm(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, init_data.allow_inputmethod, init_data.cur_inputmethod);
    }
}

void setTextBoxMaxContentLen(int len)
{
    if (init_data.winId && MMK_IsOpenWin(JBED_IM_EDITBOX_WIN_ID) && len > 0) {
        GUIEDIT_SetTextMaxLen(MMIJAVA_IM_TEXT_EDIT_BOX_CTRL_ID, len, len);
    }
}

LOCAL void IMCallbackProcess(MMIJAVA_IM_CALLBACK_TYPE_E type,int cmdIdx,MMI_STRING_T * editbox_str_ptr,MMIJAVA_IM_CALLBACK_FUNC callbackfun)
{
    MMI_STRING_T str_info={0};
    wchar      * wstr_ptr = PNULL;

    //SCI_ASSERT(PNULL != callbackfun);/*assert verified*/
    if(PNULL == callbackfun)
        return;

    SCI_TRACE_LOW("[IMCallbackProcess]type = %d, cmdIdx = %d", type, cmdIdx);
    if(PNULL == editbox_str_ptr || PNULL == editbox_str_ptr->wstr_ptr)
    {
        (*callbackfun)(type, cmdIdx, PNULL);
    }
    else
    {
        SCI_TRACE_LOW("[IMCallbackProcess]editbox_str_ptr->wstr_len = %d", editbox_str_ptr->wstr_len);
        str_info.wstr_len  = editbox_str_ptr->wstr_len;
        wstr_ptr = SCI_ALLOCA((str_info.wstr_len + 1)<<1);
        SCI_MEMSET(wstr_ptr, 0, (str_info.wstr_len + 1)<<1);
        MMI_MEMCPY(wstr_ptr, str_info.wstr_len<<1, editbox_str_ptr->wstr_ptr, editbox_str_ptr->wstr_len<<1, editbox_str_ptr->wstr_len<<1);
        str_info.wstr_ptr = wstr_ptr;
        (*callbackfun)(type, cmdIdx, &str_info);
        SCI_FREEIF(wstr_ptr);
    }
}

PUBLIC MMIJAVA_IM_ERR_E MMIJAVA_IM_OpenTextEditWindow(
                                                 MMI_STRING_T  * title_str_ptr,        //[in] title str for editbox
                                                 MMI_STRING_T  * text_str_ptr,        //[in] text str in editbox
                                                 uint16          max_ucs2_len,       //[in] max uss2 str len
                                                 uint16          max_ascii_len,    //[in] max asscii str len
                                                 GUIIM_TYPE_T    allow_inputmethod,  //[in] allowed input modes
                                                 GUIIM_TYPE_T    cur_inputmethod, //[in] init input mode
                                                 GUIEDIT_TYPE_E  edit_type,      //[in] edit style
                                                 MMIJAVA_IM_CALLBACK_FUNC callback   //[in] callback function
                                            )
{
    return MMIJAVA_IM_OpenTextEditWindowEx(
                                         title_str_ptr,        //[in] title str for editbox
                                         text_str_ptr,        //[in] text str in editbox
                                         NULL,                   //[in] ticker
                                         max_ucs2_len,       //[in] max uss2 str len
                                         max_ascii_len,    //[in] max asscii str len
                                         allow_inputmethod,  //[in] allowed input modes
                                         cur_inputmethod, //[in] init input mode
                                         edit_type,      //[in] edit style
                                         FALSE,         // It's TextField, not TextBox
                                         0,
                                         NULL,
                                         callback   //[in] callback function
                                       );
}

PUBLIC MMIJAVA_IM_ERR_E MMIJAVA_IM_OpenTextEditWindowEx(
                                                 MMI_STRING_T  * title_str_ptr,        //[in] title str for editbox
                                                 MMI_STRING_T  * text_str_ptr,        //[in] default str in editbox
                                                 MMI_STRING_T  * ticker_str_ptr,        //[in] Ticker Str
                                                 uint16          max_ucs2_len,       //[in] max uss2 str len
                                                 uint16          max_ascii_len,    //[in] max asscii str len
                                                 GUIIM_TYPE_T    allow_inputmethod,  //[in] allowed input modes
                                                 GUIIM_TYPE_T    cur_inputmethod, //[in] init input mode
                                                 GUIEDIT_TYPE_E  edit_type,      //[in] edit style
                                                 BOOLEAN isTextBox,              //[in] If it is TextBox
                                                 int cmdNum,
                                                 NativeCommand * cmdList,
                                                 MMIJAVA_IM_CALLBACK_FUNC callback   //[in] callback function
                                            )
{
    MMIJAVA_IM_ERR_E ret_val = MMIJAVA_IM_ERR_OK;

    if(PNULL == callback || 0 == max_ucs2_len || 0 == max_ascii_len)
    {
        return MMIJAVA_IM_ERR_PARAM;
    }
    MMIAPIJAVA_NotifyCheckAndForceCloseTextEditWinFromMMI();
    //wait native input method window close,otherwise will cause second displayable free first displayable resource issue.
    do
    {
        SCI_Sleep(100);
    } while(MMK_IsOpenWin(JBED_IM_EDITBOX_WIN_ID));

    init_data.title = copyMmiStr(title_str_ptr);
    init_data.text = copyMmiStr(text_str_ptr);
    init_data.ticker = copyMmiStr(ticker_str_ptr);

    init_data.max_ascii_len = max_ascii_len;
    init_data.max_ucs2_len = max_ucs2_len;
    init_data.allow_inputmethod = allow_inputmethod;
    init_data.cur_inputmethod = cur_inputmethod;
    init_data.edit_type = edit_type;
    init_data.cmdNum = cmdNum;
    init_data.cmdList = cmdList;
    init_data.callbackfunc = callback;
    init_data.isTextBox = isTextBox;

    s_is_txt_edit_win_used = TRUE;
    MMIAPIJAVA_NotifyCreateTextEditWinFromMMI();
    return ret_val;
}

void MMIJAVA_IM_CloseTextEditWindow()
{
    if(MMK_IsOpenWin(JBED_IM_EDITBOX_WIN_ID))
    {
        SCI_TRACE_LOW("MMIJAVA_IM_CloseTextEditWindow.");
        s_is_txt_edit_win_used = FALSE;
        MMK_CloseWin(JBED_IM_EDITBOX_WIN_ID);
    }

    if(MMK_IsOpenWin(JBED_IM_EDITBOX_MENU_ID))
    {
        s_is_txt_edit_win_used = FALSE;
        MMK_CloseWin(JBED_IM_EDITBOX_MENU_ID);
    }
}

/*****************************************************************************/
//     Description : MMIJAVA_OpenTextEditWin
//    Global resource dependence :
//  Author:murphy.xie
//    Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_EnterTextEditWin(void)
{
    MMIJAVA_SetJavaIsEnablePause(FALSE);

    if(MMK_IsOpenWin(JBED_IM_EDITBOX_WIN_ID))
    {
        SCI_TRACE_LOW("MMIJAVA_OpenTextEditWindow - INFO: has already opened.");
        MMK_CloseWin(JBED_IM_EDITBOX_WIN_ID);
    }

    MMK_CreateWin((uint32 *)JBED_MMI_TEXT_EDIT_WIN_TAB, (ADD_DATA)&init_data);

}

PUBLIC void MMIJAVA_SetEditorTypeURL(BOOLEAN is_url)
{
    SCI_TRACE_LOW("[MMIJAVA_SetEditorTypeURL]is_url = %d", is_url);
    s_is_editor_url = is_url;
}

PUBLIC BOOLEAN MMIJAVA_IsEditorTypeURL(void)
{
    SCI_TRACE_LOW("[MMIJAVA_IsEditorTypeURL]s_is_editor_url = %d", s_is_editor_url);
    return s_is_editor_url;
}

 BOOLEAN  HandleMMIJavaTextEditEvent(int event)
{
    switch(event)
    {
        case MMI_NAMS_EVENT_CREATE_TEXTEDIT_WIN_FROM_MMI:
            MMIJAVA_SetTextEditorForegroundStatus(TRUE);
            MMK_CreateWin((uint32 *)JBED_MMI_TEXT_EDIT_WIN_TAB, (ADD_DATA)&init_data);
            return TRUE;
        break;
        case MMI_NAMS_EVENT_CHECK_AND_FORCE_CLOSE_TEXTEDIT_WIN_FROM_MMI:
            if(MMK_IsOpenWin(JBED_IM_EDITBOX_WIN_ID))
            {
                SCI_TRACE_LOW("MMIJAVA_OpenTextEditWindow - INFO: has already opened.");
                s_is_txt_edit_win_used = FALSE;
                MMK_CloseWin(JBED_IM_EDITBOX_WIN_ID);
                do
                {
                    SCI_Sleep(100);
                }
                while(MMK_IsOpenWin(JBED_IM_EDITBOX_WIN_ID));
            }
            return TRUE;
        break;
        case MMI_NAMS_EVENT_CLOSE_TEXTEDIT_WIN_FROM_MMI:
            MMIJAVA_IM_CloseTextEditWindow();
            //remove clear data , only close window
            return TRUE;
        break;
        default:
            return FALSE;
            break;
    }
}

/**
 *MMIJAVA_GetTextEditorForegroundStatus()
 * Get java native textbox status, foreground/background.
 * @return ture means foreground , otherwise background.
 */
PUBLIC BOOLEAN MMIJAVA_GetTextEditorForegroundStatus(void)
{
    return s_java_textbox_in_foreground;
}
/**
 *MMIJAVA_SetTextEditForegroundStatus()
 * Set java native box status.
 * @status, true means set java native textbox on foreground status
 *          false means set java native textbox on background
 */
PUBLIC void MMIJAVA_SetTextEditorForegroundStatus(BOOLEAN isForeground)
{
    s_java_textbox_in_foreground =  isForeground;
}
