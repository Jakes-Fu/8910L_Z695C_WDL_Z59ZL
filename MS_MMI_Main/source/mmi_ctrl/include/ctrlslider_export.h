/*****************************************************************************
** File Name:      ctrlslider_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Creat
******************************************************************************/

#ifndef _CTRLSLIDER_EXPORT_H_
#define _CTRLSLIDER_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"

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
typedef struct
{
    GUI_BOTH_RECT_T     both_rect;  /*!<ºáÊúÆÁÇøÓò*/
} GUISLIDER_INIT_DATA_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get line size
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC int16 CTRLSLIDER_GetLineSize(
                                    MMI_HANDLE_T ctrl_handle
                                    );

/*****************************************************************************/
//  Description : get page size
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC int16 CTRLSLIDER_GetPageSize(
                                    MMI_HANDLE_T ctrl_handle
                                    );

/*****************************************************************************/
//  Description : get range max
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC int16 CTRLSLIDER_GetRangeMax(
                                    MMI_HANDLE_T ctrl_handle
                                    );

/*****************************************************************************/
//  Description : get range min
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC int16 CTRLSLIDER_GetRangeMin(
                                    MMI_HANDLE_T ctrl_handle
                                    );

/*****************************************************************************/
//  Description : get current position
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC int16 CTRLSLIDER_GetSelection(
                                     MMI_HANDLE_T ctrl_handle
                                     );

/*****************************************************************************/
//  Description : set line size
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetLineSize(
                                      MMI_HANDLE_T ctrl_handle,
                                      int16 line_size
                                      );

/*****************************************************************************/
//  Description : set page size
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetPageSize(
                                      MMI_HANDLE_T ctrl_handle,
                                      int16 page_size
                                      );

/*****************************************************************************/
//  Description : set max
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetRangeMax(
                                      MMI_HANDLE_T ctrl_handle,
                                      int16 max
                                      );

/*****************************************************************************/
//  Description : set min
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetRangeMin(
                                      MMI_HANDLE_T ctrl_handle,
                                      int16 min
                                      );

/*****************************************************************************/
//  Description : set current position
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetSelection(
                                       MMI_HANDLE_T ctrl_handle,
                                       int16 cur
                                       );

/*****************************************************************************/
//  Description : set display style, vertical or horizontal
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetVerDisp(
                                     MMI_HANDLE_T ctrl_handle,
                                     BOOLEAN is_vertical
                                     );

/*****************************************************************************/
// 	Description : create the slider control
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note: return ctrl handle
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLSLIDER_Create(
                                      MMI_HANDLE_T win_handle,
                                      MMI_HANDLE_T parent_ctrl_handle,
                                      MMI_CTRL_ID_T ctrl_id
                                      );

/*****************************************************************************/
//  Description : set need thumb or not
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetNeedThumb(
                                      MMI_HANDLE_T ctrl_handle,
                                      BOOLEAN is_true
                                      );

/*****************************************************************************/
//  Description : set need channel foreground or not
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetNeedChannelFg(
                                           MMI_HANDLE_T ctrl_handle,
                                           BOOLEAN is_true
                                           );

/*****************************************************************************/
//  Description : set need foreground or not
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetNeedBg(
                                    MMI_HANDLE_T ctrl_handle,
                                    BOOLEAN is_true
                                    );

/*****************************************************************************/
//  Description : set channel size
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetChannelSize(
                                         MMI_HANDLE_T ctrl_handle,
                                         uint16 channel_width,
                                         uint16 channel_height
                                         );

/*****************************************************************************/
//  Description : set thumb size
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetThumbSize(
                                       MMI_HANDLE_T ctrl_handle,
                                       uint16 thumb_width,
                                       uint16 thumb_height
                                       );

/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SliderSetRect(
                                        MMI_HANDLE_T ctrl_handle, 
                                        const GUI_RECT_T* rect_ptr
                                        );

/*****************************************************************************/
//  Description : display slider control
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_UpdateSlider(
                                       MMI_HANDLE_T ctrl_handle
                                       );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
