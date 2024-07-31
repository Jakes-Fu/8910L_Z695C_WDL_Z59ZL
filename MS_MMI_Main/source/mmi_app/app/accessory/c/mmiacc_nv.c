/*************************************************************************
 ** File Name:      mmiacc_nv.c                                          *
 ** Author:         haiyang.hu                                           *
 ** Date:           2006/09/18                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/18     haiyang.hu       Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "mmiacc_nv.h"
#include "mmi_modu_main.h"

//#include "mmi_pccamera.h"
const uint16 acc_nv_len[] =
{
#if defined WORLD_CLOCK_SUPPORT
    sizeof(WT_CITY_RECORD_INFO)*WT_DEFAULT_CITY_NUM,// s_default_city_info[WT_DEFAULT_CITY_NUM]
#endif
#ifdef CUSTOM_UNITCONVERSION_SUPPORT
    //sizeof(BOOLEAN) * UC_CONV_CUSTOMIZE_MAX_NUM, //MMI_SMARTDUALSIM_NV_FIST_TIME_PROMPTED
    sizeof(UC_CONV_CUSTOMIZE_T),//MMI_SMARTDUALSIM_NV_FORWARDING_DIRECTION
#endif
#if (0) //def MMI_JEWISH_CALENDAR
    // Due to T-upgrade reason, can not modify the order of current nv items, new items can be only added at the end of the whole nv list
    // In uis8910 queen project ,the last module is datacounter, so move below items to module datacounter.  
    sizeof(uint8),				  // MMINV_JEWISH_CALENDAR_LOCATION_INFO
    sizeof(uint8),				  // MMINV_JEWISH_CALENDAR_DLS_SETTING
#endif
    sizeof(uint8),				  // MMINV_JEWISH_CALENDAR_DLS_SETTING
};
/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIACC_RegNv(void)
{
#if defined WORLD_CLOCK_SUPPORT || defined(CUSTOM_UNITCONVERSION_SUPPORT) || defined(MMI_JEWISH_CALENDAR)
    MMI_RegModuleNv(MMI_MODULE_ACC, acc_nv_len, sizeof(acc_nv_len) / sizeof(uint16));
#endif
}
