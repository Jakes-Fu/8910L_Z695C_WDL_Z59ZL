/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004      maryxiao       Create
******************************************************************************/

#define _MMISET_NOTIFICATIONS_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmiphone_export.h"
#include "mmiset_id.h"
//#include "mmiset_menutable.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiset_menutable.h"
#include "mmipub.h"
#include "mmiset_text.h"
#include "mmi_appmsg.h"
#include "mmiset_notifications.h"
#include "guires.h"
#include "mmiset_nv.h"
#include "mmiset_wintab.h"
#include "mmiudisk_export.h"
#include "guiownerdraw.h"
#include "mmiset_image.h"
#include "mmiset_text.h"
#include "guisetlist.h"
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmi_livewallpaper_text.h"
#endif
#include "mmk_tp.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmiset_func.h"
#include "mmi_resource.h"
#include "mmiset_export.h"
#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_text.h"
#include "mmiqbtheme_export.h"
#endif
#ifdef UI_P3D_SUPPORT
#include "mmi_winse.h"
#endif
#include "mmiset_internal.h"
#include "mmiset_lock_effect_internal.h"

//#ifdef PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM
#include "mmiset_mainmenu_export.h"
//#endif

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#include "mmiset_lock_effect_export.h"
#endif
#ifdef MMI_DEVICELOCK_SUPPORT
#include "mmi_devicelock_export.h"
#endif
/*----------------------------------------------------------------------------*/
/*                         Data/Variables                                     */
/*----------------------------------------------------------------------------*/

//noticifcations setting
enum {
    ID_NOTIFICATIONS_LIST_MESSAGES,
    ID_NOTIFICATIONS_LIST_CALLS,
    ID_NOTIFICATIONS_LIST_ALARMS,
    ID_NOTIFICATIONS_LIST_CALENDAR,
    ID_NOTIFICATIONS_LIST_VOICE_MAIL,
    ID_NOTIFICATIONS_LIST_OUTLOOK,
    ID_NOTIFICATIONS_LIST_OPERATOR,
    ID_NOTIFICATIONS_LIST_FM_RADIO,
    ID_NOTIFICATIONS_LIST_MUSIC
};
enum {
	ID_NOTIFICATIONS_TYPE = 0,
    ID_NOTIFICATIONS_MESSAGES,
    ID_NOTIFICATIONS_CALLS,
    ID_NOTIFICATIONS_VOICE_MAIL,
    ID_NOTIFICATIONS_OTHERS,
};
enum {
	ID_NOTIFICATIONS_TYPE_DETAILS = 0,
    ID_NOTIFICATIONS_TYPE_ICON_ONLY,
};

