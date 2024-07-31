/*****************************************************************************
** File Name:      mmicalendar_jewish_def.h                                   
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

#ifndef _MMICALENDAR_JEWISH_DEF_H_
#define _MMICALENDAR_JEWISH_DEF_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef MMI_JEWISH_CALENDAR 
#include "sci_types.h"
#include "mmicom_time.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define JEWISH_MONTH_DAY                    MMICOM_DATE_SMALL_MONTH_DAY
#define JEWISH_LEAP_MONTH_DAY               MMICOM_DATE_LEAP_MONTH_DAY

#define JEWISH_MONTH_NUM                    MMICOM_DATE_MAX_MONTH
#define JEWISH_LEAP_MONTH_NUM               13

#define GREGORIAN_MONTH_NUM                 MMICOM_DATE_MAX_MONTH

#define DAYS_OF_NORMAL_YEAR         365

#define JEWISHCAL_DLS_TEXT_RECT         {60, 37, 180, 68}
#define JEWISHCAL_DLS_BG_RECT           {29, 30, 211, 68}
#define JEWISHCAL_LOCATION_TITLE_RECT   {0, 24, 240, 40}
/**--------------------------------------------------------------------------*
**                         TYPE AND STRUCT                                  *
**--------------------------------------------------------------------------*/

typedef enum
{
    JEWISH_DLS_OPT_AUTO,
    JEWISH_DLS_OPT_WINTER,
    JEWISH_DLS_OPT_SUMMER,
    JEWISH_DLS_OPT_MAX
} JEWISH_DLS_TYPE_E;

//month
typedef enum
{
    JEWISH_MONTH_NISAN = 1,
    JEWISH_MONTH_IYAR,
    JEWISH_MONTH_SIVAN,
    JEWISH_MONTH_TAMUZ,
    JEWISH_MONTH_AV,
    JEWISH_MONTH_ELUL,
    JEWISH_MONTH_TISHRI,
    JEWISH_MONTH_CHESHVAN,
    JEWISH_MONTH_KISLEV,
    JEWISH_MONTH_TEVET,
    JEWISH_MONTH_SHVAT,
    JEWISH_MONTH_ADAR,
    JEWISH_MONTH_ADAR_I = 12,
    JEWISH_MONTH_ADAR_II,
    JEWISH_MONTH_MAX
}JEWISH_MONTH_E;

typedef struct
{
    uint16   j_year; // jewish year
    uint8    j_month; // jewish month
    uint8    j_day; // jewish day of the month
    uint16   j_hour; // jewish hour
    uint16   g_year;  // gregorian year
    uint8    g_month; // gregorian month
    uint8    g_day;   // gregorian day of the month
} MMI_JEWISH_CALENDAR_T;

typedef struct
{
    uint16   year;
    uint8    month;
    uint8    day;
    double   hour;
} MMI_JEWISH_CALENDAR_DATE_T;

typedef struct
{
    //base times: sunrise, sunset, transit
    double      setting;
    double      rising;
    double      transit;

    //additonal times
    double      setting_height;
    double      alot_90min;
    double      alot_72min;
    double      tfilin;
    double      kochavim;
    int32       bavli_index;
    int32       yerushalmi_index;
    BOOLEAN     dls;
}MMI_JEWISH_SUNTIME_T;

/**
 * This struct is used to represent a location, for which we will calc the jewish times.
 * A location has longitute, latitude, elevation & time zone (shift from GMT)
 */
typedef struct
{
    uint32      text_id;
    double      latitude;
    double      longitute;
    double      timezone;
    int32       height;
    BOOLEAN     is_abroad;
    uint32      rule_for_candle_light;
}MMI_JEWISHCAL_LOCATION_ITEM_T;

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif
