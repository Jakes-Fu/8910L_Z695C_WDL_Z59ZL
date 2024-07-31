/****************************************************************************
** File Name:      mmidatacounter_period.c                            
** Author:                                                                  
** Date:            2015/08/24
** Description:    This file is used to process msg to main window of CALCULATION
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
****************************************************************************/
#define _MMIDATACOUNTER_DATA_C_  

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_idle_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "mmk_kbd.h"
#include "guilcd.h"
#include "window_parse.h"
#include "mmi_image.h"
#include "guicommon.h"
#include "mmi_text.h"
#include "tb_dal.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "guimenu.h"

#include "guimsgbox.h"
#include "guilabel.h"
#include "mmk_timer.h"
#include "mmi_default.h"
#include "guibutton.h"
#include "guistatusbar.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmk_msg.h"
#include "guiwin.h"
#include "guimenu.h"
#include "guitext.h"
#include "ctrlform_export.h"

#include "mmi_resource.h"
#include "guistring.h"
#include "mmipdp_export.h"
#include "guiform.h"

#include "datacounter_text.h"
#include "datacounter_menutable.h"
#include "datacounter_internal.h"
#include "datacounter_id.h"
#include "mmiphone_export.h"
#include "mmiphone_internal.h"
#include "mmidatacounter_nv.h"
#include "mmicalendar_export.h"
#include "mmi_solarlunar.h"
#include "mmiacc_position.h"
#include "datacounter_image.h"

#define LINENUM                         6       //日历上显示日期的行数
#define COLNUM                          WEEKDAY_NUM       //日历上显示日期的列数
#define NUM_LENGTH                      10//5

LOCAL MN_DUAL_SYS_E m_period_simId = MMI_SmartDS_SIM_ID_1;

LOCAL const uint8 dataCounter_s_gui_calendar_month_day[MONTHNUM] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
LOCAL const uint8 dataCounter_s_gui_calendar_month_day_leap[MONTHNUM] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

LOCAL const uint8 dataCounter_s_gui_calendar_first_day[MONTHNUM] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
LOCAL const uint8 dataCounter_s_gui_calendar_first_day_leap[MONTHNUM] = {0, 3, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6};
MMI_CALENDAR_T dataCounter_g_calendar_show = {0};
LOCAL BOOLEAN MMIPEROID_GetPeroidValue(MN_DUAL_SYS_E simId);
LOCAL MMI_RESULT_E HandleDataCounterPeroidWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param);
LOCAL uint8 DataCounter_GetWeekday(
                          uint16        year,
                          uint8         month,
                          uint8         day
                          );

LOCAL void DataCounter_GetPosByDate( 
                        MMI_CALENDAR_T*    calendar_ctrl_ptr,
                        uint16                  year,
                        uint8                   month,
                        uint8                   day,
                        uint32*                 row_ptr,
                        uint32*                 col_ptr
                        );
LOCAL void DataCounter_DisplayDate(GUIOWNDRAW_INFO_T *owner_draw_ptr);
LOCAL void DataCounter_DrawMonthDates(
                          MMI_CALENDAR_T*    calendar_ctrl_ptr,
                          BOOLEAN isDrawAll
                          );

LOCAL void* DataCounterCALENDAR_Init(
                              GUI_RECT_T rect //日历的显示区域
                              );

LOCAL void DataCounterCALENDAR_SetParam(     //返回PNULL，表示初始化失败，不等于PNULL，表示初始化成功。
                                  MMI_CALENDAR_T       *calendar_ptr,
                                  uint16 			        solar_year,	    //阳历年		
                                  uint8 			        solar_month,	//阳历月
                                  uint8			            solar_date	    //阳历日
                                  );

LOCAL void DrawBackground(MMI_WIN_ID_T win_id, GUI_RECT_T rect, GUI_COLOR_T color);

LOCAL GUI_COLOR_T m_color = MMI_WHITE_COLOR;
LOCAL uint16 period_text_h = 0;

WINDOW_TABLE( DATA_COUNTER_WIN_PEROID_TAB ) = 
{
	WIN_STATUSBAR,
	WIN_ID(MMIACC_DATA_COUNTER_WIN_PEROID_ID),
	WIN_FUNC((uint32) HandleDataCounterPeroidWinMsg),
	WIN_STYLE(WS_HAS_TRANSPARENT),
	WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, STXT_RETURN),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMI_DATACOUNTER_PEROID_FORM_CTRL_ID),
	CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMI_DATACOUNTER_PEROID_SET_CTRL_ID, MMI_DATACOUNTER_PEROID_FORM_CTRL_ID),
	CHILD_TEXT_CTRL(FALSE, MMI_DATACOUNTER_PEROID_LABEL, MMI_DATACOUNTER_PEROID_FORM_CTRL_ID),
	CHILD_OWNDRAW_CTRL(TRUE, MMI_DATACOUNTER_PEROID_DATE, MMI_DATACOUNTER_PEROID_FORM_CTRL_ID, DataCounter_DisplayDate),
	END_WIN
};

