#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "mmiphone_export.h"

#include "mmi_filemgr.h"
#include "version.h"
#include "mmipdp_export.h"
#include "in_message.h"
#include "mmieng_internal.h"
#include "mmisms_app.h"
#include "mmi_module.h"
#include "dal_time.h"
#include "mmk_authen.h"
#include "mmisms_set.h"
#include "mmi_nv.h"

static char s_sfr_com_buff[128];
static char s_sfr_meid_buff[16] = {0};
extern uint8 g_sfr_main_sim_sys;
extern uint8 g_sfr_sub_sim_sys;

const char *sfr_dm_get_imei1(void);
const char *sfr_dm_get_imei2(void);

//电信自注册只要求上传14位
const char *sfr_dm_get_meid14(void)
{
#ifdef MEID_WRITE_SUPPORT
    MMIAPICOM_MEID_GetStr(s_sfr_meid_buff);
    s_sfr_meid_buff[14] = 0;
    return s_sfr_meid_buff;
#else
    sfr_dm_get_imei1();
    s_sfr_com_buff[14] = 0;
    return s_sfr_com_buff;
#endif
}

const char *sfr_dm_get_meid(void)
{
    return sfr_dm_get_imei1();
}

const char *sfr_dm_get_model(void)
{
    return SelfReg_DeviceMode;
}

const char *sfr_dm_get_sw_version(void)
{
    return SelfReg_DeviceVer;
}

const char *sfr_dm_get_sim1_cdma_imsi(void)
{
    return "";//"460036180704398";
}

const char *sfr_dm_get_ue_type(void)
{
    return "1";
}

const char *sfr_dm_get_sim1_iccid(void)
{
    if (MMIAPIPHONE_GetSimExistedStatus(g_sfr_main_sim_sys))
    {
        char sim_iccid[2*MNSIM_ICCID_ID_NUM_LEN + 1];
        MNSIM_ICCID_T ccid;

        MNSIM_GetICCIDEx(g_sfr_main_sim_sys, &ccid);
        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, ccid.id_num, 20, s_sfr_com_buff);

        return s_sfr_com_buff;
    }
    return "";
}

const char *sfr_dm_get_sim1_lte_imsi(void)
{
    if (MMIAPIPHONE_GetSimExistedStatus(g_sfr_main_sim_sys))
    {
        MN_IMSI_T tmp_imsi = MNSIM_GetImsiEx(g_sfr_main_sim_sys);
    
        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)tmp_imsi.imsi_val, MN_MAX_IMSI_ARR_LEN<<1, s_sfr_com_buff);

        return s_sfr_com_buff + 1; // +1 is skip the "9"
    }
    return "";
}

const char *sfr_dm_get_sim2_cdma_imsi(void)
{
    return "";
}

const char *sfr_dm_get_sim2_iccid(void)
{
    if (MMIAPIPHONE_GetSimExistedStatus(g_sfr_sub_sim_sys))
    {
        char sim_iccid[2*MNSIM_ICCID_ID_NUM_LEN + 1];
        MNSIM_ICCID_T ccid;

        MNSIM_GetICCIDEx(g_sfr_sub_sim_sys, &ccid);
        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, ccid.id_num, 20, s_sfr_com_buff);

        return s_sfr_com_buff;
    }
    return "";
}

const char *sfr_dm_get_sim2_lte_imsi(void)
{
    if (MMIAPIPHONE_GetSimExistedStatus(g_sfr_sub_sim_sys))
    {
        MN_IMSI_T tmp_imsi = MNSIM_GetImsiEx(g_sfr_sub_sim_sys);
    
        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)tmp_imsi.imsi_val, MN_MAX_IMSI_ARR_LEN<<1, s_sfr_com_buff);

        return s_sfr_com_buff + 1; // +1 is skip the "9"
    }
    return "";
}

const char *sfr_dm_get_mac_id(void)
{
#if 0
    char buf[8];

    //return "24:e2:71:f4:d7:b0";
    if (TCPIPNETIF_GetMac(buf, 8, TCPIPNETIF_GetNetifWithNetid(g_sfr_linker.net_id)))
    {
        sprintf(s_sfr_com_buff, "%02x:%02x:%02x:%02x:%02x:%02x",
                buf[0], buf[1], buf[2], buf[3], buf[4],buf[5]);
    
        return s_sfr_com_buff;
    }
#endif
    return "";
}

const char *sfr_dm_get_os_version(void)
{
    return ZDT_PlatVer;
}

const char *sfr_dm_get_imei1(void)
{
    int i;
    char imei_str[18]={0};
    MN_IMEI_T tmp_imei;

    if (MNNV_GetIMEIEx(g_sfr_main_sim_sys, tmp_imei))
    {
        for (i = 0; i < 8; i++) 
        {
            imei_str[2 * i] = (tmp_imei[i]) & 0x0F;
            imei_str[2 * i + 1] = tmp_imei[i] >> 4;
        }
    }
    sprintf(s_sfr_com_buff,
            "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",
            imei_str[1], imei_str[2], imei_str[3], imei_str[4],
            imei_str[5], imei_str[6], imei_str[7], imei_str[8],
            imei_str[9], imei_str[10], imei_str[11], imei_str[12],
            imei_str[13], imei_str[14], imei_str[15]);

    return s_sfr_com_buff;
}

