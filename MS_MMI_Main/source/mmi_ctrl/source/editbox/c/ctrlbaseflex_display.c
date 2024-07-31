/*****************************************************************************
 ** File Name:      ctrlbaseflex_display.c                                    *
 ** Author:         hua.fang                                                  *
 ** Date:           09/12/2012                                                *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
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
#include "ctrlbaseflex.h"
#include "mmi_gui_trc.h"
#include "guistring.h"
#include "mmk_type.h"
#include "mmi_theme.h"
#include "spml_api.h"
#include "guiprgbox.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
#define GUIEDIT_SPACE	0x0020
#define GUIEDIT_LEFT_PARENTHESES	0x0028
#define GUIEDIT_RIGHT_PARENTHESES	0x0029   
#define GUIEDIT_PLUS	0x002B
#define GUIEDIT_SEMICOLON	0x3b
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
//  Description : 显示部分内容
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DisplayPart (
        CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
        GUIEDIT_UPDATE_T    *update_ptr
        );

/*****************************************************************************/
//  Description : display text dividing line
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayTextDividingLine (
        CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
        );

/*****************************************************************************/
//  Description : display First message and list number when non-active
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetFirstAndListNumber (	MMI_STRING_T	*text_str, 
        wchar	*dest_ptr, 
        uint8	item_num 
        );

/*****************************************************************************/
//  Description : display text string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayTextString (
        CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr, //in:
        BOOLEAN             is_update_bg,   //in:
        GUIEDIT_UPDATE_T    *update_ptr     //in:
        );

/*****************************************************************************/
//  Description : is the last line in one page,rect is small
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//LOCAL BOOLEAN IsLastLinePageSmallRect (
//    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
//    uint16              line_index
//);

/*****************************************************************************/
//  Description : display scroll bar
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayScrollBar (
        CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
        BOOLEAN             is_update
        );

/*****************************************************************************/
//  Description : display window softkey
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayWinSoftkey (
        CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
        );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : display information bar,include im icon and remaining characters
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_DisplayInfoBar (
        CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr
        )
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;

    if (base_ctrl_ptr->is_active)
    {
        // call virtual method to display left num
        VTLBASEFLEX_DisplayLeftNum (baseflex_ctrl_ptr);

        //update title
        GUIWIN_UpdateTitle (base_ctrl_ptr->win_handle);
    }
}

