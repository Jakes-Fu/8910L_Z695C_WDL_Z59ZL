/*****************************************************************************
** File Name:      xxx_mmi_image.h                                           *
** Author:                                                                   *
** Date:           07/2007                                                   *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef KURO_MMI_IMAGE_H_
#define KURO_MMI_IMAGE_H_

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

typedef enum KURO_MMI_IMAGE_ID_E
{
	KURO_IMAGE_NULL = ( MMI_MODULE_KURO << 16 ) | MMI_SYS_RES_FLAG,
#include "kuro_mdu_def.h"
	KURO_IMAGE_MAX_ID
} KURO_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //KURO_MMI_IMAGE_H_
