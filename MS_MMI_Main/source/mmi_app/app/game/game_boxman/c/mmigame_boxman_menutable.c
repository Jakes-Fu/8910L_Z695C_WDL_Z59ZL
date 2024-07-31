/****************************************************************************
** File Name:      mmigame_boxman_menutable.c                            
** Author:          Annie.an                                                              
** Date:            2006/04/01
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to define the menu and related information for game.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 04/01/2006     annie.an          Create
** 
****************************************************************************/
#define _MMIGAME_BOXMAN_MENUTABLE_C_  
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_BOXMAN_SUPPORT
#include "window_parse.h"
#include "guimenu.h"
#include "mmigame_link_text.h"
#include "mmi_image.h"
#include "mmigame_boxman_menutable.h"
#include "mmi_modu_main.h"
#include "mmigame_id.h"
#include "mmi_module.h"
#include "mmigame_menutable.h"
#include "mmigame_link_image.h"


const GUIMENU_ITEM_T  menu_boxman[] =
{
    {ID_GMBOXMAN_NEW,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_NEW_GAME,  0, 0, 0, 0},
    {ID_GMBOXMAN_RES,   TIP_NULL, {STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN},   TXT_COMMON_COMMON_CONTINUE,  0, 0, 0,  0},
    {ID_GMBOXMAN_SELECT,TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_GMBOXMAN_SELECT,  0, 0, 0, 0},
    {ID_GMBOXMAN_INFO,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_GAME_INFO,  0, 0, 0,  0}
};
// end of game menu 

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T game_boxman_menu_table[] = 
{
    {NULL},
#include "mmigame_boxman_menutable.def"
};
#undef  MENU_DEF

/*****************************************************************************/
//   FUNCTION:      reg quintet nenu group
// 	Description :   
//2
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIGMBOXMAN_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_GAME_BOXMAN, game_boxman_menu_table);
}


#endif