const char *sfr_dm_get_imei2(void)
{
#if defined MULTI_SIM_SYS_SINGLE 
    return "";
#else
    int i;
    char imei_str[18]={0};
    MN_IMEI_T tmp_imei;

    if (MNNV_GetIMEIEx(g_sfr_sub_sim_sys, tmp_imei))
    {
        for (i = 0; i < 8; i++) 
        {
            imei_str[2 * i] = (tmp_imei[i]) & 0x0F;
            imei_str[2 * i + 1] = tmp_imei[i] >> 4;
        }
    }
    sprintf(s_sfr_com_buff,
            "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",
            imei_str[1], imei_str[2], imei_str[3], imei_str[4],
            imei_str[5], imei_str[6], imei_str[7], imei_str[8],
            imei_str[9], imei_str[10], imei_str[11], imei_str[12],
            imei_str[13], imei_str[14], imei_str[15]);

    return s_sfr_com_buff;
#endif
}

const char *sfr_dm_get_sim1_cell_id(void)
{
    //return "123456789";
    if (MMIAPIPHONE_GetSimExistedStatus(g_sfr_main_sim_sys))
    {
        MSG_CAMP_STATUS_T plmn_info;
        MNPHONE_GetPlmnInfoEx(g_sfr_main_sim_sys, &plmn_info);
        sprintf(s_sfr_com_buff, "%d", plmn_info.cell_id);
        return s_sfr_com_buff;
    }
    return "";
}

const char *sfr_dm_get_sim2_cell_id(void)
{
    if (MMIAPIPHONE_GetSimExistedStatus(g_sfr_sub_sim_sys))
    {
        MSG_CAMP_STATUS_T plmn_info;
        MNPHONE_GetPlmnInfoEx(g_sfr_sub_sim_sys, &plmn_info);
        sprintf(s_sfr_com_buff, "%d", plmn_info.cell_id);
        return s_sfr_com_buff;
    }
    return "";
}

const char *sfr_dm_get_sim1_lac(void)
{
    if (MMIAPIPHONE_GetSimExistedStatus(g_sfr_main_sim_sys))
    {
        MSG_CAMP_STATUS_T plmn_info;
        MNPHONE_GetPlmnInfoEx(g_sfr_main_sim_sys, &plmn_info);
        sprintf(s_sfr_com_buff, "%d", plmn_info.lac);
        return s_sfr_com_buff;
    }
    return "";
}

const char *sfr_dm_get_sim2_lac(void)
{
    if (MMIAPIPHONE_GetSimExistedStatus(g_sfr_sub_sim_sys))
    {
        MSG_CAMP_STATUS_T plmn_info;
        MNPHONE_GetPlmnInfoEx(g_sfr_sub_sim_sys, &plmn_info);
        sprintf(s_sfr_com_buff, "%d", plmn_info.lac);
        return s_sfr_com_buff;
    }
    return "";
}

const char *sfr_dm_get_data_sim(void)
{
    MN_DUAL_SYS_E _dusim = g_sfr_main_sim_sys;

    MMIAPIPHONE_GetDataServiceSIM(&_dusim);

    return (_dusim == g_sfr_main_sim_sys ? "1" : "2");
}

const char *sfr_dm_get_rom_size(void)
{
    return ZDT_PlatROM;
}

const char *sfr_dm_get_ram_size(void)
{
    return ZDT_PlatRAM;
}

const char *sfr_dm_get_sim1_volte_sw(void)
{
    if (MMIAPIPHONE_GetSimExistedStatus(g_sfr_main_sim_sys))
    {
        if (!MNPHONE_ImsIsSupport(g_sfr_main_sim_sys))
            return "3"; //3:not support
        if (!MNPHONE_ImsIsSet(g_sfr_main_sim_sys))
            return "2";  // 2:off 
        return "1";  // 1: on
    }
    return NULL; // NULL, no this item
}

const char *sfr_dm_get_sim2_volte_sw(void)
{
    if (MMIAPIPHONE_GetSimExistedStatus(g_sfr_sub_sim_sys))
    {
        if (!MNPHONE_ImsIsSupport(g_sfr_sub_sim_sys))
            return "3"; //3:not support
        if (!MNPHONE_ImsIsSet(g_sfr_sub_sim_sys))
            return "2";  // 2:off 
        return "1";  // 1: on
    }
    return NULL; // NULL, no this item
}

const char *sfr_dm_get_accsee_type(void)
{
    return "1"; //1:GPRS 2:Wifi 
}

const char *sfr_dm_get_time_total_mseconds(void)
{
    uint32 sec = 315504000; // secs form 1970-1-1 8:00:00 to 1980-1-1 00:00:00

    // sec for 1980-01-01 
    sec += MMIAPICOM_GetCurTime();

    sprintf(s_sfr_com_buff, "%d000", sec);

    return s_sfr_com_buff;
}
