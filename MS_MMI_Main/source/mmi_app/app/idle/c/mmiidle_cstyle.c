/*****************************************************************************
** File Name:      mmiidle_cstyle.c                                          *
** Author:                                                                   *
** Date:           11/07/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2011        jian.ma              Creat
******************************************************************************/

#define _MMIIDLE_CSTYLE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmi_app_idle_trc.h"
#include "mmiidle_cstyle.h"
#include "mmk_type.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmifm_export.h"
#include "mmiset_text.h"
#include "guires.h" 
#include "mmiidle_func.h"
#include "mmicom_statusarea.h"
#include "mmicountedtime_export.h"
#include "mmiidle_func.h"
#include "mmi_id.h"
#include "mmi_appmsg.h"
#include "mmimp3_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#ifdef MMI_ENABLE_DCD
#include "mmidcd_export.h"
#endif
#include "guistatusbar.h"
#if defined MMIWIDGET_SUPPORT || defined MMI_GRID_IDLE_SUPPORT
#include "mmiwidget.h"
#endif
#include "mmi_image.h"

#ifdef MMI_SMART_DUAL_SIM_SUPPORT
#include "smartdualsim_export.h"
#endif

#include "guictrl_api.h"
#include "guilabel.h"
#include "mmicom_banner.h"
#include "mmisrv_cameraroll_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define IDLE_MP3_TIMER_PERIOD       500 //idle下mp3名字显示的定时器周期

#ifdef MAINLCD_SIZE_128X160 //arvin zhang added for idle softkey icon display
#define MMIIDLE_SOFTKEY_ICON_MAX        2//left icon plus right icon
#define MMIIDLE_SOFTKEY_MARGIN_WIDTH    5//margin space
#endif
#ifdef BAR_PHONE_TOUCH_WIDGET
#define MMIIDLE_SOFTKEY_ADD_HEIGHT    5//176*220 TOUCH 的范围太小，增加高度
#endif
#if defined TOUCH_PANEL_SUPPORT
typedef enum
{
    MMI_COMMON_IDLE_ITEM1 =0,  //sim1运营商名称
    MMI_COMMON_IDLE_ITEM2 = MMI_DUAL_SYS_MAX-1,/*lint !e778*/
    MMI_COMMON_IDLE_ITEM3,  //个性词语/到计时
    MMI_COMMON_IDLE_ITEM4,  //今日日程
    MMI_COMMON_IDLE_ITEM5,  //mp3/fm
    MMI_COMMON_IDLE_ITEM6,  //时间
    MMI_COMMON_IDLE_ITEM7,  //日期
    MMI_COMMON_IDLE_ITEM8,  //星期
    MMI_COMMON_IDLE_ITEM9,  //左软键
    MMI_COMMON_IDLE_ITEM10,  //右软键
    MMI_COMMON_IDLE_ITEM11,  //中软键
    MMI_COMMON_IDLE_MAX_ITEM
}MMI_COMMON_IDLE_ITEM_E;

typedef struct  
{
    GUI_RECT_T                  rect; //显示区域
    MMI_IDLE_DISPLAY_INDEX_E    type; //显示类型
}MMIIDLE_COMMON_DISPLAY_T;
#endif

#define FORM_BOTTOM_MAGRIN_HEIGHT 6
#define FORM_CHILD_BUTTON_HEIGHT 32
#define FORM_CHILD_FM_HEIGHT 72
#define FORM_CHILD_FM_FRQ_HEIGHT 30
#define FORM_CHILD_FM_NAVI_WIDTH 84
#define FORM_CHILD_MUSIC_HEIGHT 100//98+2
#define FORM_CHILD_MUSIC_NAME_HEIGHT 24
#define FORM_CHILD_MUSIC_PROGRESS_HEIGHT 3
#define FORM_CHILD_MUSIC_NAVI_HEIGHT 35
#define FORM_CHILD_MUSIC_NAVI_WIDTH 228
#define FORM_CHILD_MUSIC_TIMEINFO_HEIGHT 24
#define FORM_CHILD_MUSIC_TIMEINFO_WIDTH 40
#define FORM_CHILD_MUSIC_BUTTON_HEIGHT 24
#define FORM_CHILD_MUSIC_BUTTON_WIDTH 24
#define FORM_CHILD_MUSIC_BUTTON_TOP_SPACE 11//space between progress and button 11
#define FORM_CHILD_MUSIC_BUTTON_PLAY_LEFT_SPACE 102//40+62
#define FORM_CHILD_MUSIC_BUTTON_NEXT_LEFT_SPACE 132//60+62+24+6

#if defined MMI_COMMON_IDLE_SUPPORT
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#if defined TOUCH_PANEL_SUPPORT
LOCAL MMIIDLE_COMMON_DISPLAY_T  s_common_idle_display_rect[MMI_COMMON_IDLE_MAX_ITEM] = {0};//普通模式下idle显示区域
#endif
#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (KURO_SUPPORT)|| defined (FM_SUPPORT)

LOCAL IDLE_MP3_NAME_INFO    g_idle_cur_mp3_name = {0}; //当前播放的MP3名称

#endif
#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)                                        
LOCAL uint8 g_mp3_name_timer_id = 0; //Idle界面循环显示MP3歌词的定时器
#endif
#ifdef UI_MULTILAYER_SUPPORT
LOCAL GUI_LCD_DEV_INFO  s_common_idle_paint_layer_handle = {0,UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_common_idle_stb_layer_handle = {0,UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_common_idle_notification_layer_handle = {0,UILAYER_NULL_HANDLE};
#endif

#if defined MAINLCD_SIZE_128X160 && defined  MMI_RES_ORIENT_LANDSCAPE
#define MMI_COMMON_IDLE_ITEM_MAX_NUM 2
#elif defined MAINLCD_SIZE_128X160 && defined  MMI_RES_ORIENT_PORTRAIT
#define MMI_COMMON_IDLE_ITEM_MAX_NUM 3
#elif defined MAINLCD_SIZE_176X220 && defined  MMI_RES_ORIENT_LANDSCAPE
#define MMI_COMMON_IDLE_ITEM_MAX_NUM 5
#else
#define MMI_COMMON_IDLE_ITEM_MAX_NUM 6
#endif
LOCAL BOOLEAN s_notification_form_auto_size = FALSE;
LOCAL GUI_RECT_T s_bottom_rect = {0}; //显示区域
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


/*****************************************************************************/
//  Description : create idle items paint layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void CreateCommonIdlePaintLayer(
                              MMI_WIN_ID_T      win_id
                              );

/*****************************************************************************/
//  Description : get idle items paint layer ptr
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO * GetCommonIdlePaintLcdDev(void);

/*****************************************************************************/
//  Description : release idle paint layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdlePaintLayer(void);

/*****************************************************************************/
//  Description : create status bar layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note: 
/*****************************************************************************/
LOCAL void CreateCommonIdleStbLayer(
                              MMI_WIN_ID_T      win_id
                              );
                              
/*****************************************************************************/
//  Description : release idle paint layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdleStbLayer(MMI_WIN_ID_T win_id);


/*****************************************************************************/
//  Description : get idle items paint layer ptr
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO * GetCommonIdleNotiLcdDev(void);

/*****************************************************************************/
//  Description : create notification layer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CreateCommonIdleNotiLayer(
                              MMI_WIN_ID_T      win_id
                              );

/*****************************************************************************/
//  Description : release notification layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdleNotiLayer(
                              MMI_WIN_ID_T      win_id
                              );

/*****************************************************************************/
//  Description : apend list item
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
LOCAL void UpdateIdleWinPushNotificationList(
                              MMI_CTRL_ID_T ctrl_id
                              );
/*****************************************************************************/
//  Description : apend list item
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
LOCAL void UpdateIdleNotificationButton(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void UpdateIdleFmRadioNotification(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
/*****************************************************************************/
LOCAL void UpdateIdleMusicNotification(void);

/*****************************************************************************/
//  Description : display the  content
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void OutIdleWinContent(void);
/*****************************************************************************/
//  Description : cal idle string start point
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL GUI_POINT_T CalStartPoint(MMI_IDLE_DISPLAY_T disp_style, uint16 str_width);
/*****************************************************************************/
//  Description : output time with image
//  Global resource dependence : 
//  Author: xiangjie
//  Note: 
/*****************************************************************************/
LOCAL void  OutputIdleWinTimerWithImage(void);

/*****************************************************************************/
//  Description : output Network name
//  Global resource dependence : 
//  Author: 
//  Note: 通过PhoneApp中提供的接口函数来获得String,然后计算坐标，将其显示出来
/*****************************************************************************/
LOCAL void  OutputIdleWinNetworkName(void);
#ifdef MMI_IDLE_CUSTOM_WORD_SUPPORT
/*****************************************************************************/
//  Description : output idle win words
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinWords(void);
#endif
/*****************************************************************************/
//  Description : output date
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  OutputIdleWinDate(void);
#ifdef MAINLCD_SIZE_128X160
LOCAL void  OutputIdleWinDateAndWeek(void);
#endif
LOCAL void  OutputIdleWinWeekAndDate(void);

/*****************************************************************************/
//  Description : output counted time
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinCountedTime(BOOLEAN is_highlight);

#ifdef CALENDAR_SUPPORT
/*****************************************************************************/
//  Description : output calendar
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinCalendar(void);
#endif

/*****************************************************************************/
//  Description : output week
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  OutputIdleWinWeek(void);

#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)
/*****************************************************************************/
//  Description : display the mp3 name or not
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
LOCAL void DisplayIdleMp3Name(void);

/*****************************************************************************/
//  Description : display the cycle mp3 name
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
LOCAL void DisplayIdleCycleMp3Name(void);
#endif
/*****************************************************************************/
//  Description : display the softkey of the idle window
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void DisplayIdleWinSoftkey(void);

#if defined TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : get tp type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_IDLE_DISPLAY_INDEX_E GetTpType(
                                        GUI_POINT_T   tp_point
                                        );
#endif
LOCAL void ResetDisplayNum(void);
LOCAL uint16 GetDisplayNum(void);
LOCAL void SetDisplayNum(void);
#if defined TOUCH_PANEL_SUPPORT
LOCAL void ResetDisplayTouchPanelArea(void);
#endif
LOCAL uint8 s_display_num = 0;

/*****************************************************************************/
//  Description : cal idle string start point
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL GUI_POINT_T CalStartPoint(MMI_IDLE_DISPLAY_T disp_style, uint16 str_width)
{
    GUI_POINT_T     point   =   {0};
    GUI_RECT_T      mainlcd_rect = MMITHEME_GetFullScreenRect();


    switch(disp_style.text_align)
    {
    case IDLE_ALIGN_LEFT:
        point.x = disp_style.text_frame_x;//mainlcd_rect.left +10;//disp_style.text_frame_x;
        point.y = disp_style.text_frame_top;
        break;

    case IDLE_ALIGN_RIGHT:
        point.x = disp_style.text_frame_x - str_width;
        point.y = disp_style.text_frame_top;
        break;

    case IDLE_ALIGN_X_CENTER:
        point.x = (mainlcd_rect.right - str_width) / 2;
        point.y = disp_style.text_frame_top;
        break;

    case IDLE_ALIGN_CUSTOM:
        point.x = disp_style.text_frame_x;
        point.y = disp_style.text_frame_top;
        break;

    default:
        break;
    }

    return point;
}


/*****************************************************************************/
//  Description : output time with image
//  Global resource dependence : 
//  Author: xiangjie
//  Note: 
/*****************************************************************************/
LOCAL void  OutputIdleWinTimerWithImage(void)
{
    MMI_IDLE_DISPLAY_T          idle_disp_style     = {0};
    GUI_POINT_T                 point               = {0};
    SCI_TIME_T                  time                = {0};
    GUI_LCD_DEV_INFO            lcd_dev_info        = *GetCommonIdlePaintLcdDev();
    MMISET_TIME_DISPLAY_TYPE_E  time_display_type   = MMIAPISET_GetTimeDisplayType();  
    MMISET_LANGUAGE_TYPE_E language_type;
    MMI_IMAGE_ID_T              am_pm_image         = IMAGE_IDLE_TIME_AM;
    uint16                      mmi_main_time_y     = MMI_MAIN_TIME_Y;
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        mmi_main_time_y = MMI_MAIN_TIME_Y_H;
    }
    TM_GetSysTime(&time);
    MMIAPISET_GetLanguageType(&language_type);

    //12小时制
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        if ((MMISET_TIME_PM_HOUR > time.hour))//AM
        {
            if ((MMISET_TIME_1_HOUR > time.hour))//00:00-1:00
            {
                time.hour = time.hour + MMISET_TIME_PM_HOUR;
            }
        }
        else//PM
        {
            if ((MMISET_TIME_PM_HOUR <= time.hour) && (MMISET_TIME_13_HOUR > time.hour))//12:00-13:00
            {
            }
            else
            {
                time.hour = time.hour-MMISET_TIME_PM_HOUR;
            }
            am_pm_image = IMAGE_IDLE_TIME_PM;
        }   
    }
    
    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_TIME);
#if defined TOUCH_PANEL_SUPPORT    
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].type = MMI_MAIN_LCD_TIME;    
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.left = MMI_MAIN_TIME_0_X;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.top  = mmi_main_time_y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.right = MMI_MAIN_TIME_AM_PM_X;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.bottom = mmi_main_time_y + MMI_MAIN_TIME_H;
#endif
    IMG_EnableTransparentColor(TRUE);

    if(time_display_type == MMISET_TIME_12HOURS)//12小时制
    {
        point.x = MMI_MAIN_TIME_0_X -15;
        point.y = mmi_main_time_y;
        GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID,IMAGE_IDLE_TIME_0 + time.hour/10,&lcd_dev_info);
        point.x = MMI_MAIN_TIME_1_X -15;
        point.y = mmi_main_time_y;
        GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID,IMAGE_IDLE_TIME_0 + time.hour%10,&lcd_dev_info);
        point.x = MMI_MAIN_TIME_COLON_X -15;
        point.y = mmi_main_time_y;
        GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID,IMAGE_IDLE_TIME_COLON,&lcd_dev_info);
        point.x = MMI_MAIN_TIME_2_X -15;
        point.y = mmi_main_time_y;
        GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID,IMAGE_IDLE_TIME_0 + time.min/10,&lcd_dev_info);
        point.x = MMI_MAIN_TIME_3_X -15;
        point.y = mmi_main_time_y;
        GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID,IMAGE_IDLE_TIME_0 + time.min%10,&lcd_dev_info);
    } 
    else
    {
        point.x = MMI_MAIN_TIME_0_X;
        point.y = mmi_main_time_y;
        GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID,IMAGE_IDLE_TIME_0 + time.hour/10,&lcd_dev_info);
        point.x = MMI_MAIN_TIME_1_X;
        point.y = mmi_main_time_y;
        GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID,IMAGE_IDLE_TIME_0 + time.hour%10,&lcd_dev_info);
        point.x = MMI_MAIN_TIME_COLON_X;
        point.y = mmi_main_time_y;
        GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID,IMAGE_IDLE_TIME_COLON,&lcd_dev_info);
        point.x = MMI_MAIN_TIME_2_X;
        point.y = mmi_main_time_y;
        GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID,IMAGE_IDLE_TIME_0 + time.min/10,&lcd_dev_info);
        point.x = MMI_MAIN_TIME_3_X;
        point.y = mmi_main_time_y;
        GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID,IMAGE_IDLE_TIME_0 + time.min%10,&lcd_dev_info);
    }
    IMG_EnableTransparentColor(FALSE);

    if(time_display_type == MMISET_TIME_12HOURS)//12小时制
    {
        point.x = MMI_MAIN_TIME_AM_PM_X;
        point.y = mmi_main_time_y;
    //  idle_disp_style.text_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_TIME);//MMI_WHITE_COLOR;//@zhaohui add，cr102440
#if defined TOUCH_PANEL_SUPPORT
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.right += MMI_DEFAULT_FONT_ASCII_WIDTH*2;
#endif
        GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID,am_pm_image,&lcd_dev_info);   
    }                    
}
/*****************************************************************************/
//  Description : output Network name
//  Global resource dependence : 
//  Author: 
//  Note: 通过PhoneApp中提供的接口函数来获得String,然后计算坐标，将其显示出来
/*****************************************************************************/
LOCAL void  OutputIdleWinNetworkName(void)
{
    MMI_IDLE_DISPLAY_T      idle_disp_style = {0};
    MMI_STRING_T            network_str     = {0};
    GUI_POINT_T             point           = {0};
    GUI_FONT_T              font            = SONG_FONT_8;
    GUI_COLOR_T             color           = 0;
    uint16                  str_width       = 0;
    GUI_LCD_DEV_INFO        lcd_dev_info    = *GetCommonIdlePaintLcdDev();
    BOOLEAN                 is_fly_mode_on  = FALSE;
    // MMI_STRING_T            fly_mode_text   = {0};
    int                     i               = 0;
    // MMISET_DUALSYS_TYPE_E e_dualsys_set = MMIAPISET_GetMultiSysSetting();
#ifdef MONOCHROME_LCD_SUPPORT /* lint_lai */
#else    
    GUI_POINT_T             dis_point       = {0};
#endif 
    uint16                  sim_icon_width  = 0;
    uint16                  sim_icon_height = 0;
    GUISTR_STYLE_T text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    GUI_RECT_T              opn_rect = {0};
    uint16                  lcd_width = 0;
    uint16                  lcd_height = 0;
    uint8                   margin = 2;
    MMISET_IDLE_DISPLAY_T   *idle_mode  =   PNULL;
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    wchar opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN] = {0};
    wchar *sim_deactivated_str = PNULL;

    const MMI_IMAGE_ID_T sim_img_id[] ={
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        IMAGE_IDLE_PROMPT_SIM
#else
        IMAGE_IDLE_PROMPT_SIM1,
        IMAGE_IDLE_PROMPT_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_SIM4
#endif
#endif
    };

    const MMI_IMAGE_ID_T no_sim_img_id[] ={
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        IMAGE_IDLE_PROMPT_NO_SIM
#else
        IMAGE_IDLE_PROMPT_NO_SIM1,
        IMAGE_IDLE_PROMPT_NO_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_NO_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_NO_SIM4
#endif
#endif
    };

    const MMI_IMAGE_ID_T gray_sim_img_id[] ={
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        IMAGE_IDLE_PROMPT_GRAY_SIM
#else
        IMAGE_IDLE_PROMPT_GRAY_SIM1,
        IMAGE_IDLE_PROMPT_GRAY_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_GRAY_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_GRAY_SIM4
#endif
#endif
    };
    idle_mode = MMIAPISET_GetIdleDisplayMode();
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    
    color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_NETWORKNAME);
    
    // 读取设置模式的值
    is_fly_mode_on = MMIAPISET_GetFlyMode();
    
    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if(((0==i)&&(0==idle_mode->visible_flag.item_flag.is_show_network_name))||(GetDisplayNum() >= MMI_COMMON_IDLE_ITEM_MAX_NUM)) continue;
#if defined MMI_MULTI_SIM_SYS_TRI ||  defined MMI_MULTI_SIM_SYS_QUAD ||  defined MMI_MULTI_SIM_SYS_DUAL 
        if(((1==i)&&(0==idle_mode->visible_flag.item_flag.is_show_network_name2))||(GetDisplayNum() >= MMI_COMMON_IDLE_ITEM_MAX_NUM)) continue;       
#endif
#if defined MMI_MULTI_SIM_SYS_TRI ||  defined MMI_MULTI_SIM_SYS_QUAD 
        if(((2==i)&&(0==idle_mode->visible_flag.item_flag.is_show_network_name3))||(GetDisplayNum() >= MMI_COMMON_IDLE_ITEM_MAX_NUM)) continue;
#endif
#if defined MMI_MULTI_SIM_SYS_QUAD
        if(((3==i)&&(0==idle_mode->visible_flag.item_flag.is_show_network_name4))||(GetDisplayNum() >= MMI_COMMON_IDLE_ITEM_MAX_NUM)) continue;
