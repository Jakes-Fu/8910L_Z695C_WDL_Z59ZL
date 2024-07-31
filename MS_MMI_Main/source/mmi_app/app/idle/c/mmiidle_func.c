/*****************************************************************************
** File Name:      mmiidle_func.c                                            *
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

#define _MMIIDLE_FUNC_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_idle_trc.h"
#include "mmiidle_func.h"

#include "mmiacc_event.h"
#include "mmidisplay_data.h"
#include "mmi_text.h"
#include "mmi_theme.h"
#if defined MMI_SMART_IDLE_SUPPORT || defined MMI_COMMON_IDLE_SUPPORT ||defined BAR_PHONE_WIDGET 
#include "mmischedule_export.h"
#endif
#include "mmi_modu_main.h"
#include "mmialarm_export.h"
#include "mmiidle_export.h"
#include "mmi_appmsg.h"
#include "mmistk_export.h"
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#include "guirichtext.h"
#endif
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmi_multi_variant.h"
#include "variant_config_nv.h"
#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
#include "mmismscb_export.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//time length

#define MMIIDLE_GET_STR_DISPLAY_LEN_OFFSET 12

#define IDLE_STK_TIMER_PERIOD       150 //idle下stk text显示的定时器周期

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL IDLE_STK_TEXT_INFO    s_idle_stk_text = {0};

LOCAL IDLE_WIN_INFO_T       s_idlewin_info; // Idle窗口的相关Information

LOCAL IDLE_PUSH_MSG_NODE_T *s_idle_push_msg_ptr_list[MMI_IDLE_PUSH_MSG_MAX_COUNT] = {PNULL};

/*
这个是锁屏界面拨打紧急号码显示的，
此功能对应的NV是NV_VARIANT_CONFIG_APP/EMERGENCY_CALL_UI，
0,1,2 UI 显示都是一样的，区别只是
0: No emergency call UI display，锁屏的时候只能打默认的紧急号码
1: Emergency call UI for China  锁屏的时候除了可以拨打紧急号码，
    还能呼出中国区常用的紧急号码，并且是通过正常呼叫的形式呼出
2: Emergency call UI for AU   锁屏的时候除了可以拨打紧急号码，
    还能呼出澳大利亚国家的紧急号码，通过标准紧急号码呼出

China:   以下四个号码 在lock screen 的时候，必须能拨出去，
通话按照普通通话处理（emergency service number），
不能 按照标准的emergency call 处理
110
119
999
120

这个需求的重点是在lock screen的时候能拨打出emergency call， 
对于中国来说， 需要可以拨打4个号码，这4个号码需要是走普通通话， 
对于澳大利亚，lock screen需要能拨出号码，000，112;  并且000，112 是要走 标准的emergency call。 

Emergency call numbers in Australia are 000 and 112. 
First one (000) is the main e-number but mobile phone users can also use 112 for emergency and call will be forwarded to 000.
*/

static const char* const s_emergency_call_china[] = 
{
    "110", "999", "120", "119"
};

static const char* const s_emergency_call_australia[] =
{
    "000", "112"
};


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
extern int32 SCI_MEMCMP(void * _BUF1, void * _BUF2, uint32 _SIZE);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef MMI_IDLE_STKTEXT_SUPPORT
/*****************************************************************************/
//  Description : draw stk set idle mode text
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DrawStkIdleModeText(uint32 block_id);
#endif
/*****************************************************************************/
//  Description : 获得idle上的文本风格
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void GetIdleTextStyle(
                            GUISTR_STYLE_T *style_ptr,
                            GUI_FONT_T     font,
                            GUI_COLOR_T    font_color
                            );

#ifdef PDA_UI_DROPDOWN_WIN
/*****************************************************************************/
//  Description :to handle STK Idle Text Win
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStkIdleTextWinMsg(
                                                                              MMI_WIN_ID_T win_id, 
                                                                              MMI_MESSAGE_ID_E msg_id, 
                                                                              DPARAM param
                                                                              );
#endif

#if defined MMI_SMART_IDLE_SUPPORT_LARGE_ICON || defined MMI_SMART_IDLE_SUPPORT_SMALL_ICON|| defined MMI_COMMON_IDLE_SUPPORT ||defined BAR_PHONE_WIDGET || defined MMI_MSTYLE_SUPPORT
#ifdef CALENDAR_SUPPORT 
/*****************************************************************************/
//  Description : display schedule
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIDLE_DisplaySchedule(const GUI_LCD_DEV_INFO *lcd_dev_ptr,uint8 direction, uint16 max_len, GUI_POINT_T  *dis_point,GUI_FONT_T font_type)
{
    BOOLEAN                 is_alloc = FALSE;
    uint8                   time_str[MMIIDLE_TIME_LEN] = {0};
    uint16                  time_wstr[MMIIDLE_TIME_LEN] = {0};
    uint16                  time_str_len = 0;   
    uint16                  event_id = 0;
    uint16                  schedule_wstr_len = 0;
    uint16                  schedule_full_wstr_len  = 0;
    uint16                  display_max_len = 0;
    MMI_STRING_T            name_str = {0};
    MMI_STRING_T            schedule_str = {0};
    MMI_STRING_T            schedule_event_str = {0};
    MMI_STRING_T            schedule_full_event_str = {0};
    MMIACC_SMART_EVENT_T*   schedule_event_ptr = PNULL;
    wchar*                  event_str_ptr = PNULL;
    wchar*                  full_str_ptr = PNULL;//[MMI_MAINSCREEN_RIGHT_MAX_PIXEL] = {0};
    uint16                  full_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    uint16                  event_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    BOOLEAN                 is_dis_suspension = FALSE;
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    MMI_STRING_T            full_str = {0};
    MMI_STRING_T            event_str = {0};
    //get schedule
    if (!MMIAPISCH_OpenEventToday(&event_id))
    {
        //今日无日程
        MMI_GetLabelTextByLang(TXT_IDLE_NOCALENDAR,&schedule_str);
    }
    else if (MMIAPISCH_IsAllPastEventToday(&event_id))
    {
        //今日无更多日程
        MMI_GetLabelTextByLang(TXT_IDLE_NOMORECALENDAR,&schedule_str);
    }
    else
    {
        //日程
        MMI_GetLabelTextByLang(TXT_IDLE_CALENDAR,&name_str);

        //get schedule
        schedule_event_ptr = (MMIACC_SMART_EVENT_T*)SCI_ALLOCA(sizeof(MMIACC_SMART_EVENT_T));
        //SCI_ASSERT(PNULL != schedule_event_ptr);
        if(PNULL == schedule_event_ptr)
        {
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_event_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_138_112_2_18_2_24_7_54,(uint8*)"");
            return 0;
        }
        SCI_MEMSET(schedule_event_ptr, 0, sizeof(MMIACC_SMART_EVENT_T) );
        MMIAPIALM_FormInteEventInfo(event_id,schedule_event_ptr);

        //timer
		MMIAPISET_FormatTimeStrByTime(
			schedule_event_ptr->event_fast_info.hour,
			schedule_event_ptr->event_fast_info.minute,
			time_str,
			MMIIDLE_TIME_LEN
			);

        time_str_len = strlen((char *)time_str);
        MMI_STRNTOWSTR(time_wstr,MMIIDLE_TIME_LEN,
            time_str,MMIIDLE_TIME_LEN,
            time_str_len);
        
        //get schedule string len
        schedule_str.wstr_len = name_str.wstr_len + time_str_len + 1 + 1;//两个空格
        
        //copy schedule string
        schedule_str.wstr_ptr = (wchar*)SCI_ALLOCA((schedule_str.wstr_len+1)*sizeof(wchar));
        if(PNULL == schedule_str.wstr_ptr )
        {
            SCI_FREE(schedule_event_ptr);
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_str.wstr_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_164_112_2_18_2_24_7_55,(uint8*)"");
            return 0;
        }
        
        SCI_MEMSET(schedule_str.wstr_ptr,0,((schedule_str.wstr_len+1)*sizeof(wchar)));
        
        schedule_event_str.wstr_len = schedule_event_ptr->event_content_info.w_length;
        schedule_event_str.wstr_ptr = (wchar*)SCI_ALLOCA((schedule_event_str.wstr_len+1)*sizeof(wchar));
        if(PNULL == schedule_event_str.wstr_ptr)
        {
            SCI_FREE(schedule_event_ptr);
            SCI_FREE(schedule_str.wstr_ptr);
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_event_str.wstr_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_176_112_2_18_2_24_7_56,(uint8*)"");
            return 0;
        }
        
        SCI_MEMSET(schedule_event_str.wstr_ptr,0,((schedule_event_str.wstr_len+1)*sizeof(wchar)));
        is_alloc = TRUE;
        if (MMI_DIRECTION_RIGHT==direction)
        {
            //copy time
            MMI_WSTRNCPY(schedule_str.wstr_ptr,schedule_str.wstr_len,
                time_wstr,time_str_len,
                time_str_len);
			schedule_str.wstr_ptr[time_str_len] = ' ';//CR165770
            //copy name
            MMI_WSTRNCPY(schedule_str.wstr_ptr+time_str_len + 1,schedule_str.wstr_len-time_str_len,
                name_str.wstr_ptr,name_str.wstr_len,
                name_str.wstr_len);
        }
        else
        {
            //copy name
            MMI_WSTRNCPY(schedule_str.wstr_ptr,schedule_str.wstr_len,
                name_str.wstr_ptr,name_str.wstr_len,
                name_str.wstr_len);
            schedule_str.wstr_ptr[name_str.wstr_len] = ' ';//cr165770
            //copy time
            MMI_WSTRNCPY(schedule_str.wstr_ptr+name_str.wstr_len + 1,schedule_str.wstr_len-name_str.wstr_len,
                time_wstr,MMIIDLE_TIME_LEN,
                time_str_len);
        }
        
	//	schedule_str.wstr_ptr[name_str.wstr_len+time_str_len + 1]= ' ';//cr173732
        MMI_WSTRNCPY(schedule_str.wstr_ptr+name_str.wstr_len+time_str_len + 1,schedule_str.wstr_len-name_str.wstr_len-time_str_len,L" ",1,1);/*lint !e64*///CR165770
        //copy event
        MMI_WSTRNCPY(schedule_event_str.wstr_ptr,schedule_event_str.wstr_len,
            schedule_event_ptr->event_content_info.w_content,schedule_event_ptr->event_content_info.w_length,
            schedule_event_ptr->event_content_info.w_length);

        event_str_ptr= (wchar*)SCI_ALLOCA(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);
        if(PNULL == event_str_ptr)
        {
            SCI_FREE(schedule_str.wstr_ptr);
            SCI_FREE(schedule_event_str.wstr_ptr);
            SCI_FREE(schedule_event_ptr);
          
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == event_str_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_219_112_2_18_2_24_7_57,(uint8*)"");
            return 0;
        }
        
        SCI_MEMSET( event_str_ptr, 0, sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);
        MMIIDLE_GetDisPlayString(schedule_event_str.wstr_ptr,schedule_event_str.wstr_len,max_len-display_max_len,font_type,event_str_ptr,&event_len);//MARYXIAO CR230653
        SCI_FREE(schedule_event_ptr);
        SCI_FREE(schedule_event_str.wstr_ptr);
    }
    //display string max len
    display_max_len = max_len;
    full_str_ptr= (wchar*)SCI_ALLOCA(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);
    //SCI_ASSERT(PNULL != full_str_ptr);
    if(PNULL == full_str_ptr)
    {
        if(PNULL != schedule_str.wstr_ptr)
        {
            SCI_FREE(schedule_str.wstr_ptr);
        }
       
        if(PNULL != event_str_ptr)
        {
            SCI_FREE(event_str_ptr);
        }
        
        //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == full_str_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_244_112_2_18_2_24_7_58,(uint8*)"");
        return 0;
    }
    
    if(event_str_ptr != PNULL)
    {
        event_str.wstr_ptr = event_str_ptr;
        event_str.wstr_len = event_len;
        schedule_full_event_str.wstr_len = schedule_str.wstr_len + event_str.wstr_len;//拼接
        schedule_full_event_str.wstr_ptr = (wchar*)SCI_ALLOCA((schedule_full_event_str.wstr_len+1)*sizeof(wchar));
        if(PNULL == schedule_full_event_str.wstr_ptr)
        {
            if(PNULL != schedule_str.wstr_ptr)
            {
                SCI_FREE(schedule_str.wstr_ptr);
            }
 
            SCI_FREE(event_str_ptr);
            
            SCI_FREE(full_str_ptr);
            
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_full_event_str.wstr_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_265_112_2_18_2_24_7_59,(uint8*)"");
            return 0;
        }
        SCI_MEMSET(schedule_full_event_str.wstr_ptr,0,((schedule_full_event_str.wstr_len+1)*sizeof(wchar)));
        MMI_WSTRNCPY(schedule_full_event_str.wstr_ptr,schedule_full_event_str.wstr_len - event_str.wstr_len,schedule_str.wstr_ptr,schedule_str.wstr_len,schedule_str.wstr_len);/*lint !e64*/
        MMI_WSTRNCPY(schedule_full_event_str.wstr_ptr + schedule_str.wstr_len,event_str.wstr_len,event_str.wstr_ptr,event_str.wstr_len,event_str.wstr_len);/*lint !e64*/
    }

    SCI_MEMSET( full_str_ptr, 0, sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);

    if(event_str_ptr != PNULL)
    {    
        is_dis_suspension = MMIIDLE_GetDisPlayString(schedule_full_event_str.wstr_ptr,schedule_full_event_str.wstr_len,display_max_len,font_type,full_str_ptr,&full_len);
    }
    else
    {
        is_dis_suspension = MMIIDLE_GetDisPlayString(schedule_str.wstr_ptr,schedule_str.wstr_len,display_max_len,font_type,full_str_ptr,&full_len);
    }
    MMIIDLE_GetIdleTextStyle(&text_style, font_type, MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_CALENDAR));

    //get right softkey text length
    full_str.wstr_ptr = full_str_ptr;
    full_str.wstr_len = full_len;
    schedule_full_wstr_len = GUISTR_GetStringWidth(&text_style,&full_str,text_state);
    schedule_wstr_len = GUISTR_GetStringWidth(&text_style,&schedule_str,text_state);
    //CR132520
   // if (!is_dis_suspension && MMI_DIRECTION_HMIDDLE == direction)
   // {
	if ((max_len > schedule_full_wstr_len)&&(MMI_DIRECTION_HMIDDLE == direction))
	{
		dis_point->x = (max_len - schedule_full_wstr_len)/2; 
	}
               
   // }
    
    //display string
    MMIIDLE_DisplayIdleText(
        (const GUI_LCD_DEV_INFO *)lcd_dev_ptr,
        (const GUI_POINT_T *)dis_point,
        font_type,
        MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_CALENDAR),
        full_str.wstr_ptr,
        full_str.wstr_len
        );

