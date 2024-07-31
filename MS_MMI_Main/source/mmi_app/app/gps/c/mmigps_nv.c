/*************************************************************************
 ** File Name:      mmigps_nv.c                                          *
 ** Author:         jian.ma                                              *
 ** Date:           2007/07/19                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2007/07/19     jian.ma         Create.                              *
*************************************************************************/
#ifndef MMIGPS_NV_C_
#define MMIGPS_NV_C_

#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"

/*the length of blue tooth nv*/
const uint16 gps_nv_len[] =
{
    0//sizeof(MMIBT_STORAGE_E)//MMINV_BT_FILE_LOCATION
};

/*****************************************************************************/
// 	Description : register bt module nv len and max item
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIGPS_RegGpsNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_GPS, gps_nv_len, sizeof(gps_nv_len) / sizeof(uint16));
}

#endif


