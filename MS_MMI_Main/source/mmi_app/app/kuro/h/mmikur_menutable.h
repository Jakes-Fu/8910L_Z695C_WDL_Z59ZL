#ifdef KURO_SUPPORT
/*****************************************************************************
** File Name:      mmimp3_menutable.h                                        *
** Author:                                                                   *
** Date:           24/03/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmimp3_internal func                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2007       Jassmine           Create
******************************************************************************/

#ifndef _MMIKUR_MENUTABLE_H_
#define _MMIKUR_MENUTABLE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
        
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//Menu ID
typedef enum
{
    MMIKUR_ID_MENU_START = (MMI_MODULE_KURO << 16),

    ID_KUR_ALLKUR,
    ID_KUR_KDB,
    ID_KUR_EQMODE,
    ID_KUR_PLAYMODE,
    ID_KUR_HELP,
    ID_KUR_LIST_PLAY,
    ID_KUR_LIST_DELETE,

    MMIMP3_ID_MENU_MAX
}MMIKUR_ID_MENU_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIKUR_MENU_LABEL_START = (MMI_MODULE_KURO << 16),
    #include "mmikur_menutable.def"
    MMIKUR_MENUTABLE_MAX
} MMIKUR_MENU_LABEL_E;

#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif
