/****************************************************************************
** File Name:      mmigame_snake_func.c                            
** Author:                                                            
** Date:           
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to create windows of quintet
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
**
** 
****************************************************************************/
#define _MMIGAME_SNAKE_FUNC_C_  

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_SNAKE_SUPPORT
#include "window_parse.h"
#include "mmi_string.h"
#include "guilistbox.h"
#include "guimsgbox.h"
#include "mmidisplay_data.h" 
#include "guilcd.h"
#include "dal_img.h"
#include "tb_dal.h"
#include "mmi_nv.h"
#include "guiref_scale.h"
#include "mmk_timer.h"
#include "guilcd.h"
#include "guires.h"
#include "mmipub.h"
#include "mmigame_text.h"
#include "mmigame_snake_id.h"
#include "mmigame_snake_image.h"
#include "mmigame_snake_position.h" 
#include "mmigame_snake_text.h"   
#include "mmigame_snake_internal.h"
#include "mmigame_snake_nv.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#if defined MAINLCD_SIZE_128X160
#define MMIGMSNAKE_GAME_FONT  MMI_DEFAULT_NORMAL_FONT
// 暂停时提示的字体，需要醒目
#define MMIGMSNAKE_GAME_PAUSED_FONT  SONG_FONT_14
#elif defined MAINLCD_SIZE_176X220
#define MMIGMSNAKE_GAME_FONT  MMI_DEFAULT_NORMAL_FONT
// 暂停时提示的字体，需要醒目
#define MMIGMSNAKE_GAME_PAUSED_FONT  SONG_FONT_20
#elif defined MAINLCD_SIZE_220X176
#define MMIGMSNAKE_GAME_FONT  MMI_DEFAULT_NORMAL_FONT
// 暂停时提示的字体，需要醒目
#define MMIGMSNAKE_GAME_PAUSED_FONT  SONG_FONT_20
#elif defined MAINLCD_SIZE_240X320
#define MMIGMSNAKE_GAME_FONT  MMI_DEFAULT_NORMAL_FONT
// 暂停时提示的字体，需要醒目
#define MMIGMSNAKE_GAME_PAUSED_FONT  SONG_FONT_24
#elif defined MAINLCD_SIZE_320X240
#define MMIGMSNAKE_GAME_FONT  MMI_DEFAULT_NORMAL_FONT
// 暂停时提示的字体，需要醒目
#define MMIGMSNAKE_GAME_PAUSED_FONT  SONG_FONT_24
#elif defined MAINLCD_SIZE_240X400
#define MMIGMSNAKE_GAME_FONT  MMI_DEFAULT_NORMAL_FONT
// 暂停时提示的字体，需要醒目
#define MMIGMSNAKE_GAME_PAUSED_FONT  SONG_FONT_24
#elif defined MAINLCD_SIZE_320X480
#define MMIGMSNAKE_GAME_FONT  MMI_DEFAULT_NORMAL_FONT
// 暂停时提示的字体，需要醒目
#define MMIGMSNAKE_GAME_PAUSED_FONT  SONG_FONT_28
#else
// 目前只在240*320上实现，其它分辨率下需要调字体大小，暂时让其编译报错处理
#error
#endif
// 游戏中写分数和难度的字体颜色
#define COLOR_SNAKE_STRING             MMI_BLUE_COLOR//文字颜色
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/

typedef enum
{
    // 蛇头，这个确定蛇的方向
    GRIDTYPE_GMSNAKE_HEAD,  
    // 蛇身
    GRIDTYPE_GMSNAKE_BODY,
    // 蛇可以吃的方块
    GRIDTYPE_GMSNAKE_FLOG, 
    GAMESNAKE_GRIDTYPE_MAX
}GAMESNAKE_GRIDTYPE_E;


