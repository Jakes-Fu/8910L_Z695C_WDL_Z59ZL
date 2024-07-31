/****************************************************************************
** File Name:      mmigame_id.h                                               *
** Author:                                                                 *
** Date:           03/29/2006                                             *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, applicaiton, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006        Annie.an        Create
** 
****************************************************************************/
#ifndef  _MMIGAME_ID_H_    
#define  _MMIGAME_ID_H_  
#ifdef GAME_SUPPORT
 /**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "mmigame_menutable.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
   extern   "C"
   {
#endif

#define WIN_ID_DEF(win_id)          win_id

typedef enum
{
	 GAME_WIN_ID_START = (MMI_MODULE_GAME_MANAGER<< 16),       

#include "mmigame_id.def"

     GAME_MAX_WIN_ID
}GAME_WINDOW_ID_E;

#undef WIN_ID_DEF


// 命名规则: 模块名 + 窗口名 + WIN_ID  ,  eg: CC_DIALING_WIN_ID (CC部分呼叫窗口ID)
typedef enum
{ 
       GAME_CTRL_ID_ID_START = GAME_MAX_WIN_ID, //(MMI_MODULE_GAME_MANAGER << 16),
       MMIGAME_MAIN_MENU_CTRL_ID,
	   MMIGAME_STATIC_TEXT_CTRL_ID, 
       MMIGAME_STATIC_LABEL_CTRL_ID,
	   MMIGAME_STATIC_LABEL_TITLE_CTRL_ID,
	   MMIGAME_SETMUSIC_CTRL_ID,
	   GAME_MAX_CTRL_ID
}GAME_CTRL_ID_T;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
#endif


