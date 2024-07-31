/*****************************************************************************
** File Name:      mmijava_permissionsetwin.h                                                
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

#ifndef  _MMIJAVA_PERMISSIONSETWIN_H_
#define  _MMIJAVA_PERMISSIONSETWIN_H_
#include "mmk_type.h"
#include "sci_types.h"

/**************************************************************************************************
  * Description: MMIJAVA_OpenPermissionWin
  * Input Paras:
  * Output Paras: 
  * note : call the func when deleting all
  *************************************************************************************************/
PUBLIC void  MMIJAVA_OpenPermissionWin(int eventid);

/**************************************************************************************************
  * Description: Handle permission MMI_NAMS_EVENT_PERMISSIONS_ANSWERS event
  * Input Paras:
  * Output Paras: 
  * note : call the func when deleting all
  *************************************************************************************************/
PUBLIC void  MMIJAVA_HandlePermissionEvent(void);
/**************************************************************************************************
  * Description: MMIJAVA_OpenPermissionWin
  * Input Paras:
  * Output Paras: 
  * note : call the func when deleting all
  *************************************************************************************************/
PUBLIC void  MMIJAVA_SetPermisson(int suite_id);



#endif //_MMIJAVA_PERMISSIONSETWIN_H_




