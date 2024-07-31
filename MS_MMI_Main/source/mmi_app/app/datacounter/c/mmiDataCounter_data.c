/****************************************************************************
** File Name:      mmiDataCounter_data.c                            
** Author:                                                                  
** Date:            2015/08/24
** Description:    This file is used to process msg to main window of CALCULATION
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
****************************************************************************/
#define _MMIDATACOUNTER_DATA_C_  

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_datacounter_trc.h"
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

#include "mmi_resource.h"
#include "guistring.h"
#include "mmipdp_export.h"

#include "datacounter_text.h"
#include "datacounter_menutable.h"
#include "datacounter_internal.h"
#include "datacounter_id.h"
#include "mmiphone_export.h"
#include "mmiphone_internal.h"
#include "mmidatacounter_nv.h"
#include "mmipub.h"

LOCAL MN_DUAL_SYS_E m_data_simId = MMI_SmartDS_SIM_ID_1;
PUBLIC void MMIAPIDataCounter_SetDataSimID(MN_DUAL_SYS_E SimId)
{
    m_data_simId = SimId;
    
    return;
}

PUBLIC MN_DUAL_SYS_E MMIAPIDataCounter_GetDataSimID(void)
{
    return m_data_simId;
}

LOCAL MMI_RESULT_E HandleDataCounterDataWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param);

LOCAL MMI_RESULT_E HandleClearDataCounterWinMsg(
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM           param
                                           );
LOCAL void MMIAPIDataCounter_readRecordDataNV(MMI_DATACOUNTER_RECORD_DATA_NV_E* record_data_nv);
LOCAL void SetDataLabelRect(void);
LOCAL MMI_RESULT_E HandleDataSetWinMsg(
									MMI_WIN_ID_T win_id, 
									MMI_MESSAGE_ID_E msg_id, 
									DPARAM param
									);
LOCAL void MMIDataCounter_DataItems();
LOCAL uint32 ReadAlertValue(MN_DUAL_SYS_E dual_sys, uint8 dataType);

LOCAL MMI_RESULT_E HandleOpenDataQuery(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             );

PUBLIC void MMIAPIDataCounter_dataAlertNotifaction(MN_DUAL_SYS_E dual_sys, DATACOUNTER_DATATYPE_E dataType);

WINDOW_TABLE( DATA_COUNTER_WIN_DATA_TAB ) = 
{
    WIN_STATUSBAR,
    WIN_ID(MMIACC_DATA_COUNTER_WIN_DATA_ID),
    WIN_FUNC((uint32) HandleDataCounterDataWinMsg),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMI_DATACOUNTER_SIM_DATA_ALERT_CTRL_ID),
    //All
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMI_DATACOUNTER_DATA_LABEL_ALL),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_RIGHT, MMI_DATACOUNTER_DATA_LABEL_ALL_TEXT),
    //Sent
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMI_DATACOUNTER_DATA_LABEL_SENT),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_RIGHT, MMI_DATACOUNTER_DATA_LABEL_SENT_TEXT),
    //Received
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMI_DATACOUNTER_DATA_LABEL_RECEIVED),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_RIGHT, MMI_DATACOUNTER_DATA_LABEL_RECEIVED_TEXT),
    WIN_SOFTKEY(STXT_OPTION, STXT_SOFTKEY_SELECT_MK, STXT_RETURN),

    END_WIN
};


WINDOW_TABLE(MMITEST_CLEAR_DATA_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleClearDataCounterWinMsg),
    WIN_ID(MMIACC_DATA_COUNTER_WIN_DATA_MENU_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_HIDE_STATUS,
    CREATE_POPMENU_CTRL(MENU_DATACOUNTER_CLEAR_DATA_TABLE_ID,MMI_DATACOUNTER_CLEAR_DATA_COUNTER_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL,STXT_SOFTKEY_SELECT_MK,STXT_RETURN),
    
    END_WIN
};

WINDOW_TABLE(MMITEST_SET_DATA_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleDataSetWinMsg),
    WIN_ID(MMIACC_DATA_COUNTER_WIN_SET_DATA_MENU_ID),
    //CREATE_POPMENU_CTRL(MENU_DATACOUNTER_DATA_ALERT_SET_TABLE_ID,MMI_DATACOUNTER_DATA_SET_CTRL_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT|WS_DISABLE_COMMON_BG),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMITEST_CLEAR_DATA_CONFIRM_WIN_TAB) =
{
	WIN_ID(MMIACC_DATA_COUNTER_WIN_CLEAR_DATA_ID),
	WIN_STATUSBAR,
	WIN_STYLE(WS_DISABLE_RETURN_WIN),
	END_WIN
};

LOCAL MMI_RESULT_E HandleClearDataQuery(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE; 
    switch (msg_id)
    {
       case MSG_OPEN_WINDOW:
	        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);	
              break;
	case MSG_GET_FOCUS:
		 break;
	case MSG_CTL_OK:
	case MSG_APP_WEB:
	case MSG_NOTIFY_MIDSK:
	case MSG_APP_OK:
              ClearDataNV(2, m_data_simId);
	        MMK_CloseWin(win_id);
		break;
       case MSG_APP_CANCEL:
		if (win_id == MMK_GetFocusWinId())
		{
			MMK_CloseWin(win_id);
		}		
		break;
       default:
		recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return recode;
}

LOCAL void Clear_DataConfirmationDialogWindow(void)
{
    MMI_WIN_ID_T     win_id_query = MMIACC_DATA_COUNTER_WIN_CLEAR_DATA_ID;
    MMIPUB_HANDLE_FUNC     handle_func = HandleClearDataQuery;
    MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_COUN_CLEAR_DATA_EXT01, PNULL, PNULL, &win_id_query,PNULL, MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,handle_func);
}

