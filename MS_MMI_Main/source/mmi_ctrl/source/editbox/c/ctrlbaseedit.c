/*****************************************************************************
** File Name:      ctrlbaseedit.c                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012      hua.fang              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guires.h"
#include "mmi_theme.h"
#include "ctrlbaseedit.h"
#include "ctrlbase_export.h"
#include "ctrlform_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

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
#define BASEEDIT_EXTRA_AREA	36
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init baseedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlInitVtbl (
    CTRLBASEEDIT_VTBL_T        *baseedit_vtbl_ptr
);

/*****************************************************************************/
//  Description : baseedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLBASEEDIT_INIT_PARAM_T   *baseedit_param_ptr
);

/*****************************************************************************/
//  Description : baseedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

///*****************************************************************************/
////  Description : convert init param
////  Global resource dependence :
////  Author: hua.fang
////  Note:
///*****************************************************************************/
//LOCAL void BaseEditCtrlPackInitParam (
//    GUIBASEEDIT_INIT_DATA_T     *baseedit_init_ptr,   //in
//    CTRLBASEEDIT_INIT_PARAM_T   *baseedit_param_ptr   //out
//);

/*****************************************************************************/
//  Description : handle baseedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BaseEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN BaseEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : set control is circular handle up/down key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetUpDownLoop (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_circular
);

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlGetHeightByWidth (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
);

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
);

/*****************************************************************************/
//  Description : set edit is circular handle left/right key msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetLeftRightLoop (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    BOOLEAN            is_circular
);

/*****************************************************************************/
// 	Description : set control select rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlGetSelectRect (
    CTRLBASE_OBJ_T         *base_ctrl_ptr,  //in:
    GUI_RECT_T             *rect_ptr       //in/out:
);

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlGetSelectIndex (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    BOOLEAN               *is_first_ptr,  //in/out:
    BOOLEAN               *is_last_ptr    //in/out:
);

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetSelectIndex (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //in:
    BOOLEAN               is_first,         //in:
    BOOLEAN               is_last           //in:
);

/*****************************************************************************/
//  Description : set edit rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetRect (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    const CAF_RECT_T  *rect_ptr
);

/*****************************************************************************/
//  Description : set edit border
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetBorder (
    CTRLBASE_OBJ_T  *base_ctrl_ptr,
    GUI_BORDER_T    *border_ptr
);

/*****************************************************************************/
//  Description : set edit bg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetBg (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    GUI_BG_T            *bg_ptr
);

/*****************************************************************************/
//  Description : set edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetFont (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    GUI_FONT_ALL_T    *font_ptr
);

/*****************************************************************************/
//  Description : get edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlGetFont (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    GUI_FONT_ALL_T    *font_ptr
);

/*****************************************************************************/
//  Description : 获取edit的字符串内容
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL wchar* BaseEditCtrlGetDispStr (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              start_pos,
    uint16              str_len
);

/*****************************************************************************/
//  Description : handle landscape edit
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleLandscapeEdit (// is create im
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    BOOLEAN             is_tp_up            //in:
);

/*****************************************************************************/
//  Description : handle edit lose active msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBaseEditLoseActive (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    BOOLEAN             is_notify       //in
);

/*****************************************************************************/
//  Description : reset display information bar,include im icon and remaining characters
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetInfoBar (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

//for [MS]
/*****************************************************************************/
// Description : 释放字符串
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void ReleaseBaseeditLabelStr(
                           CTRLBASEEDIT_OBJ_T *baseedit_ptr
                           );
/*****************************************************************************/
// Description : 释放字符串
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void ReleaseBaseeditHintTextStr(
                           CTRLBASEEDIT_OBJ_T *baseedit_ptr
                           );
/*****************************************************************************/
// Description : get title control ptr.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL CTRLBASEEDIT_OBJ_T *GetBaseeditPtr(
                                   MMI_HANDLE_T  win_handle,
                                   MMI_CTRL_ID_T ctrl_id
                                   );
//end [MS]

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
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
)
{
    GUIEDIT_TYPE_E      type = GUIEDIT_TYPE_NULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        type = baseedit_ctrl_ptr->type;
    }

    return (type);
}


/*****************************************************************************/
//  Description : get edit style
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUIEDIT_STYLE_E CTRLBASEEDIT_GetStyle (
    MMI_CTRL_ID_T ctrl_id
)
{
    GUIEDIT_STYLE_E      style = GUIEDIT_STYLE_SINGLE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        style = baseedit_ctrl_ptr->style;
    }

    return (style);
}
/*****************************************************************************/
//  Description : set edit is display im icon and number info
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:text and list edit display,others not display
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetDispImIcon (
    MMI_CTRL_ID_T     ctrl_id,                  //in:
    BOOLEAN           is_disp_im_icon           //in:
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        baseedit_ctrl_ptr->is_disp_im_icon = is_disp_im_icon;
    }
}

/*****************************************************************************/
// 	Description : set edit is permit display border
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_PermitBorder (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_permit
)
{
    BOOLEAN             result = FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        baseedit_ctrl_ptr->is_permit_border = is_permit;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set edit bg is transparent display image
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_SetBgTransparent (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_transparent
)
{
    BOOLEAN             result = FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        baseedit_ctrl_ptr->is_bg_transparent = is_transparent;

        result = TRUE;
    }

    return (result);
}

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
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        if (GUIEDIT_CURSOR_DEFAULT_WIDTH <= margin_lr)
        {
            baseedit_ctrl_ptr->common_theme.margin_lr_space = margin_lr;
        }

        baseedit_ctrl_ptr->common_theme.margin_tb_space = margin_tb;
		baseedit_ctrl_ptr->is_line_num_cached = FALSE;
    }
}

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
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;
    GUI_FONT_ALL_T      font_all = {0};

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        font_all = baseedit_ctrl_ptr->common_theme.font;

        if (PNULL != font_size_ptr)
        {
            font_all.font = *font_size_ptr;
        }

        if (PNULL != font_color_ptr)
        {
            font_all.color = *font_color_ptr;
        }

        VTLBASE_SetFont (baseedit_ctrl_ptr, &font_all);
    }
}


/*****************************************************************************/
//  Description : Get edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_GetBg (
    MMI_CTRL_ID_T       ctrl_id,
    GUI_BG_T            *bg_ptr
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
	 *bg_ptr = baseedit_ctrl_ptr->common_theme.bg;
    }
}


/*****************************************************************************/
//  Description : Get edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_GetFont (
    MMI_CTRL_ID_T       ctrl_id,
    GUI_FONT_ALL_T         *font_all
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {

        font_all->font = baseedit_ctrl_ptr->common_theme.font.font;

       font_all->color = baseedit_ctrl_ptr->common_theme.font.color;
    }
}



/*****************************************************************************/
//  Description : Get edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_GetRect (
    MMI_CTRL_ID_T       ctrl_id,
    GUI_RECT_T*         rect_ptr
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        rect_ptr->left= baseedit_ctrl_ptr->base_ctrl.rect.left;
		rect_ptr->bottom= baseedit_ctrl_ptr->base_ctrl.rect.bottom;
        rect_ptr->top = baseedit_ctrl_ptr->base_ctrl.rect.top;
		rect_ptr->right= baseedit_ctrl_ptr->base_ctrl.rect.right;

    }
}


/*****************************************************************************/
//  Description : Get edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLBASEEDIT_GetIMHeight(MMI_CTRL_ID_T ctrl_id)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
	if(PNULL != baseedit_ctrl_ptr)
	{
		return 30;
	}
   return 0;
}

/*****************************************************************************/
//  Description : Get edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_GetDisplayRect (
    MMI_CTRL_ID_T       ctrl_id,
    GUI_RECT_T        *rect_ptr
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {

        rect_ptr->left= baseedit_ctrl_ptr->base_ctrl.display_rect.left;
		rect_ptr->bottom= baseedit_ctrl_ptr->base_ctrl.display_rect.bottom;
        rect_ptr->top = baseedit_ctrl_ptr->base_ctrl.display_rect.top;
		rect_ptr->right= baseedit_ctrl_ptr->base_ctrl.display_rect.right;
    }
}
/*****************************************************************************/
//  Description : set edit align
//  Global resource dependence :
//  Author: Jassmine
//  Note: only for single line
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetAlign (
    MMI_CTRL_ID_T  ctrl_id,//in
    GUI_ALIGN_E    align   //in
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if ((PNULL != baseedit_ctrl_ptr) &&
        ((GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)||(GUIEDIT_STYLE_SINGLE_UP== baseedit_ctrl_ptr->style)))
    {
        VTLBASEEDIT_SetAlign (baseedit_ctrl_ptr, align);
    }
}

/*****************************************************************************/
//  Description : set edit rect is adaptive
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetRectAdaptive(
                                         MMI_CTRL_ID_T  ctrl_id,//in
                                         BOOLEAN        is_true //in:
                                         )
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        baseedit_ctrl_ptr->is_rect_adaptive = is_true;
    }
}

