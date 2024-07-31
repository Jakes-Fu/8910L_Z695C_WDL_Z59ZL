/*****************************************************************************
** File Name:     mmigame_goldminer_menutable.h                              *
** Author:                                                                   *
** Date:           06/09/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/09/2012                      Create                                    *
******************************************************************************/

#ifndef _MMIGAME_GOLDMINER_MENUTABLE_H_
#define _MMIGAME_GOLDMINER_MENUTABLE_H_
#ifdef GAME_GOLDMINER_SUPPORT
//#include "mmigame.h"
#include "mmi_module.h"
#include "guimenu.h"
#include "mmieng_menutable.h"
#include "mmigame_goldminer_text.h"
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
    GAMEGOLDMINER_ID_MENU_START = (MMI_MODULE_GAME_GOLDMINER << 16),                
    ID_GMGOLDMINER_NEW,         
    ID_GMGOLDMINER_SCORE_VIEW,
    ID_GMGOLDMINER_LEVEL,       
    ID_GMGOLDMINER_INFO,    
    GAMEGOLDMINER_ID_MENU_MAX
}GAMEGOLDMINER_ID_MENU_E;



#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    GOLDMINER_MENU_START = (MMI_MODULE_GAME_GOLDMINER << 16),//GAMEGOLDMINER_ID_MENU_MAX,
    #include "mmigame_goldminer_menutable.def" 
    GMGOLDMINER_MENU_MAX
}GOLDMINER_MENU_E;

#undef MENU_DEF



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/
#endif
#endif//_MMIGAME_GOLDMINER_MENUTABLE_H_