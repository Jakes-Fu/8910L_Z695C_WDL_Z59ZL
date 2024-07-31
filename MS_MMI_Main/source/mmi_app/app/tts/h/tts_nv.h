
#ifndef _MMITTS_NV_H_ 
#define _MMITTS_NV_H_

typedef enum
{
    MMINV_TTS_DATA_1 = MMI_MODULE_TTS << 16,
    MMINV_TTS_DATA_2,
    MMINV_TTS_ITEM_NUM
} MMITTS_NV_ITEM_E;

PUBLIC void MMITTS_RegNv(void);
PUBLIC void MMITTS_NVGetData(uint8 idx, char *buff);
PUBLIC void MMITTS_NVSetData(uint8 idx, char *buff);


#endif // _MMITTS_NV_H_

