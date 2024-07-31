#ifdef MMIENG_PING_SUPPORT
#ifdef WIN32
        #include "std_header.h"
#endif
#include "mmi_default.h"
#include "mmidisplay_data.h"
#include "window_parse.h"
#include "mmi_menutable.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmicom_string.h"
#include "mmi_nv.h"
#include "mmipub.h"
#include "guirichtext.h"
#include "guiedit.h"
#include "guiim_base.h"
#include "mmipb_app.h"
#include "mmiconnection_text.h"
#include "mmiping_text.h"
#include "app_tcp_if.h"
#include "mmiping_internal.h"
#include "mmiconnection_export.h"
#include "mmiconnection_text.h"
#include "mmiconnection_image.h"
#include "mmiconnection_id.h"
#include "mmiping_id.h"
#include "mmieng_text.h"
#include "mmipub.h"
#include "mmieng_id.h"
#include "guires.h"
#include "guitext.h"

LOCAL MMIPING_SETTING_INFO_T s_ping_setting_info = {0};
LOCAL MN_DUAL_SYS_E s_ping_dual_sys = MN_DUAL_SYS_1;
LOCAL MMIPING_SELECT_MODE_E s_ping_select_mode = MMIPING_SELECT_NORMAL;

WINDOW_TABLE(MMIENG_SHOW_PACKET_INFO_WIN_TAB) =
{
	WIN_FUNC((uint32)HandleShowPacketInfoWinMsg),
	WIN_ID(MMIPING_SHOW_PACKET_WIN_ID),
	WIN_TITLE(TXT_ENG_PING_MODE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIPING_CTRL_ID_SHOW_PACKET_INFO),
    END_WIN
};

WINDOW_TABLE(MMIENG_PING_COUNT_WIN_TAB) =
{
    WIN_FUNC((uint32)HandlePingInputCntMsg),
    WIN_ID(MMIPING_PING_COUNT_WIN_ID),
    WIN_TITLE(TXT_ENG_PING_MODE_COUNT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_DIGITAL_CTRL(MMIPING_MAX_COUNT_LEN, MMIPING_CTRL_ID_COUNT),
    END_WIN
};

#ifdef IPVERSION_SUPPORT_V4_V6
WINDOW_TABLE(MMIENG_PING_IPTYPE_WIN_TAB) =
{
    WIN_FUNC((uint32)HandlePingSettingIpTypeMsg),
    WIN_ID(MMIPING_PING_IPTYPE_WIN_ID),
    WIN_TITLE(TXT_ENG_PING_MODE_IPTYPE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIPING_CTRL_ID_IPTYPE),
    END_WIN
};
#endif

WINDOW_TABLE(MMIENG_PING_WEBADDRESS_WIN_TAB) =
{
    WIN_FUNC((uint32)HandlePingInputWebAddressWinMsg),
    WIN_ID(MMIPING_PING_WEBADDRESS_WIN_ID),
    WIN_TITLE(TXT_ENG_PING_MODE_WEB_ADDRESS),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //≤Œ ˝Œ™≤‚ ‘”√
    CREATE_EDIT_TEXT_CTRL(MMIPING_WEBADDRESS_MAX_URL_LEN ,MMIPING_CTRL_ID_WEBADDRESS),
    END_WIN
};

WINDOW_TABLE(MMIENG_PING_MODE_NORMAL_WIN_TAB) = 
{
    WIN_FUNC((uint32)MMIENG_HandlePingModeSettingWinMsg),    
    WIN_ID(MMIPING_PING_MODE_WIN_ID),
    WIN_TITLE(TXT_ENG_PING_NORMAL),
    WIN_SOFTKEY(TXT_EDIT, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPING_CTRL_ID_SETTING_INFO),
    END_WIN
};

WINDOW_TABLE(MMIENG_PING_MODE_LONG_WIN_TAB) = 
{
    WIN_FUNC((uint32)MMIENG_HandlePingModeSettingWinMsg),    
    WIN_ID(MMIPING_PING_MODE_WIN_ID),
    WIN_TITLE(TXT_ENG_PING_LONG),
    WIN_SOFTKEY(TXT_EDIT, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPING_CTRL_ID_SETTING_INFO),
    END_WIN
};


WINDOW_TABLE( MMIENG_PING_SELECT_MODE_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandlePingSelectModeWindow),
    WIN_ID( MMIPING_PING_SELECT_MODE_WIN_ID ),
    WIN_TITLE(TXT_ENG_PING_SELECT_MODE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPING_CTRL_ID_SELECT_MODE_INFO),
    END_WIN
};
PUBLIC MMIPING_SELECT_MODE_E MMIAPIPING_GetSelectPingMode(void)
{
    SCI_TraceLow("[PING] MMIAPIPING_GetSelectPingMode s_ping_select_mode=%d", s_ping_select_mode);
    return s_ping_select_mode;
}

