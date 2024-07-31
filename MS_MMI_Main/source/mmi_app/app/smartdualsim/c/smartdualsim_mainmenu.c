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
#include "mmicom_banner.h"
#include "mmi_resource.h"
#include "guistring.h"
#include "mmipdp_export.h"

#include "smartdualsim_id.h"
#include "smartdualsim_text.h"
#include "smartdualsim_image.h"
#include "smartdualsim_menutable.h"
#include "smartdualsim_export.h"
#include "smartdualsim_internal.h"
#include "smartdualsim_nv.h"

LOCAL MMI_RESULT_E HandleMainMenuWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         );

LOCAL void MMISmartDS_setMainMenuItems(void);
LOCAL void MMISmartDS_updateMainMenuItems(void);

WINDOW_TABLE( SMARTDUALSIM_MAIN_MENU_WIN_TAB ) = 
{
    WIN_ID(MMI_SMARTDUALSIM_MAIN_MENU_WIN_ID),
    WIN_FUNC((uint32) HandleMainMenuWinMsg),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMI_SMARTDUALSIM_MAIN_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};


PUBLIC MMI_HANDLE_T MMISmartDS_MainMenuWin(void)
{
    MMK_CloseWin(MMI_SMARTDUALSIM_MAIN_MENU_WIN_ID);
    return MMK_CreateWin((uint32*)SMARTDUALSIM_MAIN_MENU_WIN_TAB, PNULL);
}

LOCAL MMI_RESULT_E HandleMainMenuWinMsg(
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
            MMISmartDS_setForwardFollowType(smartDS_Forward_follow_menuset);
            MMISmartDS_setMainMenuItems();
            MMK_SetAtvCtrl(win_id, MMI_SMARTDUALSIM_MAIN_MENU_CTRL_ID);

            break;

        case MSG_GET_FOCUS:
            break;

        case MSG_LOSE_FOCUS:
            break;

        case MSG_FULL_PAINT:
            if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_SETT_LIST_DUAL_SIM_EXT02))
            {
                GUIWIN_UpdateStb();
            }
            MMISmartDS_updateMainMenuItems();
            break;

        case MSG_APP_WEB:
//        case MSG_APP_OK:
//        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
        
            {
                MMI_MENU_ID_T menu_id = 0;
                MMI_MENU_GROUP_ID_T group_id = 0;

                menu_id = GUILIST_GetCurItemIndex(MMI_SMARTDUALSIM_MAIN_MENU_CTRL_ID);
                switch(menu_id)
                {
#if 0
                    case MMI_SMARTDUALSIM_CALL_FORWARDING:
                    {
                        GUI_POINT_T start_point = {0, 0};
                        MMICOM_BANNER_INFO_T banner_info = {0};
                        
                        banner_info.banner_type = MMI_BANNER_TEXT;
                        banner_info.start_point = start_point;
                        if(!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)
                            || !MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2)
                            )
                        {
                            banner_info.text1_id = STR_FWD_NOTE_TWO_SIM_EXT01;
                            MMIAPICOM_OpenBannerWin(win_id, banner_info);
                            break;
                        }
                        if (!MMIPHONE_IsSimOk(MN_DUAL_SYS_1) 
                            || !MMIPHONE_IsSimOk(MN_DUAL_SYS_2))
                        {
                            banner_info.text1_id = TXT_SIM_ISNOT_READY;
                            MMIAPICOM_OpenBannerWin(win_id, banner_info);
                            break;
                        }

                        getSIMNumber(MN_DUAL_SYS_1);
                        getSIMNumber(MN_DUAL_SYS_2);
                        if(simNumbers.sim_number1_len == 0
                            || simNumbers.sim_number2_len == 0)
                        {
                            banner_info.text1_id = STR_FWD_NOTE_SIM_NUMBER_EXT01;
                            MMIAPICOM_OpenBannerWin(win_id, banner_info);
                            break;
                        }
                    }
                    {
                        BOOLEAN forwardEnabled = FALSE;
                        BOOLEAN itemEnableState = FALSE;

                        MMI_ReadNVItem(MMI_SMARTDUALSIM_NV_ENABLED, (void *)&forwardEnabled);
                        CTRLLIST_GetToggleState(MMI_SMARTDUALSIM_MAIN_MENU_CTRL_ID, MMI_SMARTDUALSIM_CALL_FORWARDING, &itemEnableState);
                        if(forwardEnabled != itemEnableState)
                        {
                            forwardEnabled = itemEnableState;
                            MMI_WriteNVItem(MMI_SMARTDUALSIM_NV_ENABLED, (void *)&forwardEnabled);
                        }
                    }
                        break;
#endif
                    case MMI_SMARTDUALSIM_FORWARD_TYPE:
#ifndef WIN32
                    {
                        if(!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)
                            || !MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2)
                            )
                        {
                            MMISmartDS_displayBanner(win_id, STR_FWD_NOTE_TWO_SIM_EXT01);
                            break;
                        }
                        if (!MMIPHONE_IsSimOk(MN_DUAL_SYS_1) 
                            || !MMIPHONE_IsSimOk(MN_DUAL_SYS_2))
                        {
                            MMISmartDS_displayBanner(win_id, STR_SIM_NOT_READY_EXT01);
                            break;
                        }

                        getSIMNumber(MN_DUAL_SYS_1);
                        getSIMNumber(MN_DUAL_SYS_2);
                        if(simNumbers.sim_number1_len == 0
                            || simNumbers.sim_number2_len == 0)
                        {
                            MMISmartDS_displayBanner(win_id, STR_FWD_NOTE_SIM_NUMBER_EXT01);
                            break;
                        }
                    }
#endif
                        MMISmartDS_ForwardTypeSelectWin(win_id);
                        break;
                    case MMI_SMARTDUALSIM_SIM_NUMBERS:
                        MMISmartDS_SimNumbersWin(win_id);
                        break;
                    case MMI_SMARTDUALSIM_SIM_SETTINGS:
                        MMISmartDS_SimSettingsWin();
                        break;
                    case MMI_SMARTDUALSIM_DATA_CONNECTION:
                        MMISmartDS_DataConnectionWin();
                        break;
                    case MMI_SMARTDUALSIM_CALL_PREFERRED:
                        MMISmartDS_CallPreferredSimWin(FALSE);
                        break;
                    case MMI_SMARTDUALSIM_SMS_PREFERRED:
                        MMISmartDS_SMSPreferredSimWin(FALSE);
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
LOCAL BOOLEAN MMISmartDS_CallForwardSimStatus(void)
{
    if(!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)
        || !MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {
        return FALSE;
    }

    if(!GetIsSameCard(MN_DUAL_SYS_1) || !GetIsSameCard(MN_DUAL_SYS_2))
    {
        getSIMNumber(MN_DUAL_SYS_1);
        getSIMNumber(MN_DUAL_SYS_2);
        if(simNumbers.sim_number1_len == 0
            || simNumbers.sim_number2_len == 0)
        {
            return FALSE;
        }
    }

    return TRUE;
}
LOCAL void MMISmartDS_setMainMenuItems(void)
{
    MMI_CTRL_ID_T               ctrl_id = MMI_SMARTDUALSIM_MAIN_MENU_CTRL_ID;
    SmartDS_Forwarding_Direction forwardType = SMARTDS_FORWARDING_NONE;
    GUILIST_ITEM_T              item_t = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    BOOLEAN forwardEnabled = FALSE;
    item_t.item_data_ptr = &item_data;

    GUILIST_SetMaxItem(ctrl_id, MMI_SMARTDUALSIM_MAINMENU_ITEM_NUM,FALSE);
    GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);

    //set item "Call forwarding"
//    item_t.item_style    = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
    
//    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
//    item_data.item_content[0].item_data.text_id = STXT_CALL_DIVERT;

//    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_CALL_FORWARDING);
    //todo: it must to async the true state
//    MMI_ReadNVItem(MMI_SMARTDUALSIM_NV_ENABLED, (void *)&forwardEnabled);
//    CTRLLIST_SetToggleState(ctrl_id, MMI_SMARTDUALSIM_CALL_FORWARDING, forwardEnabled);

    //set item "Forward types"
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[0].item_data.text_id = STR_FWD_LIST_FORWARD_EXT02;
    //todo: it must to async the true state
    item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[1].is_opacity = TRUE;
    GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);
    if(!MMISmartDS_CallForwardSimStatus())
    {
     SCI_TraceLow(
            "smartDS MMISmartDS_setMainMenuItems call forward sim status is FASLE");
        MMI_WriteNVItem(MMI_SMARTDUALSIM_NV_FORWARDING_DIRECTION, (void *)&forwardType);
    }
    else
    {
        MMI_ReadNVItem(MMI_SMARTDUALSIM_NV_FORWARDING_DIRECTION, (void *)&forwardType);
    }
        SCI_TraceLow(
            "smartDS MMISmartDS_setMainMenuItems open forwardType=%d",
        forwardType);
    if(SMARTDS_FORWARDING_BOTH_SIM == forwardType)
    {
        item_data.item_content[1].item_data.text_id = STR_FWD_LIST_BTW_SIM_EXT02;
    }
    else if(SMARTDS_FORWARDING_1_TO_2 == forwardType)
    {
        item_data.item_content[1].item_data.text_id = STR_FWD_LIST_TO_SIM2_EXT02;
    }
    else if(SMARTDS_FORWARDING_2_TO_1 == forwardType)
    {
        item_data.item_content[1].item_data.text_id = STR_FWD_LIST_TO_SIM1_EXT02;
    }
    else
    {
        //forwardType = SMARTDS_FORWARDING_NONE;
        //MMI_WriteNVItem(MMI_SMARTDUALSIM_NV_FORWARDING_DIRECTION, (void *)&forwardType);
        item_data.item_content[1].item_data.text_id = STR_FWD_LIST_OFF_EXT01;
    }
    
    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_FORWARD_TYPE);

    //set item "SIM numbers"
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[0].item_data.text_id = STR_FWD_LIST_NUM_EXT02;
    
    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_SIM_NUMBERS);

    //set item "SIM settings"
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[0].item_data.text_id = STR_SIM_LIST_SETT_EXT02;
    
    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_SIM_SETTINGS);

    //set item "Call settings"
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_LIST_PRE_SIM_CALL;
    
    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_CALL_PREFERRED);

    //set item "SMS settings"
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_LIST_PRE_SIM_MSG;
    
    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_SMS_PREFERRED);

    //set item "SIM settings"
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[0].item_data.text_id = STR_WMLS_SETT_CONN_BEHAVIOR;
    
    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_DATA_CONNECTION);

    if((!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)
        && !MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))||MMIAPISET_GetFlyMode())
    {
        GUILIST_SetItemGray(ctrl_id, MMI_SMARTDUALSIM_FORWARD_TYPE, TRUE);
        GUILIST_SetItemInvalid(ctrl_id, MMI_SMARTDUALSIM_FORWARD_TYPE, TRUE);
        
        GUILIST_SetItemGray(ctrl_id, MMI_SMARTDUALSIM_SIM_NUMBERS, TRUE);
        GUILIST_SetItemInvalid(ctrl_id, MMI_SMARTDUALSIM_SIM_NUMBERS, TRUE);
        
        GUILIST_SetItemGray(ctrl_id, MMI_SMARTDUALSIM_SIM_SETTINGS, TRUE);
        GUILIST_SetItemInvalid(ctrl_id, MMI_SMARTDUALSIM_SIM_SETTINGS, TRUE);
        
        GUILIST_SetItemGray(ctrl_id, MMI_SMARTDUALSIM_DATA_CONNECTION, TRUE);
        GUILIST_SetItemInvalid(ctrl_id, MMI_SMARTDUALSIM_DATA_CONNECTION, TRUE);
    }

    if(!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)
        || !MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {
        GUILIST_SetItemGray(ctrl_id, MMI_SMARTDUALSIM_CALL_PREFERRED, TRUE);
        GUILIST_SetItemInvalid(ctrl_id, MMI_SMARTDUALSIM_CALL_PREFERRED, TRUE);
        
        GUILIST_SetItemGray(ctrl_id, MMI_SMARTDUALSIM_SMS_PREFERRED, TRUE);
        GUILIST_SetItemInvalid(ctrl_id, MMI_SMARTDUALSIM_SMS_PREFERRED, TRUE);
    }


}

