/****************************************************************************
** File Name:     mmibt_inquireddevice.c                                        *
** Author:         yuantao.xu                                            *
** Date:           8/22/2012                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the bluetooth device paired*
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 8/22/2012    yuantao.xu           Create                                  *
**                                                                         *
****************************************************************************/
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#define _MMIBT_INQUIREDDEVICE_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#include "mmi_app_bt_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_timer.h"
#include "mmibt_func.h"
#include "mmipub.h"
#include "guilabel.h"
#include "mmifmm_export.h"   //bt_abs.h中用到ffs.h中的踞构
#include "bt_abs.h"
#include "bt_cfg.h"
#include "mmibt_text.h"
#include "mmi_default.h"
#include "guitext.h"
#include "mmibt_a2dp.h"
#include "mmibt_nv.h"
#include "mmifmm_text.h"
#include "guimenu.h"
#include "guires.h"
#include "guiedit.h"
#include "mmibt_inquireddevice.h"
#include "mmi_autotest.h"
#include "mmicc_export.h"
#include "guictrl_api.h"
#include "mmikl_export.h"
#include "mmibt_app.h"
#include "mmibt_headset.h"
#include "mmi_appmsg.h"
#include "mmicom_banner.h"
#include "variant_config_nv.h"
#ifdef BLUETOOTH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define  MMIBT_PIN_MIN_LENGTH               1                   //pair pin code min wstr_len
#define  MMIBT_PIN_MAX_LENGTH               16                  //pair pin code max wstr_len
#define  MMIBT_PAIR_TEXT_LEN                (51+BT_DEVICE_NAME_SIZE+7)//TXT+DEVICE_NAME+SSP_KEY
// 用于判断蓝牙密码输入的位数是否正确。
#define PAIR_CODE_LEN_IS_IN_RANGE(_LEN)            \
    ((MMIBT_PIN_MIN_LENGTH <= (_LEN))    \
    && (MMIBT_PIN_MAX_LENGTH >= (_LEN))) \

#ifdef MMI_PDA_SUPPORT
typedef enum
{
    BT_OPTMENU_NODE_ROOT,           //root/*lint !e749*/
    BT_OPTMENU_NODE_AUTHORITY_RW,
    BT_OPTMENU_NODE_AUTHORITY_READ
}MMIPB_OPTMENU_NODE_E;
#endif
#ifdef TOUCH_PANEL_SUPPORT
typedef enum
{
    BT_INQUIRY_OPTMENU_NODE_ROOT,           //root/*lint !e749*/
    MMIBT_INQUIRY_STOP_ID,
    MMIBT_INQUIRY_PAIRY_ID
}MMIPB_INQUIRY_OPTMENU_NODE_E;
#endif

//lulifei add the paired device
typedef enum{
	ID_PAIRED_DEVICE_SEARCH_DEVICE,
	ID_PAIRED_DEVICE_INQUIRED_DEVICE
}MMIBT_PAIRED_DEVICE_TYPE_E;

typedef struct mmibt_device_list_param_tag
{
    MMIBT_TRANSFER_FILE_INFO_T    transfer_file_info;
    MMIBT_OPERTATION_TYPE_E     root_opertion;
    uint32              current_service;
}MMIBT_DEVICE_LIST_PARAM_T;

#define BT_HEADSET_OPENWAIT_TIMER    (2*1000)  //被动配对时等待主动连接的timer
LOCAL uint8 s_headset_openwait_timer = 0;

LOCAL uint8 s_cur_select_index = 0;
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
static  BT_DEVICE_INFO   *s_inquired_device_ptr = PNULL;      //the info of inquired device
static  uint8            s_inquired_device_num = 0;           // the number of inquired device
static  BOOLEAN           s_bt_is_self_pair = FALSE;
static  BT_DEVICE_INFO      s_pin_request_info = {0};         //the address info of request input pin
static  BT_DEVICE_INFO_EXT  s_pin_request_info_ext = {0};     //the address info of ssp
static uint16          list_num = 0; //lulifei add the number of the paired devices
static  BOOLEAN             s_update_ui = FALSE;//lulifei add to update the UI
static uint8	  paired_device_index[MMIBT_MAX_DEV_ITEMS] = {0}; //lulifei add the paired device index
static uint16          headset_item = 10; //lulifei add the number of the paired devices
static  BOOLEAN             s_is_pin_cancel = FALSE;          //TRUE: user refuse pair. FALSE: user accept pair request
extern  MMIBT_OPERTATION_TYPE_E g_interrupt_opertor;
extern BT_DEVICE_INFO     g_current_bt;
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL BOOLEAN IsSelfPair(void);
/*****************************************************************************/
//  Discription:  check is searched device already be searched
//  Global resource dependence: None
//  Author:
//  Note :
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN IsSearchingDeviceExist(BT_ADDRESS *bt_addr_ptr);
/*****************************************************************************/
//  Discription: Handle blue tooth  pin input win msg
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandlePinInputWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          );
/*****************************************************************************/
//  Discription: Handle blue tooth  Inquired device list win msg
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleInquiredDevListWinMsg(
                                                 MMI_WIN_ID_T win_id,
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param
                                                 );
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleInquiryOptMenuWinMsg(
                                                 MMI_WIN_ID_T win_id,
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param
                                                 );

/*****************************************************************************/
//  Discription: Handle blue tooth  numeric passkey check
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleNumericPasskeyWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          );

/*****************************************************************************/
//  Discription: Handle searching new device waiting win msg
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCanbeCancelWaitingWinMsg(
                                                    MMI_WIN_ID_T win_id,
                                                    MMI_MESSAGE_ID_E msg_id,
                                                    DPARAM param
                                                    );
#ifdef MMI_PDA_SUPPORT
LOCAL void CreateSBandPopmenu(void);

/*****************************************************************************/
//  Description : MMIFMM_CreateNewSoftKey
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIBT_CreateNewSoftKey(MMI_HANDLE_T win_handle, MMI_TEXT_ID_T leftkey_id, MMI_TEXT_ID_T midkey_id, MMI_TEXT_ID_T rightkey_id);
#endif
/*****************************************************************************/
//  Discription: Open the blue tooth Inquired device list win
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL void  OpenInquiredDevListWin(void);

/*****************************************************************************/
//  Description : display numeric passkey information
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void Display_NumericPasskeyInfo(void);

/*****************************************************************************/
//  Discription:check headset permit to open
//  Global resource dependence: none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HeadsetOpenIsPermit(BT_ADDRESS * bt_addr);
/*****************************************************************************/
//  Discription: insert menu node bu text id
//  Global resource dependence: none
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                     MMI_CTRL_ID_T      ctrl_id,        //控件id
                     MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                     uint16          	node_id,        //节点编号
                     uint16          	parent_node_id, //父节点编号
                     uint16          	index,          //位置
                     BOOLEAN            is_graged
                     );

/*****************************************************************************/
//  Discription:
//  Global resource dependence: none
//  Author: yang.yang
//  Note:
//
/*****************************************************************************/
LOCAL void MMIBT_OpenWaitTimerOut(void);

/*****************************************************************************/
//  Discription:Add the paired device list
//  Global resource dependence: none
//  Author: lifei.lu
//  Note:
//
/*****************************************************************************/
LOCAL void MMIBT_AddPairedDeviceList(MMIBT_PAIRED_DEVICE_TYPE_E pair_display);

/*****************************************************************************/
//  Discription:Add the inquire device ui
//  Global resource dependence: none
//  Author: lifei.lu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenInquireDeviceListWin(MMIBT_DEVICE_LIST_PARAM_T *param_ptr);

#ifdef MMI_BT_PBAP_SUPPORT
/*****************************************************************************/
//  Discription:check headset permit to open
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandsetGatewayOpenIsPermit(BT_ADDRESS * bt_addr);
#endif