PUBLIC MMIPING_SETTING_INFO_T* MMIAPIPING_GetSettingInfoBySIMID(void)
{
    return &s_ping_setting_info;
}

PUBLIC BOOLEAN MMIAPIPING_IsPacketShowWinOpened(void)
{
    SCI_TraceLow("[PING] MMIAPIPING_IsPacketShowWinOpened ");
    return (MMK_IsOpenWin(MMIPING_SHOW_PACKET_WIN_ID));
}
LOCAL void ShowVersionInfo1(
                            BOOLEAN   need_update
                            )
{   
    MMI_STRING_T    text_s = {0};
    uint16          uint16_str[250] = {0};
    uint16          uint16_str_len = 0;
    uint16          temp_len = 0;
    RES_PING_STR*   ping_rsp_str = PNULL;

	ping_rsp_str = MMIPING_GetDisplayString();
	if (PNULL != ping_rsp_str)
	{
        temp_len = SCI_STRLEN((char*)ping_rsp_str);
	}
    SCI_TraceLow("[PING] ShowVersionInfo1 temp_len=%d",temp_len);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)ping_rsp_str, temp_len);
    {
       GUI_FONT_T font = MMI_DEFAULT_TEXT_FONT;
       GUI_COLOR_T font_color = MMI_BLACK_COLOR;
       GUITEXT_SetFont(MMIPING_CTRL_ID_SHOW_PACKET_INFO,&font,&font_color);
    }
    text_s.wstr_ptr = uint16_str;
    text_s.wstr_len = uint16_str_len;
    GUITEXT_SetString(MMIPING_CTRL_ID_SHOW_PACKET_INFO, text_s.wstr_ptr,text_s.wstr_len, need_update);

    return;
}

LOCAL MMI_RESULT_E HandleShowPacketInfoWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    uint8 timer_id = 0;
    SCI_TraceLow("[PING]:HandleShowPacketInfoWinMsg  MSG_ID=0x%x", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUITEXT_SetCircularHandle(FALSE, MMIPING_CTRL_ID_SHOW_PACKET_INFO);
        MMK_SetAtvCtrl(win_id, MMIPING_CTRL_ID_SHOW_PACKET_INFO);

        ShowVersionInfo1(FALSE); 
        GUIWIN_SetSoftkeyTextId(MMIPING_SHOW_PACKET_WIN_ID, TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_NULL, STXT_RETURN, FALSE);

        break;

    case MSG_FULL_PAINT:
        ShowVersionInfo1(FALSE); 
        break;

#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:
#endif
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
        if (MMIPING_SELECT_LONG_MODE == MMIAPIPING_GetSelectPingMode() && (FALSE == MMIAPIPING_GetIsManualQuit()))
        {
            MMIAPIPING_SetIsManualQuit(TRUE);
        }
        else
        {
            MMK_CloseWin(MMIPING_SHOW_PACKET_WIN_ID);
        }
        break;
        
    case MSG_KEYDOWN_RED:
        if (MMIPING_SELECT_LONG_MODE == MMIAPIPING_GetSelectPingMode())
        {
            MMIAPIPING_SetIsManualQuit(TRUE);
        }
        MMK_CloseWin(MMIPING_SHOW_PACKET_WIN_ID);
        break;

#ifdef FLIP_PHONE_SUPPORT
    case MSG_KEYDOWN_FLIP:     
        break;
#endif
    case MSG_CLOSE_WINDOW:
        timer_id = MMIAPIPING_GetLongPingTimerID();
        if (0 != timer_id)
        {
            MMK_StopTimer(timer_id);
            MMIAPIPING_SetLongPingTimerID(timer_id);
        }
        MMIAPIPDP_Deactive(MMI_MODULE_PING);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

#ifdef IPVERSION_SUPPORT_V4_V6
LOCAL MMI_RESULT_E HandlePingSettingIpTypeMsg(
                                              MMI_WIN_ID_T  	win_id,
                                              MMI_MESSAGE_ID_E	msg_id,
                                              DPARAM			param
                                              )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    uint16          list_index = 0;
    uint16          i = 0;
    MMI_CTRL_ID_T   ctrl_id = MMIPING_CTRL_ID_IPTYPE;
    CONNECTION_MMI_TEXT_ID_E    text_id[3] = {TXT_COMMON_IPV4, TXT_COMMON_IPV6, TXT_COMMON_IPV4V6};
    MMIPING_SETTING_INFO_T *setting_detail = (MMIPING_SETTING_INFO_T*)MMK_GetWinAddDataPtr(win_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, 3, FALSE);
        for (i = 0; i < 3; i++)
        {
#if !defined MMI_GUI_STYLE_TYPICAL
            MMIAPISET_AppendListItemByTextId(text_id[i], TXT_NULL, ctrl_id, GUIITEM_STYLE_RADIO_ONE_LINE_TEXT );
#else
            MMIAPISET_AppendListItemByTextId(text_id[i], TXT_NULL, ctrl_id, GUIITEM_STYLE_RADIO_ONE_LINE_TEXT );
#endif
        }
        switch (setting_detail->ip_type)
        {
            case 0:
                list_index = 0;
                break;
            case 1:
                list_index = 1;
                break;
            case 2:
                list_index = 2;
                break;
            default:
                break;
        }
        //end
        GUILIST_SetSelectedItem(ctrl_id, list_index, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, list_index);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        list_index = GUILIST_GetCurItemIndex(ctrl_id);
        switch (list_index)
        {
            case 0:
                setting_detail->ip_type = MMICONNECTION_IP_V4;
                break;
            case 1:
                setting_detail->ip_type = MMICONNECTION_IP_V6;
                break;
            case 2:
                setting_detail->ip_type = MMICONNECTION_IP_V4V6;
                break;
            default:
                break;
        }
        //end
        MMK_CloseWin(win_id);//∑µªÿ…œ“ªº∂
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);//∑µªÿ…œ“ªº∂
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}
#endif