/*****************************************************************************/
//  Description : update text display for moving cursor
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_UpdateForMoveCursor (
        CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,
        BOOLEAN                     is_update_str,
        uint16			            prev_cursor_pos
        )
{
    GUIEDIT_UPDATE_T    update_info = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (is_update_str)
    {
        //set update info
        update_info.update_mode      = GUIEDIT_UPDATE_PART;
        update_info.start_line_index = BASEEDIT_GetTopLineIndex(baseedit_ctrl_ptr);
        update_info.end_line_index   = BASEEDIT_GetBottomLineIndex (baseedit_ctrl_ptr);

        //display string
        DisplayTextString (baseflex_ctrl_ptr, TRUE, &update_info);

        //display scroll bar
        DisplayScrollBar (baseflex_ctrl_ptr, TRUE);
    }

    //set display cursor
    baseedit_ctrl_ptr->cursor.is_display    = TRUE;
    baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;

    //display cursor
    BASEFLEX_DisplayCursor (baseflex_ctrl_ptr);

    if (prev_cursor_pos != baseedit_ctrl_ptr->cursor.cur_pos)
    {
        //post move cursor msg
        BASEEDIT_NotifyCursorMsg (baseedit_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : update edit display
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_UpdateDisplay (
        CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr, //in：
        GUIEDIT_UPDATE_T        *update_ptr     //in:may PNULL for fixed format
        )
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (PNULL != update_ptr && MMK_IsFocusWin (base_ctrl_ptr->win_handle))
    {
        //stop cursor timer
        BASEFLEX_StopCursorTimer (baseflex_ctrl_ptr);

        switch (update_ptr->update_mode)
        {
            case GUIEDIT_UPDATE_PART:
                DisplayPart (baseflex_ctrl_ptr, update_ptr);
                break;

            case GUIEDIT_UPDATE_ALL:
                VTLBASEEDIT_DisplayAll (baseedit_ctrl_ptr, TRUE);
                break;

            default:
                break;
        }
    }
}

/*****************************************************************************/
//  Description : 显示所有内容
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BaseFlexCtrlDisplayAll (
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
        BOOLEAN             is_update_prgbox
        )
{
    GUI_RECT_T          cross_rect = {0};
    GUIEDIT_UPDATE_T    update_info = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*)baseedit_ctrl_ptr;

    //display bg
    //get crossed rect
    if (GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, base_ctrl_ptr->rect))
    {
        BASEEDIT_DisplayBg (baseedit_ctrl_ptr, cross_rect);   	 

        if((GUIEDIT_STYLE_MULTI_DYN_DOWN == baseedit_ctrl_ptr->style)
                || (GUIEDIT_STYLE_MULTI_DYN_UP == baseedit_ctrl_ptr->style)
                ||(GUIEDIT_STYLE_SINGLE_DYN_DOWN == baseedit_ctrl_ptr->style)
                ||(GUIEDIT_STYLE_SINGLE_UP == baseedit_ctrl_ptr->style)
                ||(GUIEDIT_STYLE_MULTI_UP == baseedit_ctrl_ptr->style)
                || baseedit_ctrl_ptr->is_inline_editor)
        {

            if (baseedit_ctrl_ptr->seperate_line == GUIEDIT_MMS_SEPERATE_LINE)
            {
                // display seperate line
                BASEEDIT_DisplaySeperateLine(baseedit_ctrl_ptr, cross_rect);
            }
            else
            {
                // display under line
                if (!baseedit_ctrl_ptr->is_permit_border)
                {
                    BASEEDIT_DisplayThemeLine(baseedit_ctrl_ptr, cross_rect);
                }
            }

            // display label and im
            BASEEDIT_DisplayEditExtraArea(baseedit_ctrl_ptr, cross_rect);
        }
		
        //display border
        if ((baseedit_ctrl_ptr->is_permit_border) && (TRUE == base_ctrl_ptr->is_active))
        {
            GUI_RECT_T          line_rect = {0};
            MMITHEME_EDIT_TEXT_DISPLAY_T  display_theme = {0};

            MMITHEME_GetTextDisplayTheme(&display_theme);	

            baseedit_ctrl_ptr->common_theme.border = display_theme.border;


            line_rect = BASEEDIT_GetClientRect(baseedit_ctrl_ptr);
            line_rect.top-=(baseedit_ctrl_ptr->common_theme.border.width+1);
            line_rect.bottom+=(baseedit_ctrl_ptr->common_theme.border.width+1);
            line_rect.left-=(baseedit_ctrl_ptr->common_theme.border.width+1);
            line_rect.right+=(baseedit_ctrl_ptr->common_theme.border.width+1);

            GUI_DisplayBorder (
				line_rect,
				base_ctrl_ptr->display_rect,
				&baseedit_ctrl_ptr->common_theme.border,
				&baseedit_ctrl_ptr->edit_layer_info);
        }
		else if ((FALSE == baseedit_ctrl_ptr->is_permit_border) && (TRUE == base_ctrl_ptr->is_active))
		{
			
		}
		else
		{
			GUI_RECT_T          line_rect = {0};
            MMITHEME_EDIT_TEXT_DISPLAY_T  display_theme = {0};

            MMITHEME_GetTextDisplayTheme(&display_theme);	

            baseedit_ctrl_ptr->common_theme.border = display_theme.border;
            if(TRUE == baseedit_ctrl_ptr->is_update_borderColor)
            {
                baseedit_ctrl_ptr->common_theme.border.color = baseedit_ctrl_ptr->base_ctrl.border.color;
            }
            else
            {
                baseedit_ctrl_ptr->common_theme.border.color = 0xef9d; //MMI_GRAY_WHITE_COLOR
            }
            line_rect = BASEEDIT_GetClientRect(baseedit_ctrl_ptr);
            line_rect.top-=(baseedit_ctrl_ptr->common_theme.border.width+1);
            line_rect.bottom+=(baseedit_ctrl_ptr->common_theme.border.width+1);
            line_rect.left-=(baseedit_ctrl_ptr->common_theme.border.width+1);
            line_rect.right+=(baseedit_ctrl_ptr->common_theme.border.width+1);

            GUI_DisplayBorder (
				line_rect,
				base_ctrl_ptr->display_rect,
				&baseedit_ctrl_ptr->common_theme.border,
				&baseedit_ctrl_ptr->edit_layer_info);
		}

        //display information bar
        BASEFLEX_DisplayInfoBar (baseflex_ctrl_ptr);


        // Add for display text count when lose focused
        if ((FALSE == base_ctrl_ptr->is_active)||(baseedit_ctrl_ptr->seperate_line == GUIEDIT_MMS_SEPERATE_LINE))
            VTLBASEFLEX_DisplayLeftNum (baseflex_ctrl_ptr);

        //display text dividing line
        DisplayTextDividingLine (baseflex_ctrl_ptr);

        //display string
        update_info.update_mode = GUIEDIT_UPDATE_ALL;
        DisplayTextString (baseflex_ctrl_ptr, FALSE, &update_info);
        
        //display scroll bar
        if((GUIEDIT_STYLE_SINGLE_DYN_DOWN == baseedit_ctrl_ptr->style)||(GUIEDIT_STYLE_SINGLE_UP == baseedit_ctrl_ptr->style))
        {
            if (0 == MMK_GetParentCtrlHandle (base_ctrl_ptr->handle))
            {
                DisplayScrollBar (baseflex_ctrl_ptr, is_update_prgbox);
            }
        }
        else
        {
            DisplayScrollBar (baseflex_ctrl_ptr, is_update_prgbox);
        }

        //display softkey
        DisplayWinSoftkey (baseflex_ctrl_ptr);

        //display highlight
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, TRUE);

        //set display cursor
        if (baseflex_ctrl_ptr->baseedit_ctrl.str_len != baseflex_ctrl_ptr->baseedit_ctrl.str_max_len)
        {
            baseedit_ctrl_ptr->cursor.is_display    = TRUE;
        }
        baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;

        //display cursor
        BASEFLEX_DisplayCursor (baseflex_ctrl_ptr);

#ifdef GUIF_CLIPBOARD
        BASEFLEX_ShowSlider (baseflex_ctrl_ptr);
#endif
    }
}

