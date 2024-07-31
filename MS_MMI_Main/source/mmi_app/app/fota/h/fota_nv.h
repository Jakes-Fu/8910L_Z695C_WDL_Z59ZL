
#ifndef _MMIFOTA_NV_H_ 
#define _MMIFOTA_NV_H_

#ifdef FOTA_SUPPORT

typedef enum
{
    MMINV_FOTA_ENABLE = MMI_MODULE_FOTA << 16,
    MMINV_FOTA_CHECK_PERIOD,
    MMINV_FOTA_LAST_TIME,
    MMINV_FOTA_VERSION,
    MMINV_FOTA_OPERATOR_IDX,
    MMINV_FOTA_IMSI,
    MMINV_FOTA_MAGIC,
    MMINV_FOTA_ITEM_NUM
}MMIFOTA_NV_ITEM_E;

typedef enum
{
    CHECK_PER_DAY,
    CHECK_PER_WEEK,
    CHECK_PER_MONTH,
    CHECK_PER_MAX
} MMI_FOTA_CHK_PERIOD_E;

#define NV_FOTA_MAGIC  0xf07af07a

PUBLIC void MMIFOTA_RegNv(void);

PUBLIC uint8 MMIFOTA_NVGetCheckPeriod(void);
PUBLIC void MMIFOTA_NVSetCheckPeriod(uint8 idx);

PUBLIC BOOLEAN MMIFOTA_Is_Upate_Finish(void);
PUBLIC void MMIFOTA_Set_Upate_Begin(void);
PUBLIC void MMIFOTA_Set_Upate_Finish(void);

#endif
#endif

