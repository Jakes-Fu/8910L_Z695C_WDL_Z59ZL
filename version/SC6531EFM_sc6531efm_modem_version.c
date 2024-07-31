/******************************************************************************
 ** File Name:      SC6531EFM_sc6531efm_bar_version.c                                 *
 ** Author:         		                                                  *
 ** DATE:           1/15/2016                                                 *	
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file include sw version of project  	              *
 **					SC6531EFM.								                  	  * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           	NAME            DESCRIPTION                               *
 ** 6/15/2016  		        		Create.                                   *
 ******************************************************************************/

#include "version.h"
#include "version_common.c"
#include "version_software_mocor.h"
#include "version_software_gsm_base.h"
#include "version_log.h"
#ifdef FOTA_SUPPORT_ADUPS
#include "adups_build_datetime.h"
#endif
//lint -e786
const char* const  s_version_info[MAX_VERSION_NUM] = 
{
#ifdef _VERSION_RELEASE_  
    "Platform Version: MOCOR_16A."_MOCOR_SW_VERSION_"_Release",
#else
    "Platform Version: MOCOR_16A."_MOCOR_SW_VERSION_"_Debug\n"
    _MOCOR_SW_PROJECT_,
#endif
    "Project Version:   SC6531EFM_BAR",
    "MMI Version:       SC6531EFM_BAR",
    "BASE  Version:     BASE_"_BASE_SW_VERSION_,  
    "HW Version:        SC6531EFM_BAR",    
    "",    //calibration  version
    "",     //download     version
    PROJECT_BUILD_TIME,
    "LOG Version:      "_VERSION_LOG_
};

#ifdef FOTA_SUPPORT_ADUPS
const char* const  s_adups_project_name = ADUPS_PROJECT_NAME;
const char* const  s_adups_version_info = ADUPS_BUILD_TIME;
#endif

//lint +e786


