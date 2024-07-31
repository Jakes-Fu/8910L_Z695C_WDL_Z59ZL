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
#include "mmicc_export.h"
#include "mmicc_id.h"
#include "mmiota_image.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmk_msg.h"
#include "guiwin.h"
#include "mmiconnection_export.h"

#include "mmipdp_internal.h"
#include "mmiphone_export.h"

#include "mmi_resource.h"
#include "guistring.h"
#include "mmipdp_export.h"
#include "mmiset_export.h"
#include "mmiset_multisim_internal.h"

#include "smartdualsim_id.h"
#include "smartdualsim_text.h"
#include "smartdualsim_image.h"
#include "smartdualsim_menutable.h"
#include "smartdualsim_internal.h"
#include "smartdualsim_export.h"

LOCAL MMI_RESULT_E HandleDataConnectionWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         );
PUBLIC void MMISmartDS_setDataConnectionItems(BOOLEAN isUpdate);

LOCAL MMI_RESULT_E HandlePreferredSimWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         );

LOCAL MMI_RESULT_E HandleCallPreferredSimWinMsg(
                                            MMI_WIN_ID_T     win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM           param
                                            );

LOCAL MMI_RESULT_E HandleSMSPreferredSimWinMsg(
                                            MMI_WIN_ID_T     win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM           param
                                            );

PUBLIC void MMISmartDS_setPreferredSimItems(void);
PUBLIC void MMISmartDS_setCallPreferredSimItems(void);
PUBLIC void MMISmartDS_setSMSPreferredSimItems(void);
LOCAL BOOLEAN MMISmartDS_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_TYPE_E e_func_type, MMISET_MAJOR_SIM_SEL_TYPE_E e_sim_sel_type);


WINDOW_TABLE( SMARTDUALSIM_DATA_CONNECTION_WIN_TAB ) = 
{
    WIN_ID(MMI_SMARTDUALSIM_DATA_CONNECTION_WIN_ID),
    WIN_FUNC((uint32) HandleDataConnectionWinMsg),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMI_SMARTDUALSIM_DATA_CONNECTION_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE( SMARTDUALSIM_PREFERRED_SIM_WIN_TAB ) = 
{
    WIN_ID(MMI_SMARTDUALSIM_PREFERRED_SIM_WIN_ID),
    WIN_FUNC((uint32) HandlePreferredSimWinMsg),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMI_SMARTDUALSIM_PREFERRED_SIM_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE( SMARTDUALSIM_CALL_PREFERRED_SIM_WIN_TAB ) = 
{
    WIN_ID(MMI_SMARTDUALSIM_CALL_PREFERRED_SIM_WIN_ID),
        WIN_FUNC((uint32) HandleCallPreferredSimWinMsg),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMI_SMARTDUALSIM_CALL_PREFERRED_SIM_CTRL_ID),
        WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
        END_WIN
};

WINDOW_TABLE( SMARTDUALSIM_SMS_PREFERRED_SIM_WIN_TAB ) = 
{
    WIN_ID(MMI_SMARTDUALSIM_SMS_PREFERRED_SIM_WIN_ID),
        WIN_FUNC((uint32) HandleSMSPreferredSimWinMsg),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMI_SMARTDUALSIM_SMS_PREFERRED_SIM_CTRL_ID),
        WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
        END_WIN
};

PUBLIC MMI_HANDLE_T MMISmartDS_DataConnectionWin(void)
{
    return MMK_CreateWin((uint32*)SMARTDUALSIM_DATA_CONNECTION_WIN_TAB, PNULL);
}

LOCAL MMI_RESULT_E HandleDataConnectionWinMsg(
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
            {
                MMI_TEXT_ID_T *textID = (MMI_TEXT_ID_T*)MMK_GetWinAddDataPtr(win_id);
            }
            MMISmartDS_setDataConnectionItems(FALSE);
            MMK_SetAtvCtrl(win_id, MMI_SMARTDUALSIM_DATA_CONNECTION_CTRL_ID);
            break;

        case MSG_GET_FOCUS:
            break;

        case MSG_LOSE_FOCUS:
            break;

        case MSG_FULL_PAINT:
            MMISmartDS_setDataConnectionItems(TRUE);
            if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_WMLS_SETT_CONN_BEHAVIOR))
            {
                GUIWIN_UpdateStb();
            }
            break;

        case MSG_APP_WEB:
//        case MSG_APP_OK:
//        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
        
            {
                MMI_MENU_ID_T menu_id = 0;
                MMI_MENU_GROUP_ID_T group_id = 0;

                //GUIMENU_GetId(MMI_SMARTDUALSIM_DATA_CONNECTION_CTRL_ID, &group_id, &menu_id);
                menu_id = GUILIST_GetCurItemIndex(MMI_SMARTDUALSIM_DATA_CONNECTION_CTRL_ID);
                switch(menu_id)
                {
                    case MMI_SMARTDUALSIM_MOBILE_DATA_SWITCH:
                        if (MMIAPICONNECTION_GetGPRSSwitchStatus() == MMICONNECTION_SETTING_GPRS_SWITCH_ON)
                        {
                            MMIAPICONNECTION_SetGPRSSwitchStatus(MMICONNECTION_SETTING_GPRS_SWITCH_OFF);
                            CTRLLIST_SetToggleState(MMI_SMARTDUALSIM_DATA_CONNECTION_CTRL_ID,MMI_SMARTDUALSIM_MOBILE_DATA_SWITCH,FALSE);
                        }
                        else
                        {
                            MMIAPICONNECTION_SetGPRSSwitchStatus(MMICONNECTION_SETTING_GPRS_SWITCH_ON);
                            CTRLLIST_SetToggleState(MMI_SMARTDUALSIM_DATA_CONNECTION_CTRL_ID,MMI_SMARTDUALSIM_MOBILE_DATA_SWITCH,TRUE);
                        }
                        break;
                    case MMI_SMARTDUALSIM_PREFEREED_SIM:
				MMISmartDS_PreferredSimWin(FALSE);
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

PUBLIC void MMISmartDS_setDataConnectionItems(BOOLEAN isUpdate)
{
    MMI_CTRL_ID_T               ctrl_id = MMI_SMARTDUALSIM_DATA_CONNECTION_CTRL_ID;
    GUILIST_ITEM_T              item_t = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    MMISET_MAJOR_SIM_SEL_TYPE_E e_major_sim_type    =   0;
    BOOLEAN                     bSim1Existed = MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1);
    BOOLEAN                     bSim2Existed = MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2);

    item_t.item_data_ptr = &item_data;
    e_major_sim_type = MMISET_GetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_WAP);

    if(!isUpdate)
    {
        GUILIST_SetMaxItem(ctrl_id, MMI_SMARTDUALSIM_DATACONNECTION_ITEM_NUM,FALSE);
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
    }

    //set item "data switch"
    item_t.item_style    = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_CHANGE;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[0].item_data.text_id = STR_LIST_MOBILE_DATA;
    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_MOBILE_DATA_SWITCH);
    CTRLLIST_SetToggleState(ctrl_id,MMI_SMARTDUALSIM_MOBILE_DATA_SWITCH,(MMIAPICONNECTION_GetGPRSSwitchStatus() == MMICONNECTION_SETTING_GPRS_SWITCH_ON)?TRUE:FALSE);

    //set item "Forward calls"
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[0].item_data.text_id = STR_SIM_LIST_PRE_SIM_EXT02;
    //todo: it must to async the true state
    item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].is_opacity = TRUE;
    GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);
    if((e_major_sim_type == MMISET_MAJOR_SIM_SEL_SIM1 && bSim1Existed)
        || !bSim2Existed)
    {
        item_data.item_content[1].item_data.text_id = STR_ILIST_SIM_ONE;
    }
    else if((e_major_sim_type == MMISET_MAJOR_SIM_SEL_SIM2 && bSim2Existed)
        || !bSim1Existed)
    {
        item_data.item_content[1].item_data.text_id = STR_ILIST_SIM_TWO;
    }
// #ifndef SMARTDS_DISABLE_ASK_EVERY_TIME
//     else if(e_major_sim_type == MMISET_MAJOR_SIM_SEL_MANUAL_SEL && bSim2Existed && bSim1Existed)
//     {
//         item_data.item_content[1].item_data.text_id = STR_DSSET_DSDS_ASKEVERYTIME;
//     }
// #endif
    else
    {
        item_data.item_content[1].item_data.text_id = STR_ILIST_SIM_ONE;
    }

    if(isUpdate)
    {
        GUILIST_ReplaceItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_PREFEREED_SIM);
    }
    else
    {
        GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_PREFEREED_SIM);
    }

    if(!bSim1Existed
        || !bSim2Existed)
    {
        GUILIST_SetItemGray(ctrl_id, MMI_SMARTDUALSIM_PREFEREED_SIM, TRUE);
        GUILIST_SetItemInvalid(ctrl_id, MMI_SMARTDUALSIM_PREFEREED_SIM, TRUE);
    }
    else if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        GUILIST_SetItemGray(ctrl_id, MMI_SMARTDUALSIM_PREFEREED_SIM, TRUE);
        GUILIST_SetItemInvalid(ctrl_id, MMI_SMARTDUALSIM_PREFEREED_SIM, TRUE);	
    }
    else
    {
        GUILIST_SetItemGray(ctrl_id, MMI_SMARTDUALSIM_PREFEREED_SIM, FALSE);
        GUILIST_SetItemInvalid(ctrl_id, MMI_SMARTDUALSIM_PREFEREED_SIM, FALSE);
    }
}

LOCAL BOOLEAN isOpenByBoot = FALSE;

PUBLIC MMI_HANDLE_T MMISmartDS_PreferredSimWin(BOOLEAN isByBoot)
{
    isOpenByBoot = isByBoot;

    MMK_CloseWin(MMI_SMARTDUALSIM_PREFERRED_SIM_WIN_ID);

    return MMK_CreateWin((uint32*)SMARTDUALSIM_PREFERRED_SIM_WIN_TAB, PNULL);
}

LOCAL MMI_RESULT_E HandlePreferredSimWinMsg(
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
            {
                MMI_TEXT_ID_T *textID = (MMI_TEXT_ID_T*)MMK_GetWinAddDataPtr(win_id);
            }
            MMISmartDS_setPreferredSimItems();
            MMK_SetAtvCtrl(win_id, MMI_SMARTDUALSIM_PREFERRED_SIM_CTRL_ID);
            break;

        case MSG_GET_FOCUS:
            break;

        case MSG_LOSE_FOCUS:
            break;

        case MSG_FULL_PAINT:
            if(isOpenByBoot == TRUE)
            {
                MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CONNECTIVITY_HDR_GPRS);
                GUIWIN_UpdateStb();
                break;
            }
            
            if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_SIM_HDR_PRE_SIM_EXT01))
            {
                GUIWIN_UpdateStb();
            }
            break;

        case MSG_APP_WEB:
//        case MSG_APP_OK:
//        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
            {
                MMI_MENU_ID_T menu_id = 0;
                MMI_MENU_GROUP_ID_T group_id = 0;

                //GUIMENU_GetId(MMI_SMARTDUALSIM_PREFERRED_SIM_CTRL_ID, &group_id, &menu_id);
                menu_id = GUILIST_GetCurItemIndex(MMI_SMARTDUALSIM_PREFERRED_SIM_CTRL_ID);
                switch(menu_id)
                {
                    case MMI_SMARTDUALSIM_PREFERRED_SIM1: 
                        //MMIAPISET_SetGPRSModeType(DATA_CARD_MODE_1);
                        MMISmartDS_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_WAP, MMISET_MAJOR_SIM_SEL_SIM1);
#ifdef MMI_SHOW_MSA
                        if(isOpenByBoot == TRUE)
                        {
                            MMIAPIHONE_CreateMicrosoftAccoutWindow();
                        }
#endif
                        MMK_CloseWin(win_id);
                        break;
                    case MMI_SMARTDUALSIM_PREFERRED_SIM2:
                        //MMIAPISET_SetGPRSModeType(DATA_CARD_MODE_2);
                        MMISmartDS_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_WAP, MMISET_MAJOR_SIM_SEL_SIM2);
#ifdef MMI_SHOW_MSA
                        if(isOpenByBoot == TRUE)
                        {
                            MMIAPIHONE_CreateMicrosoftAccoutWindow();
                        }
#endif
                        MMK_CloseWin(win_id);
                        break;
                #ifndef SMARTDS_DISABLE_ASK_EVERY_TIME
                    case MMI_SMARTDUALSIM_ASK_EVERY_TIME:
                        MMISmartDS_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_WAP, MMISET_MAJOR_SIM_SEL_MANUAL_SEL);
                        MMK_CloseWin(win_id);
                        break;
                #endif
                    default:
                        break;
                }
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            if(isOpenByBoot == TRUE)
            {
                break;
            }
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
            break;

        case MSG_APP_RED:
        case MSG_KEYUP_RED:
            if(isOpenByBoot == TRUE)
            {
                break;
            }
            recode = MMI_RESULT_FALSE;
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

PUBLIC void MMISmartDS_setPreferredSimItems(void)
{
    MMI_CTRL_ID_T               ctrl_id = MMI_SMARTDUALSIM_PREFERRED_SIM_CTRL_ID;
    GUILIST_ITEM_T              item_t = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    MMISET_MAJOR_SIM_SEL_TYPE_E e_major_sim_type    =   0;

    item_t.item_data_ptr = &item_data;
    e_major_sim_type = MMISET_GetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_WAP);

    GUILIST_SetMaxItem(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_ITEM_NUM,FALSE);
    //GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);

    //set item "SIM1"
    item_t.item_style = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    if(isOpenByBoot == FALSE)
    {
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    }
    else
    {
    item_data.softkey_id[2] = TXT_NULL;
    }
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_ILIST_SIM_ONE;

    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_PREFERRED_SIM1);
    if(e_major_sim_type == MMISET_MAJOR_SIM_SEL_SIM1)
    {
        GUILIST_SetSelectedItem(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM1, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM1);
        //CTRLLIST_SetToggleState(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM1, TRUE);
    }

    //set item "SIM2"
    item_t.item_style = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    if(isOpenByBoot == FALSE)
    {
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    }
    else
    {
    item_data.softkey_id[2] = TXT_NULL;
    }
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_ILIST_SIM_TWO;
    
    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_PREFERRED_SIM2);
    if(e_major_sim_type == MMISET_MAJOR_SIM_SEL_SIM2)
    {
        GUILIST_SetSelectedItem(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM2, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM2);
        //CTRLLIST_SetToggleState(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM2, TRUE);
    }

// #ifndef SMARTDS_DISABLE_ASK_EVERY_TIME
//     //set item "Ask everytime"
//     item_t.item_style    = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
//     item_data.softkey_id[0] = IMAGE_NULL;
//     item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
//     item_data.softkey_id[2] = IMAGE_COMMON_BACK;
//     
//     item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
//     item_data.item_content[0].item_data.text_id = STR_DSSET_DSDS_ASKEVERYTIME;
// 
//     GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_ASK_EVERY_TIME);
// 
//     //todo: it must to async the true state
//     if(e_major_sim_type == MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
//     {
//         GUILIST_SetSelectedItem(ctrl_id, MMI_SMARTDUALSIM_ASK_EVERY_TIME, TRUE);
//         GUILIST_SetCurItemIndex(ctrl_id, MMI_SMARTDUALSIM_ASK_EVERY_TIME);
//         //CTRLLIST_SetToggleState(ctrl_id, MMI_SMARTDUALSIM_ASK_EVERY_TIME, TRUE);
//     }
// #endif

    GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
}

LOCAL BOOLEAN MMISmartDS_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_TYPE_E e_func_type, MMISET_MAJOR_SIM_SEL_TYPE_E e_sim_sel_type)
{
    MMISET_MAJOR_SIM_SEL_TYPE_E         e_major_sim_type    =   0;

    if(e_func_type >= MMISET_MAJOR_SIM_SEL_FUNC_MAX)
    {
         return FALSE;
    }

    e_major_sim_type = MMISET_GetMajorSimSelType(e_func_type);

    if(e_sim_sel_type < MMISET_MAJOR_SIM_SEL_TYPE_MAX)
    {
        if (e_sim_sel_type != e_major_sim_type)
        {
            //release current pdp with the other SIM
            if(e_sim_sel_type == MMISET_MAJOR_SIM_SEL_SIM1
                && MMIPDP_PsIsActivedPdpLinkExist(MN_DUAL_SYS_2))
            {
                MMIAPIPDP_ReleaseAll(NULL);
            }
            else if(e_sim_sel_type == MMISET_MAJOR_SIM_SEL_SIM2
                && MMIPDP_PsIsActivedPdpLinkExist(MN_DUAL_SYS_1))
            {
                MMIAPIPDP_ReleaseAll(NULL);
            }

            //save user selection
            return MMISET_SetMajorSimSelType(e_func_type, e_sim_sel_type);
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        return FALSE;
    }
}

PUBLIC MMI_HANDLE_T MMISmartDS_CallPreferredSimWin(BOOLEAN isByBoot)
{
    isOpenByBoot = isByBoot;

    MMK_CloseWin(MMI_SMARTDUALSIM_CALL_PREFERRED_SIM_WIN_ID);

    return MMK_CreateWin((uint32*)SMARTDUALSIM_CALL_PREFERRED_SIM_WIN_TAB, PNULL);
}

LOCAL MMI_RESULT_E HandleCallPreferredSimWinMsg(
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
            {
                MMI_TEXT_ID_T *textID = (MMI_TEXT_ID_T*)MMK_GetWinAddDataPtr(win_id);
            }
            MMISmartDS_setCallPreferredSimItems();
            MMK_SetAtvCtrl(win_id, MMI_SMARTDUALSIM_CALL_PREFERRED_SIM_CTRL_ID);
            break;

        case MSG_GET_FOCUS:
            break;

        case MSG_LOSE_FOCUS:
            break;

        case MSG_FULL_PAINT:
            if(isOpenByBoot == TRUE)
            {
                MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_HDR_PRE_SIM_CALL);
                GUIWIN_UpdateStb();
                break;
            }
            
            if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_SIM_HDR_PRE_SIM_EXT01))
            {
                GUIWIN_UpdateStb();
            }
            break;

        case MSG_APP_WEB:
//        case MSG_APP_OK:
//        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
            {
                MMI_MENU_ID_T menu_id = 0;
                MMI_MENU_GROUP_ID_T group_id = 0;

                //GUIMENU_GetId(MMI_SMARTDUALSIM_PREFERRED_SIM_CTRL_ID, &group_id, &menu_id);
                menu_id = GUILIST_GetCurItemIndex(MMI_SMARTDUALSIM_CALL_PREFERRED_SIM_CTRL_ID);
                switch(menu_id)
                {
                    case MMI_SMARTDUALSIM_PREFERRED_SIM1: 
                        //MMIAPISET_SetGPRSModeType(DATA_CARD_MODE_1);
                        MMISmartDS_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_CALL, MMISET_MAJOR_SIM_SEL_SIM1);
#ifdef MMI_SHOW_MSA
                        if(isOpenByBoot == TRUE)
                        {
                            MMIAPIHONE_CreateMicrosoftAccoutWindow();
                        }
#endif
                        MMK_CloseWin(win_id);
                        break;
                    case MMI_SMARTDUALSIM_PREFERRED_SIM2:
                        //MMIAPISET_SetGPRSModeType(DATA_CARD_MODE_2);
                        MMISmartDS_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_CALL, MMISET_MAJOR_SIM_SEL_SIM2);
#ifdef MMI_SHOW_MSA
                        if(isOpenByBoot == TRUE)
                        {
                            MMIAPIHONE_CreateMicrosoftAccoutWindow();
                        }
#endif
                        MMK_CloseWin(win_id);
                        break;
                #ifndef SMARTDS_DISABLE_ASK_EVERY_TIME
                    case MMI_SMARTDUALSIM_ASK_EVERY_TIME:
                        MMISmartDS_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_CALL, MMISET_MAJOR_SIM_SEL_MANUAL_SEL);
                        MMK_CloseWin(win_id);
                        break;
                #endif
                    default:
                        break;
                }
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            if(isOpenByBoot == TRUE)
            {
                break;
            }
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
            break;

        case MSG_APP_RED:
        case MSG_KEYUP_RED:
            if(isOpenByBoot == TRUE)
            {
                break;
            }
            recode = MMI_RESULT_FALSE;
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

