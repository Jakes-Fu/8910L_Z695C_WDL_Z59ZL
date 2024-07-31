/*****************************************************************************
** File Name:      guiprgbox.c                                               *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2007      Jassmine              Creat
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "guicommon.h"
#include "guires.h"
#include "mmi_theme.h"
#include "guistring.h"
#include "guigraph.h"

#include "ctrlprogressbar.h"
#include "mmi_string.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define     PRGBOX_MIN_BAR_LENGTH           8
#define		PRGBOX_ARROW_WIDTH				18
#define     UILAYER_ALPHA_MAX (255)
#define     PRGBOX_LETTER_SCALE              3
#define     VOLUME_BRIGHTNESS_START_LENGTH     32
#define     VOLUME_BRIGHTNESS_RAISE_LENGTH     26
#define     VOLUME_BRIGHTNESS_BAR_MARGIN       6
#define     VOLUME_BRIGHTNESS_BAR_TEXT_HEIGHT  24
#define     VOLUME_BRIGHTNESS_BAR_BG_HEIGHT    28
#define     VOLUME_BRIGHTNESS_BG_TOP_MARGIN    3
#define     VOLUME_BRIGHTNESS_BG_BOTTOM_MARGIN 1
#define     VOLUME_BAR_ZERO_BORDER             2
#define     VOLUME_BRIGHTNESS_TEXT_LENGTH      50

#define     NPS_SCORE_START_LENGTH     33
#define     NPS_SCORE_RAISE_LENGTH     23
#define     NPS_SCORE_LABEL_WIDTH      32
#define MAKE_BREW_RGB(r,g,b) ((r >> 3)<<11 | (g>>2)<<5 | (b>>3))

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init progressbar class
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressBarInitVtbl (
    CTRLPROGRESSBAR_VTBL_T *owndraw_vtbl_ptr
);
LOCAL BOOLEAN ProgressBarSetDisplayRect(
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
);

/*****************************************************************************/
//  Description : progressbar construct
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressBarConstruct (
    CTRLBASE_OBJ_T *base_ctrl_ptr,
    void *param_ptr
);

/*****************************************************************************/
//  Description : progressbar destruct
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressBarDestruct (
    CTRLBASE_OBJ_T *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ProgressBarCtrlPackInitParam (
    GUIPRGBOX_INIT_DATA_T *progressbar_init_ptr, //in
    CTRLPROGRESSBAR_INIT_PARAM_T *progressbar_param_ptr //out
);

/*****************************************************************************/
// 	Description : display progress box
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressBarDraw (
    CTRLPROGRESSBAR_OBJ_T   *progressbar_ptr
);

/*****************************************************************************/
// 	Description : display vertical scroll box
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayVerticalScroll (
    CTRLPROGRESSBAR_OBJ_T   *progressbar_ptr
);

/*****************************************************************************/
// 	Description : display scroll bar
//	Global resource dependence :
//  Author: James
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayScrollBar (
    GUI_RECT_T          rect,           //display rect
    CTRLPROGRESSBAR_OBJ_T    *progressbar_ptr,
    BOOLEAN             is_vertical
);

/*****************************************************************************/
// 	Description : display horizontal scroll box
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayHorizontalScroll (
    CTRLPROGRESSBAR_OBJ_T  *progressbar_ptr
);

#ifdef GUIF_PRGBOX_LETTERSCROLL
/*****************************************************************************/
// 	Description : display horizontal scroll box
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayLettersScroll (
    CTRLPROGRESSBAR_OBJ_T  *progressbar_ptr
);

/*****************************************************************************/
//  Description : 初始化字母表
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void InitLettersTable(
    CTRLPROGRESSBAR_OBJ_T *prgbox_ctrl_ptr
);

/*==============================================================================
Description: 创建显示的层
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 在初始化及屏幕旋转的时候，都需要建立显示层
==============================================================================*/
LOCAL BOOLEAN CreateLetterBarDispLayer(CTRLPROGRESSBAR_OBJ_T *prgbox_ctrl_ptr);
#endif

/*****************************************************************************/
//  Description : display vertical or horizontal process box
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayProcess (
    GUIPRGBOX_STYLE_E      style,  //style
    CTRLPROGRESSBAR_OBJ_T       *progressbar_ptr
);

/*****************************************************************************/
//  Description : display vertical or horizontal process box
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayVolumeBar (
    CTRLPROGRESSBAR_OBJ_T       *progressbar_ptr
);

/*****************************************************************************/
//  Description : display KeyPad Volume
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayKeyPadVolumeBar (
								CTRLPROGRESSBAR_OBJ_T  	*progressbar_ptr
);

/*****************************************************************************/
//  Description : display vertical or horizontal process box
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayBrightnessBar (
    CTRLPROGRESSBAR_OBJ_T       *progressbar_ptr
);


/*****************************************************************************/
//  Description : display vertical or horizontal score bar
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayNPSScoreBar (
    CTRLPROGRESSBAR_OBJ_T       *progressbar_ptr
);
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle tp press key message for progress like windows.
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL GUIPRGBOX_RESULT_E HandleWindowsTpMsg (
    CTRLPROGRESSBAR_OBJ_T	 *progressbar_ptr,
    uint32               *first_item_index_ptr, //in/out
    uint32               *cur_item_index_ptr,   //in/out:
    GUI_POINT_T          point
);

/*****************************************************************************/
//  Description : handle tp press key message
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleScrollTpMsg (
    uint32                *first_item_index_ptr,//in/out
    uint32                *cur_item_index_ptr,  //in/out:
    GUI_POINT_T           tp_point,
    CTRLPROGRESSBAR_OBJ_T      *progressbar_ptr,
    GUIPRGBOX_RESULT_E    *handle_result_ptr    //in/out:handle result
);

/*****************************************************************************/
//  Description : handle tp press key message
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTpDownMsg (//if handle
    uint32                *cur_item_index_ptr,	//in/out:
    uint32                *first_item_index_ptr,  //in/out
    GUI_POINT_T           tp_point,
    CTRLPROGRESSBAR_OBJ_T      *progressbar_ptr,
    GUIPRGBOX_RESULT_E    *handle_result_ptr  //in/out:handle result
);

/*****************************************************************************/
//  Description : handle tp move key message
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTpMoveMsg (
    uint32                *first_item_index_ptr,  //in/out
    uint32                *cur_item_index_ptr,	//in/out:
    GUI_POINT_T           tp_point,
    CTRLPROGRESSBAR_OBJ_T      *progressbar_ptr,
    GUIPRGBOX_RESULT_E    *handle_result_ptr  //in/out:handle result
);

/*****************************************************************************/
//  Description : handle tp press key message
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTpMsg (//if handle
    uint32                *cur_item_index_ptr,	//in/out:
    uint32                *first_item_index_ptr,  //in/out
    GUI_POINT_T           tp_point,
    CTRLPROGRESSBAR_OBJ_T      *progressbar_ptr,
    GUIPRGBOX_RESULT_E    *handle_result_ptr,  //in/out:handle result
    MMI_MESSAGE_ID_E      msg_id
);
#endif

/*****************************************************************************/
//  Description :  Get Scroll Bar Height
//  Global resource dependence : none
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetScrollBarLength (
    CTRLPROGRESSBAR_OBJ_T	*progressbar_ptr,
    uint32				cur_item_index,
    uint32				first_item_index
);

/*****************************************************************************/
//  Description :  Get Scroll Bar rectangle
//  Global resource dependence : none
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetScrollBarRect (
    CTRLPROGRESSBAR_OBJ_T	*progressbar_ptr,
    uint32    		cur_item_index,
    uint32		    first_item_index
);

/*****************************************************************************/
//  Description :  Get Scroll rectangle, excluding arrow rectangle.
//  Global resource dependence : none
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetScrollRect (
    CTRLPROGRESSBAR_OBJ_T * progressbar_ptr
);

/*****************************************************************************/
//  Description :  get current item index according to point position
//  Global resource dependence : none
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetCurrentItem (
    CTRLPROGRESSBAR_OBJ_T * progressbar_ptr,
    uint32 position
);

/*****************************************************************************/
//  Description :  init arrow rectangle
//  Global resource dependence : none
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void InitArrowRect (
    CTRLPROGRESSBAR_OBJ_T * progressbar_ptr
);

/*****************************************************************************/
// 	Description : handle progress box msg function
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ProgressBarHandleMsg (
    CTRLBASE_OBJ_T         *ctrl_ptr,  //control pointer
    MMI_MESSAGE_ID_E   msg_id,  	//message
    DPARAM             param       //add data
);

/*****************************************************************************/
//  Description : init progress box control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ProgressBarInit (
    CTRLBASE_OBJ_T         *ctrl_ptr,  //control pointer
    CTRLPROGRESSBAR_INIT_PARAM_T *init_param_ptr
);

/*****************************************************************************/
//  Description : Get probox list pointer from control id
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL CTRLPROGRESSBAR_OBJ_T* GetProBoxPtr (
    MMI_HANDLE_T ctrl_handle
);

/*****************************************************************************/
//  Description : probox type of
//  Global resource dependence :
//  Author:James.Zhang
//  Note:该函数进行运行时类型判别
/*****************************************************************************/
LOCAL BOOLEAN ProBoxTypeOf (
    CTRLBASE_OBJ_T* probox_ptr
);

/*****************************************************************************/
//  Description : send move msg
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void PostMoveItemMsg (
    const CTRLPROGRESSBAR_OBJ_T* progressbar_ptr,
    MMI_MESSAGE_ID_E        notify_code,
    uint32                  first_index
);

/*****************************************************************************/
//  Description : prgbox set rect
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressBarSetRect (
    CTRLBASE_OBJ_T *base_ctrl_ptr,
    const GUI_RECT_T *rect_ptr
);

/*****************************************************************************/
//  Description : Display rect progressbar
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayRectBarProcess (
    CTRLPROGRESSBAR_OBJ_T       *progressbar_ptr
);

/*****************************************************************************/
//  Description : Set rect progressbar theme color (DIALOG/DEFAULT)
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetRectBarTheme (
    CTRLPROGRESSBAR_OBJ_T   *progressbar_ptr
);


#if defined GUIF_PRGBOX_AUTOHIDE || defined GUIF_PRGBOX_LETTERSCROLL
LOCAL void ReleaseDispLayer (CTRLPROGRESSBAR_OBJ_T *progressbar_ptr);
#endif

LOCAL void GetDispRect (CTRLPROGRESSBAR_OBJ_T *progressbar_ptr, GUI_RECT_T *rect_ptr);

#if defined(GUIF_PRGBOX_AUTOHIDE)
LOCAL void ClearDispInfo (CTRLPROGRESSBAR_OBJ_T *progressbar_ptr);
LOCAL BOOLEAN CreateDispLayer (CTRLPROGRESSBAR_OBJ_T *progressbar_ptr);

LOCAL BOOLEAN HandleAutoHideTpMsg (
    uint32                *cur_item_index_ptr, //[in/out]
    uint32                *first_item_index_ptr, //[in/out]
    GUI_POINT_T           tp_point,
    CTRLPROGRESSBAR_OBJ_T      *progressbar_ptr,
    GUIPRGBOX_RESULT_E    *handle_result_ptr, //[in/out]
    MMI_MESSAGE_ID_E      msg_id
);
LOCAL BOOLEAN DisplayAutoHideScroll (CTRLPROGRESSBAR_OBJ_T   *progressbar_ptr);
LOCAL void RemovePrgCB (uint8 timer_id, uint32 param);
LOCAL void OutPrgCB (uint8 timer_id, uint32 param);

LOCAL BOOLEAN NeedShowThumb (CTRLPROGRESSBAR_OBJ_T *progressbar_ptr);
LOCAL uint32 GetFirstIndexByThumbTop (
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr,
    int16 *thumb_top_ptr
);
LOCAL uint16 GetThumbTopByFirstIndex (
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr,
    int32 first_index
);
#endif


/*****************************************************************************/
//  Description :  get current item index according to point position
//  Global resource dependence : none
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetFirstItem (
    CTRLPROGRESSBAR_OBJ_T	*progressbar_ptr,
    uint32            first_index,
    int32             position
);

/*****************************************************************************/
//  Description : set parameter: total item count and item count per page
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void SetProgressBarParamByPtr (
    CTRLPROGRESSBAR_OBJ_T   *progressbar_ptr,
    uint32                  total_item_count,
    uint32                  items_per_page
);

/*****************************************************************************/
//  Description : Display rect waiting indicator
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayRectWaitingIndicator (
                                            CTRLPROGRESSBAR_OBJ_T       *progressbar_ptr
                                           );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get ownerdraw type
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T CTRLPROGRESSBAR_GetType()
{
    static TYPE_TAG_T obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,
                       "progressbar",
                       sizeof (CTRLPROGRESSBAR_OBJ_T),
                       (OBJECT_CONSTRUCTOR_FUNC) ProgressBarConstruct,
                       (OBJECT_DESTRUCTOR_FUNC) ProgressBarDestruct,
                       sizeof(CTRLPROGRESSBAR_INIT_PARAM_T),
                       (PARAM_PACKING_FUNC)ProgressBarCtrlPackInitParam,
                       sizeof (CTRLPROGRESSBAR_VTBL_T),
                       (VTBL_INIT_FUNC) ProgressBarInitVtbl);
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init progressbar class
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressBarInitVtbl (
    CTRLPROGRESSBAR_VTBL_T *progressbar_vtbl_ptr
)
{
    CTRLBASE_VTBL_T *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) progressbar_vtbl_ptr;

    if (PNULL == progressbar_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = ProgressBarHandleMsg;
    base_ctrl_vtbl_ptr->SetRect = ProgressBarSetRect;
    //ProgressBar有内嵌在Form中的情况
    base_ctrl_vtbl_ptr->GetDisplayRect = BaseCtrlGetDisplayRectNotImp;
	base_ctrl_vtbl_ptr->SetDisplayRect = ProgressBarSetDisplayRect;
    return TRUE;
}

/*****************************************************************************/
//  Description : set progressbar display rect
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressBarSetDisplayRect(
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
)
{
    BOOLEAN         result = TRUE;
    CTRLPROGRESSBAR_OBJ_T  *probar_ctrl_ptr = PNULL;
    probar_ctrl_ptr = (CTRLPROGRESSBAR_OBJ_T *)base_ctrl_ptr;
    if ((PNULL != probar_ctrl_ptr)&&(PNULL != rect_ptr))
    {
        base_ctrl_ptr->display_rect = *rect_ptr;
        if (is_update)
        {
            CTRLPROGRESSBAR_Update(probar_ctrl_ptr->base_ctrl.ctrl_id);
        }
    }
    return (result);
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ProgressBarCtrlPackInitParam (
    GUIPRGBOX_INIT_DATA_T *init_ptr, //in
    CTRLPROGRESSBAR_INIT_PARAM_T *param_ptr //out
)
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*)param_ptr;

    if ((PNULL == init_ptr) || (PNULL == param_ptr))
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = init_ptr->both_rect;

    param_ptr->is_forbid_paint = init_ptr->is_forbid_paint;
    param_ptr->lcd_id = init_ptr->lcd_id;
    param_ptr->style = init_ptr->style;
    param_ptr->theme_type = init_ptr->theme_type;
}

/*****************************************************************************/
//  Description : owner draw control construct
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressBarConstruct (
    CTRLBASE_OBJ_T *base_ctrl_ptr,
    void *param_ptr
)
{
    BOOLEAN result = TRUE;

    if ( (PNULL == base_ctrl_ptr) || (PNULL == param_ptr))
    {
        return FALSE;
    }

    ProgressBarInit (base_ctrl_ptr, (CTRLPROGRESSBAR_INIT_PARAM_T *) param_ptr);
    //set state
    VTLBASE_SetCanActive(base_ctrl_ptr, TRUE);

    return result;
}

/*****************************************************************************/
//  Description : owner draw control destruct
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressBarDestruct (
    CTRLBASE_OBJ_T *base_ctrl_ptr
)
{
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr = (CTRLPROGRESSBAR_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr)
    {
        return FALSE;
    }

    //释放基本字母表
    if (progressbar_ptr->base_letters_table != PNULL)
    {
        SCI_FREE (progressbar_ptr->base_letters_table);
        progressbar_ptr->base_letters_table = PNULL;
    }

    if (progressbar_ptr->letter_postion_table != PNULL)
    {
        SCI_FREE (progressbar_ptr->letter_postion_table);
        progressbar_ptr->letter_postion_table = PNULL;
    }

#if defined(GUIF_PRGBOX_AUTOHIDE)
    ReleaseDispLayer (progressbar_ptr);
#else
#ifdef GUIF_PRGBOX_LETTERSCROLL

    if (GUIPRGBOX_STYLE_LETTERS_SCROLL == progressbar_ptr->prgbox_style)
    {
        ReleaseDispLayer (progressbar_ptr);
    }

#endif
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description : init progress box control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ProgressBarInit (
    CTRLBASE_OBJ_T *base_ctrl_ptr,
    CTRLPROGRESSBAR_INIT_PARAM_T *init_param_ptr
)
{
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr = (CTRLPROGRESSBAR_OBJ_T*) base_ctrl_ptr;
    GUI_RECT_T             horizontal_progress_rect = {0};    

	GUI_RECT_T label_rect = {0};
	GUI_RECT_T fg_rect = {0};
	uint16 lcd_width = MMITHEME_GetClientRectWidth();
	
	label_rect.left = VOLUME_BRIGHTNESS_BAR_MARGIN; 
	label_rect.top = VOLUME_BRIGHTNESS_BAR_MARGIN; 
	label_rect.right = RECT_PROGRESSBAR_WIDTH - 1; 
	label_rect.bottom = VOLUME_BRIGHTNESS_BAR_MARGIN + VOLUME_BRIGHTNESS_BAR_TEXT_HEIGHT - 1;
	
	fg_rect.left = 0; 
	fg_rect.top = VOLUME_BRIGHTNESS_BAR_MARGIN + VOLUME_BRIGHTNESS_BAR_TEXT_HEIGHT;
	fg_rect.right	= lcd_width - 1; 
	fg_rect.bottom = VOLUME_BRIGHTNESS_BAR_HEIGHT - VOLUME_BRIGHTNESS_BAR_MARGIN - 1;

	if(init_param_ptr->style == GUIPRGBOX_STYLE_VOLUME_BAR)
	{
		init_param_ptr->theme_type = MMITHEME_PRGBOX_TYPE_VOLUME;
	}
	else if(init_param_ptr->style == GUIPRGBOX_STYLE_BRIGHTNESS_BAR)
	{
		init_param_ptr->theme_type = MMITHEME_PRGBOX_TYPE_BRIGHTNESS;
	}
	else if(init_param_ptr->style == GUIPRGBOX_STYLE_NPS_SCORE_BAR)
	{
		init_param_ptr->theme_type = MMITHEME_PRGBOX_TYPE_NPS_SCORE;
	}
	else if(init_param_ptr->style == GUIPRGBOX_STYLE_KEYPAD_VOLUME_BAR)
	{
	       init_param_ptr->theme_type =MMITHEME_PRGBOX_TYPE_KEYPAD_VOLUME;
	}

    //get theme
    MMITHEME_GetVerticalScrollTheme (init_param_ptr->theme_type, &progressbar_ptr->ver_scroll_theme);
    MMITHEME_GetHorizontalScrollTheme (init_param_ptr->theme_type, &progressbar_ptr->hor_scroll_theme);
    MMITHEME_GetProcessTheme (init_param_ptr->theme_type, &progressbar_ptr->process_theme);

    //set other info
    base_ctrl_ptr->lcd_dev_info.lcd_id = (GUI_LCD_ID_E)init_param_ptr->lcd_id;

    progressbar_ptr->win_id = MMK_GetWinHandleByCtrl (base_ctrl_ptr->handle);
    progressbar_ptr->prgbox_style = init_param_ptr->style;
    progressbar_ptr->is_forbid_paint = init_param_ptr->is_forbid_paint;

#if defined(GUIF_PRGBOX_AUTOHIDE)
    progressbar_ptr->disp_layer.block_id = UILAYER_NULL_HANDLE;

    GUIRES_GetImgWidthHeight (&progressbar_ptr->thumb_width,
                              &progressbar_ptr->thumb_height,
                              progressbar_ptr->ver_scroll_theme.thumb_img_id,
                              progressbar_ptr->win_id);
#else
    progressbar_ptr->disp_layer = base_ctrl_ptr->lcd_dev_info;
#endif

    // 初始化箭头区域
    InitArrowRect (progressbar_ptr);

    progressbar_ptr->is_move_valid = FALSE;
    progressbar_ptr->point_offset = 0;
    progressbar_ptr->is_loop_scroll = TRUE;

    // 是否显示滑块
    progressbar_ptr->is_need_slide_block = TRUE; // 默认情况下需要滑块

    // 是否显示背景
    progressbar_ptr->is_need_bg = TRUE;

#ifdef GUIF_PRGBOX_LETTERSCROLL
    if (GUIPRGBOX_STYLE_LETTERS_SCROLL == init_param_ptr->style)
    {
        //字母滚动条：初始化基本字母表
        InitLettersTable (progressbar_ptr);
    }
	
#endif

    MMITHEME_GetPrgLcdDev (&base_ctrl_ptr->lcd_dev_info);

    if (GUIPRGBOX_STYLE_HORIZONTAL_PROCESS == init_param_ptr->style)
    {    
    	horizontal_progress_rect.top = 0;
    	horizontal_progress_rect.bottom = horizontal_progress_rect.top + RECT_PROGRESSBAR_HEIGHT;
    	horizontal_progress_rect.left = 0;
    	horizontal_progress_rect.right = horizontal_progress_rect.left + RECT_PROGRESSBAR_WIDTH;        
    	GUI_SetVOrHRect( base_ctrl_ptr->handle, (GUI_RECT_T *)&horizontal_progress_rect,&(base_ctrl_ptr->both_rect));     
    }
    else
    {
    	progressbar_ptr->label_rect = label_rect;
    	progressbar_ptr->fg_rect = fg_rect;
    }

    //Set rect rpgoress theme 
    if ((GUIPRGBOX_STYLE_HORIZONTAL_PROCESS == init_param_ptr->style) ||
		(GUIPRGBOX_STYLE_RECT_WAITING_INDICATOR_PROCESS == init_param_ptr->style)
		)
    {
        SetRectBarTheme(progressbar_ptr);   
    }

    return;
}

#ifdef GUIF_PRGBOX_LETTERSCROLL
/*****************************************************************************/
//  Description : 初始化字母表
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void InitLettersTable (
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr
)
{
    uint32              i = 0;

    if (progressbar_ptr != PNULL)
    {
//temp only support english
        progressbar_ptr->is_letter_bar_valid = TRUE;
        progressbar_ptr->base_letters_table = SCI_ALLOC_APPZ (sizeof (uint16) * ('Z' - 'A' + 2));
        //符号
        *progressbar_ptr->base_letters_table = '#';
        progressbar_ptr->letters_table_len = 1;

        //字母
        for (i = 0; i <= ('Z' - 'A'); i++)
        {
            * (progressbar_ptr->base_letters_table + i + 1) = 'A' + i;
        }

        progressbar_ptr->letters_table_len += ('Z' - 'A') + 1;

        progressbar_ptr->letter_postion_table = SCI_ALLOC_APPZ (sizeof (uint16) * progressbar_ptr->letters_table_len);
    }
}
#endif

/*****************************************************************************/
//  Description : send move msg
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void PostMoveItemMsg (
    const CTRLPROGRESSBAR_OBJ_T* progressbar_ptr,
    MMI_MESSAGE_ID_E        notify_code,
    uint32                  first_index
)
{
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T*)progressbar_ptr;

    if (PNULL == progressbar_ptr)
    {
        return;
    }

    CTRLMSG_PostNotifyEx (base_ctrl_ptr->handle, notify_code, &first_index, sizeof (uint32));
}

/*****************************************************************************/
// 	Description : handle progress box msg function
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ProgressBarHandleMsg (
    CTRLBASE_OBJ_T         *ctrl_ptr,  //control pointer
    MMI_MESSAGE_ID_E   msg_id,  	//message
    DPARAM             param       //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLPROGRESSBAR_OBJ_T    *progressbar_ptr = (CTRLPROGRESSBAR_OBJ_T *) ctrl_ptr;
	CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T*)progressbar_ptr;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T*)progressbar_ptr;
#endif

    if (PNULL == ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    GUI_INVALID_PARAM (param);/*lint !e522*/

    switch (msg_id)
    {
#if defined(GUIF_PRGBOX_AUTOHIDE)
    case MSG_CTL_OPEN:
		if(progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_VERTICAL_WINDOWS || progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_RECT_WAITING_INDICATOR_PROCESS)
		{
			progressbar_ptr->is_active = TRUE;
		}
		
        break;
		
    case MSG_CTL_GET_ACTIVE:
    //case MSG_CTL_GET_FOCUS:
        progressbar_ptr->is_active = TRUE;
		ProgressBarDraw (progressbar_ptr);
		result = CTRLMSG_SendNotify(base_ctrl_ptr->handle,MSG_NOTIFY_GET_ACTIVE);
        break;

    //case MSG_CTL_CLOSE:
    case MSG_CTL_LOSE_ACTIVE:
    //case MSG_CTL_LOSE_FOCUS:
        progressbar_ptr->is_active = FALSE;
        ClearDispInfo (progressbar_ptr);
		ProgressBarDraw (progressbar_ptr);
		result = CTRLMSG_SendNotify(base_ctrl_ptr->handle,MSG_NOTIFY_LOSE_ACTIVE);
        break;

#ifdef GUIF_PRGBOX_LETTERSCROLL
    case MSG_LCD_SWITCH:
        ReleaseDispLayer (progressbar_ptr);

        if ( (GUIPRGBOX_STYLE_LETTERS_SCROLL == progressbar_ptr->prgbox_style))
        {
            //需要重新设置total item
            SetProgressBarParamByPtr (
                    progressbar_ptr,
                    progressbar_ptr->total_item_num,
                    progressbar_ptr->items_per_page);
        }

        break;
#endif

#else

#ifdef GUIF_PRGBOX_LETTERSCROLL
    case MSG_LCD_SWITCH:
        ReleaseDispLayer (progressbar_ptr);

        if ( (GUIPRGBOX_STYLE_LETTERS_SCROLL == progressbar_ptr->prgbox_style))
        {
            //需要重新设置total item
            SetProgressBarParamByPtr (
                    progressbar_ptr,
                    progressbar_ptr->total_item_num,
                    progressbar_ptr->items_per_page);
        }

        break;
#endif

	case MSG_CTL_GET_ACTIVE:
		{
			progressbar_ptr->is_active = TRUE;
			ProgressBarDraw (progressbar_ptr);
			result = CTRLMSG_SendNotify(base_ctrl_ptr->handle,MSG_NOTIFY_GET_ACTIVE);
		}
		break;

	case MSG_CTL_LOSE_ACTIVE:
		{
			progressbar_ptr->is_active = FALSE;
			ProgressBarDraw (progressbar_ptr);
		}
		break;
#endif

    case MSG_CTL_PAINT:

        if (!progressbar_ptr->is_forbid_paint)
        {
            ProgressBarDraw (progressbar_ptr);
        }

        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
    case MSG_TP_PRESS_UP:
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T			point = {0};
            uint32			    cur_item_index = 0;
            uint32			    first_item_index = 0;
            BOOLEAN			    ret = FALSE;
            GUIPRGBOX_RESULT_E	handle_result = GUIPRGBOX_RESULT_NONE;

            if (MSG_TP_PRESS_DOWN == msg_id)
            {
                CTRLMSG_PostNotifyEx (base_ctrl_ptr->handle, MSG_NOTIFY_PRGBOX_CLICK, PNULL, 0);
            }

            point.x = MMK_GET_TP_X (param);
            point.y = MMK_GET_TP_Y (param);
            cur_item_index = progressbar_ptr->cur_item_index;
            first_item_index = progressbar_ptr->first_item_index;

#if defined(GUIF_PRGBOX_AUTOHIDE)

            if (GUIPRGBOX_STYLE_VERTICAL_WINDOWS == progressbar_ptr->prgbox_style)
            {
                ret = HandleAutoHideTpMsg (&cur_item_index,
                                           &first_item_index,
                                           point,
                                           progressbar_ptr,
                                           &handle_result,
                                           msg_id);
            }
            else
            {
                ret = HandleTpMsg (&cur_item_index, &first_item_index, point, progressbar_ptr, &handle_result, msg_id);
            }

#else
            ret = HandleTpMsg (&cur_item_index, &first_item_index, point, progressbar_ptr, &handle_result, msg_id);
#endif

            if (ret)
            {
#ifdef GUIF_PRGBOX_LETTERSCROLL

                //send msg to parent ctrl
                if (GUIPRGBOX_STYLE_LETTERS_SCROLL == progressbar_ptr->prgbox_style)
                {
                    PostMoveItemMsg (progressbar_ptr, MSG_NOTIFY_PRGBOX_MOVE, cur_item_index);
                }
                else
#endif
                {
                    switch (handle_result)
                    {
                    case GUIPRGBOX_RESULT_NONE:
                        break;

                    case GUIPRGBOX_RESULT_HEAD:
                        break;

                    case GUIPRGBOX_RESULT_TAIL:
                        break;

                    case GUIPRGBOX_RESULT_PRE_ONE:
                        PostMoveItemMsg (progressbar_ptr, MSG_NOTIFY_PRGBOX_MOVEUP_ONE, first_item_index);
                        break;

                    case GUIPRGBOX_RESULT_NEXT_ONE:
                        PostMoveItemMsg (progressbar_ptr, MSG_NOTIFY_PRGBOX_MOVEDOWN_ONE, first_item_index);
                        break;

                    case GUIPRGBOX_RESULT_PRE_PAGE:
                        PostMoveItemMsg (progressbar_ptr, MSG_NOTIFY_PRGBOX_MOVEUP_PAGE, first_item_index);
                        break;

                    case GUIPRGBOX_RESULT_NEXT_PAGE:
                        PostMoveItemMsg (progressbar_ptr, MSG_NOTIFY_PRGBOX_MOVEDOWN_PAGE, first_item_index);
                        break;

                    case GUIPRGBOX_RESULT_PRE_MOVE:
                    case GUIPRGBOX_RESULT_NEXT_MOVE:
                        PostMoveItemMsg (progressbar_ptr, MSG_NOTIFY_PRGBOX_MOVE, first_item_index);
                        break;

                    default:
                        break;
                    }
                }

            }
            else
            {
                result = MMI_RESULT_FALSE;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_UPDATE_COLOR_THEME: 
        if (TRUE == progressbar_ptr->is_waiting_indicator)
        {
            MMITHEME_GetDialogWaitingIndTheme(&progressbar_ptr->process_theme);
        }        
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : prgbox set rect
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressBarSetRect (
    CTRLBASE_OBJ_T *base_ctrl_ptr,
    const GUI_RECT_T *rect_ptr
)
{
    BOOLEAN          result = FALSE;
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr = PNULL;

    if (PNULL == base_ctrl_ptr || PNULL == rect_ptr)
    {
        return FALSE;
    }

    progressbar_ptr = (CTRLPROGRESSBAR_OBJ_T *) base_ctrl_ptr;

    if (!GUI_EqualRect (base_ctrl_ptr->rect, *rect_ptr))
    {
        base_ctrl_ptr->rect = *rect_ptr;

#if defined(GUIF_PRGBOX_AUTOHIDE)
        ReleaseDispLayer (progressbar_ptr);
#endif

        GUI_SetVOrHRect (base_ctrl_ptr->handle, (GUI_RECT_T *) rect_ptr, & (base_ctrl_ptr->both_rect));

        InitArrowRect (progressbar_ptr);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : display progress box
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressBarDraw (
    CTRLPROGRESSBAR_OBJ_T   *progressbar_ptr
)
{
    BOOLEAN     result = FALSE;
#if defined(GUIF_PRGBOX_AUTOHIDE)
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T*)progressbar_ptr;
#endif

    if (PNULL == progressbar_ptr)
    {
        return FALSE;
    }

    switch (progressbar_ptr->prgbox_style)
    {
    case GUIPRGBOX_STYLE_VERTICAL_SCROLL:
        result = DisplayVerticalScroll (progressbar_ptr);
        break;

    case GUIPRGBOX_STYLE_HORIZONTAL_SCROLL:
        result = DisplayHorizontalScroll (progressbar_ptr);
        break;

    case GUIPRGBOX_STYLE_VERTICAL_WINDOWS:
#if defined(GUIF_PRGBOX_AUTOHIDE)
        if (!progressbar_ptr->is_active)
        {
            result = FALSE;
            break;
        }
#ifdef UI_MULTILAYER_SUPPORT
        CreateDispLayer (progressbar_ptr);

        //加载层
        {
            UILAYER_APPEND_BLT_T append_layer = {0};

            ClearDispInfo (progressbar_ptr);

            append_layer.lcd_dev_info = progressbar_ptr->disp_layer;
            append_layer.layer_level = UILAYER_LEVEL_HIGH;

            UILAYER_AppendBltLayer (&append_layer);
        }
#endif
        //draw
        result = DisplayAutoHideScroll (progressbar_ptr);


        //起timer移除层
        if (0 != progressbar_ptr->remove_timer_id)
        {
            MMK_StopTimer (progressbar_ptr->remove_timer_id);
            progressbar_ptr->remove_timer_id = 0;
        }

        progressbar_ptr->remove_timer_id = MMK_CreateTimerCallback (
                                               progressbar_ptr->ver_scroll_theme.hide_tick,
                                               RemovePrgCB,
                                               (uint32) base_ctrl_ptr->handle,
                                               FALSE);
#else
        result = DisplayVerticalScroll (progressbar_ptr);
#endif
        break;

    case GUIPRGBOX_STYLE_HORIZONTAL_WINDOWS:
        result = DisplayHorizontalScroll (progressbar_ptr);
        break;

    case GUIPRGBOX_STYLE_VERTICAL_PROCESS:
//    case GUIPRGBOX_STYLE_HORIZONTAL_PROCESS:
        result = DisplayProcess (progressbar_ptr->prgbox_style, progressbar_ptr);
        break;
    case GUIPRGBOX_STYLE_HORIZONTAL_PROCESS:
        result = DisplayRectBarProcess (progressbar_ptr);
        break;
                
    case GUIPRGBOX_STYLE_RECT_WAITING_INDICATOR_PROCESS:    
        result = DisplayRectWaitingIndicator (progressbar_ptr);
        break;		
        
	case GUIPRGBOX_STYLE_VOLUME_BAR:
        result = DisplayVolumeBar (progressbar_ptr);
        break;

    case GUIPRGBOX_STYLE_KEYPAD_VOLUME_BAR:
		result = DisplayKeyPadVolumeBar (progressbar_ptr);
		break;

	case GUIPRGBOX_STYLE_BRIGHTNESS_BAR:
        result = DisplayBrightnessBar (progressbar_ptr);
        break;
	case GUIPRGBOX_STYLE_NPS_SCORE_BAR:
		result = DisplayNPSScoreBar (progressbar_ptr);
        break;
	
#ifdef GUIF_PRGBOX_LETTERSCROLL
    case GUIPRGBOX_STYLE_LETTERS_SCROLL:
#ifdef UI_MULTILAYER_SUPPORT
        /*字母滚动条*/
        CreateLetterBarDispLayer (progressbar_ptr);

        //加载层
        {
            UILAYER_APPEND_BLT_T append_layer = {0};

            append_layer.lcd_dev_info = progressbar_ptr->disp_layer;
            append_layer.layer_level = UILAYER_LEVEL_HIGH;

            UILAYER_AppendBltLayer (&append_layer);
        }
#endif
        result = DisplayLettersScroll (progressbar_ptr);
        break;
#endif
    default:
        SCI_PASSERT (FALSE, ("ProgressBarDraw: prgbox style %d is error!", progressbar_ptr->prgbox_style)); /*assert verified*/
        break;
    }

    return (result);
}


/*****************************************************************************/
//  Description : 判断箭头是否能显示完整，如果不能，则不显示箭头
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisplayArrow (
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr
)
{
    BOOLEAN is_true = TRUE;
    uint16 pre_arrow_width = 0; // 前一个箭头
    uint16 pre_arrow_height = 0; // 前一个箭头
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != progressbar_ptr); /*assert verified*/
    if (PNULL == progressbar_ptr)
    {
        return FALSE;
    }

    // 获取箭头的宽度

    switch (progressbar_ptr->prgbox_style)
    {
    case GUIPRGBOX_STYLE_VERTICAL_SCROLL:    //vertical scroll
    case GUIPRGBOX_STYLE_VERTICAL_WINDOWS:   //horizontal scroll like window style
    case GUIPRGBOX_STYLE_VERTICAL_PROCESS:   //vertical process
#ifndef MMI_RES_LOW_COST
        GUIRES_GetImgWidthHeight (&pre_arrow_width, &pre_arrow_height, progressbar_ptr->ver_scroll_theme.up_arrow_id, progressbar_ptr->win_id);

        if (base_ctrl_ptr->rect.right - base_ctrl_ptr->rect.left + 1 < pre_arrow_width
                || base_ctrl_ptr->rect.bottom - base_ctrl_ptr->rect.top + 1 < pre_arrow_height)
        {
            is_true = FALSE;
        }

#else
        is_true = FALSE;
#endif
        break;
    case GUIPRGBOX_STYLE_HORIZONTAL_SCROLL:  //horizontal scroll
    case GUIPRGBOX_STYLE_HORIZONTAL_WINDOWS: //horizontal scroll like window style
    case GUIPRGBOX_STYLE_HORIZONTAL_PROCESS: //horizontal process
        GUIRES_GetImgWidthHeight (&pre_arrow_width, &pre_arrow_height, progressbar_ptr->hor_scroll_theme.right_arrow_id, progressbar_ptr->win_id);

        if (base_ctrl_ptr->rect.right - base_ctrl_ptr->rect.left + 1 < pre_arrow_width
                || base_ctrl_ptr->rect.bottom - base_ctrl_ptr->rect.top + 1 < pre_arrow_height)
        {
            is_true = FALSE;
        }

        break;
#ifdef GUIF_PRGBOX_LETTERSCROLL
    case GUIPRGBOX_STYLE_LETTERS_SCROLL:
        is_true = FALSE;
        break;
#endif
	case GUIPRGBOX_STYLE_VOLUME_BAR:
	case GUIPRGBOX_STYLE_BRIGHTNESS_BAR:
	case GUIPRGBOX_STYLE_NPS_SCORE_BAR:
        is_true = FALSE;
        break;
    default:
        is_true = TRUE;
        break;
    }


    return is_true;
}

/*****************************************************************************/
// 	Description : display vertical scroll box
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayVerticalScroll (
    CTRLPROGRESSBAR_OBJ_T   *progressbar_ptr
)
{
    BOOLEAN         result = TRUE;
    uint16          up_arrow_height = 0;
    uint16          down_arrow_height = 0;
    GUI_RECT_T      groove_lcd_rect = {0};
    GUI_RECT_T      groove_image_rect = {0};
    GUI_RECT_T      up_arrow_lcd_rect = {0};
    GUI_RECT_T      up_arrow_image_rect = {0};
    GUI_RECT_T      down_arrow_lcd_rect = {0};
    GUI_RECT_T      down_arrow_image_rect = {0};
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != progressbar_ptr); /*assert verified*/
    if (PNULL == progressbar_ptr)
    {
        return FALSE;
    }

    IMG_EnableTransparentColor (TRUE);
    //set groove image and lcd display rect
    groove_lcd_rect          = base_ctrl_ptr->rect;
    groove_image_rect.right  = (int16) (groove_lcd_rect.right - groove_lcd_rect.left);
    groove_image_rect.bottom = (int16) (groove_lcd_rect.bottom - groove_lcd_rect.top);

    if (progressbar_ptr->is_need_bg)
    {

        if (GUI_BG_IMG == progressbar_ptr->process_theme.bg_type)
        {
            //display scroll groove bar bg
            GUIRES_DisplayImg (PNULL,
                               &groove_lcd_rect,
                               &groove_image_rect,
                               progressbar_ptr->win_id,
                               progressbar_ptr->ver_scroll_theme.groove_bg_id,
                               &base_ctrl_ptr->lcd_dev_info);
        }
        else if (GUI_BG_COLOR == progressbar_ptr->process_theme.bg_type)
        {
            GUI_FillRect (&base_ctrl_ptr->lcd_dev_info, groove_lcd_rect, progressbar_ptr->process_theme.bg_color);
        }
        else
        {
            //SCI_TRACE_LOW:"DisplayVerticalScroll bg type = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIPRGBOX_972_112_2_18_3_20_39_253, (uint8*) "d", progressbar_ptr->process_theme.bg_type);
        }
    }

    if (!IsDisplayArrow (progressbar_ptr))
    {
        result = DisplayScrollBar (groove_lcd_rect, progressbar_ptr, TRUE);
        //set rect for touch panel
        progressbar_ptr->pre_groove_rect        = groove_lcd_rect;
        progressbar_ptr->pre_groove_rect.bottom = (int16) (progressbar_ptr->scroll_bar_rect.top - 1);
        progressbar_ptr->next_groove_rect       = groove_lcd_rect;
        progressbar_ptr->next_groove_rect.top   = (int16) (progressbar_ptr->scroll_bar_rect.bottom + 1);
    }
    else
    {
        //set scroll groove up arrow image and lcd display rect
        GUIRES_GetImgWidthHeight (PNULL, &up_arrow_height, progressbar_ptr->ver_scroll_theme.up_arrow_id, progressbar_ptr->win_id);
        up_arrow_lcd_rect        = groove_lcd_rect;
        up_arrow_lcd_rect.bottom = (int16) (groove_lcd_rect.top + up_arrow_height - 1);

        if (up_arrow_lcd_rect.bottom > up_arrow_lcd_rect.top)
        {
            up_arrow_image_rect.right  = groove_image_rect.right;
            up_arrow_image_rect.bottom = (int16) (up_arrow_height - 1);

            //display up arrow
            GUIRES_DisplayImg (PNULL,
                               &up_arrow_lcd_rect,
                               &up_arrow_image_rect,
                               progressbar_ptr->win_id,
                               progressbar_ptr->ver_scroll_theme.up_focus_arrow_id,
                               &base_ctrl_ptr->lcd_dev_info);

            //set scroll groove down arrow image and lcd display rect
            GUIRES_GetImgWidthHeight (PNULL, &down_arrow_height, progressbar_ptr->ver_scroll_theme.down_arrow_id, progressbar_ptr->win_id);
            down_arrow_lcd_rect     = groove_lcd_rect;
            down_arrow_lcd_rect.top = (int16) (groove_lcd_rect.bottom - down_arrow_height + 1);

            if (down_arrow_lcd_rect.bottom > down_arrow_lcd_rect.top)
            {
                down_arrow_image_rect.right  = groove_image_rect.right;
                down_arrow_image_rect.bottom = (int16) (down_arrow_height - 1);

                //display scroll groove down arrow
                GUIRES_DisplayImg (PNULL,
                                   &down_arrow_lcd_rect,
                                   &down_arrow_image_rect,
                                   progressbar_ptr->win_id,
                                   progressbar_ptr->ver_scroll_theme.down_focus_arrow_id,
                                   &base_ctrl_ptr->lcd_dev_info);

                //set rect for touch panel
                progressbar_ptr->pre_arrow_rect		= up_arrow_lcd_rect;
                progressbar_ptr->next_arrow_rect    = down_arrow_lcd_rect;

                //set scroll bar display rect
                groove_lcd_rect.top    = (int16) (groove_lcd_rect.top + up_arrow_height);
                groove_lcd_rect.bottom = (int16) (groove_lcd_rect.bottom - down_arrow_height);

                //display vertical scroll bar
                result = DisplayScrollBar (groove_lcd_rect, progressbar_ptr, TRUE);

                //set rect for touch panel
                progressbar_ptr->pre_groove_rect        = groove_lcd_rect;
                progressbar_ptr->pre_groove_rect.bottom = (int16) (progressbar_ptr->scroll_bar_rect.top - 1);
                progressbar_ptr->next_groove_rect       = groove_lcd_rect;
                progressbar_ptr->next_groove_rect.top   = (int16) (progressbar_ptr->scroll_bar_rect.bottom + 1);
            }
            else
            {
                result = FALSE;
            }
        }
        else
        {
            result = FALSE;
        }
    }

    IMG_EnableTransparentColor (FALSE);

    return (result);
}

/*****************************************************************************/
// 	Description : display scroll bar
//	Global resource dependence :
//  Author: James
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayScrollBar (
    GUI_RECT_T          rect,               //display rect
    CTRLPROGRESSBAR_OBJ_T    *progressbar_ptr,
    BOOLEAN             is_vertical
)
{
    BOOLEAN         result = TRUE;
    uint32          scroll_bar_len = 0;
    GUI_RECT_T      image_display_rect = {0};
    GUI_RECT_T      lcd_display_rect = rect;
    MMI_IMAGE_ID_T  scroll_bar_id = 0;
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != progressbar_ptr); /*assert verified*/
    if (PNULL == progressbar_ptr)
    {
        return FALSE;
    }

    if (progressbar_ptr->is_need_slide_block && ( (rect.right > rect.left) &&
            (rect.bottom > rect.top) && (0 != progressbar_ptr->total_item_num)))
    {
        if (is_vertical)
        {
            //set scroll bar image id
            scroll_bar_id = progressbar_ptr->ver_scroll_theme.scroll_bar_id;

            //calculate scroll bar display rect
            scroll_bar_len = GetScrollBarLength (
                                 progressbar_ptr,
                                 progressbar_ptr->cur_item_index,
                                 progressbar_ptr->first_item_index
                             );

            lcd_display_rect = GetScrollBarRect (
                                   progressbar_ptr,
                                   progressbar_ptr->cur_item_index,
                                   progressbar_ptr->first_item_index
                               );

            //set scroll bar image display rect
            image_display_rect.right  = (int16) (rect.right - rect.left);
            image_display_rect.bottom = (int16) (scroll_bar_len - 1);
        }
        else
        {
            //set scroll bar image id
            scroll_bar_id = progressbar_ptr->hor_scroll_theme.scroll_bar_id;

            scroll_bar_len = GetScrollBarLength (
                                 progressbar_ptr,
                                 progressbar_ptr->cur_item_index,
                                 progressbar_ptr->first_item_index
                             );

            lcd_display_rect = GetScrollBarRect (
                                   progressbar_ptr,
                                   progressbar_ptr->cur_item_index,
                                   progressbar_ptr->first_item_index
                               );

            //set scroll bar image display rect
            image_display_rect.right  = (int16) (scroll_bar_len - 1);
            image_display_rect.bottom = (int16) (rect.bottom - rect.top);
        }

        //display scroll bar
#if defined(GUIF_PRGBOX_AUTOHIDE)

        if (GUIPRGBOX_STYLE_VERTICAL_WINDOWS == progressbar_ptr->prgbox_style)
        {
            GUIRES_DisplayImg (PNULL,
                               &lcd_display_rect,
                               &image_display_rect,
                               progressbar_ptr->win_id,
                               scroll_bar_id,
                               &progressbar_ptr->disp_layer);
        }
        else
        {
            GUIRES_DisplayImg (PNULL,
                               &lcd_display_rect,
                               &image_display_rect,
                               progressbar_ptr->win_id,
                               scroll_bar_id,
                               &base_ctrl_ptr->lcd_dev_info);
        }

#else
        GUIRES_DisplayImg (PNULL,
                           &lcd_display_rect,
                           &image_display_rect,
                           progressbar_ptr->win_id,
                           scroll_bar_id,
                           &base_ctrl_ptr->lcd_dev_info);
#endif

        //set rect for touch panel
        progressbar_ptr->scroll_bar_rect = lcd_display_rect;
    }
    else
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : display horizontal scroll box
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayHorizontalScroll (
    CTRLPROGRESSBAR_OBJ_T  *progressbar_ptr
)
{
    BOOLEAN         result = TRUE;
    uint16          left_arrow_width = 0;
    uint16          right_arrow_width = 0;
    GUI_RECT_T      groove_lcd_rect = {0};
    GUI_RECT_T      groove_image_rect = {0};
    GUI_RECT_T      left_arrow_lcd_rect = {0};
    GUI_RECT_T      left_arrow_image_rect = {0};
    GUI_RECT_T      right_arrow_lcd_rect = {0};
    GUI_RECT_T      right_arrow_image_rect = {0};
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != progressbar_ptr); /*assert verified*/
    if (PNULL == progressbar_ptr)
    {
        return FALSE;
    }

    IMG_EnableTransparentColor (TRUE);
    //set groove image and lcd display rect
    groove_lcd_rect          = base_ctrl_ptr->rect;
    groove_image_rect.right  = (int16) (groove_lcd_rect.right - groove_lcd_rect.left);
    groove_image_rect.bottom = (int16) (groove_lcd_rect.bottom - groove_lcd_rect.top);

    if (progressbar_ptr->is_need_bg)
    {
        //display scroll groove bar bg
        GUIRES_DisplayImg (PNULL,
                           &groove_lcd_rect,
                           &groove_image_rect,
                           progressbar_ptr->win_id,
                           progressbar_ptr->hor_scroll_theme.groove_bg_id,
                           &base_ctrl_ptr->lcd_dev_info);
    }

    //set scroll groove left arrow image and lcd display rect
    GUIRES_GetImgWidthHeight (&left_arrow_width, PNULL, progressbar_ptr->hor_scroll_theme.left_arrow_id, progressbar_ptr->win_id);
    left_arrow_lcd_rect       = groove_lcd_rect;
    left_arrow_lcd_rect.right = (int16) (groove_lcd_rect.left + left_arrow_width - 1);

    if (left_arrow_lcd_rect.right > left_arrow_lcd_rect.left)
    {
        left_arrow_image_rect.right  = (int16) (left_arrow_width - 1);
        left_arrow_image_rect.bottom = groove_image_rect.bottom;

        //display left arrow
        GUIRES_DisplayImg (PNULL,
                           &left_arrow_lcd_rect,
                           &left_arrow_image_rect,
                           progressbar_ptr->win_id,
                           progressbar_ptr->hor_scroll_theme.left_arrow_id,
                           &base_ctrl_ptr->lcd_dev_info);

        //set scroll groove right arrow image and lcd display rect
        GUIRES_GetImgWidthHeight (&right_arrow_width, PNULL, progressbar_ptr->hor_scroll_theme.right_arrow_id, progressbar_ptr->win_id);
        right_arrow_lcd_rect     = groove_lcd_rect;
        right_arrow_lcd_rect.left = (int16) (groove_lcd_rect.right - right_arrow_width + 1);

        if (right_arrow_lcd_rect.right > right_arrow_lcd_rect.left)
        {
            right_arrow_image_rect.right  = (int16) (right_arrow_width - 1);
            right_arrow_image_rect.bottom = groove_image_rect.bottom;

            //display scroll groove down arrow
            GUIRES_DisplayImg (PNULL,
                               &right_arrow_lcd_rect,
                               &right_arrow_image_rect,
                               progressbar_ptr->win_id,
                               progressbar_ptr->hor_scroll_theme.right_arrow_id,
                               &base_ctrl_ptr->lcd_dev_info);

            //set scroll bar display rect
            groove_lcd_rect.left  = (int16) (groove_lcd_rect.left + left_arrow_width);
            groove_lcd_rect.right = (int16) (groove_lcd_rect.right - right_arrow_width);

            //display horizontal scroll bar
            result = DisplayScrollBar (groove_lcd_rect, progressbar_ptr, FALSE);

            //set rect for touch panel
            progressbar_ptr->pre_arrow_rect        = left_arrow_lcd_rect;
            progressbar_ptr->next_arrow_rect       = right_arrow_lcd_rect;
            progressbar_ptr->pre_groove_rect       = groove_lcd_rect;
            progressbar_ptr->pre_groove_rect.right = (int16) (progressbar_ptr->scroll_bar_rect.left - 1);
            progressbar_ptr->next_groove_rect      = groove_lcd_rect;
            progressbar_ptr->next_groove_rect.left = (int16) (progressbar_ptr->scroll_bar_rect.right + 1);
        }
        else
        {
            result = FALSE;
        }
    }
    else
    {
        result = FALSE;
    }

    IMG_EnableTransparentColor (FALSE);

    return (result);
}

#ifdef GUIF_PRGBOX_LETTERSCROLL
/*****************************************************************************/
// 	Description : display horizontal scroll box
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayLettersScroll (
    CTRLPROGRESSBAR_OBJ_T  *progressbar_ptr
)
{
    BOOLEAN         result = TRUE;
    uint16          i = 0;
    uint32          letter_count = 0;
    uint16          scale = 0;
    uint16          font_height = 0;
    uint16          font_width = GUI_GetFontWidth (GUIPRGBOX_LETTER_FONT, 'A');
    uint16          all_space_height = 0;
    uint16          one_space_height = 0;
    uint16          unicode = 0;
    uint16          bg_width = 0;
    uint16          bg_height = 0;
    GUISTR_STYLE_T   text_style      = {0};
    uint16          compensation = 0;
    GUI_RECT_T      text_rect = {0};
    GUI_RECT_T      scoll_rect = {0};
    //GUI_BG_T        bg_info = {0};
    MMI_STRING_T    string = {0};
    wchar           str_buf[2] = {0};
    UILAYER_APPEND_BLT_T append_layer = {0};
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != progressbar_ptr); /*assert verified*/
    if (PNULL == progressbar_ptr)
    {
        return FALSE;
    }

    IMG_EnableTransparentColor (TRUE);

    //draw letters  bar
    font_height = GUIFONT_GetHeight (GUIPRGBOX_LETTER_FONT);
    text_style.align = ALIGN_HVMIDDLE;

    if (MMK_IsWindowLandscape (progressbar_ptr->win_id))
    {
        //竖屏
        text_rect = base_ctrl_ptr->both_rect.h_rect;
    }
    else
    {
        text_rect = base_ctrl_ptr->both_rect.v_rect;
    }

    if (progressbar_ptr->total_item_num < progressbar_ptr->letters_table_len)
    {
        //缩略显示，字母中间要用一个符号间隔
        letter_count = (progressbar_ptr->total_item_num << 1) - 1;
        scale = PRGBOX_LETTER_SCALE;
    }
    else
    {
        //全字母显示，字母中间不需要用符号间隔
        letter_count = progressbar_ptr->total_item_num;
        scale = 1;
    }

    if ( (text_rect.bottom - text_rect.top) > (font_height * letter_count)) /*lint !e574 !e737*/
    {

        progressbar_ptr->scroll_bar_rect = text_rect;
        base_ctrl_ptr->rect = text_rect;
        base_ctrl_ptr->display_rect  = text_rect;
        //CreateDispLayer(progressbar_ptr);
#ifdef UI_MULTILAYER_SUPPORT
        //disp_layer每次都会重画，使用双缓冲切换的方式
        UILAYER_SwitchDoubleMem (&progressbar_ptr->disp_layer, TRUE);

        //ClearDispInfo(progressbar_ptr);
        UILAYER_Clear (& (progressbar_ptr->disp_layer));

        append_layer.lcd_dev_info = progressbar_ptr->disp_layer;
        append_layer.layer_level = UILAYER_LEVEL_HIGH;

        UILAYER_AppendBltLayer (&append_layer);
#endif
        //text_rect.right = text_rect.right -2;
        text_rect.top = text_rect.top + 2;
        text_rect.bottom = text_rect.bottom - 2;
        //bg_info = MMITHEME_GetCommonBg();


        //draw select hight light
        if (progressbar_ptr->is_need_bg)
        {
            GUIRES_GetImgWidthHeight (&bg_width, &bg_height, progressbar_ptr->ver_scroll_theme.groove_bg_id, progressbar_ptr->win_id);
            scoll_rect = text_rect;

            if (scoll_rect.right > scoll_rect.left + bg_width)
            {
                scoll_rect.left = scoll_rect.left + ( (scoll_rect.right - scoll_rect.left - bg_width) >> 1);
                scoll_rect.right = scoll_rect.left + bg_width;
            }

            GUI_SetAlphaOverwrite (TRUE);
            GUIRES_DisplayImg (PNULL,
                               &scoll_rect,
                               PNULL,
                               progressbar_ptr->win_id,
                               progressbar_ptr->ver_scroll_theme.groove_bg_id,
                               &progressbar_ptr->disp_layer);
            GUI_SetAlphaOverwrite (FALSE);
        }

        if (progressbar_ptr->total_item_num < progressbar_ptr->letters_table_len)
        {
            //缩略显示，字母中间要用一个符号间隔
            letter_count = (progressbar_ptr->total_item_num << 1) - 1;
            scale = PRGBOX_LETTER_SCALE;
        }
        else
        {
            //全字母显示，字母中间不需要用符号间隔
            letter_count = progressbar_ptr->total_item_num;
            scale = 1;
        }

        all_space_height = (text_rect.bottom - text_rect.top) - (font_height * letter_count);   /*lint !e737*/

        if (letter_count > 1)
        {
            one_space_height = (all_space_height) / (letter_count - 1);//间隔
            compensation = all_space_height - one_space_height * (letter_count - 1);//像素补偿值
        }

        text_style.font = GUIPRGBOX_LETTER_FONT;
        string.wstr_ptr = str_buf;
        string.wstr_len = 1;

        for (i = 0; i < progressbar_ptr->total_item_num; i++)
        {
            if (i * scale > progressbar_ptr->letters_table_len)
            {
                unicode = progressbar_ptr->base_letters_table[progressbar_ptr->letters_table_len -1];
            }
            else
            {
                unicode = progressbar_ptr->base_letters_table[i * scale];
            }

            if (i > 0)
            {
                if (compensation > 0)
                {
                    compensation--;
                    text_rect.top = text_rect.top + one_space_height + font_height + 1;
                }
                else
                {
                    text_rect.top = text_rect.top + one_space_height + font_height;
                }
            }

            progressbar_ptr->letter_postion_table[i] = text_rect.top;

            text_rect.bottom = text_rect.top + font_height;

            //draw letters
            string.wstr_ptr[0] = unicode;
            GUISTR_DrawTextToLCDInRect (
                &progressbar_ptr->disp_layer,
                &text_rect,       //the fixed display area
                &text_rect,       //用户要剪切的实际区域
                &string,
                &text_style,
                GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN,
                GUISTR_TEXT_DIR_AUTO
            );

            if (scale > 1)
            {
                //draw mark
                if (compensation > 0)
                {
                    compensation--;
                    text_rect.top = text_rect.top + one_space_height + font_height + 1;
                }
                else
                {
                    text_rect.top = text_rect.top + one_space_height + font_height;
                }

                text_rect.bottom = text_rect.top + font_height;

                //string.wstr_ptr[0] = 29;
                if (text_rect.bottom <= progressbar_ptr->scroll_bar_rect.bottom)
                {
                    int16 x = 0;
                    int16 y = 0;
                    uint16 radius = 0;

                    x = text_rect.left + ( (text_rect.right - text_rect.left) >> 1);
                    y = text_rect.top + (font_height >> 1);
                    radius = MIN ( (text_rect.right - text_rect.left), font_width);
                    radius = radius >> 1;
                    LCD_FillCircle (&progressbar_ptr->disp_layer, &text_rect, x, y, radius, 0);
                    //GUI_FillRect(&progressbar_ptr->disp_layer, text_rect, 0);
                }

//		   GUISTR_DrawTextToLCDInRect(
//			   &progressbar_ptr->disp_layer,
//			   &text_rect,       //the fixed display area
//			   &text_rect,       //用户要剪切的实际区域
//			   &string,
//			   &text_style,
//			   GUISTR_STATE_SINGLE_LINE,
//			   GUISTR_TEXT_DIR_AUTO
// 			   );
            }
        }


        IMG_EnableTransparentColor (FALSE);
    }
    else
    {
        progressbar_ptr->is_letter_bar_valid = FALSE;
    }

    return (result);
}
#endif

