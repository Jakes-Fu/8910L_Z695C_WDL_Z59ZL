/***************************************************************************
** File Name:      mmigame_goldminer_id.h                                  *
** Author:                                                                 *
** Date:           06/09/2012                                              *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:    This file is used to describe the data struct of        *
**                 system, applicaiton, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/09/2012                      Create                                  *             
****************************************************************************/
#ifndef  _MMIGAME_GOLDMINER_ID_H_    
#define  _MMIGAME_GOLDMINER_ID_H_  

 /**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#ifdef GAME_GOLDMINER_SUPPORT
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "mmigame_goldminer_menutable.h"
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
    GOLDMINER_WIN_ID_START = (MMI_MODULE_GAME_GOLDMINER << 16),
    #include "mmigame_goldminer_id.def"
    GOLDMINER_MAX_WIN_ID
}GOLDMINER_WINDOW_ID_E;

#undef WIN_ID_DEF


// 命名规则: 模块名 + 窗口名 + CTRL_ID  
typedef enum
{
    MMIGMGOLDMINER_CTRL_ID_START = GOLDMINER_MAX_WIN_ID, //(MMI_MODULE_GAME_GOLDMINER << 16),      
    // 主界面
    MMIGMGOLDMINER_MAINMENU_CTRL_ID,
    // 设置难度界面
    MMIGMGOLDMINER_SETLEVEL_LABEL_CTRL_ID,
    MMIGMGOLDMINER_SETLEVEL_EDITBOX_CTRL_ID,
    // 查看最高分界面
    //MMIGMGOLDMINER_SCORE_CTRL_ID,
    MMIGMGOLDMINER_RECORD_LEVEL_CTRL_ID,
    MMIGMGOLDMINER_RECORD_TIME_CTRL_ID,
    // 游戏帮助界面
    MMIGMGOLDMINER_HELP_CTRL_ID,
    
    MMIGMGOLDMINER_CTRL_ID_MAX
}GMGOLDMINER_CTRL_ID_T;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
#endif//_MMIGAME_GOLDMINER_ID_H_

