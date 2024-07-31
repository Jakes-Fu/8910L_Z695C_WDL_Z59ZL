/*****************************************************************************
** File Name:      mmijava_backgroundwin.h                                                
** Author:         murphy.xie   
** Date:           2012/08/15
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe backgroun function
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE              NAME           DESCRIPTION                              *
** 2012/08/15        murphy.xie     Create                                 *
******************************************************************************/

#ifndef  _MMIJAVA_BACKGROUNDWIN_H_
#define  _MMIJAVA_BACKGROUNDWIN_H_

#if defined(SUPPORT_BACKGROUND_RUN)
#include "mmk_type.h"

#if defined(JAVA_SUPPORT_TYPE_SVM)
 
/**************************************************************************************************/
// Description: Interface for enter red key dialog when press red key.
// Date:2011-09-01
// Author:Martin.Zhao
// Note:
//*************************************************************************************************/
PUBLIC void MMIJAVA_EnterResumeWin(void);

#endif

// Description: Interface for enter red key dialog when press red key.
// Date:2011-09-01
// Author:Martin.Zhao
// Note:
//*************************************************************************************************/
PUBLIC void MMIJAVA_OpenRedkeyDialog(void);

#endif //SUPPORT_BACKGROUND_RUN

#endif //_MMIJAVA_BACKGROUNDWIN_H_




