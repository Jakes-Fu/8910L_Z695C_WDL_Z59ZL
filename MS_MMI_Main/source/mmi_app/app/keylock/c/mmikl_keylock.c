/*****************************************************************************
** File Name:      mmi_keylock.c                                             *
** Author:                                                                   *
** Date:           2006-5                                                    *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe key lock                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2006       Bruce.Chi           Create                                  *
*****************************************************************************/
#define _MMI_KEYLOCK_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_keylock_trc.h"
#include "mmikl_export.h"
#include "mmi_id.h"
#include "mmi_text.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmisms_export.h"
#include "mmicc_export.h"
#include "mmi_appmsg.h"
#include "guires.h"
#include "mmialarm_export.h"
#include "graphics_draw.h"
#include "guictrl_api.h"
#include "mmi_wallpaper_export.h"
#include "guibutton.h"
#include "mmicl_export.h"
#ifdef CALENDAR_SUPPORT
#include "mmischedule_export.h"
#endif
#include "mmk_tp.h"
#include "mmitheme_special.h"
#include "mmiset_nv.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "guistatusbar.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "tp_export.h"
#ifdef JAVA_SUPPORT_IA
#include "mmijava_export.h"
#endif
#include "mmipub.h"
#include "mmiidle_export.h"
#include "mmiidle_statusbar.h"
#include "mmicom_statusarea.h"
#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#include "mmikl_effectmain.h"
#endif
#include "mmikl_internal.h"
#ifdef NPS_SUPPORT
#include "nps_export.h"
#endif
#include "mmiconnection_export.h"

#ifdef MMI_DEVICELOCK_SUPPORT
#include "mmi_devicelock_export.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 //for keylock info 
#if defined ANALOG_CLK_SUPPORT
#define  MMI_KEYLOCK_USE_ANALOG_CLOCK
#endif
//#define  MMI_KEYLOCK_USE_LAYER_DRAW_TIPS

#define  KEYLOCK_INFO_NUM  3
#ifdef MAINLCD_SIZE_240X320
#define  KEYLOCK_TIME_TOP_OFFSET    0//4
#else
#define  KEYLOCK_TIME_TOP_OFFSET    4
#endif
#ifndef MMI_KEYLOCK_MINI_SUPPORT
#define  KEYLOCK_DATE_TOP_OFFSET    46
#else
#define  KEYLOCK_DATE_TOP_OFFSET    20//46
#endif
#define  KEYLOCK_DATE_FONT_HEIGHT   36//12//change by James li
//#define  KEYLOCK_COUNT_STRING_SIZE     5
#ifdef MMI_KEYLOCK_MINI_SUPPORT
#ifdef MAINLCD_SIZE_128X160
#ifdef MMI_RES_ORIENT_LANDSCAPE
#define KEY_LOCK_QQVGA_MISSEVENT_TOP 70//80
#define KEY_LOCK_QQVGA_MISSEVENT_LEFT 65//20
#else
#define KEY_LOCK_QQVGA_MISSEVENT_TOP 100//80
#define KEY_LOCK_QQVGA_MISSEVENT_LEFT 50//20
#endif
#else
#define KEY_LOCK_QQVGA_MISSEVENT_TOP 170
#define KEY_LOCK_QQVGA_MISSEVENT_LEFT 50
#define KEY_LOCK_QQVGA_MISSEVENT_TOP_H 130
#define KEY_LOCK_QQVGA_MISSEVENT_LEFT_H 70
#endif
#endif
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct
{
    BOOLEAN is_new_sms_alerted;     //在键盘锁定期间是否有新SMS
    BOOLEAN is_new_mms_alerted;     //在键盘锁定期间是否有新MMS
#if defined(MMI_VCARD_SUPPORT)
    BOOLEAN is_new_vcard_alerted;       //在键盘锁定期间是否有新Vcard
#endif
    BOOLEAN is_missed_call;         //在键盘锁定期间是否有未接电话
#if defined(MMI_VCALENDAR_SUPPORT)
    BOOLEAN is_new_vcalendar_alerted;       //在键盘锁定期间是否有新vcalendar
#endif
}MMI_KL_EVENT_T;



typedef struct 
{
    void   *data_ptr;
    uint32  data_type;  //0: RGB565, 1: ARGB888
    uint16  width;
    uint16  height;
    int16   start_x;
    int16   start_y;
}MMI_KL_CLOCK_ALPHA_IMG_T; /*lint !e770*/
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMI_KL_EVENT_T       s_st_mmi_kl_event;
LOCAL MMI_KL_WIN_STATUS_E  s_e_mmi_kl_win_status;
//LOCAL BOOLEAN              s_is_press_down = FALSE;

LOCAL uint8                s_unlock_timer_id = 0;
LOCAL uint8                s_tips_show_timer_id = 0;
LOCAL uint8                s_unlocked_show_timer_id = 0;

/* @fen.xie for MS00173072:
TRUE:表示当前需先挂断所有的call后，再处理合盖事件(回退到idle 及锁键盘)*/
LOCAL BOOLEAN           s_is_later_handle_slide = FALSE;

PUBLIC void MMIKL_UnlockedTipsShowTimer(
                                   MMI_WIN_ID_T     win_id
                                   );
PUBLIC BOOLEAN MMIKL_StopUnlockedTipsShowTimer(void);

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
  
/**-------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get key lock ring state
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIKL_GetKeyLockRing(void);

/*****************************************************************************/
//  Description : lock phone handle louse focus msg
//  Global resource dependence :
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleKLLouseFocus(void);

#ifdef MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT
/*****************************************************************************/
//  FUNCTION:       SetWCFormParam
// 	Description :   set the from parameter
//	Global resource dependence : 
//  Author:         murphy.xie
//  Date:           2010/05/12
//	Note:   
/*****************************************************************************/
LOCAL void SetKeyLockControlParam(void);

/*****************************************************************************/
//  Description : SwitchControlRectInfo
//  Global resource dependence : 
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL void SwitchControlRectInfo(void);

/*****************************************************************************/
//  Description : DisplayClockCallbackFun
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayClockCallbackFun (GUIANIM_OWNER_DRAW_T    *draw_ptr);
#endif
LOCAL BOOLEAN DrawSoftkey(MMI_WIN_ID_T win_id);
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : PressFormInfoCtrlCallbackFun for draw message/call/event tips
//  Global resource dependence : 
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL void PressFormInfoCtrlCallbackFun (GUIANIM_OWNER_DRAW_T    *draw_ptr);
#endif

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
WINDOW_TABLE(MMIKC_DISP_WINTAB) =
{
    WIN_FUNC((uint32)MMIKL_HandleEffeckKLWinMsg),
	WIN_MOVE_STYLE(MOVE_FORBIDDEN),
#ifndef EFFECT_RIPPLE_SUPPORT //144786 
    WIN_HIDE_STATUS,
#endif
    WIN_ID(MMIKL_CLOCK_DISP_WIN_ID),
    WIN_STYLE( WS_DISABLE_RETURN_WIN|WS_HAS_ANIM_BG|WS_DISABLE_COMMON_BG ),
    CREATE_ANIM_CTRL(MMIKL_MAIN_ANIM_BG_ID,MMIKL_CLOCK_DISP_WIN_ID),
    //WIN_SOFTKEY(TXT_IDLE_UNLOCK,TXT_NULL,TXT_NULL),
    END_WIN
};
#endif

/*****************************************************************************/
//  Description : MMIKL_CreatWallpaperCtrl
//  Global resource dependence : 
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_CreatWallpaperCtrl(MMISET_KEY_LOCK_EFFECT_E lock_type)
{
    GUI_BOTH_RECT_T         anim_ctrl_rect  = MMITHEME_GetFullScreenBothRect();
    BOOLEAN                 result           = TRUE;
#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
    MMIAPIIDLE_SetInitWallpaper_KL(FALSE, MMIKL_CLOCK_DISP_WIN_ID, MMIKL_MAIN_ANIM_BG_ID,TRUE);    
#else
    MMIAPIIDLE_SetInitWallpaper(FALSE,MMIKL_CLOCK_DISP_WIN_ID,MMIKL_MAIN_ANIM_BG_ID);
#endif
    GUIAPICTRL_SetBothRect(MMIKL_MAIN_ANIM_BG_ID,&anim_ctrl_rect);
    GUIAPICTRL_SetState(MMIKL_MAIN_ANIM_BG_ID, GUICTRL_STATE_DISABLE_TP | GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
    return (result);
}

#ifdef MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : DisplayKeyLockTips not use layer just directly draw to main lcd
//  Global resource dependence : 
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL void DisplayKeyLockTips(void)
{
    uint16                      str_width = 0;
    GUI_RECT_T                  tips_rect = {0};
    MMI_STRING_T                tips_string = {0};
    GUISTR_STYLE_T              str_style = {0};
    GUISTR_STATE_T              str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK;
    uint16                      image_width = 0,image_height = 0;
    uint16                      bg_width = 0,bg_height = 0;
    uint16                      lcd_width = 0;
    uint16                      lcd_height = 0;
	GUI_LCD_DEV_INFO			lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    uint8                       str_margin = 4;
    
    {

        //get rect info
        GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_KEYLOCK_TIPS_BG,MMIKL_CLOCK_DISP_WIN_ID);
        GUIRES_GetImgWidthHeight(&bg_width,&bg_height, IMAGE_IDLE_KEYLOCK_PRESS_BG,MMIKL_CLOCK_DISP_WIN_ID);
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

        //get string
        MMI_GetLabelTextByLang(TXT_KEYLOCK_UNLOCK_TIP,&tips_string);

        //set string style
        str_style.font       = MMI_DEFAULT_NORMAL_FONT;
        str_style.font_color = MMI_DEFAULT_TEXT_COLOR;
        str_style.align      = ALIGN_HVMIDDLE;

        //get string width
        str_width = GUISTR_GetStringWidth(&str_style,
            &tips_string,
            str_state);

        //get tips rect
        tips_rect.right = lcd_width -1;
        tips_rect.left = MAX(tips_rect.left, (tips_rect.right - tips_rect.left - str_width)/2);
        tips_rect.bottom = lcd_height -1 -MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT- bg_height + image_height/2;
        tips_rect.top  = tips_rect.bottom - image_height+1;

        if((str_width + str_margin*2) > lcd_width) //双行显示 
        {
            //str_style.font = MMI_DEFAULT_SMALL_FONT;
            tips_rect.top  = tips_rect.bottom - (image_height+1)*3/2;
        }
        
        IMG_EnableTransparentColor(TRUE);
        //display bg image
        GUIRES_DisplayImg(PNULL,&tips_rect,
            PNULL,
            MMIKL_CLOCK_DISP_WIN_ID,
            IMAGE_IDLE_KEYLOCK_TIPS_BG,
            &lcd_dev_info);


        //display text
        tips_rect.right = tips_rect.right - str_margin;
        tips_rect.left = tips_rect.left + str_margin;
        tips_rect.bottom = tips_rect.bottom - str_margin;
        tips_rect.top  = tips_rect.top + str_margin;
        GUISTR_DrawTextToLCDInRect(&lcd_dev_info,
            &tips_rect,
            &tips_rect,
            &tips_string,
            &str_style,
            str_state,
            GUISTR_TEXT_DIR_AUTO);
        IMG_EnableTransparentColor(FALSE);

    }
}
#endif
/*****************************************************************************/
//  Description : SwitchControlRectInfo
//  Global resource dependence : 
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL void SwitchControlRectInfo(void)
{
    uint16                      image_width = 0,image_height = 0;
    GUI_RECT_T                  both_rect = MMITHEME_GetClientRect();
#ifdef TOUCH_PANEL_SUPPORT 
    uint16                      used_width = 0;
    uint16                      width = 0;
    uint16                      info_width = 0;
#endif
    //clock
    #ifdef MMI_KEYLOCK_USE_ANALOG_CLOCK
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KEYLOCK_ACLOCK,MMIKL_CLOCK_DISP_WIN_ID);
    #else
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_KEYLOCK_TIME_BG,MMIKL_CLOCK_DISP_WIN_ID);
    #endif
    both_rect.top  = MMI_STATUSBAR_HEIGHT;
    both_rect.bottom = both_rect.top + image_height;
    GUIAPICTRL_SetRect(MMIKL_CLOCK_ANIM_ID,&both_rect);
    
