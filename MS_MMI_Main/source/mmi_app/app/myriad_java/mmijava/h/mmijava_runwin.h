/*****************************************************************************
** File Name:      mmijava_runwin.h
** Author:         murphy.xie
** Date:           2012/08/15
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe DATA of alarm
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE              NAME           DESCRIPTION                              *
** 2012/08/15        murphy.xie     Create                                 *
******************************************************************************/

#ifndef  _MMIJAVA_RUNWIN_H_
#define  _MMIJAVA_RUNWIN_H_
#include "mmk_type.h"
#include "sci_types.h"

/*****************************************************************************/
//  Description : Entry point of running appliaction window
//  Global resource dependence :
//  Author: Martin.Zhao
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_CreateRunningWin(void);
/*****************************************************************************/
//  Description : close running window
//  Global resource dependence :
//  Author: Martin.Zhao
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_CloseRunningWin(void);

/*****************************************************************************/
//  Description : MMIJAVA_SetIsJavaEnablePause
//  Global resource dependence :
//  Author:murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetJavaIsEnablePause(BOOLEAN is_enable);

#endif //_MMIJAVA_RUNWIN_H_




