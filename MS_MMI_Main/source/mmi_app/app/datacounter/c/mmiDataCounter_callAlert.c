/****************************************************************************
** File Name:      mmiDataCounter_callAlert.c                            
** Author:                                                                  
** Date:            2015/08/24
** Description:    This file is used to process msg to main window of CALCULATION
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
****************************************************************************/
#define _MMIDATACOUNTER_CALL_ALERT_C_  

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
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
#include "guibutton.h"
#include "guistatusbar.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmk_msg.h"
#include "guiwin.h"
#include "guimenu.h"
#include "guitext.h"
#include "ctrlform_export.h"

#include "mmi_resource.h"
#include "guistring.h"
#include "mmipdp_export.h"
#include "guiform.h"

#include "datacounter_text.h"
#include "datacounter_menutable.h"
#include "datacounter_internal.h"
#include "datacounter_id.h"
#include "mmiphone_export.h"
#include "mmiphone_internal.h"
#include "mmidatacounter_nv.h"
#include "mmicalendar_export.h"
#include "mmi_solarlunar.h"
#include "mmiacc_position.h"
#include "datacounter_image.h"


LOCAL MMI_RESULT_E HandleDataCounterCallAlertWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param);

WINDOW_TABLE( DATA_COUNTER_WIN_CALL_ALERT_TAB ) = 
{
	WIN_STATUSBAR,
	WIN_ID(MMIACC_DATA_COUNTER_WIN_CALL_LIMIT_ID),
	WIN_FUNC((uint32) HandleDataCounterCallAlertWinMsg),
	WIN_SOFTKEY(TXT_NULL, TXT_COMMON_OK, STXT_RETURN),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMI_DATACOUNTER_CALL_FORM_CTRL_ID),
	CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMI_DATACOUNTER_CALL_ALERT_LABEL_TEXT, MMI_DATACOUNTER_CALL_FORM_CTRL_ID),
	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMI_DATACOUNTER_CALL_SUB_FORM_CTRL_ID, MMI_DATACOUNTER_CALL_FORM_CTRL_ID),
	CHILD_EDIT_DIGITAL_CTRL(TRUE, DATACOUNTER_DATA_INPUT_MAX_LENGTH, MMI_DATACOUNTER_CALL_ALERT_LABEL_EDIT, MMI_DATACOUNTER_CALL_SUB_FORM_CTRL_ID),
	CHILD_TEXT_CTRL(TRUE, MMI_DATACOUNTER_CALL_ALERT_LABEL_HM, MMI_DATACOUNTER_CALL_SUB_FORM_CTRL_ID),
	END_WIN
};


#define LIMIT_LABLE_MARGIN     5
#define LIMIT_LABLE_HEIGHT     22

LOCAL void ReadCallNv(MMI_DATACOUNTER_DATA_SET_TYPE* setType)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    uint32 nvItem = 0;
    MMI_STRING_T minStr = {0};
    wchar* temp = L"100";
    MN_DUAL_SYS_E SimId = MMIAPIDataCounter_GetCallSimID();
    MMI_GetLabelTextByLang(STR_START_DEALY_MIN, &minStr);
	
    if (MMI_SmartDS_SIM_ID_1 == SimId)
    {
        switch(m_dataType.dataType)
        {
            case DATACOUNTER_OUTGOING_TYPE:
                nvItem = MMI_DATACOUNTER_SIM1_CALL_OUTGOING_LIMIT;
                break;
                
            case DATACOUNTER_INCOMING_TYPE:
                nvItem = MMI_DATACOUNTER_SIM1_CALL_INCOMING_LIMIT;
                break;
                
            case DATACOUNTER_ALL_TYPE:
                nvItem = MMI_DATACOUNTER_SIM1_CALL_ALL_LIMIT;
                break;
                
            default:
                nvItem = MMI_DATACOUNTER_SIM1_CALL_ALL_LIMIT;
                break;
        }
        
        MMINV_READ(nvItem, setType, return_value);
    }
    else if( MMI_SmartDS_SIM_ID_2 == SimId )
    {
        switch(m_dataType.dataType)
        {
            case DATACOUNTER_OUTGOING_TYPE:
                nvItem = MMI_DATACOUNTER_SIM2_CALL_OUTGOING_LIMIT;
                break;
            case DATACOUNTER_INCOMING_TYPE:
                nvItem = MMI_DATACOUNTER_SIM2_CALL_INCOMING_LIMIT;
                break;
            case DATACOUNTER_ALL_TYPE:
                nvItem = MMI_DATACOUNTER_SIM2_CALL_ALL_LIMIT;
                break;
            default:
                nvItem = MMI_DATACOUNTER_SIM2_CALL_ALL_LIMIT;
                break;
        }
        MMINV_READ(nvItem, setType, return_value);
    }

    if ( return_value != MN_RETURN_SUCCESS )
    { 
        setType->dataType = m_dataType;
        setType->unit = 0;
        MMIAPICOM_Wstrcpy(setType->limit, temp);
        MMINV_WRITE(nvItem, setType);
    }

	return;
}

