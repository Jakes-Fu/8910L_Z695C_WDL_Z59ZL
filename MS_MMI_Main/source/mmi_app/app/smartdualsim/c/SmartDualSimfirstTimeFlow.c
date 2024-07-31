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
#include "mmicom_banner.h"

#include "mmi_resource.h"
#include "guistring.h"
#include "mmipdp_export.h"
#include "mmiphone_export.h"
#include "mmipb_export.h"
#include "mmiset_export.h"
#include "smartdualsim_id.h"
#include "smartdualsim_text.h"
#include "smartdualsim_image.h"
#include "smartdualsim_menutable.h"
#include "smartdualsim_nv.h"
#include "smartdualsim_internal.h"
#include "smartdualsim_export.h"

extern PUBLIC BOOLEAN MMIAPISET_SetSmartDualSIMCFNRC(MMI_HANDLE_T win_id, SmartDS_Forwarding_Direction direction);


#define sim_number_TextFieldLength MMIPB_BCD_NUMBER_MAX_LEN
#define MSG_SMARTDS_INTERNAL_MSG_START 0xF0
#define MSG_SMARTDS_INTERNAL_CONTINUE_SET_FORWARD    (MSG_USER_START + (MMI_MODULE_SMARTDUALSIM << 16) + MSG_SMARTDS_INTERNAL_MSG_START)

LOCAL SmartDS_Forwarding_Direction destForwardType = SMARTDS_FORWARDING_NONE;
LOCAL smartDS_Forward_follow_type forwardFollowType = smartDS_Forward_follow_max;

LOCAL MMI_RESULT_E  HandleFirstTimeFlowQueryDialogMsg(
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM           param 
                                              );

LOCAL MMI_RESULT_E  HandleForwardOkDialogMsg(
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM           param 
                                              );

LOCAL MMI_RESULT_E HandleSimNumbersWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        );
LOCAL MMIPB_ERROR_E saveSIMNumber(MN_DUAL_SYS_E dual_sys);

LOCAL MMI_RESULT_E HandleForwardTypeSelectWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         );
LOCAL void MMISmartDS_setForwardTypeItems(void);
LOCAL void MMISmartDS_setForwardTypeItemEnable(MMI_WIN_ID_T parentWinId);

LOCAL void MMISmartDS_FirstTimeFlowQueryDialog(void);

LOCAL BOOLEAN MMISmartDS_setSimForward(
                        MMI_HANDLE_T win_id, 
                        SmartDS_Forwarding_Direction newForwardType);

WINDOW_TABLE( SMARTDUALSIM_SIM_NUMBERS_EDIT_WIN_TAB ) = 
{
    WIN_ID(MMI_SMARTDUALSIM_SIMNUMBERS_EDIT_WIN_ID),
    WIN_FUNC((uint32) HandleSimNumbersWinMsg),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_SMARTDUALSIM_SIM_NUMBER_FORM_CTRL_ID),
//    CHILD_EDIT_TEXT_CTRL(TRUE, sim_number_TextFieldLength, MMI_SMARTDUALSIM_SIM1_NUMBER_EDIT_CTRL_ID, MMI_SMARTDUALSIM_SIM_NUMBER_FORM_CTRL_ID),
//    CHILD_EDIT_TEXT_CTRL(TRUE, sim_number_TextFieldLength, MMI_SMARTDUALSIM_SIM2_NUMBER_EDIT_CTRL_ID, MMI_SMARTDUALSIM_SIM_NUMBER_FORM_CTRL_ID),
    CHILD_EDIT_PHONENUM_CTRL(TRUE, sim_number_TextFieldLength, MMI_SMARTDUALSIM_SIM1_NUMBER_EDIT_CTRL_ID, MMI_SMARTDUALSIM_SIM_NUMBER_FORM_CTRL_ID),
    CHILD_EDIT_PHONENUM_CTRL(TRUE, sim_number_TextFieldLength, MMI_SMARTDUALSIM_SIM2_NUMBER_EDIT_CTRL_ID, MMI_SMARTDUALSIM_SIM_NUMBER_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE( SMARTDUALSIM_FORWARD_TYPE_SELECT_WIN_TAB ) = 
{
    WIN_ID(MMI_SMARTDUALSIM_FORWARDTYPE_SELECT_WIN_ID),
    WIN_FUNC((uint32) HandleForwardTypeSelectWinMsg),
    //CREATE_MENU_CTRL(MENU_FORWARD_TYPE_TABLE_ID, MMI_SMARTDUALSIM_FORWARD_TYPE_MENU_CTRL_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_SMARTDUALSIM_FORWARD_TYPE_FORM_CTRL_ID),
    CHILD_TEXT_CTRL(FALSE, MMI_SMARTDUALSIM_FORWARD_TYPE_LABEL_CTRL_ID, MMI_SMARTDUALSIM_FORWARD_TYPE_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMI_SMARTDUALSIM_FORWARD_TYPE_MENU_CTRL_ID, MMI_SMARTDUALSIM_FORWARD_TYPE_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

dual_sim_numbers simNumbers = {0};

PUBLIC void MMISmartDS_ModuleInit(void)
{
    MMISmartDS_RegMenuGroup();
    MMISmartDS_RegModuleNv();
}

PUBLIC BOOLEAN MMISmartDS_CheckToDisplayPromptWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id)
{
    BOOLEAN recode = FALSE;
    BOOLEAN firstTimePrompted = FALSE;

    if(MMK_IsFocusWin(win_id))
    {
        MMI_ReadNVItem(MMI_SMARTDUALSIM_NV_FIST_TIME_PROMPTED, &firstTimePrompted);
        if(firstTimePrompted)
        {
            recode = FALSE;
        }
#ifdef WIN32
        else
#else
        else if(MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_1)
                && MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_2)
                && MMIAPIPHONE_IsCSAvail(MN_DUAL_SYS_1)
                && MMIAPIPHONE_IsCSAvail(MN_DUAL_SYS_2)
                && MMIAPIPHONE_GetRoamingStatus(MN_DUAL_SYS_1) == MMIPHONE_ROAMING_STATUS_NO_ROMING
                && MMIAPIPHONE_GetRoamingStatus(MN_DUAL_SYS_2) == MMIPHONE_ROAMING_STATUS_NO_ROMING
                && MMIAPIPB_IsPbReady()
                )
#endif
        {
            if(MSG_SMARTDS_FIRST_FLOW_CHECK == msg_id)
            {
                //MMK_SendMsg(win_id, MSG_SMARTDS_FIRST_FLOW_OK, (DPARAM)MSG_SMARTDS_FIRST_FLOW_OK);
                MMK_PostMsg(win_id, MSG_SMARTDS_FIRST_FLOW_OK, PNULL, 0);
                recode = FALSE;
            }
            else if(MSG_SMARTDS_FIRST_FLOW_PROMPT == msg_id)
            {
                MMISmartDS_FirstTimeFlowQueryDialog();
                firstTimePrompted = TRUE;
                MMI_WriteNVItem(MMI_SMARTDUALSIM_NV_FIST_TIME_PROMPTED, &firstTimePrompted);
                recode = TRUE;
            }
        }
    }
    return recode;
}

