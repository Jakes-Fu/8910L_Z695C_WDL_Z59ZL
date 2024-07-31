/*****************************************************************************
** File Name:      xxx_mmi_text.h                                            *
** Author:                                                                   *
** Date:           05/2006                                                   *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool					              *
******************************************************************************/

#ifndef GAME_LINK_MMI_TEXT_H_
#define GAME_LINK_MMI_TEXT_H_

#include "sci_types.h"
#ifdef GAME_LINK_SUPPORT
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
#include "game_link_mdu_def.h"
#endif

typedef enum GAME_LINK_MMI_TEXT_ID_E
{
	GAME_LINK_TXT_NULL = ( MMI_MODULE_GAME_LINK << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "game_link_mdu_def.h"
#endif
	GAME_LINK_TXT_MAX
} GAME_LINK_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif
#endif
#endif // GAME_LINK_MMI_TEXT_H_
