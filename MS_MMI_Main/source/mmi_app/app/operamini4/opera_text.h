/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */
#ifndef OPERA_TEXT_H_
#define OPERA_TEXT_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

typedef enum OPERA_MMI_TEXT_ID_E
{
#ifdef OPERA_MOBILE_SUPPORT
    OPERA_TXT_NULL = ( MMI_MODULE_OPERA_MOBILE << 16 ) | MMI_SYS_RES_FLAG,
    #include "operamobile_mdu_def.h"
#else//OPERA_MINI_SUPPORT
    OPERA_TXT_NULL = ( MMI_MODULE_OPERA_MINI << 16 ) | MMI_SYS_RES_FLAG,
    #include "operamini_mdu_def.h"
#endif

    OPERA_TXT_MAX
} OPERA_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // OPERA_TEXT_H_