/*****************************************************************************/
// Description : Set timedate_editor_displaytype 
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetTimeDateEditorDisplayType(
                                     CTRLBASEEDIT_OBJ_T    *baseedit_ptr,
                                     TIMEDATEEDITOR_DISPLAY_TYPE_E display_type
                                     )
{
    if (PNULL != baseedit_ptr)
    {
        baseedit_ptr->timedate_editor_displaytype = display_type;
    }
}

/*****************************************************************************/
// Description : Get timedate_editor_displaytype 
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC TIMEDATEEDITOR_DISPLAY_TYPE_E CTRLBASEEDIT_GetTimeDateEditorDisplayType(
                                     CTRLBASEEDIT_OBJ_T    *baseedit_ptr
                                     )
{
    if (PNULL != baseedit_ptr)
    {
       return baseedit_ptr->timedate_editor_displaytype;
    }
    else 
    {
        return TIMEDATE_DEFAUL_DISPLAYTYPE;
    }
}

/*****************************************************************************/
//  Description : get baseedit type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T BASEEDIT_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,                                     // parent class type
                       "baseedit",                                         // control name
                       sizeof (CTRLBASEEDIT_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) BaseEditCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) BaseEditCtrlDestruct,      // control destructor function
                       0,                                                  // size of init param struct
                       PNULL,                                              // convert init param
                       sizeof (CTRLBASEEDIT_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) BaseEditCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : get baseedit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC CTRLBASEEDIT_OBJ_T* BASEEDIT_GetPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (BaseEditTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLBASEEDIT_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : resize edit rect by im rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_ResizeRect (//edit is change
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr  //in
)
{
    BOOLEAN         result = FALSE;
    GUI_RECT_T      edit_rect = {0};
    GUI_RECT_T      im_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    //set edit rect
    edit_rect = base_ctrl_ptr->rect;

    //reset rect
    base_ctrl_ptr->rect = baseedit_ctrl_ptr->original_rect;

    //get im rect
    if ((0 != baseedit_ctrl_ptr->im_ctrl_handle) &&
        (GUIIM_GetImRect (baseedit_ctrl_ptr->im_ctrl_handle, &im_rect)))
    {
        // 输入法给的区域，有可能im_rect.top == im_rect.bottom，对于输入法，它认为
        // 是无效区域，但给form后，会计算这个区域，导致错误
        if (GUI_IsInvalidRect(im_rect))
        {
            SCI_MEMSET(&im_rect, 0, sizeof(im_rect));
        }

        if (BASEEDIT_IsParentCtrl(baseedit_ctrl_ptr))
        {
            //im rect is not in edit
            CTRLMSG_SendNotifyEx (base_ctrl_ptr->handle, MSG_NOTIFY_RESIZE, &im_rect);
        }
		else if(baseedit_ctrl_ptr->is_inline_editor && baseedit_ctrl_ptr->original_rect.top < 0)
		{
			//doing nothing for inline editor
		}
        else
        {
            //modify edit rect
            if ((im_rect.top > baseedit_ctrl_ptr->original_rect.top) &&
                (im_rect.top <= baseedit_ctrl_ptr->original_rect.bottom))
            {
                base_ctrl_ptr->rect.bottom = (int16) (im_rect.top - 1);
            }
        }
    }

    //edit is change
    if (!GUI_EqualRect (edit_rect, base_ctrl_ptr->rect))
    {
        // virtual method to handle rect changed
        baseedit_ctrl_ptr->is_line_num_cached = FALSE;
        VTLBASEEDIT_OnRectChanged (baseedit_ctrl_ptr);
#ifdef UI_MULTILAYER_SUPPORT
        if(baseedit_ctrl_ptr->is_inline_editor)
        {
        	UILAYER_ReleaseLayerExt(&baseedit_ctrl_ptr->edit_layer_info); 
        }
        UILAYER_ReleaseLayerExt(&baseedit_ctrl_ptr->label_layer_info);
        //UILAYER_ReleaseLayerExt(&baseedit_ctrl_ptr->theme_line_layer_info);
 #endif
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : init baseedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlInitVtbl (
    CTRLBASEEDIT_VTBL_T        *baseedit_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) baseedit_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = BaseEditCtrlHandleMsg;
    base_ctrl_vtbl_ptr->SetRect = BaseEditCtrlSetRect;
    base_ctrl_vtbl_ptr->SetBorder = BaseEditCtrlSetBorder;
    base_ctrl_vtbl_ptr->SetBg = BaseEditCtrlSetBg;
    base_ctrl_vtbl_ptr->SetFont = BaseEditCtrlSetFont;
    base_ctrl_vtbl_ptr->GetFont = BaseEditCtrlGetFont;

    // 以下接口用于实现内嵌form
    base_ctrl_vtbl_ptr->GetHeightByWidth = BaseEditCtrlGetHeightByWidth;
    base_ctrl_vtbl_ptr->SetDisplayRect = BaseEditCtrlSetDisplayRect;
    base_ctrl_vtbl_ptr->SetLeftRightLoop = BaseEditCtrlSetLeftRightLoop;
    base_ctrl_vtbl_ptr->SetUpDownLoop = BaseEditCtrlSetUpDownLoop;
    base_ctrl_vtbl_ptr->GetSelectRect = BaseEditCtrlGetSelectRect;
    base_ctrl_vtbl_ptr->GetSelectIndex = BaseEditCtrlGetSelectIndex;
    base_ctrl_vtbl_ptr->SetSelectIndex = BaseEditCtrlSetSelectIndex;

    // 本类的虚方法
    baseedit_vtbl_ptr->SetCursorPos = BaseEditCtrlSetCursorPos;
    baseedit_vtbl_ptr->SetHighlight = BaseEditCtrlSetHighlight;
    baseedit_vtbl_ptr->GetDispStr = BaseEditCtrlGetDispStr;
    baseedit_vtbl_ptr->OnCursorChanged = BaseEditCtrlOnCursorChanged;

    return TRUE;
}

/*****************************************************************************/
//  Description : baseedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlConstruct (
    CTRLBASE_OBJ_T              *base_ctrl_ptr,
    CTRLBASEEDIT_INIT_PARAM_T   *baseedit_param_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == baseedit_param_ptr)
    {
        return FALSE;
    }

    //set edit style
    baseedit_ctrl_ptr->style = GUIEDIT_STYLE_MULTI;

    // TODO: this should not be here
    //set edit display direction
    baseedit_ctrl_ptr->display_dir = GUIEDIT_DISPLAY_DIR_LT;

    //set edit rect is adaptive
    baseedit_ctrl_ptr->is_rect_adaptive = baseedit_param_ptr->is_rect_adaptive;

    baseedit_ctrl_ptr->is_inline_editor=baseedit_param_ptr->is_inline_editor;
    //set edit rect and type
    baseedit_ctrl_ptr->type          = baseedit_param_ptr->type;
    baseedit_ctrl_ptr->original_rect = base_ctrl_ptr->both_rect.v_rect;

    //set edit get active create im
    baseedit_ctrl_ptr->is_create_im_active = TRUE;

    //set edit permit display border
    baseedit_ctrl_ptr->is_permit_border = TRUE;
    baseedit_ctrl_ptr->is_update_borderColor = FALSE;

	baseedit_ctrl_ptr->is_line_num_cached = FALSE;
	baseedit_ctrl_ptr->cache_total_line_num = 1;
#ifdef ZDT_EDIT_NUM_IMG_SUPPORT
		baseedit_ctrl_ptr->use_phnum_img = FALSE;
#endif
    //set string max len
	if (GUIEDIT_TYPE_PHONENUM == baseedit_ctrl_ptr->type)
	{
		baseedit_ctrl_ptr->str_max_len = (uint16)MAX(baseedit_param_ptr->str_max_len + 1,1);
	}
	else
	{
		baseedit_ctrl_ptr->str_max_len = (uint16)MAX(baseedit_param_ptr->str_max_len,1);
	}

    //set cursor
    baseedit_ctrl_ptr->cursor.x_coordinate = GUIEDIT_CURSOR_INVALID_COORDINATE;

    //set no highlight
    BASEEDIT_SetNoHighlight (baseedit_ctrl_ptr, baseedit_ctrl_ptr->cursor.cur_pos);

    //set line
    baseedit_ctrl_ptr->total_line_num = 1;

    //get edit theme
    MMITHEME_GetEditTheme (&baseedit_ctrl_ptr->common_theme);
    SCI_ASSERT (baseedit_ctrl_ptr->common_theme.dividing_line_width <= baseedit_ctrl_ptr->common_theme.line_space); /*assert verified*/
    SCI_ASSERT (GUIEDIT_CURSOR_DEFAULT_WIDTH <= baseedit_ctrl_ptr->common_theme.margin_lr_space); /*assert verified*/

    base_ctrl_ptr->lcd_dev_info = baseedit_ctrl_ptr->common_theme.lcd_dev;
    if(!baseedit_ctrl_ptr->is_inline_editor)
    {
        baseedit_ctrl_ptr->edit_layer_info = base_ctrl_ptr->lcd_dev_info;
    }

    return (result);
}

