/****************************************************************************
** File Name:      mmigame_quintet_id.h                                               *
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
#ifndef  _MMIGAME_LINK_ID_H_    
#define  _MMIGAME_LINK_ID_H_  

 /**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#ifdef GAME_LINK_SUPPORT
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "mmigame_link_menutable.h"
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
	   LINKS_WIN_ID_START = (MMI_MODULE_GAME_LINK << 16),    

#include "mmigame_link_id.def"

       LINKS_MAX_WIN_ID
}LINK_WINDOW_ID_E;

#undef WIN_ID_DEF


// 命名规则: 模块名 + 窗口名 + WIN_ID  ,  eg: CC_DIALING_WIN_ID (CC部分呼叫窗口ID)
typedef enum
{
       GMLINK_CTRL_ID_ID_START = LINKS_MAX_WIN_ID, //(MMI_MODULE_GAME_LINK << 16),      
       MMIGMLINK_MAIN_MENU_CTRL_ID,
	   MMIGAME_SCORE_MSGBOX_CTRL_ID,
	   MMIGMLINK_SETCLASS_CTRL_ID,
	   MMIGMLINK_STATIC_TEXT_CTRL_ID,
	   MMILINK_STATIC_TEXT_CTRL_ID,
	   MMIGMLINK_BUTTON_CTRL_ID,
	   MMIGAME_LINKS_SCORE_LABEL_CTRL_ID
}GMLINK_CTRL_ID_T;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
#endif