//Calendar
LOCAL void DataCounter_DisplayDate(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    uint16 lcd_height = 0;
    MMI_DATACOUNTER_DATE_NV_E date_nv_e = {0};
    GUI_COLOR_T color = MMI_WHITE_COLOR;
    MMI_CALENDAR_T *calendar_ctrl_ptr = &dataCounter_g_calendar_show;
    SCI_DATE_T date = {0};
    GUI_RECT_T cal_rect = {0};
    GUI_RECT_T client_rect= MMITHEME_GetClientRectEx(calendar_ctrl_ptr->win_id);

    if(period_text_h > 1)
    	client_rect.top = 100;
    else
		client_rect.top = 80;
	
    if ( m_period_simId == MMI_SmartDS_SIM_ID_1 )
    {
        MMINV_READ(MMI_DATACOUNTER_SIM1_SAVE_SELECT_DATE, &date_nv_e, return_value);

        if ( return_value == MN_RETURN_FAILURE )
        {
            SCI_MEMSET(&date_nv_e, 0, sizeof(date_nv_e));
            MMINV_WRITE(MMI_DATACOUNTER_SIM1_SAVE_SELECT_DATE, &date_nv_e);
        }
    }
    else if( m_period_simId == MMI_SmartDS_SIM_ID_2 )
    {
        MMINV_READ(MMI_DATACOUNTER_SIM2_SAVE_SELECT_DATE, &date_nv_e, return_value);
        if ( return_value == MN_RETURN_FAILURE )
        {
            SCI_MEMSET(&date_nv_e, 0, sizeof(date_nv_e));
            MMINV_WRITE(MMI_DATACOUNTER_SIM2_SAVE_SELECT_DATE, &date_nv_e);
        }
    }

    if ( date_nv_e.date.mday > 31 || date_nv_e.date.mday <= 0)
    {
        date_nv_e.date.mday = 0;
        TM_GetSysDate(&date);
        date.mday = 1;//reset day to 1
    }
    else
    {
        date.mday = date_nv_e.date.mday;
        date.mon = date_nv_e.date.mon;
        date.year = date_nv_e.date.year;
    }

    if ( calendar_ctrl_ptr->solar_date == 0 && 
        calendar_ctrl_ptr->solar_month == 0 && 
        calendar_ctrl_ptr->solar_year == 0 )
    {
        DataCounterCALENDAR_SetParam(calendar_ctrl_ptr, date.year, date.mon, date.mday);
    }

    DrawBackground(owner_draw_ptr->win_handle, client_rect, MMI_BLACK_COLOR);
    DataCounter_DrawMonthDates(calendar_ctrl_ptr, TRUE);
}

uint8 DataCounterCALENDAR_GetMonthday(
                            uint16      year,
                            uint8       month
                            )
{
	uint8 ret_val = 0;

    if (month == 0 || month > MONTHNUM)
    {
        //SCI_TRACE_LOW:"MMICALENDAR_GetMonthday error month=%d"
        return 0;
    }
    if (MMIAPICOM_IsLeapYear(year))
    {
        ret_val = dataCounter_s_gui_calendar_month_day_leap[month - 1];
    }
    else
    {
        ret_val  = dataCounter_s_gui_calendar_month_day[month - 1];
    }

    return ret_val;
}


LOCAL GUI_RECT_T DataCounterGetDateRect(
                             MMI_CALENDAR_T* calendar_ctrl_ptr,
                             uint32               row,
                             uint32               col
                             )
{
    GUI_RECT_T rect = {0};
    int16 istyle_height=0;
    int16 istyle_week_height=0;

    if( GUI_CALENDAR_MONTH_VIEW == calendar_ctrl_ptr->view_type )
    { 
        if(row >= LINENUM || col >= COLNUM)
        {
            return rect;
        }
        
		if(!MMITHEME_IsMainScreenLandscape())
		{
		    rect.left   = CALENDAR_MONTH_ITEM0_LEFT_V+col*CALENDAR_BLOCK_WIDTH_V;
		    rect.top    = 110+row*31;

		    rect.right  = rect.left + CALENDAR_BLOCK_WIDTH_V - 1;
		    rect.bottom = rect.top + 31 - 1;
		}
		else
		{
#if defined(MMI_ISTYLE_SUPPORT)		
            if(MMITHEME_IsIstyle())
            {
                istyle_height=CALENDAR_ISSTYLE_DATE_HEIGHT_OFFSET_H;
            }
            else
#endif				
            {
                istyle_height=CALENDAR_BLOCK_HEIGHT_H;
            }
		    rect.left   = CALENDAR_MONTH_ITEM0_LEFT_H+col*CALENDAR_BLOCK_WIDTH_H;
		    rect.top    = CALENDAR_MONTH_LINE0_TOP_H+row*istyle_height;/*lint !e737*/

		    rect.right  = rect.left + CALENDAR_BLOCK_WIDTH_H - 1;
		    rect.bottom = rect.top + istyle_height  - 1;
		}
    }
    else
    {
        if(!MMITHEME_IsMainScreenLandscape())
        {
#if defined(MMI_ISTYLE_SUPPORT)        
            if(MMITHEME_IsIstyle())
            {
                istyle_height=CALENDAR_ISSTYLE_WEEKLY_DATE_Y_V;
                istyle_week_height=CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_V;
            }
            else
#endif				
            {
                istyle_height=CALENDAR_WEEKLY_DATE_Y_V;
                istyle_week_height=CALENDAR_WEEKLY_SELECT_HEIGHT_V;
            }
            rect.left   = CALENDAR_BLOCK_WIDTH_V;
            rect.top    = (uint16)istyle_height + istyle_week_height * row;/*lint !e737*/
            rect.right  = rect.left + CALENDAR_BLOCK_WIDTH_V - 1;
		    rect.bottom = rect.top + istyle_week_height  - 1;
        }
        else
        {
#if defined(MMI_ISTYLE_SUPPORT)        
            if(MMITHEME_IsIstyle())
            {
                istyle_height=CALENDAR_ISSTYLE_WEEKLY_DATE_Y_H;
                istyle_week_height=CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_H;
            }
            else
#endif				
            {
                istyle_height=CALENDAR_WEEKLY_DATE_Y_H;
                istyle_week_height=CALENDAR_WEEKLY_SELECT_HEIGHT_H;
            }
            rect.left   = CALENDAR_BLOCK_WIDTH_H;
            rect.top    = (uint16)istyle_height + istyle_week_height * row;/*lint !e737*/
            rect.right  = rect.left + CALENDAR_BLOCK_WIDTH_H - 1;
		    rect.bottom = rect.top + istyle_week_height  - 1;
        }
    }
#if defined(MMI_ISTYLE_SUPPORT)
    if(MMITHEME_IsIstyle())
    {
        rect.top+=MMITHEME_GetWinTitleHeight();
        rect.bottom+=MMITHEME_GetWinTitleHeight();
    }
#endif
    return rect;
}

LOCAL MMI_STRING_T DataCounter_CreateNumString(
                                   uint16 num
                                   )
{
    wchar*       num_ptr = SCI_ALLOC_APP( (NUM_LENGTH+1)*sizeof(wchar) );
    char         buf[20] = {0};
    
    MMI_STRING_T num_string = {0};
    MMIAPICOM_Int2Str(num,buf,20); /*lint !e64*/
    MMI_STRNTOWSTR(num_ptr,NUM_LENGTH,buf,20,NUM_LENGTH);/*lint !e64*/
    num_string.wstr_ptr = num_ptr;
    num_string.wstr_len = MMIAPICOM_Wstrlen(num_string.wstr_ptr);

    return num_string;
}

LOCAL __inline void SetCalendarTextStyle(
                                         GUISTR_STYLE_T *style_ptr,
                                         GUI_ALIGN_E align,
                                         GUI_FONT_T  font,
                                         GUI_COLOR_T font_color
                                         )
{
    //SCI_ASSERT(PNULL != style_ptr);/*assert verified*/
    if(PNULL == style_ptr)
    {
        return;
    }
    
    style_ptr->align = align;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
    style_ptr->line_space = 0;
}

LOCAL BOOLEAN DataCounterDrawMonthDate(
                            MMI_CALENDAR_T* calendar_ctrl_ptr,
                            uint16               year,
                            uint8                month,
                            uint8                day,
                            uint8                month_day,
                            GUI_RECT_T*    rect_ptr,
                            BOOLEAN              is_highlight
                            )
{
    BOOLEAN result = FALSE;
//     MMI_IMAGE_ID_T	    bmp_id = IMAGE_NULL;
    SCI_DATE_T          today_t = {0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    GUI_LCD_DEV_INFO *lcd_info_ptr=PNULL;

#ifdef MMI_GUI_STYLE_MINISCREEN
        GUI_RECT_T rect_day = {0};
#endif

    lcd_info_ptr=MMITHEME_GetDefaultLcdDev();

    if(PNULL == calendar_ctrl_ptr || PNULL == rect_ptr)
    {
        return FALSE;
    }
    
    if ( day <= month_day )
    {
       GUI_COLOR_T color = m_color;
        MMI_STRING_T num_string = DataCounter_CreateNumString( day );
        
        if ( is_highlight )
        {

            //color = GUI_RGB2RGB565(255, 255, 255);
            if ( MMK_IsActiveCtrl(MMI_DATACOUNTER_PEROID_DATE) )
            	{

			GUIRES_DisplayImg(PNULL,
					rect_ptr,
					PNULL,
					calendar_ctrl_ptr->win_id,
					IMAGE_FOCUSED_TODAY_RED,
					lcd_info_ptr);
            	}
		else
		{
		      if ( MMIPEROID_GetPeroidValue(m_period_simId) == TRUE )
		      	{
			GUIRES_DisplayImg(PNULL,
					rect_ptr,
					PNULL,
					calendar_ctrl_ptr->win_id,
					IMAGE_FOCUSED_RED_2,
					lcd_info_ptr);
		      	}
		}
           
        }
        else
        {
        	;

	}
		
        SetCalendarTextStyle(&text_style, ALIGN_HVMIDDLE, CALENDAR_DATE_FONT, color);
        // display

       GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)lcd_info_ptr,
            (const GUI_RECT_T      *)rect_ptr,
            (const GUI_RECT_T      *)rect_ptr,
            (const MMI_STRING_T    *)&num_string,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );

        MMIAPICOM_DestroyString( num_string );

        result = TRUE;
    }
    
    return result;
}

LOCAL void DrawBackgroundByRect(
                                const GUI_RECT_T*        rect_ptr,
                                GUI_COLOR_T color
                                )
{
   GUI_LCD_DEV_INFO calendar_lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    LCD_FillRect(&calendar_lcd_dev_info,*rect_ptr, color);
}

LOCAL void DrawBackground(MMI_WIN_ID_T win_id, GUI_RECT_T rect, GUI_COLOR_T color)
{
    //GUI_RECT_T rect = {0};//MMITHEME_GetFullScreenRect();
    //uint32 statusBar_h=MMITHEME_GetStatusBarHeight();
    //GUI_RECT_T client_rect= MMITHEME_GetClientRectEx(win_id);
    DrawBackgroundByRect(&rect, color);
    
}

LOCAL void DataCounter_DrawMonthDates(
                          MMI_CALENDAR_T*    calendar_ctrl_ptr,
                          BOOLEAN isDrawAll
                          )
{
    uint8 day = 0;
    uint16 colum = 0;
    uint16 row_ = 0;
    GUI_RECT_T rect      = {0};
    uint32     row       = 0;
    uint32     col       = 0;
    uint8      month_day = 0;//coverity12688

    if(PNULL == calendar_ctrl_ptr)
    {
        return;
		
    }
	if ( isDrawAll )
	{
	    calendar_ctrl_ptr->redraw_mode = GUI_CALENDAR_REDRAW_ALL;
	}
    month_day = DataCounterCALENDAR_GetMonthday( calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month );
     if( GUI_CALENDAR_REDRAW_PART <= calendar_ctrl_ptr->redraw_mode )
    {   
        if( GUI_CALENDAR_REDRAW_PART == calendar_ctrl_ptr->redraw_mode )
        {
            if( calendar_ctrl_ptr->pre_date != calendar_ctrl_ptr->solar_date )
            {
                DataCounter_GetPosByDate( calendar_ctrl_ptr, calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month,
                    calendar_ctrl_ptr->solar_date, &row, &col );
                
                rect = DataCounterGetDateRect( calendar_ctrl_ptr, row, col );
                DrawBackgroundByRect(&rect, MMI_BLACK_COLOR);
                DataCounterDrawMonthDate( calendar_ctrl_ptr, calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, 
                    calendar_ctrl_ptr->solar_date, month_day, &rect, TRUE );
                
                DataCounter_GetPosByDate( calendar_ctrl_ptr, calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month,
                    calendar_ctrl_ptr->pre_date, &row, &col );
                rect = DataCounterGetDateRect( calendar_ctrl_ptr, row, col );
                DrawBackgroundByRect(&rect, MMI_BLACK_COLOR);
                DataCounterDrawMonthDate( calendar_ctrl_ptr, calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, 
                    calendar_ctrl_ptr->pre_date, month_day, &rect, FALSE );
            }
        }
       else
        {
	    for( day = 1; day <= month_day; day++ )
	     {
	           DataCounter_GetPosByDate( calendar_ctrl_ptr, calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, day, &row, &col );
	            rect = DataCounterGetDateRect( calendar_ctrl_ptr, row, col );
	            DataCounterDrawMonthDate( calendar_ctrl_ptr, calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, 
	                    day, month_day, &rect, (BOOLEAN)( day == calendar_ctrl_ptr->solar_date ) );
	     }
        }
     	}

}

LOCAL void DataCounterCALENDAR_SetParam(     //返回PNULL，表示初始化失败，不等于PNULL，表示初始化成功。
                                  MMI_CALENDAR_T       *calendar_ptr,
                                  uint16 			        solar_year,	    //阳历年		
                                  uint8 			        solar_month,	//阳历月
                                  uint8			            solar_date	    //阳历日
                                  )
{
    calendar_ptr->pre_year  = calendar_ptr->solar_year  = solar_year;       //年
    calendar_ptr->pre_month = calendar_ptr->solar_month = solar_month;      //月
    calendar_ptr->pre_date  = calendar_ptr->solar_date  = solar_date;       //日
}

LOCAL uint8 DataCounter_GetWeekday(
                          uint16        year,
                          uint8         month,
                          uint8         day
                          )
{
    uint8   ret_val = 0;
    uint16  year_plus = 0;   
    year_plus = (uint16)((year-1) + (year-1)/4 - (year-1)/100 + (year-1)/400);

    if (MMIAPICOM_IsLeapYear(year))
    {
	    ret_val = (uint8)(( day + dataCounter_s_gui_calendar_first_day_leap[month - 1] + year_plus ) % WEEKDAY_NUM);
    }
    else
    {
        ret_val = (uint8)(( day + dataCounter_s_gui_calendar_first_day[month - 1] + year_plus ) % WEEKDAY_NUM);
    }

    return ret_val;
}

LOCAL void DataCounter_GetPosByDate( 
                        MMI_CALENDAR_T*    calendar_ctrl_ptr,
                        uint16                  year,
                        uint8                   month,
                        uint8                   day,
                        uint32*                 row_ptr,
                        uint32*                 col_ptr
                        )
{
    uint32 offset;
    if(PNULL == calendar_ctrl_ptr || PNULL == row_ptr || PNULL == col_ptr)
    {
        return;
    }
    
    offset = day - 1;//( WEEKDAY_NUM + DataCounter_GetWeekday( year, month, 1 ) - calendar_ctrl_ptr->first_day_of_week ) % WEEKDAY_NUM + day - 1;

    if( GUI_CALENDAR_MONTH_VIEW == calendar_ctrl_ptr->view_type )
    {
        *row_ptr = offset / WEEKDAY_NUM;
        *col_ptr = offset % WEEKDAY_NUM;
    }
    else
    {
        *row_ptr = offset % WEEKDAY_NUM;
        *col_ptr = 0;
    }
}

LOCAL void* DataCounterCALENDAR_Init(
                              GUI_RECT_T rect //日历的显示区域
                              )
{
    MMI_CALENDAR_T* calendar_ptr = &dataCounter_g_calendar_show;
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
    MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;
    MMIAPISET_GetLanguageType(&lang_type);
#endif
    SCI_MEMSET(calendar_ptr, 0x00, sizeof(MMI_CALENDAR_T));

    calendar_ptr->rect          = rect;     //控件的显示区域
    calendar_ptr->solar_year    = 2000;     //年
    calendar_ptr->solar_month   = 1;        //月
    calendar_ptr->solar_date    = 1;        //日

    calendar_ptr->first_day_of_week = MMIAPISET_GetFirstDayOfWeek();

#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
    if( MMISET_LANGUAGE_SIMP_CHINESE == lang_type
        || MMISET_LANGUAGE_TRAD_CHINESE == lang_type
        //|| MMISET_LANGUAGE_ENGLISH == lang_type
        )
    {
        //calendar_ptr->is_need_lunar_date = TRUE;
    }
#endif
    return (void*)calendar_ptr;
}

LOCAL void MovePrevDay(
                         CALE_SOLAR_DATE_T  date_in,
                         CALE_SOLAR_DATE_T  *date_out_ptr
                       )
{
    uint8   date     = date_in.day;

    //SCI_ASSERT(NULL != date_out_ptr);/*assert verified*/
    if(PNULL == date_out_ptr)
    {
        return;
    }
    
    if (1 != date)   //不是第一天，移到前一天
    {
	    date--;
    }
    
    date_out_ptr->day = date;

    return;
}

/*****************************************************************************/
//    Description : move one day forward
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void MoveNextDay(
                         CALE_SOLAR_DATE_T  date_in,
                         CALE_SOLAR_DATE_T  *date_out_ptr
                       )
{
    uint16      year   = date_in.year;
    uint8       month   = date_in.month;
    uint8       date    = date_in.day;

    //SCI_ASSERT(NULL != date_out_ptr);/*assert verified*/
    if(PNULL == date_out_ptr)
    {
        return;
    }
    
    if (MMICALENDAR_GetMonthday(year, month) != date)   //不是最后一天, 移到后一天
    {
	    date++;
    }
    
    date_out_ptr->day = date;

    return;
}

/*****************************************************************************/
//    Description : move one week backward
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void MovePrevWeek(
                         CALE_SOLAR_DATE_T  date_in,
                         CALE_SOLAR_DATE_T  *date_out_ptr
                         )
{
    uint16      year   = date_in.year;
    uint8       month   = date_in.month;
    uint8       date    = date_in.day;

    //SCI_ASSERT(NULL != date_out_ptr);/*assert verified*/
    if(PNULL == date_out_ptr)
    {
        return;
    }
    
    if (WEEKDAY_NUM < date)   //大于7，前移7天
    {
	    date -= WEEKDAY_NUM;
    }
    else
    {
	  GUIAPICTRL_SetState(MMI_DATACOUNTER_PEROID_SET_CTRL_ID, GUICTRL_STATE_DISABLE_KEY|GUICTRL_STATE_DISABLE_ACTIVE, FALSE);
	  MMK_SetAtvCtrl(dataCounter_g_calendar_show.win_id, MMI_DATACOUNTER_PEROID_SET_CTRL_ID);
    }
	
    
    date_out_ptr->day = date;

    return;
}

/*****************************************************************************/
//    Description : move one week forward
//    Global resource dependence : 
//    Author: zhanglu
//    Note:
/*****************************************************************************/
LOCAL void MoveNextWeek(
                         CALE_SOLAR_DATE_T  date_in,
                         CALE_SOLAR_DATE_T  *date_out_ptr
                           )
{
    uint8       date    = date_in.day;
    uint16      year   = date_in.year;
    uint8       month   = date_in.month;	
    uint8       monthday = 0;


    //SCI_ASSERT(NULL != date_out_ptr);/*assert verified*/
    if(PNULL == date_out_ptr)
    {
        return;
    }
    
    monthday = MMICALENDAR_GetMonthday(year, month);
    if (monthday >= (date + WEEKDAY_NUM))   //当前月
    {
	    date += WEEKDAY_NUM;
    }
    date_out_ptr->day = date;

}

LOCAL void HandleCalendarKey(
                             MMI_CALENDAR_T* 	calendar_ctrl_ptr,
                             CALENDAR_MOVE_MODE_E 	mode
                             )
{
    CALE_SOLAR_DATE_T   date = {0};

    //SCI_ASSERT(PNULL != calendar_ctrl_ptr);/*assert verified*/
    if(NULL == calendar_ctrl_ptr)
    {
        return;
    }

    calendar_ctrl_ptr->pre_year  = date.year  = calendar_ctrl_ptr->solar_year;
    calendar_ctrl_ptr->pre_month = date.month = calendar_ctrl_ptr->solar_month;
    calendar_ctrl_ptr->pre_date  = date.day = calendar_ctrl_ptr->solar_date;

    switch( mode )
    {
    case  DATACOUNTER_MOVE_PREV_WEEK:     //移到上一行
        MovePrevWeek(date, &date);
        break;

    case  DATACOUNTER_MOVE_NEXT_WEEK:   //移到下一行
        MoveNextWeek(date, &date);
        break;
    
    case  DATACOUNTER_MOVE_PREV_DAY:   //移到上一天
        MovePrevDay(date, &date);
        break;
    
    case  DATACOUNTER_MOVE_NEXT_DAY:  //移到下一天
        MoveNextDay(date, &date);
	    break;
    default:
		break;
    }
	
    calendar_ctrl_ptr->solar_year = date.year;
    calendar_ctrl_ptr->solar_month = date.month;
    calendar_ctrl_ptr->solar_date = date.day;
    if(2==calendar_ctrl_ptr->solar_month )
   {
      if((!GUI_IsLeapYear(date.year))&&calendar_ctrl_ptr->solar_date>=29 )
      {
          calendar_ctrl_ptr->solar_date =28;
      }
   }

    if( GUI_CALENDAR_MONTH_VIEW == calendar_ctrl_ptr->view_type )
    {
       calendar_ctrl_ptr->redraw_mode = GUI_CALENDAR_REDRAW_PART;
    }
}

//End

LOCAL void InitPeroidCtrl(MMI_TEXT_ID_T textID)
{	
    MMI_CTRL_ID_T  ctrl_id = MMI_DATACOUNTER_PEROID_SET_CTRL_ID;
    GUILIST_ITEM_T  item_t = {0};
    MMI_STRING_T text = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    item_t.item_data_ptr = &item_data;
    GUILIST_SetMaxItem(ctrl_id, 1, FALSE );
    GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);

    item_t.item_style = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
    item_t.user_data =  MMI_DATACOUNTER_PEROID_SET_LIST;
    item_data.item_content[0].item_data_type      = GUIITEM_DATA_TEXT_ID;              
    item_data.item_content[0].item_data.text_id = textID;
    
    GUILIST_InsertItem(ctrl_id, &item_t, 0);

}

LOCAL void UpdatePeroidCtrl(MMI_TEXT_ID_T textID,  uint16 index)
{
    MMI_CTRL_ID_T  ctrl_id = MMI_DATACOUNTER_PEROID_SET_CTRL_ID;
    GUILIST_ITEM_T  item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    item_t.item_data_ptr = &item_data;
    item_t.item_style = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
    item_t.user_data =  MMI_DATACOUNTER_PEROID_SET_LIST;
    item_data.item_content[0].item_data_type      = GUIITEM_DATA_TEXT_ID;              
    item_data.item_content[0].item_data.text_id = textID;

    GUILIST_ReplaceItem(ctrl_id, &item_t, index);
}

LOCAL void DrawPeroidInfo(uint16 index)
{
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_POINT_T point = {0};
	BOOLEAN bReturn = FALSE;
	GUISTR_STYLE_T text_style = {0};
	GUI_RECT_T rect_ptr = {0};
	GUISTR_STATE_T state = GUISTR_STATE_BOLD;
	MMI_CTRL_ID_T  ctrl_id = MMI_DATACOUNTER_PEROID_SET_CTRL_ID;
	MMI_STRING_T text = {0};
	
	MMI_GetLabelTextByLang(STR_COUN_RESET_DATE_EXT01, &text);

	bReturn = GUILIST_GetItemRect(ctrl_id, index, &rect_ptr);
	
	if ( bReturn )
	{
		point.x = (int16)rect_ptr.left;
        	point.y = (int16)100;
		text_style.font = CAF_FONT_20;
		text_style.bg_color = MMI_BLACK_COLOR;
		text_style.font_color = MMI_WHITE_COLOR;
        	GUISTR_DrawTextToLCDMultiLine( 
                                                            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                                            (const GUI_POINT_T *)&point,
                                                            (const MMI_STRING_T *)&text,
                                                            (GUISTR_STYLE_T *)&text_style,
                                                            state
                                                            );
	}

	
}

LOCAL BOOLEAN MMIPEROID_GetPeroidValue(MN_DUAL_SYS_E simId)
{
	BOOLEAN  is_peroid_reset = DEFAULT_PEROID_VALUE;
    	MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

	if ( simId == MMI_SmartDS_SIM_ID_1 )
   	{
    		MMINV_READ(MMI_DATACOUNTER_SIM1_PEROID_NV_FLAG,&is_peroid_reset,return_value);
	}
	else if( simId == MMI_SmartDS_SIM_ID_2 )
	{
		MMINV_READ(MMI_DATACOUNTER_SIM2_PEROID_NV_FLAG,&is_peroid_reset,return_value);
	}
	
	if (MN_RETURN_SUCCESS != return_value)
    	{	
        	is_peroid_reset = DEFAULT_PEROID_VALUE;
		if ( simId == MMI_SmartDS_SIM_ID_1 )
        	{
        		MMINV_WRITE(MMI_DATACOUNTER_SIM1_PEROID_NV_FLAG,&is_peroid_reset);
		}
		else if( simId == MMI_SmartDS_SIM_ID_2 )
		{
			MMINV_WRITE(MMI_DATACOUNTER_SIM2_PEROID_NV_FLAG,&is_peroid_reset);
		}
    	}
    
    return (is_peroid_reset);

}

LOCAL void MMIPEROID_SetIsPeroidReset(MN_DUAL_SYS_E simId,
                                      BOOLEAN  is_auto_redial
                                      )
{
    if ( simId == MMI_SmartDS_SIM_ID_1 )
   {
    	MMINV_WRITE(MMI_DATACOUNTER_SIM1_PEROID_NV_FLAG,&is_auto_redial);
   }
   else if(simId == MMI_SmartDS_SIM_ID_2)
   {
	MMINV_WRITE(MMI_DATACOUNTER_SIM2_PEROID_NV_FLAG,&is_auto_redial);
   }
}

#define     UPDATE_LIST_SWITCH                   1

LOCAL BOOLEAN bSwitchFlag = FALSE;

