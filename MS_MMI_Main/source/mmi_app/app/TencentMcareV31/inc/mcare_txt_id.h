#ifndef MCARE_TEXT_H_
#define MCARE_TEXT_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Text Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE   MACRO_MDU_TEXT
#include "macro_mdu_def.h"

typedef enum MCARE_TEXT_ID_E
{
	MCARE_TXT_NULL = MMI_MODULE_MCARE_MINI<< 16 | MMI_SYS_RES_FLAG,
#include "tencentmcarev31_mdu_def.h"	
	MCARE_TXT_MAX
} MCARE_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // MCARE_MMI_TEXT_H_
