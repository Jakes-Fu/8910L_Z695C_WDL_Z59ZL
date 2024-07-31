/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/

#include "opcfg.h"
#include "opdevinternal.h"
#include "opimport.h"

#include "mmi_module.h"
#include "mmi_modu_main.h"

#define WIN_ID_DEF(win_id, win_id_name)          win_id_name,

static const uint8 opera_mini_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
#include "operamini_win_id.def"    
};

static const uint8 opera_mobile_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
#include "operamobile_win_id.def"    
};

#undef WIN_ID_DEF

void OD_MobileResInit(OD_Context *context)
{
#ifdef OPERA_MOBILE_SUPPORT
#ifndef MMI_AUTOTEST_LOW_MEMORY
    MMI_RegWinIdNameArr(MMI_MODULE_OPERA_MOBILE, (const uint8**)opera_mobile_id_name_arr);/*lint !e64*/
#endif
#endif
}

void OD_MiniResInit(OD_Context *context)
{
#ifdef OPERA_MINI_SUPPORT
#ifndef MMI_AUTOTEST_LOW_MEMORY
    MMI_RegWinIdNameArr(MMI_MODULE_OPERA_MINI, (const uint8**)opera_mini_id_name_arr);/*lint !e64*/
#endif
#endif
}


int OD_GetBreamStringCount(OD_Context *context)
{
	OD_TRACE_LOW("Error. OD_GetBreamStringCount has not been implemented");

	return -1;
}

OD_STATUS OD_GetBreamString(OD_Context *context, int key, const OD_WCHAR **str)
{
	OD_TRACE_LOW("Error. OD_GetBreamString has not been implemented");

	return OD_NOT_SUPPORTED;
}

OD_STATUS OD_GetOperaString(OD_Context *context, int key, const OD_WCHAR **str)
{
	OD_TRACE_LOW("Error. OD_GetOperaString has not been implemented");

	return OD_NOT_SUPPORTED;
}

const OD_FileBlobType* OD_GetStaticFiles(OD_Context *context)
{
	(context);

	/* we don't use custom resources on 6600L due to lack of ROM */
#if defined(OD_BUILD_OPERA_MINI) && !defined(PLATFORM_SC6600L)
	return (const OD_FileBlobType*)OperaMini_GetCustomDeployFiles();
#else
	return NULL;
#endif
}

const OD_FileBlobType *OD_GetDeployFiles(OD_Context *context)
{
	OD_TRACE_LOW("Error. OD_GetDeployFiles has not been implemented");

	return OD_NULL;
}
