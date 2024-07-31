#include "sci_types.h"
#include "mmk_app.h"
#include "mmi_module.h"
#include "mn_type.h"
#include "mn_api.h"
#include "mmi_signal_ext.h"
#include "mmicmccdm_header.h"
#include "mmicmccdm_nv.h"

LOCAL uint16 mmicmccdm_nv_len[] =
{
    sizeof(int32),    // MMINV_CMCCDM_EVENT_ID

};

//LOCAL SFR_ICCID s_sfr_iccid[MMI_DUAL_SYS_MAX] = {0,};

PUBLIC void MMICMCCDM_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_CMCCDM, mmicmccdm_nv_len, sizeof(mmicmccdm_nv_len) / sizeof(uint16));
}

PUBLIC uint32 MMICMCCDM_GetEventID(void)
{
    int32 evid;

    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_CMCCDM_EVENT_ID, &evid))
    {
        evid = 0; 
        MMI_WriteNVItem(MMINV_CMCCDM_EVENT_ID, &evid);
    }
    return evid;
}

PUBLIC void MMICMCCDM_SetEventID(uint32 evid)
{
    MMI_WriteNVItem(MMINV_CMCCDM_EVENT_ID, &evid);
}


