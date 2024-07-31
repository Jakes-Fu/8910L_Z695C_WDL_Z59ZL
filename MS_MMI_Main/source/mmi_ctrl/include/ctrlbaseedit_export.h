/*****************************************************************************
** File Name:      ctrlbaseedit_export.h                                        *
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

#ifndef _CTRLBASEEDIT_EXPORT_H_
#define _CTRLBASEEDIT_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_app.h"
#include "guiim.h"
#include "ctrlbase_export.h"
#include "mmitheme_edit.h"

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

/*! @enum GUIEDIT_TYPE_E
@brief �༭������
*/
typedef enum
{
    GUIEDIT_TYPE_NULL,      /*!<����λ*/
    GUIEDIT_TYPE_TEXT,      /*!<�ı��༭��*/
    GUIEDIT_TYPE_PHONENUM,  /*!<�绰����༭��,[0-9,*,#]*/
    GUIEDIT_TYPE_DIGITAL,   /*!<���ֱ༭��,[0-9]*/
    GUIEDIT_TYPE_PASSWORD,  /*!<����༭��,[0-9,*,#]*/
    GUIEDIT_TYPE_LIST,      /*!<�б�༭��list[�绰�����б�,E-mail�б�]*/
    GUIEDIT_TYPE_DATE,      /*!<���ڱ༭��*/
    GUIEDIT_TYPE_TIME,      /*!<ʱ��༭��*/
    GUIEDIT_TYPE_IP,        /*!<IP�༭��*/
    GUIEDIT_TYPE_TOUCH_DATE,/*!<ȫ����������*/
    GUIEDIT_TYPE_TOUCH_TIME,/*!<ȫ������ʱ��*/
    GUIEDIT_TYPE_PICKER,    /*!<����ѡȡ*/
    GUIEDIT_TYPE_MAX        /*!<����λ*/
} GUIEDIT_TYPE_E;

/*! @enum GUIEDIT_STYLE_E
@brief �༭����
*/
typedef enum
{
    GUIEDIT_STYLE_NULL,     /*!<����λ*/
    GUIEDIT_STYLE_SINGLE,   /*!<����,û�й�����*/
    GUIEDIT_STYLE_MULTI,    /*!<����,�й�����*/
    GUIEDIT_STYLE_ADAPTIVE, /*!<����,û�й�����,��Ƕ����Ͽؼ���*/
    GUIEDIT_STYLE_SINGLE_DYN_DOWN, // multi at active and single at non-active
    GUIEDIT_STYLE_MULTI_DYN_DOWN, // [MS] ����
    GUIEDIT_STYLE_MULTI_DYN_UP, // [MS] 
    GUIEDIT_STYLE_SINGLE_UP,// Single line with themeline, im icon
    GUIEDIT_STYLE_MULTI_UP,
    GUIEDIT_STYLE_MAX       /*!<����λ*/
} GUIEDIT_STYLE_E;

/*! @enum GUIEDIT_DISPLAY_DIR_E
@brief �༭����ʾ����
*/
typedef enum
{
    GUIEDIT_DISPLAY_DIR_LT, /*!<�����Ͽ�ʼ��ʾ*/
    GUIEDIT_DISPLAY_DIR_RB, /*!<�����¿�ʼ��ʾ*/
    GUIEDIT_DISPLAY_DIR_MAX /*!<����λ*/
} GUIEDIT_DISPLAY_DIR_E;

/*! @enum GUIEDIT_SEPARATOR_E
@brief �༭��ָ�������
*/
typedef enum
{
    GUIEDIT_SEPARATOR_NULL,         /*!<����λ*/
    GUIEDIT_SEPARATOR_DOT,          /*!<'.'*/
    GUIEDIT_SEPARATOR_LINE,         /*!<'-'*/
    GUIEDIT_SEPARATOR_COMMA,        /*!<','*/
    GUIEDIT_SEPARATOR_BLANK,        /*!<' '*/
    GUIEDIT_SEPARATOR_COLON,        /*!<':'*/
    GUIEDIT_SEPARATOR_SOLIDUS,      /*!<'/'*/
    GUIEDIT_SEPARATOR_SEMICOLON,    /*!<';'*/
    GUIEDIT_SEPARATOR_MAX           /*!<����λ*/
} GUIEDIT_SEPARATOR_E;

