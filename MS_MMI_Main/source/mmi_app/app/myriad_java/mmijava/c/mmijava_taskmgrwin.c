/**
 * Copyright 2011 Myriad Group AG. All Rights Reserved.
 * $Id: //depot/dev/project/spreadtrum/6530_new/export/src/mmijava_taskmgrwin.c#8 $
 */

/**
 * @file
 * implemented relevant taskmanager for the 6800h build of FBCC
 */
#include <window_parse.h>
#include <guilistbox.h>
#include <jbed_exported_defines.h>
#include <jbed_main.h>
#include <cpl_tm.h>
#include <opl_global.h>
#include <os_api.h>
#include <mmi_common.h>
#include <mmipub.h>
#include <mmi_appmsg.h>
#include <mmijava_id.h>
#include <java_mmi_image.h>
#include <java_mmi_text.h>
#include <mmijava_common.h>
#include <mmijava_menutable.h>
#include <mmijava_export.h>
#include "mmijava_suitefun.h"
// to prevent build errors: bool_t redefinitio
#define _TYPES_H_
#include "mmiidle_export.h"
#undef _TYPES_H_


#ifdef MMI_TASK_MANAGER
#define MMK_CreateJavaWin(_WIN_TABLE_, _ADD_DATA_)  OpenJavaWin(_WIN_TABLE_, _ADD_DATA_)
#else
#define MMK_CreateJavaWin   MMK_CreateWin
#endif

#ifdef SUPPORT_TASK_MANAGER

LOCAL MMI_RESULT_E Jbed_HandleJbedAppWinMsg(
                                                     MMI_WIN_ID_T      win_id,
                                                     MMI_MESSAGE_ID_E  msg_id,
                                                     DPARAM            param
                                             );

WINDOW_TABLE(JBED_APP_WIN_TAB) =
{
    WIN_FUNC((uint32)Jbed_HandleJbedAppWinMsg),
    WIN_ID(JBED_APP_WIN_ID),
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL|GUITAB_STATE_SINGLE_LINE, MMIJAVA_TAB_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(JBED_SUITE_LIST_MENU_WIN_TAB) =
{
#ifndef SUPPORT_TASK_MANAGER
    WIN_TITLE(TXT_APP_MANAGER),
#endif
    WIN_FUNC((uint32)Jbed_HandleSuiteListMenuWinMsg),
    WIN_ID(MMIJAVA_AMS_WIN_ID),
#if (defined MMI_PDA_SUPPORT)&& (defined SUPPORT_TASK_MANAGER)
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIJAVA_SUITE_LIST_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIJAVA_SUITE_LIST_CTRL_ID, MMIJAVA_SUITE_LIST_FORM_CTRL_ID),
#else
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIJAVA_SUITE_LIST_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
/*****************************************************************************/
//  Description :Get current running app list.
//  Global resource dependence : none
//  Author: martin zhao
//  Note:
/*****************************************************************************/
LOCAL void MMIJava_tm_openRunningAppList(
                                            MMI_WIN_ID_T win_id,
                                            MMI_CTRL_ID_T ctrl_id
                                            );

/*****************************************************************************/
//  Description : Add running app to listview.
//  Global resource dependence : none
//  Author: martin zhao
//  Note:
/*****************************************************************************/
LOCAL void MMIJava_tm_appendRunningAppData2List(
                                                  BOOLEAN         is_text,
                                                  uint16          item_index,
                                                  uint16          content_index,
                                                  MMI_CTRL_ID_T   ctrl_id
                                                  );

/*****************************************************************************/
//  Description : Process task manager window message.
//  Global resource dependence : none
//  Author: martin zhao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Jbed_HandleTMListMenuWinMsg(
                                                 MMI_WIN_ID_T     win_id,
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM           param
                                                 );
#endif
/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : the process message function of the java
//  Global resource dependence :
//  Author:Sean.Xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  JavaApplet_HandleEvent(
                         IAPPLET_T*          iapplet_ptr,
                         MMI_MESSAGE_ID_E    msg_id,
                         DPARAM              param
                         );

LOCAL MMI_RESULT_E HandleJavaAppletWinMsg(
                                     MMI_WIN_ID_T      win_id,
                                     MMI_MESSAGE_ID_E  msg_id,
                                     DPARAM            param
                                     );

LOCAL MMIJAVA_APPLET_T *s_java_applet_instance = PNULL;

BOOLEAN g_is_destroy_all_midlets = FALSE;