LOCAL void OpenSetAlertSelectMenu(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id)
{
    GUIMENU_POP_SEL_INFO_T  pop_info = {0};
    MMI_STRING_T kstring = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};

    pop_info.is_static = FALSE;
    pop_info.ctrl_id = ctrl_id;
    
    GUIMENU_CreatDynamic(PNULL,win_id,ctrl_id,GUIMENU_STYLE_POPUP_RADIO);
    MMI_GetLabelTextByLang(STR_COUN_ALERT_OFF_EXT01, &kstring);
    node_item.item_text_ptr = &kstring; 
    node_item.is_grayed = FALSE;
    
    GUIMENU_InsertNode(0,MMI_DATACOUNTER_DATA_ALERT_SET_OFF_MENU,0,&node_item,ctrl_id);
    pop_info.node_id =MMI_DATACOUNTER_DATA_ALERT_SET_OFF_MENU;
    GUIMENU_SetPopItemStatus(select_index == 0,&pop_info);
        
    MMI_GetLabelTextByLang(STR_DATA_ALERT_SETTINGS_ALL, &kstring);
    node_item.item_text_ptr = &kstring; 
    GUIMENU_InsertNode(1,MMI_DATACOUNTER_DATA_ALERT_SET_ALL_MENU,0,&node_item,ctrl_id);
    pop_info.node_id =MMI_DATACOUNTER_DATA_ALERT_SET_ALL_MENU;
    GUIMENU_SetPopItemStatus(select_index == 1,&pop_info);

    MMI_GetLabelTextByLang(STR_DATA_ALERT_SETTINGS_SENT, &kstring);
    node_item.item_text_ptr = &kstring; 
    GUIMENU_InsertNode(2,MMI_DATACOUNTER_DATA_ALERT_SET_SENT_MENU,0,&node_item,ctrl_id);
    pop_info.node_id =MMI_DATACOUNTER_DATA_ALERT_SET_SENT_MENU;
    GUIMENU_SetPopItemStatus(select_index == 2,&pop_info);

    MMI_GetLabelTextByLang(STR_DATA_ALERT_SETTINGS_RECV, &kstring);
    node_item.item_text_ptr = &kstring; 
    GUIMENU_InsertNode(3,MMI_DATACOUNTER_DATA_ALERT_SET_RECEIVED_MENU,0,&node_item,ctrl_id);
    pop_info.node_id =MMI_DATACOUNTER_DATA_ALERT_SET_RECEIVED_MENU;
    GUIMENU_SetPopItemStatus(select_index == 3,&pop_info);

    GUIMENU_SetPopMenuCentered(ctrl_id,TRUE);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id,TXT_NULL,STXT_SOFTKEY_SELECT_MK,STXT_RETURN);
}

LOCAL MMI_RESULT_E HandleDataSetWinMsg(
									MMI_WIN_ID_T win_id, 
									MMI_MESSAGE_ID_E msg_id, 
									DPARAM param
									)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMI_DATACOUNTER_DATA_SET_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    uint8 menu_index = 0;
    uint32 item_id = 0;   
    MMI_DATACOUNTER_DATA_E dataType = {0};
    MMI_STRING_T statusBar_str = {0};
    
    MN_DUAL_SYS_E simID = m_data_simId;
    dataType.simID = m_data_simId;
    switch(msg_id)
    {
       case MSG_OPEN_WINDOW:
            OpenSetAlertSelectMenu(win_id, ctrl_id);
            CTRLMENU_SetSelectedItem(ctrl_id, select_index);
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;

       case MSG_FULL_PAINT:
            MMI_GetLabelTextByLang(STR_HDR_SET_ALERT, &statusBar_str);
            GUIMENU_SetMenuTitle(&statusBar_str, ctrl_id);
	   break;	

	   case MSG_APP_OK:
			break;
	   case MSG_CTL_MIDSK:
	   		item_id = GUIMENU_GetCurNodeId(ctrl_id);       
			switch(item_id)
			{
				case MMI_DATACOUNTER_DATA_ALERT_SET_OFF_MENU:
					menu_index = 0;
					break;
				case MMI_DATACOUNTER_DATA_ALERT_SET_ALL_MENU:
					menu_index = 1;
                                 dataType.dataType = 2;
					MMIAPIDATACOUNTER_OpenDataAlertWin(&dataType);
					break;
				case MMI_DATACOUNTER_DATA_ALERT_SET_SENT_MENU:
					menu_index = 2;
					dataType.dataType = 0;
					MMIAPIDATACOUNTER_OpenDataAlertWin(&dataType);
					break;
				case MMI_DATACOUNTER_DATA_ALERT_SET_RECEIVED_MENU:
					menu_index = 3;
					dataType.dataType = 1;
					MMIAPIDATACOUNTER_OpenDataAlertWin(&dataType);
					break;
				default:
					;
					break;
			}
			
			if ( MMI_SmartDS_SIM_ID_1 == m_data_simId )
			{
				MMINV_WRITE(MMI_DATACOUNTER_SIM1_DATA_SAVE_SELECT_ITEM, &menu_index);
			}
			else if( MMI_SmartDS_SIM_ID_2 == m_data_simId )
			{
                           MMINV_WRITE(MMI_DATACOUNTER_SIM2_DATA_SAVE_SELECT_ITEM, &menu_index);
			}

                    select_index = menu_index;

			MMK_CloseWin(win_id);
			
	   	break;

       case MSG_CTL_OK:	
	   	break;
       case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
	     MMK_CloseWin(win_id);
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
LOCAL MMI_RESULT_E HandleClearDataCounterWinMsg(
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM           param
                                           )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMI_DATACOUNTER_CLEAR_DATA_COUNTER_MENU_CTRL_ID;
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_CTL_OK:
        break;
    case MSG_CTL_MIDSK:
	 Clear_DataConfirmationDialogWindow();
	 MMK_CloseWin(win_id);
		break;
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

//read storage data type
LOCAL void readRevData(MMI_DATACOUNTER_DATA_SET_TYPE* setType, MN_DUAL_SYS_E simCard)
{
      MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
      uint32 nvItem = 0;
      if ( MMI_SmartDS_SIM_ID_1 == simCard )
      {
        nvItem = MMI_DATACOUNTER_SIM1_DATA_RECEIVED_LIMIT;
      }
      else if( MMI_SmartDS_SIM_ID_2 == simCard  )
      {
         nvItem = MMI_DATACOUNTER_SIM2_DATA_RECEIVED_LIMIT;
      }

      MMINV_READ(nvItem, setType, return_value);

      if ( return_value != MN_RETURN_SUCCESS   )
      {
        SCI_MEMSET(setType, 0x00, sizeof(MMI_DATACOUNTER_DATA_SET_TYPE));
        MMINV_WRITE(nvItem, setType);
      }
	
}

LOCAL void readSentData(MMI_DATACOUNTER_DATA_SET_TYPE* setType, MN_DUAL_SYS_E simCard)
{
      MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
      uint32 nvItem = 0;
      if ( MMI_SmartDS_SIM_ID_1 == simCard )
      {
           nvItem = MMI_DATACOUNTER_SIM1_DATA_SENT_LIMIT;
      }
      else if( MMI_SmartDS_SIM_ID_2 == simCard  )
      {
            nvItem = MMI_DATACOUNTER_SIM2_DATA_SENT_LIMIT;
      }

	MMINV_READ(nvItem, setType, return_value);
}

LOCAL void readALLData(MMI_DATACOUNTER_DATA_SET_TYPE* setType, MN_DUAL_SYS_E simCard)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    uint32 nvItem = 0;
    if ( MMI_SmartDS_SIM_ID_1 == simCard )
    {
       nvItem = MMI_DATACOUNTER_SIM1_DATA_ALL_LIMIT;
    }
    else if( MMI_SmartDS_SIM_ID_2 == simCard  )
    {
        nvItem = MMI_DATACOUNTER_SIM2_DATA_ALL_LIMIT;
    }

    MMINV_READ(nvItem, setType, return_value);

}

