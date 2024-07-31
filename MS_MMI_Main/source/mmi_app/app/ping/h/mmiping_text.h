#ifdef MMIENG_PING_SUPPORT
#include "sci_types.h"
#include "mmi_module.h"

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "ping_mdu_def.h"
#endif

typedef enum PING_MMI_TEXT_ID_E {
        PING_TXT_NULL = ( MMI_MODULE_PING << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "ping_mdu_def.h"
#endif
        EXAMPLE_TXT_MAX
} PING_MMI_TEXT_ID_E;
#endif