#endif        
        //idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + (i - MN_DUAL_SYS_1));
		idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());
        font = idle_disp_style.text_font;

        MMIIDLE_GetIdleTextStyle(&text_style, font, color);

        if (is_fly_mode_on)
        {
            MMI_GetLabelTextByLang(TXT_SET_FLY_MODE, &network_str);
        }
        else if (MMIAPIPHONE_GetSimExistedStatus(i))
        {
            if (MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting((MN_DUAL_SYS_E)i))
            {
                SCI_MEMSET(&network_str, 0x00, sizeof(network_str));
                network_str.wstr_ptr = (wchar *)opn_spn_name;
                SCI_MEMSET(network_str.wstr_ptr, 0x00, sizeof(opn_spn_name));
            
                MMIAPIPHONE_GetSimAndServiceString(i, &network_str);
            }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            else
            {
                MMISET_SIM_NAME_T sim_name = {0};
                MMI_STRING_T sim_deactivated = {0};

                sim_name = MMIAPISET_GetSimName(i);

                MMI_GetLabelTextByLang(TXT_SIM_DEACTIVATED, &sim_deactivated);

                //此处不判断是否需要加入空格，统一都多分配一个空格的存储空间(2bytes)    
                sim_deactivated_str = (wchar *)SCI_ALLOC_APP(((sim_deactivated.wstr_len + sim_name.wstr_len + 2)*sizeof(wchar)));
                SCI_MEMSET(sim_deactivated_str, 0x00, ((sim_deactivated.wstr_len + sim_name.wstr_len + 2)*sizeof(wchar)));

                MMI_WSTRNCPY(sim_deactivated_str,
                                            (sim_deactivated.wstr_len + sim_name.wstr_len + 1),
                                            sim_name.wstr_arr,
                                            sim_name.wstr_len,
                                            sim_name.wstr_len);

                // ASCII 码数据，两个字符串中间追加一个空格。
                if (MMIAPICOM_IsASCIIString(sim_name.wstr_arr,sim_name.wstr_len)
                    && MMIAPICOM_IsASCIIString(sim_deactivated.wstr_ptr ,sim_name.wstr_len))
                {
                    MMIAPICOM_Wstrcat(sim_deactivated_str, L" "); 
                    MMI_WSTRNCPY(sim_deactivated_str + sim_name.wstr_len + 1,
                                            (sim_deactivated.wstr_len + 1),
                                            sim_deactivated.wstr_ptr,
                                            sim_deactivated.wstr_len,
                                            sim_deactivated.wstr_len);
                    
                    network_str.wstr_ptr = (wchar *)sim_deactivated_str;
                    network_str.wstr_len = sim_deactivated.wstr_len + sim_name.wstr_len + 1 ;
                }
                else
                {
                    MMI_WSTRNCPY(sim_deactivated_str + sim_name.wstr_len ,
                                            (sim_deactivated.wstr_len + 1),
                                            sim_deactivated.wstr_ptr,
                                            sim_deactivated.wstr_len,
                                            sim_deactivated.wstr_len);
                    network_str.wstr_ptr = (wchar *)sim_deactivated_str;
                    network_str.wstr_len = sim_deactivated.wstr_len + sim_name.wstr_len ;
                }
            }
#endif
        }
        else
        {
		     if(0 == MMIAPIPHONE_GetSimCardOkNum(PNULL, 0))
      		    {
      		    	if(i==1)
      		    	{
      				continue;
      		    	}
      		    }
	  	     else   if(1 == MMIAPIPHONE_GetSimCardOkNum(PNULL, 0))
	  		{
	  		    continue;
	  		}
#ifndef MMI_IDLE_MINI_SUPPORT
            MMI_GetLabelTextByLang(TXT_INSERT_SIM, &network_str);
#else
            MMI_GetLabelTextByLang(TXT_INSERT_SIM_MINI, &network_str);  
#endif                

        }

        str_width = GUISTR_GetStringWidth(&text_style,&network_str,text_state);
        point = CalStartPoint(idle_disp_style, str_width);
        #if defined MAINLCD_SIZE_128X160
        //point.y += MMI_IDLE_COMMON_NETWORKNAME_ADJUST;
        #else
        //point.y += MMI_IDLE_COMMON_NETWORKNAME_ADJUST;
        #endif
		if(i == 0)
		{
			point.y = point.y +2;	
		}
        if (MMIAPIPHONE_GetSimExistedStatus(i))
        {
            if (MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting(i))
            {
                image_id = sim_img_id[i];
            }
            else
            {
                image_id = gray_sim_img_id[i];
            }
        }
        else
        {
            image_id = no_sim_img_id[i];
        }

        GUIRES_GetImgWidthHeight(&sim_icon_width,&sim_icon_height,image_id,MAIN_IDLE_WIN_ID);
		if(point.x < sim_icon_width+MMI_IDLE_SIM_ICON_INTERVAL)
		{
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
			point.x = point.x + sim_icon_width+MMI_IDLE_SIM_ICON_INTERVAL;
#else
			point.x = sim_icon_width+MMI_IDLE_SIM_ICON_INTERVAL;
#endif
		}
        opn_rect.left = point.x;
        opn_rect.top = point.y;
        opn_rect.right = lcd_width -1;
        opn_rect.bottom = opn_rect.top + (GUIFONT_GetHeight(text_style.font) + margin);
        
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&opn_rect,
            (const GUI_RECT_T      *)&opn_rect,
            (const MMI_STRING_T     *)&network_str,
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
            );

        if (PNULL != sim_deactivated_str)
        {
            SCI_FREE(sim_deactivated_str);
            sim_deactivated_str = PNULL;
        }
#if defined TOUCH_PANEL_SUPPORT
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM1+i].type = MMI_MAIN_LCD_PLMN + (i - MN_DUAL_SYS_1);
                    
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM1+i].rect.left=point.x;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM1+i].rect.top=point.y;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM1+i].rect.right=point.x+str_width;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM1+i].rect.bottom=point.y+GUI_GetFontHeight(font,UNICODE_HANZI);
#endif
           
#ifdef MONOCHROME_LCD_SUPPORT            
#else       
#if 0
        if (point.x - sim_icon_width >= MMI_IDLE_SIM_ICON_INTERVAL)
        {
            IMG_EnableTransparentColor(TRUE);
            dis_point.x = point.x - sim_icon_width - MMI_IDLE_SIM_ICON_INTERVAL;
            dis_point.y = point.y + MMI_DEFAULT_FONT_HEIGHT - sim_icon_height -1;
                
            GUIRES_DisplayImg(&dis_point,
                PNULL,
                PNULL,
                MAIN_IDLE_WIN_ID,
                image_id,
                &lcd_dev_info);

            IMG_EnableTransparentColor(FALSE);
#if defined TOUCH_PANEL_SUPPORT
            s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM1+i].rect.left=point.x - sim_icon_width - MMI_IDLE_SIM_ICON_INTERVAL;
#endif
        }
#endif        
       SetDisplayNum();
#endif	            
    }
}
#ifdef MMI_IDLE_CUSTOM_WORD_SUPPORT
/*****************************************************************************/
//  Description : output idle win words
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinWords(void)
{
    MMI_IDLE_DISPLAY_T      idle_disp_style = {0};
    GUI_POINT_T             point           = {0};
    GUI_FONT_T              font            = SONG_FONT_8;
    GUI_COLOR_T             color           = 0;
    uint16                  str_width       = 0;
    MMISET_IDLE_DISPLAY_T   *idle_mode      = PNULL;
    GUI_LCD_DEV_INFO        lcd_dev_info    = *GetCommonIdlePaintLcdDev();
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_EFFECT;
    MMI_STRING_T            text   = {0};
    MMI_STRING_T            idle_words   = {0};
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
    uint16                  icon_width = 0;/*lint !e529*/
#endif
    
    idle_mode = MMIAPISET_GetIdleDisplayMode();

    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());
    font = idle_disp_style.text_font;
    color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_IDLEWORD);    

    MMIIDLE_GetIdleTextStyle(&text_style, font, color);
    
    //个性词语内容为空时显示默认个性词语
    if(idle_mode->wstr_len == 0)
    {
		MMI_GetLabelTextByLang(TXT_SET_CUSTOM_WORD, &idle_words);			

		//默认内容的长度超过idle_mode->idle_words的最大长度
        text.wstr_ptr = idle_words.wstr_ptr;
        text.wstr_len = idle_words.wstr_len;
    }
    else
    {
        text.wstr_len = idle_mode->wstr_len;
        text.wstr_ptr = idle_mode->idle_words;
    }

    str_width = GUISTR_GetStringWidth(&text_style,&text,text_state);
	
    point = CalStartPoint(idle_disp_style, str_width);
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
   //display icon
    GUIRES_DisplayImg(&point,
      PNULL,
      PNULL,
      MMK_GetFirstAppletHandle(),
      IMAGE_IDLE_PROMPT_WORDS,
      GetCommonIdlePaintLcdDev());
    GUIRES_GetImgWidthHeight(&icon_width,PNULL, IMAGE_IDLE_PROMPT_WORDS,MMK_GetFirstAppletHandle());
    point.x = point.x + icon_width + 7;
#endif
#if defined TOUCH_PANEL_SUPPORT
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].type = MMI_MAIN_LCD_CUSTOM_WORD;    
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.left=point.x;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.top=point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.right=point.x+str_width;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.bottom=point.y+GUI_GetFontHeight(font,UNICODE_HANZI);
#endif    
    GUISTR_DrawTextToLCDSingleLine( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&point,
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state
        );
	SetDisplayNum();
}
#endif
LOCAL void ResetDisplayNum(void)
{
	s_display_num = 0;

}
LOCAL uint16 GetDisplayNum(void)
{
    return s_display_num;
}
LOCAL void SetDisplayNum(void)
{
	if (s_display_num<=MMI_COMMON_IDLE_ITEM_MAX_NUM)
	{
		s_display_num++;
	}
}
#if defined TOUCH_PANEL_SUPPORT
LOCAL void ResetDisplayTouchPanelArea(void)
{
	uint8 i;
	for (i=MMI_MAIN_LCD_NONE;i<MMI_COMMON_IDLE_MAX_ITEM;i++)
	{
		s_common_idle_display_rect[i].type = MMI_MAIN_LCD_NONE;    
		s_common_idle_display_rect[i].rect.left = 0;
		s_common_idle_display_rect[i].rect.top = 0;
		s_common_idle_display_rect[i].rect.right = 0;
        s_common_idle_display_rect[i].rect.bottom = 0;	
	}

}
#endif
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_NotificationFillBottomRect(void)
{
	GUI_LCD_DEV_INFO        lcd_dev_info             = *GetCommonIdleNotiLcdDev();
    LCD_FillArgbRect(&lcd_dev_info, s_bottom_rect, MMITHEME_GetTransparenceColor(), (uint8)GUIILIST_MASK_BG_ALPHA);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_NotificationEnterControlPanel(void)
{
    if(MMK_IsNaviUpKeyPress())
    {
        //only for test, need to change to open control panel API
        //MMIENVSET_OpenMainMenuWindow();
    }
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_NotificationAccessingControlPanel(void)
{
    MMI_HANDLE_T idle_win_handle = MMK_ConvertIdToHandle(MAIN_IDLE_WIN_ID);
    MMI_CTRL_ID_T  ctrl_id = MMK_GetActiveCtrlId(idle_win_handle);
    MMICOM_BANNER_INFO_T banner_info = {0};
    GUIFORM_CHILD_DISPLAY_E   child_list_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_button_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_music_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_fm_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_PUSH_MSG_LIST_CTRL_ID,&child_list_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_PUSH_MSG_BUTTON_CTRL_ID,&child_button_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_FM_FORM_CTRL_ID,&child_fm_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_MUSIC_FORM_CTRL_ID,&child_music_display_type);

    banner_info.banner_type = MMI_BANNER_TEXT;
    banner_info.text1_id = STR_CONTROL_PANEL_EXT01;

    if(GUIFORM_CHILD_DISP_HIDE != child_fm_display_type)
    {
        if(MMIIDLE_FM_PALY_ANIM_CTRL_ID == ctrl_id)
        {
            MMIAPICOM_OpenBannerWin(MAIN_IDLE_WIN_ID, banner_info);
        }
    }
    else if(GUIFORM_CHILD_DISP_HIDE != child_music_display_type)
    {
        if(MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID == ctrl_id)
        {
            MMIAPICOM_OpenBannerWin(MAIN_IDLE_WIN_ID, banner_info);
        }
    }
    else if(GUIFORM_CHILD_DISP_HIDE != child_list_display_type)
    {
        if(GUILIST_GetCurItemIndex(MMIIDLE_PUSH_MSG_LIST_CTRL_ID) == GUILIST_GetTopItemIndex(MMIIDLE_PUSH_MSG_LIST_CTRL_ID))
        {
            MMIAPICOM_OpenBannerWin(MAIN_IDLE_WIN_ID, banner_info);
        }
    }
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_NotificationShowClearAllBtn(void)
{
    MMI_HANDLE_T idle_win_handle = MMK_ConvertIdToHandle(MAIN_IDLE_WIN_ID);
    GUIFORM_CHILD_DISPLAY_E   child_list_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_PUSH_MSG_LIST_CTRL_ID,&child_list_display_type);
    if(GUIFORM_CHILD_DISP_HIDE != child_list_display_type)
    {
        //高亮在最后一条时，再向下按键出现btn
        if(GUILIST_GetCurItemIndex(MMIIDLE_PUSH_MSG_LIST_CTRL_ID) == (GUILIST_GetTotalItemNum(MMIIDLE_PUSH_MSG_LIST_CTRL_ID)-1))
        {
            GUIFORM_SetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_PUSH_MSG_BUTTON_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
            MMK_SendMsg(MAIN_IDLE_WIN_ID,MSG_IDLE_UPDATE_NOTIFICATION_LIST,PNULL);
        }
    }
    
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_NotificationUpdateSoftkey(void)
{
    MMI_HANDLE_T idle_win_handle = MMK_ConvertIdToHandle(MAIN_IDLE_WIN_ID);
    MMI_CTRL_ID_T  ctrl_id = MMK_GetActiveCtrlId(idle_win_handle);
    MMI_SOFTKEY_STYLE_T      softkey_style = {0};
    GUIFORM_CHILD_DISPLAY_E   child_list_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_button_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_music_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_fm_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_PUSH_MSG_LIST_CTRL_ID,&child_list_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_PUSH_MSG_BUTTON_CTRL_ID,&child_button_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_FM_FORM_CTRL_ID,&child_fm_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_MUSIC_FORM_CTRL_ID,&child_music_display_type);
    
    
    MMITHEME_GetSoftkeyStyle(&softkey_style);
    GUIWIN_SetSoftkeyStyle(idle_win_handle,
                           softkey_style.font_color,
                           softkey_style.bg_type,
                           softkey_style.bg_color,
                           IMAGE_NULL);
    if(GUIFORM_CHILD_DISP_HIDE == child_list_display_type
        && GUIFORM_CHILD_DISP_HIDE == child_button_display_type
        && GUIFORM_CHILD_DISP_HIDE == child_music_display_type
        && GUIFORM_CHILD_DISP_HIDE == child_fm_display_type
        )
    {
        GUIWIN_SeSoftkeytButtonTextId(idle_win_handle,TXT_NULL, MIDDLE_BUTTON, TRUE);
    }
    else if(MMIIDLE_PUSH_MSG_BUTTON_CTRL_ID == ctrl_id)
    {
        GUIWIN_SeSoftkeytButtonTextId(idle_win_handle,TXT_NULL, MIDDLE_BUTTON, TRUE);
    }
    else if((GUIFORM_CHILD_DISP_HIDE != child_music_display_type && MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID == ctrl_id)
        || (GUIFORM_CHILD_DISP_HIDE != child_fm_display_type && MMIIDLE_FM_PALY_ANIM_CTRL_ID == ctrl_id)
        )
    {
        GUIWIN_SeSoftkeytButtonTextId(idle_win_handle,STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, TRUE);
    }
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_NotificationRectCalculate(void)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint16          client_height = 0;
    uint16          form_height = 0;
    uint16          form_relative_height = 0;
    GUI_BOTH_RECT_T  parent_both_rect  = {0};
    GUIFORM_CHILD_HEIGHT_T      child_form_height = {0,GUIFORM_CHILD_HEIGHT_AUTO};
    GUIFORM_CHILD_HEIGHT_T list_form_height = {0,GUIFORM_CHILD_HEIGHT_FIXED};
    GUIFORM_CHILD_HEIGHT_T button_form_height = {FORM_CHILD_BUTTON_HEIGHT,GUIFORM_CHILD_HEIGHT_FIXED};
    GUIFORM_CHILD_HEIGHT_T fm_form_height = {FORM_CHILD_FM_HEIGHT,GUIFORM_CHILD_HEIGHT_FIXED};
    GUIFORM_CHILD_HEIGHT_T music_form_height = {FORM_CHILD_MUSIC_HEIGHT,GUIFORM_CHILD_HEIGHT_FIXED};
    GUIFORM_CHILD_DISPLAY_E   child_list_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_button_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_music_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_fm_display_type = GUIFORM_CHILD_DISP_MAX;

    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_PUSH_MSG_LIST_CTRL_ID,&child_list_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_PUSH_MSG_BUTTON_CTRL_ID,&child_button_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_FM_FORM_CTRL_ID,&child_fm_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_MUSIC_FORM_CTRL_ID,&child_music_display_type);

    //list height
    if(GUIFORM_CHILD_DISP_HIDE != child_list_display_type)
    {
        THEMELIST_ITEM_STYLE_T *my_style = THEMELIST_GetStyle(GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_MS);
        uint16 list_item_num = GUILIST_GetTotalItemNum(MMIIDLE_PUSH_MSG_LIST_CTRL_ID);
        list_form_height.add_data = list_item_num * my_style->height;
        form_relative_height += list_form_height.add_data;
    }
    //button height
    if(GUIFORM_CHILD_DISP_HIDE != child_button_display_type)
    {
        form_relative_height += button_form_height.add_data;
    }
    //child fm from height
    if(GUIFORM_CHILD_DISP_HIDE != child_fm_display_type)
    {
        form_relative_height += fm_form_height.add_data;
    }
    //child music from height
    if(GUIFORM_CHILD_DISP_HIDE != child_music_display_type)
    {
        form_relative_height += music_form_height.add_data;
    }


    //button rect
    if(GUIFORM_CHILD_DISP_HIDE != child_button_display_type)
    {
        GUIFORM_SetChildHeight(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_PUSH_MSG_BUTTON_CTRL_ID, &button_form_height);
    }
    //child fm from rect
    if(GUIFORM_CHILD_DISP_HIDE != child_fm_display_type)
    {
        uint16 ver_space = 6;
        uint16 hor_space = 0;
        GUIFORM_CHILD_HEIGHT_T fm_navi_lable_form_height = {FORM_CHILD_FM_FRQ_HEIGHT,GUIFORM_CHILD_HEIGHT_FIXED};
        GUIFORM_CHILD_WIDTH_T fm_navi_form_width = {FORM_CHILD_FM_NAVI_WIDTH,GUIFORM_CHILD_WIDTH_FIXED};
    
        GUIFORM_SetChildHeight(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_FM_FORM_CTRL_ID, &fm_form_height);
        GUIFORM_SetSpace(MMIIDLE_FM_FORM_CTRL_ID, &hor_space, &ver_space);
        GUIFORM_SetMargin(MMIIDLE_FM_FORM_CTRL_ID,6);
    
        GUIFORM_SetChildHeight(MMIIDLE_FM_FORM_CTRL_ID, MMIIDLE_FM_FREQ_LABLE_CTRL_ID, &fm_navi_lable_form_height);
        GUIFORM_SetChildWidth(MMIIDLE_FM_FORM_CTRL_ID, MMIIDLE_FM_CHIL_NAVI_FORM_CTRL_ID, &fm_navi_form_width);
    }
    //child music from rect
    if(GUIFORM_CHILD_DISP_HIDE != child_music_display_type)
    {
        GUIFORM_CHILD_HEIGHT_T music_lable_form_height = {FORM_CHILD_MUSIC_NAME_HEIGHT,GUIFORM_CHILD_HEIGHT_FIXED};
        GUIFORM_CHILD_HEIGHT_T music_progress_height = {FORM_CHILD_MUSIC_PROGRESS_HEIGHT,GUIFORM_CHILD_HEIGHT_FIXED};
        GUIFORM_CHILD_HEIGHT_T music_navi_form_height = {FORM_CHILD_MUSIC_NAVI_HEIGHT,GUIFORM_CHILD_HEIGHT_FIXED};
        GUIFORM_CHILD_WIDTH_T music_navi_form_width = {FORM_CHILD_MUSIC_NAVI_WIDTH,GUIFORM_CHILD_WIDTH_FIXED};
    
        GUIFORM_SetChildHeight(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_MUSIC_FORM_CTRL_ID, &music_form_height);
        GUIFORM_SetMargin(MMIIDLE_MUSIC_FORM_CTRL_ID,6);
    
        GUIFORM_SetChildHeight(MMIIDLE_MUSIC_FORM_CTRL_ID, MMIIDLE_MUSIC_NAME_LABLE_CTRL_ID, &music_lable_form_height);
        GUIFORM_SetChildHeight(MMIIDLE_MUSIC_FORM_CTRL_ID, MMIIDLE_MUSIC_ARTIST_LABLE_CTRL_ID, &music_lable_form_height);
        GUIFORM_SetChildHeight(MMIIDLE_MUSIC_FORM_CTRL_ID, MMIIDLE_MUSIC_PROGRESSBAR_CTRL_ID, &music_progress_height);
        GUIFORM_SetChildHeight(MMIIDLE_MUSIC_FORM_CTRL_ID, MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID, &music_navi_form_height);
        GUIFORM_SetChildWidth(MMIIDLE_MUSIC_FORM_CTRL_ID, MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID, &music_navi_form_width);
    }


    if (GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height))
    {
        client_height = (uint16)(lcd_height - MMI_STATUSBAR_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT);
    }

    if(client_height <= form_relative_height)
    {
        s_notification_form_auto_size = TRUE;
    }
    else
    {
        s_notification_form_auto_size = FALSE;
    }
    if(s_notification_form_auto_size)
    {
        uint16 bottom_margin = FORM_BOTTOM_MAGRIN_HEIGHT;
        parent_both_rect = MMITHEME_GetWinClientBothRect(MAIN_IDLE_WIN_ID);
        parent_both_rect.v_rect.bottom -= bottom_margin;
        parent_both_rect.h_rect.bottom -= bottom_margin;
        GUIAPICTRL_SetBothRect(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, &parent_both_rect);

        //list rect
        if(GUIFORM_CHILD_DISP_HIDE != child_list_display_type)
        {
            GUIFORM_SetChildHeight(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_PUSH_MSG_LIST_CTRL_ID, &child_form_height);
        }
    }
    else
    {
        GUI_BOTH_RECT_T  both_rect  = {0};

        //list rect
        if(GUIFORM_CHILD_DISP_HIDE != child_list_display_type)
        {
            GUIFORM_SetChildHeight(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_PUSH_MSG_LIST_CTRL_ID, &list_form_height);

            GUIAPICTRL_GetBothRect(MMIIDLE_PUSH_MSG_LIST_CTRL_ID, &both_rect);
            both_rect.v_rect.top = both_rect.v_rect.bottom - list_form_height.add_data;
            both_rect.h_rect.top = both_rect.h_rect.bottom - list_form_height.add_data;
            GUIAPICTRL_SetBothRect(MMIIDLE_PUSH_MSG_LIST_CTRL_ID, &both_rect);

        }
        //parent form rect
        if(GUIFORM_CHILD_DISP_HIDE != child_music_display_type
            ||GUIFORM_CHILD_DISP_HIDE != child_fm_display_type
            ||GUIFORM_CHILD_DISP_HIDE != child_list_display_type
            ||GUIFORM_CHILD_DISP_HIDE != child_button_display_type)
        {
            GUIAPICTRL_GetBothRect(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, &parent_both_rect);
            parent_both_rect.v_rect.top = parent_both_rect.v_rect.bottom - form_relative_height;
            parent_both_rect.h_rect.top = parent_both_rect.h_rect.bottom - form_relative_height;
            GUIAPICTRL_SetBothRect(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, &parent_both_rect);
        }
    }
    if(0 < form_relative_height)
    {
        s_bottom_rect.top = parent_both_rect.v_rect.bottom;
        s_bottom_rect.bottom = lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
        s_bottom_rect.left = parent_both_rect.v_rect.left;
        s_bottom_rect.right  = parent_both_rect.v_rect.right;
    }
    else
    {
        s_bottom_rect.top = 0;
        s_bottom_rect.bottom = 0;
        s_bottom_rect.left = 0;
        s_bottom_rect.right  = 0;
    }
    GUIFORM_ForceLayout(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_UpdateFmMusicNotification(void)
{
    GUIFORM_SetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_MUSIC_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    GUIFORM_SetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_FM_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    if(MMIAPIMP3_IsPlayerActive()
            || MMIAPIAP_IsDisplayForPlayStateNone(FALSE, FALSE)
            )
    {
        GUIFORM_SetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_MUSIC_FORM_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
        UpdateIdleMusicNotification();
        MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID);
    }
    if(FM_STOP != MMIAPIFM_GetStatus())
    {
        GUIFORM_SetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_FM_FORM_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
        UpdateIdleFmRadioNotification();
        MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_FM_PALY_ANIM_CTRL_ID);
    }
    MMIIDLE_NotificationRectCalculate();
}


