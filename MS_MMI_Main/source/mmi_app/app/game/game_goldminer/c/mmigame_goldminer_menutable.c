/***************************************************************************
** File Name:      mmigame_goldminer_menutable.c                           *
** Author:                                                                 *
** Date:           06/09/2012                                              *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    define the menu and related information for game.       *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                         *
** 06/09/2012                          Create                              *
****************************************************************************/
#define _MMIGAME_GOLDMINER_MENUTABLE_C_ 

#include "std_header.h"
#ifdef GAME_GOLDMINER_SUPPORT
#include "window_parse.h"
#include "mmigame_goldminer_text.h"
#include "mmigame_goldminer_menutable.h"
#include "mmigame_goldminer_image.h"
#include "guimenu.h"
#include "mmi_image.h"
#include "mmi_modu_main.h"
#include "mmigame_id.h"
#include "mmi_module.h"
#include "mmigame_menutable.h"

const GUIMENU_ITEM_T  menu_goldminer[] =
{
    // 新游戏
    {ID_GMGOLDMINER_NEW, TIP_NULL, {STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN},  TXT_GOLDMINER_NEW_GAME,  0, 0, 0, 0},
    // 查看分数
    {ID_GMGOLDMINER_SCORE_VIEW, TIP_NULL, {STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN}, TXT_GOLDMINER_HIGHRECORD, 0, 0, 0, 0},
    // 设置
    {ID_GMGOLDMINER_LEVEL, TIP_NULL, {STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN}, TXT_GOLDMINER_LEVEL,  0, 0, 0,  0},
    {ID_GMGOLDMINER_INFO, TIP_NULL, {STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN},  TXT_GOLDMINER_HELP,  0, 0, 0,  0},
};


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T game_goldminer_menu_table[] = 
{
    {NULL},
    #include "mmigame_goldminer_menutable.def"
};
#undef  MENU_DEF

/*****************************************************************************/
//  FUNCTION:      reg nenu group
//  Description :   
//  Global resource dependence : 
//  Author:  
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMGOLDMINER_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_GAME_GOLDMINER, game_goldminer_menu_table);
}

#endif
