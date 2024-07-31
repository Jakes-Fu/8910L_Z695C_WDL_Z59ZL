/****************************************************************************
** File Name:      mmigame_snake_wintab.c                            
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
#define _MMIGAME_SNAKE_WINTAB_C_


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_SNAKE_SUPPORT
#include "window_parse.h"
#include "mmk_app.h"
#include "guimsgbox.h"
#include "guilcd.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "guitext.h"
#include "mmidisplay_data.h"
#include "mmigame_snake_menutable.h"
#include "mmigame_menutable.h"
#include "mmigame_snake_id.h"  
#include "tb_dal.h"
#include "mmigame_snake_nv.h"
#include "mmi_textfun.h"
#include "mmigame_snake_internal.h"
#include "mmigame_snake_text.h"
#include "mmigame_snake_image.h"
#include "mmigame_id.h"
#include "window_parse.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_theme.h"
#include "mmi_nv.h"
#include "mmipub.h"
#include "mmk_timer.h"
#include "mmi_default.h"
#include "guires.h"  
#include "mmiset_export.h" 

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
// 暂时只在240X320平台上，不同分辨率这个值需要调，暂时让其编译报错
#if defined MAINLCD_SIZE_128X160
#define SNAKE_SCORE_VIEW_HEIGHT  26      
#elif defined MAINLCD_SIZE_176X220
#define SNAKE_SCORE_VIEW_HEIGHT  26      
#elif defined MAINLCD_SIZE_220X176
#define SNAKE_SCORE_VIEW_HEIGHT  26      
#elif defined MAINLCD_SIZE_240X320
#define SNAKE_SCORE_VIEW_HEIGHT  26      
#elif defined MAINLCD_SIZE_320X240
#define SNAKE_SCORE_VIEW_HEIGHT  26      
#elif defined MAINLCD_SIZE_240X400
#define SNAKE_SCORE_VIEW_HEIGHT  26      
#elif defined MAINLCD_SIZE_320X480
#define SNAKE_SCORE_VIEW_HEIGHT  26      
#else
#error
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA  DECLARE                           */
/*---------------------------------------------------------------------------*/
//extern  MMI_TEXT_ID_T          s_snake_level_list[SNAKE_TOTAL_LEVEL]; /* !e18*/
extern  uint8                   s_snake_timer;    
extern  SNAKE_LEVEL_E           s_snake_current_level;
extern  MMI_MESSAGE_ID_E     s_snake_direction;  
extern  MMI_MESSAGE_ID_E     s_snake_direction_cache; 
extern  SNAKE_GAME_STATE_E s_snake_game_state;


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/    

//*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  FUNCTION:     HandleSnakeNewWinMsg
//  Description :   
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSnakeNewWinMsg( 
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        );

/*****************************************************************************/
//  Description : HandleGMSnakeMainMenuWindow
//       
//  Global resource dependence : 
//   Author:        peilong.zhou
//   RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGMSnakeMainMenuWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               );

/*****************************************************************************/
//  FUNCTION:     HandleSnakeScoreViewWinMsg
//  Description :   查看分数
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSnakeScoreViewWinMsg( 
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          );


/*****************************************************************************/
//  FUNCTION:      HandleSnakeSetLevelWinMsg
//  Description :  
//  Global resource dependence : 
//  Author:        peilong.zhou 
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSnakeSetLevelWinMsg(
                                              MMI_WIN_ID_T win_id,  
                                              MMI_MESSAGE_ID_E msg_id,  
                                              DPARAM param
                                              );

/*****************************************************************************/
//  FUNCTION:     HandleSnakeHelpWinMsg
//  Description :   
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSnakeHelpWinMsg( 
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         );

/*****************************************************************************/
//  FUNCTION:      CancelSnakeSetLevelWinMsg
//  Description :  
//  Global resource dependence : 
//  Author:        peilong.zhou 
//  Note:   
/*****************************************************************************/
LOCAL void  CancelSnakeSetLevelWinMsg(void);

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/

// 创建贪吃蛇主菜单
WINDOW_TABLE( MMIGMSNAKE_MAIN_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleGMSnakeMainMenuWindow),    
    WIN_ID( MMIGMSNAKE_MAIN_MENU_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_SNAKE, MMIGMSNAKE_MAINMENU_CTRL_ID),
    END_WIN
};

// 贪吃蛇游戏界面
WINDOW_TABLE( SNAKE_NEW_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32) HandleSnakeNewWinMsg ),    
    WIN_ID( SNAKE_NEW_WIN_ID ),
    // WIN_TITLE( TXT_SNAKE_HIGHSCORE ),
    WIN_SOFTKEY(TXT_NULL, TXT_SNAKE_PAUSE, STXT_RETURN),
    END_WIN
}; 

