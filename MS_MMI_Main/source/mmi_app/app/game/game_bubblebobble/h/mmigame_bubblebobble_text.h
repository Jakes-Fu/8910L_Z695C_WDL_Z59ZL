/*****************************************************************************
** File Name:      mmigame_bubblebobble_text.h                                          *
** Author:                                                                   *
** Date:             01/06/2012                                                   *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE                 NAME             DESCRIPTION                          *
** 01/06/2012     junxue.zhu        Create.                              *
*************************************************************************/

#ifndef GAME_BUBBLEBOBBLE_MMI_TEXT_H_
#define GAME_BUBBLEBOBBLE_MMI_TEXT_H_
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// ������ԴID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "game_bubblebobble_mdu_def.h"
#endif

typedef enum GAME_BUBBLEBOBBLE_MMI_TEXT_ID_E
{
	GAME_BUBBLEBOBBLE_TXT_NULL = ( MMI_MODULE_GAME_BUBBLEBOBBLE << 16 ) | MMI_SYS_RES_FLAG,
//#ifndef CODE_MATCH_RES_CHECK
#include "game_bubblebobble_mdu_def.h"
//#endif

	GAME_BUBBLEBOBBLE_TXT_MAX
} GAME_BUBBLEBOBBLE_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif
#endif
#endif // GAME_BOXMAN_MMI_TEXT_H_
