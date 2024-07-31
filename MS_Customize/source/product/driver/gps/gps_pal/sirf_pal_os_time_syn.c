/**
 * @addtogroup platform_src_sirf_pal_synergy
 * @{
 */

/*
 *                
 *
 */

/**
 * @file   sirf_pal_os_time_syn.c
 *
 * @brief  SiRF PAL time module. his file has been ported to use Synergy
 * Framework functions. This means that if the Framework is already ported
 * to a platform this files can be used directly on top of that without any
 * modifications.
 *
 */

//#include "sys_time_ext.h"
#include "rtc_drv.h"
#include "dal_time.h"
#include "sirf_types.h"
#include "sirf_pal.h"

#include "csr_time.h"


#define SECONDS_PER_MINUTE (60)
#define SECONDS_PER_HOUR   (SECONDS_PER_MINUTE * 60)
#define SECONDS_PER_DAY    (SECONDS_PER_HOUR * 24)

static tSIRF_DATE_TIME pal_time_struct;

extern ERR_TM_E TM_GetDateOfDayNum( // If successful, return ERR_TM_NONE                                   
    SCI_DATE_T  *date_ptr,                                            
    uint32      day_num            
    );
extern float MMIAPIACC_GetLocalTimeZone(void);
/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */



/**
 * @brief Get the current time from the system in milliseconds.
 * @return                    Relative milliseconds counter.
 */
tSIRF_UINT32 SIRF_PAL_OS_TIME_SystemTime( tSIRF_VOID )
{
   CsrTime time;
   
   time = CsrTimeGet(NULL);
   time = time / 1000;

   return time;

} /* SIRF_PAL_OS_TIME_SystemTime() */


/**
 * @brief Get the real-time clock value.
 * @param[out] date_time      Structure to populate with the RTC values.
 * @return                    Result code.
 */
tSIRF_RESULT SIRF_PAL_OS_TIME_DateTime( tSIRF_DATE_TIME *date_time )
{
    TB_RTC_TIME_T now;
    SCI_DATE_T sci_now_date;
    CsrTime msTime;

    if (date_time == NULL)
    {
        return SIRF_FAILURE;
    }

    RTC_Get(&now);/*lint !e507 !e64*/
    TM_GetDateOfDayNum(&sci_now_date, now.day);
    msTime = CsrTimeGet(NULL);

    date_time->year         = sci_now_date.year;
    date_time->month        = sci_now_date.mon;
    date_time->day          = sci_now_date.mday;
    date_time->minute       = now.min;
    date_time->hour         = now.hour;
    date_time->second       = now.sec;
    date_time->milliseconds = (msTime / 1000) % 1000;

    return SIRF_SUCCESS;

} /* SIRF_PAL_OS_TIME_DateTime() */

tSIRF_RESULT SIRF_PAL_OS_TIME_UTCDateTime( tSIRF_DATE_TIME *date_time )
{
    tSIRF_RESULT result;
    tSIRF_INT16 tz;

    result = SIRF_PAL_OS_TIME_DateTime(date_time);
    if (result == SIRF_FAILURE)
    {
        return SIRF_FAILURE;
    }

    /* ATTENTION! The 6800H platform seems to be hardcoded to always return timezone China */
    tz = (tSIRF_INT16)MMIAPIACC_GetLocalTimeZone();

    /* Take care of date wrapping */
    if ((date_time->hour - tz) < 0)
    {
        date_time->day = date_time->day - 1;
        date_time->hour = (date_time->hour + 24) - tz;
    }
    else if ((date_time->hour - tz) >= 24)
    {
        date_time->day = date_time->day + 1;
        date_time->hour = (date_time->hour - 24) - tz;
    }
    else
    {
        date_time->hour = date_time->hour - tz;
    }
	
    return SIRF_SUCCESS;
}

tSIRF_RESULT SIRF_PAL_OS_TIME_RTCRead( tSIRF_UINT16 *weekno, tSIRF_INT32 *timeOfWeek )
{
    (void)weekno;
    (void)timeOfWeek;

    return SIRF_FAILURE;
}

/**
 * @brief Conversion from UNIX timestamp to tSIRF_DATE_TIME.
 *
 * Convert a time value to a structure. 
 *
 *
 * @param timer Pointer to stored time. The time is represented as seconds 
 *              elapsed since midnight (00:00:00), January 1, 1970, 
 *              coordinated universal time (UTC).
 *
 * @return None
 */
tSIRF_DATE_TIME* SIRF_PAL_OS_TIME_gmtime(const tSIRF_TIME_T *timer)
{
    tSIRF_UINT32 myTimer;
    TB_RTC_TIME_T now;
    SCI_DATE_T sci_now_date;

    myTimer = (tSIRF_UINT32)*timer;

    /* There is a 10 year difference - compencate for that */
    myTimer = myTimer - ((3652 * SECONDS_PER_DAY));

    now.day = (myTimer/SECONDS_PER_DAY);
    myTimer = (myTimer - (now.day * SECONDS_PER_DAY));
    now.hour = (myTimer/SECONDS_PER_HOUR);
    myTimer = (myTimer - (now.hour * SECONDS_PER_HOUR));
    now.min= (myTimer/SECONDS_PER_MINUTE);
    myTimer = (myTimer - (now.min * SECONDS_PER_MINUTE));
    now.sec = myTimer;

    TM_GetDateOfDayNum(&sci_now_date, now.day);

    pal_time_struct.year         = sci_now_date.year;
    pal_time_struct.month        = sci_now_date.mon;
    pal_time_struct.day          = sci_now_date.mday;
    pal_time_struct.minute       = now.min;
    pal_time_struct.hour         = now.hour;
    pal_time_struct.second       = now.sec;
    pal_time_struct.milliseconds = 0;

    return &pal_time_struct;
}


/**
 * @}
 * End of file.
 */


