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
#ifndef  _MMIGAME_QUINTET_ID_H_    
#define  _MMIGAME_QUINTET_ID_H_  
#ifdef GAME_BOXMAN_SUPPORT
 /**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "mmigame_boxman_menutable.h"
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
	GMBOXMAN_WIN_ID_START = (MMI_MODULE_GAME_BOXMAN << 16),    

#include "mmigame_boxman_id.def"
			
		GMQUT_MAX_WIN_ID
}GMBOXMAN_WINDOW_ID_E;


#undef WIN_ID_DEF

// 命名规则: 模块名 + 窗口名 + WIN_ID  ,  eg: CC_DIALING_WIN_ID (CC部分呼叫窗口ID)
typedef enum
{
    GMBOXMAN_CTRL_ID_ID_START = GMQUT_MAX_WIN_ID, //(MMI_MODULE_GAME_BOXMAN << 16),  
    MMIGMBOXMAN_MAIN_MENU_CTRL_ID,
    MMIGAME_BOXMAN_LAST_MAP_BUT_CTRL_ID,
    MMIGAME_BOXMAN_NEXT_MAP_BUT_CTRL_ID,
    MMIGAME_BOXMAN_GO_BACK_BUT_CTRL_ID,
    MMIGAME_BOXMAN_MAP_NUM_LABEL_CTRL_ID,       //  the label control for boxman map num
    MMIGAME_BOXMAN_SELECT_MAP_NUM_INPUT_CTRL_ID,        //the map num input control
    MMIGAME_BOXMAN_SELECT_MAP_TIP_LABEL_CTRL_ID,    //the select map tip label
    MMIGAME_BOXMAN_SELECT_CONFIRM_BUT_CTRL_ID,   //选关确定button
    MMIGAME_BOXMAN_SELECT_CANCEL_BUT_CTRL_ID, 
    MMIGAME_STATIC_TEXT_CTRL_ID,//选关取消button
    MMIGAME_BOXMAN_FORM_CTRL_ID,
    MMIGAME_BOXMAN_FORM1_CTRL_ID,
    MMIGAME_BOXMAN_LABEL1_CTRL_ID,
    MMIGAME_BOXMAN_EDIT1_CTRL_ID,
    MMIGAME_BOXMAN_BUTTON_CTRL_ID,
    GMBOXMAN_MAX_CTRL_ID
}GMBOXMAN_CTRL_ID_T;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
#endif
