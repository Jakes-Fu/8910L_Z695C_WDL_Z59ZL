/****************************************************************************
** File Name:      mmigps_export.h                                         *
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

#ifndef _MMIGPS_WINTAB_H_    
#define _MMIGPS_WINTAB_H_    

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "sci_types.h"
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
//地图中的一个点的结构
typedef struct
{
	uint16 x;	//点在地图中的x坐标值
	uint16 y;	//点在地图中的y坐标值
}GPS_POINT_T;

/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description : open gps main win
//	Global resource dependence : none
//	Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIGPS_OpenMainWin(void);

/*****************************************************************************/
//	Description : close GPS task
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIGPS_CloseGPSTask(void);

/*****************************************************************************/
//	Description : Is gps Opened
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIAPIGPS_IsGpsOpened(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef   __cplusplus
    }
#endif

#endif