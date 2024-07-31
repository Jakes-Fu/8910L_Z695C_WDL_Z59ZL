/*****************************************************************************
** File Name:      mmijava_midletinfowin.h                                                
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

#ifndef  _MMIJAVA_MIDLETINFO_H_
#define  _MMIJAVA_MIDLETINFO_H_

#include "mmk_type.h"
#include "sci_types.h"

/*****************************************************************************/
//  Description : open java midlet detail info window
//  Global resource dependence : none
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_OpenMidletInfoWin(int event_id);

/*****************************************************************************/
//  Description :Check whether this menu_id should display on dynamic popup menu, return
//  TRUE if display, otherwise return FALSE
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_ShowSuiteInfo(int  suite_id);

#endif //_MMIJAVA_MIDLETINFO_H_





