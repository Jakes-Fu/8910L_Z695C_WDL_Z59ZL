#include "mmi_app_idle_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "mmk_kbd.h"
#include "guilcd.h"
#include "window_parse.h"
#include "mmi_image.h"
#include "guicommon.h"
#include "mmi_text.h"
#include "tb_dal.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "guimenu.h"
#include "guitext.h"
#include "guimsgbox.h"
#include "guilabel.h"
#include "mmk_timer.h"
#include "mmi_default.h"
#include "mmiacc.h"
#include "guiblock.h"
#include "guibutton.h"
#include "mmipub.h"
#include "guistatusbar.h"
#include "mmialarm_id.h"
#include "mmiset_id.h"
#include "mmicc_id.h"
#include "mmiota_image.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmk_msg.h"
#include "guiwin.h"
#include "guilistbox.h"
#include "mn_api.h"
#include "mmi_resource.h"
#include "guistring.h"
#include "mmipdp_export.h"

#include "mmiset_export.h"
#include "mmiset_multisim_export.h"

#include "smartdualsim_id.h"
#include "smartdualsim_text.h"
#include "smartdualsim_image.h"
#include "smartdualsim_menutable.h"
#include "smartdualsim_internal.h"
#include "smartdualsim_export.h"

#include "mmisms_export.h"
#include "mn_api.h"

extern PUBLIC BOOLEAN MMIAPISET_SetSmartDualSIMStandby(BOOLEAN * is_standby_ptr, MN_DUAL_SYS_E dual_sys);
extern PUBLIC void MMIAPISET_SetSmartDualSIMPinEnable(MN_DUAL_SYS_E dual_sys, BOOLEAN enable);
extern PUBLIC void MMIAPISET_ChangeSmartDualSIMPin(MN_DUAL_SYS_E dual_sys);
extern PUBLIC MMI_HANDLE_T MMIAPISET_SmartDualSIMopenFDNbySIM(MN_DUAL_SYS_E dual_sys);

LOCAL MMI_RESULT_E HandleSimSettingsWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         );
LOCAL void MMISmartDS_setSimSettingsItems(BOOLEAN isUpdate);

LOCAL MMI_HANDLE_T MMISmartDS_SimOneWin(MN_DUAL_SYS_E simID);
LOCAL MMI_RESULT_E HandleSimOneWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         );
LOCAL void MMISmartDS_setSimOnesItems(MN_DUAL_SYS_E simID, BOOLEAN isUpdate);

LOCAL MMI_HANDLE_T MMISmartDS_SimNameWin(MN_DUAL_SYS_E simID);
LOCAL MMI_RESULT_E HandleSimNameWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        );
LOCAL BOOLEAN MMISmartDS_SetSimStatus(MN_DUAL_SYS_E simID, BOOLEAN simOn);

LOCAL MMI_RESULT_E HandleNetworkWaittingIndicatorWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        );

LOCAL MMI_HANDLE_T MMISmartDS_VodafoneCarrierWin(MN_DUAL_SYS_E simID);
LOCAL MMI_RESULT_E HandleVodafoneCarrierWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        );
LOCAL void MMISmartDS_setSimVodafoneCarrierItems(MN_DUAL_SYS_E simID, BOOLEAN isUpdate);