/*****************************************************************************/
//  Description :Idle screen更新List item的图标数据
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void UpdateNotificationListItemIconData(GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr)
{
    //add anim into list item

    if(PNULL != need_item_content_ptr && MMIIDLE_PUSH_MSG_LIST_CTRL_ID == need_item_content_ptr->ctrl_id)
    {

        uint32 user_data = 0;
        GUIITEM_CONTENT_T  item_content = {0};      /*lint !e64*/
        IDLE_PUSH_MSG_NODE_T *idle_push_msg_ptr = PNULL;
        
        GUILIST_GetItemData(need_item_content_ptr->ctrl_id, need_item_content_ptr->item_index, &user_data);
        idle_push_msg_ptr = (IDLE_PUSH_MSG_NODE_T *)user_data;


        if(PNULL != idle_push_msg_ptr)
        {
            if(MMI_IDLE_ICON_DATA_ANIM_PATH == idle_push_msg_ptr->idle_push_msg_info.icon_data_type)
            {
                GUIANIM_FILE_INFO_T   anim_path = {0};
            
                anim_path.full_path_wstr_len = MMIAPICOM_Wstrlen(idle_push_msg_ptr->idle_push_msg_info.icon_data.full_path_wstr_ptr);
                anim_path.full_path_wstr_ptr = idle_push_msg_ptr->idle_push_msg_info.icon_data.full_path_wstr_ptr;

                item_content.item_data_type = GUIITEM_DATA_ANIM_PATH;     
                item_content.item_data.anim_path_ptr = &anim_path;
                GUILIST_SetItemContent(need_item_content_ptr->ctrl_id, 
                                                &item_content, 
                                                need_item_content_ptr->item_index, 
                                                need_item_content_ptr->item_content_index); 
            }
        }          
    }
}

#if 0
/*****************************************************************************/
//  Description :处理Idle screen 控件对OK键的响应
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void HandleIdleScreenCtrlMidSK( MMI_WIN_ID_T win_id)
{
            uint16 selected_index = 0;
            GUIFORM_CHILD_DISPLAY_E   child_music_display_type = GUIFORM_CHILD_DISP_MAX;
            GUIFORM_CHILD_DISPLAY_E   child_fm_display_type = GUIFORM_CHILD_DISP_MAX;
            MMI_CTRL_ID_T  ctrl_id = MMK_GetActiveCtrlId(win_id);

            if(MMIIDLE_PUSH_MSG_LIST_CTRL_ID == ctrl_id)
            {
            
                GUILIST_ITEM_T  *item_ptr  = PNULL;
                IDLE_PUSH_MSG_NODE_T *idle_push_msg_ptr = PNULL;
                uint32 user_data = 0;
                
                selected_index = GUILIST_GetCurItemIndex(MMIIDLE_PUSH_MSG_LIST_CTRL_ID);
                item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, selected_index);
                GUILIST_GetItemData(ctrl_id, selected_index, &user_data);
                idle_push_msg_ptr = (IDLE_PUSH_MSG_NODE_T *)user_data;
                if(PNULL != item_ptr && PNULL != idle_push_msg_ptr)
                {
                    if (GUILIST_GetItemState(item_ptr, GUIITEM_STATE_LEFT_FOCUS))
                    {
                        //当前item处于左键focus状态
                        if(PNULL != idle_push_msg_ptr->idle_push_msg_info.app_left_focus_action_func_ptr)
                        {
                            void *para1 = PNULL;
                            void *para2 = PNULL;

                             if(MMI_IDLE_PUSH_MSG_WNS == idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type)
                             {
                                 para1 = (void *)idle_push_msg_ptr->idle_push_msg_info.appname;
                             }
                             else if(MMI_IDLE_PUSH_MSG_SMS == idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type
                                   || MMI_IDLE_PUSH_MSG_MISSED_CALL == idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type
                                   || MMI_IDLE_PUSH_MSG_ONGOING_CALL == idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type
                                         )
                             {
                                 para1 = (void *)idle_push_msg_ptr->idle_push_msg_info.sender_num;
                             }

                             idle_push_msg_ptr->idle_push_msg_info.app_left_focus_action_func_ptr(
                                        para1, 
                                        para2);
                        }
                        else
                        {
                            if(MMI_IDLE_PUSH_MSG_SMS != idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type)                        
                            {         
                                MMIIDLE_RemovePushMsg(idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type,
                                    idle_push_msg_ptr->idle_push_msg_info.appname,
                                    idle_push_msg_ptr->idle_push_msg_info.sender_num);                             
                            }
                            else
                            {
                                 MMIIDLE_RemovePushMsgEx(idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type,
                                     idle_push_msg_ptr->idle_push_msg_info.appname,
                                     &idle_push_msg_ptr->idle_push_msg_info.pb_bcd);   
                            }
                        }
                    }
                    else if (GUILIST_GetItemState(item_ptr, GUIITEM_STATE_RIGHT_FOCUS))
                    {
                        //当前item处理右键focus状态
                        if(PNULL != idle_push_msg_ptr->idle_push_msg_info.app_right_focus_action_func_ptr)
                        {
                            void *para1 = PNULL;
                            void *para2 = PNULL;

                            if(MMI_IDLE_PUSH_MSG_SMS == idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type
                                || MMI_IDLE_PUSH_MSG_MISSED_CALL == idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type
                                || MMI_IDLE_PUSH_MSG_ONGOING_CALL == idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type
                                )
                             {
                                 para1 = (void *)idle_push_msg_ptr->idle_push_msg_info.sender_num;
                                 para2 = (void *)idle_push_msg_ptr->idle_push_msg_info.recv_num;
                             }
                            
                             idle_push_msg_ptr->idle_push_msg_info.app_right_focus_action_func_ptr(
                                        para1, 
                                        para2);
                        }
                    }
                    else
                    {
                        if(PNULL != idle_push_msg_ptr->idle_push_msg_info.app_entry_func_ptr)
                        {
                            void *para1 = PNULL;
                            void *para2 = PNULL;

                             if(MMI_IDLE_PUSH_MSG_WNS == 
                                idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type)
                             {
                                 para1 = (void *)idle_push_msg_ptr->idle_push_msg_info.appname;
                             }
                             
                             idle_push_msg_ptr->idle_push_msg_info.app_entry_func_ptr(
                                                                                                                            para1, 
                                                                                                                            para2);
                             if(MMI_IDLE_PUSH_MSG_SMS != idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type)
                             {
                                 MMIIDLE_RemovePushMsg(idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type,
                                     idle_push_msg_ptr->idle_push_msg_info.appname,
                                     idle_push_msg_ptr->idle_push_msg_info.sender_num);                             
                             }
                             else
                             {
                                 MMIIDLE_RemovePushMsgEx(idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type,
                                     idle_push_msg_ptr->idle_push_msg_info.appname,
                                     idle_push_msg_ptr->idle_push_msg_info.pb_bcd);     
                             }

                        }
                    }
                }
            }
            else if(GUIFORM_CHILD_DISP_HIDE != child_music_display_type && MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID == ctrl_id)
            {
#ifdef MMI_AUDIO_PLAYER_SUPPORT
                //if(MMIAPIMP3_IsPlayerActive())
                {
                    MMIAPIMP3_OpenMp3Player();
                }
#endif
            }
            else if(GUIFORM_CHILD_DISP_HIDE != child_fm_display_type && MMIIDLE_FM_PALY_ANIM_CTRL_ID == ctrl_id)
            {
#ifdef FM_SUPPORT
                //if(FM_STOP != MMIAPIFM_GetStatus())
                {
                    MMIAPIFM_OpenMainWin();
                }
#endif
            }

        }
#endif
/*****************************************************************************/
//  Description : handle common idle message
//  Global resource dependence : 
//  Author: jianma
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIIDLE_CommonHandleMsg ( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
#if defined TOUCH_PANEL_SUPPORT
    GUI_POINT_T             point       = {0};
    static MMI_IDLE_DISPLAY_INDEX_E s_common_down_tp_type  = MMI_MAIN_LCD_NONE;
    MMI_IDLE_DISPLAY_INDEX_E up_tp_type  = MMI_MAIN_LCD_NONE;
    LOCAL BOOLEAN            s_is_process_tp_down   = FALSE;  //是否先处理了 tp down key    
#endif 
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    GUIFORM_CHILD_DISPLAY_E   child_music_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_CHILD_DISPLAY_E   child_fm_display_type = GUIFORM_CHILD_DISP_MAX;
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_MUSIC_FORM_CTRL_ID,&child_music_display_type);
    GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_FM_FORM_CTRL_ID,&child_fm_display_type);
    
    switch (msg_id)
    {
    case MSG_FULL_PAINT:
#if defined TOUCH_PANEL_SUPPORT
        SCI_MEMSET(s_common_idle_display_rect,0,sizeof(s_common_idle_display_rect));
#endif 
        OutIdleWinContent();
        //MMIAPIIDLE_DisplaySoftkey();
        DisplayIdleWinSoftkey();
#ifdef MMI_ENABLE_DCD
        MMIAPIDCD_HandleIdleWinMessage(win_id,msg_id,param);
#endif
#ifdef MMI_SMART_DUAL_SIM_SUPPORT
		MMISmartDS_CheckToDisplayPromptWin(win_id, MSG_SMARTDS_FIRST_FLOW_CHECK);
#endif
        break;
#if 0
    case MSG_APP_OK:
#ifdef MMI_MOCOR_SAMPLE
    MMIAPISAMPLE_CreateHelloSprdWin();
    break;
#endif
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef MMI_ENABLE_DCD
        MMIAPIDCD_HandleIdleWinMessage(win_id,msg_id,param);
#else
        MMIAPIMENU_CreatMainMenu();

#endif
    break;
#if 0

    case MSG_CTL_MIDSK:
        HandleIdleScreenCtrlMidSK(win_id);
        break;
#endif

    
#ifdef SOS_KEYLONGWEB
	case MSG_KEYUP_WEB:
#else
	case MSG_APP_WEB:
#endif		

#ifdef CMCC_UI_STYLE
    //MO一键上网标记在web键
    MMIAPISET_EnterMonternetWin();
#else
#ifdef MMI_ENABLE_DCD
        MMIAPIDCD_HandleIdleWinMessage(win_id,msg_id,param);
#else
        MMIAPIMENU_CreatMainMenu();
#endif     
#endif
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:    //右软键
      ///  MMIAPISET_OpenFuncKeyWin(win_id, MMISET_FUNCKEY_RIGHT);
       MMIAPIPB_OpenListAllGroupWin();
        break;
#ifdef ZDT_PRODUCT_Z685S

 case MSG_KEYREPEAT_UP:
    case MSG_APP_UP:

        MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_UP);

        break;

    case MSG_KEYREPEAT_DOWN:
    case MSG_APP_DOWN:

        MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_DOWN);
        
        break;

    case MSG_KEYREPEAT_LEFT:
    //case MSG_APP_LEFT:
    case MSG_KEYUP_LEFT://bugzilla 4216

        MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_LEFT);

        
        break;

    case MSG_KEYREPEAT_RIGHT:
    //case MSG_APP_RIGHT:
    case MSG_KEYUP_RIGHT://bugzilla 4216

        MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_RIGHT);

        
        break;
#endif

     #if  0
#ifdef MMIWNS_TEST_SUPPORT
    case MSG_APP_RED:
        MMIWNS_TestStart();
        break;
#endif
     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        UpdateNotificationListItemIconData((GUILIST_NEED_ITEM_CONTENT_T*)param);
        break;

        
    case MSG_KEYREPEAT_UP:
    case MSG_APP_UP:
        MMIIDLE_NotificationAccessingControlPanel();
        MMIIDLE_NotificationUpdateSoftkey();
#if 0 //UNISOC_MMI_Delete
        MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_UP);
#endif
        break;

    case MSG_KEYREPEAT_DOWN:
    case MSG_APP_DOWN:
        MMIIDLE_NotificationShowClearAllBtn();
        MMIIDLE_NotificationUpdateSoftkey();
#if 0 //UNISOC_MMI_Delete
        MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_DOWN);
#endif        
        break;

    case MSG_KEYREPEAT_LEFT:
    //case MSG_APP_LEFT:
    case MSG_KEYUP_LEFT://bugzilla 4216
#if 0 //UNISOC_MMI_Delete
        MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_LEFT);
#endif
        {
            MMI_CTRL_ID_T  ctrl_id = MMK_GetActiveCtrlId(win_id);
            if(GUIFORM_CHILD_DISP_HIDE != child_music_display_type && MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID == ctrl_id)
            {
#ifdef MMI_AUDIO_PLAYER_SUPPORT
                if(MMIMP3_AUDIO_PLAYING == MMIAPIMP3_GetAudioState())
                {
                    MMIAPIMP3_PauseMp3(FALSE);  
                }
                else if(MMIMP3_AUDIO_PAUSED == MMIAPIMP3_GetAudioState())
                {
                    MMIAPIMP3_ResumeMp3(FALSE);  
                }
                else if(MMIMP3_PLAYER_STATE_NONE == MMIAPIMP3_GetPlayerState())
                {
                    uint32 offset = MMIMP3_ZERO_OFFSET;
                    
                    offset = MMIAPIAP_GetCurPlayOffset(offset);
                    
                    MMIAPIMP3_PlayAudioPlayer(offset);
                }
                UpdateIdleMusicNotification();
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
#endif
            }
            else if(GUIFORM_CHILD_DISP_HIDE != child_fm_display_type && MMIIDLE_FM_PALY_ANIM_CTRL_ID == ctrl_id)
            {
#ifdef FM_SUPPORT
                if(FM_PLAYING == MMIAPIFM_GetStatus())
                {
                    MMIAPIFM_Pause(TRUE);  
                }
                else if(FM_SUSPENDED == MMIAPIFM_GetStatus())
                {
                    MMIAPIFM_Resume(TRUE);  
                }
                UpdateIdleFmRadioNotification();
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
#endif
            }
        }
        break;

    case MSG_KEYREPEAT_RIGHT:
    //case MSG_APP_RIGHT:
    case MSG_KEYUP_RIGHT://bugzilla 4216
#if 0 //UNISOC_MMI_Delete
        MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_RIGHT);
#endif
        {
            MMI_CTRL_ID_T  ctrl_id = MMK_GetActiveCtrlId(win_id);
            if(GUIFORM_CHILD_DISP_HIDE != child_music_display_type && MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID == ctrl_id)
            {
#ifdef MMI_AUDIO_PLAYER_SUPPORT
                MMIAPIMP3_GoToPreOrNextMp3(FALSE);
#ifdef MMIAP_F_RELEASE_BG_AUDIO
                MMIAPIAP_ReleaseAudioHandle();
#endif
                UpdateIdleMusicNotification();
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
#endif
            }
            else if(GUIFORM_CHILD_DISP_HIDE != child_fm_display_type && MMIIDLE_FM_PALY_ANIM_CTRL_ID == ctrl_id)
            {
#ifdef FM_SUPPORT
                if(MMIFM_GetValidChannelCount() > 1)
                {
                    MMIFM_AdjustChannel((int)1);
                    UpdateIdleFmRadioNotification();
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                }
#endif
            }
        }
        break;
#endif
#if 0
defined TOUCH_PANEL_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        s_is_process_tp_down = TRUE;
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);    
#ifdef MMI_SPRITE_ENABLE
        if (MMIIDLE_PressDownSprite(point.x, point.y))
        {
            break;
        }
#endif
          s_common_down_tp_type = GetTpType(point);
// 	     if(MMI_MAIN_LCD_COUNTEDTIME == s_common_down_tp_type)
//          {
//             //highlight效果
//             OutputIdleWinCountedTime(TRUE);
//          }  
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        if(!s_is_process_tp_down)
            break;//没有受到tp down ,不处理up

        s_is_process_tp_down = FALSE;//reset 
        
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
#ifdef MMI_SPRITE_ENABLE
        if (MMIIDLE_PressUpSprite(point.x, point.y))
        {
            s_common_down_tp_type = MMI_MAIN_LCD_NONE;
            break;
        }
#endif

        up_tp_type = GetTpType(point);
        if (s_common_down_tp_type != up_tp_type)
        {
            s_common_down_tp_type = MMI_MAIN_LCD_NONE;
            break;
        }     
		
        switch(up_tp_type) 
        {
        case MMI_MAIN_LCD_TIME:
            MMIAPISET_SetTime();
            break;
            
        case MMI_MAIN_LCD_WEEK:
        case MMI_MAIN_LCD_DATE:
            MMIAPISET_SetDate();
            //MMIAPISET_OpenTimeOrDateWindow((uint32*)MMISET_SET_DATE_WIN_TAB, FALSE);
            break;

        case MMI_MAIN_LCD_MP3:
            if(FM_STOP != MMIAPIFM_GetStatus())
            {
                MMIAPIFM_OpenMainWin();
            }
#ifdef MMI_AUDIO_PLAYER_SUPPORT
            else if(MMIAPIMP3_IsPlayerActive())
            {
                MMIAPIMP3_OpenMp3Player();
            }
#endif
#ifdef KURO_SUPPORT
            else if(MMIAPIKUR_IsPlayerActive())
            {
                MMIAPIKUR_OpenKurPlayer();
            }
#endif            
            break;

        case MMI_MAIN_LCD_COUNTEDTIME:
			 MMIAPICT_OpenCoutimeRunningWin();// 打开倒记时窗口
            break;

#ifdef CALENDAR_SUPPORT
        case MMI_MAIN_LCD_CALENDAR:
            MMIAPIIDLE_OpenScheduleWin(win_id);
            break;
#endif

        case MMI_MAIN_LCD_CUSTOM_WORD:
            MMIAPISET_OpenSetIdleDisWin();
            break;

        case MMI_MAIN_LCD_PLMN:
#ifdef MMI_MULTI_SIM_SYS_SINGLE
            MMIAPIIDLE_OpenSelNetWorkWin(MN_DUAL_SYS_1);
#else
            MMIAPISET_OpenMultiSimSettingsWindow();
#endif
            break;

        #ifndef MMI_MULTI_SIM_SYS_SINGLE
        case MMI_MAIN_LCD_PLMN2:
            MMIAPISET_OpenMultiSimSettingsWindow();
            break;
            
         #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)   
         case MMI_MAIN_LCD_PLMN3:
            MMIAPISET_OpenMultiSimSettingsWindow();
            break;
         #endif /*defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD) */
         
         #if defined(MMI_MULTI_SIM_SYS_QUAD)
         case MMI_MAIN_LCD_PLMN4:
            MMIAPISET_OpenMultiSimSettingsWindow();
            break;
         #endif /* MMI_MULTI_SIM_SYS_QUAD */
         
         #endif /* MMI_MULTI_SIM_SYS_SINGLE */

        case MMI_MAIN_LCD_LEFT_SOFTKEY:
            MMK_SendMsg(win_id,MSG_APP_OK,PNULL);
            break;

        case MMI_MAIN_LCD_RIGHT_SOFTKEY:
            MMK_SendMsg(win_id,MSG_APP_CANCEL,PNULL);
            break;
        default:
            MMIAPIMENU_CreatMainMenu();
            break;
        } 
        s_common_down_tp_type = MMI_MAIN_LCD_NONE;
        break;
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_OPEN_WINDOW:
    case MSG_GET_FOCUS:
#ifdef UI_MULTILAYER_SUPPORT        
        if (UILAYER_IsLayerActive(MMITHEME_GetDefaultLcdDev()))
        {
            UILAYER_Clear(MMITHEME_GetDefaultLcdDev());
        }
        ReleaseCommonIdlePaintLayer();
        ReleaseCommonIdleStbLayer(win_id);
        ReleaseCommonIdleNotiLayer(win_id);
        CreateCommonIdlePaintLayer(win_id);
        CreateCommonIdleStbLayer(win_id);
#endif   
#ifndef ZDT_PRODUCT_Z685S 
        GUIWIN_SetStbState(win_id,GUISTATUSBAR_STATE_WITHOUT_HEADER,TRUE);
        MMIAPICOM_StatusAreaIdlescreenSet();
        UpdateIdleWinPushNotificationList(MMIIDLE_PUSH_MSG_LIST_CTRL_ID);
        UpdateIdleNotificationButton();
		if(MSG_OPEN_WINDOW == msg_id)
		{
			MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_PUSH_MSG_LIST_CTRL_ID);
		}
        MMIIDLE_UpdateFmMusicNotification();
        MMIIDLE_NotificationUpdateSoftkey();
#ifdef CAMERAROLL_SUPPORT         
        MMISRV_CAMERAROLL_Check_auto_upload_photo();
#endif
#ifdef UI_MULTILAYER_SUPPORT        
        CreateCommonIdleNotiLayer(win_id);
#endif 
#else
	  OutIdleWinContent();
	MMIAPICOM_StatusAreaIdlescreenSet();	
#endif
        break;
    case MSG_CLOSE_WINDOW:
    case MSG_LOSE_FOCUS:
#ifdef UI_MULTILAYER_SUPPORT        
        ReleaseCommonIdlePaintLayer();
        ReleaseCommonIdleStbLayer(win_id);
        ReleaseCommonIdleNotiLayer(win_id);
#endif        
        GUIWIN_SetStbState(win_id,GUISTATUSBAR_STATE_WITHOUT_HEADER,FALSE);
        MMIAPICOM_StatusAreaWithheaderSet();
	 GUIWIN_SetStbItemTextValid(0, FALSE);
        break;
    case MSG_IDLE_UPDATE_COUNTEDTIME:
    case MSG_IDLE_UPDATE_DATETIME:
    case MSG_STK_SET_UP_IDLE_MODE:
#ifdef UI_MULTILAYER_SUPPORT        
        OutIdleWinContent();
        //DisplayIdleWinSoftkey();
#else        
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
#endif
        break;
