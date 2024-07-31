#include "sci_types.h"
#include "mmk_app.h"
#include "mn_type.h"
#include "mmi_module.h"
#include "tts_nv.h"

LOCAL uint16 mmitts_nv_len[] =
{
    256, // MMINV_TTS_DATA_1 
    256, // MMINV_TTS_DATA_2
};

PUBLIC void MMITTS_RegNv(void)
{
	MMI_RegModuleNv(MMI_MODULE_TTS, mmitts_nv_len, sizeof(mmitts_nv_len) / sizeof(uint16));
}

PUBLIC void MMITTS_NVGetData(uint8 idx, char *buff)
{
    if (idx > 0 && idx < (MMINV_TTS_ITEM_NUM - MMINV_TTS_DATA_1))
    {
		if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_TTS_DATA_1 + idx, buff))
		{
			SCI_MEMSET(buff, 0, 256); 
			MMI_WriteNVItem(MMINV_TTS_DATA_1 + idx, buff);
		}
    }
}

PUBLIC void MMITTS_NVSetData(uint8 idx, char *buff)
{

    if (idx > 0 && idx < (MMINV_TTS_ITEM_NUM - MMINV_TTS_DATA_1))
    {
		MMI_WriteNVItem(MMINV_TTS_DATA_1 + idx, buff);
    }
}

