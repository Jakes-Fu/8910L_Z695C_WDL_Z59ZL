/***************************************************************************
** File Name:      ctrlcheckbox_export.h                                   *
** Author:         hua.fang                                                *
** Date:           08/03/2011                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2011        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _CTRLCHECKBOX_EXPORT_H_
#define _CTRLCHECKBOX_EXPORT_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "cafcontrol.h"
#include "mmk_type.h"

#include "ctrlbutton_export.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
/*! @struct GUICHECKBOX_INIT_DATA_T
@brief checkbox控件初始化数据
*/
typedef struct
{
    GUI_BOTH_RECT_T     both_rect;  /*!<横竖区域*/
} GUICHECKBOX_INIT_DATA_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief 获得button是否处于选中状态
@author Jassmine
@param ctrl_id [in] 控件id
@return 是否选中
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLCHECKBOX_GetSelect(
                                     MMI_CTRL_ID_T    ctrl_id
                                     );

/***************************************************************************//*!
@brief 设置button是否处于选中状态
@author Jassmine
@param ctrl_id   [in] 控件id
@param is_select [in] 是否选中
@return
@note 
*******************************************************************************/
PUBLIC void CTRLCHECKBOX_SetSelect(
                                  MMI_CTRL_ID_T     ctrl_id,
                                  BOOLEAN           is_select
                                  );


/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif  // _CTRLCHECKBOX_EXPORT_H_
