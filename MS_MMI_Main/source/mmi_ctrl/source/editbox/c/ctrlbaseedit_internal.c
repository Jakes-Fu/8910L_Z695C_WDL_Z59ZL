/*****************************************************************************
 ** File Name:      ctrlbaseedit_internal.c                                   *
 ** Author:         hua.fang                                                  *
 ** Date:           09/10/2012                                                *
 ** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:                                                              *
 ******************************************************************************
 **                         Important Edit History                            *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/2012        hua.fang          Create                                   *
 ******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "ctrlbaseedit.h"
#include "mmi_gui_trc.h"
#include "mmi_theme.h"
#include "ctrlform_export.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

#define MAKE_BREW_RGB(r,g,b) ((r >> 3)<<11 | (g>>2)<<5 | (b>>3))

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : adjust rect top position by align
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustRectTopByAlign (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
        GUI_RECT_T          *rect_ptr           //in/out:
        );

/*****************************************************************************/
//  Description : get line string pixel
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetEditLineStrPixel (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
        uint16              line_index,     //in:
        uint16              rect_width      //in:
        );

/*****************************************************************************/
//  Description : notify parent edit change msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SendChangeMsg (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
        BOOLEAN             is_line_num_change //in:
        );

/*****************************************************************************/
//  Description : adjust rect for mirrorRect
//  Global resource dependence :
//  Author: Howie
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_MirrorRect(
        GUI_RECT_T    *rect_dest_ptr
        );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_Init(
        CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr
        )
{
    // 以下这些设置，因为需要各个子类的配置信息，而这些信息是在构造时才初始化，
    // 这样时序上就有问题，所以放在这个消息里做，让子类调用

    //alloc string memory
    SCI_ASSERT (PNULL == baseedit_ctrl_ptr->str_ptr); /*assert verified*/
    baseedit_ctrl_ptr->str_ptr = BASEEDIT_AllocStrMemory (baseedit_ctrl_ptr->str_max_len);

    //alloc line info memory
    SCI_ASSERT (PNULL == baseedit_ctrl_ptr->line_info_ptr); /*assert verified*/
    baseedit_ctrl_ptr->line_info_ptr = BASEEDIT_AllocLineMemory (baseedit_ctrl_ptr->str_max_len);
}

/*****************************************************************************/
//  Description : alloc edit string memory
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC wchar* BASEEDIT_AllocStrMemory (
        uint16     max_len
        )
{
    wchar   *edit_str_ptr = PNULL;

    if (0 < max_len)
    {
        edit_str_ptr = SCI_ALLOC_APP ( (max_len + 1) * sizeof (wchar));

        SCI_MEMSET (edit_str_ptr, 0, ( (max_len + 1) *sizeof (wchar)));
    }

    return (edit_str_ptr);
}

/*****************************************************************************/
//  Description : alloc edit line info memory
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUIEDIT_LINE_T* BASEEDIT_AllocLineMemory (
        uint16   max_len
        )
{
    GUIEDIT_LINE_T  *line_info_ptr = PNULL;

    if (0 < max_len)
    {
        line_info_ptr = SCI_ALLOC_APP ( (max_len + 1) * sizeof (GUIEDIT_LINE_T));

        SCI_MEMSET (line_info_ptr, 0, ( (max_len + 1) *sizeof (GUIEDIT_LINE_T)));
    }

    return (line_info_ptr);
}

