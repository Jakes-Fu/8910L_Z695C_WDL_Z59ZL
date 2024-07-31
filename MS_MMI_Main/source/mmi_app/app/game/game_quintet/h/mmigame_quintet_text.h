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

#ifndef GAME_QUINTET_MMI_TEXT_H_
#define GAME_QUINTET_MMI_TEXT_H_
#ifdef GAME_QUINTET_SUPPORT
#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// ÎÄ×Ö×ÊÔ´ID
///////////////////////////////////////////////////////////////////////////////
//michael wang for5
#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

typedef enum GAME_QUINTET_MMI_TEXT_ID_E
{
	GAME_QUINTET_TXT_NULL = ( MMI_MODULE_GAME_QUINTET << 16 ) | MMI_SYS_RES_FLAG,
#include "game_quintet_mdu_def.h"
	GAME_QUINTET_TXT_MAX
} GAME_QUINTET_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // GAME_QUINTET_MMI_TEXT_H_
#endif
