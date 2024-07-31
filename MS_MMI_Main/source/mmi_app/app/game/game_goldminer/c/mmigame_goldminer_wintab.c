/***************************************************************************
** File Name:      mmigame_goldminer_wintab.c                              *                          
** Author:                                                                 *  
** Date:           06/09/2012                                              *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.     * 
** Description:    This file is used to create windows of game             *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                         *    
** 06/09/2012                          Create                              * 
****************************************************************************/
#define _MMIGAME_GOLDMINER_WINTAB_C_


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_GOLDMINER_SUPPORT
#include "window_parse.h"
#include "mmigame_goldminer_internal.h"
#include "mmigame_goldminer_text.h"
#include "mmigame_goldminer_image.h"
#include "mmigame_goldminer_nv.h"
#include "mmigame_goldminer_id.h" 
#include "mmigame_goldminer_menutable.h"
#include "mmigame_goldminer_control.h" 
#include "mmigame_goldminer_position.h"
#include "mmk_app.h"
#include "guimsgbox.h"
#include "guilcd.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "guitext.h"
#include "mmidisplay_data.h"
#include "mmigame_menutable.h"
#include "tb_dal.h"
#include "mmi_textfun.h"
#include "mmigame_id.h"
#include "window_parse.h"
#include "mmi_common.h"
#include "mmi_theme.h"
#include "mmi_nv.h"
#include "mmipub.h"
#include "mmk_timer.h"
#include "mmi_default.h"
#include "guires.h"  
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA  DECLARE                           */
/*---------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/    

//*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  FUNCTION:     HandleGoldMinerNewWinMsg
//  Description :   
//  Global resource dependence : 
//  Author: 
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGoldMinerNewWinMsg( 
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        );

/*****************************************************************************/
//  Description : HandleGMGoldMinerMainMenuWindow       
//  Global resource dependence : 
//  Author:   
//  RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGMGoldMinerMainMenuWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               );

/*****************************************************************************/
//  FUNCTION:     HandleGoldMinerScoreViewWinMsg
//  Description :   查看分数
//  Global resource dependence : 
//  Author:       
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGoldMinerScoreViewWinMsg( 
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          );


/*****************************************************************************/
//  FUNCTION:      HandleGoldMinerSetLevelWinMsg
//  Description :  
//  Global resource dependence : 
//  Author:    
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGoldMinerSetLevelWinMsg(
                                              MMI_WIN_ID_T win_id,  
                                              MMI_MESSAGE_ID_E msg_id,  
                                              DPARAM param
                                              );

/*****************************************************************************/
//  FUNCTION:     HandleGoldMinerHelpWinMsg
//  Description :   
//  Global resource dependence : 
//  Author: 
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGoldMinerHelpWinMsg( 
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         );


/*****************************************************************************/
//                         Constant Variables                                *
/*****************************************************************************/

// 主菜单
WINDOW_TABLE( MMIGMGOLDMINER_MAIN_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleGMGoldMinerMainMenuWindow),    
    WIN_ID( GOLDMINER_MAIN_MENU_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_GOLDMINER, MMIGMGOLDMINER_MAINMENU_CTRL_ID),
    END_WIN
};

// 游戏界面
WINDOW_TABLE( GOLDMINER_NEW_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleGoldMinerNewWinMsg ),    
    WIN_ID( GOLDMINER_NEW_WIN_ID ),
    WIN_HIDE_STATUS,
    //WIN_SOFTKEY(TXT_NULL, TXT_GOLDMINER_PAUSE, STXT_RETURN),
    END_WIN
}; 

// 分数查看
WINDOW_TABLE( GOLDMINER_SCOREVIEW_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleGoldMinerScoreViewWinMsg ),    
    WIN_ID( GOLDMINER_SCOREVIEW_WIN_ID ),
    WIN_TITLE( TXT_GOLDMINER_HIGHRECORD ),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIGMGOLDMINER_RECORD_LEVEL_CTRL_ID),       
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIGMGOLDMINER_RECORD_TIME_CTRL_ID),       
    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
    END_WIN
}; 