/*****************************************************************************/
//  Description : display the number of remaining characters
//  Global resource dependence :
//  Author: Jassmine
//  Note: 自需求要求，当前输入法改用文本而勿用图标，即对本函数功能进行了扩展
//	      实现输入法提示信息和剩余字符个数信息的真正分离，独立控制
/*****************************************************************************/
PUBLIC void BaseFlexCtrlDisplayLeftNum (
        CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
        )
{
    wchar           remain_num[GUIEDIT_NUM_INFO_MAX_LEN] = {0};
    uint16          remain_num_len = 0;
    wchar           text_num[GUIEDIT_NUM_INFO_MAX_LEN] = {0};
    uint16          text_num_len = 0;
    MMI_STRING_T    sz_im_text = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;
    GUISTR_STYLE_T  str_style = {0}; /*lint !e64*/
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_WORDBREAK;
    GUI_RECT_T      str_rect = {0};

#if 0 // for display input method
    wchar           input_num[GUIEDIT_NUM_INFO_MAX_LEN] = {0};
    uint16          input_num_len = 0;
    MMI_STRING_T    sz_input_text = {0};
    GUI_RECT_T      input_rect = {0};
#endif

#ifdef MAINLCD_SIZE_128X64//syy modify 2010.12.8 -begin
    GUI_RECT_T       rect = {0, 0, 127, 14};

    GUI_FillRect (&baseedit_ctrl_ptr->edit_layer_info, rect, 0xffff);
#endif//syy modify 2010.12.8 -end

    if (baseflex_ctrl_ptr->is_disp_num_info || baseedit_ctrl_ptr->is_disp_im_icon)
    {
        //get remaining number string
        if (baseflex_ctrl_ptr->is_disp_num_info)
        {
            VTLBASEFLEX_GetLeftNumInfo (
                    baseflex_ctrl_ptr,
                    GUIEDIT_NUM_INFO_MAX_LEN,
                    remain_num,
                    &remain_num_len);
        }

#if 1
        /*扩展功能，加上有需要的地方显示输入法提示文本*/
        if (baseedit_ctrl_ptr->is_disp_im_icon &&    
                (0 != baseedit_ctrl_ptr->im_ctrl_handle) &&
                ((GUIIM_INPUT_TYPE_KEYBOARD == GUIIM_GetInput (baseedit_ctrl_ptr->im_ctrl_handle)) ||
                 (GUIIM_INPUT_TYPE_QWERT == GUIIM_GetInput (baseedit_ctrl_ptr->im_ctrl_handle))))
        {
            //get method text
            sz_im_text = GUIIM_ImGetText (baseedit_ctrl_ptr->im_ctrl_handle);
        }
#endif
        //set font
        GUIWIN_SetTitleSubTextParam (
                base_ctrl_ptr->win_handle,
                PNULL,
                &baseedit_ctrl_ptr->common_theme.info_font,
                PNULL);

        if (0 != sz_im_text.wstr_len)
        {
            MMIAPICOM_Wstrcpy (text_num, sz_im_text.wstr_ptr);
            MMIAPICOM_Wstrcat (text_num, L" ");
            MMIAPICOM_Wstrncat (text_num, remain_num,
                    MIN (GUIEDIT_NUM_INFO_MAX_LEN - sz_im_text.wstr_len - 1, remain_num_len));
        }
        else
        {
            MMIAPICOM_Wstrcpy (text_num, remain_num);
        }

        text_num_len = MMIAPICOM_Wstrlen (text_num);

        if (baseedit_ctrl_ptr-> seperate_line == GUIEDIT_MMS_SEPERATE_LINE)
        {
            //display string
            str_rect = base_ctrl_ptr->rect;
            str_rect.left = str_rect.left + 49 ;
            str_rect.right = str_rect.left + 80 ;
            str_rect.bottom = str_rect.bottom -4;

            str_style.align = ALIGN_BOTTOM;
            str_style.font = baseedit_ctrl_ptr->common_theme.font.font;

            if (TRUE == base_ctrl_ptr->is_active)
                str_style.font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_EDIT_DEFAULT_FONT);
            else
            {
                str_style.font_color = baseedit_ctrl_ptr->common_theme.non_focused_color;
            }

            sz_im_text.wstr_len = text_num_len;
            sz_im_text.wstr_ptr = text_num;

            if (base_ctrl_ptr->display_rect.bottom < str_rect.bottom)
            {
                return;
            }

            GUISTR_DrawTextToLCDInRect( 
                    (const GUI_LCD_DEV_INFO*)&baseedit_ctrl_ptr->edit_layer_info,
                    (const GUI_RECT_T*)&str_rect,       //the fixed display area
                    (const GUI_RECT_T*)&str_rect,       //用户要剪切的实际区域
                    (const MMI_STRING_T*)&sz_im_text,
                    &str_style,
                    str_state,
                    GUISTR_TEXT_DIR_AUTO
                    );

#if 0 // For input method text
            //display input
            input_rect = base_ctrl_ptr->rect;
            input_rect.left = input_rect.left + 170 ;
            input_rect.right= input_rect.left + 48 ;
            input_rect.bottom = input_rect.bottom -4;

            str_style.align = ALIGN_BOTTOM;
            str_style.font = baseedit_ctrl_ptr->common_theme.font.font;
            str_style.font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_TITLE);

            sz_input_text.wstr_len = input_num_len;
            sz_input_text.wstr_ptr = input_num;

            GUISTR_DrawTextToLCDInRect( 
                    (const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    (const GUI_RECT_T*)&input_rect,       //the fixed display area
                    (const GUI_RECT_T*)&input_rect,       //用户要剪切的实际区域
                    (const MMI_STRING_T*)&sz_input_text,
                    &str_style,
                    str_state,
                    GUISTR_TEXT_DIR_AUTO
                    );
#endif


        }
        else if (baseedit_ctrl_ptr->is_inline_editor && baseflex_ctrl_ptr->is_disp_num_info)
        {
            //display string
            str_rect = base_ctrl_ptr->rect;
            str_rect.top = str_rect.bottom-2-16;
            str_rect.left = str_rect.left + 5;
            str_rect.right = str_rect.left + 80 ;
            str_rect.bottom = str_rect.bottom -2;

            str_style.align = ALIGN_BOTTOM;
            str_style.font =CAF_FONT_16;

            if (TRUE == base_ctrl_ptr->is_active)
                str_style.font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_TITLE);
            else
            {
                str_style.font_color = baseedit_ctrl_ptr->common_theme.non_focused_color;
            }

            sz_im_text.wstr_len = text_num_len;
            sz_im_text.wstr_ptr = text_num;
#ifdef UI_MULTILAYER_SUPPORT	
            UILAYER_ClearRect(&baseedit_ctrl_ptr->edit_layer_info, str_rect);
#endif
            BASEEDIT_DisplayBg (baseedit_ctrl_ptr, str_rect);
            //GUI_FillRect (&baseedit_ctrl_ptr->edit_layer_info, str_rect, baseedit_ctrl_ptr->common_theme.bg.color);	
            GUISTR_DrawTextToLCDInRect( 
                    (const GUI_LCD_DEV_INFO*)&baseedit_ctrl_ptr->edit_layer_info,
                    (const GUI_RECT_T*)&str_rect,       //the fixed display area
                    (const GUI_RECT_T*)&str_rect,       //用户要剪切的实际区域
                    (const MMI_STRING_T*)&sz_im_text,
                    &str_style,
                    str_state,
                    GUISTR_TEXT_DIR_AUTO
                    );

        }			
