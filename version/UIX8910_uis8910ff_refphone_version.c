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
    "Platform Version: MOCOR_20B."_MOCOR_SW_VERSION_"_Release",
#else
    "Platform Version: MOCOR_20B."_MOCOR_SW_VERSION_"_Debug",
#endif
#ifdef ZDT_WDL_A03ZL_SUPPORT
#ifdef LEBAO_MUSIC_SUPPORT
		  	"Z695C_HW6.2_ARW_GX8733_WDL_A03ZL_MG_YT_V1.6_20230714",
    "MMI Version:  Z695C_HW6.2_ARW_GX8733_WDL_A03ZL_MG_YT_V1.6_20230714",
#else
	  	"Z695C_HW6.2_ARW_GX8733_WDL_A03ZL_YT_V1.6_20230821",
    "MMI Version:  Z695C_HW6.2_ARW_GX8733_WDL_A03ZL_YT_V1.6_20230821",
#endif	
#elif defined(ZDT_WDL_C27ZL_SUPPORT)
	  	"Z695C_HW6.2_ARW_GX8733_WDL_C27ZL_YT_V1.4_20231120",
    "MMI Version:  Z695C_SU_HW6.2_ARW_GX8733_WDL_C27ZL_YT_V1.4_20231120",	
 #elif defined(ZDT_WDL_Z59_SUPPORT)
 	  	"Z695C_8910L_HW6.2_ARW_GX8733_WDL_Z59_FM_V1.0_20240722",
    "MMI Version: Z695C_8910L_SU_HW6.2_ARW_GX8733_WDL_Z59_FM_V1.0_20240722",	
 #elif defined(ZDT_WDL_Z60_SUPPORT)
 	  	"Z695C_HW6.2_ARW_GX8733_WDL_Z60_V1.1_20240720",
    "MMI Version:  Z695C_SU_HW6.2_ARW_GX8733_WDL_Z60_V1.1_20240720",	
#elif defined(ZDT_WDL_Z62_SUPPORT)	
 	  	"Z695C_HW6.2_ARW_2810_WDL_Z62_V1.0_20240621",
    "MMI Version:  Z695C_SU_HW6.2_ARW_2810_WDL_Z62_V1.0_20240621",	
#else
  	"Z695C_HW6.2_ARW_GX8733_WDL_D69ZL_YT_V2.5_20230906",
    "MMI Version:  Z695C_HW6.2_ARW_GX8733_WDL_D69ZL_YT_V2.5_20230906",
#endif	
    "AP SVN :"AP_SW_VERSION,
    "CP SVN :",
    "BASE  Version:     BASE_"_BASE_SW_VERSION_,  
    "HW Version:UIS8910FF",    
    "",    //calibration  version
    "",     //download     version
    PROJECT_BUILD_TIME,
    "LOG Version:      "_VERSION_LOG_,
    "RH77",//MEMORY_NAME
#if defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)
    "GE2 Version:      "_MOCOR_GE2_VERSION_,
#endif
};

/*Bug 1070436*//*Bug 1070447*//*Bug 1070507*//*Bug 1070638*//*Bug 1070614*/
const char  * const s_ims_ua_version_buf = {"Unisoc 220 4G "};
//lint +e786

