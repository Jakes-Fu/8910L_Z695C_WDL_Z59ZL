/*****************************************************************************
** File Name:      mmicalendar_jewish_export.h                                               
** Author:           mark.hu    
** Date:            2005/0907
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe calendar window             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2005/09/07    mark.hu              Create                                 *
** 2006/08/09    liqing.peng          Modify
******************************************************************************/


#ifndef _MMIJEWISHCAL_EXPORT_H_
#define _MMIJEWISHCAL_EXPORT_H_



/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef MMI_JEWISH_CALENDAR 
#include "sci_types.h"
#include "mmicom_time.h"
#include "mmicalendar_jewish_def.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIJEWISHCAL_LOCATION_MAX_NUM			    14
#define MMIJEWISHCAL_DLS_SETTING_MAX_NUM			3
#define MMIJEWISHCAL_GEMATRIA_MAX_NUM               60
#define MMIJEWISHCAL_HOLIDAY_MAX_NUM                2
#define MMIJEWISHCAL_BAVLI_MASECHET_NUM             40
#define MMIJEWISHCAL_YERUSHALMI_MASECHET_NUM        39

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//    Description : MMIAPICALENDAR_OpenHebrewCalMainWin
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPICALENDAR_OpenJewishCalMainWin(void);

/*****************************************************************************/
//    Description : MMICALENDAR_IsHebrewLeapYear
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICALENDAR_IsHebrewLeapYear(uint16 year);

/*****************************************************************************/
//    Description : get last month of Jewish year
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMICALENDAR_GetLastMonthOfJewishYear(uint16 year);

/*****************************************************************************/
//    Description : get last day of Jewish month
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMICALENDAR_GetLastDayOfJewishMonth(uint8 month, uint16 year);

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
                                                 );

/*****************************************************************************/
//    Description : absolute from Jewish date
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC MMISET_DATE_T MMICALENDAR_JewishDateFromAbsolute(uint32 abs_date);

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
                                                );

/*****************************************************************************/
//    Description : MMICALENDAR_GetLastDayOfGregorianMonth
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMICALENDAR_GetLastDayOfGregorianMonth(
                                                    uint8 month, 
                                                    uint16 year
                                                    );

/*****************************************************************************/
//    Description : MMICALENDAR_GetWeekDayOfGregorianDate
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMICALENDAR_GetWeekDayOfGregorianDate(uint8 abs_date);

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
                                                    );

/*****************************************************************************/
//    Description : MMICALENDAR_GregorianDateFromAbsolute
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC MMISET_DATE_T MMICALENDAR_GregorianDateFromAbsolute(uint32 abs_date);

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
                                                  );

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
                                                         );

/*****************************************************************************/
//    Description : MMICALENDAR_GetHolidayForJewishDate
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMICALENDAR_GetHolidayForJewishDate(BOOLEAN isAbroad,
                                                MMI_TEXT_ID_T *holidays //out
                                                );

/*****************************************************************************/
//    Description : MMICALENDAR_Number2Gematria
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMICALENDAR_Number2Gematria(wchar*text, int num);

/*****************************************************************************/
//    Description : Calculates whether a gregorian date has DLS (in israel only)
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICALENDAR_isIsraelDLSOn(MMISET_DATE_T greg_date);

/*****************************************************************************/
//    Description : MMICALENDAR_CalcDaySunTimes
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC MMI_JEWISH_SUNTIME_T MMICALENDAR_CalcDaySunTimes(
                                                        MMI_JEWISHCAL_LOCATION_ITEM_T location, 
                                                        MMISET_DATE_T g_date, 
                                                        BOOLEAN dlsStatus
                                                        );

/*****************************************************************************/
//    Description : ConvertJulianToGregorianDate
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC MMI_JEWISH_CALENDAR_DATE_T ConvertJulianToGregorianDate(
                                                               double j_day, 
                                                               double timezone
                                                               );

/*****************************************************************************/
//    Description : MMIJEWISHCAL_CompareDate
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC int8 MMIJEWISHCAL_CompareDate(
                                     MMISET_DATE_T  left_date,
                                     MMISET_DATE_T  right_date
                                     );

/*****************************************************************************/
//    Description : MMIJEWISHCAL_GetWeekday
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMIJEWISHCAL_GetWeekday(uint32 abs_date);

/*****************************************************************************/
//    Description : Calculates "Sha'a Zmanit" according to 2 methds:
//                  1. Magen Avraham - from Alot Hashachar
//                  2. Gra - from Hanetz
//    Global resource dependence : 
//    Author: 
//    Note: @param byGra (boolean) true for gra, false for magen avraham method
//          @return (double) sha'a zmanit according to the method
/*****************************************************************************/
PUBLIC double MMICALENDAR_GetZmanit(BOOLEAN byGra);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

    
#endif  //_MMIJEWISHCAL_EXPORT_H_
#endif

