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

#ifndef _MMIGAME_QUINTET_MENUTABLE_H_
#define _MMIGAME_QUINTET_MENUTABLE_H_
#ifdef GAME_QUINTET_SUPPORT
//#include "mmigame.h"
#include "mmigame_internal.h"
#include "mmi_module.h"
#include "guimenu.h"
#include "mmieng_menutable.h"
#include "mmigame_quintet_text.h"
#include "mmi_image.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                                                                       *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                                                          */
/*-------------------------------------------------------------------------*/
typedef enum
{
	GAMEQUT_ID_MENU_START = (MMI_MODULE_GAME_QUINTET << 16),				
	ID_GMQUT_NEW_ITEM_ID,
	ID_GMQUT_RES_ITEM_ID,	
	ID_GMQUT_SET_ITEM_ID,	
	ID_GMQUT_INFO_ITEM_ID,
	ID_GMQUT_LAST_ITEM_ID,			
	GAMEQUT_ID_MENU_MAX
}GAMEQUT_ID_MENU_E;



#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    GMQUT_MENU_LABEL_START = (MMI_MODULE_GAME_QUINTET << 16),
       #include "mmigame_quintet_menutable.def" 
    GMQUT_MENUTABLE_MAX
} GMQUT_MENU_LABEL_E;

#undef MENU_DEF



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif /* MENUTABLE_H */
#endif

