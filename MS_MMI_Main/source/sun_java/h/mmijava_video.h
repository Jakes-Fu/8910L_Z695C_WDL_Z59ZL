/*****************************************************************************
** File Name:      mmijava_video.h                                          *
** Author:         hui.zhao                                               *
** Date:           6/2009                                                    *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 6/2009       hui.zhao            Create                                *
******************************************************************************/
#ifndef _MMIJAVA_VIDEO_H_
#define _MMIJAVA_VIDEO_H_

#ifdef JAVA_SUPPORT_SUN
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : display notify处理
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
PUBLIC void MMIJAVA_CallbackFrameEndNotifyProc(void);

/*****************************************************************************/
//  Description : display notify处理
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
PUBLIC void MMIJAVA_CallbackEndNotifyProc(void);

BOOLEAN java_HandleVideoMessage(uint16 msg_id);

BOOLEAN mmijava_IsVPRunning(void);

int32 mmijava_GetVPLocation(GUI_RECT_T *rect);

int32 mmijava_SetVideoVolume(int32 volume);
#endif
#endif