//     MMIIDLE_DisplayIdleText(
//         (const GUI_LCD_DEV_INFO *)lcd_dev_ptr,
//         (const GUI_POINT_T *)dis_point,
//         font_type,
//         MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_CALENDAR),
//         schedule_str.wstr_ptr,
//         schedule_str.wstr_len
//         );

//     if(event_str_ptr != PNULL)
//     {
//         dis_point->x = dis_point->x + schedule_wstr_len;
//         MMIIDLE_DisplayIdleText(
//             (const GUI_LCD_DEV_INFO *)lcd_dev_ptr,
//             (const GUI_POINT_T *)dis_point,
//             font_type,
//             MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_CALENDAR),
//             event_str_ptr,
//             event_len
//             );
//     }
	//end for NEWMS00222937
    
    if(PNULL!= event_str_ptr) 
    {   
        SCI_FREE(event_str_ptr);
    }

    SCI_FREE(full_str_ptr);
    
    //free memory
    if (is_alloc&&PNULL!= schedule_str.wstr_ptr)
    {
        SCI_FREE(schedule_str.wstr_ptr);
    }
    
    SCI_FREE(schedule_full_event_str.wstr_ptr);
    
    return schedule_wstr_len;    
   
}

PUBLIC uint16 MMIIDLE_DisplayScheduleEx(
                                              GUI_LCD_DEV_INFO *lcd_dev_ptr,
                                              GUI_RECT_T      *rect_ptr,
                                              GUI_FONT_T      font_type
                                             )

{
    BOOLEAN                 is_alloc = FALSE;
    uint8                   time_str[MMIIDLE_TIME_LEN] = {0};
    uint16                  time_wstr[MMIIDLE_TIME_LEN] = {0};
    uint16                  time_str_len = 0;   
    uint16                  event_id = 0;
    uint16                  display_max_len = 0;
    uint16                  max_len = MMITHEME_GetClientRectWidth();
    MMI_STRING_T            name_str = {0};
    MMI_STRING_T            schedule_str = {0};
    MMI_STRING_T            schedule_event_str = {0};
    MMI_STRING_T            schedule_full_event_str = {0};
    MMIACC_SMART_EVENT_T*   schedule_event_ptr = PNULL;
    wchar*                  event_str_ptr = PNULL;
    wchar*                  full_str_ptr = PNULL;//[MMI_MAINSCREEN_RIGHT_MAX_PIXEL] = {0};
    uint16                  full_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    uint16                  event_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    BOOLEAN                 is_dis_suspension = FALSE;
    MMI_STRING_T            event_str = {0};
    //MMI_DIRECTION_SET_E     direction = MMI_DIRECTION_LEFT;
    uint16                  wstr_width = 0;
    //get schedule
    if (!MMIAPISCH_OpenEventToday(&event_id))
    {
        //今日无日程
        MMI_GetLabelTextByLang(TXT_IDLE_NOCALENDAR,&schedule_str);
    }
    else if (MMIAPISCH_IsAllPastEventToday(&event_id))
    {
        //今日无更多日程
        MMI_GetLabelTextByLang(TXT_IDLE_NOMORECALENDAR,&schedule_str);
    }
    else
    {
        //日程
        MMI_GetLabelTextByLang(TXT_IDLE_CALENDAR,&name_str);

        //get schedule
        schedule_event_ptr = (MMIACC_SMART_EVENT_T*)SCI_ALLOCA(sizeof(MMIACC_SMART_EVENT_T));
        //SCI_ASSERT(PNULL != schedule_event_ptr);
        if(PNULL == schedule_event_ptr)
        {
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_event_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_138_112_2_18_2_24_7_54,(uint8*)"");
            return 0;
        }
        SCI_MEMSET(schedule_event_ptr, 0, sizeof(MMIACC_SMART_EVENT_T) );
        MMIAPIALM_FormInteEventInfo(event_id,schedule_event_ptr);

        //timer
		MMIAPISET_FormatTimeStrByTime(
			schedule_event_ptr->event_fast_info.hour,
			schedule_event_ptr->event_fast_info.minute,
			time_str,
			MMIIDLE_TIME_LEN
			);
        time_str_len = strlen((char *)time_str);
        MMI_STRNTOWSTR(time_wstr,MMIIDLE_TIME_LEN,
            time_str,MMIIDLE_TIME_LEN,
            time_str_len);

        //get schedule string len
        schedule_str.wstr_len = name_str.wstr_len + time_str_len +1;
        
        //copy schedule string
        schedule_str.wstr_ptr = (wchar*)SCI_ALLOCA((schedule_str.wstr_len+1)*sizeof(wchar));
        if(PNULL == schedule_str.wstr_ptr )
        {
            SCI_FREE(schedule_event_ptr);
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_str.wstr_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_164_112_2_18_2_24_7_55,(uint8*)"");
            return 0;
        }
        
        SCI_MEMSET(schedule_str.wstr_ptr,0,((schedule_str.wstr_len+1)*sizeof(wchar)));
        
        schedule_event_str.wstr_len = schedule_event_ptr->event_content_info.w_length;
        schedule_event_str.wstr_ptr = (wchar*)SCI_ALLOCA((schedule_event_str.wstr_len+1)*sizeof(wchar));
        if(PNULL == schedule_event_str.wstr_ptr)
        {
            SCI_FREE(schedule_event_ptr);
            SCI_FREE(schedule_str.wstr_ptr);
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_event_str.wstr_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_176_112_2_18_2_24_7_56,(uint8*)"");
            return 0;
        }
        
        SCI_MEMSET(schedule_event_str.wstr_ptr,0,((schedule_event_str.wstr_len+1)*sizeof(wchar)));
        is_alloc = TRUE;
        /*if (MMI_DIRECTION_RIGHT==direction)
        {
            //copy time
            MMI_WSTRNCPY(schedule_str.wstr_ptr,schedule_str.wstr_len,
                time_wstr,time_str_len,
                time_str_len);
            //copy name
            MMI_WSTRNCPY(schedule_str.wstr_ptr+time_str_len,schedule_str.wstr_len-time_str_len,
                name_str.wstr_ptr,name_str.wstr_len,
                name_str.wstr_len);
        }
        else*/
        {
            //copy name
            MMI_WSTRNCPY(schedule_str.wstr_ptr,schedule_str.wstr_len,
                name_str.wstr_ptr,name_str.wstr_len,
                name_str.wstr_len);

            //copy time
            MMI_WSTRNCPY(schedule_str.wstr_ptr+name_str.wstr_len,schedule_str.wstr_len-name_str.wstr_len,
                time_wstr,MMIIDLE_TIME_LEN,
                time_str_len);
        }

        MMI_WSTRNCPY(schedule_str.wstr_ptr+name_str.wstr_len+time_str_len,schedule_str.wstr_len-name_str.wstr_len-time_str_len,L" ",1,1);/*lint !e64*/
        //copy event
        MMI_WSTRNCPY(schedule_event_str.wstr_ptr,schedule_event_str.wstr_len,
            schedule_event_ptr->event_content_info.w_content,schedule_event_ptr->event_content_info.w_length,
            schedule_event_ptr->event_content_info.w_length);

        event_str_ptr= (wchar*)SCI_ALLOCA(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);
        if(PNULL == event_str_ptr)
        {
            SCI_FREE(schedule_str.wstr_ptr);
            SCI_FREE(schedule_event_str.wstr_ptr);
            SCI_FREE(schedule_event_ptr);
          
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == event_str_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_219_112_2_18_2_24_7_57,(uint8*)"");
            return 0;
        }
        
        SCI_MEMSET( event_str_ptr, 0, sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);
        MMIIDLE_GetDisPlayString(schedule_event_str.wstr_ptr,schedule_event_str.wstr_len,max_len-display_max_len,font_type,event_str_ptr,&event_len);//MARYXIAO CR230653
        SCI_FREE(schedule_event_ptr);
        SCI_FREE(schedule_event_str.wstr_ptr);
    }
    //display string max len
    display_max_len = max_len - rect_ptr->left;
    full_str_ptr= (wchar*)SCI_ALLOCA(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);
    //SCI_ASSERT(PNULL != full_str_ptr);
    if(PNULL == full_str_ptr)
    {
        if(PNULL != schedule_str.wstr_ptr)
        {
            SCI_FREE(schedule_str.wstr_ptr);
        }
       
        if(PNULL != event_str_ptr)
        {
            SCI_FREE(event_str_ptr);
        }
        
        //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == full_str_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_244_112_2_18_2_24_7_58,(uint8*)"");
        return 0;
    }
    
    if(event_str_ptr != PNULL)
    {
        event_str.wstr_ptr = event_str_ptr;
        event_str.wstr_len = event_len;
        schedule_full_event_str.wstr_len = schedule_str.wstr_len + event_str.wstr_len;//拼接
        schedule_full_event_str.wstr_ptr = (wchar*)SCI_ALLOCA((schedule_full_event_str.wstr_len+1)*sizeof(wchar));
        if(PNULL == schedule_full_event_str.wstr_ptr)
        {
            if(PNULL != schedule_str.wstr_ptr)
            {
                SCI_FREE(schedule_str.wstr_ptr);
            }
 
            SCI_FREE(event_str_ptr);
            
            SCI_FREE(full_str_ptr);
            
            //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_DisplaySchedule PNULL == schedule_full_event_str.wstr_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_265_112_2_18_2_24_7_59,(uint8*)"");
            return 0;
        }
        SCI_MEMSET(schedule_full_event_str.wstr_ptr,0,((schedule_full_event_str.wstr_len+1)*sizeof(wchar)));
        MMI_WSTRNCPY(schedule_full_event_str.wstr_ptr,schedule_full_event_str.wstr_len - event_str.wstr_len,schedule_str.wstr_ptr,schedule_str.wstr_len,schedule_str.wstr_len);/*lint !e64*/
        MMI_WSTRNCPY(schedule_full_event_str.wstr_ptr + schedule_str.wstr_len,event_str.wstr_len,event_str.wstr_ptr,event_str.wstr_len,event_str.wstr_len);/*lint !e64*/
    }

    SCI_MEMSET( full_str_ptr, 0, sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);

    if(event_str_ptr != PNULL)
    {    
        is_dis_suspension = MMIIDLE_GetDisPlayString(schedule_full_event_str.wstr_ptr,schedule_full_event_str.wstr_len,display_max_len,font_type,full_str_ptr,&full_len);
    }
    else
    {
        is_dis_suspension = MMIIDLE_GetDisPlayString(schedule_str.wstr_ptr,schedule_str.wstr_len,display_max_len,font_type,full_str_ptr,&full_len);
    }
        
    //display string
    wstr_width = MMIIDLE_DisplayLineTextInRect(
            lcd_dev_ptr,
            rect_ptr,
            font_type,
            MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_CALENDAR),
            full_str_ptr, 
            full_len
            );
    
//	    wstr_width = MMIIDLE_DisplayLineTextInRect(
//	            lcd_dev_ptr,
//	            rect_ptr,
//	            font_type,
//	            MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_CALENDAR),
//	            schedule_str.wstr_ptr, 
//	            schedule_str.wstr_len
//	            );
//	            
//	
//	    if(event_str_ptr != PNULL)
//	    {
//	        rect_ptr->left = rect_ptr->left + wstr_width;
//	 
//	        //display string
//	        wstr_width = MMIIDLE_DisplayLineTextInRect(
//	                lcd_dev_ptr,
//	                rect_ptr,
//	                font_type,
//	                MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_CALENDAR),
//	                event_str_ptr, 
//	                event_len
//	                );
//	    }
	//end for NEWMS00222937
    
    if(PNULL!= event_str_ptr) 
    {   
        SCI_FREE(event_str_ptr);
    }

    SCI_FREE(full_str_ptr);
    
    //free memory
    if (is_alloc&&PNULL!= schedule_str.wstr_ptr)
    {
        SCI_FREE(schedule_str.wstr_ptr);
    }
    
    SCI_FREE(schedule_full_event_str.wstr_ptr);
    
    return wstr_width;    
   
}
#endif

#ifdef MMI_SMSCB_CELL_INFO_SUPPORT 
/*****************************************************************************/
//  Description : display schedule
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIDLE_DisplayCellInfo(const GUI_LCD_DEV_INFO *lcd_dev_ptr,uint8 direction, uint16 max_len, GUI_POINT_T  *dis_point,GUI_FONT_T font_type)
{

    uint16                  display_max_len = 0;
    uint8 * p_cell_info = PNULL;
    BOOLEAN                 is_dis_suspension = FALSE;
    uint16                  cell_info_wstr_len = 0;
    MMI_STRING_T            cell_info_str = {0};
    wchar*                  full_str_ptr = PNULL;
    uint16                  full_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    MMIIDLE_GetIdleTextStyle(&text_style, font_type, MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_CALENDAR));
    p_cell_info = MMIAPISMSCB_GetCellInfoStr();
    if(PNULL == p_cell_info)
    {
        SCI_TRACE_LOW("MMIIDLE_DisplayCellInfo alloc memory failed!");
        return;
    }
    cell_info_str.wstr_len = SCI_STRLEN(p_cell_info);
    cell_info_str.wstr_ptr = (wchar*)SCI_ALLOCA((cell_info_str.wstr_len+1)*sizeof(wchar));
    if(PNULL == cell_info_str.wstr_ptr)
    {
        SCI_TRACE_LOW("MMIIDLE_DisplayCellInfo alloc memory failed!");
        return;
    }
    cell_info_str.wstr_ptr = MMIAPICOM_StrToWstr(p_cell_info, cell_info_str.wstr_ptr);

    full_str_ptr= (wchar*)SCI_ALLOCA(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);
    is_dis_suspension = MMIIDLE_GetDisPlayString(cell_info_str.wstr_ptr,cell_info_str.wstr_len,display_max_len,font_type,full_str_ptr,&full_len);
    MMIIDLE_GetIdleTextStyle(&text_style, font_type, MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_CALENDAR));

    cell_info_wstr_len = GUISTR_GetStringWidth(&text_style,&cell_info_str,text_state);

    //display string
    MMIIDLE_DisplayIdleText(
        (const GUI_LCD_DEV_INFO *)lcd_dev_ptr,
        (const GUI_POINT_T *)dis_point,
        font_type,
        MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_CALENDAR),
        cell_info_str.wstr_ptr,
        cell_info_str.wstr_len
        );

    if(PNULL != cell_info_str.wstr_ptr)
    {
        SCI_FREE(cell_info_str.wstr_ptr);
    }
    if(PNULL != full_str_ptr)
    {
        SCI_FREE(full_str_ptr);
    }
        
    return cell_info_wstr_len;    
}
#endif

