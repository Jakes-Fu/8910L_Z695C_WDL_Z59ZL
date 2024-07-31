#ifndef _SMART_DUAL_SIM_FIRST_TIME_FLOW_
#define _SMART_DUAL_SIM_FIRST_TIME_FLOW_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmk_msg.h"
#include "mmidisplay_data.h"
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define MSG_SMARTDS_FIRST_FLOW_CHECK     (MSG_USER_START + (MMI_MODULE_SMARTDUALSIM << 16))
#define MSG_SMARTDS_FIRST_FLOW_OK        (MSG_USER_START + (MMI_MODULE_SMARTDUALSIM << 16) + 1)
#define MSG_SMARTDS_FIRST_FLOW_PROMPT    (MSG_USER_START + (MMI_MODULE_SMARTDUALSIM << 16) + 2)

typedef enum
{
    SMARTDS_FORWARDING_NONE,
    SMARTDS_FORWARDING_1_TO_2,
    SMARTDS_FORWARDING_2_TO_1,
    SMARTDS_FORWARDING_BOTH_SIM,
    SMARTDS_FORWARDING_CLOSE_1_TO_2,
    SMARTDS_FORWARDING_CLOSE_2_TO_1,
    SMARTDS_FORWARDING_CLOSE_BOTH_SIM
}SmartDS_Forwarding_Direction;

PUBLIC void MMISmartDS_ModuleInit(void);
PUBLIC void MMISmartDS_ForwardOkDialog(SmartDS_Forwarding_Direction direction);
PUBLIC void MMISmartDS_ForwardFailedDialog(SmartDS_Forwarding_Direction direction, MMI_TEXT_ID_T promptStrID);
PUBLIC MMI_HANDLE_T MMISmartDS_MainMenuWin(void);
PUBLIC BOOLEAN MMISmartDS_CheckToDisplayPromptWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id);
PUBLIC void MMISmartDS_CloseNetworkWaittingWin(void);
PUBLIC MMI_TEXT_ID_T MMISmartDS_getSimPLMNSelectModeStrId(MN_PHONE_PLMN_SELECT_MODE_E selectMode);
PUBLIC SmartDS_Forwarding_Direction MMISmartDS_getDestForwardType(void);
PUBLIC MMI_HANDLE_T MMISmartDS_PreferredSimWin(BOOLEAN isByBoot);
PUBLIC BOOLEAN MMISmartDS_IsNetworkWaittingWinOpen(void);
 PUBLIC BOOLEAN MMISmartDS_IsPlmnListWinOpen(void);

#ifdef __cplusplus
}
#endif
#endif //_SMART_DUAL_SIM_FIRST_TIME_FLOW_

