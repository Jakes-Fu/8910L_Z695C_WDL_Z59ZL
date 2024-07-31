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
 ** 1/15/2016  		        		Create.                                   *
 ******************************************************************************/
#include "version.h"
#include "version_common.c"
#include "version_software_mocor.h"
#include "version_software_ap.h"
#include "version_software_gsm_base.h"
#include "version_log.h"

#if defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)
#include "version_software_gps.h"
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
    "1.0.2316",
    "MMI Version:       UIS8910",
    "AP SVN :"AP_SW_VERSION,
    "CP SVN :",
    "BASE  Version:     BASE_"_BASE_SW_VERSION_,  
    "HW Version:        UIS8910",    
    "",    //calibration  version
    "",     //download     version
    PROJECT_BUILD_TIME,
    "LOG Version:      "_VERSION_LOG_,
    "RH77",//MEMORY_NAME
#if defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)
    "GE2 Version:      "_MOCOR_GE2_VERSION_,
#endif
};
//lint +e786