#ifdef TOUCH_PANEL_SUPPORT 
    //set press animal bg
    both_rect = MMITHEME_GetClientRect();
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_KEYLOCK_PRESS_BG,MMIKL_CLOCK_DISP_WIN_ID);
    both_rect.top = both_rect.bottom - image_height;
    GUIAPICTRL_SetRect(MMIKL_PRESS_ANIM_BG_ID,&both_rect);


    //set lock icon botton
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_KEYLOCK_PRESS_DOWN,MMIKL_CLOCK_DISP_WIN_ID);
    both_rect.right = both_rect.left + image_width -1;
    GUIAPICTRL_SetRect(MMIKL_PRESS_BOTTON_ID,&both_rect);
    used_width = image_width;
    

    //set info bg
    both_rect = MMITHEME_GetClientRect();
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_KEYLOCK_PRESS_BG,MMIKL_CLOCK_DISP_WIN_ID);
    both_rect.bottom = both_rect.bottom - MMI_KEYLOCK_SHOW_GAP;  //@CR NEWMS00180557 2012/3/20
    both_rect.top = both_rect.bottom - image_height;
    
    width = MMITHEME_GetClientRectWidth();
    info_width = (width - used_width)/KEYLOCK_INFO_NUM;
    //hint    
    both_rect.left = both_rect.right + 1 - info_width;
    GUIAPICTRL_SetRect(MMIKL_PRESS_ANIM_EVENT_ID,&both_rect);

    //message
    both_rect.right = both_rect.left - 1;
    both_rect.left = both_rect.right + 1 - info_width;
    GUIAPICTRL_SetRect(MMIKL_PRESS_ANIM_MESSAGE_ID,&both_rect);
    
    //call
    both_rect.right = both_rect.left - 1;
    both_rect.left = both_rect.right + 1 - info_width;
    GUIAPICTRL_SetRect(MMIKL_PRESS_ANIM_CALL_ID,&both_rect);
#endif
}
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : PressFormInfoCtrlCallbackFun for draw message/call/event tips
//  Global resource dependence : 
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL void PressFormInfoCtrlCallbackFun (GUIANIM_OWNER_DRAW_T    *draw_ptr)
{
    GUI_LCD_DEV_INFO    lcd_dev_info =   {GUI_LCD_0, GUI_BLOCK_0};
    GUI_RECT_T          tips_rect = {0};
    uint16              image_width = 0,image_height = 0;
    uint16              bg_width = 0,bg_height =0;
    uint32              count_num = 0;
    char                count_str[KEYLOCK_COUNT_STRING_SIZE] = {0};
    uint8               str_len = 0;
    wchar			    count_wstr[5]  = {0};
    MMI_STRING_T        count_string = {0};
    GUISTR_STATE_T      text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE;	
    GUISTR_STYLE_T      str_style = {0};
    uint16              count_string_pixel_width = 0;
    uint16              tips_margin = 0;

    if(MMK_GetCtrlId(draw_ptr->ctrl_handle) ==  MMIKL_PRESS_ANIM_CALL_ID)
    {
        count_num = MMIAPICC_GetMissedCallNumber();
    }
    else if(MMK_GetCtrlId(draw_ptr->ctrl_handle) ==  MMIKL_PRESS_ANIM_MESSAGE_ID)
    {
        count_num = MMIAPISMS_GetAllUnreadMsgCount()
        + MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_MAX)
        ;
    }
    else if(MMK_GetCtrlId(draw_ptr->ctrl_handle) ==  MMIKL_PRESS_ANIM_EVENT_ID)
    {
        count_num =MMIAPIALM_GetTotalTimesOfArrivedEvent();
    }
    
    if(count_num > 0)
    {
        sprintf(count_str,"+%d",count_num);
        str_len = strlen(count_str);
        MMI_STRNTOWSTR(count_wstr,KEYLOCK_COUNT_STRING_SIZE,(const uint8*)count_str,KEYLOCK_COUNT_STRING_SIZE,str_len);
        tips_rect = draw_ptr->display_rect;
        count_string.wstr_ptr = count_wstr;
        count_string.wstr_len = str_len;
        
        count_string_pixel_width = GUI_CalculateStringPiexlNum(count_string.wstr_ptr,count_string.wstr_len,SONG_FONT_12,0);
        GUIRES_GetImgWidthHeight(&bg_width,&bg_height, IMAGE_IDLE_KEYLOCK_CALL,MMIKL_CLOCK_DISP_WIN_ID);
        GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_KEYLOCK_TIPS_COUNT_BG,MMIKL_CLOCK_DISP_WIN_ID);

        tips_margin = image_width/4;
        
        if(count_string_pixel_width < image_width)
        {
            count_string_pixel_width = image_width;
            tips_margin = 0;
        }
        
        tips_rect.right = MIN(draw_ptr->display_x + bg_width + image_width/2,tips_rect.right);//tips的一半与bg图片重叠  
        tips_rect.left = MAX(tips_rect.left,tips_rect.right - count_string_pixel_width  - tips_margin);  //image_width/4 作为左右的margin  

        tips_rect.top = MAX(tips_rect.top,draw_ptr->display_y - image_height/2);//tips的一半与bg图片重叠
        tips_rect.bottom  = MIN(tips_rect.top +image_height,tips_rect.bottom);

        GUIRES_DisplayImg(PNULL,&tips_rect,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_KEYLOCK_TIPS_COUNT_BG,&lcd_dev_info);

        str_style.angle = ANGLE_0;
        str_style.align = ALIGN_HVMIDDLE;
        str_style.font = SONG_FONT_12;
        str_style.font_color = MMI_BLACK_COLOR;

        GUISTR_DrawTextToLCDInRect(&lcd_dev_info,&tips_rect,&tips_rect,&count_string,&str_style,text_state,GUISTR_TEXT_DIR_AUTO);
    }
}
#endif
#ifdef MMI_KEYLOCK_USE_ANALOG_CLOCK
/*****************************************************************************/
//  Description : alpha blending RGB565 data quickly
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint16 BlendRGB565(uint16 bk_color, uint16 fore_color, uint8 alpha)
{
    uint32 blend_color = 0;
    uint32 temp_back_color = 0;
    uint32 temp_fore_color = 0;
    
    alpha = (alpha + 1) >> 3;

    temp_fore_color = ( fore_color | (fore_color << 16)) & BLEND_MASK;
    temp_back_color = ( bk_color | (bk_color << 16)) & BLEND_MASK;
    
    blend_color = ((((temp_fore_color - temp_back_color) * alpha ) >> 5 ) + temp_back_color) & BLEND_MASK;
    blend_color = ((blend_color & 0xFFFF) | (blend_color >> 16));
    
    return (uint16)blend_color;
}

/*****************************************************************************/
//  Description : Alpha blend two images
//  Global resource dependence : 
//  Author:
//  Note: the output image is in the background image buffer, overwritting the orignial data.
//		  the type of the output image data is the same as that of the background image
/*****************************************************************************/
LOCAL BOOLEAN ProcessAlphaBlending(MMI_KL_CLOCK_ALPHA_IMG_T *bk_img_ptr, MMI_KL_CLOCK_ALPHA_IMG_T *fore_img_ptr)
{
    uint32 fore_width  = fore_img_ptr->width;
    uint32 fore_height = fore_img_ptr->height;
    uint32 bk_width = bk_img_ptr->width;
    uint32 bk_height = bk_img_ptr->height;
    int32 start_x = fore_img_ptr->start_x;
    int32 start_y = fore_img_ptr->start_y;
    uint32 blend_width = fore_width;
    uint32 blend_height = fore_height;
    uint32 *fore_ptr = (uint32 *)fore_img_ptr->data_ptr;
    GUI_RECT_T dst_rect = {0};
    GUI_RECT_T bk_rect = {0};
    GUI_RECT_T for_rect = {0};  
    
    bk_rect.left  = bk_img_ptr->start_x;
    bk_rect.top   = bk_img_ptr->start_y;
    bk_rect.right = bk_img_ptr->start_x + bk_img_ptr->width;
    bk_rect.bottom  = bk_img_ptr->start_y + bk_img_ptr->height;    

    for_rect.left  = fore_img_ptr->start_x;
    for_rect.top   = fore_img_ptr->start_y;
    for_rect.right = fore_img_ptr->start_x + fore_img_ptr->width;
    for_rect.bottom  = fore_img_ptr->start_y + fore_img_ptr->height;    

    if (!GUI_IntersectRect(&dst_rect, bk_rect, for_rect))
    {
        return FALSE;
    }

    if (start_y < 0)
    {
        start_y = -start_y;
        blend_height -= start_y;        /*lint !e737*/
        fore_ptr += (start_y * fore_width);     /*lint !e737*/
        start_y = 0;
    }

    if (start_x < 0)
    {
        start_x = -start_x;
        blend_width -= start_x;     /*lint !e737*/
        fore_ptr += start_x;
        start_x = 0;
    }

    blend_width = (start_x + blend_width) > bk_width        /*lint !e737*/
                    ? (bk_width - start_x) : blend_width;       /*lint !e737*/
    blend_height = (start_y + blend_height) > bk_height         /*lint !e737*/
                    ? (bk_height - start_y) : blend_height;     /*lint !e737*/

    if (IMGREF_FORMAT_RGB565 == bk_img_ptr->data_type)     //output RGB565
    {        
        uint32 i = 0, j = 0;
        uint16 *bk_ptr = (uint16 *)bk_img_ptr->data_ptr;

        bk_ptr += (start_y * bk_width + start_x);       /*lint !e737*/

        for (i=0; i<blend_height; i++)
        {
            for (j=0; j<blend_width; j++)
            {
                uint32 alpha, r, g, b;
                uint32 fore_value = *(fore_ptr + j);    
                uint16 bk_value = *(bk_ptr + j);

                alpha = ARGB_GET_A(fore_value);
                r = ARGB_GET_R(fore_value);
                g = ARGB_GET_G(fore_value);
                b = ARGB_GET_B(fore_value);

                fore_value = RGB888_TO_RGB565(r, g, b);
                bk_value = BlendRGB565(bk_value, (uint16)fore_value, alpha);
                *(bk_ptr + j) = bk_value;
            }

            bk_ptr += bk_width;
            fore_ptr += fore_width;
        }
    } 

    return TRUE;
}

/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DrawAClock(MMI_WIN_ID_T win_id,int16  top)
{
    GUI_RECT_T        win_rect = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
    SCI_TIME_T        sys_time        = {0};
    GUI_POINT_T       point = {0};  
    CLOCK_DRAW_IN_T   aclock_input_param  = {0};
    CLOCK_DRAW_OUT_T  aclock_output_param = {0};
    MMI_KL_CLOCK_ALPHA_IMG_T bk_img = {0};
    MMI_KL_CLOCK_ALPHA_IMG_T fore_img = {0};
    UILAYER_INFO_T    layer_info = {0};
    uint16            lcd_width  = 0;
    uint16            lcd_height = 0;
    uint16            width  = 0;
    uint16            height = 0;
    
    //aclock dial.
    aclock_input_param.clock_dial.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.clock_dial.width, &aclock_input_param.clock_dial.height, IMAGE_KEYLOCK_ACLOCK, win_id);
    aclock_input_param.clock_dial.data_ptr = (uint8 *)MMI_GetLabelImage(IMAGE_KEYLOCK_ACLOCK,win_id,&(aclock_input_param.clock_dial.data_size));
    aclock_input_param.clock_dial.center_x = aclock_input_param.clock_dial.width/2;
    aclock_input_param.clock_dial.center_y = aclock_input_param.clock_dial.height/2;   

    //aclock hour hand
    aclock_input_param.hour_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.hour_handle.width, &aclock_input_param.hour_handle.height, IMAGE_KEYLOCK_ACLOCK_HOUR, win_id);    
    aclock_input_param.hour_handle.data_ptr = (uint8 *)MMI_GetLabelImage(IMAGE_KEYLOCK_ACLOCK_HOUR,win_id,&(aclock_input_param.hour_handle.data_size));
    aclock_input_param.hour_handle.center_x = aclock_input_param.hour_handle.width/2;
    aclock_input_param.hour_handle.center_y = aclock_input_param.hour_handle.height/2;    

    //aclock min hand
    aclock_input_param.min_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.min_handle.width, &aclock_input_param.min_handle.height, IMAGE_KEYLOCK_ACLOCK_MIN, win_id);        
    aclock_input_param.min_handle.data_ptr = (uint8 *)MMI_GetLabelImage(IMAGE_KEYLOCK_ACLOCK_MIN,win_id,&(aclock_input_param.min_handle.data_size));
    aclock_input_param.min_handle.center_x = aclock_input_param.min_handle.width/2;
    aclock_input_param.min_handle.center_y = aclock_input_param.min_handle.height/2; 

    aclock_input_param.target_buf_size = aclock_input_param.clock_dial.width * aclock_input_param.clock_dial.height * 4;
    aclock_input_param.target_buf_ptr = (void*)SCI_ALLOCA(aclock_input_param.target_buf_size);

    if (PNULL == aclock_input_param.target_buf_ptr)
    {
        //SCI_TRACE_LOW:"error: DrawAClock target_buf_ptr alloc failed,  return directly"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKL_KEYLOCK_557_112_2_18_2_31_4_6,(uint8*)"");
        return;
    }

    //get system time
    TM_GetSysTime(&sys_time);  

    aclock_input_param.time.hour = sys_time.hour;
    aclock_input_param.time.minute  = sys_time.min;
    aclock_input_param.time.second = sys_time.sec;

    GRAPH_DrawClock(&aclock_input_param, &aclock_output_param);

    //display aclock
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    UILAYER_GetLayerInfo(&lcd_dev_info, &layer_info);
    
    bk_img.data_type = IMGREF_FORMAT_RGB565;

    bk_img.data_ptr = UILAYER_GetLayerBufferPtr(&lcd_dev_info);
    if (PNULL == bk_img.data_ptr)
    {
        SCI_FREE(aclock_input_param.target_buf_ptr);
        SCI_TRACE_LOW("error: DrawAClock PNULL == bk_img.data_ptr");      
        return;
    }
    bk_img.width = layer_info.mem_width;
    bk_img.height = layer_info.layer_height;
    
    fore_img.data_ptr = aclock_output_param.output_ptr;
    if (PNULL == fore_img.data_ptr)
    {
        SCI_FREE(aclock_input_param.target_buf_ptr);
        SCI_TRACE_LOW("error: DrawAClock PNULL == fore_img.data_ptr");      
        return;
    }

    fore_img.data_type = IMGREF_FORMAT_ARGB888;
    fore_img.width = aclock_input_param.clock_dial.width;
    fore_img.height = aclock_input_param.clock_dial.height;
   
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    fore_img.start_x = (lcd_width - aclock_input_param.clock_dial.width)/2;   
    fore_img.start_y = top; 
    
    if (!ProcessAlphaBlending(&bk_img, &fore_img))
    {
        //SCI_TRACE_LOW:"error: foreground image rect is not in background image rect, return directly"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKL_KEYLOCK_600_112_2_18_2_31_4_7,(uint8*)"");
        SCI_FREE(aclock_input_param.target_buf_ptr);    
        return;
    }

    //in the end, display the center point.
    GUIRES_GetImgWidthHeight(&width, &height, IMAGE_KEYLOCK_ACLOCK_CENTER, win_id); 

    point.x = (lcd_width - width)/2;
    point.y = top + (aclock_input_param.clock_dial.height - height)/2;

    GUIRES_DisplayImg(&point,
                      PNULL,
                      PNULL,
                      win_id,
                      IMAGE_KEYLOCK_ACLOCK_CENTER,
                      &lcd_dev_info);

    MMITHEME_StoreUpdateRect(&lcd_dev_info,win_rect);
        
    SCI_FREE(aclock_input_param.target_buf_ptr);
}
#endif

