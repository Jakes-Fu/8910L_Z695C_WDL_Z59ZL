/****************************************************************************
** File Name:      mmigps_internal.h                                       *
** Author:         jian.ma                                                 *
** Date:           07/19/2007                                              *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/19/2007     jian.ma     Create                                       *
**                                                                         *
****************************************************************************/

#ifndef _MMIGPS_INTERNAL_H_    
#define _MMIGPS_INTERNAL_H_    

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "sci_types.h"
#include "freq_cfg.h"
#include "mmi_appmsg.h"
#include "sfs.h"
#include "ffs.h"
#include "os_api.h"
#include "sci_api.h"
#include "mmigps_osapi.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Discription:   gps task
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC void GPSTask(uint32 argc, void* argv);

/*****************************************************************************/
//	Description : Is permit play
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIGPS_IsPermitPlay(void );

/*****************************************************************************/
//	Description : open gps main win
//	Global resource dependence : none
//	Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E GPS_CopyResource(uint16* src_path_ptr,uint16* dest_path_ptr);


PUBLIC int32 MMIGPS_DelMessage( int32 im,unsigned long iP1,unsigned long ip2 );

PUBLIC SFS_ERROR_E MMIGPS_CopyDir(uint16 * sourceDir,uint16 * destDir);

PUBLIC SFS_ERROR_E MMIGPS_CopyOneFile( uint16 * sourceDir,uint16 * destDir );

/*****************************************************************************/
//	Description : get gps task id
//	Global resource dependence : none
//	Author:juan.zhang
//	Note:
/*****************************************************************************/
PUBLIC BLOCK_ID MMIGPS_GetGPSTaskID(void);

/*****************************************************************************/
//	Description : set gps task id
//	Global resource dependence : none
//	Author:juan.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIGPS_SetGPSTaskID(BLOCK_ID gps_task);

/*****************************************************************************/
//	Description : set GPS state
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIGPS_SetGpsState(uint32 state);

/*****************************************************************************/
//	Description : get  GPS state
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC int32  MMIGPS_GetGpsState( void );

/*****************************************************************************/
//	Description : Is gps Opened
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC int MMIGPS_IsGpsOpened(void);

/*****************************************************************************/
//	Description : Set freq in Gps
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIGPS_SetFreqInGPS(PROD_FREQ_INDEX_E index);

/*****************************************************************************/
//	Description : restore Freq 
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIGPS_RestoreFreqInGPS(void);

/*****************************************************************************/
//	Description : set permit play
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIGPS_SetPermitPlay(BOOLEAN is_permit_play );

/*****************************************************************************/
//	Description : delete message
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIGPS_DelMessage( int32 im,unsigned long iP1,unsigned long ip2 );

/*****************************************************************************/
//	Description : close GPS win
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIGPS_CloseWind( void );

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIGPS_StopPlay(void);


/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIGPS_OsFree( void );

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC void GPS_get_app_queue(void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef   __cplusplus
    }
#endif

#endif