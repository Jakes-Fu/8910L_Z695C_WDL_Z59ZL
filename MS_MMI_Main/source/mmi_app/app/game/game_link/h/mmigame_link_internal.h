/*****************************************************************************
** File Name:      mmigame_link_internal.h                                                   *
** Author:                                                                   *
** Date:           04/03/2006                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe the internal api of game              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/03/2006      Annie.an      Create
******************************************************************************/

#ifndef _MMIGAME_LINK_INTERNAL_H_
#define _MMIGAME_LINK_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#ifdef GAME_LINK_SUPPORT
#include "os_api.h"
#include "mmk_type.h"
#include "guicommon.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "mmi_position.h"
#include "mmigame_link_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define LINKS_Y_OFFSET              4           //整个地图的y偏移量
#define LINKS_BOARD_ROWS            10          //地图的水平子图片个数
#define LINKS_BOARD_COLS            10          //地图的垂直子图片个数
#define LINKS_FULL_TIME             70          //每一关的时间70秒
#define LINKS_WASH_SWAP_TIMES       40          //每次洗牌交换次数
#define LINKS_SCORE_ELIMINATE_PAIR  10          //消除一对的分数
#define LINKS_SCORE_EPISODE(time)   (time * 10) //过关时根据时间的分数
#define LINKS_TOTAL_WASH_TIMES      3           //每次游戏最多洗牌次数
#define LINKS_TOTAL_TIP_TIMES       7           //每次游戏最多提示次数

#define COLOR_LINKS_BACK            0x0020      //背景色
#define COLOR_LINKS_RECT_BACK       0x22F0      //框的背景色
#define COLOR_LINKS_RECT_LINE       0x8410      //框的边框色
#define COLOR_LINKS_STRING          0xb5b6// MMI_DARK_GRAY_COLOR        //文字颜色
#define COLOR_LINKS_CONNECT_LINE    MMI_GRAY_WHITE_COLOR        //连接线的颜色
#define COLOR_LINKS_MIST_PRESSED    0x6e7e      //被点击的图片的alpha blending颜色
#define COLOR_LINKS_MIST_TIPED      0xffff    //被提示的图片的alpha blending颜色


#define LINKS_TILE_X(col)   (g_LINKS_TILES_RECT_LEFT + (col - 1) * LINKS_TILE_WIDTH)
#define LINKS_TILE_Y(row)   (g_LINKS_TILES_RECT_TOP + (row -1 ) * LINKS_TILE_HEIGHT)
#define LINKS_TILE_COL(x)   ((x + LINKS_TILE_WIDTH - g_LINKS_TILES_RECT_LEFT) / LINKS_TILE_WIDTH)
#define LINKS_TILE_ROW(y)   ((y + LINKS_TILE_HEIGHT - g_LINKS_TILES_RECT_TOP) / LINKS_TILE_HEIGHT)


/*
#define LINKS_TILE_X(col)   (tiles_left + (col - 1) * tile_width)
#define LINKS_TILE_Y(row)   (tiles_top + (row -1 ) * tile_height)
#define LINKS_TILE_COL(x)   ((x + tile_width - tiles_left) / tile_width)
#define LINKS_TILE_ROW(y)   ((y + tile_height - tiles_top) / tile_height)
*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
//游戏的难度
typedef enum
{
    LINKS_LEVEL_EASY,
    LINKS_LEVEL_NORMAL,
    LINKS_LEVEL_HARD,
    LINKS_TOTAL_LEVEL
}LINKS_LEVEL_E;

//游戏结束状态
typedef enum
{
    LINKS_GAME_NULL,
    LINKS_GAME_ON,
    LINKS_GAME_OVER,
    LINKS_GAME_PAUSE,
    LINKS_GAME_PASS,
    LINKS_TOTAL_STATE
}LINKS_GAME_STATE_E;

//子图片的显示状态
typedef enum
{
    LINKS_TILE_NORMAL,            //普通状态
    LINKS_TILE_PRESSED,           //被点击后的状态
    LINKS_TILE_TIPED               //点击提示后的状态
}LINKS_TILE_STATUS_E;

//游戏的每一关
typedef enum
{
    LINKS_EPISODE_STILL,                 //第一关  不动
    LINKS_EPISODE_MOVEDOWN,              //第二关  向下
    LINKS_EPISODE_MOVELEFT,              //第三关  向左
    LINKS_EPISODE_V_DISPERSE,            //第四关  上下分离
    LINKS_EPISODE_V_CENTRALIZE,          //第五关  上下集中
    LINKS_EPISODE_H_DISPERSE,            //第六关  左右分离
    LINKS_EPISODE_H_CENTRALIZE,          //第七关  左右集中
    LINKS_EPISODE_TOTAL                  //总关数
}LINKS_EPISODE_E;

//子图片的id
typedef enum
{
    LINKS_TILE_ID_0,
    LINKS_TILE_ID_1,
    LINKS_TILE_ID_2,
    LINKS_TILE_ID_3,
    LINKS_TILE_ID_4,
    LINKS_TILE_ID_5,
    LINKS_TILE_ID_6,
    LINKS_TILE_ID_7,
    LINKS_TILE_ID_8,
    LINKS_TILE_ID_9,
    LINKS_TILE_ID_10,
    LINKS_TILE_ID_11,
    LINKS_TILE_ID_12,
    LINKS_TILE_ID_13,
    LINKS_TILE_ID_14,
    LINKS_TILE_ID_15,

    LINKS_TILE_MAX_NUMBER,         //子图片总数
    LINKS_TILE_BLANK = 256, // #define LINKS_TILE_BLANK      256              // 无图片时的id
}LINKS_TILES_ID_E;

//描述一个子图片的信息
typedef struct
{
    uint8                     row;
    uint8                     col;
    LINKS_TILES_ID_E          id;
}LINKS_TILE_T;

//描述一条连接路径
typedef struct 
{
    GUI_POINT_T               points[4];              //每个点的具体位置
    uint8                     point_number;           //该路径的点的数量为 2-4个是有效路径的
}LINKS_PATH_T;

/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_Init(void)
//  Description :   
//
//  Global resource dependence : 
//   Author:        taul.zhu
//  Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIGMLINK_Init(void);

/*****************************************************************************/
//   FUNCTION:    MMIGMQUT_RegNv(void)
//  Description :   
//
//  Global resource dependence : 
//   Author:        taul.zhu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMLINK_RegNv(void);