/*****************************************************************************/
//  Description : display vertical or horizontal process box
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayProcess (
    GUIPRGBOX_STYLE_E      style,  //style
    CTRLPROGRESSBAR_OBJ_T       *progressbar_ptr
)
{
    BOOLEAN         result = TRUE;
    GUI_RECT_T      fg_rect = {0};
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    GUIRES_DisplayImg (NULL,
                       &base_ctrl_ptr->rect,
                       NULL,
                       progressbar_ptr->win_id,
                       progressbar_ptr->process_theme.bg_img_id,
                       (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info);

    //set foreground rect
    fg_rect = base_ctrl_ptr->rect;

    switch (style)
    {
    case GUIPRGBOX_STYLE_VERTICAL_PROCESS:

        //from bottom to top
        if ( (progressbar_ptr->cur_item_index + 1) != progressbar_ptr->total_item_num)
        {
            fg_rect.top = (int16) (fg_rect.bottom - (fg_rect.bottom - fg_rect.top) * progressbar_ptr->cur_item_index / progressbar_ptr->total_item_num);    /*lint !e737*/
        }

        break;

    case GUIPRGBOX_STYLE_HORIZONTAL_PROCESS:

        //from left to right
        if ( (progressbar_ptr->cur_item_index + 1) != progressbar_ptr->total_item_num)
        {
            fg_rect.right = (int16) (fg_rect.left + (fg_rect.right - fg_rect.left) * progressbar_ptr->cur_item_index / progressbar_ptr->total_item_num);    /*lint !e737*/
        }

        break;

    default:
        SCI_PASSERT (FALSE, ("DisplayProcess: style %d is error!", style)); /*assert verified*/
        break;
    }

    //fill foreground color
    if (0 != progressbar_ptr->cur_item_index)
    {
//         GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
//                      fg_rect,
//                      progressbar_ptr->process_theme.fg_color);
        GUIRES_DisplayImg (NULL,
                           &fg_rect,
                           NULL,
                           progressbar_ptr->win_id,
                           progressbar_ptr->process_theme.fg_img_id,
                           (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info);
    }

    return (result);
}

/*****************************************************************************/
//  Description : display vertical or horizontal process box
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayVolumeBar (
                                CTRLPROGRESSBAR_OBJ_T       *progressbar_ptr
                                )
{
    BOOLEAN         result = TRUE;
    GUI_RECT_T      fg_rect = {0};
    GUI_RECT_T      other_rect = {0};
    GUI_RECT_T      text_rect = {0};
    GUISTR_STATE_T  str_state = GUISTR_STATE_NONE;
    GUISTR_STYLE_T  str_style = {0};
    MMI_STRING_T    str_info = {0};
    uint8 data_buf[3] = {0};
    wchar wstr[3] = {0};
    int32 buff_length = 3;
    char* str = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;
    
    GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
        base_ctrl_ptr->rect,
        progressbar_ptr->process_theme.bg_rect_color);
    
    text_rect.left = progressbar_ptr->label_rect.left;
    text_rect.top = base_ctrl_ptr->rect.top + progressbar_ptr->label_rect.top;
    text_rect.right = progressbar_ptr->label_rect.right;
    text_rect.bottom = base_ctrl_ptr->rect.top + progressbar_ptr->label_rect.bottom;
    str_style.font = progressbar_ptr->process_theme.text_font;
    
    //set foreground rect
    fg_rect.left = progressbar_ptr->fg_rect.left;
    fg_rect.top = base_ctrl_ptr->rect.top + progressbar_ptr->fg_rect.top;
    fg_rect.right = progressbar_ptr->fg_rect.right;
    fg_rect.bottom = base_ctrl_ptr->rect.top + progressbar_ptr->fg_rect.bottom;
    
    GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,fg_rect,
        progressbar_ptr->process_theme.bg_color);
    
    //from left to right
    if ( progressbar_ptr->cur_item_index < progressbar_ptr->total_item_num)
    {
        if(progressbar_ptr->is_active)
        {
            str_style.font_color = progressbar_ptr->process_theme.label_text_focus_color;
            str_style.align = ALIGN_LEFT;
            str_style.font = progressbar_ptr->process_theme.title_font;
            MMITHEME_GetResText(progressbar_ptr->process_theme.text1_id, progressbar_ptr->base_ctrl.win_handle, &str_info);
            GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                (const GUI_RECT_T *)&text_rect,
                (const GUI_RECT_T *)&text_rect,
                (const MMI_STRING_T *)&str_info,
                &str_style,
                str_state,
                GUISTR_TEXT_DIR_AUTO);
            
            if(progressbar_ptr->cur_item_index ==0)
            {
                GUI_RECT_T      border_rect = {0};
                
                border_rect = fg_rect;
                border_rect.bottom = fg_rect.top + VOLUME_BAR_ZERO_BORDER - 1;
                GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    border_rect,
                    progressbar_ptr->process_theme.fg_color);
                
                border_rect = fg_rect;
                border_rect.top = fg_rect.bottom - VOLUME_BAR_ZERO_BORDER + 1;
                GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    border_rect,
                    progressbar_ptr->process_theme.fg_color);
                
                border_rect = fg_rect;
                border_rect.right = fg_rect.left + VOLUME_BAR_ZERO_BORDER - 1;
                GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    border_rect,
                    progressbar_ptr->process_theme.fg_color);
                
                border_rect = fg_rect;
                border_rect.left = fg_rect.right - VOLUME_BAR_ZERO_BORDER + 1;
                GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    border_rect,
                    progressbar_ptr->process_theme.fg_color);
                
                text_rect.left = fg_rect.left;
                text_rect.right = fg_rect.left + VOLUME_BRIGHTNESS_START_LENGTH - 1;
                text_rect.top = fg_rect.top + VOLUME_BRIGHTNESS_BG_TOP_MARGIN;
                text_rect.bottom = fg_rect.bottom - VOLUME_BRIGHTNESS_BG_BOTTOM_MARGIN;
                str_style.font_color = progressbar_ptr->process_theme.text_color;
                str_style.align = ALIGN_HVMIDDLE;
                sprintf((char*)data_buf, "%ld", 0);
                str = (char*)data_buf;
                str_info.wstr_len = strlen(str);
                str_info.wstr_ptr = wstr;
                str_style.font = progressbar_ptr->process_theme.text_font;
                MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, str_info.wstr_len, str_info.wstr_len );
                GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    (const GUI_RECT_T *)&text_rect,
                    (const GUI_RECT_T *)&text_rect,
                    (const MMI_STRING_T *)&str_info,
                    &str_style,
                    str_state,
                    GUISTR_TEXT_DIR_AUTO);
                
                text_rect.right = fg_rect.right - VOLUME_BRIGHTNESS_BAR_MARGIN;
                text_rect.left = text_rect.right - VOLUME_BRIGHTNESS_TEXT_LENGTH*3 + 1;
                str_style.align = ALIGN_RVMIDDLE;
                MMITHEME_GetResText(progressbar_ptr->process_theme.text2_id, progressbar_ptr->base_ctrl.win_handle, &str_info);
                GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    (const GUI_RECT_T *)&text_rect,
                    (const GUI_RECT_T *)&text_rect,
                    (const MMI_STRING_T *)&str_info,
                    &str_style,
                    str_state,
                    GUISTR_TEXT_DIR_AUTO);
            }
            else if(progressbar_ptr->cur_item_index ==1)
            {
                wchar point = (wchar)0x25CF;
                
                fg_rect.right = VOLUME_BRIGHTNESS_START_LENGTH - 1;
                GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    fg_rect,
                    progressbar_ptr->process_theme.fg_color);
                
                other_rect = fg_rect;
                other_rect.left = fg_rect.right + 1;
                other_rect.right = progressbar_ptr->fg_rect.right;
                GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    other_rect,
                    progressbar_ptr->process_theme.focus_other_color);
                
                text_rect = fg_rect;
                text_rect.top = fg_rect.top + VOLUME_BRIGHTNESS_BG_TOP_MARGIN;
                text_rect.bottom = fg_rect.bottom - VOLUME_BRIGHTNESS_BG_BOTTOM_MARGIN;
                str_style.font_color = progressbar_ptr->process_theme.text_color;
                str_style.align = ALIGN_HVMIDDLE;
                str_style.font = progressbar_ptr->process_theme.text_font;
                str_info.wstr_len = 1;
                str_info.wstr_ptr = &point; /*lint !e733*/
                GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    (const GUI_RECT_T *)&text_rect,
                    (const GUI_RECT_T *)&text_rect,
                    (const MMI_STRING_T *)&str_info,
                    &str_style,
                    str_state,
                    GUISTR_TEXT_DIR_AUTO);
                
                text_rect.right = progressbar_ptr->fg_rect.right - VOLUME_BRIGHTNESS_BAR_MARGIN;
                text_rect.left = text_rect.right - VOLUME_BRIGHTNESS_TEXT_LENGTH*3 + 1;
                str_style.align = ALIGN_RVMIDDLE;
                MMITHEME_GetResText(progressbar_ptr->process_theme.text3_id, progressbar_ptr->base_ctrl.win_handle, &str_info);
                GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    (const GUI_RECT_T *)&text_rect,
                    (const GUI_RECT_T *)&text_rect,
                    (const MMI_STRING_T *)&str_info,
                    &str_style,
                    str_state,
                    GUISTR_TEXT_DIR_AUTO);
            }
            else
            {
                fg_rect.right = (int16) (VOLUME_BRIGHTNESS_START_LENGTH + (progressbar_ptr->cur_item_index - 2) * VOLUME_BRIGHTNESS_RAISE_LENGTH - 1);    /*lint !e737*/
                GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    fg_rect,
                    progressbar_ptr->process_theme.fg_color);
                
                other_rect = fg_rect;
                other_rect.left = fg_rect.right + 1;
                other_rect.right = progressbar_ptr->fg_rect.right;
                GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    other_rect,
                    progressbar_ptr->process_theme.focus_other_color);
                
                text_rect.left = fg_rect.left;
                text_rect.right = fg_rect.left + VOLUME_BRIGHTNESS_START_LENGTH - 1;
                text_rect.top = fg_rect.top + VOLUME_BRIGHTNESS_BG_TOP_MARGIN;
                text_rect.bottom = fg_rect.bottom - VOLUME_BRIGHTNESS_BG_BOTTOM_MARGIN;
                
                str_style.font_color = progressbar_ptr->process_theme.text_color;
                str_style.align = ALIGN_HVMIDDLE;
                str_style.font = progressbar_ptr->process_theme.text_font;
                sprintf((char*)data_buf, "%ld", progressbar_ptr->cur_item_index - 1);
                str = (char*)data_buf;
                str_info.wstr_len = strlen(str);
                str_info.wstr_ptr = wstr;
                MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, str_info.wstr_len, str_info.wstr_len );
                GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    (const GUI_RECT_T *)&text_rect,
                    (const GUI_RECT_T *)&text_rect,
                    (const MMI_STRING_T *)&str_info,
                    &str_style,
                    str_state,
                    GUISTR_TEXT_DIR_AUTO);
            }
        }
        else
        {
			MMI_DIRECTION_SET_E dir = MMITHEME_GetDisDirection();
#if 0
            GUI_POINT_T     start_point = {0};
            
            start_point.x = text_rect.left;
            start_point.y = text_rect.top;
#endif
            str_style.font_color = progressbar_ptr->process_theme.text_color;
            str_style.align = (MMI_DIRECTION_LEFT == dir) ? ALIGN_LEFT : ALIGN_RIGHT;
            str_style.begin_alpha = str_style.end_alpha = (uint8)(255*0.6);
            str_style.font = progressbar_ptr->process_theme.title_font;
            str_state = GUISTR_STATE_EFFECT | GUISTR_STATE_GRADUAL_HOR;
            MMITHEME_GetResText(progressbar_ptr->process_theme.text1_id, progressbar_ptr->base_ctrl.win_handle, &str_info);
#if 0
            GUISTR_DrawTextToLCDSingleLine((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                (const GUI_POINT_T *)&start_point, 
                (const MMI_STRING_T *)&str_info, 
                &str_style, 
                str_state);
#endif
            GUISTR_DrawSingleLineGradualInRect(
                (const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info, 
                (const GUI_RECT_T *)&text_rect,
                (const MMI_STRING_T *)&str_info, 
                &str_style, 
                str_state);
            
            str_state = GUISTR_STATE_NONE;
            
            if(progressbar_ptr->cur_item_index ==0)
            {
                text_rect.left = fg_rect.left;
                text_rect.right = fg_rect.left + VOLUME_BRIGHTNESS_START_LENGTH - 1;
                text_rect.top = fg_rect.top + VOLUME_BRIGHTNESS_BG_TOP_MARGIN;
                text_rect.bottom = fg_rect.bottom - VOLUME_BRIGHTNESS_BG_BOTTOM_MARGIN;
                
                str_style.align = ALIGN_HVMIDDLE;
                str_style.font = progressbar_ptr->process_theme.text_font;
                sprintf((char*)data_buf, "%ld", 0);
                str = (char*)data_buf;
                str_info.wstr_len = strlen(str);
                str_info.wstr_ptr = wstr;
                MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, str_info.wstr_len, str_info.wstr_len );
                GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    (const GUI_RECT_T *)&text_rect,
                    (const GUI_RECT_T *)&text_rect,
                    (const MMI_STRING_T *)&str_info,
                    &str_style,
                    str_state,
                    GUISTR_TEXT_DIR_AUTO);
                
                text_rect.right = fg_rect.right - VOLUME_BRIGHTNESS_BAR_MARGIN;
                text_rect.left = text_rect.right - VOLUME_BRIGHTNESS_TEXT_LENGTH*3 + 1;
                str_style.align = ALIGN_RVMIDDLE;
                MMITHEME_GetResText(progressbar_ptr->process_theme.text2_id, progressbar_ptr->base_ctrl.win_handle, &str_info);
                GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    (const GUI_RECT_T *)&text_rect,
                    (const GUI_RECT_T *)&text_rect,
                    (const MMI_STRING_T *)&str_info,
                    &str_style,
                    str_state,
                    GUISTR_TEXT_DIR_AUTO);
            }
            else if(progressbar_ptr->cur_item_index ==1)
            {
                wchar point = (wchar)0x25CF;
                
                fg_rect.right = VOLUME_BRIGHTNESS_START_LENGTH - 1;    /*lint !e737*/
                GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    fg_rect,
                    progressbar_ptr->process_theme.unfocus_fg_color);
                
                text_rect = fg_rect;
                text_rect.top = fg_rect.top + VOLUME_BRIGHTNESS_BG_TOP_MARGIN;
                text_rect.bottom = fg_rect.bottom - VOLUME_BRIGHTNESS_BG_BOTTOM_MARGIN;
                
                str_style.align = ALIGN_HVMIDDLE;
                str_style.font = progressbar_ptr->process_theme.text_font;
                str_info.wstr_len = 1;
                str_info.wstr_ptr = &point; /*lint !e733*/
                GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    (const GUI_RECT_T *)&text_rect,
                    (const GUI_RECT_T *)&text_rect,
                    (const MMI_STRING_T *)&str_info,
                    &str_style,
                    str_state,
                    GUISTR_TEXT_DIR_AUTO);
                
                text_rect.right = progressbar_ptr->fg_rect.right - VOLUME_BRIGHTNESS_BAR_MARGIN;
                text_rect.left = text_rect.right - VOLUME_BRIGHTNESS_TEXT_LENGTH*3 + 1;
                str_style.align = ALIGN_RVMIDDLE;
                MMITHEME_GetResText(progressbar_ptr->process_theme.text3_id, progressbar_ptr->base_ctrl.win_handle, &str_info);
                GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    (const GUI_RECT_T *)&text_rect,
                    (const GUI_RECT_T *)&text_rect,
                    (const MMI_STRING_T *)&str_info,
                    &str_style,
                    str_state,
                    GUISTR_TEXT_DIR_AUTO);
            }
            else
            {
                fg_rect.right = (int16) (VOLUME_BRIGHTNESS_START_LENGTH + (progressbar_ptr->cur_item_index - 2) * VOLUME_BRIGHTNESS_RAISE_LENGTH - 1);    /*lint !e737*/
                GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    fg_rect,
                    progressbar_ptr->process_theme.unfocus_fg_color);
                
                text_rect.left = fg_rect.left;
                text_rect.right = fg_rect.left + VOLUME_BRIGHTNESS_START_LENGTH - 1;
                text_rect.top = fg_rect.top + VOLUME_BRIGHTNESS_BG_TOP_MARGIN;
                text_rect.bottom = fg_rect.bottom - VOLUME_BRIGHTNESS_BG_BOTTOM_MARGIN;
                
                str_style.align = ALIGN_HVMIDDLE;
                str_style.font = progressbar_ptr->process_theme.text_font;
                sprintf((char*)data_buf, "%ld", progressbar_ptr->cur_item_index - 1);
                str = (char*)data_buf;
                str_info.wstr_len = strlen(str);
                str_info.wstr_ptr = wstr;
                MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, str_info.wstr_len, str_info.wstr_len );
                GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    (const GUI_RECT_T *)&text_rect,
                    (const GUI_RECT_T *)&text_rect,
                    (const MMI_STRING_T *)&str_info,
                    &str_style,
                    str_state,
                    GUISTR_TEXT_DIR_AUTO);
            }
        }
    }	
    
    return (result);
}

