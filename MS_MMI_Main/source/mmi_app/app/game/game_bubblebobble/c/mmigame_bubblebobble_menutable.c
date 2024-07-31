/****************************************************************************
** File Name:      mmigame_bubblebobble_menutable.c                            
** Author:          junxue.zhu                                                             
** Date:            2012/06/01
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to define the menu and related information for game.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 01/06/2006     junxue.zhu          Create
** 
****************************************************************************/
#define _MMIGAME_BUBBLEBOBBLE_MENUTABLE_C_  
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
#include "window_parse.h"
#include "guimenu.h"
#include "mmigame_link_text.h"
#include "mmi_image.h"
#include "mmigame_bubblebobble_menutable.h"
#include "mmi_modu_main.h"
#include "mmigame_id.h"
#include "mmi_module.h"
#include "mmigame_menutable.h"
#include "mmigame_link_image.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 
 /**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
 const GUIMENU_ITEM_T  menu_bubblebobble[] =
 {
    {ID_GMBUBBLEBOBBLE_NEW,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_NEW_GAME,  0, 0, 0, 0},
//     {ID_GMBUBBLEBOBBLE_RES,   TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},   TXT_COMMON_COMMON_CONTINUE,  0, 0, 0,  0},
     {ID_GMBUBBLEBOBBLE_SELECT, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_GMBUBBLE_SELECT,  0, 0, 0, 0},
     {ID_GMBUBBLEBOBBLE_HIGHSCORE,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_BUBBLE_HIGHSCORE,  0, 0, 0,  0}
 };
// end of game menu 

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T game_bubblebobble_menu_table[] = 
{
    {NULL},
#include "mmigame_bubblebobble_menutable.def"
};
#undef  MENU_DEF
/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/
/*****************************************************************************/
//   FUNCTION:      reg quintet nenu group
// 	Description :   
//	Global resource dependence : 
//   Author:      junxue.zhu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIGMBUBBLEBOBBLE_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_GAME_BUBBLEBOBBLE, game_bubblebobble_menu_table);
}


#endif