LOCAL void readSetDataStorageDataType(uint8* select_index)
{
      MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
	uint32 nvItem = 0;
	MMI_DATACOUNTER_DATA_SET_TYPE setType = {0};
	wchar* temp = L"100";
	if ( MMI_SmartDS_SIM_ID_1 == m_data_simId )
	{
	    nvItem = MMI_DATACOUNTER_SIM1_DATA_SAVE_SELECT_ITEM;
	}
	else if( MMI_SmartDS_SIM_ID_2 == m_data_simId )
	{
          nvItem = MMI_DATACOUNTER_SIM2_DATA_SAVE_SELECT_ITEM;
	}

	MMINV_READ(nvItem, select_index, return_value);
	if ( return_value != MN_RETURN_SUCCESS  )
	{
           *select_index = 0;
	    MMINV_WRITE(nvItem, select_index);
	}
	
}

#define NETWORKNAME_BUF_MAX_LEN   50

LOCAL void MMIDataCounter_DataItems()
{
    uint32 i = 0; 
    MMI_CTRL_ID_T ctrl_id = MMI_DATACOUNTER_SIM_DATA_ALERT_CTRL_ID;
    GUILIST_ITEM_T item_t = {0};
    MMI_TEXT_ID_T text_id = 0;
    wchar strBuffer[11] = {0};
    MMI_STRING_T dataTypeStr = {0};
    MMI_STRING_T mbStr = {0};
    wchar text_buffer[10] = {0};
    MMI_STRING_T result_string = {0};
    MMI_DATACOUNTER_DATA_SET_TYPE setType = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    item_t.item_data_ptr = &item_data;

    GUILIST_SetMaxItem(ctrl_id, 1, FALSE);
    GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);

    MMI_GetLabelTextByLang(STR_ALERT_VALUE_INPUT_FORMAT, &mbStr);
    switch(select_index)
    {
        case 0:
            text_id = STR_COUN_ALERT_OFF_EXT01;
            break;
            
        case 1:
            text_id = STR_DATA_ALERT_SETTINGS_ALL;
            //get all
            readALLData(&setType, m_data_simId);
            if ( setType.unit == 0 )
            {
                MMI_GetLabelTextByLang(STR_GAL_LIST_MB, &dataTypeStr);
            }
            else
            {
                MMI_GetLabelTextByLang(STR_GAL_LIST_GB, &dataTypeStr);
            }
            result_string.wstr_ptr = strBuffer;
            MMIAPICOM_CustomGenerateReplaceStr(&dataTypeStr, L"%U", setType.limit, sizeof(strBuffer)/sizeof(wchar), &result_string);
            result_string.wstr_len = MMIAPICOM_Wstrlen(result_string.wstr_ptr);
            break;
            
        case 2:
            text_id = STR_DATA_ALERT_SETTINGS_SENT;
            //get sent
            readSentData(&setType, m_data_simId);
            if ( setType.unit == 0 )
            {
                MMI_GetLabelTextByLang(STR_GAL_LIST_MB, &dataTypeStr);
            }
            else
            {
                MMI_GetLabelTextByLang(STR_GAL_LIST_GB, &dataTypeStr);
            }
            result_string.wstr_ptr = strBuffer;
            MMIAPICOM_CustomGenerateReplaceStr(&dataTypeStr, L"%U", setType.limit, sizeof(strBuffer)/sizeof(wchar), &result_string);
            result_string.wstr_len = MMIAPICOM_Wstrlen(result_string.wstr_ptr);
            break;
            
        case 3:
            text_id = STR_DATA_ALERT_SETTINGS_RECV;
            //get rev
            readRevData(&setType, m_data_simId);
            if (setType.unit == 0)
            {
                MMI_GetLabelTextByLang(STR_GAL_LIST_MB, &dataTypeStr);
            }
            else
            {
                MMI_GetLabelTextByLang(STR_GAL_LIST_GB, &dataTypeStr);
            }
            result_string.wstr_ptr = strBuffer;
            MMIAPICOM_CustomGenerateReplaceStr(&dataTypeStr, L"%U", setType.limit, sizeof(strBuffer)/sizeof(wchar), &result_string);
            result_string.wstr_len = MMIAPICOM_Wstrlen(result_string.wstr_ptr);
            break;
            
        default:
            break;
    }
    //set item "Counter period"
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_2TEXT_MS_EXT;
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_COUN_LIST_ALERT_EXT02;
    item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id = text_id;
    item_data.item_content[1].is_custom_font_color = TRUE;
    item_data.item_content[1].custom_font_color_focus = MMI_DARK_GRAY_COLOR;

    if ( select_index != 0 )
    {
        item_data.item_content[2].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer = result_string;
        item_data.item_content[2].is_custom_font_color = TRUE;
        item_data.item_content[2].custom_font_color_focus = MMI_DARK_GRAY_COLOR;
    }

    GUILIST_InsertItem(ctrl_id, &item_t, 0);  

    return;
}

