/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/

#ifndef SPRD_OPERAMINI_API_ADAPT_PLATFORM_H
#define SPRD_OPERAMINI_API_ADAPT_PLATFORM_H

#include "opcfg.h"
#include "opdevinternal.h"
#include "opimport.h"


static MMIBROWSER_ERROR_E opera_clone_browser_entry_param(MMIBROWSER_ENTRY_PARAM *dst_entry_param, const MMIBROWSER_ENTRY_PARAM *src_entry_param);

static OD_STATUS opera_create_browser_entry_param(MMIBROWSER_ENTRY_PARAM **entry_param);

static void opera_release_browser_entry_param(MMIBROWSER_ENTRY_PARAM *entry_param);

#if defined(OD_BUILD_OPERA_MINI)
static OD_MMI_TASK MMIBROWSER_ERROR_E operamini_start(MMIBROWSER_ENTRY_PARAM * entry_param_ptr);
#endif /* OD_BUILD_OPERA_MINI */

#if defined(OD_BUILD_OPERA_MOBILE)
static OD_MMI_TASK MMIBROWSER_ERROR_E operamobile_start(OD_Context* context, MMIBROWSER_ENTRY_PARAM* entry_param_ptr);
#endif /* OD_BUILD_OPERA_MOBILE */


#endif /*SPRD_OPERAMINI_API_ADAPT_PLATFORM_H*/
