/****************************************************************************
** File Name:	   mmicc_speeddial.c								        *
** Author:																   *
** Date:		   08/23/2003											   *
** Copyright:	   2003 Spreadtrum, Incorporated. All Rights Reserved.	   *
** Description:    This file is used to handle speeddial 		   *
****************************************************************************
**						   Important Edit History						   *
** ------------------------------------------------------------------------*
** DATE 		  NAME			   DESCRIPTION							   *
** 08/23/2003		 bin.ji		   Create
** 
****************************************************************************/
#include "mmi_app_cc_trc.h"
#if defined(MMI_SPEEDDIAL_SUPPORT)
#define _MMI_SPEEDDIAL_C

/**----------------------------------------------------------------------*
**                         Include Files                                *
**----------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmi_nv.h"
#include "mmicc_speeddial.h"
#include "mmi_menutable.h"
#include "guilistbox.h"
#include "mmipb_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmicc_export.h"
#include "mmi_textfun.h"
//#include "guilabel.h"
#include "mmi_appmsg.h"
#include "mmidisplay_data.h"
#include "mmicc_nv.h"
#include "mmicc_id.h"
#include "mmicc_menutable.h"
#include "mmicc_text.h"
#include "mmipub.h"
#include "guiedit.h"
#include "guirichtext.h"
#include "mmipb_text.h"
#include "mmicom_banner.h"
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#include "mmiplmn_export.h"
#endif
#include "nvitem.h"
#include "variant_config_nv.h"
#include "mmipb_image.h"
#include "guitext.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMISPEEDDIAL_TITLE_HEIGHT MMI_TITLE_HEIGHT
#define	MMISPEEDDIAL_NUMBER_MAX_LEN				CC_MAX_TELE_NUM_LEN

#define MMISPEEDDIAL_LABEL_HEIGHT				MMI_LIST_ITEM_HEIGHT

#define MMI_SPEED_DIAL_PB_STORAGE_ID(X)   (X>>16)
#define MMI_SPEED_DIAL_PB_ENTRY_ID(X)   (X&0x0000FFFF) 

LOCAL uint8 s_cur_index = 0;


/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
#ifdef MMI_WIDGET_SPEED_DIAL
PUBLIC CC_SPEEDKEY_ENTRY_T s_speeddial_record[SPEEDKEY_MAX_NUM];//记录快捷拨号对应号码
#else
LOCAL CC_SPEEDKEY_ENTRY_T s_speeddial_record[SPEEDKEY_MAX_NUM];//记录快捷拨号对应号码
#endif
LOCAL CC_SPEEDKEY_ENTRY_T s_pb_speeddial_entry;//电话簿快捷拨号信息
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description : speed dial edit select contact callback
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void CC_SpeadDailEditSelectContactCallback(MMIPB_HANDLE_T handle);

/*****************************************************************************/
//	Description : 
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void CC_SpeadDailListSelectContactCallback(MMIPB_HANDLE_T handle);

/*****************************************************************************/
//	Description : the handle function of speeddial edit window
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleSpeedDialEditWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
//	Description : the green key MSG_UP handle function of speeddial edit window,
//	Global resource dependence : 
//	Author:Michael.Shi
//	Note: especially for multisim 
/*****************************************************************************/
LOCAL void HandleSpeedDialEditGreenUpMsg(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//	Description : the handle function of speeddial menu window
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleSpeedDialMenuWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
//	Description : the handle function of speeddial menu window
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleSpeedDialListMenuWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
//	Description : the handle function of speeddial menu window
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleSpeedDialDeleteQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
//	Description : the handle function of speeddial list window
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleSpeedDialListWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
//	Description : to add the string to listbox
//	Global resource dependence : 
//	Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void  AppendItemToListBox(
                                MMI_CTRL_ID_T ctrl_id,
                                uint16		  index,
                                wchar* string_ptr,
                                uint16		  string_len,
                                MMI_TEXT_ID_T left_softkey_id,
                                MMI_TEXT_ID_T right_softkey_id,
                                BOOLEAN       is_update
                                );

/*****************************************************************************/
//	Description : to add the string to listbox
//	Global resource dependence : 
//	Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void  AppendItemToListBoxNew(
                                MMI_CTRL_ID_T ctrl_id,
                                uint16		  index,
                                MMI_STRING_T* string_ptr,
                                MMI_TEXT_ID_T string2_text_id,
                                MMI_TEXT_ID_T left_softkey_id,
                                MMI_TEXT_ID_T right_softkey_id,
                                GUIANIM_FILE_INFO_T* anim_path,
                                wchar* name_abbr,
                                uint32 item_user_data,
                                BOOLEAN       is_update
                                );
                
/*****************************************************************************/
// 	Description : to handle alert window closed message
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlertClosedMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         );

/*****************************************************************************/
//	Description : check whether current selected in list is NULL
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSelectedListNULL(void);

/*****************************************************************************/
//	Description : open the window for speeddial List
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void OpenSpeedDialListWin(void);

/*****************************************************************************/
//	Description : open the window for speeddial menu
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void OpenSpeedDialMenuWin(void);

/*****************************************************************************/
//	Description : open the window for speeddial edit
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void OpenSpeedDialEditWin(
                                wchar* number_ptr,
                                uint16 number_len
                                );

/*****************************************************************************/
//	Description : 得到选中项号码，为editbox准备好数据
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void GetSelectedItem(
                           uint16   list_index,
                           wchar*   number_ptr,
                           uint16   max_len,
                           uint16*  number_len_ptr
                           );

/*****************************************************************************/
//	Description : to save the  speed key item after edit
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void SaveSpeedDialItem(
                             uint8* number_ptr,
                             uint16 max_len,
                             uint8 number_len
                             );

/*****************************************************************************/
//	Description : to save the  speed key item after edit
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void SaveSpeedDialItemNew(
                             uint8* number_ptr,
                             uint16 max_len,
                             uint8 number_len,
                             BOOLEAN is_sms
                             );

/*****************************************************************************/
//	Description : 产生类似“按键3：xxxxx”的list item字串
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void GenSpeedStr(
                       uint16               index, 
                       CC_SPEEDKEY_ENTRY_T  *speedkey_entry_ptr,
                       MMI_STRING_T         *name_str_ptr,
                       uint16               max_name_str_len
                       );

/*****************************************************************************/
//	Description : 产生类似“3(xxxxx)”的list item字串
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void GenSpeedDialListItemInfo(
                       uint16               index,                  //in
                       CC_SPEEDKEY_ENTRY_T  *speedkey_entry_ptr,    //in
                       MMI_STRING_T         *name_str_ptr,          //out
                       wchar               *name_abbr,        //out
                       GUIANIM_FILE_INFO_T* pb_anim_path_ptr, //out
                       uint32 *item_user_data //out
                       );

/*****************************************************************************/
// 	Description : fill back ground with back ground color
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void FillRectWithBGColor(void);

/*****************************************************************************/
// 	Description : to handle Green Key UP_MSG for speed view dial window
//	Global resource dependence : 
//  Author: Michael.shi
//	Note: especially for multisim
/*****************************************************************************/
LOCAL void HandleListGreenUpMSG(
                                            MN_DUAL_SYS_E dual_sys,
                                            MMI_WIN_ID_T win_id );

/*****************************************************************************/
// 	Description : to handle speed dial view window
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleViewWinMsg(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     );

/*****************************************************************************/
//	Description : generate phone string of view window
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void GenerateViewPhoneString(
                                   CC_SPEEDKEY_ENTRY_T  *speedkey_entry_ptr,    //in
                                   MMI_STRING_T         *phone_str_ptr,          //out
                                   uint16               max_name_str_len        //in
                                   );

/*****************************************************************************/
//	Description : generate name string of view window
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void GenerateViewNameString(
                                  CC_SPEEDKEY_ENTRY_T  *speedkey_entry_ptr,    //in
                                  MMI_STRING_T         *name_str_ptr,          //out
                                  uint16               max_name_str_len        //in
                                  );

/*****************************************************************************/
//	Description : open the window for speeddial
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void SpeedDial(
                               MN_DUAL_SYS_E dual_sys,
                               uint16        key_num
                               );

/*****************************************************************************/
//	Description : Get Speed Dial Offset
//	Global resource dependence : 
//	Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL uint8 GetSpeedDialOffset(void);

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//	Description : set item menu
//	Global resource dependence : 
//	Author:rong.gu
//	Note:
/*****************************************************************************/
LOCAL void SetItemMenuTitle(void);
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSpeedDialSettingsWinMsg(
				     MMI_WIN_ID_T      win_id, 
 				     MMI_MESSAGE_ID_E  msg_id, 
 				     DPARAM            param
 				     );

/**---------------------------------------------------------------------------*/
/**                         Constant Variables                                */
/**---------------------------------------------------------------------------*/

WINDOW_TABLE(MMISPEEDDIAL_SETTINGS_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleSpeedDialSettingsWinMsg),
    WIN_ID( MMISPEEDDIAL_SETTINGS_WIN_ID),
    WIN_TITLE(STXT_MEMORY_OPTIONS_SPEED_DIAL),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISPEEDDIAL_SETTINGS_LIST_CTRL_ID),
    END_WIN
};


#if 0
WINDOW_TABLE(MMISPEEDDIAL_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_PB_SET_SPEEDDIAL_NUM ),
    WIN_FUNC((uint32) CC_HandleSpeedDialListWinMsg ),    
    WIN_ID( MMISPEEDDIAL_LIST_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMISPEEDDIAL_LISTBOX_CTRL_ID),
    END_WIN    
};
#else
WINDOW_TABLE(MMISPEEDDIAL_LIST_WIN_TAB) = 
{
    WIN_FUNC((uint32) CC_HandleSpeedDialListWinMsg ),    
    WIN_ID( MMISPEEDDIAL_LIST_WIN_ID ),
    WIN_TITLE(STXT_HEADER_SPEED_DIALLING),		  
    WIN_SOFTKEY(STXT_SOFTKEY_MENU_MK, TXT_NULL, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMISPEEDDIAL_LISTBOX_CTRL_ID),
    END_WIN    
};

#endif

#ifdef ZDT_PRODUCT_BH
LOCAL MMI_RESULT_E SpeedDirectionsWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

WINDOW_TABLE(MMISPEEDDIAL_DIRECTIONS_WIN_TAB) = 
{
    WIN_FUNC((uint32)SpeedDirectionsWinHandleMsg),
    WIN_ID(MMISPEEDDIAL_DIRECTIONS_WIN_ID),
    WIN_TITLE(TXT_DIRECTIONS),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMISPEEDDIAL_DIRECTIONS_CTRL_ID),
    END_WIN
};


#endif

WINDOW_TABLE(MMISPEEDDIAL_MENUEDIT_WIN_TAB) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC((uint32) CC_HandleSpeedDialMenuWinMsg ),    
        WIN_ID( MMISPEEDDIAL_MENU_WIN_ID ),
 #if defined (MAINLCD_SIZE_128X160)
       WIN_TITLE(STXT_SOFTKEY_MENU_MK),
#endif       
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#ifdef DPHONE_SUPPORT//syy modify 2010.12.1 -begin
        CREATE_MENU_CTRL(MENU_CC_SPEEDDIAL_OPT_EDIT, MMISPEEDDIAL_POPMENU_CTRL_ID),
#else
        CREATE_POPMENU_CTRL(MENU_CC_SPEEDDIAL_OPT_EDIT, MMISPEEDDIAL_POPMENU_CTRL_ID),
#endif//syy modify 2010.12.1 -end

//boda, add title
#if defined (MAINLCD_SIZE_128X160)
        WIN_TITLE(TXT_COMMON_CONTENTS),
#endif//#if defined (MAINLCD_SIZE_128X160)

        END_WIN    
};

WINDOW_TABLE(MMISPEEDDIAL_MENUEDITDEL_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) CC_HandleSpeedDialMenuWinMsg ),    
    WIN_ID( MMISPEEDDIAL_MENU_WIN_ID ),
 #if defined (MAINLCD_SIZE_128X160)
       WIN_TITLE(STXT_SOFTKEY_MENU_MK),
#endif         
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),   
#ifdef DPHONE_SUPPORT//syy modify 2010.12.1 -begin
    CREATE_MENU_CTRL(MENU_CC_SPEEDDIAL_OPT_EDITDEL, MMISPEEDDIAL_POPMENU_CTRL_ID),  
#else
    CREATE_POPMENU_CTRL(MENU_CC_SPEEDDIAL_OPT_EDITDEL, MMISPEEDDIAL_POPMENU_CTRL_ID), 
#endif//syy modify 2010.12.1 -end
    END_WIN    
};

WINDOW_TABLE(MMISPEEDDIAL_LIST_ITEM_EMPTY_MENU_WIN_TAB) = 
{
    WIN_FUNC((uint32) CC_HandleSpeedDialListMenuWinMsg ),    
    WIN_ID( MMISPEEDDIAL_MENU_WIN_ID ),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),   
    CREATE_POPMENU_CTRL(MENU_CC_SPEEDDIAL_ITEM_EMPTY, MMISPEEDDIAL_POPMENU_CTRL_ID), 
    END_WIN    
};

WINDOW_TABLE(MMISPEEDDIAL_LIST_ITEM_SMS_MENU_WIN_TAB) = 
{
    WIN_FUNC((uint32) CC_HandleSpeedDialListMenuWinMsg ),    
    WIN_ID( MMISPEEDDIAL_MENU_WIN_ID ),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),   
    CREATE_POPMENU_CTRL(MENU_CC_SPEEDDIAL_ITEM_SMS, MMISPEEDDIAL_POPMENU_CTRL_ID), 
    END_WIN    
};

WINDOW_TABLE(MMISPEEDDIAL_LIST_ITEM_CALL_MENU_WIN_TAB) = 
{
    WIN_FUNC((uint32) CC_HandleSpeedDialListMenuWinMsg ),    
    WIN_ID( MMISPEEDDIAL_MENU_WIN_ID ),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),   
    CREATE_POPMENU_CTRL(MENU_CC_SPEEDDIAL_ITEM_CALL, MMISPEEDDIAL_POPMENU_CTRL_ID), 
    END_WIN    
};

WINDOW_TABLE(MMISPEEDDIAL_EDIT_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_INPUTNUM ),
    WIN_FUNC((uint32) CC_HandleSpeedDialEditWinMsg ),    
    WIN_ID( MMISPEEDDIAL_EDIT_WIN_ID ),
    WIN_SOFTKEY(STXT_MAIN_PB, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_PHONENUM_CTRL((SPEEDKEY_MAX_LEN - 2),MMISPEEDDIAL_EDITBOX_CTRL_ID),
    END_WIN    
};

// 查看speed dial设置信息
WINDOW_TABLE(MMISPEEDDIAL_VIEW_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_VIEW ),
    WIN_FUNC((uint32) HandleViewWinMsg ),    
    WIN_ID( MMISPEEDDIAL_VIEW_WIN_ID ),
#ifndef MMI_GUI_STYLE_TYPICAL
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_EDIT, TXT_NULL, STXT_RETURN),    
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    
    CREATE_RICHTEXT_CTRL(MMISPEEDDIAL_VIEW_RICHTEXT_CTRL_ID),
    END_WIN
};

