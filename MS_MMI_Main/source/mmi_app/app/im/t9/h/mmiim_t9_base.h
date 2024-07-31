/*****************************************************************************
** File Name:      mmiim_t9_base.h                                           *
** Author:         haiwu.chen                                                *
** Date:           24/04/2012                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2012        haiwu.chen            Create                               *
******************************************************************************/
#ifndef _MMIIM_T9_BASE_H_
#define _MMIIM_T9_BASE_H_

#if defined(__cplusplus)
extern "C" {
#endif

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiim.h"
#include "guifont.h"
#include "guilcd.h"
#include "mmiim_internal.h"
#include "mmi_position.h"
#include "mmiim_sp_basic.h" 
//#include "ctrlim.h"

#ifdef IM_ENGINE_T9
#include "t9.h"
#include "t9api.h"
#endif

#ifdef IM_ENGINE_XT9
#include "et9api.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#if (defined IM_ENGINE_T9)

#if (!defined IM_SPRD_MULTITAP_SUPPORT)
//#define IM_SPRD_MULTITAP_SUPPORT  //T9 multitap暂时使用公司使用的multitap数据
#endif

#if (!defined MMIIM_T9_MULTITAP)
#define MMIIM_T9_MULTITAP
#endif

//T9 need word completion, default open, samsung not define word completion
#if (!defined IM_WORD_COMPLETION_SUPPORT)
//#define IM_WORD_COMPLETION_SUPPORT
#endif

#if ((!defined KEYPAD_TYPE_QWERTY_KEYPAD) && (defined MMI_IME_USER_DATABASE) && defined (MMI_IME_SMART_TYPE_SUPPORT))
#define MMI_IME_ADD_NEW_WORD
#endif

#define T9_MAX_ADD_WORD_LEN             (64)

/*针对text的buf size begin, 由于英文单词需要空间会大一点，这里先用最大的*/
#define T9_MAX_INPUT_BUF_SIZE           (32)


#define T9_MAX_TXT_BUF_SIZE             2048 //4*1024  //MMI中text buffer的最大值
#define T9_KEY_BUF_SIZE                 30
#define T9_SELECTION_BUFFER_SIZE        9
#define T9_INPUT_MODE_NUM               5   //T9输入法类型的总个数
#define T9_DYM_CTXT_FENCE               2
#define T9_DYM_CTXT_SEARCH_DEPTH        4
#define	IM_SELECTIONPAGE_MAX_LEN        20  //参考 GUI_SELECTIONPAGE_MAX_LEN
#define T9SPELLBUFSIZE                  128
#define T9_CAUDB_SIZE                   110

#ifdef MMI_IME_USER_DATABASE
#define T9_UDB_SIZE                     (4 * 1024) 
/* 20k bytes (T9 recommended), if we don't turn on word completion
* it should be 4k butes. It should be bigger than T9MINUDBDATABYTES */

#define T9_UDB_FILE_NAME_LEN            (20)
#endif

#ifdef MMI_IME_TUDB_SUPPORT
#define T9_EN_TUDB_SIZE                 (30 * 1024)
#define T9_ALPHA_TUDB_SIZE              (30 * 1024)
#endif

#ifdef MMI_IME_SAMSUNG_SUPPORT
#define MMIIM_TEXT_BAR_SEG_MAX          T9AWMAXSELECTIONOBJECT//2
#else
//分段表示的最大内容
#define MMIIM_TEXT_BAR_SEG_MAX          T9AWMAXSELECTIONOBJECT
#endif

//每个单词的最大长度，包括'\0'
#define MMIIM_TEXT_WORD_SIZE_MAX        (T9AWMAXWORDSIZE+1)

//可以设置得大一点，用于存放每行的buf, for chinese,if chinese neednot pls delete
#define MMIIM_TEXT_BAR_CHAR_MAX         (64)

#define MMIIM_T9_CONVERSION_WORD_SIZE_MAX 50

#ifdef T9CDB_IN_BUILD
#define T9_CDB_SIZE                     1600
#endif

#define T9_UDB_FENCE                    2
#define T9_EXPL_TIMER_OUT               750 

#define MMIIM_KEY_TYPE_MASK             (0xff00)
#define MMIIM_KEY_MASK                  (0x00ff)
#define MMIIM_KEY_MASK_NONE             (0x0000)
#define MMIIM_VKEY_NONE                 (0xFFFF0000)
#define MMIIM_VKEY_PRESSSED             (0x00100000)
#define MMIIM_VKEY_RELEASED             (0x00200000)
#define MMIIM_VKEY_LONGCLICK            (0x00300000)
#define MMIIM_VKEY_REPEAT               (0x00400000)


///////////////////////////////////////////////////////////////////
/*             与分辨率相关的宏定义                              */
///////////////////////////////////////////////////////////////////

#if (defined (MAINLCD_SIZE_128X128) || defined (MAINLCD_SIZE_128X160))
#define MMIIM_T9_CC_SELECTPAGE_NUM      (5) 
#define MMIIM_TEXT_BAR_SPELL_Y          (1)
#define MMIIM_TEXT_BAR_CAND_Y           (1)
#define MMIIM_TEXT_BAR_EDIT_HEIGHT      (20)
#define MMIIM_TEXT_BAR_CAND_HEIGHT      (20)
#define MMIIM_TEXT_BAR_SPACE            (1)
#define MMIIM_TEXT_EDIT_BAR_SPACE       (4) //edit bar 上面字符间矩
#define MMIIM_TEXT_POPUP_LAYER_WIDTH    (MMI_MAINSCREEN_WIDTH)
#define MMIIM_TEXT_POPUP_WIDTH          (40)
#define MMIIM_TEXT_POPUP_HEIGHT         (MMIIM_TEXT_BAR_EDIT_HEIGHT * MMIIM_T9_CC_SELECTPAGE_NUM)

#define MMIIM_TEXT_NEWWORD_SPACE_Y      (2) //NEW WORD两行之间间隔
#define MMIIM_TEXT_NEWWORD_BAR_HEIGHT   ((2*MMIIM_TEXT_BAR_CAND_HEIGHT)+ MMIIM_TEXT_NEWWORD_SPACE_Y)

#define MMIIM_TEXT_BAR_IDC_W            (2)
#define MMIIM_TEXT_BAR_IDC_H            (2)
#define MMIIM_TEXT_BAR_IDC_H_SPACE      (1)
#define MMIIM_TEXT_BAR_IDC_Y            (2)
#define MMIIM_TEXT_BAR_IDC_LEFT_X       (2)//空出两边的位置
#define MMIIM_TEXT_BAR_IDC_RIGHT_X      (MMI_MAINSCREEN_WIDTH-MMIIM_TEXT_BAR_IDC_LEFT_X)
#elif (defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_220X176))
#define MMIIM_T9_CC_SELECTPAGE_NUM      (7) 
#define MMIIM_TEXT_BAR_SPELL_Y          (1)
#define MMIIM_TEXT_BAR_CAND_Y           (1)
#define MMIIM_TEXT_BAR_EDIT_HEIGHT      (15)
#define MMIIM_TEXT_BAR_CAND_HEIGHT      (24)
#define MMIIM_TEXT_BAR_SPACE            (3)
#define MMIIM_TEXT_EDIT_BAR_SPACE       (2) //edit bar 上面字符间矩
#define MMIIM_TEXT_POPUP_LAYER_WIDTH    (MMI_MAINSCREEN_WIDTH)
#define MMIIM_TEXT_POPUP_WIDTH          (40)
#define MMIIM_TEXT_POPUP_HEIGHT         (MMIIM_TEXT_BAR_EDIT_HEIGHT * MMIIM_T9_CC_SELECTPAGE_NUM)

#define MMIIM_TEXT_NEWWORD_SPACE_Y      (3) //NEW WORD两行之间间隔
#define MMIIM_TEXT_NEWWORD_BAR_HEIGHT   ((2*MMIIM_TEXT_BAR_CAND_HEIGHT)+ MMIIM_TEXT_NEWWORD_SPACE_Y)

#define MMIIM_TEXT_BAR_IDC_W            (4)
#define MMIIM_TEXT_BAR_IDC_H            (4)
#define MMIIM_TEXT_BAR_IDC_H_SPACE      (2)
#define MMIIM_TEXT_BAR_IDC_Y            (3)
#define MMIIM_TEXT_BAR_IDC_LEFT_X       (3)//空出两边的位置
#define MMIIM_TEXT_BAR_IDC_RIGHT_X      (MMI_MAINSCREEN_WIDTH-MMIIM_TEXT_BAR_IDC_LEFT_X)
#elif (defined (MAINLCD_SIZE_240X320) || defined (MAINLCD_SIZE_240X400) || defined (MAINLCD_SIZE_320X240))
#define MMIIM_T9_CC_SELECTPAGE_NUM      (4)
#define MMIIM_TEXT_BAR_BORDER_WIDTH     (3)
#define MMIIM_TEXT_BAR_SPELL_Y          (1)
#define MMIIM_TEXT_BAR_CAND_Y           (1)
#define MMIIM_TEXT_BAR_EDIT_HEIGHT      (24)
#define MMIIM_TEXT_BAR_CAND_HEIGHT      (24)
#define MMIIM_TEXT_BAR_SPACE            (5)
#define MMIIM_TEXT_BAR_MARGIN           (2)
#define MMIIM_TEXT_POPUP_MARGIN         (0)
#define MMIIM_TEXT_EDIT_BAR_SPACE       (6) //edit bar 上面字符间矩
#define MMIIM_TEXT_POPUP_LAYER_WIDTH    (MMI_MAINSCREEN_WIDTH)
#define MMIIM_TEXT_POPUP_WIDTH          (60)
#define MMIIM_TEXT_POPUP_HEIGHT         (MMIIM_TEXT_BAR_EDIT_HEIGHT * MMIIM_T9_CC_SELECTPAGE_NUM)

#define MMIIM_TEXT_NEWWORD_SPACE_Y      (4) //NEW WORD两行之间间隔
#define MMIIM_TEXT_NEWWORD_BAR_HEIGHT   ((2*MMIIM_TEXT_BAR_CAND_HEIGHT)+ MMIIM_TEXT_NEWWORD_SPACE_Y)

#define MMIIM_TEXT_BAR_IDC_W            (4)
#define MMIIM_TEXT_BAR_IDC_H            (4)
#define MMIIM_TEXT_BAR_IDC_H_SPACE      (2)
#define MMIIM_TEXT_BAR_IDC_Y            (3)
#define MMIIM_TEXT_BAR_IDC_LEFT_X       (5)//空出两边的位置
#define MMIIM_TEXT_BAR_IDC_RIGHT_X      (MMI_MAINSCREEN_WIDTH-MMIIM_TEXT_BAR_IDC_LEFT_X)
#else
#define MMIIM_T9_CC_SELECTPAGE_NUM      (7) 
#define MMIIM_TEXT_BAR_SPELL_Y          (1)
#define MMIIM_TEXT_BAR_CAND_Y           (1)
#define MMIIM_TEXT_BAR_EDIT_HEIGHT      (15)
#define MMIIM_TEXT_BAR_CAND_HEIGHT      (32)
#define MMIIM_TEXT_BAR_SPACE            (5)
#define MMIIM_TEXT_EDIT_BAR_SPACE       (2) //edit bar 上面字符间矩
#define MMIIM_TEXT_POPUP_LAYER_WIDTH    (MMI_MAINSCREEN_WIDTH)
#define MMIIM_TEXT_POPUP_WIDTH          (40)
#define MMIIM_TEXT_POPUP_HEIGHT         (MMIIM_TEXT_BAR_EDIT_HEIGHT * MMIIM_T9_CC_SELECTPAGE_NUM)

#define MMIIM_TEXT_NEWWORD_SPACE_Y      (4) //NEW WORD两行之间间隔
#define MMIIM_TEXT_NEWWORD_BAR_HEIGHT   ((2*MMIIM_TEXT_BAR_CAND_HEIGHT)+ MMIIM_TEXT_NEWWORD_SPACE_Y)

#define MMIIM_TEXT_BAR_IDC_W            (4)
#define MMIIM_TEXT_BAR_IDC_H            (4)
#define MMIIM_TEXT_BAR_IDC_H_SPACE      (2)
#define MMIIM_TEXT_BAR_IDC_Y            (3)
#define MMIIM_TEXT_BAR_IDC_LEFT_X       (5)//空出两边的位置
#define MMIIM_TEXT_BAR_IDC_RIGHT_X      (MMI_MAINSCREEN_WIDTH-MMIIM_TEXT_BAR_IDC_LEFT_X)
#endif

enum
{
    MMIIM_BAR_EDIT,
    MMIIM_BAR_SPELL,
    MMIIM_BAR_CAND,
};
typedef uint8 MMIIM_BAR_TYPE_T;

typedef struct
{
    uint8 im_vkey_id;
    uint8 t9_key_id;
}MMIIM_IM_T9_KEYID_CONVERT_T;

//#ifdef T9CHINESE
typedef struct
{
    GUI_RECT_T rect;            
    MMI_WIN_ID_T win_id;
    GUI_LCD_DEV_INFO lcd_dev_info;

    uint8 width_label;
    uint8 width_space;

    int8 has_label;             // 0 - none, 1 - has
    int8 has_hot;               // 0 - none, 1 - has
    uint8 hot_index;            // [0, n] - hot index, 本页里

    uint8 seg_index;            //本页第一个是在总的第几个
    uint8 cur_page_cnt;         //count words in current page
    
    uint8 page_index;
    uint8 page_cnt;
    
    uint8 seg_cnt;              //total words count
    MMIIM_TEXT_BAR_SEG_INFO_T seg[MMIIM_TEXT_BAR_SEG_MAX];
 
    uint8 text_len;
    wchar text[MMIIM_TEXT_BAR_CHAR_MAX]; //用空格分隔的所有片段 

    GUI_RECT_T rect_idc_left;
    GUI_RECT_T rect_idc_right;

    MMIIM_IDC_STATE_T idc_left;
    MMIIM_IDC_STATE_T idc_right;

    uint8 is_display_upper_side;
} MMIIM_TEXT_BAR_T;
//#endif

typedef struct
{
    BOOLEAN is_initialized;
    BOOLEAN is_multi_line_style;
}EDITOR_STYLE_INFO;

typedef struct
{    
    uint16 len;
    uint8 *p_ldb_buff;
    
}MMIIM_T9_LDB_DATA_T;

typedef T9U16 MMIIM_T9_CONVERSION_TABLE_T[MMIIM_T9_CONVERSION_WORD_SIZE_MAX][2];
typedef MMIIM_T9_CONVERSION_TABLE_T const MMIIM_T9_CONVERSION_TABLE_DEF_T;

#endif //IM_ENGINE_T9

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

#ifdef IM_ENGINE_T9 // functions only in used in t9 below
/*****************************************************************************/
//  Description : MMIIM_ConvertMsgToT9KeyID
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_ConvertMsgToT9KeyID(MMIIM_SP_MSG_T *msg_ptr, T9KEY *eKeyID_ptr);

/*****************************************************************************/
//  Description : MMIIM_ConvertSpKeyToT9KeyID
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_ConvertSpKeyToT9KeyID(
    MMIIM_SP_VKEY_T msg_id,
    T9KEY *T9KeyID_ptr
);

#ifdef T9CHINESE
/*****************************************************************************/
//  Description : MMIIM_TextBarGenTextSeg
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: generate bar data
/*****************************************************************************/
PUBLIC int32 MMIIM_TextBarGenSeg(
    MMIIM_TEXT_BAR_T * bar_ptr,
    wchar split,
    MMIIM_BAR_TYPE_T bar_type
);

/*****************************************************************************/
//  Description : MMIIM_EditBarDraw
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIIM_EditBarDraw(MMIIM_TEXT_BAR_T const * bar_ptr);

/*****************************************************************************/
//  Description : MMIIM_SpellBarDraw
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIIM_SpellBarDraw(MMIIM_TEXT_BAR_T * bar_ptr, BOOLEAN is_need_border);

/*****************************************************************************/
//  Description : MMIIM_CandBarDraw
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIIM_CandBarDraw(
	MMIIM_TEXT_BAR_T * bar_ptr,
	GUI_RECT_T pre_bar_rect,
	BOOLEAN is_need_border
);

/*****************************************************************************/
//  Description : popup list clear layer
//  Global resource dependence : none
//  Author: cheney.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIIM_PopupListClearLayer(MMIIM_TEXT_BAR_T const * bar_ptr);

/*****************************************************************************/
//  Description : MMIIM_PopupListReleaseLayer
//  Global resource dependence : none
//  Author: cheney.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_PopupListReleaseLayer(MMIIM_TEXT_BAR_T *bar_ptr);

/*****************************************************************************/
//  Description : MMIIM_PredictBarDraw
//  Global resource dependence : none
//  Author: cheney.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIIM_PredictBarDraw(
	MMIIM_TEXT_BAR_T * bar_ptr,
	MMIIM_CAND_WIN_T *cand_info_ptr,
	MMI_DIRECTION_SET_E dis_direction,
	BOOLEAN is_need_border);

