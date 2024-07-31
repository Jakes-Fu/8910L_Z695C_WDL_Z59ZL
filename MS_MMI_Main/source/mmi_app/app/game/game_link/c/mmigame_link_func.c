
/****************************************************************************
** File Name:      mmigame_link_func.c                            
** Author:            Annie.an                                                      
** Date:            2006/04/01
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to create windows of quintet
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 04/01/2006     annie.an          Create
** 
****************************************************************************/
#define _MMIGAME_LINK_FUNC_C_  

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_game_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_LINK_SUPPORT
#include "window_parse.h"
//#include "mmigame.h"
#include "mmigame_internal.h"
#include "mmigame_text.h"
#include "mmigame_link_text.h"   
#include "mmigame_link_internal.h"
#include "mmigame_link_nv.h"
#include "guilistbox.h"
#include "guimsgbox.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmiacc_winmain.h" 
#include "mmidisplay_data.h" 
#include "mmigame_link_id.h"
#include "mmigame_link_image.h"
#include "guilcd.h"
#include "dal_img.h"
#include "tb_dal.h"
#include "mmi_nv.h"
#include "guiref_scale.h"
#include "mmk_timer.h"
#include "guilcd.h"
#include "mmigame_position.h"
#include "guires.h"
#include "guistring.h"
#include "mmi_resource.h"
#include "guibutton.h"
#include "mmialarm_export.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define MMIGMLINK_TEXT_FONT SONG_FONT_14


/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARE                                                                           *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//   FUNCTION:     MMIGAME_GetRctTime
//  Description :   
//
//  Global resource dependence : 
//   Author:      
//  Note:   
/*****************************************************************************/
LOCAL MMIACC_RTC_TIME_T *MMIGMLINK_GetRctTime( MMIACC_RTC_TIME_T *time_ptr );

/*****************************************************************************/
//  FUNCTION:     GetConnectPath
//  Description :   get a connected path between two tile
//  Global resource dependence :   s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetConnectPath(
                             LINKS_TILE_T   tile1,
                             LINKS_TILE_T   tile2,
                             LINKS_PATH_T*  path
                             );

/*****************************************************************************/
//  Description : set back color
//  Global resource dependence : none
//  Author: Hao.Zhang
//  Note:
/*****************************************************************************/
//LOCAL void SetBackColor(void);
/*****************************************************************************/
//  FUNCTION:     GetAConnectedTilePair
//  Description :   get a connected tile pair from current board
//  Global resource dependence :   s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetAConnectedTilePair(
                                    LINKS_TILE_T* tile_pair
                                    );
/*****************************************************************************/
//  FUNCTION:     GetLeftRightConnectPath
//  Description :   get a connected path between two tile at same row
//  Global resource dependence :   s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetLeftRightConnectPath(
                                      LINKS_TILE_T   tile1,
                                      LINKS_TILE_T   tile2,
                                      LINKS_PATH_T*  path
                                      );

/*****************************************************************************/
//  FUNCTION:     GetTopBottomConnectPath
//  Description :   get a connected path between two tile at same column
//  Global resource dependence :   s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetTopBottomConnectPath(
                                      LINKS_TILE_T   tile1,
                                      LINKS_TILE_T   tile2,
                                      LINKS_PATH_T*  path
                                      );

/*****************************************************************************/
//  FUNCTION:     GetLeftBottomRightTopConnectPath
//  Description :   get a connected path between two tile one at left bottom
//                  and one at right top
//  Global resource dependence :   s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetLeftBottomRightTopConnectPath(
                                               LINKS_TILE_T   tile1,
                                               LINKS_TILE_T   tile2,
                                               LINKS_PATH_T*  path
                                               );

/*****************************************************************************/
//  FUNCTION:     GetLeftTopRightBottomConnectPath
//  Description :   get a connected path between two tile one at left top
//                  and one at right bottom
//  Global resource dependence :   s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetLeftTopRightBottomConnectPath(
                                               LINKS_TILE_T   tile1,
                                               LINKS_TILE_T   tile2,
                                               LINKS_PATH_T*  path
                                               );
/*****************************************************************************/
//  FUNCTION:     IsCanHorizontalDirectConnect
//  Description :   check if two tile can be direct connect horizontally
//  Global resource dependence :   s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsCanHorizontalDirectConnect(
                                           LINKS_TILE_T   tile1,
                                           LINKS_TILE_T   tile2
                                           );

/*****************************************************************************/
//  FUNCTION:     IsCanVerticalDirectConnect
//  Description :   check if two tile can be direct connect vertically
//  Global resource dependence :   s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsCanVerticalDirectConnect(
                                         LINKS_TILE_T   tile1,
                                         LINKS_TILE_T   tile2
                                         );

/*****************************************************************************/
//  FUNCTION:     ProcessKeyWinMsg
//  Description :   
//  Global resource dependence : 
//  Author: juan.zhang
//  Note:   
/*****************************************************************************/
LOCAL void MoveLeft(void);

/*****************************************************************************/
//  FUNCTION:     ProcessKeyWinMsg
//  Description :   
//  Global resource dependence : 
//  Author: juan.zhang
//  Note:   
/*****************************************************************************/
LOCAL void MoveRight(void);

/*****************************************************************************/
//  FUNCTION:     ProcessKeyWinMsg
//  Description :   
//  Global resource dependence : 
//  Author: juan.zhang
//  Note:   
/*****************************************************************************/
LOCAL void MoveUp(void);

/*****************************************************************************/
//  FUNCTION:     ProcessKeyWinMsg
//  Description :   
//  Global resource dependence : 
//  Author: juan.zhang
//  Note:   
/*****************************************************************************/
LOCAL void MoveDown(void);

/**---------------------------------------------------------------------------*
**                       LOCAL DATA DECLARATION
**---------------------------------------------------------------------------*/
PUBLIC uint8 g_links_map_row_num = 0;
PUBLIC uint8 g_link_map_column_num = 0;

PUBLIC LINKS_LEVEL_E  s_links_current_level = LINKS_LEVEL_EASY;      //当前的难度
PUBLIC LINKS_TILE_T   s_links_board[LINKS_BOARD_ROWS][LINKS_BOARD_COLS]; //地图

LOCAL uint8           s_links_time_left = 0;          //当前游戏关剩余时间
LOCAL uint16          s_links_score = 0;              //总计分数
LOCAL LINKS_EPISODE_E s_links_current_episode = LINKS_EPISODE_STILL;    //当前是第几关 0-based
LOCAL uint8           s_links_random_times = 0;       //洗牌次数
LOCAL uint8           s_links_tip_times = 0;          //提示次数
LOCAL LINKS_TILE_T    *s_links_last_pressed_tile_ptr = NULL;  //上一次点击的子图片
LOCAL LINKS_TILE_T    s_links_connected_pair[2];   //当前局面可以连接的一个图片对

LOCAL BOOLEAN         s_can_resume = FALSE;
LOCAL MMIACC_RTC_TIME_T s_time;

extern uint8           s_game_over_timer;   //计时器
extern uint8           s_links_timer;       //计时器
extern BOOLEAN         s_links_is_paused ;  //是否暂停
extern LINKS_GAME_STATE_E   s_links_game_state;

PUBLIC int32 g_LINKS_TILES_RECT_LEFT = 0;
PUBLIC int32 g_LINKS_TILES_RECT_TOP = 0;


/**---------------------------------------------------------------------------*
**                       function bodies  -- local
**---------------------------------------------------------------------------*/
PUBLIC  BOOLEAN  MMIGMLINK_Init(void)
{
    uint16 nv_return = 0;
    uint16 high_score[3] = {0};
    uint8 lever[6] = {0};
    
    MMINV_READ(MMINV_GMLINK_LEVER_SET, (uint16*)lever, nv_return);
    if (MN_RETURN_SUCCESS != nv_return)
    {
        SCI_MEMSET(&lever[0], 0, 6);
        MMINV_WRITE(MMINV_GMLINK_LEVER_SET, &lever[0]);
    }
    
    
    MMINV_READ(MMINV_GMLINK_HIGH_SCORE, ( int16*)high_score, nv_return);   
    if (MN_RETURN_SUCCESS != nv_return)
    {
        SCI_MEMSET(&high_score[0], 0, 6);
        MMINV_WRITE(MMINV_GMLINK_HIGH_SCORE, &high_score[0]);
    }
    
    return TRUE;
    
}


/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_InitModule
//  Description :   
//
//  Global resource dependence : 
//   Author:        taul.zhu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMLINK_InitModule(void)
{
    MMIGMLINK_RegMenuGroup();  //初始化im menu
    MMIGMLINK_RegNv();   //初始化im nv
    MMIGAME_LINK_RegWinIdNameArr();
#ifdef MMI_TASK_MANAGER
    MMIGMLINK_RegAppletInfo();
#endif
}
/*****************************************************************************/
//   FUNCTION:     MMIGAME_AddTwoString
//  Description :   add two string 
//
//  Global resource dependence : 
//   Author:      
//  Note:   
/*****************************************************************************/
PUBLIC MMI_STRING_T MMIGMLINK_AddTwoString(
                                           MMI_STRING_T first, 
                                           MMI_STRING_T second
                                           )
{
    MMI_STRING_T result = {0};
    // uint16 i = 0;
	uint16	j = 0;
    
//     result.is_ucs2 = first.is_ucs2 || second.is_ucs2;
//     if (result.is_ucs2 && !first.is_ucs2) result.length = first.length *2;
//     else result.length = first.length;
//     if (result.is_ucs2 && !second.is_ucs2) result.length += second.length *2;
//     else result.length += second.length;
    
    result.wstr_len = first.wstr_len + second.wstr_len;

    result.wstr_ptr = (wchar*)SCI_ALLOC_APP( (result.wstr_len + 1) * sizeof(wchar) );

    if (PNULL == result.wstr_ptr)
    {
        result.wstr_len = 0;
        return result;
    }
    
    MMI_WSTRNCPY( result.wstr_ptr, result.wstr_len, first.wstr_ptr, first.wstr_len, first.wstr_len );
    j = first.wstr_len;

    // add first string    
//     if (result.is_ucs2 && !first.is_ucs2 )
//     {
//         for(i =0,j=0; i<first.length; i++)          // change to ucs2
//         {
//             result.str_ptr[j++] = 0;                    // first byte is 0
//             result.str_ptr[j++] = first.str_ptr[i];
//         }       // end of for
//     }else
//     {
//         MMI_MEMCPY( result.str_ptr, result.length, first.str_ptr, first.length, first.length);
//         j = first.length;
//     }
    
    MMI_WSTRNCPY( result.wstr_ptr+j, result.wstr_len-j, second.wstr_ptr, second.wstr_len, second.wstr_len );

    // add second string
//     if (result.is_ucs2 && !second.is_ucs2 )
//     {
//         for(i =0; i<second.length; i++)          // change to ucs2
//         {
//             result.str_ptr[j++] = 0;                    // first byte is 0
//             result.str_ptr[j++] = second.str_ptr[i];
//         }       // end of for
//     }else 
//     {
//         MMI_MEMCPY( result.str_ptr+j, result.length-j, second.str_ptr, second.length, second.length);
//     }
    
    return result;
    
} 
#ifdef MAINLCD_SIZE_176X220
/*****************************************************************************/
//  FUNCTION:     SetBlackBackgroung
//  Description :   SetBlackBackgroundColor
//  Author:      mary.xiao
//  Note:   
/*****************************************************************************/
LOCAL void SetBlackBackgroung()	//MODIFY BY MARY 2010/12/08
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN}; 

    if(MMITHEME_IsMainScreenLandscape())
    {
        GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
        GUI_FillRect(&lcd_dev_info, rect, MMI_BACKGROUND_COLOR);    
    }
    else
    {
        GUI_RECT_T rect = MMITHEME_GetClientRect();
        rect.top = 0;   
        GUI_FillRect(&lcd_dev_info, rect, MMI_BACKGROUND_COLOR);
    }       
}
#endif
/*****************************************************************************/
//  FUNCTION:     DrawAll
//  Description :   dispaly all the interface
//  Global resource dependence : s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawAll(
                    MMI_WIN_ID_T    win_id
                    )
{
    uint8 i = 0, j = 0;
#ifdef MAINLCD_SIZE_176X220//MODIFY BY MARY 2010/12/08
    SetBlackBackgroung();
#elif defined MAINLCD_SIZE_240X400
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();

    if(!MMITHEME_IsMainScreenLandscape())
    {
        rect.bottom = rect.bottom - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT; 
    }
    LCD_FillRect(&lcd_dev_info, rect, MMI_BACKGROUND_COLOR);// modify by mary
#else    
    SetBackColor(win_id);
#endif

    //draw status strings
    DrawTimeRect(win_id);
    DrawTimeString(win_id);
    DrawScoreRect(win_id);
    DrawScoreString(win_id);
    DrawEpisodeRect(win_id);
    DrawEpisodeString();
    DrawWashCardRect(win_id);
    DrawWashCardString();
    DrawTipRect(win_id);
    DrawTipString(win_id);
    DrawPauseRect();
    DrawPauseString(win_id);

   if(MMITHEME_IsMainScreenLandscape())
      {
        GUIBUTTON_Update(MMIGMLINK_BUTTON_CTRL_ID);
      }  

    //draw tiles
    for (i = 1; i < LINKS_BOARD_ROWS - 1; i++)
    {
        for (j = 0; j < LINKS_BOARD_COLS; j++)
        {
            DrawTile(s_links_board[i][j], LINKS_TILE_NORMAL,win_id);
        }
    }
    
    //added by ying.xu,2010601修改选中一个图标按暂停再恢复后，图标选中标志消失的问题
     if (s_links_last_pressed_tile_ptr != NULL)
     {
         DrawTile(*s_links_last_pressed_tile_ptr, LINKS_TILE_PRESSED,win_id);
     }
     //end of adding

    if (s_links_is_paused)
    {
        DrawPauseScreen(win_id);
    }   
    //  GUIWIN_SetSoftkeyTextId(MMK_ConvertIdToHandle(win_id),  TXT_NULL,TXT_NULL,STXT_RETURN,TRUE);
                
}


/*****************************************************************************/
//  FUNCTION:     OnTimerAction
//  Description :   
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void OnTimerAction(
                          MMI_WIN_ID_T  win_id
                          )
{
    if (s_links_time_left > 1)
    {
        s_links_time_left--;
        DrawTimeString(win_id);
    }
    else
    {
        s_links_time_left = 0;
        s_can_resume = FALSE; 
        
        DrawTimeString(win_id);
        if (s_links_timer >0 )
        {
            MMK_StopTimer(s_links_timer);
            s_links_timer = 0;
        }
        DrawGameOverScreen(win_id);
        s_links_game_state = LINKS_GAME_OVER;
        if (s_game_over_timer != 0)
        {
            MMK_StopTimer(s_game_over_timer);
            s_game_over_timer = 0;
        }
        s_game_over_timer = MMK_CreateTimer(MMI_2SECONDS,FALSE);
        EndingGameLinks(win_id); 
    }
}

/*****************************************************************************/
//  FUNCTION:     DrawConnectLine
//  Description :   dispaly connect line
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawConnectLine(
                            LINKS_PATH_T   path
                            )
{
    uint8 i = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
    for (i = 1; i < path.point_number; i++)
    {
        if (path.points[i].y >= path.points[i - 1].y)
        {
            LCD_DrawLine(&lcd_dev_info, path.points[i - 1].x, path.points[i - 1].y, 
                path.points[i].x, path.points[i].y, COLOR_LINKS_CONNECT_LINE);
        }
        else
        {
            LCD_DrawLine(&lcd_dev_info, path.points[i].x, path.points[i].y, 
                path.points[i - 1].x, path.points[i - 1].y, COLOR_LINKS_CONNECT_LINE);
        }
    }
}

/*****************************************************************************/
//  Description : 文本风格
//  Global resource dependence : none
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL __inline void SetLinkTextStyle(
                                       GUISTR_STYLE_T *style_ptr,
                                       GUI_ALIGN_E align,
                                       GUI_FONT_T  font,
                                       GUI_COLOR_T font_color
                                       )
{
    //SCI_ASSERT(PNULL != style_ptr);
    if(PNULL == style_ptr)
    {
        //SCI_TRACE_LOW:"[mmigame_link_func.c] SetLinkTextStyle (PNULL == style_ptr)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_FUNC_487_112_2_18_2_21_24_4,(uint8*)"");
        return;
    }
    style_ptr->align = align;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
    style_ptr->line_space = 0;
}

