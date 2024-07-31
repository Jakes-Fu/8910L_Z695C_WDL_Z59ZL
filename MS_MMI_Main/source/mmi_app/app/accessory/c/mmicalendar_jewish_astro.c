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
#include "math.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifndef M_PI
#define M_PI 3.1415926535
#endif

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
extern MMI_JEWISH_CALENDAR_T  g_current_date;
extern MMI_JEWISH_SUNTIME_T   g_current_suntimes;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
static double s_old_UT = 0;
static double s_old_sidereal = 0;
static double s_old_sunlong = 0;
static double s_old_ET = 0;

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                                 */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//    Description : Gregorian2JulianDate
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Gregorian2JulianDate(
                                  double timezone, 
                                  uint16 year, 
                                  uint8 month, 
                                  uint8 day, 
                                  uint16 hours
                                  );

/*****************************************************************************/
//    Description : JDg
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double JDg(int year, int month, int day);

/*****************************************************************************/
//    Description : Calculate the delta
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Delta(double j_day);

/*****************************************************************************/
//    Description : SunAngleOnSettingOrRising
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double SunAngleOnSettingOrRising(
                                       int iHeight, 
                                       BOOLEAN ConsiderHeight, 
                                       double BaseAngle
                                       );

/*****************************************************************************/
//    Description : RisingTransitSetting
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL MMI_JEWISH_SUNTIME_T RisingTransitSetting(
                                                double UT, 
                                                MMI_JEWISHCAL_LOCATION_ITEM_T location, 
                                                double UTsetting, 
                                                double UTrising, 
                                                double UTtransit, 
                                                double RiseSetAltitude
                                                );

/*****************************************************************************/
//    Description : Sidereal
//                  Serial Time: Aries head distance from Greenwich's declaration.
//                  Astronomical Algorithms 1991 Page 84
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Sidereal(double UT);

/*****************************************************************************/
//    Description : Sidereal
//                  Serial Time: Aries head distance from Greenwich's declaration.
//                  Astronomical Algorithms 1991 Page 84
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double SunRightAscension(double ET);

/*****************************************************************************/
//    Description : Obliquity
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Obliquity(double ET);

/*****************************************************************************/
//    Description : SunLong
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double SunLong(double ET);

/*****************************************************************************/
//    Description : RightAscension
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double RightAscension(double Lm, double Bm, double Ob);

/*****************************************************************************/
//    Description : SunDeclination
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double SunDeclination(double ET);

/*****************************************************************************/
//    Description : Declination
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Declination(double Lm, double Bm, double Ob);

/*****************************************************************************/
//    Description : Declination
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double HoursAngle(double declination, double latitude, double distance, int8 height);

/*****************************************************************************/
//    Description : Altitude
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Altitude(double angle, double declination, double latitude);

/*****************************************************************************/
//    Description : Yinterpolation
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Yinterpolation(double N, double y1, double y2, double y3);

/*****************************************************************************/
//    Description : X0to1
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double X0to1(double X);

/*****************************************************************************/
//    Description : ToDegrees
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL __inline double ToDegrees(double radians);

/*****************************************************************************/
//    Description : ToRadians
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL __inline double ToRadians(double degrees);

/*****************************************************************************/
//    Description : Sine
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Sine(double degrees);

/*****************************************************************************/
//    Description : Sine
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Cosine(double degrees);

/*****************************************************************************/
//    Description : Sine
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Tangent(double degrees);

/*****************************************************************************/
//    Description : ArcTangent
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double ArcTangent(double degrees);

/*****************************************************************************/
//    Description : ArcSine
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double ArcSine(double degrees);

/*****************************************************************************/
//    Description : ArcSine
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double ArcCosine(double degrees);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
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
                                                        )
{
    MMI_JEWISH_SUNTIME_T suntimes = {0};
    MMI_JEWISH_SUNTIME_T times = {0};
    MMISET_DATE_T jewish_start = {0};
    MMISET_DATE_T tisha = {0};
    MMISET_DATE_T kipur = {0};
    double  j_date = 0; //the Julian date is used for all calculations
    double  alot_90_min = 0;
    double  alot_72_min = 0;
    double  tfilin = 0;
    double  kochavim = 0;
    double  julian_start = 0;
    int32   delta = 0;
    int32   passedYears=0;

    SCI_TRACE_LOW("MMICALENDAR_CalcDaySunTimes year = %d, month = %d, day = %d", g_date.date_year, g_date.date_month, g_date.date_day);

    //the Julian date is used for all calculations
    j_date = Gregorian2JulianDate(location.timezone, g_date.date_year, g_date.date_month, g_date.date_day, 12);
    SCI_TRACE_LOW("MMICALENDAR_CalcDaySunTimes j_date setting = %lf", g_current_suntimes.setting);

    //calc the setting, transit & rising times (regular)
    suntimes = RisingTransitSetting(j_date, location, 0, 0, 0, -0.833);
    SCI_TRACE_LOW("MMICALENDAR_CalcDaySunTimes suntimes setting = %lf", suntimes.setting);

    //set whether the summer-clock is on
    suntimes.dls = dlsStatus;
    
    //calc the setting time according to location's height (used for "shkia")
    times = RisingTransitSetting(j_date, location, 0, 0, 0, SunAngleOnSettingOrRising(location.height, TRUE, -0.833));
    suntimes.setting_height = times.setting;
    SCI_TRACE_LOW("MMICALENDAR_CalcDaySunTimes suntimes setting_height = %lf", suntimes.setting_height);

    //calculate various additional times of day
    times = RisingTransitSetting(j_date, location, 0, 0, 0, -19.75);
    alot_90_min = times.rising;
    suntimes.alot_90min = alot_90_min;
    SCI_TRACE_LOW("MMICALENDAR_CalcDaySunTimes suntimes alot_90min = %lf", suntimes.alot_90min);

    times = RisingTransitSetting(j_date, location, 0, 0, 0, -16.05);
    alot_72_min = times.rising;
    suntimes.alot_72min = alot_72_min;
    SCI_TRACE_LOW("MMICALENDAR_CalcDaySunTimes suntimes alot_72min = %lf", suntimes.alot_72min);

    times = RisingTransitSetting(j_date, location, 0, 0, 0, -11.5);
    tfilin = times.rising;
    suntimes.tfilin = tfilin;
    SCI_TRACE_LOW("MMICALENDAR_CalcDaySunTimes suntimes tfilin = %lf", suntimes.tfilin);

    times = RisingTransitSetting(j_date, location, 0, 0, 0, -8.5);
    kochavim = times.setting;
    suntimes.kochavim = kochavim;
    SCI_TRACE_LOW("MMICALENDAR_CalcDaySunTimes suntimes kochavim = %lf", suntimes.kochavim);

    //calculate the date for the "daf yomi" bavli
    //CalendarDate gregStart = new CalendarDate(3, 8, 2012);
    julian_start = Gregorian2JulianDate(location.timezone, 2012, 8, 3, 12);
    delta = (int)(j_date - julian_start + 1 + 10*2711) % 2711;
    suntimes.bavli_index = delta;
    SCI_TRACE_LOW("MMICALENDAR_CalcDaySunTimes suntimes bavli_index = %lf", suntimes.bavli_index);

    //calculate the date for the "daf yomi" yerushalmi
//     gregStart = new CalendarDate(12, 24, 4, 2014);
    julian_start = Gregorian2JulianDate(location.timezone, 2014, 4, 24, 12);
    
    //no modulu here - we want to go over the passed days and drop days of kipur & tisha be'av
    delta = (int)(j_date - julian_start + 1);

    //calculate how many yom-kipur and tisha-be'av we need to substract
    jewish_start = MMICALENDAR_Gregorian2Jewish(2014, 4, 24);
    
    //every missed day reduces the index by 1
    if (delta <= 0)
    {
        //make sure delta is positive ...
        delta = (delta + 10*1554 - 1) % 1554 + 1;
        
        //traverse the years, till we get to the jewish date of the desired day
        //every kipur or tisha-be'av we find - reduce the plan's index by 1
        for (passedYears=0;; ++passedYears)
        {
            kipur = MMICALENDAR_Jewish2Gregorian((jewish_start.date_year - passedYears), 7, 10);
            tisha = MMICALENDAR_Jewish2Gregorian((jewish_start.date_year - passedYears - 1), 5, 9);
            
            if (MMIJEWISHCAL_CompareDate(kipur, g_date) < 0)
            {
                break;
            }
            ++delta;
            
            if (MMIJEWISHCAL_CompareDate(tisha, g_date) < 0)
            {
                break;
            }
            ++delta;
        }
        
        //make sure delta is positive ...
        delta = (delta + 10*1554 - 1) % 1554 + 1;
    }
    else
    {
        //traverse the years, till we get to the jewish date of the desired day
        //every kipur or tisha-be'av we find - reduce the plan's index by 1
        for (passedYears = 0;; ++passedYears)
        {
            tisha = MMICALENDAR_Jewish2Gregorian((jewish_start.date_year + passedYears), 5, 9);
            kipur = MMICALENDAR_Jewish2Gregorian((jewish_start.date_year + passedYears + 1), 7, 10);
            
            if (MMIJEWISHCAL_CompareDate(tisha, g_date) > 0)
            {
                break;
            }
            --delta;
            
            if (MMIJEWISHCAL_CompareDate(kipur, g_date) > 0)
            {
                break;
            }
            --delta;
        }
        
        delta %= 1554;
    }
    
    //set the plan's index for future use
    suntimes.yerushalmi_index = delta;
    SCI_TRACE_LOW("MMICALENDAR_CalcDaySunTimes yerushalmi_index = %d", suntimes.yerushalmi_index);

    return suntimes;
}

/*****************************************************************************/
//    Description : Calculates whether a gregorian date has DLS (in israel only)
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICALENDAR_isIsraelDLSOn(MMISET_DATE_T greg_date)
{
    //summer clock in israel is activated as follwos:
    //turns on on the last friday before April 2nd
    //turns off on the last sunday before kipur
    MMISET_DATE_T greg_date_summer = {0};
    MMISET_DATE_T jewish_date_summer = {0};
    MMISET_DATE_T greg_date_winter = {0};
    uint32 absApril2nd = 0;
    uint32 abs_date = 0;
    uint32 abs_kipur = 0;
    uint8 weekday = 0;
    BOOLEAN result = TRUE;

    //calculate the transition date to summer clock
    absApril2nd = MMICALENDAR_AbsoluteFromGregorianDate(greg_date.date_year, 4, 2);
    weekday = MMIJEWISHCAL_GetWeekday(absApril2nd);
    abs_date = absApril2nd - ((weekday + 2) % 7);
    
    greg_date_summer = MMICALENDAR_GregorianDateFromAbsolute(abs_date);
    
    SCI_TRACE_LOW("MMICALENDAR_isIsraelDLSOn on the greg year %d summer clock starts on %d, %d, %d", 
        greg_date.date_year, greg_date_summer.date_year, greg_date_summer.date_month, greg_date_summer.date_day);
    
    //in case the date we check is before the transition date - this is winter clock for sure
    if (MMIJEWISHCAL_CompareDate(greg_date, greg_date_summer) < 0)
    {
        result = FALSE;
    }
    
    //calculate the transition date to winter clock, on the following jewish year (based on kipur)
    jewish_date_summer = MMICALENDAR_JewishDateFromAbsolute(abs_date);
    //kipur_jewish = new CalendarDate(10, 7, jewishTransitionDateSummer.getYear() + 1);
    abs_kipur = MMICALENDAR_AbsoluteFromJewishDate((jewish_date_summer.date_year + 1), 7, 10);
    weekday = MMIJEWISHCAL_GetWeekday(abs_kipur);
    abs_date = abs_kipur - ((weekday + 6) % 7 + 1);
    greg_date_winter = MMICALENDAR_GregorianDateFromAbsolute(abs_date);
    
    SCI_TRACE_LOW("MMICALENDAR_isIsraelDLSOn on the greg year %d summer clock ends on %d, %d, %d", 
        greg_date.date_year, greg_date_winter.date_year, greg_date_winter.date_month, greg_date_winter.date_day);
    
    //in case the date we check is after the transition date - this is winter clock
    if (MMIJEWISHCAL_CompareDate(greg_date, greg_date_winter) >= 0)
    {
        result = FALSE;
    }
    
    SCI_TRACE_LOW("MMICALENDAR_isIsraelDLSOn result = %d", result);

    //the only option left - we are on summer clock ...
    return result;
}

/*****************************************************************************/
//    Description : Gregorian2JulianDate
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL __inline double ToDegrees(double radians)
{
    return radians * (180.0 / M_PI);
}

/*****************************************************************************/
//    Description : Gregorian2JulianDate
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL __inline double ToRadians(double degrees)
{
    double result = 0;
    result = degrees / 180.0 * M_PI;
    return result;
}

/*****************************************************************************/
//    Description : Sine
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Sine(double degrees)
{
    double result = 0;
    result = sin(ToRadians(fmod(degrees, 360.0)));
    return result;
}

/*****************************************************************************/
//    Description : Cosine
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Cosine(double degrees)
{
    double result = 0;
    result = cos(ToRadians(fmod(degrees, 360.0)));
    return result;
}

/*****************************************************************************/
//    Description : Tangent
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Tangent(double degrees)
{
    double result = 0;
    result = tan(ToRadians(fmod(degrees, 360.0)));
    return result;
}

/*****************************************************************************/
//    Description : ArcTangent
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double ArcTangent(double degrees)
{
    double result = 0;
    result = ToDegrees(atan(degrees));
    return result;
}

/*****************************************************************************/
//    Description : ArcSine
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double ArcSine(double degrees)
{
    double result = 0;
    double value = 0;

    value = sqrt(1 - degrees * degrees) + 1 - 1.0E-22;
    result = 2 * ArcTangent(degrees / value);
    return result;
}

/*****************************************************************************/
//    Description : ArcSine
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double ArcCosine(double degrees)
{
    double result = 0;
    result = 90 - ArcSine(degrees);
    return result;
}

/*****************************************************************************/
//    Description : Gregorian2JulianDate
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Gregorian2JulianDate(
                                  double timezone, 
                                  uint16 year, 
                                  uint8 month, 
                                  uint8 day, 
                                  uint16 hours
                                  )
{
    double j_day = 0;

    j_day = JDg(year, month, day);
    j_day = j_day + (hours - timezone) / 24;
    return j_day;
}

/*****************************************************************************/
//    Description : JDg
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double JDg(int year, int month, int day)
{
    //months are January or February
    if (month < 3)
    {
        month = (short) (month + 12);
        //January=13, February=14
        year = (short) (year - 1);
        //of the previous year
    }
    //months are January or February
    return day + (153 * month - 457) / 5 + (int) (365.25 * year) - (int) (year * 0.01) + (int) (year * 0.0025) + 1721118.5;
}

/*****************************************************************************/
//    Description : Calculate the delta
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Delta(double j_day)
{
    return 65.8;
}

/*****************************************************************************/
//    Description : SunAngleOnSettingOrRising
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double SunAngleOnSettingOrRising(
                                       int iHeight, 
                                       BOOLEAN ConsiderHeight, 
                                       double BaseAngle
                                       )
{
    double HH = 0;

    // Consideration of height
    if (ConsiderHeight)
    {
        HH = 0.0353 * sqrt(abs(iHeight));
        return BaseAngle - HH;
    }
    else
    {
        return BaseAngle;
    }
}