/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_InitModule
//  Description :   
//
//  Global resource dependence : 
//   Author:        taul.zhu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMLINK_InitModule(void);

/*****************************************************************************/
//   FUNCTION:      reg quintet nenu group
//  Description :   
//
//  Global resource dependence : 
//   Author:        taul.zhu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMLINK_RegMenuGroup(void);

/*****************************************************************************/
//  FUNCTION:      RandomizeBoard
//  Description :  Randomized the game board when game start or need randomize
//  Global resource dependence : 
//  Author:        Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void RandomizeBoard(
                      void
                      );

/*****************************************************************************/
//  FUNCTION:      InitStage
//  Description :  Initialize a stage
//  Global resource dependence : 
//  Author:        Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void InitStage(
                      void
                      );

/*****************************************************************************/
//  FUNCTION:     StartGameLinks
//  Description :   
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void StartGameLinks(
                      void
                      );

/*****************************************************************************/
//  FUNCTION:     ResumeGameLinks
//  Description :   
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void ResumeGameLinks(
                       void
                       );

/*****************************************************************************/
//  FUNCTION:     EndingGameSquare
//  Description :   
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void EndingGameLinks(
                            MMI_WIN_ID_T win_id 
                            );


/*****************************************************************************/
//  FUNCTION:     OnTimerAction
//  Description :   
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void OnTimerAction(
            MMI_WIN_ID_T win_id);


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
                            );

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
            uint16 score);
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
                );
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
                        );

/*****************************************************************************/
//  FUNCTION:     IsAllEliminate
//  Description :   is all tiles are eliminated
//  Global resource dependence : s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN IsAllEliminate(
                        void
                        );

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
                        );

/*****************************************************************************/
//  FUNCTION:     DrawTimeString
//  Description :   dispaly time information
//  Global resource dependence : s_links_time_left
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawTimeString(
            MMI_WIN_ID_T win_id
            );

/*****************************************************************************/
//  FUNCTION:     DrawTimeRect
//  Description :   dispaly time rect including title
//  Global resource dependence :
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawTimeRect(
            MMI_WIN_ID_T win_id
            );

/*****************************************************************************/
//  FUNCTION:     DrawScoreString
//  Description :   dispaly score information
//  Global resource dependence : s_links_score
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawScoreString(
            MMI_WIN_ID_T win_id
            );

/*****************************************************************************/
//  FUNCTION:     DrawScoreRect
//  Description :   dispaly score rect including title
//  Global resource dependence :
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawScoreRect(
            MMI_WIN_ID_T win_id
            );

/*****************************************************************************/
//  FUNCTION:     DrawEpisodeString
//  Description :   dispaly episode information
//  Global resource dependence : s_links_current_episode
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawEpisodeString(
            void
            );

/*****************************************************************************/
//  FUNCTION:     DrawEpisodeRect
//  Description :   dispaly episode rect including title
//  Global resource dependence :
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawEpisodeRect(
            MMI_WIN_ID_T win_id
            );

