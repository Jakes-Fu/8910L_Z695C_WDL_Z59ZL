/********************************************************************************************
** File Name:    mmigame_common.h                                                        *
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

#ifndef _MMIGAME_GOLDMINER_MODE_H_
#define _MMIGAME_GOLDMINER_MODE_H_

#include "mmigame_shooter_common.h"
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
//cell type of gold miner
typedef enum 
{
    MMIGM_GOLDMINER_CELLTYPE_MINI_GOLD,                     
    MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD,             
    MMIGM_GOLDMINER_CELLTYPE_MINI_STONE,                 
    MMIGM_GOLDMINER_CELLTYPE_BIG_STONE,	
    MMIGM_GOLDMINER_CELLTYPE_DIAMOND,
    MMIGM_GOLDMINER_CELLTYPE_BOMB,
    MMIGM_GOLDMINER_CELLTYPE_ENERGY,
    MMIGM_GOLDMINER_CELLTYPE_MAX
}MMIGM_GOLDMINER_CELL_TYPE_E;

//timer type
typedef enum 
{
    MMIGM_GOLDMINER_TIMER_SWING, 
    MMIGM_GOLDMINER_TIMER_STRETCH, 
    MMIGM_GOLDMINER_TIMER_COUNTED,
    MMIGM_GOLDMINER_TIMER_MAX
}MMIGM_GOLDMINER_TIMER_E;

//entity event
typedef enum
{
	  EVENT_NONE,
	  EVENT_START,       //启动
	  EVENT_HITOBJECT,   //碰撞抓取
	  EVENT_OUTRANGE,    //到达最大范围
	  EVENT_REATCHTOP,   //回到顶层矿工处
	  EVENT_MAX
}MMI_ENTITY_EVENT_E;

//game event
typedef enum
{
	  GAME_EVENT_NONE,
	  GAME_EVENT_LOSE_FOCUS,  //失焦
	  GAME_EVENT_GET_FOCUS,   //聚焦
         GAME_EVENT_TIME_OUT,    //时间到
         GAME_EVENT_MAX
}MMI_GAME_EVENT_E;

typedef struct
{
    uint8 bomb_num_cur;     //当前数
    uint8 energy_num_cur;
    uint8 bomb_num_last;    //上一关数，当过关失败时，用于恢复到之前的Tool num
    uint8 energy_num_last;
    BOOLEAN tool_speed_up;
} MMI_GAME_GOLDMINER_TOOLS_T;


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : get cell info
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_GAME_CELL_T  **MMIGOLDMINER_GetGridCellInfoM(void);

/*****************************************************************************/
//  Description : get grid info
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_GAME_GRID_T  *MMIGOLDMINER_GetGridInfoM(void);
/*****************************************************************************/
//  Description : get shooter info
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_GAME_INFO_T *MMIGOLDMINER_GetGameInfoM(void);
/*****************************************************************************/
//  Description : 设置倒计时
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 每次timer到减一操作
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_UpdateCountDownTimeM(void);
/*****************************************************************************/
//  Description : 获取倒计时
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: View中用来显示
/*****************************************************************************/
PUBLIC uint32 MMIGOLDMINER_GetCountDownTimeM(void);
/*****************************************************************************/
//  Description : 计算当前得分
//  Global resource dependence : 
//  Author: Michael.Wang
//  Note: 
/*****************************************************************************/    
PUBLIC void MMIGOLDMINER_UpdateCurScoreAndToolsM(void);
/*****************************************************************************/
//  Description : 检测当前level 是否达到完成条件
//  Global resource dependence : 
//  Author: Michael.Wang
//  Note: 
/*****************************************************************************/   
PUBLIC BOOLEAN MMIGOLDMINER_CheckLevelFinishEnableM(void);
/*****************************************************************************/
//  Description : 初始化被捕获的object 信息
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: View中用来显示
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_SetMoveCellNoneM(void);
/*****************************************************************************/
//  Description : 获取bomb 数量
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: View中用来显示
/*****************************************************************************/
PUBLIC uint32 MMIGOLDMINER_GetBombNumM();
/*****************************************************************************/
//  Description : 设置bomb 数量
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: View中用来显示
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_UpdateBombNumM();
/*****************************************************************************/
//  Description : 获取energy 数量
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: View中用来显示
/*****************************************************************************/
PUBLIC uint32 MMIGOLDMINER_GetEnergyNumM();
/*****************************************************************************/
//  Description : 设置energy 数量
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: View中用来显示
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_UpdateEnergyNumM();
/*****************************************************************************/
//  Description : 判断Game是否通关
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIGOLDMINER_IsGameFinishM();
/*****************************************************************************/
//  Description : 通关后重新玩
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_ReplayWithGameFinishM();
/*****************************************************************************/
//  Description : 判断是否可以进入下一关
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 是否进入的条件由参数传入
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_GoToNextLevelM(BOOLEAN is_next_level);
/*****************************************************************************/
//  Description : 根据移动的point判断当前移动到哪个网格中
//  Global resource dependence : 
//  Author: Michael.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_GetCellRowColByPoint(const GUI_POINT_T point, uint16 *row, uint16 *col);

/*****************************************************************************/
//  Description : init gold miner
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_InitGameM(void);

/*****************************************************************************/
//  Description : init gold miner
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_ExitGameM(void);

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_StartTimerM(MMIGM_GOLDMINER_TIMER_E timer_type);

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_StopTimerM(MMIGM_GOLDMINER_TIMER_E timer_type);

/*****************************************************************************/
//  Description : get swing timer
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC MMIGM_GOLDMINER_TIMER_E MMIGOLDMINER_GetTimerTypeM(uint8 timer);

/*****************************************************************************/
//  Description : set the miner/clippers state
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_SetClipperStateM(MMI_GAME_ENTITY_STATE_E state);

/*****************************************************************************/
//  Description : get the miner/clippers state
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_GAME_ENTITY_STATE_E MMIGOLDMINER_GetClipperStateM(void);

/*****************************************************************************/
//  Description : set the game state
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_SetGameStateM(MMI_GAME_STATE_E state);

/*****************************************************************************/
//  Description : get the game state
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_GAME_STATE_E MMIGOLDMINER_GetGameStateM(void);

/*****************************************************************************/
//  Description : get current event
//  Global resource dependence : 
//  Author: Michael.Wang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIGOLDMINER_GetCatchedObjectImgM(void);

/*****************************************************************************/
//  Description : get current event
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC  MMI_ENTITY_EVENT_E MMIGOLDMINER_GetEventM(void);

/*****************************************************************************/
//  Description : get shooter info
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_GAME_SHOOTER_T *MMIGOLDMINER_GetShooterInfoM();

/*****************************************************************************/
//  Description : stretch clipper
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_ClippersMoveM(
                               BOOLEAN is_down,
                               BOOLEAN is_with_target
                               );

/*****************************************************************************/
//  Description : swing clippers
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_SwingM(void);
/*****************************************************************************/
//  Description : 设置object 移动工具是否使用
//  Global resource dependence : 
//  Author: Michael.Wang
//  Note: "加速药水"
/*****************************************************************************/    
PUBLIC void MMIGOLDMINER_SetClippersMoveWithEnergyM(BOOLEAN is_speed_up);
/*****************************************************************************/
//  Description : 获取object 移动工具是否使用
//  Global resource dependence : 
//  Author: Michael.Wang
//  Note: "加速药水"
/*****************************************************************************/   
PUBLIC BOOLEAN MMIGOLDMINER_GetClippersSpecMoveM();

 PUBLIC uint8 MMIGOLDMINER_GetMinerTimesM();

 PUBLIC void MMIGOLDMINER_UpdateRecordM();

 PUBLIC void MMIGOLDMINER_SetGameInfoByLevelM(uint32 level);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

