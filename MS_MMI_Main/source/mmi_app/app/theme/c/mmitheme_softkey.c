/*****************************************************************************
** File Name:      mmitheme_softkey.c                                        *
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
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmitheme_softkey.h"
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
#include "mmiset_id.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmitheme_special_func.h"
#include "block_mem.h"
//#include "mmiset_display.h"
//#include "guires.h"
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
#include "mmitheme_button.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

//#ifdef GUIF_SOFTKEY_PDASTYLE
//#ifdef MAINLCD_SIZE_320X480
//#define MMI_PDA_SOFTKEY_BAR_HEIGHT          54  //ȫ����SOFTKEY���߶�
//#elif defined MAINLCD_SIZE_240X400
//#define MMI_PDA_SOFTKEY_BAR_HEIGHT          40//54  //ȫ����SOFTKEY���߶�
//#else
//#error
//#endif
//#endif

//button softkey data

#define BUTTONSOFTKEY_STYLE_NUM     3   //(GUISOFTKEY_STYLE_3BUTTON-GUISOFTKEY_STYLE_1BUTTON+1)
#define MMI_BACKGROUND_ALPHA        255//100%��͸��
typedef struct
{
    uint32 col_num;
    uint16 h_side_margin;
    uint16 v_side_margin;
    uint16 h_item_space;
    uint16 v_item_space;
}MMI_BUTTONSOFTKEY_MARGIN_T;


LOCAL const MMI_BUTTONSOFTKEY_MARGIN_T s_buttonsoftkey_margin[BUTTONSOFTKEY_STYLE_NUM]=
{
    {
        1,
        MMITHEME_SOFTKEY_1BUTTON_H_SIDE_MARGIN,
        MMITHEME_SOFTKEY_1BUTTON_V_SIDE_MARGIN,
        MMITHEME_SOFTKEY_1BUTTON_H_ITEM_SPACE,
        MMITHEME_SOFTKEY_1BUTTON_V_ITEM_SPACE,
    },
    {
        2,
        MMITHEME_SOFTKEY_2BUTTON_H_SIDE_MARGIN,
        MMITHEME_SOFTKEY_2BUTTON_V_SIDE_MARGIN,
        MMITHEME_SOFTKEY_2BUTTON_H_ITEM_SPACE,
        MMITHEME_SOFTKEY_2BUTTON_V_ITEM_SPACE,
    },
    {
        3,
        MMITHEME_SOFTKEY_3BUTTON_H_SIDE_MARGIN,
        MMITHEME_SOFTKEY_3BUTTON_V_SIDE_MARGIN,
        MMITHEME_SOFTKEY_3BUTTON_H_ITEM_SPACE,
        MMITHEME_SOFTKEY_3BUTTON_V_ITEM_SPACE,
    }
};

LOCAL const MMI_BUTTONSOFTKEY_MARGIN_T s_buttonsoftkey_h_margin[BUTTONSOFTKEY_STYLE_NUM]=
{
    {
        1,
        MMITHEME_H_SOFTKEY_1BUTTON_H_SIDE_MARGIN,
        MMITHEME_H_SOFTKEY_1BUTTON_V_SIDE_MARGIN,
        MMITHEME_H_SOFTKEY_1BUTTON_H_ITEM_SPACE,
        MMITHEME_H_SOFTKEY_1BUTTON_V_ITEM_SPACE,
    },
    {
        2,
        MMITHEME_H_SOFTKEY_2BUTTON_H_SIDE_MARGIN,
        MMITHEME_H_SOFTKEY_2BUTTON_V_SIDE_MARGIN,
        MMITHEME_H_SOFTKEY_2BUTTON_H_ITEM_SPACE,
        MMITHEME_H_SOFTKEY_2BUTTON_V_ITEM_SPACE,
    },
    {
        3,
        MMITHEME_H_SOFTKEY_3BUTTON_H_SIDE_MARGIN,
        MMITHEME_H_SOFTKEY_3BUTTON_V_SIDE_MARGIN,
        MMITHEME_H_SOFTKEY_3BUTTON_H_ITEM_SPACE,
        MMITHEME_H_SOFTKEY_3BUTTON_V_ITEM_SPACE,
    }
};

/*****************************************************************************/
// Description : get button style index
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL uint32 GetButtonStyleIndex(
                                 GUISOFTKEY_BUTTONSTYLE_E style
                                 );