/**---------------------------------------------------------------------------*
**                       LOCAL DATA DECLARATION
**---------------------------------------------------------------------------*/
// 当前的分数
LOCAL  uint16       s_snake_currentscore = 0; 
// 用来保存蛇的位置snake_body_cache[0]为蛇的尾巴，s_snake_body_cache[s_snake_total-1]为蛇头
//LOCAL  uint16       s_snake_body_cache[SNAKE_XGRIDS*SNAKE_YGRIDS] = {0};
LOCAL  uint16       *s_snake_body_cache = PNULL;
// 用来保存屏幕中的格子各点是否是当前蛇所在的点
//LOCAL  BOOLEAN      s_snake_is_snakebody[SNAKE_XGRIDS*SNAKE_YGRIDS] ={0} ; 
LOCAL  BOOLEAN       *s_snake_is_snakebody = PNULL;
// 蛇可以吃的方块的位置
LOCAL  uint16       s_snake_flogindex = 0; 
// 总共蛇身长(格数)，吃掉一个方块长度加1.初始暂定4格
LOCAL  uint16       s_snake_total = 4;
// 格子图片的数组，不同类型格子用不同图片，根据GAMESNAKE_GRIDTYPE_E决定，一一对应
LOCAL  const  MMI_IMAGE_ID_T s_snake_gridimg[] = 
    {IMAGE_GAME_SNAKE_HEAD,IMAGE_GAME_SNAKE_BODY,IMAGE_GAME_SNAKE_FLOG};
// 蛇运动的方向
PUBLIC  MMI_MESSAGE_ID_E    s_snake_direction = MSG_KEYDOWN_RIGHT;   
// 按方向键时蛇改变的方向的暂存，该变量主要是为了快速按键的处理
PUBLIC  MMI_MESSAGE_ID_E    s_snake_direction_cache = 0;   
// 当前游戏的难度
PUBLIC  SNAKE_LEVEL_E       s_snake_current_level = SNAKE_LEVEL_EASY;      //当前的难度
// 计时器，蛇运动的时间，根据难度安排时间
PUBLIC uint8                s_snake_timer = 0;
// 游戏状态
PUBLIC SNAKE_GAME_STATE_E   s_snake_game_state = SNAKE_GAME_NULL;
// 难度等级的字符串数组和SNAKE_LEVEL_E一一对应
PUBLIC const MMI_TEXT_ID_T  s_snake_level_list[SNAKE_TOTAL_LEVEL] =
    {TXT_SNAKE_LEVEL_EASY,TXT_SNAKE_LEVEL_NORMAL,TXT_SNAKE_LEVEL_HARD,};

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARE                                                                           *
**--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                       function bodies  -- local
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//   FUNCTION:      MMIGMSNAKE_Init
//  Description :   
//
//  Global resource dependence : 
//   Author:        peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIGMSNAKE_Init(void)
{
    uint16 nv_return = 0;
    uint16 high_score = 0;
    uint8 level[2] = {0};
    //初始化nv
    MMINV_READ(MMINV_GMSNAKE_LEVER_SET, level, nv_return);
    if (MN_RETURN_SUCCESS != nv_return)
    {
        SCI_MEMSET(&level[0], 0, sizeof(level));
        MMINV_WRITE(MMINV_GMSNAKE_LEVER_SET, &level[0]);
    }
    MMINV_READ(MMINV_GMSNAKE_HIGH_SCORE, &high_score, nv_return);   
    if (MN_RETURN_SUCCESS != nv_return)
    {
        high_score = 0;
        MMINV_WRITE(MMINV_GMSNAKE_HIGH_SCORE, &high_score);
    }
    //level只用了level[1]，加一个防错处理
    if (level[1] >= SNAKE_TOTAL_LEVEL)
    {
        level[1] = SNAKE_LEVEL_EASY;
    }
    s_snake_current_level = (SNAKE_LEVEL_E)level[1];
    return TRUE;
    
}

/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_InitModule
//  Description :   
//
//  Global resource dependence : 
//   Author:        peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMSNAKE_InitModule(void)
{
    //初始化im menu
    MMIGMSNAKE_RegMenuGroup();  
    //初始化im nv
    MMIGMSNAKE_RegNv();   
    MMIGAME_SNAKE_RegWinIdNameArr();
}


