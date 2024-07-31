#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "mmk_timer.h"
#include "mmiphone_export.h"
#include "mmi_filemgr.h"
#include "version.h"
#include "socket_api.h"
#include "mmipdp_export.h"

#include "mmieng_internal.h"
#include "mmieng_nv.h"
#include "mmiconnection_export.h"
#include "mmipdp_internal.h"
#include "mmi_module.h"
#include "mmipub.h"
#include "dal_time.h"
#include "mmk_authen.h"
#include "mmi_nv.h"
#include "mmialarm_service.h"

#include "mmicmccdm_nv.h"

#include "mmk_app.h"
#include "mmicmccdm_lwm2m.h"
#include "mmicmccdm_main.h"

#include "socket.h"
#ifdef LWM2M_SUPPORT
#include "lwm2msdk.h"
#endif


/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
#define START_RETRY_MAX 	(3)
#define REGISTER_RETRY_MAX 	(3)


LOCAL BOOLEAN b_cmccdm_running = FALSE;
LOCAL uint8 s_cmccdm_starter = 0;
LOCAL uint8 s_cmccdm_pdp_retry = 0;
LOCAL uint8 s_cmccdm_register_times = 0;
extern uint8 s_register_timer;

LOCAL void MMICMCCDM_SelfDMCoapRun(uint8 timer_id, uint32 param);
PUBLIC void MMI_Coap_Run(void);
PUBLIC void MMICMCCDM_ResetEvent(int loop_mins);
PUBLIC BOOLEAN MMICMCCDM_CheckEvent(void);


/**--------------------------------------------------------------------------*
**                                                         *
**--------------------------------------------------------------------------*/
PUBLIC BOOLEAN MMICMCCDM_IsChinaMobile(MN_DUAL_SYS_E sim)
{
    // is the sim ChinaMobile ?
    PHONE_SERVICE_STATUS_T      service_status= {0};

    MMIAPIPHONE_GetCurServiceStatus(sim, &service_status);
    SCI_TRACE_LOW("[CMCC DM] sim [%d] mcc =%d, mnc = %d", sim, service_status.plmn.mcc, service_status.plmn.mnc);

    if( (460 == service_status.plmn.mcc) &&
        ((00 == service_status.plmn.mnc ) || (02 == service_status.plmn.mnc ) || (07 == service_status.plmn.mnc )))
    {
        SCI_TRACE_LOW("[CMCC DM] sim [%d] mcc =%d, mnc = %d (CMCC)", sim, service_status.plmn.mcc, service_status.plmn.mnc);
        return TRUE;
    }

    return FALSE;
}

LOCAL void MMICMCCDM_SelfDMCoapRun(uint8 timer_id, uint32 param)
{
    MMK_StopTimer(s_cmccdm_starter);
    s_cmccdm_starter = 0;

    if (!MMICMCCDM_ConnectPDP())

    {
        if (s_cmccdm_pdp_retry++ < START_RETRY_MAX)
        {
            SCI_TraceLow("[CMCC DM] Timer -- Retry to Start PDP [retry : %d times]", s_cmccdm_pdp_retry);
            s_cmccdm_starter = MMK_CreateTimerCallback(10 * 1000, MMICMCCDM_SelfDMCoapRun, PNULL, FALSE);
        }
        else
        {
            s_cmccdm_pdp_retry = 0;
            SCI_TraceLow("[CMCC DM] Timer -- Start PDP [final fail !!!] Re-initiate in 1 hour");
	     s_cmccdm_starter = MMK_CreateTimerCallback(3600 * 1000, MMI_Coap_Run, PNULL, FALSE);
        }
    }
}

PUBLIC void MMI_Coap_Run(void)
{
    if (b_cmccdm_running)
    {
        SCI_TraceLow("[CMCC_DM] selfDM_Running.");
        return;
    }

    MMK_StopTimer(s_cmccdm_starter);

    if (MMIAPICONNECTION_isGPRSSwitchedOff())
    {
    
        SCI_TraceLow("[CMCC_DM] GPRS is off !!! --- [can't send register package SFR-STOP].");
        return;
    }

    b_cmccdm_running = TRUE;
    SCI_TraceLow("[CMCC_DM] Self Register By COAP");
    s_cmccdm_pdp_retry = 0;
    s_cmccdm_register_times = 0;
    s_cmccdm_starter = MMK_CreateTimerCallback(10*1000, MMICMCCDM_SelfDMCoapRun, PNULL, FALSE);
}

