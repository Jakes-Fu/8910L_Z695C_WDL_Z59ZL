/*****************************************************************************
** File Name:      mmitheme_prgbox.c                                         *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#define _MMITHEME_PRGBOX_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "guiblock.h"
#include "mmitheme_prgbox.h"
#include "mmi_image.h"
#include "mmidisplay_data.h"
#include "mmi_theme.h"
#include "mmi_text.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef MONOCHROME_LCD_SUPPORT 
#define MMITHEME_PROCESS_BG_COLOR   MMI_BLACK_COLOR
#define MMITHEME_PROCESS_FG_COLOR   MMI_WHITE_COLOR
#else
#define MMITHEME_PROCESS_BG_COLOR   MMI_DARK_GRAY_COLOR
#define MMITHEME_PROCESS_FG_COLOR   MMI_GREEN_COLOR
#endif

#define MMITHEME_VOLUME_BRIGHTNRESS_BG_COLOR RGB8882RGB565(0x202020) 
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

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get vertical scroll box theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetVerticalScrollTheme(
                                            MMITHEME_PRGBOX_TYPE_E        theme_type,
                                            MMITHEME_PRGBOX_VER_SCROLL_T   *theme_ptr //in/out:
                                            )
{
    //SCI_ASSERT(PNULL != theme_ptr);/*assert verified*/
    if (PNULL == theme_ptr)
    {
        return;
    }
    
    if( MMITHEME_PRGBOX_DEFAULT_TYPE == theme_type )
    {
#ifndef MMI_RES_LOW_COST
        //set groove arrow
        theme_ptr->up_arrow_id         = IMAGE_SCROLL_UP_ARROW;
        theme_ptr->up_focus_arrow_id   = IMAGE_SCROLL_UP_ARROW_FOCUS;
        theme_ptr->down_arrow_id       = IMAGE_SCROLL_DOWN_ARROW;
        theme_ptr->down_focus_arrow_id = IMAGE_SCROLL_DOWN_ARROW_FOCUS;
#endif
        
        //set scroll bar
        theme_ptr->scroll_bar_id = IMAGE_SCROLL_INDICATOR;
#ifndef MMI_RES_LOW_COST        
        //set groove bg
        theme_ptr->groove_bg_id = IMAGE_SCROLL_VER_GROOVE;
#endif

#if defined(GUIF_PRGBOX_AUTOHIDE)
        theme_ptr->thumb_img_id = IMAGE_SCROLL_THUMB;
        theme_ptr->hide_tick = 2000;
        theme_ptr->out_tick = 100;
        theme_ptr->thumb_threshold = 5;
        theme_ptr->bar_width = 4;
#endif
    }
    else
    {
#ifndef MMI_RES_LOW_COST       
		//set groove arrow
        theme_ptr->up_arrow_id         = IMAGE_SCROLL_UP_ARROW_2;
        theme_ptr->up_focus_arrow_id   = IMAGE_SCROLL_UP_ARROW_FOCUS_2;
        theme_ptr->down_arrow_id       = IMAGE_SCROLL_DOWN_ARROW_2;
        theme_ptr->down_focus_arrow_id = IMAGE_SCROLL_DOWN_ARROW_FOCUS_2;
        
        //set scroll bar
        theme_ptr->scroll_bar_id = IMAGE_SCROLL_VER_BAR_2;
#ifdef GUIF_PRGBOX_AUTOHIDE
        //set groove bg
        theme_ptr->groove_bg_id = IMAGE_SCROLL_VER_BG;
#else
		theme_ptr->groove_bg_id = IMAGE_SCROLL_VER_BAR_2;
#endif
#endif
    }
}

/*****************************************************************************/
//  Description : get horizontal scroll box theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetHorizontalScrollTheme(
                                              MMITHEME_PRGBOX_TYPE_E       theme_type,
                                              MMITHEME_PRGBOX_HOR_SCROLL_T *theme_ptr //in/out:
                                              )
{
    //SCI_ASSERT(PNULL != theme_ptr);/*assert verified*/
    if (PNULL == theme_ptr)
    {
        return;
    }
    
    if( MMITHEME_PRGBOX_DEFAULT_TYPE == theme_type )
    {
#ifndef MMI_RES_LOW_COST 
        //set groove arrow
        theme_ptr->left_arrow_id        = IMAGE_SCROLL_LEFT_ARROW;
        theme_ptr->left_focus_arrow_id  = IMAGE_SCROLL_LEFT_ARROW_FOCUS;
        theme_ptr->right_arrow_id       = IMAGE_SCROLL_RIGHT_ARROW;
        theme_ptr->right_focus_arrow_id = IMAGE_SCROLL_RIGHT_ARROW_FOCUS;
#endif        
        //set scroll bar
        theme_ptr->scroll_bar_id = IMAGE_SCROLL_INDICATOR;
#ifndef MMI_RES_LOW_COST         
        //set groove bg
        theme_ptr->groove_bg_id = IMAGE_SCROLL_HOR_GROOVE;
#endif
    }
    else
    {
#ifndef MMI_RES_LOW_COST  
		//set groove arrow
        theme_ptr->left_arrow_id        = IMAGE_SCROLL_LEFT_ARROW_2;
        theme_ptr->left_focus_arrow_id  = IMAGE_SCROLL_LEFT_ARROW_FOCUS_2;
        theme_ptr->right_arrow_id       = IMAGE_SCROLL_RIGHT_ARROW_2;
        theme_ptr->right_focus_arrow_id = IMAGE_SCROLL_RIGHT_ARROW_FOCUS_2;
        
        //set scroll bar
        theme_ptr->scroll_bar_id = IMAGE_SCROLL_VER_BAR_2;
        
        //set groove bg
#ifdef MMI_COMPLIE_E
        theme_ptr->groove_bg_id = IMAGE_SCROLL_HOR_GROOVE_2;
#else
        theme_ptr->groove_bg_id = NULL;
#endif
#endif
    }
}

/*****************************************************************************/
//  Description : get process box theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetProcessTheme(
									 MMITHEME_PRGBOX_TYPE_E        theme_type,
                                     MMITHEME_PRGBOX_PROCESS_T     *theme_ptr //in/out:
                                     )
{
    //SCI_ASSERT(PNULL != theme_ptr);/*assert verified*/
    if (PNULL == theme_ptr)
    {
        return;
    }
    
    //set process color
    theme_ptr->fg_color = MMITHEME_PROCESS_FG_COLOR;
    theme_ptr->bg_color = MMITHEME_PROCESS_BG_COLOR;
    
    if((theme_type == MMITHEME_PRGBOX_TYPE_VOLUME) || (theme_type == MMITHEME_PRGBOX_TYPE_BRIGHTNESS) 
        || (theme_type == MMITHEME_PRGBOX_TYPE_NPS_SCORE) || (theme_type == MMITHEME_PRGBOX_TYPE_KEYPAD_VOLUME))
    {
        theme_ptr->fg_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT);
        theme_ptr->bg_color = MMITHEME_VOLUME_BRIGHTNRESS_BG_COLOR;
        theme_ptr->bg_rect_color = MMI_BLACK_COLOR;
        theme_ptr->focus_other_color = MMITHEME_VOLUME_BRIGHTNRESS_BG_COLOR;//MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_2);
        theme_ptr->unfocus_fg_color = MMI_OPACITY60_WHITE_COLOR;//MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_3);
        theme_ptr->text_color = MMI_WHITE_COLOR;
        theme_ptr->title_font = MMI_DEFAULT_BIG_FONT;
        theme_ptr->text_font = MMI_DEFAULT_NORMAL_FONT;
        theme_ptr->label_text_focus_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT);
        
        if(theme_type == MMITHEME_PRGBOX_TYPE_VOLUME)
        {
            theme_ptr->text1_id = STXT_VOLUME_MK;
            theme_ptr->text2_id = STXT_SIMPLEX_MODE_SEL_SILENT;
            theme_ptr->text3_id = STXT_SIMPLEX_MODE_SEL_BEEP;
        }
        else if(theme_type == MMITHEME_PRGBOX_TYPE_BRIGHTNESS)
        {
            //nhl modify the brigghtness string
            //theme_ptr->text1_id = STR_BACKLIGHT_INTENSITY;
            theme_ptr->text1_id = STR_PHONE_BRIGHTNESS_EXT01;
        }
        else if (theme_type == MMITHEME_PRGBOX_TYPE_KEYPAD_VOLUME)
        {
            theme_ptr->text1_id = STR_SETT_KEYPAD_VOLUME_EXT01;
            theme_ptr->text2_id = STXT_SIMPLEX_MODE_SEL_SILENT;
        }
        else
        {
            theme_ptr->text1_id = STXT_RETURN;
        }
    }

#ifndef MMI_RES_LOW_COST 
    //////////////////////////////////////////////////////////////////////////
    // added by feng.xiao for NEWMS00180461
    // set progress img
    theme_ptr->fg_img_id = IMAGE_CONTROL_PROGRESS;
    theme_ptr->bg_img_id = IMAGE_CONTROL_PROGRESS_BG;