#if 0
    case MSG_IDLE_UPDATE_MP3: 
    case MSG_IDLE_UPDATE_NOTIFICATION_LIST:
        UpdateIdleWinPushNotificationList(MMIIDLE_PUSH_MSG_LIST_CTRL_ID);
        UpdateIdleNotificationButton();
        MMIIDLE_UpdateFmMusicNotification();
        MMIIDLE_NotificationUpdateSoftkey();
        if(0 < GUILIST_GetTotalItemNum(MMIIDLE_PUSH_MSG_LIST_CTRL_ID)
            && MSG_IDLE_UPDATE_NOTIFICATION_LIST == msg_id)
        {
            MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_PUSH_MSG_LIST_CTRL_ID);
        }
#ifdef UI_MULTILAYER_SUPPORT        
        ReleaseCommonIdleNotiLayer(win_id);
        CreateCommonIdleNotiLayer(win_id);
#endif        
#ifdef UI_MULTILAYER_SUPPORT        
        OutIdleWinContent();
        //DisplayIdleWinSoftkey();
#else        
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
#endif
        break;

    case MSG_COMMON_BANNER_CLOSE:
        MMIIDLE_NotificationEnterControlPanel();
        break;
#endif
// #ifdef MMI_SMART_DUAL_SIM_SUPPORT
//     case MSG_SMARTDS_FIRST_FLOW_OK:
        //if(MSG_SMARTDS_FIRST_FLOW_OK == (MMI_MESSAGE_ID_E)param)
//         {
//     		MMISmartDS_CheckToDisplayPromptWin(win_id, MSG_SMARTDS_FIRST_FLOW_PROMPT);
//         }
//         break;
// #endif
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result ;
}

#ifdef MAINLCD_SIZE_128X160
LOCAL void  OutputIdleWinDateAndWeek(void)
{

    MMI_IDLE_DISPLAY_T          idle_disp_style     = {0};
    GUI_POINT_T                 point               = {0};
    uint8                       datestr[30]         = {0};
    wchar                       datewstr[30]        = {0};
    wchar                       wstr_buf[30]        = {0};
    GUI_FONT_T                  font                = SONG_FONT_8;
    GUI_COLOR_T                 color               = 0;
    uint16                      str_width           = 0;
    MMI_STRING_T                m_string            = {0};
    GUI_LCD_DEV_INFO            lcd_dev_info        = *GetCommonIdlePaintLcdDev();  
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T            text   = {0};
    SCI_DATE_T date = {0};
    MMI_TEXT_ID_T week_table[] = {
                                        TXT_IDLE_SHORT_SUNDAY,
                                        TXT_IDLE_SHORT_MONDAY,
                                        TXT_IDLE_SHORT_TUESDAY,
                                        TXT_IDLE_SHORT_WEDNESDAY,
                                        TXT_IDLE_SHORT_THURSDAY,
                                        TXT_IDLE_SHORT_FRIDAY,
                                        TXT_IDLE_SHORT_SATURDAY
    };
#ifdef MULTI_LAN_SUPPORT_DE
	MMI_TEXT_ID_T short_week_table[] = {
									TXT_SUNDAY,//       TXT_SHORT_IDLE_SUNDAY,
									TXT_MONDAY,//       TXT_SHORT_IDLE_MONDAY,
									TXT_TUESDAY,//      TXT_SHORT_IDLE_TUESDAY,
									TXT_WEDNESDAY,//       TXT_SHORT_IDLE_WEDNESDAY,
									TXT_THURSDAY,//       TXT_SHORT_IDLE_THURSDAY,
									TXT_FRIDAY,//       TXT_SHORT_IDLE_FRIDAY,
									TXT_SATURDAY,//       TXT_SHORT_IDLE_SATURDAY
	};
#endif
	
    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_DATE);
    font = idle_disp_style.text_font;
    color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_DATE);
    TM_GetSysDate(&date);
#ifdef MULTI_LAN_SUPPORT_DE
	if((MMISET_LANGUAGE_SIMP_CHINESE == MMISET_GetLanguageType())||
		MMISET_LANGUAGE_TRAD_CHINESE == MMISET_GetLanguageType())
	{
		MMI_GetLabelTextByLang(short_week_table[date.wday], &m_string);
	}
	else
	{
		MMI_GetLabelTextByLang(week_table[date.wday], &m_string);
	}
#else
    MMI_GetLabelTextByLang(week_table[date.wday], &m_string);      
#endif
    MMIAPISET_GetCurrentDateStr(datestr,'-',sizeof(datestr));
    strncat(datestr," ",2);/*lint !e64*/
    MMIAPICOM_StrToWstr(datestr, wstr_buf);    
    MMIIDLE_GetIdleTextStyle(&text_style, font, color);
    text.wstr_ptr = datewstr;
    MMIAPICOM_Wstrncpy(text.wstr_ptr,wstr_buf,MMIAPICOM_Wstrlen(wstr_buf));
    MMIAPICOM_Wstrncat(text.wstr_ptr, m_string.wstr_ptr,m_string.wstr_len);     
    text.wstr_len = (uint16)MMIAPICOM_Wstrlen(text.wstr_ptr);
    str_width = GUISTR_GetStringWidth(&text_style,&text,text_state);
   
    point = CalStartPoint(idle_disp_style, str_width);
    GUISTR_DrawTextToLCDSingleLine( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&point,
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state
        );

}

#endif


/*****************************************************************************/
//  Description :在Idle screen上显示时间
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void OutputIdleWinTime(void)
{
    GUI_POINT_T                point               = {0};
    GUI_FONT_T                  font                = SONG_FONT_36;
    GUI_COLOR_T               color               = MMI_WHITE_COLOR;
    GUISTR_STYLE_T         time_text_style = {0};
    uint8 time_buf[MMIIDLE_TIME_LEN]  = {0};
    uint16 time_str_width = 0;
    uint8 time_hour = 0;
    uint8 time_min = 0;
    MMI_STRING_T time_string = {0};
    MMI_STRING_T time_type_str = {0};
    wchar                   time_wstr_buf[MMIIDLE_TIME_LEN]        = {0};
    SCI_TIME_T         time = {0};
    MMI_IDLE_DISPLAY_T          idle_disp_style     = {0};
    GUI_LCD_DEV_INFO            lcd_dev_info        = *GetCommonIdlePaintLcdDev();  
    MMISET_TIME_DISPLAY_TYPE_E  time_type = MMISET_TIME_12HOURS;
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;

    //显示时间类型:
    time_type = MMIAPISET_GetTimeDisplayType();

    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_TIME);
    font = idle_disp_style.text_font;
    color = MMI_WHITE_COLOR;//MMITHEME_GetCurThemeFontColor(MMI_MAIN_LCD_TIME);
    MMIIDLE_GetIdleTextStyle(&time_text_style, font, color);
     
    //get system time
    TM_GetSysTime(&time);
    //MMIAPISET_FormatTimeStrByTime(time.hour, time.min, (uint8*)time_buf, MMIIDLE_TIME_LEN);

    time_min = time.min;
    time_hour = time.hour;
 
    if(time_type == MMISET_TIME_12HOURS)
    {
        if ((MMISET_TIME_PM_HOUR > time.hour))//AM
        {
            if ((MMISET_TIME_1_HOUR > time.hour))//00:00-1:00
            {
                time_hour = time.hour + MMISET_TIME_PM_HOUR;
            }
        }
        else//PM
        {
            if ((MMISET_TIME_PM_HOUR <= time.hour) && (MMISET_TIME_13_HOUR > time.hour))//12:00-13:00
            {
            }
            else
            {
                time_hour = time.hour -MMISET_TIME_PM_HOUR;
            }
        }
    }

    sprintf((char *)time_buf, "%02d:%02d", time_hour, time_min);

    MMIAPICOM_StrToWstr(time_buf, time_wstr_buf);

    time_string.wstr_ptr = time_wstr_buf;
    time_string.wstr_len = MMIAPICOM_Wstrlen(time_string.wstr_ptr);

    time_str_width = GUISTR_GetStringWidth(&time_text_style, &time_string, text_state);
   
    point = CalStartPoint(idle_disp_style, time_str_width);
    GUISTR_DrawTextToLCDSingleLine( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&point,
        (const MMI_STRING_T     *)&time_string,
        &time_text_style,
        text_state
        );


    //12小时制
    if(time_type == MMISET_TIME_12HOURS)
    {
        if ((MMISET_TIME_PM_HOUR > time.hour))//AM
        {
            MMI_GetLabelTextByLang(TXT_AM, &time_type_str);
        }
        else//PM
        {
            MMI_GetLabelTextByLang(TXT_PM, &time_type_str);
        }   
        time_text_style.font = SONG_FONT_18;
        time_text_style.align = ALIGN_LEFT;
        point.x += time_str_width + 6;
        point.y += 14;

        GUISTR_DrawTextToLCDSingleLine( 
                    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                    (const GUI_POINT_T      *)&point,
                    (const MMI_STRING_T     *)&time_type_str,
                    &time_text_style,
                    text_state
                    );

    }
}

LOCAL void  OutputIdleWinWeekAndDate(void)
{

    MMI_IDLE_DISPLAY_T          idle_disp_style     = {0};
    GUI_POINT_T                 point               = {0};
    uint8                       datestr[30]         = {0};
    wchar                       date_wstr[30]        = {0};
    wchar                       wstr_buf[30]        = {0};
    wchar                       wstr_split[]        = {L", "};
    GUI_FONT_T                  font                = SONG_FONT_12;
    GUI_COLOR_T                 color               = 0;
    uint16                      str_width           = 0;
    MMI_STRING_T                week_string            = {0};
    MMI_STRING_T            display_string   = {0};
    GUI_LCD_DEV_INFO            lcd_dev_info        = *GetCommonIdlePaintLcdDev();  
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
    SCI_DATE_T date = {0};
    MMI_TEXT_ID_T week_table[] = {
                                            TXT_SUNDAY,//       TXT_SHORT_IDLE_SUNDAY,
                                            TXT_MONDAY,//       TXT_SHORT_IDLE_MONDAY,
                                            TXT_TUESDAY,//      TXT_SHORT_IDLE_TUESDAY,
                                            TXT_WEDNESDAY,//       TXT_SHORT_IDLE_WEDNESDAY,
                                            TXT_THURSDAY,//       TXT_SHORT_IDLE_THURSDAY,
                                            TXT_FRIDAY,//       TXT_SHORT_IDLE_FRIDAY,
                                            TXT_SATURDAY,//       TXT_SHORT_IDLE_SATURDAY
                                            };

    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_DATE);
    font = idle_disp_style.text_font;
    color = MMI_WHITE_COLOR;//MMITHEME_GetCurThemeFontColor(MMI_MAIN_LCD_DATE);
    
    MMIIDLE_GetIdleTextStyle(&text_style, font, color);
    
    TM_GetSysDate(&date);
    MMI_GetLabelTextByLang(week_table[date.wday], &week_string);      
    MMIAPISET_GetCurrentDateStr(datestr,'.',sizeof(datestr));
    MMIAPICOM_StrToWstr(datestr, date_wstr); 
    
    display_string.wstr_ptr = wstr_buf;

    MMIAPICOM_Wstrncpy(display_string.wstr_ptr, week_string.wstr_ptr,week_string.wstr_len);   
    MMIAPICOM_Wstrncat(display_string.wstr_ptr, wstr_split,MMIAPICOM_Wstrlen(wstr_split));     
    MMIAPICOM_Wstrncat(display_string.wstr_ptr, date_wstr,MMIAPICOM_Wstrlen(date_wstr));   

    display_string.wstr_len = MMIAPICOM_Wstrlen(display_string.wstr_ptr);

    str_width = GUISTR_GetStringWidth(&text_style,&display_string,text_state);
   
    point = CalStartPoint(idle_disp_style, str_width);
    GUISTR_DrawTextToLCDSingleLine( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&point,
        (const MMI_STRING_T     *)&display_string,
        &text_style,
        text_state
        );

}
/*****************************************************************************/
//  Description : Display the progress according to dynamic playing info..
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MusicDisplayProgress(void)
{
	GUI_LCD_DEV_INFO        lcd_dev_info             = *GetCommonIdleNotiLcdDev();
    GUI_RECT_T          bg_rect = {0};
    GUI_RECT_T          progress_rect = {0};
    MMI_CTRL_ID_T       active_ctrl_id = MMK_GetActiveCtrlId(MAIN_IDLE_WIN_ID);
    MMI_CTRL_ID_T       ctrl_id = MMIIDLE_MUSIC_PROGRESSBAR_CTRL_ID;
    GUI_COLOR_T         progress_color = MMI_WHITE_COLOR;
    GUI_COLOR_T         bg_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_1);

    if(MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID == active_ctrl_id)
    {
        bg_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_1);
        progress_color = MMI_WHITE_COLOR;
    }
    else
    {
        bg_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_3);
        progress_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT);
    }
    //draw bg
    GUIOWNDRAW_GetDisplayRect(ctrl_id, &bg_rect);
    GUI_FillRect((const GUI_LCD_DEV_INFO*) &lcd_dev_info, bg_rect, bg_color);

    if(MMIAPIMP3_IsPlayerActive()
            || MMIAPIAP_IsDisplayForPlayStateNone(FALSE, FALSE)
      )
    {
        MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};
        MMIAPIMP3_GetPlayingDynInfo(&dyn_info,FALSE);

        progress_rect = bg_rect;
        if(dyn_info.total_length != 0)
        {
            progress_rect.right = progress_rect.left 
                + (uint32)((progress_rect.right-progress_rect.left)*((uint64_t)dyn_info.cur_offset)/dyn_info.total_length);/*lint !e737*/
        }
        else
        {
            progress_rect.right = progress_rect.left;     
        }
        GUI_FillRect((const GUI_LCD_DEV_INFO*) &lcd_dev_info, progress_rect, progress_color);
    }
}

/*****************************************************************************/
//  Description : Display the timeinfo according to dynamic playing info..
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MusicDisplayTimeInfo(void)
{
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T    cur_str_t   = {0};
    MMI_STRING_T    total_str_t = {0};
    wchar        cur_time_wstr[32]   = {0}; 
    wchar        total_time_wstr[32] = {0}; 
    char         cur_time_str[32] = {0};
    char         total_time_str[32] = {0};
	GUI_LCD_DEV_INFO        lcd_dev_info    = *GetCommonIdleNotiLcdDev();
    GUI_RECT_T parent_display_rect = {0};
    GUI_RECT_T curtime_display_rect = {0};
    GUI_RECT_T totaltime_display_rect = {0};
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};

    GUIAPICTRL_GetDisplayRect(MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID, &parent_display_rect);

    text_style.font = MMI_DEFAULT_NORMAL_FONT;
    text_style.font_color = MMI_WHITE_COLOR;
    
    if (MMIAPIMP3_IsPlayerActive()
        || MMIAPIAP_IsDisplayForPlayStateNone(FALSE, TRUE)
       )
    {        
        MMIAPIMP3_GetPlayingDynInfo(&dyn_info,FALSE);
        curtime_display_rect.left = parent_display_rect.left;
        curtime_display_rect.right = parent_display_rect.left + FORM_CHILD_MUSIC_TIMEINFO_WIDTH;
        totaltime_display_rect.left = parent_display_rect.right - FORM_CHILD_MUSIC_TIMEINFO_WIDTH;
        totaltime_display_rect.right = totaltime_display_rect.left + FORM_CHILD_MUSIC_TIMEINFO_WIDTH;
        if(dyn_info.total_time / 60 < 10)//0:00
        {
            cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.1ld:%.2ld", dyn_info.cur_time / 60, dyn_info.cur_time % 60);
            total_str_t.wstr_len = sprintf((char*)total_time_str, "%.1ld:%.2ld", dyn_info.total_time / 60, dyn_info.total_time % 60);
        }
        else if(dyn_info.total_time/60 < 60)//00:00
        {
            cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld", dyn_info.cur_time/60, dyn_info.cur_time%60);
            total_str_t.wstr_len = sprintf((char*)total_time_str, "%.2ld:%.2ld", dyn_info.total_time/60, dyn_info.total_time%60);
        }
        else if(dyn_info.total_time / 60 < 60*10)//0:00:00
        {
            cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.1ld:%.2ld:%.2ld", (dyn_info.cur_time / 60) / 60,
                                         (dyn_info.cur_time / 60) % 60,
                                         (dyn_info.cur_time % 60));
            total_str_t.wstr_len = sprintf((char*)total_time_str, "%.1ld:%.2ld:%.2ld", (dyn_info.total_time / 60) / 60,
                                           (dyn_info.total_time / 60) % 60,
                                           dyn_info.total_time % 60);
            curtime_display_rect.right = parent_display_rect.left + FORM_CHILD_MUSIC_TIMEINFO_WIDTH + 12;
            totaltime_display_rect.left = parent_display_rect.right - FORM_CHILD_MUSIC_TIMEINFO_WIDTH -12;
            totaltime_display_rect.right = totaltime_display_rect.left + FORM_CHILD_MUSIC_TIMEINFO_WIDTH+12;
        }
        else//00:00:00
        {
            cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld:%.2ld", (dyn_info.cur_time/60)/60, 
                (dyn_info.cur_time/60)%60, 
                (dyn_info.cur_time%60));
            total_str_t.wstr_len = sprintf((char*)total_time_str, "%.2ld:%.2ld:%.2ld", (dyn_info.total_time/60)/60,
                (dyn_info.total_time/60)%60, 
                dyn_info.total_time%60);
            curtime_display_rect.right = parent_display_rect.left + FORM_CHILD_MUSIC_TIMEINFO_WIDTH + 20;
            totaltime_display_rect.left = parent_display_rect.right - FORM_CHILD_MUSIC_TIMEINFO_WIDTH -20;
            totaltime_display_rect.right = totaltime_display_rect.left + FORM_CHILD_MUSIC_TIMEINFO_WIDTH+20;
        }

        cur_str_t.wstr_ptr = cur_time_wstr;
        total_str_t.wstr_ptr = total_time_wstr;
        MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)cur_time_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
        MMI_STRNTOWSTR(total_str_t.wstr_ptr, total_str_t.wstr_len, (uint8*)total_time_str, total_str_t.wstr_len, total_str_t.wstr_len);

        //cur time info
        curtime_display_rect.top = parent_display_rect.top;
        curtime_display_rect.bottom = curtime_display_rect.top + FORM_CHILD_MUSIC_TIMEINFO_HEIGHT;
        //curtime_display_rect.left = parent_display_rect.left;
        //curtime_display_rect.right = curtime_display_rect.left + FORM_CHILD_MUSIC_TIMEINFO_WIDTH;
        text_style.align = ALIGN_LVMIDDLE;
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &curtime_display_rect,
            &curtime_display_rect,
            &cur_str_t,    
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
            );

        //total time info
        totaltime_display_rect.top = parent_display_rect.top;
        totaltime_display_rect.bottom = totaltime_display_rect.top + FORM_CHILD_MUSIC_TIMEINFO_HEIGHT;
        //totaltime_display_rect.left = parent_display_rect.right - FORM_CHILD_MUSIC_TIMEINFO_WIDTH;
        //totaltime_display_rect.right = totaltime_display_rect.left + FORM_CHILD_MUSIC_TIMEINFO_WIDTH;
        text_style.align = ALIGN_RVMIDDLE;
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &totaltime_display_rect,
            &totaltime_display_rect,
            &total_str_t,      
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
            );        
    } 
}

/*****************************************************************************/
//  Description : Display Button
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MusicDisplayButton(void)
{
	GUI_LCD_DEV_INFO        lcd_dev_info    = *GetCommonIdleNotiLcdDev();
    GUI_RECT_T              parent_display_rect = {0};
    GUI_RECT_T              playbtn_display_rect = {0};
    GUI_RECT_T              nextbtn_display_rect = {0};
    MMI_IMAGE_ID_T         img_id = IMAGE_NULL;
    GUIAPICTRL_GetDisplayRect(MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID, &parent_display_rect);

    if(MMIAPIMP3_IsPlayerPlaying())
    {
        img_id = IMAGE_IDLE_SCREEN_PLAYER_PAUSE;
    }
    else //if(MMIAPIMP3_IsPlayerPaused())
    {
        img_id = IMAGE_IDLE_SCREEN_PLAYER_PLAY;
    }
    playbtn_display_rect.top = parent_display_rect.top + FORM_CHILD_MUSIC_BUTTON_TOP_SPACE;
    playbtn_display_rect.bottom = playbtn_display_rect.top + FORM_CHILD_MUSIC_BUTTON_HEIGHT;
    playbtn_display_rect.left = parent_display_rect.left + FORM_CHILD_MUSIC_BUTTON_PLAY_LEFT_SPACE;
    playbtn_display_rect.right = playbtn_display_rect.left + FORM_CHILD_MUSIC_BUTTON_WIDTH;
    
    GUIRES_DisplayImg(PNULL,
        &playbtn_display_rect,
        PNULL,
        MAIN_IDLE_WIN_ID,
        img_id,
        &lcd_dev_info);

    //set next anim
    nextbtn_display_rect.top = parent_display_rect.top + FORM_CHILD_MUSIC_BUTTON_TOP_SPACE;
    nextbtn_display_rect.bottom = nextbtn_display_rect.top + FORM_CHILD_MUSIC_BUTTON_HEIGHT;
    nextbtn_display_rect.left = parent_display_rect.left + FORM_CHILD_MUSIC_BUTTON_NEXT_LEFT_SPACE;
    nextbtn_display_rect.right = nextbtn_display_rect.left + FORM_CHILD_MUSIC_BUTTON_WIDTH;
    
    GUIRES_DisplayImg(PNULL,
        &nextbtn_display_rect,
        PNULL,
        MAIN_IDLE_WIN_ID,
        IMAGE_IDLE_SCREEN_PLAYER_NEXT,
        &lcd_dev_info);
}

/*****************************************************************************/
//  Description : DrawProgressOwnerDrawCtrl
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UpdateIdleMusicNotiProgressOwnerDrawCtrl(
    GUIOWNDRAW_INFO_T *owner_draw_ptr
)
{
    MusicDisplayProgress();
}

/*****************************************************************************/
//  Description : DrawtimeinfoOwnerDrawCtrl
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UpdateIdleMusicNotitimeinfoOwnerDrawCtrl(
    GUIOWNDRAW_INFO_T *owner_draw_ptr
)
{
    MusicDisplayTimeInfo();
}

/*****************************************************************************/
//  Description : Draw play next button OwnerDrawCtrl
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UpdateIdleMusicNotiPlayNextbtnOwnerDrawCtrl(
    GUIOWNDRAW_INFO_T *owner_draw_ptr
)
{
    MusicDisplayButton();
}