/*****************************************************************************/
//  Description : baseedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN                 result = TRUE;
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) base_ctrl_ptr;
    GUIEDIT_CONFIG_KEY_T    *im_config_ptr = PNULL;
    GUIEDIT_CONFIG_KEY_T    *im_config_next_ptr = PNULL;

    if (PNULL == baseedit_ctrl_ptr)
    {
        return FALSE;
    }

    //free edit string memory
    if (PNULL != baseedit_ctrl_ptr->str_ptr)
    {
        SCI_FREE (baseedit_ctrl_ptr->str_ptr);
    }

    //free line info memory
    if (PNULL != baseedit_ctrl_ptr->line_info_ptr)
    {
        SCI_FREE (baseedit_ctrl_ptr->line_info_ptr);
    }

    //free im config key memory
    im_config_ptr = baseedit_ctrl_ptr->im_config_key_ptr;

    while (PNULL != im_config_ptr)
    {
        im_config_next_ptr = im_config_ptr->next_ptr;

        //free
        SCI_FREE (im_config_ptr->value_ptr);
        SCI_FREE (im_config_ptr);

        im_config_ptr = im_config_next_ptr;
    }

    baseedit_ctrl_ptr->im_config_key_ptr = PNULL;

    //free im control memory
    if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
    {
        baseedit_ctrl_ptr->im_ctrl_handle = 0;
    }

	//	for [MS]
	if((GUIEDIT_STYLE_MULTI_DYN_DOWN == baseedit_ctrl_ptr->style)
		||(GUIEDIT_STYLE_MULTI_DYN_UP == baseedit_ctrl_ptr->style)

		||(GUIEDIT_STYLE_SINGLE_DYN_DOWN == baseedit_ctrl_ptr->style)
		||(GUIEDIT_STYLE_SINGLE_UP == baseedit_ctrl_ptr->style)
		||baseedit_ctrl_ptr->is_inline_editor)
	{    
#ifdef UI_MULTILAYER_SUPPORT	
		uint8 *label_layer_buf_ptr = PNULL;
		uint8 *hint_text_layer_buf_ptr = PNULL;

		uint8 *edit_layer_buf_ptr = PNULL;

		label_layer_buf_ptr = UILAYER_GetLayerBufferPtr(&baseedit_ctrl_ptr->label_layer_info);
		hint_text_layer_buf_ptr = UILAYER_GetLayerBufferPtr(&baseedit_ctrl_ptr->hint_text_layer_info);

		if(baseedit_ctrl_ptr->is_inline_editor)
		{
			edit_layer_buf_ptr = UILAYER_GetLayerBufferPtr(&baseedit_ctrl_ptr->edit_layer_info);
		}

		if(PNULL == label_layer_buf_ptr)
		{
			UILAYER_ReleaseLayer(&baseedit_ctrl_ptr->label_layer_info);
		}
		if(PNULL == hint_text_layer_buf_ptr)
		{
			UILAYER_ReleaseLayer(&baseedit_ctrl_ptr->hint_text_layer_info);
		}

        /*

		if(PNULL == theme_line_layer_buf_ptr)
		{
			UILAYER_ReleaseLayer(&baseedit_ctrl_ptr->theme_line_layer_info);
		}

		*/
        if(PNULL == edit_layer_buf_ptr)
        {
        	UILAYER_ReleaseLayerExt(&baseedit_ctrl_ptr->edit_layer_info);
        }                          
#endif
	}
	// end [MS]
    return (result);
}


LOCAL MMI_RESULT_E INLINEEDIT_HandleNumKey(MMI_MESSAGE_ID_E      msg_id, MMI_RESULT_E        ret)//FOR INLINE EDITOR, java doesn't want deal these keys.
{
	 MMI_RESULT_E        result = ret;
	switch (msg_id)	
	{
    case MSG_APP_1:
    case MSG_KEYUP_1:
    case MSG_APP_2:
    case MSG_KEYUP_2:
    case MSG_APP_3:
    case MSG_KEYUP_3:
    case MSG_APP_4:
    case MSG_KEYUP_4:
    case MSG_APP_5:
    case MSG_KEYUP_5:
    case MSG_APP_6:
    case MSG_KEYUP_6:
    case MSG_APP_7:
    case MSG_KEYUP_7:
    case MSG_APP_8:
    case MSG_KEYUP_8:
    case MSG_APP_9:
    case MSG_KEYUP_9:
    case MSG_APP_0:
    case MSG_KEYUP_0:
		{
			result = MMI_RESULT_TRUE;
		}
		break;
	default:
		break;
	}
	return result;
}

/*****************************************************************************/
//  Description : handle baseedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BaseEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = PNULL;

    if (PNULL == baseedit_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_OPEN:
        //set edit handle right softkey long msg
        BASEEDIT_SetSoftkeyBtnState (baseedit_ctrl_ptr, 2, FALSE, TRUE);
        break;

    case MSG_CTL_GET_FOCUS:
        if(baseedit_ctrl_ptr != PNULL && baseedit_ctrl_ptr->is_inline_editor)
        {
        	CreateBaseeditEditLayer(baseedit_ctrl_ptr, &(base_ctrl_ptr->display_rect));
        }
        break;
    case MSG_CTL_LOSE_FOCUS:
        // 过滤此消息，不让父类处理
#ifdef UI_MULTILAYER_SUPPORT	
        if(baseedit_ctrl_ptr != PNULL && baseedit_ctrl_ptr->is_inline_editor)
        {
        	UILAYER_ReleaseLayerExt(&baseedit_ctrl_ptr->edit_layer_info);        
        }
#endif
        break;

    case MSG_CTL_GET_ACTIVE:
        base_ctrl_ptr->is_active = TRUE;

        //landscape edit for PDA
        if ((HandleLandscapeEdit (baseedit_ctrl_ptr, FALSE)) &&
            (baseedit_ctrl_ptr->is_create_im_active))
        {
            //create im
            BASEEDIT_CreateImCtrl (baseedit_ctrl_ptr);
        }

        //notify parent control
        result = CTRLMSG_SendNotify (base_ctrl_ptr->handle, MSG_NOTIFY_GET_ACTIVE);

        //resize edit rect
        BASEEDIT_ResizeRect (baseedit_ctrl_ptr);

		if ((GUIEDIT_STYLE_MULTI_DYN_DOWN == baseedit_ctrl_ptr->style) 
			|| (GUIEDIT_STYLE_MULTI_DYN_UP == baseedit_ctrl_ptr->style)

			||(GUIEDIT_STYLE_SINGLE_DYN_DOWN == baseedit_ctrl_ptr->style)
			||(GUIEDIT_STYLE_SINGLE_UP == baseedit_ctrl_ptr->style)
            ||baseedit_ctrl_ptr->is_inline_editor)
		{
			CTRLBASE_SetActiveEx(base_ctrl_ptr->ctrl_id,TRUE,FALSE); //[MS]
		}

		if(GUIEDIT_STYLE_SINGLE_DYN_DOWN == baseedit_ctrl_ptr->style) 
		{
			if (0 != MMK_GetParentCtrlHandle (base_ctrl_ptr->handle))
			{
				GUIFORM_NOTIFY_LINE_T   notify_line = {0};
				//set notify param
				notify_line.is_line_num_change = TRUE;
				notify_line.ctrl_handle = base_ctrl_ptr->handle;
				CTRLMSG_SendNotifyEx (base_ctrl_ptr->handle, MSG_NOTIFY_LINE_CHANGE, &notify_line);
				
				result = TRUE;
			}
		}
        break;

    case MSG_CTL_LOSE_ACTIVE:
        result = HandleBaseEditLoseActive (baseedit_ctrl_ptr, FALSE);
		
		if ((GUIEDIT_STYLE_MULTI_DYN_DOWN == baseedit_ctrl_ptr->style) 
			|| (GUIEDIT_STYLE_MULTI_DYN_UP == baseedit_ctrl_ptr->style)
			||(GUIEDIT_STYLE_SINGLE_DYN_DOWN == baseedit_ctrl_ptr->style)
			||(GUIEDIT_STYLE_SINGLE_UP == baseedit_ctrl_ptr->style)
            ||baseedit_ctrl_ptr->is_inline_editor)
		{
			CTRLBASE_SetActiveEx(base_ctrl_ptr->ctrl_id,FALSE,FALSE);		// [MS]
		}

        break;

    case MSG_NOTIFY_LOSE_ACTIVE:
        result = HandleBaseEditLoseActive (baseedit_ctrl_ptr, TRUE);

		if(GUIEDIT_STYLE_SINGLE_DYN_DOWN == baseedit_ctrl_ptr->style) 
		{
			if (0 != MMK_GetParentCtrlHandle (base_ctrl_ptr->handle))
			{
				GUIFORM_NOTIFY_LINE_T   notify_line = {0};
				//set notify param
				notify_line.is_line_num_change = TRUE;
				notify_line.ctrl_handle = base_ctrl_ptr->handle;
				CTRLMSG_SendNotifyEx (base_ctrl_ptr->handle, MSG_NOTIFY_LINE_CHANGE, &notify_line);
				
				result = TRUE;
			}
		}
        break;

    case MSG_CTL_PAINT:
        if (VTLBASE_GetVisible (base_ctrl_ptr))
        {
            if(baseedit_ctrl_ptr != PNULL && baseedit_ctrl_ptr->is_inline_editor && MMK_IsFocusWin(base_ctrl_ptr->win_handle))
            {
                CreateBaseeditEditLayer(baseedit_ctrl_ptr, &(base_ctrl_ptr->display_rect));
            }            
            VTLBASEEDIT_DisplayAll (
                    baseedit_ctrl_ptr,
                    MMK_IsWinOnPaint(base_ctrl_ptr->win_handle) ? FALSE : TRUE);
        }
        break;
       case MSG_CTL_UPDATE_COLOR_THEME:
		if(!baseedit_ctrl_ptr->is_inline_editor)
		{
            baseedit_ctrl_ptr->common_theme.bg.color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT);
			baseedit_ctrl_ptr->common_theme.theme_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT); 
		}
	break;    
			
