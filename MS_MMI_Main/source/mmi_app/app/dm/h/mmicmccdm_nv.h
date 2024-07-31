#ifndef _MMISFR_NV_H_ 
#define _MMISFR_NV_H_

typedef enum
{
    MMINV_CMCCDM_EVENT_ID = MMI_MODULE_CMCCDM << 16,
    MMINV_SFR_ITEM_NUM
} MMISFR_NV_ITEM_E;

PUBLIC void MMICMCCDM_RegNv(void);
PUBLIC uint32 MMICMCCDM_GetEventID(void);
PUBLIC void MMICMCCDM_SetEventID(uint32 evid);

#endif
