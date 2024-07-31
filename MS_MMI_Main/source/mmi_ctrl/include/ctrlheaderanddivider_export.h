/*****************************************************************************
** File Name:      ctrlheaderanddivider_export.h                                        *
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

#ifndef _CTRLHEADERANDDIVIDER_EXPORT_H_
#define _CTRLHEADERANDDIVIDER_EXPORT_H_ 

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
/*! @def GUIHEADERANDDIVIDER_TEXT_MAXLEN
@brief title文本最大长度
*/ 
#define GUIHEADERANDDIVIDER_TEXT_MAXLEN 	    60

/*! @def GUIHEADERANDDIVIDER_SUBTEXT_MAXLEN
@brief 索引信息最大长度
*/
#define GUIHEADERANDDIVIDER_SUBTEXT_MAXLEN     15

/*! @typedef GUIHEADERANDDIVIDER_BUTTON_INDEX_T
@brief title的箭头信息
*/
typedef uint8 GUIHEADERANDDIVIDER_BUTTON_INDEX_T;

/*! @def GUIHEADERANDDIVIDER_BUTTON_LEFT
@brief title左边Button
*/
#define GUIHEADERANDDIVIDER_BUTTON_LEFT         0

/*! @def GUIHEADERANDDIVIDER_BUTTON_RIGHT
@brief title右边Button
*/
#define GUIHEADERANDDIVIDER_BUTTON_RIGHT        1

/*! @def GUIHEADERANDDIVIDER_BUTTON_NUM
@brief Button数量
*/
#define GUIHEADERANDDIVIDER_BUTTON_NUM          2

        
/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*!  @typedef GUIHEADERANDDIVIDER_SCROLL_TYPE_E
@brief title scroll
*/
typedef enum
{
    	HEADERANDDIVIDER_NOSCROLL,                 /*!< 无 */
	HEADERANDDIVIDER_SCROLL_LEFT_OUT,          /*!< 从右向左滚动*/  
	HEADERANDDIVIDER_NOSCROLL_BY_ELLIPSIS      /*!< 不循环，区域不够则显示省略点 */   
}GUIHEADERANDDIVIDER_SCROLL_TYPE_E;

/*! @typedef GUIHEADERANDDIVIDER_BUTTON_STATE_E
@brief title箭头显示模式
*/
typedef enum
{
    	HEADERANDDIVIDER_BUTTON_NORMAL,     /*!< 正常模式 */
    	HEADERANDDIVIDER_BUTTON_INVISIBLE,  /*!< 不可见模式 */
	HEADERANDDIVIDER_BUTTON_GRAYED,     /*!< 灰化模式 */
	HEADERANDDIVIDER_BUTTON_STATE_MAX   /*!< 系统保留 */
}GUIHEADERANDDIVIDER_BUTTON_STATE_E;

typedef enum
{
    	GUIHEADERANDDIVIDER_DATA_TEXT_ID,     /*!< 显示文本 */ 
	GUIHEADERANDDIVIDER_DATA_TEXT_BUFFER  /*!< 显示文本 */  
}GUIHEADERANDDIVIDER_TEXT_DATA_E;

typedef struct  
{
    MMI_IMAGE_ID_T       image_id;       /*!< image_id */ 
    MMI_IMAGE_ID_T       press_image_id; /*!< press_image_id */ 
    MMI_IMAGE_ID_T       gray_image_id;  /*!< gray_image_id */ 

    GUIHEADERANDDIVIDER_TEXT_DATA_E text_type;      /*!< 文本类型 */ 
    MMI_TEXT_ID_T        text_id;        /*!< 文本ID */ 
    wchar                *icon_str_ptr;  /*!< 文本指针*/
    uint16               str_len;        /*!< 文本长度 */ 
}GUIHEADERANDDIVIDER_BUTTON_INFO_T;


/*! @struct GUIHEADERANDDIVIDER_INIT_DATA_T
@brief title控件初始化信息
*/
typedef struct
{
    	MMI_IMAGE_ID_T       	icon_id;       /*!< image_id */ 
  	MMI_TEXT_ID_T		text_id;    /*!< text_id */ 
	MMI_IMAGE_ID_T       	line_image_id;       /*!< image_id */ 

} GUIHEADERANDDIVIDER_INIT_DATA_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : 清除sub text的显示信息
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_ClearSubTextInfo(    
                                         MMI_HANDLE_T   win_handle,
                                         MMI_CTRL_ID_T    ctrl_id
                                         );

/*****************************************************************************/
// Description : 动态创建状态栏控件
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_CreateCtrlDyn(
                                      MMI_HANDLE_T win_handle,
                                      MMI_CTRL_ID_T ctrl_id, 
                                      MMI_TEXT_ID_T text_id
                                      );

/*****************************************************************************/
// Description : 获得HEADERANDDIVIDER上button的状态
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC GUIHEADERANDDIVIDER_BUTTON_STATE_E CTRLHEADERANDDIVIDER_GetButtonStatus(    
                                                        MMI_HANDLE_T   win_handle,
                                                        MMI_CTRL_ID_T    ctrl_id,
                                                        GUIHEADERANDDIVIDER_BUTTON_INDEX_T   index
                                                        );

