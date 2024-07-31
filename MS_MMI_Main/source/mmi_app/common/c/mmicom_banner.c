/****************************************************************************
 ** File Name:      mmicom_banner.c                                          *
 ** Author:         Yang.Yang                                                *
 ** Date:           07/04/2015                                               *
 ** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.      *
 ** Description:    This file is used to describe the application modual.    *
 ****************************************************************************
 **                         Important Edit History                          *
 ** ------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                             *
 ** 4/2015        Yang.Yang            Create
 ** 
 ****************************************************************************/
#define MMICOM_BANNER_C

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guires.h"
#include "window_parse.h"
#include "mmi_id.h"
#include "guilcd.h"
#include "guiblock.h"
#include "guistring.h"
#include "mmidisplay_data.h"
#include "mmi_common.h"
#include "mmk_timer.h"
#include "mmicom_banner.h"
#include "ui_layer.h"
#include "guigraph.h"
#include "mmi_position.h"
#include "mmi_theme.h"
#include "mmi_appmsg.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
#define MMI_BANNER_5SECONDS 5000

#define MMI_BANNER_TEXT_FONT MMI_DEFAULT_SMALL_FONT
#define MMI_BANNER_TEXT_COLOR MMI_WHITE_COLOR
#define MMI_BANNER_BACKGROUND_COLOR MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_2)
#define MMI_BANNER_LINE_COLOR MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_3)

#define MMI_TEXT_BANNER_TEXT_HEIGHT 28 //14
#if defined (MAINLCD_SIZE_128X160)
#define MMI_TEXT_BANNER_TEXT_WIDTH 120
#else
#define MMI_TEXT_BANNER_TEXT_WIDTH 228
#endif
#define MMI_TEXT_BANNER_V_MARGE 2
#define MMI_TEXT_BANNER_H_MARGE 6

#define MMI_ICONTEXT_BANNER_HEIGHT 56
#define MMI_ICONTEXT_BANNER_ICON_SIZE 24
#define MMI_ICONTEXT_BANNER_TEXT_HEIGHT 24
#define MMI_ICONTEXT_BANNER_TEXT_V_MARGE 4
#define MMI_ICONTEXT_BANNER_ICON_V_MARGE 6
#define MMI_ICONTEXT_BANNER_H_MARGE 6
#define MMI_BANNER_LINE_SPACE 4

/**---------------------------------------------------------------------------*
 **                          TYPES/CONSTANTS                                *
 **---------------------------------------------------------------------------*/