/*! @struct GUIEDIT_INIT_LIST_T
@brief �б�༭���ʼ������
*/
typedef struct
{
    uint16      item_num;       /*!<item����*/
    uint16      item_max_len;   /*!<ÿ��item������ַ�����*/
} GUIEDIT_INIT_LIST_T;

/*! @union GUIEDIT_INIT_ADD_U
@brief �༭���ʼ����������
*/
typedef union
{
    GUIEDIT_INIT_LIST_T     list;   /*!<�б��ʼ������*/
} GUIEDIT_INIT_ADD_U;

/*! @struct GUIEDIT_INIT_DATA_T
@brief �༭���ʼ������
*/
typedef struct
{
    BOOLEAN                 is_rect_adaptive;   /*!<�����Ƿ�����ַ����Զ�����*/
    BOOLEAN is_inline_editor;
    uint16                  str_max_len;        /*!<���������ַ�����*/
    GUI_BOTH_RECT_T         both_rect;          /*!<��������*/
    GUIEDIT_TYPE_E          type;               /*!<�༭������*/
    GUIEDIT_INIT_ADD_U      add_data;           /*!<��ʼ����������*/
} GUIEDIT_INIT_DATA_T;

/*! @struct GUIEDIT_HIGHLIGHT_T
@brief �༭�������Ϣ
*/
typedef struct
{
    BOOLEAN     is_underline;   /*!<TRUE:im�ύ��ȷ���ַ�,FALSE:����/ճ����ȷ���ַ�*/
    uint16      start_pos;      /*!<������ʼλ��*/
    uint16      end_pos;        /*!<��������λ��*/
    GUI_RECT_T  rect;
} GUIEDIT_HIGHLIGHT_T;

// for [MS]
typedef enum
{
    	GUILABEL_DATA_TEXT_ID,     /*!< ��ʾ�ı� */ 
	GUILABEL_DATA_TEXT_BUFFER  /*!< ��ʾ�ı� */  
}GUIEDITLABEL_TEXT_DATA_E;

typedef enum
{
    	GUIHINTTEXT_DATA_TEXT_ID,     /*!< ��ʾ�ı� */ 
	GUIHINTTEXT_DATA_TEXT_BUFFER  /*!< ��ʾ�ı� */  
}GUIEDITHINTTEXT_TEXT_DATA_E;
// end [MS]

typedef enum
{
GUIEDIT_MMS_RESERVE,
GUIEDIT_MMS_SEPERATE_LINE

}GUIEDIT_SEPERATE_LINE_T;


/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get edit type
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUIEDIT_TYPE_E CTRLBASEEDIT_GetType (
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : set edit is display im icon and number info
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:text and list edit display,others not display
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetDispImIcon (
    MMI_CTRL_ID_T     ctrl_id,                //in:
    BOOLEAN           is_disp_im_icon        //in:
);

/*****************************************************************************/
// 	Description : set edit is permit display border
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_PermitBorder (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_permit
);

/*****************************************************************************/
//  Description : set edit bg is transparent display image
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_SetBgTransparent (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_transparent
);

/*****************************************************************************/
//  Description : set edit margin
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetMargin (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    uint16            margin_lr,  //in:left and right margin
    uint16            margin_tb   //in:top and bottom margin
);

/*****************************************************************************/
//  Description : set edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetFont (
    MMI_CTRL_ID_T       ctrl_id,
    GUI_FONT_T          *font_size_ptr,
    GUI_COLOR_T         *font_color_ptr
);


/*****************************************************************************/
//  Description : Get edit Bg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_GetBg (
    MMI_CTRL_ID_T       ctrl_id,
    GUI_BG_T            *bg_ptr
);

/*****************************************************************************/
//  Description : Get edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_GetFont (
    MMI_CTRL_ID_T       ctrl_id,
    GUI_FONT_ALL_T         *font_all
);