/**--------------------------------------------------------------------------*
**                function declaration                                       *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetNotificaitonsWindow(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);
/*****************************************************************************/
//  Description :choose notification type
//  Global resource dependence :
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNotificationTypeWindow(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
// the window for select font
WINDOW_TABLE( MMISET_SET_NOTIFICATIONS_WIN_TAB ) = {
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetNotificaitonsWindow ),
    WIN_ID( MMISET_SET_NOTIFICATIONS_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_NOTIFICATIONS_CTRL_ID),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),

    END_WIN
};
WINDOW_TABLE( MMISET_SET_NOTIFICATIONS_TYPE_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleNotificationTypeWindow ),
    WIN_ID( MMISET_SET_NOTIFICATIONS_TYPE_WIN_ID ),
    WIN_STYLE( WS_HAS_TRANSPARENT|WS_DISABLE_COMMON_BG),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_NOTIFICATION_TYPE_ITEMS,MMISET_SET_NOTIFICATION_TYPE_MENU_CTRL_ID),
    END_WIN
};
/*****************************************************************************/
//  Description : open phone setting shortcut window
//  Global resource dependence :
//  Author: applezhang
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_EnterSettingNotificationsWin(void) {
    MMK_CreateWin((uint32 *)MMISET_SET_NOTIFICATIONS_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :get Notification note nv
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/

PUBLIC BOOLEAN MMISet_GetNotification(MMISET_NOTIFICATION_TYPE_E n_type)
{
    MN_RETURN_RESULT_E          return_value    =   MN_RETURN_FAILURE;
    BOOLEAN                     is_on          =   FALSE;
    SET_NV_ITEM_E               nv_item   =  MMINV_SET_NOTIFICATION_MESSAGES;
	switch(n_type){
	case MMISET_NOTIFICATION_MESSAGES:
		nv_item = MMINV_SET_NOTIFICATION_MESSAGES;

		if(TRUE == MMIAPISET_IsDisableShareViaOption())
		{
			MMINV_READ(nv_item, &is_on, return_value);
			is_on = FALSE;
			if (MN_RETURN_SUCCESS != return_value)
			{
				MMINV_WRITE(nv_item, &is_on);
			}
			return is_on;
		}
		break;
	case MMISET_NOTIFICATION_CALLS:
		nv_item = MMINV_SET_NOTIFICATION_CALLS;
		break;
	case MMISET_NOTIFICATION_MAILS:
		nv_item = MMINV_SET_NOTIFICATION_MAILS;
		break;
	case MMISET_NOTIFICATION_ALARMS:
		nv_item = MMINV_SET_NOTIFICATION_ALARMS;
		break;
	case MMISET_NOTIFICATION_CALENDAR:
		nv_item = MMINV_SET_NOTIFICATION_CALENDAR;
		break;
	case MMISET_NOTIFICATION_OUTLOOK:
		nv_item = MMINV_SET_NOTIFICATION_OUTLOOK;
		break;
	case MMISET_NOTIFICATION_OPERATOR:
		nv_item = MMINV_SET_NOTIFICATION_OPERATOR;
		break;
	case MMISET_NOTIFICATION_RADIO:
		nv_item = MMINV_SET_NOTIFICATION_RADIO;
		break;
	case MMISET_NOTIFICATION_MUSIC:
		nv_item = MMINV_SET_NOTIFICATION_MUSIC;
		break;
	case MMISET_NOTIFICATION_OTHERS:
	case MMISET_NOTIFICATION_MAX:
	default:
		nv_item = MMINV_SET_NOTIFICATION_OTHERS;//other apps
		//not found
		break;
	}
    MMINV_READ(nv_item, &is_on, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_on= TRUE;
        MMINV_WRITE(nv_item, &is_on);
    }
    return is_on;
}
/*****************************************************************************/
//  Description :set Notification note nv
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/

PUBLIC void MMISet_SetNotification(MMISET_NOTIFICATION_TYPE_E n_type,BOOLEAN is_on)
{
    MN_RETURN_RESULT_E          return_value    =   MN_RETURN_FAILURE;
    BOOLEAN                     is_switch          =   FALSE;
    SET_NV_ITEM_E               nv_item   =  MMINV_SET_NOTIFICATION_MESSAGES;
	switch(n_type){
	case MMISET_NOTIFICATION_MESSAGES:
		nv_item = MMINV_SET_NOTIFICATION_MESSAGES;
		break;
	case MMISET_NOTIFICATION_CALLS:
		nv_item = MMINV_SET_NOTIFICATION_CALLS;
		break;
	case MMISET_NOTIFICATION_MAILS:
		nv_item = MMINV_SET_NOTIFICATION_MAILS;
		break;
	case MMISET_NOTIFICATION_ALARMS:
		nv_item = MMINV_SET_NOTIFICATION_ALARMS;
		break;
	case MMISET_NOTIFICATION_CALENDAR:
		nv_item = MMINV_SET_NOTIFICATION_CALENDAR;
		break;
	case MMISET_NOTIFICATION_OUTLOOK:
		nv_item = MMINV_SET_NOTIFICATION_OUTLOOK;
		break;
	case MMISET_NOTIFICATION_OPERATOR:
		nv_item = MMINV_SET_NOTIFICATION_OPERATOR;
		break;
	case MMISET_NOTIFICATION_RADIO:
		nv_item = MMINV_SET_NOTIFICATION_RADIO;
		break;
	case MMISET_NOTIFICATION_MUSIC:
		nv_item = MMINV_SET_NOTIFICATION_MUSIC;
		break;
	case MMISET_NOTIFICATION_OTHERS:
	case MMISET_NOTIFICATION_MAX:
	default:
		nv_item = MMINV_SET_NOTIFICATION_OTHERS;//other apps
		//not found
		break;
	}
    MMINV_WRITE(nv_item, &is_on);
#ifdef MMI_DEVICELOCK_SUPPORT
    MMIAPIDEVICELOCK_EnableDisplayNotification(n_type,is_on);
#endif
}

/*****************************************************************************/
//  Description :get Notification note nv
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/

PUBLIC uint8 MMISet_GetNotificationShowType(void)
{
    MN_RETURN_RESULT_E          return_value    	=   MN_RETURN_FAILURE;
    uint8		                n_showtype          =   ID_NOTIFICATIONS_TYPE_DETAILS;
    SET_NV_ITEM_E               nv_item  			=  MMINV_SET_NOTIFICATION_TYPE;
    MMINV_READ(nv_item, &n_showtype, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        n_showtype = ID_NOTIFICATIONS_TYPE_DETAILS;
        MMINV_WRITE(nv_item, &n_showtype);
    }
    return n_showtype;
}
/*****************************************************************************/
//  Description :set Notification note nv
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/

PUBLIC void MMISet_SetNotificationShowType(uint8 n_showtype)
{
    MN_RETURN_RESULT_E          return_value    =   MN_RETURN_FAILURE;
    SET_NV_ITEM_E               nv_item   		=  MMINV_SET_NOTIFICATION_TYPE;
//	if(n_showtype > ID_NOTIFICATIONS_TYPE_ICON_ONLY || n_showtype < ID_NOTIFICATIONS_TYPE_DETAILS)
    if(n_showtype > ID_NOTIFICATIONS_TYPE_ICON_ONLY)
    {
        n_showtype = ID_NOTIFICATIONS_TYPE_DETAILS;
    }
    MMINV_WRITE(nv_item, &n_showtype);
}

/*****************************************************************************/
//  Description :HandleSetNotificaitonsWindow
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleSetNotificaitonsWindow(
    MMI_WIN_ID_T    win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
) {
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;

    MMI_TEXT_ID_T               str_id[] = {STR_ILIST_MESSAGE_EXT01,STR_ILIST_CALL_EXT01,
                                            STR_NOTIF_VOICEMAIL_EXT02,STR_ILIST_OTHER_APPS_EXT01
                                           };
    int16                       i = 0;
    GUILIST_ITEM_T              item_t = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    uint16                      max_items = 0;
    uint16                      cur_item = 0;
    MMI_CTRL_ID_T               ctrl_id = MMISET_SET_NOTIFICATIONS_CTRL_ID;

    switch(msg_id) {
        case MSG_OPEN_WINDOW:

            MMK_SetAtvCtrl(win_id,ctrl_id);

#ifdef MMI_ORIG_UNISOC_SUPPORT
            max_items = 5;
#else
            max_items = 4;
#endif
            cur_item  = 0;
            //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d"
            GUILIST_SetMaxItem(ctrl_id,max_items, FALSE );
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            item_t.item_style    = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
            item_t.item_data_ptr = &item_data;

            for(i = 0; i < max_items-1; i++)
            {
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id = str_id[i];
				item_data.softkey_id[0] = STXT_SOFTKEY_CHANGE;
				item_data.softkey_id[1] = TXT_NULL;
				item_data.softkey_id[2] = STXT_RETURN;
				item_t.user_data = ID_NOTIFICATIONS_MESSAGES+i;
                if(!GUILIST_InsertItem(ctrl_id, &item_t, i)) {
                    break;
                }
				/*
				if(MMISet_GetNotification(i)){
					CTRLLIST_SetToggleState(ctrl_id,i,TRUE);
				}else{
					CTRLLIST_SetToggleState(ctrl_id,i,FALSE);
				}*/
            }
            SCI_MEMSET(&item_t, 0x00,sizeof(item_t));
            SCI_MEMSET(&item_data, 0x00, sizeof(item_data));
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
            item_t.item_data_ptr = &item_data;
            item_t.user_data = ID_NOTIFICATIONS_TYPE;
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id = STR_NOTIF_TYPE_EXT01;
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
			if(MMISet_GetNotificationShowType()==0)
            	item_data.item_content[1].item_data.text_id = STR_NOTIF_REGULAR_EXT01;
			else
            	item_data.item_content[1].item_data.text_id = STR_NOTIF_ICON_EXT01;
			GUILIST_InsertItem(ctrl_id, &item_t, 0);

            if(TRUE == MMIAPISET_IsDisableShareViaOption())
            {
                 CTRLLIST_RemoveItem(ctrl_id,1);
            }
            GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, cur_item);
            break;
        case MSG_FULL_PAINT: {
            //reset status bar
//             MMI_STRING_T    temp_str = {0};
//             MMI_GetLabelTextByLang(STR_OPT_NFS_NOTIFICATION_HDR, &temp_str);
//             if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len)) {
//                 GUIWIN_UpdateStb();
//             }
            SCI_MEMSET(&item_t, 0x00,sizeof(item_t));
            SCI_MEMSET(&item_data, 0x00, sizeof(item_data));
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
            item_t.item_data_ptr = &item_data;
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id = STR_NOTIF_TYPE_EXT01;
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
            item_data.softkey_id[0] = STXT_SELECT;
            item_data.softkey_id[1] = TXT_NULL;
            item_data.softkey_id[2] = STXT_RETURN;
			if(MMISet_GetNotificationShowType()==0)
            	item_data.item_content[1].item_data.text_id = STR_NOTIF_REGULAR_EXT01;
			else
            	item_data.item_content[1].item_data.text_id = STR_NOTIF_ICON_EXT01;
			item_data.item_content[1].is_opacity = TRUE;
// 			GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);
			GUILIST_ReplaceItem(ctrl_id, &item_t,0);

		if(FALSE == MMIAPISET_IsDisableShareViaOption())
		{
			if(MMISet_GetNotification(MMISET_NOTIFICATION_MESSAGES))//message
				CTRLLIST_SetToggleState(ctrl_id,1,TRUE);
			else
				CTRLLIST_SetToggleState(ctrl_id,1,FALSE);
			if(MMISet_GetNotification(MMISET_NOTIFICATION_CALLS))//call
				CTRLLIST_SetToggleState(ctrl_id,2,TRUE);
			else
				CTRLLIST_SetToggleState(ctrl_id,2,FALSE);
			if(MMISet_GetNotification(MMISET_NOTIFICATION_MAILS))//mail
				CTRLLIST_SetToggleState(ctrl_id,3,TRUE);
			else
				CTRLLIST_SetToggleState(ctrl_id,3,FALSE);
#ifdef MMI_ORIG_UNISOC_SUPPORT
			if(MMISet_GetNotification(MMISET_NOTIFICATION_OTHERS))//others app
				CTRLLIST_SetToggleState(ctrl_id,4,TRUE);
			else
				CTRLLIST_SetToggleState(ctrl_id,4,FALSE);
#endif
		}
		else
		{

			if(MMISet_GetNotification(MMISET_NOTIFICATION_CALLS))//call
				CTRLLIST_SetToggleState(ctrl_id,1,TRUE);
			else
				CTRLLIST_SetToggleState(ctrl_id,1,FALSE);
			if(MMISet_GetNotification(MMISET_NOTIFICATION_MAILS))//mail
				CTRLLIST_SetToggleState(ctrl_id,2,TRUE);
			else
				CTRLLIST_SetToggleState(ctrl_id,2,FALSE);
#ifdef MMI_ORIG_UNISOC_SUPPORT
			if(MMISet_GetNotification(MMISET_NOTIFICATION_OTHERS))//others app
				CTRLLIST_SetToggleState(ctrl_id,3,TRUE);
			else
				CTRLLIST_SetToggleState(ctrl_id,3,FALSE);
#endif
		}
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
			break;
        //case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, cur_item, &menu_id);
            switch (menu_id) {
				case ID_NOTIFICATIONS_TYPE:
					MMK_CreateWin((uint32 *)MMISET_SET_NOTIFICATIONS_TYPE_WIN_TAB, PNULL);
					break;
				case ID_NOTIFICATIONS_MESSAGES:
					if(MMISet_GetNotification(MMISET_NOTIFICATION_MESSAGES))
						MMISet_SetNotification(MMISET_NOTIFICATION_MESSAGES,FALSE);
					else
						MMISet_SetNotification(MMISET_NOTIFICATION_MESSAGES,TRUE);
					break;
				case ID_NOTIFICATIONS_CALLS:
					if(MMISet_GetNotification(MMISET_NOTIFICATION_CALLS))
						MMISet_SetNotification(MMISET_NOTIFICATION_CALLS,FALSE);
					else
						MMISet_SetNotification(MMISET_NOTIFICATION_CALLS,TRUE);
					break;
				case ID_NOTIFICATIONS_VOICE_MAIL:
					if(MMISet_GetNotification(MMISET_NOTIFICATION_MAILS))
						MMISet_SetNotification(MMISET_NOTIFICATION_MAILS,FALSE);
					else
						MMISet_SetNotification(MMISET_NOTIFICATION_MAILS,TRUE);
					break;
#ifdef MMI_ORIG_UNISOC_SUPPORT
				case ID_NOTIFICATIONS_OTHERS://temp
					if(MMISet_GetNotification(MMISET_NOTIFICATION_OTHERS))
					{
						MMISet_SetNotification(MMISET_NOTIFICATION_OTHERS,FALSE);
						MMIAPIWNS_Stop();
					}
					else{
						MMISet_SetNotification(MMISET_NOTIFICATION_OTHERS,TRUE);
						MMIAPIWNS_Init();
					}
					break;
#endif
				/*
                case ID_NOTIFICATIONS_LIST_MESSAGES:
					//to do.....
					//SPRD add
                    //MMIAPISET_OpenCallSettingsWindow();
                    break;

                case ID_NOTIFICATIONS_LIST_CALLS:
                    //to do.....
                    //SPRD add
                    //MMIAPIIM_OpenSetWin();
                    break;
                case ID_NOTIFICATIONS_LIST_VOICE_MAIL:
					//to do.....
                    //SPRD add
                    //MMIAPIPHONE_OpenBtryCapWin();
                    break;
                case ID_NOTIFICATIONS_LIST_ALARMS:
                    //to do.....
                    //SPRD add
                    //MMISET_EnterSettingShortcutWin();
                    break;

                case ID_NOTIFICATIONS_LIST_CALENDAR:
					//to do.....
                    //SPRD add
                    //MMIAPISET_EnterDisplaySettingWin();
                    break;

                case ID_NOTIFICATIONS_LIST_OUTLOOK:
					//to do.....
                    //SPRD add
                    //MMIAPIPHONE_OpenBtryCapWin();
                    break;
                case ID_NOTIFICATIONS_LIST_OPERATOR:
					//to do.....
                    //SPRD add
                    //MMIAPIPHONE_OpenBtryCapWin();
                    break;

                case ID_NOTIFICATIONS_LIST_FM_RADIO:
					//to do.....
                    //SPRD add
                    //MMIAPIPHONE_OpenBtryCapWin();
                    break;

                case ID_NOTIFICATIONS_LIST_MUSIC:
					//to do.....
                    //SPRD add
                    //MMIAPIPHONE_OpenBtryCapWin();
                    break;
                */
                default:
                    break;
            }
			//if(MMISet_GetNotification((MMISET_NOTIFICATION_TYPE_E)cur_item))
			//	MMISet_SetNotification((MMISET_NOTIFICATION_TYPE_E)cur_item,FALSE);
			//else
			//	MMISet_SetNotification((MMISET_NOTIFICATION_TYPE_E)cur_item,TRUE);
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
LOCAL MMI_RESULT_E HandleNotificationTypeWindow(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
){
    uint32              cur_index = 0;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMISET_SET_NOTIFICATION_TYPE_MENU_CTRL_ID;
    MMI_MENU_ID_T       menu_id = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;
	MMI_STRING_T  		string_title = {0};
	uint8				index = 0;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
	    GUIMENU_SetPopMenuCentered(ctrl_id, TRUE);
		//MMI_GetLabelTextByLang(STR_OPT_NFS_NOTIFICATION_HDR,&string_title);
		//GUIMENU_SetMenuTitle(&string_title,ctrl_id);
		index = MMISet_GetNotificationShowType();
		CTRLMENU_SetSelectedItem(ctrl_id, (uint16)index);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
	case MSG_FULL_PAINT:
		CTRLMENU_SetPopMenuSoftkeyType(ctrl_id,GUISK_DATA_TEXT_ID,GUISK_DATA_TEXT_ID,GUISK_DATA_IMAGE_ID);
		GUIMENU_SetDynamicMenuSoftkey(ctrl_id, STXT_SELECT, TXT_NULL, STXT_RETURN);
		//GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
		//GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SELECT, MIDDLE_BUTTON, TRUE);
		//GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_BACK, RIGHT_BUTTON, TRUE);
		break;
    //case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif
	    GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
	    switch (menu_id)
	    {
	    case ID_SET_NOTIFICATION_DETAILS:
			MMISet_SetNotificationShowType(0);
			break;
	    case ID_SET_NOTIFICATION_ICON_ONLY:
			MMISet_SetNotificationShowType(1);
	       	break;
	    }
		MMK_CloseWin(win_id);
		break;
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
	case MSG_CLOSE_WINDOW:
		break;
    default:
        result = MMI_RESULT_FALSE;
        break;
   }
    return (result);
}