#if 1
        //display string
        GUIWIN_SetTitleSubText (base_ctrl_ptr->win_handle,
                TRUE,
                text_num,
                text_num_len,
                FALSE);

#endif

    }

    else
    {
        //display string
        GUIWIN_SetTitleSubText (base_ctrl_ptr->win_handle,
                FALSE,
                PNULL,
                0,
                FALSE);
    }
}

/*****************************************************************************/
//  Description : 获取剩余字数信息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 BaseFlexCtrlGetLeftNumInfo ( // [ret] the left char num
        CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,     // [in]
        uint16                  str_max_len,            // [in] the max size of char
        wchar                   *str_ptr,               // [out] left char num string
        uint16                  *str_len_ptr            // [out] length of string
        )
{
    uint16  left_char_num = 0;
    uint8   remain_num[GUIEDIT_NUM_INFO_MAX_LEN] = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //get the number of remaining characters
    if (baseedit_ctrl_ptr->str_max_len > baseedit_ctrl_ptr->str_len)
    {
        if (GUIEDIT_TYPE_PHONENUM == baseedit_ctrl_ptr->type)
        {
            if ( !baseedit_ctrl_ptr->is_first_plus)
            {
                left_char_num = (uint16)(baseedit_ctrl_ptr->str_max_len - baseedit_ctrl_ptr->str_len - 1);
            }
            else if (baseedit_ctrl_ptr->is_first_plus)
            {
                left_char_num = (uint16)(baseedit_ctrl_ptr->str_max_len - baseedit_ctrl_ptr->str_len);
            }
            if (left_char_num >= baseedit_ctrl_ptr->str_max_len)
            {
                left_char_num = baseedit_ctrl_ptr->str_max_len - 1;
            }
        }
        else
        {
            left_char_num = (uint16)(baseedit_ctrl_ptr->str_max_len - baseedit_ctrl_ptr->str_len);
        }
    }
    else
    {
        left_char_num = 0;
    }

    if (PNULL != str_ptr && PNULL != str_len_ptr && 0 != str_max_len)
    {
        *str_len_ptr = (uint16) sprintf ( (char *) remain_num, "%d", left_char_num);
        SCI_ASSERT (*str_len_ptr < GUIEDIT_NUM_INFO_MAX_LEN); /*assert verified*/

        //convert wchar
        MMI_STRNTOWSTR (str_ptr, str_max_len,
                (const uint8 *) remain_num, GUIEDIT_NUM_INFO_MAX_LEN,
                *str_len_ptr);
    }

    return (left_char_num);
}

/*****************************************************************************/
//  Description : 显示部分内容
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DisplayPart (
        CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
        GUIEDIT_UPDATE_T    *update_ptr
        )
{
    GUI_RECT_T          cross_rect = {0};
    GUIEDIT_UPDATE_T    update_info = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if((GUIEDIT_STYLE_MULTI_DYN_DOWN == baseedit_ctrl_ptr->style)
            || (GUIEDIT_STYLE_MULTI_DYN_UP == baseedit_ctrl_ptr->style)
            ||(GUIEDIT_STYLE_SINGLE_DYN_DOWN == baseedit_ctrl_ptr->style)
            ||(GUIEDIT_STYLE_SINGLE_UP == baseedit_ctrl_ptr->style)
            ||(GUIEDIT_STYLE_MULTI_UP == baseedit_ctrl_ptr->style)
            ||baseedit_ctrl_ptr->is_inline_editor)

    {
        if (GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, base_ctrl_ptr->rect))
        {


            if (baseedit_ctrl_ptr->seperate_line == GUIEDIT_MMS_SEPERATE_LINE)
            {
                // display seperate line
                BASEEDIT_DisplaySeperateLine(baseedit_ctrl_ptr, cross_rect);
            }
            else
            {
                // display under line
                if (!baseedit_ctrl_ptr->is_permit_border)
                {
                    BASEEDIT_DisplayThemeLine(baseedit_ctrl_ptr, cross_rect);
                }
            }
            // display label and im
            BASEEDIT_DisplayEditExtraArea(baseedit_ctrl_ptr, cross_rect);
        }
    }
    //display information bar
    BASEFLEX_DisplayInfoBar (baseflex_ctrl_ptr);

    //display string
    DisplayTextString (baseflex_ctrl_ptr, TRUE, update_ptr);

    //display scroll bar
    DisplayScrollBar (baseflex_ctrl_ptr, TRUE);

    //display softkey
    DisplayWinSoftkey (baseflex_ctrl_ptr);

    //display highlight
    BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, TRUE);

    //set display cursor
    baseedit_ctrl_ptr->cursor.is_display    = TRUE;
    baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;

    //display cursor
    BASEFLEX_DisplayCursor (baseflex_ctrl_ptr);

#ifdef GUIF_CLIPBOARD
    BASEFLEX_ShowSlider (baseflex_ctrl_ptr);
#endif
}

/*****************************************************************************/
//  Description : display text dividing line
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayTextDividingLine (
        CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
        )
{
    uint16          i = 0;
    uint16          top_line_index = 0;
    uint16          bottom_line_index = 0;
    GUI_RECT_T      line_rect = {0};
    GUI_RECT_T      cross_rect = {0};
    GUI_RECT_T      edit_client_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (0 < baseedit_ctrl_ptr->common_theme.dividing_line_width)
    {
        top_line_index = BASEEDIT_GetTopLineIndex(baseedit_ctrl_ptr);
        bottom_line_index = top_line_index + BASEEDIT_GetPageLineNum(baseedit_ctrl_ptr);

        edit_client_rect = BASEEDIT_GetClientRect(baseedit_ctrl_ptr);

        //draw all line
        for (i = top_line_index; i <= bottom_line_index; i++)
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
/*****************************************************************************/
//  Description : display First message and list number when non-active
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetFirstAndListNumber (	MMI_STRING_T	*text_str, 
        wchar	*dest_ptr, 
        uint8	item_num 
        )
{
    uint8 item_num_str[10] = {0};
    uint8 item_num_str_len =0;
    wchar item_num_wstr[10] = {0};
    uint16 pos = 0;
    wchar ch = NULL;

    // Integer to Wstring
    MMIAPICOM_Int2Str(item_num,(uint8*)item_num_str, 10);
    MMIAPICOM_StrToWstr(item_num_str , item_num_wstr);

    // find first ";"
    for(pos = 0; pos < text_str->wstr_len ; pos++)
    {
        ch = text_str->wstr_ptr[pos];
        if ( ch == GUIEDIT_SPACE)
        {
            break;
        }
        dest_ptr[pos] = ch;
    }
    dest_ptr[pos++] = GUIEDIT_SPACE ;
    dest_ptr[pos++] = GUIEDIT_LEFT_PARENTHESES;
    dest_ptr[pos++] = GUIEDIT_PLUS;
    SCI_MEMCPY((void*)(dest_ptr + pos) , item_num_wstr , strlen(item_num_str) * sizeof(wchar));
    pos += strlen(item_num_str);
    dest_ptr[pos++] = GUIEDIT_RIGHT_PARENTHESES;
    text_str->wstr_ptr = dest_ptr;
    text_str->wstr_len = pos;
}

/*****************************************************************************/
//  Description : display text string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayTextString (
        CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr, //in:
        BOOLEAN             is_update_bg,   //in:
        GUIEDIT_UPDATE_T    *update_ptr     //in:
        )
{
    wchar           *str_ptr = PNULL;
    uint16          i = 0;
    uint16          start_line_index = 0;
    uint16          end_line_index = 0;
    GUI_RECT_T      text_rect = {0};
    GUI_RECT_T      cross_rect = {0};
    GUI_ALIGN_E     align = ALIGN_LEFT;
    MMI_STRING_T    text_str = {0};
    GUISTR_STYLE_T  str_style = {0};/*lint !e64*/
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE;
    GUI_RECT_T      client_rect = {0};
    GUI_RECT_T      edit_client_rect = {0};
    uint8 item_num = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;
    wchar *line_str = PNULL ;
    //uint8 language_type = MMISET_GetCurrentLanguageType();

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != update_ptr); /*assert verified*/
    if (PNULL == update_ptr)
    {
        return;
    }

    client_rect = MMITHEME_GetClientRectEx (base_ctrl_ptr->win_handle);
    edit_client_rect = BASEEDIT_GetClientRect (baseedit_ctrl_ptr);
    if ((GUIEDIT_STYLE_MULTI_DYN_UP == baseedit_ctrl_ptr->style)||(GUIEDIT_STYLE_SINGLE_UP == baseedit_ctrl_ptr->style))
    {
        edit_client_rect = base_ctrl_ptr->rect;
    }
    //set start and end line index
    switch (update_ptr->update_mode)
    {
        case GUIEDIT_UPDATE_PART:
            start_line_index = update_ptr->start_line_index;
            end_line_index   = update_ptr->end_line_index;

            // [MS] fix when BASEFLEX_AddString line change start line index not start at 0
            if (GUIEDIT_STYLE_MULTI_DYN_UP == baseedit_ctrl_ptr->style)
            {
                start_line_index = 0;	
            }
            break;

        case GUIEDIT_UPDATE_ALL:
            start_line_index = BASEEDIT_GetTopLineIndex(baseedit_ctrl_ptr);
            end_line_index   = BASEEDIT_GetBottomLineIndex (baseedit_ctrl_ptr);
            break;

        default:
            SCI_PASSERT (FALSE, ("DisplayTextString:update mode %d is error!", update_ptr->update_mode)); /*assert verified*/
            break;
    }

    str_ptr = VTLBASEEDIT_GetDispStr (
            baseedit_ctrl_ptr,
            0,
            baseedit_ctrl_ptr->str_len);

    if((GUIEDIT_STYLE_SINGLE_DYN_DOWN == baseedit_ctrl_ptr->style) 
            && (FALSE == base_ctrl_ptr->is_active))
    {
        end_line_index = start_line_index; // [MS] when this type not active will only show first line
        item_num = GUIEDIT_GetListItemNum(base_ctrl_ptr->ctrl_id);
        if (0 != item_num)
        {		
            item_num -= 1;
        }
    }

    //update bg
    if (is_update_bg)
    {
        BASEEDIT_UpdateTextBg (baseedit_ctrl_ptr, start_line_index, end_line_index);
    }

    if (0 != baseedit_ctrl_ptr->str_len)
    {
        for (i = start_line_index; i <= end_line_index; i++)
        {
            //get text rect
            text_rect = BASEEDIT_GetTextRect (baseedit_ctrl_ptr, i);

            //get crossed rect
            if (GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, text_rect)
                    && GUI_IntersectRect (&cross_rect, client_rect, cross_rect)
                    && GUI_IntersectRect(&cross_rect, edit_client_rect, cross_rect))
            {
                //get line string len
                text_str.wstr_len = BASEEDIT_GetLineStrLen (baseedit_ctrl_ptr, i);

                if (0 < text_str.wstr_len)
                {
                    text_str.wstr_ptr = &str_ptr[baseedit_ctrl_ptr->line_info_ptr[i].start_pos];

                    if((GUIEDIT_STYLE_SINGLE_DYN_DOWN == baseedit_ctrl_ptr->style) // [MS]
                            && (FALSE == base_ctrl_ptr->is_active)
                            && (0 != item_num))
                    {
                        line_str = SCI_ALLOC_APPZ((text_str.wstr_len + 10) * sizeof(wchar));
                        SetFirstAndListNumber(&text_str,line_str,item_num);
                    }

                    //set align
                    align = VTLBASEEDIT_GetAlign (baseedit_ctrl_ptr);

                    //is last line in one page
                    //if ( (IsLastLinePageSmallRect (baseflex_ctrl_ptr, i)) &&
                    //        (0 < baseedit_ctrl_ptr->common_theme.line_space / 2))
                    //{
                    //    text_rect.top = (int16) (text_rect.top + baseedit_ctrl_ptr->common_theme.line_space / 2 - 1);
                    //}

                    //set string style
                    str_style.font       = baseedit_ctrl_ptr->common_theme.font.font;
                    str_style.font_color = baseedit_ctrl_ptr->common_theme.font.color;
                    str_style.align      = align;
                    str_style.char_space = baseedit_ctrl_ptr->common_theme.char_space;

                    if (GUIEDIT_DISPLAY_DIR_LT == baseedit_ctrl_ptr->display_dir)
                    {
                        str_state |= GUISTR_STATE_ALIGN_BY_DIR;
                    }
                    if ((GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style) 
                            &&(TRUE == base_ctrl_ptr->is_active)
                            &&(TRUE == baseedit_ctrl_ptr->is_display_inverse))
                    {
                        str_state |= GUISTR_STATE_INVERSE;
                    }

                    if(TEXT_DIR_RTL == SPMLAPI_GetTextDirection(text_str.wstr_ptr, text_str.wstr_len))
                    {
                        //display string
                        GUISTR_DrawTextToLCDInRect (
                                (const GUI_LCD_DEV_INFO*) &baseedit_ctrl_ptr->edit_layer_info,
                                (const GUI_RECT_T*) &text_rect,
                                (const GUI_RECT_T*) &cross_rect,
                                &text_str,
                                &str_style,
                                str_state,
                                TEXT_DIR_RTL);
                    }
#ifdef ZDT_EDIT_NUM_IMG_SUPPORT
                    else if(baseedit_ctrl_ptr->use_phnum_img)
                    {
                        GUISTR_DrawTextImgToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                            (const GUI_RECT_T*)&text_rect,
                            (const GUI_RECT_T*)&cross_rect,
                            &text_str,
                            &str_style,
                            str_state,
                            baseedit_ctrl_ptr->line_info_ptr[i].direction);
                    }
#endif 
                    else
                    {
                        //display string
                        GUISTR_DrawTextToLCDInRect (
                                (const GUI_LCD_DEV_INFO*) &baseedit_ctrl_ptr->edit_layer_info,
                                (const GUI_RECT_T*) &text_rect,
                                (const GUI_RECT_T*) &cross_rect,
                                &text_str,
                                &str_style,
                                str_state,
                                TEXT_DIR_AUTO);//baseedit_ctrl_ptr->line_info_ptr[i].direction);
                    }                    

                    if((GUIEDIT_STYLE_SINGLE_DYN_DOWN == baseedit_ctrl_ptr->style) // [MS]
                            && (FALSE == base_ctrl_ptr->is_active))
                    {
                        SCI_FREE(line_str);
                    }
                }
            }
        }
    }
    else if (0 != baseflex_ctrl_ptr->default_str_len)
    {
        SPML_TEXT_DIR_T text_dir = TEXT_DIR_LTR;

        text_rect = BASEEDIT_GetTextRect (baseedit_ctrl_ptr, 0);
        text_rect.bottom = base_ctrl_ptr->rect.bottom;  // BUGBUG: 需要扣除边框和margin

        //get crossed rect
        if (GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, text_rect) &&
                GUI_IntersectRect (&cross_rect, client_rect, cross_rect) &&
                GUI_IntersectRect(&cross_rect, edit_client_rect, cross_rect))
        {
            text_str.wstr_ptr = baseflex_ctrl_ptr->default_str_ptr;
            text_str.wstr_len = baseflex_ctrl_ptr->default_str_len;

            if(TRUE == MMISET_IsCurrentLanguageTextAlignRTLDirection())
            {
                align = ALIGN_RIGHT;
            }
            else
            {
                //set align
                align = ALIGN_LEFT;//GUIEDIT_GetAlign(edit_ctrl_ptr);
            }
            //set string style
            str_style.font       = baseedit_ctrl_ptr->common_theme.font.font;
            str_style.font_color = baseflex_ctrl_ptr->flex_theme.default_str_color;
            str_style.align      = align;
            str_style.char_space = baseedit_ctrl_ptr->common_theme.char_space;
            str_style.line_space = baseedit_ctrl_ptr->common_theme.line_space;

            str_state &= (~GUISTR_STATE_SINGLE_LINE);       // 默认多行显示


            if ((GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)||(GUIEDIT_STYLE_SINGLE_UP== baseedit_ctrl_ptr->style))
            {
                str_state |= GUISTR_STATE_NOT_HALF_CHAR;
                str_state |= GUISTR_STATE_SINGLE_LINE;
            }

            if (GUIEDIT_DISPLAY_DIR_LT == baseedit_ctrl_ptr->display_dir)
            {
                str_state |= GUISTR_STATE_ALIGN_BY_DIR;
            }

            text_dir = SPMLAPI_GetTextDirection (
                    baseflex_ctrl_ptr->default_str_ptr,
                    baseflex_ctrl_ptr->default_str_len);

#ifdef ZDT_EDIT_NUM_IMG_SUPPORT
			if(baseedit_ctrl_ptr->use_phnum_img)
			{
				GUISTR_DrawTextImgToLCDInRect(
					(const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
					(const GUI_RECT_T*)&text_rect,
					(const GUI_RECT_T*)&cross_rect,
					&text_str,
					&str_style,
					str_state,
					text_dir);

			}
			else
#endif
			{
            GUISTR_DrawTextToLCDInRect (
                    (const GUI_LCD_DEV_INFO*) &baseedit_ctrl_ptr->edit_layer_info,
                    (const GUI_RECT_T*) &text_rect,
                    (const GUI_RECT_T*) &cross_rect,
                    &text_str,
                    &str_style,
                    str_state,
                    text_dir);
			}
        }
    }
}

/*****************************************************************************/
//  Description : display scroll bar
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayScrollBar (
        CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
        BOOLEAN             is_update
        )
{
    //uint16          cursor_line_index = 0;
    GUI_RECT_T      prg_rect = {0};
    GUI_RECT_T      cross_rect = {0};
    int32           top_line_offset = 0;
    int32           min_top_line_offset = 0;
    int32           max_top_line_offset = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (PNULL != baseedit_ctrl_ptr->prgbox_ctrl_ptr)
    {
        prg_rect = BASEFLEX_GetScrollBarRect (baseflex_ctrl_ptr);

        //get crossed rect
        if (GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, prg_rect))
        {
            //update bg
            BASEEDIT_DisplayBg (baseedit_ctrl_ptr, cross_rect);

            //get current cursor line index
            //cursor_line_index = BASEEDIT_GetCursorLineIndex (baseedit_ctrl_ptr, baseedit_ctrl_ptr->cursor.cur_pos);

            BASEEDIT_GetTopLineOffsetRange(
                    baseedit_ctrl_ptr, 
                    &min_top_line_offset,
                    &max_top_line_offset);

            top_line_offset = baseedit_ctrl_ptr->top_line_offset;

            if (top_line_offset < min_top_line_offset)
            {
                top_line_offset = 0;
            }
            else if (top_line_offset > max_top_line_offset)
            {
                top_line_offset = max_top_line_offset;
            }

            //set current positon
            GUIPRGBOX_SetPosByPtr (
                    is_update,
                    top_line_offset,
                    top_line_offset,
                    baseedit_ctrl_ptr->prgbox_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : display window softkey
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayWinSoftkey (
        CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
        )
{
    BOOLEAN         result = FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //im display softkey
    if ((0 != baseedit_ctrl_ptr->im_ctrl_handle) &&
            (GUIIM_DisplaySoftkey (baseedit_ctrl_ptr->im_ctrl_handle)))
    {
        result = TRUE;
    }

    //display edit softkey
    if (!result)
    {
        BASEEDIT_DisplaySoftkey (baseedit_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : is the last line in one page,rect is small
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//LOCAL BOOLEAN IsLastLinePageSmallRect (
//    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
//    uint16              line_index
//)
//{
//    BOOLEAN     result = FALSE;
//    uint16      font_height = 0;
//    uint16      line_height = 0;
//    uint16      page_height = 0;
//    GUI_RECT_T  edit_rect = {0};
//    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

//    //convert line index
//    line_index = BASEEDIT_ConvertLineIndex (baseedit_ctrl_ptr, line_index);

//    //is the last line in one page
//    if ( (line_index - baseedit_ctrl_ptr->top_line_index + 1) == baseedit_ctrl_ptr->page_line_num)
//    {
//        //get font height
//        font_height = BASEEDIT_GetFontHeight (baseedit_ctrl_ptr);

//        //get line height
//        line_height = (uint16) (font_height + baseedit_ctrl_ptr->common_theme.line_space);

//        //get edit rect
//        edit_rect = BASEEDIT_GetClientRect (baseedit_ctrl_ptr);

//        //get page height
//        page_height = (uint16) (edit_rect.bottom - edit_rect.top + 1 - (baseedit_ctrl_ptr->common_theme.margin_tb_space << 1));

//        //rect is normal
//        if ( (page_height % line_height) >= (font_height + baseedit_ctrl_ptr->common_theme.line_space / 2))
//        {
//            result = TRUE;
//        }
//    }

//    return (result);
//}

