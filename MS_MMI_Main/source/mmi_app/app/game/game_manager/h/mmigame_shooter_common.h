/********************************************************************************************
** File Name:    mmigame_shooter_common.h                                                        *
** Author:         michael.wang                                                                     *
** Date:            6/4/2012                                                                              *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is the common part of shooting game framwork *
**********************************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME             DESCRIPTION                               *
** 4/6/2012        michael.wang         Create
******************************************************************************/

#ifndef _MMIGAME_SHOOTER_COMMON_H_
#define _MMIGAME_SHOOTER_COMMON_H_

#include "std_header.h"
#include "window_parse.h"
#include "mmk_app.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

//entity state of shooting game 
typedef enum 
{
    MMI_GAME_ENTITY_NOT_USE,            //no use
    MMI_GAME_ENTITY_NORMAL,             //normal
    MMI_GAME_ENTITY_READY,              //ready
    MMI_GAME_ENTITY_MOTION,             //runing or working
    MMI_GAME_ENTITY_HIT,                //hit
    MMI_GAME_ENTITY_NOCATCH,            //add for goldminer
    MMI_GAME_ENTITY_END,                //work end
    MMI_GAME_ENTITY_STATE_MAX
}MMI_GAME_ENTITY_STATE_E;

//shooter state of shooting game 
typedef enum 
{
    MMI_GAME_SHOOTER_READY,                //ready
    MMI_GAME_SHOOTER_SHOOT,              //runing or working
    MMI_GAME_SHOOTER_STATE_MAX
}MMI_GAME_SHOOTER_STATE_E;

//msgs of shooting game,use for msg exchange of MVC mode
typedef enum 
{
    MMI_GAME_GRID_CHANGE,                //grid change
    MMI_GAME_GRID_DRAW,                   //grid draw
    MMI_GAME_MSG_MAX
}MMI_GAME_MSG_TYPE_E;

//game state
typedef enum 
{
    MMI_GAME_START,         //game start
    MMI_GAME_PLAY,          //game playing
    MMI_GAME_PAUSE,         //game pause
    MMI_GAME_LEVEL_DONE,    //game level done (current level done)
    MMI_GAME_OVER,          //game over (current level fail)
    MMI_GAME_FINISH,        //game finish ( current level )
    MMI_GAME_STATE_MAX
}MMI_GAME_STATE_E;

//events of shooting game,you can add 
/*
typedef enum 
{
    MMI_GAME_EVENT_NONE,
    MMI_GAME_EVENT_TIMEOUT,
    MMI_GAME_EVENT_MAX
}MMI_GAME_EVENT_E;
*/
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
    uint16      cell_height;
    uint16      cell_weight;
} MMI_GAME_GRID_T;

typedef struct 
{
  GUI_POINT_T object_pos;   //object position
  uint16      max_distance; //max object distance
  uint16      min_distance; //min object distance
  MMI_IMAGE_ID_T image_id;  
} MMI_GAME_ENTITY_T;

//struct --> shooter
typedef struct
{
    uint8       object_speed;     
    uint16      move_speed;
    uint16      angle; 
    GUI_POINT_T shooter_pos;  //shooter position
    MMI_IMAGE_ID_T image_id;  
    MMI_GAME_SHOOTER_STATE_E    shooter_status;
    MMI_GAME_ENTITY_STATE_E     object_status; 
    MMI_GAME_ENTITY_T     object;
} MMI_GAME_SHOOTER_T;


//struct --> game info
typedef struct
{
    uint8   level;
    uint32  time;
    MMI_GAME_STATE_E    status;
    MMI_GAME_SCORE_T    score;    
    MMI_GAME_GRID_T     grid;    
    MMI_GAME_SHOOTER_T  shooter;
} MMI_GAME_INFO_T;

//struct --> cell
typedef struct
{
    uint8   type;
    uint8   groupid;
} MMI_GAME_CELL_T;

//struct --> cell
typedef struct
{
    uint16   high_level;
    uint16   time;
} MMI_GAME_RECORD_T;

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/******************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:   input:  angle
//          output: sin(angle) * PRCISION
/*****************************************************************************/
PUBLIC float SIN_S(int16 angle);

/******************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:   input:  angle
//          output: sin(angle) * PRCISION
/*****************************************************************************/
PUBLIC float COS_S(int16 angle);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