/*---------------------------------------------------------------------------*/
//the list of inquiried device win table
WINDOW_TABLE(MMIBT_INQUIRED_DEV_LIST_WIN_TAB) =
{
    WIN_FUNC((uint32) HandleInquiredDevListWinMsg ),
    //WIN_TITLE(TXT_NULL),
    WIN_ID(MMIBT_INQUIRED_DEV_LIST_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIBT_INQUIRED_DEV_LIST_CTRL_ID),
    //CREATE_ANIM_CTRL(MMIBT_ANIMATE_SEARCHING_CTRL_ID, MMIBT_INQUIRED_DEV_LIST_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
#else
    WIN_SOFTKEY(PNULL, PNULL, STXT_RETURN),
#endif
    END_WIN
};
// the window for input pin,startup
WINDOW_TABLE( MMIBT_PIN_INPUT_WIN_TAB) =
{
    WIN_FUNC( (uint32)HandlePinInputWinMsg ),
    WIN_ID( MMIBT_PIN_INPUT_WIN_ID ),
    //WIN_TITLE( TXT_BT_ENTER_PIN ),
	//WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_EDIT_PASSWORD_CTRL(MMIBT_PIN_MAX_LENGTH,MMIBT_PIN_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the window for input pin,startup
WINDOW_TABLE( MMIBT_NUMERIC_PASSKEY_WIN_TAB) =
{
    WIN_FUNC( (uint32)HandleNumericPasskeyWinMsg ),
    WIN_ID( MMIBT_NUMERIC_PASSKEY_WIN_ID ),
    WIN_TITLE( STR_BT_ACCEPT_PAIR_REQUEST_EXT01 ),
    CREATE_TEXT_CTRL(MMIBT_NUMERIC_PASSKEY_CTRL_ID),
    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),    //bug 1022595
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE(MMIBT_INQUIRY_OPT_WIN_TAB) =
{
    WIN_FUNC( (uint32)HandleInquiryOptMenuWinMsg),
    WIN_ID(MMIBT_INQUIRY_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    END_WIN
};
#endif

/*****************************************************************************/
//  Discription: Handle blue tooth  Inquired device list win msg
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleInquiredDevListWinMsg(
                                                 MMI_WIN_ID_T win_id,
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param
                                                 )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    uint16                  cur_select_item  = 0;
    uint16                  i = 0;
    uint16                  total_num = 0;
    uint16                  total_list_num = 0;
    uint16                  anim_width = 0;
    uint16                  anim_height = 0;
    uint16                  frame_num = 0;
    BT_STATUS               return_value = BT_PENDING;
    MMI_STRING_T            wait_text = {0};
    GUI_LCD_DEV_INFO        lcd_dev_info =  {0};
    GUI_BOTH_RECT_T         anim_both_rect = MMITHEME_GetWinTitleBothRect(win_id);
#ifdef MMI_PDA_SUPPORT
    GUI_BOTH_RECT_T         list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#endif
    uint8                   addr_str[BT_ADDRESS_SIZE*4] = {0};
    uint16                  index = 0;
    uint32                  addr_str_len = 0;
    MMI_STRING_T    query_text = {0};
    MMI_STRING_T    name_text = {0};
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();
    //lulifei add for the opertion of paired devices
    MMIBT_DEVICE_LIST_PARAM_T  *win_param_ptr = (MMIBT_DEVICE_LIST_PARAM_T *)MMK_GetWinUserData(win_id);
    BT_DEVICE_INFO current_device_info = {0};
    MMI_STRING_T            text_str1 = {0};
    MMI_STRING_T            text_str2 = {0};
    MMI_STRING_T    updatestb_str = {0};
    wchar text_temp[32] = {0};
    wchar old_str_N[16] = {0};
    wchar replace_str[16] = {0};
    wchar replace_str_1[16] = {0};
    SCI_TRACE_LOW("[MMIBT] HandleInquiredDevListWinMsg: msg_id=0x%x", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUIRES_GetAnimWidthHeight(&anim_width, &anim_height, &frame_num, AINM_BT_SEARCHING, win_id);
            
            if(MMI_DIRECTION_LEFT == MMITHEME_GetDisDirection())
            {
                anim_both_rect.h_rect.left = anim_both_rect.h_rect.right - anim_width;
                anim_both_rect.v_rect.left = anim_both_rect.v_rect.right - anim_width;
#ifndef MMI_PDA_SUPPORT
                anim_both_rect.h_rect.top = (anim_both_rect.h_rect.bottom - anim_height)/2;
                anim_both_rect.v_rect.top = (anim_both_rect.v_rect.bottom - anim_height)/2;
#else
                anim_both_rect.h_rect.top   += (anim_both_rect.h_rect.bottom - anim_both_rect.h_rect.top - anim_height)/2;
                anim_both_rect.v_rect.top   += (anim_both_rect.v_rect.bottom - anim_both_rect.h_rect.top - anim_height)/2;
                anim_both_rect.h_rect.bottom = anim_both_rect.h_rect.top + anim_height - 1;
                anim_both_rect.v_rect.bottom = anim_both_rect.v_rect.top + anim_height - 1;
#endif
            }
            else
            {
                anim_both_rect.h_rect.left = 0;
                anim_both_rect.v_rect.left = 0;
                anim_both_rect.h_rect.right = anim_width -1;
                anim_both_rect.v_rect.right = anim_width -1;
#ifndef MMI_PDA_SUPPORT
                anim_both_rect.h_rect.top = (anim_both_rect.h_rect.bottom - anim_height)/2;
                anim_both_rect.v_rect.top = (anim_both_rect.v_rect.bottom - anim_height)/2;
#else
                anim_both_rect.h_rect.top   += (anim_both_rect.h_rect.bottom - anim_both_rect.h_rect.top - anim_height)/2;
                anim_both_rect.v_rect.top   += (anim_both_rect.v_rect.bottom - anim_both_rect.h_rect.top - anim_height)/2;
                anim_both_rect.h_rect.bottom = anim_both_rect.h_rect.top + anim_height - 1;
                anim_both_rect.v_rect.bottom = anim_both_rect.v_rect.top + anim_height - 1;
#endif
            }
            
            GUIAPICTRL_SetBothRect(MMIBT_ANIMATE_SEARCHING_CTRL_ID, &anim_both_rect);
#ifdef MMI_PDA_SUPPORT
#if defined(MMI_ISTYLE_SUPPORT)
            if (MMITHEME_IsIstyle())
            {
                GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_STOP, FALSE);
                GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_LEFT, MSG_CTL_PENOK, MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID);
                MMK_SetWinDisplayStyleState(win_id, WS_HAS_BUTTON_SOFTKEY, FALSE);
            }
            else
#endif
            {
                MMK_SetWinDisplayStyleState(win_id, WS_HAS_BUTTON_SOFTKEY, TRUE);
                MMIBT_CreateNewSoftKey(win_id, TXT_NULL, STXT_STOP, TXT_NULL);
            }
            
            list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
            GUIAPICTRL_SetBothRect(MMIBT_INQUIRED_DEV_LIST_CTRL_ID, &list_both_rect);
#endif
            MMIDEFAULT_AllowOpenKeylocWin(FALSE);
            lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
            lcd_dev_info.block_id = GUI_BLOCK_MAIN;
            AnimatePlayCtrlByState(MMIBT_ANIM_INIT);
            GUIWIN_SetTitleTextId(win_id, PNULL, FALSE);
            
            GUIWIN_SetTitleBackground(win_id, MMITHEME_GetWinTitleBar(), 0);
            GUIWIN_SetTitleFontColor(win_id, MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0));
            MMK_SetAtvCtrl( win_id, MMIBT_INQUIRED_DEV_LIST_CTRL_ID );
            if(s_inquired_device_ptr != PNULL)
            {
                SCI_FREE(s_inquired_device_ptr);
                s_inquired_device_ptr = PNULL;
            }
            if(s_inquired_device_ptr == PNULL)
            {
                s_inquired_device_ptr = (BT_DEVICE_INFO *)SCI_ALLOCA(sizeof(BT_DEVICE_INFO)*MMIBT_MAX_DEV_ITEMS);
                if(s_inquired_device_ptr != PNULL)
                {
                    SCI_MEMSET(s_inquired_device_ptr, 0x00, sizeof(BT_DEVICE_INFO)*MMIBT_MAX_DEV_ITEMS);
                }
            }
            GUIWIN_SetSoftkeyTextId(win_id,  PNULL, PNULL, STXT_RETURN, TRUE);
            //GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_BACK,RIGHT_BUTTON,TRUE);
        }
        break;
    case MSG_GET_FOCUS:
        //lulifei add to update the UI
        if(s_update_ui)
        {
            MMK_PostMsg(win_id, MSG_BT_SEARCH_CANCELED, PNULL, PNULL);
        }
        else
        {
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, PNULL);
        }
        break;
    case MSG_FULL_PAINT:
        //reset status bar
        total_num = GUILIST_GetTotalItemNum(MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
        MMI_GetLabelTextByLang(STR_BT_HDR_AVAILABLE_EXT01, &updatestb_str);
        MMIAPICOM_Wstrncpy(text_temp, updatestb_str.wstr_ptr,updatestb_str.wstr_len);

        SCI_TRACE_LOW("[MMIBT] HandleInquiredDevListWinMsg: MSG_FULL_PAINT total_num=%d", total_num);
        if(total_num == 0)
        {
            MMIAPICOM_Wstrncpy(old_str_N,L"(%N)",MMIAPICOM_Wstrlen(L"(%N)"));
            MMIAPICOM_Wstrncpy(replace_str,L" ",MMIAPICOM_Wstrlen(L" "));

            if((MMIBT_OPC_SEARCH == MMIBT_GetOpertor().current_opertion) && (!MMK_IsOpenWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID)))
            {
                MMI_GetLabelTextByLang(STR_NOTE_WAITING, &wait_text);
                MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_SEARCH_DEVICE_PUBWIN_ID,IMAGE_NULL,
                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleCanbeCancelWaitingWinMsg);
                MMIBT_StartOperationWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID,MMIBT_OPC_SEARCH,CreatePubWinCallBack);
            }
        }
        else if(total_num >= 10)
        {
            wchar* str_num[9] = {L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9"};
            MMIAPICOM_Wstrncpy(old_str_N,L"%N",MMIAPICOM_Wstrlen(L"%N"));
            MMIAPICOM_Wstrcat(replace_str,str_num[(total_num/10) - 1]);
            itoa(total_num%10,replace_str_1,10);
            MMIAPICOM_Wstrcat(replace_str,replace_str_1);
        }
        else
        {
            MMIAPICOM_Wstrncpy(old_str_N,L"%N",MMIAPICOM_Wstrlen(L"%N"));
            itoa(total_num,replace_str,10);
        }

        MMIAPICOM_WstrReplace(text_temp, sizeof(text_temp), old_str_N, replace_str);
        if(MMIAPICOM_StatusAreaSetViewHeaderinfo(text_temp, MMIAPICOM_Wstrlen(text_temp)))
        {
            GUIWIN_UpdateStb();
        }
        break;

#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL,PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                break;
            default:
                if(MMIBT_OPC_SEARCH == MMIBT_GetOpertor().current_opertion)
                {
                    break;
                }
                if(GUILIST_GetTotalItemNum(MMIBT_INQUIRED_DEV_LIST_CTRL_ID)>0)
                {
                    CreateSBandPopmenu();
                }
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_MENU:
        if(MMIBT_OPC_SEARCH == MMIBT_GetOpertor().current_opertion)
        {
            break;
        }
        if(GUILIST_GetTotalItemNum(MMIBT_INQUIRED_DEV_LIST_CTRL_ID)>0)
        {
            CreateSBandPopmenu();
        }
        break;
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
        total_list_num = GUILIST_GetTotalItemNum(MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
        if(total_list_num == 0) {break;}
        if(MMIBT_OPC_SEARCH == MMIBT_GetOpertor().current_opertion)
        {
            //only can pair when stop search
            break;
        }
        cur_select_item = GUILIST_GetCurItemIndex(MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
        if(cur_select_item < list_num)
        {
            int                     index = 0;
            MMIBT_OPERTATION_TYPE_E root_opertion = 0;
            BT_DEVICE_INFO dev_info = {0};
            BOOLEAN is_exist_connect_headset = MMIBT_OpenedHeadsetIsExist(&dev_info);
            
            //MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_LIST_PAIRED);
            index = paired_device_index[cur_select_item];
            s_cur_select_index = index;
            BT_GetPairedDeviceInfo(BT_SERVICE_ALL, index, &current_device_info);
            MMIBT_SetCurBtInfo(&current_device_info);
            SCI_TRACE_LOW("[MMIBT]HandleInquiredDevListWinMsg_menu MMIBT_GetOpertor().root_opertion =%d",MMIBT_GetOpertor().root_opertion);
            if(PNULL != win_param_ptr)
            {
			SCI_TRACE_LOW("[MMIBT]HandleInquiredDevListWinMsg: %d, %d, %d",is_exist_connect_headset, win_param_ptr->root_opertion, win_param_ptr->current_service);
            }
            if(PNULL == win_param_ptr)
            {
                root_opertion = MMIBT_GetOpertor().root_opertion;
            }
            else
            {
                root_opertion = win_param_ptr->root_opertion;
            }
            if(MMIBT_OPC_MAIN == root_opertion)
            {
                BOOLEAN                is_connect = FALSE;
                uint16                 device_type = 0;
                MMI_MENU_GROUP_ID_T group_id = MENU_BT_OTHER_DEVICE_DETAILS;
                
                //option menu
                BT_GetDeviceType(&current_device_info, &device_type);
                if(device_type & BT_DEVICE_CLASS_AUDIO)
                {
                    //headset
                    is_connect = MMIBT_DeviceIsConnectedHeadset(&current_device_info.addr);
                    //open or close headset permit
                    if(is_connect)
                    {
                        //current headset not connected
                        MMIBT_EnterDeviceDetailsWin(&current_device_info,PNULL,HEADSET_DISCONNECTED);
                        group_id = MENU_BT_DEACTIVE_DEVICE_DETAILS;
                    }
                    else
                    {
                        //current headset already connected
                        MMIBT_EnterDeviceDetailsWin(&current_device_info,PNULL,HEADSET_CONNECTED);
                        group_id = MENU_BT_DEVICE_DETAILS;
                    }
                    GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_AUTO_CONNECT_TOGGLE, FALSE);
                }
                else
                {
                    //not headset
                    MMIBT_EnterDeviceDetailsWin(&current_device_info,PNULL,NOT_HEADSET);                 
                }
                //lulifei add for BT UI menu spec begin
                GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_DEVICE_CONNECT, FALSE);
                GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_DEVICE_DISCONNECT, FALSE);
                GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_DEVICE_RENAME, FALSE);
                GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_DEVICE_DELETE_ALL, FALSE);
                GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_DEVICE_DETAIL, FALSE);
		   		GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_AUTO_CONNECT_TOGGLE, FALSE);
                //GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_ACCREDIT_SETTINGS, FALSE);
                GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_SEATCH_DEVICE, FALSE);
                //lulifei add for BT UI menu spec end
            }
            else if(is_exist_connect_headset && (PNULL != win_param_ptr)&&((win_param_ptr->root_opertion == MMIBT_OPC_RECIEVE) ||(win_param_ptr->root_opertion == MMIBT_OPC_OPP_SEND)))
            {
                wchar    replace_wchar[100] = {0};
                wchar    test[100] = {0};
                BT_STATUS      result = BT_ERROR;
                name_text.wstr_ptr = dev_info.name;
                name_text.wstr_len = MMIAPICOM_Wstrlen(dev_info.name);
                query_text.wstr_ptr = test;
                SCI_TRACE_LOW("[MMIBT]HandleInquiredDevListWinMsgOK1: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x", 
				g_current_bt.addr.addr[0],
				g_current_bt.addr.addr[1],
				g_current_bt.addr.addr[2],
				g_current_bt.addr.addr[3],
				g_current_bt.addr.addr[4],
				g_current_bt.addr.addr[5]);
                SCI_TRACE_LOW("[MMIBT]HandleInquiredDevListWinMsgOK2: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x", 
				dev_info.addr.addr[0],
				dev_info.addr.addr[1],
				dev_info.addr.addr[2],
				dev_info.addr.addr[3],
				dev_info.addr.addr[4],
				dev_info.addr.addr[5]);
                MMIAPICOM_Wstrncpy(replace_wchar, name_text.wstr_ptr, name_text.wstr_len);
                MMIAPICOM_CustomGenerateReplaceStrByTextId(STR_BLUETOOTH_PAIRED_OTHER_DISC,L"%U",replace_wchar,100,&query_text);
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_CLOSE_HEADSET);
                MMIBT_OpenQueryWin(MMI_BT_QUERY_WIN_ID,&query_text,  PNULL, MMIBT_OPC_CLOSE_HEADSET);
            }
            else
            {
                //selected this device to do operation
                DoOperation(root_opertion,TRUE);
            }
            break;
        }
        else
        {
            break;
        }
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    {
        BOOLEAN                 is_connect = FALSE;
        int32              same_bt = 0;
        uint32                  service_type = 0;
        int                     index = 0;
        MMIBT_OPERTATION_TYPE_E opc = MMIBT_OPC_NOT_CHANGE;
        BT_DEVICE_INFO dev_info = {0};
            
        if(MMIBT_OPC_SEARCH == MMIBT_GetOpertor().current_opertion)
        {
            //only can pair when stop search
            break;
        }
        total_list_num = GUILIST_GetTotalItemNum(MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
        if(total_list_num == 0)
        {
            break;
        }
            cur_select_item = GUILIST_GetCurItemIndex(MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
        SCI_TRACE_LOW("[MMIBT] HandleInquiredDevListWinMsg: Web1 %d, %d", cur_select_item, list_num);

        //lulifei add the pired devices
        if(cur_select_item < list_num)
        {
            index = paired_device_index[cur_select_item];
            BT_GetPairedDeviceInfo(BT_SERVICE_ALL, index, &current_device_info);

            service_type =  BT_GetPairedDeviceService(&current_device_info.addr);
            if(BT_SERVICE_A2DP & service_type || (BT_SERVICE_HANDSET|BT_SERVICE_HANDFREE) & service_type)
            {
                is_connect = MMIBT_DeviceIsConnectedHeadset(&current_device_info.addr);
                headset_item = cur_select_item;
                //connect or disconnect headset
                if(is_connect)
                {
                    //if exist HFG service and HFG  active need to Close
                    //or if exist A2DP service and A2DP active need to Close too
                    opc = MMIBT_OPC_CLOSE_HEADSET;
                }
                else
                {
                    opc = MMIBT_OPC_OPEN_HEADSET;
                }
            }
            else
            {
                //authority
                opc = MMIBT_OPC_DEVICE_DETAILS;
            }
            MMIBT_SetCurBtInfo(&current_device_info);
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, opc);
            //connect headset
            if(BT_GetState() || MMIBT_OPC_DEVICE_DETAILS == opc)
            {
                
                if(MMIBT_OPC_CLOSE_HEADSET == opc)
                {
                    //open or close headset query
                    MMI_GetLabelTextByLang(STR_SOFTKEY_BT_DISCONNECT_MK, &text_str1);
                    text_str2.wstr_ptr = current_device_info.name;
                    text_str2.wstr_len = MMIAPICOM_Wstrlen(current_device_info.name);
                    //BT is ON, do operation
                    MMIBT_OpenQueryWin(MMI_BT_QUERY_WIN_ID, &text_str1,  & text_str2, opc);
                }
                else
                {
                    DoOperation(opc, FALSE);
                }
            }
            else
            {
                MMI_GetLabelTextByLang(STR_BT_TURN_ON, &text_str1);
                MMIBT_OpenQueryWin(MMI_BT_QUERY_WIN_ID, &text_str1, PNULL, opc);
            }
        }
        else
        {
            BOOLEAN is_exist_connect_headset = MMIBT_OpenedHeadsetIsExist(&dev_info);
            SCI_TRACE_LOW("[MMIBT]HandleInquiredDevListWinMsg:Web2 %d, %d, %d", opc, win_param_ptr->root_opertion, win_param_ptr->current_service);
            SCI_TRACE_LOW("[MMIBT]HandleInquiredDevListWinMsg:Web3 %d, %d", is_exist_connect_headset, service_type);
            if(is_exist_connect_headset &&((win_param_ptr->root_opertion == MMIBT_OPC_RECIEVE) ||(win_param_ptr->root_opertion == MMIBT_OPC_OPP_SEND)))
            {
                wchar    replace_wchar[100] = {0};
                wchar    test[100] = {0};
                BT_STATUS      result = BT_ERROR;
                name_text.wstr_ptr = dev_info.name;
                name_text.wstr_len = MMIAPICOM_Wstrlen(dev_info.name);
                query_text.wstr_ptr = test;
                SCI_TRACE_LOW("[MMIBT]HandleInquiredDevListWinMsg:Web4 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x", 
				g_current_bt.addr.addr[0],
				g_current_bt.addr.addr[1],
				g_current_bt.addr.addr[2],
				g_current_bt.addr.addr[3],
				g_current_bt.addr.addr[4],
				g_current_bt.addr.addr[5]);
                SCI_TRACE_LOW("[MMIBT]HandleInquiredDevListWinMsg:Web5 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x", 
				dev_info.addr.addr[0],
				dev_info.addr.addr[1],
				dev_info.addr.addr[2],
				dev_info.addr.addr[3],
				dev_info.addr.addr[4],
				dev_info.addr.addr[5]);
                //MMIBT_SetCurBtInfo(&dev_info);
                MMIAPICOM_Wstrncpy(replace_wchar, name_text.wstr_ptr, name_text.wstr_len);
                MMIAPICOM_CustomGenerateReplaceStrByTextId(STR_BLUETOOTH_PAIRED_OTHER_DISC,L"%U",replace_wchar,100,&query_text);
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_CLOSE_HEADSET);
                MMIBT_OpenQueryWin(MMI_BT_QUERY_WIN_ID,&query_text,  PNULL, MMIBT_OPC_CLOSE_HEADSET);
                break;
            }
            cur_select_item = cur_select_item - list_num;
            SCI_MEMSET(&s_pin_request_info, 0, sizeof(BT_DEVICE_INFO));
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_PAIR);
            if(s_inquired_device_ptr != PNULL)
            {
                SCI_MEMCPY(&s_pin_request_info, &s_inquired_device_ptr[cur_select_item],sizeof(BT_DEVICE_INFO));
                MMIBT_SetCurBtInfo(&s_inquired_device_ptr[cur_select_item]);
            }
            BtPauseBGPlay(MMIBT_PAIR_SUSPEND);
            return_value = BT_PairDevice(&s_pin_request_info.addr);
            SCI_TRACE_LOW("[MMIBT]HandleInquiredDevListWinMsg:Web6 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x", 
				s_pin_request_info.addr.addr[0],
				s_pin_request_info.addr.addr[1],
				s_pin_request_info.addr.addr[2],
				s_pin_request_info.addr.addr[3],
				s_pin_request_info.addr.addr[4],
				s_pin_request_info.addr.addr[5]);
            if (BT_PENDING != return_value)
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
                BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
            }
            else
            {
                SetSelfPair(TRUE);
                SetIsPinCancel(FALSE);
                //pause by pair
                //need resume search suspend
                BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
                MMI_GetLabelTextByLang(STR_NOTE_WAITING, &wait_text);
                MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_PAIR_REQUEST_WAIT_WIN_ID,IMAGE_NULL,
                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleCanbeCancelWaitingWinMsg);
                MMIBT_StartOperationWin(MMIBT_PAIR_REQUEST_WAIT_WIN_ID,MMIBT_OPC_PAIR,CreatePubWinCallBack);
            }
        }
    }
        break;
    case MSG_CTL_LIST_MOVEDOWN:
    case MSG_CTL_LIST_MOVEUP:
    case MSG_CTL_LIST_MOVEBOTTOM:
    case MSG_CTL_LIST_MOVETOP:
    case MSG_CTL_LIST_PREPAGE:
    case MSG_CTL_LIST_NXTPAGE:
		index = GUILIST_GetCurItemIndex(MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
		if(index < MMIBT_MAX_DEV_ITEMS && s_inquired_device_ptr != PNULL)
		{
			SCI_MEMSET(addr_str, 0x00, sizeof(addr_str));
			sprintf((char*)addr_str, "%x", s_inquired_device_ptr[index].addr.addr[0]);
			for(i = 1; i < BT_ADDRESS_SIZE; i++)
			{
				addr_str_len = strlen((char*)addr_str);
				sprintf((char*)&addr_str[addr_str_len], " %x", s_inquired_device_ptr[index].addr.addr[i]);
			}
			MMI_Char_AutoTest_Trace((char*)MMI_BT_TEST_CHAR, strlen(MMI_BT_TEST_CHAR), (char*)addr_str, strlen((char*)addr_str));
		}
		result = MMI_RESULT_FALSE;
		break;
    case MSG_BT_SEARCH_FINISHED:
    case MSG_BT_SEARCH_CANCELED:
        if(MMIBT_GetOpertor().current_opertion == MMIBT_OPC_SEARCH){
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_INIT);
        }
