


/*****************************************************************************
** File Name:     mmigame_menutable.h                                                   *
** Author:                                                                   *
** Date:           11/21/2005                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2005       annie.an      Create
******************************************************************************/

#ifndef _MMIGAME_MENUTABLE_H_
#define _MMIGAME_MENUTABLE_H_
#ifdef GAME_SUPPORT
//#include "mmigame.h"
#include "mmigame_internal.h"
#include "mmi_module.h"
#include "guimenu.h"
#include "mmi_menutable.h"
#include "mmigame_text.h"
#include "mmi_image.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/

typedef enum
{
	GAME_ID_MENU_START = (MMI_MODULE_GAME_MANAGER << 16),
	IDGAME_MENU_LINK_ITEM_ID,
	IDGAME_MENU_BOXMAN_ITEM_ID,
	IDGAME_MENU_QUT_ITEM_ID,
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
    IDGAME_MENU_BUBBLEBOBBLE_ITEM_ID,
#endif
#ifdef GAME_GOLDMINER_SUPPORT
    IDGAME_MENU_GOLDMINER_ITEM_ID,
#endif
#ifdef GAME_SNAKE_SUPPORT
    IDGAME_MENU_SNAKE_ITEM_ID,
#endif
#ifdef GAME_TETRIS_SUPPORT
    IDGAME_MENU_SQUARE_ITEM_ID,
#endif
	ID_GM_SET,
	ID_GM_MUSIC,
	GAME_ID_MENU_MAX
}GAME_ID_MENU_E;




#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    GAME_MENU_LABEL_START = (MMI_MODULE_GAME_MANAGER << 16),
    #include "mmigame_menutable.def"
    GAME_MENUTABLE_MAX
} GAME_MENU_LABEL_E;

#undef MENU_DEF

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/
#endif //GAME_SUPPORT
#endif /* MENUTABLE_H */


