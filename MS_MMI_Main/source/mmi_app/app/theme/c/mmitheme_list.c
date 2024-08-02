/*****************************************************************************
** File Name:      mmitheme_list.c                                           *
** Author:         Great.Tian                                                *
** Date:           03/9/2011                                                 *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe the mmi theme               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2011        Xiaoqing.lu      Create                                    *
**                                                                           *
*****************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_theme_trc.h"
#include "std_header.h"
#include "mmitheme_list.h"
#include "mmi_theme.h"
#include "mmi_common.h"
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
#include "mmisms_image.h"
#include "mmicc_image.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
#define LIST_NUMBER_MAX_DIGIT    4     // 目前在ListBox的Index的显示当中，最大支持4位的
#define LIST_HIGHLIGHT_PERCENT_LUCENCY 128 // LIST高亮条的透明度
#define ASCII_CODE_0             0x30  // ascii code 0
#define ASCII_CODE_ZHE           0x2F  // ascii code /
#define LISTBOX_STEP_NUM_HOR     5     // 定义horizontal方向左右滑动时的次数,用于左右滑动时
#define LISTBOX_STEP_NUM_VERT    3     // 定义vertical方向上下滑动的次数, 用于item和text上下滑动

#define LISTBOX_KEY_TIME                        300   // 连续移动的按键Timer值

#define MMITHEME_LIST_STYLE_TEXT_TIME      500 //ms

#define MMITHEME_LIST_STYLE_MOVE_TIME       50//ms

// 所有的按像素滚动的像素数
#define MMI_SMALL_LIST_ITEM_HEIGHT    22

#define MMI_LIST_ITEM_SPLIT_BG_COLOR  MMI_DARK_GRAY_COLOR/*0x841*/

#define MMI_LIST_TIP_HIDE_TICK 2000
#define MMI_LIST_TIP_FONT_COLOR 0xef7d

#define MAKE_BREW_RGB(r,g,b) ((r >> 3)<<11 | (g>>2)<<5 | (b>>3))

//for bug1280069
#if defined (MAINLCD_SIZE_128X160)
#define MMI_LIST_BIG_FONT_WIDTH 8
#else
#define MMI_LIST_BIG_FONT_WIDTH 10
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#if 0
/*****************************************************************************/
//  Description : convert listnum to string, return convert digit
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL uint8 ConvertListNumToStr(
    uint8 *str_ptr, //[OUT] string buffer pointer
    uint16 number   //[IN]  list number
);
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



///////////////////////BEGIN///////////////////////

//|---------------------------|
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_split =
{
    MMI_LIST_SPLIT_ITEM_HEIGHT, MMI_LIST_SPLIT_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS,   MMI_LIST_ITEM_TOP_MARGIN_MS + 2,  MMI_LIST_ITEM_RIGHT_MS,  MMI_LIST_SPLIT_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN_MS },
            { MMI_LIST_TEXT_LEFT_SPACE_MS,   MMI_LIST_ITEM_TOP_MARGIN_MS + 2,  MMI_LIST_ITEM_RIGHT_MS,  MMI_LIST_SPLIT_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN_MS },
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|        text          @    |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text_icon =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_RIGHT_ICON_LEFT_SPACE), MMI_LIST_ITEM_HEIGHT - 1 },
            { MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_RIGHT_ICON_LEFT_SPACE), MMI_LIST_ITEM_HEIGHT - 1 },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_SELECT_ICON_WIDTH), 1,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_ITEM_HEIGHT - 2},
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_SELECT_ICON_WIDTH), 1,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_ITEM_HEIGHT - 2},
            0, 0,
            100, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};


#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|        text          @    |
//|---------------------------|
//GUIITEM_STYLE_HALF_LINE_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text_icon_2 =
{
    MMI_LIST_ITEM_HEIGHT/2 , MMI_LIST_ITEM_HEIGHT/2,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE), (MMI_LIST_ITEM_HEIGHT - 1) / 2},
            { MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE), (MMI_LIST_ITEM_HEIGHT - 1) / 2},
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1,  MMI_LIST_ITEM_RIGHT, (MMI_LIST_ITEM_HEIGHT - 2) / 2 },
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1,  MMI_LIST_ITEM_RIGHT, (MMI_LIST_ITEM_HEIGHT - 2) / 2 },
            0, 0,
            100, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

#endif

//|---------------------------|
//|     @   text          @   |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text_icon =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2 },
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2 },
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + 2*MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_ITEM_HEIGHT - 1 },
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + 2*MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_EXPAND_ITEM_HEIGHT - 1 },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE), 1,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_ITEM_HEIGHT - 1 },
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE), 1,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_ITEM_HEIGHT - 1 },
            0, 0,
            100, 100
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|-----------------------------------------------------------------------------|
//|                            @   text          @                              |
//|这种风格与GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON唯一不同就在于图片1不可以check|
//|-----------------------------------------------------------------------------|
// GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON_EXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text_icon_ext =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2 },
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2 },
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_ITEM_HEIGHT - 1 },
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_EXPAND_ITEM_HEIGHT - 1 },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_ITEM_HEIGHT - 2 },
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_ITEM_HEIGHT - 2 },
            0, 0,
            100, 100
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
#endif

/* 以下都为单行item, check为多选, radio为单选, number会在item前增加数字 */

//|---------------------------|
//|        text               |
//|---------------------------|
// GUIITEM_STYLE_ONE_LINE_TEXT,
#if defined MAINLCD_SIZE_128X160 || defined MAINLCD_SIZE_176X220
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#else
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT -1 },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#endif

#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|        text               |
//|---------------------------|//NEWMS00185675 非GUIF_LIST_PDASTYLE风格下高亮，字体大小不变
// GUIITEM_STYLE_ONE_LINE_TEXT_DEFAULT_FONT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text_2 =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#endif


//|---------------------------|
//|  @     text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ICON_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2}, // normal
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE ,  MMI_LIST_ITEM_HEIGHT - 2}, // focus
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE * 2, MMI_LIST_ITEM_HEIGHT - 1 },
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE * 2, MMI_LIST_EXPAND_ITEM_HEIGHT - 1 },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 85
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|  @     text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ICON_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text_without_effect_str =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2}, // normal
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE ,  MMI_LIST_ITEM_HEIGHT - 2}, // focus
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT - 1 },
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT - 1 },
            MMI_LIST_EXPAND_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|  @  @  text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_TWO_ICON_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_icon_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_DEFAULT_FONT_ASCII_WIDTH * 4 + MMI_LIST_ICON_LEFT_SPACE * 2 ,  MMI_LIST_ITEM_HEIGHT - 1 }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_EXPAND_ITEM_HEIGHT - 2 }, /* 2 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_DEFAULT_FONT_ASCII_WIDTH * 4 + MMI_LIST_ICON_LEFT_SPACE * 2 ,  0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT }, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 + MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT - 1},   /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|  ⑴   text                |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_NUMBER_TEXT,
//s_item_style_1_text

#endif

//|---------------------------|
//|  @     text       text    |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_MAINSCREEN_WIDTH / 3 * 2 /*- MMI_LIST_ICON_LEFT_SPACE*/, MMI_LIST_ITEM_HEIGHT },   /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_MAINSCREEN_WIDTH / 3 * 2 /*- MMI_LIST_ICON_LEFT_SPACE*/, MMI_LIST_EXPAND_ITEM_HEIGHT  - 1},   /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 35
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_MAINSCREEN_WIDTH / 3 * 2 + MMI_LIST_ICON_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_ITEM_HEIGHT },   /* 3 */
            { MMI_MAINSCREEN_WIDTH / 3 * 2 + MMI_LIST_ICON_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_ITEM_HEIGHT - 1},   /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            35, 70/*65*/
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|   @   text           text |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT,  // item pos is not differnt with above one
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text_text_2 =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 0,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 0,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            //for bug1280069，因为联系人最大支持2000，右侧的坐标需要减去6倍的字宽。
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_MAINSCREEN_WIDTH - MMI_LIST_BIG_FONT_WIDTH * 6-4 - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_ITEM_HEIGHT - 1}, /* 2 */ // for bug1249522
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_MAINSCREEN_WIDTH - MMI_LIST_BIG_FONT_WIDTH * 6-4 - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_EXPAND_ITEM_HEIGHT - 1}, /* 2 */ // for bug1249522
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_MAINSCREEN_WIDTH - MMI_LIST_BIG_FONT_WIDTH * 6 -4, 0,  MMI_LIST_ITEM_RIGHT - 1, MMI_LIST_ITEM_HEIGHT - 1 },   /* 3 */ // for bug1249522
            { MMI_MAINSCREEN_WIDTH - MMI_LIST_BIG_FONT_WIDTH * 6 -4, 0,  MMI_LIST_ITEM_RIGHT - 1, MMI_LIST_ITEM_HEIGHT - 1 },   /* 3 */ // for bug1249522
            SONG_FONT_20, SONG_FONT_20, //MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            100, 100
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|  @  @  text       text    |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_TWO_ICON_TWO_TEXT,
#ifdef GUIF_LIST_PDASTYLE
#ifdef MAINLCD_SIZE_240X320
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_icon_text_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE - 16,  MMI_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE - 16,  MMI_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2  - 16,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 - 16,  MMI_LIST_ITEM_HEIGHT  - 2}, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2  - 16,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 - 16,  MMI_LIST_EXPAND_ITEM_HEIGHT  - 2}, /* 2 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 - 16,  0,  MMI_MAINSCREEN_WIDTH * 2 / 3, MMI_LIST_ITEM_HEIGHT -1}, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 - 16,  0,  MMI_MAINSCREEN_WIDTH * 2 / 3, MMI_LIST_ITEM_HEIGHT -1}, /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH * 2 / 3 + 2, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT -1 },   /* 4 */
            { MMI_MAINSCREEN_WIDTH * 2 / 3 + 2, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT -1},   /* 4 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            100, 100
        },
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};
#else
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_icon_text_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE - 20,  MMI_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE - 20,  MMI_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2  - 20,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 - 20,  MMI_LIST_ITEM_HEIGHT  - 2}, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2  - 20,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 - 20,  MMI_LIST_EXPAND_ITEM_HEIGHT  - 2}, /* 2 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 - 20,  0,  MMI_MAINSCREEN_WIDTH * 2 / 3, MMI_LIST_ITEM_HEIGHT -1 }, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 - 20,  0,  MMI_MAINSCREEN_WIDTH * 2 / 3, MMI_LIST_ITEM_HEIGHT -1 }, /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH * 2 / 3 + 2, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT -1},   /* 4 */
            { MMI_MAINSCREEN_WIDTH * 2 / 3 + 2, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT -1 },   /* 4 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            100, 100
        },
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};
#endif
#else
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_icon_text_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_ITEM_HEIGHT }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_EXPAND_ITEM_HEIGHT - 2}, /* 2 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_MAINSCREEN_WIDTH * 2 / 3, MMI_LIST_ITEM_HEIGHT }, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_MAINSCREEN_WIDTH * 2 / 3, MMI_LIST_ITEM_HEIGHT }, /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH * 2 / 3 + 2, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT },   /* 4 */
            { MMI_MAINSCREEN_WIDTH * 2 / 3 + 2, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT },   /* 4 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            100, 100
        },
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};
#endif

//|---------------------------|
//|           | text |        |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_BUTTON_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_buttontext =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|   | text |      |text|    |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_TWO_BUTTON_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_2buttontext =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_TEXT_LEFT_SPACE, 0, (MMI_MAINSCREEN_WIDTH) / 2 - MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_BUTTON_HEIGHT },      /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE, 0, (MMI_MAINSCREEN_WIDTH) / 2 - MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_BUTTON_HEIGHT },      /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            {(MMI_MAINSCREEN_WIDTH / 2 + MMI_LIST_TEXT_LEFT_SPACE) ,  0,  MMI_MAINSCREEN_WIDTH - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_BUTTON_HEIGHT },     /* 2 */
            {(MMI_MAINSCREEN_WIDTH / 2 + MMI_LIST_TEXT_LEFT_SPACE) ,  0,  MMI_MAINSCREEN_WIDTH - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_BUTTON_HEIGHT },     /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            50, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#endif

//zmt add start
//GUIITEM_SYTLE_ZMT_PUBLISH_LIST_MS
LOCAL THEMELIST_ITEM_STYLE_T const zmt_publish_list_item =
{
    2*ZMT_LIST_LINE_HIGHT, 2*ZMT_LIST_LINE_HIGHT,
    {
        {
            0,
            { ZMT_LIST_ITEM_PANDDING, 0, ZMT_LIST_LINE_WIDTH+ZMT_LIST_ITEM_PANDDING, 2*ZMT_LIST_LINE_HIGHT},
            { ZMT_LIST_ITEM_PANDDING, 0, ZMT_LIST_LINE_WIDTH+ZMT_LIST_ITEM_PANDDING, 2*ZMT_LIST_LINE_HIGHT},
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            {ZMT_LIST_ITEM_PANDDING+4,  0,  ZMT_LIST_LINE_WIDTH+ZMT_LIST_ITEM_PANDDING, 2*ZMT_LIST_LINE_HIGHT},
            {ZMT_LIST_ITEM_PANDDING+4,  0,  ZMT_LIST_LINE_WIDTH+ZMT_LIST_ITEM_PANDDING, 2*ZMT_LIST_LINE_HIGHT},
            SONG_FONT_20, SONG_FONT_20,
            0, 0
        },
        {
            0,
            {ZMT_LIST_LINE_WIDTH+2*ZMT_LIST_ITEM_PANDDING, 0,  MMI_MAINSCREEN_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            {ZMT_LIST_LINE_WIDTH+2*ZMT_LIST_ITEM_PANDDING, 0,  MMI_MAINSCREEN_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            SONG_FONT_22, SONG_FONT_22,
            0, 0
        },
        {0},
        {0},
        {0},
    },
    0, 1
};
//GUIITEM_SYTLE_ZMT_BOOK_LIST_MS,
LOCAL THEMELIST_ITEM_STYLE_T const zmt_book_list_item =
{
    2.5*ZMT_LIST_LINE_HIGHT, 2.5*ZMT_LIST_LINE_HIGHT,
    {
        {
            0,
            { ZMT_LIST_ITEM_PANDDING, 0.6*ZMT_LIST_LINE_HIGHT, ZMT_LIST_LINE_WIDTH+ZMT_LIST_ITEM_PANDDING, 2*ZMT_LIST_LINE_HIGHT},
            { ZMT_LIST_ITEM_PANDDING, 0.6*ZMT_LIST_LINE_HIGHT, ZMT_LIST_LINE_WIDTH+ZMT_LIST_ITEM_PANDDING, 2*ZMT_LIST_LINE_HIGHT},
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            {ZMT_LIST_ITEM_PANDDING+4,  0.6*ZMT_LIST_LINE_HIGHT,  ZMT_LIST_LINE_WIDTH+ZMT_LIST_ITEM_PANDDING, 2*ZMT_LIST_LINE_HIGHT},
            {ZMT_LIST_ITEM_PANDDING+4,  0.6*ZMT_LIST_LINE_HIGHT,  ZMT_LIST_LINE_WIDTH+ZMT_LIST_ITEM_PANDDING, 2*ZMT_LIST_LINE_HIGHT},
            SONG_FONT_18, SONG_FONT_18,
            0, 0
        },
        {
            0,
            {ZMT_LIST_LINE_WIDTH+2*ZMT_LIST_ITEM_PANDDING, 10,  MMI_MAINSCREEN_WIDTH, ZMT_LIST_LINE_HIGHT+10},
            {ZMT_LIST_LINE_WIDTH+2*ZMT_LIST_ITEM_PANDDING, 10,  MMI_MAINSCREEN_WIDTH, ZMT_LIST_LINE_HIGHT+10},
            SONG_FONT_22, SONG_FONT_22,
            0, 0
        },
        {
            0,
            {ZMT_LIST_LINE_WIDTH+2*ZMT_LIST_ITEM_PANDDING, ZMT_LIST_LINE_HIGHT+11, MMI_MAINSCREEN_WIDTH, 2.3*ZMT_LIST_LINE_HIGHT},
            {ZMT_LIST_LINE_WIDTH+2*ZMT_LIST_ITEM_PANDDING, ZMT_LIST_LINE_HIGHT+11, MMI_MAINSCREEN_WIDTH, 2.3*ZMT_LIST_LINE_HIGHT},
            SONG_FONT_22, SONG_FONT_22,
            0, 0
        },     
        {0},
        {0},
    },
    0, 1
};
//GUIITEM_SYTLE_ZMT_UNIT_LIST_MS,
LOCAL THEMELIST_ITEM_STYLE_T const zmt_unit_list_item =
{
    1.5*ZMT_LIST_LINE_HIGHT, 1.5*ZMT_LIST_LINE_HIGHT,
    {
        {
            0,
            { 5, 0,  MMI_MAINSCREEN_WIDTH-ZMT_LIST_LINE_WIDTH, 1.5*ZMT_LIST_LINE_HIGHT },       /* 2 */
            { 5, 0,  MMI_MAINSCREEN_WIDTH-ZMT_LIST_LINE_WIDTH, 1.5*ZMT_LIST_LINE_HIGHT },       /* 2 */
            SONG_FONT_20, SONG_FONT_20,
            0, 100
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH-ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH, 1.5*ZMT_LIST_LINE_HIGHT},     /* 2 */
            { MMI_MAINSCREEN_WIDTH-ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH, 1.5*ZMT_LIST_LINE_HIGHT},     /* 2 */
            0, 0,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },
};
//GUIITEM_SYTLE_ZMT_LISTEN_SET_MS,
LOCAL THEMELIST_ITEM_STYLE_T const zmt_listen_set_list_item =
{
    2.5*ZMT_LIST_LINE_HIGHT, 2.5*ZMT_LIST_LINE_HIGHT,
    {
        {
            0,
            { 5, 0,  MMI_MAINSCREEN_WIDTH, 1.5*ZMT_LIST_LINE_HIGHT },       /* 2 */
            { 5, 0,  MMI_MAINSCREEN_WIDTH, 1.5*ZMT_LIST_LINE_HIGHT },       /* 2 */
            SONG_FONT_24, SONG_FONT_24,
            0, 100
        },
        {
            0,
            { 5, 1.5*ZMT_LIST_LINE_HIGHT, MMI_MAINSCREEN_WIDTH, 2.5*ZMT_LIST_LINE_HIGHT},     /* 2 */
            { 5, 1.5*ZMT_LIST_LINE_HIGHT, MMI_MAINSCREEN_WIDTH, 2.5*ZMT_LIST_LINE_HIGHT},     /* 2 */
            SONG_FONT_20, SONG_FONT_20,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },
};
//GUIITEM_STYLE_POETRY_GRADE_LIST_MS
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_poetry_grade_list_ms =
{
    2*ZMT_LIST_LINE_HIGHT, 2*ZMT_LIST_LINE_HIGHT,
    {
        //icon
        {
            0,
            {0.4*ZMT_LIST_LINE_WIDTH, 0, 1.4*ZMT_LIST_LINE_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            {0.4*ZMT_LIST_LINE_WIDTH, 0, 1.4*ZMT_LIST_LINE_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            0, 0
        }, 
        //text
        {
            0,
            {1.4*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            {1.4*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            SONG_FONT_24, SONG_FONT_24,
            0, 0
        },    
        {0},
        {0},
        {0},
        {0},
    },
    0, MMILIST_INVALID_ITEM_INDEX
};
//GUIITEM_STYLE_POETRY_ITEM_LIST_MS
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_poetry_item_list_ms =
{
    2*ZMT_LIST_LINE_HIGHT, 2*ZMT_LIST_LINE_HIGHT,
    {
        //bg
        {
            0,
            {0, 0, MMI_MAINSCREEN_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            {0, 0, MMI_MAINSCREEN_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            0, 0,
            0, 0
        }, 
        //text
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            {0.2*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH - 0.2*ZMT_LIST_LINE_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            {0.2*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH - 0.2*ZMT_LIST_LINE_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            SONG_FONT_24, SONG_FONT_24,
            0, 0
        },   
        {0},
        {0},
        {0},
        {0},
    },
    0, MMILIST_INVALID_ITEM_INDEX
};
//GUIITEM_STYLE_FORMULA_ITEM_LIST_MS
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_formula_item_list_ms =
{
    ZMT_LIST_ITEM_HIGHT, ZMT_LIST_ITEM_HIGHT,
    {
        //bg
        {
            0,
            {0, 0, MMI_MAINSCREEN_WIDTH/2, ZMT_LIST_ITEM_HIGHT},
            {0, 0, MMI_MAINSCREEN_WIDTH/2, ZMT_LIST_ITEM_HIGHT},
            0, 0,
            0, 0
        },
        //text
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            {0, 0, MMI_MAINSCREEN_WIDTH/2, ZMT_LIST_ITEM_HIGHT},
            {0, 0, MMI_MAINSCREEN_WIDTH/2, ZMT_LIST_ITEM_HIGHT},
            SONG_FONT_20, SONG_FONT_20,
            0, 0
        }, 
        {0},
        {0},
        {0},
        {0},
    },
    0, MMILIST_INVALID_ITEM_INDEX
};
//GUIITEM_STYLE_CLASS_MAIN_LIST_MS
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_class_main_list_ms =
{
    2.5*ZMT_LIST_LINE_HIGHT, 2.5*ZMT_LIST_LINE_HIGHT,
    {
        //bg
        {
            0,
            {0, 0, MMI_MAINSCREEN_WIDTH, 2.5*ZMT_LIST_LINE_HIGHT},
            {0, 0, MMI_MAINSCREEN_WIDTH, 2.5*ZMT_LIST_LINE_HIGHT},
            0, 0,
            0, 0
        }, 
        //icon
        {
            0,
            {0.3*ZMT_LIST_LINE_WIDTH, 0, 2*ZMT_LIST_LINE_WIDTH, 2.5*ZMT_LIST_LINE_HIGHT},
            {0.3*ZMT_LIST_LINE_WIDTH, 0, 2*ZMT_LIST_LINE_WIDTH, 2.5*ZMT_LIST_LINE_HIGHT},
            0, 0,
            0, 0
        }, 
        //text
        {
            GUIITEM_CONTENT_STATE_MULTILINE,
            {2*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH-10, 2.5*ZMT_LIST_LINE_HIGHT},
            {2*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH-10, 2.5*ZMT_LIST_LINE_HIGHT},
            SONG_FONT_24, SONG_FONT_24,
            0, 0
        },   
        {0},
        {0},
        {0},
    },
    //这里的2,2是为了让索引为2的content text显示多行，且要配合GUIITEM_CONTENT_STATE_MULTILINE使用
    2, 2
};
//GUIITEM_STYLE_CLASS_SECTION_LIST_MS
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_class_section_list_ms =
{
    1.5*ZMT_LIST_LINE_HIGHT, 1.5*ZMT_LIST_LINE_HIGHT,
    {
        //text
        {
            GUIITEM_CONTENT_STATE_MULTILINE,
            {0.3*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH-1.2*ZMT_LIST_LINE_WIDTH, 1.5*ZMT_LIST_LINE_HIGHT},
            {0.3*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH-1.2*ZMT_LIST_LINE_WIDTH, 1.5*ZMT_LIST_LINE_HIGHT},
            SONG_FONT_20, SONG_FONT_20,
            0, 0
        },
        //icon
        {
            0,
            {MMI_MAINSCREEN_WIDTH-1.2*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH-5, 1.5*ZMT_LIST_LINE_HIGHT},
            {MMI_MAINSCREEN_WIDTH-1.2*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH-5, 1.5*ZMT_LIST_LINE_HIGHT},
            0, 0,
            0, 0
        }, 
        {0},
        {0},
        {0},
        {0},
    },
    //这里的0,2是为了让索引为0的content text显示多行，且要配合GUIITEM_CONTENT_STATE_MULTILINE使用
    0, 2
};
//GUIITEM_STYLE_PINYIN_TABLE_LIST_MS
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_pinyin_table_list_ms =
{
    1.5*ZMT_LIST_LINE_HIGHT, 1.5*ZMT_LIST_LINE_HIGHT,
    {
        //icon
        {
            0,
            {0, 0, MMI_MAINSCREEN_WIDTH/2, 1.5*ZMT_LIST_LINE_HIGHT},
            {0, 0, MMI_MAINSCREEN_WIDTH/2, 1.5*ZMT_LIST_LINE_HIGHT},
            0, 0,
            0, 0
        },
        //text
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            {0, 0, MMI_MAINSCREEN_WIDTH/2, 1.5*ZMT_LIST_LINE_HIGHT},
            {0, 0, MMI_MAINSCREEN_WIDTH/2, 1.5*ZMT_LIST_LINE_HIGHT},
            SONG_FONT_24, SONG_FONT_24,
            0, 0
        }, 
        {0},
        {0},
        {0},
        {0},
    },
    0, MMILIST_INVALID_ITEM_INDEX
};
//zmt add end

//|---------------------------|
//| text                text  |
//|---------------------------|
// GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT,
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_small_text_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_MAINSCREEN_WIDTH / 2,  MMI_LIST_ITEM_HEIGHT },     /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_MAINSCREEN_WIDTH / 2,  MMI_LIST_ITEM_HEIGHT },     /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            {MMI_MAINSCREEN_WIDTH / 2 + 3 * MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_MAINSCREEN_WIDTH, MMI_LIST_ITEM_HEIGHT },   /* 2 */
            {MMI_MAINSCREEN_WIDTH / 2 + 3 * MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_MAINSCREEN_WIDTH, MMI_LIST_ITEM_HEIGHT },   /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            50, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#else
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_small_text_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT / 2 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT },     /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT / 2 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT },     /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            GUIITEM_CONTENT_STATE_CHECK | GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            {MMI_LIST_ITEM_RIGHT / 2 + MMI_LIST_TEXT_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_ITEM_HEIGHT },     /* 2 */
            {MMI_LIST_ITEM_RIGHT / 2 + MMI_LIST_TEXT_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_ITEM_HEIGHT },     /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            50, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#endif
#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//| text             text  @  |
//|---------------------------|
// GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_small_text_text_icon =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,
    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_MAINSCREEN_WIDTH/3 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},  /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_MAINSCREEN_WIDTH/3 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},  /* 1 */
            0, 0,
            0, 50
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH / 2 + MMI_LIST_TEXT_LEFT_SPACE,  0, MMI_MAINSCREEN_WIDTH-MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},       /* 2 */
            { MMI_MAINSCREEN_WIDTH / 2 + MMI_LIST_TEXT_LEFT_SPACE,  0, MMI_MAINSCREEN_WIDTH-MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},       /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE,  1,  MMI_MAINSCREEN_WIDTH, MMI_LIST_ITEM_HEIGHT - 2 },       /* 3 */
            { MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE,  1,  MMI_MAINSCREEN_WIDTH, MMI_LIST_ITEM_HEIGHT - 2 },       /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            100, 100
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//| text   text      @     @  |
//|---------------------------|
// GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_TEXT_TWO_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_small_2text_2icon =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,
    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_MAINSCREEN_WIDTH / 3 - MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT },     /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_MAINSCREEN_WIDTH / 3 - MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT },     /* 1 */
            0, 0,
            0, 50
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH / 3 - MMI_LIST_TEXT_LEFT_SPACE,  0, MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 - MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_ITEM_HEIGHT }, /* 2 */
            { MMI_MAINSCREEN_WIDTH / 3 - MMI_LIST_TEXT_LEFT_SPACE,  0, MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 - MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_ITEM_HEIGHT }, /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 - MMI_LIST_ICON_LEFT_SPACE * 2,  1,  MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_HEIGHT - 2}, /* 3 */
            { MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 - MMI_LIST_ICON_LEFT_SPACE * 2,  1,  MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_HEIGHT - 2}, /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            100, 100
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_RIGHT, MMI_MAINSCREEN_WIDTH - 2},     /* 4 */
            { MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_RIGHT, MMI_MAINSCREEN_WIDTH - 2},     /* 4 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            100, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#endif

//|---------------------------|
//|      text              √ |
//|---------------------------|
// GUIITEM_STYLE_ONE_LINE_CHECK,
// s_item_style_1_text

//|---------------------------|
//|     text              √  |
//|---------------------------|
// GUIITEM_STYLE_ONE_LINE_CHECK_EXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text_check_ext =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE * 2, MMI_LIST_ITEM_HEIGHT - 1},   /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE * 2, MMI_LIST_ITEM_HEIGHT - 1},   /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|      text             ⊙  |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_RADIO,
//s_item_style_1_text
//|---------------------------|
//|      text             ⊙  |
//|---------------------------|
// GUIITEM_STYLE_ONE_LINE_RADIO_EXT,
//s_item_style_1_text_check_ext
//|---------------------------|
//|      text           @  √ |
//|---------------------------|
// GUIITEM_STYLE_ONE_LINE_CHECK_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_check_icon =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT - 1 },       /* 2 */
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT - 1 },       /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 80
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|      text           @  ⊙ |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_RADIO_ICON,
//s_item_style_1_check_icon

