#include <stdio.h>

#include "std_header.h"
#include "os_api.h"
#include "mmiphone_export.h"
#include "mmi_filemgr.h"
#include "version.h"
#include "mmipdp_export.h"
#include "mmieng_internal.h"
#include "mmi_module.h"
#include "mmk_authen.h"
#include "mmi_nv.h"
#include "mmipb_export.h"
#include "mmk_timer.h"
#ifdef LWM2M_SUPPORT
#include "lwm2msdk.h"
#endif

#include "mmiutil.h"
#include "mmicmccdm_main.h"
#include "mmicc_export.h"
#include "mmipb_common.h"
#include "mmiconnection_export.h"

#if defined LWM2M_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define RETRY_TIME_INTERVAL 60000 //milliseconds
#define MAX_RETRY_TIMES  3

/**--------------------------------------------------------------------------*
**                                                  *
**--------------------------------------------------------------------------*/

extern uint8 s_retry_times;




PUBLIC Options MMICMCCDM_DMCoapRegDate(void)
{
    MN_IMEISVN_T imeisvn = {0};
    MN_IMSI_T imsi = MNSIM_GetImsiEx(0);
  //  Options opt;
    char imsi_val[(MN_MAX_IMSI_ARR_LEN << 1) + 1] = { 0 };
    char ori_imei[(MN_MAX_IMEI_LENGTH << 1) + 1] = {0};

        Options opt =
    {
         "",         //CMEI/IMEI
         "",       //CMEI/IMEI2
         "",                 //IMSI
         "v2.0",           //DM version,v2.0
         "M100000698",     //appkey
         "6w10Hl1B3338718u97879GP5514kP15t", //pwd password
         4,                // 4: ipv4-fixed; 6:ipv6;
         "shipei.fxltsbl.com", //host or ip: shipei.fxltsbl.com(test)/m.fxltsbl.com(release)
        5683,             //server port¸ 5683 generally
        300,              //update interval 300s(test),86400s(rel)
        56831,            //local port
        0,                 //booststrap 0,not use; other n,use; set current value to 0
        0
      };   

    if (MNNV_GetIMEISVNEx(0, &imeisvn))
    {
        
        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)imeisvn.imei,
                           MN_MAX_IMEI_LENGTH << 1, ori_imei);
        SCI_TRACE_LOW("MMISFR_GenerateData ori_imei len=%d", strlen(ori_imei));
        SCI_MEMCPY(opt.szCMEI_IMEI, &ori_imei[1], strlen(ori_imei) - 1);//first is '*'
    }

    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, imsi.imsi_val, MN_MAX_IMSI_ARR_LEN << 1,
                       imsi_val);
    SCI_TRACE_LOW("[CMCC DM] MMICMCCDM_GenerateData imsi_val len=%d", strlen(imsi_val));
    SCI_MEMCPY(opt.szIMSI, &imsi_val[1],   strlen(imsi_val) - 1);//first is '9'

    return opt;
}


 LOCAL uint8 *MMICMCCDM_GetNetAccount(void)
{
    MMICONNECTION_LINKSETTING_DETAIL_T *linksetting = PNULL;
    static uint8 NetAccount[MMICONNECTION_MAX_APN_LEN + 1] = {0};
    uint8 linkNum = 0;
    int i;
    SCI_TRACE_LOW("[CMCC DM] MMICMCCDM_GetNetAccount()");
    
    linkNum = MMIAPICONNECTION_GetLinkSettingNum(0);
    for (i = 0; i < linkNum; i++)
    {
        linksetting = PNULL;
        linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(0, i);

        // print apn information and plmn
        if (PNULL != linksetting && 0 != linksetting->apn_len)
        {
            SCI_MEMCPY(NetAccount, linksetting->apn, strlen(linksetting->apn));
            break;
        }
    }
    return NetAccount;
}

LOCAL uint8 *MMICMCCDM_GetNetworkType(void)
{
    MN_GMMREG_RAT_E gmmreg_rat;
    static uint8 nettype[3] = {0};
    gmmreg_rat = MNPHONE_GetCurrentRAT(0);
    switch (gmmreg_rat)
    {
        case MN_GMMREG_RAT_4G:
            SCI_MEMCPY(nettype, "4G", 3);
            break;
        case MN_GMMREG_RAT_3G:
        //  case MN_GMMREG_RAT_TD:
        case MN_GMMREG_RAT_GPRS_AND_3G:
        case MN_GMMREG_RAT_VDS_GSM:
            SCI_MEMCPY(nettype, "3G", 3);
            break;
        case MN_GMMREG_RAT_GPRS:
            SCI_MEMCPY(nettype, "2G", 3);
            break;
        default:
            SCI_MEMCPY(nettype, "2G", 3);
            break;
    }
    return nettype;

}


LOCAL BOOLEAN IsVolteOn(MN_DUAL_SYS_E sim)
{
    return ((MMIAPISET_GetVolteInfo()) & (1 << sim));
}

LOCAL int MMIAPIPHONE_GetVolteStatus(MN_DUAL_SYS_E sim)
{
    if (!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
    {
       if (!IsVolteOn(MN_DUAL_SYS_1))
           return "0";  // 0:off 
       return "1";  // 1: on
    }
    return ""; // no sim
}

PUBLIC int MMICMCCDM_UEReportInfo(int resId, char **outbuff)
{
    BOOLEAN  is_volte_state;
    char buff[512];
    int  buflen;
    int iret = 0;
    *outbuff = NULL;
    buflen = sizeof(buff);
    memset(buff, 0, sizeof(buff));

    snprintf(buff, buflen, "%s", "unknown"); //default
    SCI_TRACE_LOW("[CMCC DM lwm2m]MMICMCCDM_DMReadInfo:resId=%d",
                  resId);
    switch (resId)
    {
        case 6601://devinfo
            snprintf(buff, buflen, "%s", "");
            break;
        case 6602://appinfo
            snprintf(buff, buflen,
                     "app_name1|pak_name1|1234000|3\r\napp_name2|pak_name2|2234000|5\r\n");
            break;
        case 6603://mac
            snprintf(buff, buflen, "");
            break;
        case 6604://rom
            snprintf(buff, buflen, "16M");
            break;
        case 6605://ram
            snprintf(buff, buflen, "24M");
            break;
        case 6606://CPU
            snprintf(buff, buflen, "ARM AA");
            break;
        case 6607://SYS VERSION
            snprintf(buff, buflen, "MOCOR20B");
            break;
        case 6608://FIRMWARE VERSION
            snprintf(buff, buflen, "UIS8910FF");
            break;
        case 6609://FIRMWARE NAME
            snprintf(buff, buflen, "UIS8910FF");
            break;
        case 6610://Volte
            is_volte_state =  MMIAPIPHONE_GetVolteStatus(0);
            if (is_volte_state == TRUE)
            {
                snprintf(buff, buflen, "0");
            }
            else
            {
                snprintf(buff, buflen, "1");
            } 
            break;
        case 6611://NetType
            snprintf(buff, buflen, MMICMCCDM_GetNetworkType());
            break;
        case 6612://Net Account
            snprintf(buff, buflen, MMICMCCDM_GetNetAccount());
            break;
        case 6613://PhoneNumber
            snprintf(buff, buflen, "");
            break;
        case 6614://Location
            snprintf(buff, buflen, "");
            break;

        default:
            iret = -1;
            break;
    }

    if (iret == 0)
    {
        //apply for buffer, sdk will free buffer
        *outbuff = (char *)malloc(strlen(buff) + 1);
        strcpy(*outbuff, buff);
    }

    SCI_TRACE_LOW("[CMCC DM lwm2m]MMISFR_DMReadInfo,ID=%d,value=%s,iret=%d\n", resId, buff,
                  iret);
    return iret;
}

PUBLIC void MMICMCCDM_PostUEReportDate(void)
{
   int ret_stop;
   int ret_run;
   SCI_TRACE_LOW("[CMCC DM lwm2m]MMISFR_PostLwM2MData s_retry_times = %d",
                  s_retry_times);
   ret_stop = LWM2M_SDK_STOP();
   ret_run = LWM2M_SDK_RUN(1);
}

#endif

