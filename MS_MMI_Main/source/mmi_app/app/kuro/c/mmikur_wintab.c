#include "mmi_app_kuro_trc.h"
#include "std_header.h"
#ifdef KURO_SUPPORT
/*****************************************************************************
** File Name:      mmikur_wintab.c                                           *
** Author:                                                                   *
** Date:           08/2007                                                   *
** Description:    This file is used to describe kur player                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2007        cmzs              create                                  *
******************************************************************************/

#define _MMIKUR_WINTAB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "window_parse.h"
#include "mmikur_id.h"
#include "mmidisplay_data.h"
#include "mmikuro_text.h"
#include "mmipub.h"
#include "guilcd.h"
#include "mmikur.h"
#include "mmikur_internal.h"
#include "mmikur_menutable.h"
#include "guilistbox.h"
#include "mmifmm_export.h"
#include "mmiidle_subwintab.h"
#include "guilabel.h"
#include "mmiidle_export.h"
#include "mmi_appmsg.h"
#include "mmikur_lyric_internal.h"
#include "guiref_scale.h"
#include "mmiaudio_ctrl.h"
#include "mmifm_export.h"
#include "mmimpeg4.h"
#include "mmi_default.h"
//#include "mmimultim.h"
#include "mmk_timer.h"
#include "mmikur_musicstyle.h"
#include "kurdecoder.h"
#include "mmiset_display.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
#include "guitext.h"
//#include "mmieng.h"
#include "mmieng_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIKUR_ANIM_TIME_OUT        150
#define MMIKUR_TOOLBAR_TIME_OUT        5000
#define MMIKUR_TUNE_VOLUME_KEY_PERIOD   300 //300ms
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL uint8     s_kur_anim_timer_id = 0;    //kur anim timer
LOCAL uint32     s_kur_last_toolbardsp_time = 0;
LOCAL uint8     s_kur_key_timer_id = 0;             //key timer
LOCAL BOOLEAN   s_kur_is_increase_volume = FALSE;   //increase or decrease volume by key timer
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : stop mp3 tune volume key timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopTuneVolumeKeyTimer(void);
/*****************************************************************************/
//  Description : start mp3 tune volume key timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartTuneVolumeKeyTimer(
                                   BOOLEAN      is_increase
                                   );
/*****************************************************************************/
//  Description : handle help window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurHelpWinMsg(
                                        MMI_WIN_ID_T        win_id,     
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );
/*****************************************************************************/
//  Description : append kur eq set item
//  Global resource dependence : 
//  Author: cmzs
//  Note:
/*****************************************************************************/
LOCAL void AppendEqSetItem(
                           MMI_CTRL_ID_T    ctrl_id
                           );

/*****************************************************************************/
//  Description : handle play mode set window message
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurPlayModeWin(
                                        MMI_WIN_ID_T        win_id,     
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );

/*****************************************************************************/
//  Description : handle eq set window message
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurEqSetWin(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E   msg_id, 
                                     DPARAM             param
                                     );

/*****************************************************************************/
//  Description : handle kur main play window touch panel down key
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurMainWinTpDownKey(
                                             DPARAM           param
                                             );

/*****************************************************************************/
//  Description : handle kur play window message
//  Global resource dependence :
//  Author:cmzs
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurPlayWinMsg(
                                       MMI_WIN_ID_T         win_id,     
                                       MMI_MESSAGE_ID_E     msg_id, 
                                       DPARAM               param
                                       );

/*****************************************************************************/
//  Description : handle kur play menu window message
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurPlayMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E   msg_id, 
                                     DPARAM             param
                                     );

/*****************************************************************************/
//  Description : handle the kur list window message.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurPlayListWinMsg(
                        MMI_WIN_ID_T       win_id,  
                        MMI_MESSAGE_ID_E   msg_id, 
                        DPARAM             param
                        );

/*****************************************************************************/
//  Description :Init the ctrl of kuro list window.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void InitKurListCtrl(
                MMI_CTRL_ID_T ctrl_id
                );

/*****************************************************************************/
//  Description :Init the list item of kuro list window.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void InitKurListItem(
                MMI_CTRL_ID_T   ctrl_id,
                uint32          item_index,
                MMI_TEXT_ID_T   left_softkey_id,
                MMI_TEXT_ID_T   mid_softkey_id,
                MMI_TEXT_ID_T   right_softkey_id
                );

/*****************************************************************************/
//  Description : Show All Kur File List
//  Global resource dependence :
//  Author:cmzs
//  Note:
/*****************************************************************************/
LOCAL void ShowAllKurList(void);
LOCAL void OpenKurListWin(void);

// the window for kur main window
WINDOW_TABLE( MMIKUR_MAIN_PLAY_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleKurPlayWinMsg ),
    WIN_ID( MMIKUR_MAIN_PLAY_WIN_ID ),
    END_WIN
};