//|---------------------------|
//| √     text               |
//|---------------------------|
//GUIITEM_STYLE_MULTI_CHOICE_PICKER_TEXT
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_multi_choice_picker_text =
{
    MMI_CHOICE_PICKER_LIST_ITEM_HEIGHT, MMI_CHOICE_PICKER_LIST_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { 0, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_CHOICE_PICKER_LIST_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },       /* 0 */
            { 0, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_CHOICE_PICKER_LIST_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },       /* 0 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//| @       text     		  |
//|---------------------------|
//GUIITEM_STYLE_MULTI_CHOICE_PICKER_INITIAL_AND_TEXT
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_multi_choice_picker_initial_and_text =
{
    MMI_CHOICE_PICKER_AVATAR_LIST_ITEM_HEIGHT, MMI_CHOICE_PICKER_AVATAR_LIST_ITEM_HEIGHT,
		
    {
		
        {
				GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
				{ MMI_LIST_ICON_LEFT_SPACE_MS+4, MMI_LIST_ICON_TOP_MARGIN_MS+8,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS-4,  MMI_CHOICE_PICKER_AVATAR_LIST_ITEM_HEIGHT - (MMI_LIST_ICON_LEFT_SPACE_MS+8)}, /*1*/
				{ MMI_LIST_ICON_LEFT_SPACE_MS+4, MMI_LIST_ICON_TOP_MARGIN_MS+8,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS-4,  MMI_CHOICE_PICKER_AVATAR_LIST_ITEM_HEIGHT - (MMI_LIST_ICON_LEFT_SPACE_MS+8)}, /*1*/
				MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
				0, 0
		},
		{
				GUIITEM_CONTENT_STATE_EFFECT_STR,
				{MMI_LIST_BIG_ICON_WIDTH_MS + (MMI_LIST_ICON_LEFT_SPACE_MS-1)*2,  MMI_LIST_ICON_LEFT_SPACE_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS, MMI_CHOICE_PICKER_AVATAR_LIST_ITEM_HEIGHT - (MMI_LIST_ICON_LEFT_SPACE_MS*2-1)}, /*0*/
				{MMI_LIST_BIG_ICON_WIDTH_MS + (MMI_LIST_ICON_LEFT_SPACE_MS-1)*2,  MMI_LIST_ICON_LEFT_SPACE_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS, MMI_CHOICE_PICKER_AVATAR_LIST_ITEM_HEIGHT - (MMI_LIST_ICON_LEFT_SPACE_MS*2-1)}, /*0*/
				MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
				0, 100
        },
		{0},
		{0},
		{0},
		{0},
    },
	
    1, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//| @       text     		  |
//|---------------------------|
//GUIITEM_STYLE_MULTI_CHOICE_PICKER_AVATAR_AND_TEXT
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_multi_choice_picker_avatar_and_text =
{
    MMI_CHOICE_PICKER_AVATAR_LIST_ITEM_HEIGHT, MMI_CHOICE_PICKER_AVATAR_LIST_ITEM_HEIGHT,

    {
{
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_LEFT_SPACE_MS-1,  MMI_LIST_ICON_LEFT_SPACE_MS+40,  MMI_LIST_ICON_LEFT_SPACE_MS-1+40}, /*1*/
			{ MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_LEFT_SPACE_MS-1,  MMI_LIST_ICON_LEFT_SPACE_MS+40,  MMI_LIST_ICON_LEFT_SPACE_MS-1+40}, 
		    0, 0,
            0, 0
        },
		{
			GUIITEM_CONTENT_STATE_EFFECT_STR,
			{MMI_LIST_BIG_ICON_WIDTH_MS + (MMI_LIST_ICON_LEFT_SPACE_MS-1)*2,  MMI_LIST_ITEM_TOP_MARGIN_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS, MMI_CHOICE_PICKER_AVATAR_LIST_ITEM_HEIGHT - (MMI_LIST_ITEM_TOP_MARGIN_MS*2-1)}, /*0*/
			{MMI_LIST_BIG_ICON_WIDTH_MS + (MMI_LIST_ICON_LEFT_SPACE_MS-1)*2,  MMI_LIST_ITEM_TOP_MARGIN_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS, MMI_CHOICE_PICKER_AVATAR_LIST_ITEM_HEIGHT - (MMI_LIST_ITEM_TOP_MARGIN_MS*2-1)}, /*0*/
			MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
			0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            {
              MMI_LIST_ICON_LEFT_SPACE_MS+3,
              MMI_LIST_ICON_LEFT_SPACE_MS,
              MMI_LIST_ICON_LEFT_SPACE_MS+40,
              MMI_LIST_ICON_LEFT_SPACE_MS-1+40,
            },
            {
              MMI_LIST_ICON_LEFT_SPACE_MS+3,
              MMI_LIST_ICON_LEFT_SPACE_MS,
              MMI_LIST_ICON_LEFT_SPACE_MS+40,
              MMI_LIST_ICON_LEFT_SPACE_MS-1+40,
            },
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
#ifdef MMIPB_RMVITEM_SUPPORT
//s_item_style_multi_choice_picker_noavatar_and_text
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_multi_choice_picker_noavatar_and_text =
{
    MMI_CHOICE_PICKER_AVATAR_LIST_ITEM_HEIGHT, MMI_CHOICE_PICKER_AVATAR_LIST_ITEM_HEIGHT,

    {
        {0},
		{
			GUIITEM_CONTENT_STATE_EFFECT_STR,
			{ MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_ICON_WIDTH_MS - MMI_LIST_BIG_ICON_WIDTH_MS - 2, MMI_CHOICE_PICKER_AVATAR_LIST_ITEM_HEIGHT - (MMI_LIST_ITEM_TOP_MARGIN_MS*2-1)}, 
            { MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_ICON_WIDTH_MS - MMI_LIST_BIG_ICON_WIDTH_MS - 2, MMI_CHOICE_PICKER_AVATAR_LIST_ITEM_HEIGHT - (MMI_LIST_ITEM_TOP_MARGIN_MS*2-1)}, 
			MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
			0, 100
        },
        {0},
        {0},
        {
            0,
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_ICON_WIDTH_MS - MMI_LIST_BIG_ICON_WIDTH_MS + 4, MMI_LIST_ICON_WIDTH_2_MS - 4, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_BIG_ICON_WIDTH_MS + 4, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - MMI_LIST_ICON_WIDTH_2_MS + 4},     /* 4 */
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_ICON_WIDTH_MS - MMI_LIST_BIG_ICON_WIDTH_MS + 4, MMI_LIST_ICON_WIDTH_2_MS - 4, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_BIG_ICON_WIDTH_MS + 4, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - MMI_LIST_ICON_WIDTH_2_MS + 4},     /* 4 */
            0, 0,
            0, 0
        },
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
#endif

/* 以下都为双行item, AND表示第二行 */

//|---------------------------|
//|        text               |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + 6},    /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + 6},    /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 +8, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},     /* 2 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 +8, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},     /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|        text         ⊙    |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_RADIO_TWO_LINE_TEXT_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_radio_2_text_text =
{
    MMI_RADIO_LIST_ITEM_HEIGHT, MMI_RADIO_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ITEM_LEFT_MARGIN, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (/*MMI_RADIO_ICON_WIDTH +*/ MMI_LIST_ITEM_LEFT_MARGIN), MMI_RADIO_LIST_ITEM_HEIGHT / 2 + 2 },/*0*/
            { MMI_LIST_ITEM_LEFT_MARGIN, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (/*MMI_RADIO_ICON_WIDTH +*/ MMI_LIST_ITEM_LEFT_MARGIN), MMI_RADIO_LIST_ITEM_HEIGHT / 2 + 2 },/*0*/
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_LEFT_MARGIN, MMI_RADIO_LIST_ITEM_HEIGHT / 2 + 2,  MMI_LIST_ITEM_RIGHT - (/*MMI_RADIO_ICON_WIDTH +*/ MMI_LIST_ITEM_LEFT_MARGIN), MMI_RADIO_LIST_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },/*1*/
            { MMI_LIST_ITEM_LEFT_MARGIN, MMI_RADIO_LIST_ITEM_HEIGHT / 2 + 2,  MMI_LIST_ITEM_RIGHT - (/*MMI_RADIO_ICON_WIDTH +*/ MMI_LIST_ITEM_LEFT_MARGIN), MMI_RADIO_LIST_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },/*1*/
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|        text         ⊙    |
//|---------------------------|
//GUIITEM_STYLE_RADIO_ONE_LINE_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_radio_1_text =
{
    MMI_LIST_ITEM_HEIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS,

    {
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_LEFT_MARGIN, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (/*MMI_RADIO_ICON_WIDTH +*/ MMI_LIST_ITEM_LEFT_MARGIN), MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_LEFT_MARGIN},/*0*/
            { MMI_LIST_ITEM_LEFT_MARGIN, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (/*MMI_RADIO_ICON_WIDTH +*/ MMI_LIST_ITEM_LEFT_MARGIN), MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_LEFT_MARGIN},/*0*/
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//| √     text           	  |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_MULTI_CHOICE_PICKER_TWO_LINE_TEXT_AND_TEXT
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_multi_choice_picker_two_line_text_and_text =
{
    MMI_CHOICE_PICKER_DOUBLE_LINE_LIST_ITEM_HEIGHT, MMI_CHOICE_PICKER_DOUBLE_LINE_LIST_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_CHOICE_PICKER_DOUBLE_LINE_LIST_ITEM_HEIGHT/2 + 2},       /* 0 */
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_CHOICE_PICKER_DOUBLE_LINE_LIST_ITEM_HEIGHT/2 + 2},       /* 0 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_CHOICE_PICKER_DOUBLE_LINE_LIST_ITEM_HEIGHT/2 + 2,  MMI_LIST_ITEM_RIGHT, MMI_CHOICE_PICKER_DOUBLE_LINE_LIST_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },       /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_CHOICE_PICKER_DOUBLE_LINE_LIST_ITEM_HEIGHT/2 + 2,  MMI_LIST_ITEM_RIGHT, MMI_CHOICE_PICKER_DOUBLE_LINE_LIST_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },       /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },   
        {0},        
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//| √  @   text              |
//|         text              |
//|---------------------------|
//Two label list item with size stamp and thumbnail--multi choice picker
//GUIITEM_STYLE_MULTI_CHOICE_PICKER_TWO_LINE_BIGICON_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_multi_choice_picker_two_line_bigicon_text_and_text =
{
    MMI_CHOICE_PICKER_DOUBLE_LINE_LIST_ITEM_HEIGHT, MMI_CHOICE_PICKER_DOUBLE_LINE_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { 0, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS-MMI_LIST_ITEM_BOTTOM_MARGIN_MS},       /* 0 */
            { 0, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS-MMI_LIST_ITEM_BOTTOM_MARGIN_MS},       /* 0 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2+2 },       /* 1 */
            { MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2+2 },       /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },   
        {
            0,
            { MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2+2,  MMI_LIST_ITEM_RIGHT-(MMI_LITT_SIZE_STAMP_WIDTH_MS+MMI_LIST_TEXT_LEFT_SPACE_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS-MMI_LIST_ITEM_BOTTOM_MARGIN_MS },       /* 2 */
            { MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2+2,  MMI_LIST_ITEM_RIGHT-(MMI_LITT_SIZE_STAMP_WIDTH_MS+MMI_LIST_TEXT_LEFT_SPACE_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS-MMI_LIST_ITEM_BOTTOM_MARGIN_MS },       /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },        
        {
            0,
            { MMI_LIST_ITEM_RIGHT_MS-MMI_LITT_SIZE_STAMP_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2+2,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS-MMI_LIST_ITEM_BOTTOM_MARGIN_MS },       /* 3 */
            { MMI_LIST_ITEM_RIGHT_MS-MMI_LITT_SIZE_STAMP_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2+2,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS-MMI_LIST_ITEM_BOTTOM_MARGIN_MS },       /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//| text            text      |
//| text            text      |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_AND_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_2text_and_2text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT * 2 / 3 - 1, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1}, /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT * 2 / 3 - 1, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1}, /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT * 2 / 3 + 1,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1}, /* 2 */
            { MMI_LIST_ITEM_RIGHT * 2 / 3 + 1,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1}, /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT * 2 / 3 - 1, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT * 2 / 3 - 1, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 50
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT * 2 / 3 + 1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            { MMI_LIST_ITEM_RIGHT * 2 / 3 + 1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|  @     text               |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_icon_text_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN},     /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN},     /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|        text           @   |
//|---------------------------|
//GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_toggle_1_text =
{
    MMI_LIST_ITEM_HEIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS,

    {
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_LEFT_MARGIN, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (MMI_TOGGLE_ICON_WIDTH + MMI_LIST_ITEM_LEFT_MARGIN), MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_LEFT_MARGIN},/*0*/
            { MMI_LIST_ITEM_LEFT_MARGIN, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (MMI_TOGGLE_ICON_WIDTH + MMI_LIST_ITEM_LEFT_MARGIN), MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_LEFT_MARGIN},/*0*/
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|        text           @   |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_TOGGLE_TWO_LINE_TEXT_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_toggle_2_text_text =
{
    MMI_TOGGLE_LIST_ITEM_HEIGHT, MMI_TOGGLE_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ITEM_LEFT_MARGIN, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (MMI_TOGGLE_ICON_WIDTH + MMI_LIST_ITEM_LEFT_MARGIN), MMI_TOGGLE_LIST_ITEM_HEIGHT / 2 + 2 },/*0*/
            { MMI_LIST_ITEM_LEFT_MARGIN, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (MMI_TOGGLE_ICON_WIDTH + MMI_LIST_ITEM_LEFT_MARGIN), MMI_TOGGLE_LIST_ITEM_HEIGHT / 2 + 2 },/*0*/
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_LEFT_MARGIN, MMI_TOGGLE_LIST_ITEM_HEIGHT / 2 + 2,  MMI_LIST_ITEM_RIGHT - (MMI_TOGGLE_ICON_WIDTH + MMI_LIST_ITEM_LEFT_MARGIN), MMI_TOGGLE_LIST_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },/*1*/
            { MMI_LIST_ITEM_LEFT_MARGIN, MMI_TOGGLE_LIST_ITEM_HEIGHT / 2 + 2,  MMI_LIST_ITEM_RIGHT - (MMI_TOGGLE_ICON_WIDTH + MMI_LIST_ITEM_LEFT_MARGIN), MMI_TOGGLE_LIST_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },/*1*/
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|  @ @   text               |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_2icon_text_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN},   /* 4 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN},   /* 4 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|  ⑴    text               |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_NUMBER_TEXT_AND_TEXT,
//s_item_style_2_text_text

//|---------------------------|
//|  @     text               |
//|        text     text      |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_icon_text_and_2text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE,   MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 }, /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 }, /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },   /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },   /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100,
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH * 3 / 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH * 3 / 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 50
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH * 3 / 5 + 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 4 */
            { MMI_MAINSCREEN_WIDTH * 3 / 5 + 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 4 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {0},
        {0},
    },

    1, 2
};