/*****************************************************************************/
//  FUNCTION:     Snake_MsgBox_Gameover
//  Description :   
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC void  Snake_MsgBox_Gameover(void)
{
    //提示游戏结束，左键重开，右键退出
    MMIPUB_OpenQueryWinByTextId(TXT_SNAKE_GAMEOVER,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
}


/*****************************************************************************/
//  FUNCTION:     Snake_ResetFrog
//  Description :   重设蛇可吃的方块的位置
//  Global resource dependence :
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN  Snake_ResetFrog(void)
{
    uint32 randomnum = 0;
    uint16 i = 0;
    // 获得随机数
    randomnum = rand();
    //GETRAND((uint8*)&randomnum,2);
    // 出去边框四周的点，在中间((SNAKE_XGRIDS-2)*(SNAKE_YGRIDS-2))区域放方块
    randomnum = randomnum%((SNAKE_XGRIDS-2)*(SNAKE_YGRIDS-2));
    randomnum = (randomnum/(SNAKE_XGRIDS-2)+1)*(SNAKE_XGRIDS)+randomnum%(SNAKE_XGRIDS-2)+1;
    s_snake_flogindex = randomnum;
    // 查找如果方块在蛇身，则将方块放在蛇尾处，保证方块不在蛇身上
    for (i = 0;i<s_snake_total;i++)
    {
        if (s_snake_body_cache[i] == randomnum)
        {
            s_snake_flogindex = s_snake_body_cache[0];
            break;
        }
    }
    return TRUE;
}

/*****************************************************************************/
//  FUNCTION:     Snake_AllocGameSpace
//  Description : 分配游戏需要的内存空间
//  Global resource dependence : 
//  Author:       leon.wang
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN Snake_AllocGameSpace(void)
{
    
    if(PNULL != s_snake_body_cache)
    {
        SCI_FREE(s_snake_body_cache);
        s_snake_body_cache = PNULL;
    }
    if(PNULL != s_snake_is_snakebody)
    {
        SCI_FREE(s_snake_is_snakebody);
        s_snake_is_snakebody = PNULL;
    }

    s_snake_body_cache = (uint16*)SCI_ALLOCAZ(sizeof(uint16)*(SNAKE_XGRIDS*SNAKE_YGRIDS));
    s_snake_is_snakebody=(BOOLEAN*)SCI_ALLOCAZ(sizeof(BOOLEAN)*(SNAKE_XGRIDS*SNAKE_YGRIDS));

    if(PNULL == s_snake_is_snakebody || PNULL == s_snake_body_cache)
    {
        if(PNULL != s_snake_body_cache)
        {
            SCI_FREE(s_snake_body_cache);
            s_snake_body_cache = PNULL;
        }
        if(PNULL != s_snake_is_snakebody)
        {
            SCI_FREE(s_snake_is_snakebody);
            s_snake_is_snakebody = PNULL;
        }
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  FUNCTION:     Snake_InitGame
//  Description :   初始化游戏
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN Snake_InitGame(void)
{

    if(PNULL == s_snake_is_snakebody || PNULL == s_snake_body_cache)
    {
        return FALSE;
    }
    else
    {
        // 将默认值设入变量
        SCI_MEMSET(s_snake_body_cache,0x0,sizeof(uint16)*(SNAKE_XGRIDS*SNAKE_YGRIDS));
        SCI_MEMSET(s_snake_is_snakebody,0x0,sizeof(BOOLEAN)*(SNAKE_XGRIDS*SNAKE_YGRIDS));
    }

    s_snake_currentscore = 0;
    // 默认4个
    s_snake_total = 4; 
    // 默认有方向启动
    s_snake_direction = MSG_KEYDOWN_RIGHT; 
    // 默认蛇从左往右
    s_snake_body_cache[0] = 0;
    s_snake_body_cache[1] = 1;
    s_snake_body_cache[2] = 2;
    s_snake_body_cache[3] = 3;
    // 默认4个的点给予标记为蛇身
    s_snake_is_snakebody[0] = 1;
    s_snake_is_snakebody[1] = 1;
    s_snake_is_snakebody[2] = 1;
    s_snake_is_snakebody[3] = 1;
    // 游戏状态置为开始
    s_snake_game_state = SNAKE_GAME_ON; 
    // 初始一个随机方块
    Snake_ResetFrog();

    return TRUE;
}

/*****************************************************************************/
//  FUNCTION:     Snake_ExitGame
//  Description :   结束游戏
//  Global resource dependence : 
//  Author:       leon.wang
//  Note:   
/*****************************************************************************/
PUBLIC void Snake_ExitGame(void)
{  
    if(PNULL != s_snake_body_cache)
    {
        SCI_FREE(s_snake_body_cache);
        s_snake_body_cache = PNULL;
    }
    if(PNULL != s_snake_is_snakebody)
    {
        SCI_FREE(s_snake_is_snakebody);
        s_snake_is_snakebody = PNULL;
    }
}

/*****************************************************************************/
//  FUNCTION:     Snake_DrawGrid
//  Description :   画蛇的格子
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN Snake_DrawGrid(MMI_WIN_ID_T win_id,uint16 nIndex,GAMESNAKE_GRIDTYPE_E gridtype)
{
    GUI_RECT_T Rc = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    // SNAKE_START_X，SNAKE_START_Y为游戏地图格子的起点
    // SNAKE_XGRIDS，SNAKE_YGRIDS为横纵格子数
    // SNAKE_GRID_WIDTH,SNAKE_GRID_HEIGHT为每个格子所占的像数
    Rc.left     = SNAKE_START_X + nIndex%SNAKE_XGRIDS*SNAKE_GRID_WIDTH;
    Rc.right    = Rc.left + SNAKE_GRID_WIDTH;
    Rc.top      = SNAKE_START_Y + nIndex/SNAKE_XGRIDS*SNAKE_GRID_HEIGHT;
    Rc.bottom = Rc.top + SNAKE_GRID_HEIGHT;

    // 不同的类型用不同的图标
    GUIRES_DisplayImg(
                    PNULL,
                    &Rc,
                    PNULL,
                    win_id,
                    s_snake_gridimg[gridtype],
                    &lcd_dev_info);    

   // GUI_FillRect(&lcd_dev_info,Rc,clrFrame);
    return TRUE;
}

/*****************************************************************************/
//  FUNCTION:     MMIGMSNAKE_AddTwoString
//  Description :    链接两个字串为一个
//  Global resource dependence : 
//  Author:      
//  Note:   
/*****************************************************************************/
PUBLIC MMI_STRING_T MMIGMSNAKE_AddTwoString(
                                          MMI_STRING_T first, 
                                          MMI_STRING_T second
                                          )
{
    MMI_STRING_T result = {0};
    uint16 j = 0;
    result.wstr_len = first.wstr_len + second.wstr_len;
    result.wstr_ptr = (wchar*)SCI_ALLOC_APP( (result.wstr_len + 1) * sizeof(wchar) );

    if (PNULL == result.wstr_ptr)
    {
        result.wstr_len = 0;
        return result;
    }
    
    MMI_WSTRNCPY( result.wstr_ptr, result.wstr_len, first.wstr_ptr, first.wstr_len, first.wstr_len );
    j = first.wstr_len;   
    MMI_WSTRNCPY( result.wstr_ptr+j, result.wstr_len-j, second.wstr_ptr, second.wstr_len, second.wstr_len );
    return result;
    
} 

/*****************************************************************************/
//  FUNCTION:     DrawTipString
//  Description :   画字
//  Global resource dependence : s_links_tip_times
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL void Snake_DrawTipString(void)
{
    GUI_POINT_T         startpoint = {0};
    GUI_RECT_T          scorerect = SNAKE_SCORE_STRING_RECT;
    GUI_RECT_T          levelrect = SNAKE_LEVEL_STRING_RECT;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN}; 
    
    uint8   asc_str[12] = {0};
    wchar   wstr[12] = {0};
    MMI_STRING_T  str2 = {0};
    MMI_STRING_T  temp_str = {0};
    MMI_STRING_T  result = {0};
    GUISTR_STYLE_T str_style ={0};

    str_style.font = MMIGMSNAKE_GAME_FONT;
    str_style.font_color = COLOR_SNAKE_STRING;

    MMI_GetLabelTextByLang(TXT_SNAKE_SCORE, &temp_str);
    sprintf((char*)asc_str,":%d",s_snake_currentscore);
      
    str2.wstr_len = strlen((char*)asc_str);     
    str2.wstr_ptr = wstr;
    MMI_STRNTOWSTR( wstr, 12, asc_str, 12, str2.wstr_len );
    result = MMIGMSNAKE_AddTwoString(temp_str,str2);
    if(PNULL != result.wstr_ptr && 0 != result.wstr_len)
    {
        // 屏幕上写上"分数:XX" 
        str_style.align = ALIGN_LEFT;
        GUISTR_DrawTextToLCDInRect( 
                                    &lcd_dev_info, 
                                    &scorerect, 
                                    &scorerect,
                                    &result,
                                    &str_style,
                                    GUISTR_STATE_ALIGN,
                                    0); 
              
        SCI_FREE(result.wstr_ptr);
    }

    startpoint.x = scorerect.left;
    startpoint.y = scorerect.top;
    MMI_GetLabelTextByLang(s_snake_level_list[s_snake_current_level], &temp_str);
    // 屏幕上写上等级"简单"
    str_style.align = ALIGN_RIGHT;
    GUISTR_DrawTextToLCDInRect( 
                                    &lcd_dev_info, 
                                    &levelrect, 
                                    &levelrect,
                                    &temp_str,
                                    &str_style,
                                    GUISTR_STATE_ALIGN,
                                    0); 


}

/*****************************************************************************/
//  FUNCTION:     Snake_Draw
//  Description :   游戏绘画
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
LOCAL void Snake_Draw(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint32              i = 0;
    uint32              snake_len = s_snake_total-1;

    // 画背景
#ifdef MMIGAME_SNAKE_LOW_MEMORY      //精简资源时，背景使用颜色平铺
    {
        GUI_RECT_T      rect =MMITHEME_GetFullScreenRect();
   
        //描绘大背景
        GUI_FillRect(&lcd_dev_info, rect, MMI_DARK_YELLOW_COLOR);
        
        //描绘中间场地
        rect.left = SNAKE_START_X;
        rect.right = SNAKE_START_X+SNAKE_XGRIDS*SNAKE_GRID_WIDTH;
        rect.top = SNAKE_START_Y;
        rect.bottom = SNAKE_START_Y+SNAKE_YGRIDS*SNAKE_GRID_HEIGHT;
        GUI_FillRect(&lcd_dev_info, rect, MMI_DARK_GREEN_COLOR);
    }
#else
    {
        GUI_POINT_T     point = {0, 0};
        //背景图片形式
        GUIRES_DisplayImg(
                        &point,
                        PNULL,
                        PNULL,
                        win_id,
                        IMAGE_GAME_SNAKE_BG,
                        &lcd_dev_info);
    }
#endif
    //该部分对数据的处理，不应该放在绘图函数中，需要移出去
    // 重设地图蛇的状
    SCI_MEMSET(s_snake_is_snakebody,0x0,sizeof(BOOLEAN)*(SNAKE_XGRIDS*SNAKE_YGRIDS));
    // 画蛇身
    for (i = 0; i<snake_len; i++)
    {
        Snake_DrawGrid(win_id, s_snake_body_cache[i], GRIDTYPE_GMSNAKE_BODY);
        s_snake_is_snakebody[s_snake_body_cache[i]] = TRUE;
    }
    // 画蛇头
    Snake_DrawGrid(win_id,s_snake_body_cache[i],GRIDTYPE_GMSNAKE_HEAD);
    s_snake_is_snakebody[s_snake_body_cache[i]] = TRUE;
    // 画方块
    Snake_DrawGrid(win_id,s_snake_flogindex,GRIDTYPE_GMSNAKE_FLOG);
    // 画分数和难度
    Snake_DrawTipString();
}

/*****************************************************************************/
//  FUNCTION:     Snake_SetRunDirection
//  Description :   通过蛇运动的方向，重设蛇的状态和吃子
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   返回false则gameover
/*****************************************************************************/
LOCAL BOOLEAN Snake_SetRunDirection(void)
{
    BOOLEAN bResult = FALSE;
    // 如果已经按键改变方向，则保存现在方向
    if(s_snake_direction_cache != 0)
    {
        s_snake_direction = s_snake_direction_cache;
        s_snake_direction_cache = 0;
    }
    // 通过蛇的方向，重设蛇的状态
    switch (s_snake_direction)
    {
        case MSG_KEYDOWN_RIGHT:
            {
                // 如果已经在最右边，再往右，gameover
                if (SNAKE_XGRIDS <= s_snake_body_cache[s_snake_total-1]%SNAKE_XGRIDS+1)
                {
                    return FALSE;
                }
                // 如果右边有蛇身所在，再往右，gameover
                if (s_snake_is_snakebody[s_snake_body_cache[s_snake_total-1]+1])
                {
                    return FALSE;
                }
                // 如果右边是可以吃的方块，则吃了它
                else if (s_snake_flogindex == s_snake_body_cache[s_snake_total-1]+1)
                {
                    s_snake_body_cache[s_snake_total] = s_snake_flogindex;
                    s_snake_is_snakebody[s_snake_flogindex] = TRUE;
                    s_snake_currentscore++;
                    s_snake_total++;
                    Snake_ResetFrog();
                    bResult = TRUE;
                }
                // 如果以上状态都不是，则蛇右移一格
                else
                {
                    SCI_MEMCPY((void *)s_snake_body_cache,(const void *)&s_snake_body_cache[1],(s_snake_total-1)*sizeof(uint16));
                    s_snake_body_cache[s_snake_total-1] = s_snake_body_cache[s_snake_total-1]+1;
                    bResult = TRUE;
                }
            }
            break;
        case MSG_KEYDOWN_LEFT:
            {
                // 如果已经在最左边，再往左，gameover
                if (0 == s_snake_body_cache[s_snake_total-1]%SNAKE_XGRIDS)
                {
                    return FALSE;
                }
                // 如果左边有蛇身所在，再往左，gameover
                if (s_snake_is_snakebody[s_snake_body_cache[s_snake_total-1]-1])
                {
                    return FALSE;
                }
                // 如果左边是可以吃的方块，则吃了它
                else if (s_snake_flogindex == s_snake_body_cache[s_snake_total-1]-1)
                {
                    s_snake_body_cache[s_snake_total] = s_snake_flogindex;
                    s_snake_is_snakebody[s_snake_flogindex] = TRUE;
                    s_snake_currentscore++;
                    s_snake_total++;
                    Snake_ResetFrog();
                    bResult = TRUE;
                }
                // 如果以上状态都不是，则蛇左移一格
                else
                {
                    SCI_MEMCPY((void *)s_snake_body_cache,(const void *)&s_snake_body_cache[1],(s_snake_total-1)*sizeof(uint16));
                    s_snake_body_cache[s_snake_total-1] = s_snake_body_cache[s_snake_total-1]-1;
                    bResult = TRUE;
                }
            }
            break;
        case MSG_KEYDOWN_DOWN:
            {
                // 如果已经在最下边，再往下，gameover
                if ((SNAKE_YGRIDS-1) <= (s_snake_body_cache[s_snake_total-1]/SNAKE_XGRIDS))
                {
                    return FALSE;
                }
                // 如果下边有蛇身所在，再往下，gameover
                if (s_snake_is_snakebody[s_snake_body_cache[s_snake_total-1]+SNAKE_XGRIDS])
                {
                    return FALSE;
                }
                // 如果下边是可以吃的方块，则吃了它
                else if (s_snake_flogindex == s_snake_body_cache[s_snake_total-1]+SNAKE_XGRIDS)
                {
                    s_snake_body_cache[s_snake_total] = s_snake_flogindex;
                    s_snake_is_snakebody[s_snake_flogindex] = TRUE;
                    s_snake_currentscore++;
                    s_snake_total++;
                    Snake_ResetFrog();
                    bResult = TRUE;
                }
                // 如果以上状态都不是，则蛇下移一格
                else
                {
                    SCI_MEMCPY((void *)s_snake_body_cache,(const void *)&s_snake_body_cache[1],(s_snake_total-1)*sizeof(uint16));
                    s_snake_body_cache[s_snake_total-1] = s_snake_body_cache[s_snake_total-1]+SNAKE_XGRIDS;
                    bResult = TRUE;
                }
            }
            break;
        case MSG_KEYDOWN_UP:
            {
                // 如果已经在最上边，再往上，gameover
                if (0 == s_snake_body_cache[s_snake_total-1]/SNAKE_XGRIDS)
                {
                    return FALSE;
                }
                // 如果上边有蛇身所在，再往上，gameover
                if (s_snake_is_snakebody[s_snake_body_cache[s_snake_total-1]-SNAKE_XGRIDS])
                {
                    return FALSE;
                }
                // 如果上边是可以吃的方块，则吃了它
                else if (s_snake_flogindex == s_snake_body_cache[s_snake_total-1]-SNAKE_XGRIDS)
                {
                    s_snake_body_cache[s_snake_total] = s_snake_flogindex;
                    s_snake_is_snakebody[s_snake_flogindex] = TRUE;
                    s_snake_currentscore++;
                    s_snake_total++;
                    Snake_ResetFrog();
                    bResult = TRUE;
                }
                // 如果以上状态都不是，则蛇上移一格
                else
                {
                    SCI_MEMCPY((void *)s_snake_body_cache,(const void *)&s_snake_body_cache[1],(s_snake_total-1)*sizeof(uint16));
                    s_snake_body_cache[s_snake_total-1] = s_snake_body_cache[s_snake_total-1]-SNAKE_XGRIDS;
                    bResult = TRUE;
                }
            }
            break;
        default:
            //{
                // 此状态不应该存在
                //SCI_ASSERT(0);
           // }
            break;
    }
    return bResult;
}

/*****************************************************************************/
//  FUNCTION:     Snake_GameGoing
//  Description :   
//  Global resource dependence : s_snake_timer
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC void  Snake_GameGoing(MMI_WIN_ID_T win_id)
{
    if (0 != s_snake_timer)  
    {
        MMK_StopTimer(s_snake_timer);
        s_snake_timer = 0;
    }
    if (SNAKE_GAME_ON != s_snake_game_state)
    {
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_COMMON_COMMON_CONTINUE, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);  
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_SNAKE_PAUSE, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);  
    }
    // 游戏暂停需要醒目提示
    if (SNAKE_GAME_PAUSE == s_snake_game_state)
    {
        MMI_STRING_T        name_str = {0};
        GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
        GUI_RECT_T          dis_rect = MMITHEME_GetClientRect();
        GUISTR_STYLE_T      str_style = {0};
        MMI_GetLabelTextByLang(TXT_SNAKE_GAMEPAUSED, &name_str);
        // 游戏运行界面主绘画
        Snake_Draw(win_id);
        str_style.font = MMIGMSNAKE_GAME_PAUSED_FONT;
        str_style.font_color = MMI_RED_COLOR;
        str_style.align = ALIGN_HVMIDDLE;
        GUISTR_DrawTextToLCDInRect( 
                                    &lcd_dev_info, 
                                    &dis_rect, 
                                    &dis_rect,
                                    &name_str,
                                    &str_style,
                                    GUISTR_STATE_ALIGN,
                                    0);
        return;
    }
    // 没有在on的状态直接退出 
    else if (SNAKE_GAME_OVER == s_snake_game_state)
    {
        Snake_Draw(win_id);
        return;
    }
    else if (SNAKE_GAME_ON != s_snake_game_state)
    {
        return;
    }
    
    // 设置蛇的方向，如果false则，gameover
    if (!Snake_SetRunDirection())
    {  
        s_snake_game_state = SNAKE_GAME_OVER;
    }
    // 游戏运行界面主绘画
    Snake_Draw(win_id);
    // gameover时的提示
    if (SNAKE_GAME_OVER == s_snake_game_state)
    {
        Snake_SaveGameScore();
        // 游戏运行界面主绘画
        Snake_Draw(win_id);
        Snake_MsgBox_Gameover();
        return;
    }
    // 根据不同的难度，设置timer
    if (s_snake_current_level == SNAKE_LEVEL_EASY)
    {
        s_snake_timer = MMK_CreateTimer( 500, FALSE);
    }
    else if (s_snake_current_level == SNAKE_LEVEL_NORMAL)
    {
        s_snake_timer = MMK_CreateTimer( 300, FALSE);
    }
    else if (s_snake_current_level == SNAKE_LEVEL_HARD)
    {
        s_snake_timer = MMK_CreateTimer( 100, FALSE);
    }
}

/*****************************************************************************/
//  FUNCTION:     Snake_SaveGameScore
//  Description :   记录游戏分数
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC void Snake_SaveGameScore(void)
{
    uint16 high_score = 0;
    uint16 nv_return = 0;   
    // 读nv
    MMINV_READ(MMINV_GMSNAKE_HIGH_SCORE, &high_score, nv_return);    
    if (MN_RETURN_SUCCESS != nv_return)
    {
        high_score = 0;
        MMINV_WRITE(MMINV_GMSNAKE_HIGH_SCORE, &high_score);
    }
    // 如果分数超过最高分，则存nv
    if (s_snake_currentscore > high_score)  
    {   
        high_score = s_snake_currentscore;  
        MMINV_WRITE(MMINV_GMSNAKE_HIGH_SCORE,&high_score);  
    }   
}

/*****************************************************************************/
//  FUNCTION:     Snake_GetLevelText
//  Description :   获取level对应的字符串
//  Global resource dependence : 
//  Author:      leon.wang
//  Note:   
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T Snake_GetLevelText(uint16 level)
{
    if(SNAKE_TOTAL_LEVEL>level)
    {
        return s_snake_level_list[level];
    }
    else
    {
        return s_snake_level_list[0];
    }

}
#endif