LOCAL void SetDataLabelRect(void)
{
    GUI_RECT_T rect = {0};
    uint32 alertValue = 0;//警介值
    uint32 alertActualValue = 0;//实际值
    MMI_DATACOUNTER_RECORD_DATA_NV_E record_data_nv = {0};
    GUI_FONT_T font_size = CAF_FONT_16;
    GUI_COLOR_T font_color = MMI_DARK_GRAY_COLOR;
    GUI_COLOR_T font_color_ex = MMI_BLUE_COLOR;

    MMIAPIDataCounter_readRecordDataNV(&record_data_nv);
    
    switch( select_index )
    {
        case 1:
            MMIAPICOM_WstrToNum(record_data_nv.buffer_ALL, MMIAPICOM_Wstrlen(record_data_nv.buffer_ALL), &alertActualValue);
            alertValue = ReadAlertValue(m_data_simId, 2);
            break;
        case 2:
            MMIAPICOM_WstrToNum(record_data_nv.buffer_send_outgoing, MMIAPICOM_Wstrlen(record_data_nv.buffer_send_outgoing), &alertActualValue);
            alertValue = ReadAlertValue(m_data_simId, 0);
            break;
        case 3:
            MMIAPICOM_WstrToNum(record_data_nv.buffer_recv_incoming, MMIAPICOM_Wstrlen(record_data_nv.buffer_recv_incoming), &alertActualValue);
            alertValue = ReadAlertValue(m_data_simId, 1);
            break;
        default:
            break;
    }
    
    // 1st line--All
    rect.left = DATACOUNTER_EDIT_X1;
    rect.top = DATACOUNTER_EDIT_Y1;
    rect.right = DATACOUNTER_EDIT_X2 - 5;
    rect.bottom = DATACOUNTER_EDIT_Y1 + DATACOUNTER_EDIT_HIGHT - 1;
    GUILABEL_SetRect(MMI_DATACOUNTER_DATA_LABEL_ALL, &rect, FALSE);
    GUILABEL_SetFont(MMI_DATACOUNTER_DATA_LABEL_ALL, font_size, font_color);
    GUILABEL_SetFont(MMI_DATACOUNTER_DATA_LABEL_ALL_TEXT, font_size, font_color);
    if ( select_index == 1 && (alertActualValue >= alertValue) )
    {
        GUILABEL_SetFont(MMI_DATACOUNTER_DATA_LABEL_ALL, font_size, font_color_ex);
        GUILABEL_SetFont(MMI_DATACOUNTER_DATA_LABEL_ALL_TEXT, font_size, font_color_ex);
    }
    
    rect.left = DATACOUNTER_EDIT_X2;
    rect.top = DATACOUNTER_EDIT_Y1;
    rect.right = DATACOUNTER_EDIT_X3;
    rect.bottom = DATACOUNTER_EDIT_Y1 + DATACOUNTER_EDIT_HIGHT - 1;
    GUILABEL_SetRect(MMI_DATACOUNTER_DATA_LABEL_ALL_TEXT, &rect, FALSE);
    
    // second line--Sent          
    rect.left = DATACOUNTER_EDIT_X1;
    rect.top = DATACOUNTER_EDIT_Y2;
    rect.right = DATACOUNTER_EDIT_X2 - 5;
    rect.bottom = DATACOUNTER_EDIT_Y2 + DATACOUNTER_EDIT_HIGHT - 1;
    GUILABEL_SetRect(MMI_DATACOUNTER_DATA_LABEL_SENT, &rect, FALSE);
    GUILABEL_SetFont(MMI_DATACOUNTER_DATA_LABEL_SENT, font_size, font_color);
    GUILABEL_SetFont(MMI_DATACOUNTER_DATA_LABEL_SENT_TEXT, font_size, font_color);
    if ( select_index == 2 && (alertActualValue >= alertValue) )
    {
        GUILABEL_SetFont(MMI_DATACOUNTER_DATA_LABEL_SENT, font_size, font_color_ex);
        GUILABEL_SetFont(MMI_DATACOUNTER_DATA_LABEL_SENT_TEXT, font_size, font_color_ex);
    }
    
    rect.left = DATACOUNTER_EDIT_X2;
    rect.top = DATACOUNTER_EDIT_Y2;
    rect.right = DATACOUNTER_EDIT_X3;
    rect.bottom = DATACOUNTER_EDIT_Y2 + DATACOUNTER_EDIT_HIGHT - 1;
    GUILABEL_SetRect(MMI_DATACOUNTER_DATA_LABEL_SENT_TEXT, &rect, FALSE);
    // 3rd line--Received
    rect.left = DATACOUNTER_EDIT_X1;
    rect.top = DATACOUNTER_EDIT_Y3;
    rect.right = DATACOUNTER_EDIT_X2 - 5;
    rect.bottom = DATACOUNTER_EDIT_Y3 + DATACOUNTER_EDIT_HIGHT - 1;
    GUILABEL_SetRect(MMI_DATACOUNTER_DATA_LABEL_RECEIVED, &rect, FALSE);
    GUILABEL_SetFont(MMI_DATACOUNTER_DATA_LABEL_RECEIVED, font_size, font_color);
    GUILABEL_SetFont(MMI_DATACOUNTER_DATA_LABEL_RECEIVED_TEXT, font_size, font_color);
    if ( select_index == 3 && (alertActualValue >= alertValue) )
    {
        GUILABEL_SetFont(MMI_DATACOUNTER_DATA_LABEL_RECEIVED, font_size, font_color_ex);
        GUILABEL_SetFont(MMI_DATACOUNTER_DATA_LABEL_RECEIVED_TEXT, font_size, font_color_ex);
    }
    
    rect.left = DATACOUNTER_EDIT_X2;
    rect.top = DATACOUNTER_EDIT_Y3;
    rect.right = DATACOUNTER_EDIT_X3;
    rect.bottom = DATACOUNTER_EDIT_Y3 + DATACOUNTER_EDIT_HIGHT - 1;
    GUILABEL_SetRect(MMI_DATACOUNTER_DATA_LABEL_RECEIVED_TEXT, &rect, FALSE);

}

