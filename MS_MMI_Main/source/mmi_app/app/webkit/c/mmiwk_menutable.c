/*****************************************************************************
** File Name:      mmibrowser_menutable.c                                    *
** Author:          li.li                                                    *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser menu table      *
*****************************************************************************/
#if defined(BROWSER_SUPPORT_WEBKIT) && ((!defined(WIN32)) || (defined(_MSC_VER) && (_MSC_VER >= 1400)))

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiwk_menutable.h"
#include "guimenu.h"
#include "mmiwk_id.h"
#include "mmimsa_id.h"
#include "mmi_modu_main.h"
#include "mmi_common.h"
#include "mmi_image.h"
#include "mmi_menutable.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

const GUIMENU_ITEM_T menu_mmiwk_popup[] =
{
    {ID_POPUP_MENU_WK_MAIN_SIGNIN,   0, {TXT_NULL,STXT_SOFTKEY_MARK_MK,TXT_COMMON_COMMON_BACK},STR_MSA_SKIP_CREATION_EXT01, 0, 0, 0, 0},
    {ID_POPUP_MENU_WK_MAIN_SIGNUP,   0, {TXT_NULL,STXT_SOFTKEY_MARK_MK,TXT_COMMON_COMMON_BACK},STR_MSA_SKIP_CREATION_EXT01, 0, 0, 0, 0},        
};


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmiwk_menu_table[] = 
{
    {0},   
    #include "mmiwk_menutable.def"
};

#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : browser reg menu group
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWK_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_WK, mmiwk_menu_table);
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
