/*****************************************************************************
** File Name:      mmigame_bubblebobble_image.h                                      *
** Author:                                                                   *
** Date:             01/06/2012                                                   *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe bubble image id                   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                   NAME             DESCRIPTION                             *
** 2006/06/01       junxue.zhu           Create
** 
****************************************************************************/

#ifndef GAME_BUBBLEBOBBLE_MMI_IMAGE_H_
#define GAME_BUBBLEBOBBLE_MMI_IMAGE_H_
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
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
#include "game_bubblebobble_mdu_def.h"
#endif

typedef enum GAME_BUBBLEBOBBLE_MMI_IMAGE_ID_E
{
	GAME_BUBBLEBOBBLE_IMAGE_NULL = ( MMI_MODULE_GAME_BUBBLEBOBBLE << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "game_bubblebobble_mdu_def.h"
#endif

	GAME_BUBBLEBOBBLE_IMAGE_MAX_ID
} GAME_BUBBLEBOBBLE_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif
#endif
#endif //GAME_BUBBLEBOBBLE_MMI_IMAGE_H_
