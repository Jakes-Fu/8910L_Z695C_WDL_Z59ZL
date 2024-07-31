/*****************************************************************************
** File Name:      mmigame_snake_image.h                                           *
** Author:                                                                   *
** Date:            26/01/2011                                                   *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef _MMIGAME_SNAKE_IMAGE_H_
#define _MMIGAME_SNAKE_IMAGE_H_

#include "sci_types.h"
#ifdef GAME_SNAKE_SUPPORT
#include "mmi_imagefun.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Image Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE//为了加入mdu_def中的imageid到GAME_SNAKE_MMI_IMAGE_ID_E
#include "macro_mdu_def.h"

typedef enum GAME_SNAKE_MMI_IMAGE_ID_E
{
    GAME_SNAKE_IMAGE_NULL = ( MMI_MODULE_GAME_SNAKE << 16 ) | MMI_SYS_RES_FLAG,
    #include "game_snake_mdu_def.h"
    GAME_SNAKE_IMAGE_MAX_ID
} GAME_SNAKE_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif
#endif
#endif //_MMIGAME_SNAKE_IMAGE_H_