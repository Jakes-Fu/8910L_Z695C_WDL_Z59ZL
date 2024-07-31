/*****************************************************************************
** File Name:      mmicp_image.h                                             *
** Author:                                                                   *
** Date:           2015/06/08                                                *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
*****************************************************************************/

#ifndef CP_MMI_IMAGE_H_
#define CP_MMI_IMAGE_H_

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
#include "cp_mdu_def.h"
#endif

typedef enum CP_MMI_IMAGE_ID_E
{
    CP_IMAGE_NULL = ( MMI_MODULE_CP << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "cp_mdu_def.h"
#endif
    CP_IMAGE_MAX_ID
} CP_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //CP_MMI_IMAGE_H_
