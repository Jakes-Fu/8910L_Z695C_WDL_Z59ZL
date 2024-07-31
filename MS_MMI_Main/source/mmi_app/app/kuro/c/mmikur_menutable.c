#include "std_header.h"
#ifdef KURO_SUPPORT
/*****************************************************************************
** File Name:      mmimp3_menutable.c                                        *
** Author:                                                                   *
** Date:           24/03/2004                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mp3 player                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2007       Jassmine.Meng       Create                                  *
******************************************************************************/

#define _MMIMP3_MENUTABLE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "window_parse.h"
#include "guimenu.h"
#include "mmikur_menutable.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmikuro_text.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define TIP_NULL    0

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//player
const GUIMENU_ITEM_T kur_menu_play[] =
{
    {ID_KUR_ALLKUR,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_KUR_ALLKUR,         0, 0, 0, NULL},
    {ID_KUR_KDB,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_KUR_KDB,                 0, 0, 0, NULL},
#ifdef MP3_EQ_SUPPORT 
    {ID_KUR_EQMODE,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_KUR_EQ_SETTING,                 0, 0, 0, NULL},
#endif
    {ID_KUR_PLAYMODE,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_KUR_PLAY_MODE,                 0, 0, 0, NULL},
    //add by liluyao 
    {ID_KUR_HELP,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMIPASTIME_HELP,                 0, 0, 0, NULL},
};

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
    _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T kur_menu_table[] = 
{
    {NULL},
    #include "mmikur_menutable.def"
};

#undef MENU_DEF
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register mp3 menu group
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_KURO, kur_menu_table);
}
#endif
