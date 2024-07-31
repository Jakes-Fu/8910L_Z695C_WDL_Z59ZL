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
    
    //�����ı�
    GUIHEADERANDDIVIDER_TEXT_DATA_E        	text_type;
    MMI_TEXT_ID_T               	text_id;
    MMI_STRING_T                	headeranddivider_str;          /*!< headeranddivider string */ 
    GUISTR_STYLE_T              	str_style;          /*!< �ı���� */ 

    uint16                      			headeranddivider_text_margin;  /*!< ��Ե */ 
    uint16                      			icon_space;         /*!< icon��� */ 

    //theme
    BOOLEAN                     		is_need_bg;         /*!< �Ƿ��б��� */ 
    MMI_IMAGE_ID_T              	bg_image;           /*!< ����ͼƬ */ 
    GUI_COLOR_T                 	bg_color;           /*!< ������ɫ */ 
    GUI_COLOR_T                 	line_color;           
    GUI_RECT_T				line_rect;
	
    //���ı�����
    GUIHEADERANDDIVIDER_SCROLL_TYPE_E      scroll_type;        /*!< ������ģʽ */ 
    BOOLEAN                     is_text_overstep;   /*!< ���ı��Ŀ�ȳ��������ʱ�򣬸�ֵΪTRUE */ 
    uint8                       scroll_timer_id;    /*!< ������timer_id */ 
    uint32                      scroll_time;        /*!< ѭ������ʱ�� */ 
    uint32                      scroll_pixel_step;  /*!< ÿ�ι����������� */ 
    uint16                      scroll_char_offset; /*!< �ַ�offset */  //record the roll num of chars (roll one char each time  
    uint16                      scroll_pixel_offset;/*!< ����offset */  //headeranddivider string pixel offset

    GUI_LCD_DEV_INFO            divider_layer_info; //label layer
	
    MMI_HANDLE_T                win_handle;         /*!< ����handle */ 

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
@brief ����headeranddivider�ı�
@author Liqing.peng
@param headeranddivider_ptr            [in]    headeranddividerָ��
@param wstr_ptr             [in]    �ı�ָ��
@param wstr_len             [in]    �ı�����
@param is_need_update       [in]    �Ƿ���Ҫˢ��
@return  ���óɹ�����TRUE��ʧ�ܷ���FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetTextByPtr(
                                     CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr,
                                     const wchar        *wstr_ptr, //[IN] string pointer
                                     uint16	            wstr_len, //[IN] string length, <=16
                                     BOOLEAN            is_need_update //[IN] is flash
                                     );


/***************************************************************************//*!
@brief ����headeranddivider���½��б�������Ϣ
@author Liqing.peng
@param headeranddivider_ptr            [in]    �ؼ�ָ��
@param text_rect_ptr        [in]    ���ַ���������
@param font_ptr             [in]    ���ַ���������
@param align_ptr            [in]    ���뷽ʽ
@return  ���óɹ�����TRUE��ʧ�ܷ���FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetSubTextParamByPtr(    
                                             CTRLHEADERANDDIVIDER_OBJ_T  *headeranddivider_ptr,       // �ؼ�ָ��
                                             GUI_RECT_T       *text_rect_ptr,   // ���ַ���������
                                             GUI_FONT_T       *font_ptr,        // ���ַ�������
                                             GUI_ALIGN_E      *align_ptr        // align type
                                             );

/***************************************************************************//*!
@brief ����headeranddivider��ɫ
@author Liqing.peng
@param headeranddivider_ptr            [in]    �ؼ�ָ��
@param font_color           [in]    Theme color
@return  ���óɹ�����TRUE��ʧ�ܷ���FALSE
@note 
*******************************************************************************/
PUBLIC void CTRLHEADERANDDIVIDER_SetThemeColorByPtr(
                                       CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr,
                                       GUI_COLOR_T       theme_color
                                       );

/***************************************************************************//*!
@brief ����headeranddivider����
@author Liqing.peng
@param headeranddivider_ptr            [in]    �ؼ�ָ��
@param rect                 [in]    �ؼ�����
@return  ���óɹ�����TRUE��ʧ�ܷ���FALSE
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
