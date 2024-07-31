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

#ifndef GAME_MANAGER_MMI_TEXT_H_
#define GAME_MANAGER_MMI_TEXT_H_

#include "sci_types.h"
#ifdef GAME_SUPPORT
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// ÎÄ×Ö×ÊÔ´ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "game_manager_mdu_def.h"
#endif

typedef enum GAME_MANAGER_MMI_TEXT_ID_E
{
	GAME_MANAGER_TXT_NULL = ( MMI_MODULE_GAME_MANAGER << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "game_manager_mdu_def.h"
#endif
	GAME_MANAGER_TXT_MAX
} GAME_MANAGER_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif
#endif //GAME_SUPPORT
#endif // GAME_MANAGER_MMI_TEXT_H_