// 游戏等级
WINDOW_TABLE( GOLDMINER_SETLEVEL_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleGoldMinerSetLevelWinMsg ),    
    WIN_ID( GOLDMINER_SETLEVEL_WIN_ID),
    WIN_TITLE(TXT_GOLDMINER_LEVEL),
    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIGMGOLDMINER_SETLEVEL_LABEL_CTRL_ID),
    CREATE_EDIT_DIGITAL_CTRL(3,MMIGMGOLDMINER_SETLEVEL_EDITBOX_CTRL_ID),
    //CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIGMGOLDMINER_SETLEVEL_CTRL_ID),
    END_WIN
}; 
// 游戏帮助
WINDOW_TABLE( GOLDMINER_INFO_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleGoldMinerHelpWinMsg ),    
    WIN_ID(GOLDMINER_INFO_WIN_ID),
    WIN_TITLE(TXT_GAME_INFO),
    CREATE_TEXT_CTRL(MMIGMGOLDMINER_HELP_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
}; 


/*****************************************************************************/
//                         Constant Variables                                *
/*****************************************************************************/

/*****************************************************************************/
//                       function bodies 
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  FUNCTION:     MMIGMGOLDMINER_OpenGameWin
//  Description :   
//  Global resource dependence : 
//  Author:      
/*****************************************************************************/
PUBLIC BOOLEAN MMIGMGOLDMINER_OpenGameWin(void)
{
    //进入游戏主菜单
   MMK_CreateWin((uint32*)MMIGMGOLDMINER_MAIN_MENU_WIN_TAB,PNULL);
   return TRUE;
}

/*****************************************************************************/
//  Description : HandleGMGoldMinerMainMenuWindow     
//  Global resource dependence : 
//  Author: 
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGMGoldMinerMainMenuWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param)
                                               
{
    uint32          group_id = 0; 
    uint32          menu_id  = 0;
    MMI_RESULT_E    recode   = MMI_RESULT_TRUE;

#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    if(MSG_KEYDOWN_UP <= msg_id && msg_id <= MSG_KEYDOWN_MENU)
    {
        uint8 keycode= (uint8)(msg_id & 0x000000ff); 
        keycode = MMIDEFAULT_ConvertQwertyKeyToTypicalKey(keycode);
        msg_id = (msg_id & 0xffffff00) | (keycode & 0x000000ff);
    }
#endif
 
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {
                MMK_SetAtvCtrl(win_id, MMIGMGOLDMINER_MAINMENU_CTRL_ID);                
            }
            break;
        
        case MSG_CTL_PENOK:
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
        case MSG_APP_OK:
            {
                GUIMENU_GetId(MMIGMGOLDMINER_MAINMENU_CTRL_ID,&group_id,&menu_id);
                switch(menu_id) 
                {
                    case ID_GMGOLDMINER_NEW:
                        {
                            //进入游戏
                            MMK_CreateWin((uint32 *)  GOLDMINER_NEW_WIN_TAB, PNULL);
                        }
                        break;
                    case ID_GMGOLDMINER_SCORE_VIEW:
                        {
                            //进入游戏最高分查看
                            MMK_CreateWin((uint32 *) GOLDMINER_SCOREVIEW_WIN_TAB, PNULL);
                        }
                        break;
                
                    case ID_GMGOLDMINER_LEVEL:
                        {
                            //进入游戏等级设置
                            MMK_CreateWin((uint32 *) GOLDMINER_SETLEVEL_WIN_TAB, PNULL);
                        }
                        break;
                        
                    case ID_GMGOLDMINER_INFO:
                        {
                            //进入帮助
                            MMK_CreateWin((uint32 *) GOLDMINER_INFO_WIN_TAB, PNULL);
                        }
                        break;

                    default:
                        {
                            //程序异常,开发调试用，目前关掉，不可能会出现此处
                            //SCI_ASSERT(0);
                        }
                        break;
                }
        
            }
            break;

        case MSG_KEYDOWN_1:    //MSG_KEYDOWN_R:
           MMK_CreateWin((uint32 *) GOLDMINER_NEW_WIN_TAB, PNULL);
           break;
        case MSG_KEYDOWN_2:    //MSG_KEYDOWN_T:
           MMK_CreateWin((uint32 *) GOLDMINER_SCOREVIEW_WIN_TAB, PNULL);
           break;
        case MSG_KEYDOWN_3:    //MSG_KEYDOWN_Y:	         
           MMK_CreateWin((uint32 *) GOLDMINER_SETLEVEL_WIN_TAB, PNULL);
           break;
	      case MSG_KEYDOWN_4:    //MSG_KEYDOWN_F:	         
           MMK_CreateWin((uint32 *) GOLDMINER_INFO_WIN_TAB, PNULL);
           break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            {
                MMK_CloseWin(win_id);
            }
            break;
            
        default:
            {
                recode = MMI_RESULT_FALSE;
            }
            break;
    }
    
    return recode;
}