// 贪吃蛇最高分查看
WINDOW_TABLE( SNAKE_SCOREVIEW_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleSnakeScoreViewWinMsg ),    
    WIN_ID( SNAKE_SCOREVIEW_WIN_ID ),
    WIN_TITLE( TXT_SNAKE_HIGHSCORE ),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIGMSNAKE_SCORE_CTRL_ID),
        
    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
    END_WIN
}; 

// 游戏等级，越难，蛇的运行速度越快
WINDOW_TABLE( SNAKE_SETLEVEL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleSnakeSetLevelWinMsg ),    
    WIN_ID( SNAKE_SETLEVEL_WIN_ID),
    WIN_TITLE(TXT_SNAKE_LEVEL),
    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIGMSNAKE_SETLEVEL_CTRL_ID),
    END_WIN
}; 
// 游戏帮助
WINDOW_TABLE( SNAKE_INFO_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleSnakeHelpWinMsg ),    
    WIN_ID(SNAKE_INFO_WIN_ID),
    WIN_TITLE(TXT_GAME_INFO),
    CREATE_TEXT_CTRL(MMIGMSNAKE_HELP_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    //WIN_STYLE(WS_HAS_SHORTCUTBAR),
    END_WIN
}; 


/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/



/**---------------------------------------------------------------------------*
**                       function bodies 
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  FUNCTION:     MMIGMSNAKE_OpenGameWin
//  Description :   
//  Global resource dependence : 
//  Author:      
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIGMSNAKE_OpenGameWin(void)
{
    //进入游戏主菜单
   MMK_CreateWin((uint32*)MMIGMSNAKE_MAIN_MENU_WIN_TAB,PNULL);
   return TRUE;
}

/*****************************************************************************/
//  FUNCTION:     MMIGMSNAKE_RsetGameDate
//  Description :   
//  Global resource dependence : 
//  Author:      
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMSNAKE_RsetGameData(void)
{
  uint16 score = 0;
  int8   level[2]  = {0};
  level[1] = 0;
  MMINV_WRITE(MMINV_GMSNAKE_HIGH_SCORE,&score);
  MMINV_WRITE(MMINV_GMSNAKE_LEVER_SET, level);
}

/*****************************************************************************/
//  Description : HandleGMSnakeMainMenuWindow     
//  Global resource dependence : 
//  Author:  peilong.zhou
//  RETRUN:   
//  Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleGMSnakeMainMenuWindow(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param)
                                               
