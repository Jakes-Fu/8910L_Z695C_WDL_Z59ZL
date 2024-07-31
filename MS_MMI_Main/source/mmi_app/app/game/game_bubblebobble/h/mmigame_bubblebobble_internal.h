/*****************************************************************************
** File Name:      mmigame_bubblebobble_internal.h                                                  *
** Author:                                                                   *
** Date:             01/06/2012                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe the internal api of game   bubble           *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                   NAME             DESCRIPTION                             *
** 2006/06/01       junxue.zhu           Create
** 
****************************************************************************/

#ifndef _MMIGAME_QUINTET_INTERNAL_H_
#define _MMIGAME_QUINTET_INTERNAL_H_
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "guicommon.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "mmigame_bubblebobble_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
 //entity state of shooting game 
typedef enum 
{
    MMI_GAME_ENTITY_NOT_USE,            //no use
    MMI_GAME_ENTITY_NORMAL,             //normal
    MMI_GAME_ENTITY_READY,                //ready
    MMI_GAME_ENTITY_MOTION,              //runing or working
    MMI_GAME_ENTITY_HIT,                      //hit
    MMI_GAME_ENTITY_END,	                //work end
    MMI_GAME_ENTITY_STATE_MAX
}MMI_GAME_ENTITY_STATE;

//shooter state of shooting game 
typedef enum 
{
    MMI_GAME_SHOOTER_READY,                //ready
    MMI_GAME_SHOOTER_SHOOT,              //runing or working
    MMI_GAME_SHOOTER_STATE_MAX
}MMI_GAME_SHOOTER_STATE;

//msgs of shooting game,use for msg exchange of MVC mode
typedef enum 
{
    MMI_GAME_GRID_CHANGE,                //grid change
    MMI_GAME_GRID_DRAW,                   //grid draw
    MMI_GAME_MSG_MAX
}MMI_GAME_MSG_TYPE;

//game state
typedef enum 
{
    MMI_GAME_START,                            //game start
    MMI_GAME_PAUSE,                            //game pause
    MMI_GAME_CONTINUE,                      //game contine
    MMI_GAME_RESUME,                         //game resume
    MMI_GAME_FINISH,                            //game finish ( current level )
    MMI_GAME_OVER,                              //game over
    MMI_GAME_STATE_MAX
}MMI_GAME_STATE;

//events of shooting game,you can add 
typedef enum 
{
    MMI_GAME_EVENT_NONE,
    MMI_GAME_EVENT_TIMEOUT,
    MMI_GAME_EVENT_MAX,		
}MMI_GAME_EVENT_E;

//struct --> score
typedef struct
{
    uint32  total_score;
    uint16  target_score;
    uint16  cur_score;
} MMI_GAME_SCORE_T;

//struct --> grid
typedef struct
{
    GUI_RECT_T  display_rect;
    uint16            cell_height;
    uint16            cell_weight;
} MMI_GAME_GRID_T;

//struct --> shooter
typedef struct
{
    uint8    object_speed;     
    uint16  move_speed;
    uint16  angle;  
    GUI_POINT_T position;
    MMI_GAME_SHOOTER_STATE  shooter_status;
    MMI_GAME_ENTITY_STATE  object_status;    
} MMI_GAME_SHOOTER_T;

//struct --> game info
typedef struct
{
    uint8   level;
    uint32 time;
    MMI_GAME_STATE            status;
    MMI_GAME_SCORE_T       score;    
    MMI_GAME_GRID_T          grid;    
    MMI_GAME_SHOOTER_T  shooter;
} MMI_GAME_INFO_T;

//struct --> cell
typedef struct
{
    uint8   type;
    uint8   groupid;
} MMI_GAME_CELL_T;
// 泡泡状态
typedef enum 
{
	BS_NotUse = 0,
		BS_Normal,
		BS_Move,
		BS_Moving,
		BS_CanPop,//可以爆炸
		BS_Poping,
		BS_CanDrop,//可以下落
		BS_CanNotDrop,
		BS_Droping,
		BS_Ready
}MMI_GAME_BUBBLESTATE;

//泡泡颜色
typedef enum 
{
	    BC_BLUE = 0,
		BC_GRAY,
		BC_GREEN,
		BC_ORANGE,
		BC_PINK,
		BC_RED,
		BC_YELLOW
}MMI_GAME_BUBBLECOLOR;

//定义泡泡的结构体
typedef struct{
	MMI_GAME_BUBBLECOLOR myColor;
	MMI_GAME_BUBBLESTATE myState;
	
	//泡泡坐标
	int32 my_X;
	int32 my_Y;
	
	//泡泡所在行列
	int32 row;
	int32 column;
	
	int16 speedX;
	int16 speedY;
	
}MMI_GAME_BUBBLE_T;

typedef struct 
{
    const void				*data_ptr;      
    uint32					data_size;
    IMGREF_DATA_FORMAT_E    data_type;      
    uint16					width;             
    uint16					height;
    int16					center_x;       //horizontal coordinate of rotation center 
    int16					center_y;       //vertical coordinate of rotation center 
}MMI_IMAGE_BUBBLE_T;

/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/
/*****************************************************************************/
//  FUNCTION:     MMIGMBUBBLEBOBBLE_AddTwoString
//  Description :   add two string 
//  Global resource dependence : 
//  Author:      junxue.zhu
//  Note:   
/*****************************************************************************/
PUBLIC MMI_STRING_T MMIGMBUBBLEBOBBLE_AddTwoString(
                MMI_STRING_T first, 
                MMI_STRING_T second
                );

/*****************************************************************************/
//   FUNCTION:      MMIGMBUBBLEBOBBLE_InitModule
//  Description :   
//  Global resource dependence : 
//   Author:        junxue.zhu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMBUBBLEBOBBLE_InitModule(void);

/*****************************************************************************/
//   FUNCTION:      MMIGMBUBBLEBOBBLE_Init
//   Description :   
//   Global resource dependence : 
//   Author:        junxue.zhu
//   Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIGMBUBBLEBOBBLE_Init(void);

/*****************************************************************************/
//  Description : Register 
//  Global resource dependence : none
//  Author: junxue.zhu
//  Note:
/*****************************************************************************/
PUBLIC void MMIGAME_BUBBLEBOBBLE_RegWinIdNameArr(void);

#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : register box game module applet info
//  Global resource dependence : 
//  Author:junxue.zhu
//  Note:
/*****************************************************************************/
PUBLIC void MMIGMBUBBLE_RegAppletInfo(void);

#endif

/*****************************************************************************/
//   FUNCTION:     MMIGMBUBBLE_AddTwoString
//  Description :    链接两个字串为一个
//
//  Global resource dependence : 
//   Author:      
//  Note:   
/*****************************************************************************/
PUBLIC MMI_STRING_T MMIGMBUBBLE_AddTwoString(
                                          MMI_STRING_T first, 
                                          MMI_STRING_T second
                                          );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif  