WINDOW_TABLE( SMARTDUALSIM_SIM_SETTINGS_WIN_TAB ) = 
{
    WIN_ID(MMI_SMARTDUALSIM_SIM_SETTINGS_WIN_ID),
    WIN_FUNC((uint32) HandleSimSettingsWinMsg),
    //CREATE_MENU_CTRL(MENU_SIM_SETTINGS_TABLE_ID, MMI_SMARTDUALSIM_SIM_SETTINGS_CTRL_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMI_SMARTDUALSIM_SIM_SETTINGS_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE( SMARTDUALSIM_SIM_ONE_WIN_TAB ) = 
{
    WIN_ID(MMI_SMARTDUALSIM_SIM_ONE_WIN_ID),
    WIN_FUNC((uint32) HandleSimOneWinMsg),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMI_SMARTDUALSIM_SIM_ONE_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE( SMARTDUALSIM_SIM_NAME_EDIT_WIN_TAB ) = 
{
    WIN_ID(MMI_SMARTDUALSIM_NAME_EDIT_WIN_ID),
    WIN_FUNC((uint32) HandleSimNameWinMsg),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_SMARTDUALSIM_SIM_NAME_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE, MMISET_MAX_SIM_NAME_LEN, MMI_SMARTDUALSIM_SIM_NAME_EDIT_CTRL_ID, MMI_SMARTDUALSIM_SIM_NAME_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SAVE_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE( SMARTDUALSIM_SIM_VODAFONE_CARRIER_WIN_TAB ) = 
{
    WIN_ID(MMI_SMARTDUALSIM_VODAFONE_CARRIER_WIN_ID),
    WIN_FUNC((uint32) HandleVodafoneCarrierWinMsg),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMI_SMARTDUALSIM_VODAFONE_CARRIER_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

PUBLIC MMI_HANDLE_T MMISmartDS_SimSettingsWin(void)
{
    if(MMK_IsOpenWin(MMI_SMARTDUALSIM_SIM_SETTINGS_WIN_ID))
            MMK_CloseWin(MMI_SMARTDUALSIM_SIM_SETTINGS_WIN_ID);
    return MMK_CreateWin((uint32*)SMARTDUALSIM_SIM_SETTINGS_WIN_TAB, PNULL);
}

LOCAL MMI_RESULT_E HandleSimSettingsWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};

    lcd_dev_info.lcd_id     = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id   = GUI_BLOCK_MAIN;

    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMISmartDS_setSimSettingsItems(FALSE);
            MMK_SetAtvCtrl(win_id, MMI_SMARTDUALSIM_SIM_SETTINGS_CTRL_ID);
            break;

        case MSG_GET_FOCUS:
            break;

        case MSG_LOSE_FOCUS:
            break;

        case MSG_FULL_PAINT:
            if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_SIM_LIST_SETT_EXT02))
            {
                GUIWIN_UpdateStb();
            }
            MMISmartDS_setSimSettingsItems(TRUE);
            break;

        case MSG_APP_WEB:
//        case MSG_APP_OK:
//        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
            {
                MMI_MENU_ID_T menu_id = 0;

                menu_id = GUILIST_GetCurItemIndex(MMI_SMARTDUALSIM_SIM_SETTINGS_CTRL_ID);
                switch(menu_id)
                {
                    case MMI_SMARTDUALSIM_SIM_SETTINGS_SIM1:
                        MMISmartDS_SimOneWin(MN_DUAL_SYS_1);
                        break;
                    case MMI_SMARTDUALSIM_SIM_SETTINGS_SIM2:
                        MMISmartDS_SimOneWin(MN_DUAL_SYS_2);
                        break;
                    default:
                        break;
                }
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
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

LOCAL void MMISmartDS_setSimSettingsItems(BOOLEAN isUpdate)
{
    MMI_CTRL_ID_T               ctrl_id = MMI_SMARTDUALSIM_SIM_SETTINGS_CTRL_ID;
    GUILIST_ITEM_T              item_t = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    item_t.item_data_ptr = &item_data;

    if(!isUpdate)
    {
        GUILIST_SetMaxItem(ctrl_id, MMI_SMARTDUALSIM_SIM_SETTINGS_ITEM_NUM,FALSE);
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
    }

    //set item "SIM1"
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_ILIST_SIM_ONE;

    if(isUpdate)
    {
        GUILIST_ReplaceItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_SIM_SETTINGS_SIM1);
    }
    else
    {
        GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_SIM_SETTINGS_SIM1);
    }
    if(!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
    {
        GUILIST_SetItemGray(ctrl_id, MMI_SMARTDUALSIM_SIM_SETTINGS_SIM1, TRUE);
        GUILIST_SetItemInvalid(ctrl_id, MMI_SMARTDUALSIM_SIM_SETTINGS_SIM1, TRUE);
    }
    
    //set item "SIM2"
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_ILIST_SIM_TWO;
    if(isUpdate)
    {
        GUILIST_ReplaceItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_SIM_SETTINGS_SIM2);
    }
    else
    {
        GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_SIM_SETTINGS_SIM2);
    }
    if(!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {
        GUILIST_SetItemGray(ctrl_id, MMI_SMARTDUALSIM_SIM_SETTINGS_SIM2, TRUE);
        GUILIST_SetItemInvalid(ctrl_id, MMI_SMARTDUALSIM_SIM_SETTINGS_SIM2, TRUE);
    }
}

