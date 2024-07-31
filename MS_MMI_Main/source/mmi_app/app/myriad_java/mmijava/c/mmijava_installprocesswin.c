/**
 * Copyright 2011-2015 Myriad Group AG. All Rights Reserved.
 * $Id: //depot/dev/project/spreadtrum/6530_new/export/src/mmijava_installprocesswin.c#10 $
 */

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

//#include "guitext.h"
#include "mmk_type.h"
#include "window_parse.h"
#include "jbed_exported_defines.h"
#include "mmijava_id.h"
//#include <mmicc_export.h>
#include "mmi_appmsg.h"
#include <mmi_common.h>
//#include <mmijava_menutable.h>
//#include <mmidisplay_data.h>
#include "mmipub.h"
#include "guitext.h"
//#include <cpl_tpal.h>
//#include <opl_global.h>
//#include <mmijava_common.h>
//#include <cpl_file.h>
//#include <guilabel.h>
//#include <mmijava_export.h>
//#include <java_mmi_text.h>
#include "jbed_main.h"

#include "mmijava_suitefun.h"
#include "mmiudisk_export.h"
#include "mmijava_backgroundwin.h"
#include "mmijava_dynamicmenufun.h"
#include "mmijava_runwin.h"
#include "mmijava_installprocesswin.h"
#include "cpl_midletinfo.h"
#ifdef DYNAMIC_MAINMENU_SUPPORT
#include "mmi_mainmenu_matrix.h"
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Declaration                   *
 **---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                          EXTERN DECLARE                                  *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//LOCAL uint32 s_create_event_id = 0;
//LOCAL uint32 s_create_win_tab  = 0;
LOCAL uint32 s_current_id = 0;
LOCAL BOOLEAN s_can_stop = FALSE;
LOCAL BOOLEAN s_be_interruptted = FALSE;

LOCAL BOOLEAN g_is_install_in_progress = FALSE;

/**
  * TRUE means OTA install, install an MIDlet from OTA server.
  * Otherwise, it's an local installation, install an MIDlet from local F/S.
  */
LOCAL BOOLEAN s_is_ota_install = FALSE;
#ifdef DYNAMIC_MAINMENU_SUPPORT
LOCAL BOOLEAN s_is_update_midlet = FALSE;
LOCAL int32 s_recalculate_progress = 0;
#endif

#define MMIJAVA_PREINSTALL    1
#define MMIJAVA_COMMONINSTALL  2

typedef struct
{
    BOOLEAN is_pending;
    int32 install_type;
    int32 suite_id;
} MMIJAVA_INSTALL_PENDING_EVENT;

LOCAL MMIJAVA_INSTALL_PENDING_EVENT install_pending_event = {0};


/*****************************************************************************/
//  Description :handle installing infomation
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Jbed_HandleInstallInfoMenuWinMsg(
                                                         MMI_WIN_ID_T     win_id,
                                                         MMI_MESSAGE_ID_E msg_id,
                                                         DPARAM           param
                                                    );

#if defined(SUPPORT_SELECT_STORAGE) && !defined(AMS_TCK_RUNNER)
/*****************************************************************************/
//  Description : to append one item to listbox by text buffer and set soft keys
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void AppendListItemByTextBuffer(
                                      const wchar *        text_ptr,
                                      uint16               text_len,
                                      uint32               user_data,
                                      MMI_TEXT_ID_T        left_softkey_id,
                                      MMI_TEXT_ID_T        middle_softkey_id,
                                      MMI_TEXT_ID_T        right_softkey_id,
                                      MMI_CTRL_ID_T        ctrl_id,
                                      GUIITEM_STYLE_E      item_style
                                      );

/*****************************************************************************/
//  Description :select avaliabel storage to store midlet
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Jbed_HandleStorageSelectWinMsg(
                                                   MMI_WIN_ID_T     win_id,
                                                   MMI_MESSAGE_ID_E msg_id,
                                                   DPARAM           param
                                                );
#endif
#ifdef SUPPORT_LANCH_AFTER_INSTALL
/*****************************************************************************/
//  Description : launch midlet query window message handle
//  Global resource dependence :
//  Author:Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E JavaLaunchMidletQueryCallback
                                        (
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                        );
#endif


/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
#if defined(SUPPORT_SELECT_STORAGE) && !defined(AMS_TCK_RUNNER)
WINDOW_TABLE(JBED_STORAGE_SELECT_WIN_TAB) =
{
    WIN_FUNC((uint32)Jbed_HandleStorageSelectWinMsg),
    WIN_ID(JBED_STORAGE_SELECT_WIN_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIJAVA_STORAGE_SELECT_CTRL_ID),
    END_WIN
};
#endif

LOCAL MMI_RESULT_E  MMIJAVA_HandleInstallAlertWinMsg(
                                             MMI_WIN_ID_T       win_id,
                                             MMI_MESSAGE_ID_E   msg_id,
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetTextDialogBlankTitle(win_id, TRUE);
        break;
    //case MSG_APP_CANCEL:
    case MSG_NOTIFY_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : handle progressbar event
//  Global resource dependence : none
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIJAVA_HandleProcessWinMsg(
                                         MMI_WIN_ID_T        win_id,
                                         MMI_MESSAGE_ID_E    msg_id,
                                         DPARAM              param
                                     )
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_be_interruptted = FALSE;
        s_can_stop = FALSE;    //when open install info window,set s_can_stop = TRUE;
        //MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    case MSG_LCD_SWITCH:
        GUIWIN_SetSoftkeyTextId(win_id, COMMON_TXT_NULL, COMMON_TXT_NULL, STXT_CANCEL, FALSE);
        s_can_stop = TRUE;
        break;

    /*-------just for test --------
    case MSG_KEYUP_CAMERA:
    case MSG_KEYDOWN_CAMERA:
        result = MMI_RESULT_TRUE;
        break;
    ----------------------------*/
    case MSG_KEYUP_RED:
        break;

    case MSG_KEYLONG_POWER:
    /*case MSG_KEYLONG_CAMREA:    */
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        if (s_can_stop)
        {
            MMI_STRING_T prompt = {0};
            MMI_GetLabelTextByLang(TXT_COMMON_CANCELING, &prompt);
            MMIJAVA_util_openWaitingWin(&prompt);

            //MMK_CloseWin(win_id);
            Jbed_nAms_installOperation(MMI_NAMS_EVENT_INSTALL_QUERY, RES_CANCEL, NULL);
        }
        break;
    case MSG_CLOSE_WINDOW:
        //s_create_event_id = 0;
        //MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        if (Jbed_tm_getRunningAppNum() == 0)
        {
            g_is_install_in_progress = FALSE;
            MMIJAVA_NotifyJVMExit();
        }
        break;

    default:
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    }
    return (result);
}


#ifdef SUPPORT_LANCH_AFTER_INSTALL
/*****************************************************************************/
//  Description : launch midlet query window message handle
//  Global resource dependence :
//  Author:Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E JavaLaunchMidletQueryCallback(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                        )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMIPUB_SetTextDialogBlankTitle(win_id, TRUE);
            break;
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
            MMK_CloseWin(win_id);
            /*
            Interface of launch midlet after installed
            */
            MMIJAVA_EnvironmentInit();
            MMIJAVA_StartSuiteInternal(g_installed_suiteId,-1);
            g_installed_suiteId = -1;
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
            break;

        default:
            result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
            break;
    }
    return result;
}

#endif

/*****************************************************************************/
//  Description :handle installing infomation
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Jbed_HandleInstallInfoMenuWinMsg(
                                                MMI_WIN_ID_T     win_id,
                                                MMI_MESSAGE_ID_E msg_id,
                                                DPARAM           param
                                            )
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIJAVA_INSTALLING_INFO_CRTL_ID;
    MMI_HANDLE_T ctrl_handle = MMI_INVALID_ID;
    MMIEvent * mmi_evt = NULL;
    //int * cur_evt_id = MMK_GetWinAddDataPtr(win_id);
    mmi_evt = MMIJAVA_find_MMIEvent(s_current_id);
    if(NULL == mmi_evt)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (mmi_evt->event == MMI_NAMS_EVENT_INSTALL_PREVIEW_JAD)
        {
            // set dialog title for preview jad.
            MMI_STRING_T title_str = {0};
            MMI_GetLabelTextByLang(STR_JAVA_WARNING_EXT01, &title_str);
            MMIPUB_SetTextDialogTitle(win_id, &title_str, TRUE);
        }
        else
        {
            MMIPUB_SetTextDialogBlankTitle(win_id, TRUE);
        }
        ctrl_handle = MMIPUB_GetDialogTextCtrlId(win_id);
        GUITEXT_SetAlign(ctrl_handle, ALIGN_LEFT);
        s_can_stop = TRUE;
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    //case MSG_APP_OK:
    //case MSG_CTL_OK:
    case MSG_APP_WEB:
        if (mmi_evt->event == MMI_NAMS_EVENT_INSTALL_QUERY_YES_NO)
        {
            Jbed_nAms_installOperation(mmi_evt->event, RES_YES, NULL);
            MMK_CloseWin(win_id);
        }
        else
        {
            Jbed_nAms_installOperation(mmi_evt->event, RES_CONTINUE, NULL);
            MMK_CloseWin(win_id);
            MMIMAINMENU_ReturnMainmenuByJava();
        }
        break;

    /* --- just for test ---
    case MSG_KEYUP_CAMERA:
    case MSG_KEYDOWN_CAMERA:
        result = MMI_RESULT_TRUE;
        break;
    case MSG_KEYLONG_CAMREA:
    -----------------------*/
    case MSG_KEYLONG_POWER:    //power -- cancel install
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_RED:
        if (mmi_evt->event == MMI_NAMS_EVENT_INSTALL)
        {
            MMK_CloseWin(JBED_INSTALLING_MENU_WIN_ID);
        }
        else if (mmi_evt->event == MMI_NAMS_EVENT_INSTALL_QUERY_YES_NO)
        {
            Jbed_nAms_installOperation(mmi_evt->event, RES_NO, NULL);
        }
        else
        {
            MMI_STRING_T prompt = {0};

            MMI_GetLabelTextByLang(TXT_COMMON_CANCELING, &prompt);
            Jbed_nAms_installOperation(mmi_evt->event, RES_CANCEL, NULL);
            MMK_CloseWin(win_id);
            if(MMK_IsOpenWin(MMIJAVA_PROGRESS_WIN_ID))
            {
                MMK_CloseWin(MMIJAVA_PROGRESS_WIN_ID);
            }
            MMIJAVA_util_openWaitingWin(&prompt);
            break;
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_KEYUP_RED:
        //ignore
        break;

    case MSG_CLOSE_WINDOW:
        MMIJAVA_util_deallocMMIEvent_byEventID(mmi_evt->event_id);
#if defined(MMIJAVA_DEUBG)
        //MMIJava_print_MMIEventList();
#endif
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
        break;
    }

    //to be implemented
    return result;
}
#if defined(SUPPORT_SELECT_STORAGE) && !defined(AMS_TCK_RUNNER)
/*****************************************************************************/
//  Description : to append one item to listbox by text buffer and set soft keys
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void AppendListItemByTextBuffer(
                                      const wchar *        text_ptr,
                                      uint16               text_len,
                                      uint32               user_data,
                                      MMI_TEXT_ID_T        left_softkey_id,
                                      MMI_TEXT_ID_T        middle_softkey_id,
                                      MMI_TEXT_ID_T        right_softkey_id,
                                      MMI_CTRL_ID_T        ctrl_id,
                                      GUIITEM_STYLE_E      item_style
                                      )
{
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/

    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = text_ptr;/*lint !e605*/
    item_data.item_content[0].item_data.text_buffer.wstr_len = text_len;

    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;

    item_t.user_data = user_data;

    GUILIST_AppendItem( ctrl_id, &item_t );
}

/*****************************************************************************/
//  Description :select avaliabel storage to store midlet
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Jbed_HandleStorageSelectWinMsg(
                                              MMI_WIN_ID_T     win_id,
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM           param
                                          )
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIJAVA_STORAGE_SELECT_CTRL_ID;
    MMIEvent * mmi_evt = NULL;
    int * cur_evt_id = MMK_GetWinAddDataPtr(win_id);
    mmi_evt = MMIJAVA_find_MMIEvent(*cur_evt_id);
    if(NULL == mmi_evt)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        if (MMI_NAMS_EVENT_SELECT_STORAGE == mmi_evt->event)
        {
            int32  i;
            uint16 item_num = 0;
            wchar ** str_arr = (wchar **)mmi_evt->data;
            GUILIST_ITEM_T      item_t      = {0};
            GUILIST_ITEM_DATA_T item_data   = {0};
            MMI_STRING_T  txt_string[5] = {0};
            MMI_STRING_T title_str = {0};

            MMI_GetLabelTextByLang(STR_HDR_SELECT_MEMORY_EXT01, &title_str);
            if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str.wstr_ptr, title_str.wstr_len))
            {
                GUIWIN_UpdateStb();
            }

            CRTL_memset(txt_string,0,sizeof(MMI_STRING_T)*5);
            item_num = (uint16)mmi_evt->data_bytes;
            GUILIST_SetMaxItem(ctrl_id, item_num, FALSE);
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);

            for(i = 0; i < item_num; i++)
            {
                //[2*i+1] must be english name of dick by wide char
                if(0==CRTL_wstrncmp(str_arr[2*i+1],L"System",CRTL_wstrlen(L"System")))
                {
                    MMI_GetLabelTextByLang(TXT_JAVA_INSTALL_SELECT_DISK_C, &txt_string[i]);
                }
                else if(0==CRTL_wstrncmp(str_arr[2*i+1],L"Phone",CRTL_wstrlen(L"Phone")))
                {
                    MMI_GetLabelTextByLang(STXT_PHONE, &txt_string[i]);
                }
                else if(0==CRTL_wstrncmp(str_arr[2*i+1],L"TFCard",CRTL_wstrlen(L"TFCard")))
                {
                    MMI_GetLabelTextByLang(STR_LIST_MEMORY_CARD_FLDR, &txt_string[i]);
                }
                else if(0==CRTL_wstrncmp(str_arr[2*i+1],L"TFCard1",CRTL_wstrlen(L"TFCard1")))
                {
#ifndef DUAL_TCARD_SUPPORT
                    MMI_GetLabelTextByLang(TXT_COMMON_SD_CARD, &txt_string[i]);
#else
                    MMI_GetLabelTextByLang(TXT_COMMON_SD_CARD0, &txt_string[i]);
#endif
                }
                else if(0==CRTL_wstrncmp(str_arr[2*i+1],L"TFCard2",CRTL_wstrlen(L"TFCard2")))
                {
                    MMI_GetLabelTextByLang(TXT_COMMON_SD_CARD1, &txt_string[i]);
                }
                else
                {
                    SCI_TRACE_LOW("===disk name is wrong==");
                }
            }

            for(i = 0; i < item_num; i++)
            {
                AppendListItemByTextBuffer(txt_string[i].wstr_ptr,
                                           txt_string[i].wstr_len,
                                           0,
                                           TXT_NULL,
                                           STXT_SOFTKEY_SELECT_MK,
                                           TXT_COMMON_COMMON_BACK,
                                           ctrl_id,
                                           GUIITEM_STYLE_ONE_LINE_TEXT_MS);
            }

            GUILIST_SetCurItemIndex(ctrl_id, 0);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    //case MSG_CTL_OK:
    case MSG_APP_WEB:
        if (mmi_evt->event == MMI_NAMS_EVENT_SELECT_STORAGE)
        {
            int32  list_index = 0;
            int32  i;
            uint16 item_num = (uint16)mmi_evt->data_bytes;
            wchar ** str_arr = (wchar **)mmi_evt->data;

            list_index = GUILIST_GetCurItemIndex(ctrl_id);

            //calculate the answer index;
            i = (list_index<<1) + 1;
            if (i < (item_num<<1))
            {
                Jbed_nAms_installOperation(mmi_evt->event, RES_CONTINUE, str_arr[i]);
            }
#ifdef DYNAMIC_MAINMENU_SUPPORT
            s_is_update_midlet = FALSE;
#endif
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_RED:
        if (mmi_evt->event == MMI_NAMS_EVENT_SELECT_STORAGE)
        {
            MMI_STRING_T prompt = {0};

            MMI_GetLabelTextByLang(TXT_COMMON_CANCELING, &prompt);
            Jbed_nAms_installOperation(mmi_evt->event, RES_CANCEL, NULL);
            MMK_CloseWin(win_id);
            MMIJAVA_util_openWaitingWin(&prompt);
            break;
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_KEYUP_RED:
        //ignore
        break;

    case MSG_CLOSE_WINDOW:
        MMIJAVA_util_deallocMMIEvent_byEventID(mmi_evt->event_id);
        MMK_FreeWinAddData(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    //to be implemented
    return result;
}
#endif

#ifdef CMCC_41_SUPPORT
LOCAL MMI_RESULT_E  JavaShowDownloadJarSuccessCallback(
                                                  MMI_WIN_ID_T win_id,
                                                  MMI_MESSAGE_ID_E msg_id,
                                                  DPARAM param
                                                  )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch( msg_id )
    {
    case MSG_OPEN_WINDOW:
        SCI_TRACE_LOW("JavaShowDownloadJarSuccessCallback(),OPEN");
        break;

    case MSG_CLOSE_WINDOW:
        SCI_TRACE_LOW("=====myriadlog=======CALL Jbed_nAms_installOperation");
        Jbed_nAms_installOperation(MMI_NAMS_EVENT_SHOW_JAR_DOWNLOAD_SUCCESS, 0, NULL);
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return (recode);
}
#endif //CMCC_41_SUPPORT

LOCAL GUIMENU_FILE_DEVICE_E MMIJAVA_getInstallRootType(int suite_id)
{
    GUIMENU_FILE_DEVICE_E type = GUIMENU_DEVICE_UDISK; //default - D disk
    uint16_t * file_root = NULL;

    CPL_midletInfo_getSuiteInfoItem(suite_id, MI_FILE_ROOT, &file_root);

    if(0 == MMIAPICOM_Wstrncmp(file_root,L"System/",7))
    {
        type = GUIMENU_DEVICE_SYSTEM;
        goto b_final;
    }
    else if(0 == MMIAPICOM_Wstrncmp(file_root,L"Phone/",6))
    {
        type = GUIMENU_DEVICE_UDISK;
        goto b_final;
    }
#ifdef DUAL_TCARD_SUPPORT
    else if(0 == MMIAPICOM_Wstrncmp(file_root,L"TFCard1/",8))
    {
        type = GUIMENU_DEVICE_SDCARD_1;
        goto b_final;
    }
    else if(0 == MMIAPICOM_Wstrncmp(file_root,L"TFCard2/",8))
    {
        type = GUIMENU_DEVICE_SDCARD_2;
    }
#else
    else if(0 == MMIAPICOM_Wstrncmp(file_root,L"TFCard/",7))
    {
        type = GUIMENU_DEVICE_SDCARD_1;
    }
#endif

b_final:
    SCI_FREEIF(file_root);
    SCI_TRACE_LOW("===MMIJAVA_getInstallRootType(%d)===",type);
    return type;
}

/*****************************************************************************/
//  Description : Initialize the install pending event as unused status.
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_InitInstallPendingEvent(void)
{
    install_pending_event.is_pending = FALSE;
    install_pending_event.install_type = -1;
    install_pending_event.suite_id = -1;
}

/*****************************************************************************/
//  Description : Handle install pending event if needed.
//  For SVM, we need to wait VM thread exit completely before launch
//  midlet after installation. In this case, set install pending event
//  and handle it when vm thread exit.
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_HandleInstallPendingEvent(void)
{
    SCI_TRACE_LOW("=== Myriad MMIJAVA_HandleInstallPendingEvent: is_pending = %d, install_type = %d, suite_id = %d",
        install_pending_event.is_pending, install_pending_event.install_type, install_pending_event.suite_id);

    if (install_pending_event.is_pending)
    {
        if (install_pending_event.install_type == MMIJAVA_PREINSTALL)
        {
#ifdef DYNAMIC_MAINMENU_SUPPORT
            MMIJAVA_UpdatePreinstallSuiteId(install_pending_event.suite_id);
            MMIMAINMENU_SetState(MMIMAINMENU_PRE_INSTALL_STATE, FALSE, PNULL);
#endif
            if (MMK_IsOpenWin(MMIJAVA_PREINSTALL_PROGRESS_WIN_ID))
            {
                MMK_CloseWin(MMIJAVA_PREINSTALL_PROGRESS_WIN_ID);
            }

            MMIJAVA_StartSuiteInternal(install_pending_event.suite_id, -1);
        }
        else if (install_pending_event.install_type == MMIJAVA_COMMONINSTALL)
        {
            MMI_STRING_T prompt_text = {0};

#ifdef DYNAMIC_MAINMENU_SUPPORT
            if(s_is_update_midlet)
            {
                s_is_update_midlet = FALSE;
                MMIMAINMENU_SetState(MMIMAINMENU_UPDATE_STATE, FALSE, PNULL);
            }
            else
            {
                MMIMAINMENU_SetState(MMIMAINMENU_INSTALL_STATE, FALSE, PNULL);
            }
#endif
            MMI_GetLabelTextByLang(STR_JAVA_QUERY_INSTALLED_EXT01, &prompt_text);
            MMIPUB_OpenTextDialogByTextPtr(PNULL,
                                           &prompt_text,
                                           PNULL,
                                           PNULL,
                                           PNULL,
                                           PNULL,
                                           MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
                                           JavaLaunchMidletQueryCallback);
        }
        else
        {
            SCI_TRACE_LOW("=== Myriad MMIJAVA_HandleInstallPendingEvent Warning: install_type = %d.", install_pending_event.install_type);
        }

        MMIJAVA_InitInstallPendingEvent();
    }
}

/*****************************************************************************/
//  Description : Finalize related installation information.
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_ResetInstallProgress(void)
{
    if (g_is_install_in_progress)
    {
        g_is_install_in_progress = FALSE;
    }
}

/*****************************************************************************/
//  Description : Finalize related installation information.
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_GetInstallProgress(void)
{
     return   g_is_install_in_progress;
}

/*****************************************************************************/
//  Description : Finalize related installation information.
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetCanbeStopedFlag(BOOLEAN flag)
{
     s_can_stop = flag;
}


#ifdef SUPPORT_UPDATE_FUNCTION
/*****************************************************************************/
//  Description :Check whether this menu_id should display on dynamic popup menu, return
//  TRUE if display, otherwise return FALSE
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_Update(int  suite_id)
{
    uint32 time_period = 2000;

#if defined(JAVA_SUPPORT_TYPE_MVM)
    if(g_is_install_in_progress)
    {
        MMIPUB_OpenTextDialogByTextId(&time_period,
                                      TXT_JAVA_INSTALL_INSTALLER_IS_BUSY,
                                      TXT_NULL,
                                      PNULL,
                                      PNULL,
                                      PNULL,
                                      MMIPUB_SOFTKEY_DIALOG_CLOSE,
                                      MMIJAVA_HandleInstallAlertWinMsg);
        break;
    }
#endif

    Jbed_nAms_update(suite_id);
    g_is_install_in_progress = TRUE;
    if(!MMK_IsOpenWin(MMIJAVA_PROGRESS_WIN_ID))
    {
        MMI_WIN_ID_T    progress_win_id = MMIJAVA_PROGRESS_WIN_ID;
        MMIPUB_OpenProgressWinByTextId(TXT_COMMON_UPDATING, &progress_win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, MMIJAVA_HandleProcessWinMsg);
        MMIPUB_SetProgressTotalSize(&progress_win_id, 100);
    }
    return  TRUE;
}
#endif

/*****************************************************************************/
//  Description : get installing state
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsInstalling(void)
{
    g_is_install_in_progress = isJbedThreadExist()? g_is_install_in_progress : FALSE;
    return g_is_install_in_progress;
}

/*****************************************************************************/
//  Description : Handle install MMI event
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_nAms_handleMMIInstallEvent(int mmi_event_id)
{
    uint32 time_period = 2000;
    MMIEvent * mmi_evt = NULL;
    int * pevt_id = NULL;
    MMI_STRING_T text_str = {0};
    mmi_evt = MMIJAVA_find_MMIEvent(mmi_event_id);
    if(NULL == mmi_evt)
    {
        return;
    }

    if(!g_is_install_in_progress)
    {
        MMIJAVA_util_deallocMMIEvent_byEventID(mmi_evt->event_id);
        return;
    }
    switch (mmi_evt->event)
    {
    case MMI_NAMS_EVENT_INSTALL:
        if (s_is_ota_install)
        {
            s_is_ota_install = FALSE;
        }
        g_is_install_in_progress = FALSE;
        switch(mmi_evt->result)
        {
        /**
         * TODO: magic number 0 refer to RESULT_NO_ERROR
         *       magic number 74 refer to RESULT_USER_CANCELLED
         *       magic number 66 refer to RESULT_COMPILATION_INSUFFICIENT_MEMORY
         *       in invalidJadException.java
         *       Next step should define it into cpl_tpal.h
         */
        case RESULT_NO_ERROR:
            {
                Jbed_nAms_updateMIDletInfo();
#ifdef DYNAMIC_MAINMENU_SUPPORT
                if(s_is_update_midlet && MMIJAVA_IsInstalledSuiteInRecord(g_installed_suiteId))
                {
                     /**
                      * Since we only add preinstall midlet in NV, the return value of
                      * MMIJAVA_IsInstalledSuiteInRecord is true means preinstall midlet
                      * suite is updated, however the preinstall midlet icon menu id is preset.
                      * Hence we can't call MMIJAVA_AddSuiteRecordToMainmenu again.
                      * Otherwise, it will generate mainmenu icon twice.
                      */
                    SCI_TRACE_LOW("=== Myriad preinstall midlet suite is updated. ===");
                }
                else
                {
                    MMIJAVA_AddSuiteRecordToMainmenu(g_installed_suiteId,(GUIMENU_FILE_DEVICE_E)MMIJAVA_getInstallRootType(g_installed_suiteId));
                }
#endif
#ifdef SUPPORT_LANCH_AFTER_INSTALL
                // set the pending event, then ask query launch alert after vm thread exit.
                install_pending_event.is_pending = TRUE;
                install_pending_event.install_type = MMIJAVA_COMMONINSTALL;
                install_pending_event.suite_id = g_installed_suiteId;
#else
                 //prompt success
                MMIPUB_OpenTextDialogByTextId(&time_period,
                                              TXT_COMPLETE,
                                              TXT_NULL,
                                              PNULL,
                                              PNULL,
                                              PNULL,
                                              MMIPUB_SOFTKEY_DIALOG_CLOSE,
                                              MMIJAVA_HandleInstallAlertWinMsg);
#endif
                break;
            }

        case RESULT_USER_CANCELLED:
            //MMIJAVA_util_closeWaitingWin(4);

            break;

        case RESULT_COMPILATION_INSUFFICIENT_MEMORY:
            if(NULL != mmi_evt->data)
            {
                text_str.wstr_ptr = mmi_evt->data;
                text_str.wstr_len = mmi_evt->data_bytes>>1;
                MMIPUB_OpenTextDialogByTextPtr(&time_period,
                                               &text_str,
                                               PNULL,
                                               PNULL,
                                               PNULL,
                                               PNULL,
                                               MMIPUB_SOFTKEY_DIALOG_CLOSE,
                                               MMIJAVA_HandleInstallAlertWinMsg);
            }
            else
            {
                MMIPUB_OpenTextDialogByTextId(&time_period,
                                              STR_JAVA_DELETE_APPS,
                                              TXT_NULL,
                                              PNULL,
                                              PNULL,
                                              PNULL,
                                              MMIPUB_SOFTKEY_DIALOG_CLOSE,
                                              MMIJAVA_HandleInstallAlertWinMsg);
            }
            break;

        default:
            {
                MMI_STRING_T prompt_text = {0};
                prompt_text.wstr_ptr = (unsigned short *)(mmi_evt->data);
                prompt_text.wstr_len = mmi_evt->data_bytes>>1;
                MMIPUB_OpenTextDialogByTextPtr(&time_period,
                                               &prompt_text,
                                               PNULL,
                                               PNULL,
                                               PNULL,
                                               PNULL,
                                               MMIPUB_SOFTKEY_DIALOG_CLOSE,
                                               MMIJAVA_HandleInstallAlertWinMsg);
                break;
            }
        }

        if (MMK_IsOpenWin(MMIJAVA_PROGRESS_WIN_ID))
        {
            MMK_CloseWin(MMIJAVA_PROGRESS_WIN_ID);
        }
        else if (Jbed_tm_getRunningAppNum() == 0)
        {
            /* if progress win is not opened, exit jbed directly */
            g_is_install_in_progress = FALSE;
            MMIJAVA_NotifyJVMExit();
        }

        if (MMK_IsOpenWin(JBED_INSTALL_INFO_MENU_POP_WIN_ID))
        {
            MMK_CloseWin(JBED_INSTALL_INFO_MENU_POP_WIN_ID);
        }

#ifdef DYNAMIC_MAINMENU_SUPPORT
        if (!install_pending_event.is_pending)
        {
            if (s_is_update_midlet)
            {
                s_is_update_midlet = FALSE;
                MMIMAINMENU_SetState(MMIMAINMENU_UPDATE_STATE, FALSE, PNULL);
            }
            else
            {
                MMIMAINMENU_SetState(MMIMAINMENU_INSTALL_STATE, FALSE, PNULL);
            }
        }
#endif

        MMIJAVA_util_deallocMMIEvent_byEventID(mmi_evt->event_id);
        break;

    case MMI_NAMS_EVENT_INSTALL_QUERY:
    case MMI_NAMS_EVENT_INSTALL_QUERY_YES_NO:
    case MMI_NAMS_EVENT_INSTALL_PREVIEW_JAD:
        if(!s_be_interruptted)
        {
            MMI_STRING_T warning_text = {0};
            MMI_STRING_T prompt_text = {0};
            MMI_WIN_ID_T progress_win_id = JBED_INSTALL_INFO_MENU_POP_WIN_ID;
            wchar *transit_data_ptr = (wchar *)(mmi_evt->data);

#ifdef DYNAMIC_MAINMENU_SUPPORT
            /* Parse the prompt text from "updateinfo=snum_..." when updating the midlet. */
            if (mmi_evt->event == MMI_NAMS_EVENT_INSTALL_QUERY
                && (mmi_evt->result == IQ_ALREADY_INSTALLED || mmi_evt->result == IQ_NEW_VERSION || mmi_evt->result == IQ_OLD_VERSION)
                    && (0 == MMIAPICOM_Wstrncmp((const wchar*)transit_data_ptr, L"updateinfo=", MMIAPICOM_Wstrlen(L"updateinfo="))))
            {
                int suite_id;
                wchar *equal_pos = MMIAPICOM_Wstrstr(transit_data_ptr, L"=");
                wchar *underline_pos = MMIAPICOM_Wstrstr(transit_data_ptr, L"_");

                // Parse suite id from storage name which is followed by "updateinfo=".
                suite_id = Jbed_nAms_getSuiteIdFromStorageName(
                    (unsigned short *)(equal_pos + 1), underline_pos - equal_pos);
                // Extract the prompt text.
                prompt_text.wstr_ptr = underline_pos + 1;
                prompt_text.wstr_len = (mmi_evt->data_bytes>>1) - (underline_pos - transit_data_ptr + 1);

                s_is_update_midlet = TRUE;
                MMIMAINMENU_SetState(MMIMAINMENU_INSTALL_STATE, FALSE, PNULL);
                MMIMAINMENU_SetState(MMIMAINMENU_UPDATE_STATE, TRUE, (DPARAM)(&suite_id));
            }
            else
#endif
            {
                prompt_text.wstr_ptr = (wchar *)(mmi_evt->data);
                prompt_text.wstr_len = mmi_evt->data_bytes>>1;
            }

            s_current_id = mmi_event_id;
            if(IQ_INFORM_USER != mmi_evt->result)
            {
                MMIPUB_OpenTextDialogByTextPtr(PNULL,
                                               &prompt_text,
                                               PNULL,
                                               PNULL,
                                               PNULL,
                                               PNULL,
                                               MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
                                               Jbed_HandleInstallInfoMenuWinMsg);
            }
            else
            {
                MMIPUB_OpenTextDialogByTextPtr(&time_period,
                                               &prompt_text,
                                               PNULL,
                                               PNULL,
                                               PNULL,
                                               PNULL,
                                               MMIPUB_SOFTKEY_DIALOG_CLOSE,
                                               MMIJAVA_HandleInstallAlertWinMsg);
                Jbed_nAms_installOperation(mmi_evt->event, RES_CONTINUE, NULL);
            }
            SCI_TRACE_LOW("============MYRIADLOG: creat win at once ==============");
        }
        else
        {
            SCI_TRACE_LOW("============MYRIADLOG: creat win delay to get focus,eventId(%d) ==============",pevt_id);
            //s_create_win_tab  = MMIJAVA_PROGRESS_WIN_ID;
            //s_create_event_id = mmi_event_id;
        }
        break;

    case MMI_NAMS_EVENT_INSTALL_CANCEL:
        MMIJAVA_util_deallocMMIEvent_byEventID(mmi_evt->event_id);
        break;

    case MMI_NAMS_EVENT_INSTALL_UNSTOPPABLE:
        s_can_stop = FALSE;
        MMIJAVA_util_deallocMMIEvent_byEventID(mmi_evt->event_id);
        break;

    case MMI_NAMS_EVENT_INSTALL_PROGRESS:
        if(MMK_IsFocusWin(MMIJAVA_PROGRESS_WIN_ID))
        {
            MMI_WIN_ID_T    progress_win_id = MMIJAVA_PROGRESS_WIN_ID;
            MMIPUB_UpdateProgressBarEx(&progress_win_id, mmi_evt->result, TRUE);
        }
#ifdef DYNAMIC_MAINMENU_SUPPORT
        else
        {
            int32 cur_progress = mmi_evt->result/(s_is_ota_install ? 3 : 2);
            MMIMAINMENU_SetInstallProgress(s_recalculate_progress + cur_progress);
        }
#endif
        MMIJAVA_util_deallocMMIEvent_byEventID(mmi_evt->event_id);
        break;

    case MMI_NAMS_EVENT_INSTALL_STATUS:
        {
#ifdef DYNAMIC_MAINMENU_SUPPORT
            if (mmi_evt->result == IS_COMPILING)
            {
                s_recalculate_progress = s_is_ota_install ? 33 : 0;
            }
            else if (mmi_evt->result == IS_OPTIMIZING_JAR_FILE)
            {
                s_recalculate_progress = s_is_ota_install ? 67 : 50;
            }
            else if (mmi_evt->result == IS_GETTING_JAR)
            {
                s_recalculate_progress = 0;
            }
            MMIMAINMENU_SetInstallProgress(s_recalculate_progress);
#else
            MMI_WIN_ID_T    progress_win_id = MMIJAVA_PROGRESS_WIN_ID;
            MMIPUB_UpdateProgressBarEx(&progress_win_id, 0, TRUE);
//如果进度条窗口是带title的样式,则内容显示在title处
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
            if (mmi_evt->result == IS_COMPILING)
            {
                GUIWIN_SetTitleTextId(MMIJAVA_PROGRESS_WIN_ID, TXT_JAVA_COMPILING, FALSE);
            }
            else if (mmi_evt->result == IS_OPTIMIZING_JAR_FILE)
            {
                GUIWIN_SetTitleTextId(MMIJAVA_PROGRESS_WIN_ID, TXT_JAVA_OPTIMIZING_JAR_FILE, FALSE);
            }
            else if (mmi_evt->result == IS_GETTING_JAR)
            {
                GUILABEL_SetTextById(MMIJAVA_PROGRESS_WIN_ID, TXT_JAVA_GETTING_JAR_FILE, FALSE);
            }
#else
            {
                MMI_STRING_T  process_text = {0};

                if (mmi_evt->result == IS_COMPILING)
                {
                    MMI_GetLabelTextByLang(TXT_JAVA_COMPILING, &process_text);
                }
                else if (mmi_evt->result == IS_OPTIMIZING_JAR_FILE)
                {
                    MMI_GetLabelTextByLang(TXT_JAVA_OPTIMIZING_JAR_FILE, &process_text);
                }
                else if (mmi_evt->result == IS_GETTING_JAR)
                {
                    MMI_GetLabelTextByLang(TXT_JAVA_GETTING_JAR_FILE, &process_text);
                }

                MMIPUB_SetProgressBarTextByPtr(MMIJAVA_PROGRESS_WIN_ID, &process_text, FALSE);
            }
#endif
#endif //DYNAMIC_MAINMENU_SUPPORT

            MMIJAVA_util_deallocMMIEvent_byEventID(mmi_evt->event_id);
        }
        break;

    case MMI_NAMS_EVENT_SELECT_STORAGE:
        {
    #if defined(SUPPORT_SELECT_STORAGE) && !defined(AMS_TCK_RUNNER)
            if(!s_be_interruptted)
            {
                pevt_id = (int *)SCI_ALLOCA(sizeof(int));
                *pevt_id = mmi_event_id;
                MMK_CreateWin((uint32 *)JBED_STORAGE_SELECT_WIN_TAB, (ADD_DATA)pevt_id);
            }
            else
            {
                //s_create_win_tab  = (uint32)JBED_STORAGE_SELECT_WIN_TAB;
                //s_create_event_id = mmi_event_id;
            }
        #else
            wchar * selsct = Jbed_getDefaultInstallDiskName();
            SCI_TRACE_LOW("===return EVENT_SELECT_STORAGE when handle MMI_NAMS_EVENT_SELECT_STORAGE==");
            Jbed_nAms_installOperation(MMI_NAMS_EVENT_SELECT_STORAGE, RES_CONTINUE, selsct);
            MMIJAVA_util_deallocMMIEvent_byEventID(mmi_evt->event_id);
        #endif
        }
        break;
#ifdef CMCC_41_SUPPORT
    case MMI_NAMS_EVENT_SHOW_JAR_DOWNLOAD_SUCCESS:
        {
            MMI_STRING_T prompt_text = {0}; //prompt text
            MMI_STRING_T prompt_head = {0};
            MMI_STRING_T prompt_jar_name = {0};
            MMI_STRING_T prompt_jar_url = {0};
            MMI_STRING_T prompt_end = {0};
            uint16_t blank[] = {0x20};
            uint32_t blank_len = 1;
            int jar_name_len = 0;
            int i,j;

            SCI_TRACE_LOW("=======myriadlog===== receive MMI_NAMS_EVENT_SHOW_JAR_DOWNLOAD_SUCCESS message");
            MMI_GetLabelTextByLang(TXT_COMMON_DOWNLOAD_COMN, &prompt_head);
            MMI_GetLabelTextByLang(TXT_SUCCESS, &prompt_end);

            //Get jar name position in jar url.
            prompt_jar_url.wstr_ptr = (unsigned short *)(mmi_evt->data);
            prompt_jar_url.wstr_len = mmi_evt->data_bytes>>1;
            SCI_TRACE_LOW("=======myriadlog===== prompt_jar_url.wstr_len =%d", prompt_jar_url.wstr_len);
            if (NULL == prompt_jar_url.wstr_ptr)
            {
                SCI_TRACE_LOW("=======myriadlog===== prompt_jar_url.wstr_ptr=null");
                break;
            }
            for (i = prompt_jar_url.wstr_len - 1; i >= 0; i--)
            {
                if (prompt_jar_url.wstr_ptr[i] == '/')
                {
                    i++;
                    SCI_TRACE_LOW("=======myriadlog===== prompt_jar_url.wstr_ptr[%d] = %x", i, prompt_jar_url.wstr_ptr[i]);
                    break;
                }
            }
            //Get jar name string from jar url.
            prompt_jar_name.wstr_len = prompt_jar_url.wstr_len - i + 1;
            SCI_TRACE_LOW("=======myriadlog===== prompt_jar_name.wstr_len =%d", prompt_jar_name.wstr_len);
            prompt_jar_name.wstr_ptr = (wchar *)SCI_ALLOCA(prompt_jar_name.wstr_len*sizeof(wchar));
            if (PNULL == prompt_jar_name.wstr_ptr)
            {
               SCI_TRACE_LOW("=====myriadlog=======Error SCI_ALLOCA fail");
               break;
            }
            SCI_MEMSET(prompt_jar_name.wstr_ptr, 0x00, (prompt_jar_name.wstr_len*sizeof(wchar)));
            MMIAPICOM_Wstrncpy(prompt_jar_name.wstr_ptr, prompt_jar_url.wstr_ptr + i, prompt_jar_name.wstr_len - 1);
            //Construct prompt text.
            prompt_text.wstr_len = prompt_head.wstr_len + prompt_jar_name.wstr_len + prompt_end.wstr_len + blank_len*2;
            prompt_text.wstr_ptr = (wchar *)SCI_ALLOCA(prompt_text.wstr_len*sizeof(wchar));
            if (PNULL == prompt_text.wstr_ptr)
            {
               SCI_TRACE_LOW("=====myriadlog=======Error SCI_ALLOCA fail");
               break;
            }

            SCI_MEMSET(prompt_text.wstr_ptr, 0x00, (prompt_text.wstr_len*sizeof(wchar)));
            MMIAPICOM_Wstrncat(prompt_text.wstr_ptr, prompt_head.wstr_ptr, prompt_head.wstr_len);
            MMIAPICOM_Wstrncat(prompt_text.wstr_ptr, blank, blank_len);
            MMIAPICOM_Wstrncat(prompt_text.wstr_ptr, prompt_jar_name.wstr_ptr, prompt_jar_name.wstr_len);
            MMIAPICOM_Wstrncat(prompt_text.wstr_ptr, blank, blank_len);
            MMIAPICOM_Wstrncat(prompt_text.wstr_ptr, prompt_end.wstr_ptr, prompt_end.wstr_len);
            MMIPUB_OpenTextDialogByTextPtr(PNULL,
                                           &prompt_text,
                                           PNULL,
                                           PNULL,
                                           PNULL,
                                           PNULL,
                                           MMIPUB_SOFTKEY_ONE,
                                           (MMIPUB_HANDLE_FUNC)JavaShowDownloadJarSuccessCallback);
            SCI_FREEIF(prompt_jar_name.wstr_ptr);
            SCI_FREEIF(prompt_text.wstr_ptr);
            break;
        }
#endif //CMCC_41_SUPPORT
    default:
        MMIJAVA_util_deallocMMIEvent_byEventID(mmi_evt->event_id);
        break;
    }

}


/*****************************************************************************/
//  Description :This API is used to install a Java application which is
//               located by the path. It can be a filesytem path, or a network
//               resource path, such as http or https.
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
//       @param path
//              the java application located.
//       @param pathSize
//              The character number of the Path(Not bytes).
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_Install(
                    uint16 * path,
                    int32    path_size
                )
{
    uint32 time_period = 2000;
    //FIXME: Address alerting for PDA style is not ready recently.
    MMI_WIN_ID_T    progress_win_id = MMIJAVA_PROGRESS_WIN_ID;
    uint32 len = 0;

    SCI_TRACE_LOW("=== Myriad MMIAPIJAVA_Install enter.");

    if (g_jbed_mutex_flag)
    {
        return;
    }

    if(MMIJAVA_HandleStateConflict())
    {
        return;
    }

    if(!Jbed_checkRootAvailable())
    {
        MMIPUB_OpenTextDialogByTextId(&time_period,
                                      STR_JAVA_NO_STORAGE,
                                      TXT_NULL,
                                      PNULL,
                                      PNULL,
                                      PNULL,
                                      MMIPUB_SOFTKEY_DIALOG_CLOSE,
                                      MMIJAVA_HandleInstallAlertWinMsg);
        return;
    }
#if defined(JAVA_SUPPORT_TYPE_SVM) && defined(SUPPORT_BACKGROUND_RUN)
    /* We already check INSTALL or UNINSTALL conflict before, if the jbed thread
     * still exists at this time, it means that the midlet is running on background
     * or vm is starting or closing.
     */
    if (isJbedThreadExist())
    {
        SCI_TRACE_LOW("=== Myriad MMIAPIJAVA_Install Jbed thread exist.");

        if (Jbed_tm_getRunningAppNum() > 0) // the midlet is running on background.
        {
            MMIJAVA_PostMIDletRunningBanner();
            return;
        }
        else // vm is starting or closing.
        {
            MMIJAVA_OpenAlertByTextId(STR_JAVA_VM_BUSY);
            return;
        }
    }
#endif

#if defined(SUPPORT_HEAD_FILE_PREINSTALL)
    if(!Jbed_headFilePreinstall_fileRestore())
    {
        MMIPUB_OpenAlertWarningWin(STR_JAVA_DELETE_APPS);
        return;
    }
#endif/*SUPPORT_HEAD_FILE_PREINSTALL*/

    Jbed_nAms_intialize(TRUE);
#ifdef DYNAMIC_MAINMENU_SUPPORT
    s_is_update_midlet = FALSE;
#endif
    Jbed_nAms_registerNotify(MMIJAVA_util_amsCB);

    g_is_install_in_progress = TRUE;

#if 0 //comment out
    if(!MMK_IsOpenWin(MMIJAVA_PROGRESS_WIN_ID))
    {
        MMIPUB_OpenProgressWinByTextId(TXT_COM_LOCALINSTALL, &progress_win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, MMIJAVA_HandleProcessWinMsg);
        MMIPUB_SetProgressTotalSize(&progress_win_id, 100);
    }
#endif

    if(Jbed_nAms_installBegin(path, path_size, &s_is_ota_install))
    {
#ifdef DYNAMIC_MAINMENU_SUPPORT
        MMIMAINMENU_SetState(MMIMAINMENU_INSTALL_STATE, TRUE, PNULL);
#endif
    }
    else    //create jbed thread fail
    {
        finializeGlobalArgs();
        Jbed_clrPendEvt();
        Jbed_nAms_setTransitionFlag(FALSE);
        MMIJAVA_CloseRunningWin();
        //add by murphy.xie,reset g_is_install_in_progress
        MMK_CloseWin(MMIJAVA_PROGRESS_WIN_ID);
        g_is_install_in_progress= FALSE;
        MMIPUB_OpenTextDialogByTextId(&time_period,
                                      STR_JAVA_NO_MEMORY,
                                      TXT_NULL,
                                      PNULL,
                                      PNULL,
                                      PNULL,
                                      MMIPUB_SOFTKEY_DIALOG_CLOSE,
                                      MMIJAVA_HandleInstallAlertWinMsg);
    }

}

/*****************************************************************************/
//  Description : handle progressbar event
//  Global resource dependence : none
//  Author: Sean Xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIJAVA_HandleCommandInstallProcessWinMsg(
                                         MMI_WIN_ID_T        win_id,
                                         MMI_MESSAGE_ID_E    msg_id,
                                         DPARAM              param
                                     )
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    case MSG_CLOSE_WINDOW:
        break;

    default:
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    }
    return (result);
}

