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

#ifndef GAME_LINK_MMI_IMAGE_H_
#define GAME_LINK_MMI_IMAGE_H_

#include "sci_types.h"
#ifdef GAME_LINK_SUPPORT
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
#include "game_link_mdu_def.h"
#endif

typedef enum GAME_LINK_MMI_IMAGE_ID_E
{
	GAME_LINK_IMAGE_NULL = ( MMI_MODULE_GAME_LINK << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "game_link_mdu_def.h"
#endif
	GAME_LINK_IMAGE_MAX_ID
} GAME_LINK_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif
#endif
#endif //GAME_LINK_MMI_IMAGE_H_