LOCAL void DisplayKLSimStatus(MN_DUAL_SYS_E dual_sys)
{

    wchar                       wstr_buf[MMIPHONE_MAX_OPER_NAME_LEN]        = {0};
    GUI_FONT_T                  font                = SONG_FONT_36;
    GUI_COLOR_T                 color               = MMI_BLACK_COLOR;
    uint16                      str_width           = 0;
    MMI_STRING_T                display_string      = {0};
    //GUI_LCD_DEV_INFO            lcd_dev_info        = *GetCommonIdlePaintLcdDev();
	GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUISTR_STYLE_T              text_style = {0};/*lint !e64*/
    GUISTR_STATE_T              text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ELLIPSIS| GUISTR_STATE_EFFECT;
    GUI_RECT_T                  string_display_rect = {0};
    GUI_RECT_T                  sim_icon_rect = {0};
    MMI_IMAGE_ID_T              img_id = 0;
	BOOLEAN 				is_fly_mode_on	= FALSE;
		
		is_fly_mode_on = MMIAPISET_GetFlyMode();

    if(dual_sys == MN_DUAL_SYS_1)
    {
        sim_icon_rect.top =  200;// MMI_IDLE_SIM_STATUS_TOP + MMI_IDLE_INTERVAL_MARGIN;
        sim_icon_rect.left = 30;//MMI_IDLE_TIME_MARGIN;
        sim_icon_rect.right= 210;//sim_icon_rect.left + MMI_IDLE_SIM_ICON_WIDTH;
        sim_icon_rect.bottom = 238;//sim_icon_rect.top + MMI_IDLE_SIM_ICON_HEIGHT;
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        img_id = IMAGE_PRIMO_IDLE_SCREEN_SINGLE_SIM;
#else
        img_id = IMAGE_PRIMO_SIM1;
#endif
    }
#ifdef MMI_MULTI_SIM_SYS_DUAL
    else if(dual_sys == MN_DUAL_SYS_2)
    {
        sim_icon_rect.top =240;// MMI_IDLE_SIM_STATUS_TOP + MMI_IDLE_SIM_STATUS_HEIGHT + MMI_IDLE_INTERVAL_MARGIN*2;
        sim_icon_rect.left = 30;// MMI_IDLE_TIME_MARGIN;
        sim_icon_rect.right= 210;// sim_icon_rect.left + MMI_IDLE_SIM_ICON_WIDTH;
        sim_icon_rect.bottom = 278;//sim_icon_rect.top + MMI_IDLE_SIM_ICON_HEIGHT;

        img_id = IMAGE_PRIMO_SIM2;
    }
#endif
    else
    {
        SCI_TRACE_LOW("[idle]: DisplaySimStatus para error, dual_sys = %d", dual_sys);
        return;
    }

    text_style.region_num = 0;
    MMIIDLE_GetIdleTextStyle(&text_style, font, color);
    text_style.align = ALIGN_HVMIDDLE ;//ALIGN_VMIDDLE;

    display_string.wstr_ptr = wstr_buf;

	if (is_fly_mode_on)
    {
        MMI_GetLabelTextByLang(TXT_SET_FLY_MODE, &display_string);
    }
	else if (MMIAPIPHONE_GetSimExistedStatus(dual_sys))
	{
		  MMIAPIPHONE_GetSimAndServiceString(dual_sys, &display_string);
	}
  	else
  	{
		 MMI_GetLabelTextByLang(TXT_INSERT_SIM, &display_string);
	}


    if(0 != display_string.wstr_len)
    {
        str_width = GUISTR_GetStringWidth(&text_style, &display_string, text_state);

        string_display_rect.top = sim_icon_rect.top + 3 - 10;
        string_display_rect.bottom = sim_icon_rect.bottom + 15 - 10;

        string_display_rect.left = 10 ;
        string_display_rect.right = 230 ;
        //text_style.effect = 0;
        GUISTR_DrawTextToLCDInRect(
                &lcd_dev_info,
                (const GUI_RECT_T *)&string_display_rect,       //the fixed display area
                (const GUI_RECT_T *)&string_display_rect,       
                (const MMI_STRING_T *)&display_string,
                &text_style,
                text_state,
                GUISTR_TEXT_DIR_AUTO);

    }


}
#if 1//def ZDT_PRODUCT_Z605
#if defined(ZDT_KEYLOCK_SCREEN_BIG_DAY_STYLE)
LOCAL void OutputKLLunarDate(void)
{
	GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
	GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
	GUI_LCD_DEV_INFO lcd_dev_info = {0};
	GUI_RECT_T          rect = {0};
	GUI_POINT_T dis_point = {0};
	MMI_STRING_T  LunarDate_str ={0};
	uint16 file_name_len = 0;
	wchar       lunar_name[50 + 1]={0};
	uint16			str_width = 0;
	uint16          str_height = 0;
	GUI_COLOR_T  lunar_color = MMI_BLACK_COLOR; // xiongkai MMI_GRAY_WHITE_COLOR;//MMI_GRAY_WHITE_COLOR ;//MMI_WHITE_COLOR;
 	MMISET_WALLPAPER_SET_T  wallpaper_info  =   {0};
	MMI_IMAGE_ID_T      bg_img = 0;
	uint16 lcd_width = 0;
	uint16 lcd_height = 0;

     
	LunarDate_str.wstr_ptr = lunar_name; 
	//ZDT_LunarDate_full(&LunarDate_str, 50);
	ZDT_LunarDate(&LunarDate_str, 50);
	MMIIDLE_GetIdleTextStyle(&text_style , SONG_FONT_38,lunar_color );    //SONG_FONT_28   xiongkai
    
       GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
		   
	str_width = GUISTR_GetStringWidth(&text_style,&LunarDate_str,state);
	str_height = GUI_GetStringHeight(SONG_FONT_38, LunarDate_str.wstr_ptr, LunarDate_str.wstr_len);   //SONG_FONT_28    xiongkai
	// 绘制字符串
	dis_point.x = (lcd_width-str_width)>>1;
	dis_point.y = lcd_height-str_height-8- 8; // 12;	 
    
	rect.top = dis_point.y;
	rect.left =  0;///dis_point.x;
	rect.right = 240 ;//rect.left  + str_width;
	rect.bottom = rect.top + str_height ;
	
	
	text_style.align = ALIGN_HMIDDLE;
	text_style.effect = 0;
	GUISTR_DrawTextToLCDInRect(
	    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
	    (const GUI_RECT_T      *)&rect,       //the fixed display area
	    (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
	    (const MMI_STRING_T    *)&LunarDate_str,
	    &text_style,
	    state,
	    GUISTR_TEXT_DIR_AUTO
	    );



}
#else
LOCAL void OutputKLLunarDate(void)
{
	GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
	GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
	GUI_LCD_DEV_INFO lcd_dev_info = {0};
	GUI_RECT_T          rect = {0};
	GUI_POINT_T dis_point = {0};
	MMI_STRING_T  LunarDate_str ={0};
	uint16 file_name_len = 0;
	wchar       lunar_name[30 + 1]={0};
	uint16			str_width = 0;
	uint16          str_height = 0;
	GUI_COLOR_T  lunar_color =MMI_WHITE_COLOR;//MMI_GRAY_WHITE_COLOR ;//MMI_WHITE_COLOR;
 	MMISET_WALLPAPER_SET_T  wallpaper_info  =   {0};
	 MMI_IMAGE_ID_T      bg_img = 0;
	 
	LunarDate_str.wstr_ptr = lunar_name; 
	ZDT_LunarDate(&LunarDate_str, 30);
	MMIIDLE_GetIdleTextStyle(&text_style , SONG_FONT_37,lunar_color );

		   
	str_width = GUISTR_GetStringWidth(&text_style,&LunarDate_str,state);
	str_height = GUI_GetStringHeight(SONG_FONT_37, LunarDate_str.wstr_ptr, LunarDate_str.wstr_len);
	// 绘制字符串
	dis_point.x = 38;
	dis_point.y = 140;	 
	rect.top = 140; //dis_point.y + 10 ;
	rect.left =  0;///dis_point.x;
	rect.right = 240 ;//rect.left  + str_width;
	rect.bottom = rect.top + str_height ;
	
	text_style.align = ALIGN_HMIDDLE;
        //text_style.effect = 0;
	GUISTR_DrawTextToLCDInRect(
	    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
	    (const GUI_RECT_T      *)&rect,       //the fixed display area
	    (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
	    (const MMI_STRING_T    *)&LunarDate_str,
	    &text_style,
	    state,
	    GUISTR_TEXT_DIR_AUTO
	    );



}
#endif
#endif
/*****************************************************************************/
//  Description : Display Data for long press key lock under digital clock
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
#if defined(ZDT_KEYLOCK_SCREEN_BIG_DAY_STYLE)
LOCAL void DisplayDate(int16 time_top)
{
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    SCI_DATE_T date = {0};
    GUISTR_STYLE_T text_style1 = {0};
    GUISTR_STYLE_T text_style2 = {0};    
    MMI_STRING_T week_string = {0};
    MMI_STRING_T month_string = {0};
    wchar w_buf[10] ={0}; 
    wchar m_buf[10] ={0};     
    wchar datewstr[30] = {0};
    wchar wstr_buf[20] = {0};
    uint8 datestr[20] = {0};
    GUI_RECT_T rect_month = {0};
    GUI_RECT_T rect_week = {0};    
    GUI_LCD_DEV_INFO lcd_dev = {0};
    GUISTR_STATE_T      state_month =         GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_BOLD;
    GUISTR_STATE_T      state_week =         GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;    
    MMI_TEXT_ID_T week_table[] = {
                                      STR_CLK_SUNDAY,
                                      STR_CLK_MONDAY,
                                      STR_CLK_TUESDAY,
                                      STR_CLK_WEDNESDAY,
                                      STR_CLK_THURSDAY,
                                      STR_CLK_FRIDAY,
                                      STR_CLK_SATURDAY
                                 };

    MMI_TEXT_ID_T month_table[] = {
                                    TXT_MONTH_1,
                                    TXT_MONTH_2,
                                    TXT_MONTH_3,
                                    TXT_MONTH_4,
                                    TXT_MONTH_5,
                                    TXT_MONTH_6,
                                    TXT_MONTH_7,
                                    TXT_MONTH_8,
                                    TXT_MONTH_9,
                                    TXT_MONTH_10,
                                    TXT_MONTH_11,
                                    TXT_MONTH_12,                                    
                                 };
    
    SCI_MEMSET(w_buf,0,sizeof(w_buf));
    SCI_MEMSET(m_buf,0,sizeof(m_buf));    
    SCI_MEMSET(datewstr,0,sizeof(datewstr));
    SCI_MEMSET(datestr,0,sizeof(datestr));
    SCI_MEMSET(wstr_buf,0,sizeof(wstr_buf));   

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    week_string.wstr_ptr = w_buf;
    month_string.wstr_ptr = m_buf;   
    
    TM_GetSysDate(&date);       
    MMI_GetLabelTextByLang(week_table[date.wday], &week_string);    
    MMI_GetLabelTextByLang(month_table[date.mon-1], &month_string);

    #if 0
    MMIIDLE_GetIdleTextStyle(&text_style1 , SONG_FONT_32,0xF840);   //xiongkai      MMI_RED_COLOR   {250,5,5} = 0xF840   //SONG_FONT_36
    text_style1.align = ALIGN_LEFT;
    text_style1.effect = 0;
    
    rect_month.left = 25;
    rect_month.right = (lcd_width>>1)-8;
    rect_month.top = 73;
    rect_month.bottom = rect_month.top + 32+ 1;                    
    
    GUISTR_DrawTextToLCDInRect( 
                                (const GUI_LCD_DEV_INFO *)(&lcd_dev),
                                (const GUI_RECT_T *)&rect_month,
                                (const GUI_RECT_T *)&rect_month,
                                (const MMI_STRING_T *)&month_string,
                                &text_style1,
                                state_month,
                                GUISTR_TEXT_DIR_AUTO
                                );
    #endif
 
    MMIIDLE_GetIdleTextStyle(&text_style2 , SONG_FONT_38,0xF840);   //xiongkai      MMI_RED_COLOR   {250,5,5} = 0xF840    //SONG_FONT_36 MMI_BLACK_COLOR
    text_style2.align = ALIGN_HMIDDLE;
    text_style2.effect = 0;
    
    rect_week.left = 1; //(lcd_width>>1) -8 +4;
    rect_week.right = lcd_width;
    rect_week.top = 102;   // 73
    rect_week.bottom = rect_week.top + 32+ 1;         

	GUISTR_DrawTextToLCDInRect( 
							(const GUI_LCD_DEV_INFO *)(&lcd_dev),
							(const GUI_RECT_T *)&rect_week,
							(const GUI_RECT_T *)&rect_week,
							(const MMI_STRING_T *)&week_string,
							&text_style2,
							state_week,
							GUISTR_TEXT_DIR_AUTO
							);

	
}
#else
LOCAL void DisplayDate(int16 time_top)
{
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    SCI_DATE_T date = {0};
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T m_string = {0};
    MMI_STRING_T final_string = {0};
    wchar m_buf[5] ={0}; 
    wchar datewstr[30] = {0};
    wchar wstr_buf[20] = {0};
    uint8 datestr[20] = {0};
    GUI_RECT_T rect = {0};
    GUI_LCD_DEV_INFO lcd_dev = {0};
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
#ifdef MAINLCD_SIZE_128X160	
    MMI_TEXT_ID_T week_table[] = {
                                        TXT_IDLE_SHORT_SUNDAY,
                                        TXT_IDLE_SHORT_MONDAY,
                                        TXT_IDLE_SHORT_TUESDAY,
                                        TXT_IDLE_SHORT_WEDNESDAY,
                                        TXT_IDLE_SHORT_THURSDAY,
                                        TXT_IDLE_SHORT_FRIDAY,
                                        TXT_IDLE_SHORT_SATURDAY
    };
#else
    MMI_TEXT_ID_T week_table[] = {
                                    TXT_SHORT_SUN, //TXT_SUNDAY,//       TXT_SHORT_IDLE_SUNDAY,
                                    TXT_SHORT_MON, //TXT_MONDAY,//       TXT_SHORT_IDLE_MONDAY,
                                    TXT_SHORT_TUE, //TXT_TUESDAY,//      TXT_SHORT_IDLE_TUESDAY,
                                    TXT_SHORT_WED,  //TXT_WEDNESDAY,//       TXT_SHORT_IDLE_WEDNESDAY,
                                    TXT_SHORT_THU,  //TXT_THURSDAY,//       TXT_SHORT_IDLE_THURSDAY,
                                    TXT_SHORT_FRI,  //TXT_FRIDAY,//       TXT_SHORT_IDLE_FRIDAY,
                                    TXT_SHORT_SAT,  //TXT_SATURDAY,//       TXT_SHORT_IDLE_SATURDAY
                                 };
#endif
    SCI_MEMSET(m_buf,0,sizeof(m_buf));
    SCI_MEMSET(datewstr,0,sizeof(datewstr));
    SCI_MEMSET(datestr,0,sizeof(datestr));
    SCI_MEMSET(wstr_buf,0,sizeof(wstr_buf));   

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    m_string.wstr_ptr = m_buf;
    TM_GetSysDate(&date);
    MMI_GetLabelTextByLang(week_table[date.wday], &m_string);
    
    MMIAPISET_GetCurrentDateStr(datestr,'-',sizeof(datestr));
    
    strncat(datestr," ",1);/*lint !e64*/
    MMIAPICOM_StrToWstr(datestr, wstr_buf);
    
    final_string.wstr_ptr = datewstr;
    MMIAPICOM_Wstrncpy(final_string.wstr_ptr,wstr_buf,MMIAPICOM_Wstrlen(wstr_buf));
    MMIAPICOM_Wstrncat(final_string.wstr_ptr, m_string.wstr_ptr,m_string.wstr_len);
    
    
    final_string.wstr_len = (uint16)MMIAPICOM_Wstrlen(final_string.wstr_ptr);
    
    //text_style.font_color = MMI_WHITE_COLOR;
    //text_style.edge_color = MMI_BLACK_COLOR;
    //text_style.font = MMI_DEFAULT_SMALL_FONT;
	MMIIDLE_GetIdleTextStyle(&text_style , SONG_FONT_36,MMI_WHITE_COLOR);
    text_style.align = ALIGN_HMIDDLE;

    rect.left = 0;
    rect.right = lcd_width;
    rect.top = time_top + KEYLOCK_DATE_TOP_OFFSET + 15 + 8 ;
    rect.bottom = rect.top + KEYLOCK_DATE_FONT_HEIGHT + 1;                    
    GUISTR_DrawTextToLCDInRect( 
                                (const GUI_LCD_DEV_INFO *)(&lcd_dev),
                                (const GUI_RECT_T *)&rect,
                                (const GUI_RECT_T *)&rect,
                                (const MMI_STRING_T *)&final_string,
                                &text_style,
                                state,
                                GUISTR_TEXT_DIR_AUTO
                                );
}
#endif
#if defined(ZDT_KEYLOCK_SCREEN_BIG_DAY_STYLE)
LOCAL void OutputKLDayBigImg(void)
{
    uint16  image_width = 0,image_height = 0;
    uint16  lcd_width = 0,lcd_height = 0;    
    SCI_DATE_T date = {0};
    GUI_POINT_T  point1 = {0};
    GUI_POINT_T  point2 = {0};
    GUI_LCD_DEV_INFO	 lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;	
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_BIG_DAY_0,MMIKL_CLOCK_DISP_WIN_ID);
    
    TM_GetSysDate(&date);   

    point1.x = 0;
    point2.x = 0;
    point1.y = 140; //  128;
    point2.y = 140; // 128;
    
    if(date.mday <10)  //  1~~9
    {
        //point1.x= (lcd_width-image_width)>>1;
        //GUIRES_DisplayImg(&point1,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_BIG_DAY_0 +date.mday,&lcd_dev_info);   
        point1.x= (lcd_width - 2*image_width -4)>>1;    
        point2.x= point1.x +image_width + 4 ;   
        GUIRES_DisplayImg(&point1,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_BIG_DAY_0,&lcd_dev_info);   
        GUIRES_DisplayImg(&point2,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_BIG_DAY_0 + date.mday,&lcd_dev_info);             
    }
    else  //  10~~31
    {
        point1.x= (lcd_width - 2*image_width -4)>>1;    
        point2.x= point1.x +image_width + 4 ;   
        GUIRES_DisplayImg(&point1,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_BIG_DAY_0 +date.mday/10,&lcd_dev_info);      
        if((date.mday%10) == 0)
        {
            GUIRES_DisplayImg(&point2,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_BIG_DAY_0,&lcd_dev_info);             
        }
        else
        {
            GUIRES_DisplayImg(&point2,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_BIG_DAY_0 + (date.mday%10),&lcd_dev_info);     
        }
    }  
}

LOCAL void DisplayLockDate( void)
{
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    SCI_DATE_T date = {0};
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T m_string = {0};
    MMI_STRING_T final_string = {0};
    wchar m_buf[5] ={0}; 
    wchar datewstr[30] = {0};
    wchar wstr_buf[20] = {0};
    uint8 datestr[20] = {0};
    GUI_RECT_T rect = {0};
    GUI_LCD_DEV_INFO lcd_dev =  {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
  GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;   
    MMI_TEXT_ID_T week_table[] = {
                                                                STXT_SUNDAY,//       TXT_SHORT_IDLE_SUNDAY,
								        STXT_MONDAY,//       TXT_SHORT_IDLE_MONDAY,
								        STXT_TUESDAY,//      TXT_SHORT_IDLE_TUESDAY,
								        STXT_WEDNESDAY,//       TXT_SHORT_IDLE_WEDNESDAY,
								        STXT_THURSDAY,//       TXT_SHORT_IDLE_THURSDAY,
								        STXT_FRIDAY,//       TXT_SHORT_IDLE_FRIDAY,
								        STXT_SATURDAY,//       TXT_SHORT_IDLE_SATURDAY
                                 };
    SCI_MEMSET(m_buf,0,sizeof(m_buf));
    SCI_MEMSET(datewstr,0,sizeof(datewstr));
    SCI_MEMSET(datestr,0,sizeof(datestr));
    SCI_MEMSET(wstr_buf,0,sizeof(wstr_buf));   

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    m_string.wstr_ptr = m_buf;
    TM_GetSysDate(&date);
    MMI_GetLabelTextByLang(week_table[date.wday], &m_string);

    final_string.wstr_ptr = datewstr;
    MMIAPICOM_Wstrncpy(final_string.wstr_ptr,m_string.wstr_ptr,m_string.wstr_len);
    final_string.wstr_len = (uint16)MMIAPICOM_Wstrlen(final_string.wstr_ptr);
    
    text_style.font = SONG_FONT_36;
    text_style.align = ALIGN_HMIDDLE;
    text_style.edge_alpha = 0xff;	
    rect.left = 0;
	rect.right = lcd_width;
	rect.top = 85;
	rect.bottom = rect.top + 38; 
    
    text_style.font_color = MMI_DARK_RED_COLOR;
    text_style.align = ALIGN_HMIDDLE;
    GUISTR_DrawTextToLCDInRect( 
                                (const GUI_LCD_DEV_INFO *)(&lcd_dev),
                                (const GUI_RECT_T *)&rect,
                                (const GUI_RECT_T *)&rect,
                                (const MMI_STRING_T *)&final_string,
                                &text_style,
                                text_state,
                                GUISTR_TEXT_DIR_AUTO
                                );
}
#endif


#ifdef MMI_KEYLOCK_MINI_SUPPORT
LOCAL void DisplayMissedEventDC(void)
{
    uint16 img_width = 0;
    uint16 img_height = 0;    
    uint16 lcd_width = 0, lcd_height = 0; 
    GUI_RECT_T rect = {0};
    GUI_LCD_DEV_INFO lcd_dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
#ifdef MAINLCD_SIZE_176X220
    BOOLEAN is_landscape = FALSE;
    
    is_landscape = MMITHEME_IsMainScreenLandscape();
#endif

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);    
    GUIRES_GetImgWidthHeight(&img_width,&img_height, IMAGE_QQVGA_KEYLOCK_MISS_CALL, MMIKL_CLOCK_DISP_WIN_ID);

    rect.right = lcd_width - 1;         
    rect.bottom = rect.top + lcd_height;

#ifdef MAINLCD_SIZE_176X220    
    if(is_landscape)
    {
            rect.left = KEY_LOCK_QQVGA_MISSEVENT_LEFT_H;
            rect.top = KEY_LOCK_QQVGA_MISSEVENT_TOP_H;
    }
    else
    {
#endif    
            rect.left = KEY_LOCK_QQVGA_MISSEVENT_LEFT;
            rect.top = KEY_LOCK_QQVGA_MISSEVENT_TOP;
#ifdef MAINLCD_SIZE_176X220            
    }
#endif
   
    MMIKL_DisplayMissedEvent(MMIKL_CLOCK_DISP_WIN_ID, &rect, &lcd_dev);
}
#endif

#ifndef TOUCH_PANEL_SUPPORT 
/*****************************************************************************/
//  Description :  Display Missed event of effect type1.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayMissedEvent(void)
{
    GUISTR_STYLE_T      style= {0};
    MMI_STRING_T        string = {0};
    GUISTR_INFO_T       info = {0};    

    GUI_LCD_DEV_INFO    lcd_dev = {0};
    GUI_RECT_T          rect = MMITHEME_GetClientRect();

    style.font = MMI_DEFAULT_NORMAL_FONT;

    MMI_GetLabelTextByLang(TXT_KL_MISS_CALLS, &string);

    GUISTR_GetStringInfo(&style, &string, GUISTR_STATE_NONE, &info);

    rect.top = (rect.bottom - rect.top)/2;
    rect.bottom = rect.top + info.height * 2 + 8;
    MMIKL_DisplayMissedEvent(MMIKL_CLOCK_DISP_WIN_ID, &rect, &lcd_dev);

}
#endif

//  Description : DisplayClockCallbackFun
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayClockCallbackFun (GUIANIM_OWNER_DRAW_T    *draw_ptr)
{
#ifdef MMI_KEYLOCK_USE_ANALOG_CLOCK
    DrawAClock(MMIKL_CLOCK_DISP_WIN_ID,draw_ptr->display_y);
#else
    GUI_POINT_T        	 		point = {0};
    SCI_TIME_T         	 		time = {0};
    GUI_LCD_DEV_INFO			lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMISET_TIME_DISPLAY_TYPE_E  time_display_type = MMIAPISET_GetTimeDisplayType();
    BOOLEAN		       			b_am = TRUE;
    MMISET_LANGUAGE_TYPE_E language_type;
    uint16                      image_width = 0,image_height = 0;
    uint16                      bg_width = 0,bg_height = 0;
    uint16                      time_total_width = 0;
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;	
    TM_GetSysTime(&time);
    MMIAPISET_GetLanguageType(&language_type);

    if(time_display_type == MMISET_TIME_12HOURS)
    {
        GET_12STYLE_HOUR(time.hour, b_am);
    }

    //计算时间总的宽度，为了居中显示
    GUIRES_GetImgWidthHeight(&bg_width,&bg_height, IMAGE_IDLE_KEYLOCK_TIME_BG,MMIKL_CLOCK_DISP_WIN_ID);
#ifdef ZDT_KEYLOCK_SCREEN_BIG_DAY_STYLE
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_BIG_TIME_COLON,MMIKL_CLOCK_DISP_WIN_ID);
#else
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_TIME_COLON,MMIKL_CLOCK_DISP_WIN_ID);
#endif
    time_total_width = time_total_width  + image_width;//colon
  
    if(time_display_type == MMISET_TIME_12HOURS)
    {
	 if(language_type != MMISET_LANGUAGE_SIMP_CHINESE )
	 {
	#ifdef ZDT_KEYLOCK_SCREEN_BIG_DAY_STYLE 
	 GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_BIG_TIME_AM_EN,MMIKL_CLOCK_DISP_WIN_ID);	 
	#else
        GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_TIME_AM_ENG,MMIKL_CLOCK_DISP_WIN_ID);	 
	#endif
	 }
	 else
	 {
#ifdef ZDT_KEYLOCK_SCREEN_BIG_DAY_STYLE        
        GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_BIG_TIME_AM,MMIKL_CLOCK_DISP_WIN_ID);
