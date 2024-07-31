/*****************************************************************************
** File Name:      mmicp_text.h                                              *
** Author:                                                                   *
** Date:           2015/06/08                                                *
** Description:    This file is used to describe text                        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
*****************************************************************************/

#ifndef CP_MMI_TEXT_H_
#define CP_MMI_TEXT_H_

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
#include "cp_mdu_def.h"
#endif

typedef enum CP_MMI_TEXT_ID_E
{
    CP_TXT_NULL = ( MMI_MODULE_CP << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "cp_mdu_def.h"
#endif
    CP_TXT_MAX
} CP_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // CP_MMI_TEXT_H_
