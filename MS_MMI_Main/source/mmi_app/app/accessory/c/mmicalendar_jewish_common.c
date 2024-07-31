/*****************************************************************************
** File Name:      mmicalendar_jewish_common.c                               *
** Author:                                                                   *
** Date:           07/24/2006                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe solar lunar calendar func   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/24/2006     liqing.peng      Create
** 08/09/2006     liqing.peng      Modify -- code review
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef MMI_JEWISH_CALENDAR
#ifdef WIN32
#include "std_header.h"
#endif
#include "os_api.h"
#include "guicommon.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_text.h"
#include "mmialarm_text.h"
#include "mmicalendar_jewish_export.h"
//#include "math.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
extern MMI_JEWISH_CALENDAR_T g_current_date;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
//month_list[i]对应每月的天数
LOCAL const uint8 s_gregorian_month_list[GREGORIAN_MONTH_NUM] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
LOCAL BOOLEAN s_is_leap_month=FALSE;

LOCAL const MMI_TEXT_ID_T s_months_leap_text_id[] =
{
    STXT_NISAN,
    STXT_IYAR,
    STXT_SIVAN,
    STXT_TAMUZ,
    STXT_AV,
    STXT_ELUL,
    STXT_TISHREI,
    STXT_CHESHVAN,
    STXT_KISLEV,
    STXT_TEVET,
    STXT_SHVAT,
    STXT_ADAR_A,
    STXT_ADAR_B
};

LOCAL const MMI_TEXT_ID_T s_months_text_id[] =
{
    STXT_NISAN,
    STXT_IYAR,
    STXT_SIVAN,
    STXT_TAMUZ,
    STXT_AV,
    STXT_ELUL,
    STXT_TISHREI,
    STXT_CHESHVAN,
    STXT_KISLEV,
    STXT_TEVET,
    STXT_SHVAT,
    STXT_ADAR,
};

LOCAL const uint16 s_alef_string[]=
{
    0x05D0, 0x05D1, 0x05D2, 0x05D3, 0x05D4, 0x05D5, 0x05D6, 0x05D7, 0x05D8
};

LOCAL const uint16 s_yud_string[]=
{
    0x05D9, 0x05DB, 0x05DC, 0x05DE, 0x05E0, 0x05E1, 0x05E2, 0x05E4, 0x05E6
};

LOCAL const uint16 s_quf_string[]=
{
    0x05E7, 0x05E8, 0x05E9
};

LOCAL const uint16 s_taf_string[]=
{
    0x05EA
};

LOCAL const uint16 s_tu_string[]=
{
    0x05D8, 0x05D5
};

LOCAL const uint16 s_tz_string[]=
{
    0x05D8, 0x05D6
};

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                                 */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//    Description : LongHeshvan
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN LongHeshvan(uint16 year);

/*****************************************************************************/
//    Description : ShortKislev
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN ShortKislev(uint16 year);

/*****************************************************************************/
//    Description : DaysInHebrewYear
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL uint32 DaysInHebrewYear(uint16 year);

