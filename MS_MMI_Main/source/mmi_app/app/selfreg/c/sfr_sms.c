#ifdef SFR_SUPPORT
#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "mmk_timer.h"
#include "mmiphone_export.h"
#include "mmi_nv.h"
#include "mmisms_send.h"
#include "version.h"

extern MMISMS_GLOBAL_T  g_mmisms_global;  

#define XmlBeginString		"<a1>"
#define XmlEndString			"</a1>"

#define DeviceBeginString		"<b1>"
#define ImeiBeginString1		"<b2>"
#define ImeiBeginString2		"<b3>"
#define ImsiBeginString		"<b4>"
#define VersionBeginString	"<b5>"
//#define DeviceMode 			"HMD-TA 1148"

#define SFR_SMS_BUF 136
#define SMS_START_RETRY_MAX 3
#define SFR_SMS_TIMER 60*1000

LOCAL BOOLEAN s_sfr_sms_done[MMI_DUAL_SYS_MAX]={FALSE,};
LOCAL uint8 s_sfr_sms_starter_retry[MMI_DUAL_SYS_MAX] = {0,};
LOCAL uint8 s_sfr_sms_starter[MMI_DUAL_SYS_MAX] = {0,};
LOCAL BOOLEAN s_sfr_sms_running[MMI_DUAL_SYS_MAX] = {FALSE,};
LOCAL BOOLEAN s_sfr_sms_cnf_ok[MMI_DUAL_SYS_MAX] = {FALSE,};

PUBLIC BOOLEAN MMISFR_IsImsiChanged(MN_DUAL_SYS_E dual_sys)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MN_IMSI_T nv_imsi[MMI_DUAL_SYS_MAX] ={0};
    MN_IMSI_T sim_imsi;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }
    MMINV_READ(MMINV_PHONE_IMSI,nv_imsi,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(nv_imsi,0,sizeof(nv_imsi));
        MMINV_WRITE(MMINV_PHONE_IMSI,nv_imsi);
    }

    sim_imsi = MNSIM_GetImsiEx(dual_sys);

    if (memcmp(&(nv_imsi[dual_sys]), &sim_imsi, sizeof(MN_IMSI_T)) == 0)
    {
        SCI_TRACE_LOW("[Self Reg SMS]: Telecom sim IMSI not changed.");
        return FALSE;
    }
    return TRUE;
}

LOCAL int MMISFR_GetIMEI(MN_DUAL_SYS_E dual_sys, char *buff, int buff_len)
{
    int i;
    char imei_str[18]={0};
    MN_IMEI_T tmp_imei;
#ifdef MMI_MULTI_SIM_SYS_SINGLE
    if (dual_sys == MN_DUAL_SYS_MAX)
    {
	sprintf(buff,
            "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",
            imei_str[1], imei_str[2], imei_str[3], imei_str[4],
            imei_str[5], imei_str[6], imei_str[7], imei_str[8],
            imei_str[9], imei_str[10], imei_str[11], imei_str[12],
            imei_str[13], imei_str[14], imei_str[15]);
	return strlen(buff);
    }
#endif
    if (MNNV_GetIMEIEx(dual_sys, tmp_imei))
    {
        for (i = 0; i < 8; i++) 
        {
            imei_str[2 * i] = (tmp_imei[i]) & 0x0F;
            imei_str[2 * i + 1] = tmp_imei[i] >> 4;
        }
    }
    sprintf(buff,
            "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",
            imei_str[1], imei_str[2], imei_str[3], imei_str[4],
            imei_str[5], imei_str[6], imei_str[7], imei_str[8],
            imei_str[9], imei_str[10], imei_str[11], imei_str[12],
            imei_str[13], imei_str[14], imei_str[15]);

    return strlen(buff);
}

LOCAL int MMISFR_GetIMSI(MN_DUAL_SYS_E dual_sys, char *buff, int buff_len)
{
    MN_IMSI_T tmp_imsi = MNSIM_GetImsiEx(dual_sys);
    
    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)tmp_imsi.imsi_val, MN_MAX_IMSI_ARR_LEN<<1, buff);

    return strlen(buff);
}