/*****************************************************************************/
//  FUNCTION:     HandleGoldMinerNewWinMsg
//  Description :   handle new  game win msg
//  Global resource dependence : 
//  Author: 
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGoldMinerNewWinMsg( 
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {
#ifndef MMI_GUI_STYLE_MINISCREEN                
                MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_90);
                MMK_SetWinAngle(win_id, LCD_ANGLE_90, FALSE); 
#endif                
                MMIGOLDMINER_StartGameC();
            }
            break;
        
        case MSG_TIMER:
            { 
                if(MMIGM_GOLDMINER_TIMER_COUNTED != MMIGOLDMINER_GetTimerTypeM(*(uint8*)param))
                {
                    //deal with event by entity state machine
                    MMI_ENTITY_EVENT_E cur_event = EVENT_NONE;

                    cur_event = MMIGOLDMINER_GetEventM();
                    MMIGOLDMINER_ClipperStateMachineC(cur_event);
                    MMIGOLDMINER_GameStateMachineC(GAME_EVENT_NONE);
                    MMIGOLDMINER_GameDrawV(win_id);
                }
                else   //游戏时间处理
                {
                    if(0 == MMIGOLDMINER_GetCountDownTimeM())
                    { 
                        MMIGOLDMINER_GameStateMachineC(GAME_EVENT_TIME_OUT); 
                    }
                    else
                    {
                        MMIGOLDMINER_UpdateCountDownTimeM();
                        MMIGOLDMINER_GameStateMachineC(GAME_EVENT_NONE);
                        MMIGOLDMINER_GameDrawV(); 
                    }                        
                 }
             }
            break;
                
        case MSG_FULL_PAINT:
            {
                MMIGOLDMINER_GameDrawV(); 
            }
            break;
    
        case MSG_APP_WEB:
        case MSG_APP_DOWN:
			{
				if(MMI_GAME_ENTITY_READY == MMIGOLDMINER_GetClipperStateM())
                {
                    MMIGOLDMINER_ClipperStateMachineC(EVENT_START);
                    MMIGOLDMINER_GameDrawV(); 
                }
                
			}
			break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_TP_PRESS_UP:
            {
                if(MMI_GAME_ENTITY_READY == MMIGOLDMINER_GetClipperStateM())
                {
                    MMIGOLDMINER_ClipperStateMachineC(EVENT_START);
                    MMIGOLDMINER_GameDrawV(); 
                }
                
                if(MMI_GAME_ENTITY_HIT == MMIGOLDMINER_GetClipperStateM())
                {
                    int tool_x = 0;
                    tool_x = MMK_GET_TP_X(param);
                    
                    if(!MMIGOLDMINER_GetToolStateC()) //同时只能有一个道具在起作用
                    {
                        MMIGOLDMINER_SetToolStateC(TRUE);
                     
                        if( tool_x < MMI_GM_GOLDMINER_SHOOTER_HORIZONTAL_X) //以shooter的初始x坐标值为touch 下道具使用的范围
                        {
                            if(MMIGOLDMINER_GetBombNumM() > 0)
                            {
                                MMIGOLDMINER_UpdateBombNumM();
                                MMIGOLDMINER_SetMoveCellNoneM();
                                MMIGOLDMINER_SetClipperStateM(MMI_GAME_ENTITY_NORMAL);
                                MMIGOLDMINER_ClipperStateMachineC(EVENT_NONE);
                                MMIGOLDMINER_GameDrawV();
                            }                        
                            
                             MMIGOLDMINER_SetToolStateC(FALSE);
                        }                        
                        else
                        {
                            if(MMIGOLDMINER_GetEnergyNumM() > 0)
                            {                   
                                MMIGOLDMINER_SetClippersMoveWithEnergyM(TRUE);
                                MMIGOLDMINER_UpdateEnergyNumM();
                            }
                        }
                    }
                }                 
            }
            break; 
