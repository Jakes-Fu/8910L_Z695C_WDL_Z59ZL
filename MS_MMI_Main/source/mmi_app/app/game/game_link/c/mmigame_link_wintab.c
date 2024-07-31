/****************************************************************************
** File Name:      mmigame_link_wintab.c                            
** Author:                                                                  
** Date:            2006/04/01
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to create windows of game
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 04/01/2006     annie         Create
** 
****************************************************************************/
#define _MMIGAME_LINK_WINTAB_C_


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmi_app_game_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_LINK_SUPPORT

#include "window_parse.h"
#include "mmk_app.h"
#include "guitext.h"
#include "guimsgbox.h"
#include "guilcd.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "mmigame_link_menutable.h"
#include "mmigame_menutable.h"
#include "mmigame_link_id.h"  
#include "tb_dal.h"
//#include "mmiacc_winmain.h"
#include "mmigame_link_nv.h"
#include "mmialarm_export.h"
//#include "mmigame.h"
#include "mmigame_internal.h"
#include "mmi_textfun.h"
#include "mmiphone_export.h"
#include "mmigame_link_internal.h"
#include "mmigame_link_text.h"
#include "mmigame_link_image.h"
#include "mmigame_id.h"
#include "window_parse.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_theme.h"
#include "mmi_nv.h"
#include "mmipub.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmk_timer.h"
#include "mmi_default.h"
#include "mmigame_position.h"
#include "guires.h"
#include "guibutton.h"
#ifdef MMI_TASK_MANAGER
#include "mmi_applet_table.h"
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA  DECLARE                           */
/*---------------------------------------------------------------------------*/
extern LINKS_TILE_T    s_links_board[LINKS_BOARD_ROWS][LINKS_BOARD_COLS]; //地图
PUBLIC uint8           s_links_timer = 0;       //计时器
PUBLIC BOOLEAN         s_links_is_paused = FALSE;  //是否暂停
PUBLIC uint8           s_game_over_timer = 0;      //计时器
PUBLIC LINKS_GAME_STATE_E   s_links_game_state = LINKS_GAME_NULL;
//LOCAL   BOOLEAN             s_is_backlight_turnoff = TRUE;

LOCAL const MMI_TEXT_ID_T s_links_class_list[LINKS_TOTAL_LEVEL] =
{
    TXT_CLASS_EASY,
    TXT_CLASS_NORMAL,
    TXT_CLASS_HARD,
};
#ifdef MMI_TASK_MANAGER
#define MMK_CreateGameLinkWin( _WIN_TABLE_, _ADD_DATA_ )  OpenGameLinkWin( _WIN_TABLE_, _ADD_DATA_ )
#else
#define MMK_CreateGameLinkWin   MMK_CreateWin
#endif

#ifdef MMI_TASK_MANAGER	
/*****************************************************************************/
//  Description : the process message function of the link game
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  GameLinkApplet_HandleEvent(    
            IAPPLET_T*          iapplet_ptr,
            MMI_MESSAGE_ID_E    msg_id, 
            DPARAM              param
            );

typedef struct 
{
    CAF_APPLET_T caf_applet;
    uint32  user_data;
}MMIGAMELINK_APPLET_T;   //vt applet定义

LOCAL MMIGAMELINK_APPLET_T *s_gamelink_applet_instance = PNULL; 
//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmigamelink_applet_array[] = 
{
    { GUID_NAME_DEF(SPRD_GAME_LINK_APPLET_ID), sizeof(MMIGAMELINK_APPLET_T), GameLinkApplet_HandleEvent , 
            IMAGE_SECMENU_ICON_PASTIME_LINKS, TXT_COMMON_GAME_LINK},
};

//applet的信息, 被mmi_applet_table.c extern
LOCAL const CAF_STATIC_APPLET_INFO_T g_mmigamelink_applet_info = 
{
    s_mmigamelink_applet_array,
    ARR_SIZE(s_mmigamelink_applet_array)
};

//#define MMIGMLINK_CAFEAPPLET(applet) ((applet)->caf_applet)
//#define MMIGMLINK_HANDLE(applet) ((MMIGMLINK_CAFEAPPLET(applet)).app_handle)
#endif


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
extern LINKS_LEVEL_E s_links_current_level;

extern PUBLIC uint8 g_links_map_row_num;
extern PUBLIC uint8 g_link_map_column_num;

extern  int32 g_LINKS_TILES_RECT_LEFT ;
extern int32 g_LINKS_TILES_RECT_TOP ;

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/            

//*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  FUNCTION:     HandleLinksNewWinMsg
//  Description :   handle new win msg
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLinksWinMsg( 
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        );


/*****************************************************************************/
//  Description : HandleQuintetMainMenuWindow
//       
//  Global resource dependence : 
//   Author:        taul.zhu
//   RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGMLinkMainMenuWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               );

/*****************************************************************************/
//  FUNCTION:     HandleLinksSViewWinMsg
//  Description :   查看分数
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLinksSViewWinMsg( 
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          );

/*****************************************************************************/
//  FUNCTION:     HandleLinksSResWinMsg
//  Description :   重设分数
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
/*
LOCAL MMI_RESULT_E HandleLinksSResWinMsg( 
MMI_WIN_ID_T win_id, 
MMI_MESSAGE_ID_E msg_id, 
DPARAM param
);
*/
/*****************************************************************************/
//  FUNCTION:      HandleLinksSetClassWinMsg
//  Description :  
//  Global resource dependence : 
//  Author:        Hao.Zhang 
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLinksSetClassWinMsg(
                                              MMI_WIN_ID_T win_id,  
                                              MMI_MESSAGE_ID_E msg_id,  
                                              DPARAM param
                                              );

/*****************************************************************************/
//  FUNCTION:     HandleLinksHelpWinMsg
//  Description :   handle help win msg
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLinksHelpWinMsg( 
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         );

/*****************************************************************************/
//  FUNCTION:      ConfirmLinksSetClassWinMsg
//  Description :  
//  Global resource dependence : 
//  Author:        Hao.Zhang 
//  Note:   
/*****************************************************************************/
LOCAL void  ConfirmLinksSetClassWinMsg(void);

/*****************************************************************************/
//  FUNCTION:      CancelLinksSetClassWinMsg
//  Description :  
//  Global resource dependence : 
//  Author:        Hao.Zhang 
//  Note:   
/*****************************************************************************/
LOCAL void  CancelLinksSetClassWinMsg(void);

/*****************************************************************************/
//  FUNCTION:     LcdSwitch
//  Description :   
//  Global resource dependence : 
//  Author:ying.xu
//  Note:   
/*****************************************************************************/
LOCAL  void LcdSwitch( MMI_WIN_ID_T win_id);

LOCAL  MMI_RESULT_E  HandleReturnButtonCallback(void);

#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : Get WWW Applet Instance
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC MMIGAMELINK_APPLET_T *MMIGMLINK_GetAppletInstance(void);

/*****************************************************************************/
//  Description : Create link setting win
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T OpenGameLinkWin(
                                  uint32*            win_table_ptr,
                                  ADD_DATA           add_data_ptr
                                  );
#endif

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
WINDOW_TABLE( MMIGMLINK_MAIN_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleGMLinkMainMenuWindow ),    
    WIN_ID( MMIGMLINK_MAIN_MENU_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_LINKS, MMIGMLINK_MAIN_MENU_CTRL_ID),
    END_WIN
};

//  begin new game
WINDOW_TABLE( LINKS_NEW_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleLinksWinMsg ),    
    WIN_ID( LINKS_NEW_WIN_ID ),
#if (defined MMI_PDA_SUPPORT) ||( defined TOUCHPANEL_TYPE)
    WIN_HIDE_STATUS,
#endif
    // WIN_TITLE( TXT_HIGH_SCORE ),
    // WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#ifndef MMI_PDA_SUPPORT
    CREATE_BUTTON_CTRL(IMAGE_LINKS_RETURN_BUTTON_BACKGROUND, MMIGMLINK_BUTTON_CTRL_ID),
#endif
    END_WIN
}; 

// score viewing
WINDOW_TABLE( LINKS_SVIEW_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleLinksSViewWinMsg ),    
    WIN_ID( LINKS_SVIEW_WIN_ID ),
    WIN_TITLE( TXT_HIGH_SCORE ),
    /*
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMIGAME_LINKS_SCORE_LABEL_CTRL_ID),
    */
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, TXT_RESET, TXT_NULL),
#else
    WIN_SOFTKEY(TXT_RESET, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

// set class of Links
WINDOW_TABLE( LINKS_SETCLASS_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleLinksSetClassWinMsg ),    
    WIN_ID( LINKS_SETCLASS_WIN_ID),
    WIN_TITLE(TXT_CLASS),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIGMLINK_SETCLASS_CTRL_ID),
    END_WIN
}; 

WINDOW_TABLE( LINKS_INFO_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleLinksHelpWinMsg ),    
    WIN_ID( LINKS_INFO_WIN_ID ),
    WIN_TITLE(TXT_GAME_INFO),
    WIN_SOFTKEY(TXT_NULL,TXT_NULL,STXT_RETURN),
    CREATE_TEXT_CTRL(MMIGMLINK_STATIC_TEXT_CTRL_ID),
    END_WIN
}; 


/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/



/**---------------------------------------------------------------------------*
**                       function bodies 
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//   FUNCTION:     MMIGMQUT_OpenGameWin
//  Description :   
//
//  Global resource dependence : 
//   Author:      
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIGMLINK_OpenGameWin(void)
{
#ifndef MMI_TASK_MANAGER
    return (MMK_CreateGameLinkWin((uint32*)MMIGMLINK_MAIN_MENU_WIN_TAB,PNULL));
#else
    MMI_APPLET_START_T start ={0};
    
    //SCI_TRACE_LOW:"MMIAPIGMLINK_OpenGameWin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_WINTAB_340_112_2_18_2_21_33_21,(uint8*)"");
    start.guid = SPRD_GAME_LINK_APPLET_ID;
    start.state = /*MMI_APPLET_STATE_BG_HIDE_WIN|*/MMI_APPLET_STATE_TASK_MANAGER;
    MMK_StartApplet( &start );
    return TRUE;
#endif    
}
/*****************************************************************************/
//  FUNCTION:     ViewScore
//  Description :   dispaly score information
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void ViewScore(
                      MMI_WIN_ID_T win_id,
                      MMI_CTRL_ID_T ctrl_id, 
                      MMI_TEXT_ID_T  title_id,
                      uint16 score)
{
    uint8 asc_str[12] = {0};
    wchar wstr[12] = {0};
    MMI_STRING_T  str2 = {0};
    MMI_STRING_T  temp_str = {0};
    MMI_STRING_T  result = {0};

   GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T          rect            = MMITHEME_GetClientRect();
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
  
   
    MMI_GetLabelTextByLang(title_id, &temp_str);
    
    sprintf((char*)asc_str,":%d",score);
    
    //str2.is_ucs2 = FALSE;
    str2.wstr_len = strlen((char*)asc_str);
    str2.wstr_ptr = wstr;
    MMI_STRNTOWSTR( wstr, 12, asc_str, 12, str2.wstr_len );
     result = MMIGMLINK_AddTwoString(temp_str,str2);
    
    
    //GUILABEL_SetText(MMIGAME_LINKS_SCORE_LABEL_CTRL_ID, &result, FALSE);
   
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font= MMI_DEFAULT_NORMAL_FONT;
    text_style.font_color = COLOR_LINKS_STRING;
  // display
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&result,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );       
    
    SCI_FREE(result.wstr_ptr);
   
  
    //    MMK_SetAtvCtrl(win_id, ctrl_id);
    
}

/*****************************************************************************/
//  Description : HandleQuintetMainMenuWindow
//       
//  Global resource dependence : 
//   Author:        taul.zhu
//   RETRUN:   
//  Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleGMLinkMainMenuWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param)
                                               
{
    uint32          group_id = 0; 
    uint32          menu_id  = 0;
    MMI_RESULT_E    recode   = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(win_id, MMIGMLINK_MAIN_MENU_CTRL_ID);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        GUIMENU_GetId(MMIGMLINK_MAIN_MENU_CTRL_ID,&group_id,&menu_id);
        switch(menu_id) 
        {
        case ID_GMLINKS_NEW:
            MMK_CreateGameLinkWin((uint32 *)  LINKS_NEW_WIN_TAB, (ADD_DATA)menu_id);
            break;
            
        case ID_GMLINKS_RES:
            MMK_CreateGameLinkWin((uint32 *) LINKS_NEW_WIN_TAB, (ADD_DATA)menu_id);
            break;
            
        case ID_GMLINKS_SCORE_VIEW:
            MMK_CreateGameLinkWin((uint32 *) LINKS_SVIEW_WIN_TAB, PNULL);
            break;
            
        case ID_GMLINKS_CLASS:
            MMK_CreateGameLinkWin((uint32 *) LINKS_SETCLASS_WIN_TAB, PNULL);
            break;
        case ID_GMLINKS_INFO:
            MMK_CreateGameLinkWin((uint32 *) LINKS_INFO_WIN_TAB, PNULL);
            break;
            
        default:
            break;
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
#ifndef MMI_TASK_MANAGER
        MMK_CloseWin(win_id);
#else
        MMIAPIGMLink_Stop(FALSE);
#endif
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
/*****************************************************************************/
//  FUNCTION:     HandleLinksNewWinMsg
//  Description :   handle new win msg
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLinksWinMsg( 
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
#ifdef TOUCH_PANEL_SUPPORT    
    GUI_POINT_T point = {0};
    GUI_RECT_T rect = {0};
#endif

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if( ID_GMLINKS_NEW == (uint32)MMK_GetWinAddDataPtr(MMK_ConvertIdToHandle(win_id)))
        {
            StartGameLinks();
        }
        else
        {
            ResumeGameLinks();
        }
        s_links_game_state = LINKS_GAME_ON;
        g_links_map_row_num = 0;
        g_link_map_column_num = 0;
        // 背光常亮
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        LcdSwitch(win_id);
        break;
        
    case MSG_TIMER:
        if (s_links_timer == *(uint8*)param)
        {
            OnTimerAction(win_id);
        }
        else if (s_game_over_timer == *(uint8*)param)
        {
            MMK_StopTimer(s_game_over_timer);
            s_game_over_timer = 0;
            MMK_CloseWin(win_id);
            s_links_game_state = LINKS_GAME_NULL;
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

    case MSG_LCD_SWITCH:
        LcdSwitch(win_id);
        break;
        
    case MSG_FULL_PAINT:
        if (LINKS_GAME_ON == s_links_game_state)
        {
            DrawAll(win_id);
        }
        else if (LINKS_GAME_OVER == s_links_game_state)
        {
            DrawGameOverScreen(win_id);
        }
        else if (LINKS_GAME_PASS == s_links_game_state)
        {
            DrawGameFinishScreen(win_id);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
    {
        GUI_RECT_T rect1 = {0}; //LINKS_PAUSE_RECT;
        GUI_RECT_T rect2 = {0} ; //LINKS_TIP_RECT;
        GUI_RECT_T rect3 = {0} ; //LINKS_WASHCARD_RECT;
   
        if(MMITHEME_IsMainScreenLandscape())
        {
            rect1.left = LINKS_PAUSE_STRING_RECT_H_LEFT;
            rect1.top =  LINKS_PAUSE_STRING_RECT_H_TOP;
            rect1.right = LINKS_PAUSE_STRING_RECT_H_RIGHT;
            rect1.bottom = LINKS_PAUSE_STRING_RECT_H_BOTTOM;
            rect2.left = LINKS_TIP_STRING_RECT_H_LEFT;
            rect2.top =  LINKS_TIP_STRING_RECT_H_TOP;
            rect2.right = LINKS_TIP_STRING_RECT_H_RIGHT;
            rect2.bottom = LINKS_TIP_STRING_RECT_H_BOTTOM;
            rect3.left = LINKS_WASHCARD_STRING_RECT_H_LEFT;
            rect3.top =  LINKS_WASHCARD_STRING_RECT_H_TOP;
            rect3.right = LINKS_WASHCARD_STRING_RECT_H_RIGHT;
            rect3.bottom = LINKS_WASHCARD_STRING_RECT_H_BOTTOM;
        }
        else
        {        
            rect1.left = LINKS_PAUSE_STRING_RECT_LEFT;
            rect1.top =  LINKS_PAUSE_STRING_RECT_TOP;
            rect1.right = LINKS_PAUSE_STRING_RECT_RIGHT;
            rect1.bottom = LINKS_PAUSE_STRING_RECT_BOTTOM;
            rect2.left = LINKS_TIP_STRING_RECT_LEFT;
            rect2.top =  LINKS_TIP_STRING_RECT_TOP;
            rect2.right = LINKS_TIP_STRING_RECT_RIGHT;
            rect2.bottom = LINKS_TIP_STRING_RECT_BOTTOM;
            rect3.left = LINKS_WASHCARD_STRING_RECT_LEFT;
            rect3.top =  LINKS_WASHCARD_STRING_RECT_TOP;
            rect3.right = LINKS_WASHCARD_STRING_RECT_RIGHT;
            rect3.bottom = LINKS_WASHCARD_STRING_RECT_BOTTOM;
        }   
    
        if (LINKS_GAME_ON == s_links_game_state)
        {
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            rect.left = g_LINKS_TILES_RECT_LEFT;
            rect.right = LINKS_TILE_X(9);
            rect.top = g_LINKS_TILES_RECT_TOP;
            rect.bottom = LINKS_TILE_Y(9);
            //点击在小图片区
            if (GUI_PointIsInRect(point, rect))
            {
                g_links_map_row_num = LINKS_TILE_ROW(point.y);
                g_link_map_column_num = LINKS_TILE_COL(point.x);
                //点击的不是空白
                if (s_links_board[LINKS_TILE_ROW(point.y)][LINKS_TILE_COL(point.x)].id != LINKS_TILE_BLANK)
                {
                    OnTileClicked(win_id, s_links_board[LINKS_TILE_ROW(point.y)][LINKS_TILE_COL(point.x)]);
                }
            }
            //点击在暂停按钮
            else if (GUI_PointIsInRect(point, rect1))
            {
                OnPauseClicked(win_id);
            }
            //点击在提示按钮
            else if (GUI_PointIsInRect(point, rect2))
            {
                OnTipClicked(win_id);
            }
            //点击在洗牌按钮
            else if (GUI_PointIsInRect(point, rect3))
            {
                OnWashCardClicked(win_id);
            }
        }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:
        if ((LINKS_GAME_ON == s_links_game_state)&&(!s_links_is_paused))
        {
            ProcessKeyWinMsg(win_id, msg_id);
        }
        break;

    case MSG_LOSE_FOCUS:
    case MSG_CLOSE_WINDOW:
        // 背光被允许关闭
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        if (s_links_timer >0 )
        {
            MMK_StopTimer(s_links_timer);
            s_links_timer = 0;
        }
        
        if (s_game_over_timer > 0)
        {
            MMK_StopTimer(s_game_over_timer);
            s_game_over_timer = 0;
            //s_links_game_state = LINKS_GAME_NULL;
        }
        
        //added by ying.xu,20100528,cr_MS00181656
        //游戏窗口被关闭，则暂停游戏
        if (!s_links_is_paused) 
        {
            s_links_is_paused = !s_links_is_paused;
            DrawPauseScreen(win_id);
        }
        // end of cr_MS00181656

        break;
        
    case MSG_APP_WEB:
        if((LINKS_GAME_ON == s_links_game_state)&&
            (s_links_board[g_links_map_row_num][g_link_map_column_num].id != LINKS_TILE_BLANK))
        {
            OnTileClicked(win_id,s_links_board[g_links_map_row_num][g_link_map_column_num]);
        }
        break;
        
    case MSG_GET_FOCUS:
        // 背光常亮
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
       // s_is_backlight_turnoff = FALSE;

        if (0 == s_links_timer && !s_links_is_paused)
        {
             s_links_timer = MMK_CreateTimer( 1000, TRUE);
        }
        break;
    
 
    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        if (s_links_timer >0 )
        {
            MMK_StopTimer(s_links_timer);
            s_links_timer = 0;
        }
        if (s_game_over_timer > 0)
        {
            MMK_StopTimer(s_game_over_timer);
            s_game_over_timer = 0;
            s_links_game_state = LINKS_GAME_NULL;
        }
        MMK_CloseWin(win_id);

        //added by ying.xu,20100531,MS00181020
        //最高分在手动退出时并不保存，只有在定时器超时后才保存
        SaveGameScore();
        // end of MS00181020
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  FUNCTION:     HandleLinksSViewWinMsg
//  Description :   查看分数
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLinksSViewWinMsg( 
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          )
{
    MMI_RESULT_E        recode          = MMI_RESULT_TRUE;
    uint16              high_score[3]   = {0};
    uint16              nv_return       = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_POINT_T         startpoint      = {0};
  
    // MMI_STRING_T        string = {0};
    // GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    // GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    // uint8 len = 0;
    // char pstr[12] = {0};
    // wchar wstr[12] = {0};
    
    // point
    startpoint.x            = 0;
    startpoint.y            = MMI_TITLE_HEIGHT;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //GUIWIN_SetSoftkeyTextId(win_id,  TXT_RESET,TXT_NULL, STXT_RETURN,FALSE);
        break;
        
        //@Hao Zhang 2005.9.14 cr31097
    case MSG_FULL_PAINT:
        MMINV_READ(MMINV_GMLINK_HIGH_SCORE, (int16*)high_score, nv_return);
        GUIRES_DisplayImg(&startpoint,
            PNULL,
            PNULL,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);
        ViewScore(win_id, MMIGAME_SCORE_MSGBOX_CTRL_ID, TXT_HIGH_SCORE, high_score[1]);
  
        break;
        
        
    case MSG_GET_FOCUS:
        //MMK_SendMsg(MMK_GetPrevWinId(win_id), MSG_FULL_PAINT, PNULL);
        break;
        
    case MSG_TIMER:
        // if (s_links_timer == *(uint8*)param)  MMK_CloseWin(win_id);
        break;

#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;

            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK,PNULL);
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
    case MSG_APP_WEB:
#ifndef MMI_TASK_MANAGER
        MMIPUB_OpenQueryWinByTextId(TXT_RESET,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
#else        
        MMIPUB_OpenQueryWinByTextIdEx(SPRD_GAME_LINK_APPLET_ID, 
                TXT_RESET,IMAGE_PUBWIN_QUERY,PNULL,PNULL, 0);
#endif        
        //MMK_CreateWin((uint32*)LINKS_SRES_WIN_TAB,NULL);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_PROMPTWIN_OK:
#ifndef MMI_TASK_MANAGER
       MMIPUB_CloseQuerytWin(PNULL);
#else
        MMIPUB_CloseQuerytWinEx(SPRD_GAME_LINK_APPLET_ID, PNULL);
#endif
        MMINV_READ(MMINV_GMLINK_HIGH_SCORE,( int16*)high_score, nv_return);
        high_score[1] = 0;
        MMINV_WRITE(MMINV_GMLINK_HIGH_SCORE, ( int16*)high_score);
        ViewScore(win_id, MMIGAME_SCORE_MSGBOX_CTRL_ID, TXT_HIGH_SCORE, high_score[1]);   
        break;
        
    case MSG_PROMPTWIN_CANCEL:
#ifndef MMI_TASK_MANAGER
       MMIPUB_CloseQuerytWin(PNULL);
#else
        MMIPUB_CloseQuerytWinEx(SPRD_GAME_LINK_APPLET_ID, PNULL);
#endif
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  FUNCTION:     HandleLinksSResWinMsg
//  Description :   重设分数
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
/*
LOCAL MMI_RESULT_E HandleLinksSResWinMsg( 
MMI_WIN_ID_T win_id, 
MMI_MESSAGE_ID_E msg_id, 
DPARAM param)
{
MMI_RESULT_E recode = MMI_RESULT_TRUE;
uint16 high_score[3];
uint16 nv_return;
GUI_POINT_T startpoint;
GUI_LCD_DEV_INFO    lcd_dev_info = {0};
startpoint.x=0;
startpoint.y=MMI_TITLE_HEIGHT;

  lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
  lcd_dev_info.block_id = GUI_BLOCK_MAIN;  
  switch(msg_id)
  {
  case MSG_OPEN_WINDOW:
  s_links_timer = 0;
  GUIWIN_SetSoftkeyTextId(win_id,COMMON_TXT_NULL,COMMON_TXT_NULL,STXT_RETURN,FALSE);
  
    MMIPUB_OpenQueryWinByTextId(TXT_RESET,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
             break;
             
               //@Hao Zhang 2005.9.14 cr31097
               case MSG_FULL_PAINT:
               LCD_DisplayCompressedBmp(
               &lcd_dev_info,
               startpoint,
               MMI_GetLabelImage(IMAGE_COMMON_BG)
               );
               MMINV_READ(MMINV_GMLINK_HIGH_SCORE,( int16*)high_score, nv_return);
               ViewScore(win_id, MMIGAME_SCORE_MSGBOX_CTRL_ID, TXT_HIGH_SCORE, high_score[1]);
               break;
               //@Hao Zhang
               
                 case MSG_LOSE_FOCUS:
                 s_links_timer = 0;
                 break;
                 
                   case MSG_GET_FOCUS:
                   //            MMK_SendMsg(MMK_GetPrevWinId(win_id), MSG_FULL_PAINT, PNULL);
                   break;
                   
                     case MSG_TIMER:
                     //            if (s_links_timer == *(uint8*)param)  MMK_CloseWin(win_id);
                     break;
                     
                       case MSG_APP_CANCEL:
                       case MSG_CTL_CANCEL:
                       //case MSG_APP_CANCEL:
                       MMK_CloseWin(win_id);
                       s_links_timer = 0;
                       break;
                       
                         case MSG_PROMPTWIN_OK:
                         MMK_CloseWin(win_id);
                         MMIPUB_CloseQuerytWin(PNULL);
                         MMINV_READ(MMINV_GMLINK_HIGH_SCORE,( int16*)high_score, nv_return);
                         high_score[1] = 0;
                         MMINV_WRITE(MMINV_GMLINK_HIGH_SCORE, ( int16*)high_score);
                         break;
                         
                           case MSG_PROMPTWIN_CANCEL:
                           MMIPUB_CloseQuerytWin(PNULL);
                           MMK_SendMsg(win_id, MSG_APP_CANCEL, NULL);
                           break;
                           
                             default:
                             recode = MMI_RESULT_FALSE;
                             break;
                             }
                             
                               return recode;
                               }
                               */
                               
                               /*****************************************************************************/
                               //  FUNCTION:     HandleLinksSResWinMsg
                               //   Description :   重设分数
                               //   Global resource dependence : 
                               //  Author:       Hao.Zhang
                               //   Note:   
                               /*****************************************************************************/
                               /*
                               LOCAL MMI_RESULT_E HandleLinksSResWinMsg( 
                               MMI_WIN_ID_T win_id, 
                               MMI_MESSAGE_ID_E msg_id, 
                               DPARAM param
                               )
                               {
                               MMI_RESULT_E        recode  = MMI_RESULT_TRUE;
                               uint16              high_score[3] = {0, 0, 0};
                               uint16              nv_return     = 0;
                               GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
                               GUI_POINT_T         startpoint      = {0};
                               
                                 switch(msg_id)
                                 {
                                 case MSG_OPEN_WINDOW:
                                 s_links_timer = 0;
                                 GUIWIN_SetSoftkeyTextId(win_id,COMMON_TXT_NULL,COMMON_TXT_NULL,STXT_RETURN,FALSE);
                                 
                                   //@Hao Zhang 2005.9.14 cr31097
                                   //MMINV_READ(MMINV_GAME_HIGH_SCORE,( int16*)high_score, nv_return);
                                   //high_score[1] = 0;
                                   //MMINV_WRITE(MMINV_GAME_HIGH_SCORE, ( int16*)high_score);
                                   //ViewScore(win_id, MMIGAME_SCORE_MSGBOX_CTRL_ID, TXT_HIGH_SCORE, 0);
                                   //@Hao Zhang
                                   
                                     //            s_links_timer = MMK_CreateTimer(2000, FALSE);
                                     GUIWIN_SetSoftkeyTextId(win_id,COMMON_TXT_NULL,COMMON_TXT_NULL,STXT_RETURN,FALSE);
                                     MMIPUB_OpenQueryWinByTextId(TXT_RESET,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                                     break;
                                     
                                       //@Hao Zhang 2005.9.14 cr31097
                                       case MSG_FULL_PAINT:
                                       MMINV_READ(MMINV_GMLINK_HIGH_SCORE, (int16*)high_score, nv_return);
                                       startpoint.x = 0;
                                       startpoint.y = MMI_TITLE_HEIGHT;
                                       LCD_DisplayCompressedBmp(&lcd_dev_info, startpoint, MMI_GetLabelImage(IMAGE_COMMON_BG,win_id));
                                       ViewScore(win_id, MMIGAME_SCORE_MSGBOX_CTRL_ID, TXT_HIGH_SCORE, high_score[1]);
                                       break;
                                       //@Hao Zhang
                                       
                                         case MSG_LOSE_FOCUS:
                                         //            MMK_StopTimer(s_links_timer);
                                         //            MMK_CloseWin(win_id);
                                         s_links_timer = 0;
                                         break;
                                         
                                           case MSG_GET_FOCUS:
                                           //            MMK_SendMsg(MMK_GetPrevWinId(win_id), MSG_FULL_PAINT, PNULL);
                                           break;
                                           
                                             case MSG_TIMER:
                                             //            if (s_links_timer == *(uint8*)param)  MMK_CloseWin(win_id);
                                             break;
                                             
                                               case MSG_APP_CANCEL:
                                               case MSG_CTL_CANCEL:
                                               //case MSG_APP_CANCEL:
                                               MMK_CloseWin(win_id);
                                               s_links_timer = 0;
                                               break;
                                               
                                                 case MSG_PROMPTWIN_OK:
                                                 MMK_CloseWin(win_id);
                                                 MMIPUB_CloseQuerytWin(PNULL);
                                                 MMINV_READ(MMINV_GMLINK_HIGH_SCORE,( int16*)high_score, nv_return);
                                                 high_score[1] = 0;
                                                 MMINV_WRITE(MMINV_GMLINK_HIGH_SCORE, ( int16*)high_score);
                                                 MMK_CreateWin((uint32 *) LINKS_SVIEW_WIN_TAB, PNULL);
                                                 break;
                                                 
                                                   case MSG_PROMPTWIN_CANCEL:
                                                   MMIPUB_CloseQuerytWin(PNULL);
                                                   MMK_SendMsg(win_id, MSG_APP_CANCEL, NULL);
                                                   
                                                     break;
                                                     
                                                       default:
                                                       recode = MMI_RESULT_FALSE;
                                                       break;
                                                       }
                                                       
                                                         return recode;
                                                         }
*/
/*****************************************************************************/
//  FUNCTION:      ConfirmLinksSetClassWinMsg
//  Description :  
//  Global resource dependence : 
//  Author:        Hao.Zhang 
//  Note:   
/*****************************************************************************/
LOCAL void  ConfirmLinksSetClassWinMsg(void)
{
    int8    level[6]  = {0};    
    uint16  sel       = 0;
    uint16  nv_return = 0;    
    
    sel = GUILIST_GetCurItemIndex(MMIGMLINK_SETCLASS_CTRL_ID);
    s_links_current_level = (LINKS_LEVEL_E)sel;
    
    MMINV_READ(MMINV_GMLINK_LEVER_SET,( int16*)level, nv_return);   
    level[2] = sel; 
    MMINV_WRITE(MMINV_GMLINK_LEVER_SET, (int16*)level); 
             
/*#ifndef MMI_TASK_MANAGER
    MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#else
    MMIPUB_OpenAlertWinByTextIdEx(SPRD_GAME_LINK_APPLET_ID, 
            PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,
            PNULL);
#endif*/
    
    MMK_CloseWin(LINKS_SETCLASS_WIN_ID);
}

/*****************************************************************************/
//  FUNCTION:      CancelLinksSetClassWinMsg
//  Description :  
//  Global resource dependence : 
//  Author:        Hao.Zhang 
//  Note:   
/*****************************************************************************/
LOCAL void  CancelLinksSetClassWinMsg(void)
{
    MMK_CloseWin(LINKS_SETCLASS_WIN_ID);
}

/*****************************************************************************/
//  FUNCTION:      HandleLinksSetClassWinMsg
//  Description :  
//  Global resource dependence : 
//  Author:        Hao.Zhang 
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLinksSetClassWinMsg(
                                              MMI_WIN_ID_T win_id,  
                                              MMI_MESSAGE_ID_E msg_id,  
                                              DPARAM param
                                              )
{
    int8          level[6]  = {0};  
    //  uint16        sel       = 0;
    uint16        nv_return = 0;       
    //MMI_TEXT_ID_T text[4]   = {0};
    //MMI_STRING_T  string    = {0};
    MMI_RESULT_E  recode    = MMI_RESULT_TRUE;
    uint32 i =0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:      //create radio list box
        GUILIST_SetMaxItem(MMIGMLINK_SETCLASS_CTRL_ID, LINKS_TOTAL_LEVEL, FALSE );//max item 3
        //MMIACC_AppendListBoxItem(MMIGMLINK_SETCLASS_CTRL_ID, &s_links_class_list[0], LINKS_TOTAL_LEVEL);
        for(i = 0; i < LINKS_TOTAL_LEVEL; i++)
        {
            MMIAPISET_AppendListItemByTextId(s_links_class_list[i], STXT_OPTION, MMIGMLINK_SETCLASS_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO );
        }
        MMINV_READ(MMINV_GMLINK_LEVER_SET,( int16*)level, nv_return);
        if (level[2] >= LINKS_TOTAL_LEVEL || level[2] < 0)
        {
            level[2] = LINKS_LEVEL_EASY;
        }
        s_links_current_level = (LINKS_LEVEL_E)level[2];
        GUILIST_SetCurItemIndex( MMIGMLINK_SETCLASS_CTRL_ID, (uint16)s_links_current_level);
        GUILIST_SetSelectedItem(MMIGMLINK_SETCLASS_CTRL_ID, (uint16)s_links_current_level, TRUE);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK,COMMON_TXT_NULL,STXT_RETURN,FALSE);
        MMK_SetAtvCtrl( win_id,  MMIGMLINK_SETCLASS_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK: 
    case MSG_KEYDOWN_OK:
    case MSG_KEYDOWN_WEB:
        SetResumValue(FALSE);
        ConfirmLinksSetClassWinMsg();
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        CancelLinksSetClassWinMsg();
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
    
    
}

/*****************************************************************************/
//  FUNCTION:     HandleLinksHelpWinMsg
//  Description :   handle help win msg
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLinksHelpWinMsg( 
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_STRING_T    string = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIGMLINK_STATIC_TEXT_CTRL_ID;


    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:		
        MMI_GetLabelTextByLang(TXT_LINKS_HELP, &string);
        GUITEXT_SetString(ctrl_id,string.wstr_ptr,string.wstr_len,FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
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

/*****************************************************************************/
//  FUNCTION:     LcdSwitch
//  Description :   
//  Global resource dependence : 
//  Author:ying.xu
//  Note:   
/*****************************************************************************/
LOCAL  void LcdSwitch( MMI_WIN_ID_T win_id)
{   
    GUI_FONT_ALL_T font_all = {0};

    font_all.font = MMI_DEFAULT_NORMAL_FONT;
    font_all.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT);
    if(MMITHEME_IsMainScreenLandscape())
    {
     g_LINKS_TILES_RECT_LEFT = LINKS_TILES_RECT_LEFT_H;
     g_LINKS_TILES_RECT_TOP = LINKS_TILES_RECT_TOP_H;   
     
    {
        GUI_RECT_T rect = {0};
     
    //GUIRES_GetImgWidthHeight(&button_width, &button_height, 
        //IMAGE_PAUSE_BACKGROUND_ICON , win_id);
    #ifdef MAINLCD_SIZE_176X220//modify by mary
        rect.right  =  LINKS_RETURN_BUTTON_RECT_H_RIGHT;
        rect.top = LINKS_RETURN_BUTTON_RECT_H_TOP;
        rect.left =  LINKS_RETURN_BUTTON_RECT_H_LEFT;
        rect.bottom  =  LINKS_RETURN_BUTTON_RECT_H_BOTTOM ;
        GUIBUTTON_SetRect(MMIGMLINK_BUTTON_CTRL_ID, &rect);
    #elif defined MAINLCD_SIZE_240X400
        rect.right  =  LINKS_RETURN_BUTTON_RECT_H_RIGHT;
        rect.bottom =  LINKS_RETURN_BUTTON_RECT_H_BOTTOM;
        rect.left =  LINKS_RETURN_BUTTON_RECT_H_LEFT;
        rect.top =  LINKS_RETURN_BUTTON_RECT_H_TOP ;
        GUIBUTTON_SetRect(MMIGMLINK_BUTTON_CTRL_ID, &rect);
    #else
        {
            GUI_RECT_T  full_rect = MMITHEME_GetFullScreenRect();
            uint16 button_height = 0;
            uint16 button_width = 0;   

            rect.right  =  full_rect.right - 4;
            rect.bottom =  full_rect.bottom - 4;
            GUIRES_GetImgWidthHeight(&button_width, &button_height, 
                    IMAGE_LINKS_RETURN_BUTTON_BACKGROUND , win_id);
            rect.left =  rect.right - button_width;
            rect.top =  rect.bottom -  button_height ;
            GUIBUTTON_SetRect(MMIGMLINK_BUTTON_CTRL_ID, &rect);
        }
    #endif
    }

        GUIBUTTON_SetTextAlign( MMIGMLINK_BUTTON_CTRL_ID,   ALIGN_HVMIDDLE) ;
        GUIBUTTON_SetFont(MMIGMLINK_BUTTON_CTRL_ID, &font_all);
        GUIBUTTON_SetTextId(MMIGMLINK_BUTTON_CTRL_ID,  STXT_RETURN);   
        GUIBUTTON_SetVisible(MMIGMLINK_BUTTON_CTRL_ID,TRUE, FALSE);  
        GUIBUTTON_SetCallBackFunc(MMIGMLINK_BUTTON_CTRL_ID , HandleReturnButtonCallback );
#ifndef MMI_PDA_SUPPORT	//pda大多数场景不显示softkey		
        MMK_DestroyDynaCtrl( MMK_GetCtrlHandleByWin(win_id , MMITHEME_GetSoftkeyCtrlId()));
#endif
    }
    else
    {
        g_LINKS_TILES_RECT_LEFT = LINKS_TILES_RECT_LEFT_S;
        g_LINKS_TILES_RECT_TOP = LINKS_TILES_RECT_TOP_S;
        GUIBUTTON_SetVisible(MMIGMLINK_BUTTON_CTRL_ID,FALSE, FALSE);
#ifndef MMI_PDA_SUPPORT	//pda大多数场景不显示softkey
        GUISOFTKEY_CreateCtrlDyn(win_id,  MMITHEME_GetSoftkeyCtrlId(),PNULL,  PNULL,  STXT_RETURN);
#endif
    }
}

/*****************************************************************************/
//  FUNCTION:     HandleReturnButtonCallback
//  Description :   
//  Global resource dependence : 
//  Author:ying.xu
//  Note:   
/*****************************************************************************/
LOCAL  MMI_RESULT_E  HandleReturnButtonCallback(void)
{
       SaveGameScore();
       MMK_CloseWin(LINKS_NEW_WIN_ID);  
       return MMI_RESULT_TRUE ;
}

#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : register link game module applet info
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIGMLINK_RegAppletInfo(void)
{
    MMI_RegAppletInfo(MMI_MODULE_GAME_LINK, &g_mmigamelink_applet_info);
}


/*****************************************************************************/
//  Description : the process message function of the link game
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  GameLinkApplet_HandleEvent(    
    IAPPLET_T*          iapplet_ptr,
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
    )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIGAMELINK_APPLET_T* applet_ptr = (MMIGAMELINK_APPLET_T*)iapplet_ptr;

    //SCI_TRACE_LOW:"mmi GameLinkApplet_HandleEvent msg_id = 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_WINTAB_1248_112_2_18_2_21_35_22,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MSG_APPLET_START:
        s_gamelink_applet_instance = applet_ptr;
        MMK_CreateGameLinkWin((uint32*)MMIGMLINK_MAIN_MENU_WIN_TAB,PNULL);
        break;
        
    case MSG_APPLET_STOP: 
        s_gamelink_applet_instance = PNULL;
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : Get WWW Applet Instance
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC MMIGAMELINK_APPLET_T *MMIGMLINK_GetAppletInstance(void)
{
    return s_gamelink_applet_instance;
}

/*****************************************************************************/
//  Description : Create link reference window for MMI_APPLET_STATE_BG_HIDE_WIN
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T OpenGameLinkWin(
                                  uint32*            win_table_ptr,
                                  ADD_DATA           add_data_ptr
                                  )
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = SPRD_GAME_LINK_APPLET_ID;//MMIGMLINK_HANDLE(MMIGMLINK_GetAppletInstance());
    win_table_create.win_table_ptr = win_table_ptr;
    win_table_create.add_data_ptr = add_data_ptr;
    
    return MMK_CreateWinTable(&win_table_create);
}

 /*****************************************************************************/
//  Description : Stop game link
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIGMLink_Stop(BOOLEAN is_minimize)
{ 
#if 0
    MMIGAMELINK_APPLET_T    *applet_ptr = MMIGMLINK_GetAppletInstance();

    if (PNULL != applet_ptr)
    {
        MMI_HANDLE_T applet_handle = MMIGMLINK_HANDLE(applet_ptr);

        if (is_minimize)
        {
            //MMK_SetAppletState(applet_handle, MMI_APPLET_STATE_BG_HIDE_WIN, TRUE);
            MMK_HideWinByApplet(applet_handle);
        }
        else
        {
            MMK_CloseApplet(applet_handle);
        }
    }
#endif
    if (is_minimize)
    {
        //MMK_SetAppletState(applet_handle, MMI_APPLET_STATE_BG_HIDE_WIN, TRUE);
        MMK_HideWinByApplet(SPRD_GAME_LINK_APPLET_ID);
    }
    else
    {
        MMK_CloseApplet(SPRD_GAME_LINK_APPLET_ID);
    }
}

#endif

#endif


/*Edit by script, ignore 1 case. Thu Apr 26 19:00:54 2012*/ //IGNORE9527


/*Edit by script, ignore 3 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
