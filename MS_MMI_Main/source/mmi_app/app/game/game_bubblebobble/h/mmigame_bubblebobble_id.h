/****************************************************************************
** File Name:      mmigame_bubblebobble_id.h                                              *
** Author:                                                                 *
** Date:             2012/06/01                                            *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, applicaiton, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                   NAME             DESCRIPTION                             *
** 2006/06/01       junxue.zhu           Create
** 
****************************************************************************/
#ifndef  _MMIGAME_QUINTET_ID_H_    
#define  _MMIGAME_QUINTET_ID_H_  
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
 /**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "mmigame_bubblebobble_menutable.h"
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
	GMBUBBLEBOBBLE_WIN_ID_START = (MMI_MODULE_GAME_BUBBLEBOBBLE << 16),    

#include "mmigame_bubblebobble_id.def"
			
		GMQUT_MAX_WIN_ID
}GMBUBBLEBOBBLE_WINDOW_ID_E;


#undef WIN_ID_DEF

// 命名规则: 模块名 + 窗口名 + WIN_ID  ,  eg: CC_DIALING_WIN_ID (CC部分呼叫窗口ID)
typedef enum
{
    GMBUBBLEBOBBLE_CTRL_ID_ID_START = GMQUT_MAX_WIN_ID, //(MMI_MODULE_GAME_BOXMAN << 16),  
    MMIGMBUBBLEBOBBLE_MAIN_MENU_CTRL_ID,
    MMIGAME_BUBBLEBOBBLE_LABEL_SCORE_CTRL_ID,
    MMIGAME_BUBBLEBOBBLE_LABEL_LEVEL_CTRL_ID,
    MMIGAME_BUBBLEBOBBLE_FORM_CTRL_ID,
    MMIGAME_BUBBLEBOBBLE_FORM1_CTRL_ID,
    MMIGAME_BUBBLEBOBBLE_LABEL1_CTRL_ID,
    MMIGAME_BUBBLEBOBBLE_EDIT1_CTRL_ID,
    MMIGMBUBBLEBOBBLE_SCORE_CTRL_ID
}GMBUBBLEBOBBLE_CTRL_ID_T;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
#endif