/*****************************************************************************/
//  Description : 获得idle上的文本风格
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_GetIdleTextStyle(
                            GUISTR_STYLE_T *style_ptr,
                            GUI_FONT_T     font,
                            GUI_COLOR_T    font_color
                            )
{
    if(PNULL == style_ptr)
    {
        //SCI_TRACE_LOW:"GetIdleTextStyle ERROR! PNULL == style_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_352_112_2_18_2_24_7_60,(uint8*)"");
        return;
    }
    style_ptr->effect = FONT_EFFECT_REVERSE_EDGE; 
    style_ptr->angle = ANGLE_0;
    style_ptr->align = ALIGN_LEFT;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
	style_ptr->edge_alpha = 0xff;
}

/*****************************************************************************/
//  Description : display string
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIDLE_DisplayIdleText(
                             const GUI_LCD_DEV_INFO *lcd_dev_ptr,
                             const GUI_POINT_T      *point_ptr,
                             GUI_FONT_T             font,
                             GUI_COLOR_T            font_color,
                             wchar                  *wstr_ptr,
                             uint16                 wstr_len
                             )
{
    GUISTR_STYLE_T  text_style = {0};/*lint !e64*/
    MMI_STRING_T    text = {0};
    GUISTR_STATE_T  text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    GUI_RECT_T      disp_rect = {0};
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint16          str_pixel_width = 0;

    GUILCD_GetLogicWidthHeight(lcd_dev_ptr->lcd_id, &lcd_width, &lcd_height);

    MMIIDLE_GetIdleTextStyle(&text_style, font, font_color);

    disp_rect.left = point_ptr->x;
    disp_rect.top = point_ptr->y;
    disp_rect.right = lcd_width - 1;
    disp_rect.bottom  = disp_rect.top + GUIFONT_GetHeight(text_style.font) +1;
    
    text.wstr_len = wstr_len;
    text.wstr_ptr = wstr_ptr;
    //display date
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)lcd_dev_ptr,
        (const GUI_RECT_T      *)&disp_rect,
        (const GUI_RECT_T      *)&disp_rect,
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );

    str_pixel_width = GUISTR_GetStringWidth(&text_style,&text,text_state);

    return (str_pixel_width);
}
#if defined MMI_SMART_IDLE_SUPPORT_LARGE_ICON || defined MMI_SMART_IDLE_SUPPORT_SMALL_ICON
/*****************************************************************************/
//  Description : display string
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIDLE_DisplayIdleTextEx(
                             const GUI_LCD_DEV_INFO *lcd_dev_ptr,
                             const GUI_POINT_T      *point_ptr,
                             GUI_FONT_T             font,
                             GUI_COLOR_T            font_color,
                             wchar                  *wstr_ptr,
                             uint16                 wstr_len,
                             GUISTR_STATE_T  text_state
                             )
{
    GUISTR_STYLE_T  text_style = {0};/*lint !e64*/
    MMI_STRING_T    text = {0};
    GUI_RECT_T      disp_rect = {0};
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint16          str_pixel_width = 0;

    GUILCD_GetLogicWidthHeight(lcd_dev_ptr->lcd_id, &lcd_width, &lcd_height);

    GetIdleTextStyle(&text_style, font, font_color);

    disp_rect.left = point_ptr->x;
    disp_rect.top = point_ptr->y;
    disp_rect.right = lcd_width - 1;
    disp_rect.bottom  = disp_rect.top + GUIFONT_GetHeight(text_style.font) +1;
    
    text.wstr_len = wstr_len;
    text.wstr_ptr = wstr_ptr;
    //display date
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)lcd_dev_ptr,
        (const GUI_RECT_T      *)&disp_rect,
        (const GUI_RECT_T      *)&disp_rect,
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );

    str_pixel_width = GUISTR_GetStringWidth(&text_style,&text,text_state);

    return (str_pixel_width);
}
#endif
/*****************************************************************************/
//  Description : get display string
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_GetDisPlayString(wchar const * src_str,     //in
                            uint16  src_len,     //in
                            uint16  max_display_len,//in
                            GUI_FONT_T font_type, //font
                            wchar * des_str,     //out
                            uint16* des_len      //out
                            )
{    
    uint16                  wstr_len= 0;
    uint8                   temp_str [MMIIDLE_TIME_LEN] = {0};
    wchar                   temp_wstr[MMIIDLE_TIME_LEN] = {0};
    uint16                  temp_str_len = 0;
    uint16                  temp_wstr_width = 0;
    BOOLEAN                 is_dis_suspension = FALSE;
    uint16                  src_str_len = src_len;
    uint16                  max_des_len = *des_len;

    SCI_MEMSET(des_str,0x00,(sizeof(wchar)*(*des_len)));

    //get suspension points
    sprintf((char *)temp_str,"...");
    temp_str_len = strlen((char *)temp_str);
    MMI_STRNTOWSTR(temp_wstr,MMIIDLE_TIME_LEN,
        temp_str,MMIIDLE_TIME_LEN,
        temp_str_len);

    temp_wstr_width = MMIAPICOM_GetStrWidth(temp_wstr,
                &(temp_str_len),
                0,
                max_display_len,
                font_type);
    wstr_len = src_str_len;
    MMIAPICOM_GetStrWidth(src_str,
                    &(src_str_len),
                    0,
                    max_display_len,
                    font_type);

    //超过一行时,末尾显示"..."的处理
    if (wstr_len > src_str_len)
    {
        is_dis_suspension = TRUE;
        //获得去除"..."后的显示宽度
        MMIAPICOM_GetStrWidth(src_str,
                    &(src_str_len),
                    0,
                    //这个函数计算不是很准确，为了避免显示不全，只有将显示区域减小一点了。
                    (max_display_len - temp_wstr_width-MMIIDLE_GET_STR_DISPLAY_LEN_OFFSET),
                    font_type);
    }
    MMI_WSTRNCPY(des_str,*des_len,src_str,src_str_len,src_str_len);

    *des_len = src_str_len;
    if (is_dis_suspension)
    {
        MMI_WSTRNCPY(des_str+src_str_len,max_des_len-src_str_len,temp_wstr,temp_str_len,temp_str_len);
        *des_len +=temp_str_len;
    }
    return is_dis_suspension;
}

PUBLIC uint16 MMIIDLE_DisplayLineTextInRect(
                              GUI_LCD_DEV_INFO *lcd_dev_ptr,
                              GUI_RECT_T      *rect_ptr,
                             GUI_FONT_T             font,
                             GUI_COLOR_T            font_color,
                             wchar                  *wstr_ptr,
                             uint16                 wstr_len
                             )
{
    GUISTR_STYLE_T  text_style = {0};/*lint !e64*/
    MMI_STRING_T    text = {0};
    GUISTR_STATE_T  text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    GUI_RECT_T      disp_rect = *rect_ptr;
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint16          str_pixel_width = 0;

    GUILCD_GetLogicWidthHeight(lcd_dev_ptr->lcd_id, &lcd_width, &lcd_height);

    MMIIDLE_GetIdleTextStyle(&text_style, font, font_color);
    text_style.align = ALIGN_LVMIDDLE;

    text.wstr_len = wstr_len;
    text.wstr_ptr = wstr_ptr;
    //display date
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)lcd_dev_ptr,
        (const GUI_RECT_T      *)&disp_rect,
        (const GUI_RECT_T      *)&disp_rect,
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );

    str_pixel_width = GUISTR_GetStringWidth(&text_style,&text,text_state);

    return (str_pixel_width);
}

#endif //defined MMI_SMART_IDLE_SUPPORT || defined MMI_COMMON_IDLE_SUPPORT
/*****************************************************************************/
//  Description : init win Info
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_InitWinInfo(void)
{
    SCI_MEMSET(((void*)&s_idlewin_info), 0, sizeof(IDLE_WIN_INFO_T));
}
/*****************************************************************************/
//  Description : get idle info
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC IDLE_WIN_INFO_T* MMIIDLE_GetIdleWinInfo(void)
{
    return &s_idlewin_info;
}
/*****************************************************************************/
//  Description : display the main and sub idle soft icon 
//  Global resource dependence : 
//  Author: Great.Tian
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_SendMsgToIdleWin(
                               MMI_MESSAGE_ID_E msg_id,
                               DPARAM            param_ptr
                               )
{
    if (MMIAPIIDLE_IdleWinIsOpen() && MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
    {
        return MMK_SendMsg(MAIN_IDLE_WIN_ID, msg_id, param_ptr);
    }
    else
    {
        return (FALSE);
    }
}
/*****************************************************************************/
//  Description : need show soft icon and network name according to setting
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting(MN_DUAL_SYS_E dual_sys)
{
    MMISET_DUALSYS_TYPE_E e_dualsys_set = MMIAPISET_GetMultiSysSetting();

    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //SCI_TRACE_LOW:"NeedShowIconAccordingToDualSysSetting ERROR! MMI_DUAL_SYS_MAX <= dual_sys,dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_567_112_2_18_2_24_7_61,(uint8*)"d",dual_sys);
        return FALSE;
    }
    if (e_dualsys_set & (0x01 << dual_sys))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : 获得idle上的文本风格
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void GetIdleTextStyle(
                            GUISTR_STYLE_T *style_ptr,
                            GUI_FONT_T     font,
                            GUI_COLOR_T    font_color
                            )
{
    if(PNULL == style_ptr)
    {
        //SCI_TRACE_LOW:"GetIdleTextStyle ERROR! PNULL == style_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_592_112_2_18_2_24_8_62,(uint8*)"");
        return;
    }

    style_ptr->effect = FONT_EFFECT_REVERSE_EDGE; 
    style_ptr->angle = ANGLE_0;
    style_ptr->align = ALIGN_LEFT;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
}
#ifdef MMI_IDLE_STKTEXT_SUPPORT
/*****************************************************************************/
//  Description : draw stk set idle mode text
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MAIN_DrawStkIdleModeText(uint32 block_id)
{
    return DrawStkIdleModeText(block_id);
}

/*****************************************************************************/
//  Description : draw stk set idle mode text
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DrawStkIdleModeText(uint32 block_id)
{
    MMI_STRING_T idle_text = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_IDLE_DISPLAY_T idle_disp_style = {0};
    GUI_COLOR_T text_color = 0;
    GUI_FONT_T text_font = SONG_FONT_8;
    GUISTR_STYLE_T text_style = {0};/*lint !e64*/
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT;
    GUI_RECT_T disp_rect = {0};
    GUI_POINT_T point = {0};
    BOOLEAN is_display = FALSE;
    MMISET_IDLE_STYLE_E     idle_style  = MMISET_IDLE_STYLE_COMMON;
    BOOLEAN is_icon_exist = FALSE;
    GUIIMG_BITMAP_T *icon_bmp_ptr = PNULL;
    BOOLEAN is_self_explanatory = FALSE;
#ifdef WIN32
	wchar test_stk[] ={'a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h','\0'};
    MMI_STRING_T test_stk_str ={0};
	s_idle_stk_text.stk_idle_str.wstr_ptr = test_stk;
	s_idle_stk_text.stk_idle_str.wstr_len = 168;
	s_idlewin_info.is_display_stk_idle_text = TRUE;
	s_idle_stk_text.is_scroll = TRUE;
#endif
    idle_style = MMIAPISET_GetIdleStyle();

    lcd_dev_info.block_id = block_id;

    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_STK_IDLE_MODE);

    text_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_STK);
    text_font = idle_disp_style.text_font;

    disp_rect.left = idle_disp_style.text_frame_x;
    disp_rect.top = idle_disp_style.text_frame_top;
    disp_rect.right = disp_rect.left + MMI_MAINSCREEN_IDLE_STK_WIDTH - 1;
    disp_rect.bottom = disp_rect.top + GUI_GetFontHeight(text_font, UNICODE_HANZI)+1 ;

    point.x = idle_disp_style.text_frame_x;
    point.y = idle_disp_style.text_frame_top;

    if (s_idlewin_info.is_display_stk_idle_text
        && PNULL != s_idle_stk_text.stk_idle_str.wstr_ptr
        && 0 != s_idle_stk_text.stk_idle_str.wstr_len)
    {
        //display icon
        icon_bmp_ptr = (GUIIMG_BITMAP_T *)MMIAPISTK_GetIdleTextIcon(&is_icon_exist, &is_self_explanatory);

        if (is_icon_exist && PNULL != icon_bmp_ptr && PNULL != icon_bmp_ptr->bit_ptr)//??Icon
        {
            GUI_POINT_T dis_point = {0};
            GUI_RECT_T show_rect = {0};

            if(point.x < (icon_bmp_ptr->img_width + MMI_IDLE_SIM_ICON_INTERVAL))
            {
                point.x = icon_bmp_ptr->img_height + MMI_IDLE_SIM_ICON_INTERVAL;
            }

            dis_point.x = point.x - icon_bmp_ptr->img_width;
            dis_point.y = point.y;

            show_rect.left = dis_point.x;
            show_rect.top = dis_point.y;
            show_rect.right = dis_point.x + icon_bmp_ptr->img_width;
            show_rect.bottom = dis_point.y + icon_bmp_ptr->img_height;

            GUIIMG_DisplayBmp(FALSE, &show_rect, &dis_point, icon_bmp_ptr, &lcd_dev_info);
        }

        if (!is_icon_exist || !is_self_explanatory)
        {
            if (s_idle_stk_text.str_index > s_idle_stk_text.stk_idle_str.wstr_len - 1)
            {
                s_idle_stk_text.str_index = 0;
            }

            idle_text.wstr_ptr = s_idle_stk_text.stk_idle_str.wstr_ptr + s_idle_stk_text.str_index;
            idle_text.wstr_len = s_idle_stk_text.stk_idle_str.wstr_len - s_idle_stk_text.str_index;
            
            GetIdleTextStyle(&text_style, text_font, text_color);

            if (s_idle_stk_text.is_scroll)
            {
                MAIN_StopIdleSTKTextTimer();
                MAIN_StartIdleSTKTextTimer();
            }
            else
            {
                text_style.align = ALIGN_HMIDDLE;
            }

            GUISTR_DrawTextToLCDInRect( 
                                                            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                                            (const GUI_RECT_T *)&disp_rect,
                                                            (const GUI_RECT_T *)&disp_rect,
                                                            (const MMI_STRING_T *)&idle_text,
                                                            &text_style,
                                                            text_state,
                                                            GUISTR_TEXT_DIR_AUTO
                                                            );

            if (s_idle_stk_text.is_scroll && s_idle_stk_text.str_index < s_idle_stk_text.stk_idle_str.wstr_len - 1)
            {
                s_idle_stk_text.str_index ++;
            }
            else
            {
                s_idle_stk_text.str_index = 0;
            }
        }

        is_display = TRUE;
    }

    return is_display;
}

