/*****************************************************************************
** File Name:      mmigame_goldminer_text.h                                  *
** Author:                                                                   *
** Date:           06/09/2011                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef _MMIGAME_GOLDMINER_TEXT_H_
#define _MMIGAME_GOLDMINER_TEXT_H_

#include "sci_types.h"
#ifdef GAME_GOLDMINER_SUPPORT
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

/*****************************************************************************/
// ÎÄ×Ö×ÊÔ´ID
/*****************************************************************************/

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

typedef enum GAME_GOLDMINER_MMI_TEXT_ID_E
{
    GAME_GOLDMINER_TXT_NULL = ( MMI_MODULE_GAME_GOLDMINER << 16 ) | MMI_SYS_RES_FLAG,
      #include "game_goldminer_mdu_def.h"
    GAME_GOLDMINER_TXT_MAX
}GAME_GOLDMINER_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif
#endif
#endif // _MMIGAME_GOLDMINER_TEXT_H_