/*****************************************************************************/
//  Description : Get edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_GetDisplayRect (
    MMI_CTRL_ID_T       ctrl_id,
    GUI_RECT_T        *rect_ptr
);

/*****************************************************************************/
//  Description : Get edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_GetRect (
    MMI_CTRL_ID_T       ctrl_id,
    GUI_RECT_T*         rect_ptr
);

/*****************************************************************************/
//  Description : Get edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLBASEEDIT_GetIMHeight(
    MMI_CTRL_ID_T       ctrl_id
);

/*****************************************************************************/
//  Description : set edit align
//  Global resource dependence :
//  Author: Jassmine
//  Note: only for single line
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetAlign (
    MMI_CTRL_ID_T  ctrl_id,//in
    GUI_ALIGN_E    align   //in
);

/*****************************************************************************/
//  Description : set edit rect is adaptive
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetRectAdaptive(
                                         MMI_CTRL_ID_T  ctrl_id,//in
                                         BOOLEAN        is_true //in:
                                         );

/*****************************************************************************/
//  Description : set edit im
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:�������е����뷨,��ʼ���뷨����������
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetIm (
    MMI_CTRL_ID_T     ctrl_id,    //in
    GUIIM_TYPE_T      allow_im,   //in:��������뼯��
    GUIIM_TYPE_T      init_im     //in:��ʼ���뷨
);

/*****************************************************************************/
//  Description : set edit im capital mode
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetImCapital (
    MMI_CTRL_ID_T			ctrl_id,	//in
    GUIIM_CAPITAL_MODE_T	capital_mode//in:��Сдģʽ
);

///*****************************************************************************/
////  Description : set edit im input type
////  Global resource dependence :
////  Author: Jassmine
////  Note:
///*****************************************************************************/
//PUBLIC void CTRLBASEEDIT_SetImInput (
//    MMI_CTRL_ID_T		ctrl_id,		//in
//    GUIIM_INPUT_TYPE_T	init_input_type,//in:��ʼ���뷨����
//    GUIIM_INPUT_TYPE_T	allow_input_type//in:������������ͼ���
//);

/*****************************************************************************/
//  Description : set edit im is enable tp
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:�Ƿ�����TP IM
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetImTp (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_enable
);

/*****************************************************************************/
//  Description : set edit im is enable hand writing
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:�Ƿ�����IM hand writing
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetImHw (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_enable
);

/*****************************************************************************/
//  Description : set edit im is forbid symbo;
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetImSymbol (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_forbid
);
/*****************************************************************************/
//  Description : set edit im is forbid symbo;
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_GetImSymbol (
    MMI_CTRL_ID_T   ctrl_id);
/*****************************************************************************/
//  Description : set edit im tag
//  Global resource dependence :
//  Author: Jassmine
//  Note: ������email�����ʱ��һЩ���뷨�ᶨ��һЩ����
//      �����ڼ���edit�ؼ�֮ǰ���ã���Ϊ�����뷨����֮�����ã����ؽ����뷨
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetImTag (
    MMI_CTRL_ID_T  ctrl_id,
    GUIIM_TAG_T    tag
);

/*****************************************************************************/
//  Description : config edit im custom key
//  Global resource dependence :
//  Author: Jassmine
//  Note: must < 9 ,only config digital or multi-tap im mode
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_ConfigImCustomKey (
    MMI_CTRL_ID_T         ctrl_id,    //in:
    GUIIM_TYPE_T          im_type,    //in:only digital or multi-tap
    GUIIM_CUSTOM_KEY_T    custom_key, //in:
    wchar                 *value_ptr, //in:
    uint32                value_count //in:
);

