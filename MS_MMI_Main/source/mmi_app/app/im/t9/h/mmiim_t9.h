/*****************************************************************************
** File Name:      mmiim_t9.h                                                *
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
#ifndef _MMIIM_T9_H_
#define _MMIIM_T9_H_


#if defined(__cplusplus)
extern "C" {
#endif

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiim_base.h"

#if defined(MMIIM_USE_KB_T9)
#include "mmiim_t9_base.h"


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 

 //hanlde keypad data
typedef struct
{
    CAF_HANDLE_T ctrl_handle;
    GUI_LCD_DEV_INFO *lcd_info_ptr;
    GUIIM_INIT_KERNEL_T init_param;
    T9AWFieldInfo aw_field_info;
    
    GUI_RECT_T rect;

    MMIIM_SP_STATE_T state;
    GUIIM_METHOD_T method;

    MMI_DIRECTION_SET_E dis_direct;

    BOOLEAN is_spell_state;

    //draw flag
    BOOLEAN is_rect_change;
    
    BOOLEAN is_timer_active;

    // recode first pressing down key value
    MMIIM_SP_VKEY_T last_key;
    MMIIM_SP_VKEY_T last_down_key;

    uint16 last_key_index;
    wchar last_char;

    //用位来表示状态: 0 bit:init value; 1 bit: alter value
    //0x01:需要做，实际上还没做，0x03需要做，实际上也到了需要做的时候；0x00，做完就不必再做了
    uint8 cap_auto_switch;
    MMIIM_T9_CAP_SWITCH_E cap_switch_type;
    MMIIM_CAND_WIN_T cand_win;
    
    MMIIM_TEXT_BAR_T predict_bar;
    
    BOOLEAN is_predict_bar_dirty;
    uint8 *t9_ldb_buff;
}MMIIM_T9_DATA_T;
 

typedef struct
{
    MMI_HANDLE_T win_handle;    //last window handle
    MMI_HANDLE_T edit_ctrl_handle;   //editbox ctrl handle
    wchar *pBuf;
    uint16 nBufLen;
    
}MMIIM_T9_ADD_WORD_T;
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
 


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus 
extern "C"{ 
#endif 

#ifdef __cplusplus 
} 
#endif 


#endif //MMIIM_USE_KB_T9

#endif //_MMIIM_T9_H_