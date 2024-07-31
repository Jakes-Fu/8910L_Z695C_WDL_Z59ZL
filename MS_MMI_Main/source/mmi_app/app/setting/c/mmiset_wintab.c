/*****************************************************************************
** File Name:      mmiset_wintab.c                                           *
** Author:                                                                   *
** Date:           12/2003                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2004       Jassmine.Meng     Creat
******************************************************************************/

#define _MMISET_WINTAB_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmi_menutable.h"
#include "guilistbox.h"
#include "guiiconlist.h"
#include "mmi_nv.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "tb_dal.h"
#include "guilabel.h"
#include "guimsgbox.h"
#include "mmiset_wintab.h"
#include "mmialarm_export.h"
#include "mmk_timer.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmiphone_export.h"
#include "mmi_appmsg.h"

#include "mmiidle_subwintab.h"
#include "mmi_textfun.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiset_display.h"
#include "mmiset_func.h"
#include "mmiset_set_ring.h"
#include "mmiset_text.h"
#include "mmiset_image.h"
#include "mmiset_id.h"
#include "mmiset_menutable.h"
#include "mmk_tp.h"

#include "guibutton.h"
#include "mmiset_nv.h"
#include "mmipub.h"
#include "tp_export.h"

//#include "mmiset.h"
#include "mmiset_internal.h"
#include "mmiim.h"
#include "mn_api.h"
#include "guirichtext.h"
#ifdef MMI_DUALMODE_ENABLE
#ifndef WIN32
#include "tb_hal.h"
#endif
#endif
#include "mn_type.h"//@$discuss,此类头文件的增加势狂需要宏控制？增加对vt蹬�刂疲黾尤硭?
#include "mn_api.h"
#include "mmi_mainmenu_export.h"
//#include "mmipb_wintab.h"
#include "mmipb_export.h"
#include "guitext.h"
//#include "freq_cfg.h"
#include "mmk_msg.h"
#include "mmi_resource.h"
#include "guires.h"
#include "mmiset_shortcut.h"
#include "guiedit.h"
#include "mmisms_export.h"
#include "guidropdownlist.h"
#include "mn_api.h"
#include "mmicom_time.h"

#include "mmicc_text.h"
#ifdef MMI_OCR_ENABLE
#include "mmiocr_export.h"
#endif
#include "mmipb_text.h"
#include "guiform.h"
//#include "mmiim_im_switching.h"

#include "guictrl_api.h"
#ifdef QQ_SUPPORT
#include "mmiqq_export.h"
#endif
#include "guisetlist.h"
#include "guiownerdraw.h"
#ifdef MULTI_SIM_GSM_CTA_SUPPORT
#include "sci_api.h"
#endif
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif


#include "guistatusbar.h"
#include"mmicc_export.h"

#include "mmiocr_export.h"
#include "mmiidle_statusbar.h"
#include "mmicom_statusarea.h"
//#include "mmiset_mainmenu.h"
#include "mmiset_mainmenu_internal.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMI_BUTTON_MARGIN (20)
//#define MULTI_FILE_THEME_SUPPORT //支持放置在SD卡的主题文件
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

LOCAL GUI_LCD_DEV_INFO s_set_button_layer_info = {0,UILAYER_NULL_HANDLE};
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern uint8 g_ringtone_max;
extern uint8 g_alert_max;
extern uint8 g_alert_default_id;
extern uint8 g_ringtone_default_id;
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/





/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : to handle the ring setting main menu window message
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRingMainMenuWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );



/*****************************************************************************/
//  Description : to handle select call ring
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectCallRingWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );

/*****************************************************************************/
//  Description : to handle select fixed call ring
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectFixedCallRingWindow(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    );

/*****************************************************************************/
//  Description : to handle select message ring
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectMsgRingWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );



/*****************************************************************************/
//  Description : to handle select fixed msg ring
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectFixedMsgRingWindow(
                                                   MMI_WIN_ID_T    win_id, 
                                                   MMI_MESSAGE_ID_E   msg_id, 
                                                   DPARAM             param
                                                   );
/*****************************************************************************/
//  Description : 添加消息铃声radio list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AppendMsgRingItem(
                             MN_DUAL_SYS_E  dual_sys,
                             MMI_CTRL_ID_T   ctrl_id
                             );



/*****************************************************************************/
//  Description : to open ring setting menu
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL void EnterRingSettingMenu( void);



/*****************************************************************************/
//  Description :铃声设置中所有label的设置
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void EnvSetRingParam(MMI_WIN_ID_T    win_id);


/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
#ifndef MMI_ENVSET_MINI_SUPPORT
// the window for ring main menu
WINDOW_TABLE( MMISET_RING_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleRingMainMenuWindow ),    
    WIN_ID( MMISET_RING_MENU_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_ENV_RING_FORM_CTRL_ID),
    END_WIN
};
#endif
// the window for ring main menu
WINDOW_TABLE( MMISET_ICON_RING_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleRingMainMenuWindow ),    
    WIN_ID( MMISET_RING_MENU_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
#ifdef DPHONE_SUPPORT 
    CREATE_MENU_CTRL(MENU_ICON_SET_RING_SELECT, MMISET_RING_MENU_CTRL_ID),
#else    
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_ENV_RING_FORM_CTRL_ID),
#endif 
    END_WIN
};

// the window for select call ring
WINDOW_TABLE( MMISET_RING_SELECT_CALL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSelectCallRingWindow ),    
        WIN_ID( MMISET_RING_SELECT_CALL_WIN_ID ),
        WIN_TITLE( TXT_SET_RING_SELECT_CALL ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_RING_SELECT_CALL_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        END_WIN
};

// the window for select fixed call ring
/*
WINDOW_TABLE( MMISET_RING_SELECT_FIXED_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSelectFixedCallRingWindow ),    
        WIN_ID( MMISET_RING_SELECT_FIXED_WIN_ID ),
        WIN_TITLE( TXT_PB_RING_PHONE ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_RING_SELECT_FIXED_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),

        END_WIN
};
*/
//dong.chunguang modify ring list by ui spec
WINDOW_TABLE( MMISET_RING_SELECT_FIXED_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSelectFixedCallRingWindow ),
    WIN_ID( MMISET_RING_SELECT_FIXED_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_RING_SELECT_FIXED_CTRL_ID),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),

    END_WIN
};
//dong.chunguang modify msg ring list by ui spec
WINDOW_TABLE( MMISET_MSG_RING_SELECT_FIXED_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSelectFixedMsgRingWindow ),    
    WIN_ID( MMISET_MSG_RING_SELECT_FIXED_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    END_WIN
};
/*
//the window for select fixed msg ring
WINDOW_TABLE( MMISET_MSG_RING_SELECT_FIXED_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSelectFixedMsgRingWindow ),    
        WIN_ID( MMISET_MSG_RING_SELECT_FIXED_WIN_ID ),
        WIN_TITLE( TXT_RING_FIXED ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID),
//#ifdef MMI_PDA_SUPPORT//bug71574
//        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
//#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
//#endif
        
        END_WIN
};
*/
// the window for select message ring
WINDOW_TABLE( MMISET_RING_SELECT_MSG_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSelectMsgRingWindow ),    
        WIN_ID( MMISET_RING_SELECT_MSG_WIN_ID ),
        WIN_TITLE( TXT_SET_RING_SELECT_MSG ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_RING_SELECT_MSG_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        END_WIN
};



/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : to handle the ring setting main menu window message
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
#ifdef DPHONE_SUPPORT 
LOCAL MMI_RESULT_E  HandleRingMainMenuWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_MENU_GROUP_ID_T          group_id   =   0;
    MMI_MENU_ID_T                menu_id    =   0;
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    MN_DUAL_SYS_E               dual_sys    =   MN_DUAL_SYS_MAX;
#endif
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMISET_RING_MENU_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(MMISET_RING_MENU_CTRL_ID,&group_id,&menu_id);
        
#if defined(MMI_MULTI_SIM_SYS_SINGLE)
        switch (menu_id)
        {
        case ID_SET_RING_SELECT_CALL:
            MMIAPISET_OpenFixCallRingWin(MN_DUAL_SYS_1, 0,0);
            break;
            
        case ID_SET_RING_SELECT_MSG:
            MMK_CreatePubListWin((uint32*)MMISET_MSG_RING_SELECT_FIXED_WIN_TAB,PNULL);
            break;
        default:
            //SCI_PASSERT(0,("HandleRingMainMenuWindow menu_id = %d",menu_id));
            break;
        }
#else //MMI_MULTI_SIM_SYS_SINGLE
        if(menu_id >= ID_SET_RING_SIM1_SELECT_CALL && menu_id < ID_SET_RING_SIM_MAX_SELECT_CALL)
        {
            dual_sys = MN_DUAL_SYS_1 + (menu_id - ID_SET_RING_SIM1_SELECT_CALL);
            MMK_CreatePubListWin((uint32*)MMISET_RING_SELECT_CALL_WIN_TAB, (ADD_DATA)dual_sys);
        }
        else if(menu_id >= ID_SET_RING_SIM1_SELECT_MSG && menu_id < ID_SET_RING_SIM_MAX_SELECT_MSG)
        {
            dual_sys = MN_DUAL_SYS_1 + (menu_id - ID_SET_RING_SIM1_SELECT_MSG);
            MMK_CreatePubListWin((uint32*)MMISET_RING_SELECT_MSG_WIN_TAB, (ADD_DATA)dual_sys);
        }    
#endif //MMI_MULTI_SIM_SYS_SINGLE

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
#else
LOCAL MMI_RESULT_E  HandleRingMainMenuWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E                 recode     =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                ctrl_id    =   0;
    uint16                       offset     =   0;
    //MN_DUAL_SYS_E               dual_sys    =   MN_DUAL_SYS_MAX;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        EnvSetRingParam(win_id);
#ifndef MMI_GUI_STYLE_TYPICAL
        GUIFORM_SetType(MMISET_ENV_RING_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif
        MMK_SetAtvCtrl(win_id, MMISET_ENV_CALL_RING_LABEL_CTRL_ID+1);
        GUIWIN_SetTitleTextId(win_id,TXT_SET_RING_SET,FALSE);
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_FINISH, FALSE);
		GUIFORM_SetCircularHandleUpDown(MMISET_ENV_RING_FORM_CTRL_ID, TRUE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint8  i  =  0;
            MMIENVSET_ADD_DATA_T *add_data_ptr = (MMIENVSET_ADD_DATA_T*)SCI_ALLOC_APP(sizeof(MMIENVSET_ADD_DATA_T));
            #ifdef TOUCH_PANEL_SUPPORT
            if(MSG_CTL_PENOK == msg_id)
            {
                ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
            }
            else
            #endif
            {
                ctrl_id = MMK_GetActiveCtrlId(win_id);
            }
            offset = ctrl_id - MMISET_ENV_CALL_RING_LABEL_CTRL_ID;
            add_data_ptr->s_dual_sys= MN_DUAL_SYS_1 + offset/4;
            add_data_ptr->ctrl_id  = ctrl_id;
            for(i=0; i<MMI_DUAL_SYS_MAX; i++)
            {
                if((4*i+1) == offset)
                {
                    MMK_CreatePubListWin((uint32*)MMISET_RING_SELECT_CALL_WIN_TAB, (ADD_DATA)add_data_ptr);
                }
                else if((4*i+3) == offset)
                {
                    MMK_CreatePubListWin((uint32*)MMISET_RING_SELECT_MSG_WIN_TAB, (ADD_DATA)add_data_ptr);
                }
            }
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
#endif
/*****************************************************************************/
//  Description : to handle select call ring
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectCallRingWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               )
{
    uint16                      cur_selection    = 0;
    uint32                      music_types      = 0;
    MMI_RESULT_E                result           = MMI_RESULT_TRUE;
    MMISET_CALL_RING_T          ring_info        = {MMISET_CALL_RING_FIXED, 0, 0};
    MMISET_CALL_MORE_RING_T     select_ring_info = {0};
    MULTIM_SELECT_RETURN_T      *win_param       = NULL;
    MMIENVSET_ADD_DATA_T*       add_data_ptr     =  (MMIENVSET_ADD_DATA_T*) MMK_GetWinAddDataPtr(win_id); 
    MMI_CTRL_ID_T               ctrl_id          = MMISET_RING_SELECT_CALL_CTRL_ID;
    MMIFMM_SELECT_INFO_T        select_info = {MMIFMM_MUSIC_ALL,0,0,0,PNULL,0,-1};
    MMI_STRING_T                ring                                   = {0};
    wchar                       file_name[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16                      *ring_id_ptr = PNULL;
    uint32 text_id[MMISET_RING_FIXED_DEFAUT_NUM] =
    {TXT_COMMON_RING_RING1,TXT_COMMON_RING_RING2,TXT_COMMON_RING_RING3,TXT_COMMON_RING_RING4
#if 0
def MMI_LOW_MEMORY_RING
    ,TXT_COMMON_RING_RING3,TXT_COMMON_RING_RING4,
    TXT_COMMON_RING_RING5,TXT_COMMON_RING_RING6,TXT_COMMON_RING_RING7,TXT_COMMON_RING_RING8
#endif
    };
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:     
        GUILIST_SetMaxItem(MMISET_RING_SELECT_CALL_CTRL_ID,MMISET_CALL_RING_MAX_CLASS, FALSE );//max item 2
        MMISET_AppendNVCallRingCalssItem(add_data_ptr->s_dual_sys, ctrl_id);/*lint !e64*/
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_FULL_PAINT:        
        ring_info = MMIAPIENVSET_GetCallRingInfo(add_data_ptr->s_dual_sys,MMIENVSET_GetCurModeId());/*lint !e64*/
        cur_selection = (uint16)ring_info.call_ring_type;     
        GUILIST_SetSelectedItem(MMISET_RING_SELECT_CALL_CTRL_ID,cur_selection,TRUE);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        switch (cur_selection)
        {
        case 0://固定铃声
            ring_info = MMIAPIENVSET_GetCallRingInfo(add_data_ptr->s_dual_sys,MMIENVSET_GetCurModeId());/*lint !e64*/
            MMIAPISET_OpenFixCallRingWin(add_data_ptr->s_dual_sys, win_id, ring_info.fix_ring_index);/*lint !e64*/
            break;
            
        case 1:            
            ring_info = MMIAPISET_GetCallRingInfo(add_data_ptr->s_dual_sys,MMIENVSET_GetCurModeId());  /*lint !e64*/ 
            //允许的来电铃声类型中不包括AMR         
			music_types = MMIFMM_MUSIC_ALL;//MMIFMM_MUSIC_MP3|MMIFMM_MUSIC_WMA|MMIFMM_MUSIC_MID|MMIFMM_MUSIC_ACC|MMIFMM_MUSIC_M4A|MMIFMM_MUSIC_WAV|MMIFMM_MUSIC_MIDI|MMIFMM_MUSIC_AMR;
#ifdef DRM_SUPPORT
             music_types |= MMIFMM_DRM_ALL; 
#endif
            select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = music_types;
            select_info.win_id = win_id;
            select_info.ring_vol = MMIAPIENVSET_GetCurModeRingVol(MMISET_RING_TYPE_CALL);
            
            //if volume is 0, play the music with volume 3
            if (0 == select_info.ring_vol)
            {
                select_info.ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
            }

            if(MMISET_CALL_RING_MORE_RING == ring_info.call_ring_type)
            {
                select_info.ticked_file_wstr = ring_info.more_ring_info.name_wstr;
                select_info.ticked_file_len= ring_info.more_ring_info.name_wstr_len;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectMusicWin(&select_info);
            }
            else
            {
                select_info.ticked_file_wstr = PNULL;
                select_info.ticked_file_len= 0;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectMusicWin(&select_info);
            }
#ifdef DRM_SUPPORT
            {
                MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT |MMIFMM_DRM_CONS_LIMIT_INTERVAL |MMIFMM_DRM_CONS_LIMIT_DATETIME;;
           		limit_value.is_rights_valid = 1;
                MMIAPIFMM_SetSelDrmLimitValue(limit_value);
            }
#endif
            break;
            
        default:
            //SCI_PASSERT(FALSE,("HandleSelectCallRingWindow: cur_selection %d is error!",cur_selection));
            break;
        }
        break;
        
        case MSG_MULTIM_SELECTED_RETURN :          
            win_param = (MULTIM_SELECT_RETURN_T *)param;
            select_ring_info.name_wstr_len = (uint16)MIN(win_param->file_name_len,MMISET_CALL_RING_NAME_MAX_LEN);
            
            MMIAPICOM_Wstrncpy( select_ring_info.name_wstr, win_param->file_name_ptr, select_ring_info.name_wstr_len );  
            
            if(MMIAPIENVSET_SetMoreCallRingInfo(add_data_ptr->s_dual_sys,&select_ring_info,MMIENVSET_GetCurModeId()))/*lint !e64*/
            { //append set call ring list
                MMISET_AppendCallRingCalssItem(add_data_ptr->s_dual_sys, ctrl_id);/*lint !e64*/

                MMIAPIFMM_SplitFullPath(select_ring_info.name_wstr, select_ring_info.name_wstr_len,
                NULL, NULL, NULL, NULL,
                file_name, &ring.wstr_len);

                ring.wstr_ptr = file_name;
                GUILABEL_SetText(add_data_ptr->ctrl_id, &ring, FALSE);
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }

            MMK_CloseWin( win_id );      
            //MMK_SendMsg(MMISET_RING_SELECT_CALL_WIN_ID,MSG_SET_SELECT_FIX_CALL_RING_RETURN,PNULL);
            break;

        case MSG_SET_SELECT_FIX_CALL_RING_RETURN:
            ring_id_ptr = (uint16 *)param;
            MMIAPIENVSET_SetFixedCallRingId(add_data_ptr->s_dual_sys,*ring_id_ptr,MMIENVSET_GetCurModeId());
            GUILABEL_SetTextById(add_data_ptr->ctrl_id, text_id[(*ring_id_ptr)-1], FALSE);
            MMK_CloseWin( win_id ); 
            break;
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );      
            break;
            
        case MSG_CLOSE_WINDOW:    
            MMK_FreeWinAddData(win_id);
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : open select fixed ring window for phone book
//  Global resource dependence : 
//  Author:kelly.li
//  Note:             
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenFixCallRingWin(
                                         MN_DUAL_SYS_E dual_sys,
                                         MMI_WIN_ID_T    win_id,
                                         uint16       selected_id
                                         )
{
    MMISET_SELECT_WIN_PARAM_T   *win_param_ptr  =   NULL;
    
    
    win_param_ptr = SCI_ALLOC_APP(sizeof(MMISET_SELECT_WIN_PARAM_T));
   // SCI_ASSERT(NULL != win_param_ptr);
   if(PNULL == win_param_ptr)
   {
        //SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_OpenFixCallRingWin"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_WINTAB_614_112_2_18_2_52_23_377,(uint8*)"");
        return;
    } 
    SCI_MEMSET(win_param_ptr, 0, sizeof(MMISET_SELECT_WIN_PARAM_T));
    
    win_param_ptr->selected_ring_id = selected_id;
    win_param_ptr->win_id = win_id;
    win_param_ptr->dual_sys = dual_sys;

    //MMK_CreatePubListWin((uint32*)MMISET_RING_SELECT_FIXED_WIN_TAB,(ADD_DATA)win_param_ptr);
    MMK_CreateWin((uint32*)MMISET_RING_SELECT_FIXED_WIN_TAB,(ADD_DATA)win_param_ptr);
}

/*****************************************************************************/
//  Description : to handle select fixed msg ring
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectFixedMsgRingWindow(
                                                   MMI_WIN_ID_T    win_id, 
                                                   MMI_MESSAGE_ID_E   msg_id, 
                                                   DPARAM             param
                                                   )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    uint32          dual_sys    =   (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
		/* pause background sound */
		MMISRVAUD_ReqVirtualHandle("FIX MSG SELECT RING", MMISRVAUD_PRI_NORMAL);
        GUILIST_SetMaxItem(MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID,/*MMISET_MSG_FIXED_RING_MAX_ITEM*/g_alert_max+1/*MMISET_MAX_RING_ID*/, FALSE );
        AppendMsgRingItem(dual_sys,MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID);/*lint !e64*/
        MMK_SetAtvCtrl(win_id,MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID);
        break;
	case MSG_FULL_PAINT:
		{//reset status bar
// 			MMI_STRING_T    temp_str = {0};
// 			MMI_GetLabelTextByLang(STXT_HEADER_RINGING_TYPE, &temp_str);
// 	    	if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len))
// 			{
// 				GUIWIN_UpdateStb();
// 			}
			GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SELECT, LEFT_BUTTON, TRUE);
			GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, TRUE);
			GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_RETURN, RIGHT_BUTTON, TRUE);
		}
		break;
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
        break;
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:
    case MSG_CTL_LIST_MOVEBOTTOM:
    case MSG_CTL_LIST_MOVETOP:
    case MSG_CTL_LIST_MOVEDOWN:
    case MSG_CTL_LIST_MOVEUP: 
    case MSG_CTL_LIST_NXTPAGE:
    case MSG_CTL_LIST_PREPAGE:
		GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SELECT, LEFT_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, TRUE);
		GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_RETURN, RIGHT_BUTTON, TRUE);
#ifndef MMI_PDA_SUPPORT
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
        MMISET_PlayRingByListItem(MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID,MMISET_RING_TYPE_MSG);