#else
        GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_TIME_AM,MMIKL_CLOCK_DISP_WIN_ID);
#endif		 
	 }
        time_total_width = time_total_width  + image_width;//am.pm
    }
#ifdef ZDT_KEYLOCK_SCREEN_BIG_DAY_STYLE                 
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_BIG_TIME_0,MMIKL_CLOCK_DISP_WIN_ID);
#else
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_TIME_0,MMIKL_CLOCK_DISP_WIN_ID);
#endif
    time_total_width = time_total_width + image_width * 4 ;//有四个数字
    
    point.y = 30; //MAX(draw_ptr->display_y,draw_ptr->display_rect.top) + KEYLOCK_TIME_TOP_OFFSET + 15;//fixed by hongjun.jia for NEWMS00184127
	point.x = MAX(draw_ptr->display_x,draw_ptr->display_rect.left);
    
    if(bg_width > time_total_width)
    {
        point.x = (bg_width - time_total_width)/2 + point.x;
    }
    if(time_display_type == MMISET_TIME_12HOURS)
    { 
        point.x = 0;
    }
    else
    { 
        point.x = 14;
    }      
    //HOUR
#ifndef ZDT_KEYLOCK_SCREEN_BIG_DAY_STYLE     
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_TIME_0 + time.hour/10,&lcd_dev_info);
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_TIME_0 + time.hour%10,&lcd_dev_info);
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_TIME_COLON,&lcd_dev_info);
    //COLON IMAGE WIDTH
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_TIME_COLON,MMIKL_CLOCK_DISP_WIN_ID);
    //MIN
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_TIME_0 + time.min/10,&lcd_dev_info);

    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_TIME_0,MMIKL_CLOCK_DISP_WIN_ID);
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_TIME_0 + time.min%10,&lcd_dev_info);
    //AM.PM
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        point.x = point.x + image_width;
        if(b_am )
        {
		if(language_type != MMISET_LANGUAGE_SIMP_CHINESE )
		{
			GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_TIME_AM_ENG,&lcd_dev_info);
		}
		else
		{
            		GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_TIME_AM,&lcd_dev_info);
		}
        }
        else
        {
		if(language_type != MMISET_LANGUAGE_SIMP_CHINESE )
		{
			GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_TIME_PM_ENG,&lcd_dev_info);
		}
		else
		{
            		GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_TIME_PM,&lcd_dev_info);
		}
        }
    }