/*****************************************************************************/
//    Description : RisingTransitSetting
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL MMI_JEWISH_SUNTIME_T RisingTransitSetting(
                                                double UT, 
                                                MMI_JEWISHCAL_LOCATION_ITEM_T location, 
                                                double UTsetting, 
                                                double UTrising, 
                                                double UTtransit, 
                                                double RiseSetAltitude
                                                )
{
    //Calculate sunrise time and statement
    //TP.nLongitude - longitude of the observer in degrees.  positively west from Greenwich.
	//TP.nLatitude - Latitude of the observer in degrees.
    MMI_JEWISH_CALENDAR_DATE_T date = {0};
    MMI_JEWISH_SUNTIME_T suntimes = {0};
	double De2 = 0;
	double De = 0;
	double Ra2 = 0;
	double RA = 0;
	double Ra1 = 0;
	double Ra3 = 0;
	double De1 = 0;
	double De3 = 0;
	double Sid = 0;
	double Sid0 = 0;
	double N = 0;
	double ET0 = 0; // the ephemeris time at 0h
	double Dm = 0;
	double h0 = 0;
	double Ha = 0;
	double H = 0;
	double ET = 0; // the length of the suntime according to the Julian day.
	double UT0 = 0; //the Universal Time at 0h
    double m[3] = {0};
	short I = 0;
	short J = 0;
	double HorizonDistance = 0;
	int Ld = 0;
	int Ly = 0;
	int Lm = 0;
	short Q = 0;
	double hh = 0;
    double temp = 0;

    suntimes.setting = UTsetting;
    suntimes.rising = UTrising;
    suntimes.transit = UTtransit;

	ET = UT + Delta(UT) / 86400;
	HorizonDistance = RiseSetAltitude;

	// for the sun
	date = ConvertJulianToGregorianDate(ET, location.timezone);
	hh = date.hour;
	Ly = date.year;
	Lm = date.month;
	Ld = date.day;

	ET0 = Gregorian2JulianDate(location.timezone, Ly, Lm, Ld, 0);

	date = ConvertJulianToGregorianDate(UT, location.timezone);
	hh = date.hour;
	Ly = date.year;
	Lm = date.month;
	Ld = date.day;

    
	UT0 = Gregorian2JulianDate(location.timezone, Ly, Lm, Ld, 0);
	
	Sid0 = Sidereal(UT0);

	Ra1 = SunRightAscension(ET0 - 1);
	Ra2 = SunRightAscension(ET0);
	Ra3 = SunRightAscension(ET0 + 1);

	De1 = SunDeclination(ET0 - 1);
	De2 = SunDeclination(ET0);
	De3 = SunDeclination(ET0 + 1);

	h0 = HoursAngle(De2, location.latitude, HorizonDistance, (short)0);

	//No sunset or sunrise
	if (h0 == 1.0E+89)
	{
        //Just thought midnight today
		suntimes.transit = h0;
		suntimes.rising = h0;
		Q = 0;
	}
	else
	{
		Q = 2;
	}

    temp = (Ra2 + location.longitute - Sid0) / 360;
	m[0] = fmod(temp, 360.0);
	m[1] = m[0] - h0 / 360;
	m[2] = m[0] + h0 / 360;
	m[0] = X0to1(m[0]);
	m[1] = X0to1(m[1]);
	m[2] = X0to1(m[2]);
	for (J = 0; J <= 1; J++)
	{
		for (I = 0; I <= Q; I++)
		{
/*            temp = Sid0 + 360.9856473 * m[I];*/
			Sid = fmod((Sid0 + 360.9856473 * m[I]), 360.0);
			// Estimated statement
			N = m[I] + Delta(UT) / 86400;
			RA = Yinterpolation(N, Ra1, Ra2, Ra3);
			De = Yinterpolation(N, De1, De2, De3);
			Ha = Sid - location.longitute - RA;
			H = Altitude(Ha, De, location.latitude);
			if (I == 0)
			{
                // Declare sunrise or sunset
				Dm = -Ha / 360;
			}
			else
			{
				Dm = (H - HorizonDistance) / (360 * Cosine(De) * Cosine(location.latitude) * Sine(Ha));
			}
			m[I] = m[I] + Dm;
			m[I] = X0to1(m[I]);
		}
	}

    suntimes.transit = (UT0 + m[0]);
    suntimes.rising = (UT0 + m[1]);
    suntimes.setting = (UT0 + m[2]);

	return suntimes;
}

/*****************************************************************************/
//    Description : ConvertJulianToGregorianDate
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC MMI_JEWISH_CALENDAR_DATE_T ConvertJulianToGregorianDate(
                                                               double j_day, 
                                                               double timezone
                                                               )
{
    MMI_JEWISH_CALENDAR_DATE_T date = {0};
    double hours = 0;
    int8 month = 0;
    int32 Je = 0;
    int32 Jc = 0;
    int32 Ja = 0;
    int32 IGreg = 0;
    int32 Julian = 0;
    int32 Jalpha = 0;
    int32 Jb = 0;
    int32 J = 0;
    int8 day = 0;
    int16 year = 0;

    if (abs(j_day) > 10000000000.0)
    {
        //todo avishai
        return date;
    }

    hours = (fmod((j_day + 0.5), 1)) * 24.0 + timezone; //modulu1
    //Calculate the time
    hours = fmod((hours + 24.0), 24); //modulu24

    Julian = (int) (j_day + timezone / 24);
    IGreg = 2299161;
    if (Julian >= IGreg)
    {
        Jalpha = (int) (((Julian - 1867216) - 0.25) / 36524.25);
        Ja = Julian + 1 + Jalpha - (int) (0.25 * Jalpha);
    }
    else
    {
        Ja = Julian;
    }

    Jb = Ja + 1524;
    Jc = (int) (6680 + ((Jb - 2439870) - 122.1) / 365.25);
    J = 365 * Jc + (int) (0.25 * Jc);
    Je = (int) ((Jb - J) / 30.6001);
    day = Jb - J - (int) (30.6001 * Je);
    //Calculate the day
    month = Je - 1;
    if (month > 12)
    {
        month = month - 12;
    }
    //Calculate the Month
    year = Jc - 4715;
    //Calculate the year
    if (month > 2)
    {
        year = year - 1;
    }
    //  IF Yr& <= 0 THEN Yr& = Yr& - 1    THERE IS YEAR NUMBER 0

    date.year = year;
    date.month = month;
    date.day = day;
    date.hour = hours;

    return date;
}

/*****************************************************************************/
//    Description : Sidereal
//                  Serial Time: Aries head distance from Greenwich's declaration.
//                  Astronomical Algorithms 1991 Page 84
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Sidereal(double UT)
{
    double T = 0;

    if (s_old_UT == UT)
    {
        return s_old_sidereal;
    }
    T = (UT - 2451545.0) / 36525.0;

    s_old_sidereal = 280.46061837 + 360.98564736629 * (UT - 2451545.0) + 0.000387933 * pow(T, 2) - pow(T, 3) / 38710000;
    s_old_sidereal = fmod(s_old_sidereal, 360.0); //modulu360
    s_old_UT = UT;
    return s_old_sidereal;
}

/*****************************************************************************/
//    Description : SunRightAscension
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double SunRightAscension(double ET)
{
    //  The direct rise of the sun.
    double Bs = 0;  //  sun length
    double LS = 0;  //  sun width
    double Ob = 0;  //  equatorial angle
    double result = 0;

    Ob = Obliquity(ET);
    LS = SunLong(ET);
    result = RightAscension(LS, Bs, Ob);

    return result;
    
}

/*****************************************************************************/
//    Description : Obliquity
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Obliquity(double ET)
{
    //  Milka angle comparator
    double T = 0;
    double result = 0;
    float temp1 = 23;
    float temp2 = 26;
    float temp3 = 60;
    float temp4 = 3600;

    T = (ET - 2451545.0) / 36525.0;
    result = temp1 + temp2 / temp3 + 41.448 / temp4 - 46.815 / temp4 * T - 0.00059 / temp4 * T * T + 0.001813 / temp4 * T * T * T;
//     result = (float)23 + (float)26 / (float)60 + 41.448 / (float)3600 - 46.815 / (float)3600 * T - 0.00059 / (float)3600 * T * T + 0.001813 / (float)3600 * T * T * T;

    return result;
}

/*****************************************************************************/
//    Description : SunLong
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double SunLong(double ET)
{
    double m = 0;
    double T = 0;
    double L0 = 0;
    double c = 0;
    double O = 0;
    double a = 0;
    double l = 0;

    //  Length of the sun today according to the Julian calendar.
    if (s_old_ET == ET)
    {
        return s_old_sunlong;
    }
    
    //  Astronomical Algorithms 1991 Page 152
    T = (ET - 2451545.0) / 36525.0;
    L0 = 280.46645 + 36000.76983 * T + 0.0003032 * T * T;
    m = 357.5291 + 35999.0503 * T - 0.0001559 * pow(T, 2) - 0.00000048 * pow(T, 3);
    m = fmod(m, 360.0);
    c = (1.9146 - 0.004817 * T - 0.000014 * pow(T, 2)) * Sine(m) + (0.019993 - 0.000101 * T) * Sine(fmod((2 * m), 360.0)) + 0.00029 * Sine(fmod((3 * m), 360.0));
    a = L0 + c;
    O = 125.04 - 1934.136 * T;
    l = a - 0.00569 - 0.00478 * Sine(O);
    
    s_old_sunlong = fmod(l, 360.0);
    s_old_ET = ET;

    return s_old_sunlong;
}

/*****************************************************************************/
//    Description : RightAscension
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double RightAscension(double Lm, double Bm, double Ob)
{
    //  The ascension of the sun (straight rise)
    //  Lm# - sun length
    //  Bm# - sun width
    //  Ob# - equatorial angle
    double result = 0;
    
    result = ArcTangent((Sine(Lm) * Cosine(Ob) - Tangent(Bm) * Sine(Ob)) / Cosine(Lm));
    if (Lm > 90 & Lm <= 270)
    {
        result = result + 180;
    }
    if (Lm > 270 & Lm < 360)
    {
        result = result + 360;
    }
    return result;
}