/*****************************************************************************/
//  FUNCTION:     DrawPauseScreen
//  Description :   dispaly pause screen
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawPauseScreen(
                            MMI_WIN_ID_T    win_id
                            )
{
    // GUI_POINT_T         dis_point = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMISET_LANGUAGE_TYPE_E  language_type = MMISET_MAX_LANGUAGE;
    // GAME_LINK_MMI_IMAGE_ID_E image_id = IMAGE_COMMON_BG;/*lint !e64*/
    
    GUI_RECT_T      bg_img_rect   = MMITHEME_GetFullScreenRect();
    
    
    MMI_STRING_T        string = {0};
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    
    MMIAPISET_GetLanguageType(&language_type);
    
    switch(language_type) 
    {
    case MMISET_LANGUAGE_SIMP_CHINESE:
        MMI_GetLabelTextByLang( (MMI_TEXT_ID_T)TXT_PAUSE,  &string);
        break;
        
    default:
        MMI_GetLabelTextEnglish( (MMI_TEXT_ID_T)TXT_PAUSE,  &string);
        break;
    }
    
// #ifdef MAINLCD_SIZE_176X220    
    LCD_FillRect(&lcd_dev_info, bg_img_rect, MMI_BACKGROUND_COLOR);//modifyby mary2010/12/08
//#elif defined MAINLCD_SIZE_240X400    
//    LCD_FillRect(&lcd_dev_info, bg_img_rect, MMI_BACKGROUND_COLOR);// modify by mary
//#elif defined (MMI_PDA_SUPPORT)
//    LCD_FillRect(&lcd_dev_info, bg_img_rect, MMI_BACKGROUND_COLOR);
//#else
//    GUIRES_DisplayImg(PNULL,
//        &bg_img_rect,
//        &bg_img_rect,
//        win_id,
//        IMAGE_COMMON_BG,
//        &lcd_dev_info);
//#endif
    
    
    SetLinkTextStyle(&text_style, ALIGN_HVMIDDLE, MMI_DEFAULT_BIG_FONT, COLOR_LINKS_STRING);
    if(MMITHEME_IsMainScreenLandscape())
    {
        uint16 button_height = 0;
        uint16 button_width = 0;
        GUIRES_GetImgWidthHeight(&button_width, &button_height,IMAGE_PAUSE_BACKGROUND_ICON, win_id);  
        
        bg_img_rect.right = ((LINKS_PAUSE_STRING_RECT_H_RIGHT + LINKS_PAUSE_STRING_RECT_H_LEFT) >> 1) -(button_width >> 1);
    }
    // display
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&bg_img_rect,       //the fixed display area
        (const GUI_RECT_T      *)&bg_img_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    
    //draw status strings
    DrawTimeRect(win_id);
    DrawTimeString(win_id);
    DrawScoreRect(win_id);
    DrawScoreString(win_id);
    DrawEpisodeRect(win_id);
    DrawEpisodeString();
    DrawWashCardRect(win_id);
    DrawWashCardString();
    DrawTipRect(win_id);
    DrawTipString(win_id);
    DrawPauseRect();
    DrawPauseString(win_id);
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        GUIBUTTON_Update(MMIGMLINK_BUTTON_CTRL_ID);
    }  
    else
    {
        GUIWIN_UpdateSoftkey(win_id);
    }
}


/*****************************************************************************/
//  FUNCTION:     DrawGameOverScreen
//  Description :   dispaly game over screen
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawGameOverScreen(MMI_WIN_ID_T win_id)
{
    uint8               len          = 0;
    uint8               pstr[12]     = {0};
    wchar               wstr[12]     = {0};
    GUI_POINT_T         startpoint   = {0};
    GUI_RECT_T          rect         = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T        string = {0};

      rect = MMITHEME_GetFullScreenRect();
      
   GUIRES_DisplayImg(&startpoint,
        &rect,
        &rect,
        win_id,
        IMAGE_COMMON_BG,
        &lcd_dev_info);

       if(MMITHEME_IsMainScreenLandscape())
      {
        GUIBUTTON_Update(MMIGMLINK_BUTTON_CTRL_ID);
      }  
      else
      {
          GUIWIN_UpdateSoftkey(win_id);
      }
   
    MMIRES_GetText( TXT_GAME_OVER, win_id, &string );

    SetLinkTextStyle(&text_style, ALIGN_HVMIDDLE, MMIGMLINK_TEXT_FONT, COLOR_LINKS_STRING);
    // display
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    
    sprintf((char*)pstr, "%d", s_links_score);  
    len = SCI_STRLEN((char*)pstr);

    MMI_STRNTOWSTR( wstr, 12, pstr, 12, len );

    rect.top += 40;
    // string info
    string.wstr_len = len;
    string.wstr_ptr = wstr;
    // display
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
}

/*****************************************************************************/
//  FUNCTION:     DrawGameFinishScreen
//  Description :   dispaly game finish screen
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawGameFinishScreen(MMI_WIN_ID_T win_id)
{
    uint8               len          = 0;
    uint8               pstr[12]     = {0};
    wchar               wstr[12]     = {0};
 
    GUI_RECT_T          rect         = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
 
      GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T        string = {0};

   rect = MMITHEME_GetFullScreenRect();    
    
   GUIRES_DisplayImg(PNULL,
        &rect,
        &rect,
        win_id,
        IMAGE_COMMON_BG,
        &lcd_dev_info);

      if(MMITHEME_IsMainScreenLandscape())
      {
        GUIBUTTON_Update(MMIGMLINK_BUTTON_CTRL_ID);
      }  
      else
      {
          GUIWIN_UpdateSoftkey(win_id);
      }
    
    MMIRES_GetText( TXT_GAME_FINISH, win_id, &string );

    SetLinkTextStyle(&text_style, ALIGN_HVMIDDLE, MMIGMLINK_TEXT_FONT, COLOR_LINKS_STRING);
    // display
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    
    sprintf((char*)pstr, "%d", s_links_score);  
    len = SCI_STRLEN((char*)pstr);

    MMI_STRNTOWSTR( wstr, 12, pstr, 12, len );

 rect.top += 40;
    // string info
    string.wstr_len = len;
    string.wstr_ptr = wstr;
    // display
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
}
/*****************************************************************************/
//  FUNCTION:      InitStage
//  Description :  Initialize a stage
//  Global resource dependence : 
//  Author:        Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void InitStage(
                      void
                      )
{
    uint8   i = 0,j = 0;
    
    //初始化s_links_board
    for (i = 0; i < LINKS_BOARD_ROWS; i++)
    {
        for (j = 0; j < LINKS_BOARD_COLS; j++)
        {
            s_links_board[i][j].id = LINKS_TILE_BLANK;
            s_links_board[i][j].row = i;
            s_links_board[i][j].col = j;
        }
    }
    switch(s_links_current_level)
    {
    case LINKS_LEVEL_EASY:
        //8x8   前8个id有8个
        for (i = 1; i < LINKS_BOARD_ROWS - 1; i++)
        {
            for (j = 1; j < LINKS_BOARD_COLS - 1; j++)
            {
                s_links_board[i][j].id = (LINKS_TILES_ID_E)(LINKS_TILE_ID_0 + i - 1);
            }
        }
        break;
    case LINKS_LEVEL_NORMAL:
        //8x6+4x4  前8个id有6个，后4个有4个
        for (i = 1; i < LINKS_BOARD_ROWS - 1; i++)
        {
            for (j = 1; j <= 6; j++)
            {
                s_links_board[i][j].id = (LINKS_TILES_ID_E)(LINKS_TILE_ID_0 + i - 1);
            }
            for (j = 7; j <= 8; j++)
            {
                s_links_board[i][j].id = (LINKS_TILES_ID_E)(LINKS_TILE_ID_0 + 8 + (i-1)/2);
            }
        }
        break;
    case LINKS_LEVEL_HARD:
        //16x4    16个id每个有4个
        for (i = 1; i < LINKS_BOARD_ROWS - 1; i++)
        {
            for (j = 1; j <= 4; j++)
            {
                s_links_board[i][j].id = (LINKS_TILES_ID_E)(LINKS_TILE_ID_0 + 2 * (i - 1));
            }
            for (j = 5; j <= 8; j++)
            {
                s_links_board[i][j].id = (LINKS_TILES_ID_E)(LINKS_TILE_ID_0 + 2 * (i - 1) + 1);
            }
        }
        break;
    default:
        //SCI_PASSERT(FALSE,("InitStage:s_links_current_level %d is error!",s_links_current_level));
        //SCI_TRACE_LOW:"[mmigame_link_func.c] InitStage s_links_current_level = %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_FUNC_794_112_2_18_2_21_24_5,(uint8*)"d",s_links_current_level);
        break;
    }
    do
    {
        RandomizeBoard();
    } while (!GetAConnectedTilePair(s_links_connected_pair));
    
    s_links_time_left = LINKS_FULL_TIME;
    s_links_last_pressed_tile_ptr = NULL;
    s_links_is_paused = FALSE;
    
    switch(s_links_current_level)
    {
    case LINKS_LEVEL_EASY:
        s_links_time_left += 20;
        break;
    case LINKS_LEVEL_NORMAL:
        break;
    case LINKS_LEVEL_HARD:
        s_links_time_left -= 20;
        break;
    default:
        break;
    }
    
    //  DrawAll();
    

}
/*****************************************************************************/
//   FUNCTION:     MMIGAME_GetRctTime
//  Description :   
//
//  Global resource dependence : 
//   Author:      
//  Note:   
/*****************************************************************************/
LOCAL MMIACC_RTC_TIME_T *MMIGMLINK_GetRctTime( MMIACC_RTC_TIME_T *time_ptr )
{
    SCI_TIME_T systime = {0};
    
    TM_GetSysTime(&systime);

    MMIAPISET_FormatTimeStrByTime(systime.hour, systime.min,(uint8*)time_ptr,RTC_DATE_STRING_LENGTH + 1);
    return( time_ptr );
}

/*****************************************************************************/
//  FUNCTION:     StartGameLinks
//  Description :   
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void StartGameLinks(void)
{
    MMIACC_RTC_TIME_T *rtc = NULL;
    int8 level[6] = {0};
    int16 nv_return = 0;
    
    if (s_links_timer > 0)
    {
        MMK_StopTimer(s_links_timer);
        s_links_timer = 0;
    }
    
    MMINV_READ(MMINV_GMLINK_LEVER_SET,( int16*)level, nv_return);
    //@hao.zhang cr48678 avoid the return value is invalid occasionally
    if (level[2] >= LINKS_TOTAL_LEVEL || level[2] < 0)
    {
        level[2] = LINKS_LEVEL_EASY;
    }
    //@hao.zhang cr48678
    s_links_current_level = (LINKS_LEVEL_E)level[2];
    
    //初始随机数
    rtc = MMIGMLINK_GetRctTime(s_time);/*lint !e64*/
    srand (((*rtc)[0] * 1000 + (*rtc)[1] *100 + (*rtc)[3] * 10 + (*rtc)[4]));
    
    s_links_score = 0;
    s_links_current_episode = LINKS_EPISODE_STILL;
    s_links_random_times = LINKS_TOTAL_WASH_TIMES;
    s_links_tip_times = LINKS_TOTAL_TIP_TIMES;
    s_can_resume = TRUE;
    
    InitStage();
    
    if (0 == s_links_timer)  
    {
        s_links_timer = MMK_CreateTimer( 1000, TRUE);
    }
    
}

/*****************************************************************************/
//  FUNCTION:     SetResumValue
//  Description :   
//  Global resource dependence : NONE
//  Author:       xiaoqing.lu 
//  Note:   
/*****************************************************************************/
PUBLIC void SetResumValue(BOOLEAN is_can_resume)
{
    s_can_resume = is_can_resume;
}

/*****************************************************************************/
//  FUNCTION:     ResumeGameLinks
//  Description :   
//  Global resource dependence : s_links_timer, s_can_resume
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void ResumeGameLinks(void)
{
    if (s_can_resume) 
    {
      //  if (!s_links_is_paused)
       // {
            s_links_is_paused = FALSE ;

            if (0 == s_links_timer)  
            {
                s_links_timer = MMK_CreateTimer( 1000, TRUE);
            }
     //   }
        //        DrawAll();
    }
    else 
    {
        StartGameLinks();
    }
}

/*****************************************************************************/
//  FUNCTION:     EndingGameLinks
//  Description :   
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void EndingGameLinks(
                            MMI_WIN_ID_T win_id
                            )
{
    uint16 high_score[3] = {0};
    uint16 nv_return = 0;
    
    s_can_resume = FALSE;
    
    MMINV_READ(MMINV_GMLINK_HIGH_SCORE, ( int16*)high_score, nv_return);    
    if (s_links_score > high_score[1])  
    {   
        high_score[1] = s_links_score;  
        MMINV_WRITE(MMINV_GMLINK_HIGH_SCORE, ( int16*)high_score);  
    }   
}

