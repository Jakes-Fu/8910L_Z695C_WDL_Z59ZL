/****************************************************************************
** File Name:      mmijava_backgroundwin.c
** Author:
** Date:           2012/8/15
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.
** Description:    This file is used to describe functions of permission set
****************************************************************************/
#include "jbed_exported_defines.h"
#include "mmk_type.h"
#include "window_parse.h"
#include "mmijava_id.h"
#include "mmiset_id.h"
#include "mmidisplay_data.h"
#include "guitext.h"
#include "java_mmi_text.h"
#include "cpl_tpal.h"
#include "opl_global.h"
#include "jbed_main.h"
#include "mmipub.h"
#include "mmijava_backgroundwin.h"
#include "mmijava_runwin.h"
#include "mmijava_common.h"

#ifdef SUPPORT_BACKGROUND_RUN
/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
typedef enum
{
    REDKEY_CHOISE_MINIMIZE_TYPE,   //VM run in background
    REDKEY_CHOISE_TERMINATE_TYPE,  //Terminate current midlet
    REDKEY_CHOISE_PAUSE_TYPE,      //Pause current midlet.
    REDKEY_CHOISE_RETURN_TYPE      //Do nothing just close redkey window and return.
} REDKEY_CHOISE_TYPE;

#define  MMIJAVA_MARGIN_TEXT_LEFT_RIGHT   6
#define  MMIJAVA_MARGIN_TEXT_TOP_BOTTOM   6

/**************************************************************************************************/
// Description:Handle redkey window message.
// Date:2011-08-27
// Author:Martin.Zhao
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaRedkeyDialgWinMsg(
                                                 MMI_WIN_ID_T        win_id,
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 );
#if defined(JAVA_SUPPORT_TYPE_SVM) && defined(SUPPORT_BACKGROUND_RUN)
LOCAL MMI_RESULT_E Jbed_HandleResumeBackgroundWinMsg(
                                                 MMI_WIN_ID_T        win_id,
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 );
#endif

/**************************************************************************************************/
// Description:Red key window table.
// Date:2011-08-27
// Author:Martin.Zhao
// Note:
//*************************************************************************************************/
WINDOW_TABLE( JBED_REDKEY_DIALOG_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleJavaRedkeyDialgWinMsg),
    WIN_ID( JBED_REDKEY_DIALOG_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIJAVA_REDKEY_DIALOG_LISTBOX_CTRL_ID),
    END_WIN
};
#if defined(JAVA_SUPPORT_TYPE_SVM) && defined(SUPPORT_BACKGROUND_RUN)
/**************************************************************************************************/
// Description: launch check win
// Note:
//*************************************************************************************************/
WINDOW_TABLE( JBED_RESUME_BACKGROUND_WIN_TAB ) =
{
    WIN_FUNC( (uint32)Jbed_HandleResumeBackgroundWinMsg),
    WIN_ID( JBED_RESUME_BACKGROUND_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIJAVA_RESUME_BACKGROUND_FATHER_FORM_CRTL_ID),
    // Query text.
    //CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIJAVA_RESUME_BACKGROUND_NOTE_FORM_CRTL_ID, MMIJAVA_RESUME_BACKGROUND_FATHER_FORM_CRTL_ID),
    CHILD_TEXT_CTRL(FALSE, MMIJAVA_RESUME_BACKGROUND_NOTE_TEXT_CTRL_ID, MMIJAVA_RESUME_BACKGROUND_FATHER_FORM_CRTL_ID),
    // Selection list.
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIJAVA_RESUME_BACKGROUND_LISTBOX_CTRL_ID, MMIJAVA_RESUME_BACKGROUND_FATHER_FORM_CRTL_ID),
    END_WIN
};
#endif

//return - true(trusted)/flase(untrusted)
LOCAL BOOLEAN CheckMidletTrustDomain(int suite_id)
{
    BOOLEAN is_trust   = FALSE;
    int32_t size       = 0;
    wchar * domain_str = NULL;

    size = Jbed_nAms_querySuitesInfo(suite_id,NAMS_SI_DOMAIN,(void*)&domain_str);
    if(size >0)
    {
        //see in Domain.java,others are trusted except "untrusted" and "midp1"
        if((0 !=CRTL_wstrncmp(domain_str,L"untrusted",size)) && (0 != CRTL_wstrncmp(domain_str,L"midp1",size)))
        {
            is_trust = TRUE;
        }
    }
    SCI_FREEIF(domain_str);

    return is_trust;
}