#ifdef GUIF_EDIT_PDASTYLE
    case MSG_LCD_SWITCH:
        if (MMK_IsWindowLandscape (base_ctrl_ptr->win_handle))
        {
            BASEEDIT_DestroyImCtrl (baseedit_ctrl_ptr);
        }
#if defined(KEYPAD_TYPE_TYPICAL_KEYPAD) || defined(KEYPAD_TYPE_QWERTY_KEYPAD)
        else if (baseedit_ctrl_ptr->is_create_im_active)
        {
            BASEEDIT_CreateImCtrl (baseedit_ctrl_ptr);
        }

#endif
        break;
#endif

    case MSG_APP_1:
    case MSG_KEYUP_1:
    case MSG_APP_2:
    case MSG_KEYUP_2:
    case MSG_APP_3:
    case MSG_KEYUP_3:
    case MSG_APP_4:
    case MSG_KEYUP_4:
    case MSG_APP_5:
    case MSG_KEYUP_5:
    case MSG_APP_6:
    case MSG_KEYUP_6:
    case MSG_APP_7:
    case MSG_KEYUP_7:
    case MSG_APP_8:
    case MSG_KEYUP_8:
    case MSG_APP_9:
    case MSG_KEYUP_9:
    case MSG_APP_STAR:
    case MSG_KEYUP_STAR:
    case MSG_APP_0:
    case MSG_KEYUP_0:
    case MSG_APP_HASH:
    case MSG_KEYUP_HASH:
#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    case MSG_APP_Q:
    case MSG_APP_W:
    case MSG_APP_E:
    case MSG_APP_R:
    case MSG_APP_T:
    case MSG_APP_Y:
    case MSG_APP_U:
    case MSG_APP_I:
    case MSG_APP_O:
    case MSG_APP_P:
    case MSG_APP_A:
    case MSG_APP_S:
    case MSG_APP_D:
    case MSG_APP_F:
    case MSG_APP_G:
    case MSG_APP_H:
    case MSG_APP_J:
    case MSG_APP_K:
    case MSG_APP_L:
    case MSG_APP_DEL:
    case MSG_APP_Z:
    case MSG_APP_X:
    case MSG_APP_C:
    case MSG_APP_V:
    case MSG_APP_B:
    case MSG_APP_N:
    case MSG_APP_M:
    case MSG_APP_COMMA:
    case MSG_APP_PERIOD:
    case MSG_APP_ENTER:
    case MSG_APP_FN:
    case MSG_APP_SHIFT:
    case MSG_APP_AT_QWERTY:
    case MSG_APP_SPACE:
    case MSG_APP_AND:
    case MSG_APP_QUESTION:
    case MSG_APP_CTRL:
    case MSG_APP_PLUS:
    case MSG_APP_LPARENTHESIS:
    case MSG_APP_RPARENTHESIS:
    case MSG_APP_MINUS:
    case MSG_APP_DQUOTES:
    case MSG_APP_SEMICOLON:
    case MSG_APP_COLON:
    case MSG_APP_SLASH:
    case MSG_APP_SAND:
    case MSG_APP_EXCLAMATION:
#endif
        if (0 == baseedit_ctrl_ptr->im_ctrl_handle &&
            !MMK_IsWindowLandscape (base_ctrl_ptr->win_handle))
        {
            BASEEDIT_CreateImCtrl (baseedit_ctrl_ptr);
        }

        if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
        {
            result = GUIIM_HandleMsg (baseedit_ctrl_ptr->im_ctrl_handle, msg_id, PNULL);
        }
        if(baseedit_ctrl_ptr->is_inline_editor)
        {
            result = INLINEEDIT_HandleNumKey(msg_id, result);
        }
        break;

    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        result = BASEEDIT_HandleSoftkeyMsg (baseedit_ctrl_ptr, msg_id, param);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        if ((!MMK_GET_TP_SLIDE (param)) &&
            (base_ctrl_ptr->is_active))
        {
            GUIIM_INPUT_TYPE_T default_input = {0};

            //landscape edit for PDA
            if ((0 == baseedit_ctrl_ptr->im_ctrl_handle) &&
                HandleLandscapeEdit (baseedit_ctrl_ptr, TRUE))
            {
                BASEEDIT_CreateImCtrl (baseedit_ctrl_ptr);
                BASEEDIT_HandleImSwitch (baseedit_ctrl_ptr);
            }

            //非触笔输入、可切换至触笔输入->切换至触笔输入
            if (GUIIM_CanSwithToTp (baseedit_ctrl_ptr->im_ctrl_handle, &default_input))
            {
                GUIIM_SetInput (baseedit_ctrl_ptr->im_ctrl_handle, default_input);
            }
        }
        break;
#endif

    case MSG_NOTIFY_IM_PAINT:
        //update window
        MMK_SendMsg (base_ctrl_ptr->win_handle, MSG_FULL_PAINT, PNULL);
        break;

    case MSG_NOTIFY_IM_SWICTH:
    case MSG_NOTIFY_IM_CHANGE_RECT:
        BASEEDIT_HandleImSwitch (baseedit_ctrl_ptr);
        break;

    case MSG_NOTIFY_IM_HIDE:
        BASEEDIT_DestroyImCtrl (baseedit_ctrl_ptr);

        if (! (baseedit_ctrl_ptr->is_rect_adaptive))
        {
            BASEEDIT_HandleImSwitch (baseedit_ctrl_ptr);
        }

//#ifdef GUIF_CLIPBOARD
//        else
//        {
//            // fix NEWMS00174274
//            BASEFLEX_ShowSlider(edit_ctrl_ptr);
//        }
//#endif
        break;

    case MSG_NOTIFY_IM_REFRESH_SOFTKEY:
        BASEEDIT_DisplaySoftkey (baseedit_ctrl_ptr);
        break;

    default:
        parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_BASEEDIT_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
        result = parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set edit rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetRect (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    const CAF_RECT_T  *rect_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    if ((!GUI_IsRectEmpty (*rect_ptr)) &&
        (!GUI_EqualRect (base_ctrl_ptr->rect, *rect_ptr)))
    {
        //set rect
        base_ctrl_ptr->rect          = *rect_ptr;
        base_ctrl_ptr->display_rect  = *rect_ptr;
        baseedit_ctrl_ptr->original_rect = *rect_ptr;

        // fix NEWMS00197533
        baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;
        baseedit_ctrl_ptr->is_line_num_cached = FALSE;
        GUI_SetVOrHRect (base_ctrl_ptr->handle, (GUI_RECT_T *) rect_ptr, & (base_ctrl_ptr->both_rect));

        // virtual method for rect changed
        VTLBASEEDIT_OnRectChanged (baseedit_ctrl_ptr);
#ifdef UI_MULTILAYER_SUPPORT	
        if(baseedit_ctrl_ptr->is_inline_editor)
        {
        	UILAYER_ReleaseLayerExt(&baseedit_ctrl_ptr->edit_layer_info);
        }
        //CreateBaseeditEditLayer(baseedit_ctrl_ptr, rect_ptr);
        UILAYER_ReleaseLayerExt(&baseedit_ctrl_ptr->label_layer_info);
        //UILAYER_ReleaseLayerExt(&baseedit_ctrl_ptr->theme_line_layer_info);        
#endif
    }

    return (result);
}

/*****************************************************************************/
//  Description : set edit border
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetBorder (
    CTRLBASE_OBJ_T  *base_ctrl_ptr,
    GUI_BORDER_T    *border_ptr
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    baseedit_ctrl_ptr->common_theme.border = *border_ptr;
    baseedit_ctrl_ptr->is_line_num_cached = FALSE;
    return TRUE;
}

/*****************************************************************************/
//  Description : set edit bg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetBg (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    GUI_BG_T            *bg_ptr
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    baseedit_ctrl_ptr->common_theme.bg = *bg_ptr;

    return TRUE;
}

/*****************************************************************************/
//  Description : set edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetFont (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    GUI_FONT_ALL_T    *font_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    // font color
    baseedit_ctrl_ptr->common_theme.font.color = font_ptr->color;

    // font size
    if (baseedit_ctrl_ptr->common_theme.font.font != font_ptr->font)
    {
        baseedit_ctrl_ptr->common_theme.font.font = font_ptr->font;
        baseedit_ctrl_ptr->is_line_num_cached = FALSE;
        //notify edit change
       BASEEDIT_NotifyLineMsg (baseedit_ctrl_ptr, 0);
	   
    }

    return (result);
}