/*****************************************************************************/
//  Description : start stk idle display timer
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MAIN_StartIdleSTKTextTimer(void)
{
    BOOLEAN result = FALSE;

    if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
        if ((0 == s_idle_stk_text.idle_display_timer) &&
            (s_idle_stk_text.is_scroll))
        {
            s_idle_stk_text.idle_display_timer = MMK_CreateWinTimer(MAIN_IDLE_WIN_ID, IDLE_STK_TIMER_PERIOD, FALSE);

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : stop stk idle display timer
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MAIN_StopIdleSTKTextTimer(void)
{
    BOOLEAN result = FALSE;

    if (0 != s_idle_stk_text.idle_display_timer)
    {
        MMK_StopTimer(s_idle_stk_text.idle_display_timer);
        s_idle_stk_text.idle_display_timer = 0;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get stk text info
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC IDLE_STK_TEXT_INFO* MMIIDLECOM_GetStkTextInfo(void)
{
    return &s_idle_stk_text;
}

/*****************************************************************************/
//  Description : draw stk set idle mode text
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_MAIN_SetIdleModeText(MN_DUAL_SYS_E dual_sys, MMI_STRING_T *stk_idle_str)
{
    MMI_IDLE_DISPLAY_T idle_disp_style = {0};
    
    if (MMI_DUAL_SYS_MAX<=dual_sys || PNULL == stk_idle_str)
    {
        //SCI_TRACE_LOW:"MMIAPIIDLE_MAIN_SetIdleModeText ERROR! dual_sys =%d, PNULL == stk_idle_str"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_785_112_2_18_2_24_8_63,(uint8*)"d",dual_sys);
        return;
    }
    //SCI_TRACE_LOW:"MMIAPIIDLE_MAIN_SetIdleModeText dual_sys=%d, len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_FUNC_788_112_2_18_2_24_8_64,(uint8*)"dd", dual_sys, stk_idle_str->wstr_len);

    MAIN_StopIdleSTKTextTimer();

    MMIIDLE_GetIdleWinInfo()->is_display_stk_idle_text = FALSE;
#ifdef PDA_UI_DROPDOWN_WIN
    MMIDROPDOWNWIN_DelNotifyRecord(DROPDOWN_NOTIFY_STK_IDLE_TEXT);
#endif

    s_idle_stk_text.is_scroll = FALSE;
    s_idle_stk_text.str_index = 0;

    //free memory
    if (PNULL != s_idle_stk_text.stk_idle_str.wstr_ptr)
    {
        SCI_FREE(s_idle_stk_text.stk_idle_str.wstr_ptr);
        s_idle_stk_text.stk_idle_str.wstr_ptr = PNULL;
    }

    if (0 != stk_idle_str->wstr_len)
    {
#ifdef PDA_UI_DROPDOWN_WIN
        MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_STK_IDLE_TEXT);
#endif

        s_idle_stk_text.dual_sys = dual_sys;
        MMIIDLE_GetIdleWinInfo()->is_display_stk_idle_text = TRUE;
        s_idle_stk_text.stk_idle_str.wstr_len = stk_idle_str->wstr_len;

        s_idle_stk_text.stk_idle_str.wstr_ptr = SCI_ALLOC_APP((s_idle_stk_text.stk_idle_str.wstr_len + 1) * sizeof(wchar));
        SCI_MEMSET(s_idle_stk_text.stk_idle_str.wstr_ptr, 0x00, ((s_idle_stk_text.stk_idle_str.wstr_len + 1) * sizeof(wchar)));
        MMI_WSTRNCPY(s_idle_stk_text.stk_idle_str.wstr_ptr,
                                s_idle_stk_text.stk_idle_str.wstr_len,
                                stk_idle_str->wstr_ptr,
                                stk_idle_str->wstr_len,
                                stk_idle_str->wstr_len);

        idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_STK_IDLE_MODE);

        s_idle_stk_text.str_width = GUI_CalculateStringPiexlNum(s_idle_stk_text.stk_idle_str.wstr_ptr,
                                                                                            s_idle_stk_text.stk_idle_str.wstr_len,
                                                                                            idle_disp_style.text_font,
                                                                                            0);

        if (MMI_MAINSCREEN_IDLE_STK_WIDTH < s_idle_stk_text.str_width)
        {
            s_idle_stk_text.is_scroll = TRUE;
        }
        else
        {
            s_idle_stk_text.is_scroll = FALSE;
        }
    }

    //inform idle win display text
    if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
    {
        MMK_PostMsg(MAIN_IDLE_WIN_ID, MSG_STK_SET_UP_IDLE_MODE, PNULL, 0);
    }
}
#endif
#ifdef PDA_UI_DROPDOWN_WIN
/*****************************************************************************/
//  Description :to handle STK Idle Text Win
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStkIdleTextWinMsg(
                                                                              MMI_WIN_ID_T win_id, 
                                                                              MMI_MESSAGE_ID_E msg_id, 
                                                                              DPARAM param
                                                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIIDLE_STK_IDLE_TEXT_CTRL_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_INVISIBLE, FALSE);//not return
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);    
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef MMI_PDA_SUPPORT
        {
            uint32 src_id = ((MMI_NOTIFY_T *)param)->src_id;

            if (MSG_CTL_PENOK == msg_id
                && MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == src_id)
            {
                MMK_CloseWin(win_id);

                break;
            }
        }
#endif
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description :Create Stk Idle Text Win
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_CreateStkIdleTextWin(void)
{
    MMI_STRING_T title = {0};
    GUIRICHTEXT_ITEM_T  item_data = {0};/*lint !e64*/
    uint16 index = 0;
    MMI_WINDOW_CREATE_T win_create = {0};
    MMI_CONTROL_CREATE_T create = {0};
    GUIRICHTEXT_INIT_DATA_T init_data = {0};
    //void* ctrl_ptr = PNULL;
    MMI_HANDLE_T win_id = MMIIDLE_STK_IDLE_TEXT_WIN_ID;
    BOOLEAN is_exist_stk_name = FALSE;
    BOOLEAN is_icon_exist = FALSE;
    GUIIMG_BITMAP_T *icon_bmp_ptr = PNULL;
    BOOLEAN is_self_explanatory = FALSE;

    MMK_CloseWin(win_id);

    win_create.applet_handle = MMK_GetFirstAppletHandle();
    win_create.win_id = win_id;
    win_create.func = HandleStkIdleTextWinMsg;
    win_create.win_priority = WIN_ONE_LEVEL;
    
    MMK_CreateWindow(&win_create);

#ifdef MMI_PDA_SUPPORT
    GUIWIN_CreateStbDyn(win_id);
#endif

    {
        //create softkey
        GUISOFTKEY_INIT_DATA_T softkey_data = {0};
        MMI_CONTROL_CREATE_T softkey_create = {0};
        MMI_CTRL_ID_T softkey_ctrl_id = 0;

#ifdef MMI_PDA_SUPPORT
        softkey_data.leftsoft_id = TXT_NULL;
        softkey_data.midsoft_id = STXT_EXIT;
        softkey_data.rightsoft_id = TXT_NULL;
#else
        softkey_data.leftsoft_id = TXT_NULL;
        softkey_data.midsoft_id = TXT_NULL;
        softkey_data.rightsoft_id = STXT_RETURN;
#endif
        softkey_ctrl_id = MMITHEME_GetSoftkeyCtrlId();
        softkey_create.ctrl_id = softkey_ctrl_id;
        softkey_create.guid    = SPRD_GUI_SOFTKEY_ID;
        softkey_create.parent_win_handle = win_id;
        softkey_create.init_data_ptr = &softkey_data;

#ifdef MMI_PDA_SUPPORT
        MMK_SetWinDisplayStyleState( win_id, WS_HAS_BUTTON_SOFTKEY | WS_DISABLE_SOFTKEY_TO_TITLE, TRUE );
#endif

        MMK_CreateControl( &softkey_create );
        MMK_SetWinSoftkeyCtrlId(win_id, softkey_ctrl_id);
    } 
    
    //set title
    GUIWIN_CreateTitleDyna(win_id, TXT_NULL);

    init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);

    create.ctrl_id = MMIIDLE_STK_IDLE_TEXT_CTRL_ID;
    create.guid = SPRD_GUI_RICHTEXT_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &init_data;
    //ctrl_ptr = 
    MMK_CreateControl( &create );

    //Set title
    is_exist_stk_name = MMIAPISTK_GetStkName(s_idle_stk_text.dual_sys, &title);

    if (!is_exist_stk_name)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_MAIN_STK, &title);
    }
    
    GUIWIN_SetTitleText(win_id, title.wstr_ptr, title.wstr_len, FALSE);


    icon_bmp_ptr = (GUIIMG_BITMAP_T *)MMIAPISTK_GetIdleTextIcon(&is_icon_exist, &is_self_explanatory);

    if (is_icon_exist && PNULL != icon_bmp_ptr && PNULL != icon_bmp_ptr->bit_ptr)//Icon
    {
        item_data.text_type = GUIRICHTEXT_TEXT_NONE;
        item_data.img_type = GUIRICHTEXT_IMAGE_BUF;
        item_data.is_bit_map = TRUE;
        item_data.img_data.buf.buf_ptr = (uint8 *)icon_bmp_ptr->bit_ptr;
        item_data.img_data.buf.size = icon_bmp_ptr->img_width * icon_bmp_ptr->img_height * 2;
        item_data.img_data.buf.width = icon_bmp_ptr->img_width;
        item_data.img_data.buf.height = icon_bmp_ptr->img_height;
        item_data.img_data.buf.height = icon_bmp_ptr->img_height;
        item_data.img_set_size = TRUE;
        item_data.img_setting_width = icon_bmp_ptr->img_width;
        item_data.img_setting_height = icon_bmp_ptr->img_height;
        item_data.img_set_align = TRUE;
        item_data.img_setting_align = GUIRICHTEXT_IMG_ALIGN_MIDDLE;

        GUIRICHTEXT_AddItem(
            MMIIDLE_STK_IDLE_TEXT_CTRL_ID,
            &item_data,
            &index
            );
    }

    if (!is_icon_exist || !is_self_explanatory)
    {
        item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;
        item_data.text_data.buf.str_ptr = s_idle_stk_text.stk_idle_str.wstr_ptr;
        item_data.text_data.buf.len = s_idle_stk_text.stk_idle_str.wstr_len;

        GUIRICHTEXT_AddItem(
            MMIIDLE_STK_IDLE_TEXT_CTRL_ID,
            &item_data,
            &index
            );
    }

    return TRUE;
}
#endif





/*****************************************************************************/
//     Description : save push msg info to nv
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
LOCAL void SavePushMsgInfo(void)
{
    int i = 0;
    IDLE_PUSH_MSG_NV_T *idle_push_msg_nv_ptr = PNULL;

    do
    {
        idle_push_msg_nv_ptr = (IDLE_PUSH_MSG_NV_T *)SCI_ALLOC_APP(sizeof(IDLE_PUSH_MSG_NV_T));
        if(PNULL == idle_push_msg_nv_ptr)
        {
            break;
        }
        SCI_MEMSET(idle_push_msg_nv_ptr, 0x0, sizeof(IDLE_PUSH_MSG_NV_T));

        for(i = 0; i < MMI_IDLE_PUSH_MSG_MAX_COUNT; i++)
        {
            int push_msg_info_size = sizeof(IDLE_PUSH_MSG_NODE_T);
            
            if(PNULL == s_idle_push_msg_ptr_list[i] 
                ||MMI_IDLE_PUSH_MSG_STATE_VALID != s_idle_push_msg_ptr_list[i]->msg_state)
            {
                break;
            }
            
            SCI_MEMCPY(&idle_push_msg_nv_ptr->idle_push_msg_info[i], 
                s_idle_push_msg_ptr_list[i],
                push_msg_info_size); 
        }

        MMI_WriteNVItem(MMINV_IDLE_PUSH_NOTIFICATION, idle_push_msg_nv_ptr);
    }while(0);

    if(PNULL != idle_push_msg_nv_ptr)
    {
        SCI_FREE(idle_push_msg_nv_ptr);
        idle_push_msg_nv_ptr = PNULL;
    }

    if(MMIIDLE_HaveNewNotification(MMI_IDLE_PUSH_MSG_WNS))
    {
        MMIAPICOM_StatusAreaSetAppNoticeState(TRUE);
    }
    else
    {
        MMIAPICOM_StatusAreaSetAppNoticeState(FALSE);
    }
}