#endif			
//非touch 版本，左右键为道具使用键
#ifndef MMI_PDA_SUPPORT    
    case MSG_APP_LEFT://bomb
            {
                if(MMI_GAME_ENTITY_HIT == MMIGOLDMINER_GetClipperStateM())
                {
                    if(!MMIGOLDMINER_GetToolStateC()) //同时只能有一个道具在起作用
                    {
                        MMIGOLDMINER_SetToolStateC(TRUE);

                        if(MMIGOLDMINER_GetBombNumM() > 0)
                        {
                            MMIGOLDMINER_UpdateBombNumM();
                            MMIGOLDMINER_SetMoveCellNoneM();
                            MMIGOLDMINER_SetClipperStateM(MMI_GAME_ENTITY_NORMAL);
                            MMIGOLDMINER_ClipperStateMachineC(EVENT_NONE);
                            MMIGOLDMINER_GameDrawV();
                        }                        
                            
                         MMIGOLDMINER_SetToolStateC(FALSE);                             
                    }
                }
            }
        break;
        
    case MSG_APP_RIGHT://energy
            {
                if(MMI_GAME_ENTITY_HIT == MMIGOLDMINER_GetClipperStateM())
                {
                    if(!MMIGOLDMINER_GetToolStateC()) //同时只能有一个道具在起作用
                    {
                        MMIGOLDMINER_SetToolStateC(TRUE);

                        if(MMIGOLDMINER_GetEnergyNumM() > 0)
                        {                   
                            MMIGOLDMINER_SetClippersMoveWithEnergyM(TRUE);
                            MMIGOLDMINER_UpdateEnergyNumM();
                        }                      
                    }
                }
            }
        break;
#endif
        
        case MSG_APP_UP:
            {
                //可以增加及时上提的功能
                //MMIGOLDMINER_GameDrawPromptAreaV();
            }
            break;                            
        
        case MSG_LOSE_FOCUS: 
            {
                // 背光被允许关闭
                MMIDEFAULT_AllowTurnOffBackLight(TRUE); 
                //游戏窗口被关闭，则暂停游戏
                if(MMI_GAME_PLAY == MMIGOLDMINER_GetGameStateM())
                {
                    MMIGOLDMINER_GameStateMachineC(GAME_EVENT_LOSE_FOCUS);
                }                
            }
            break;

        case MSG_CLOSE_WINDOW:
            {
                // 背光被允许关闭
                 MMIDEFAULT_AllowTurnOffBackLight(TRUE); 
                //free 掉所有指针
                 MMIGOLDMINER_ExitGameC();
            }
            break;

        case MSG_GET_FOCUS:
            {
                // 背光常亮
                if (MMI_GAME_PAUSE == MMIGOLDMINER_GetGameStateM())
                {
                    MMIGOLDMINER_GameStateMachineC(GAME_EVENT_GET_FOCUS);
                    MMIDEFAULT_AllowTurnOffBackLight(FALSE);
                }

                if(MMI_GAME_LEVEL_DONE == MMIGOLDMINER_GetGameStateM() ||
                   MMI_GAME_OVER == MMIGOLDMINER_GetGameStateM() ||
                   MMI_GAME_FINISH == MMIGOLDMINER_GetGameStateM())
                {
                    MMIGOLDMINER_GameStateMachineC(GAME_EVENT_NONE);
                }
            }
            break;
 
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            { 
                GoldMiner_SaveGameScore();
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_PROMPTWIN_OK:
            {
                // 提示框，按左键重新开始游戏 
                GoldMiner_SaveGameScore();
                GoldMiner_InitGame(); 
                // 背光常亮
                MMIDEFAULT_AllowTurnOffBackLight(FALSE);
                MMIPUB_CloseQuerytWin(PNULL);
            }
            break;
        
        case MSG_PROMPTWIN_CANCEL:
            {
                // 按右键退出游戏 
                GoldMiner_SaveGameScore();
                MMIPUB_CloseQuerytWin(PNULL);
                MMK_CloseWin(win_id);
            }
            break;
        
        default:
            {
                recode = MMI_RESULT_FALSE;
            }
            break;
    }
    
    return recode;
}