/*****************************************************************************/
//  FUNCTION:     DrawWashCardString
//  Description :   dispaly wash card information
//  Global resource dependence : s_links_random_times
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawWashCardString(
            void
            );

/*****************************************************************************/
//  FUNCTION:     DrawWashCardRect
//  Description :   dispaly wash card rect
//  Global resource dependence :
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawWashCardRect(
                              MMI_WIN_ID_T    win_id);

/*****************************************************************************/
//  FUNCTION:     DrawTipString
//  Description :   dispaly tip information
//  Global resource dependence : s_links_tip_times
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawTipString(
            MMI_WIN_ID_T win_id
            );

/*****************************************************************************/
//  FUNCTION:     DrawTipRect
//  Description :   dispaly tip rect
//  Global resource dependence :
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawTipRect(
           MMI_WIN_ID_T    win_id
            );

/*****************************************************************************/
//  FUNCTION:     DrawPauseString
//  Description :   dispaly pause information
//  Global resource dependence : s_links_is_paused
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawPauseString(
            MMI_WIN_ID_T win_id
            );

/*****************************************************************************/
//  FUNCTION:     DrawPauseRect
//  Description :   dispaly pause rect
//  Global resource dependence :
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawPauseRect(
            void
            );

/*****************************************************************************/
//  FUNCTION:     DrawTile
//  Description :   dispaly a tile
//  Global resource dependence :
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawTile(
            LINKS_TILE_T  tile,
            LINKS_TILE_STATUS_E     status, //子图片状态 普通/点击/提示
            MMI_WIN_ID_T win_id
            );

/*****************************************************************************/
//  FUNCTION:     DrawAll
//  Description :   dispaly all the interface
//  Global resource dependence : s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawAll(
            MMI_WIN_ID_T win_id
            );

/*****************************************************************************/
//  FUNCTION:     DrawConnectLine
//  Description :   dispaly connect line
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawConnectLine(
                        LINKS_PATH_T   path
                        );

/*****************************************************************************/
//  FUNCTION:     DrawPauseScreen
//  Description :   dispaly pause screen
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawPauseScreen(
                        MMI_WIN_ID_T win_id
                        );

/*****************************************************************************/
//  FUNCTION:     DrawGameOverScreen
//  Description :   dispaly game over screen
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawGameOverScreen(
                        MMI_WIN_ID_T win_id
                        );

/*****************************************************************************/
//  FUNCTION:     DrawGameFinishScreen
//  Description :   dispaly game finish screen
//  Global resource dependence : 
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void DrawGameFinishScreen(
                        MMI_WIN_ID_T win_id
                        );

/*****************************************************************************/
//  FUNCTION:     OnPauseClicked
//  Description :   handle when pause button is clicked
//  Global resource dependence :   s_links_is_paused
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void OnPauseClicked(
                        MMI_WIN_ID_T win_id
                        );

/*****************************************************************************/
//  FUNCTION:     OnTipClicked
//  Description :   handle when tip button is clicked
//  Global resource dependence :   s_links_tip_times, s_links_connected_pair
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void OnTipClicked(
                        MMI_WIN_ID_T win_id
                        );

/*****************************************************************************/
//  FUNCTION:     OnWashCardClicked
//  Description :   handle when wash card button is clicked
//  Global resource dependence :   s_links_random_times, s_links_connected_pair
//                                 s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void OnWashCardClicked(
                        MMI_WIN_ID_T win_id
                        );

/*****************************************************************************/
//  FUNCTION:     OnTileClicked
//  Description :   handle when tile is clicked
//  Global resource dependence :   s_links_connected_pair, s_links_board
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void OnTileClicked(
                       MMI_WIN_ID_T win_id,
                       LINKS_TILE_T tile
                        );

/*****************************************************************************/
//  FUNCTION:     SetBackColor
//  Description :   
//  Global resource dependence : NONE
//  Author:       
//  Note:   
/*****************************************************************************/
PUBLIC void SetBackColor(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  FUNCTION:     SetResumValue
//  Description :   
//  Global resource dependence : NONE
//  Author:       xiaoqing.lu 
//  Note:   
/*****************************************************************************/
PUBLIC void SetResumValue(
                          BOOLEAN is_can_resume
                          );
/*****************************************************************************/
//  Description : Register acc menu group
//  Global resource dependence : none
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIGAME_LINK_RegWinIdNameArr(void);

/*****************************************************************************/
//  FUNCTION:     SaveGameScore
//  Description :  for CR MS00181020
//  Global resource dependence : 
//  Author:       ying.xu
//  Note:   
/*****************************************************************************/
PUBLIC void SaveGameScore(void);

#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : register link game module applet info
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIGMLINK_RegAppletInfo(void);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif  