/*****************************************************************************/
//  Description : display KeyPad Volume
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayKeyPadVolumeBar (
									  CTRLPROGRESSBAR_OBJ_T     
									  *progressbar_ptr
)
{
	BOOLEAN         result = TRUE;
    GUI_RECT_T      fg_rect = {0};
	GUI_RECT_T      other_rect = {0};
	GUI_RECT_T      text_rect = {0};
	GUISTR_STATE_T  str_state = GUISTR_STATE_NONE;
	GUISTR_STYLE_T  str_style = {0};
	MMI_STRING_T    str_info = {0};
	uint8 data_buf[3] = {0};
    wchar wstr[3] = {0};
    int32 buff_length = 3;
    char* str = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

	GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
				base_ctrl_ptr->rect,
				progressbar_ptr->process_theme.bg_rect_color);

	text_rect.left = progressbar_ptr->label_rect.left;
	text_rect.top = base_ctrl_ptr->rect.top + progressbar_ptr->label_rect.top;
	text_rect.right = progressbar_ptr->label_rect.right;
	text_rect.bottom = base_ctrl_ptr->rect.top + progressbar_ptr->label_rect.bottom;
	str_style.font = progressbar_ptr->process_theme.text_font;
	
    //set foreground rect
    fg_rect.left = progressbar_ptr->fg_rect.left;
	fg_rect.top = base_ctrl_ptr->rect.top + progressbar_ptr->fg_rect.top;
	fg_rect.right = progressbar_ptr->fg_rect.right;
	fg_rect.bottom = base_ctrl_ptr->rect.top + progressbar_ptr->fg_rect.bottom;

	GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,fg_rect,
				progressbar_ptr->process_theme.bg_color);

	//from left to right
    if ( progressbar_ptr->cur_item_index < progressbar_ptr->total_item_num)
    {
    	if(progressbar_ptr->is_active)
		{
			str_style.font_color = progressbar_ptr->process_theme.label_text_focus_color;
			str_style.align = ALIGN_LEFT;
			MMITHEME_GetResText(progressbar_ptr->process_theme.text1_id, progressbar_ptr->base_ctrl.win_handle, &str_info);
 			GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
		                               (const GUI_RECT_T *)&text_rect,
		                               (const GUI_RECT_T *)&text_rect,
		                               (const MMI_STRING_T *)&str_info,
		                               &str_style,
		                               str_state,
		                               GUISTR_TEXT_DIR_AUTO);
		
			if(progressbar_ptr->cur_item_index ==0)
	    	{
	    		GUI_RECT_T      border_rect = {0};

				border_rect = fg_rect;
				border_rect.bottom = fg_rect.top + VOLUME_BAR_ZERO_BORDER - 1;
				GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
	                      border_rect,
	                      progressbar_ptr->process_theme.fg_color);
				
				border_rect = fg_rect;
				border_rect.top = fg_rect.bottom - VOLUME_BAR_ZERO_BORDER + 1;
				GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
	                      border_rect,
	                      progressbar_ptr->process_theme.fg_color);

				border_rect = fg_rect;
				border_rect.right = fg_rect.left + VOLUME_BAR_ZERO_BORDER - 1;
				GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
	                      border_rect,
	                      progressbar_ptr->process_theme.fg_color);

				border_rect = fg_rect;
				border_rect.left = fg_rect.right - VOLUME_BAR_ZERO_BORDER + 1;
				GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
	                      border_rect,
	                      progressbar_ptr->process_theme.fg_color);

				text_rect.left = fg_rect.left;
				text_rect.right = fg_rect.left + VOLUME_BRIGHTNESS_START_LENGTH - 1;
				text_rect.top = fg_rect.top + VOLUME_BRIGHTNESS_BG_TOP_MARGIN;
				text_rect.bottom = fg_rect.bottom - VOLUME_BRIGHTNESS_BG_BOTTOM_MARGIN;
				str_style.font_color = progressbar_ptr->process_theme.text_color;
				str_style.align = ALIGN_HVMIDDLE;
				sprintf((char*)data_buf, "%ld", 0);
	            str = (char*)data_buf;
	            str_info.wstr_len = strlen(str);
	            str_info.wstr_ptr = wstr;
	            MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, str_info.wstr_len, str_info.wstr_len );
				GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
		                               (const GUI_RECT_T *)&text_rect,
		                               (const GUI_RECT_T *)&text_rect,
		                               (const MMI_STRING_T *)&str_info,
		                               &str_style,
		                               str_state,
		                               GUISTR_TEXT_DIR_AUTO);
				
				text_rect.right = fg_rect.right - VOLUME_BRIGHTNESS_BAR_MARGIN;
				text_rect.left = text_rect.right - VOLUME_BRIGHTNESS_TEXT_LENGTH*3 + 1;
				str_style.align = ALIGN_RVMIDDLE;
				MMITHEME_GetResText(progressbar_ptr->process_theme.text2_id, progressbar_ptr->base_ctrl.win_handle, &str_info);
				GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
		                               (const GUI_RECT_T *)&text_rect,
		                               (const GUI_RECT_T *)&text_rect,
		                               (const MMI_STRING_T *)&str_info,
		                               &str_style,
		                               str_state,
		                               GUISTR_TEXT_DIR_AUTO);
			}
			else
	    	{
	    		fg_rect.right = (int16) (VOLUME_BRIGHTNESS_START_LENGTH + (progressbar_ptr->cur_item_index - 1) * VOLUME_BRIGHTNESS_RAISE_LENGTH - 1);    /*lint !e737*/
				GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
	                      fg_rect,
	                      progressbar_ptr->process_theme.fg_color);

				other_rect = fg_rect;
				other_rect.left = fg_rect.right + 1;
				other_rect.right = progressbar_ptr->fg_rect.right;
				GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
	                      other_rect,
	                      progressbar_ptr->process_theme.focus_other_color);
				
				text_rect.left = fg_rect.left;
				text_rect.right = fg_rect.left + VOLUME_BRIGHTNESS_START_LENGTH - 1;
				text_rect.top = fg_rect.top + VOLUME_BRIGHTNESS_BG_TOP_MARGIN;
				text_rect.bottom = fg_rect.bottom - VOLUME_BRIGHTNESS_BG_BOTTOM_MARGIN;
				
				str_style.font_color = progressbar_ptr->process_theme.text_color;
				str_style.align = ALIGN_HVMIDDLE;
				sprintf((char*)data_buf, "%ld", progressbar_ptr->cur_item_index );
	            str = (char*)data_buf;
	            str_info.wstr_len = strlen(str);
	            str_info.wstr_ptr = wstr;
	            MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, str_info.wstr_len, str_info.wstr_len );
				GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
		                               (const GUI_RECT_T *)&text_rect,
		                               (const GUI_RECT_T *)&text_rect,
		                               (const MMI_STRING_T *)&str_info,
		                               &str_style,
		                               str_state,
		                               GUISTR_TEXT_DIR_AUTO);
			}
    	}
		else
		{
			MMI_DIRECTION_SET_E dir = MMITHEME_GetDisDirection();
#if 0
			GUI_POINT_T     start_point = {0};
			
			start_point.x = text_rect.left;
			start_point.y = text_rect.top;
#endif
			str_style.font_color = progressbar_ptr->process_theme.text_color;
            str_style.align = (MMI_DIRECTION_LEFT == dir) ? ALIGN_LEFT : ALIGN_RIGHT;
			str_style.begin_alpha = str_style.end_alpha = (uint8)(255*0.6);
			str_state = GUISTR_STATE_EFFECT | GUISTR_STATE_GRADUAL_HOR;
			MMITHEME_GetResText(progressbar_ptr->process_theme.text1_id, progressbar_ptr->base_ctrl.win_handle, &str_info);
#if 0			
			GUISTR_DrawTextToLCDSingleLine((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
										   (const GUI_POINT_T *)&start_point, 
										   (const MMI_STRING_T *)&str_info, 
										   &str_style, 
										   str_state);
#endif
			GUISTR_DrawSingleLineGradualInRect(
				(const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info, 
				(const GUI_RECT_T *)&text_rect,
				(const MMI_STRING_T *)&str_info, 
				&str_style, 
                str_state);
			
			str_state = GUISTR_STATE_NONE;
			
			if(progressbar_ptr->cur_item_index ==0)
			{
				text_rect.left = fg_rect.left;
				text_rect.right = fg_rect.left + VOLUME_BRIGHTNESS_START_LENGTH - 1;
				text_rect.top = fg_rect.top + VOLUME_BRIGHTNESS_BG_TOP_MARGIN;
				text_rect.bottom = fg_rect.bottom - VOLUME_BRIGHTNESS_BG_BOTTOM_MARGIN;
				
				str_style.align = ALIGN_HVMIDDLE;
				sprintf((char*)data_buf, "%ld", 0);
	            str = (char*)data_buf;
	            str_info.wstr_len = strlen(str);
	            str_info.wstr_ptr = wstr;
	            MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, str_info.wstr_len, str_info.wstr_len );
				GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
		                               (const GUI_RECT_T *)&text_rect,
		                               (const GUI_RECT_T *)&text_rect,
		                               (const MMI_STRING_T *)&str_info,
		                               &str_style,
		                               str_state,
		                               GUISTR_TEXT_DIR_AUTO);
				
				text_rect.right = fg_rect.right - VOLUME_BRIGHTNESS_BAR_MARGIN;
				text_rect.left = text_rect.right - VOLUME_BRIGHTNESS_TEXT_LENGTH*3 + 1;
				str_style.align = ALIGN_RVMIDDLE;
				MMITHEME_GetResText(progressbar_ptr->process_theme.text2_id, progressbar_ptr->base_ctrl.win_handle, &str_info);
				GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
		                               (const GUI_RECT_T *)&text_rect,
		                               (const GUI_RECT_T *)&text_rect,
		                               (const MMI_STRING_T *)&str_info,
		                               &str_style,
		                               str_state,
		                               GUISTR_TEXT_DIR_AUTO);
			}
			else
			{
				fg_rect.right = (int16) (VOLUME_BRIGHTNESS_START_LENGTH + (progressbar_ptr->cur_item_index - 1) * VOLUME_BRIGHTNESS_RAISE_LENGTH - 1);    /*lint !e737*/
				GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                      fg_rect,
                      progressbar_ptr->process_theme.unfocus_fg_color);

				text_rect.left = fg_rect.left;
				text_rect.right = fg_rect.left + VOLUME_BRIGHTNESS_START_LENGTH - 1;
				text_rect.top = fg_rect.top + VOLUME_BRIGHTNESS_BG_TOP_MARGIN;
				text_rect.bottom = fg_rect.bottom - VOLUME_BRIGHTNESS_BG_BOTTOM_MARGIN;
				
				str_style.align = ALIGN_HVMIDDLE;
				sprintf((char*)data_buf, "%ld", progressbar_ptr->cur_item_index );
	            str = (char*)data_buf;
	            str_info.wstr_len = strlen(str);
	            str_info.wstr_ptr = wstr;
	            MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, str_info.wstr_len, str_info.wstr_len );
				GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
		                               (const GUI_RECT_T *)&text_rect,
		                               (const GUI_RECT_T *)&text_rect,
		                               (const MMI_STRING_T *)&str_info,
		                               &str_style,
		                               str_state,
		                               GUISTR_TEXT_DIR_AUTO);
			}
		}
	}	

	return (result);
}
/*****************************************************************************/
//  Description : display vertical or horizontal process box
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayBrightnessBar (
    CTRLPROGRESSBAR_OBJ_T       *progressbar_ptr
)
{
    BOOLEAN         result = TRUE;
    GUI_RECT_T      fg_rect = {0};
    GUI_RECT_T      other_rect = {0};
    GUI_RECT_T      text_rect = {0};
    GUISTR_STATE_T  str_state = GUISTR_STATE_NONE;
    GUISTR_STYLE_T  str_style = {0};
    MMI_STRING_T    str_info = {0};
    uint8 data_buf[3] = {0};
    wchar wstr[3] = {0};
    int32 buff_length = 3;
    char* str = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;
    
    GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
        base_ctrl_ptr->rect,
        progressbar_ptr->process_theme.bg_rect_color);
    
    text_rect.left = progressbar_ptr->label_rect.left;
    text_rect.top = base_ctrl_ptr->rect.top + progressbar_ptr->label_rect.top;
    text_rect.right = progressbar_ptr->label_rect.right;
    text_rect.bottom = base_ctrl_ptr->rect.top + progressbar_ptr->label_rect.bottom;
    str_style.font = progressbar_ptr->process_theme.text_font;
    
    //set foreground rect
    fg_rect.left = progressbar_ptr->fg_rect.left;
    fg_rect.top = base_ctrl_ptr->rect.top + progressbar_ptr->fg_rect.top;
    fg_rect.right = progressbar_ptr->fg_rect.right;
    fg_rect.bottom = base_ctrl_ptr->rect.top + progressbar_ptr->fg_rect.bottom;
    
    GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,fg_rect,
        progressbar_ptr->process_theme.bg_color);
    
    //from left to right
    if ( progressbar_ptr->cur_item_index < progressbar_ptr->total_item_num)
    {
        if(progressbar_ptr->is_active)
        {
            str_style.font_color = progressbar_ptr->process_theme.label_text_focus_color;
            str_style.align = ALIGN_LEFT;
            str_style.font = progressbar_ptr->process_theme.title_font;
            MMITHEME_GetResText(progressbar_ptr->process_theme.text1_id, progressbar_ptr->base_ctrl.win_handle, &str_info);
            GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                (const GUI_RECT_T *)&text_rect,
                (const GUI_RECT_T *)&text_rect,
                (const MMI_STRING_T *)&str_info,
                &str_style,
                str_state,
                GUISTR_TEXT_DIR_AUTO);
            
            fg_rect.right = (int16) ((VOLUME_BRIGHTNESS_START_LENGTH+4) + progressbar_ptr->cur_item_index * (VOLUME_BRIGHTNESS_RAISE_LENGTH+8) - 1);    /*lint !e737*/
            GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                fg_rect,
                progressbar_ptr->process_theme.fg_color);
            
            other_rect = fg_rect;
            other_rect.left = fg_rect.right + 1;
            other_rect.right = progressbar_ptr->fg_rect.right;
            GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                other_rect,
                progressbar_ptr->process_theme.focus_other_color);
            
            text_rect.left = fg_rect.left;
            text_rect.right = fg_rect.left + (VOLUME_BRIGHTNESS_START_LENGTH+4) - 1;
            text_rect.top = fg_rect.top + VOLUME_BRIGHTNESS_BG_TOP_MARGIN;
            text_rect.bottom = fg_rect.bottom - VOLUME_BRIGHTNESS_BG_BOTTOM_MARGIN;
            str_style.font_color = progressbar_ptr->process_theme.text_color;
            str_style.align = ALIGN_HVMIDDLE;
            str_style.font = progressbar_ptr->process_theme.text_font;
            sprintf((char*)data_buf, "%ld", progressbar_ptr->cur_item_index + 1);
            str = (char*)data_buf;
            str_info.wstr_len = strlen(str);
            str_info.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, str_info.wstr_len, str_info.wstr_len );
            GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                (const GUI_RECT_T *)&text_rect,
                (const GUI_RECT_T *)&text_rect,
                (const MMI_STRING_T *)&str_info,
                &str_style,
                str_state,
                GUISTR_TEXT_DIR_AUTO);
        }
        else
        {
	     MMI_DIRECTION_SET_E dir = MMITHEME_GetDisDirection();
	
            str_style.font_color = progressbar_ptr->process_theme.text_color;
            str_style.align = (MMI_DIRECTION_LEFT == dir) ? ALIGN_LEFT : ALIGN_RIGHT;
            str_style.begin_alpha = str_style.end_alpha = (uint8)(255*0.6);
            str_style.font = progressbar_ptr->process_theme.title_font;
            str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_EFFECT | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_NOT_HALF_CHAR | GUISTR_STATE_GRADUAL_VER;
            MMITHEME_GetResText(progressbar_ptr->process_theme.text1_id, progressbar_ptr->base_ctrl.win_handle, &str_info);

            GUISTR_DrawSingleLineGradualInRect(
                (const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info, 
                (const GUI_RECT_T *)&text_rect,
                (const MMI_STRING_T *)&str_info, 
                &str_style, 
                str_state);

            str_state = GUISTR_STATE_NONE;
            
            fg_rect.right = (int16) ((VOLUME_BRIGHTNESS_START_LENGTH+4) + progressbar_ptr->cur_item_index * (VOLUME_BRIGHTNESS_RAISE_LENGTH+8) - 1);    /*lint !e737*/
            GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                fg_rect,
                progressbar_ptr->process_theme.unfocus_fg_color);
            
            text_rect.left = fg_rect.left;
            text_rect.right = fg_rect.left + (VOLUME_BRIGHTNESS_START_LENGTH+4) - 1;
            text_rect.top = fg_rect.top + VOLUME_BRIGHTNESS_BG_TOP_MARGIN;
            text_rect.bottom = fg_rect.bottom - VOLUME_BRIGHTNESS_BG_BOTTOM_MARGIN;
            str_style.align = ALIGN_HVMIDDLE;
            str_style.font = progressbar_ptr->process_theme.text_font;
            sprintf((char*)data_buf, "%ld", progressbar_ptr->cur_item_index + 1);
            str = (char*)data_buf;
            str_info.wstr_len = strlen(str);
            str_info.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, str_info.wstr_len, str_info.wstr_len );
            GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                (const GUI_RECT_T *)&text_rect,
                (const GUI_RECT_T *)&text_rect,
                (const MMI_STRING_T *)&str_info,
                &str_style,
                str_state,
                GUISTR_TEXT_DIR_AUTO);
        }
    }	
    
    return (result);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: zhenxing.chen
