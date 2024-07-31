/****************************************************************************
** File Name:      game_menutable.c                            
** Author:    Annie.an                                                              
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
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_SUPPORT
#define _GAME_MENUTABLE_C_  



#include "window_parse.h"
#include "guimenu.h"
#include "mmigame_text.h"
#include "mmi_image.h"
#include "mmigame_menutable.h"
#include "mmi_modu_main.h"
#include "mmigame_id.h"
#include "mmi_module.h"
//#include "mmigame_link_menutable.h"
#include "mmigame_link_image.h"
#include "mmigame_boxman_menutable.h"
#include "mmigame_boxman_image.h"
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
#include "mmigame_bubblebobble_menutable.h"
#include "mmigame_bubblebobble_image.h"
#include "mmigame_bubblebobble_text.h"
#endif
#include "mmi_menutable.h"
#include "mmigame_internal.h"
#ifdef GAME_LINK_SUPPORT
#include "mmigame_link_text.h"
#endif
#ifdef GAME_SNAKE_SUPPORT
#include "mmigame_snake_text.h"
#endif
#ifdef GAME_TETRIS_SUPPORT
#include "mmigame_square_text.h"
#endif
#ifdef GAME_GOLDMINER_SUPPORT
#include "mmigame_goldminer_text.h"
#endif
//michael wang for5
#include "mmigame_quintet_text.h"
//michael wang 
//============== MET MEX start ===================
#ifdef MET_MEX_SUPPORT
#include "met_mex_text.h"
#include "met_mex_image.h"
#endif // MET_MEX_SUPPORT
//============== MET MEX start ===================
// begin of game menu 
#if defined (MAINLCD_SIZE_128X160) || defined (MAINLCD_SIZE_176X220)
const GUIMENU_ITEM_T  menu_gm_list[] =
{
#if (defined(TOUCH_PANEL_SUPPORT)&& defined(GAME_LINK_SUPPORT))
    {IDGAME_MENU_LINK_ITEM_ID,  TIP_NULL, {STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN},TXT_COMMON_GAME_LINK,   0,   0,0,0},
#endif
#ifdef GAME_BOXMAN_SUPPORT
    {IDGAME_MENU_BOXMAN_ITEM_ID,TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_BOXMAN,  0,  0,0,0},
#endif
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
    {IDGAME_MENU_BUBBLEBOBBLE_ITEM_ID,TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_BUBBLEBOBBLE,  0,  0,0,0},
#endif
//michael wang for5
#ifdef GAME_QUINTET_SUPPORT
      {IDGAME_MENU_QUT_ITEM_ID,TIP_NULL, {STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN},TXT_GAME_QUINTET, 0 ,  0,0,0},
#endif

#if defined(GAME_SNAKE_SUPPORT) 
    {IDGAME_MENU_SNAKE_ITEM_ID,  TIP_NULL,{STXT_SOFTKEY_OK_MK,TXT_NULL,STXT_RETURN},TXT_SNAKE,   0,   0,0,MENU_NULL},
#endif
	
#if defined(GAME_TETRIS_SUPPORT) 
    {IDGAME_MENU_SQUARE_ITEM_ID,  TIP_NULL,{STXT_SOFTKEY_OK_MK,TXT_NULL,STXT_RETURN},TXT_GMSQU_TETRIS, 0,   0,0,MENU_NULL},
#endif

#if defined(GAME_GOLDMINER_SUPPORT)
    {IDGAME_MENU_GOLDMINER_ITEM_ID,  TIP_NULL,{STXT_SOFTKEY_OK_MK,TXT_NULL,STXT_RETURN},TXT_GOLDMINER,   0,   0,0,MENU_NULL},
#endif

#if (!defined(GAME_BOXMAN_SUPPORT) && !defined(GAME_LINK_SUPPORT) && !defined(GAME_TETRIS_SUPPORT) && !defined(GAME_SNAKE_SUPPORT) && !defined(GAME_QUINTET_SUPPORT))
    {0,0, {TXT_NULL, TXT_NULL, TXT_NULL},0,  0,  0,0,0}
#endif
};
#else
const GUIMENU_ITEM_T  menu_gm_list[] =
{
#if (defined(TOUCH_PANEL_SUPPORT)&& defined(GAME_LINK_SUPPORT))
    {IDGAME_MENU_LINK_ITEM_ID,  TIP_NULL, {STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN},TXT_COMMON_GAME_LINK,   IMAGE_SECMENU_ICON_PASTIME_LINKS,   0,0,0},
#endif
#if defined GAME_BOXMAN_SUPPORT && !defined MMI_SECMENU_ICON_DEFAULT
    {IDGAME_MENU_BOXMAN_ITEM_ID,TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_BOXMAN,  IMAGE_SECMENU_ICON_PASTIME_BOXMAN,  0,0,0},
#endif
#if defined(GAME_GOLDMINER_SUPPORT)
    {IDGAME_MENU_GOLDMINER_ITEM_ID,  TIP_NULL,{STXT_SOFTKEY_OK_MK,TXT_NULL,STXT_RETURN},TXT_GOLDMINER,   IMAGE_NULL,   0,0,MENU_NULL},
#endif
//michael wang for5
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
    {IDGAME_MENU_BUBBLEBOBBLE_ITEM_ID,TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_BUBBLEBOBBLE,  IMAGE_SECMENU_ICON_PASTIME_BUBBLEBOBBLE,  0,0,0},
#endif
#ifdef GAME_QUINTET_SUPPORT
      {IDGAME_MENU_QUT_ITEM_ID,TIP_NULL, {STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN},TXT_GAME_QUINTET, IMAGE_SECMENU_ICON_PASTIME_QUT ,  0,0,0},
#endif
#if defined(GAME_SNAKE_SUPPORT) 
#ifndef MMI_SECMENU_ICON_DEFAULT
    {IDGAME_MENU_SNAKE_ITEM_ID,  TIP_NULL,{STXT_SOFTKEY_OK_MK,TXT_NULL,STXT_RETURN},TXT_SNAKE,   IMAGE_SECMENU_ICON_PASTIME_SNAKE,   0,0,MENU_NULL},
#else
    {IDGAME_MENU_SNAKE_ITEM_ID,  TIP_NULL,{STXT_SOFTKEY_OK_MK,TXT_NULL,STXT_RETURN},TXT_SNAKE,   0,   0,0,MENU_NULL},
#endif
#endif

#if defined(GAME_TETRIS_SUPPORT) 
#ifndef MMI_SECMENU_ICON_DEFAULT
    {IDGAME_MENU_SQUARE_ITEM_ID,  TIP_NULL,{STXT_SOFTKEY_OK_MK,TXT_NULL,STXT_RETURN},TXT_GMSQU_TETRIS, IMAGE_SECMENU_ICON_PASTIME_TETRIS,   0,0,MENU_NULL},
#else
    {IDGAME_MENU_SQUARE_ITEM_ID,  TIP_NULL,{STXT_SOFTKEY_OK_MK,TXT_NULL,STXT_RETURN},TXT_GMSQU_TETRIS, 0,   0,0,MENU_NULL},
#endif
#endif

#if defined(GAME_GOLDMINER_SUPPORT) 
#ifndef MMI_SECMENU_ICON_DEFAULT
    {IDGAME_MENU_GOLDMINER_ITEM_ID,  TIP_NULL,{STXT_SOFTKEY_OK_MK,TXT_NULL,STXT_RETURN},TXT_GOLDMINER, IMAGE_SECMENU_ICON_PASTIME_GOLDMINER,   0,0,MENU_NULL},
#else
    {IDGAME_MENU_GOLDMINER_ITEM_ID,  TIP_NULL,{STXT_SOFTKEY_OK_MK,TXT_NULL,STXT_RETURN},TXT_GOLDMINER, 0,   0,0,MENU_NULL},
#endif
#endif

#if (!defined(GAME_BOXMAN_SUPPORT) && !defined(GAME_LINK_SUPPORT) && !defined(GAME_TETRIS_SUPPORT) && !defined(GAME_SNAKE_SUPPORT) && !defined(GAME_QUINTET_SUPPORT))
    {0,0, {TXT_NULL, TXT_NULL, TXT_NULL},0,  0,  0,0,0}
#endif
};
#endif
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T game_menu_table[] = 
{
    {NULL},
#include "mmigame_menutable.def"
        //#include "mmieng_menutable.def"
};
#undef  MENU_DEF

/*****************************************************************************/
// 	Description :  Register menu group
//	Global resource dependence : 
//  Author:KELLY.LI
//	Note: 
/*****************************************************************************/
PUBLIC void MMIGAME_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_GAME_MANAGER, game_menu_table);
}
#endif

