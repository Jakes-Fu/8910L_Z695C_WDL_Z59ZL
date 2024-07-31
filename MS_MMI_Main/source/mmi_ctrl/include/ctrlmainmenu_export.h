/*****************************************************************************
** File Name:      ctrlmainmenu_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _CTRLMAINMENU_EXPORT_H_
#define _CTRLMAINMENU_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"
#include "mmitheme_mainmenu.h"

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
#define MSG_CTRLMAINMENU_SET_MOVE_STATE         (MSG_USER_START + 1)
#define MSG_CTRLMAINMENU_CANCEL_MOVE_ICON       (MSG_USER_START + 2)
#define MSG_CTRLMAINMENU_GET_ID                 (MSG_USER_START + 3)
#define MSG_CTRLMAINMENU_IS_MOVE_STATE          (MSG_USER_START + 4)
#define MSG_CTRLMAINMENU_SET_RECT               (MSG_USER_START + 5)
#define MSG_CTRLMAINMENU_RELOAD_ITEM            (MSG_USER_START + 6)
#define MSG_CTRLMAINMENU_GET_CUR_INDEX          (MSG_USER_START + 7)
#define MSG_CTRLMAINMENU_GET_OFFSET_Y           (MSG_USER_START + 8)
#define MSG_CTRLMAINMENU_IS_MOVE_MODE           (MSG_USER_START + 9)
#define MSG_CTRLMAINMENU_SET_MOVE_MODE          (MSG_USER_START + 10)
#define MSG_CTRLMAINMENU_IS_COLOR_THEME_MODE    (MSG_USER_START + 11)
#define MSG_CTRLMAINMENU_SET_COLOR_THEME_MODE    (MSG_USER_START + 12)
#define MSG_CTRLMAINMENU_REDRAW                 (MSG_USER_START + 13)
#define MSG_CTRLMAINMENU_LOSE_FOCUS_REDRAW      (MSG_USER_START + 14)
#define MSG_CTRLMAINMENU_IS_UNINSTALLABLE       (MSG_USER_START + 15)
/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : CTRLMAINMENU_GetRect
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void CTRLMAINMENU_GetRect (
                                  MMI_HANDLE_T ctrl_handle, 
                                  GUI_RECT_T* rect_ptr //in/out
                                  );

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC MMIMAINMENU_TYPE_E CTRLMAINMENU_GetStyle(
                                                MMI_HANDLE_T ctrl_handle
                                                );

/*****************************************************************************/
// 	Description : CTRLMAINMENU_GetlayerInfo
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void CTRLMAINMENU_GetlayerInfo (
                                       MMI_HANDLE_T ctrl_handle, 
                                       GUI_LCD_DEV_INFO* layer_info_ptr //in/out
                                       );

/*****************************************************************************/
//  Description : 设置菜单是否处于移动状态
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMAINMENU_InitMoveState(
                                         MMI_HANDLE_T  ctrl_handle
                                         );

/*****************************************************************************/
// 	Description : CTRLMAINMENU_CancelMoveIcon
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMAINMENU_CancelMoveIcon(
                                       MMI_CTRL_ID_T        ctrl_id
                                       );

/*****************************************************************************/
// 	Description : CTRLMAINMENU_GetId
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void CTRLMAINMENU_GetId(
                       MMI_CTRL_ID_T        ctrl_id,
                       MMI_MENU_GROUP_ID_T  *group_id_ptr,  //group id
                       MMI_MENU_ID_T        *menu_id_ptr    //menu id
                       );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMAINMENU_SetColorThemeMode(
                                   MMI_CTRL_ID_T        ctrl_id,
                                   BOOLEAN              is_color_theme_mode
                                   );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMAINMENU_IsColorThemeMode(
                                   MMI_CTRL_ID_T        ctrl_id
                                   );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMAINMENU_IsMoveState(
                                   MMI_CTRL_ID_T        ctrl_id
                                   );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMAINMENU_IsMoveMode(
                                   MMI_CTRL_ID_T        ctrl_id
                                   );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMAINMENU_IsUninstallable(
                                   MMI_CTRL_ID_T        ctrl_id
                                   );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMAINMENU_SetMoveMode(
                                   MMI_CTRL_ID_T        ctrl_id,
                                   BOOLEAN              is_move_mode
                                   );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMAINMENU_Redraw(
                                   MMI_CTRL_ID_T        ctrl_id
                                   );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMAINMENU_LoseFocusRedraw(
                                   MMI_CTRL_ID_T        ctrl_id
                                   );

/*****************************************************************************/
//  Description : get page manager
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void* CTRLMAINMENU_GetPM(
                            MMI_CTRL_ID_T      ctrl_id
                            );
#ifdef QBTHEME_SUPPORT							
/*****************************************************************************/
//  Description : qbtheme ReloadItem
//  Global resource dependence : 
//  Author: guodong.liu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMAINMENU_ReloadItem(
                                   MMI_CTRL_ID_T        ctrl_id
                                   );
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
