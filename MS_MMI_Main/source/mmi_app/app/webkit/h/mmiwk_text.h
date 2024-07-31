#ifndef MMIWK_TEXT_H_
#define MMIWK_TEXT_H_
#if defined(BROWSER_SUPPORT_WEBKIT)
#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

typedef enum WEBKIT_MMI_TEXT_ID_E
{
    MMIWK_TXT_NULL = ( MMI_MODULE_WK << 16 ) | MMI_SYS_RES_FLAG,
    #include "webkit_mdu_def.h"
    MMIWK_TXT_MAX
}
WEBKIT_MMIWK_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif
#endif
#endif // MMIWK_TEXT_H_
