/****************************************************************************
** File Name:      mmipb_view.c                                         *
** Author:                                                                 *
** Date:           06/11/2004                                              *
** Copyright:      2004 SpreaDtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the window for phonebook  *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/2011        maryxiao        Create
** 
****************************************************************************/
#define _MMIPB_WINTAB_C_  

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmipb_set.h"
#include "guilistbox.h"
#include "guilabel.h"
#include "guitoolbar.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmk_timer.h"
//#include "mmipb_internal.h"
#include "mmiaudio_ctrl.h" 
#include "mn_api.h"
#include "mmi_text.h"
#include "mmifmm_text.h"
#include "mmisms_text.h"
#include "mmidisplay_data.h"
#include "mmisms_export.h"
#include "mmicc_export.h"
#include "mmicc_image.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmimms_export.h"
#include "guitext.h"
#include "mmipb_search.h"
//#include "mmipb_menutable.h"
#include "mmipb_id.h"
#include "mmipb_image.h"
#include "mmipb_text.h"
#include "mmiset_text.h"
#include "guibutton.h"
#include "mmipb_id.h"
#include "mmipub.h"
#include "mmifmm_export.h"
//#include "mmipb_position.h"
#include "guirichtext.h"
#include "mmivcard_export.h"
#include "mmiocr_export.h"
#include "guitab.h"
#include "mmiudisk_export.h"
#include "mmibt_text.h"
#include "mmicc_text.h"
#include "mmiidle_export.h"
#include "mmivcard_export.h"
#include "mmisd_export.h"
#include "mmi_mainmenu_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "guitips.h"
#include "mmifdn_export.h"
#include "mmikl_export.h"
#include "window_parse.h"
#include "mmi_default.h"
#include "guiedit.h"
#include "guiim_base.h"
#include "mmimms_text.h"
#include "mmi_appmsg.h"
//#include "guictrl_api.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmipim_export.h"
#include "guiform.h"
//#include "guires.h"
//#include "mmiim_multitap.h"
#include "guistring.h"
//#include "im_image.h"

//#include "mmiset_call.h"
#include "mmiset_call_export.h"
#if (defined MMIWIDGET_SUPPORT ) || (defined MMI_GRID_IDLE_SUPPORT )
#include "mmiwidget_export.h"
#include "mmiwidget_image.h"
#endif
#ifdef ASP_SUPPORT
#include "mmiasp_export.h"
#endif
#include "guiurl.h"
#include "mmicl_export.h"

//+CR206252
#ifdef PUSH_EMAIL_SUPPORT
#include "mmimail_text.h"
#include "mmimail_export.h"
#include "mmimail_internal.h"
#endif
#include "mmiim.h"
#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif
#include "mmivc_common.h"
#include "mmisms_id.h"
#include "mmiset_text.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#ifdef MMI_PDA_SUPPORT  
#include "mmicc_id.h"
#include "mmicc_image.h"
#endif
#include "guiurl.h"

#include "mmipb_menu.h"
#include "mmipb_view.h"
#ifdef MMIPB_SYNC_WITH_O365
#include "mmipb_task.h"
#include "mmipb_datalist.h"
#include "mmipb_sync_storage.h"

#include "mmimsa_export.h"
#include "mmiconnection_export.h"

//#define MMIPB_SYNC_USE_PC_TOKEN //shiwei add test
#endif
#include "variant_config_nv.h"
#include "nvitem.h"

#ifdef MMIPB_SELECT_DISPLAY_LOCATION_SUPPORT
LOCAL MMI_TEXT_ID_T s_display_opt_list[]={
    TXT_COMMON_ALL,
    TXT_PHONE,
    TXT_SIM_SEL_SIM1,
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
    TXT_SIM_SEL_SIM2,             //SIM卡2
    #if (defined MMI_MULTI_SIM_SYS_TRI||defined MMI_MULTI_SIM_SYS_QUAD)
    TXT_SIM_SEL_SIM3,             //SIM卡3
    #endif
    #ifdef MMI_MULTI_SIM_SYS_QUAD
    TXT_SIM_SEL_SIM4,             //SIM卡4
    #endif
    #endif
};
#endif

LOCAL BOOLEAN WritePbSet(uint32 item_id, uint32  value);
LOCAL uint32 ReadPbSet(uint32 item_id);

#ifdef MMI_PDA_SUPPORT //ping.gao modify for non-pda compile error
#define MENU_TYPE_SIGLE_LINE   (1<<0)
#define MENU_TYPE_ICON          (1<<1)
#define MENU_TYPE_FIXED_SUB_TEXT    (1<<2)
#define MENU_TYPE_SPECIAL_SUB   (1<<3)

#define MMIPB_SET_DISPLAY_OPT_LIST_MAX (MN_DUAL_SYS_MAX+2)
#define MMIPB_SET_DISPLAY_PERIOD_LIST_MAX (4)
#define MMIPB_SET_UPDATE_PERIOD_LIST_MAX (6)



//#define MMIPB_SNS_GROUP_NUM           (uint8)(3)
#define MMIPB_SET_ACCOUNT_LIST_MAX   (7)

//#define MMIPB_ISSPLIT(X)     (X == 0XFFFFFFFF)
//#define SPLIT_DATA          0XFFFFFFFF
#define MMIPB_SNS_FLAG   (1<<31)

#define PB_NET_CONNECT_INDEX (0)
#define PB_AUTO_UPDATE_INDEX (1)


typedef enum
{
    PB_GROUP_SNS_FACEBOOK = MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM,
    PB_GROUP_SNS_TWITTER,
    PB_GROUP_SNS_SINA,
    PB_GROUP_SNS_MAX
}PB_GROUP_SNS_LIST_E;

//typedef void (*DRAW_SUBMENU)(void);

typedef struct mmi_pbset_menu_s{
    MMI_TEXT_ID_T res_id; // pb set main menu main text
    uint32 menu_type; // pb set main menu item type
    MMI_TEXT_ID_T sub_fix_id;  // pb set main menu sub fixed text
    uint32 nv_id;       // pb set main menu nv id
    MMI_TEXT_ID_T* sub_array; // pb set menu sub menu 
    uint32 sub_count; // pb set menu sub menu count
    void (*draw_func)(MMI_HANDLE_T applet_handle );  //pb set menu sub menu draw function
}MMIPB_SET_MENU_T;

LOCAL MMI_TEXT_ID_T s_pos_list[]={
    TXT_PB_MOBLIE,
   TXT_SIM_SEL_SIM1,
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
    TXT_SIM_SEL_SIM2,             //SIM卡2
    #if (defined MMI_MULTI_SIM_SYS_TRI||defined MMI_MULTI_SIM_SYS_QUAD)
    TXT_SIM_SEL_SIM3,             //SIM卡3
    #endif
    #ifdef MMI_MULTI_SIM_SYS_QUAD
    TXT_SIM_SEL_SIM4,             //SIM卡4
    #endif
    #endif
   // TXT_PB_SET_TCARD,
    TXT_COMMON_ASK_EVERYTIME,
};

LOCAL MMI_TEXT_ID_T s_display_period_list[MMIPB_SET_DISPLAY_PERIOD_LIST_MAX]={
    TXT_PB_SET_DISPLAY_PERIOD_1DAY,
    TXT_PB_SET_DISPLAY_PERIOD_3DAY,
    TXT_PB_SET_DISPLAY_PERIOD_7DAY,
    TXT_PB_SET_DISPLAY_PERIOD_30DAY, 
};

LOCAL MMI_TEXT_ID_T s_update_period_list[MMIPB_SET_UPDATE_PERIOD_LIST_MAX]={
    TXT_PB_SET_UPDATE_PERIOD_NONE,
    TXT_PB_SET_UPDATE_PERIOD_30MIN,
    TXT_PB_SET_UPDATE_PERIOD_1HOUR,
    TXT_PB_SET_UPDATE_PERIOD_4HOUR,
    TXT_PB_SET_UPDATE_PERIOD_8HOUR,
    TXT_ALARM_MODE_EVERYDAY
};
#ifdef SNS_SUPPORT
typedef enum
{
     MMIPB_SNS_GROUP_ID,
     MMIPB_SNS_GROUP_TEXT,
     MMIPB_SNS_GROUP_ICON,    
     MMIPB_SNS_GROUP_ITEM_NUM
} MMIPB_SNS_GROUP_ACCPUNT_E;

LOCAL const  uint32 s_defaut_sns_list[MMIPB_SNS_GROUP_NUM][MMIPB_SNS_GROUP_ITEM_NUM] = {
    MMIPB_GROUP_SNS_SINA,TXT_MAINMENU_ICON_SINA,  IMAGE_PHONEBOOK_GROUP_SINA,
    MMIPB_GROUP_SNS_TIWTTER,TXT_COMMON_TIWTTER, IMAGE_PHONEBOOK_GROUP_TWITTER,
    MMIPB_GROUP_SNS_FACEBOOK,TXT_COMMON_FACEBOOK, IMAGE_PHONEBOOK_GROUP_FACEBOOK,
};
#endif
LOCAL uint32 s_group_id[MMIPB_MAX_GROUP_NUM] = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

#if defined(MMI_SPEEDDIAL_SUPPORT) 
/*****************************************************************************/
//  Description : open speeaddial win
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
LOCAL void  MMIPB_OpenSpeedDialWin(MMI_HANDLE_T applet_handle);
#endif

#ifdef SNS_SUPPORT 
LOCAL void  MMIPB_OpenSetAccountWin(MMI_HANDLE_T applet_handle);
#endif

/*****************************************************************************/
//  Description : pb设置主菜单处理函数
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbSetMenuWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

#ifdef MMIPB_SELECT_STORE_LOCATION_SUPPORT
/*****************************************************************************/
// 	Description : pb设置新建联系人保存位置窗口处理函数
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbSetSavePosWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#endif

/*****************************************************************************/
//  Description :  详情message 处理函数 
//  Global resource dependence : 
//  Author: gaoping
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDetailMessageWinMsg(
                                MMI_WIN_ID_T win_id, 
                                MMI_MESSAGE_ID_E msg_id, 
                                DPARAM param
                                );

#if defined (MMIPB_SIMDN_SUPPORT)
/*****************************************************************************/
//  Description : 打开本机号码设置
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIPB_OpenMSISDN(MMI_HANDLE_T applet_handle);
#endif

#ifdef SNS_SUPPORT 
/*****************************************************************************/
//  Description : pb账户设置处理函数
//  Global resource dependence : 
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbSetAccountWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#endif

WINDOW_TABLE( MMIPB_DETAILS_MESSAGE_LIST_TAB_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleDetailMessageWinMsg),    
    WIN_ID( MMIPB_DETAILS_MESSAGE_LIST_TAB_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPB_DETAILS_MESSAGE_LIST_CTRL_ID),
    
    END_WIN
};

WINDOW_TABLE( MMIPB_SET_MENUWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandlePbSetMenuWinMsg),   
    WIN_TITLE(STXT_MAIN_SETTING),   
    WIN_ID( MMIPB_SET_MENUWIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPB_SET_MENU_CTRL_ID),
    END_WIN
};
#ifdef MMIPB_SELECT_STORE_LOCATION_SUPPORT
WINDOW_TABLE( MMIPB_SET_SAVEPOSWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandlePbSetSavePosWinMsg ),    
    WIN_TITLE( STXT_STORE_MEDIUM),
    WIN_ID( MMIPB_SET_SAVE_POS_WID_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_FINISH),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIPB_SET_SAVE_POS_LIST_CTRL_ID),
    END_WIN
};
#endif

#ifdef SNS_SUPPORT 
WINDOW_TABLE( MMIPB_SET_ACCOUNTWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandlePbSetAccountWinMsg ),    
    WIN_TITLE( TXT_ACCOUNT),
    WIN_ID( MMIPB_SET_ACCOUNT_WID_ID ),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPB_SET_ACCOUNT_LIST_CTRL_ID),
    END_WIN
};
#endif


LOCAL const MMIPB_SET_MENU_T s_set_menulist[]={
#ifdef SNS_SUPPORT     
    {TXT_ACCOUNT,            MENU_TYPE_FIXED_SUB_TEXT, TXT_PB_SET_ACCOUNT_SUBSTR,0, 0, 0, MMIPB_OpenSetAccountWin},
#endif
#ifdef MMIPB_SELECT_STORE_LOCATION_SUPPORT
#ifdef MMI_PDA_SUPPORT
    {STXT_STORE_MEDIUM,    MENU_TYPE_ICON,                    0 ,  MMINV_PHONEBOOK_SAVE_POSTION, s_pos_list, sizeof(s_pos_list)/sizeof(s_pos_list[0]) +1 , MMIPB_OpenSetSavePosWin},
#endif
#endif
#ifdef MMIPB_SELECT_DISPLAY_LOCATION_SUPPORT
{TXT_DISPLAY_OPTION, MENU_TYPE_ICON,   TXT_PB_SET_DISPLAY_OPTION_SUBSTR ,  MMINV_PHONEBOOK_DISPLAY_OPTION, s_display_opt_list, sizeof(s_display_opt_list)/sizeof(s_display_opt_list[0]) +1, MMIPB_OpenSetDisplayOptWin},
#endif
#if defined(MMI_SPEEDDIAL_SUPPORT) 
    {TXT_COMMON_SPEEDDIAL,        MENU_TYPE_FIXED_SUB_TEXT,    TXT_PB_SET_SPEEDDIAL_NUM,   0, PNULL, 0, MMIPB_OpenSpeedDialWin},   
#endif
#ifdef MMIPB_SIMDN_SUPPORT
    {TXT_PB_MSISDN,   MENU_TYPE_FIXED_SUB_TEXT,    TXT_PB_SET_LOCAL_NUMBER_SUBSTR , 0,PNULL, 0, MMIPB_OpenMSISDN}, /*lint !e64*/
    {TXT_PB_SDN,     MENU_TYPE_FIXED_SUB_TEXT,    TXT_PB_SDN ,    0,PNULL, 0, MMIPB_OpenSDNWin},/*lint !e64*/
#endif
    {TXT_COMMON_DETAIL_MEMORY,              MENU_TYPE_SIGLE_LINE,            0,                   0, PNULL, 0, MMIPB_OpenMemDetail},
};