#endif
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
		//break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        //如果是list单击, 直接返回; 只响应双击
        if ( MMK_IsSingleClick(msg_id, param))
        {
#ifdef MMI_PDA_SUPPORT
            MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
            MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
            MMISET_PlayRingByListItem(MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID,MMISET_RING_TYPE_MSG);
#endif
            break;
        }
        else
        {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
            if ( MSG_CTL_PENOK == msg_id && PNULL != param )
            {
                MMI_NOTIFY_T* notify_ptr = (MMI_NOTIFY_T*)param;
                if(notify_ptr->src_id == MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID)
                {
                    break;
                }
            }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

        }
        
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
        /*if(GUILIST_GetCurItemIndex(MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID) == 0){
			//enter music
		//WXY need change to new api	MMIAPMULTILIST_EnterSongListWin(0);
			//MMIAPPreMAINWIN_Enter(TRUE);
			break;
		}else*/{
// #ifdef MMI_MULTI_SIM_SYS_SINGLE
        	MMISET_SetMsgRingByRadioItem(dual_sys, MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID);
// #else
//         	MMIAPIENVSET_SetBothSIMFixMsgRingId(GUILIST_GetCurItemIndex(MMISET_MSG_RING_SELECT_FIXED_WIN_CTRL_ID));
// #endif
		}
		MMK_CloseWin( win_id );
        //update msg ring type list
        //  MMK_SendMsg(MMISET_RING_SELECT_MSG_WIN_ID,MSG_FULL_PAINT,PNULL);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_GET_FOCUS:
        //MMIDEFAULT_EnableKeyRing(FALSE);
        //MMIDEFAULT_EnableTpRing(FALSE);    
        break;
        
    case MSG_LOSE_FOCUS:
        //stop ring
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        //MMIDEFAULT_EnableKeyRing(TRUE);
        //MMIDEFAULT_EnableTpRing(TRUE);        
        break;
        
    case MSG_CLOSE_WINDOW:
        //stop ring
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
		/* resume background sound */
        MMISRVAUD_FreeVirtualHandle("FIX MSG SELECT RING");
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : to handle select fixed call ring
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectFixedCallRingWindow(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    )
{
    uint16                      cur_selection   =   0;
    uint16                      ring_id         =   0;
    MMI_RESULT_E                result          =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id         =   MMISET_RING_SELECT_FIXED_CTRL_ID;
    MMISET_SELECT_WIN_PARAM_T   *win_param      =   NULL;
    
    win_param = (MMISET_SELECT_WIN_PARAM_T *)MMK_GetWinAddDataPtr(win_id);
    //SCI_ASSERT(PNULL != win_param);
   if(PNULL == win_param)
   {
        //SCI_TRACE_LOW:"mmisetting_assert HandleSelectFixedCallRingWindow"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_WINTAB_754_112_2_18_2_52_23_378,(uint8*)"");
        return MMI_RESULT_FALSE;
    }     
    //SCI_TRACE_LOW:"[MMISET]:HandleSelectFixedCallRingWindow, msg = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_WINTAB_757_112_2_18_2_52_23_379,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
		/* pause background sound */  /*Bug 163146 后台播放音频，来电铃声设置,暂停后台播放*/
        MMISRVAUD_ReqVirtualHandle("FIX CALL SELECT RING", MMISRVAUD_PRI_NORMAL);  //bug163146
#ifdef ORTHODOX_FIXED_RING_ONLY
        if(MMIAPISET_IsOrthodoxVariant())
        {
            GUILIST_SetMaxItem(ctrl_id, g_ringtone_max, FALSE );
        }
        else
#endif
        {
            GUILIST_SetMaxItem(ctrl_id, g_ringtone_max+1, FALSE );
        }
        MMISET_AppendFixedCallRingItem(win_param->dual_sys, ctrl_id,win_param);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
	case MSG_FULL_PAINT:
		{
            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SELECT, LEFT_BUTTON, TRUE);
		    GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, TRUE);
		    GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_RETURN, RIGHT_BUTTON, TRUE);
        }
        break;
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
        break;       
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:
        
    case MSG_CTL_LIST_MOVEDOWN:
    case MSG_CTL_LIST_MOVEUP: 
    case MSG_CTL_LIST_MOVEBOTTOM:
    case MSG_CTL_LIST_MOVETOP:
    case MSG_CTL_LIST_NXTPAGE:
    case MSG_CTL_LIST_PREPAGE: 
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SELECT, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_RETURN, RIGHT_BUTTON, TRUE); 

#ifndef MMI_PDA_SUPPORT         
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
        MMISET_PlayRingByListItem(ctrl_id,MMISET_RING_TYPE_CALL);