/*****************************************************************************/
//  Description : MMIIM_SP_TextBarClear
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIIM_TextBarClear(MMIIM_TEXT_BAR_T *bar_ptr);

/*****************************************************************************/
//  Description : MMIIM_SP_TextBarInit
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIIM_TextBarInit(MMIIM_TEXT_BAR_T * bar_ptr);

/*****************************************************************************/
//  Description : MMIIM_InitIdcRect
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIIM_InitIdcRect(MMIIM_TEXT_BAR_T * bar_ptr);


/*****************************************************************************/
//  Description : popup list create layer for chinese
//  Global resource dependence : none
//  Author:arvin
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_PopupListCreateLayer(MMIIM_TEXT_BAR_T * bar_ptr);

/*****************************************************************************/
//  Description : popup list append layer for chinese
//  Global resource dependence : none
//  Author:arvin
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_PopupListAppendLayer(GUI_LCD_DEV_INFO *dev_info_ptr, GUI_RECT_T *bar_rect);

#endif  //end t9chinese

#endif  //end IM_ENGINE_T9



#ifdef IM_ENGINE_XT9 // functions only in used in xt9 below
#define XT9_SEL_LIST_SIZE   32

#endif


//this type is used in t9 and xt9
typedef struct  
{
    uint16 client_lang;
    uint16 engine_lang;
}IM_LANG_CLINET_ENGINE_CONVERT_T;


//operation common in t9 and xt9
/*****************************************************************************/
//  Description : select highlight word
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_EditorSelectHighlightChars(
    GUIIM_INIT_KERNEL_T *init_param_ptr,
    uint16 first_idx,
    uint16 last_idx
);

/*****************************************************************************/
//  Description : cursor is at the end
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_IsCursorAtTheEnd(GUIIM_INIT_KERNEL_T *init_param_ptr);

/*****************************************************************************/
//  Description : get current cursor position
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_GetEditContent(GUIIM_INIT_KERNEL_T *init_param_ptr, GUIIM_CONTEXT_T *con_ptr);

/*****************************************************************************/
//  Description : MMIIM_IsContentEmpty
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_IsContentEmpty(GUIIM_INIT_KERNEL_T *init_param_ptr);

/*****************************************************************************/
//  Description : MMIIM_NeedSwitchLeadCap
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_NeedSwitchLeadCap(GUIIM_INIT_KERNEL_T *init_param_ptr);

/*****************************************************************************/
//  Description : get current cursor position
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIIM_GetEditCursor(GUIIM_INIT_KERNEL_T *init_param_ptr);

/*****************************************************************************/
//  Description : move cursor
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_SetEditCursor(
    GUIIM_INIT_KERNEL_T *init_param_ptr,
    GUIIM_SETCURSOR_TYPE_E type,
    int16 value
);

