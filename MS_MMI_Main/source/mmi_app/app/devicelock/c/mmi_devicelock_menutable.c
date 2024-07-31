/*****************************************************************************
** File Name:      ***_menutable.c                                         *
** Author:                                                   *
** Date:                                                          *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to handle ebook menutable               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**                                    *
******************************************************************************/
#define _MMI_DEVICELOCK_MENUTABLE_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "window_parse.h"
#include "guimenu.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmi_menutable.h"
#include "mmi_module.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_devicelock_menutable.h"
#include "mmi_devicelock_text.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


const GUIMENU_ITEM_T menu_devicelock_setting[] =
{   
    {ID_DEVICELOCK_TEST_MENU,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, STXT_SOFTKEY_MARK_MK,  0, 0, 0, MENU_NULL},  
};

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmi_devicebook_menu_table[] = 
{
    {NULL},
    #include "mmi_devicelock_menutable.def"
};

#undef MENU_DEF

/*****************************************************************************/
// 	Description : Register ebook menu group
//	Global resource dependence : none
//  Author: songbin.zeng
//	Note:
/*****************************************************************************/
PUBLIC void MMIDEVICELOCK_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_DEVICELOCK, mmi_devicebook_menu_table);
}
