/*-------------------------------------------------------------------------*/
/*                         VARIABLES                                       */
/*-------------------------------------------------------------------------*/
/*****************************************************************************
** File Name:     mmigame_snake_menutable.h                                                   *
** Author:                                                                   *
** Date:           26/01/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 25/01/2011  peilong.zhou      Create
******************************************************************************/

#ifndef _MMIGAME_SNAKE_MENUTABLE_H_
#define _MMIGAME_SNAKE_MENUTABLE_H_
#ifdef GAME_SNAKE_SUPPORT
//#include "mmigame.h"
#include "mmi_module.h"
#include "guimenu.h"
#include "mmieng_menutable.h"
#include "mmigame_snake_text.h"
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
    GAMESNAKE_ID_MENU_START = (MMI_MODULE_GAME_SNAKE << 16),                
    ID_GMSNAKE_NEW,         
    ID_GMSNAKE_SCORE_VIEW,
    ID_GMSNAKE_LEVEL,       
    ID_GMSNAKE_INFO,    
    GAMESNAKE_ID_MENU_MAX
}GAMESNAKE_ID_MENU_E;



#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    GMSNAKE_MENU_LABEL_START = (MMI_MODULE_GAME_SNAKE << 16),//GAMESNAKE_ID_MENU_MAX,
    #include "mmigame_snake_menutable.def" 
    GMSNAKE_MENUTABLE_MAX
} GSNAKE_MENU_LABEL_E;

#undef MENU_DEF



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/
#endif
#endif//_MMIGAME_SNAKE_MENUTABLE_H_