/*****************************************************************************/
//  Description : 获取通知对应的图标。
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FreePushMsgNode(IDLE_PUSH_MSG_NODE_T *free_push_msg_node_ptr)
{
    int i = 0;
    BOOLEAN b_isfind = FALSE;
    IDLE_PUSH_MSG_NODE_T *tmp_push_msg_node_ptr = PNULL;

    if(PNULL == free_push_msg_node_ptr)
    {
        SCI_TRACE_LOW("[idle]:FreePushMsgNode(): para error.");
        return FALSE;
    }

    for(i = 0; i < MMI_IDLE_PUSH_MSG_MAX_COUNT; i++)
    {
        tmp_push_msg_node_ptr = s_idle_push_msg_ptr_list[i];
        if(free_push_msg_node_ptr == tmp_push_msg_node_ptr)
        {
            b_isfind = TRUE;
            break;
        }
    }

    if(b_isfind)
    {
        for(; i < MMI_IDLE_PUSH_MSG_MAX_COUNT - 1; i++)
        {
            s_idle_push_msg_ptr_list[i] = s_idle_push_msg_ptr_list[i + 1];
        }
        s_idle_push_msg_ptr_list[i] = PNULL;

        SCI_FREE(free_push_msg_node_ptr);
        free_push_msg_node_ptr = PNULL;

        SavePushMsgInfo();
        
        if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
        {
            MMK_SendMsg(MAIN_IDLE_WIN_ID, MSG_IDLE_UPDATE_NOTIFICATION_LIST, PNULL);
        }
        return TRUE;
    }
    else
    {
        SCI_TRACE_LOW("[idle]:FreePushMsgNode(): error: push msg node not found.");
        return FALSE;
    }

}


/*****************************************************************************/
//  Description : 获取通知对应的图标。
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
LOCAL IDLE_PUSH_MSG_NODE_T *GetNewMsgNode(IDLE_PUSH_MSG_INFO_T *push_msg_info_ptr)
{
    IDLE_PUSH_MSG_NODE_T *tmp_push_msg_node_ptr = PNULL;
    int i = 0;

    for(i = 0; i < MMI_IDLE_PUSH_MSG_MAX_COUNT; i++)
    {
        if(PNULL == s_idle_push_msg_ptr_list[i] 
           || MMI_IDLE_PUSH_MSG_STATE_VALID != s_idle_push_msg_ptr_list[i]->msg_state)
        {
            break;
        }
    }

    if(MMI_IDLE_PUSH_MSG_MAX_COUNT == i)
    {
        //消息已满，将最旧的消息覆盖。
        tmp_push_msg_node_ptr = s_idle_push_msg_ptr_list[0];
        for(i = 0; i < MMI_IDLE_PUSH_MSG_MAX_COUNT - 1; i++)
        {
            s_idle_push_msg_ptr_list[i] = s_idle_push_msg_ptr_list[i + 1];
        }

        //判断最后一条是否是ongoing call，
        //ongoing消息优先级最高，显示在Idle最下面
        if(MMI_IDLE_PUSH_MSG_ONGOING_CALL == s_idle_push_msg_ptr_list[MMI_IDLE_PUSH_MSG_MAX_COUNT - 1]->idle_push_msg_info.e_push_msg_type)
        {
            //新消息放在倒数第二项
            s_idle_push_msg_ptr_list[MMI_IDLE_PUSH_MSG_MAX_COUNT - 2] = tmp_push_msg_node_ptr;
        }
        else
        {
            //新消息放在最后一项
            s_idle_push_msg_ptr_list[MMI_IDLE_PUSH_MSG_MAX_COUNT - 1] = tmp_push_msg_node_ptr;
        }
        
        SCI_MEMSET(tmp_push_msg_node_ptr, 0x0, sizeof(IDLE_PUSH_MSG_NODE_T));
    }
    else
    {
        tmp_push_msg_node_ptr = (IDLE_PUSH_MSG_NODE_T *)SCI_ALLOC_APP(sizeof(IDLE_PUSH_MSG_NODE_T));
        if(PNULL != tmp_push_msg_node_ptr)
        {
            SCI_MEMSET(tmp_push_msg_node_ptr, 0x0, sizeof(IDLE_PUSH_MSG_NODE_T));
            if(i > 0 && 
                MMI_IDLE_PUSH_MSG_ONGOING_CALL == s_idle_push_msg_ptr_list[i  - 1]->idle_push_msg_info.e_push_msg_type)
            {
               //新消息放在倒数第二项
                s_idle_push_msg_ptr_list[i] = s_idle_push_msg_ptr_list[i - 1];
                s_idle_push_msg_ptr_list[i - 1] = tmp_push_msg_node_ptr;
            }
            else
            {
                //新消息放在最后一项
                s_idle_push_msg_ptr_list[i] = tmp_push_msg_node_ptr;
            }

        }
    }

     return tmp_push_msg_node_ptr;
}

/*****************************************************************************/
//  Description : 获取通知对应的图标。
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSameCaller(MMICC_NUMBER_T *cc_num1, 
                                                              MMICC_NUMBER_T *cc_num2)
{
    
    if(PNULL == cc_num1 || PNULL == cc_num2)
    {
        return FALSE;
    }

    if((cc_num1->tel_num_len != cc_num2->tel_num_len)
        ||(cc_num1->dual_sys != cc_num2->dual_sys))
    {
        return FALSE;
    }
    
    if(0 != SCI_MEMCMP(cc_num1->tel_num, cc_num2->tel_num, cc_num1->tel_num_len))
    {
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : 获取通知对应的图标。
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
LOCAL IDLE_PUSH_MSG_NODE_T *GetExistPushMsgNode(
                                                    MMI_IDLE_PUSH_MSG_TYPE_E e_push_msg_type,
                                                    uint8 *appname, 
                                                    MMICC_NUMBER_T *cc_num,
                                                    MMIPB_BCD_NUMBER_T *pb_bcd
                                                    )
{
    int i = 0;
    IDLE_PUSH_MSG_NODE_T *tmp_push_msg_node_ptr = PNULL;

    for(i = 0; i < MMI_IDLE_PUSH_MSG_MAX_COUNT; i++)
    {
        if(PNULL == s_idle_push_msg_ptr_list[i] 
           || MMI_IDLE_PUSH_MSG_STATE_VALID != s_idle_push_msg_ptr_list[i]->msg_state)
        {
            break;
        }

        if(e_push_msg_type == s_idle_push_msg_ptr_list[i]->idle_push_msg_info.e_push_msg_type)
        {
            if(MMI_IDLE_PUSH_MSG_ONGOING_CALL == e_push_msg_type
                || MMI_IDLE_PUSH_MSG_MISSED_CALL == e_push_msg_type)
            {
                //对于已经存在的同一联系人的信息，未接来电，
                //只需记录最新一条即可
                if(IsSameCaller(&s_idle_push_msg_ptr_list[i]->idle_push_msg_info.cc_number,
                                                cc_num))
                {
                    tmp_push_msg_node_ptr = s_idle_push_msg_ptr_list[i];
                    break;
                }                

            }
            else if( MMI_IDLE_PUSH_MSG_SMS == e_push_msg_type)
            {
                if(pb_bcd != PNULL && 0 == MMIAPIPB_BCDCmp(&s_idle_push_msg_ptr_list[i]->idle_push_msg_info.pb_bcd,(MMIPB_BCD_NUMBER_T*)pb_bcd))
                {
                    tmp_push_msg_node_ptr = s_idle_push_msg_ptr_list[i];
                    break;
                }
            }
            else if(MMI_IDLE_PUSH_MSG_WNS == e_push_msg_type)
            {
                //同一应用的Push消息
                //只需记录最新一条即可
                if(!stricmp(s_idle_push_msg_ptr_list[i]->idle_push_msg_info.appname, appname))
                {
                    tmp_push_msg_node_ptr = s_idle_push_msg_ptr_list[i];
                    break;
                }
            }
            else
            {
                //其他应用appname作为关键字，如同一个闹钟和日程，只需记录一条即可
                if(!stricmp(s_idle_push_msg_ptr_list[i]->idle_push_msg_info.appname, appname))
                {
                    tmp_push_msg_node_ptr = s_idle_push_msg_ptr_list[i];
                    break;
                }
            }
        }

    }

    return tmp_push_msg_node_ptr;

}


/*****************************************************************************/
//  Discription: Print the infomation about push notifuication message
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
LOCAL void PrintIdlePushMsgInfo(IDLE_PUSH_MSG_INFO_T *push_msg_info_ptr)
{
    int i = 0;
    uint8 title[MMIIDLE_LIST_MAX_STRLEN + 1] = {0}; 
    uint8 contents[MMIIDLE_LIST_MAX_STRLEN + 1] = {0}; 
    int contents_len = 0;
    
    SCI_TRACE_LOW("[idle]:PrintIdlePushMsgInfo(): enter===========================");

    if(PNULL != push_msg_info_ptr)
    {
        SCI_TRACE_LOW("[idle]: appname = (%s).", push_msg_info_ptr->appname);
        SCI_TRACE_LOW("[idle]: cc_number = (%s).", push_msg_info_ptr->cc_number.tel_num);
        SCI_TRACE_LOW("[idle]: cc_number_len = (%d).", push_msg_info_ptr->cc_number.tel_num_len);
        SCI_TRACE_LOW("[idle]: dual_sys = (%d).", push_msg_info_ptr->cc_number.dual_sys);
        SCI_TRACE_LOW("[idle]: pb_bcd = (%s).", push_msg_info_ptr->pb_bcd.number);
        SCI_TRACE_LOW("[idle]: pb_bcd_len = (%d).", push_msg_info_ptr->pb_bcd.number_len);
        SCI_TRACE_LOW("[idle]: npi_ton = (%d).", push_msg_info_ptr->pb_bcd.npi_ton);
        SCI_TRACE_LOW("[idle]: recv_num = (%s).", push_msg_info_ptr->recv_num);
        SCI_TRACE_LOW("[idle]: e_push_msg_type = (%d).", push_msg_info_ptr->e_push_msg_type);
        SCI_TRACE_LOW("[idle]: icon_data_type = (%d).", push_msg_info_ptr->icon_data_type);
#if 0
        SCI_TRACE_LOW("[idle]: app_entry_func_ptr = (0x%08x).", push_msg_info_ptr->app_entry_func_ptr);
        SCI_TRACE_LOW("[idle]: app_left_focus_action_func_ptr = (0x%08x).", push_msg_info_ptr->app_left_focus_action_func_ptr);
        SCI_TRACE_LOW("[idle]: app_right_focus_action_func_ptr = (0x%08x).", push_msg_info_ptr->app_right_focus_action_func_ptr);
#else
        SCI_TRACE_LOW("[idle]: app_entry_e = (%d).", push_msg_info_ptr->app_entry_e);
        SCI_TRACE_LOW("[idle]: app_left_focus_action_e = (%d).", push_msg_info_ptr->app_left_focus_action_e);
        SCI_TRACE_LOW("[idle]: app_right_focus_action_e = (%d).", push_msg_info_ptr->app_right_focus_action_e);
#endif
        MMIAPICOM_WstrToStr(push_msg_info_ptr->title, title);
        title[MMIIDLE_LIST_MAX_STRLEN] = '0';
        MMIAPICOM_WstrToStr(push_msg_info_ptr->contents, contents);
        contents[MMIIDLE_LIST_MAX_STRLEN] = '0';
        SCI_TRACE_LOW("[idle]: title = (%s).", title);
        SCI_TRACE_LOW("[idle]: contents = (%s)***********************", contents);

        contents_len = MMIAPICOM_Wstrlen(push_msg_info_ptr->contents);
        contents_len = MIN(contents_len, MMIIDLE_LIST_MAX_STRLEN);
        for(i = 0; i < contents_len; )
        {
            SCI_TRACE_LOW("0x%04x 0x%04x 0x%04x 0x%04x    0x%04x 0x%04x 0x%04x 0x%04x", 
                push_msg_info_ptr->contents[i],
                push_msg_info_ptr->contents[i + 1],
                push_msg_info_ptr->contents[i + 2],
                push_msg_info_ptr->contents[i + 3],
                push_msg_info_ptr->contents[i + 4],
                push_msg_info_ptr->contents[i + 5],
                push_msg_info_ptr->contents[i + 6],
                push_msg_info_ptr->contents[i + 7]);
            i += 8;
        }
        
    }
    SCI_TRACE_LOW("[idle]:PrintIdlePushMsgInfo(): left===========================");
}

/*****************************************************************************/
//  Description : 获取通知对应的设置。
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
BOOLEAN getNotificationSetting(MMI_IDLE_PUSH_MSG_TYPE_E e_push_msg_type)
{
//#ifdef UNISOC_VERSION

    BOOLEAN b_ret = FALSE;
    switch(e_push_msg_type)
    {
    case MMI_IDLE_PUSH_MSG_SMS:
        b_ret = MMISet_GetNotification(MMISET_NOTIFICATION_MESSAGES);
        break;

    case MMI_IDLE_PUSH_MSG_ONGOING_CALL:
        b_ret = TRUE;
        break;
    case MMI_IDLE_PUSH_MSG_MISSED_CALL:
        b_ret = MMISet_GetNotification(MMISET_NOTIFICATION_CALLS);
        break;
       
    case MMI_IDLE_PUSH_MSG_VOICE_MAIL:
        b_ret = MMISet_GetNotification(MMISET_NOTIFICATION_MAILS);
        break;
        
    case MMI_IDLE_PUSH_MSG_STK:
    case MMI_IDLE_PUSH_MSG_OPERATER_SETTING:
    case MMI_IDLE_PUSH_MSG_SNOOZED_CALENDER:
    case MMI_IDLE_PUSH_MSG_SNOOZED_ALARM:
    case MMI_IDLE_PUSH_MSG_WNS:
        b_ret = MMISet_GetNotification(MMISET_NOTIFICATION_OTHERS/*MMINV_SET_NOTIFICATION_OTHERS*/);
        break;
  
    default:
        break;
        
    }
    
    return b_ret;
    
//#else //#ifdef UNISOC_VERSION

//    return TRUE;

//#endif //#ifdef UNISOC_VERSION


}



/*****************************************************************************/
//  Description : 提供给应用的接口，保存IDLE界面显示的通知信息
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_AppendPushMsg(IDLE_PUSH_MSG_INFO_T *push_msg_info_ptr)
{
    IDLE_PUSH_MSG_NODE_T *tmp_push_msg_node_ptr = PNULL;

    PrintIdlePushMsgInfo(push_msg_info_ptr);

    
    if(PNULL != push_msg_info_ptr)
    {
        if(MMI_IDLE_PUSH_MSG_TYPE_MAX <= push_msg_info_ptr->e_push_msg_type)
        {
            return FALSE;
        }

        if(!getNotificationSetting(push_msg_info_ptr->e_push_msg_type))
        {
            SCI_TRACE_LOW("[idle]:MMIIDLE_AppendPushMsg(): notification setting is off");
            return FALSE;
        }

        tmp_push_msg_node_ptr = GetExistPushMsgNode(push_msg_info_ptr->e_push_msg_type,
                                                    push_msg_info_ptr->appname, 
                                                    &push_msg_info_ptr->cc_number,
                                                    &push_msg_info_ptr->pb_bcd
                                                    );

        if(PNULL == tmp_push_msg_node_ptr)
        {
            tmp_push_msg_node_ptr = GetNewMsgNode(push_msg_info_ptr);
        }

        if(PNULL == tmp_push_msg_node_ptr)
        {
            SCI_TRACE_LOW("[idle]:MMIIDLE_AppendPushMsg(): tmp_push_msg_node_ptr is null");
            return FALSE;
        }

        SCI_MEMCPY(&(tmp_push_msg_node_ptr->idle_push_msg_info), 
                                push_msg_info_ptr,
                                sizeof(IDLE_PUSH_MSG_INFO_T)); 
        tmp_push_msg_node_ptr->msg_state = MMI_IDLE_PUSH_MSG_STATE_VALID;

        if(MMI_IDLE_PUSH_MSG_ONGOING_CALL == push_msg_info_ptr->e_push_msg_type)
        {
            tmp_push_msg_node_ptr->msg_count = 1;
        }
        else
        {
            tmp_push_msg_node_ptr->msg_count++;
        }

        if(tmp_push_msg_node_ptr->msg_count > 1)
        {
            uint8 title_str[MMIIDLE_LIST_MAX_STRLEN + 1] = {0};
            wchar title_wstr[MMIIDLE_LIST_MAX_STRLEN + 1] = {0};
            uint16 title_str_len = 0;
        
            sprintf((char *)title_str,"(%d)",(int)tmp_push_msg_node_ptr->msg_count);
            title_str_len = strlen(title_str);
            MMIAPICOM_StrToWstr(title_str,title_wstr); 
            MMIAPICOM_Wstrncpy(&title_wstr[title_str_len], 
                                                    tmp_push_msg_node_ptr->idle_push_msg_info.title,
                                                    MMIIDLE_LIST_MAX_STRLEN - title_str_len); 
            MMIAPICOM_Wstrncpy(tmp_push_msg_node_ptr->idle_push_msg_info.title,
                                                    title_wstr,
                                                    MMIIDLE_LIST_MAX_STRLEN); 

        }

        SavePushMsgInfo();

        if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
        {
            MMK_SendMsg(MAIN_IDLE_WIN_ID,MSG_IDLE_UPDATE_NOTIFICATION_LIST,PNULL);
        }
    }
    

    return TRUE;
}

PUBLIC void MMICC_RemoveAllOnGoingForConferenceCallMsg(void)
{
    int i = 0;

    for(i = 0; i < MMI_IDLE_PUSH_MSG_MAX_COUNT; i ++)
    {
        if(PNULL != s_idle_push_msg_ptr_list[i] && 
			s_idle_push_msg_ptr_list[i]->idle_push_msg_info.e_push_msg_type == MMI_IDLE_PUSH_MSG_ONGOING_CALL)
        {
            SCI_FREE(s_idle_push_msg_ptr_list[i]);
            s_idle_push_msg_ptr_list[i] = PNULL;
        }
    }

    SavePushMsgInfo();

    if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
        MMK_SendMsg(MAIN_IDLE_WIN_ID,MSG_IDLE_UPDATE_NOTIFICATION_LIST,PNULL);
    }

}

/**************************************************************************/ 
// Description : 提供给应用的接口，清除Idle Screen消息的接口
// Global resource dependence : 
// Author: grant.peng 
// Note: 
/**************************************************************************/ 
PUBLIC void MMIIDLE_RemovePushMsg(
                                MMI_IDLE_PUSH_MSG_TYPE_E e_push_msg_type,
                                uint8 *appname, 
                                MMICC_NUMBER_T *cc_number)
{
    IDLE_PUSH_MSG_NODE_T *tmp_push_msg_node_ptr = PNULL;

    SCI_TRACE_LOW("[idle]: MMIIDLE_RemovePushMsg(): appname = (%s).", appname);
    if(PNULL != cc_number)
    {
        SCI_TRACE_LOW("[idle]: cc_number = (%s).", cc_number->tel_num);
        SCI_TRACE_LOW("[idle]: cc_number_len = (%d).", cc_number->tel_num_len);
    }
    
    tmp_push_msg_node_ptr = GetExistPushMsgNode(e_push_msg_type,
                                                                                                appname, 
                                                                                                cc_number,
                                                                                                PNULL
                                                                                                );
    if(PNULL != tmp_push_msg_node_ptr)
    {
        FreePushMsgNode(tmp_push_msg_node_ptr);
    }

}



/**************************************************************************/ 
// Description : 提供给应用的接口，清除Idle Screen消息的接口
// Global resource dependence : 
// Author: grant.peng 
// Note: 
/**************************************************************************/ 
PUBLIC void MMIIDLE_RemovePushMsgByType(
                                MMI_IDLE_PUSH_MSG_TYPE_E e_push_msg_type)
{
    IDLE_PUSH_MSG_NODE_T *tmp_push_msg_node_ptr = PNULL;

    int i = 0;
    int j = 0;

    for(i = 0; i < MMI_IDLE_PUSH_MSG_MAX_COUNT; )
    {
        if(PNULL == s_idle_push_msg_ptr_list[i] 
           || MMI_IDLE_PUSH_MSG_STATE_VALID != s_idle_push_msg_ptr_list[i]->msg_state)
        {
            break;
        }

        if(e_push_msg_type == s_idle_push_msg_ptr_list[i]->idle_push_msg_info.e_push_msg_type)
        {
            SCI_FREE(s_idle_push_msg_ptr_list[i]);
            s_idle_push_msg_ptr_list[i] = PNULL;

            for(j = i + 1; j < MMI_IDLE_PUSH_MSG_MAX_COUNT; j++)
            {
                if(PNULL == s_idle_push_msg_ptr_list[j] 
                   || MMI_IDLE_PUSH_MSG_STATE_VALID != s_idle_push_msg_ptr_list[j]->msg_state)
                {
                    s_idle_push_msg_ptr_list[j - 1] = PNULL;
                    break;
                }
                s_idle_push_msg_ptr_list[j - 1] = s_idle_push_msg_ptr_list[j];
            }
        }
        else
        {
             i++;
        }
    }
    
    SavePushMsgInfo();

    if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
        MMK_SendMsg(MAIN_IDLE_WIN_ID,MSG_IDLE_UPDATE_NOTIFICATION_LIST,PNULL);
    }

}