/**---------------------------------------------------------------------------*
**                         Function Definitions                              *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description : speed dial edit select contact callback
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void CC_SpeadDailEditSelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMISPEEDDIAL_EDIT_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);   
}

/*****************************************************************************/
//	Description : 
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void CC_SpeadDailListSelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMISPEEDDIAL_LIST_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);   
}
/*****************************************************************************/
// 	Description : fill back ground with back ground color
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void FillRectWithBGColor(void)
{
    GUI_RECT_T          rect = {0};
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    rect = MMITHEME_GetClientRect();
    
    LCD_FillRect(&lcd_dev_info, rect, MMI_WINDOW_BACKGROUND_COLOR);
}            
/*****************************************************************************/
// 	Description : to handle speed view dial window
//	Global resource dependence : 
//  Author: Michael.shi
//	Note: 
/*****************************************************************************/
LOCAL void HandleListGreenUpMSG(
                                            MN_DUAL_SYS_E dual_sys,
                                            MMI_WIN_ID_T win_id )
{    
    wchar           number_ptr[SPEEDKEY_MAX_LEN + 1] = {0};
    uint16          number_len = 0;    
    uint8            temp_str[SPEEDKEY_MAX_LEN + 1] = {0};
    
    uint32          list_index = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    SCI_MEMSET(number_ptr, 0x00, ((SPEEDKEY_MAX_LEN + 1)*sizeof(wchar)));
    GetSelectedItem(list_index, number_ptr, SPEEDKEY_MAX_LEN, &number_len);
    MMI_WSTRNTOSTR((uint8 *)temp_str, SPEEDKEY_MAX_LEN + 1, number_ptr, number_len, number_len);

    MMIAPICC_MakeCall(
                                    dual_sys,
                                    temp_str,
                                    number_len,
                                    PNULL,
                                    PNULL,
                                    MMIAPICC_GetCallSimType(dual_sys),
                                    CC_CALL_NORMAL_CALL,
                                    PNULL
                                    );
}

/*****************************************************************************/
// 	Description : to handle speed view dial window
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleViewWinMsg(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32          list_index = (uint32)MMK_GetWinAddDataPtr(win_id);
    wchar           number_ptr[SPEEDKEY_MAX_LEN + 1] = {0};
    uint16          number_len = 0;
    
    switch( msg_id )
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMISPEEDDIAL_VIEW_RICHTEXT_CTRL_ID);
        GUIWIN_SetSoftkeyTextId( win_id,  TXT_EDIT, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef  MMI_MULTI_GREEN_KEY_SINGLE
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        HandleListGreenUpMSG(MN_DUAL_SYS_MAX,win_id);
        break;
#else
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        HandleListGreenUpMSG(MMIAPICC_CheckDualSysType(msg_id, MN_DUAL_SYS_MAX),win_id);        
        break;        
#endif

#ifdef VT_SUPPORT
    case MSG_KEYLONG_GREEN:
        {
            uint8 temp_str[SPEEDKEY_MAX_LEN + 1] = {0};
             SCI_MEMSET(number_ptr, 0x00, ((SPEEDKEY_MAX_LEN + 1)*sizeof(wchar)));
            GetSelectedItem(list_index, number_ptr, SPEEDKEY_MAX_LEN, &number_len);
            MMI_WSTRNTOSTR((uint8 *)temp_str, SPEEDKEY_MAX_LEN + 1, number_ptr, number_len, number_len);

            MMIAPICC_MakeCall(
                                            MN_DUAL_SYS_1,
                                            temp_str,
                                            number_len,
                                            PNULL,
                                            PNULL,
                                            CC_MAJOR_SIM_CALL,
                                            CC_CALL_VIDEO_CALL,
                                            PNULL
                                            );
        }
        break;
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
    if (PNULL != param)
    {
        uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;        
        switch (src_id)
        {
        case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
            MMK_SendMsg(win_id, MSG_CTL_OK, PNULL);
            break;
        case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
            MMK_SendMsg(win_id, MSG_CTL_CANCEL, PNULL);
            break;          
        default:
            break;
        }
    }
    break;    
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
 
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:    
        GetSelectedItem(list_index, number_ptr, SPEEDKEY_MAX_LEN, &number_len);
        OpenSpeedDialEditWin(number_ptr, number_len);
        MMK_CloseWin( win_id );
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    case MSG_FULL_PAINT:
        FillRectWithBGColor();
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return ( recode );
}

/*****************************************************************************/
//	Description : open the window for speeddial List
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAPICC_OpenSetSpeedDial(MNBCD *number_ptr, uint8 number_len, uint8 npi_ton)
{
    //    SCI_TRACE_LOW("MMIAPICC_OpenSetSpeedDial.c:enter function CC_OpenSetSpeedDial()");
    
    //set
    s_pb_speeddial_entry.number_len = MIN(number_len, PHONEBOOK_MAX_DN_NUMBER_LEN);
    SCI_MEMCPY(s_pb_speeddial_entry.number, number_ptr, s_pb_speeddial_entry.number_len);
    s_pb_speeddial_entry.npi_ton = npi_ton;
    
    OpenSpeedDialListWin();
}

/*****************************************************************************/
//	Description : open the window for speeddial List
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void OpenSpeedDialListWin(void)
{
    //    SCI_TRACE_LOW("mmi_speeddial.c:enter function OpenSpeedDialListWin()");
    
    //if( MMK_IsOpenWin( MMISPEEDDIAL_LIST_WIN_ID ))	//@samboo modify 20071001
    {
        MMK_CloseWin( MMISPEEDDIAL_LIST_WIN_ID );
    }
    
    //create the speeddial list window
    MMK_CreateWin((uint32 *)MMISPEEDDIAL_LIST_WIN_TAB, PNULL);
    
    MMK_SetAtvCtrl( MMISPEEDDIAL_LIST_WIN_ID, MMISPEEDDIAL_LISTBOX_CTRL_ID );
}

#if 0
/*****************************************************************************/
//	Description : the handle function of speeddial list window
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleSpeedDialListWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint8 i = 0;
    MMI_STRING_T name_str = {0};
    MN_RETURN_RESULT_E	ret_val = MN_RETURN_FAILURE;
    CC_SPEEDKEY_ENTRY_T	speedkey_entry = {0};
    wchar prompt[SPEEDKEY_PROMPT_LEN + 1] = {0};
    uint16 cur_index = 0;
    uint8 speed_dial_offset = 0;
    
    //SCI_TRACE_LOW:"mmi_speeddial.c: CC_HandleSpeedDialListWinMsg( %d, %d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_567_112_2_18_2_10_36_298,(uint8*)"dd",win_id, msg_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
    case MSG_GET_FOCUS:

        if (MSG_GET_FOCUS == msg_id)
        {
            cur_index = GUILIST_GetCurItemIndex(MMISPEEDDIAL_LISTBOX_CTRL_ID);
        }

        GUILIST_SetMaxItem(MMISPEEDDIAL_LISTBOX_CTRL_ID, SPEEDKEY_MAX_NUM, FALSE );//max item 7/8
        
        speed_dial_offset = GetSpeedDialOffset();
      
        //append item for list
        for( i = 0; i < SPEEDKEY_MAX_NUM - speed_dial_offset; i++ )
        {
            SCI_MEMSET(&speedkey_entry, 0, sizeof(speedkey_entry));
            // @baokun : 电话本里设置2快捷拨号后，在idle长按2，提示未设置号码
            MMINV_READ((MMI_CC_SPEEDKEY_2 + speed_dial_offset + i), &speedkey_entry, ret_val);
 
            if( MN_RETURN_SUCCESS != ret_val )
            {
                SCI_MEMSET(&speedkey_entry, 0, sizeof(speedkey_entry));
                speedkey_entry.number_len = 0;
                s_speeddial_record[i] = speedkey_entry;     
                
                // @baokun : 电话本里设置2快捷拨号后，在idle长按2，提示未设置号码
                MMINV_WRITE((MMI_CC_SPEEDKEY_2 + speed_dial_offset + i), &speedkey_entry);
            }
            else
            {
                s_speeddial_record[i]= speedkey_entry;
            }
            
            //合成list的字符串，同时判断是否存在于pb中
            name_str.wstr_ptr = prompt;
            name_str.wstr_len = SPEEDKEY_PROMPT_LEN;
            
            GenSpeedStr(i, &speedkey_entry, &name_str, SPEEDKEY_PROMPT_LEN);/*lint !e506 */
            
            {
                AppendItemToListBox(
                    MMISPEEDDIAL_LISTBOX_CTRL_ID, 
                    i,
                    name_str.wstr_ptr, 
                    name_str.wstr_len,
                    STXT_OPTION, 
                    STXT_EXIT,
                    FALSE);
            }
        } 
        
        if (MSG_GET_FOCUS == msg_id)
        {
            GUILIST_SetCurItemIndex(MMISPEEDDIAL_LISTBOX_CTRL_ID, cur_index);
        }
        else
        {
            GUILIST_SetCurItemIndex(MMISPEEDDIAL_LISTBOX_CTRL_ID, 0);
        }
#if 0	//defined(MMI_WIDGET_SPEED_DIAL)
    {
        uint8 * cur_item_index = (uint8*)(MMK_GetWinAddDataPtr(win_id));
        cur_index = (uint16)(*cur_item_index) -1;
        MMK_SetAtvCtrl( win_id, MMISPEEDDIAL_LISTBOX_CTRL_ID );
        GUILIST_SetCurItemIndex(MMISPEEDDIAL_LISTBOX_CTRL_ID, cur_index);
    }
#endif

        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:

#ifndef MMI_GUI_STYLE_TYPICAL
	case MSG_CTL_LIST_LONGOK:
#endif
        OpenSpeedDialMenuWin();
        break;  
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            uint32	list_index = 0;
            uint16 number_len = 0;
            wchar number_ptr[SPEEDKEY_MAX_LEN + 1] = {0};
            
            list_index = GUILIST_GetCurItemIndex( MMISPEEDDIAL_LISTBOX_CTRL_ID );
            //set data for editbox
            SCI_MEMSET(number_ptr, 0x00, ((SPEEDKEY_MAX_LEN + 1)*sizeof(wchar)));
            GetSelectedItem(list_index, number_ptr, SPEEDKEY_MAX_LEN, &number_len);
            
            //get current pb number
            if (0 == number_len)
            {
                uint8 temp_string[SPEEDKEY_MAX_LEN + 1] = {0};
                number_len = (uint16)MMIAPICOM_GenDispNumber((MN_NUMBER_TYPE_E)((s_pb_speeddial_entry.npi_ton & 0xf0) >> 4),
                    s_pb_speeddial_entry.number_len,
                    s_pb_speeddial_entry.number,
                    temp_string,
                    SPEEDKEY_MAX_LEN        
                    );
	            MMI_STRNTOWSTR(number_ptr, SPEEDKEY_MAX_LEN + 1, (uint8 *)temp_string, SPEEDKEY_MAX_LEN + 1, SPEEDKEY_MAX_LEN + 1);	      
            }
            //open edit win
            OpenSpeedDialEditWin(number_ptr, number_len);
        }
        break;
        
    case MMI_SELECT_CONTACT_CNF:
        // get the number from the PB
        if (MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)param) > 0)
        {
            MMIPB_BCD_NUMBER_T	bcd_number = {0};
            uint8				temp_num[MMISPEEDDIAL_NUMBER_MAX_LEN + 1]= {0};
            wchar 				tele_num[MMISPEEDDIAL_NUMBER_MAX_LEN + 1] = {0};             
            uint16                      number_len = 0;
            MMI_STRING_T         temp_str = {0};

            if (MMIAPIPB_GetSelectNumber((MMIPB_HANDLE_T)param, &bcd_number,MMIPB_BCD_NUMBER_MAX_LEN, 0))
            {
                number_len = MMIAPIPB_BCDNumberToString(&bcd_number, 
                                                       (MMISPEEDDIAL_NUMBER_MAX_LEN + 2), 
                                                       temp_num);
//                 number_len = MMIAPICOM_GenDispNumber(
//                     MMIPB_GetNumberTypeFromUint8(bcd_number.npi_ton), 
//                     (uint8)MIN(bcd_number.number_len, MMISPEEDDIAL_NUMBER_MAX_LEN), 
//                     bcd_number.number,  
//                     temp_num,
//                     (MMISPEEDDIAL_NUMBER_MAX_LEN + 2)
//                     );                
                SaveSpeedDialItem(temp_num, number_len, (uint8)number_len);
#if defined(MMI_GUI_STYLE_TYPICAL)
                MMIPUB_OpenAlertSuccessWin(TXT_SET_SUCCESS);
#endif

                MMI_STRNTOWSTR(tele_num, MMISPEEDDIAL_NUMBER_MAX_LEN + 1, 
                     (uint8 *)temp_num, MMISPEEDDIAL_NUMBER_MAX_LEN + 1, 
                     MMISPEEDDIAL_NUMBER_MAX_LEN + 1);   
                temp_str.wstr_len = number_len;
                temp_str.wstr_ptr = tele_num;
                cur_index = GUILIST_GetCurItemIndex( MMISPEEDDIAL_LISTBOX_CTRL_ID );
                GenSpeedStr(               
                    cur_index,
                    &s_speeddial_record[cur_index],
                    &temp_str,
                    SPEEDKEY_PROMPT_LEN/*lint !e506 */
                    );
                AppendItemToListBox(                  
                    MMISPEEDDIAL_LISTBOX_CTRL_ID, 
                    cur_index,
                    temp_str.wstr_ptr, 
                    temp_str.wstr_len,
                    STXT_OPTION, 
                    STXT_EXIT,
                    TRUE);
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:    
        {
        #ifdef MMI_MULTI_GREEN_KEY_SINGLE
            CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;            
        #else
            CC_CALL_SIM_TYPE_E sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);
            MN_DUAL_SYS_E  dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_MAX);
        #endif
            
            wchar number_ptr[SPEEDKEY_MAX_LEN + 1] = {0};
            uint16 number_len = 0;
            uint8 temp_string[SPEEDKEY_MAX_LEN + 1] = {0};
            uint16 list_index = GUILIST_GetCurItemIndex( MMISPEEDDIAL_LISTBOX_CTRL_ID );
            SCI_MEMSET(number_ptr, 0x00, ((SPEEDKEY_MAX_LEN + 1)*sizeof(wchar)));
            GetSelectedItem(list_index, number_ptr, SPEEDKEY_MAX_LEN, &number_len);
            MMI_WSTRNTOSTR((uint8 *)temp_string, SPEEDKEY_MAX_LEN + 1, number_ptr, number_len, number_len);

        #ifdef MMI_MULTI_GREEN_KEY_SINGLE
            MMIAPICC_MakeCall(
                                            MN_DUAL_SYS_MAX,
                                            temp_string,
                                            number_len,
                                            PNULL,
                                            PNULL,
                                            sim_type,
                                            CC_CALL_NORMAL_CALL,
                                            PNULL
                                            );
        #else            
            MMIAPICC_MakeCall(
                                            dual_sys,
                                            temp_string,
                                            number_len,
                                            PNULL,
                                            PNULL,
                                            sim_type,
                                            CC_CALL_NORMAL_CALL,
                                            PNULL
                                            );
        #endif
        }
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        //        SCI_TRACE_LOW("CC_HandleSpeedDialListWinMsg() msg_id = %d", msg_id);
        break;
    }
    return result;
}
#else

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
//        
/*****************************************************************************/
LOCAL BOOLEAN SpeedDialListUpdateListContentItem(
											 MMI_WIN_ID_T win_id,
											 MMI_CTRL_ID_T ctrl_id, 
											 void *item_ptr)
{
	GUILIST_NEED_ITEM_CONTENT_T* item_content_ptr = item_ptr;
	GUIANIM_FILE_INFO_T anim_path = {0};
	MMIPB_CUSTOM_PHOTO_T* custom_photo_ptr = PNULL;
	MMIPB_CONTACT_T* contact_ptr = PNULL;
	MMIPB_CONTACT_BASE_INFO_T contact_node ={0};
	GUILIST_ITEM_T* list_item_ptr = PNULL;
	uint32 item_index  = 0;    
	uint16 item_content_index = 0;
	BOOLEAN is_photo_id_exist = FALSE;
	uint32 item_user_data = 0;
	GUILIST_ITEM_DATA_T item_data = {0};
	
	
	if(PNULL == item_ptr)
	{
		return FALSE;
	}
	item_index =((GUILIST_NEED_ITEM_CONTENT_T*)item_ptr)->item_index;    
	list_item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id,item_index);
	if(PNULL == list_item_ptr)
	{
		return FALSE;
	}
	custom_photo_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_CUSTOM_PHOTO_T));
	contact_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_CONTACT_T));
	
	if(custom_photo_ptr == PNULL || contact_ptr == PNULL)
	{
		if(custom_photo_ptr != PNULL)
		{
			SCI_FREE(custom_photo_ptr);
		}        
		if(contact_ptr != PNULL)
		{
			SCI_FREE(contact_ptr);
		}        
		return FALSE;        
	}
	
	item_user_data=list_item_ptr->user_data;
	if(item_user_data){
		uint16 storage_id = MMI_SPEED_DIAL_PB_STORAGE_ID(item_user_data);
		uint16 entry_id = MMI_SPEED_DIAL_PB_ENTRY_ID(item_user_data);
		MMIPB_GetContactInfo(entry_id, storage_id, contact_ptr);
		item_content_index = ((GUILIST_NEED_ITEM_CONTENT_T*)item_ptr)->item_content_index;
		if((contact_ptr->photo_id_flag & MMIPB_PHOTO_ID_EXIST))
		{
			if( MMIPB_ReadPhotoFile(entry_id, storage_id, custom_photo_ptr))
			{
				if (MMIAPIFMM_IsFileExist(
					(const uint16 *)custom_photo_ptr->file_name,
					(uint16)(custom_photo_ptr->file_name_len)
					))
				{
					anim_path.full_path_wstr_ptr = custom_photo_ptr->file_name;
					anim_path.full_path_wstr_len = custom_photo_ptr->file_name_len;
					item_data.item_content[item_content_index].item_data_type = GUIITEM_DATA_ANIM_PATH;
					item_data.item_content[item_content_index].item_data.anim_path_ptr =&anim_path;
				}
				
			}
		}
		else
		{
			return FALSE; 
			
		}
	}
	GUILIST_SetItemContent( ctrl_id, &item_data.item_content[item_content_index], item_index, item_content_index );
	SCI_FREE(custom_photo_ptr);
	SCI_FREE(contact_ptr);
	
	return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
