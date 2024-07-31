
#include "sci_types.h"
#include "mmi_modu_main.h"
#include "smartdualsim_nv.h"
#include "smartdualsim_internal.h"
#include "smartdualsim_export.h"

const uint16 smartdualsim_nv_len[] =
{
    sizeof(BOOLEAN), //MMI_SMARTDUALSIM_NV_FIST_TIME_PROMPTED
    sizeof(BOOLEAN), //MMI_SMARTDUALSIM_NV_ENABLED
    sizeof(SmartDS_Forwarding_Direction),//MMI_SMARTDUALSIM_NV_FORWARDING_DIRECTION
};


PUBLIC void MMISmartDS_RegModuleNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_SMARTDUALSIM, smartdualsim_nv_len, sizeof(smartdualsim_nv_len)/sizeof(uint16));
}