/**************************************************************************************************/
// Description:Create redkey dialog list items.
// Date:2011-08-27
// Author:Martin.Zhao
// Note:
//*************************************************************************************************/
LOCAL void CreateRedKeyDialogListBox(void)
{
    if(g_active_launching_app == PNULL)
    {
        SCI_TRACE_LOW("===Warning: in CreateRedKeyDialogListBox,g_active_launching_app is NULL==");
        //no active app return directly
        return;
    }

    if(CheckMidletTrustDomain(g_active_launching_app->sid))  /*min/pause/exit*/
    {
        //Run in backgroud
        MMIAPISET_AppendListItemByTextIdExt(
            STR_JAVA_MINIMIZE,
            TXT_NULL,
            STXT_SOFTKEY_SELECT_MK,
            TXT_COMMON_COMMON_BACK,
            MMIJAVA_REDKEY_DIALOG_LISTBOX_CTRL_ID,
            GUIITEM_STYLE_ONE_LINE_TEXT_MS);
        //Pause VM
        MMIAPISET_AppendListItemByTextIdExt(
            STR_JAVA_PAUSE,
            TXT_NULL,
            STXT_SOFTKEY_SELECT_MK,
            TXT_COMMON_COMMON_BACK,
            MMIJAVA_REDKEY_DIALOG_LISTBOX_CTRL_ID,
            GUIITEM_STYLE_ONE_LINE_TEXT_MS);
        //Terminate midlet
        MMIAPISET_AppendListItemByTextIdExt(
            STR_JAVA_EXIT,
            TXT_NULL,
            STXT_SOFTKEY_SELECT_MK,
            TXT_COMMON_COMMON_BACK,
            MMIJAVA_REDKEY_DIALOG_LISTBOX_CTRL_ID,
            GUIITEM_STYLE_ONE_LINE_TEXT_MS);
    }
    else    /*pause/exit*/
    {
        //Pause VM
        MMIAPISET_AppendListItemByTextIdExt(
            STR_JAVA_PAUSE,
            TXT_NULL,
            STXT_SOFTKEY_SELECT_MK,
            TXT_COMMON_COMMON_BACK,
            MMIJAVA_REDKEY_DIALOG_LISTBOX_CTRL_ID,
            GUIITEM_STYLE_ONE_LINE_TEXT_MS);
        //Terminate midlet
        MMIAPISET_AppendListItemByTextIdExt(
            STR_JAVA_EXIT,
            TXT_NULL,
            STXT_SOFTKEY_SELECT_MK,
            TXT_COMMON_COMMON_BACK,
            MMIJAVA_REDKEY_DIALOG_LISTBOX_CTRL_ID,
            GUIITEM_STYLE_ONE_LINE_TEXT_MS);
    }
    //Update listbox
#ifndef MMI_PDA_SUPPORT
    GUILIST_SetCurItemIndex(MMIJAVA_REDKEY_DIALOG_LISTBOX_CTRL_ID, 0);
    GUILIST_SetSelectedItem(MMIJAVA_REDKEY_DIALOG_LISTBOX_CTRL_ID, 0, TRUE);
#endif
}

/**************************************************************************************************/
// Description:Close windows when exit redkey window.
// Date:2011-08-27
// Author:Martin.Zhao
// Note:
//*************************************************************************************************/
LOCAL void CloseWindows(void)
{
    SCI_TRACE_LOW("CloseWindows ENTER");
    MMIJAVA_CloseRunningWin();
    MMK_CloseWin(JBED_REDKEY_DIALOG_WIN_ID);
#ifdef MMI_PDA_SUPPORT
    MMK_CloseWin(JBED_APP_WIN_ID);
#endif
    MMK_CloseWin(MMIJAVA_AMS_WIN_ID);
    MMK_CloseWin(MMIJAVA_AMS_MIDLETLIST_WIN_ID);
    MMK_CloseWin(JBED_LOCAL_INSTALL_MENU_WIN_ID);
    MMK_CloseWin(JBED_LOCAL_INSTALL_WIN_ID);
#if 0 //UNISOC_MMI_Delete
    MMK_CloseWin(MAIN_MAINMENU_WIN_ID);
#endif
}

