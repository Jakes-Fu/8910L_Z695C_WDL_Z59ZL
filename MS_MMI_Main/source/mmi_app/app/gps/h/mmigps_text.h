/*****************************************************************************
** File Name:      xxx_mmi_text.h                                            *
** Author:                                                                   *
** Date:           07/2007                                                   *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef GPS_MMI_TEXT_H_
#define GPS_MMI_TEXT_H_

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
#include "gps_mdu_def.h"
#endif

typedef enum GPS_MMI_TEXT_ID_E
{
	GPS_TXT_NULL = ( MMI_MODULE_GPS << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "gps_mdu_def.h"
#endif
	GPS_TXT_MAX
} GPS_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // GPS_MMI_TEXT_H_
