/***************************************************************************
** File Name:      sample_export.h                                             *
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
#ifndef _NPS_EXPORT_H_
#define _NPS_EXPORT_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

#include "sci_types.h"
#include "mmk_type.h"

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
// 	Description : Register hello menu group
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPINPS_IsTimerArrive(void);

/*****************************************************************************/
// 	Description : prompt NPS select screen
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPINPS_PromptNPSJudgeFunction(void);

/*****************************************************************************/
// 	Description : change prompt timer for test
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPINPS_PromptTimerChange(BOOLEAN is_change_time,
                                                        uint32 one_hour,
                                                        uint16 hour_count);
/*****************************************************************************/
// 	Description : NPS restore factory setting
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPINPS_SettingNPSToFactory(void);
/*****************************************************************************/
// 	Description : Register hello menu group
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPINPS_IsAlreadyPrompt(void);

/*****************************************************************************/
// 	Description : Register hello menu group
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPINPS_StartNPSTimer(void);

/*****************************************************************************/
//  Description : open hello sprd windows
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPINPS_InitModule(void);

/*****************************************************************************/
//  Description : 打开反馈询问窗口                        
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPINPS_OpenMainMenuSelectWindow(BOOLEAN is_select_prompt) ;
/*****************************************************************************/
//  Description : 打开正在发送窗口                        
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPINPS_OpenFeedbackSendingWindow(void);
/*****************************************************************************/
//  Description : 打开反馈窗口                        
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPINPS_OpenMainMenuWindow(BOOLEAN is_prompt);
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif//_NPS_EXPORT_H_