/**************************************************************************************************/
// Description:Process user choise on redkey dialog.
// Date:2011-08-27
// Author:Martin.Zhao
// Note:
//*************************************************************************************************/
LOCAL void OnRedkeyChoise(REDKEY_CHOISE_TYPE choise_type)
{
    MMI_STRING_T prompt = {0};
    SCI_TRACE_LOW("OnRedkeyChoise, choise_type = %d", choise_type);
    switch (choise_type)
    {
        case REDKEY_CHOISE_MINIMIZE_TYPE:
        {
    #ifdef MMI_PAUSE_IN_BACKGROUND_AS_DEFAULT
            if (NULL != g_active_launching_app)
            {
                Jbed_nAms_noPauseInBackground(g_active_launching_app->sid, g_active_launching_app->mid);
            }
            Jbed_nAms_gotoBackground();
    #endif
            Jbed_nAms_requestVmToState(STATE_ACTIVE);
            //CloseWindows();
            MMK_HideWindow(JBED_RUNNING_APP_WIN_ID, TRUE);
            MMK_CloseWin(JBED_REDKEY_DIALOG_WIN_ID);
            MMK_CloseWin(MMIJAVA_AMS_MIDLETLIST_WIN_ID);
            // We need back to mainmenu if the midlet is opened at "Installed apps".
            if(MMK_IsOpenWin(MMIJAVA_AMS_WIN_ID))
            {
                MMK_CloseWin(MMIJAVA_AMS_WIN_ID);
            }
            // Close Settings window, need to change the win id
            // MMISET_SET_PHONE_WIN_ID as Settings window on their side.
            if(MMK_IsOpenWin(MMISET_SET_PHONE_WIN_ID))
            {
                MMK_CloseWin(MMISET_SET_PHONE_WIN_ID);
            }
        }
        break;

        case REDKEY_CHOISE_PAUSE_TYPE:
        {
        #ifdef JAVA_SUPPORT_TYPE_MVM
            if (NULL != g_active_launching_app)
            {
                Jbed_nAms_Pause(g_active_launching_app->sid, g_active_launching_app->mid);
            }
            Jbed_nAms_requestVmToState(STATE_ACTIVE);
        #else
            Jbed_nAms_requestVmToState(STATE_IDLE);
        #endif
            //CloseWindows();
            MMK_HideWindow(JBED_RUNNING_APP_WIN_ID, TRUE);
            MMK_CloseWin(JBED_REDKEY_DIALOG_WIN_ID);
            MMK_CloseWin(MMIJAVA_AMS_MIDLETLIST_WIN_ID);
            // We need back to mainmenu if the midlet is opened at "Installed apps".
            if(MMK_IsOpenWin(MMIJAVA_AMS_WIN_ID))
            {
                MMK_CloseWin(MMIJAVA_AMS_WIN_ID);
            }
            // Close Settings window, need to change the win id
            // MMISET_SET_PHONE_WIN_ID as Settings window on their side.
            if(MMK_IsOpenWin(MMISET_SET_PHONE_WIN_ID))
            {
                MMK_CloseWin(MMISET_SET_PHONE_WIN_ID);
            }
        }
        break;

        case REDKEY_CHOISE_TERMINATE_TYPE:
        {
            MMI_GetLabelTextByLang(STR_JAVA_CLOSING, &prompt);
            if (NULL != g_active_launching_app)
            {
                SCI_TRACE_LOW("===ready to open wait win,test 6==");
                MMIJAVA_util_openWaitingWin(&prompt);
                Jbed_nAms_destroy(g_active_launching_app->sid, g_active_launching_app->mid);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(STR_JAVA_VM_BUSY);
                SCI_TRACE_LOW("===exit by request down==");
                Jbed_requestState(JBED_MAIN_DOWN, NULL, NULL);
                Jbed_wakeupVM();
            }

            SCI_TRACE_LOW("=== Myriad OnRedkeyChoise call Jbed_createJbedDownTimer ==");
            Jbed_createJbedDownTimer();

            CloseWindows();
        }
        break;

        case REDKEY_CHOISE_RETURN_TYPE:
        {
            MMK_CloseWin(JBED_REDKEY_DIALOG_WIN_ID);
        }
        break;

        default:
            SCI_TRACE_LOW("OnRedkeyChoise, Error choise_type = %d", choise_type);
            break;
        }
}

