
/*****************************************************************************
** File Name:      mmijava_dynamicmenufun.h                                                
** Author:         murphy.xie   
** Date:           2012/08/15
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe dynmic to mainmenu function
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE              NAME           DESCRIPTION                              *
** 2012/08/15        murphy.xie     Create                                 *
******************************************************************************/
#ifndef  _MMIJAVA_DYNAMICMENU_H_
#define  _MMIJAVA_DYNAMICMENU_H_
#include "mmk_type.h"
#include "sci_types.h"
#include "mmi_mainmenu_synchronize.h"

#ifdef DYNAMIC_MAINMENU_SUPPORT

/*****************************************************************************/
//  Description : delete the suite entry point from main menu according the suite ID
//  Global resource dependence : none
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_DeleteSuiteRecordInMainmenu(int32 suite_id);

/*****************************************************************************/
//  Description : delete all the suite entry points from main menu
//  Global resource dependence : none
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_DeleteAllSuitesRecorInMainmenu(void);

/*****************************************************************************/
//  Description : add suite to mainmenu
//  Global resource dependence : none
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_AddSuiteRecordToMainmenu(
                                          int32 suite_id,
                                          GUIMENU_FILE_DEVICE_E device_type
                                      );

/*****************************************************************************/
//  Description : Read preinstall icons information from rom zip and
//      generate preinstall MIDlets meinmenu.
//  @param res_id, the resource id of preinstall data.
//  NOTE: 1) On your side, you must compress the preinstall
//      resource(including jad, jar and icon) into standard zip format.
//  Global resource dependence : none
//  Author: Sean Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_GeneratePreinstallMainmenu(MMI_DATA_ID_T res_id);

/*****************************************************************************/
//  Description : Update preinstall suite id to the prameter data of main menu.
//  @param suite_id, the suite id.
//  Global resource dependence : none
//  Author: Sean Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_UpdatePreinstallSuiteId(int32 suite_id);

/*****************************************************************************/
//  Description : To check if given suite id is in record.
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsInstalledSuiteInRecord(int suiteId);

/*****************************************************************************/
//  Description : Init installed suite NV
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_InstalledSuiteNV_Init(void);

/*****************************************************************************/
//  Description : Add SD Card Suites To Mainmenu
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_AddSDCardSuitesToMainmenu(void);

#endif

#endif //_MMIJAVA_DYNAMICMENU_H_