/*****************************************************************************/
//  Description : display the mp3 name or not
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
LOCAL void UpdateIdleFmRadioNotification(void)
{
    MMIFM_CUR_INFO_T *cur_inof_ptr = PNULL;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    MMI_STRING_T        freq_string = {0};
    wchar               wstr_old[]        = {L"%S"};
    wchar               fm_name[MMIFM_CHANNEL_NAME_LEN + MMIFM_FREQ_MAX_LEN + 1] = {0};
    char                  freq_num_str[MMIFM_FREQ_MAX_LEN + 1] = {0};
    wchar               freq_num_wstr[MMIFM_FREQ_MAX_LEN + 1] = {0};
    MMI_STRING_T            label_str = {0};
    GUI_FONT_T      font_type = MMI_DEFAULT_NORMAL_FONT;    /*!<字体风格*/
    GUI_COLOR_T     font_color = MMI_WHITE_COLOR;   /*!<字体颜色*/

    GUIFORM_PermitChildFont(MMIIDLE_FM_FORM_CTRL_ID, FALSE);
    GUILABEL_SetFont(MMIIDLE_FM_FREQ_LABLE_CTRL_ID, font_type, font_color);

#ifdef FM_SUPPORT
    cur_inof_ptr = MMIFM_GetFMCurrentInfo();
#endif
    if (PNULL != cur_inof_ptr)
    {
        MMIAPIFM_ConvertFreqToString(cur_inof_ptr->cur_freq, (char*) freq_num_str);
    }

    MMIAPICOM_StrToWstr(freq_num_str, freq_num_wstr);

    MMI_GetLabelTextByLang(STR_RADIO_FM_FREQ_INDICATOR, &freq_string);//"%S MHz"

    MMIAPICOM_Wstrncpy(fm_name, freq_string.wstr_ptr, freq_string.wstr_len);
    MMIAPICOM_WstrReplace(fm_name, sizeof(fm_name), wstr_old, freq_num_wstr);
    
    label_str.wstr_ptr = (wchar *)fm_name;
    label_str.wstr_len = (uint16)MMIAPICOM_Wstrlen(fm_name);
    GUILABEL_SetText(MMIIDLE_FM_FREQ_LABLE_CTRL_ID, &label_str, FALSE);

    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_ptr = PNULL;

    //set play/pause anim
    control_info.ctrl_id = MMIIDLE_FM_PALY_ANIM_CTRL_ID;
    if(FM_PLAYING == MMIAPIFM_GetStatus())
    {
        data_info.img_id = IMAGE_IDLE_SCREEN_PLAYER_PAUSE;
    }
    else if(FM_SUSPENDED == MMIAPIFM_GetStatus())
    {
        data_info.img_id = IMAGE_IDLE_SCREEN_PLAYER_PLAY;
    }
    display_info.align_style = GUIANIM_ALIGN_HMIDDLE_BOTTOM;
    GUIANIM_SetParam(&control_info, &data_info, PNULL, &display_info);

    //set next anim
    control_info.ctrl_id = MMIIDLE_FM_NEXT_ANIM_CTRL_ID;
    data_info.img_id = IMAGE_IDLE_SCREEN_PLAYER_NEXT;
    display_info.align_style = GUIANIM_ALIGN_RIGHT_BOTTOM;
    GUIANIM_SetParam(&control_info, &data_info, PNULL, &display_info);

    //GUIAPICTRL_SetState(MMIIDLE_FM_FORM_CTRL_ID, GUICTRL_STATE_INVISIBLE, FALSE);
}

/*****************************************************************************/
//  Description : display the mp3 name or not
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
LOCAL void UpdateIdleMusicNotification(void)
{
    wchar               music_name[MMIMP3_NAME_MAX_LEN + 1]={0};
    MMI_STRING_T            name_str = {0};
    MMI_STRING_T            artist_str = {0};
    MMIMP3_ALBUM_INFO_T album_info  = {0};
    GUI_FONT_T      font_type = MMI_DEFAULT_NORMAL_FONT;    /*!<字体风格*/
    GUI_COLOR_T     font_color = MMI_WHITE_COLOR;   /*!<字体颜色*/

    GUIFORM_PermitChildFont(MMIIDLE_MUSIC_FORM_CTRL_ID, FALSE);
    
    //song name
    GUILABEL_SetFont(MMIIDLE_MUSIC_NAME_LABLE_CTRL_ID,font_type,font_color);
    name_str.wstr_ptr = music_name;
    MMIAPIMP3_GetCurMp3Name(&name_str,MMIMP3_NAME_MAX_LEN);
    GUILABEL_SetText(MMIIDLE_MUSIC_NAME_LABLE_CTRL_ID,&name_str,FALSE);

    //artist
    GUILABEL_SetFont(MMIIDLE_MUSIC_ARTIST_LABLE_CTRL_ID,font_type,font_color);
    MMIAPIMP3_GetCurMp3AlbumInfo(&album_info);
    artist_str.wstr_ptr = album_info.artist;
    artist_str.wstr_len = (uint16)MMIAPICOM_Wstrlen(artist_str.wstr_ptr);
    GUILABEL_SetText(MMIIDLE_MUSIC_ARTIST_LABLE_CTRL_ID,&artist_str,FALSE);

    //GUIAPICTRL_SetState(MMIIDLE_MUSIC_FORM_CTRL_ID, GUICTRL_STATE_INVISIBLE, FALSE);
}

/*****************************************************************************/
//  Description : apend list item
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
LOCAL void UpdateIdleWinPushNotificationList(
                              MMI_CTRL_ID_T ctrl_id
                              )
{
    int i = 0;
    int msg_count = 0;
    uint16 cur_index = GUILIST_GetCurItemIndex(ctrl_id);
    GUILIST_RemoveAllItems(ctrl_id);

    for(i = 0; i < MMI_IDLE_PUSH_MSG_MAX_COUNT; i ++)
    {
        IDLE_PUSH_MSG_NODE_T *idle_push_msg_node_ptr = PNULL;

        idle_push_msg_node_ptr = MMIIDLE_GetPushMsgNode(i);
        if(PNULL != idle_push_msg_node_ptr 
            && MMI_IDLE_PUSH_MSG_STATE_VALID == idle_push_msg_node_ptr->msg_state)
        {
            int content_index = 0;
            GUILIST_ITEM_T          item_t     = {0};
            GUILIST_ITEM_DATA_T  item_data = {0};
            GUIANIM_FILE_INFO_T   anim_path = {0};
            
            item_t.item_style       = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_MS;
            item_t.left_item_style  = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS;
            item_t.right_item_style = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_WITH_RIGHT_FOCUS_MS;
            item_t.item_data_ptr = &item_data;
            item_t.user_data = idle_push_msg_node_ptr;
            
            if(MMI_IDLE_ICON_DATA_ANIM_PATH == idle_push_msg_node_ptr->idle_push_msg_info.icon_data_type)
            {
                anim_path.full_path_wstr_len = MMIAPICOM_Wstrlen(idle_push_msg_node_ptr->idle_push_msg_info.icon_data.full_path_wstr_ptr);
                anim_path.full_path_wstr_ptr = idle_push_msg_node_ptr->idle_push_msg_info.icon_data.full_path_wstr_ptr;
                item_data.item_content[content_index].item_data_type     = GUIITEM_DATA_ANIM_PATH;
                item_data.item_content[content_index].item_data.anim_path_ptr = &anim_path;
            }
            else if(MMI_IDLE_ICON_DATA_IMAGE_ID == idle_push_msg_node_ptr->idle_push_msg_info.icon_data_type)
            {
                item_data.item_content[content_index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[content_index].item_data.image_id = 
                                        idle_push_msg_node_ptr->idle_push_msg_info.icon_data.image_id;
            }
            else
            {
                item_data.item_content[content_index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[content_index].item_data.image_id = 
                    MMIIDLE_GetPushMsgIcon(idle_push_msg_node_ptr->idle_push_msg_info.e_push_msg_type);
            }
            
            content_index++;
            
            item_data.item_content[content_index].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[content_index].item_data.text_buffer.wstr_ptr = 
                    idle_push_msg_node_ptr->idle_push_msg_info.title;
            item_data.item_content[content_index].item_data.text_buffer.wstr_len = 
                    MMIAPICOM_Wstrlen(idle_push_msg_node_ptr->idle_push_msg_info.title);
            content_index++;
            
            item_data.item_content[content_index].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[content_index].item_data.text_buffer.wstr_ptr = 
                    idle_push_msg_node_ptr->idle_push_msg_info.contents;
            item_data.item_content[content_index].item_data.text_buffer.wstr_len = 
                    MMIAPICOM_Wstrlen(idle_push_msg_node_ptr->idle_push_msg_info.contents);

            MMIIDLE_SetPushMsgSoftkey(idle_push_msg_node_ptr->idle_push_msg_info.e_push_msg_type, &item_data);

            GUILIST_AppendItem(ctrl_id, &item_t);
            SetDisplayNum();
            msg_count++;
        }
        else
        {
            break;
        }
    }

    if(0 == msg_count)
    {
        GUIFORM_SetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, ctrl_id, GUIFORM_CHILD_DISP_HIDE);
    }
    else
    {
        
        THEMELIST_ITEM_STYLE_T *my_style = THEMELIST_GetStyle(GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_MS);
        if(PNULL != my_style)
        {
            GUILIST_SetCurItemIndex(ctrl_id, cur_index);

            GUILIST_SetListState(ctrl_id, GUILIST_STATE_CYC_SCROLL_ITEM, FALSE);
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);

            //设置focus时需要显示左右三角图标
            //GUILIST_SetShowLRTriangleIcon(ctrl_id, TRUE, TRUE);
            //设置需要响应左右键处理
            //GUILIST_SetHandleLeftRightEx(ctrl_id, TRUE, TRUE);
            MMIIDLE_SetPushListItemLRAction(ctrl_id, msg_count);

            
            //设置左focus时显示的左Quick图标、设置右focus时显示的右Quick图标
            //GUILIST_SetQuickActionIcon(ctrl_id,  left_focus_image,  right_focus_image);

            //设置List背景半透明
            GUILIST_SetBgTransparent(ctrl_id, TRUE, (uint8)GUIILIST_MASK_BG_ALPHA);

            GUIFORM_SetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, ctrl_id, GUIFORM_CHILD_DISP_NORMAL);
            //MMK_SetAtvCtrl(MAIN_IDLE_WIN_ID, ctrl_id);
        }
        else
        {
            SCI_TRACE_LOW("UpdateIdleWinPushNotificationList form theme list style not exist!!!");
        }
        //append button item
#if 0
        if(msg_count>0)
        {
            GUILIST_ITEM_T          item_t     = {0};
            GUILIST_ITEM_DATA_T  item_data = {0};
            THEMELIST_ITEM_STYLE_T btnitemstyle = {0};
            THEMELIST_ITEM_STYLE_T* pItemStyle = THEMELIST_GetStyle(GUIITEM_LAYOUT_ONE_LINE_BUTTON_TEXT);
            SCI_MEMCPY(&btnitemstyle, pItemStyle, sizeof(*pItemStyle));
            
            item_t.item_style       = GUIITEM_LAYOUT_ONE_LINE_BUTTON_TEXT;
            item_t.item_data_ptr = &item_data; 
            
            item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[1].item_data.text_id = STR_NOTIF_CLEAR_ALL_EXT01;
            
            item_data.softkey_id[MIDDLE_BUTTON] = TXT_NULL;
            
            GUILIST_AppendItem(ctrl_id, &item_t);
            //btnitemstyle.height = FORM_CHILD_BUTTON_HEIGHT;
            //btnitemstyle.height_focus = FORM_CHILD_BUTTON_HEIGHT;
            //GUILIST_SetItemStyleEx(ctrl_id,msg_count,&btnitemstyle);
            GUILIST_SetItemShowLRTriangleIconByIndex(ctrl_id,msg_count,FALSE,FALSE);
            GUILIST_SetItemHandleLeftRightByIndex(ctrl_id,msg_count,FALSE,FALSE);
    
        }
#endif

    }


}

/*****************************************************************************/
//  Description : Handle clear all button click event on idle 
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleClearAllBtnClick(void)
{
    MMIIDLE_RemoveAllPushMsg();
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : apend list item
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
LOCAL void UpdateIdleNotificationButton(void)
{
    GUI_BG_T bg = {0};
    GUI_FONT_ALL_T font ={0};

    GUIBUTTON_PermitBorder(MMIIDLE_PUSH_MSG_BUTTON_CTRL_ID,FALSE);
    
    //CTRLBUTTON_SetIsRectButton(MMIIDLE_PUSH_MSG_BUTTON_CTRL_ID, FALSE);
    GUIBUTTON_SetTextId(MMIIDLE_PUSH_MSG_BUTTON_CTRL_ID, STR_NOTIF_CLEAR_ALL_EXT01);
    GUIBUTTON_SetCallBackFunc(MMIIDLE_PUSH_MSG_BUTTON_CTRL_ID,HandleClearAllBtnClick);

    bg.bg_type = GUI_BG_COLOR;
    bg.shape = GUI_SHAPE_RECT;
    bg.color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT);
    GUIBUTTON_SetPressedFg(MMIIDLE_PUSH_MSG_BUTTON_CTRL_ID,&bg);

    font.color = MMI_WHITE_COLOR;
    font.font = MMI_DEFAULT_BIG_FONT;
    GUIBUTTON_SetFont(MMIIDLE_PUSH_MSG_BUTTON_CTRL_ID,&font);

    GUIBUTTON_SetRunSheen(MMIIDLE_PUSH_MSG_BUTTON_CTRL_ID,FALSE);

    if(0 == GUILIST_GetTotalItemNum(MMIIDLE_PUSH_MSG_LIST_CTRL_ID))
    {
        GUIFORM_SetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_PUSH_MSG_BUTTON_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    }
}
/*****************************************************************************/
//  Description : update idle 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_UpdateMusicProgressTimeinfo(void)
{
    if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
    {
        MMI_CTRL_ID_T   ctrl_id1 =  MMIIDLE_MUSIC_PROGRESSBAR_CTRL_ID;
        MMI_CTRL_ID_T   ctrl_id2 =  MMIIDLE_MUSIC_CURTIME_OWNDRAW_CTRL_ID;
        MMI_CTRL_ID_T   ctrl_id3 =  MMIIDLE_MUSIC_TOTALTIME_OWNDRAW_CTRL_ID;
        
        GUIOWNDRAW_Update(ctrl_id1);
        GUIOWNDRAW_Update(ctrl_id2);
        GUIOWNDRAW_Update(ctrl_id3);
    }     
}
/******************************************************************************/
//  Description : Create softkey.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_CreateSoftkey(void)
{
    MMI_HANDLE_T idle_win_handle = MMK_ConvertIdToHandle(MAIN_IDLE_WIN_ID);
    //create softkey
    GUISOFTKEY_INIT_DATA_T softkey_data = {0};
    MMI_CONTROL_CREATE_T softkey_create = {0};
    MMI_CTRL_ID_T softkey_ctrl_id = 0;

    softkey_data.leftsoft_id = IMAGE_NULL;
    softkey_data.midsoft_id = TXT_NULL;
    softkey_data.rightsoft_id = IMAGE_NULL;

    softkey_ctrl_id = MMITHEME_GetSoftkeyCtrlId();
    softkey_create.ctrl_id = softkey_ctrl_id;
    softkey_create.guid    = SPRD_GUI_SOFTKEY_ID;
    softkey_create.parent_win_handle = idle_win_handle;
    softkey_create.init_data_ptr = &softkey_data;

    MMK_SetWinDisplayStyleState(idle_win_handle, WS_HAS_BUTTON_SOFTKEY | WS_DISABLE_SOFTKEY_TO_TITLE, TRUE );

    MMK_CreateControl( &softkey_create );
    MMK_SetWinSoftkeyCtrlId(idle_win_handle, softkey_ctrl_id);
	GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, TXT_COMMON_MAINMENU, LEFT_BUTTON, FALSE);
    GUIWIN_SeSoftkeytButtonTextId(idle_win_handle, TXT_IDLE_LOCK, RIGHT_BUTTON, FALSE);
}
/******************************************************************************/
//  Description : Create control dynamically.
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_CreateMusicNotificationCtrl(void)
{
    MMI_HANDLE_T idle_win_handle = MMK_ConvertIdToHandle(MAIN_IDLE_WIN_ID);
    uint16 ver_space = 0;
    uint16 hor_space = 0;
    //Music Parent Form
    {
        GUIFORM_DYNA_CHILD_T    child_data = {0};
        GUIFORM_INIT_DATA_T     init_data = {0};
        MMITHEME_FORM_T         form_theme = {0};         
        //init data
        init_data.layout_type = GUIFORM_LAYOUT_ORDER;

        child_data.is_bg = FALSE;
        child_data.is_get_active = TRUE;
        child_data.guid          = SPRD_GUI_FORM_ID;
        child_data.child_handle  = MMIIDLE_MUSIC_FORM_CTRL_ID;
        child_data.init_data_ptr = &init_data;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
            MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,
            &child_data);
		MMITHEME_GetFormTheme(&form_theme);
		form_theme.form_active_child.bg.shape = GUI_SHAPE_RECT;
		CTRLFORM_SetFormActiveChildTheme(MMK_ConvertIdToHandle (MMIIDLE_MUSIC_FORM_CTRL_ID), &form_theme.form_active_child.bg, PNULL);
        GUIFORM_SetStyle(MMIIDLE_MUSIC_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetChildSpace(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_MUSIC_FORM_CTRL_ID,&hor_space, &ver_space);
        //GUICTRL_SetProgress(MMK_GetCtrlPtr(MMIIDLE_MUSIC_FORM_CTRL_ID),FALSE);
    }
    //child song name label
    {
        GUILABEL_INIT_DATA_T    init_data = {0};
    	GUIFORM_DYNA_CHILD_T    child_data = {0};
    	
        //init data
        init_data.align = GUILABEL_ALIGN_MIDDLE;

    	child_data.is_bg = FALSE;
    	child_data.is_get_active = FALSE;
    	child_data.guid = SPRD_GUI_LABEL_ID;
    	child_data.init_data_ptr = &init_data;
    	child_data.child_handle = MMIIDLE_MUSIC_NAME_LABLE_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
            MMIIDLE_MUSIC_FORM_CTRL_ID,
            &child_data);
    }
    //child artist label
    {
        GUILABEL_INIT_DATA_T    init_data = {0};
    	GUIFORM_DYNA_CHILD_T    child_data = {0};
    	
        //init data
        init_data.align = GUILABEL_ALIGN_MIDDLE;

    	child_data.is_bg = FALSE;
    	child_data.is_get_active = FALSE;
    	child_data.guid = SPRD_GUI_LABEL_ID;
    	child_data.init_data_ptr = &init_data;
    	child_data.child_handle = MMIIDLE_MUSIC_ARTIST_LABLE_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
            MMIIDLE_MUSIC_FORM_CTRL_ID,
            &child_data);
    }
    //child progressbar
    {
        GUIOWNDRAW_INIT_DATA_T      init_data = {0};
    	GUIFORM_DYNA_CHILD_T    child_data = {0};

        init_data.OwnDrawFunc = UpdateIdleMusicNotiProgressOwnerDrawCtrl;

    	child_data.is_bg = FALSE;
    	child_data.is_get_active = FALSE;
    	child_data.guid = SPRD_GUI_OWNDRAW_ID;
    	child_data.init_data_ptr = &init_data;
    	child_data.child_handle = MMIIDLE_MUSIC_PROGRESSBAR_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
            MMIIDLE_MUSIC_FORM_CTRL_ID,
            &child_data);
    }
    //child NAVI form
    {
        GUIFORM_DYNA_CHILD_T    child_data = {0};
        GUIFORM_INIT_DATA_T     init_data = {0};

        //init data
        init_data.layout_type = GUIFORM_LAYOUT_SBS;

        child_data.is_get_active = TRUE;
        child_data.guid          = SPRD_GUI_FORM_ID;
        child_data.child_handle  = MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID;
        child_data.init_data_ptr = &init_data;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
            MMIIDLE_MUSIC_FORM_CTRL_ID,
            &child_data);

        GUIFORM_SetChildAlign(MMIIDLE_MUSIC_FORM_CTRL_ID,MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID,GUIFORM_CHILD_ALIGN_HMIDDLE);
        GUIFORM_SetChildSpace(MMIIDLE_MUSIC_FORM_CTRL_ID,MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID,&hor_space, &ver_space);
        GUIFORM_SetStyle(MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    }
    //child crutime
    {
        GUIOWNDRAW_INIT_DATA_T      init_data = {0};
    	GUIFORM_DYNA_CHILD_T    child_data = {0};

        init_data.OwnDrawFunc = UpdateIdleMusicNotitimeinfoOwnerDrawCtrl;

    	child_data.is_bg = FALSE;
    	child_data.is_get_active = FALSE;
    	child_data.guid = SPRD_GUI_OWNDRAW_ID;
    	child_data.init_data_ptr = &init_data;
    	child_data.child_handle = MMIIDLE_MUSIC_CURTIME_OWNDRAW_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
            MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID,
            &child_data);
    }
    //child Play/Pause
    {
        GUIOWNDRAW_INIT_DATA_T      init_data = {0};
    	GUIFORM_DYNA_CHILD_T    child_data = {0};

        init_data.OwnDrawFunc = UpdateIdleMusicNotiPlayNextbtnOwnerDrawCtrl;


        child_data.is_bg = FALSE;
    	child_data.is_get_active = TRUE;
    	child_data.guid = SPRD_GUI_OWNDRAW_ID;
    	child_data.init_data_ptr = &init_data;
    	child_data.child_handle = MMIIDLE_MUSIC_PALY_OWNDRAW_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
            MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID,
            &child_data);
    }
    //child Forward
    {
        GUIOWNDRAW_INIT_DATA_T      init_data = {0};
    	GUIFORM_DYNA_CHILD_T    child_data = {0};

        init_data.OwnDrawFunc = UpdateIdleMusicNotiPlayNextbtnOwnerDrawCtrl;

        child_data.is_bg = FALSE;
    	child_data.is_get_active = FALSE;
    	child_data.guid = SPRD_GUI_OWNDRAW_ID;
    	child_data.init_data_ptr = &init_data;
    	child_data.child_handle = MMIIDLE_MUSIC_NEXT_OWNDRAW_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
            MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID,
            &child_data);
    }
    //child totaltime
    {
        GUIOWNDRAW_INIT_DATA_T      init_data = {0};
    	GUIFORM_DYNA_CHILD_T    child_data = {0};

        init_data.OwnDrawFunc = UpdateIdleMusicNotitimeinfoOwnerDrawCtrl;

    	child_data.is_bg = FALSE;
    	child_data.is_get_active = FALSE;
    	child_data.guid = SPRD_GUI_OWNDRAW_ID;
    	child_data.init_data_ptr = &init_data;
    	child_data.child_handle = MMIIDLE_MUSIC_TOTALTIME_OWNDRAW_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
            MMIIDLE_MUSIC_CHIL_FORM_CTRL_ID,
            &child_data);
    }
}