/*****************************************************************************/
//  FUNCTION:     ProcessKeyWinMsg
//  Description :   
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC MMI_RESULT_E ProcessKeyWinMsg(
                                     MMI_WIN_ID_T win_id,
                                     MMI_MESSAGE_ID_E  msg_id
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    if(s_links_last_pressed_tile_ptr != &s_links_board[g_links_map_row_num][g_link_map_column_num])
    {
        DrawTile(s_links_board[g_links_map_row_num][g_link_map_column_num], LINKS_TILE_NORMAL,win_id);
    }
    
    switch (msg_id)
    {
    case MSG_KEYDOWN_LEFT:
        MoveLeft();
        break;
        
    case MSG_KEYDOWN_RIGHT:
        MoveRight();
        break;
        
    case MSG_KEYDOWN_UP:
        MoveUp();
        break;
        
    case MSG_KEYDOWN_DOWN:
        MoveDown();
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    if((g_links_map_row_num !=0)&&(g_link_map_column_num!=0))
    {
        DrawTile(s_links_board[g_links_map_row_num][g_link_map_column_num], LINKS_TILE_PRESSED,win_id);
    }
    
    return recode;
}

/*****************************************************************************/
//  FUNCTION:     ProcessKeyWinMsg
//  Description :   
//  Global resource dependence : 
//  Author: juan.zhang
//  Note:   
/*****************************************************************************/
LOCAL void MoveLeft()
{
    uint8 i = 0;
    BOOLEAN is_found = FALSE;
    
    if(0 == g_links_map_row_num)
    {
        g_links_map_row_num = 1;
    }
    if((g_link_map_column_num + g_links_map_row_num*LINKS_BOARD_ROWS)>0)
    {
        for(i = g_link_map_column_num + g_links_map_row_num*LINKS_BOARD_ROWS-1; i >0;i--)
        {
            if(LINKS_TILE_BLANK != s_links_board[i/LINKS_BOARD_COLS][i%LINKS_BOARD_ROWS].id)
            {
                is_found = TRUE;
                break;
            }
        }
        if(is_found)
        {
            g_link_map_column_num = i%LINKS_BOARD_ROWS;
            g_links_map_row_num = i/LINKS_BOARD_ROWS;
        }
    }
}

/*****************************************************************************/
//  FUNCTION:     ProcessKeyWinMsg
//  Description :   
//  Global resource dependence : 
//  Author: juan.zhang
//  Note:   
/*****************************************************************************/
LOCAL void MoveRight()
{
    uint8 i = 0;
    BOOLEAN is_found = FALSE;
    
    if((g_link_map_column_num + g_links_map_row_num*LINKS_BOARD_ROWS) < ((LINKS_BOARD_ROWS-1)*LINKS_BOARD_COLS-2))
    {
        for(i = g_link_map_column_num + g_links_map_row_num*LINKS_BOARD_ROWS+1; i <=((LINKS_BOARD_ROWS-1)*LINKS_BOARD_COLS-2);i++)
        {
            if(LINKS_TILE_BLANK != s_links_board[i/LINKS_BOARD_COLS][i%LINKS_BOARD_ROWS].id)
            {
                is_found = TRUE;
                break;
            }
        }
        if(is_found)
        {
            g_link_map_column_num = i%LINKS_BOARD_ROWS;
            g_links_map_row_num = i/LINKS_BOARD_ROWS;
        }
    }
}

/*****************************************************************************/
//  FUNCTION:     ProcessKeyWinMsg
//  Description :   
//  Global resource dependence : 
//  Author: juan.zhang
//  Note:   
/*****************************************************************************/
LOCAL void MoveUp()
{
    uint8 i = 0;
    BOOLEAN is_found = FALSE;
    
    if((g_links_map_row_num + g_link_map_column_num*LINKS_BOARD_COLS) > 0)
    {
        for(i = (g_links_map_row_num+g_link_map_column_num*LINKS_BOARD_COLS)-1; i > 0;i--)
        {
            if(LINKS_TILE_BLANK != s_links_board[i%LINKS_BOARD_ROWS][i/LINKS_BOARD_COLS].id)
            {
                is_found = TRUE;
                break;
            }
        }
        if(is_found)
        {
            g_link_map_column_num = i/LINKS_BOARD_COLS;
            g_links_map_row_num = i%LINKS_BOARD_ROWS;
        }
    }
}

/*****************************************************************************/
//  FUNCTION:     ProcessKeyWinMsg
//  Description :   
//  Global resource dependence : 
//  Author: juan.zhang
//  Note:   
/*****************************************************************************/
LOCAL void MoveDown()
{
    uint8 i = 0;
    BOOLEAN is_found = FALSE;
    
    if((g_links_map_row_num + g_link_map_column_num*LINKS_BOARD_COLS) < (LINKS_BOARD_ROWS*(LINKS_BOARD_COLS-1)-2))
    {
        for(i = (g_links_map_row_num+g_link_map_column_num*LINKS_BOARD_COLS)+1; i <=(LINKS_BOARD_ROWS*(LINKS_BOARD_COLS-1)-2);i++)
        {
            if(LINKS_TILE_BLANK != s_links_board[i%LINKS_BOARD_ROWS][i/LINKS_BOARD_COLS].id)
            {
                is_found = TRUE;
                break;
            }
        }
        if(is_found)
        {
            g_link_map_column_num = i/LINKS_BOARD_COLS;
            g_links_map_row_num = i%LINKS_BOARD_ROWS;
        }
    }
}

/*****************************************************************************/
//  FUNCTION:     CloseScoreWindow
//  Description :   close score information window
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN CloseScoreWindow(void)
{
    return MMK_CloseWin(LINKS_SVIEW_WIN_ID);
}

/*****************************************************************************/
//  FUNCTION:     CloseResetScoreWindow
//  Description :   close reset score window
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
//PUBLIC BOOLEAN CloseResetScoreWindow(void)
//{
//    return MMK_CloseWin(LINKS_SRES_WIN_ID);
//}
//

/*****************************************************************************/
//  FUNCTION:     GetImageID
//  Description :   get system image id of a tile
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T GetImageID(
                                 LINKS_TILE_T              tile,
                                 LINKS_TILE_STATUS_E     status //子图片状态 普通/点击/提示
                                 )
{
    MMI_IMAGE_ID_T ids[LINKS_TILE_MAX_NUMBER] = { IMAGE_LINKS_01,
        IMAGE_LINKS_02,
        IMAGE_LINKS_03,
        IMAGE_LINKS_04,
        IMAGE_LINKS_05,
        IMAGE_LINKS_06,
        IMAGE_LINKS_07,
        IMAGE_LINKS_08,
        IMAGE_LINKS_09,
        IMAGE_LINKS_10,
        IMAGE_LINKS_11,
        IMAGE_LINKS_12,
        IMAGE_LINKS_13,
        IMAGE_LINKS_14,
        IMAGE_LINKS_15,
        IMAGE_LINKS_16
    };
    //SCI_ASSERT(tile.id >= LINKS_TILE_ID_0 && tile.id < LINKS_TILE_MAX_NUMBER);
    //SCI_PASSERT(tile.id >= LINKS_TILE_ID_0 && tile.id < LINKS_TILE_MAX_NUMBER, ("tile.id = %d", tile.id));
    if((tile.id < LINKS_TILE_ID_0) || (tile.id >= LINKS_TILE_MAX_NUMBER))
    {
        //SCI_TRACE_LOW:"[mmigame_link_func.c]:GetImageID tile.id = %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_FUNC_1159_112_2_18_2_21_25_6,(uint8*)"d",tile.id);
        return ids[0];
    }
    return ids[tile.id];
}

/*****************************************************************************/
//  FUNCTION:     IsAllEliminate
//  Description :   is all tiles are eliminated
//  Global resource dependence : s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN IsAllEliminate(
                              void
                              )
{
    uint8   i = 0, j = 0;
    for (i = 0; i < LINKS_BOARD_ROWS; i++)
    {
        for (j = 0; j < LINKS_BOARD_COLS; j++)
        {
            if (s_links_board[i][j].id != LINKS_TILE_BLANK)
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

/*****************************************************************************/
//  FUNCTION:     AutoMoveTile
//  Description :   move tiles in different episode
//  Global resource dependence : s_links_board, s_links_current_episode
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void AutoMoveTile(
                         LINKS_TILE_T tile1,
                         LINKS_TILE_T tile2
                         )
{
    int8   i = 0, j = 0;
    
    switch (s_links_current_episode)
    {
    case LINKS_EPISODE_MOVEDOWN:
        j = tile1.col;
        for (i = tile1.row - 1; i >= 0; i--)
        {
            s_links_board[i + 1][j].id = s_links_board[i][j].id;
        }
        
        //如果先移动在一列的两个tile的下面一个，那么上面一个的row要修改
        if (tile2.col == tile1.col && tile1.row > tile2.row)
        {
            tile2.row++;
            //SCI_ASSERT(s_links_board[tile2.row][tile2.col].id == LINKS_TILE_BLANK);
            if(s_links_board[tile2.row][tile2.col].id != LINKS_TILE_BLANK)
            {
                //SCI_TRACE_LOW:"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_MOVEDOWN error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_FUNC_1216_112_2_18_2_21_25_7,(uint8*)"dd",tile2.row,tile2.col);
                break;
            }
        }
        
        j = tile2.col;
        for (i = tile2.row - 1; i >= 0; i--)
        {
            s_links_board[i + 1][j].id = s_links_board[i][j].id;
        }
        break;
    case LINKS_EPISODE_MOVELEFT:
        i = tile1.row;
        for (j = tile1.col + 1; j < LINKS_BOARD_COLS; j++)
        {
            s_links_board[i][j - 1].id = s_links_board[i][j].id;
        }
        //如果先移动一行的两个tile的左边一个，那么右边一个的col要修改
        if (tile2.row == tile1.row && tile1.col < tile2.col)
        {
            tile2.col--;
            //SCI_ASSERT(s_links_board[tile2.row][tile2.col].id == LINKS_TILE_BLANK);
            if(s_links_board[tile2.row][tile2.col].id != LINKS_TILE_BLANK)
            {
                //SCI_TRACE_LOW:"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_MOVELEFT error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_FUNC_1240_112_2_18_2_21_25_8,(uint8*)"dd",tile2.row,tile2.col);
                break;
            }
        }
        
        i = tile2.row;
        for (j = tile2.col + 1; j < LINKS_BOARD_COLS; j++)
        {
            s_links_board[i][j - 1].id = s_links_board[i][j].id;
        }
        break;
    case LINKS_EPISODE_V_DISPERSE:
        j = tile1.col;
        //上半部向上
        if (tile1.row < LINKS_BOARD_ROWS / 2)
        {
            for (i = tile1.row + 1; i < LINKS_BOARD_ROWS / 2; i++)
            {
                s_links_board[i - 1][j].id = s_links_board[i][j].id;
            }
            s_links_board[4][j].id = LINKS_TILE_BLANK;
        }
        //下半部向下
        else
        {
            for (i = tile1.row - 1; i >= LINKS_BOARD_ROWS / 2; i--)
            {
                s_links_board[i + 1][j].id = s_links_board[i][j].id;
            }
            s_links_board[5][j].id = LINKS_TILE_BLANK;
        }
        
        //如果移动上半部的一列的两个tile的上面一个，那么下面一个的row要修改
        if (tile1.row < LINKS_BOARD_ROWS / 2 && tile2.row < LINKS_BOARD_ROWS / 2
            && tile1.col == tile2.col && tile1.row < tile2.row)
        {
            tile2.row--;
            //SCI_ASSERT(s_links_board[tile2.row][tile2.col].id == LINKS_TILE_BLANK);
            if(s_links_board[tile2.row][tile2.col].id != LINKS_TILE_BLANK)
            {
                //SCI_TRACE_LOW:"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_V_DISPERSE error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_FUNC_1280_112_2_18_2_21_25_9,(uint8*)"dd",tile2.row,tile2.col);
                break;
            }
        }
        //如果都在下半部，类似
        else if (tile1.row >= LINKS_BOARD_ROWS / 2 && tile2.row >= LINKS_BOARD_ROWS / 2
            && tile1.col == tile2.col && tile1.row > tile2.row)
        {
            tile2.row++;
            //SCI_ASSERT(s_links_board[tile2.row][tile2.col].id == LINKS_TILE_BLANK);
            if(s_links_board[tile2.row][tile2.col].id != LINKS_TILE_BLANK)
            {
                //SCI_TRACE_LOW:"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_V_DISPERSE (else) error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_FUNC_1292_112_2_18_2_21_25_10,(uint8*)"dd",tile2.row,tile2.col);
                break;
            }
        }
        
        j = tile2.col;
        //上半部向上
        if (tile2.row < LINKS_BOARD_ROWS / 2)
        {
            for (i = tile2.row + 1; i < LINKS_BOARD_ROWS / 2; i++)
            {
                s_links_board[i - 1][j].id = s_links_board[i][j].id;
            }
            s_links_board[4][j].id = LINKS_TILE_BLANK;
        }
        //下半部向下
        else
        {
            for (i = tile2.row - 1; i >= LINKS_BOARD_ROWS / 2; i--)
            {
                s_links_board[i + 1][j].id = s_links_board[i][j].id;
            }
            s_links_board[5][j].id = LINKS_TILE_BLANK;
        }
        break;
    case LINKS_EPISODE_V_CENTRALIZE:
        j = tile1.col;
        //上半部向下
        if (tile1.row < LINKS_BOARD_ROWS / 2)
        {
            for (i = tile1.row - 1; i >= 0; i--)
            {
                s_links_board[i + 1][j].id = s_links_board[i][j].id;
            }
        }
        //下半部向上
        else
        {
            for (i = tile1.row + 1; i < LINKS_BOARD_ROWS; i++)
            {
                s_links_board[i - 1][j].id = s_links_board[i][j].id;
            }
        }
        //如果移动上半部的一列的两个tile的下面一个，那么上面一个的row要修改
        if (tile1.row < LINKS_BOARD_ROWS / 2 && tile2.row < LINKS_BOARD_ROWS / 2
            && tile1.col == tile2.col && tile1.row > tile2.row)
        {
            tile2.row++;
            //SCI_ASSERT(s_links_board[tile2.row][tile2.col].id == LINKS_TILE_BLANK);
            if(s_links_board[tile2.row][tile2.col].id != LINKS_TILE_BLANK)
            {
                //SCI_TRACE_LOW:"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_V_CENTRALIZE error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_FUNC_1343_112_2_18_2_21_26_11,(uint8*)"dd",tile2.row,tile2.col);
                break;
            }
        }
        //如果都在下半部，类似
        else if (tile1.row >= LINKS_BOARD_ROWS / 2 && tile2.row >= LINKS_BOARD_ROWS / 2
            && tile1.col == tile2.col && tile1.row < tile2.row)
        {
            tile2.row--;
            //SCI_ASSERT(s_links_board[tile2.row][tile2.col].id == LINKS_TILE_BLANK);
            if(s_links_board[tile2.row][tile2.col].id != LINKS_TILE_BLANK)
            {
                //SCI_TRACE_LOW:"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_V_CENTRALIZE (else) error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_FUNC_1355_112_2_18_2_21_26_12,(uint8*)"dd",tile2.row,tile2.col);
                break;
            }
        }
        
        j = tile2.col;
        //上半部向下
        if (tile2.row < LINKS_BOARD_ROWS / 2)
        {
            for (i = tile2.row - 1; i >= 0; i--)
            {
                s_links_board[i + 1][j].id = s_links_board[i][j].id;
            }
        }
        //下半部向上
        else
        {
            for (i = tile2.row + 1; i < LINKS_BOARD_ROWS; i++)
            {
                s_links_board[i - 1][j].id = s_links_board[i][j].id;
            }
        }
        break;
    case LINKS_EPISODE_H_DISPERSE:
        i = tile1.row;
        //左半部向左
        if (tile1.col < LINKS_BOARD_COLS / 2)
        {
            for (j = tile1.col + 1; j < LINKS_BOARD_COLS / 2; j++)
            {
                s_links_board[i][j - 1].id = s_links_board[i][j].id;
            }
            s_links_board[i][4].id = LINKS_TILE_BLANK;
        }
        //右半部向右
        else
        {
            for (j = tile1.col - 1; j >= LINKS_BOARD_COLS / 2; j--)
            {
                s_links_board[i][j + 1].id = s_links_board[i][j].id;
            }
            s_links_board[i][5].id = LINKS_TILE_BLANK;
        }
        
        //如果移动左半部的一行的两个tile的左面一个，那么右面一个的col要修改
        if (tile1.col < LINKS_BOARD_COLS / 2 && tile2.col < LINKS_BOARD_COLS / 2
            && tile1.row == tile2.row && tile1.col < tile2.col)
        {
            tile2.col--;
            //SCI_ASSERT(s_links_board[tile2.row][tile2.col].id == LINKS_TILE_BLANK);
            if(s_links_board[tile2.row][tile2.col].id != LINKS_TILE_BLANK)
            {
                //SCI_TRACE_LOW:"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_H_DISPERSE error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_FUNC_1407_112_2_18_2_21_26_13,(uint8*)"dd",tile2.row,tile2.col);
                break;
            }
        }
        //如果都在右半部，类似
        else if (tile1.col >= LINKS_BOARD_ROWS / 2 && tile2.col >= LINKS_BOARD_ROWS / 2
            && tile1.row == tile2.row && tile1.col > tile2.col)
        {
            tile2.col++;
            //SCI_ASSERT(s_links_board[tile2.row][tile2.col].id == LINKS_TILE_BLANK);
            if(s_links_board[tile2.row][tile2.col].id != LINKS_TILE_BLANK)
            {
                //SCI_TRACE_LOW:"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_H_DISPERSE (else) error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_FUNC_1419_112_2_18_2_21_26_14,(uint8*)"dd",tile2.row,tile2.col);
                break;
            }
        }
        
        i = tile2.row;
        //左半部向左
        if (tile2.col < LINKS_BOARD_COLS / 2)
        {
            for (j = tile2.col + 1; j < LINKS_BOARD_COLS / 2; j++)
            {
                s_links_board[i][j - 1].id = s_links_board[i][j].id;
            }
            s_links_board[i][4].id = LINKS_TILE_BLANK;
        }
        //右半部向右
        else
        {
            for (j = tile2.col - 1; j >= LINKS_BOARD_COLS / 2; j--)
            {
                s_links_board[i][j + 1].id = s_links_board[i][j].id;
            }
            s_links_board[i][5].id = LINKS_TILE_BLANK;
        }
        break;
    case LINKS_EPISODE_H_CENTRALIZE:
        i = tile1.row;
        //左半部向右
        if (tile1.col < LINKS_BOARD_COLS / 2)
        {
            for (j = tile1.col - 1; j >= 0; j--)
            {
                s_links_board[i][j + 1].id = s_links_board[i][j].id;
            }
        }
        //右半部向左
        else
        {
            for (j = tile1.col + 1; j < LINKS_BOARD_COLS; j++)
            {
                s_links_board[i][j - 1].id = s_links_board[i][j].id;
            }
        }
        
        //如果移动左半部的一行的两个tile的右面一个，那么左面一个的col要修改
        if (tile1.col < LINKS_BOARD_COLS / 2 && tile2.col < LINKS_BOARD_COLS / 2
            && tile1.row == tile2.row && tile1.col > tile2.col)
        {
            tile2.col++;
            //SCI_ASSERT(s_links_board[tile2.row][tile2.col].id == LINKS_TILE_BLANK);
            if(s_links_board[tile2.row][tile2.col].id != LINKS_TILE_BLANK)
            {
                //SCI_TRACE_LOW:"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_H_CENTRALIZE error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_FUNC_1471_112_2_18_2_21_26_15,(uint8*)"dd",tile2.row,tile2.col);
                break;
            }
        }
        //如果都在右半部，类似
        else if (tile1.col >= LINKS_BOARD_ROWS / 2 && tile2.col >= LINKS_BOARD_ROWS / 2
            && tile1.row == tile2.row && tile1.col < tile2.col)
        {
            tile2.col--;
            //SCI_ASSERT(s_links_board[tile2.row][tile2.col].id == LINKS_TILE_BLANK);
            if(s_links_board[tile2.row][tile2.col].id != LINKS_TILE_BLANK)
            {
                //SCI_TRACE_LOW:"[mmigame_link_func.c]:AutoMoveTile LINKS_EPISODE_H_CENTRALIZE (else) error:(s_links_board[tile2.row = %d][tile2.col = %d].id != LINKS_TILE_BLANK)"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_FUNC_1483_112_2_18_2_21_26_16,(uint8*)"dd",tile2.row,tile2.col);
                break;
            }
        }
        
        i = tile2.row;
        //左半部向右
        if (tile2.col < LINKS_BOARD_COLS / 2)
        {
            for (j = tile2.col - 1; j >= 0; j--)
            {
                s_links_board[i][j + 1].id = s_links_board[i][j].id;
            }
        }
        //右半部向左
        else
        {
            for (j = tile2.col + 1; j < LINKS_BOARD_COLS; j++)
            {
                s_links_board[i][j - 1].id = s_links_board[i][j].id;
            }
        }
        break;
    default:
        break;
    }
}

/*****************************************************************************/
//  FUNCTION:     DrawTimeString
//  Description :   dispaly time information
//  Global resource dependence : s_links_time_left
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawTimeString(
                           MMI_WIN_ID_T win_id
                           )
{
    uint8 len = 0;
    uint8 pstr[12] = {0};
    wchar wstr[12] = {0};
    uint8 seconds = 0, minutes = 0;
    GUI_RECT_T rect2 = {0} ;   
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T        string = {0};

    if(MMITHEME_IsMainScreenLandscape())
    {        
        rect2.left = LINKS_TIME_STRING_RECT_H_LEFT;
        rect2.top =  LINKS_TIME_STRING_RECT_H_TOP ;
        rect2.right = LINKS_TIME_STRING_RECT_H_RIGHT;
        rect2.bottom = LINKS_TIME_STRING_RECT_H_BOTTOM;
    }
    else
    {
        rect2.left = LINKS_TIME_STRING_RECT_LEFT;
        rect2.top =  LINKS_TIME_STRING_RECT_TOP ;
        rect2.right = LINKS_TIME_STRING_RECT_RIGHT;
        rect2.bottom = LINKS_TIME_STRING_RECT_BOTTOM;
    }
    
    minutes = s_links_time_left / 60;
    seconds = s_links_time_left % 60;
    sprintf((char*)pstr, "%d:%2.2d", minutes, seconds); 
    
    len = SCI_STRLEN((char*)pstr);

    MMI_STRNTOWSTR( wstr, 12, pstr, 12, len );

    // text
    string.wstr_len = len;
    string.wstr_ptr = wstr;
    // style
    SetLinkTextStyle(&text_style, ALIGN_LVMIDDLE, MMIGMLINK_TEXT_FONT, COLOR_LINKS_STRING);
    // display
//#ifdef MAINLCD_SIZE_176X220 //modify by mary
	LCD_FillRect(&lcd_dev_info, rect2, MMI_BACKGROUND_COLOR);// modify by mary
/*#elif defined MAINLCD_SIZE_240X400    
    LCD_FillRect(&lcd_dev_info, rect2, MMI_BACKGROUND_COLOR);// modify by mary
#elif defined MMI_PDA_SUPPORT
    LCD_FillRect(&lcd_dev_info, rect2, MMI_BACKGROUND_COLOR);
#else
     GUIRES_DisplayImg(PNULL,
        &rect2,
        &rect2,
        win_id,
        IMAGE_COMMON_BG,
        &lcd_dev_info);
        LCD_FillRect(&lcd_dev_info, rect2, MMI_BACKGROUND_COLOR);
#endif*/
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&rect2,       //the fixed display area
        (const GUI_RECT_T      *)&rect2,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
}

/*****************************************************************************/
//  FUNCTION:     DrawTimeRect
//  Description :   dispaly time rect including title
//  Global resource dependence :
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawTimeRect(
                          MMI_WIN_ID_T  win_id
                                             )
{  
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    // GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    // GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    // MMI_STRING_T        string = {0};
    GUI_RECT_T      rect =   {0};
    
     if(MMITHEME_IsMainScreenLandscape())
     {
         rect.left = LINKS_TIME_TITLE_RECT_H_LEFT;
         rect.top =  LINKS_TIME_TITLE_RECT_H_TOP ;
         rect.right = LINKS_TIME_TITLE_RECT_H_RIGHT;
         rect.bottom = LINKS_TIME_TITLE_RECT_H_BOTTOM;
     }
     else
     {
         rect.left = LINKS_TIME_TITLE_RECT_LEFT;
         rect.top =  LINKS_TIME_TITLE_RECT_TOP ;
         rect.right = LINKS_TIME_TITLE_RECT_RIGHT;
         rect.bottom = LINKS_TIME_TITLE_RECT_BOTTOM;
     }  
    
    GUIRES_DisplayImg(PNULL,
        &rect,
        PNULL,
        win_id,
        IMAGE_TIME_ICON,
        &lcd_dev_info);
}

/*****************************************************************************/
//  FUNCTION:     DrawScoreString
//  Description :   dispaly score information
//  Global resource dependence : s_links_score
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawScoreString(MMI_WIN_ID_T    win_id)
{
    uint8 len = 0;
    uint8 pstr[12] = {0};
    wchar wstr[12] = {0};

    GUI_RECT_T rect2 = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T        string = {0};

    if(MMITHEME_IsMainScreenLandscape())
    {     
        rect2.left = LINKS_SCORE_STRING_RECT_H_LEFT;
        rect2.top =  LINKS_SCORE_STRING_RECT_H_TOP;
        rect2.right = LINKS_SCORE_STRING_RECT_H_RIGHT;
        rect2.bottom = LINKS_SCORE_STRING_RECT_H_BOTTOM;
    }
    else
    {        
        rect2.left = LINKS_SCORE_STRING_RECT_LEFT;
        rect2.top =  LINKS_SCORE_STRING_RECT_TOP;
        rect2.right = LINKS_SCORE_STRING_RECT_RIGHT;
        rect2.bottom = LINKS_SCORE_STRING_RECT_BOTTOM;
    }
    
    sprintf((char*)pstr, "%d", s_links_score);

    len = SCI_STRLEN((char*)pstr);
    
    MMI_STRNTOWSTR( wstr, 12, pstr, 12, len );

    // string info
    string.wstr_len = len;
    string.wstr_ptr = wstr;
    //
    SetLinkTextStyle(&text_style, ALIGN_LVMIDDLE, MMIGMLINK_TEXT_FONT, COLOR_LINKS_STRING);
    // display
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&rect2,       //the fixed display area
        (const GUI_RECT_T      *)&rect2,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
}


/*****************************************************************************/
//  FUNCTION:     DrawScoreRect
//  Description :   dispaly score rect including title
//  Global resource dependence :
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawScoreRect(
                          MMI_WIN_ID_T  win_id
                                              )
{
    
    GUI_RECT_T rect = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    // GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    // GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    // MMI_STRING_T        string = {0};

    if(MMITHEME_IsMainScreenLandscape())
    {
       rect.left = LINKS_SCORE_TITLE_RECT_H_LEFT;
       rect.top =  LINKS_SCORE_TITLE_RECT_H_TOP;
       rect.right = LINKS_SCORE_TITLE_RECT_H_RIGHT;
       rect.bottom = LINKS_SCORE_TITLE_RECT_H_BOTTOM;
    }
    else
    {        
        rect.left = LINKS_SCORE_TITLE_RECT_LEFT;
        rect.top =  LINKS_SCORE_TITLE_RECT_TOP;
        rect.right = LINKS_SCORE_TITLE_RECT_RIGHT;
        rect.bottom = LINKS_SCORE_TITLE_RECT_BOTTOM;
    }
   
       GUIRES_DisplayImg(PNULL,
        &rect,
        PNULL,
        win_id,
        IMAGE_SCORE_ICON,
        &lcd_dev_info);
}


/*****************************************************************************/
//  FUNCTION:     DrawEpisodeString
//  Description :   dispaly episode information
//  Global resource dependence : s_links_current_episode
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawEpisodeString(
                              void
                              )
{
    uint8 len = 0;
    uint8 pstr[12] = {0};
    wchar wstr[12] = {0};
    // GUI_POINT_T startpoint = {0};
    GUI_RECT_T rect = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T        string = {0};

    if(MMITHEME_IsMainScreenLandscape())
    {       
        rect.left = LINKS_EPISODE_STRING_RECT_H_LEFT;
        rect.top =  LINKS_EPISODE_STRING_RECT_H_TOP;
        rect.right = LINKS_EPISODE_STRING_RECT_H_RIGHT;
        rect.bottom = LINKS_EPISODE_STRING_RECT_H_BOTTOM;
    }
    else
    {        
        rect.left = LINKS_EPISODE_STRING_RECT_LEFT;
        rect.top =  LINKS_EPISODE_STRING_RECT_TOP ;
        rect.right = LINKS_EPISODE_STRING_RECT_RIGHT;
        rect.bottom = LINKS_EPISODE_STRING_RECT_BOTTOM;
    }
    
    sprintf((char*)pstr, "%d", s_links_current_episode + 1);
    
    len = SCI_STRLEN((char*)pstr);
    
    MMI_STRNTOWSTR( wstr, 12, pstr, 12, len );
    
    // string info
    string.wstr_len = len;
    string.wstr_ptr = wstr;
    //
    SetLinkTextStyle(&text_style, ALIGN_LVMIDDLE, MMIGMLINK_TEXT_FONT, COLOR_LINKS_STRING);
    // display
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
}


/*****************************************************************************/
//  FUNCTION:     DrawEpisodeRect
//  Description :   dispaly episode rect including title
//  Global resource dependence :
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawEpisodeRect(
                            MMI_WIN_ID_T  win_id
                            )
{
    // GUI_POINT_T startpoint = {0};
    GUI_RECT_T rect = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    // GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    // GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    // MMI_STRING_T        string = {0};
    
     if(MMITHEME_IsMainScreenLandscape())
     {        
        rect.left = LINKS_EPISODE_TITLE_RECT_H_LEFT;
        rect.top =  LINKS_EPISODE_TITLE_RECT_H_TOP;
        rect.right = LINKS_EPISODE_TITLE_RECT_H_RIGHT;
        rect.bottom = LINKS_EPISODE_TITLE_RECT_H_BOTTOM;
     }
     else 
     {  
        rect.left = LINKS_EPISODE_TITLE_RECT_LEFT;
        rect.top =  LINKS_EPISODE_TITLE_RECT_TOP;
        rect.right = LINKS_EPISODE_TITLE_RECT_RIGHT;
        rect.bottom = LINKS_EPISODE_TITLE_RECT_BOTTOM;
     }
      GUIRES_DisplayImg(PNULL,
        &rect,
        PNULL,
        win_id,
        IMAGE_EPISODE_ICON,
        &lcd_dev_info);
}


/*****************************************************************************/
//  FUNCTION:     DrawWashCardString
//  Description :   dispaly wash card information
//  Global resource dependence : s_links_random_times
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawWashCardString(
                               void
                               )
{
    uint8 len = 0;
    uint8 pstr[12] = {0};
    wchar wstr[12] = {0};
    // GUI_POINT_T startpoint = {0};  
    GUI_RECT_T rect = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T        string = {0};

  
    sprintf((char*)pstr, "x%d", s_links_random_times);
    len = SCI_STRLEN((char*)pstr);

    MMI_STRNTOWSTR( wstr, 12, pstr, 12, len );

    if(MMITHEME_IsMainScreenLandscape())
    {       
        rect.left = LINKS_WASHCARD_NUMBER_RECT_H_LEFT;
        rect.top =  LINKS_WASHCARD_NUMBER_RECT_H_TOP;
        rect.right = LINKS_WASHCARD_NUMBER_RECT_H_RIGHT;
        rect.bottom = LINKS_WASHCARD_NUMBER_RECT_H_BOTTOM;
    }
    else
    {      
        rect.left = LINKS_WASHCARD_NUMBER_RECT_LEFT;
        rect.top =  LINKS_WASHCARD_NUMBER_RECT_TOP;
        rect.right = LINKS_WASHCARD_NUMBER_RECT_RIGHT;
        rect.bottom = LINKS_WASHCARD_NUMBER_RECT_BOTTOM;
    }
      
    string.wstr_len = len;
    string.wstr_ptr = wstr;

    // style
    SetLinkTextStyle(&text_style, ALIGN_LVMIDDLE, MMIGMLINK_TEXT_FONT, COLOR_LINKS_STRING);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
}


/*****************************************************************************/
//  FUNCTION:     DrawWashCardRect
//  Description :   dispaly wash card rect
//  Global resource dependence :
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawWashCardRect(
                               MMI_WIN_ID_T    win_id  
                             )
{   
      
    GUI_RECT_T rect = {0};   
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN}; 
    // GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    // GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    // MMI_STRING_T        string = {0};
    uint16 button_width = 0;
    uint16 button_height = 0;
    GUIRES_GetImgWidthHeight(&button_width, &button_height,
    IMAGE_WASHCARD_ICON , win_id);      
     
     if(MMITHEME_IsMainScreenLandscape())
    {        
        rect.left = LINKS_WASHCARD_STRING_RECT_H_LEFT;
        rect.right = button_width + rect.left -1;
        rect.top =  LINKS_WASHCARD_STRING_RECT_H_TOP;      
        rect.bottom = rect.top + button_height -1;        
     }
     else
     {
        rect.left =  LINKS_WASHCARD_STRING_RECT_LEFT;
        rect.top =  LINKS_WASHCARD_STRING_RECT_TOP;
        rect.right =  button_width + rect.left -1;
        rect.bottom = button_height + rect.top -1;
     }
    
    GUIRES_DisplayImg(PNULL,
                    &rect,
                    PNULL,
                    win_id,
                    IMAGE_WASHCARD_ICON,
                    &lcd_dev_info);

}


/*****************************************************************************/
//  FUNCTION:     DrawTipString
//  Description :   dispaly tip information
//  Global resource dependence : s_links_tip_times
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawTipString(
                          MMI_WIN_ID_T  win_id
                          )
{
    uint8 len = 0;
    uint8 pstr[12] = {0};
    wchar wstr[12] = {0};
    // GUI_POINT_T startpoint = {0};   
    GUI_RECT_T rect = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN}; 
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T        string = {0};


     if(MMITHEME_IsMainScreenLandscape())
    {        
        rect.left = LINKS_TIP_NUMBER_RECT_H_LEFT;
        rect.top =  LINKS_TIP_NUMBER_RECT_H_TOP ;
        rect.right = LINKS_TIP_NUMBER_RECT_H_RIGHT;
        rect.bottom = LINKS_TIP_NUMBER_RECT_H_BOTTOM;
    }
    else
    {
        rect.left = LINKS_TIP_NUMBER_RECT_LEFT;
        rect.top =  LINKS_TIP_NUMBER_RECT_TOP ;
        rect.right = LINKS_TIP_NUMBER_RECT_RIGHT;
        rect.bottom =LINKS_TIP_NUMBER_RECT_BOTTOM;
    
    }
    sprintf((char*)pstr, "x%d", s_links_tip_times);
    len = SCI_STRLEN((char*)pstr);
    
    MMI_STRNTOWSTR( wstr, 12, pstr, 12, len );
 
    string.wstr_len = len;
    string.wstr_ptr = wstr;
//#ifdef MAINLCD_SIZE_176X220
     LCD_FillRect(&lcd_dev_info, rect, MMI_BACKGROUND_COLOR);//modify by mary 2010/12/08
/*#elif defined MAINLCD_SIZE_240X400    
    LCD_FillRect(&lcd_dev_info, rect, MMI_BACKGROUND_COLOR);// modify by mary
#elif defined MMI_PDA_SUPPORT
    LCD_FillRect(&lcd_dev_info, rect, MMI_BACKGROUND_COLOR);
#else
    GUIRES_DisplayImg(PNULL,
        &rect,
        &rect,
        win_id,
        IMAGE_COMMON_BG,
        &lcd_dev_info);
#endif*/
    // style
    SetLinkTextStyle(&text_style, ALIGN_HVMIDDLE, MMIGMLINK_TEXT_FONT, COLOR_LINKS_STRING);
    // display
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
}

/*****************************************************************************/
//  FUNCTION:     DrawTipRect
//  Description :   dispaly tip rect
//  Global resource dependence :
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawTipRect(
                         MMI_WIN_ID_T    win_id
                            )
{  
     // GUI_POINT_T startpoint = {0};
    GUI_RECT_T rect = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    // GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    // GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    // MMI_STRING_T        string = {0};
    uint16 button_width = 0;
    uint16 button_height = 0;
    GUIRES_GetImgWidthHeight(&button_width, &button_height,
    IMAGE_TIP_ICON , win_id);
    
    if(MMITHEME_IsMainScreenLandscape())
    {               
        rect.left = LINKS_TIP_STRING_RECT_H_LEFT;
        rect.right =  rect.left + button_width -1;    
        rect.top =  LINKS_TIP_STRING_RECT_H_TOP ;
        rect.bottom = rect.top + button_height -1;
    }
    else
    {
        rect.left = LINKS_TIP_STRING_RECT_LEFT;
        rect.top =  LINKS_TIP_STRING_RECT_TOP ;
        rect.right = rect.left  + button_width - 1;
        rect.bottom = LINKS_TIP_STRING_RECT_BOTTOM;  
    }
      GUIRES_DisplayImg(PNULL,
        &rect,
        PNULL,
        win_id,
        IMAGE_TIP_ICON,
        &lcd_dev_info);
}


/*****************************************************************************/
//  FUNCTION:     DrawPauseString
//  Description :   dispaly pause information
//  Global resource dependence : s_links_is_paused
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawPauseString(
                            MMI_WIN_ID_T    win_id
                            )
{
    // uint32 string_id = s_links_is_paused? TXT_COMMON_COMMON_CONTINUE : TXT_GAMELINKS_PAUSE;
    //   uint32 string_id=TXT_COMMON_COMMON_CONTINUE;
    
    GUI_RECT_T rect = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    // GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    // GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    // MMI_STRING_T        string = {0};
    uint16 button_width = 0;
    uint16 button_height = 0;
    GUIRES_GetImgWidthHeight(&button_width, &button_height,
    IMAGE_RESUME_ICON , win_id);  

     if(MMITHEME_IsMainScreenLandscape())
     {
         rect.left = LINKS_PAUSE_STRING_RECT_H_LEFT;
         rect.top =  LINKS_PAUSE_STRING_RECT_H_TOP ;
         rect.right =  rect.left  + button_width -1;
         rect.bottom = rect.top + button_height -1;
     }
     else
     {
         rect.left = LINKS_PAUSE_STRING_RECT_LEFT;
         rect.top =  LINKS_PAUSE_STRING_RECT_TOP ;
         rect.right = rect.left  + button_width-1;//LINKS_PAUSE_STRING_RECT_RIGHT;
         rect.bottom = rect.top + button_height -1;//LINKS_PAUSE_STRING_RECT_BOTTOM;  
     }      
 
    if(s_links_is_paused)
    {
        GUIRES_DisplayImg(PNULL,
        &rect,
        PNULL,
        win_id,
        IMAGE_RESUME_ICON,
        &lcd_dev_info);	 
    }
    else
    {
        GUIRES_DisplayImg(PNULL,
        &rect,
        PNULL,
        win_id,
        IMAGE_PAUSE_ICON,
        &lcd_dev_info);	 
    }
}


/*****************************************************************************/
//  FUNCTION:     DrawPauseRect
//  Description :   dispaly pause rect
//  Global resource dependence :
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawPauseRect(void)
{  // GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    // GUI_RECT_T rect = LINKS_PAUSE_RECT;
    
    //  LCD_FillRect(&lcd_dev_info, rect, COLOR_LINKS_RECT_BACK);
    //  LCD_DrawRect(&lcd_dev_info, rect, COLOR_LINKS_RECT_LINE);
}

/*****************************************************************************/
//  FUNCTION:     DrawTile
//  Description :   dispaly a tile
//  Global resource dependence :
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawTile(
                     LINKS_TILE_T           tile,
                     LINKS_TILE_STATUS_E    status, //子图片状态 普通/点击/提示
                     MMI_WIN_ID_T           win_id
                     )
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    GUI_RECT_T rect = {0};
    GUI_POINT_T startpoint = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
    startpoint.x = LINKS_TILE_X(tile.col);
    if(startpoint.x <= 0)
        startpoint.x = 1;
    startpoint.y = LINKS_TILE_Y(tile.row);
    rect.left = startpoint.x;
    rect.right = startpoint.x + LINKS_TILE_WIDTH-1;
    rect.top = startpoint.y;
    rect.bottom = startpoint.y + LINKS_TILE_HEIGHT-1;
    
    //非空白图片
    if (tile.id != LINKS_TILE_BLANK)
    {
        if (status == LINKS_TILE_NORMAL)
        {
        
            GUIRES_DisplayImg(PNULL,
                &rect,
                PNULL,
                win_id,
                IMAGE_COMMON_BG,
                &lcd_dev_info);   
               
        }
     
        image_id = GetImageID(tile, status);
        
        GUIRES_DisplayImg(&startpoint,
            PNULL,
            PNULL,
            win_id,
            image_id,
            &lcd_dev_info);
        
        if (status != LINKS_TILE_NORMAL)
        {
            GUI_COLOR_T color = (status == LINKS_TILE_PRESSED) ? 
            COLOR_LINKS_MIST_PRESSED : COLOR_LINKS_MIST_TIPED;
            LCD_MistBmp(&lcd_dev_info, rect, color);
        }
    }
    //空白图片
    else
    {  
           rect.left++;
           rect.right--;
           rect.top++;
           rect.bottom--;
           
    }
}


/*****************************************************************************/
//  FUNCTION:      RandomizeBoard
//  Description :  Randomized the game board when game start or need randomize
//  Global resource dependence : 
//  Author:        Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void RandomizeBoard(void)
{
    uint16 i = 0, j = 0, k = 0;
    uint16 tile_num = 0;
    LINKS_TILE_T *tiles = NULL;
    LINKS_TILES_ID_E swap_tile_id = LINKS_TILE_MAX_NUMBER;
    uint16 wash_times = LINKS_WASH_SWAP_TIMES;
    
    //计算局面上的tile总数
    for (i = 0; i < LINKS_BOARD_ROWS; i++)
    {
        for (j = 0; j < LINKS_BOARD_COLS; j++)
        {
            //如果该位置有图片
            if (s_links_board[i][j].id != LINKS_TILE_BLANK 
                && s_links_board[i][j].id < LINKS_TILE_MAX_NUMBER)
            {
                tile_num++;
            }
        }
    }
    
    if (0 == tile_num)
    {
        return;
    }
    //将二维不连续局面保存在一维连续数组中
    tiles = (LINKS_TILE_T *)SCI_ALLOC_APP(tile_num * sizeof(LINKS_TILE_T));
    k = 0;
    for (i = 0; i < LINKS_BOARD_ROWS; i++)
    {
        for (j = 0; j < LINKS_BOARD_COLS; j++)
        {
            //如果该位置有图片
            if (s_links_board[i][j].id != LINKS_TILE_BLANK)
            {
                SCI_MEMCPY(&(tiles[k]), &(s_links_board[i][j]), sizeof(LINKS_TILE_T));
                k++;
            }
        }
    }
    
    //在一维数组中交换洗牌
    while (wash_times > 0)
    {
        i = rand() % tile_num;
        j = rand() % tile_num;
        swap_tile_id = tiles[i].id;
        tiles[i].id = tiles[j].id;
        tiles[j].id = swap_tile_id;
        wash_times--;
    }
    
    for (k = 0; k < tile_num; k++)
    {
        i = tiles[k].row;
        j = tiles[k].col;
        s_links_board[i][j].id = tiles[k].id;
    }
    
    SCI_FREE(tiles);
}