/**************************************************************************************************/
// Description:Handle redkey window message.
// Date:2011-08-27
// Author:Martin.Zhao
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaRedkeyDialgWinMsg(
                                                 MMI_WIN_ID_T        win_id,
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 )
{
    MMI_RESULT_E  result  = MMI_RESULT_TRUE;
    uint16        action_item_tid = 0;
    static  BOOLEAN b_terminal_flag = FALSE;

    SCI_TRACE_LOW("HandleJavaRedkeyDialgWinMsg, msg_id = 0x%x", msg_id);
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T title_str;

            // update title on status bar.
            MMI_GetLabelTextByLang(STR_JAVA_SELECT, &title_str);
            if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str.wstr_ptr, title_str.wstr_len))
            {
                GUIWIN_UpdateStb();
            }

            b_terminal_flag = FALSE;
            GUILIST_SetMaxItem(MMIJAVA_REDKEY_DIALOG_LISTBOX_CTRL_ID, 3, FALSE);
            GUILIST_SetListState(MMIJAVA_REDKEY_DIALOG_LISTBOX_CTRL_ID, GUILIST_STATE_SPLIT_LINE, FALSE);
            CreateRedKeyDialogListBox();
            MMK_SetAtvCtrl(win_id,MMIJAVA_REDKEY_DIALOG_LISTBOX_CTRL_ID);
            break;
        }
        case MSG_FULL_PAINT:
            break;

        case MSG_CTL_MIDSK:
        case MSG_KEYDOWN_WEB:
        case MSG_CTL_OK:
        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            action_item_tid = GUILIST_GetCurItemIndex(MMIJAVA_REDKEY_DIALOG_LISTBOX_CTRL_ID);
            SCI_TRACE_LOW("HandleJavaRedkeyDialgWinMsg GUILIST_GetCurItemIndex %d", action_item_tid);
            if (NULL == g_active_launching_app)
            {
                SCI_TRACE_LOW("===Warning: in HandleJavaRedkeyDialgWinMsg,g_active_launching_app is null!");
                break;
            }
            if(CheckMidletTrustDomain(g_active_launching_app->sid))
            {
                if(action_item_tid == 0)
                {
                    OnRedkeyChoise(REDKEY_CHOISE_MINIMIZE_TYPE);
                #if defined(MMI_TASK_MANAGER)
                    MMIAPIJAVA_StartApplet();
                #endif
                }
                else if(action_item_tid == 1)
                {
                    OnRedkeyChoise(REDKEY_CHOISE_PAUSE_TYPE);
                #if defined(MMI_TASK_MANAGER)
                    MMIAPIJAVA_StartApplet();
                #endif
                }
                else
                {
                    b_terminal_flag = TRUE;
                    OnRedkeyChoise(REDKEY_CHOISE_TERMINATE_TYPE);
                }
            }
            else
            {
                if(action_item_tid == 0)
                {
                    OnRedkeyChoise(REDKEY_CHOISE_PAUSE_TYPE);
                #if defined(MMI_TASK_MANAGER)
                    MMIAPIJAVA_StartApplet();
                #endif
                }
                else
                {
                    b_terminal_flag = TRUE;
                    OnRedkeyChoise(REDKEY_CHOISE_TERMINATE_TYPE);
                }
            }
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            OnRedkeyChoise(REDKEY_CHOISE_RETURN_TYPE);
            break;

        case MSG_KEYDOWN_RED:
            b_terminal_flag = TRUE;
            OnRedkeyChoise(REDKEY_CHOISE_TERMINATE_TYPE);
            break;

        case MSG_KEYUP_RED:
            break;

        case MSG_LOSE_FOCUS:
            //FIXME: When alert info comes, do we need to pause running applet
            //           and send vm to background?
        #ifdef JAVA_SUPPORT_TYPE_MVM
            //new code here if neened
        #else
            if(!b_terminal_flag)
            {
                Jbed_vmWantToIdle();
            }
        #endif
            break;

        case MSG_GET_FOCUS:
            //FIXME: When alert info leaves, need to restore
        #ifdef JAVA_SUPPORT_TYPE_MVM
            //new code here if needed
        #else
            Jbed_vmWantForeground(TRUE);
        #endif
            break;

        case MSG_CLOSE_WINDOW:
            b_terminal_flag = FALSE;
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return (result);
}

