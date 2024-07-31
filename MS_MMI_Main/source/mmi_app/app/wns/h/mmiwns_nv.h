/***************************************************************************
** File Name:     mmiwns_nv.h                                                                                                                  *
** Author:          grant peng                                                                                                                    *
** Date:           16/7/2015                                                                                                                    *
** Copyright:      2014 Spreatrum, Incoporated. All Rights Reserved.                                             *
** Description:     This file defines the function about nv                                                                   *
****************************************************************************
**                         Important Edit History                                                                                                 *
** ------------------------------------------------------------------------ *
** DATE                 NAME             DESCRIPTION                                                                                    *
** 7/2015          grant.peng              Create                                                                                         *
**                                                                                                                                                                 *
****************************************************************************/

#ifndef _MMIWNS_NV_H_
#define _MMIWNS_NV_H_

#include "sci_types.h"
#include "mmi_module.h"

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
    MMIWNS_NV_AVALAIBLE_CHANNEL_NUM = MMI_MODULE_WNS << 16,
    MMIWNS_NV_PNS_DATA_APP1,
    MMIWNS_NV_PNS_DATA_APP2,
    MMIWNS_NV_PNS_DATA_APP3,
    MMIWNS_NV_PNS_DATA_APP4,
    MMIWNS_NV_PNS_DATA_APP5,
    MMIWNS_NV_PNS_DATA_APP6,
    MMIWNS_NV_PNS_DATA_APP7,
    MMIWNS_NV_PNS_DATA_APP8,
    MMIWNS_NV_PNS_DATA_APP9,
    MMIWNS_NV_PNS_DATA_APP10,
    MMITEST_NV_MAX_ITEM_NUM
}MMIWNS_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
//Description : register wns module nv
//Global resource dependence : none
//  Author: grant.peng
//Note:
/*****************************************************************************/
PUBLIC void MMI_RegWnsNv(void);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMITEST_NV_H_