{
    uint32          group_id = 0; 
    uint32          menu_id  = 0;
    MMI_RESULT_E    recode   = MMI_RESULT_TRUE;

#if defined KEYPAD_TYPE_QWERTY_KEYPAD
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
                MMK_SetAtvCtrl(win_id, MMIGMSNAKE_MAINMENU_CTRL_ID);
            }
            break;
        
        case MSG_CTL_PENOK:
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
        case MSG_APP_OK:
            {
                GUIMENU_GetId(MMIGMSNAKE_MAINMENU_CTRL_ID,&group_id,&menu_id);
                switch(menu_id) 
                {
                    case ID_GMSNAKE_NEW:
                        {
                            //进入游戏
                            if(Snake_AllocGameSpace())
                            {
                                MMK_CreateWin((uint32 *)SNAKE_NEW_WIN_TAB, PNULL);
                            }
                            else
                            {
                                MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_MEMORY);
                            }
                        }
                        break;
                    case ID_GMSNAKE_SCORE_VIEW:
                        {
                            //进入游戏最高分查看
                            MMK_CreateWin((uint32 *) SNAKE_SCOREVIEW_WIN_TAB, PNULL);
                        }
                        break;
                
                    case ID_GMSNAKE_LEVEL:
                        {
                            //进入游戏等级设置
                            MMK_CreateWin((uint32 *) SNAKE_SETLEVEL_WIN_TAB, PNULL);
                        }
                        break;
                        
                    case ID_GMSNAKE_INFO:
                        {
                            //进入帮助
                            MMK_CreateWin((uint32 *) SNAKE_INFO_WIN_TAB, PNULL);
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

        case MSG_KEYDOWN_1:
	     //case MSG_KEYDOWN_R:
			MMK_CreateWin((uint32 *) SNAKE_NEW_WIN_TAB, PNULL);
			break;
    	case MSG_KEYDOWN_2:
        //case MSG_KEYDOWN_T:
			MMK_CreateWin((uint32 *) SNAKE_SCOREVIEW_WIN_TAB, PNULL);
			break;
    	case MSG_KEYDOWN_3:
    	//case MSG_KEYDOWN_Y:	         
			MMK_CreateWin((uint32 *) SNAKE_SETLEVEL_WIN_TAB, PNULL);
			break;
	    case MSG_KEYDOWN_4:
	    //case MSG_KEYDOWN_F:	         
			MMK_CreateWin((uint32 *) SNAKE_INFO_WIN_TAB, PNULL);
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
//  FUNCTION:     HandleSnakeNewWinMsg
//  Description :   handle new win msg
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSnakeNewWinMsg( 
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {
#ifdef MAINLCD_LOGIC_ANGLE_90
                MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_90);
                MMK_SetWinAngle(win_id, LCD_ANGLE_90, TRUE);
#else
                MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
                MMK_SetWinAngle(win_id, LCD_ANGLE_0, TRUE);
#endif
                //打开窗口先进行游戏初始化，并开始游戏
				MMIGMSNAKE_Init();
                Snake_InitGame();
                s_snake_game_state = SNAKE_GAME_ON;
                // 背光常亮
                MMIDEFAULT_AllowTurnOffBackLight(FALSE);
                //set softkey transparent
                GUIWIN_SetSoftkeyVisible(win_id, FALSE);
            }
            break;
        case MSG_LCD_SWITCH:
            break;
        
        case MSG_TIMER:
            {
                //蛇每走一步的时间都是通过这个实现的
                if (s_snake_timer == *(uint8*)param)
                {
                    //蛇走
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
                    //Snake_GameGoing(win_id);
                }     
                else
                {
                    recode = MMI_RESULT_FALSE;
                }
            }
            break;
                
        case MSG_FULL_PAINT:
            {
                //画游戏界面
                Snake_GameGoing(win_id);
            }
            break;
            
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_CTL_MIDSK:        
        case MSG_APP_WEB:
            {
                // 游戏中按ok键暂停游戏并提示
                if (SNAKE_GAME_ON == s_snake_game_state)
                {
                    // 背光被允许关闭
                    MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                    s_snake_game_state = SNAKE_GAME_PAUSE;
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
                    //Snake_GameGoing(win_id);
                }
                // 暂停中按ok键
                else if (SNAKE_GAME_PAUSE == s_snake_game_state)
                {
                    // 背光不被允许关闭
                    MMIDEFAULT_AllowTurnOffBackLight(FALSE);
                    s_snake_game_state = SNAKE_GAME_ON;
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
                    //Snake_GameGoing(win_id);
                }
            }
            break;
        case MSG_APP_LEFT:
            {
                // 按左键，当前是左或者右的时候不响应
                 if ((SNAKE_GAME_ON == s_snake_game_state)
                     &&(MSG_APP_RIGHT != s_snake_direction)
                     &&(MSG_APP_LEFT != s_snake_direction))
                {
                    s_snake_direction_cache = msg_id;
                    //Snake_GameGoing(win_id);
                }
            }
            break;                   
        
        case MSG_APP_RIGHT:
            {
                // 按右键，当前是左或者右的时候不响应
                 if ((SNAKE_GAME_ON == s_snake_game_state)
                     &&(MSG_APP_RIGHT != s_snake_direction)
                     &&(MSG_APP_LEFT != s_snake_direction))
                {
                    s_snake_direction_cache = msg_id;
                    //Snake_GameGoing(win_id);
                }
            }
            break;                   
        
        case MSG_APP_UP:
            {
                // 按上键，当前是上或者下的时候不响应
                if ((SNAKE_GAME_ON == s_snake_game_state)
                    &&(MSG_APP_UP != s_snake_direction)
                    &&(MSG_APP_DOWN != s_snake_direction))
                {
                    s_snake_direction_cache = msg_id;
                    //Snake_GameGoing(win_id);
                }
            }
            break;                   
        
        case MSG_APP_DOWN:
            {
                // 按下键，当前是上或者下的时候不响应
                if ((SNAKE_GAME_ON == s_snake_game_state)
                    &&(MSG_APP_DOWN != s_snake_direction)
                    &&(MSG_APP_UP != s_snake_direction))
                {
                    s_snake_direction_cache = msg_id;
                   // Snake_GameGoing(win_id);
                }
            }
            break;  

        case MSG_LOSE_FOCUS: 
            {
                // 背光被允许关闭
                MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                if (s_snake_timer >0 )
                {
                    MMK_StopTimer(s_snake_timer);
                    s_snake_timer = 0;
                }
                //游戏窗口被关闭，则暂停游戏
                if (SNAKE_GAME_ON == s_snake_game_state) 
                {
                    s_snake_game_state = SNAKE_GAME_PAUSE;
                }
            }
            break;

        case MSG_CLOSE_WINDOW:
            {
                Snake_ExitGame();
                // 背光被允许关闭
                MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                if (s_snake_timer >0 )
                {
                    MMK_StopTimer(s_snake_timer);
                    s_snake_timer = 0;
                }
            }
            break;

        case MSG_GET_FOCUS:
            {
                // 背光常亮
                if (SNAKE_GAME_ON == s_snake_game_state)
                {
                    MMIDEFAULT_AllowTurnOffBackLight(FALSE);
                }
                //Snake_GameGoing(win_id);
            }
            break;
		case MSG_APP_RED: 
            {
                // power键返回idle，记录最高分
                if (s_snake_timer >0 )
                {
                    MMK_StopTimer(s_snake_timer);
                    s_snake_timer = 0;
                }      
                Snake_SaveGameScore();
                MMK_ReturnIdleWin();
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            {
                // 右键退出游戏，记录最高分
                if (s_snake_timer >0 )
                {
                    MMK_StopTimer(s_snake_timer);
                    s_snake_timer = 0;
                }      
                Snake_SaveGameScore();
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_PROMPTWIN_OK:
            {
                // 提示框，按左键重新开始游戏
                s_snake_game_state = SNAKE_GAME_ON;
                Snake_SaveGameScore();
                Snake_InitGame(); 
                // 背光常亮
                MMIDEFAULT_AllowTurnOffBackLight(FALSE);
                MMIPUB_CloseQuerytWin(PNULL);
            }
            break;
        
        case MSG_PROMPTWIN_CANCEL:
            {
                // 按右键退出游戏
                s_snake_game_state = SNAKE_GAME_OVER;
                Snake_SaveGameScore();
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
//  FUNCTION:     Snake_ViewScore
//  Description :   显示分数
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
LOCAL void Snake_ViewScore(
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

    MMI_GetLabelTextByLang(title_id, &temp_str);
    sprintf((char*)asc_str,":%d",score);
    str2.wstr_len = strlen((char*)asc_str);     
    str2.wstr_ptr = wstr;
    MMI_STRNTOWSTR( wstr, 12, asc_str, 12, str2.wstr_len );
    result = MMIGMSNAKE_AddTwoString(temp_str,str2);
    if (PNULL == result.wstr_ptr || 0 == result.wstr_len)
    {
        return;
    }
    GUILABEL_SetFont(ctrl_id, MMI_DEFAULT_TEXT_FONT, MMI_BLACK_COLOR);
    GUILABEL_SetText(ctrl_id, &result, FALSE);

    SCI_FREE(result.wstr_ptr);

}

/*****************************************************************************/
//  FUNCTION:     HandleSnakeScoreViewWinMsg
//  Description :   查看最高分数
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSnakeScoreViewWinMsg( 
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          )
{
    MMI_RESULT_E        recode          = MMI_RESULT_TRUE;
    static uint16       high_score      = 0;
    uint16              nv_return       = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
     
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {
                // 读nv
                MMINV_READ(MMINV_GMSNAKE_HIGH_SCORE, &high_score, nv_return);
                if (MN_RETURN_SUCCESS != nv_return)
                {
                    high_score = 0;
                    MMINV_WRITE(MMINV_GMSNAKE_HIGH_SCORE,&high_score);
                }
                // 设置softkey
                GUIWIN_SetSoftkeyTextId(win_id,  TXT_SNAKE_RESET,TXT_NULL, STXT_RETURN,FALSE);
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
                Snake_ViewScore(win_id, MMIGMSNAKE_SCORE_CTRL_ID, TXT_SNAKE_HIGHSCORE, high_score);
            }
            break;
            
        case MSG_GET_FOCUS:
            break;
        
        case MSG_TIMER:
            break;
        
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            {
                // 提示分数重置
                MMIPUB_OpenQueryWinByTextId(TXT_GMSQU_RESET_HIGHT_SCORE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
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
                high_score = 0;
                MMINV_WRITE(MMINV_GMSNAKE_HIGH_SCORE, ( int16*)&high_score);
                Snake_ViewScore(win_id, MMIGMSNAKE_SCORE_CTRL_ID, TXT_SNAKE_HIGHSCORE, high_score);
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
//  FUNCTION:      CancelSnakeSetLevelWinMsg
//  Description :  
//  Global resource dependence : 
//  Author:        peilong.zhou 
//  Note:   
/*****************************************************************************/
LOCAL void  CancelSnakeSetLevelWinMsg(void)
{
    MMK_CloseWin(SNAKE_SETLEVEL_WIN_ID);
}

/*****************************************************************************/
//  FUNCTION:      ConfirmSnakeSetLevelWinMsg
//  Description :    设定难度
//  Global resource dependence : 
//  Author:        peilong.zhou 
//  Note:   
/*****************************************************************************/
LOCAL void  ConfirmSnakeSetLevelWinMsg(void)
{
    int8    level[2]  = {0};    
    uint16  sel       = 0;
    uint16  nv_return = 0;    
    
    sel = GUILIST_GetCurItemIndex(MMIGMSNAKE_SETLEVEL_CTRL_ID);
    s_snake_current_level = (SNAKE_LEVEL_E)sel;
    //读nv
    MMINV_READ(MMINV_GMSNAKE_LEVER_SET,level, nv_return);   
    if (MN_RETURN_SUCCESS != nv_return)
    {
        SCI_MEMSET(&level[0], 0, sizeof(level));
        MMINV_WRITE(MMINV_GMSNAKE_LEVER_SET, &level[0]);
    }  
    // 设定难度
    level[1] = sel; 
    MMINV_WRITE(MMINV_GMSNAKE_LEVER_SET, level); 
             
    MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
    
    MMK_CloseWin(SNAKE_SETLEVEL_WIN_ID);
}

/*****************************************************************************/
//  FUNCTION:      HandleSnakeSetLevelWinMsg
//  Description :  
//  Global resource dependence : 
//  Author:        peilong.zhou 
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSnakeSetLevelWinMsg(
                                              MMI_WIN_ID_T win_id,  
                                              MMI_MESSAGE_ID_E msg_id,  
                                              DPARAM param
                                              )
{
    MMI_RESULT_E  recode    = MMI_RESULT_TRUE;
    uint32 i =0;
	uint16 nv_return = 0;
	uint8 level[2] = {0};
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {
                MMI_TEXT_ID_T  level_text = {0};

                // 创建难度菜单
                GUILIST_SetMaxItem(MMIGMSNAKE_SETLEVEL_CTRL_ID, SNAKE_TOTAL_LEVEL, FALSE );//max item 3
                for (i = 0; i < SNAKE_TOTAL_LEVEL; i++)
                {
                    level_text = Snake_GetLevelText(i);
                    MMIAPISET_AppendListItemByTextId(level_text, STXT_OPTION, MMIGMSNAKE_SETLEVEL_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO );
                    
                }

				MMINV_READ(MMINV_GMSNAKE_LEVER_SET, level, nv_return);
				if (MN_RETURN_SUCCESS != nv_return)
				{
					SCI_MEMSET(&level[0], 0, sizeof(level));
					MMINV_WRITE(MMINV_GMSNAKE_LEVER_SET, &level[0]);
				}
				s_snake_current_level = (SNAKE_LEVEL_E)level[1];
                GUILIST_SetCurItemIndex( MMIGMSNAKE_SETLEVEL_CTRL_ID, (uint16)s_snake_current_level);
                GUILIST_SetSelectedItem(MMIGMSNAKE_SETLEVEL_CTRL_ID, (uint16)s_snake_current_level, TRUE);
                GUIWIN_SetSoftkeyTextId(win_id, STXT_SOFTKEY_OK_MK,COMMON_TXT_NULL,STXT_RETURN,FALSE);
                MMK_SetAtvCtrl( win_id,  MMIGMSNAKE_SETLEVEL_CTRL_ID);
            }
            break;
        
        case MSG_CTL_PENOK:
        case MSG_CTL_OK: 
        case MSG_APP_OK:
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
            {
                ConfirmSnakeSetLevelWinMsg();
            }
            break;
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            {
                CancelSnakeSetLevelWinMsg();
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
//  FUNCTION:     HandleSnakeHelpWinMsg
//  Description :   handle help win msg
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSnakeHelpWinMsg( 
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
    
                MMK_SetAtvCtrl(win_id, MMIGMSNAKE_HELP_CTRL_ID);  
                // 画帮助
                MMI_GetLabelTextByLang(TXT_SNAKE_HELP, &string);
                GUITEXT_SetString(MMIGMSNAKE_HELP_CTRL_ID , string.wstr_ptr, string.wstr_len, FALSE);
                GUITEXT_SetFont(MMIGMSNAKE_HELP_CTRL_ID, PNULL, &text_color);                
            }
            break;
        
        case MSG_FULL_PAINT:
            break;
    
        case MSG_CTL_OK:
        case MSG_APP_OK:
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

#endif