LOCAL MMI_HANDLE_T MMISmartDS_SimOneWin(MN_DUAL_SYS_E simID)
{
    if(MMK_IsOpenWin(MMI_SMARTDUALSIM_SIM_ONE_WIN_ID))
            MMK_CloseWin(MMI_SMARTDUALSIM_SIM_ONE_WIN_ID);
    return MMK_CreateWin((uint32*)SMARTDUALSIM_SIM_ONE_WIN_TAB, (ADD_DATA)simID);
}

LOCAL MMI_RESULT_E HandleSimOneWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E simID = (MN_DUAL_SYS_E)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMISmartDS_setSimOnesItems(simID, FALSE);
            MMK_SetAtvCtrl(win_id, MMI_SMARTDUALSIM_SIM_ONE_CTRL_ID);
            break;

        case MSG_GET_FOCUS:
            if(!MMIAPIPHONE_GetSimExistedStatus(simID))
                MMK_CloseWin(win_id);	
            break;

        case MSG_LOSE_FOCUS:
            break;

        case MSG_FULL_PAINT:
        {
            MMI_CTRL_ID_T statusTitleID = STR_ILIST_SIM_ONE;

            if(MN_DUAL_SYS_2 == simID)
            {
                statusTitleID = STR_ILIST_SIM_TWO;
            }
            if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(statusTitleID))
            {
                GUIWIN_UpdateStb();
            }
        }
            MMISmartDS_setSimOnesItems(simID, TRUE);
            if(!MMIAPIPHONE_GetSimExistedStatus(simID))
            {
                MMIPUB_OpenAlertWarningWin(TXT_SIM_PLUG_OUT);
            }
            break;

        case MSG_APP_WEB:
//        case MSG_APP_OK:
//        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
            {
                uint16          curItemIndex = 0;
                MMI_MENU_ID_T menu_id = 0;

                curItemIndex = GUILIST_GetCurItemIndex(MMI_SMARTDUALSIM_SIM_ONE_CTRL_ID);
                GUILIST_GetItemData(MMI_SMARTDUALSIM_SIM_ONE_CTRL_ID,curItemIndex, &menu_id);
                switch(menu_id)
                {
                    case MMI_SMARTDUALSIM_SIM_ONE_NAME:
                        MMISmartDS_SimNameWin(simID);
                        break;
                    case MMI_SMARTDUALSIM_SIM_ONE_STATUS:
                    { 
                            BOOLEAN isSelected = FALSE;
                            BOOLEAN isSetAllow = FALSE;
                        if (MMIAPISET_GetFlyMode())
                        {
                            MMIAPIPHONE_AlertSimNorOKStatus();
                        }
                        else
                        {
                            isSetAllow = MMISmartDS_SetSimStatus(simID, isSelected);
                            CTRLLIST_GetToggleState(MMI_SMARTDUALSIM_SIM_ONE_CTRL_ID, curItemIndex, &isSelected);
                            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                            if(FALSE == isSetAllow)
                            {
                                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SYS_WAITING_AND_TRY_LATER, TXT_NULL, 
                                    IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                            }
                        }		
                    }
                        break;
                    case MMI_SMARTDUALSIM_SIM_ONE_NETWORK_SELECT:
                        MMIAPISET_OpenSelNetWorkWin(simID);
                        break;
                    case MMI_SMARTDUALSIM_SIM_ONE_PIN_REQUEST:
                        {
                            BOOLEAN enable = FALSE;

                            CTRLLIST_GetToggleState(MMI_SMARTDUALSIM_SIM_ONE_CTRL_ID, curItemIndex, &enable);
                            MMIAPISET_SetSmartDualSIMPinEnable(simID, enable);
                        }
                        break;
                    case MMI_SMARTDUALSIM_SIM_ONE_CHANGE_PIN:
                        MMIAPISET_ChangeSmartDualSIMPin(simID);
                        break;
                    case MMI_SMARTDUALSIM_SIM_ONE_VODAFONE_CARRIER:
                        MMISmartDS_VodafoneCarrierWin(simID);
                        break;
                    case MMI_SMARTDUALSIM_SIM_ONE_FIXED_DIALING:
                        MMIAPISET_SmartDualSIMopenFDNbySIM(simID);
                        break;
                    case MMI_SMARTDUALSIM_SIM_ONE_CHANGE_PIN2:
                        MMISET_OpenPin2Win(simID);
                        break;
#if defined(OMA_CP_SEC_SUPPORT)
                    case MMI_SMARTDUALSIM_SIM_ONE_CUSTOMER_PIN:
                        {
                            BOOLEAN enable = FALSE;

                            CTRLLIST_GetToggleState(MMI_SMARTDUALSIM_SIM_ONE_CTRL_ID, curItemIndex, &enable);
                            MMIAPISET_SetOTASecStatus(simID, enable);
                        }
                        break;
#endif
                    default:
                        break;
                }
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
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

LOCAL void MMISmartDS_setSimOnesItems(MN_DUAL_SYS_E simID, BOOLEAN isUpdate)
{
    MMI_CTRL_ID_T               ctrl_id = MMI_SMARTDUALSIM_SIM_ONE_CTRL_ID;
    GUILIST_ITEM_T              item_t = {0};
    MMISET_SIM_NAME_T           sim_name = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    MN_PHONE_SELECT_PLMN_T      sel_net_mode    =   {MN_PHONE_PLMN_SELECT_MANUAL_MODE, 0};
    MMISET_DUALSYS_TYPE_E       e_dualsys_setting = MMISET_DAULSYS_MAX;
    BOOLEAN                     toggleState = FALSE;
    uint16                      vodafoneCarrierSel = 0;
    uint16                      curItemIndex = 0;

    item_t.item_data_ptr = &item_data;
    if(simID >= MN_DUAL_SYS_MAX)
    {
        return;
    }

    if(!isUpdate)
    {
        GUILIST_SetMaxItem(ctrl_id, MMI_SMARTDUALSIM_SIM_ONE_ITEM_NUM,FALSE);
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
    }

    //name
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_SIM_LIST_NAME_EXT02;
    //todo:need to sync
    item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].is_opacity = TRUE;
    GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);
    sim_name = MMIAPISET_GetSimName(simID);
    item_data.item_content[1].item_data.text_buffer.wstr_len = sim_name.wstr_len;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = sim_name.wstr_arr;

    item_t.user_data    = MMI_SMARTDUALSIM_SIM_ONE_NAME;
    curItemIndex = 0;
    if(isUpdate)
    {
        GUILIST_ReplaceItem(ctrl_id, &item_t, curItemIndex);
    }
    else
    {
        GUILIST_InsertItem(ctrl_id, &item_t, curItemIndex);
    }

    //SIM Status
    item_t.item_style    = GUIITEM_STYLE_TOGGLE_TWO_LINE_TEXT_TEXT;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_CHANGE;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_SIM_LIST_STATUS_EXT02;
    //todo: need to sync
    item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].is_opacity = TRUE;
    GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);
    e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    if (e_dualsys_setting & (0x01 << simID))
    {
        item_data.item_content[1].item_data.text_id = STR_SIM_LIST_ONLINE_EXT02;
        toggleState = TRUE;
    }
    else
    {
        item_data.item_content[1].item_data.text_id = STR_SIM_LIST_OFFLINE_EXT02;
        toggleState = FALSE;
    }

    item_t.user_data    = MMI_SMARTDUALSIM_SIM_ONE_STATUS;
    curItemIndex++;
    if(isUpdate)
    {
        GUILIST_ReplaceItem(ctrl_id, &item_t, curItemIndex);
    }
    else
    {
        GUILIST_InsertItem(ctrl_id, &item_t, curItemIndex);
    }
    if(MMICC_GetCallNum()>0)
     {
        GUILIST_SetItemGray(ctrl_id, curItemIndex, TRUE);
        GUILIST_SetItemInvalid(ctrl_id,curItemIndex, TRUE);
        //toggleState = FALSE;	
     }else{
        GUILIST_SetItemGray(ctrl_id, curItemIndex, FALSE);
        GUILIST_SetItemInvalid(ctrl_id,curItemIndex, FALSE);
        //toggleState = TRUE;	
     }	 	
    CTRLLIST_SetToggleState(ctrl_id, curItemIndex, toggleState);

    //Network selection
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_LIST_NETWORK_SELECTION_EXT01;
    //todo: need to sync
    item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].is_opacity = TRUE;
    GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);
    MNPHONE_GetPlmnSelectModeEx(simID, &sel_net_mode);
    if(sel_net_mode.select_mode == MN_PHONE_PLMN_SELECT_AUTO_MODE)
    {
        item_data.item_content[1].item_data.text_id = STXT_AUTO_MODE;
    }
    else
    {
        item_data.item_content[1].item_data.text_id = STXT_MANUAL_MODE;
    }

    item_t.user_data    = MMI_SMARTDUALSIM_SIM_ONE_NETWORK_SELECT;
    curItemIndex++;
    if(isUpdate)
    {
        GUILIST_ReplaceItem(ctrl_id, &item_t, curItemIndex);
    }
    else
    {
        GUILIST_InsertItem(ctrl_id, &item_t, curItemIndex);
    }

    //pin request
    curItemIndex++;
    if(!isUpdate)
    {
    item_t.item_style    = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_CHANGE;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STXT_PIN_CODE_CNTRL;
    
    item_t.user_data    = MMI_SMARTDUALSIM_SIM_ONE_PIN_REQUEST;
    GUILIST_InsertItem(ctrl_id, &item_t, curItemIndex);
    }
    CTRLLIST_SetToggleState(ctrl_id, curItemIndex, MNSIM_IsPin1EnableEx(simID));

    //change pin
    curItemIndex++;
    if(!isUpdate)
    {
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STXT_PIN_CHANGE;
    
    item_t.user_data    = MMI_SMARTDUALSIM_SIM_ONE_CHANGE_PIN;
    GUILIST_InsertItem(ctrl_id, &item_t, curItemIndex);
    }

    //Vodafone carrier
if(1 == MMIAPISET_GetVodafoneSimConfig())
{
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_VODAFONE_EXT01;
    //todo: need to sync
    item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].is_opacity = TRUE;
    GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);

    vodafoneCarrierSel = MMIAPIPHONE_GetVodafoneSimSelectResult(simID, PNULL);
    if(vodafoneCarrierSel == 1)
    {
        item_data.item_content[1].item_data.text_id = STR_VODAFONE_CONTRACT_EXT01;
    }
    else if(vodafoneCarrierSel == 2)
    {
        item_data.item_content[1].item_data.text_id = STR_VODAFONE_PAYG_EXT01;
    }
    else
    {
        item_data.item_content[1].item_data.text_id = TXT_NULL;
    }

    item_t.user_data    = MMI_SMARTDUALSIM_SIM_ONE_VODAFONE_CARRIER;
    curItemIndex++;
    if(isUpdate)
    {
        GUILIST_ReplaceItem(ctrl_id, &item_t, curItemIndex);
    }
    else
    {
        GUILIST_InsertItem(ctrl_id, &item_t, curItemIndex);
    }
}

    //fixed dialing
    curItemIndex++;
    if(!isUpdate)
    {
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STXT_MENU_FDN;
    
    item_t.user_data    = MMI_SMARTDUALSIM_SIM_ONE_FIXED_DIALING;
    GUILIST_InsertItem(ctrl_id, &item_t, curItemIndex);
    }
    //change pin2
    curItemIndex++;
    if(!isUpdate)
    {
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STXT_PIN2_CHANGE;
    
    item_t.user_data    = MMI_SMARTDUALSIM_SIM_ONE_CHANGE_PIN2;
    GUILIST_InsertItem(ctrl_id, &item_t, curItemIndex);
    }