/**************************************************************************/ 
// Description : 提供给应用的接口，清除Idle Screen消息的接口
// Global resource dependence : 
// Author: grant.peng 
// Note: 
/**************************************************************************/ 
PUBLIC void MMIIDLE_RemovePushMsgEx(
                                MMI_IDLE_PUSH_MSG_TYPE_E e_push_msg_type,
                                uint8 *appname, 
                                MMIPB_BCD_NUMBER_T *pb_bcd
                                )
{
    IDLE_PUSH_MSG_NODE_T *tmp_push_msg_node_ptr = PNULL;

    SCI_TRACE_LOW("[idle]: MMIIDLE_RemovePushMsg(): appname = (%s) e_push_msg_type = (%d) .", appname, e_push_msg_type);

    if(PNULL != pb_bcd)
    {
        SCI_TRACE_LOW("[idle]: pb_bcd = (%s).", pb_bcd->number);
        SCI_TRACE_LOW("[idle]: pb_bcd_len = (%d).", pb_bcd->number_len);
    }

    tmp_push_msg_node_ptr = GetExistPushMsgNode(e_push_msg_type,
                                                                                                appname, 
                                                                                                PNULL,
                                                                                                pb_bcd
                                                                                                );
    if(PNULL != tmp_push_msg_node_ptr)
    {
        FreePushMsgNode(tmp_push_msg_node_ptr);
    }

}

/*****************************************************************************/
//  Description : 清除Idle 界面上所有的通知
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_RemoveAllPushMsg(void)
{
    int i = 0;

    for(i = 0; i < MMI_IDLE_PUSH_MSG_MAX_COUNT; i ++)
    {
        if(PNULL != s_idle_push_msg_ptr_list[i])
        {
            SCI_FREE(s_idle_push_msg_ptr_list[i]);
            s_idle_push_msg_ptr_list[i] = PNULL;
        }
    }

    SavePushMsgInfo();

    if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
        MMK_SendMsg(MAIN_IDLE_WIN_ID,MSG_IDLE_UPDATE_NOTIFICATION_LIST,PNULL);
    }

}


/*****************************************************************************/
//  Description : Get push msg node pointer
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
PUBLIC IDLE_PUSH_MSG_NODE_T *MMIIDLE_GetPushMsgNode(uint16 node_index)
{
    if(MMI_IDLE_PUSH_MSG_MAX_COUNT > node_index)
    {
        return s_idle_push_msg_ptr_list[node_index];
    }

    return PNULL;
}

/*****************************************************************************/
//  Description : 获取通知对应的图标。
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIIDLE_GetPushMsgIcon(MMI_IDLE_PUSH_MSG_TYPE_E e_push_msg_type)
{
    MMI_IMAGE_ID_T push_msg_icon = 0;

    switch(e_push_msg_type)
    {
#if 0    
    case MMI_IDLE_PUSH_MSG_SMS:
        push_msg_icon = IMAGE_IDLE_SCREEN_MESSAGE;
        break;
        
    case MMI_IDLE_PUSH_MSG_MISSED_CALL:
        push_msg_icon = IMAGE_IDLE_SCREEN_CALL;
        break;
       
    case MMI_IDLE_PUSH_MSG_WNS:
        push_msg_icon = IMAGE_IDLE_SCREEN_PUSH;
        break;

    case MMI_IDLE_PUSH_MSG_SNOOZED_ALARM:
        push_msg_icon = IMAGE_IDLE_SCREEN_ALARM;
        break;

    case MMI_IDLE_PUSH_MSG_SNOOZED_CALENDER:
        push_msg_icon = IMAGE_IDLE_SCREEN_CALENDAR;
        break;
        
    case MMI_IDLE_PUSH_MSG_VOICE_MAIL:
        push_msg_icon = IMAGE_IDLE_SCREEN_VOICE_MAIL;
        break;
        
    case MMI_IDLE_PUSH_MSG_OPERATER_SETTING:
        push_msg_icon = IMAGE_IDLE_SCREEN_OPERATOR_MSG;
        break;

    case MMI_IDLE_PUSH_MSG_STK:
        push_msg_icon = IMAGE_IDLE_SCREEN_MESSAGE;
        break;
  #endif
    default:
        push_msg_icon = IMAGE_NULL;
        break;
        
    }

    return push_msg_icon; 
}

#ifdef MMI_COMMON_IDLE_SUPPORT
/*****************************************************************************/
//  Description : 设置通知列表每一项对应的软键
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_SetPushMsgSoftkey(MMI_IDLE_PUSH_MSG_TYPE_E e_push_msg_type, GUILIST_ITEM_DATA_T *item_data)
{

    if(MMI_IDLE_PUSH_MSG_TYPE_MAX > e_push_msg_type && PNULL != item_data)
    {
        switch(e_push_msg_type)
        {
        
        case MMI_IDLE_PUSH_MSG_SMS:
            item_data->softkey_id[0] = TXT_COMMON_MAINMENU;
            item_data->softkey_id[1] = STXT_SOFTKEY_READ_MK;
            item_data->softkey_id[2] = TXT_IDLE_LOCK;

            item_data->lfocus_softkey_id[0] = TXT_COMMON_MAINMENU;
            item_data->lfocus_softkey_id[1] = STXT_SOFTKEY_DISMISS_MK;            //Dismiss
            item_data->lfocus_softkey_id[2] = TXT_IDLE_LOCK;

            item_data->rfocus_softkey_id[0] = TXT_COMMON_MAINMENU;
            item_data->rfocus_softkey_id[1] = STXT_SOFTKEY_REPLY;
            item_data->rfocus_softkey_id[2] = TXT_IDLE_LOCK;
            break;
            
        case MMI_IDLE_PUSH_MSG_MISSED_CALL:
            item_data->softkey_id[0] = TXT_COMMON_MAINMENU;
            item_data->softkey_id[1] = STXT_SOFTKEY_OPEN_MK;
            item_data->softkey_id[2] = TXT_IDLE_LOCK;

            item_data->lfocus_softkey_id[0] = TXT_COMMON_MAINMENU;
            item_data->lfocus_softkey_id[1] = STXT_SOFTKEY_DISMISS_MK;            //Dismiss
            item_data->lfocus_softkey_id[2] = TXT_IDLE_LOCK;

            item_data->rfocus_softkey_id[0] = TXT_COMMON_MAINMENU;
            item_data->rfocus_softkey_id[1] = STXT_SOFTKEY_CALL_MK;
            item_data->rfocus_softkey_id[2] = TXT_IDLE_LOCK;
            break;
            
        case MMI_IDLE_PUSH_MSG_ONGOING_CALL:
            item_data->softkey_id[0] = TXT_COMMON_MAINMENU;
            item_data->softkey_id[1] = STXT_SOFTKEY_OPEN_MK;
            item_data->softkey_id[2] = TXT_IDLE_LOCK;

            item_data->lfocus_softkey_id[0] = TXT_COMMON_MAINMENU;
            item_data->lfocus_softkey_id[1] = STXT_SOFTKEY_DISMISS_MK;
            item_data->lfocus_softkey_id[2] = TXT_IDLE_LOCK;

            break;
            
        case MMI_IDLE_PUSH_MSG_WNS:
            item_data->softkey_id[0] = TXT_COMMON_MAINMENU;
            item_data->softkey_id[1] = STXT_SOFTKEY_OPEN_MK;
            item_data->softkey_id[2] = TXT_IDLE_LOCK;

            item_data->lfocus_softkey_id[0] = TXT_COMMON_MAINMENU;
            item_data->lfocus_softkey_id[1] = STXT_SOFTKEY_DISMISS_MK;
            item_data->lfocus_softkey_id[2] = TXT_IDLE_LOCK;
            break;

        case MMI_IDLE_PUSH_MSG_SNOOZED_ALARM:
        case MMI_IDLE_PUSH_MSG_SNOOZED_CALENDER:
            item_data->softkey_id[0] = TXT_COMMON_MAINMENU;
            item_data->softkey_id[1] = STXT_SOFTKEY_OPEN_MK;
            item_data->softkey_id[2] = TXT_IDLE_LOCK;

            item_data->lfocus_softkey_id[0] = TXT_COMMON_MAINMENU;
            item_data->lfocus_softkey_id[1] = STXT_SOFTKEY_DISMISS_MK;
            item_data->lfocus_softkey_id[2] = TXT_IDLE_LOCK;
            break;
            
        default:
            item_data->softkey_id[0] = TXT_COMMON_MAINMENU;
            item_data->softkey_id[1] = STXT_SOFTKEY_OPEN_MK;
            item_data->softkey_id[2] = TXT_IDLE_LOCK;
            break;
            
        }

    }

    return; 
}
#endif