LOCAL void MMIAPIDataCounter_readRecordDataNV(MMI_DATACOUNTER_RECORD_DATA_NV_E* record_data_nv)
{
    uint32 item_id = 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    if ( MMI_SmartDS_SIM_ID_1 == m_data_simId)
    {
        item_id = MMI_DATACOUNTER_SIM1_DATA_RECORD;
    }
    else if( MMI_SmartDS_SIM_ID_2 == m_data_simId )
    {
        item_id = MMI_DATACOUNTER_SIM2_DATA_RECORD;
    }

    MMINV_READ(item_id, record_data_nv, return_value);
    if ( return_value != MN_RETURN_SUCCESS )
    {
        SCI_MEMSET(record_data_nv, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
    }
    
}
LOCAL void DisplayDataCounterDataInfo(void)
{
    uint32 i = 0; 
    MMI_STRING_T        text            = {0};
    MMI_STRING_T        str            = {0};
    uint32 data_size = 0;
    wchar data_all_buff[15] = {0};
    wchar data_sent_buff[15] = {0};
    wchar data_recv_buff[15] = {0};
    
    MMI_DATACOUNTER_RECORD_DATA_NV_E record_data_nv = {0};

    MMIAPIDataCounter_readRecordDataNV(&record_data_nv);
    //All
    MMIAPICOM_WstrToNum(record_data_nv.buffer_ALL, MMIAPICOM_Wstrlen(record_data_nv.buffer_ALL), &data_size);
    str.wstr_ptr = data_all_buff;
    FormatDataFlowrate(&str, sizeof(data_all_buff)/sizeof(wchar), data_size);
    str.wstr_len = MMIAPICOM_Wstrlen(str.wstr_ptr);
    MMIRES_GetText(STR_DATA_ALERT_SETTINGS_ALL, MMK_GetFirstAppletHandle(), &text);
    GUILABEL_SetText(MMI_DATACOUNTER_DATA_LABEL_ALL, &text, FALSE);
    GUILABEL_SetText(MMI_DATACOUNTER_DATA_LABEL_ALL_TEXT, &str, FALSE);

    //Sent
    MMIAPICOM_WstrToNum(record_data_nv.buffer_send_outgoing, MMIAPICOM_Wstrlen(record_data_nv.buffer_send_outgoing), &data_size);
    str.wstr_ptr = data_sent_buff;
    FormatDataFlowrate(&str, sizeof(data_sent_buff)/sizeof(wchar), data_size);
    str.wstr_len = MMIAPICOM_Wstrlen(str.wstr_ptr);
    MMIRES_GetText(STR_DATA_ALERT_SETTINGS_SENT, MMK_GetFirstAppletHandle(), &text);
    GUILABEL_SetText(MMI_DATACOUNTER_DATA_LABEL_SENT, &text, FALSE);
    GUILABEL_SetText(MMI_DATACOUNTER_DATA_LABEL_SENT_TEXT, &str, FALSE);

    //Received
    MMIAPICOM_WstrToNum(record_data_nv.buffer_recv_incoming, MMIAPICOM_Wstrlen(record_data_nv.buffer_recv_incoming), &data_size);
    str.wstr_ptr = data_recv_buff;
    FormatDataFlowrate(&str, sizeof(data_recv_buff)/sizeof(wchar), data_size);
    str.wstr_len = MMIAPICOM_Wstrlen(str.wstr_ptr);
    MMIRES_GetText(STR_DATA_ALERT_SETTINGS_RECV, MMK_GetFirstAppletHandle(), &text);
    GUILABEL_SetText(MMI_DATACOUNTER_DATA_LABEL_RECEIVED, &text, FALSE);
    GUILABEL_SetText(MMI_DATACOUNTER_DATA_LABEL_RECEIVED_TEXT, &str, FALSE);
	 
}

LOCAL MMI_RESULT_E HandleDataCounterDataWinMsg(
									MMI_WIN_ID_T win_id, 
									MMI_MESSAGE_ID_E msg_id, 
									DPARAM param
									)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_MENU_ID_T menu_id = 0;
    MMI_STRING_T    statusBar_str = {0};
    readSetDataStorageDataType(&select_index);
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id, MMI_DATACOUNTER_SIM_DATA_ALERT_CTRL_ID);
            break;
            
        case MSG_FULL_PAINT:
            SetDataLabelRect();
            MMIDataCounter_DataItems();
            DisplayDataCounterDataInfo();
            MMI_GetLabelTextByLang(STR_SIM_MANAGER_DATA, &statusBar_str);
            if (MMIAPICOM_StatusAreaSetViewHeaderinfo(statusBar_str.wstr_ptr, statusBar_str.wstr_len)) 
            {
                GUIWIN_UpdateStb();
            }
            break;	
            
        case MSG_KEYDOWN_LEFT:
        case MSG_KEYUP_LEFT:
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            break;
            
        case MSG_KEYDOWN_RIGHT:
        case MSG_KEYUP_RIGHT:
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            break;
            
        case MSG_APP_OK:
            break;
            
        case MSG_CTL_MIDSK:
            MMK_CreateWin((uint32*)MMITEST_SET_DATA_WIN_TAB, PNULL);
            //MMIAPIDataCounter_dataAlertNotifaction(MN_DUAL_SYS_1, DATACOUNTER_SENT_DATATYPE);
            break;

        case MSG_CTL_OK:	
            MMK_CreateWin((uint32*)MMITEST_CLEAR_DATA_WIN_TAB,PNULL);
            break;
            
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_APP_RED:
	    MMK_CloseWin(win_id);
	    MMK_PostMsg(MMI_DATACOUNTER_SIM_RESULT_CTRL_ID, MSG_APP_RED, PNULL, 0);
	    break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}