/*****************************************************************************/
//  Description : get edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlGetFont (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    GUI_FONT_ALL_T    *font_ptr
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    *font_ptr = baseedit_ctrl_ptr->common_theme.font;

    return TRUE;
}

/*****************************************************************************/
//  Description : set edit is circular handle left/right key msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetLeftRightLoop (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    BOOLEAN            is_circular
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    baseedit_ctrl_ptr->common_theme.is_cir_handle_lf = is_circular;

    return TRUE;
}

/*****************************************************************************/
// 	Description : set control is circular handle up/down key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetUpDownLoop (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_circular
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    baseedit_ctrl_ptr->common_theme.is_cir_handle_ud = is_circular;

    return TRUE;
}

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlGetHeightByWidth (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
)
{
    BOOLEAN         result = TRUE;
    BOOLEAN         is_border = TRUE;
    uint16          line_height = 0;
    uint16          edit_width = 0;
    uint16          total_line_num = 0;
    int32           real_height = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    //test whether exist border
    if ((!baseedit_ctrl_ptr->is_permit_border) ||
        (GUI_BORDER_NONE == baseedit_ctrl_ptr->common_theme.border.type))
    {
        is_border = FALSE;
    }

    //get line height
    line_height = BASEEDIT_GetLineHeight(baseedit_ctrl_ptr);

    //get text box width

    if (baseedit_ctrl_ptr-> seperate_line == GUIEDIT_MMS_SEPERATE_LINE)
    {
        edit_width = (uint16) width;
    }
    else
    {
        // should align BASEFLEX_CreateOrDestroyPrgCtrl edit_width
        edit_width = (uint16) (width - GUIEDIT_LEFT_MARGIN - GUIEDIT_RIGHT_MARGIN +1);
    }

    if (is_border)
    {
        edit_width = (uint16) (edit_width - (baseedit_ctrl_ptr->common_theme.border.width << 1));
    }

    //get edit string line number
    total_line_num = VTLBASEEDIT_GetLineNum (baseedit_ctrl_ptr, edit_width);

    //must one line
    if (0 == total_line_num)
    {
        total_line_num = 1;
    }

    //get text width and height
	if(GUIEDIT_STYLE_SINGLE_DYN_DOWN == baseedit_ctrl_ptr->style)
	{
		if(TRUE == base_ctrl_ptr->is_active)
		{
			real_height = (int32) line_height * total_line_num;
		}
		else
		{
			real_height = (int32) line_height;	
		}
	}
	else
	{
    real_height = (int32) line_height * total_line_num;
	}

    //add margin
    real_height = real_height + (baseedit_ctrl_ptr->common_theme.margin_tb_space << 1);

    //add border
    if (is_border)
    {
        real_height = real_height + (baseedit_ctrl_ptr->common_theme.border.width << 1);
    }

    if (real_height < 0x7fff)
    {
    	if ((GUIEDIT_STYLE_MULTI_DYN_DOWN == baseedit_ctrl_ptr->style) 
		|| (GUIEDIT_STYLE_MULTI_DYN_UP == baseedit_ctrl_ptr->style)

		||(GUIEDIT_STYLE_SINGLE_DYN_DOWN == baseedit_ctrl_ptr->style)
		||(GUIEDIT_STYLE_SINGLE_UP == baseedit_ctrl_ptr->style)
        ||baseedit_ctrl_ptr->is_inline_editor)
    	{
    	    	*height_ptr = (uint16) real_height + BASEEDIT_EXTRA_AREA; // [MS] real height + extra area
    	}
	else
	{
        *height_ptr = (uint16) real_height;
    }
    }
    else
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
)
{
    BOOLEAN             result = TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    if (!GUI_EqualRect (base_ctrl_ptr->display_rect, *rect_ptr))
    {
        //set display rect
        base_ctrl_ptr->display_rect = *rect_ptr;
        baseedit_ctrl_ptr->is_line_num_cached = FALSE;
        // fix NEWMS00197533
        baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;
    }

    if (is_update)
    {
        //update edit
        VTLBASEEDIT_DisplayAll (baseedit_ctrl_ptr, TRUE);
    }
    /*
	else
	{
		if(!base_ctrl_ptr->is_active && (0 != baseedit_ctrl_ptr->theme_line_layer_info.block_id))
		{
			UILAYER_SetLayerColorKey(&(baseedit_ctrl_ptr->theme_line_layer_info), TRUE, UILAYER_TRANSPARENT_COLOR);
			UILAYER_Clear(&(baseedit_ctrl_ptr->theme_line_layer_info));		
		}	
	}
	*/

    return (result);
}