/*****************************************************************************/
//  Description : set back color
//  Global resource dependence : none
//  Author: Hao.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void SetBackColor(
                         MMI_WIN_ID_T   win_id
                         )
{   
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN}; 
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect(); 
    LCD_FillRect(&lcd_dev_info, rect, MMI_BACKGROUND_COLOR); 
}

/*****************************************************************************/
//  FUNCTION:     GetAConnectedTilePair
//  Description :   get a connected tile pair from current board
//  Global resource dependence :   s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetAConnectedTilePair(
                                    LINKS_TILE_T* tile_pair
                                    )
{
    uint16 i = 0, j = 0, k = 0;
    LINKS_TILE_T different_id_tile[LINKS_TILE_MAX_NUMBER][8] = {0}; //每一种图片在一局中最多出现8次
    uint8 different_id_tile_num[LINKS_TILE_MAX_NUMBER] = {0};
    LINKS_PATH_T  path = {0};
    
    //当前局面每一种图片计数置0
    for (i = 0; i < LINKS_TILE_MAX_NUMBER; i++)
    {
        different_id_tile_num[i] = 0;
    }
    //对当前局面的不同图片进行分类
    for (i = 0; i < LINKS_BOARD_ROWS; i++)
    {
        for (j = 0; j < LINKS_BOARD_COLS; j++)
        {
            //如果该位置有图片
            if (s_links_board[i][j].id != LINKS_TILE_BLANK 
                && s_links_board[i][j].id < LINKS_TILE_MAX_NUMBER)
            {
                different_id_tile[s_links_board[i][j].id]
                    [different_id_tile_num[s_links_board[i][j].id]] 
                    = s_links_board[i][j];
                different_id_tile_num[s_links_board[i][j].id]++;
            }
        }
    }
    
    //对每一类寻找是否存在可连接对
    for (i = 0; i < LINKS_TILE_MAX_NUMBER; i++)
    {
        //如果有至少2个这种类型的tile
        if (different_id_tile_num[i] > 1)
        {
            for (j = 0; j < different_id_tile_num[i] - 1; j++)
            {
                for (k = j + 1; k < different_id_tile_num[i]; k++)
                {
                    //如果可连接
                    if (GetConnectPath(different_id_tile[i][j], different_id_tile[i][k], 
                        &path))
                    {
                        SCI_MEMCPY(&(tile_pair[0]), &(different_id_tile[i][j]), 
                            sizeof(LINKS_TILE_T));
                        SCI_MEMCPY(&(tile_pair[1]), &(different_id_tile[i][k]), 
                            sizeof(LINKS_TILE_T));
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}
/*****************************************************************************/
//  FUNCTION:     GetConnectPath
//  Description :   get a connected path between two tile
//  Global resource dependence :   s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetConnectPath(
                             LINKS_TILE_T   tile1,
                             LINKS_TILE_T   tile2,
                             LINKS_PATH_T*  path
                             )
{
    LINKS_TILE_T swap_tile = {0};
    //SCI_ASSERT(path != NULL);
    //SCI_ASSERT(tile1.id != LINKS_TILE_BLANK && tile2.id != LINKS_TILE_BLANK);
    if(path == NULL)
    {
        //SCI_TRACE_LOW:"[mmigame_link_func.c]: GetConnectPath path == NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_FUNC_2364_112_2_18_2_21_28_17,(uint8*)"");
        return FALSE;
    }
    if((tile1.id == LINKS_TILE_BLANK) || (tile2.id == LINKS_TILE_BLANK))
    {
        //SCI_TRACE_LOW:"[mmigame_link_func.c]: GetConnectPath tile1.id = %d,tile2.id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_FUNC_2369_112_2_18_2_21_28_18,(uint8*)"dd",tile1.id,tile2.id);
        return FALSE;
    }

    if (tile1.id == tile2.id)
    {
        //保证tile1在tile2的左边
        if (tile1.col > tile2.col)
        {
            SCI_MEMCPY(&swap_tile, &tile1, sizeof(LINKS_TILE_T));
            SCI_MEMCPY(&tile1, &tile2, sizeof(LINKS_TILE_T));
            SCI_MEMCPY(&tile2, &swap_tile, sizeof(LINKS_TILE_T));
        }
        //如果处于上下位形
        if (tile1.col == tile2.col)
        {
            //保证tile1在tile2的上边
            if (tile1.row > tile2.row)
            {
                SCI_MEMCPY(&swap_tile, &tile1, sizeof(LINKS_TILE_T));
                SCI_MEMCPY(&tile1, &tile2, sizeof(LINKS_TILE_T));
                SCI_MEMCPY(&tile2, &swap_tile, sizeof(LINKS_TILE_T));
            }
            return GetTopBottomConnectPath(tile1, tile2, path);
        }
        //如果处于左右位形
        else if (tile1.row == tile2.row)
        {
            return GetLeftRightConnectPath(tile1, tile2, path);
        }
        //如果左下右上
        else if (tile1.row > tile2.row)
        {
            return GetLeftBottomRightTopConnectPath(tile1, tile2, path);
        }
        //如果左上右下
        else if (tile1.row < tile2.row)
        {
            return GetLeftTopRightBottomConnectPath(tile1, tile2, path);
        }
    }
    return FALSE;
    
}
/*****************************************************************************/
//  FUNCTION:     GetLeftRightConnectPath
//  Description :   get a connected path between two tile at same row
//  Global resource dependence :   s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetLeftRightConnectPath(
                                      LINKS_TILE_T   tile1,
                                      LINKS_TILE_T   tile2,
                                      LINKS_PATH_T*  path
                                      )
{
    uint16 i = 0;
    uint8 org_tile1_row = tile1.row;

 GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect() ;
    // uint16 full_width = full_rect.right - full_rect.left ;
    // uint16 fuu_height = full_rect.bottom - full_rect.top ;
    
    //SCI_ASSERT(tile1.row == tile2.row);
    //SCI_ASSERT(tile1.col < tile2.col);
    if((tile1.row != tile2.row) || (tile1.col >= tile2.col))
    {
        //SCI_TRACE_LOW:"[mmigame_link_func.c]:GetLeftRightConnectPath error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_FUNC_2435_112_2_18_2_21_29_19,(uint8*)"");
        return FALSE;
    }
    
    //搜寻两个图片可否直接连接
    if (IsCanHorizontalDirectConnect(tile1, tile2))
    {
        path->point_number = 2;
        //给path->points赋值为这两个图片
        path->points[0].x = LINKS_TILE_X(tile1.col) + (LINKS_TILE_WIDTH / 2);
        if(path->points[0].x <= 0)
            path->points[0].x = 1;
        path->points[0].y = LINKS_TILE_Y(tile1.row) + (LINKS_TILE_HEIGHT / 2);
        path->points[1].x = LINKS_TILE_X(tile2.col) + (LINKS_TILE_WIDTH / 2);
        if(path->points[1].x <= 0)
            path->points[1].x = 1;
        path->points[1].y = LINKS_TILE_Y(tile2.row) + (LINKS_TILE_HEIGHT / 2);
        return TRUE;
    }
    //搜寻两个图片均向上延伸是否可连接
    i = org_tile1_row - 1;
    while (LINKS_TILE_BLANK == s_links_board[i][tile1.col].id
        && LINKS_TILE_BLANK == s_links_board[i][tile2.col].id)
    {
        tile1.row = i;
        tile2.row = i;
        if (IsCanHorizontalDirectConnect(tile1, tile2))
        {
            path->point_number = 4;
            //给path->points赋值
            path->points[0].x = LINKS_TILE_X(tile1.col) + (LINKS_TILE_WIDTH / 2);
            if(path->points[0].x <= 0)
                path->points[0].x = 1;
            path->points[0].y = LINKS_TILE_Y(org_tile1_row);
            path->points[1].x = LINKS_TILE_X(tile1.col) + (LINKS_TILE_WIDTH / 2);
            if(path->points[1].x <= 0)
                path->points[1].x = 1;
            if (path->points[1].x >= full_rect.right )
                path->points[1].x = full_rect.right -3;
            path->points[1].y = LINKS_TILE_Y(i) + (LINKS_TILE_HEIGHT / 2);
        if(path->points[1].y <= 0)
                path->points[1].y = 1;  
            path->points[2].x = LINKS_TILE_X(tile2.col) + (LINKS_TILE_WIDTH / 2);
            if(path->points[2].x <= 0)
                path->points[2].x = 1;
            if (path->points[2].x >= full_rect.right )
                path->points[2].x = full_rect.right -3;
            path->points[2].y = LINKS_TILE_Y(i) + (LINKS_TILE_HEIGHT / 2);
        if(path->points[2].y <= 0)
                path->points[2].y = 1;      
            path->points[3].x = LINKS_TILE_X(tile2.col) + (LINKS_TILE_WIDTH / 2);
            if(path->points[3].x <= 0)
                path->points[3].x = 1;
            path->points[3].y = LINKS_TILE_Y(org_tile1_row);
            return TRUE;
        }
        i--;
    }
    //搜寻两个图片均向下延伸是否可连接
    i = org_tile1_row + 1;
    while (i < LINKS_BOARD_ROWS && LINKS_TILE_BLANK == s_links_board[i][tile1.col].id
        && LINKS_TILE_BLANK == s_links_board[i][tile2.col].id)
    {
        tile1.row = i;
        tile2.row = i;
        if (IsCanHorizontalDirectConnect(tile1, tile2))
        {
            path->point_number = 4;
            //给path->points赋值
            path->points[0].x = LINKS_TILE_X(tile1.col) + (LINKS_TILE_WIDTH / 2);
            if(path->points[0].x <= 0)
                path->points[0].x = 1;
            path->points[0].y = LINKS_TILE_Y(org_tile1_row) + LINKS_TILE_HEIGHT;
            path->points[1].x = LINKS_TILE_X(tile1.col) + (LINKS_TILE_WIDTH / 2);
            if(path->points[1].x <= 0)
                path->points[1].x = 1;
            if (path->points[1].x >= full_rect.right )
                path->points[1].x = full_rect.right -3;
            path->points[1].y = LINKS_TILE_Y(i) + (LINKS_TILE_HEIGHT / 2);
          if (path->points[1].y >=  full_rect.bottom )
                path->points[1].y = full_rect.bottom -1;
            path->points[2].x = LINKS_TILE_X(tile2.col) + (LINKS_TILE_WIDTH / 2);
            if(path->points[2].x <= 0)
                path->points[2].x = 1;
            if (path->points[2].x >= full_rect.right )
                path->points[2].x = full_rect.right -3;
            path->points[2].y = LINKS_TILE_Y(i) + (LINKS_TILE_HEIGHT / 2);
     if (path->points[2].y >=  full_rect.bottom )
                path->points[2].y = full_rect.bottom -1;
            path->points[3].x = LINKS_TILE_X(tile2.col) + (LINKS_TILE_WIDTH / 2);
            if(path->points[3].x <= 0)
                path->points[3].x = 1;
            path->points[3].y = LINKS_TILE_Y(org_tile1_row) + LINKS_TILE_HEIGHT;
            return TRUE;
        }
        i++;
    }
    return FALSE;
}

/*****************************************************************************/
                                      //  FUNCTION:     GetTopBottomConnectPath
                                      //    Description :   get a connected path between two tile at same column
                                      //    Global resource dependence :   s_links_board
                                      //  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetTopBottomConnectPath(
                                      LINKS_TILE_T   tile1,
                                      LINKS_TILE_T   tile2,
                                      LINKS_PATH_T*  path
                                      )
{
    uint16 i = 0;
    uint8 org_tile1_col = tile1.col;

    GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect() ;
    // uint16 full_width = full_rect.right - full_rect.left ;
    // uint16 fuu_height = full_rect.bottom - full_rect.top ;
    
    //SCI_ASSERT(tile1.col == tile2.col);
    //SCI_ASSERT(tile1.row < tile2.row);
    if((tile1.col != tile2.col) || (tile1.row >= tile2.row))
    {
        //SCI_TRACE_LOW:"[mmigame_link_func.c]:GetTopBottomConnectPath error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_LINK_FUNC_2557_112_2_18_2_21_29_20,(uint8*)"");
        return FALSE;
    }
    
    //搜寻两个图片可否直接连接
    if (IsCanVerticalDirectConnect(tile1, tile2))
    {
        path->point_number = 2;
        //给path->points赋值为这两个图片
        path->points[0].x = LINKS_TILE_X(tile1.col) + (LINKS_TILE_WIDTH / 2);
        if(path->points[0].x <= 0)
            path->points[0].x = 1;
        path->points[0].y = LINKS_TILE_Y(tile1.row) + (LINKS_TILE_HEIGHT / 2);
        path->points[1].x = LINKS_TILE_X(tile2.col) + (LINKS_TILE_WIDTH / 2);
        if(path->points[1].x <= 0)
            path->points[1].x = 1;
        path->points[1].y = LINKS_TILE_Y(tile2.row) + (LINKS_TILE_HEIGHT / 2);
        return TRUE;
    }
    //搜寻两个图片均向左延伸是否可连接
    i = org_tile1_col - 1;
    while (LINKS_TILE_BLANK == s_links_board[tile1.row][i].id
        && LINKS_TILE_BLANK == s_links_board[tile2.row][i].id)
    {
        tile1.col = i;
        tile2.col = i;
        if (IsCanVerticalDirectConnect(tile1, tile2))
        {
            path->point_number = 4;
            //给path->points赋值
            path->points[0].x = LINKS_TILE_X(org_tile1_col);
            if(path->points[0].x <= 0)
                path->points[0].x = 1;
            path->points[0].y = LINKS_TILE_Y(tile1.row) + (LINKS_TILE_HEIGHT / 2);
            path->points[1].x = LINKS_TILE_X(i) + (LINKS_TILE_WIDTH / 2);
            if(path->points[1].x <= 0)
                path->points[1].x = 1;
            if (path->points[1].x >= full_rect.right )
                path->points[1].x = full_rect.right -3;
            path->points[1].y = LINKS_TILE_Y(tile1.row) + (LINKS_TILE_HEIGHT / 2);
            path->points[2].x = LINKS_TILE_X(i) + (LINKS_TILE_WIDTH / 2);
            if(path->points[2].x <= 0)
                path->points[2].x = 1;
            if (path->points[2].x >= full_rect.right )
                path->points[2].x = full_rect.right -3;
            path->points[2].y = LINKS_TILE_Y(tile2.row) + (LINKS_TILE_HEIGHT / 2);
            path->points[3].x = LINKS_TILE_X(org_tile1_col);
            if(path->points[3].x <= 0)
                path->points[3].x = 1;
            path->points[3].y = LINKS_TILE_Y(tile2.row) + (LINKS_TILE_HEIGHT / 2);
            return TRUE;
        }
        i--;
    }
    //搜寻两个图片均向右延伸是否可连接
    i = org_tile1_col + 1;
    while (i < LINKS_BOARD_COLS && LINKS_TILE_BLANK == s_links_board[tile1.row][i].id
        && LINKS_TILE_BLANK == s_links_board[tile2.row][i].id)
    {
        tile1.col = i;
        tile2.col = i;
        if (IsCanVerticalDirectConnect(tile1, tile2))
        {
            path->point_number = 4;
            //给path->points赋值
            path->points[0].x = LINKS_TILE_X(org_tile1_col) + LINKS_TILE_WIDTH - g_LINKS_TILES_RECT_LEFT;//CR251423
            if(path->points[0].x <= 0)
                path->points[0].x = 1;
            path->points[0].y = LINKS_TILE_Y(tile1.row) + (LINKS_TILE_HEIGHT / 2);
            path->points[1].x = LINKS_TILE_X(i) + (LINKS_TILE_WIDTH / 2);
            if(path->points[1].x <= 0)
                path->points[1].x = 1;
            if (path->points[1].x >= full_rect.right )
                path->points[1].x = full_rect.right -3;
            path->points[1].y = LINKS_TILE_Y(tile1.row) + (LINKS_TILE_HEIGHT / 2);
            path->points[2].x = LINKS_TILE_X(i) + (LINKS_TILE_WIDTH / 2);
            if(path->points[2].x <= 0)
                path->points[2].x = 1;
            if (path->points[2].x >= full_rect.right )
                path->points[2].x = full_rect.right -3;
            path->points[2].y = LINKS_TILE_Y(tile2.row) + (LINKS_TILE_HEIGHT / 2);
            path->points[3].x = LINKS_TILE_X(org_tile1_col) + LINKS_TILE_WIDTH - g_LINKS_TILES_RECT_LEFT;//CR251423
            if(path->points[3].x <= 0)
                path->points[3].x = 1;
            path->points[3].y = LINKS_TILE_Y(tile2.row) + (LINKS_TILE_HEIGHT / 2);
            return TRUE;
        }
        i++;
    }
    return FALSE;
}

/*****************************************************************************/
                                      //  FUNCTION:     GetLeftBottomRightTopConnectPath
                                      //    Description :   get a connected path between two tile one at left bottom
                                      //                  and one at right top
                                      //    Global resource dependence :   s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetLeftBottomRightTopConnectPath(
                                               LINKS_TILE_T   tile1,
                                               LINKS_TILE_T   tile2,
                                               LINKS_PATH_T*  path
                                               )
{
    int16 i = 0, j = 0;
    uint8 tile1_up_blank_num = 0;     //tile1上边可扩展点的数目
    uint8 tile1_down_blank_num = 0;   //tile1下边可扩展点的数目
    uint8 tile1_left_blank_num = 0;     //tile1左边可扩展点的数目
    uint8 tile1_right_blank_num = 0;    //tile1右边可扩展点的数目
    uint8 tile2_up_blank_num = 0;     //tile2上边可扩展点的数目
    uint8 tile2_down_blank_num = 0;   //tile2下边可扩展点的数目
    uint8 tile2_left_blank_num = 0;     //tile2左边可扩展点的数目
    uint8 tile2_right_blank_num = 0;    //tile2右边可扩展点的数目
    LINKS_TILE_T moveable_tile1, moveable_tile2;    //判断中使用的参数，位置可变

    GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect() ;
    // uint16 full_width = full_rect.right - full_rect.left ;
    // uint16 fuu_height = full_rect.bottom - full_rect.top ;
    
    SCI_MEMCPY(&moveable_tile1, &tile1, sizeof(LINKS_TILE_T));
    SCI_MEMCPY(&moveable_tile2, &tile2, sizeof(LINKS_TILE_T));
    
    //计算tile1和tile2各边可扩展点的数目
    //tile1向上扩展
    i = tile1.row - 1;
    while (i >= 0 && LINKS_TILE_BLANK == s_links_board[i][tile1.col].id)
    {
        i--;
        tile1_up_blank_num++;
    }
    //tile1向下扩展
    i = tile1.row + 1;
    while (i < LINKS_BOARD_ROWS && LINKS_TILE_BLANK == s_links_board[i][tile1.col].id)
    {
        i++;
        tile1_down_blank_num++;
    }
    //tile1向左扩展
    j = tile1.col - 1;
    while (j >= 0 && LINKS_TILE_BLANK == s_links_board[tile1.row][j].id)
    {
        j--;
        tile1_left_blank_num++;
    }
    //tile1向右扩展
    j = tile1.col + 1;
    while (j < LINKS_BOARD_COLS && LINKS_TILE_BLANK == s_links_board[tile1.row][j].id)
    {
        j++;
        tile1_right_blank_num++;
    }
    //tile2向上扩展
    i = tile2.row - 1;
    while (i >= 0 && LINKS_TILE_BLANK == s_links_board[i][tile2.col].id)
    {
        i--;
        tile2_up_blank_num++;
    }
    //tile2向下扩展
    i = tile2.row + 1;
    while (i < LINKS_BOARD_ROWS && LINKS_TILE_BLANK == s_links_board[i][tile2.col].id)
    {
        i++;
        tile2_down_blank_num++;
    }
    //tile2向左扩展
    j = tile2.col - 1;
    while (j >= 0 && LINKS_TILE_BLANK == s_links_board[tile2.row][j].id)
    {
        j--;
        tile2_left_blank_num++;
    }
    //tile2向右扩展
    j = tile2.col + 1;
    while (j < LINKS_BOARD_COLS && LINKS_TILE_BLANK == s_links_board[tile2.row][j].id)
    {
        j++;
        tile2_right_blank_num++;
    }
    
    //下面按照各种可能的情况，对不同扩展点的组合进行检查，看是否存在连接
    //tile1上——tile2左 直接检查
    if (tile1.row - tile1_up_blank_num <= tile2.row
        && tile2.col - tile2_left_blank_num <= tile1.col)
    {
        path->point_number = 3;
        //给path->points赋值
        path->points[0].x = LINKS_TILE_X(tile1.col) + (LINKS_TILE_WIDTH / 2);
        if(path->points[0].x <= 0)
            path->points[0].x = 1;
        path->points[0].y = LINKS_TILE_Y(tile1.row);
        path->points[1].x = LINKS_TILE_X(tile1.col) + (LINKS_TILE_WIDTH / 2);
        if(path->points[1].x <= 0)
            path->points[1].x = 1;
        if (path->points[1].x >= full_rect.right )
            path->points[1].x = full_rect.right -3;
        path->points[1].y = LINKS_TILE_Y(tile2.row) + (LINKS_TILE_HEIGHT / 2);
        path->points[2].x = LINKS_TILE_X(tile2.col);
        if(path->points[2].x <= 0)
            path->points[2].x = 1;
        if (path->points[2].x >= full_rect.right )
            path->points[2].x = full_rect.right -3;
        path->points[2].y = LINKS_TILE_Y(tile2.row) + (LINKS_TILE_HEIGHT / 2);
        return TRUE;
    }
    //tile1右——tile2下 直接检查
    if (tile1.col + tile1_right_blank_num >= tile2.col 
        && tile2.row + tile2_down_blank_num >= tile1.row)
    {
        path->point_number = 3;
        //给path->points赋值
        path->points[0].x = LINKS_TILE_X(tile1.col) + LINKS_TILE_WIDTH;
        if(path->points[0].x <= 0)
            path->points[0].x = 1;
        path->points[0].y = LINKS_TILE_Y(tile1.row) + (LINKS_TILE_HEIGHT / 2);
        path->points[1].x = LINKS_TILE_X(tile2.col) + (LINKS_TILE_WIDTH / 2);
        if(path->points[1].x <= 0)
            path->points[1].x = 1;
        if (path->points[1].x >= full_rect.right )
            path->points[1].x = full_rect.right -3;
        path->points[1].y = LINKS_TILE_Y(tile1.row) + (LINKS_TILE_HEIGHT / 2);
        path->points[2].x = LINKS_TILE_X(tile2.col) + (LINKS_TILE_WIDTH / 2);
        if(path->points[2].x <= 0)
            path->points[2].x = 1;
        if (path->points[2].x >= full_rect.right )
            path->points[2].x = full_rect.right -3;
        path->points[2].y = LINKS_TILE_Y(tile2.row) + LINKS_TILE_HEIGHT;
        return TRUE;
    }
    //tile1上——tile2上 检查水平直接连接
    for (i = 1; i <= tile1_up_blank_num; i++)
    {
        for (j = 1; j <= tile2_up_blank_num; j++)
        {
            moveable_tile1.row = tile1.row - i;
            moveable_tile1.col = tile1.col;
            moveable_tile2.row = tile2.row - j;
            moveable_tile2.col = tile2.col;
            if (IsCanHorizontalDirectConnect(moveable_tile1, moveable_tile2))
            {
                path->point_number = 4;
                //给path->points赋值
                path->points[0].x = LINKS_TILE_X(tile1.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[0].x <= 0)
                    path->points[0].x = 1;
                path->points[0].y = LINKS_TILE_Y(tile1.row);
                path->points[1].x = LINKS_TILE_X(moveable_tile1.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[1].x <= 0)
                    path->points[1].x = 1;
                if (path->points[1].x >= full_rect.right )
                    path->points[1].x = full_rect.right -3;
                path->points[1].y = LINKS_TILE_Y(moveable_tile1.row) + (LINKS_TILE_HEIGHT / 2);
                path->points[2].x = LINKS_TILE_X(moveable_tile2.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[2].x <= 0)
                    path->points[2].x = 1;
                if (path->points[2].x >= full_rect.right )
                    path->points[2].x = full_rect.right -3;
                path->points[2].y = LINKS_TILE_Y(moveable_tile2.row) + (LINKS_TILE_HEIGHT / 2);
                path->points[3].x = LINKS_TILE_X(tile2.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[3].x <= 0)
                    path->points[3].x = 1;
                path->points[3].y = LINKS_TILE_Y(tile2.row);
                return TRUE;
            }
        }
    }
    //tile1上——tile2下 检查水平直接连接
    for (i = 1; i <= tile1_up_blank_num; i++)
    {
        for (j = 1; j <= tile2_down_blank_num; j++)
        {
            moveable_tile1.row = tile1.row - i;
            moveable_tile1.col = tile1.col;
            moveable_tile2.row = tile2.row + j;
            moveable_tile2.col = tile2.col;
            if (IsCanHorizontalDirectConnect(moveable_tile1, moveable_tile2))
            {
                path->point_number = 4;
                //给path->points赋值
                path->points[0].x = LINKS_TILE_X(tile1.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[0].x <= 0)
                    path->points[0].x = 1;
                path->points[0].y = LINKS_TILE_Y(tile1.row);
                path->points[1].x = LINKS_TILE_X(moveable_tile1.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[1].x <= 0)
                    path->points[1].x = 1;
                if (path->points[1].x >= full_rect.right )
                    path->points[1].x = full_rect.right -3;
                path->points[1].y = LINKS_TILE_Y(moveable_tile1.row) + (LINKS_TILE_HEIGHT / 2);
                path->points[2].x = LINKS_TILE_X(moveable_tile2.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[2].x <= 0)
                    path->points[2].x = 1;
                if (path->points[2].x >= full_rect.right )
                    path->points[2].x = full_rect.right -3;
                path->points[2].y = LINKS_TILE_Y(moveable_tile2.row) + (LINKS_TILE_HEIGHT / 2);
                path->points[3].x = LINKS_TILE_X(tile2.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[3].x <= 0)
                    path->points[3].x = 1;
                path->points[3].y = LINKS_TILE_Y(tile2.row) + LINKS_TILE_HEIGHT;
                return TRUE;
            }
        }
    }
    //tile1下——tile2下 检查水平直接连接
    for (i = 1; i <= tile1_down_blank_num; i++)
    {
        for (j = 1; j <= tile2_down_blank_num; j++)
        {
            moveable_tile1.row = tile1.row + i;
            moveable_tile1.col = tile1.col;
            moveable_tile2.row = tile2.row + j;
            moveable_tile2.col = tile2.col;
            if (IsCanHorizontalDirectConnect(moveable_tile1, moveable_tile2))
            {
                path->point_number = 4;
                //给path->points赋值
                path->points[0].x = LINKS_TILE_X(tile1.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[0].x <= 0)
                    path->points[0].x = 1;
                path->points[0].y = LINKS_TILE_Y(tile1.row) + LINKS_TILE_HEIGHT;
                path->points[1].x = LINKS_TILE_X(moveable_tile1.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[1].x <= 0)
                    path->points[1].x = 1;
                if (path->points[1].x >= full_rect.right )
                    path->points[1].x = full_rect.right -3;
                path->points[1].y = LINKS_TILE_Y(moveable_tile1.row) + (LINKS_TILE_HEIGHT / 2);
                path->points[2].x = LINKS_TILE_X(moveable_tile2.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[2].x <= 0)
                    path->points[2].x = 1;
                if (path->points[2].x >= full_rect.right )
                    path->points[2].x = full_rect.right -3;
                path->points[2].y = LINKS_TILE_Y(moveable_tile2.row) + (LINKS_TILE_HEIGHT / 2);
                path->points[3].x = LINKS_TILE_X(tile2.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[3].x <= 0)
                    path->points[3].x = 1;
                path->points[3].y = LINKS_TILE_Y(tile2.row) + LINKS_TILE_HEIGHT;
                return TRUE;
            }
        }
    }
    //tile1左——tile2左 检查垂直直接连接
    for (i = 1; i <= tile1_left_blank_num; i++)
    {
        for (j = 1; j <= tile2_left_blank_num; j++)
        {
            moveable_tile1.row = tile1.row;
            moveable_tile1.col = tile1.col - i;
            moveable_tile2.row = tile2.row;
            moveable_tile2.col = tile2.col - j;
            if (IsCanVerticalDirectConnect(moveable_tile1, moveable_tile2))
            {
                path->point_number = 4;
                //给path->points赋值
                path->points[0].x = LINKS_TILE_X(tile1.col);
                if(path->points[0].x <= 0)
                    path->points[0].x = 1;
                path->points[0].y = LINKS_TILE_Y(tile1.row) + (LINKS_TILE_HEIGHT / 2);
                path->points[1].x = LINKS_TILE_X(moveable_tile1.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[1].x <= 0)
                    path->points[1].x = 1;
                if (path->points[1].x >= full_rect.right )
                    path->points[1].x = full_rect.right -3;
                path->points[1].y = LINKS_TILE_Y(moveable_tile1.row) + (LINKS_TILE_HEIGHT / 2);
                path->points[2].x = LINKS_TILE_X(moveable_tile2.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[2].x <= 0)
                    path->points[2].x = 1;
                if (path->points[2].x >= full_rect.right )
                    path->points[2].x = full_rect.right -3;
                path->points[2].y = LINKS_TILE_Y(moveable_tile2.row) + (LINKS_TILE_HEIGHT / 2);
                path->points[3].x = LINKS_TILE_X(tile2.col);
                if(path->points[3].x <= 0)
                    path->points[3].x = 1;
                path->points[3].y = LINKS_TILE_Y(tile2.row) + (LINKS_TILE_HEIGHT / 2);
                return TRUE;
            }
        }
    }
    //tile1右——tile2左 检查垂直直接连接
    for (i = 1; i <= tile1_right_blank_num; i++)
    {
        for (j = 1; j <= tile2_left_blank_num; j++)
        {
            moveable_tile1.row = tile1.row;
            moveable_tile1.col = tile1.col + i;
            moveable_tile2.row = tile2.row;
            moveable_tile2.col = tile2.col - j;
            if (IsCanVerticalDirectConnect(moveable_tile1, moveable_tile2))
            {
                path->point_number = 4;
                //给path->points赋值
                path->points[0].x = LINKS_TILE_X(tile1.col) + LINKS_TILE_WIDTH;
                if(path->points[0].x <= 0)
                    path->points[0].x = 1;
                path->points[0].y = LINKS_TILE_Y(tile1.row) + (LINKS_TILE_HEIGHT / 2);
                path->points[1].x = LINKS_TILE_X(moveable_tile1.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[1].x <= 0)
                    path->points[1].x = 1;
                if (path->points[1].x >= full_rect.right )
                    path->points[1].x = full_rect.right -3;
                path->points[1].y = LINKS_TILE_Y(moveable_tile1.row) + (LINKS_TILE_HEIGHT / 2);
                path->points[2].x = LINKS_TILE_X(moveable_tile2.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[2].x <= 0)
                    path->points[2].x = 1;
                if (path->points[2].x >= full_rect.right )
                    path->points[2].x = full_rect.right -3;
                path->points[2].y = LINKS_TILE_Y(moveable_tile2.row) + (LINKS_TILE_HEIGHT / 2);
                path->points[3].x = LINKS_TILE_X(tile2.col);
                if(path->points[3].x <= 0)
                    path->points[3].x = 1;
                path->points[3].y = LINKS_TILE_Y(tile2.row) + (LINKS_TILE_HEIGHT / 2);
                return TRUE;
            }
        }
    }
    //tile1右——tile2右 检查垂直直接连接
    for (i = 1; i <= tile1_right_blank_num; i++)
    {
        for (j = 1; j <= tile2_right_blank_num; j++)
        {
            moveable_tile1.row = tile1.row;
            moveable_tile1.col = tile1.col + i;
            moveable_tile2.row = tile2.row;
            moveable_tile2.col = tile2.col + j;
            if (IsCanVerticalDirectConnect(moveable_tile1, moveable_tile2))
            {
                path->point_number = 4;
                //给path->points赋值
                path->points[0].x = LINKS_TILE_X(tile1.col) + LINKS_TILE_WIDTH;
                if(path->points[0].x <= 0)
                    path->points[0].x = 1;
                path->points[0].y = LINKS_TILE_Y(tile1.row) + (LINKS_TILE_HEIGHT / 2);
                path->points[1].x = LINKS_TILE_X(moveable_tile1.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[1].x <= 0)
                    path->points[1].x = 1;
                if (path->points[1].x >= full_rect.right )
                    path->points[1].x = full_rect.right -3;
                path->points[1].y = LINKS_TILE_Y(moveable_tile1.row) + (LINKS_TILE_HEIGHT / 2);
                path->points[2].x = LINKS_TILE_X(moveable_tile2.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[2].x <= 0)
                    path->points[2].x = 1;
                if (path->points[2].x >= full_rect.right )
                    path->points[2].x = full_rect.right -3;
                path->points[2].y = LINKS_TILE_Y(moveable_tile2.row) + (LINKS_TILE_HEIGHT / 2);
                path->points[3].x = LINKS_TILE_X(tile2.col) + LINKS_TILE_WIDTH;
                if(path->points[3].x <= 0)
                    path->points[3].x = 1;
                path->points[3].y = LINKS_TILE_Y(tile2.row) + (LINKS_TILE_HEIGHT / 2);
                return TRUE;
            }
        }
    }
    
    return FALSE;
}

/*****************************************************************************/
//  FUNCTION:     GetLeftTopRightBottomConnectPath
//  Description :   get a connected path between two tile one at left top
//                  and one at right bottom
//  Global resource dependence :   s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetLeftTopRightBottomConnectPath(
                                               LINKS_TILE_T   tile1,
                                               LINKS_TILE_T   tile2,
                                               LINKS_PATH_T*  path
                                               )
{
    uint8 i = 0, j = 0;
    uint8 tile1_up_blank_num = 0;     //tile1上边可扩展点的数目
    uint8 tile1_down_blank_num = 0;   //tile1下边可扩展点的数目
    uint8 tile1_left_blank_num = 0;     //tile1左边可扩展点的数目
    uint8 tile1_right_blank_num = 0;    //tile1右边可扩展点的数目
    uint8 tile2_up_blank_num = 0;     //tile2上边可扩展点的数目
    uint8 tile2_down_blank_num = 0;   //tile2下边可扩展点的数目
    uint8 tile2_left_blank_num = 0;     //tile2左边可扩展点的数目
    uint8 tile2_right_blank_num = 0;    //tile2右边可扩展点的数目
    LINKS_TILE_T moveable_tile1, moveable_tile2;    //判断中使用的参数，位置可变

   GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect() ;
    // uint16 full_width = full_rect.right - full_rect.left ;
    // uint16 fuu_height = full_rect.bottom - full_rect.top ;
    
    SCI_MEMCPY(&moveable_tile1, &tile1, sizeof(LINKS_TILE_T));
    SCI_MEMCPY(&moveable_tile2, &tile2, sizeof(LINKS_TILE_T));
    
    //计算tile1和tile2各边可扩展点的数目
    //tile1向上扩展
    i = tile1.row - 1;
    while (LINKS_TILE_BLANK == s_links_board[i][tile1.col].id)
    {
        i--;
        tile1_up_blank_num++;
    }
    //tile1向下扩展
    i = tile1.row + 1;
    while (i < LINKS_BOARD_ROWS && LINKS_TILE_BLANK == s_links_board[i][tile1.col].id)
    {
        i++;
        tile1_down_blank_num++;
    }
    //tile1向左扩展
    j = tile1.col - 1;
    while (LINKS_TILE_BLANK == s_links_board[tile1.row][j].id)
    {
        j--;
        tile1_left_blank_num++;
    }
    //tile1向右扩展
    j = tile1.col + 1;
    while (j < LINKS_BOARD_COLS && LINKS_TILE_BLANK == s_links_board[tile1.row][j].id)
    {
        j++;
        tile1_right_blank_num++;
    }
    //tile2向上扩展
    i = tile2.row - 1;
    while (LINKS_TILE_BLANK == s_links_board[i][tile2.col].id)
    {
        i--;
        tile2_up_blank_num++;
    }
    //tile2向下扩展
    i = tile2.row + 1;
    while (i < LINKS_BOARD_ROWS && LINKS_TILE_BLANK == s_links_board[i][tile2.col].id)
    {
        i++;
        tile2_down_blank_num++;
    }
    //tile2向左扩展
    j = tile2.col - 1;
    while (LINKS_TILE_BLANK == s_links_board[tile2.row][j].id)
    {
        j--;
        tile2_left_blank_num++;
    }
    //tile2向右扩展
    j = tile2.col + 1;
    while (j < LINKS_BOARD_COLS && LINKS_TILE_BLANK == s_links_board[tile2.row][j].id)
    {
        j++;
        tile2_right_blank_num++;
    }
    
    //下面按照各种可能的情况，对不同扩展点的组合进行检查，看是否存在连接
    //tile1下--tile2左 直接检查
    if (tile1.row + tile1_down_blank_num >= tile2.row
        && tile2.col - tile2_left_blank_num <= tile1.col)
    {
        path->point_number = 3;
        //给path->points赋值
        path->points[0].x = LINKS_TILE_X(tile1.col) + (LINKS_TILE_WIDTH / 2);
        if(path->points[0].x <= 0)
            path->points[0].x = 1;
        path->points[0].y = LINKS_TILE_Y(tile1.row) + LINKS_TILE_HEIGHT;
        if(path->points[0].y < 0)
            path->points[0].y = 0;
        path->points[1].x = LINKS_TILE_X(tile1.col) + (LINKS_TILE_WIDTH / 2);
        if(path->points[1].x <= 0)
            path->points[1].x = 1;
        if (path->points[1].x >= full_rect.right )
            path->points[1].x = full_rect.right -3;
        path->points[1].y = LINKS_TILE_Y(tile2.row) + (LINKS_TILE_HEIGHT / 2);
        if(path->points[1].y < 0)
            path->points[1].y = 0;
        path->points[2].x = LINKS_TILE_X(tile2.col);
        if(path->points[2].x <= 0)
            path->points[2].x = 1;
        if (path->points[2].x >= full_rect.right )
            path->points[2].x = full_rect.right -3;
        path->points[2].y = LINKS_TILE_Y(tile2.row) + (LINKS_TILE_HEIGHT / 2);
        if(path->points[2].y < 0)
            path->points[2].y = 0;
        return TRUE;
    }
    //tile1右--tile2上 直接检查
    if (tile1.col + tile1_right_blank_num >= tile2.col 
        && tile2.row - tile2_up_blank_num <= tile1.row)
    {
        path->point_number = 3;
        //给path->points赋值
        path->points[0].x = LINKS_TILE_X(tile1.col) + LINKS_TILE_WIDTH;
        if(path->points[0].x <= 0)
            path->points[0].x = 1;
        path->points[0].y = LINKS_TILE_Y(tile1.row) + (LINKS_TILE_HEIGHT / 2);
        if(path->points[0].y < 0)
            path->points[0].y = 0;
        path->points[1].x = LINKS_TILE_X(tile2.col) + (LINKS_TILE_WIDTH / 2);
        if(path->points[1].x <= 0)
            path->points[1].x = 1;
        if (path->points[1].x >= full_rect.right )
            path->points[1].x = full_rect.right -3;
        path->points[1].y = LINKS_TILE_Y(tile1.row) + (LINKS_TILE_HEIGHT / 2);
        if(path->points[1].y < 0)
            path->points[1].y = 0;
        path->points[2].x = LINKS_TILE_X(tile2.col) + (LINKS_TILE_WIDTH / 2);
        if(path->points[2].x <= 0)
            path->points[2].x = 1;
        if (path->points[2].x >= full_rect.right )
            path->points[2].x = full_rect.right -3;
        path->points[2].y = LINKS_TILE_Y(tile2.row);
        if(path->points[2].y < 0)
            path->points[2].y = 0;

        return TRUE;
    }
    //tile1上--tile2上 检查水平直接连接
    for (i = 1; i <= tile1_up_blank_num; i++)
    {
        for (j = 1; j <= tile2_up_blank_num; j++)
        {
            moveable_tile1.row = tile1.row - i;
            moveable_tile1.col = tile1.col;
            moveable_tile2.row = tile2.row - j;
            moveable_tile2.col = tile2.col;
            if (IsCanHorizontalDirectConnect(moveable_tile1, moveable_tile2))
            {
                path->point_number = 4;
                //给path->points赋值
                path->points[0].x = LINKS_TILE_X(tile1.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[0].x <= 0)
                    path->points[0].x = 1;
                path->points[0].y = LINKS_TILE_Y(tile1.row);
                if(path->points[0].y < 0)
                    path->points[0].y = 0;
                path->points[1].x = LINKS_TILE_X(moveable_tile1.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[1].x <= 0)
                    path->points[1].x = 1;
                if (path->points[1].x >= full_rect.right )
                    path->points[1].x = full_rect.right -3;
                path->points[1].y = LINKS_TILE_Y(moveable_tile1.row) + (LINKS_TILE_HEIGHT / 2);
                if(path->points[1].y < 0)
                    path->points[1].y = 0;                
                path->points[2].x = LINKS_TILE_X(moveable_tile2.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[2].x <= 0)
                    path->points[2].x = 1;
                if (path->points[2].x >= full_rect.right )
                    path->points[2].x = full_rect.right -3;
                path->points[2].y = LINKS_TILE_Y(moveable_tile2.row) + (LINKS_TILE_HEIGHT / 2);
                if(path->points[2].y < 0)
                    path->points[2].y = 0;                
                path->points[3].x = LINKS_TILE_X(tile2.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[3].x <= 0)
                    path->points[3].x = 1;
                path->points[3].y = LINKS_TILE_Y(tile2.row);
                if(path->points[3].y < 0)
                    path->points[3].y = 0;                
                return TRUE;
            }
        }
    }
    //tile1下--tile2上 检查水平直接连接
    for (i = 1; i <= tile1_down_blank_num; i++)
    {
        for (j = 1; j <= tile2_up_blank_num; j++)
        {
            moveable_tile1.row = tile1.row + i;
            moveable_tile1.col = tile1.col;
            moveable_tile2.row = tile2.row - j;
            moveable_tile2.col = tile2.col;
            if (IsCanHorizontalDirectConnect(moveable_tile1, moveable_tile2))
            {
                path->point_number = 4;
                //给path->points赋值
                path->points[0].x = LINKS_TILE_X(tile1.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[0].x <= 0)
                    path->points[0].x = 1;
                path->points[0].y = LINKS_TILE_Y(tile1.row) + LINKS_TILE_HEIGHT;
                if(path->points[0].y < 0)
                    path->points[0].y = 0;
                path->points[1].x = LINKS_TILE_X(moveable_tile1.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[1].x <= 0)
                    path->points[1].x = 1;
                if (path->points[1].x >= full_rect.right )
                    path->points[1].x = full_rect.right -3;
                path->points[1].y = LINKS_TILE_Y(moveable_tile1.row) + (LINKS_TILE_HEIGHT / 2);
                if(path->points[1].y < 0)
                    path->points[1].y = 0;
                path->points[2].x = LINKS_TILE_X(moveable_tile2.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[2].x <= 0)
                    path->points[2].x = 1;
                if (path->points[2].x >= full_rect.right )
                    path->points[2].x = full_rect.right -3;
                path->points[2].y = LINKS_TILE_Y(moveable_tile2.row) + (LINKS_TILE_HEIGHT / 2);
                if(path->points[2].y < 0)
                    path->points[2].y = 0;
                path->points[3].x = LINKS_TILE_X(tile2.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[3].x <= 0)
                    path->points[3].x = 1;
                path->points[3].y = LINKS_TILE_Y(tile2.row);
                if(path->points[3].y < 0)
                    path->points[3].y = 0;

                return TRUE;
            }
        }
    }
    //tile1下--tile2下 检查水平直接连接
    for (i = 1; i <= tile1_down_blank_num; i++)
    {
        for (j = 1; j <= tile2_down_blank_num; j++)
        {
            moveable_tile1.row = tile1.row + i;
            moveable_tile1.col = tile1.col;
            moveable_tile2.row = tile2.row + j;
            moveable_tile2.col = tile2.col;
            if (IsCanHorizontalDirectConnect(moveable_tile1, moveable_tile2))
            {
                path->point_number = 4;
                //给path->points赋值
                path->points[0].x = LINKS_TILE_X(tile1.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[0].x <= 0)
                    path->points[0].x = 1;
                path->points[0].y = LINKS_TILE_Y(tile1.row) + LINKS_TILE_HEIGHT;
                if(path->points[0].y < 0)
                    path->points[0].y = 0;
                path->points[1].x = LINKS_TILE_X(moveable_tile1.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[1].x <= 0)
                    path->points[1].x = 1;
                if (path->points[1].x >= full_rect.right )
                    path->points[1].x = full_rect.right -3;
                path->points[1].y = LINKS_TILE_Y(moveable_tile1.row) + (LINKS_TILE_HEIGHT / 2);
                if(path->points[1].y < 0)
                    path->points[1].y = 0;
                path->points[2].x = LINKS_TILE_X(moveable_tile2.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[2].x <= 0)
                    path->points[2].x = 1;
                if (path->points[2].x >= full_rect.right )
                    path->points[2].x = full_rect.right -3;
                path->points[2].y = LINKS_TILE_Y(moveable_tile2.row) + (LINKS_TILE_HEIGHT / 2);
                if(path->points[2].y < 0)
                    path->points[2].y = 0;
                path->points[3].x = LINKS_TILE_X(tile2.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[3].x <= 0)
                    path->points[3].x = 1;
                path->points[3].y = LINKS_TILE_Y(tile2.row) + LINKS_TILE_HEIGHT;
                if(path->points[3].y < 0)
                    path->points[3].y = 0;
                return TRUE;
            }
        }
    }
    //tile1左--tile2左 检查垂直直接连接
    for (i = 1; i <= tile1_left_blank_num; i++)
    {
        for (j = 1; j <= tile2_left_blank_num; j++)
        {
            moveable_tile1.row = tile1.row;
            moveable_tile1.col = tile1.col - i;
            moveable_tile2.row = tile2.row;
            moveable_tile2.col = tile2.col - j;
            if (IsCanVerticalDirectConnect(moveable_tile1, moveable_tile2))
            {
                path->point_number = 4;
                //给path->points赋值
                path->points[0].x = LINKS_TILE_X(tile1.col);
                if(path->points[0].x <= 0)
                    path->points[0].x = 1;
                path->points[0].y = LINKS_TILE_Y(tile1.row) + (LINKS_TILE_HEIGHT / 2);
                if(path->points[0].y < 0)
                    path->points[0].y = 0;
                path->points[1].x = LINKS_TILE_X(moveable_tile1.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[1].x <= 0)
                    path->points[1].x = 1;
                if (path->points[1].x >= full_rect.right )
                    path->points[1].x = full_rect.right -3;
                path->points[1].y = LINKS_TILE_Y(moveable_tile1.row) + (LINKS_TILE_HEIGHT / 2);
                if(path->points[1].y < 0)
                    path->points[1].y = 0;
                path->points[2].x = LINKS_TILE_X(moveable_tile2.col) + (LINKS_TILE_WIDTH / 2);
                if(path->points[2].x <= 0)
                    path->points[2].x = 1;
                if (path->points[2].x >= full_rect.right )
                    path->points[2].x = full_rect.right -3;
                path->points[2].y = LINKS_TILE_Y(moveable_tile2.row) + (LINKS_TILE_HEIGHT / 2);
                if(path->points[2].y < 0)
                    path->points[2].y = 0;
                path->points[3].x = LINKS_TILE_X(tile2.col);
                if(path->points[3].x <= 0)
                    path->points[3].x = 1;
                path->points[3].y = LINKS_TILE_Y(tile2.row) + (LINKS_TILE_HEIGHT / 2);
                if(path->points[3].y < 0)
                    path->points[3].y = 0;                
                return TRUE;
            }
        }
    }
    //tile1右--tile2左 检查垂直直接连接
    for (i = 1; i <= tile1_right_blank_num; i++)
    {
        for (j = 1; j <= tile2_left_blank_num; j++)
        {
            moveable_tile1.row = tile1.row;
            moveable_tile1.col = tile1.col + i;
            moveable_tile2.row = tile2.row;
            moveable_tile2.col = tile2.col - j;
            if (IsCanVerticalDirectConnect(moveable_tile1, moveable_tile2))
            {
                path->point_number = 4;
                //给path->points赋值
                path->points[0].x = (uint16)(LINKS_TILE_X(tile1.col) + LINKS_TILE_WIDTH);
                if(path->points[0].x <= 0)
                    path->points[0].x = 1;
                path->points[0].y = (uint16)(LINKS_TILE_Y(tile1.row) + (LINKS_TILE_HEIGHT / 2));
                if(path->points[0].y < 0)
                    path->points[0].y = 0;
                path->points[1].x = (uint16)(LINKS_TILE_X(moveable_tile1.col) + (LINKS_TILE_WIDTH / 2));
                if(path->points[1].x <= 0)
                    path->points[1].x = 1;
                if (path->points[1].x >= full_rect.right )
                    path->points[1].x = full_rect.right -3;
                path->points[1].y = (uint16)(LINKS_TILE_Y(moveable_tile1.row) + (LINKS_TILE_HEIGHT / 2));
                if(path->points[1].y < 0)
                    path->points[1].y = 0;
                path->points[2].x = (uint16)(LINKS_TILE_X(moveable_tile2.col) + (LINKS_TILE_WIDTH / 2));
                if(path->points[2].x <= 0)
                    path->points[2].x = 1;
                if (path->points[2].x >= full_rect.right )
                    path->points[2].x = full_rect.right -3;
                path->points[2].y = (uint16)(LINKS_TILE_Y(moveable_tile2.row) + (LINKS_TILE_HEIGHT / 2));
                if(path->points[2].y < 0)
                    path->points[2].y = 0;
                path->points[3].x = (uint16)(LINKS_TILE_X(tile2.col));
                if(path->points[3].x <= 0)
                    path->points[3].x = 1;
                path->points[3].y = (uint16)(LINKS_TILE_Y(tile2.row) + (LINKS_TILE_HEIGHT / 2));
                if(path->points[3].y < 0)
                    path->points[3].y = 0;
                return TRUE;
            }
        }
    }
    //tile1右--tile2右 检查垂直直接连接
    for (i = 1; i <= tile1_right_blank_num; i++)
    {
        for (j = 1; j <= tile2_right_blank_num; j++)
        {
            moveable_tile1.row = tile1.row;
            moveable_tile1.col = tile1.col + i;
            moveable_tile2.row = tile2.row;
            moveable_tile2.col = tile2.col + j;
            if (IsCanVerticalDirectConnect(moveable_tile1, moveable_tile2))
            {
                path->point_number = 4;
                //给path->points赋值
                path->points[0].x = (uint16)(LINKS_TILE_X(tile1.col) + LINKS_TILE_WIDTH);
                if(path->points[0].x <= 0)
                    path->points[0].x = 1;
                path->points[0].y = (uint16)(LINKS_TILE_Y(tile1.row) + (LINKS_TILE_HEIGHT / 2));
                if(path->points[0].y < 0)
                    path->points[0].y = 0;
                path->points[1].x = (uint16)(LINKS_TILE_X(moveable_tile1.col) + (LINKS_TILE_WIDTH / 2));
                if(path->points[1].x <= 0)
                    path->points[1].x = 1;
                if (path->points[1].x >= full_rect.right )
                    path->points[1].x = full_rect.right -3;
                path->points[1].y = (uint16)(LINKS_TILE_Y(moveable_tile1.row) + (LINKS_TILE_HEIGHT / 2));
                if(path->points[1].y < 0)
                    path->points[1].y = 0;
                path->points[2].x = (uint16)(LINKS_TILE_X(moveable_tile2.col) + (LINKS_TILE_WIDTH / 2));
                if(path->points[2].x <= 0)
                    path->points[2].x = 1;
                if (path->points[2].x >= full_rect.right )
                    path->points[2].x = full_rect.right -3;
                path->points[2].y = (uint16)(LINKS_TILE_Y(moveable_tile2.row) + (LINKS_TILE_HEIGHT / 2));
                if(path->points[2].y < 0)
                    path->points[2].y = 0;
                path->points[3].x = (uint16)(LINKS_TILE_X(tile2.col) + LINKS_TILE_WIDTH);
                if(path->points[3].x <= 0)
                    path->points[3].x = 1;
                path->points[3].y = (uint16)(LINKS_TILE_Y(tile2.row) + (LINKS_TILE_HEIGHT / 2));
                if(path->points[3].y < 0)
                    path->points[3].y = 0;
                return TRUE;
            }
        }
    }
    
    return FALSE;
}
/*****************************************************************************/
//  FUNCTION:     IsCanHorizontalDirectConnect
//  Description :   check if two tile can be direct connect horizontally
//  Global resource dependence :   s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsCanHorizontalDirectConnect(
                                           LINKS_TILE_T   tile1,
                                           LINKS_TILE_T   tile2
                                           )
{
    uint16 i = 0;
    uint16 j = 0;
    LINKS_TILE_T swap_tile = {0};
    
    if (tile1.row != tile2.row)
    {
        return FALSE;
    }
    
    //如果相邻
    if (abs(tile1.col - tile2.col) == 1)
    {
        return TRUE;
    }
    
    //保证tile1在tile2的左边
    if (tile1.col > tile2.col)
    {
        SCI_MEMCPY(&swap_tile, &tile1, sizeof(LINKS_TILE_T));
        SCI_MEMCPY(&tile1, &tile2, sizeof(LINKS_TILE_T));
        SCI_MEMCPY(&tile2, &swap_tile, sizeof(LINKS_TILE_T));
    }
    
    i = tile1.row;
    j = tile1.col + 1;
    
    while (j < tile2.col)
    {
        if (s_links_board[i][j].id != LINKS_TILE_BLANK)
        {
            return FALSE;
        }
        j++;
    }
    return TRUE;
    
}