typedef struct  
{
    MMI_WIN_ID_T      parent_win_id;
    MMI_BANNER_TYPE_E banner_type;
    GUI_RECT_T        banner_rect;
    GUI_RECT_T        text1_rect;
    GUI_RECT_T        text2_rect;
    GUI_RECT_T        image_rect;
    MMI_TEXT_ID_T     text1_id;
    MMI_TEXT_ID_T     text2_id;
    MMI_IMAGE_ID_T    image_id;
    GUI_COLOR_T       background_color;
    GUI_LCD_DEV_INFO  dev_layer_handle;
    uint8             control_banner_timer_id;
    MMI_STRING_T      text1_str;//add
    MMI_STRING_T      text2_str;
    MMI_HANDLE_T      banner_win;
}MMICOM_BANNER_T;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMICOM_BANNER_T s_banner_para = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBannerWindow(
        MMI_WIN_ID_T      win_id, 
        MMI_MESSAGE_ID_E  msg_id, 
        DPARAM       param
        );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void InitControlPara(MMICOM_BANNER_INFO_T banner_info);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void DrawControlBanner(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void StartControlBannerTimer(void);

/*****************************************************************************/
//  Description : stop control panel timer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void StopControlBannerTimer(void);

/*****************************************************************************/
//  Description :show banner
//  Global resource dependence : 
//  Author:
//  Note:The win_id is parent window id
/*****************************************************************************/
#ifdef UI_MULTILAYER_SUPPORT
LOCAL void DisplayControlBanner(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description :timer call back
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void BannerTimerCallBack(uint8 timer_id,uint32 param);
#endif
/*****************************************************************************/
//  Description : close banner
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
LOCAL void CloseBanner(void);

/*****************************************************************************/
//  Description : Check banner is needed to paint when open banner
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN CheckBannerNeedPaint(MMICOM_BANNER_INFO_T *banner_info, MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : close and creeate barnner .FOR: New Control are smaller than old controls
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN ReCreateBarnner(MMICOM_BANNER_INFO_T banner_info, MMI_WIN_ID_T win_id);
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetBltLayer(const GUI_LCD_DEV_INFO *lcd_info_ptr);
#endif
/**--------------------------------------------------------------------------*
 **                         WINDOW  DEFINITION                               *
 **--------------------------------------------------------------------------*/
WINDOW_TABLE(MMICOMMON_BANNER_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleBannerWindow),    
    WIN_ID( MMICOMMON_BANNER_WIN_ID ),
    WIN_STYLE(WS_NO_DEFAULT_STYLE | WS_TOOL_WIN | WS_NEED_PAINT_PRE_WIN),
    WIN_MOVE_STYLE(MOVE_NOT_FULL_SCREEN_WINDOW),
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
typedef enum {
    BNR_CREATE,
    BNR_PAINT,
    BNR_FROZE,
    BNR_CLOSE
} BNR_STATUS;

LOCAL BNR_STATUS s_banner_status = BNR_CLOSE;
LOCAL LCD_ANGLE_E s_lcd_ang = LCD_ANGLE_0;

LOCAL MMI_RESULT_E  HandleBannerWindow(
        MMI_WIN_ID_T      win_id, 
        MMI_MESSAGE_ID_E  msg_id, 
        DPARAM       param
        )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            {	        
                MMK_SetWinRect(win_id, &s_banner_para.banner_rect);
                MMK_SetWinPriority(win_id, MMK_GetWinPriority(s_banner_para.parent_win_id));
                StartControlBannerTimer();
                s_banner_status = BNR_CREATE;
                SCI_TraceLow("[mmi-banner] -- HandleBannerWindow :: MSG_OPEN_WINDOW [win id = %x]", win_id);
            }
            break;

        case MSG_FULL_PAINT:
            DrawControlBanner(win_id);
            s_banner_status = BNR_PAINT;
            s_lcd_ang = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);
            SCI_TraceLow("[mmi-banner] -- HandleBannerWindow :: MSG_FULL_PAINT [win id : %x]", win_id);
/*
            if(MMK_GetPrevWinId(win_id))
            {
                SCI_TraceLow("[mmi-banner] -- HandleBannerWindow :: MSG_FULL_PAINT -- set focus [%x]", MMK_GetPrevWinId(win_id));

                MMK_SendMsg(MMK_GetPrevWinId(win_id), MSG_GET_FOCUS, PNULL);   
            }
*/
            break;

        case MSG_TIMER:
            SCI_TraceLow("[mmi-banner] -- HandleBannerWindow :: MSG_TIMER");

            if(s_banner_para.control_banner_timer_id == *((uint8 *)param))
            {
                StopControlBannerTimer();
                MMK_CloseWin(win_id); 
            }
            else
            {
                recode = MMI_RESULT_FALSE;
            }
            break;

        case MSG_CLOSE_WINDOW:
            SCI_TraceLow("[mmi-banner] -- HandleBannerWindow :: MSG_CLOSE_WINDOW");
            s_banner_status = BNR_CLOSE;
            StopControlBannerTimer();
            MMK_SendMsg(s_banner_para.parent_win_id, MSG_COMMON_BANNER_CLOSE, PNULL);
            if(PNULL != s_banner_para.text1_str.wstr_ptr)
            {
                SCI_Free(s_banner_para.text1_str.wstr_ptr);
                s_banner_para.text1_str.wstr_ptr = PNULL;
            }
            if(PNULL != s_banner_para.text2_str.wstr_ptr)
            {
                SCI_Free(s_banner_para.text2_str.wstr_ptr);
                s_banner_para.text2_str.wstr_ptr = PNULL;
            }
            break;

        case MSG_APP_OK:
        case MSG_APP_CANCEL:
        case MSG_APP_WEB:
        case MSG_LOSE_FOCUS:
        case MSG_APP_DOWN:
            //yuguang.liu
        case MSG_APP_0:
        case MSG_APP_1:
        case MSG_APP_2:
        case MSG_APP_3:
        case MSG_APP_4:
        case MSG_APP_5:
        case MSG_APP_6:
        case MSG_APP_7:
        case MSG_APP_8:
        case MSG_APP_9:
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
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
        case MSG_APP_Z:	
        case MSG_APP_X:	
        case MSG_APP_C:	
        case MSG_APP_V: 
        case MSG_APP_B:	
        case MSG_APP_N:	
        case MSG_APP_M:	
        case MSG_APP_COMMA:	
        case MSG_APP_PERIOD:	
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
        case MSG_APP_HASH:
        case MSG_APP_STAR:
        case MSG_APP_LEFT:
        case MSG_APP_RIGHT:
        case MSG_APP_UP:
            //yuguang.liu
            if(PNULL != s_banner_para.text1_str.wstr_ptr)
            {
                SCI_Free(s_banner_para.text1_str.wstr_ptr);
                s_banner_para.text1_str.wstr_ptr = PNULL;
            }
            if(PNULL != s_banner_para.text2_str.wstr_ptr)
            {
                SCI_Free(s_banner_para.text2_str.wstr_ptr);
                s_banner_para.text2_str.wstr_ptr = PNULL;
            }
            MMK_CloseWin(win_id);
            SCI_TraceLow("[mmi-banner] -- HandleBannerWindow :: some msg -- post to [%x]", MMK_GetFocusWinId());

            MMK_SendMsg(MMK_GetFocusWinId(), msg_id, param);
            break;

        case MSG_KEYUP_RED:
#ifdef MMI_MSTYLE_SUPPORT            
            if((GetStartupLockStatus()))
            {
                MMK_CloseWin(win_id); 
            }
            else
#endif                
            {
                recode = MMI_RESULT_FALSE;
            }
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

PUBLIC void ExceptBannerRegion(GUI_RECT_T *rect_ptr)
{
    if (s_banner_status == BNR_PAINT)
    {
        SCI_TraceLow("[mmi-banner] -- ExceptBannerRegion  [s_banner_status == paint]");
        s_banner_status = BNR_FROZE;
        return;
    }
    else if (s_banner_status == BNR_FROZE)
    {
        if (s_lcd_ang != GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID))
        {
            SCI_TraceLow("[mmi-banner] -- ExceptBannerRegion LCD switched redraw [s_banner_status == froze]");
            MMK_CloseWin(MMICOMMON_BANNER_WIN_ID);
            return;
        }
        SCI_TraceLow("[mmi-banner] -- ExceptBannerRegion  [s_banner_status == froze]");
        if (rect_ptr->top < s_banner_para.banner_rect.bottom)
            rect_ptr->top = s_banner_para.banner_rect.bottom;
        return;
    }
    SCI_TraceLow("[mmi-banner] -- ExceptBannerRegion  [s_banner_status == %d]", s_banner_status);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 默认居中； 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_OpenBannerWin(MMI_WIN_ID_T win_id,
        MMICOM_BANNER_INFO_T banner_info)
{
#ifdef UI_MULTILAYER_SUPPORT   
    if(PNULL != s_banner_para.text1_str.wstr_ptr)
    {
        SCI_Free(s_banner_para.text1_str.wstr_ptr);
        s_banner_para.text1_str.wstr_ptr = PNULL;
    }
    if(PNULL != s_banner_para.text2_str.wstr_ptr)
    {
        SCI_Free(s_banner_para.text2_str.wstr_ptr);
        s_banner_para.text2_str.wstr_ptr = PNULL;
    }
    s_banner_para.parent_win_id = win_id;

    InitControlPara(banner_info);
    DisplayControlBanner(MAIN_IDLE_WIN_ID);

#else
    BOOLEAN  needed_paint = CheckBannerNeedPaint(&banner_info, win_id);
    if(PNULL != s_banner_para.text1_str.wstr_ptr)
    {
        SCI_Free(s_banner_para.text1_str.wstr_ptr);
        s_banner_para.text1_str.wstr_ptr = PNULL;
    }
    if(PNULL != s_banner_para.text2_str.wstr_ptr)
    {
        SCI_Free(s_banner_para.text2_str.wstr_ptr);
        s_banner_para.text2_str.wstr_ptr = PNULL;
    }
    s_banner_para.parent_win_id = win_id;

    InitControlPara(banner_info);

    if(MMK_IsOpenWin(MMICOMMON_BANNER_WIN_ID))
    {
        GUI_RECT_T rc;
        MMK_GetWinRect(s_banner_para.banner_win, &rc);
        if (!GUI_EqualRect(rc, s_banner_para.banner_rect))
        {
            if ((rc.bottom - rc.top) > (s_banner_para.banner_rect.bottom -s_banner_para.banner_rect.top))
            {
                ReCreateBarnner( banner_info, win_id);
            }
            MMK_SetWinRect(s_banner_para.banner_win, &s_banner_para.banner_rect);
            if (needed_paint)
            {
                MMK_SendMsg(s_banner_para.banner_win, MSG_FULL_PAINT, PNULL);
            }
        }
        else if (needed_paint)
        {
            MMK_SendMsg(s_banner_para.banner_win, MSG_FULL_PAINT, PNULL);
        }
        
        //add by qin.he for buglist id: 1060849
        StopControlBannerTimer();
        StartControlBannerTimer();
    }
    else 
    {
        s_banner_para.banner_win = MMK_CreateWin((uint32*)MMICOMMON_BANNER_WIN_TAB, PNULL);
    }
    MMITHEME_SetUpdateDelayCount (0);

#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : close and creeate barnner .FOR: New Control are smaller than old controls
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN ReCreateBarnner(MMICOM_BANNER_INFO_T banner_info, MMI_WIN_ID_T win_id)
{
    
    MMIAPICOM_CloseBanner();

    s_banner_para.parent_win_id = win_id;
    
    s_banner_para.banner_win = MMK_CreateWin((uint32*)MMICOMMON_BANNER_WIN_TAB, PNULL);
    
    InitControlPara(banner_info);

    return TRUE;
}
/*****************************************************************************/
//  Description : Check banner is needed to paint when open banner
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN CheckBannerNeedPaint(MMICOM_BANNER_INFO_T *banner_info, MMI_WIN_ID_T win_id)
{
    if (banner_info == NULL)
        return TRUE;

    if (MMK_IsOpenWin(MMICOMMON_BANNER_WIN_ID) && banner_info->banner_type == s_banner_para.banner_type)
    {
            switch (s_banner_para.banner_type)
            {
                case MMI_BANNER_TEXT:
                    if (s_banner_para.text1_id == banner_info->text1_id 
                        && s_banner_para.banner_rect.left == banner_info->start_point.x 
                        && s_banner_para.banner_rect.top == banner_info->start_point.y
                        && win_id == s_banner_para.parent_win_id)
                        return FALSE;
                    break;
                case MMI_BANNER_ICONTEXT:
                case MMI_BANNER_STR_TEXT:
                case MMI_BANNER_STR_ICONTEXT:
                    //if needed ,to do
                    break;
                default:
                    break;
            }       
    }
    return TRUE;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void InitControlPara(MMICOM_BANNER_INFO_T banner_info)
{
    uint16     lcd_width = 0;
    uint16     text_width = 0;
    uint16     line_num = 0;
    GUI_RECT_T default_rect = {0};
    GUISTR_STYLE_T str_style = {0};
    GUISTR_STATE_T str_state = GUISTR_STATE_WORDBREAK;
    MMI_STRING_T str_info = {0}; 

    lcd_width = MMITHEME_GetClientRectWidth();
    s_banner_para.banner_type = banner_info.banner_type;
    s_banner_para.background_color = MMI_BANNER_BACKGROUND_COLOR;
#ifndef UI_MULTILAYER_SUPPORT   
    s_banner_para.dev_layer_handle.lcd_id = 0;
    s_banner_para.dev_layer_handle.block_id = 0;
#endif
    switch(s_banner_para.banner_type)
    {
        case MMI_BANNER_TEXT:
            {
                s_banner_para.text1_id = banner_info.text1_id;
                s_banner_para.text2_id = TXT_NULL;
                s_banner_para.image_id = IMAGE_NULL;
                s_banner_para.text2_rect = default_rect;
                s_banner_para.image_rect = default_rect;

                str_style.font = MMI_BANNER_TEXT_FONT;
                MMI_GetLabelTextByLang(s_banner_para.text1_id, &str_info);
                //text_width = GUISTR_GetStringWidth(&str_style, &str_info, str_state);
                //line_num = (text_width + MMI_TEXT_BANNER_TEXT_WIDTH - 1) / MMI_TEXT_BANNER_TEXT_WIDTH;
                line_num =  GUI_CalculateStringLinesByPixelNum(
                        MMI_TEXT_BANNER_TEXT_WIDTH,
                        str_info.wstr_ptr,
                        str_info.wstr_len,
                        str_style.font,
                        0,
                        TRUE
                        );

                s_banner_para.banner_rect.left = banner_info.start_point.x;
                s_banner_para.banner_rect.top = banner_info.start_point.y;
                s_banner_para.banner_rect.right = lcd_width - 1;
                s_banner_para.banner_rect.bottom = s_banner_para.banner_rect.top + line_num*MMI_TEXT_BANNER_TEXT_HEIGHT + 2*MMI_TEXT_BANNER_V_MARGE + (line_num-1)*MMI_BANNER_LINE_SPACE- 1;

                s_banner_para.text1_rect.left = s_banner_para.banner_rect.left + MMI_TEXT_BANNER_H_MARGE;
                s_banner_para.text1_rect.top = s_banner_para.banner_rect.top + MMI_TEXT_BANNER_V_MARGE;
                s_banner_para.text1_rect.right = s_banner_para.banner_rect.right - MMI_TEXT_BANNER_H_MARGE;
                s_banner_para.text1_rect.bottom = s_banner_para.banner_rect.bottom - MMI_TEXT_BANNER_V_MARGE;
            }
            break;

        case MMI_BANNER_ICONTEXT:
            {
                s_banner_para.banner_rect.left = banner_info.start_point.x;
                s_banner_para.banner_rect.top = banner_info.start_point.y;
                s_banner_para.banner_rect.right = lcd_width - 1;
                s_banner_para.banner_rect.bottom = s_banner_para.banner_rect.top + MMI_ICONTEXT_BANNER_HEIGHT - 1;

                s_banner_para.image_rect.left = s_banner_para.banner_rect.left + MMI_ICONTEXT_BANNER_H_MARGE;
                s_banner_para.image_rect.top = s_banner_para.banner_rect.top + MMI_ICONTEXT_BANNER_ICON_V_MARGE;
                s_banner_para.image_rect.right = s_banner_para.image_rect.left + MMI_ICONTEXT_BANNER_ICON_SIZE - 1;
                s_banner_para.image_rect.bottom = s_banner_para.image_rect.top + MMI_ICONTEXT_BANNER_ICON_SIZE - 1; 

                s_banner_para.text1_rect.left = s_banner_para.image_rect.right + MMI_ICONTEXT_BANNER_H_MARGE + 1;
                s_banner_para.text1_rect.top = s_banner_para.banner_rect.top + MMI_ICONTEXT_BANNER_TEXT_V_MARGE;
                s_banner_para.text1_rect.right = s_banner_para.banner_rect.right - MMI_ICONTEXT_BANNER_H_MARGE;
                s_banner_para.text1_rect.bottom = s_banner_para.text1_rect.top + MMI_ICONTEXT_BANNER_TEXT_HEIGHT - 1;

                s_banner_para.text2_rect.left = s_banner_para.text1_rect.left;
                s_banner_para.text2_rect.top = s_banner_para.text1_rect.bottom + 1;
                s_banner_para.text2_rect.right = s_banner_para.text1_rect.right;
                s_banner_para.text2_rect.bottom = s_banner_para.text2_rect.top + MMI_ICONTEXT_BANNER_TEXT_HEIGHT - 1;

                s_banner_para.text1_id = banner_info.text1_id;
                s_banner_para.text2_id = banner_info.text2_id;
                s_banner_para.image_id = banner_info.image_id;
            }
            break;
        case MMI_BANNER_STR_TEXT:
            {
                s_banner_para.text1_str.wstr_ptr = SCI_ALLOC_APPZ((banner_info.text1_str.wstr_len + 1) * sizeof(wchar));
                //SCI_MEMSET(s_banner_para.text1_str.wstr_ptr,0,((banner_info.text1_str.wstr_len + 1) * sizeof(wchar)));
                if ( PNULL == s_banner_para.text1_str.wstr_ptr )
                {
                    return;
                }
                MMI_MEMCPY(s_banner_para.text1_str.wstr_ptr,(banner_info.text1_str.wstr_len * sizeof(wchar)),
                        banner_info.text1_str.wstr_ptr,(banner_info.text1_str.wstr_len * sizeof(wchar)),
                        (banner_info.text1_str.wstr_len * sizeof(wchar)));
                s_banner_para.text1_str.wstr_len = banner_info.text1_str.wstr_len;
                s_banner_para.text2_str.wstr_ptr = PNULL;
                s_banner_para.text2_str.wstr_len = 0;
                s_banner_para.image_id = IMAGE_NULL;
                s_banner_para.text2_rect = default_rect;
                s_banner_para.image_rect = default_rect;

                str_style.font = MMI_BANNER_TEXT_FONT;
                //text_width = GUISTR_GetStringWidth(&str_style, &(s_banner_para.text1_str), str_state);
                //line_num = (text_width + MMI_TEXT_BANNER_TEXT_WIDTH - 1) / MMI_TEXT_BANNER_TEXT_WIDTH;
                line_num =  GUI_CalculateStringLinesByPixelNum(
                        MMI_TEXT_BANNER_TEXT_WIDTH,
                        s_banner_para.text1_str.wstr_ptr,
                        s_banner_para.text1_str.wstr_len,
                        str_style.font,
                        0,
                        TRUE
                        );

                s_banner_para.banner_rect.left = banner_info.start_point.x;
                s_banner_para.banner_rect.top = banner_info.start_point.y;
                s_banner_para.banner_rect.right = lcd_width - 1;
                s_banner_para.banner_rect.bottom = s_banner_para.banner_rect.top + line_num*MMI_TEXT_BANNER_TEXT_HEIGHT + 2*MMI_TEXT_BANNER_V_MARGE + (line_num-1)*MMI_BANNER_LINE_SPACE- 1;

                s_banner_para.text1_rect.left = s_banner_para.banner_rect.left + MMI_TEXT_BANNER_H_MARGE;
                s_banner_para.text1_rect.top = s_banner_para.banner_rect.top + MMI_TEXT_BANNER_V_MARGE;
                s_banner_para.text1_rect.right = s_banner_para.banner_rect.right - MMI_TEXT_BANNER_H_MARGE;
                s_banner_para.text1_rect.bottom = s_banner_para.banner_rect.bottom - MMI_TEXT_BANNER_V_MARGE;
            }
            break;
        case MMI_BANNER_STR_ICONTEXT:
            {
                s_banner_para.banner_rect.left = banner_info.start_point.x;
                s_banner_para.banner_rect.top = banner_info.start_point.y;
                s_banner_para.banner_rect.right = lcd_width - 1;
                s_banner_para.banner_rect.bottom = s_banner_para.banner_rect.top + MMI_ICONTEXT_BANNER_HEIGHT - 1;

                s_banner_para.image_rect.left = s_banner_para.banner_rect.left + MMI_ICONTEXT_BANNER_H_MARGE;
                s_banner_para.image_rect.top = s_banner_para.banner_rect.top + MMI_ICONTEXT_BANNER_ICON_V_MARGE;
                s_banner_para.image_rect.right = s_banner_para.image_rect.left + MMI_ICONTEXT_BANNER_ICON_SIZE - 1;
                s_banner_para.image_rect.bottom = s_banner_para.image_rect.top + MMI_ICONTEXT_BANNER_ICON_SIZE - 1; 

                s_banner_para.text1_rect.left = s_banner_para.image_rect.right + MMI_ICONTEXT_BANNER_H_MARGE + 1;
                s_banner_para.text1_rect.top = s_banner_para.banner_rect.top + MMI_ICONTEXT_BANNER_TEXT_V_MARGE;
                s_banner_para.text1_rect.right = s_banner_para.banner_rect.right - MMI_ICONTEXT_BANNER_H_MARGE;
                s_banner_para.text1_rect.bottom = s_banner_para.text1_rect.top + MMI_ICONTEXT_BANNER_TEXT_HEIGHT - 1;

                s_banner_para.text2_rect.left = s_banner_para.text1_rect.left;
                s_banner_para.text2_rect.top = s_banner_para.text1_rect.bottom + 1;
                s_banner_para.text2_rect.right = s_banner_para.text1_rect.right;
                s_banner_para.text2_rect.bottom = s_banner_para.text2_rect.top + MMI_ICONTEXT_BANNER_TEXT_HEIGHT - 1;

                s_banner_para.text1_str.wstr_ptr = SCI_ALLOC_APPZ((banner_info.text1_str.wstr_len + 1) * sizeof(wchar));
                //SCI_MEMSET(s_banner_para.text1_str.wstr_ptr,0,((banner_info.text1_str.wstr_len + 1) * sizeof(wchar)));
                if ( PNULL == s_banner_para.text1_str.wstr_ptr )
                {
                    return;
                }
                MMI_MEMCPY(s_banner_para.text1_str.wstr_ptr,(banner_info.text1_str.wstr_len * sizeof(wchar)),
                        banner_info.text1_str.wstr_ptr,(banner_info.text1_str.wstr_len * sizeof(wchar)),
                        (banner_info.text1_str.wstr_len * sizeof(wchar)));
                s_banner_para.text1_str.wstr_len = banner_info.text1_str.wstr_len;

                s_banner_para.text2_str.wstr_ptr = SCI_ALLOC_APPZ((banner_info.text2_str.wstr_len + 1) * sizeof(wchar));
                //SCI_MEMSET(s_banner_para.text2_str.wstr_ptr,0,((banner_info.text2_str.wstr_len + 1) * sizeof(wchar)));
                if ( PNULL == s_banner_para.text2_str.wstr_ptr )
                {
                    return;
                }
                MMI_MEMCPY(s_banner_para.text2_str.wstr_ptr,(banner_info.text2_str.wstr_len * sizeof(wchar)),
                        banner_info.text2_str.wstr_ptr,(banner_info.text2_str.wstr_len * sizeof(wchar)),
                        (banner_info.text2_str.wstr_len * sizeof(wchar)));
                s_banner_para.text2_str.wstr_len = banner_info.text2_str.wstr_len;
                s_banner_para.image_id = banner_info.image_id;
            }
            break;

        default:
            break;
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void DrawControlBanner(MMI_WIN_ID_T win_id)
{
    const GUI_LCD_DEV_INFO* lcd_dev_info_ptr = &s_banner_para.dev_layer_handle;
    MMI_STRING_T      text_str = {0};
    GUISTR_STYLE_T    text_style = {0};
    GUISTR_STATE_T    state = GUISTR_STATE_ALIGN|GUISTR_STATE_ALIGN_BY_DIR;

    text_style.align = ALIGN_LVMIDDLE;
    text_style.font = MMI_BANNER_TEXT_FONT;
    text_style.font_color = MMI_BANNER_TEXT_COLOR;
    text_style.line_space = MMI_BANNER_LINE_SPACE;

#ifdef UI_MULTILAYER_SUPPORT    
    SetBltLayer(lcd_dev_info_ptr);
    UILAYER_ClearRect(lcd_dev_info_ptr, s_banner_para.banner_rect);
#endif
    SCI_TraceLow("[sky-banner] -- HandleBannerWindow :: DrawControlBanner type = %d", s_banner_para.banner_type);

    switch(s_banner_para.banner_type)
    {
        case MMI_BANNER_TEXT:
            {
                //背景
                GUI_FillRect(lcd_dev_info_ptr, s_banner_para.banner_rect, s_banner_para.background_color);

                //文本
                MMI_GetLabelTextByLang(s_banner_para.text1_id, &text_str);
                state = state|GUISTR_STATE_WORDBREAK;//state | GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
                GUISTR_DrawTextToLCDInRect( 
                        lcd_dev_info_ptr,
                        (const GUI_RECT_T *)&s_banner_para.text1_rect,       //the fixed display area
                        (const GUI_RECT_T *)&s_banner_para.text1_rect,       //用户要剪切的实际区域
                        (const MMI_STRING_T *)&text_str,
                        &text_style,
                        state,
                        GUISTR_TEXT_DIR_AUTO
                        ); 

                //直线
                // 			LCD_DrawThickLine(lcd_dev_info_ptr, s_banner_para.banner_rect.left, s_banner_para.banner_rect.bottom - 1,
                // 							  s_banner_para.banner_rect.right, s_banner_para.banner_rect.bottom - 1, MMI_BANNER_LINE_COLOR, 1);
                LCD_DrawThickLineEx_NonCap(lcd_dev_info_ptr, s_banner_para.banner_rect.left, s_banner_para.banner_rect.bottom,
                        s_banner_para.banner_rect.right, s_banner_para.banner_rect.bottom, MMI_BANNER_LINE_COLOR, 1, GUILCD_PEN_STYLE_DEFAULT);
            }
            break;

        case MMI_BANNER_ICONTEXT:
            {
                //背景
                GUI_FillRect(lcd_dev_info_ptr, s_banner_para.banner_rect, s_banner_para.background_color);

                //文本1
                MMI_GetLabelTextByLang(s_banner_para.text1_id, &text_str);
                state = state | GUISTR_STATE_SINGLE_LINE;
                GUISTR_DrawTextToLCDInRect( 
                        lcd_dev_info_ptr,
                        (const GUI_RECT_T *)&s_banner_para.text1_rect,       //the fixed display area
                        (const GUI_RECT_T *)&s_banner_para.text1_rect,       //用户要剪切的实际区域
                        (const MMI_STRING_T *)&text_str,
                        &text_style,
                        state,
                        GUISTR_TEXT_DIR_AUTO
                        );

                //文本2
                MMI_GetLabelTextByLang(s_banner_para.text2_id, &text_str);
                GUISTR_DrawTextToLCDInRect( 
                        lcd_dev_info_ptr,
                        (const GUI_RECT_T *)&s_banner_para.text2_rect,       //the fixed display area
                        (const GUI_RECT_T *)&s_banner_para.text2_rect,       //用户要剪切的实际区域
                        (const MMI_STRING_T *)&text_str,
                        &text_style,
                        state,
                        GUISTR_TEXT_DIR_AUTO
                        );

                //图片
                GUIRES_DisplayImg(PNULL, &s_banner_para.image_rect, PNULL, win_id, s_banner_para.image_id, lcd_dev_info_ptr); 

                //直线
                LCD_DrawThickLineEx_NonCap(lcd_dev_info_ptr, s_banner_para.banner_rect.left, s_banner_para.banner_rect.bottom,
                        s_banner_para.banner_rect.right, s_banner_para.banner_rect.bottom, MMI_BANNER_LINE_COLOR, 1, GUILCD_PEN_STYLE_DEFAULT);
            }
            break;
        case MMI_BANNER_STR_TEXT:
            {
                //背景
                GUI_FillRect(lcd_dev_info_ptr, s_banner_para.banner_rect, s_banner_para.background_color);

                //文本
                text_str.wstr_ptr = s_banner_para.text1_str.wstr_ptr;
                text_str.wstr_len = s_banner_para.text1_str.wstr_len;
                state = state | GUISTR_STATE_WORDBREAK;
                GUISTR_DrawTextToLCDInRect( 
                        lcd_dev_info_ptr,
                        (const GUI_RECT_T *)&s_banner_para.text1_rect,       //the fixed display area
                        (const GUI_RECT_T *)&s_banner_para.text1_rect,       //用户要剪切的实际区域
                        (const MMI_STRING_T *)&text_str,
                        &text_style,
                        state,
                        GUISTR_TEXT_DIR_AUTO
                        ); 

                //直线
                LCD_DrawThickLineEx_NonCap(lcd_dev_info_ptr, s_banner_para.banner_rect.left, s_banner_para.banner_rect.bottom,
                        s_banner_para.banner_rect.right, s_banner_para.banner_rect.bottom, MMI_BANNER_LINE_COLOR, 1, GUILCD_PEN_STYLE_DEFAULT);
            }
            break;
        case MMI_BANNER_STR_ICONTEXT:
            {
                //背景
                GUI_FillRect(lcd_dev_info_ptr, s_banner_para.banner_rect, s_banner_para.background_color);

                //文本1
                text_str.wstr_ptr = s_banner_para.text1_str.wstr_ptr;
                text_str.wstr_len = s_banner_para.text1_str.wstr_len;
                state = state |GUISTR_STATE_SINGLE_LINE;
                GUISTR_DrawTextToLCDInRect( 
                        lcd_dev_info_ptr,
                        (const GUI_RECT_T *)&s_banner_para.text1_rect,       //the fixed display area
                        (const GUI_RECT_T *)&s_banner_para.text1_rect,       //用户要剪切的实际区域
                        (const MMI_STRING_T *)&text_str,
                        &text_style,
                        state,
                        GUISTR_TEXT_DIR_AUTO
                        );

                //文本2
                text_str.wstr_ptr = s_banner_para.text2_str.wstr_ptr;
                text_str.wstr_len = s_banner_para.text2_str.wstr_len;
                GUISTR_DrawTextToLCDInRect( 
                        lcd_dev_info_ptr,
                        (const GUI_RECT_T *)&s_banner_para.text2_rect,       //the fixed display area
                        (const GUI_RECT_T *)&s_banner_para.text2_rect,       //用户要剪切的实际区域
                        (const MMI_STRING_T *)&text_str,
                        &text_style,
                        state,
                        GUISTR_TEXT_DIR_AUTO
                        );

                //图片
                GUIRES_DisplayImg(PNULL, &s_banner_para.image_rect, PNULL, win_id, s_banner_para.image_id, lcd_dev_info_ptr); 

                //直线
                LCD_DrawThickLineEx_NonCap(lcd_dev_info_ptr, s_banner_para.banner_rect.left, s_banner_para.banner_rect.bottom,
                        s_banner_para.banner_rect.right, s_banner_para.banner_rect.bottom, MMI_BANNER_LINE_COLOR, 1, GUILCD_PEN_STYLE_DEFAULT);
            }
            break;

        default:
            break;
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void StartControlBannerTimer(void)
{
    if (0 < s_banner_para.control_banner_timer_id)
    {
        MMK_StopTimer(s_banner_para.control_banner_timer_id);
        s_banner_para.control_banner_timer_id = 0;
    }
#ifndef UI_MULTILAYER_SUPPORT
    s_banner_para.control_banner_timer_id = MMK_CreateWinTimer(MMICOMMON_BANNER_WIN_ID,MMI_BANNER_5SECONDS,FALSE);
#else
    s_banner_para.control_banner_timer_id = MMK_CreateTimerCallbackEx(MMI_BANNER_5SECONDS,BannerTimerCallBack,PNULL,TRUE,PNULL,PNULL);
#endif
}

/*****************************************************************************/
//  Description : stop control panel timer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void StopControlBannerTimer(void)
{
    if (0 < s_banner_para.control_banner_timer_id)
    {
        MMK_StopTimer(s_banner_para.control_banner_timer_id);
        s_banner_para.control_banner_timer_id = 0;
    }
}

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetBltLayer(const GUI_LCD_DEV_INFO *lcd_info_ptr)
{
    UILAYER_APPEND_BLT_T append_layer = {0};

    append_layer.lcd_dev_info = *lcd_info_ptr;
    append_layer.layer_level = /*UILAYER_LEVEL_HIGH*/UILAYER_LEVEL_DEBUG;
    UILAYER_RemoveBltLayer(lcd_info_ptr); // 先remove再blt，保证在最上层
    UILAYER_AppendBltLayer(&append_layer);        
}
#endif

/*****************************************************************************/
//  Description :show banner
//  Global resource dependence : 
//  Author:
//  Note:The win_id is parent window id
/*****************************************************************************/
#ifdef UI_MULTILAYER_SUPPORT
LOCAL void DisplayControlBanner(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T      win_rect = {0};
    GUI_RECT_T      old_win_rect = {0};
    win_rect.left = s_banner_para.banner_rect.left;
    win_rect.top = s_banner_para.banner_rect.top;
    win_rect.right = s_banner_para.banner_rect.right;
    win_rect.bottom = s_banner_para.banner_rect.bottom;
    MMK_GetWinRect(win_id, &old_win_rect);
    MMK_SetWinRect(win_id, &win_rect);

    if ( 0 != s_banner_para.control_banner_timer_id )
    {
        MMK_SendMsg(s_banner_para.parent_win_id, MSG_COMMON_BANNER_CLOSE, PNULL);
        UILAYER_Clear(&s_banner_para.dev_layer_handle);
        UILAYER_ReleaseLayer(&s_banner_para.dev_layer_handle);
        s_banner_para.dev_layer_handle.block_id = 0;
        MMK_DelayUpdateScreen();
    }
    if(UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T    create_info = {0};

        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = win_rect.left;
        create_info.offset_y = win_rect.top;
        create_info.width = (win_rect.right-win_rect.left+1);
        create_info.height = (win_rect.bottom-win_rect.top+1);
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = TRUE; 
        UILAYER_CreateLayer(
                &create_info,
                &s_banner_para.dev_layer_handle
                );
        SetBltLayer(&s_banner_para.dev_layer_handle);
        UILAYER_SetLayerColorKey(&s_banner_para.dev_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
    }

    StartControlBannerTimer();
    DrawControlBanner(win_id);
    MMK_SetWinRect(win_id, &old_win_rect);
}

/*****************************************************************************/
//  Description :timer call back
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void BannerTimerCallBack(uint8 timer_id,uint32 param)
{
    if (s_banner_para.control_banner_timer_id == timer_id)
    {
        CloseBanner();
    }

}
#endif
/*****************************************************************************/
//  Description : close banner win
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
PUBLIC void MMIAPICOM_CloseBannerWin(uint32 keystatus, uint32 keycode)
{
    if ((keystatus == KEY_PRESSED) && ((keycode == KEY_UP) || (keycode == KEY_DOWN) || (keycode == KEY_RED) || (keycode == KEY_CANCEL)))
    {
        if (0 != s_banner_para.control_banner_timer_id)
        {
            CloseBanner();
        }
    }
}

/*****************************************************************************/
//  Description : MMIAPICOM_IsBannerWinOpen
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsBannerWinOpen(void)
{
    if (0 != s_banner_para.control_banner_timer_id)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : close banner 
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
PUBLIC void MMIAPICOM_CloseBanner(void)
{
    if (0 != s_banner_para.control_banner_timer_id)
    {
        CloseBanner();
    }
}

PUBLIC MMI_WIN_ID_T MMIAPICOM_GetBannerParentWin(void)
{
    return s_banner_para.parent_win_id;
}

/*****************************************************************************/
//  Description : close banner
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
LOCAL void CloseBanner(void)
{
#ifdef UI_MULTILAYER_SUPPORT
    StopControlBannerTimer();
    MMK_SendMsg(s_banner_para.parent_win_id, MSG_COMMON_BANNER_CLOSE, PNULL);
    UILAYER_Clear(&s_banner_para.dev_layer_handle);
    UILAYER_ReleaseLayer(&s_banner_para.dev_layer_handle);
    s_banner_para.dev_layer_handle.block_id = 0;

    MMK_DelayUpdateScreen();
    if(PNULL != s_banner_para.text1_str.wstr_ptr)
    {
        SCI_Free(s_banner_para.text1_str.wstr_ptr);
        s_banner_para.text1_str.wstr_ptr = PNULL;
    }
    if(PNULL != s_banner_para.text2_str.wstr_ptr)
    {
        SCI_Free(s_banner_para.text2_str.wstr_ptr);
        s_banner_para.text2_str.wstr_ptr = PNULL;
    }
#else
    MMK_CloseWin(s_banner_para.banner_win);
#endif
}

