/*****************************************************************************
** File Name:      headeranddivider.h                                               *
** Author:                                                                   *
** Date:                                                          *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _CTRLHEADERANDDIVIDER_H_
#define _CTRLHEADERANDDIVIDER_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlheaderanddivider_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
// headeranddivider type info
#define CTRL_HEADERANDDIVIDER_TYPE     (HEADERANDDIVIDER_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// headeranddivider init param
typedef struct
{
    	CTRLBASE_INIT_PARAM_T       	base_ctrl_param;
  	MMI_TEXT_ID_T		        	text_id;         /*!< text_id */

    // self inif param ...
} CTRLHEADERANDDIVIDER_INIT_PARAM_T;

//headeranddivider control
typedef struct _headeranddivider_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;
    
    //标题文本
    GUIHEADERANDDIVIDER_TEXT_DATA_E        	text_type;
    MMI_TEXT_ID_T               	text_id;
    MMI_STRING_T                	headeranddivider_str;          /*!< headeranddivider string */ 
    GUISTR_STYLE_T              	str_style;          /*!< 文本风格 */ 

    uint16                      			headeranddivider_text_margin;  /*!< 边缘 */ 
    uint16                      			icon_space;         /*!< icon间隔 */ 

    //theme
    BOOLEAN                     		is_need_bg;         /*!< 是否有背景 */ 
    MMI_IMAGE_ID_T              	bg_image;           /*!< 背景图片 */ 
    GUI_COLOR_T                 	bg_color;           /*!< 背景颜色 */ 
    GUI_COLOR_T                 	line_color;           
    GUI_RECT_T				line_rect;
	
    //长文本滚动
    GUIHEADERANDDIVIDER_SCROLL_TYPE_E      scroll_type;        /*!< 滚动条模式 */ 
    BOOLEAN                     is_text_overstep;   /*!< 当文本的宽度超过区域的时候，该值为TRUE */ 
    uint8                       scroll_timer_id;    /*!< 滚动条timer_id */ 
    uint32                      scroll_time;        /*!< 循环滚动时间 */ 
    uint32                      scroll_pixel_step;  /*!< 每次滚动的像素数 */ 
    uint16                      scroll_char_offset; /*!< 字符offset */  //record the roll num of chars (roll one char each time  
    uint16                      scroll_pixel_offset;/*!< 像素offset */  //headeranddivider string pixel offset

    GUI_LCD_DEV_INFO            divider_layer_info; //label layer
	
    MMI_HANDLE_T                win_handle;         /*!< 窗口handle */ 

    // self member ...
} CTRLHEADERANDDIVIDER_OBJ_T;

// headeranddivider class for public or protected method
typedef struct _headeranddivider_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    // self vtbl ...
} CTRLHEADERANDDIVIDER_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get headeranddivider type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T HEADERANDDIVIDER_GetType(void);
/***************************************************************************//*!
@brief 设置headeranddivider文本
@author Liqing.peng
@param headeranddivider_ptr            [in]    headeranddivider指针
@param wstr_ptr             [in]    文本指针
@param wstr_len             [in]    文本长度
@param is_need_update       [in]    是否需要刷新
@return  设置成功返回TRUE，失败返回FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetTextByPtr(
                                     CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr,
                                     const wchar        *wstr_ptr, //[IN] string pointer
                                     uint16	            wstr_len, //[IN] string length, <=16
                                     BOOLEAN            is_need_update //[IN] is flash
                                     );


/***************************************************************************//*!
@brief 设置headeranddivider右下角列表索引信息
@author Liqing.peng
@param headeranddivider_ptr            [in]    控件指针
@param text_rect_ptr        [in]    子字符串的区域
@param font_ptr             [in]    子字符串的字体
@param align_ptr            [in]    对齐方式
@return  设置成功返回TRUE，失败返回FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetSubTextParamByPtr(    
                                             CTRLHEADERANDDIVIDER_OBJ_T  *headeranddivider_ptr,       // 控件指针
                                             GUI_RECT_T       *text_rect_ptr,   // 子字符串的区域
                                             GUI_FONT_T       *font_ptr,        // 子字符串字体
                                             GUI_ALIGN_E      *align_ptr        // align type
                                             );

/***************************************************************************//*!
@brief 设置headeranddivider颜色
@author Liqing.peng
@param headeranddivider_ptr            [in]    控件指针
@param font_color           [in]    Theme color
@return  设置成功返回TRUE，失败返回FALSE
@note 
*******************************************************************************/
PUBLIC void CTRLHEADERANDDIVIDER_SetThemeColorByPtr(
                                       CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr,
                                       GUI_COLOR_T       theme_color
                                       );

/***************************************************************************//*!
@brief 设置headeranddivider区域
@author Liqing.peng
@param headeranddivider_ptr            [in]    控件指针
@param rect                 [in]    控件区域
@return  设置成功返回TRUE，失败返回FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetRectByPtr(    
                                     CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr,
                                     GUI_RECT_T	    rect
                                     );

/*****************************************************************************/
// 	Description : Set headeranddivider Visible
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetIsVisible(    
                                     CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr,
                                     BOOLEAN is_visible,
                                     BOOLEAN is_need_update
                                     );

/*****************************************************************************/
// Description : get headeranddivider control ptr.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC CTRLHEADERANDDIVIDER_OBJ_T *CTRLHEADERANDDIVIDER_GetheaderanddividerPtr(
                                             MMI_HANDLE_T  win_handle,
                                             MMI_CTRL_ID_T ctrl_id
                                             );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