/*****************************************************************************/
//  FUNCTION:     IsCanVerticalDirectConnect
//  Description :   check if two tile can be direct connect vertically
//  Global resource dependence :   s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN IsCanVerticalDirectConnect(
                                          LINKS_TILE_T   tile1,
                                          LINKS_TILE_T   tile2
                                          )
{
    uint16 i = 0;
    uint16 j = 0;
    LINKS_TILE_T swap_tile = {0};
    
    if (tile1.col != tile2.col)
    {
        return FALSE;
    }
    
    //如果相邻
    if (abs(tile1.row - tile2.row) == 1)
    {
        return TRUE;
    }
    
    //保证tile1在tile2的上边
    if (tile1.row > tile2.row)
    {
        SCI_MEMCPY(&swap_tile, &tile1, sizeof(LINKS_TILE_T));
        SCI_MEMCPY(&tile1, &tile2, sizeof(LINKS_TILE_T));
        SCI_MEMCPY(&tile2, &swap_tile, sizeof(LINKS_TILE_T));
    }
    
    i = tile1.row + 1;
    j = tile1.col;
    
    while (i < tile2.row)
    {
        if (s_links_board[i][j].id != LINKS_TILE_BLANK)
        {
            return FALSE;
        }
        i++;
    }
    return TRUE;
}
/*****************************************************************************/
//  FUNCTION:     OnPauseClicked
//  Description :   handle when pause button is clicked
//  Global resource dependence :   s_links_is_paused
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void OnPauseClicked(
                           MMI_WIN_ID_T win_id
                           )
{
    //暂停状态点击暂停按钮
    if (s_links_is_paused)
    {
        s_links_is_paused = !s_links_is_paused;
        if (0 == s_links_timer)  
        {
            s_links_timer = MMK_CreateTimer(1000, TRUE);
        }
        DrawAll(win_id);
    }
    //非暂停状态点击暂停按钮
    else
    {
        s_links_is_paused = !s_links_is_paused;
        if (s_links_timer >0 )
        {
            MMK_StopTimer(s_links_timer);
            s_links_timer = 0;
        }
        DrawPauseScreen(win_id);
    }
}

