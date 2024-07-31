/****************************************************************************
** File Name:      dataCounterUtil.c                            
** Author:                                                                  
** Date:            2015/08/24       
** Description:    This file is used to process msg to main window of CALCULATION
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*                           

****************************************************************************/
#define _MMIDATACOUNTER_UTIL_C_  

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_idle_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guistring.h"
#include "mmipdp_export.h"
#include "datacounter_internal.h"
#include "datacounter_image.h"
#include "datacounter_id.h"

PUBLIC void MMIAPIDataCounter_ThresholdTips(MN_DUAL_SYS_E dual_sys, MMI_STRING_T *text1_ptr,
                               MMI_STRING_T *text2_ptr, MMIPUB_HANDLE_FUNC handle_func)
{
    MMI_WIN_ID_T win_id = MMIACC_DATA_COUNTER_WIN_OPEN_ID;
    
    MMI_IMAGE_ID_T   image_id = IMAGE_ALERT_DATACOUNTER_TIPS;

    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    SCI_TraceLow("MMIAPIDataCounter_ThresholdTips start");
    MMIPUB_OpenAlarmDialogWin(2, text1_ptr, text2_ptr, PNULL, win_id, image_id, IMAGE_NULL, WIN_THREE_LEVEL, MMIPUB_SOFTKEY_CUSTOMER, handle_func);

}

void readIncomingCallDuration(MN_DUAL_SYS_E dual_sys, MMI_STRING_T* time_str_ptr)
{

	uint32 incoming_call_time_duration = 0;
	CC_CALL_TIME_COUNT_T  incoming_calls_time[MMI_DUAL_SYS_MAX] = {0};
	MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
	
       if ( dual_sys >=  MMI_DUAL_SYS_MAX )
	{
		call_TimeCountToStr(time_str_ptr, incoming_call_time_duration);
		return;
	}
       
	
	MMINV_READ(MMINV_CL_RECEIVEDCALLS_TIME, incoming_calls_time, return_value);
       if (MN_RETURN_FAILURE != return_value)
	{
		#ifdef MMI_CSP_SUPPORT
			if ( incoming_calls_time[dual_sys].line1_call_time != 0)
			{
				incoming_call_time_duration = incoming_calls_time[dual_sys].line1_call_time;
			}
			else
			{
				incoming_call_time_duration = incoming_calls_time[dual_sys].line2_call_time;
			}
		#else
			incoming_call_time_duration = incoming_calls_time[dual_sys].call_time;
		#endif
		
	}

	call_TimeCountToStr(time_str_ptr, incoming_call_time_duration);

}

void readOutgoingCallDuration(MN_DUAL_SYS_E dual_sys, MMI_STRING_T* time_str_ptr)
{

	uint32 outgoing_call_time_duration = 0;
	CC_CALL_TIME_COUNT_T  outgoing_calls_time[MMI_DUAL_SYS_MAX] = {0};
	MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
       if ( dual_sys >=  MMI_DUAL_SYS_MAX )
	{
		call_TimeCountToStr(time_str_ptr, outgoing_call_time_duration);
		return;
	}
       
	MMINV_READ(MMINV_CL_DIALEDCALLS_TIME, outgoing_calls_time, return_value);
       if (MN_RETURN_FAILURE != return_value)
	{
		#ifdef MMI_CSP_SUPPORT
			if ( outgoing_calls_time[dual_sys].line1_call_time != 0)
			{
				outgoing_call_time_duration = outgoing_calls_time[dual_sys].line1_call_time;
			}
			else
			{
				outgoing_call_time_duration = outgoing_calls_time[dual_sys].line2_call_time;
			}
		#else
			outgoing_call_time_duration = outgoing_calls_time[dual_sys].call_time;
		#endif
		
	}

	call_TimeCountToStr(time_str_ptr, outgoing_call_time_duration);
}


void readAllCallDuration(MN_DUAL_SYS_E dual_sys, MMI_STRING_T* time_str_ptr)
{

	uint32 all_call_time_duration = 0;

	CC_CALL_TIME_COUNT_T  all_calls_time[MMI_DUAL_SYS_MAX] = {0};
	MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
       if ( dual_sys >=  MMI_DUAL_SYS_MAX )
	{
		call_TimeCountToStr(time_str_ptr, all_call_time_duration);
		return;
	}
       
	MMINV_READ(MMINV_CL_ALLCALLS_TIME, all_calls_time, return_value);
       if (MN_RETURN_FAILURE != return_value)
	{
		#ifdef MMI_CSP_SUPPORT
			if ( all_calls_time[dual_sys].line1_call_time != 0)
			{
				all_call_time_duration = all_calls_time[dual_sys].line1_call_time;
			}
			else
			{
				all_call_time_duration = all_calls_time[dual_sys].line2_call_time;
			}
		#else
			all_call_time_duration = all_calls_time[dual_sys].call_time;
		#endif
		
	}

	call_TimeCountToStr(time_str_ptr, all_call_time_duration);
}


LOCAL void call_TimeCountToStr(MMI_STRING_T* time_str_ptr, uint32 time_count)
{
    uint8 time_str_arr[CALL_DISPLAY_TIME_LEN + 2] = {0};
    if (PNULL != time_str_ptr && PNULL != time_str_ptr->wstr_ptr)
    {
        sprintf((char*)time_str_arr,
            "%02d h %02d min %02d ms",
            (int)(time_count / CALL_HOUR_TO_SECOND),
            (int)((time_count % CALL_HOUR_TO_SECOND) / CALL_MINUTE_TO_SECOND),
            (int)(time_count % CALL_MINUTE_TO_SECOND)
            );
        time_str_ptr->wstr_len = MIN(time_str_ptr->wstr_len, strlen((char*)time_str_arr));  /*lint !e666 */ 
        //prompt_str.wstr_ptr = time_str_arr;
        MMI_STRNTOWSTR(time_str_ptr->wstr_ptr,
            time_str_ptr->wstr_len,
            (uint8 *)time_str_arr,
            time_str_ptr->wstr_len,
            time_str_ptr->wstr_len);
    }
}



