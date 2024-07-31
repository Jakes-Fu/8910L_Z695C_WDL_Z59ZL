/*************************************************************************
 ** File Name:      mmicp_nv.h                                           *
 ** Author:         fu.ruilin                                            *
 ** Date:           2015/06/08                                           *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2015/06/08     fu.ruilin        Create.                              *
*************************************************************************/

#ifndef MMICP_NV_H_
#define MMICP_NV_H_

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
    MMINV_CP_PREFERRED_SIM = MMI_MODULE_CP << 16,
    MMINV_CP_MAX_ITEM_NUM
}CP_NV_ITEM_E;

PUBLIC void MMICP_RegNv(void);

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

