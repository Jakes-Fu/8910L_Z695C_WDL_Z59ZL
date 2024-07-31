#ifndef __SMARTDUALSIM_IMAGE_H__
#define __SMARTDUALSIM_IMAGE_H__

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern "C"{
#endif

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

typedef enum 
{
    MMI_SMARTDUALSIM_START_IMAGE_ID = (MMI_MODULE_SMARTDUALSIM << 16) | MMI_SYS_RES_FLAG,
    #include "smartdualsim_mdu_def.h"
}MMI_SMARTDUALSIM_IMAGE_ID_E;

#undef MACRO_MDU_TYPE


#ifdef __cplusplus
}
#endif
#endif //__SMARTDUALSIM_IMAGE_H__