/******************************************************************************/
//  Description : Create control dynamically.
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_CreateFmNotificationCtrl(void)
{
    MMI_HANDLE_T idle_win_handle = MMK_ConvertIdToHandle(MAIN_IDLE_WIN_ID);
    uint16 ver_space = 0;
    uint16 hor_space = 0;
    //FM Parent Form
    {
        GUIFORM_DYNA_CHILD_T    child_data = {0};
        GUIFORM_INIT_DATA_T     init_data = {0};
        MMITHEME_FORM_T         form_theme = {0};        
        //init data
        init_data.layout_type = GUIFORM_LAYOUT_ORDER;

        child_data.is_bg = FALSE;
        child_data.is_get_active = TRUE;
        child_data.guid          = SPRD_GUI_FORM_ID;
        child_data.child_handle  = MMIIDLE_FM_FORM_CTRL_ID;
        child_data.init_data_ptr = &init_data;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
            MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,
            &child_data);
		MMITHEME_GetFormTheme(&form_theme);
		form_theme.form_active_child.bg.shape = GUI_SHAPE_RECT;
		CTRLFORM_SetFormActiveChildTheme(MMK_ConvertIdToHandle (MMIIDLE_FM_FORM_CTRL_ID), &form_theme.form_active_child.bg, PNULL);
        GUIFORM_SetStyle(MMIIDLE_FM_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetChildSpace(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_FM_FORM_CTRL_ID,&hor_space, &ver_space);
    }
    //child label
    {
        GUILABEL_INIT_DATA_T    init_data = {0};
    	GUIFORM_DYNA_CHILD_T    child_data = {0};
    	
        //init data
        init_data.align = GUILABEL_ALIGN_MIDDLE;

    	child_data.is_bg = FALSE;
    	child_data.is_get_active = FALSE;
    	child_data.guid = SPRD_GUI_LABEL_ID;
    	child_data.init_data_ptr = &init_data;
    	child_data.child_handle = MMIIDLE_FM_FREQ_LABLE_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
            MMIIDLE_FM_FORM_CTRL_ID,
            &child_data);

        GUIFORM_SetChildAlign(MMIIDLE_FM_FORM_CTRL_ID,MMIIDLE_FM_FREQ_LABLE_CTRL_ID,GUIFORM_CHILD_ALIGN_HMIDDLE);
    }
    //child NAVI form
    {
        GUIFORM_DYNA_CHILD_T    child_data = {0};
        GUIFORM_INIT_DATA_T     init_data = {0};

        //init data
        init_data.layout_type = GUIFORM_LAYOUT_SBS;

        child_data.is_get_active = TRUE;
        child_data.guid          = SPRD_GUI_FORM_ID;
        child_data.child_handle  = MMIIDLE_FM_CHIL_NAVI_FORM_CTRL_ID;
        child_data.init_data_ptr = &init_data;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
            MMIIDLE_FM_FORM_CTRL_ID,
            &child_data);

        GUIFORM_SetChildAlign(MMIIDLE_FM_FORM_CTRL_ID,MMIIDLE_FM_CHIL_NAVI_FORM_CTRL_ID,GUIFORM_CHILD_ALIGN_HMIDDLE);
        GUIFORM_SetChildSpace(MMIIDLE_FM_FORM_CTRL_ID,MMIIDLE_FM_CHIL_NAVI_FORM_CTRL_ID,&hor_space, &ver_space);
        GUIFORM_SetStyle(MMIIDLE_FM_CHIL_NAVI_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    }
    //child Null
    {
        GUIANIM_INIT_DATA_T    init_data = {0};
        GUIFORM_DYNA_CHILD_T    child_data = {0};

        //init data
        init_data.is_form_bg = FALSE;

        child_data.is_bg = FALSE;
    	child_data.is_get_active = FALSE;
    	child_data.guid = SPRD_GUI_BUTTON_ID;
    	child_data.init_data_ptr = &init_data;
    	child_data.child_handle = MMIIDLE_FM_NULL_ANIM_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
            MMIIDLE_FM_CHIL_NAVI_FORM_CTRL_ID,
            &child_data);
    }
    //child Play/Pause
    {
        GUIANIM_INIT_DATA_T    init_data = {0};
        GUIFORM_DYNA_CHILD_T    child_data = {0};

        //init data
        init_data.is_form_bg = FALSE;

        child_data.is_bg = FALSE;
    	child_data.is_get_active = TRUE;
    	child_data.guid = SPRD_GUI_ANIM_ID;
    	child_data.init_data_ptr = &init_data;
    	child_data.child_handle = MMIIDLE_FM_PALY_ANIM_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
            MMIIDLE_FM_CHIL_NAVI_FORM_CTRL_ID,
            &child_data);
    }
    //child Forward
    {
        GUIANIM_INIT_DATA_T    init_data = {0};
        GUIFORM_DYNA_CHILD_T    child_data = {0};

        //init data
        init_data.is_form_bg = FALSE;

        child_data.is_bg = FALSE;
    	child_data.is_get_active = FALSE;
    	child_data.guid = SPRD_GUI_ANIM_ID;
    	child_data.init_data_ptr = &init_data;
    	child_data.child_handle = MMIIDLE_FM_NEXT_ANIM_CTRL_ID;

        //create
        GUIFORM_CreatDynaChildCtrl(idle_win_handle,
            MMIIDLE_FM_CHIL_NAVI_FORM_CTRL_ID,
            &child_data);
    }
}
/******************************************************************************/
//  Description : Create control dynamically.
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_CreatePushMsgNotificationCtrl(void)
{
    MMI_HANDLE_T idle_win_handle = MMK_ConvertIdToHandle(MAIN_IDLE_WIN_ID);
    GUI_BOTH_RECT_T  both_rect  = MMITHEME_GetWinClientBothRect(idle_win_handle);
    //Form msg list
    {
        GUILIST_INIT_DATA_T    init_data = {0};
        GUIFORM_DYNA_CHILD_T    child_data = {0};

        both_rect.v_rect.top = both_rect.v_rect.bottom;
        both_rect.h_rect.top = both_rect.h_rect.bottom;
        
        init_data.both_rect = both_rect;
        init_data.is_dynamic_item_data = FALSE;
        init_data.max_item_num = MMI_IDLE_PUSH_MSG_MAX_COUNT;
        init_data.max_selected_num = init_data.max_item_num;
        init_data.scroll_bar_type = GUIPRGBOX_STYLE_VERTICAL_SCROLL;
        init_data.type = GUILIST_TEXTLIST_E;
        
        child_data.is_bg = FALSE;
        child_data.is_get_active = TRUE;
        child_data.guid = SPRD_GUI_LIST_ID;
        child_data.init_data_ptr = &init_data;
        child_data.child_handle = MMIIDLE_PUSH_MSG_LIST_CTRL_ID;

        GUIFORM_CreatDynaChildCtrl(idle_win_handle, 
            MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, 
            &child_data);
        GUILIST_PermitBorder(MMIIDLE_PUSH_MSG_LIST_CTRL_ID,FALSE);
        GUILIST_SetIsPermitBottomMargin(MMIIDLE_PUSH_MSG_LIST_CTRL_ID,FALSE);
    }
    //from button
    {
        GUIBUTTON_INIT_DATA_T       button_init = {0};
        GUIFORM_DYNA_CHILD_T    child_data = {0};

        child_data.child_handle = MMIIDLE_PUSH_MSG_BUTTON_CTRL_ID;
        child_data.guid = SPRD_GUI_BUTTON_ID;
        
        child_data.init_data_ptr = &button_init;
        
        child_data.is_bg = FALSE;
        child_data.is_get_active = TRUE;

        GUIFORM_CreatDynaChildCtrl(idle_win_handle, 
            MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, 
            &child_data);
        GUIFORM_SetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, MMIIDLE_PUSH_MSG_BUTTON_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    }
}

/******************************************************************************/
//  Description : Create control dynamically.
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_CreateDynamicCtrl(MMI_WIN_ID_T win_id)
{
    MMI_HANDLE_T idle_win_handle = MMK_ConvertIdToHandle(win_id);
    GUI_BOTH_RECT_T  both_rect  = MMITHEME_GetWinClientBothRect(win_id);
    uint16 bottom_margin = FORM_BOTTOM_MAGRIN_HEIGHT;
    uint16 ver_space = 0;
    uint16 hor_space = 0;

    both_rect.v_rect.bottom -= bottom_margin;
    both_rect.h_rect.bottom -= bottom_margin;
    //parent form
    {
        GUI_BG_T bg = {0};
                
        GUIFORM_CreatDynaCtrl(idle_win_handle,MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,GUIFORM_LAYOUT_ORDER);
        bg.bg_type = GUI_BG_HALF_TRANS;
        bg.color = MMITHEME_GetTransparenceColor();
        GUIFORM_SetBg(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,&bg);
        //GUIFORM_PermitChildBg(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,FALSE);
        GUIFORM_PermitChildBorder(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, FALSE);
        GUIFORM_PermitChildFont(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, FALSE);
        //GUIFORM_SetCircularHandleUpDown(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,TRUE);
        GUIFORM_SetType(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,GUIFORM_TYPE_TP);
        GUIAPICTRL_SetBothRect(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, &both_rect);
        GUIFORM_SetDividingLine(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,0,MMITHEME_GetTransparenceColor());
        GUIFORM_SetMargin(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,0);
        GUIFORM_SetSpace(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, &hor_space, &ver_space);
    }

    MMIIDLE_CreateFmNotificationCtrl();
    MMIIDLE_CreateMusicNotificationCtrl();
    MMIIDLE_CreatePushMsgNotificationCtrl();
}

LOCAL void OutputIdleLunarDate(void)
{
	GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
	GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
	GUI_LCD_DEV_INFO            lcd_dev_info        = *GetCommonIdlePaintLcdDev();	
	GUI_RECT_T          rect = {0};
	GUI_POINT_T dis_point = {0};
	MMI_STRING_T  LunarDate_str ={0};
	uint16 file_name_len = 0;
	wchar       lunar_name[30 + 1]={0};
	uint16			str_width = 0;
	uint16          str_height = 0;
	GUI_COLOR_T  lunar_color =MMI_WHITE_COLOR ;//MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_DATE);
 	MMISET_WALLPAPER_SET_T  wallpaper_info  =   {0};
	 MMI_IMAGE_ID_T      bg_img = 0;
	 
	LunarDate_str.wstr_ptr = lunar_name; 
	ZDT_LunarDate(&LunarDate_str, 30);
	MMIIDLE_GetIdleTextStyle(&text_style , MMI_DEFAULT_NORMAL_FONT,lunar_color );
		   
	str_width = GUISTR_GetStringWidth(&text_style,&LunarDate_str,state);
	str_height = GUI_GetStringHeight(MMI_DEFAULT_NORMAL_FONT, LunarDate_str.wstr_ptr, LunarDate_str.wstr_len);
	// 绘制字符串
	dis_point.x = 0;
	dis_point.y = 136;	 
	rect.top = dis_point.y ;
	rect.left =  0;///dis_point.x;
	rect.right = 240 ;//rect.left  + str_width;
	rect.bottom = rect.top + str_height +2 ;
	
	text_style.align = ALIGN_HMIDDLE;
		
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


/*****************************************************************************/
//  Description : output date
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  OutputIdleWinDate(void)
{
    MMI_IDLE_DISPLAY_T          idle_disp_style     = {0};
    GUI_POINT_T                 point               = {0};
    //SCI_DATE_T                  date                = {0};
#ifdef DPHONE_SUPPORT //added by taiping.lai 2011/01/04
    SCI_TIME_T         	 	  time = {0};
    uint8                         datestr[21]         = {0};
    wchar                       datewstr[21]        = {0};
#else    
    uint8                       datestr[16]         = {0};
    wchar                       datewstr[16]        = {0};
#endif 
    uint16                      len                 = {0}; 
    GUI_FONT_T                  font                = MMI_DEFAULT_NORMAL_FONT;//SONG_FONT_8;
    GUI_COLOR_T                 color               = 0;
    uint16                      str_width           = 0;
    GUI_LCD_DEV_INFO            lcd_dev_info        = *GetCommonIdlePaintLcdDev();
    //MMISET_DATE_DISPLAY_TYPE_E  date_display_type   = MMISET_GetDateDisplayType();    
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T            text   = {0};


    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_DATE);
    if(MMITHEME_IsMainScreenLandscape())
    {
        
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
		idle_disp_style.text_frame_top = IDLE_DATE_TOP_H;
        idle_disp_style.text_frame_x = MMITHEME_GetClientRectWidth()-IDLE_DATE_LEFT_MARGIN;
#elif defined MAINLCD_SIZE_176X220 
		idle_disp_style.text_frame_top = IDLE_DATE_TOP_H;
        idle_disp_style.text_frame_x = IDLE_DATE_LEFT_H;
#else
        //idle_disp_style.text_frame_x = IDLE_DATE_LEFT_H ;
#endif
    }
    font = SONG_FONT_36;// idle_disp_style.text_font;
    color = MMI_WHITE_COLOR ;//MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_DATE);
      
    MMIAPISET_GetCurrentDateStr(datestr,'-',16);

#ifdef DPHONE_SUPPORT //added by taiping.lai 2011/01/04
	TM_GetSysTime(&time);
	len = strlen((char *)datestr);
	datestr[len++] = ' ';
	MMIAPISET_FormatTimeStrByTime(time.hour, time.min, datestr + len, sizeof(datestr) - len);
	len = strlen((char *)datestr);
    MMI_STRNTOWSTR( datewstr, 20, datestr, 20, len );
#else
    len = strlen((char *)datestr);
    MMI_STRNTOWSTR( datewstr, 16, datestr, 16, len );
#endif 
    MMIIDLE_GetIdleTextStyle(&text_style, font, color);
    text_style.align = ALIGN_LEFT;
    
    text.wstr_len = len;
    text.wstr_ptr = datewstr;

    str_width = GUISTR_GetStringWidth(&text_style,&text,text_state);
   
    point = CalStartPoint(idle_disp_style, str_width);
    point.x = 5;
#if defined TOUCH_PANEL_SUPPORT  
    if (point.x <s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.right)
    {
#ifdef MAINLCD_SIZE_176X220
        //point.x = s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.right+5;
#elif  defined (MAINLCD_SIZE_128X160)
        //point.x = s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.right;
#else
        point.x = s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.right+10;
#endif        
    }

    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM7].type = MMI_MAIN_LCD_DATE;
   
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM7].rect.left   = point.x;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM7].rect.top    = point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM7].rect.right  = point.x+str_width;
	s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM7].rect.bottom = point.y+GUI_GetFontHeight(font,UNICODE_HANZI);
#endif    
    GUISTR_DrawTextToLCDSingleLine( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&point,
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state
        );
}

/*****************************************************************************/
//  Description : output counted time
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinCountedTime(BOOLEAN is_highlight)
{
#ifdef COUNTEDTIME_SUPPORT
    MMI_IDLE_DISPLAY_T          idle_disp_style = {0};
    GUI_POINT_T                 point           = {0};
    SCI_TIME_T                  time            = {0};
    uint8                       timestr[20]     = {0};
    //wchar                         disptime[20] = {0};
    uint8                       disptime_len    = 0;
    MMI_STRING_T                tmp_prompt      = {0};
    MMI_STRING_T                disp_str        = {0};

    GUI_FONT_T                  font            = SONG_FONT_8;
    GUI_COLOR_T                 color           = 0;
    uint16                      str_width       = 0;
    GUI_LCD_DEV_INFO            lcd_dev_info    = *GetCommonIdlePaintLcdDev();
    GUISTR_STYLE_T              text_style = {0};/*lint !e64*/
    GUISTR_STATE_T              text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
//@for light function modify
#if defined MAINLCD_SIZE_240X320
    uint16                      icon_width = 0;
#endif

    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());
    font = idle_disp_style.text_font;
    color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_COUNTTIME);

    //"倒计时"
    MMI_GetLabelTextByLang(TXT_TOOLS_COUNTEDTIME, &tmp_prompt);

    //XX:XX
    time = MMIAPICT_GetCurTime();
//     if (MMIAPICT_IsLessThanOnemin())
//     {
//         sprintf((char*)timestr,": %02d:%02d", time.min, time.sec);//小于1分钟时显示"分分:秒秒"      
//     }
//     else
//     {
//         sprintf((char*)timestr,": %02d:%02d", time.hour, time.min);//大于等于1分钟时显示"时时:分分"
//     }
    sprintf((char*)timestr,": %02d:%02d:%02d", time.hour,time.min, time.sec);
    disptime_len = strlen((char *)timestr); 

    // the total string length
    disp_str.wstr_len =  disptime_len + tmp_prompt.wstr_len ;

    // allocate memory
    disp_str.wstr_ptr = SCI_ALLOC_APP((disp_str.wstr_len + 1) * sizeof(wchar) );
    
    if (PNULL == disp_str.wstr_ptr)
    {
        //SCI_TRACE_LOW:"MMIIDLE OutputIdleWinCountedTime PNULL == disp_str.wstr_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_CSTYLE_988_112_2_18_2_23_42_21,(uint8*)"");
        return;
    }
    //disp_str.is_ucs2 = tmp_prompt.is_ucs2;
    
    //copy the string
    MMIAPICOM_Wstrncpy(
        disp_str.wstr_ptr, 
        tmp_prompt.wstr_ptr,  
        tmp_prompt.wstr_len
                );

    // copy the num
    MMI_STRNTOWSTR(
                &disp_str.wstr_ptr[tmp_prompt.wstr_len], 
                disptime_len,
                timestr,  
                disptime_len,
                disptime_len
                );

    if (is_highlight)
    {
        MMIIDLE_GetIdleTextStyle(&text_style, font, MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_COUNTTIME));
    }
    else
    {
        MMIIDLE_GetIdleTextStyle(&text_style, font, color);
    }

    str_width = GUISTR_GetStringWidth(&text_style,&disp_str,text_state);
    
    point = CalStartPoint(idle_disp_style, str_width);

//@for light function modify
#if defined MAINLCD_SIZE_240X320 
#if 0
   //display icon
    GUIRES_DisplayImg(&point,
      PNULL,
      PNULL,
      MMK_GetFirstAppletHandle(),
      IMAGE_IDLE_PROMPT_COUNT_DOWN,
      GetCommonIdlePaintLcdDev());
    GUIRES_GetImgWidthHeight(&icon_width,PNULL, IMAGE_IDLE_PROMPT_COUNT_DOWN,MMK_GetFirstAppletHandle());
    
    point.x = point.x + icon_width + 7;
#endif
#endif
#if defined TOUCH_PANEL_SUPPORT
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].type        = MMI_MAIN_LCD_COUNTEDTIME;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.left   = point.x;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.top    = point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.right  = point.x+str_width;
	s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.bottom = point.y+GUI_GetFontHeight(font,UNICODE_HANZI);
#endif

    if (is_highlight)
    {
        GUISTR_DrawTextToLCDSingleLine( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_POINT_T      *)&point,
            (const MMI_STRING_T     *)&disp_str,
            &text_style,
            text_state
            );
    }
    else
    {
        GUISTR_DrawTextToLCDSingleLine( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_POINT_T      *)&point,
            (const MMI_STRING_T     *)&disp_str,
            &text_style,
            text_state
            );
    }

    if (PNULL != disp_str.wstr_ptr)
    {
        SCI_FREE(disp_str.wstr_ptr);
        disp_str.wstr_ptr = PNULL;
    }
	SetDisplayNum();
#endif
}

#ifdef CALENDAR_SUPPORT
/*****************************************************************************/
//  Description : output calendar
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinCalendar(void)
{
    MMI_IDLE_DISPLAY_T			idle_disp_style = {0};   
    GUI_POINT_T		            point           = {0};
    GUI_FONT_T                  font            = SONG_FONT_8;
    uint16                      str_width       = 0;
	GUI_RECT_T                  mainlcd_rect = MMITHEME_GetFullScreenRect();
//@for light function modify
#if defined TOUCH_PANEL_SUPPORT || defined MAINLCD_SIZE_240X320
    uint16                      icon_width = 0;/*lint !e529*/
#endif
    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());
    font = idle_disp_style.text_font; 
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
    point.x = idle_disp_style.text_frame_x;
    point.y = idle_disp_style.text_frame_top;

    GUIRES_DisplayImg(&point,
        PNULL,
        PNULL,
        MMK_GetFirstAppletHandle(),
        IMAGE_IDLE_PROMPT_SCHEDULE,
        GetCommonIdlePaintLcdDev());

    GUIRES_GetImgWidthHeight(&icon_width,PNULL, IMAGE_IDLE_PROMPT_SCHEDULE,MMK_GetFirstAppletHandle());
    point.x = idle_disp_style.text_frame_x + icon_width + 7;
    point.y = idle_disp_style.text_frame_top;
   
    str_width = MMIIDLE_DisplaySchedule(GetCommonIdlePaintLcdDev(),MMI_DIRECTION_LEFT,mainlcd_rect.right,&point,font);
#else
    point.x = idle_disp_style.text_frame_x;
    point.y = idle_disp_style.text_frame_top;

    str_width = MMIIDLE_DisplaySchedule(GetCommonIdlePaintLcdDev(),MMI_DIRECTION_HMIDDLE,mainlcd_rect.right,&point,font);
#endif

#if defined TOUCH_PANEL_SUPPORT
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].type        = MMI_MAIN_LCD_CALENDAR;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].rect.left   = idle_disp_style.text_frame_x+ icon_width+ 7;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].rect.top    = point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].rect.right  = point.x+str_width;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].rect.bottom = point.y+GUI_GetFontHeight(font,UNICODE_HANZI);
#endif 
    SetDisplayNum()	;
}
#endif

/*****************************************************************************/
//  Description : output week
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  OutputIdleWinWeek(void)
{
#ifdef DPHONE_SUPPORT
	return;
#else  /* lint_lai */ 
    MMI_IDLE_DISPLAY_T  idle_disp_style = {0};
    GUI_POINT_T         point       = {0};
    SCI_DATE_T          date        = {0};
    GUI_FONT_T          font        = MMI_DEFAULT_NORMAL_FONT;
    GUI_COLOR_T         color       = 0;
    uint16              str_width   = 0;
    MMI_STRING_T        str_week    = {0};
#if defined TOUCH_PANEL_SUPPORT
	GUI_RECT_T          mainlcd_rect = MMITHEME_GetFullScreenRect();
#endif
    MMI_TEXT_ID_T       week_table[] = {
                                       TXT_SHORT_SUN,
                                       TXT_SHORT_MON,
                                       TXT_SHORT_TUE,
                                       TXT_SHORT_WED,
                                       TXT_SHORT_THU,
                                       TXT_SHORT_FRI,
                                       TXT_SHORT_SAT};
                                       
    GUI_LCD_DEV_INFO    lcd_dev_info = *GetCommonIdlePaintLcdDev();
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
		


    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_WEEK);
    if(MMITHEME_IsMainScreenLandscape())
    {
        
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
		idle_disp_style.text_frame_top = IDLE_WEEK_TOP_H;
        idle_disp_style.text_frame_x = MMITHEME_GetClientRectWidth() - 6;
#elif defined MAINLCD_SIZE_176X220  
        idle_disp_style.text_frame_top = IDLE_WEEK_TOP_H;
        idle_disp_style.text_frame_x = IDLE_WEEK_LEFT_H;
#else
        //idle_disp_style.text_frame_x = IDLE_WEEK_LEFT_H;
#endif
    }
    font =SONG_FONT_36 ;// idle_disp_style.text_font;
    color = MMI_WHITE_COLOR;// MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_WEEK);

    TM_GetSysDate(&date);
    MMI_GetLabelTextByLang(week_table[date.wday],&str_week);

    MMIIDLE_GetIdleTextStyle(&text_style, font, color);

    str_width = GUISTR_GetStringWidth(&text_style,&str_week,text_state);
    point = CalStartPoint(idle_disp_style, str_width);
#if defined TOUCH_PANEL_SUPPORT    
    if (point.x < s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM7].rect.right)
    {
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
        point.x = MIN(s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM7].rect.right +5,mainlcd_rect.right-str_width);
#else
        point.x = MIN(s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM7].rect.right +10,mainlcd_rect.right-str_width-7);
#endif        
    }
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM8].type        = MMI_MAIN_LCD_WEEK;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM8].rect.left   = point.x;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM8].rect.top    = point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM8].rect.right  = MIN(point.x+str_width,mainlcd_rect.right);
	s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM8].rect.bottom = point.y+GUI_GetFontHeight(font,UNICODE_HANZI);
#endif    
    GUISTR_DrawTextToLCDSingleLine( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&point,
        (const MMI_STRING_T     *)&str_week,
        &text_style,
        text_state
        );
#endif    
}

#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
/*****************************************************************************/
//  Description : output cell info
//  Global resource dependence : 
//  Author: Yongli Tong
//  Note: 
/*****************************************************************************/
LOCAL void  OutputIdleCellInfo(void)
{

    MMI_IDLE_DISPLAY_T			idle_disp_style = {0};   
    GUI_POINT_T		            point           = {0};
    GUI_FONT_T                  font            = SONG_FONT_8;
    uint16                      str_width       = 0;
	GUI_RECT_T                  mainlcd_rect = MMITHEME_GetFullScreenRect();
//@for light function modify
#if defined TOUCH_PANEL_SUPPORT || defined MAINLCD_SIZE_240X320
    uint16                      icon_width = 0;/*lint !e529*/
#endif
    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum() + 1);
    font = idle_disp_style.text_font; 
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
    point.x = idle_disp_style.text_frame_x;
    point.y = idle_disp_style.text_frame_top;

    GUIRES_DisplayImg(&point,
        PNULL,
        PNULL,
        MMK_GetFirstAppletHandle(),
        IMAGE_IDLE_PROMPT_SCHEDULE,
        GetCommonIdlePaintLcdDev());

    GUIRES_GetImgWidthHeight(&icon_width,PNULL, IMAGE_IDLE_PROMPT_SCHEDULE,MMK_GetFirstAppletHandle());
    point.x = idle_disp_style.text_frame_x + icon_width + 7;
    point.y = idle_disp_style.text_frame_top;
   
    str_width = MMIIDLE_DisplayCellInfo(GetCommonIdlePaintLcdDev(),MMI_DIRECTION_LEFT,mainlcd_rect.right,&point,font);
#else
    point.x = idle_disp_style.text_frame_x;
    point.y = idle_disp_style.text_frame_top;

    str_width = MMIIDLE_DisplayCellInfo(GetCommonIdlePaintLcdDev(),MMI_DIRECTION_HMIDDLE,mainlcd_rect.right,&point,font);
#endif

#if defined TOUCH_PANEL_SUPPORT
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].type        = MMI_MAIN_LCD_CALENDAR;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].rect.left   = idle_disp_style.text_frame_x+ icon_width+ 7;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].rect.top    = point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].rect.right  = point.x+str_width;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].rect.bottom = point.y+GUI_GetFontHeight(font,UNICODE_HANZI);
#endif 
    SetDisplayNum()	;

}
#endif

#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)
/*****************************************************************************/
//  Description : display the mp3 name or not
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
LOCAL void DisplayIdleMp3Name(void)
{
//    IDLE_WIN_INFO_T       *idlewin_info_ptr = MMIIDLE_GetIdleWinInfo();
    MMISET_IDLE_DISPLAY_T   *idle_set_ptr  =   PNULL;
    idle_set_ptr = MMIAPISET_GetIdleDisplayMode();
//    if (idlewin_info_ptr->is_display_mp3)//显示mp3 name
//    {
        //display mp3 name
        if (g_idle_cur_mp3_name.is_scroll)
        {
            MMIIDLECOM_StartIdleMp3Timer();
            DisplayIdleCycleMp3Name();
        }
        else
        {
            MMI_IDLE_DISPLAY_T  idle_disp_style = {0};
            GUI_FONT_T          font            = SONG_FONT_8;
            GUI_COLOR_T         color           = 0;
            GUI_POINT_T         point           = {0};
            GUI_LCD_DEV_INFO    lcd_dev_info    = *GetCommonIdlePaintLcdDev();
            uint8               string_width    =0;
            GUISTR_STYLE_T      text_style = {0};/*lint !e64*/
            GUISTR_STATE_T      text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT;
            MMI_STRING_T        text = {0};
            uint16              lcd_width = 0; // lcd宽度
            uint16              lcd_height = 0; // lcd高度
            uint16              music_name_max_width = 0; //MMI_MAINSCREEN_MP3_WIDTH;
            wchar               mp3_name[MMIMP3_NAME_MAX_LEN + 1]={0};
//@for light function modify
#if defined MAINLCD_SIZE_240X320
            MMI_IMAGE_ID_T      icon_id  = IMAGE_IDLE_PROMPT_PLAY;/*lint !e529*/
            uint16              icon_width = 0;/*lint !e529*/
#endif
            text.wstr_ptr = mp3_name; 
			idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());

            if (MMIAPIMP3_IsPlayerActive() 
                && idle_set_ptr->visible_flag.item_flag.is_show_mp3&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
            {
                #ifdef FM_SUPPORT
                if ((FM_PLAYING == MMIAPIFM_GetStatus()) 
                    && idle_set_ptr->visible_flag.item_flag.is_show_fm&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
                {
                    //display fm
                    MMIAPIFM_GetCurFreqName(&text, MMIMP3_NAME_MAX_LEN);
                }
                else
                #endif
                {
                    MMIAPIMP3_GetCurMp3Name(&text, MMIMP3_NAME_MAX_LEN);
                }
                 SetDisplayNum();   
            }  
            else
            { 
#ifdef FM_SUPPORT
                if ((FM_STOP != MMIAPIFM_GetStatus()) 
                    && idle_set_ptr->visible_flag.item_flag.is_show_fm && (GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
                {
                    //display fm
                    MMIAPIFM_GetCurFreqName(&text, MMIMP3_NAME_MAX_LEN);
					SetDisplayNum();
                }
				
#endif
            }
            GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
            music_name_max_width = (lcd_width-MMI_MAINSCREEN_MP3_LEFT-MMI_MAINSCREEN_MP3_LEFT);
           
            font =SONG_FONT_26 ;//idle_disp_style.text_font;
            color = MMI_WHITE_COLOR ;///MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_MP3);
             idle_disp_style.text_frame_top = 173;//
            //text.wstr_len = g_idle_cur_mp3_name.length;
            //text.wstr_ptr = g_idle_cur_mp3_name.mp3_name;
            g_idle_cur_mp3_name.str_width = GUI_CalculateStringPiexlNum(text.wstr_ptr,
                text.wstr_len,
                                                                    font,0);
            MMIIDLE_GetIdleTextStyle(&text_style, font, color);
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
        if((idle_set_ptr->visible_flag.item_flag.is_show_mp3&&(MMIMP3_PLAYER_STATE_NONE!=MMIAPIMP3_GetPlayerState())) 
#ifdef FM_SUPPORT
        ||(idle_set_ptr->visible_flag.item_flag.is_show_fm &&((FM_STOP != MMIAPIFM_GetStatus())))&& (GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM)
#endif
          )
        {
            point.x = idle_disp_style.text_frame_x; 
            point.y = idle_disp_style.text_frame_top;
            if(MMIAPIMP3_IsPlayerPlaying() || (FM_PLAYING == MMIAPIFM_GetStatus()))
            {
                icon_id = IMAGE_IDLE_PROMPT_PLAY;
            }
            else if(MMIAPIMP3_IsPlayerActive() || (FM_SUSPENDED == MMIAPIFM_GetStatus()))
            {
                icon_id = IMAGE_IDLE_PROMPT_PAUSE;
            }
			#if 0
            //display icon
            GUIRES_DisplayImg(&point,
                PNULL,
                PNULL,
                MMK_GetFirstAppletHandle(),
                icon_id,
                GetCommonIdlePaintLcdDev());
            GUIRES_GetImgWidthHeight(&icon_width,PNULL, IMAGE_IDLE_PROMPT_SCHEDULE,MMK_GetFirstAppletHandle());
            point.x = point.x + icon_width + 7;
			#endif	
        }
#else
            point.x = ((music_name_max_width - g_idle_cur_mp3_name.str_width)>>1)+MMI_MAINSCREEN_MP3_LEFT;            
            point.y = idle_disp_style.text_frame_top;
#endif
#if defined TOUCH_PANEL_SUPPORT            
            s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].type = MMI_MAIN_LCD_MP3;
            
            s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.left = point.x;
#endif
            string_width = GUISTR_GetStringWidth(&text_style,&text,text_state);
#if defined TOUCH_PANEL_SUPPORT
            s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.right  = point.x + string_width;
            s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.top    = point.y;
            s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.bottom = point.y + GUI_GetFontHeight(font,UNICODE_HANZI);
#endif
            GUISTR_DrawTextToLCDSingleLine( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_POINT_T      *)&point,
                (const MMI_STRING_T     *)&text,
                &text_style,
                text_state
                );
			
        }

//    }
}

/*****************************************************************************/
//  Description : display the cycle mp3 name
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
LOCAL void DisplayIdleCycleMp3Name(void)
{
    uint16              str_index           = 0;
    MMI_IDLE_DISPLAY_T  idle_disp_style     = {0};
    GUI_RECT_T          rect                = {0};
    GUI_FONT_T          font                = SONG_FONT_8;
    GUI_COLOR_T         color               = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info        = *GetCommonIdlePaintLcdDev();
    GUISTR_STYLE_T      text_style          = {0};/*lint !e64*/
//#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT)	
 //   GUISTR_STATE_T      text_state          = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE |GUISTR_STATE_EFFECT;
//#else
    GUISTR_STATE_T      text_state          = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT;
//#endif		
    MMI_STRING_T        text                = {0};
	GUI_RECT_T                  mainlcd_rect = MMITHEME_GetFullScreenRect();
    wchar               mp3_name[MMIMP3_NAME_MAX_LEN + 1]={0};
    MMISET_IDLE_DISPLAY_T   *idle_set_ptr  =   PNULL;
    idle_set_ptr = MMIAPISET_GetIdleDisplayMode();
    text.wstr_ptr = mp3_name;
	
	MMIIDLE_GetIdleTextStyle(&text_style, font, color);	
#ifdef FM_SUPPORT    
	if (FM_PLAYING == MMIAPIFM_GetStatus()&& idle_set_ptr->visible_flag.item_flag.is_show_fm&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
	{
		MMIAPIFM_GetCurFreqName(&text, MMIMP3_NAME_MAX_LEN);
		idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());
		font = SONG_FONT_26 ;//idle_disp_style.text_font;
		 color =  MMI_WHITE_COLOR  ;//color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_FM);
        SetDisplayNum();
	}
	else 
#endif     
             if(MMIAPIMP3_IsPlayerActive() 
                && idle_set_ptr->visible_flag.item_flag.is_show_mp3&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
	{
		MMIAPIMP3_GetCurMp3Name(&text, MMIMP3_NAME_MAX_LEN);
		idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());
		font = SONG_FONT_26 ;//idle_disp_style.text_font;
		color =  MMI_WHITE_COLOR ;// MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_MP3);
        SetDisplayNum();
    }
 idle_disp_style.text_frame_top = 173;//
    //显示mp3 name,防止定时器到时之前，停止mp3播放，Mp3名字长度为0，导致出错!
    if (MMIIDLE_GetIdleWinInfo()->is_display_mp3)
    {
        //set rect
        rect.left = MMI_MAINSCREEN_MP3_LEFT;
        rect.right = mainlcd_rect.right - MMI_MAINSCREEN_MP3_LEFT;
        rect.top = idle_disp_style.text_frame_top;
        rect.bottom = rect.top + GUI_GetStringHeight(font, text.wstr_ptr, 
                            text.wstr_len);
	rect.bottom -= 1;	
#if defined TOUCH_PANEL_SUPPORT
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].type     = MMI_MAIN_LCD_MP3;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.left=rect.left;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.right=rect.right;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.top=rect.top;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.bottom=rect.bottom;
#endif


        str_index = g_idle_cur_mp3_name.str_index;

        if (str_index < MMIMP3_NAME_MAX_LEN)
        {
            text.wstr_len = (uint16)(text.wstr_len-str_index);
            text.wstr_ptr = text.wstr_ptr+str_index;

            text_style.font = font ;
		text_style.font_color = color;
            GUISTR_DrawTextToLCDInRect( 
                                       (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                       (const GUI_RECT_T      *)&rect,       //the fixed display area
                                       (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                                       (const MMI_STRING_T     *)&text,
                                       &text_style,
                                       text_state,
                                       GUISTR_TEXT_DIR_AUTO
                                       );
        }
    }
      // SetDisplayNum();
}
#endif

#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (KURO_SUPPORT)|| defined (FM_SUPPORT)
/*****************************************************************************/
//  Description : set idle mp3 name
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleBgPlayName(
                                BOOLEAN     is_display,
                                MMIBGPLAY_TYPE_E bg_type
                                )
{
    MMI_IDLE_DISPLAY_T  idle_disp_style = {0};
    GUI_FONT_T          font            = SONG_FONT_8;
    MMI_STRING_T        text            = {0};
    uint16              lcd_width = 0; // lcd宽度
    uint16              lcd_height = 0; // lcd高度
    uint16              music_name_max_width = 0; 
    wchar               mp3_name[MMIMP3_NAME_MAX_LEN + 1]={0};


    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_MP3);
    font = idle_disp_style.text_font;
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    music_name_max_width = lcd_width - MMI_MAINSCREEN_MP3_LEFT - MMI_MAINSCREEN_MP3_LEFT;    
    
    //显示当前播放的mp3 name,包括更换当前的mp3 name  

    if (is_display)
    {
        MMIIDLE_GetIdleWinInfo()->is_display_mp3 = is_display;
       
        text.wstr_ptr = mp3_name;

        if(MMIBGPLAY_TYPE_FM == bg_type)
        {
            //get fm name
            MMIAPIFM_GetCurFreqName(&text, MMIMP3_NAME_MAX_LEN);
        }
#ifdef KURO_SUPPORT        
        else if(MMIBGPLAY_TYPE_KURO == bg_type)
        {
            MMIAPIKUR_GetCurKurName(&text, MMIMP3_NAME_MAX_LEN);
        }
#endif         
#ifdef MMI_AUDIO_PLAYER_SUPPORT        
        else if(MMIBGPLAY_TYPE_MP3 == bg_type)
        {
            MMIAPIMP3_GetCurMp3Name(&text, MMIMP3_NAME_MAX_LEN);
        }
#endif        
        //set global variable
        //g_idle_cur_mp3_name.is_ucs2 = text.is_ucs2;
        //g_idle_cur_mp3_name.length = text.wstr_len;
        g_idle_cur_mp3_name.str_index = 0;
        
        //is scroll
        g_idle_cur_mp3_name.str_width = GUI_CalculateStringPiexlNum(text.wstr_ptr,
                                                                    text.wstr_len,
                                                                    font,0);
        if (music_name_max_width < g_idle_cur_mp3_name.str_width)
        {
            g_idle_cur_mp3_name.is_scroll = TRUE;
        }
        else
        {
            g_idle_cur_mp3_name.is_scroll = FALSE;
        }
    }
    else
    {
        if((MMIBGPLAY_TYPE_FM == bg_type && !MMIAPIMP3_IsPlayerActive() 
#ifdef KURO_SUPPORT
            && !MMIAPIKUR_IsPlayerActive()
#endif
            )
           || (MMIBGPLAY_TYPE_MP3 == bg_type && FM_STOP == MMIAPIFM_GetStatus() 
#ifdef KURO_SUPPORT
&& !MMIAPIKUR_IsPlayerActive()
#endif
            )
           || (MMIBGPLAY_TYPE_KURO == bg_type && FM_STOP == MMIAPIFM_GetStatus() && !MMIAPIMP3_IsPlayerActive()))
        {
            //no bg play
            MMIIDLE_GetIdleWinInfo()->is_display_mp3 = is_display;
            SCI_MEMSET(&g_idle_cur_mp3_name,0,sizeof(IDLE_MP3_NAME_INFO));
        }
    }

    //if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
    {
        MMIIDLE_SendMsgToIdleWin(MSG_IDLE_UPDATE_MP3, PNULL);
    }
}
#endif
/*****************************************************************************/
//  Description : display the  content
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void OutIdleWinContent(void)
{
    MMISET_IDLE_DISPLAY_T   *idle_mode  =   PNULL;
#ifdef UI_MULTILAYER_SUPPORT
    if(UILAYER_IsLayerActive(&s_common_idle_paint_layer_handle))
        {
            UILAYER_Clear(&s_common_idle_paint_layer_handle);
        }
#endif
    idle_mode = MMIAPISET_GetIdleDisplayMode();
    ResetDisplayNum();
#if defined TOUCH_PANEL_SUPPORT
    ResetDisplayTouchPanelArea();
#endif
    //show time and date
    if ( idle_mode->visible_flag.item_flag.is_show_time_and_date)
    {    
#if 1 //UNISOC_MMI_Delete
#if defined(DPHONE_SUPPORT) 
#else        
        OutputIdleWinTimerWithImage();  
#endif
#else
        OutputIdleWinTime();
#endif

#if 1 //UNISOC_MMI_Delete
#ifdef MAINLCD_SIZE_128X160
        OutputIdleWinDateAndWeek();
#else
        OutputIdleWinDate();                     
        OutputIdleWinWeek();
        OutputIdleLunarDate();
#endif

#else
        OutputIdleWinWeekAndDate();
#endif
    }

#if 1 //UNISOC_MMI_Delete
    //show network name
        OutputIdleWinNetworkName(); 
#endif

    //UpdateIdleWinPushNotificationList(MMIIDLE_PUSH_MSG_LIST_CTRL_ID);

#if 1//defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)
    if ((//CR134667
#ifdef MMI_AUDIO_PLAYER_SUPPORT
    (idle_mode->visible_flag.item_flag.is_show_mp3)
#else
    0
#endif
#ifdef FM_SUPPORT
    ||(idle_mode->visible_flag.item_flag.is_show_fm)
#endif		
    )&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
    {
        DisplayIdleMp3Name();
    }
#endif 

//show counted time
#if 0
def COUNTEDTIME_SUPPORT
    if (MMIAPICT_IsShowIdleCoutime()&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM)&&idle_mode->visible_flag.item_flag.is_show_countdown)
    {
        OutputIdleWinCountedTime(FALSE);
    }
    else
#endif           
    {
#if 0
def MMI_IDLE_CUSTOM_WORD_SUPPORT        
    //show idle words
    if ( idle_mode->visible_flag.item_flag.is_show_idle_words && (GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
    {
        OutputIdleWinWords();
    }
#endif        
    }

 #if 0 //UNISOC_MMI_Delete

#ifdef MMI_MULTI_SIM_SYS_QUAD
    if(
    (!MMIAPIMP3_IsPlayerActive())
    #ifdef FM_SUPPORT
     && (!MMIAPIFM_IsFmActive())
    #endif
    #ifdef KURO_SUPPORT
     && (!MMIAPIKUR_IsPlayerActive())
    #endif
    )
#endif    
    {
   
#ifdef CALENDAR_SUPPORT
        //show calendar
        if (idle_mode->visible_flag.item_flag.is_show_today_schedule&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
        {
            OutputIdleWinCalendar();
        }
#endif
#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
        OutputIdleCellInfo();
#endif
    }
#endif

#ifdef MMI_IDLE_STKTEXT_SUPPORT
    //if(idle_mode->visible_flag.item_flag.is_show_stk_idle_text)
    if(MMIIDLE_GetIdleWinInfo()->is_display_stk_idle_text)
    {
        GUI_LCD_DEV_INFO    lcd_dev_info    = *GetCommonIdlePaintLcdDev();
        MAIN_DrawStkIdleModeText(lcd_dev_info.block_id);
    }
#endif
    GUIWIN_UpdateSoftkey(MAIN_IDLE_WIN_ID);
    MMIIDLE_NotificationFillBottomRect();
    MMK_DrawAllChildCtrl(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID);
}

#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)
/*****************************************************************************/
//  Description : get idle mp3 info
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC IDLE_MP3_NAME_INFO* MMIIDLECOM_GetIdleMp3Info(void)
{
    return &g_idle_cur_mp3_name;
}
#endif

/*****************************************************************************/
//  Description : display the softkey of the idle window
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void DisplayIdleWinSoftkey(void)
{
    GUI_POINT_T             point                = {0};
	GUI_LCD_DEV_INFO        lcd_dev_info             = *GetCommonIdlePaintLcdDev();
	GUI_RECT_T              mainlcd_rect             = MMITHEME_GetFullScreenRect();

#if defined (MMI_RES_ORIENT_PORTRAIT) && defined (MAINLCD_SIZE_128X160)//arvin zhang added for idle softkey icon    	
	uint16                  idle_softkey_icon_width  = 0;
    uint16                  idle_softkey_icon_height = 0; 
	uint32                  i                        = 0;
	MMI_IMAGE_ID_T          idle_softkey_icon[MMIIDLE_SOFTKEY_ICON_MAX] = {0};
#ifndef MMI_COMMON_IDLE_IMG_SOFTKEY_SUPPORT	
#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
    if (0 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))
    {
		idle_softkey_icon[0] = IMAGE_IDLE_ICON_MENU;
		idle_softkey_icon[1] = IMAGE_IDLE_ICON_SOS;
    }
	else
#endif
	{
		idle_softkey_icon[0] = IMAGE_IDLE_ICON_MENU;
		idle_softkey_icon[1] = IMAGE_IDLE_ICON_PB;
    }
#else

#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
    if (0 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))
    {
		idle_softkey_icon[0] = IMAGE_IDLE_SHORTCUT_MAINMENU;
		idle_softkey_icon[1] = IMAGE_IDLE_ICON_SOS;
    }
	else
#endif

	{
		idle_softkey_icon[0] = IMAGE_IDLE_SHORTCUT_MAINMENU;
		idle_softkey_icon[1] = IMAGE_IDLE_SHORTCUT_PB;
    }
#endif
#ifdef MMI_COMMON_IDLE_IMG_SOFTKEY_SUPPORT
#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
    if (0 != MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))
#endif
	{
     MMIAPISET_GetFSMainIMGId(&idle_softkey_icon[0],&idle_softkey_icon[MMIIDLE_SOFTKEY_ICON_MAX-1]);
	}

#endif
    GUIRES_GetImgWidthHeight(&idle_softkey_icon_width, &idle_softkey_icon_height, idle_softkey_icon[0], MAIN_IDLE_WIN_ID);

	for (i = 0; i < MMIIDLE_SOFTKEY_ICON_MAX; i++)
	{
		
	    //set softkey icon display dis_point
		if (0 == i)
		{
			point.x = mainlcd_rect.left + MMIIDLE_SOFTKEY_MARGIN_WIDTH;//left icon		  
		}
		else if ((MMIIDLE_SOFTKEY_ICON_MAX-1) == i)
		{
			point.x = mainlcd_rect.right  - idle_softkey_icon_width - MMIIDLE_SOFTKEY_MARGIN_WIDTH;//right icon		    
		}
        else
        {
            point.x = (mainlcd_rect.right>>1 )- idle_softkey_icon_width + 2*MMIIDLE_SOFTKEY_MARGIN_WIDTH; 
        }
		point.y = mainlcd_rect.bottom - idle_softkey_icon_height - MMIIDLE_SOFTKEY_MARGIN_WIDTH;// the same height
		//display softkey icon
		GUIRES_DisplayImg(&point,
			PNULL,
			PNULL,
			MAIN_IDLE_WIN_ID,
			idle_softkey_icon[i],
		    &lcd_dev_info);
	}
#else 
    MMI_STRING_T            mess            = {0};	
    uint16                  str_pixel       = 0;
    uint16                  font_height     = 0; 
    uint16                  str_width       = 0;
    wchar                   *full_str       = PNULL;
    uint16                  full_len        = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
#ifndef TOUCH_PANEL_SUPPORT
    GUI_RECT_T              right_softkey_display_rect ={0};
    GUI_RECT_T              left_softkey_display_rect ={0};
#endif	
    //按照移动定制，左软键硬按键绑定为主菜单

#ifdef BAR_PHONE_WIDGET 
    MMI_TEXT_ID_T           left_key_id     = TXT_COMMON_MAINMENU;
     MMI_TEXT_ID_T           right_key_id    = TXT_IDLE_BUSINESS_MODE;
#else   
    MMI_TEXT_ID_T           left_key_id     = TXT_COMMON_MAINMENU;//STXT_MAIN_MESSAGE;
    MMI_TEXT_ID_T           right_key_id    = STXT_MAIN_PB;
#endif    
    //MMI_TEXT_ID_T           middle_key_id    = TXT_COMM_OPEN;
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
	
#ifdef CMCC_UI_STYLE
    GUI_POINT_T             dis_point       = {0};
    uint16                  mid_icon_width = 0;
    uint16                  mid_icon_height = 0;
#endif
    full_str = SCI_ALLOCA(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);
    SCI_MEMSET(full_str,0,(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL));
    MMIAPISET_GetFSMainTextId(&left_key_id,&right_key_id);
    
    //显示左软键
    font_height = GUI_GetFontHeight(MMI_FULLSCREEN_SOFTKEY_TEXT_FONT, UNICODE_HANZI);
    
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)left_key_id,&mess);
    