/*****************************************************************************/
//  FUNCTION:     OnTipClicked
//  Description :   handle when tip button is clicked
//  Global resource dependence :   s_links_tip_times, s_links_connected_pair
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void OnTipClicked(
                         MMI_WIN_ID_T   win_id
                         )
{
    LINKS_PATH_T   path = {0};//maryxiao cr230490 //when tip add line
    
    if (s_links_tip_times > 0 && !s_links_is_paused)
    {
        s_links_tip_times--;
        DrawTipString(win_id);
        DrawTile(s_links_connected_pair[0], LINKS_TILE_TIPED,win_id);
        DrawTile(s_links_connected_pair[1], LINKS_TILE_TIPED,win_id);
        GetConnectPath(s_links_connected_pair[0], s_links_connected_pair[1], &path);
                           
        DrawConnectLine(path);
    }
}

/*****************************************************************************/
//  FUNCTION:     OnWashCardClicked
//  Description :   handle when wash card button is clicked
//  Global resource dependence :   s_links_random_times, s_links_connected_pair
//                                 s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void OnWashCardClicked(
                              MMI_WIN_ID_T  win_id
                              )
{
    if (s_links_random_times > 0 && !s_links_is_paused)
    {
        do {
            RandomizeBoard();
        } while (!GetAConnectedTilePair(s_links_connected_pair));
        s_links_random_times--;
        DrawAll(win_id);
    }
}

