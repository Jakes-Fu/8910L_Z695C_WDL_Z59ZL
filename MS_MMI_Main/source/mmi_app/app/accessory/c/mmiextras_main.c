/****************************************************************************
** File Name:      mmiextras_main.c
** Author:
** Date:            2007/12/14
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.
** Description:    This file is used to process msg to main window of extra app
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION
** 12/14/2004     zhaohui          Create
**
****************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_accessory_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "dal_time.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "guibutton.h"
#include "mmidisplay_data.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "mmicc_export.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
#include "mmialarm_export.h"

#include "mmi_default.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiacc_id.h"
#include "mmiacc_image.h"
#include "mmiacc_text.h"
#include "mmiacc_position.h"
#include "mmialarm_image.h"
#include "mmicountedtime_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_id.h"
#include "mmiidle_export.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "guires.h"
#include "mmikl_export.h"
#include "guiedit.h"
#include "guistring.h"
#include "mmiidle_subwintab.h"
//#include "guictrl_api.h"
#include "mmi_theme.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif
#include "mmipicview_export.h"
#ifdef VT_SUPPORT
#include "mmivt_export.h"
#endif

#include "mmi_timer_export.h"

#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif

#include "guictrl_api.h"
#ifdef MMI_GRID_IDLE_SUPPORT
#include "mmiwidget_export.h"
#endif
#include "guitab.h"
#include "mmiatv_export.h"
#include "guiedit.h"
//added by du.yuxuan
#include "mmiextras_export.h"
#include "mmistopwatch_export.h"
#ifndef WIN32
#include "gpio_prod_api.h"
#endif
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
//end


typedef enum{
    ID_EXTRAS_MAIN_MENU_CT,
    ID_EXTRAS_MAIN_MENU_SW,
    ID_EXTRAS_MAIN_MENU_FLASHL,
    ID_EXTRAS_MAIN_MENU_UT,
    ID_EXTRAS_MAIN_MENU_VR
};


typedef enum{
    CT_SEL_MENU_NODE_ROOT,
    CT_SEL_MENU_TIMER_EDIT,
    CT_SEL_MENU_TIMER_STOP
};

PUBLIC void MMIAPIEXTRAS_OpenMainWin(void);

LOCAL MMI_RESULT_E  HandleExtrasMainWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
);
LOCAL void AddExtrasListCtrl(uint16 current_item);
LOCAL void AppendExtrasItem2Line(
                                 MMI_CTRL_ID_T  ctrl_id,
                                 MMI_TEXT_ID_T  text_1,
                                 MMI_TEXT_ID_T  text_2 
);
LOCAL void AppendFlashlightItem(
                                MMI_CTRL_ID_T  ctrl_id,
                                MMI_TEXT_ID_T  text_1);
LOCAL void AppendExtrasItem1Line(
                                 MMI_CTRL_ID_T  ctrl_id,
                                 MMI_TEXT_ID_T  text_1
);
LOCAL void InsertMenuNode(
                                MMI_CTRL_ID_T       ctrl_id,        //控件id
                                MMI_TEXT_ID_T       text_id,        //插入的节点的文本
                                uint16              node_id,        //节点编号
                                uint16              parent_node_id, //父节点编号
                                uint16              index
                                 );
LOCAL void CreateSimSelectionMenu(
                                MMI_WIN_ID_T win_id,
                                MMI_CTRL_ID_T ctrl_id);

LOCAL MMI_RESULT_E HandleCTSelMenuWindow(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param

);

WINDOW_TABLE(EXTRAS_MAIN_WIN_TAB)={
    WIN_FUNC((uint32) HandleExtrasMainWinMsg ),
    WIN_ID( MMIEXTRAS_MAIN_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIEXTRAS_MAIN_MENU_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_OPEN_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE(EXTRAS_SEL_MENU_WIN_TAB)={
    WIN_FUNC((uint32) HandleCTSelMenuWindow ),
    WIN_ID( MMIEXTRAS_CT_SEL_MENU_WIN_ID ),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

PUBLIC void MMIAPIEXTRAS_OpenMainWin(void)
{
        MMK_CreateWin((uint32 *)EXTRAS_MAIN_WIN_TAB,PNULL);
}

LOCAL MMI_RESULT_E  HandleExtrasMainWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_CTRL_ID_T ctrl_id = MMIEXTRAS_MAIN_MENU_LIST_CTRL_ID;
    uint16 cur_item = 0;
    MMI_STRING_T cur_time = {0};
    wchar time_temp[9]  = {0};
    MMI_TEXT_ID_T text_id = STR_HDR_APPS_EXTRAS;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            //AddExtrasListCtrl();
            MMK_SetAtvCtrl(win_id,ctrl_id);
            break;
        case MSG_FULL_PAINT:
             if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(text_id))
             {
                 GUIWIN_UpdateStb();
             } 
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            AddExtrasListCtrl(cur_item);
            GUILIST_SetCurItemIndex(ctrl_id,cur_item);
            break;
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            switch(cur_item){
                case ID_EXTRAS_MAIN_MENU_CT:
#ifdef COUNTEDTIME_SUPPORT
                    if(FALSE == IsTimerRunning())
                    {
                        MMIAPICT_OpenCoutimeWin();
                    }
                    else
#endif
                    {
                        MMK_CreateWin((uint32*)EXTRAS_SEL_MENU_WIN_TAB,PNULL);
                    }
                    break;
#ifdef STOPWATCH_SUPPORT
                case ID_EXTRAS_MAIN_MENU_SW:
                    MMIAPIST_OpenStopWatchWin();
                    break;
#endif
                case ID_EXTRAS_MAIN_MENU_FLASHL:
                    MMIACC_OpenFlashlighWin();
                    break;
#ifdef MMI_UNITCONVERSION_SUPPORT
                case ID_EXTRAS_MAIN_MENU_UT:
                    MMIAPIUNITCONV_OpenMainWin();
                    break;
#endif
#ifdef MMI_RECORD_SUPPORT
                case ID_EXTRAS_MAIN_MENU_VR:
                    MMIAPIRECORD_OpenMainWin(PNULL);
                    break;
#endif
                default:
                    break;
                }
            break;
        case MSG_ACC_UPDATE_COUNTEDTIME:
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

LOCAL void AddExtrasListCtrl(uint16 current_item)
{
    MMI_CTRL_ID_T ctrl_id = MMIEXTRAS_MAIN_MENU_LIST_CTRL_ID;
    uint16 list_index = 0;
#ifdef MMI_ORIG_UNISOC_SUPPORT
#ifdef MMI_RECORD_SUPPORT
    uint16 total_num = 5;
#else
    uint16 total_num = 4;
#endif
#else
    uint16 total_num = 4;
#endif

#ifndef COUNTEDTIME_SUPPORT
	total_num -= 1;
#endif

    GUILIST_SetMaxItem(ctrl_id, total_num, FALSE);

#ifdef COUNTEDTIME_SUPPORT
    MMIAPICT_AppendCTListItem(MMIEXTRAS_MAIN_MENU_LIST_CTRL_ID, current_item);
#endif
    //AppendExtrasItem2Line(MMIEXTRAS_MAIN_MENU_LIST_CTRL_ID, STXT_COUNTDOWN_TIMER);
    AppendExtrasItem2Line(MMIEXTRAS_MAIN_MENU_LIST_CTRL_ID, STXT_STOPW_MENU, STR_POC_ILIST_OFF);
    AppendFlashlightItem(MMIEXTRAS_MAIN_MENU_LIST_CTRL_ID, STR_FLASHL_SETT_TORCH);
    AppendExtrasItem1Line(MMIEXTRAS_MAIN_MENU_LIST_CTRL_ID, STR_CNV_CONVERTER);
#ifdef MMI_ORIG_UNISOC_SUPPORT
#ifdef MMI_RECORD_SUPPORT
    AppendExtrasItem1Line(MMIEXTRAS_MAIN_MENU_LIST_CTRL_ID, STXT_HEADER_VOICE_REC);
#endif
#endif
    //GUILIST_SetCurItemIndex(ctrl_id, list_index);
}

LOCAL void AppendExtrasItem2Line(
                                 MMI_CTRL_ID_T  ctrl_id,
                                 MMI_TEXT_ID_T  text_1,
                                 MMI_TEXT_ID_T  text_2
)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;

    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id = text_2;
    item_data.item_content[1].is_opacity = TRUE;
    item_data.item_content[1].font_color_opacity = 153;
    item_data.item_content[1].font_color_opacity_focus = 153;

    GUILIST_AppendItem(ctrl_id, &item_t);

}


LOCAL void AppendFlashlightItem(
                                MMI_CTRL_ID_T  ctrl_id,
                                MMI_TEXT_ID_T  text_1)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;

    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;

#ifndef WIN32
    if(GPIO_GetTorchStatus())
    {
        item_data.item_content[1].item_data.text_id = STR_POC_ILIST_ON;
    }
    else
    {
        item_data.item_content[1].item_data.text_id = STR_POC_ILIST_OFF;
    }
#endif

    item_data.item_content[1].is_opacity = TRUE;
    item_data.item_content[1].font_color_opacity = 153;
    item_data.item_content[1].font_color_opacity_focus = 153;

    GUILIST_AppendItem(ctrl_id, &item_t);

}
LOCAL void AppendExtrasItem1Line(
                                 MMI_CTRL_ID_T  ctrl_id,
                                 MMI_TEXT_ID_T  text_1
)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_MS;

    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    GUILIST_AppendItem(ctrl_id, &item_t);
}

LOCAL void InsertMenuNode(
                                MMI_CTRL_ID_T       ctrl_id,        //控件id
                                MMI_TEXT_ID_T       text_id,        //插入的节点的文本
                                uint16              node_id,        //节点编号
                                uint16              parent_node_id, //父节点编号
                                uint16              index
                                 )
{
    MMI_STRING_T            string = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    MMI_GetLabelTextByLang(text_id, &string);
    node_item.item_text_ptr = &string;
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}

LOCAL void CreateSimSelectionMenu(
                                MMI_WIN_ID_T win_id,
                                MMI_CTRL_ID_T ctrl_id)
{
    uint8 index = 0;
    MMI_TEXT_ID_T label = TXT_NULL;

    GUIMENU_CreatDynamic(PNULL,win_id,ctrl_id,GUIMENU_STYLE_POPUP);
    label = STR_CNTDN_EDIT_TIMER_EXT01;
    InsertMenuNode(ctrl_id,label,CT_SEL_MENU_TIMER_EDIT,
            CT_SEL_MENU_NODE_ROOT,index);

    index++;
    label = STXT_COUNTDOWN_TIMER_STOP;
    InsertMenuNode(ctrl_id,label,CT_SEL_MENU_TIMER_STOP,
            CT_SEL_MENU_NODE_ROOT,index);

    GUIMENU_SetPopMenuCentered(ctrl_id,TRUE);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id,TXT_NULL,STXT_SOFTKEY_SELECT_MK,TXT_COMMON_COMMON_BACK);
}

LOCAL MMI_RESULT_E HandleCTSelMenuWindow(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param

)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T menu_ctrl_id = MMICT_CUSTOM_SEL_MENU_CTRL_ID;
    uint8 cur_node = 0;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            CreateSimSelectionMenu(win_id,menu_ctrl_id);
            MMK_SetAtvCtrl(win_id, menu_ctrl_id);
            MMK_SetWinHasStb(win_id, FALSE);
            break;
        case MSG_FULL_PAINT:
            //AddExtrasListCtrl();
            break;
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
            cur_node = GUIMENU_GetCurNodeId(menu_ctrl_id);
            switch(cur_node)
            {
                case CT_SEL_MENU_TIMER_EDIT:
#ifdef COUNTEDTIME_SUPPORT
                    //MMIAPICT_StopTimer();
                    MMIAPICT_OpenCoutimeWin();
#endif
                    MMK_CloseWin(win_id);
                    break;
                case CT_SEL_MENU_TIMER_STOP:
#ifdef COUNTEDTIME_SUPPORT
                    MMIAPICT_StopTimer();
#endif
                    MMK_CloseWin(win_id);
                    break;
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}