LOCAL void AppendPbSetListItem(MMI_CTRL_ID_T       ctrl_id,
                          GUIITEM_STYLE_E     item_style,
                          MMI_IMAGE_ID_T      icon_id,
                          const MMI_STRING_T*  primary_str_ptr,
                          const MMI_STRING_T*   second_str_ptr,
                          uint32               user_data,
                          uint16               pos,
                           BOOLEAN          is_update
)
{
    GUILIST_ITEM_T      item_t = {0};/*lint !e64 */
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64 */
    uint16               content_index = 0;
    item_t.item_style    = item_style;//GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.user_data = user_data;
    item_t.item_data_ptr = &item_data;

    if(GUIITEM_STYLE_ONE_LINE_ICON_TEXT == item_style)
    {
        item_data.item_content[content_index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[content_index].item_data.image_id = icon_id;
        content_index++;

        if ( PNULL != primary_str_ptr && 0 < primary_str_ptr->wstr_len && PNULL != primary_str_ptr->wstr_ptr )
        {
            item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[content_index].item_data.text_buffer = *primary_str_ptr;
            content_index++;
        }
    }
    else if(GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2 == item_style)
    {
        item_data.item_content[content_index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[content_index].item_data.image_id = icon_id;
        content_index++;

        if ( PNULL != primary_str_ptr && 0 < primary_str_ptr->wstr_len && PNULL != primary_str_ptr->wstr_ptr )
        {
            item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[content_index].item_data.text_buffer = *primary_str_ptr;
            content_index++;
        }
        
        if ( PNULL != second_str_ptr && 0 < second_str_ptr->wstr_len && PNULL != second_str_ptr->wstr_ptr )
        {
            item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[content_index].item_data.text_buffer = *second_str_ptr;
        }
    }
    else
    {
        if ( PNULL != primary_str_ptr && 0 < primary_str_ptr->wstr_len && PNULL != primary_str_ptr->wstr_ptr )
        {
            item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[content_index].item_data.text_buffer = *primary_str_ptr;
            content_index++;
        }
        if(GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM == item_style)
        {
            if(icon_id != 0)
            {
            
                //need icon
                item_data.item_content[content_index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[content_index].item_data.image_id = icon_id;
            }
            content_index++;
        }

        if ( PNULL != second_str_ptr && 0 < second_str_ptr->wstr_len && PNULL != second_str_ptr->wstr_ptr )
        {
            item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[content_index].item_data.text_buffer = *second_str_ptr;
        }
    }
    
    if( is_update )
    {
        GUILIST_ReplaceItem(ctrl_id,&item_t,pos);      
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}

/*****************************************************************************/
//  Description : 进入设置窗口
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenSetWin(MMI_HANDLE_T applet_handle)
{
//    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
   // MMIPB_DeleteAllSelNode(applet_ptr->instance.handle); //是否需要
    MMK_CreateWinByApplet(applet_handle,(uint32 *)MMIPB_SET_MENUWIN_TAB, PNULL);
}
#ifdef MMIPB_SELECT_STORE_LOCATION_SUPPORT
/*****************************************************************************/
// 	Description : pb设置新建联系人保存位置窗口处理函数
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbSetSavePosWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_CTRL_ID_T ctrl_id =  MMK_GetCtrlHandleByWin(win_id,MMIPB_SET_SAVE_POS_LIST_CTRL_ID);
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 save_position_nv_id = 0;
    uint32 save_position_index = 0;
    uint16 sel_index = 0;
    uint16 item_index = 0;
    
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:   
        GUILIST_SetMaxItem(ctrl_id, PB_SAVE_POSITION_MAX, FALSE ); 
        save_position_nv_id = ReadPbSet(MMINV_PHONEBOOK_SAVE_POSTION);
        
        for(save_position_index = 0; save_position_index < PB_SAVE_POSITION_MAX; save_position_index++)
        {
            GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
            GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

            if(save_position_index >= PB_SAVE_POSITION_SIM_BEGAIN && save_position_index  <= PB_SAVE_POSITION_SIM_END)
            {
                MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1 + save_position_index - PB_SAVE_POSITION_SIM_BEGAIN;
                if(!MMIAPIPHONE_GetSimExistedStatus(dual_sys))
                {
                    continue;
                }
            }
            
            item_t.item_style  = GUIITEM_STYLE_ONE_LINE_RADIO;            
            item_t.item_data_ptr = &item_data;
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id = s_pos_list[save_position_index];
            item_t.user_data = save_position_index;
            GUILIST_AppendItem( ctrl_id, &item_t );
            if(save_position_index == save_position_nv_id)
            {
                GUILIST_SetSelectedItem(ctrl_id, item_index, TRUE);
                GUILIST_SetCurItemIndex( ctrl_id, item_index);
            }
            item_index++;
        }
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    
#ifdef MMI_PDA_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#else //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif
        save_position_nv_id = 0;
        sel_index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id,sel_index,&save_position_nv_id);
        WritePbSet(MMINV_PHONEBOOK_SAVE_POSTION, save_position_nv_id);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}
#endif

#ifdef MMIPB_SELECT_STORE_LOCATION_SUPPORT
/*****************************************************************************/
//  Description : 创建pb设置保存位置选择窗口
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIPB_OpenSetSavePosWin(MMI_HANDLE_T applet_handle)
{
    MMK_CreatePubListWinEx(applet_handle,
        (uint32*)MMIPB_SET_SAVEPOSWIN_TAB, PNULL);
}
#endif
#if defined(MMI_SPEEDDIAL_SUPPORT) 
/*****************************************************************************/
//  Description : open speeaddial win
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
LOCAL void  MMIPB_OpenSpeedDialWin(MMI_HANDLE_T applet_handle)
{
    MMIAPICC_OpenSetSpeedDial(PNULL, 0, 0); 
}
#endif

#ifdef SNS_SUPPORT 
LOCAL void MMIPB_InitSNSGroup(void)    
{
     //初始化sns_group使用信息,以后需要修改
}
LOCAL void  MMIPB_OpenSetAccountWin(MMI_HANDLE_T applet_handle )
{
    MMIPB_InitSNSGroup();
    MMK_CreateWinByApplet(applet_handle,(uint32 *)MMIPB_SET_ACCOUNTWIN_TAB, PNULL);
}
#endif

/*****************************************************************************/
//  Description : 更新pb 设置菜单列表
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
LOCAL void  UpdatePDAPbSetListCtrl(MMI_CTRL_ID_T ctrl_id, BOOLEAN is_init)
{
    uint32 item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
    uint32 idx=0;
    uint32 local_idx = 0;
    uint32 total = sizeof(s_set_menulist)/sizeof(MMIPB_SET_MENU_T);
    MMI_IMAGE_ID_T icon_id=IMAGE_COMMON_RIGHT_ARROW;//Fixed ^ icon
    MMI_STRING_T primary_str = {0};
    MMI_STRING_T second_str = {0};
    
    for(idx = 0; idx < total; idx++)
    {
        if(s_set_menulist[idx].menu_type & MENU_TYPE_SIGLE_LINE)//单行
        {
            item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
        }
        else //双行
        {
            if(s_set_menulist[idx].menu_type & MENU_TYPE_ICON)
            {
                item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
                icon_id=IMAGE_COMMON_RIGHT_ARROW;
            }
            else
            {
                item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
                icon_id = 0;
            }            

            if(s_set_menulist[idx].menu_type & MENU_TYPE_FIXED_SUB_TEXT)
            {
                MMI_GetLabelTextByLang(s_set_menulist[idx].sub_fix_id, &second_str);
            }
            else
            {
                local_idx = ReadPbSet(s_set_menulist[idx].nv_id);
                if(local_idx >= s_set_menulist[idx].sub_count)
                {
                    local_idx = 0;
                }
                MMI_GetLabelTextByLang(s_set_menulist[idx].sub_array[local_idx], &second_str);
            }
        }
        MMI_GetLabelTextByLang(s_set_menulist[idx].res_id, &primary_str);
        AppendPbSetListItem(ctrl_id,item_style,icon_id,&primary_str, &second_str, 0, idx, is_init);
    }
}

/*****************************************************************************/
//  Description : pb设置主菜单处理函数
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbSetMenuWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id =  MMK_GetCtrlHandleByWin(win_id,MMIPB_SET_MENU_CTRL_ID);
//    uint32 i = 0;
    uint16 cur_index = 0;
    //uint32 user_data = 0;
    uint32 total_num = sizeof(s_set_menulist)/sizeof(MMIPB_SET_MENU_T);
    MMI_HANDLE_T    applet_handle = 0;
    MMIPB_APPLET_T *  applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtrByWin(win_id);

    if(PNULL == applet_ptr)
    {

        return MMI_RESULT_FALSE;
    }

    applet_handle = applet_ptr->caf_applet.app_handle;  

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, total_num, FALSE ); 
        UpdatePDAPbSetListCtrl(ctrl_id, FALSE);
#if defined MMI_ISTYLE_SUPPORT 		
        if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
	{
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
	}
#endif		
        MMK_SetAtvCtrl(win_id, ctrl_id);        
        break;

    case MSG_GET_FOCUS:
        UpdatePDAPbSetListCtrl(ctrl_id, TRUE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        if(cur_index < total_num)
        {
            if(s_set_menulist[cur_index].draw_func)
            {
                 s_set_menulist[cur_index].draw_func(applet_handle);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    
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

#ifdef SNS_SUPPORT 
LOCAL MMI_TEXT_ID_T GetSnsGroupText(uint8 group_id)
{
    uint16 i = 0;
    for(i = 0; i < MMIPB_SNS_GROUP_NUM; i++)
    {
        if(s_defaut_sns_list[i][MMIPB_SNS_GROUP_ID] == group_id)
        {
            return s_defaut_sns_list[i][MMIPB_SNS_GROUP_TEXT];
        }
    }
    return 0;
}
LOCAL MMI_TEXT_ID_T GetSnsGroupIcon(uint8 group_id)
{
    uint16 i = 0;
    for(i = 0; i < MMIPB_SNS_GROUP_NUM; i++)
    {
        if(s_defaut_sns_list[i][MMIPB_SNS_GROUP_ID] == group_id)
        {
            return s_defaut_sns_list[i][MMIPB_SNS_GROUP_ICON];
        }
    }
    return 0;
}

/*****************************************************************************/
//  Description : 更新pb 账户设置列表
//  Global resource dependence : 
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL void  UpdatePDAPbSetAccountListCtrl(MMI_CTRL_ID_T ctrl_id)
{
    uint32 idx=0;
    uint16 cur_index = 0;
    uint8   sns_group_id = 0;    
    MMI_STRING_T primary_str = {0};
    MMI_STRING_T second_str = {0};
    
    cur_index = GUILIST_GetCurItemIndex(ctrl_id);

    //first remove all list items
    if(GUILIST_GetTotalItemNum(ctrl_id) > 0)
    {
    	GUILIST_RemoveAllItems(ctrl_id);
    }
    
    
    GUILIST_SetMaxItem(ctrl_id, MMIPB_SNS_GROUP_NUM, FALSE ); 

    
    second_str.wstr_len = MMIPB_GetSNSAccountEmailLen();
    second_str.wstr_ptr = SCI_ALLOC_APPZ(second_str.wstr_len);
            
    //获取SNS账户信息
    for(sns_group_id = MMIPB_GROUP_SNS_SINA; sns_group_id < MMIPB_GROUP_SNS_MAX; sns_group_id++)
    {
        //if(s_group_id[sns_group_id] == sns_group_id)
        {
           MMI_GetLabelTextByLang(GetSnsGroupText(sns_group_id), &primary_str);

            SCI_MEMSET(second_str.wstr_ptr,0x00,second_str.wstr_len);
            MMIPB_GetSNSAccountEmail(sns_group_id,&second_str);
             
            AppendPbSetListItem(ctrl_id,GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2,
                                                        GetSnsGroupIcon(sns_group_id),
                                                        &primary_str, &second_str, 
                                                        MMIPB_SNS_FLAG | sns_group_id,  
                                                        idx, FALSE);
            idx++;
        }
     }
    SCI_Free(second_str.wstr_ptr);
    GUILIST_SetCurItemIndex(ctrl_id, cur_index);
}
 
/*****************************************************************************/
//  Description : pb账户设置处理函数
//  Global resource dependence : 
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbSetAccountWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id =  MMK_GetCtrlHandleByWin(win_id,MMIPB_SET_ACCOUNT_LIST_CTRL_ID);
   // uint32 i = 0;
    uint16 cur_index = 0;
    uint32 user_data = 0;
//    uint32  total_num = 0;

    MMI_HANDLE_T    applet_handle = 0;
    MMIPB_APPLET_T *  applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtrByWin(win_id);

    if(PNULL == applet_ptr)
    {

        return MMI_RESULT_FALSE;
    }

    applet_handle = applet_ptr->caf_applet.app_handle;  
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        UpdatePDAPbSetAccountListCtrl(ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);        
        break;

    case MSG_GET_FOCUS:
        UpdatePDAPbSetAccountListCtrl(ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, cur_index, &user_data);
        if(!MMIPB_ISSPLIT(user_data))
        {
            if(PB_NET_CONNECT_INDEX == user_data)
            {
                //网络连接处理函数
            }
            else 
            {
                //sns 账户
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    
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
#endif

/*****************************************************************************/
// 	Description : to handle the contact msg log list select item message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePBDetailMsgSelectWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

                                    
// the window for call log icon main menu
WINDOW_TABLE( MMIPB_DETAILMSG_SELECT_WIN_TAB ) = 
{

    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePBDetailMsgSelectWindow ), 
    WIN_ID( MMIPB_DETAILMSG_SELECT_WIN_ID ),  
    WIN_TITLE(TXT_DELETE),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_DETAILMSG_SELECT_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS, MMIPB_DETAILMSG_SELECT_TITLE_FORM_CTRL_ID, MMIPB_DETAILMSG_SELECT_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIPB_DETAILMSG_SELECT_TITLE_LABEL_CTRL_ID, MMIPB_DETAILMSG_SELECT_TITLE_FORM_CTRL_ID),
    CHILD_CHECKBOX_CTRL(TRUE, MMIPB_DETAILMSG_SELECT_TITLE_BUTTON_CTRL_ID, MMIPB_DETAILMSG_SELECT_TITLE_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE,GUILIST_CHECKLIST_E,MMIPB_DETAILMSG_SELECT_LIST_CTRL_ID,MMIPB_DETAILMSG_SELECT_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMIPB_DETAILMSG_SELECT_FORM_CTRL_ID),

    END_WIN
};

/*****************************************************************************/
//  Description : create message
//  Global resource dependence : none
//  Author: gao.ping
//  Note: 
/*****************************************************************************/
MMISMS_DETAIL_MSGLOG_DATA_T        *s_msglogData_ptr=PNULL;

/*****************************************************************************/
//  Description : handle message win message
//  Global resource dependence : none
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
LOCAL void AppendMsglogList(MMI_CTRL_ID_T   ctrl_id)
{
    uint32          i = 0;
    GUILIST_ITEM_T  item = {0};

    GUILIST_RemoveAllItems(ctrl_id);
    GUILIST_SetMaxItem(ctrl_id,s_msglogData_ptr->matched_count,TRUE);
    //set item style
    item.item_style =  GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT;
    //item.item_state = GUIITEM_STATE_CONTENT_CHECK;//NEWMS00121925
    for (i=0;i<s_msglogData_ptr->matched_count;i++)
    {
            GUILIST_AppendItem( ctrl_id, &item );
    }
    
}

/*****************************************************************************/
//  Description : Some APIS for Contact msg lists win
//  Global resource dependence : none
//  Author: sam.hua
//  Note: 
    //PBDetailMsg_GetMaxNum 获取最大的个数
    //PBDetailMsg_SetSelectedbyIndex        //设置当前ITEM对于的选择标记
    //PBDetailMsg_SetAllSelected            //设置All ITEM对于的选择标记
    //PBDetailMsg_DeleteSelection           //删除选择的短信
    //PBDetailMsg_StartSelection            //进行多选删除的初始化
    //PBDetailMsg_ExitSelection             //退出多选删除
    //PBDetailMsg_ExitWin                   //退出联系人的短信列表
    //PBDetailMsg_DeletedCallback           //删除完成之后的回调函数
    //PBDetailMsg_BeginSearch               //启动当前联系人的短信搜索
    //PBDetailMsg_UpdateLogList             //更新短信列表
/*****************************************************************************/
/*****************************************************************************/
//  Description :  获取最大的个数
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL uint16 PBDetailMsg_GetMaxNum(void)
{
   if (s_msglogData_ptr != PNULL)
    return s_msglogData_ptr->matched_count;
   return 0; 
}

/*****************************************************************************/
//  Description : 更新短信列表
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void PBDetailMsg_UpdateLogList(MMI_HANDLE_T list_handle)
{
    if (s_msglogData_ptr == PNULL)
        return;

    if (s_msglogData_ptr->last_matched_count !=s_msglogData_ptr->matched_count)
    {
        //Reset Selected status if search result number changed
        if (s_msglogData_ptr->isSelecting )
        {
            SCI_MEMSET(s_msglogData_ptr->itemSelected,0x00,PBDETAIL_MSG_LOG_MAX_NUM);
            s_msglogData_ptr->last_matched_count = s_msglogData_ptr->matched_count;
        }
    }
    
    AppendMsglogList(list_handle);
}
/*****************************************************************************/
//  Description : //设置当前ITEM对于的选择标记
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void PBDetailMsg_SetSelectedbyIndex(uint16 index)
{
    if (s_msglogData_ptr == PNULL)
        return;

    if (s_msglogData_ptr->itemSelected[index])
    {
        s_msglogData_ptr->itemSelected[index]= FALSE;
    }
    else
    {
        s_msglogData_ptr->itemSelected[index]= TRUE;
    }
}
/*****************************************************************************/
//  Description : 设置All ITEM对于的选择标记
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void PBDetailMsg_SetAllSelected(BOOLEAN isSelected)
{
	uint16 i;
    if (s_msglogData_ptr == PNULL)
        return;
        
    for (i=0;i<PBDETAIL_MSG_LOG_MAX_NUM;i++)
    {
        s_msglogData_ptr->itemSelected[i] = isSelected;
    }
    return;
}
/*****************************************************************************/
//  Description : 删除选择的短信
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PBDetailMsg_DeleteSelection(void)
{
     if (s_msglogData_ptr == PNULL)
        return FALSE;
    return MMISMS_StartDeleteMatchMsgById(s_msglogData_ptr);
}
/*****************************************************************************/
//  Description : 退出多选删除
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void PBDetailMsg_ExitSelection(void)
{
    if (s_msglogData_ptr == PNULL)
        return;
    s_msglogData_ptr->isSelecting = FALSE;
    SCI_MEMSET(s_msglogData_ptr->itemSelected,0x00,PBDETAIL_MSG_LOG_MAX_NUM);
    return;
}
/*****************************************************************************/
//  Description :进行多选删除的初始化
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void PBDetailMsg_StartSelection(void)
{
     if (s_msglogData_ptr == PNULL)
        return;

     s_msglogData_ptr->isSelecting = TRUE;
     SCI_MEMSET(s_msglogData_ptr->itemSelected,0x00,PBDETAIL_MSG_LOG_MAX_NUM);
     return;
}
/*****************************************************************************/
//  Description : //退出联系人的短信列表
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void PBDetailMsg_ExitWin(void)
{
    if (s_msglogData_ptr!=PNULL)
    {
        SCI_FREE(s_msglogData_ptr);
    }
    return;
}
/*****************************************************************************/
//  Description : //删除完成之后的回调函数
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void PBDetailMsg_DeletedCallback(BOOLEAN result)
{
    if (s_msglogData_ptr==PNULL)
    {
        return;
    }
    if (result)
    {
        MMK_CloseWin(s_msglogData_ptr->msg_select_win_handle);
    }
    return;
}
/*****************************************************************************/
//  Description : //启动当前联系人的短信搜索
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void PBDetailMsg_BeginSearch(MMI_HANDLE_T win_handle)
{
    if (s_msglogData_ptr == PNULL)
        return;
    s_msglogData_ptr->msg_select_win_handle = win_handle;
    s_msglogData_ptr->callback = PBDetailMsg_DeletedCallback;
    MMIAPISMS_SearchMatchedItem(s_msglogData_ptr->contact_ptr,s_msglogData_ptr);/*lint !e718*/
    return;
}


/*****************************************************************************/
//	Description : Open contact msg list win API
//	Global resource dependence : 
//	Author:tao.xue
//	Note:
/*****************************************************************************/
PUBLIC void MMICL_CreateDetailMessageChildWin(MMI_WIN_ID_T	parent_win_id)
{
    MMIPB_CONTACT_T * contact_ptr = (MMIPB_CONTACT_T *)MMK_GetWinAddDataPtr(parent_win_id);
    if (s_msglogData_ptr!=PNULL)
    {
        SCI_FREE(s_msglogData_ptr);
    }
    s_msglogData_ptr= SCI_ALLOC_APPZ(sizeof(MMISMS_DETAIL_MSGLOG_DATA_T));
    s_msglogData_ptr->contact_ptr = contact_ptr;
    MMK_CreateChildWin(parent_win_id, (uint32*)MMIPB_DETAILS_MESSAGE_LIST_TAB_WIN_TAB, PNULL);
}

/*****************************************************************************/
//	Description : Open contact msg list select win API
//	Global resource dependence : 
//	Author:tao.xue
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_EnterDetailMsgListSelectWin(MMI_HANDLE_T app_handle)
{
    MMK_CreateWinByApplet(app_handle,(uint32*)MMIPB_DETAILMSG_SELECT_WIN_TAB,PNULL);
    return;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void PBDetailMsg_ValidatePrivacyTabSwitch(BOOLEAN is_permitted, DPARAM param_ptr)
{
    if (is_permitted)
    {
        MMIPB_SetCLUnProtect(TRUE);
    }
}
    
/*****************************************************************************/
//  Description : handle message win message
//  Global resource dependence : none
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDetailMessageWinMsg(
                                MMI_WIN_ID_T win_id, 
                                MMI_MESSAGE_ID_E msg_id, 
                                DPARAM param
                                )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T parent_win_id = MMK_GetParentWinHandle(win_id);
    MMI_HANDLE_T    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_DETAILS_MESSAGE_LIST_CTRL_ID);
    MMI_HANDLE_T    applet_handle = 0;
    MMIPB_APPLET_T  *applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtrByWin(win_id);

    if(PNULL == applet_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;  
   
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            {            
                PBDetailMsg_BeginSearch(win_id);

                PBDetailMsg_UpdateLogList(ctrl_handle);

                MMK_SetAtvCtrl(win_id,MMIPB_DETAILS_MESSAGE_LIST_CTRL_ID);
            }
            break;
        case MSG_FULL_PAINT:
            if ((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS)))
            {
                GUILIST_RemoveAllItems(ctrl_handle);
                
            }
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_RETURN, FALSE);
            if (s_msglogData_ptr->matched_count>0)
            {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_OPTION, FALSE);
            }
            else
            {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_NULL, FALSE);
            }
            GUIWIN_SetTitleIsVisible(win_id, TRUE, TRUE);
            
	    break;
            
        case MSG_GET_FOCUS:
                PBDetailMsg_BeginSearch(win_id);
                PBDetailMsg_UpdateLogList(ctrl_handle);
                MMK_SetAtvCtrl(win_id,MMIPB_DETAILS_MESSAGE_LIST_CTRL_ID);
            break;
        case MSG_CTL_LIST_NEED_ITEM_DATA:
            {
                GUILIST_NEED_ITEM_DATA_T    *need_data_ptr = PNULL;
        
                need_data_ptr = (GUILIST_NEED_ITEM_DATA_T *)param;
                need_data_ptr->ctrl_handle = ctrl_handle;
                MMIAPISMS_SetMatchItemContent(need_data_ptr,(uint32)s_msglogData_ptr->order_id_list[need_data_ptr->item_index],FALSE);
            }
            break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
            //Get item data;
			{
                uint16 sel_item_index = 0;

                sel_item_index = GUILIST_GetCurItemIndex(ctrl_handle);

                MMIAPISMS_OpenMatchedItem((uint32)s_msglogData_ptr->order_id_list[sel_item_index]);
            }
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_APP_MENU:
            //MMK_CloseParentWin(parent_win_id);  
            //Enter Delete Select
//            
            if (s_msglogData_ptr->matched_count>0)
            {
                MMIPB_MENU_DATA_INFOR_T add_data = {0};
                //SCI_MEMSET(&add_data,0,sizeof(MMIPB_MENU_DATA_INFOR_T));
                add_data.applet_handle = applet_ptr->caf_applet.app_handle;
                add_data.parent_win =MMK_GetWinHandle(applet_handle, win_id);
                add_data.menu_type = MMIPB_DETAIL_MSGLOG_OPT_MENU;
                add_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
                MMIPB_EnterPbTabOptMenu(&add_data);		
            }
            break;
        case MSG_CTL_TAB_SWITCH:
            //隐私检查
            if (MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS))
            {
                if (!MMIPB_GetCLUnProtect())
                {
                    //需要进行隐私检查
                    MMIAPISET_ValidatePrivacy(MMISET_PROTECT_SMS, PBDetailMsg_ValidatePrivacyTabSwitch, PNULL, 0);
                }
            }    
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            //Set Back 
            MMK_CloseParentWin(parent_win_id);    
            break;

        case MSG_CLOSE_WINDOW:
            PBDetailMsg_ExitWin();
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}   