LOCAL int MMISFR_SmsBuildData(MN_DUAL_SYS_E dual_sys, uint8 * msg_ptr)
{
    char buff[128];
    int len;
    MN_DUAL_SYS_E dual_sim;
    int temp_len=0;
    char prochead[4]= {0};
    //uint32 crc;

    uint8 * message = PNULL;
    message = SCI_ALLOC_APPZ(SFR_SMS_BUF);
    if(message == NULL) return;
    SCI_MEMSET(message, 0, SFR_SMS_BUF);
   
    strcpy(message, XmlBeginString);

    //Device mod
    strcat(message, DeviceBeginString);
    strcat(message, SelfReg_DeviceMode);

    //IMEI1
    strcat(message, ImeiBeginString1);
    SCI_MEMSET(buff, 0, 128);
    MMISFR_GetIMEI(dual_sys, buff, len);
    strcat(message, buff);
    //IMEI2
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    dual_sim = dual_sys == MN_DUAL_SYS_1? MN_DUAL_SYS_2: MN_DUAL_SYS_1;
#else
    dual_sim = MN_DUAL_SYS_MAX;
#endif
    strcat(message, ImeiBeginString2);
    SCI_MEMSET(buff, 0, 128);
    MMISFR_GetIMEI(dual_sim, buff, len);
    strcat(message, buff);

    //IMSI
    strcat(message, ImsiBeginString);
    SCI_MEMSET(buff, 0, 128);
    MMISFR_GetIMSI(dual_sys, buff, len);
    strcat(message, buff+1);//+1 is skip the "9"

    //Software version
    strcat(message, VersionBeginString);
    strcat(message, SelfReg_DeviceVer);

    strcat(message, XmlEndString);

    temp_len = strlen(message);
    prochead[0]=0x03;//IMS SMS
    prochead[1]=0x03;//TYPE
    prochead[2]=temp_len;//LENTH
    prochead[3]=0x00;//FILL BIT

    SCI_TRACE_LOW("[Self Reg SMS]: MMISFR_SmsBuildData   temp_len=%d", temp_len);

    SCI_MEMCPY(msg_ptr, prochead, 4);
    SCI_MEMCPY(&msg_ptr[4], message, temp_len);

    //CDMA
    //crc= img_crc32(0, msg_ptr, 4 + strlen(message));
    //sprintf(msg_ptr + 4 + strlen(message),"%08x",crc);

    SCI_FREE(message);
    return temp_len+4;
}


LOCAL void MMISFR_SendSMS(MN_DUAL_SYS_E dual_sys)
{
    MMISMS_SEND_DATA_T send_data = {0};
    uint8 *server_id = "10659401";
    uint8 *msg_ptr = PNULL;
    int data_len=0;
    BOOLEAN result = FALSE;

    msg_ptr = SCI_ALLOC_APPZ(SFR_SMS_BUF+4);
    if(msg_ptr == NULL) return;
    SCI_MEMSET(msg_ptr, 0, SFR_SMS_BUF+4);
	
    data_len = MMISFR_SmsBuildData(dual_sys, msg_ptr);

    send_data.binary_sms_data.is_binary = TRUE;
    send_data.binary_sms_data.binary_length = data_len;
    send_data.binary_sms_data.binary_data = msg_ptr;
    send_data.dst_port = PNULL;
    send_data.src_port = PNULL;
    send_data.dest_addr_ptr = (uint8 *)server_id;
    send_data.dest_addr_len = strlen(server_id);
    send_data.dual_sys = dual_sys;
    send_data.is_need_packet = FALSE;
    send_data.send_callback = PNULL;
    send_data.is_sfr_sms = TRUE;

    SCI_TraceBuf("[Self Reg SMS] MMISFR_SendSMS msg data: ", send_data.binary_sms_data.binary_data,send_data.binary_sms_data.binary_length);

    result = MMIAPISMS_SendSmsForOther(&send_data);

    SCI_TRACE_LOW("[Self Reg SMS] MMIAPISMS_SendSmsForOther RESULT=%d",result);
    SCI_FREE(msg_ptr);
}


