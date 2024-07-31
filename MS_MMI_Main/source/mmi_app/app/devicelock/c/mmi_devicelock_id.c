/*************************************************************************
 ** File Name:      ***_id.c                                          *
 ** Author:                                                  *
 ** Date:                                                *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 **                             *
*************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmi_devicelock_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmi_devicelock_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDEVICELOCK_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_DEVICELOCK, mmi_devicelock_id_name_arr);
}