/*****************************************************************************/
//  FUNCTION:     OnTileClicked
//  Description :   handle when tile is clicked
//  Global resource dependence :   s_links_connected_pair, s_links_board
//                  s_links_last_pressed_tile_ptr, s_links_is_paused
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void OnTileClicked(
                          MMI_WIN_ID_T win_id,
                          LINKS_TILE_T tile
                          )
{
    LINKS_PATH_T path = {0};
    GUI_RECT_T   update_rect = MMITHEME_GetFullScreenRect();
 
    //是否处于非暂停状态
    if (!s_links_is_paused)
    {
        //是否这是点击的第二个图片
        if (s_links_last_pressed_tile_ptr != NULL)
        {
            //是否和上次点击的是同一个图片
            if (s_links_last_pressed_tile_ptr->col == tile.col
                && s_links_last_pressed_tile_ptr->row == tile.row)
            {
                s_links_last_pressed_tile_ptr = NULL;
                DrawTile(tile, LINKS_TILE_NORMAL,win_id);
            }
            else
            {
                //和上次点击的图片可以相连
                if (GetConnectPath(*s_links_last_pressed_tile_ptr, tile, &path))
                {
                    s_links_board[tile.row][tile.col].id = LINKS_TILE_BLANK;
                    s_links_last_pressed_tile_ptr->id = LINKS_TILE_BLANK;
                    //SCI_ASSERT(s_links_board[s_links_last_pressed_tile_ptr->row]    /*assert verified*/
                      //  [s_links_last_pressed_tile_ptr->col].id == LINKS_TILE_BLANK);/* assert to do */

                    if(s_links_board[s_links_last_pressed_tile_ptr->row][s_links_last_pressed_tile_ptr->col].id != LINKS_TILE_BLANK)
                    {
                        return;
                    }
                    s_links_score += LINKS_SCORE_ELIMINATE_PAIR * (s_links_current_level + 1);                    
                    DrawConnectLine(path);      
                    GUILCD_InvalidateRect(GUI_MAIN_LCD_ID,update_rect,GUIREF_SCALE_BLOCK0);                   
                    SCI_Sleep(200);
                    //图片已经消完
                    if (IsAllEliminate())
                    {
                        s_links_score += LINKS_SCORE_EPISODE(s_links_time_left);
                        s_links_current_episode++;
                        //已通关
                        if (s_links_current_episode == LINKS_EPISODE_TOTAL)                         
                        {
                           
                            if (s_links_timer != 0)
                            {
                                MMK_StopTimer(s_links_timer);
                                s_links_timer = 0;
                            }
                            //显示通关画面，游戏退出
                            DrawGameFinishScreen(win_id);
                            s_links_game_state = LINKS_GAME_PASS;
                            if (s_game_over_timer != 0)
                            {
                                MMK_StopTimer(s_game_over_timer);
                                s_game_over_timer = 0;
                            }
                            s_game_over_timer = MMK_CreateTimer(MMI_2SECONDS,FALSE);
                            EndingGameLinks(win_id);
                            return ;
                        }
                        //未通关
                        else
                        {  
                            //                          s_links_random_times++;
                            //进入初始化下一关流程
                            InitStage();
                            DrawAll(win_id);
                            return;
                        }
                    }
                    //图片未消完
                    else
                    {
                        //不同关的图片移动
                        AutoMoveTile(s_links_board[tile.row][tile.col], *s_links_last_pressed_tile_ptr);
                        DrawAll(win_id);
                        //还存在可消除的图片对
                        if (GetAConnectedTilePair(s_links_connected_pair))
                        {
                            //这里无代码
                        }
                        else
                        {
                            if (s_links_random_times > 0)
                            {
                                //自动洗牌
                                OnWashCardClicked(win_id);
                                s_links_last_pressed_tile_ptr = NULL;
                            }
                            else
                            {
                                //游戏结束
                                DrawGameOverScreen(win_id);
                                s_links_game_state = LINKS_GAME_OVER;
                                if (s_game_over_timer != 0)
                                {
                                    MMK_StopTimer(s_game_over_timer);
                                    s_game_over_timer = 0;
                                }
                                if (s_links_timer >0 )
                                {
                                    MMK_StopTimer(s_links_timer);
                                    s_links_timer = 0;
                                }
                                s_game_over_timer = MMK_CreateTimer(MMI_2SECONDS,FALSE);
                                
                                // cleas the params
                                s_links_last_pressed_tile_ptr = NULL;
                                EndingGameLinks(win_id);
                            }
                            return;
                        }
                    }
                    
                }
                DrawTile(s_links_board[tile.row][tile.col], LINKS_TILE_NORMAL,win_id);
                DrawTile(*s_links_last_pressed_tile_ptr, LINKS_TILE_NORMAL,win_id);
                DrawScoreString(win_id);
                s_links_last_pressed_tile_ptr = NULL;
            }
        }
        else
        {
            s_links_last_pressed_tile_ptr = &(s_links_board[tile.row][tile.col]);
            DrawTile(tile, LINKS_TILE_PRESSED,win_id);
        }
    }
}

/*****************************************************************************/
//  FUNCTION:     SaveGameScore
//  Description :  for CR MS00181020
//  Global resource dependence : 
//  Author:       ying.xu
//  Note:   
/*****************************************************************************/
PUBLIC void SaveGameScore(void)
{
    uint16 high_score[3] = {0};
    uint16 nv_return = 0;   
    
    MMINV_READ(MMINV_GMLINK_HIGH_SCORE, ( int16*)high_score, nv_return);    
    if (s_links_score > high_score[1])  
    {   
        high_score[1] = s_links_score;  
        MMINV_WRITE(MMINV_GMLINK_HIGH_SCORE, ( int16*)high_score);  
    }   
}

#endif
