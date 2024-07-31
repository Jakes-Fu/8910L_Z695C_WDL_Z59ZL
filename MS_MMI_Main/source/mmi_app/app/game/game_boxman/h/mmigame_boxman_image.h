/*****************************************************************************
** File Name:      xxx_mmi_image.h                                           *
** Author:                                                                   *
** Date:           05/2006                                                   *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef GAME_BOXMAN_MMI_IMAGE_H_
#define GAME_BOXMAN_MMI_IMAGE_H_
#ifdef GAME_BOXMAN_SUPPORT
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
#include "game_boxman_mdu_def.h"
#endif

typedef enum GAME_BOXMAN_MMI_IMAGE_ID_E
{
	GAME_BOXMAN_IMAGE_NULL = ( MMI_MODULE_GAME_BOXMAN << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "game_boxman_mdu_def.h"
#endif

	GAME_BOXMAN_IMAGE_MAX_ID
} GAME_BOXMAN_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif
#endif
#endif //GAME_BOXMAN_MMI_IMAGE_H_
