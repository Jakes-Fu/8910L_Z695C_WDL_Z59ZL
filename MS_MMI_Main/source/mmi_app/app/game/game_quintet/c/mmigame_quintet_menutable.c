/****************************************************************************
** File Name:      mmigame_quintet_menutable.c                            
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
#define _MMIGAME_QUINTET_MENUTABLE_C_  
#ifdef GAME_QUINTET_SUPPORT

#include "std_header.h"
#include "window_parse.h"
#include "guimenu.h"
#include "mmigame_quintet_text.h"
#include "mmi_image.h"
#include "mmigame_quintet_menutable.h"
#include "mmi_modu_main.h"
#include "mmigame_id.h"
#include "mmi_module.h"
#include "mmigame_menutable.h"

const GUIMENU_ITEM_T  menu_quintet[] =
{
    {ID_GMQUT_NEW_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_GMQUT_NEW_GAME,  0, 0, 0, 0},
    {ID_GMQUT_RES_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_GMQUT_CONTINUE,  0, 0, 0,  0},
    {ID_GMQUT_SET_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_GMQUT_SETTING,  0, 0, 0, 0},
    {ID_GMQUT_INFO_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_GMQUT_INFO,  0, 0, 0,  0},
    {ID_GMQUT_LAST_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_GMQUT_LAST,  0, 0, 0,  0},
};



#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T game_quintet_menu_table[] = 
{
    {NULL},
    #include "mmigame_quintet_menutable.def"
};
#undef  MENU_DEF

/*****************************************************************************/
//   FUNCTION:      reg quintet nenu group
// 	Description :   
//
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIGMQUT_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_GAME_QUINTET, game_quintet_menu_table);
}
#endif