#if defined(OMA_CP_SEC_SUPPORT)
    //Customer pin
    curItemIndex++;
    if(!isUpdate)
    {
    item_t.item_style    = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_CHANGE;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_USER_PIN_REQUEST;
    
    item_t.user_data    = MMI_SMARTDUALSIM_SIM_ONE_CUSTOMER_PIN;
    GUILIST_InsertItem(ctrl_id, &item_t, curItemIndex);
    }
    CTRLLIST_SetToggleState(ctrl_id, curItemIndex, (MMIAPISET_GetOTASecStatus(simID)?TRUE:FALSE));
#endif
}

PUBLIC MMI_TEXT_ID_T MMISmartDS_getSimPLMNSelectModeStrId(MN_PHONE_PLMN_SELECT_MODE_E selectMode)
{
    if(MN_PHONE_PLMN_SELECT_MANUAL_MODE == selectMode)
    {
        return STXT_MANUAL_MODE;
    }
    else if(MN_PHONE_PLMN_SELECT_AUTO_MODE == selectMode)
    {
        return STXT_AUTO_MODE;
    }
    else
    {
        return TXT_NULL;
    }
}

LOCAL MMI_HANDLE_T MMISmartDS_SimNameWin(MN_DUAL_SYS_E simID)
{
    return MMK_CreateWin((uint32*)SMARTDUALSIM_SIM_NAME_EDIT_WIN_TAB, (ADD_DATA)simID);
}

LOCAL MMI_RESULT_E HandleSimNameWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           nameEditor = MMI_SMARTDUALSIM_SIM_NAME_EDIT_CTRL_ID;
    MMISET_SIM_NAME_T       simName = {0};
    MN_DUAL_SYS_E           simID = (MN_DUAL_SYS_E)MMK_GetWinAddDataPtr(win_id);

    MMI_STRING_T            nameInfo = {0};

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            GUI_BG_T  	backgrand = {0};
            GUI_COLOR_T font_color_ptr = MMI_DEFAULT_TEXT_COLOR;
            MMI_CTRL_ID_T   formCtrlId = MMI_SMARTDUALSIM_SIM_NAME_FORM_CTRL_ID;

            //style for form
            backgrand.bg_type = GUI_BG_COLOR;
            backgrand.color = MMI_BACKGROUND_COLOR;
            GUIFORM_SetStyle(formCtrlId,GUIFORM_STYLE_NORMAL);
            GUIFORM_SetBg(formCtrlId, &backgrand);
            GUIFORM_PermitChildFont(formCtrlId,FALSE);
            GUIFORM_PermitChildBorder(formCtrlId,FALSE);
            GUIFORM_PermitChildBg(formCtrlId,FALSE);

            //style for editor 1
            GUIEDIT_SetStyle(nameEditor, GUIEDIT_STYLE_MULTI_DYN_DOWN);
            GUIEDIT_SetIm(nameEditor,((GUIIM_TYPE_T)GUIIM_TYPE_ENGLISH|(GUIIM_TYPE_T)GUIIM_TYPE_ABC|(GUIIM_TYPE_T)GUIIM_TYPE_DIGITAL|(GUIIM_TYPE_T)GUIIM_TYPE_SMART),GUIIM_TYPE_ABC);
            GUIEDIT_SetFontColor(nameEditor, font_color_ptr);
            if(MN_DUAL_SYS_1 == simID)
            {
                CTRLBASEEDIT_SetLabelId(win_id, nameEditor, STR_ILIST_SIM_ONE, FALSE);
            }
            else
            {
                CTRLBASEEDIT_SetLabelId(win_id, nameEditor, STR_ILIST_SIM_TWO, FALSE);
            }
            //GUIEDIT_SetDefaultStringById(nameEditor, STR_LIST_SIM_NUMBER_EXT01);
            GUIEDIT_SetSoftkeyEx(nameEditor, LEFT_BUTTON, 0, TXT_NULL, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            GUIEDIT_SetSoftkeyEx(nameEditor, MIDDLE_BUTTON, 0, STXT_SOFTKEY_SAVE_MK, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            GUIEDIT_SetSoftkeyEx(nameEditor, RIGHT_BUTTON, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, PNULL, PNULL, EDIT_BUTTON_TEXT_ID,  PNULL);

            simName = MMIAPISET_GetSimName(simID);
            GUIEDIT_SetString(nameEditor, 
                simName.wstr_arr, simName.wstr_len);

            //set acitve the first editor.
            MMK_SetAtvCtrl(win_id, nameEditor);

        }
            break;
        case MSG_GET_FOCUS:
            break;
        case MSG_FULL_PAINT:
            if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_FWD_HDR_SMART_EXT02))
            {
                GUIWIN_UpdateStb();
            }
            break;