PUBLIC void MMISmartDS_setCallPreferredSimItems(void)
{
    MMI_CTRL_ID_T               ctrl_id = MMI_SMARTDUALSIM_CALL_PREFERRED_SIM_CTRL_ID;
    GUILIST_ITEM_T              item_t = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    MMISET_MAJOR_SIM_SEL_TYPE_E e_major_sim_type    =   0;

    item_t.item_data_ptr = &item_data;
    e_major_sim_type = MMISET_GetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_CALL);

    GUILIST_SetMaxItem(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_ITEM_NUM,FALSE);
    //GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);

    //set item "SIM1"
    item_t.item_style = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    if(isOpenByBoot == FALSE)
    {
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    }
    else
    {
    item_data.softkey_id[2] = TXT_NULL;
    }
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_ILIST_SIM_ONE;

    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_PREFERRED_SIM1);
    if(e_major_sim_type == MMISET_MAJOR_SIM_SEL_SIM1)
    {
        GUILIST_SetSelectedItem(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM1, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM1);
        //CTRLLIST_SetToggleState(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM1, TRUE);
    }

    //set item "SIM2"
    item_t.item_style = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    if(isOpenByBoot == FALSE)
    {
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    }
    else
    {
    item_data.softkey_id[2] = TXT_NULL;
    }
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_ILIST_SIM_TWO;
    
    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_PREFERRED_SIM2);
    if(e_major_sim_type == MMISET_MAJOR_SIM_SEL_SIM2)
    {
        GUILIST_SetSelectedItem(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM2, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM2);
        //CTRLLIST_SetToggleState(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM2, TRUE);
    }

#ifndef SMARTDS_DISABLE_ASK_EVERY_TIME
    //set item "Ask everytime"
    item_t.item_style    = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[0].item_data.text_id = STR_DSSET_DSDS_ASKEVERYTIME;

    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_ASK_EVERY_TIME);

    //todo: it must to async the true state
    if(e_major_sim_type == MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
    {
        GUILIST_SetSelectedItem(ctrl_id, MMI_SMARTDUALSIM_ASK_EVERY_TIME, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, MMI_SMARTDUALSIM_ASK_EVERY_TIME);
        //CTRLLIST_SetToggleState(ctrl_id, MMI_SMARTDUALSIM_ASK_EVERY_TIME, TRUE);
    }
#endif

    GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
}

PUBLIC MMI_HANDLE_T MMISmartDS_SMSPreferredSimWin(BOOLEAN isByBoot)
{
    isOpenByBoot = isByBoot;

    MMK_CloseWin(MMI_SMARTDUALSIM_SMS_PREFERRED_SIM_WIN_ID);

    return MMK_CreateWin((uint32*)SMARTDUALSIM_SMS_PREFERRED_SIM_WIN_TAB, PNULL);
}

LOCAL MMI_RESULT_E HandleSMSPreferredSimWinMsg(
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
            {
                MMI_TEXT_ID_T *textID = (MMI_TEXT_ID_T*)MMK_GetWinAddDataPtr(win_id);
            }
            MMISmartDS_setSMSPreferredSimItems();
            MMK_SetAtvCtrl(win_id, MMI_SMARTDUALSIM_SMS_PREFERRED_SIM_CTRL_ID);
            break;

        case MSG_GET_FOCUS:
            break;

        case MSG_LOSE_FOCUS:
            break;

        case MSG_FULL_PAINT:
            if(isOpenByBoot == TRUE)
            {
                MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_HDR_PRE_SIM_MSG);
                GUIWIN_UpdateStb();
                break;
            }
            
            if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_SIM_HDR_PRE_SIM_EXT01))
            {
                GUIWIN_UpdateStb();
            }
            break;

        case MSG_APP_WEB:
//        case MSG_APP_OK:
//        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
            {
                MMI_MENU_ID_T menu_id = 0;
                MMI_MENU_GROUP_ID_T group_id = 0;

                //GUIMENU_GetId(MMI_SMARTDUALSIM_PREFERRED_SIM_CTRL_ID, &group_id, &menu_id);
                menu_id = GUILIST_GetCurItemIndex(MMI_SMARTDUALSIM_SMS_PREFERRED_SIM_CTRL_ID);
                switch(menu_id)
                {
                    case MMI_SMARTDUALSIM_PREFERRED_SIM1: 
                        //MMIAPISET_SetGPRSModeType(DATA_CARD_MODE_1);
                        MMISmartDS_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_SMS, MMISET_MAJOR_SIM_SEL_SIM1);
#ifdef MMI_SHOW_MSA
                        if(isOpenByBoot == TRUE)
                        {
                            MMIAPIHONE_CreateMicrosoftAccoutWindow();
                        }
#endif
                        MMK_CloseWin(win_id);
                        break;
                    case MMI_SMARTDUALSIM_PREFERRED_SIM2:
                        //MMIAPISET_SetGPRSModeType(DATA_CARD_MODE_2);
                        MMISmartDS_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_SMS, MMISET_MAJOR_SIM_SEL_SIM2);
#ifdef MMI_SHOW_MSA
                        if(isOpenByBoot == TRUE)
                        {
                            MMIAPIHONE_CreateMicrosoftAccoutWindow();
                        }
#endif
                        MMK_CloseWin(win_id);
                        break;
                #ifndef SMARTDS_DISABLE_ASK_EVERY_TIME
                    case MMI_SMARTDUALSIM_ASK_EVERY_TIME:
                        MMISmartDS_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_SMS, MMISET_MAJOR_SIM_SEL_MANUAL_SEL);
                        MMK_CloseWin(win_id);
                        break;
                #endif
                    default:
                        break;
                }
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            if(isOpenByBoot == TRUE)
            {
                break;
            }
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
            break;

        case MSG_APP_RED:
        case MSG_KEYUP_RED:
            if(isOpenByBoot == TRUE)
            {
                break;
            }
            recode = MMI_RESULT_FALSE;
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

PUBLIC void MMISmartDS_setSMSPreferredSimItems(void)
{
    MMI_CTRL_ID_T               ctrl_id = MMI_SMARTDUALSIM_SMS_PREFERRED_SIM_CTRL_ID;
    GUILIST_ITEM_T              item_t = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    MMISET_MAJOR_SIM_SEL_TYPE_E e_major_sim_type    =   0;

    item_t.item_data_ptr = &item_data;
    e_major_sim_type = MMISET_GetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_SMS);

    GUILIST_SetMaxItem(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_ITEM_NUM,FALSE);
    //GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);

    //set item "SIM1"
    item_t.item_style = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    if(isOpenByBoot == FALSE)
    {
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    }
    else
    {
    item_data.softkey_id[2] = TXT_NULL;
    }
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_ILIST_SIM_ONE;

    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_PREFERRED_SIM1);
    if(e_major_sim_type == MMISET_MAJOR_SIM_SEL_SIM1)
    {
        GUILIST_SetSelectedItem(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM1, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM1);
        //CTRLLIST_SetToggleState(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM1, TRUE);
    }

    //set item "SIM2"
    item_t.item_style = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    if(isOpenByBoot == FALSE)
    {
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    }
    else
    {
    item_data.softkey_id[2] = TXT_NULL;
    }
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_ILIST_SIM_TWO;
    
    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_PREFERRED_SIM2);
    if(e_major_sim_type == MMISET_MAJOR_SIM_SEL_SIM2)
    {
        GUILIST_SetSelectedItem(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM2, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM2);
        //CTRLLIST_SetToggleState(ctrl_id, MMI_SMARTDUALSIM_PREFERRED_SIM2, TRUE);
    }

#ifndef SMARTDS_DISABLE_ASK_EVERY_TIME
    //set item "Ask everytime"
    item_t.item_style    = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[0].item_data.text_id = STR_DSSET_DSDS_ASKEVERYTIME;

    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_ASK_EVERY_TIME);

    //todo: it must to async the true state
    if(e_major_sim_type == MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
    {
        GUILIST_SetSelectedItem(ctrl_id, MMI_SMARTDUALSIM_ASK_EVERY_TIME, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, MMI_SMARTDUALSIM_ASK_EVERY_TIME);
        //CTRLLIST_SetToggleState(ctrl_id, MMI_SMARTDUALSIM_ASK_EVERY_TIME, TRUE);
    }
#endif

    GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
}