/*****************************************************************************/
//  FUNCTION:     GoldMiner_ViewScore
//  Description :   显示分数
//  Global resource dependence : 
//  Author:
//  Note:   
/*****************************************************************************/
LOCAL void GoldMiner_ViewScore(
                               MMI_WIN_ID_T win_id,
                               //MMI_CTRL_ID_T ctrl_id, 
                               //MMI_TEXT_ID_T  title_id,
                               MMI_GAME_RECORD_T record)
{
    uint8 asc_str_level[12] = {0};
    uint8 asc_str_time[12] = {0};
    wchar wstr_level[12] = {0};
    wchar wstr_time[12] = {0};
    MMI_STRING_T  str_level = {0};    
    MMI_STRING_T  str_time = {0}; 
    MMI_STRING_T  result_level = {0};
    MMI_STRING_T  result_time = {0};
    MMI_STRING_T  temp_str_level = {0};
    MMI_STRING_T  temp_str_time = {0};
    
    GUI_RECT_T level_rect = MMI_GOLDMINER_VIEW_LEVLE_RECT;
    GUI_RECT_T time_rect = MMI_GOLDMINER_VIEW_TIME_RECT;

    MMI_GetLabelTextByLang(TXT_GOLDMINER_HIGHLEVEL, &temp_str_level);
    MMI_GetLabelTextByLang(TXT_GOLDMINER_USETIME, &temp_str_time);
    
    sprintf((char*)asc_str_level,":%d",record.high_level);
    str_level.wstr_len = strlen((char*)asc_str_level);     
    str_level.wstr_ptr = wstr_level;
    MMI_STRNTOWSTR( wstr_level, 12, asc_str_level, 12, str_level.wstr_len );
    MMIAPICOM_CatTwoString(&result_level, &temp_str_level, &str_level);
    if (PNULL == result_level.wstr_ptr || 0 == result_level.wstr_len)
    {
        return;
    }
    GUILABEL_SetRect(MMIGMGOLDMINER_RECORD_LEVEL_CTRL_ID,&level_rect,TRUE);
#ifdef MAINLCD_SIZE_128X160        
    GUILABEL_SetFont(MMIGMGOLDMINER_RECORD_LEVEL_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_BLACK_COLOR);
#else
    GUILABEL_SetFont(MMIGMGOLDMINER_RECORD_LEVEL_CTRL_ID, MMI_DEFAULT_BIG_FONT, MMI_BLACK_COLOR);
#endif
    GUILABEL_SetText(MMIGMGOLDMINER_RECORD_LEVEL_CTRL_ID, &result_level, FALSE);

    sprintf((char*)asc_str_time,":%d",record.time);
    str_time.wstr_len = strlen((char*)asc_str_time);     
    str_time.wstr_ptr = wstr_time;
    MMI_STRNTOWSTR( wstr_time, 12, asc_str_time, 12, str_time.wstr_len );
    MMIAPICOM_CatTwoString(&result_time, &temp_str_time, &str_time);
    if (PNULL == result_time.wstr_ptr || 0 == result_time.wstr_len)
    {
        return;
    }
    GUILABEL_SetRect(MMIGMGOLDMINER_RECORD_TIME_CTRL_ID,&time_rect,TRUE);
#ifdef MAINLCD_SIZE_128X160    
    GUILABEL_SetFont(MMIGMGOLDMINER_RECORD_TIME_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_BLACK_COLOR);
#else
    GUILABEL_SetFont(MMIGMGOLDMINER_RECORD_TIME_CTRL_ID, MMI_DEFAULT_BIG_FONT, MMI_BLACK_COLOR);
#endif
    GUILABEL_SetText(MMIGMGOLDMINER_RECORD_TIME_CTRL_ID, &result_time, FALSE);
    
    //SCI_FREE(result_level.wstr_ptr);
    //SCI_FREE(result_time.wstr_ptr);

}

