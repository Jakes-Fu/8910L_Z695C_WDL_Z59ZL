/*****************************************************************************
** File Name:     mmigame_bubblebobble_menutable.h                                                 *
** Author:                                                                   *
** Date:             01/06/2012                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                  NAME             DESCRIPTION                               *
** 01/06/2012       junxue.zhu          Create
******************************************************************************/

#ifndef _MMIGAME_BUBBLEBOBBLE_MENUTABLE_H_
#define _MMIGAME_BUBBLEBOBBLE_MENUTABLE_H_
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
//#include "mmigame.h"
#include "mmigame_internal.h"
#include "mmi_module.h"
#include "guimenu.h"
#include "mmieng_menutable.h"
#include "mmigame_bubblebobble_text.h"
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
	GAMEBUBBLEBOBBLE_ID_MENU_START = (MMI_MODULE_GAME_BUBBLEBOBBLE << 16),				
	ID_GMBUBBLEBOBBLE_NEW,    
    ID_GMBUBBLEBOBBLE_SELECT,
    ID_GMBUBBLEBOBBLE_HIGHSCORE,			
	GMBUBBLEBOBBLE_ID_MENU_MAX
}BUBBLEBOBBLE_ID_MENU_E;



#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    GMBUBBLEBOBBLE_MENU_LABEL_START = (MMI_MODULE_GAME_BUBBLEBOBBLE << 16),
       #include "mmigame_bubblebobble_menutable.def" 
    GMBUBBLEBOBBLE_MENUTABLE_MAX
} GMBUBBLEBOBBLE_MENU_LABEL_E;

#undef MENU_DEF

/*****************************************************************************/
//   FUNCTION:      reg bubble menu
// 	Description :   
//	Global resource dependence : 
//   Author:       junxue.zhu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIGMBUBBLEBOBBLE_RegMenuGroup(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/
#endif
#endif /* MENUTABLE_H */