/*****************************************************************************/
// 	Description : to handle the contact msg log list select item message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePBDetailMsgSelectWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    GUIFORM_CHILD_WIDTH_T  child_width = {0};
    MMI_HANDLE_T list_handle =MMK_GetCtrlHandleByWin(win_id,MMIPB_DETAILMSG_SELECT_LIST_CTRL_ID);
    MMI_HANDLE_T btn_handle = MMK_GetCtrlHandleByWin(win_id,MMIPB_DETAILMSG_SELECT_TITLE_BUTTON_CTRL_ID);
    MMI_HANDLE_T titleform_handle = MMK_GetCtrlHandleByWin(win_id,MMIPB_DETAILMSG_SELECT_TITLE_FORM_CTRL_ID);
    MMI_HANDLE_T form_handle = MMK_GetCtrlHandleByWin(win_id,MMIPB_DETAILMSG_SELECT_FORM_CTRL_ID);
    MMI_HANDLE_T label_handle = MMK_GetCtrlHandleByWin(win_id,MMIPB_DETAILMSG_SELECT_TITLE_LABEL_CTRL_ID);
    MMI_HANDLE_T    applet_handle = 0;
    MMIPB_APPLET_T  *applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtrByWin(win_id);

    if(PNULL == applet_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;  
    

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        
        
        GUILABEL_SetTextById(label_handle,TXT_PB_SELECT_ALL, FALSE);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_CANCEL, TRUE);
        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_width.add_data = 40;
        GUIFORM_SetChildWidth(titleform_handle,btn_handle,&child_width);
        GUIFORM_PermitChildBorder(titleform_handle, FALSE);
        GUIBUTTON_PermitBorder(btn_handle, FALSE);

        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUIFORM_SetChildHeight(form_handle,list_handle, &child_height);
        GUIFORM_IsSlide(form_handle, FALSE);
        GUIFORM_PermitChildBg(form_handle,FALSE);
        GUILIST_PermitBorder(list_handle,FALSE);
        
        GUILIST_SetMaxSelectedItem(list_handle,PBDetailMsg_GetMaxNum());
        GUIBUTTON_SetSelect(btn_handle, FALSE);                

        PBDetailMsg_BeginSearch(win_id);
        PBDetailMsg_StartSelection();

        MMK_SetAtvCtrl(win_id, list_handle);
        break;
    case MSG_FULL_PAINT:
  //      if (s_cl_need_update)   //if msg changed ,clear selected status
        {
         //PBDetailMsg_SetAllSelected(FALSE);
  //          s_cl_need_update = FALSE;
        }
        PBDetailMsg_UpdateLogList(list_handle);
        break;
    case MSG_GET_FOCUS:
        PBDetailMsg_BeginSearch(win_id);
        PBDetailMsg_UpdateLogList(list_handle);
        break;
                
    case MSG_CTL_LIST_MARK_ITEM:
        {
            uint32 sel_item_index =0;
            
			sel_item_index = GUILIST_GetCurItemIndex(list_handle);
            
            PBDetailMsg_SetSelectedbyIndex((uint16)sel_item_index);
            if(GUILIST_GetSelectedItemNum(list_handle) <  GUILIST_GetTotalItemNum(list_handle))           
            {
                if (GUIBUTTON_GetSelect(btn_handle))
                {
                    GUIBUTTON_SetSelect(btn_handle, FALSE);
					GUIBUTTON_Update(btn_handle);
                }
            }
            else
            {
                if (!GUIBUTTON_GetSelect(btn_handle) )
                {
                    GUIBUTTON_SetSelect(btn_handle, TRUE);
					GUIBUTTON_Update(btn_handle);
                }
            }
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
                case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                    if (GUILIST_GetSelectedItemNum(list_handle)<1)
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_ENTRY,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                    }
                    else
                    {
                       MMIPUB_OpenQueryWinByTextIdEx(applet_handle,TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL,PNULL);                
                    }
                    break;
                case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                    break;
                case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                    MMK_CloseWin(win_id);   
                    break;
                case MMIPB_DETAILMSG_SELECT_TITLE_BUTTON_CTRL_ID:
                    PBDetailMsg_SetAllSelected(GUIBUTTON_GetSelect(btn_handle));
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, 0,0);
                    break;
                default:
                    break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T    *need_data_ptr = PNULL;

            need_data_ptr = (GUILIST_NEED_ITEM_DATA_T *)param;
            need_data_ptr->ctrl_handle = list_handle;
            MMIAPISMS_SetMatchItemContent(need_data_ptr, (uint32)s_msglogData_ptr->order_id_list[need_data_ptr->item_index],TRUE);
            GUILIST_SetSelectedItem(list_handle, need_data_ptr->item_index,s_msglogData_ptr->itemSelected[need_data_ptr->item_index]);
        }
        break;
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWinEx(applet_handle, PNULL);
		break;
    case MSG_PROMPTWIN_OK:      //确定退出
        MMIPUB_CloseQuerytWinEx(applet_handle, PNULL);
        PBDetailMsg_DeleteSelection();  
            
		break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //Set Back 
        MMK_CloseWin(win_id);    
        break;
    case MSG_CLOSE_WINDOW:
        PBDetailMsg_ExitSelection();
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
//no MMI_PDA_SUPPORT
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef MMIPB_SIMCONTACT_SUPPORT
/*****************************************************************************/
//  Description                         : Get contact memory status
//  Global resource dependence          : none 
//  Author                              : zhongping.shi
//  Input                               : MMI_WIN_ID_T win_id,
//                                        MMI_MESSAGE_ID_E  msg_id,
//                                        DPARAM            param
//  Output                              : none
//  Return                              : MMI_RESULT_E result
//  Caution                             : none
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleContactsSettingsMemoryByWinMsg(
				     MMI_WIN_ID_T      win_id, 
 				     MMI_MESSAGE_ID_E  msg_id, 
 				     DPARAM            param
 				     );
#endif //MMIPB_SIMCONTACT_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleContactsSettingsWinMsg(
				     MMI_WIN_ID_T      win_id, 
				     MMI_MESSAGE_ID_E  msg_id, 
				     DPARAM            param
				     );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleContactsSettingsSortByWinMsg(
				     MMI_WIN_ID_T      win_id, 
 				     MMI_MESSAGE_ID_E  msg_id, 
 				     DPARAM            param
 				     );

#if defined (MMIPB_SIMDN_SUPPORT)
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleContactsSettingsSdnSimSelectWinMsg(
				     MMI_WIN_ID_T      win_id, 
 				     MMI_MESSAGE_ID_E  msg_id, 
 				     DPARAM            param
 				     );
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

WINDOW_TABLE(MMIPB_SETTINGS_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleContactsSettingsWinMsg),
    WIN_ID( MMIPB_SETTINGS_WIN_ID),
    WIN_TITLE(STXT_HEADER_PHONEBOOK_OPTIONS),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPB_SETTINGS_LISTBOX_CTRL_ID),
    END_WIN
};

#ifdef MMIPB_SIMCONTACT_SUPPORT
WINDOW_TABLE(MMIPB_SETTINGS_MEMORY_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleContactsSettingsMemoryByWinMsg),
    WIN_ID( MMIPB_SETTINGS_MEMORY_WIN_ID),
    WIN_TITLE(STXT_MEMORY_A_B),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPB_SETTINGS_MEMORY_LISTBOX_CTRL_ID),
    END_WIN
};
#endif //MMIPB_SIMCONTACT_SUPPORT

WINDOW_TABLE(MMIPB_SETTINGS_SORT_BY_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleContactsSettingsSortByWinMsg),
    WIN_ID( MMIPB_SETTINGS_SORT_BY_WIN_ID),
    WIN_TITLE(STR_CONTA_LIST_SORT_EXT01),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPB_SETTINGS_SORTBY_LISTBOX_CTRL_ID),
    END_WIN
};

#if defined (MMIPB_SIMDN_SUPPORT)
WINDOW_TABLE(MMIPB_SETTINGS_SDN_SIM_SELECT_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleContactsSettingsSdnSimSelectWinMsg),
    WIN_ID( MMIPB_SETTINGS_SDN_SIM_SELECT_WIN_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPB_SETTINGS_SDN_SIM_SELECT_LISTBOX_CTRL_ID),
    END_WIN
};
#endif


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : write pb set nv
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN  WritePbSet(uint32 item_id, uint32  value)
{
    BOOLEAN ret = FALSE;
    ERR_MNDATAMAG_CODE_E  nv_result   = ERR_MNDATAMAG_NO_ERR;   
    
    nv_result = MNNV_WriteItem(MMI_GetTrueNVItemId(item_id), sizeof(uint32), (void *)(&value));  
    ret = (nv_result == ERR_MNDATAMAG_NO_ERR ? TRUE : FALSE);
    return ret;
}

/*****************************************************************************/
// 	Description : read pb set nv
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
LOCAL uint32  ReadPbSet(uint32 item_id)
{
    ERR_MNDATAMAG_CODE_E  nv_result   = ERR_MNDATAMAG_NO_ERR;
    uint32 value = 0;
    
    nv_result = MNNV_ReadItem(MMI_GetTrueNVItemId(item_id), sizeof(uint32), (void *)(&value));  

    if(ERR_MNDATAMAG_NO_ERR != nv_result)
    {
        if(MMINV_PHONEBOOK_DISPLAY_OPTION == item_id)
        {
            value = MMIPB_CONTACT_DISPLAY_ALL;//phone and sim
        }
        else if(MMINV_PHONEBOOK_SAVE_POSTION == item_id)
        {
            value = PB_SAVE_POSITION_ALWAYS_QUERY;
        }
        else
        {
            value = 0; //默认list第一个
        }
        WritePbSet(item_id, value);
    }
    return value;
}

/*****************************************************************************/
// 	Description : read new contact save positon
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC PB_SAVE_POSITION_TYPE_E  MMIPB_ReadSavePos(void)
{
   return (PB_SAVE_POSITION_TYPE_E)ReadPbSet(MMINV_PHONEBOOK_SAVE_POSTION); 
}

/*****************************************************************************/
// 	Description : read contact list display option, 按bit位计算，bit0表示phone,bit1表示sim
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC MMIPB_CONTACT_DISPLAY_TYPE_E  MMIPB_ReadDisplayOpt(void)
{
   return ReadPbSet(MMINV_PHONEBOOK_DISPLAY_OPTION); 
}

#ifdef MMIPB_SELECT_DISPLAY_LOCATION_SUPPORT
/*****************************************************************************/
//  Description : handle pb SET win msg
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbSetDisplayOptWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

WINDOW_TABLE( MMIPB_SET_DISPOPTWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandlePbSetDisplayOptWinMsg ),    
    WIN_TITLE( TXT_DISPLAY_OPTION),
    WIN_ID( MMIPB_SET_DISPLAY_OPTION_WID_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIPB_SET_DISPLAY_OPTION_LIST_CTRL_ID),
    END_WIN
};

/*****************************************************************************/
// 	Description : pb设置显示选项窗口处理函数
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbSetDisplayOptWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 display_pos_nv = 0;    
    uint16 display_pos_index = 0;
    uint16 sel_index = 0;
    uint16 item_index = 0;
    
    MMI_CTRL_ID_T ctrl_id =  MMK_GetCtrlHandleByWin(win_id,MMIPB_SET_DISPLAY_OPTION_LIST_CTRL_ID);
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(PNULL == applet_ptr)
    {
        return recode;
    }
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW: 
        GUILIST_SetMaxItem(ctrl_id, MMIPB_DISPLAY_MAX, FALSE ); 
        display_pos_nv = ReadPbSet(MMINV_PHONEBOOK_DISPLAY_OPTION);
        
        for(display_pos_index = 0; display_pos_index < ARR_SIZE(s_display_opt_list); display_pos_index++)
        {
            GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
            GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

            if(display_pos_index >= MMIPB_CONTACT_DISPLAY_SIM1 && display_pos_index  <= MMIPB_DISPLAY_MAX - 1)
            {
                MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1 + display_pos_index - MMIPB_CONTACT_DISPLAY_SIM1;
                if(!MMIAPIPHONE_GetSimExistedStatus(dual_sys))
                {
                    continue;
                }
            }
            
            item_t.item_style  = GUIITEM_STYLE_ONE_LINE_RADIO;            
            item_t.item_data_ptr = &item_data;
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id = s_display_opt_list[display_pos_index];/*lint !e661*/
            item_t.user_data = display_pos_index;
            GUILIST_AppendItem( ctrl_id, &item_t );
            if(display_pos_nv == display_pos_index)
            {
                GUILIST_SetSelectedItem(ctrl_id, item_index, TRUE);
                GUILIST_SetCurItemIndex(ctrl_id, item_index);
            }
            item_index++;    
        }
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    
#ifdef MMI_PDA_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:  
#else
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif        
        display_pos_nv = 0;
        sel_index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id,sel_index,&display_pos_nv); 
        
        WritePbSet(MMINV_PHONEBOOK_DISPLAY_OPTION, display_pos_nv);
        MMK_SendMsg(applet_ptr->instance.active_win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, PNULL);
        MMK_CloseWin(win_id);
        break;


     
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//  Description : 创建pb 设置显示选项选择窗口
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIPB_OpenSetDisplayOptWin(MMI_HANDLE_T applet_handle)
{
    MMK_CreatePubListWinEx(applet_handle,
        (uint32*)MMIPB_SET_DISPOPTWIN_TAB, PNULL);
}
#endif

#if defined (MMIPB_SIMDN_SUPPORT)
/*****************************************************************************/
//  Description : 打开本机号码设置
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIPB_OpenMSISDN(MMI_HANDLE_T applet_handle)
{
    return MMIPB_OpenMSISDNWin(applet_handle, FALSE);
}

#endif

extern BOOLEAN   g_mmipb_sort_by_surname;
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN  MMIPB_SettingWriteSortBySurname(BOOLEAN value)
{
	BOOLEAN ret = FALSE;
	ERR_MNDATAMAG_CODE_E  nv_result   = ERR_MNDATAMAG_NO_ERR;   
#if 0
	nv_result = MNNV_WriteItem(MMI_GetTrueNVItemId(MMINV_PHONEBOOK_SORT_BY_SURNAME), sizeof(BOOLEAN), (void *)(&value));  
	ret = (nv_result == ERR_MNDATAMAG_NO_ERR ? TRUE : FALSE);
	g_mmipb_sort_by_surname = MMIPB_SettingsIsSortBySurname();
#else
    uint8   is_sort_by_surname = 0;
    uint16  nv_len = 0;
    
    if(TRUE == value)
    {
        is_sort_by_surname = 0;
    }
    nv_len = NVITEM_GetLength(SETTINGS_NV_PHONEBOOK_SORT_BY_SURNAME);
    nv_result = EFS_NvitemWrite(SETTINGS_NV_PHONEBOOK_SORT_BY_SURNAME, nv_len, &is_sort_by_surname, TRUE);
    
    ret = (nv_result == ERR_MNDATAMAG_NO_ERR ? TRUE : FALSE);
    g_mmipb_sort_by_surname = is_sort_by_surname;

#endif
    return ret;
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_SettingsIsSortBySurname()
{
    BOOLEAN value = TRUE;

#if 0
    ERR_MNDATAMAG_CODE_E  nv_result   = ERR_MNDATAMAG_NO_ERR;
	
	nv_result = MNNV_ReadItem(MMI_GetTrueNVItemId(MMINV_PHONEBOOK_SORT_BY_SURNAME), sizeof(BOOLEAN), (void *)(&value));  
	
    if(ERR_MNDATAMAG_NO_ERR != nv_result)
    {
        value = FALSE;
        MMIPB_SettingWriteSortBySurname(value);
    }
#else
    NVITEM_ERROR_E nv_result   = ERR_MNDATAMAG_NO_ERR;
    uint8   is_sort_by_surname = 0;
    uint16  nv_len = 0;
    
    nv_len = NVITEM_GetLength(SETTINGS_NV_PHONEBOOK_SORT_BY_SURNAME);
    nv_result = EFS_NvitemRead(SETTINGS_NV_PHONEBOOK_SORT_BY_SURNAME, nv_len, &is_sort_by_surname);
#if 0	
    if(0 == is_sort_by_surname)
    {
        value = FALSE;
    }
    else
    {
        value = TRUE;
    }
#endif
    if(NVERR_NONE != nv_result)
    {
        value = TRUE;
        MMIPB_SettingWriteSortBySurname(value);
    }
#endif

	return value;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void AppendListItemForSettings(MMI_CTRL_ID_T ctrl_id,
    uint32              item_id,
    uint32*             item_index,      
    uint16              item_style,
    MMI_TEXT_ID_T       text1_id,
    MMI_TEXT_ID_T       text2_id,
    BOOLEAN update
    )
{    
	GUILIST_ITEM_T      item_t = {0};
	GUILIST_ITEM_DATA_T item_data = {0};
	uint16               content_index = 0;
	
	item_t.item_style    = item_style;
	item_t.item_data_ptr = &item_data;
	item_data.softkey_id[LEFT_BUTTON] = TXT_NULL;
	item_data.softkey_id[MIDDLE_BUTTON] = STXT_SOFTKEY_SELECT_MK;
	item_data.softkey_id[RIGHT_BUTTON] = TXT_COMMON_COMMON_BACK;
	item_t.user_data     = item_id;
	
	if(item_style == GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS)
	{
		content_index = 0;
		item_data.item_content[content_index].item_data_type =GUIITEM_DATA_TEXT_ID;
		item_data.item_content[content_index].item_data.text_id = text1_id;
		content_index++;
		item_data.item_content[content_index].item_data_type =GUIITEM_DATA_TEXT_ID;
		item_data.item_content[content_index].item_data.text_id = text2_id;
		item_data.item_content[content_index].is_opacity = TRUE;
		GUILIST_SetItemContentOpacityFontColor(&item_data,content_index,255*0.6,255*0.6);
	}
	else if(item_style == GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT)
	{
		content_index = 0;
		item_data.item_content[content_index].item_data_type =GUIITEM_DATA_TEXT_ID;
		item_data.item_content[content_index].item_data.text_id = text1_id;
		item_data.softkey_id[MIDDLE_BUTTON] = STXT_SOFTKEY_CHANGE;
	}
	else if(item_style == GUIITEM_STYLE_ONE_LINE_TEXT_MS)
	{
		content_index = 0;
		item_data.item_content[content_index].item_data_type =GUIITEM_DATA_TEXT_ID;
		item_data.item_content[content_index].item_data.text_id = text1_id;
	}

	if(update)
	{
		GUILIST_ReplaceItem(ctrl_id, &item_t, *item_index);
	}
	else
	{
		GUILIST_AppendItem( ctrl_id, &item_t );
		(*item_index)++;
	}
	
#ifdef MMIPB_SYNC_WITH_O365
		if (MMIPB_SETTINGS_LIST_INDEX_CONNECT == item_id)
		{
			BOOLEAN is_heart_beat_on = MMIPB_SYNC_NV_Is_HeartBeat_On();
			CTRLLIST_SetToggleState(ctrl_id, MMIPB_SETTINGS_LIST_INDEX_CONNECT, is_heart_beat_on);
		}
#endif			
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL void SetSettingsList(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id)
{     
	uint32 item_index = 0;
#ifdef MMIPB_SIMCONTACT_SUPPORT
        MMIPB_MEMORY_E enStatus = MMIPB_GetContactSaveStatus();
#endif

	GUILIST_SetMaxItem(
		ctrl_id, 
		MMIPB_SETTINGS_LIST_INDEX_MAX,
		FALSE
		);
#ifdef MMIPB_SIMCONTACT_SUPPORT
        if (MMIPB_MEMORY_PHONEANDSIM == enStatus)
        {
            AppendListItemForSettings(
                ctrl_id,
                MMIPB_SETTINGS_LIST_INDEX_MEMORY,
                &item_index,
                GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
                STXT_MEMORY_A_B,
                STR_PBSEL_PHONE_AND_SIM,
                FALSE
            );
        }
        else if (MMIPB_MEMORY_PHONE == enStatus)
        {
            AppendListItemForSettings(
                ctrl_id,
                MMIPB_SETTINGS_LIST_INDEX_MEMORY,
                &item_index,
                GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
                STXT_MEMORY_A_B,
                STXT_PHONE,
                FALSE
            );
        }
        else if (MMIPB_MEMORY_SIM1 == enStatus)
        {
            AppendListItemForSettings(
                ctrl_id,
                MMIPB_SETTINGS_LIST_INDEX_MEMORY,
                &item_index,
                GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
                STXT_MEMORY_A_B,
                #ifdef MMI_MULTI_SIM_SYS_SINGLE
                STR_ILIST_SIM_EXT01,   
                #else
                STR_CONTACT_LIST_MEMORY_S1,
                #endif
                FALSE
            );
        }
        else
        {
            AppendListItemForSettings(
                ctrl_id,
                MMIPB_SETTINGS_LIST_INDEX_MEMORY,
                &item_index,
                GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
                STXT_MEMORY_A_B,
                STR_CONTACT_LIST_MEMORY_S2,
                FALSE
            );
        }
#endif //MMIPB_SIMCONTACT_SUPPORT
#if 0
	AppendListItemForSettings(
		ctrl_id,
		MMIPB_SETTINGS_LIST_INDEX_SORT_BY,
		&item_index,
		GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
		STR_CONTA_LIST_SORT_EXT01,
		MMIPB_SettingsIsSortBySurname()?STR_CONTA_LIST_LAST_NAME_EXT01:STR_CONTA_LIST_FIRST_NAME_EXT01,
		FALSE
	);

	#if defined (MMIPB_SIMDN_SUPPORT)
	AppendListItemForSettings(
		ctrl_id,
		MMIPB_SETTINGS_LIST_INDEX_SDN,
		&item_index,
		GUIITEM_STYLE_ONE_LINE_TEXT_MS,
		TXT_PB_SDN,
		TXT_NULL,
		FALSE
	);
	#endif
#endif
#ifdef MMI_BT_PBAP_SUPPORT
    AppendListItemForSettings(
        ctrl_id,
        MMIPB_SETTINGS_LIST_INDEX_ANDROID_SYNC,
        &item_index,
        GUIITEM_STYLE_ONE_LINE_TEXT_MS,
        STR_CONTACT_SYNC_VIA_BT,
        TXT_NULL,
        FALSE
        );
#endif
#ifdef MMI_SHOW_MSA
	#ifdef MMIPB_SYNC_WITH_O365
	AppendListItemForSettings(
		ctrl_id,
		MMIPB_SETTINGS_LIST_INDEX_CONNECT,
		&item_index,
		GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT,
		STR_ONEDRIVE_ILIST_CONNECT_EXT01,
		TXT_NULL,
		FALSE
	);
	AppendListItemForSettings(
		ctrl_id,
		MMIPB_SETTINGS_LIST_INDEX_SYNC,
		&item_index,
		GUIITEM_STYLE_ONE_LINE_TEXT_MS,
		STR_ONEDRIVE_LIST_SYNC_EXT01,
		TXT_NULL,
		FALSE
	);
	#endif
#endif
}
#ifdef MMIPB_SIMCONTACT_SUPPORT
LOCAL MMIPB_MEMORY_E g_mmipb_save_by_memory = MMIPB_MEMORY_INIT;
#endif //MMIPB_SIMCONTACT_SUPPORT
#ifdef MMIPB_SIMCONTACT_SUPPORT
/*****************************************************************************/
//  Description                         : read contact memory status from nv
//  Global resource dependence          : mmipb_app.h 
//  Author                              : zhongping.shi
//  Input                               : none
//  Output                              : none
//  Return                              : MMIPB_MEMORY_E nv_status
//  Caution                             : none
/*****************************************************************************/
PUBLIC MMIPB_MEMORY_E MMIPB_ReadContactSaveStatusFromNV(void)
{
    uint16             uilen = 0;
    uint32           uistatus = 0;
    MMIPB_MEMORY_E nv_status = MMIPB_MEMORY_INIT;
    NVITEM_ERROR_E nv_result = ERR_MNDATAMAG_WRITE_READ_NV_ERROR;

    nv_result = MNNV_ReadItem(MMI_GetTrueNVItemId(SETTINGS_NV_PHONEBOOK_MEMSTATUS), sizeof(uint32), (void *)(&uistatus));  

    if (ERR_MNDATAMAG_NO_ERR == nv_result)
    {
        nv_status = uistatus;
    }
    else
    {
        SCI_TRACE_LOW("[MMIPB] Failed to get contact save status from nv,"
                      "len = %d, nv_result = 0x%x.", uilen, nv_result);
    }
    
    return nv_status;
}

/*****************************************************************************/
//  Description                         : write contact memory status to nv
//  Global resource dependence          : none 
//  Author                              : zhongping.shi
//  Input                               : MMIPB_MEMORY_E enstatus
//  Output                              : none
//  Return                              : NVITEM_ERROR_E nv_result
//  Caution                             : none
/*****************************************************************************/
LOCAL NVITEM_ERROR_E mmipb_WriteContactSaveStatusToNV(MMIPB_MEMORY_E enstatus)
{
    NVITEM_ERROR_E  nv_result   = ERR_MNDATAMAG_NO_ERR;   
    return nv_result;
}

/*****************************************************************************/
//  Description                         : Get contact memory status
//  Global resource dependence          : none 
//  Author                              : zhongping.shi
//  Input                               : none
//  Output                              : none
//  Return                              : MMIPB_MEMORY_E g_mmipb_save_by_memory
//  Caution                             : none
/*****************************************************************************/
PUBLIC MMIPB_MEMORY_E MMIPB_GetContactSaveStatus(void)
{
    return g_mmipb_save_by_memory;
}

/*****************************************************************************/
//  Description                         : Get contact memory status
//  Global resource dependence          : none 
//  Author                              : zhongping.shi
//  Input                               : MMIPB_MEMORY_E enStatus
//  Output                              : none
//  Return                              : none
//  Caution                             : none
/*****************************************************************************/
PUBLIC void MMIPB_SetContactSaveStatus(MMIPB_MEMORY_E enStatus)
{
    uint16             uilen = 0;
    NVITEM_ERROR_E nv_result = ERR_MNDATAMAG_NO_ERR;
    
    nv_result = MNNV_WriteItem(MMI_GetTrueNVItemId(SETTINGS_NV_PHONEBOOK_MEMSTATUS), sizeof(MMIPB_MEMORY_E), (void *)(&enStatus));  

    if (ERR_MNDATAMAG_NO_ERR == nv_result)
    {
        g_mmipb_save_by_memory = enStatus;
    }
    else
    {
        SCI_TRACE_LOW("[MMIPB] Failed to get contact save status from nv,"
                                 "enStatus = %d, len = %d, nv_result = 0x%x.", enStatus, uilen, nv_result);
    }
    
    return;
}
#endif //MMIPB_SIMCONTACT_SUPPORT

#ifdef MMIPB_SYNC_WITH_O365

extern PUBLIC void contact_sync_get_token_cb( int result, char*  buf, int buflen);

LOCAL void MMIPB_OpenFirstSyncConfirmDialogWin(void);
LOCAL MMI_RESULT_E  HandleEntryMsaAccountTipWinMsg(
											  MMI_WIN_ID_T win_id, 
											  MMI_MESSAGE_ID_E msg_id, 
											  DPARAM param
											  );

//PB_Sync start,20151026
LOCAL MMI_RESULT_E  HandleEntryMsaAccountTipWinMsg2(
											  MMI_WIN_ID_T win_id, 
											  MMI_MESSAGE_ID_E msg_id, 
											  DPARAM param
											  );
LOCAL MMI_RESULT_E  HandleSettingMsaAccountTipWinMsg(
											  MMI_WIN_ID_T win_id, 
											  MMI_MESSAGE_ID_E msg_id, 
											  DPARAM param
											  );

LOCAL BOOLEAN s_bSyncCheckWaitDialogCanceled = FALSE;

WINDOW_TABLE( MMIPB_ENTRY_MSA_ACCOUNT_TIP_WIN_TAB ) = 
{
	WIN_FUNC((uint32)HandleEntryMsaAccountTipWinMsg),    
	WIN_ID(MMIPB_ENTRY_MSA_ACCOUNT_TIP_WIN_ID),
	//pb sync start,20151019
	WIN_SOFTKEY(TXT_INFO,STXT_SOFTKEY_OK_MK, STXT_SOFTKEY_SKIP_RSK),
	//pb sync end
	CREATE_RICHTEXT_CTRL(MMIPB_ENTRY_MSA_ACCOUNT_TIP_CTRL_ID),
	END_WIN
};

//PB_Sync start,20151026
WINDOW_TABLE( MMIPB_ENTRY_MSA_ACCOUNT_TIP_WIN_TAB2 ) = 
{
	WIN_FUNC((uint32)HandleEntryMsaAccountTipWinMsg2),    
	WIN_ID(MMIPB_ENTRY_MSA_ACCOUNT_TIP_WIN_ID),
	WIN_SOFTKEY(TXT_INFO,STXT_SOFTKEY_OK_MK, STXT_SOFTKEY_SKIP_RSK),
	CREATE_RICHTEXT_CTRL(MMIPB_ENTRY_MSA_ACCOUNT_TIP_CTRL_ID),
	END_WIN
};
//PB_Sync end

WINDOW_TABLE( MMIPB_SETTING_MSA_ACCOUNT_TIP_WIN_TAB ) = 
{
	WIN_FUNC((uint32)HandleSettingMsaAccountTipWinMsg),    
	WIN_ID(MMIPB_SETTING_MSA_ACCOUNT_TIP_WIN_ID),

	WIN_SOFTKEY(TXT_NULL, TXT_NULL,TXT_COMMON_COMMON_BACK),
	CREATE_RICHTEXT_CTRL(MMIPB_SETTING_MSA_ACCOUNT_TIP_CTRL_ID),
	END_WIN
};

//zouruoqi add for prompting error windows
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMIPB_OpenSyncErrorDialogWinMsg
(
 MMI_WIN_ID_T     win_id, 
 MMI_MESSAGE_ID_E msg_id, 
 DPARAM           param 
 )
{
	MMI_RESULT_E        recode = MMI_RESULT_TRUE;
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		break;
	case MSG_APP_OK:
	case MSG_CTL_OK:
		 break;
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		MMK_CloseWin(win_id);
		MMIPB_UpdatePBSyncSettingMenu();
		break;
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
		MMK_CloseWin(win_id);
		MMIPB_UpdatePBSyncSettingMenu();
		break;
	case MSG_CLOSE_WINDOW:
		break;
	default:
		
		recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
		break;
	}
	return (recode);
}

//end zouruoqi



//zouruoqi add for prompting error windows
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_OpenSyncErrorPermissionDialogWin(void)
{
	MMI_WIN_ID_T alert_id = MMIPB_ALERT_INFO_WIN_ID;
	SCI_TRACE_LOW("[MMIPB]OpenSyncErrorPermissionDialogWin ");
	
	MMIPUB_OpenConfirmationDialogByTextId(
		PNULL,
		STR_CONTACT_SYNC_NO_PERMISSON,
		TXT_NULL,
		IMAGE_NULL,
		&alert_id,
		PNULL,
		MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
		HandleMMIPB_OpenSyncErrorDialogWinMsg
		);				
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_OpenSyncErrorNetworkDialogWin(void)
{
	MMI_WIN_ID_T alert_id = MMIPB_ALERT_INFO_WIN_ID;
	SCI_TRACE_LOW("[MMIPB]OpenSyncErrorNetworkDialogWin ");
	MMIPUB_OpenConfirmationDialogByTextId(
		PNULL,
		STR_CONTACT_SYNC_FAIL_NETWORK,
		TXT_NULL,
		IMAGE_NULL,
		&alert_id,
		PNULL,
		MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
		HandleMMIPB_OpenSyncErrorDialogWinMsg
		);				
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_OpenSyncConflictDialogWin(void)
{
	//MMI_WIN_ID_T alert_id = MMIPB_ALERT_INFO_WIN_ID;	
	MMIPUB_OpenConfirmationDialogByTextId(
		PNULL,
		STR_CONTACTS_NOT_READY_EXT01,
		TXT_NULL,
		IMAGE_NULL,
		PNULL,
		PNULL,
		MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
		HandleMMIPB_OpenSyncErrorDialogWinMsg
		);	
    SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_OpenSyncConflictDialogWin");
}
//end zouruoqi
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void MMIPB_SetSyncCheckWaitDialogCanceled(BOOLEAN canceled)
{
	s_bSyncCheckWaitDialogCanceled = canceled;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsSyncCheckWaitDialogCanceled(void)
{
	return s_bSyncCheckWaitDialogCanceled;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_Is_MSA_Account_Exist(void)
{
	BOOLEAN bRet = FALSE;

#ifdef MMIPB_SYNC_USE_PC_TOKEN
	return TRUE;
#else	
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_Is_MSA_Account_Exist");

#ifdef MMI_SHOW_MSA
	bRet = MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CONTACT_SYNC);

	if (bRet && !MMIPB_SYNC_NV_Is_MSA_Sign_In())
	{
		SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_NV_Set_MSA_Sign_In is FALSE when MMIAPIMSA_IsAccountExist, correcit it!!!");
		MMIPB_SYNC_NV_Set_MSA_Sign_In(TRUE);
	}

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_Is_MSA_Account_Exist, bRet=%d", bRet);
#endif

    return bRet;
#endif
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void MMIPB_Create_MSA_Account(MSA_TOKEN_CALLBACK_FUNC func)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_Create_MSA_Account");

#if defined(BROWSER_SUPPORT_WEBKIT) && ((!defined(WIN32)) || (defined(_MSC_VER ) && (_MSC_VER >= 1400)))
#ifdef MMIPB_SYNC_USE_PC_TOKEN
    MMIAPIPC_get_token(TRUE, func);
#else
    MMIAPIMSA_get_token(TRUE ,MSA_APP_TYPE_CONTACT_SYNC,  func);
#endif
#endif	
	
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_Create_MSA_Account");
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSyncResultDialogWinMsg
(
 MMI_WIN_ID_T     win_id, 
 MMI_MESSAGE_ID_E msg_id, 
 DPARAM           param 
 )
{
	MMI_RESULT_E        recode = MMI_RESULT_TRUE;
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		break;
	case MSG_APP_OK:
	case MSG_CTL_OK:
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		break;
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
		MMK_CloseWin(win_id);
		break;
	case MSG_CLOSE_WINDOW:
		break;
	default:
		recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
		break;
	}
	return (recode);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_OpenSyncResultDialogWin(MMI_TEXT_ID_T title_str_id, MMI_TEXT_ID_T content_str_id)
{
	MMI_STRING_T title = {0};
	MMI_GetLabelTextByLang(title_str_id, &title);
	MMIPUB_OpenTextDialogByTextId(
		PNULL,
		TXT_NULL,//title_str_id,
		content_str_id,
		IMAGE_NULL,
		PNULL,
		PNULL,
		MMIPUB_SOFTKEY_DIALOG_OK,
		HandleSyncResultDialogWinMsg
		);		
	MMIPUB_SetTextDialogTitle(MMIPUB_ALERT_WIN_ID,&title,TRUE);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_ShowUpToDateDialog(void)
{
	MMIPB_OpenSyncResultDialogWin(STR_CONTA_UP_TO_DATE_EXT01, STR_CONTACT_SYNCED_EXT01);	
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_ShowSyncInBackgroundDialog(void)
{
	MMIPB_OpenSyncResultDialogWin(STR_CONTA_COPYING_EXT01, STR_BACKGROUND_OPERATION_EXT01);	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFirstSyncConfirmDialogWinMsg
(
 MMI_WIN_ID_T     win_id, 
 MMI_MESSAGE_ID_E msg_id, 
 DPARAM           param 
 )
{
	MMI_RESULT_E        recode = MMI_RESULT_TRUE;
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		 break;
	case MSG_APP_OK:
	case MSG_CTL_OK:
		 break;
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		//MMIPB_SYNC_NV_Set_First_Sync(FALSE);
		MMK_CloseWin(win_id);
		break;
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
		

		if (MMIPB_SYNC_Start_FirstSync())
		{
			MMIPB_ShowSyncInBackgroundDialog();
		}
		else
		{
			SCI_TRACE_LOW("[MMIPBSYNC] HandleFirstSyncConfirmDialogWinMsg MMIPB_SYNC_Start_FirstSync failed.");
            MMIPB_OpenSyncConflictDialogWin();
		}
		MMK_CloseWin(win_id);
		break;
	case MSG_CLOSE_WINDOW:
		break;
	default:
		recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
		break;
	}
	return (recode);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void MMIPB_OpenFirstSyncConfirmDialogWin(void)
{
	MMI_WIN_ID_T alert_id = MMIPB_ALERT_INFO_WIN_ID;	
	MMIPUB_OpenConfirmationDialogByTextId(
		PNULL,
		STR_CONNECT_TO_INTERNET_EXT01,
		TXT_NULL,
		IMAGE_NULL,
		&alert_id,
		PNULL,
		MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
		HandleFirstSyncConfirmDialogWinMsg
		);				
}

/*****************************************************************************/
// 	Description: handle first pb sync MSA account tip window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEntryMsaAccountTipWinMsg(
											  MMI_WIN_ID_T win_id, 
											  MMI_MESSAGE_ID_E msg_id, 
											  DPARAM param
											  )
{
	GUI_RECT_T      rect = MMITHEME_GetClientRect();
	MMI_RESULT_E    recode = MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
	MMI_CTRL_ID_T ctrl_id = MMIPB_ENTRY_MSA_ACCOUNT_TIP_CTRL_ID;
	
	lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
	lcd_dev_info.block_id = GUI_BLOCK_MAIN;
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
			GUIRICHTEXT_ITEM_T item_data = {0};    /*lint !e64*/ 
			MMI_STRING_T string = {0}; 
			uint16    index= 0;

			//pb sync start
			GUIRICHTEXT_ITEM_T item_dataIcon = {0};
			MMI_STRING_T stringicon = {0}; 
			GUIRICHTEXT_ITEM_T item_dataPrompt = {0};
			MMI_STRING_T stringprompt = {0}; 
			GUIRICHTEXT_ITEM_T item_dataSpace = {0};
			MMI_STRING_T stringSpace = {0};
			//设置右软键
			GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_SKIP_RSK, RIGHT_BUTTON, TRUE);
			//pb sync end
			
			item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
			item_data.text_type = GUIRICHTEXT_TEXT_BUF;
			
			MMI_GetLabelTextByLang(STR_TITLE_CONTACTS_SAFE, &string);
			item_data.text_data.buf.len = string.wstr_len;
			item_data.text_data.buf.str_ptr = string.wstr_ptr;
			item_data.text_frame_type = GUIRICHTEXT_FRAME_NONE;
			item_data.text_set_align = TRUE;
			item_data.setting_align = ALIGN_HMIDDLE;
			CTRLRICHTEXT_SetTxtMargin(ctrl_id,0);
			CTRLRICHTEXT_SetBorderSpace(ctrl_id,0);
			CTRLRICHTEXT_SetItemBorderSpace(ctrl_id,0);
			//CTRLRICHTEXT_SetFont(ctrl_id,ASC_ME_FONT_38);
			GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
			GUIRICHTEXT_SetFocusItem(ctrl_id, 0);

			//pb sync start
			//添加图标
			item_dataIcon.img_type = GUIRICHTEXT_IMAGE_RES;
			item_dataIcon.text_type = GUIRICHTEXT_TEXT_NONE;
			item_dataIcon.img_data.res.id = IMAGE_custom_MS_identity_promo_screen_contacts;  //图片资源
			item_dataIcon.img_setting_align = GUIRICHTEXT_IMG_ALIGN_MIDDLE;
			//添加提示
			item_dataPrompt.img_type = GUIRICHTEXT_IMAGE_NONE;
			item_dataPrompt.text_type = GUIRICHTEXT_TEXT_BUF;			
			MMI_GetLabelTextByLang(STR_QUERY_CONTACTS_SAFE, &stringprompt);
			item_dataPrompt.text_data.buf.len = stringprompt.wstr_len;
			item_dataPrompt.text_data.buf.str_ptr = stringprompt.wstr_ptr;
			item_dataPrompt.text_set_align = TRUE;
			item_dataPrompt.setting_align = ALIGN_HMIDDLE;
			item_dataPrompt.text_set_font = TRUE;
			item_dataPrompt.setting_font = CAF_FONT_16;
			//留白区域
			item_dataSpace.img_type = GUIRICHTEXT_IMAGE_NONE;
			item_dataSpace.text_type = GUIRICHTEXT_TEXT_BUF;			
			item_dataSpace.text_data.buf.len = stringSpace.wstr_len;
			item_dataSpace.text_data.buf.str_ptr = stringSpace.wstr_ptr;
			//pb sync end

			//pb sync start
			GUIRICHTEXT_AddItem(ctrl_id, &item_dataSpace, &index);
			GUIRICHTEXT_AddItem(ctrl_id, &item_dataIcon, &index);
			GUIRICHTEXT_AddItem(ctrl_id, &item_dataSpace, &index);
			GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
			GUIRICHTEXT_AddItem(ctrl_id, &item_dataPrompt, &index);
			CTRLRICHTEXT_SetBgColor(ctrl_id, MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_2));
			MMK_SetAtvCtrl(win_id, ctrl_id);
		}
		break;
		
	case MSG_FULL_PAINT:
		LCD_FillRect(&lcd_dev_info, rect, MMI_GRAY_WHITE_COLOR);
		break;
		
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
	case MSG_APP_WEB:
		break;
	case MSG_CTL_OK:
		//点击左键
		MMK_CreateWin((uint32*)MMIPB_SETTING_MSA_ACCOUNT_TIP_WIN_TAB, PNULL);
		break;
	case MSG_CTL_MIDSK:  //点击中键
		//MMIPB_SYNC_NV_Set_HeartBeat(TRUE);
		MMIPB_OpenFirstSyncConfirmDialogWin();
		MMK_CloseWin(win_id);
		break;
	case MSG_CLOSE_WINDOW:
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
}

/*****************************************************************************/
// 	Description: entry first pb sync MSA account tip window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_EntryMsaAccountTip(void)
{
    MMK_CreateWin((uint32*)MMIPB_ENTRY_MSA_ACCOUNT_TIP_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : get token cb func
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_MSA_Get_Token_Func_For_FirstSync( int result, char*  buf, uint16 buflen)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_MSA_Get_Token_Func_For_FirstSync,result=%d", result);
	if(MSA_OK == result)
	{
		contact_sync_get_token_cb(result, buf, buflen);
		MMIPB_SYNC_NV_Set_MSA_Sign_In(TRUE);
		MMIAPIMSA_Exit();

		MMIPB_EntryMsaAccountTip();
	}
	else if(MSA_CANCEL == result)
	{
		//Go to first use sim card ??
		//MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);  
	}
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_MSA_Get_Token_Func_For_FirstSync");
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleConnectConfirmDialogWinMsg
(
 MMI_WIN_ID_T     win_id, 
 MMI_MESSAGE_ID_E msg_id, 
 DPARAM           param 
 )
{
	MMI_RESULT_E        recode = MMI_RESULT_TRUE;
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		break;
	case MSG_APP_OK:
	case MSG_CTL_OK:
		 break;
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		MMK_CloseWin(win_id);
		break;
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
		
		MMIPB_SYNC_NV_Set_HeartBeat(TRUE);
		if (MMIPB_SYNC_Start_HeavySync(TRUE))
		{
			MMIPB_ShowSyncInBackgroundDialog();
		}
		else
		{
			SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_OpenConnectConfirmDialogWin MMIPB_SYNC_Start_HeavySync failed.");
            MMIPB_OpenSyncConflictDialogWin();
		}
		MMK_CloseWin(win_id);
		break;
	case MSG_CLOSE_WINDOW:
		break;
	default:
		recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
		break;
	}
	return (recode);
}



/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_OpenConnectConfirmDialogWin(void)
{
	MMI_WIN_ID_T alert_id = MMIPB_ALERT_INFO_WIN_ID;	
	MMIPUB_OpenConfirmationDialogByTextId(
		PNULL,
		STR_CONNECT_TO_INTERNET_EXT01,
		TXT_NULL,
		IMAGE_NULL,
		&alert_id,
		PNULL,
		MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
		HandleConnectConfirmDialogWinMsg
		);				
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_DoFirstSync(void)
{
#ifdef MMI_SHOW_MSA
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_DoFirstSync");
	
	if (FALSE == MMIPB_Is_MSA_Account_Exist())
	{
		MMIPB_Create_MSA_Account(MMIPB_MSA_Get_Token_Func_For_FirstSync);
	}
	else
	{
		MMIPB_EntryMsaAccountTip();
	}
#endif
	MMIPB_SYNC_NV_Set_First_Sync(FALSE);
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_DoFirstSync");
}

/*****************************************************************************/
//  Description : get token cb func
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_MSA_Get_Token_Func_For_Heartbeat( int result, char*  buf, uint16 buflen)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_MSA_Get_Token_Func_For_Heartbeat,result=%d", result);
	
	if(MSA_OK == result)
	{
		MMIPB_SYNC_NV_Set_HeartBeat(TRUE);
		
		contact_sync_get_token_cb(result, buf, buflen);

		MMIPB_SYNC_NV_Set_MSA_Sign_In(TRUE);		
		
		//How to show setting screen??
		MMIAPIMSA_Exit();
		//zouruoqi add for sync after connect
		MMIPB_OpenConnectConfirmDialogWin();
		//end
	}
	else if(MSA_CANCEL == result)
	{
		//How to show setting screen??
		MMIPB_UpdatePBSyncSettingMenu();
	}
	
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_MSA_Get_Token_Func_For_Heartbeat");
}

//PB_Sync start,20151026
LOCAL MMI_RESULT_E  HandleEntryMsaAccountTipWinMsg2(
											  MMI_WIN_ID_T win_id, 
											  MMI_MESSAGE_ID_E msg_id, 
											  DPARAM param
											  )
{
	GUI_RECT_T      rect = MMITHEME_GetClientRect();
	MMI_RESULT_E    recode = MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
	MMI_CTRL_ID_T ctrl_id = MMIPB_ENTRY_MSA_ACCOUNT_TIP_CTRL_ID;
	
	lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
	lcd_dev_info.block_id = GUI_BLOCK_MAIN;
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
			GUIRICHTEXT_ITEM_T item_data = {0};    /*lint !e64*/ 
			MMI_STRING_T string = {0}; 
			uint16    index= 0;

			//pb sync start
			GUIRICHTEXT_ITEM_T item_dataIcon = {0};
			MMI_STRING_T stringicon = {0}; 
			GUIRICHTEXT_ITEM_T item_dataPrompt = {0};
			MMI_STRING_T stringprompt = {0}; 
			GUIRICHTEXT_ITEM_T item_dataSpace = {0};
			MMI_STRING_T stringSpace = {0};
			//设置右软键
			GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_SKIP_RSK, RIGHT_BUTTON, TRUE);
			//pb sync end
			
			item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
			item_data.text_type = GUIRICHTEXT_TEXT_BUF;
			
			MMI_GetLabelTextByLang(STR_TITLE_CONTACTS_SAFE, &string);
			item_data.text_data.buf.len = string.wstr_len;
			item_data.text_data.buf.str_ptr = string.wstr_ptr;
			item_data.text_frame_type = GUIRICHTEXT_FRAME_NONE;
			item_data.text_set_align = TRUE;
			item_data.setting_align = ALIGN_HMIDDLE;
			CTRLRICHTEXT_SetTxtMargin(ctrl_id,0);
			CTRLRICHTEXT_SetBorderSpace(ctrl_id,0);
			CTRLRICHTEXT_SetItemBorderSpace(ctrl_id,0);
			//CTRLRICHTEXT_SetFont(ctrl_id,ASC_ME_FONT_38);
			GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
			GUIRICHTEXT_SetFocusItem(ctrl_id, 0);

			//pb sync start
			//添加图标
			item_dataIcon.img_type = GUIRICHTEXT_IMAGE_RES;
			item_dataIcon.text_type = GUIRICHTEXT_TEXT_NONE;
			item_dataIcon.img_data.res.id = IMAGE_custom_MS_identity_promo_screen_contacts;  //图片资源
			item_dataIcon.img_setting_align = GUIRICHTEXT_IMG_ALIGN_MIDDLE;
			//添加提示
			item_dataPrompt.img_type = GUIRICHTEXT_IMAGE_NONE;
			item_dataPrompt.text_type = GUIRICHTEXT_TEXT_BUF;			
			MMI_GetLabelTextByLang(STR_QUERY_CONTACTS_SAFE, &stringprompt);
			item_dataPrompt.text_data.buf.len = stringprompt.wstr_len;
			item_dataPrompt.text_data.buf.str_ptr = stringprompt.wstr_ptr;
			item_dataPrompt.text_set_align = TRUE;
			item_dataPrompt.setting_align = ALIGN_HMIDDLE;
			item_dataPrompt.text_set_font = TRUE;
			item_dataPrompt.setting_font = CAF_FONT_16;
			//留白区域
			item_dataSpace.img_type = GUIRICHTEXT_IMAGE_NONE;
			item_dataSpace.text_type = GUIRICHTEXT_TEXT_BUF;			
			item_dataSpace.text_data.buf.len = stringSpace.wstr_len;
			item_dataSpace.text_data.buf.str_ptr = stringSpace.wstr_ptr;
			//pb sync end

			//pb sync start
			GUIRICHTEXT_AddItem(ctrl_id, &item_dataSpace, &index);
			GUIRICHTEXT_AddItem(ctrl_id, &item_dataIcon, &index);
			GUIRICHTEXT_AddItem(ctrl_id, &item_dataSpace, &index);
			GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
			GUIRICHTEXT_AddItem(ctrl_id, &item_dataPrompt, &index);
			CTRLRICHTEXT_SetBgColor(ctrl_id, MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_2));
			MMK_SetAtvCtrl(win_id, ctrl_id);
		}
		break;
		
	case MSG_FULL_PAINT:
		LCD_FillRect(&lcd_dev_info, rect, MMI_GRAY_WHITE_COLOR);
		break;
		
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
	case MSG_APP_WEB:
		break;
	case MSG_CTL_OK:
		//点击左键
		MMK_CreateWin((uint32*)MMIPB_SETTING_MSA_ACCOUNT_TIP_WIN_TAB, PNULL);
		break;
	case MSG_CTL_MIDSK:  //点击中键
		MMIPB_Create_MSA_Account(MMIPB_MSA_Get_Token_Func_For_Heartbeat);
		MMK_CloseWin(win_id);
		break;
	case MSG_CLOSE_WINDOW:
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
}
//PB_Sync end
/*****************************************************************************/
// 	Description: handle heavy pb sync MSA account tip window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingMsaAccountTipWinMsg(
											  MMI_WIN_ID_T win_id, 
											  MMI_MESSAGE_ID_E msg_id, 
											  DPARAM param
											  )
{
	GUI_RECT_T      rect = MMITHEME_GetClientRect();
	MMI_RESULT_E    recode = MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
	MMI_CTRL_ID_T ctrl_id = MMIPB_SETTING_MSA_ACCOUNT_TIP_CTRL_ID;
	//pb_sync add start,20151103
	MMI_STRING_T title_str = {0}; 
	//pb_sync add end
	lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
	lcd_dev_info.block_id = GUI_BLOCK_MAIN;
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
			GUIRICHTEXT_ITEM_T item_data = {0};    /*lint !e64*/ 
			MMI_STRING_T string = {0}; 
			uint16    index= 0;

			//添加标题
			MMI_GetLabelTextByLang(STR_MSA_TITLE_ACCOUNT, &title_str );
        		if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str .wstr_ptr, title_str .wstr_len))
        		{
            			GUIWIN_UpdateStb();
        		}
			
			item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
			item_data.text_type = GUIRICHTEXT_TEXT_BUF;
			
			MMI_GetLabelTextByLang(STR_MSA_OUTLOOK_CONTACTS_EXT01, &string);
			item_data.tag_type = GUIRICHTEXT_TAG_PARSE_URL;
			item_data.text_data.buf.len = string.wstr_len;
			item_data.text_data.buf.str_ptr = string.wstr_ptr;
			item_data.text_set_font = TRUE;
			item_data.setting_font = CAF_FONT_18;
			GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

			GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
			GUIRICHTEXT_SetFocusItem(ctrl_id, 0);
			
			MMK_SetAtvCtrl(win_id, ctrl_id);
		}
		
		break;
	case MSG_GET_FOCUS:
		//添加标题
		MMI_GetLabelTextByLang(STR_MSA_TITLE_ACCOUNT, &title_str );
        	if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str .wstr_ptr, title_str .wstr_len))
        	{
            		GUIWIN_UpdateStb();
        	}
		break;
	case MSG_FULL_PAINT:
		LCD_FillRect(&lcd_dev_info, rect, MMI_GRAY_WHITE_COLOR);
		break;
		
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
	case MSG_APP_WEB:
	case MSG_CTL_OK:
		break;
	case MSG_CTL_MIDSK:
	case MSG_APP_OK:
		

		MMIPB_Create_MSA_Account(MMIPB_MSA_Get_Token_Func_For_Heartbeat);
		MMK_CloseWin(win_id);
		break;

	case MSG_CLOSE_WINDOW:
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
}

