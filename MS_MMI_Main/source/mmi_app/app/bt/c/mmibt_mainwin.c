/****************************************************************************
** File Name:     mmibt_mainwin.c                                        *
** Author:         yuantao.xu                                            *
** Date:           8/3/2012                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 8/3/2012    yuantao.xu     Create                                  *
**                                                                         *
****************************************************************************/
#define _MMIBT_MAINWIN_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#include "mmi_app_bt_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_timer.h"
#include "mmibt_func.h"
#include "mmibt_mainwin.h"
#include "mmibt_app.h"
#include "mmipub.h"
#include "guilabel.h"
#include "mmifmm_export.h"
#include "bt_abs.h"
#include "bt_cfg.h"
#include "mmibt_text.h"
#include "mmi_default.h"
#include "mmiset_export.h"
#include "guitext.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmivcard_export.h"
#include "mmieng_export.h"
#include "mmicl_export.h"
#include "mmiphone_export.h"
#include "mmibt_a2dp.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
#include "mmibt_nv.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmipicview_export.h"
#include "mmialarm_export.h"
#include "mmi_mainmenu_export.h"
#include "mmifmm_text.h"
#include "guimenu.h"
#include "guires.h"
#include "mmidc_export.h"
#include "guiedit.h"
#include "mmiidle_export.h"
#include "mmimtv_export.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "mmi_autotest.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "mmieng_id.h"
#include "guictrl_api.h"
#include "mmieng_text.h"
#include "mmienvset_export.h"
#include "mime_type.h"
#include "mmisms_export.h"
#ifdef DRM_SUPPORT
#include "mmidrm.h"
#endif
#include "mmikl_export.h"
#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#include "mmiidle_statusbar.h"
#include "mmimultim_image.h"
#include "mmisrvaud_api.h"
#include "nvitem.h"
#include "variant_config_nv.h"

#ifdef MMI_BT_PBAP_SUPPORT
#include "mmipb_export.h"
#include "mmipb_app.h"
#include "mmipb_common.h"
#include "mmipb_text.h"
#include "mmifilearray_export.h"
//#include "mmipb_task.h"
#endif

#ifdef BLUETOOTH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#ifdef MMI_BT_PBAP_SUPPORT 
#define BT_PBAP_TIMER_OUT            (30*1000)       //同步联系人请求响应过时保护时长
LOCAL uint8 s_bt_connect_timeout_timer = 0;
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                */
/*---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: Handle blue tooth main menu win msg
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleMainMenuWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          );
/*****************************************************************************/
//  Description :HandleHideDeviceWindow
//  Global resource dependence :
//  Author:lifei.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleHideDeviceWin(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         );

#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Discription: create list menu by current operation
//  Global resource dependence: None
//  Author:
//  Note :
//  Return:
/*****************************************************************************/
LOCAL void MMIBT_CreateMainMenuList(
                    MMI_WIN_ID_T             win_id, //IN: list menu's window id
                    MMI_CTRL_ID_T            ctrl_id, //IN: list menu's id
                    MMI_TEXT_ID_T            text_id //IN: list menu titles's text id
                               );
#endif

PUBLIC uint32  MMIBT_GetDeviceHideNDT( void );
PUBLIC NDT_BT_CONFIG_T * MMIBT_GetNdtDeviceHideOrNot(void);
LOCAL void HideDeviceAfterTimer(void);
LOCAL void StopHideDeviceAfterTimer(void);

#ifdef MMI_BT_PBAP_SUPPORT
/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIBT_HandlePubwinConnectWaitingMsg(
                                                       MMI_WIN_ID_T         win_id, 
                                                       MMI_MESSAGE_ID_E    msg_id, 
                                                       DPARAM              param
                                                       );

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinSyncWaitingMsg(
                                              MMI_WIN_ID_T         win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              );

/*****************************************************************************/
//  Description : open query win
//  Global resource dependence : 
//  Author: yanyan.an
//  Note: 
/*****************************************************************************/
LOCAL void OpenQueryWin(MMI_TEXT_ID_T text_id, MMIPUB_HANDLE_FUNC func);

/*****************************************************************************/
//  Description : MMIBT_CreateTimer
//  Global resource dependence : 
//  Author: yanyan.an
//  Note: 创建超时定时器
/*****************************************************************************/
LOCAL void MMIBT_CreateTimer(void);

/******************************************************************************/
// Description: MMIBT_ProcessTimerOver
// Global resource dependence: 
// Author: yanyan.an
// Note: 超时保护机制
/******************************************************************************/
LOCAL void MMIBT_ProcessTimerOver(uint8 timer_id, uint32 param);

#endif

/*******************************************************************************/
//window table define
WINDOW_TABLE(MMIBT_MAIN_MENU_WIN_TAB) =
{
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
    WIN_FUNC((uint32) HandleMainMenuWinMsg ),
    WIN_ID(MMIBT_MAIN_MENU_WIN_ID),
    WIN_TITLE(TXT_BLUETOOTH),
#ifndef MMI_GUI_STYLE_TYPICAL
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIBT_MAIN_LIST_CTRL_ID),
#endif
//#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //WIN_SOFTKEY(NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
//#endif
    END_WIN

};