//        
/*****************************************************************************/
LOCAL void CC_SpeedDialDisplaybannerInfo(MMI_WIN_ID_T win_id, MMI_TEXT_ID_T text_id, uint16 cur_index)
{
	MMICOM_BANNER_INFO_T banner_info = {0};
	MMI_STRING_T banner_str = {0};
	MMI_STRING_T    result_str = {0};
	wchar result_wchar[200] = {0};
	result_str.wstr_ptr = result_wchar;

	if(cur_index >= SPEEDKEY_MAX_NUM)
	{
		banner_info.banner_type = MMI_BANNER_TEXT;
		banner_info.start_point.x = 0;
		banner_info.start_point.y = 0;
		banner_info.text1_id = text_id;
	}
	else
	{
		#ifdef ZDT_PRODUCT_Z685S
		MMIAPICOM_CustomGenerateNumReplaceStrByTextId(text_id, 
				L"%N", 
				cur_index+1,
				200, 
				&result_str);
		#else
		MMIAPICOM_CustomGenerateNumReplaceStrByTextId(text_id, 
			L"%N", 
			cur_index+2,
			200, 
			&result_str);
		#endif
		banner_str.wstr_ptr = result_str.wstr_ptr;
		banner_str.wstr_len = result_str.wstr_len;
		banner_info.banner_type = MMI_BANNER_STR_TEXT;
		banner_info.start_point.x = 0;
		banner_info.start_point.y = 0;
		banner_info.text1_str.wstr_ptr= banner_str.wstr_ptr;	
		banner_info.text1_str.wstr_len= banner_str.wstr_len;	
	}
	
	MMIAPICOM_OpenBannerWin(win_id, banner_info);
}

#ifdef PHONE_NUMBER_VOICE
static MMI_WIN_ID_T s_hero_speedcall_win_id = 0;
#endif

/*****************************************************************************/
//	Description : the handle function of speeddial list window
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleSpeedDialListWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	uint8 i = 0;
	MMI_STRING_T name_str = {0};
	MMI_STRING_T    title_str = {0};
	MN_RETURN_RESULT_E	ret_val = MN_RETURN_FAILURE;
	CC_SPEEDKEY_ENTRY_T	speedkey_entry = {0};
	wchar prompt[SPEEDKEY_PROMPT_LEN + 1] = {0};
	uint16 cur_index = 0;
	uint8 speed_dial_offset = 0;
	
	//SCI_TRACE_LOW:"mmi_speeddial.c: CC_HandleSpeedDialListWinMsg( %d, %d)"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_567_112_2_18_2_10_36_298,(uint8*)"dd",win_id, msg_id);
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
	case MSG_GET_FOCUS:
#ifdef PHONE_NUMBER_VOICE
	s_hero_speedcall_win_id = win_id;
	Trace_Log_Buf_Print("CC_HandleSpeedDialListWinMsg MSG_OPEN_WINDOW");

#endif

		MMI_GetLabelTextByLang(STXT_HEADER_SPEED_DIALLING, &title_str);
		if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str.wstr_ptr, title_str.wstr_len))
		{
			GUIWIN_UpdateStb();
		}
		
		//if (MSG_GET_FOCUS == msg_id)
		{
		//	cur_index = GUILIST_GetCurItemIndex(MMISPEEDDIAL_LISTBOX_CTRL_ID);
		}
		
		GUILIST_SetMaxItem(MMISPEEDDIAL_LISTBOX_CTRL_ID, SPEEDKEY_MAX_NUM, FALSE );//max item 7/8
		
		speed_dial_offset = GetSpeedDialOffset();
		
		//append item for list
		for( i = 0; i < SPEEDKEY_MAX_NUM - speed_dial_offset; i++ )
		{
			GUIANIM_FILE_INFO_T anim_path = {0};
			wchar	anim_full_path_wstr[MMIFILE_FULL_PATH_MAX_LEN +1] = {0};
			uint32 item_user_data = 0;
			wchar name_abbr[3] = {0};
			MMI_TEXT_ID_T string2_text_id = TXT_NULL;
			anim_path.full_path_wstr_ptr = anim_full_path_wstr;

			SCI_MEMSET(&speedkey_entry, 0, sizeof(speedkey_entry));
			// @baokun : 电话本里设置2快捷拨号后，在idle长按2，提示未设置号码
			MMINV_READ((MMI_CC_SPEEDKEY_1 + speed_dial_offset + i), &speedkey_entry, ret_val);
			
			if( MN_RETURN_SUCCESS != ret_val )
			{
				SCI_MEMSET(&speedkey_entry, 0, sizeof(speedkey_entry));
				speedkey_entry.number_len = 0;
				s_speeddial_record[i] = speedkey_entry;     
				
				// @baokun : 电话本里设置2快捷拨号后，在idle长按2，提示未设置号码
				MMINV_WRITE((MMI_CC_SPEEDKEY_1 + speed_dial_offset + i), &speedkey_entry);
			}
			else
			{
				s_speeddial_record[i]= speedkey_entry;
			}
			
			//合成list的字符串，同时判断是否存在于pb中
			name_str.wstr_ptr = prompt;
			name_str.wstr_len = SPEEDKEY_PROMPT_LEN;
			
			GenSpeedDialListItemInfo(i, &speedkey_entry, &name_str, name_abbr,&anim_path, &item_user_data);
			if(speedkey_entry.number_len > 0 || 0 != speedkey_entry.npi_ton)
			{
				if(speedkey_entry.is_sms)
				{
					string2_text_id = STR_ILIST_MESSAGE_EXT01; //STR_SPEED_DIAL_SMS_EXT01;
				}
				else
				{
					string2_text_id = STXT_SOFTKEY_CALL_MK;
				}
			}
			else
			{
				string2_text_id = STR_SPEED_DIAL_UNDEFINED_EXT01;
			}
			AppendItemToListBoxNew(
				MMISPEEDDIAL_LISTBOX_CTRL_ID, 
				i,
				&name_str, 
				string2_text_id,
				STXT_SOFTKEY_MENU_MK, 
				TXT_COMMON_COMMON_BACK,
				&anim_path,
				name_abbr,
				item_user_data,
				FALSE);
		} 
		
		if (MSG_GET_FOCUS == msg_id)
		{
			GUILIST_SetCurItemIndex(MMISPEEDDIAL_LISTBOX_CTRL_ID, cur_index);
		}
		else
		{
			GUILIST_SetCurItemIndex(MMISPEEDDIAL_LISTBOX_CTRL_ID, (MMIPB_HANDLE_T)s_cur_index-2);
		}
		GUILIST_SetListState(MMISPEEDDIAL_LISTBOX_CTRL_ID, GUILIST_STATE_SPLIT_LINE, FALSE);
		break;
	case MSG_CTL_LIST_NEED_ITEM_CONTENT:
		SpeedDialListUpdateListContentItem(win_id, MMISPEEDDIAL_LISTBOX_CTRL_ID,param);
		break;		
	case MSG_APP_WEB:
	case MSG_CTL_MIDSK:
		MMIAPIPB_OpenMainWin(
			MMIPB_ALL_CONTACT,
			MMIPB_LIST_RADIO, 
			MMIPB_LIST_FIELD_NUMBER,
			0,
			PNULL,
			CC_SpeadDailListSelectContactCallback
			);
		break;
	case MSG_CTL_OK:
	case MSG_APP_OK:
		OpenSpeedDialMenuWin();
		break;  
	case MMI_SELECT_CONTACT_CNF:
		// get the number from the PB
		if (MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)param) > 0)
		{
			MMIPB_BCD_NUMBER_T	bcd_number = {0};
			uint8				temp_num[MMISPEEDDIAL_NUMBER_MAX_LEN + 1]= {0};
			wchar 				tele_num[MMISPEEDDIAL_NUMBER_MAX_LEN + 1] = {0};             
			uint16                      number_len = 0;
			MMI_STRING_T         temp_str = {0};
			wchar name_abbr[3] = {0};

			
			if (MMIAPIPB_GetSelectNumber((MMIPB_HANDLE_T)param, &bcd_number,MMIPB_BCD_NUMBER_MAX_LEN, 0))
			{
				GUIANIM_FILE_INFO_T anim_path = {0};
				uint32 item_user_data = 0;
				BOOLEAN is_update = FALSE;
				wchar	anim_full_path_wstr[MMIFILE_FULL_PATH_MAX_LEN +1] = {0};
				anim_path.full_path_wstr_ptr = anim_full_path_wstr;

				number_len = MMIAPIPB_BCDNumberToString(&bcd_number, 
					(MMISPEEDDIAL_NUMBER_MAX_LEN + 2), 
					temp_num);
				cur_index = GUILIST_GetCurItemIndex( MMISPEEDDIAL_LISTBOX_CTRL_ID );
				if(s_speeddial_record[cur_index].number_len > 0 || 0 != s_speeddial_record[cur_index].npi_ton)
				{
				    is_update = TRUE;
				}
				//SaveSpeedDialItem(temp_num, number_len, (uint8)number_len);
				SaveSpeedDialItemNew(temp_num, number_len, (uint8)number_len, FALSE);
#if defined(MMI_GUI_STYLE_TYPICAL)
				MMIPUB_OpenAlertSuccessWin(TXT_SET_SUCCESS);
#endif
				
				MMI_STRNTOWSTR(tele_num, MMISPEEDDIAL_NUMBER_MAX_LEN + 1, 
					(uint8 *)temp_num, MMISPEEDDIAL_NUMBER_MAX_LEN + 1, 
					MMISPEEDDIAL_NUMBER_MAX_LEN + 1);   
				temp_str.wstr_len = number_len;
				temp_str.wstr_ptr = tele_num;
				GenSpeedDialListItemInfo(cur_index, 
					&s_speeddial_record[cur_index], 
					&temp_str, 
					name_abbr,&anim_path, &item_user_data);
				AppendItemToListBoxNew(
					MMISPEEDDIAL_LISTBOX_CTRL_ID, 
					cur_index,
					&temp_str, 
					STXT_SOFTKEY_CALL_MK,
					STXT_SOFTKEY_MENU_MK, 
					TXT_COMMON_COMMON_BACK,
					&anim_path,
					name_abbr,
					item_user_data,
					TRUE);

				if(is_update)
				{
					CC_SpeedDialDisplaybannerInfo(win_id,STR_SPEED_DIAL_UPDATE_DIAL_EXT01, cur_index);
				}
				else
				{
					CC_SpeedDialDisplaybannerInfo(win_id,STR_SPEED_DIAL_CONF_DIAL, cur_index);
				}
				
			}
			else
			{
			    CC_SpeedDialDisplaybannerInfo(win_id, STR_SPEED_DIAL_NO_NUMBER_EXT01, SPEEDKEY_MAX_NUM);
			}
		}
		else
		{
		    CC_SpeedDialDisplaybannerInfo(win_id, STR_SPEED_DIAL_NO_NUMBER_EXT01, SPEEDKEY_MAX_NUM);
		}
		break;
		
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
	case MSG_APP_GREEN:
	case MSG_APP_CALL2:
	case MSG_APP_CALL3:
	case MSG_APP_CALL4:    
		{
#ifdef MMI_MULTI_GREEN_KEY_SINGLE
			CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;            
#else
			CC_CALL_SIM_TYPE_E sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);
			MN_DUAL_SYS_E  dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_MAX);
#endif
			
			wchar number_ptr[SPEEDKEY_MAX_LEN + 1] = {0};
			uint16 number_len = 0;
			uint8 temp_string[SPEEDKEY_MAX_LEN + 1] = {0};
			uint16 list_index = GUILIST_GetCurItemIndex( MMISPEEDDIAL_LISTBOX_CTRL_ID );
			SCI_MEMSET(number_ptr, 0x00, ((SPEEDKEY_MAX_LEN + 1)*sizeof(wchar)));
			GetSelectedItem(list_index, number_ptr, SPEEDKEY_MAX_LEN, &number_len);
			if(0 == number_len)
			{
			    break;
			}
			MMI_WSTRNTOSTR((uint8 *)temp_string, SPEEDKEY_MAX_LEN + 1, number_ptr, number_len, number_len);
			
#ifdef MMI_MULTI_GREEN_KEY_SINGLE
			MMIAPICC_MakeCall(
				MN_DUAL_SYS_MAX,
				temp_string,
				number_len,
				PNULL,
				PNULL,
				sim_type,
				CC_CALL_NORMAL_CALL,
				PNULL
				);
#else            
			MMIAPICC_MakeCall(
				dual_sys,
				temp_string,
				number_len,
				PNULL,
				PNULL,
				sim_type,
				CC_CALL_NORMAL_CALL,
				PNULL
				);
