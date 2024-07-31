/**
 * Copyright 2011 Myriad Group AG. All Rights Reserved.
 * $Id: //depot/dev/project/spreadtrum/6530_new/export/src/mmijava_nv.c#1 $
 */

/*****************************************************************************
** File Name:      mmijava_nv.c                                        *
** Description:    This file is used to store data on NV               *
******************************************************************************/
#ifndef _MMIJAVA_NV_C
#define _MMIJAVA_NV_C

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include <std_header.h>
#endif
#ifdef JAVA_SUPPORT_MYRIAD
#include <mmijava_nv.h>
#include <mmijava_common.h>
#include "mmi_modu_main.h"
#ifdef SUPPORT_DOWNLOAD
#include "mmijava_dlwin.h"
#endif
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
/*the length of java nv*/
const uint16 mmijava_nv_len[] =
{
    #ifdef SUPPORT_DOWNLOAD
    sizeof(MMIJAVA_DOWNLOAD_DETAIL_T),
    #else
    sizeof( uint8 ), //for compiling error only
    #endif
    #ifdef DYNAMIC_MAINMENU_SUPPORT
    MAX_INSTALLED_MIDLETS_NUM * sizeof(MMIJAVA_INSTALLED_ITEM_T)
    #endif
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register java module nv len and max item
//  Global resource dependence : none
//   Author: Martin.Zhao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_JAVA, mmijava_nv_len, sizeof(mmijava_nv_len) / sizeof(uint16));
    return TRUE;
}

#endif
#endif
