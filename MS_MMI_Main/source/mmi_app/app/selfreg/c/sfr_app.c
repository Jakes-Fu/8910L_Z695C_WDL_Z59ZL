#ifdef SFR_SUPPORT_CTCC
#undef ENABLE_HTTPC
#endif

#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "mmk_timer.h"
#include "mmiphone_export.h"
#include "mmi_filemgr.h"
#ifdef ENABLE_HTTPC
#include "httpc_api.h"
#else 
#include "http_api.h"
#endif
#include "version.h"
#include "socket_api.h"
#include "mmipdp_export.h"
#include "in_message.h"
#include "mmieng_internal.h"
#include "mmisms_app.h"
#include "mmieng_nv.h"
#include "mmiconnection_export.h"
#include "mmipdp_internal.h"
#include "mmi_module.h"
#include "dal_time.h"
#include "mmk_authen.h"
#include "mmisms_set.h"
#include "mmi_nv.h"
#include "mmialarm_service.h"

#include "sfr_carrier_manager.h"
#include "sfr_header.h"

#define START_RETRY_MAX 	(5)
#define REGISTER_RETRY_MAX 	(10)


LOCAL BOOLEAN b_sfr_running = FALSE;
LOCAL uint8 s_sfr_starter = 0;
LOCAL uint8 s_sfr_pdp_retry = 0;
LOCAL uint8 s_sfr_register_times = 0;

uint8 g_sfr_main_sim_sys = MN_DUAL_SYS_1;
uint8 g_sfr_sub_sim_sys = MN_DUAL_SYS_2;
uint8 g_sfr_cdma_sim_sys = MN_DUAL_SYS_1;

PUBLIC void MMISFR_SelfRegisterByHttp(uint8 timer_id, uint32 param);
PUBLIC void MMISFR_ResetEvent(void);
PUBLIC void MMISFR_CheckEvent(void);

PUBLIC BOOLEAN MMISFR_IsChinaTelecom(MN_DUAL_SYS_E sim)
{
    // is the sim China Telecom ?
    PHONE_SERVICE_STATUS_T      service_status= {0};

    MMIAPIPHONE_GetCurServiceStatus(sim, &service_status);
    SCI_TRACE_LOW("[Self Reg] sim [%d] mcc =%d, mnc = %d", sim, service_status.plmn.mcc, service_status.plmn.mnc);

    if( (460 == service_status.plmn.mcc) &&
        ((03 == service_status.plmn.mnc ) || (05 == service_status.plmn.mnc ) || (11 == service_status.plmn.mnc )))
    {
        SCI_TRACE_LOW("[Self Reg] sim [%d] mcc =%d, mnc = %d (CTCC)", sim, service_status.plmn.mcc, service_status.plmn.mnc);
        return TRUE;
    }

    return FALSE;
}

PUBLIC BOOLEAN MMISFR_IsChinaMobile(MN_DUAL_SYS_E sim)
{
    // is the sim China Telecom ?
    PHONE_SERVICE_STATUS_T      service_status= {0};

    MMIAPIPHONE_GetCurServiceStatus(sim, &service_status);
    SCI_TRACE_LOW("[Self Reg] sim [%d] mcc =%d, mnc = %d", sim, service_status.plmn.mcc, service_status.plmn.mnc);

    if( (460 == service_status.plmn.mcc) &&
        ((00 == service_status.plmn.mnc ) || (02 == service_status.plmn.mnc ) || (07 == service_status.plmn.mnc )))
    {
        SCI_TRACE_LOW("[Self Reg] sim [%d] mcc =%d, mnc = %d (CMCC)", sim, service_status.plmn.mcc, service_status.plmn.mnc);
        return TRUE;
    }

    return FALSE;
}

PUBLIC void MMISFR_Http_Run(void);

PUBLIC void MMISFR_Run(void)
{
    uint8 i_sim;

    for(i_sim = 0; i_sim < MMI_DUAL_SYS_MAX; i_sim++)
    {
        if (MMIAPIPHONE_GetSimExistedStatus(i_sim))
        {
            if(SIM_STATUS_OK != MMIAPIPHONE_GetSimStatus(i_sim))
            {
                SCI_TraceLow("[Self Reg] Sim [%d] Card Not Ready.", i_sim);
                return;
            }
            if (MMISFR_IsChinaTelecom(i_sim) )
            {
                g_sfr_cdma_sim_sys = i_sim;
                if(MMISFR_IsImsiChanged(i_sim))
                {
                    SCI_TraceLow("[Self Reg] imsi Changed i_sim=%d", i_sim);
                    MMISFR_SMS_Run(i_sim);
                }
            }
        }
    }

    if((g_sfr_linker.carrier = SFRM_GetFirstRegCarrier()) != NULL)
    {
        MMISFR_Http_Run();
    } 
    else 
    {
        MMISFR_CheckEvent();
    }
}

