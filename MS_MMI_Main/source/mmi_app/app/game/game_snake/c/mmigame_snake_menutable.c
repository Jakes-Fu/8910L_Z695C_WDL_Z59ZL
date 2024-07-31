/****************************************************************************
** File Name:      mmigame_snake_menutable.c                            
** Author:          peilong.zhou                                                              
** Date:            26/01/2011
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to define the menu and related information for game.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 26/01/2011     peilong.zhou          Create
** 
****************************************************************************/
#define _MMIGAME_SNAKE_MENUTABLE_C_  


#include "std_header.h"
#ifdef GAME_SNAKE_SUPPORT
#include "window_parse.h"
#include "guimenu.h"
#include "mmigame_snake_text.h"
#include "mmi_image.h"
#include "mmigame_snake_menutable.h"
#include "mmi_modu_main.h"
#include "mmigame_id.h"
#include "mmi_module.h"
#include "mmigame_menutable.h"
#include "mmigame_snake_image.h"


const GUIMENU_ITEM_T  menu_snake[] =
{
    // 开始游戏
    {ID_GMSNAKE_NEW,              TIP_NULL, {STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN},  TXT_SNAKE_NEW_GAME,  0, 0, 0, 0},
    // 查看最高分
    {ID_GMSNAKE_SCORE_VIEW,  TIP_NULL, {STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN},   TXT_SNAKE_HIGHSCORE,  0, 0, 0, 0},
    // 难得设置
    {ID_GMSNAKE_LEVEL,           TIP_NULL, {STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN},   TXT_SNAKE_LEVEL,  0, 0, 0,  0},
    {ID_GMSNAKE_INFO,             TIP_NULL, {STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN},   TXT_SNAKE_GAME_INFO,  0, 0, 0,  0},
};


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T game_snake_menu_table[] = 
{
    {NULL},
    #include "mmigame_snake_menutable.def"
};
#undef  MENU_DEF

/*****************************************************************************/
//   FUNCTION:      reg quintet nenu group
//  Description :   
//  Global resource dependence : 
//   Author:        peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMSNAKE_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_GAME_SNAKE, game_snake_menu_table);
}

#endif
