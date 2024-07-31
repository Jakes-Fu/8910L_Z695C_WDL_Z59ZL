/*************************************************************************
 ** File Name:      mmialarm_nv.h                                        *
 ** Author:         liqing.peng                                          *
 ** Date:           2006/09/12                                           *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/12     liqing.peng      Create.                              *
*************************************************************************/
#ifdef MMIPB_SYNC_WITH_O365

#ifndef _PBSYNC_NV_H_
#define _PBSYNC_NV_H_

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
    PBSYNCNV_TIMER_COUNT = MMI_MODULE_CONTACTSYNC << 16,
    PBSYNCNV_IS_TIMER_ARRIVE,
    PBSYNCNV_IS_ALREADY_PROMPT,
    PBSYNCNV_MAX_ITEM_NUM
}PBSYNC_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register hello module nv len and max item
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMI_RegPBSYNCNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif 

#endif //MMIPB_SYNC_WITH_O365