//lulifei add the hide device
WINDOW_TABLE( MMIBT_HIDE_DEVICE_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleHideDeviceWin ),
    WIN_ID( MMIBT_HIDE_DEVICE_WIN_ID ),
    ///WIN_TITLE(TXT_SET_SYSTEM),
    WIN_SOFTKEY(NULL, STXT_SOFTKEY_SELECT_MK, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIBT_HIDE_DEVICE_CTRL_ID),
    END_WIN
};
//device hide
enum{
	ID_SET_LIST_HIDE_DEVICE_HIDDEN,
	ID_SET_LIST_HIDE_DEVICE_VISIBLE,
	ID_SET_LIST_HIDE_DEVICE_TEMPORARY_AVAILABLE
};
//end
NDT_BT_CONFIG_T bt_config = {0};
LOCAL uint8         s_temproary_avilable_timer=0;
#define TEMPROARY_AVILABLE_TIMER_2MIN   (2*60*1000)
BOOLEAN startDeviceHideTimer = FALSE;

/*****************************************************************************/
//  Discription: Get the show string of lock status
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleMainMenuWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          )
{
#ifdef MMI_GUI_STYLE_TYPICAL
    MMI_MENU_ID_T               menu_id = 0;
#endif
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
#ifndef MMI_GUI_STYLE_TYPICAL
    uint16                     cur_index = 0;
#endif

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
        GUILIST_SetMaxItem(MMIBT_MAIN_LIST_CTRL_ID,MMIBT_MAIN_MEUN_NUM, FALSE);
        MMIBT_UpdateMainMenu(FALSE);
        MMK_SetAtvCtrl(win_id, MMIBT_MAIN_LIST_CTRL_ID);
#else
        MMK_SetAtvCtrl(win_id, MMIBT_MAIN_MENU_CTRL_ID);
#endif
        //Register Event
        break;
#ifndef MMI_GUI_STYLE_TYPICAL
     case MSG_CTL_LIST_CHECK_CONTENT:
         cur_index =  GUILIST_GetCurItemIndex(MMIBT_MAIN_LIST_CTRL_ID);
         switch(cur_index)
         {
         case BT_INDEX_OPEN:
         case BT_INDEX_VISIBLE:
         case BT_INDEX_HELP:
             MMK_SendMsg(win_id, MSG_APP_OK,PNULL);
             break;
             
         default:
             break;
         }
         break;
#endif
     case MSG_FULL_PAINT:
         {//reset status bar
             MMI_STRING_T    temp_str = {0};
             MMI_GetLabelTextByLang(STR_BT_HDR_BLUETOOTH_EXT01, &temp_str);
             if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len))
             {
                 GUIWIN_UpdateStb();
             }
         }
         break;
             
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifndef MMI_GUI_STYLE_TYPICAL
        cur_index =  GUILIST_GetCurItemIndex(MMIBT_MAIN_LIST_CTRL_ID);
        switch(cur_index)
        {
        case BT_INDEX_OPEN:
            if (BT_GetState())
            {
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_POWER_ONOFF);
                MMIBT_OpenBluetooth(FALSE, TRUE);
            }
            else
            {
                MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_POWER_ONOFF);
                MMIBT_OpenBluetooth(TRUE, TRUE);
            }
            break;
        case BT_INDEX_VISIBLE:
            /*if(MMIBT_GetIsDeviceVisible())
            {
            DoOperation(MMIBT_OPC_HIDE,FALSE);
            }
            else
            {
            DoOperation(MMIBT_OPC_VISIBLE,FALSE);
            }*/
            MMK_CreateWin((uint32 *)MMIBT_HIDE_DEVICE_WIN_TAB, PNULL);
            break;
        case BT_INDEX_PAIRES:
            {
                //MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_LIST_PAIRED);
                //MMIBT_EnterDeviceListWin(PNULL,MMIBT_GetOpertor().root_opertion,BT_SERVICE_ALL);
                MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_SEARCH);
                MMIBT_EnterInquireDeviceListWin(PNULL,MMIBT_GetOpertor().root_opertion,BT_SERVICE_ALL);
                if(BT_GetState())
                {
                    //DoOperation(MMIBT_OPC_SEARCH, FALSE);
                }
            }
            break;
#ifndef PDA_UI_DROPDOWN_WIN
		case BT_INDEX_FILETRANS:
            {
         		MMIBT_OpenTransferWin();
            }
            break;
#endif
#ifdef MMIBT_FTS_SUPPORT
        case BT_INDEX_SHARED:
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SHARED_FILE_SETTING);
            DoOperation(MMIBT_OPC_SHARED_FILE_SETTING, FALSE);
            break;
