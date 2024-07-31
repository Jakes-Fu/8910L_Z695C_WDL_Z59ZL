#include "guiblock.h"
#include "guigraph.h"
#include "guilcd.h"
#include "mmi_appmsg.h"
#include "mmk_app.h"
#include "sci_api.h"
#include "window_parse.h"

#include "autocit_lcd.h"

#define AUTOCIT_LCD_GUI_COLOR_WHITE		0xFFFF
#define AUTOCIT_LCD_GUI_COLOR_BLACK		0x20E4
#define AUTOCIT_LCD_GUI_COLOR_RED		0xF800
#define AUTOCIT_LCD_GUI_COLOR_GREEN		0x07E0
#define AUTOCIT_LCD_GUI_COLOR_BLUE		0x001F

#define AUTOCIT_LCD_UPDATE_DELAY        2000/* ms */
#define AUTOCIT_LCD_UPDATE_COUNT_MAX    7

typedef struct
{
	int color_num;
	GUI_COLOR_T* colors;
}AUTOCIT_LCD_UPDATE_PARAM;

static GUI_LCD_DEV_INFO s_autocit_lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
static GUI_RECT_T s_autocit_lcd_rect = {0};
static GUI_RECT_T s_autocit_lcd_top_rect = {0};
static GUI_RECT_T s_autocit_lcd_bottom_rect = {0};

static GUI_RECT_T s_autocit_lcd_draw8x8rect = {0};

static MMI_HANDLE_T s_autocit_win_handle = NULL;
static AUTOCIT_LCD_TYPE s_autocit_lcd_type = autocit_lcd_type_white;
static uint8 s_autocit_lcd_timer_id = 0;
static uint8 s_autocit_update_count = 0;

static GUI_COLOR_T color_white[]        = {AUTOCIT_LCD_GUI_COLOR_WHITE};
static GUI_COLOR_T color_black[]        = {AUTOCIT_LCD_GUI_COLOR_BLACK};
static GUI_COLOR_T color_red[]          = {AUTOCIT_LCD_GUI_COLOR_RED};
static GUI_COLOR_T color_green[]        = {AUTOCIT_LCD_GUI_COLOR_GREEN};
static GUI_COLOR_T color_blue[]         = {AUTOCIT_LCD_GUI_COLOR_BLUE};
static GUI_COLOR_T color_white_black[]  = {AUTOCIT_LCD_GUI_COLOR_WHITE, AUTOCIT_LCD_GUI_COLOR_BLACK};
static GUI_COLOR_T color_black_white[]  = {AUTOCIT_LCD_GUI_COLOR_BLACK, AUTOCIT_LCD_GUI_COLOR_WHITE};
static GUI_COLOR_T color_8X8_grid[]     = {AUTOCIT_LCD_GUI_COLOR_BLACK, AUTOCIT_LCD_GUI_COLOR_WHITE};

static AUTOCIT_LCD_UPDATE_PARAM update_parame[] =
{
    {0, NULL},
    {1, color_white},
    {1, color_black},
    {1, color_red},
    {1, color_green},
    {1, color_blue},
    {2, color_white_black},
    {2, color_black_white},
    {2, color_8X8_grid},
};

/*****************************************************************************/
// 	Description : AUTOCIT_LCD_HandleAutoCitLcdWinMsg
//	Global resource dependence : 
//  Author: Cheney Wu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E AUTOCIT_LCD_HandleAutoCitLcdWinMsg(
                                                      MMI_WIN_ID_T win_id,
                                                      MMI_MESSAGE_ID_E msg_id,
                                                      DPARAM param
                                                      );


/*****************************************************************************/
// 	Description : AUTOCIT_HandleAutoCitWinMsg
//	Global resource dependence : 
//  Author: Cheney Wu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E AUTOCIT_HandleAutoCitWinMsg(
                                                   MMI_WIN_ID_T win_id,
                                                   MMI_MESSAGE_ID_E msg_id,
                                                   DPARAM param
                                                   );


WINDOW_TABLE(AUTOCIT_WIN_TAB) =
{
    WIN_FUNC((uint32)AUTOCIT_HandleAutoCitWinMsg),
    WIN_PRIO( WIN_TWO_LEVEL ),
    WIN_ID(AUTOCIT_WIN_ID),
//    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_HIDE_STATUS,
    END_WIN
};