#if defined(BAR_PHONE_WIDGET)    
    point.y =  mainlcd_rect.bottom - MMITHEME_GetSoftkeyHeight()/2 - font_height/2 + 1;/*lint !e737*/
#else
    point.y =  mainlcd_rect.bottom - MMITHEME_GetSoftkeyHeight()/2 - font_height/2;/*lint !e737*/
#endif    
#ifdef  DPHONE_SUPPORT
    point.x = 1;
#elif defined(BAR_PHONE_WIDGET)    
    point.x = 2;
#else    
    point.x = 1;
#endif
  
    MMIIDLE_GetIdleTextStyle(&text_style, MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,  MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_SOFTKEY));
    
    str_pixel = GUISTR_GetStringWidth(&text_style,&mess,text_state);
    MMIIDLE_GetDisPlayString(mess.wstr_ptr,mess.wstr_len,(mainlcd_rect.right -1)/2,MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,full_str,&full_len);
#if defined TOUCH_PANEL_SUPPORT
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].type        = MMI_MAIN_LCD_LEFT_SOFTKEY;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.left   = point.x;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.top    = point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.right  = point.x+str_pixel+1;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.bottom = point.y+font_height;
    
    str_width = MMIIDLE_DisplayIdleText(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T *)&point,
        MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,
        MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_SOFTKEY),
        full_str, 
        full_len
        );
#ifdef BAR_PHONE_TOUCH_WIDGET
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.top    = s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.top-MMIIDLE_SOFTKEY_ADD_HEIGHT;
#endif
#else
    left_softkey_display_rect.left   = point.x;
    left_softkey_display_rect.top    = point.y;
    left_softkey_display_rect.right  = point.x+str_pixel+1;
    left_softkey_display_rect.bottom = point.y+font_height;

        str_width = MMIIDLE_DisplayIdleText(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T *)&point,
        MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,
        MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_SOFTKEY),
        full_str, 
        full_len
        );
#endif
    //显示右软键
    SCI_MEMSET(full_str,0,(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL));
    full_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;

#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
    if (0 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_SOS,&mess);
    }
    else
#endif
	{
	    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)right_key_id,&mess);  
	}

    str_pixel = GUISTR_GetStringWidth(&text_style,&mess,text_state);
    MMIIDLE_GetDisPlayString(mess.wstr_ptr,mess.wstr_len,(mainlcd_rect.right -1)/2,MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,full_str,&full_len);
#ifdef  DPHONE_SUPPORT
#if defined TOUCH_PANEL_SUPPORT
	point.x = MAX((mainlcd_rect.right - str_pixel-1),(s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.right+1));
#else
	point.x = MAX((mainlcd_rect.right - str_pixel-1),(left_softkey_display_rect.right+1));
#endif
#elif defined(BAR_PHONE_WIDGET)    
#if defined TOUCH_PANEL_SUPPORT
    point.x = MAX((mainlcd_rect.right - str_pixel-4),(s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.right+3));
#else
    point.x = MAX((mainlcd_rect.right - str_pixel-4),(left_softkey_display_rect.right+3));
#endif
//#elif defined (MAINLCD_SIZE_128X160)
//    point.x = MAX((mainlcd_rect.right - str_pixel-4),(s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.right));
#else    
#if defined TOUCH_PANEL_SUPPORT
    point.x = MAX((mainlcd_rect.right - str_pixel-4),(s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.right+10));
#else 
    point.x = MAX((mainlcd_rect.right - str_pixel-4),(left_softkey_display_rect.right+10));
#endif
#endif
#if defined(BAR_PHONE_WIDGET)    
    point.y = mainlcd_rect.bottom - MMITHEME_GetSoftkeyHeight()/2 - font_height/2 + 1;/*lint !e737*/
#else
    point.y = mainlcd_rect.bottom - MMITHEME_GetSoftkeyHeight()/2 - font_height/2;/*lint !e737*/
#endif    
#if defined TOUCH_PANEL_SUPPORT
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].type        = MMI_MAIN_LCD_RIGHT_SOFTKEY;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.left   = point.x;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.top    = point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.right  = MIN((point.x+str_pixel+1),mainlcd_rect.right);
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.bottom = point.y+font_height;
    
    str_width = MMIIDLE_DisplayIdleText(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T *)&point,
        MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,
        MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_SOFTKEY),
        full_str, 
        full_len
        );
#ifdef BAR_PHONE_TOUCH_WIDGET
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.top    = s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.top-MMIIDLE_SOFTKEY_ADD_HEIGHT;
#endif
#else
    right_softkey_display_rect.left   = point.x;
    right_softkey_display_rect.top    = point.y;
    right_softkey_display_rect.right  = MIN((point.x+str_pixel+1),mainlcd_rect.right);
    right_softkey_display_rect.bottom = point.y+font_height;

            str_width = MMIIDLE_DisplayIdleText(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T *)&point,
        MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,
        MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_SOFTKEY),
        full_str, 
        full_len
        );

#endif
    SCI_FREE(full_str);
    full_str = PNULL;
#ifdef CMCC_UI_STYLE
#ifndef MMI_IDLE_MINI_SUPPORT
    GUIRES_GetImgWidthHeight(&mid_icon_width, &mid_icon_height,IMAGE_IDLE_MIDDLE_SOFTKEY_ICON , MAIN_IDLE_WIN_ID);

    dis_point.x = (mainlcd_rect.right>>1 )- mid_icon_width;
    dis_point.y = mainlcd_rect.bottom - mid_icon_height - 2;
    //display middle softkey icon
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        MAIN_IDLE_WIN_ID,
        IMAGE_IDLE_MIDDLE_SOFTKEY_ICON,
        &lcd_dev_info);
#endif    
#endif
#endif//#ifdef MAINLCD_SIZE_128X160
}
#if defined TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : get tp type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_IDLE_DISPLAY_INDEX_E GetTpType(
                                        GUI_POINT_T   tp_point
                                        )
{
    uint32                            i               = 0;
    MMI_IDLE_DISPLAY_INDEX_E          tp_type         = MMI_MAIN_LCD_NONE;    
    
    for (i = 0; i < MMI_COMMON_IDLE_MAX_ITEM; i++)
    {
        if (GUI_PointIsInRect(tp_point,s_common_idle_display_rect[i].rect))
        {
            tp_type = s_common_idle_display_rect[i].type;
            break;
        }
    }

    return (tp_type);
}
#endif

#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)
/*****************************************************************************/
//  Description : start mp3 name idle timer
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIIDLECOM_StartIdleMp3Timer(void)
{
    BOOLEAN     result  =   FALSE;

    if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
        if ((0 == g_mp3_name_timer_id) &&
            (MMIIDLECOM_GetIdleMp3Info()->is_scroll))
        {
            g_mp3_name_timer_id = MMK_CreateWinTimer(MAIN_IDLE_WIN_ID, IDLE_MP3_TIMER_PERIOD, FALSE);
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : stop mp3 name idle timer
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLECOM_StopIdleMp3Timer(void)
{
    BOOLEAN     result  =   FALSE;

    if (0 != g_mp3_name_timer_id)
    {
        MMK_StopTimer(g_mp3_name_timer_id);
        g_mp3_name_timer_id = 0;
        result = TRUE;
    }

    return (result);
}
/*****************************************************************************/
// Description : UpdateMp3NameStartIndex
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void MMIIDLECOM_UpdateMp3NameStartIndex(void)
{  
    MMI_STRING_T        text = {0};
    wchar               mp3_name[MMIMP3_NAME_MAX_LEN + 1]={0};
    text.wstr_ptr = mp3_name;

    if(FM_PLAYING == MMIAPIFM_GetStatus())
    {
        MMIAPIFM_GetCurFreqName(&text, MMIMP3_NAME_MAX_LEN);
    }
    else
    {
        MMIAPIMP3_GetCurMp3Name(&text, MMIMP3_NAME_MAX_LEN);
    }
    
    MMIIDLECOM_GetIdleMp3Info()->str_index += 1;
    if (0 == text.wstr_len)//防错处理，防止没有文件名的Mp3
    {
        MMIIDLECOM_GetIdleMp3Info()->str_index = 0;
    }
    else
    {
        MMIIDLECOM_GetIdleMp3Info()->str_index %= text.wstr_len;
    }
    g_mp3_name_timer_id = 0;
}
/*****************************************************************************/
// Description : MMIIDLECOM_GetMp3NameTimerId
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC uint8 MMIIDLECOM_GetMp3NameTimerId(void)
{
    return g_mp3_name_timer_id;
}


#endif
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : create idle items paint layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void CreateCommonIdlePaintLayer(
                              MMI_WIN_ID_T      win_id
                              )
{
    uint16   layer_width = 0;
    uint16   layer_height = 0;
    UILAYER_CREATE_T    create_info = {0};
    UILAYER_APPEND_BLT_T        append_layer = {0};

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_common_idle_paint_layer_handle))    
    {
        //get tips layer width height
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&layer_width,&layer_height);    

        //creat layer
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = layer_width;
        create_info.height = layer_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE; 
        UILAYER_CreateLayer(
            &create_info,
            &s_common_idle_paint_layer_handle);

        //set color key
        UILAYER_SetLayerColorKey(&s_common_idle_paint_layer_handle,TRUE,UILAYER_TRANSPARENT_COLOR);
        
        append_layer.lcd_dev_info = s_common_idle_paint_layer_handle;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id, MMICOMMON_SOFTKEY_CTRL_ID), &s_common_idle_paint_layer_handle);
    }

    if (UILAYER_IsLayerActive(&s_common_idle_paint_layer_handle))
    {
        UILAYER_Clear(&s_common_idle_paint_layer_handle);
    }

}
#endif

/*****************************************************************************/
//  Description : get idle items paint layer ptr
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO * GetCommonIdlePaintLcdDev(void)
{
    GUI_LCD_DEV_INFO            *lcd_dev_info_ptr = PNULL;
#ifdef UI_MULTILAYER_SUPPORT
    if(UILAYER_IsLayerActive(&s_common_idle_paint_layer_handle))
    {
        lcd_dev_info_ptr = &s_common_idle_paint_layer_handle;
    }
    else
#endif
    {
        lcd_dev_info_ptr = MMITHEME_GetDefaultLcdDev(); /*lint !e605*/
    }
    return lcd_dev_info_ptr;
}
/*****************************************************************************/
//  Description : get idle items paint layer ptr
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO * GetCommonIdleNotiLcdDev(void)
{
    GUI_LCD_DEV_INFO            *lcd_dev_info_ptr = PNULL;
#ifdef UI_MULTILAYER_SUPPORT
    if(UILAYER_IsLayerActive(&s_common_idle_notification_layer_handle))
    {
        lcd_dev_info_ptr = &s_common_idle_notification_layer_handle;
    }
    else
#endif
    {
        lcd_dev_info_ptr = MMITHEME_GetDefaultLcdDev(); /*lint !e605*/
    }
    return lcd_dev_info_ptr;
}

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : release idle paint layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdlePaintLayer(void)
{
    // 释放层
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_common_idle_paint_layer_handle))    
    {
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(MAIN_IDLE_WIN_ID,
                              MMICOMMON_SOFTKEY_CTRL_ID),  MMITHEME_GetDefaultLcdDev()); /*lint !e605*/
        UILAYER_RELEASELAYER(&s_common_idle_paint_layer_handle); /*lint !e506 !e774 !e831*/
    }
}

/*****************************************************************************/
//  Description : create status bar layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note: 
/*****************************************************************************/
LOCAL void CreateCommonIdleStbLayer(
                              MMI_WIN_ID_T      win_id
                              )
{
    uint16 width = 0;
    uint16 height = 0;
    UILAYER_CREATE_T create_info = {0};  
    GUI_RECT_T main_rect = MMITHEME_GetFullScreenRect();    
    UILAYER_APPEND_BLT_T        append_layer = {0};

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_common_idle_stb_layer_handle))    
    {
        width  = main_rect.right - main_rect.left + 1;
        height = MMI_STATUSBAR_HEIGHT;
          
        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = win_id;
        create_info.offset_x        = 0;
        create_info.offset_y        = 0;
        create_info.width           = width;
        create_info.height          = height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;   
        create_info.format          = UILAYER_MEM_DOUBLE_COPY;
        
        UILAYER_CreateLayer(&create_info, &s_common_idle_stb_layer_handle);
            
        UILAYER_SetLayerColorKey(&s_common_idle_stb_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        append_layer.lcd_dev_info = s_common_idle_stb_layer_handle;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
        
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id, MMICOMMON_STATUSBAR_CTRL_ID), &s_common_idle_stb_layer_handle);             
        GUISTATUSBAR_SetBgIsDesplay(win_id, TRUE);                
    }
    
    if (UILAYER_IsLayerActive(&s_common_idle_stb_layer_handle))
    {
        UILAYER_Clear(&s_common_idle_stb_layer_handle);
    }
}

/*****************************************************************************/
//  Description : release idle paint layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdleStbLayer(
                              MMI_WIN_ID_T      win_id
                              )
{
    // 释放层
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_common_idle_stb_layer_handle))    
    {
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id,
                MMICOMMON_STATUSBAR_CTRL_ID),  MMITHEME_GetDefaultLcdDev()); /*lint !e605*/            
        UILAYER_RELEASELAYER(&s_common_idle_stb_layer_handle);/*lint !e506 !e774 !e831*/
    }
}

/*****************************************************************************/
//  Description : create notification layer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CreateCommonIdleNotiLayer(
                              MMI_WIN_ID_T      win_id
                              )
{
    uint16   layer_width = 0;
    uint16   layer_height = 0;
    UILAYER_CREATE_T create_info = {0};  
    UILAYER_APPEND_BLT_T        append_layer = {0};
    GUI_BOTH_RECT_T  both_rect  = {0};

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_common_idle_notification_layer_handle))    
    {
        //get tips layer width height
        GUIFORM_CHILD_DISPLAY_E   child_display_type1 = GUIFORM_CHILD_DISP_MAX;
        GUIFORM_CHILD_DISPLAY_E   child_display_type2 = GUIFORM_CHILD_DISP_MAX;
        GUIFORM_CHILD_DISPLAY_E   child_display_type3 = GUIFORM_CHILD_DISP_MAX;
        GUIFORM_CHILD_DISPLAY_E   child_display_type4 = GUIFORM_CHILD_DISP_MAX;
        GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_MUSIC_FORM_CTRL_ID,&child_display_type1);
        GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_FM_FORM_CTRL_ID,&child_display_type2);
        GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_PUSH_MSG_LIST_CTRL_ID,&child_display_type3);
        GUIFORM_GetChildDisplay(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID,MMIIDLE_PUSH_MSG_BUTTON_CTRL_ID,&child_display_type4);
        if(s_notification_form_auto_size)
        {
            both_rect = MMITHEME_GetWinClientBothRect(MAIN_IDLE_WIN_ID);
        }
        else if(GUIFORM_CHILD_DISP_HIDE != child_display_type1
            ||GUIFORM_CHILD_DISP_HIDE != child_display_type2
            ||GUIFORM_CHILD_DISP_HIDE != child_display_type3
            ||GUIFORM_CHILD_DISP_HIDE != child_display_type4)
        {
            GUIAPICTRL_GetBothRect(MMIIDLE_NOTI_PARENT_FORM_CTRL_ID, &both_rect);
        }
        layer_width = both_rect.v_rect.right - both_rect.v_rect.left;
        layer_height = both_rect.v_rect.bottom - both_rect.v_rect.top;
        layer_height += FORM_BOTTOM_MAGRIN_HEIGHT;
          
        create_info.lcd_id          = GUI_MAIN_LCD_ID;
        create_info.owner_handle    = win_id;
        create_info.offset_x        = both_rect.v_rect.left;
        create_info.offset_y        = both_rect.v_rect.top;
        create_info.width           = layer_width;
        create_info.height          = layer_height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;   
        //create_info.format          = UILAYER_MEM_DOUBLE_COPY;
        
        UILAYER_CreateLayer(&create_info, &s_common_idle_notification_layer_handle);
            
        //UILAYER_SetLayerAlpha(&s_common_idle_notification_layer_handle,165);
        append_layer.lcd_dev_info = s_common_idle_notification_layer_handle;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
        
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_NOTI_PARENT_FORM_CTRL_ID), &s_common_idle_notification_layer_handle);             
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_PUSH_MSG_LIST_CTRL_ID), &s_common_idle_notification_layer_handle);             
    }
    
    if (UILAYER_IsLayerActive(&s_common_idle_notification_layer_handle))
    {
        UILAYER_Clear(&s_common_idle_notification_layer_handle);
    }
}

/*****************************************************************************/
//  Description : release notification layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdleNotiLayer(
                              MMI_WIN_ID_T      win_id
                              )
{
    // 释放层
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_common_idle_notification_layer_handle))    
    {
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id,
                MMIIDLE_PUSH_MSG_LIST_CTRL_ID),  MMITHEME_GetDefaultLcdDev()); /*lint !e605*/            
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id,
                MMIIDLE_NOTI_PARENT_FORM_CTRL_ID),  MMITHEME_GetDefaultLcdDev()); /*lint !e605*/            
        UILAYER_RELEASELAYER(&s_common_idle_notification_layer_handle);/*lint !e506 !e774 !e831*/
    }
}

#endif

#endif //defined MMI_COMMON_IDLE_SUPPORT



/*Edit by script, ignore 2 case. Thu Apr 26 19:00:55 2012*/ //IGNORE9527
