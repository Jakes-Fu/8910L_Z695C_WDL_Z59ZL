/*****************************************************************************
** File Name:      xxx_mmi_anim.h                                            *
** Author:                                                                   *
** Date:           05/2006                                                   *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef JAVA_MMI_ANIM_H_
#define JAVA_MMI_ANIM_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Animation Resource ID
///////////////////////////////////////////////////////////////////////////////

typedef enum JAVA_MMI_ANIM_ID_E
{
	JAVA_IMAGE_AMIN_NULL = MMI_MODULE_JAVA << 16,
	JAVA_IMAGE_AMIN_MAX
} JAVA_MMI_ANIM_ID_E;

#ifdef __cplusplus
}
#endif

#endif //JAVA_MMI_ANIM_H_
