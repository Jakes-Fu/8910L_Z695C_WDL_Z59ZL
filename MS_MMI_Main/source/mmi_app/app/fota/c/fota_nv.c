#ifdef FOTA_SUPPORT

#include "sci_types.h"
#include "mmk_app.h"
#include "mmi_module.h"
#include "mn_type.h"
#include "mn_api.h"
#include "http_api.h"
#include "fota_api.h"
#include "fota_header.h"
#include "fota_nv.h"

LOCAL uint16 mmifota_nv_len[] =
{
    sizeof(BOOLEAN), //MMINV_FOTA_ENABLE 
    sizeof(uint8),  //MMINV_FOTA_CHECK_PERIOD
    sizeof(uint32),  //MMINV_FOTA_LAST_TIME
    sizeof(uint32),  //MMINV_FOTA_VERSION
    sizeof(uint32),  //MMINV_FOTA_OPERATOR_IDX
    sizeof(MN_IMSI_T), //MMINV_FOTA_IMSI
    sizeof(uint32)  // MMINV_FOTA_MAGIC
};

PUBLIC void MMIFOTA_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_FOTA, mmifota_nv_len, sizeof(mmifota_nv_len) / sizeof(uint16));
}

PUBLIC uint8 MMIFOTA_NVGetCheckPeriod(void)
{
    // 0 - never check (disable)
    // 1 - check every day
    // 2 - check every week
    // 3 - check every month
    uint8 per = CHECK_PER_MONTH;
    BOOLEAN en = FALSE ;

    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_FOTA_ENABLE, &en))
    {
	  en = FALSE;
        MMI_WriteNVItem(MMINV_FOTA_ENABLE, &en);
    }
	
    if (!en) return 0;

    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_FOTA_CHECK_PERIOD, &per))
    {
        per =  CHECK_PER_MONTH;
        MMI_WriteNVItem(MMINV_FOTA_CHECK_PERIOD, &per);
    }

    if (per >= CHECK_PER_MAX)
    {
        per =  CHECK_PER_MONTH;
        MMI_WriteNVItem(MMINV_FOTA_CHECK_PERIOD, &per);
    }
	
    return per + 1;
}

PUBLIC void MMIFOTA_NVSetCheckPeriod(uint8 idx)
{
    // 0 - never check (disable)
    // 1 - check every day
    // 2 - check every week
    // 3 - check every month
    uint8 per;
    BOOLEAN en;

    if (idx == 0)
    {
        en = FALSE;
        per =  0;
    }
    else
    {
        en = TRUE;
        per =  idx <= CHECK_PER_MAX ? idx - 1 : 0;
    }

    MMI_WriteNVItem(MMINV_FOTA_ENABLE, &en);
    MMI_WriteNVItem(MMINV_FOTA_CHECK_PERIOD, &per);

    MMIFOTA_ReStart();
}

PUBLIC void MMIFOTA_NVSaveIMSI(void)
{
    MN_IMSI_T imsi = MNSIM_GetImsiEx(FOTA_UPDATE_SIM);
    MMI_WriteNVItem(MMINV_FOTA_IMSI, &imsi);
}

PUBLIC BOOLEAN MMIFOTA_NVIsIMSIChanged(void)
{
    MN_IMSI_T save_imsi;
    MN_IMSI_T sim_imsi;

    sim_imsi = MNSIM_GetImsiEx(FOTA_UPDATE_SIM);

    MMI_ReadNVItem(MMINV_FOTA_IMSI, &save_imsi);

    if (memcmp(&save_imsi, &sim_imsi, sizeof(MN_IMSI_T)) == 0)
        return FALSE;

    return TRUE;
}

PUBLIC BOOLEAN MMIFOTA_Is_Upate_Finish(void)
{
    uint32 magic;

    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_FOTA_MAGIC, &magic))
    {
        magic = 0;
        MMI_WriteNVItem(MMINV_FOTA_MAGIC, &magic);
        return TRUE;
    }

    return (magic == NV_FOTA_MAGIC) ? FALSE : TRUE;
}

PUBLIC void MMIFOTA_Set_Upate_Begin(void)
{
    uint32 magic;

    magic = NV_FOTA_MAGIC;

    MMI_WriteNVItem(MMINV_FOTA_MAGIC, &magic);
}

PUBLIC void MMIFOTA_Set_Upate_Finish(void)
{
    uint32 magic;

    magic = 0;

    MMI_WriteNVItem(MMINV_FOTA_MAGIC, &magic);
}

#endif