PUBLIC void MMICMCCDM_DMRun(void)
{
    uint8 i_sim;
    BOOLEAN is_cmcc = FALSE;
    for(i_sim = 0; i_sim < MMI_DUAL_SYS_MAX; i_sim++)
    {
        if (MMIAPIPHONE_GetSimExistedStatus(i_sim))
        {
            if(SIM_STATUS_OK != MMIAPIPHONE_GetSimStatus(i_sim))
            { 
                
                SCI_TraceLow("[CMCC DM] Sim [%d] Card Not Ready.", i_sim);
                return;
            }
	     if(SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(i_sim) && MMICMCCDM_IsChinaMobile(i_sim))
	     {
	        is_cmcc = TRUE;
	     }
        }
    }
     if(is_cmcc)	
     	{
     	  SCI_TraceLow("[CMCC DM] MMI_Coap_Run()", i_sim);
     	   MMI_Coap_Run();
     	}
       
}

PUBLIC void MMICMCCDM_Update(int loop)
{   
    b_cmccdm_running = FALSE;
  if (s_register_timer > 0)
    {
        MMK_StopTimer(s_register_timer);
    }
    LWM2M_SDK_FINI();
    MMIAPIPDP_Deactive(MMI_MODULE_CMCCDM);
    if (loop > 0)
      {
           MMICMCCDM_ResetEvent(loop); 
    	}
}

PUBLIC void MMICMCCDM_ReTry(int seconds)
{
  
    SCI_TraceLow("[CMCC DM] Retry.");
#ifdef LWM2M_SUPPORT
    LWM2M_SDK_STOP();
    LWM2M_SDK_FINI();
#endif
    MMIAPIPDP_Deactive(MMI_MODULE_CMCCDM);
    b_cmccdm_running = FALSE;
    MMK_StopTimer(s_cmccdm_starter);	

    if (++s_cmccdm_register_times < REGISTER_RETRY_MAX)
    {
        b_cmccdm_running = TRUE;
        s_cmccdm_starter = MMK_CreateTimerCallback(seconds*1000, MMICMCCDM_SelfDMCoapRun, PNULL, FALSE);
    }
}


LOCAL BOOLEAN MMICMCCDM_Event_Callback(MMIALMSVC_ID_T svc_id, MMIALMSVC_EVENT_TYPE_E svc_type)
{
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};
  
    uint8 isim = 0;
   
    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);

    SCI_TraceLow("[CMCCDM] MMICMCCDM_Event_Callback -- svc_id = %d", svc_id);
    SCI_TraceLow("[CMCCDM] MMICMCCDM_Event_Callback Time : %d-%d-%d %d:%d", sys_date.year,sys_date.mon,sys_date.mday,sys_time.hour,sys_time.min);

    MMICMCCDM_DMRun();
    return TRUE;
}