//读取警介值
LOCAL uint32 ReadAlertValue(MN_DUAL_SYS_E dual_sys, uint8 dataType)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    MMI_STRING_T mbStr = {0};
    int strEqual = 0;
    uint32 dataAlertSize = 0;
    MMI_DATACOUNTER_DATA_SET_TYPE setType = {0};
    wchar text_buffer[10] = {0};

    MMI_GetLabelTextByLang(STR_ALERT_VALUE_INPUT_FORMAT, &mbStr);
    //读取警介值
    if ( dataType == DATACOUNTER_SENT_TYPE )
    {
        readSentData(&setType, dual_sys);
    }
    else if( dataType == DATACOUNTER_REV_TYPE)
    {
        readRevData(&setType, dual_sys);
    }
    else if( dataType == DATACOUNTER_ALL_TYPE )
    {
        readALLData(&setType, dual_sys);
    }
    MMIAPICOM_WstrToNum(setType.limit, MMIAPICOM_Wstrlen(setType.limit), &dataAlertSize);
    strEqual = MMIAPICOM_Wstrcmp(setType.limit, L"0");
    if ( setType.unit == 0 )
    {
        dataAlertSize *= (1024*1024);
    }
    else
    {
        dataAlertSize *= (1024*1024*1024);
    }

    if ( strEqual != 0  && (dataAlertSize & 0xffffffff) == 0 )
    {
        dataAlertSize = 0xffffffff;
    }

    return dataAlertSize;

}

//读取实时数据
LOCAL uint32 ReadActualValue(MN_DUAL_SYS_E dual_sys, uint8 dataType)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    MMI_DATACOUNTER_DATE_NV_E nv_date = {0};
    MMI_RECORD_INSERT_SIM_CARD_T recordInsertSimInfo[MMI_DUAL_SYS_MAX] = {0};
    MMI_DATACOUNTER_RECORD_DATA_NV_E record_data_nv = {0};
    uint16 nYear = 0;
    uint8 nMonth = 0;
    uint8 nDate = 0;
    uint8 monthDay = 0;
    BOOLEAN isPeroidSet = FALSE;
    uint32 dataActualSize = 0;
    
    ReadResetPeriodDate(&nv_date);
    
    if ( nv_date.date.year == 0 &&  nv_date.date.mon == 0 && nv_date.date.mday == 0  )
    {
        MMINV_READ(MMI_NV_INSERT_SIM_TIME, &recordInsertSimInfo, return_value);
        isPeroidSet = FALSE;
        nYear = recordInsertSimInfo[dual_sys].recordInsertSimTime.year;
        nMonth = recordInsertSimInfo[dual_sys].recordInsertSimTime.mon;
        nDate = recordInsertSimInfo[dual_sys].recordInsertSimTime.mday;
    }
    else
    {
        isPeroidSet = TRUE;
        nYear = nv_date.date.year;
        nMonth = nv_date.date.mon;
        nDate = nv_date.date.mday;
    }

    monthDay = MMICALENDAR_GetMonthday(nYear, nMonth);

    LoadDataNV(dual_sys, isPeroidSet, nDate, nYear, nMonth, monthDay, &record_data_nv);
    
    if ( dataType == DATACOUNTER_SENT_TYPE )
    {
        MMIAPICOM_WstrToNum(record_data_nv.buffer_send_outgoing, MMIAPICOM_Wstrlen(record_data_nv.buffer_send_outgoing), &dataActualSize);
    }
    else if( dataType == DATACOUNTER_REV_TYPE )
    {
        MMIAPICOM_WstrToNum(record_data_nv.buffer_recv_incoming, MMIAPICOM_Wstrlen(record_data_nv.buffer_recv_incoming), &dataActualSize);
    }
    else if( dataType == DATACOUNTER_ALL_TYPE )
    {
        MMIAPICOM_WstrToNum(record_data_nv.buffer_ALL, MMIAPICOM_Wstrlen(record_data_nv.buffer_ALL), &dataActualSize);
    }

    return dataActualSize;
}

PUBLIC void ResetDataShowJustOne(MN_DUAL_SYS_E dual_sys, uint8 dataType, BOOLEAN showjustOne)
{
    uint32 item_id = 0;
    DATACOUNTER_SHOW_WIN_JUST_ONE_E showJustone = {0};
    if ( MN_DUAL_SYS_1 == dual_sys )
    {
        item_id = MMI_DATACOUNTER_SIM1_DATA_SHOW_WIN_NV;
    }
    else if( MN_DUAL_SYS_2 == dual_sys )
    {
        item_id = MMI_DATACOUNTER_SIM2_DATA_SHOW_WIN_NV;
    }

    showJustone.dataType = dataType;
    showJustone.showJustOne = showjustOne;
    
    MMINV_WRITE(item_id, &showJustone);
}

LOCAL void ReadDataShowJustOne(MN_DUAL_SYS_E dual_sys, DATACOUNTER_SHOW_WIN_JUST_ONE_E* showJustone)
{
    uint32 item_id = 0;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    
    if ( MN_DUAL_SYS_1 == dual_sys )
    {
        item_id = MMI_DATACOUNTER_SIM1_DATA_SHOW_WIN_NV;
    }
    else if( MN_DUAL_SYS_2 == dual_sys )
    {
        item_id = MMI_DATACOUNTER_SIM2_DATA_SHOW_WIN_NV;
    }

    MMINV_READ(item_id, showJustone, return_value);
     if ( return_value != MN_RETURN_SUCCESS  )
    {
        showJustone->dataType = 0;
        showJustone->showJustOne = FALSE;
        MMINV_WRITE(item_id, showJustone);
    }
}

PUBLIC void MMIAPIDataCounter_dataAlertNotifaction(MN_DUAL_SYS_E dual_sys, DATACOUNTER_DATATYPE_E dataType)
{
    BOOLEAN bAlertNotifaction = TRUE;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    DATACOUNTER_SHOW_WIN_JUST_ONE_E showJustone = {0};
    SCI_DATE_T nv_date = {0};
    wchar buff_temp[20] = {0};
    wchar str_buff[20] = {0};
    MMI_STRING_T dataActual = {0};
    MMI_STRING_T text1 = {0};
    MMI_STRING_T text2 = {0};
    wchar buff_str[DATACOUNTER_TEXT_LEN+1] = {0};
    MMI_STRING_T data_type_str = {0};
    uint32 nvItem = 0;
    uint32 item_id = 0;
    uint8 alertIndex = 0;
    uint8 alertStatus = 0;
    BOOLEAN bAlertStatus = FALSE;
    uint32 dataActualSize = 0;
    uint32 dataAlertSize = 0;
    wchar text_temp[DATACOUNTER_TEXT_LEN+1] = {0};
    MMI_STRING_T text1_temp = {0};
    //wchar old_str[] = {0x005c, 0x006e, 0x0025, 0x0055, 0x0000};
    wchar old_str[] = {0x0025, 0x0055, 0x0000};
    wchar newstr[] = {0x000a, 0x0000};

    SCI_TRACE_LOW("MMIAPIDataCounter_dataAlertNotifaction dataType = %d", dataType);

     if ( MN_DUAL_SYS_1 == dual_sys )
    {
        item_id = MMI_DATACOUNTER_SIM1_DATA_RECORD;
    }
    else if( MN_DUAL_SYS_2 == dual_sys )
    {
        item_id = MMI_DATACOUNTER_SIM2_DATA_RECORD;
    }
    //如果不是同一张卡不做处理
    if ( FALSE == CheckSameSimCard(dual_sys, item_id) )
    {
        bAlertNotifaction = FALSE;
        SCI_TRACE_LOW("MMIAPIDataCounter_dataAlertNotifaction CheckSameSimCard %d", bAlertNotifaction);
    }

    if ( bAlertNotifaction )
    {
        //读取警介状态
        if ( MMI_SmartDS_SIM_ID_1 == dual_sys )
        {
            nvItem = MMI_DATACOUNTER_SIM1_DATA_SAVE_SELECT_ITEM;
        }
        else if( MMI_SmartDS_SIM_ID_2 == dual_sys )
        {
            nvItem = MMI_DATACOUNTER_SIM2_DATA_SAVE_SELECT_ITEM;
        }

        MMINV_READ(nvItem, &alertIndex, return_value);

        if ( return_value != MN_RETURN_SUCCESS  )
        {
               alertIndex = 0;
        }
        SCI_TRACE_LOW("MMIAPIDataCounter_dataAlertNotifaction bAlertStatus = %d", bAlertStatus);
        switch(alertIndex)
        {
            case 0:
                    bAlertStatus = FALSE;
                break;
            case 1:
                    bAlertStatus = TRUE;
                    alertStatus = 3;//all
                break;
            case 2:
                    bAlertStatus = TRUE;
                    alertStatus = 0;//sent
                break;
            case 3:
                    bAlertStatus = TRUE;
                    alertStatus = 1;//receive
                break;
            default:
                break;
        }
        
        dataActualSize = ReadActualValue(dual_sys, dataType);
        SCI_TRACE_LOW("MMIAPIDataCounter_dataAlertNotifaction bAlertStatus = %d, dataActualSize = %d", bAlertStatus, dataActualSize);
        if ( !bAlertStatus )//警介处于关闭状态，不提醒
        {
            bAlertNotifaction = FALSE;
        }
        else
        {
            ReadDataShowJustOne(dual_sys, &showJustone);
            SCI_TRACE_LOW("MMIAPIDataCounter_dataAlertNotifaction showJustone.dataType = %d", showJustone.dataType);
            SCI_TRACE_LOW("MMIAPIDataCounter_dataAlertNotifaction showJustone.showJustOne = %d", showJustone.showJustOne);
             //警介提示对话框如果弹过就不再给用户提示
            if ( showJustone.dataType == alertStatus && showJustone.showJustOne == TRUE )
            {
                bAlertNotifaction = FALSE;
            }

            if ( bAlertNotifaction )
            {
                //设置的警介状态与当前数据类型不匹配，不提醒
                if ( (alertStatus == 0 && dataType != alertStatus) || 
                    (alertStatus == 1 && dataType != alertStatus) )
                {
                    bAlertNotifaction = FALSE;
                }

                SCI_TRACE_LOW("MMIAPIDataCounter_dataAlertNotifaction bAlertNotifaction 111  = %d", bAlertNotifaction);
                if ( bAlertNotifaction )
               {
                    if ( alertStatus == 3 )
                    {
                        dataType = DATACOUNTER_ALL_DATATYPE;
                    }
                    dataActualSize = ReadActualValue(dual_sys, dataType);
                    dataAlertSize = ReadAlertValue(dual_sys, dataType);

                    SCI_TRACE_LOW("MMIAPIDataCounter_dataAlertNotifaction dataActualSize = %d", dataActualSize);
                    SCI_TRACE_LOW("MMIAPIDataCounter_dataAlertNotifaction dataAlertSize = %d", dataAlertSize);
                    if ( dataActualSize >= dataAlertSize )
                    {
                        MMI_GetLabelTextByLang(STR_COUN_LIMIT_REACHED_EXT01, &text1_temp);
                        MMIAPICOM_Wstrncpy(text_temp, text1_temp.wstr_ptr , text1_temp.wstr_len);
                        MMIAPICOM_WstrReplace(text_temp, sizeof(text_temp), old_str, newstr);
                        text1.wstr_ptr = text_temp;
                        text1.wstr_len = MMIAPICOM_Wstrlen(text1.wstr_ptr);

                        switch(alertStatus)
                        {
                            case 3:
                                MMI_GetLabelTextByLang(STR_COUN_LIMIT_ALL_DATA_EXT01, &data_type_str);
                                break;
                            case 0:
                                MMI_GetLabelTextByLang(STR_COUN_LIMIT_SENT_DATA_EXT01, &data_type_str);
                                break;
                                
                            case 1:
                                MMI_GetLabelTextByLang(STR_COUN_LIMIT_RECEIV_DATA_EXT01, &data_type_str);
                                break;
                            default:
                                break;
                        }

                        text2.wstr_ptr = buff_str;
                        text2.wstr_len = sizeof(buff_str)/sizeof(wchar);
                        
                        MMIAPICOM_Wstrncat(text2.wstr_ptr, data_type_str.wstr_ptr, data_type_str.wstr_len);
                        MMIAPICOM_Wstrncat(text2.wstr_ptr, L" ", MMIAPICOM_Wstrlen(L" "));

                        dataActual.wstr_ptr = str_buff;
                        dataActual.wstr_len = sizeof(str_buff)/sizeof(wchar);
                        FormatDataFlowrate(&dataActual, dataActual.wstr_len, dataActualSize);
                        MMIAPICOM_Wstrncat(text2.wstr_ptr, str_buff, MMIAPICOM_Wstrlen(str_buff));
                        
                        ResetDataShowJustOne(dual_sys, alertStatus, TRUE);
                        bAlertNotifaction = TRUE;
                    }
                    else
                    {
                        bAlertNotifaction = FALSE;
                    }
                }
            }

        }
    }

    SCI_TRACE_LOW("MMIAPIDataCounter_dataAlertNotifaction bAlertNotifaction 2222 = %d", bAlertNotifaction);
    if ( bAlertNotifaction )
    {
        m_data_simId = dual_sys;
        MMIAPIDataCounter_ThresholdTips(dual_sys, &text1, &text2, HandleOpenDataQuery);
    }
    if (MMK_IsOpenWin(MMIACC_DATA_COUNTER_WIN_DATA_ID) && (MMIACC_DATA_COUNTER_WIN_DATA_ID == MMK_GetFocusWinId()))
    {
        MMK_SendMsg(MMIACC_DATA_COUNTER_WIN_DATA_ID, MSG_FULL_PAINT, PNULL);
    }
    else if (MMK_IsOpenWin(MMIACC_DATA_COUNTER_WIN_RESULT_ID)
             && (MMIACC_DATA_COUNTER_WIN_RESULT_ID == MMK_GetFocusWinId()))
    {
        MMK_SendMsg(MMIACC_DATA_COUNTER_WIN_RESULT_ID, MSG_FULL_PAINT, PNULL);
    }
    
    return;
}