LOCAL MMI_RESULT_E Jbed_HandleResumeBackgroundWinMsg(
                                                 MMI_WIN_ID_T        win_id,
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 )
{
    MMI_RESULT_E  result  = MMI_RESULT_TRUE;
    uint16        action_item_tid = 0;

    //SCI_TRACE_LOW("===Jbed_HandleLaunchCheckWinMsg, msg_id = 0x%x", msg_id);
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T title_str;
            MMI_STRING_T note_ptr = {0};
            GUI_BG_T bg = {0};

            // update title on status bar.
            MMI_GetLabelTextByLang(STR_JAVA_SELECT, &title_str);
            if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str.wstr_ptr, title_str.wstr_len))
            {
                GUIWIN_UpdateStb();
            }

            // set form background color.
            bg.bg_type = GUI_BG_COLOR;
            bg.color = MMI_BLACK_COLOR;
            GUIFORM_SetBg(MMIJAVA_RESUME_BACKGROUND_FATHER_FORM_CRTL_ID, &bg);

            // set query text.
            if (MMIJAVA_GetReplaceText(STR_JAVA_BACKGROUND_QUERY, g_active_launching_app->sid, &note_ptr))
            {
                GUI_FONT_T    font_ptr = MMI_DEFAULT_NORMAL_FONT;
                GUI_COLOR_T   font_color = MMI_WHITE_COLOR;

                GUIFORM_SetStyle(MMIJAVA_RESUME_BACKGROUND_FATHER_FORM_CRTL_ID, GUIFORM_STYLE_NORMAL);
                GUIFORM_PermitChildBorder(MMIJAVA_RESUME_BACKGROUND_FATHER_FORM_CRTL_ID, FALSE);
                GUIFORM_PermitChildFont(MMIJAVA_RESUME_BACKGROUND_FATHER_FORM_CRTL_ID, FALSE);
                GUITEXT_SetFont(MMIJAVA_RESUME_BACKGROUND_NOTE_TEXT_CTRL_ID, &font_ptr, &font_color);
                GUITEXT_SetMarginEx(MMIJAVA_RESUME_BACKGROUND_NOTE_TEXT_CTRL_ID,
                                    MMIJAVA_MARGIN_TEXT_LEFT_RIGHT,
                                    MMIJAVA_MARGIN_TEXT_LEFT_RIGHT,
                                    MMIJAVA_MARGIN_TEXT_TOP_BOTTOM,
                                    MMIJAVA_MARGIN_TEXT_TOP_BOTTOM);
                GUITEXT_IsDisplayPrg(FALSE, MMIJAVA_RESUME_BACKGROUND_NOTE_TEXT_CTRL_ID);
                GUITEXT_SetString(MMIJAVA_RESUME_BACKGROUND_NOTE_TEXT_CTRL_ID, note_ptr.wstr_ptr, note_ptr.wstr_len, FALSE);
                SCI_FREEIF(note_ptr.wstr_ptr);
            }

            GUILIST_SetMaxItem(MMIJAVA_RESUME_BACKGROUND_LISTBOX_CTRL_ID, 2, FALSE);
            GUILIST_SetListState(MMIJAVA_RESUME_BACKGROUND_LISTBOX_CTRL_ID, GUILIST_STATE_SPLIT_LINE, FALSE);
            //resume background
            MMIAPISET_AppendListItemByTextIdExt(
                STR_JAVA_RESUME,
                TXT_NULL,
                STXT_SOFTKEY_SELECT_MK,
                TXT_COMMON_COMMON_BACK,
                MMIJAVA_RESUME_BACKGROUND_LISTBOX_CTRL_ID,
                GUIITEM_STYLE_ONE_LINE_TEXT_MS);
            //exit background
            MMIAPISET_AppendListItemByTextIdExt(
                STR_JAVA_EXIT,
                TXT_NULL,
                STXT_SOFTKEY_SELECT_MK,
                TXT_COMMON_COMMON_BACK,
                MMIJAVA_RESUME_BACKGROUND_LISTBOX_CTRL_ID,
                GUIITEM_STYLE_ONE_LINE_TEXT_MS);
            //Update listbox
            GUILIST_SetCurItemIndex(MMIJAVA_RESUME_BACKGROUND_LISTBOX_CTRL_ID, 0);
            MMK_SetAtvCtrl(win_id, MMIJAVA_RESUME_BACKGROUND_LISTBOX_CTRL_ID);
            break;
        }

        case MSG_FULL_PAINT:
            break;

        case MSG_CTL_MIDSK:
        case MSG_KEYDOWN_WEB:
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_CTL_PENOK:
            action_item_tid = GUILIST_GetCurItemIndex(MMIJAVA_RESUME_BACKGROUND_LISTBOX_CTRL_ID);
            //SCI_TRACE_LOW("HandleJavaRedkeyDialgWinMsg GUILIST_GetCurItemIndex %d", action_item_tid);
            if(action_item_tid == 0)
            {
                MMIJAVA_CreateRunningWin();
                MMK_CloseWin(win_id);//CR136218
                Jbed_vmWantForeground(TRUE);
                SCI_TRACE_LOW("=== recovery from background ==");
            }
            else
            {
                MMI_STRING_T prompt = {0};
                MMI_GetLabelTextByLang(STR_JAVA_CLOSING, &prompt);
                MMK_CloseWin(win_id);
                SCI_TRACE_LOW("=== exit from background ==");

                MMIJAVA_util_openWaitingWin(&prompt);
                SCI_TRACE_LOW("===exit by request down==");
                Jbed_requestState(JBED_MAIN_DOWN, NULL, NULL);
                Jbed_wakeupVM();
                SCI_TRACE_LOW("=== Myriad HandleResumeBackgroundWinMsg call Jbed_createJbedDownTimer ==");
                Jbed_createJbedDownTimer();
            }
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;

        case MSG_KEYDOWN_RED:
            break;

        case MSG_LOSE_FOCUS:
            break;

        case MSG_GET_FOCUS:
            break;

        case MSG_CLOSE_WINDOW:
            MMIJAVA_SetJavaIsEnablePause(TRUE);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return (result);
}

/**************************************************************************************************/
// Description: Interface for enter red key dialog when press red key.
// Date:2011-09-01
// Author:Martin.Zhao
// Note:
//*************************************************************************************************/
PUBLIC void MMIJAVA_OpenRedkeyDialog(void)
{
    if (PNULL != g_active_launching_app)
    {
        SCI_TRACE_LOW("===Red key:open red key handle-win");
        MMIJAVA_updateRunningWinState(STATE_LOSE_FOCUS, TRUE);
        SCI_SLEEP(150);
        MMIJAVA_SetJavaIsEnablePause(FALSE);
        MMK_CreateWin((uint32*)JBED_REDKEY_DIALOG_WIN_TAB, PNULL);
    }
    else
    {
        SCI_TRACE_LOW("===open red key win fail,anything's wrong?");
        MMIJAVA_NotifyJVMExit();
    }
}


/**************************************************************************************************/
// Description: Interface for enter red key dialog when press red key.
// Date:2011-09-01
// Author:Martin.Zhao
// Note:
//*************************************************************************************************/
PUBLIC void MMIJAVA_EnterResumeWin(void)
{
    MMK_CreateWin((uint32*)JBED_RESUME_BACKGROUND_WIN_TAB, PNULL);
}

#endif