//|---------------------------|
//|  @     text               |
//|        text     text      |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT_SMALL,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_icon_text_and_2text_small =
{
    MMI_SMALL_LIST_ITEM_HEIGHT * 2 - 1, MMI_SMALL_LIST_ITEM_HEIGHT * 2 - 1,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_SMALL_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_SMALL_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_RIGHT, MMI_SMALL_LIST_ITEM_HEIGHT },     /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_RIGHT, MMI_SMALL_LIST_ITEM_HEIGHT },     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100,
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_SMALL_LIST_ITEM_HEIGHT, MMI_MAINSCREEN_WIDTH * 3 / 5, MMI_SMALL_LIST_ITEM_HEIGHT * 2 }, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_SMALL_LIST_ITEM_HEIGHT, MMI_MAINSCREEN_WIDTH * 3 / 5, MMI_SMALL_LIST_ITEM_HEIGHT * 2 }, /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 50
        },
        {
            0,
            { 100/*MMI_MAINSCREEN_WIDTH*3/5 + 2*/, MMI_SMALL_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_RIGHT, MMI_SMALL_LIST_ITEM_HEIGHT * 2 },     /* 4 */
            { 100/*MMI_MAINSCREEN_WIDTH*3/5 + 2*/, MMI_SMALL_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_RIGHT, MMI_SMALL_LIST_ITEM_HEIGHT * 2 },     /* 4 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            50, 100
        },
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};


#endif

//|---------------------------|
//|  @ @   text               |
//|        text     text      |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_2icon_text_and_2text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },     /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },     /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_DEFAULT_FONT_ASCII_WIDTH * 6 + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_DEFAULT_FONT_ASCII_WIDTH * 6 + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 2 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },   /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },   /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100,
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 18,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8 , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT  }, /* 4 */
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 18,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8 , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT  }, /* 4 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 50
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 5 */
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 5 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            50, 100
        },
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|  @     text     text      |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_icon_2text_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 },   /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 },   /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 9, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 9, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            //MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            SONG_FONT_28, SONG_FONT_28,
            0, 50
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 9, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 9, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            //MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            SONG_FONT_28, SONG_FONT_28,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_TOP_MARGIN},     /* 4 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_TOP_MARGIN},     /* 4 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};



//|---------------------------|
//|  @     text     text      |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_SMALL,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_icon_2text_and_text_small =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 },   /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 },   /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 50,
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_TOP_MARGIN},     /* 4 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_TOP_MARGIN},     /* 4 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|  @ @   text     text      |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_TWO_ICON_TWO_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_2icon_2text_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_MAINSCREEN_WIDTH / 3 * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_MAINSCREEN_WIDTH / 3 * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 50,
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH / 3 * 2 + MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 4 */
            { MMI_MAINSCREEN_WIDTH / 3 * 2 + MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 4 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },     /* 5 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },     /* 5 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
    },

    2, 4
};


//|---------------------------|
//|  ┏┓   text              |
//|  ┗┛            text     |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_text_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { 1,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },       /* 1 */
            { 1,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },     /* 2 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH / 3 * 2 + 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            { MMI_MAINSCREEN_WIDTH / 3 * 2 + 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};



//|---------------------------|
//|  ┏┓   text              |
//|  ┗┛   text              |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_text_and_text_2 =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            //for bug1278969，单选list布局和多选list布局保持一致，否则会导致128X160分辨率下布局错误
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};


#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|  ┏┓   text         @    |
//|  ┗┛   text              |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_text_icon_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { 1,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },   /* 1 icon */
            { 1,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },   /* 1 icon */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 text */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 text */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, 0, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, /* 3 icon */
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, 0, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, /* 3 icon */
            0, 0,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1}, /* 4 text */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1}, /* 4 text */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};



//|---------------------------|
//|  ┏┓   text       @ @ @  |
//|  ┗┛   text              |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_3ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_text_3icon_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { 1,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },     /* 1 */
            { 1,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },     /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + 3,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   /* 2 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + 3,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //3
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //3
            0, 0,
            100, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //4
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //4
            0, 0,
            100, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //5
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //5
            0, 0,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_HEIGHT + 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN},
            { MMI_LIST_ITEM_HEIGHT + 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN},
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
    },

    5, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|      text              ⊙ |
//|      text                 |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_RADIO,
// s_item_style_2_text_text
#endif

//|---------------------------|
//|      text           @  √ |
//|      text                 |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_CHECK_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_icon_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 2 * MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 2 * MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - 2 * MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE , 0, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            { MMI_LIST_ITEM_RIGHT - 2 * MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE , 0, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            0, 0,
            100, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - 2 * MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },  /* 2 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - 2 * MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },  /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|  ┏┓   text         @    |
//|  ┗┛   text              |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_2TEXT_AND_ICON,
//s_item_style_2_anim_text_icon_and_text
#endif

//|---------------------------|
//|         text         ┏┓ |
//|         text         ┗┛ |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_and_text_anim =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_RIGHT_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     // 1 第一行文字
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_RIGHT_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     // 1 第一行文字
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_SELECT_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_SELECT_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            0, 0,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_RIGHT_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, // 3 第二行文字.
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_RIGHT_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, // 3 第二行文字.
#if defined(MAINLCD_SIZE_128X160)
            MMI_LIST_BIG_ITEM_FONT, MMI_LIST_BIG_ITEM_FONT,
#else
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
#endif
            0, 100
        },
        {0},
        {0},
        {0},
    },

    0, 2
};

#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//| ┏┓    text         ┏┓ |
//| ┗┛    text         ┗┛ |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ANIM,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_text_and_text_anim =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 2 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 2 },       /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 0
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     // 1 第一行文字
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     // 1 第一行文字
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            0, 0,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_TOP_MARGIN}, // 3 第二行文字.
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_TOP_MARGIN}, // 3 第二行文字.
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
    },

    0, 2
};
#endif

//|---------------------------|
//|  text           ┏┓ ┏┓ |
//|  text           ┗┛ ┗┛ |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_TEXT_2ANIM_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_2anim_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_2ANIM_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   // 1 第一行文字
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_2ANIM_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   // 1 第一行文字
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH) - 2 * MMI_LIST_ICON_LEFT_SPACE, 1, MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, // 2 图片
            { MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH) - 2 * MMI_LIST_ICON_LEFT_SPACE, 1, MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, // 2 图片
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            0, 0,
            100, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_TOP_MARGIN}, // 3 第二行文字.
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_TOP_MARGIN}, // 3 第二行文字.
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
    },

    0, 3
};
/* 以下都为高亮时双行, 非高亮时单行的item */

//|---------------------------|
//|  @     text               |
//|        text               |
//|---------------------------|

//|---------------------------|
//|  @     text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_2_icon_text_and_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            GUIITEM_STATE_CONTENT_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE / 2, 1, MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2 },        /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_ITEM_HEIGHT },       /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            {0},
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN},     /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};



#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|  @ @   text               |
//|        text               |
//|---------------------------|

//|---------------------------|
//|  @ @   text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_2_2icon_text_and_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_DEFAULT_FONT_ASCII_WIDTH * 4 + MMI_LIST_ICON_LEFT_SPACE * 2 ,  MMI_LIST_ITEM_HEIGHT - 1 }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_DEFAULT_FONT_ASCII_WIDTH * 4 + MMI_LIST_ICON_LEFT_SPACE * 2 ,  0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT }, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            {0},
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN},   /* 4 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|  ⑴    text               |
//|        text               |
//|---------------------------|

//|---------------------------|
//|  ⑴    text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_TWO_LINE_NUMBER_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_2_text_and_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},    /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},    /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            {0},
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|  @     text               |
//|        text     text      |
//|---------------------------|

//|---------------------------|
//|  @     text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_2_icon_text_and_2text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE / 2, 1, MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE / 2,  MMI_LIST_ITEM_HEIGHT - 2 },        /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 }, /* 1 */
            0, 0,
            0, 0
        },        
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + (MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE + 2) / 2,  0,  MMI_LIST_ITEM_RIGHT / 2, MMI_LIST_ITEM_HEIGHT },        /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },   /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },        
        {
            0,
            {0},
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH * 3 / 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            0,
            {0},
            { MMI_MAINSCREEN_WIDTH * 3 / 5 + 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 4 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            50, 100
        },
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|  @ @   text               |
//|        text     text      |
//|---------------------------|

//|---------------------------|
//|  @ @   text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_2_2icon_text_and_2text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },     /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },     /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_DEFAULT_FONT_ASCII_WIDTH * 6 + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_DEFAULT_FONT_ASCII_WIDTH * 6 + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 2 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },   /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },   /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 18,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8 , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT  }, /* 4 */
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 18,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8 , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT  }, /* 4 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 50
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 5 */
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 5 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|  ┏┓   text       @ @ @  |
//|  ┗┛   text              |
//|---------------------------|

//|---------------------------|
//|  ┏┓   text       @ @ @  |
//|---------------------------|
//GUIITEM_STYLE_ONE_TWO_ANIM_TEXT_AND_TEXT_3ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_2_anim_text_and_text_3icon =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { 1,   1,  MMI_LIST_ITEM_HEIGHT - 3,  MMI_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            { 1,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },     /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_HEIGHT,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_HEIGHT * 3, MMI_LIST_ITEM_HEIGHT  - 1},     /* 2 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + 3,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 3, 1, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2, MMI_LIST_ITEM_HEIGHT - 2}, //3
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //3
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2, 1, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_ITEM_HEIGHT - 2}, //4
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //4
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT  - 2},//5
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //5
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            {0},
            { MMI_LIST_ITEM_HEIGHT + 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 6 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

#endif
/*以下都是三行*/

//|---------------------------|
//|        text               |
//|        text               |
//|        text               |
//|---------------------------|
// GUIITEM_STYLE_THREE_LINE_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_3_text =
{
    (MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2) * 3 - 1, (MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2) * 3 - 1,

    {
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { 2,   0,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     /* 1 */
            { 2,   0,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { 2,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1},     /* 1 */
            { 2,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1},     /* 1 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { 2,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_ITEM_RIGHT, (MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2) * 3 },    /* 1 */
            { 2,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_ITEM_RIGHT, (MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2) * 3 },    /* 1 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|        text               |
//|        text        @      |
//|        text               |
//|---------------------------|
// GUIITEM_STYLE_THREE_LINE_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_3_text_and_text_and_text_icon =
{
    (MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2) * 3 - 1, (MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2) * 3 - 1,

    {
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { 14,   MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 2 * MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},
            { 14,   MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 2 * MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { 14,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 2 * MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1},
            { 14,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 2 * MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1},
            MMI_LIST_BIG_ITEM_FONT, MMI_LIST_BIG_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { 14,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 2 * MMI_LIST_ICON_LEFT_SPACE, (MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2) * 3 - MMI_LIST_ITEM_BOTTOM_MARGIN },
            { 14,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 2 * MMI_LIST_ICON_LEFT_SPACE, (MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2) * 3 - MMI_LIST_ITEM_BOTTOM_MARGIN },
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1},
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1},
            0, 0,
            100, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#endif

//|---------------------------|
//|        text     @         |
//|        text     text      |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_TEXT_ICON_AND_TWO_TEXT_SMALL,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_icon_and_two_text_small =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 2 * MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1 }, /* 2 */
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 2 * MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1 }, /* 2 */
            0, 0,
            100, 100
        },
        {
            0,
#ifdef MAINLCD_SIZE_128X160
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 , MMI_MAINSCREEN_WIDTH / 3 + MMI_LIST_TEXT_LEFT_SPACE * 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN }, /* 3 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 , MMI_MAINSCREEN_WIDTH / 3 + MMI_LIST_TEXT_LEFT_SPACE * 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN }, /* 3 */
#else
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH / 2 + MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },   /* 3 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH / 2 + MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },   /* 3 */
#endif
            //MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            SONG_FONT_28, SONG_FONT_28,
            0, 50
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
#ifdef MAINLCD_SIZE_128X160
            { MMI_MAINSCREEN_WIDTH / 3 + MMI_LIST_TEXT_LEFT_SPACE * 4, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT + MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN }, /* 4 */
            { MMI_MAINSCREEN_WIDTH / 3 + MMI_LIST_TEXT_LEFT_SPACE * 4, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT + MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN }, /* 4 */
#else
            { MMI_LIST_TEXT_LEFT_SPACE + MMI_MAINSCREEN_WIDTH / 2 + 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN }, /* 4 */
            { MMI_LIST_TEXT_LEFT_SPACE + MMI_MAINSCREEN_WIDTH / 2 + 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN }, /* 4 */
#endif
            //MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            SONG_FONT_28, SONG_FONT_28,
            50, 100
        },
        {0},
        {0},
    },

    1, 3
};

#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//| ┏┓   text     text      |
//| ┗┛   @text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_2text_and_icon_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ICON_LEFT_SPACE  + 1,  0,  MMI_MAINSCREEN_WIDTH / 5 * 4, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1  }, /* 3 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ICON_LEFT_SPACE  + 1,  0,  MMI_MAINSCREEN_WIDTH / 5 * 4, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1  }, /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 10, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },  /* 4  4/5*/
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 10, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },  /* 4  4/5*/
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },     /* 5 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },     /* 5 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
    },

    0, 3
};

//|---------------------------|
//| ┏┓   text     text      |
//| ┗┛   text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_2text_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ICON_LEFT_SPACE + 1,  0,  MMI_MAINSCREEN_WIDTH / 5 * 4 - 6, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1  }, /* 3 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ICON_LEFT_SPACE + 1,  0,  MMI_MAINSCREEN_WIDTH / 5 * 4 - 6, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1  }, /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 10 , 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 4 */
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 10 , 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 4 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + 3 ,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },     /* 5 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + 3 ,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },     /* 5 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
    },

    0, 3
};


//|---------------------------|
//| ┏┓   @text             |
//| ┗┛   @text     text          |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_ICON_TEXT_AND_ICON_2TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_icon_text_and_icon_2text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},       /* ANIMATE */
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},       /* ANIMATE */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH - 8 , MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH - 8 , MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH - 14, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},   /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH - 14, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},   /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            {MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH * 3 / 5 + MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8 - 40 + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            {MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH * 3 / 5 + MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8 - 40 + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 50
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8 + MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_MAINSCREEN_WIDTH * 3 / 5 - 40, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT + 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8 + MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_MAINSCREEN_WIDTH * 3 / 5 - 40, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT + 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

#endif

//|---------------------------|
//| ┏┓   text               |
//| ┗┛                      |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_TEXT,//mary
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            //for bug1285748,bug1280528
#if defined (MAINLCD_SIZE_128X160)
            { MMI_LIST_ICON_LEFT_SPACE,   MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},      /* ANIMATE */
            { MMI_LIST_ICON_LEFT_SPACE,   MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},      /* ANIMATE */
#else
            { MMI_LIST_ICON_LEFT_SPACE,   MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},      /* ANIMATE */
            { MMI_LIST_ICON_LEFT_SPACE,   MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},      /* ANIMATE */
#endif
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            //for bug1285748,bug1280528
#if defined (MAINLCD_SIZE_128X160)
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 + 3*MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, /* 3 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 + 3*MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, /* 3 */
#else
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, /* 3 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, /* 3 */
#endif
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX //for bug1256646
};

#ifndef MMI_THEME_LIST_MINI_SUPPORT

//|---------------------------|
//| ┏┓   text               |
//| ┗┛                      |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_CHECK_ANIM_TEXT,//mary
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_text_2 =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},      /* ANIMATE */
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},      /* ANIMATE */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX   //for bug1268218
};

//|---------------------------|
//| ┏┓   text            ⊙ |
//| ┗┛                      |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_RADIO_ANIM_TEXT,//mary
//s_item_style_2_anim_text_2

#endif
//|---------------------------|
//| ┏┓   text             √|
//| ┗┛                      |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_CHECK_ANIM_TEXT_AND_TEXT,//mary
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_check_anim_text_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},      /* ANIMATE */
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},      /* ANIMATE */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,//bug1255133，该类型用于pb带多选框的list，字体太大，该小一些
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX //for bug1271736
};


//|---------------------------|
//| ┏┓   text             √|
//| ┗┛   text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_RADIO_ANIM_TEXT_AND_TEXT,//mary
//s_item_style_2_anim_text_and_text

#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|  text  text      畅?┏┓ |
//|  text           ┗┛ ┗┛ |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_2TEXT_2ANIM_AND_TEXT,//mary
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_2text_2anim_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN, MMI_LIST_TEXT_LEFT_SPACE + MMI_MAINSCREEN_WIDTH * 2 / 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1}, // 1 第一行文字
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN, MMI_LIST_TEXT_LEFT_SPACE + MMI_MAINSCREEN_WIDTH * 2 / 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1}, // 1 第一行文字
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE + MMI_MAINSCREEN_WIDTH * 2 / 5 + 5,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1}, // 1 第二行文字
            { MMI_LIST_TEXT_LEFT_SPACE + MMI_MAINSCREEN_WIDTH * 2 / 5 + 5,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1}, // 1 第二行文字
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH) - 2 * MMI_LIST_ICON_LEFT_SPACE, 1, MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, // 2 图片
            { MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH) - 2 * MMI_LIST_ICON_LEFT_SPACE, 1, MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, // 2 图片
            0, 0,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            0, 0,
            100, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_TOP_MARGIN}, // 3 第三行文字.
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_TOP_MARGIN}, // 3 第三行文字.
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//| ┏┓   @text  text           |
//| ┗┛   @text              |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_ICON_TWO_TEXT_AND_ICON_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_icon_two_text_and_icon_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},       /* ANIMATE */
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},       /* ANIMATE */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH - 8 , MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH - 8 , MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH - 14, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},   /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH - 14, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},   /* 1 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            0,
            {MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH * 3 / 5 + MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8 - 30 + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            {MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH * 3 / 5 + MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8 - 30 + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 5, 15, MMI_LIST_ITEM_RIGHT - 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 5, 15, MMI_LIST_ITEM_RIGHT - 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
    },

    0, MMILIST_INVALID_ITEM_INDEX
};


#ifdef MMI_PDA_SUPPORT
//|---------------------------|
//|        text     text  @   |
//|        text           @   |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_2text_1icon_and_text_icon =
{
    MMI_ISTYLE_LIST_ITEM_HEIGHT, MMI_ISTYLE_LIST_ITEM_HEIGHT,

    {
        {
            GUILIST_STATE_TEXT_STYLE,
            { MMI_LIST_ICON_LEFT_SPACE, MMI_ISTYLE_LIST_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_BIG_ICON_WIDTH, MMI_ISTYLE_LIST_ITEM_HEIGHT},     /* 3 */
            { MMI_LIST_ICON_LEFT_SPACE, MMI_ISTYLE_LIST_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_BIG_ICON_WIDTH, MMI_ISTYLE_LIST_ITEM_HEIGHT},     /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 50
        },
        {
            GUILIST_STATE_TEXT_STYLE,
            { MMI_LIST_ICON_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - 2 * MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_ISTYLE_LIST_ITEM_HEIGHT / 2 - 1 },     /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - 2 * MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_ISTYLE_LIST_ITEM_HEIGHT / 2 - 1 },     /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            50, 100
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH, MMI_ISTYLE_LIST_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT / 2, MMI_ISTYLE_LIST_ITEM_HEIGHT}, /* 2 */
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH, MMI_ISTYLE_LIST_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT / 2, MMI_ISTYLE_LIST_ITEM_HEIGHT}, /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            100, 100
        },
        {0},
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_DEFAULT_FONT_ASCII_WIDTH * 10,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE, MMI_ISTYLE_LIST_ITEM_HEIGHT - 1 },   /* 4 */
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_DEFAULT_FONT_ASCII_WIDTH * 10,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE, MMI_ISTYLE_LIST_ITEM_HEIGHT - 1 },   /* 4 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_BIG_ICON_WIDTH, 0, MMI_LIST_ITEM_RIGHT, MMI_ISTYLE_LIST_ITEM_HEIGHT - 1},
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_BIG_ICON_WIDTH, 0, MMI_LIST_ITEM_RIGHT, MMI_ISTYLE_LIST_ITEM_HEIGHT - 1},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            100, 100
        },
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|        text     text  @   |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_TEXT_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text_text_icon =
{
    MMI_ISTYLE_LIST_ITEM_HEIGHT, MMI_ISTYLE_LIST_ITEM_HEIGHT,

    {
        {
            GUILIST_STATE_TEXT_STYLE,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT / 2,  MMI_ISTYLE_LIST_ITEM_HEIGHT - 1},     /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT / 2,  MMI_ISTYLE_LIST_ITEM_HEIGHT - 1},     /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 50
        },
        {
            GUILIST_STATE_TEXT_STYLE,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_RIGHT / 2 ,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_ISTYLE_LIST_ITEM_HEIGHT - 1 },    /* 2 */
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_RIGHT / 2 ,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_ISTYLE_LIST_ITEM_HEIGHT - 1 },    /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, 0,  MMI_LIST_ITEM_RIGHT, MMI_ISTYLE_LIST_ITEM_HEIGHT - 1 },       /* 3 */
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, 0,  MMI_LIST_ITEM_RIGHT, MMI_ISTYLE_LIST_ITEM_HEIGHT - 1 },       /* 3 */
            0, 0,
            100, 100
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|  @     text       text   @|
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text_text_icon =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { 0, 0,  MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_ITEM_HEIGHT },       /* 1 */
            { 0, 0,  MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_ITEM_HEIGHT },       /* 1 */
            0, 0,
            0, 0
        },
        {
            GUILIST_STATE_TEXT_STYLE,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH,  0,  MMI_MAINSCREEN_WIDTH / 3 * 2 - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_HEIGHT },   /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH,  0,  MMI_MAINSCREEN_WIDTH / 3 * 2 - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_HEIGHT },   /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 50
        },
        {
            GUILIST_STATE_TEXT_STYLE,
            { MMI_MAINSCREEN_WIDTH / 3 * 2 + MMI_LIST_ICON_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_ITEM_HEIGHT },   /* 3 */
            { MMI_MAINSCREEN_WIDTH / 3 * 2 + MMI_LIST_ICON_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_ITEM_HEIGHT },   /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT},       /* 4 */
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT},       /* 4 */
            0, 0,
            100, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

