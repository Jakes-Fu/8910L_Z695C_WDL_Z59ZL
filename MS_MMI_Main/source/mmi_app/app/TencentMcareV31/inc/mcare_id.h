/*****************************************************************************
** File Name:      mcare_id.h                                               *
** Author:         wilsonzhu wiltan                                                *
** Date:           2011/06/06                                                *
** Copyright:      2011 Tencent, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe mcare win and control id      *
******************************************************************************/
#ifndef _MCARE_ID_H_
#define _MCARE_ID_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Screen ID
///////////////////////////////////////////////////////////////////////////////

#define MAX_SCREEN_NUM (25)

#define WIN_ID_DEF(win_id)          win_id

// window ID
typedef enum
{
	MCARE_WIN_ID_START = (MMI_MODULE_MCARE_MINI << 16),

#include "mcare_id.def"
	MCARE_WIN_ID_START_PLAT,

	MCARE_MAX_WIN_ID = MCARE_WIN_ID_START + MAX_SCREEN_NUM
}MCARE_WINDOW_ID_E;

#undef WIN_ID_DEF

///////////////////////////////////////////////////////////////////////////////
// Control ID
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
	MCARE_CTRL_ID_ID_START = MCARE_MAX_WIN_ID,
	MMIMCR_EDIT_CONTROL_ID,
	MCARE_MAX_CTRL_ID = MCARE_MAX_WIN_ID + 100
}MCARE_CONTROL_ID_E;

#ifdef __cplusplus
}
#endif

#endif // _MCARE_ID_H_