#endif
        case BT_INDEX_NAME:
            MMIBT_EnterEditWin(PNULL,TRUE);
            break;

        case BT_INDEX_LOCATION:
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SAVE_LOCATION);
            MMIBT_EnterStorageSelectWin();
            break;
        case BT_INDEX_HELP:
            MMIBT_EnterTextBoxWin(PNULL,MMIBT_OPC_HELP);
            break;
        default:
            break;
    }

#else
        menu_id = GUIMENU_GetCurNodeId(MMIBT_MAIN_MENU_CTRL_ID);
        switch (menu_id)
        {
        case ID_SET_BT_ON:
            //Open BT
            {
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_POWER_ONOFF);
                MMIBT_OpenBluetooth(TRUE, TRUE);
            }
            break;
        case ID_SET_BT_OFF:
            //Close BT
            {
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_POWER_ONOFF);
                MMIBT_OpenBluetooth(FALSE, TRUE);
            }
            break;
        case ID_MY_BT_VISIBILITY:
            //"BT Visibility"
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_VISIBILITY);
            if (BT_GetState())
            {
                MMIBT_EnterSetVisibilityWin();
            }
            else
            {
                MMI_STRING_T display_str = {0};
                MMI_GetLabelTextByLang(STR_BT_TURN_ON, &display_str);
                MMIBT_OpenQueryWin(MMI_BT_QUERY_WIN_ID,&display_str, PNULL, MMIBT_OPC_VISIBILITY);
            }
            break;
        case ID_BT_DEVICE_LIST:
            //"Paired Device"
            {
                //set opeartion info
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_LIST_PAIRED);
                MMIBT_EnterDeviceListWin(PNULL,MMIBT_GetOpertor().root_opertion,BT_SERVICE_ALL);
            }
            break;
        case ID_BT_SHARED_FILE_SETTING:
            //"Set Shared File"
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SHARED_FILE_SETTING);
            if (BT_GetState())
            {
                DoOperation(MMIBT_OPC_SHARED_FILE_SETTING, FALSE);
            }
            else
            {
                MMI_STRING_T display_str = {0};

                MMI_GetLabelTextByLang(STR_BT_TURN_ON, &display_str);
                MMIBT_OpenQueryWin(MMI_BT_QUERY_WIN_ID, &display_str, PNULL, MMIBT_OPC_SHARED_FILE_SETTING);
            }
            break;
        case ID_MY_BT_DEVICE_NAME:
            //"Device Name"
            break;
        case ID_BT_SAVE_FILE_LOCATION:
            //"Save File Location"
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SAVE_LOCATION);
            MMIBT_EnterStorageSelectWin();
            break;
        case ID_BT_HELP://device list
            //"Help"
            MMIBT_EnterTextBoxWin(PNULL,MMIBT_OPC_HELP);
            break;
        default:
            break;
        }
#endif
        break;

        case MSG_GET_FOCUS:
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            //UnRegister event
            MMK_DestroyDynaCtrl(MMIBT_MAIN_MENU_CTRL_ID);
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}

/*****************************************************************************/
//  Description :HandleHideDeviceWindow
//  Global resource dependence :
//  Author:lifei.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleHideDeviceWin(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
#if defined (MOTION_SENSOR_TYPE)&& !defined(FLASH_SUPPORT)
    BOOLEAN                   is_sensor_mute    =  FALSE;
    BOOLEAN          is_sensor_change_wallpaper =  FALSE;
    uint32                   sensor_data        =   0;
#endif
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    MMI_TEXT_ID_T       str_id[] = {STR_LIST_HIDDEN_MK,STR_LIST_ALWAYS_VISIBLE_MK,STR_LIST_TEMP_VISIBLE_MK};

    int16               i = 0;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_T      item4_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    GUILIST_ITEM_DATA_T item4_data = {0};
    uint16  max_items = 0;
    uint16   cur_item = 0;
    MMI_CTRL_ID_T               ctrl_id = MMIBT_HIDE_DEVICE_CTRL_ID;
    MMIBT_HIDE_DEVICE_TYPE_E     hide_mode      =   MMIBT_HIDE_DEVICE_HIDDEN;
    MN_RETURN_RESULT_E      return_value    =   MN_RETURN_FAILURE;
    MMI_STRING_T             str_info={0};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        max_items = 3;
        cur_item  = 0;
        //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d"
        GUILIST_SetMaxItem(ctrl_id,max_items, FALSE );
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
        item_t.item_style    = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
        item_t.item_data_ptr = &item_data;
        
        item_data.softkey_id[0] = TXT_COMMON_OK;
        item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
        item_data.softkey_id[2] = STXT_RETURN;
        GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_RETURN,RIGHT_BUTTON,TRUE);
        for(i = 0; i < max_items; i++)
        {
            //if(i==4)//the CONFSERVICE is tongle style
            //	continue;
            item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id = str_id[i];
            if(!GUILIST_InsertItem(ctrl_id, &item_t, i))
            {
                break;
            }
        }
        //GUIWIN_SetStbItemText(MMICOM_WIN_TEXT_INFO,L"System");
        MMINV_READ(MMINV_BT_HIDE_DEVICE,&cur_item,return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            cur_item = MMIBT_HIDE_DEVICE_HIDDEN;
            MMINV_WRITE(MMINV_BT_HIDE_DEVICE,&cur_item);
        }
        GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, cur_item);
        break;
    case MSG_FULL_PAINT:
        {//reset status bar
            MMI_STRING_T    temp_str = {0};
            MMI_GetLabelTextByLang(STR_BT_HDR_DETECTABILITY, &temp_str);
            if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len))
            {
                GUIWIN_UpdateStb();
            }
        }
		break;
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_item = GUILIST_GetCurItemIndex(ctrl_id);
        SCI_TRACE_LOW("HandleHideDeviceWin-cur_item=%d", cur_item);
        switch (cur_item)
        {
        case ID_SET_LIST_HIDE_DEVICE_HIDDEN:
            hide_mode = MMIBT_HIDE_DEVICE_HIDDEN;
            MMINV_WRITE(MMINV_BT_HIDE_DEVICE, &hide_mode);
            MMI_GetLabelTextByLang(STR_LIST_HIDDEN_MK, &str_info);
#ifndef MMI_GUI_STYLE_TYPICAL
            MMIBT_AppendTwoLineTextIconSmallTextListItem(MMIBT_MAIN_LIST_CTRL_ID,STR_BT_SETT_PHONE_DETECTABILITY,IMAGE_COMMON_RIGHT_ARROW,str_info,BT_INDEX_VISIBLE,TRUE);
#endif
            DoOperation(MMIBT_OPC_HIDE,FALSE);
            MMK_CloseWin(win_id);
            break;
        case ID_SET_LIST_HIDE_DEVICE_VISIBLE:
            hide_mode = MMIBT_HIDE_DEVICE_VISIBLE;
            MMINV_WRITE(MMINV_BT_HIDE_DEVICE, &hide_mode);
            MMI_GetLabelTextByLang(STR_LIST_ALWAYS_VISIBLE_MK, &str_info);
#ifndef MMI_GUI_STYLE_TYPICAL
            MMIBT_AppendTwoLineTextIconSmallTextListItem(MMIBT_MAIN_LIST_CTRL_ID,STR_BT_SETT_PHONE_DETECTABILITY,IMAGE_COMMON_RIGHT_ARROW,str_info,BT_INDEX_VISIBLE,TRUE);
#endif
            DoOperation(MMIBT_OPC_VISIBLE,FALSE);
            MMK_CloseWin(win_id);
            break;
        case ID_SET_LIST_HIDE_DEVICE_TEMPORARY_AVAILABLE:
            hide_mode = MMIBT_HIDE_DEVICE_TEMPORARY_AVAILABLE;
            MMINV_WRITE(MMINV_BT_HIDE_DEVICE, &hide_mode);
            DoOperation(MMIBT_OPC_VISIBLE,FALSE);
            if( s_temproary_avilable_timer != 0 )
            {
                MMK_StopTimer(s_temproary_avilable_timer);
                s_temproary_avilable_timer = 0;
            }
            s_temproary_avilable_timer = MMK_CreateTimerCallback(TEMPROARY_AVILABLE_TIMER_2MIN, HideDeviceAfterTimer, PNULL, FALSE);
            MMI_GetLabelTextByLang(STR_LIST_TEMP_VISIBLE_MK, &str_info);
#ifndef MMI_GUI_STYLE_TYPICAL
            MMIBT_AppendTwoLineTextIconSmallTextListItem(MMIBT_MAIN_LIST_CTRL_ID,STR_BT_SETT_PHONE_DETECTABILITY,IMAGE_COMMON_RIGHT_ARROW,str_info,BT_INDEX_VISIBLE,TRUE);
#endif
            MMK_CloseWin(win_id);
            break;
            
        default:
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

/*****************************************************************************/
// 	Description : get the type of device hide
//	Global resource dependence :
//  Author: lifei.lu
//	Note:
/*****************************************************************************/
PUBLIC MMIBT_HIDE_DEVICE_TYPE_E  MMIBT_GetDeviceHideMode( void )
{
    MMIBT_HIDE_DEVICE_TYPE_E     hide_mode      =   MMIBT_HIDE_DEVICE_HIDDEN;
    MN_RETURN_RESULT_E      return_value    =   MN_RETURN_FAILURE;

    //get tv out mode from NV
    MMINV_READ(MMINV_BT_HIDE_DEVICE,&hide_mode,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        hide_mode = MMIBT_HIDE_DEVICE_HIDDEN;
        MMINV_WRITE(MMINV_BT_HIDE_DEVICE,&hide_mode);
    }

    return (hide_mode);
}

PUBLIC uint32  MMIBT_GetDeviceHideNDT( void )
{
    NVITEM_ERROR_E nv_reslut = NVERR_NONE;
    uint16 nv_len = 0;
    
    bt_config.is_visible = 0;
    //SCI_MEMSET(bt_config.name, 0, 20);
    bt_config.storage_src = 0;
    nv_len = NVITEM_GetLength(APP_NV_BT_CONFIG);
    nv_reslut = EFS_NvitemRead(APP_NV_BT_CONFIG, nv_len ,(uint8*)(&bt_config));
    
    if (NVERR_NONE == nv_reslut)
    {
        //sprintf(str, "NV item id: %d \nNV item value \nis_visible : %d \nname: %s \nstorage: %d", \
        //	APP_NV_BT_CONFIG, bt_config.is_visible, bt_config.name, bt_config.storage_src);
        SCI_TRACE_LOW("LLF_NV  gmw-- is_visible=%d,storage_src=%d", bt_config.is_visible,  bt_config.storage_src);
        return SCI_SUCCESS;
    }
    else
    {
        SCI_TRACE_LOW("LLF_NV  gmw-- nv_reslut=%d", nv_reslut);
        return SCI_ERROR;
    }
}

PUBLIC NDT_BT_CONFIG_T * MMIBT_GetNdtDeviceHideOrNot(void)
{
    return &bt_config;
}
/*****************************************************************************/
// 	Description :After 2 minutes, we should hide the devices.
//	Global resource dependence :
//  Author:lifei.lu
//	Note:
/*****************************************************************************/
LOCAL void HideDeviceAfterTimer(void)
{
    if (s_temproary_avilable_timer != 0)
    {
        SCI_TRACE_LOW("HideDeviceAfterTimer-Enter");
        DoOperation(MMIBT_OPC_HIDE,FALSE);
        MMK_StartTimerCallback(s_temproary_avilable_timer, TEMPROARY_AVILABLE_TIMER_2MIN, StopHideDeviceAfterTimer, PNULL, FALSE);
    }
}

/*****************************************************************************/
// 	Description :After 2 minutes, we should stop timer.
//	Global resource dependence :
//  Author:lifei.lu
//	Note:
/*****************************************************************************/
LOCAL void StopHideDeviceAfterTimer(void)
{
    if (s_temproary_avilable_timer != 0)
    {
        MMK_StopTimer(s_temproary_avilable_timer);
        s_temproary_avilable_timer = 0;
    }
}

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Discription:
//  Global resource dependence: none
//  Author: maryxiao
//  Note:
//
/*****************************************************************************/

MMI_TEXT_ID_T MMIBT_GetLocation(void)
{
    MMIFILE_DEVICE_E dev    = MMI_DEVICE_UDISK;
    MMI_TEXT_ID_T    text_id = {0};
    MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/
    
    MMIBT_GetFileUserSetting(MMINV_BT_FILE_LOCATION, &setting);
    
    do
    {
        if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(dev), MMIAPIFMM_GetDevicePathLen(dev)))
        {
            if(dev== setting.filedev)
            {
                text_id = MMIAPIFMM_GetDeviceName(dev);
            }
        }
    }while(++dev < MMI_DEVICE_NUM);
    
    
    return text_id;
}
/*****************************************************************************/
//  Discription: when bt is off set function inavailable, else set function
//  available
//  Global resource dependence: none
//  Author: maryxiao
//  Note:
//
/*****************************************************************************/
LOCAL void MMIBT_SetFunctionGray(BOOLEAN isgray)
{
     int     i = 0;

     for(i=BT_INDEX_NAME; i<BT_INDEX_HELP; i++)
     {
        GUILIST_SetItemGray(MMIBT_MAIN_LIST_CTRL_ID, i, isgray);
        GUILIST_SetItemInvalid(MMIBT_MAIN_LIST_CTRL_ID, i, isgray);
     }

#ifndef PDA_UI_DROPDOWN_WIN
	 if(FALSE == isgray)
 	 {
		if(!MMIBT_IsOnTransferStatus())
		{
			GUILIST_SetItemGray(MMIBT_MAIN_LIST_CTRL_ID, BT_INDEX_FILETRANS, TRUE);
        	GUILIST_SetItemInvalid(MMIBT_MAIN_LIST_CTRL_ID, BT_INDEX_FILETRANS, TRUE);
		}
 	 }
#endif
}
/*****************************************************************************/
//  Discription: append one line one item
//  Global resource dependence: none
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void AppendOneLineTextListItem(
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id,
                            uint16           pos,
                            BOOLEAN          is_update
                            )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;

    item_t.item_data_ptr = &item_data;
    item_data.softkey_id[0] = TXT_COMMON_OK;
    item_data.softkey_id[2] = STXT_RETURN;
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;

    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}
