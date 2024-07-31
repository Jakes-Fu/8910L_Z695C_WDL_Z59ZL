/*************************************************************************
 ** File Name:      mmibt_nv.c                                             *
 ** Author:         kelly.li                                         *
 ** Date:           2006/08/21                                           *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/08/21     kelly.li      Create.                              *
*************************************************************************/
//#ifndef MMIBT_NV_C_
#define MMIBT_NV_C_
#ifdef WIN32
#include "std_header.h"
#endif

//#include "sci_types.h"
#include "mmi_modu_main.h"
#include "mmibt_func.h"

/*the length of blue tooth nv*/
const uint16 bt_nv_len[] =
{
    sizeof(MMIBT_STORAGE_E),//MMINV_BT_FILE_LOCATION
    sizeof(MMIBT_SHAIRED_DIR_T),
    sizeof(MMIBT_SHAIRED_AUT_T),
    sizeof(MMINV_BT_HIDE_DEVICE),
#ifdef WIN32
    sizeof(BT_INFO),
#endif
};

/*****************************************************************************/
// 	Description : register bt module nv len and max item
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIBT_RegBtNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_BT, bt_nv_len, sizeof(bt_nv_len) / sizeof(uint16));
}

//#endif