PUBLIC void MMISFR_End(void)
{
    if (g_sfr_linker.carrier != NULL)
    {
        SCI_TraceLow("[Self Reg] Save the Carrier NV");
        g_sfr_linker.carrier->carrier_saver(g_sfr_linker.carrier->carr_context);
        g_sfr_linker.carrier->carier_free(g_sfr_linker.carrier->carr_context);
    }

    g_sfr_linker.carrier = SFRM_GetNextRegCarrier();

    if (g_sfr_linker.carrier != NULL)
    {
        g_sfr_linker.http_return= FALSE;
	 SCI_TraceLow("[Self Reg] Start Next Reg Carrier.");
        SlfReg_PostPackageRequest();
    }
    else
    {
    	 SCI_TraceLow("[Self Reg] No Next Reg Carrier. ---- END");
        SlfReg_CloseRequest();
        MMIAPIPDP_Deactive(MMI_MODULE_SFR);
        b_sfr_running = FALSE;
        MMISFR_ResetEvent();
    }
}

PUBLIC void MMISFR_ReTry(void)
{
    if (g_sfr_linker.carrier != NULL)
    {
        g_sfr_linker.carrier->carier_free(g_sfr_linker.carrier->carr_context);
    }
    SCI_TraceLow("[Self Reg] Retry.");

    SlfReg_CloseRequest();
    MMIAPIPDP_Deactive(MMI_MODULE_SFR);
    b_sfr_running = FALSE;
    MMK_StopTimer(s_sfr_starter);	

    if (++s_sfr_register_times < REGISTER_RETRY_MAX)
    {
        b_sfr_running = TRUE;
        s_sfr_starter = MMK_CreateTimerCallback(10*1000, MMISFR_SelfRegisterByHttp, PNULL, FALSE);
    }
}
PUBLIC void MMISFR_Check_SimExited(MN_DUAL_SYS_E dual_sys)
{
	// added by zdtone for only sim2 reg
    if(dual_sys == MN_DUAL_SYS_2)
    {
        if (FALSE == MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)
            || (FALSE == MMISFR_IsChinaTelecom(MN_DUAL_SYS_1)))
        {
            g_sfr_main_sim_sys = MN_DUAL_SYS_2;
            g_sfr_sub_sim_sys = MN_DUAL_SYS_1;
        }
        else
        {
            g_sfr_main_sim_sys = MN_DUAL_SYS_1;
            g_sfr_sub_sim_sys = MN_DUAL_SYS_2;
       }
    }
}

PUBLIC void MMISFR_Http_Run(void)
{
    if (b_sfr_running)
    {
        SCI_TraceLow("[Self Reg] Running.");
        return;
    }
    
	// added by zdtone for only sim2 reg
    MMISFR_Check_SimExited(g_sfr_cdma_sim_sys);

    MMK_StopTimer(s_sfr_starter);
    b_sfr_running = TRUE;
    SCI_TraceLow("[Self Reg] Self Register by http.");
    s_sfr_pdp_retry = 0;
    s_sfr_register_times = 0;
    s_sfr_starter = MMK_CreateTimerCallback(10*1000, MMISFR_SelfRegisterByHttp, PNULL, FALSE);
}

PUBLIC void MMISFR_SelfRegisterByHttp(uint8 timer_id, uint32 param)
{
    MMK_StopTimer(s_sfr_starter);
    s_sfr_starter = 0;

    if (!MMISFR_ConnectPDP())
    {
	 if (s_sfr_pdp_retry++ < START_RETRY_MAX)
	 {
            SCI_TraceLow("[Self Reg] Timer -- Retry to Start PDP [retry : %d times]", s_sfr_pdp_retry);
            s_sfr_starter = MMK_CreateTimerCallback(10 * 1000, MMISFR_SelfRegisterByHttp, PNULL, FALSE);
	 }
	 else
	 {
	     SCI_TraceLow("[Self Reg] Timer -- Start PDP [final fail !!!]");
	 }
    }
}

