/*****************************************************************************
** File Name:      mmigame_snake_internal.h                                                   *
** Author:                                                                   *
** Date:           26/01/2011                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe the internal api of game              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 26/01/2011  peilong.zhou      Create
******************************************************************************/

#ifndef _MMIGAME_SNAKE_INTERNAL_H_
#define _MMIGAME_SNAKE_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#ifdef GAME_SNAKE_SUPPORT
#include "os_api.h"
#include "mmk_type.h"
#include "guicommon.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "mmi_position.h"

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


/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
//游戏的难度，难度越高，蛇游动的速度越快
typedef enum
{
    SNAKE_LEVEL_EASY,
    SNAKE_LEVEL_NORMAL,
    SNAKE_LEVEL_HARD,
    SNAKE_TOTAL_LEVEL
}SNAKE_LEVEL_E;

//游戏结束状态
typedef enum
{
    SNAKE_GAME_NULL,
    SNAKE_GAME_ON,
    SNAKE_GAME_OVER,
    SNAKE_GAME_PAUSE,
    SNAKE_TOTAL_STATE
}SNAKE_GAME_STATE_E;

/*****************************************************************************/
//  FUNCTION:      MMIGMQUT_Init(void)
//  Description :   
//  Global resource dependence : 
//  Author:        peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIGMSNAKE_Init(void);

/*****************************************************************************/
//  FUNCTION:    MMIGMQUT_RegNv(void)
//  Description :   
//  Global resource dependence : 
//  Author:        peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMSNAKE_RegNv(void);

/*****************************************************************************/
//  FUNCTION:      MMIGMQUT_InitModule
//  Description :   
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMSNAKE_InitModule(void);

/*****************************************************************************/
//  FUNCTION:      reg quintet nenu group
//  Description :   
//  Global resource dependence : 
//  Author:        peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMSNAKE_RegMenuGroup(void);

/*****************************************************************************/
//  FUNCTION:     MMIGMSNAKE_AddTwoString
//  Description :   
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC MMI_STRING_T MMIGMSNAKE_AddTwoString(MMI_STRING_T first,MMI_STRING_T second);

/*****************************************************************************/
//  FUNCTION:     Snake_AllocGameSpace
//  Description : 分配游戏需要的内存空间
//  Global resource dependence : 
//  Author:       leon.wang
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN Snake_AllocGameSpace(void);

/*****************************************************************************/
//  FUNCTION:     Snake_InitGame
//  Description :   
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN Snake_InitGame(void);

/*****************************************************************************/
//  FUNCTION:     Snake_ExitGame
//  Description :   结束游戏
//  Global resource dependence : 
//  Author:       leon.wang
//  Note:   
/*****************************************************************************/
PUBLIC void Snake_ExitGame(void);

/*****************************************************************************/
//  Description : MMIGMQUT_OpenGameWin      
//  Global resource dependence : 
//  Author:        peilong.zhou
//  RETRUN:   
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIGMSNAKE_OpenGameWin(void);

/*****************************************************************************/
//  FUNCTION:     MMIGMSNAKE_RsetGameDate
//  Description :   
//  Global resource dependence : 
//  Author:      
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMSNAKE_RsetGameData(void);
/*****************************************************************************/
//  FUNCTION:     Snake_StartGameLinks
//  Description :   
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC void Snake_GameGoing(
                            MMI_WIN_ID_T win_id
                            );

/*****************************************************************************/
//  Description : Register acc menu group
//  Global resource dependence : none
//  Author:   peilong.zhou
//  Note:
/*****************************************************************************/
PUBLIC void MMIGAME_SNAKE_RegWinIdNameArr(void);

/*****************************************************************************/
//  FUNCTION:     Snake_SaveGameScore
//  Description :  
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC void Snake_SaveGameScore(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif//_MMIGAME_SNAKE_INTERNAL_H_

