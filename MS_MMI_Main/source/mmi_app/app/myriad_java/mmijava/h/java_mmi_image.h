/*
 * Copyright 2011 Myriad Group AG. All Rights Reserved.
 * $Id: //depot/dev/project/spreadtrum/6530_new/export/inc/java_mmi_image.h#1 $
 */

#ifndef JAVA_MMI_IMAGE_H_
#define JAVA_MMI_IMAGE_H_

#include "sci_types.h"
#include "mmi_imagefun.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Image Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "java_mdu_def.h"
#endif

typedef enum SET_MMI_IMAGE_ID_E
{
   SET_IMAGE_NULL = (MMI_MODULE_JAVA << 16) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "java_mdu_def.h"
#endif
   SET_IMAGE_MAX_ID
} SET_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //JAVA_MMI_IMAGE_H_
