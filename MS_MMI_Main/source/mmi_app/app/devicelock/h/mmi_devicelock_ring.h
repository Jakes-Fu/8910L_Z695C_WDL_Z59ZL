/*****************************************************************************
** File Name:      xxx_mmi_ring.h                                           *
** Author:                                                                   *
** Date:                                                        *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef _MMI_DEVICELOCK_RING_H_
#define _MMI_DEVICELOCK_RING_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// RING Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_RING
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "devicelock_mdu_def.h"
#endif

typedef enum DEVICELOCK_RING_ID_E
{
	DEVICELOCK_RING_NULL = ( MMI_MODULE_DEVICELOCK << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "devicelock_mdu_def.h"
#endif
	DEVICELOCK_RING_MAX_ID
} DEVICELOCK_RING_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //_MMI_DEVICELOCK_RING_H_
