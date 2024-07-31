/*************************************************************************
 ** File Name:      mmipicview_nv.h                                      *
 ** Author:         Jassmine                                             *
 ** Date:           2008/12/19                                           *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.   *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2008/12/19    Jassmine           Create.                             *
*************************************************************************/
#ifdef PIC_VIEWER_SUPPORT
#ifndef MMIPICVIEW_NV_H_
#define MMIPICVIEW_NV_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
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
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMINV_PICVIEW_SLIDE_EFFECT = MMI_MODULE_PIC_VIEWER << 16,
    MMINV_PICVIEW_SLIDE_TIME,
    MMINV_PICVIEW_VIEW_TYPE,
    MMINV_PICVIEW_DEFAULT_PATH,
    MMINV_PICVIEW_IS_FIRST_ENTER,    
    MMINV_PICVIEW_MAX_NV_ITEM_NUM,
} PICVIEW_NV_ITEM_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register pic viewer module nv len and max item
//	Global resource dependence : none
//  Author: jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_RegNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif
