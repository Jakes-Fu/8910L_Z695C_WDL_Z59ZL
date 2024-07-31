#ifndef __SMART_DUAL_SIM_ID__
#define __SMART_DUAL_SIM_ID__

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "mmiwclk_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define WIN_ID_DEF(win_id)          win_id
typedef enum
{
    MMI_SMARTDUALSIM_START_WIN_ID = (MMI_MODULE_SMARTDUALSIM << 16),
    MMI_SMARTDUALSIM_FIST_TIME_ASK_WIN_ID,
    MMI_SMARTDUALSIM_SIMNUMBERS_EDIT_WIN_ID,
    MMI_SMARTDUALSIM_FORWARDTYPE_SELECT_WIN_ID,
    MMI_SMARTDUALSIM_FORWARD_SET_SUCCESS_WIN_ID,
    MMI_SMARTDUALSIM_FORWARD_SET_FAIL_WIN_ID,
    MMI_SMARTDUALSIM_MAIN_MENU_WIN_ID,
    MMI_SMARTDUALSIM_SIM_SETTINGS_WIN_ID,
    MMI_SMARTDUALSIM_SIM_ONE_WIN_ID,
    MMI_SMARTDUALSIM_DATA_CONNECTION_WIN_ID,
    MMI_SMARTDUALSIM_PREFERRED_SIM_WIN_ID,
    MMI_SMARTDUALSIM_CALL_PREFERRED_SIM_WIN_ID,
    MMI_SMARTDUALSIM_SMS_PREFERRED_SIM_WIN_ID,
    MMI_SMARTDUALSIM_NAME_EDIT_WIN_ID,
    MMI_SMARTDUALSIM_VODAFONE_CARRIER_WIN_ID,
    MMI_SMARTDUALSIM_MAX_WID_ID
}MMI_SMARTDUALSIM_WIN_ID_E;
#undef WIN_ID_DEF

typedef enum
{
    MMI_SMARTDUALSIM_START_CTRL_ID = MMI_SMARTDUALSIM_MAX_WID_ID,
    MMI_SMARTDUALSIM_SIM_NUMBER_FORM_CTRL_ID,
    MMI_SMARTDUALSIM_SIM1_LABEL_CTRL_ID,
    MMI_SMARTDUALSIM_SIM1_NUMBER_EDIT_CTRL_ID,
    MMI_SMARTDUALSIM_SIM2_NUMBER_EDIT_CTRL_ID,

    MMI_SMARTDUALSIM_FORWARD_TYPE_FORM_CTRL_ID,
    MMI_SMARTDUALSIM_FORWARD_TYPE_LABEL_CTRL_ID,
    MMI_SMARTDUALSIM_FORWARD_TYPE_MENU_CTRL_ID,
    MMI_SMARTDUALSIM_MAIN_MENU_CTRL_ID,
    MMI_SMARTDUALSIM_SIM_SETTINGS_CTRL_ID,
    MMI_SMARTDUALSIM_SIM_ONE_CTRL_ID,
    MMI_SMARTDUALSIM_DATA_CONNECTION_CTRL_ID,
    MMI_SMARTDUALSIM_PREFERRED_SIM_CTRL_ID,
    MMI_SMARTDUALSIM_CALL_PREFERRED_SIM_CTRL_ID,
    MMI_SMARTDUALSIM_SMS_PREFERRED_SIM_CTRL_ID,

    MMI_SMARTDUALSIM_SIM_NAME_FORM_CTRL_ID,
    MMI_SMARTDUALSIM_SIM_NAME_EDIT_CTRL_ID,

    MMI_SMARTDUALSIM_VODAFONE_CARRIER_LIST_CTRL_ID,

    MMI_SMARTDUALSIM_MAX_CTRL_ID
}MMI_SMARTDUALSIM_CTRL_ID_E;
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif //__SMART_DUAL_SIM_ID__