#else
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_KL_BIG_TIME_0 + time.hour/10,&lcd_dev_info);
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_KL_BIG_TIME_0 + time.hour%10,&lcd_dev_info);
    point.x = point.x + image_width- 10;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_KL_BIG_TIME_COLON,&lcd_dev_info);
    //COLON IMAGE WIDTH
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_BIG_TIME_COLON,MMIKL_CLOCK_DISP_WIN_ID);
    //MIN
    point.x = point.x + image_width -10;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_KL_BIG_TIME_0 + time.min/10,&lcd_dev_info);

    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_BIG_TIME_0,MMIKL_CLOCK_DISP_WIN_ID);
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_KL_BIG_TIME_0 + time.min%10,&lcd_dev_info);
    //AM.PM
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        point.x = point.x + image_width;
        if(b_am )
        {
		if(language_type != MMISET_LANGUAGE_SIMP_CHINESE )
		{
			GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_KL_BIG_TIME_AM_EN,&lcd_dev_info);
		}
		else
		{
            		GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_KL_BIG_TIME_AM,&lcd_dev_info);
		}
        }
        else
        {
		if(language_type != MMISET_LANGUAGE_SIMP_CHINESE )
		{
			GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_KL_BIG_TIME_PM_EN,&lcd_dev_info);
		}
		else
		{
            		GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_KL_BIG_TIME_PM,&lcd_dev_info);
		}
        }
    }
#endif
    #ifdef MAINLCD_SIZE_240X400
    point.y+=2*KEYLOCK_TIME_TOP_OFFSET;
    #endif
    DisplayDate(point.y);

#if defined(ZDT_KEYLOCK_SCREEN_BIG_DAY_STYLE)
    OutputKLDayBigImg();
#endif    
	
    OutputKLLunarDate();
#if  !defined(ZDT_KEYLOCK_SCREEN_BIG_DAY_STYLE)
	 if(0 == MMIAPIPHONE_GetSimCardOkNum(PNULL, 0))
	{
		 DisplayKLSimStatus(MN_DUAL_SYS_1);
	}
       else   if(1 == MMIAPIPHONE_GetSimCardOkNum(PNULL, 0))
	{
		  if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
		  {
			 DisplayKLSimStatus(MN_DUAL_SYS_1);
		  }
		  else   if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
		  {
			 DisplayKLSimStatus(MN_DUAL_SYS_2);
		  }
	}
	 else
	 {
    		DisplayKLSimStatus(MN_DUAL_SYS_1);
    		DisplayKLSimStatus(MN_DUAL_SYS_2);
	 }
#endif	 
#ifdef MMI_KEYLOCK_MINI_SUPPORT
   ///   DisplayMissedEventDC();
#endif
#endif
#ifndef TOUCH_PANEL_SUPPORT 
#ifndef MMI_KEYLOCK_MINI_SUPPORT
  //  DisplayMissedEvent();
#endif
#endif
}

/*****************************************************************************/
//  FUNCTION:       SetWCFormParam
// 	Description :   set the from parameter
//	Global resource dependence : 
//  Author:         murphy.xie
//  Date:           2010/05/12
//	Note:   
/*****************************************************************************/
LOCAL void SetKeyLockControlParam(void)
{
    GUI_BG_T                    bg = {0};
    GUIANIM_DATA_INFO_T         data_info =    {0};
    GUIANIM_DISPLAY_INFO_T      display_info = {0};
    GUIANIM_CTRL_INFO_T         control_info = {0};

    //set control rect info
    SwitchControlRectInfo();
    
    //set status bar
#ifndef EFFECT_RIPPLE_SUPPORT //144786 
	GUIWIN_SetStbState(MMIKL_CLOCK_DISP_WIN_ID, GUISTATUSBAR_STATE_USE_LAYER, TRUE);

#if defined (MAINLCD_SIZE_240X320)
    GUIWIN_SetStbBgIsDesplay(MMIKL_CLOCK_DISP_WIN_ID, FALSE);
#endif

	#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR    
	//GUIWIN_SetStbBgImageId(MMIKL_CLOCK_DISP_WIN_ID, IMAGE_IDLE_STATUS_BAR);
	#endif
#endif
    //set softkey common bg
    //GUIWIN_SetSoftkeyCommonBg(MMIKL_CLOCK_DISP_WIN_ID,&bg);
    //GUIWIN_SetSoftkeyTextId(MMIKL_CLOCK_DISP_WIN_ID,TXT_IDLE_UNLOCK,TXT_NULL,TXT_NULL,FALSE);
    DrawSoftkey(MMIKL_CLOCK_DISP_WIN_ID); 
    
    //set timer owerdraw bg
    #ifdef MMI_KEYLOCK_USE_ANALOG_CLOCK
    data_info.img_id = IMAGE_KEYLOCK_ACLOCK;//IMAGE_IDLE_KEYLOCK_TIME_BG;
    #else
    data_info.img_id = IMAGE_IDLE_KEYLOCK_TIME_BG;
    #endif
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = MMIKL_CLOCK_ANIM_ID;
    control_info.ctrl_ptr = PNULL;
    
    display_info.is_syn_decode = TRUE;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    GUIANIM_SetOwnerDraw(MMIKL_CLOCK_ANIM_ID,DisplayClockCallbackFun);
	
#ifdef TOUCH_PANEL_SUPPORT 
    //set lock icon botton
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_IDLE_KEYLOCK_PRESS_DOWN;
    GUIBUTTON_SetRunSheen(MMIKL_PRESS_BOTTON_ID,FALSE);
    GUIBUTTON_SetPressedBg(MMIKL_PRESS_BOTTON_ID, &bg);
    GUIBUTTON_SetHandleLong(MMIKL_PRESS_BOTTON_ID, TRUE);
    MMK_SetAtvCtrl(MMIKL_CLOCK_DISP_WIN_ID,MMIKL_PRESS_BOTTON_ID);
       

    //set info bg
    display_info.align_style = GUIANIM_ALIGN_HMIDDLE_BOTTOM;
    data_info.img_id = IMAGE_IDLE_KEYLOCK_HINT;
    control_info.ctrl_id = MMIKL_PRESS_ANIM_EVENT_ID;
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    GUIANIM_SetOwnerDraw(MMIKL_PRESS_ANIM_EVENT_ID,PressFormInfoCtrlCallbackFun);


    //message
    data_info.img_id = IMAGE_IDLE_KEYLOCK_MESSAGE;
    control_info.ctrl_id = MMIKL_PRESS_ANIM_MESSAGE_ID;
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    GUIANIM_SetOwnerDraw(MMIKL_PRESS_ANIM_MESSAGE_ID,PressFormInfoCtrlCallbackFun);

    
    //call
    data_info.img_id = IMAGE_IDLE_KEYLOCK_CALL;
    control_info.ctrl_id = MMIKL_PRESS_ANIM_CALL_ID;
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    GUIANIM_SetOwnerDraw(MMIKL_PRESS_ANIM_CALL_ID,PressFormInfoCtrlCallbackFun);


    //set press animal bg
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = MMIKL_PRESS_ANIM_BG_ID;
    control_info.ctrl_ptr = PNULL;
    
    display_info.is_syn_decode = TRUE;
    display_info.align_style = GUIANIM_ALIGN_LEFT_VMIDDLE;
    display_info.bg.bg_type = GUI_BG_IMG;
    display_info.bg.img_id  = IMAGE_IDLE_KEYLOCK_PRESS_BG;
    
    GUIANIM_SetParam(&control_info,PNULL,PNULL,&display_info);
    
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_NULL;
    GUIANIM_SetErrorBg(MMIKL_PRESS_ANIM_BG_ID,&bg);
#endif
}
#endif

/*****************************************************************************/
//  Description : key lock init
//  Global resource dependence :
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_Init(void)
{
    SCI_MEMSET(&s_st_mmi_kl_event, 0, sizeof(s_st_mmi_kl_event));
    s_e_mmi_kl_win_status = KL_UNLOCKED;
    return TRUE;
}
/*****************************************************************************/
//  Description : get win status
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC MMI_KL_WIN_STATUS_E MMIKL_GetWinStatus(void)
{
    return s_e_mmi_kl_win_status;
}

#ifdef HERO_ENGINE_TTS_SUPPORT 
#include "heroEngine_mmi_text.h"
extern void hero_tts_lock_screen_broadcast(BOOLEAN isLock);
#endif
/*****************************************************************************/
//  Description : set win status
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKL_SetWinStatus(MMI_KL_WIN_STATUS_E status)
{
    if (!MMIKL_IsPreviewState())
    {
        s_e_mmi_kl_win_status = status;
    }
}

