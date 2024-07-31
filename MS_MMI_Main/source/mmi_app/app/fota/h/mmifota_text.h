
#ifndef FOTA_MMI_TEXT_H_
#define FOTA_MMI_TEXT_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "fota_mdu_def.h"
#endif

typedef enum FOTA_MMI_TEXT_ID_E
{
	FOTA_TXT_NULL = MMI_MODULE_FOTA << 16 | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "fota_mdu_def.h"
#endif
	FOTA_TXT_MAX
} FOTA_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // FOTA_MMI_TEXT_H_
