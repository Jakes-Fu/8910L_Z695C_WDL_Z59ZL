/*-------------------------------------------------------------------------*/
/*                         VARIABLES                                       */
/*-------------------------------------------------------------------------*/
/*****************************************************************************
** File Name:     mmigame_quintet_menutable.h                                                   *
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

#ifndef _MMIGAME_BOXMAN_MENUTABLE_H_
#define _MMIGAME_BOXMAN_MENUTABLE_H_
#ifdef GAME_BOXMAN_SUPPORT
//#include "mmigame.h"
#include "mmigame_internal.h"
#include "mmi_module.h"
#include "guimenu.h"
#include "mmieng_menutable.h"
#include "mmigame_boxman_text.h"
#include "mmi_image.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                                                                       *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

   // const GUI_MENU_ITEM_T  menu_menu_quintet[];
/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                                                          */
/*-------------------------------------------------------------------------*/
typedef enum
{
	GAMEBOXMAN_ID_MENU_START = (MMI_MODULE_GAME_BOXMAN << 16),				
	ID_GMBOXMAN_NEW,  
    ID_GMBOXMAN_RES,  
    ID_GMBOXMAN_SELECT,
    ID_GMBOXMAN_INFO, 			
	GAMEBOXMAN_ID_MENU_MAX
}GAMEBOXMAN_ID_MENU_E;



#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    GMBOXMAN_MENU_LABEL_START = (MMI_MODULE_GAME_BOXMAN << 16),
       #include "mmigame_boxman_menutable.def" 
    GMBOXMAN_MENUTABLE_MAX
} GMBOXMAN_MENU_LABEL_E;

#undef MENU_DEF


/*****************************************************************************/
//   FUNCTION:      reg quintet nenu group
// 	Description :   
//2
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIGMBOXMAN_RegMenuGroup(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/
#endif
#endif /* MENUTABLE_H */