/*****************************************************************************/
//    Description : HebrewCalendarElapsedDays
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL uint32 HebrewCalendarElapsedDays(uint16 year);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//    Description : MMICALENDAR_IsHebrewLeapYear
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICALENDAR_IsHebrewLeapYear(uint16 year)
{
    BOOLEAN result = FALSE;

    if ((((year * 7) + 1) % 19) < 7)
    {
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
//    Description : get last month of Jewish year
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMICALENDAR_GetJewishMonthName(
                                                    uint8 month_num,
                                                    uint16 year
                                                    )
{
    if(MMICALENDAR_IsHebrewLeapYear(year))
    {
        return s_months_leap_text_id[month_num - 1];
    }
    else
    {
        return s_months_text_id[month_num - 1];
    }
}

/*****************************************************************************/
//    Description : get last month of Jewish year
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMICALENDAR_GetLastMonthOfJewishYear(uint16 year)
{
    if (MMICALENDAR_IsHebrewLeapYear(year))
    {
        return JEWISH_LEAP_MONTH_NUM;
    }
    else
    {
        return JEWISH_MONTH_NUM;
    }
}

/*****************************************************************************/
//    Description : LongHeshvan
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN LongHeshvan(uint16 year)
{
    if ((DaysInHebrewYear(year) % 10) == 5)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//    Description : ShortKislev
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN ShortKislev(uint16 year)
{
    if ((DaysInHebrewYear(year) % 10) == 3)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//    Description : DaysInHebrewYear
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL uint32 DaysInHebrewYear(uint16 year)
{
    uint32 day = 0;

    day = (HebrewCalendarElapsedDays(year + 1) - HebrewCalendarElapsedDays(year));
    return day;
}

/*****************************************************************************/
//    Description : get last day of Jewish month
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMICALENDAR_GetLastDayOfJewishMonth(uint8 month, uint16 year)
{
    if ((month == 2) || (month == 4) || (month == 6) ||
        (month == 10) || (month == 13))
    {
        return JEWISH_LEAP_MONTH_DAY;
    }
    else if ((month == 12) && (!MMICALENDAR_IsHebrewLeapYear(year))
        || ((month == 8) && (!LongHeshvan(year)))
        || ((month == 9) && (ShortKislev(year))))
    {
        return JEWISH_LEAP_MONTH_DAY;
    }
    else
    {
        return JEWISH_MONTH_DAY;
    }
}

/*****************************************************************************/
//    Description : HebrewCalendarElapsedDays
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL uint32 HebrewCalendarElapsedDays(uint16 year)
{
    uint32 value = 0;
    uint32 parts = 0;
    uint32 alternativeDay = 0;
    uint32 monthsElapsed = 0;
    uint32 partsElapsed = 0;
    uint32 hoursElapsed = 0;
    uint32 day = 0;
    
    /* Months in complete cycles so far */
    value = 235 * ((year - 1) / 19);
    monthsElapsed = value;
    
    /* Regular months in this cycle */
    value = 12 * ((year - 1) % 19);
    monthsElapsed += value;
    
    /* Leap months this cycle */
    value = ((((year - 1) % 19) * 7) + 1) / 19;
    monthsElapsed += value;
    
    partsElapsed = (((monthsElapsed % 1080) * 793) + 204);
    hoursElapsed = (5 + (monthsElapsed * 12) +
        ((monthsElapsed / 1080) * 793) + (partsElapsed / 1080));
    
    /* Conjunction day */
    day = 1 + (29 * monthsElapsed) + (hoursElapsed / 24);
    
    /* Conjunction parts */
    parts = ((hoursElapsed % 24) * 1080) + (partsElapsed % 1080);
    
    /* If new moon is at or after midday, */
    if ((parts >= 19440) ||

        /* ...or is on a Tuesday... */
        (((day % 7) == 2) &&
        /* at 9 hours, 204 parts or later */
        (parts >= 9924) &&
        /* of a common year */
        (!MMICALENDAR_IsHebrewLeapYear(year))) ||
        
        /* ...or is on a Monday at... */
        (((day % 7) == 1) &&
        /* 15 hours, 589 parts or later... */
        (parts >= 16789) &&
        /* at the end of a leap year */
        (MMICALENDAR_IsHebrewLeapYear(year - 1))))
        /* Then postpone Rosh HaShanah one day */
    {
        alternativeDay = day + 1;
    }
    else
    {
        alternativeDay = day;
    }
    
    /* If Rosh HaShanah would occur on Sunday, Wednesday, or Friday*/
    if (((alternativeDay % 7) == 0) ||
        ((alternativeDay % 7) == 3) ||
        ((alternativeDay % 7) == 5))
    {
        /* Then postpone it one (more) day and return */
        alternativeDay++;
    }
    
    return (alternativeDay);
}

/*****************************************************************************/
//    Description : absolute from Jewish date
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMICALENDAR_AbsoluteFromJewishDate(
                                                 uint16 year, 
                                                 uint8 month, 
                                                 uint8 day
                                                 )
{
    uint32 value = 0;
    uint32 returnValue = 0;
    uint8 m = 0;
    uint8 last_month = 0;
    
    /* Days so far this month */
    value = day;
    returnValue = value;
    
    /* If before Tishri */
    if (month < 7)
    {
        /* Then add days in prior months this year before and after Nisan.*/
        last_month = MMICALENDAR_GetLastMonthOfJewishYear(year);
        for (m = 7; m <= last_month; m++)
        {
            value = MMICALENDAR_GetLastDayOfJewishMonth(m, year);
            returnValue += value;
        }
        for (m = 1; m < month; m++)
        {
            value = MMICALENDAR_GetLastDayOfJewishMonth(m, year);
            returnValue += value;
        }
    }
    else
    {
        for (m = 7; m < month; m++)
        {
            value = MMICALENDAR_GetLastDayOfJewishMonth(m, year);
            returnValue += value;
        }
    }
    
    /* Days in prior years */
    value = HebrewCalendarElapsedDays(year);
    returnValue += value;

    /* Days elapsed before absolute date 1 */
    value = 1373429;
    returnValue -= value;

    return (returnValue);
}

/*****************************************************************************/
//    Description : absolute from Jewish date
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC MMISET_DATE_T MMICALENDAR_JewishDateFromAbsolute(uint32 abs_date)
{
    MMISET_DATE_T date = {0};
    uint16 year = 0;
    uint8 month = 0;
    uint8 day = 0;
    uint32 approx = 0;
    uint32 temp = 0;
    
    /* Approximation */
    approx = (abs_date + 1373429) / 366;
    
    /* Search forward from the approximation */
    year = approx;
//     while(abs_date >= temp)
    for(;;)
    {
        temp = MMICALENDAR_AbsoluteFromJewishDate(year+1, 7, 1);

        if (abs_date < temp)
        {
            break;
        }
        
        year++;
    }
    date.date_year = year;

    /* Starting month for search for month */
    temp = MMICALENDAR_AbsoluteFromJewishDate(date.date_year, 1, 1);
    if (abs_date < temp)
    {
        month = 7;
    }
    else
    {
        month = 1;
    }
    
    /* Search forward from either Tishri or Nisan */
    for(;;)
    {
        day = MMICALENDAR_GetLastDayOfJewishMonth(month, date.date_year);

        temp = MMICALENDAR_AbsoluteFromJewishDate(date.date_year, month, day);
        if (abs_date <= temp)
        {
            break;
        }
        month++;
    }
    date.date_month = month;

    /* Calculate the day by subtraction */
    temp = MMICALENDAR_AbsoluteFromJewishDate(date.date_year, date.date_month, 1);
    date.date_day = abs_date - temp + 1;

    return date;
}

/*****************************************************************************/
//    Description : MMICALENDAR_GetWeekdayOfHebrewDate
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMICALENDAR_GetWeekdayOfHebrewDate(
                                                uint8 hebDay, 
                                                uint8 hebMonth, 
                                                uint16 hebYear
                                                )
{
    uint8 weekday = 0;
    uint32 abs_date = 0;
    
    abs_date = MMICALENDAR_AbsoluteFromJewishDate(hebYear, hebMonth, hebDay);
    weekday = abs_date % 7;
    
    return weekday;
}

/*****************************************************************************/
//    Description : MMICALENDAR_GetLastDayOfGregorianMonth
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMICALENDAR_GetLastDayOfGregorianMonth(
                                                    uint8 month, 
                                                    uint16 year
                                                    )
{
    if ((month == 2) 
        && ((year % 4) == 0) 
        && ((year % 400) != 100)
        && ((year % 400) != 200) 
        && ((year % 400) != 300))
    {
        return MMICOM_DATE_LEAP_MONTH_DAY;
    }
    else
    {
        return s_gregorian_month_list[month - 1];
    }
}

/*****************************************************************************/
//    Description : MMICALENDAR_GetWeekDayOfGregorianDate
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMICALENDAR_GetWeekDayOfGregorianDate(uint8 abs_date)
{
    return (abs_date % 7);
}

/*****************************************************************************/
//    Description : MMICALENDAR_AbsoluteFromGregorianDate
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMICALENDAR_AbsoluteFromGregorianDate(
                                                    uint16 year, 
                                                    uint8 month, 
                                                    uint8 day
                                                    )
{
    uint32 value = 0;
    uint8 i = 0;

    /* Days so far this month */
    value = day;

    /* Days in prior months this year */
    for (i = 1; i < month; i++)
    {
        value += MMICALENDAR_GetLastDayOfGregorianMonth(i, year);
    }
    
    /* Days in prior years */
    value = value + (365 * (year - 1));

    /* Julian leap days in prior years ... */
    value = value + ((year - 1) / 4);

    /* ... minus prior century years ... */
    value -= ((year - 1) / 100);

    /* ... plus prior years divisible by 400 */
    value += ((year - 1) / 400);

    return value;
}

/*****************************************************************************/
//    Description : MMICALENDAR_GregorianDateFromAbsolute
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC MMISET_DATE_T MMICALENDAR_GregorianDateFromAbsolute(uint32 abs_date)
{
    MMISET_DATE_T date = {0};
    uint16 year = 0;
    uint8 month = 0;
    uint8 day = 0;
    uint32 approx = 0;
    uint32 temp = 0;

    /* Approximation */
    approx = abs_date / 366;
    
    /* Search forward from the approximation */
    year = approx;
    for(;;)
    {
        temp = MMICALENDAR_AbsoluteFromGregorianDate(year + 1, 1, 1);
        if (abs_date < temp)
        {
            break;
        }
        year++;
    }
    date.date_year = year;
    
    /* Search forward from January */
    month = 1;
    for(;;)
    {
        day = MMICALENDAR_GetLastDayOfGregorianMonth(month, year);
        temp = MMICALENDAR_AbsoluteFromGregorianDate(date.date_year, month, day);
        if (abs_date <= temp) 
        {
            break;
        }
        month++;
    }
    date.date_month = month;
    
    /* Calculate the day by subtraction */
    temp = MMICALENDAR_AbsoluteFromGregorianDate(year, month, 1);
    date.date_day = abs_date - temp + 1;
    
    return date;
}

/*****************************************************************************/
//    Description : MMICALENDAR_Gregorian2Jewish
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC MMISET_DATE_T MMICALENDAR_Gregorian2Jewish(
                                                  uint16 g_year, 
                                                  uint8 g_month,
                                                  uint8 g_day
                                                  )
{
    MMISET_DATE_T jewish_date = {0};
    uint32 absolute = 0;

    absolute = MMICALENDAR_AbsoluteFromGregorianDate(g_year, g_month, g_day);

    jewish_date = MMICALENDAR_JewishDateFromAbsolute(absolute);
    SCI_TRACE_LOW("MMICALENDAR_Gregorian2Jewish jewish_date year = %d", jewish_date.date_year);
    SCI_TRACE_LOW("MMICALENDAR_Gregorian2Jewish jewish_date month = %d", jewish_date.date_month);
    SCI_TRACE_LOW("MMICALENDAR_Gregorian2Jewish jewish_date day = %d", jewish_date.date_day);

    return jewish_date;
}

/*****************************************************************************/
//    Description : MMICALENDAR_Jewish2Gregorian
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC MMISET_DATE_T MMICALENDAR_Jewish2Gregorian(
                                                  uint16 j_year, 
                                                  uint8 j_month,
                                                  uint8 j_day
                                                  )
{
    MMISET_DATE_T gregorian_date = {0};
    uint32 absolute = 0;

    absolute = MMICALENDAR_AbsoluteFromJewishDate(j_year, j_month, j_day);
    gregorian_date = MMICALENDAR_GregorianDateFromAbsolute(absolute);
    SCI_TRACE_LOW("MMICALENDAR_Jewish2Gregorian gregorian_date year = %d", gregorian_date.date_year);
    SCI_TRACE_LOW("MMICALENDAR_Jewish2Gregorian gregorian_date month = %d", gregorian_date.date_month);
    SCI_TRACE_LOW("MMICALENDAR_Jewish2Gregorian gregorian_date day = %d", gregorian_date.date_day);

    return gregorian_date;
}

/*****************************************************************************/
//    Description : MMICALENDAR_GetHolidayForJewishDate
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMICALENDAR_GetHolidayForJewishDate(BOOLEAN isAbroad,
                                                MMI_TEXT_ID_T *holidays //out
                                                )
{
    uint8  index = 0;
    uint16 heb_year = 0;
    uint8  heb_month = 0;
    uint8  heb_day = 0;
    uint8  monthEsther = 0;
    int8  hagadolDay = 0;
    MMI_TEXT_ID_T text_id = TXT_NULL;

    heb_year = g_current_date.j_year;
    heb_month = g_current_date.j_month;
    heb_day = g_current_date.j_day;

    if (MMICALENDAR_IsHebrewLeapYear(heb_year))
    {
        monthEsther = 13;
    }
    else
    {
        monthEsther = 12;
    }

    hagadolDay = 14;
    while (MMICALENDAR_GetWeekdayOfHebrewDate(hagadolDay, 1, heb_year) != 6)
    {
        hagadolDay -= 1;
    }

    switch(heb_month)
    {
    case JEWISH_MONTH_NISAN:
        if (heb_day == hagadolDay)
        {
            holidays[0] = TXT_HOLIDAY_SHABAT_GADOL;
        }
        if (heb_day == 14 && heb_month == 1)
        {
            holidays[0] = TXT_HOLIDAY_EREV_PESACH;
        }
        if ((heb_day == 15)
            || (heb_day == 16 && isAbroad))
        {
            holidays[0] = TXT_HOLIDAY_PESACH;
        }
        if ((heb_day == 16 && !isAbroad)
            || (heb_day == 17)
            || (heb_day == 18)
            || (heb_day == 19)
            || (heb_day == 20))
        {
            holidays[0] = TXT_HOLIDAY_CHOL_HAMOED_PESACH;
        }
        
        if ((heb_day == 21 && !isAbroad)
            || (heb_day == 22 && isAbroad))
        {
            holidays[0] = TXT_HOLIDAY_SHVII_PESACH_YIZKOR;
        }
        if (heb_day == 21 && isAbroad)
        {
            holidays[0] = TXT_HOLIDAY_SHVII_PESACH;
        }
        break;
    case JEWISH_MONTH_IYAR:
        if (heb_day == 14 )
        {
            holidays[0] = TXT_HOLIDAY_PESACH_SHENI;
        }
        if (heb_day == 18)
        {
            holidays[0] = TXT_HOLIDAY_LAG_BAOMER;
        }
        break;
    case JEWISH_MONTH_SIVAN:
        if (heb_day == 5)
        {
            holidays[0] = TXT_HOLIDAY_EREV_SHAVUOT;
        }
        if ((heb_day == 6 && isAbroad)
            || (heb_day == 7 && isAbroad))
        {
            holidays[0] = TXT_HOLIDAY_SHAVUOT;
        }
        if (heb_day == 6 && !isAbroad)
        {
            holidays[0] = TXT_HOLIDAY_SHAVUOT_YIZKOR;
        }
        break;
    case JEWISH_MONTH_TAMUZ:
        if(MMICALENDAR_GetWeekdayOfHebrewDate(17, 4, heb_year) == 6)
        {
            if (heb_day == 18)
            {
                holidays[0] = TXT_HOLIDAY_TZOM_TAMUZ;
            }
        }
        else
        {
            if (heb_day == 17)
            {
                holidays[0] = TXT_HOLIDAY_TZOM_TAMUZ;
            }
        }
        break;
    case JEWISH_MONTH_AV:
        if (MMICALENDAR_GetWeekdayOfHebrewDate(9, 5, heb_year) == 6)
        {
            if (heb_day == 10)
            {
                holidays[0] = TXT_HOLIDAY_TISHA_BEAV;
            }
        }
        else
        {
            if (heb_day == 9)
            {
                holidays[0] = TXT_HOLIDAY_TISHA_BEAV;
            }
        }
        
        if (heb_day == 15)
        {
            holidays[0] = TXT_HOLIDAY_TU_BEAV;
        }
        break;
    case JEWISH_MONTH_ELUL:
        if (heb_day == 29 && heb_month == 6)
        {
            holidays[0] = TXT_HOLIDAY_EREV_ROSH_HSANA;
        }
        break;
    case JEWISH_MONTH_TISHRI:
        if (heb_day == 1)
        {
            holidays[0] = TXT_HOLIDAY_ROSH_SHANA_A;
        }
        if (heb_day == 2)
        {
            holidays[0] = TXT_HOLIDAY_ROSH_SHANA_B;
        }
        if (MMICALENDAR_GetWeekdayOfHebrewDate(3, 7, heb_year) == 6)
        {
            if (heb_day == 4)
            {
                holidays[0] = TXT_HOLIDAY_TZOM_GEDALYA;
            }
        }
        else
        {
            if (heb_day == 3)
            {
                holidays[0] = TXT_HOLIDAY_TZOM_GEDALYA;
            }
        }
        
        if (heb_day == 9)
        {
            holidays[0] = TXT_HOLIDAY_EREV_KIPUR;
        }
        if (heb_day == 10)
        {
            holidays[0] = TXT_HOLIDAY_KIPUR_YIZKOR;
        }
        if (heb_day == 14)
        {
            holidays[0] = TXT_HOLIDAY_EREV_SUKOT;
        }
        if (heb_day == 15)
        {
            if (isAbroad)
            {
                holidays[0] = TXT_HOLIDAY_SUKOT;
            }
            else
            {
                holidays[0] = TXT_HOLIDAY_SUKOT;
            }
        }
        if (heb_day == 16)
        {
            if (isAbroad)
            {
                holidays[0] = TXT_HOLIDAY_SUKOT;
            }
            else
            {
                holidays[0] = TXT_HOLIDAY_CHOL_MOED_SUKOT;
            }
        }

        if (heb_day == 17 || heb_day == 18 || heb_day == 19 || heb_day == 20)
        {
            holidays[0] = TXT_HOLIDAY_CHOL_MOED_SUKOT;
        }
        if (heb_day == 21)
        {
            holidays[0] = TXT_HOLIDAY_HOSHANA_RABA;
        }          
        if (heb_day == 22)
        {
            if (!isAbroad)
            {
                holidays[0] = TXT_HOLIDAY_SHMINI_ATZERET_YIZKOR;
                holidays[1] = TXT_HOLIDAY_SIMCHAT_TORA;
            }
            else
            {
                holidays[0] = TXT_HOLIDAY_SHMINI_ATZERET_YIZKOR;
            }
        }
        if (heb_day == 23)
        {
            if (isAbroad)
            {
                holidays[0] = TXT_HOLIDAY_SIMCHAT_TORA;
            }
        }
        break;
    case JEWISH_MONTH_CHESHVAN:
        break;
    case JEWISH_MONTH_KISLEV:
    case JEWISH_MONTH_TEVET:
        if (heb_day == 25 && heb_month == JEWISH_MONTH_KISLEV)
        {
            holidays[0] = TXT_HOLIDAY_HANUKA_1;
        }
        if (heb_day == 26 && heb_month == JEWISH_MONTH_KISLEV)
        {
            holidays[0] = TXT_HOLIDAY_HANUKA_2;
        }
        if (heb_day == 27 && heb_month == JEWISH_MONTH_KISLEV)
        {
            holidays[0] = TXT_HOLIDAY_HANUKA_3;
        }
        if (heb_day == 28 && heb_month == JEWISH_MONTH_KISLEV)
        {
            holidays[0] = TXT_HOLIDAY_HANUKA_4;
        }
        if (heb_day == 29 && heb_month == JEWISH_MONTH_KISLEV)
        {
            holidays[0] = TXT_HOLIDAY_HANUKA_5;
        }

        if (heb_day == 10 && heb_month == JEWISH_MONTH_TEVET)
        {
            holidays[0] = TXT_HOLIDAY_TZOM_TEVET;
        }
        if (MMICALENDAR_GetLastDayOfJewishMonth(JEWISH_MONTH_KISLEV, heb_year) == 30)
        {
            if (heb_day == 30 && heb_month == JEWISH_MONTH_KISLEV)
            {
                holidays[0] = TXT_HOLIDAY_HANUKA_6;
            }
            if (heb_day == 1 && heb_month == JEWISH_MONTH_TEVET)
            {
                holidays[0] = TXT_HOLIDAY_HANUKA_7;
            }
            if (heb_day == 2 && heb_month == JEWISH_MONTH_TEVET)
            {
                holidays[0] = TXT_HOLIDAY_HANUKA_8;
            }
        }
        if (MMICALENDAR_GetLastDayOfJewishMonth(JEWISH_MONTH_KISLEV, heb_year) == 29)
        {
            if (heb_day == 1 && heb_month == JEWISH_MONTH_TEVET)
            {
                holidays[0] = TXT_HOLIDAY_HANUKA_6;
            }
            if (heb_day == 2 && heb_month == JEWISH_MONTH_TEVET)
            {
                holidays[0] = TXT_HOLIDAY_HANUKA_7;
            }
            if (heb_day == 3 && heb_month == JEWISH_MONTH_TEVET)
            {
                holidays[0] = TXT_HOLIDAY_HANUKA_8;
            }
        }
        break;
    
        break;
    case JEWISH_MONTH_SHVAT:
        if (heb_day == 15)
        {
            holidays[0] = TXT_HOLIDAY_TU_BISHVAT;
        }
        break;
    case JEWISH_MONTH_ADAR_I:
    case JEWISH_MONTH_ADAR_II:
        if (MMICALENDAR_GetWeekdayOfHebrewDate(13, monthEsther, heb_year) == 6)
        {
            if (heb_day == 11 && heb_month == monthEsther)
            {
                holidays[0] = TXT_HOLIDAY_TAANIT_ESTER;
            }
        }
        else
        {
            if (heb_day == 13 && heb_month == monthEsther)
            {
                holidays[0] = TXT_HOLIDAY_TAANIT_ESTER;
            }
        }
        
        if (heb_day == 14 && heb_month == monthEsther)
        {
            holidays[0] = TXT_HOLIDAY_PURIM;
        }
        if (heb_day == 15 && heb_month == monthEsther)
        {
            holidays[0] = TXT_HOLIDAY_SHUSHAN_PURIM;
        }
        if (MMICALENDAR_IsHebrewLeapYear(heb_year))
        {
            if (heb_day == 14 && heb_month == 12)
            {
                holidays[0] = TXT_HOLIDAY_PURIM_KATAN;
            }
            if (heb_day == 15 && heb_month == 12)
            {
                holidays[0] = TXT_HOLIDAY_SHUSHAN_PURIM_KATAN;
            }
        }
        break;
    default:
            break;
    }
}

/*****************************************************************************/
//    Description : MMICALENDAR_Number2Gematria
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMICALENDAR_Number2Gematria(wchar*text, int num)
{
    uint8 index = 0;
    uint16 length = 0;

    if (num > 1000)
    {
        MMICALENDAR_Number2Gematria(text, num / 1000);
        num %= 1000;
    }
    
    while (num >= 400)
    {
        MMIAPICOM_Wstrncat(text, s_taf_string, 1);
        num -= 400;
    }
 
    if (num >= 100)
    {
        index = (num / 100) - 1;
        MMIAPICOM_Wstrncat(text, &s_quf_string[index], 1);
        num %= 100;
    }
    
    if (num >= 10)
    {
        switch (num)
        {
        case 15:
            MMIAPICOM_Wstrncat(text, s_tu_string, 2);
            num = 0;
            break;
        case 16:
            MMIAPICOM_Wstrncat(text, s_tz_string, 2);
            num = 0;
            break;
        default:
            index = (num/10) - 1;
            MMIAPICOM_Wstrncat(text, &s_yud_string[index], 1);
            break;
        }
        num %= 10;
    }
    
    if (num > 0)
    {
        index = num - 1;
        MMIAPICOM_Wstrncat(text, &s_alef_string[index], 1);
    }

    length = MMIAPICOM_Wstrlen(text);
    if(length == 1)
    {
        MMIAPICOM_Wstrncat(text, L"'", 1);
    }
}

/*****************************************************************************/
//    Description : MMIJEWISHCAL_GetWeekday
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMIJEWISHCAL_GetWeekday(uint32 abs_date)
{
    return (abs_date % 7);
}

/*****************************************************************************/
//    Description : compare date
//    Global resource dependence : 
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC int8 MMIJEWISHCAL_CompareDate(
                                     MMISET_DATE_T  left_date,
                                     MMISET_DATE_T  right_date
                                     )
{
    int8 result = 0;
    
    if ( left_date.date_year > right_date.date_year )
    {
        result = 1;
    }
    else if ( left_date.date_year < right_date.date_year )
    {
        result = -1;
    }
    else
    {
        if ( left_date.date_month > right_date.date_month )
        {
            result = 1;
        }
        else if ( left_date.date_month < right_date.date_month )
        {
            result = -1;
        }
        else
        {
            if ( left_date.date_day > right_date.date_day )
            {
                result = 1;
            }
            else if ( left_date.date_day < right_date.date_day )
            {
                result = -1;
            }
        }
    }
    
    return result;
}
#endif