//  Note:add for nps score is 0, the score would high light or not flag
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetScoreHighLightFlag( 
        MMI_CTRL_ID_T  ctrl_id,
        BOOLEAN  highlight_flag
        )
{
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr = PNULL;
    
    progressbar_ptr = GetProBoxPtr (ctrl_id);

    if (PNULL == progressbar_ptr)
    {
        return;
    }
    progressbar_ptr->is_high_light = highlight_flag;   
}

/*****************************************************************************/
//  Description : display vertical or horizontal score bar
//  Global resource dependence :
//  Author: 
//  Note:   [MS]
/*****************************************************************************/
LOCAL BOOLEAN DisplayNPSScoreBar (
    CTRLPROGRESSBAR_OBJ_T       *progressbar_ptr
)
{
	BOOLEAN         result = TRUE;
    GUI_RECT_T      fg_rect = {0};
	GUI_RECT_T      other_rect = {0};
	GUI_RECT_T      text_rect = {0};
	GUI_RECT_T      text_rect0 = {0};
	GUI_RECT_T      text_rect10 = {0};
	GUISTR_STATE_T  str_state = GUISTR_STATE_NONE;
	GUISTR_STYLE_T  str_style = {0};
	MMI_STRING_T    str_info = {0};
	MMI_STRING_T    str_info_0 = {0};
	MMI_STRING_T    str_info_10 = {0};
	uint8 data_buf[3] = {0};
	uint8 data_buff0[2] = {0};
	uint8 data_buff10[2] = {0x0a};
    wchar wstr[3] = {0};
    wchar wstr0[3] = {'0'};
	wchar wstr10[3] = {'1','0'};
    int32 buff_length = 3;
    char* str = PNULL;
	char* str0 = PNULL;
    GUI_RECT_T      cross_rect = {0};
    GUI_RECT_T      cross_rect0 = {0};
    GUI_RECT_T      cross_rect10 = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;
	GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
				base_ctrl_ptr->display_rect,//->rect
				progressbar_ptr->process_theme.bg_color);
	if((0 >= base_ctrl_ptr->rect.bottom) || (319 <= base_ctrl_ptr->rect.top)) //超出显示区域
	{
		return(result);
	}
	else if(base_ctrl_ptr->rect.top < base_ctrl_ptr->display_rect.top)//上半部分超出显示区域,超出到状态栏的范围
	{
       	//set foreground rect
       fg_rect.left = progressbar_ptr->fg_rect.left;
	   fg_rect.bottom = base_ctrl_ptr->display_rect.bottom - VOLUME_BRIGHTNESS_BAR_MARGIN;
	   if((base_ctrl_ptr->display_rect.top + (progressbar_ptr->fg_rect.bottom - progressbar_ptr->fg_rect.top)) > fg_rect.bottom)
	   {
		   fg_rect.top = base_ctrl_ptr->display_rect.top;
	   }
	   else
	   {
		   fg_rect.top = fg_rect.bottom - (progressbar_ptr->fg_rect.bottom - progressbar_ptr->fg_rect.top);
		    //0
	       text_rect0.left = progressbar_ptr->label_rect.left;
	       text_rect0.right = text_rect0.left + NPS_SCORE_LABEL_WIDTH;
		   text_rect0.bottom = fg_rect.top - 1;
// 		   if((base_ctrl_ptr->display_rect.top + (progressbar_ptr->label_rect.bottom - progressbar_ptr->label_rect.top)) > text_rect0.bottom)
// 		   {
//                text_rect0.top = base_ctrl_ptr->display_rect.top;
// 		   }
// 		   else
// 		   {
			   text_rect0.top = text_rect0.bottom - (progressbar_ptr->label_rect.bottom - progressbar_ptr->label_rect.top);
/*		   }*/
		   //10
	      text_rect10.right = progressbar_ptr->label_rect.right - VOLUME_BRIGHTNESS_BAR_MARGIN;
	      text_rect10.left = text_rect10.right - NPS_SCORE_LABEL_WIDTH;
		  text_rect10.top = text_rect0.top;
		  text_rect10.bottom = text_rect0.bottom;
	   }       
	}
	else if((319 > base_ctrl_ptr->rect.top) && (319 < base_ctrl_ptr->rect.bottom))//下半部分超出显示区域
	{
	   text_rect0.left = progressbar_ptr->label_rect.left;
	   text_rect0.top = base_ctrl_ptr->display_rect.top + progressbar_ptr->label_rect.top;
	   text_rect0.right = text_rect0.left + NPS_SCORE_LABEL_WIDTH;
	   text_rect0.bottom = base_ctrl_ptr->display_rect.top + progressbar_ptr->label_rect.bottom;
       //10
	   text_rect10.right = progressbar_ptr->label_rect.right - VOLUME_BRIGHTNESS_BAR_MARGIN;
	   text_rect10.left = text_rect10.right - NPS_SCORE_LABEL_WIDTH;
	   text_rect10.top = base_ctrl_ptr->display_rect.top + progressbar_ptr->label_rect.top;	
	   text_rect10.bottom = base_ctrl_ptr->display_rect.top + progressbar_ptr->label_rect.bottom;
	   str_style.font = progressbar_ptr->process_theme.text_font;
	   if(text_rect10.bottom > base_ctrl_ptr->display_rect.bottom)
	   {
		   text_rect0.bottom = base_ctrl_ptr->display_rect.bottom;
		   text_rect10.bottom = base_ctrl_ptr->display_rect.bottom;
	   }
	   else
	   {
           //set foreground rect
           fg_rect.left = progressbar_ptr->fg_rect.left;
           fg_rect.top = base_ctrl_ptr->display_rect.top + progressbar_ptr->fg_rect.top;
	       //fg_rect.right = progressbar_ptr->fg_rect.right;//@coverity 26022 Unused value
	       fg_rect.bottom = base_ctrl_ptr->display_rect.top + progressbar_ptr->fg_rect.bottom;
	   }
	}
	else 
	{
       //0
	   text_rect0.left = progressbar_ptr->label_rect.left;
	   text_rect0.top = base_ctrl_ptr->display_rect.top + progressbar_ptr->label_rect.top;
	   text_rect0.right = text_rect0.left + NPS_SCORE_LABEL_WIDTH;
	   text_rect0.bottom = base_ctrl_ptr->display_rect.top + progressbar_ptr->label_rect.bottom;
       //10
	   text_rect10.right = progressbar_ptr->label_rect.right - VOLUME_BRIGHTNESS_BAR_MARGIN;
	   text_rect10.left = text_rect10.right - NPS_SCORE_LABEL_WIDTH;
	   text_rect10.top = base_ctrl_ptr->display_rect.top + progressbar_ptr->label_rect.top;	
	   text_rect10.bottom = base_ctrl_ptr->display_rect.top + progressbar_ptr->label_rect.bottom;
//	   str_style.font = progressbar_ptr->process_theme.text_font;
       //set foreground rect
       fg_rect.left = progressbar_ptr->fg_rect.left;
       fg_rect.top = base_ctrl_ptr->display_rect.top + progressbar_ptr->fg_rect.top;
	   //fg_rect.right = progressbar_ptr->fg_rect.right;//@coverity 26022 Unused value
	   fg_rect.bottom = base_ctrl_ptr->display_rect.top + progressbar_ptr->fg_rect.bottom;
	}

	//from left to right
    if ( progressbar_ptr->cur_item_index < progressbar_ptr->total_item_num)
    {
        if(progressbar_ptr->is_active)
		{
			str_style.font = progressbar_ptr->process_theme.text_font;
			str_style.font_color = progressbar_ptr->process_theme.label_text_focus_color;
           if(GUI_IntersectRect(&cross_rect0,text_rect0,base_ctrl_ptr->display_rect))
           {
			str_style.align = ALIGN_LEFT;
			sprintf((char*)data_buff0, "%-2ld", 0);
			str0 = (char*)data_buff0;
            str_info_0.wstr_len = strlen((char*)data_buff0);
            str_info_0.wstr_ptr = wstr0;
			MMI_STRNTOWSTR( wstr0, buff_length, (uint8*)str0, str_info.wstr_len, str_info.wstr_len );
			GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
	                               (const GUI_RECT_T *)&text_rect0,
	                               (const GUI_RECT_T *)&cross_rect0,
	                               (const MMI_STRING_T *)&str_info_0,
	                               &str_style,
	                               str_state,
	                               GUISTR_TEXT_DIR_AUTO);
           }
           if(GUI_IntersectRect(&cross_rect10,text_rect10,base_ctrl_ptr->display_rect))
           {
			str_style.align = ALIGN_RIGHT;
			sprintf((char*)data_buff10, "%-2ld", 10);
			str0 = (char*)data_buff10;
			str_info_10.wstr_len = strlen((char*)data_buff10);
            str_info_10.wstr_ptr = wstr10;
			MMI_STRNTOWSTR( wstr10, buff_length, (uint8*)str0, str_info.wstr_len, str_info.wstr_len );
			GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
	                               (const GUI_RECT_T *)&text_rect10,
	                               (const GUI_RECT_T *)&cross_rect10,
	                               (const MMI_STRING_T *)&str_info_10,
	                               &str_style,
	                               str_state,
	                               GUISTR_TEXT_DIR_AUTO);
           }
			if( 0 >= progressbar_ptr->cur_item_index)
			{
			#if 0
				fg_rect.right = fg_rect.left;
                GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                      fg_rect,
                      progressbar_ptr->process_theme.fg_color);
            #endif
				other_rect = fg_rect;
			    other_rect.left = fg_rect.left;  //bug498882, other_rect.left = fg_rect.left + 1;
			    other_rect.right = progressbar_ptr->fg_rect.right;
			    GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                      other_rect,
                      progressbar_ptr->process_theme.focus_other_color);
                
                if(progressbar_ptr->is_high_light)//add for india operator requirement for bug504228 
                {    
                   fg_rect.right = (int16) (NPS_SCORE_START_LENGTH - 1);    /*lint !e737*/
			       GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                      fg_rect,
                      progressbar_ptr->process_theme.fg_color);
                   text_rect.left = fg_rect.left;
                   text_rect.right = fg_rect.left + NPS_SCORE_START_LENGTH - 1;
                   text_rect.top = fg_rect.top + VOLUME_BRIGHTNESS_BG_TOP_MARGIN;
                   text_rect.bottom = fg_rect.bottom - VOLUME_BRIGHTNESS_BG_BOTTOM_MARGIN;
                   str_style.font_color = progressbar_ptr->process_theme.text_color;
                  if(GUI_IntersectRect(&cross_rect,text_rect,base_ctrl_ptr->display_rect))
                  {
                   str_style.align = ALIGN_HVMIDDLE;
                   sprintf((char*)data_buf, "%-2ld", progressbar_ptr->cur_item_index);
                   str = (char*)data_buf;
                   str_info.wstr_len = strlen(str);
                   str_info.wstr_ptr = wstr;
                   MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, str_info.wstr_len, str_info.wstr_len );
                   GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                                   (const GUI_RECT_T *)&text_rect,
                                   (const GUI_RECT_T *)&cross_rect,
                                   (const MMI_STRING_T *)&str_info,
                                   &str_style,
                                   str_state,
                                   GUISTR_TEXT_DIR_AUTO);  //end add
                   }
                 }
            }
            else
            {   //focus的分数条颜色
                if( (0<progressbar_ptr->cur_item_index) && (progressbar_ptr->cur_item_index<2))
                {
                   fg_rect.right = (int16) (NPS_SCORE_START_LENGTH - 1);    /*lint !e737*/
			       GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                      fg_rect,
                      progressbar_ptr->process_theme.fg_color);
				}
				else
				{
                   fg_rect.right = (int16) (NPS_SCORE_START_LENGTH + (progressbar_ptr->cur_item_index - 1) * NPS_SCORE_RAISE_LENGTH - 1);    /*lint !e737*/
			       GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                      fg_rect,
                      progressbar_ptr->process_theme.fg_color);
				}    		  
             //focus的背景色
			  other_rect = fg_rect;
			  other_rect.left = fg_rect.right + 1;
			  other_rect.right = progressbar_ptr->fg_rect.right;
			  GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                      other_rect,
                      progressbar_ptr->process_theme.focus_other_color);
		  	  text_rect.left = fg_rect.left;
			  text_rect.right = fg_rect.left + NPS_SCORE_START_LENGTH - 1;
			  text_rect.top = fg_rect.top + VOLUME_BRIGHTNESS_BG_TOP_MARGIN;
			  text_rect.bottom = fg_rect.bottom - VOLUME_BRIGHTNESS_BG_BOTTOM_MARGIN;
             if(GUI_IntersectRect(&cross_rect,text_rect,base_ctrl_ptr->display_rect))
             {
			  str_style.font_color = progressbar_ptr->process_theme.text_color;
			  str_style.align = ALIGN_HVMIDDLE;
			  sprintf((char*)data_buf, "%-2ld", progressbar_ptr->cur_item_index);
              str = (char*)data_buf;
              str_info.wstr_len = strlen(str);
              str_info.wstr_ptr = wstr;
              MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, str_info.wstr_len, str_info.wstr_len );
			  GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
	                               (const GUI_RECT_T *)&text_rect,
	                               (const GUI_RECT_T *)&cross_rect,
	                               (const MMI_STRING_T *)&str_info,
	                               &str_style,
	                               str_state,
	                               GUISTR_TEXT_DIR_AUTO);
             }
			}
    	}
		else
		{
			str_style.font = progressbar_ptr->process_theme.text_font;
			str_style.font_color = progressbar_ptr->process_theme.text_color;
           if(GUI_IntersectRect(&cross_rect0,text_rect0,base_ctrl_ptr->display_rect))
           {
            str_style.align = ALIGN_LEFT;
			str_style.begin_alpha = str_style.end_alpha = (uint8)(255*0.6);
			sprintf((char*)data_buff0, "%-2ld", 0);
			str0 = (char*)data_buff0;
            str_info_0.wstr_len = strlen((char*)data_buff0);
            str_info_0.wstr_ptr = wstr0;
			MMI_STRNTOWSTR( wstr0, buff_length, (uint8*)str0, str_info.wstr_len, str_info.wstr_len );
			GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
	                               (const GUI_RECT_T *)&text_rect0,
	                               (const GUI_RECT_T *)&cross_rect0,
	                               (const MMI_STRING_T *)&str_info_0,
	                               &str_style,
	                               str_state,
	                               GUISTR_TEXT_DIR_AUTO);
           }
           if(GUI_IntersectRect(&cross_rect10,text_rect10,base_ctrl_ptr->display_rect))
           {
			str_style.align = ALIGN_RIGHT;
			sprintf((char*)data_buff10, "%-2ld", 10);
			str0 = (char*)data_buff10;
			str_info_10.wstr_len = strlen((char*)data_buff10);
            str_info_10.wstr_ptr = wstr10;
			MMI_STRNTOWSTR( wstr10, buff_length, (uint8*)str0, str_info.wstr_len, str_info.wstr_len );
			GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
	                               (const GUI_RECT_T *)&text_rect10,
	                               (const GUI_RECT_T *)&cross_rect10,
	                               (const MMI_STRING_T *)&str_info_10,
	                               &str_style,
	                               str_state,
	                               GUISTR_TEXT_DIR_AUTO);
           }
			if( 0 >= progressbar_ptr->cur_item_index)
			{
                other_rect = fg_rect;
			    other_rect.left = fg_rect.left;//bug498882, other_rect.left = fg_rect.left + 1;
			    other_rect.right = progressbar_ptr->fg_rect.right;
			    GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                      other_rect,
                      progressbar_ptr->process_theme.bg_color);
                if(progressbar_ptr->is_high_light)
                 {
                //add for india operator requirement for bug504228
                   fg_rect.right = (int16) (NPS_SCORE_START_LENGTH - 1);    /*lint !e737*/
                     GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                          fg_rect,
                          progressbar_ptr->process_theme.unfocus_fg_color);
                   text_rect.left = fg_rect.left;
                   text_rect.right = fg_rect.left + NPS_SCORE_START_LENGTH - 1;
                   text_rect.top = fg_rect.bottom - (progressbar_ptr->fg_rect.bottom - progressbar_ptr->fg_rect.top) + VOLUME_BRIGHTNESS_BG_TOP_MARGIN;
                   text_rect.bottom = fg_rect.bottom - VOLUME_BRIGHTNESS_BG_BOTTOM_MARGIN;
                   str_style.font_color = progressbar_ptr->process_theme.text_color;
                  if(GUI_IntersectRect(&cross_rect,text_rect,base_ctrl_ptr->display_rect))
                  {
                   str_style.align = ALIGN_HVMIDDLE;
                   sprintf((char*)data_buf, "%-2ld", progressbar_ptr->cur_item_index);
                   str = (char*)data_buf;
                   str_info.wstr_len = strlen(str);
                   str_info.wstr_ptr = wstr;
                   MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, str_info.wstr_len, str_info.wstr_len );                  
                   GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                                   (const GUI_RECT_T *)&text_rect,
                                   (const GUI_RECT_T *)&cross_rect,
                                   (const MMI_STRING_T *)&str_info,
                                   &str_style,
                                   str_state,
                                   GUISTR_TEXT_DIR_AUTO);  //end add
                  }
                 }
                else
                 {
                    GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                                      other_rect,
                                      progressbar_ptr->process_theme.focus_other_color);
                 }
            }
            else
			{   //unfocus的分数条颜色
				if( (0<progressbar_ptr->cur_item_index) && (progressbar_ptr->cur_item_index<2))
				{
                   fg_rect.right = (int16) (NPS_SCORE_START_LENGTH - 1);    /*lint !e737*/
		           GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                          fg_rect,
                          progressbar_ptr->process_theme.unfocus_fg_color);
				}
				else
				{
                   fg_rect.right = (int16) (NPS_SCORE_START_LENGTH + (progressbar_ptr->cur_item_index - 1) * NPS_SCORE_RAISE_LENGTH - 1);    /*lint !e737*/
			       GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                          fg_rect,
                          progressbar_ptr->process_theme.unfocus_fg_color);
				}   
			str_state = GUISTR_STATE_NONE;	
	        //unfocus时的背景
            other_rect = fg_rect;
			other_rect.left = fg_rect.right + 1;
			other_rect.right = progressbar_ptr->fg_rect.right;
			GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                      other_rect,
                      progressbar_ptr->process_theme.bg_color);
			text_rect.left = fg_rect.left;
			text_rect.right = fg_rect.left + NPS_SCORE_START_LENGTH - 1;
            text_rect.top = fg_rect.bottom - (progressbar_ptr->fg_rect.bottom - progressbar_ptr->fg_rect.top) + VOLUME_BRIGHTNESS_BG_TOP_MARGIN;
            text_rect.bottom = fg_rect.bottom - VOLUME_BRIGHTNESS_BG_BOTTOM_MARGIN;
           if(GUI_IntersectRect(&cross_rect,text_rect,base_ctrl_ptr->display_rect))
           {
			str_style.align = ALIGN_HVMIDDLE;
			sprintf((char*)data_buf, "%-2ld", progressbar_ptr->cur_item_index);
            str = (char*)data_buf;
            str_info.wstr_len = strlen(str);
            str_info.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, str_info.wstr_len, str_info.wstr_len );
			GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
	                               (const GUI_RECT_T *)&text_rect,
	                               (const GUI_RECT_T *)&cross_rect,
	                               (const MMI_STRING_T *)&str_info,
	                               &str_style,
	                               str_state,
	                               GUISTR_TEXT_DIR_AUTO);
           }
			}
		}
	}	
	return (result);
}

/*****************************************************************************/
//  Description : Display rect waiting indicator
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayRectWaitingIndicator (
                                            CTRLPROGRESSBAR_OBJ_T       *progressbar_ptr
                                          )
{
    BOOLEAN            result = TRUE;
	GUI_LCD_DEV_INFO   lcd_dev_info_ptr = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};    
    GUI_RECT_T         win_rect = {0};
    uint16             logic_width = 0;
    uint16             logic_height = 0;
    MMI_STRING_T       spinner_text = {0};
    CTRLBASE_OBJ_T     *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;    
    GUI_RECT_T          text_rect = {0};    
    GUI_BOTH_RECT_T    *anim_both_rect_ptr = PNULL;  
    GUIANIM_INIT_DATA_T     anim_init = {0};
    MMI_CONTROL_CREATE_T    create = {0};
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0}; 
    void                    *anim_ctrl  = PNULL;    
    GUISTR_STYLE_T      text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    //MMI_STRING_T        content_str = {0};
    //wchar               content_text[] = L"Uploading 4/9";   
    GUI_RECT_T          bg_rect = {0};

    //Create waiting indicator anim
    if (PNULL == progressbar_ptr->waiting_ind_animctrl_handle)
    {    
        if (TRUE == progressbar_ptr->is_active)
        {
            data_info.img_id = progressbar_ptr->process_theme.waiting_indicator_focused_img_id;
        }    
        else 
        {    
            data_info.img_id = progressbar_ptr->process_theme.waiting_indicator_unfocused_img_id;
        }   

        display_info.align_style = GUIANIM_ALIGN_LEFT_TOP;
        display_info.is_play_once = FALSE;
        display_info.is_need_end_ind = TRUE;
        display_info.is_update = FALSE;
        
        anim_init.both_rect = base_ctrl_ptr->both_rect;
        anim_init.both_rect.v_rect.bottom = base_ctrl_ptr->both_rect.v_rect.bottom - 6;           
        anim_init.both_rect.v_rect.top = base_ctrl_ptr->both_rect.v_rect.bottom - 15;
        anim_both_rect_ptr = &anim_init.both_rect;

        //creat anim
        create.ctrl_id           = 0;
        create.guid              = SPRD_GUI_ANIM_ID;
        create.init_data_ptr     = &anim_init;
        create.parent_win_handle = progressbar_ptr->win_id;
        create.parent_ctrl_handle = base_ctrl_ptr->handle;
        anim_ctrl = MMK_CreateControl(&create);

        ctrl_info.ctrl_ptr   = anim_ctrl;
        ctrl_info.is_ctrl_id = FALSE;
        ctrl_info.ctrl_id = 0;

        GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);        
        CTRLANIM_SetPeriodByPtr(10,anim_ctrl);

        progressbar_ptr->waiting_ind_animctrl_handle = anim_ctrl;
    }
    
    //content_str.wstr_ptr = content_text;
    //content_str.wstr_len = MMIAPICOM_Wstrlen(content_text);
    //progressbar_ptr->rect_progress_title = &content_str;
    
    text_style.align = ALIGN_LEFT;           
    text_style.font = SONG_FONT_14;
    text_style.font_color = 0xFFFF;

    text_rect = base_ctrl_ptr->both_rect.v_rect;
    text_rect.bottom = anim_init.both_rect.v_rect.top;    
    text_rect.top = text_rect.bottom - (GUI_GetFontHeight(SONG_FONT_14,UNICODE_HANZI));    

    bg_rect.bottom = anim_init.both_rect.v_rect.bottom + 6;
    
    if ( PNULL != progressbar_ptr->rect_progress_title)
    {
        bg_rect.top = text_rect.top - 7;
    }
    else 
    {
        bg_rect.top = anim_init.both_rect.v_rect.top;   
    }
    
    bg_rect.left = 0;
    bg_rect.right = 239;

    //Draw background 
    /*
    if (FALSE == progressbar_ptr->process_theme.is_dialog)
    {
        GUI_FillRect(
            (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info, 
            bg_rect,
            MAKE_BREW_RGB(0x02, 0x59, 0xA2)
        );
    }
    */
    
    //Draw title 
    if ( PNULL != progressbar_ptr->rect_progress_title)
    {
        GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info,               
        (const GUI_RECT_T *)&text_rect,       //the fixed display area
        (const GUI_RECT_T *)&text_rect,       
        (const MMI_STRING_T *)progressbar_ptr->rect_progress_title,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    }       
    return (result);
}

