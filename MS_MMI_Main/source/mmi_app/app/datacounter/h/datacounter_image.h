#ifndef __DATACOUNTER_IMAGE_H__
#define __DATACOUNTER_IMAGE_H__

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern "C"{
#endif

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "datacounter_mdu_def.h"
#endif

typedef enum 
{
	MMI_DATACOUNTER_START_IMAGE_ID = (MMI_MODULE_DATA_COUNTER << 16) | MMI_SYS_RES_FLAG,
	#include "datacounter_mdu_def.h"
	DATACOUNTER_IMAGE_MAX_ID
}MMI_SMARTDUALSIM_IMAGE_ID_E;

#undef MACRO_MDU_TYPE


#ifdef __cplusplus
}
#endif
#endif //__DATACOUNTER_IMAGE_H__