/*****************************************************************************/
//  Discription:
//  Global resource dependence: none
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_UpdateMainMenu(BOOLEAN is_init)
{
    MMI_IMAGE_ID_T              imgae_id = IMAGE_NULL;
    uint16                   device_name[BT_DEVICE_NAME_SIZE] = {0};
    MMI_STRING_T             str_info={0};
    wchar                    device_wname[BT_DEVICE_NAME_SIZE] = {0};
    MMI_IMAGE_ID_T              imgae_icon_id = IMAGE_NULL;
    BOOLEAN                     is_bt_on = FALSE;
    MMI_TEXT_ID_T               left_text_id = TXT_NULL;

    MMIBT_HIDE_DEVICE_TYPE_E                      cur_selection   =   MMIBT_HIDE_DEVICE_HIDDEN;
    MN_RETURN_RESULT_E      return_value    =   MN_RETURN_FAILURE;

    if (BT_GetState())
    {
        imgae_id = IMAGE_CHECK_SELECTED_ICON;
        imgae_icon_id = IMAGE_COMMON_RIGHT_ARROW;
        is_bt_on = TRUE;
        left_text_id = TXT_COMMON_CLOSE;
    }
    else
    {
        imgae_id = IMAGE_CHECK_UNSELECTED_ICON;
        imgae_icon_id = IMAGE_COMMON_RIGHT_ARROW;
        left_text_id = TXT_OPEN;
    }
    MMIBT_AppendOneLineTextIconListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_BLUETOOTH,imgae_id,BT_INDEX_OPEN,left_text_id,is_init);
#if 0	
    AppendPdaBtItem1ToggleLine(MMIBT_MAIN_LIST_CTRL_ID,STR_BT_LIST_BLUETOOTH_EXT01,BT_INDEX_OPEN,left_text_id,is_init);
    CTRLLIST_SetToggleState(MMIBT_MAIN_LIST_CTRL_ID,BT_INDEX_OPEN,(BT_GetState())?TRUE:FALSE);
#endif
    BT_GetLocalName(device_name);
    if(0 == device_name[0] ){
        BT_SetLocalName(L"MyFirstPhone");
        BT_GetLocalName(device_name);
    }
    MMIAPICOM_StrToWstr(device_name, device_wname);/*lint !e64*/
    str_info.wstr_len = MMIAPICOM_Wstrlen(device_name);
    str_info.wstr_ptr = device_name;
    MMIBT_AppendTwoLineTextIconSmallTextListItem(MMIBT_MAIN_LIST_CTRL_ID,STR_BT_SETT_RENAME_PHONE,imgae_icon_id,str_info,BT_INDEX_NAME,is_init);

    if(MMIBT_GetIsDeviceVisible())
    {
        if(is_bt_on)
        {
            imgae_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {
            imgae_id = IMAGE_CONTROL_LIST_CHECK_DISABLED;
        }
        left_text_id = TXT_COMMON_CLOSE;
    }
    else
    {
        if(is_bt_on)
        {
            imgae_id = IMAGE_CHECK_UNSELECTED_ICON;
        }
        else
        {
            imgae_id = IMAGE_CHECK_UNSELECTED_ICON_GRAY;
        }
        left_text_id = TXT_OPEN;
    }
 
    //cur_selection = MMIBT_GetDeviceHideMode;
    MMINV_READ(MMINV_BT_HIDE_DEVICE,&cur_selection,return_value);
    if (MN_RETURN_SUCCESS != return_value )
    {
        MMIBT_GetDeviceHideNDT();
        SCI_TRACE_LOW("MMIBT_UpdateMainMenu-bt_config.is_visible=%d", bt_config.is_visible);
        if(bt_config.is_visible)
        {
            cur_selection = MMIBT_HIDE_DEVICE_VISIBLE;
            MMINV_WRITE(MMINV_BT_HIDE_DEVICE,&cur_selection);
            DoOperation(MMIBT_OPC_VISIBLE,FALSE);
        }else{
            cur_selection = MMIBT_HIDE_DEVICE_HIDDEN;
            MMINV_WRITE(MMINV_BT_HIDE_DEVICE,&cur_selection);
            DoOperation(MMIBT_OPC_HIDE,FALSE);
        }
        MMINV_READ(MMINV_BT_HIDE_DEVICE,&cur_selection,return_value);
    }
    SCI_TRACE_LOW("MMIBT_Mainwin cur_mainwin %d" , cur_selection);
    if(cur_selection == MMIBT_HIDE_DEVICE_HIDDEN)
    {
        MMI_GetLabelTextByLang(STR_LIST_HIDDEN_MK, &str_info);
    }
    else if(cur_selection == MMIBT_HIDE_DEVICE_VISIBLE)
    {
        MMI_GetLabelTextByLang(STR_LIST_ALWAYS_VISIBLE_MK, &str_info);
    }
    else if(cur_selection == MMIBT_HIDE_DEVICE_TEMPORARY_AVAILABLE)
    {
        MMI_GetLabelTextByLang(STR_LIST_TEMP_VISIBLE_MK, &str_info);
    }
    else
    {
        MMI_GetLabelTextByLang(STR_LIST_ALWAYS_VISIBLE_MK, &str_info);
    }
    //MMIBT_AppendOneLineTextIconListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_MY_BT_VISIBILITY,imgae_id,BT_INDEX_VISIBLE,left_text_id,is_init);
    MMIBT_AppendTwoLineTextIconSmallTextListItem(MMIBT_MAIN_LIST_CTRL_ID,STR_BT_SETT_PHONE_DETECTABILITY,imgae_icon_id,str_info,BT_INDEX_VISIBLE,is_init);
    AppendOneLineTextListItem(MMIBT_MAIN_LIST_CTRL_ID,STR_BT_LIST_AVAILABLE_EXT01,BT_INDEX_PAIRES,is_init);	

/*#ifndef PDA_UI_DROPDOWN_WIN
	AppendOneLineTextListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_BT_TRANSFER_FILES,BT_INDEX_FILETRANS,is_init);
#endif
#ifdef MMIBT_FTS_SUPPORT
    AppendOneLineTextListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_BT_SET_SHARED_FILE,BT_INDEX_SHARED,is_init);
#endif

    str_info.wstr_len = 0;
    str_info.wstr_ptr = PNULL;
    MMI_GetLabelTextByLang(MMIBT_GetLocation(), &str_info);
    MMIBT_AppendTwoLineTextIconSmallTextListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_BT_SAVE_FILE_LOCATION,imgae_icon_id,str_info,BT_INDEX_LOCATION,is_init);
    AppendOneLineTextListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_HELP,BT_INDEX_HELP,is_init);*/
    if(is_bt_on)
    {
        MMIBT_SetFunctionGray(FALSE);
    }
    else
    {
        MMIBT_SetFunctionGray(TRUE);
    }

    if(is_init)
    {
        if (MMK_IsFocusWin(MMIBT_MAIN_MENU_WIN_ID))
        {
            MMK_SendMsg(MMIBT_MAIN_MENU_WIN_ID, MSG_FULL_PAINT, PNULL);
        }
    }
}
#endif
/*****************************************************************************/
//  Discription: Open the blue tooth main menu win
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_OpenMainMenuWin(void)
{
#ifndef WIN32
#ifdef FM_SUPPORT
    if( MMIAPIFM_IsFmActive() ) 
    {
        MMIPUB_OpenAlertWarningWin(TXT_EXIT_BACKGROUND_FM);
        return;
    }
#endif

#ifdef WIFI_SUPPORT_UNISOC_RX
    if ( MMIWIFI_GetIsWlanOn() )
    {
        MMIPUB_OpenAlertWarningWin(TXT_EXIT_WIFI_IS_OPEN);
        return;
    }
#endif
#endif
    //设置蓝牙的主操作信息
    MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_MAIN);
    //创建主菜单窗口
    if(MMK_IsOpenWin(MMIBT_MAIN_MENU_WIN_ID)){
        MMK_CloseWin(MMIBT_MAIN_MENU_WIN_ID);
    }
    MMK_CreateWin((uint32 *)MMIBT_MAIN_MENU_WIN_TAB, NULL);
    //创建主窗口的菜单