#endif
		}
		break;
		
	case MSG_CLOSE_WINDOW:
#ifdef PHONE_NUMBER_VOICE
			s_hero_speedcall_win_id = 0;
	Trace_Log_Buf_Print("CC_HandleSpeedDialListWinMsg MSG_CLOSE_WINDOW");

#endif
		
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return result;
}

#endif

#ifdef ZDT_PRODUCT_BH

LOCAL MMI_RESULT_E SpeedDirectionsWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint8 buf[521] = {0};
      wchar wstr[521] = {0};
      char* str = PNULL;
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
   	GUI_FONT_T font = MMI_DEFAULT_BIG_FONT ;
	GUI_COLOR_T color = MMI_BLACK_COLOR;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:

   
  		 MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_SPEED_DIRECTIONS,&text);
		GUITEXT_SetFont(MMISPEEDDIAL_DIRECTIONS_CTRL_ID, &font, &color );
        GUITEXT_SetString(MMISPEEDDIAL_DIRECTIONS_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
        MMK_SetAtvCtrl(win_id, MMISPEEDDIAL_DIRECTIONS_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:     
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}



#endif

#ifdef PHONE_NUMBER_VOICE
PUBLIC unsigned int hero_SpeedCall_GetWinId(void)
{
	return s_hero_speedcall_win_id;
}

PUBLIC void hero_LoSpeedCall_ResetWinId(void)
{
	s_hero_speedcall_win_id = 0;
}
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//	Description : set item menu
//	Global resource dependence : 
//	Author:rong.gu
//	Note:
/*****************************************************************************/
LOCAL void SetItemMenuTitle(void)
{
    MMI_STRING_T name_str = {0};
    MN_RETURN_RESULT_E	ret_val = MN_RETURN_FAILURE;
    CC_SPEEDKEY_ENTRY_T	speedkey_entry = {0};
    wchar prompt[SPEEDKEY_PROMPT_LEN + 1] = {0};
    uint16 cur_index = 0;
    cur_index = GUILIST_GetCurItemIndex( MMISPEEDDIAL_LISTBOX_CTRL_ID );
     
    SCI_MEMSET(&speedkey_entry, 0, sizeof(speedkey_entry));
    // @baokun : 电话本里设置2快捷拨号后，在idle长按2，提示未设置号码
    MMINV_READ((MMI_CC_SPEEDKEY_1 + cur_index), &speedkey_entry, ret_val);

    if( MN_RETURN_SUCCESS != ret_val )
    {
        SCI_MEMSET(&speedkey_entry, 0, sizeof(speedkey_entry));
        speedkey_entry.number_len = 0;
        s_speeddial_record[cur_index] = speedkey_entry;     
        
        // @baokun : 电话本里设置2快捷拨号后，在idle长按2，提示未设置号码
        MMINV_WRITE((MMI_CC_SPEEDKEY_1 + cur_index), &speedkey_entry);
    }
    else
    {
        s_speeddial_record[cur_index]= speedkey_entry;
    }
    
    //合成list的字符串，同时判断是否存在于pb中
    name_str.wstr_ptr = prompt;
    name_str.wstr_len = SPEEDKEY_PROMPT_LEN;
    
    GenSpeedStr(cur_index, &speedkey_entry, &name_str, SPEEDKEY_PROMPT_LEN);/*lint !e506 */

#if defined (MAINLCD_SIZE_128X160)
    MMI_GetLabelTextByLang(STXT_SOFTKEY_MENU_MK, &name_str);
    GUIMENU_SetMenuTitle(&name_str,MMISPEEDDIAL_POPMENU_CTRL_ID);
#else
    GUIMENU_SetMenuTitle(&name_str, MMISPEEDDIAL_POPMENU_CTRL_ID);
#endif

}
#endif

#if 1
/*****************************************************************************/
//	Description : open the window for speeddial menu
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void OpenSpeedDialMenuWin(void)
{       
    //if( MMK_IsOpenWin( MMISPEEDDIAL_MENU_WIN_ID ))	//@samboo modify 20071001
    {
        MMK_CloseWin( MMISPEEDDIAL_MENU_WIN_ID );
    }
    
    //create the speeddial menu window
    if (IsSelectedListNULL())   //选中list是空的，创建edit菜单
    {
        MMK_CreateWin((uint32 *)MMISPEEDDIAL_MENUEDIT_WIN_TAB, PNULL);
    }
    else   //选中list不是空的，创建edit, del菜单
    {
        MMK_CreateWin((uint32 *)MMISPEEDDIAL_MENUEDITDEL_WIN_TAB, PNULL);
#ifndef MMI_GUI_STYLE_TYPICAL
        SetItemMenuTitle();
#endif
    }
    
    MMK_SetAtvCtrl( MMISPEEDDIAL_MENU_WIN_ID, MMISPEEDDIAL_POPMENU_CTRL_ID );
}
#else
/*****************************************************************************/
//	Description : open the window for speeddial menu
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void OpenSpeedDialMenuWin(void)
{
	uint16 list_index = 0;
	MMK_CloseWin( MMISPEEDDIAL_MENU_WIN_ID );
	list_index = GUILIST_GetCurItemIndex( MMISPEEDDIAL_LISTBOX_CTRL_ID );
	if (0 == s_speeddial_record[list_index].number_len)
	{
		//MMK_CreateWin((uint32 *)MMISPEEDDIAL_LIST_ITEM_EMPTY_MENU_WIN_TAB, PNULL);
	}
	else
	{
		if(s_speeddial_record[list_index].is_sms)
		{
			MMK_CreateWin((uint32 *)MMISPEEDDIAL_LIST_ITEM_SMS_MENU_WIN_TAB, PNULL);
		}
		else
		{
			MMK_CreateWin((uint32 *)MMISPEEDDIAL_LIST_ITEM_CALL_MENU_WIN_TAB, PNULL);
		}
	}
	
	MMK_SetAtvCtrl( MMISPEEDDIAL_MENU_WIN_ID, MMISPEEDDIAL_POPMENU_CTRL_ID );
}
#endif

/*****************************************************************************/
//	Description : check whether current selected in list is NULL
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSelectedListNULL(void)
{
    uint16 list_index = 0;
    wchar number_ptr[SPEEDKEY_MAX_LEN + 1] = {0};
    uint16 number_len = 0;
    
    list_index = GUILIST_GetCurItemIndex( MMISPEEDDIAL_LISTBOX_CTRL_ID );
    SCI_MEMSET(number_ptr, 0x00, ((SPEEDKEY_MAX_LEN + 1)*sizeof(wchar)));
    GetSelectedItem(list_index, number_ptr, SPEEDKEY_MAX_LEN, &number_len);
    if (0 == number_len)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : to handle alert window closed message
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlertClosedMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
    
    if (MSG_CLOSE_WINDOW == msg_id)
    {      
        MMK_CloseWin(MMISPEEDDIAL_MENU_WIN_ID);   
    }
    
    return recode;
}

/*****************************************************************************/
//	Description : the handle function of speeddial menu window
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleSpeedDialMenuWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32	list_index = 0;
    wchar number_ptr[SPEEDKEY_MAX_LEN + 1] = {0};
    uint16 number_len = 0;
    MMI_STRING_T temp_str = {0};
//     wchar temp_buffer[SPEEDKEY_PROMPT_LEN + 1] = {0};
    
    //SCI_TRACE_LOW:"mmi_speeddial.c: CC_HandleSpeedDialMenuWinMsg( %d, %d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_920_112_2_18_2_10_37_299,(uint8*)"dd",win_id, msg_id);
    
    switch(msg_id)
    {	   
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        list_index = GUILIST_GetCurItemIndex( MMISPEEDDIAL_LISTBOX_CTRL_ID );
        GUIMENU_GetId(MMISPEEDDIAL_POPMENU_CTRL_ID, &group_id, &menu_id);
        switch (menu_id)
        {
        case ID_SPEEDDIAL_EDIT:
            //set data for editbox
            SCI_MEMSET(number_ptr, 0x00, ((SPEEDKEY_MAX_LEN + 1)*sizeof(wchar)));
            GetSelectedItem(list_index, number_ptr, SPEEDKEY_MAX_LEN, &number_len);
            
            //get current pb number
            if (0 == number_len)
            {
                uint8 temp_string[SPEEDKEY_MAX_LEN + 1] = {0};
                number_len = (uint16)MMIAPICOM_GenDispNumber((MN_NUMBER_TYPE_E)((s_pb_speeddial_entry.npi_ton & 0xf0) >> 4),
                    s_pb_speeddial_entry.number_len,
                    s_pb_speeddial_entry.number,
                    temp_string,
                    SPEEDKEY_MAX_LEN        
                    );
	            MMI_STRNTOWSTR(number_ptr, SPEEDKEY_MAX_LEN + 1, (uint8 *)temp_string, SPEEDKEY_MAX_LEN + 1, SPEEDKEY_MAX_LEN + 1);	      
            }
            //open edit win
            OpenSpeedDialEditWin(number_ptr, number_len);
            MMK_CloseWin(win_id);
            break;
            
        case ID_SPEEDDIAL_DEL:
            //open query win
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,PNULL,CC_HandleSpeedDialDeleteQueryWinMsg);
            MMK_CloseWin(win_id);
            break;

        case ID_SPEEDDIAL_AUDIO_DIAL:
        case ID_SPEEDDIAL_VIDEO_DIAL:
        case ID_SPEEDDIAL_IPDIAL:
            {
                uint8 temp_string[SPEEDKEY_MAX_LEN + 1] = {0};
                uint32 call_type = 0;
                
                switch(menu_id) 
                {
                case ID_SPEEDDIAL_AUDIO_DIAL:
                    call_type = CC_CALL_NORMAL_CALL;
                	break;
#ifdef VT_SUPPORT
                case ID_SPEEDDIAL_VIDEO_DIAL:
                    call_type = CC_CALL_VIDEO_CALL;
                	break;
#endif
#ifdef MMI_IP_CALL_SUPPORT
                case ID_SPEEDDIAL_IPDIAL:
                    call_type = CC_CALL_IP_CALL;
                	break;
#endif
                default:
                    call_type = CC_CALL_NORMAL_CALL;
                    break;
                }

                SCI_MEMSET(number_ptr, 0x00, ((SPEEDKEY_MAX_LEN + 1)*sizeof(wchar)));
                GetSelectedItem(list_index, number_ptr, SPEEDKEY_MAX_LEN, &number_len);
                MMI_WSTRNTOSTR((uint8 *)temp_string, SPEEDKEY_MAX_LEN + 1, number_ptr, number_len, number_len);
                MMIAPICC_MakeCall(
                                                MN_DUAL_SYS_MAX,
                                                temp_string,
                                                number_len,
                                                PNULL,
                                                PNULL,
                                                CC_CALL_SIM_MAX,
                                                call_type,
                                                PNULL
                                                );   
                MMK_CloseWin(win_id);
            }
            break;
            
        case ID_SPEEDDIAL_VIEW:
            {
                uint16 i_richtext_index = 0;
                GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
                wchar item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
                MMI_STRING_T num_str = {0};
                wchar prompt[SPEEDKEY_PROMPT_LEN + 1] = {0};
                MMI_STRING_T name_str = {0};
                wchar name[CC_MAX_PBNAME_LEN + 1] = {0};
                uint16 index = 0;

                num_str.wstr_ptr = prompt;
                name_str.wstr_ptr = name;
                
                MMK_CreateWin( (uint32*)MMISPEEDDIAL_VIEW_WIN_TAB, (ADD_DATA)list_index );

                item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
                item_data.text_type = GUIRICHTEXT_TEXT_BUF; 
                
                index = GUILIST_GetCurItemIndex(MMISPEEDDIAL_LISTBOX_CTRL_ID);

                //name:
                MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_SPEEDDIAL_VIEW_NAME, &temp_str);
                item_data.text_data.buf.len = temp_str.wstr_len;
                item_data.text_data.buf.str_ptr = temp_str.wstr_ptr;
                GUIRICHTEXT_AddItem(MMISPEEDDIAL_VIEW_RICHTEXT_CTRL_ID, &item_data, &i_richtext_index); 

                GenerateViewNameString(&s_speeddial_record[index], &name_str, SPEEDKEY_PROMPT_LEN);   /*lint !e506 */
                item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &name_str, GUIRICHTEXT_TEXT_MAX_LEN);
                item_data.text_data.buf.str_ptr = item_text;

                GUIRICHTEXT_AddItem(MMISPEEDDIAL_VIEW_RICHTEXT_CTRL_ID, &item_data, &i_richtext_index); 

                //number:
                MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_PHONE_NUMBER, &temp_str);
                item_data.text_data.buf.len = temp_str.wstr_len;
                item_data.text_data.buf.str_ptr = temp_str.wstr_ptr;
                GUIRICHTEXT_AddItem(MMISPEEDDIAL_VIEW_RICHTEXT_CTRL_ID, &item_data, &i_richtext_index); 

                GenerateViewPhoneString(&s_speeddial_record[index], &num_str, SPEEDKEY_PROMPT_LEN);/*lint !e506 */
                SCI_MEMSET(item_text, 0x00, sizeof(item_text));
                item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &num_str, GUIRICHTEXT_TEXT_MAX_LEN);
                item_data.text_data.buf.str_ptr = item_text;

                GUIRICHTEXT_AddItem(MMISPEEDDIAL_VIEW_RICHTEXT_CTRL_ID, &item_data, &i_richtext_index); 
                MMK_CloseWin(win_id);
            }
            break;
            
        case ID_SPEEDDIAL_EDIT_CONTACT:
            /*MMIAPIPB_OpenNumberList(
                MMIPB_LIST_FOR_SMS, //IN: 列表是为哪个模块显示
                MMIPB_LIST_RADIO,   //IN: 号码列表的类型
                0,       //IN: 如果是多选时，此项限制了多选的最大个数
                PNULL      //IN: 打开列表时的位置情况，本模块调用时有用
                );*/

                MMIAPIPB_OpenMainWin(
                                 MMIPB_ALL_CONTACT,
                                 MMIPB_LIST_RADIO, 
                                 MMIPB_LIST_FIELD_NUMBER,
                                 0,
                                 PNULL,
                                 CC_SpeadDailListSelectContactCallback
                               );
            MMK_CloseWin(win_id);
            break;
            
        default:
            break;            
        }
        break;    
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        case MSG_APP_RED:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CLOSE_WINDOW:
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}