/*****************************************************************************/
// 	Description : set control select rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlGetSelectRect (
    CTRLBASE_OBJ_T         *base_ctrl_ptr,  //in:
    GUI_RECT_T             *rect_ptr       //in/out:
)
{
    BOOLEAN             result = FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    if (baseedit_ctrl_ptr != PNULL && (base_ctrl_ptr->is_active) && (PNULL != rect_ptr))
    {
        //set cursor rect is invalid
        baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;

        result = BASEEDIT_GetCursorRect (baseedit_ctrl_ptr, PNULL, rect_ptr);
		if(result &&(baseedit_ctrl_ptr-> seperate_line == GUIEDIT_MMS_SEPERATE_LINE))
		{
			rect_ptr->bottom = rect_ptr->bottom +31;
		}
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlGetSelectIndex (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    BOOLEAN               *is_first_ptr,  //in/out:
    BOOLEAN               *is_last_ptr    //in/out:
)
{
    BOOLEAN             result = FALSE;
    uint16              cursor_index = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    if (PNULL != baseedit_ctrl_ptr)
    {
        cursor_index = BASEEDIT_GetCursorLineIndex (baseedit_ctrl_ptr, baseedit_ctrl_ptr->cursor.cur_pos);

        if (0 == cursor_index)
        {
            if (PNULL != is_first_ptr)
            {
                *is_first_ptr = TRUE;
            }
        }

        if (cursor_index == (baseedit_ctrl_ptr->total_line_num - 1))
        {
            if (PNULL != is_last_ptr)
            {
                *is_last_ptr = TRUE;
            }
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetSelectIndex (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //in:
    BOOLEAN               is_first,         //in:
    BOOLEAN               is_last           //in:
)
{
    BOOLEAN             result = FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    if (is_first)
    {
        BaseEditCtrlSetCursorPos(baseedit_ctrl_ptr,0);
        result = TRUE;
    }
    else if (is_last)
    {
        BaseEditCtrlSetCursorPos(baseedit_ctrl_ptr,baseedit_ctrl_ptr->str_len);
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : 获取edit的字符串内容
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL wchar* BaseEditCtrlGetDispStr (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              start_pos,
    uint16              str_len
)
{
    return baseedit_ctrl_ptr->str_ptr + start_pos;
}

/*****************************************************************************/
//  Description : on cursor changed
//  Global resource dependence :
//  Author: arvin
//  Note:
/*****************************************************************************/
LOCAL void BaseEditCtrlOnCursorChanged (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr
)
{
	if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
	{
		MMK_SendMsg(baseedit_ctrl_ptr->im_ctrl_handle, MSG_NOTIFY_EDITBOX_IMSYNC_CURSOR, 0);
	}
}

/*****************************************************************************/
//  Description : handle landscape edit
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleLandscapeEdit (// is create im
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    BOOLEAN             is_tp_up            //in:
)
{
    BOOLEAN  result = TRUE;

#if defined GUIF_EDIT_PDASTYLE && defined MMI_IM_PDA_SUPPORT   //与IM相关，有可能IM 不支持使用PDA 版本的UI
    MMI_CTRL_ID_T           ctrl_id = 0;
    GUIEDIT_CONFIG_KEY_T    *im_config_ptr = PNULL;
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    if (MMK_IsWindowLandscape (base_ctrl_ptr->win_handle))
    {
        if (is_tp_up)
        {
            if (MMITHEME_IsLandscapeEdit (base_ctrl_ptr->win_handle))
            {
                result = TRUE;
            }
            else
            {
                result = FALSE;

                //open edit win
                ctrl_id = MMITHEME_OpenLandscapeEdit (baseedit_ctrl_ptr->type, base_ctrl_ptr->handle);

                if (PNULL != ctrl_id)
                {
                    // call virtual method to set landscape edit by style edit
                    VTLBASEEDIT_SetLandscapeEdit (baseedit_ctrl_ptr, ctrl_id);

                    //config im
                    im_config_ptr = baseedit_ctrl_ptr->im_config_key_ptr;

                    while (PNULL != im_config_ptr)
                    {
                        CTRLBASEEDIT_ConfigImCustomKey (
                                ctrl_id,
                                im_config_ptr->im_type,
                                im_config_ptr->custom_key,
                                im_config_ptr->value_ptr,
                                im_config_ptr->value_count);

                        im_config_ptr = im_config_ptr->next_ptr;
                    }
                }
            }
        }
        else
        {
            //not landscape edit and disable im tp
            if ((!MMITHEME_IsLandscapeEdit (base_ctrl_ptr->win_handle)) &&
                (BASEEDIT_GetImTp (baseedit_ctrl_ptr)))
            {
                result = FALSE;
            }
        }
    }

#endif

    return (result);
}

/*****************************************************************************/
//  Description : handle edit lose active msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBaseEditLoseActive (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    BOOLEAN             is_notify       //in
)
{
    BOOLEAN         is_handle = FALSE;
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    if (is_notify)
    {
        if (!MMK_IsActiveCtrl (base_ctrl_ptr->handle))
        {
            is_handle = TRUE;
        }
    }
    else
    {
        is_handle = TRUE;
    }

    if (is_handle)
    {
        base_ctrl_ptr->is_active = FALSE;

        //destroy im
        BASEEDIT_DestroyImCtrl (baseedit_ctrl_ptr);

        //reset display info bar
        ResetInfoBar (baseedit_ctrl_ptr);

        //reset softkey
        BASEEDIT_ResetSoftkey (baseedit_ctrl_ptr);

        //notify parent control
        result = CTRLMSG_SendNotify (base_ctrl_ptr->handle, MSG_NOTIFY_LOSE_ACTIVE);
    }
    else
    {
        // 当焦点在输入法上，点击edit后焦点落到edit上，此时输入法失去焦点发
        // MSG_NOTIFY_LOSE_ACTIVE，如果这里不将焦点设回到输入法上的话，那后续
        // 按键的处理都是给edit的，状态就不对了。所以，如果是notify_lose_active
        // 消息，且edit控件是有焦点的情况下，将焦点重设回输入法。
        BASEEDIT_SetImActive (baseedit_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : reset display information bar,include im icon and remaining characters
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetInfoBar (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    MMI_IMAGE_ID_T  icon_id = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    //set title icon
    GUIWIN_SetTitleIcon (base_ctrl_ptr->win_handle, icon_id);

    //reset sub string
    GUIWIN_SetTitleSubText (base_ctrl_ptr->win_handle, FALSE, PNULL, 0, FALSE);

    //update title
    GUIWIN_UpdateTitle (base_ctrl_ptr->win_handle);
}

//for [MS]
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: xiaoqinglu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CreateBaseeditThemeLineLayer(
									   CTRLBASEEDIT_OBJ_T		*baseedit_ctrl_ptr,
									   GUI_RECT_T            *line_rect
                                  )
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)baseedit_ctrl_ptr;

    if ((PNULL == baseedit_ctrl_ptr)||(PNULL == line_rect))
	{
        return result;
    }
#ifdef UI_MULTILAYER_SUPPORT
    if(UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T create_info = {0};
		
        //init pop layer info
        create_info.lcd_id       = base_ctrl_ptr->lcd_dev_info.lcd_id;
        create_info.owner_handle = base_ctrl_ptr->handle;
        create_info.offset_x     = line_rect->left;
        create_info.offset_y     = line_rect->top;
        create_info.width        = line_rect->right - line_rect->left + 1;
        create_info.height       = line_rect->bottom - line_rect->top + 1;
		
        //create pop layer
        if (0 == baseedit_ctrl_ptr->theme_line_layer_info.block_id || UILAYER_NULL_HANDLE == baseedit_ctrl_ptr->theme_line_layer_info.block_id)
		{
			if(UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer(&create_info, &(baseedit_ctrl_ptr->theme_line_layer_info)))
			{
				UILAYER_SetLayerColorKey(&(baseedit_ctrl_ptr->theme_line_layer_info), TRUE, UILAYER_TRANSPARENT_COLOR);
				UILAYER_Clear(&(baseedit_ctrl_ptr->theme_line_layer_info));
				result = TRUE;
			}
		}
		else
		{
			UILAYER_SetLayerColorKey(&(baseedit_ctrl_ptr->theme_line_layer_info), TRUE, UILAYER_TRANSPARENT_COLOR);
			UILAYER_Clear(&(baseedit_ctrl_ptr->theme_line_layer_info));
			result = TRUE;
		}
	}
#endif
    return result;
}

/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CreateBaseeditEditLayer(
									   CTRLBASEEDIT_OBJ_T		*baseedit_ctrl_ptr,
									   GUI_RECT_T            *edit_rect
                                  )
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)baseedit_ctrl_ptr;

    if ((PNULL == baseedit_ctrl_ptr)||(PNULL == edit_rect))
	{
        return result;
    }
#ifdef UI_MULTILAYER_SUPPORT
    if(UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T create_info = {0};
		UILAYER_APPEND_BLT_T append_layer = {0};
        //init pop layer info
        create_info.is_static_layer = TRUE;
        create_info.lcd_id       = base_ctrl_ptr->lcd_dev_info.lcd_id;
        create_info.owner_handle = base_ctrl_ptr->handle;
        create_info.offset_x     = edit_rect->left;
        create_info.offset_y     = edit_rect->top;
        create_info.width        = edit_rect->right - edit_rect->left + 1;
        create_info.height       = edit_rect->bottom - edit_rect->top + 1;
		create_info.is_bg_layer  = FALSE;
        //create pop layer
        if (0 == baseedit_ctrl_ptr->edit_layer_info.block_id || UILAYER_NULL_HANDLE == baseedit_ctrl_ptr->edit_layer_info.block_id)
		{
			if(UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer(&create_info, &(baseedit_ctrl_ptr->edit_layer_info)))
			{
				//UILAYER_SetLayerColorKey(&(baseedit_ctrl_ptr->edit_layer_info), TRUE, UILAYER_TRANSPARENT_COLOR);

				UILAYER_Clear(&(baseedit_ctrl_ptr->edit_layer_info));
				append_layer.layer_level = UILAYER_LEVEL_NORMAL;
                append_layer.lcd_dev_info = baseedit_ctrl_ptr->edit_layer_info;                
                UILAYER_AppendBltLayer(&append_layer);
                //UILAYER_SetLayerAlpha(&baseedit_ctrl_ptr->edit_layer_info, 0);
                UILAYER_ActiveLayer(&baseedit_ctrl_ptr->edit_layer_info);
				result = TRUE;
			}
		}
		else
		{
			//UILAYER_SetLayerColorKey(&(baseedit_ctrl_ptr->edit_layer_info), TRUE, UILAYER_TRANSPARENT_COLOR);
			//UILAYER_Clear(&(baseedit_ctrl_ptr->edit_layer_info));
			result = TRUE;
		}
	}
#endif
    return result;
}

/****************************************************************************/    
//  Description: Create Pop Blink Layer
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CreateBaseeditLabelLayer(
                                  CTRLBASEEDIT_OBJ_T		*baseedit_ctrl_ptr,
                                  GUI_RECT_T            *label_rect
                                  )
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)baseedit_ctrl_ptr;
	
    if ((PNULL == baseedit_ctrl_ptr)||(PNULL == label_rect))
    {
        return result;
    }
#ifdef UI_MULTILAYER_SUPPORT
    if(UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T create_info = {0};
		
        //init pop layer info
        create_info.lcd_id       = base_ctrl_ptr->lcd_dev_info.lcd_id;
        create_info.owner_handle = base_ctrl_ptr->handle;
        create_info.offset_x     = label_rect->left;
        create_info.offset_y     = label_rect->top;
        create_info.width        = label_rect->right - label_rect->left + 1;
        create_info.height       = label_rect->bottom - label_rect->top + 1;
		
        //create pop layer
        if (0 == baseedit_ctrl_ptr->label_layer_info.block_id || UILAYER_NULL_HANDLE == baseedit_ctrl_ptr->label_layer_info.block_id )
		{
			if(UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer(&create_info, &(baseedit_ctrl_ptr->label_layer_info)))
			{
				UILAYER_SetLayerColorKey(&(baseedit_ctrl_ptr->label_layer_info), TRUE, UILAYER_TRANSPARENT_COLOR);
				UILAYER_Clear(&(baseedit_ctrl_ptr->label_layer_info));
				result = TRUE;
			}
		}
		else
		{
			UILAYER_SetLayerColorKey(&(baseedit_ctrl_ptr->label_layer_info), TRUE, UILAYER_TRANSPARENT_COLOR);
			UILAYER_Clear(&(baseedit_ctrl_ptr->label_layer_info));
			result = TRUE;
		}
	}
#endif
    return result;
}

/*****************************************************************************/
// Description : 获取字符串
// Global resource dependence : none
// Author: xiaoqinglu
// Note:
/*****************************************************************************/
PUBLIC MMI_STRING_T GetBaseeditLabelText(
                                CTRLBASEEDIT_OBJ_T    *baseedit_ptr
                                )
{
    MMI_STRING_T    label_str = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_ptr); /*assert verified*/
    if (PNULL == baseedit_ptr)
    {
        return label_str;
    }

    if (GUILABEL_DATA_TEXT_ID == baseedit_ptr->label_text_type)
    {
        MMITHEME_GetResText(baseedit_ptr->label_id, baseedit_ptr->base_ctrl.win_handle, &label_str);
    }
    else if (GUILABEL_DATA_TEXT_BUFFER == baseedit_ptr->label_text_type)
    {
        //get string
        label_str.wstr_ptr = baseedit_ptr->label_str.wstr_ptr;
        label_str.wstr_len = baseedit_ptr->label_str.wstr_len;
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }

    return label_str;
}
/*****************************************************************************/
// Description : 获取字符串
// Global resource dependence : none
// Author: xiaoqinglu
// Note:
/*****************************************************************************/
PUBLIC MMI_STRING_T GetBaseeditHintTextText(
                                CTRLBASEEDIT_OBJ_T    *baseedit_ptr
                                )
{
    MMI_STRING_T    hint_text_str = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_ptr); /*assert verified*/
    if (PNULL == baseedit_ptr)
    {
        return hint_text_str;
    }

    if (GUIHINTTEXT_DATA_TEXT_ID == baseedit_ptr->hint_text_text_type)
    {
        MMITHEME_GetResText( baseedit_ptr->hint_text_id, baseedit_ptr->base_ctrl.win_handle, &hint_text_str );
    }
    else if (GUIHINTTEXT_DATA_TEXT_BUFFER == baseedit_ptr->hint_text_text_type)
    {
        //get string
        hint_text_str.wstr_ptr = baseedit_ptr->hint_text_str.wstr_ptr;
        hint_text_str.wstr_len = baseedit_ptr->hint_text_str.wstr_len;
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }

    return hint_text_str;
}
/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN BaseEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_BASEEDIT_TYPE);
}
/*****************************************************************************/
// Description : 释放字符串
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void ReleaseBaseeditLabelStr(
                           CTRLBASEEDIT_OBJ_T *baseedit_ptr
                           )
{
    if (PNULL != baseedit_ptr)
    {
        if (PNULL != baseedit_ptr->label_str.wstr_ptr)
        {
            SCI_FREE(baseedit_ptr->label_str.wstr_ptr);
        }
        
        baseedit_ptr->label_str.wstr_ptr = PNULL;
        baseedit_ptr->label_str.wstr_len = 0;
    }
}
/*****************************************************************************/
// Description : 释放字符串
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void ReleaseBaseeditHintTextStr(
                           CTRLBASEEDIT_OBJ_T *baseedit_ptr
                           )
{
    if (PNULL != baseedit_ptr)
    {
        if (PNULL != baseedit_ptr->hint_text_str.wstr_ptr)
        {
            SCI_FREE(baseedit_ptr->hint_text_str.wstr_ptr);
        }
        
        baseedit_ptr->hint_text_str.wstr_ptr = PNULL;
        baseedit_ptr->hint_text_str.wstr_len = 0;
    }
}
/*****************************************************************************/
// Description : set the window label string
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_SetLabelByPtr(
                                     CTRLBASEEDIT_OBJ_T  *baseedit_ptr,
                                     const wchar*     wstr_ptr, //[IN] string pointer
                                     uint16           wstr_len, //[IN] string length, <=16
                                     BOOLEAN          is_need_update //[IN] is flash
                                     )
{
    BOOLEAN                result  = TRUE;
    
    if(PNULL == baseedit_ptr)
    {
        return FALSE;
    }
    // 申请之前必须释放原来的内存，否则会产生内存泄漏
    ReleaseBaseeditLabelStr(baseedit_ptr);

    if (0 != wstr_len && NULL != wstr_ptr)
    {
        baseedit_ptr->label_str.wstr_ptr = SCI_ALLOC_APP(((wstr_len + 1)*sizeof(wchar)));
        SCI_MEMSET(baseedit_ptr->label_str.wstr_ptr, 0, ((wstr_len + 1)*sizeof(wchar)));
        baseedit_ptr->label_str.wstr_len = wstr_len;
        
        MMI_WSTRNCPY( baseedit_ptr->label_str.wstr_ptr, baseedit_ptr->label_str.wstr_len, wstr_ptr, wstr_len, wstr_len );

        GUI_ReplaceChar(&baseedit_ptr->label_str, LF_CHAR, BL_CHAR);
        GUI_ReplaceChar(&baseedit_ptr->label_str, CR_CHAR, BL_CHAR);
    }

    baseedit_ptr->label_text_type = GUILABEL_DATA_TEXT_BUFFER;

    if (is_need_update)
    {
        BaseFlexCtrlDisplayAll(baseedit_ptr,FALSE);
    }
    
    return (result);
}
/*****************************************************************************/
// Description : set the window hint text string
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_SetHintTextByPtr(
                                     CTRLBASEEDIT_OBJ_T  *baseedit_ptr,
                                     const wchar*     wstr_ptr, //[IN] string pointer
                                     uint16           wstr_len, //[IN] string length, <=16
                                     BOOLEAN          is_need_update //[IN] is flash
                                     )
{
    BOOLEAN                result  = TRUE;
    
    if(PNULL == baseedit_ptr)
    {
        return FALSE;
    }
    // 申请之前必须释放原来的内存，否则会产生内存泄漏
    ReleaseBaseeditHintTextStr(baseedit_ptr);

    if (0 != wstr_len && NULL != wstr_ptr)
    {
        baseedit_ptr->hint_text_str.wstr_ptr = SCI_ALLOC_APP(((wstr_len + 1)*sizeof(wchar)));
        SCI_MEMSET(baseedit_ptr->hint_text_str.wstr_ptr, 0, ((wstr_len + 1)*sizeof(wchar)));
        baseedit_ptr->hint_text_str.wstr_len = wstr_len;
        
        MMI_WSTRNCPY( baseedit_ptr->hint_text_str.wstr_ptr, baseedit_ptr->hint_text_str.wstr_len, wstr_ptr, wstr_len, wstr_len );

        GUI_ReplaceChar(&baseedit_ptr->hint_text_str, LF_CHAR, BL_CHAR);
        GUI_ReplaceChar(&baseedit_ptr->hint_text_str, CR_CHAR, BL_CHAR);
    }

    baseedit_ptr->hint_text_text_type = GUILABEL_DATA_TEXT_BUFFER;

    if (is_need_update)
    {
        BaseFlexCtrlDisplayAll(baseedit_ptr,FALSE);
    }
    
    return (result);
}

/*****************************************************************************/
// Description : get headeranddivider control ptr.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL CTRLBASEEDIT_OBJ_T *GetBaseeditPtr(
                                   MMI_HANDLE_T  win_handle,
                                   MMI_CTRL_ID_T ctrl_id
                                   )
{
    CTRLBASEEDIT_OBJ_T* baseedit_ptr = PNULL;
    MMI_HANDLE_T ctrl_handle = 0;
    
    if(0 != win_handle)
    {
        win_handle = MMK_ConvertIdToHandle( win_handle );
        
        ctrl_handle = MMK_ConvertIdToHandle( ctrl_id );

        if (0 != ctrl_id && ctrl_handle == ctrl_id)
        {
            baseedit_ptr = (CTRLBASEEDIT_OBJ_T*)MMK_GetCtrlPtr(ctrl_id);
        }
        else
        {
            baseedit_ptr = (CTRLBASEEDIT_OBJ_T*)MMK_GetCtrlPtrByWin(win_handle, ctrl_id);
        }

        if(PNULL != baseedit_ptr)
        {
            SCI_ASSERT(BaseEditTypeOf((CTRLBASE_OBJ_T*)baseedit_ptr ));/*assert verified*/
        }
    }
    
    return baseedit_ptr;
}

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
                                )
{
    return CTRLBASEEDIT_SetLabelByPtr(GetBaseeditPtr(win_handle, ctrl_id), wstr_ptr, 
wstr_len, is_need_update);
}
/*****************************************************************************/
// Description : set the window baseedit label string
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
                                )
{
    return CTRLBASEEDIT_SetHintTextByPtr(GetBaseeditPtr(win_handle, ctrl_id), wstr_ptr, 
wstr_len, is_need_update);
}
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
                                  )
{
    BOOLEAN                result  = FALSE;
    CTRLBASEEDIT_OBJ_T        *baseedit_ptr = PNULL;
    
    baseedit_ptr = GetBaseeditPtr(win_handle, ctrl_id);
    
    if(NULL != baseedit_ptr)
    {
        baseedit_ptr->is_disp_password = is_disp;

        result = TRUE;

       if (is_need_update)
        {
           BaseFlexCtrlDisplayAll(baseedit_ptr,FALSE);
         }
    }

    return (result);
}

/*****************************************************************************/
// Description : set the window headeranddivider string, by text id
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_SetLabelId(
                                  MMI_HANDLE_T        win_handle,
                                  MMI_CTRL_ID_T        ctrl_id,
                                  MMI_TEXT_ID_T        text_id, //[IN] the string id
                                  BOOLEAN            is_need_update //[IN] is flash
                                  )
{
    BOOLEAN                result  = FALSE;
    CTRLBASEEDIT_OBJ_T        *baseedit_ptr = PNULL;
    
    baseedit_ptr = GetBaseeditPtr(win_handle, ctrl_id);
    
    if(NULL != baseedit_ptr)
    {
        // text type
        baseedit_ptr->label_text_type = GUILABEL_DATA_TEXT_ID;

        //  text id
        baseedit_ptr->label_id = text_id;
       
        result = TRUE;

        if (is_need_update)
        {
            BaseFlexCtrlDisplayAll(baseedit_ptr,FALSE);
        }
    }

    return (result);
}

