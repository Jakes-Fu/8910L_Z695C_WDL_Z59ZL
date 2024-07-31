/*****************************************************************************
** File Name:      xxx_mmi_ring.h                                            *
** Author:                                                                   *
** Date:           05/2006                                                   *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef _MMI_RING_H__
#define _MMI_RING_H__

#include "sci_types.h"
#include "audio_config.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

typedef struct _AUD_RING_DATA_INFO_T
{
	AUD_RING_TYPE_E   type;
	uint32            data_len;
	uint8            *data_ptr;
} MMIAUD_RING_DATA_INFO_T, * PMMIAUD_RING_DATA_INFO_T;


typedef enum _MMI_RING_ID_E
{
	JAVA_RING_NULL = MMI_MODULE_JAVA << 16,
	RING_MAX_ID
} MMI_RING_ID_E;

#ifdef __cplusplus
}
#endif

#endif // _MMI_RING_H__