/*****************************************************************************/
//  Description : Display rect progressbar
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayRectBarProcess (
    CTRLPROGRESSBAR_OBJ_T       *progressbar_ptr
)
{
    BOOLEAN         result = TRUE;
    GUI_RECT_T      fg_rect = {0};
	GUI_RECT_T      bg_rect = {0};
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    //set background
    bg_rect = base_ctrl_ptr->rect;
//    bg_rect.left += 3; 
//    bg_rect.right -= 3;
    bg_rect.top += 6;
    bg_rect.bottom -= 6;
    
    GUI_FillRect((const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info, bg_rect, progressbar_ptr->process_theme.bg_color);

    //set foreground
    fg_rect = base_ctrl_ptr->rect;
//    fg_rect.left += 3; 
//    fg_rect.right -= 3;
    fg_rect.top += 6;
    fg_rect.bottom -= 6;

    if ( 0 != progressbar_ptr->total_item_num)
    {
        if ( (progressbar_ptr->cur_item_index + 1) <= progressbar_ptr->total_item_num)
        {
        	fg_rect.right = (int16) (fg_rect.left + (fg_rect.right - fg_rect.left) * progressbar_ptr->cur_item_index / progressbar_ptr->total_item_num);    /*lint !e737*/
        }
    }
    else 
    {
       fg_rect.right = fg_rect.left;
    }
        
    if (0 != progressbar_ptr->cur_item_index)
    {
		GUI_FillRect((const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info, fg_rect, progressbar_ptr->process_theme.fg_color);
    }

    return (result);
}

/*****************************************************************************/
//  Description : Set rect progressbar theme color (DIALOG/DEFAULT)
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetRectBarTheme (
    CTRLPROGRESSBAR_OBJ_T   *progressbar_ptr
)
{
    if (PNULL == progressbar_ptr)
        return ;

    if (TRUE == progressbar_ptr->process_theme.is_dialog)
    {            
        progressbar_ptr->process_theme.fg_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT) ;
        progressbar_ptr->process_theme.bg_color = GUI_RGB2RGB565(0x66, 0x66, 0x66);
       
    }
    else 
    {
        progressbar_ptr->process_theme.fg_color = 0xCD7C;
        progressbar_ptr->process_theme.bg_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_3);        
    }   
}

/*****************************************************************************/
//  Description : Set rect progressbar theme (DIALOG/DEFAULT)
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetIsRectBarInDialog (
    MMI_CTRL_ID_T  	ctrl_id,
    BOOLEAN       	is_in_dialog    
)
{
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr = PNULL;

    progressbar_ptr = GetProBoxPtr (ctrl_id);

    if (PNULL == progressbar_ptr) 
    {
        return;
    }
    else 
    {
        progressbar_ptr->process_theme.is_dialog = is_in_dialog;
    }
    
    SetRectBarTheme(progressbar_ptr);
}

/*****************************************************************************/
//  Description : Set rect progressbar position (x position/y position)
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetRectBarPosition (
    MMI_CTRL_ID_T  ctrl_id,
    uint16         x_position,    
    uint16         y_position
)
{
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr = PNULL;    
    CTRLBASE_OBJ_T *base_ctrl_ptr = PNULL;    
    GUI_RECT_T rect = {0};

    progressbar_ptr = GetProBoxPtr (ctrl_id);
    base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;
    
    if (PNULL == progressbar_ptr)
        return;
        
    rect.top = y_position;
    rect.bottom = rect.top + RECT_PROGRESSBAR_HEIGHT;
    rect.left = x_position;
    rect.right = RECT_PROGRESSBAR_WIDTH - rect.left - 1;
    ProgressBarSetRect(base_ctrl_ptr, &rect);       

    //Draw progressbar 
    ProgressBarDraw (progressbar_ptr);
}

/*****************************************************************************/
//  Description : Set is_waiting_indicator flag
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetIsInWaitingInd(
                                            MMI_CTRL_ID_T  ctrl_id,
                                            BOOLEAN        is_waiting_indicator,
                                            BOOLEAN        is_update
                                            )                                                  
{
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr = PNULL;    
    CTRLBASE_OBJ_T *base_ctrl_ptr = PNULL;    
    GUI_RECT_T rect = {0};

    progressbar_ptr = GetProBoxPtr (ctrl_id);        
    if (PNULL == progressbar_ptr)
        return;
        
    progressbar_ptr->is_waiting_indicator = is_waiting_indicator;        

    //Draw progressbar 
    if(TRUE == is_update)
    {
        ProgressBarDraw (progressbar_ptr);
    }
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle tp press key message for progress like windows.
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL GUIPRGBOX_RESULT_E HandleWindowsTpMsg (
    CTRLPROGRESSBAR_OBJ_T	 *progressbar_ptr,
    uint32               *first_item_index_ptr,	//in/out
    uint32               *cur_item_index_ptr,   //in/out:
    GUI_POINT_T          point
)
{
    uint32      total_item_num = 0; // the move steps of the progress
    uint32		first_item_index = *first_item_index_ptr;
    uint32      items_per_page = 0;
    uint32		end_item_index = 0;

    GUIPRGBOX_RESULT_E      result = GUIPRGBOX_RESULT_NONE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != cur_item_index_ptr); /*assert verified*/
    if (PNULL == cur_item_index_ptr)
    {
        return GUIPRGBOX_RESULT_NONE;
    }


    total_item_num = progressbar_ptr->total_item_num;
    items_per_page = progressbar_ptr->items_per_page;

    if (IsDisplayArrow (progressbar_ptr)
            && GUI_PointIsInRect (point, progressbar_ptr->pre_arrow_rect))
    {
        //if press previous arrow rect
        if (first_item_index > 0)
        {
            first_item_index--;

            result = GUIPRGBOX_RESULT_PRE_ONE;
        }
        else
        {
            result = GUIPRGBOX_RESULT_HEAD;
        }
    }
    else if (IsDisplayArrow (progressbar_ptr)
             && GUI_PointIsInRect (point, progressbar_ptr->next_arrow_rect))
    {
        //if press next arrow rect
        if (first_item_index + items_per_page < total_item_num)
        {
            first_item_index++;
            end_item_index = first_item_index + items_per_page - 1;

            result = GUIPRGBOX_RESULT_NEXT_ONE;
        }
        else
        {
            end_item_index = total_item_num - 1;
            result = GUIPRGBOX_RESULT_TAIL;
        }
    }
    else if (GUI_PointIsInRect (point, progressbar_ptr->pre_groove_rect))
    {
        //if press previous page rect
        if (first_item_index >= items_per_page)
        {
            first_item_index = first_item_index - items_per_page;
        }
        else
        {
            first_item_index = 0;
        }

        result = GUIPRGBOX_RESULT_PRE_PAGE;
    }
    else if (GUI_PointIsInRect (point, progressbar_ptr->next_groove_rect))
    {
        //if press next page rect
        if (first_item_index + items_per_page * 2 - 1 < total_item_num)
        {
            first_item_index = first_item_index + items_per_page;
            end_item_index = first_item_index + items_per_page - 1;
        }
        else
        {
            first_item_index = total_item_num - items_per_page;
            end_item_index = total_item_num - 1;
        }

        result = GUIPRGBOX_RESULT_NEXT_PAGE;
    }

    *first_item_index_ptr = first_item_index;

    return (result);
}

/*****************************************************************************/
//  Description : handle tp press key message
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleScrollTpMsg (
    uint32                *first_item_index_ptr,//in/out
    uint32                *cur_item_index_ptr,  //in/out:
    GUI_POINT_T           tp_point,
    CTRLPROGRESSBAR_OBJ_T      *progressbar_ptr,
    GUIPRGBOX_RESULT_E    *handle_result_ptr    //in/out:handle result
)
{
    uint32      first_item_index = *first_item_index_ptr;
    uint32      cur_item_index = *cur_item_index_ptr;
    uint32      line_num_page = progressbar_ptr->items_per_page;

#ifdef GUIF_PRGBOX_LETTERSCROLL

    //字母滚动条
    if (GUIPRGBOX_STYLE_LETTERS_SCROLL == progressbar_ptr->prgbox_style)
    {
        if (GUI_PointIsInRect (tp_point, progressbar_ptr->scroll_bar_rect))
        {
            cur_item_index = GetCurrentItem (
                                 progressbar_ptr,
                                 tp_point.y);
        }
    }
    else
#endif

        //if press previous arrow rect
        if (GUI_PointIsInRect (tp_point, progressbar_ptr->pre_arrow_rect))
        {
            if (0 < cur_item_index)
            {
                cur_item_index--;
            }
            else
            {
                if (progressbar_ptr->is_loop_scroll)
                {
                    cur_item_index = progressbar_ptr->total_item_num - 1;
                }
            }

            *handle_result_ptr = GUIPRGBOX_RESULT_PRE_ONE;
        }
    //if press next arrow rect
        else if (GUI_PointIsInRect (tp_point, progressbar_ptr->next_arrow_rect))
        {
            if (cur_item_index < (progressbar_ptr->total_item_num - 1))
            {
                cur_item_index++;
            }
            else
            {
                if (progressbar_ptr->is_loop_scroll)
                {
                    cur_item_index = 0;
                }
            }

            *handle_result_ptr = GUIPRGBOX_RESULT_NEXT_ONE;
        }
    //if press previous page rect
        else if (GUI_PointIsInRect (tp_point, progressbar_ptr->pre_groove_rect))
        {
            if (first_item_index == 0)
            {
                ;
            }
            else if (first_item_index > line_num_page)
            {
                first_item_index = first_item_index - line_num_page;
                cur_item_index = first_item_index;
                *handle_result_ptr = GUIPRGBOX_RESULT_PRE_PAGE;
            }
            else
            {
                first_item_index = 0;
                cur_item_index = first_item_index;
                *handle_result_ptr = GUIPRGBOX_RESULT_PRE_PAGE;
            }
        }
    //if press next page rect
        else if (GUI_PointIsInRect (tp_point, progressbar_ptr->next_groove_rect))
        {
            if ( (first_item_index + line_num_page) < progressbar_ptr->total_item_num)
            {
                first_item_index = first_item_index + line_num_page;

                if (first_item_index > (progressbar_ptr->total_item_num - line_num_page))
                {
                    first_item_index = progressbar_ptr->total_item_num - line_num_page;
                }

                cur_item_index = first_item_index;
                *handle_result_ptr = GUIPRGBOX_RESULT_NEXT_PAGE;
            }
            else
            {
                first_item_index = progressbar_ptr->total_item_num - line_num_page;
                cur_item_index = progressbar_ptr->total_item_num - 1;
                *handle_result_ptr = GUIPRGBOX_RESULT_NEXT_PAGE;
            }
        }

    *cur_item_index_ptr = cur_item_index;
    progressbar_ptr->cur_item_index = cur_item_index;

    return (TRUE);
}