LOCAL BOOLEAN MMISFR_Event_Callback(MMIALMSVC_ID_T svc_id, MMIALMSVC_EVENT_TYPE_E svc_type)
{
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};
    char sim_iccid[SFR_ICCID_LENGTH + 1] = {0};
    uint8 isim = 0;
   
    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);

    SCI_TraceLow("[Self Reg] MMISFR_Event_Callback -- svc_id = %d", svc_id);
    SCI_TraceLow("[Self Reg] MMISFR_Event_Callback Time : %d-%d-%d %d:%d", sys_date.year,sys_date.mon,sys_date.mday,sys_time.hour,sys_time.min);

    // 1. clean the saved meid ....
    memset (sim_iccid, '0', SFR_ICCID_LENGTH);
    for (isim = 0 ; isim < MMI_DUAL_SYS_MAX; isim++) 
    {
        MMISFR_NVSetICCID(isim, sim_iccid);
    }

    // 2. start next self reg ...
    if((g_sfr_linker.carrier = SFRM_GetFirstRegCarrier()) != NULL)
    {
        MMISFR_Http_Run();
    }
}

#define IS_LEAPYEAR(y)      ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0)

PUBLIC void MMISFR_ResetEvent(void)
{
    uint8 days_this_mon = 0;
    int32 sfr_event_id = 0;
    MMIALMSVC_EVENT_REG_INFO_T content_event_info = {0};
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};
    uint8 m_days [] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);

    //get event id from nv ...
    sfr_event_id = MMISFR_GetEventID();
    
    if (sfr_event_id > 0)
    {
        MMIALMSVC_UnRegService(sfr_event_id, TRUE);
    }

    SCI_MEMSET(&content_event_info, 0x00, sizeof(content_event_info));

    content_event_info.event_type = ALMSVC_EVENT_OTHER;
    content_event_info.event_time.year = sys_date.year;
    content_event_info.event_time.mon = sys_date.mon;
    content_event_info.event_time.mday = sys_date.mday;
    content_event_info.event_time.hour = sys_time.hour;
    content_event_info.event_time.min = sys_time.min;
    content_event_info.event_time.sec = 0;
    content_event_info.callback = (MMIALMSVC_HANDLE_CALLBACK)MMISFR_Event_Callback;

    SCI_TraceLow("[Self Reg] Current Time : %d-%d-%d %d:%d", content_event_info.event_time.year,content_event_info.event_time.mon,content_event_info.event_time.mday,content_event_info.event_time.hour,content_event_info.event_time.min);
    // add envent after 30 days
    days_this_mon = (IS_LEAPYEAR(sys_date.year) && (sys_date.mon == 2)) ? 29 : m_days[sys_date.mon - 1];
    content_event_info.event_time.mday += 30;
    if (content_event_info.event_time.mday > days_this_mon)
    {
        content_event_info.event_time.mday -= days_this_mon;
	 if (++content_event_info.event_time.mon > 12)
	 {
	     content_event_info.event_time.mon = 1;
	     ++content_event_info.event_time.year;
	 }
    }

    SCI_TraceLow("[Self Reg] Add a New Event %d-%d-%d %d:%d", content_event_info.event_time.year,content_event_info.event_time.mon,content_event_info.event_time.mday,content_event_info.event_time.hour,content_event_info.event_time.min);

    sfr_event_id = MMIALMSVC_RegService(&content_event_info, TRUE, TRUE);

    SCI_TraceLow("[Self Reg] Add a New Event event_id = %d", sfr_event_id);

    MMIALMSVC_SetAlmService();

    //write event id to nv ...
    MMISFR_SetEventID(sfr_event_id);
}

PUBLIC void MMISFR_CheckEvent(void)
{
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};
    SCI_ALARM_T cur_time = {0};
    int32 sfr_event_id = 0;

    sfr_event_id = MMISFR_GetEventID();

    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);

    cur_time.year = sys_date.year;
    cur_time.mon = sys_date.mon;
    cur_time.mday = sys_date.mday;
    cur_time.hour = sys_time.hour;
    cur_time.min = sys_time.min;
    cur_time.sec = 0;

    if (MMIALMSVC_IsValidTimedSvc(sfr_event_id, cur_time))
    {
        SCI_TraceLow("[Self Reg] Event Valid (cur time :  %d-%d-%d %d:%d)", 
            cur_time.year, cur_time.mon, cur_time.mday, cur_time.hour, cur_time.min);
        return;
    }

    MMISFR_Event_Callback(sfr_event_id, 0);
}

PUBLIC void SFR_LongTrace(const char *string, int len)
{
    if (len < 128)
    {
        SCI_TraceLow("[Self Reg] :%s ",string);
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
        
           SCI_TraceLow("[Self Reg] :%s ",buff);
        
          p += l;
          len -= l;
        }
    }
}