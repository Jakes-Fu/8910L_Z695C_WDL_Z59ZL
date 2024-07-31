/*****************************************************************************
** File Name:      mmijava_suitefun.h                                                
** Author:         murphy.xie   
** Date:           2012/08/15
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe java application function
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE              NAME           DESCRIPTION                              *
** 2012/08/15        murphy.xie     Create                                   *
******************************************************************************/

#ifndef  _MMIJAVA_SUITEFUN_H_
#define  _MMIJAVA_SUITEFUN_H_

/*****************************************************************************/
//  Description :Check whether this menu_id should display on dynamic popup menu, return
//  TRUE if display, otherwise return FALSE
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsPreinstallSuite(int suite_id);

/*****************************************************************************/
//  Description: Start a MIDlet suite with conflict checking.
//  @param suite_id, the suite id of midlet suite which you want to run.
//  @param midlet_num, the midlet number of the suite you want to run.
//         Set the default value of 1 when midlet_num is passed by less or equal to zero.
//  TRUE if display, otherwise return FALSE
//  @return TRUE if it is successful to start the suite.
//          FASLE if it is failed to start the suite.
//  Author: Sean Xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_StartSuiteInternal(int suite_id, int midlet_num);

/*****************************************************************************/
//  Description: Start a MIDlet suite without conflict checking.
//  @param suite_id, the suite id of midlet suite which you want to run.
//  @param midlet_num, the midlet number of the suite you want to run.
//         Set the default value of 1 when midlet_num is passed by less or equal to zero.
//  TRUE if display, otherwise return FALSE
//  @return TRUE if it is successful to start the suite.
//          FASLE if it is failed to start the suite.
//  Author: Sean Xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_StartSuite(int suite_id, int midlet_num);

/*****************************************************************************/
//  Description : Delete Midlet suite by suite ID from option menu
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_DeleteSuite(int suite_id);

#ifdef SUPPORT_DELETE_ALL_FUNCTION
/*****************************************************************************/
//  Description : Delete all Midlet suites from option menu
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_DeleteAll(void);
#endif

/*****************************************************************************/
//  Description :return suite_id according to selected index of listbox
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC int MMIJAVA_GetSuiteID(uint32 _index);

#if defined (NATIVE_AMS) && defined (AMS_TCK_RUNNER)
/*****************************************************************************/
//  Description : Start jvm for tck test
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_StartTCK(void);
#endif
/*****************************************************************************/
//  Description : handle the event of clicking on java icon
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_OpenMainMenu(void);

#endif //_MMIJAVA_SUITEFUN_H_