/*****************************************************************************/
//  Description : 把高亮的确定
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_ClearImeComp(GUIIM_INIT_KERNEL_T *init_param_ptr);

/*****************************************************************************/
//  Description : Is shade state
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_Editor_IsShadeState(void);

/*****************************************************************************/
//Description : add replace chars
//Global resource dependence : none
//Author: haiwu.chen
//Note: include commit or comp chars, txt_ptr=commit buf+underline buf(in order)
/*****************************************************************************/
PUBLIC void MMIIM_ReplaceChars(
    GUIIM_INIT_KERNEL_T *init_param_ptr,
    uint16 *txt_ptr,
    uint16 commit_len,
    uint16 underline_len
);

/*****************************************************************************/
//  Description : set flag g_is_multi_line_style
//  Global resource dependence : none
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_SetMultiLineStyle(BOOLEAN is_multi_line_style);

/*****************************************************************************/
//  Description : get the flag g_is_multi_line_style value
//  Global resource dependence : none
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_GetMultiLineStyle(CAF_HANDLE_T ctrl_handle);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: arvin
//  Note:
/*****************************************************************************/
PUBLIC MMIIM_T9_CONVERSION_TABLE_DEF_T* MMIIM_CheckConversionTable(T9UINT lang, T9SYMB previous_char);

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
    }
#endif

#endif //_MMIIM_T9_BASE_H_