LOCAL MMI_RESULT_E HandleDataCounterPeroidWinMsg(
									MMI_WIN_ID_T win_id, 
									MMI_MESSAGE_ID_E msg_id, 
									DPARAM param
									)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMI_DATACOUNTER_PEROID_SET_CTRL_ID;
    MMI_MENU_ID_T menu_id = 0;
    MMI_STRING_T statusBar_str = {0};
    MMI_STRING_T text = {0};
    GUI_BOTH_RECT_T ss = {0};
    GUI_BG_T bg = {0};
    uint16 hor_space = 0;
    uint16 ver_space = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    GUIFORM_CHILD_WIDTH_T   osd_item_width = {0};
    GUI_FONT_T font = CAF_FONT_14;
    GUI_COLOR_T color = MMI_WHITE_COLOR;
    MMI_CALENDAR_T *calendar_ctrl_ptr = &dataCounter_g_calendar_show;
    BOOLEAN is_on;
    //GUI_RECT_T rect = {0};
    GUI_RECT_T cal_rect = {0};
    MMI_DATACOUNTER_DATE_NV_E date_nv_e = {0};
    GUI_FONT_ALL_T FONT_ALL = {0};
    uint16 index = 0;
    
    calendar_ctrl_ptr->win_id = win_id;
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    FONT_ALL.font = CAF_FONT_14;
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            InitPeroidCtrl(STR_COUN_MONTHLY_RESET_EXT01);
            if ( MMIPEROID_GetPeroidValue(m_period_simId)  == DEFAULT_PEROID_VALUE )
            {
                m_color = FONT_ALL.color = MMI_DARK_GRAY_COLOR;
                CTRLLIST_SetToggleState(ctrl_id, 0, FALSE);
            }
            else
            {
                m_color = FONT_ALL.color = MMI_WHITE_COLOR;
                CTRLLIST_SetToggleState(ctrl_id, 0, TRUE);
            }

            osd_item_width.type     = GUIFORM_CHILD_WIDTH_FIXED;
            osd_item_width.add_data = lcd_width;
            GUIFORM_SetChildWidth(MMI_DATACOUNTER_PEROID_FORM_CTRL_ID, MMI_DATACOUNTER_PEROID_LABEL, &osd_item_width);
            GUITEXT_SetFont(MMI_DATACOUNTER_PEROID_LABEL, &font, &m_color);
            GUITEXT_SetMarginEx(MMI_DATACOUNTER_PEROID_LABEL, 4, 0, 2, 2);

            MMI_GetLabelTextByLang(STR_COUN_RESET_DATE_EXT01, &text);
            GUITEXT_SetString(MMI_DATACOUNTER_PEROID_LABEL, text.wstr_ptr, text.wstr_len, TRUE);
	        period_text_h =	GUI_CalculateStringLinesByPixelNum(
					lcd_width,
					text.wstr_ptr,
					text.wstr_len,
					font,
					0,
					TRUE
					);
            //GUIFORM_SetMargin(MMI_DATACOUNTER_PEROID_FORM_CTRL_ID,0);
            GUIFORM_PermitChildBorder(MMI_DATACOUNTER_PEROID_FORM_CTRL_ID, FALSE);
            GUIFORM_PermitChildFont(MMI_DATACOUNTER_PEROID_FORM_CTRL_ID,FALSE);

            GUIFORM_SetChildTheme(MMI_DATACOUNTER_PEROID_FORM_CTRL_ID, PNULL, PNULL, &FONT_ALL);
            
            bg.bg_type = GUI_BG_COLOR;
            bg.color = MMI_BLACK_COLOR;
            GUIFORM_SetBg(MMI_DATACOUNTER_PEROID_FORM_CTRL_ID, &bg);
            MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_FULL_PAINT:
            MMI_GetLabelTextByLang(STR_COUN_LIST_PERIOD_EXT01, &statusBar_str);
            if(MMIAPICOM_StatusAreaSetViewHeaderinfo(statusBar_str.wstr_ptr, statusBar_str.wstr_len)) 
            {
                GUIWIN_UpdateStb();
            }
        break;	
    case UPDATE_LIST_SWITCH:
            is_on = *(BOOLEAN*)param;
            if ( is_on == DEFAULT_PEROID_VALUE )
            {
                CTRLLIST_SetToggleState(ctrl_id, 0, FALSE);
                //GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_DISABLE_KEY|GUICTRL_STATE_DISABLE_ACTIVE, FALSE);

                m_color = FONT_ALL.color = MMI_DARK_GRAY_COLOR;
            }
            else
            {
                CTRLLIST_SetToggleState(ctrl_id, 0, TRUE);
                m_color = FONT_ALL.color = MMI_WHITE_COLOR;
            }
            GUITEXT_SetFont(MMI_DATACOUNTER_PEROID_LABEL,&font, &m_color);
            //GUIFORM_SetChildTheme(MMI_DATACOUNTER_PEROID_FORM_CTRL_ID, PNULL, PNULL, &FONT_ALL);

            MMK_SendMsg(MMI_DATACOUNTER_PEROID_DATE, MSG_CTL_PAINT, PNULL);
            MMK_SendMsg(MMI_DATACOUNTER_PEROID_LABEL, MSG_CTL_PAINT, PNULL);
        break;
    case MSG_APP_OK:
    break;
    case MSG_CTL_MIDSK:
            if ( MMK_IsActiveCtrl(ctrl_id))
            {
                index = GUILIST_GetCurItemIndex(ctrl_id);
                GUILIST_GetItemData(ctrl_id, index, &menu_id);
                switch( menu_id )
                {
                    case MMI_DATACOUNTER_PEROID_SET_LIST:
                        CTRLLIST_GetToggleState(ctrl_id,index,&is_on);
                        MMIPEROID_SetIsPeroidReset(m_period_simId, is_on);

                        //MMK_PostMsg(win_id, MSG_FULL_PAINT, (DPARAM)&is_on, sizeof(is_on));
                        MMK_SendMsg(win_id, UPDATE_LIST_SWITCH, (DPARAM)&is_on);
                        break;
                    default:
                        break;
                }
            }
            else if( MMK_IsActiveCtrl(MMI_DATACOUNTER_PEROID_DATE) )
            {
                date_nv_e.date.year = calendar_ctrl_ptr->solar_year;
                date_nv_e.date.mon = calendar_ctrl_ptr->solar_month;
                date_nv_e.date.mday = calendar_ctrl_ptr->solar_date;
                date_nv_e.dataResetFlag = 0;
                if ( m_period_simId == MMI_SmartDS_SIM_ID_1 )
                {
                    MMINV_WRITE(MMI_DATACOUNTER_SIM1_SAVE_SELECT_DATE, &date_nv_e);
                }
                else if( m_period_simId == MMI_SmartDS_SIM_ID_2 )
                {
                    MMINV_WRITE(MMI_DATACOUNTER_SIM2_SAVE_SELECT_DATE, &date_nv_e);
                }

                ClearDataCounterPartRecord(4, m_period_simId);
                ResetCallShowJustOne(m_period_simId, 0, FALSE);//清除call第一次弹框提示
                ResetDataShowJustOne(m_period_simId, 0, FALSE);
                ResetSmsShowJustOne(m_period_simId, 0, FALSE);
                ResetMmsShowJustOne(m_period_simId, 0, FALSE);

                MMK_CloseWin(win_id);
            }
        break;

    case MSG_CTL_OK:	
        break;
    case MSG_NOTIFY_GET_ACTIVE:
            if ( MMK_IsActiveCtrl(MMI_DATACOUNTER_PEROID_DATE))
            {
                GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, STXT_SOFTKEY_SAVE_MK, STXT_RETURN, TRUE);
                //GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_BACK,2,TRUE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, STXT_SOFTKEY_SELECT_MK, STXT_RETURN, TRUE);
                //GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_BACK,2,TRUE);
            }

        break;

    case MSG_NOTIFY_LOSE_ACTIVE:
            if ( MMIPEROID_GetPeroidValue(m_period_simId)  == DEFAULT_PEROID_VALUE )
            {
                if ( MMK_IsActiveCtrl(MMI_DATACOUNTER_PEROID_DATE))
                {
                    GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_DISABLE_KEY|GUICTRL_STATE_DISABLE_ACTIVE, FALSE);
                    MMK_SetAtvCtrl(win_id, ctrl_id);
                }
            }
            else
            {
                if ( MMK_IsActiveCtrl(MMI_DATACOUNTER_PEROID_DATE))
                {
                    bSwitchFlag = TRUE;
                    GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_DISABLE_KEY|GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
                }
            }
        break;

    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
            ;
        break;
    case MSG_KEYUP_UP:
         if ( MMK_IsActiveCtrl(MMI_DATACOUNTER_PEROID_DATE) )
            {
                HandleCalendarKey(calendar_ctrl_ptr, DATACOUNTER_MOVE_PREV_WEEK);
                DataCounter_DrawMonthDates(calendar_ctrl_ptr, FALSE);
            }
        break;
    case MSG_APP_UP:
        /*
        if ( MMK_IsActiveCtrl(MMI_DATACOUNTER_PEROID_DATE) )
            {
                HandleCalendarKey(calendar_ctrl_ptr, DATACOUNTER_MOVE_PREV_WEEK);
                DataCounter_DrawMonthDates(calendar_ctrl_ptr, FALSE);
            }*/
        break;
    case MSG_KEYUP_DOWN:
        if ( bSwitchFlag )
       {
            bSwitchFlag = FALSE;
        }
        else
        {
            if ( MMK_IsActiveCtrl(MMI_DATACOUNTER_PEROID_DATE) )
            {
                HandleCalendarKey(calendar_ctrl_ptr, DATACOUNTER_MOVE_NEXT_WEEK);
                DataCounter_DrawMonthDates(calendar_ctrl_ptr, FALSE);
            }
        }

        break;
    case  MSG_APP_DOWN:
        /*
        if ( MMK_IsActiveCtrl(MMI_DATACOUNTER_PEROID_DATE) )
            {
                HandleCalendarKey(calendar_ctrl_ptr, DATACOUNTER_MOVE_NEXT_WEEK);
                DataCounter_DrawMonthDates(calendar_ctrl_ptr, FALSE);
            }*/
        break;
    case  MSG_APP_LEFT:
            if ( MMK_IsActiveCtrl(MMI_DATACOUNTER_PEROID_DATE) )
            {
                HandleCalendarKey(calendar_ctrl_ptr, DATACOUNTER_MOVE_PREV_DAY);
                DataCounter_DrawMonthDates(calendar_ctrl_ptr, FALSE);
            }
        break;
    case  MSG_APP_RIGHT:
        if ( MMK_IsActiveCtrl(MMI_DATACOUNTER_PEROID_DATE) )
        {
            HandleCalendarKey(calendar_ctrl_ptr, DATACOUNTER_MOVE_NEXT_DAY);
            DataCounter_DrawMonthDates(calendar_ctrl_ptr, FALSE);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
	     period_text_h = 0;
            MMK_CloseWin(win_id);
        break;
    default:
            recode = MMI_RESULT_FALSE;
        break;

    }

    return recode;
}

/*****************************************************************************/
// 	Description : MMIAPIDATACOUNTER_OpenDataWin
//         
//	Global resource dependence : 
//   Author:        zhanglu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIDATACOUNTER_OpenPeroidWin(void* param)
{
    SCI_MEMSET(&dataCounter_g_calendar_show, 0x00, sizeof(dataCounter_g_calendar_show));
    m_period_simId = *((MN_DUAL_SYS_E*)param);
    MMK_CreateWin((uint32*)DATA_COUNTER_WIN_PEROID_TAB,(ADD_DATA)param);
    

}
//#else
/*****************************************************************************/
// 	Description : MMIAPIDATACOUNTER_OpenDataWin
//
//	Global resource dependence :
//   Author:        zhanglu
//	Note:
/*****************************************************************************/
/*
PUBLIC void MMIAPIDATACOUNTER_OpenDataWin(uint32 simcard)
{

}*/
//#endif//DATA_COUNTER_SUPPORT


/*Edit by script, ignore 3 case. Fri Apr 27 09:38:47 2012*/ //IGNORE9527