WINDOW_TABLE(AUTOCIT_LCD_WIN_TAB) =
{
    WIN_FUNC((uint32)AUTOCIT_LCD_HandleAutoCitLcdWinMsg),
    WIN_PRIO( WIN_TWO_LEVEL ),
    WIN_ID(AUTOCIT_LCD_WIN_ID),
//    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_HIDE_STATUS,
    END_WIN
};

/*****************************************************************************/
// 	Description : AUTOCIT_LCD_ConvertKey
//	Global resource dependence : 
//  Author: Cheney Wu
//	Note:
/*****************************************************************************/
LOCAL MMI_MESSAGE_ID_E AUTOCIT_LCD_ConvertKey(MMI_MESSAGE_ID_E msg_id)
{
    if (MMK_IsKeyMsg(msg_id))
    {
        uint8 key_code = (uint8)(msg_id & 0xff);

        key_code = MMIDEFAULT_ConvertQwertyKeyToTypicalKey(key_code);

        msg_id = (msg_id & 0xff00) | key_code;
    }

    return msg_id;
}

/*****************************************************************************/
// 	Description : AUTOCIT_LCD_Draw8X8Grid
//	Global resource dependence : 
//  Author: Cheney Wu
//	Note:
/*****************************************************************************/
LOCAL void AUTOCIT_LCD_Draw8X8Grid(void)
{ 
    uint16 gridwight = (s_autocit_lcd_rect.right + 1)/8;
    uint16 gridheight = (s_autocit_lcd_rect.bottom + 1)/8;
    GUI_COLOR_T draw_color[2]={AUTOCIT_LCD_GUI_COLOR_BLACK,AUTOCIT_LCD_GUI_COLOR_WHITE};
    int color_idx = 0;
    
    uint16 irow = 0;
    uint16 icolume = 0;
    
    for(irow = 0;irow < 8;irow ++)
    {   
        color_idx = irow%2;
        for(icolume = 0;icolume < 8;icolume ++)
        {    
            s_autocit_lcd_draw8x8rect.left = irow * gridwight;
            s_autocit_lcd_draw8x8rect.top = icolume * gridheight;
            s_autocit_lcd_draw8x8rect.right = s_autocit_lcd_draw8x8rect.left + gridwight;
            s_autocit_lcd_draw8x8rect.bottom = s_autocit_lcd_draw8x8rect.top + gridheight;
            LCD_FillRect(&s_autocit_lcd_dev_info,s_autocit_lcd_draw8x8rect,draw_color[color_idx]);
            color_idx ++;
            color_idx = color_idx % 2; 
        }
    }
}

/*****************************************************************************/
// 	Description : AUTOCIT_LCD_FillRectWithColor
//	Global resource dependence : 
//  Author: Cheney Wu
//	Note:
/*****************************************************************************/
LOCAL void AUTOCIT_LCD_FillRectWithColor(int num, ...)
{
    va_list ap;
    GUI_COLOR_T* pcolor;
    va_start(ap, num);
    
    pcolor = va_arg(ap, GUI_COLOR_T*); /* get color array address */
    
    switch(num)
    {
    case 1:
        {
            GUI_COLOR_T color = *pcolor;
            SCI_TRACE_LOW("AUTOCIT_LCD_FillRectWithColor() 1 color[0x%x]", color);
            LCD_FillRect(&s_autocit_lcd_dev_info, s_autocit_lcd_rect, color);
        }
        break;
    case 2:
        {
            GUI_COLOR_T color1 = *pcolor++;
            GUI_COLOR_T color2 = *pcolor;
            SCI_TRACE_LOW("AUTOCIT_LCD_FillRectWithColor() 2 color1[0x%x] color2[0x%x]", color1, color2);
            LCD_FillRect(&s_autocit_lcd_dev_info, s_autocit_lcd_top_rect, color1);
            LCD_FillRect(&s_autocit_lcd_dev_info, s_autocit_lcd_bottom_rect, color2);
        }
        break;
    default:
        break;
    }
    
    va_end(ap);
}

