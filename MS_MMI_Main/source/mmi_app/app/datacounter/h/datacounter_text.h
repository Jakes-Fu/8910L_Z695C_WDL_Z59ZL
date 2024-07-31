#ifndef __DATACOUNTER_TEXT_H__
#define __DATACOUNTER_TEXT_H__

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern "C"{
#endif

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

typedef enum 
{
	MMI_DATACOUNTER_START_TEXT_ID = (MMI_MODULE_DATA_COUNTER << 16) | MMI_SYS_RES_FLAG,
	#include "datacounter_mdu_def.h"
}MMI_DATACOUNTER_TEXT_ID_E;

#undef MACRO_MDU_TYPE


#ifdef __cplusplus
}
#endif
#endif //__DATACOUNTER_TEXT_H__