#ifdef MMI_PDA_SUPPORT
        if(MMK_IsOpenWin(MMIBT_INQUIRY_OPT_WIN_ID))
        {
            MMK_CloseWin(MMIBT_INQUIRY_OPT_WIN_ID);
        }
#endif
        //GUIWIN_SetTitleTextId(win_id, TXT_BT_INQUIRED_DEV, FALSE);
#ifdef MMI_PDA_SUPPORT
#if defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle())
        {
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_RETURN, FALSE);
        }
        else
#endif
        {
            MMIBT_CreateNewSoftKey(win_id, TXT_NULL, STXT_STOP, TXT_NULL);
        }
#endif
        GUIWIN_SetTitleBackground(win_id, MMITHEME_GetWinTitleBar(), 0);

#ifndef MMI_BLUETOOTH_MINI_SUPPORT
        AnimatePlayCtrlByState(MMIBT_ANIM_STOP);
#else
        GUIANIM_SetVisible(MMIBT_ANIMATE_SEARCHING_CTRL_ID, FALSE, TRUE);
#endif
        total_num = GUILIST_GetTotalItemNum(MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
        cur_select_item = GUILIST_GetCurItemIndex(MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
        if(total_num > 0 || s_update_ui)
        {
            GUILIST_RemoveAllItems(MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
            //lulifei add for paired device begin
            MMIBT_AddPairedDeviceList(ID_PAIRED_DEVICE_INQUIRED_DEVICE);
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_INIT);
            
            for(i = 0; s_inquired_device_ptr != PNULL && i < s_inquired_device_num; i++)
            {
                MMIBT_AppendDeviceListItem(
                    &s_inquired_device_ptr[i],
                    STR_BT_STATUS_AVAILABLE_EXT01,
                    GUIITEM_STYLE_ONE_LINE_TEXT_MS,
                    TXT_NULL,
                    STR_SOFTKEY_BT_PAIR_MK,
                    STXT_RETURN,
                    MMIBT_INQUIRED_DEV_LIST_CTRL_ID
                    );
            }
            if(cur_select_item < s_inquired_device_num)
            {
                GUILIST_SetCurItemIndex(MMIBT_INQUIRED_DEV_LIST_CTRL_ID, cur_select_item);
            }
        }
        if(s_update_ui == TRUE)
        {
            s_update_ui = FALSE;
        }
        if(MMK_IsFocusWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
        {
            MMK_SendMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if(MMIBT_OPC_SEARCH == MMIBT_GetOpertor().current_opertion)
        {
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_INIT);
            //stop search
            return_value = BT_CancelSearch();
            if(BT_PENDING == return_value)
            {
                //canceling
                //OpenCancelWaitingWin();
            }
            else
            {
                MMK_SendMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID , MSG_BT_SEARCH_CANCELED, PNULL);
            }
        }
        else
        {
            //exit
            MMK_CloseWin(win_id);
        }
        BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
        break;

    case MSG_CLOSE_WINDOW:
        BT_CancelSearch();
        BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        AnimatePlayCtrlByState(MMIBT_ANIM_STOP);
        if(MMIBT_OPC_AUTO_TEST == operation_info.root_opertion)
        {
            //run next step
            MMIBT_CreateUitestWin();
        }
        if(s_inquired_device_ptr != PNULL)
        {
            SCI_FREE(s_inquired_device_ptr);
            s_inquired_device_ptr = PNULL;
        }
        if(PNULL != win_param_ptr)
        {
            if(MMIBT_OPC_OPP_SEND == win_param_ptr->root_opertion)
            {
                if(win_param_ptr->transfer_file_info.is_temp_file)
                {
                    MMIAPIFMM_DeleteFile(win_param_ptr->transfer_file_info.transfer_file_name, PNULL);
                }
            }
            SCI_FREEIF(win_param_ptr);
        }
#if defined(BLUETOOTH_SUPPORT_SPRD_BT) && !defined(WIN32)
        BT_FreeEirBuffer();
        SCI_TRACE_LOW("BT_FreeEirBuffer called!");
#endif
        break;
    case MSG_KEYDOWN_RED:
        if(MMIBT_OPC_AUTO_TEST != operation_info.root_opertion)
        {
            result=MMI_RESULT_FALSE;
        }
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Discription: Handle Inquired device list option menu msg
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleInquiryOptMenuWinMsg(
                                                 MMI_WIN_ID_T win_id,
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param
                                                 )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    uint16              node_id       = 0;
    BT_STATUS           return_value = BT_PENDING;
    uint16              cur_select_item = 0;
    MMI_STRING_T            wait_text = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        
        node_id = GUIMENU_GetCurNodeId(MMIBT_LIST_INQUIRY_MENU_CTRL_ID);
        if(node_id == MMIBT_INQUIRY_STOP_ID)
        {
            return_value = BT_CancelSearch();
            if(BT_PENDING == return_value)
            {
                //canceling
                //OpenCancelWaitingWin();
            }
            else
            {
                MMK_SendMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID , MSG_BT_SEARCH_CANCELED, PNULL);
            }
        }
        else if(node_id == MMIBT_INQUIRY_PAIRY_ID)
        {
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_PAIR);
            cur_select_item = GUILIST_GetCurItemIndex(MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
            cur_select_item = cur_select_item - list_num;
            SCI_MEMSET(&s_pin_request_info, 0, sizeof(BT_DEVICE_INFO));
            if(s_inquired_device_ptr != PNULL)
            {
                SCI_MEMCPY(&s_pin_request_info, &s_inquired_device_ptr[cur_select_item],sizeof(BT_DEVICE_INFO));
                SCI_TRACE_LOW("[MMIBT]HandleInquiryOptMenuWinMsg: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x", 
    				s_pin_request_info.addr.addr[0],
    				s_pin_request_info.addr.addr[1],
    				s_pin_request_info.addr.addr[2],
    				s_pin_request_info.addr.addr[3],
    				s_pin_request_info.addr.addr[4],
    				s_pin_request_info.addr.addr[5]);
                MMIBT_SetCurBtInfo(&s_inquired_device_ptr[cur_select_item]);
            }
            BtPauseBGPlay(MMIBT_PAIR_SUSPEND);
            return_value = BT_PairDevice(&s_pin_request_info.addr);
            if (BT_PENDING != return_value)
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
                BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
            }
            else
            {
                SetSelfPair(TRUE);
                SetIsPinCancel(FALSE);
                //pause by pair
                //need resume search suspend
                BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
                MMI_GetLabelTextByLang(STR_NOTE_WAITING, &wait_text);
                MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_PAIR_REQUEST_WAIT_WIN_ID,IMAGE_NULL,
                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleCanbeCancelWaitingWinMsg);
                MMIBT_StartOperationWin(MMIBT_PAIR_REQUEST_WAIT_WIN_ID,MMIBT_OPC_PAIR,CreatePubWinCallBack);
            }
        }
        
        MMK_CloseWin(win_id);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
/*****************************************************************************/
//  Discription: Handle blue tooth  pin input win msg
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandlePinInputWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_STRING_T        pin_value= {0};
    uint8               pin_str[MMIBT_PIN_MAX_LENGTH + 2 ] ={0};
    MMI_STRING_T        wait_text = {0};
    BT_STATUS           return_value = BT_PENDING;
    GUI_BOTH_RECT_T     edit_both_rect = {0};
    MMIBT_PIN_INPUT_PARAM_T *win_param_ptr = (MMIBT_PIN_INPUT_PARAM_T *)MMK_GetWinUserData(win_id);
    //MMI_CTRL_ID_T       input_field_ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIBT_PIN_EDITBOX_CTRL_ID );
    MMI_CTRL_ID_T       input_field_ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID );
    GUIIM_TYPE_T allow_im = GUIIM_TYPE_NONE;
    GUIIM_TYPE_T init_im = GUIIM_TYPE_NONE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
        //GUIAPICTRL_SetBothRect(MMIBT_PIN_EDITBOX_CTRL_ID, &edit_both_rect);
        
        //set left softkey
        //GUIEDIT_SetSoftkey(input_field_ctrl_id,0,MMIBT_PIN_MIN_LENGTH,TXT_NULL,STXT_SOFTKEY_OK_MK,PNULL);
        allow_im =  GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;/*lint !e655*/
//        init_im = GUIIM_TYPE_ABC;
        init_im = GUIIM_TYPE_DIGITAL;
        GUIEDIT_SetIm(input_field_ctrl_id,allow_im,init_im);
        MMIPUB_SetInputDialogEditorType(win_id, DIALOG_PASSWORD_EDITOR);
        GUIEDIT_SetTextMaxLen (input_field_ctrl_id, MMIBT_PIN_MAX_LENGTH,MMIBT_PIN_MAX_LENGTH);
        MMIPUB_SetDialogBgLayerAlpha(win_id, 127);
        MMK_SetAtvCtrl(win_id,input_field_ctrl_id);
        break;
        
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;

            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                GUIEDIT_GetString(input_field_ctrl_id,&pin_value);

                if (PAIR_CODE_LEN_IS_IN_RANGE(pin_value.wstr_len))
                {
                    win_param_ptr->callbackfunc(win_id,pin_value.wstr_ptr,pin_value.wstr_len);
                }
                else
                {
                    MMIPUB_OpenAlertFailWin(TXT_LENGTH_ERROR);
                    GUIEDIT_ClearAllStr(input_field_ctrl_id);
                }
                break;
            case  MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_CloseWin(win_id);
                SetIsPinCancel(TRUE);
                BT_CancelPair(&win_param_ptr->pin_request_device_info.addr);
                BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
                break;
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif//TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_CTL_MIDSK:
        // get the value about PIN
        GUIEDIT_GetString(input_field_ctrl_id,&pin_value);

        if (PAIR_CODE_LEN_IS_IN_RANGE(pin_value.wstr_len))
        {
            win_param_ptr->callbackfunc(win_id,pin_value.wstr_ptr,pin_value.wstr_len);
        }
        else
        {
            //MMIPUB_OpenAlertFailWin(TXT_LENGTH_ERROR);
            GUIEDIT_ClearAllStr(input_field_ctrl_id);
        }
        break;
    case MSG_FULL_PAINT:
        GUIEDIT_GetString(input_field_ctrl_id,&pin_value);
        if(0 == pin_value.wstr_len) {
            GUIWIN_SetSoftkeyBtnState(win_id,1,TRUE,FALSE);
            GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_RETURN,2,TRUE);
        } else {
            GUIWIN_SetSoftkeyBtnState(win_id,1,FALSE,FALSE);
            GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_DELETE,2,TRUE);
        }
        GUIWIN_SeSoftkeytButtonTextId(win_id,PNULL,0,TRUE);
	   /*allow_im = GUIIM_TYPE_ENGLISH |GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;/*lint !e655
          init_im = GUIIM_TYPE_ABC;
          GUIEDIT_SetIm(input_field_ctrl_id,allow_im,init_im);*/
	   break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        SetIsPinCancel(TRUE);
        if(0 != MMIAPICOM_Wstrnlen((wchar*)win_param_ptr->pin_request_device_info.addr.addr, BT_ADDRESS_SIZE/sizeof(wchar)))
        {
            BT_CancelPair(&win_param_ptr->pin_request_device_info.addr);
        }
        BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
        break;

    case MSG_CLOSE_WINDOW:
        SCI_FREEIF(win_param_ptr);
        break;
    default:
        result = MMIPUB_HandleDialogInputFieldWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Discription: Handle blue tooth  numeric passkey check
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleNumericPasskeyWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_STRING_T        wait_text = {0};
    BT_STATUS           return_value = BT_PENDING;
    GUI_BOTH_RECT_T     both_rect = {0};

    SCI_TRACE_LOW("HandleNumericPasskeyWinMsg: msg_id=0x%x", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T    name_text = {0};
            wchar    replace_wchar[100] = {0};
            wchar    test[100] = {0};
            MMI_STRING_T    query_text = {0};
            name_text.wstr_ptr = s_pin_request_info_ext.name;
            name_text.wstr_len = MMIAPICOM_Wstrlen(s_pin_request_info_ext.name);
            MMIAPICOM_Wstrncpy(replace_wchar, name_text.wstr_ptr, name_text.wstr_len);
            query_text.wstr_ptr = test;
            MMIAPICOM_CustomGenerateReplaceStrByTextId(STR_BT_ACCEPT_PAIR_REQUEST_EXT01,L"%U",replace_wchar,100,&query_text);
            SCI_TRACE_LOW("HandleNumericPasskeyWinMsg: open: name_wstr_len=%d, wstr_len=%d", name_text.wstr_len, query_text.wstr_len);
            GUITITLE_SetText(win_id, MMITHEME_GetTitleCtrlId(), query_text.wstr_ptr, query_text.wstr_len, TRUE);
            both_rect = MMITHEME_GetFullScreenBothRect();
            MMK_SetBothRect(win_id, &both_rect);
            MMIDEFAULT_AllowOpenKeylocWin(FALSE);
            Display_NumericPasskeyInfo();
        }
        break;

    case MSG_FULL_PAINT:
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                s_is_pin_cancel = FALSE;
                return_value = BT_InputPairPin(&s_pin_request_info_ext.addr,(uint8*)s_pin_request_info_ext.ssp_numric_key,strlen(s_pin_request_info_ext.ssp_numric_key));
                if (BT_PENDING == return_value)
                {
                    MMI_GetLabelTextByLang(STR_BT_WAIT_PAIRING_EXT01, &wait_text);
                    MMIPUB_OpenWaitWin(1,&wait_text, PNULL ,PNULL,MMIBT_PAIR_WAIT_WIN_ID,IMAGE_NULL,
                        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleCanbeCancelWaitingWinMsg);

                }
                else
		        {
                     MMK_CloseWin(win_id);
		        }
                break;
            case  MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_CloseWin(win_id);
                s_is_pin_cancel = TRUE;
                if(0 != MMIAPICOM_Wstrnlen((wchar*)s_pin_request_info_ext.addr.addr, BT_ADDRESS_SIZE/sizeof(wchar)))
                {
                    BT_CancelPair(&s_pin_request_info_ext.addr);
                }
                BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
                break;
            default:
                break;
            }
        }
        break;