#endif 
        
        break;     
    case MSG_CTL_OK:
    case MSG_APP_OK:
        //break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        //如果是list单击, 直接返回; 只响应双击
        if ( MMK_IsSingleClick(msg_id, param))
        {
#ifdef MMI_PDA_SUPPORT
            MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
            MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
            MMISET_PlayRingByListItem(ctrl_id,MMISET_RING_TYPE_CALL);
#endif
            break;
        }
        else
        {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
            if ( MSG_CTL_PENOK == msg_id && PNULL != param )
            {
                MMI_NOTIFY_T* notify_ptr = (MMI_NOTIFY_T*)param;
                if(notify_ptr->src_id == MMISET_RING_SELECT_FIXED_CTRL_ID)
                {
                    break;
                }
            }
#endif
            
        }
        
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        ring_id = (uint16)(cur_selection +1);
/*#ifdef ORTHODOX_FIXED_RING_ONLY
        if((cur_selection == 0) && (!MMIAPISET_IsOrthodoxVariant()))
#else
        if(cur_selection == 0)
#endif
        {
            if ( MMISET_SET_SOUNDS_WIN_ID == win_param->win_id)
            {
                //enter music
               //WXY need change to new api MMIAPMULTILIST_EnterSongListWin(0);
            }
            else
            {
                //WXY need change to new api MMIAPMULTILIST_EnterSongListWin(win_param->win_id);
                MMK_CloseWin( win_id );
            }
            break;
        }
        else*/
        {
#ifdef ORTHODOX_FIXED_RING_ONLY
            if(MMIAPISET_IsOrthodoxVariant())
            {
                ring_id = (uint8)(cur_selection + 1);
            }
#endif

            if ( 0 != win_param->win_id)
            {
                MMK_SendMsg(win_param->win_id,MSG_SET_SELECT_FIX_CALL_RING_RETURN,(void*)&ring_id);
            }
            else
            {
#ifdef MMI_MULTI_SIM_SYS_SINGLE
                MMIAPIENVSET_SetFixedCallRingId(win_param->dual_sys,ring_id,MMIENVSET_GetCurModeId());
#else
                MMIAPIENVSET_SetBothSIMFixedCallRingId(ring_id);
#endif
            }
        }
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_GET_FOCUS:
        //MMIDEFAULT_EnableKeyRing(FALSE);
        //MMIDEFAULT_EnableTpRing(FALSE);       
        break;
        
    case MSG_LOSE_FOCUS:
        //stop ring
        SCI_TRACE_LOW("HandleSelectFixedCallRingWindow: FocusWinHandle=0x%x, BannerWinHandle=0x%x", MMK_GetFocusWinHandle(), MMK_ConvertIdToHandle(MMICOMMON_BANNER_WIN_ID));
        if(MMK_GetFocusWinHandle() != MMK_ConvertIdToHandle(MMICOMMON_BANNER_WIN_ID))
        {
            MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        }
        //MMIDEFAULT_EnableKeyRing(TRUE);
        //MMIDEFAULT_EnableTpRing(TRUE);
        break;
        
    case MSG_CLOSE_WINDOW:
        //stop ring
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        /* resume background sound */ /*Bug 163146 后台播放音频，结束来电铃声设置,恢复后台播放*/
        MMISRVAUD_FreeVirtualHandle("FIX CALL SELECT RING");  //bug163146
        SCI_FREE(win_param);
        win_param = NULL;
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : to handle select message ring
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectMsgRingWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    uint16                  cur_selection       = 0;
    MMI_RESULT_E            result              = MMI_RESULT_TRUE;
    MMISET_MSG_RING_T       ring_info           = {MMISET_MSG_RING_FIXED, 0, 0};
    MMISET_CALL_MORE_RING_T select_ring_info    ={0};
    MULTIM_SELECT_RETURN_T  *win_param          = NULL;
    MMIENVSET_ADD_DATA_T*       add_data_ptr     =  (MMIENVSET_ADD_DATA_T*) MMK_GetWinAddDataPtr(win_id); 
    MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_MUSIC_ALL,0,0,0,PNULL,0,-1};
    MMI_STRING_T                ring                                   = {0};
    wchar                       file_name[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
    uint32                      ring_id                                = 0;
    uint32 text_id[MMISET_MSG_FIXED_RING_MAX_ITEM] =
    {
        TXT_COMMON_RING_RING1,TXT_COMMON_RING_RING2
#ifndef MMI_LOW_MEMORY_RING
        ,TXT_RING3,TXT_RING4,
        TXT_RING5,TXT_RING6,
#endif
    };

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_RING_SELECT_MSG_CTRL_ID,MMISET_MSG_RING_MAX_CLASS, FALSE );//max item 2
        MMISET_AppendMsgRingClassItem(add_data_ptr->s_dual_sys, MMISET_RING_SELECT_MSG_CTRL_ID);/*lint !e64*/
        MMK_SetAtvCtrl(win_id,MMISET_RING_SELECT_MSG_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
        ring_info = MMIAPISET_GetMsgRingInfo(add_data_ptr->s_dual_sys,MMIENVSET_GetCurModeId());/*lint !e64*/
        cur_selection = (uint16)ring_info.msg_ring_type;     
        GUILIST_SetSelectedItem(MMISET_RING_SELECT_MSG_CTRL_ID,cur_selection,TRUE);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(MMISET_RING_SELECT_MSG_CTRL_ID);
        switch (cur_selection)
        {
        case 0://固定铃声  
            //MMK_CreatePubListWin((uint32*)MMISET_MSG_RING_SELECT_FIXED_WIN_TAB, (ADD_DATA)add_data_ptr->s_dual_sys);
            MMK_CreateWin((uint32*)MMISET_MSG_RING_SELECT_FIXED_WIN_TAB, (ADD_DATA)add_data_ptr->s_dual_sys);
            break;
            
        case 1:
            select_info.ring_vol = MMIAPIENVSET_GetCurModeRingVol(MMISET_RING_TYPE_MSG);
            //if volume is 0, play the music with volume 3
            if (0 == select_info.ring_vol)
            {
                select_info.ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
            }
            ring_info = MMIAPISET_GetMsgRingInfo(add_data_ptr->s_dual_sys,MMIENVSET_GetCurModeId());/*lint !e64*/           
#ifdef DRM_SUPPORT
            select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL;
#endif
            if(MMISET_MSG_RING_MORE_RING == ring_info.msg_ring_type)
            {
                select_info.ticked_file_wstr = ring_info.more_ring_info.name_wstr;
                select_info.ticked_file_len= ring_info.more_ring_info.name_wstr_len;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectMusicWin(&select_info);
            }
            else
            {
                select_info.ticked_file_wstr = PNULL;
                select_info.ticked_file_len= 0;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectMusicWin(&select_info);
            }
#ifdef DRM_SUPPORT
            {
                 MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                 limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT |MMIFMM_DRM_CONS_LIMIT_INTERVAL |MMIFMM_DRM_CONS_LIMIT_DATETIME;;
           		 limit_value.is_rights_valid = 1;
                 MMIAPIFMM_SetSelDrmLimitValue(limit_value);
            }
#endif
            break;
            
        default:
            //SCI_PASSERT(FALSE,("HandleSelectCallRingWindow: cur_selection %d is error!",cur_selection));
            break;
        }
        break;        
        
        case MSG_MULTIM_SELECTED_RETURN:
            win_param = (MULTIM_SELECT_RETURN_T *)param;
            select_ring_info.name_wstr_len = (uint16)MIN(win_param->file_name_len,MMISET_CALL_RING_NAME_MAX_LEN);
            MMIAPICOM_Wstrncpy( select_ring_info.name_wstr, win_param->file_name_ptr, select_ring_info.name_wstr_len );                 
            if(!MMISET_SetMoreMsgRingInfo(add_data_ptr->s_dual_sys, MMISET_RING_SELECT_MSG_CTRL_ID,&select_ring_info))/*lint !e64*/
 			{
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
            else
            {
                MMIAPIFMM_SplitFullPath(select_ring_info.name_wstr, select_ring_info.name_wstr_len,
                                NULL, NULL, NULL, NULL,
                                file_name, &ring.wstr_len);

                ring.wstr_ptr = file_name;
                GUILABEL_SetText(add_data_ptr->ctrl_id, &ring, FALSE);
            }
            MMK_CloseWin( win_id ); 
            break;
            
        case MSG_SET_SELECT_FIX_MSG_RING_RETURN:
            ring_id = (uint32)param;
            GUILABEL_SetTextById(add_data_ptr->ctrl_id, text_id[ring_id-1], FALSE);
            MMK_CloseWin( win_id );
            break;
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );      
            break;
            
        case MSG_CLOSE_WINDOW:
            MMK_FreeWinAddData(win_id);
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 添加消息铃声radio list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
   
LOCAL void AppendMsgRingItem(
                             MN_DUAL_SYS_E  dual_sys,
                             MMI_CTRL_ID_T   ctrl_id
                             )
{
    #define LIST_STR_LEN    20 
    uint8               ring_vol      = 0;
    uint8               ring_id       = 0;
    uint16              cur_selection = 0;
    MMISET_MSG_RING_T   msg_ring_info = {MMISET_MSG_RING_FIXED, 0, 0};

    GUILIST_RemoveAllItems(ctrl_id);
    MMIAPISET_AppendFixedItems(
                                    TXT_RING,
                                    /*MMISET_MSG_FIXED_RING_MAX_ITEM*/g_alert_max,
                                    TXT_COMMON_OK,
                                    TXT_NULL,
                                    STXT_RETURN,
                                    ctrl_id,
                                    GUIITEM_STYLE_ONE_LINE_TEXT_MS
									//dong.chunguang modify as ui spec
                                    //GUIITEM_STYLE_ONE_LINE_RADIO
                                    );
    //get selected msg ring info
    msg_ring_info = MMIAPISET_GetMsgRingInfo(dual_sys,MMIENVSET_GetCurModeId());
    
    if (MMISET_MSG_RING_FIXED == msg_ring_info.msg_ring_type)
    {
        cur_selection = (uint16)(msg_ring_info.fix_ring_index - 1);
        //cur_selection = (uint16)(msg_ring_info.fix_ring_index);//except choose from music
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
    }
    else
    {
        cur_selection = 0;//GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_SetSelectedItem(ctrl_id,cur_selection, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
    }
    
    ring_id = (uint8)(cur_selection + 1);   
    
    //get ring volume
    ring_vol = MMIAPIENVSET_GetCurModeRingVol(MMISET_RING_TYPE_MSG);
    
    //if volume is 0, play the music with volume 3
    if (0 == ring_vol)
    {
        ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
    }
    //if (MMISET_MSG_RING_MORE_RING == msg_ring_info.msg_ring_type||cur_selection ==0)
		//return;//except choose from music
    //play ring
    MMIAPISET_PlayCallRingByVol(ring_vol,ring_id, 1, MMISET_RING_TYPE_MSG, PNULL);
}


/*****************************************************************************/
//  Description : 更新闹钟的设置
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_UpdateAlarmSet(void)
{
#if 0	/*时间更新统一处理*/
    //update alarm event
    MMIALM_UpdateEvents();
    
    //set alarm icon in idle
    if (MMIALM_HasActiveEvent())
    {
        MAIN_SetIdleAlarmState(TRUE);
    }
#endif
}






/*****************************************************************************/
//  Description : to open ring setting menu
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL void EnterRingSettingMenu( void)
{
    MMK_CreateWin((uint32 *)MMISET_ICON_RING_MENU_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : to enter ring setting menu
//  Global resource dependence : 
//  Author:xiangjie.xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_EnterRingSettingMenu( void)
{
    EnterRingSettingMenu(); 
}


/*****************************************************************************/
//  Description :铃声设置中所有label的设置
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void EnvSetRingParam(MMI_WIN_ID_T    win_id)
{
    uint16 loop = 0;
    MMISET_CALL_RING_T  call_ring_info ={0};
    MMISET_MSG_RING_T   msg_ring_info = {0};
    GUILABEL_INIT_DATA_T text_label_init = {0};
    GUIFORM_DYNA_CHILD_T  child_data = {0};
    GUIFORM_DYNA_CHILD_T  child_form_data = {0};
    GUIFORM_INIT_DATA_T     init_data = {0};
    MN_DUAL_SYS_E               dual_sys    =   MN_DUAL_SYS_MAX;
    MMI_STRING_T                ring                                   = {0};
    wchar                       file_name[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};

    uint32 text_id[MMISET_RING_FIXED_DEFAUT_NUM] =
    {TXT_COMMON_RING_RING1,TXT_COMMON_RING_RING2,TXT_COMMON_RING_RING3,TXT_COMMON_RING_RING4
#if 0
def MMI_LOW_MEMORY_RING
    ,TXT_COMMON_RING_RING3,TXT_COMMON_RING_RING4,
    TXT_COMMON_RING_RING5,TXT_COMMON_RING_RING6,TXT_COMMON_RING_RING7,TXT_COMMON_RING_RING8
#endif
    };
#ifdef MMI_MULTI_SIM_SYS_SINGLE
    uint32 call_ring_title_id[]= {TXT_SET_RING_SELECT_CALL};
    uint32 msg_ring_title_id[] = {TXT_SET_RING_SELECT_MSG};
#else
    uint32 call_ring_title_id[]=
    {
        TXT_SET_RING_SIM1_SELECT_CALL,
        TXT_SET_RING_SIM2_SELECT_CALL,
        TXT_SET_RING_SIM3_SELECT_CALL,
        TXT_SET_RING_SIM4_SELECT_CALL
    };
    uint32 msg_ring_title_id[]=
    {
        TXT_SET_RING_SIM1_SELECT_MSG,
        TXT_SET_RING_SIM2_SELECT_MSG,
        TXT_SET_RING_SIM3_SELECT_MSG,
        TXT_SET_RING_SIM4_SELECT_MSG
    };
#endif
    for(loop = MN_DUAL_SYS_1; loop < 4 * MMI_DUAL_SYS_MAX; loop++)
    {
        if(0 == loop%2) //当loop = 0，2，4，6...时，创建一个可focus的子form
        {
            child_form_data.child_handle = MMISET_ENV_RING_CHILD_FORM_CTRL_ID + loop/2;
            child_form_data.guid = SPRD_GUI_FORM_ID;
            child_form_data.is_get_active = TRUE;
            child_form_data.is_bg       =   FALSE;
            init_data.layout_type = GUIFORM_LAYOUT_ORDER;
            child_form_data.init_data_ptr = &init_data;
            GUIFORM_CreatDynaChildCtrl(win_id, MMISET_ENV_RING_FORM_CTRL_ID, &child_form_data);
            GUIFORM_SetStyle(child_form_data.child_handle,GUIFORM_STYLE_UNIT);
        }
        //开始创建子form的子label：分为可active和不可active两种
        child_data.child_handle = MMISET_ENV_CALL_RING_LABEL_CTRL_ID + loop;
        child_data.guid = SPRD_GUI_LABEL_ID;
        if(0 == loop%2)//单数位置创建不可active的label
        {
            child_data.is_get_active = FALSE;
            text_label_init.align = GUILABEL_ALIGN_LEFT;
        }
        else if(1 == loop%2) //双数位置创建可active的label控件
        {
            child_data.is_get_active = TRUE;
            text_label_init.align = GUILABEL_ALIGN_DEFAULT;
        }
        child_data.is_bg       =   FALSE;
        child_data.init_data_ptr = &text_label_init;
        GUIFORM_CreatDynaChildCtrl(win_id, child_form_data.child_handle, &child_data);
        if(0 == loop%4)//0,4,8...的相对位置放着sim1,sim2..来电铃声的title
        {
            GUILABEL_SetTextById(MMISET_ENV_CALL_RING_LABEL_CTRL_ID+loop, call_ring_title_id[loop/4], FALSE);
        }
        else if(2 == loop%4)//2,6,10...的相对位置放着sim1,sim2..信息铃声的title
        {
            GUILABEL_SetTextById(MMISET_ENV_CALL_RING_LABEL_CTRL_ID+loop, msg_ring_title_id[loop/4], FALSE);
        } 
        
        dual_sys = MN_DUAL_SYS_1 + loop/4;
        call_ring_info = MMIAPISET_GetCallRingInfo(dual_sys,MMIENVSET_GetCurModeId());
        msg_ring_info = MMIAPISET_GetMsgRingInfo(dual_sys,MMIENVSET_GetCurModeId());
        
        if(1 == loop%4)//1，5，9...的相对位置放着sim1,sim2..来电铃声的信息
        {
            if(MMISET_CALL_RING_FIXED == call_ring_info.call_ring_type)
            {
                GUILABEL_SetTextById(MMISET_ENV_CALL_RING_LABEL_CTRL_ID+loop, text_id[call_ring_info.fix_ring_index - 1], FALSE);  
            }
            else if(MMISET_CALL_RING_MORE_RING == call_ring_info.call_ring_type)
            {
                MMIAPIFMM_SplitFullPath(call_ring_info.more_ring_info.name_wstr, call_ring_info.more_ring_info.name_wstr_len,
                                        NULL, NULL, NULL, NULL,
                                        file_name, &ring.wstr_len);
                
                ring.wstr_ptr = file_name;
                GUILABEL_SetText(MMISET_ENV_CALL_RING_LABEL_CTRL_ID+loop, &ring, FALSE);
            }
        }
        else if(3 == loop%4)//1，5，9...的相对位置放着sim1,sim2..信息铃声的信息
        {
            if(MMISET_MSG_RING_FIXED == msg_ring_info.msg_ring_type)
            {
                GUILABEL_SetTextById(MMISET_ENV_CALL_RING_LABEL_CTRL_ID+loop, text_id[msg_ring_info.fix_ring_index - 1], FALSE);  
            }
            else if(MMISET_MSG_RING_MORE_RING == msg_ring_info.msg_ring_type)
            {
                MMIAPIFMM_SplitFullPath(msg_ring_info.more_ring_info.name_wstr, msg_ring_info.more_ring_info.name_wstr_len,
                                        NULL, NULL, NULL, NULL,
                                        file_name, &ring.wstr_len);
                
                ring.wstr_ptr = file_name;
                GUILABEL_SetText(MMISET_ENV_CALL_RING_LABEL_CTRL_ID+loop, &ring, FALSE);
            }
        }
    }
}
#ifdef MMI_PDA_PREVIEW_BUTTON_SUPPORT
/*****************************************************************************/
//  Description : create button layer.
//  Global resource dependence : none
//  Author: arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateButtonLayer(MMI_WIN_ID_T win_id)
{
    UILAYER_CREATE_T   create_info = {0};
    UILAYER_RESULT_E  result = UILAYER_RESULT_SUCCESS;
    uint16 layer_width = 0;
    uint16 layer_height = 0;
    
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &layer_width, &layer_height);
    /*创建BUTTON使用的层*/
    create_info.lcd_id          = LCD_ID_0;
    create_info.owner_handle    = win_id;
    create_info.offset_x        = 0;
    create_info.offset_y        = 0;
    create_info.width           = MAX(layer_width,layer_height);//因为只有第一次时初始化，之后可能进行横竖屏转换导致部分区域没有在layer内
    create_info.height          = MAX(layer_width,layer_height);//故设置layer的区域为横竖屏区域的并集。CR163796
    create_info.is_bg_layer     = FALSE;
    create_info.is_static_layer = FALSE;   

    result = UILAYER_CreateLayer(&create_info, &s_set_button_layer_info);
    if(UILAYER_RESULT_SUCCESS == result)
    {
        UILAYER_APPEND_BLT_T  append_blt = {0};
        UILAYER_Clear(&s_set_button_layer_info);
        append_blt.layer_level = UILAYER_LEVEL_NORMAL;
        append_blt.lcd_dev_info = s_set_button_layer_info;
        UILAYER_AppendBltLayer(&append_blt);         
        return TRUE;
    }
    else
    {
        s_set_button_layer_info.block_id = 0;
        return FALSE;
    }
}
/*****************************************************************************/
//  Description : CreateButton
//  Global resource dependence : none
//  Author: arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL void CreateButton(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_id, GUI_BOTH_RECT_T both_rect)
{
    GUIBUTTON_INIT_DATA_T   init_data = {0};
    MMI_HANDLE_T handle = 0;
    GUI_FONT_ALL_T font_all = {0};
    GUI_BG_T btn_bg = {0};
    
    init_data.both_rect = both_rect;
    init_data.bg.bg_type = GUI_BG_IMG;
#ifdef MMI_PDA_SUPPORT                
    init_data.bg.img_id = IMAGE_PUBWIN_BUTTON_PDA;//IMAGE_COMMON_PICVIEW_BUTTON;
#else
    init_data.bg.img_id = IMAGE_SOFTKEY_BG;
#endif

    handle = GUIBUTTON_CreateDynamic(MMK_GetFocusWinId(), ctrl_id, &init_data);
    GUIBUTTON_SetTextId(handle, text_id);

    btn_bg.bg_type = GUI_BG_IMG;
#if defined(MMIPUBWIN_PDASTYLE_SUPPORT) || defined(GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT) || defined(GUIPOPMENU_POP_CONTEXT_SUPPORT) || defined(GUIF_EDIT_PDASTYLE) || defined(GUIF_SOFTKEY_PDASTYLE)
    btn_bg.img_id = IMAGE_PUBWIN_BUTTON_HL_PDA;    
#else
    btn_bg.img_id = IMAGE_SOFTKEY_BG_DOWN;    
#endif
    GUIBUTTON_SetPressedBg(handle, &btn_bg);
    GUIBUTTON_SetRunSheen(handle, FALSE);

#ifdef MMI_PDA_SUPPORT
        font_all.color = MMI_BLACK_COLOR;
#else
        font_all.color = MMI_WHITE_COLOR;
#endif
    font_all.font = MMI_DEFAULT_NORMAL_FONT;
    GUIBUTTON_SetFont(handle, &font_all);

}

/*****************************************************************************/
//  Description : MMIAPISET_CreatButtonLayer供静态墙纸和屏保或其它模块使用 
//  Global resource dependence :必须跟Destroy成对使用
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_CreatSetButton(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ok_ctrl_id,GUIBUTTON_CALLBACK_FUNC ok_call_back,GUIBUTTON_CALLBACK_FUNC cancel_call_back)
{
    MMI_CTRL_ID_T cancel_ctrl_id = MMISET_SET_CANCEL_BUTTON;
    uint16    button_icon_width = 0;
    uint16    button_icon_height = 0;
    uint16 layer_width = 0;
    uint16 layer_height = 0;
    GUI_RECT_T  rc = {0};
    GUI_BOTH_RECT_T  both_rect = {0};

#ifdef MMI_PDA_SUPPORT            
    GUIRES_GetImgWidthHeight(&button_icon_width, &button_icon_height, IMAGE_PUBWIN_BUTTON_PDA, win_id);
#else
    GUIRES_GetImgWidthHeight(&button_icon_width, &button_icon_height, IMAGE_SOFTKEY_BG, win_id);
#endif
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &layer_width, &layer_height);

    if(layer_width < layer_height)
    {
    button_icon_width = layer_width/3 - 4;

    rc.left = ((layer_width/2) - button_icon_width)/2;
    rc.right = rc.left + button_icon_width;
    rc.bottom = layer_height - MMI_BUTTON_MARGIN;
    rc.top = rc.bottom - button_icon_height;
    both_rect.v_rect = rc;
        
        button_icon_width = layer_height/3 - 4;

        rc.left = ((layer_height/2) - button_icon_width)/2;
        rc.right = rc.left + button_icon_width;
        rc.bottom = layer_width - MMI_BUTTON_MARGIN;
        rc.top = rc.bottom - button_icon_height;
    both_rect.h_rect = rc;
    }
    else
    {
        button_icon_width = layer_width/3 - 4;

        rc.left = ((layer_width/2) - button_icon_width)/2;
        rc.right = rc.left + button_icon_width;
        rc.bottom = layer_height - MMI_BUTTON_MARGIN;
        rc.top = rc.bottom - button_icon_height;
        both_rect.h_rect = rc;
        
        button_icon_width = layer_height/3 - 4;

        rc.left = ((layer_height/2) - button_icon_width)/2;
        rc.right = rc.left + button_icon_width;
        rc.bottom = layer_width - MMI_BUTTON_MARGIN;
        rc.top = rc.bottom - button_icon_height;
        both_rect.v_rect = rc;
    }
    CreateButton(win_id, ok_ctrl_id, TXT_COMMON_OK, both_rect);
   IGUICTRL_SetState(MMK_GetCtrlPtr(ok_ctrl_id), GUICTRL_STATE_DISABLE_ACTIVE, FALSE);
    if(layer_width < layer_height)
    {
        button_icon_width = layer_width/3 - 4;
    rc.left = ((layer_width/2) - button_icon_width)/2 + (layer_width/2);
    rc.right = rc.left + button_icon_width;
    rc.bottom = layer_height - MMI_BUTTON_MARGIN;
    rc.top = rc.bottom - button_icon_height;
    both_rect.v_rect = rc;
       
        button_icon_width = layer_height/3 - 4;
        rc.left = ((layer_height/2) - button_icon_width)/2 + (layer_height/2);
        rc.right = rc.left + button_icon_width;
        rc.bottom = layer_width - MMI_BUTTON_MARGIN;
        rc.top = rc.bottom - button_icon_height;
    both_rect.h_rect = rc;
    }
    else
    {
        button_icon_width = layer_width/3 - 4;
        rc.left = ((layer_width/2) - button_icon_width)/2 + (layer_width/2);
        rc.right = rc.left + button_icon_width;
        rc.bottom = layer_height - MMI_BUTTON_MARGIN;
        rc.top = rc.bottom - button_icon_height;
        both_rect.h_rect = rc;
       
        button_icon_width = layer_height/3 - 4;
        rc.left = ((layer_height/2) - button_icon_width)/2 + (layer_height/2);
        rc.right = rc.left + button_icon_width;
        rc.bottom = layer_width - MMI_BUTTON_MARGIN;
        rc.top = rc.bottom - button_icon_height;
        both_rect.v_rect = rc;
    }
    CreateButton(win_id, cancel_ctrl_id, STXT_CANCEL, both_rect);
    IGUICTRL_SetState(MMK_GetCtrlPtr(cancel_ctrl_id), GUICTRL_STATE_DISABLE_ACTIVE, FALSE);
    GUIBUTTON_SetCallBackFunc(cancel_ctrl_id, cancel_call_back);    
    GUIBUTTON_SetCallBackFunc(ok_ctrl_id, ok_call_back);
}
/*****************************************************************************/
//  Description : 创建button显示的层，并设置控件到层上,与
//                    MMIAPISET_DestroySetButtonLayer成对调用
//  Global resource dependence : 
//  Author:arvin.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_CreateSetButtonLayer(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ok_ctrl_id)
{
    MMI_CTRL_ID_T cancel_ctrl_id = MMISET_SET_CANCEL_BUTTON;
    
    if(CreateButtonLayer(win_id))
    {
        MMK_SetCtrlLcdDevInfo(ok_ctrl_id, &s_set_button_layer_info);
        MMK_SetCtrlLcdDevInfo(cancel_ctrl_id, &s_set_button_layer_info);

    }
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO* MMIAPISET_GetButtonLayer()
{
    return &s_set_button_layer_info;
}
/*****************************************************************************/
//  Description : 当创建了这两个button的控件失去焦点时调用
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_DestroySetButtonLayer(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ok_ctrl_id)
{
    MMI_CTRL_ID_T cancel_ctrl_id = MMISET_SET_CANCEL_BUTTON;
    GUI_LCD_DEV_INFO default_lcd_dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
    UILAYER_ReleaseLayer(&s_set_button_layer_info); 
    s_set_button_layer_info.lcd_id = 0;   
    s_set_button_layer_info.block_id = UILAYER_NULL_HANDLE;   
    MMK_SetCtrlLcdDevInfo(ok_ctrl_id, &default_lcd_dev);
    MMK_SetCtrlLcdDevInfo(cancel_ctrl_id, &default_lcd_dev);
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:juan.wu
//	Note: 
/*****************************************************************************/
PUBLIC  MMI_RESULT_E MMIAPISET_SetOkButtonCallback(void)
{
    return (MMI_RESULT_E)MMK_SendMsg(MMK_GetFocusWinHandle(), MSG_APP_OK, PNULL);
}

/*****************************************************************************/
//  Description : SetCancelButtonCallback
//  Global resource dependence : none
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISET_SetCancelButtonCallback(void)
{
    return (MMI_RESULT_E)MMK_CloseWin(MMK_GetFocusWinHandle());
}

/*****************************************************************************/
//  Description : MMISET_AppendButtonLayer
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 这个button layer在full paint的时候要append一下
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendSetButtonLayer(void)
{
    UILAYER_APPEND_BLT_T  append_blt = {0};
    
    if(UILAYER_IsLayerActive(&s_set_button_layer_info))
    {
        
        UILAYER_Clear(&s_set_button_layer_info);
        append_blt.layer_level = UILAYER_LEVEL_NORMAL;
        append_blt.lcd_dev_info = s_set_button_layer_info;
        UILAYER_AppendBltLayer(&append_blt);         
    }
}

/*****************************************************************************/
//  Description : MMISET_AppendButtonLayer
//  Global resource dependence : 
//  Author: murphy.xie
//  Note: 这个button layer在button update时，clear一下层，变成透明
/*****************************************************************************/
PUBLIC void MMIAPISET_ClearButtonLayer(GUI_RECT_T update_rect)
{    
	if (UILAYER_IsLayerActive(&s_set_button_layer_info))
	{
		UILAYER_ClearRect(&s_set_button_layer_info, update_rect);
	}
}
#endif

/*****************************************************************************/
//  Description : handle flight mode win msg
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_HandleFlightmodeWinMsg(
                                            MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param
                                            )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint32 i = 0;

            MMISET_StopPSApplication();

            MMIAPISET_SetFlyMode(TRUE);
 
            MMIAPIPHONE_DeactivePsAccordingToSetting();

            //MMIPUB_CloseQuerytWin(PNULL);
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);    
}
/*****************************************************************************/
//  Description : open select fixed ring window for phone book
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenFixCallRingListWin(
                                         MN_DUAL_SYS_E dual_sys,
                                         MMI_WIN_ID_T    win_id,
                                         uint16       selected_id
                                         )
{
    MMISET_SELECT_WIN_PARAM_T   *win_param_ptr  =   NULL;

    win_param_ptr = SCI_ALLOC_APP(sizeof(MMISET_SELECT_WIN_PARAM_T));
   // SCI_ASSERT(NULL != win_param_ptr);
   if(PNULL == win_param_ptr)
   {
        //SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_OpenFixCallRingWin"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_WINTAB_614_112_2_18_2_52_23_377,(uint8*)"");
        return;
    }
    SCI_MEMSET(win_param_ptr, 0, sizeof(MMISET_SELECT_WIN_PARAM_T));

    win_param_ptr->selected_ring_id = selected_id;
    win_param_ptr->win_id = win_id;
    win_param_ptr->dual_sys = dual_sys;
	MMK_CreateWin((uint32*)MMISET_RING_SELECT_FIXED_WIN_TAB,(ADD_DATA)win_param_ptr);
    //MMK_CreatePubListWin((uint32*)MMISET_RING_SELECT_FIXED_WIN_TAB,(ADD_DATA)win_param_ptr);
}
/*****************************************************************************/
//  Description : open select fixed msg window for phone book
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenFixMsgRingListWin(
                                         MN_DUAL_SYS_E dual_sys,
                                         MMI_WIN_ID_T    win_id,
                                         uint16       selected_id
                                         )
{
/*
    MMISET_SELECT_WIN_PARAM_T   *win_param_ptr  =   NULL;

    win_param_ptr = SCI_ALLOC_APP(sizeof(MMISET_SELECT_WIN_PARAM_T));
   // SCI_ASSERT(NULL != win_param_ptr);
   if(PNULL == win_param_ptr)
   {
        //SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_OpenFixCallRingWin"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_WINTAB_614_112_2_18_2_52_23_377,(uint8*)"");
        return;
    }
    SCI_MEMSET(win_param_ptr, 0, sizeof(MMISET_SELECT_WIN_PARAM_T));

    win_param_ptr->selected_ring_id = selected_id;
    win_param_ptr->win_id = win_id;
    win_param_ptr->dual_sys = dual_sys;
*/
	MMK_CreateWin((uint32*)MMISET_MSG_RING_SELECT_FIXED_WIN_TAB,(ADD_DATA)dual_sys);
}
/*****************************************************************************/
//  Description : open select fixed msg window for phone book
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsOpenMsgRingWin(void)
{
	if(MMK_IsOpenWin(MMISET_MSG_RING_SELECT_FIXED_WIN_ID))
		return TRUE;
	else
		return FALSE;
}
/*Edit by script, ignore 6 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
#ifdef PHONE_NUMBER_VOICE

#define MMI_DIAL_VOICE_DEFAULT_VALUE 0 
#define MMI_TTS_VOL_DEFAULT_VALUE  9///7
MMISET_TTS_CODE_INFO_T	tts_info = {0};
#if  0
PUBLIC uint8 MMIAPISET_GetDIALVOICE_type( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
     		cur_set.tts_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  
		cur_set.dial_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
		cur_set.cc_code = 1; 
		//cur_set.hour_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  
		cur_set.time_code = MMI_DIAL_VOICE_DEFAULT_VALUE;
		cur_set.kl_code = MMI_DIAL_VOICE_DEFAULT_VALUE;
		cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

    //Trace_Log_Buf_Print("cur_set.dial_code=%d",cur_set.dial_code );
    return (cur_set.dial_code);
}
PUBLIC uint8 MMIAPISET_GetTTSCode_type( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
   		cur_set.tts_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  
		cur_set.dial_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
		cur_set.cc_code = 1; 
		//cur_set.hour_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  
		cur_set.time_code = MMI_DIAL_VOICE_DEFAULT_VALUE;
		cur_set.kl_code = MMI_DIAL_VOICE_DEFAULT_VALUE;
		cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;	
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

     // Trace_Log_Buf_Print("cur_set.tts_code=%d",cur_set.tts_code );
    return (cur_set.tts_code);
}

PUBLIC uint8 MMIAPISET_GetCCCode_type( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
      	cur_set.tts_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  
		cur_set.dial_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
		cur_set.cc_code = 1; 
		//cur_set.hour_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  
		cur_set.time_code = MMI_DIAL_VOICE_DEFAULT_VALUE;
		cur_set.kl_code = MMI_DIAL_VOICE_DEFAULT_VALUE;
		cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

    //Trace_Log_Buf_Print("cur_set.dial_code=%d",cur_set.cc_code );
    return (cur_set.cc_code);
}

PUBLIC uint8 MMIAPISET_GetHourCode_type( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
		cur_set.tts_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  
		cur_set.dial_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
		cur_set.cc_code = 1; 
		//cur_set.hour_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  
		cur_set.time_code = MMI_DIAL_VOICE_DEFAULT_VALUE;
		cur_set.kl_code = MMI_DIAL_VOICE_DEFAULT_VALUE;
		cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

  ///  Trace_Log_Buf_Print("cur_set.dial_code=%d",cur_set.cc_code );
 //   return (cur_set.hour_code);
}


PUBLIC uint8 MMIAPISET_GetTimeCode_type( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
		cur_set.tts_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  
		cur_set.dial_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
		cur_set.cc_code = 1; 
		//cur_set.hour_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  
		cur_set.time_code = MMI_DIAL_VOICE_DEFAULT_VALUE;
		cur_set.kl_code = MMI_DIAL_VOICE_DEFAULT_VALUE;	
		cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

  ///  Trace_Log_Buf_Print("cur_set.dial_code=%d",cur_set.cc_code );
    return (cur_set.time_code);
}

PUBLIC uint8 MMIAPISET_SetTimeCode_type(uint timecode )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);
	cur_set.time_code = timecode;
	 MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);

}



PUBLIC uint8 MMIAPISET_GetKlCode_type( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
		cur_set.tts_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  
		cur_set.dial_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
		cur_set.cc_code = 1; 
		//cur_set.hour_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  
		cur_set.time_code = MMI_DIAL_VOICE_DEFAULT_VALUE;
		cur_set.kl_code = MMI_DIAL_VOICE_DEFAULT_VALUE;	
		cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

   /// Trace_Log_Buf_Print("cur_set.kl_code=%d",cur_set.kl_code );
    return (cur_set.kl_code);
}

PUBLIC uint8 MMIAPISET_GetTTSVol( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
		cur_set.tts_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  
		cur_set.dial_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
		cur_set.cc_code = 1; 
		//cur_set.hour_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  
		cur_set.kl_code = MMI_DIAL_VOICE_DEFAULT_VALUE;	
		cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

   /// Trace_Log_Buf_Print("cur_set.dial_code=%d",cur_set.tts_vol );
    return (cur_set.tts_vol);
}

static void MMISET_TTSVOICEResult(MMISET_TTS_CODE_INFO_T cur_item)
{
    MMINV_WRITE(MMINV_SET_TTS_STYLE, &cur_item);
}



LOCAL void MMIAPISET_DialCodeTextId(                                    
                                                MMI_TEXT_ID_T        text_id,
                                                MMI_TEXT_ID_T        left_softkey_id,
                                                MMI_TEXT_ID_T        middle_softkey_id,
                                                MMI_TEXT_ID_T        right_softkey_id,
                                                MMI_CTRL_ID_T        ctrl_id,
                                                GUIITEM_STYLE_E      item_style
                                                )
{
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    
    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    GUILIST_AppendItem( ctrl_id, &item_t );      
}



LOCAL void  SetTTSCode(void)
{
    uint8 loop = 0;
    uint16 cur_selection =0;
    uint32 text_id[] =
    {
        TXT_OPEN,
        TXT_CLOSE
    };

    GUIFORM_SetStyle(MMISET_MENU_CODE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMISET_MENU_CODE_LABEL_CTRL_ID,TXT_SET_TTS_CODE,FALSE);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_MENU_CODE_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_MENU_CODE_SETLIST_CTRL_ID, text_id[loop]);
    }

    cur_selection =MMIAPISET_GetTTSCode_type() ;//读取nv中当前环境的设置
    GUISETLIST_SetCtrlState(MMISET_MENU_CODE_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMISET_MENU_CODE_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetTitleTextId(MMISET_MENU_CODE_SETLIST_CTRL_ID,TXT_SET_TTS_CODE);	
}

LOCAL void  SetDialCode(void)
{
    uint8 loop = 0;
    uint16 cur_selection =0;
    uint32 text_id[] =
    {
        TXT_OPEN,
        TXT_CLOSE
    };

    GUIFORM_SetStyle(MMISET_DIAL_CODE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMISET_DIAL_CODE_LABEL_CTRL_ID,TXT_SET_DIAL_CODE,FALSE);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_DIAL_CODE_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_DIAL_CODE_SETLIST_CTRL_ID, text_id[loop]);
    }

    cur_selection =MMIAPISET_GetDIALVOICE_type() ;//读取nv中当前环境的设置
    GUISETLIST_SetCtrlState(MMISET_DIAL_CODE_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMISET_DIAL_CODE_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetTitleTextId(MMISET_DIAL_CODE_SETLIST_CTRL_ID,TXT_SET_DIAL_CODE);	
}

LOCAL void  SetCcCode(void)
{
    uint8 loop = 0;
    uint16 cur_selection =0;
    uint32 text_id[] =
    {
        TXT_OPEN,
        TXT_CLOSE
    };

    GUIFORM_SetStyle(MMISET_CC_CODE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMISET_CC_CODE_LABEL_CTRL_ID,TXT_SET_CC_CODE,FALSE);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_CC_CODE_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_CC_CODE_SETLIST_CTRL_ID, text_id[loop]);
    }

    cur_selection =MMIAPISET_GetCCCode_type() ;//读取nv中当前环境的设置
    GUISETLIST_SetCtrlState(MMISET_CC_CODE_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMISET_CC_CODE_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetTitleTextId(MMISET_CC_CODE_SETLIST_CTRL_ID,TXT_SET_CC_CODE);	
}


LOCAL void  SetHourCode(void)
{
    uint8 loop = 0;
    uint16 cur_selection =0;
	
    uint32 text_id[] =
    {
        TXT_TIME_DAYMODE,
		TXT_TIME_ALLDAYMODE,	
        TXT_CLOSE
    };

    GUIFORM_SetStyle(MMISET_ALARM_CODE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMISET_ALARM_CODE_LABEL_CTRL_ID,TXT_SET_HOUR_CODE,FALSE);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_ALARM_CODE_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_ALARM_CODE_SETLIST_CTRL_ID, text_id[loop]);
    }
    cur_selection =MMIAPISET_GetHourCode_type() ;//读取nv中当前环境的设置
    GUISETLIST_SetCtrlState(MMISET_ALARM_CODE_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMISET_ALARM_CODE_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetTitleTextId(MMISET_ALARM_CODE_SETLIST_CTRL_ID,TXT_SET_HOUR_CODE);	
}

LOCAL void  SetTimeCode(void)
{
    uint8 loop = 0;
    uint16 cur_selection =0;
    uint32 text_id[] =
    {
        TXT_OPEN,
        TXT_CLOSE
    };

    GUIFORM_SetStyle(MMISET_TIME_CODE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMISET_TIME_CODE_LABEL_CTRL_ID,TXT_SET_TIME_CODE,FALSE);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_TIME_CODE_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_TIME_CODE_SETLIST_CTRL_ID, text_id[loop]);
    }
    cur_selection =MMIAPISET_GetTimeCode_type() ;//读取nv中当前环境的设置
    GUISETLIST_SetCtrlState(MMISET_TIME_CODE_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMISET_TIME_CODE_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetTitleTextId(MMISET_TIME_CODE_SETLIST_CTRL_ID,TXT_SET_TIME_CODE);	
}


LOCAL void  SetKlCode(void)
{
    uint8 loop = 0;
    uint16 cur_selection =0;
    uint32 text_id[] =
    {
        TXT_OPEN,
        TXT_CLOSE
    };

    GUIFORM_SetStyle(MMISET_KL_CODE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMISET_KL_CODE_LABEL_CTRL_ID,TXT_SET_KL_CODE,FALSE);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_KL_CODE_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_KL_CODE_SETLIST_CTRL_ID, text_id[loop]);
    }
    cur_selection =MMIAPISET_GetKlCode_type() ;//读取nv中当前环境的设置
    GUISETLIST_SetCtrlState(MMISET_KL_CODE_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMISET_KL_CODE_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetTitleTextId(MMISET_KL_CODE_SETLIST_CTRL_ID,TXT_SET_KL_CODE);	
}


LOCAL void  SetTTsVolParam(void)
{
	uint8                      s_cur_vol   =   0;
	uint16                     i           =   0;
	MMI_STRING_T               text        =   {0};
	wchar                      ch[3]       =   {0};
	GUIFORM_SetStyle(MMISET_TTS_VOL_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);  
	///GUIEDIT_SetStyle(MMISET_TTS_VOL_EDIT_CTRL_ID, GUIEDIT_STYLE_SINGLE);


	s_cur_vol =MMIAPISET_GetTTSVol() ;
	SCI_MEMSET(ch, 0, sizeof(ch));  
	if(s_cur_vol < 10)
	{
		ch[0] = s_cur_vol + '0';
		text.wstr_ptr = ch;
		text.wstr_len = 1;
	}
	else
	{
		ch[1] = (s_cur_vol-10) + '0';
		ch[0] = '1';
		text.wstr_ptr = ch;
		text.wstr_len = 2;
	}
	GUILABEL_SetText(MMISET_TTS_VOL_EDIT_CTRL_ID, &text, FALSE);
	//GUIEDIT_SetString(MMISET_TTS_VOL_EDIT_CTRL_ID,text.wstr_ptr,1);
	//GUILABEL_SetText(MMISET_TTS_VOL_EDIT_LABEL_CTRL_ID, &text, FALSE);
	GUILABEL_SetTextById(MMISET_TTS_VOL_LABEL_CTRL_ID, TXT_SET_TTS_VOL, FALSE);




}
LOCAL void SaveTTsVol(uint8 vol)
{
	//MMISET_TTS_CODE_INFO_T	tts_info = {0};
	
	tts_info.tts_vol = vol;

	MMISET_TTSVOICEResult(tts_info);

}


LOCAL void SaveTTsTypeSet(void)
{

//	MMISET_TTS_CODE_INFO_T	tts_info = {0};
	uint16		   tts_code =  0;
	uint16		 dial_code		 =	0;
	uint16	cc_code 	=  0;
	uint16	hour_code 	=  0;
	uint16	time_code 	=  0;
	uint16	kl_code 	=  0;
	uint8   tts_vol[2] = {0};
	uint8 tts_vol_int = 0;
	MMI_STRING_T str_vol = {0};
	
	tts_code = (uint16)GUISETLIST_GetCurIndex(MMISET_MENU_CODE_SETLIST_CTRL_ID);
	tts_info.tts_code = tts_code;
	//MMISET_TTSVOICEResult(tts_info);

	dial_code	 =	 (uint16)GUISETLIST_GetCurIndex(MMISET_DIAL_CODE_SETLIST_CTRL_ID);
	tts_info.dial_code = dial_code;

	cc_code    =   (uint16)GUISETLIST_GetCurIndex(MMISET_CC_CODE_SETLIST_CTRL_ID);
	tts_info.cc_code = cc_code; 
	
	//hour_code    =   (uint16)GUISETLIST_GetCurIndex(MMISET_ALARM_CODE_SETLIST_CTRL_ID);
	//tts_info.hour_code = hour_code; 

	time_code    =   (uint16)GUISETLIST_GetCurIndex(MMISET_TIME_CODE_SETLIST_CTRL_ID);
	tts_info.time_code = time_code; 

	kl_code    =   (uint16)GUISETLIST_GetCurIndex(MMISET_KL_CODE_SETLIST_CTRL_ID);
	tts_info.kl_code = kl_code; 


	tts_vol_int =MMIAPISET_GetTTSVol() ;
	//GUIEDIT_GetString(MMISET_TTS_VOL_EDIT_CTRL_ID, &str_vol);	
	//MMIAPICOM_WstrToStr(str_vol.wstr_ptr,tts_vol);
	//tts_vol_int = (uint8)MMIAPICOM_StrToInt(tts_vol, 1);
	tts_info.tts_vol = tts_vol_int;
	
	MMISET_TTSVOICEResult(tts_info);


// MMISMS_SetTTSVol((uint8)para_ptr->cur_value);
}

LOCAL void SetTTsVolumeCB(MMICOM_PANEL_CALLBACK_T *para_ptr) 
{
	
    if(PNULL == para_ptr) return;
    
  //  MMIENVSET_SetCurModeOptValue(s_ring_type, para_ptr->cur_value, MN_DUAL_SYS_1);

	tts_info.tts_vol = para_ptr->cur_value;
   	MMISET_TTSVOICEResult(tts_info);
    //display the vol


     if (MMK_IsOpenWin(MMIENVSET_DIAL_RING_TYPE_WIN_ID))
     {
         SetTTsVolParam();
		 MMK_PostMsg(MMIENVSET_DIAL_RING_TYPE_WIN_ID, MSG_FULL_PAINT, PNULL, NULL);
     }

    return;
}


LOCAL     MMI_RESULT_E  HandleDialRingTypeWindow(
                                                MMI_WIN_ID_T    win_id,  // 窗口ID
                                                MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                DPARAM             param    // 消息参数
                                                )
{
   
  	MMI_RESULT_E     recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	MMI_CTRL_ID_T   ctrl_id  = 0;
	uint16					 max_item = 2; 
	uint8  str_tts_vol[2] = {0};
	uint8 tts_vol = 0 ;
	MMI_STRING_T net_freq = {0};
	MMICOM_PANEL_INFO_T       panel_info = {0};
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
		GUIFORM_SetType(MMISET_TTS_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif      
		SetTTSCode();
		SetDialCode();
		SetCcCode();
		//SetHourCode();
		SetTimeCode();
		SetKlCode();
		SetTTsVolParam();
		MMK_SetAtvCtrl(win_id, MMISET_MENU_CODE_SETLIST_CTRL_ID);

		break;
	  case MSG_FULL_PAINT:
//        if(!MMIAPICOM_IsPanelOpened())
//        {
//            MMIAPISET_StopAppRing();
//        }
        break;
#if 		1
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
	{
			 if(MSG_CTL_PENOK == msg_id)
        {
            ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
        }
        else
        {
            ctrl_id= MMK_GetActiveCtrlId(win_id);
        }

        if(MMIAPICC_IsInState(CC_IN_CALL_STATE) && MMISET_TTS_VOL_EDIT_CTRL_ID == ctrl_id)
        {
            MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
            break;
        }
        switch(ctrl_id)
        {
       
        case MMISET_TTS_VOL_EDIT_CTRL_ID://闹钟
         
            panel_info.min_value = MMISET_VOL_ZERO;
            panel_info.max_value = MMISET_VOL_MAX;
            break;

        default:
            panel_info.min_value = MMISET_VOL_ZERO;
            panel_info.max_value = MMISET_VOL_MAX;
            break;
        }

   
        {
            //s_dual_sys = MMIAPISET_GetActiveSim();
            panel_info.x = -1;
            panel_info.y = -1;
			panel_info.cur_value = MMIAPISET_GetTTSVol();
			panel_info.last_value = MMIAPISET_GetTTSVol();
            //panel_info.min_value = MMISET_VOL_ZERO;
            //panel_info.max_value = MMISET_VOL_MAX;
            panel_info.set_callback = SetTTsVolumeCB;
     
            if(MMIAPICOM_IsPanelOpened()) 
            {
                MMIAPICOM_ClosePanelChildWin();
            }
            MMIAPICOM_OpenPanelChildWin(MMK_GetFocusWinId(), &panel_info);
         

        }

        //MMK_CreateWin((uint32 *)MMIENVSET_RING_VOL_WIN_TAB,(ADD_DATA)menu_id);
 
		}
		
		break;
#endif		

	 
	case MSG_GET_FOCUS://for cr NEWMS00131734
		SaveTTsTypeSet();
		break;	
	case MSG_APP_OK:
	case MSG_CTL_OK:	
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	    SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
		
	case MSG_APP_RED:
      //  SaveTTsTypeSet();
        recode = MMI_RESULT_FALSE;
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
   
}


// 拨号音的选择窗口
WINDOW_TABLE(MMIENVSET_DIAL_RING_TYPE_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)HandleDialRingTypeWindow),
        //win_id
	WIN_ID(MMIENVSET_DIAL_RING_TYPE_WIN_ID),
	//窗口的标题
	WIN_TITLE(TXT_SET_RING_TTS),
        //创建  控件
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_TTS_FORM_CTRL_ID),

	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISET_MENU_CODE_FORM_CTRL_ID, MMISET_TTS_FORM_CTRL_ID),
	CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_MENU_CODE_LABEL_CTRL_ID, MMISET_MENU_CODE_FORM_CTRL_ID),
	CHILD_SETLIST_CTRL(TRUE, MMISET_MENU_CODE_SETLIST_CTRL_ID, MMISET_MENU_CODE_FORM_CTRL_ID),

	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISET_DIAL_CODE_FORM_CTRL_ID, MMISET_TTS_FORM_CTRL_ID),
	CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_DIAL_CODE_LABEL_CTRL_ID, MMISET_DIAL_CODE_FORM_CTRL_ID),
	CHILD_SETLIST_CTRL(TRUE, MMISET_DIAL_CODE_SETLIST_CTRL_ID, MMISET_DIAL_CODE_FORM_CTRL_ID),

	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISET_CC_CODE_FORM_CTRL_ID, MMISET_TTS_FORM_CTRL_ID),
	CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_CC_CODE_LABEL_CTRL_ID, MMISET_CC_CODE_FORM_CTRL_ID),
	CHILD_SETLIST_CTRL(TRUE, MMISET_CC_CODE_SETLIST_CTRL_ID, MMISET_CC_CODE_FORM_CTRL_ID),

	//CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISET_ALARM_CODE_FORM_CTRL_ID, MMISET_TTS_FORM_CTRL_ID),
//	CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_ALARM_CODE_LABEL_CTRL_ID, MMISET_ALARM_CODE_FORM_CTRL_ID),
	///CHILD_SETLIST_CTRL(TRUE, MMISET_ALARM_CODE_SETLIST_CTRL_ID, MMISET_ALARM_CODE_FORM_CTRL_ID),


	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISET_TIME_CODE_FORM_CTRL_ID, MMISET_TTS_FORM_CTRL_ID),
	CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_TIME_CODE_LABEL_CTRL_ID, MMISET_TIME_CODE_FORM_CTRL_ID),
	CHILD_SETLIST_CTRL(TRUE, MMISET_TIME_CODE_SETLIST_CTRL_ID, MMISET_TIME_CODE_FORM_CTRL_ID),

	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISET_KL_CODE_FORM_CTRL_ID, MMISET_TTS_FORM_CTRL_ID),
	CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_KL_CODE_LABEL_CTRL_ID, MMISET_KL_CODE_FORM_CTRL_ID),
	CHILD_SETLIST_CTRL(TRUE, MMISET_KL_CODE_SETLIST_CTRL_ID, MMISET_KL_CODE_FORM_CTRL_ID),

	//  CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_DIAL_CODE_SETLIST_CTRL_ID),

	//CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISET_TTS_VOL_FORM_CTRL_ID, MMISET_TTS_FORM_CTRL_ID),
	//CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_TTS_VOL_LABEL_CTRL_ID, MMISET_TTS_VOL_FORM_CTRL_ID),
	//CHILD_EDIT_DIGITAL_CTRL( TRUE,1, MMISET_TTS_VOL_EDIT_CTRL_ID, MMISET_TTS_VOL_FORM_CTRL_ID),
	
	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISET_TTS_VOL_FORM_CTRL_ID, MMISET_TTS_FORM_CTRL_ID),
	CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_TTS_VOL_LABEL_CTRL_ID, MMISET_TTS_VOL_FORM_CTRL_ID),
	CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT, TRUE, MMISET_TTS_VOL_EDIT_CTRL_ID, MMISET_TTS_VOL_FORM_CTRL_ID),

		
        
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),   
	//结束宏定义
        END_WIN
};
PUBLIC void MMIAPIST_OpenTTSWin(void)
{
    MMK_CreateWin((uint32*)MMIENVSET_DIAL_RING_TYPE_WIN_TAB, PNULL);
}

PUBLIC void MMIAPIST_ResetTTSWin(void)
{
	MMISET_TTS_CODE_INFO_T           cur_set = {0};   
	MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

	cur_set.tts_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.dial_code = MMI_DIAL_VOICE_DEFAULT_VALUE;	
	cur_set.cc_code = 1; 
	//cur_set.hour_code = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.time_code = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.kl_code = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;

	MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);

}
#endif
PUBLIC MMI_RESULT_E SetPointtimeTimeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL   MMI_RESULT_E  HandleEditPointTimeWindow(
                                                MMI_WIN_ID_T    win_id,  // 窗口ID
                                                MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                DPARAM             param    // 消息参数
                                                );
LOCAL     MMI_RESULT_E  SetPointtimeWinHandleMsg(
                                                MMI_WIN_ID_T    win_id,  // 窗口ID
                                                MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                DPARAM             param    // 消息参数
                                                );
LOCAL MMI_RESULT_E SetPointtimeDirectionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

PUBLIC MMI_RESULT_E Start_Time_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
PUBLIC MMI_RESULT_E End_Time_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);


WINDOW_TABLE(MMISET_SET_POINTTIME_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)SetPointtimeWinHandleMsg),
        //win_id
        WIN_ID(MMISET_POINTTIME_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ZDT_TIME_SWITCH),
        //创建  控件

  	 /// CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_POINTTIME_CTRL_ID),
       CREATE_MENU_CTRL(MENU_SET_POINT_TIME, MMISET_POINTTIME_CTRL_ID),
      
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};

WINDOW_TABLE(MMISET_SET_POINTTIME_DIRECTION_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)SetPointtimeDirectionWinHandleMsg),
        //win_id
        WIN_ID(MMISET_POINTTIME_DIRECTION_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_POINT_TIME_DIRECTION),
        //创建  控件

  	 /// CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_POINTTIME_CTRL_ID),
       ///CREATE_MENU_CTRL(MENU_SET_POINT_TIME, MMISET_POINTTIME_CTRL_ID),
       CREATE_TEXT_CTRL(MMISET_POINTTIME_DIRECTION_CTRL_ID),         
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};



WINDOW_TABLE(MMISET_SET_POINTTIME_TIME_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)SetPointtimeTimeWinHandleMsg),
        //win_id
        WIN_ID(MMISET_POINTTIME_TIME_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_POINT_TIME_SETTING),
        //创建  控件

  	  CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_POINTTIME_TIME_CTRL_ID),

      
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};


WINDOW_TABLE(MMISET_EDIT_POINTTIME_TIME_WIN_TAB) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)HandleEditPointTimeWindow),
        //win_id
	WIN_ID(MMISET_EDIT_POINTTIME_TIME_WIN_ID),
	//窗口的标题
	WIN_TITLE(TXT_BYCUSTOM_SET),
        //创建  控件
	   CREATE_MENU_CTRL(MENU_SET_CUSTOMIZE_TIME, MMISET_PHONE_CUSTOMIZE_TIME_CTRL_ID),
		
        
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),   
	//结束宏定义
        END_WIN
};

WINDOW_TABLE(MMISET_START_TIME_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)Start_Time_HandleMsg),
        //win_id
        WIN_ID(MMISET_START_TIME_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_POINT_TIME_START),
        //创建  控件
  	  CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_TIME_START_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};


WINDOW_TABLE(MMISET_END_TIME_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)End_Time_HandleMsg),
        //win_id
        WIN_ID(MMISET_END_TIME_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_POINT_TIME_END),
        //创建  控件
  	  CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_TIME_END_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};


LOCAL void MMIAPISET_DialCodeTextId(                                    
                                                MMI_TEXT_ID_T        text_id,
                                                MMI_TEXT_ID_T        left_softkey_id,
                                                MMI_TEXT_ID_T        middle_softkey_id,
                                                MMI_TEXT_ID_T        right_softkey_id,
                                                MMI_CTRL_ID_T        ctrl_id,
                                                GUIITEM_STYLE_E      item_style
                                                )
{
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    
    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    GUILIST_AppendItem( ctrl_id, &item_t );      
}



PUBLIC uint8 ZDT_GetTTSSwitch_type( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
	cur_set.tts_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.dial_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
	cur_set.menu_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.onekey_time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.cc_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.pb_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.newsms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.sms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
	cur_set.charge_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.kl_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

    //Trace_Log_Buf_Print("cur_set.dial_code=%d",cur_set.cc_code );
    return (cur_set.tts_switch);
}
PUBLIC uint8 ZDT_GetDialSwitch_type( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
	cur_set.tts_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.dial_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
	cur_set.menu_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.onekey_time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.cc_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.pb_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.newsms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.sms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
	cur_set.charge_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.kl_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

    //Trace_Log_Buf_Print("cur_set.dial_code=%d",cur_set.cc_code );
    return (cur_set.dial_switch);
}
PUBLIC uint8 ZDT_GetMenuSwitch_type( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
	cur_set.tts_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.dial_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
	cur_set.menu_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.onekey_time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.cc_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.pb_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.newsms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.sms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
	cur_set.charge_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.kl_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

    //Trace_Log_Buf_Print("cur_set.dial_code=%d",cur_set.cc_code );
    return (cur_set.menu_switch);
}
PUBLIC uint8 ZDT_GetOnekeytimeSwitch_type( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
	cur_set.tts_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.dial_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
	cur_set.menu_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.onekey_time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.cc_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.pb_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.newsms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.sms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
	cur_set.charge_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.kl_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

    //Trace_Log_Buf_Print("cur_set.dial_code=%d",cur_set.cc_code );
    return (cur_set.onekey_time_switch);
}
PUBLIC uint8 ZDT_GetCCSwitch_type( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
	cur_set.tts_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.dial_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
	cur_set.menu_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.onekey_time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.cc_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.pb_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.newsms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.sms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
	cur_set.charge_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.kl_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

    //Trace_Log_Buf_Print("cur_set.dial_code=%d",cur_set.cc_code );
    return (cur_set.cc_switch);
}
PUBLIC uint8 ZDT_GetPBSwitch_type( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
	cur_set.tts_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.dial_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
	cur_set.menu_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.onekey_time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.cc_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.pb_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.newsms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.sms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
	cur_set.charge_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.kl_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

    //Trace_Log_Buf_Print("cur_set.dial_code=%d",cur_set.cc_code );
    return (cur_set.pb_switch);
}
PUBLIC uint8 ZDT_GetNewSmsSwitch_type( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
	cur_set.tts_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.dial_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
	cur_set.menu_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.onekey_time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.cc_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.pb_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.newsms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.sms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
	cur_set.charge_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.kl_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

    //Trace_Log_Buf_Print("cur_set.dial_code=%d",cur_set.cc_code );
    return (cur_set.newsms_switch);
}
PUBLIC uint8 ZDT_GetSmsSwitch_type( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
	cur_set.tts_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.dial_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
	cur_set.menu_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.onekey_time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.cc_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.pb_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.newsms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.sms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
	cur_set.charge_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.kl_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

    //Trace_Log_Buf_Print("cur_set.dial_code=%d",cur_set.cc_code );
    return (cur_set.sms_switch);
}

PUBLIC uint8 ZDT_GetChargeSwitch_type( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
	cur_set.tts_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.dial_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
	cur_set.menu_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.onekey_time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.cc_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.pb_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.newsms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.sms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
	cur_set.charge_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.kl_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

    //Trace_Log_Buf_Print("cur_set.dial_code=%d",cur_set.cc_code );
    return (cur_set.charge_switch);
}


PUBLIC uint8 ZDT_GetKLSwitch_type( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
	cur_set.tts_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.dial_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
	cur_set.menu_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.onekey_time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.cc_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.pb_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.newsms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.sms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
	cur_set.charge_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.kl_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

    //Trace_Log_Buf_Print("cur_set.dial_code=%d",cur_set.cc_code );
    return (cur_set.kl_switch);
}

PUBLIC uint8 ZDT_GetVol_Num( void )
{
    MMISET_TTS_CODE_INFO_T           cur_set = {0};   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
	cur_set.tts_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.dial_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
	cur_set.menu_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.onekey_time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.cc_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.pb_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.newsms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.sms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
	cur_set.charge_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.kl_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

    //Trace_Log_Buf_Print("cur_set.dial_code=%d",cur_set.cc_code );
    return (cur_set.tts_vol);
}


LOCAL void  SetTTSSwitchCode( MMI_CTRL_ID_T ctrl_id)

{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, 2, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,2);
    //append item
    MMIAPISET_DialCodeTextId(TXT_OPEN,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_CLOSE,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	if( 0 == ZDT_GetTTSSwitch_type())//读取nv中当前环境的设置
	{
		cur_selection = 0;
	}
	else
	{
		cur_selection = 1;
	}
	GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

   GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}
LOCAL void SaveTTSSwitchCode( MMI_CTRL_ID_T ctrl_id)
{
	MMISET_TTS_CODE_INFO_T  cur_set ={0};
	MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
	uint8  tts_switch = 0;
    tts_switch =(uint16)GUILIST_GetCurItemIndex(MMISET_TTS_SWITCH_CTRL_ID);
	 MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);
	 cur_set.tts_switch = tts_switch;
    MMINV_WRITE(MMINV_SET_TTS_STYLE, &cur_set); 
	
}
PUBLIC MMI_RESULT_E TTS_Switch_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   
	MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	MMI_HANDLE_T	ctrl_id = MMISET_TTS_SWITCH_CTRL_ID;
	uint16					 max_item = 2; 
	uint8  tts_vol[2] = {0};
		MMI_STRING_T net_freq = {0};
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
	  SetTTSSwitchCode(ctrl_id);
	  MMK_SetAtvCtrl(win_id, MMISET_TTS_SWITCH_CTRL_ID);
		break;
			
	  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	//	SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
	  case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
			SaveTTSSwitchCode(ctrl_id);
			  MMK_CloseWin(win_id);
		break;	
	 
		
	case MSG_APP_RED:
		//SaveTTsTypeSet();
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}


WINDOW_TABLE(MMISET_TTS_SWITCH_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)TTS_Switch_HandleMsg),
        //win_id
        WIN_ID(MMISET_TTS_SWITCH_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ZDT_TTS_SWITCH),
        //创建  控件
  	  CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_TTS_SWITCH_CTRL_ID),
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};


LOCAL void  SetDialSwitchCode( MMI_CTRL_ID_T ctrl_id)

{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, 2, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,2);
    //append item
    MMIAPISET_DialCodeTextId(TXT_OPEN,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_CLOSE,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	if( 0 == ZDT_GetDialSwitch_type())//读取nv中当前环境的设置
	{
		cur_selection = 0;
	}
	else
	{
		cur_selection = 1;
	}
	GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

   GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}
LOCAL void SaveDialSwitchCode( MMI_CTRL_ID_T ctrl_id)
{
	MMISET_TTS_CODE_INFO_T  cur_set ={0};
	MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
	uint8  dial_switch = 0;
    dial_switch =(uint16)GUILIST_GetCurItemIndex(MMISET_DIAL_SWITCH_CTRL_ID);
	 MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);
	 cur_set.dial_switch = dial_switch;
    MMINV_WRITE(MMINV_SET_TTS_STYLE, &cur_set); 
	
}
PUBLIC MMI_RESULT_E Dial_Switch_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   
	MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	MMI_HANDLE_T	ctrl_id = MMISET_DIAL_SWITCH_CTRL_ID;
	uint16					 max_item = 2; 
	uint8  tts_vol[2] = {0};
		MMI_STRING_T net_freq = {0};
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
	  SetDialSwitchCode(ctrl_id);
	  MMK_SetAtvCtrl(win_id, MMISET_DIAL_SWITCH_CTRL_ID);
		break;
			
	  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	//	SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
	  case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
			SaveDialSwitchCode(ctrl_id);
			  MMK_CloseWin(win_id);
		break;	
	 
		
	case MSG_APP_RED:
		//SaveTTsTypeSet();
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}


WINDOW_TABLE(MMISET_DIAL_SWITCH_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)Dial_Switch_HandleMsg),
        //win_id
        WIN_ID(MMISET_DIAL_SWITCH_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ZDT_DIAL_SWITCH),
        //创建  控件
  	  CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_DIAL_SWITCH_CTRL_ID),
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};

LOCAL void  SetMenuSwitchCode( MMI_CTRL_ID_T ctrl_id)

{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, 2, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,2);
    //append item
    MMIAPISET_DialCodeTextId(TXT_OPEN,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_CLOSE,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	if( 0 == ZDT_GetMenuSwitch_type())//读取nv中当前环境的设置
	{
		cur_selection = 0;
	}
	else
	{
		cur_selection = 1;
	}
	GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

   GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}
LOCAL void SaveMenuSwitchCode( MMI_CTRL_ID_T ctrl_id)
{
	MMISET_TTS_CODE_INFO_T  cur_set ={0};
	MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
	uint8  menu_switch = 0;
    menu_switch =(uint16)GUILIST_GetCurItemIndex(MMISET_MENU_SWITCH_CTRL_ID);
	 MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);
	 cur_set.menu_switch = menu_switch;
    MMINV_WRITE(MMINV_SET_TTS_STYLE, &cur_set); 
	
}
PUBLIC MMI_RESULT_E Menu_Switch_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   
	MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	MMI_HANDLE_T	ctrl_id = MMISET_MENU_SWITCH_CTRL_ID;
	uint16					 max_item = 2; 
	uint8  tts_vol[2] = {0};
		MMI_STRING_T net_freq = {0};
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
	  SetMenuSwitchCode(ctrl_id);
	  MMK_SetAtvCtrl(win_id, MMISET_MENU_SWITCH_CTRL_ID);
		break;
			
	  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	//	SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
	  case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
			SaveMenuSwitchCode(ctrl_id);
			MMK_CloseWin(win_id);
		break;	
	 
		
	case MSG_APP_RED:
		//SaveTTsTypeSet();
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}


WINDOW_TABLE(MMISET_MENU_SWITCH_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)Menu_Switch_HandleMsg),
        //win_id
        WIN_ID(MMISET_MENU_SWITCH_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ZDT_MENU_SWITCH),
        //创建  控件
  	  CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_MENU_SWITCH_CTRL_ID),
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};

PUBLIC MMI_RESULT_E Onekeytime_Switch_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E SetOnekeytimeDirectionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
WINDOW_TABLE(MMISET_ONEKEYTIME_SWITCH_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)Onekeytime_Switch_HandleMsg),
        //win_id
        WIN_ID(MMISET_ONEKEYTIME_SWITCH_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ZDT_ONEKEY_TIME_SWITCH),
        //创建  控件
  	  CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_ONEKEYTIME_SWITCH_CTRL_ID),
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};

WINDOW_TABLE(MMISET_SET_ONEKEYTIME_DIRECTION_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)SetOnekeytimeDirectionWinHandleMsg),
        //win_id
        WIN_ID(MMISET_ONEKEYTIME_DIRECTION_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ONEKEY_TIME_DIRECTION),
        //创建  控件

  	 /// CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_POINTTIME_CTRL_ID),
       ///CREATE_MENU_CTRL(MENU_SET_POINT_TIME, MMISET_POINTTIME_CTRL_ID),
       CREATE_TEXT_CTRL(MMISET_ONTKEYTIME_DIRECTION_CTRL_ID),         
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};


LOCAL void  SetOneKeyTimeSwitchCode( MMI_CTRL_ID_T ctrl_id)

{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, 2, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,2);
    //append item
    MMIAPISET_DialCodeTextId(TXT_OPEN,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_CLOSE,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	if( 0 == ZDT_GetOnekeytimeSwitch_type())//读取nv中当前环境的设置
	{
		cur_selection = 0;
	}
	else
	{
		cur_selection = 1;
	}
	GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

   GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}
LOCAL void SaveOnekeytimeSwitchCode( MMI_CTRL_ID_T ctrl_id)
{
	MMISET_TTS_CODE_INFO_T  cur_set ={0};
	MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
	uint8  onekeytime_switch = 0;
    onekeytime_switch =(uint16)GUILIST_GetCurItemIndex(MMISET_ONEKEYTIME_SWITCH_CTRL_ID);
	 MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);
	 cur_set.onekey_time_switch= onekeytime_switch;
    MMINV_WRITE(MMINV_SET_TTS_STYLE, &cur_set); 
	
}
PUBLIC MMI_RESULT_E Onekeytime_Switch_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   
	MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	MMI_HANDLE_T	ctrl_id = MMISET_ONEKEYTIME_SWITCH_CTRL_ID;
	uint16					 max_item = 2; 
	uint8  tts_vol[2] = {0};
		MMI_STRING_T net_freq = {0};
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
	  SetOneKeyTimeSwitchCode(ctrl_id);
	  MMK_SetAtvCtrl(win_id, MMISET_ONEKEYTIME_SWITCH_CTRL_ID);
		break;
			
	  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	//	SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
	  case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
			SaveOnekeytimeSwitchCode(ctrl_id);
			  MMK_CloseWin(win_id);
		break;	
	 
		
	case MSG_APP_RED:
		//SaveTTsTypeSet();
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}

LOCAL     MMI_RESULT_E  SetOnekeytimeWinHandleMsg(
                                                MMI_WIN_ID_T    win_id,  // 窗口ID
                                                MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                DPARAM             param    // 消息参数
                                                )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIMENU_GetId(MMISET_ONTKEYTIME_CTRL_ID,&group_id,&menu_id);
        
            MMK_SetAtvCtrl(win_id, MMISET_ONTKEYTIME_CTRL_ID);
            break;

        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            GUIMENU_GetId(MMISET_ONTKEYTIME_CTRL_ID,&group_id,&menu_id);
            switch (menu_id)
            {

                case ID_ZDT_ONEKEY_TIME:
                    MMK_CreateWin((uint32*)MMISET_ONEKEYTIME_SWITCH_WIN_TAB,PNULL);
                    break;

                case ID_ZDT_ONEKEY_DIRECTION://
                    MMK_CreateWin((uint32*)MMISET_SET_ONEKEYTIME_DIRECTION_WIN_TAB,PNULL);
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

LOCAL MMI_RESULT_E SetOnekeytimeDirectionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_STRING_T            text = {0};
	MMI_RESULT_E            result = MMI_RESULT_TRUE;
	GUI_RECT_T                rect = {0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:
	{

		MMI_STRING_T	text_s = {0};
		
		uint16			uint16_str_len = 0;
		char*		   temp_ptr = PNULL;
		uint16			temp_len = 0;
		GUI_FONT_T font = MMI_DEFAULT_BIG_FONT ;
		GUI_COLOR_T color = MMI_BLACK_COLOR ;//MMI_BACKGROUND_COLOR;
		
		MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ONEKEY_TIME_INFO,&text_s);
		
		GUITEXT_SetAlign(MMISET_ONTKEYTIME_DIRECTION_CTRL_ID,ALIGN_HVMIDDLE);
		GUITEXT_SetFont(MMISET_ONTKEYTIME_DIRECTION_CTRL_ID, &font, &color );
		GUITEXT_SetString(MMISET_ONTKEYTIME_DIRECTION_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, TRUE);
	}

        break;
    case MSG_FULL_PAINT:
        {	
		MMI_STRING_T	text_s = {0};

		
		uint16			uint16_str_len = 0;
		char*		   temp_ptr = PNULL;
		uint16			temp_len = 0;
		GUI_FONT_T font = MMI_DEFAULT_BIG_FONT ;
		GUI_COLOR_T color = MMI_BLACK_COLOR ;//MMI_BACKGROUND_COLOR;
		
		MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ONEKEY_TIME_INFO,&text_s);
		
		GUITEXT_SetAlign(MMISET_ONTKEYTIME_DIRECTION_CTRL_ID,ALIGN_HVMIDDLE);
		GUITEXT_SetFont(MMISET_ONTKEYTIME_DIRECTION_CTRL_ID, &font, &color );
		GUITEXT_SetString(MMISET_ONTKEYTIME_DIRECTION_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, TRUE);
	
            MMK_SetAtvCtrl(win_id, MMISET_ONTKEYTIME_DIRECTION_CTRL_ID);
          
        }
        break;
	case MSG_APP_OK:
	  case MSG_CTL_OK:
		 
		 MMK_CloseWin(win_id);
		   break;
	  case MSG_KEYDOWN_CANCEL:

	  case MSG_CTL_CANCEL:		  
				   MMK_CloseWin(win_id);
		   break;

	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}


WINDOW_TABLE(MMISET_SET_ONEKEYTIME_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)SetOnekeytimeWinHandleMsg),
        //win_id
        WIN_ID(MMISET_ONEKEYTIME_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ZDT_ONEKEY_TIME_SWITCH),
        //创建  控件

  	 /// CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_POINTTIME_CTRL_ID),
       CREATE_MENU_CTRL(MENU_SET_ONEKEY_TIME, MMISET_ONTKEYTIME_CTRL_ID),
      
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};





LOCAL void  SetCCSwitchCode( MMI_CTRL_ID_T ctrl_id)

{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, 3, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,3);
    //append item
	MMIAPISET_DialCodeTextId(TXT_TTS_INCOMING_1,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	///MMIAPISET_DialCodeTextId(TXT_TTS_INCOMING_5,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_TTS_INCOMING_999,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_TTS_INCOMING_0,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);


	cur_selection = ZDT_GetCCSwitch_type();//读取nv中当前环境的设置

	GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

   GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}
LOCAL void SaveCCSwitchCode( MMI_CTRL_ID_T ctrl_id)
{
	MMISET_TTS_CODE_INFO_T  cur_set ={0};
	MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
	uint8  cc_switch = 0;
    cc_switch =(uint16)GUILIST_GetCurItemIndex(MMISET_CC_SWITCH_CTRL_ID);
	 MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);
	 cur_set.cc_switch= cc_switch;
    MMINV_WRITE(MMINV_SET_TTS_STYLE, &cur_set); 
	
}
PUBLIC MMI_RESULT_E CC_Switch_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   
	MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	MMI_HANDLE_T	ctrl_id = MMISET_CC_SWITCH_CTRL_ID;
	uint16					 max_item = 2; 
	uint8  tts_vol[2] = {0};
		MMI_STRING_T net_freq = {0};
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
	  SetCCSwitchCode(ctrl_id);
	  MMK_SetAtvCtrl(win_id, MMISET_CC_SWITCH_CTRL_ID);
		break;
			
	  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	//	SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
	  case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
			SaveCCSwitchCode(ctrl_id);
			  MMK_CloseWin(win_id);
		break;	
	 
		
	case MSG_APP_RED:
		//SaveTTsTypeSet();
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}


WINDOW_TABLE(MMISET_CC_SWITCH_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)CC_Switch_HandleMsg),
        //win_id
        WIN_ID(MMISET_CC_SWITCH_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ZDT_CC_SWITCH),
        //创建  控件
  	  CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_CC_SWITCH_CTRL_ID),
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};

LOCAL void  SetPBSwitchCode( MMI_CTRL_ID_T ctrl_id)

{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, 2, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,2);
    //append item
    MMIAPISET_DialCodeTextId(TXT_OPEN,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_CLOSE,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	if( 0 == ZDT_GetPBSwitch_type())//读取nv中当前环境的设置
	{
		cur_selection = 0;
	}
	else
	{
		cur_selection = 1;
	}
	GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

   GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}
LOCAL void SavePBSwitchCode( MMI_CTRL_ID_T ctrl_id)
{
	MMISET_TTS_CODE_INFO_T  cur_set ={0};
	MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
	uint8  pb_switch = 0;
    pb_switch =(uint16)GUILIST_GetCurItemIndex(MMISET_PB_SWITCH_CTRL_ID);
	 MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);
	 cur_set.pb_switch= pb_switch;
    MMINV_WRITE(MMINV_SET_TTS_STYLE, &cur_set); 
	
}
PUBLIC MMI_RESULT_E PB_Switch_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   
	MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	MMI_HANDLE_T	ctrl_id = MMISET_PB_SWITCH_CTRL_ID;
	uint16					 max_item = 2; 
	uint8  tts_vol[2] = {0};
		MMI_STRING_T net_freq = {0};
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
	  SetPBSwitchCode(ctrl_id);
	  MMK_SetAtvCtrl(win_id, MMISET_PB_SWITCH_CTRL_ID);
		break;
			
	  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	//	SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
	  case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
			SavePBSwitchCode(ctrl_id);
			  MMK_CloseWin(win_id);
		break;	
	 
		
	case MSG_APP_RED:
		//SaveTTsTypeSet();
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}


WINDOW_TABLE(MMISET_PB_SWITCH_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)PB_Switch_HandleMsg),
        //win_id
        WIN_ID(MMISET_PB_SWITCH_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ZDT_PB_SWITCH),
        //创建  控件
  	  CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_PB_SWITCH_CTRL_ID),
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};

LOCAL void  SetNewSmsSwitchCode( MMI_CTRL_ID_T ctrl_id)

{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, 2, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,2);
    //append item
    MMIAPISET_DialCodeTextId(TXT_OPEN,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_CLOSE,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	if( 0 == ZDT_GetNewSmsSwitch_type())//读取nv中当前环境的设置
	{
		cur_selection = 0;
	}
	else
	{
		cur_selection = 1;
	}
	GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

   GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}
LOCAL void SaveNewSmsSwitchCode( MMI_CTRL_ID_T ctrl_id)
{
	MMISET_TTS_CODE_INFO_T  cur_set ={0};
	MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
	uint8  newsms_switch = 0;
    newsms_switch =(uint16)GUILIST_GetCurItemIndex(MMISET_NEWSMS_SWITCH_CTRL_ID);
	 MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);
	 cur_set.newsms_switch= newsms_switch;
    MMINV_WRITE(MMINV_SET_TTS_STYLE, &cur_set); 
	
}
PUBLIC MMI_RESULT_E NewSMS_Switch_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   
	MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	MMI_HANDLE_T	ctrl_id = MMISET_NEWSMS_SWITCH_CTRL_ID;
	uint16					 max_item = 2; 
	uint8  tts_vol[2] = {0};
		MMI_STRING_T net_freq = {0};
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
	  SetNewSmsSwitchCode(ctrl_id);
	  MMK_SetAtvCtrl(win_id, MMISET_NEWSMS_SWITCH_CTRL_ID);
		break;
			
	  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	//	SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
	  case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
			SaveNewSmsSwitchCode(ctrl_id);
			  MMK_CloseWin(win_id);
		break;	
	 
		
	case MSG_APP_RED:
		//SaveTTsTypeSet();
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}


WINDOW_TABLE(MMISET_NEWSMS_SWITCH_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)NewSMS_Switch_HandleMsg),
        //win_id
        WIN_ID(MMISET_NEWSMS_SWITCH_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ZDT_NEWSMS_SWITCH),
        //创建  控件
  	  CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_NEWSMS_SWITCH_CTRL_ID),
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};

LOCAL void  SetSmsSwitchCode( MMI_CTRL_ID_T ctrl_id)

{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, 2, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,2);
    //append item
    MMIAPISET_DialCodeTextId(TXT_OPEN,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_CLOSE,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	if( 0 == ZDT_GetSmsSwitch_type())//读取nv中当前环境的设置
	{
		cur_selection = 0;
	}
	else
	{
		cur_selection = 1;
	}
	GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

   GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}
LOCAL void SaveSmsSwitchCode( MMI_CTRL_ID_T ctrl_id)
{
	MMISET_TTS_CODE_INFO_T  cur_set ={0};
	MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
	uint8  sms_switch = 0;
    sms_switch =(uint16)GUILIST_GetCurItemIndex(MMISET_SMS_SWITCH_CTRL_ID);
	 MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);
	 cur_set.sms_switch= sms_switch;
    MMINV_WRITE(MMINV_SET_TTS_STYLE, &cur_set); 
	
}
PUBLIC MMI_RESULT_E SMS_Switch_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   
	MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	MMI_HANDLE_T	ctrl_id = MMISET_SMS_SWITCH_CTRL_ID;
	uint16					 max_item = 2; 
	uint8  tts_vol[2] = {0};
		MMI_STRING_T net_freq = {0};
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
	  SetSmsSwitchCode(ctrl_id);
	  MMK_SetAtvCtrl(win_id, MMISET_SMS_SWITCH_CTRL_ID);
		break;
			
	  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	//	SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
	  case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
			SaveSmsSwitchCode(ctrl_id);
			  MMK_CloseWin(win_id);
		break;	
	 
		
	case MSG_APP_RED:
		//SaveTTsTypeSet();
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}


WINDOW_TABLE(MMISET_SMS_SWITCH_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)SMS_Switch_HandleMsg),
        //win_id
        WIN_ID(MMISET_SMS_SWITCH_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ZDT_SMS_SWITCH),
        //创建  控件
  	  CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_SMS_SWITCH_CTRL_ID),
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};


LOCAL void  SetChargeSwitchCode( MMI_CTRL_ID_T ctrl_id)

{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, 2, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,2);
    //append item
    MMIAPISET_DialCodeTextId(TXT_OPEN,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_CLOSE,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	if( 0 == ZDT_GetChargeSwitch_type())//读取nv中当前环境的设置
	{
		cur_selection = 0;
	}
	else
	{
		cur_selection = 1;
	}
	GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

   GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}
LOCAL void SaveChargeSwitchCode( MMI_CTRL_ID_T ctrl_id)
{
	MMISET_TTS_CODE_INFO_T  cur_set ={0};
	MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
	uint8  charge_switch = 0;
    charge_switch =(uint16)GUILIST_GetCurItemIndex(MMISET_CHARGE_SWITCH_CTRL_ID);
	 MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);
	 cur_set.charge_switch= charge_switch;
    MMINV_WRITE(MMINV_SET_TTS_STYLE, &cur_set); 
	
}
PUBLIC MMI_RESULT_E Charge_Switch_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   
	MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	MMI_HANDLE_T	ctrl_id = MMISET_CHARGE_SWITCH_CTRL_ID;
	uint16					 max_item = 2; 
	uint8  tts_vol[2] = {0};
		MMI_STRING_T net_freq = {0};
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
	  SetChargeSwitchCode(ctrl_id);
	  MMK_SetAtvCtrl(win_id, MMISET_CHARGE_SWITCH_CTRL_ID);
		break;
			
	  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	//	SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
	  case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
			SaveChargeSwitchCode(ctrl_id);
			  MMK_CloseWin(win_id);
		break;	
	 
		
	case MSG_APP_RED:
		//SaveTTsTypeSet();
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}


WINDOW_TABLE(MMISET_CHARGE_SWITCH_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)Charge_Switch_HandleMsg),
        //win_id
        WIN_ID(MMISET_CHARGE_SWITCH_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ZDT_CHARGE_SWITCH),
        //创建  控件
  	  CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_CHARGE_SWITCH_CTRL_ID),
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};


LOCAL void  SetKLSwitchCode( MMI_CTRL_ID_T ctrl_id)

{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, 2, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,2);
    //append item
    MMIAPISET_DialCodeTextId(TXT_OPEN,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_CLOSE,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	if( 0 == ZDT_GetKLSwitch_type())//读取nv中当前环境的设置
	{
		cur_selection = 0;
	}
	else
	{
		cur_selection = 1;
	}
	GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

   GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}
LOCAL void SaveKLSwitchCode( MMI_CTRL_ID_T ctrl_id)
{
	MMISET_TTS_CODE_INFO_T  cur_set ={0};
	MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
	uint8  kl_switch = 0;
    kl_switch =(uint16)GUILIST_GetCurItemIndex(MMISET_KL_SWITCH_CTRL_ID);
	 MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);
	 cur_set.kl_switch= kl_switch;
    MMINV_WRITE(MMINV_SET_TTS_STYLE, &cur_set); 
	
}
PUBLIC MMI_RESULT_E KL_Switch_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   
	MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	MMI_HANDLE_T	ctrl_id = MMISET_KL_SWITCH_CTRL_ID;
	uint16					 max_item = 2; 
	uint8  tts_vol[2] = {0};
		MMI_STRING_T net_freq = {0};
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
	  SetKLSwitchCode(ctrl_id);
	  MMK_SetAtvCtrl(win_id, MMISET_KL_SWITCH_CTRL_ID);
		break;
			
	  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	//	SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
	  case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
			SaveKLSwitchCode(ctrl_id);
			  MMK_CloseWin(win_id);
		break;	
	 
		
	case MSG_APP_RED:
		//SaveTTsTypeSet();
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}


WINDOW_TABLE(MMISET_KL_SWITCH_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)KL_Switch_HandleMsg),
        //win_id
        WIN_ID(MMISET_KL_SWITCH_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ZDT_KL_SWITCH),
        //创建  控件
  	  CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_KL_SWITCH_CTRL_ID),
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};



LOCAL void  SetVolSwitchCode( MMI_CTRL_ID_T ctrl_id)

{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, 9, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,9);
    //append item
	MMIAPISET_DialCodeTextId(TXT_ZDT_VOL_1,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_ZDT_VOL_2,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_ZDT_VOL_3,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_ZDT_VOL_4,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_ZDT_VOL_5,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_ZDT_VOL_6,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_ZDT_VOL_7,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_ZDT_VOL_8,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_ZDT_VOL_9,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	
	cur_selection = ZDT_GetVol_Num() - 1;//读取nv中当前环境的设置
	

	GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

   GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}
LOCAL void SaveVolSwitchCode( MMI_CTRL_ID_T ctrl_id)
{
	MMISET_TTS_CODE_INFO_T  cur_set ={0};
	MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
	uint8  vol_switch = 0;
    vol_switch =(uint16)GUILIST_GetCurItemIndex(MMISET_VOL_SWITCH_CTRL_ID);
	 MMINV_READ(MMINV_SET_TTS_STYLE,&cur_set,return_value);
	 cur_set.tts_vol= vol_switch + 1;
    MMINV_WRITE(MMINV_SET_TTS_STYLE, &cur_set); 
	
}
PUBLIC MMI_RESULT_E Vol_Switch_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   
	MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	MMI_HANDLE_T	ctrl_id = MMISET_VOL_SWITCH_CTRL_ID;
	uint16					 max_item = 2; 
	uint8  tts_vol[2] = {0};
		MMI_STRING_T net_freq = {0};
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
	  SetVolSwitchCode(ctrl_id);
	  MMK_SetAtvCtrl(win_id, MMISET_VOL_SWITCH_CTRL_ID);
		break;
			
	  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	//	SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
	  case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
			SaveVolSwitchCode(ctrl_id);
			  MMK_CloseWin(win_id);
		break;	
	 
		
	case MSG_APP_RED:
		//SaveTTsTypeSet();
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}


WINDOW_TABLE(MMISET_VOL_SWITCH_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)Vol_Switch_HandleMsg),
        //win_id
        WIN_ID(MMISET_VOL_SWITCH_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ZDT_VOL_SWITCH),
        //创建  控件
  	  CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_VOL_SWITCH_CTRL_ID),
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};


LOCAL MMI_RESULT_E HandleSetZdtTTSWindow(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIMENU_GetId(MMISET_PHONE_TTS_MENU_CTRL_ID,&group_id,&menu_id);
        
            MMK_SetAtvCtrl(win_id, MMISET_PHONE_TTS_MENU_CTRL_ID);
            break;

        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            GUIMENU_GetId(MMISET_PHONE_TTS_MENU_CTRL_ID,&group_id,&menu_id);
            switch (menu_id)
            {

                case ID_ZDT_TTS_SWITCH:
                    MMK_CreateWin((uint32*)MMISET_TTS_SWITCH_WIN_TAB,PNULL);
                    break;

                case ID_ZDT_DIAL_SWITCH://
                    MMK_CreateWin((uint32*)MMISET_DIAL_SWITCH_WIN_TAB,PNULL);
                    break;
   		 case ID_ZDT_MENU_SWITCH:
                    MMK_CreateWin((uint32*)MMISET_MENU_SWITCH_WIN_TAB,PNULL);
                    break;
                 	 case ID_ZDT_TIME_SWITCH:
  			MMK_CreateWin((uint32 *)MMISET_SET_POINTTIME_WIN_TAB, PNULL);

			
			break;             

		 case ID_ZDT_ONEKEY_TIME_SWITCH:
                  // MMK_CreateWin((uint32*)MMISET_ONEKEYTIME_SWITCH_WIN_TAB,PNULL);
				   
			 MMK_CreateWin((uint32*)MMISET_SET_ONEKEYTIME_WIN_TAB,PNULL);	   	
                    break;
		case ID_ZDT_CC_SWITCH:
			 MMK_CreateWin((uint32*)MMISET_CC_SWITCH_WIN_TAB,PNULL);
			break;
		 case ID_ZDT_PB_SWITCH:
			 MMK_CreateWin((uint32*)MMISET_PB_SWITCH_WIN_TAB,PNULL);
			break;
		case ID_ZDT_NEWSMS_SWITCH:
			 MMK_CreateWin((uint32*)MMISET_NEWSMS_SWITCH_WIN_TAB,PNULL);
			break;

		case ID_ZDT_SMS_SWITCH:
			MMK_CreateWin((uint32*)MMISET_SMS_SWITCH_WIN_TAB,PNULL);
			break;
		case ID_ZDT_VOL_SWITCH:
			MMK_CreateWin((uint32*)MMISET_VOL_SWITCH_WIN_TAB,PNULL);
			break;

		case ID_ZDT_CHARGE_SWITCH:
			MMK_CreateWin((uint32*)MMISET_CHARGE_SWITCH_WIN_TAB,PNULL);
			break;	
		case ID_ZDT_KL_SWITCH:
			MMK_CreateWin((uint32*)MMISET_KL_SWITCH_WIN_TAB,PNULL);
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


WINDOW_TABLE( MMISET_PHONE_TTS_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
    WIN_FUNC( (uint32)HandleSetZdtTTSWindow ),
    WIN_ID( MMISET_PHONE_TTS_WIN_ID ),

    WIN_TITLE(TXT_SET_RING_TTS),

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    CREATE_MENU_CTRL(MENU_SET_TTS_SETTING, MMISET_PHONE_TTS_MENU_CTRL_ID),

    END_WIN
};




PUBLIC void MMIAPIST_OpenTTSWin(void)
{
    MMK_CreateWin((uint32*)MMISET_PHONE_TTS_WIN_TAB, PNULL);
}

PUBLIC void MMIAPIST_ResetTTSWin(void)
{
	MMISET_TTS_CODE_INFO_T           cur_set = {0};   
	MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;

	{
	cur_set.tts_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.dial_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  	
	cur_set.menu_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.onekey_time_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.cc_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.pb_switch = MMI_DIAL_VOICE_DEFAULT_VALUE; 
	cur_set.newsms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.sms_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
	cur_set.tts_vol = MMI_TTS_VOL_DEFAULT_VALUE;
	cur_set.charge_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;  
	cur_set.kl_switch = MMI_DIAL_VOICE_DEFAULT_VALUE;
        MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);
    }

	//MMINV_WRITE(MMINV_SET_TTS_STYLE,&cur_set);

}



#else
LOCAL void MMIAPISET_DialCodeTextId(                                    
                                                MMI_TEXT_ID_T        text_id,
                                                MMI_TEXT_ID_T        left_softkey_id,
                                                MMI_TEXT_ID_T        middle_softkey_id,
                                                MMI_TEXT_ID_T        right_softkey_id,
                                                MMI_CTRL_ID_T        ctrl_id,
                                                GUIITEM_STYLE_E      item_style
                                                )
{
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    
    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    GUILIST_AppendItem( ctrl_id, &item_t );      
}

#endif


//////////////////////////////////////////////////////////////////////////////

///PUBLIC MMI_RESULT_E PointtimeCodeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);



#if 0
WINDOW_TABLE(MMISET_SET_POINTTIME_CODE_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)PointtimeCodeWinHandleMsg),
        //win_id
        WIN_ID(MMISET_POINTTIME_CODE_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_POINT_CODE_SETTING),
        //创建  控件

  	  CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_POINTTIME_CODE_CTRL_ID),

      
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};
#endif


////////////////////////////////////////////////////////////////////////////////

#if 0
LOCAL void AppendPdaCallSettingsItem1Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_IMAGE_ID_T image_id,   //icon
                                 uint32         menu_id   //用户数据
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

     //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

     //icon
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.text_id = image_id;
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = menu_id;

    GUILIST_AppendItem(ctrl_id, &item_t);
}


LOCAL void  InitPdaPointTimeSettingsCtrl(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMISET_SET_POINT_TIME_MENU_CTRL_ID;
    uint16 list_max = 2;
	
    GUILIST_SetMaxItem(ctrl_id, list_max, FALSE );


    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_POINT_CODE_SETTING, IMAGE_NULL, ID_SET_PONTTIME_CODE_SETTING);

    AppendPdaCallSettingsItem1Line(ctrl_id, TXT_POINT_TIME_SETTING, IMAGE_NULL, ID_SET_PONTTIME_TIME_SETTING);



}


LOCAL MMI_RESULT_E  HandlePointTimeWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_MENU_ID_T               menu_id     =   0;
    MMI_RESULT_E                recode      =   MMI_RESULT_TRUE;
     MMISET_SIM_SELECT_TYPE_E    select_type =   0;
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
    uint16 index = 0;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_POINT_TIME_MENU_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitPdaPointTimeSettingsCtrl();    
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:           
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, index, &menu_id);

        switch (menu_id)
        {
  
        case ID_SET_PONTTIME_CODE_SETTING:
            MMK_CreateWin((uint32 *)MMISET_SET_POINTTIME_CODE_WIN_TAB, PNULL);
            break;  

           
        case ID_SET_PONTTIME_TIME_SETTING:
            MMK_CreateWin((uint32 *)MMISET_SET_POINTTIME_TIME_WIN_TAB, PNULL);
            break;
              
        default:
            break;
        }
        break;
        
    case MSG_GET_FOCUS:
        index = GUILIST_GetCurItemIndex(ctrl_id); 
        GUILIST_RemoveAllItems(ctrl_id);
        InitPdaPointTimeSettingsCtrl();
 
        GUILIST_SetCurItemIndex(ctrl_id, index); 
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

WINDOW_TABLE( MMISET_SET_POINTTIME_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePointTimeWindow ),    
        WIN_ID( MMISET_SET_POINTTIME_WIN_ID ),
        WIN_TITLE(TXT_SET_HOUR_CODE),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_POINT_TIME_MENU_CTRL_ID),

        END_WIN
};

PUBLIC void MMIAPIST_PontTimeWin(void)
{
    MMK_CreateWin((uint32*)MMISET_SET_POINTTIME_WIN_TAB, PNULL);
}


/////////////////////////////////////////////////////////////////////////////
PUBLIC uint8 Get_Pointtime_type( void )
{
    MMI_POINTTIME_INFO_T  cur_set ={0};
	
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_POINTTIME_STYLE,&cur_set,return_value);

   if (MN_RETURN_SUCCESS != return_value)
   {
        
		 cur_set.hour_code = 0;
	   cur_set.time_code = 1;
	  cur_set.start_time =8;
	  cur_set.end_time = 18;
       MMINV_WRITE(MMINV_SET_POINTTIME_STYLE,&cur_set);
    }

    return cur_set.hour_code;
}

LOCAL void SavePointtimeTypeSet( MMI_CTRL_ID_T ctrl_id)
{
	MMI_POINTTIME_INFO_T  cur_set ={0};
	MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;

	uint8  torch_code = 0;
    torch_code =(uint16)GUILIST_GetCurItemIndex(MMISET_POINTTIME_CODE_CTRL_ID);
	 MMINV_READ(MMINV_SET_POINTTIME_STYLE,&cur_set,return_value);
	 cur_set.hour_code = torch_code;
    MMINV_WRITE(MMINV_SET_POINTTIME_STYLE, &cur_set); 
	
}

LOCAL void  SetPointtimeCode( MMI_CTRL_ID_T ctrl_id)

{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, 2, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,2);

    //append item
    MMIAPISET_DialCodeTextId(TXT_OPEN,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_CLOSE,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	if( 0 == Get_Pointtime_type())//读取nv中当前环境的设置
	{
		cur_selection = 0;
	}
	else
	{
		cur_selection = 1;
	}
	GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

   GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}

PUBLIC MMI_RESULT_E PointtimeCodeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   
	MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	MMI_HANDLE_T	ctrl_id = MMISET_POINTTIME_CODE_CTRL_ID;
	uint16					 max_item = 2; 
	uint8  tts_vol[2] = {0};
		MMI_STRING_T net_freq = {0};
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
	  SetPointtimeCode(ctrl_id);
	  MMK_SetAtvCtrl(win_id, MMISET_POINTTIME_CODE_CTRL_ID);
		break;
			
	case MSG_GET_FOCUS://for cr NEWMS00131734
		SavePointtimeTypeSet(ctrl_id);
		break;		  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	//	SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
	  case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
			SavePointtimeTypeSet(ctrl_id);
			  MMK_CloseWin(win_id);
		break;	
	 
		
	case MSG_APP_RED:
		//SaveTTsTypeSet();
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}
#endif

///////////////////////////////////////////////////////////////////////

PUBLIC uint8 Get_Pointtime_readstatus( void )
{
    MMI_POINTTIME_INFO_T  cur_set ={0};
	
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_POINTTIME_STYLE,&cur_set,return_value);

   if (MN_RETURN_SUCCESS != return_value)
   {
        
	  cur_set.cur_auto_time_read_status = 1;
	   cur_set.cur_auto_time_quantum_mode = 1;
	  cur_set.cur_auto_time_start_time =8;
	  cur_set.cur_auto_time_end_time = 18;
       MMINV_WRITE(MMINV_SET_POINTTIME_STYLE,&cur_set);
    }

    return cur_set.cur_auto_time_read_status;
}


PUBLIC uint8 Get_Pointtime_mode( void )
{
    MMI_POINTTIME_INFO_T  cur_set ={0};
	
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_POINTTIME_STYLE,&cur_set,return_value);

   if (MN_RETURN_SUCCESS != return_value)
   {
        
	  cur_set.cur_auto_time_read_status = 1;
	   cur_set.cur_auto_time_quantum_mode = 1;
	  cur_set.cur_auto_time_start_time =8;
	  cur_set.cur_auto_time_end_time = 18;
       MMINV_WRITE(MMINV_SET_POINTTIME_STYLE,&cur_set);
    }

    return cur_set.cur_auto_time_quantum_mode;
}

PUBLIC uint8 Get_StartTimeParam( void )
{
    MMI_POINTTIME_INFO_T  cur_set ={0};
	
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_POINTTIME_STYLE,&cur_set,return_value);

   if (MN_RETURN_SUCCESS != return_value)
   {
	  cur_set.cur_auto_time_read_status = 1;
	   cur_set.cur_auto_time_quantum_mode = 1;
	  cur_set.cur_auto_time_start_time =8;
	  cur_set.cur_auto_time_end_time = 18;
       MMINV_WRITE(MMINV_SET_POINTTIME_STYLE,&cur_set);
    }

    return cur_set.cur_auto_time_start_time;
}

PUBLIC uint8 Get_endTimeParam( void )
{
    MMI_POINTTIME_INFO_T  cur_set ={0};
	
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_POINTTIME_STYLE,&cur_set,return_value);

   if (MN_RETURN_SUCCESS != return_value)
   {
	cur_set.cur_auto_time_read_status = 1;
	cur_set.cur_auto_time_quantum_mode = 1;
	cur_set.cur_auto_time_start_time =8;
	cur_set.cur_auto_time_end_time = 18;
	MMINV_WRITE(MMINV_SET_POINTTIME_STYLE,&cur_set);
    }

    return cur_set.cur_auto_time_end_time;
}



///////////////////////////////////////////////////////////////////////////



LOCAL void  SetPointtimeTime( MMI_CTRL_ID_T ctrl_id)

{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, 5, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,5);

    //append item
    MMIAPISET_DialCodeTextId(TXT_CLOSE,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);	
	MMIAPISET_DialCodeTextId(TXT_POINT_TIME_DAYTIME,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_POINT_TIME_NIGHTTIME,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_POINT_TIME_ALLTIME,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_POINT_TIME_BYCUSTOM,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	cur_selection = Get_Pointtime_mode();//读取nv中当前环境的设置

	GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

   GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}


PUBLIC MMI_RESULT_E SetPointtimeTimeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   
	MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	MMI_HANDLE_T	ctrl_id = MMISET_POINTTIME_TIME_CTRL_ID;
	uint16					 max_item = 2; 
	uint8  tts_vol[2] = {0};
		MMI_STRING_T net_freq = {0};
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
	  SetPointtimeTime(ctrl_id);
	  MMK_SetAtvCtrl(win_id, MMISET_POINTTIME_TIME_CTRL_ID);
		break;
			
	//case MSG_GET_FOCUS://for cr NEWMS00131734
	///	SaveTorchTypeSet(ctrl_id);
	//	break;		  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	//	SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
	  case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
			{
				MMI_POINTTIME_INFO_T  cur_set ={0};
				MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;

				uint8  time_code = 0;
				time_code =(uint16)GUILIST_GetCurItemIndex(MMISET_POINTTIME_TIME_CTRL_ID);
				
				MMINV_READ(MMINV_SET_POINTTIME_STYLE,&cur_set,return_value);
				cur_set.cur_auto_time_quantum_mode= time_code;
				MMINV_WRITE(MMINV_SET_POINTTIME_STYLE, &cur_set); 
				if(time_code ==  4 )
				{
					 MMK_CreateWin((uint32 *)MMISET_EDIT_POINTTIME_TIME_WIN_TAB, PNULL);
				}
				else 
				{
					if(time_code == 0)
					{
						cur_set.cur_auto_time_read_status = 0;
						cur_set.cur_auto_time_quantum_mode = 0;
						
					}
					else if(time_code == 1)
					{
						cur_set.cur_auto_time_read_status = 1;
						cur_set.cur_auto_time_quantum_mode = 1;
						cur_set.cur_auto_time_start_time = 8;
						cur_set.cur_auto_time_end_time = 18;
					}
					else if(time_code == 2)
					{
						cur_set.cur_auto_time_read_status = 1;
						cur_set.cur_auto_time_quantum_mode = 2;
						cur_set.cur_auto_time_start_time = 19;
						cur_set.cur_auto_time_end_time = 23;
					}
					else if(time_code == 3)
					{
						cur_set.cur_auto_time_read_status = 1;
						cur_set.cur_auto_time_quantum_mode = 3;
						cur_set.cur_auto_time_start_time = 0;
						cur_set.cur_auto_time_end_time = 23;
					}
					MMINV_WRITE(MMINV_SET_POINTTIME_STYLE, &cur_set); 
					  MMK_CloseWin(win_id);
				}
			}
			
		break;	
	 
		
	case MSG_APP_RED:
		//SaveTTsTypeSet();
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}



LOCAL     MMI_RESULT_E  SetPointtimeWinHandleMsg(
                                                MMI_WIN_ID_T    win_id,  // 窗口ID
                                                MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                DPARAM             param    // 消息参数
                                                )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIMENU_GetId(MMISET_POINTTIME_CTRL_ID,&group_id,&menu_id);
            MMK_SetAtvCtrl(win_id, MMISET_POINTTIME_CTRL_ID);
            break;
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            GUIMENU_GetId(MMISET_POINTTIME_CTRL_ID,&group_id,&menu_id);
            switch (menu_id)
            {
                case ID_ZDT_POINT_TIME:
                    MMK_CreateWin((uint32*)MMISET_SET_POINTTIME_TIME_WIN_TAB,PNULL);
                    break;
                case ID_ZDT_POINT_DIRECTION://
                    MMK_CreateWin((uint32*)MMISET_SET_POINTTIME_DIRECTION_WIN_TAB,PNULL);
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
LOCAL MMI_RESULT_E SetPointtimeDirectionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_STRING_T            text = {0};
	MMI_RESULT_E            result = MMI_RESULT_TRUE;
	GUI_RECT_T                rect = {0, 0, MMI_MAINSCREEN_WIDTH-1, MMI_MAINSCREEN_HEIGHT-1};
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:
	{
		MMI_STRING_T	text_s = {0};
		uint16			uint16_str_len = 0;
		char*		   temp_ptr = PNULL;
		uint16			temp_len = 0;
		GUI_FONT_T font = MMI_DEFAULT_BIG_FONT ;
		GUI_COLOR_T color = MMI_BLACK_COLOR ;//MMI_BACKGROUND_COLOR;
		MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_POINT_TIME_INFO,&text_s);
		GUITEXT_SetAlign(MMISET_POINTTIME_DIRECTION_CTRL_ID,ALIGN_HVMIDDLE);
		GUITEXT_SetFont(MMISET_POINTTIME_DIRECTION_CTRL_ID, &font, &color );
		GUITEXT_SetString(MMISET_POINTTIME_DIRECTION_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, TRUE);
	}
        break;
    case MSG_FULL_PAINT:
        {	
		MMI_STRING_T	text_s = {0};
		uint16			uint16_str_len = 0;
		char*		   temp_ptr = PNULL;
		uint16			temp_len = 0;
		GUI_FONT_T font = MMI_DEFAULT_BIG_FONT ;
		GUI_COLOR_T color = MMI_BLACK_COLOR ;//MMI_BACKGROUND_COLOR;
		MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_POINT_TIME_INFO,&text_s);
		GUITEXT_SetAlign(MMISET_POINTTIME_DIRECTION_CTRL_ID,ALIGN_HVMIDDLE);
		GUITEXT_SetFont(MMISET_POINTTIME_DIRECTION_CTRL_ID, &font, &color );
		GUITEXT_SetString(MMISET_POINTTIME_DIRECTION_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, TRUE);
            MMK_SetAtvCtrl(win_id, MMISET_POINTTIME_DIRECTION_CTRL_ID);
        }
        break;
	case MSG_APP_OK:
	  case MSG_CTL_OK:
		 MMK_CloseWin(win_id);
		   break;
	  case MSG_KEYDOWN_CANCEL:
	  case MSG_CTL_CANCEL:		  
				   MMK_CloseWin(win_id);
		   break;
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

//////////////////////////////////////////////////////////////////////

LOCAL     MMI_RESULT_E  HandleEditPointTimeWindow(
                                                MMI_WIN_ID_T    win_id,  // 窗口ID
                                                MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                DPARAM             param    // 消息参数
                                                )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIMENU_GetId(MMISET_PHONE_CUSTOMIZE_TIME_CTRL_ID,&group_id,&menu_id);
        
            MMK_SetAtvCtrl(win_id, MMISET_PHONE_CUSTOMIZE_TIME_CTRL_ID);
            break;

        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            GUIMENU_GetId(MMISET_PHONE_CUSTOMIZE_TIME_CTRL_ID,&group_id,&menu_id);
            switch (menu_id)
            {

                case ID_ZDT_START_TIME:
                    MMK_CreateWin((uint32*)MMISET_START_TIME_WIN_TAB,PNULL);
                    break;

                case ID_ZDT_END_TIME://
                    MMK_CreateWin((uint32*)MMISET_END_TIME_WIN_TAB,PNULL);
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


LOCAL void SaveStartTimeSet( MMI_CTRL_ID_T ctrl_id)
{
	MMI_POINTTIME_INFO_T  cur_set ={0};
	MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
	uint8  start_time = 0;
        start_time =(uint16)GUILIST_GetCurItemIndex(ctrl_id);
	 MMINV_READ(MMINV_SET_POINTTIME_STYLE,&cur_set,return_value);
	  cur_set.cur_auto_time_read_status = 1;
	 cur_set.cur_auto_time_quantum_mode =4;
	 cur_set.cur_auto_time_start_time = start_time;
        MMINV_WRITE(MMINV_SET_POINTTIME_STYLE, &cur_set); 
	//if()
	
}

LOCAL void  SetStartTimeCode( MMI_CTRL_ID_T ctrl_id)

{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
   MMI_TEXT_ID_T           starttime_id     = TXT_0_POINT;	
    int8 i = 0;
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, 24, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,24);
    //append item

	   	// starttime_id = starttime_id + i ;
	MMIAPISET_DialCodeTextId(TXT_0_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_1_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_2_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_3_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_4_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_5_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_6_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_7_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_8_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_9_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_10_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_11_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_12_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_13_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_14_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_15_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_16_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_17_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_18_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_19_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_20_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_21_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_22_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_23_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);

	cur_selection =Get_StartTimeParam();
	
	GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

   GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}
PUBLIC MMI_RESULT_E Start_Time_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   
	MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	MMI_HANDLE_T	ctrl_id = MMISET_TIME_START_CTRL_ID;
	uint16					 max_item = 2; 
	uint8  tts_vol[2] = {0};
		MMI_STRING_T net_freq = {0};
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
	  SetStartTimeCode(ctrl_id);
	  MMK_SetAtvCtrl(win_id, MMISET_TIME_START_CTRL_ID);
		break;
			
	  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	//	SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
	  case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
			SaveStartTimeSet(ctrl_id);
			  MMK_CloseWin(win_id);
		break;	
	 
		
	case MSG_APP_RED:
		//SaveTTsTypeSet();
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}





LOCAL void  SetEndTimeCode( MMI_CTRL_ID_T ctrl_id)

{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
   MMI_TEXT_ID_T           starttime_id     = TXT_0_POINT;	
    int8 i = 0;
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, 24, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,24);
    //append item
	MMIAPISET_DialCodeTextId(TXT_0_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_1_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_2_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_3_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_4_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_5_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_6_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_7_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_8_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_9_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_10_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_11_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_12_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_13_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_14_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_15_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_16_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_17_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_18_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_19_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_20_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_21_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_22_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_DialCodeTextId(TXT_23_POINT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);

	cur_selection =Get_endTimeParam();
	
	GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

   GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}

LOCAL void SaveEndTimeSet( MMI_CTRL_ID_T ctrl_id)
{
	MMI_POINTTIME_INFO_T  cur_set ={0};
	MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
	uint8  start_time = 0;
        start_time =(uint16)GUILIST_GetCurItemIndex(ctrl_id);
	 MMINV_READ(MMINV_SET_POINTTIME_STYLE,&cur_set,return_value);
	  cur_set.cur_auto_time_read_status = 1;
	 cur_set.cur_auto_time_quantum_mode =4;
	 cur_set.cur_auto_time_end_time = start_time;
        MMINV_WRITE(MMINV_SET_POINTTIME_STYLE, &cur_set); 
	//if()
	
}

PUBLIC MMI_RESULT_E End_Time_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   
	MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	MMI_HANDLE_T	ctrl_id = MMISET_TIME_END_CTRL_ID;
	uint16					 max_item = 2; 
	uint8  tts_vol[2] = {0};
		MMI_STRING_T net_freq = {0};
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
	  SetEndTimeCode(ctrl_id);
	  MMK_SetAtvCtrl(win_id, MMISET_TIME_END_CTRL_ID);
		break;
			
	  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	//	SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
	  case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
			SaveEndTimeSet(ctrl_id);
			  MMK_CloseWin(win_id);
		break;	
	 
		
	case MSG_APP_RED:
		//SaveTTsTypeSet();
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}


PUBLIC uint8 MMIAPIST_ResetPointstatus( void )
{
    MMI_POINTTIME_INFO_T  cur_set ={0};
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
   {
	   cur_set.cur_auto_time_read_status = 1;
	   cur_set.cur_auto_time_quantum_mode = 1;
	  cur_set.cur_auto_time_start_time =8;
	  cur_set.cur_auto_time_end_time = 18;
       MMINV_WRITE(MMINV_SET_POINTTIME_STYLE,&cur_set);
    }
}



LOCAL void MMIAPISET_CommonTextId(                                    
                                                MMI_TEXT_ID_T        text_id,
                                                MMI_TEXT_ID_T        left_softkey_id,
                                                MMI_TEXT_ID_T        middle_softkey_id,
                                                MMI_TEXT_ID_T        right_softkey_id,
                                                MMI_CTRL_ID_T        ctrl_id,
                                                GUIITEM_STYLE_E      item_style
                                                )
{
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    
    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    GUILIST_AppendItem( ctrl_id, &item_t );      
}

#ifdef SOS_KEYLONGWEB
BOOLEAN  sos_is_connected = FALSE;     //紧急拨号是否 接通    接通后停止下一次拨号
uint8    sos_times = 0;    // 紧急拨号次数  最大3次
BOOLEAN  sos_temp = FALSE;   /// 紧急拨号 标志位 true 表示正在紧急拨号中


MMISET_SOS_T		 sos_call = {0}; 
uint8	enable_temp = 0;
PUBLIC uint16 MMIAPIGET_SosCall_Status( void )
{
   
    MN_RETURN_RESULT_E       return_value = MN_RETURN_FAILURE;
    MMINV_READ(MMINV_SET_SOS_CALL_TYPE,&sos_call,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(&sos_call,0,sizeof(MMISET_SOS_T)); 
		sos_call.is_enable = 1;
        MMINV_WRITE(MMINV_SET_SOS_CALL_TYPE,&sos_call);
    }

  //  SCI_TRACE_LOW("MMIAPISET_GetMultiDialScreen_type cur_set=%x",cur_set);
    return (sos_call.is_enable);
}
PUBLIC uint8* MMIAPIGET_SosCall_Num1( void )
{
	//uint8 sos_ptr[11] = {0};
    MN_RETURN_RESULT_E		return_value = MN_RETURN_FAILURE;
	MMINV_READ(MMINV_SET_SOS_CALL_TYPE,&sos_call,return_value);
	if (MN_RETURN_SUCCESS != return_value)
	{
	  SCI_MEMSET(&sos_call,0,sizeof(MMISET_SOS_T)); 
	  sos_call.is_enable = 1;
	  MMINV_WRITE(MMINV_SET_SOS_CALL_TYPE,&sos_call);
	}
	//SCI_MEMCPY(sos_ptr, sos_call.sos,sos_call.sos_len);
	return (sos_call.sos[0]);

}
PUBLIC uint8* MMIAPIGET_SosCall_Num2( void )
{
	//uint8 sos_ptr[11] = {0};
    MN_RETURN_RESULT_E		return_value = MN_RETURN_FAILURE;
	MMINV_READ(MMINV_SET_SOS_CALL_TYPE,&sos_call,return_value);
	if (MN_RETURN_SUCCESS != return_value)
	{
	  SCI_MEMSET(&sos_call,0,sizeof(MMISET_SOS_T)); 
	  sos_call.is_enable = 1;
	  MMINV_WRITE(MMINV_SET_SOS_CALL_TYPE,&sos_call);
	}
	//SCI_MEMCPY(sos_ptr, sos_call.sos,sos_call.sos_len);
	return (sos_call.sos[1]);

}
PUBLIC uint8* MMIAPIGET_SosCall_Num3( void )
{
	//uint8 sos_ptr[11] = {0};
    MN_RETURN_RESULT_E		return_value = MN_RETURN_FAILURE;
	MMINV_READ(MMINV_SET_SOS_CALL_TYPE,&sos_call,return_value);
	if (MN_RETURN_SUCCESS != return_value)
	{
	  SCI_MEMSET(&sos_call,0,sizeof(MMISET_SOS_T)); 
	  sos_call.is_enable = 1;
	  MMINV_WRITE(MMINV_SET_SOS_CALL_TYPE,&sos_call);
	}
	//SCI_MEMCPY(sos_ptr, sos_call.sos,sos_call.sos_len);
	return (sos_call.sos[2]);

}
PUBLIC wchar* MMIAPIGET_SosCall_SMS( void )
{
	//uint8 sos_ptr[11] = {0};
    MN_RETURN_RESULT_E		return_value = MN_RETURN_FAILURE;
	MMINV_READ(MMINV_SET_SOS_CALL_TYPE,&sos_call,return_value);
	if (MN_RETURN_SUCCESS != return_value)
	{
	  SCI_MEMSET(&sos_call,0,sizeof(MMISET_SOS_T)); 
	  sos_call.is_enable = 1;
	  MMINV_WRITE(MMINV_SET_SOS_CALL_TYPE,&sos_call);
	}
	//SCI_MEMCPY(sos_ptr, sos_call.sos,sos_call.sos_len);
	return (sos_call.w_sms);

}
PUBLIC void  MMISMS_GetSOSWord( 
                                    wchar *word_ptr //[out]:the frequent word
                                    )
                                    
{
    MN_RETURN_RESULT_E		return_value = MN_RETURN_FAILURE;
     uint16 sms_len = 0 ;
    if (PNULL == word_ptr)
    {
        return;
    }
 	MMINV_READ(MMINV_SET_SOS_CALL_TYPE,&sos_call,return_value);
	if (MN_RETURN_SUCCESS != return_value)
	{
	  SCI_MEMSET(&sos_call,0,sizeof(MMISET_SOS_T)); 
	  sos_call.is_enable = 1;
	  MMINV_WRITE(MMINV_SET_SOS_CALL_TYPE,&sos_call);
	}
	sms_len =	MMIAPICOM_Wstrlen(sos_call.w_sms);
	MMI_WSTRNCPY(    word_ptr, 
					   MMISET_SOS_SMS_MAX_LEN, 
					   sos_call.w_sms, 
					   sms_len, 
					   sms_len
					   );

	
}


PUBLIC uint16 MMIAPIGET_SosCall_Len( void )
{  
	MN_RETURN_RESULT_E		return_value = MN_RETURN_FAILURE;
	MMINV_READ(MMINV_SET_SOS_CALL_TYPE,&sos_call,return_value);
	if (MN_RETURN_SUCCESS != return_value)
	{
	  SCI_MEMSET(&sos_call,0,sizeof(MMISET_SOS_T)); 
	  sos_call.is_enable = 1;
	  MMINV_WRITE(MMINV_SET_SOS_CALL_TYPE,&sos_call);
	}
	
	return (sos_call.sos_len);


}


 LOCAL void SaveSOSModify( void)
{

	MMISET_SOS_T	sos_info = {0};
	 MN_RETURN_RESULT_E		return_value = MN_RETURN_FAILURE;
	uint16		   sos_code =  0;
 	MMI_STRING_T str_sos_num1 = {0};	
	uint16    sos_num_len1 = 0 ;
	wchar  w_num1[MMISET_SOS_MAX_LEN] = { 0 };	
 	MMI_STRING_T str_sos_num2 = {0};	
	uint16    sos_num_len2 = 0 ;
	wchar  w_num2[MMISET_SOS_MAX_LEN]= { 0 };	
	MMI_STRING_T str_sos_num3 = {0};	
	uint16    sos_num_len3 = 0 ;
	wchar  w_num3[MMISET_SOS_MAX_LEN]= { 0 };	
	MMI_STRING_T  str_sos_sms = {0};

	//SCI_MEMSET(&sos_info,0,sizeof(MMISET_SOS_T)); 
	MMINV_READ(MMINV_SET_SOS_CALL_TYPE,&sos_info,return_value);
	//sos_code = (uint16)GUISETLIST_GetCurIndex(MMISET_SOS_SETLIST_CTRL_ID);
	//sos_info.is_enable = sos_code;
	//MMISET_TTSVOICEResult(tts_info);

	GUIEDIT_GetString(MMISET_SOS_NUM1_EDIT_CTRL_ID,&str_sos_num1);
	sos_num_len1 = MIN(str_sos_num1.wstr_len,MMISET_SOS_MAX_LEN);
	MMI_WSTRNCPY(
					w_num1,
					MMISET_SOS_MAX_LEN,
					str_sos_num1.wstr_ptr,
					sos_num_len1,
					sos_num_len1
					);

	SCI_MEMSET(&sos_info.sos[0],0,(MMISET_SOS_MAX_LEN*sizeof(uint8)));
	MMI_WSTRNTOSTR(    sos_info.sos[0], 
                       MMISET_SOS_MAX_LEN, 
                       w_num1, 
                       sos_num_len1, 
                       sos_num_len1
                       );
#if 1
	GUIEDIT_GetString(MMISET_SOS_NUM2_EDIT_CTRL_ID,&str_sos_num2);
	sos_num_len2 = MIN(str_sos_num2.wstr_len,MMISET_SOS_MAX_LEN);
	MMI_WSTRNCPY(
					w_num2,
					MMISET_SOS_MAX_LEN,
					str_sos_num2.wstr_ptr,
					sos_num_len2,
					sos_num_len2
					);
	SCI_MEMSET(&sos_info.sos[1],0,(MMISET_SOS_MAX_LEN*sizeof(uint8)));

	MMI_WSTRNTOSTR(    sos_info.sos[1], 
					   MMISET_SOS_MAX_LEN, 
					   w_num2, 
					   sos_num_len2, 
					   sos_num_len2
					   );
		
	GUIEDIT_GetString(MMISET_SOS_NUM3_EDIT_CTRL_ID,&str_sos_num3);
	sos_num_len3 = MIN(str_sos_num3.wstr_len,MMISET_SOS_MAX_LEN);
	MMI_WSTRNCPY(
				w_num3,
				MMISET_SOS_MAX_LEN,
				str_sos_num3.wstr_ptr,
				sos_num_len3,
				sos_num_len3
				);
	SCI_MEMSET(&sos_info.sos[2],0,(MMISET_SOS_MAX_LEN*sizeof(uint8)));

	MMI_WSTRNTOSTR(    sos_info.sos[2], 
	               MMISET_SOS_MAX_LEN, 
	               w_num3, 
	               sos_num_len3, 
	               sos_num_len3
	               );
#endif

		
	GUIEDIT_GetString(MMISET_SOS_SMS_EDIT_CTRL_ID,&str_sos_sms);
	
	SCI_MEMSET(&sos_info.w_sms,0,(50*sizeof(wchar)));
	MMI_WSTRNCPY( sos_info.w_sms, 
	               50, 
	               str_sos_sms.wstr_ptr, 
	               str_sos_sms.wstr_len, 
	               str_sos_sms.wstr_len
	               );

	sos_info.sos_len = strlen(sos_info.sos[0]);

	
	MMINV_WRITE(MMINV_SET_SOS_CALL_TYPE,&sos_info);


// MMISMS_SetTTSVol((uint8)para_ptr->cur_value);
}

 LOCAL void SetSosCodeType(
                              uint8 is_enable
                              )
{
	MMISET_SOS_T	sos_info = {0};
	 MN_RETURN_RESULT_E		return_value = MN_RETURN_FAILURE;
	 
 	MMINV_READ(MMINV_SET_SOS_CALL_TYPE,&sos_info,return_value);
	sos_info.is_enable = is_enable;
    MMINV_WRITE(MMINV_SET_SOS_CALL_TYPE, &sos_info);
}
 LOCAL void  SetSosCode(void)
 {

 #if 0
    uint8 loop = 0;
    uint16 cur_selection =0;
    uint32 text_id[] =
    {
        TXT_OPEN,
        TXT_CLOSE
    };

    GUIFORM_SetStyle(MMISET_SOS_CODE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMISET_SOS_LABEL_CTRL_ID,TXT_SOS_CALL,TRUE);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_SOS_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_SOS_SETLIST_CTRL_ID, text_id[loop]);
    }
    cur_selection =MMIAPIGET_SosCall_Status() ;//读取nv中当前环境的设置
    GUISETLIST_SetCtrlState(MMISET_SOS_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMISET_SOS_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetTitleTextId(MMISET_SOS_SETLIST_CTRL_ID,TXT_SOS_CALL);	
	#endif
		 GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/

	uint16 cur_selection =0;
	MMI_STRING_T    temp_str = {0};
	GUI_RECT_T  client_rect = MMITHEME_GetClientRect(); 
	 GUILIST_SetMaxItem(MMISET_SOS_SETLIST_CTRL_ID, 1, FALSE);

	item_t.item_style	 = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
	item_t.item_data_ptr = &item_data;

	item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
	item_data.item_content[0].item_data.text_id = TXT_SOS_CALL;
	GUILIST_AppendItem( MMISET_SOS_SETLIST_CTRL_ID, &item_t );
	cur_selection = MMIAPIGET_SosCall_Status();
	CTRLLIST_SetToggleState(MMISET_SOS_SETLIST_CTRL_ID,0,(cur_selection==0)?TRUE:FALSE);
	GUILIST_SetListState(MMISET_SOS_SETLIST_CTRL_ID, GUILIST_STATE_SPLIT_LINE, FALSE);
}



LOCAL void  EditSosNum1(void )
{

#if 1
	 uint8 loop = 0;
	MMI_STRING_T str_name = {0};
	MMI_STRING_T str_num = {0};	 
	uint8* tele_num = PNULL; 
	uint8 max_len = 0;
	uint8 tele_len = 0 ;
    wchar num[MMISET_SOS_MAX_LEN] = {0};


 GUIFORM_SetStyle(MMISET_SOS_NUM1_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
 GUILABEL_SetTextById(MMISET_SOS_NUM1_LABEL_CTRL_ID,TXT_SET_SOS_CALL_NUM1,TRUE);

 tele_num = MMIAPIGET_SosCall_Num1() ;//读取nv中当前环境的设置
 GUIEDIT_SetFont(MMISET_SOS_NUM1_EDIT_CTRL_ID,SONG_FONT_28 );
 tele_len = strlen(tele_num);

 tele_len = MIN(MMISET_SOS_MAX_LEN, tele_len);

 MMI_STRNTOWSTR(
                       num, 
                       tele_len, 
                       tele_num, 
                       tele_len, 
                       tele_len
                       );


 GUIEDIT_SetString(MMISET_SOS_NUM1_EDIT_CTRL_ID, num, tele_len);
// GUISETLIST_SetCtrlState(MMISET_SOS_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
// GUISETLIST_SetCurIndex(MMISET_SOS_SETLIST_CTRL_ID, cur_selection);
 //GUISETEdit_SetTitleTextId(MMISET_SOS_SETLIST_CTRL_ID,TXT_SET_SOS_CALL_NUM); 
 #endif
   
}

 LOCAL void  EditSosNum2(void )
 {
	  uint8 loop = 0;
	 MMI_STRING_T str_name = {0};
	 MMI_STRING_T str_num = {0};  
	 uint8* tele_num = PNULL; 
	 uint8 max_len = 0;
	 uint8 tele_len = 0 ;
	 wchar num[MMISET_SOS_MAX_LEN] = {0};
 
 
  GUIFORM_SetStyle(MMISET_SOS_NUM2_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
  GUILABEL_SetTextById(MMISET_SOS_NUM2_LABEL_CTRL_ID,TXT_SET_SOS_CALL_NUM2,FALSE);
 
  tele_num = MMIAPIGET_SosCall_Num2() ;//读取nv中当前环境的设置
  GUIEDIT_SetFont(MMISET_SOS_NUM2_EDIT_CTRL_ID,SONG_FONT_28 );
  tele_len = strlen(tele_num);
 
  tele_len = MIN(MMISET_SOS_MAX_LEN, tele_len);
 
  MMI_STRNTOWSTR(
						num, 
						tele_len, 
						tele_num, 
						tele_len, 
						tele_len
						);
 
 
  GUIEDIT_SetString(MMISET_SOS_NUM2_EDIT_CTRL_ID, num, tele_len);
 // GUISETLIST_SetCtrlState(MMISET_SOS_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
 // GUISETLIST_SetCurIndex(MMISET_SOS_SETLIST_CTRL_ID, cur_selection);
  //GUISETEdit_SetTitleTextId(MMISET_SOS_SETLIST_CTRL_ID,TXT_SET_SOS_CALL_NUM); 
 }

 LOCAL void  EditSosNum3(void )
 {
	  uint8 loop = 0;
	 MMI_STRING_T str_name = {0};
	 MMI_STRING_T str_num = {0};  
	 uint8* tele_num = PNULL; 
	 uint8 max_len = 0;
	 uint8 tele_len = 0 ;
	 wchar num[MMISET_SOS_MAX_LEN] = {0};
 
 
  GUIFORM_SetStyle(MMISET_SOS_NUM3_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
  GUILABEL_SetTextById(MMISET_SOS_NUM3_LABEL_CTRL_ID,TXT_SET_SOS_CALL_NUM3,FALSE);
 
  tele_num = MMIAPIGET_SosCall_Num3() ;//读取nv中当前环境的设置
  GUIEDIT_SetFont(MMISET_SOS_NUM3_EDIT_CTRL_ID,SONG_FONT_28 );
  tele_len = strlen(tele_num);
 
  tele_len = MIN(MMISET_SOS_MAX_LEN, tele_len);
 
  MMI_STRNTOWSTR(
						num, 
						tele_len, 
						tele_num, 
						tele_len, 
						tele_len
						);
 
 
  GUIEDIT_SetString(MMISET_SOS_NUM3_EDIT_CTRL_ID, num, tele_len);
 // GUISETLIST_SetCtrlState(MMISET_SOS_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
 // GUISETLIST_SetCurIndex(MMISET_SOS_SETLIST_CTRL_ID, cur_selection);
  //GUISETEdit_SetTitleTextId(MMISET_SOS_SETLIST_CTRL_ID,TXT_SET_SOS_CALL_NUM); 
 }


 LOCAL void  EditSosSMS(void )
 { 
 	MMI_STRING_T str_sos_sms= {0};
 	 wchar*  p_sms = PNULL;
	 wchar w_contant_sms[50] = {0}; 
	 uint8 max_len = 0;
	 uint16 sms_len = 0 ;
	 wchar num[MMISET_SOS_MAX_LEN] = {0};

 
  GUIFORM_SetStyle(MMISET_SOS_SMS_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
  GUILABEL_SetTextById(MMISET_SOS_SMS_LABEL_CTRL_ID,TXT_SET_SOS_SMS,FALSE);
 
  p_sms = MMIAPIGET_SosCall_SMS() ;//读取nv中当前环境的设置
  sms_len = MMIAPICOM_Wstrlen(p_sms);
  if(sms_len == 0)
  {
	MMI_GetLabelTextByLang(TXT_INI_SOS_SMS, &str_sos_sms);
	p_sms = str_sos_sms.wstr_ptr;
	sms_len = str_sos_sms.wstr_len;
  }
  //srt_sms.wstr_ptr = p_sms;
 // srt_sms.wstr_len =  sms_len;
  GUIEDIT_SetFont(MMISET_SOS_SMS_EDIT_CTRL_ID,SONG_FONT_28 );
  GUIEDIT_SetString(MMISET_SOS_SMS_EDIT_CTRL_ID, p_sms, sms_len);
 }

 LOCAL void  DisplaySOSText(void )
  { 
	 MMI_STRING_T str_sos_ingo= {0};
	  wchar*  p_sms = PNULL;
	  wchar w_contant_sms[50] = {0}; 
	  uint8 max_len = 0;
	  uint16 sms_len = 0 ;
	  wchar num[MMISET_SOS_MAX_LEN] = {0};

  
   GUIFORM_SetStyle(MMISET_SOS_TEXT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
   GUILABEL_SetTextById(MMISET_SOS_TEXT_LABEL_CTRL_ID,TXT_SOS_HELP,FALSE);
  // GUILABEL_SetTextById(MMISET_SOS_TEXT_CTRL_ID,TXT_SOS_HELP_INFO,FALSE);
   MMI_GetLabelTextByLang(TXT_SOS_HELP_INFO, &str_sos_ingo);

   GUITEXT_SetString(MMISET_SOS_TEXT_CTRL_ID, str_sos_ingo.wstr_ptr, str_sos_ingo.wstr_len, TRUE);  

  }

 LOCAL MMI_RESULT_E  HandleSetSosCallWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
 {
	
	 MMI_RESULT_E	  recode = MMI_RESULT_TRUE;
     MMI_CTRL_ID_T   ctrl_id	= MMISET_SOS_SETLIST_CTRL_ID;
	 GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
	uint16 				  max_item = 2; 
	uint8	tts_vol[2] = {0};
	uint16 cur_selection =0;
	MMI_STRING_T    temp_str = {0};
	GUI_RECT_T  client_rect = MMITHEME_GetClientRect(); 
	
	 switch (msg_id)
	 {
	 case MSG_OPEN_WINDOW:
	 GUIFORM_SetType(MMISET_SOS_FORM_CTRL_ID,GUIFORM_TYPE_TP);
	 SetSosCode();
 	 EditSosNum1();
     EditSosNum2();
     EditSosNum3();
	 EditSosSMS();
	  DisplaySOSText();
	 //SetTtsVolParam();
	 MMK_SetAtvCtrl(win_id, MMISET_SOS_SETLIST_CTRL_ID);
		 break;
  case MSG_FULL_PAINT:
  	{
            GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
            GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
            LCD_FillRect(&lcd_dev_info, full_rect,MMI_BLACK_COLOR);
            MMI_GetLabelTextByLang(TXT_SET_SOS_CALL, &temp_str);
            if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len))
			{
                GUIWIN_UpdateStb();
            }
		 if(MMK_GetActiveCtrlId(win_id) == MMISET_SOS_SETLIST_CTRL_ID)
		 	{
                GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SELECT, MIDDLE_BUTTON, TRUE);
              //  GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, TRUE);
             //   GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_PRIMO_COMMON_BACK, RIGHT_BUTTON, TRUE);
            }
		 	else
		 	{
				  GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, TRUE);
			}
  	}
			break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	 case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
	 case MSG_CTL_OK:
	 case MSG_APP_OK:
		 {
				 MMI_STRING_T net_freq = {0};
				 if(MSG_CTL_PENOK == msg_id)
				 {
					 ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
				 }
				 else
				 {
					 ctrl_id= MMK_GetActiveCtrlId(win_id);
				 }
				 switch(ctrl_id)
				 {
	 
				 case MMISET_SOS_NUM1_EDIT_CTRL_ID:
				 case MMISET_SOS_NUM2_EDIT_CTRL_ID:
				 case MMISET_SOS_NUM3_EDIT_CTRL_ID:	 
				 case MMISET_SOS_SMS_EDIT_CTRL_ID:
				 case	MMISET_SOS_TEXT_CTRL_ID:
						 SaveSOSModify();
						 //MMK_CloseWin(win_id);
					 break;
		   
				 default:
					  recode = MMI_RESULT_FALSE;
					 break;
				 }
				 MMK_CloseWin(win_id);
		  }

			break;
	 case MSG_CTL_MIDSK:
	 case MSG_APP_WEB:
		{
			MMI_STRING_T net_freq = {0};
			if(MSG_CTL_PENOK == msg_id)
			{
				ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
			}
			else
			{
				ctrl_id= MMK_GetActiveCtrlId(win_id);
			}

			switch(ctrl_id)
			{

			 case MMISET_SOS_SETLIST_CTRL_ID:
                     cur_selection = MMIAPIGET_SosCall_Status();
                    if(cur_selection == 1) {
                        SetSosCodeType(0);
                    
                    } else {
                        SetSosCodeType(1);
                  
                    }
               
    
                    break;
			case MMISET_SOS_NUM1_EDIT_CTRL_ID:
			case MMISET_SOS_NUM2_EDIT_CTRL_ID:
			case MMISET_SOS_NUM3_EDIT_CTRL_ID:	
			case MMISET_SOS_SMS_EDIT_CTRL_ID:
			case MMISET_SOS_TEXT_FORM_CTRL_ID:
					SaveSOSModify();
					//MMK_CloseWin(win_id);
				break;
	  
			default:
				 recode = MMI_RESULT_FALSE;
				break;
			}
		}
	 break;  
	  
	// case MSG_GET_FOCUS://for cr NEWMS00131734
		// SaveSOSModify();
	//	 break; 	   
	 case MSG_APP_CANCEL:
	 case MSG_CTL_CANCEL:
		 MMK_CloseWin(win_id);
		 break;
		 
	 case MSG_APP_RED:

		 recode = MMI_RESULT_FALSE;
		 break;
	 default:
		 recode = MMI_RESULT_FALSE;
		 break;
	 }
	 
	 return recode;
	
 }

 
WINDOW_TABLE( MMISET_SOS_CALL_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleSetSosCallWindow ),    
    WIN_ID( MMISET_SOS_CALL_WIN_ID),
    WIN_TITLE(TXT_SET_SOS_CALL ),
	//创建  控件
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_SOS_FORM_CTRL_ID),

	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISET_SOS_CODE_FORM_CTRL_ID, MMISET_SOS_FORM_CTRL_ID),
	//CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_SOS_LABEL_CTRL_ID, MMISET_SOS_CODE_FORM_CTRL_ID),
	//CHILD_SETLIST_CTRL(TRUE, MMISET_SOS_SETLIST_CTRL_ID, MMISET_SOS_CODE_FORM_CTRL_ID),
	CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISET_SOS_SETLIST_CTRL_ID, MMISET_SOS_CODE_FORM_CTRL_ID),



	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISET_SOS_NUM1_FORM_CTRL_ID, MMISET_SOS_FORM_CTRL_ID),
	CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_SOS_NUM1_LABEL_CTRL_ID, MMISET_SOS_NUM1_FORM_CTRL_ID),
	CHILD_EDIT_PHONENUM_CTRL(TRUE,MMISET_SOS_MAX_LEN ,MMISET_SOS_NUM1_EDIT_CTRL_ID, MMISET_SOS_NUM1_FORM_CTRL_ID),
#if 1
	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISET_SOS_NUM2_FORM_CTRL_ID, MMISET_SOS_FORM_CTRL_ID),
	CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_SOS_NUM2_LABEL_CTRL_ID, MMISET_SOS_NUM2_FORM_CTRL_ID),
	CHILD_EDIT_PHONENUM_CTRL(TRUE,MMISET_SOS_MAX_LEN ,MMISET_SOS_NUM2_EDIT_CTRL_ID, MMISET_SOS_NUM2_FORM_CTRL_ID),

	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISET_SOS_NUM3_FORM_CTRL_ID, MMISET_SOS_FORM_CTRL_ID),
	CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_SOS_NUM3_LABEL_CTRL_ID, MMISET_SOS_NUM3_FORM_CTRL_ID),
	CHILD_EDIT_PHONENUM_CTRL(TRUE,MMISET_SOS_MAX_LEN ,MMISET_SOS_NUM3_EDIT_CTRL_ID, MMISET_SOS_NUM3_FORM_CTRL_ID),
#endif

	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISET_SOS_SMS_FORM_CTRL_ID, MMISET_SOS_FORM_CTRL_ID),
	CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_SOS_SMS_LABEL_CTRL_ID, MMISET_SOS_SMS_FORM_CTRL_ID),
	CHILD_EDIT_TEXT_CTRL(TRUE,50 ,MMISET_SOS_SMS_EDIT_CTRL_ID, MMISET_SOS_SMS_FORM_CTRL_ID),

	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISET_SOS_TEXT_FORM_CTRL_ID, MMISET_SOS_FORM_CTRL_ID),
	CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMISET_SOS_TEXT_LABEL_CTRL_ID, MMISET_SOS_TEXT_FORM_CTRL_ID),
	CHILD_TEXT_CTRL(TRUE, MMISET_SOS_TEXT_CTRL_ID, MMISET_SOS_TEXT_FORM_CTRL_ID),


	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	END_WIN
};

PUBLIC void MMIAPIST_OpenSOSWin(void)
{
    MMK_CreateWin((uint32*)MMISET_SOS_CALL_WIN_TAB, PNULL);
}

PUBLIC void ZdtSendSOSMessage(void)
{
    uint8 index = 0;
    BOOLEAN result = FALSE;
    MMI_STRING_T sos_msg_str = {0};
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMISMS_MULTI_SEND_DATA_T send_data = {0};
    uint8 contacts_index = 0;
    uint8 sos_send_num[MMISET_PHONE_NUM_MAX_LEN] = {0};
	wchar w_sms_sos[50] = {0};
	uint16 wstr_len = 0 ;
	uint8* sos_num1 = {0};
	uint8 len1 = 0;
	uint8* sos_num2 = {0};
	uint8 len2 = 0;
	uint8* sos_num3 = {0};
	uint8 len3 = 0;
	uint8 len[3] = {0};
	uint8* sos_num[3] = {0};

	

    if(MMIAPIPHONE_GetSimAvailableNum(&dual_sys, 1))
    {
        dual_sys = MMIAPISET_GetActiveSim();
		SCI_MEMSET(w_sms_sos,0,sizeof(w_sms_sos));
	   // SCI_MEMSET(sos_msg_str.wstr_ptr, 0x00, sizeof(wchar)*sos_msg_str.wstr_len);
		MMISMS_GetSOSWord(&w_sms_sos);
		wstr_len = MMIAPICOM_Wstrlen(w_sms_sos);
		if(wstr_len > 0)
		{
			sos_msg_str.wstr_len  = wstr_len;
			sos_msg_str.wstr_ptr = w_sms_sos;
			//MMI_WSTRNCPY(    str_sos_sms.wstr_ptr, 
			//			   MMISET_SOS_SMS_MAX_LEN, 
			//			   w_sms_sos, 
			//			   wstr_len, 
			//			   wstr_len
			//			   );
		}
		else
		{
			MMI_GetLabelTextByLang(TXT_INI_SOS_SMS, &sos_msg_str);
			wstr_len = sos_msg_str.wstr_len;
		}
     
        //SCI_MEMCPY(sos_msg_str.wstr_ptr, (wchar *)s_sos_msg_info.msg_content,sos_msg_str.wstr_len);

    	send_data.dual_sys = (MN_DUAL_SYS_E)dual_sys;
        send_data.sms_content_ptr = &sos_msg_str;
        send_data.is_need_packet = TRUE;
        send_data.send_callback = PNULL;

		sos_num[0] = 	MMIAPIGET_SosCall_Num1();
		len[0] = strlen(sos_num[0]);
		//SCI_MEMSET(sos_num2,0,14);

		sos_num[1] = MMIAPIGET_SosCall_Num2();
		len[1]  = strlen(sos_num[1]);
		//SCI_MEMSET(sos_num3,0,14);
		sos_num[2] = MMIAPIGET_SosCall_Num3();
		len[2]  = strlen(sos_num[2]);

		  for(index = 0; index < 3; index++)
		  {
		  	if(len[index] > 0)
		  	{
				SCI_MEMCPY(
				 send_data.dest_num[contacts_index],
				sos_num[index],
				len[index]
				);
				contacts_index++;
			}


		  }
		 #if 0 
		if(len1 > 0 )
		{
			SCI_MEMCPY(
				 send_data.dest_num[0],
				sos_num1,
				len1
				);
		 contacts_index++;
		}
		if(len2 > 0 )
		{
		 SCI_MEMCPY(      
           send_data.dest_num[1], 
            sos_num2,
            len2
            );  
		 contacts_index++;
		}
		if(len3 > 0 )
		{
		 SCI_MEMCPY(      
           send_data.dest_num[2], 
            sos_num3,
            len3
            );  
		 contacts_index++;
		}
 #endif
        send_data.dest_num_len = contacts_index;
        result = MMIAPISMS_MultiSendSms(&send_data);
        SCI_TRACE_LOW("[SOS]sendSOSMessage result %d.", result);
     
    }
    else
    {
    	SCI_TRACE_LOW("[SOS]sendSOSMessage failed for there's no available sim card.");
    }

}


PUBLIC void MMIAPIST_ResetSOS(void)
{

	MN_RETURN_RESULT_E	   return_value = MN_RETURN_FAILURE;
  // MMINV_READ(MMINV_SET_SOS_CALL_TYPE,&sos_call,return_value);
  // if (MN_RETURN_SUCCESS != return_value)
   {
	 SCI_MEMSET(&sos_call,0,sizeof(MMISET_SOS_T)); 
	   sos_call.is_enable = 1;
	 MMINV_WRITE(MMINV_SET_SOS_CALL_TYPE,&sos_call);
   }


}


#endif

PUBLIC BOOLEAN MMI_IsRingSelect(void)
{
	if (MMK_IsOpenWin(MMISET_RING_SELECT_CALL_WIN_ID))
	{
		return TRUE;
	}
	else if (MMK_IsOpenWin(MMISET_RING_SELECT_FIXED_WIN_ID))
	{
		return TRUE;
	}    
	else if (MMK_IsOpenWin(MMISET_RING_SELECT_MSG_WIN_ID))
	{
		return TRUE;
	}  
	else if (MMK_IsOpenWin(MMISET_MSG_RING_SELECT_FIXED_WIN_ID))
	{
		return TRUE;
	}     
	else
	{
		return FALSE;
	}
}


#define  ZDT_IMSI_LEN 15
MMISET_IMSI_T		lock_imsi= {0}; 
char         g_zdt_sim_imsi[ZDT_IMSI_LEN+1] = {0};

uint16  MMIZDT_Get_SIM_IMSI(MN_DUAL_SYS_E dual_sys,char * imsi_ptr)
{
	uint8 i=0, j=0;
	MN_IMSI_T imsi = {0};
       char imsi_buf[ZDT_IMSI_LEN+1] = {0};
       
	imsi = MNSIM_GetImsiEx(dual_sys);

	if(imsi.imsi_val == NULL )
		return 0;

	for(i = 0, j = 0; i < 8; i++)
	{
		imsi_buf[j++] = (imsi.imsi_val[i] & 0x0F) + '0';
		imsi_buf[j++] = ((imsi.imsi_val[i] >> 4) & 0x0F) + '0';
	}
    
	for( i=0; i<ZDT_IMSI_LEN; i++ )
	{
		imsi_ptr[i] = imsi_buf[i+1];
	}
	imsi_ptr[ZDT_IMSI_LEN] = 0x00;
	  SCI_TRACE_LOW("MMIZDT_Get_SIM_IMSI STR imsi_buf= %s,dual_sys==%d",imsi_buf,dual_sys);
	return ZDT_IMSI_LEN;
}


PUBLIC uint8* MMIAPIGET_NV_IMSI( void )
{  
	MN_RETURN_RESULT_E		return_value = MN_RETURN_FAILURE;
	MMINV_READ(MMINV_SET_IMSI_TYPE,&lock_imsi,return_value);
	if (MN_RETURN_SUCCESS != return_value)
	{
	  SCI_MEMSET(&lock_imsi,0,sizeof(MMISET_IMSI_T)); 
	  MMINV_WRITE(MMINV_SET_IMSI_TYPE,&lock_imsi);
	}
	  SCI_TRACE_LOW("MMIAPIGET_NV_IMSI STR = %s,imsi_ptrle==%d",lock_imsi.imsi,strlen(lock_imsi.imsi));
	return lock_imsi.imsi;
}

 PUBLIC void MMISetNV_IMSI(char * imsi_ptr   )
{
	MMISET_IMSI_T	lock_imsi_info = {0};
	 MN_RETURN_RESULT_E		return_value = MN_RETURN_FAILURE;
	 
        SCI_TRACE_LOW("MMIZDT_Get_SIM_IMSI STR = %s,imsi_ptrle==%d",imsi_ptr,strlen(imsi_ptr));
	if(strlen(imsi_ptr) == 15)
	{
	    SCI_MEMSET(&lock_imsi_info,0,sizeof(MMISET_IMSI_T)); 
	    SCI_MEMCPY(lock_imsi_info.imsi,imsi_ptr,15);
	    MMINV_WRITE(MMINV_SET_IMSI_TYPE, &lock_imsi_info);
	}
}


PUBLIC BOOLEAN MMIAPICmp_IMSI_Info( MN_DUAL_SYS_E set_sys )
{
	//MMISET_IMSI_T imsisvn = {0};/*lint !e64*/
	 uint8* imsisvn_num = PNULL; 
	uint8 imsi_len =0;
	uint8 imsi_sim_len = 0;
	// SCI_MEMSET(&imsisvn_num,0,sizeof(MMISET_IMSI_T)); 
       SCI_MEMSET(g_zdt_sim_imsi,0,ZDT_IMSI_LEN+1);
       MMIZDT_Get_SIM_IMSI(set_sys,g_zdt_sim_imsi);

	   imsisvn_num =MMIAPIGET_NV_IMSI();
  //	SCI_MEMCPY(imsisvn.imsi ,MMIAPIGET_NV_IMSI(),15);
	imsi_sim_len = strlen(g_zdt_sim_imsi);
	imsi_len= strlen(imsisvn_num);
	
	SCI_TRACE_LOW("g_zdt_sim_imsilen=%d,len=%d",imsi_sim_len,imsi_len);
	if(imsi_len == 0  )
	{
		MMISetNV_IMSI(g_zdt_sim_imsi);
		SCI_TRACE_LOW("g_zdt_sim_imsi=%s,len=%d",g_zdt_sim_imsi,strlen(g_zdt_sim_imsi));
		return TRUE;
	}

	if(imsi_sim_len <15 || imsi_len< 15)
	{
		return  FALSE;
	}
	
	//imei_info2_cmp = MMIAPIGET_IMEI2_Info();
	SCI_TRACE_LOW("aaaag_zdt_sim_imsi=%s,len=%d",g_zdt_sim_imsi,strlen(g_zdt_sim_imsi));
	SCI_TRACE_LOW("MMIAPICmp_IMSI_Info imsisvn.imsi=%s,len=%d",imsisvn_num,imsi_len);

	
	if(SCI_STRLEN(g_zdt_sim_imsi) != SCI_STRLEN(imsisvn_num) )
	{
		SCI_TRACE_LOW("len !=");
		SCI_TRACE_LOW("MMIAPICmp_IMSlen FALSE");
		return  FALSE;
	}
	if(0 == MMIAPICOM_StrCmp((uint8 *)g_zdt_sim_imsi, SCI_STRLEN(g_zdt_sim_imsi), (uint8 *)imsisvn_num, SCI_STRLEN(imsisvn_num)))
	{
		SCI_TRACE_LOW("MMIAPICmp_IMSI_Info TRUE");

		return TRUE;
	}
	else
	{
		SCI_TRACE_LOW("MMIAPICmp_IMSI_Info FALSE");
		return  FALSE;
	}

}

