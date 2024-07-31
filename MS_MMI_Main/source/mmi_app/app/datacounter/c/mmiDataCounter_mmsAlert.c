/****************************************************************************
** File Name:      mmiDataCounter_mmsAlert.c                            
** Author:                                                                  
/****************************************************************************
** File Name:      mmiDataCounter_smsAlert.c                            
** Author:                                                                  
** Date:            2015/08/24
** Description:    This file is used to process msg to main window of CALCULATION
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
****************************************************************************/
#define _MMIDATACOUNTER_MMS_ALERT_C_  

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


LOCAL MMI_RESULT_E HandleDataCounterMmsAlertWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param);

WINDOW_TABLE( DATA_COUNTER_WIN_MMS_ALERT_TAB ) = 
{
	WIN_STATUSBAR,
	WIN_ID(MMIACC_DATA_COUNTER_WIN_MMS_LIMIT_ID),
	WIN_FUNC((uint32) HandleDataCounterMmsAlertWinMsg),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMI_DATACOUNTER_MMS_FORM_CTRL_ID),
	CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMI_DATACOUNTER_MMS_ALERT_LABEL_TEXT, MMI_DATACOUNTER_MMS_FORM_CTRL_ID),
	CHILD_EDIT_DIGITAL_CTRL(TRUE, DATACOUNTER_SMS_MMS_INPUT_MAX_LENGTH, MMI_DATACOUNTER_MMS_ALERT_LABEL_EDIT, MMI_DATACOUNTER_MMS_FORM_CTRL_ID),
	WIN_SOFTKEY(TXT_NULL, TXT_COMMON_OK, STXT_RETURN), 
	END_WIN
};

