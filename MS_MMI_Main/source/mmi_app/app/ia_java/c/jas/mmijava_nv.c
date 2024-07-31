/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
/*****************************************************************************
** File Name:      mmijava_nv.c                                       *
** Description:    This file is used to describe digit camera                *
******************************************************************************/
#ifndef _MMIJAVA_NV_C
#define _MMIJAVA_NV_C

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
//#include "sci_types.h"
#include "mmijava_nv.h"
#include "mmijava_internal.h"
#include "mmi_modu_main.h"
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
/*the length of java nv*/
const uint16 mmijava_nv_len[] =
{      
	    sizeof(MMIJAVA_DOWNLOAD_DETAIL_T),
	    sizeof(MMIJAVA_LINK_SETTING_T),
        sizeof(uint8),                                                //s_cur_java_store_dev_type     
        sizeof(uint32),  //MMINV_PREINSTALL_INFO


        (MMIFILE_FULL_PATH_MAX_LEN + 1)*sizeof(wchar),
        (MMIFILE_FULL_PATH_MAX_LEN + 1)*sizeof(wchar)
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register java module nv len and max item
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_RegJavaNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_JAVA, mmijava_nv_len, sizeof(mmijava_nv_len) / sizeof(uint16));
    return TRUE;
}

#endif
#endif
