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

#ifndef _MMIGAME_LINK_MENUTABLE_H_
#define _MMIGAME_LINK_MENUTABLE_H_
#ifdef GAME_LINK_SUPPORT
//#include "mmigame.h"
#include "mmigame_internal.h"
#include "mmi_module.h"
#include "guimenu.h"
#include "mmieng_menutable.h"
#include "mmigame_link_text.h"
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
			GAMEINKS_ID_MENU_START = (MMI_MODULE_GAME_LINK << 16),				
				ID_GMLINKS_NEW, 		
				ID_GMLINKS_RES, 		
				ID_GMLINKS_SCORE_VIEW,
				ID_GMLINKS_SCORE_RES,	
				ID_GMLINKS_CLASS, 		
				ID_GMLINKS_INFO, 				
				GAMELINKS_ID_MENU_MAX
		}GAMEINKS_ID_MENU_E;



#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    GMINKS_MENU_LABEL_START = (MMI_MODULE_GAME_LINK << 16),
       #include "mmigame_link_menutable.def" 
    GMINKS_MENUTABLE_MAX
} GMINKS_MENU_LABEL_E;

#undef MENU_DEF



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/
#endif
#endif /* MENUTABLE_H */


