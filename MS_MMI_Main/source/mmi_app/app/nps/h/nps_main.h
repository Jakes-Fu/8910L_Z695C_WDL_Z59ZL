/***************************************************************************
** File Name:      hellosprd.h                                             *
** Author:                                                                 *
** Date:           4/01/2009                                               *
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 4/2009         Xiaoqing.Lu      Create                                  *
**																		   *
****************************************************************************/
#ifndef _NPS_MAIN_H_
#define _NPS_MAIN_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
//#include "common_mdu_def.h"
#include "sci_types.h"
#include "mmk_type.h"
#include "mmi_common.h"


/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef _cplusplus
	extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : register hello module nv len and max item
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMI_RegNPSNv(void);

/*****************************************************************************/
// 	Description : register hello module nv len and max item
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC char* MMINPS_GetHttpParamUrl(void);

/*****************************************************************************/
// 	Description : register hello module nv len and max item
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMINPS_HttpParamUrlFree(char* param_url);

/*****************************************************************************/
//  Description : 打开反馈窗口                        
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPINPS_OpenCompleteWindow(void);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif//_NPS_MAIN_H_