#else
    case MSG_CTL_PENOK:
#endif
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        s_is_pin_cancel = FALSE;
        return_value = BT_InputPairPin(&s_pin_request_info_ext.addr,(uint8*)s_pin_request_info_ext.ssp_numric_key,strlen(s_pin_request_info_ext.ssp_numric_key));
        if (BT_PENDING == return_value)
        {
            MMI_GetLabelTextByLang(STR_BT_WAIT_PAIRING_EXT01, &wait_text);
            MMIPUB_OpenWaitWin(1,&wait_text, PNULL ,PNULL,MMIBT_PAIR_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);

        }
        else
		{
             MMK_CloseWin(win_id);
		}
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        s_is_pin_cancel = TRUE;
        if(0 != MMIAPICOM_Wstrnlen((wchar*)s_pin_request_info_ext.addr.addr, BT_ADDRESS_SIZE/sizeof(wchar)))
        {
            BT_CancelPair(&s_pin_request_info_ext.addr);
        }
        BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
        break;

    case MSG_APP_RED:
        s_is_pin_cancel = TRUE;
        if(0 != MMIAPICOM_Wstrnlen((wchar*)s_pin_request_info_ext.addr.addr, BT_ADDRESS_SIZE/sizeof(wchar)))
        {
            BT_CancelPair(&s_pin_request_info_ext.addr);
        }
        BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
        result = MMI_RESULT_FALSE;
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Discription: when paired device successful should modify inquired device info
//  Global resource dependence: None
//  Author: kelly.li
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void ModifyInquiredDeviceInfo(uint16  delete_device_index)
{
    int32       i = 0;

    for (i = delete_device_index; s_inquired_device_ptr != PNULL && i < (s_inquired_device_num - 1); i ++)
    {
        SCI_MEMSET(&s_inquired_device_ptr[i], 0 , sizeof(BT_DEVICE_INFO));

        //get device name info from message body
        MMIAPICOM_Wstrncpy(s_inquired_device_ptr[i].name ,
            s_inquired_device_ptr[i + 1].name,
            MMIAPICOM_Wstrlen((wchar*)s_inquired_device_ptr[i + 1].name));

        //get device addr info from message body
        SCI_MEMCPY(s_inquired_device_ptr[i].addr.addr,
            s_inquired_device_ptr[i + 1].addr.addr,
            BT_ADDRESS_SIZE);

        //get new inquired device class info
        s_inquired_device_ptr[i].dev_class = s_inquired_device_ptr[i + 1].dev_class;
    }
}
/*****************************************************************************/
//  Discription: get inquired device name info
//  Global resource dependence: None
//  Author: kelly.li
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetInqDevName(
                                BT_MSG_T *msg_body_ptr
                                )
{
    BT_DEVICE_INFO      dev_info   = *((BT_DEVICE_INFO*)msg_body_ptr->body_ptr);
    MMI_CTRL_ID_T       ctrl_id = 0;
    MMI_WIN_ID_T        win_id = 0;
	BT_DEVICE_INFO 		paired_dev_info = {0};
    char temp[256] = {0};
    //lulifei add the paired device
    uint16 i;
    BOOLEAN isSameDev = FALSE;

    //turn on the back light
    if(MMIAPICC_IsInState(CC_MT_CONNECTING_STATE) || MMIAPICC_IsInState(CC_MO_CONNECTING_STATE))
    {
        //SCI_TRACE_LOW:"MMIBT_GetInqDevName in calling or alarming !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5939_112_2_18_2_8_12_266,(uint8*)"");
        MMK_SendMsg(MMIBT_SEARCH_DEVICE_PUBWIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID);
        BT_CancelSearch();
        return;
    }
    if(BT_SUCCESS != msg_body_ptr->status)
    {
        //SCI_TRACE_LOW:"MMIBT_GetInqDevName status %d !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5947_112_2_18_2_8_12_267,(uint8*)"d", msg_body_ptr->status);
        return;
    }
    if(IsSearchingDeviceExist(&(dev_info.addr)) || (s_inquired_device_num >= MMIBT_MAX_DEV_ITEMS))
    {
        //SCI_TRACE_LOW:"MMIBT_GetInqDevName alerady exist s_inquired_device_num %d !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5952_112_2_18_2_8_12_268,(uint8*)"d", msg_body_ptr->status);
        return;
    }
    MMIDEFAULT_SetBackLight(TRUE);
    if(0 == s_inquired_device_num)
    {
        //need open search list window
        if(s_inquired_device_ptr != PNULL)
        {
            SCI_MEMSET(s_inquired_device_ptr, 0x00, (MMIBT_MAX_DEV_ITEMS *sizeof(BT_DEVICE_INFO)));
        }
        OpenInquiredDevListWin();
        //lulifei add for paired device begin
        MMIBT_AddPairedDeviceList(ID_PAIRED_DEVICE_SEARCH_DEVICE);
        MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SEARCH);
        //MMIBT_SetCurrentSevice(BT_SERVICE_ALL);
        MMIBT_StopOperationWin(MMIBT_OPC_SEARCH);
    }
    
    //add inquired device info to list
    if (MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID)
        && (MMIBT_OPC_SEARCH == MMIBT_GetOpertor().current_opertion))
    {
        ctrl_id = MMIBT_INQUIRED_DEV_LIST_CTRL_ID;
        win_id = MMIBT_INQUIRED_DEV_LIST_WIN_ID;
        //get device name info from message body
        MMIAPICOM_WstrToStr(dev_info.name, temp);
        if(MMIAPICOM_Wstrlen((wchar*)dev_info.name) > BT_DEVICE_NAME_SIZE
            ||0 == MMIAPICOM_Wstrlen((wchar*)dev_info.name))
        {
            return;
        }
        //lulifei add this for compare the paired device and searched device address
        for(i=0 ; i < list_num;i++ ){
            SCI_MEMSET(&paired_dev_info, 0x00, sizeof(paired_dev_info));
            BT_GetPairedDeviceInfo(BT_SERVICE_ALL, paired_device_index[i], &paired_dev_info);
            if(MMIAPICOM_Stricmp(&paired_dev_info.addr.addr,&dev_info.addr.addr)==0){
                isSameDev = TRUE;
            }
        }
        if (!isSameDev)
        {
            uint16    dev_type = 0;
            uint8         bt_profile_list = 0;
            
            EFS_NvitemRead(APP_NV_BT_PROFILE_LIST, sizeof(uint8), &bt_profile_list);
            BT_GetDeviceType(&dev_info, &dev_type);
#ifdef WIN32
            bt_profile_list = 1;
#endif
            if(1 == bt_profile_list)
            {
                // do not show the computer device
                if(dev_type & BT_DEVICE_CLASS_COMPUTER)
                {
                    SCI_TRACE_LOW("MMIBT_GetInqDevName skip BT_DEVICE_CLASS_COMPUTER");
                    return;
                }
                // do not show the phone device
#if 0
                if(dev_type & BT_DEVICE_CLASS_PHONE)
                {
                    SCI_TRACE_LOW("MMIBT_GetInqDevName skip BT_DEVICE_CLASS_PHONE");
                    return;
                }
#endif
            }

            if(s_inquired_device_ptr != PNULL )
            {
                MMIAPICOM_Wstrncpy(s_inquired_device_ptr[s_inquired_device_num].name, dev_info.name, MMIAPICOM_Wstrlen( dev_info.name));
                //get device addr info from message body
                SCI_MEMCPY(
                    s_inquired_device_ptr[s_inquired_device_num].addr.addr,
                    dev_info.addr.addr,
                    BT_ADDRESS_SIZE
                    );
	            //get new inquired device class info
	            s_inquired_device_ptr[s_inquired_device_num].dev_class = dev_info.dev_class;
	            s_inquired_device_num ++;

	        }
	        MMIBT_AppendDeviceListItem(
	            &dev_info,
	            STR_BT_STATUS_AVAILABLE_EXT01,
	            GUIITEM_STYLE_ONE_LINE_TEXT_MS,
	            TXT_NULL,
	            TXT_NULL,
	            STXT_STOP,
	            ctrl_id
	            );
	        if (MMK_IsFocusWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
	        {
	            //only search list in focus need paint
	            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, PNULL);
	            GUILIST_ValidCurItemPos(ctrl_id);
	        }
        }
    }
	else
	{
		BT_CancelSearch();
		BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
	}

}
/*****************************************************************************/
//  Discription:  check is searched device already be searched
//  Global resource dependence: None
//  Author:
//  Note :
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN IsSearchingDeviceExist(BT_ADDRESS *bt_addr_ptr)
{
	BOOLEAN is_exist = FALSE;
	uint32  i = 0;

    if(PNULL == bt_addr_ptr || (0 == s_inquired_device_num) || s_inquired_device_ptr == PNULL)
	{
		return FALSE;
	}

	for(i = 0; i < s_inquired_device_num; i++)
	{
		if(!memcmp(bt_addr_ptr, &(s_inquired_device_ptr[i].addr), sizeof(BT_ADDRESS)))
		{
            is_exist = TRUE;
			break;
		}
	}

	return is_exist;

}
/*****************************************************************************/
//  Discription: get device pair complete confirm (success or failure)
//  Global resource dependence: None
//  Author: kelly.li
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetPairComCnf(BT_MSG_T *msg_body_ptr)
{
    uint16                  cur_pos = 0;
    uint16                  device_index = 0;
    uint16                  dev_type = 0;
    MMI_STRING_T            name_text = {0};
    MMI_WIN_ID_T            alert_id = MMI_BT_REMOTE_ALERT_WIN_ID;
    uint32                  current_service = 0;
    BT_DEVICE_INFO * pbtinfo = (BT_DEVICE_INFO *)msg_body_ptr->body_ptr;
    BT_DEVICE_INFO         current_bt_device = MMIBT_GetCurBtInfo();
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();
    //lulifei add the paired device list item
    uint16	cur_real_pos = 0;
    uint16 i;
    MMICOM_BANNER_INFO_T banner_info = {0};
    wchar    replace_wchar[50] = {0};
    MMI_STRING_T    result1_string = {0};
    MMI_STRING_T    result2_string = {0};
    MMI_STRING_T    device_name = {0};
    wchar text_temp[64] = {0};
    wchar old_str[] = {0x005c, 0x006e,0x0000};
    wchar old_str1[] = {0x0025, 0x0055,0x0000};
    wchar newstr[] = {0x000a,0x0000};
    wchar test1[64] = {0};
    wchar test_exa[3] = {0x002e,0x002e,0x002e};
    MMI_STRING_T            body_text = {0};
    //when searching, then pair a device, the search will stop and return this msg
    BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);


    //SCI_TRACE_LOW:"MMIBT_GetPairComCnf msg_body_ptr->status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6084_112_2_18_2_8_12_269,(uint8*)"d",msg_body_ptr->status);
    if (BT_SUCCESS == msg_body_ptr->status)
    {
        //pair device successful
        BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
        BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
        if(IsSelfPair())
        {
            if (MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
            {
                MMIDEFAULT_SetBackLight(TRUE);
                if(MMIBT_OPC_MAIN != operation_info.root_opertion && (!memcmp(&s_pin_request_info.addr, &current_bt_device.addr, sizeof(BT_ADDRESS))))
                {
                    SCI_Sleep(MMI_3SECONDS);
                    DoOperation(operation_info.root_opertion, FALSE);
                }
                cur_pos = GUILIST_GetCurItemIndex( MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
                //lulifei add for the list item
                //DoOperation(operation_info.root_opertion, FALSE);
                cur_real_pos = cur_pos - list_num;
                SCI_TRACE_LOW("[MMIBT] MMIBT_GetPairComCnf: cur_pos=%d, list_num=%d, cur_real_pos=%d, s_inquired_device_num=%d", cur_pos, list_num, cur_real_pos, s_inquired_device_num);
                if((s_inquired_device_ptr == PNULL) || (cur_real_pos >= s_inquired_device_num))//bug1077512
                {
                    SCI_TRACE_LOW("[MMIBT] MMIBT_GetPairComCnf: s_inquired_device_ptr is PNULL or cur_real_pos is invalid");
                }
                else if(!memcmp(s_pin_request_info.addr.addr, s_inquired_device_ptr[cur_real_pos].addr.addr, sizeof(s_pin_request_info.addr.addr)))
                {
                    //配对成功的设备在当前的搜索列表中
                    //如果不是设置窗口或搜索到的设备只有一项，则关闭搜索设备列表窗口
                    /*lulifei remove to keep the UI
                    if (MMIBT_OPC_MAIN != operation_info.root_opertion || 1 == s_inquired_device_num)
                    {
                    MMK_CloseWin( MMIBT_INQUIRED_DEV_LIST_WIN_ID );
                    }
                    else
                    {*/
                    //将配对成功的设备从搜索到的设备列表中移除，并更新设备列表
                    device_name.wstr_ptr = s_pin_request_info.name;
                    device_name.wstr_len = MMIAPICOM_Wstrlen(s_pin_request_info.name);
                    if(device_name.wstr_len >50)
                    {
                        MMIAPICOM_Wstrncpy(test1,device_name.wstr_ptr,50);
                        MMIAPICOM_Wstrcat(test1,test_exa);
                    }
                    else
                    {
                        MMIAPICOM_Wstrncpy(test1,device_name.wstr_ptr,device_name.wstr_len);
                    }
                    MMI_GetLabelTextByLang(STR_BT_CONF_CONNECTED, &result1_string);
                    MMIAPICOM_Wstrncpy(text_temp, result1_string.wstr_ptr,result1_string.wstr_len);
                    MMIAPICOM_WstrReplace(text_temp, sizeof(text_temp), old_str1, test1);
                    
                    //MMIAPICOM_WstrReplace(text_temp, sizeof(text_temp), old_str, newstr);
                    MMIAPICOM_WstrReplace(text_temp, sizeof(text_temp), old_str, L" ");
                    //result2_string.wstr_ptr = text_temp;
                    //result2_string.wstr_len = MMIAPICOM_Wstrlen(result2_string.wstr_ptr);
                    //MMIAPICOM_Wstrcat(result2_string.wstr_ptr, device_name.wstr_ptr);
                    //MMIAPICOM_Wstrcat(result2_string.wstr_ptr, L" ");
                    banner_info.text1_str.wstr_ptr = text_temp;
                    banner_info.text1_str.wstr_len= MMIAPICOM_Wstrlen(banner_info.text1_str.wstr_ptr );
                    
                    banner_info.banner_type = MMI_BANNER_STR_TEXT;
                    //banner_info.start_point = start_point;
                    MMIAPICOM_OpenBannerWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID, banner_info);
                    //lulifei add the paired device
                    GUILIST_RemoveItem( MMIBT_INQUIRED_DEV_LIST_CTRL_ID, cur_pos);
                    device_index = cur_real_pos;
                    ModifyInquiredDeviceInfo(device_index);
                    s_update_ui = TRUE;
                    s_inquired_device_num --;
                    if(MMK_IsFocusWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
                    {
                        MMK_PostMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID,MSG_FULL_PAINT,PNULL,PNULL);
                    }
                    //}
                }
            }
        }

        if(MMIBT_OPC_MAIN == operation_info.root_opertion)
		{
            current_service = BT_GetPairedDeviceService(&pbtinfo->addr);
            if((BT_SERVICE_FTP|BT_SERVICE_OPP) & current_service)
            {
                MMI_STRING_T display_str = {0};

                MMI_GetLabelTextByLang(STR_BT_SETT_CONN_TYPE, &display_str);
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_DEVICE_AUTO_CONNECT_HINT);
                MMIBT_OpenQueryWin(MMI_BT_QUERY_WIN_ID, &display_str, PNULL, MMIBT_OPC_DEVICE_AUTO_CONNECT_HINT);
            }
		}

        //if headset need open this headset
        BT_GetDeviceType(&s_pin_request_info, &dev_type);
        if(dev_type & BT_DEVICE_CLASS_AUDIO)
        {
            //headset need open it directly
            if(HeadsetOpenIsPermit(&(s_pin_request_info.addr)))
            {
                 MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_OPEN_HEADSET);
                 OpenHeadset(&s_pin_request_info.addr);
            }
        }