/*****************************************************************************/
//	Description : the handle function of speeddial menu window
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleSpeedDialListMenuWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
	MMI_MENU_GROUP_ID_T group_id = 0;
	MMI_MENU_ID_T menu_id = 0;
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	uint32	list_index = 0;
	wchar number_ptr[SPEEDKEY_MAX_LEN + 1] = {0};
	uint16 number_len = 0;
	uint8 temp_ptr[PHONEBOOK_MAX_DN_NUMBER_LEN*2 + 2] = {0};
	
	switch(msg_id)
	{	   
	case MSG_OPEN_WINDOW:
        if(TRUE == MMIAPISET_IsDisableShareViaOption())
        {
            GUIMENU_GetId(MMISPEEDDIAL_POPMENU_CTRL_ID, &group_id, &menu_id);
            GUIMENU_SetItemVisible(MMISPEEDDIAL_POPMENU_CTRL_ID, group_id , ID_SPEEDDIAL_ASSIGN_SMS, FALSE);
        }
		break;
		
	case MSG_APP_WEB:
	//case MSG_CTL_OK:
	//case MSG_APP_OK:
	case MSG_CTL_MIDSK:
		list_index = GUILIST_GetCurItemIndex( MMISPEEDDIAL_LISTBOX_CTRL_ID );
		GUIMENU_GetId(MMISPEEDDIAL_POPMENU_CTRL_ID, &group_id, &menu_id);
		switch (menu_id)
		{
		case ID_SPEEDDIAL_EDIT:
			MMIAPIPB_OpenMainWin(
				MMIPB_ALL_CONTACT,
				MMIPB_LIST_RADIO, 
				MMIPB_LIST_FIELD_NUMBER,
				0,
				PNULL,
				CC_SpeadDailListSelectContactCallback
				);
			MMK_CloseWin(win_id);
			break;
		case ID_SPEEDDIAL_ASSIGN_SMS:
			GetSelectedItem(list_index, number_ptr, SPEEDKEY_MAX_LEN, &number_len);
			MMI_WSTRNTOSTR((uint8 *)temp_ptr, PHONEBOOK_MAX_DN_NUMBER_LEN*2, number_ptr, number_len,number_len);
			SaveSpeedDialItemNew(temp_ptr, strlen(temp_ptr), strlen(temp_ptr),TRUE);
			MMK_CloseWin(win_id);
			CC_SpeedDialDisplaybannerInfo(win_id, STR_SPEED_DIAL_UPDATE_DIAL_EXT01, list_index);
			break;
		case ID_SPEEDDIAL_ASSIGN_CALL:
			SCI_MEMSET(number_ptr, 0x00, ((SPEEDKEY_MAX_LEN + 1)*sizeof(wchar)));
			GetSelectedItem(list_index, number_ptr, SPEEDKEY_MAX_LEN, &number_len);
			MMI_WSTRNTOSTR((uint8 *)temp_ptr, PHONEBOOK_MAX_DN_NUMBER_LEN*2, number_ptr, number_len,number_len);
			SaveSpeedDialItemNew(temp_ptr, strlen(temp_ptr), strlen(temp_ptr),FALSE);
			MMK_CloseWin(win_id);
			CC_SpeedDialDisplaybannerInfo(win_id, STR_SPEED_DIAL_UPDATE_DIAL_EXT01, list_index);
			break;
		case ID_SPEEDDIAL_DEL:
			#if 0
			//open query win
			MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,PNULL,CC_HandleSpeedDialDeleteQueryWinMsg);
			#else
			{
				//char num_str[4+1] = {0};
				//wchar num_wstr[4+1] = {0};
				//wchar result_str[100] = {0};
				MMI_STRING_T notification_str = {0};
				//MMI_STRING_T temp_str = {0};
				
				//MMI_GetLabelTextByLang(TEXT_SPEED_DIAL_ERASE_QUERY, &temp_str);
				//sprintf((char *)num_str,"%c%c%c", ' ', list_index + '2' + GetSpeedDialOffset(), '.');	
				//MMIAPICOM_StrToWstr(num_str, num_wstr); 
				//MMIAPICOM_Wstrcat(result_str,temp_str.wstr_ptr);
				//MMIAPICOM_Wstrcat(result_str,num_wstr);
				
				MMI_STRING_T    result_str = {0};
				wchar result_wchar[200] = {0};
				result_str.wstr_ptr = result_wchar;
				#ifdef ZDT_PRODUCT_Z685S
				MMIAPICOM_CustomGenerateNumReplaceStrByTextId(STXT_SPEED_DIAL_ERASE_QUERY, 
									L"%N", 
									list_index+1,
									200, 
									&result_str);

				#else

				MMIAPICOM_CustomGenerateNumReplaceStrByTextId(STXT_SPEED_DIAL_ERASE_QUERY, 
					L"%N", 
					list_index+2,
					200, 
					&result_str);
				#endif
				notification_str.wstr_ptr = result_str.wstr_ptr;
				notification_str.wstr_len = result_str.wstr_len;
				MMIPUB_OpenConfirmationDialogByTextPtr(
					PNULL,   //time period,default is 3s
					&notification_str,         //text 1
					PNULL,         //text 2
					IMAGE_NULL,           //alert image id
					PNULL,        //win id,default is MMIPUB_ALERT_WIN_ID
					PNULL,  //win priority,default is WIN_ONE_LEVEL
					MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,      //softkey style
					CC_HandleSpeedDialDeleteQueryWinMsg         //handle function                                         
					);
			}
			#endif
			MMK_CloseWin(win_id);
			break;
			
		default:
			break;            
		}
		break;    
		
		case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:
		case MSG_APP_RED:
			MMK_CloseWin(win_id);
			break;
			
		case MSG_CLOSE_WINDOW:
			break;
			
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return result;
}

/*****************************************************************************/
//	Description : the handle function of speeddial menu window
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleSpeedDialDeleteQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32	list_index = 0;
//     wchar number_ptr[SPEEDKEY_MAX_LEN + 1] = {0};
//     uint16 number_len = 0;
    MMI_STRING_T temp_str = {0};
    wchar temp_buffer[SPEEDKEY_PROMPT_LEN + 1] = {0};

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_OK:
  // case MSG_APP_OK:
        break;
        case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
 
        list_index = GUILIST_GetCurItemIndex(MMISPEEDDIAL_LISTBOX_CTRL_ID);
        //SaveSpeedDialItem((uint8 *)"", 0, 0);
        SaveSpeedDialItemNew((uint8 *)"", 0, 0, FALSE);
        temp_str.wstr_ptr = temp_buffer;
        temp_str.wstr_len = SPEEDKEY_PROMPT_LEN;
        GenSpeedStr(
            list_index,
            &s_speeddial_record[list_index],
            &temp_str,
            SPEEDKEY_PROMPT_LEN/*lint !e506 */
            );
#if 0
        AppendItemToListBox(
            MMISPEEDDIAL_LISTBOX_CTRL_ID,
            list_index, 
            temp_str.wstr_ptr,
            temp_str.wstr_len,
            STXT_OPTION, 
            STXT_EXIT,
            TRUE
            );
#else
        AppendItemToListBoxNew(
            MMISPEEDDIAL_LISTBOX_CTRL_ID, 
            list_index,
            &temp_str, 
            STR_SPEED_DIAL_UNDEFINED_EXT01,
            STXT_SOFTKEY_MENU_MK, 
            TXT_COMMON_COMMON_BACK,
            PNULL,
            PNULL,
            0,
            TRUE);
#endif
        
        MMK_CloseWin(win_id);
        CC_SpeedDialDisplaybannerInfo(win_id, STXT_SPEED_DIAL_KEY_ERASED, list_index);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

/*****************************************************************************/
//	Description : open the window for speeddial edit
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void OpenSpeedDialEditWin(
                                wchar* number_ptr,
                                uint16 number_len
                                )
{    
    //if( MMK_IsOpenWin( MMISPEEDDIAL_EDIT_WIN_ID ))	//@samboo modify 20071001
    {
        MMK_CloseWin( MMISPEEDDIAL_EDIT_WIN_ID );
    }
    
    //create the speeddial edit window
    MMK_CreatePubEditWin((uint32 *)MMISPEEDDIAL_EDIT_WIN_TAB, PNULL);
    
    //SCI_MEMSET(&title_str, 0, sizeof(MMI_STRING_T) );	
    //	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_INPUTNUM, &title_str);	
    if ( PNULL != number_ptr )
    {
        GUIEDIT_SetString(
            MMISPEEDDIAL_EDITBOX_CTRL_ID,
            number_ptr,
            number_len);
    }
    MMK_SetAtvCtrl( MMISPEEDDIAL_EDIT_WIN_ID, MMISPEEDDIAL_EDITBOX_CTRL_ID );
}
/*****************************************************************************/
//	Description : the handle function of speeddial edit window
//	Global resource dependence : 
//	Author:Michael.shi
//	Note:
/*****************************************************************************/
LOCAL void HandleSpeedDialEditGreenUpMsg(MN_DUAL_SYS_E dual_sys)
{    
    MMI_STRING_T   num_string = {0};
    
    GUIEDIT_GetString(MMISPEEDDIAL_EDITBOX_CTRL_ID, &num_string);
    
    if ((PNULL != num_string.wstr_ptr) && (0 != num_string.wstr_len))
    {
        uint8 temp_string[SPEEDKEY_MAX_LEN + 1] = {0};
        MMI_WSTRNTOSTR((uint8 *)temp_string, SPEEDKEY_MAX_LEN, num_string.wstr_ptr, num_string.wstr_len, num_string.wstr_len);

        MMIAPICC_MakeCall(
                                        dual_sys,
                                        temp_string,
                                        num_string.wstr_len,
                                        PNULL,
                                        PNULL,
                                        MMIAPICC_GetCallSimType(dual_sys),
                                        CC_CALL_NORMAL_CALL,
                                        PNULL
                                        );
    }
}

/*****************************************************************************/
//	Description : the handle function of speeddial edit window
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleSpeedDialEditWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint16	list_index = 0;
    MMI_STRING_T   num_string = {0};
    MMI_STRING_T temp_str = {0};
    wchar temp_buffer[SPEEDKEY_PROMPT_LEN + 1] = {0};
    
    switch(msg_id)
    {	
    case MSG_OPEN_WINDOW:
        //+CR229294
        #ifdef DPHONE_SUPPORT
        {
            GUI_RECT_T edit_rect = {0};

            edit_rect = MMITHEME_GetClientRect();
            edit_rect.top += MMI_STATUSBAR_HEIGHT  ;
            GUIEDIT_SetRect(MMISPEEDDIAL_EDITBOX_CTRL_ID, &edit_rect);
        }       
        #endif
        //-CR229294

        //set left softkey
        GUIEDIT_SetSoftkey(MMISPEEDDIAL_EDITBOX_CTRL_ID, 0, 1, STXT_MAIN_PB, TXT_COMMON_OK, PNULL);
        break;

#if 0
//#ifndef MMI_GUI_STYLE_TYPICAL
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        {
            GUIEDIT_GetString(MMISPEEDDIAL_EDITBOX_CTRL_ID, &num_string);	
        
            if (0 == num_string.wstr_len)
            {    
                MMIPUB_SetWinSoftkey(win_id, STXT_MAIN_PB, STXT_RETURN, TRUE);
            }
            else
            {
#ifdef MMI_PDA_SUPPORT
                MMIPUB_SetWinSoftkey(win_id, TXT_COMMON_OK, STXT_RETURN, TRUE);
#else
                MMIPUB_SetWinSoftkey(win_id, TXT_COMMON_OK, TXT_DELETE, TRUE);
#endif
            }
        }
        break;
#endif

    //+CR229294
    #ifdef DPHONE_SUPPORT
    case MSG_FULL_PAINT:
        {
            GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
            GUI_RECT_T rect = {0};

            rect = MMITHEME_GetClientRect();
            GUI_FillRect(&lcd_dev_info, rect, MMI_BACKGROUND_COLOR);
        }       
        break;
    #endif
    //-CR229294
    
#ifdef  MMI_MULTI_GREEN_KEY_SINGLE
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        HandleSpeedDialEditGreenUpMsg(MN_DUAL_SYS_MAX);
        break;
#else
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        HandleSpeedDialEditGreenUpMsg(MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_MAX));
    break;
#endif

#ifdef VT_SUPPORT
    case MSG_KEYLONG_GREEN:
        GUIEDIT_GetString(MMISPEEDDIAL_EDITBOX_CTRL_ID, &num_string);
        if ((PNULL != num_string.wstr_ptr) && (0 != num_string.wstr_len))
        {
            uint8 temp_string[SPEEDKEY_MAX_LEN + 1] = {0};
            MMI_WSTRNTOSTR((uint8 *)temp_string, SPEEDKEY_MAX_LEN,  num_string.wstr_ptr, num_string.wstr_len, num_string.wstr_len);

            MMIAPICC_MakeCall(
                                            MN_DUAL_SYS_1,
                                            temp_string,
                                            num_string.wstr_len,
                                            PNULL,
                                            PNULL,
                                            CC_MAJOR_SIM_CALL,
                                            CC_CALL_VIDEO_CALL,
                                            PNULL
                                            );
        }
        break;
