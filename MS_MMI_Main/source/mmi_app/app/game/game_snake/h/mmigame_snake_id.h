/****************************************************************************
** File Name:      mmigame_quintet_id.h                                               *
** Author:                                                                 *
** Date:           26/01/2011                                             *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, applicaiton, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 26/01/2011  peilong.zhou        Create
** 
****************************************************************************/
#ifndef  _MMIGAME_SNAKE_ID_H_    
#define  _MMIGAME_SNAKE_ID_H_  

 /**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#ifdef GAME_SNAKE_SUPPORT
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "mmigame_snake_menutable.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
   extern   "C"
   {
#endif

#define WIN_ID_DEF(win_id, win_id_name)          win_id,

typedef enum
{
    SNAKE_WIN_ID_START = (MMI_MODULE_GAME_SNAKE << 16),
    #include "mmigame_snake_id.def"
    SNAKE_MAX_WIN_ID
}SNAKE_WINDOW_ID_E;

#undef WIN_ID_DEF


// 命名规则: 模块名 + 窗口名 + WIN_ID  ,  eg: CC_DIALING_WIN_ID (CC部分呼叫窗口ID)
typedef enum
{
    MMIGMSNAKE_CTRL_ID_START = SNAKE_MAX_WIN_ID, //(MMI_MODULE_GAME_SNAKE << 16),      
    // 主界面
    MMIGMSNAKE_MAINMENU_CTRL_ID,
    // 设置难度界面
    MMIGMSNAKE_SETLEVEL_CTRL_ID,
    // 查看最高分界面
    MMIGMSNAKE_SCORE_CTRL_ID,
	// 查看最高分界面
    MMIGMSNAKE_SCORETITLE_CTRL_ID,
    // 游戏帮助界面
    MMIGMSNAKE_HELP_CTRL_ID,
    
    MMIGMSNAKE_CTRL_ID_MAX
}GMSNAKE_CTRL_ID_T;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
#endif//_MMIGAME_SNAKE_ID_H_