#ifdef MMI_BT_PBAP_SUPPORT
        else if(dev_type & BT_DEVICE_CLASS_PHONE)
        {
            //headset need open it directly
            if(HandsetGatewayOpenIsPermit(&(s_pin_request_info.addr)))
            {
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_OPEN_HANDSET_GATEWAY);
                OpenHandsetGateway(&s_pin_request_info.addr);
                //DoOperation(MMIBT_OPC_OPEN_HEADSET, FALSE);
//                MMIAPIBT_DirectConnectPbapService();
            }
        }
#endif
        if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
        {
            //Update Device list
            MMK_PostMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, PNULL, PNULL);
        }
        MMK_CloseWin(MMI_BT_PAIR_QUERY_WIN_ID);
        //close input pin win
        MMK_CloseWin(MMIBT_PIN_INPUT_WIN_ID);
    }
    else
    {
        //close pair again query window
        MMK_CloseWin(MMI_BT_PAIR_QUERY_WIN_ID);
        if (!IsPinCancel())
        {
            if (IsSelfPair())
            {
                MMI_STRING_T query_text = {0};
                
                MMIDEFAULT_SetBackLight(TRUE);
                MMI_GetLabelTextByLang(STR_BT_DEVICE_NO_REPOND_EXT01, &query_text);
                //query try agian
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_PAIR);
                BtPauseBGPlay(MMIBT_PAIR_SUSPEND);
                BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
                MMIBT_OpenQueryWin(MMIBT_RETRY_WIN_ID, &query_text, PNULL, MMIBT_OPC_PAIR);
            }
            else
            {
                alert_id = MMI_BT_ALERT_WIN_ID;
                MMIDEFAULT_SetBackLight(TRUE);
                BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
                BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
                MMI_GetLabelTextByLang(STR_BT_TITLE_CONN_FAILED_EXT01, &name_text);
                MMI_GetLabelTextByLang(STR_BT_FAILED_OUT_RANGE_EXT01, &body_text);
                MMIPUB_OpenAlertWinByTextPtr(PNULL, &name_text, &body_text,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                //MMIPUB_OpenConfirmationDialogByTextPtr(PNULL, &name_text, PNULL,IMAGE_NULL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
        }
        else
        {
            BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
            BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
        }
        SetIsPinCancel(FALSE);
        if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
        {
            //Update Device list
            MMK_PostMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, PNULL, PNULL);
        }
        //close input pin win
        MMK_CloseWin(MMIBT_PIN_INPUT_WIN_ID);
    }
    MMIBT_StopOperationWin(MMIBT_OPC_PAIR);
    MMK_CloseWin(MMIBT_PAIR_WAIT_WIN_ID);
    MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
	MMK_CloseWin(MMIBT_NUMERIC_PASSKEY_WIN_ID);
}

/*****************************************************************************/
//  Discription: get earphone device pair complete confirm (success or failure)
//  Global resource dependence: None
//  Author:
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetPairComIND(BT_MSG_T *msg_body_ptr)
{
    MMI_STRING_T            name_text = {0};
    MMI_STRING_T            body_text = {0};
    MMI_WIN_ID_T            alert_id = MMI_BT_REMOTE_ALERT_WIN_ID;
    BT_DEVICE_INFO * pbtinfo = (BT_DEVICE_INFO *)msg_body_ptr->body_ptr;

    //when searching, then pair a device, the search will stop and return this msg
    BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);

    //SCI_TRACE_LOW:"MMIBT_GetPairComIND msg_body_ptr->status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11013_112_2_18_2_8_22_353,(uint8*)"d",msg_body_ptr->status);
    if (BT_SUCCESS == msg_body_ptr->status)
    {
        //pair device successful
        BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
        BtResumeBGPlay(MMIBT_PAIR_SUSPEND);

        SCI_MEMCPY(&s_pin_request_info, pbtinfo,sizeof(BT_DEVICE_INFO));
        SCI_TRACE_LOW("[MMIBT]MMIBT_GetPairComIND: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x", 
			s_pin_request_info.addr.addr[0],
			s_pin_request_info.addr.addr[1],
			s_pin_request_info.addr.addr[2],
			s_pin_request_info.addr.addr[3],
			s_pin_request_info.addr.addr[4],
			s_pin_request_info.addr.addr[5]);

        s_headset_openwait_timer = MMK_CreateTimerCallback(BT_HEADSET_OPENWAIT_TIMER, MMIBT_OpenWaitTimerOut, 0, FALSE);
        SCI_TRACE_LOW("[MMIBT] MMIBT_GetPairComIND, copy, timer_id = %d", s_headset_openwait_timer);

        if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
        {
            //Update Device list
            MMK_PostMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, PNULL, PNULL);
        }
        //close pair again query window
        if(!MMK_IsOpenWin( MMIBT_INCOMING_FILE_WIN_ID )
#ifdef MMI_BT_PBAP_SUPPORT
            && !MMK_IsOpenWin( MMIBT_PBAP_CONNECT_WAIT_WIN_ID )
#endif
            )
        {
            //only when incoming win not opened,can prompt success
            //not local pair, need promt success
			MMI_GetLabelTextByLang(STR_BT_STATUS_PAIRED_EXT01, &name_text);
			MMIDEFAULT_SetBackLight(TRUE);
            MMIPUB_OpenAlertWinByTextPtr(PNULL,&name_text, PNULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            //MMIPUB_OpenConfirmationDialogByTextPtr(PNULL,&name_text, PNULL,IMAGE_NULL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        //MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(MMI_BT_PAIR_QUERY_WIN_ID);
        //close input pin win
        MMK_CloseWin(MMIBT_PIN_INPUT_WIN_ID);
    }
    else
    {
        //close pair again query window
        MMK_CloseWin(MMI_BT_PAIR_QUERY_WIN_ID);
        if (!MMIBT_IsPinCancel())
        {
            alert_id = MMI_BT_ALERT_WIN_ID;
            MMIDEFAULT_SetBackLight(TRUE);
            BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
            BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
            MMI_GetLabelTextByLang(STR_BT_TITLE_CONN_FAILED_EXT01, &name_text);
            MMI_GetLabelTextByLang(STR_BT_FAILED_OUT_RANGE_EXT01, &body_text);
            MMIPUB_OpenAlertWinByTextPtr(PNULL, &name_text, &body_text,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            //MMIPUB_OpenConfirmationDialogByTextPtr(PNULL, &name_text, PNULL,IMAGE_NULL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        else
        {
            BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
            BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
        }
        MMIBT_SetPinCancel(FALSE);
        //close input pin win
        MMK_CloseWin(MMIBT_PIN_INPUT_WIN_ID);
        //user do not refuse pair, reminder the result
    }
    MMIBT_StopOperationWin(MMIBT_OPC_PAIR);
    MMK_CloseWin(MMIBT_PAIR_WAIT_WIN_ID);
    MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
    MMK_CloseWin(MMIBT_NUMERIC_PASSKEY_WIN_ID);
}

/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC void OpenSearchDeviceWaitWin(
                                   uint32 service_type
                                   )
{
    MMI_STRING_T            wait_text = {0};
    BT_STATUS               return_value = BT_SUCCESS;
    uint8  paired_device_number = 0;
    
    return_value = BT_CancelSearch();
    MMI_GetLabelTextByLang(TXT_COMMON_SEARCHING, &wait_text);
    s_inquired_device_num = 0;
    paired_device_number =  BT_GetPairedDeviceCount(BT_SERVICE_ALL) ;

    MMIPUB_OpenWaitWin(1,PNULL,PNULL,PNULL,MMIBT_SEARCH_DEVICE_PUBWIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleCanbeCancelWaitingWinMsg);

    MMIBT_StartOperationWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID,MMIBT_OPC_SEARCH,CreatePubWinCallBack);

    if( paired_device_number > 0)
    {
        OpenInquiredDevListWin();
        MMIBT_AddPairedDeviceList(ID_PAIRED_DEVICE_SEARCH_DEVICE);
        MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SEARCH);
        //MMIBT_SetCurrentSevice(BT_SERVICE_ALL);
        MMIBT_StopOperationWin(MMIBT_OPC_SEARCH);
    }

    if(BT_PENDING != return_value)//already canceled, search directly
    {
        return_value = BT_SearchDevice(service_type);

        SCI_TRACE_LOW("[MMIBT] OpenSearchDeviceWaitWin: return_value=%d", return_value);
        if(BT_PENDING != return_value)
        {
            MMK_SendMsg(MMIBT_SEARCH_DEVICE_PUBWIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
            MMK_CloseWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID);
            BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
            if(BT_ERROR == return_value)//bug1137998
            {
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_INIT);
                //stop search
                return_value = BT_CancelSearch();
            }
        }
    }
}
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC void SetSelfPair(BOOLEAN  is_self_pair)
{
    s_bt_is_self_pair = is_self_pair;
}
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL BOOLEAN IsSelfPair(void)
{
    return s_bt_is_self_pair ;
}
/*****************************************************************************/
//  Discription: get the input pin code request
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
PUBLIC void   MMIBT_GetInputPinReq(BT_MSG_T* msg_body_ptr)
{
    BT_DEVICE_INFO  device_info = {0};
    MMI_STRING_T    query_text = {0};
	MMI_STRING_T    name_text = {0};
	MMI_WIN_ID_T    alert_id = MMI_BT_REMOTE_ALERT_WIN_ID;
	wchar    replace_wchar[100] = {0};
	wchar    test[100] = {0};

    if ((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        //SCI_TRACE_LOW:"MMIBT_GetInputPinReq PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5454_112_2_18_2_8_11_258,(uint8*)"");
        return ;
    }

	device_info = *(( BT_DEVICE_INFO *) msg_body_ptr->body_ptr);
#if 0
    if(MMIAPICC_IsInState(CC_MT_CONNECTING_STATE) || MMIAPICC_IsInState(CC_MO_CONNECTING_STATE))
    {
        //calling, not deal with this request
		SetIsPinCancel(TRUE);
		BT_CancelPair(&device_info.addr);
        return ;
    }
#endif
	if(MMK_IsOpenWin(MMI_BT_PAIR_QUERY_WIN_ID) &&
		(MMIBT_OPC_INPUT_PIN == MMIBT_GetInterruptOper()
	     ||MMIBT_OPC_PAIR == MMIBT_GetInterruptOper())
	 )
	{
		//SCI_TRACE_LOW:"MMIBT_GetInputPinReq in pairing !!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5470_112_2_18_2_8_11_259,(uint8*)"");
              if(0 != MMIAPICOM_Wstrnlen((wchar*)device_info.addr.addr, BT_ADDRESS_SIZE/sizeof(wchar)))
              {
		    BT_CancelPair(&device_info.addr);
		}
		return;
	}
	if(MMK_IsOpenWin(MMIBT_RETRY_WIN_ID) &&
		(MMIBT_OPC_INPUT_PIN == MMIBT_GetInterruptOper()
	     ||MMIBT_OPC_PAIR == MMIBT_GetInterruptOper())
	 )
	{
		//SCI_TRACE_LOW:"MMIBT_GetInputPinReq in repairing !!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5479_112_2_18_2_8_11_260,(uint8*)"");
              if(0 != MMIAPICOM_Wstrnlen((wchar*)device_info.addr.addr, BT_ADDRESS_SIZE/sizeof(wchar)))
              {
		    BT_CancelPair(&device_info.addr);
              }
		return;
	}

	if(MMK_IsOpenWin(MMIENG_BT_CANCEL_WAIT_WIN_ID))
	{
		//cancelling, need not response this message
		//SCI_TRACE_LOW:"MMIBT_GetInputPinReq in Cacelling !!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5487_112_2_18_2_8_11_261,(uint8*)"");
		return;
	}

    MMIDEFAULT_SetBackLight(TRUE);
    //store the request pair device address
    SCI_MEMSET(&s_pin_request_info, 0, sizeof(BT_DEVICE_INFO));
    SCI_MEMCPY(&s_pin_request_info, &device_info,sizeof(BT_DEVICE_INFO));
    SCI_TRACE_LOW("[MMIBT]MMIBT_GetInputPinReq: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x", 
		s_pin_request_info.addr.addr[0],
		s_pin_request_info.addr.addr[1],
		s_pin_request_info.addr.addr[2],
		s_pin_request_info.addr.addr[3],
		s_pin_request_info.addr.addr[4],
		s_pin_request_info.addr.addr[5]);

    //有配对请求等待窗口说明是本机发起的配对
    if(MMK_IsOpenWin(MMIBT_PAIR_REQUEST_WAIT_WIN_ID))
    {
        MMIBT_StopOperationWin(MMIBT_OPC_PAIR);
        MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_INPUT_PIN);
        BtPauseBGPlay(MMIBT_INPUTPIN_SUSPEND);
        BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
		if(BT_SUCCESS == msg_body_ptr->status)
		{
            MMIBT_CreatePinInputWin(&s_pin_request_info);
		}
		else
		{
			MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, PNULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
		}
    }
    else //??????
    {
        MMIBT_SetSelfPair(FALSE);
        if(MMIBT_IsPhoneReady())
        {
            //MMI_GetLabelTextByLang(STR_BT_ACCEPT_PAIR_REQUEST_EXT01, &query_text);
            name_text.wstr_ptr = s_pin_request_info.name;
            name_text.wstr_len = MMIAPICOM_Wstrlen(s_pin_request_info.name);
            MMIAPICOM_Wstrncpy(replace_wchar, name_text.wstr_ptr, name_text.wstr_len);
            query_text.wstr_ptr = test;
            MMIAPICOM_CustomGenerateReplaceStrByTextId(STR_BT_ACCEPT_PAIR_REQUEST_EXT01,L"%U",replace_wchar,100,&query_text);
            MMIBT_OpenQueryWin(MMI_BT_PAIR_QUERY_WIN_ID, &query_text, PNULL, MMIBT_OPC_INPUT_PIN);
        }
        else
        {
            SetIsPinCancel(TRUE);
            if(0 != MMIAPICOM_Wstrnlen((wchar*)s_pin_request_info.addr.addr, BT_ADDRESS_SIZE/sizeof(wchar)))
            {
                BT_CancelPair(&s_pin_request_info.addr);
            }
        }
    }
}

