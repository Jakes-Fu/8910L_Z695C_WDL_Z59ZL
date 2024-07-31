/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */

#include "mmi_app_ia_java_trc.h"
#include "os_api.h"
#include "jktypes.h"
#include "jkvmtimer.h"
#include "mmijava_export.h"

//LOCAL SCI_TIMER_PTR     s_timer = SCI_NULL;
LOCAL JK_TIMER_HANDLER  s_jblend_vm_timer_handler = PNULL;
LOCAL uint8 			s_vm_timer_id = 0;

LOCAL void timer_func(uint8 timerID, uint32 param)
{
    if(s_jblend_vm_timer_handler != PNULL)
    {
        s_jblend_vm_timer_handler();
    }
}

JKSint JkSetVmTimerHandler(const JK_TIMER_HANDLER handler, JKSint recommended_interval)
{
    if (handler)
    {
	    //SCI_TRACE_LOW:"[JkSetVmTimerHandler] handler!=null."
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKVMTIMER_29_112_2_18_2_22_43_342,(uint8*)"");
        s_jblend_vm_timer_handler = handler;
        s_vm_timer_id= MMK_CreateTimerCallback((uint32)recommended_interval, timer_func, NULL, SCI_TRUE);
        MMK_StartTimerCallback(s_vm_timer_id, (uint32)recommended_interval, timer_func, NULL, SCI_TRUE);
    }
    else
    {
        //SCI_TRACE_LOW:"[JkSetVmTimerHandler] handler!==null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKVMTIMER_36_112_2_18_2_22_43_343,(uint8*)"");
	    if(s_vm_timer_id != 0)
	    {
            MMK_StopTimer(s_vm_timer_id);
            s_vm_timer_id = 0;        
	    }
        
	    s_jblend_vm_timer_handler = PNULL;
    }
    
    return (JKSint)recommended_interval;
}