// The array of applet
LOCAL const CAF_STATIC_APPLET_T s_mmijava_applet_array[] =
{
    {GUID_NAME_DEF(SPRD_JAVA_APPLET_ID), sizeof(MMIJAVA_APPLET_T), JavaApplet_HandleEvent,
        IMAGE_SECMENU_ICON_PASTIME_JAVA, TXT_JAVA},
};

//applet information, will be used in mmi_applet_table.c
LOCAL const CAF_STATIC_APPLET_INFO_T g_mmijava_applet_info =
{
    s_mmijava_applet_array,
    ARR_SIZE(s_mmijava_applet_array)
};

WINDOW_TABLE(MMIJAVA_APPLET_WIN_TAB) =
{
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC((uint32)HandleJavaAppletWinMsg),
    WIN_STYLE(WS_HAS_TRANSPARENT),
    WIN_ID(MMIJAVA_APPLET_WIN_ID),
    END_WIN
};
#endif //MMI_TASK_MANAGER

#ifdef SUPPORT_TASK_MANAGER

WINDOW_TABLE(JBED_TM_LIST_MENU_WIN_TAB) =
{
#ifndef MMI_PDA_SUPPORT
    WIN_TITLE(TXT_APPLICATION_MANAGER),
#else
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIJAVA_TM_LIST_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIJAVA_TM_LIST_CRTL_ID, MMIJAVA_TM_LIST_FORM_CTRL_ID),
#endif
    WIN_FUNC((uint32)Jbed_HandleTMListMenuWinMsg),
    WIN_ID(JBED_TM_LIST_MENU_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIJAVA_TM_LIST_CRTL_ID),
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
LOCAL const uint32 s_tm_java_menu_root_item[][2] =
{
    {JAVA_OPTION_MENU_ID_DELETE, TXT_JAVA_DESTROY},
};

/*****************************************************************************/
//  Description :Get running app's suite id and midlet id and return select index.
//  Global resource dependence :
//  Author: Martin.Zhao
//  Note:
/*****************************************************************************/
LOCAL int MMIJAVA_util_getSelectedRunningAppId(MMI_CTRL_ID_T ctrl_id, int * suite_id, int * midlet_id)
{
    uint16      selectIndex;
    RunningApp* raInfo = NULL;

    if (suite_id == NULL || midlet_id == NULL)
    {
        return -1;
    }

    selectIndex = GUILIST_GetCurItemIndex(ctrl_id);

    Jbed_tm_queryRunningAppInfo(&raInfo);

    if (raInfo == NULL)
    {
        *suite_id = -1;
        *midlet_id = -1;
        return -1;
    }

    *suite_id = raInfo[selectIndex].sid;
    *midlet_id = raInfo[selectIndex].mid;

    SCI_FREE(raInfo);

    return selectIndex;
}
/*****************************************************************************/
//  Description : deal with long Pop up .
//  Global resource dependence : none
//  Author: Erin Fan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTMJavaLongPopUpMsg(
                                             MMI_WIN_ID_T        win_id,
                                             MMI_MESSAGE_ID_E    msg_id,
                                             DPARAM              param
                                             );

/*****************************************************************************/
//  Description :Destroy Midlet  from TaskManager
//  Global resource dependence : none
//  Author: Erin Fan
//  Note:
/*****************************************************************************/
LOCAL void MMIJava_TMList_Destroy(void);

/*****************************************************************************/
//  Description : Delete midlet from TaskManager message handler
//  Global resource dependence : none
//  Author: Erin Fan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIJava_TMList_destroy_callBack
                                        (
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                        );

WINDOW_TABLE(JBED_TM_LONG_POPUP_OPT_MENU_WIN_TAB)=
{
    WIN_FUNC( (uint32)HandleTMJavaLongPopUpMsg),
    WIN_ID(JBED_LONG_POPUP_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    END_WIN
};
#endif

/*****************************************************************************/
//  Description : Handle task manager message.
//  Global resource dependence : none
//  Author: martin zhao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Jbed_HandleTMListMenuWinMsg
(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMIJAVA_TM_LIST_CRTL_ID;
    GUILIST_NEED_ITEM_DATA_T    *item_data_ptr = PNULL;
    GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
#ifdef MMI_PDA_SUPPORT
    MMI_CTRL_ID_T          list_ctrl_id = MMIJAVA_TM_LIST_CRTL_ID;
    GUIFORM_CHILD_HEIGHT_T tm_height = {0};
#endif

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
            tm_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
            GUILIST_PermitBorder(list_ctrl_id, FALSE);
            GUIFORM_IsSlide(MMIJAVA_TM_LIST_FORM_CTRL_ID, FALSE);
            GUIFORM_SetChildDisplay(MMIJAVA_TM_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
            GUIFORM_SetChildHeight(MMIJAVA_TM_LIST_FORM_CTRL_ID, list_ctrl_id, &tm_height);
            GUIFORM_PermitChildBg(MMIJAVA_TM_LIST_FORM_CTRL_ID, FALSE);
#endif
            MMIJava_tm_openRunningAppList(win_id, ctrl_id);

            break;

        case MSG_CTL_LIST_NEED_ITEM_DATA:
            item_data_ptr = (GUILIST_NEED_ITEM_DATA_T *)param;
            if(PNULL != item_data_ptr);
                MMIJava_tm_appendRunningAppData2List(TRUE,item_data_ptr->item_index,0,ctrl_id);
            break;

        case MSG_CTL_LIST_NEED_ITEM_CONTENT:
            item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
            if(PNULL != item_content_ptr);
                MMIJava_tm_appendRunningAppData2List(FALSE, item_content_ptr->item_index, item_content_ptr->item_content_index, ctrl_id);
            break;

        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            if (Jbed_tm_queryRunningAppInfo(NULL) > 0)
            {
                int sid = 0;
                int mid = 0;
                MMI_STRING_T prompt = {0};

                MMI_GetLabelTextByLang(STR_JAVA_CLOSING, &prompt);
                MMIJAVA_util_getSelectedRunningAppId(ctrl_id, &sid, &mid);

                MMIJAVA_util_openWaitingWin(&prompt);
                Jbed_nAms_destroy(sid, mid);
            }
            else
            {
                SCI_TRACE_LOW("Jbed_HandleTMListMenuWinMsg:Error-Jbed_tm_queryRunningAppInfo return 0");
            }
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
            if (Jbed_tm_queryRunningAppInfo(NULL) > 0)
            {
                int sid = 0;
                int mid = 0;

                MMIJAVA_util_getSelectedRunningAppId(ctrl_id, &sid, &mid);
                Jbed_nAms_launch(sid, mid);
            }
            break;

#ifdef MMI_PDA_SUPPORT
        case MSG_CTL_LIST_LONGOK:
            {
                uint32                  i = 0;
                uint32                  item_num = 0;
                uint16                  nodex_index = 0;
                MMI_STRING_T            kstring = {0};
                GUIMENU_DYNA_ITEM_T     node_item = {0};
                const GUILIST_ITEM_T*   item_ptr = PNULL;
                MMI_STRING_T            title_str = {0};
                uint32                  menu_ctrl_id = MMIJAVA_MAIN_OPTION_CRTL_ID;

                item_num = sizeof(s_tm_java_menu_root_item)/sizeof(*s_tm_java_menu_root_item);
                nodex_index = 0;
                menu_ctrl_id = MMIJAVA_MAIN_OPTION_CRTL_ID;
                MMK_CreateWin((uint32 *)JBED_TM_LONG_POPUP_OPT_MENU_WIN_TAB, (ADD_DATA)menu_ctrl_id);
                GUIMENU_CreatDynamic(PNULL, JBED_LONG_POPUP_OPT_MENU_WIN_ID, MMIJAVA_MAIN_OPTION_CRTL_ID, GUIMENU_STYLE_POPUP_AUTO);
                for(i = 0; i<item_num; i++)
                {
                    MMI_GetLabelTextByLang(s_tm_java_menu_root_item[i][1], &kstring);
                    node_item.item_text_ptr = &kstring;
                    GUIMENU_InsertNode(nodex_index, s_tm_java_menu_root_item[i][0], 0, &node_item, MMIJAVA_MAIN_OPTION_CRTL_ID);
                    nodex_index++;
                }

                item_ptr = GUILIST_GetItemPtrByIndex(MMIJAVA_TM_LIST_CRTL_ID, GUILIST_GetCurItemIndex(ctrl_id));
                title_str = item_ptr->item_data_ptr->item_content[1].item_data.text_buffer;
                GUIMENU_SetMenuTitle(&title_str, MMIJAVA_MAIN_OPTION_CRTL_ID);
            }
            break;
#endif

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
#ifdef MMI_PDA_SUPPORT
            MMK_CloseWin(JBED_APP_WIN_ID);
#else
            MMK_CloseWin(win_id);
#endif
            break;

        case MSG_JAVA_NOTIFY_TASKLIST_UPDATE:
            MMIJava_tm_openRunningAppList(win_id, ctrl_id);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    //to be implement
    return result;
}

/*****************************************************************************/
//  Description : Process MSG_OPEN_WINDOW message and Init task manager app list.
//  Global resource dependence : none
//  Author: martin zhao
//  Note:
/*****************************************************************************/
LOCAL void MMIJava_tm_openRunningAppList(
                                           MMI_WIN_ID_T win_id,
                                           MMI_CTRL_ID_T ctrl_id
                                          )
{
    int  i;
    int  midlet_count;
    GUILIST_ITEM_T item          = {0x0};
    GUILIST_ITEM_DATA_T item_data = {0x0};

    midlet_count = Jbed_tm_queryRunningAppInfo(NULL);

    if (GUILIST_GetTotalItemNum(ctrl_id) > 0)
    {
        GUILIST_RemoveAllItems(ctrl_id);
    }
    GUILIST_SetSyncDecodeAnim(ctrl_id, TRUE);
    GUILIST_SetDefaultIcon(ctrl_id, JAVA_ICON, JAVA_ICON);
    GUILIST_SetMaxItem(ctrl_id, midlet_count, TRUE);
    item.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item.item_data_ptr = &item_data;

    for (i = 0; i < midlet_count; i++)
    {
        GUILIST_AppendItem(ctrl_id, &item);
    }

    GUILIST_SetCurItemIndex(ctrl_id, 0);
    MMK_SetAtvCtrl(win_id, ctrl_id);
    if (midlet_count == 0)
    {
        GUIWIN_SetSoftkeyTextId(win_id, COMMON_TXT_NULL, COMMON_TXT_NULL, STXT_RETURN, FALSE);
    }
    else
    {
#ifdef SUPPORT_BACKGROUND_RUN
        GUIWIN_SetSoftkeyTextId(win_id, TXT_JAVA_DESTROY, TXT_JAVA_GOTO_FOREGROUND, STXT_RETURN, FALSE);
#endif
    }
}

/*****************************************************************************/
//  Description : Append running app list to taks manager list.
//  Global resource dependence : none
//  Author: martin zhao
//  Note:
/*****************************************************************************/
LOCAL void MMIJava_tm_appendRunningAppData2List( BOOLEAN         is_text,
                                                  uint16          item_index,
                                                  uint16          content_index,
                                                  MMI_CTRL_ID_T   ctrl_id
                                                 )
{
    GUILIST_ITEM_DATA_T  item_data    = {0x0};
    RunningApp *         run_app_ptr = NULL;
    GUIANIM_FILE_INFO_T  pngPtr = {0x0};

    Jbed_tm_queryRunningAppInfo(&run_app_ptr);
    if (NULL == run_app_ptr)
    {
        SCI_TRACE_LOW("MMIJava_tm_appendRunningAppData2List run_app_ptr is NULL!");
        return;
    }

    if (is_text)
    {
        //set icon information
        if (run_app_ptr[item_index].pngPtr.full_path_wstr_ptr  == NULL)
        {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[0].item_data.image_id = JAVA_ICON;
        }
        else
        {
            pngPtr.full_path_wstr_len = 0;
            pngPtr.full_path_wstr_ptr = NULL;
            item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
            item_data.item_content[0].item_data.anim_path_ptr = &pngPtr;
        }

        //set text information
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = run_app_ptr[item_index].text_buffer.wstr_len;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = run_app_ptr[item_index].text_buffer.wstr_ptr;

        GUILIST_SetItemData(ctrl_id, &item_data, item_index);
    }
    else
    {
        //set icon information
        if (run_app_ptr[item_index].pngPtr.full_path_wstr_ptr  == NULL)
        {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[0].item_data.image_id = JAVA_ICON;
        }
        else
        {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
            item_data.item_content[0].item_data.anim_path_ptr = &(run_app_ptr[item_index].pngPtr);
        }
        GUILIST_SetItemContent(ctrl_id, &item_data.item_content[0], item_index, content_index);
    }

    SCI_FREEIF(run_app_ptr);
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : HandleTMJavaLongPopUpMsg
//  Global resource dependence : none
//  Author: Erin Fan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTMJavaLongPopUpMsg(
                                             MMI_WIN_ID_T        win_id,
                                             MMI_MESSAGE_ID_E    msg_id,
                                             DPARAM              param
                                             )
{
    MMI_RESULT_E   result  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  ctrl_id = 0;
    uint32         node_id = JAVA_OPTION_MENU_ID_DELETE;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            break;

        case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
            ctrl_id = (MMI_CTRL_ID_T)MMK_GetWinAddDataPtr(win_id);
            node_id = GUIMENU_GetCurNodeId(ctrl_id);
            switch (node_id)
            {
                case JAVA_OPTION_MENU_ID_DELETE:
                    MMIJava_TMList_Destroy();
                    break;

                default:
                    break;
            }
            MMK_CloseWin(win_id);
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return result;
}

static MMI_RESULT_E Jbed_HandleJbedAppWinMsg(
                                         MMI_WIN_ID_T      win_id,
                                         MMI_MESSAGE_ID_E  msg_id,
                                         DPARAM            param
                                     )
{
    MMI_IMAGE_ID_T g_s_java_focustab_icon[] =
    {
        IMAGE_JAVA_JAVALIST_FOCUSED,
        IMAGE_JAVA_TASKMANAGER_FOCUSED,
    };

    MMI_STRING_T    str_text = {0};
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    int child_param = -1;


    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:

        MMK_SetAtvCtrl(win_id, MMIJAVA_TAB_CTRL_ID);
        GUITAB_SetMaxItemNum(MMIJAVA_TAB_CTRL_ID, 2);
        MMI_GetLabelTextByLang(TXT_APP_MANAGER, &str_text);
        GUITAB_AppendSimpleItem(
                    MMIJAVA_TAB_CTRL_ID,
                    &str_text,
                    g_s_java_focustab_icon[0],
                    g_s_java_focustab_icon[0]
                    );
        GUITAB_AddChildWindow(
                    MMIJAVA_TAB_CTRL_ID,
                    MMIJAVA_AMS_WIN_ID,
                    0);
        MMI_GetLabelTextByLang(TXT_APPLICATION_MANAGER, &str_text);
        GUITAB_AppendSimpleItem(
                    MMIJAVA_TAB_CTRL_ID,
                    &str_text,
                    g_s_java_focustab_icon[1],
                    g_s_java_focustab_icon[1]
                    );
        GUITAB_AddChildWindow(
                    MMIJAVA_TAB_CTRL_ID,
                    JBED_TM_LIST_MENU_WIN_ID,
                    1);
        GUITAB_SetCurSel(MMIJAVA_TAB_CTRL_ID, 0);

        MMK_CreateChildWin(
            JBED_APP_WIN_ID,
            (uint32*)MMIJAVA_AMS_WIN_TAB,
            (ADD_DATA)child_param);
        MMK_CreateChildWin(
            JBED_APP_WIN_ID,
            (uint32*)JBED_TM_LIST_MENU_WIN_TAB,
            NULL);
        MMK_SetChildWinFocus(JBED_APP_WIN_ID, MMIJAVA_AMS_WIN_ID);
        break;

    case MSG_GET_FOCUS:
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        break;

    case MSG_NOTIFY_TAB_SWITCH:
        if(1 == GUITAB_GetCurSel(MMIJAVA_TAB_CTRL_ID))
        {
            MMK_SendMsg(JBED_TM_LIST_MENU_WIN_ID, MSG_JAVA_NOTIFY_TASKLIST_UPDATE, NULL);
        }
        break;

    case MSG_APP_MENU:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
         /*Entrance of pop munu*/
        if(0 == GUITAB_GetCurSel(MMIJAVA_TAB_CTRL_ID))
        {
            MMIJAVA_OpenMeunPubWin(JAVA_MAIN_POPUP_MENU,GUIMENU_STYLE_OPTIONS);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        break;

    //case MSG_APP_CANCEL:
    //case MSG_CTL_CANCEL:
    //    MMK_CloseWin(win_id);
    //    break;

    case MSG_CLOSE_WINDOW:
        MMIJAVA_NotifyJVMExit();
        if(!MMIAPIJAVA_IsJavaRuning())
        {
            Jbed_nAms_finalize();
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}



/*****************************************************************************/
//  Description :handle the message of opening suiteList
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Jbed_HandleSuiteListMenuWinMsg(
                                               MMI_WIN_ID_T win_id,
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM param
                                           )
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIJAVA_SUITE_LIST_CTRL_ID;
    static int suite_id = 0;
    void   *select_ptr = NULL;
    MMI_CTRL_ID_T          list_ctrl_id = MMIJAVA_SUITE_LIST_CTRL_ID;
    GUIFORM_CHILD_HEIGHT_T child_height = {0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        select_ptr = MMK_GetWinAddDataPtr(win_id);
        suite_id = (int)select_ptr;
        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUILIST_PermitBorder(list_ctrl_id, FALSE);
        GUIFORM_IsSlide(MMIJAVA_SUITE_LIST_FORM_CTRL_ID, FALSE);
        GUIFORM_SetChildDisplay(MMIJAVA_SUITE_LIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildHeight(MMIJAVA_SUITE_LIST_FORM_CTRL_ID, list_ctrl_id, &child_height);
        GUIFORM_PermitChildBg(MMIJAVA_SUITE_LIST_FORM_CTRL_ID, FALSE);
        MMIJAVA_AMSOpenList(win_id, ctrl_id, suite_id);
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        if(suite_id < 0)
            MMIJAVA_AMSUpdateList(msg_id, param,MMK_IsFocusWin(win_id));
        else
            MMIJAVA_MidletUpdateList(msg_id, param, suite_id,MMK_IsFocusWin(win_id));
        break;

    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        if(suite_id < 0)
            MMIJAVA_AMSUpdateList(msg_id, param,MMK_IsFocusWin(win_id));
        else
            MMIJAVA_MidletUpdateList(msg_id, param, suite_id,MMK_IsFocusWin(win_id));
        break;

    case MSG_APP_MENU:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
         /*Entrance of pop munu*/
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        //run directly after select key or click the item.
        break;

    case MSG_CTL_LIST_LONGOK:
    {
#if 0
        uint32                  i = 0;
        uint32                  item_num = 0;
        uint16                  nodex_index = 0;
        int                     select_suite_id = 0;
        MMI_STRING_T            kstring = {0};
        GUIMENU_DYNA_ITEM_T     node_item = {0};
        const GUILIST_ITEM_T*         item_ptr = PNULL;
        MMI_STRING_T            title_str = {0};
        uint32                  menu_ctrl_id = MMIJAVA_MAIN_OPTION_CRTL_ID;

        item_num = TABLE_SIZE(s_java_suite_menu_root_item);
        nodex_index = 0;
        menu_ctrl_id = MMIJAVA_MAIN_OPTION_CRTL_ID;
        MMK_CreateWin((uint32 *)JBED_LONG_POPUP_OPT_MENU_WIN_TAB,(ADD_DATA)menu_ctrl_id);
        GUIMENU_CreatDynamic(PNULL,JBED_LONG_POPUP_OPT_MENU_WIN_ID, MMIJAVA_MAIN_OPTION_CRTL_ID,GUIMENU_STYLE_POPUP_AUTO);
        for(i = 0;i<item_num;i++)
        {
            select_suite_id = MMIJAVA_util_getSelectedAppId(ctrl_id);
            MMI_GetLabelTextByLang(s_java_suite_menu_root_item[i][1], &kstring);
            node_item.item_text_ptr = &kstring;
            GUIMENU_InsertNode(nodex_index, s_java_suite_menu_root_item[i][0], 0, &node_item, MMIJAVA_MAIN_OPTION_CRTL_ID);
            if(MMIJAVA_isMenuAvailable(s_java_suite_menu_root_item[i][0], select_suite_id))
            {
                GUIMENU_SetNodeGrayed(TRUE, i, 0, MMIJAVA_MAIN_OPTION_CRTL_ID);
            }
            nodex_index ++;
        }

        item_ptr = GUILIST_GetItemPtrByIndex(MMIJAVA_SUITE_LIST_CTRL_ID, GUILIST_GetCurItemIndex(ctrl_id));
        title_str = item_ptr->item_data_ptr->item_content[1].item_data.text_buffer;
        GUIMENU_SetMenuTitle(&title_str, MMIJAVA_MAIN_OPTION_CRTL_ID);
#endif
        MMIJAVA_OpenMeunPubWin(JAVA_LONG_PRESS_POPUP_MENU,GUIMENU_STYLE_POPUP_AUTO);

    }

        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if(suite_id < 0)
            MMK_CloseWin(JBED_APP_WIN_ID);
        else
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
            suite_id = -1;
            MMK_SendMsg(win_id, MSG_JAVA_TRIGGER_JBED_REFRESH, (DPARAM )suite_id);
        }
        break;

    case MSG_CLOSE_WINDOW:
        break;

    case MSG_JAVA_NOTIFY_DELETED:
    case MSG_JAVA_TRIGGER_JBED_REFRESH:
          if(MSG_JAVA_NOTIFY_DELETED == msg_id)
          {
              suite_id = -1;
          }
          else
          {
              suite_id = (int)param;
          }
        GUILIST_SetCurItemIndex(MMIJAVA_SUITE_LIST_CTRL_ID, 0);
        GUILIST_SetTopItemIndex(MMIJAVA_SUITE_LIST_CTRL_ID, 0);

        MMIJAVA_AMSOpenList(win_id, ctrl_id, suite_id);
        MMK_UpdateScreen();
        MMIPUB_CloseAlertWin();
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description :Destroy Midlet  from TaskManager
//  Global resource dependence : none
//  Author: Erin Fan
//  Note:
/*****************************************************************************/
LOCAL void MMIJava_TMList_Destroy(void)
{
    MMI_STRING_T prompt_text = {0};

    MMI_GetLabelTextByLang(TXT_JAVA_DESTROY, &prompt_text);
#ifndef MMI_GUI_STYLE_MINISCREEN
    MMIPUB_OpenAlertWinByTextPtr(NULL, NULL, &prompt_text, IMAGE_PUBWIN_QUERY,
               NULL, NULL, MMIPUB_SOFTKEY_OKEXIT, MMIJava_TMList_destroy_callBack);
#else
    MMIPUB_OpenAlertWinByTextPtr(NULL, &prompt_text, NULL, IMAGE_PUBWIN_QUERY,
               NULL, NULL, MMIPUB_SOFTKEY_OKEXIT, MMIJava_TMList_destroy_callBack);
#endif
}

/*****************************************************************************/
//  Description : Delete midlet from TaskManager message handler
//  Global resource dependence : none
//  Author: Erin Fan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIJava_TMList_destroy_callBack
                                        (
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                        )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIJAVA_TM_LIST_CRTL_ID;

    switch (msg_id)
    {
        case MSG_APP_OK:
        case MSG_CTL_OK:
        {
            if (Jbed_tm_queryRunningAppInfo(NULL) > 0)
            {
                int sid = 0;
                int mid = 0;
                MMI_STRING_T prompt = {0};

                MMIJAVA_util_getSelectedRunningAppId(ctrl_id, &sid, &mid);
                Jbed_nAms_destroy(sid, mid);
                MMI_GetLabelTextByLang(STR_JAVA_CLOSING, &prompt);
                MMIJAVA_util_openWaitingWin(&prompt);
                MMK_CloseWin(win_id);
            }
        }
        break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
            break;
    }

    return result;
}
#endif

/**************************************************************************************************
  * Description: MMIJAVA_OpenBackgroundSelectWin
  * Input Paras:
  * Output Paras:
  * note : call the func when deleting all
  *************************************************************************************************/
PUBLIC void  MMIJAVA_OpenTaskMangerWin(void)
{
   MMK_CreateWin((uint32*)JBED_APP_WIN_TAB, PNULL);
}

#endif


/*****************************************************************************/
//  Description : Destroy current all running midlets, the vm will be shutdown
//                when the last midlet is destroyed.
//  Global resource dependence :
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_Destroy_AllRunningMIDlets(void)
{
    int i;
    int count = 0;
    RunningApp* raInfo = NULL;
    MMI_STRING_T prompt = {0};

    SCI_TRACE_LOW("===ready to kill running midlet(s)==");
    count = Jbed_tm_queryRunningAppInfo(&raInfo);
    if (count <= 0 || raInfo == NULL)
    {
        SCI_TRACE_LOW("MMIJAVA_Destroy_AllRunningMIDlets warning: There is no available running midlet information!!!");
        return;
    }

    if (!MMK_IsOpenWin(JBED_COMM_WAITING_WIN_ID) && !MMK_IsOpenWin(MMIPUB_ALERT_TEXT_WIN_ID) )
    {
        MMI_GetLabelTextByLang(STR_JAVA_CLOSING, &prompt);
        MMIJAVA_util_openWaitingWin(&prompt);
    }
#ifdef MMI_TASK_MANAGER
    MMIJAVA_SetIsDestroyAllMIDlets(TRUE);
#endif

#ifdef JAVA_SUPPORT_TYPE_SVM
    SCI_TRACE_LOW("===all midlets:exit by request down==");
    Jbed_requestState(JBED_MAIN_DOWN, NULL, NULL);
    Jbed_wakeupVM();
#else
    for (i = 0; i < count; i++)
    {
        Jbed_nAms_destroy(raInfo[i].sid, raInfo[i].mid);
    }
#endif
    SCI_FREE(raInfo);
}

#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : Create java  reference window for MMI_APPLET_STATE_BG_HIDE_WIN
//  Global resource dependence :
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T OpenJavaWin(uint32* win_table_ptr, ADD_DATA add_data_ptr)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = SPRD_JAVA_APPLET_ID;
    win_table_create.win_table_ptr = win_table_ptr;
    win_table_create.add_data_ptr = add_data_ptr;

    return MMK_CreateWinTable(&win_table_create);
}

/**************************************************************************************************/
// Description:applet window message handler
// Date:
// Author:Sean.Xu
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaAppletWinMsg(
                                       MMI_WIN_ID_T      win_id,
                                       MMI_MESSAGE_ID_E  msg_id,
                                       DPARAM            param
                                       )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    SCI_TRACE_LOW("HandleJavaAppletWinMsg:msg_id=%d", msg_id);
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            break;

        case MSG_FULL_PAINT:
            MMK_PostMsg(win_id, MSG_LOSE_FOCUS , PNULL, 0);
            break;

        case MSG_CLOSE_WINDOW:
            MMIAPIJAVA_StopApplet(FALSE);
            break;

        case MSG_GET_FOCUS:
            break;

        case MSG_LOSE_FOCUS:
            MMIAPIJAVA_StopApplet(TRUE);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return result;
}

/*****************************************************************************/
//  Description : the process message function of the JAVA applet
//  Global resource dependence :
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  JavaApplet_HandleEvent(
                                    IAPPLET_T*          iapplet_ptr,
                                    MMI_MESSAGE_ID_E    msg_id,
                                    DPARAM              param
                                    )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIJAVA_APPLET_T* applet_ptr = (MMIJAVA_APPLET_T*)iapplet_ptr;

    SCI_TRACE_LOW("JavaApplet_HandleEvent msg_id = 0x%04x", msg_id);

    switch (msg_id)
    {
        case MSG_APPLET_START:
            s_java_applet_instance = applet_ptr;
            MMK_CreateJavaWin((uint32*)MMIJAVA_APPLET_WIN_TAB, PNULL);
            break;

        // According to the design of application manager, there is no double cheking mechanism
        // for MSG_APPLET_STOP, so we have to destroy all running midlets without querying.
        case MSG_APPLET_STOP:
            MMIJAVA_Destroy_AllRunningMIDlets();
            s_java_applet_instance = PNULL;
            break;

        case MSG_APPLET_RESUME:
            MMIAPIJAVA_OpenTaskMgr();
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return result;
}

/*****************************************************************************/
//  Description : register java module applet info
//  Global resource dependence :
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_RegAppletInfo(void)
{
    MMI_RegAppletInfo(MMI_MODULE_JAVA, &g_mmijava_applet_info);
}

/*****************************************************************************/
//  Description : Return JAVA Applet Instance
//  Global resource dependence :
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC MMIJAVA_APPLET_T *MMIJAVA_GetAppletInstance(void)
{
    return s_java_applet_instance;
}

/*****************************************************************************/
//  Description : Stop java applet
//  Global resource dependence :
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StopApplet(BOOLEAN is_minimize)
{
    if (is_minimize)
    {
        MMK_HideWinByApplet(SPRD_JAVA_APPLET_ID);
    }
    else
    {
        MMK_CloseApplet(SPRD_JAVA_APPLET_ID);
    }
}

/*****************************************************************************/
//  Description : start java applet
//  Global resource dependence :
//  Author:Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StartApplet(void)
{
    MMI_APPLET_START_T start = {0};
    if(NULL == MMK_GetAppletPtr(SPRD_JAVA_APPLET_ID))
    {
        SCI_TRACE_LOW("MMIAPIJAVA_StartApplet");
        start.guid = SPRD_JAVA_APPLET_ID;
        start.state = MMI_APPLET_STATE_BG_HIDE_WIN|MMI_APPLET_STATE_TASK_MANAGER;
        MMK_StartApplet(&start);
    }
    else
    {
        SCI_TRACE_LOW("not need to MMIAPIJAVA_StartApplet");
    }
}

/*****************************************************************************/
//  Description : get the value of g_is_destroy_all_midlets.
//  Global resource dependence :
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_GetIsDestroyAllMIDlets(void)
{
    return g_is_destroy_all_midlets;
}

/*****************************************************************************/
//  Description : set the value of g_is_destroy_all_midlets.
//  Global resource dependence :
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetIsDestroyAllMIDlets(BOOLEAN flag)
{
    g_is_destroy_all_midlets = flag;
}

PUBLIC void MMIJAVA_CloseJavaAppletWin(void)
{
    if(MMK_IsOpenWin(MMIJAVA_APPLET_WIN_ID))
    {
        MMK_CloseWin(MMIJAVA_APPLET_WIN_ID);
    }
}

#endif //MMI_TASK_MANAGER
