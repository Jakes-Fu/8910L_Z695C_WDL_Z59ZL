#ifndef __SMARTDUALSIM_NV__
#define __SMARTDUALSIM_NV__

#include "guimenu.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_module.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

typedef enum
{
    MMI_SMARTDUALSIM_NV_FIST_TIME_PROMPTED = (MMI_MODULE_SMARTDUALSIM << 16),
    MMI_SMARTDUALSIM_NV_ENABLED,
    MMI_SMARTDUALSIM_NV_FORWARDING_DIRECTION,
    MMI_SMARTDUALSIM_NV_MAX_ITEM_NUM
}MMI_SMARTDUALSIM_NV_ITEM_E;


PUBLIC void MMISmartDS_RegModuleNv(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //__SMARTDUALSIM_NV__