/*****************************************************************************/
//  Discription: get the input pin code request
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
PUBLIC void   MMIBT_GetNumericPasskeyReq(BT_MSG_T* msg_body_ptr)
{
    BT_DEVICE_INFO_EXT device_info = {0};
    MMI_STRING_T    query_text = {0};
	MMI_STRING_T    name_text = {0};
    //wchar           text_info[MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH] = {0};
    MMI_WIN_ID_T    alert_id = MMI_BT_REMOTE_ALERT_WIN_ID;
    uint8           bt_spp_numric_key[7] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    wchar    replace_wchar[100] = {0};
    wchar    test[100] = {0};


    if ((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        //SCI_TRACE_LOW:"MMIBT_GetInputPinReq PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5454_112_2_18_2_8_11_258,(uint8*)"");
        return ;
    }

	device_info = *(( BT_DEVICE_INFO_EXT *) msg_body_ptr->body_ptr);
#if 0
    if(MMIAPICC_IsInState(CC_MT_CONNECTING_STATE) || MMIAPICC_IsInState(CC_MO_CONNECTING_STATE))
    {
        //calling, not deal with this request
		s_is_pin_cancel = TRUE;
		BT_CancelPair(&device_info.addr);
        return ;
    }
#endif
	if(MMK_IsOpenWin(MMI_BT_PAIR_QUERY_WIN_ID) &&
		(MMIBT_OPC_INPUT_PIN == g_interrupt_opertor
	     ||MMIBT_OPC_PAIR == g_interrupt_opertor)
	 )
	{
		//SCI_TRACE_LOW:"MMIBT_GetInputPinReq in pairing !!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5470_112_2_18_2_8_11_259,(uint8*)"");
              if(0 != MMIAPICOM_Wstrnlen((wchar*)device_info.addr.addr, BT_ADDRESS_SIZE/sizeof(wchar)))
              {
		    BT_CancelPair(&device_info.addr);
              }
		return;
	}
	if(MMK_IsOpenWin(MMIBT_RETRY_WIN_ID) &&
		(MMIBT_OPC_INPUT_PIN == g_interrupt_opertor
	     ||MMIBT_OPC_PAIR == g_interrupt_opertor)
	 )
	{
		//SCI_TRACE_LOW:"MMIBT_GetInputPinReq in repairing !!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5479_112_2_18_2_8_11_260,(uint8*)"");
              if(0 != MMIAPICOM_Wstrnlen((wchar*)device_info.addr.addr, BT_ADDRESS_SIZE/sizeof(wchar)))
              {
		    BT_CancelPair(&device_info.addr);
              }
		return;
	}

	if(MMK_IsOpenWin(MMIENG_BT_CANCEL_WAIT_WIN_ID))
	{
		//cancelling, need not response this message
		//SCI_TRACE_LOW:"MMIBT_GetInputPinReq in Cacelling !!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5487_112_2_18_2_8_11_261,(uint8*)"");
		return;
	}

    MMIDEFAULT_SetBackLight(TRUE);
    //store the request pair device address
    SCI_MEMSET(&s_pin_request_info_ext, 0, sizeof(BT_DEVICE_INFO_EXT));
    SCI_MEMCPY(&s_pin_request_info_ext, &device_info,sizeof(BT_DEVICE_INFO_EXT));

    //有配对请求等待窗口说明是本机发起的配对
    MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_NUMERIC_PASSKEY_CHECK);

    BtPauseBGPlay(MMIBT_INPUTPIN_SUSPEND);
    BtResumeBGPlay(MMIBT_PAIR_SUSPEND);

    //蓝牙耳机主动配对
    if(0 == memcmp(bt_spp_numric_key, s_pin_request_info_ext.ssp_numric_key, 7))
    {
        //MMI_GetLabelTextByLang(STR_BT_ACCEPT_PAIR_REQUEST_EXT01, &query_text);
        name_text.wstr_ptr = s_pin_request_info_ext.name;
        name_text.wstr_len = MMIAPICOM_Wstrlen(s_pin_request_info_ext.name);
        query_text.wstr_ptr = test;
        MMIAPICOM_Wstrncpy(replace_wchar, name_text.wstr_ptr, name_text.wstr_len);
        SCI_TRACE_LOW("MMIBT_GetNumericPasskeyReq require name %d " ,  replace_wchar);
        MMIAPICOM_CustomGenerateReplaceStrByTextId(STR_BT_ACCEPT_PAIR_REQUEST_EXT01,L"%U",replace_wchar,100,&query_text);
        MMIBT_OpenQueryWin(MMI_BT_PAIR_QUERY_WIN_ID,&query_text,  PNULL, MMIBT_OPC_NUMERIC_PASSKEY_CHECK);
        
        return;
    }

	if(BT_SUCCESS == msg_body_ptr->status)
	{
		MMK_CreateWin((uint32 *)MMIBT_NUMERIC_PASSKEY_WIN_TAB, PNULL);
	}
	else
	{
		MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, PNULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
	}
    MMK_CloseWin(MMIBT_PAIR_REQUEST_WAIT_WIN_ID);
}

/*****************************************************************************/
//  Discription:
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC  void MMIBT_NumbericPasskeyCheck(void)
{
    MMI_STRING_T        wait_text = {0};
    BT_STATUS           return_value = BT_ERROR;
    MMI_WIN_ID_T               visible_win = MMIBT_PAIR_WAIT_WIN_ID;
    MMI_WIN_PRIORITY_E         win_priority_ptr = WIN_ONE_LEVEL;

    return_value = BT_InputPairPin(&s_pin_request_info_ext.addr,(uint8*)s_pin_request_info_ext.ssp_numric_key,strlen(s_pin_request_info_ext.ssp_numric_key));
    if (BT_PENDING == return_value)
    {
       MMI_GetLabelTextByLang(STR_BT_WAIT_PAIRING_EXT01, &wait_text);
       //MMIPUB_OpenWaitWin(1,&wait_text, PNULL ,PNULL,MMIBT_PAIR_WAIT_WIN_ID,IMAGE_NULL,
           //ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleCanbeCancelWaitingWinMsg);
       MMIPUB_OpenProgressWaitingIndWinByTextId(STR_BT_WAIT_PAIRING_EXT01,&visible_win,&win_priority_ptr,MMIPUB_SOFTKEY_NONE,PNULL);
    }
}

/*****************************************************************************/
//  Discription:
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC  void MMIBT_CancelNumricPair(void)
{
    s_is_pin_cancel = TRUE;
    if(0 != MMIAPICOM_Wstrnlen((wchar*)s_pin_request_info_ext.addr.addr, BT_ADDRESS_SIZE/sizeof(wchar)))
    {
        BT_CancelPair(&s_pin_request_info_ext.addr);
    }
    BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
}

/*****************************************************************************/
//  Discription:
//  Global resource dependence: none
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC  BT_DEVICE_INFO GetPinRequestInfo(void)
{
    return  s_pin_request_info;
}
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC void SetIsPinCancel(BOOLEAN  is_pin_cancel)
{
    s_is_pin_cancel = is_pin_cancel;
}
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN IsPinCancel(void)
{
    return s_is_pin_cancel;
}
/*****************************************************************************/
//  Discription: Handle searching new device waiting win msg
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCanbeCancelWaitingWinMsg(
                                                    MMI_WIN_ID_T win_id,
                                                    MMI_MESSAGE_ID_E msg_id,
                                                    DPARAM param
                                                    )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    BT_STATUS       bt_status = BT_SUCCESS;
	static BOOLEAN  s_need_cancel = FALSE;
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();

    SCI_TRACE_LOW("[MMIBT] HandleCanbeCancelWaitingWinMsg: msg_id=0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_need_cancel = TRUE;
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        //GUIWIN_SetSoftkeyTextId(win_id,  PNULL, PNULL,IMAGE_COMMON_BACK, FALSE);
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
	    GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)STXT_SOFTKEY_CANCEL_MK, FALSE);
        //GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_CANCEL,RIGHT_BUTTON,TRUE);
        break;
	case MSG_BT_AUTO_CLOSE_WIN:
        s_need_cancel = FALSE;
		break;
    case MSG_LOSE_FOCUS:
        if (MMIAPIKL_IsKLWinOnTop())
        {
            MMK_CloseWin( win_id );
        }
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
    case MSG_APP_CANCEL:
        s_need_cancel = FALSE;
        //User cancel
        bt_status = CancelOperation(operation_info.current_opertion);
        if((MSG_APP_RED == msg_id)&&!(MMIBT_OPC_AUTO_TEST == operation_info.root_opertion))
        {
            //need back to idle
            recode = MMI_RESULT_FALSE;
        }
        else if(bt_status != BT_PENDING)
        {
            AnimatePlayCtrlByState(MMIBT_ANIM_STOP);
            if(MMIBT_OPC_AUTO_TEST == operation_info.root_opertion)
            {
                //run next step
                MMIBT_CreateUitestWin();
            }
        }
        else
        {
            //OpenCancelWaitingWin();
        }
        if(MMIBT_GetOpertor().current_opertion == MMIBT_OPC_SEARCH)
        {
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_INIT);
        }
        MMK_CloseWin( win_id );
        break;
    case MSG_CLOSE_WINDOW:
        if(s_need_cancel)
        {
            BT_CancelSearch();
        }
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }

    return recode;
}
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : CreateSBandPopmenu
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
LOCAL void CreateSBandPopmenu(void)
{
    MMI_STRING_T      title_str = {0};
    uint16            cur_index     = 0;
    GUILIST_ITEM_T*   cur_list_ptr = {0};
    MMI_CTRL_ID_T      ctrl_id       = MMIBT_INQUIRED_DEV_LIST_CTRL_ID;
    
    MMK_CreateWin((uint32 *)MMIBT_INQUIRY_OPT_WIN_TAB,PNULL);
    GUIMENU_CreatDynamic(PNULL,MMIBT_INQUIRY_OPT_WIN_ID, MMIBT_LIST_INQUIRY_MENU_CTRL_ID,GUIMENU_STYLE_POPUP_AUTO);
    InsertNodeByLabel(
        MMIBT_LIST_INQUIRY_MENU_CTRL_ID,//ctrl_id
        STR_SOFTKEY_BT_PAIR_MK,//text_ptr
        MMIBT_INQUIRY_PAIRY_ID,//node_id
        0, //parent_node_id
        0,//index
        FALSE//is_grayed
        );
    
    cur_index = GUILIST_GetCurItemIndex(ctrl_id);
    cur_list_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, cur_index);/*lint !e605*/
    if(PNULL != cur_list_ptr)
    {
        title_str.wstr_len = cur_list_ptr->item_data_ptr->item_content[1].item_data.text_buffer.wstr_len;
        title_str.wstr_ptr = cur_list_ptr->item_data_ptr->item_content[1].item_data.text_buffer.wstr_ptr;
        GUIMENU_SetMenuTitle(&title_str, MMIBT_LIST_INQUIRY_MENU_CTRL_ID);
    }
}

/*****************************************************************************/
//  Description : MMIFMM_CreateNewSoftKey
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIBT_CreateNewSoftKey(MMI_HANDLE_T win_handle, MMI_TEXT_ID_T leftkey_id, MMI_TEXT_ID_T midkey_id, MMI_TEXT_ID_T rightkey_id)
{
    GUISOFTKEY_INIT_DATA_T  softkey_data = {0};
    MMI_CONTROL_CREATE_T softkey_create = {0};
    MMI_CTRL_ID_T   softkey_ctrl_id = 0;

    MMK_DestroyControl(MMK_GetCtrlHandleByWin(win_handle , MMITHEME_GetSoftkeyCtrlId()));

    softkey_data.leftsoft_id = leftkey_id;
    softkey_data.midsoft_id = midkey_id;
    softkey_data.rightsoft_id = rightkey_id;


    softkey_ctrl_id = MMITHEME_GetSoftkeyCtrlId();
    softkey_create.ctrl_id = softkey_ctrl_id;
    softkey_create.guid    = SPRD_GUI_SOFTKEY_ID;
    softkey_create.parent_win_handle = win_handle;
    softkey_create.init_data_ptr = &softkey_data;

    MMK_CreateControl( &softkey_create );

    MMK_SetWinSoftkeyCtrlId(win_handle, softkey_ctrl_id);
    return TRUE;
}
#endif
/****************************************************************************/
//  Discription: Open the bluetooth Inquired device list win
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL void  OpenInquiredDevListWin(void)
{
    MMK_CreateWin((uint32 *)MMIBT_INQUIRED_DEV_LIST_WIN_TAB, PNULL);

    GUILIST_SetMaxItem(MMIBT_INQUIRED_DEV_LIST_CTRL_ID,MMIBT_MAX_DEV_ITEMS, FALSE);//max item 20

    GUILIST_SetListState(MMIBT_INQUIRED_DEV_LIST_CTRL_ID, GUILIST_STATE_NEED_TITLE, FALSE );
}
/*****************************************************************************/
//  Discription:check headset permit to open
//  Global resource dependence: none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HeadsetOpenIsPermit(BT_ADDRESS * bt_addr)
{
#if 0
    uint32      service_type = 0;
#endif
    BT_DEVICE_INFO dev_info = {0};

#if 0
 	if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
 	{
 		//SCI_TRACE_LOW:"HeadsetOpenIsPermit in calling or alarming !"
 		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10854_112_2_18_2_8_22_347,(uint8*)"");
 		return FALSE;
 	}
