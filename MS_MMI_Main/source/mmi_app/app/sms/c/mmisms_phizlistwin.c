/*****************************************************************************
 ** File Name:   mmisms_phizlistwin.c
 ** Author:      sam.hua
 ** Date:        23/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe phizlist Win
 *****************************************************************************
 **                         Important Edit History                           *
 ** -------------------------------------------------------------------------*
 *  ** DATE           NAME             DESCRIPTION                           *
 *  23/aug/2012      sam.hua       written
 ****************************************************************************/

#include "guilistbox.h"
#include "guimenu.h"
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmisms_anim.h"
#include "mmisms_id.h"
#include "mmisms_app.h"
#include "mmisms_mainmenuwin.h"
#include "mmisms_messagecountwin.h"
#include "mmisms_messageboxwin.h"
#include "mmisms_commonui.h"


/*---------------------------------------------------------------------------*/
/*                            DEFINES                                        */
/*---------------------------------------------------------------------------*/
#if defined (MAINLCD_SIZE_128X160)
#define MMISMS_PHIZ_NUM    21    //
#define PHIZ_MAX_LEN 7
#define MMISMS_PHIZ_HEIGHT  30//phiz height
#define MMISMS_PHIZ_ROW     3
#define MMISMS_PHIZ_COL    4
#define MMISMS_PHIZ_NUM_LIST      12
#define MMISMS_PHIZ_FONT  SONG_FONT_18
#else
#define MMISMS_PHIZ_NUM    21    //
#define PHIZ_MAX_LEN 7
#define MMISMS_PHIZ_HEIGHT  60//phiz height
#define MMISMS_PHIZ_ROW     3
#define MMISMS_PHIZ_COL    4
#define MMISMS_PHIZ_NUM_LIST      12
#define MMISMS_PHIZ_FONT  SONG_FONT_24
#endif
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
const char * const s_sms_phiz[MMISMS_PHIZ_NUM] =
{
    ":-)",
    "^O^",
    ":-]",
    "*^_^*",
    ";-)",
    ":~)",
    "i-)",
    "<@_@>",
    ":-(",
    ":-O",
    ":-D",
    "?_?",
    "<:I",
    "(:-...",
    ":-(*)",
    ":-')",
    "X-<",
    ":-\\",
    ":-e",
    "#-)",
    ":-7"
};//插入表情符

static wchar const * s_sms_phiz_list[MMISMS_PHIZ_NUM_LIST] =
{
    L":)",
    L";)",
    L":D",
    L":P",
    L":*",
    L":o",
    L":|",
    L":/",
    L"8)",
    L":(",
    L">:(",
    L">:o"
};//插入表情符
typedef struct
{
    //MMI_HANDLE_T im_handle;
    
    GUI_LCD_DEV_INFO lcd_dev_info;

    MMI_WIN_ID_T win_id;
    GUI_RECT_T win_rect;
    GUI_RECT_T title_rect;
    //panel
    GUI_RECT_T rect_phiz;
    uint16 row;
    uint16 col;

    //page tips
    GUI_RECT_T rect_page_tip;

    //color
    GUI_COLOR_T color_bg;
    GUI_COLOR_T color_border;
    GUI_COLOR_T color_font;
    GUI_COLOR_T color_hot;

    GUI_FONT_T font;
    GUI_FONT_T font_min;
    GUI_FONT_T font_label;

    //disp
    uint16 row_cnt;
    uint16 first_row;
    uint16 hot_idx;

    //data
    uint32 phiz_cnt;
} MMISMS_PHIZ_WIN_DATA_T;

LOCAL MMISMS_PHIZ_WIN_DATA_T *win_phiz_data_ptr = PNULL;
/** -------------------------------------------------------------------------*/
//                          LOCAL FUNCTIONS                                 //
/** -------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : to handle the message of window which choice the phiz
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhizListWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM                param        //IN:
);
/*****************************************************************************/
//     Description : init smiley
//    Global resource dependence :
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void  InitPhizData(MMISMS_PHIZ_WIN_DATA_T *win_data_ptr);
/*==============================================================================
Function Name: GetHotPos
Description: GetHotPos
Author: 
Global resource dependence: NONE
Note: 负行的修正
==============================================================================*/
LOCAL BOOLEAN GetPhizHotPos(
    MMISMS_PHIZ_WIN_DATA_T *win_data_ptr,
    int16 *row_ptr,
    int16 *col_ptr
);
/*==============================================================================
Function Name: GetSymbRect
Description: 获取指定位置的背景框大小
Author: 
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN GetPhizRect(
    MMISMS_PHIZ_WIN_DATA_T *win_data_ptr,
    int16 row,
    int16 col,
    GUI_RECT_T *rect_ptr
);
/*==============================================================================
Function Name: MMIIM_SP_SYMB_GetSymbStrPtr
Description: 获取字符串的指针
Author: 
Global resource dependence: NONE
Note: 宽字符，0结尾
==============================================================================*/
wchar const * MMISMS_GetPhizStrPtr(wchar symb);
/*****************************************************************************/
//     Description : draw phiz data
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void DrawPhizWin(MMISMS_PHIZ_WIN_DATA_T *win_data_ptr);
/*****************************************************************************/
//     Description : load total phiz to list
//    Global resource dependence :
//  Author: Tracy zhang
//    Note:
/*****************************************************************************/
LOCAL void LoadTotalPhiz2List(
    MMI_CTRL_ID_T        ctrl_id        //IN:
);


/** -------------------------------------------------------------------------*/
//                          WINTAB DEFINES                                  //
/** -------------------------------------------------------------------------*/

// the phiz list window
WINDOW_TABLE( MMISMS_PHIZLIST_WIN_TAB ) =
{
    //WIN_TITLE( TXT_SMS_PHIZ ),
    WIN_FUNC((uint32) HandlePhizListWinMsg),
    WIN_ID( MMISMS_PHIZLIST_WIN_ID ),
    WIN_HIDE_STATUS,
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
#endif
    //CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_PHIZ_LISTBOX_CTRL_ID ),

    END_WIN
};


/** -------------------------------------------------------------------------*/
//                          Function definition                              //
/** -------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : enter phiz win
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_OpenPhizWin(MMI_HANDLE_T ctrl_handle)
{
    win_phiz_data_ptr = (MMISMS_PHIZ_WIN_DATA_T*)SCI_ALLOC_APP(sizeof(MMISMS_PHIZ_WIN_DATA_T));
    SCI_MEMSET(win_phiz_data_ptr, 0, sizeof(MMISMS_PHIZ_WIN_DATA_T));
#ifdef MMI_PDA_SUPPORT
    MMK_CreatePubListWin((uint32 *)MMISMS_PHIZLIST_WIN_TAB, (ADD_DATA)ctrl_handle);
#else
    MMK_CreateWin((uint32 *)MMISMS_PHIZLIST_WIN_TAB, (ADD_DATA)ctrl_handle);
#endif
}
/*==============================================================================
Function Name: GetHotPos
Description: GetHotPos
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 负行的修正
==============================================================================*/
LOCAL BOOLEAN GetPhizHotPos(
    MMISMS_PHIZ_WIN_DATA_T *win_data_ptr,
    int16 *row_ptr,
    int16 *col_ptr
)
{
    int16 hot_idx;

    if (PNULL == win_data_ptr)
    {
        return FALSE;
    }

    hot_idx = win_data_ptr->hot_idx;
    hot_idx -= win_data_ptr->first_row * win_data_ptr->col;

    if (hot_idx < 0)
    {
        //如果是负的，则从-1行还是算，因为没有-0行
        hot_idx -= win_data_ptr->col;
    }

    *row_ptr = hot_idx / win_data_ptr->col;
    *col_ptr = hot_idx % win_data_ptr->col;

    return TRUE;
}
/*==============================================================================
Function Name: GetSymbRect
Description: 获取指定位置的背景框大小
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN GetPhizRect(
    MMISMS_PHIZ_WIN_DATA_T *win_data_ptr,
    int16 row,
    int16 col,
    GUI_RECT_T *rect_ptr
)
{
    int16 rect_width = 0;
    int16 rect_height = 0;
    int16 unit_width = 0;
    int16 unit_height = 0;

    if (row < 0 || row >= win_data_ptr->row
            || col < 0 || col >= win_data_ptr->col)
    {
        return FALSE;
    }

    rect_width = win_data_ptr->rect_phiz.right - win_data_ptr->rect_phiz.left + 1;
    rect_height = win_data_ptr->rect_phiz.bottom - win_data_ptr->rect_phiz.top + 1;
    unit_width = rect_width / win_data_ptr->col;
    unit_height = rect_height / win_data_ptr->row;

    rect_ptr->top = win_data_ptr->rect_phiz.top + unit_height * row;
    rect_ptr->left = win_data_ptr->rect_phiz.left + unit_width * col;
    rect_ptr->bottom = (row == win_data_ptr->row - 1) ?
                       win_data_ptr->rect_phiz.bottom : (rect_ptr->top + unit_height);
    rect_ptr->right = (col == win_data_ptr->col - 1) ?
                      win_data_ptr->rect_phiz.right : (rect_ptr->left + unit_width);

    rect_ptr->top++;
    rect_ptr->left++;
    rect_ptr->bottom--;
    rect_ptr->right--;

    return TRUE;
}
/*==============================================================================
Function Name: MMIIM_SP_SYMB_GetSymbStrPtr
Description: 获取字符串的指针
Author: 
Global resource dependence: NONE
Note: 宽字符，0结尾
==============================================================================*/
wchar const * MMISMS_GetPhizStrPtr(wchar symb)
{
    wchar const *phiz_str_ptr = NULL;

    phiz_str_ptr = s_sms_phiz_list[symb];
   
    return phiz_str_ptr;
}

/*****************************************************************************/
//     Description : draw phiz data
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void DrawPhizWin(MMISMS_PHIZ_WIN_DATA_T *win_data_ptr)
{   
    GUISTR_STYLE_T tip_style = {0};
    int16 unit_width = 0;
    int16 unit_height = 0;
    int16 rect_width = 0;
    int16 rect_height = 0;
    
    GUI_RECT_T hot_rect = {0};
    int16 hot_row = 0;
    int16 hot_col = 0;
    uint32 symb_first = 0;
    uint32 symb_per_page = 0;
    uint16 symb_idx = 0;
    uint16 symb_row = 0;
    uint16 symb_col = 0;
    
    GUI_RECT_T rect_symb = {0};
    MMI_STRING_T symb_info = {0};
    GUISTR_STYLE_T symb_style = {0};
    
    
    MMI_STRING_T prompt_str = {0};

    if (PNULL == win_data_ptr)
    {
        return;
    }

    //draw...
    //draw panel
    LCD_FillRect(&(win_data_ptr->lcd_dev_info),
                 win_data_ptr->win_rect,
                 win_data_ptr->color_bg);

    //draw title
    MMI_GetLabelTextByLang(STR_HEADER_SMILEY, &prompt_str);

     //draw page tip
    tip_style.font = win_data_ptr->font_min;
    tip_style.font_color = win_data_ptr->color_font;
    tip_style.align = ALIGN_LVMIDDLE;
    GUISTR_DrawTextToLCDInRect(&win_data_ptr->lcd_dev_info,
                               &win_data_ptr->title_rect,
                               &win_data_ptr->title_rect,
                               &prompt_str,
                               &tip_style,
                               GUISTR_STATE_NONE,
                               GUISTR_TEXT_DIR_AUTO);

        //cal row and column
    win_data_ptr->rect_phiz.bottom = win_data_ptr->rect_phiz.top + MMISMS_PHIZ_HEIGHT * MMISMS_PHIZ_ROW ; //180;
    rect_width = win_data_ptr->rect_phiz.right - win_data_ptr->rect_phiz.left + 1;
    rect_height = win_data_ptr->rect_phiz.bottom - win_data_ptr->rect_phiz.top + 1;
    unit_width = rect_width / win_data_ptr->col;//每个单元格宽度60
    unit_height = rect_height / win_data_ptr->row;

        //hot
    GetPhizHotPos(win_data_ptr, &hot_row, &hot_col);

    //SCI_ASSERT(hot_row >= 0); /*assert verified*/
    if(hot_row < 0)
    {
        return;
    }
    
    hot_rect.top = win_data_ptr->rect_phiz.top + unit_height * hot_row;
    hot_rect.left = win_data_ptr->rect_phiz.left + unit_width * hot_col;
    hot_rect.bottom = (hot_row == win_data_ptr->row - 1) ?
                      win_data_ptr->rect_phiz.bottom : (hot_rect.top + unit_height);
    hot_rect.right = (hot_col == win_data_ptr->col - 1) ?
                     win_data_ptr->rect_phiz.right : (hot_rect.left + unit_width);
    hot_rect.top++;
    hot_rect.left++;
    hot_rect.bottom--;
    hot_rect.right--;

    LCD_FillRect(&(win_data_ptr->lcd_dev_info),
                 hot_rect,
                 win_data_ptr->color_hot);

    //draw symbol
    symb_first = win_data_ptr->first_row * win_data_ptr->col;

    symb_per_page = win_data_ptr->row * win_data_ptr->col;
    symb_style.font = win_data_ptr->font;
    symb_style.font_color = win_data_ptr->color_font;
    symb_style.align = ALIGN_HVMIDDLE;
    //画表情
    
    for (symb_idx = 0; symb_idx < symb_per_page; symb_idx++)
    {
        //没有符号了，则不画了
        if (symb_first + symb_idx >= win_data_ptr->phiz_cnt) break;

        symb_row = symb_idx / win_data_ptr->col;
        symb_col = symb_idx % win_data_ptr->col;
        GetPhizRect(win_data_ptr, symb_row, symb_col, &rect_symb);

        //cur_symb = win_data_ptr->symb_list[symb_first + symb_idx];

        
            symb_info.wstr_ptr = MMISMS_GetPhizStrPtr(symb_idx); //lint !e605
            symb_info.wstr_len = MMIAPICOM_Wstrlen(symb_info.wstr_ptr);
            symb_style.font = win_data_ptr->font;

            //string
            GUISTR_DrawTextToLCDInRect(&win_data_ptr->lcd_dev_info,
                                       &rect_symb,
                                       &rect_symb,
                                       &symb_info,
                                       &symb_style,
                                       GUISTR_STATE_NONE,
                                       GUISTR_TEXT_DIR_AUTO);            
        
      
        
    }

   

}
/*****************************************************************************/
//     Description : init phiz data
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void  InitPhizData(MMISMS_PHIZ_WIN_DATA_T *win_data_ptr)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T key_rect = {0};

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    if(PNULL == win_data_ptr)
    {
        return;
    }
    
    win_data_ptr->lcd_dev_info = lcd_dev_info;

    //该窗口无status bar
    win_data_ptr->win_rect = MMITHEME_GetClientRectEx(win_data_ptr->win_id);
    //win_data_ptr->win_rect.top = 0;

    //该窗口有title
    win_data_ptr->title_rect = win_data_ptr->win_rect;
    win_data_ptr->title_rect.bottom = MMI_TITLE_THEME_HEIGHT-1;//MMITHEME_GetStatusBarHeight() - 1;
    
    win_data_ptr->rect_phiz= win_data_ptr->win_rect;
    win_data_ptr->rect_phiz.top = MMI_TITLE_THEME_HEIGHT; //MMITHEME_GetStatusBarHeight();
    win_data_ptr->rect_phiz.bottom = win_data_ptr->rect_phiz.top + MMISMS_PHIZ_HEIGHT * MMISMS_PHIZ_ROW;//180;

    win_data_ptr->row = MMISMS_PHIZ_ROW;
    win_data_ptr->col = MMISMS_PHIZ_COL;

    win_data_ptr->color_bg = GUI_RGB2RGB565(255,255,255);/*lint !e572*/
    win_data_ptr->color_border = GUI_RGB2RGB565(255,255,255);/*lint !e572*/
    win_data_ptr->color_font = GUI_RGB2RGB565(0,0,0);/*lint !e572*/
    win_data_ptr->color_hot = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT);

    win_data_ptr->font = MMISMS_PHIZ_FONT;
    win_data_ptr->phiz_cnt = MMISMS_PHIZ_NUM_LIST;
    win_data_ptr->font_min = MMI_DEFAULT_NORMAL_FONT;
    //win_data_ptr->font_label = MMIIM_SP_SYMB_FONT_LABEL;

    //LoadPhizData(win_data_ptr);

    //空出地方来放按钮
    //win_data_ptr->rect_symb.top += MMIIM_SP_SYMB_MARGIN_V;
    //win_data_ptr->rect_symb.top += MMIIM_SP_SYMB_KEY_HEIGHT;

    key_rect.top = win_data_ptr->win_rect.top ;
    key_rect.left = win_data_ptr->win_rect.left;
    key_rect.bottom = key_rect.top + MMISMS_PHIZ_HEIGHT- 1;
    key_rect.right = win_data_ptr->win_rect.right;
    
    //close
    win_data_ptr->rect_page_tip = key_rect;

    return;
}


/*****************************************************************************/
//     Description : to handle the message of window which choice the phiz
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhizListWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM                param        //IN:
)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
    
    int16 hot_row = 0;
    int16 hot_col = 0;
    int16 cur_index = 0;
    MMI_STRING_T prompt_str = {0};
    MMI_HANDLE_T    ctrl_handle= (MMI_HANDLE_T)MMK_GetWinAddDataPtr(win_id);
	BOOLEAN is_right = FALSE;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        win_phiz_data_ptr->win_id = win_id;
        InitPhizData(win_phiz_data_ptr);
        break;

        case MSG_CLOSE_WINDOW:
            if(PNULL !=win_phiz_data_ptr)
            {
                SCI_FREE(win_phiz_data_ptr);
            }              
            break;

        case MSG_FULL_PAINT:
            //draw
            DrawPhizWin(win_phiz_data_ptr);
            break;
        
        case MSG_CTL_OK:
        case MSG_APP_OK:
            break;
        case MSG_APP_WEB:
             {
                cur_index = win_phiz_data_ptr->hot_idx;
                prompt_str.wstr_ptr = MMISMS_GetPhizStrPtr(cur_index); //lint !e605
                prompt_str.wstr_len = MMIAPICOM_Wstrlen(prompt_str.wstr_ptr);
                is_right = GUIEDIT_InsertString(ctrl_handle,prompt_str.wstr_ptr,prompt_str.wstr_len);
				if(!is_right)
				{
					// can not insert to edit, because the wstr_len is out of range.
					MMIPUB_OpenAlertWarningWin(STR_MSG_MAX_LENGTH);					
				}
             }
            MMK_CloseWin(MMISMS_EDITOPTION_WIN_ID);
            MMK_CloseWin(win_id);
            break;

        case MSG_APP_LEFT:

            if (win_phiz_data_ptr->hot_idx > 0)
            {
                win_phiz_data_ptr->hot_idx--;
            }
            else
            {
                win_phiz_data_ptr->hot_idx = win_phiz_data_ptr->phiz_cnt -1;
            }
            GetPhizHotPos(win_phiz_data_ptr, &hot_row, &hot_col);
            //SCI_ASSERT(hot_row <= win_data_ptr->row); /*assert verified*/
            if(hot_row > win_phiz_data_ptr->row)
            {
                break;
            }
            DrawPhizWin(win_phiz_data_ptr);
            break;

        case MSG_APP_RIGHT:

            if (win_phiz_data_ptr->hot_idx < win_phiz_data_ptr->phiz_cnt - 1)
            {
                win_phiz_data_ptr->hot_idx++;
                
            }
            else
            {
                win_phiz_data_ptr->hot_idx = 0;
            }
            GetPhizHotPos(win_phiz_data_ptr, &hot_row, &hot_col);
            //SCI_ASSERT(hot_row <= win_data_ptr->row); /*assert verified*/
            if(hot_row > win_phiz_data_ptr->row)
            {
                break;
            }

            DrawPhizWin(win_phiz_data_ptr);
            break;

        case MSG_APP_UP:

            if (win_phiz_data_ptr->hot_idx >= win_phiz_data_ptr->col)
            {
                win_phiz_data_ptr->hot_idx -= win_phiz_data_ptr->col;
                
            }
            else
            {
                win_phiz_data_ptr->hot_idx += win_phiz_data_ptr->phiz_cnt - win_phiz_data_ptr->col;
            }
            GetPhizHotPos(win_phiz_data_ptr, &hot_row, &hot_col);
            //SCI_ASSERT(hot_row <= win_data_ptr->row); /*assert verified*/
            if(hot_row > win_phiz_data_ptr->row)
            {
                break;
            }
                
            DrawPhizWin(win_phiz_data_ptr);
            break;

        case MSG_APP_DOWN:

            if (win_phiz_data_ptr->hot_idx < win_phiz_data_ptr->phiz_cnt - win_phiz_data_ptr->col)
            {
                win_phiz_data_ptr->hot_idx += win_phiz_data_ptr->col;
                
            }
            else
            {
                win_phiz_data_ptr->hot_idx += win_phiz_data_ptr->col - win_phiz_data_ptr->phiz_cnt;
            }
            GetPhizHotPos(win_phiz_data_ptr, &hot_row, &hot_col);
            //SCI_ASSERT(hot_row <= win_data_ptr->row); /*assert verified*/
            if(hot_row > win_phiz_data_ptr->row)
            {
                break;
            }
           
            DrawPhizWin(win_phiz_data_ptr);
            break;
            
        case MSG_LCD_SWITCH:
            InitPhizData(win_phiz_data_ptr);
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        default:
            res = MMI_RESULT_FALSE;
            break;
            
    }
    
    return res;
}
#if 0//MS_MMI_Delete
LOCAL MMI_RESULT_E HandlePhizListWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM                param        //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_PHIZ_LISTBOX_CTRL_ID;
    uint16 cur_index = 0;
    BOOLEAN is_right = TRUE;
    wchar sms_phiz[20] = {0};
    MMI_HANDLE_T    ctrl_handle= (MMI_HANDLE_T)MMK_GetWinAddDataPtr(win_id);
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            LoadTotalPhiz2List(ctrl_id);
            GUILIST_SetNeedPrgbar(ctrl_id, TRUE);
            MMK_SetAtvCtrl(win_id, ctrl_id);
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifndef MMI_PDA_SUPPORT
        case MSG_CTL_OK:
        case MSG_APP_OK:
#endif
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            cur_index = GUILIST_GetCurItemIndex(ctrl_id);

            MMI_STRNTOWSTR( sms_phiz,
                            20,
                            (const uint8 *)s_sms_phiz[cur_index],
                            strlen((char *)s_sms_phiz[cur_index]),
                            strlen((char *)s_sms_phiz[cur_index]));

            // insert the selected phiz to editbox
            is_right = GUIEDIT_InsertString(
                            ctrl_handle,
                           sms_phiz,
                           (uint16)MMIAPICOM_Wstrlen(sms_phiz));
            if (!is_right)
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_BEYOND_LEN);
            }
            MMK_CloseWin(MMISMS_EDITOPTION_WIN_ID);
            MMK_CloseWin(win_id);
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
#endif
/*****************************************************************************/
//     Description : load total phiz to list
//    Global resource dependence :
//  Author: Tracy zhang
//    Note:
/*****************************************************************************/
LOCAL void LoadTotalPhiz2List(
    MMI_CTRL_ID_T        ctrl_id        //IN:
)
{
    uint16 i = 0;
    uint8 j = 0;
    uint8 phiz_len = 0;
    MMI_STRING_T item_str = {0};
    MMI_STRING_T prompt_str = {0};

    item_str.wstr_ptr = SCI_ALLOC_APP((GUILIST_STRING_MAX_NUM + 1) * sizeof(wchar));

    GUILIST_SetMaxItem(ctrl_id, MMISMS_PHIZ_NUM, FALSE );//max item 10

    for (i = 0; i < MMISMS_PHIZ_NUM; i++)
    {
        SCI_MEMSET(item_str.wstr_ptr, 0, ((GUILIST_STRING_MAX_NUM + 1)*sizeof(wchar)));

        phiz_len = strlen((char *)s_sms_phiz[i]);
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)(TXT_SMS_SMILE + i), &prompt_str);

        MMI_STRNTOWSTR(item_str.wstr_ptr, GUILIST_STRING_MAX_NUM, (const uint8 *)s_sms_phiz[i], phiz_len, phiz_len);

        for (j = phiz_len; j < PHIZ_MAX_LEN; j++)
        {
            item_str.wstr_ptr[j] = 0x0020;
        }

        MMI_WSTRNCPY(&(item_str.wstr_ptr[PHIZ_MAX_LEN]),
                     MIN(GUILIST_STRING_MAX_NUM - PHIZ_MAX_LEN , prompt_str.wstr_len),
                     prompt_str.wstr_ptr,
                     MIN(GUILIST_STRING_MAX_NUM - PHIZ_MAX_LEN , prompt_str.wstr_len),
                     MIN(GUILIST_STRING_MAX_NUM - PHIZ_MAX_LEN , prompt_str.wstr_len));

        item_str.wstr_len = MIN(PHIZ_MAX_LEN + prompt_str.wstr_len, GUILIST_STRING_MAX_NUM);

        MMISMS_AppendListItem(
            item_str.wstr_ptr,
            (uint8)item_str.wstr_len,
            ctrl_id,
            TXT_COMMON_OK,
            TXT_NULL,
            i,
            MMISMS_APPEND_LIST_ITEM_TYPE
        );
    }

    if (PNULL != item_str.wstr_ptr)
    {
        SCI_FREE(item_str.wstr_ptr);
        item_str.wstr_ptr = PNULL;
    }
}

