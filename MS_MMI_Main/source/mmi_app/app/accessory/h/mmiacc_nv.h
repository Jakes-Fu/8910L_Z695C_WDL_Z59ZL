/*************************************************************************
 ** File Name:      mmiacc_nv.h                                          *
 ** Author:         haiyang.hu                                           *
 ** Date:           2006/09/18                                           *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/18     haiyang.hu        Create.                             *
*************************************************************************/

#ifndef _MMIACC_NV_H_
#define _MMIACC_NV_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"
#ifdef WORLD_CLOCK_SUPPORT
#include "mmiwclk_export.h"
#endif

#ifdef CUSTOM_UNITCONVERSION_SUPPORT
#include "mmiunitconversion_export.h"
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define MMINV_READ(_item_id,_addr,_ret_value)\
_ret_value = MMI_ReadNVItem(_item_id, _addr)

#define MMINV_WRITE(_item_id,_addr) \
			 MMI_WriteNVItem(_item_id, _addr)

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
#ifdef WORLD_CLOCK_SUPPORT 
    MMINV_ACC_WORLDCLOCK_CITY_INFO = MMI_MODULE_ACC<< 16,
#endif
#ifdef CUSTOM_UNITCONVERSION_SUPPORT
    MMINV_ACC_UNITCONV_CUSTOMIZE_ITEM,
#endif
#if (0) //def MMI_JEWISH_CALENDAR
    // Due to T-upgrade reason, can not modify the order of current nv items, new items can be only added at the end of the whole nv list
    // In uis8910 queen project ,the last module is datacounter, so move below items to module datacounter.   it should be in MMI_MODULE_ACC.
    MMINV_JEWISH_CALENDAR_LOCATION_INFO,
    MMINV_JEWISH_CALENDAR_DLS_SETTING,
#endif
	MMINV_ACC_DUMMY_ITEM,
	MMIACC_MAX_NV_ITEM_NUM
}MMIACC_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIACC_RegNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIACC_NV_H_