/*****************************************************************************/
//  Description : reset win status
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKL_ReSetWinStatus(void)
{
    s_e_mmi_kl_win_status = KL_UNLOCKED;
}
/*****************************************************************************/
//  Description : lock phone handle louse focus msg
//  Global resource dependence :
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleKLLouseFocus(void)
{
    //GUIWIN_SetSoftkeyTextId(MMIKL_CLOCK_DISP_WIN_ID, TXT_IDLE_UNLOCK, TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, TRUE);
	DrawSoftkey(MMIKL_CLOCK_DISP_WIN_ID);
    //s_is_press_down = FALSE;
    return TRUE;
}
PUBLIC void MMIAPIKL_CloseAllLight(void)
{
    SCI_TRACE_LOW("MMIAPIKL_CloseAllLight: ");

    if (!MMK_IsOpenWin(MMIKL_CLOCK_DISP_WIN_ID))
    {
        SCI_TRACE_LOW("MMIAPIKL_CloseAllLight: close now");
#ifdef LOCK_SCREEN_SPECIAL_EFFECT_SUPPORT
        MMISEPC_CloseAllLight(); 
#else
        MMIDEFAULT_CloseAllLight();
#endif
    }
    else
    {
        SCI_TRACE_LOW("MMIAPIKL_CloseAllLight: close post message ");
        //MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID, MSG_KEY_LOCK_CLOSE_LIGHT, NULL, 0);
    }
}

/*****************************************************************************/
//  Description : lock phone
//  Global resource dependence :
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_LockPhone(void)
{
#ifdef MMI_DEVICELOCK_SUPPORT
    return MMIAPIDEVICELOCK_LockPhone();
#else
#if 0//def MMI_MSTYLE_SUPPORT
    return MMIAPIIDLE_LockPhone();
#else
    MMI_WIN_ID_T    win_id  = MMIKL_CLOCK_DISP_WIN_ID;
    SCI_MEMSET(&s_st_mmi_kl_event, 0, sizeof(s_st_mmi_kl_event));
    if (MMIAPITP_Coordinate_IsWinOpen())
    {
        return FALSE;
    }

    if (MMIKL_IsPreviewState())
    {
        MMK_CloseWin(win_id);
    }

    s_e_mmi_kl_win_status = KL_LOCKED;
    MMIAPISET_SetIsNeedValidatePrivacy(TRUE);
    if (!MMK_IsOpenWin(win_id))
    {
        MMITHEME_SetKeyLockWin(TRUE);
#ifdef HERO_ENGINE_TTS_SUPPORT
		{
				hero_tts_lock_screen_broadcast(TRUE);
				//MMIDEFAULT_CloseAllLight();
				//MMIPUB_OpenAlertWarningWin(TXT_HERO_TTS_LOCK_SCREEN_LOCKED);
		}
#endif
       #ifndef TTS_SUPPORT_NONE
#ifdef PHONE_NUMBER_VOICE
	#ifndef WIN32
	Lock_To_Speech();
	#endif
#endif
#endif
 
#ifndef LOCK_SCREEN_SPECIAL_EFFECT_SUPPORT 
    ///    MMIAPIKL_PlayKeyLockRing();
#endif		
#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
        MMK_CreateWin((uint32 *)MMIKC_DISP_WINTAB, PNULL);
#endif
        MMITHEME_SetKeyLockWin(FALSE);
    }
    else
    {
        MMK_WinGrabFocus(win_id);
//      MMK_SendMsg(win_id,MSG_GET_FOCUS,PNULL);
    }
    
    
    return TRUE;
#endif
#endif
}
/*****************************************************************************/
//  Description : open lock preview phone
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKL_OpenPreviewLockPhone(uint16 cur_item_index)
{
    s_e_mmi_kl_win_status = KL_PREVIEW;
#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
    MMK_CreateWin((uint32 *)MMIKC_DISP_WINTAB, (ADD_DATA)cur_item_index);
#endif
}
/*****************************************************************************/
//  Description : is preview state
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_IsPreviewState(void)
{
    BOOLEAN result = FALSE;

    if (KL_PREVIEW == s_e_mmi_kl_win_status)
    {
        result = TRUE;
    }
    return result;
}
/*****************************************************************************/
//  Description : unlock phone
//  Global resource dependence :
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_UnlockPhone(void)
{
#ifdef MMI_DEVICELOCK_SUPPORT
    return MMIAPIDEVICELOCK_UnlockPhone();
#else //#ifdef MMI_DEVICELOCK_SUPPORT
#if 0//def MMI_MSTYLE_SUPPORT

    return MMIAPIIDLE_UnlockPhone();

#else //#ifdef MMI_MSTYLE_SUPPORT

    MMI_COMMON_WINDOW_ID_E  win_id = MMIKL_CLOCK_DISP_WIN_ID;

    if (MMK_IsOpenWin(win_id))
    {
#ifdef MMIWIDGET_WEATHER_ANIM        
        MMK_PostMsg(MAIN_IDLE_WIN_ID, MSG_WEATHER_FORBID_ANIM, PNULL, 0);
#endif
#ifdef HERO_ENGINE_TTS_SUPPORT
	{
	    hero_tts_lock_screen_broadcast(FALSE);
	}
#endif
        MMK_CloseWin(win_id);
    }
    else
    {
        //do nothing
    }
    
    return TRUE;
    
#endif //#ifdef MMI_MSTYLE_SUPPORT
#endif //#ifdef MMI_DEVICELOCK_SUPPORT
}

/*****************************************************************************/
//  Description : get whether phone is locked
//  Global resource dependence :
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_IsPhoneLocked(void)
{
#ifdef MMI_DEVICELOCK_SUPPORT
    return MMIAPIDEVICELOCK_IsPhoneLocked();
#else //#ifdef MMI_DEVICELOCK_SUPPORT
#if 0//def MMI_MSTYLE_SUPPORT
    return MMIAPIIDLE_IsPhoneLocked();

#else //#ifdef MMI_MSTYLE_SUPPORT
    SCI_TRACE_LOW("s_e_mmi_kl_win_status is %d",s_e_mmi_kl_win_status);
    switch(s_e_mmi_kl_win_status)
    {
    case KL_LOCKED:             //锁定。此时界面左软键显示"解锁"。
        return TRUE;
        //break;

    case KL_UNLOCKED_ONCE:      //解锁操作一次。此时界面右软键显示"解锁"。
        return TRUE;
        //break;

    case KL_UNLOCKED:           //解锁。此时应该关闭锁定窗口。
        return FALSE;
        //break;

    case KL_STATUS_MAX:
        return FALSE;
        //break;

    default:
        return FALSE;
        //break;
    }

#endif //#ifdef MMI_MSTYLE_SUPPORT
#endif //#ifdef MMI_DEVICELOCK_SUPPORT

}

/*****************************************************************************/
//  Description : set new sms arrived status
//  Global resource dependence :
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_SetNewSMSStatus(void)
{
    s_st_mmi_kl_event.is_new_sms_alerted = TRUE;
    
    return TRUE;
}

#if defined(MMI_VCARD_SUPPORT)
/*****************************************************************************/
//  Description : set new vcard arrived status
//  Global resource dependence :
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_SetNewVcardStatus(void)
{
    s_st_mmi_kl_event.is_new_vcard_alerted = TRUE;
    
    return TRUE;
}
#endif

#if defined(MMI_VCALENDAR_SUPPORT)
/*****************************************************************************/
//  Description : set new vcalendar arrived status
//  Global resource dependence :
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_SetNewVcalendarStatus(void)
{
    s_st_mmi_kl_event.is_new_vcalendar_alerted = TRUE;
    
    return TRUE;
}
#endif

/*****************************************************************************/
//  Description : set new mms arrived status
//  Global resource dependence :
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN MMIKL_SetNewMMSStatus(void)
//{
//  s_st_mmi_kl_event.is_new_mms_alerted = TRUE;
//
//  return TRUE;
// }

/*****************************************************************************/
//  Description : set missed call status
//  Global resource dependence :
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_SetMissedCallStatus(void)
{
    s_st_mmi_kl_event.is_missed_call = TRUE;

    return TRUE;
}

/*****************************************************************************/
//  Description : send minute and hour interrupt to kl window
//  Global resource dependence :
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKL_SendHourOrMinuteInd(void)
{
    if (MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID))
    {
        MMK_SendMsg(MMIKL_CLOCK_DISP_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
}

LOCAL BOOLEAN DrawSoftkey(MMI_WIN_ID_T win_id)
{
    MMISET_WALLPAPER_SET_T          wallpaper_info      = {0};

#ifdef ZDT_PRODUCT_JGT
	COMMON_MMI_IMAGE_ID_E	image_id =	 IMAGE_WALLPAPER_1 ;//IMAGE_COMMON_BG ;//IMAGE_WALLPAPER_1_JGT;
#else
	COMMON_MMI_IMAGE_ID_E	image_id = IMAGE_WALLPAPER_1 ;//IMAGE_IDLE_KEYLOCK_BG ;// IMAGE_IDLE_KEYLOCK;
#endif
	GUI_RECT_T   rect_bmp = {0,280,239,319};
	GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
	GUI_RECT_T  rect_soft_text ={0};
	GUI_POINT_T 	point_bmp = {0};
	GUI_POINT_T 	point_slide_unlock = {0};
	GUI_POINT_T 	softkey_point = {0};
	MMI_STRING_T	unlock_str = {0};
	MMI_STRING_T	softkey_str = {0};
	GUI_FONT_T		font = SONG_FONT_38;
	uint16			string_width = 0;
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	wchar					full_str[MMI_MAINSCREEN_RIGHT_MAX_PIXEL] = {0};
	uint16					full_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
	
	//MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
	if( wallpaper_info.wallpaper_type != MMISET_WALLPAPER_FIXED_PIC)
	{
	 /// wallpaper_info.wallpaper_type	= MMISET_WALLPAPER_FIXED_PIC;
	// wallpaper_info.wallpaper_id = 0;
	}
	//image_id = MMIAPISET_GetWallpaperByIndex(wallpaper_info.wallpaper_id);

      #if defined(ZDT_KEYLOCK_SCREEN_BIG_DAY_STYLE)
          return TRUE;
      #endif


	GUIRES_DisplayImg(PNULL,
		&rect_bmp,
		&rect_bmp,
		win_id,
		image_id,
		&lcd_dev_info);
					
	//draw tip for unlock

	if(KL_UNLOCKED_ONCE == s_e_mmi_kl_win_status)
	{
#if 0
//!(defined( ZDT_PRODUCT_JGT)||defined(ZDT_PRODUCT_DX))

		MMI_GetLabelTextByLang(TXT_COMMON_OK, &softkey_str);
		
		string_width = GUI_CalculateStringPiexlNum(softkey_str.wstr_ptr,
								softkey_str.wstr_len,
								font,
								0);
		
	

		rect_soft_text.left = MMI_MAINSCREEN_WIDTH-string_width-3;
		rect_soft_text.top = MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
		rect_soft_text.right = MMI_MAINSCREEN_WIDTH;
		rect_soft_text.bottom = 319;
	 #ifdef  ZDT_PRODUCT_JG
	 MMIIDLE_GetIdleTextStyle(&text_style, font,  MMI_BACKGROUND_COLOR);
   #else	
		MMIIDLE_GetIdleTextStyle(&text_style, font,  MMI_BACKGROUND_COLOR);
   #endif
		GUISTR_DrawTextToLCDInRect( 
			(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
			(const GUI_RECT_T	   *)&rect_soft_text,
			(const GUI_RECT_T	   *)&rect_soft_text,
			(const MMI_STRING_T 	*)&softkey_str,
			&text_style,
			text_state,
			GUISTR_TEXT_DIR_AUTO
			);

#endif
	}
	else
	{
		MMI_GetLabelTextByLang(TXT_IDLE_UNLOCK, &softkey_str);
		string_width = GUI_CalculateStringPiexlNum(
								softkey_str.wstr_ptr,
								softkey_str.wstr_len,
								font,
								0
								);

	#if 1//def 	ZDT_PRODUCT_WDL // wuxx del

		rect_soft_text.left = MMI_MAINSCREEN_WIDTH-string_width-3;
			rect_soft_text.top = MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
			rect_soft_text.right = MMI_MAINSCREEN_WIDTH;
			rect_soft_text.bottom = 319;

	#else
		rect_soft_text.left = 3;
		rect_soft_text.top = MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
		rect_soft_text.right = string_width + 26;
		rect_soft_text.bottom = 319;

	#endif
	 #ifdef  ZDT_PRODUCT_JG
	 MMIIDLE_GetIdleTextStyle(&text_style, font,  MMI_BLACK_COLOR);
   #else			
		MMIIDLE_GetIdleTextStyle(&text_style, font,  MMI_BLACK_COLOR);
   #endif
		GUISTR_DrawTextToLCDInRect( 
			(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
			(const GUI_RECT_T	   *)&rect_soft_text,
			(const GUI_RECT_T	   *)&rect_soft_text,
			(const MMI_STRING_T 	*)&softkey_str,
			&text_style,
			text_state,
			GUISTR_TEXT_DIR_AUTO
			);

		
	}


	return TRUE;
}

#ifdef MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT 
/*****************************************************************************/
//  Description : HandlePenLongOk
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void HandleKeyLockPenLongOk(MMI_WIN_ID_T win_id, DPARAM param)
{
    if(param == PNULL)
        return;
    else
    {
        MMK_PostMsg(win_id, MSG_KEY_LOCK_CLOSE_WIN,NULL, 0);
    }
}
#endif
LOCAL void DisplayUnLockedTips(MMI_TEXT_ID_T  tips)
{
    uint16                      str_width = 0;
    GUI_RECT_T                  tips_rect = {0};
    MMI_STRING_T                tips_string = {0};
    GUISTR_STYLE_T              str_style = {0};
    GUISTR_STATE_T              str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK;
    uint16                      image_width = 0,image_height = 0;
    uint16                      lcd_width = 0;
    uint16                      lcd_height = 0;
    uint8                       str_margin = 10;
    GUI_LCD_DEV_INFO            layer_handle_ptr = {0};
#ifdef MAINLCD_SIZE_176X220
    GUISTR_INFO_T               str_info = {0};
	uint16                      str_height = 0;
#endif
    
  //  if (UILAYER_IsLayerActive(layer_handle_ptr))
    {
#ifdef MAINLCD_SIZE_176X220

#else
		//get rect info
        GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_PUBWIN_BG,MMIKL_CLOCK_DISP_WIN_ID);
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

        MMI_GetLabelTextByLang(tips,&tips_string);

        //set string style
        str_style.font       = SONG_FONT_34 ;//MMI_DEFAULT_NORMAL_FONT;
        str_style.font_color = MMI_BLACK_COLOR;
        str_style.align      = ALIGN_HVMIDDLE;

        //get string width
        str_width = GUISTR_GetStringWidth(&str_style,
            &tips_string,
            str_state);
        
        //get tips rect
        tips_rect.left =6;// MAX(0,(lcd_width - (str_width + str_margin * 2) ) / 2);
        tips_rect.right = lcd_width  - tips_rect.left;
        tips_rect.bottom = lcd_height/2+image_height - 110;
        tips_rect.top  = tips_rect.bottom - image_height+1;
        
      
    
        //display bg image
        GUIRES_DisplayImg(PNULL,&tips_rect,
            PNULL,
            MMIKL_CLOCK_DISP_WIN_ID,
            IMAGE_KL_PUBWIN_BG,
            &layer_handle_ptr);


        //display text
        tips_rect.right = tips_rect.right - str_margin;
        tips_rect.left = tips_rect.left + str_margin;
        tips_rect.bottom = tips_rect.bottom ;
        tips_rect.top  = tips_rect.top ;
        GUISTR_DrawTextToLCDInRect(&layer_handle_ptr,
            &tips_rect,
            &tips_rect,
            &tips_string,
            &str_style,
            str_state,
            GUISTR_TEXT_DIR_AUTO);
#endif
    }
}

extern BOOLEAN KL_RED_KEY;
BOOLEAN s_is_redkey_closelight = TRUE;
/******************************************************************************/
//  Description : handle kl display window msg
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_HandleKLDispWinMsg(
                        MMI_WIN_ID_T    win_id, 
                        uint16          msg_id, 
                        DPARAM          param
                        )
{
    MMI_RESULT_E            recode  =   MMI_RESULT_TRUE;
    //static BOOLEAN          s_is_timer_on       = FALSE;
	//LOCAL BOOLEAN s_is_redkey_closelight = TRUE;  //按power键是否为了closelight

    //SCI_TRACE_LOW:"HandleKLDispWinMsg: 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKL_KEYLOCK_1070_112_2_18_2_31_5_8,(uint8*)"d",msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //effect0 支持gensor,需要设置回来  
          s_is_redkey_closelight = TRUE;
        if(MMIKL_IsPreviewState())
        {
            MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_CUR);
        }
        else
        {
            MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_ALL);
        }

        MMK_SetWinAngle(win_id, MMK_GetScreenAngle(), FALSE);     
       
        SetKeyLockControlParam();
        break;
    case MSG_FULL_PAINT:
		DrawSoftkey(win_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_NOTIFY_PENLONGOK:
        HandleKeyLockPenLongOk(win_id, param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_LCD_SWITCH://reset to begin
        SwitchControlRectInfo();
        //stop timer
        MMIKL_StopUnlockConfirmTimer();
        MMIKL_StopTipsShowTimer();
        MMIKL_SetWinStatus(KL_LOCKED);
        HandleKLLouseFocus();
        break;
     case MSG_KEYDOWN_RED:

        if (MMIKL_IsPreviewState())
        {
            recode = MMI_RESULT_FALSE;
        }   
        break;
	case MSG_BACKLIGHT_TURN_ON:	
		//s_is_redkey_closelight = FALSE;  //闭光界面下按power键会点亮屏幕

		break;

	case MSG_KEYUP_RED:
		#if 0
		if (s_is_redkey_closelight && MMIDEFAULT_GetBackLightState() && MMIAPISET_GetRedKeyLock())
		{
			if (KL_LOCKED == MMIKL_GetWinStatus() || KL_UNLOCKED_ONCE == MMIKL_GetWinStatus())
			{
				//close all light and enter deep sleep
#ifdef LOCK_SCREEN_SPECIAL_EFFECT_SUPPORT        
				MMISEPC_CloseAllLight();
#else
				MMIDEFAULT_CloseAllLight();
#endif
			}
		}
		else
		#endif
		if(MMIDEFAULT_GetBackLightState() && s_is_redkey_closelight)   //MMIAPISET_GetRedKeyLock()
		{
       			MMIDEFAULT_TurnOnBackLight();
		        if (KL_LOCKED == MMIKL_GetWinStatus() || KL_UNLOCKED_ONCE == MMIKL_GetWinStatus())
		        {
                             //   MMIAPIKL_PlayKeyLockRing();
                                MMIKL_SetWinStatus(KL_UNLOCKED);
                    //检查s_st_mmi_kl_event，判断是否有未读SMS、MMS、MISSED CALL发生
                    //如果有，需要调用这3个模块的函数，来弹出各自的提示窗口。
#ifdef MMIWIDGET_WEATHER_ANIM
                    MMK_PostMsg(MAIN_IDLE_WIN_ID, MSG_WEATHER_NEED_ANIM, PNULL, 0);
#endif            
                    MMIKL_CheckPromptStatus();
        			/*if(MMIAPINPS_IsTimerArrive() 
        				&& (!MMIAPINPS_IsAlreadyPrompt())
        				//&& (MAIN_IDLE_WIN_ID == MMK_GetPrevWinId(MMIKL_CLOCK_DISP_WIN_ID))  //前一个窗口应该是任何界面
        				&&(MMIPHONE_IsSimOk(MN_DUAL_SYS_1)
					#ifndef MMI_MULTI_SIM_SYS_SINGLE
        				|| MMIPHONE_IsSimOk(MN_DUAL_SYS_2)
					#endif
        				)
        				&& (MMICONNECTION_SETTING_GPRS_SWITCH_ON == MMIAPICONNECTION_GetGPRSSwitchStatus()))
        			{
        				MMIAPINPS_OpenMainMenuSelectWindow(TRUE);  //[MS]
        			}*/
        			  	MMIAPIKL_PlayKeyLockRing();
        				MMIKL_UnlockedTipsShowTimer(win_id);
        				DisplayUnLockedTips(TXT_UNLOCKED_UNLOCK_TIP);
		        }
			//MMK_CloseWin(win_id); 
                 }
		 s_is_redkey_closelight = TRUE;

		 break;
    //case MSG_KEYDOWN_WEB:
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_RIGHT:
  ///  case MSG_KEYDOWN_STAR:
  case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_HASH:
        if (KL_UNLOCKED_ONCE == s_e_mmi_kl_win_status)
        {
            //stop timer
            MMIKL_StopUnlockConfirmTimer();
            MMIKL_SetWinStatus(KL_LOCKED);
            HandleKLLouseFocus();
        }
        
        break;

    //键盘锁下允许拨打紧急拨号
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
  //  case MSG_KEYDOWN_0:
       // if (!MMIAPICC_OpenEmergencyWin(msg_id))
        {
            if (KL_UNLOCKED_ONCE == s_e_mmi_kl_win_status)
            {
                //stop timer
                MMIKL_StopUnlockConfirmTimer();
                MMIKL_SetWinStatus(KL_LOCKED);
                HandleKLLouseFocus();
            }
        }
        break;
        
    case MSG_KEYLONG_RED://锁键盘界面不能关机
        break;

    case MSG_LOSE_FOCUS:
#ifndef EFFECT_RIPPLE_SUPPORT //144786 
        MAIN_SetStbTimeVisible(FALSE);
#endif

        //stop timer
        if (MMIKL_StopUnlockConfirmTimer())
        {
            MMIKL_SetWinStatus(KL_LOCKED);
        }

        HandleKLLouseFocus();
        break;

    case MSG_GET_FOCUS:
#ifndef EFFECT_RIPPLE_SUPPORT //144786 
#ifdef PDA_UI_DROPDOWN_WIN
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_INVALID, TRUE); 
#endif
        MAIN_SetStbTimeVisible(FALSE);//edit for crNEWMS00192287
#endif

    //for CR NEWMS00147908
    //#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
    //MMIAPIIDLE_SetInitWallpaper_KL(FALSE, MMIKL_CLOCK_DISP_WIN_ID, MMIKL_MAIN_ANIM_BG_ID );
    //#endif
#ifdef MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT
        MMIKL_CreatWallpaperCtrl(MMISET_KEY_LOCK_EFFECT0);
#endif

        if(s_e_mmi_kl_win_status == KL_UNLOCKED)
        {
            //s_is_press_down = FALSE;
            MMK_CloseWin(win_id);
        }
        break;
#if 0 
       case MSG_KEYLONG_OK:
		 if (MMIDEFAULT_IsBacklightOn())
		{
			MMIKL_SetWinStatus(KL_UNLOCKED);
			//检查s_st_mmi_kl_event，判断是否有未读SMS、MMS、MISSED CALL发生
			//如果有，需要调用这3个模块的函数，来弹出各自的提示窗口。
			MMIAPIKL_PlayKeyLockRing();
			MMIKL_UnlockedTipsShowTimer(win_id);
			DisplayUnLockedTips(TXT_UNLOCKED_UNLOCK_TIP);
		}

		
		break; 
#endif
	#if 0
    case MSG_APP_OK:
    case MSG_APP_MENU:
        if ((MMIDEFAULT_IsBacklightOn()) && 
            (s_e_mmi_kl_win_status == KL_LOCKED))
        {
            MMIKL_SetWinStatus(KL_UNLOCKED_ONCE);
            //s_is_press_down = FALSE;
            //start key lock timer
            MMIKL_StartUnlockConfirmTimer(win_id);
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)TXT_COMMON_OK, TRUE);
            MMIDEFAULT_TurnOnBackLight();
			s_is_redkey_closelight = TRUE;
        }       
        else if (KL_UNLOCKED_ONCE == s_e_mmi_kl_win_status)
        {
            //stop timer
            MMIKL_StopUnlockConfirmTimer();
            MMIKL_SetWinStatus(KL_LOCKED);
            HandleKLLouseFocus();

        }
        else if (MMIKL_IsPreviewState())
        {
#ifdef MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT
            MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_EFFECT0);
