/*
 * Copyright 2011 Myriad Group AG. All Rights Reserved.
 * $Id: //depot/dev/project/spreadtrum/6530_new/export/inc/mmijava_nv.h#1 $
 */

/**
 * @file
 * Define NV relative method for 6800h
 */
#ifndef _MMIJAVA_NV_H_
#define _MMIJAVA_NV_H_

#include <sci_types.h>
#include <mmi_module.h>

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    JAVA_DOWNLOAD_INFO = MMI_MODULE_JAVA << 16,
#ifdef DYNAMIC_MAINMENU_SUPPORT
    JAVANV_INSTALLED_SUITE_INFO,
#endif
    JAVA_NV_MAX_NV_ITEM_NUM
}JAVA_NV_ITEM_E;
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register java module nv len and max item
// Global resource dependence : none
//   Author: martin.zhao
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_RegNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