#ifdef MMI_MSTYLE_SUPPORT
/*****************************************************************************/
//  Description : 设置通知列表每一项对应的软键
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_SetPushMsgSoftkey(MMI_IDLE_PUSH_MSG_TYPE_E e_push_msg_type, GUILIST_ITEM_DATA_T *item_data)
{

    if(MMI_IDLE_PUSH_MSG_TYPE_MAX > e_push_msg_type && PNULL != item_data)
    {
        switch(e_push_msg_type)
        {
        
        case MMI_IDLE_PUSH_MSG_SMS:
            item_data->softkey_id[0] = IMAGE_NULL;
            item_data->softkey_id[1] = STXT_SOFTKEY_READ_MK;
            item_data->softkey_id[2] = IMAGE_NULL;

            item_data->lfocus_softkey_id[0] = IMAGE_NULL;
            item_data->lfocus_softkey_id[1] = STXT_SOFTKEY_DISMISS_MK;            //Dismiss
            item_data->lfocus_softkey_id[2] = IMAGE_NULL;

            item_data->rfocus_softkey_id[0] = IMAGE_NULL;
            item_data->rfocus_softkey_id[1] = STXT_SOFTKEY_REPLY;
            item_data->rfocus_softkey_id[2] = IMAGE_NULL;
            break;
            
        case MMI_IDLE_PUSH_MSG_MISSED_CALL:
            item_data->softkey_id[0] = IMAGE_NULL;
            item_data->softkey_id[1] = STXT_SOFTKEY_OPEN_MK;
            item_data->softkey_id[2] = IMAGE_NULL;

            item_data->lfocus_softkey_id[0] = IMAGE_NULL;
            item_data->lfocus_softkey_id[1] = STXT_SOFTKEY_DISMISS_MK;            //Dismiss
            item_data->lfocus_softkey_id[2] = IMAGE_NULL;

            item_data->rfocus_softkey_id[0] = IMAGE_NULL;
            item_data->rfocus_softkey_id[1] = STXT_SOFTKEY_CALL_MK;
            item_data->rfocus_softkey_id[2] = IMAGE_NULL;
            break;
            
        case MMI_IDLE_PUSH_MSG_ONGOING_CALL:
            item_data->softkey_id[0] = IMAGE_NULL;
            item_data->softkey_id[1] = STXT_SOFTKEY_OPEN_MK;
            item_data->softkey_id[2] = IMAGE_NULL;

            item_data->lfocus_softkey_id[0] = IMAGE_NULL;
            item_data->lfocus_softkey_id[1] = STXT_SOFTKEY_DISMISS_MK;
            item_data->lfocus_softkey_id[2] = IMAGE_NULL;

            break;
            
        case MMI_IDLE_PUSH_MSG_WNS:
            item_data->softkey_id[0] = IMAGE_NULL;
            item_data->softkey_id[1] = STXT_SOFTKEY_OPEN_MK;
            item_data->softkey_id[2] = IMAGE_NULL;

            item_data->lfocus_softkey_id[0] = IMAGE_NULL;
            item_data->lfocus_softkey_id[1] = STXT_SOFTKEY_DISMISS_MK;
            item_data->lfocus_softkey_id[2] = IMAGE_NULL;
            break;

        case MMI_IDLE_PUSH_MSG_SNOOZED_ALARM:
        case MMI_IDLE_PUSH_MSG_SNOOZED_CALENDER:
            item_data->softkey_id[0] = IMAGE_NULL;
            item_data->softkey_id[1] = STXT_SOFTKEY_OPEN_MK;
            item_data->softkey_id[2] = IMAGE_NULL;

            item_data->lfocus_softkey_id[0] = IMAGE_NULL;
            item_data->lfocus_softkey_id[1] = STXT_SOFTKEY_DISMISS_MK;
            item_data->lfocus_softkey_id[2] = IMAGE_NULL;
            break;
            
        default:
            item_data->softkey_id[0] = IMAGE_NULL;
            item_data->softkey_id[1] = STXT_SOFTKEY_OPEN_MK;
            item_data->softkey_id[2] = IMAGE_NULL;
            break;
            
        }

    }

    return; 
}
#endif //#ifdef MMI_MSTYLE_SUPPORT


/*****************************************************************************/
//  Description : 设置通知列表每一项对应的左右Action
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_SetPushListItemLRAction(MMI_CTRL_ID_T ctrl_id, int item_count)
{

    IDLE_PUSH_MSG_NODE_T *idle_push_msg_ptr = PNULL;
    uint32 user_data = 0;
    int item_index = 0;

    for(item_index = 0; item_index < item_count; item_index++)
    {
        GUILIST_GetItemData(ctrl_id, item_index, &user_data);
        idle_push_msg_ptr = (IDLE_PUSH_MSG_NODE_T *)user_data;
        if(PNULL != idle_push_msg_ptr)
        {
            switch(idle_push_msg_ptr->idle_push_msg_info.e_push_msg_type)
            {
                case MMI_IDLE_PUSH_MSG_SMS:
                    GUILIST_SetItemShowLRTriangleIconByIndex(ctrl_id, item_index, TRUE, TRUE);
                    GUILIST_SetItemHandleLeftRightByIndex(ctrl_id, item_index, TRUE, TRUE);
                    CTRLLIST_SetItemQuickIconByIndex(ctrl_id, item_index, 
                            IMAGE_SHORTCUT_DELETE, 
                            IMAGE_IDLE_SCREEN_REPLY);
                    break;
                    
                case MMI_IDLE_PUSH_MSG_MISSED_CALL:
                    GUILIST_SetItemShowLRTriangleIconByIndex(ctrl_id, item_index, TRUE, TRUE);
                    GUILIST_SetItemHandleLeftRightByIndex(ctrl_id, item_index, TRUE, TRUE);
                    CTRLLIST_SetItemQuickIconByIndex(ctrl_id, item_index, 
                            IMAGE_SHORTCUT_DELETE, 
                            IMAGE_IDLE_SCREEN_CALL_BACK);
                    break;
                    
                case MMI_IDLE_PUSH_MSG_ONGOING_CALL:
                    GUILIST_SetItemShowLRTriangleIconByIndex(ctrl_id, item_index, FALSE, FALSE);
                    GUILIST_SetItemHandleLeftRightByIndex(ctrl_id, item_index, FALSE, FALSE);
                    CTRLLIST_SetItemQuickIconByIndex(ctrl_id, item_index, 
                            IMAGE_NULL, 
                            IMAGE_NULL);
                    break;
                    
                case MMI_IDLE_PUSH_MSG_WNS:
                    GUILIST_SetItemShowLRTriangleIconByIndex(ctrl_id, item_index, TRUE, FALSE);
                    GUILIST_SetItemHandleLeftRightByIndex(ctrl_id, item_index, TRUE, FALSE);
                    CTRLLIST_SetItemQuickIconByIndex(ctrl_id, item_index, 
                            IMAGE_SHORTCUT_DELETE, 
                            IMAGE_NULL);
                    break;
                case MMI_IDLE_PUSH_MSG_SNOOZED_ALARM:
                    GUILIST_SetItemShowLRTriangleIconByIndex(ctrl_id, item_index, TRUE, FALSE);
                    GUILIST_SetItemHandleLeftRightByIndex(ctrl_id, item_index, TRUE, FALSE);
                    CTRLLIST_SetItemQuickIconByIndex(ctrl_id, item_index, 
                            IMAGE_SHORTCUT_DELETE, 
                            IMAGE_NULL);
                    break;
                case MMI_IDLE_PUSH_MSG_SNOOZED_CALENDER:
                    GUILIST_SetItemShowLRTriangleIconByIndex(ctrl_id, item_index, TRUE, FALSE);
                    GUILIST_SetItemHandleLeftRightByIndex(ctrl_id, item_index, TRUE, FALSE);
                    CTRLLIST_SetItemQuickIconByIndex(ctrl_id, item_index, 
                            IMAGE_SHORTCUT_DELETE, 
                            IMAGE_NULL);
                    break;
                case MMI_IDLE_PUSH_MSG_STK:
                    GUILIST_SetItemShowLRTriangleIconByIndex(ctrl_id, item_index, TRUE, FALSE);
                    GUILIST_SetItemHandleLeftRightByIndex(ctrl_id, item_index, TRUE, FALSE);
                    CTRLLIST_SetItemQuickIconByIndex(ctrl_id, item_index, 
                            IMAGE_SHORTCUT_DELETE, 
                            IMAGE_NULL);
                    break;

                default:
                    break;
            }
        }
    }
}



/*****************************************************************************/
//  Description : 获取List 左右键Action对应的图标。
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIIDLE_GetRightFocusIcon(MMI_IDLE_PUSH_MSG_TYPE_E e_push_msg_type)
{
    MMI_IMAGE_ID_T right_focus_icon = 0;

    switch(e_push_msg_type)
    {
    
    case MMI_IDLE_PUSH_MSG_SMS:
        right_focus_icon = IMAGE_IDLE_SCREEN_REPLY;
        break;
        
    case MMI_IDLE_PUSH_MSG_MISSED_CALL:
        right_focus_icon = IMAGE_IDLE_SCREEN_CALL_BACK;
        break;
        
    case MMI_IDLE_PUSH_MSG_ONGOING_CALL:
        right_focus_icon = IMAGE_IDLE_SCREEN_CALL_BACK;
        break;
        
    case MMI_IDLE_PUSH_MSG_WNS:
        right_focus_icon = IMAGE_NULL;
        break;

    default:
        right_focus_icon = 0;
        break;
        
    }

    return right_focus_icon; 
}

/*****************************************************************************/
//     Description : load push msg info form nv
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_LoadPushMsgInfo(void)
{
    int i = 0;
    MN_RETURN_RESULT_E ret_value = MN_RETURN_SUCCESS;
    IDLE_PUSH_MSG_NV_T *idle_push_msg_nv_ptr = PNULL;

    char appname[4] = {'S','T','K','\0'};

    SCI_TRACE_LOW("[wns]:MMIIDLE_LoadPushMsgInfo(): enter.");

  
    

    do
    {
        idle_push_msg_nv_ptr = (IDLE_PUSH_MSG_NV_T *)SCI_ALLOC_APP(sizeof(IDLE_PUSH_MSG_NV_T));
        if(PNULL == idle_push_msg_nv_ptr)
        {
            break;
        }
        SCI_MEMSET(idle_push_msg_nv_ptr, 0x0, sizeof(IDLE_PUSH_MSG_NV_T));

        ret_value = MMI_ReadNVItem(MMINV_IDLE_PUSH_NOTIFICATION, idle_push_msg_nv_ptr);
        if (MN_RETURN_SUCCESS == ret_value)
        {
            for(i = 0; i < MMI_IDLE_PUSH_MSG_MAX_COUNT; i++)
            {
                IDLE_PUSH_MSG_NODE_T *push_msg_node_ptr = PNULL;
                int push_msg_info_size = sizeof(IDLE_PUSH_MSG_NODE_T);
                
                if(MMI_IDLE_PUSH_MSG_STATE_VALID 
                    != idle_push_msg_nv_ptr->idle_push_msg_info[i].msg_state)
                {
                    break;
                }
                
                push_msg_node_ptr = (IDLE_PUSH_MSG_NODE_T *)SCI_ALLOC_APP(push_msg_info_size);
                if(PNULL != push_msg_node_ptr)
                {
                    SCI_MEMSET(push_msg_node_ptr, 0x0, push_msg_info_size);
                    SCI_MEMCPY(push_msg_node_ptr, 
                        &idle_push_msg_nv_ptr->idle_push_msg_info[i], 
                        push_msg_info_size); 
                    s_idle_push_msg_ptr_list[i] = push_msg_node_ptr;
                }
            }
        }

    }while(0);

    MMIIDLE_RemovePushMsg(MMI_IDLE_PUSH_MSG_STK, appname, PNULL);

    if(PNULL != idle_push_msg_nv_ptr)
    {
        SCI_FREE(idle_push_msg_nv_ptr);
        idle_push_msg_nv_ptr = PNULL;
    }

    if(MMIIDLE_HaveNewNotification(MMI_IDLE_PUSH_MSG_WNS))
    {
        MMIAPICOM_StatusAreaSetAppNoticeState(TRUE);
    }
    else
    {
        MMIAPICOM_StatusAreaSetAppNoticeState(FALSE);
    }

}