#endif

//|---------------------------|
//||_____| text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_LEFT_SPACE_TEXT,   //xiyuan edit can adjust left space of text
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_left_space_text =
{
    MMI_LIST_EXPAND_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_PS,   0,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_EXPAND_ITEM_HEIGHT },       /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE_PS,   0,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_EXPAND_ITEM_HEIGHT },       /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0}
    },

    0, MMILIST_INVALID_ITEM_INDEX
};



#endif

//GUIITEM_STYLE_TWO_LINE_TEXT_ANIM_AND_TEXT,//minghu for cr123283
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_anim_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            GUILIST_STATE_TEXT_STYLE,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   // 1 第一行文字
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   // 1 第一行文字
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_TOP_MARGIN}, // 3 第二行文字.
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_TOP_MARGIN}, // 3 第二行文字.
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    0, 3
};

#ifndef MMI_THEME_LIST_MINI_SUPPORT
#ifdef QBTHEME_SUPPORT
//|---------------------------|
//|        text               |
//|        text               |
//|        anim               |
//|---------------------------|
//GUIITEM_STYLE_THREE_LINE_TEXT_TEXT_ANIM,    //cheng.luo
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_3_text_text_anim =
{
    MMI_LIST_QBTHEME_ITEM_HEIGHT, MMI_LIST_QBTHEME_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE, QB_FORM_MARGIN, MMI_LIST_ITEM_RIGHT, QB_FORM_MARGIN + MMI_DEFAULT_NORMAL_FONT_SIZE},
            { MMI_LIST_TEXT_LEFT_SPACE, QB_FORM_MARGIN, MMI_LIST_ITEM_RIGHT, QB_FORM_MARGIN + MMI_DEFAULT_NORMAL_FONT_SIZE},
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE, QB_FORM_MARGIN + MMI_DEFAULT_NORMAL_FONT_SIZE + MMI_LIST_HEIGHT_SPACE / 2, MMI_LIST_ITEM_RIGHT, QB_FORM_MARGIN + MMI_DEFAULT_NORMAL_FONT_SIZE + MMI_LIST_HEIGHT_SPACE / 2 + MMI_DEFAULT_SMALL_FONT_SIZE},
            { MMI_LIST_TEXT_LEFT_SPACE, QB_FORM_MARGIN + MMI_DEFAULT_NORMAL_FONT_SIZE + MMI_LIST_HEIGHT_SPACE / 2, MMI_LIST_ITEM_RIGHT, QB_FORM_MARGIN + MMI_DEFAULT_NORMAL_FONT_SIZE + MMI_LIST_HEIGHT_SPACE / 2 + MMI_DEFAULT_SMALL_FONT_SIZE},
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, QB_FORM_MARGIN + MMI_DEFAULT_NORMAL_FONT_SIZE + MMI_LIST_HEIGHT_SPACE + MMI_DEFAULT_SMALL_FONT_SIZE, MMI_LIST_ITEM_RIGHT, MMI_LIST_QBTHEME_ITEM_HEIGHT - QB_FORM_MARGIN },
            { MMI_LIST_ICON_LEFT_SPACE, QB_FORM_MARGIN + MMI_DEFAULT_NORMAL_FONT_SIZE + MMI_LIST_HEIGHT_SPACE + MMI_DEFAULT_SMALL_FONT_SIZE, MMI_LIST_ITEM_RIGHT, MMI_LIST_QBTHEME_ITEM_HEIGHT - QB_FORM_MARGIN },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_MARK_WIDTH + 1, QB_FORM_MARGIN, MMI_LIST_ITEM_RIGHT, QB_FORM_MARGIN + MMI_LIST_QBTHEME_MARK_WIDTH - 1},
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_MARK_WIDTH + 1, QB_FORM_MARGIN, MMI_LIST_ITEM_RIGHT, QB_FORM_MARGIN + MMI_LIST_QBTHEME_MARK_WIDTH - 1},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|                           |
//|   anim     text     icon  |
//|                           |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ANIM_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_anim_text_icon =
{
    MMI_LIST_QBTHEME_ITEM2_HEIGHT, MMI_LIST_QBTHEME_ITEM2_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, (MMI_LIST_QBTHEME_ITEM2_HEIGHT - MMI_LIST_QBTHEME_ANIM_HEIGHT) / 2, MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_QBTHEME_ANIM_WIDTH - 1, (MMI_LIST_QBTHEME_ITEM2_HEIGHT + MMI_LIST_QBTHEME_ANIM_HEIGHT) / 2},
            { MMI_LIST_ICON_LEFT_SPACE, (MMI_LIST_QBTHEME_ITEM2_HEIGHT - MMI_LIST_QBTHEME_ANIM_HEIGHT) / 2, MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_QBTHEME_ANIM_WIDTH - 1, (MMI_LIST_QBTHEME_ITEM2_HEIGHT + MMI_LIST_QBTHEME_ANIM_HEIGHT) / 2},
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_QBTHEME_ANIM_WIDTH - 1 + MMI_LIST_QBTHEME_ITEM2_SPACE, MMI_LIST_ITEM_TOP_MARGIN, MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_ICON_WIDTH - MMI_LIST_QBTHEME_ITEM2_SPACE * 2, MMI_LIST_QBTHEME_ITEM2_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_QBTHEME_ANIM_WIDTH - 1 + MMI_LIST_QBTHEME_ITEM2_SPACE, MMI_LIST_ITEM_TOP_MARGIN, MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_ICON_WIDTH - MMI_LIST_QBTHEME_ITEM2_SPACE * 2, MMI_LIST_QBTHEME_ITEM2_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_ICON_WIDTH - MMI_LIST_QBTHEME_ITEM2_SPACE, (MMI_LIST_QBTHEME_ITEM2_HEIGHT - MMI_LIST_QBTHEME_ICON_WIDTH) / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_ITEM2_SPACE, (MMI_LIST_QBTHEME_ITEM2_HEIGHT + MMI_LIST_QBTHEME_ICON_WIDTH) / 2 },
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_ICON_WIDTH - MMI_LIST_QBTHEME_ITEM2_SPACE, (MMI_LIST_QBTHEME_ITEM2_HEIGHT - MMI_LIST_QBTHEME_ICON_WIDTH) / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_ITEM2_SPACE, (MMI_LIST_QBTHEME_ITEM2_HEIGHT + MMI_LIST_QBTHEME_ICON_WIDTH) / 2 },
            0, 0,
            100, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_MARK_WIDTH + 1, MMI_LIST_ITEM_TOP_MARGIN, MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_TOP_MARGIN + MMI_LIST_QBTHEME_MARK_WIDTH - 1},
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_MARK_WIDTH + 1, MMI_LIST_ITEM_TOP_MARGIN, MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_TOP_MARGIN + MMI_LIST_QBTHEME_MARK_WIDTH - 1},
            0, 0,
            100, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

#endif
#endif
//|---------------------------|
//|   @     text            @ |
//|   @     text              |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_and_two_icon_text_and_icon_2 =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,
    {
        {
            0,
            {  0 , MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 3 / 4 },
            {  0 , MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 3 / 4 },
            0, 0,
            0, 0
        },
        {
            0,
            {  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_BIG_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            {  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_BIG_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            {  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + MMI_LIST_HEIGHT_SPACE / 2, MMI_LIST_ITEM_RIGHT/3 + MMI_LIST_ITEM_SELECT_ICON_WIDTH ,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 1 */
            {  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + MMI_LIST_HEIGHT_SPACE / 2, MMI_LIST_ITEM_RIGHT/3 + MMI_LIST_ITEM_SELECT_ICON_WIDTH ,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 1 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 0
        },
        {
            0,
            {MMI_LIST_ITEM_RIGHT /3 + MMI_LIST_ITEM_SELECT_ICON_WIDTH + 4, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + MMI_LIST_HEIGHT_SPACE / 2, MMI_LIST_ITEM_RIGHT/3 * 2 , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT },
            {MMI_LIST_ITEM_RIGHT /3 + MMI_LIST_ITEM_SELECT_ICON_WIDTH + 4, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + MMI_LIST_HEIGHT_SPACE / 2, MMI_LIST_ITEM_RIGHT/3 * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT },
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            {MMI_LIST_ITEM_RIGHT/3 * 2 + 4, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + MMI_LIST_HEIGHT_SPACE / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT },
            {MMI_LIST_ITEM_RIGHT/3 * 2 + 4, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + MMI_LIST_HEIGHT_SPACE / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT },
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 6 , MMI_LIST_ITEM_TOP_MARGIN, MMI_LIST_ITEM_RIGHT , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + MMI_LIST_HEIGHT_SPACE},
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 6 , MMI_LIST_ITEM_TOP_MARGIN, MMI_LIST_ITEM_RIGHT , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + MMI_LIST_HEIGHT_SPACE},
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            100, 100
        },
    },

    1, 3
};

//|---------------------------|
//|  @     text     text      |
//|        text               |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_NORMAL,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_icon_two_text_and_text_normal =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,
    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 },   /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 },   /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 7, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 7, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 7, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 7, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_TOP_MARGIN},     /* 4 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_TOP_MARGIN},     /* 4 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
		{0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};

#ifndef MMI_THEME_LIST_MINI_SUPPORT
#ifdef SEARCH_SUPPORT
//|---------------------------|
//|  @     text               |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_bigicon_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,
    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MENUICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + 2 ,  MENUICON_HEIGHT + 2 },  /* ANIMATE */
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MENUICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + 2 ,  MENUICON_HEIGHT + 2 },  /* ANIMATE */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE + MENUICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, /* 3 */
            { MMI_LIST_ICON_LEFT_SPACE + MENUICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#endif

//|---------------------------|
//| ┏┓   text     text      |
//| ┗┛   text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_2,
#ifdef MMI_PDA_SUPPORT
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_two_text_and_text_2 =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,
    {
        {
            0,
            { 1,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + 3,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, 
            { 1,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + 3,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, 
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + 6,  0,  MMI_MAINSCREEN_WIDTH/5*4 - 7, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1  }, 
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + 6,  0,  MMI_MAINSCREEN_WIDTH/5*4 - 7, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1  }, 
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH/5*4 - 6, 0,  MMI_LIST_ITEM_RIGHT + 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1 }, 
            { MMI_MAINSCREEN_WIDTH/5*4 - 6, 0,  MMI_LIST_ITEM_RIGHT + 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1 }, 
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + 6,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, 
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + 6,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, 
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#else
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_two_text_and_text_2 =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,
    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, 
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, 
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + 2,  0,  MMI_MAINSCREEN_WIDTH/4*3 - 7, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1  }, 
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + 2,  0,  MMI_MAINSCREEN_WIDTH/4*3 - 7, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1  }, 
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH/4*3 - 6, 0,  MMI_LIST_ITEM_RIGHT + 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1 }, 
            { MMI_MAINSCREEN_WIDTH/4*3 - 6, 0,  MMI_LIST_ITEM_RIGHT + 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1 }, 
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, 
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, 
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#endif
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_two_text_and_text_3_scroll = 
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,
    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, 
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, 
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + 2,  0,  MMI_MAINSCREEN_WIDTH/4*3 - 7, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1  }, 
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + 2,  0,  MMI_MAINSCREEN_WIDTH/4*3 - 7, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1  }, 
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH/5*4 - 6, 0,  MMI_LIST_ITEM_RIGHT + 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1 }, 
            { MMI_MAINSCREEN_WIDTH/5*4 - 6, 0,  MMI_LIST_ITEM_RIGHT + 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1 }, 
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, 
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, 
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};
#if defined(ZDT_PHILIPS_DIAL_SEARCH_STYLE) //   A20210129  MMI_LIST_ITEM_BIG_ICON_WIDTH--24
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_two_text_and_text_3_scroll_new = 
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,
    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  24 + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, 
            { MMI_LIST_ICON_LEFT_SPACE,   0,  24 + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, 
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { 24 + MMI_LIST_ICON_LEFT_SPACE + 2,  0,  MMI_MAINSCREEN_WIDTH/4*3 - 7, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1  }, 
            { 24 + MMI_LIST_ICON_LEFT_SPACE + 2,  0,  MMI_MAINSCREEN_WIDTH/4*3 - 7, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1  }, 
            SONG_FONT_30, SONG_FONT_30,//MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH/5*4 - 6, 0,  MMI_LIST_ITEM_RIGHT + 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1 }, 
            { MMI_MAINSCREEN_WIDTH/5*4 - 6, 0,  MMI_LIST_ITEM_RIGHT + 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1 }, 
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { 24 + MMI_LIST_ICON_LEFT_SPACE + 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, 
            { 24 + MMI_LIST_ICON_LEFT_SPACE + 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, 
            SONG_FONT_30, SONG_FONT_30,//MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};
#endif
#endif

//add for pb_avatar_text_item_quick_action
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_pb_one_line_avatar_text_left_focus_ms =
{
    MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS,
    {  
         {
            0,
            {
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MASK_X_MS,
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MARGIN_MS,
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MASK_X_MS+MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_HEIGHT_WIDTH_MS-1,
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MARGIN_MS+MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_HEIGHT_WIDTH_MS-1,
            },
            {
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MASK_X_MS,
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MARGIN_MS,
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MASK_X_MS+MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_HEIGHT_WIDTH_MS-1,
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MARGIN_MS+MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_HEIGHT_WIDTH_MS-1,
            },
            0, 0,
            0, 0
        },
			
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            {
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MASK_X_MS + MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_HEIGHT_WIDTH_MS + MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MARGIN_MS-1,
              MMI_LIST_PB_QUICK_ACTION_ITEM_TEXT_TOP_MARGIN_MS,
              239,
              MMI_LIST_PB_QUICK_ACTION_ITEM_TEXT_TOP_MARGIN_MS + MMI_LIST_PB_QUICK_ACTION_ITEM_TEXT_HEIGHT_MS,
            },
            {
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MASK_X_MS + MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_HEIGHT_WIDTH_MS + MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MARGIN_MS-1,
              MMI_LIST_PB_QUICK_ACTION_ITEM_TEXT_TOP_MARGIN_MS,
              239,
              MMI_LIST_PB_QUICK_ACTION_ITEM_TEXT_TOP_MARGIN_MS + MMI_LIST_PB_QUICK_ACTION_ITEM_TEXT_HEIGHT_MS,
            },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            {
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MASK_X_MS+3,
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MARGIN_MS,
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MASK_X_MS+MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_HEIGHT_WIDTH_MS-1,
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MARGIN_MS+MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_HEIGHT_WIDTH_MS-1,
            },
            {
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MASK_X_MS+3,
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MARGIN_MS,
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MASK_X_MS+MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_HEIGHT_WIDTH_MS-1,
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MARGIN_MS+MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_HEIGHT_WIDTH_MS-1,
            },
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
#ifdef MMIPB_RMVITEM_SUPPORT
//s_item_style_pb_one_line_noavatar_text_left_focus_ms
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_pb_one_line_noavatar_text_left_focus_ms =
{
    MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS,
    {  
        {0},
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            {
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MASK_X_MS + MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MARGIN_MS,
              MMI_LIST_PB_QUICK_ACTION_ITEM_TEXT_TOP_MARGIN_MS,
              MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_ICON_WIDTH_MS - 12,
              MMI_LIST_PB_QUICK_ACTION_ITEM_TEXT_TOP_MARGIN_MS + MMI_LIST_PB_QUICK_ACTION_ITEM_TEXT_HEIGHT_MS,
            },
            {
              MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MASK_X_MS + MMI_LIST_PB_QUICK_ACTION_ITEM_AVATAR_MARGIN_MS,
              MMI_LIST_PB_QUICK_ACTION_ITEM_TEXT_TOP_MARGIN_MS,
              MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_ICON_WIDTH_MS - 12,
              MMI_LIST_PB_QUICK_ACTION_ITEM_TEXT_TOP_MARGIN_MS + MMI_LIST_PB_QUICK_ACTION_ITEM_TEXT_HEIGHT_MS,
            },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
#endif
/*Microsoft project need to support+++*/
//|---------------------------|
//|           text            |
//|---------------------------|
// One line item
// GUIITEM_STYLE_ONE_LINE_TEXT_MS
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text_ms =
{
    MMI_LIST_ITEM_HEIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS,
	
    {
        {
            GUIITEM_CONTENT_STATE_NONE,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},/* 0 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},/* 0 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },
	
    0, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//| [leftQuick] text          |
//|---------------------------|
//One line item--Quick Action list item
//GUIITEM_STYLE_ONE_LINE_TEXT_WITH_LEFT_FOCUS_MS
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text_with_left_focus_ms = 
{
    MMI_LIST_ITEM_HEIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS,

    {
        {
            GUIITEM_CONTENT_STATE_NONE,
            { MMI_LIST_ITEM_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},/* 0 */
            { MMI_LIST_ITEM_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},/* 0 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },
	
    1, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//| text          [rightQuick]|
//|---------------------------|
//One line item -- Quick Action list item
//GUIITEM_STYLE_ONE_LINE_TEXT_WITH_RIGHT_FOCUS_MS
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text_with_right_focus_ms = 
{
    MMI_LIST_ITEM_HEIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS,

    {
        {
            GUIITEM_CONTENT_STATE_NONE,
            {MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_ITEM_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},/* 0 */
            {MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_ITEM_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},/* 0 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },
	
    1, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|   @       text            |
//|---------------------------|
//One line item with icon
//GUIITEM_STYLE_ONE_LINE_ICON_TEXT_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text_ms =
{
    MMI_LIST_ITEM_HEIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS,
	
    {
        {
            GUIITEM_CONTENT_STATE_NONE,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_TOP_MARGIN_MS,  MMI_LIST_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ICON_BOTTOM_MARGIN_MS}, /*1*/
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_TOP_MARGIN_MS,  MMI_LIST_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ICON_BOTTOM_MARGIN_MS}, /*1*/
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS + MMI_LIST_TEXT_LEFT_SPACE_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},/*2*/
            { MMI_LIST_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS + MMI_LIST_TEXT_LEFT_SPACE_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

//add for single item choice picker style.
//|---------------------------|
//| √  @       text          |
//|---------------------------|
// GUIITEM_STYLE_ONE_LINE_CHECK_ICON_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_check_icon_ms =
{
    MMI_LIST_ITEM_HEIGHT_MS+ 6, MMI_LIST_ITEM_HEIGHT_MS +6,      /////// ZDT_PRODUCT_Z685S 

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ICON_TOP_MARGIN_MS,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ICON_BOTTOM_MARGIN_MS },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ICON_TOP_MARGIN_MS,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ICON_BOTTOM_MARGIN_MS},       /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_ICON_TOP_MARGIN_MS-4 ,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT_MS + 7 - MMI_LIST_ICON_BOTTOM_MARGIN_MS  },       /* 2 */
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_ICON_TOP_MARGIN_MS-4  ,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT_MS+ 7  - MMI_LIST_ICON_BOTTOM_MARGIN_MS },       /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 80
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
//End

//|---------------------------|
//|   @       text            |
//|---------------------------|
//目前未使用
// GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_bigicon_text_ms =
{
    MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS,
    {
        {
            GUIITEM_CONTENT_STATE_NONE,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_TOP_MARGIN_MS,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - MMI_LIST_ICON_BOTTOM_MARGIN_MS},  /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_TOP_MARGIN_MS,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - MMI_LIST_ICON_BOTTOM_MARGIN_MS},  /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE_MS + MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_TEXT_LEFT_SPACE_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ITEM_TOP_MARGIN_MS*2-1)}, /*2*/
            { MMI_LIST_ICON_LEFT_SPACE_MS + MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_TEXT_LEFT_SPACE_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ITEM_TOP_MARGIN_MS*2-1)}, /*2*/
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|   @       text            |
//|---------------------------|
//One line avatar item
// GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_MASK_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_bigicon_text_with_mask_ms =
{
    MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS,
    {
        //icon
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - MMI_LIST_ICON_LEFT_SPACE_MS},  /* icon */
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - MMI_LIST_ICON_LEFT_SPACE_MS},  /* icon */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            {MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ITEM_TOP_MARGIN_MS*2-1)}, /*text*/
            {MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ITEM_TOP_MARGIN_MS*2-1)}, /*text*/
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        //add for name abbreviation text area
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { MMI_LIST_ICON_LEFT_SPACE_MS+3, MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - MMI_LIST_ICON_LEFT_SPACE_MS},  /* icon */
            { MMI_LIST_ICON_LEFT_SPACE_MS+3, MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - MMI_LIST_ICON_LEFT_SPACE_MS},  /* icon */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};
#ifdef MMIPB_RMVITEM_SUPPORT
//s_item_style_no_bigicon_text_with_mask_ms
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_no_bigicon_text_with_mask_ms =
{
    MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS,
    {
        {0},
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            {MMI_LIST_ICON_LEFT_SPACE_MS + 5,  MMI_LIST_ITEM_TOP_MARGIN_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_ICON_WIDTH_MS - 12, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ITEM_TOP_MARGIN_MS*2-1)},
            {MMI_LIST_ICON_LEFT_SPACE_MS + 5,  MMI_LIST_ITEM_TOP_MARGIN_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_ICON_WIDTH_MS - 12, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ITEM_TOP_MARGIN_MS*2-1)},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        //add for name abbreviation text area
        {0},
        {0},
        {
            0,
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_ICON_WIDTH_MS - 6, MMI_LIST_ICON_WIDTH_2_MS - 4, MMI_LIST_ITEM_RIGHT_MS - 6, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - MMI_LIST_ICON_WIDTH_2_MS + 4},     /* 4 */
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_ICON_WIDTH_MS - 6, MMI_LIST_ICON_WIDTH_2_MS - 4, MMI_LIST_ITEM_RIGHT_MS - 6, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - MMI_LIST_ICON_WIDTH_2_MS + 4},     /* 4 */
            0, 0,
            0, 100
        },
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
#endif
//|---------------------------|
//|   text       text         |
//|---------------------------|
//One line avatar item –Initials (no photo available)
// GUIITEM_STYLE_ONE_LINE_INITIALTEXT_TEXT_WITH_MASK_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_initialtext_text_with_mask_ms =
{
    MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS,
    {
        //text
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS+9,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ICON_LEFT_SPACE_MS+9)},  /* text */
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS+9,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ICON_LEFT_SPACE_MS+9)}, 
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            {MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ICON_LEFT_SPACE_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ICON_LEFT_SPACE_MS*2-1)}, /*text*/
            {MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ICON_LEFT_SPACE_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ICON_LEFT_SPACE_MS*2-1)}, /*text*/
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|   @       text   [simsel] |
//|---------------------------|
//One line avatar item -- Quick Action for sim selection
// GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_SIMSEL_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_bigicon_text_with_simsel_ms =
{
    MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS,
    {
        //icon
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - MMI_LIST_ICON_LEFT_SPACE_MS},  /* icon */
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - MMI_LIST_ICON_LEFT_SPACE_MS},  /* icon */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            //{MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ITEM_TOP_MARGIN_MS*2-1)}, /*text*/
            {MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_SIMSEL_WIDTH_MS+MMI_LIST_ICON_LEFT_SPACE_MS*2), MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ITEM_TOP_MARGIN_MS*2-1)}, /*text*/
            {MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_SIMSEL_WIDTH_MS+MMI_LIST_ICON_LEFT_SPACE_MS*2), MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ITEM_TOP_MARGIN_MS*2-1)}, /*text*/
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|   text      text  [simsel]|
//|---------------------------|
//One line avatar item –Initials (no photo available) -- Quick Action for sim selection
// GUIITEM_STYLE_ONE_LINE_INITIALTEXT_TEXT_WITH_SIMSEL_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_initialtext_text_with_simsel_ms =
{
    MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS,
    {
        //text
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS+9,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ICON_LEFT_SPACE_MS+9)},  /* text */
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS+9,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ICON_LEFT_SPACE_MS+9)}, 
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            {MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ICON_LEFT_SPACE_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ICON_LEFT_SPACE_MS*2-1)}, /*text*/
            {MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ICON_LEFT_SPACE_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_SIMSEL_WIDTH_MS+MMI_LIST_ICON_LEFT_SPACE_MS*2), MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ICON_LEFT_SPACE_MS*2-1)}, /*text*/
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//| [leftQuick]  @      text  |
//|---------------------------|
//One line avatar item--Quick Action list item
//GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_LEFT_FOCUS_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_bigicon_text_with_left_focus_ms =
{
    MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS,

    {
        /*icon*/    
        {
            0,
            { MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TRIANGLE_WIDTH_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - MMI_LIST_ICON_LEFT_SPACE_MS},  /* icon */
            { MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TRIANGLE_WIDTH_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - MMI_LIST_ICON_LEFT_SPACE_MS},  /* icon */
            0, 0,
            0, 0
        },                
        /*text*/
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            {MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TRIANGLE_WIDTH_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ITEM_TOP_MARGIN_MS*2-1)}, /*text*/
            {MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TRIANGLE_WIDTH_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ITEM_TOP_MARGIN_MS*2-1)}, /*text*/
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|  @      text [rightQuick] |
//|---------------------------|
//One line avatar item -- Quick Action list item
// GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_RIGHT_FOCUS_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_bigicon_text_with_right_focus_ms =
{
    MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS,
    {
        //icon
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - MMI_LIST_ICON_LEFT_SPACE_MS},  /* icon */
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - MMI_LIST_ICON_LEFT_SPACE_MS},  /* icon */
            0, 0,
            0, 0
        },
        /*text*/
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            {MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ITEM_TOP_MARGIN_MS*2-1)}, /*text*/
            {MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ITEM_TOP_MARGIN_MS*2-1)}, /*text*/
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},            
    },

    2, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//| [leftQuick]  @       text |
//|---------------------------|
//One line avatar item –Initials (no photo available)-- Quick Action list item
//GUIITEM_STYLE_ONE_LINE_INITIALTEXT_TEXT_WITH_LEFT_FOCUS_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_initialtext_text_with_left_focus_ms =
{
    MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS,
    {
        //text
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TRIANGLE_WIDTH_MS+MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS+9,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ICON_LEFT_SPACE_MS+9)},  /* text */
            { MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TRIANGLE_WIDTH_MS+MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS+9,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ICON_LEFT_SPACE_MS+9)}, 
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            {MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TRIANGLE_WIDTH_MS,  MMI_LIST_ICON_LEFT_SPACE_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ICON_LEFT_SPACE_MS*2-1)}, /*text*/
            {MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TRIANGLE_WIDTH_MS,  MMI_LIST_ICON_LEFT_SPACE_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ICON_LEFT_SPACE_MS*2-1)}, /*text*/
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//| @       text  [rightQuick]|
//|---------------------------|
//One line avatar item –Initials (no photo available)-- Quick Action list item
//GUIITEM_STYLE_ONE_LINE_INITIALTEXT_TEXT_WITH_RIGHT_FOCUS_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_initialtext_text_with_right_focus_ms =
{
    MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS,
    {
        //text
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS+9,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ICON_LEFT_SPACE_MS+9)},  /* text */
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS+9,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ICON_LEFT_SPACE_MS+9)}, 
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            {MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ICON_LEFT_SPACE_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ICON_LEFT_SPACE_MS*2-1)}, /*text*/
            {MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ICON_LEFT_SPACE_MS*2-1,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS - (MMI_LIST_ICON_LEFT_SPACE_MS*2-1)}, /*text*/
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|   @   text           text |
//|---------------------------|
//One line item with icon and counter
//GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text_and_text_ms =
{
    MMI_LIST_ITEM_HEIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_TOP_MARGIN_MS, MMI_LIST_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ICON_BOTTOM_MARGIN_MS},  /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_TOP_MARGIN_MS, MMI_LIST_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ICON_BOTTOM_MARGIN_MS},    
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS + MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_COUNTER_WIDTH_MS - MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS}, /* 2 */
            { MMI_LIST_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS + MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_COUNTER_WIDTH_MS - MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS}, /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_COUNTER_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS*2,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},   /* 3 */
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_COUNTER_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS*2,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},   /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            100, 100
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|      text      text       |
//|---------------------------|
//One line drill down item with counter
//GUIITEM_STYLE_ONE_LINE_TEXT_AND_TEXT_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text_and_text_ms =
{
    MMI_LIST_ITEM_HEIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS - (MMI_LIST_COUNTER_WIDTH_MS + 11) - MMI_LIST_TEXT_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS - (MMI_LIST_COUNTER_WIDTH_MS + 11) - MMI_LIST_TEXT_LEFT_SPACE_MS,  MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT_MS - (MMI_LIST_COUNTER_WIDTH_MS + 11), MMI_LIST_ITEM_TOP_MARGIN_MS*2, MMI_LIST_ITEM_RIGHT_MS,  MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},
            { MMI_LIST_ITEM_RIGHT_MS - (MMI_LIST_COUNTER_WIDTH_MS + 11), MMI_LIST_ITEM_TOP_MARGIN_MS*2, MMI_LIST_ITEM_RIGHT_MS,  MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//GUIITEM_STYLE_ONE_LINE_TEXT_AND_TEXT_MS_EXT
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text_and_text_ms_ext =
{
    MMI_LIST_ITEM_HEIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_COUNTER_WIDTH_MS + MMI_LIST_TEXT_LEFT_SPACE_MS+30,  MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_COUNTER_WIDTH_MS + MMI_LIST_TEXT_LEFT_SPACE_MS+30,  MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_TEXT_LEFT_SPACE_MS + MMI_LIST_COUNTER_WIDTH_MS + 50, MMI_LIST_ITEM_TOP_MARGIN_MS*2, MMI_LIST_ITEM_RIGHT_MS,  MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},
            { MMI_LIST_TEXT_LEFT_SPACE_MS + MMI_LIST_COUNTER_WIDTH_MS + 50, MMI_LIST_ITEM_TOP_MARGIN_MS*2, MMI_LIST_ITEM_RIGHT_MS,  MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//End
//added by du.yuxuan for stopwatch
//|---------------------------|
//|      text      text       |
//|---------------------------|
//One line item text and text
//GUIITEM_STYLE_ONE_LINE_2TEXT_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_extras_2text_ms =
{
    MMI_LIST_EXTRAS_ITEM_HEIGHT_MS, MMI_LIST_EXTRAS_ITEM_HEIGHT_MS,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_EXTRAS_ITEM_TOP_MARGIN_MS, MMI_LIST_TEXT_LEFT_SPACE_MS + MMI_LIST_EXTRAS_COUNTER_WIDTH_MS,  MMI_LIST_EXTRAS_ITEM_HEIGHT_MS - MMI_LIST_EXTRAS_ITEM_BOTTOM_MARGIN_MS},
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_EXTRAS_ITEM_TOP_MARGIN_MS, MMI_LIST_TEXT_LEFT_SPACE_MS + MMI_LIST_EXTRAS_COUNTER_WIDTH_MS,  MMI_LIST_EXTRAS_ITEM_HEIGHT_MS - MMI_LIST_EXTRAS_ITEM_BOTTOM_MARGIN_MS},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_TEXT_LEFT_SPACE_MS + MMI_LIST_EXTRAS_COUNTER_WIDTH_MS + MMI_LIST_EXTRAS_ITEM_MIDDLE_SPACE_MS, MMI_LIST_EXTRAS_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS,  MMI_LIST_EXTRAS_ITEM_HEIGHT_MS - MMI_LIST_EXTRAS_ITEM_BOTTOM_MARGIN_MS},
            { MMI_LIST_TEXT_LEFT_SPACE_MS + MMI_LIST_EXTRAS_COUNTER_WIDTH_MS + MMI_LIST_EXTRAS_ITEM_MIDDLE_SPACE_MS, MMI_LIST_EXTRAS_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS,  MMI_LIST_EXTRAS_ITEM_HEIGHT_MS - MMI_LIST_EXTRAS_ITEM_BOTTOM_MARGIN_MS},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//end

//|-----------------------------------------------------------|
//|                         text                              |
//|这种风格与GUIITEM_STYLE_ONE_LINE_TEXT唯一不同就在于text坐标|
//|-----------------------------------------------------------|
//One line sub value drill down item
//GUIITEM_STYLE_ONE_LINE_SUB_TEXT_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_sub_text_ms =
{
    MMI_LIST_ITEM_HEIGHT_MS, MMI_LIST_ITEM_HEIGHT_MS,

    {
        {
            0,
            { MMI_LIST_SUB_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS,  MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},
            { MMI_LIST_SUB_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS,  MMI_LIST_ITEM_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|        text               |
//|        text               |
//|---------------------------|
//Two label list item - 01
//GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_text_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|        text       [simSel]|
//|        text       [simSel]|
//|---------------------------|
//Two label list item - 01--Quick action used for SIM selection
//GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_WITH_SIMSEL_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_text_with_simsel_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_SIMSEL_WIDTH_MS+MMI_LIST_ICON_LEFT_SPACE_MS*2), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_SIMSEL_WIDTH_MS+MMI_LIST_ICON_LEFT_SPACE_MS*2), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|        text               |
//|        text               |
//|---------------------------|
//Two label list item - 02
//GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_2_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_text_2_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 - 2},    /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 - 2},    /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 - 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 - 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};
//add by James li begin
//|---------------------------|
//|        text               |
//|        text               |
//|---------------------------|
//Two label list item - 02 for alarm
//GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_2_MS_FOR_ALARM,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_text_2_ms_for_alarm =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 - 2},    /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 - 2},    /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 - 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 - 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};
//add by James li end
//|---------------------------|
//|        text       [simSel]|
//|        text       [simSel]|
//|---------------------------|
//Two label list item - 02--Quick action used for SIM selection
//GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_2_WITH_SIMSEL_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_text_2_with_simsel_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 - 2},    /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_SIMSEL_WIDTH_MS+MMI_LIST_ICON_LEFT_SPACE_MS*2), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 - 2},    /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 - 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 - 2, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_SIMSEL_WIDTH_MS+MMI_LIST_ICON_LEFT_SPACE_MS*2), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|      text          text   |
//|      text                 |
//|---------------------------|
//Two label list item with time stamp
//GUIITEM_STYLE_TWO_LINE_2TEXT_AND_TEXT_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_2text_and_text_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_TIME_STAMP_WIDTH_MS - MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_TIME_STAMP_WIDTH_MS - MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_TIME_STAMP_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS + 4, MMI_LIST_ITEM_RIGHT_MS - 6, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},     /* 2 */
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_TIME_STAMP_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS + 4, MMI_LIST_ITEM_RIGHT_MS - 6, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},     /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|      text          text   |
//|      text                 |
//|---------------------------|
//Two label list item with time stamp--Quick Action list item
//GUIITEM_STYLE_THREE_PARTS_2TEXT_AND_TEXT_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_3parts_2text_and_text_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        /*1*/
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_TIME_STAMP_WIDTH_MS - MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_TIME_STAMP_WIDTH_MS - MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        /*2*/
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_TIME_STAMP_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS + 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},     /* 2 */
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_TIME_STAMP_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS + 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},     /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        /*3*/
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//| [leftQuick]  text         |
//| [leftQuick]  text         |
//|---------------------------|
//Two label list item with time stamp--Quick Action list item
//GUIITEM_STYLE_2TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2text_and_text_with_left_focus_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        /*1*/
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        /*2*/
        {0},
        /*3*/
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//| text         [rightQuick] |
//| text         [rightQuick] |
//|---------------------------|
//Two label list item with time stamp--Quick Action list item
//GUIITEM_STYLE_2TEXT_AND_TEXT_WITH_RIGHT_FOCUS_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2text_and_text_with_right_focus_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        /*1*/
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        /*2*/
        {0},
        /*3*/
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|      text          text   |
//|      text           @     |
//|---------------------------|
//Two label list item with time stamp and icon/spinner
//GUIITEM_STYLE_TWO_LINE_2TEXT_AND_TEXT_ICON_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_2text_and_text_icon_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+2,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_TIME_STAMP_WIDTH_MS - MMI_LIST_TEXT_LEFT_SPACE_MS - 10, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_TIME_STAMP_WIDTH_MS - MMI_LIST_TEXT_LEFT_SPACE_MS -10, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_TIME_STAMP_WIDTH_MS -12, MMI_LIST_ITEM_TOP_MARGIN_MS + 4, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_RIGHT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},     /* 2 */
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_TIME_STAMP_WIDTH_MS -12, MMI_LIST_ITEM_TOP_MARGIN_MS + 4, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_RIGHT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},     /* 2 */
            SONG_FONT_16, SONG_FONT_16,    ////ZDT_PRODUCT_Z685S
            0, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS - MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS - MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            SONG_FONT_26, SONG_FONT_26,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS - 4},     /* 4 */
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS - 4},     /* 4 */
            0, 0,
            0, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|  |--|   text              |
//|  |__|   text              |
//|---------------------------|
//目前未使用
//GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_text_and_text_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_TOP_MARGIN_MS*2, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ICON_BOTTOM_MARGIN_MS*2},       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_TOP_MARGIN_MS*2, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ICON_BOTTOM_MARGIN_MS*2},       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};
//|-----------------------------------------------------------------------------|
//|  ┏┓   text                                                                |
//|  ┗┛   text                                                                |
//|这种风格与GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_MS唯一不同就在于2icon重叠|
//|-----------------------------------------------------------------------------|
//Two label list item with avatar
//GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_MASK_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_bigicon_text_and_text_with_mask_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {   //icon
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS*2, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_TOP_MARGIN_MS*2}, /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS*2, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_TOP_MARGIN_MS*2}, /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        //add for name abbreviation text area
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { MMI_LIST_ICON_LEFT_SPACE_MS + 3, MMI_LIST_ICON_MARGIN_MS, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - (MMI_LIST_ICON_MARGIN_MS+1)}, /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE_MS + 3, MMI_LIST_ICON_MARGIN_MS, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - (MMI_LIST_ICON_MARGIN_MS+1)}, /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 0
        },
        {0},
        {0},
    },

    4, MMILIST_INVALID_ITEM_INDEX
};
#ifdef MMIPB_RMVITEM_SUPPORT
//s_item_style_no_bigicon_text_and_text_with_mask_ms
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_no_bigicon_text_and_text_with_mask_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {   //icon
        {0},
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            {MMI_LIST_ICON_LEFT_SPACE_MS + 5,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_ICON_WIDTH_MS - 6, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            {MMI_LIST_ICON_LEFT_SPACE_MS + 5,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_ICON_WIDTH_MS - 6, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            {MMI_LIST_ICON_LEFT_SPACE_MS + 5, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_ICON_WIDTH_MS - 6, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            {MMI_LIST_ICON_LEFT_SPACE_MS + 5, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_ICON_WIDTH_MS - 6, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        //add for name abbreviation text area
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
#endif
//|---------------------------|
//|  ┏┓   text              | 
//|  ┗┛   text              |      
//|---------------------------|
//Two label list item with avatar –Initials (no photo available)
// GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_MASK_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_initialtext_text_and_text_with_mask_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,
    {
        //text
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_MARGIN_MS*2, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ICON_MARGIN_MS*2},  /*1*/
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_MARGIN_MS*2, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ICON_MARGIN_MS*2}, 
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4}, /*2*/
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4}, /*2*/
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /*3*/
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /*3*/
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};
#ifdef MMIPB_RMVITEM_SUPPORT
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_no_initialtext_text_and_text_with_mask_ms = 
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,
    {
        //text
        {0},
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4}, /*2*/
            { MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4}, /*2*/
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /*3*/
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /*3*/
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};
#endif
//|------------------------------------|
//|  ┏┓   text               [simsel]|
//|  ┗┛   text                       |
//|------------------------------------|
//Two label list item with avatar -- Quick Action for sim selection
//GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_SIMSEL_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_bigicon_text_and_text_with_simsel_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {   //icon
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_MARGIN_MS+1, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - (MMI_LIST_ICON_MARGIN_MS+1)}, /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_MARGIN_MS+1, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - (MMI_LIST_ICON_MARGIN_MS+1)}, /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_SIMSEL_WIDTH_MS+MMI_LIST_ICON_LEFT_SPACE_MS*2), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_SIMSEL_WIDTH_MS+MMI_LIST_ICON_LEFT_SPACE_MS*2), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|  ┏┓   text     [simsel] | 
//|  ┗┛   text              |
//|---------------------------|
//Two label list item with avatar –Initials (no photo available) -- Quick Action for sim selection
// GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_SIMSEL_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_initialtext_text_and_text_with_simsel_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,
    {
        //text
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_MARGIN_MS*2, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ICON_MARGIN_MS*2},  /*1*/
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_MARGIN_MS*2, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ICON_MARGIN_MS*2}, 
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4}, /*2*/
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_SIMSEL_WIDTH_MS+MMI_LIST_ICON_LEFT_SPACE_MS*2), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4}, /*2*/
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /*3*/
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_SIMSEL_WIDTH_MS+MMI_LIST_ICON_LEFT_SPACE_MS*2), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /*3*/
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};
//|------------------------------------|
//| [leftQuick] ┏┓    text           |
//|             ┗┛    text           |
//|------------------------------------|
//Two label list item with avatar -- Quick Action list item
//GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_bigicon_text_and_text_with_left_focus_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        /*icon*/    
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS*2,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS,  MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_TOP_MARGIN_MS*2},  /* icon */
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS*2,  MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS,  MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_TOP_MARGIN_MS*2},  /* icon */
            0, 0,
            0, 0
        },                
        /*text*/
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2+MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2+MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        /*text*/
        {
            0,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2+MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2+MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};
#ifdef MMIPB_RMVITEM_SUPPORT
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_no_bigicon_text_and_text_with_left_focus_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        /*icon*/    
        {0},                
        /*text*/
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            { MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        /*text*/
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};
#endif
//|------------------------------------|
//|  ┏┓   text           [rightQuick]|
//|  ┗┛   text                       |
//|------------------------------------|
//Two label list item with avatar -- Quick Action list item
//GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_RIGHT_FOCUS_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_bigicon_text_and_text_with_right_focus_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {   //icon
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS*2, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_TOP_MARGIN_MS*2}, /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS*2, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_TOP_MARGIN_MS*2}, /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};
#ifdef MMIPB_RMVITEM_SUPPORT
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_no_bigicon_text_and_text_with_right_focus_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {   //icon
        {0},
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS*2), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            { MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS*2), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS*2), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS*2), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};
#endif
//|---------------------------|
//| [leftQuick] ┏┓   text   | 
//|             ┗┛   text   |      
//|---------------------------|
//Two label list item with avatar –Initials (no photo available) -- Quick Action list item
// GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_initialtext_text_and_text_with_left_focus_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,
    {
        //text
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS+MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_MARGIN_MS*2, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ICON_MARGIN_MS*2},  /*1*/
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS+MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_MARGIN_MS*2, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ICON_MARGIN_MS*2}, 
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2+MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4}, /*2*/
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2+MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4}, /*2*/
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2+MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /*3*/
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2+MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /*3*/
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|  ┏┓   text  [rightQuick]|
//|  ┗┛   text              |
//|---------------------------|
//Two label list item with avatar –Initials (no photo available) -- Quick Action list item
// GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_RIGHT_FOCUS_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_initialtext_text_and_text_with_right_focus_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,
    {
        //text
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_MARGIN_MS*2, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ICON_MARGIN_MS*2},  /*1*/
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_MARGIN_MS*2, MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ICON_MARGIN_MS*2}, 
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4}, /*2*/
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4}, /*2*/
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /*3*/
            { MMI_LIST_BIG_ICON_WIDTH_MS + MMI_LIST_ICON_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /*3*/
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};
#ifdef MMIPB_RMVITEM_SUPPORT
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_no_initialtext_text_and_text_with_right_focus_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,
    {
        //text
        {0},
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4}, /*2*/
            { MMI_LIST_ICON_LEFT_SPACE_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4}, /*2*/
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /*3*/
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /*3*/
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};
#endif
//|----------------------------------------------------------------------------|
//|  @     text                                                                |
//|        text                                                                |
//|这种风格与GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_MS唯一不同就在于ICON坐标|
//|----------------------------------------------------------------------------|
//Two label list item with icon
//GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_icon_text_and_text_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        //icon
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_MARGIN_MS, MMI_LIST_ICON_LEFT_SPACE_MS + MMI_LIST_ICON_WIDTH_2_MS, MMI_LIST_ICON_MARGIN_MS + MMI_LIST_ICON_WIDTH_2_MS},       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_MARGIN_MS, MMI_LIST_ICON_LEFT_SPACE_MS + MMI_LIST_ICON_WIDTH_2_MS, MMI_LIST_ICON_MARGIN_MS + MMI_LIST_ICON_WIDTH_2_MS},       /* 1 */
            0, 0,
            0, 0
        },
        //text
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS * 3 + MMI_LIST_ICON_WIDTH_2_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},     /* 2 */
            { MMI_LIST_ICON_LEFT_SPACE_MS * 3 + MMI_LIST_ICON_WIDTH_2_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        //text
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS * 3 + MMI_LIST_ICON_WIDTH_2_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_ICON_LEFT_SPACE_MS * 3 + MMI_LIST_ICON_WIDTH_2_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//| [leftQuick] @ text        |
//| [leftQuick]   text        |
//|---------------------------|
//Two label list item with icon--Quick Action list item
//GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_icon_text_and_text_with_left_focus_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        //icon
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ICON_MARGIN_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS + MMI_LIST_ICON_WIDTH_2_MS, MMI_LIST_ICON_MARGIN_MS + MMI_LIST_ICON_WIDTH_2_MS},       /* 1 */
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ICON_MARGIN_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS + MMI_LIST_ICON_WIDTH_2_MS, MMI_LIST_ICON_MARGIN_MS + MMI_LIST_ICON_WIDTH_2_MS},       /* 1 */
            0, 0,
            0, 0
        },
        //text
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS+MMI_LIST_ICON_WIDTH_2_MS+MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},     /* 2 */
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS+MMI_LIST_ICON_WIDTH_2_MS+MMI_LIST_ICON_LEFT_SPACE_MS*2,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        //text
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS+MMI_LIST_ICON_WIDTH_2_MS+MMI_LIST_ICON_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS+MMI_LIST_ICON_WIDTH_2_MS+MMI_LIST_ICON_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//| @ text      [rightQuick]  |
//|   text      [rightQuick]  |
//|---------------------------|
//Two label list item with icon--Quick Action list item
//GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_WITH_RIGHT_FOCUS_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_icon_text_and_text_with_right_focus_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        //icon
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_MARGIN_MS, MMI_LIST_ICON_LEFT_SPACE_MS + MMI_LIST_ICON_WIDTH_2_MS, MMI_LIST_ICON_MARGIN_MS + MMI_LIST_ICON_WIDTH_2_MS},       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_MARGIN_MS, MMI_LIST_ICON_LEFT_SPACE_MS + MMI_LIST_ICON_WIDTH_2_MS, MMI_LIST_ICON_MARGIN_MS + MMI_LIST_ICON_WIDTH_2_MS},       /* 1 */
            0, 0,
            0, 0
        },
        //text
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS * 3 + MMI_LIST_ICON_WIDTH_2_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},     /* 2 */
            { MMI_LIST_ICON_LEFT_SPACE_MS * 3 + MMI_LIST_ICON_WIDTH_2_MS,  MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        //text
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS * 3 + MMI_LIST_ICON_WIDTH_2_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_ICON_LEFT_SPACE_MS * 3 + MMI_LIST_ICON_WIDTH_2_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|      text                 |
//|      text          text   |
//|---------------------------|
//Two label list item with size stamp
//GUIITEM_STYLE_TWO_LINE_TEXT_AND_2TEXT_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_and_2text_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS-(MMI_LITT_SIZE_STAMP_WIDTH_MS+9*MMI_LIST_TEXT_LEFT_SPACE_MS+4), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS-(MMI_LITT_SIZE_STAMP_WIDTH_MS+9*MMI_LIST_TEXT_LEFT_SPACE_MS+4), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT_MS-9*MMI_LITT_SIZE_STAMP_WIDTH_MS-4, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_ITEM_RIGHT_MS-9*MMI_LITT_SIZE_STAMP_WIDTH_MS-4, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//add list style for dataCounter
//GUIITEM_STYLE_TWO_LINE_TEXT_AND_2TEXT_MS_EXT
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_and_2text_ms_ext =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,
    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LITT_SIZE_STAMP_WIDTH_MS+MMI_LIST_TEXT_LEFT_SPACE_MS+30, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LITT_SIZE_STAMP_WIDTH_MS+MMI_LIST_TEXT_LEFT_SPACE_MS+30, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LITT_SIZE_STAMP_WIDTH_MS+MMI_LIST_TEXT_LEFT_SPACE_MS+40, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LITT_SIZE_STAMP_WIDTH_MS+MMI_LIST_TEXT_LEFT_SPACE_MS+40, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//End
//|---------------------------|
//|  @   text                 |
//|      text          text   |
//|---------------------------|
//Two label list item with size stamp and thumbnail,
//GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_2TEXT_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_bigicon_text_and_2text_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_TEXT_LEFT_SPACE_MS+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS-MMI_LIST_ITEM_BOTTOM_MARGIN_MS},    /* 0 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_TEXT_LEFT_SPACE_MS+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS-MMI_LIST_ITEM_BOTTOM_MARGIN_MS},    /* 0 */
            0, 0,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TEXT_LEFT_SPACE_MS*2, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2+2},    /* 1 */
            { MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TEXT_LEFT_SPACE_MS*2, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2+2},    /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },        
        {
            0,
            { MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TEXT_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS-(MMI_LITT_SIZE_STAMP_WIDTH_MS+MMI_LIST_TEXT_LEFT_SPACE_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            { MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TEXT_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS-(MMI_LITT_SIZE_STAMP_WIDTH_MS+MMI_LIST_TEXT_LEFT_SPACE_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT_MS-MMI_LITT_SIZE_STAMP_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_ITEM_RIGHT_MS-MMI_LITT_SIZE_STAMP_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//| [leftQuick]  text         |
//| [leftQuick]  text         |
//|---------------------------|
//Two label list item with size stamp--Quick Action list item
//GUIITEM_STYLE_TWO_LINE_TEXT_AND_2TEXT_WITH_LEFT_FOCUS_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_text_and_2text_with_left_focus_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        /*1*/
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        /*2*/
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        /*3*/
        {0},
        {0},
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};
//End

//|---------------------------|
//| [leftquick] @   text      |
//|                 text      |
//|---------------------------|
//Two label list item with size stamp and thumbnail -- Quick Action list item
//与GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS不同在于Bigicon的坐标不同
//GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_2_WITH_LEFT_FOCUS_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_bigicon_text_and_text_2_with_left_focus_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {   
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ICON_MARGIN_MS,  (MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS)+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS-MMI_LIST_ITEM_BOTTOM_MARGIN_MS},    /* 0 */
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ICON_MARGIN_MS,  (MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS)+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS-MMI_LIST_ITEM_BOTTOM_MARGIN_MS},    /* 0 */
            0, 0,
            0, 100
        },
        {
            0,
            { (MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS)+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2+2},    /* 1 */
            { (MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS)+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2+2},    /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },        
        {
            0,
            { (MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS)+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            { (MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS)+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|  @   text     [leftquick] |
//|      text                 |
//|---------------------------|
//Two label list item with size stamp and thumbnail -- Quick Action list item
//与GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_RIGHT_FOCUS_MS不同在于Bigicon的坐标不同
//GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_2_WITH_RIGHT_FOCUS_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_bigicon_text_and_text_2_with_right_focus_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {   
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_MARGIN_MS,  MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS-MMI_LIST_ITEM_BOTTOM_MARGIN_MS},    /* 0 */
            { MMI_LIST_ICON_LEFT_SPACE_MS, MMI_LIST_ICON_MARGIN_MS,  MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS-MMI_LIST_ITEM_BOTTOM_MARGIN_MS},    /* 0 */
            0, 0,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TEXT_LEFT_SPACE_MS*2, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2+2},    /* 1 */
            { MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TEXT_LEFT_SPACE_MS*2, MMI_LIST_ITEM_TOP_MARGIN_MS,  MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2+2},    /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },        
        {
            0,
            { MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TEXT_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            { MMI_LIST_ITEM_HEIGHT_BIG_ICON_MS+MMI_LIST_TEXT_LEFT_SPACE_MS*2, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS-(MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS), MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};

//[toggle and quick action list]
//|---------------------------|
//|        text           @   |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_TOGGLE_TWO_LINE_TEXT_TEXT_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_toggle_2_text_text_ms =
{
    MMI_TOGGLE_LIST_ITEM_HEIGHT, MMI_TOGGLE_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ITEM_LEFT_MARGIN, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (MMI_TOGGLE_ICON_WIDTH + MMI_LIST_ITEM_LEFT_MARGIN), MMI_TOGGLE_LIST_ITEM_HEIGHT / 2 + 2 },/*0*/
            { MMI_LIST_ITEM_LEFT_MARGIN, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (MMI_TOGGLE_ICON_WIDTH + MMI_LIST_ITEM_LEFT_MARGIN), MMI_TOGGLE_LIST_ITEM_HEIGHT / 2 + 2 },/*0*/
            MMI_DEFAULT_BIG_FONT, MMI_DEFAULT_BIG_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_LEFT_MARGIN, MMI_TOGGLE_LIST_ITEM_HEIGHT / 2 + 2,  MMI_LIST_ITEM_RIGHT - (MMI_TOGGLE_ICON_WIDTH + MMI_LIST_ITEM_LEFT_MARGIN), MMI_TOGGLE_LIST_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },/*1*/
            { MMI_LIST_ITEM_LEFT_MARGIN, MMI_TOGGLE_LIST_ITEM_HEIGHT / 2 + 2,  MMI_LIST_ITEM_RIGHT - (MMI_TOGGLE_ICON_WIDTH + MMI_LIST_ITEM_LEFT_MARGIN), MMI_TOGGLE_LIST_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },/*1*/
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//GUIITEM_STYLE_TOGGLE_TWO_LINE_TEXT_TEXT_WITH_LEFT_FOCUS_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_toggle_2_text_text_with_left_focus_ms =
{
    MMI_TOGGLE_LIST_ITEM_HEIGHT, MMI_TOGGLE_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (MMI_TOGGLE_ICON_WIDTH + MMI_LIST_ITEM_LEFT_MARGIN), MMI_TOGGLE_LIST_ITEM_HEIGHT / 2 + 2 },/*0*/
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (MMI_TOGGLE_ICON_WIDTH + MMI_LIST_ITEM_LEFT_MARGIN), MMI_TOGGLE_LIST_ITEM_HEIGHT / 2 + 2 },/*0*/
            MMI_DEFAULT_BIG_FONT, MMI_DEFAULT_BIG_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_TOGGLE_LIST_ITEM_HEIGHT / 2 + 2,  MMI_LIST_ITEM_RIGHT - (MMI_TOGGLE_ICON_WIDTH + MMI_LIST_ITEM_LEFT_MARGIN), MMI_TOGGLE_LIST_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },/*1*/
            { MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS+MMI_LIST_ICON_LEFT_SPACE_MS+MMI_LIST_TRIANGLE_WIDTH_MS, MMI_TOGGLE_LIST_ITEM_HEIGHT / 2 + 2,  MMI_LIST_ITEM_RIGHT - (MMI_TOGGLE_ICON_WIDTH + MMI_LIST_ITEM_LEFT_MARGIN), MMI_TOGGLE_LIST_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },/*1*/
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//[end]

//[indicate sim card in call log]
//|---------------------------|
//|        text            @   |
//|        text                 |
//|---------------------------|
//Two label list item with icon
//GUIITEM_STYLE_TWO_LINE_TEXT_ICON_AND_TEXT_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_icon_and_text_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS - MMI_LIST_RIGHT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS - MMI_LIST_RIGHT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            /*{ MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS, MMI_LIST_ICON_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ICON_MARGIN_MS + MMI_LIST_ICON_WIDTH_2_MS},     /* 2 *
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS, MMI_LIST_ICON_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ICON_MARGIN_MS + MMI_LIST_ICON_WIDTH_2_MS},     /* 2 */
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS, MMI_LIST_ICON_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ICON_MARGIN_MS + MMI_LIST_SMALL_ICON_WIDTH_MS},     /* 2 */
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS, MMI_LIST_ICON_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ICON_MARGIN_MS + MMI_LIST_SMALL_ICON_WIDTH_MS},     /* 2 */
            0, 0,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//[end]

//|---------------------------|
//|         text         ┏┓ |
//|---------------------------|//|         text         ┏┓ |
//|         text         ┗┛ |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM_MS
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_and_text_anim_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,
    {
        //text
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS-24, MMI_LIST_ITEM2_HEIGHT_MS+2},  /*1*/
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS-24, MMI_LIST_ITEM2_HEIGHT_MS+2},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 0
        },
        {
			GUIITEM_CONTENT_STATE_EFFECT_STR,
			{ MMI_LIST_TEXT_LEFT_SPACE_MS,  MMI_LIST_ITEM2_HEIGHT_MS+2,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS-6}, /*2*/
			{ MMI_LIST_TEXT_LEFT_SPACE_MS,  MMI_LIST_ITEM2_HEIGHT_MS+2,  MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS-6}, /*2*/
			MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
			0, 0
		},
		{
			0,
			{ MMI_LIST_ITEM_RIGHT_MS-18, MMI_LIST_ITEM_TOP_MARGIN_MS*2-1, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_TEXT_LEFT_BIG_SPACE_MS - 1},     /*3*/
			{ MMI_LIST_ITEM_RIGHT_MS-18, MMI_LIST_ITEM_TOP_MARGIN_MS*2-1, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_TEXT_LEFT_BIG_SPACE_MS - 1},     /*3*/
			0, 0,
			0, 0
		},
		{0},
		{0},
		{0},
    },	
    2, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|      text         text   @|
//|      text                @|
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_2TEXT_ICON_AND_TEXT_ICON_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_2text_icon_and_text_icon_ms =
{
    MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,
		
    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_TIME_STAMP_WIDTH_MS - 2*MMI_LIST_TEXT_LEFT_SPACE_MS-MMI_LIST_SMALL_ICON_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_TIME_STAMP_WIDTH_MS - 2*MMI_LIST_TEXT_LEFT_SPACE_MS-MMI_LIST_SMALL_ICON_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},    /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_TIME_STAMP_WIDTH_MS-MMI_LIST_TEXT_LEFT_SPACE_MS-MMI_LIST_SMALL_ICON_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS + 4, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS - MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},     /* 2 */
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_TIME_STAMP_WIDTH_MS-MMI_LIST_TEXT_LEFT_SPACE_MS-MMI_LIST_SMALL_ICON_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS + 4, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS - MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2},     /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 35
        },
        {
        0,
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS - MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + 2, MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS - MMI_LIST_TEXT_LEFT_SPACE_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS + MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_TOP_MARGIN_MS + MMI_LIST_ITEM_TOP_MARGIN_MS+MMI_LIST_SMALL_ICON_WIDTH_MS},     /* 4 */
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS, MMI_LIST_ITEM_TOP_MARGIN_MS + MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_ITEM_TOP_MARGIN_MS + MMI_LIST_ITEM_TOP_MARGIN_MS+MMI_LIST_SMALL_ICON_WIDTH_MS},     /* 4 */
            0, 0,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS - 4},     /* 4 */
            { MMI_LIST_ITEM_RIGHT_MS - MMI_LIST_SMALL_ICON_WIDTH_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS/2 + MMI_LIST_ITEM_TOP_MARGIN_MS, MMI_LIST_ITEM_RIGHT_MS, MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_LIST_ITEM_BOTTOM_MARGIN_MS - 4},     /* 4 */
            0, 0,
            0, 100
        },
        {0},
    },
	
    0, MMILIST_INVALID_ITEM_INDEX
};
/*Microsoft project need to support---*/
///////////////////////END///////////////////////



typedef struct
{
    GUIITEM_STYLE_E id;
    THEMELIST_ITEM_STYLE_T const *style;
} THEMELIST_STYLE_TABLE_T;


LOCAL THEMELIST_STYLE_TABLE_T const s_style_table[] =
{
    {GUIITEM_STYLE_DEFAULT, &s_item_style_1_text},
    {GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT, &s_item_style_split},
    {GUIITEM_STYLE_ONE_LINE_TEXT_ICON, &s_item_style_1_text_icon},
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_HALF_LINE_TEXT_ICON, &s_item_style_1_text_icon_2},
#endif
    {GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON, &s_item_style_1_icon_text_icon},
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON_EXT, &s_item_style_1_icon_text_icon_ext},
#endif
    {
        GUIITEM_STYLE_ONE_LINE_TEXT,
#if defined MAINLCD_SIZE_128X160 || defined MAINLCD_SIZE_176X220
        &s_item_style_1_text
#else
        &s_item_style_1_text
#endif
    },
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_ONE_LINE_TEXT_DEFAULT_FONT, &s_item_style_1_text_2},
#endif


    {GUIITEM_STYLE_ONE_LINE_ICON_TEXT, &s_item_style_1_icon_text},
    {GUIITEM_STYLE_ONE_LINE_ICON_TEXT_WITHOUT_EFFECT_STR, &s_item_style_1_icon_text_without_effect_str},

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_ONE_LINE_TWO_ICON_TEXT, &s_item_style_1_icon_icon_text },

    {GUIITEM_STYLE_ONE_LINE_NUMBER_TEXT, &s_item_style_1_text},
#endif
    {GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT, &s_item_style_1_icon_text_text},

    {GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT,  &s_item_style_1_icon_text_text_2 },

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {
        GUIITEM_STYLE_ONE_LINE_TWO_ICON_TWO_TEXT,
#ifdef GUIF_LIST_PDASTYLE
#ifdef MAINLCD_SIZE_240X320
        &s_item_style_1_icon_icon_text_text
#else
        &s_item_style_1_icon_icon_text_text
#endif // MAINLCD_SIZE_240X320
#else
        &s_item_style_1_icon_icon_text_text
#endif // GUIF_LIST_PDASTYLE
    },

    {GUIITEM_STYLE_ONE_LINE_BUTTON_TEXT, &s_item_style_1_buttontext },

    {GUIITEM_STYLE_ONE_LINE_TWO_BUTTON_TEXT, &s_item_style_1_2buttontext},