#ifdef ZDT_WDL_Z35ZL_SUPPORT



extern  BOOLEAN  s_is_torch_open;



LOCAL void SetTorchLight(BOOLEAN torch_open )
{
#ifndef WIN32
         GPIO_SetTorch(torch_open);
#endif
}
LOCAL void SaveTorchTypeSet(void)
{
	uint8  torch_code = 0;
     

      torch_code =(uint16)GUILIST_GetCurItemIndex(MMISET_TORCH_SETLIST_CTRL_ID);   ///GUISETLIST_GetCurIndex(MMISET_TORCH_SETLIST_CTRL_ID);
    //MMINV_WRITE(MMINV_SET_CDB_STYLE, &cdb_code); 
	
	if(torch_code)
	{
		SetTorchLight(FALSE);
		s_is_torch_open = FALSE;
	}
	else
	{
		SetTorchLight(TRUE);
		s_is_torch_open =TRUE;
	}
}

PUBLIC uint8 Get_Torch_type( void )
{
    uint8  cur_set =0;  
   if(s_is_torch_open )
   {
	cur_set = 0;
   }
   else
   {
	cur_set = 1;
   }

    return cur_set;
}



LOCAL void  SetTorchCode( MMI_CTRL_ID_T ctrl_id)
{
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
    GUILIST_SetMaxItem( ctrl_id, 2, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,2);
    //append item
    MMIAPISET_CommonTextId(TXT_OPEN,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_CommonTextId(TXT_CLOSE,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	 if(Get_Torch_type() == 0)
    {
		cur_selection = 0;

	}
	else if(Get_Torch_type() == 1)
    {
		cur_selection = 1;
    }

	///}
	GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
    GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}

PUBLIC MMI_RESULT_E TorchWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   
	MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
	static uint16		  cur_selection = 0;
	  MMI_CTRL_ID_T   ctrl_id  = MMISET_TORCH_SETLIST_CTRL_ID;
	uint16					 max_item = 2; 

	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
	  SetTorchCode(ctrl_id);
	  MMK_SetAtvCtrl(win_id, MMISET_TORCH_SETLIST_CTRL_ID);
		break;
			
	 case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
	case MSG_CTL_OK:
	case MSG_APP_OK:
	case MSG_APP_WEB:
		SaveTorchTypeSet();
		MMK_CloseWin(win_id);
		break;		  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	//	SaveTTsTypeSet();
		MMK_CloseWin(win_id);
		break;
		
	case MSG_APP_RED:
		//SaveTTsTypeSet();
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}

WINDOW_TABLE(MMIENVSET_TORCH_TYPE_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)TorchWinHandleMsg),
        //win_id
        WIN_ID(MMISET_TORCH_TYPE_WIN_ID),
        //窗口的标题
        WIN_TITLE(STR_FLASHL_HDR_TORCH),
        //创建  控件
  	 CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_TORCH_SETLIST_CTRL_ID),
      
      WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};
PUBLIC void MMISET_OpenTORCHWin(void)
{
    MMK_CreateWin((uint32*)MMIENVSET_TORCH_TYPE_WIN_TAB, PNULL);
}





#endif