#endif        

    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        GUIEDIT_GetString(MMISPEEDDIAL_EDITBOX_CTRL_ID, &num_string);
        if ((PNULL != num_string.wstr_ptr) && (0 != num_string.wstr_len))
        {
            if(MMIAPICOM_IsValidNumberStringEx(&num_string))//@fen.xie MS00178937
            {
                uint8 temp_ptr[SPEEDKEY_PROMPT_LEN + 1] = {0};
                MMI_WSTRNTOSTR((uint8 *)temp_ptr, SPEEDKEY_PROMPT_LEN, num_string.wstr_ptr, num_string.wstr_len,num_string.wstr_len);
                SaveSpeedDialItem(
                     temp_ptr,
                    num_string.wstr_len,     //max len
                    (uint8)num_string.wstr_len
                    );

#if defined(MMI_GUI_STYLE_TYPICAL)
                MMIPUB_OpenAlertSuccessWin(TXT_SET_SUCCESS);
#endif
                
                MMK_CloseWin(MMISPEEDDIAL_EDIT_WIN_ID);
                MMK_CloseWin(MMISPEEDDIAL_MENU_WIN_ID);
                list_index = GUILIST_GetCurItemIndex( MMISPEEDDIAL_LISTBOX_CTRL_ID );
                temp_str.wstr_ptr = temp_buffer;
                temp_str.wstr_len = SPEEDKEY_MAX_LEN;
                GenSpeedStr(
                    list_index,
                    &s_speeddial_record[list_index],
                    &temp_str,
                    SPEEDKEY_PROMPT_LEN/*lint !e506 */
                    );
                /*
                AppendItemToListBox(
                    MMISPEEDDIAL_LISTBOX_CTRL_ID,
                    list_index, 
                    temp_str.wstr_ptr,
                    temp_str.wstr_len,
                    STXT_OPTION, 
                    STXT_EXIT,
                    TRUE
                    );
                */

        AppendItemToListBoxNew(
            MMISPEEDDIAL_LISTBOX_CTRL_ID, 
            list_index,
            &temp_str, 
            STXT_SOFTKEY_CALL_MK,
            STXT_SOFTKEY_MENU_MK, 
            TXT_COMMON_COMMON_BACK,
            PNULL,
            PNULL,
            0,
            TRUE);
				
            }
            else//@fen.xie MS00178937:无效号码,不允许保存
            {
                MMIPUB_OpenAlertWarningWin(STR_INVALID_NUM_EXT01);
            }
        }
        else    //调用pb接口
        {
           /* MMIAPIPB_OpenNumberList(
                MMIPB_LIST_FOR_SMS, //IN: 列表是为哪个模块显示
                MMIPB_LIST_RADIO,   //IN: 号码列表的类型
                0,       //IN: 如果是多选时，此项限制了多选的最大个数
                PNULL      //IN: 打开列表时的位置情况，本模块调用时有用
                );*/
            MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_RADIO, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_NUMBER,
                        0,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        CC_SpeadDailEditSelectContactCallback
                        );
        }
        break;  
        
    case MMI_SELECT_CONTACT_CNF:
        // get the number from the PB
        if (MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)param) > 0)
        {
            MMIPB_BCD_NUMBER_T	bcd_number = {0};
            uint8				tele_len = 0;
            uint8				temp_num[MMISPEEDDIAL_NUMBER_MAX_LEN + 1]= {0};//discuss?
            wchar 				tele_num[MMISPEEDDIAL_NUMBER_MAX_LEN + 1] = {0};
            
            if (MMIAPIPB_GetSelectNumber((MMIPB_HANDLE_T)param, &bcd_number,MMIPB_BCD_NUMBER_MAX_LEN,  0))//if (MMIAPIPB_GetDetailInSelectedList(PNULL, &bcd_number, PNULL, 0, PNULL, PNULL))
            {
                tele_len = MMIAPIPB_BCDNumberToString(&bcd_number, 
                                                     (MMISPEEDDIAL_NUMBER_MAX_LEN + 2), 
                                                     temp_num);
//                 tele_len = MMIAPICOM_GenDispNumber( 
//                     MMIPB_GetNumberTypeFromUint8(bcd_number.npi_ton), 
//                     (uint8)MIN(bcd_number.number_len, MMISPEEDDIAL_NUMBER_MAX_LEN), 
//                     bcd_number.number,  
//                    temp_num,
//                     (MMISPEEDDIAL_NUMBER_MAX_LEN + 2)
//                     );

                MMI_STRNTOWSTR(tele_num, MMISPEEDDIAL_NUMBER_MAX_LEN + 1, (uint8 *)temp_num, MMISPEEDDIAL_NUMBER_MAX_LEN + 1, MMISPEEDDIAL_NUMBER_MAX_LEN + 1);
                
                GUIEDIT_SetString(MMISPEEDDIAL_EDITBOX_CTRL_ID, tele_num, tele_len);
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
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//	Description : 得到选中项号码，为editbox准备好数据
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void GetSelectedItem(
                           uint16   list_index,
                           wchar*   number_ptr,
                           uint16   max_len,
                           uint16*  number_len_ptr
                           )
{
    uint8 temp_str[SPEEDKEY_MAX_LEN + 1] = {0};
    
    if(PNULL == number_ptr || PNULL == number_len_ptr)
    {
        //SCI_TRACE_LOW:"GetSelectedItem PNULL == number_ptr || PNULL == number_len_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1451_112_2_18_2_10_38_300,(uint8*)"");
        return;
    }
    *number_len_ptr = (uint16)MMIAPICOM_GenDispNumber((MN_NUMBER_TYPE_E)
        ((s_speeddial_record[list_index].npi_ton & 0xf0) >> 4),
        s_speeddial_record[list_index].number_len,
        s_speeddial_record[list_index].number,
        temp_str,
        (uint8)max_len        
        );
	MMI_STRNTOWSTR(number_ptr, max_len + 1, (uint8 *)temp_str, max_len + 1, max_len + 1);
}

/*****************************************************************************/
//	Description : to save the  speed key item after edit
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void SaveSpeedDialItem(
                             uint8* number_ptr,
                             uint16 max_len,
                             uint8 number_len
                             )
{
    uint16 list_index = GUILIST_GetCurItemIndex( MMISPEEDDIAL_LISTBOX_CTRL_ID );
    MMI_PARTY_NUMBER_T party_num = {0};/*lint !e64*/
    
    SCI_MEMSET(&party_num, 0, sizeof(MMI_PARTY_NUMBER_T));
    
    if(PNULL == number_ptr )
    {
        //SCI_TRACE_LOW:"SaveSpeedDialItem PNULL == number_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1481_112_2_18_2_10_38_301,(uint8*)"");
        return;
    }
    
    party_num.number_type = MN_NUM_TYPE_UNKNOW;
    
    MMIAPICOM_GenPartyNumber(
        number_ptr,
        number_len,
        &party_num
        );
    s_speeddial_record[list_index].npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
    
    MMI_MEMCPY(
        (void*)s_speeddial_record[list_index].number,
        sizeof(s_speeddial_record[list_index].number),
        (void*)party_num.bcd_num,
        sizeof(party_num.bcd_num),
        party_num.num_len
        );
    
    s_speeddial_record[list_index].number_len = party_num.num_len;

    MMINV_WRITE((MMI_CC_SPEEDKEY_1 + GetSpeedDialOffset()  + list_index), &s_speeddial_record[list_index]);
    
}

/*****************************************************************************/
//	Description : to save the  speed key item after edit
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void SaveSpeedDialItemNew(
                             uint8* number_ptr,
                             uint16 max_len,
                             uint8 number_len,
                             BOOLEAN is_sms
                             )
{
    uint16 list_index = GUILIST_GetCurItemIndex( MMISPEEDDIAL_LISTBOX_CTRL_ID );
    MMI_PARTY_NUMBER_T party_num = {0};/*lint !e64*/
    
    SCI_MEMSET(&party_num, 0, sizeof(MMI_PARTY_NUMBER_T));
    
    if(PNULL == number_ptr )
    {
        //SCI_TRACE_LOW:"SaveSpeedDialItem PNULL == number_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1481_112_2_18_2_10_38_301,(uint8*)"");
        return;
    }
    
    party_num.number_type = MN_NUM_TYPE_UNKNOW;
    
    MMIAPICOM_GenPartyNumber(
        number_ptr,
        number_len,
        &party_num
        );
    s_speeddial_record[list_index].npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
    
    MMI_MEMCPY(
        (void*)s_speeddial_record[list_index].number,
        sizeof(s_speeddial_record[list_index].number),
        (void*)party_num.bcd_num,
        sizeof(party_num.bcd_num),
        party_num.num_len
        );
    
    s_speeddial_record[list_index].number_len = party_num.num_len;
    s_speeddial_record[list_index].is_sms = is_sms;

    MMINV_WRITE((MMI_CC_SPEEDKEY_1 + GetSpeedDialOffset()  + list_index), &s_speeddial_record[list_index]);
    
}

/*****************************************************************************/
//	Description : generate name string of view window
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void GenerateViewPhoneString(
                                   CC_SPEEDKEY_ENTRY_T  *speedkey_entry_ptr,    //in
                                   MMI_STRING_T         *phone_str_ptr,          //out
                                   uint16               max_name_str_len        //in
                                   )
{
/*
uint8    	        colon = ':';
uint8    	        merge_array[SPEEDKEY_PROMPT_LEN] = {0};
//  uint8    	        name_array[SPEEDKEY_PROMPT_LEN] = {0};
uint8               number_ptr[SPEEDKEY_PROMPT_LEN] = {0};
uint16	            list_index = 0;
uint16              number_len = 0;

  MMI_STRING_T	    colon_str = {0};
  MMI_STRING_T	    merge_str = {0};
  MMI_STRING_T	    name_str = {0};
  MMI_STRING_T	    number_str = {0};
  
    colon_str.str_ptr = &colon;
    merge_str.str_ptr = merge_array;
    //  name_str.str_ptr = name_array;
    
      colon_str.is_ucs2 = FALSE;
      colon_str.length = 1;
      
        list_index = GUILIST_GetCurItemIndex(MMISPEEDDIAL_LISTBOX_CTRL_ID);
        
          SCI_MEMSET(number_ptr, 0x00, SPEEDKEY_PROMPT_LEN);
          GetSelectedItem(list_index, number_ptr, SPEEDKEY_PROMPT_LEN, &number_len);
          
            number_str.is_ucs2 = FALSE;
            number_str.str_ptr = number_ptr;
            number_str.length = number_len;
            
              MMIAPICOM_MergeTwoString( &colon_str, &number_str,&merge_str, SPEEDKEY_PROMPT_LEN);
              
                MMI_GetLabelTextByLang(TXT_COMMON_NUMBER, &name_str);
                
                  //    GUILIST_GetContentOfCurItem(MMIPB_SELECT_NUM_TYPE_LISTBOX_CTRL_ID,
                  //								  &name_str.is_ucs2,
                  //								  &(uint8)name_str.length,
                  //								  name_str.str_ptr
                  //								  );
                  
                    MMIAPICOM_MergeTwoString(&name_str, &merge_str,phone_str_ptr, SPEEDKEY_PROMPT_LEN);
    */
    uint8 temp_str[SPEEDKEY_MAX_LEN + 1] = {0};
    wchar temp_wstr[SPEEDKEY_MAX_LEN + 1] = {0};
    MMI_STRING_T num_str = {0};
    
    num_str.wstr_len = (uint16)MMIAPICOM_GenDispNumber((MN_NUMBER_TYPE_E)
        ((speedkey_entry_ptr->npi_ton & 0xf0) >> 4),
        speedkey_entry_ptr->number_len,                
        speedkey_entry_ptr->number,
        temp_str,
        SPEEDKEY_MAX_LEN
        );
    MMI_STRNTOWSTR(temp_wstr, SPEEDKEY_MAX_LEN + 1, (uint8 *)temp_str, SPEEDKEY_MAX_LEN + 1,SPEEDKEY_MAX_LEN + 1);
    num_str.wstr_ptr = temp_wstr;

    phone_str_ptr->wstr_len = MIN(num_str.wstr_len, max_name_str_len);
    MMI_WSTRNCPY(phone_str_ptr->wstr_ptr, max_name_str_len, num_str.wstr_ptr, num_str.wstr_len, phone_str_ptr->wstr_len);
}



/*****************************************************************************/
//	Description : generate name string of view window
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void GenerateViewNameString(
                                  CC_SPEEDKEY_ENTRY_T  *speedkey_entry_ptr,    //in
                                  MMI_STRING_T         *name_str_ptr,          //out
                                  uint16               max_name_str_len        //in
                                  )
{
    MMI_STRING_T temp_str = {0};
    wchar temp[CC_MAX_PBNAME_LEN + 1] = {0};
    
    temp_str.wstr_ptr = temp;

    if (speedkey_entry_ptr->number_len > 0
        || 0 != speedkey_entry_ptr->npi_ton)     //非空记录
    {
        BOOLEAN is_find = FALSE;
        MMIPB_BCD_NUMBER_T pb_number = {0};
        
        SCI_MEMSET(pb_number.number, 0xff, MMIPB_BCD_NUMBER_MAX_LEN);        
        
        pb_number.npi_ton = speedkey_entry_ptr->npi_ton;
        pb_number.number_len = speedkey_entry_ptr->number_len;
        MMI_MEMCPY(
            pb_number.number,
            pb_number.number_len,
            speedkey_entry_ptr->number,
            sizeof(speedkey_entry_ptr->number),
            speedkey_entry_ptr->number_len
            );
        
        // 获得号码所对应的姓名
        is_find = MMIAPIPB_GetNameByNumber(&pb_number, &temp_str, max_name_str_len, FALSE);

        
        // Generate to the string number
        if (!is_find || 0 == temp_str.wstr_len)   //在pb中查找不存在或名字为空
        {
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_NO_NAME, &temp_str);
        }
    }
    else
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_BLANK, &temp_str);
    }

    name_str_ptr->wstr_len = MIN(temp_str.wstr_len, max_name_str_len);
    MMI_WSTRNCPY(name_str_ptr->wstr_ptr, max_name_str_len, temp_str.wstr_ptr, temp_str.wstr_len, name_str_ptr->wstr_len);
}

/*****************************************************************************/
//	Description : 产生类似“3(xxxxx)”的list item字串
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void GenSpeedStr(
                       uint16               index,                  //in
                       CC_SPEEDKEY_ENTRY_T  *speedkey_entry_ptr,    //in
                       MMI_STRING_T         *name_str_ptr,          //out
                       uint16               max_name_str_len        //in
                       )
{
    MMI_STRING_T dest_temp_str = {0};
    MMI_STRING_T left_str = {0};
    MMI_STRING_T right_str = {0};
    MMI_STRING_T disp_str = {0};
    
    if(PNULL == speedkey_entry_ptr ||PNULL == name_str_ptr ||MMIPB_BCD_NUMBER_MAX_LEN < speedkey_entry_ptr->number_len)
    {
        //SCI_TRACE_LOW:"GenSpeedStr speedkey_entry_ptrm, name_str_ptr,speedkey_entry_ptr->number_len maybe null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1648_112_2_18_2_10_39_302,(uint8*)"");
        return;
    }
   
    //分配空间
    dest_temp_str.wstr_ptr = SCI_ALLOC_APP(( SPEEDKEY_PROMPT_LEN + 1) * sizeof(wchar) );
    if (PNULL == dest_temp_str.wstr_ptr)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1660_112_2_18_2_10_39_303,(uint8*)"");
        return;
    }
    dest_temp_str.wstr_len = SPEEDKEY_PROMPT_LEN;
    
    left_str.wstr_ptr = SCI_ALLOC_APP(4 * sizeof(wchar));
    if (PNULL == left_str.wstr_ptr)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1660_112_2_18_2_10_39_303,(uint8*)"");
        return;
    }
    right_str.wstr_ptr = SCI_ALLOC_APP(2 * sizeof(wchar));
    if (PNULL == right_str.wstr_ptr)
    {
        //SCI_TRACE_LOW:"ccapp GenSpeedStr mem alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1660_112_2_18_2_10_39_303,(uint8*)"");
        return;
    }    
    SCI_MEMSET(dest_temp_str.wstr_ptr, 0, (SPEEDKEY_PROMPT_LEN + 1)*sizeof(wchar));
    
    //左右两边字串
    // @baokun : 电话本里设置2快捷拨号后，在idle长按2，提示未设置号码
    left_str.wstr_ptr[0] = index + '1' + GetSpeedDialOffset();

    left_str.wstr_ptr[1] = '.';
    left_str.wstr_ptr[2] = ' ';
    left_str.wstr_len = 3;
    
    //right_str.wstr_ptr[0] = ')';
    right_str.wstr_len = 0;
    
    if (speedkey_entry_ptr->number_len > 0
        || 0 != speedkey_entry_ptr->npi_ton)     //非空记录
    {
        BOOLEAN is_find = FALSE;
        MMIPB_BCD_NUMBER_T pb_number = {0};
        
        disp_str.wstr_ptr = SCI_ALLOC_APP( (SPEEDKEY_PROMPT_LEN + 1) * sizeof(wchar) );
        if(PNULL == disp_str.wstr_ptr)
        {
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1660_112_2_18_2_10_39_303,(uint8*)"");
            return;
        }
        SCI_MEMSET(pb_number.number, 0xff, MMIPB_BCD_NUMBER_MAX_LEN);        
        //
        pb_number.npi_ton = speedkey_entry_ptr->npi_ton;
        pb_number.number_len = speedkey_entry_ptr->number_len;
        MMI_MEMCPY(
            pb_number.number,
            pb_number.number_len,
            speedkey_entry_ptr->number,
            sizeof(speedkey_entry_ptr->number),
            speedkey_entry_ptr->number_len
            );
        //
        is_find = MMIAPIPB_GetNameByNumber(&pb_number, &disp_str, SPEEDKEY_PROMPT_LEN, FALSE);/*lint !e506 */
