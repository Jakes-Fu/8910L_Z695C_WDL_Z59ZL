/***********************************************************************
**Author:    huangjc
**Date:      2007-4-18
**CopyRight:AMOI 2007
**Description:This file define the function of java nv
***********************************************************************/
/***********************************************************************
**                                                Edit History
**      Date                    Name               Description
***2007-4-18              huangjc                create
***********************************************************************/
#ifndef _MMIJAVA_NV_H_
#define _MMIJAVA_NV_H_

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
    MMINV_JAVA_DOWNLOAD_INFO = MMI_MODULE_JAVA << 16,
    MMINV_JAVA_LINKSETTING_INFO,   

    MMINV_JAVA_CURRENT_STORE_DEVICE,
    MMINV_PREINSTALL_INFO,
    
    MMINV_OTA_TEMP_JADFILE_INFO,
    MMINV_OTA_TEMP_JARFILE_INFO,        
    MMINV_JAVA_MAX_NV_ITEM_NUM
}JAVA_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register java module nv len and max item
//	Global resource dependence : none
//   Author: huangjc
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_RegJavaNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