/*****************************************************************************/
//  Description : handle tp press key message
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTpDownMsg (//if handle
    uint32                *cur_item_index_ptr,	//in/out:
    uint32                *first_item_index_ptr,  //in/out
    GUI_POINT_T           tp_point,
    CTRLPROGRESSBAR_OBJ_T      *progressbar_ptr,
    GUIPRGBOX_RESULT_E    *handle_result_ptr  //in/out:handle result
)
{
    BOOLEAN     result = FALSE;
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != cur_item_index_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != handle_result_ptr); /*assert verified*/
    if ( (PNULL == cur_item_index_ptr) || (PNULL == handle_result_ptr))
    {
        return FALSE;
    }

    *handle_result_ptr = GUIPRGBOX_RESULT_NONE;

    if ( (PNULL != progressbar_ptr) &&
            (GUI_PointIsInRect (tp_point, base_ctrl_ptr->rect)))
    {
        progressbar_ptr->cur_item_index = *cur_item_index_ptr;
        progressbar_ptr->first_item_index = *first_item_index_ptr;

        switch (progressbar_ptr->prgbox_style)
        {
        case GUIPRGBOX_STYLE_VERTICAL_SCROLL:
        case GUIPRGBOX_STYLE_HORIZONTAL_SCROLL:
#ifdef GUIF_PRGBOX_LETTERSCROLL
        case GUIPRGBOX_STYLE_LETTERS_SCROLL:

            if (GUIPRGBOX_STYLE_LETTERS_SCROLL == progressbar_ptr->prgbox_style)
            {
                progressbar_ptr->is_need_bg = TRUE;
            }

#endif
            result = HandleScrollTpMsg (
                         first_item_index_ptr,
                         cur_item_index_ptr,
                         tp_point,
                         progressbar_ptr,
                         handle_result_ptr
                     );
            break;

        case GUIPRGBOX_STYLE_VERTICAL_WINDOWS:
        case GUIPRGBOX_STYLE_HORIZONTAL_WINDOWS:
            *handle_result_ptr = HandleWindowsTpMsg (progressbar_ptr, first_item_index_ptr, cur_item_index_ptr, tp_point);
            result = TRUE;
            break;

        default:
            break;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle tp move key message
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTpMoveMsg (
    uint32                *first_item_index_ptr,  //in/out
    uint32                *cur_item_index_ptr,	//in/out:
    GUI_POINT_T           tp_point,
    CTRLPROGRESSBAR_OBJ_T      *progressbar_ptr,
    GUIPRGBOX_RESULT_E    *handle_result_ptr  //in/out:handle result
)
{
    BOOLEAN             result = FALSE;
    uint32              cur_item_index = 0;
    uint32              bar_length = 0;
    GUI_RECT_T          bar_rect = {0};
    GUI_RECT_T          rect = {0};
    uint32				first_item_index = *first_item_index_ptr;
    uint32				new_first_index = 0;

    bar_rect = GetScrollBarRect (
                   progressbar_ptr,
                   progressbar_ptr->cur_item_index,
                   first_item_index
               );

    rect = GetScrollRect (progressbar_ptr);

    bar_length = GetScrollBarLength (
                     progressbar_ptr,
                     progressbar_ptr->cur_item_index,
                     first_item_index
                 );

    if (progressbar_ptr->is_move_valid)
    {
        switch (progressbar_ptr->prgbox_style)
        {
        case GUIPRGBOX_STYLE_VERTICAL_SCROLL:
            progressbar_ptr->scroll_bar_rect.top    = (int16) (tp_point.y - progressbar_ptr->point_offset);             /*lint !e737*/
            progressbar_ptr->scroll_bar_rect.bottom = (int16) (progressbar_ptr->scroll_bar_rect.top + bar_length - 1);  /*lint !e737*/

            if (progressbar_ptr->scroll_bar_rect.bottom > rect.bottom)
            {
                cur_item_index = progressbar_ptr->total_item_num - 1;
            }
            else if (progressbar_ptr->scroll_bar_rect.top < rect.top)
            {
                cur_item_index = 0;
            }
            else
            {
                cur_item_index = GetCurrentItem (
                                     progressbar_ptr,
                                     progressbar_ptr->scroll_bar_rect.top + bar_length / 2);    /*lint !e737*/
            }

            if (cur_item_index > progressbar_ptr->cur_item_index)
            {
                *handle_result_ptr = GUIPRGBOX_RESULT_NEXT_MOVE;
            }
            else if (cur_item_index < progressbar_ptr->cur_item_index)
            {
                *handle_result_ptr = GUIPRGBOX_RESULT_PRE_MOVE;
            }

            break;

        case GUIPRGBOX_STYLE_HORIZONTAL_SCROLL:
            progressbar_ptr->scroll_bar_rect.left  = (int16) (tp_point.x - progressbar_ptr->point_offset);              /*lint !e737*/
            progressbar_ptr->scroll_bar_rect.right = (int16) (progressbar_ptr->scroll_bar_rect.left + bar_length - 1);  /*lint !e737*/

            if (progressbar_ptr->scroll_bar_rect.right > rect.right)
            {
                cur_item_index = progressbar_ptr->total_item_num - 1;
            }
            else if (progressbar_ptr->scroll_bar_rect.left < rect.left)
            {
                cur_item_index = 0;
            }
            else
            {
                cur_item_index = GetCurrentItem (
                                     progressbar_ptr,
                                     tp_point.y);
            }

            if (cur_item_index > progressbar_ptr->cur_item_index)
            {
                *handle_result_ptr = GUIPRGBOX_RESULT_NEXT_MOVE;
            }
            else if (cur_item_index < progressbar_ptr->cur_item_index)
            {
                *handle_result_ptr = GUIPRGBOX_RESULT_PRE_MOVE;
            }

            break;

        case GUIPRGBOX_STYLE_VERTICAL_WINDOWS:
            progressbar_ptr->scroll_bar_rect.top    = (int16) (tp_point.y - bar_length / 2);        /*lint !e737*/
            progressbar_ptr->scroll_bar_rect.bottom = (int16) (tp_point.y + bar_length / 2 - 1);    /*lint !e737*/

            if (progressbar_ptr->scroll_bar_rect.bottom > rect.bottom)
            {
                first_item_index = progressbar_ptr->total_item_num - progressbar_ptr->items_per_page;
            }
            else if (progressbar_ptr->scroll_bar_rect.top < rect.top)
            {
                first_item_index = 0;
            }
            else
            {
                new_first_index = GetFirstItem (progressbar_ptr, first_item_index, tp_point.y - progressbar_ptr->point_offset); /*lint !e737*/

                first_item_index = new_first_index;
            }

            cur_item_index = *cur_item_index_ptr;

            if (first_item_index > progressbar_ptr->first_item_index)
            {
                *handle_result_ptr = GUIPRGBOX_RESULT_NEXT_MOVE;
            }
            else if (first_item_index < progressbar_ptr->first_item_index)
            {
                *handle_result_ptr = GUIPRGBOX_RESULT_PRE_MOVE;
            }

            break;

        case GUIPRGBOX_STYLE_HORIZONTAL_WINDOWS:
            break;
#ifdef GUIF_PRGBOX_LETTERSCROLL
        case GUIPRGBOX_STYLE_LETTERS_SCROLL:

            progressbar_ptr->is_need_bg = TRUE;

            if (cur_item_index >= progressbar_ptr->total_item_num)
            {
                cur_item_index = progressbar_ptr->total_item_num - 1;
            }
            else
            {
                cur_item_index = GetCurrentItem (
                                     progressbar_ptr,
                                     tp_point.y);
            }

            //SCI_TRACE_LOW:"[baokun] TP MOVE tp_point.y %d cur_item_index %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIPRGBOX_2162_112_2_18_3_20_41_254, (uint8*) "dd", tp_point.y, cur_item_index);

            if (cur_item_index > progressbar_ptr->cur_item_index)
            {
                *handle_result_ptr = GUIPRGBOX_RESULT_NEXT_MOVE;
            }
            else if (cur_item_index < progressbar_ptr->cur_item_index)
            {
                *handle_result_ptr = GUIPRGBOX_RESULT_PRE_MOVE;
            }

            break;
#endif
        default:
            break;
        }

        if (progressbar_ptr->cur_item_index != cur_item_index)
        {
#ifdef GUIF_PRGBOX_LETTERSCROLL

            if (GUIPRGBOX_STYLE_LETTERS_SCROLL == progressbar_ptr->prgbox_style)
            {
                DisplayLettersScroll (progressbar_ptr);
            }
            else
#endif
            {
                DisplayVerticalScroll (progressbar_ptr);
            }

            progressbar_ptr->cur_item_index = cur_item_index;
        }

        *cur_item_index_ptr = cur_item_index;
        *first_item_index_ptr = first_item_index;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle tp press key message
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTpMsg (//if handle
    uint32                *cur_item_index_ptr,	//in/out:
    uint32                *first_item_index_ptr,  //in/out
    GUI_POINT_T           tp_point,
    CTRLPROGRESSBAR_OBJ_T      *progressbar_ptr,
    GUIPRGBOX_RESULT_E    *handle_result_ptr,  //in/out:handle result
    MMI_MESSAGE_ID_E      msg_id
)
{
    BOOLEAN             result = FALSE;
    GUI_RECT_T          bar_rect = {0};
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != cur_item_index_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != handle_result_ptr); /*assert verified*/
    //SCI_ASSERT(ProBoxTypeOf((CTRLBASE_OBJ_T*)progressbar_ptr));    /*assert verified*/
    if ( (PNULL == cur_item_index_ptr) || (PNULL == handle_result_ptr) || (! (ProBoxTypeOf ( (CTRLBASE_OBJ_T*) progressbar_ptr))))
    {
        return FALSE;
    }

    *handle_result_ptr = GUIPRGBOX_RESULT_NONE;

    if (PNULL != progressbar_ptr)
    {
        if (GUI_PointIsInRect (tp_point, base_ctrl_ptr->rect))
        {
            bar_rect = GetScrollBarRect (
                           progressbar_ptr,
                           progressbar_ptr->cur_item_index,
                           progressbar_ptr->first_item_index
                       );

            if (msg_id == MSG_TP_PRESS_DOWN)
            {
                result = HandleTpDownMsg (
                             cur_item_index_ptr,
                             first_item_index_ptr,
                             tp_point,
                             progressbar_ptr,
                             handle_result_ptr);

                progressbar_ptr->is_move_valid = FALSE;

                if (GUI_PointIsInRect (tp_point, bar_rect))
                {
                    if ( (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_VERTICAL_SCROLL)
                            || (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_VERTICAL_WINDOWS)
#ifdef GUIF_PRGBOX_LETTERSCROLL
                            || (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_LETTERS_SCROLL)
#endif
                       )
                    {
                        progressbar_ptr->point_offset = tp_point.y - bar_rect.top;
                    }
                    else if ( (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_HORIZONTAL_SCROLL)
                              || (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_HORIZONTAL_WINDOWS))
                    {
                        progressbar_ptr->point_offset = tp_point.x - bar_rect.left;
                    }

                    progressbar_ptr->is_move_valid = TRUE;
                }
            }
            else if ( (msg_id == MSG_TP_PRESS_MOVE) && (progressbar_ptr->is_move_valid))
            {
                result = HandleTpMoveMsg (
                             first_item_index_ptr,
                             cur_item_index_ptr,
                             tp_point,
                             progressbar_ptr,
                             handle_result_ptr);
            }
        }

#ifdef GUIF_PRGBOX_LETTERSCROLL

        if (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_LETTERS_SCROLL)
        {
            if (msg_id == MSG_TP_PRESS_UP)
            {
                //去掉字母滚动条的背景
                progressbar_ptr->is_need_bg = FALSE;
            }

            //CTRLMSG_SendNotify(base_ctrl_ptr->handle, MSG_CTL_PAINT);
            DisplayLettersScroll (progressbar_ptr);
        }

#endif
    }

    return (result);
}
#endif

/*****************************************************************************/
//  Description :  Get Scroll Bar Height
//  Global resource dependence : none
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetScrollBarLength (
    CTRLPROGRESSBAR_OBJ_T	*progressbar_ptr,
    uint32				cur_item_index,
    uint32				first_item_index
)
{
    GUI_RECT_T		scroll_bar_rect = {0};
    uint32          bar_length = 0;

    scroll_bar_rect = GetScrollBarRect (
                          progressbar_ptr,
                          cur_item_index,
                          first_item_index
                      );

    if (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_VERTICAL_SCROLL
            || progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_VERTICAL_WINDOWS)
    {
        bar_length = scroll_bar_rect.bottom - scroll_bar_rect.top + 1;
    }
    else if (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_HORIZONTAL_SCROLL
             || progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_HORIZONTAL_WINDOWS)
    {
        bar_length = scroll_bar_rect.right - scroll_bar_rect.left + 1;
    }
    else
    {
        bar_length = scroll_bar_rect.bottom - scroll_bar_rect.top + 1;
    }

    return bar_length;
}

/*****************************************************************************/
//  Description :  Get Scroll Bar rectangle
//  Global resource dependence : none
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetScrollBarRect (
    CTRLPROGRESSBAR_OBJ_T	*progressbar_ptr,
    uint32		    cur_item_index,
    uint32    		first_item_index
)
{
    GUI_RECT_T      bar_rect = {0};
    GUI_RECT_T      rect = {0};
    int16           width = 0;
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    if (!IsDisplayArrow (progressbar_ptr))
    {
        rect = base_ctrl_ptr->rect;

#if defined(GUIF_PRGBOX_AUTOHIDE)
#ifdef GUIF_PRGBOX_LETTERSCROLL

        if (GUIPRGBOX_STYLE_LETTERS_SCROLL != progressbar_ptr->prgbox_style)
#endif
        {
            GetDispRect (progressbar_ptr, &rect);

            if (GUIPRGBOX_STYLE_VERTICAL_WINDOWS == progressbar_ptr->prgbox_style)
            {
                if (MMI_DIRECTION_LEFT == MMITHEME_GetDisDirection())
                {
                    rect.left = rect.right - progressbar_ptr->ver_scroll_theme.bar_width + 1;
                }
                else
                {
                    rect.right = rect.left + progressbar_ptr->ver_scroll_theme.bar_width - 1;
                }
            }
        }

#endif
    }
    else
    {
        rect = GetScrollRect (progressbar_ptr);
    }

    if (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_VERTICAL_SCROLL)
    {
        bar_rect = rect;
        bar_rect.top    = (int16) (bar_rect.top + cur_item_index * (rect.bottom - rect.top + 1 - PRGBOX_MIN_BAR_LENGTH) / progressbar_ptr->total_item_num);     /*lint !e737*/
        bar_rect.bottom = (int16) (rect.top + (cur_item_index + 1) * (rect.bottom - rect.top + 1 - PRGBOX_MIN_BAR_LENGTH) / progressbar_ptr->total_item_num);   /*lint !e737*/

        if (bar_rect.bottom > bar_rect.top)
        {
            bar_rect.bottom--;
        }

        bar_rect.bottom += PRGBOX_MIN_BAR_LENGTH;
    }
    else if (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_HORIZONTAL_SCROLL)
    {
        bar_rect = rect;
        bar_rect.left  = (int16) (bar_rect.left + cur_item_index * (rect.right - rect.left + 1 - PRGBOX_MIN_BAR_LENGTH) / progressbar_ptr->total_item_num);     /*lint !e737*/
        bar_rect.right = (int16) (rect.left + (cur_item_index + 1) * (rect.right - rect.left + 1 - PRGBOX_MIN_BAR_LENGTH) / progressbar_ptr->total_item_num);   /*lint !e737*/

        if (bar_rect.right > bar_rect.left)
        {
            bar_rect.right--;
        }

        bar_rect.right += PRGBOX_MIN_BAR_LENGTH;
    }
    else if (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_VERTICAL_WINDOWS)
    {
        bar_rect = rect;

        if (progressbar_ptr->total_item_num > progressbar_ptr->items_per_page)
        {
            // 计算进度条需要的高度
            width = (int16) ( (rect.bottom - rect.top + 1) * progressbar_ptr->items_per_page / progressbar_ptr->total_item_num);    /*lint !e737*/

            if (width < PRGBOX_MIN_BAR_LENGTH)
            {
                width = PRGBOX_MIN_BAR_LENGTH;
            }

            // 如果得出的宽度比所能容纳的还大，就取最大值
            if (rect.bottom - rect.top + 1 < width)
            {
                width = (int16) (rect.bottom - rect.top + 1);
            }

            // 计算进度条的起始位置
            bar_rect.top = (int16) (rect.top + ((int32)first_item_index * (rect.bottom - rect.top + 1 - width))  \
                                    / ((int32)(progressbar_ptr->total_item_num - progressbar_ptr->items_per_page)));

            bar_rect.bottom = (int16) (bar_rect.top + width - 1);
        }

        //MS00173431, 写新短信，不断触笔输入回车，右侧滚动条显示异常
        if (first_item_index + progressbar_ptr->items_per_page >= progressbar_ptr->total_item_num
                || bar_rect.bottom > rect.bottom)
        {
            width = (int16) (bar_rect.bottom - bar_rect.top);

            bar_rect.bottom = rect.bottom;
            bar_rect.top    = (int16) (rect.bottom - width);
        }
    }
    else if (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_HORIZONTAL_WINDOWS)
    {
        bar_rect = rect;

        if (progressbar_ptr->total_item_num > progressbar_ptr->items_per_page)
        {
            width = (int16) ( (rect.right - rect.left + 1) * progressbar_ptr->items_per_page / progressbar_ptr->total_item_num);    /*lint !e737*/

            if (width < PRGBOX_MIN_BAR_LENGTH)
            {
                width = PRGBOX_MIN_BAR_LENGTH;
            }

            if (rect.right - rect.left + 1 < width)
            {
                width = (int16) (rect.right - rect.left + 1);
            }

            bar_rect.left = (int16) (rect.left + ((int32)first_item_index * (rect.right - rect.left + 1 - width))  \
                                     / ((int32)(progressbar_ptr->total_item_num - progressbar_ptr->items_per_page)));

            bar_rect.right = (int16) (bar_rect.left + width - 1);
        }

        //MS00173431, 写新短信，不断触笔输入回车，右侧滚动条显示异常
        if (first_item_index + progressbar_ptr->items_per_page >= progressbar_ptr->total_item_num
                || bar_rect.right > rect.right)
        {
            width = (int16) (bar_rect.right - bar_rect.left);

            bar_rect.right = rect.right;
            bar_rect.left  = (int16) (rect.right - width);
        }
    }

#ifdef GUIF_PRGBOX_LETTERSCROLL
    else if (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_LETTERS_SCROLL)
    {
        //字母滚动条
        bar_rect = rect;
    }

#endif

    //当条数很多时,需要调整right,bottom的值,保证right>left,bottom>top
    if (bar_rect.right <= bar_rect.left)
    {
        if (bar_rect.left < rect.right)
        {
            bar_rect.right = (int16) (bar_rect.left + 1);
        }
        else
        {
            bar_rect.right = rect.right;
            bar_rect.left  = (int16) (bar_rect.right - 1);
        }
    }

    if (bar_rect.bottom <= bar_rect.top)
    {
        if (bar_rect.top < rect.bottom)
        {
            bar_rect.bottom = (int16) (bar_rect.top + 1);
        }
        else
        {
            bar_rect.bottom = rect.bottom;
            bar_rect.top    = (int16) (bar_rect.bottom - 1);
        }
    }

    return bar_rect;
}

/*****************************************************************************/
//  Description :  Get Scroll rectangle, excluding arrow rectangle.
//  Global resource dependence : none
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetScrollRect (
    CTRLPROGRESSBAR_OBJ_T * progressbar_ptr
)
{
    GUI_RECT_T      scroll_rect = {0};
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    scroll_rect = base_ctrl_ptr->rect;

    if (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_VERTICAL_SCROLL
            || progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_VERTICAL_WINDOWS)
    {
        scroll_rect.top    = (int16) (progressbar_ptr->pre_arrow_rect.bottom + 1);
        scroll_rect.bottom = (int16) (progressbar_ptr->next_arrow_rect.top - 1);
    }
    else if (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_HORIZONTAL_SCROLL
             || progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_HORIZONTAL_WINDOWS)
    {
        scroll_rect.left  = (int16) (progressbar_ptr->pre_arrow_rect.right + 1);
        scroll_rect.right = (int16) (progressbar_ptr->next_arrow_rect.left - 1);
    }

    return scroll_rect;
}

/*****************************************************************************/
//  Description :  get current item index according to point position
//  Global resource dependence : none
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetCurrentItem (
    CTRLPROGRESSBAR_OBJ_T * progressbar_ptr,
    uint32 position
)
{
    uint32          i = 0, ret = 0;
    GUI_RECT_T      bar_rect = {0};
    uint32          start = 0, end = 0;

    ret = progressbar_ptr->total_item_num - 1;
    start = end = 0;

    for (i = 0; i < progressbar_ptr->total_item_num; i++)
    {
        bar_rect = GetScrollBarRect (progressbar_ptr, i, 0);

        if (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_VERTICAL_SCROLL)
        {
            start = bar_rect.top + PRGBOX_MIN_BAR_LENGTH / 2;
            end = bar_rect.bottom - PRGBOX_MIN_BAR_LENGTH / 2;
        }
        else if (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_HORIZONTAL_SCROLL)
        {
            start = bar_rect.left + PRGBOX_MIN_BAR_LENGTH / 2;
            end = bar_rect.right - PRGBOX_MIN_BAR_LENGTH / 2;
        }

#ifdef GUIF_PRGBOX_LETTERSCROLL
        else if (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_LETTERS_SCROLL)
        {
            start = progressbar_ptr->letter_postion_table[i];

            if (i + 1 < progressbar_ptr->total_item_num)
            {
                end =  progressbar_ptr->letter_postion_table[i + 1] - 1;
            }
            else
            {
                end = bar_rect.bottom;
            }
        }

#endif

        if ( (position >= start) && (position <= end))
        {
            ret = i;
            break;
        }
    }

    return ret;
}

/*****************************************************************************/
//  Description :  get current item index according to point position
//  Global resource dependence : none
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetFirstItem (
    CTRLPROGRESSBAR_OBJ_T	*progressbar_ptr,
    uint32            first_index,
    int32             position
)
{
    int32           i = 0;
    uint32          ret = 0;
    GUI_RECT_T      bar_rect = {0};
    int16           start = 0, end = 0;
    uint32			last_first_index = 0;

    ret = first_index;
    last_first_index = progressbar_ptr->total_item_num - progressbar_ptr->items_per_page;
    start = end = 0;

    for (i = last_first_index; i >= 0; i--)
    {
        bar_rect = GetScrollBarRect (progressbar_ptr, 0, i);

        if (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_VERTICAL_WINDOWS)
        {
            start = bar_rect.top;
            end = bar_rect.bottom;
        }
        else if (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_HORIZONTAL_WINDOWS)
        {
            start = bar_rect.left;
            end = bar_rect.right;
        }

        if ( (position >= start) && (position <= end))
        {
            ret = i;
            break;
        }
    }

    return ret;
}

/*****************************************************************************/
//  Description :  init arrow rectangle
//  Global resource dependence : none
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void InitArrowRect (
    CTRLPROGRESSBAR_OBJ_T * progressbar_ptr
)
{
    uint16              pre_arrow_height = 0;
    uint16              next_arrow_height = 0;
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    progressbar_ptr->pre_arrow_rect = base_ctrl_ptr->rect;
    progressbar_ptr->next_arrow_rect = base_ctrl_ptr->rect;

    if (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_VERTICAL_SCROLL)
    {
        GUIRES_GetImgWidthHeight (PNULL, &pre_arrow_height, progressbar_ptr->ver_scroll_theme.up_arrow_id, progressbar_ptr->win_id);
        GUIRES_GetImgWidthHeight (PNULL, &next_arrow_height, progressbar_ptr->ver_scroll_theme.down_arrow_id, progressbar_ptr->win_id);

        progressbar_ptr->pre_arrow_rect.bottom = (int16) (base_ctrl_ptr->rect.top + pre_arrow_height - 1);
        progressbar_ptr->next_arrow_rect.top   = (int16) (base_ctrl_ptr->rect.bottom - next_arrow_height + 1);
    }
    else if (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_HORIZONTAL_SCROLL)
    {
        GUIRES_GetImgWidthHeight (PNULL, &pre_arrow_height, progressbar_ptr->hor_scroll_theme.left_arrow_id, progressbar_ptr->win_id);
        GUIRES_GetImgWidthHeight (PNULL, &next_arrow_height, progressbar_ptr->hor_scroll_theme.right_arrow_id, progressbar_ptr->win_id);

        progressbar_ptr->pre_arrow_rect.right = (int16) (base_ctrl_ptr->rect.left + pre_arrow_height - 1);
        progressbar_ptr->next_arrow_rect.left = (int16) (base_ctrl_ptr->rect.right - next_arrow_height + 1);
    }
}

/*****************************************************************************/
//  Description : Get probox list pointer from control id
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL CTRLPROGRESSBAR_OBJ_T* GetProBoxPtr (
    MMI_HANDLE_T ctrl_handle
)
{
    CTRLBASE_OBJ_T* ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL == ctrl_ptr || !ProBoxTypeOf (ctrl_ptr))
    {
        ctrl_ptr = PNULL;
    }

    return (CTRLPROGRESSBAR_OBJ_T*)ctrl_ptr;
}

/*****************************************************************************/
//  Description : probox type of
//  Global resource dependence :
//  Author:James.Zhang
//  Note:该函数进行运行时类型判别
/*****************************************************************************/
LOCAL BOOLEAN ProBoxTypeOf (
    CTRLBASE_OBJ_T* probox_ptr
)
{
    return TYPEMNG_IsTypeOf(probox_ptr, CTRL_PROGRESSBAR_TYPE);
}


#ifdef GUIF_PRGBOX_LETTERSCROLL
/*==============================================================================
Description: 创建显示的层
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 在初始化及屏幕旋转的时候，都需要建立显示层
==============================================================================*/
LOCAL BOOLEAN CreateLetterBarDispLayer (CTRLPROGRESSBAR_OBJ_T *progressbar_ptr)
{
    UILAYER_CREATE_T create_info = {0};
    GUI_RECT_T disp_rect = {0};

    if (PNULL == progressbar_ptr)
    {
        return FALSE;
    }
#ifdef UI_MULTILAYER_SUPPORT
    if (!UILAYER_IsMultiLayerEnable())
    {
        SCI_PASSERT (0, ("[GuiPrgBox].Init: MultiLayer Disabled!")); /*assert verified*/
        //return FALSE;
    }

    if (UILAYER_NULL_HANDLE != progressbar_ptr->disp_layer.block_id)
    {
        return TRUE;
    }

    GetDispRect (progressbar_ptr, &disp_rect);

    create_info.lcd_id = MAIN_LCD_ID;
    create_info.owner_handle = progressbar_ptr->win_id;
    create_info.offset_x = disp_rect.left;
    create_info.offset_y = disp_rect.top;
    create_info.width = disp_rect.right - disp_rect.left;
    create_info.height = (disp_rect.bottom - disp_rect.top + 1);
    create_info.is_bg_layer = FALSE;//TRUE;
    create_info.is_static_layer = FALSE;
    create_info.format = UILAYER_MEM_DOUBLE_COPY;

    UILAYER_CreateLayer (&create_info, & (progressbar_ptr->disp_layer));
    UILAYER_SetLayerColorKey (& (progressbar_ptr->disp_layer), TRUE, UILAYER_TRANSPARENT_COLOR);
#endif
    return TRUE;
}
#endif

#if defined GUIF_PRGBOX_AUTOHIDE || defined GUIF_PRGBOX_LETTERSCROLL
PUBLIC void CTRLPROGRESSBAR_SetInvisible(CTRLPROGRESSBAR_OBJ_T *progressbar_ptr)
{
	ReleaseDispLayer(progressbar_ptr);
}
/*==============================================================================
Description: 释放显示层
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void ReleaseDispLayer (CTRLPROGRESSBAR_OBJ_T *progressbar_ptr)
{
    if (PNULL != progressbar_ptr)
    {
#if defined(GUIF_PRGBOX_AUTOHIDE)
        ClearDispInfo (progressbar_ptr);
#endif
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_RELEASELAYER (& (progressbar_ptr->disp_layer)); /*lint !e506 !e774*/
#endif
//        UILAYER_ReleaseLayer(&(progressbar_ptr->disp_layer));
//        progressbar_ptr->disp_layer.block_id = UILAYER_NULL_HANDLE;
    }

    return;
}
#endif

/*==============================================================================
Description: 获取显示区域
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 根据横竖屏获取
==============================================================================*/
LOCAL void GetDispRect (CTRLPROGRESSBAR_OBJ_T *progressbar_ptr, GUI_RECT_T *rect_ptr)
{
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    if (PNULL == progressbar_ptr || PNULL == rect_ptr)
    {
        return;
    }

    if (MMK_IsWindowLandscape (progressbar_ptr->win_id))
    {
        *rect_ptr = base_ctrl_ptr->both_rect.h_rect;
    }
    else
    {
        *rect_ptr = base_ctrl_ptr->both_rect.v_rect;
    }

    return;
}

#if defined(GUIF_PRGBOX_AUTOHIDE)
/*==============================================================================
Description: 创建显示的层
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 在初始化及屏幕旋转的时候，都需要建立显示层
==============================================================================*/
LOCAL BOOLEAN CreateDispLayer (CTRLPROGRESSBAR_OBJ_T *progressbar_ptr)
{
    UILAYER_CREATE_T create_info = {0};
    GUI_RECT_T buffer_rect = {0};

    if (PNULL == progressbar_ptr)
    {
        return FALSE;
    }
#ifdef UI_MULTILAYER_SUPPORT
    if (!UILAYER_IsMultiLayerEnable())
    {
        SCI_PASSERT (0, ("[GuiPrgBox].Init: MultiLayer Disabled!")); /*assert verified*/
        //return FALSE;
    }

    if (UILAYER_NULL_HANDLE != progressbar_ptr->disp_layer.block_id)
    {
        return TRUE;
    }

    GetDispRect (progressbar_ptr, &buffer_rect);

    if (MMI_DIRECTION_LEFT == MMITHEME_GetDisDirection())
    {
        buffer_rect.left = buffer_rect.right - progressbar_ptr->thumb_width + 1;
    }
    else
    {
        buffer_rect.right = buffer_rect.left + progressbar_ptr->thumb_width - 1;
    }

    create_info.lcd_id = MAIN_LCD_ID;
    create_info.owner_handle = progressbar_ptr->win_id;
    create_info.offset_x = buffer_rect.left;
    create_info.offset_y = buffer_rect.top;
    create_info.width = progressbar_ptr->thumb_width;
    create_info.height = (buffer_rect.bottom - buffer_rect.top + 1);
    create_info.is_bg_layer = FALSE;//TRUE;
    create_info.is_static_layer = FALSE;
    create_info.format = UILAYER_MEM_DOUBLE_COPY;

    UILAYER_CreateLayer (&create_info, & (progressbar_ptr->disp_layer));
    UILAYER_SetLayerColorKey (& (progressbar_ptr->disp_layer), TRUE, UILAYER_TRANSPARENT_COLOR);
#endif
    return TRUE;
}


/*==============================================================================
Description: 自动隐藏滚动条的tp消息处理
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
1 仅仅是 GUIPRGBOX_STYLE_VERTICAL_WINDOWS 类型
2 此处的操作相对之前的，简化了很多
==============================================================================*/
LOCAL BOOLEAN HandleAutoHideTpMsg (
    uint32                *cur_item_index_ptr, //[in/out]
    uint32                *first_item_index_ptr, //[in/out]
    GUI_POINT_T           tp_point,
    CTRLPROGRESSBAR_OBJ_T      *progressbar_ptr,
    GUIPRGBOX_RESULT_E    *handle_result_ptr, //[in/out]
    MMI_MESSAGE_ID_E      msg_id
)
{
#ifdef TOUCH_PANEL_SUPPORT
    GUI_RECT_T disp_rect = {0};
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(ProBoxTypeOf((CTRLBASE_OBJ_T*)progressbar_ptr));    /*assert verified*/
    //SCI_ASSERT(PNULL != first_item_index_ptr);/*assert verified*/
    //SCI_ASSERT(PNULL != cur_item_index_ptr);/*assert verified*/
    //SCI_ASSERT(PNULL != handle_result_ptr);/*assert verified*/
    if ( (!ProBoxTypeOf ( (CTRLBASE_OBJ_T*) progressbar_ptr)) || (PNULL == first_item_index_ptr) || (PNULL == cur_item_index_ptr) || (PNULL == handle_result_ptr))
    {
        return FALSE;
    }

    *handle_result_ptr = GUIPRGBOX_RESULT_NONE;

    GetDispRect (progressbar_ptr, &disp_rect);

    if (tp_point.y <= disp_rect.top
            || disp_rect.bottom <= tp_point.y)
    {
        return FALSE;
    }

    if (!NeedShowThumb (progressbar_ptr))
    {
        //NEWMS00193624 2.响应tp消息（在非NeedShowThumb的情况。）
        //return FALSE;
    }

    if (msg_id == MSG_TP_PRESS_DOWN)
    {
        progressbar_ptr->last_tp_pos = tp_point.y;
        progressbar_ptr->last_tp_offset = tp_point.y - base_ctrl_ptr->rect.top;
    }
    else if (msg_id == MSG_TP_PRESS_MOVE)
    {
        int16 thumb_top = tp_point.y - progressbar_ptr->last_tp_offset;
        uint32 new_first_index = GetFirstIndexByThumbTop (progressbar_ptr, &thumb_top);

        progressbar_ptr->last_tp_pos = tp_point.y;

        if (progressbar_ptr->last_tp_offset != tp_point.y - thumb_top)
        {
            progressbar_ptr->last_tp_offset = tp_point.y - thumb_top;
        }

        if (new_first_index > *first_item_index_ptr)
        {
            *handle_result_ptr = GUIPRGBOX_RESULT_NEXT_MOVE;
        }
        else if (new_first_index < *first_item_index_ptr)
        {
            *handle_result_ptr = GUIPRGBOX_RESULT_PRE_MOVE;
        }
        else
        {
            return FALSE;
        }

        *first_item_index_ptr = new_first_index;
    }
    else if (msg_id == MSG_TP_PRESS_UP)
    {
        //do nothing...
    }

#endif
    return TRUE;
}


/*==============================================================================
Description: 在新层上显示当前的滚动条
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 根据页数的多少，自动判定显示的类型，指示条还是指示滑块
==============================================================================*/
LOCAL BOOLEAN DisplayAutoHideScroll (CTRLPROGRESSBAR_OBJ_T   *progressbar_ptr)
{
    BOOLEAN         result = TRUE;
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != progressbar_ptr); /*assert verified*/
    if (PNULL == progressbar_ptr)
    {
        return FALSE;
    }
#ifdef UI_MULTILAYER_SUPPORT
    //disp_layer每次都会重画，使用双缓冲切换的方式
    UILAYER_SwitchDoubleMem (&progressbar_ptr->disp_layer, TRUE);

    UILAYER_Clear (& (progressbar_ptr->disp_layer));
#endif
    IMG_EnableTransparentColor (TRUE);
    GUI_SetAlphaOverwrite (TRUE);

    if (NeedShowThumb (progressbar_ptr))
    {
        result = GUIRES_DisplayImg (PNULL,
                                    & (base_ctrl_ptr->rect),
                                    PNULL,
                                    progressbar_ptr->win_id,
                                    progressbar_ptr->ver_scroll_theme.thumb_img_id,
                                    & (progressbar_ptr->disp_layer));
    }
    else
    {
        GUI_RECT_T disp_rect = {0};

        //set groove image and lcd display rect
        GetDispRect (progressbar_ptr, &disp_rect);

        //非内嵌在form中的情况下
        if ((0 == base_ctrl_ptr->display_rect.left)&&
            (0 == base_ctrl_ptr->display_rect.top)&&
            (0 == base_ctrl_ptr->display_rect.right)&&
            (0 == base_ctrl_ptr->display_rect.bottom))

        {
            result = TRUE;
        }    
        else
        {
            result = GUI_IntersectRect(&disp_rect, base_ctrl_ptr->display_rect, disp_rect);
        }    

        //get display rect
        if (result)
        {         
            if (MMI_DIRECTION_LEFT == MMITHEME_GetDisDirection())
            {
                disp_rect.left = disp_rect.right - progressbar_ptr->ver_scroll_theme.bar_width + 1;
            }
            else
            {
                disp_rect.right = disp_rect.left + progressbar_ptr->ver_scroll_theme.bar_width - 1;
            }
            
            result = DisplayScrollBar (disp_rect, progressbar_ptr, TRUE);
        }
    }

    progressbar_ptr->out_state = UILAYER_ALPHA_MAX;

    GUI_SetAlphaOverwrite (FALSE);
    IMG_EnableTransparentColor (FALSE);

    return (result);
}


