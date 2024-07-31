/*
 *
 * Copyright  1990-2009 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */

#include "mmi_sun_java_trc.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "std_header.h"
#include "os_api.h"
#include "dal_time.h"
#include "mmiwclk_export.h"

#include "javacall_time.h"
#include "javacall_platform_defs.h"
#include "javacall_memory.h"
#include "javacall_logging.h"

#define ENABLE_TIMER_SUSPEND_RESUME 0

typedef struct {
    SCI_TIMER_PTR handle;
    javacall_callback_func callback;
    javacall_bool cyclic;
} Timer;

static Timer timers[16];
        
/*static void callback(uint32 handle) {
    Timer *timer = (Timer*)handle;
    timer->callback(timer);
    if (JAVACALL_FALSE == timer->cyclic) {
        timer->handle = SCI_NULL;
    }
}*/
void javacall_time_initialize(void){
}
void javacall_time_finalize_all(void) {
}
/**
 *
 * Create a native timer to expire in wakeupInSeconds or less seconds.
 *
 * @param wakeupInMilliSecondsFromNow time to wakeup in milli-seconds
 *                              relative to current time
 *                              if -1, then ignore the call
 * @param cyclic <tt>JAVACALL_TRUE</tt>  indicates that the timer should be
 *               repeated cuclically,
 *               <tt>JAVACALL_FALSE</tt> indicates that this is a one-shot
 *               timer that should call the callback function once
 * @param func callback function should be called in platform's context once the timer
 *			   expires
 * @param handle A pointer to the returned handle that on success will be
 *               associated with this timer.
 *
 * @return on success returns <tt>JAVACALL_OK</tt>,
 *         or <tt>JAVACALL_FAIL</tt> or negative value on failure
 */
javacall_result javacall_time_initialize_timer(
                     int                      wakeupInMilliSecondsFromNow, 
                    javacall_bool            cyclic, 
                     javacall_callback_func   func,
					/*OUT*/ javacall_handle	*handle) {
    Timer *timer;
    for (timer = timers; timers + sizeof(timers) / sizeof(Timer) > timer
            && SCI_NULL != timer->handle;  timer++);

    if (timers + sizeof(timers) / sizeof(Timer) == timer) {
        //SCI_TRACE_LOW:"No free timer\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,JAVACALL_TIME_BASED_ON_PLATFORM_TIMER_86_112_2_18_3_27_12_15,(uint8*)"");
        return JAVACALL_FAIL;
    }

    if (JAVACALL_TRUE == cyclic)
        timer->handle = SCI_CreatePeriodTimer("java timer", (TIMER_FUN)func, (uint32)timer,
                                wakeupInMilliSecondsFromNow, SCI_AUTO_ACTIVATE);
    else
        timer->handle = SCI_CreateTimer("java timer", (TIMER_FUN)func, (uint32)timer,
                                wakeupInMilliSecondsFromNow, SCI_AUTO_ACTIVATE);
    
    if (SCI_NULL == timer->handle)
    {
        return JAVACALL_FAIL;
    }

    timer->callback = func;
    timer->cyclic = cyclic;
    *handle = (javacall_handle)timer;
    return JAVACALL_OK;
}

/**
 *
 * Disable a set native timer
 * @param handle The handle of the timer to be finalized
 *
 * @return on success returns <tt>JAVACALL_OK</tt>,
 *         <tt>JAVACALL_FAIL</tt> or negative value on failure
 */
javacall_result javacall_time_finalize_timer(javacall_handle handle) {
    Timer *timer = handle;
    
    uint32 result = SCI_DeleteTimer(timer->handle);
    //SCI_TRACE_LOW:"Timer deletion returns %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JAVACALL_TIME_BASED_ON_PLATFORM_TIMER_119_112_2_18_3_27_12_16,(uint8*)"d", result);
    timer->handle = SCI_NULL;
    if (SCI_SUCCESS == result)
        return JAVACALL_OK;
    else
        return JAVACALL_FAIL;
}

/*
 *
 * Temporarily disable timer interrupts. This is called when
 * the VM is about to sleep (when there's no Java thread to execute)
 *
 * @param handle timer handle to suspend
 *
 */
void javacall_time_suspend_ticks(javacall_handle handle) {
#if ENABLE_TIMER_SUSPEND_RESUME
    Timer *timer = handle;
    if (SCI_IsTimerActive(timer->handle) == SCI_TRUE) {
        SCI_DeactiveTimer(timer->handle);
    }
#endif
}

/*
 *
 * Enable  timer interrupts. This is called when the VM
 * wakes up and continues executing Java threads.
 *
 * @param handle timer handle to resume
 *
 */
void javacall_time_resume_ticks(javacall_handle handle) {
#if ENABLE_TIMER_SUSPEND_RESUME
    Timer *timer = handle;
    if (SCI_IsTimerActive(timer->handle) == SCI_FALSE) {
        SCI_ActiveTimer(timer->handle);
    }
#endif
}

/*
 * Suspend the current process sleep for ms milliseconds
 */
void javacall_time_sleep(javacall_uint64 ms){
    SCI_Sleep(ms);
    return;
}

LOCAL char* timeZone[24] = {"GMT-11:00", "GMT-10:00", "GMT-09:00", "GMT-08:00", "GMT-07:00", "GMT-06:00", 
                              "GMT-05:00", "GMT-04:00", "GMT-03:00", "GMT-02:00", "GMT-01:00", "GMT+00:00", 
                              "GMT+01:00", "GMT+02:00", "GMT+03:00", "GMT+04:00", "GMT+05:00", "GMT+06:00", 
                              "GMT+07:00", "GMT+08:00", "GMT+09:00", "GMT+10:00", "GMT+11:00", "GMT+12:00"};

/**
 * Return local timezone ID string. This string is maintained by this
 * function internally. Caller must NOT try to free it.
 *
 * This function should handle daylight saving time properly. For example,
 * for time zone America/Los_Angeles, during summer time, this function
 * should return GMT-07:00 and GMT-08:00 during winter time.
 *
 * @return Local timezone ID string pointer. The ID string should be in the
 *         format of GMT+/-??:??. For example, GMT-08:00 for PST.
 */
char* javacall_time_get_local_timezone(void){
    int timeZoneIndex;

    timeZoneIndex = (int)MMIAPIACC_GetLocalTimeZone();
    return timeZone[timeZoneIndex + 11];
}

#ifdef _MSC_VER
extern __int64 startTimeInMillisecond;
extern const __int64 deltaTime;
#else
extern long long startTimeInMillisecond;
extern const long long deltaTime;
#endif

/**
 * returns number of milliseconds elapsed since midnight(00:00:00), January 1, 1970,
 *
 * @return milliseconds elapsed since midnight (00:00:00), January 1, 1970
 */
javacall_int64 /*OPTIONAL*/ javacall_time_get_milliseconds_since_1970(void){    
    javacall_int64 timeInMilliSecond
                            = startTimeInMillisecond + SCI_GetTickCount();
    
    // SCI_TRACE_LOW("\ntime %lus, %ld.%03lds\n", TM_GetTotalSeconds() + 946684800,
    //                             (long)(timeInMilliSecond / 1000), (long)(timeInMilliSecond % 1000));

    return timeInMilliSecond;
}

/**
 * returns the number of seconds elapsed since midnight (00:00:00), January 1, 1970,
 *
 * @return seconds elapsed since midnight (00:00:00), January 1, 1970
 */
javacall_time_seconds /*OPTIONAL*/ javacall_time_get_seconds_since_1970(void){
    return TM_GetTotalSeconds() + deltaTime - MMIAPIACC_GetLocalTimeZone() * 3600;
}

/**
 * returns the milliseconds elapsed time counter
 *
 * @return elapsed time in milliseconds
 */
javacall_time_milliseconds /*OPTIONAL*/ javacall_time_get_clock_milliseconds(void){
    return SCI_GetTickCount();
}

#ifdef __cplusplus
}
#endif