/*****************************************************************************/
//  FUNCTION:     HandleGoldMinerScoreViewWinMsg
//  Description :   查看分数
//  Global resource dependence : 
//  Author:  
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGoldMinerScoreViewWinMsg( 
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          )
{
    MMI_RESULT_E        recode          = MMI_RESULT_TRUE;
    //static uint16       high_score      = 0;  //注意排除static  
    static MMI_GAME_RECORD_T record = {0};
    uint16              nv_return       = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
     
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {               
                // 读nv
                MMINV_READ(MMINV_GMGOLDMINER_HIGH_SCORE, &record, nv_return);
                if (MN_RETURN_SUCCESS != nv_return)
                {
                    record.high_level = 0;
                    record.time = 0;
                    MMINV_WRITE(MMINV_GMGOLDMINER_HIGH_SCORE,&record);
                }
                // 设置softkey
                GUIWIN_SetSoftkeyTextId(win_id,  TXT_GOLDMINER_RESET,TXT_NULL, STXT_RETURN,FALSE);
            }
            break;
        
        case MSG_FULL_PAINT:
            {
                // 画背景
                GUI_POINT_T startpoint = {0};
                startpoint.y = MMI_TITLE_HEIGHT;

                GUIRES_DisplayImg(&startpoint,
                    PNULL,
                    PNULL,
                    win_id,
                    IMAGE_COMMON_BG,
                    &lcd_dev_info);
                // 画分数
                GoldMiner_ViewScore(win_id, record);
            }
            break;
        
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            {
                // 提示分数重置
                MMIPUB_OpenQueryWinByTextId(TXT_GOLDMINER_RESET,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            }
            break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            {
                MMK_CloseWin(win_id);
            }
            break;
        
        case MSG_PROMPTWIN_OK:
            {
                // 分数重置为0，并显示
                MMIPUB_CloseQuerytWin(PNULL);
                record.high_level= 0;
                record.time = 0;
                MMINV_WRITE(MMINV_GMGOLDMINER_HIGH_SCORE, (int16*)&record);
                GoldMiner_ViewScore(win_id, record);
            }
            break;
        
        case MSG_PROMPTWIN_CANCEL:
            {
                MMIPUB_CloseQuerytWin(PNULL);
            }
            break;
        
        default:
            {
                recode = MMI_RESULT_FALSE;
            }
            break;
    }
    
    return recode;
}

/*****************************************************************************/
//  FUNCTION:      ConfirmGoldMinerSetLevelWinMsg
//  Description :    设定难度
//  Global resource dependence : 
//  Author:        peilong.zhou 
//  Note:   
/*****************************************************************************/
LOCAL void  ConfirmGoldMinerSetLevelWinMsg(void)
{
    int8    level[2]  = {0};    
    uint16  sel       = 0;
    uint16  nv_return = 0;   
    MMI_GAME_RECORD_T record = {0};

    MMI_STRING_T edit_box_str = {0};
    uint32 edit_num_value = 0;
    
    MMINV_READ(MMINV_GMGOLDMINER_HIGH_SCORE, &record, nv_return);

    GUIEDIT_GetString(MMIGMGOLDMINER_SETLEVEL_EDITBOX_CTRL_ID,&edit_box_str);

    MMIAPICOM_WstrToNum(edit_box_str.wstr_ptr,edit_box_str.wstr_len,&edit_num_value);

    if(record.high_level)
    {
        if(edit_num_value > record.high_level)
        {
            GUIEDIT_ClearAllStr(MMIGMGOLDMINER_SETLEVEL_EDITBOX_CTRL_ID);
            MMIPUB_OpenAlertWarningWin(TXT_GOLDMINER_SETLEVEL_FAIL);
        }
        else
        {
            MMIGOLDMINER_SetGameInfoByLevelM(edit_num_value);
            MMK_CreateWin((uint32 *)  GOLDMINER_NEW_WIN_TAB, PNULL);
            MMK_CloseWin(GOLDMINER_SETLEVEL_WIN_ID);
        }
    }
    else
    {
        if(1 == edit_num_value)
        {
            //MMIGOLDMINER_SetGameInfoByLevelM(edit_num_value);
            MMK_CreateWin((uint32 *)  GOLDMINER_NEW_WIN_TAB, PNULL);
            MMK_CloseWin(GOLDMINER_SETLEVEL_WIN_ID);
        }
        else
        {
            GUIEDIT_ClearAllStr(MMIGMGOLDMINER_SETLEVEL_EDITBOX_CTRL_ID);
            MMIPUB_OpenAlertWarningWin(TXT_GOLDMINER_SETLEVEL_FAIL);
        }
    }
    
    
}

/*****************************************************************************/
//  FUNCTION:      CreateSetLevelView
//  Description :    Create set level view info
//  Global resource dependence : 
//  Author:        Michael.Wang
//  Note:   
/*****************************************************************************/
LOCAL void CreateSetLevelView(void)
{
    uint16  nv_return = 0;   
    uint8 asc_str_num1[12] = {0};
    uint8 asc_str_num2[12] = {0};
    wchar wstr_num1[12] = {0};
    wchar wstr_num2[12] = {0};
    MMI_STRING_T  str_num1 = {0};    
    MMI_STRING_T  str_num2 = {0}; 
    MMI_STRING_T  result_num = {0};
    MMI_STRING_T  temp_str = {0};
    MMI_GAME_RECORD_T record = {0};
    GUI_BORDER_T        edit_border = {0}; 
    GUI_RECT_T lable_rect = MMI_GOLDMINER_SETLEVEL_LABLE_RECT;
    GUI_RECT_T edit_rect = MMI_GOLDMINER_SETLEVEL_EDIT_RECT;
    
    MMINV_READ(MMINV_GMGOLDMINER_HIGH_SCORE, &record, nv_return);
    
    sprintf((char*)asc_str_num1,"%d",1);       
    str_num1.wstr_len = strlen((char*)asc_str_num1);     
    str_num1.wstr_ptr = wstr_num1;
    MMI_STRNTOWSTR( wstr_num1, 12, asc_str_num1, 12, str_num1.wstr_len );
    result_num.wstr_len = str_num1.wstr_len;
    result_num.wstr_ptr = str_num1.wstr_ptr;
    
    if(record.high_level > 1)
    {
        sprintf((char*)asc_str_num2," ~ %d",record.high_level);
     
        str_num2.wstr_len = strlen((char*)asc_str_num2);     
        str_num2.wstr_ptr = wstr_num2;
        MMI_STRNTOWSTR( wstr_num2, 12, asc_str_num2, 12, str_num2.wstr_len );

        MMIAPICOM_CatTwoString(&result_num, &str_num1, &str_num2);
        if (PNULL == result_num.wstr_ptr || 0 == result_num.wstr_len)
        {
            return;
        }
    
    }
    
    MMI_GetLabelTextByLang(TXT_GOLDMINER_SET_LEVEL, &temp_str);

    GUILABEL_SetRect(MMIGMGOLDMINER_SETLEVEL_LABEL_CTRL_ID,&lable_rect,TRUE);
#ifdef MAINLCD_SIZE_128X160       
    GUILABEL_SetFont(MMIGMGOLDMINER_SETLEVEL_LABEL_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_BLACK_COLOR);
#else
    GUILABEL_SetFont(MMIGMGOLDMINER_SETLEVEL_LABEL_CTRL_ID, MMI_DEFAULT_BIG_FONT, MMI_BLACK_COLOR);
#endif
    GUILABEL_SetText(MMIGMGOLDMINER_SETLEVEL_LABEL_CTRL_ID, &temp_str, FALSE);

    edit_border.type  = GUI_BORDER_ROUNDED;
    edit_border.width = 2;
    edit_border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
    
    GUIEDIT_SetRect(MMIGMGOLDMINER_SETLEVEL_EDITBOX_CTRL_ID,&edit_rect);
    GUIEDIT_SetBorder(MMIGMGOLDMINER_SETLEVEL_EDITBOX_CTRL_ID,&edit_border);
#ifdef MAINLCD_SIZE_128X160    
    GUIEDIT_SetFont(MMIGMGOLDMINER_SETLEVEL_EDITBOX_CTRL_ID,MMI_DEFAULT_TEXT_FONT);
#else
    GUIEDIT_SetFont(MMIGMGOLDMINER_SETLEVEL_EDITBOX_CTRL_ID,MMI_DEFAULT_BIG_FONT);
#endif
    GUIEDIT_SetAlign(MMIGMGOLDMINER_SETLEVEL_EDITBOX_CTRL_ID,ALIGN_LVMIDDLE);
    GUIEDIT_SetDefaultString(MMIGMGOLDMINER_SETLEVEL_EDITBOX_CTRL_ID, result_num.wstr_ptr, result_num.wstr_len);
}