// the window for Kur play list window
WINDOW_TABLE( MMIKUR_PLAY_LIST_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleKurPlayListWinMsg ),
    WIN_ID( MMIKUR_PLAY_LIST_WIN_ID ),
    WIN_TITLE( TXT_KUR_PLAY_LIST ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIKUR_PLAY_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),  
    END_WIN
};
//the window for kur player menu window
WINDOW_TABLE(MMIKUR_PLAY_MENU_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleKurPlayMenuWinMsg),    
    WIN_ID( MMIKUR_PLAY_MENU_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(MENU_KUR_PLAY,MENU_KUR_PLAY_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the window for kur eq mode setting
WINDOW_TABLE( MMIKUR_EQ_SET_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleKurEqSetWin),    
    WIN_ID( MMIKUR_EQ_SET_WIN_ID ),
    WIN_TITLE( TXT_KUR_EQ_SETTING ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIKUR_EQ_SET_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
// the window for kur play mode window
WINDOW_TABLE( MMIKUR_PLAY_MODE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleKurPlayModeWin ),    
    WIN_ID( MMIKUR_PLAY_MODE_WIN_ID ),
    WIN_TITLE( TXT_KUR_PLAY_MODE ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIKUR_PLAY_MODE_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIKUR_HELP_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleKurHelpWinMsg),
    WIN_ID(MMIKUR_HELP_WIN_ID),
    WIN_TITLE( TXT_MMIPASTIME_HELP ),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIKUR_HELP_TB_CTRL_ID),
    END_WIN
};


/*****************************************************************************/
//  Description : start kur tune volume key timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartTuneVolumeKeyTimer(
                                   BOOLEAN      is_increase
                                   )
{
    if (0 == s_kur_key_timer_id)
    {
        s_kur_key_timer_id = MMK_CreateTimer(MMIKUR_TUNE_VOLUME_KEY_PERIOD,TRUE);
        s_kur_is_increase_volume = is_increase;
    }
    else
    {
        //SCI_TRACE_LOW:"StartTuneVolumeKeyTimer: the key timer has started!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_WINTAB_274_112_2_18_2_31_44_58,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : stop mp3 tune volume key timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopTuneVolumeKeyTimer(void)
{
    if (0 < s_kur_key_timer_id)
    {
        MMK_StopTimer(s_kur_key_timer_id);
        s_kur_key_timer_id = 0;
    }
    else
    {
        //SCI_TRACE_LOW:"StopTuneVolumeKeyTimer: the key timer has stop!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_WINTAB_291_112_2_18_2_31_44_59,(uint8*)"");
    }
}
/*****************************************************************************/
//  Description : Open  Kur File List
//  Global resource dependence :
//  Author:cmzs
//  Note:
/*****************************************************************************/
LOCAL void OpenKurListWin(void)
{
    if (0 < MMIKUR_GetTotalNum())
    {
        MMK_CloseWin(MMIKUR_PLAY_LIST_WIN_ID);        
        MMK_CreateWin((uint32 *)MMIKUR_PLAY_LIST_WIN_TAB, PNULL);
    }
}

/*****************************************************************************/
//  Description : Show All Kur File List
//  Global resource dependence :
//  Author:cmzs
//  Note:
/*****************************************************************************/
LOCAL void ShowAllKurList(void)
{
    MMI_WIN_ID_T        alert_win_id = MMIKUR_ALERT_WIN_ID;
    
    if (0 < MMIKUR_GetTotalNum())
    {
        MMK_CloseWin(MMIKUR_PLAY_LIST_WIN_ID);
        
        MMK_CreateWin((uint32 *)MMIKUR_PLAY_LIST_WIN_TAB, PNULL);
    }
    else
    {
        //提示没有kur
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_KUR_NO_MP3,TXT_NULL,
            IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
}

/*****************************************************************************/
//  Description : stop kur play and close play window
//  Global resource dependence :
//  Author:cmzs
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_StopKurAndCloseWin(void)
{
    MMIAPIKUR_StopKurPlayer();
    MMIAPIKUR_CloseWin();

    //if (!MMK_IsOpenWin(MMIKUR_MAIN_PLAY_WIN_ID))
    {
        if (MMIKUR_PLAYER_STATE_NONE == MMIKUR_GetPlayerState())
        {
            MMIAUDIO_ResumeOtherBgPlay(MMIBGPLAY_TYPE_KURO);
        }
    }
}


/*****************************************************************************/
//  Description : close kuro play window
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_CloseWin(
                void
                )
{
    MMK_CloseMiddleWin(MMIKUR_WIN_ID_START, MMIKUR_MAX_WIN_ID);
}

/*****************************************************************************/
//  Description : Open kur player
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_OpenKurPlayer(void)
{
    MMI_WIN_ID_T     alert_win_id = MMIKUR_ALERT_WIN_ID;
    FILEARRAY_DATA_T kur_info = {0};
    FILEARRAY_DATA_T klr_info = {0};

    if(MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_NULL,
          IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return ;
    }
    else if(MMIAPISD_IsCardLogOn())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_CARDLOG_USING);
        return;
    }
    else if (MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
        return;
    }

    MMIAPIKUR_InitKurFolder();
    MMIKUR_OnPowerOn();
    
    if(MMIKUR_AUDIO_PLAYING  == MMIKUR_GetAudioState() ||
       MMIKUR_AUDIO_PAUSED   == MMIKUR_GetAudioState())
    {
        MMIKUR_GetCurKurInfo(&kur_info);
        MMIKUR_GetCurKlrInfo(&klr_info);
        if(MMIAPIFMM_IsFileExist(kur_info.filename, kur_info.name_len) &&
           MMIAPIFMM_IsFileExist(klr_info.filename, klr_info.name_len))
        {
            if(!MMK_IsOpenWin(MMIKUR_MAIN_PLAY_WIN_ID))
            {  
#ifdef UI_P3D_SUPPORT
                MMI_Disable3DEffect(MMI_3D_EFFECT_KUR_PLAYER);
#endif             
                MMK_CreateWin((uint32 *)MMIKUR_MAIN_PLAY_WIN_TAB, PNULL);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIKURO]:MMIAPIKUR_OpenKurPlayer, Kur Or Klr Not Exits!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_WINTAB_407_112_2_18_2_31_44_60,(uint8*)"");
            MMIAPIKUR_StopKurPlayer();
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_KUR_NO_MP3,TXT_NULL,
                IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,MMIKUR_HandleKurError);
        }
    }
    else
    {
        MMK_CreateWin((uint32 *)MMIKUR_PLAY_MENU_WIN_TAB, PNULL);
    }
}