#ifdef MMI_GUI_STYLE_TYPICAL
    MMIBT_CreateMainMenuList(MMIBT_MAIN_MENU_WIN_ID, MMIBT_MAIN_MENU_CTRL_ID, STR_BT_LIST_BLUETOOTH_EXT01);
#endif
}
#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Discription: create list menu by current operation
//  Global resource dependence: None
//  Author:
//  Note :
//  Return:
/*****************************************************************************/
LOCAL void MMIBT_CreateMainMenuList(
                                    MMI_WIN_ID_T             win_id, //IN: list menu's window id
                                    MMI_CTRL_ID_T            ctrl_id, //IN: list menu's id
                                    MMI_TEXT_ID_T            text_id //IN: list menu titles's text id
                                    )
{
	GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMI_STRING_T    kstring = {0};
    uint32          node_id = 0;
    uint16           i = 0;
    uint32 s_bt_menu_main[MMIBT_MAIN_MEUN_NUM][2] ={
                                                    {ID_SET_BT_ON,              TXT_BT_SET_ON},//Set "BT ON" items
                                                    {ID_MY_BT_DEVICE_NAME,      STR_BT_SETT_RENAME_PHONE},//set "BT Name" item
                                                    {ID_MY_BT_VISIBILITY,       STR_BT_SETT_PHONE_DETECTABILITY},//Set "BT Visibility" items
                                                    {ID_BT_DEVICE_LIST,         STR_BT_LIST_AVAILABLE_EXT01},//set "Paired Device" item
/*lulifei remove this for UI spec
#ifdef MMIBT_FTS_SUPPORT
                                                    {ID_BT_SHARED_FILE_SETTING, TXT_BT_SET_SHARED_FILE},//set "Shared file Setting" item
#endif
                                                    {ID_BT_SAVE_FILE_LOCATION,  TXT_BT_SAVE_FILE_LOCATION},//set "BT save file location" item
                                                    {ID_BT_HELP,                TXT_HELP}//set "Help" item*/
                                                    };
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    if(!MMK_IsOpenWin(win_id))
    {
        SCI_TRACE_LOW("MMIBT_CreateMainMenuList window already opened !");
        return;
    }

    //creat dynamic menu
    GUIMENU_CreatDynamic(&both_rect,win_id,ctrl_id,GUIMENU_STYLE_THIRD);
    //GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);

    MMK_SetAtvCtrl(win_id, ctrl_id);

    //set menu title
    MMI_GetLabelTextByLang(text_id, &kstring);
    GUIMENU_SetMenuTitle(&kstring,ctrl_id);

        //set bt main menu items
    for(i = 0; i < MMIBT_MAIN_MEUN_NUM; i++)
    {
        if((ID_SET_BT_ON == s_bt_menu_main[i][MMIBT_MEUN_ID_INDEX]) && BT_GetState())
        {
            //bt is on need change item info to {ID_SET_BT_ON, TXT_BT_SET_ON}
            text_id = TXT_SET_BT_OFF;
            node_id = ID_SET_BT_OFF;
        }
        else
        {
            //get item info from s_bt_menu_main
            node_id = s_bt_menu_main[i][MMIBT_MEUN_ID_INDEX];
            text_id = s_bt_menu_main[i][MMIBT_MEUN_TEXT_INDEX];
        }
        MMI_GetLabelTextByLang(text_id, &kstring);
        node_item.item_text_ptr = &kstring;
        GUIMENU_InsertNode(i, node_id, 0, &node_item, ctrl_id);
    }
 }
#endif
/*****************************************************************************/
// Description : 蓝牙开关设置页面
// Global resource dependence :
// Author: Ming.Song
// Note:2007-8-6
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIBT_SetBluetoothMode (void)
{
   //建立蓝牙开关设置窗口
   MMIBT_OpenMainMenuWin();
   //返回窗口ID;
   return MMIBT_MAIN_MENU_WIN_ID;
}
#endif

#ifdef MMI_BT_PBAP_SUPPORT
/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIBT_HandlePubwinConnectWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        MMIBT_StopPbapSyncTimer();
	    MMIBT_PbapClientTpDisconnect();
	    MMK_CloseWin(win_id);
	    break;
    
    case MSG_APP_RED:
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinSyncWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMIBT_PbapAbortClient();
        MMIBT_SetPbapCancelValue(TRUE);
	    MMK_CloseWin(win_id);
        MMIPUB_OpenAlertSuccessWin(STXT_CANCEL);
        break;
     
    case MSG_APP_RED:
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}

/*****************************************************************************/
//  Description : open query win
//  Global resource dependence : 
//  Author: yanyan.an
//  Note: 
/*****************************************************************************/
LOCAL void OpenQueryWin(MMI_TEXT_ID_T text_id, MMIPUB_HANDLE_FUNC    func)
{
	MMIPUB_OpenQueryWinByTextId(text_id, IMAGE_PUBWIN_QUERY,PNULL,(MMIPUB_HANDLE_FUNC)func);
}

/*****************************************************************************/
//  Description : MMIBT_CreateTimer
//  Global resource dependence : 
//  Author: yanyan.an
//  Note: 创建超时定时器
/*****************************************************************************/
PUBLIC void MMIBT_CreatePbapSyncTimer(void)
{
    SCI_TRACE_LOW("[MMIBT] MMIBT_CreatePbapSyncTimer enter");
    
    if(s_bt_connect_timeout_timer != 0) 
    {
        MMIBT_StopPbapSyncTimer();
    }
    
    s_bt_connect_timeout_timer = MMK_CreateTimerCallback(BT_PBAP_TIMER_OUT, MMIBT_ProcessTimerOver, 0, FALSE);
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_CreatePbapSyncTimer exit, timer_id = %d", s_bt_connect_timeout_timer);
}

/******************************************************************************/
// Description: MMIBT_StopTimer
// Global resource dependence: 
// Author: yanyan.an
// Note: 停止超时定时器
/******************************************************************************/
PUBLIC void MMIBT_StopPbapSyncTimer(void)
{
    SCI_TRACE_LOW("[MMIBT] MMIBT_StopPbapSyncTimer enter");
    
    if(s_bt_connect_timeout_timer != 0)    
    {   
        MMK_StopTimer(s_bt_connect_timeout_timer);   
        s_bt_connect_timeout_timer = 0;    
    }   
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_StopPbapSyncTimer exit, timer_id = %d", s_bt_connect_timeout_timer);
}

/******************************************************************************/
// Description: MMIBT_ProcessTimerOver
// Global resource dependence: 
// Author: yanyan.an
// Note: 超时保护机制
/******************************************************************************/
LOCAL void MMIBT_ProcessTimerOver(uint8 timer_id, uint32 param)
{
    SCI_TRACE_LOW("[MMIBT] MMIBT_ProcessTimerOver enter");

    MMIBT_StopPbapSyncTimer();
    MMK_CloseWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID);
    MMIPUB_OpenAlertWarningWin(STR_CONTACT_SYNC_INTERRUPT);
    MMIBT_PbapClientTpDisconnect();
}
#endif