LOCAL void MMISmartDS_updateMainMenuItems(void)
{
    GUIITEM_CONTENT_T itemForwardType = {0};
    SmartDS_Forwarding_Direction forwardType = SMARTDS_FORWARDING_NONE;
    GUILIST_ITEM_T          item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data   =   {0};/*lint !e64*/
    BOOLEAN forwardEnabled = FALSE;

//update enabled
//    MMI_ReadNVItem(MMI_SMARTDUALSIM_NV_ENABLED, (void *)&forwardEnabled);
//    CTRLLIST_SetToggleState(MMI_SMARTDUALSIM_MAIN_MENU_CTRL_ID, MMI_SMARTDUALSIM_CALL_FORWARDING, forwardEnabled);

//update forward types
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[0].item_data.text_id = STR_FWD_LIST_FORWARD_EXT02;

    //todo: it must to async the true state
    item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[1].is_opacity = TRUE;
    GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);
    MMI_ReadNVItem(MMI_SMARTDUALSIM_NV_FORWARDING_DIRECTION, (void *)&forwardType);
        SCI_TraceLow(
            "smartDS MMISmartDS_updateMainMenuItems open forwardType=%d",
        forwardType);
    if(SMARTDS_FORWARDING_BOTH_SIM == forwardType)
    {
        item_data.item_content[1].item_data.text_id = STR_FWD_LIST_BTW_SIM_EXT02;
    }
    else if(SMARTDS_FORWARDING_1_TO_2 == forwardType)
    {
        item_data.item_content[1].item_data.text_id = STR_FWD_LIST_TO_SIM2_EXT02;
    }
    else if(SMARTDS_FORWARDING_2_TO_1 == forwardType)
    {
        item_data.item_content[1].item_data.text_id = STR_FWD_LIST_TO_SIM1_EXT02;
    }
    else
    {
        item_data.item_content[1].item_data.text_id = STR_FWD_LIST_OFF_EXT01;
    }
    GUILIST_ReplaceItem(MMI_SMARTDUALSIM_MAIN_MENU_CTRL_ID, &item_t, MMI_SMARTDUALSIM_FORWARD_TYPE);
//  GUILIST_SetItemContent(MMI_SMARTDUALSIM_MAIN_MENU_CTRL_ID, 
//                      &itemForwardType,
//                      MMI_SMARTDUALSIM_FORWARD_TYPE,
//                      1);
    if(!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)
        && !MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {
        GUILIST_SetItemGray(MMI_SMARTDUALSIM_MAIN_MENU_CTRL_ID, MMI_SMARTDUALSIM_FORWARD_TYPE, TRUE);
        GUILIST_SetItemInvalid(MMI_SMARTDUALSIM_MAIN_MENU_CTRL_ID, MMI_SMARTDUALSIM_FORWARD_TYPE, TRUE);
    }

}

