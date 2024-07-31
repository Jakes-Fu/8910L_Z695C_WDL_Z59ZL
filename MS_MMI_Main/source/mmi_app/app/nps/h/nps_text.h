/*****************************************************************************
** File Name:      xxx_mmi_text.h                                            *
** Author:                                                                   *
** Date:           04/2009                                                   *
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef NPS_MMI_TEXT_H_
#define NPS_MMI_TEXT_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Text Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "nps_mdu_def.h"
#endif

typedef enum NPS_MMI_TEXT_ID_E
{
	NPS_TXT_NULL = ( MMI_MODULE_NPS << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "nps_mdu_def.h"
#endif
	HELLO_TXT_MAX
} NPS_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // NPS_MMI_TEXT_H_