PUBLIC BOOLEAN MMIIDLE_HaveNewNotification(MMI_IDLE_PUSH_MSG_TYPE_E e_type)
{
    int i = 0;

    for(i = 0; i < MMI_IDLE_PUSH_MSG_MAX_COUNT; i ++)
    {
        IDLE_PUSH_MSG_NODE_T *idle_push_msg_node_ptr = PNULL;

        idle_push_msg_node_ptr = MMIIDLE_GetPushMsgNode(i);
        if(PNULL != idle_push_msg_node_ptr
            && MMI_IDLE_PUSH_MSG_STATE_VALID == idle_push_msg_node_ptr->msg_state)
        {
            if(e_type == idle_push_msg_node_ptr->idle_push_msg_info.e_push_msg_type)
            {
                return TRUE;
            }
        }
        else
        {
            break;
        }
    }

    return FALSE;

}



/*****************************************************************************/
// Description : is chinese emergency call 
// Global resource dependence : 
//  Author:bownzhang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN IsEmergencyCall(MMI_STRING_T *str_info_ptr, uint16 *emergency_call_len_ptr, 
                                                                       const char *emergency_call_list[], uint16 emergency_call_list_num)
{
    int i = 0;
    uint16 temp_emergency_call_len = 0;
    wchar temp_emergency_call[MMIIDLE_EMERGENCY_CALL_NUM_MAX_LEN + 1] = {0};
    BOOLEAN is_input_emergency_call = FALSE;

    for (i = 0; i < emergency_call_list_num; i++)
    {
        temp_emergency_call_len = strlen(emergency_call_list[i]);
        temp_emergency_call_len = MIN(temp_emergency_call_len, str_info_ptr->wstr_len);
        MMI_STRNTOWSTR(temp_emergency_call,
            MMIIDLE_EMERGENCY_CALL_NUM_MAX_LEN,
            (uint8 *)emergency_call_list[i],
            strlen(emergency_call_list[i]),
            temp_emergency_call_len);
        
        if (0 != MMIAPICOM_CompareTwoWstr(str_info_ptr->wstr_ptr, str_info_ptr->wstr_len, temp_emergency_call, temp_emergency_call_len))
        {
            is_input_emergency_call = FALSE;
        }
        else if (strlen(emergency_call_list[i]) < str_info_ptr->wstr_len)
        {
            is_input_emergency_call = FALSE;
        }
        else
        {
            *emergency_call_len_ptr = strlen(emergency_call_list[i]);
            is_input_emergency_call = TRUE;
            
            return is_input_emergency_call;
        }
    }

    return is_input_emergency_call;

}


/*****************************************************************************/
// Description : is chinese emergency call 
// Global resource dependence : 
//  Author:bownzhang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN IsChineseEmergencyCall(MMI_STRING_T *str_info_ptr, uint16 *emergency_call_len_ptr, MN_DUAL_SYS_E dual_sys)
{
    uint16 emergency_call_list_num = 0;
    BOOLEAN is_input_emergency_call = FALSE;

    emergency_call_list_num = ARR_SIZE(s_emergency_call_china);

    is_input_emergency_call = IsEmergencyCall(str_info_ptr, emergency_call_len_ptr, s_emergency_call_china, emergency_call_list_num);
    
    return is_input_emergency_call;

}


/*****************************************************************************/
// Description : is chinese emergency call 
// Global resource dependence : 
//  Author:bownzhang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN IsAustraliaEmergencyCall(MMI_STRING_T *str_info_ptr, uint16 *emergency_call_len_ptr, MN_DUAL_SYS_E dual_sys)
{
    uint16 emergency_call_list_num = 0;
    BOOLEAN is_input_emergency_call = FALSE;

    emergency_call_list_num = ARR_SIZE(s_emergency_call_australia);

    is_input_emergency_call = IsEmergencyCall(str_info_ptr, emergency_call_len_ptr, s_emergency_call_australia, emergency_call_list_num);
    
    return is_input_emergency_call;

}

/*****************************************************************************/
// Description : get ndt emergency call config 
// Global resource dependence : 
//  Author:bownzhang
// Note:
/*****************************************************************************/
LOCAL uint8 GetNDTEmergencyConfig(void)
{
     /*
        这个是锁屏界面拨打紧急号码显示的，
        此功能对应的NV是NV_VARIANT_CONFIG_APP/EMERGENCY_CALL_UI，
        0,1,2 UI 显示都是一样的，区别只是
        0: No emergency call UI display，锁屏的时候只能打默认的紧急号码
        1: Emergency call UI for China  锁屏的时候除了可以拨打紧急号码，
            还能呼出中国区常用的紧急号码，并且是通过正常呼叫的形式呼出
        2: Emergency call UI for AU   锁屏的时候除了可以拨打紧急号码，
            还能呼出澳大利亚国家的紧急号码，通过标准紧急号码呼出
     */       

    uint8 emergency_config = MMI_DNT_ReadEmergencyCallUIConfigure();

    SCI_TRACE_LOW("[idle]:GetNDTEmergencyConfig(): emergency_config[%d]", emergency_config);

    return emergency_config;
}


/*****************************************************************************/
// Description : emergency call win msg
// Global resource dependence : 
//  Author:bownzhang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIdle_IsInputEmergencyCall(MMI_STRING_T *str_info_ptr, uint16 *emergency_call_len_ptr, MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_input_emergency_call = FALSE;
    //uint8 emergency_config = 0;

    is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(str_info_ptr, emergency_call_len_ptr, dual_sys);

    /*
    对于澳大利亚和中国区紧急号码的判断，CC模块会根据有卡无卡来判断
    Idle不用关注
    */
    /*
    if (!is_input_emergency_call)
    {
        emergency_config = GetNDTEmergencyConfig();
        if(1 == emergency_config)
        {
            is_input_emergency_call = IsChineseEmergencyCall(str_info_ptr, emergency_call_len_ptr, dual_sys);
        }
        else if(2 == emergency_config)
        {
            is_input_emergency_call = IsAustraliaEmergencyCall(str_info_ptr, emergency_call_len_ptr, dual_sys);
        }
        else
        {
            //Do nothing
        }
    }
    */
    
    return is_input_emergency_call;
}




/*****************************************************************************/
// Description : get sim card and plmn service status string to display in idle
//Global resource dependence : 
//  Author: bruce.chi
//Note:  
/*****************************************************************************/
PUBLIC uint32 MMIIDLE_GetSimAndServiceString(MMI_STRING_T *network_str_ptr)
{
    uint8   emergency_config = 0;
    SIM_STATUS_E    sim_status = SIM_STATUS_MAX;
    MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;
    IDLE_DISPLAY_SERVICE_TYPE_T service_type[MN_DUAL_SYS_MAX] = {0};    

    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    
    if(PNULL == network_str_ptr  ||PNULL == network_str_ptr->wstr_ptr)
    {
        SCI_TRACE_LOW("[idle]: MMIIDLE_GetSimAndServiceString(): the network_str_ptr or network_str_ptr->wstr_ptr is NULL");
        return IDLE_DISPLAY_SERVICE_MAX;
    }

    network_str_ptr->wstr_len = 0;

    emergency_config =  MMI_DNT_ReadEmergencyCallUIConfigure();
    SCI_TRACE_LOW("[idle]: MMIIDLE_GetSimAndServiceString(): emergency_config(%d)", emergency_config);
    if(0 == emergency_config)
    {
        return IDLE_DISPLAY_NORMAL;
    }

    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MN_DUAL_SYS_MAX; dual_sys++)
    {
        sim_status = MMIAPIPHONE_GetSimStatus(dual_sys);
        plmn_status = MMIAPIPHONE_GetPlmnStatus(dual_sys);

        SCI_TRACE_LOW("[idle]: MMIIDLE_GetSimAndServiceString(): dual_sys=%d, sim_status %d, plmn_status=%d",
                                dual_sys, sim_status, plmn_status);

        //check service status
        switch(plmn_status)
        {
            case PLMN_NO_SERVICE:
                service_type[dual_sys] = IDLE_DISPLAY_NO_SERVICE;
                break;
            case PLMN_NORMAL_GPRS_ONLY:
            case PLMN_EMERGENCY_ONLY:
            case PLMN_EMERGENCY_SIM_INVALID:
            case PLMN_EMERGENCY_GPRS_ONLY:
            case PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY:
                service_type[dual_sys] = IDLE_DISPLAY_LIMITED;
                break;
            case PLMN_FULL_PS_SERVICE:
            case PLMN_REGISTER_GPRS_ONLY:
            case PLMN_REGISTER_SERVICE:                    
            case PLMN_NORMAL_GSM_ONLY:
            case PLMN_NORMAL_GSM_GPRS_BOTH:
                if(sim_status == SIM_STATUS_OK)
                {
                    service_type[dual_sys] = IDLE_DISPLAY_NORMAL;
                    SCI_TRACE_LOW("[idle]: MMIIDLE_GetSimAndServiceString(): dual_sys=%d SIM is on work", dual_sys);
                    return service_type[dual_sys];
                }
                else
                {
                    service_type[dual_sys] = IDLE_DISPLAY_LIMITED;
                }
                break;
            default:
                service_type[dual_sys] = IDLE_DISPLAY_LIMITED;
                break;
        }

    }

    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MN_DUAL_SYS_MAX; dual_sys++)
    {
        if(IDLE_DISPLAY_LIMITED == service_type[dual_sys])
        {
            MMI_GetLabelTextByLang(STXT_EMERGENCY_ONLY, network_str_ptr);
            SCI_TRACE_LOW("[idle]: MMIIDLE_GetSimAndServiceString(): Emergency call only");
            return IDLE_DISPLAY_LIMITED;
        }
    }

    MMI_GetLabelTextByLang(STR_SIM_NO_SERVICE_EXT01, network_str_ptr);
    SCI_TRACE_LOW("[idle]: MMIIDLE_GetSimAndServiceString(): NO SERVICE");
    
    return IDLE_DISPLAY_NO_SERVICE;
}


/*****************************************************************************/
//  Discription: set ps fast sleep timer
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
PUBLIC void MMIIDLE_SetPsFastSleepTimer(uint16 time_out)
{
#ifndef WIN32
#ifdef MMI_COMPILE_E
    extern void at_set_no_data_time(uint16 time);

    SCI_TRACE_LOW("[idle]:MMIIDLE_SetPsFastSleepTimer time_out(%d)", time_out);
    at_set_no_data_time(time_out);
#endif
#endif
}




#ifdef BROWSER_SUPPORT
PUBLIC void MMIIDLE_StartBrowser (void)
{
    MMIBROWSER_ENTRY_PARAM entry_param = {0}; 
    MMI_MULTI_VAR_URL_T operator_url={0};

    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.is_browser_type_customized = FALSE;

    //Get url from NDT
    if(MMI_MULTIVAR_ReadCurHomePage(&operator_url))
    {
        entry_param.url_ptr = (char*)(operator_url.url); 
        entry_param.type = MMIBROWSER_ACCESS_URL;
        //check length, it should between 0<len<128
        if (strlen(entry_param.url_ptr) ==0 )
        {
            entry_param.type = MMIBROWSER_ACCESS_MAINMENU;  
        }
        else if (strlen(entry_param.url_ptr) >127) //Max len>127,shorten it.
        {
            operator_url.url[127]='\0';
        }
    }
    else  //error, no homepage
    {
        entry_param.type = MMIBROWSER_ACCESS_MAINMENU;  
    }
 
    MMIAPIBROWSER_Entry (&entry_param);
}
#endif

extern void MMIALM_AppEntry(void *param1, void *param2);
extern void MMICALEND_AppEntry(void *param1, void *param2);
extern void  MMIAPISMS_EnterSmsChatWin(void *param1, void *param2);
extern void  MMIAPISMS_AnswerIdleMsg(void *param1, void *param2);
extern BOOLEAN CustomShowCurrentCallStatus(IDLE_APP_PARA_T param1, IDLE_APP_PARA_T param2);
extern BOOLEAN CustomShowCallLogWin(IDLE_APP_PARA_T param1, IDLE_APP_PARA_T param2);
extern BOOLEAN MMICC_CallMissedCall(IDLE_APP_PARA_T param1, IDLE_APP_PARA_T param2);
#ifdef MMIWNS_SUPPORT
extern void MMIWNS_AppEntry(void *param1, void *param2);
#endif
PUBLIC IDLE_APP_ENTRY_FUNC_T MMIIDLE_GetPushMsgEntryFunc(IDLE_APP_ENTRY_E enty)
{
    switch(enty)
    {
    case IDLE_APP_ENTRY_SMS:
#ifdef SMS_CHAT_SUPPORT
        return MMIAPISMS_EnterSmsChatWin;
#else
        return NULL;
#endif
    case IDLE_APP_ENTRY_SMS_ANSWER:
        return MMIAPISMS_AnswerIdleMsg;
	 case IDLE_APP_ENTRY_ALARM:
	     return MMIALM_AppEntry;
	 case IDLE_APP_ENTRY_CALENDER:
	     return MMICALEND_AppEntry;
	 case IDLE_APP_ENTRY_CALLLOG:
	     return CustomShowCallLogWin;
	 case IDLE_APP_ENTRY_CALLMISSED:
	     return MMICC_CallMissedCall;
	case IDLE_APP_ENTRY_CURCALL:
	     return CustomShowCurrentCallStatus;
	case IDLE_APP_ENTRY_WNS:
#ifdef MMIWNS_SUPPORT
	     return MMIWNS_AppEntry;
#else
	     return NULL;
#endif
    }
    return NULL;
}

PUBLIC void MMIIDLE_ExecutePushMsgEntryFunc(IDLE_APP_ENTRY_E enty, void *para1, void *para2)
{
    IDLE_APP_ENTRY_FUNC_T _func = MMIIDLE_GetPushMsgEntryFunc(enty);
    if (_func != NULL)
    {
        _func(para1, para2);	
    }
}





/*Edit by script, ignore 1 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
