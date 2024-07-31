/*****************************************************************************
** File Name:      mmitheme_headeranddivider.c                                          *
** Author:         Great.Tian                                                *
** Date:           03/9/2011                                                 *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe the mmi theme               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2011        Xiaoqing.lu      Create								     *
**																		     *
*****************************************************************************/



/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmitheme_headeranddivider.h"
#include "mmi_theme.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guilcd.h"
#include "mmi_image.h"
#include "mmi_anim.h"
#include "mmidisplay_data.h"
#include "guisoftkey.h"
#include "mmi_menutable.h"
#include "guilistbox.h"
#include "mmi_text.h"
#include "mmi_id.h"
#include "mmi_default.h"
#include "mmi_textfun.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmitv_out.h"
#ifndef WIN32
#include "arm_reg.h"
#endif
#include "guibutton.h"
#ifdef _SMMI_UNIT_TEST_
#define LOCAL 
#endif
#include "guiref_scale.h"
#include "guifont.h"
#include "mmiidle_export.h"
#include "mmk_app.h"
//#include "mmi_wallpaper_export.h"
#include "mmiset_id.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmitheme_special_func.h"
#include "block_mem.h"
//#include "mmiset_display.h"
#include "guires.h"
#include "mmi_nv.h"
#ifdef UI_P3D_SUPPORT
#include "mmi3d_export.h"
#endif
#include "mmi_custom_define.h"
#include "mmi_position.h"
#include "guistring.h"
#include "ui_layer.h"
#include "mmi_color.h"
#include "mmi_resource.h"
#include "tp_id.h"
#include "mmi_modu_main.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
//headeranddivider
#define HEADERANDDIVIDER_SCROLL_TIMEOUT		    500  //ms

/**---------------------------------------------------------------------------*
 **                         public function                                   *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : get the title style, is called by the title control
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetHeaderAndDividerStyle(
                                      MMI_HEADERANDDIVIDER_STYLE_T* headeranddivider_style_ptr
                                      )
{

    //SCI_PASSERT(PNULL != title_style_ptr, ("MMITHEME_GetTitleStyle: the param is PNULL!")); /*assert verified*/
    if (PNULL == headeranddivider_style_ptr)
    {
        return FALSE;
    }
    SCI_MEMSET(headeranddivider_style_ptr, 0x00, sizeof(MMI_HEADERANDDIVIDER_STYLE_T));/*assert verified*/

#ifdef MONOCHROME_LCD_SUPPORT 
//    headeranddivider_style_ptr->bg_image    = IMAGE_NULL;		//±³¾°Í¼Æ¬
    headeranddivider_style_ptr->bg_color     = MMI_WHITE_COLOR; 
	headeranddivider_style_ptr->font         = MMI_TITLE_TEXT_FONT;		//×ÖÌå
    headeranddivider_style_ptr->font_color   = MMI_WHITE_COLOR;	//×ÖÌåÑÕÉ«
    headeranddivider_style_ptr->char_space   = 0;
#else
// #ifdef MAINLCD_SIZE_240X320 //@for light function modify
//         title_style_ptr->bg_image     = IMAGE_COMMON_TITLE_BAR;//IMAGE_TITLE_BAR;		//±³¾°Í¼Æ¬
// #else
//        headeranddivider_style_ptr->bg_image     = MMITHEME_GetWinHeaderAndDividerBar();    //±³¾°Í¼Æ¬
//#endif
#endif

    headeranddivider_style_ptr->bg_color     = MMI_BACKGROUND_COLOR; //MMI_BLACK_COLOR;
	headeranddivider_style_ptr->line_color	 = MMI_WHITE_COLOR;
#ifdef GUIF_TITLE_PDASTYLE
	// PDA ÖØÐÂÈ¡±³¾°Í¼Æ¬
//	headeranddivider_style_ptr->bg_image     = MMITHEME_GetWinHeaderAndDividerBar();    //±³¾°Í¼Æ¬
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {
        headeranddivider_style_ptr->font         = MMI_DEFAULT_BIG_FONT;		//×ÖÌå
    }
    else
#endif
#endif
    {
        headeranddivider_style_ptr->font         = MMI_TITLE_TEXT_FONT;		//×ÖÌå
    }
    headeranddivider_style_ptr->font_color   = MMITHEME_GetCurThemeFontColor(MMI_THEME_TITLE);//MMI_WHITE_COLOR;			//×ÖÌåÑÕÉ«
    headeranddivider_style_ptr->char_space   = 0;

#ifdef GUIF_TITLE_PDASTYLE
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)   
    if (MMITHEME_IsIstyle())
    {
        headeranddivider_style_ptr->align_type = ALIGN_HMIDDLE;
    }
    else
#endif
    {
        headeranddivider_style_ptr->align_type = ALIGN_LVMIDDLE;
    }
    headeranddivider_style_ptr->title_text_margin = MMI_TITLE_TEXT_MARGIN_PDA;
    headeranddivider_style_ptr->sub_font       = SONG_FONT_12;
#else
//fixed by hongjun.jia.176X220ºÍ128X160ÕâÁ½ÖÖ·Ö±æÂÊÏÂ£¬title¿¿×óÏÔÊ¾
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160) || defined (MAINLCD_SIZE_240X320)
    headeranddivider_style_ptr->align_type = ALIGN_LVMIDDLE;
#else
	headeranddivider_style_ptr->align_type = ALIGN_HMIDDLE;
#endif
    headeranddivider_style_ptr->title_text_margin = MMI_TITLE_TEXT_L_MARGIN;
    headeranddivider_style_ptr->sub_font       = SONG_FONT_12;
#endif

    headeranddivider_style_ptr->scroll_time    = HEADERANDDIVIDER_SCROLL_TIMEOUT;
    headeranddivider_style_ptr->scroll_pixel_step = MMITHEME_ScrollPixelNum();

//    headeranddivider_style_ptr->button_margin = MMI_TITLE_ARROW_MARGIN;

//    headeranddivider_style_ptr->icon_space = 4;

//    headeranddivider_style_ptr->is_need_button = FALSE;
    return (TRUE);
}

/*****************************************************************************/
// 	Description : get title default ctrl id
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetHeaderAndDividerCtrlId(void)
{
	return MMICOMMON_TITLE_CTRL_ID;
}

/*****************************************************************************/
// 	Description : get the window style
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetWinHeaderAndDividerMaxLen(void)
{
    return GUITITLE_TEXT_MAXLEN;
}

/*****************************************************************************/
// 	Description : get the title bar
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMITHEME_GetWinHeaderAndDividerBar(void)
{
#if defined (MMI_PDA_SUPPORT) && defined MMI_ISTYLE_SUPPORT
    if (MMITHEME_IsIstyle())
    {
        return IMAGE_ISTYLE_TITLE_BAR;
    }
    else
#endif
    {
        return IMAGE_TITLE_BAR;
    }
}

#ifdef GUIF_TITLE_PDASTYLE
/*****************************************************************************/
// 	Description : get the Theme title height
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetThemeHeaderAndDividerHeight_V(void)
{
#if defined (MMI_PDA_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {
        return MMI_TITLE_THEME_HEIGHT_ISTYLE_PDA;
    }
    else
#endif
    {
        return MMI_TITLE_THEME_HEIGHT_PDA;
    }
}

/*****************************************************************************/
// 	Description : get the Theme title height Horizontal
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetThemeHeaderAndDividerHeight_H(void)
{
#if defined (MMI_PDA_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {
        return MMI_TITLE_THEME_HEIGHT_H_ISTYLE_PDA;
    }
    else
#endif
    {
        return MMI_TITLE_THEME_HEIGHT_H_PDA;
    }
}
#endif