/*****************************************************************************/
// Description : 获得title上arrow状态
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC GUIHEADERANDDIVIDER_BUTTON_STATE_E CTRLHEADERANDDIVIDER_GetRollArrowStatus(    
                                                          MMI_HANDLE_T   win_handle,
                                                          MMI_CTRL_ID_T    ctrl_id,
                                                          GUIHEADERANDDIVIDER_BUTTON_INDEX_T   index
                                                          );

/*****************************************************************************/
// Description : set the HEADERANDDIVIDER background
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetBackground(
                                      MMI_HANDLE_T   win_handle,
                                      MMI_CTRL_ID_T     ctrl_id,
                                      MMI_IMAGE_ID_T bg_image,
                                      GUI_COLOR_T    bg_color
                                      );

/*****************************************************************************/
// Description : set HEADERANDDIVIDER font color
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC void CTRLHEADERANDDIVIDER_SetFontColor(
                                  MMI_HANDLE_T      win_handle,
                                  MMI_CTRL_ID_T        ctrl_id,
                                  GUI_COLOR_T       font_color
                                  );

/*****************************************************************************/
// Description : 设置title上是否有arrow
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetRollArrow(    
                                     MMI_HANDLE_T   win_handle,
                                     MMI_CTRL_ID_T    ctrl_id,
                                     BOOLEAN        is_need_button  //是否需要title上的arrow图标
                                     );

/*****************************************************************************/
// Description : 设置HEADERANDDIVIDER上是否有arrow
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetRollArrowStatus(    
                                           MMI_HANDLE_T   win_handle,
                                           MMI_CTRL_ID_T    ctrl_id,
                                           GUIHEADERANDDIVIDER_BUTTON_INDEX_T   index,
                                           GUIHEADERANDDIVIDER_BUTTON_STATE_E  status
                                           );

/*****************************************************************************/
// Description : 设置HEADERANDDIVIDER上是否有button
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetNeedButton(    
                                      MMI_HANDLE_T   win_handle,
                                      MMI_CTRL_ID_T    ctrl_id,
                                      BOOLEAN        is_need_button  //是否需要title上的arrow图标
                                      );

/*****************************************************************************/
// Description : 设置HEADERANDDIVIDER上button的状态
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetButtonStatus(    
                                        MMI_HANDLE_T   win_handle,
                                        MMI_CTRL_ID_T    ctrl_id,
                                        GUIHEADERANDDIVIDER_BUTTON_INDEX_T   index,
                                        GUIHEADERANDDIVIDER_BUTTON_STATE_E  status
                                        );

/*****************************************************************************/
// Description : 设置HEADERANDDIVIDER右下角列表索引信息
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetSubText(    
                                   MMI_HANDLE_T     win_handle,
                                   MMI_CTRL_ID_T    ctrl_id,
                                   BOOLEAN          is_need_sub_text,   //是否需要列表索引信息
                                   const wchar      *wstr_ptr,        //[IN] string pointer
                                   uint16            wstr_len,        //[IN] string length
                                   BOOLEAN          is_need_update        //是否立即显示
                                   );

/*****************************************************************************/
// Description : update the window HEADERANDDIVIDER
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC void CTRLHEADERANDDIVIDER_Update(
                            MMI_HANDLE_T    win_handle,
                            MMI_CTRL_ID_T    ctrl_id
                            );

/*****************************************************************************/
// Description : 设置HEADERANDDIVIDER左边的小图标
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
/*PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetIcon(    
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                MMI_IMAGE_ID_T  icon_id
                                );*/

/*****************************************************************************/
// Description : 设置HEADERANDDIVIDER区域
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetRect(    
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                GUI_RECT_T        rect
                                );

/*****************************************************************************/
// Description : 设置HEADERANDDIVIDER右下角列表索引信息
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetSubTextParam(
                                        MMI_HANDLE_T    win_handle,
                                        MMI_CTRL_ID_T   ctrl_id,
                                        GUI_RECT_T      *text_rect_ptr,   // 子字符串的区域
                                        GUI_FONT_T      *font_ptr,        // 子字符串字体
                                        GUI_ALIGN_E     *align_ptr        // align type
                                        );
                                        
/*****************************************************************************/
// Description : set the window HEADERANDDIVIDER string, by text id
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetTextId(
                                  MMI_HANDLE_T        win_handle,
                                  MMI_CTRL_ID_T        ctrl_id,
                                  MMI_TEXT_ID_T        text_id, //[IN] the string id
                                  BOOLEAN            is_need_update //[IN] is flash
                                  );

/*****************************************************************************/
// Description : set the window HEADERANDDIVIDER align type
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetTextAlignType(
                                         MMI_HANDLE_T                win_handle,
                                         MMI_CTRL_ID_T            ctrl_id,
                                         GUI_ALIGN_E                align
                                         );

/*****************************************************************************/
// Description : set the window HEADERANDDIVIDER scroll type
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetTextScrollType(
                                          MMI_HANDLE_T                win_handle,
                                          MMI_CTRL_ID_T            ctrl_id,
                                          GUIHEADERANDDIVIDER_SCROLL_TYPE_E    scroll_type
                                          );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