#endif // MMI_THEME_LIST_MINI_SUPPORT

//zmt add start
    {GUIITEM_SYTLE_ZMT_PUBLISH_LIST_MS, &zmt_publish_list_item},
    {GUIITEM_SYTLE_ZMT_BOOK_LIST_MS, &zmt_book_list_item},
    {GUIITEM_SYTLE_ZMT_UNIT_LIST_MS, &zmt_unit_list_item},
    {GUIITEM_SYTLE_ZMT_LISTEN_SET_MS, &zmt_listen_set_list_item},
    {GUIITEM_STYLE_POETRY_GRADE_LIST_MS, &s_item_style_poetry_grade_list_ms},
    {GUIITEM_STYLE_POETRY_ITEM_LIST_MS, &s_item_style_poetry_item_list_ms},
    {GUIITEM_STYLE_FORMULA_ITEM_LIST_MS, &s_item_style_formula_item_list_ms},
    {GUIITEM_STYLE_CLASS_MAIN_LIST_MS, &s_item_style_class_main_list_ms},
    {GUIITEM_STYLE_CLASS_SECTION_LIST_MS, &s_item_style_class_section_list_ms},
    {GUIITEM_STYLE_PINYIN_TABLE_LIST_MS, &s_item_style_pinyin_table_list_ms},
//zmt add end

    {
        GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT,
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
        &s_item_style_1_small_text_text
#else
        &s_item_style_1_small_text_text
#endif
    },
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_TEXT_ICON, &s_item_style_1_small_text_text_icon},
    {GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_TEXT_TWO_ICON, &s_item_style_1_small_2text_2icon },
#endif

    {GUIITEM_STYLE_ONE_LINE_CHECK, &s_item_style_1_text},

    {GUIITEM_STYLE_ONE_LINE_CHECK_EXT, &s_item_style_1_text_check_ext},

    {GUIITEM_STYLE_ONE_LINE_RADIO, &s_item_style_1_text},
    {GUIITEM_STYLE_ONE_LINE_RADIO_EXT, &s_item_style_1_text_check_ext},
    {GUIITEM_STYLE_ONE_LINE_CHECK_ICON, &s_item_style_1_check_icon },
    {GUIITEM_STYLE_ONE_LINE_CHECK_ICON_MS, &s_item_style_1_check_icon_ms},

    {GUIITEM_STYLE_ONE_LINE_RADIO_ICON, &s_item_style_1_check_icon},

	{GUIITEM_STYLE_MULTI_CHOICE_PICKER_TEXT, &s_item_style_multi_choice_picker_text},
#ifndef MMIPB_RMVITEM_SUPPORT
	{GUIITEM_STYLE_MULTI_CHOICE_PICKER_AVATAR_AND_TEXT, &s_item_style_multi_choice_picker_avatar_and_text },
#else
    {GUIITEM_STYLE_MULTI_CHOICE_PICKER_AVATAR_AND_TEXT, &s_item_style_multi_choice_picker_noavatar_and_text },
#endif
	{GUIITEM_STYLE_MULTI_CHOICE_PICKER_INITIAL_AND_TEXT, &s_item_style_multi_choice_picker_initial_and_text },
	{GUIITEM_STYLE_MULTI_CHOICE_PICKER_TWO_LINE_TEXT_AND_TEXT, &s_item_style_multi_choice_picker_two_line_text_and_text },	
    {GUIITEM_STYLE_MULTI_CHOICE_PICKER_TWO_LINE_TEXT_AND_TEXT_2, &s_item_style_multi_choice_picker_two_line_text_and_text},
    {GUIITEM_STYLE_MULTI_CHOICE_PICKER_TWO_LINE_BIGICON_TEXT_AND_TEXT, &s_item_style_multi_choice_picker_two_line_bigicon_text_and_text},
    {GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT, &s_item_style_2_text_text },

    {GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_AND_TEXT_AND_TEXT, &s_item_style_2_2text_and_2text },

    {GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT, &s_item_style_2_icon_text_and_text },

    {GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT, &s_item_style_toggle_1_text},

    {GUIITEM_STYLE_TOGGLE_TWO_LINE_TEXT_TEXT, &s_item_style_toggle_2_text_text},

    {GUIITEM_STYLE_RADIO_TWO_LINE_TEXT_TEXT, &s_item_style_radio_2_text_text},
    
    {GUIITEM_STYLE_RADIO_ONE_LINE_TEXT, &s_item_style_radio_1_text},
    
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT, &s_item_style_2_2icon_text_and_text },
    {GUIITEM_STYLE_TWO_LINE_NUMBER_TEXT_AND_TEXT, &s_item_style_2_text_text},
    {GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT, &s_item_style_2_icon_text_and_2text },
    {GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT_SMALL, &s_item_style_2_icon_text_and_2text_small },
#endif

    {GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT, &s_item_style_2_2icon_text_and_2text },
    {GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT, &s_item_style_2_icon_2text_and_text },
    {GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_SMALL, &s_item_style_2_icon_2text_and_text_small },

    {GUIITEM_STYLE_TWO_LINE_TWO_ICON_TWO_TEXT_AND_TEXT, &s_item_style_2_2icon_2text_and_text },

    {GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT, &s_item_style_2_anim_text_and_text },
    {GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2, &s_item_style_2_anim_text_and_text_2 },


#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ICON, &s_item_style_2_anim_text_icon_and_text },



    {GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_3ICON, &s_item_style_2_anim_text_3icon_and_text },

    {GUIITEM_STYLE_TWO_LINE_RADIO, &s_item_style_2_text_text},
#endif

    {GUIITEM_STYLE_TWO_LINE_CHECK_ICON, &s_item_style_2_text_icon_and_text},

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_ANIM_2TEXT_AND_ICON, &s_item_style_2_anim_text_icon_and_text},
#endif

    {GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM, &s_item_style_2_text_and_text_anim },
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ANIM, &s_item_style_2_anim_text_and_text_anim },
#endif

    {GUIITEM_STYLE_TWO_LINE_TEXT_2ANIM_AND_TEXT, &s_item_style_2_text_2anim_and_text },
    {GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TEXT, &s_item_style_1_2_icon_text_and_text },

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT, &s_item_style_1_2_2icon_text_and_text },

    {GUIITEM_STYLE_ONE_TWO_LINE_NUMBER_TEXT_AND_TEXT, &s_item_style_1_2_text_and_text },
    {GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT, &s_item_style_1_2_icon_text_and_2text},
    {GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT, &s_item_style_1_2_2icon_text_and_2text },

    {GUIITEM_STYLE_ONE_TWO_ANIM_TEXT_AND_TEXT_3ICON, &s_item_style_1_2_anim_text_and_text_3icon },

#endif
    {GUIITEM_STYLE_THREE_LINE_TEXT, &s_item_style_3_text },
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_THREE_LINE_TEXT_ICON, &s_item_style_3_text_and_text_and_text_icon },
#endif

    {GUIITEM_STYLE_TWO_LINE_TEXT_ICON_AND_TWO_TEXT_SMALL, &s_item_style_2_text_icon_and_two_text_small },
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_ICON, &s_item_style_2_anim_2text_and_icon_text },

    {GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT, &s_item_style_2_anim_2text_and_text },

    {GUIITEM_STYLE_TWO_LINE_ANIM_ICON_TEXT_AND_ICON_2TEXT, &s_item_style_2_anim_icon_text_and_icon_2text },
#endif

    {GUIITEM_STYLE_TWO_LINE_ANIM_TEXT, &s_item_style_2_anim_text },
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_CHECK_ANIM_TEXT, &s_item_style_2_anim_text_2 },

    {GUIITEM_STYLE_TWO_LINE_RADIO_ANIM_TEXT, &s_item_style_2_anim_text_2},
#endif
    {GUIITEM_STYLE_TWO_LINE_CHECK_ANIM_TEXT_AND_TEXT, &s_item_style_2_check_anim_text_and_text },

    {GUIITEM_STYLE_TWO_LINE_RADIO_ANIM_TEXT_AND_TEXT, &s_item_style_2_check_anim_text_and_text},

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_2TEXT_2ANIM_AND_TEXT, &s_item_style_2_2text_2anim_and_text },

    {GUIITEM_STYLE_TWO_LINE_ANIM_ICON_TWO_TEXT_AND_ICON_TEXT, &s_item_style_2_anim_icon_two_text_and_icon_text },

#ifdef MMI_PDA_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_ICON, &s_item_style_2_2text_1icon_and_text_icon },
    {GUIITEM_STYLE_ONE_LINE_TEXT_TEXT_ICON, &s_item_style_1_text_text_icon },

    {GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT_ICON, &s_item_style_1_icon_text_text_icon },

#endif

    {GUIITEM_STYLE_ONE_LINE_LEFT_SPACE_TEXT, &s_item_style_1_left_space_text },
#endif

    {GUIITEM_STYLE_TWO_LINE_TEXT_ANIM_AND_TEXT, &s_item_style_2_text_anim_and_text },

#ifndef MMI_THEME_LIST_MINI_SUPPORT
#ifdef QBTHEME_SUPPORT
    {GUIITEM_STYLE_THREE_LINE_TEXT_TEXT_ANIM, &s_item_style_3_text_text_anim },

    {GUIITEM_STYLE_ONE_LINE_ANIM_TEXT_ICON, &s_item_style_1_anim_text_icon },

#endif
#endif
    {GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2, &s_item_style_2_anim_and_two_icon_text_and_icon_2 },

    {GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_NORMAL, &s_item_style_2_icon_two_text_and_text_normal },
#ifndef MMI_THEME_LIST_MINI_SUPPORT
#ifdef SEARCH_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT, &s_item_style_2_bigicon_text },
#endif

    {GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_2, &s_item_style_2_anim_two_text_and_text_2 },
    {GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_3_SCROLL, &s_item_style_2_anim_two_text_and_text_3_scroll },
    #if defined(ZDT_PHILIPS_DIAL_SEARCH_STYLE) //   A20210129
    {GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_3_SCROLL_NEW, &s_item_style_2_anim_two_text_and_text_3_scroll_new },
    #endif
#endif
    //add for pb_avatar_text_item_quick_action
#ifndef MMIPB_RMVITEM_SUPPORT
    {GUIITEM_STYLE_PB_ONE_LINE_AVATAR_TEXT_LEFT_FOCUS_MS, &s_item_style_pb_one_line_avatar_text_left_focus_ms},
#else
    {GUIITEM_STYLE_PB_ONE_LINE_AVATAR_TEXT_LEFT_FOCUS_MS, &s_item_style_pb_one_line_noavatar_text_left_focus_ms},
#endif
    /*Microsoft project need to support+++*/
    {GUIITEM_STYLE_ONE_LINE_TEXT_MS, &s_item_style_1_text_ms},
    {GUIITEM_STYLE_ONE_LINE_TEXT_WITH_LEFT_FOCUS_MS, &s_item_style_1_text_with_left_focus_ms},
    {GUIITEM_STYLE_ONE_LINE_TEXT_WITH_RIGHT_FOCUS_MS, &s_item_style_1_text_with_right_focus_ms},
	{GUIITEM_STYLE_ONE_LINE_ICON_TEXT_MS, &s_item_style_1_icon_text_ms},
	{GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_MS, &s_item_style_1_bigicon_text_ms},
#ifndef MMIPB_RMVITEM_SUPPORT
    {GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_MASK_MS,&s_item_style_1_bigicon_text_with_mask_ms},
#else
    {GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_MASK_MS,&s_item_style_no_bigicon_text_with_mask_ms},
#endif
    {GUIITEM_STYLE_ONE_LINE_INITIALTEXT_TEXT_WITH_MASK_MS, &s_item_style_1_initialtext_text_with_mask_ms},
    {GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_SIMSEL_MS, &s_item_style_1_bigicon_text_with_simsel_ms},
    {GUIITEM_STYLE_ONE_LINE_INITIALTEXT_TEXT_WITH_SIMSEL_MS, &s_item_style_1_initialtext_text_with_simsel_ms},
    {GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_LEFT_FOCUS_MS, &s_item_style_1_bigicon_text_with_left_focus_ms},
    {GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_RIGHT_FOCUS_MS, &s_item_style_1_bigicon_text_with_right_focus_ms},
    {GUIITEM_STYLE_ONE_LINE_INITIALTEXT_TEXT_WITH_LEFT_FOCUS_MS, &s_item_style_1_initialtext_text_with_left_focus_ms},
    {GUIITEM_STYLE_ONE_LINE_INITIALTEXT_TEXT_WITH_RIGHT_FOCUS_MS, &s_item_style_1_initialtext_text_with_right_focus_ms},
	{GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT_MS, &s_item_style_1_icon_text_and_text_ms},
	{GUIITEM_STYLE_ONE_LINE_TEXT_AND_TEXT_MS, &s_item_style_1_text_and_text_ms},
        {GUIITEM_STYLE_ONE_LINE_TEXT_AND_TEXT_MS_EXT, &s_item_style_1_text_and_text_ms_ext},
//added by du.yuxuan
	{GUIITEM_STYLE_ONE_LINE_2TEXT_MS, &s_item_style_1_extras_2text_ms},
//end
	{GUIITEM_STYLE_ONE_LINE_SUB_TEXT_MS, &s_item_style_1_sub_text_ms},
	{GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS, &s_item_style_2_text_text},
    {GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_WITH_SIMSEL_MS, &s_item_style_2_text_text_with_simsel_ms},
	{GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_2_MS, &s_item_style_2_text_text_2_ms},
    //add by James li begin
	{GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_2_MS_FOR_ALARM, &s_item_style_2_text_text_2_ms_for_alarm},
    //add by James li end
    {GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_2_WITH_SIMSEL_MS, &s_item_style_2_text_text_2_with_simsel_ms},
	{GUIITEM_STYLE_TWO_LINE_2TEXT_AND_TEXT_MS, &s_item_style_2_2text_and_text_ms},
    {GUIITEM_STYLE_THREE_PARTS_2TEXT_AND_TEXT_MS, &s_item_style_3parts_2text_and_text_ms},
    {GUIITEM_STYLE_2TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS, &s_item_style_2text_and_text_with_left_focus_ms},
    {GUIITEM_STYLE_TWO_LINE_TEXT_AND_2TEXT_WITH_LEFT_FOCUS_MS,&s_item_style_text_and_2text_with_left_focus_ms},
    {GUIITEM_STYLE_2TEXT_AND_TEXT_WITH_RIGHT_FOCUS_MS, &s_item_style_2text_and_text_with_right_focus_ms},
	{GUIITEM_STYLE_TWO_LINE_2TEXT_AND_TEXT_ICON_MS, &s_item_style_2_2text_and_text_icon_ms},
	{GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_MS, &s_item_style_2_anim_text_and_text},	
#ifndef MMIPB_RMVITEM_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_MASK_MS, &s_item_style_2_bigicon_text_and_text_with_mask_ms},
#else
    {GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_MASK_MS, &s_item_style_no_bigicon_text_and_text_with_mask_ms},
#endif
    {GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_MASK_MS, &s_item_style_2_initialtext_text_and_text_with_mask_ms},
    {GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_SIMSEL_MS, &s_item_style_2_bigicon_text_and_text_with_simsel_ms},
    {GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_SIMSEL_MS, &s_item_style_2_initialtext_text_and_text_with_simsel_ms},
    {GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS, &s_item_style_2_bigicon_text_and_text_with_left_focus_ms},
    {GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_RIGHT_FOCUS_MS, &s_item_style_2_bigicon_text_and_text_with_right_focus_ms},
    {GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS, &s_item_style_2_initialtext_text_and_text_with_left_focus_ms},
    {GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_RIGHT_FOCUS_MS, &s_item_style_2_initialtext_text_and_text_with_right_focus_ms},
	{GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_MS, &s_item_style_2_icon_text_and_text},
    {GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS, &s_item_style_2_icon_text_and_text_with_left_focus_ms},
    {GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_WITH_RIGHT_FOCUS_MS, &s_item_style_2_icon_text_and_text_with_right_focus_ms},
    {GUIITEM_STYLE_TWO_LINE_TEXT_AND_2TEXT_MS, &s_item_style_2_text_and_2text_ms},
    {GUIITEM_STYLE_TWO_LINE_TEXT_AND_2TEXT_MS_EXT,&s_item_style_2_text_and_2text_ms_ext},
    {GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_2TEXT_MS, &s_item_style_2_bigicon_text_and_2text_ms},
    {GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_2_WITH_LEFT_FOCUS_MS, &s_item_style_2_bigicon_text_and_text_2_with_left_focus_ms},
    {GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_2_WITH_RIGHT_FOCUS_MS, &s_item_style_2_bigicon_text_and_text_2_with_right_focus_ms},
#ifndef MMIPB_RMVITEM_SUPPORT
	{GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_2_MASK_MS, &s_item_style_2_bigicon_text_and_text_with_mask_ms},
    {GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_2_MASK_MS,&s_item_style_2_initialtext_text_and_text_with_mask_ms},
    {GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_2_MASK_RIGHT_FOCUS_MS,&s_item_style_2_bigicon_text_and_text_with_right_focus_ms},
    {GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_2_MASK_LEFT_FOCUS_MS,&s_item_style_2_bigicon_text_and_text_with_left_focus_ms},
    {GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_2_MASK_RIGHT_FOCUS_MS,&s_item_style_2_initialtext_text_and_text_with_right_focus_ms},
#else
	{GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_2_MASK_MS, &s_item_style_no_bigicon_text_and_text_with_mask_ms},
    {GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_2_MASK_MS,&s_item_style_no_initialtext_text_and_text_with_mask_ms},
    {GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_2_MASK_RIGHT_FOCUS_MS,&s_item_style_no_bigicon_text_and_text_with_right_focus_ms},
    {GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_2_MASK_LEFT_FOCUS_MS,&s_item_style_no_bigicon_text_and_text_with_left_focus_ms},
    {GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_2_MASK_RIGHT_FOCUS_MS,&s_item_style_no_initialtext_text_and_text_with_right_focus_ms},
#endif
	{GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_2_MASK_LEFT_FOCUS_MS,&s_item_style_2_initialtext_text_and_text_with_left_focus_ms},
    //[toggle and quick action list]
    {GUIITEM_STYLE_TOGGLE_TWO_LINE_TEXT_TEXT_MS, &s_item_style_toggle_2_text_text},
    {GUIITEM_STYLE_TOGGLE_TWO_LINE_TEXT_TEXT_WITH_LEFT_FOCUS_MS, &s_item_style_toggle_2_text_text_with_left_focus_ms},
    //[end]
    //[indicate sim card in call log]
    {GUIITEM_STYLE_TWO_LINE_TEXT_ICON_AND_TEXT_MS, &s_item_style_2_text_icon_and_text_ms},
    
    //[end]
	{GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM_MS,&s_item_style_2_text_and_text_anim},
	{GUIITEM_STYLE_TWO_LINE_2TEXT_ICON_AND_TEXT_ICON_MS,&s_item_style_2_2text_icon_and_text_icon_ms},

};


//0 default, 1 focus
const uint8 s_item_theme_font_table[GUIITEM_THEME_MAX][2] =
{
    /* GUIITEM_THEME_DEFAULT */
    { MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT },
    /* GUIITEM_THEME_NORMAL */
    { MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT },
    /* GUIITEM_THEME_EXPAND */
    { MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT },
    /* GUIITEM_THEME_SMALL */
    { MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT },
    /* GUIITEM_THEME_BIG */
    { MMI_LIST_BIG_ITEM_FONT, MMI_LIST_BIG_ITEM_FONT },
};

// 存放特殊颜色值
const GUI_COLOR_T s_item_content_color[MMITHEME_COLOR_MAX] =
{
    0x0000,
    /*lint -e572*//*lint -e778*/
    GUI_RGB2RGB565(240, 21, 0),
    GUI_RGB2RGB565(255, 0, 151),
    GUI_RGB2RGB565(221, 107, 176),
    GUI_RGB2RGB565(51, 153, 51),
    GUI_RGB2RGB565(144, 64, 0),
    GUI_RGB2RGB565(162, 0, 255),
    GUI_RGB2RGB565(0, 119, 244),
    GUI_RGB2RGB565(1, 172, 170),
    GUI_RGB2RGB565(144, 144, 144),
    GUI_RGB2RGB565(40, 107, 197),
    //zmt add start
    GUI_RGB2RGB565(80, 162, 254),
    GUI_RGB2RGB565(255, 255, 255),
    //zmt add end
    /*lint -e572*//*lint -e778*/
};


/*****************************************************************************/
//  Description : get the item style state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMITHEME_GetItemContentColor(
    MMITHEME_COLOR_ID_E color_id
)
{
    SCI_ASSERT(color_id < MMITHEME_COLOR_MAX); /*assert verified*/

    return s_item_content_color[color_id];
}

/*****************************************************************************/
//  Description : get the item style
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC const GUIITEM_STYLE_T *MMITHEME_GetItemStyle(GUIITEM_STYLE_E style)
{
    return PNULL;
}

