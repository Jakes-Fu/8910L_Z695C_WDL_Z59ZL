#ifdef SFR_SUPPORT_CTCC
#undef ENABLE_HTTPC
#endif

#include "sci_types.h"
#include "mmk_app.h"
#include "mmi_module.h"
#include "mn_type.h"
#include "mn_api.h"
#ifdef ENABLE_HTTPC
#include "httpc_api.h"
#else 
#include "http_api.h"
#endif
#include "mmi_signal_ext.h"
#include "sfr_carrier_manager.h"
#include "sfr_header.h"
#include "sfr_nv.h"

LOCAL uint16 mmisfr_nv_len[] =
{
    MMI_DUAL_SYS_MAX *sizeof(SFR_ICCID), //MMINV_SFR_SIM0_ICCID 
    sizeof(int32),
};

LOCAL SFR_ICCID s_sfr_iccid[MMI_DUAL_SYS_MAX] = {0,};

PUBLIC void MMISFR_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_SFR, mmisfr_nv_len, sizeof(mmisfr_nv_len) / sizeof(uint16));
}

PUBLIC char * MMISFR_NVGetICCID(uint8 sim)
{
    if (sim >= MMI_DUAL_SYS_MAX)
    {
        return NULL;
    }

    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_SFR_SIM_ICCID, &s_sfr_iccid))
    {
        SCI_MEMSET(&s_sfr_iccid, 0, MMI_DUAL_SYS_MAX *sizeof(SFR_ICCID));
        MMI_WriteNVItem(MMINV_SFR_SIM_ICCID, &s_sfr_iccid);
    }
   return s_sfr_iccid[sim].num;
}

PUBLIC void MMISFR_NVSetICCID(uint8 sim, char *iccid)
{
    if (sim >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_SFR_SIM_ICCID, &s_sfr_iccid))
    {
        SCI_MEMSET(&s_sfr_iccid, 0, MMI_DUAL_SYS_MAX *sizeof(SFR_ICCID));
    }

    SCI_MEMSET(&s_sfr_iccid[sim], 0, sizeof(SFR_ICCID));
    SCI_MEMCPY(s_sfr_iccid[sim].num, iccid, SFR_ICCID_LENGTH);

    MMI_WriteNVItem(MMINV_SFR_SIM_ICCID, &s_sfr_iccid);
}

PUBLIC BOOLEAN MMISFR_IsICCIDChanged(MN_DUAL_SYS_E sim)
{
    char *p_nv_iccid;
    char nv_iccid[SFR_ICCID_LENGTH + 1] = {0};
    char sim_iccid[SFR_ICCID_LENGTH + 1] = {0};
    MNSIM_ICCID_T ccid;

    if (sim >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    p_nv_iccid = MMISFR_NVGetICCID(sim);
    if (p_nv_iccid == NULL)
    {
        return FALSE;
    }

    SCI_MEMCPY(nv_iccid, p_nv_iccid, SFR_ICCID_LENGTH);

    MNSIM_GetICCIDEx(sim, &ccid);
    MMIAPICOM_BcdToStr(0/*PACKED_LSB_FIRST*/, ccid.id_num, 20, sim_iccid);

    SCI_TraceLow("[Self Reg] Telecom sim ICCID sim (%d) : [%s]", sim, sim_iccid);
    SCI_TraceLow("[Self Reg] Telecom sim ICCID sim (%d) nv : [%s]", sim, nv_iccid);
    if (SCI_MEMCMP(sim_iccid, nv_iccid, SFR_ICCID_LENGTH) == 0)
    {
        SCI_TraceLow("[Self Reg] Telecom sim ICCID not changed.");
        return FALSE;
    }

    SCI_TraceLow("[Self Reg] Telecom sim ICCID changed sim (%d): [%s]", sim, sim_iccid);
    return TRUE;
}

PUBLIC BOOLEAN MMISFR_SaveICCID(MN_DUAL_SYS_E sim)
{
    char sim_iccid[SFR_ICCID_LENGTH + 1] = {0};
    MNSIM_ICCID_T ccid;

    if (sim >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    MNSIM_GetICCIDEx(sim, &ccid);
    MMIAPICOM_BcdToStr(0/*PACKED_LSB_FIRST*/, ccid.id_num, 20, sim_iccid);
    MMISFR_NVSetICCID(sim, sim_iccid);
    SCI_TraceLow("[Self Reg] Telecom sim ICCID Save [%s]", sim_iccid);
    if (MMISFR_IsICCIDChanged(sim))
    {
       SCI_TraceLow("[Self Reg] Telecom sim ICCID Save [OOOOROROROORORO]", sim_iccid);
    }
    return TRUE;
}

PUBLIC uint32 MMISFR_GetEventID(void)
{
    uint32 evid;

    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_SFR_EVENT_ID, &evid))
    {
        evid = 0; 
        MMI_WriteNVItem(MMINV_SFR_EVENT_ID, &evid);
    }
    return evid;
}

PUBLIC void MMISFR_SetEventID(uint32 evid)
{
    MMI_WriteNVItem(MMINV_SFR_EVENT_ID, &evid);
}