LOCAL void MMISmartDS_FirstTimeFlowQueryDialog(void)
{
    MMI_WIN_ID_T    queryWinId = MMI_SMARTDUALSIM_FIST_TIME_ASK_WIN_ID;
    MMI_STRING_T      progress_str = {0};

    MMISmartDS_setForwardFollowType(smartDS_Forward_follow_poweron);
    MMIPUB_OpenConfirmationDialogByTextId(
                                        PNULL,   //time period,default is 2s
                                        STR_SMART_DUAL_SIM_EXT01,           //text 1,no default TXT_NULL
                                        PNULL,           //text 2,no default TXT_NULL
                                        PNULL,           //alert image id
                                        &queryWinId,        //win id,default is MMIPUB_ALERT_WIN_ID
                                        PNULL,  //win priority,default is WIN_ONE_LEVEL
                                        MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,      //softkey style
                                        HandleFirstTimeFlowQueryDialogMsg         //handle function
                                        );
}

LOCAL MMI_RESULT_E  HandleFirstTimeFlowQueryDialogMsg(
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
//        case MSG_APP_OK:
        case MSG_APP_WEB:
//        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
            MMK_CloseWin(win_id);
            MMISmartDS_SimNumbersWin(win_id);
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
        default:
            recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
            break;
    }
    return (recode);
}