/*****************************************************************************/
//  Description : get content text color
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T THEMELIST_GetContentTextColor(
    GUIITEM_THEME_STATE_E state,
    const THEMELIST_ITEM_STYLE_T *item_style_ptr,
    uint16 content_index
)
{
    GUI_COLOR_T color = 0;

    //invalid
    if (GUIITEM_THEME_STATE_INVALID == state
        || GUIITEM_THEME_STATE_FOCUS_INVALID == state)
    {
        color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM2);
    }
    else if (GUIITEM_THEME_STATE_FOCUS == state)
    {
#ifdef MONOCHROME_LCD_SUPPORT
        color = MMI_WHITE_COLOR;//CR221441
#else
        color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM1);
#endif
    }
    else
    {
#ifdef MONOCHROME_LCD_SUPPORT
        color =  MMI_BLACK_COLOR;//CR221441
#else
        color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0);
#endif
    }

#ifdef GUIF_LIST_PDASTYLE // pda双行支持不同色彩

    // 这里暂时这么修改，取字体颜色太复杂！
    if (PNULL != item_style_ptr && MMI_LIST_ITEM_SMALL_FONT == item_style_ptr->content[content_index].font)
    {
        color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM_SMALL_FONT);
    }

#endif

    return color;
}


/*****************************************************************************/
//  Description : get the item layout data
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC const GUIITEM_LAYOUT_DATA_T *MMITHEME_GetItemLayoutData(GUIITEM_LAYOUT_ID_E layout_id, GUIITEM_THEME_STATE_E state)
{
    return PNULL;
}


/*****************************************************************************/
//  Description : get the list style, is called by the list control
//  Global resource dependence :
//  Author:Lin.Lin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetListStyle(
    MMITHEME_LIST_STYLE_T *list_style_ptr
)
{
    const MMI_IMAGE_ID_T num_icon_id[] =
    {
        IMAGE_NUMBER_1,
        IMAGE_NUMBER_2,
        IMAGE_NUMBER_3,
        IMAGE_NUMBER_4,
        IMAGE_NUMBER_5,
        IMAGE_NUMBER_6,
        IMAGE_NUMBER_7,
        IMAGE_NUMBER_8,
        IMAGE_NUMBER_9,
        IMAGE_NUMBER_10,
        IMAGE_NUMBER_11,
        IMAGE_NUMBER_12,
        IMAGE_NUMBER_13,
        IMAGE_NUMBER_14,
        IMAGE_NUMBER_15,
        IMAGE_NUMBER_16,
        IMAGE_NUMBER_17,
        IMAGE_NUMBER_18,
        IMAGE_NUMBER_19,
        IMAGE_NUMBER_20
    };

    SCI_PASSERT(PNULL != list_style_ptr, ("MMITHEME_GetListStyle: the param is PNULL!")); /*assert verified*/
    //SCI_MEMSET(list_style_ptr, 0, sizeof(MMITHEME_LIST_STYLE_T));

    list_style_ptr->back_img_disp_rect   = MMITHEME_GetFullScreenRect();
    list_style_ptr->bg_info.color           = MMI_WINDOW_BACKGROUND_COLOR;

    list_style_ptr->bg_info.bg_type         = GUI_BG_IMG;
#ifdef MONOCHROME_LCD_SUPPORT
    list_style_ptr->highlight_back_color = MMI_BLACK_COLOR;
    list_style_ptr->is_disp_hilight_image = FALSE;
    list_style_ptr->highlight_back_image = 0;
#else
#if defined MAINLCD_SIZE_240X320
    list_style_ptr->highlight_back_color = MMI_BLACK_COLOR;
#else
    list_style_ptr->highlight_back_color = MMI_GRAY_WHITE_COLOR;
#endif
    list_style_ptr->is_disp_hilight_image = FALSE;
    list_style_ptr->highlight_back_image = IMAGE_COMMON_SELECTED_BAR;
#endif//syy modify 2010.11.30 -end

    list_style_ptr->a_percent_lucency    = LIST_HIGHLIGHT_PERCENT_LUCENCY;
    list_style_ptr->left_space           = 0;
    list_style_ptr->right_space          = 0;

    list_style_ptr->tag_width            = MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_MARGIN*2;
    list_style_ptr->tag_img_width        = MMI_CHOICE_PICKER_SELECT_ICON_WIDTH;
    list_style_ptr->avatar_checkbox_width = MMI_CHOICE_PICKER_BIG_ICON_WIDTH;
    list_style_ptr->radio_tag_width = MMI_RADIO_ICON_WIDTH + MMI_LIST_ICON_MARGIN*2; 
    list_style_ptr->toggle_tag_width = MMI_TOGGLE_ICON_WIDTH + MMI_LIST_ICON_MARGIN*2;
    
    // for text scroll!
    list_style_ptr->text_time            = MMITHEME_LIST_STYLE_TEXT_TIME;

    list_style_ptr->marked_image_id      = IMAGE_CHECKBOX_ON_CHECK_ICON;
    list_style_ptr->avatar_marked_image_id = IMAGE_AVATAR_CHECKBOX_ON_CHECK_ICON;
    list_style_ptr->radio_selected_image_id   = IMAGE_RADIO_SELECTED_ICON;   // 单选选中图标
    list_style_ptr->radio_unselected_image_id = IMAGE_RADIO_UNSELECTED_ICON; // 单选未选中图标
    list_style_ptr->check_selected_image_id   = IMAGE_CHECK_SELECTED_ICON;//IMAGE_CHECK_SELECTED_ICON;   // 多选选中图标
    list_style_ptr->check_unselected_image_id = IMAGE_CHECK_UNSELECTED_ICON; // 多选未选中图标
    list_style_ptr->avatar_checkbox_selected_image_id = IMAGE_AVATAR_CHECKBOX_SELECTED_ICON;//头像多选选中图标
    list_style_ptr->avatar_checkbox_unselected_image_id = IMAGE_AVATAR_CHECKBOX_UNSELECTED_ICON;//头像多选未选中图标
	list_style_ptr->avatar_checkbox_unselected_avatar_image_1_id = IMAGE_AVATAR_CHECKBOX_MASK_1_OFF;
    list_style_ptr->avatar_checkbox_unselected_avatar_image_2_id =IMAGE_AVATAR_CHECKBOX_MASK_2_OFF;
    list_style_ptr->toggle_on_image_id = IMAGE_CONTROL_TOGGLE_ON;//开启图标
    list_style_ptr->toggle_off_image_id = IMAGE_CONTROL_TOGGLE_OFF;//关闭图标

    list_style_ptr->long_key_period = LISTBOX_KEY_TIME;

    SCI_MEMCPY(list_style_ptr->num_icon_id, num_icon_id, sizeof(list_style_ptr->num_icon_id));

    list_style_ptr->text_ok_id     = TXT_COMMON_OK;
    list_style_ptr->text_return_id = STXT_RETURN;
    list_style_ptr->text_cancel_id = STXT_CANCEL;
    list_style_ptr->text_select_id = STXT_SELECT;
    list_style_ptr->text_mark_id   = STXT_SOFTKEY_MARK_MK;
    list_style_ptr->text_null_id   = TXT_NULL;

#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
    list_style_ptr->image_null_id   = IMAGE_NULL;
    list_style_ptr->image_ok_id     = IMAGE_COMMON_SOFTKEY_OK_ICON;
    list_style_ptr->image_return_id = IMAGE_COMMON_SOFTKEY_RETURN_ICON;
    list_style_ptr->image_cancel_id = IMAGE_COMMON_SOFTKEY_CANCEL_ICON;
    list_style_ptr->image_select_id = IMAGE_COMMON_SOFTKEY_SELECTED_ICON;
    list_style_ptr->image_mark_id   = IMAGE_COMMON_SOFTKEY_MARKED_ICON;
#endif
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
    list_style_ptr->left_key_type = GUILIST_SK_TYPE_TEXT_ID;
    list_style_ptr->mid_key_type = GUILIST_SK_TYPE_IMAGE_ID;
    list_style_ptr->right_key_type = GUILIST_SK_TYPE_TEXT_ID;
#else
    list_style_ptr->left_key_type = GUILIST_SK_TYPE_TEXT_ID;
    list_style_ptr->mid_key_type = GUILIST_SK_TYPE_TEXT_ID;
    list_style_ptr->right_key_type = GUILIST_SK_TYPE_TEXT_ID;
#endif

    list_style_ptr->text_empty_id  = TXT_EMPTY_LIST;
    list_style_ptr->text_empty_font  = MMI_DEFAULT_TEXT_FONT;
    list_style_ptr->text_empty_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0);

#ifdef MAINLCD_SIZE_240X320 //@for light function modify
    list_style_ptr->title_bg_image_id = IMAGE_NULL;//IMAGE_TITLE_BAR;
#else
    list_style_ptr->title_bg_image_id = MMITHEME_GetWinTitleBar();
#endif
    list_style_ptr->icon_default_id = IMAGE_COMMON_PIC_ICON;
    list_style_ptr->icon_error_id = IMAGE_COMMON_PIC_ERROR;
    list_style_ptr->mist_color = MMI_LIST_ITEM_MIST_COLOR;

    list_style_ptr->auto_switch_time = MMILIST_AUTO_SWITCH_TIME;
    list_style_ptr->slide_time = MMILIST_SLIDE_TIME;

#ifdef MMI_PDA_SUPPORT // 只有PDA的时候，高亮才消失
    list_style_ptr->is_hl_disappear = TRUE; // 高亮会消失
#else
    list_style_ptr->is_hl_disappear = FALSE; // 高亮不消失
#endif

#if defined GUIF_LIST_PDASTYLE
    list_style_ptr->split_item_bg_info.bg_type = GUI_BG_IMG;
    list_style_ptr->split_item_bg_info.img_id = IMAGE_CONTROL_LIST_SPLIT_ITEM_BG;
    list_style_ptr->expand_image_id = IMAGE_CONTROL_LIST_RIGHT_ARROW;
    list_style_ptr->add_item_image_id = IMAGE_CONTROL_LIST_ADD;
    list_style_ptr->delete_item_image_id = IMAGE_CONTROL_LIST_DELETE;
    list_style_ptr->delete_query_image_id = IMAGE_CONTROL_LIST_DELETE_QUERY;
#else
    list_style_ptr->split_item_bg_info.bg_type = GUI_BG_COLOR;
    list_style_ptr->split_item_bg_info.color = MMI_LIST_ITEM_SPLIT_BG_COLOR;
#endif
    list_style_ptr->split_item_text_font_color = MMI_DARK_BLUE_COLOR;//MMI_DARK_WHITE_COLOR;
    list_style_ptr->split_item_text_alpha = (255*0.50);
    list_style_ptr->split_line_hight = 3;
    list_style_ptr->split_line_color_with_label = MMI_BLACK_COLOR;//MMI_DARK_WHITE_COLOR;
	
    list_style_ptr->split_line_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SPLIT_LINE);

    list_style_ptr->is_slide_used = TRUE;

    list_style_ptr->item_font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0);            //item color

    //tip
    list_style_ptr->tip_hide_tick = MMI_LIST_TIP_HIDE_TICK;
#ifdef GUIF_LIST_PDASTYLE
    list_style_ptr->tip_bg = IMAGE_CONTROL_LIST_TIP_BG;
#endif
    list_style_ptr->tip_font_color = MMI_LIST_TIP_FONT_COLOR;

    list_style_ptr->select_cancel_index = LEFT_BUTTON;
#ifdef MMI_COMPILE_E//8910UIX  
    list_style_ptr->avatar_mask_image_id = IMAGE_LISTITEM_AVATAR_MASK_BLUE;//IMAGE_LIST_MASK;
#else
    list_style_ptr->avatar_mask_image_id = IMAGE_LISTITEM_AVATAR_MASK_RED;//IMAGE_LIST_MASK;
#endif
    list_style_ptr->avatar_mask_left_space = MMI_LIST_ICON_LEFT_SPACE_MS - 1;
    list_style_ptr->avatar_mask_width = MMI_LIST_BIG_ICON_WIDTH_MS+2;
    list_style_ptr->initial_mask_image_id = IMAGE_LISTITEM_AVATAR_INITIALS;//IMAGE_LIST_AVATAR_INITIALS;
    list_style_ptr->initial_mask_left_space = MMI_LIST_ICON_LEFT_SPACE_MS;
    list_style_ptr->initial_mask_width = MMI_LIST_BIG_ICON_WIDTH_MS+2;

    list_style_ptr->left_triangle_icon_id = IMAGE_LISTITEM_SHORTCUT_64_LEFT_RED;//IMAGE_LIST_SHORTCUT_LEFT;
    list_style_ptr->right_triangle_icon_id = IMAGE_LISTITEM_SHORTCUT_64_RIGHT_RED;//IMAGE_LIST_SHORTCUT_RIGHT;
    list_style_ptr->triangle_width = MMI_LIST_TRIANGLE_WIDTH_MS;
    list_style_ptr->left_triangle_left_space = 0;
    list_style_ptr->right_triangle_right_space = MMITHEME_GetFullScreenRect().right;

    list_style_ptr->quick_icon_bg_width = MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS;
    list_style_ptr->quick_icon_margin = MMI_LIST_QUICK_ICON_MARGIN_MS;
    list_style_ptr->quick_icon_width = MMI_LIST_QUICK_ICON_WIDTH_MS;
    list_style_ptr->quick_and_triangel_space = MMI_LIST_ICON_LEFT_SPACE_MS;
    list_style_ptr->left_quick_icon_image = PNULL;
    list_style_ptr->right_quick_icon_image = PNULL;
    list_style_ptr->simsel_icon_image = PNULL;
    list_style_ptr->simsel_width = MMI_LIST_SIMSEL_WIDTH_MS;

    list_style_ptr->is_custom_right_sk = FALSE;
    list_style_ptr->image_accessory_id = PNULL;
    return (TRUE);
}

//add for pb_avatar_text_item_quick_action
#if 0
/*****************************************************************************/
//  Description : get the list style, is called by the list control
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetListStyleByItemStateAndSytle(
    MMITHEME_LIST_STYLE_T *list_style_ptr,
    uint32/*GUIITEM_STATE_T*/ item_state,
    GUIITEM_STYLE_E item_style    
)
{
	if(!item_state&(~GUIITEM_STATE_LEFT_FOCUS) && !item_state&(~GUIITEM_STATE_RIGHT_FOCUS))
	{
		list_style_ptr->quick_icon_bg_width = MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS;
		list_style_ptr->quick_icon_margin = MMI_LIST_QUICK_ICON_MARGIN_MS;
		list_style_ptr->quick_icon_width = MMI_LIST_QUICK_ICON_WIDTH_MS;
		return TRUE;
	}
	
	if(GUIITEM_STYLE_PB_ONE_LINE_AVATAR_TEXT_LEFT_FOCUS_MS == item_style)
	{
		list_style_ptr->quick_icon_bg_width = MMI_LIST_PB_QUICK_ACTION_ITEM_QUICK_ICON_BG_WIDTH_MS;
		list_style_ptr->quick_icon_margin = MMI_LIST_PB_QUICK_ACTION_ITEM_QUICK_ICON_MARGIN_MS-1;
		list_style_ptr->quick_icon_width = MMI_LIST_PB_QUICK_ACTION_ITEM_QUICK_ICON_HEIGHT_WIDTH_MS;
	}
	else
	{
		list_style_ptr->quick_icon_bg_width = MMI_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS;
		list_style_ptr->quick_icon_margin = MMI_LIST_QUICK_ICON_MARGIN_MS;
		list_style_ptr->quick_icon_width = MMI_LIST_QUICK_ICON_WIDTH_MS;
	}
	
	return TRUE;
}
#endif
//add for pb_avatar_text_item_quick_action end

#ifdef EFFECT_3D_LIST_ROLL_SUPPORT

/*****************************************************************************/
//  Description : is need 3d list
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsNeed3DList(void)
{
    return (BOOLEAN)((MMI_IsPermit3D((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMI_3D_SECOND_LIST))
                     && (MMIAPISET_GetList3DStyle()));
}

/*****************************************************************************/
//  Description : alloc 3d list buffer
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_Alloc3DListBuf(
    uint32 buf_size,
    MMI_HANDLE_T    win_handle
)
{
    MMIAPI3D_Alloc3DMenuBuf((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMI_3D_SECOND_LIST, buf_size, win_handle);
}

/*****************************************************************************/
//  Description : save 3d list buffer
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_Save3DListBuf(
    GUILIST_3D_BUF_E type,
    GUI_RECT_T rect
)
{
    if (type < GUILIST_BUF_MAX)
    {
        MMI3D_ICONMENU_BUF_E buf_type = PREV_BUF;
        SE_RECT_T se_rect = {0};

        switch (type)
        {
            case GUILIST_PREV_BUF:
                buf_type = PREV_BUF;
                break;

            case GUILIST_NEXT_BUF:
                buf_type = NEXT_BUF;
                break;

            case GUILIST_PREV_BG_BUF:
                buf_type = PREV_BG_BUF;
                break;

            case GUILIST_NEXT_BG_BUF:
                buf_type = NEXT_BG_BUF;
                break;

            default:
                break;
        }

        se_rect.left = rect.left;
        se_rect.right = rect.right;
        se_rect.top = rect.top;
        se_rect.bottom = rect.bottom;

        MMIAPI3D_SaveMenuIcon(buf_type, &se_rect);
    }
}

/*****************************************************************************/
//  Description : set 3d list direction
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_Set3DListDirection(
    BOOLEAN is_next
)
{
    SE_DIRECTION_E  se_dir = SE_DIRECTION_DOWN;

    if (is_next)
    {
        se_dir = SE_DIRECTION_DOWN;
    }
    else
    {
        se_dir = SE_DIRECTION_UP;
    }

    //set icon move direction
    MMIAPI3D_SetIconMenuDirection(se_dir);
}

/*****************************************************************************/
//  Description : start 3d list anim
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_Start3DListAnim(void)
{
    MMIAPI3D_StartItemAnim((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMI_3D_SECOND_LIST);
}

/*****************************************************************************/
//  Description : free 3d list buf
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_Free3DListBuf(void)
{
    MMIAPI3D_Free3DMenuBuf((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMI_3D_SECOND_LIST);
}
#endif

/*****************************************************************************/
//  Description : get the list item backgroup image id
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMITHEME_GetListItemBGImage(GUIITEM_THEME_BGIMAGE_TYPE_E item_bg_type)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;

#ifdef GUIF_LIST_PDASTYLE

    switch (item_bg_type)
    {
        case GUIITEM_THEME_ALERT_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_ALERT_BG;
            break;

        case GUIITEM_THEME_ITEM_FULL_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_BG;
            break;

        case GUIITEM_THEME_ITEM_HEAD_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_HEAD_BG;
            break;

        case GUIITEM_THEME_ITEM_BODAY_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_BODAY_BG;
            break;

        case GUIITEM_THEME_ITEM_TAIL_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_TAIL_BG;
            break;

        case GUIITEM_THEME_HIGHLIGHT_ITEM_FULL_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_BG_PR;
            break;

        case GUIITEM_THEME_HIGHLIGHT_ITEM_HEAD_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_HEAD_BG_PR;//IMAGE_LIST_ITEM_HEAD_BG_PR;
            break;

        case GUIITEM_THEME_HIGHLIGHT_ITEM_BODAY_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_BG_PR;//IMAGE_CONTROL_LIST_ITEM_BODAY_BG_PR;
            break;

        case GUIITEM_THEME_HIGHLIGHT_ITEM_TAIL_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_TAIL_BG_PR;
            break;

        default:
            break;
    }

#endif
    return image_id;
}

/*****************************************************************************/
//  Description : get the list item disp data
//  Global resource dependence :
//  Author: xiaoqinglu
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetListItemDefData(
    MMITHEME_LIST_CUSTDATA_T *data_ptr
)
{
    //SCI_ASSERT(PNULL != data_ptr);
    if (PNULL == data_ptr)
    {
        return;
    }

    data_ptr->data_type = GUILIST_TYPE_TEXT_ID;
    data_ptr->str_style.font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0);            //item color
    data_ptr->str_style.font = MMI_DEFAULT_BIG_FONT;
    data_ptr->str_style.align = ALIGN_HVMIDDLE;
#ifndef MMI_RES_LOW_COST
    data_ptr->left_icon_id = IMAGE_CONTROL_LIST_LEFT_SLIDE;
#endif

    data_ptr->left_text_id = TXT_SMS;
#ifndef MMI_RES_LOW_COST
    data_ptr->right_icon_id = IMAGE_CONTROL_LIST_RIGHT_SLIDE;
#endif
    data_ptr->right_text_id = TXT_DIAL;

    return;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC THEMELIST_ITEM_STYLE_T *THEMELIST_GetStyle(GUIITEM_STYLE_E style_id)
{
    uint16 i = 0;
    uint16 style_count = 0;
    THEMELIST_ITEM_STYLE_T *style_ptr = PNULL;

    style_count = sizeof(s_style_table)/sizeof(THEMELIST_STYLE_TABLE_T);

    for (i = 0; i < style_count; i++)
    {
        if (style_id == s_style_table[i].id)
        {
            style_ptr = s_style_table[i].style;
            break;
        }
    }
    return style_ptr;
}

/*****************************************************************************/
//  Description : get content text color
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T THEMELIST_GetSplitItemTextColor(void)
{
    return MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM_SMALL_FONT);
}


PUBLIC int16 THEMELIST_GetListBaseWidth(void)
{
    return MMI_LIST_ITEM_RIGHT;
}
