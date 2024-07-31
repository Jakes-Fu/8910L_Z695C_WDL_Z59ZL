/******************************************************************************
 ** File Name:      version.h                                                 *
 ** Author:         Tony Yao                                                  *
 ** DATE:           23/08/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 23/08/2002     Tony.Yao         Create.                                   *
 ** 03/11/2003     Xueliang.Wang    Modify.                                   *
 ******************************************************************************/
#ifndef _VERSION_H
#define _VERSION_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "arm_reg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define SelfReg_DeviceMode 			"ZBH-BIHEE A30"
#define SelfReg_DeviceVer 			"A30_3.0"
#define ZDT_DeviceMode 			"BIHEE A30"
#define ZDT_DeviceSWVer 			"A30_3.0"
#define ZDT_DeviceHWVer 			"Z685 V2.1"
#define ZDT_PlatVer 			"MOCOR16A"
#define ZDT_PlatRAM 			"16M"
#define ZDT_PlatROM 			"24M"
#define ZDT_PlatCPU 			"500MHz"
typedef enum
{
    PLATFORM_VERSION,
    PROJECT_VERSION,
    MMI_VERSION,
    AP_SVN_VERSION,
    CP_SVN_VERSION,
    BASE_VERSION,
    HW_VERSION,
    CALIBRATION_VERSION,
    DOWNLOAD_VERSION,
    BUILD_TIME,
    LOG_VERSION,
    MEMORY_NAME,
#if defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)
    GE2_VER,
#endif
    MAX_VERSION_NUM 
}VERSION_TYPE_E;


const char*  VERSION_GetInfo(uint32 type);

uint32  VERSION_GetInfoNum(void);


/*****************************************************************************/
//  Description:    return the project version
//	Global resource dependence:
//  Author:         fancier.fan
//	Note: 2004 1 16
/*****************************************************************************/
const char * COMMON_GetProjectVersionInfo(void);     // return software version 
/*+CR79304*/
/*****************************************************************************/
//  Description:    return the version specified by version_type param
//	Global resource dependence:
//  Author:         minqian.qian
//	Note: 
/*****************************************************************************/
const char * COMMON_GetVersionInfo(
                uint8 version_type      //refer to VERSION_TYPE_E
                );
/*-CR79304*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /* End of version.h*/