PUBLIC void MMISmartDS_ForwardOkDialog(SmartDS_Forwarding_Direction direction)
{
    //MMI_STRING_T      progress_str = {0};
    MMI_WIN_ID_T        win_id = MMI_SMARTDUALSIM_FORWARD_SET_SUCCESS_WIN_ID;
    MMI_STRING_T        strOrigin1 = {0};
    MMI_STRING_T        strOrigin2 = {0};
    MMI_STRING_T        str1 = {0};
    MMI_STRING_T        str2 = {0};
    MMI_TEXT_ID_T       text1Id = TXT_NULL;
    MMI_TEXT_ID_T       text2Id = TXT_NULL;
	wchar*              originPtr = PNULL;
    uint32              strLen = 0;
    wchar*              wstrOldN = L"\\n";
    wchar*              wstrNewN = L"\n";
	wchar*              wstrOldU = L"%U";

#ifdef WIN32
	wchar*              wstrDefNum1 = L"18515850291";
	wchar*              wstrDefNum2 = L"13120495373";

    simNumbers.sim_number1_len = MMIAPICOM_Wstrlen(wstrDefNum1);
    MMIAPICOM_Wstrcpy(simNumbers.sim_number1, wstrDefNum1);
    simNumbers.sim_number2_len = MMIAPICOM_Wstrlen(wstrDefNum2);
    MMIAPICOM_Wstrcpy(simNumbers.sim_number2, wstrDefNum2);
#endif

        SCI_TraceLow(
            "smartDS MMISmartDS_ForwardOkDialog open direction=%d",
        direction);
    //Save the forwarding type
    if(direction > SMARTDS_FORWARDING_BOTH_SIM)
    {
        SmartDS_Forwarding_Direction newForwardType = SMARTDS_FORWARDING_NONE;

        MMI_WriteNVItem(MMI_SMARTDUALSIM_NV_FORWARDING_DIRECTION, (void *)&newForwardType);
        if(destForwardType != SMARTDS_FORWARDING_NONE)
        {
            MMK_PostMsg(MMI_SMARTDUALSIM_FORWARDTYPE_SELECT_WIN_ID, MSG_SMARTDS_INTERNAL_CONTINUE_SET_FORWARD, PNULL, 0);
        }
        else
        {
            if(smartDS_Forward_follow_menuset == forwardFollowType)
            {
                MMK_CloseWin(MMI_SMARTDUALSIM_FORWARDTYPE_SELECT_WIN_ID);
            }
            else
            {
                MMK_PostMsg(MMI_SMARTDUALSIM_FORWARDTYPE_SELECT_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
            }
        }
        return;
    }
    else
    {
        MMI_WriteNVItem(MMI_SMARTDUALSIM_NV_FORWARDING_DIRECTION, (void *)&direction);
        if(smartDS_Forward_follow_menuset == forwardFollowType)
        {
            MMK_CloseWin(MMI_SMARTDUALSIM_FORWARDTYPE_SELECT_WIN_ID);
            return;
        }
    }

    if(direction == SMARTDS_FORWARDING_BOTH_SIM)
    {
        text1Id = STR_FWD_NOTE_STATUS_1_EXT02;
        text2Id = STR_FWD_NOTE_STATUS_2_EXT02;
    }
    else if(direction == SMARTDS_FORWARDING_1_TO_2)
    {
        text1Id = STR_FWD_NOTE_STATUS_1_EXT02;
    }
    else if(direction == SMARTDS_FORWARDING_2_TO_1)
    {
        text2Id = STR_FWD_NOTE_STATUS_2_EXT02;
    }
    else
    {
        text1Id = TXT_NULL;
        text2Id = TXT_NULL;
    }

    originPtr = PNULL;
    if(text1Id != TXT_NULL)
    {
        MMI_GetLabelTextByLang(text1Id, &strOrigin1);
        strLen = strOrigin1.wstr_len;
        strLen += simNumbers.sim_number2_len > 0 ? simNumbers.sim_number2_len : 0;
        strLen += 1;
        strLen *= sizeof(wchar);
        originPtr = SCI_ALLOC_APP(strLen);
        if(originPtr)
        {
            SCI_MEMSET(originPtr, 0x00, strLen);
            MMIAPICOM_Wstrncpy(originPtr, strOrigin1.wstr_ptr, strOrigin1.wstr_len);//(originPtr, strOrigin1.wstr_ptr);
            if(simNumbers.sim_number2_len > 0)
            {
                MMIAPICOM_WstrReplace(originPtr, strLen, wstrOldU, simNumbers.sim_number2);
            }
            MMIAPICOM_WstrReplace(originPtr, strLen, wstrOldN, wstrNewN);
            str1.wstr_ptr = originPtr;
            str1.wstr_len = MMIAPICOM_Wstrlen(originPtr);
        }
    }
    originPtr = PNULL;
    if(text2Id != TXT_NULL)
    {
        MMI_GetLabelTextByLang(text2Id, &strOrigin2);
        strLen = strOrigin2.wstr_len;
        strLen += simNumbers.sim_number1_len > 0 ? simNumbers.sim_number1_len : 0;
        strLen += 1;
        strLen *= sizeof(wchar);
        originPtr = SCI_ALLOC_APP(strLen);
        if(originPtr)
        {
            SCI_MEMSET(originPtr, 0x00, strLen);
            MMIAPICOM_Wstrncpy(originPtr, strOrigin2.wstr_ptr, strOrigin2.wstr_len);//MMIAPICOM_Wstrcpy(originPtr, strOrigin2.wstr_ptr);
            if(simNumbers.sim_number1_len > 0)
            {
                MMIAPICOM_WstrReplace(originPtr, strLen, wstrOldU, simNumbers.sim_number1);
            }
            MMIAPICOM_WstrReplace(originPtr, strLen, wstrOldN, wstrNewN);
            str2.wstr_ptr = originPtr;
            str2.wstr_len = MMIAPICOM_Wstrlen(originPtr);
        }
    }

MMIPUB_OpenTextDialogByTextPtr(
                                         PNULL,   //time period,default is 3s
                                         &str1,         //text 1
                                         &str2,         //text 2
                                         IMAGE_NULL,           //alert image id
                                         &win_id,        //win id,default is MMIPUB_ALERT_WIN_ID
                                         PNULL,  //win priority,default is WIN_ONE_LEVEL
                                         MMIPUB_SOFTKEY_DIALOG_OK,      //softkey style
                                         HandleForwardOkDialogMsg         //handle function                                         
                                         );
/*    MMIPUB_OpenConfirmationDialogByTextPtr(
                                        PNULL,   //time period,default is 2s
                                        &str1,           //text 1,no default TXT_NULL
                                        &str2,           //text 2,no default TXT_NULL
                                        PNULL,           //alert image id
                                        &win_id,        //win id,default is MMIPUB_ALERT_WIN_ID
                                        PNULL,  //win priority,default is WIN_ONE_LEVEL
                                        MMIPUB_SOFTKEY_DIALOG_OK,      //softkey style
                                        HandleForwardOkDialogMsg         //handle function
                                        );
*/
    if(str1.wstr_ptr)
    {
        SCI_FREE(str1.wstr_ptr);
    }
    if(str2.wstr_ptr)
    {
        SCI_FREE(str2.wstr_ptr);
    }
}

LOCAL MMI_RESULT_E  HandleForwardOkDialogMsg(
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM           param 
                                              )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;    

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_CloseWin(MMI_SMARTDUALSIM_SIMNUMBERS_EDIT_WIN_ID);
            MMK_CloseWin(MMI_SMARTDUALSIM_FORWARDTYPE_SELECT_WIN_ID);
            break;
//        case MSG_APP_OK:
        case MSG_APP_WEB:
//        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
//        case MSG_CTL_CANCEL:
//        case MSG_APP_CANCEL:
            MMK_CloseWin(MMI_SMARTDUALSIM_SIMNUMBERS_EDIT_WIN_ID);
            MMK_CloseWin(MMI_SMARTDUALSIM_FORWARDTYPE_SELECT_WIN_ID);
            //MMK_CloseMiddleWin(MMI_SMARTDUALSIM_SIMNUMBERS_EDIT_WIN_ID, MMI_SMARTDUALSIM_FORWARDTYPE_SELECT_WIN_ID);
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
        default:
            recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
            break;
    }
    return (recode);
}