#endif

    if(PNULL != bt_addr)
    {
        //first check already exist opened heaset
        if(MMIBT_OpenedHeadsetIsExist(&dev_info))
        {
           if(memcmp(&dev_info.addr, bt_addr, sizeof(BT_ADDRESS)))
           {
               //exist alrady opened headset
               //SCI_TRACE_LOW:"HeadsetOpenIsPermit exist alrady opened headset !"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10865_112_2_18_2_8_22_348,(uint8*)"");
               return FALSE;
           }
        }
		#if 0
        service_type = BT_GetPairedDeviceService(bt_addr);
        if ((BT_SERVICE_HANDSET |BT_SERVICE_HANDFREE) & service_type)
        {
            //Handfree, can not opened in call
            if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
            {
                //SCI_TRACE_LOW:"HeadsetOpenIsPermit in call !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10875_112_2_18_2_8_22_349,(uint8*)"");
                return FALSE;
            }
        }
        if((BT_SERVICE_A2DP | BT_SERVICE_HANDSET |BT_SERVICE_HANDFREE) & service_type)
        {
            //BT module does not need to judge audio condition
            /*
            //A2DP can not open, when music playing or paused
            if(MMIAPIMP3_IsPlayerActive())
            {
                //mp3 playing or paused
                //SCI_TRACE_LOW:"HeadsetOpenIsPermit mp3 pause or playing !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10887_112_2_18_2_8_22_350,(uint8*)"");
                return FALSE;
            }
            else if(MMIAPIKUR_IsPlayerActive())
            {
                //kuro playing or paused
                //SCI_TRACE_LOW:"HeadsetOpenIsPermit kuro pause or playing  !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10893_112_2_18_2_8_22_351,(uint8*)"");
                return FALSE;
            }
            */
        }
		#endif

    }
    //SCI_TRACE_LOW:"HeadsetOpenIsPermit ok"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10900_112_2_18_2_8_22_352,(uint8*)"");
    return TRUE;
}
/*****************************************************************************/
//  Discription: insert menu node bu text id
//  Global resource dependence: none
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                     MMI_CTRL_ID_T      ctrl_id,        //控件id
                     MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                     uint16          	node_id,        //节点编号
                     uint16          	parent_node_id, //父节点编号
                     uint16          	index,          //位置
                     BOOLEAN            is_graged
                     )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = is_graged;
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}
/*****************************************************************************/
//  Discription: open pin input win
//  Global resource dependence: none
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E OpenPinInputWin(BT_DEVICE_INFO *param_ptr,BTFUNCWINCALLBACK call_back)
{
    MMIBT_PIN_INPUT_PARAM_T *win_param_ptr = PNULL;
    MMI_HANDLE_T                win_handle = 0;
    BOOLEAN                     res = MMI_RESULT_FALSE;
    wchar             title_text[] = L"PIN";
    MMI_STRING_T	  title_str = {0};
    MMI_WIN_ID_T linkMenu_dialog_id = MMIBT_PIN_INPUT_WIN_ID;
    MMI_STRING_T    result1_string = {0};
    wchar text_temp[64] = {0};
    wchar text_temp1[64] = {0};

    if(PNULL == param_ptr)
        return res;

    if(MMK_IsOpenWin(MMIBT_PIN_INPUT_WIN_ID))
    {
        MMK_CloseWin(MMIBT_PIN_INPUT_WIN_ID);
    }

    win_param_ptr = (MMIBT_PIN_INPUT_PARAM_T *)SCI_ALLOC_APPZ(sizeof(MMIBT_PIN_INPUT_PARAM_T));
    if(PNULL != win_param_ptr)
    {
        SCI_MEMCPY(&win_param_ptr->pin_request_device_info, param_ptr, sizeof(BT_DEVICE_INFO));
        win_param_ptr->callbackfunc = call_back;
        //win_handle = MMK_CreatePubEditWin((uint32 *)MMIBT_PIN_INPUT_WIN_TAB, PNULL);
        //lulifei modify the paired device name
        MMI_GetLabelTextByLang(STR_BT_ENTER_CODE, &result1_string);
        MMIAPICOM_Wstrncpy(text_temp, result1_string.wstr_ptr,result1_string.wstr_len);
        MMIAPICOM_Wstrncpy(text_temp1, param_ptr-> name,MMIAPICOM_Wstrlen(param_ptr-> name));
        MMIAPICOM_WstrReplace(text_temp, sizeof(text_temp), L"%U", text_temp1);
        title_str.wstr_ptr = text_temp;
        title_str.wstr_len = MMIAPICOM_Wstrlen( title_str.wstr_ptr);
        
        //MMIPUB_OpenDialogInputFieldWinByTextPtr(&title_str,&linkMenu_dialog_id,PNULL,MMIPUB_SOFTKEY_CUSTOMER,HandlePinInputWinMsg);
        MMIPUB_OpenDialogPasswordWinByTextPtr(&title_str,&linkMenu_dialog_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,HandlePinInputWinMsg);
        MMK_SetWinUserData(linkMenu_dialog_id, (void *)win_param_ptr);
        res = MMI_RESULT_TRUE;
        /*if (!win_handle || (MMI_INVALID_ID == win_handle))
        {
        SCI_FREEIF(win_param_ptr);
        }
        else
        {
        MMK_SetWinUserData(MMIBT_PIN_INPUT_WIN_ID, (void *)win_param_ptr);
        res = MMI_RESULT_TRUE;
    }*/
    }
    else
    {
        SCI_FREEIF(win_param_ptr);
        win_param_ptr= PNULL;
        SCI_TRACE_LOW("OpenPinInputWin alloc fail");
    }

    return res;
}
/*****************************************************************************/
//  Discription: open pin input win
//  Global resource dependence: none
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void InputPairedPin(MMI_HANDLE_T win_handle,const wchar *str_ptr,int str_len)
{
    BT_STATUS        return_value = BT_PENDING;
    uint8            pin_str[MMIBT_PIN_MAX_LENGTH + 2 ] ={0};
    BT_DEVICE_INFO   pin_request_info = MMIBT_GetPinRequestInfo();
    MMI_STRING_T     wait_text = {0};
    MMI_WIN_ID_T  progress_dialog_id = MMIBT_PAIR_WAIT_WIN_ID;
    MMI_WIN_PRIORITY_E      win_priority = WIN_ONE_LEVEL;
    
    MMI_WSTRNTOSTR(pin_str, MMIBT_PIN_MAX_LENGTH, str_ptr,str_len, str_len);
    pin_str[str_len] = 0;
    SetIsPinCancel(FALSE);
    return_value = BT_InputPairPin(&pin_request_info.addr,(uint8*)pin_str,str_len);
    if (BT_PENDING == return_value)
    {
        MMI_GetLabelTextByLang(STR_BT_WAIT_PAIRING_EXT01, &wait_text);
        //lulifei modify for the progress dialog
        //MMIPUB_OpenWaitWin(1,&wait_text, PNULL ,PNULL,MMIBT_PAIR_WAIT_WIN_ID,IMAGE_NULL,
        //ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleCanbeCancelWaitingWinMsg);
        MMIPUB_OpenProgressWaitingIndWinByTextId(STR_BT_WAIT_PAIRING_EXT01,&progress_dialog_id,&win_priority,MMIPUB_SOFTKEY_NONE,PNULL);
        MMIBT_StartOperationWin(MMIBT_PAIR_WAIT_WIN_ID,MMIBT_OPC_PAIR,CreatePubWinCallBack);
    }
    else
    {
        MMK_CloseWin(win_handle);
    }
}

/*****************************************************************************/
//  Description : display numeric passkey information
//  Global resource dependence : none
//  Author:
//  Note:the 'MMIBT_PAIR_TEXT_LEN/2' in SCI_MEMCPY is to avoid array access
//       violation of content_str, it should be adjusted once the txt
//       TXT_BT_NUMERIC_PASSKEY couldn't display completely to a certain language
/*****************************************************************************/
LOCAL void Display_NumericPasskeyInfo(void)
{
    MMI_STRING_T        text_str                        = {0};//get and store TXT_BT_NUMERIC_PASSKEY
    MMI_CTRL_ID_T       ctrl_id                         = MMIBT_NUMERIC_PASSKEY_CTRL_ID;
    wchar               ssp_key[7]                      = {0};//get and store ssp pairing key
    wchar               content_str[MMIBT_PAIR_TEXT_LEN]= {0};//store the whole pairing info
    uint32              content_len                     = 0;
    MMI_STRING_T    result1_string = {0};
    wchar text_temp[64] = {0};
    
    //get the content and length of TXT_BT_NUMERIC_PASSKEY, then copy the content to content_str
    MMI_GetLabelTextByLang(STR_BT_ENTER_CODE, &text_str);
    MMIAPICOM_Wstrncpy(text_temp, text_str.wstr_ptr,text_str.wstr_len);
    MMIAPICOM_WstrReplace(text_temp, sizeof(text_temp), L"%U", L" ");
    result1_string.wstr_ptr = text_temp;
    result1_string.wstr_len = MMIAPICOM_Wstrlen( result1_string.wstr_ptr);
    if(result1_string.wstr_ptr != PNULL)
    {
        SCI_MEMCPY(content_str, result1_string.wstr_ptr, sizeof(wchar)*MIN(result1_string.wstr_len,MMIBT_PAIR_TEXT_LEN/2));
    }
    
    //get the device name from s_pin_request_info_ext and concat it to content_str, then contact a line break
    MMIAPICOM_Wstrcat(content_str, L" ");
	MMIAPICOM_Wstrncat(content_str, s_pin_request_info_ext.name, MMIAPICOM_Wstrlen(s_pin_request_info_ext.name));
    MMIAPICOM_Wstrcat(content_str, L"?\n");

    //get ssp pairing key and concat to content_str
    MMIAPICOM_StrToWstr(s_pin_request_info_ext.ssp_numric_key, ssp_key);
    MMIAPICOM_Wstrncat(content_str, ssp_key, MMIAPICOM_Wstrlen(ssp_key));

    //set the whole pairing info to the ctrl
    content_len = MMIAPICOM_Wstrlen(content_str);
    GUITEXT_SetString(ctrl_id, content_str, content_len, FALSE);

    SCI_TRACE_LOW("Display_NumericPasskeyInfo info len %d",content_len);
}

/*****************************************************************************/
//  Discription:
//  Global resource dependence: none
//  Author: yang.yang
//  Note:
//
/*****************************************************************************/
LOCAL void MMIBT_OpenWaitTimerOut(void)
{
    uint16    dev_type = 0;

    MMIBT_StopOpenWaitTimer();

    BT_GetDeviceType(&s_pin_request_info, &dev_type);
    if(dev_type & BT_DEVICE_CLASS_AUDIO)
    {
        //headset need open it directly
        if(HeadsetOpenIsPermit(&(s_pin_request_info.addr)))
        {
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_OPEN_HEADSET);
            OpenHeadset(&s_pin_request_info.addr);
        }
    }
#ifdef MMI_BT_PBAP_SUPPORT
    else if(dev_type & BT_DEVICE_CLASS_PHONE)
    {
        //headset need open it directly
        if(HandsetGatewayOpenIsPermit(&(s_pin_request_info.addr)))
        {
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_OPEN_HANDSET_GATEWAY);
            OpenHandsetGateway(&s_pin_request_info.addr);
        }
    }
#endif
}

/*****************************************************************************/
//  Discription:
//  Global resource dependence: none
//  Author: yang.yang
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_StopOpenWaitTimer(void)
{
    if(s_headset_openwait_timer != 0)
    {
        MMK_StopTimer(s_headset_openwait_timer);
        s_headset_openwait_timer = 0;
    }
    SCI_TRACE_LOW("[MMIBT] MMIBT_CloseOpenWaitTimer, timer_id = %d", s_headset_openwait_timer);
}