/*****************************************************************************/
//    Description : SunDeclination
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double SunDeclination(double ET)
{
    //  The inclination of the sun
    //  Lm# - sun length
    //  Bm# - sun width
    //  Ob# - equatorial angle
    double Bs = 0;
    double LS = 0;
    double Ob = 0;
    double result = 0;

    Ob = Obliquity(ET);
    LS = SunLong(ET);
    result = Declination(LS, Bs, Ob);

    return result;
}

/*****************************************************************************/
//    Description : Declination
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Declination(double Lm, double Bm, double Ob)
{
    //  Calculate the inclination of the sun
    //  Lm# - sun length
    //  Bm# - sun width
    //  Ob# - equatorial angle
    double result = 0;

    result = ArcSine(Sine(Bm) * Cosine(Ob) + Cosine(Bm) * Sine(Ob) * Sine(Lm));
    return result;
}

/*****************************************************************************/
//    Description : Declination
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double HoursAngle(double declination, double latitude, double distance, int8 height)
{
    //  Calculate the time angle of the sunset
    //  declination:  The inclination from the sky equator
    //  latitude: The degree of geographical latitude
    //  distance: The distance from the horizon at the requested hour
    //  height: Altitude in meters
    double H = 0;
    double X = 0;
    double result = 0;

    if (height >= 0)
    {
        // Explanatory Supplement to the Astronomical Almanac, 1992, p.487
        H = 0.0353 * sqrt(height);
    }
    else
    {
        H = -0.0353 * sqrt(-height);
    }
    X = (Sine(distance + H) - Sine(latitude) * Sine(declination)) / (Cosine(declination) * Cosine(latitude));
    if (X <= 1 & X >= -1)
    {
        result = ArcCosine(X);
    }
    else
    {
        // no sunset sign
        result = 1.0E+89;
    }
    return result;
}

/*****************************************************************************/
//    Description : X0to1
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double X0to1(double X)
{
    double result = 0;

    result = X;
    if (X > 1)
    {
        result = X - 1;
    }
    if (X < 0)
    {
        result = X + 1;
    }

    return result;
}

/*****************************************************************************/
//    Description : Yinterpolation
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Yinterpolation(double N, double y1, double y2, double y3)
{
    // Astronomical Formulae for Calculators (Jean Meeus) Fourth Edition formula 2.3
    double a = 0;
    double b = 0;
    double c = 0;
    double result = 0;

    a = (y2 - y1);
    if (a < -300)
    {
        a = a + 360;
    }
    if (a > 300)
    {
        a = a - 360;
    }

    b = (y3 - y2);
    if (b < -300)
    {
        b = b + 360;
    }
    if (b > 300)
    {
        b = b - 360;
    }
    c = b - a;
    
    result = y2 + N * (a + b + N * c) / 2;
    return result;
}

/*****************************************************************************/
//    Description : Altitude
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL double Altitude(double angle, double declination, double latitude)
{
    //Rum above the horizon
    double result = 0;

    result = ArcSine(Sine(latitude) * Sine(declination) + Cosine(latitude) * Cosine(declination) * Cosine(angle));
    return result;
}

/*****************************************************************************/
//    Description : Calculates "Sha'a Zmanit" according to 2 methds:
//                  1. Magen Avraham - from Alot Hashachar
//                  2. Gra - from Hanetz
//    Global resource dependence : 
//    Author: 
//    Note: @param byGra (boolean) true for gra, false for magen avraham method
//          @return (double) sha'a zmanit according to the method
/*****************************************************************************/
PUBLIC double MMICALENDAR_GetZmanit(BOOLEAN byGra)
{
	//since the time is in julian format, we first need to convert it to real time & then divide to 12
	double zmanit;

	if (byGra)
	{
		zmanit = (g_current_suntimes.transit - g_current_suntimes.rising) / 6.0;
	}
	else
	{
		zmanit = (g_current_suntimes.transit - g_current_suntimes.alot_90min) / 6.0;
	}

    SCI_TRACE_LOW("MMICALENDAR_GetZmanit byGra = %d, zmanit = %d", byGra, zmanit);
	return zmanit;
}

#endif