LOCAL MMI_RESULT_E HandlePingInputCntMsg(
                                         MMI_WIN_ID_T	    win_id,
                                         MMI_MESSAGE_ID_E	msg_id,
                                         DPARAM				param
                                         )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    uint32          port_value = 0;
    uint8           port_str[MMIPING_MAX_COUNT_LEN + 2] = {0};
    wchar           port_str2[MMIPING_MAX_COUNT_LEN + 2] = {0};
    uint8			port_len = 0;
    MMI_STRING_T string_info= {0};
    MMIPING_SETTING_INFO_T *setting_detail = (MMIPING_SETTING_INFO_T*)MMK_GetWinUserData(win_id);
    MMI_CTRL_ID_T ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID);
    GUIIM_TYPE_T         input_mode = GUIIM_TYPE_DIGITAL;
    wchar                dot_str[] = {'.'};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
    	GUIEDIT_SetTextMaxLen(ctrl_id, MMIPING_MAX_COUNT_LEN/2, MMIPING_MAX_COUNT_LEN);
        GUIEDIT_ConfigImCustomKey(ctrl_id, input_mode, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
        GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_DOT_NUM);
        MMIPUB_SetDialogBgLayerAlpha(win_id, 127);

        _snprintf((char*)port_str, (MMIPING_MAX_COUNT_LEN + 1),"%d", setting_detail->count);
        port_len=strlen((char*)port_str);
        MMIAPICOM_StrToWstr((uint8 *)port_str, port_str2);
        GUIEDIT_SetString(ctrl_id,port_str2,port_len);
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUIEDIT_GetString(ctrl_id,&string_info);
        if(0 == string_info.wstr_len)
        {
            _snprintf((char*)port_str, (MMIPING_MAX_COUNT_LEN + 1),"%d", setting_detail->count);
            port_len=strlen((char*)port_str);
            MMIAPICOM_StrToWstr((uint8 *)port_str, port_str2);
            GUIEDIT_SetString(ctrl_id,port_str2,port_len);
            //MMIPUB_OpenAlertWarningWin(TXT_CONNECTION_PORT_EMPTY);
            MMIPUB_OpenConfirmationDialogByTextId(PNULL,TXT_ENG_PING_MODE_EMPTY_COUNT,PNULL,
                                                  IMAGE_NULL,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,PNULL);
            break;
        }
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8 *)port_str2);//÷ª‘ –Ì ‰»Î ˝◊÷£¨ø…“‘”√’‚∏ˆ∫Ø ˝
        port_value = atoi((char*)port_str2);
        if (MMIPING_MAX_PORT_VALUE < port_value)
        {
            _snprintf((char*)port_str, (MMIPING_MAX_COUNT_LEN + 1),"%d", setting_detail->count);
            port_len=strlen((char*)port_str);
            MMIAPICOM_StrToWstr((uint8 *)port_str, port_str2);
            GUIEDIT_SetString(ctrl_id,port_str2,port_len);
            MMIPUB_OpenConfirmationDialogByTextId(PNULL,TXT_ENG_PING_MODE_BIG_COUNT,PNULL,
                                                  IMAGE_NULL,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,PNULL);
        }
        else
        {
            setting_detail->count = (uint16)port_value;
            MMK_CloseWin(win_id);
        }

        break;
    case MSG_FULL_PAINT:
        GUIWIN_SeSoftkeytButtonTextId(win_id,STR_LIST_DELETE,2,TRUE);
        GUIWIN_SetSoftkeyBtnState(win_id,1,FALSE,FALSE);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMK_CloseWin(win_id);//∑µªÿ…œ“ªº∂
        break;

    default:

        recode = MMIPUB_HandleDialogInputFieldWinMsg(win_id,msg_id,param);
        GUIEDIT_SetIm(ctrl_id,input_mode,input_mode);
        break;
    }
    return recode;
}

LOCAL MMI_RESULT_E HandlePingInputWebAddressWinMsg(
                                                   MMI_WIN_ID_T	    win_id,
                                                   MMI_MESSAGE_ID_E	msg_id,
                                                   DPARAM	        param
                                                   )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMI_STRING_T string_info = {0};
    MMIPING_SETTING_INFO_T *setting_detail =(MMIPING_SETTING_INFO_T *)MMK_GetWinUserData(win_id);
    MMI_CTRL_ID_T ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID);
    MMI_STRING_T  inputstr = {0};
    wchar tempbuf[MMIPING_WEBADDRESS_MAX_URL_LEN + 1]= {0};
    uint16 tempbuf_len = 0;
    GUIIM_TYPE_T allow_im = GUIIM_TYPE_NONE;
    GUIIM_TYPE_T init_im = GUIIM_TYPE_NONE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set im
        allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;/*lint !e655*/
        init_im = GUIIM_TYPE_ABC;
        GUIEDIT_SetIm(ctrl_id,allow_im,init_im);
        GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_URL);
        MMIAPICOM_StrToWstr((uint8 *)setting_detail->homepage, tempbuf);
        GUIEDIT_SetString(ctrl_id,tempbuf,setting_detail->homepage_len);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUIEDIT_GetString(ctrl_id,&string_info);
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8 *)tempbuf);//÷ª‘ –Ì ‰»ÎASCII£¨ø…“‘”√’‚∏ˆ∫Ø ˝
        SCI_MEMSET(setting_detail->homepage,0,sizeof(setting_detail->homepage));
        tempbuf_len = MIN(strlen((char*)tempbuf),MMIPING_WEBADDRESS_MAX_URL_LEN);
        SCI_MEMCPY(setting_detail->homepage,tempbuf,tempbuf_len);
        setting_detail->homepage_len=strlen((char *)tempbuf);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMK_CloseWin(win_id);//∑µªÿ…œ“ªº∂
        break;
        
    case MSG_FULL_PAINT:
        GUIWIN_SetSoftkeyBtnState(win_id,1,FALSE,FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id,STR_LIST_DELETE,2,TRUE);
        break;

    default:
        recode = MMIPUB_HandleDialogInputFieldWinMsg(win_id,msg_id,param);
        break;
    }
    
    return recode;
}
												   