PUBLIC BOOLEAN CTRLBASEEDIT_SetDeActiveBorderColor(
                                  MMI_HANDLE_T        win_handle,
                                  MMI_CTRL_ID_T        ctrl_id,
                                  CAF_COLOR_T		borderColor
                                  )
{
    BOOLEAN                result  = FALSE;
    CTRLBASEEDIT_OBJ_T        *baseedit_ptr = PNULL;
    
    baseedit_ptr = GetBaseeditPtr(win_handle, ctrl_id);
    
    if(NULL != baseedit_ptr)
    {
        baseedit_ptr->base_ctrl.border.color = borderColor;
        baseedit_ptr->is_update_borderColor = TRUE;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : set the window headeranddivider string, by text id
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_SetHintTextId(
                                  MMI_HANDLE_T        win_handle,
                                  MMI_CTRL_ID_T        ctrl_id,
                                  MMI_TEXT_ID_T        text_id, //[IN] the string id
                                  BOOLEAN            is_need_update //[IN] is flash
                                  )
{
    BOOLEAN                result  = FALSE;
    CTRLBASEEDIT_OBJ_T        *baseedit_ptr = PNULL;
    
    baseedit_ptr = GetBaseeditPtr(win_handle, ctrl_id);
    
    if(NULL != baseedit_ptr)
    {
        // text type
        baseedit_ptr->hint_text_text_type = GUIHINTTEXT_DATA_TEXT_ID;

        //  text id
        baseedit_ptr->hint_text_id = text_id;
       
        result = TRUE;

        if (is_need_update)
        {
            BaseFlexCtrlDisplayAll(baseedit_ptr,FALSE);
        }
    }

    return (result);
}


/*****************************************************************************/
// Description : set if inlineeditor or not
// Global resource dependence : 
// Author:mary.xiao
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_SetInlineStyle(
                                  MMI_CTRL_ID_T        ctrl_id,
                                  BOOLEAN			is_inline_style
                                  )
{
    BOOLEAN                result  = FALSE;
    CTRLBASEEDIT_OBJ_T        *baseedit_ptr = PNULL;
    
    baseedit_ptr = BASEEDIT_GetPtr (ctrl_id);
    
    if(NULL != baseedit_ptr)
    {
        baseedit_ptr->is_inline_editor= is_inline_style;

        result = TRUE;

    }

    return (result);
}

/*****************************************************************************/
// Description : set if inlineeditor or not
// Global resource dependence : 
// Author:mary.xiao
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_SetVisible(
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN          is_visible,
    BOOLEAN          is_update
                                  )
{
    BOOLEAN                result  = FALSE;
    CTRLBASEEDIT_OBJ_T        *baseedit_ptr = PNULL;
	CTRLBASE_OBJ_T      *base_ctrl_ptr = PNULL;
    
    baseedit_ptr = BASEEDIT_GetPtr (ctrl_id);
    
    if(NULL != baseedit_ptr)
    {
		base_ctrl_ptr = (CTRLBASE_OBJ_T*)baseedit_ptr;
		if(baseedit_ptr->is_inline_editor)//for inline editor,即使窗口上没有焦点空间，也不主动设置焦点
		{
			base_ctrl_ptr->is_visible = is_visible;
		}
		else
		{
			VTLBASE_SetVisible(base_ctrl_ptr, is_visible);
		}

        if (is_update)              
        {
			if (is_visible)
			{
                if(baseedit_ptr != PNULL && baseedit_ptr->is_inline_editor )
                {
                    CreateBaseeditEditLayer(baseedit_ptr, &(base_ctrl_ptr->display_rect));
                }  
				VTLBASEEDIT_DisplayAll (
						baseedit_ptr,
						TRUE);
			}
			
			else
			{
				 if(baseedit_ptr != PNULL && baseedit_ptr->is_inline_editor )
				 {
#ifdef UI_MULTILAYER_SUPPORT	
					UILAYER_Clear(&(baseedit_ptr->edit_layer_info ));//wil add after baokun checkin
#endif
#if defined GUIF_PRGBOX_AUTOHIDE || defined GUIF_PRGBOX_LETTERSCROLL				
					CTRLPROGRESSBAR_SetInvisible(baseedit_ptr->prgbox_ctrl_ptr);
#endif
				 }
				//CTRLMSG_SendNotifyEx(base_ctrl_ptr->handle,MSG_NOTIFY_UPDATE,&base_ctrl_ptr->rect);
			}
        }

    }

    return (result);
}



/*****************************************************************************/
// Description : set if inlineeditor or not
// Global resource dependence : 
// Author:mary.xiao
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_Update( MMI_CTRL_ID_T    ctrl_id )
{
    BOOLEAN                result  = FALSE;
    CTRLBASEEDIT_OBJ_T        *baseedit_ptr = PNULL;
	CTRLBASE_OBJ_T      *base_ctrl_ptr = PNULL;
    
    baseedit_ptr = BASEEDIT_GetPtr (ctrl_id);
    
    if(NULL != baseedit_ptr)
    {
		base_ctrl_ptr = (CTRLBASE_OBJ_T*)baseedit_ptr;
		MMK_SendMsg(base_ctrl_ptr->handle, MSG_CTL_PAINT, PNULL);

    }

    return (result);
}


/*****************************************************************************/
//  Description : get edit line rect,except dividing line,progress bar
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16  CTRLINLINEEDIT_GetTextHeight(
					GUIEDIT_TYPE_E type, 
					uint16 line_space, 
					uint16 edit_rect_right, 
					uint16 edit_rect_left,  
					GUI_FONT_T font,
					uint8      char_space,
					const wchar* str_ptr,  
					uint16     str_len
)
{
	return INLINEEDIT_GetTextHeight(type, line_space, edit_rect_right, edit_rect_left, font, char_space, str_ptr, str_len);
}

/*****************************************************************************/
//  Description : Set String display inverse flag
//  Global resource dependence : 
//  Author: Howie
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetIsDisplayInverse(
                                MMI_CTRL_ID_T      ctrl_id,     //in:
                                BOOLEAN                is_display_inverse
                                )
{
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr = PNULL;

    //get text pointer by control id
    baseedit_ctrl_ptr = BASEEDIT_GetPtr(ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        //set is text dialog flag 
        baseedit_ctrl_ptr->is_display_inverse= is_display_inverse;
    }
}

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
    uint16                           str_len,    //in:0,不变
    MMI_TEXT_ID_T                    text_1,     //in:<str_len display text_1
    MMI_TEXT_ID_T                    text_2,     //in:>=str_len display text_2
    MMITHEME_EDIT_SET_BUTTON_PFUNC   setsoftkey  //in:优先通过函数设置softkey[in:字符串指针in:字符串长度,out:softkey值]
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = GetBaseeditPtr(win_handle, ctrl_id);

    if ((PNULL != baseedit_ctrl_ptr) &&
        (index < GUISOFTKEY_BUTTON_NUM))
    {
        baseedit_ctrl_ptr->common_theme.softkey.button[index].is_modify  = TRUE;
        baseedit_ctrl_ptr->common_theme.softkey.button[index].str_len    = str_len;
        baseedit_ctrl_ptr->common_theme.softkey.button[index].setsoftkey = setsoftkey;

        if (0 != text_1)
        {
            baseedit_ctrl_ptr->common_theme.softkey.button[index].text_1 = text_1;
        }

        if (0 != text_2)
        {
            baseedit_ctrl_ptr->common_theme.softkey.button[index].text_2 = text_2;
        }
    }
}

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
                               uint16                           str_len,    //in:0,不变
                               MMI_TEXT_ID_T                    text_1,     //in:<str_len display text_1
                               MMI_TEXT_ID_T                    text_2,     //in:>=str_len display text_2
                               MMI_IMAGE_ID_T                   image_id,
                               MMI_IMAGE_ID_T                   image_id_2, // MS
                               BOOLEAN                               show_icon,
                               MMITHEME_EDIT_SET_BUTTON_PFUNC   setsoftkey  //in:优先通过函数设置softkey[in:字符串指针in:字符串长度,out:softkey值]
                               )
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = GetBaseeditPtr(win_handle, ctrl_id);

    if (PNULL != baseedit_ctrl_ptr &&
        (index < GUISOFTKEY_BUTTON_NUM))
    {
        if (!show_icon)
        {
            baseedit_ctrl_ptr->common_theme.softkey.button[index].button_type = EDIT_BUTTON_TEXT_ID;
            //set softkey text
            CTRLBASEEDIT_SetSoftkey(win_handle, ctrl_id, index, str_len, text_1, text_2, setsoftkey);
        } 
        else
        {
            baseedit_ctrl_ptr->common_theme.softkey.button[index].button_type = EDIT_BUTTON_IMAGE_ID;
            //set softkey button icon
            baseedit_ctrl_ptr->common_theme.softkey.button[index].is_modify  = TRUE;
            baseedit_ctrl_ptr->common_theme.softkey.button[index].image_id = image_id;
            baseedit_ctrl_ptr->common_theme.softkey.button[index].setsoftkey = setsoftkey;

            baseedit_ctrl_ptr->common_theme.softkey.button[index].str_len = str_len; //[MS]
            baseedit_ctrl_ptr->common_theme.softkey.button[index].image_id_2 = image_id_2; //[MS]
        }
    }
}

// End

