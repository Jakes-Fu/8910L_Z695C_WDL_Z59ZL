/*****************************************************************************
** File Name:      mmigame_goldminer_image.h                                           *
** Author:                                                                   *
** Date:           06/09/2012                                                   *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef _MMIGAME_GOLDMINER_IMAGE_H_
#define _MMIGAME_GOLDMINER_IMAGE_H_

#include "sci_types.h"
#ifdef GAME_GOLDMINER_SUPPORT
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

typedef enum GAME_GOLDMINER_MMI_IMAGE_ID_E
{
    GAME_GOLDMINER_IMAGE_NULL = ( MMI_MODULE_GAME_GOLDMINER << 16 ) | MMI_SYS_RES_FLAG,
    #include "game_goldminer_mdu_def.h"
    GAME_GOLDMINER_IMAGE_MAX_ID
} GAME_GOLDMINER_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif
#endif
#endif //_MMIGAME_GOLDMINER_IMAGE_H_