/*****************************************************************************/
//  Discription: open the window of new file win
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_DeleteDeviceCnf(
                                  BT_MSG_T *msg_body_ptr
                                  )
{
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();
    //lulifei add to update the delete device
    uint16 cur_pos = 0 ;
    BT_DEVICE_INFO * pbtinfo = (BT_DEVICE_INFO *)msg_body_ptr->body_ptr;
    BT_DEVICE_INFO delete_dev_info = {0};
    MMICOM_BANNER_INFO_T banner_info = {0};
    wchar    replace_wchar[50] = {0};
    MMI_STRING_T    result1_string = {0};
    MMI_STRING_T    result2_string = {0};
    MMI_STRING_T    device_name = {0};
    wchar text_temp[64] = {0};
    wchar old_str[] = {0x005c, 0x006e,0x0000};
    wchar old_str1[] = {0x0025, 0x0055,0x0000};
    wchar newstr[] = {0x000a,0x0000};
    wchar test1[64] = {0};
    wchar test_exa[3] = {0x002e,0x002e,0x002e};
	BT_DEVICE_INFO current_device_info = MMIBT_GetCurBtInfo();

    /*if (PNULL == msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_DeleteDeviceCnf PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4763_112_2_18_2_8_9_252,(uint8*)"");
        return ;
    }*/
    //SCI_TRACE_LOW:"MMIBT_DeleteDeviceCnf: The action result is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4766_112_2_18_2_8_9_253,(uint8*)"d",msg_body_ptr->status);
    MMIDEFAULT_SetBackLight(TRUE);
    if (BT_SUCCESS == msg_body_ptr->status)
    {
        SCI_TRACE_LOW("[MMIBT]MMIBT_DeleteDeviceCnf: %d, %d,%d", MMK_IsOpenWin( MMIBT_DELETE_DEVICE_WAIT_WIN_ID ), operation_info.current_opertion, MMIBT_OPC_DEVICE_DELETE);
        if (MMK_IsOpenWin( MMIBT_DELETE_DEVICE_WAIT_WIN_ID ))
        {
            //user delete device
            if(MMIBT_OPC_DEVICE_DELETE == operation_info.current_opertion)
            {
                //delete one cnf
                if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
                {
                    device_name.wstr_ptr = current_device_info.name;
                    device_name.wstr_len = MMIAPICOM_Wstrlen(current_device_info.name);
                    if(device_name.wstr_len >50){
                        MMIAPICOM_Wstrncpy(test1,device_name.wstr_ptr,50);
                        MMIAPICOM_Wstrcat(test1,test_exa);
                    }
                    else
                    {
                        MMIAPICOM_Wstrncpy(test1,device_name.wstr_ptr,device_name.wstr_len);
                    }
                    MMI_GetLabelTextByLang(STR_BT_CONF_REMOVED, &result1_string);
                    MMIAPICOM_Wstrncpy(text_temp, result1_string.wstr_ptr,result1_string.wstr_len);
                    MMIAPICOM_WstrReplace(text_temp, sizeof(text_temp), old_str1, test1);
                    
                    //MMIAPICOM_WstrReplace(text_temp, sizeof(text_temp), old_str, newstr);
                    MMIAPICOM_WstrReplace(text_temp, sizeof(text_temp), old_str, L" ");
                    //result2_string.wstr_ptr = text_temp;
                    //result2_string.wstr_len = MMIAPICOM_Wstrlen(result2_string.wstr_ptr);
                    //MMIAPICOM_Wstrcat(result2_string.wstr_ptr, device_name.wstr_ptr);
                    //MMIAPICOM_Wstrcat(result2_string.wstr_ptr, L" ");
                    banner_info.text1_str.wstr_ptr = text_temp;
                    banner_info.text1_str.wstr_len= MMIAPICOM_Wstrlen(banner_info.text1_str.wstr_ptr );
                    banner_info.banner_type = MMI_BANNER_STR_TEXT;
                    //banner_info.start_point = start_point;
                    MMIAPICOM_OpenBannerWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID, banner_info);
                    
                    //cur_pos = GUILIST_GetCurItemIndex( MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
                    //BT_GetPairedDeviceInfo(BT_SERVICE_ALL, paired_device_index[cur_pos], &delete_dev_info);
                    //if(!memcmp(s_delete_device_info.addr.addr, delete_dev_info.addr.addr, sizeof(s_delete_device_info.addr.addr))){
                    //GUILIST_RemoveItem( MMIBT_INQUIRED_DEV_LIST_CTRL_ID, cur_pos);
                    //if(s_delete_device_info != NULL){
                    SCI_TRACE_LOW("MMIBT_DeleteDeviceCnf s_inquired_device_num= %d" + s_inquired_device_num);
                    
                    if(s_inquired_device_ptr != PNULL )
                    {
                        SCI_MEMSET(&s_inquired_device_ptr[s_inquired_device_num], 0 , sizeof(BT_DEVICE_INFO));
                        //get device name info from message body
                        MMIAPICOM_Wstrncpy(s_inquired_device_ptr[s_inquired_device_num ].name ,
                            current_device_info.name,
                            MMIAPICOM_Wstrlen((wchar*)current_device_info.name));
                        //get device addr info from message body
                        SCI_MEMCPY(s_inquired_device_ptr[s_inquired_device_num].addr.addr,
                            current_device_info.addr.addr,
                            BT_ADDRESS_SIZE);
                        //get new inquired device class info
                        s_inquired_device_ptr[s_inquired_device_num ].dev_class = current_device_info.dev_class;
                        s_inquired_device_num ++;
                        s_update_ui = TRUE;
                    }
                    //          }
                    MMK_SendMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID, MSG_FULL_PAINT, NULL);
                }
                else if(MMK_IsOpenWin(MMIBT_HANDFREE_MENU_WIN_ID))
                {
                    MMK_SendMsg(MMIBT_HANDFREE_MENU_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, NULL);
                }
                
                {
                    MMIBT_StopOperationWin(operation_info.current_opertion);
                }
                
            }
            else if(MMIBT_OPC_DEVICE_DELETE_ALL == operation_info.current_opertion)
            {
                SCI_TRACE_LOW("[MMIBT]MMIBT_DeleteDeviceCnf:delete all ");
                //delete all cnf, delete next or other operation
                DoOperation(operation_info.current_opertion,FALSE);
            }
            else
            {
                           SCI_TRACE_LOW("[MMIBT]MMIBT_DeleteDeviceCnf:else %d", MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID));
				if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
				{
					MMK_SendMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID, MSG_BT_SEARCH_FINISHED, NULL);
				}
				else if(MMK_IsOpenWin(MMIBT_HANDFREE_MENU_WIN_ID))
				{
					MMK_SendMsg(MMIBT_HANDFREE_MENU_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, NULL);
				}

				MMIBT_StopOperationWin(operation_info.current_opertion);
			}
		}
    }
    else
    {
        if (MMK_IsOpenWin( MMIBT_DELETE_DEVICE_WAIT_WIN_ID ))
        {
            ClosePrmAndOpenDoneWin(
                                    MMIBT_DELETE_DEVICE_WAIT_WIN_ID,
                                    TXT_ERROR,
                                    IMAGE_PUBWIN_FAIL,
                                    MMI_2SECONDS
                                    );
        }
    }
}
/*****************************************************************************/
//  Discription: Open the blue tooth inquire device list  win
//  Global resource dependence: none
//  Author: lifei.lu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_EnterInquireDeviceListWin(MMIBT_TRANSFER_FILE_INFO_T *file_info_ptr,
                                    MMIBT_OPERTATION_TYPE_E	root_opertion,
                                    uint32         service)
{
    MMIBT_DEVICE_LIST_PARAM_T device_list_param = {0};

    device_list_param.root_opertion = root_opertion;
    device_list_param.current_service = service;
    if(PNULL != file_info_ptr)
    {
        SCI_MEMCPY(device_list_param.transfer_file_info.transfer_file_name,file_info_ptr->transfer_file_name,sizeof(device_list_param.transfer_file_info.transfer_file_name));
        device_list_param.transfer_file_info.is_temp_file = file_info_ptr->is_temp_file;
    }
    OpenInquireDeviceListWin(&device_list_param);
}
/*****************************************************************************/
//  Discription: Open the blue tooth inqure device list  win
//  Global resource dependence: none
//  Author: lifei.lu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenInquireDeviceListWin(MMIBT_DEVICE_LIST_PARAM_T *param_ptr)
{
    MMIBT_DEVICE_LIST_PARAM_T *win_param_ptr = PNULL;
    MMI_HANDLE_T                win_handle = 0;
    BOOLEAN                     res = MMI_RESULT_FALSE;

    if(PNULL == param_ptr)
        return res;

    if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
    {
        MMK_CloseWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID);
    }

    win_param_ptr = (MMIBT_DEVICE_LIST_PARAM_T *)SCI_ALLOC_APPZ(sizeof(MMIBT_DEVICE_LIST_PARAM_T));
    if(PNULL != win_param_ptr)
    {
        win_param_ptr->current_service = param_ptr->current_service;
        win_param_ptr->root_opertion = param_ptr->root_opertion;
        SCI_MEMSET(win_param_ptr->transfer_file_info.transfer_file_name,0,sizeof(win_param_ptr->transfer_file_info.transfer_file_name));
        SCI_MEMCPY(win_param_ptr->transfer_file_info.transfer_file_name, param_ptr->transfer_file_info.transfer_file_name,
                        sizeof(win_param_ptr->transfer_file_info.transfer_file_name));
        win_param_ptr->transfer_file_info.is_temp_file = param_ptr->transfer_file_info.is_temp_file;
        win_handle = MMK_CreateWin((uint32 *)MMIBT_INQUIRED_DEV_LIST_WIN_TAB,PNULL);
        if (!win_handle || (MMI_INVALID_ID == win_handle))
        {
            SCI_FREEIF(win_param_ptr);
        }
        else
        {
            MMK_SetWinUserData(MMIBT_INQUIRED_DEV_LIST_WIN_ID, (void *)win_param_ptr);
            res = MMI_RESULT_TRUE;
            DoOperation(MMIBT_OPC_SEARCH, FALSE);
        }
    }
    else
    {
        SCI_FREEIF(win_param_ptr);
        SCI_TRACE_LOW("OpenInquireDeviceListWin alloc fail");
    }

    return res;
}

/*****************************************************************************/
//  Discription: append item to list control(device name to list)
//  Global resource dependence: none
//  Author: lu.lifei
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_ReplaceDeviceListItem(
                                      BT_DEVICE_INFO   *device, BOOLEAN openornot)
{

    MMI_TEXT_ID_T    string_detail= TXT_NULL;
    GUIITEM_STYLE_E  item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    MMI_TEXT_ID_T    left_softkey_id= STXT_OPTION;
    MMI_TEXT_ID_T    middle_softkey_id = TXT_NULL;
    MMI_TEXT_ID_T right_softkey_id  = STXT_RETURN;
    MMI_CTRL_ID_T    ctrl_id = MMIBT_INQUIRED_DEV_LIST_CTRL_ID;
    GUILIST_ITEM_T      item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint32                 index = 0;

    if(device == PNULL)
    {
        return;
    }

   if(openornot)
   {
       middle_softkey_id = STR_SOFTKEY_BT_DISCONNECT_MK;
       string_detail = STR_SOFTKEY_BT_CONNECT_MK;
   }
   else
   {
       middle_softkey_id = STR_SOFTKEY_BT_CONNECT_MK;
       string_detail = STR_SOFTKEY_BT_DISCONNECT_MK;
   }

    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM; //GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    //set softkey
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    //set text1
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen((wchar*)device->name);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = device->name;
    item_data.item_content[2].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[2].item_data.text_id = string_detail;
    item_data.item_content[2].is_opacity = TRUE;
    GUILIST_SetItemContentOpacityFontColor(&item_data,2,255*0.6,255*0.6);
    GUILIST_ReplaceItem( ctrl_id, &item_t,headset_item);
}
/*****************************************************************************/
//  Discription:Add the paired device list
//  Global resource dependence: none
//  Author: lifei.lu
//  Note:
//
/*****************************************************************************/
LOCAL void MMIBT_AddPairedDeviceList(MMIBT_PAIRED_DEVICE_TYPE_E pair_display)
{
    int32          i = 0;
    uint32        paired_service = 0;
    BT_DEVICE_INFO dev_info = {0};
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();
    MMI_TEXT_ID_T left_softkey_id   = TXT_NULL;
    MMI_TEXT_ID_T middle_softkey_id = TXT_NULL;
    MMI_TEXT_ID_T string_detail = TXT_NULL;
    MMI_TEXT_ID_T right_softkey_id  = STXT_RETURN;

    //MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_LIST_PAIRED);
    MMIBT_SetCurrentSevice(BT_SERVICE_ALL);
    //get the paired device and append to the list
    MMIBT_GetPairedDevList(BT_SERVICE_ALL, paired_device_index);
    list_num = BT_GetPairedDeviceCount(BT_SERVICE_ALL);
    for(i =0; i<list_num; i++ )
    {
        //set middle softkey
        SCI_MEMSET(&dev_info, 0x00, sizeof(dev_info));
        BT_GetPairedDeviceInfo(BT_SERVICE_ALL, paired_device_index[i], &dev_info);

        paired_service = BT_GetPairedDeviceService(&(dev_info.addr));
        BT_GetPairedDeviceName(&(dev_info.addr), dev_info.name);

        if(MMIBT_OPC_MAIN == operation_info.root_opertion)
        {
            if(MMIBT_DeviceIsConnectedHeadset(&(dev_info.addr)))
            {
                ////current device is headset and already actived middle softkey is "Disconnect"
#if 0//def MID_SOFTKEY_USE_IMG_SUPPORT
                middle_softkey_id = IMAGE_COMMON_SOFTKEY_DISCONNECT_ICON;
#else
                middle_softkey_id = STR_SOFTKEY_BT_DISCONNECT_MK;
                string_detail = STR_BT_STATUS_CONNECTED_EXT01;
#endif
            }
            else if((paired_service & (BT_SERVICE_HANDSET |BT_SERVICE_HANDFREE)) || (paired_service & BT_SERVICE_A2DP))
            {
                //current device is headset and not active, middle softkey is "Connect"
#if 0//def MID_SOFTKEY_USE_IMG_SUPPORT
                middle_softkey_id = IMAGE_COMMON_SOFTKEY_CONNECT_ICON;
#else
                middle_softkey_id = STR_SOFTKEY_BT_CONNECT_MK;
                string_detail = STR_BT_STATUS_PAIRED_EXT01;
#endif
            }
            else
            {
#if 0//def MID_SOFTKEY_USE_IMG_SUPPORT
                middle_softkey_id = IMAGE_COMMON_SOFTKEY_DETAIL_ICON;
#else
                middle_softkey_id = STXT_SOFTKEY_DETAILS_MK;
                string_detail = STR_BT_STATUS_PAIRED_EXT01;
#endif
            }
			left_softkey_id = STXT_OPTION;
        }
        if(MMIBT_OPC_OPP_SEND == operation_info.root_opertion)
        {
            if(MMIBT_DeviceIsConnectedHeadset(&(dev_info.addr)))
            {
                //current device is headset and already actived middle softkey is "Disconnect"
                middle_softkey_id = STR_SOFTKEY_BT_DISCONNECT_MK;
                string_detail = STR_BT_STATUS_CONNECTED_EXT01;
            }
            else if((paired_service & (BT_SERVICE_HANDSET |BT_SERVICE_HANDFREE)) || (paired_service & BT_SERVICE_A2DP))
            {
                //current device is headset and not active, middle softkey is "Connect"
                middle_softkey_id = STR_SOFTKEY_BT_CONNECT_MK;
                string_detail = STR_BT_STATUS_PAIRED_EXT01;
            }
            else
            {
                middle_softkey_id = STXT_SOFTKEY_DETAILS_MK;
                string_detail = STR_BT_STATUS_PAIRED_EXT01;
            }
            left_softkey_id= STR_LIST_SHARE;
        }

#ifdef MMI_BT_PBAP_SUPPORT
        if(MMIBT_OPC_CONNECT_PBAP == operation_info.root_opertion)
        {
            middle_softkey_id = STXT_SOFTKEY_DETAILS_MK;
            string_detail = STR_BT_STATUS_PAIRED_EXT01;
            if(paired_service & BT_SERVICE_PBAP_SERVER)
            {
                left_softkey_id= STR_LIST_SHARE;
            }
            else
            {
                left_softkey_id= TXT_NULL;
            }
        }
#endif

        if(pair_display == ID_PAIRED_DEVICE_SEARCH_DEVICE)
        {
            right_softkey_id = STXT_STOP;
            MMIBT_AppendDeviceListItem(
                &(dev_info),
                string_detail,
                GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT,
                TXT_NULL,
                TXT_NULL,
                right_softkey_id,
                MMIBT_INQUIRED_DEV_LIST_CTRL_ID
                );
        }
        else
        {
            MMIBT_AppendDeviceListItem(
                &(dev_info),
                string_detail,
                GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT,
                left_softkey_id,
                middle_softkey_id,
                right_softkey_id,
                MMIBT_INQUIRED_DEV_LIST_CTRL_ID
                );
        }
    }
    SCI_TRACE_LOW("MMIBT_AddPairedDevice add the pair device successfully");
}
#endif
#ifdef MMI_BT_PBAP_SUPPORT
/*****************************************************************************/
//  Discription:check headset permit to open
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandsetGatewayOpenIsPermit(BT_ADDRESS * bt_addr)
{
    uint32      service_type = 0;
    BT_DEVICE_INFO dev_info = {0};
    BT_ADDRESS  dev_addr = {0};
    
    if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        //SCI_TRACE_LOW:"HandsetGatewayOpenIsPermit in calling or alarming !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10854_112_2_18_2_8_22_347,(uint8*)"");
        return FALSE;
    }
    if(PNULL != bt_addr)
    {
        //first check already exist opened heaset
        if(MMIBT_OpenedHeadsetIsExist(&dev_info))
        {
            if(memcmp(&dev_info.addr, bt_addr, sizeof(BT_ADDRESS)))
            {
                //exist alrady opened headset
                //SCI_TRACE_LOW:"HeadsetOpenIsPermit exist alrady opened headset !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10865_112_2_18_2_8_22_348,(uint8*)"");
                return FALSE;
            }
        }
        //first check already exist opened heaset
        if(MMIBT_OpenedHandsetGatwayIsExist(&dev_info))
        {
           if(memcmp(&dev_info.addr, bt_addr, sizeof(BT_ADDRESS)))
           {
               //exist alrady opened headset
               //SCI_TRACE_LOW:"HandsetGatewayOpenIsPermit exist alrady opened headset !"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10865_112_2_18_2_8_22_348,(uint8*)"");
               return FALSE;
           }
        }
        service_type = BT_GetPairedDeviceService(bt_addr);
        if ((BT_SERVICE_HANDFREE_GATEWAY |BT_SERVICE_HEADSET_GATEWAY) & service_type)
        {
            if(MMIBT_IsConnectDevice(&dev_addr))
            {
                SCI_TRACE_LOW("HeadsetOpenIsPermit if Hfu is not connected");
                return FALSE;
            }
        }
        if((BT_SERVICE_HEADSET_GATEWAY|BT_SERVICE_HANDFREE_GATEWAY | BT_SERVICE_MAP_SERVER |BT_SERVICE_PBAP_SERVER) & service_type)
        {
            //BT module does not need to judge audio condition
            /*
            //A2DP can not open, when music playing or paused
            if(MMIAPIMP3_IsPlayerActive())
            {
                //mp3 playing or paused 
                //SCI_TRACE_LOW:"HandsetGatewayOpenIsPermit mp3 pause or playing !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10887_112_2_18_2_8_22_350,(uint8*)"");
                return FALSE;
            }
            else if(MMIAPIKUR_IsPlayerActive())
            {
                //kuro playing or paused
                //SCI_TRACE_LOW:"HandsetGatewayOpenIsPermit kuro pause or playing  !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10893_112_2_18_2_8_22_351,(uint8*)"");
                return FALSE;
            }
            */
        }

    }
    //SCI_TRACE_LOW:"HandsetGatewayOpenIsPermit ok"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10900_112_2_18_2_8_22_352,(uint8*)"");
    return TRUE;
}
#endif