/*****************************************************************************/
//  FUNCTION:      HandleGoldMinerSetLevelWinMsg
//  Description :  
//  Global resource dependence : 
//  Author:        peilong.zhou 
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGoldMinerSetLevelWinMsg(
                                              MMI_WIN_ID_T win_id,  
                                              MMI_MESSAGE_ID_E msg_id,  
                                              DPARAM param
                                              )
{
    MMI_RESULT_E  recode    = MMI_RESULT_TRUE;
    uint32 i =0;
    GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {
                CreateSetLevelView();
                GUIWIN_SetSoftkeyTextId(win_id, STXT_SOFTKEY_OK_MK,COMMON_TXT_NULL,STXT_RETURN,FALSE);
                MMK_SetAtvCtrl( win_id,  MMIGMGOLDMINER_SETLEVEL_EDITBOX_CTRL_ID);
            }
            break;

         case MSG_FULL_PAINT:
            {
                // 画背景
                GUI_POINT_T startpoint = {0};
                startpoint.y = MMI_TITLE_HEIGHT;

                GUIRES_DisplayImg(&startpoint,
                    PNULL,
                    PNULL,
                    win_id,
                    IMAGE_COMMON_BG,
                    &lcd_dev_info);                
            }
            break;
        
        case MSG_CTL_PENOK:
        case MSG_CTL_OK: 
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_KEYUP_WEB:
            {
                ConfirmGoldMinerSetLevelWinMsg();
            }
            break;
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            {
                MMK_CloseWin(win_id);
            }
            break;
        
        default:
            {
                recode = MMI_RESULT_FALSE;
            }
            break;
    }
    
    return recode;

}

/*****************************************************************************/
//  FUNCTION:     HandleGoldMinerHelpWinMsg
//  Description :   handle help win msg
//  Global resource dependence : 
//  Author:
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGoldMinerHelpWinMsg( 
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {
                GUI_COLOR_T  text_color = MMI_BLACK_COLOR;
                MMI_STRING_T string = {0};     
    
                MMK_SetAtvCtrl(win_id, MMIGMGOLDMINER_HELP_CTRL_ID);  
                MMI_GetLabelTextByLang(TXT_GOLDMINER_GAME_INFO, &string);
                GUITEXT_SetString(MMIGMGOLDMINER_HELP_CTRL_ID , string.wstr_ptr, string.wstr_len, FALSE);
                GUITEXT_SetFont(MMIGMGOLDMINER_HELP_CTRL_ID, PNULL, &text_color);                
            }
            break;
        
        case MSG_FULL_PAINT:
            break;
    
        case MSG_CTL_OK:
        case MSG_APP_OK:
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