//        case MSG_APP_OK:
        case MSG_APP_WEB:
//        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
            GUIEDIT_GetString(nameEditor, &nameInfo);
            //if(nameInfo.wstr_len != 0)
            {
                simName.wstr_len = nameInfo.wstr_len;
                MMIAPICOM_Wstrncpy( simName.wstr_arr, nameInfo.wstr_ptr, simName.wstr_len );
                MMISET_SetSimName(simID, &simName);
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
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

LOCAL MMI_HANDLE_T MMISmartDS_VodafoneCarrierWin(MN_DUAL_SYS_E simID)
{
    return MMK_CreateWin((uint32*)SMARTDUALSIM_SIM_VODAFONE_CARRIER_WIN_TAB, (ADD_DATA)simID);
}

PUBLIC BOOLEAN MMIAPIPHONE_SetVodafoneSimSelectResult(uint32 sys_num,
													 uint16  cur_selection);
LOCAL MMI_RESULT_E HandleVodafoneCarrierWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E simID = (MN_DUAL_SYS_E)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMISmartDS_setSimVodafoneCarrierItems(simID, FALSE);
            MMK_SetAtvCtrl(win_id, MMI_SMARTDUALSIM_VODAFONE_CARRIER_LIST_CTRL_ID);
            break;

        case MSG_GET_FOCUS:
            break;

        case MSG_LOSE_FOCUS:
            break;

        case MSG_FULL_PAINT:
        {
            if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_HDR_VODAFONE_EXT01))
            {
                GUIWIN_UpdateStb();
            }
        }
            break;

        case MSG_APP_WEB:
//        case MSG_APP_OK:
//        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
            {
                uint16          curItemIndex = 0;
                MMI_MENU_ID_T menu_id = 0;
				uint16          vodafoneCarrierSel = 0;
                curItemIndex = GUILIST_GetCurItemIndex(MMI_SMARTDUALSIM_VODAFONE_CARRIER_LIST_CTRL_ID);
                GUILIST_GetItemData(MMI_SMARTDUALSIM_VODAFONE_CARRIER_LIST_CTRL_ID,curItemIndex, &menu_id);
				vodafoneCarrierSel = MMIAPIPHONE_GetVodafoneSimSelectResult(simID, PNULL);

				switch(menu_id)
                {
                    case MMI_SMARTDUALSIM_VODAFONE_UK_CONTRACT:
						//dong.chunguang add for reset default apn id when UK change
						if(vodafoneCarrierSel!=1)//not contract
						{
							MMICONNECTION_SetApnDefaultId(simID,0);//reset default apn id
						}
						//end
                        MMIAPIPHONE_SetVodafoneSimSelectResult(simID, 0);
		      //lulifei add to set vodafone begin
#ifdef MMI_GPRS_SUPPORT
		      SetVodafoneApn(simID, 0);
#endif
		      //lulifei add to set vodafone end
                        MMK_CloseWin(win_id);
                        break;
                    case MMI_SMARTDUALSIM_VODAFONE_UK_PAYG:
						//dong.chunguang add for reset default apn id when UK change
						if(vodafoneCarrierSel!=2)//not payg
						{
							MMICONNECTION_SetApnDefaultId(simID,0);//reset default apn id
						}
						//end
                        MMIAPIPHONE_SetVodafoneSimSelectResult(simID, 1);
		      //lulifei add to set vodafone begin
#ifdef MMI_GPRS_SUPPORT
		    SetVodafoneApn(simID, 1);
#endif
                      //lulifei add to set vodafone end
                        MMK_CloseWin(win_id);
                        break;
                    default:
                        break;
                }
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
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


LOCAL void MMISmartDS_setSimVodafoneCarrierItems(MN_DUAL_SYS_E simID, BOOLEAN isUpdate)
{
    MMI_CTRL_ID_T               ctrl_id = MMI_SMARTDUALSIM_VODAFONE_CARRIER_LIST_CTRL_ID;
    GUILIST_ITEM_T              item_t = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    uint16                      vodafoneCarrierSel = 0;

    item_t.item_data_ptr = &item_data;
    if(simID >= MN_DUAL_SYS_MAX)
    {
        return;
    }

    if(!isUpdate)
    {
        GUILIST_SetMaxItem(ctrl_id, MMI_SMARTDUALSIM_VODAFONE_UK_ITEM_NUM,FALSE);
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
    }

    vodafoneCarrierSel = MMIAPIPHONE_GetVodafoneSimSelectResult(simID, PNULL);

    //Vodafone UK Contract
    if(!isUpdate)
    {
    item_t.item_style    = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_VODAFONE_CONTRACT_EXT01;
    
    item_t.user_data    = MMI_SMARTDUALSIM_VODAFONE_UK_CONTRACT;
    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_VODAFONE_UK_CONTRACT);
    }

    //Vodafone UK Contract
    if(!isUpdate)
    {
    item_t.item_style    = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_VODAFONE_PAYG_EXT01;
    
    item_t.user_data    = MMI_SMARTDUALSIM_VODAFONE_UK_PAYG;
    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_VODAFONE_UK_PAYG);
    }

    if(1 == vodafoneCarrierSel)
    {
        GUILIST_SetSelectedItem(ctrl_id, MMI_SMARTDUALSIM_VODAFONE_UK_CONTRACT, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, MMI_SMARTDUALSIM_VODAFONE_UK_CONTRACT);
    }
    else if(2 == vodafoneCarrierSel)
    {
        GUILIST_SetSelectedItem(ctrl_id, MMI_SMARTDUALSIM_VODAFONE_UK_PAYG, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, MMI_SMARTDUALSIM_VODAFONE_UK_PAYG);
    }
}

LOCAL BOOLEAN MMISmartDS_SetSimStatus(MN_DUAL_SYS_E simID, BOOLEAN simOn)
{
    uint32 i = 0;
    MMISET_DUALSYS_TYPE_E   e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    BOOLEAN                 retCode = FALSE;
    LOCAL                   BOOLEAN s_is_standby[MMI_DUAL_SYS_MAX] = {0};

    if(simID >= MN_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (e_dualsys_setting & (0x01 << i))
        {
            s_is_standby[i] = TRUE;
        }
        else
        {
            s_is_standby[i] = FALSE;
        }
    }

//    if(s_is_standby[simID] != simOn)
    {
        if(FALSE == s_is_standby[simID])
        {
            if(!MMIAPIPHONE_GetIsPsDeactiveCnf((MN_DUAL_SYS_E)simID))
            {
                return FALSE;
            }
            s_is_standby[simID] = TRUE;
        }
        else    // active flymode
        {
            if (MMIAPIPHONE_GetSimExistedStatus(simID))
            {
                if (FALSE == MMIAPIPHONE_GetIsPsReady(simID))
                {
                    return FALSE;
                }
            }
            s_is_standby[simID] = FALSE;            
        }
        retCode = MMIAPISET_SetSmartDualSIMStandby(s_is_standby, simID);
    }

    return TRUE;
}