/*****************************************************************************/
//  Description : notify parent control or parent window notify message
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E BASEEDIT_NotifyParentMsg (
        MMI_HANDLE_T        ctrl_handle,
        MMI_MESSAGE_ID_E    notify_msg
        )
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;

    switch (notify_msg)
    {
        case MSG_NOTIFY_OK:
        case MSG_NOTIFY_MIDSK:
        case MSG_NOTIFY_CANCEL:
        case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        case MSG_NOTIFY_EDITBOX_MAXTEXT:
            //notify parent control
            result = CTRLMSG_PostNotify (ctrl_handle, notify_msg);
            break;

        default:
            //SCI_TRACE_LOW:"GUIEDIT_NotifyParentMsg:notify_msg = %d is error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_INTERNAL_467_112_2_18_3_16_5_131, (uint8*) "d", notify_msg);
            break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : notify parent total line number has change
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_NotifyLineMsg (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
        uint16              prev_total_num  //in:
        )
{
    BOOLEAN     result = FALSE;
    int page_num = BASEEDIT_GetPageLineNum(baseedit_ctrl_ptr);
    //post parent control
    if (prev_total_num == baseedit_ctrl_ptr->total_line_num)
    {
        //set cursor rect is invalid
        baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;

        result = BASEEDIT_NotifyCursorMsg (baseedit_ctrl_ptr);
    }
    else if(baseedit_ctrl_ptr->is_inline_editor && (prev_total_num > baseedit_ctrl_ptr->total_line_num))//inline editor
    {
        MMK_SendMsg(((CTRLBASE_OBJ_T*) baseedit_ctrl_ptr)->handle,MSG_NOTIFY_IIEDT_REDUCE_LINE,PNULL);
    }
    else
    {
        result = SendChangeMsg (baseedit_ctrl_ptr, TRUE);
        if(baseedit_ctrl_ptr->is_inline_editor && (baseedit_ctrl_ptr->total_line_num>page_num))//inline editor
        {
            MMK_SendMsg(((CTRLBASE_OBJ_T*) baseedit_ctrl_ptr)->handle,MSG_NOTIFY_REACH_END,PNULL);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : notify parent edit move cursor
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_NotifyCursorMsg (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
        )
{
    BOOLEAN         result = FALSE;
    GUI_RECT_T      cursor_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    if (baseedit_ctrl_ptr->is_rect_adaptive)
    {
        //get cursor rect
        if (BASEEDIT_GetCursorRect (baseedit_ctrl_ptr, PNULL, &cursor_rect))
        {
            //current cursor is not display all
            if ((cursor_rect.top < base_ctrl_ptr->display_rect.top) ||
                    (cursor_rect.bottom > base_ctrl_ptr->display_rect.bottom))
            {
                result = SendChangeMsg (baseedit_ctrl_ptr, FALSE);
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : display edit background
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_DisplayBg (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
        GUI_RECT_T          bg_rect
        )
{
    GUI_RECT_T          img_rect = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    //set image rect
    img_rect.left   = (int16) (bg_rect.left - base_ctrl_ptr->rect.left);
    img_rect.top    = (int16) (bg_rect.top - base_ctrl_ptr->rect.top);
    img_rect.right  = (int16) (img_rect.left + bg_rect.right - bg_rect.left);
    img_rect.bottom = (int16) (img_rect.top + bg_rect.bottom - bg_rect.top);

    //set bg display info
    bg_display.is_transparent = baseedit_ctrl_ptr->is_bg_transparent;
    bg_display.rect           = base_ctrl_ptr->rect;
    bg_display.display_rect   = bg_rect;
    bg_display.img_rect       = img_rect;
    bg_display.ctrl_handle    = base_ctrl_ptr->handle;
    bg_display.win_handle     = base_ctrl_ptr->win_handle;

    //display bg
    GUI_DisplayBg (
            &baseedit_ctrl_ptr->common_theme.bg,
            &bg_display,
            &baseedit_ctrl_ptr->edit_layer_info);
}

/*****************************************************************************/
//  Description : display edit background
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_DisplayThemeLine (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
        GUI_RECT_T          bg_rect
        )
{
    GUI_RECT_T          line_rect = {0};
    GUI_RECT_T          cross_rect = {0};
    GUI_RECT_T			edit_client_rect = {0};
    GUI_COLOR_T			line_color;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;
    UILAYER_APPEND_BLT_T append_layer_info = {0};
    uint16      lcd_height = 0;
    uint16      lcd_width = 0;
    uint16 end_line_index   = BASEEDIT_GetBottomLineIndex (baseedit_ctrl_ptr);
    GUI_RECT_T text_rect = BASEEDIT_GetTextRect (baseedit_ctrl_ptr, end_line_index);

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    // get edit client rect
    edit_client_rect = BASEEDIT_GetClientRect(baseedit_ctrl_ptr);
    if (!GUI_IntersectRect (&cross_rect, edit_client_rect, bg_rect))
    {
        return;
    }
    if ((GUIEDIT_STYLE_MULTI_DYN_DOWN == baseedit_ctrl_ptr->style)||(GUIEDIT_STYLE_MULTI_UP == baseedit_ctrl_ptr->style))
    {
        // Add 1 pixel for line and text overlap problem
        line_rect.top = text_rect.bottom +1;
        line_rect.left = GUIEDIT_ITEM_VERTICAL_MARGIN;
        line_rect.right = lcd_width - GUIEDIT_ITEM_VERTICAL_MARGIN;
        line_rect.bottom = line_rect.top + 2;
    }
    else if(baseedit_ctrl_ptr->is_inline_editor)
    {
        line_rect = edit_client_rect;
        line_rect.bottom= edit_client_rect.bottom +2 + baseedit_ctrl_ptr->common_theme.margin_tb_space;
        line_rect.top = line_rect.bottom - 2;
        line_rect.left = GUIEDIT_ITEM_VERTICAL_MARGIN;
        line_rect.right = lcd_width - GUIEDIT_ITEM_VERTICAL_MARGIN;
    }
    else
    {
        line_rect = edit_client_rect;
        line_rect.bottom= edit_client_rect.bottom +2;
        line_rect.top = edit_client_rect.bottom;
        line_rect.left = GUIEDIT_ITEM_VERTICAL_MARGIN;
        line_rect.right = lcd_width - GUIEDIT_ITEM_VERTICAL_MARGIN;
    }
    //	else
    //	{
    //		line_rect = edit_client_rect;
    //		line_rect.top = edit_client_rect.bottom - 6;
    //	}

    if (!GUI_IntersectRect (&cross_rect, line_rect, bg_rect))
    {
        return;
    }

    // draw theme line

    if (TRUE == base_ctrl_ptr->is_active)
    {
        if(baseedit_ctrl_ptr->is_inline_editor && baseedit_ctrl_ptr->common_theme.is_themeline_set)
        {
            line_color = baseedit_ctrl_ptr->common_theme.themeline_focused_color;
            LCD_FillRect(&baseedit_ctrl_ptr->edit_layer_info, line_rect, line_color);
        }
        else if (GUIEDIT_STYLE_SINGLE_UP == baseedit_ctrl_ptr->style)
        {
            //			line_color = baseedit_ctrl_ptr->common_theme.non_focused_color ;
            line_color = MMITHEME_GetAccentColor(MMITHEME_GetAccentId(), MMITHEME_OFFSET_COLOR_DEFAULT);
            LCD_FillRect(&baseedit_ctrl_ptr->edit_layer_info, line_rect, line_color);

        }

        else
        {
            line_color = MMITHEME_GetAccentColor(MMITHEME_GetAccentId(), MMITHEME_OFFSET_COLOR_DEFAULT);
            LCD_FillRect(&baseedit_ctrl_ptr->edit_layer_info, line_rect, line_color);
        }
        //		if(0 != baseedit_ctrl_ptr->theme_line_layer_info.block_id)
        //			UILAYER_ClearRect((const GUI_LCD_DEV_INFO*)&baseedit_ctrl_ptr->theme_line_layer_info,line_rect);
    }
    else
    {
        if(baseedit_ctrl_ptr->is_inline_editor && baseedit_ctrl_ptr->common_theme.is_themeline_set)
        {
            line_color = baseedit_ctrl_ptr->common_theme.themeline_unfocused_color;
        }
        else
        {
            line_color = baseedit_ctrl_ptr->common_theme.non_focused_color ;
        }
        LCD_FillArgbRect( &baseedit_ctrl_ptr->edit_layer_info, 
                line_rect, 
                line_color,
                135);
        /*

           if(0 != baseedit_ctrl_ptr->theme_line_layer_info.block_id)
           {
           UILAYER_ReleaseLayer(&baseedit_ctrl_ptr->theme_line_layer_info);
           baseedit_ctrl_ptr->theme_line_layer_info.block_id = 0; 
           }

        //CreateBaseeditThemeLineLayer(baseedit_ctrl_ptr, &line_rect);
        //UILAYER_ClearRect((const GUI_LCD_DEV_INFO*)&baseedit_ctrl_ptr->theme_line_layer_info,line_rect);

        append_layer_info.layer_level = UILAYER_LEVEL_NORMAL;
        append_layer_info.lcd_dev_info = baseedit_ctrl_ptr->theme_line_layer_info;

        //append pop layer
        UILAYER_AppendBltLayer(&append_layer_info);
        UILAYER_SetLayerAlpha(&baseedit_ctrl_ptr->theme_line_layer_info,135);

        LCD_FillRect(&baseedit_ctrl_ptr->theme_line_layer_info, line_rect, line_color);
         */
        //LCD_FillRect(&baseedit_ctrl_ptr->base_ctrl.lcd_dev_info, line_rect, line_color);
    }
}



/*****************************************************************************/
//  Description : display SeperateLine for message
//  Global resource dependence :
//  Author: Howie
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_DisplaySeperateLine (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
        GUI_RECT_T          bg_rect
        )
{
    GUI_RECT_T          line_rect = {0};
    GUI_RECT_T          cross_rect = {0};
    GUI_RECT_T			edit_client_rect = {0};
    //   GUI_COLOR_T			line_color;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;
    //    UILAYER_APPEND_BLT_T append_layer_info = {0};
    MMI_IMAGE_ID_T im_draw = 0; //IMAGE_NULL;
    // get edit client rect
    edit_client_rect = BASEEDIT_GetClientRect(baseedit_ctrl_ptr);

    line_rect = edit_client_rect;
    line_rect.bottom= edit_client_rect.bottom +31;
    line_rect.top =  line_rect.bottom -28;

    // draw seperate line
    im_draw = baseedit_ctrl_ptr->image_id;
    //im_draw = CTRLIM_ImMessagePenIcon(baseedit_ctrl_ptr->im_ctrl_handle);

    if (!GUI_IntersectRect(&cross_rect, bg_rect,line_rect))
    {
        return;
    }

    GUIRES_DisplayImg ( PNULL,
            &cross_rect,
            PNULL,
            base_ctrl_ptr->win_handle,
            im_draw,
            (const GUI_LCD_DEV_INFO*) &baseedit_ctrl_ptr->edit_layer_info);

}

/*****************************************************************************/
//  Description : display edit background
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_DisplayEditExtraArea (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
        GUI_RECT_T          bg_rect
        )
{
    MMI_STRING_T	label_str = {0};
    MMITHEME_EDIT_TEXT_DISPLAY_T  display_theme = {0};
    MMI_IMAGE_ID_T im_Icon = 0; //IMAGE_NULL;
    GUI_RECT_T  im_icon_rect = {0};
    GUISTR_STYLE_T  str_style = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;
    GUI_RECT_T  cross_rect = {0};

    // Add for pen icon
    GUI_RECT_T  im_icon_pen_rect = {0};
    MMI_IMAGE_ID_T im_pen_Icon = 0; //IMAGE_NULL;
    //uint8 language_type = MMISET_GetCurrentLanguageType();
    MMITHEME_GetTextDisplayTheme(&display_theme);
    lcd_dev_info = baseedit_ctrl_ptr->edit_layer_info;
    // set label and icon text string style
    str_style.font = SONG_FONT_16;
    str_style.align = ALIGN_LVMIDDLE;
    str_style.char_space = 0;
    if (TRUE == base_ctrl_ptr->is_active)
    {
        if(baseedit_ctrl_ptr->is_inline_editor && baseedit_ctrl_ptr->common_theme.is_themeline_set)
        {
            str_style.font_color = baseedit_ctrl_ptr->common_theme.themeline_focused_color;
        }
        else
        {
            str_style.font_color = MMITHEME_GetAccentColor(MMITHEME_GetAccentId(),MMITHEME_OFFSET_COLOR_DEFAULT);
        }
    }
    else
    {
        if(baseedit_ctrl_ptr->is_inline_editor && baseedit_ctrl_ptr->common_theme.is_themeline_set)
        {
            str_style.font_color = baseedit_ctrl_ptr->common_theme.themeline_unfocused_color;
        }
        else
        {
            str_style.font_color = baseedit_ctrl_ptr->common_theme.non_focused_color;
        }
    }

    // Add for pen icon
    if (GUIEDIT_STYLE_SINGLE_UP == baseedit_ctrl_ptr->style)
    {
        im_Icon = CTRLIM_ImGetDefaultIcon(baseedit_ctrl_ptr->im_ctrl_handle);
        if (CTRLIM_ImGetPenIcon(baseedit_ctrl_ptr->im_ctrl_handle) != PNULL)
            im_pen_Icon = CTRLIM_ImGetDefaultPenIcon(baseedit_ctrl_ptr->im_ctrl_handle);
    }
    else
    {
        im_Icon = CTRLIM_ImGetIcon(baseedit_ctrl_ptr->im_ctrl_handle);
        if (CTRLIM_ImGetPenIcon(baseedit_ctrl_ptr->im_ctrl_handle) != PNULL)
            im_pen_Icon = CTRLIM_ImGetPenIcon(baseedit_ctrl_ptr->im_ctrl_handle);
    }

    label_str = GetBaseeditLabelText(baseedit_ctrl_ptr);
    if ( (PNULL != label_str.wstr_ptr) && (0 < label_str.wstr_len))
    {
        //GUI_RECT_T	top_rect = {0};
        GUI_RECT_T	label_rect = {0};

        label_rect = base_ctrl_ptr->rect;
        label_rect.top = base_ctrl_ptr->rect.top + 6;
        label_rect.bottom = label_rect.top + 24;
        label_rect.left = label_rect.left + baseedit_ctrl_ptr->common_theme.margin_lr_space +1;
        label_rect.right = label_rect.left + 176;

        im_icon_rect = base_ctrl_ptr->rect;
        im_icon_rect.right = im_icon_rect.right -GUIEDIT_ITEM_VERTICAL_MARGIN;
        im_icon_rect.left = im_icon_rect.right - 28;
        im_icon_rect.top = base_ctrl_ptr->rect.top + 10;
        im_icon_rect.bottom = im_icon_rect.top + 16;

        im_icon_pen_rect = im_icon_rect;
        im_icon_pen_rect.right = im_icon_pen_rect.left - 2;
        im_icon_pen_rect.left = im_icon_pen_rect.right - 16;

        // Mirror calculate
        if (TRUE == MMISET_IsCurrentLanguageTextAlignRTLDirection())
        {
            BASEEDIT_MirrorRect(&label_rect);
            BASEEDIT_MirrorRect(&im_icon_rect);
            BASEEDIT_MirrorRect(&im_icon_pen_rect);
            str_style.align = ALIGN_RIGHT;
        }

        if(TRUE == base_ctrl_ptr->is_active)
        {
            BASEEDIT_DisplayBg (baseedit_ctrl_ptr, label_rect);

            // Add for icon overlay after display part
            BASEEDIT_DisplayBg (baseedit_ctrl_ptr, im_icon_rect);
            BASEEDIT_DisplayBg (baseedit_ctrl_ptr, im_icon_pen_rect);

            if(GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, label_rect))
            {
                // draw label		
                GUISTR_DrawTextToLCDInRect (
                        &lcd_dev_info,
                        &label_rect,
                        &cross_rect,				
                        &label_str,
                        &str_style,
                        GUISTR_STATE_SINGLE_LINE,
                        GUISTR_TEXT_DIR_AUTO);
            }
#if 0            
            // draw im icon
            GUIRES_DisplayImg ( PNULL,
                    &im_icon_rect,
                    PNULL,
                    base_ctrl_ptr->win_handle,
                    im_Icon,
                    &lcd_dev_info);

            // Add for pen icon
            GUIRES_DisplayImg ( PNULL,
                    &im_icon_pen_rect,
                    PNULL,
                    base_ctrl_ptr->win_handle,
                    im_pen_Icon,
                    &lcd_dev_info);
#endif
            if(MMISTATUSBAR_ITEM_HEIGHT > im_icon_rect.top)
            {
                GUIWIN_UpdateStb();
            }
        }
        else
        {
            if(GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, label_rect))
            {
#ifdef UI_MULTILAYER_SUPPORT
                CreateBaseeditLabelLayer(baseedit_ctrl_ptr, &label_rect);
                UILAYER_SetLayerAlpha(&baseedit_ctrl_ptr->label_layer_info,135);
#else 
                if (TRUE == base_ctrl_ptr->is_active)
                {
                    str_style.font_color = MMITHEME_GetAccentColor(MMITHEME_GetAccentId(), MMITHEME_OFFSET_COLOR_DEFAULT);
                }
                else 
                {
                    str_style.font_color = MAKE_BREW_RGB(102, 102, 102);
                }
#endif
                GUISTR_DrawTextToLCDInRect (
                        &baseedit_ctrl_ptr->label_layer_info,
                        &label_rect,
                        &cross_rect,				
                        &label_str,
                        &str_style,
                        GUISTR_STATE_SINGLE_LINE,
                        GUISTR_TEXT_DIR_AUTO);
#ifdef UI_MULTILAYER_SUPPORT
                UILAYER_BltLayerToLayer(&lcd_dev_info,
                        &baseedit_ctrl_ptr->label_layer_info,
                        &label_rect, TRUE);   

                UILAYER_ReleaseLayer(&baseedit_ctrl_ptr->label_layer_info);
                baseedit_ctrl_ptr->label_layer_info.block_id = 0;
#endif
            }
        }
    }
    else
    {	
#if 0
        if (GUIEDIT_STYLE_MULTI_UP == baseedit_ctrl_ptr->style)
        {
            uint16 end_line_index = BASEEDIT_GetBottomLineIndex (baseedit_ctrl_ptr);
            GUI_RECT_T text_rect = BASEEDIT_GetTextRect (baseedit_ctrl_ptr, end_line_index);
            im_icon_rect.top = text_rect.bottom + 7; //原来+10, 微调成输入字下方+7
            im_icon_rect.right = 240 -GUIEDIT_ITEM_VERTICAL_MARGIN;
            im_icon_rect.left = im_icon_rect.right - 28;
            im_icon_rect.bottom = im_icon_rect.top + 16;	

            // Add for pen icon
            im_icon_pen_rect = im_icon_rect;
            im_icon_pen_rect.right = im_icon_pen_rect.left - 2;
            im_icon_pen_rect.left = im_icon_pen_rect.right - 16;
        }
        else
        {
            // draw im icon
            im_icon_rect = base_ctrl_ptr->rect;
            im_icon_rect.right = im_icon_rect.right -GUIEDIT_ITEM_VERTICAL_MARGIN;
            im_icon_rect.left = im_icon_rect.right - 28;
            im_icon_rect.bottom = base_ctrl_ptr->rect.bottom - 4;
            im_icon_rect.top = im_icon_rect.bottom - 16;

            // Add for pen icon
            im_icon_pen_rect = im_icon_rect;
            im_icon_pen_rect.right = im_icon_pen_rect.left - 2;
            im_icon_pen_rect.left = im_icon_pen_rect.right - 16;
        }

        if (!GUI_RectIsCovered (bg_rect,im_icon_rect) && !GUI_RectIsCovered(bg_rect, im_icon_pen_rect))
        {
            return;
        }
        if (baseedit_ctrl_ptr->seperate_line != GUIEDIT_MMS_SEPERATE_LINE)
        {
            // Add for icon become wide
            BASEEDIT_DisplayBg (baseedit_ctrl_ptr, im_icon_rect);
            BASEEDIT_DisplayBg (baseedit_ctrl_ptr, im_icon_pen_rect);
        }

        GUIRES_DisplayImg ( PNULL,
                &im_icon_pen_rect,
                PNULL,
                base_ctrl_ptr->win_handle,
                im_pen_Icon,
                &lcd_dev_info);

        GUIRES_DisplayImg ( PNULL,
                &im_icon_rect,
                PNULL,
                base_ctrl_ptr->win_handle,
                im_Icon,
                &lcd_dev_info);
#endif        
    }
}

/*****************************************************************************/
//  Description : update text string bg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_UpdateTextBg (
        CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr,     //in:
        uint16                  start_line_index,       //in:
        uint16                  end_line_index          //in:
        )
{
    uint16          i = 0;
    GUI_RECT_T      line_rect = {0};
    GUI_RECT_T      bg_rect = {0};
    GUI_RECT_T      cross_rect = {0};
    GUI_RECT_T      edit_client_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    // get edit client rect
    edit_client_rect = BASEEDIT_GetClientRect(baseedit_ctrl_ptr);

    //get bg rect
    bg_rect = BASEEDIT_GetLineRect (baseedit_ctrl_ptr, start_line_index);

    if (end_line_index > start_line_index)
    {
        line_rect = BASEEDIT_GetLineRect (baseedit_ctrl_ptr, end_line_index);
        bg_rect.bottom = line_rect.bottom;
        // for [MS]
        if((GUIEDIT_STYLE_MULTI_DYN_DOWN == baseedit_ctrl_ptr->style)
                ||(GUIEDIT_STYLE_SINGLE_DYN_DOWN == baseedit_ctrl_ptr->style))
        {
            //Fix theme line become wide after over 1 line
            //base_ctrl_ptr->display_rect.bottom = line_rect.bottom + 6;
            //bg_rect.left = edit_client_rect.left = base_ctrl_ptr->display_rect.left;
            //bg_rect.right = edit_client_rect.right = base_ctrl_ptr->display_rect.right;
        }

        if((GUIEDIT_STYLE_MULTI_DYN_UP == baseedit_ctrl_ptr->style)||(GUIEDIT_STYLE_SINGLE_UP == baseedit_ctrl_ptr->style))
        {
            // 			bg_rect.bottom = base_ctrl_ptr->display_rect.bottom;
            //  			edit_client_rect.bottom = base_ctrl_ptr->display_rect.bottom = bg_rect.bottom = base_ctrl_ptr->rect.bottom;
            bg_rect.left = edit_client_rect.left = base_ctrl_ptr->display_rect.left;
            bg_rect.right = edit_client_rect.right = base_ctrl_ptr->display_rect.right;
        }
    }

    //get crossed rect
    if (GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, bg_rect)
            && GUI_IntersectRect(&cross_rect, edit_client_rect, cross_rect))
    {
        //display bg
        BASEEDIT_DisplayBg (baseedit_ctrl_ptr, cross_rect);

        if (GUIEDIT_STYLE_MULTI_DYN_DOWN == baseedit_ctrl_ptr->style)
        {
            BASEEDIT_DisplayThemeLine(baseedit_ctrl_ptr, cross_rect);
        }

        //draw dividing line
        if ((0 < baseedit_ctrl_ptr->common_theme.dividing_line_width) &&
                (end_line_index > start_line_index))
        {
            for (i = start_line_index; i < end_line_index; i++)
            {
                //get line rect
                line_rect        = BASEEDIT_GetLineRect (baseedit_ctrl_ptr, i);
                line_rect.top    = (int16) (line_rect.bottom + 1);
                line_rect.bottom = (int16) (line_rect.top + baseedit_ctrl_ptr->common_theme.dividing_line_width - 1);

                //get crossed rect
                if (GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, line_rect)
                        && GUI_IntersectRect(&cross_rect, edit_client_rect, cross_rect))
                {
                    //draw line
                    GUI_FillRect (
                            (const GUI_LCD_DEV_INFO*) &baseedit_ctrl_ptr->edit_layer_info,
                            cross_rect,
                            baseedit_ctrl_ptr->common_theme.dividing_line_color);
                }
            }
        }
    }
}

/*****************************************************************************/
//  Description : get edit display rect height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetDispHeight(
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
        )
{
    uint16      display_height = 0;
    GUI_RECT_T  edit_rect = {0};

    //get text rect
    edit_rect = BASEEDIT_GetClientRect(baseedit_ctrl_ptr);

    //get display height
    display_height = (uint16)(edit_rect.bottom - edit_rect.top + 1);

    return (display_height);
}

/*****************************************************************************/
//  Description : get edit rect,except border
//  Global resource dependence :
//  Author: Jassmine
//  Note:
// 这里拿edit的客户区，该区域要扣除边框和margin区域
/*****************************************************************************/
PUBLIC GUI_RECT_T BASEEDIT_GetClientRect (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
        )
{
    BOOLEAN         is_border = TRUE;
    GUI_RECT_T      edit_rect = {0};
    GUI_RECT_T      zero_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    //edit is exist border
    if ((!baseedit_ctrl_ptr->is_permit_border) ||
            (GUI_BORDER_NONE == baseedit_ctrl_ptr->common_theme.border.type))
    {
        is_border = FALSE;
    }
    //	[MS] waiting for form set because child is_border = TRUE
    //     if ((is_border) && (!GUI_IsRectEmpty (base_ctrl_ptr->rect)))
    //     {
    //         edit_rect.left   = (int16) (base_ctrl_ptr->rect.left + baseedit_ctrl_ptr->common_theme.border.width);
    //         edit_rect.right  = (int16) (base_ctrl_ptr->rect.right - baseedit_ctrl_ptr->common_theme.border.width);
    //         edit_rect.top    = (int16) (base_ctrl_ptr->rect.top + baseedit_ctrl_ptr->common_theme.border.width);
    //         edit_rect.bottom = (int16) (base_ctrl_ptr->rect.bottom - baseedit_ctrl_ptr->common_theme.border.width);
    //     }
    //     else
    //	[MS] end

    if (TIMEDATE_DEFAUL_DISPLAYTYPE != CTRLBASEEDIT_GetTimeDateEditorDisplayType(base_ctrl_ptr))
    {
        if ((is_border) && (!GUI_IsRectEmpty (base_ctrl_ptr->rect)))
        {
            edit_rect.left   = (int16) (base_ctrl_ptr->rect.left + baseedit_ctrl_ptr->common_theme.border.width);
            edit_rect.top    = (int16) (base_ctrl_ptr->rect.top + baseedit_ctrl_ptr->common_theme.border.width);
        }
        else
        {
            edit_rect = base_ctrl_ptr->display_rect;
        }

        edit_rect.top += 30;
        edit_rect.bottom = edit_rect.top + 28;
        edit_rect.left += 6;
        if(TIMEDATE_COMBINE_DISPLAYTYPE != CTRLBASEEDIT_GetTimeDateEditorDisplayType(base_ctrl_ptr))
        {
            edit_rect.right = edit_rect.left + 228;
        }

        if (GUI_IsInvalidRectEx(edit_rect))
        {
            edit_rect = zero_rect;
        }

        SCI_ASSERT (edit_rect.top <= edit_rect.bottom); /*assert verified*/
    }	
    else 
    {    
        edit_rect = base_ctrl_ptr->rect;

        if ((GUIEDIT_STYLE_MULTI_DYN_DOWN == baseedit_ctrl_ptr->style) 
                || (GUIEDIT_STYLE_MULTI_DYN_UP == baseedit_ctrl_ptr->style)
                || (GUIEDIT_STYLE_SINGLE_DYN_DOWN == baseedit_ctrl_ptr->style)
                ||(GUIEDIT_STYLE_SINGLE_UP == baseedit_ctrl_ptr->style)
                ||(GUIEDIT_STYLE_MULTI_UP == baseedit_ctrl_ptr->style)
                ||baseedit_ctrl_ptr->is_inline_editor)
        {
            MMI_STRING_T	label_str = {0};

            label_str = GetBaseeditLabelText(baseedit_ctrl_ptr);
            if ( (PNULL != label_str.wstr_ptr) && (0 < label_str.wstr_len))
            {
                edit_rect.top = base_ctrl_ptr->rect.top + 30;
                edit_rect.bottom = base_ctrl_ptr->rect.bottom - 6;
            }
            else
            {
                edit_rect.bottom = base_ctrl_ptr->rect.bottom - 30;
                edit_rect.top = base_ctrl_ptr->rect.top;
                //edit_rect.top = edit_rect.bottom - 28;

            }

            if ((baseedit_ctrl_ptr->seperate_line != GUIEDIT_MMS_SEPERATE_LINE) &&(baseedit_ctrl_ptr->style != GUIEDIT_STYLE_MULTI_UP))
            {
                edit_rect.left = edit_rect.left + baseedit_ctrl_ptr->common_theme.margin_lr_space +1;
                edit_rect.right -= baseedit_ctrl_ptr->common_theme.margin_lr_space;
            }
        }

        edit_rect.top += baseedit_ctrl_ptr->common_theme.margin_tb_space;

        if (GUIEDIT_STYLE_MULTI_DYN_DOWN != baseedit_ctrl_ptr->style)  // for [MS]
        {
            if (GUIEDIT_STYLE_MULTI_DYN_UP != baseedit_ctrl_ptr->style) 
            {
                if (GUIEDIT_STYLE_SINGLE_DYN_DOWN!= baseedit_ctrl_ptr->style) 
                {

                    if(/*!baseedit_ctrl_ptr->is_inline_editor && */GUIEDIT_STYLE_SINGLE_UP != baseedit_ctrl_ptr->style)
                    {
                        edit_rect.bottom -= baseedit_ctrl_ptr->common_theme.margin_tb_space;
                        if(!baseedit_ctrl_ptr->is_inline_editor)
                        {
                            edit_rect.left += baseedit_ctrl_ptr->common_theme.margin_lr_space;
                            edit_rect.right -= baseedit_ctrl_ptr->common_theme.margin_lr_space;
                        }
                    }


                }	
            }
        }

        if (GUI_IsInvalidRectEx(edit_rect))
        {
            edit_rect = zero_rect;
        }

        SCI_ASSERT (edit_rect.top <= edit_rect.bottom); /*assert verified*/
    }
    return (edit_rect);
}

/*****************************************************************************/
//  Description : get edit line height
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetLineHeight(
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
        )
{
    uint16          font_height = 0;
    uint16          line_height = 0;

    //get font height
    font_height = BASEEDIT_GetFontHeight (baseedit_ctrl_ptr);

    //get line height
    line_height = (uint16) (font_height + baseedit_ctrl_ptr->common_theme.line_space);

    return line_height;
}
/*****************************************************************************/
//  Description : get font height
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 INLINEEDIT_GetFontHeight(
        GUIEDIT_TYPE_E type,
        GUI_FONT_T font
        )
{
    uint16          font_height = 0;

    switch (type)
    {
        case GUIEDIT_TYPE_TEXT:
            font_height = GUI_GetFontHeight (font, UNICODE_HANZI);
            break;

        case GUIEDIT_TYPE_PHONENUM:
        case GUIEDIT_TYPE_DIGITAL:
        case GUIEDIT_TYPE_PASSWORD:
        case GUIEDIT_TYPE_LIST:
        case GUIEDIT_TYPE_DATE:
        case GUIEDIT_TYPE_TIME:
        case GUIEDIT_TYPE_IP:
        default:
            font_height = GUI_GetFontHeight (font, UNICODE_ASCII);
            break;
    }

    return (font_height);
}

/*****************************************************************************/
//  Description : get edit line height
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 INLINEEDIT_GetLineHeight(
        GUIEDIT_TYPE_E type, 
        uint16 line_space, 
        GUIEDIT_TYPE_E font
        )
{
    uint16          font_height = 0;
    uint16          line_height = 0;

    //get font height
    font_height = INLINEEDIT_GetFontHeight(type, font);

    //get line height
    line_height = (uint16) (font_height + line_space);

    return line_height;
}

/*****************************************************************************/
//  Description : get edit line height
//  Global resource dependence :
//  Author: Jassmine
//  Note:// inline editor 
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetLineHeightEx(
        MMI_CTRL_ID_T    ctrl_id 
        )
{
    CTRLBASEEDIT_OBJ_T        *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if(PNULL == baseedit_ctrl_ptr)
    {
        return 0;
    }
    return BASEEDIT_GetLineHeight(baseedit_ctrl_ptr);
}

/*****************************************************************************/
//  Description : get edit line height
//  Global resource dependence :
//  Author: Jassmine
//  Note:// inline editor 
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_IsActive(
        MMI_CTRL_ID_T    ctrl_id 
        )
{
    CTRLBASEEDIT_OBJ_T        *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if(PNULL == baseedit_ctrl_ptr)
        return 0;
    return baseedit_ctrl_ptr->base_ctrl.is_active;
}    



/*****************************************************************************/
//  Description : get edit line rect,except dividing line,progress bar
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16  INLINEEDIT_GetTextHeight(
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
    uint16          line_height = 0;
    uint16 	     total_line_num = 0;
    uint16       edit_width = 0;

    line_height = INLINEEDIT_GetLineHeight(type, line_space, font);
    edit_width = (uint16) (edit_rect_right - edit_rect_left + 1);
    total_line_num = GUI_CalculateStringLinesByPixelNum (
            edit_width,
            str_ptr,
            str_len,
            font,
            char_space,
            0);  
    return line_height * total_line_num;
}

/*****************************************************************************/
//  Description : get edit line rect,except dividing line,progress bar
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T BASEEDIT_GetLineRect (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
        uint16              line_index          //in:from 0
        )
{
    uint16          line_height = 0;
    uint16          dividing_line_width = 0;
    uint16          top_line_index = 0;
    GUI_RECT_T      edit_rect = {0};
    GUI_RECT_T      line_rect = {0};

    //adjust line index
    top_line_index = BASEEDIT_GetTopLineIndex(baseedit_ctrl_ptr);
    //SCI_ASSERT (line_index >= top_line_index); /*assert verified*/
    //line_index = (uint16) (line_index - top_line_index);

    //get line height
    line_height = BASEEDIT_GetLineHeight(baseedit_ctrl_ptr);

    //get edit rect
    edit_rect = BASEEDIT_GetClientRect (baseedit_ctrl_ptr);

    if (GUI_IsRectEmpty (edit_rect))
    {
        return line_rect;
    }

    //get dividing line width
    dividing_line_width = baseedit_ctrl_ptr->common_theme.dividing_line_width;

    //get line rect
    line_rect = edit_rect;

    //adjust line rect top
    if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
    {
        AdjustRectTopByAlign (baseedit_ctrl_ptr, &line_rect);
    }

    if (TIMEDATE_DEFAUL_DISPLAYTYPE != CTRLBASEEDIT_GetTimeDateEditorDisplayType(baseedit_ctrl_ptr))
    {
        line_rect.top    = (int16) (line_rect.top + line_height * line_index - 0);
        line_rect.bottom = (int16) (line_rect.top + 28 - 0);
    }
    else 
    {
        //set line rect
        if(GUIEDIT_STYLE_MULTI_DYN_UP == baseedit_ctrl_ptr->style)
        {
            uint16      i = 0;
            uint16      edit_width = 0;
            uint16      total_line_num = 0;

            i = line_index;
            //change 36 to 30 for delete number not clear problem
            line_rect.bottom = (int16) (baseedit_ctrl_ptr->base_ctrl.rect.bottom) - 30;
            line_rect.top    = line_rect.bottom;
            // 
            //get edit box rect
            //edit_rect = BASEEDIT_GetClientRect (baseedit_ctrl_ptr);
            //get edit box width
            edit_width = (uint16) (edit_rect.right - edit_rect.left + 1);
            //get total number
            total_line_num = VTLBASEEDIT_GetLineNum (baseedit_ctrl_ptr, edit_width);

            do 
            {
                line_rect.top -= line_height ;
                i++;

            } while (i < total_line_num );
//delete by song 20201015 解决短信输入框中分割线跳动的问题
#if 0
            if (((line_height * (line_index + 1)) < (line_rect.bottom - line_rect.top)))
            {
                line_rect.bottom = (int16) (line_rect.top + line_height + 2 - dividing_line_width - 1) + 3;
            }
            else
#endif
            {
                line_rect.bottom = (int16) (line_rect.top + line_height + 2 - dividing_line_width - 1);
            }
        }
        else
        {
//delete by song 20201015 解决短信输入框中分割线跳动的问题
#if 0
            if (((line_height * (line_index + 1)) < (line_rect.bottom - line_rect.top)))
            {
                line_rect.top    = (int16) (line_rect.top + line_height * line_index - baseedit_ctrl_ptr->top_line_offset) + 3;
                line_rect.bottom = (int16) (line_rect.top + line_height - dividing_line_width - 1) + 3;
            }
            else
#endif
            {
                line_rect.top    = (int16) (line_rect.top + line_height * line_index - baseedit_ctrl_ptr->top_line_offset);
                line_rect.bottom = (int16) (line_rect.top + line_height - dividing_line_width - 1);
            }
        }
    }

    //SCI_ASSERT (line_rect.top < edit_rect.bottom); /*assert verified*/

    VTLBASEEDIT_CheckLineRect(baseedit_ctrl_ptr, &line_rect);

    //adjust rect
    if (line_rect.bottom > edit_rect.bottom)
    {
        line_rect.bottom = edit_rect.bottom;
    }

    return (line_rect);
}

/*****************************************************************************/
//  Description : get one line text rect,except margin
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T BASEEDIT_GetTextRect (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
        uint16              line_index      //in:from 0
        )
{
    GUI_RECT_T      text_rect = {0};

    //get line rect
    text_rect = BASEEDIT_GetLineRect (baseedit_ctrl_ptr, line_index);

    ////except margin
    //text_rect.left  = (uint16) (text_rect.left + baseedit_ctrl_ptr->common_theme.margin_lr_space);
    //text_rect.right = (uint16) (text_rect.right - baseedit_ctrl_ptr->common_theme.margin_lr_space);

    return (text_rect);
}

/*****************************************************************************/
//  Description : get line string length
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetLineStrLen (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
        uint16              line_index          //in:from 0
        )
{
    uint16      str_len = 0;

    if (line_index < baseedit_ctrl_ptr->total_line_num)
    {
        //is last line
        if ( (line_index + 1) == baseedit_ctrl_ptr->total_line_num)
        {
            if (BASEEDIT_IsFixedFormat (baseedit_ctrl_ptr))
            {
                str_len = (uint16) (baseedit_ctrl_ptr->str_len - baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos);
            }
            else
            {

                if (((GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)||(GUIEDIT_STYLE_SINGLE_UP== baseedit_ctrl_ptr->style)) &&
                        (0 < baseedit_ctrl_ptr->line_info_ptr[line_index+1].start_pos))
                {
                    str_len = (uint16) (baseedit_ctrl_ptr->line_info_ptr[line_index+1].start_pos - baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos);
                }
                else
                {
                    str_len = (uint16) (baseedit_ctrl_ptr->str_len - baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos);
                }
            }
        }
        else
        {
            str_len = (uint16) (baseedit_ctrl_ptr->line_info_ptr[line_index+1].start_pos - baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos);
        }
    }

    return (str_len);
}

/*****************************************************************************/
//  Description : adjust cursor/highlight rect left position by align
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_AdjustLeftByAlign (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
        uint16              line_index,         //in:
        GUI_RECT_T          *rect_ptr           //in/out:
        )
{
    uint16          string_pixel = 0;
    GUI_ALIGN_E     align = ALIGN_LEFT;

    //get string pixel
    string_pixel = GetEditLineStrPixel (
            baseedit_ctrl_ptr,
            line_index,
            (uint16) (rect_ptr->right - rect_ptr->left + 1));

    //get align
    align = VTLBASEEDIT_GetAlign (baseedit_ctrl_ptr);

    //get sting start position
    switch (align)
    {
        case ALIGN_RIGHT:
        case ALIGN_RVMIDDLE:
            if (rect_ptr->right > string_pixel)
            {
                //set left start
                if (0 < string_pixel)
                {
                    rect_ptr->left = (int16) (rect_ptr->right - string_pixel + 1);
                }
                else
                {
                    rect_ptr->left = rect_ptr->right;
                }
            }

            break;

        case ALIGN_HMIDDLE:
        case ALIGN_HVMIDDLE:
            if (rect_ptr->left + rect_ptr->right > string_pixel)
            {
                //set left start
                rect_ptr->left = (int16) ( (rect_ptr->left + rect_ptr->right - string_pixel + 1) >> 1);
            }

            break;

        default:
            break;
    }
}

/*****************************************************************************/
//  Description : get font height
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetFontHeight (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
        )
{
    uint16          font_height = 0;

    switch (baseedit_ctrl_ptr->type)
    {
        case GUIEDIT_TYPE_TEXT:
            font_height = GUI_GetFontHeight (baseedit_ctrl_ptr->common_theme.font.font, UNICODE_HANZI);
            break;

    case GUIEDIT_TYPE_PHONENUM:
#ifdef ZDT_EDIT_NUM_IMG_SUPPORT
			if(baseedit_ctrl_ptr->use_phnum_img)
			{
				font_height = baseedit_ctrl_ptr->phnum_img_height;
			}
			else
			{
				font_height = GUI_GetFontHeight(baseedit_ctrl_ptr->common_theme.font.font,UNICODE_ASCII);
			}
			break;
#endif
    case GUIEDIT_TYPE_DIGITAL:
    case GUIEDIT_TYPE_PASSWORD:
    case GUIEDIT_TYPE_LIST:
    case GUIEDIT_TYPE_DATE:
    case GUIEDIT_TYPE_TIME:
    case GUIEDIT_TYPE_IP:
    default:
        font_height = GUI_GetFontHeight (baseedit_ctrl_ptr->common_theme.font.font, UNICODE_ASCII);
        break;
    }

    return (font_height);
}


/*****************************************************************************/
//  Description : get edit separator char
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC char BASEEDIT_GetSeparatorChar (
        GUIEDIT_SEPARATOR_E    separator_style
        )
{
    char        separator_char = 0;

    switch (separator_style)
    {
        case GUIEDIT_SEPARATOR_DOT:
            separator_char = '.';
            break;

        case GUIEDIT_SEPARATOR_LINE:
            separator_char = '-';
            break;

        case GUIEDIT_SEPARATOR_COMMA:
            separator_char = ',';
            break;

        case GUIEDIT_SEPARATOR_BLANK:
            separator_char = ' ';
            break;

        case GUIEDIT_SEPARATOR_COLON:
            separator_char = ':';
            break;

        case GUIEDIT_SEPARATOR_SOLIDUS:
            separator_char = '/';
            break;

        case GUIEDIT_SEPARATOR_SEMICOLON:
            separator_char = ';';
            break;

        default:
            //SCI_TRACE_LOW:"GetSeparatorChar:separator style %d is error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_INTERNAL_843_112_2_18_3_16_6_133, (uint8*) "d", separator_style);
            break;
    }

    return (separator_char);
}

/*****************************************************************************/
//  Description : edit is fixed format,for example time,date,IP
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_IsFixedFormat (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
        )
{
    BOOLEAN     result = FALSE;

    switch (baseedit_ctrl_ptr->type)
    {
        case GUIEDIT_TYPE_TEXT:
        case GUIEDIT_TYPE_PHONENUM:
        case GUIEDIT_TYPE_DIGITAL:
        case GUIEDIT_TYPE_PASSWORD:
        case GUIEDIT_TYPE_LIST:
            result = FALSE;
            break;

        case GUIEDIT_TYPE_DATE:
        case GUIEDIT_TYPE_TIME:
        case GUIEDIT_TYPE_IP:
            result = TRUE;
            break;

        default:
            //SCI_TRACE_LOW:"GUIEDIT_IsFixedFormat:edit type %d is error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_INTERNAL_1147_112_2_18_3_16_7_134, (uint8*) "d", baseedit_ctrl_ptr->type);
            break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : convert line index,when display from right and bottom,need convert
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC uint16 BASEEDIT_ConvertLineIndex (
//    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
//    uint16              index
//)
//{
//    uint16  line_index = index;

//    if ( (baseedit_ctrl_ptr->total_line_num < baseedit_ctrl_ptr->page_line_num) &&
//            (GUIEDIT_DISPLAY_DIR_RB == baseedit_ctrl_ptr->display_dir))
//    {
//        line_index = (uint16) (baseedit_ctrl_ptr->page_line_num + line_index - baseedit_ctrl_ptr->total_line_num);
//    }

//    return (line_index);
//}

/*****************************************************************************/
//  Description : 用来判断字符是否为换行符
//  Global resource dependence :
//  Author: Jassmine
//  Note:
// 由于密码风格不需要换行，所以永远也不可能是换行符
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_IsNewLineChar (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
        uint16              char_index
        )
{
    BOOLEAN is_new_line = FALSE;

#if 0//UNISOC_MMI_Delete
    if ((GUIEDIT_TYPE_PASSWORD != baseedit_ctrl_ptr->type) &&
            (0 < char_index) && (char_index <= baseedit_ctrl_ptr->str_len) &&
            (GUI_IsNewlineChar (baseedit_ctrl_ptr->str_ptr[char_index - 1])))
    {
        //carriage return or line feed
        is_new_line = TRUE;
    }
#else
    if ((0 < char_index) && (char_index <= baseedit_ctrl_ptr->str_len) &&
            (GUI_IsNewlineChar (baseedit_ctrl_ptr->str_ptr[char_index - 1])))
    {
        //carriage return or line feed
        is_new_line = TRUE;
    }
#endif

    return is_new_line;
}

/*****************************************************************************/
//  Description : get line number in one page
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetPageLineNum (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
        )
{
    uint16          font_height = 0;
    uint16          line_height = 0;
    uint16          page_height = 0;
    uint16          line_num = 0;
    GUI_RECT_T      edit_rect = {0};

    //get font height
    font_height = BASEEDIT_GetFontHeight (baseedit_ctrl_ptr);

    //get line height
    line_height = BASEEDIT_GetLineHeight(baseedit_ctrl_ptr);

    //get edit rect
    edit_rect = BASEEDIT_GetClientRect (baseedit_ctrl_ptr);

    if (!GUI_IsRectEmpty (edit_rect))
    {
        //get page height
        page_height = (uint16) (edit_rect.bottom - edit_rect.top + 1);

        line_num = (uint16) (page_height / line_height);

        if ( (page_height % line_height) >= font_height)
        {
            line_num++;
        }

        //modify line number for single edit

        if (((GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)||(GUIEDIT_STYLE_SINGLE_UP== baseedit_ctrl_ptr->style)) && (0 < line_num))
        {
            line_num = 1;
        }

        if (line_num == 0)
        {
            line_num = 1;
        }
    }

    return (line_num);
}

/*****************************************************************************/
//  Description : get total content height
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 BASEEDIT_GetTotalHeight(
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
        )
{
    uint16  line_height = 0;
    line_height = BASEEDIT_GetLineHeight(baseedit_ctrl_ptr);

    return line_height * baseedit_ctrl_ptr->total_line_num;
}

/*****************************************************************************/
//  Description : get display top line index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetTopLineIndex (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
        )
{
    uint16  line_height = 0;
    int32   top_index = 0;

    line_height = BASEEDIT_GetLineHeight(baseedit_ctrl_ptr);

    top_index = baseedit_ctrl_ptr->top_line_offset / line_height;
    if (top_index < 0)
    {
        top_index = 0;
    }
    else if (top_index >= (int32)baseedit_ctrl_ptr->total_line_num)
    {
        if (baseedit_ctrl_ptr->total_line_num > 0)
        {
            top_index = baseedit_ctrl_ptr->total_line_num - 1;
        }
        else
        {
            top_index = 0;
        }
    }

    return (uint16)top_index;
}

/*****************************************************************************/
//  Description : get display bottom line index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetBottomLineIndex (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
        )
{
    uint16  line_height = 0;
    int32   bottom_index = 0;
    int32   bottom_offset = 0;

    line_height = BASEEDIT_GetLineHeight(baseedit_ctrl_ptr);
    bottom_offset = baseedit_ctrl_ptr->top_line_offset + BASEEDIT_GetDispHeight(baseedit_ctrl_ptr);

    bottom_index = bottom_offset / line_height;

    if (bottom_index < 0)
    {
        bottom_index = 0;
    }
    else if (bottom_index >= (int32)baseedit_ctrl_ptr->total_line_num)
    {
        if (baseedit_ctrl_ptr->total_line_num > 0)
        {
            bottom_index = baseedit_ctrl_ptr->total_line_num - 1;
        }
        else
        {
            bottom_index = 0;
        }
    }

    return (uint16)bottom_index;
}

/*****************************************************************************/
//  Description : get top line offset range
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_GetTopLineOffsetRange(
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
        int32               *min_top_line_offset_ptr,
        int32               *max_top_line_offset_ptr
        )
{
    uint16  display_height = BASEEDIT_GetDispHeight(baseedit_ctrl_ptr);
    uint32  total_height = BASEEDIT_GetTotalHeight(baseedit_ctrl_ptr);
    int32   min_top_line_offset = 0;
    int32   max_top_line_offset = 0;


    if ((GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)||(GUIEDIT_STYLE_SINGLE_UP== baseedit_ctrl_ptr->style))       // 单行
    {
        min_top_line_offset = 0;
        max_top_line_offset = 0;
    }
    else if ((total_height < display_height) &&
            (GUIEDIT_DISPLAY_DIR_RB == baseedit_ctrl_ptr->display_dir))  // 多行，从下往上显示
    {
        min_top_line_offset = (total_height - display_height);
        max_top_line_offset = min_top_line_offset;
    }
    else
    {
        min_top_line_offset = 0;

        if (total_height > display_height)
        {
            max_top_line_offset = total_height - display_height;
        }
        else
        {
            max_top_line_offset = 0;
        }
    }

    if (PNULL != min_top_line_offset_ptr)
    {
        *min_top_line_offset_ptr = min_top_line_offset;
    }

    if (PNULL != max_top_line_offset_ptr)
    {
        *max_top_line_offset_ptr = max_top_line_offset;
    }
}

/*****************************************************************************/
//  Description : adjust rect top position by align
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustRectTopByAlign (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
        GUI_RECT_T          *rect_ptr           //in/out:
        )
{
    uint16          font_height = 0;
    GUI_ALIGN_E     align = ALIGN_LEFT;

    //get font height
    font_height = BASEEDIT_GetFontHeight (baseedit_ctrl_ptr);

    //get align
    align = VTLBASEEDIT_GetAlign (baseedit_ctrl_ptr);

    //get sting start position
    switch (align)
    {
        case ALIGN_BOTTOM:
            //set cursor top
            rect_ptr->top = (int16) (rect_ptr->bottom - font_height + 1);
            break;

        case ALIGN_VMIDDLE:
        case ALIGN_LVMIDDLE:
        case ALIGN_RVMIDDLE:
        case ALIGN_HVMIDDLE:
            //set cursor top
            rect_ptr->top = (int16) ( (rect_ptr->top + rect_ptr->bottom - font_height + 1) >> 1);
            break;

        default:
            break;
    }
}

/*****************************************************************************/
//  Description : get line string pixel
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetEditLineStrPixel (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
        uint16              line_index,     //in:
        uint16              rect_width      //in:
        )
{
    wchar       *str_ptr = PNULL;
    uint16      str_len = 0;
    uint16      char_num = 0;
    uint16      string_pixel = 0;

    //get string
    str_ptr = baseedit_ctrl_ptr->str_ptr + baseedit_ctrl_ptr->line_info_ptr[line_index].start_pos;
    str_len = BASEEDIT_GetLineStrLen (baseedit_ctrl_ptr, line_index);

    //get string pixel
    if(GUI_GetStringInfoInLine (
                str_ptr, str_len,
                baseedit_ctrl_ptr->common_theme.font.font,
                (uint8) (baseedit_ctrl_ptr->common_theme.char_space),
                rect_width,
                &char_num,
                &string_pixel,
                FALSE))
    {
        return (string_pixel);
    };

    return 0;
}

/*****************************************************************************/
//  Description : notify parent edit change msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SendChangeMsg (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
        BOOLEAN             is_line_num_change //in:
        )
{
    BOOLEAN                 result = FALSE;
    GUIFORM_NOTIFY_LINE_T   notify_line = {0};
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    //post parent control
    if (0 != MMK_GetParentCtrlHandle (base_ctrl_ptr->handle))
    {
        //set notify param
        notify_line.is_line_num_change = is_line_num_change;
        notify_line.ctrl_handle = base_ctrl_ptr->handle;
        CTRLMSG_SendNotifyEx (base_ctrl_ptr->handle, MSG_NOTIFY_LINE_CHANGE, &notify_line);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : edit's parent is control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_IsParentCtrl(
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
        )
{
    BOOLEAN         result = FALSE;
    MMI_HANDLE_T    parent_handle = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    if (PNULL != base_ctrl_ptr)
    {
        //get parent control
        parent_handle = MMK_GetParentCtrlHandle(base_ctrl_ptr->handle);

        if (0 != parent_handle)
        {
            result = TRUE;
        }
    }

    return (result);
}

PUBLIC void BASEEDIT_MirrorRect(
        GUI_RECT_T    *rect_dest_ptr
        )
{
    GUI_RECT_T          temp_rect = {0};
    uint16      lcd_height = 0;
    uint16      lcd_width = 0;
    temp_rect = *rect_dest_ptr;

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    rect_dest_ptr->left   = lcd_width - temp_rect.right;
    rect_dest_ptr->right  = lcd_width -temp_rect.left;
}