/*****************************************************************************/
// 	Description : AUTOCIT_HandleAutoCitWinMsg
//	Global resource dependence : 
//  Author: Cheney Wu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E AUTOCIT_HandleAutoCitWinMsg(
                                                   MMI_WIN_ID_T win_id,
                                                   MMI_MESSAGE_ID_E msg_id,
                                                   DPARAM param
                                                   )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    SCI_TRACE_LOW("AUTOCIT_HandleAutoCitWinMsg() msg_id[0x%x]", msg_id);
    
    msg_id = AUTOCIT_LCD_ConvertKey(msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMIDEFAULT_TurnOnLCDBackLight();
            MMIDEFAULT_AllowTurnOffBackLight(SCI_FALSE);
        }
        break;
        
        
    case MSG_GET_FOCUS:
    case MSG_LOSE_FOCUS:
    case MSG_FULL_PAINT:
        break;
    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowTurnOffBackLight(SCI_TRUE);
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_DEL:
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
    case MSG_KEYDOWN_STAR:
    case MSG_KEYDOWN_0:
    case MSG_KEYDOWN_HASH:
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_MIDDLE:
    case MSG_KEYDOWN_WEB:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : AUTOCIT_LCD_HandleAutoCitLcdWinMsg
//	Global resource dependence : 
//  Author: Cheney Wu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E AUTOCIT_LCD_HandleAutoCitLcdWinMsg(
                                                      MMI_WIN_ID_T win_id,
                                                      MMI_MESSAGE_ID_E msg_id,
                                                      DPARAM param
                                                      )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    SCI_TRACE_LOW("AUTOCIT_LCD_HandleAutoCitLcdWinMsg() msg_id[0x%x]", msg_id);
    
    msg_id = AUTOCIT_LCD_ConvertKey(msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint16 lcd_width = 0;
            uint16 lcd_height = 0;
            
            s_autocit_lcd_type = (AUTOCIT_LCD_TYPE)MMK_GetWinAddDataPtr(win_id);
            
            SCI_TRACE_LOW("AUTOCIT_LCD_HandleAutoCitLcdWinMsg() MSG_OPEN_WINDOW s_autocit_lcd_type[%d]", s_autocit_lcd_type);
            
            GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
            s_autocit_lcd_rect.right  = (int16)(lcd_width - 1);
            s_autocit_lcd_rect.bottom = (int16)(lcd_height - 1);
            
            s_autocit_lcd_top_rect = s_autocit_lcd_bottom_rect = s_autocit_lcd_rect;
            
            s_autocit_lcd_draw8x8rect = s_autocit_lcd_rect;
            
            s_autocit_lcd_top_rect.bottom = (int16)((lcd_height / 2) - 1);
            s_autocit_lcd_bottom_rect.top = (int16)(lcd_height / 2);
            
            s_autocit_lcd_timer_id = MMK_CreateWinTimer(win_id, 20, FALSE);
            s_autocit_update_count = 0;
            MMIDEFAULT_TurnOnLCDBackLight();
            MMIDEFAULT_AllowTurnOffBackLight(SCI_FALSE);
        }
        break;
        
    case MSG_AUTOCIT_LCD_UPDATE:
        {
            s_autocit_lcd_type = (AUTOCIT_LCD_TYPE)param;
            
            MMK_StopTimer(s_autocit_lcd_timer_id);
            s_autocit_lcd_timer_id = MMK_CreateWinTimer(win_id, 20, FALSE);
            s_autocit_update_count = 0;
        }
        break;
        
    case MSG_TIMER:
        if (*(uint8*)param == s_autocit_lcd_timer_id)
        {
            SCI_TRACE_LOW("AUTOCIT_LCD_HandleAutoCitLcdWinMsg() MSG_TIMER s_autocit_lcd_type[%d]", s_autocit_lcd_type);
            MMK_SendMsg(s_autocit_win_handle, MSG_FULL_PAINT, NULL);
            if (autocit_lcd_type_all == s_autocit_lcd_type)
            {
                SCI_TRACE_LOW("AUTOCIT_LCD_HandleAutoCitLcdWinMsg() MSG_TIMER s_autocit_update_count[%d]", s_autocit_update_count);
                if (s_autocit_update_count < AUTOCIT_LCD_UPDATE_COUNT_MAX)
                {
                    s_autocit_lcd_timer_id = MMK_CreateWinTimer(win_id, AUTOCIT_LCD_UPDATE_DELAY, FALSE);
                    s_autocit_update_count++;
                }
            }
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_GET_FOCUS:
    case MSG_FULL_PAINT:
        switch (s_autocit_lcd_type)
        {
        case autocit_lcd_type_white:
        case autocit_lcd_type_black:
        case autocit_lcd_type_red:
        case autocit_lcd_type_green:
        case autocit_lcd_type_blue:
        case autocit_lcd_type_white_black:
        case autocit_lcd_type_black_white:
            {
                AUTOCIT_LCD_UPDATE_PARAM cur_param = update_parame[s_autocit_lcd_type];
                AUTOCIT_LCD_FillRectWithColor(cur_param.color_num, cur_param.colors);
                break;
            }
        case autocit_lcd_type_8X8_grid:
            {
                AUTOCIT_LCD_Draw8X8Grid();
                break;
            }
        case autocit_lcd_type_all:
            {
                if (s_autocit_update_count < AUTOCIT_LCD_UPDATE_COUNT_MAX)
                {
                    AUTOCIT_LCD_UPDATE_PARAM cur_param = update_parame[s_autocit_update_count];
                    AUTOCIT_LCD_FillRectWithColor(cur_param.color_num, cur_param.colors);
                }
                break;
            }
        default:
            break;
        }
        break;
    case MSG_CLOSE_WINDOW:
        s_autocit_win_handle = NULL;
        MMIDEFAULT_AllowTurnOffBackLight(SCI_TRUE);
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_DEL:
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
    case MSG_KEYDOWN_STAR:
    case MSG_KEYDOWN_0:
    case MSG_KEYDOWN_HASH:
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_MIDDLE:
    case MSG_KEYDOWN_WEB:
    case MSG_AUTOCIT_LCD_CLOSE:
        MMK_StopTimer(s_autocit_lcd_timer_id);
        s_autocit_lcd_timer_id = 0;
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : AUTOCIT_OpenMainWin
//	Global resource dependence : 
//  Author: Cheney Wu
//	Note:
/*****************************************************************************/
PUBLIC void AUTOCIT_OpenMainWin(void)
{
    if (MMK_CreateWin((uint32 *)AUTOCIT_WIN_TAB, PNULL) == 0)
    {
        SCI_TRACE_LOW("AUTOCIT_OpenMainWin() create win failed!!!");
    }
}

/*****************************************************************************/
// 	Description : AUTOCIT_LCD_OpenMainWin
//	Global resource dependence : 
//  Author: Cheney Wu
//	Note:
/*****************************************************************************/
PUBLIC void AUTOCIT_LCD_OpenMainWin(AUTOCIT_LCD_TYPE type)
{
    if (NULL == s_autocit_win_handle)
    {
        s_autocit_win_handle = MMK_CreateWin((uint32 *)AUTOCIT_LCD_WIN_TAB, (ADD_DATA)type);
        if (s_autocit_win_handle == 0)
        {
            SCI_TRACE_LOW("AUTOCIT_LCD_OpenMainWin() create win failed!!!");
        }
    }
    else
    {
        SCI_TRACE_LOW("AUTOCIT_LCD_OpenMainWin() win exist!!!");
        if (s_autocit_lcd_type != autocit_lcd_type_all)
        {
            s_autocit_lcd_type = type;
            MMK_SendMsg(s_autocit_win_handle, MSG_AUTOCIT_LCD_UPDATE, (DPARAM)type);
        }
    }
}

/*****************************************************************************/
// 	Description : AUTOCIT_LCD_CloseMainWin
//	Global resource dependence : 
//  Author: Cheney Wu
//	Note:
/*****************************************************************************/
PUBLIC void AUTOCIT_LCD_CloseMainWin(void)
{
    if (s_autocit_win_handle != NULL)
    {
        MMK_SendMsg(s_autocit_win_handle, MSG_AUTOCIT_LCD_CLOSE, NULL);
    }
}

