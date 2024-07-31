/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
/*************************************************************************
** File Name:      mmijava_id.c                                          *
** Description:     This file defines the function about nv             *
*************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
//#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"
#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmijava_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
#include "mmijava_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_JAVA, mmijava_id_name_arr);/*lint !e64*/
}
#endif