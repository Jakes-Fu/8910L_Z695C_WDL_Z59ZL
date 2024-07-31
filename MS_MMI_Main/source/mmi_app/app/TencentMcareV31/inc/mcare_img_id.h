/*****************************************************************************
** File Name:      mcare_img_id.h                                               *
** Author:         wilsonzhu wiltan                                                *
** Date:           2011/06/06                                                *
** Copyright:      2011 Tencent, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe mcare img id      *
******************************************************************************/
#ifndef MCARE_IMG_H_
#define MCARE_IMG_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Text Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE   MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

typedef enum MCARE_IMG_ID_E
{
	MCARE_IMG_NULL = (MMI_MODULE_MCARE_MINI<< 16 )| MMI_SYS_RES_FLAG,
#include "tencentmcarev31_mdu_def.h"	
	MCARE_IMG_MAX
} MCARE_IMG_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // MCARE_MMI_IMG_H_
