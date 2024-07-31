/*****************************************************************************
** File Name:      mmijava_installprocesswin.h                                                
** Author:         murphy.xie   
** Date:           2012/08/15
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe install process function
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE              NAME           DESCRIPTION                              *
** 2012/08/15        murphy.xie     Create                                 *
******************************************************************************/
#ifndef  _MMIJAVA_INSTALLPROCESSWIN_H_
#define  _MMIJAVA_INSTALLPROCESSWIN_H_
#include "mmk_type.h"
#include "sci_types.h"

/*****************************************************************************/
//  Description : Finalize related installation information.
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetInstallCanbeStoped(BOOLEAN flag);

/*****************************************************************************/
//  Description : get installing state
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsInstalling(void);

/*****************************************************************************/
//  Description : get install progress flag
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_GetInstallProgress(void);

/*****************************************************************************/
//  Description : reset install progress flag
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_ResetInstallProgress(void);
/*****************************************************************************/
//  Description : set if can be stoped flag
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetCanbeStopedFlag(BOOLEAN flag);

/*****************************************************************************/
//  Description : Initialize the install pending event as unused status.
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_InitInstallPendingEvent(void);

/*****************************************************************************/
//  Description : Handle install pending event if needed.
//  For SVM, we need to wait VM thread exit completely before launch
//  midlet after installation. In this case, set install pending event
//  and handle it when vm thread exit.
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_HandleInstallPendingEvent(void);

#endif //_MMIJAVA_INSTALLPROCESSWIN_H_