LOCAL MMI_RESULT_E HandleDataCounterCallAlertWinMsg(
									MMI_WIN_ID_T win_id, 
									MMI_MESSAGE_ID_E msg_id, 
									DPARAM param
									)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_MENU_ID_T menu_id = 0;
    GUI_BG_T bg = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_DATACOUNTER_DATA_SET_TYPE setType = {0};
    MMI_STRING_T    statusBar_str = {0};
    GUI_RECT_T limit_label = {0};
    GUI_RECT_T limit_edit= {0};
    GUI_RECT_T limit_mb_gb ={0};
    MMI_STRING_T  limit_str    = {0};
    MMI_STRING_T  limit_mb_gb_str   = {0};
    uint32 limit_num = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    GUI_BORDER_T border = {0};
    IGUICTRL_T *ctrl_ptr = NULL;
    GUI_RECT_T   bg_rect = {0};
    GUI_BOTH_RECT_T both_client_rect = MMITHEME_GetWinClientBothRect(win_id);
    MN_DUAL_SYS_E SimId = MMIAPIDataCounter_GetCallSimID();
    GUIFORM_CHILD_WIDTH_T   osd_item_width = {0};
    GUI_BG_T bg1 = {0};
    MMI_STRING_T minStr = {0};
    MMI_STRING_T hourStr = {0};
    GUI_FONT_ALL_T FONT_ALL = {0};
    GUI_FONT_ALL_T FONT_ALL1 = {0};
    uint32 nvItem = 0;
    MMI_STRING_T edit_str = {0};
    MMI_TEXT_ID_T type_label_id = 0;
    GUI_COLOR_T color = MMI_WHITE_COLOR;
    uint16 hor_space = 35;
    uint16 ver_space = 0;
    uint16 cur_pos = 0;
    FONT_ALL.font= CAF_FONT_12;
    FONT_ALL.color = MMI_WHITE_COLOR;
    FONT_ALL1.font = CAF_FONT_12;
    FONT_ALL1.color = MMI_DARK_WHITE_COLOR;
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    MMI_GetLabelTextByLang(STR_START_DEALY_MIN, &minStr);
    MMI_GetLabelTextByLang(STR_COUN_UNIT_H_EXT02, &hourStr);
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        bg_rect = both_client_rect.h_rect;
    }
    else
    {
        bg_rect = both_client_rect.v_rect;
    }
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:	
            bg.bg_type = GUI_BG_COLOR;
            bg.color = MMI_BLACK_COLOR;
            GUIFORM_SetBg(MMI_DATACOUNTER_CALL_FORM_CTRL_ID, &bg);
            GUIFORM_SetChildTheme(MMI_DATACOUNTER_CALL_FORM_CTRL_ID, &bg, PNULL, &FONT_ALL);
            bg_rect.bottom = bg_rect.top + 70 - 6;			
            CTRLFORM_SetRect(MMI_DATACOUNTER_CALL_FORM_CTRL_ID , &bg_rect);	
            GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id, MMI_DATACOUNTER_CALL_FORM_CTRL_ID), &hor_space, &ver_space);

            ctrl_ptr = MMK_GetCtrlPtr(MMI_DATACOUNTER_CALL_FORM_CTRL_ID);
            IGUICTRL_SetProgress(ctrl_ptr, FALSE);

            GUILABEL_SetTextById(MMI_DATACOUNTER_CALL_ALERT_LABEL_TEXT,STR_COUN_LIST_LIMIT_EXT02,FALSE);
            //set limit edit
            IGUICTRL_SetCircularHandleLeftRight(MMK_GetCtrlPtr(MMI_DATACOUNTER_CALL_ALERT_LABEL_EDIT), FALSE);
            GUIEDIT_SetStyle(MMI_DATACOUNTER_CALL_ALERT_LABEL_EDIT, GUIEDIT_STYLE_SINGLE);
            CTRLBASEEDIT_SetIsDisplayInverse(MMI_DATACOUNTER_CALL_ALERT_LABEL_EDIT, TRUE);
            
            ReadCallNv(&setType);
            GUIEDIT_SetString(MMI_DATACOUNTER_CALL_ALERT_LABEL_EDIT, setType.limit, MMIAPICOM_Wstrlen(setType.limit));
            //set H/MIN unit
            GUITEXT_SetBg(MMI_DATACOUNTER_CALL_ALERT_LABEL_HM, &bg1);
            if (setType.unit == 0)
            {
                GUITEXT_SetString(MMI_DATACOUNTER_CALL_ALERT_LABEL_HM, minStr.wstr_ptr, minStr.wstr_len, FALSE);
            }
            else
            {
                GUITEXT_SetString(MMI_DATACOUNTER_CALL_ALERT_LABEL_HM, hourStr.wstr_ptr, hourStr.wstr_len, FALSE);
            }

            osd_item_width.type     = GUIFORM_CHILD_WIDTH_FIXED;
            osd_item_width.add_data = lcd_width -100 ;
            GUIFORM_SetChildWidth(MMI_DATACOUNTER_CALL_SUB_FORM_CTRL_ID, MMI_DATACOUNTER_CALL_ALERT_LABEL_EDIT,   &osd_item_width);
            osd_item_width.add_data = 60 ;
            GUIFORM_SetChildWidth(MMI_DATACOUNTER_CALL_SUB_FORM_CTRL_ID, MMI_DATACOUNTER_CALL_ALERT_LABEL_HM,   &osd_item_width);

            bg1.bg_type = GUI_BG_COLOR;
            bg1.color = MMI_BLACK_COLOR;        
            border.type = GUI_BORDER_NONE;
            CTRLFORM_SetActiveChildTheme(MMI_DATACOUNTER_CALL_SUB_FORM_CTRL_ID, &bg1, &border, &FONT_ALL1);
            CTRLFORM_SetChildTheme(MMI_DATACOUNTER_CALL_SUB_FORM_CTRL_ID, &bg1, &border, &FONT_ALL1);

            MMK_SetAtvCtrl(win_id, MMI_DATACOUNTER_CALL_ALERT_LABEL_EDIT);
            //GUIEDIT_SetSoftkey(MMI_DATACOUNTER_CALL_ALERT_LABEL_EDIT, 1, 1, TXT_NULL, TXT_COMMON_OK, PNULL);	      
            //GUIEDIT_SetSoftkeyEx(MMI_DATACOUNTER_CALL_ALERT_LABEL_EDIT, 2, 1, STXT_RETURN,TXT_DELETE, PNULL, PNULL, EDIT_BUTTON_TEXT_ID, PNULL);
            break;

       case MSG_FULL_PAINT:
            GUI_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR);
            limit_edit = bg_rect ; 
            limit_edit.bottom = bg_rect.top+70;
            limit_edit.top = limit_edit.bottom - 6;
            limit_edit.left +=6;
            limit_edit.right -=6;
            
            if( m_dataType.dataType == DATACOUNTER_OUTGOING_TYPE )
            {
                type_label_id = STR_COUN_LIST_CL_DIAL_EXT02;
            }
            else if( m_dataType.dataType == DATACOUNTER_INCOMING_TYPE )
            {
                type_label_id = STR_COUN_LIST_CL_RECEI_EXT02;
            }
            else if( m_dataType.dataType == DATACOUNTER_ALL_TYPE )
            {
                type_label_id = STR_DATA_ALERT_SETTINGS_ALL;
            }
            
            MMI_GetLabelTextByLang(type_label_id, &statusBar_str);
            if(MMIAPICOM_StatusAreaSetViewHeaderinfo(statusBar_str.wstr_ptr, statusBar_str.wstr_len)) 
            {
                GUIWIN_UpdateStb();
            }
            GUI_FillRect(&lcd_dev_info, limit_edit, MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT));
	   	break;	
        
	case MSG_KEYUP_RIGHT:
            cur_pos = GUIEDIT_GetCursorCurPos(MMI_DATACOUNTER_CALL_ALERT_LABEL_EDIT);
            GUIEDIT_GetString(MMI_DATACOUNTER_CALL_ALERT_LABEL_EDIT, &edit_str);
            if ( cur_pos < edit_str.wstr_len )
            {
                return recode;
            }
                        
            bg1.bg_type = GUI_BG_COLOR;
            bg1.color = MMI_WHITE_COLOR;        
            border.type = GUI_BORDER_NONE;
            GUIAPICTRL_SetState(MMI_DATACOUNTER_CALL_ALERT_LABEL_EDIT, GUICTRL_STATE_DISABLE_KEY | GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
            MMK_SetAtvCtrl(win_id, MMI_DATACOUNTER_CALL_ALERT_LABEL_HM);
            
            ctrl_ptr = MMK_GetCtrlPtr(MMI_DATACOUNTER_CALL_ALERT_LABEL_HM);
            IGUICTRL_SetBg(ctrl_ptr, &(bg1));
            IGUICTRL_SetBorder(ctrl_ptr, &(border));

            MMK_SendMsg(win_id, MSG_FULL_PAINT, 0 );
		break;
		
	case MSG_KEYUP_LEFT:
            bg1.bg_type = GUI_BG_COLOR;
            bg1.color = MMI_BLACK_COLOR;        
            border.type = GUI_BORDER_NONE;
            ctrl_ptr = MMK_GetCtrlPtr(MMI_DATACOUNTER_CALL_ALERT_LABEL_HM);
            IGUICTRL_SetBg(ctrl_ptr, &(bg1));
            IGUICTRL_SetBorder(ctrl_ptr, &(border));
            
            GUIAPICTRL_SetState(MMI_DATACOUNTER_CALL_ALERT_LABEL_EDIT, GUICTRL_STATE_DISABLE_KEY | GUICTRL_STATE_DISABLE_ACTIVE, FALSE);
            MMK_SetAtvCtrl(win_id, MMI_DATACOUNTER_CALL_ALERT_LABEL_EDIT);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, 0 );
		break;
        
	   case MSG_APP_OK:
			break;
			
	   case MSG_KEYUP_DOWN:
            if (MMK_IsActiveCtrl(MMI_DATACOUNTER_CALL_ALERT_LABEL_HM))
            {
                GUITEXT_GetString(MMI_DATACOUNTER_CALL_ALERT_LABEL_HM, &limit_mb_gb_str);
                if (0 == MMIAPICOM_CompareTwoWstr(limit_mb_gb_str.wstr_ptr, limit_mb_gb_str.wstr_len, minStr.wstr_ptr, minStr.wstr_len))
                {
                    GUITEXT_SetString(MMI_DATACOUNTER_CALL_ALERT_LABEL_HM, hourStr.wstr_ptr, hourStr.wstr_len, TRUE);
                }
                else
                {
                    GUITEXT_SetString(MMI_DATACOUNTER_CALL_ALERT_LABEL_HM, minStr.wstr_ptr, minStr.wstr_len, TRUE);
                } 

                MMK_SendMsg(MMI_DATACOUNTER_CALL_ALERT_LABEL_HM, MSG_CTL_PAINT, PNULL);
            }
	   	break;
	   	
	   case MSG_KEYUP_UP:
            if (MMK_IsActiveCtrl(MMI_DATACOUNTER_CALL_ALERT_LABEL_HM))
            {
                GUITEXT_GetString(MMI_DATACOUNTER_CALL_ALERT_LABEL_HM, &limit_mb_gb_str);
                if (0 == MMIAPICOM_CompareTwoWstr(limit_mb_gb_str.wstr_ptr, limit_mb_gb_str.wstr_len, minStr.wstr_ptr, minStr.wstr_len))
                {
                    GUITEXT_SetString(MMI_DATACOUNTER_CALL_ALERT_LABEL_HM, hourStr.wstr_ptr, hourStr.wstr_len, TRUE);
                }
                else
                {
                    GUITEXT_SetString(MMI_DATACOUNTER_CALL_ALERT_LABEL_HM, minStr.wstr_ptr, minStr.wstr_len, TRUE);
                } 

                MMK_SendMsg(MMI_DATACOUNTER_CALL_ALERT_LABEL_HM, MSG_CTL_PAINT, PNULL);
            }
            break;
            
	   case MSG_CTL_MIDSK:
            SCI_MEMSET(&setType, 0x00, sizeof(MMI_DATACOUNTER_DATA_SET_TYPE));
            GUITEXT_GetString(MMI_DATACOUNTER_CALL_ALERT_LABEL_HM, &limit_mb_gb_str);
            if (0 == MMIAPICOM_CompareTwoWstr(limit_mb_gb_str.wstr_ptr, limit_mb_gb_str.wstr_len, minStr.wstr_ptr, minStr.wstr_len))
            {
                setType.unit = 0;
            }
            else
            {
                setType.unit = 1;
            }
            //save set nv value
            GUIEDIT_GetString(MMI_DATACOUNTER_CALL_ALERT_LABEL_EDIT, &limit_str);
            if (limit_str.wstr_ptr != PNULL && limit_str.wstr_len > 0)
            {
                //MMIAPICOM_WstrToNum(limit_str.wstr_ptr, limit_str.wstr_len, &limit_num);
                if (MMI_SmartDS_SIM_ID_1 == SimId)
                {
                    switch (m_dataType.dataType)
                    {
                        case DATACOUNTER_OUTGOING_TYPE:
                            nvItem = MMI_DATACOUNTER_SIM1_CALL_OUTGOING_LIMIT;
                            break;
                            
                        case DATACOUNTER_INCOMING_TYPE:
                            nvItem = MMI_DATACOUNTER_SIM1_CALL_INCOMING_LIMIT;
                            break;
                            
                        case DATACOUNTER_ALL_TYPE:
                            nvItem = MMI_DATACOUNTER_SIM1_CALL_ALL_LIMIT;
                            break;
                            
                        default:
                            nvItem = MMI_DATACOUNTER_SIM1_CALL_ALL_LIMIT;
                            break;
                    }
                }
                else if( MMI_SmartDS_SIM_ID_2 == SimId )
                {
                    switch(m_dataType.dataType)
                    {
                        case DATACOUNTER_OUTGOING_TYPE:
                            nvItem = MMI_DATACOUNTER_SIM2_CALL_OUTGOING_LIMIT;
                            break;
                            
                         case DATACOUNTER_INCOMING_TYPE:
                            nvItem = MMI_DATACOUNTER_SIM2_CALL_INCOMING_LIMIT;
                            break;
                            
                         case DATACOUNTER_ALL_TYPE:
                            nvItem = MMI_DATACOUNTER_SIM2_CALL_ALL_LIMIT;
                            break;
                            
                          default:
                            nvItem = MMI_DATACOUNTER_SIM2_CALL_ALL_LIMIT;
                            break;
                    }
                }

                setType.dataType.dataType = m_dataType.dataType;
                setType.dataType.simID = SimId;
                MMIAPICOM_Wstrcpy(setType.limit, limit_str.wstr_ptr);
                MMINV_WRITE(nvItem, &setType);
                ResetCallShowJustOne(SimId, m_dataType.dataType, FALSE);

                MMK_CloseWin(win_id);
            }
            break;

        case MSG_CTL_OK:	
        	break;
        	
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            MMK_PostMsg(MMI_DATACOUNTER_SIM_RESULT_CTRL_ID, MSG_APP_RED, PNULL, 0);
            break;

        case MSG_APP_RED:
	        MMK_PostMsg(MMI_DATACOUNTER_SIM_RESULT_CTRL_ID, MSG_APP_RED, PNULL, 0);
	    break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;

    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : MMIAPIDATACOUNTER_OpenDataAlertWin
//         
//	Global resource dependence : 
//   Author:        zhanglu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIDATACOUNTER_OpenCallAlertWin(void* param)
{
    m_dataType = *((MMI_DATACOUNTER_DATA_E*)param);
    MMIAPIDataCounter_SetCallSimID(m_dataType.simID);
    MMK_CreateWin((uint32*)DATA_COUNTER_WIN_CALL_ALERT_TAB,(ADD_DATA)param);
    

}
//#else
/*****************************************************************************/
// 	Description : MMIAPIDATACOUNTER_OpenDataWin
//
//	Global resource dependence :
//   Author:        zhanglu
//	Note:
/*****************************************************************************/
/*
PUBLIC void MMIAPIDATACOUNTER_OpenDataWin(uint32 simcard)
{

}*/
//#endif//DATA_COUNTER_SUPPORT


/*Edit by script, ignore 3 case. Fri Apr 27 09:38:47 2012*/ //IGNORE9527
