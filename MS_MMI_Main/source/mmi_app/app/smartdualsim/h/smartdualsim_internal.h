#ifndef __SMART_DUAL_SIM_INTERNAL__
#define __SMART_DUAL_SIM_INTERNAL__

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

#define SMARTDS_PB_BCD_NUMBER_MAX_LEN (MMIPB_BCD_NUMBER_MAX_LEN*2+2)

typedef enum
{
    smartDS_Forward_follow_poweron,
    smartDS_Forward_follow_menuset,
    smartDS_Forward_follow_max,
}smartDS_Forward_follow_type;

typedef struct
{
    uint16      sim_number1_len;
    uint16      sim_number2_len;
    wchar       sim_number1[SMARTDS_PB_BCD_NUMBER_MAX_LEN];
    wchar       sim_number2[SMARTDS_PB_BCD_NUMBER_MAX_LEN];
}dual_sim_numbers;

extern dual_sim_numbers simNumbers;

PUBLIC MMI_HANDLE_T MMISmartDS_ForwardTypeSelectWin(MMI_WIN_ID_T parentWinId);
PUBLIC MMI_HANDLE_T MMISmartDS_SimNumbersWin(MMI_WIN_ID_T parentWinId);
PUBLIC MMI_HANDLE_T MMISmartDS_SimSettingsWin(void);
PUBLIC MMI_HANDLE_T MMISmartDS_DataConnectionWin(void);
PUBLIC MMI_HANDLE_T MMISmartDS_CallPreferWin(void);
PUBLIC MMI_HANDLE_T MMISmartDS_SMSPerferWin(void);
PUBLIC void getSIMNumber(MN_DUAL_SYS_E dual_sys);
PUBLIC smartDS_Forward_follow_type MMISmartDS_getForwardFollowType(void);
PUBLIC void MMISmartDS_setForwardFollowType(smartDS_Forward_follow_type followType);
PUBLIC void MMISmartDS_displayBanner(MMI_WIN_ID_T win_id, MMI_TEXT_ID_T infoID);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