/*****************************************************************************/
// 	Description: entry email tip window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_SettingMsaAccountTip(void)
{
	//PB_Sync start,20151026
   //MMK_CreateWin((uint32*)MMIPB_SETTING_MSA_ACCOUNT_TIP_WIN_TAB, PNULL);
   MMK_CreateWin((uint32*)MMIPB_ENTRY_MSA_ACCOUNT_TIP_WIN_TAB2, PNULL);
	//PB_Sync end
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleConnectNoTokenConfirmDialogWinMsg
(
 MMI_WIN_ID_T     win_id, 
 MMI_MESSAGE_ID_E msg_id, 
 DPARAM           param 
 )
{
	MMI_RESULT_E        recode = MMI_RESULT_TRUE;
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		break;
	case MSG_APP_OK:
	case MSG_CTL_OK:
		 break;
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		MMK_CloseWin(win_id);
		break;
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
		
		MMIPB_SettingMsaAccountTip();
		MMK_CloseWin(win_id);
		break;
	case MSG_CLOSE_WINDOW:
		break;
	default:
		recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
		break;
	}
	return (recode);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_OpenConnectNoTokenConfirmDialogWin(void)
{
	MMI_WIN_ID_T alert_id = MMIPB_ALERT_INFO_WIN_ID;	
	MMIPUB_OpenConfirmationDialogByTextId(
		PNULL,
		STR_MSA_CONTACT_SYNC_EXT01,
		TXT_NULL,
		IMAGE_NULL,
		&alert_id,
		PNULL,
		MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
		HandleConnectNoTokenConfirmDialogWinMsg
		);				
}



/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_DoSyncHeartBeartOn(void)
{
#ifdef MMI_SHOW_MSA
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_DoSyncHeartBeartOn");
	
	if (FALSE == MMIPB_Is_MSA_Account_Exist())
	{
		MMIPB_OpenConnectNoTokenConfirmDialogWin();
	}
	else
	{
		//MMIPB_SYNC_NV_Set_HeartBeat(TRUE);
		MMIPB_OpenConnectConfirmDialogWin();		
	}
	
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_DoSyncHeartBeartOn");
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSyncConfirmDialogWinMsg
(
 MMI_WIN_ID_T     win_id, 
 MMI_MESSAGE_ID_E msg_id, 
 DPARAM           param 
 )
{
    

	MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    SCI_TRACE_LOW("+[MMIPBSYNC] HandleSyncConfirmDialogWinMsg1 ");
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		break;

	case MSG_APP_OK:
	case MSG_CTL_OK:
		 break;
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
	case	MSG_APP_RED:
        SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_OpenSyncConfirmDialogWin1 MSG_APP_RED: HeavySyncCanceled");
		MMIPB_SYNC_HeavySyncCanceled();
		MMK_CloseWin(win_id);
		break;

	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
		MMIPB_SYNC_HeavySyncConfirmed();
		
		MMK_CloseWin(win_id);
		MMIPB_ShowSyncInBackgroundDialog();
		break;

	case MSG_CLOSE_WINDOW:
		break;

	default:
		recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
		break;
	}
	return (recode);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_OpenSyncConfirmDialogWin(void)
{
   

	MMI_WIN_ID_T alert_id = MMIPB_ALERT_INFO_WIN_ID;	
	MMIPUB_OpenConfirmationDialogByTextId(
		PNULL,
		STR_CONTACT_SYNC_EXT01,
		TXT_NULL,
		IMAGE_NULL,
		&alert_id,
		PNULL,
		MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
		HandleSyncConfirmDialogWinMsg
		);	
     SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_OpenSyncConfirmDialogWin1");
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSyncCheckWaitDialogWinMsg
(
 MMI_WIN_ID_T     win_id, 
 MMI_MESSAGE_ID_E msg_id, 
 DPARAM           param 
 )
{
	MMI_RESULT_E        recode = MMI_RESULT_TRUE;
	static uint8        s_wait_ready_timer_id = 0;
	MMI_HANDLE_T    applet_handle = {0};
	MMIPB_APPLET_T* applet_ptr = PNULL;
	
	applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
	if(PNULL == applet_ptr)
	{
		return recode;
	}
	applet_handle = applet_ptr->caf_applet.app_handle;
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		MMIPB_SetSyncCheckWaitDialogCanceled(FALSE);
		break;

	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
	case MSG_APP_OK:
	case MSG_CTL_OK:
		 break;
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
    case	MSG_APP_RED:
		MMIPB_SetSyncCheckWaitDialogCanceled(TRUE);
		MMK_CloseWin(win_id);
		break;

	case MSG_CLOSE_WINDOW:
		break;
	default:
		recode = MMIPUB_HandleProgressWinMsg(win_id, msg_id, param);
		break;
	}
	return (recode);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_OpenSyncCheckWaitDialogWin(MMI_HANDLE_T applet_handle)
{
	BOOLEAN bRet = MMIPB_SYNC_Start_HeavySync(FALSE);
	if (bRet)
	{
		MMI_WIN_ID_T alert_id = MMIPB_SYNC_CHECK_WAITING_WIN_ID;
		MMI_STRING_T string = {0}; 
		MMI_GetLabelTextByLang(STR_WAIT_CHECK_UPDATE_EXT01, &string);

		MMIPUB_OpenProgressWaitingIndWinByTextPtrEx(
			MMK_GetFirstAppletHandle(),
			&string,
			&alert_id,
			PNULL,
			MMIPUB_SOFTKEY_DIALOG_CLOSE,
			HandleSyncCheckWaitDialogWinMsg,
			PNULL
			);
	}
	else
	{
		SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_OpenSyncCheckWaitDialogWin MMIPB_SYNC_Start_HeavySync failed.");
        MMIPB_OpenSyncConflictDialogWin();
	}
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_CloseSyncCheckWaitDialog(void)
{
	MMK_CloseWin(MMIPB_SYNC_CHECK_WAITING_WIN_ID);		
}
//zouruoqi
PUBLIC void MMIPB_CloseSyncResultDialog(void)
{
     MMK_CloseWin(MMIPUB_ALERT_WIN_ID);
}
//zouruoqi
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
//        
/*****************************************************************************/
PUBLIC void MMIPB_UpdatePBSyncSettingMenu(void)
{
	uint32 i = 0;
	MMI_HANDLE_T applet_handle = NULL;
	MMIPB_APPLET_T* applet_ptr = PNULL;		  
	MMI_HANDLE_T win_handle = NULL;
	MMI_QUEUE_NODE_T * pb_queue = MMIPB_GetAppletQueue();

	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_UpdatePBSyncSettingMenu2");
	if(pb_queue != PNULL)
	{
		for (i=0; i< pb_queue->current_count; i++)
		{
			uint8 index = 0;
			applet_handle =	*(MMI_HANDLE_T *)MMIQUEUE_PeekQueue(pb_queue, i);
			applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);  
			if(PNULL == applet_ptr)
			{
				continue;
			}

			win_handle =MMK_GetWinHandle(applet_handle, MMIPB_SETTINGS_WIN_ID);

			if(MMK_IsOpenWin(win_handle) && MMK_IsFocusWin(win_handle))
			{
			SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_UpdatePBSyncSettingMenu2 sent get_focus");
				SCI_TRACE_LOW("[MMIPBSYNC] sent get_focus");
				MMK_PostMsg(win_handle, MSG_GET_FOCUS, PNULL,0);
			}
		} 
	}

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_UpdatePBSyncSettingMenu2");
}
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleContactsSettingsWinMsg(
				     MMI_WIN_ID_T      win_id, 
 				     MMI_MESSAGE_ID_E  msg_id, 
 				     DPARAM            param
 				     )
{
	MMI_RESULT_E        result  = MMI_RESULT_TRUE;
	MMI_HANDLE_T list_ctr_id = NULL;
	//MMI_STRING_T title_str = {0};
	uint16 cur_index = 0;
	uint32 item_index = 0;
#ifdef MMIPB_SYNC_WITH_O365	
	BOOLEAN is_heart_beat_on = FALSE;
#endif
#ifdef MMIPB_SIMCONTACT_SUPPORT
    MMIPB_MEMORY_E enStatus = MMIPB_GetContactSaveStatus();
#endif

	MMI_HANDLE_T    applet_handle = 0;
	MMIPB_APPLET_T* applet_ptr = PNULL;
	
	applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
	if(PNULL == applet_ptr)
	{
		return result;
	}
	applet_handle = applet_ptr->caf_applet.app_handle;

	
	list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_SETTINGS_LISTBOX_CTRL_ID);
	
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
        SCI_TRACE_LOW("[MMIPBSYNC] HandleContactsSettingsWinMsg MSG_OPEN_WINDOW");
		SetSettingsList(win_id, list_ctr_id);
		MMK_SetActiveCtrl(list_ctr_id, FALSE);
		GUILIST_SetCurItemIndex(list_ctr_id,0);
		if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STXT_HEADER_PHONEBOOK_OPTIONS))
		{
			GUIWIN_UpdateStb();
		}
#if defined (MMIPB_SIMDN_SUPPORT)
		{
			MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
			BOOLEAN       is_support_sdn = FALSE;
			
			for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
			{
				if(MNSIM_GetSdnStatusEx(dual_sys))
				{
					is_support_sdn = TRUE;
					break;
				}
			}

			#ifdef WIN32
			is_support_sdn= TRUE;
			#endif

			if(!is_support_sdn)
			{
				GUILIST_SetItemGray(list_ctr_id, MMIPB_SETTINGS_LIST_INDEX_SDN, TRUE);
				GUILIST_SetItemInvalid(list_ctr_id, MMIPB_SETTINGS_LIST_INDEX_SDN, TRUE);
				
			}
			else
			{
				GUILIST_SetItemGray(list_ctr_id, MMIPB_SETTINGS_LIST_INDEX_SDN, FALSE);
				GUILIST_SetItemInvalid(list_ctr_id, MMIPB_SETTINGS_LIST_INDEX_SDN, FALSE);
				
			}
		}
#endif				
        //MMK_PostMsg(win_id,MSG_FULL_PAINT, PNULL,0);
		break;
	case MSG_GET_FOCUS:
		SCI_TRACE_LOW("[MMIPBSYNC] HandleContactsSettingsWinMsg MSG_GET_FOCUS");
		if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STXT_HEADER_PHONEBOOK_OPTIONS))
		{
			GUIWIN_UpdateStb();
		}
		item_index = 0;
#ifdef MMIPB_SIMCONTACT_SUPPORT
        if (MMIPB_MEMORY_PHONEANDSIM == enStatus)
        {
            AppendListItemForSettings(
                list_ctr_id,
                MMIPB_SETTINGS_LIST_INDEX_MEMORY,
                &item_index,
                GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
                STXT_MEMORY_A_B,
                STR_PBSEL_PHONE_AND_SIM,
                TRUE
            );
        }
        else if (MMIPB_MEMORY_PHONE == enStatus)
        {
            AppendListItemForSettings(
                list_ctr_id,
                MMIPB_SETTINGS_LIST_INDEX_MEMORY,
                &item_index,
                GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
                STXT_MEMORY_A_B,
                STXT_PHONE,
                TRUE
            );
        }
        else if (MMIPB_MEMORY_SIM1 == enStatus)
        {
            AppendListItemForSettings(
                list_ctr_id,
                MMIPB_SETTINGS_LIST_INDEX_MEMORY,
                &item_index,
                GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
                STXT_MEMORY_A_B,
                #ifdef MMI_MULTI_SIM_SYS_SINGLE
                STR_ILIST_SIM_EXT01,   
                #else
                STR_CONTACT_LIST_MEMORY_S1,
                #endif
                TRUE
            );
        }
        else
        {
            AppendListItemForSettings(
                list_ctr_id,
                MMIPB_SETTINGS_LIST_INDEX_MEMORY,
                &item_index,
                GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
                STXT_MEMORY_A_B,
                STR_CONTACT_LIST_MEMORY_S2,
                TRUE
            );
        }
        item_index = MMIPB_SETTINGS_LIST_INDEX_SORT_BY;
#endif //MMIPB_SIMCONTACT_SUPPORT
		AppendListItemForSettings(
			list_ctr_id,
			MMIPB_SETTINGS_LIST_INDEX_SORT_BY,
			&item_index,
			GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
			STR_CONTA_LIST_SORT_EXT01,
			MMIPB_SettingsIsSortBySurname()?STR_CONTA_LIST_LAST_NAME_EXT01:STR_CONTA_LIST_FIRST_NAME_EXT01,
			TRUE
			);
#ifdef MMIPB_SYNC_WITH_O365
        item_index = MMIPB_SETTINGS_LIST_INDEX_CONNECT;
		AppendListItemForSettings(
			list_ctr_id,
			MMIPB_SETTINGS_LIST_INDEX_CONNECT,
			&item_index,
			GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT,
			STR_ONEDRIVE_ILIST_CONNECT_EXT01,
			TXT_NULL,
			TRUE
		);
		item_index = MMIPB_SETTINGS_LIST_INDEX_SYNC;
		AppendListItemForSettings(
			list_ctr_id,
			MMIPB_SETTINGS_LIST_INDEX_SYNC,
			&item_index,
			GUIITEM_STYLE_ONE_LINE_TEXT_MS,
			STR_ONEDRIVE_LIST_SYNC_EXT01,
			TXT_NULL,
			TRUE
		);
#endif
#ifdef MMI_BT_PBAP_SUPPORT
        item_index = MMIPB_SETTINGS_LIST_INDEX_ANDROID_SYNC;
        AppendListItemForSettings(
            list_ctr_id,
            MMIPB_SETTINGS_LIST_INDEX_ANDROID_SYNC,
            &item_index,
            GUIITEM_STYLE_ONE_LINE_TEXT_MS,
            STR_CONTACT_SYNC_VIA_BT,
            TXT_NULL,
            TRUE
            );
#endif
		MMK_PostMsg(win_id,MSG_FULL_PAINT, PNULL,0);
		break;
	case MSG_FULL_PAINT:
		SCI_TRACE_LOW("[MMIPBSYNC] HandleContactsSettingsWinMsg MSG_FULL_PAINT");
#ifdef MMIPB_SYNC_WITH_O365			
        if ((TRUE == MMIPB_SYNC_IsSyncing())|| FALSE == MMIPB_SYNC_IsHttpDisconnected()||
			(MMICONNECTION_SETTING_GPRS_SWITCH_ON != MMIAPICONNECTION_GetGPRSSwitchStatus()))
		{
			/* when syncing, set heart-beat menu to  gray */            
			GUILIST_SetItemGray(list_ctr_id, MMIPB_SETTINGS_LIST_INDEX_CONNECT, TRUE);
			GUILIST_SetItemInvalid(list_ctr_id, MMIPB_SETTINGS_LIST_INDEX_CONNECT, TRUE);
		}
		else
		{
				
			GUILIST_SetItemGray(list_ctr_id, MMIPB_SETTINGS_LIST_INDEX_CONNECT, FALSE);
			GUILIST_SetItemInvalid(list_ctr_id, MMIPB_SETTINGS_LIST_INDEX_CONNECT, FALSE);
		}
		if ((FALSE == MMIPB_SYNC_NV_Is_HeartBeat_On()) ||
			(MMICONNECTION_SETTING_GPRS_SWITCH_ON != MMIAPICONNECTION_GetGPRSSwitchStatus()) ||
			(TRUE == MMIPB_SYNC_IsSyncing())||FALSE == MMIPB_SYNC_IsHttpDisconnected())
		{
			/* when heart-beat is off or syncing, set sync menu to  gray */
			GUILIST_SetItemGray(list_ctr_id, MMIPB_SETTINGS_LIST_INDEX_SYNC, TRUE);
			GUILIST_SetItemInvalid(list_ctr_id, MMIPB_SETTINGS_LIST_INDEX_SYNC, TRUE);
		}
		else
		{
		    	
			GUILIST_SetItemGray(list_ctr_id, MMIPB_SETTINGS_LIST_INDEX_SYNC, FALSE);
			GUILIST_SetItemInvalid(list_ctr_id, MMIPB_SETTINGS_LIST_INDEX_SYNC, FALSE);
		}
#endif		
		break;
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
		cur_index =  GUILIST_GetCurItemIndex(list_ctr_id);
#ifdef MMIPB_SIMCONTACT_SUPPORT
		if (MMIPB_SETTINGS_LIST_INDEX_MEMORY == cur_index)
		{
			MMK_CreateWinByApplet(applet_handle,(uint32*)MMIPB_SETTINGS_MEMORY_WIN_TAB, PNULL);
		}
#endif
		if(MMIPB_SETTINGS_LIST_INDEX_SORT_BY == cur_index)
		{
			MMK_CreateWinByApplet(applet_handle,(uint32*)MMIPB_SETTINGS_SORT_BY_WIN_TAB, PNULL);
		}
#if defined (MMIPB_SIMDN_SUPPORT)
		else if(MMIPB_SETTINGS_LIST_INDEX_SDN == cur_index)
		{
		#ifndef MMI_MULTI_SIM_SYS_SINGLE
			MMK_CreateWinByApplet(applet_handle,(uint32*)MMIPB_SETTINGS_SDN_SIM_SELECT_WIN_TAB, PNULL);
		#else
			MMIPB_OpenSDNWin(applet_handle);
		#endif
		}
#endif
#ifdef MMI_BT_PBAP_SUPPORT
        else if (MMIPB_SETTINGS_LIST_INDEX_ANDROID_SYNC == cur_index)
        {
#ifdef FM_SUPPORT
        if(MMIAPIFM_IsFmActive()) 
        {
            MMIPUB_OpenAlertWarningWin(TXT_EXIT_BACKGROUND_FM);
        }
        else
        {
            HandleContactSyncPBAPAction(win_id);
        }
#else
            HandleContactSyncPBAPAction(win_id);
#endif
        }
#endif
#ifdef MMIPB_SYNC_WITH_O365
		else if (cur_index == MMIPB_SETTINGS_LIST_INDEX_CONNECT)
		{
			if (FALSE == MMIPB_SYNC_IsSyncing())
			{
				CTRLLIST_GetToggleState(list_ctr_id, MMIPB_SETTINGS_LIST_INDEX_CONNECT, &is_heart_beat_on);
				if (is_heart_beat_on)
				{
					MMIPB_DoSyncHeartBeartOn();
					
					GUILIST_SetItemGray(list_ctr_id, MMIPB_SETTINGS_LIST_INDEX_SYNC, FALSE);
					GUILIST_SetItemInvalid(list_ctr_id, MMIPB_SETTINGS_LIST_INDEX_SYNC, FALSE);
				}			
				else
				{
					MMIPB_SYNC_NV_Set_HeartBeat(FALSE);
					
					/* when heart-beat is off or syncing, set sync menu to  gray */
					GUILIST_SetItemGray(list_ctr_id, MMIPB_SETTINGS_LIST_INDEX_SYNC, TRUE);
					GUILIST_SetItemInvalid(list_ctr_id, MMIPB_SETTINGS_LIST_INDEX_SYNC, TRUE);
                    MMK_PostMsg(win_id,MSG_FULL_PAINT, PNULL,0);
				}
				
			}
		}
		else if (MMIPB_SETTINGS_LIST_INDEX_SYNC == cur_index)
		{
			MMIPB_OpenSyncCheckWaitDialogWin(applet_handle);
		}
#endif
		break;
	case MSG_CLOSE_WINDOW:
		break;
	default:
		result = FALSE;
		break;
	}
	
	return (result);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSortWaitDialogWinMsg
(
 MMI_WIN_ID_T     win_id, 
 MMI_MESSAGE_ID_E msg_id, 
 DPARAM           param 
 )
{
	MMI_RESULT_E        recode = MMI_RESULT_TRUE;
	static uint8        s_wait_ready_timer_id = 0;
	MMI_HANDLE_T    applet_handle = {0};
	MMIPB_APPLET_T* applet_ptr = PNULL;
	
	applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
	if(PNULL == applet_ptr)
	{
		return recode;
	}
	applet_handle = applet_ptr->caf_applet.app_handle;
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		if(!MMIAPIPB_IsPbReady())
		{
			if (s_wait_ready_timer_id != 0)
			{
				MMK_StopTimer(s_wait_ready_timer_id);
				s_wait_ready_timer_id = 0;
			}
			s_wait_ready_timer_id = MMK_CreateWinTimer(win_id, 500,TRUE);
			s_wait_ready_timer_id = MMK_CreateWinTimer(win_id, 500,TRUE);
		}
		else
		{
			if(0 != s_wait_ready_timer_id)
			{
				MMK_StopTimer(s_wait_ready_timer_id); 
				s_wait_ready_timer_id = 0;
			}
			MMK_CloseWin(win_id);
		}
		break;
	case MSG_TIMER:  
		if(MMK_IsFocusWin(win_id))
		{
			//current not other app occour
			if (s_wait_ready_timer_id == *((uint8 *) param))
			{              
				if (MMIPB_IsPbReady())
				{
					MMK_CloseWin(win_id);
				}
			}
		}
		
		break;
	case MSG_APP_OK:
	case MSG_CTL_OK:
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		MMK_CloseAllWinByApplet(applet_handle);
		break;
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
		//MMK_CloseWin(win_id);
		break;
	case MSG_CLOSE_WINDOW:
		if(0 != s_wait_ready_timer_id)
		{
			MMK_StopTimer(s_wait_ready_timer_id); 
			s_wait_ready_timer_id = 0;
		}
		break;
	default:
		recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
		break;
	}
	return (recode);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_OpenSortWaitDialogWin(MMI_HANDLE_T applet_handle)
{
	MMI_WIN_ID_T alert_id = MMIPB_SORT_WAITING_WIN_ID; 
	MMIPUB_OpenConfirmationDialogWinByTextIdEx(
		applet_handle,
		PNULL,
		STR_NOTE_WAITING,
		TXT_NULL,
		IMAGE_NULL,
		&alert_id,
		PNULL,
		MMIPUB_SOFTKEY_DIALOG_CLOSE,
		HandleSortWaitDialogWinMsg,
		PNULL
		);				
}
#ifdef MMIPB_SIMCONTACT_SUPPORT
/*****************************************************************************/
//  Description                         : Get contact memory status
//  Global resource dependence          : none 
//  Author                              : zhongping.shi
//  Input                               : MMI_WIN_ID_T win_id,
//                                        MMI_CTRL_ID_T ctrl_id,
//  Output                              : none
//  Return                              : MMI_RESULT_E result
//  Caution                             : none
/*****************************************************************************/
LOCAL void mmipb_SetSettingsMemoryList(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{     
	uint32 item_index = 0;
	
	GUILIST_SetMaxItem(
		ctrl_id, 
		MMIPB_SETTINGS_MEMORY_INDEX_MAX,
		FALSE
		);
	if ((MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)) || (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2)))
    {        
        AppendListItemForSettings(
            ctrl_id,
            MMIPB_SETTINGS_MEMORY_INDEX_PHONEANDSIM,
            &item_index,
            GUIITEM_STYLE_ONE_LINE_TEXT_MS,
            STR_PBSEL_PHONE_AND_SIM,
            TXT_NULL,
            FALSE
            );
    }
        
    AppendListItemForSettings(
        ctrl_id,
        MMIPB_SETTINGS_MEMORY_INDEX_PHONE,
        &item_index,
        GUIITEM_STYLE_ONE_LINE_TEXT_MS,
        STXT_PHONE,
        TXT_NULL,
        FALSE
        );
    
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
    {        
        AppendListItemForSettings(
            ctrl_id,
            MMIPB_SETTINGS_MEMORY_INDEX_SIM1,
            &item_index,
            GUIITEM_STYLE_ONE_LINE_TEXT_MS,
            #ifdef MMI_MULTI_SIM_SYS_SINGLE
            STR_ILIST_SIM_EXT01,	
            #else
            STR_CONTACT_LIST_MEMORY_S1,
            #endif
            TXT_NULL,
            FALSE
            );
    }
    
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {        
        AppendListItemForSettings(
            ctrl_id,
            MMIPB_SETTINGS_MEMORY_INDEX_SIM2,
            &item_index,
            GUIITEM_STYLE_ONE_LINE_TEXT_MS,
            STR_CONTACT_LIST_MEMORY_S2,
            TXT_NULL,
            FALSE
            );
    }
		
	return;
}
/*****************************************************************************/
//  Description                         : Get display index by memory status
//  Global resource dependence          : none 
//  Author                              : zhongping.shi
//  Input                               : MMIPB_MEMORY_E enStatus
//  Output                              : none
//  Return                              : uint16 uiIndex
//  Caution                             : none
/*****************************************************************************/
LOCAL uint16 mmipb_GetCurIndexByStatus(MMIPB_MEMORY_E enStatus)
{
	uint16 uiIndex = 0;
	
    if (MMIPB_MEMORY_PHONEANDSIM == enStatus)
    {
        uiIndex = 0;
    }
    else if (MMIPB_MEMORY_PHONE == enStatus)
    {
        if ((MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)) || (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2)))
        {
            uiIndex = 1;
        }
        else
        {
            uiIndex = 0;
        }
    }
    else if (MMIPB_MEMORY_SIM1 == enStatus)
    {
        uiIndex = 2;
    }
    else if (MMIPB_MEMORY_SIM2 == enStatus)
    {
        if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
        {
            uiIndex = 3;
        }
        else
        {
            uiIndex = 2;
        }
    }
    else
    {
        SCI_TRACE_LOW("[MMIPB-memory] Error memory status: %d", enStatus);
    }

    return uiIndex;
}

/*****************************************************************************/
//  Description                         : Get memory status by display index
//  Global resource dependence          : none 
//  Author                              : zhongping.shi
//  Input                               : uint16 uiIndex
//  Output                              : none
//  Return                              : MMIPB_MEMORY_E enStatus
//  Caution                             : none
/*****************************************************************************/
LOCAL MMIPB_MEMORY_E mmipb_GetStatusByCurIndex(uint16 uiIndex)
{
    MMIPB_MEMORY_E enStatus = MMIPB_MEMORY_INIT;
    
    if (0 == uiIndex)
    {
        if ((MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)) || (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2)))
        {
            enStatus = MMIPB_MEMORY_PHONEANDSIM;
        }
        else
        {
            enStatus = MMIPB_MEMORY_PHONE;
        }
    }
    else if (1 == uiIndex)
    {
        if ((MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)) || (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2)))
        {
            enStatus = MMIPB_MEMORY_PHONE;
        }
    }
    else if (2 == uiIndex)
    {
        if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
        {
            enStatus = MMIPB_MEMORY_SIM1;
        }
        else
        {
            enStatus = MMIPB_MEMORY_SIM2;
        }
    }
    else if (3 == uiIndex)
    {
        enStatus = MMIPB_MEMORY_SIM2;
    }
    else
    {
        SCI_TRACE_LOW("[MMIPB-memory] Error display index: %d", uiIndex);
    }

    return enStatus;
}

/*****************************************************************************/
//  Description                         : Get contact memory status
//  Global resource dependence          : none 
//  Author                              : zhongping.shi
//  Input                               : MMI_WIN_ID_T win_id,
//                                        MMI_MESSAGE_ID_E  msg_id,
//                                        DPARAM            param
//  Output                              : none
//  Return                              : MMI_RESULT_E result
//  Caution                             : none
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleContactsSettingsMemoryByWinMsg(
				     MMI_WIN_ID_T      win_id, 
 				     MMI_MESSAGE_ID_E  msg_id, 
 				     DPARAM            param
 				     )
{
	MMI_RESULT_E       result  = MMI_RESULT_TRUE;
	uint16             uiIndex = 0;
	MMI_HANDLE_T   list_ctr_id = NULL;
	MMIPB_APPLET_T* applet_ptr = PNULL;
	MMI_HANDLE_T applet_handle = {0};
	MMIPB_MEMORY_E    enStatus = MMIPB_GetContactSaveStatus();
	
	applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
	if(PNULL == applet_ptr)
	{
		return result;
	}
	applet_handle = applet_ptr->caf_applet.app_handle;
	
	list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_SETTINGS_MEMORY_LISTBOX_CTRL_ID);
	
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		mmipb_SetSettingsMemoryList(win_id, list_ctr_id);
		MMK_SetActiveCtrl(list_ctr_id, FALSE);
        GUILIST_SetCurItemIndex(list_ctr_id, mmipb_GetCurIndexByStatus(enStatus)); 
		
		if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STXT_MEMORY_A_B))
		{
			GUIWIN_UpdateStb();
		}
		break;
	case MSG_GET_FOCUS:
		if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STXT_MEMORY_A_B))
		{
			GUIWIN_UpdateStb();
		}
		MMK_CloseWin(win_id);
		break;
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
		uiIndex =  GUILIST_GetCurItemIndex(list_ctr_id);
		MMIPB_SetContactSaveStatus(mmipb_GetStatusByCurIndex(uiIndex));
		
        MMIPB_SetOpcType(applet_ptr->instance.handle, 0);
        MMIPB_OpenWaitReadyWin(applet_handle, 1);
        
		MMK_CloseWin(win_id);
		break;
	case MSG_CLOSE_WINDOW:
		break;
	default:
		result = FALSE;
		break;
	}
	
	return (result);
}
#endif //#ifdef MMIPB_SIMCONTACT_SUPPORT

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL void SetSettingsSortByList(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id)
{     
	uint32 item_index = 0;
	
	GUILIST_SetMaxItem(
		ctrl_id, 
		MMIPB_SETTINGS_SORT_BY_INDEX_MAX,
		FALSE
		);
	AppendListItemForSettings(
		ctrl_id,
		MMIPB_SETTINGS_SORT_BY_INDEX_FIRST_NAME,
		&item_index,
		GUIITEM_STYLE_ONE_LINE_TEXT_MS,
		STR_CONTA_LIST_FIRST_NAME_EXT01,
		TXT_NULL,
		FALSE
		);
	AppendListItemForSettings(
		ctrl_id,
		MMIPB_SETTINGS_SORT_BY_INDEX_SUR_NAME,
		&item_index,
		GUIITEM_STYLE_ONE_LINE_TEXT_MS,
		STR_CONTA_LIST_LAST_NAME_EXT01,
		TXT_NULL,
		FALSE
		);
	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleContactsSettingsSortByWinMsg(
				     MMI_WIN_ID_T      win_id, 
 				     MMI_MESSAGE_ID_E  msg_id, 
 				     DPARAM            param
 				     )
{
	MMI_RESULT_E        result  = MMI_RESULT_TRUE;
	MMI_HANDLE_T list_ctr_id = NULL;
	//MMI_STRING_T title_str = {0};
	uint16 cur_index = 0;
	MMI_HANDLE_T    applet_handle = {0};
	MMIPB_APPLET_T* applet_ptr = PNULL;
	
	applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
	if(PNULL == applet_ptr)
	{
		return result;
	}
	applet_handle = applet_ptr->caf_applet.app_handle;
	
	list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_SETTINGS_SORTBY_LISTBOX_CTRL_ID);
	
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		SetSettingsSortByList(win_id, list_ctr_id);
		MMK_SetActiveCtrl(list_ctr_id, FALSE);
		if(MMIPB_SettingsIsSortBySurname())
		{
			GUILIST_SetCurItemIndex(list_ctr_id,1);
		}
		else
		{
			GUILIST_SetCurItemIndex(list_ctr_id,0);
		}
		if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CONTA_LIST_SORT_EXT01))
		{
			GUIWIN_UpdateStb();
		}
		break;
	case MSG_GET_FOCUS:
		if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CONTA_LIST_SORT_EXT01))
		{
			GUIWIN_UpdateStb();
		}
		break;
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
		#ifdef MMIPB_SYNC_WITH_O365        
                if (MMIPB_SYNC_IsSyncing())
                {
                    MMIPB_OpenSyncConflictPromptWin(applet_handle);
                    break;
                }
		#endif
		cur_index =  GUILIST_GetCurItemIndex(list_ctr_id);
		if(MMIPB_SettingsIsSortBySurname() && MMIPB_SETTINGS_SORT_BY_INDEX_FIRST_NAME == cur_index)
		{
			MMIPB_SettingWriteSortBySurname(FALSE);
			MMIPB_SetOpcType(applet_ptr->instance.handle, 0);
			MMIPB_BeginToReSort();
			MMIPB_OpenWaitReadyWin(applet_handle, 1);
		}
		else if(!MMIPB_SettingsIsSortBySurname() && MMIPB_SETTINGS_SORT_BY_INDEX_SUR_NAME == cur_index)
		{
			MMIPB_SettingWriteSortBySurname(TRUE);
			MMIPB_SetOpcType(applet_ptr->instance.handle, 0);
			MMIPB_BeginToReSort();
			MMIPB_OpenWaitReadyWin(applet_handle, 1);
		}
		MMK_CloseWin(win_id);
		break;
	case MSG_CLOSE_WINDOW:
		break;
	default:
		result = FALSE;
		break;
	}
	
	return (result);
}

#if defined (MMIPB_SIMDN_SUPPORT)
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL void SetSettingsSdnSimSelectList(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id)
{     
	uint32 item_index = 0;
	BOOLEAN       is_support_sdn = FALSE;
	
	GUILIST_SetMaxItem(
		ctrl_id, 
		2,
		FALSE
		);
	AppendListItemForSettings(
		ctrl_id,
		0,
		&item_index,
		GUIITEM_STYLE_ONE_LINE_TEXT_MS,
		STR_ILIST_SIM_ONE,
		TXT_NULL,
		FALSE
		);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
	AppendListItemForSettings(
		ctrl_id,
		1,
		&item_index,
		GUIITEM_STYLE_ONE_LINE_TEXT_MS,
		STR_ILIST_SIM_TWO,
		TXT_NULL,
		FALSE
		);
#endif
	
	if(MNSIM_GetSdnStatusEx(MN_DUAL_SYS_1))
	{
		is_support_sdn = TRUE;
	}
#ifdef WIN32
	is_support_sdn= TRUE;
#endif
	
	if(!is_support_sdn)
	{
		GUILIST_SetItemGray(ctrl_id, 0, TRUE);
		GUILIST_SetItemInvalid(ctrl_id, 0, TRUE);
	}
	else
	{
		GUILIST_SetItemGray(ctrl_id, 0, FALSE);
		GUILIST_SetItemInvalid(ctrl_id, 0, FALSE);
	}
	is_support_sdn = FALSE;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
	if(MNSIM_GetSdnStatusEx(MN_DUAL_SYS_2))
	{
		is_support_sdn = TRUE;
	}
#ifdef WIN32
	is_support_sdn= TRUE;
#endif
	
	if(!is_support_sdn)
	{
		GUILIST_SetItemGray(ctrl_id, 1, TRUE);
		GUILIST_SetItemInvalid(ctrl_id, 1, TRUE);
	}
	else
	{
		GUILIST_SetItemGray(ctrl_id, 1, FALSE);
		GUILIST_SetItemInvalid(ctrl_id, 1, FALSE);
	}
#endif
	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleContactsSettingsSdnSimSelectWinMsg(
				     MMI_WIN_ID_T      win_id, 
 				     MMI_MESSAGE_ID_E  msg_id, 
 				     DPARAM            param
 				     )
{
	MMI_RESULT_E        result  = MMI_RESULT_TRUE;
	MMI_HANDLE_T list_ctr_id = NULL;
	//MMI_STRING_T title_str = {0};
	uint16 cur_index = 0;
	MMI_HANDLE_T    applet_handle = {0};
	MMIPB_APPLET_T* applet_ptr = PNULL;
	
	applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
	if(PNULL == applet_ptr)
	{
		return result;
	}
	applet_handle = applet_ptr->caf_applet.app_handle;
	
	list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_SETTINGS_SDN_SIM_SELECT_LISTBOX_CTRL_ID);
	
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		SetSettingsSdnSimSelectList(win_id, list_ctr_id);		
		MMK_SetActiveCtrl(list_ctr_id, FALSE);
		if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_SIM_HDR_SELECTION_EXT01))
		{
			GUIWIN_UpdateStb();
		}
		break;
	case MSG_GET_FOCUS:
		if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_SIM_HDR_SELECTION_EXT01))
		{
			GUIWIN_UpdateStb();
		}
		break;
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
		cur_index =  GUILIST_GetCurItemIndex(list_ctr_id);
		if(0 == cur_index)
		{
			MMIPB_OpenSDNWinWithSim(applet_handle,MN_DUAL_SYS_1);
		}
		#ifndef MMI_MULTI_SIM_SYS_SINGLE
		else if( 1 == cur_index)
		{
			MMIPB_OpenSDNWinWithSim(applet_handle,MN_DUAL_SYS_2);
		}
		#endif
		MMK_CloseWin(win_id);
		break;
	case MSG_CLOSE_WINDOW:
		break;
	default:
		result = FALSE;
		break;
	}
	
	return (result);
}
#endif

/*****************************************************************************/
//  Description : 进入设置窗口
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenContactsSettingsWin(MMI_HANDLE_T applet_handle)
{
    MMK_CreateWinByApplet(applet_handle, (uint32*)MMIPB_SETTINGS_WIN_TAB, PNULL);
}

/*Edit by script, ignore 6 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