//			MMIAPIPB_FindAlphaAndGroupByNumber(
//            &disp_str,
//            PNULL,
//            PNULL,
//            &pb_number,
//            PNULL,
//            FALSE
//            );
        //
        if (!is_find || 0 == disp_str.wstr_len)   //在pb中查找不存在或名字为空
        {

            uint8 temp_str[SPEEDKEY_MAX_LEN + 1] = {0};
            disp_str.wstr_len = (uint16)MMIAPICOM_GenDispNumber((MN_NUMBER_TYPE_E)
                ((speedkey_entry_ptr->npi_ton & 0xf0) >> 4),
                speedkey_entry_ptr->number_len,                
                speedkey_entry_ptr->number,
                temp_str,
                SPEEDKEY_MAX_LEN
                );
            MMI_STRNTOWSTR(disp_str.wstr_ptr, SPEEDKEY_MAX_LEN + 1, (uint8 *)temp_str, SPEEDKEY_MAX_LEN + 1,SPEEDKEY_MAX_LEN + 1);           
        }
        else
        {
        }
        MMIAPICOM_MergeTwoString(
            &left_str,
            &disp_str,
            &dest_temp_str,
            SPEEDKEY_PROMPT_LEN/*lint !e506 */
            );        
        SCI_FREE(disp_str.wstr_ptr);
    }
    else
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)STXT_BUTTON_ADD_CONTACT_EXT01, &disp_str);
        MMIAPICOM_MergeTwoString(
            &left_str,
            &disp_str,
            &dest_temp_str,
            SPEEDKEY_PROMPT_LEN/*lint !e506 */
            );
    }
    
    
    MMIAPICOM_MergeTwoString(
        &dest_temp_str,
        &right_str,
        name_str_ptr,
        SPEEDKEY_PROMPT_LEN/*lint !e506 */
        );
    
    //释放
    SCI_FREE(dest_temp_str.wstr_ptr);
    SCI_FREE(left_str.wstr_ptr);
    SCI_FREE(right_str.wstr_ptr);
}

/*****************************************************************************/
//	Description : 产生类似“3(xxxxx)”的list item字串
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void GenSpeedDialListItemInfo(
                       uint16               index,                  //in
                       CC_SPEEDKEY_ENTRY_T  *speedkey_entry_ptr,    //in
                       MMI_STRING_T         *name_str_ptr,          //out
                       wchar               *name_abbr,        //out
                       GUIANIM_FILE_INFO_T* pb_anim_path_ptr, //out
                       uint32 *item_user_data //out
                       )
{
	MMI_STRING_T dest_temp_str = {0};
	MMI_STRING_T left_str = {0};
	MMI_STRING_T right_str = {0};
	MMI_STRING_T disp_str = {0};
	uint16 entry_id = 0;
	uint16 storage_id = 0;
	
	if(PNULL == speedkey_entry_ptr 
		||PNULL == name_str_ptr 
		||MMIPB_BCD_NUMBER_MAX_LEN < speedkey_entry_ptr->number_len
		||PNULL == name_abbr
		||PNULL == pb_anim_path_ptr
		||PNULL == item_user_data
		)
	{
		//SCI_TRACE_LOW:"GenSpeedStr speedkey_entry_ptrm, name_str_ptr,speedkey_entry_ptr->number_len maybe null"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1648_112_2_18_2_10_39_302,(uint8*)"");
		return;
	}
	
	//分配空间
	dest_temp_str.wstr_ptr = SCI_ALLOC_APP(( SPEEDKEY_PROMPT_LEN + 1) * sizeof(wchar) );
    if (PNULL == dest_temp_str.wstr_ptr)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1660_112_2_18_2_10_39_303,(uint8*)"");
        return;
    }

	dest_temp_str.wstr_len = SPEEDKEY_PROMPT_LEN;
	
	left_str.wstr_ptr = SCI_ALLOC_APP(4 * sizeof(wchar));
    if (PNULL == left_str.wstr_ptr)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1660_112_2_18_2_10_39_303,(uint8*)"");
        return;
    }

	right_str.wstr_ptr = SCI_ALLOC_APP(2 * sizeof(wchar));
	if (PNULL == right_str.wstr_ptr)
	{
		//SCI_TRACE_LOW:"ccapp GenSpeedStr mem alloc failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1660_112_2_18_2_10_39_303,(uint8*)"");
		return;
	}    
	SCI_MEMSET(dest_temp_str.wstr_ptr, 0, (SPEEDKEY_PROMPT_LEN + 1)*sizeof(wchar));
	
	//左右两边字串
	// @baokun : 电话本里设置2快捷拨号后，在idle长按2，提示未设置号码
	left_str.wstr_ptr[0] = index + '1' + GetSpeedDialOffset();
	
	left_str.wstr_ptr[1] = '.';
	left_str.wstr_ptr[2] = ' ';
	left_str.wstr_len = 3;
	
	//right_str.wstr_ptr[0] = ')';
	right_str.wstr_len = 0;
	
	if (speedkey_entry_ptr->number_len > 0
		|| 0 != speedkey_entry_ptr->npi_ton)     //非空记录
	{
		BOOLEAN is_find = FALSE;
		MMIPB_BCD_NUMBER_T pb_number = {0};
		MMI_STRING_T name = {0};
		MMI_STRING_T abbr_name = {0};

		SCI_MEMSET(pb_number.number, 0xff, MMIPB_BCD_NUMBER_MAX_LEN);
		disp_str.wstr_ptr = SCI_ALLOC_APP( (SPEEDKEY_PROMPT_LEN + 1) * sizeof(wchar) );
        if (PNULL == disp_str.wstr_ptr)
        {
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1660_112_2_18_2_10_39_303,(uint8*)"");
            return;
        }
		SCI_MEMSET(disp_str.wstr_ptr, 0x00, (SPEEDKEY_PROMPT_LEN + 1) * sizeof(wchar) );        
		name.wstr_ptr = SCI_ALLOC_APP( (MMIPB_MAX_STRING_LEN + 1) * sizeof(wchar) );
        if (PNULL == name.wstr_ptr)
        {
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1660_112_2_18_2_10_39_303,(uint8*)"");
            return;
        }
		SCI_MEMSET(name.wstr_ptr, 0x00, (MMIPB_MAX_STRING_LEN + 1) * sizeof(wchar) );        
		abbr_name.wstr_ptr = SCI_ALLOC_APP( (MMIPB_MAX_STRING_LEN + 1) * sizeof(wchar) );
        if (PNULL == abbr_name.wstr_ptr)
        {
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1660_112_2_18_2_10_39_303,(uint8*)"");
            return;
        }
		SCI_MEMSET(abbr_name.wstr_ptr, 0x00, (MMIPB_MAX_STRING_LEN + 1) * sizeof(wchar) );        
		//
		pb_number.npi_ton = speedkey_entry_ptr->npi_ton;
		pb_number.number_len = speedkey_entry_ptr->number_len;
		MMI_MEMCPY(
			pb_number.number,
			pb_number.number_len,
			speedkey_entry_ptr->number,
			sizeof(speedkey_entry_ptr->number),
			speedkey_entry_ptr->number_len
			);
		is_find = MMIAPIPB_GetNameAvatarByNumber(&pb_number, &name, &abbr_name, pb_anim_path_ptr, &entry_id,&storage_id,  MMIPB_MAX_STRING_LEN, FALSE);
		MMIAPICOM_Wstrcpy(disp_str.wstr_ptr, name.wstr_ptr);
		disp_str.wstr_len = MMIAPICOM_Wstrlen(disp_str.wstr_ptr);
		if (!is_find || 0 == disp_str.wstr_len)   //在pb中查找不存在或名字为空
		{
			uint8 temp_str[SPEEDKEY_MAX_LEN + 1] = {0};
			disp_str.wstr_len = (uint16)MMIAPICOM_GenDispNumber((MN_NUMBER_TYPE_E)
				((speedkey_entry_ptr->npi_ton & 0xf0) >> 4),
				speedkey_entry_ptr->number_len,                
				speedkey_entry_ptr->number,
				temp_str,
				SPEEDKEY_MAX_LEN
				);
			MMI_STRNTOWSTR(disp_str.wstr_ptr, SPEEDKEY_MAX_LEN + 1, (uint8 *)temp_str, SPEEDKEY_MAX_LEN + 1,SPEEDKEY_MAX_LEN + 1);           
		}
		
		if(is_find)
		{
			*item_user_data = storage_id<<16 | entry_id;
			if( 0 == pb_anim_path_ptr->full_path_wstr_len)
			{
			    MMIAPICOM_Wstrncpy(name_abbr, abbr_name.wstr_ptr, MIN(3, abbr_name.wstr_len));
			}
		}
		MMIAPICOM_MergeTwoString(
			&left_str,
			&disp_str,
			&dest_temp_str,
			SPEEDKEY_PROMPT_LEN/*lint !e506 */
			);        
		SCI_FREE(disp_str.wstr_ptr);
		SCI_FREE(name.wstr_ptr);
		SCI_FREE(abbr_name.wstr_ptr);
	}
	else
	{
		MMI_GetLabelTextByLang((MMI_TEXT_ID_T)STXT_BUTTON_ADD_CONTACT_EXT01, &disp_str);
		MMIAPICOM_MergeTwoString(
			&left_str,
			&disp_str,
			&dest_temp_str,
			SPEEDKEY_PROMPT_LEN/*lint !e506 */
			);
	}
	
	
	MMIAPICOM_MergeTwoString(
		&dest_temp_str,
		&right_str,
		name_str_ptr,
		SPEEDKEY_PROMPT_LEN/*lint !e506 */
		);
	
	//释放
	SCI_FREE(dest_temp_str.wstr_ptr);
	SCI_FREE(left_str.wstr_ptr);
	SCI_FREE(right_str.wstr_ptr);
}

/*****************************************************************************/
//	Description : open the window for speeddial
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void SpeedDial(
                               MN_DUAL_SYS_E dual_sys,
                               uint16        key_num
                               )
{
    CC_SPEEDKEY_ENTRY_T	speedkey_entry = {0};
    MN_RETURN_RESULT_E	ret_val = MN_RETURN_FAILURE;
    uint8 tele_num[SPEEDKEY_MAX_LEN] = {0};
    uint8 tele_len = 0;

    if (key_num < 1 || key_num > 9)
    {
        //SCI_TRACE_LOW:"ccapp SpeedDial error key_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1769_112_2_18_2_10_39_304,(uint8*)"d", key_num);
        return;
    }
    
    //从nv中读取快捷拨号
    SCI_MEMSET(&speedkey_entry, 0, sizeof( CC_SPEEDKEY_ENTRY_T ));
    
    MMINV_READ((MMI_CC_SPEEDKEY_1 + key_num - 1), &speedkey_entry, ret_val);  //if press key_2, then key_num==2, NV index is MMI_CC_SPEEDKEY_2.
    
    //SCI_TRACE_LOW:"[SPEEDDIAL] MMIAPICC_SpeedDial ret_val %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1778_112_2_18_2_10_39_305,(uint8*)"d",ret_val);
    
    if( MN_RETURN_SUCCESS != ret_val )
    {
        SCI_MEMSET(&speedkey_entry, 0, sizeof( CC_SPEEDKEY_ENTRY_T ));
        speedkey_entry.number_len = 0;
        
        MMINV_WRITE((MMI_CC_SPEEDKEY_1 + key_num - 1), &speedkey_entry);

    }
    //检查快捷拨号是否设置
    if (0 == speedkey_entry.number_len)    //未设置快捷拨号
    {
        MMIPUB_OpenAlertWarningWin((TXT_SPEEDDIAL_KEY1_NOT_SET + key_num - 1));
        
    }
    else    //已设置快捷拨号
    {
        tele_len = (uint16)MMIAPICOM_GenDispNumber((MN_NUMBER_TYPE_E)
            ((speedkey_entry.npi_ton & 0xf0) >> 4),
            speedkey_entry.number_len,
            speedkey_entry.number,
            tele_num,
            SPEEDKEY_MAX_LEN
            );
        
        MMIAPICC_MakeCall(
                                        dual_sys,
                                        tele_num,
                                        tele_len,
                                        PNULL,
                                        PNULL,
                                        MMIAPICC_GetCallSimType(dual_sys),
                                        CC_CALL_NORMAL_CALL,
                                        PNULL
                                        );
    }
}

/*****************************************************************************/
//	Description : open the window for speeddial
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_SpeedDial(
                               MN_DUAL_SYS_E dual_sys,
                               uint16        key_num
                               )
{
    CC_SPEEDKEY_ENTRY_T	speedkey_entry = {0};
    MN_RETURN_RESULT_E	ret_val = MN_RETURN_FAILURE;

    if (key_num < 1 || key_num > 9)
    {
        //SCI_TRACE_LOW:"ccapp SpeedDial error key_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1831_112_2_18_2_10_39_306,(uint8*)"d", key_num);
        return;
    }
    
    //从nv中读取快捷拨号
    SCI_MEMSET(&speedkey_entry, 0, sizeof( CC_SPEEDKEY_ENTRY_T ));
    
    MMINV_READ((MMI_CC_SPEEDKEY_1 + key_num - 1), &speedkey_entry, ret_val);  //if press key_2, then key_num==2, NV index is MMI_CC_SPEEDKEY_2.
    
    //SCI_TRACE_LOW:"[SPEEDDIAL] MMIAPICC_SpeedDial ret_val %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1840_112_2_18_2_10_39_307,(uint8*)"d",ret_val);
    
    if( MN_RETURN_SUCCESS != ret_val )
    {
        SCI_MEMSET(&speedkey_entry, 0, sizeof( CC_SPEEDKEY_ENTRY_T ));
        speedkey_entry.number_len = 0;
        
        MMINV_WRITE((MMI_CC_SPEEDKEY_1 + key_num - 1), &speedkey_entry);

    }
    //检查快捷拨号是否设置
    if (0 == speedkey_entry.number_len)    //未设置快捷拨号
    {
    	MMIAPICC_SpeedDialConfirmationDialogByTextPtr(key_num);
        //MMIPUB_OpenAlertWarningWin((TXT_SPEEDDIAL_KEY2_NOT_SET + key_num - 2));
#ifndef MMI_GUI_STYLE_TYPICAL
        MMIPUB_SetWinTitleTextId( MMIPUB_ALERT_WIN_ID, TXT_DIAL, FALSE );
#endif
    }
    else
    {
        SpeedDial(dual_sys, key_num);
    }
}

/*****************************************************************************/
//  Description : MMIAPICC_HandleAddSpeedDialWinMsg
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIAPICC_HandleAddSpeedDialWinMsg(
													   MMI_WIN_ID_T     win_id, 
													   MMI_MESSAGE_ID_E msg_id, 
													   DPARAM           param 
													   )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
	
    switch(msg_id)
    {
	case MSG_APP_OK:
	case MSG_APP_WEB:
	MMK_CloseWin(win_id);
	MMK_CloseWin( MMISPEEDDIAL_LIST_WIN_ID );
    MMK_CreateWin((uint32 *)MMISPEEDDIAL_LIST_WIN_TAB, (ADD_DATA)(s_cur_index));
    MMK_SetAtvCtrl( MMISPEEDDIAL_LIST_WIN_ID, MMISPEEDDIAL_LISTBOX_CTRL_ID );
		break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
		MMK_CloseWin(win_id);
		MMK_CloseWin(win_id);
		break;
    default:
		recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}


/*****************************************************************************/
//	Description : tips for first time to make a speed dial function
//	Global resource dependence : 
//	Note:
/*****************************************************************************/


PUBLIC void MMIAPICC_SpeedDialConfirmationDialogByTextPtr(uint16 key_num){
        wchar     wstr_temp[]  = {L"%N"};
        MMI_STRING_T     old_string = {0};
        MMI_STRING_T     new_string = {0};
        uint16        length = 0;
        wchar  speed_dail_digital_ptr[5] = {0};
        wchar text_temp[100] = {0};

        MMI_GetLabelTextByLang(STR_SPEED_DIAL_CONF_SET_EXT01, &old_string);
        sprintf((wchar *)speed_dail_digital_ptr, "%d", key_num);
        MMIAPICOM_Wstrncpy(text_temp, old_string.wstr_ptr,old_string.wstr_len);
        MMIAPICOM_WstrReplace(text_temp, sizeof(text_temp), wstr_temp, speed_dail_digital_ptr);
        new_string.wstr_ptr = (wchar *)text_temp;
        new_string.wstr_len = (uint16)MMIAPICOM_Wstrlen(text_temp);


        MMIPUB_OpenConfirmationDialogByTextPtr ( PNULL,
        &new_string,
        PNULL,
        PNULL,
        PNULL,
        PNULL,
        MMIPUB_SOFTKEY_OKCANCEL,
        MMIAPICC_HandleAddSpeedDialWinMsg);
	} 



/*****************************************************************************/
//	Description : to add the string to listbox
//	Global resource dependence : 
//	Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void  AppendItemToListBox(
                                MMI_CTRL_ID_T ctrl_id,
                                uint16		  index,
                                wchar* string_ptr,
                                uint16		  string_len,
                                MMI_TEXT_ID_T left_softkey_id,
                                MMI_TEXT_ID_T right_softkey_id,
                                BOOLEAN       is_update
                                )
{
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    
    if(PNULL == string_ptr)
    {
        //SCI_TRACE_LOW:"AppendItemToListBox string_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1883_112_2_18_2_10_39_308,(uint8*)"");
        return;
    }
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = string_len;
    
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = string_ptr;
    
//     MMI_WSTRNCPY( item_data.item_content[0].item_data.text_buffer.wstr,
//         item_data.item_content[0].item_data.text_buffer.wstr_len,
//         string_ptr,
//         item_data.item_content[0].item_data.text_buffer.wstr_len,
//         item_data.item_content[0].item_data.text_buffer.wstr_len);   
  
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = right_softkey_id;
    
    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, index );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}

/*****************************************************************************/
//	Description : to add the string to listbox
//	Global resource dependence : 
//	Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void  AppendItemToListBoxNew(
                                MMI_CTRL_ID_T ctrl_id,
                                uint16		  index,
                                MMI_STRING_T* string_ptr,
                                MMI_TEXT_ID_T string2_text_id,
                                MMI_TEXT_ID_T left_softkey_id,
                                MMI_TEXT_ID_T right_softkey_id,
                                GUIANIM_FILE_INFO_T* anim_path,
                                wchar* name_abbr,
                                uint32 item_user_data,
                                BOOLEAN       is_update
                                )
{
	GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
	
	if(PNULL == string_ptr)
	{
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1883_112_2_18_2_10_39_308,(uint8*)"");
		return;
	}
	
	item_t.item_style    = GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_MASK_MS;
	item_t.item_data_ptr = &item_data;
	item_t.user_data = item_user_data;
	
	if(PNULL != anim_path && anim_path->full_path_wstr_len > 0)
	{
		item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
		item_data.item_content[0].item_data.anim_path_ptr = anim_path;
	}
	else
	{
		item_data.item_content[0].item_data_type    = GUIITEM_DATA_IMAGE_ID;
		if(PNULL != name_abbr && MMIAPICOM_Wstrlen(name_abbr) > 0)
		{
			item_data.item_content[0].item_data.image_id = IMAGE_LISTITEM_AVATAR_INITIALS;
		}
		else
		{
			item_data.item_content[0].item_data.image_id = IMAGE_CONTACTS_UNDEFINED_CONTACT;
		}
	}
	
	item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
	item_data.item_content[1].item_data.text_buffer.wstr_len = string_ptr->wstr_len;    
	item_data.item_content[1].item_data.text_buffer.wstr_ptr = string_ptr->wstr_ptr;
	
	item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_ID;
	item_data.item_content[2].item_data.text_id = string2_text_id;    
	item_data.item_content[2].is_opacity = TRUE;
	GUILIST_SetItemContentOpacityFontColor(&item_data,2,255*0.6,255*0.6);
	if(PNULL != name_abbr)
	{
	    item_data.item_content[3].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
	    item_data.item_content[3].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(name_abbr);    
	    item_data.item_content[3].item_data.text_buffer.wstr_ptr = name_abbr;
	}
	
	if(s_speeddial_record[index].number_len > 0 || 0 != s_speeddial_record[index].npi_ton)
	{
		item_data.softkey_id[1] = STR_SPEED_DIAL_LIST_EDIT;
		item_data.softkey_id[0] = left_softkey_id;
	}
	else
	{
		item_data.softkey_id[1] = STR_SOFTKEY_ADD_MK;
		item_data.softkey_id[0] = left_softkey_id;//TXT_NULL;
	}

	item_data.softkey_id[2] = right_softkey_id;
	
	if( is_update )
	{
		GUILIST_ReplaceItem( ctrl_id, &item_t, index );
	}
	else
	{
		GUILIST_AppendItem( ctrl_id, &item_t );
	}
}

/*****************************************************************************/
//	Description : Get Speed Dial Offset
//	Global resource dependence : 
//	Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL uint8 GetSpeedDialOffset(void)
{
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    uint32 i = MN_DUAL_SYS_1;
    uint32 j = 0;
    uint8 speed_dial_offset = 0;

//    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        MMIPLMN_KEYBOARD_SHORTCUTS_T *keyboard_shortcuts_ptr = (MMIPLMN_KEYBOARD_SHORTCUTS_T *)MMIAPIPLMN_GetLocalKeyboardShortcuts(i);

        if (PNULL != keyboard_shortcuts_ptr)
        {
            MMI_MESSAGE_ID_E speed_dial_msg = 0;
                
            for (j = 0; j < MMIPLMN_KEYBOARD_SHORTCUTS_MAX; j++)
            {
                if (NULL != keyboard_shortcuts_ptr->msg_id[j] && speed_dial_msg < keyboard_shortcuts_ptr->msg_id[j])
                {
                    speed_dial_msg = keyboard_shortcuts_ptr->msg_id[j];
                }
            }

            if (speed_dial_msg >= MSG_KEYLONG_2 && speed_dial_msg <= MSG_KEYLONG_9)
            {
                speed_dial_offset = speed_dial_msg - MSG_KEYLONG_1;
            }
            else
            {
                speed_dial_offset = 0;
            }

//            break;
        }
        else
        {
            speed_dial_offset = 0;
        }

        SCI_TRACE_LOW("GetSpeedDialOffset speed_dial_offset=%d", speed_dial_offset);

        return speed_dial_offset;
    }

//    return speed_dial_offset;
#else
    return 0;
#endif
}
#endif//#if defined(MMI_SPEEDDIAL_SUPPORT)

#if defined(MMI_WIDGET_SPEED_DIAL)
PUBLIC void OpenSpeedDialListWinSetCurItem(uint8 *cur_item)
{
    MMK_CloseWin( MMISPEEDDIAL_LIST_WIN_ID );
    MMK_CreateWin((uint32 *)MMISPEEDDIAL_LIST_WIN_TAB, (ADD_DATA)(cur_item));
    MMK_SetAtvCtrl( MMISPEEDDIAL_LIST_WIN_ID, MMISPEEDDIAL_LISTBOX_CTRL_ID );
}
#endif

/*****************************************************************************/
//	Description : 
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void MMIAPICC_SpeedDialEnableWrite(BOOLEAN value)
{
	MMINV_WRITE(MMI_CC_SPEEDKEY_ENABLE, &value);
}

/*****************************************************************************/
//	Description :
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_SpeedDialEnableRead(void)
{
	BOOLEAN value = TRUE;
	MN_RETURN_RESULT_E	ret_val = MN_RETURN_FAILURE;
	
	MMINV_READ(MMI_CC_SPEEDKEY_ENABLE, &value, ret_val);
	if( MN_RETURN_SUCCESS != ret_val )
	{
		NVITEM_ERROR_E nv_reslut = NVERR_NONE;
		uint16 nv_len = 0;
		uint8 ndt_config_value = 0;
		
		nv_len = NVITEM_GetLength(TELE_NV_ENABLE_SPEED_DIAL);
		nv_reslut = EFS_NvitemRead(TELE_NV_ENABLE_SPEED_DIAL,(uint16)sizeof(ndt_config_value),(uint8*)&ndt_config_value);
		if (NVERR_NONE != nv_reslut)
		{
			value = TRUE;				
		}
		else
		{
			value = ndt_config_value;
		}
		
		MMIAPICC_SpeedDialEnableWrite(value);
	}
	return value;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL void AppendListItemForSpeedDial(MMI_CTRL_ID_T ctrl_id,
    uint32              item_id,
    uint32*             item_index,      
    GUIITEM_STYLE_E     item_style,
    MMI_TEXT_ID_T       text1_id,
    MMI_TEXT_ID_T       text2_id,
    BOOLEAN update
    )
{    
	GUILIST_ITEM_T      item_t = {0};
	GUILIST_ITEM_DATA_T item_data = {0};
	uint16               content_index = 0;
	
	item_t.item_style    = item_style;
	item_t.item_data_ptr = &item_data;
	item_data.softkey_id[LEFT_BUTTON] = TXT_NULL;
	item_data.softkey_id[MIDDLE_BUTTON] = STXT_SOFTKEY_SELECT_MK;
	item_data.softkey_id[RIGHT_BUTTON] = TXT_COMMON_COMMON_BACK;
	item_t.user_data     = item_id;
	
	if(item_style == GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT)
	{
		content_index = 0;
		item_data.item_content[content_index].item_data_type =GUIITEM_DATA_TEXT_ID;
		item_data.item_content[content_index].item_data.text_id = text1_id;
		item_data.softkey_id[MIDDLE_BUTTON] = STXT_SOFTKEY_CHANGE;
	}
	else if(item_style == GUIITEM_STYLE_ONE_LINE_TEXT_MS)
	{
		content_index = 0;
		item_data.item_content[content_index].item_data_type =GUIITEM_DATA_TEXT_ID;
		item_data.item_content[content_index].item_data.text_id = text1_id;
	}
	if(update)
	{
		GUILIST_ReplaceItem(ctrl_id, &item_t, *item_index);
	}
	else
	{
		GUILIST_AppendItem( ctrl_id, &item_t );
		(*item_index)++;
	}
	
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL void SetSpeedDialSettingsList(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id)
{     
	uint32 item_index = 0;

#ifdef ZDT_PRODUCT_BH
	GUILIST_SetMaxItem(
			ctrl_id, 
			3,
			FALSE
			);

#else
	GUILIST_SetMaxItem(
		ctrl_id, 
		2,
		FALSE
		);
#endif


	AppendListItemForSpeedDial(
		ctrl_id,
		0,
		&item_index,
		GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT,
		STXT_HEADER_SPEED_DIALLING,
		TXT_NULL,
		FALSE
	);
	GUILIST_SetToggleState(ctrl_id, 0, MMIAPICC_SpeedDialEnableRead());
	AppendListItemForSpeedDial(
		ctrl_id,
		1,
		&item_index,
		GUIITEM_STYLE_ONE_LINE_TEXT_MS,
		STR_SPEED_DIAL_LIST_EDIT,
		TXT_NULL,
		FALSE
	);

#ifdef ZDT_PRODUCT_BH
	AppendListItemForSpeedDial(
		ctrl_id,
		2,
		&item_index,
		GUIITEM_STYLE_ONE_LINE_TEXT_MS,
		TXT_DIRECTIONS,
		TXT_NULL,
		FALSE
	);

#endif
	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSpeedDialSettingsWinMsg(
				     MMI_WIN_ID_T      win_id, 
 				     MMI_MESSAGE_ID_E  msg_id, 
 				     DPARAM            param
 				     )
{
	MMI_RESULT_E        result  = MMI_RESULT_TRUE;
	MMI_HANDLE_T list_ctr_id = NULL;
	//MMI_STRING_T title_str = {0};
	uint16 cur_index = 0;
	uint32 item_index = 0;
	
	list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISPEEDDIAL_SETTINGS_LIST_CTRL_ID);
	
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		SetSpeedDialSettingsList(win_id, list_ctr_id);
		MMK_SetActiveCtrl(list_ctr_id, FALSE);
		GUILIST_SetCurItemIndex(list_ctr_id,0);
		if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STXT_HEADER_SPEED_DIALLING))
		{
			GUIWIN_UpdateStb();
		}
		break;
	case MSG_GET_FOCUS:
		if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STXT_HEADER_SPEED_DIALLING))
		{
			GUIWIN_UpdateStb();
		}
		break;
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
	case MSG_CTRL_TOGGLE_CHANGED:
		if(MMIAPICC_SpeedDialEnableRead())
		{
			MMIAPICC_SpeedDialEnableWrite(FALSE);
		}
		else
		{
			MMIAPICC_SpeedDialEnableWrite(TRUE);
		}
		break;
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
		cur_index =  GUILIST_GetCurItemIndex(list_ctr_id);
		if(0 == cur_index)
		{
		}
		else if(1 == cur_index)
		{
			 MMIAPICC_OpenSetSpeedDial(PNULL, 0, 0);
		}
#ifdef ZDT_PRODUCT_BH
		else if(2 == cur_index)
		{

		MMK_CreateWin((uint32 *)MMISPEEDDIAL_DIRECTIONS_WIN_TAB, PNULL);

		// MMIAPICC_OpenSetSpeedDial(PNULL, 0, 0);
		}

#endif

		break;
	case MSG_CLOSE_WINDOW:
		break;
	default:
		result = FALSE;
		break;
	}
	
	return (result);
}
LOCAL void ResetSpeedDialItemNew(
                             uint8* number_ptr,
                             uint16 max_len,
                             uint8 number_len,
                             BOOLEAN is_sms
                             )
{
   
    MMI_PARTY_NUMBER_T party_num = {0};/*lint !e64*/
    int i = 0;
    SCI_MEMSET(&party_num, 0, sizeof(MMI_PARTY_NUMBER_T));
    for(i = 0; i < SPEEDKEY_MAX_NUM;i++)
    {
	    if(PNULL == number_ptr )
	    {
	        //SCI_TRACE_LOW:"SaveSpeedDialItem PNULL == number_ptr"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_SPEEDDIAL_1481_112_2_18_2_10_38_301,(uint8*)"");
	        return;
	    }
	    
	    party_num.number_type = MN_NUM_TYPE_UNKNOW;
	    
	    MMIAPICOM_GenPartyNumber(
	        number_ptr,
	        number_len,
	        &party_num
	        );
		
	    s_speeddial_record[i].npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
	    
	    MMI_MEMCPY(
	        (void*)s_speeddial_record[i].number,
	        sizeof(s_speeddial_record[i].number),
	        (void*)party_num.bcd_num,
	        sizeof(party_num.bcd_num),
	        party_num.num_len
	        );
	    
	    s_speeddial_record[i].number_len = party_num.num_len;
	    s_speeddial_record[i].is_sms = is_sms;

	    MMINV_WRITE((MMI_CC_SPEEDKEY_1 +  i), &s_speeddial_record[i]);
    }
    
}

PUBLIC void  MMIAPICC_ResetSpeedSettings(void)
{
	MMIAPICC_SpeedDialEnableWrite(TRUE);
	 ResetSpeedDialItemNew((uint8 *)"", 0, 0, FALSE);

}


/*****************************************************************************/
//	Description : 
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAPICC_OpenSpeedDialSettingsWin(void)
{
    MMK_CreateWin((uint32 *)MMISPEEDDIAL_SETTINGS_WIN_TAB, PNULL);
}

/*****************************************************************************/
//	Description : 
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_SpeedDialIsEnable()
{
	return MMIAPICC_SpeedDialEnableRead();
}


/*Edit by script, ignore 5 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
