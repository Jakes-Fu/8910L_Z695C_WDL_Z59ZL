/*****************************************************************************
** File Name:      mmigame_quintet_internal.h                                                   *
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

#ifndef _MMIGAME_QUINTET_INTERNAL_H_
#define _MMIGAME_QUINTET_INTERNAL_H_
#ifdef GAME_BOXMAN_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "guicommon.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "mmigame_boxman_export.h"

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
#define BOXMAN_MAX_MAP_ROW              9   //地图最大行数
#define BOXMAN_MAX_MAP_COL              9   //地图最大列数
#define BOXMAN_MAX_MOVE_STEP            30  //两点之间的最大移动步数
#define BOXMAN_MAX_GO_BACK_STEPS        5   //最大回退步数
#define BOXMAN_MAP_NUMS                 15  //总的地图个数
#define BOXMAN_MAX_MAP_AIM              15  //地图中的最大目标点数目
#define BOXMAN_MAX_SEARCH_STEP          64  //最大路径搜索步数
#define BOXMAN_MAX_SELECT_MAP_NUM_LEN   2   //最大选关数字长度

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//移动方向
typedef enum
{
    TOWARDS_LEFT=1, //向左
    TOWARDS_RIGHT,  //向右
    TOWARDS_UP,     //向上
    TOWARDS_DOWN    //向下
} BOXMAN_MOVE_DIRECTION_E;
//地图数据值
typedef enum
{
    BOXMAN_WALLOUT=0,   //表示墙外
    BOXMAN_SPACE,   //表示空地
    BOXMAN_WALL,    //表示墙
    BOXMAN_MAN, //表示小人
    BOXMAN_BOX, //表示箱子
    BOXMAN_AIM, //表示目标
    BOXMAN_MAN_IN_AIM,  //表示小人在目标
    BOXMAN_BOX_IN_AIM,  //表示箱子在目标
    BOXMAN_SUB_MAP_NUMS //子图个数
} BOXMAN_MAP_DATA_VALUE_E;
//一幅地图的行列数结构
typedef struct
{
    uint8 row;  //行数
    uint8 col;  //列数
} BOXMAN_MAP_ROW_AND_COL_T;
//地图中的一个点的结构
typedef struct
{
    uint16 x;   //点在地图中的x坐标值
    uint16 y;   //点在地图中的y坐标值
} BOXMAN_POINT_T;//地图左上角为坐标原点，原点坐标为(0,0),向右，x值增加，向下，y值减小。
//一次箱子的移动步骤,用在回退队列中
typedef struct
{
    BOXMAN_POINT_T box_before_move; //移动前箱子的位置
    BOXMAN_MAP_DATA_VALUE_E aim_value_before_move;  //目标位置的移动前地图数据值
    BOXMAN_MAP_DATA_VALUE_E box_value_before_move;  //箱子位置的移动前地图数据值
    BOXMAN_MAP_DATA_VALUE_E man_value_before_move;  //小人位置的移动前地图数据值
    BOXMAN_MOVE_DIRECTION_E move_direc; //移动方向
    BOXMAN_MAP_DATA_VALUE_E aim_value_after_move;   //目标位置的移动后地图数据值
    BOXMAN_MAP_DATA_VALUE_E box_value_after_move;   //箱子位置的移动后地图数据值
    BOXMAN_MAP_DATA_VALUE_E man_value_after_move;   //小人位置的移动后地图数据值
} BOXMAN_BOX_MOVE_STEP_T;
//回退队列记录结构
typedef struct
{
    uint8 then_current_map[BOXMAN_MAX_MAP_ROW][BOXMAN_MAX_MAP_COL]; //当时的地图数据
    BOXMAN_POINT_T then_current_man;    //当时的小人的位置
} BOXMAN_GO_BACK_T;
//移动队列结构
typedef struct
{
    uint8 move_num; //队列中的移动步数
    BOXMAN_POINT_T queue[BOXMAN_MAX_MOVE_STEP]; //移动队列
} MOVE_QUEUE_T;


/*****************************************************************************/
//   FUNCTION:     MMIGAME_AddTwoString
//  Description :   add two string 
//
//  Global resource dependence : 
//   Author:      
//  Note:   
/*****************************************************************************/
PUBLIC MMI_STRING_T MMIGMBOXMAN_AddTwoString(
                MMI_STRING_T first, 
                MMI_STRING_T second
                );
/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_InitModule
//  Description :   
//
//  Global resource dependence : 
//   Author:        taul.zhu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMBOXMAN_InitModule(void);
/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_InitModule
//  Description :   
//
//  Global resource dependence : 
//   Author:        taul.zhu
//  Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIGMBOXMAN_Init(void);

/*****************************************************************************/
//  Description : Register acc menu group
//  Global resource dependence : none
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIGAME_BOXMAN_RegWinIdNameArr(void);

#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : register box game module applet info
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIGMBOX_RegAppletInfo(void);

#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif  