LOCAL void ReadMMSNv(wchar* limit_num)
{
      MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
	uint32 nvItem = 0;
	MMI_DATACOUNTER_DATA_SET_TYPE setType = {0};
	wchar* temp = L"100";
    MN_DUAL_SYS_E SimId = MMIAPIDataCounter_GetMMSSimID();
	
      if ( MMI_SmartDS_SIM_ID_1 == SimId)
     {
        switch( m_dataType.dataType )
        {
            case DATACOUNTER_SENT_TYPE:
                nvItem = MMI_DATACOUNTER_SIM1_MMS_SENT_LIMIT;
                break;
            case DATACOUNTER_REV_TYPE:
                nvItem = MMI_DATACOUNTER_SIM1_MMS_RECEIVED_LIMIT;
                break;
            case DATACOUNTER_ALL_TYPE:
                nvItem = MMI_DATACOUNTER_SIM1_MMS_ALL_LIMIT;
                break;
            default:
                nvItem = MMI_DATACOUNTER_SIM1_MMS_ALL_LIMIT;
                break;
        }   
        MMINV_READ(nvItem, &setType, return_value);	      
     }
     else if( MMI_SmartDS_SIM_ID_2 == SimId )
     {
        switch( m_dataType.dataType )
        {
            case DATACOUNTER_SENT_TYPE:
                nvItem = MMI_DATACOUNTER_SIM2_MMS_SENT_LIMIT;
                break;
            case DATACOUNTER_REV_TYPE:
                nvItem = MMI_DATACOUNTER_SIM2_MMS_RECEIVED_LIMIT;
                break;
            case DATACOUNTER_ALL_TYPE:
                nvItem = MMI_DATACOUNTER_SIM2_MMS_ALL_LIMIT;
                break;
            default:
                nvItem = MMI_DATACOUNTER_SIM2_MMS_ALL_LIMIT;
                break;
        }   
           MMINV_READ(nvItem, &setType, return_value);
      }

      if ( return_value != MN_RETURN_SUCCESS )
      { 
	     setType.dataType = m_dataType;
	     MMIAPICOM_Wstrcpy(setType.limit, temp);
	     MMIAPICOM_Wstrcpy(limit_num, temp);
	     MMINV_WRITE(nvItem, &setType);

      }
	else
	{
	    MMIAPICOM_Wstrcpy(limit_num, setType.limit );
	}
}
LOCAL MMI_RESULT_E HandleDataCounterMmsAlertWinMsg(
									MMI_WIN_ID_T win_id, 
									MMI_MESSAGE_ID_E msg_id, 
									DPARAM param
									)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_MENU_ID_T menu_id = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_BG_T bg = {0};
    MMI_DATACOUNTER_DATA_SET_TYPE setType = {0};
    MMI_STRING_T    statusBar_str = {0};
    GUI_RECT_T limit_label = {0};
    GUI_RECT_T limit_edit= {0};
    MMI_STRING_T  limit_str    = {0};
    uint32 limit_num = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    wchar inputInfo[DATACOUNTER_SMS_MMS_INPUT_MAX_LENGTH] = {0};
    GUIFORM_CHILD_WIDTH_T   osd_item_width = {0};
    GUI_FONT_ALL_T FONT_ALL = {0};
    GUI_FONT_ALL_T FONT_ALL1 = {0};
    GUI_BORDER_T border = {0};
    IGUICTRL_T *ctrl_ptr = NULL;
    GUI_RECT_T   bg_rect = {0};
    uint16 hor_space = 0;
    uint16 ver_space = 0;
    uint16 cur_pos = 0;
    GUI_BOTH_RECT_T both_client_rect = MMITHEME_GetWinClientBothRect(win_id);
    MN_DUAL_SYS_E SimId = MMIAPIDataCounter_GetMMSSimID();
    uint32 nvItem = 0;
    MMI_STRING_T edit_str = {0};
    MMI_TEXT_ID_T type_label_id = 0;
    GUI_COLOR_T color = MMI_WHITE_COLOR;
    FONT_ALL.font= CAF_FONT_12;
    FONT_ALL.color = MMI_DARK_WHITE_COLOR;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
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
            border.type = GUI_BORDER_NONE;
            GUIFORM_SetBg(MMI_DATACOUNTER_MMS_FORM_CTRL_ID, &bg);
            CTRLFORM_SetActiveChildTheme(MMI_DATACOUNTER_MMS_FORM_CTRL_ID, &bg, &border, &FONT_ALL);
		GUIFORM_SetChildTheme(MMI_DATACOUNTER_MMS_FORM_CTRL_ID, &bg, &border, &FONT_ALL);
	   	//set limit label
		//GUITEXT_SetFont(MMI_DATACOUNTER_DATA_ALERT_LABEL_TEXT, &font, &color);
		 bg_rect.bottom = bg_rect.top + 70 - 6;			
            CTRLFORM_SetRect(MMI_DATACOUNTER_MMS_FORM_CTRL_ID , &bg_rect);	
            GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id, MMI_DATACOUNTER_MMS_FORM_CTRL_ID), &hor_space, &ver_space);
            ctrl_ptr = MMK_GetCtrlPtr(MMI_DATACOUNTER_MMS_FORM_CTRL_ID);
            IGUICTRL_SetProgress(ctrl_ptr, FALSE);

            GUILABEL_SetTextById(MMI_DATACOUNTER_MMS_ALERT_LABEL_TEXT,STR_COUN_LIST_LIMIT_EXT02,FALSE);
            //set limit edit
            GUIEDIT_SetStyle(MMI_DATACOUNTER_MMS_ALERT_LABEL_EDIT, GUIEDIT_STYLE_SINGLE);
	     CTRLBASEEDIT_SetIsDisplayInverse(MMI_DATACOUNTER_MMS_ALERT_LABEL_EDIT, TRUE);
            //GUIEDIT_SetBg(MMI_DATACOUNTER_DATA_ALERT_LABEL_EDIT, &bg1);
            ReadMMSNv(inputInfo);
            //MMIAPICOM_Int2Str(limit_num, temp, 10);
            //MMIAPICOM_StrToWstr(temp, inputInfo);
            GUIEDIT_SetString(MMI_DATACOUNTER_MMS_ALERT_LABEL_EDIT, inputInfo, MMIAPICOM_Wstrlen(inputInfo));
            osd_item_width.type     = GUIFORM_CHILD_WIDTH_FIXED;
            osd_item_width.add_data = lcd_width -50 ;
            GUIFORM_SetChildWidth(MMI_DATACOUNTER_MMS_FORM_CTRL_ID, MMI_DATACOUNTER_MMS_ALERT_LABEL_EDIT,   &osd_item_width);
            MMK_SetAtvCtrl(win_id, MMI_DATACOUNTER_MMS_ALERT_LABEL_EDIT);
            GUIEDIT_SetSoftkey(MMI_DATACOUNTER_MMS_ALERT_LABEL_EDIT, 1, 1, TXT_NULL, TXT_COMMON_OK, PNULL);
            GUIEDIT_SetSoftkeyEx(MMI_DATACOUNTER_MMS_ALERT_LABEL_EDIT, 2, 1, STXT_RETURN,TXT_DELETE, PNULL, PNULL, EDIT_BUTTON_TEXT_ID, PNULL);
            break;

       case MSG_FULL_PAINT:
                GUI_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR);

                limit_edit = bg_rect ; 
                limit_edit.bottom = bg_rect.top+70;
                limit_edit.top = limit_edit.bottom - 6;
                limit_edit.left +=6;
                limit_edit.right -=6;
                if( m_dataType.dataType == DATACOUNTER_SENT_TYPE )
                {
                    type_label_id = STR_DATA_ALERT_SETTINGS_SENT;
                }
                else if( m_dataType.dataType == DATACOUNTER_REV_TYPE )
                {
                    type_label_id = STR_DATA_ALERT_SETTINGS_RECV;
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
        
	case MSG_CTL_MIDSK:
            //save set nv value
            GUIEDIT_GetString(MMI_DATACOUNTER_MMS_ALERT_LABEL_EDIT, &limit_str);
            if ( limit_str.wstr_ptr != PNULL && limit_str.wstr_len > 0 )
            {
                //MMIAPICOM_WstrToNum(limit_str.wstr_ptr, limit_str.wstr_len, &limit_num);
                if ( MMI_SmartDS_SIM_ID_1 == SimId)
                {
                    switch( m_dataType.dataType )
                    {
                        case DATACOUNTER_SENT_TYPE:
                            nvItem = MMI_DATACOUNTER_SIM1_MMS_SENT_LIMIT;
                            break;
                         case DATACOUNTER_REV_TYPE:
                            nvItem = MMI_DATACOUNTER_SIM1_MMS_RECEIVED_LIMIT;
                            break;
                         case DATACOUNTER_ALL_TYPE:
                            nvItem = MMI_DATACOUNTER_SIM1_MMS_ALL_LIMIT;
                            break;
                          default:
                            nvItem = MMI_DATACOUNTER_SIM1_MMS_ALL_LIMIT;
                            break;
                    }
                }
                else if( MMI_SmartDS_SIM_ID_2 == SimId )
                {
                    switch( m_dataType.dataType )
                    {
                        case DATACOUNTER_SENT_TYPE:
                            nvItem = MMI_DATACOUNTER_SIM2_MMS_SENT_LIMIT;
                            break;
                         case DATACOUNTER_REV_TYPE:
                            nvItem = MMI_DATACOUNTER_SIM2_MMS_RECEIVED_LIMIT;
                            break;
                         case DATACOUNTER_ALL_TYPE:
                            nvItem = MMI_DATACOUNTER_SIM2_MMS_ALL_LIMIT;
                            break;
                          default:
                            nvItem = MMI_DATACOUNTER_SIM2_MMS_ALL_LIMIT;
                            break;
                    }
                }

                setType.dataType = m_dataType;
                MMIAPICOM_Wstrcpy(setType.limit, limit_str.wstr_ptr);
                MMINV_WRITE(nvItem, &setType);
                ResetMmsShowJustOne(SimId, m_dataType.dataType, FALSE);
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
// 	Description : MMIAPIDATACOUNTER_OpenSmsAlertWin
//         
//	Global resource dependence : 
//   Author:        zhanglu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIDATACOUNTER_OpenMmsAlertWin(void* param)
{
    m_dataType = *((MMI_DATACOUNTER_DATA_E*)param);
    MMIAPIDataCounter_SetMMSSimID(m_dataType.simID);
    MMK_CreateWin((uint32*)DATA_COUNTER_WIN_MMS_ALERT_TAB,(ADD_DATA)param);
    

}
//#else
/*****************************************************************************/
// 	Description : MMIAPIDATACOUNTER_OpenSmsAlertWin
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

