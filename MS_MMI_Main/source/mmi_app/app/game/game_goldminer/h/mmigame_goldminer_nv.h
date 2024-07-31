/*************************************************************************
 ** File Name:      mmigame_goldminer_nv.h                               *
 ** Author:                                                              *
 ** Date:           06/09/2012                                           *
 ** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.   *
 ** Description:    This file defines the function about nv              *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 06/09/2012                      Create.                              *
*************************************************************************/

#ifndef _MMIGAME_GOLDMINER_NV_H_
#define _MMIGAME_GOLDMINER_NV_H_

#include "sci_types.h"
#ifdef GAME_GOLDMINER_SUPPORT
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
    MMINV_GMGOLDMINER_LEVEL_SET = MMI_MODULE_GAME_GOLDMINER<< 16,  //游戏难度保存
    MMINV_GMGOLDMINER_HIGH_SCORE,//游戏最高分保存
    MMINV_GMGOLDMINER_NV_ITEM_NUM
} GMGOLDMINER_NV_ITEM_E;

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
#endif//_MMIGAME_GOLDMINER_NV_H_

