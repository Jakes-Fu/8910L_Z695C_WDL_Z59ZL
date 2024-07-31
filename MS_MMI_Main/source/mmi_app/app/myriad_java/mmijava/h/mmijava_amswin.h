/*****************************************************************************
** File Name:      mmijava_amswin.h                                                
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

#ifndef  _MMIJAVA_AMSWIN_H_
#define  _MMIJAVA_AMSWIN_H_


#include "sci_types.h"
#include "mmk_type.h"



/*****************************************************************************/
//  Description :add suites info to listbox,include icon and name
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
//  param isFocus:add the param for - if window is on focus,use midlet icon,or-else
    //use default.why:when run delete operation,jbed thread and mmi thread may read-write files
    //at same time,crash may occur
/*****************************************************************************/
PUBLIC void MMIJAVA_AMSUpdateList(
                                                  MMI_MESSAGE_ID_E msg_id,
                                                  void * param,
                                                  BOOLEAN isFocus
                                              );

/*****************************************************************************/
//  Description :add items to listbox according to the count of midlet in a suite
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_AMSOpenList(
                                    MMI_WIN_ID_T win_id,
                                    MMI_CTRL_ID_T ctrl_id,
                                    int suite_id
                                   );

/*****************************************************************************/
//  Description :add info midlets in a suite to listbox,include icon and name
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
//  param isFocus:add the param for - if window is on focus,use midlet icon,or-else
    //use default.why:when run delete operation,jbed thread and mmi thread may read-write files
    //at same time,crash may occur
/*****************************************************************************/
PUBLIC void MMIJAVA_MidletUpdateList(
                                       MMI_MESSAGE_ID_E msg_id,
                                       void * param,
                                       int suite_id,
                                       BOOLEAN isFocus
                                      );


/**************************************************************************************************
  * Description: MMIJAVA_OpenAmsWin
  * Input Paras:
  * Output Paras: 
  * note : call the func when deleting all
  *************************************************************************************************/
PUBLIC void  MMIJAVA_EnterMidletListWin(uint32 suiteid);

//FuncWin Entry
PUBLIC void MMIJAVA_EnterAmsWin(void);


#endif