#endif   
    theme_ptr->bg_type = GUI_BG_IMG;
    theme_ptr->is_dialog = FALSE;
    
    MMITHEME_GetDialogWaitingIndTheme(theme_ptr);
    
    return;
}

/*****************************************************************************/
//  Description : get waiting indicator theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetDialogWaitingIndTheme(
                                     MMITHEME_PRGBOX_PROCESS_T     *theme_ptr //in/out:
                                     )
{
    if (PNULL == theme_ptr)
    {
        return;
    }
    
    switch(MMITHEME_GetAccentId())
    {
#ifdef MMI_COMPILE_E
        case MMITHEME_ACCENT_COLOR_GREEN:
            theme_ptr->waiting_indicator_focused_img_id = IMAGE_WAITING_INDICATOR_GREEN;            
            theme_ptr->waiting_indicator_unfocused_img_id = IMAGE_WAITING_INDICATOR_GREEN_DARK;
            break;
        case MMITHEME_ACCENT_COLOR_COPPER:
            theme_ptr->waiting_indicator_focused_img_id = IMAGE_WAITING_INDICATOR_COPPER;            
            theme_ptr->waiting_indicator_unfocused_img_id = IMAGE_WAITING_INDICATOR_COPPER_DARK;
            break;
        case MMITHEME_ACCENT_COLOR_BLUE:
            theme_ptr->waiting_indicator_focused_img_id = IMAGE_WAITING_INDICATOR_BLUE;            
            theme_ptr->waiting_indicator_unfocused_img_id = IMAGE_WAITING_INDICATOR_BLUE_DARK;
            break;
        case MMITHEME_ACCENT_COLOR_VIOLENT:
            theme_ptr->waiting_indicator_focused_img_id = IMAGE_WAITING_INDICATOR_VIOLET;
            theme_ptr->waiting_indicator_unfocused_img_id = IMAGE_WAITING_INDICATOR_VIOLET_DARK;            
            break;
        case MMITHEME_ACCENT_COLOR_RED:
            theme_ptr->waiting_indicator_focused_img_id = IMAGE_WAITING_INDICATOR_RED;            
            theme_ptr->waiting_indicator_unfocused_img_id = IMAGE_WAITING_INDICATOR_RED_DARK;            
            break;
#endif
        case MMITHEME_ACCENT_COLOR_YELLOW:
            theme_ptr->waiting_indicator_focused_img_id = IMAGE_WAITING_INDICATOR_YELLOW;            
            theme_ptr->waiting_indicator_unfocused_img_id = IMAGE_WAITING_INDICATOR_YELLOW_DARK;            
            break;

         default:
            theme_ptr->waiting_indicator_focused_img_id = IMAGE_WAITING_INDICATOR_YELLOW;            
            theme_ptr->waiting_indicator_unfocused_img_id = IMAGE_WAITING_INDICATOR_YELLOW_DARK;            
            break;
    }
}

/*****************************************************************************/
//  Description : get process box theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetPrgLcdDev(
                                  GUI_LCD_DEV_INFO *lcd_dev_info_ptr//in/out:
                                  )
{
    if (PNULL != lcd_dev_info_ptr)
    {
        //set background lcd dev
        lcd_dev_info_ptr->lcd_id   = GUI_MAIN_LCD_ID;
        lcd_dev_info_ptr->block_id = GUI_BLOCK_MAIN;
    }
    return;
}

/*==============================================================================
Description: 获取prgbox的初始控制宽度
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
1 控制高度根据使用场景定义
2 因为在非自动隐藏的时候，控制宽度和显示宽度是一致的，故而使用默认命名
==============================================================================*/
PUBLIC uint16 MMITHEME_GetScrollBarWidth(void)
{
#if defined(GUIF_PRGBOX_AUTOHIDE)
    return 0;
#else
    return SCROLL_BAR_WIDTH;
#endif
}


/*****************************************************************************/
// 	Description : get the slide win scroll bar width
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetSlideScrollBarWidth(void)
{
#if defined(GUIF_PRGBOX_AUTOHIDE)
    return MMITHEME_SLIDEWIN_SCROLLBAR_WIDTH;
#else
    return MMITHEME_SLIDEWIN_SCROLLBAR_WIDTH;
#endif
}

/*****************************************************************************/
//  Description : get scroll bar width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetScrollBarWidthByState(
                                                BOOLEAN is_narrow_state
                                                )
{
    uint16      bar_width = 0;

    if (is_narrow_state)
    {
        bar_width = MMITHEME_GetSlideScrollBarWidth();
    }
    else
    {
        bar_width = MMITHEME_GetScrollBarWidth();
    }

    return (bar_width);
}