#endif
            MMK_CloseWin(win_id);
        }
        else
        {
             MMIDEFAULT_TurnOnBackLight();
        }
        break;

  
	case MSG_APP_STAR: 
	 case MSG_APP_WEB:	
   //    case MSG_APP_CANCEL:
        if ((MMIDEFAULT_IsBacklightOn()) && 
            (KL_UNLOCKED_ONCE == s_e_mmi_kl_win_status))
        {
            MMIKL_SetWinStatus(KL_UNLOCKED);
            //检查s_st_mmi_kl_event，判断是否有未读SMS、MMS、MISSED CALL发生
            //如果有，需要调用这3个模块的函数，来弹出各自的提示窗口。
#ifdef MMIWIDGET_WEATHER_ANIM
            MMK_PostMsg(MAIN_IDLE_WIN_ID, MSG_WEATHER_NEED_ANIM, PNULL, 0);
#endif            
            MMIKL_CheckPromptStatus();
#ifdef NPS_SUPPORT
			if(MMIAPINPS_IsTimerArrive() 
				&& (!MMIAPINPS_IsAlreadyPrompt())
//				&& (MAIN_IDLE_WIN_ID == MMK_GetPrevWinId(MMIKL_CLOCK_DISP_WIN_ID))  //前一个窗口应该是任何界面
				&&(MMIPHONE_IsSimOk(MN_DUAL_SYS_1)
#ifndef MMI_MULTI_SIM_SYS_SINGLE
				|| MMIPHONE_IsSimOk(MN_DUAL_SYS_2)
#endif
				)
				&& (MMICONNECTION_SETTING_GPRS_SWITCH_ON == MMIAPICONNECTION_GetGPRSSwitchStatus()))
			{
				MMIAPINPS_OpenMainMenuSelectWindow(TRUE);  //[MS]
			}
#endif
			  MMIAPIKL_PlayKeyLockRing();
			MMK_CloseWin(win_id); 
        }
        if (MMIKL_IsPreviewState())
        {
            MMK_CloseWin(win_id);
        }
        break;
            #endif  
    case MSG_TIMER:
        if (s_unlock_timer_id == *(uint8*)param)
        {
            //stop timer
            MMIKL_StopUnlockConfirmTimer();
            {
                if (KL_UNLOCKED_ONCE == s_e_mmi_kl_win_status)
                {
                    MMI_TP_STATUS_E     state = MMI_TP_NONE;
                    uint16              lcd_width = 0;
                    uint16              lcd_height = 0;
                     GUI_POINT_T        point = {0};
                    GUI_RECT_T          tmp_rect = {0};
                    
                    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
                    
                    //取一半的区域 --softkey 
                    tmp_rect.bottom = lcd_height -1;
                    tmp_rect.top = lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
                    tmp_rect.right = lcd_width -1;
                    tmp_rect.left = lcd_width/2;
#ifdef TOUCH_PANEL_SUPPORT                     
                    MMK_GetLogicTPMsg(&state, &point);

                    if ((MMI_TP_NONE != state) && /*lint !e774*/
                        (GUI_PointIsInRect(point,tmp_rect))) /*lint !e774*/ //如果此时正touch 在右软键，不做任何处理，重启timer
                    {
                        MMIKL_StartUnlockConfirmTimer(win_id);
                    }
                    else
#endif                        
                    {
                        MMIKL_SetWinStatus(KL_LOCKED);
                        HandleKLLouseFocus();
                        GUIWIN_UpdateSoftkey(win_id);
                    }
                }
            }
        }
        else if(s_tips_show_timer_id == *(uint8*)param)
        {
            MMIKL_StopTipsShowTimer();
            if(MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
            }
        }
		else if(s_unlocked_show_timer_id == *(uint8*)param)
		{

			MMIKL_StopUnlockedTipsShowTimer();
			MMK_CloseWin(win_id); 

		}
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        if (KL_LOCKED == s_e_mmi_kl_win_status)
        {
            if(0 ==s_tips_show_timer_id )
            {
                DisplayKeyLockTips();//for keylock tips 
                MMIKL_StartTipsShowTimer(win_id);
                break;
            }
        }
        break;

    //for  directly enter missed call,unread event,unread message
    case MSG_TP_PRESS_LONG:
        if(!MMIKL_IsPreviewState() )
        {
            GUI_POINT_T point = {0};
            uint32 count_num = 0;
            
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(MMK_PosIsCtrl(MMIKL_PRESS_ANIM_EVENT_ID,point))//event
            {
                //count_num = MMIAPIALM_GetTotalTimesOfArrivedEvent(); for NEWMS00182095
                //if(count_num > 0)
                {
#ifdef CALENDAR_SUPPORT
                    MMIAPISCH_OpenUnreadEventWin();
#endif
                    MMK_PostMsg(win_id, MSG_KEY_LOCK_CLOSE_WIN,NULL, 0);
                }

            }
            else if(MMK_PosIsCtrl(MMIKL_PRESS_ANIM_CALL_ID,point))//missed call
            {
                count_num = MMIAPICC_GetMissedCallNumber();
                if(count_num > 0)
                {
                    MMIAPICL_OpenCallsListWindow(MMICL_CALL_MISSED);
                    MMIAPICC_ClearMissedCallNum();
                    MMK_PostMsg(win_id, MSG_KEY_LOCK_CLOSE_WIN,NULL, 0);
                }
                else
                {
                    MMIAPIIDLE_OpenDialWin(msg_id, FALSE);

                    MMK_PostMsg(win_id, MSG_KEY_LOCK_CLOSE_WIN,NULL, 0);
                }
            }
            else if(MMK_PosIsCtrl(MMIKL_PRESS_ANIM_MESSAGE_ID,point)) //unread message
            {
                //count_num = MMIAPISMS_GetAllUnreadMsgCount() + MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_MAX);
                //if(count_num > 0)
                {
                    MMIAPISMS_OpenMainMenuWin();
                    MMK_PostMsg(win_id, MSG_KEY_LOCK_CLOSE_WIN,NULL, 0);
                }
            }
            else
            {
                //SCI_TRACE_LOW:"not in any range"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKL_KEYLOCK_1356_112_2_18_2_31_6_9,(uint8*)"");
            }
        }
        break;

    case MSG_TP_PRESS_UP:
       break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

  //  case MSG_NOTIFY_STATUSBAR_PAINT:
  //      s_is_timer_on = MAIN_IsStbDispTime();
  //      MAIN_SetStbTimeVisible(FALSE);
  //      break;

 //   case MSG_NOTIFY_STATUSBAR_PAINT_END:
 //       MAIN_SetStbTimeVisible(s_is_timer_on);
 //       s_is_timer_on = FALSE;
 //       break;

    case MSG_KEY_LOCK_CLOSE_WIN:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:  
        //stop timer
        MMIKL_StopTipsShowTimer();
        MMIKL_StopUnlockConfirmTimer();
		MMIKL_StopUnlockedTipsShowTimer();
        
        if (!MMIKL_IsPreviewState())
        {
            MMIDEFAULT_TurnOnLCDBackLight();       
            MMIDEFAULT_StartAutoKeylockTimer();
        }
        
        MMIKL_SetWinStatus(KL_UNLOCKED);
      
        GUIWIN_SetStbState(win_id,GUISTATUSBAR_STATE_WITHOUT_HEADER,FALSE);
        //MMIAPICOM_StatusAreaWithheaderSet();
        break;
    case MSG_KEYLOCK_UNDEAL_EVENT:
        MMK_WinGrabFocus(win_id);
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
        break;

	case MSG_NOTIFY_UPDATE:
        {
            uint16          lcd_width = 0;
            uint16          lcd_height = 0;
            GUI_RECT_T      tp_rect = *(GUI_RECT_T *)param;
             GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
             if((tp_rect.top >= (lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT -1)) &&
                (tp_rect.bottom <(lcd_height)) )
            {
                GUIANIM_UpdateRect(*(GUI_RECT_T *)param,MMIKL_MAIN_ANIM_BG_ID);
            }
        }
		break;

    default:
        recode = MMI_RESULT_FALSE;
        break;

    }
    
#ifdef EFFECT_RIPPLE_SUPPORT
 if (!MMIKL_IsPreviewState())
 {
    MMITHEME_HandleRippleEffect(win_id, msg_id, param, TRUE);
 }

#endif

    return recode;
}
#endif

/******************************************************************************/
//  Description : check prompt status (missed call, new sms, new mms)
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIKL_CheckPromptStatus(void)
{
    if (s_st_mmi_kl_event.is_missed_call)
    {
        MMIAPICC_ShowMCHint();
    }

    if (s_st_mmi_kl_event.is_new_sms_alerted)
    {
#ifndef MMI_SMS_MINI_SUPPORT
        MMIAPISMS_ShowNewMsgPrompt();
#endif
    }

#if defined(MMI_VCARD_SUPPORT)
    if (s_st_mmi_kl_event.is_new_vcard_alerted)
    {
        MMIAPISMS_ShowNewVcardOrVcalendarPrompt(FALSE,MMIVC_SMS_SEND_VCARD);
    }
#endif

#if defined(MMI_VCALENDAR_SUPPORT)
    if (s_st_mmi_kl_event.is_new_vcalendar_alerted)
    {
        MMIAPISMS_ShowNewVcardOrVcalendarPrompt(FALSE,MMIVC_SMS_SEND_VCALENDAR);
    }
#endif

    if (s_st_mmi_kl_event.is_new_mms_alerted)
    {

    }
}


/******************************************************************************/
//  Description : check whether key lock window is on top
//  Global resource dependence : none
//  Author: 
//  Note:   If ture: when user press left or right soft key, I will response at once
//          If false : I will make no response
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_IsKLWinOnTop(void)
{
#ifdef MMI_DEVICELOCK_SUPPORT
    return MMIAPIDEVICELOCK_IsDeviceLockWinOnTop();
#else
#if 0//def MMI_MSTYLE_SUPPORT
    return MMIAPIIDLE_IsDeviceLockWinOnTop();
#else
    return (MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID)&& !MMIKL_IsPreviewState());
#endif
#endif
}

/*****************************************************************************/
//  Description : MMIKL_StartTipsShowTimer
//  Global resource dependence : 
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIKL_StartTipsShowTimer(
                                   MMI_WIN_ID_T     win_id
                                   )
{   
    MMIKL_StopTipsShowTimer();
    if (0 == s_tips_show_timer_id)
    {
        s_tips_show_timer_id = MMK_CreateWinTimer(win_id,KEY_UNLOCK_CONFIRM_TIME,FALSE);
    }
}

/*****************************************************************************/
//  Description : get tips timer id
//  Global resource dependence : 
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIKL_GetUnlockConfirmTimerID(void)
{
    return s_unlock_timer_id;
}


/*****************************************************************************/
//  Description : get tips timer id
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIKL_GetTipsTimerID(void)
{
    return s_tips_show_timer_id;
}
/*****************************************************************************/
//  Description : stop anim timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_StopTipsShowTimer(void)
{
    BOOLEAN     result  =   FALSE;

    if (0 < s_tips_show_timer_id)
    {
        MMK_StopTimer(s_tips_show_timer_id);
        s_tips_show_timer_id = 0;
        result = TRUE;
    }

    return (result);
}
/******************************************************************************/
//  Description : start unlock confirm timer
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIKL_StartUnlockConfirmTimer(
                                   MMI_WIN_ID_T     win_id
                                   )
{   
    MMIKL_StopUnlockConfirmTimer();
    if (0 == s_unlock_timer_id)
    {
        s_unlock_timer_id = MMK_CreateWinTimer(win_id,KEY_UNLOCK_CONFIRM_TIME,FALSE);
    }
}
/******************************************************************************/
//  Description : get unlock timer id
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIKL_GetUnlockTimerID(void)
{
    return s_unlock_timer_id;
}
/*****************************************************************************/
//  Description : stop anim timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_StopUnlockConfirmTimer(void)
{
    BOOLEAN     result  =   FALSE;

    if (0 < s_unlock_timer_id)
    {
        MMK_StopTimer(s_unlock_timer_id);
        s_unlock_timer_id = 0;

        result = TRUE;
    }

    return (result);
}


PUBLIC void MMIKL_UnlockedTipsShowTimer(
                                   MMI_WIN_ID_T     win_id
                                   )
{   
    MMIKL_StopUnlockedTipsShowTimer();
    if (0 == s_unlocked_show_timer_id)
    {
        s_unlocked_show_timer_id = MMK_CreateWinTimer(win_id,1000,FALSE);
    }
}


PUBLIC BOOLEAN MMIKL_StopUnlockedTipsShowTimer(void)
{
    BOOLEAN     result  =   FALSE;

    if (0 < s_unlocked_show_timer_id)
    {
        MMK_StopTimer(s_unlocked_show_timer_id);
        s_unlocked_show_timer_id = 0;

        result = TRUE;
    }

    return (result);
}
/*****************************************************************************/
//  Description : set is later handle slide key
//  Global resource dependence :
//  Author: fen.xie
//  Note: MS00173072
/*****************************************************************************/
PUBLIC void MMIAPIKL_SetIsLaterHandleSlideKey(BOOLEAN is_later_handle_slide)
{
    s_is_later_handle_slide = is_later_handle_slide;
}
/*****************************************************************************/
//  Description : get is later handle slide key
//  Global resource dependence :
//  Author: fen.xie
//  Note: MS00173072
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_GetIsLaterHandleSlideKey(void)
{
    return s_is_later_handle_slide;
}
/*****************************************************************************/
//  Description : get key lock ring state
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIKL_GetKeyLockRing(void)
{
    MMIENVSET_SETTING_T mode_setting = {0};
    
    MMIAPIENVSET_GetModeValue(MMIAPIENVSET_GetActiveModeId(),&mode_setting);

    return (!(BOOLEAN)mode_setting.is_tp_key_lock_on);
}
/*****************************************************************************/
//  Description : play key lock ring
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKL_PlayKeyLockRing(void)
{
#ifndef WIN32
    if(
        !MMIAPIENG_GetIQModeStatus()// add by kunliu
#ifdef MMI_RECORD_SUPPORT
        &&!MMIAPIRECORD_IsOpened()
#endif         
        #ifdef JAVA_SUPPORT_IA
        && (!MMIAPIJAVA_IfUseAudioDevice())//java is not use audio device
        #endif
        ) 
    {
        if(MMIKL_GetKeyLockRing())
        {
            //MMIAPISET_StopAllRing(TRUE);
           // MMIAPISET_PlayRing(MMIAPISET_GetActiveSim(), FALSE, 0, 1, MMISET_RING_TYPE_KEY_LOCK, PNULL);
        }
    }
#ifndef TTS_SUPPORT_NONE        
	#ifdef PHONE_NUMBER_VOICE
	#ifndef WIN32
	UnLock_To_Speech();
	#endif
	#endif
#endif

#ifdef HERO_ENGINE_TTS_SUPPORT
	{
	    hero_tts_lock_screen_broadcast(FALSE);
	    //MMIDEFAULT_CloseAllLight();
	    //MMIPUB_OpenAlertWarningWin(TXT_HERO_TTS_LOCK_SCREEN_LOCKED);
	}
#endif

#endif
}





/*Edit by script, ignore 3 case. Thu Apr 26 19:00:57 2012*/ //IGNORE9527


/*Edit by script, ignore 1 case. Fri Apr 27 09:37:19 2012*/ //IGNORE9527

