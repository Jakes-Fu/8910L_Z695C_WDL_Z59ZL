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
@brief title�ı���󳤶�
*/ 
#define GUIHEADERANDDIVIDER_TEXT_MAXLEN 	    60

/*! @def GUIHEADERANDDIVIDER_SUBTEXT_MAXLEN
@brief ������Ϣ��󳤶�
*/
#define GUIHEADERANDDIVIDER_SUBTEXT_MAXLEN     15

/*! @typedef GUIHEADERANDDIVIDER_BUTTON_INDEX_T
@brief title�ļ�ͷ��Ϣ
*/
typedef uint8 GUIHEADERANDDIVIDER_BUTTON_INDEX_T;

/*! @def GUIHEADERANDDIVIDER_BUTTON_LEFT
@brief title���Button
*/
#define GUIHEADERANDDIVIDER_BUTTON_LEFT         0

/*! @def GUIHEADERANDDIVIDER_BUTTON_RIGHT
@brief title�ұ�Button
*/
#define GUIHEADERANDDIVIDER_BUTTON_RIGHT        1

/*! @def GUIHEADERANDDIVIDER_BUTTON_NUM
@brief Button����
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
    	HEADERANDDIVIDER_NOSCROLL,                 /*!< �� */
	HEADERANDDIVIDER_SCROLL_LEFT_OUT,          /*!< �����������*/  
	HEADERANDDIVIDER_NOSCROLL_BY_ELLIPSIS      /*!< ��ѭ�������򲻹�����ʾʡ�Ե� */   
}GUIHEADERANDDIVIDER_SCROLL_TYPE_E;

/*! @typedef GUIHEADERANDDIVIDER_BUTTON_STATE_E
@brief title��ͷ��ʾģʽ
*/
typedef enum
{
    	HEADERANDDIVIDER_BUTTON_NORMAL,     /*!< ����ģʽ */
    	HEADERANDDIVIDER_BUTTON_INVISIBLE,  /*!< ���ɼ�ģʽ */
	HEADERANDDIVIDER_BUTTON_GRAYED,     /*!< �һ�ģʽ */
	HEADERANDDIVIDER_BUTTON_STATE_MAX   /*!< ϵͳ���� */
}GUIHEADERANDDIVIDER_BUTTON_STATE_E;

typedef enum
{
    	GUIHEADERANDDIVIDER_DATA_TEXT_ID,     /*!< ��ʾ�ı� */ 
	GUIHEADERANDDIVIDER_DATA_TEXT_BUFFER  /*!< ��ʾ�ı� */  
}GUIHEADERANDDIVIDER_TEXT_DATA_E;

typedef struct  
{
    MMI_IMAGE_ID_T       image_id;       /*!< image_id */ 
    MMI_IMAGE_ID_T       press_image_id; /*!< press_image_id */ 
    MMI_IMAGE_ID_T       gray_image_id;  /*!< gray_image_id */ 

    GUIHEADERANDDIVIDER_TEXT_DATA_E text_type;      /*!< �ı����� */ 
    MMI_TEXT_ID_T        text_id;        /*!< �ı�ID */ 
    wchar                *icon_str_ptr;  /*!< �ı�ָ��*/
    uint16               str_len;        /*!< �ı����� */ 
}GUIHEADERANDDIVIDER_BUTTON_INFO_T;


/*! @struct GUIHEADERANDDIVIDER_INIT_DATA_T
@brief title�ؼ���ʼ����Ϣ
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
// Description : ���sub text����ʾ��Ϣ
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_ClearSubTextInfo(    
                                         MMI_HANDLE_T   win_handle,
                                         MMI_CTRL_ID_T    ctrl_id
                                         );

/*****************************************************************************/
// Description : ��̬����״̬���ؼ�
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
// Description : ���HEADERANDDIVIDER��button��״̬
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
// Description : ���title��arrow״̬
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
// Description : ����title���Ƿ���arrow
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetRollArrow(    
                                     MMI_HANDLE_T   win_handle,
                                     MMI_CTRL_ID_T    ctrl_id,
                                     BOOLEAN        is_need_button  //�Ƿ���Ҫtitle�ϵ�arrowͼ��
                                     );

/*****************************************************************************/
// Description : ����HEADERANDDIVIDER���Ƿ���arrow
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
// Description : ����HEADERANDDIVIDER���Ƿ���button
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetNeedButton(    
                                      MMI_HANDLE_T   win_handle,
                                      MMI_CTRL_ID_T    ctrl_id,
                                      BOOLEAN        is_need_button  //�Ƿ���Ҫtitle�ϵ�arrowͼ��
                                      );

/*****************************************************************************/
// Description : ����HEADERANDDIVIDER��button��״̬
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
// Description : ����HEADERANDDIVIDER���½��б�������Ϣ
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetSubText(    
                                   MMI_HANDLE_T     win_handle,
                                   MMI_CTRL_ID_T    ctrl_id,
                                   BOOLEAN          is_need_sub_text,   //�Ƿ���Ҫ�б�������Ϣ
                                   const wchar      *wstr_ptr,        //[IN] string pointer
                                   uint16            wstr_len,        //[IN] string length
                                   BOOLEAN          is_need_update        //�Ƿ�������ʾ
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
// Description : ����HEADERANDDIVIDER��ߵ�Сͼ��
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
// Description : ����HEADERANDDIVIDER����
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
// Description : ����HEADERANDDIVIDER���½��б�������Ϣ
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetSubTextParam(
                                        MMI_HANDLE_T    win_handle,
                                        MMI_CTRL_ID_T   ctrl_id,
                                        GUI_RECT_T      *text_rect_ptr,   // ���ַ���������
                                        GUI_FONT_T      *font_ptr,        // ���ַ�������
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
