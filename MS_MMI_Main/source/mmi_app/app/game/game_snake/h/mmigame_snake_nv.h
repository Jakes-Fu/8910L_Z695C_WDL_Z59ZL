/*************************************************************************
 ** File Name:      mmigame_snake_nv.h                                    *
 ** Author:         peilong.zhou                                           *
 ** Date:           26/01/2011                                          *
 ** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.   *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 26/01/2011  peilong.zhou        Create.                              *
*************************************************************************/

#ifndef _MMIGAME_SNAKE_NV_H_
#define _MMIGAME_SNAKE_NV_H_

#include "sci_types.h"
#ifdef GAME_SNAKE_SUPPORT
#include "mmi_module.h"

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
typedef enum
{  
    MMINV_GMSNAKE_LEVER_SET = MMI_MODULE_GAME_SNAKE<< 16,  //游戏难度保存
    MMINV_GMSNAKE_HIGH_SCORE,//游戏最高分保存
    MMINV_GMSNAKE_NV_ITEM_NUM
} GMSNAKE_NV_ITEM_E;

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif//_MMIGAME_SNAKE_NV_H_

