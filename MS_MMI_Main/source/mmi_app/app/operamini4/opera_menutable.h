/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */
#ifndef OPERA_MENUTAB_H_
#define OPER_MENUTAB_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif

 
typedef enum
{
#ifdef OPERA_MOBILE_SUPPORT
    ID_OPERA_MENU_START = (MMI_MODULE_OPERA_MOBILE<< 16),
#else//OPERA_MINI_SUPPORT
    ID_OPERA_MENU_START = (MMI_MODULE_OPERA_MINI<< 16),
#endif

    ID_OPERA_CC_VOICE,
    ID_OPERA_CC_IP,
    ID_OPERA_CC_VIDEO,

	
    ID_OPERA_MENU_TOTAL
}ID_OPERA_MENU_E;
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
#ifdef OPERA_MOBILE_SUPPORT
    MMI_OPERA_BEGIN_MENU_START = (MMI_MODULE_OPERA_MOBILE<< 16),
#else//OPERA_MINI_SUPPORT
    MMI_OPERA_BEGIN_MENU_START = (MMI_MODULE_OPERA_MINI<< 16),
#endif
	
	#include "opera_menutable.def"
	MENUTABLE_MAX
} MMI_OPERA_MENUTAB_E;
#undef MENU_DEF

/*****************************************************************************/
//  Description : reg menu
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIOPERA_RegMenuGroup(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