LOCAL void AppendPdaConnectionItem2Line(
								MMI_CTRL_ID_T  ctrl_id,    //ÔøΩÿºÔøΩID
								MMI_TEXT_ID_T  text_1,	   //ÔøΩÔøΩÔøΩƒ±ÔøΩ
								MMI_STRING_T  text_str, 	//ÔøΩÔøΩÔøΩƒ±ÔøΩ
								MMI_IMAGE_ID_T image_id,   //ÕºÔøΩÔøΩID
								uint32		   user_data   //ÔøΩ√ªÔøΩÔøΩÔøΩÔøΩÔøΩ
								)
{
   GUILIST_ITEM_T	   item_t = {0};
   GUILIST_ITEM_DATA_T item_data = {0};
   wchar temp_wstr[MMIPING_STRNUM_MAX_LEN + 2] = {0};
   wchar temp_len = 0;
   wchar *temp_wstr_ptr = PNULL;
   wchar *malloc_wstr_ptr = PNULL;

   if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
   {
	   temp_wstr_ptr = temp_wstr;
	   temp_len = text_str.wstr_len;
	   /*ÔøΩÔøΩÔøΩÔøΩ÷¥ÔøΩÃ´ÔøΩÔøΩÔøΩÔøΩÃ¨ÔøΩÔøΩÔøΩÔøΩ*/
	   if(text_str.wstr_len > MMIPING_STRNUM_MAX_LEN)
	   {
		   malloc_wstr_ptr = SCI_ALLOC_APPZ(text_str.wstr_len*sizeof(wchar));
		   if(PNULL != malloc_wstr_ptr)
		   {
			   temp_wstr_ptr = malloc_wstr_ptr;
		   }
		   else
		   {
			   /*ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÎ≤ªÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ πÔøΩÔøΩƒ¨ÔøΩœµƒ£ÔøΩ»°min*/
			   temp_len= MIN(text_str.wstr_len, MMIPING_STRNUM_MAX_LEN);
		   }
	   }

	   SCI_MEMSET(temp_wstr_ptr, 0x00, (temp_len*sizeof(wchar)));

	   MMIAPICOM_Wstrncpy(temp_wstr_ptr,
		   text_str.wstr_ptr,
		   temp_len);
	   temp_len = text_str.wstr_len;
	   item_t.item_style	= GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
   }
   else
   {
	   item_t.item_style	= GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
   }

   item_t.item_data_ptr = &item_data;
   item_t.user_data 	= user_data;

   //text 1
   item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
   item_data.item_content[0].item_data.text_id = text_1;

   //text 2
   if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
   {
	   item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
	   item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr_ptr;
	   item_data.item_content[2].item_data.text_buffer.wstr_len = temp_len;
   }

   //anim
   item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
   item_data.item_content[1].item_data.image_id = image_id;

   GUILIST_AppendItem(ctrl_id, &item_t);
   if(PNULL != malloc_wstr_ptr)
   {
	   SCI_FREE(malloc_wstr_ptr);
	   malloc_wstr_ptr = PNULL;
   }
}

LOCAL void InitPingSettingLinkListCtrl(MMIPING_SETTING_INFO_T* setting_detail)
{
    MMI_CTRL_ID_T ctrl_id = MMIPING_CTRL_ID_SETTING_INFO;
    uint8 temp_str[MMIPING_STRNUM_MAX_LEN + 2] = {0};
    wchar temp_wstr[MMIPING_STRNUM_MAX_LEN + 2] = {0};
    MMI_STRING_T text_str= {0};
    MMI_TEXT_ID_T text_id = TXT_NULL;
    GUILIST_POS_INFO_T pos_info = {0};
    
    if (PNULL == setting_detail)
    {
        return;
    }
    
    GUILIST_GetCurPosInfo(ctrl_id, &pos_info);
    GUILIST_RemoveAllItems(ctrl_id);
    GUILIST_SetMaxItem(ctrl_id, 5, FALSE);

    SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
    text_str.wstr_len = MIN(setting_detail->homepage_len, MMIPING_STRNUM_MAX_LEN);
    MMI_STRNTOWSTR(temp_wstr, MMIPING_STRNUM_MAX_LEN, setting_detail->homepage, text_str.wstr_len, text_str.wstr_len);
    text_str.wstr_ptr = temp_wstr;
    text_str.wstr_len = MMIAPICOM_Wstrlen(temp_wstr) ;
    AppendPdaConnectionItem2Line(
        ctrl_id,
        TXT_ENG_PING_MODE_WEB_ADDRESS,
        text_str,
        IMAGE_NULL,
        (uint32)MMIPING_MENU_ID_WEB_ADDRESS
        );
    
#ifdef IPVERSION_SUPPORT_V4_V6
    if (MMICONNECTION_IP_V6 == (uint16)setting_detail->ip_type)
    {
        text_id = TXT_COMMON_IPV6;
    }
    else if (MMICONNECTION_IP_V4 == (uint16)setting_detail->ip_type)
    {
        text_id = TXT_COMMON_IPV4;
    }
    else
    {
        text_id = TXT_COMMON_IPV4V6;
    }
    MMI_GetLabelTextByLang(text_id, &text_str);
    AppendPdaConnectionItem2Line(
        ctrl_id,
        TXT_ENG_PING_MODE_IPTYPE,
        text_str,
        IMAGE_NULL,
        (uint32)MMIPING_MENU_ID_IP_TYPE
        );
#endif
    if (MMIPING_SELECT_LONG_MODE != s_ping_select_mode)
    {
        SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
        sprintf((char *)temp_str, "%d", setting_detail->count);
        text_str.wstr_len = SCI_STRLEN((char*)temp_str);
        text_str.wstr_len = MIN(text_str.wstr_len, MMIPING_STRNUM_MAX_LEN);
        MMI_STRNTOWSTR(temp_wstr, MMIPING_STRNUM_MAX_LEN, temp_str, text_str.wstr_len, text_str.wstr_len);
        text_str.wstr_ptr = temp_wstr;
        text_str.wstr_len = MMIAPICOM_Wstrlen(temp_wstr) ;
        AppendPdaConnectionItem2Line(
            ctrl_id,
            TXT_ENG_PING_MODE_COUNT,
            text_str,
            IMAGE_NULL,
            (uint32)MMIPING_MENU_ID_COUNT
            );
    }

    return;
}
PUBLIC void MMIAPIPING_UpdatePacketShowWin(void)
{
    if (MMK_IsOpenWin(MMIPING_SHOW_PACKET_WIN_ID))
    {
		MMK_SendMsg(MMIPING_SHOW_PACKET_WIN_ID, MSG_FULL_PAINT, PNULL);
    }

    return;
}
PUBLIC BOOLEAN MMIAPIENG_CreateShowPacketInfoWin(void)
{
    MMI_HANDLE_T handle = PNULL;

    handle = MMK_CreateWin((uint32*)MMIENG_SHOW_PACKET_INFO_WIN_TAB, PNULL);

    if (PNULL == handle)
    {
        return FALSE;
    }

    return TRUE;
}

LOCAL MMI_RESULT_E MMIENG_HandlePingModeSettingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    BOOLEAN return_val = FALSE;
    static MMIPING_SETTING_INFO_T *temp_setting_detail = PNULL;
    MMI_WIN_ID_T linkMenu_dialog_id = MMIPING_CTRL_ID_SETTING_INFO;
    MN_DUAL_SYS_E cur_dual_sys = s_ping_dual_sys;
    
    SCI_TraceLow("[PING] MMIENG_HandlePingModeSettingWinMsg cur_dual_sys=%d", cur_dual_sys); 
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(MMIPING_PING_MODE_WIN_ID, MMIPING_CTRL_ID_SETTING_INFO);
        temp_setting_detail = (MMIPING_SETTING_INFO_T*)SCI_ALLOCA(sizeof(MMIPING_SETTING_INFO_T));
        if (temp_setting_detail == PNULL)
        {
            MMIPUB_OpenAlertFailWin(TXT_COMMON_NO_MEMORY);
            MMK_CloseWin(win_id);
            break;
        }
        else
        {
            SCI_MEMSET(temp_setting_detail, 0, sizeof(MMIPING_SETTING_INFO_T));
            SCI_MEMCPY(temp_setting_detail, &s_ping_setting_info, sizeof(MMIPING_SETTING_INFO_T));
        }

		GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, TXT_EDIT, STXT_RETURN, FALSE);
        InitPingSettingLinkListCtrl(temp_setting_detail);
        //InitPdaSettingLinkListCtrl();
        break;
    case MSG_GET_FOCUS:
        InitPingSettingLinkListCtrl(temp_setting_detail);
        break;
                
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
#if defined MMI_GPRS_SUPPORT
            uint16 index = 0;
            uint32 user_data = 0;
            MMI_STRING_T temp_str = {0};
            index = GUILIST_GetCurItemIndex(MMIPING_CTRL_ID_SETTING_INFO);
            GUILIST_GetItemData(MMIPING_CTRL_ID_SETTING_INFO, index, &user_data);

            switch ((MMIPING_MENU_ID_E)user_data)//≤Àµ•Ω⁄µ„ID
            {
#ifdef IPVERSION_SUPPORT_V4_V6
            case MMIPING_MENU_ID_IP_TYPE:
                MMK_CreateWin((uint32 *)MMIENG_PING_IPTYPE_WIN_TAB,(ADD_DATA)temp_setting_detail);
                break;
#endif

            case MMIPING_MENU_ID_COUNT:
                linkMenu_dialog_id = MMIPING_PING_COUNT_WIN_ID;
                MMI_GetLabelTextByLang(TXT_ENG_PING_MODE_COUNT, &temp_str);
                MMIPUB_OpenDialogInputFieldWinByTextPtr(&temp_str,&linkMenu_dialog_id,PNULL,MMIPUB_SOFTKEY_CUSTOMER, HandlePingInputCntMsg);
                MMK_SetWinUserData (linkMenu_dialog_id, (void *)temp_setting_detail);
                break;

            case MMIPING_MENU_ID_WEB_ADDRESS:
                linkMenu_dialog_id = MMIPING_PING_WEBADDRESS_WIN_ID;
                MMI_GetLabelTextByLang(TXT_ENG_PING_MODE_WEB_ADDRESS, &temp_str);
                MMIPUB_OpenDialogInputFieldWinByTextPtrLen(&temp_str,&linkMenu_dialog_id, PNULL, 
                                                           MMIPUB_SOFTKEY_CUSTOMER, MMIPING_WEBADDRESS_MAX_URL_LEN, HandlePingInputWebAddressWinMsg);
                MMK_SetWinUserData (linkMenu_dialog_id, (void *)temp_setting_detail);
                break;

            default:
                break;
            }
#endif
        }
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:

        if (0 != memcmp(&s_ping_setting_info, temp_setting_detail, sizeof(MMIPING_SETTING_INFO_T)))
        {
            SCI_MEMSET(&s_ping_setting_info, 0, sizeof(MMIPING_SETTING_INFO_T));
            SCI_MEMCPY(&s_ping_setting_info, temp_setting_detail, sizeof(MMIPING_SETTING_INFO_T));
        }
        
        if (((MMIPING_SELECT_NORMAL == s_ping_select_mode) && (0 == s_ping_setting_info.count)) 
             || (0 == s_ping_setting_info.homepage_len))
        {
            MMIPUB_OpenAlertFailWin(TXT_ENG_PING_MODE_INVALID_INFO);
        }
        else
        {
            return_val = MMIAPIPING_ActivePDP(cur_dual_sys);
        }
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;    
}
LOCAL MMI_RESULT_E HandlePingSelectModeWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16                     cur_item = 0;
    MMI_RESULT_E                recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMIPING_CTRL_ID_SELECT_MODE_INFO;
    GUILIST_ITEM_T               item_t = {0};
    GUILIST_ITEM_DATA_T       item_data = {0};
    MMI_STRING_T               temp_str = {0};
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id,ctrl_id);
            GUILIST_SetMaxItem(ctrl_id, 2, FALSE);
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
            item_t.item_data_ptr = &item_data;
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;            
            item_data.item_content[0].item_data.text_id = TXT_ENG_PING_NORMAL;
            GUILIST_AppendItem( ctrl_id, &item_t );  

            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
            item_t.item_data_ptr = &item_data;
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;            
            item_data.item_content[0].item_data.text_id = TXT_ENG_PING_LONG;
            GUILIST_AppendItem( ctrl_id, &item_t );

            GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, cur_item);
            break;
            
        case MSG_FULL_PAINT:
            MMI_GetLabelTextByLang(TXT_ENG_PING_SELECT_MODE, &temp_str);
            if (MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len))
            {
                GUIWIN_UpdateStb();
            }
            break;
            
        case MSG_CTL_MIDSK:
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            switch (cur_item)
            {
                case 0:
                    s_ping_select_mode = MMIPING_SELECT_NORMAL;
                    MMK_CreateWin((uint32*)MMIENG_PING_MODE_NORMAL_WIN_TAB, PNULL);
                    break;
                    
                case 1:
                    s_ping_select_mode = MMIPING_SELECT_LONG_MODE;
                    MMK_CreateWin((uint32*)MMIENG_PING_MODE_LONG_WIN_TAB, PNULL);
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

PUBLIC BOOLEAN MMIAPIENG_OpenPingSettingWin(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    if (TRUE == isReturnOK)
    {
        s_ping_dual_sys = dual_sys;
        SCI_TraceLow("[PING] MMIAPIENG_OpenPingSettingWin dual_sys=%d",dual_sys); 
        MMK_CreateWin((uint32 *)MMIENG_PING_SELECT_MODE_WIN_TAB, PNULL);
        //MMK_CreateWin((uint32 *)MMIENG_PING_MODE_WIN_TAB, PNULL);
    }

    return result;
}
PUBLIC void MMIAPIENG_OpenPingSelectWin(void)
{
#ifndef MMI_DUALMODE_ENABLE
    uint16  sim_ok = MN_DUAL_SYS_1;
    uint32  sim_num = 0;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok, 1);
    if (0 == sim_num)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (1 == sim_num)
    {   
        MMIAPIENG_OpenPingSettingWin(sim_ok, TRUE, PNULL);
    }
    else
    {
        MMI_WIN_ID_T    selectSimWinID = MMIENG_SIM_SELECT_WIN_ID;
        MMIPHONE_SELECT_SIM_DATA_T  caller_data = {0};
        
        caller_data.append_type = (uint32)MMISET_APPEND_SYS_OK;
        caller_data.callback_func = MMIAPIENG_OpenPingSettingWin;
        caller_data.select_type= MMIPHONE_SELECTION_SIM_ONLY;
        caller_data.publist_id = MMIENG_LISTBOX_CTRL_ID;
        caller_data.win_id_ptr = &selectSimWinID;
        caller_data.user_data  = PNULL;
        caller_data.extra_data = PNULL;
    
        MMIAPIPHONE_OpenSelectSimWin(&caller_data);
        return;
    }
#else
     MMIAPIENG_OpenPingSettingWin(MN_DUAL_SYS_1);
     
     return;     
#endif
}
#endif