/*****************************************************************************/
//  Description: check whether im is open
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_IsImOpen (
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description: judge whether application can set softkey or not
//  Author: arvin.zhang
//  Note:app cann't modify softkey if im is working on several state
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_IsImNeedSoftkey (
	MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : when edit get active, is create im
//  Global resource dependence :
//  Author: Jassmine
//  Note: default TRUE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_CreateImEditActive (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    BOOLEAN           is_create   //in:
);

/*****************************************************************************/
//  Description: judge whether the TP panel is visible
//  Author: wancan.you
//  Param
//       ctrl_id :control id
//  Return:
//       TRUE:visible;FALSE:invisible
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_IsTPPanelVisible (
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : get im handle
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLBASEEDIT_GetIMHandle(
                                        MMI_CTRL_ID_T     ctrl_id    //in:
                                        );

//for [MS]
/*****************************************************************************/
// Description : set the password display or not
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_SetDispPassword(
                                  MMI_HANDLE_T        win_handle,
                                  MMI_CTRL_ID_T        ctrl_id,
                                  BOOLEAN            is_disp,
                                  BOOLEAN			is_need_update
                                  );
/*****************************************************************************/
// Description : set the window baseedit label string, by text id
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_SetLabelId(
                                  MMI_HANDLE_T        win_handle,
                                  MMI_CTRL_ID_T        ctrl_id,
                                  MMI_TEXT_ID_T        text_id, //[IN] the string id
                                  BOOLEAN            is_need_update //[IN] is flash
                                  );
/*****************************************************************************/
// Description : set the window baseedit hint text string, by text id
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_SetHintTextId(
                                  MMI_HANDLE_T        win_handle,
                                  MMI_CTRL_ID_T        ctrl_id,
                                  MMI_TEXT_ID_T        text_id, //[IN] the string id
                                  BOOLEAN            is_need_update //[IN] is flash
                                  );
/*****************************************************************************/
// Description : set the window baseedit label string
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_SetLabel(
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                const wchar*    wstr_ptr, //[IN] string pointer
                                uint16          wstr_len, //[IN] string length, <=16
                                BOOLEAN         is_need_update //[IN] is flash
                                );
                                  
/*****************************************************************************/
// Description : set the window baseedit hint text string
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_SetHintText(
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                const wchar*    wstr_ptr, //[IN] string pointer
                                uint16          wstr_len, //[IN] string length, <=16
                                BOOLEAN         is_need_update //[IN] is flash
                                );                                  
//end [MS]

// Added by sunhongzhe for Date and Time edit control to set softkey params.
/*****************************************************************************/
//  Description : set edit softkey,left,middle,right button
//  Global resource dependence :
//  Author: hongzhe.sun
//  Note:
//      If id equals 0, do nothing.
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetSoftkey (
    MMI_HANDLE_T        win_handle,
    MMI_CTRL_ID_T                    ctrl_id,    //in:
    uint16                           index,      //in:softkey index
    uint16                           str_len,    //in:0,����
    MMI_TEXT_ID_T                    text_1,     //in:<str_len display text_1
    MMI_TEXT_ID_T                    text_2,     //in:>=str_len display text_2
    MMITHEME_EDIT_SET_BUTTON_PFUNC   setsoftkey  //in:����ͨ����������softkey[in:�ַ���ָ��in:�ַ�������,out:softkeyֵ]
);

/*****************************************************************************/
//  Description : set edit softkey,left,middle,right button
//  Global resource dependence : 
//  Author: hongzhe.sun
//  Note: Extend GUIEDIT_SetSoftkey, support icon.
//
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetSoftkeyEx(
                               MMI_HANDLE_T        win_handle,
                               MMI_CTRL_ID_T                    ctrl_id,    //in:
                               uint16                           index,      //in:softkey index
                               uint16                           str_len,    //in:0,����
                               MMI_TEXT_ID_T                    text_1,     //in:<str_len display text_1
                               MMI_TEXT_ID_T                    text_2,     //in:>=str_len display text_2
                               MMI_IMAGE_ID_T                   image_id,
                               MMI_IMAGE_ID_T                   image_id_2, // MS
                               BOOLEAN                        show_icon,
                               MMITHEME_EDIT_SET_BUTTON_PFUNC   setsoftkey  //in:����ͨ����������softkey[in:�ַ���ָ��in:�ַ�������,out:softkeyֵ]
                               );

PUBLIC BOOLEAN CTRLBASEEDIT_SetDeActiveBorderColor(
                                  MMI_HANDLE_T        win_handle,
                                  MMI_CTRL_ID_T        ctrl_id,
                                  CAF_COLOR_T	    borderColor
                                  );


// End

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