/*==============================================================================
Description: 隐藏滚动条的显示
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 用于显示之后启动的隐藏定时器回调
==============================================================================*/
LOCAL void RemovePrgCB (uint8 timer_id, uint32 param)
{
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr = GetProBoxPtr (param);
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    if (PNULL == progressbar_ptr)
    {
        return;
    }

    if (0 == timer_id || timer_id != progressbar_ptr->remove_timer_id)
    {
        return;
    }

    progressbar_ptr->remove_timer_id = MMK_CreateTimerCallback (
                                           progressbar_ptr->ver_scroll_theme.out_tick,
                                           OutPrgCB,
                                           (uint32) base_ctrl_ptr->handle,
                                           FALSE);

    return;
}


/*==============================================================================
Description: 隐藏滚动条的动画效果
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 用于隐藏定时器启动后的隐藏效果回调
==============================================================================*/
LOCAL void OutPrgCB (uint8 timer_id, uint32 param)
{
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr = GetProBoxPtr (param);
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    if (PNULL == progressbar_ptr)
    {
        return;
    }

    if (0 == timer_id || timer_id != progressbar_ptr->remove_timer_id)
    {
        return;
    }

    progressbar_ptr->out_state -= UILAYER_ALPHA_MAX / 10;
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_WeakLayerAlpha (& (progressbar_ptr->disp_layer), progressbar_ptr->out_state);
#endif
    GUILCD_Invalidate (progressbar_ptr->disp_layer.lcd_id, 0);

    if (progressbar_ptr->out_state > UILAYER_ALPHA_MAX / 10)
    {
        progressbar_ptr->remove_timer_id = MMK_CreateTimerCallback (
                                               progressbar_ptr->ver_scroll_theme.out_tick,
                                               OutPrgCB,
                                               (uint32) base_ctrl_ptr->handle,
                                               FALSE);
    }
    else
    {
        ClearDispInfo (progressbar_ptr);

        SCI_MEMSET (& (base_ctrl_ptr->rect), 0, sizeof (base_ctrl_ptr->rect));
    }

    return;
}


/*==============================================================================
Description: 清除未完成的隐藏信息
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 中断隐藏定时器
==============================================================================*/
LOCAL void ClearDispInfo (CTRLPROGRESSBAR_OBJ_T *progressbar_ptr)
{
    if (PNULL == progressbar_ptr)
    {
        return;
    }

    if (0 != progressbar_ptr->remove_timer_id)
    {
        MMK_StopTimer (progressbar_ptr->remove_timer_id);
        progressbar_ptr->remove_timer_id = 0;
    }
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_RemoveBltLayer (& (progressbar_ptr->disp_layer));
#endif
    return;
}


/*==============================================================================
Description: 是否需要显示拖动滑块
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 一般来讲，在有滑块的时候，需要设置控制区域
==============================================================================*/
LOCAL BOOLEAN NeedShowThumb (CTRLPROGRESSBAR_OBJ_T *progressbar_ptr)
{
    uint32 page_cnt = 0;

    if (PNULL == progressbar_ptr || 0 == progressbar_ptr->items_per_page)
    {
        return FALSE;
    }

    if (progressbar_ptr->is_fobid_thumb)
    {
        return FALSE;
    }

    page_cnt = progressbar_ptr->total_item_num / progressbar_ptr->items_per_page + 1;

    return (page_cnt > progressbar_ptr->ver_scroll_theme.thumb_threshold);
    //return TRUE;
}


/*==============================================================================
Description: 根据当前thumb的top获取first index
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: GetThumbTopByFirstIndex，逆过程，有取整误差
==============================================================================*/
LOCAL uint32 GetFirstIndexByThumbTop (
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr,
    int16 *thumb_top_ptr //[in/out]
)
{
    uint32 first_index = 0;
    uint16 disp_height = 0;
    GUI_RECT_T disp_rect = {0};

    if (PNULL == progressbar_ptr || PNULL == thumb_top_ptr)
    {
        return 0;
    }

    GetDispRect (progressbar_ptr, &disp_rect);

    disp_height = disp_rect.bottom - disp_rect.top + 1;

    if (disp_height <= progressbar_ptr->thumb_height
            || progressbar_ptr->total_item_num < progressbar_ptr->items_per_page)
    {
        return 0;
    }

    if (*thumb_top_ptr <= disp_rect.top)
    {
        first_index = 0;
        *thumb_top_ptr = disp_rect.top;
    }
    else if (*thumb_top_ptr >= disp_rect.bottom - progressbar_ptr->thumb_height + 1)
    {
        first_index = progressbar_ptr->total_item_num - progressbar_ptr->items_per_page + 1;
        *thumb_top_ptr = disp_rect.bottom - progressbar_ptr->thumb_height + 1;
    }
    else
    {
        uint16 disp_pos = *thumb_top_ptr - disp_rect.top;
        uint32 scroll_cnt = progressbar_ptr->total_item_num - progressbar_ptr->items_per_page;
        uint16 scroll_height = disp_height - progressbar_ptr->thumb_height;

        first_index = disp_pos * scroll_cnt / scroll_height;
    }

    return first_index;
}


/*==============================================================================
Description: 根据当前页的first index获取thumb的top
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: GetFirstIndexByThumbTop，逆过程，有取整误差
==============================================================================*/
LOCAL uint16 GetThumbTopByFirstIndex (
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr,
    int32 first_index
)
{
    uint16 disp_height = 0;
    uint16 disp_pos = 0;
    uint32 scroll_cnt = 0;
    uint16 scroll_height = 0;
    GUI_RECT_T disp_rect = {0};

    if (PNULL == progressbar_ptr)
    {
        return 0;
    }

    GetDispRect (progressbar_ptr, &disp_rect);

    disp_height = disp_rect.bottom - disp_rect.top + 1;
    scroll_cnt = progressbar_ptr->total_item_num - progressbar_ptr->items_per_page;
    scroll_height = disp_height - progressbar_ptr->thumb_height;

    if (0 != scroll_cnt)
    {
        disp_pos = (uint32) first_index * scroll_height / scroll_cnt;
    }

    disp_pos += disp_rect.top;

    return disp_pos;
}

#endif

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author: allen
// Note: 2005.02.02
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESSBAR_ResizeRect (
    MMI_HANDLE_T handle,
    GUI_RECT_T      new_rect  // the new total rectangle
)
{
    CTRLPROGRESSBAR_OBJ_T* progressbar_ptr = GetProBoxPtr (handle);
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    if (PNULL == progressbar_ptr)
    {
        return FALSE;
    }

    if (!ProBoxTypeOf ( (CTRLBASE_OBJ_T*) progressbar_ptr))
    {
        return FALSE;
    }

    base_ctrl_ptr->rect = new_rect;

    return TRUE;
}


/*****************************************************************************/
//  Description : set progress bar rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetRect (
    MMI_HANDLE_T    ctrl_handle,
    GUI_RECT_T        *rect_ptr
)
{
    CTRLPROGRESSBAR_OBJ_T    *prg_ctrl_ptr = PNULL;

    //get progress bar pointer by control id
    prg_ctrl_ptr = GetProBoxPtr (ctrl_handle);

    if (PNULL != prg_ctrl_ptr)
    {
        CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) prg_ctrl_ptr;

        //set rect
        base_ctrl_ptr->rect = *rect_ptr;

        InitArrowRect (prg_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : set parameter: total item count and item count per page
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetParam(
    MMI_CTRL_ID_T  ctrl_id,
    uint32         total_item_count,
    uint32         items_per_page
)
{
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr = PNULL;

    //get progress box control pointer
    progressbar_ptr = GetProBoxPtr (ctrl_id);

    if (PNULL != progressbar_ptr)
    {
        SetProgressBarParamByPtr (progressbar_ptr, total_item_count, items_per_page);
    }
    else 
    {
        return;
    }
}


/*****************************************************************************/
//  Description : set parameter: total item count and item count per page
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetBothRect (
    MMI_HANDLE_T handle,
    GUI_BOTH_RECT_T     *both_rect_ptr
)
{
    CTRLPROGRESSBAR_OBJ_T* progressbar_ptr = GetProBoxPtr (handle);
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;

    if (PNULL != progressbar_ptr && PNULL !=  both_rect_ptr)
    {
#if defined GUIF_PRGBOX_AUTOHIDE || defined GUIF_PRGBOX_LETTERSCROLL
        ReleaseDispLayer (progressbar_ptr);
#endif
        base_ctrl_ptr->both_rect = *both_rect_ptr;
    }
}


/*****************************************************************************/
//  Description : set parameter: total item count and item count per page
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void SetProgressBarParamByPtr (
    CTRLPROGRESSBAR_OBJ_T   *progressbar_ptr,
    uint32                  total_item_count,
    uint32                  items_per_page
)
{
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;
#ifdef GUIF_PRGBOX_LETTERSCROLL
    uint16 font_height = 0;
    GUI_RECT_T scoll_rect = {0};
#endif

    //uint16     scale = 0;
    //uint16     one_space_height = 0;
    if (PNULL != progressbar_ptr)
    {
        //kevin.lou modified:delete assert
        //SCI_ASSERT(ProBoxTypeOf((CTRLBASE_OBJ_T*)progressbar_ptr));    /*assert verified*/
        if (!ProBoxTypeOf ( (CTRLBASE_OBJ_T*) progressbar_ptr))
        {
            return;
        }

        //set pos
#ifdef GUIF_PRGBOX_LETTERSCROLL

        if (GUIPRGBOX_STYLE_LETTERS_SCROLL == progressbar_ptr->prgbox_style)
        {
            //letters scrollbar, its item count is fixed
            font_height = GUIFONT_GetHeight (GUIPRGBOX_LETTER_FONT);

            if (MMK_IsWindowLandscape (progressbar_ptr->win_id))
            {
                //竖屏
                scoll_rect = base_ctrl_ptr->both_rect.h_rect;
            }
            else
            {
                scoll_rect = base_ctrl_ptr->both_rect.v_rect;
            }

            if ( (scoll_rect.bottom - scoll_rect.top) > (font_height* progressbar_ptr->letters_table_len))
            {
                //可以显示全字母表
                progressbar_ptr->total_item_num = progressbar_ptr->letters_table_len;
            }
            else
            {
                //不可以显示全字母表，按比例显示
                //one_space_height = (base_ctrl_ptr->both_rect.v_rect.bottom - base_ctrl_ptr->both_rect.v_rect.top)/progressbar_ptr->letters_table_len;
                //scale = (font_height/one_space_height) + 2;
                progressbar_ptr->total_item_num = (progressbar_ptr->letters_table_len) / PRGBOX_LETTER_SCALE;

                if (progressbar_ptr->total_item_num * PRGBOX_LETTER_SCALE < (progressbar_ptr->letters_table_len))
                {
                    progressbar_ptr->total_item_num++;
                }
            }
        }
        else
#endif
        {

            progressbar_ptr->total_item_num = total_item_count;
        }

        progressbar_ptr->items_per_page = items_per_page;
    }
}


/*****************************************************************************/
//  Description : set bg color
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetBgColor (
    MMI_CTRL_ID_T       ctrl_id,
    GUI_COLOR_T         bg_color
)
{
    CTRLPROGRESSBAR_OBJ_T* progressbar_ptr = GetProBoxPtr (ctrl_id);

    if (PNULL != progressbar_ptr)
    {
        //kevin.lou modified:delete assert
        //SCI_ASSERT(ProBoxTypeOf((CTRLBASE_OBJ_T*)progressbar_ptr));    /*assert verified*/
        if (!ProBoxTypeOf ( (CTRLBASE_OBJ_T*) progressbar_ptr))
        {
            return;
        }

        //set pos
        progressbar_ptr->process_theme.bg_color = bg_color;
        progressbar_ptr->process_theme.bg_type = GUI_BG_COLOR;
    }
}

/*****************************************************************************/
//  Description : 设置是否需要滑块
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:返回设置前的数据
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESSBAR_SetNeedSlideBlock (
    MMI_HANDLE_T handle,
    BOOLEAN              is_true
)
{
    CTRLPROGRESSBAR_OBJ_T* progressbar_ptr = GetProBoxPtr (handle);
    BOOLEAN is_need_slide_block = FALSE;

    if (PNULL != progressbar_ptr)
    {
        //kevin.lou modified:delete assert
        //SCI_ASSERT(ProBoxTypeOf((CTRLBASE_OBJ_T*)progressbar_ptr));    /*assert verified*/
        if (!ProBoxTypeOf ( (CTRLBASE_OBJ_T*) progressbar_ptr))
        {
            return FALSE;
        }

        is_need_slide_block = progressbar_ptr->is_need_slide_block;

        progressbar_ptr->is_need_slide_block = is_true;
    }

    return is_need_slide_block;
}

/*****************************************************************************/
//  Description : 设置是否需要滑块
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:返回设置前的数据
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetFobidThumb (
    MMI_CTRL_ID_T ctrl_id
)
{
    CTRLPROGRESSBAR_OBJ_T* progressbar_ptr = GetProBoxPtr (ctrl_id);

    if (PNULL != progressbar_ptr && ProBoxTypeOf ( (CTRLBASE_OBJ_T*) progressbar_ptr))
    {
#if defined(GUIF_PRGBOX_AUTOHIDE)
        progressbar_ptr->is_fobid_thumb = TRUE;
#endif
    }
}


/*****************************************************************************/
//  Description : 设置是否需要滑块
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:返回设置前的数据
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESSBAR_SetNeedBg (
    MMI_HANDLE_T handle,
    BOOLEAN      is_true
)
{
    BOOLEAN result = FALSE;
    CTRLPROGRESSBAR_OBJ_T* progressbar_ptr = GetProBoxPtr (handle);

    if (PNULL != progressbar_ptr)
    {
        if (!ProBoxTypeOf ( (CTRLBASE_OBJ_T*) progressbar_ptr))
        {
            return FALSE;
        }

        progressbar_ptr->is_need_bg = is_true;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : set position by control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetPos (
    BOOLEAN                 is_update,      //is update
    uint32                  cur_item_index,
    uint32			        first_item_index,
    MMI_CTRL_ID_T           ctrl_id         //control id
)
{
    uint32 max_item_index = 0;
    CTRLPROGRESSBAR_OBJ_T* progressbar_ptr = GetProBoxPtr(ctrl_id);
#if defined(GUIF_PRGBOX_AUTOHIDE)
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) progressbar_ptr;
#endif

    if (PNULL == progressbar_ptr)
    {
        return;
    }

    if (!ProBoxTypeOf ( (CTRLBASE_OBJ_T*) progressbar_ptr))
    {
        return;
    }

#ifdef GUIF_PRGBOX_LETTERSCROLL

    if ( (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_LETTERS_SCROLL))
    {
        //
    }
    else
#endif
    {
        //check current index
        if (progressbar_ptr->cur_item_index <= progressbar_ptr->total_item_num)
        {
            progressbar_ptr->cur_item_index = cur_item_index;
        }
        else
        {
            progressbar_ptr->cur_item_index = progressbar_ptr->total_item_num - 1;
        }

        //check first index
        if (first_item_index + progressbar_ptr->items_per_page <= progressbar_ptr->total_item_num)
        {
            progressbar_ptr->first_item_index = first_item_index;
        }
        else
        {
            progressbar_ptr->first_item_index = progressbar_ptr->total_item_num - progressbar_ptr->items_per_page;
        }

        if ( (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_VERTICAL_WINDOWS) ||
                (progressbar_ptr->prgbox_style == GUIPRGBOX_STYLE_HORIZONTAL_WINDOWS))
        {
            // As windows style, move count is: 'total item count' plus 'item count per page' add '1'
            max_item_index = progressbar_ptr->total_item_num - progressbar_ptr->items_per_page;

            if (cur_item_index > max_item_index)
            {
                progressbar_ptr->cur_item_index = max_item_index;
            }
        }

#if defined(GUIF_PRGBOX_AUTOHIDE)

        if (GUIPRGBOX_STYLE_VERTICAL_WINDOWS == progressbar_ptr->prgbox_style)
        {
            GUI_RECT_T ctrl_rect = {0};

            if (NeedShowThumb (progressbar_ptr))
            {
                GUI_RECT_T disp_rect = {0};

                GetDispRect (progressbar_ptr, &disp_rect);

                ctrl_rect.top = GetThumbTopByFirstIndex (progressbar_ptr, progressbar_ptr->first_item_index);
                ctrl_rect.bottom = ctrl_rect.top + progressbar_ptr->thumb_height - 1;

                if (MMI_DIRECTION_LEFT == MMITHEME_GetDisDirection())
                {
                    ctrl_rect.right = disp_rect.right;
                    ctrl_rect.left = ctrl_rect.right - progressbar_ptr->thumb_width  + 1;
                }
                else
                {
                    ctrl_rect.left = disp_rect.left;
                    ctrl_rect.right = ctrl_rect.left + progressbar_ptr->thumb_width  - 1;
                }
            }
            else
            {
                //NEWMS00193624 3.改变响应tp消息的rect（在非NeedShowThumb的情况。）
                ctrl_rect = GetScrollBarRect (
                                progressbar_ptr,
                                progressbar_ptr->cur_item_index,
                                progressbar_ptr->first_item_index
                            );
            }

            base_ctrl_ptr->rect = ctrl_rect;
        }

#endif
    }

    if (is_update)
    {
        ProgressBarDraw(progressbar_ptr);
    }
}

/*****************************************************************************/
//  Description : set progress vertical groove background image id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESSBAR_SetVerGrooveBgImg (
    MMI_CTRL_ID_T    ctrl_id,
    MMI_IMAGE_ID_T   bg_id
)
{
    BOOLEAN             result = FALSE;
    CTRLPROGRESSBAR_OBJ_T    *progressbar_ptr = PNULL;

    //check the image is valid
    if (MMITHEME_GetInvalidImageID() != bg_id)
    {
        //get progress box control pointer
        progressbar_ptr = GetProBoxPtr (ctrl_id);

        if (PNULL != progressbar_ptr)
        {
            progressbar_ptr->ver_scroll_theme.groove_bg_id = bg_id;
            result = TRUE;
        }
    }

    return (result);
}


/*****************************************************************************/
//  Description : set progress scroll bar image id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESSBAR_SetScrollBarImg (
    MMI_CTRL_ID_T    ctrl_id,
    MMI_IMAGE_ID_T   bg_id
)
{
    BOOLEAN             result = FALSE;
    CTRLPROGRESSBAR_OBJ_T    *progressbar_ptr = PNULL;

    //check the image is valid
    if (MMITHEME_GetInvalidImageID() != bg_id)
    {
        //get progress box control pointer
        progressbar_ptr = GetProBoxPtr (ctrl_id);

        if (PNULL != progressbar_ptr)
        {
            switch (progressbar_ptr->prgbox_style)
            {
            case GUIPRGBOX_STYLE_VERTICAL_SCROLL:
            case GUIPRGBOX_STYLE_VERTICAL_PROCESS:
            case GUIPRGBOX_STYLE_VERTICAL_WINDOWS:
                progressbar_ptr->ver_scroll_theme.scroll_bar_id = bg_id;
                result = TRUE;
                break;
            case GUIPRGBOX_STYLE_HORIZONTAL_SCROLL:
            case GUIPRGBOX_STYLE_HORIZONTAL_PROCESS:
            case GUIPRGBOX_STYLE_HORIZONTAL_WINDOWS:
                progressbar_ptr->hor_scroll_theme.scroll_bar_id = bg_id;
                result = TRUE;
                break;
            default:
                break;
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : set whether loop scroll
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetLoopScroll (
    MMI_HANDLE_T handle,
    BOOLEAN           is_loop_scroll
)
{
    CTRLPROGRESSBAR_OBJ_T* progressbar_ptr = GetProBoxPtr (handle);

    if (PNULL != progressbar_ptr)
    {
        if (!ProBoxTypeOf ( (CTRLBASE_OBJ_T*) progressbar_ptr))
        {
            return;
        }

        progressbar_ptr->is_loop_scroll = is_loop_scroll;
    }
}


/*****************************************************************************/
//  Description : update progress box control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_Update (
    MMI_CTRL_ID_T   ctrl_id //control id
)
{
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr = PNULL;

    //get progress box control pointer
    progressbar_ptr = GetProBoxPtr (ctrl_id);

    if (PNULL == progressbar_ptr)
    {
        return;
    }

    //update
    ProgressBarDraw (progressbar_ptr);
}

/***************************************************************************//*!
@brief 获取当前滚动条的位置所对应字母的unicode
@author Jassmine
@param      ctrl_id：［IN］滚动条的控件ID
@param      item_pos：滚动条的当前位置
@return 返回item_pos所对应的字母的unicode
@note
*******************************************************************************/
PUBLIC wchar  CTRLPROGRESSBAR_GetWcharByPos (
    MMI_HANDLE_T handle,
    uint16       item_pos
)
{
    wchar  ret_value = 0;
    uint16  scale = 1;
    CTRLPROGRESSBAR_OBJ_T* progressbar_ptr = GetProBoxPtr (handle);

    if (PNULL == progressbar_ptr || (!progressbar_ptr->is_letter_bar_valid))
    {
        return ret_value;
    }

    if (progressbar_ptr->base_letters_table != PNULL)
    {
        if (progressbar_ptr->letters_table_len > progressbar_ptr->total_item_num)
        {
            scale = PRGBOX_LETTER_SCALE;
        }

        if (item_pos < progressbar_ptr->total_item_num)
        {
            if (scale*item_pos + 1 < progressbar_ptr->letters_table_len)
            {
                ret_value = progressbar_ptr->base_letters_table[scale*item_pos];
            }
            else
            {
                ret_value = progressbar_ptr->base_letters_table[progressbar_ptr->letters_table_len -1];
            }
        }
    }

    return ret_value;
}


PUBLIC uint32 CTRLPROGRESSBAR_GetTotalItemNum (MMI_HANDLE_T handle)
{
    CTRLPROGRESSBAR_OBJ_T *  progressbar_ptr = PNULL;

    progressbar_ptr = GetProBoxPtr (handle);

    if (PNULL == progressbar_ptr) return 0;

    return progressbar_ptr->total_item_num;
}


PUBLIC uint32 CTRLPROGRESSBAR_GetFirstItemIndex (MMI_HANDLE_T handle)
{
    CTRLPROGRESSBAR_OBJ_T *  progressbar_ptr = PNULL;

    progressbar_ptr = GetProBoxPtr (handle);

    if (PNULL == progressbar_ptr) return 0;

    return progressbar_ptr->first_item_index;
}

/*****************************************************************************/
//  Description : set volume by text id 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetVolumeTextById(
										  MMI_CTRL_ID_T ctrl_id,
										  MMI_TEXT_ID_T text_id
										  )
{
    CTRLPROGRESSBAR_OBJ_T *progressbar_ptr = PNULL;    
   	
    progressbar_ptr = GetProBoxPtr (ctrl_id);        
    if (PNULL == progressbar_ptr)
        return;
	(&progressbar_ptr->process_theme)->text1_id = text_id;
}