static void MMIJAVA_RequireLaunchMIDlet(int suite_id)
{
    if (MMK_IsOpenWin(MMIJAVA_PREINSTALL_PROGRESS_WIN_ID))
    {
        MMK_CloseWin(MMIJAVA_PREINSTALL_PROGRESS_WIN_ID);
    }
    MMIJAVA_EnvironmentInit();
    MMIJAVA_StartSuite(suite_id, -1);
}

PUBLIC void MMIJAVA_nAms_handleMMICommandInstallEvent(EVENT_TASKTOMMI_T *event_ptr)
{
    uint32 time_period = 2000;
    MMI_WIN_ID_T    progress_win_id = MMIJAVA_PREINSTALL_PROGRESS_WIN_ID;

    if(NULL == event_ptr)
    {
        return;
    }

    SCI_TRACE_LOW("=== myriad java handleMMICommandInstallEvent event = %d, result = %d\n",
        event_ptr->eventId, event_ptr->result);
    switch (event_ptr->eventId)
    {
        case MMI_NAMS_EVENT_COMMAND_INSTALL:
            g_is_install_in_progress = FALSE;

            if (event_ptr->result == RESULT_NO_ERROR)
            {
                int  suite_id = Jbed_nAms_getSuiteIdFromStorageName((unsigned short *)event_ptr->data, event_ptr->dataLen>>1);
                MMIPUB_UpdateProgressBarEx(&progress_win_id, 100, TRUE);
                Jbed_nAms_updateMIDletInfo();

                // set the pending event, then launch the midlet after vm thread exit.
                install_pending_event.is_pending = TRUE;
                install_pending_event.install_type = MMIJAVA_PREINSTALL;
                install_pending_event.suite_id = suite_id;
            }
            else // install failed.
            {
                MMI_STRING_T prompt_text = {0};
                prompt_text.wstr_ptr = (unsigned short *)(event_ptr->data);
                prompt_text.wstr_len = event_ptr->dataLen>>1;
                MMIPUB_OpenTextDialogByTextPtr(&time_period,
                                               &prompt_text,
                                               PNULL,
                                               PNULL,
                                               PNULL,
                                               PNULL,
                                               MMIPUB_SOFTKEY_DIALOG_CLOSE,
                                               MMIJAVA_HandleInstallAlertWinMsg);
                if (MMK_IsOpenWin(MMIJAVA_PREINSTALL_PROGRESS_WIN_ID))
                {
                    MMK_CloseWin(MMIJAVA_PREINSTALL_PROGRESS_WIN_ID);
                }
#ifdef DYNAMIC_MAINMENU_SUPPORT
                MMIMAINMENU_SetState(MMIMAINMENU_PRE_INSTALL_STATE, FALSE, PNULL);
#endif
            }
            break;
        case MMI_NAMS_EVENT_COMMAND_INSTALL_STATUS:
            {
#ifdef DYNAMIC_MAINMENU_SUPPORT
                if (event_ptr->result == IS_COMPILING)
                {
                    s_recalculate_progress = 0;
                }
                else if (event_ptr->result == IS_OPTIMIZING_JAR_FILE)
                {
                    s_recalculate_progress = 50;
                }
                else if (event_ptr->result == IS_GETTING_JAR)
                {
                    s_recalculate_progress = 0;
                }
                MMIMAINMENU_SetInstallProgress(s_recalculate_progress);
#else
                {
                    MMI_STRING_T  process_text = {0};

                    MMIPUB_UpdateProgressBarEx(&progress_win_id, 0, TRUE);

                    if (event_ptr->result == IS_COMPILING)
                    {
                        MMI_GetLabelTextByLang(TXT_JAVA_COMPILING, &process_text);
                    }
                    else if (event_ptr->result == IS_OPTIMIZING_JAR_FILE)
                    {
                        MMI_GetLabelTextByLang(TXT_JAVA_OPTIMIZING_JAR_FILE, &process_text);
                    }
                    else if (event_ptr->result == IS_GETTING_JAR)
                    {
                        MMI_GetLabelTextByLang(TXT_JAVA_GETTING_JAR_FILE, &process_text);
                    }

                    MMIPUB_SetProgressBarTextByPtr(MMIJAVA_PREINSTALL_PROGRESS_WIN_ID, &process_text, FALSE);
                }
#endif
                break;
            }
        case MMI_NAMS_EVENT_COMMAND_INSTALL_PROGRESS:
#ifdef DYNAMIC_MAINMENU_SUPPORT
            {
                int32 cur_progress = event_ptr->result/2;
                MMIMAINMENU_SetInstallProgress(s_recalculate_progress + cur_progress);
            }
#else
            if(MMK_IsFocusWin(MMIJAVA_PREINSTALL_PROGRESS_WIN_ID))
            {
                MMIPUB_UpdateProgressBarEx(&progress_win_id, event_ptr->result, TRUE);
            }
#endif
            break;
    }

    SCI_FREEIF(event_ptr->data);
}