/*****************************************************************************/
// 	Description : get the softkey style, is called by the softkey control
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetSoftkeyStyle(
                                        MMI_SOFTKEY_STYLE_T* softkey_style_ptr
	                                    )
{
    //SCI_PASSERT(PNULL != softkey_style_ptr, ("MMITHEME_GetSoftkeyStyle: the param is PNULL!")); /*assert verified*/
    if (PNULL == softkey_style_ptr)
    {
        return FALSE;
    }
    SCI_MEMSET(softkey_style_ptr, 0x00, sizeof(MMI_SOFTKEY_STYLE_T));

    softkey_style_ptr->bg_type      = GUI_SOFTKEY_BG_COLOR_ONLY;
#ifdef MONOCHROME_LCD_SUPPORT 
    softkey_style_ptr->bg_image     = 0;  //����ͼƬ
    softkey_style_ptr->bg_color     = MMI_WHITE_COLOR;
#else 
//    softkey_style_ptr->bg_image     = IMAGE_SOFTKEY_BG;  //����ͼƬ
    softkey_style_ptr->bg_color     = MMI_WHITE_COLOR;//MMI_BLACK_COLOR;
#endif//syy modify 2010.11.30 -end 
    softkey_style_ptr->bg_alpha     = MMI_BACKGROUND_ALPHA;

    softkey_style_ptr->font_color   = MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT);     //������ɫ
    softkey_style_ptr->pressed_font_color   = MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_PRESSED);     //������ɫ
    softkey_style_ptr->font         = MMI_FULLSCREEN_SOFTKEY_TEXT_FONT;           //���壬����μ���ʱû���ϣ�softkeyʵʱȥȡ
    softkey_style_ptr->small_font         = MMI_FULLSCREEN_SOFTKEY_TEXT_SMALL_FONT;           //���壬����μ���ʱû���ϣ�softkeyʵʱȥȡ
    softkey_style_ptr->first_icon_offset   = MMI_SOFTKEYBAR_FIRST_ICON_POS ;          // ��һ��ͼ���ƫ����
    softkey_style_ptr->second_icon_offset  = MMI_SOFTKEYBAR_SECOND_ICON_POS ;         // �ڶ���ͼ���ƫ����
    softkey_style_ptr->third_icon_offset   = MMI_SOFTKEYBAR_THIRD_ICON_POS;           // ������ͼ���ƫ����

#ifdef MONOCHROME_LCD_SUPPORT 
#else
#ifndef GUIF_SOFTKEY_PDASTYLE
    softkey_style_ptr->skb_button[LEFT_BUTTON].icon_up_id = IMAGE_SOFTKEY_BG;
    softkey_style_ptr->skb_button[MIDDLE_BUTTON].icon_up_id = IMAGE_SOFTKEY_BG;
    softkey_style_ptr->skb_button[RIGHT_BUTTON].icon_up_id = IMAGE_SOFTKEY_BG;
    softkey_style_ptr->skb_button[LEFT_BUTTON].icon_down_id = IMAGE_SOFTKEY_BG_DOWN;
    softkey_style_ptr->skb_button[MIDDLE_BUTTON].icon_down_id = IMAGE_SOFTKEY_BG_DOWN;
    softkey_style_ptr->skb_button[RIGHT_BUTTON].icon_down_id = IMAGE_SOFTKEY_BG_DOWN;
#endif
#endif//syy modify 2010.11.30 -end

    softkey_style_ptr->skb_button[LEFT_BUTTON].rect   = MMITHEME_GetSoftkeyRect(LEFT_BUTTON);
    softkey_style_ptr->skb_button[MIDDLE_BUTTON].rect = MMITHEME_GetSoftkeyRect(MIDDLE_BUTTON);
    softkey_style_ptr->skb_button[RIGHT_BUTTON].rect  = MMITHEME_GetSoftkeyRect(RIGHT_BUTTON);

    softkey_style_ptr->skb_button[LEFT_BUTTON].is_show = TRUE;
    softkey_style_ptr->skb_button[MIDDLE_BUTTON].is_show = TRUE;
    softkey_style_ptr->skb_button[RIGHT_BUTTON].is_show = TRUE;
    
    return (TRUE);
}
/*****************************************************************************/
// 	Description : get the mid softkey is null
//	Global resource dependence : 
//  Author: nanji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsMidkeyNull(
									 MMI_TEXT_ID_T text_id
									 )
{
    BOOLEAN result = FALSE;
	if (TXT_NULL == text_id)
	{
		result = TRUE;
	}

	return result;
}
/*****************************************************************************/
// 	Description : get the default softkey image info
//	Global resource dependence : 
//  Author: jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetDefaultSoftkeyImg(
                                          GUISOFTKEY_IMG_T    *img_ptr    //in/out
                                          )
{
    //SCI_ASSERT(PNULL != img_ptr);/*assert verified*/
    if (PNULL == img_ptr)//xiyuan edit for pclint
    {
return;
}
#ifndef GUIF_SOFTKEY_PDASTYLE
        //set image
        img_ptr->bg_img         = IMAGE_SOFTKEY_BG;
        img_ptr->left_down_img  = IMAGE_SOFTKEY_BG_DOWN;
        img_ptr->left_up_img    = IMAGE_SOFTKEY_BG;
        img_ptr->mid_down_img   = IMAGE_SOFTKEY_BG_DOWN;
        img_ptr->mid_up_img     = IMAGE_SOFTKEY_BG;
        img_ptr->right_down_img = IMAGE_SOFTKEY_BG_DOWN;
        img_ptr->right_up_img   = IMAGE_SOFTKEY_BG;
#endif
}

/*****************************************************************************/
// 	Description : get the height of the softkey area, is called by the MMI Kernel
//	Global resource dependence : 
//  Author:Robert.Lu
//	Note:
/*****************************************************************************/
PUBLIC uint32  MMITHEME_GetSoftkeyHeight(void)
{
#ifdef GUIF_SOFTKEY_PDASTYLE
    return MMI_PDA_SOFTKEY_BAR_HEIGHT;
#else
    return MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
#endif
}
/*****************************************************************************/
//  Description :get  softkey v_space
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMITHEME_GetSoftkeyHorSpace(void)
{
    return MMI_SOFTKEY_SPACE_H;
}
/*****************************************************************************/
//  Description :get  softkey button width
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMITHEME_GetSoftkeyButtonWidth(void)
{
    return MMI_SOFTKEY_BUTTON_WIDTH;
}

/*****************************************************************************/
//  Description : get win softkey height
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMITHEME_GetWinSoftkeyHeight(
                                           MMI_HANDLE_T win_handle
                                           )
{
#ifdef GUIF_SOFTKEY_PDASTYLE
    if (MMK_GetWinDisplayStyleState(win_handle, WS_HAS_BUTTON_SOFTKEY))
    {
        return MMI_PDA_SOFTKEY_BAR_HEIGHT;
    }
    else
    {
        return 0;
    }
#else
    return MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
#endif
}

/*****************************************************************************/
// 	Description : copy rect
//	Global resource dependence : 
//  Author:lin.lin
//	Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetSoftKeyTextRect(
                                              GUI_ALIGN_E    align_num,
                                              GUI_RECT_T     rect
                                              )
{
    GUI_RECT_T button_rect;

    switch(align_num)
    {
    case ALIGN_LVMIDDLE:
        button_rect = GUI_CreateRect(
            (uint16)(rect.left + MMI_SOFTKEY_TEXT_HOR_SPACE),
            (uint16)(rect.top),
            (uint16)(rect.right),
            rect.bottom
            );
        break;

    case ALIGN_HVMIDDLE:
        button_rect = GUI_CreateRect(
            rect.left,
            rect.top ,
            rect.right,
            rect.bottom
            );
        break;

    case ALIGN_RVMIDDLE:
        button_rect = GUI_CreateRect(
            (uint16)(rect.left),
            (uint16)(rect.top),
            (uint16)(rect.right - MMI_SOFTKEY_TEXT_HOR_SPACE),
            rect.bottom
            );
        break;

    default:
        button_rect = GUI_CreateRect(
            rect.left,
            rect.top,
            rect.right,
            rect.bottom
            );
        break;
    }

    return button_rect;
}

/*****************************************************************************/
// 	Description : get softkey text font
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC GUI_FONT_T MMITHEME_GetSoftKeyTextFont(void)
{
    MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;

    MMIAPISET_GetLanguageType(&lang_type);

    //���������������ֲ�ͬ���壬����������ô����壬������С����
    if (MMISET_LANGUAGE_SIMP_CHINESE == lang_type
        || MMISET_LANGUAGE_TRAD_CHINESE == lang_type)
    {
        return MMI_FULLSCREEN_SOFTKEY_TEXT_FONT;
    }
    else
    {
        return MMI_FULLSCREEN_SOFTKEY_TEXT_SMALL_FONT;
    }
}

/*****************************************************************************/
// 	Description : get the softkey bar rect
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetSoftkeyCtrlId(void)
{
	return MMICOMMON_SOFTKEY_CTRL_ID;
}

/*****************************************************************************/
// 	Description : get the softkey button ctrl id
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetSoftkeyButtonCtrlId(
                                            MMI_CTRL_ID_T *button_ctrl_id_arr     //out
                                            )
{
    if (PNULL != button_ctrl_id_arr)
    {
        button_ctrl_id_arr[0] = MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID;
        button_ctrl_id_arr[1] = MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID;
        button_ctrl_id_arr[2] = MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID;
    }
}

/*****************************************************************************/
// Description : get button style index
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL uint32 GetButtonStyleIndex(
                                 GUISOFTKEY_BUTTONSTYLE_E style
                                 )
{
    uint32 style_index = 0;

    switch (style)
    {
    case GUISOFTKEY_BUTTONSTYLE_2BUTTON:
        style_index = 1;
        break;
    case GUISOFTKEY_BUTTONSTYLE_3BUTTON:
        style_index = 2;
        break;
    case GUISOFTKEY_BUTTONSTYLE_1BUTTON:
    default:
        style_index = 0;
        break;
    }
    return style_index;
}

/*****************************************************************************/
// Description : calc softkey button rect
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_CalcSoftkeyButtonRect(
                             const GUI_RECT_T *rect_ptr,    //in
                             GUISOFTKEY_BUTTONSTYLE_E style,    //in
                             GUI_RECT_T *button_rect_ptr   //out
                             )
{
    MMI_BUTTONSOFTKEY_MARGIN_T margin = {0};
    GUI_RECT_T rect_arr[GUISOFTKEY_BUTTON_NUM] = {0};
    uint32 col_num = 0;
    uint32 i = 0;
    uint32 style_index = 0;

    if ((PNULL == rect_ptr) || (PNULL == button_rect_ptr))
    {
        return FALSE;
    }
    if ((style<GUISOFTKEY_BUTTONSTYLE_1BUTTON) || (style>GUISOFTKEY_BUTTONSTYLE_3BUTTON))
    {
        return FALSE;
    }
    style_index = GetButtonStyleIndex(style);
    if (MMITHEME_IsMainScreenLandscape())
    {
        margin = s_buttonsoftkey_h_margin[style_index];
    }
    else
    {
        margin = s_buttonsoftkey_margin[style_index];
    }
    col_num = margin.col_num;
    //middle softkey
    GUI_CreateMatrixRect(
        (GUI_RECT_T*)rect_ptr, 
        margin.h_side_margin, 
        margin.v_side_margin, 
        margin.h_item_space, 
        margin.v_item_space,
        1, 
        col_num, 
        rect_arr
        ); 
    for (i = 0; i < GUISOFTKEY_BUTTON_NUM; i++)
    {
        button_rect_ptr[i] = rect_arr[i];
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : get the button softkey style, is called by the softkey control
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetButtonSoftkeyStyle(
                                              MMI_BUTTONSOFTKEY_STYLE_T* softkey_style_ptr
                                              )
{
#ifdef GUIF_SOFTKEY_PDASTYLE
    if (PNULL == softkey_style_ptr)
    {
        return FALSE;
    }
    softkey_style_ptr->bg_image = IMAGE_PDASOFTKEY_BG;
    softkey_style_ptr->font.font = MMI_DEFAULT_BUTTON_FONT;
    softkey_style_ptr->font.color = MMI_BLACK_COLOR;

    softkey_style_ptr->img_id = IMAGE_PUBWIN_BUTTON_PDA;
    softkey_style_ptr->pressed_img_id = IMAGE_PUBWIN_BUTTON_HL_PDA;
    softkey_style_ptr->grayed_img_id = IMAGE_PUBWIN_BUTTON_GRAY_PDA;
#endif

    return (TRUE);
}


