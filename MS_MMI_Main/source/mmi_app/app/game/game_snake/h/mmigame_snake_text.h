/*****************************************************************************
** File Name:      mmigame_snake_text.h                                            *
** Author:                                                                   *
** Date:            26/01/2011                                                   *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                  *
******************************************************************************/

#ifndef _MMIGAME_SNAKE_TEXT_H_
#define _MMIGAME_SNAKE_TEXT_H_

#include "sci_types.h"
#ifdef GAME_SNAKE_SUPPORT
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// 文字资源ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT//为了加入mdu_def中的imageid到GAME_SNAKE_MMI_TEXT_ID_E
#include "macro_mdu_def.h"

typedef enum GAME_SNAKE_MMI_TEXT_ID_E
{
    GAME_SNAKE_TXT_NULL = ( MMI_MODULE_GAME_SNAKE << 16 ) | MMI_SYS_RES_FLAG,
      #include "game_snake_mdu_def.h"
    GAME_SNAKE_TXT_MAX
} GAME_SNAKE_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif
#endif
#endif // _MMIGAME_SNAKE_TEXT_H_