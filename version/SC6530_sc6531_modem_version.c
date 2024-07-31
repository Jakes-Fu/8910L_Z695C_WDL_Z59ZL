/******************************************************************************
 ** File Name:      sc6600m_sp7400_version.c                                  *
 ** Author:         Jimmy.Jia                                                 *
 ** DATE:           1/8/2005                                                  *	
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file include sw version of project  		*
 **					SP7400.								                  	  * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           	NAME            DESCRIPTION                             *
 ** 1/8/2005  		Jimmy.Jia		Create.                            *
 ******************************************************************************/

#include "version.h"
#include "version_common.c"
#include "version_software_mocor.h"
#include "version_software_gsm_base.h"
#include "version_log.h"
//lint -e786
const char* const  s_version_info[MAX_VERSION_NUM] = 
{
#ifdef _VERSION_RELEASE_  
    "Platform Version: MOCOR_12C."_MOCOR_SW_VERSION_"_Release",
#else
    "Platform Version: MOCOR_12C."_MOCOR_SW_VERSION_"_Debug\n"
    _MOCOR_SW_PROJECT_,
#endif
    "Project Version:   SC6531_modem",
    "MMI Version:       SC6531_modem",
    "BASE  Version:     BASE_"_BASE_SW_VERSION_,  
    "HW Version:        SC6531_modem",    
    "",    //calibration  version
    "",     //download     version
    PROJECT_BUILD_TIME,
    "LOG Version:      "_VERSION_LOG_
};
//lint +e786


