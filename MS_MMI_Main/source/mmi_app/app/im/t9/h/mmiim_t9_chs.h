/*****************************************************************************
** File Name:      mmiim_t9_chs.h                                            *
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
//#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_CHINESE_S_SUPPORT) || (defined IM_CHINESE_T_SUPPORT) || (defined IM_TRAD_CHINESE_SUPPORT)

#ifndef _MMIIM_T9_CHS_H_
#define _MMIIM_T9_CHS_H_


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
#define T9_CHS_LDB_MAXNUM  64  /*t9中文最大LDB数目为64*/

 //hanlde keypad data
typedef struct
{
    CAF_HANDLE_T ctrl_handle;
    GUI_LCD_DEV_INFO *lcd_info_ptr;
    GUIIM_INIT_KERNEL_T init_param;
    T9CCFieldInfo cc_field_info;
    T9AWFieldInfo aw_field_info;
    MMIIM_SP_STATE_T state;
    GUIIM_METHOD_T method;
    
    //draw flag
    BOOLEAN is_rect_change; 
    
    GUI_RECT_T rect;
    GUI_RECT_T bar_rect_0;
    GUI_RECT_T bar_rect_1; 

    MMIIM_TEXT_BAR_T bar_cand;
    MMIIM_TEXT_BAR_T bar_edit;
    
    BOOLEAN is_bar_cand_dirty;
    BOOLEAN is_bar_edit_dirty;
    
    MMIIM_SP_VKEY_T last_key;
    //MMIIM_T9_LDB_DATA_T *t9_ldb_data;
    uint8 *t9_ldb_data[T9_CHS_LDB_MAXNUM + 1];
    uint8 *t9_ldb_buff;
    
}MMIIM_T9_CHS_DATA_T;


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

#endif //_MMIIM_T9_CHS_H_

//#endif //#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_CHINESE_S_SUPPORT)