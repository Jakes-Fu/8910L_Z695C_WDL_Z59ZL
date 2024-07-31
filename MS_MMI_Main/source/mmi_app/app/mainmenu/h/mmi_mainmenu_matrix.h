/*****************************************************************************
** File Name:      mmi_mainmenu_matrix.h                                     *
** Author:                                                                   *
** Date:           10/17/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2012      xiaoqing.lu              Create
******************************************************************************/

#ifndef _MMI_MAINMENU_MATRIX_H_
#define _MMI_MAINMENU_MATRIX_H_ 
#ifdef MATRIX_MAIN_MENU_SUPPORT
/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlmainmenu_export.h"
#include "mmi_link.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef uint32 MMIMAINMENU_STATE_T;
#define MMIMAINMENU_INSTALL_STATE              0x00000001
#define MMIMAINMENU_PRE_INSTALL_STATE          0x00000002
#define MMIMAINMENU_UPDATE_STATE               0x00000004
#define MMIMAINMENU_UNINSTALL_STATE            0x00000008
#define MMIMAINMENU_LAUNCH_STATE               0x00000010
#define MMIMAINMENU_PERMISSION_SETTING_STATE   0x00000020
#define MMIMAINMENU_GET_INFO_STATE             0x00000040

typedef enum
{
    MAINMENU_BANNER_TYPE_INSTALL_CANCEL = 0,
    MAINMENU_BANNER_TYPE_INSTALLING,
    MAINMENU_BANNER_TYPE_UNINSTALLING,
    MAINMENU_BANNER_TYPE_RUNNING,
    MAINMENU_BANNER_TYPE_MAX
}MAINMENU_BANNER_TYPE_E;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
 PUBLIC void SetCurColorIndex(uint8 cur_color_theme_index);
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_MatrixRegister(
                                          CTRLMAINMENU_PM_APIS_T *apis
                                          );

/*****************************************************************************/
//  Description : get softkey layer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO MMIMAINMENU_GetSoftkeyLayer(void);

/*****************************************************************************/
//  Description : Init Menu Id Array
//  Global resource dependence :
//  Author: 
//  RETRUN: 
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIMAINMENU_GetALLShortCut( uint8  *shortcut_menu,uint8 max);

PUBLIC void MMIMAINMENU_InitMenuIdArray(void);

#ifdef JAVA_SUPPORT_MYRIAD
/*****************************************************************************/
//  Description : Append Preinstall Item
//  Global resource dependence :
//  Author: 
//  RETRUN: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_AppendPreinstallItem(void);
#endif

/*****************************************************************************/
//  Description : Get Menu Id By Index
//  Global resource dependence :
//  Author: 
//  RETRUN: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMAINMENU_GetMenuIdByIndex(
    uint8 index
);

/*****************************************************************************/
//  Description : Get Item Count
//  Global resource dependence :
//  Author: 
//  RETRUN: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMAINMENU_GetItemCount(void);

/*****************************************************************************/
//  Description : Set Pre State
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_SetPreState(void);

/*****************************************************************************/
//  Description : Set Cur Item Index
//  Global resource dependence :
//  Author: 
//  RETRUN: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_SetCurItemIndex(
    uint16 cur_item_index
);

/*****************************************************************************/
//  Description : Get Cur Item suite_id, mainmenu_id
//  Global resource dependence :
//  Author: 
//  RETRUN: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_GetCurItemInfo(
    int32 *suite_id_ptr,
    uint32 *menu_id_ptr
);

#if defined DYNAMIC_MAINMENU_SUPPORT
/*****************************************************************************/
//  Description : ÑWâÿ–€êF∆…µ»≥X
//  Global resource dependence : 
//  Author: cheney.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_DeleteCurrentPreloadItem(void);
#endif

/*****************************************************************************/
//  Description : Get Item Info By Suite Id
//  Global resource dependence :
//  Author: 
//  RETRUN: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_GetItemInfoBySuiteId(
    int32 suite_id,        //[in]
    uint32 *menu_id_ptr,   //[out]
    uint8 *item_index_ptr  //[out]
);

/*****************************************************************************/
//  Description : Get Suite Id By Menu Id
//  Global resource dependence :
//  Author: 
//  RETRUN: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_GetSuiteIdByMenuId(
    uint32 menu_id,      //[in]
    int32 *suite_id_ptr  //[out]
);

/*****************************************************************************/
//  Description : Del Menu Id
//  Global resource dependence :
//  Author: 
//  RETRUN: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_DelMenuId(
    uint32 menu_id
);

/*****************************************************************************/
//  Description : Add Menu Id
//  Global resource dependence :
//  Author: 
//  RETRUN: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_AddMenuId(
    uint32 menu_id
);

/*****************************************************************************/
//  Description : Set State
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_SetState(
    MMIMAINMENU_STATE_T state,
    BOOLEAN is_true,
    DPARAM  param
);

/*****************************************************************************/
//  Description : Get State
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_GetState(
    MMIMAINMENU_STATE_T state
);

/*****************************************************************************/
//  Description : Set Install Progress
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_SetInstallProgress(
    uint8 install_progress
);

/*****************************************************************************/
//  Description : Get Install Progress
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIMAINMENU_GetInstallProgress(void);

/*****************************************************************************/
//  Description : Set Install App Menu Id
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_SetInstallAppMenuId(
    uint32 menu_id
);

/*****************************************************************************/
//  Description : Return Mainmenu By Java
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_ReturnMainmenuByJava(void);

/*****************************************************************************/
//  Description : Is Focus On Install Item
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_IsFocusOnInstallItem(void);

/*****************************************************************************/
//  Description : Is PreInstall Item
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_IsPreInstallItem(void);

#ifdef ATEST_SUPPORT
extern PUBLIC GUI_RECT_T Atest_MatrixMenuGetItemRect(void *menu_ctrl_ptr, int16 item_index);
#endif

/*****************************************************************************/
//  Description : Get Preview Color Theme Index
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIMAINMENU_GetPreviewColorThemeIndex(void);

/*****************************************************************************/
//  Description : Update
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_Update(void);

/*****************************************************************************/
//  Description : Is Item Layer Active
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_IsItemLayerActive(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMI_MAINMENU_MATRIX_H_
#endif // MATRIX_MAIN_MENU_SUPPORT
/*@}*/