PUBLIC void MMISFR_NVSetIMSI(uint8 dual_sys)
{
    MN_IMSI_T imsi[MMI_DUAL_SYS_MAX] = {0};
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;	
    MN_IMSI_T sim_imsi;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }
    MMINV_READ(MMINV_PHONE_IMSI,imsi,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(imsi,0,sizeof(imsi));
        MMINV_WRITE(MMINV_PHONE_IMSI,imsi);
    }
    sim_imsi = MNSIM_GetImsiEx(dual_sys);
    SCI_MEMCPY(&(imsi[dual_sys]), &sim_imsi,sizeof(MN_IMSI_T));

    MMINV_WRITE(MMINV_PHONE_IMSI, imsi);
}

LOCAL void MMISFR_SelfRegisterBySMS(uint8 timer_id, uint32 param)
{
    MN_DUAL_SYS_E dual_sys = (MN_DUAL_SYS_E)param;

    SCI_TRACE_LOW("[Self Reg SMS] MMISFR_SelfRegisterBySMS dual_sys =%d----running=%d --CNF ok =%d", dual_sys, s_sfr_sms_running[dual_sys],s_sfr_sms_cnf_ok[dual_sys]);

    if( !s_sfr_sms_running[dual_sys])
    {
        SCI_TRACE_LOW("[Self Reg SMS] MMISFR_SelfRegisterBySMS Timer can NOT found");
        MMK_StopTimer(s_sfr_sms_starter[dual_sys]);
        return;
    }
    if(s_sfr_sms_cnf_ok[dual_sys])
    {
	    //write NV and stop timer
	    SCI_TRACE_LOW("[Self Reg SMS]MMISFR_SelfRegisterBySMS CNF OK");
	    MMISFR_NVSetIMSI(dual_sys);
	    MMK_StopTimer(s_sfr_sms_starter[dual_sys]);
	    s_sfr_sms_done[dual_sys] = TRUE;
	    s_sfr_sms_running[dual_sys] = FALSE;
    }
    else
    {
	    MMK_StopTimer(s_sfr_sms_starter[dual_sys]);
	    //Resend
	    SCI_TRACE_LOW("[Self Reg SMS]MMISFR_SelfRegisterBySMS resend retry = %d", s_sfr_sms_starter_retry[dual_sys]);
	    if(s_sfr_sms_starter_retry[dual_sys] ++ < SMS_START_RETRY_MAX)
	    {
	        MMISFR_SendSMS(dual_sys);
	        s_sfr_sms_starter[dual_sys] = MMK_CreateTimerCallback(SFR_SMS_TIMER, MMISFR_SelfRegisterBySMS, (uint32)dual_sys, FALSE);
	    }
	    else
	    {
	        s_sfr_sms_done[dual_sys] = TRUE;
	    }
    }
}

PUBLIC BOOLEAN MMISFR_SMSGetRunningStatus(MN_DUAL_SYS_E dual_sys)
{
    return s_sfr_sms_running[dual_sys];
}
PUBLIC void MMISFR_SMSSetCnfOk(MN_DUAL_SYS_E dual_sys, BOOLEAN isSuccess)
{
    s_sfr_sms_cnf_ok[dual_sys] = isSuccess;
}

PUBLIC void MMISFR_SMS_Run(MN_DUAL_SYS_E dual_sys)
{
    if(s_sfr_sms_done[dual_sys])
    {
        SCI_TraceLow("[Self Reg SMS] Already done.");
        return;
    }
    if(s_sfr_sms_running[dual_sys])
    {
        SCI_TraceLow("[Self Reg SMS] Running.");
        return;
    }

    MMK_StopTimer(s_sfr_sms_starter[dual_sys]);
    s_sfr_sms_running[dual_sys] = TRUE;
    SCI_TRACE_LOW("[Self Reg SMS] Self Register by SMS");

    //if(g_bappsms_smsAvailable)

    s_sfr_sms_starter_retry[dual_sys] = 0;
    s_sfr_sms_starter[dual_sys] = MMK_CreateTimerCallback(SFR_SMS_TIMER, MMISFR_SelfRegisterBySMS, (uint32)dual_sys, FALSE);
}
#endif

