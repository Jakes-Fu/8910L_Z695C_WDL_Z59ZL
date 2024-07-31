/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/

#ifndef SPRD_OPDEV_OPERA_ID_H
#define SPRD_OPDEV_OPERA_ID_H

#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "mmi_appmsg.h"

#ifdef __cplusplus
extern   "C"
{
#endif

//MMI_MODULE_OPERA_MINI and MMI_MODULE_OPERA_MOBILE are defined in mmi_res_prj_def.h
//#define MMI_MODULE_OPERA_MINI   (62)
//#define MMI_MODULE_OPERA_MOBILE (MMI_MODULE_OPERA_MINI + 1)

#define WIN_ID_DEF(win_id, win_id_name)          win_id,
// window ID

#ifdef OPERA_MOBILE_SUPPORT
typedef enum
{
    OPERAMOBILE_WIN_ID_START = (MMI_MODULE_OPERA_MOBILE << 16),
    
#include "operamobile_win_id.def"

    OPERAMOBILE_MAX_WIN_ID
} OPERAMOBILE_WINDOW_ID_E;
#endif /* def OPERA_MOBILE_SUPPORT */ 

#ifdef OPERA_MINI_SUPPORT
typedef enum
{
    OPERAMINI_WIN_ID_START = (MMI_MODULE_OPERA_MINI << 16),

#include "operamini_win_id.def"

    OPERAMINI_MAX_WIN_ID
} OPERAMINI_WINDOW_ID_E;
#endif /* OPERA_MINI_SUPPORT */

#undef WIN_ID_DEF

#if defined(OPERA_MOBILE_SUPPORT) || defined(OPERA_MINI_SUPPORT)
//Common window/ctrl/message IDs

// Common window IDs
#ifdef OPERA_MOBILE_SUPPORT
#define OPERA_EDITOR_WIN_ID OPERAMOBILE_EDITOR_WIN_ID
#define OPERA_PASSWD_EDITOR_WIN_ID OPERAMOBILE_PASSWD_EDITOR_WIN_ID
#define OPERA_SIM_SELECTION_WIN_ID OPERAMOBILE_SIM_SELECTION_WIN_ID
#define OPERA_APN_SELECTION_WIN_ID OPERAMOBILE_APN_SELECTION_WIN_ID
#define OPERA_MAX_WIN_ID  OPERAMOBILE_MAX_WIN_ID
#else
#define OPERA_EDITOR_WIN_ID OPERAMINI_EDITOR_WIN_ID
#define OPERA_PASSWD_EDITOR_WIN_ID OPERAMINI_PASSWD_EDITOR_WIN_ID
#define OPERA_SIM_SELECTION_WIN_ID OPERAMINI_SIM_SELECTION_WIN_ID
#define OPERA_APN_SELECTION_WIN_ID OPERAMINI_APN_SELECTION_WIN_ID
#define OPERA_MAX_WIN_ID  OPERAMINI_MAX_WIN_ID
#endif

// Common control IDs
typedef enum
{
    OPERA_CTRL_ID_ID_START = OPERA_MAX_WIN_ID,
    OPERA_EDITOR_CTRL_ID,
    OPERA_PASSWD_EDITOR_CTRL_ID,
    OPERA_SIM_SELECTION_CTRL_ID,
    OPERA_APN_SELECTION_CTRL_ID,

#ifdef MMI_PDA_SUPPORT
    /* Form IDs for support PDA style MMI. */
    OPERA_EDITOR_FORM_ID,
    OPERA_PASSWD_EDITOR_FORM_ID,
#endif /* MMI_PDA_SUPPORT */

    OPERA_CTRL_ID_ID_MAX
}OPERA_CTRL_ID_E;

// Common message IDs
typedef enum
{
    OPERA_MMI_MSG_1 = MSG_MMI_OPERA_START,
    //TODO, add your mmi message here. handle it in _ProcessMmiMsg(main.c). 
    OPERA_MMI_ASYNC_CALL_MSG,
    OD_OPERA_TASK_MESSAGE,
    OPERA_MMI_MSG_MAX = MSG_MMI_OPERA_END
} OPERA_MMI_MSG_ID_E;

#define OPERA_ASYNC_CALL_MSG OPERA_MMI_ASYNC_CALL_MSG

#endif /* #if defined(OPERA_MOBILE_SUPPORT) || defined(OPERA_MINI_SUPPORT) */

#ifdef __cplusplus
};
#endif

#endif /* SPRD_OPDEV_OPERA_ID_H */