PUBLIC void MMISmartDS_ForwardFailedDialog(SmartDS_Forwarding_Direction direction, MMI_TEXT_ID_T promptStrID)
{
    MMI_WIN_ID_T        win_id = MMI_SMARTDUALSIM_FORWARD_SET_FAIL_WIN_ID;
        SCI_TraceLow(
            "smartDS MMISmartDS_ForwardFailedDialog open direction=%d",
        direction);

    destForwardType = SMARTDS_FORWARDING_NONE;
    if(promptStrID != TXT_NULL)
    {
        MMIPUB_OpenConfirmationDialogByTextId(
                                        PNULL,   //time period,default is 2s
                                        promptStrID,           //text 1,no default TXT_NULL
                                        PNULL,           //text 2,no default TXT_NULL
                                        PNULL,           //alert image id
                                        &win_id,        //win id,default is MMIPUB_ALERT_WIN_ID
                                        PNULL,  //win priority,default is WIN_ONE_LEVEL
                                        MMIPUB_SOFTKEY_DIALOG_OK,      //softkey style
                                        HandleForwardOkDialogMsg         //handle function
                                        );
    }
    else
    {
        if(smartDS_Forward_follow_menuset == forwardFollowType)
        {
            MMK_CloseWin(MMI_SMARTDUALSIM_FORWARDTYPE_SELECT_WIN_ID);
            return;
        }
    }
}

PUBLIC MMI_HANDLE_T MMISmartDS_SimNumbersWin(MMI_WIN_ID_T parentWinId)
{
    MMI_WIN_ID_T * parentId = SCI_ALLOC_APP(sizeof(MMI_WIN_ID_T));
    if(PNULL == parentId)
    {
        return 0;
    }
    *parentId = parentWinId;
    return MMK_CreateWin((uint32*)SMARTDUALSIM_SIM_NUMBERS_EDIT_WIN_TAB, (ADD_DATA)parentId);
}