LOCAL MMI_RESULT_E HandleOpenDataQuery(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE; 
    DIALOG_TYPE dialog_type = DIALOG_WITH_ALARM;
    MMIPUB_DIALOG_SOFTKEY_T dialog_softkey = {0};
    switch (msg_id)
    {
       case MSG_OPEN_WINDOW:
            dialog_softkey.index = LEFT_BUTTON;
            dialog_softkey.type = DIALOG_SOFTKEY_TEXT_ID;
            dialog_softkey.content = PNULL;
            MMIPUB_SetDialogSoftkey(&win_id, dialog_type, &dialog_softkey);

            dialog_softkey.index = MIDDLE_BUTTON;
            dialog_softkey.type = DIALOG_SOFTKEY_TEXT_ID;
            dialog_softkey.content = STXT_SOFTKEY_OPEN_MK;
            MMIPUB_SetDialogSoftkey(&win_id, dialog_type, &dialog_softkey);

            dialog_softkey.index = RIGHT_BUTTON;
            dialog_softkey.type = DIALOG_SOFTKEY_TEXT_ID;
            dialog_softkey.content = STXT_SOFTKEY_STOP_RSK;
            MMIPUB_SetDialogSoftkey(&win_id, dialog_type, &dialog_softkey);
            recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);	
            break;
       case MSG_FULL_PAINT:
            MMIPUB_HandleWaitWinMsg(win_id, MSG_FULL_PAINT, PNULL);
            break;
	case MSG_GET_FOCUS:
        //recode = MMIPUB_HandleWaitWinMsg(win_id, MSG_FULL_PAINT, PNULL);
		 break;
	case MSG_CTL_OK:
	case MSG_APP_WEB:
	case MSG_NOTIFY_MIDSK:
               MMK_CreateWin((uint32*)DATA_COUNTER_WIN_DATA_TAB, PNULL);
	        MMK_CloseWin(win_id);
		break;
       case MSG_APP_CANCEL:
		if (win_id == MMK_GetFocusWinId())
		{
			MMK_CloseWin(win_id);
		}		
		break;
       default:
		recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : MMIAPIDATACOUNTER_OpenDataWin
//         
//	Global resource dependence : 
//   Author:        zhanglu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIDATACOUNTER_OpenDataWin(void* param)
{
    m_data_simId = *((MN_DUAL_SYS_E*)param);
    MMK_CreateWin((uint32*)DATA_COUNTER_WIN_DATA_TAB,(ADD_DATA)param);
    

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
