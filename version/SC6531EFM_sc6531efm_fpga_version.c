/******************************************************************************
 ** File Name:      SC6531EFM_sc6531efm_fpga_version.c                                *
 ** Author:                                                                   *
 ** DATE:           1/15/2016                                                 *	
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file include sw version of project  		          *
 **					SC6531EFM.								                  	  * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           	NAME            DESCRIPTION                               *
 ** 1/15/2016  						Create.                                   *
 ******************************************************************************/
#ifndef _SC6531EFM_FPGA_VERSION_C_
#define _SC6531EFM_FPGA_VERSION_C_

#include "version.h"
#include "version_common.c"
#include "version_software_mocor.h"
#include "version_software_gsm_base.h"
#include "version_log.h"
//lint -e786
const char* const  s_version_info[MAX_VERSION_NUM] = 
{
#ifdef _VERSION_RELEASE_  
    "Platform Version: MOCOR_"_MOCOR_SW_VERSION_"_Release",
#else
    "Platform Version: MOCOR_"_MOCOR_SW_VERSION_"_Debug\n"
    _MOCOR_SW_PROJECT_,
#endif
    "Project Version:   SC6531EFM_FPGA",
    "MMI Version:       SC6531EFM_FPGA",
    "BASE  Version:     BASE_"_BASE_SW_VERSION_,  
    "HW Version:        SC6531EFM_FPGA",    
    "",    //calibration  version
    "",     //download     version
    PROJECT_BUILD_TIME,
    "LOG Version:      "_VERSION_LOG_
};
//lint +e786

#endif  // #ifndef _SC6531EFM_FPGA_VERSION_C_