/*****************************************************************************/
//  Description : 添加播放模式radio list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AppendPlayModeListItem(
                                  MMI_CTRL_ID_T     ctrl_id
                                  )
{
    uint16      cur_selection = 0;

    //set max mode
    GUILIST_SetMaxItem(ctrl_id,MMIKUR_PLAY_MAX_MODE,FALSE);

    //add items to listbox
    MMIAPISET_AppendListItemByTextId(TXT_KUR_PLAY_ONE_ONCE,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_KUR_PLAY_ONE_REPEAT,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_KUR_PLAY_LIST_ONCE,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_KUR_PLAY_LIST_REPEAT,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_KUR_PLAY_RANDOM_REPEAT,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_KUR_PLAY_RANDOM_ONCE,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);

    //get selected play mode
    cur_selection = (uint16)(MMIKUR_GetPlayMode());

    //set selected item
    GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);

}

/*****************************************************************************/
//  Description : handle play mode set window message
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurPlayModeWin(
                                        MMI_WIN_ID_T        win_id,     
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    uint16          cur_selection = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIKUR_PLAY_MODE_LIST_CTRL_ID;
    MMI_WIN_ID_T    alert_win_id = MMIKUR_ALERT_WIN_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendPlayModeListItem(ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //get the selected item
        GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
        MMIKUR_SetPlayMode((MMIKUR_PLAY_MODE_E)cur_selection);

        //success!
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,
            IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

        MMK_CloseWin( win_id );
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle eq set window message
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurEqSetWin(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E   msg_id, 
                                     DPARAM             param
                                     )
{
    uint16          cur_selection = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIKUR_EQ_SET_LIST_CTRL_ID;
    MMI_WIN_ID_T    alert_win_id = MMIKUR_ALERT_WIN_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendEqSetItem(ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

        case MSG_CTL_MIDSK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
        MMIKUR_SetEqMode((MMIAUDIO_EQ_MODE_E)cur_selection);

        //success!
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,
            IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

        MMK_CloseWin( win_id );
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle kur main play window touch panel down key
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurMainWinTpDownKey(
                                             DPARAM           param
                                             )
{
    GUI_RECT_T      play_button_rect = {190,0,240,35};
    GUI_RECT_T      stop_button_rect = {190,36,240,68};
    GUI_RECT_T      prev_button_rect = {190,69,240,101};
    GUI_RECT_T      next_button_rect = {190,102,240,134};
    GUI_RECT_T      list_button_rect = {190,135,240,167};
    GUI_RECT_T      mode_button_rect = {190,168,240,200};
    GUI_RECT_T      volume_button_rect = {190,201,240,233};
    GUI_RECT_T      close_button_rect = {190,234,240,266};

    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    GUI_POINT_T     tp_point = {0};
//get tp point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);

    if(!MMIKUR_IsBackgroundReady())
    {
        return result;
    }
    s_kur_last_toolbardsp_time = SCI_GetTickCount();

    if(MMIKUR_GetbdispVolume())
    {
        GUI_RECT_T rcCloseVolume = {198,95,221,110};
        GUI_RECT_T rcAddVolume   = {198,114,221,130};
        GUI_RECT_T rcSubVolume   = {198,200,221,218};
        GUI_RECT_T rcVolume      = {190,90,230,230};
        
        if(GUI_PointIsInRect(tp_point,rcCloseVolume))
        {
            MMIKLR_HideKurVolumeWin();
        }
        else if(GUI_PointIsInRect(tp_point,rcAddVolume))
        {
            if(MMIAPIKUR_SetKurVolume(TRUE))
            {
                MMIKLR_DisplayKurVolumeWin(TRUE);
                StartTuneVolumeKeyTimer(TRUE);
            }
        }
        else if(GUI_PointIsInRect(tp_point,rcSubVolume))
        {
            if(MMIAPIKUR_SetKurVolume(FALSE))
            {
                MMIKLR_DisplayKurVolumeWin(TRUE);
                StartTuneVolumeKeyTimer(FALSE);
            }
        }
        else if(!GUI_PointIsInRect(tp_point,rcVolume))
        {
            MMIKLR_HideKurVolumeWin();
            MMIKLR_DisplayToolBar(TRUE);
        }
    }
    else
    {
        if(!MMIKUR_Getbdisptoolbar())
        {
            MMIKLR_HideKurVolumeWin();
            MMIKLR_DisplayToolBar(FALSE);
            return result;
        }
        //if press eq button
        if (GUI_PointIsInRect(tp_point,play_button_rect))
        {
            switch(MMIKUR_GetPlayerState())
            {
                case MMIKUR_PLAYER_STATE_NONE:
                    MMIAPIKUR_PlayKurPlayer();
                    break;
                case MMIKUR_PLAYER_AUDIO_ACTIVE:
                    {
                        switch(MMIKUR_GetAudioState())
                        {
                            case MMIKUR_AUDIO_OPENED:
                                break;
                                
                            case MMIKUR_AUDIO_PLAYING:
                                MMIAPIKUR_PauseKurPlayer(TRUE);
                                break;

                            case MMIKUR_AUDIO_PAUSED:
                                MMIAPIKUR_ResumeKurPlayer(TRUE);
                                break;

                            case MMIKUR_AUDIO_STOPPED:
                                break;
                                
                            case MMIKUR_AUDIO_CLOSED:
                                break;
                            
                            default:
                                break;
                        }
                    }
                    break;

                case MMIKUR_PLAYER_MUSIC_SWITCH:
                    MMIAPIKUR_PauseKurPlayer(TRUE);
                    break;

                case MMIKUR_PLAYER_SWITCH_PAUSED:
                    MMIAPIKUR_ResumeKurPlayer(TRUE);
                    break;
                    
                default:
                    break;
            }
            MMIKLR_DisplayToolBar(TRUE);
        }
        else if (GUI_PointIsInRect(tp_point,stop_button_rect))
        {
            MMIAPIKUR_StopKurPlayer();
            MMIKUR_DisplayKurPlayWin(TRUE,MMIKUR_MAIN_PLAY_WIN_ID);
        }
        else if (GUI_PointIsInRect(tp_point,prev_button_rect))
        {
            MMIAPIKUR_GoToPreOrNextKur(TRUE);
        }
        else if (GUI_PointIsInRect(tp_point,next_button_rect))
        {
            MMIAPIKUR_GoToPreOrNextKur(FALSE);
        }
        else if (GUI_PointIsInRect(tp_point,close_button_rect))
        {
            MMK_CloseWin(MMIKUR_MAIN_PLAY_WIN_ID);
        }
        else if (GUI_PointIsInRect(tp_point,mode_button_rect))
        {
            MMIKUR_GoToNextLyricMode();
            MMIKLR_DisplayToolBar(TRUE);
        }
        else if (GUI_PointIsInRect(tp_point,volume_button_rect))
        {
            MMIKLR_DisplayKurVolumeWin(TRUE);
        }
        else if (GUI_PointIsInRect(tp_point,list_button_rect))
        {
            OpenKurListWin();
        }
    }
    return (result);
}

/*****************************************************************************/
//  Description : handle kur play window message
//  Global resource dependence :
//  Author:cmzs
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurPlayWinMsg(
                                       MMI_WIN_ID_T         win_id,     
                                       MMI_MESSAGE_ID_E     msg_id, 
                                       DPARAM               param
                                       )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMISET_BACKLIGHT_SET_T backlight_info;

    //SCI_TRACE_LOW:"[MMIKURO]: HandleKurPlayWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_WINTAB_717_112_2_18_2_31_45_61,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MSG_KUR_PLAY_CALL_BACK:
        //SCI_TRACE_LOW:"[MMIKURO]: KUR:MSG_KUR_PLAY_CALL_BACK"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_WINTAB_721_112_2_18_2_31_45_62,(uint8*)"");
        MMIAPIKUR_PlayKurCallBack();
        break;

    case MSG_KUR_PLAY_START:
        //SCI_TRACE_LOW:"[MMIKURO]: KUR:MSG_KUR_PLAY_START"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_WINTAB_726_112_2_18_2_31_45_63,(uint8*)"");
        MMIAPIKUR_PlayKurPlayer();
        break;

    case MSG_OPEN_WINDOW:
        //MMIDEFAULT_EnableKeyRing(FALSE);
        //MMIDEFAULT_EnableTpRing(FALSE);
        MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_KURO, FALSE);
        //if (MMIKUR_PLAYER_STATE_NONE == MMIKUR_GetPlayerState())
        {
            MMIAUDIO_PauseOtherBgPlay(MMIBGPLAY_TYPE_KURO);
        }
        MMIDEFAULT_TurnOnBackLight();
        MMIDEFAULT_AllowTurnOffBackLight(FALSE); 
        if(MMIAPIKUR_IsPlayerPlaying())
        {
            MMIKUR_PlayKlr();
        }
        break;

    case MSG_FULL_PAINT:       
        MMIKUR_DisplayKurPlayWin(TRUE,win_id);
        break;

    case MSG_GET_FOCUS:
        MMIDEFAULT_TurnOnBackLight();
        MMIDEFAULT_AllowTurnOffBackLight(FALSE); 
        //start anim timer
        MMIKUR_DisplayKurPlayWin(TRUE,win_id);
        break;

    case MSG_LOSE_FOCUS:
        //stop anim timer
        MMIKUR_StopAnimTimer();
        StopTuneVolumeKeyTimer();
        if(MMIKUR_Getbdisptoolbar() || MMIKUR_GetbdispVolume())
        {
            //SCI_TRACE_LOW:"[MMIKURO]: HandleKurPlayWinMsg close volume and toolbar"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_WINTAB_763_112_2_18_2_31_45_64,(uint8*)"");
            MMIKLR_HideKurVolumeWin();
            MMIKLR_HideToolBar();
        }
        break;

    case MSG_TIMER:
        if (s_kur_anim_timer_id == *((uint8 *)param))
        {
            //set kur anim index
            MMIKUR_StopAnimTimer();
      
            //display kur anim
            MMIKUR_DisplayKlr(MMIKUR_MAIN_PLAY_WIN_ID,FALSE,FALSE,MMIKUR_GetKlrMode());
            //start kur anim
            MMIKUR_StartAnimTimer();
        }
        else if (s_kur_key_timer_id == *((uint8 *)param))
        {
            //tune volume
            if (MMIAPIKUR_SetKurVolume(s_kur_is_increase_volume))
            {
                MMIKLR_DisplayKurVolumeWin(TRUE);
            }
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        if(MMIKUR_Getbdisptoolbar() || MMIKUR_GetbdispVolume())
        {
            if(SCI_GetTickCount() - s_kur_last_toolbardsp_time > MMIKUR_TOOLBAR_TIME_OUT)
            {
                MMIKLR_HideKurVolumeWin();
                MMIKLR_HideToolBar();
            }
        }
        break;


    case MSG_CTL_OK:
    case MSG_APP_OK:
        if(!MMK_IsOpenWin(MMIKUR_PLAY_MENU_WIN_ID))
        {        
            MMK_CreateWin((uint32 *)MMIKUR_PLAY_MENU_WIN_TAB,PNULL);
        }
        break;
    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMIAPIKUR_StopKurAndCloseWin();
        break;
    case MSG_APP_WEB:
        switch(MMIKUR_GetPlayerState())
        {
            case MMIKUR_PLAYER_STATE_NONE:
                MMIAPIKUR_PlayKurPlayer();
                break;
            case MMIKUR_PLAYER_AUDIO_ACTIVE:
                {
                    switch(MMIKUR_GetAudioState())
                    {
                        case MMIKUR_AUDIO_OPENED:
                            break;
                            
                        case MMIKUR_AUDIO_PLAYING:
                            MMIAPIKUR_PauseKurPlayer(TRUE);
                            break;

                        case MMIKUR_AUDIO_PAUSED:
                            MMIAPIKUR_ResumeKurPlayer(TRUE);
                            break;

                        case MMIKUR_AUDIO_STOPPED:
                            break;
                            
                        case MMIKUR_AUDIO_CLOSED:
                            break;
                        
                        default:
                            break;
                    }
                }
                break;

            case MMIKUR_PLAYER_MUSIC_SWITCH:
                MMIAPIKUR_PauseKurPlayer(TRUE);
                break;

            case MMIKUR_PLAYER_SWITCH_PAUSED:
                MMIAPIKUR_ResumeKurPlayer(TRUE);
                break;
                
            default:
                break;
        }
        break;

    case MSG_KEYLONG_WEB:
        break;
        
    case MSG_APP_LEFT:
        MMIKUR_GoToNextLyricMode();
        break;

    case MSG_APP_RIGHT:
        OpenKurListWin();
        break;

    case MSG_APP_UP:
    case MSG_KEYLONG_BACKWARD:
        MMIAPIKUR_GoToPreOrNextKur(TRUE);
        break;

    case MSG_APP_DOWN:
    case MSG_KEYLONG_FORWARD:
        MMIAPIKUR_GoToPreOrNextKur(FALSE);
        break;
        
    case MSG_APP_UPSIDE:
    case MSG_APP_STAR:
    case MSG_APP_DOWNSIDE:
    case MSG_APP_HASH:
        if (MMICOM_RETURN_INCREASE ==MMIAPICOM_IsIncrease(msg_id))
        {
            StopTuneVolumeKeyTimer();
            if(MMIAPIKUR_SetKurVolume(TRUE))
            {
                StartTuneVolumeKeyTimer(TRUE);
            }
            s_kur_last_toolbardsp_time = SCI_GetTickCount();
            MMIKLR_DisplayKurVolumeWin(TRUE);
        }
        else
        {
            StopTuneVolumeKeyTimer();
            if(MMIAPIKUR_SetKurVolume(FALSE))
            {
                StartTuneVolumeKeyTimer(FALSE);
            }
            s_kur_last_toolbardsp_time = SCI_GetTickCount();
            MMIKLR_DisplayKurVolumeWin(TRUE);
        }
        break;

    case MSG_KEYUP_UP:
    case MSG_KEYUP_DOWN:
    case MSG_KEYUP_LEFT:
    case MSG_KEYUP_RIGHT:
    case MSG_KEYUP_UPSIDE:
    case MSG_KEYUP_DOWNSIDE:
    case MSG_KEYPRESSUP_UP:
    case MSG_KEYPRESSUP_DOWN:
    case MSG_KEYPRESSUP_LEFT:
    case MSG_KEYPRESSUP_RIGHT:
    case MSG_KEYPRESSUP_UPSIDE:
    case MSG_KEYPRESSUP_DOWNSIDE:
    case MSG_KEYUP_STAR:
    case MSG_KEYUP_HASH:
        StopTuneVolumeKeyTimer();
        break;

    case MSG_KEYPRESSUP_WEB:
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        //handle tp key
        result = HandleKurMainWinTpDownKey(param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        StopTuneVolumeKeyTimer();
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_KUR_START_READ_PIC:
        MMIKUR_HandleReadPicMsg(win_id);
        break;
/*        
    case MSG_APP_RED:
        MMIAPIKUR_StopKurAndCloseWin();
        result = MMI_RESULT_FALSE;
        break;
*/
    case MSG_CLOSE_WINDOW:
        MMK_CloseWin(MMIKUR_PLAY_MENU_WIN_ID);
        
        if(MMIAPIKUR_IsPlayerPaused())
        {
            MMIAPIKUR_StopKurPlayer();
        }

        MMIKUR_StopAnimTimer();
        MMIKLR_HideKurVolumeWin();
        MMIKUR_Setbdisptoolbar(FALSE);
        MMIKUR_FreeKlrBuffer();
        MMIKUR_ReSetPicReadInfo();
        
        MMIDEFAULT_AllowTurnOffBackLight(TRUE); 
        //MMIDEFAULT_EnableKeyRing(TRUE);
        //MMIDEFAULT_EnableTpRing(TRUE);
        MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_KURO, TRUE);

        if (MMIKUR_PLAYER_STATE_NONE == MMIKUR_GetPlayerState())
        {
            MMIKUR_ResetList();
            MMIAUDIO_ResumeOtherBgPlay(MMIBGPLAY_TYPE_KURO);
        }
#ifdef UI_P3D_SUPPORT
        MMI_Enable3DEffect(MMI_3D_EFFECT_KUR_PLAYER);
#endif         
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : display kur play window
//  Global resource dependence :
//  Author:cmzs
//  Note:
/*****************************************************************************/
void MMIKUR_DisplayKurPlayWin(
                              BOOLEAN           is_update,//is update name and lyric
                              MMI_WIN_ID_T      win_id
                              )
{
    GUI_RECT_T screen_rect = MMITHEME_GetFullScreenRect();
    LCD_FillRect(MMITHEME_GetDefaultLcdDev(), screen_rect, MMI_BLACK_COLOR);
    
    MMIKUR_DisplayKlr(win_id,TRUE,TRUE,MMIKUR_GetKlrMode());
    if(MMIKUR_Getbdisptoolbar())
    {
        MMIKLR_DisplayToolBar(TRUE);
    }
    //启动kur动画的定时器
    MMIKUR_StartAnimTimer();
}

/*****************************************************************************/
//  Description : append kur eq set item
//  Global resource dependence : 
//  Author: cmzs
//  Note:
/*****************************************************************************/
LOCAL void AppendEqSetItem(
                           MMI_CTRL_ID_T    ctrl_id
                           )
{
    uint16  cur_selection = 0;

    //set max item
    GUILIST_SetMaxItem(ctrl_id,MMIAUDIO_EQ_MAX_MODE,FALSE);

    //append items
    MMIAPISET_AppendListItemByTextId(TXT_COMM_REGULAR,STXT_SOFTKEY_OK_MK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_KUR_CLASSIC_EQ,STXT_SOFTKEY_OK_MK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_KUR_ODEUM_EQ,STXT_SOFTKEY_OK_MK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_KUR_JAZZ_EQ,STXT_SOFTKEY_OK_MK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_KUR_ROCK_EQ,STXT_SOFTKEY_OK_MK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_KUR_SOFT_ROCK_EQ,STXT_SOFTKEY_OK_MK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);

    //get current eq set
    cur_selection = (uint16)MMIKUR_GetEqMode();

    //set select item
    GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
  
}

/*****************************************************************************/
//  Description : start kur anim timer
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_StartAnimTimer(void)
{
    BOOLEAN         result = FALSE;
    MMI_WIN_ID_T    win_id = MMIKUR_MAIN_PLAY_WIN_ID;
    if ((MMK_IsFocusWin(win_id)) &&
        (MMIKUR_AUDIO_PLAYING == MMIKUR_GetAudioState()) &&
        (0 == s_kur_anim_timer_id))
    {
        s_kur_anim_timer_id = MMK_CreateWinTimer(win_id,MMIKUR_ANIM_TIME_OUT,FALSE);
        result = TRUE;
    }
    //SCI_TRACE_LOW:"[MMIKURO]:MMIKUR_StartAnimTimer s_kur_anim_timer_id= %d,result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_WINTAB_1053_112_2_18_2_31_45_65,(uint8*)"dd",s_kur_anim_timer_id,result);
    return (result);
}

/*****************************************************************************/
//  Description : stop kur anim timer
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_StopAnimTimer(
                 void
                 )
{
    //SCI_TRACE_LOW("[MMIKURO]:MMIKUR_StopAnimTimer s_kur_anim_timer_id= %d",s_kur_anim_timer_id);
    if (0 != s_kur_anim_timer_id)
    {
        MMK_StopTimer(s_kur_anim_timer_id);
        s_kur_anim_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : handle kur play menu window message
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurPlayMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E   msg_id, 
                                     DPARAM             param
                                     )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MENU_KUR_PLAY_MENU_CTRL_ID;
    MMI_MENU_ID_T       menu_id = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;
//  BOOLEAN             is_need_a2dp = FALSE;
    MMI_WIN_ID_T        alert_win_id = MMIKUR_ALERT_WIN_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //MMIDEFAULT_EnableKeyRing(FALSE);
        //MMIDEFAULT_EnableTpRing(FALSE);
        MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_KURO, FALSE);
        //if (MMIKUR_PLAYER_STATE_NONE == MMIKUR_GetPlayerState())
        {
            MMIAUDIO_PauseOtherBgPlay(MMIBGPLAY_TYPE_KURO);
        }

        MMK_SetAtvCtrl(win_id,ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

    case MSG_KUR_LOAD_ALL_KURO_FINISH:
        ShowAllKurList();
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {
            case ID_KUR_ALLKUR:
                MMIKUR_LoadAllKuro();
                break;
                
            case ID_KUR_KDB:
                MMIAPIKUR_OpenKurMusicStyle();
//              MMIAPIKUR_OpenPlayListWin();
                break;
            case ID_KUR_EQMODE:
#ifdef  BLUETOOTH_SUPPORT
                if (MMIAPIBT_GetActiveBTHeadset())
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_BT_NOT_SUPPORT_EQ,TXT_NULL,
                        IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                }
                else
#endif        
                {                
                    MMK_CreateWin((uint32 *)MMIKUR_EQ_SET_WIN_TAB, PNULL);
                }
                break;
            case ID_KUR_PLAYMODE:
                MMK_CreateWin((uint32 *)MMIKUR_PLAY_MODE_WIN_TAB, PNULL);
                break;

            case ID_KUR_HELP:
                MMK_CreateWin((uint32 *)MMIKUR_HELP_WIN_TAB, PNULL);
                break;
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        //MMIDEFAULT_EnableKeyRing(TRUE);
        //MMIDEFAULT_EnableTpRing(TRUE);
        MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_KURO, TRUE);
        if  (MMIKUR_PLAYER_STATE_NONE == MMIKUR_GetPlayerState() &&
            !MMK_IsOpenWin(MMIKUR_MAIN_PLAY_WIN_ID))
        {
            MMIKUR_ResetList();
            MMIAUDIO_ResumeOtherBgPlay(MMIBGPLAY_TYPE_KURO);
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;  
}

/*****************************************************************************/
//  Description : handle the kur list window message.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurPlayListWinMsg(
                        MMI_WIN_ID_T       win_id,  
                        MMI_MESSAGE_ID_E   msg_id, 
                        DPARAM             param
                        )
{
    MMI_RESULT_E    result  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIKUR_PLAY_LIST_CTRL_ID;
    uint16          cur_selection = 0;
    GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = PNULL;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitKurListCtrl(ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        GUILIST_SetCurItemIndex(ctrl_id, MMIKUR_GetActiveIndex());
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;

			//arvinzhang modify:delete assert
            //SCI_ASSERT( PNULL != need_item_data_ptr );
            
            //SCI_TRACE_LOW("[MMIKURO]:HandleKurPlayListWinMsg,MSG_CTL_LIST_NEED_ITEM_DATA; index = %d",need_item_data_ptr->item_index);
            if(PNULL != need_item_data_ptr)
            {
                InitKurListItem(
	                ctrl_id,
	                need_item_data_ptr->item_index,
	                TXT_COMMON_OK,
	                TXT_NULL,
	                STXT_RETURN
	                );
            }

        }
        break;

    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(MMIKUR_PLAY_LIST_CTRL_ID);

        MMIAUDIO_PauseOtherBgPlay(MMIBGPLAY_TYPE_KURO);
        MMIAPIKUR_StopKurPlayer();
        MMIKUR_SetPlayerState(MMIKUR_PLAYER_MUSIC_SWITCH);
        
        MMIKUR_SetStartIndex(cur_selection);
        MMIKUR_LoadKuroByIndex(cur_selection);
        
        if(!MMK_IsOpenWin(MMIKUR_MAIN_PLAY_WIN_ID))
        {     
#ifdef UI_P3D_SUPPORT
            MMI_Disable3DEffect(MMI_3D_EFFECT_KUR_PLAYER);
#endif         
            MMK_CreateWin((uint32 *)MMIKUR_MAIN_PLAY_WIN_TAB, PNULL);
        }
        MMK_PostMsg(MMIKUR_MAIN_PLAY_WIN_ID,MSG_KUR_PLAY_START,PNULL,0);
        MMK_CloseWin(win_id);
        MMK_CloseWin(MMIKUR_PLAY_MENU_WIN_ID);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description :Init the ctrl of kuro list window.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void InitKurListCtrl(
                MMI_CTRL_ID_T ctrl_id
                )
{
    GUILIST_ITEM_T  item_t    = {0};
    uint32          i         = 0;
    uint32          total_num = MMIKUR_GetTotalNum();
    MMI_HANDLE_T    list_handle = 0;

    list_handle = MMK_ConvertIdToHandle(ctrl_id);
    
    if(GUILIST_GetTotalItemNum(ctrl_id))
    {
        GUILIST_RemoveAllItems(ctrl_id);
    }
    GUILIST_SetMaxItem( ctrl_id, total_num, TRUE);
    
    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
    GUILIST_SetTitleStyle(ctrl_id, GUILIST_TITLE_ONLY);
    
    if(0 < total_num)
    {
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
        for(i=0; i<total_num; i++)
        {
            GUILIST_AppendItem(list_handle, &item_t);
        }
    }
}

/*****************************************************************************/
//  Description :Init the list item of kuro list window.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void InitKurListItem(
                MMI_CTRL_ID_T   ctrl_id,
                uint32          item_index,
                MMI_TEXT_ID_T   left_softkey_id,
                MMI_TEXT_ID_T   mid_softkey_id,
                MMI_TEXT_ID_T   right_softkey_id
                )
{
    GUILIST_ITEM_T       item_t    = {0};
    GUILIST_ITEM_DATA_T  item_data = {0};
    wchar                file_name[FMM_SEARCH_FILENAME_MAXLEN + 1] = {0};
    uint16               file_name_len = 0;
    MMIFILE_DEVICE_E     dev_type = MMI_DEVICE_SYSTEM;
    FILEARRAY_DATA_T     kur_info = {0};
    
    item_t.item_data_ptr = &item_data;

    if(0 < MMIKUR_GetTotalNum())
    {
        if(MMIKUR_GetKurInfoByIndex(item_index, &kur_info))
        {
            dev_type = MMIKUR_GetFileDevType(kur_info.filename, kur_info.name_len);

            MMIAPIFMM_SplitFullPath(
                kur_info.filename, kur_info.name_len,
                PNULL, PNULL, PNULL, PNULL, 
                file_name, &file_name_len
                );
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIKURO] InitKurListItem MMIKUR_GetKurInfoByIndex failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_WINTAB_1324_112_2_18_2_31_46_66,(uint8*)"");
            return;
        }
    }

    //soft key
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = mid_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;

    //icon 
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    if(FS_UDISK == dev_type)
    {
        item_data.item_content[0].item_data.image_id = IMAGE_COMMON_MUSIC_UDISK_ICON; 
    }
    else if(FS_MMC == dev_type || FS_MMC1 == dev_type)
    {
        item_data.item_content[0].item_data.image_id = IMAGE_COMMON_MUSIC_SD_ICON; 
    }
    else
    {
        SCI_PASSERT(FALSE,("[MMIKUR] : InitPlayListItem:dev_type = %d is error!",dev_type));/*assert verified*/
    }
    
    // text 1
    {
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = file_name_len;

        item_data.item_content[1].item_data.text_buffer.wstr_ptr = file_name;

//         MMI_WSTRNCPY(
//             item_data.item_content[1].item_data.text_buffer.wstr,
//             GUILIST_STRING_MAX_NUM,
//             file_name,
//             file_name_len,
//             item_data.item_content[1].item_data.text_buffer.wstr_len
//             );
    }

    GUILIST_SetItemData( ctrl_id, &item_data, item_index );
}

LOCAL MMI_RESULT_E HandleKurHelpWinMsg(
                                        MMI_WIN_ID_T        win_id,     
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIKUR_HELP_TB_CTRL_ID;
    MMI_STRING_T show_content = {0};
    
    switch( msg_id )
    {
    case MSG_OPEN_WINDOW:
        MMI_GetLabelTextByLang(TXT_KURO_HELP, &show_content);

        GUITEXT_SetString(ctrl_id, show_content.wstr_ptr, show_content.wstr_len, FALSE);   

        MMK_SetAtvCtrl( win_id, ctrl_id );
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : mp3 play main win is focus
//  Global resource dependence :
//  Author: Jasmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsPlayerFocus(void)
{
    return (MMK_IsFocusWin(MMIKUR_MAIN_PLAY_WIN_ID));
}

#endif


/*Edit by script, ignore 3 case. Thu Apr 26 19:00:57 2012*/ //IGNORE9527


/*Edit by script, ignore 4 case. Fri Apr 27 09:38:51 2012*/ //IGNORE9527