LOCAL MMI_RESULT_E HandleSimNumbersWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        )
{
    MMI_RESULT_E        recode  = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        *parentWinId = (MMI_WIN_ID_T *)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T       editCtrlId1 = MMI_SMARTDUALSIM_SIM1_NUMBER_EDIT_CTRL_ID;
    MMI_CTRL_ID_T       editCtrlId2 = MMI_SMARTDUALSIM_SIM2_NUMBER_EDIT_CTRL_ID;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            GUI_BG_T  	backgrand = {0};
            GUI_COLOR_T font_color_ptr = MMI_DEFAULT_TEXT_COLOR;
            MMI_CTRL_ID_T   formCtrlId = MMI_SMARTDUALSIM_SIM_NUMBER_FORM_CTRL_ID;

            //style for form
            backgrand.bg_type = GUI_BG_COLOR;
            backgrand.color = MMI_BACKGROUND_COLOR;
            GUIFORM_SetStyle(formCtrlId,GUIFORM_STYLE_NORMAL);
            GUIFORM_SetBg(formCtrlId, &backgrand);
            GUIFORM_PermitChildFont(formCtrlId,FALSE);
            GUIFORM_PermitChildBorder(formCtrlId,FALSE);
            GUIFORM_PermitChildBg(formCtrlId,FALSE);

            //style for editor 1
            GUIEDIT_SetStyle(editCtrlId1, GUIEDIT_STYLE_MULTI_DYN_DOWN);
            GUIEDIT_SetIm(editCtrlId1,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
            GUIEDIT_SetImTag(editCtrlId1, GUIIM_TAG_PHONE_NUM);
            GUIEDIT_SetFontColor(editCtrlId1, font_color_ptr);
            GUIEDIT_SetSoftkeyEx(editCtrlId1, LEFT_BUTTON, 0, TXT_NULL, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            if(*parentWinId == MMI_SMARTDUALSIM_MAIN_MENU_WIN_ID)
            {
                GUIEDIT_SetSoftkeyEx(editCtrlId1, MIDDLE_BUTTON, 0, STXT_SOFTKEY_SAVE_MK, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            }
            else
            {
                GUIEDIT_SetSoftkeyEx(editCtrlId1, MIDDLE_BUTTON, 0, TXT_NEXT_COMMAND, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            }
            GUIEDIT_SetSoftkeyEx(editCtrlId1, RIGHT_BUTTON, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, PNULL, PNULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            CTRLBASEEDIT_SetLabelId(win_id, editCtrlId1, STR_ILIST_SIM_ONE, FALSE);
            GUIEDIT_SetDefaultStringById(editCtrlId1, STR_LIST_SIM_NUMBER_EXT01);
            getSIMNumber(MN_DUAL_SYS_1);
            GUIEDIT_SetString( editCtrlId1, 
                simNumbers.sim_number1, simNumbers.sim_number1_len);
            if(!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
            {
                GUIFORM_SetChildDisplay(formCtrlId, editCtrlId1, GUIFORM_CHILD_DISP_GRAY);
            }

            //style for editor 2
            GUIEDIT_SetStyle(editCtrlId2, GUIEDIT_STYLE_MULTI_DYN_DOWN);
            GUIEDIT_SetIm(editCtrlId2,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
            GUIEDIT_SetImTag(editCtrlId2, GUIIM_TAG_PHONE_NUM);
            GUIEDIT_SetFontColor(editCtrlId2, font_color_ptr);
            GUIEDIT_SetSoftkeyEx(editCtrlId2, LEFT_BUTTON, 0, TXT_NULL, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            if(*parentWinId == MMI_SMARTDUALSIM_MAIN_MENU_WIN_ID)
            {
                GUIEDIT_SetSoftkeyEx(editCtrlId2, MIDDLE_BUTTON, 0, STXT_SOFTKEY_SAVE_MK, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            }
            else
            {
                GUIEDIT_SetSoftkeyEx(editCtrlId2, MIDDLE_BUTTON, 0, TXT_NEXT_COMMAND, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            }
            GUIEDIT_SetSoftkeyEx(editCtrlId2, RIGHT_BUTTON, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, PNULL, PNULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            CTRLBASEEDIT_SetLabelId(win_id, editCtrlId2, STR_ILIST_SIM_TWO, FALSE);
            GUIEDIT_SetDefaultStringById(editCtrlId2, STR_LIST_SIM_NUMBER_EXT01);
            getSIMNumber(MN_DUAL_SYS_2);
            GUIEDIT_SetString( editCtrlId2, 
                simNumbers.sim_number2, simNumbers.sim_number2_len);
            if(!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
            {
                GUIFORM_SetChildDisplay(formCtrlId, editCtrlId2, GUIFORM_CHILD_DISP_GRAY);
            }

            //set acitve the first editor.
            if(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
            {
                MMK_SetAtvCtrl(win_id, editCtrlId1);
            }
            else if(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
            {
                MMK_SetAtvCtrl(win_id, editCtrlId2);
            }

            if(*parentWinId == MMI_SMARTDUALSIM_MAIN_MENU_WIN_ID)
            {
                //todo:change softkey
            }
        }
            break;
        case MSG_GET_FOCUS:
            break;
        case MSG_FULL_PAINT:
            if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_FWD_HDR_NUM_EXT02))
            {
                GUIWIN_UpdateStb();
            }
            break;
//        case MSG_APP_OK:
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
//        case MSG_CTL_OK:
            {
                if(!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)
                    && !MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
                {
                    break;
                }
                if(!MMIPB_IsPbReady())
                {
                    MMISmartDS_displayBanner(win_id, STR_CONTACTS_NOT_READY_EXT01);
                }

                if(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
                {
                    saveSIMNumber(MN_DUAL_SYS_1);
                }
                if(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
                {
                    saveSIMNumber(MN_DUAL_SYS_2);
                }
                {
                    SCI_TraceLow(
                        "smartDS HandleSimNumbersWinMsg *parentWinId=%d;MMI_SMARTDUALSIM_FIST_TIME_ASK_WIN_ID=%d",
                        *parentWinId,MMI_SMARTDUALSIM_FIST_TIME_ASK_WIN_ID);
                    getSIMNumber(MN_DUAL_SYS_1);
                    getSIMNumber(MN_DUAL_SYS_2);
                    if(*parentWinId == MMI_SMARTDUALSIM_MAIN_MENU_WIN_ID)
                    {
                        MMISmartDS_displayBanner(win_id, STR_SIM_NUMBER_SAVED_EXT01);
                        MMK_CloseWin(win_id);
                    }
                    else if(*parentWinId == MMI_SMARTDUALSIM_FIST_TIME_ASK_WIN_ID)
                    {
                        if(simNumbers.sim_number1_len > 0
                            && simNumbers.sim_number2_len > 0)
                        {
                            MMISmartDS_ForwardTypeSelectWin(*parentWinId);
                        }
                        else
                        {
                            MMISmartDS_displayBanner(win_id, STR_FWD_NOTE_SIM_NUMBER_EXT01);
                        }
                    }
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

PUBLIC void getSIMNumber(MN_DUAL_SYS_E dual_sys)
{
    MMIPB_BCD_NUMBER_T sim_number[1]={0};
    uint8 sim_num[SMARTDS_PB_BCD_NUMBER_MAX_LEN]={0};
    wchar*   wstr_ptr;
    uint16*  wstr_len;

    if(dual_sys == MN_DUAL_SYS_1)
    {
        simNumbers.sim_number1_len = 0;
        SCI_MEMSET(simNumbers.sim_number1, 0x00, sizeof(wchar)*SMARTDS_PB_BCD_NUMBER_MAX_LEN);
        wstr_ptr = simNumbers.sim_number1;
        wstr_len = &(simNumbers.sim_number1_len);
    }
    else
    {
        simNumbers.sim_number2_len = 0;
        SCI_MEMSET(simNumbers.sim_number2, 0x00, sizeof(wchar)*SMARTDS_PB_BCD_NUMBER_MAX_LEN);
        wstr_ptr = simNumbers.sim_number2;
        wstr_len = &(simNumbers.sim_number2_len);
    }
    SCI_TraceLow(
        "smartDS getSIMNumber simNumbers.sim_number1=%d",
        sizeof(simNumbers.sim_number1));
    SCI_TraceLow(
        "smartDS getSIMNumber simNumbers.sim_number2_len=%d",
        simNumbers.sim_number2_len);

    MMIAPIPB_GetMSISDNumberArrayBySim(sim_number,dual_sys,0);    //Get first one

    if(sim_number[0].number_len >0)
    {
        size_t len;
        
        MMIAPICOM_GenDispNumber(MMIPB_GetNumberTypeFromUint8(sim_number[0].npi_ton),
                sim_number[0].number_len, sim_number[0].number, sim_num, SMARTDS_PB_BCD_NUMBER_MAX_LEN);
        MMI_STRNTOWSTR(wstr_ptr, SMARTDS_PB_BCD_NUMBER_MAX_LEN,sim_num,SMARTDS_PB_BCD_NUMBER_MAX_LEN, MIN(SMARTDS_PB_BCD_NUMBER_MAX_LEN,SMARTDS_PB_BCD_NUMBER_MAX_LEN));
        len = MMIAPICOM_Wstrlen(wstr_ptr);
        *wstr_len = (uint16)len;
    SCI_TraceLow(
        "smartDS getSIMNumber len=%d;*wstr_len=%d",
        len,*wstr_len);
    SCI_TraceLow(
        "smartDS getSIMNumber lsimNumbers.sim_number2_len%d;",
        simNumbers.sim_number2_len);
    }
    return;
}

LOCAL MMIPB_ERROR_E saveSIMNumber(MN_DUAL_SYS_E dual_sys)
{
    MMIPB_ERROR_E retcode = MMIPB_ERROR_SUCCESS;
    MMIPB_CONTACT_T *contact_ptr = PNULL;

    MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
    MMI_STRING_T        str_info = {0};
    uint8  num_buf[MMIPB_NUMBER_MAX_STRING_LEN] = {0};
    uint8  number_index = 0;
    uint16 num_len = 0;
    BOOLEAN numChanged = FALSE;

    if(MN_DUAL_SYS_1 == dual_sys)
    {
        GUIEDIT_GetString(MMI_SMARTDUALSIM_SIM1_NUMBER_EDIT_CTRL_ID, &str_info);
        if(0 != MMIAPICOM_Wstrcmp(simNumbers.sim_number1, str_info.wstr_ptr))
        {
            numChanged = TRUE;
        }
        else
        {
            return MMIPB_ERROR_SUCCESS;
        }
    }
    else
    {
        GUIEDIT_GetString(MMI_SMARTDUALSIM_SIM2_NUMBER_EDIT_CTRL_ID, &str_info);
        if(0 != MMIAPICOM_Wstrcmp(simNumbers.sim_number2, str_info.wstr_ptr))
        {
            numChanged = TRUE;
        }
        else
        {
            return MMIPB_ERROR_SUCCESS;
        }
    }

    if(str_info.wstr_len == 0)
    {
        return MMIAPIPB_DeleteContact(1, MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + dual_sys, PHONEBOOK_MSISDN_STORAGE));
    }

    if(numChanged)
    {
        contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
        if(PNULL == contact_ptr)
        {
            return MMIPB_ERROR_ERROR;
        }
        SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T))

        num_len = MIN(MMIPB_NUMBER_MAX_STRING_LEN, str_info.wstr_len);
        MMI_WSTRNTOSTR(num_buf, MMIPB_NUMBER_MAX_STRING_LEN, str_info.wstr_ptr, MMIPB_NUMBER_MAX_STRING_LEN, num_len);
        if (str_info.wstr_len > 0 && number_index < MMIPB_MAX_NV_PHONE_NUM && MMIAPICOM_GenPartyNumber((uint8 *)(num_buf), num_len, &party_num))
        {
            contact_ptr->num_type[number_index] = MMIPB_ADN_NUMBER+number_index;
            contact_ptr->number[number_index].npi_ton = (uint8)((party_num.number_type << 0X04) | MMIPB_DEFAULT_NPI);
            contact_ptr->number[number_index].number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
            SCI_MEMCPY(contact_ptr->number[number_index].number, party_num.bcd_num, contact_ptr->number[number_index].number_len);
        }


        contact_ptr->contact_id = 1;
        contact_ptr->storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + dual_sys, PHONEBOOK_MSISDN_STORAGE);
        contact_ptr->num_type[number_index] = MMIPB_ADN_NUMBER+number_index;
        retcode = MMIAPIPB_AddContact(contact_ptr);

        SCI_FREE(contact_ptr);
    }

    return retcode;
}

PUBLIC MMI_HANDLE_T MMISmartDS_ForwardTypeSelectWin(MMI_WIN_ID_T parentWinId)
{
    MMI_WIN_ID_T * parentId = SCI_ALLOC_APP(sizeof(MMI_WIN_ID_T));
    if(PNULL == parentId)
    {
        return 0;
    }
    *parentId = parentWinId;
    return MMK_CreateWin((uint32*)SMARTDUALSIM_FORWARD_TYPE_SELECT_WIN_TAB, (ADD_DATA)parentId);
}

LOCAL MMI_RESULT_E HandleForwardTypeSelectWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T    *parentWinId = (MMI_WIN_ID_T *)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T   formCtrlId = MMI_SMARTDUALSIM_FORWARD_TYPE_FORM_CTRL_ID;
    MMI_CTRL_ID_T   labelCtrlId = MMI_SMARTDUALSIM_FORWARD_TYPE_LABEL_CTRL_ID;

    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T label_str = {0};
            GUI_BG_T  	backgrand = {0};
            GUI_COLOR_T font_color_ptr = MMI_WHITE_COLOR;
            GUI_FONT_T font = CAF_FONT_16;
            MMISET_LANGUAGE_TYPE_E  language_type = MMISET_MAX_LANGUAGE;

            MMIAPISET_GetLanguageType(&language_type);
            if(MMISET_LANGUAGE_ITALIAN == language_type)
            {
                font = CAF_FONT_14;
            }
            backgrand.bg_type = GUI_BG_COLOR;
            backgrand.color = MMI_BACKGROUND_COLOR;
            GUIFORM_SetStyle(formCtrlId,GUIFORM_STYLE_NORMAL);
            GUIFORM_SetBg(formCtrlId, &backgrand);
            GUIFORM_PermitChildFont(formCtrlId,FALSE);
            GUIFORM_PermitChildBorder(formCtrlId,FALSE);

            if(*parentWinId == MMI_SMARTDUALSIM_FIST_TIME_ASK_WIN_ID)
            {
                GUITEXT_SetMarginEx(
                                   labelCtrlId,    //in
                                   2,  //in:left margin
                                   0,   //in:right margin
                                   3,  //in:top margin
                                   3   //in:bottom margin
                                   );
                GUITEXT_SetFont(labelCtrlId,        //in:
                                &font,      //in:may PNULL
                                &font_color_ptr //in:may PNULL
                                );
                MMI_GetLabelTextByLang(STR_FWD_BETWEEN_SIMS_EXT01, &label_str);
                GUITEXT_SetString(labelCtrlId,label_str.wstr_ptr,label_str.wstr_len,FALSE);
            }
            else
            {
                GUIFORM_SetChildDisplay(formCtrlId,
                                    labelCtrlId, 
                                    GUIFORM_CHILD_DISP_HIDE);
                
            }
        }
            MMISmartDS_setForwardTypeItems();
            MMISmartDS_setForwardTypeItemEnable(*parentWinId);
            MMK_SetAtvCtrl(win_id, MMI_SMARTDUALSIM_FORWARD_TYPE_MENU_CTRL_ID);
            break;

        case MSG_GET_FOCUS:
            break;

        case MSG_LOSE_FOCUS:
            break;

        case MSG_FULL_PAINT:
            if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_FWD_HDR_FORWARD_EXT02))
            {
                GUIWIN_UpdateStb();
            }
            MMISmartDS_setForwardTypeItemEnable(*parentWinId);
            break;

        case MSG_APP_WEB:
//        case MSG_APP_OK:
//        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
            {
                MMI_MENU_ID_T menu_id = 0;
                MMI_MENU_GROUP_ID_T group_id = 0;
                SmartDS_Forwarding_Direction oldForwardType = SMARTDS_FORWARDING_NONE;
                SmartDS_Forwarding_Direction newForwardType = SMARTDS_FORWARDING_NONE;

                MMI_ReadNVItem(MMI_SMARTDUALSIM_NV_FORWARDING_DIRECTION, (void*)&oldForwardType);

                menu_id = GUILIST_GetCurItemIndex(MMI_SMARTDUALSIM_FORWARD_TYPE_MENU_CTRL_ID);
                switch(menu_id)
                {
                    case MMI_SMARTDUALSIM_FORWARD_SIM_BOTH:
                        newForwardType = SMARTDS_FORWARDING_BOTH_SIM;
                        break;
                    case MMI_SMARTDUALSIM_FORWARD_SIM_1_TO_2:
                        newForwardType = SMARTDS_FORWARDING_1_TO_2;
                        break;
                    case MMI_SMARTDUALSIM_FORWARD_SIM_2_TO_1:
                        newForwardType = SMARTDS_FORWARDING_2_TO_1;
                        break;
                    case MMI_SMARTDUALSIM_FORWARD_OFF:
                        newForwardType = SMARTDS_FORWARDING_NONE;
                        break;
                    default:
                        break;
                }

                SCI_TraceLow(
                    "smartDS MMI_SMARTDUALSIM_FORWARDTYPE_SELECT_WIN_ID=%d;",
                    MMI_SMARTDUALSIM_FORWARDTYPE_SELECT_WIN_ID);
                SCI_TraceLow(
                    "smartDS newForwardType=%d;oldForwardType=%d;parentWinId=%d",
                    newForwardType, oldForwardType, *parentWinId);

                if(newForwardType != oldForwardType)
                {

                    if(*parentWinId == MMI_SMARTDUALSIM_MAIN_MENU_WIN_ID)
                    {
                        //todo:save forward type nv
                    }
                    else if(*parentWinId == MMI_SMARTDUALSIM_FIST_TIME_ASK_WIN_ID)
                    {
                        //BOOLEAN forwardEnabled = TRUE;
                        //MMI_WriteNVItem(MMI_SMARTDUALSIM_NV_ENABLED, (void *)&forwardEnabled);
                    }
                    MMISmartDS_setSimForward(win_id, newForwardType);
                }
                else if((SMARTDS_FORWARDING_NONE == oldForwardType)
                        && (*parentWinId == MMI_SMARTDUALSIM_FIST_TIME_ASK_WIN_ID))
                {
                    MMK_CloseWin(MMI_SMARTDUALSIM_SIMNUMBERS_EDIT_WIN_ID);
                    MMK_CloseWin(MMI_SMARTDUALSIM_FORWARDTYPE_SELECT_WIN_ID);
                    MMK_CloseWin(win_id);
                }
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
            break;

        case MSG_SMARTDS_INTERNAL_CONTINUE_SET_FORWARD:
            if(destForwardType != SMARTDS_FORWARDING_NONE)
            {
                MMISmartDS_setSimForward(win_id, destForwardType);
                destForwardType = SMARTDS_FORWARDING_NONE;
            }
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

LOCAL void MMISmartDS_setForwardTypeItems(void)
{
    MMI_CTRL_ID_T               ctrl_id = MMI_SMARTDUALSIM_FORWARD_TYPE_MENU_CTRL_ID;
    GUILIST_ITEM_T              item_t = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};

    item_t.item_data_ptr = &item_data;

    GUILIST_SetMaxItem(ctrl_id, MMI_SMARTDUALSIM_FORWARD_ITEM_NUM,FALSE);
    GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);

    //set item "Both items"
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[0].item_data.text_id = STR_FWD_LIST_BTW_SIM_EXT02;

    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_FORWARD_SIM_BOTH);

    //set item "Forward calls"
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[0].item_data.text_id = STR_FWD_LIST_TO_SIM2_EXT02;
    
    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_FORWARD_SIM_1_TO_2);

    //set item "SIM numbers"
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[0].item_data.text_id = STR_FWD_LIST_TO_SIM1_EXT02;
    
    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_FORWARD_SIM_2_TO_1);

    //set item "off"
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
    item_data.item_content[0].item_data.text_id = STR_FWD_LIST_OFF_EXT01;
    
    GUILIST_InsertItem(ctrl_id, &item_t, MMI_SMARTDUALSIM_FORWARD_OFF);
}

LOCAL void MMISmartDS_setForwardTypeItemEnable(MMI_WIN_ID_T parentWinId)
{
    MMI_CTRL_ID_T               ctrl_id = MMI_SMARTDUALSIM_FORWARD_TYPE_MENU_CTRL_ID;
    SmartDS_Forwarding_Direction curForwardType = SMARTDS_FORWARDING_NONE;

    MMI_ReadNVItem(MMI_SMARTDUALSIM_NV_FORWARDING_DIRECTION, (void*)&curForwardType);

    switch(curForwardType)
    {
        case SMARTDS_FORWARDING_BOTH_SIM:
            GUILIST_SetItemGray(ctrl_id, MMI_SMARTDUALSIM_FORWARD_SIM_BOTH, TRUE);
            GUILIST_SetItemInvalid(ctrl_id, MMI_SMARTDUALSIM_FORWARD_SIM_BOTH, TRUE);
            break;
        case SMARTDS_FORWARDING_1_TO_2:
            GUILIST_SetItemGray(ctrl_id, MMI_SMARTDUALSIM_FORWARD_SIM_1_TO_2, TRUE);
            GUILIST_SetItemInvalid(ctrl_id, MMI_SMARTDUALSIM_FORWARD_SIM_1_TO_2, TRUE);
            break;
        case SMARTDS_FORWARDING_2_TO_1:
            GUILIST_SetItemGray(ctrl_id, MMI_SMARTDUALSIM_FORWARD_SIM_2_TO_1, TRUE);
            GUILIST_SetItemInvalid(ctrl_id, MMI_SMARTDUALSIM_FORWARD_SIM_2_TO_1, TRUE);
            break;
        case SMARTDS_FORWARDING_NONE:
            if(parentWinId == MMI_SMARTDUALSIM_MAIN_MENU_WIN_ID)
            {
                GUILIST_SetItemGray(ctrl_id, MMI_SMARTDUALSIM_FORWARD_OFF, TRUE);
                GUILIST_SetItemInvalid(ctrl_id, MMI_SMARTDUALSIM_FORWARD_OFF, TRUE);
            }
            break;
        default:
            break;
    }
}

PUBLIC void MMIAPISET_CloseSmartDualSIMCFNRC(SmartDS_Forwarding_Direction direction);
LOCAL BOOLEAN MMISmartDS_setSimForward(
                        MMI_HANDLE_T win_id, 
                        SmartDS_Forwarding_Direction newForwardType)
{
    SmartDS_Forwarding_Direction oldForwardType = SMARTDS_FORWARDING_NONE;
    SCI_TraceLow(
        "smartDS MMISmartDS_setSimForward newForwardType=%d",
        newForwardType);

    MMI_ReadNVItem(MMI_SMARTDUALSIM_NV_FORWARDING_DIRECTION, (void*)&oldForwardType);
    if(oldForwardType == newForwardType)
    {
        return TRUE;
    }

    SCI_TraceLow(
        "smartDS MMISmartDS_setSimForward close oldForwardType=%d",
    oldForwardType);
    if(oldForwardType > SMARTDS_FORWARDING_NONE && oldForwardType <= SMARTDS_FORWARDING_BOTH_SIM)
    {
        MMIAPISET_CloseSmartDualSIMCFNRC(oldForwardType + (SMARTDS_FORWARDING_CLOSE_1_TO_2 - SMARTDS_FORWARDING_1_TO_2));
        destForwardType = newForwardType;
        return TRUE;
    }

    SCI_TraceLow(
        "smartDS MMISmartDS_setSimForward open newForwardType=%d",
    newForwardType);
    if(newForwardType > SMARTDS_FORWARDING_NONE && newForwardType <= SMARTDS_FORWARDING_BOTH_SIM)
    {
        return MMIAPISET_SetSmartDualSIMCFNRC(win_id, newForwardType);
    }
    else
    {
        return FALSE;
    }
}

PUBLIC SmartDS_Forwarding_Direction MMISmartDS_getDestForwardType(void)
{
    return destForwardType;
}

PUBLIC smartDS_Forward_follow_type MMISmartDS_getForwardFollowType(void)
{
    return forwardFollowType;
}

PUBLIC void MMISmartDS_setForwardFollowType(smartDS_Forward_follow_type followType)
{
    forwardFollowType = followType;
}

PUBLIC void MMISmartDS_displayBanner(MMI_WIN_ID_T win_id, MMI_TEXT_ID_T infoID)
{
    GUI_POINT_T start_point = {0, 0};
    MMICOM_BANNER_INFO_T banner_info = {0};
    
    banner_info.banner_type = MMI_BANNER_TEXT;
    banner_info.start_point = start_point;

    banner_info.text1_id = infoID;
    MMIAPICOM_OpenBannerWin(win_id, banner_info);
}
