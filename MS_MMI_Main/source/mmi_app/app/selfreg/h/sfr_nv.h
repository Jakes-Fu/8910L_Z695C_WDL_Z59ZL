
#ifndef _MMISFR_NV_H_ 
#define _MMISFR_NV_H_

typedef enum
{
    MMINV_SFR_SIM_ICCID = MMI_MODULE_SFR << 16,
    MMINV_SFR_EVENT_ID,
    MMINV_SFR_ITEM_NUM
} MMISFR_NV_ITEM_E;

PUBLIC void MMISFR_RegNv(void);
PUBLIC char * MMISFR_NVGetICCID(uint8 sim);
PUBLIC void MMISFR_NVSetICCID(uint8 sim, char *iccid);
PUBLIC BOOLEAN MMISFR_IsICCIDChanged(MN_DUAL_SYS_E sim);
PUBLIC uint32 MMISFR_GetEventID(void);
PUBLIC void MMISFR_SetEventID(uint32 evid);

#endif