LOCAL  void IncreaseDateByOne(SCI_DATE_T  *pdate_ptr)
{
    const uint8 month_info[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const uint8 month_info_leap[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (pdate_ptr->mon<1 ||pdate_ptr->mon>12)
    {
        SCI_TRACE_LOW("[CMCCDM] IncreaseDateByOne error! mon=%d", pdate_ptr->mon);
        return;
    }
    if(MMIAPICOM_IsLeapYear((pdate_ptr->year)))
    {
        if((pdate_ptr->mon == 12)&&(pdate_ptr->mday == 31))
        {
            pdate_ptr->mday = 1;
            pdate_ptr->mon = 1;
            pdate_ptr->year++; 
        }
        else if(pdate_ptr->mday == month_info_leap[pdate_ptr->mon-1])
        {
            pdate_ptr->mday = 1;
            pdate_ptr->mon++;
        }
        else
        {
            pdate_ptr->mday++;
        }
    }
    else        // not leap year
    {
        if((pdate_ptr->mon == 12)&&(pdate_ptr->mday == 31))
        {
            pdate_ptr->mday = 1;
            pdate_ptr->mon = 1;
            pdate_ptr->year++;
        }
        else if(pdate_ptr->mday == month_info[pdate_ptr->mon-1])
        {
            pdate_ptr->mday = 1;
            pdate_ptr->mon++;
        }
        else
        {
            pdate_ptr->mday++;
        }
    }

    if (pdate_ptr->year >MMICOM_DATE_MAX_YEAR)
    {
        pdate_ptr->year = MMICOM_DATE_MIN_YEAR;       // when 2099, return 1980
    }
}

LOCAL  void IncreaseTimeByMin( SCI_DATE_T *pdate, SCI_TIME_T *ptime, int min)
{
    int i = 0;

    i = ptime->min + min;
    if (i < 60)
    {
        ptime->min = i;
    }
    else
    {
        ptime->hour += i/60;
        ptime->min = i%60;

        for (i = 0; i < ptime->hour/24; i++)
        {
            IncreaseDateByOne(pdate);
        }
        ptime->hour = ptime->hour%24;
    }
}

PUBLIC void MMICMCCDM_ResetEvent(int loop_mins)
{
    int32 sfr_event_id = 0;
    MMIALMSVC_EVENT_REG_INFO_T content_event_info = {0};
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};

    if (loop_mins <= 0)
    	return;
    
    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);

    //get event id from nv ...
    sfr_event_id = MMICMCCDM_GetEventID();
    
    if (sfr_event_id > 0)
    {
        MMIALMSVC_UnRegService(sfr_event_id, TRUE);
    }

    SCI_TraceLow("[CMCCDM] Current Time : %d-%d-%d %d:%d", 
		sys_date.year,sys_date.mon,sys_date.mday,
		sys_time.hour,sys_time.min);

    IncreaseTimeByMin(&sys_date, &sys_time, loop_mins);

    SCI_MEMSET(&content_event_info, 0x00, sizeof(content_event_info));

    content_event_info.event_type = ALMSVC_EVENT_OTHER;
    content_event_info.event_time.year = sys_date.year;
    content_event_info.event_time.mon = sys_date.mon;
    content_event_info.event_time.mday = sys_date.mday;
    content_event_info.event_time.hour = sys_time.hour;
    content_event_info.event_time.min = sys_time.min;
    content_event_info.event_time.sec = 0;
    content_event_info.callback = (MMIALMSVC_HANDLE_CALLBACK)MMICMCCDM_Event_Callback;

    SCI_TraceLow("[CMCCDM] Add a New Event %d-%d-%d %d:%d", 
		content_event_info.event_time.year,content_event_info.event_time.mon,content_event_info.event_time.mday,
		content_event_info.event_time.hour,content_event_info.event_time.min);

    sfr_event_id = MMIALMSVC_RegService(&content_event_info, TRUE, TRUE);

    SCI_TraceLow("[CMCCDM] Add a New Event event_id = %d", sfr_event_id);

    MMIALMSVC_SetAlmService();

    //write event id to nv ...
    MMICMCCDM_SetEventID(sfr_event_id);
}

PUBLIC BOOLEAN MMICMCCDM_CheckEvent(void)
{
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};
    SCI_ALARM_T cur_time = {0};
    int32 cmccdm_event_id = 0;

    cmccdm_event_id = MMICMCCDM_GetEventID();

    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);

    if (sys_time.hour == 0 && sys_time.min == 0 && sys_time.sec == 0)
        return FALSE;

    cur_time.year = sys_date.year;
    cur_time.mon = sys_date.mon;
    cur_time.mday = sys_date.mday;
    cur_time.hour = sys_time.hour;
    cur_time.min = sys_time.min;
    cur_time.sec = 0;

    if (MMIALMSVC_IsValidTimedSvc(cmccdm_event_id, cur_time))
    {
        SCI_TraceLow("[CMCCDM] Event Valid (cur time :  %d-%d-%d %d:%d)", 
            cur_time.year, cur_time.mon, cur_time.mday, cur_time.hour, cur_time.min);
        return FALSE;
    }

    MMICMCCDM_Event_Callback(cmccdm_event_id, 0);

   return TRUE;
}

PUBLIC void CMCCDM_LongTrace(const char *string, int len)
{
    if (len < 128)
    {
        SCI_TraceLow("[CMCC DM] :%s ",string);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TraceLow("[CMCC DM] :%s ",buff);
        
          p += l;
          len -= l;
        }
    }
}

PUBLIC void CMCCDM_BinTrace(const char *src, int len)
{
    int i;

    for (i = 0; i + 15 < len; i+= 16)
    {
       SCI_TraceLow("[CMCC DM] :0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x",
	   	src[i+0], src[i+1], src[i+2], src[i+3], src[i+4], src[i+5], src[i+6], src[i+7], 
	   	src[i+8], src[i+9], src[i+10], src[i+11], src[i+12], src[i+13], src[i+14], src[i+15]);
    }
    if ( i < len)
    {
        char buff[128];
	 for( ; i < len; i++)
	 {
	     char tmp[8];
	    sprintf (tmp, "0x%.2x, ", src[i]);
	    strcat (buff, tmp);
	 }
	 SCI_TraceLow("[CMCC DM] :%s", buff);
    }
}

