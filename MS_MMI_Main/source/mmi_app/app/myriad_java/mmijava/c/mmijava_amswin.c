/****************************************************************************
** File Name:      mmijava_amswin.c                                         *
** Author:         murphy.xie                                               *
** Date:           08/02/2012                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application suite list *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2012        murphy.xie       Create
****************************************************************************/

/**-------------------------------------------------------------------------*
 **                         Include Files                                   *
 **-------------------------------------------------------------------------*/
#include "jbed_exported_defines.h"
#include "window_parse.h"
#include "sci_types.h"
#include "mmijava_id.h"
#include "mmi_appmsg.h"
#include "java_mmi_text.h"
#include "mmijava_amswin.h"
#include "mmi_pubmenuwin.h"
#include "java_mmi_image.h"
#include "mmipub.h"
#include "mmijava_localinstallwin.h"
#include "mmijava_suitefun.h"
#include "mmijava_netsetwin.h"
#include "mmijava_permissionsetwin.h"
#include "mmijava_dlwin.h"
#include "mmijava_aboutwin.h"
#include "mmijava_midletinfowin.h"
#include "mmi_menutable.h"

//*************************************************************************************************/
typedef struct
{
    int32       suite_id; //recored suite id for option operate
    uint32      groud_id; //MENU GROUD ID for different menu
}JAVA_AMSWINDATA_T;//JAVA AMS WINDOWN USER DATA


/**************************************************************************************************/
// Description:HandleJavaAMSWinMsg
// Date:2012-8-15
// Author:murphy.xie
// Note:JAVA 应用列表窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaAMSWinMsg(
                                       MMI_WIN_ID_T      win_id,
                                       MMI_MESSAGE_ID_E  msg_id,
                                       DPARAM            param
                                       );

/**************************************************************************************************/
// Description:HandleJavaAMSMidletListWinMsg
// Date:2012-8-15
// Author:murphy.xie
// Note:JAVA MIDlet列表窗口处理函数，如果应用是包含多个MIDlet的应用
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaAMSMidletListWinMsg(
                                                 MMI_WIN_ID_T        win_id,
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 );




WINDOW_TABLE( MMIJAVA_AMS_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleJavaAMSWinMsg),
    WIN_ID( MMIJAVA_AMS_WIN_ID ),
    //WIN_TITLE(TXT_JAVA_INSTALLED_APPS),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIJAVA_SUITE_LIST_CTRL_ID),
    WIN_SOFTKEY(STXT_SOFTKEY_MENU_MK, TXT_NULL, TXT_COMMON_COMMON_BACK),
    END_WIN
};


WINDOW_TABLE( MMIJAVA_AMS_MIDLETLIST_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleJavaAMSMidletListWinMsg),
    WIN_ID( MMIJAVA_AMS_MIDLETLIST_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIJAVA_MIDLET_LIST_WIN_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_OPEN_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

LOCAL void HandleJavaPubMenuOKHandler (MMIPUB_MENU_NOTIFY_T *param)
{
    MMI_MENU_GROUP_ID_T         group_id = 0;
    MMI_MENU_ID_T               menu_id  = 0;
    int                         suite_id = 0;
    JAVA_AMSWINDATA_T*          win_data_p = PNULL;

    if (PNULL == param)
    {
      return;
    }

    if (0 == param->group_id && 0 == param->menu_id)
    {
        menu_id = GUIMENU_GetCurNodeId (param->ctrl_id);
    }
    else
    {
        GUIMENU_GetId (param->ctrl_id, &group_id, &menu_id);
    }

    win_data_p = (JAVA_AMSWINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (win_data_p == PNULL)
    {
        return;
    }

    suite_id = win_data_p->suite_id;

    switch(menu_id)
    {
        case JAVA_SET_ID_PERMISSION_SETTING:
            MMIJAVA_SetPermisson(suite_id);
            break;
        case JAVA_OPTION_MENU_ID_INFO:
            MMIJAVA_ShowSuiteInfo(suite_id);
            break;
#if defined (NATIVE_AMS) && defined (AMS_TCK_RUNNER)
        case JAVA_OPTION_MENU_ID_TCK_RUNNER:
            MMIJAVA_StartTCK();
            break;
        case JAVA_OPTION_MENU_ID_TCK_NET_SETTING:
            MMIJAVA_OpenNetSettingWin(suite_id);
            break;
#endif
        default:
            SCI_TRACE_LOW("=== Myriad HandleJavaPubMenuOKHandler default: menu_id = %d.", menu_id);
            break;
    }
}

LOCAL void HandleJavaPubMenuInitHandler (MMIPUB_MENU_NOTIFY_T *param)
{
    MMI_CTRL_ID_T               ctrl_id  = 0;
    MMI_MENU_GROUP_ID_T         group_id = 0;
    int                         suite_id = 0;
    JAVA_AMSWINDATA_T*          win_data_p = PNULL;
    BOOLEAN                     is_preinstall = FALSE;
#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
    MMI_STRING_T title_text = {0};
#endif

    if (PNULL == param)
    {
      return ;
    }

    ctrl_id = param->ctrl_id;
    win_data_p = (JAVA_AMSWINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (win_data_p == PNULL)
    {
        return;
    }
    group_id = win_data_p->groud_id;

    // No MIDlet installed, Some option menu should be not availabe ---  grayed
    if(Jbed_nAms_getSuitesCount(NULL, 0) <= 0)
    {
        GUIMENU_SetItemGrayed(ctrl_id,group_id, JAVA_SET_ID_PERMISSION_SETTING, TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id, JAVA_OPTION_MENU_ID_INFO, TRUE);
    }

#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
    MMI_GetLabelTextByLang(STXT_OPTION, &title_text);
    GUIMENU_SetMenuTitle( &title_text,ctrl_id );
#endif
}

/*****************************************************************************/
//  Description : open menu pub win
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL void MMIJAVA_OpenMeunPubWin (
    MMI_MENU_GROUP_ID_T grup_id,
    GUIMENU_STYLE_E   menu_style
)
{

    MMIPUB_MENU_PARAM_T menuparam = {0};

    menuparam.group_id = grup_id;
    menuparam.win_id = JBED_MAIN_OPTION_WIN_ID;
    menuparam.ctrl_id = MMIJAVA_MAIN_OPTION_CRTL_ID;

    menuparam.dst_handle = MMIJAVA_AMS_WIN_ID;
    menuparam.func.OnMenuOk = HandleJavaPubMenuOKHandler;
    menuparam.func.OnMenuInit = HandleJavaPubMenuInitHandler;
    menuparam.menu_style = menu_style;
    MMIPUB_OpenPubMenuWin (&menuparam);
}

/*****************************************************************************/
//  Description : ams windown open control function
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E AmsWin_OpenCtrlFunc(MMI_WIN_ID_T win_id)  //OpenCtrl
{
    JAVA_AMSWINDATA_T *win_data_p =PNULL;

    win_data_p = (JAVA_AMSWINDATA_T*)SCI_ALLOC_APPZ(sizeof(JAVA_AMSWINDATA_T));
    if (win_data_p == PNULL)
    {
        //MMIAPPSAMPLE_RaiseError(1);
        MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }

    MMK_SetWinUserData(win_id, (ADD_DATA)(win_data_p));
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : ams windown open init view function
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E AmsWin_OpenViewFunc(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id)
{
    MMIJAVA_AMSOpenList(win_id,ctrl_id,-1);
    return MMI_RESULT_TRUE;
}


/*****************************************************************************/
//  Description : ams windown update ams list data struct function
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E AmsWin_UpdateData(MMI_WIN_ID_T win_id,void * param)
{
    MMIJAVA_AMSUpdateList(MSG_CTL_LIST_NEED_ITEM_DATA, param,TRUE/*MMK_IsFocusWin(win_id)*/);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : ams windown update ams list content
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E AmsWin_UpdateContent(MMI_WIN_ID_T win_id,void * param)
{
    MMIJAVA_AMSUpdateList(MSG_CTL_LIST_NEED_ITEM_CONTENT, param,TRUE/*MMK_IsFocusWin(win_id)*/);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : ams windown list ok function
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  AmsWin_OKFunc(MMI_WIN_ID_T win_id)
{
    int  suite_id = 0;
    JAVA_AMSWINDATA_T *win_data_p = (JAVA_AMSWINDATA_T*)MMK_GetWinUserData(win_id);

    if(PNULL == win_data_p)
    {
        return  MMI_RESULT_FALSE;
    }

    suite_id = win_data_p->suite_id;
    MMIJAVA_StartSuiteInternal(suite_id,-1);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : ams windown close window control function
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E AmsWin_CloseCtrlFunc(MMI_WIN_ID_T win_id) //Close Ctrl
{

    JAVA_AMSWINDATA_T *win_data_p = (JAVA_AMSWINDATA_T*)MMK_GetWinUserData(win_id);
    if (win_data_p != PNULL)
    {
        SCI_FREE(win_data_p);
    }

    MMIJAVA_NotifyJVMExit();
    if(!MMIAPIJAVA_IsJavaRuning())
    {
        Jbed_nAms_finalize();
    }
    return MMI_RESULT_FALSE;
}

/**************************************************************************************************/
// Description:HandleJavaAMSWinMsg
// Date:2007-06-02
// Author:huangjc
// Note:JAVA 应用列表窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaAMSWinMsg(
                                       MMI_WIN_ID_T      win_id,
                                       MMI_MESSAGE_ID_E  msg_id,
                                       DPARAM            param
                                       )

{
    MMI_RESULT_E                result      = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = (MMI_CTRL_ID_T)MMIJAVA_SUITE_LIST_CTRL_ID;
    JAVA_AMSWINDATA_T*          win_data_p = (JAVA_AMSWINDATA_T*)MMK_GetWinUserData(win_id);
    SCI_TRACE_LOW("HandleJavaAMSWinMsg, msg_id = 0x%02x");

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
    {
        MMIJAVA_UpdateTitle(STR_INSTALLED_APPS_EXT01);
        if (!AmsWin_OpenCtrlFunc(win_id))
        {
            break;
        }

        AmsWin_OpenViewFunc(win_id,ctrl_id);
        break;
    }
    case MSG_GET_FOCUS:
        MMIJAVA_UpdateTitle(STR_INSTALLED_APPS_EXT01);
        AmsWin_OpenViewFunc(win_id,ctrl_id);
        break;

    case MSG_FULL_PAINT:
        break;
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        AmsWin_UpdateData(win_id,param);
        break;

    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        AmsWin_UpdateContent(win_id,param);
        break;
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_WEB:
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
        {
            int ret_id = MMIJAVA_GetSuiteID(GUILIST_GetCurItemIndex(ctrl_id));

            if (ret_id >= 0)
            {
                win_data_p->suite_id = ret_id;
                AmsWin_OKFunc(win_id);
            }
        }
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        win_data_p->suite_id = MMIJAVA_GetSuiteID(GUILIST_GetCurItemIndex(ctrl_id));
        win_data_p->groud_id = JAVA_LONG_PRESS_POPUP_MENU;
        MMIJAVA_OpenMeunPubWin(JAVA_LONG_PRESS_POPUP_MENU,GUIMENU_STYLE_POPUP_AUTO);

        break;
#endif
#ifdef MMI_PDA_SUPPORT
    case MSG_APP_MENU:
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
#endif
        win_data_p->suite_id = MMIJAVA_GetSuiteID(GUILIST_GetCurItemIndex(ctrl_id));
        win_data_p->groud_id = JAVA_MAIN_POPUP_MENU;
        MMIJAVA_OpenMeunPubWin(JAVA_MAIN_POPUP_MENU,GUIMENU_STYLE_POPUP);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
    case MSG_CLOSE_WINDOW:
        AmsWin_CloseCtrlFunc(win_id);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}


/*****************************************************************************/
//  Description : midlet list window init function
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MidletListWin_OpenViewFunc(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T  ctrl_id,int suite_id)
{
    MMIJAVA_AMSOpenList(win_id, ctrl_id, suite_id);
    return MMI_RESULT_TRUE;
}


/*****************************************************************************/
//  Description : midlet list update data function
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MidletListWin_UpdateData(void*  param,int suite_id)
{
    MMIJAVA_MidletUpdateList(MSG_CTL_LIST_NEED_ITEM_DATA, param, suite_id,TRUE);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : midlet list update content function
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MidletListWin_UpdateContent(void * param,int suite_id)
{
    MMIJAVA_MidletUpdateList(MSG_CTL_LIST_NEED_ITEM_CONTENT, param, suite_id,TRUE);
    return MMI_RESULT_TRUE;
}

/**************************************************************************************************/
// Description:HandleJavaAMSMidletListWinMsg
// Date:2007-06-02
// Author:huangjc
// Note:JAVA MIDlet列表窗口处理函数，如果应用是包含多个MIDlet的应用
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaAMSMidletListWinMsg(
                                                 MMI_WIN_ID_T        win_id,
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 )
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = (MMI_CTRL_ID_T)MMIJAVA_MIDLET_LIST_WIN_CTRL_ID;
    int           suite_id = (int)MMK_GetWinAddDataPtr(win_id);


    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MidletListWin_OpenViewFunc(win_id,ctrl_id,suite_id);
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        MidletListWin_UpdateData(param,suite_id);
        break;

    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        MidletListWin_UpdateContent(param,suite_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
             uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
             switch (src_id)
             {
                 case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                     MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                     break;
                 case MMIJAVA_MIDLET_LIST_WIN_CTRL_ID:
                     //MMIJAVA_midlet_Launch(win_id, ctrl_id, suite_id);
                     MMIJAVA_StartSuiteInternal(suite_id,GUILIST_GetCurItemIndex(ctrl_id)+1);
                     break;
                 default:
                     break;
             }

         }
         break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_OK:
    //case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        MMIJAVA_StartSuiteInternal(suite_id,GUILIST_GetCurItemIndex(ctrl_id)+1);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_GET_FOCUS:
        MidletListWin_OpenViewFunc(win_id,ctrl_id,suite_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (!MMK_IsOpenWin(MMIJAVA_AMS_WIN_ID))
        {
            MMIJAVA_NotifyJVMExit();
            if(!MMIAPIJAVA_IsJavaRuning())
            {
                Jbed_nAms_finalize();
            }
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    //to be implement
    return result;
}



/*****************************************************************************/
//  Description :add items to listbox according to the count of midlet in a suite
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_AMSOpenList(
                                      MMI_WIN_ID_T    win_id,
                                      MMI_CTRL_ID_T   ctrl_id,
                                      int             suite_id
                                  )
{
    int     i;
    int     count;
    wchar* name_content = NULL;
    int     name_size = 0;
    int     CurItemIndex = 0;
    GUILIST_ITEM_T item = {0x0};
    GUILIST_ITEM_DATA_T item_data = {0x0};

    if(suite_id >= 0) // suite id is starting from 0.
        count = Jbed_nAms_getMIDletsCount(suite_id);
    else
        count  = Jbed_nAms_getSuitesCount(NULL, 0);

    CurItemIndex = GUILIST_GetCurItemIndex(ctrl_id);

    if (GUILIST_GetTotalItemNum(ctrl_id) > 0)
    {
        GUILIST_RemoveAllItems(ctrl_id);
    }
    GUILIST_SetSyncDecodeAnim(ctrl_id, TRUE);
    GUILIST_SetDefaultIcon(ctrl_id, JAVA_ICON, JAVA_ICON);
    GUILIST_SetMaxItem(ctrl_id, count, TRUE);
    GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
    item.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT_MS;
    item.item_data_ptr = &item_data;

    for (i = 0; i < count; i++)
    {
        GUILIST_AppendItem(ctrl_id, &item);
    }

    GUILIST_SetCurItemIndex(ctrl_id, CurItemIndex>count?count:CurItemIndex);
    MMK_SetAtvCtrl(win_id, ctrl_id);

    if(suite_id >= 0)
    {
        //set Suite name as the title of MIDlet list.
        name_size = Jbed_nAms_querySuitesInfo(suite_id, (int)NAMS_SI_NAME, (void *)&name_content);
        if(MMIAPICOM_StatusAreaSetViewHeaderinfo(name_content, name_size))
        {
            GUIWIN_UpdateStb();
        }
        SCI_FREEIF(name_content);
    }

}
/*****************************************************************************/
//  Description :add suites info to listbox,include icon and name
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_AMSUpdateList(
                                           MMI_MESSAGE_ID_E msg_id,
                                           void * param,
                                           BOOLEAN isFocus      //window is focus or not
                                        )
{
    GUILIST_ITEM_DATA_T item_data = {0x0};
    GUILIST_NEED_ITEM_CONTENT_T *item_content = NULL;
    MMI_CTRL_ID_T ctrl_id = 0;

    int  item_index, item_content_index, i;
    int *list_ids = NULL;
    uint32  item_count = 0;
    GUIANIM_FILE_INFO_T  png_ptr = {0x0};
    wchar * name_content = NULL;
    wchar * icon_path    = NULL;
    int    name_size = 0;
    int    icon_size = 0;

    //add pointer-check
    item_content = (GUILIST_NEED_ITEM_CONTENT_T*)param;
    if(NULL ==item_content)
    {
        SCI_TRACE_LOW("=== MMIJAVA_AMSUpdateList,param is null");
        return;
    }
    ctrl_id = item_content->ctrl_id;
    //first invoke this function,list_ids is null,and store suite_id info in list_ids
    item_count = GUILIST_GetTotalItemNum(ctrl_id);
    list_ids = (int*)SCI_ALLOCA(item_count * sizeof(uint32));
    if (list_ids == NULL)
    {
        SCI_TRACE_LOW("MMIJava_util_openSuitesList - ERROR: alloc memory failure");
        return;
    }
    Jbed_nAms_getSuitesCount(list_ids, item_count);

    //get suite_id from list_ids according to item_index
    item_index = item_content->item_index;
    item_content_index = item_content->item_content_index;

    if (msg_id == MSG_CTL_LIST_NEED_ITEM_DATA)
    {
        name_size = Jbed_nAms_querySuitesInfo(list_ids[item_index], NAMS_SI_NAME, (void *)&name_content);

        if(isFocus)
        {
            icon_size = Jbed_nAms_querySuitesInfo(list_ids[item_index], NAMS_SI_ICON, (void *)&icon_path);
        }
        else
        {
            SCI_TRACE_LOW("===== lose focus,set default icon 1 ====");
            icon_size = 0;   //if window not on focus,use default icon.
        }

        //set icon information
        if(icon_size > 0)
        {
            png_ptr.full_path_wstr_len = icon_size;
            png_ptr.full_path_wstr_ptr = icon_path;
            item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
            item_data.item_content[0].item_data.anim_path_ptr = &png_ptr;
        }
        else
        {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[0].item_data.image_id = JAVA_ICON;
        }

        //set text information
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

        if(NULL == name_content)
        {
            item_data.item_content[1].item_data.text_buffer.wstr_len = 4; //wstrlen(L"NULL")
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = L"NULL";
        }
        else
        {
            item_data.item_content[1].item_data.text_buffer.wstr_len = name_size;
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = name_content;
        }

        //set softkey information
        item_data.softkey_id[0] = STXT_SOFTKEY_MENU_MK;
        item_data.softkey_id[1] = STXT_SOFTKEY_OPEN_MK;

        if (Jbed_nAms_getMIDletsCount(list_ids[item_index]) == 1)
        {
#if defined(SUPPORT_BACKGROUND_RUN) && defined(JBED_MVM)
            if(Jbed_tm_queryRunningAppByID(list_ids[item_index], 1))
                item_data.softkey_id[1] = TXT_JAVA_GOTO_FOREGROUND;
            else
#endif
                item_data.softkey_id[1] = STXT_SOFTKEY_OPEN_MK;
        }

        item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;

        GUILIST_SetItemData(ctrl_id, &item_data, item_index);
        SCI_FREEIF(name_content);

        if(PNULL != icon_path)
        {
            SCI_FREEIF(icon_path);
        }
    }
    else if (msg_id == MSG_CTL_LIST_NEED_ITEM_CONTENT)
    {
        if(isFocus)
        {
            icon_size = Jbed_nAms_querySuitesInfo(list_ids[item_index], NAMS_SI_ICON, (void *)&icon_path);
        }
        else
        {
            SCI_TRACE_LOW("===== lose focus,set default icon 2 ====");
            icon_size = 0 ;
        }

        //set icon information
        if(icon_size > 0)
        {
            png_ptr.full_path_wstr_len = icon_size;
            png_ptr.full_path_wstr_ptr = icon_path;
            item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
            item_data.item_content[0].item_data.anim_path_ptr = &png_ptr;
            GUILIST_SetItemContent(ctrl_id, &item_data.item_content[0], item_index, item_content_index);
        }
        else
        {
            SCI_TRACE_LOW("===== set icon image fail ====");
        }

        if(PNULL != icon_path)
        {
            SCI_FREEIF(icon_path);
        }
    }

    SCI_FREEIF(list_ids);
    item_count = 0;
}


/*****************************************************************************/
//  Description :add info midlets in a suite to listbox,include icon and name
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_MidletUpdateList(
                                             MMI_MESSAGE_ID_E msg_id,
                                             void * param,
                                             int suite_id,
                                             BOOLEAN isFocus
                                         )
{
    GUILIST_ITEM_DATA_T item_data = {0x0};
    GUILIST_NEED_ITEM_CONTENT_T *item_content = NULL;
    MMI_CTRL_ID_T ctrl_id = 0;

    int  item_index =0, item_content_index =0;
    int  item_count = 0;
    GUIANIM_FILE_INFO_T  png_ptr = {0x0};
    wchar * name_content = NULL;
    wchar * icon_path    = NULL;
    int    name_size = 0;
    int    icon_size = 0;

    item_content = (GUILIST_NEED_ITEM_CONTENT_T*)param;
    if(NULL ==item_content)
    {
        SCI_TRACE_LOW("===== MMIJAVA_MidletUpdateList,param is null ====");
        return;
    }
    ctrl_id = item_content->ctrl_id;

    if (msg_id == MSG_CTL_LIST_NEED_ITEM_DATA)
    {
        item_index = item_content->item_index;
        item_content_index = item_content->item_content_index;
        name_size = Jbed_nAms_queryMIDletInfoByIdx(suite_id, item_index, NAMS_MI_MIDLET, (void *)&name_content);

        if(isFocus)
        {
            //nix.long(2012.03.02) paras 2 item_index is not be assigned,pay attention here
            icon_size = Jbed_nAms_queryMIDletInfoByIdx(suite_id, item_index, NAMS_MI_ICON, (void *)&icon_path);
        }
        else
        {
            icon_size = 0;
        }

        //set icon information
        if(icon_size > 0)
        {
            png_ptr.full_path_wstr_len = icon_size<<1;
            png_ptr.full_path_wstr_ptr = icon_path;
            item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
            item_data.item_content[0].item_data.anim_path_ptr = &png_ptr;
        } else {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[0].item_data.image_id = JAVA_ICON;
        }

        //set text information
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

        if(NULL == name_content)
        {
            item_data.item_content[1].item_data.text_buffer.wstr_len = 4; //wstrlen(L"NULL")
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = L"NULL";
        }
        else
        {
            item_data.item_content[1].item_data.text_buffer.wstr_len = name_size;
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = name_content;
        }

        //set softkey information
#if defined(SUPPORT_BACKGROUND_RUN) && defined(JBED_MVM)
        if(Jbed_tm_queryRunningAppByID(suite_id, item_index + 1))
            item_data.softkey_id[1] = TXT_JAVA_GOTO_FOREGROUND;
        else
#endif
        item_data.softkey_id[1] = STXT_SOFTKEY_OPEN_MK;
        item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;

        GUILIST_SetItemData(ctrl_id, &item_data, item_index);
        SCI_FREEIF(name_content);
        if(NULL != icon_path)
            SCI_FREEIF(icon_path);
    }
    else if (msg_id == MSG_CTL_LIST_NEED_ITEM_CONTENT)
    {
        item_count = GUILIST_GetTotalItemNum(ctrl_id);
        item_index = item_content->item_index;
        item_content_index = item_content->item_content_index;

        if(isFocus)
        {
            icon_size = Jbed_nAms_queryMIDletInfoByIdx(suite_id, item_index, NAMS_MI_ICON, (void *)&icon_path);
        }
        else
        {
            icon_size = 0;
        }

        //set icon information
        if(icon_size > 0)
        {
            png_ptr.full_path_wstr_len = (uint16)(icon_size<<1);
            png_ptr.full_path_wstr_ptr = icon_path;
            item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
            item_data.item_content[0].item_data.anim_path_ptr = &png_ptr;
        }
        else
        {
            SCI_TRACE_LOW("===== set icon image fail 2 ====");
        }

        GUILIST_SetItemContent(ctrl_id, &item_data.item_content[0], item_index, item_content_index);
        if(NULL != icon_path)
            SCI_FREEIF(icon_path);
    }
}


/**************************************************************************************************
  * Description: MMIJAVA_EnterAmsWin
  * Input Paras:
  * Output Paras:
  * note : call the func when deleting all
  *************************************************************************************************/
PUBLIC void MMIJAVA_EnterAmsWin(void)
{
    MMK_CreateWin((uint32*)MMIJAVA_AMS_WIN_TAB, PNULL);
}


/**************************************************************************************************
  * Description: MMIJAVA_EnterMidletListWin
  * Input Paras:
  * Output Paras:
  * note : call the func when deleting all
  *************************************************************************************************/
PUBLIC void  MMIJAVA_EnterMidletListWin(uint32 suiteid)
{
    SCI_TRACE_LOW("MMIJAVA_EnterMidletListWin suiteid = %d",suiteid);
    MMK_CreateWin((uint32*)MMIJAVA_AMS_MIDLETLIST_WIN_TAB, (ADD_DATA)suiteid);
}