/** Command install a midlet suite with specified jad or jar path. */
PUBLIC void MMIAPIJAVA_CommandInstall(char *path)
{
    uint32 time_period = 2000;
    MMI_WIN_ID_T    progress_win_id = MMIJAVA_PREINSTALL_PROGRESS_WIN_ID;
    char * argv[2];
    char *filePrefix = "file:///";
    static char insUrl[128];

    SCI_TRACE_LOW("=== Myriad MMIAPIJAVA_CommandInstall enter.");

    if(MMIJAVA_HandleStateConflict())
    {
        return;
    }

    if(!Jbed_checkRootAvailable())
    {
        MMIPUB_OpenTextDialogByTextId(&time_period,
                                      STR_JAVA_NO_STORAGE,
                                      TXT_NULL,
                                      PNULL,
                                      PNULL,
                                      PNULL,
                                      MMIPUB_SOFTKEY_DIALOG_CLOSE,
                                      MMIJAVA_HandleInstallAlertWinMsg);
        return;
    }
#if defined(JAVA_SUPPORT_TYPE_SVM) && defined(SUPPORT_BACKGROUND_RUN)
    /* We already check INSTALL or UNINSTALL conflict before, if the jbed thread
     * still exists at this time, it means that the midlet is running on background
     * or vm is starting or closing.
     */
    if (isJbedThreadExist())
    {
        SCI_TRACE_LOW("=== Myriad MMIAPIJAVA_CommandInstall Jbed thread exist.");

        if (Jbed_tm_getRunningAppNum() > 0) // the midlet is running on background.
        {
            MMIJAVA_PostMIDletRunningBanner();
            return;
        }
        else // vm is starting or closing.
        {
            MMIJAVA_OpenAlertByTextId(STR_JAVA_VM_BUSY);
            return;
        }
    }
#endif

    CRTL_memset(insUrl, 0, 128);

    if (path == NULL)
    {
        SCI_TRACE_LOW("=== Myriad MMIAPIJAVA_CommandInstall Warning: install path is null.");
        return;
    }
    else if (CRTL_strncmp(path, filePrefix, CRTL_strlen(filePrefix)) == 0)
    {
        CRTL_strcpy(insUrl, path);
        argv[0] = "-install";
        argv[1] = insUrl;
    }
    else // The install path must start with "file:///"
    {
        CRTL_strcpy(insUrl, filePrefix);
        CRTL_strcat(insUrl, path);
        argv[0] = "-install";
        argv[1] = insUrl;
    }

    SCI_TRACE_LOW("=== myriad Java MMIAPIJAVA_CommandInstall argv[1] = %s", argv[1]);

    Jbed_nAms_intialize(FALSE);
    Jbed_nAms_registerNotify(MMIJAVA_util_amsCB);

    if (JavaEntry(2, argv))
    {
        g_is_install_in_progress = TRUE;
#ifdef DYNAMIC_MAINMENU_SUPPORT
        MMIMAINMENU_SetState(MMIMAINMENU_PRE_INSTALL_STATE, TRUE, PNULL);
#else
        if(!MMK_IsOpenWin(MMIJAVA_PREINSTALL_PROGRESS_WIN_ID))
        {
            MMIPUB_OpenProgressWinByTextId(TXT_COM_LOCALINSTALL,
                                           &progress_win_id,
                                           PNULL,
                                           MMIPUB_SOFTKEY_CUSTOMER,
                                           MMIJAVA_HandleCommandInstallProcessWinMsg);
            MMIPUB_SetProgressTotalSize(&progress_win_id, 100);
        }
#endif
    }
    else
    {
        Jbed_nAms_finalize();
        MMIPUB_OpenTextDialogByTextId(&time_period,
                                      STR_JAVA_NO_MEMORY,
                                      TXT_NULL,
                                      PNULL,
                                      PNULL,
                                      PNULL,
                                      MMIPUB_SOFTKEY_DIALOG_CLOSE,
                                      MMIJAVA_HandleInstallAlertWinMsg);
    }

}

/*****************************************************************************/
//  Description : Is Can Stop
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsCanStop(void)
{
    return s_can_stop;
}

PUBLIC void MMIJAVA_OpenAlertByTextId(uint32 text_id)
{
    uint32 time_period = 2000;

    MMIPUB_OpenTextDialogByTextId(&time_period,
                                  (MMI_TEXT_ID_T)text_id,
                                  TXT_NULL,
                                  PNULL,
                                  PNULL,
                                  PNULL,
                                  MMIPUB_SOFTKEY_DIALOG_CLOSE,
                                  MMIJAVA_HandleInstallAlertWinMsg);
}

