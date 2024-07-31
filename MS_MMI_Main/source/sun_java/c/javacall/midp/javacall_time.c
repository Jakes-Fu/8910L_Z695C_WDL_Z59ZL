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

#ifdef __cplusplus
extern "C" {
#endif

#include "os_api.h"
#include "dal_time.h"
#include "mmiwclk_export.h"

#include "javacall_time.h"
#include "javacall_platform_defs.h"
#include "javacall_memory.h"
#include "javacall_logging.h"
#include "mmi_appmsg.h"
//#include "mmijava_id.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmijava_config.h"
#include "mmijava_main.h"

#ifdef WIN32
#define MAX_TIMEOUT_VAL (0xffffffff - 0x00000013)
#else
#define MAX_TIMEOUT_VAL 0xffffffff
#endif

#define TIME_DBG //javacall_printf
#define ENABLE_TIMER_SUSPEND_RESUME 0

typedef struct _JTimerList {
    struct _JTimerList      *next;
    uint32              elapsetick; // 0; disposable timer / !0; cyclic
    uint32              abstick; // time scheduled to fire
    uint32              suspendtick; // remaining time when suspended
    javacall_callback_func  handler;
    javacall_bool           bPersistent;
} JTimerList;


#define JTIMR_NAME "JTIMER"
static SCI_TIMER_PTR __jtimer;
static JTimerList* __jtimerList;
static javacall_callback_func g_direct_jtimer_func=PNULL;

// Timeout callback is not in task context, but in ISR context
// Do whatever quickly and leave.

//lint -esym(552, call_para)
javacall_timer_callback_para call_para = {0};    /*lint !e552*/ //global varibale
static void __jtimr_handler(uint32 arg) {
    JTimerList *timer2fire, *jtimerList, *prev, *curr, *fireprev;
    uint32 curtick;

    // Skip the suspended timer
    timer2fire = __jtimerList;
    fireprev = NULL;
    while (timer2fire && timer2fire->suspendtick > 0) {
        fireprev = timer2fire;
        timer2fire = timer2fire->next;
    }

    curtick = SCI_GetTickCount();
    if ((timer2fire != NULL) && (timer2fire->abstick <= curtick)) {
        //curtick = timer2fire->abstick;
        // At least, the first timer should be fired
        jtimerList = timer2fire->next;
        //timer2fire->handler(timer2fire);
        if (timer2fire->handler == g_direct_jtimer_func) {
            TIME_DBG(LOG_STR("headr beat handler=0x%p, para=0x%p"), call_para.handler, call_para.para);
            timer2fire->handler((void*)timer2fire);
        } else { 
//            if (is_ignored_network_timer_notify(timer2fire->handler, timer2fire) == FALSE) {
        call_para.handler=timer2fire->handler;
        call_para.para=(void*)timer2fire;
            TIME_DBG(LOG_STR("handler=0x%p, para=0x%p, g_direct_jtimer_func=0x%x"), call_para.handler, call_para.para, g_direct_jtimer_func);
        
        //MMIAPICOM_OtherTaskToMMI( MMI_JAVA_WIN_ID, MSG_JAVA_TIMER_NOTIFY, &call_para, sizeof(javacall_timer_callback_para) );
        MMIJAVA_SendJVMMessage2MMI(APP_JAVA_BACKGROUND_TIMER_EVENT, BACKGROUND_TIMER_EVENT);
//            }
        }
        if (0==timer2fire->elapsetick) { // one-off timer
            javacall_free(timer2fire);
        } else {
            timer2fire->abstick = curtick + timer2fire->elapsetick;
            if (NULL==jtimerList ||
            (timer2fire->abstick <= jtimerList->abstick)) {
                timer2fire->next = jtimerList;
                jtimerList = timer2fire;
            } else {
                prev = jtimerList;
                curr = jtimerList->next;
                for (; curr && timer2fire->abstick > curr->abstick;
                    prev=curr, curr=curr->next)
        {
            ;
        }

                prev->next = timer2fire;
                timer2fire->next = curr;
            }
        }
    
        if (fireprev == NULL) {
            // First timer fired
            __jtimerList = jtimerList;
        } else {
            fireprev->next = jtimerList;
        }
        if (NULL != jtimerList) {
            if (jtimerList->abstick > curtick+1) {
                SCI_ChangeTimer(__jtimer, __jtimr_handler, jtimerList->abstick-curtick);
            } else { // should be fired right away but
                // We sacrifice some precision
                SCI_ChangeTimer(__jtimer, __jtimr_handler, 1);
            }
        } else {
            SCI_ChangeTimer(__jtimer, __jtimr_handler, MAX_TIMEOUT_VAL);
        }
    } else if (timer2fire == NULL) {
        // No active timer
        SCI_ChangeTimer(__jtimer, __jtimr_handler, MAX_TIMEOUT_VAL);
    } else {
        SCI_ChangeTimer(__jtimer, __jtimr_handler, timer2fire->abstick-curtick);
    }
    if (SCI_FALSE==SCI_IsTimerActive(__jtimer)) {
        SCI_ActiveTimer(__jtimer);
    }
}

#define LOCK_JTIMER()   if (NULL != __jtimerList) {SCI_ChangeTimer(__jtimer, (TIMER_FUN)__jtimr_handler, MAX_TIMEOUT_VAL);}

void javacall_time_initialize(void) {
    __jtimer = SCI_CreateTimer(JTIMR_NAME, (TIMER_FUN)__jtimr_handler, 0,
                            MAX_TIMEOUT_VAL, SCI_AUTO_ACTIVATE);

    g_direct_jtimer_func=PNULL;
    if (NULL==__jtimer) {
        //TIME_DBG("[javacall_time_initialize] timer create error"); 
        return;
    }
    __jtimerList = NULL;

    return;
}

void javacall_time_finalize_all(void) {
    JTimerList *prev, *cur, *timer2del;
    uint32 curtick;
    
    // protect critical section from timer ISR
    LOCK_JTIMER();
    
    g_direct_jtimer_func=PNULL;
    cur = __jtimerList;
    prev = __jtimerList = NULL; // start by emptying list
    for (;cur;) {
        if (cur->bPersistent==JAVACALL_TRUE) { // must outlive Java lifecycle
            if (NULL==prev) { // the first timer to remain
                prev = __jtimerList = cur;
            } else {
                prev->next = cur;
                prev = cur;
            }
            cur = cur->next;
        } else { // remove
            timer2del = cur;
            cur = cur->next;
            javacall_free(timer2del);
        }
    }

    // restart timer if needed
    if (NULL != __jtimerList) {
        curtick = SCI_GetTickCount();
        if (curtick >= __jtimerList->abstick) { // fire right away
            __jtimr_handler(NULL);
        } else { // schedule timer
            SCI_ChangeTimer(__jtimer, (TIMER_FUN)__jtimr_handler, __jtimerList->abstick - curtick);
        }
    } else {
        SCI_ChangeTimer(__jtimer, (TIMER_FUN)__jtimr_handler, MAX_TIMEOUT_VAL);
    }

    if (SCI_FALSE==SCI_IsTimerActive(__jtimer)) {
        SCI_ActiveTimer(__jtimer);
    }
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
					/*OUT*/ javacall_handle	*handle){
    JTimerList *newt, *cur, *prev;
    uint32 elapsetick, abstick;
    char timerName[SCI_MAX_NAME_SIZE];
    BOOLEAN timerActive;
    uint32  rescheduleTime, remainTime;

    newt = (JTimerList*)javacall_malloc(sizeof(JTimerList));
    if (NULL==newt) {
        //javacall_print("[javacall_time_initialize_timer] >>  adding new timer  OOM");
        return JAVACALL_OUT_OF_MEMORY;
    }
    elapsetick = (uint32)wakeupInMilliSecondsFromNow;

    // protect critical section from timer ISR
    LOCK_JTIMER();

    abstick = SCI_GetTickCount();
    abstick += elapsetick;

    SCI_GetTimerInfo(__jtimer, timerName, &timerActive, &remainTime, &rescheduleTime);

    // the new timer should be the first to fire
    if (NULL==__jtimerList || elapsetick < remainTime) {
        newt->next = __jtimerList;
        __jtimerList = newt;
    } else {
    // the new timer comes the second at earliest, say,
    // there is one timer already running at least.
        prev = __jtimerList; // can't be NULL
        cur = __jtimerList->next; // can be NULL
        for (; cur && abstick >= cur->abstick; prev=cur, cur=cur->next)
        {
            ;
        }
        prev->next = newt;
        newt->next = cur;
    }

    newt->elapsetick = (JAVACALL_TRUE==cyclic) ? elapsetick : 0 ;
    newt->abstick = abstick;
    newt->suspendtick = 0;
    newt->handler = func;
    newt->bPersistent = JAVACALL_FALSE;
    *handle = newt;

    // restart timer if needed
#define curtick abstick
    if (NULL != __jtimerList) {
        curtick = SCI_GetTickCount();
        if (curtick >= __jtimerList->abstick) { // fire right away
            __jtimr_handler(NULL);
        } else { // schedule timer
            SCI_ChangeTimer(__jtimer, (TIMER_FUN)__jtimr_handler, __jtimerList->abstick - curtick);
        }
    } else {
        SCI_ChangeTimer(__jtimer, (TIMER_FUN)__jtimr_handler, MAX_TIMEOUT_VAL);
    }
    if (SCI_FALSE==SCI_IsTimerActive(__jtimer)) {
        SCI_ActiveTimer(__jtimer);
    }
#undef curtick
    return JAVACALL_OK;
}

javacall_result javacall_time_set_persistent_timer(javacall_handle handle, int flag) {
    JTimerList * timer=(JTimerList *)handle;
    TIME_DBG(LOG_STR("handle=0x%x"));
    if (timer != NULL) {
        timer->bPersistent = (javacall_bool)flag;
    }
    return JAVACALL_OK;
}
/* This function is for java heart beat timer to avoid sending message to
 * platform. Do not call this excetpt for heart beat
 */
javacall_result javacall_time_set_direct_func(javacall_callback_func func) {
    javacall_printf(LOG_STR("func=0x%x"), func);
    if (g_direct_jtimer_func == PNULL) {
        g_direct_jtimer_func = func;
    }
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
    JTimerList *timer2del, *cur, *prev;
    uint32 curtick;

    if (NULL==__jtimerList)
        return JAVACALL_OK;

    // protect critical section from timer ISR
    LOCK_JTIMER();

    timer2del = (JTimerList*)handle;
    if (timer2del->handler == g_direct_jtimer_func)
        g_direct_jtimer_func=PNULL;

    // if the timer to delete is the current one, reset timer
    if (timer2del==__jtimerList) {
        __jtimerList = timer2del->next;
        javacall_free(timer2del);
    } else { // timer to delete comes the second at earliest.
        prev = __jtimerList; // can't be NULL
        cur = __jtimerList->next; // can be NULL
        for (; cur; prev=cur, cur=cur->next) {
            if (cur==timer2del) {
                prev->next = cur->next;
                javacall_free(timer2del);
                break;
            }
        }
    }

    // restart timer if needed
    if (NULL != __jtimerList) {
        curtick = SCI_GetTickCount();
        if (curtick >= __jtimerList->abstick) { // fire right away
            __jtimr_handler(NULL);
        } else { // schedule timer
            SCI_ChangeTimer(__jtimer, (TIMER_FUN)__jtimr_handler, __jtimerList->abstick - curtick);
        }
    } else {
        SCI_ChangeTimer(__jtimer, (TIMER_FUN)__jtimr_handler, MAX_TIMEOUT_VAL);
    }
    if (SCI_FALSE==SCI_IsTimerActive(__jtimer)) {
        SCI_ActiveTimer(__jtimer);
    }

    return JAVACALL_OK;
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
    JTimerList *timer2sus;
    uint32 curtick;

    if (NULL==__jtimerList)
        return;

    // protect critical section from timer ISR
    LOCK_JTIMER();

    timer2sus = (JTimerList*)handle;

    curtick = SCI_GetTickCount();
    timer2sus->suspendtick = (timer2sus->abstick - curtick) > 0? (timer2sus->abstick - curtick) : 1;

    // restart timer if needed
    if (NULL != __jtimerList) {

        if (curtick >= __jtimerList->abstick) { // fire right away
            __jtimr_handler(NULL);
        } else { // schedule timer
            SCI_ChangeTimer(__jtimer, (TIMER_FUN)__jtimr_handler, __jtimerList->abstick - curtick);
        }
    } else {
        SCI_ChangeTimer(__jtimer, (TIMER_FUN)__jtimr_handler, MAX_TIMEOUT_VAL);
    }
    if (SCI_FALSE==SCI_IsTimerActive(__jtimer)) {
        SCI_ActiveTimer(__jtimer);
    }
    return;
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
void javacall_time_resume_ticks(javacall_handle handle){
#if ENABLE_TIMER_SUSPEND_RESUME
    JTimerList *timer2resume, *cur, *prev;
    uint32 curtick;

    if (NULL==__jtimerList)
        return;

    // protect critical section from timer ISR
    LOCK_JTIMER();

    timer2resume = (JTimerList*)handle;
    curtick = SCI_GetTickCount();
    timer2resume->abstick = curtick + timer2resume->suspendtick;
    timer2resume->suspendtick = 0;

    //Rearrange the timer list
    if (__jtimerList->next != NULL) {
        prev = __jtimerList; // can't be NULL
        cur = __jtimerList->next; // can be NULL
        for (; cur; prev=cur, cur=cur->next) {
            if (cur==timer2resume) {
                prev->next = cur->next;
                break;
            }
        }

        if (timer2resume->abstick <= __jtimerList->abstick) {
            timer2resume->next = __jtimerList;
            __jtimerList = timer2resume;
        } else {
            prev = __jtimerList;
            cur = __jtimerList->next;
            for (; cur && timer2resume->abstick > cur->abstick;
                            prev=cur, cur=cur->next);
            prev->next = timer2resume;
            timer2resume->next = cur;
        }
    }

    // restart timer if needed
    if (NULL != __jtimerList) {
        if (curtick >= __jtimerList->abstick) { // fire right away
            __jtimr_handler(NULL);
        } else { // schedule timer
            SCI_ChangeTimer(__jtimer, (TIMER_FUN)__jtimr_handler, __jtimerList->abstick - curtick);
        }
    } else {
        SCI_ChangeTimer(__jtimer, (TIMER_FUN)__jtimr_handler, MAX_TIMEOUT_VAL);
    }
    if (SCI_FALSE==SCI_IsTimerActive(__jtimer)) {
        SCI_ActiveTimer(__jtimer);
    }

    return;
#endif
}

/*
 * Suspend the current process sleep for ms milliseconds
 */
void javacall_time_sleep(javacall_uint64 ms){
    SCI_Sleep((uint32)ms);
    return;
}

/**
 *
 * Create a native timer to expire in wakeupInSeconds or less seconds.
 * At least one native timer can be used concurrently.
 * If a later timer exists, cancel it and create a new timer
 *
 * @param type type of alarm to set, either JAVACALL_TIMER_PUSH, JAVACALL_TIMER_EVENT
 *                              or JAVACALL_TIMER_WATCHDOG
 * @param wakeupInMilliSecondsFromNow time to wakeup in milli-seconds
 *                              relative to current time
 *                              if -1, then ignore the call
 *
 * @return <tt>JAVACALL_OK</tt> on success, <tt>JAVACALL_FAIL</tt> on failure
 */
////javacall_result	javacall_time_create_timer(javacall_timer_type type, int wakeupInMilliSecondsFromNow){
//    return JAVACALL_FAIL;
//}
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
    int16 timeZoneIndex = 0;

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
    return TM_GetTotalSeconds() + deltaTime - (uint32)(MMIAPIACC_GetLocalTimeZone() * 3600);    /*lint !e712*/
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

