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
#include "opdevstacktest.h"
#include "operamobileapi.h"

#include "mmi_appmsg.h"
#include "mmi_common.h"
#include "mmi_default.h"
#include "mmi_theme.h"
#include "mmk_type.h"
#include "mmidisplay_data.h"
#include "mmipub.h"
#include "mmiudisk_export.h"
#include "window_parse.h"

#include "opera_api_adapt_platform.h"


/************************************************************************/
/* Message Handler                                                      */
/************************************************************************/
#if defined(OD_BUILD_OPERA_MOBILE)

OD_MMI_TASK MMI_RESULT_E OperaMobile_MMIMessageHandler(PWND app_ptr, uint16 msg_id, DPARAM param)
{
	OD_Context* context = OperaMobile_GetODContext();

	if (!context || !IsValidContext(context))
	{
		/* Opera mobile not running now */
		OD_TRACE_LOW("Opera mobile not running now in OperaMobile_MMIMessageHandler, context = 0x%x", context);
		return MMI_RESULT_FALSE;
	}

	return OD_MMIMessageHandler(context, app_ptr, msg_id, param);
}

MMI_APPLICATION_T g_opera_mobile_app = { OperaMobile_MMIMessageHandler, CT_APPLICATION, NULL };


/************************************************************************/
/* Window Table                                                         */
/************************************************************************/
		
static WINDOW_TABLE(OPERAMOBILE_MAIN_WINDOW_TABLE) = 
{
    WIN_FUNC((uint32)OD_SPRD_MainWinProc),
    WIN_ID(OPERAMOBILE_MAIN_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif /* MMI_ONLY_IDLE_DISP_STATUSBAR */
    END_WIN
};


/************************************************************************/
/* App Entry                                                            */
/************************************************************************/

OD_MMI_TASK MMIBROWSER_ERROR_E OperaMobile_Entry(const MMIBROWSER_ENTRY_PARAM* entry_param_ptr)
{
	OD_Context* context = NULL;
	MMIBROWSER_ENTRY_PARAM *entry_param = NULL;

	OD_CHECK_ARGS(entry_param_ptr, MMIBROWSER_ERROR_INPUT);

#ifdef OD_SIM_DYNLINK_SUPPORT
	OD_InitFuncs();
#endif

#ifdef OD_STACK_TEST
	OD_StackTest_init();
	OD_StackTest_initTaskInfo(OD_STACKTEST_BREAM_TASK);
#endif

	context = OperaMobile_AllocODContext(sizeof(OD_Context));
	if (!context)
	{
		OD_TRACE_LOW("Error. Opera Mobile Context allocation fail.");
		return MMIBROWSER_FAIL;
	}

	OD_TRACE_LOW("OperaMobile Context 0x%x", context);

	if (!IsValidContext(context))
	{
		OD_STATUS ret;

		OD_InitDebugger();

		context->data.memory.size = OD_MEMORY_POOL_SIZE_MOBILE;
		context->data.app.win_id = OPERAMOBILE_MAIN_WIN_ID;
		context->data.app.module_id = MMI_MODULE_OPERA_MOBILE;

		ret = OperaMobile_InitODContext();
		if (ret != OD_SUCCESS)
		{
			OD_TRACE_LOW("Error. Opera Mobile Context Init Fail!");
			return MMIBROWSER_FAIL;
		}
	}

	if (opera_create_browser_entry_param(&entry_param) != OD_SUCCESS)
	{
		return MMIBROWSER_FAIL;
	}

	if (opera_clone_browser_entry_param(entry_param, entry_param_ptr) != MMIBROWSER_SUCC)
	{
		return MMIBROWSER_FAIL;
	}

	if (context->data.app.entry_arg)
	{
		opera_release_browser_entry_param(context->data.app.entry_arg);
	}

	context->data.app.entry_arg = entry_param;

	return OD_AppEntry(context, (uint32*)OPERAMOBILE_MAIN_WINDOW_TABLE, entry_param_ptr);
}

OD_MMI_TASK MMIBROWSER_ERROR_E operamobile_start(OD_Context* context, MMIBROWSER_ENTRY_PARAM* entry_param_ptr)
{
	OD_STATUS ret = OD_ERROR;

	if (!entry_param_ptr)
	{
		OD_TRACE_LOW("Error. Could not launch browser, MMIBROWSER_ENTRY_PARAM is NULL");
		return MMIBROWSER_FAIL;
	}

	switch (entry_param_ptr->type)
	{
	case MMIBROWSER_HOMEPAGE:
		{
			OD_APIRequest req;
			const char* argv[1];

			argv[0] = "-h";

			req.argc = 1;
			req.argv = argv;
			ret = context->callback.app.startApp(&req, NULL, NULL);
		}
	case MMIBROWSER_ACCESS_URL:
		{
			OD_APIRequest req;
			const char* argv[2];

			argv[0] = "-url";
			argv[1] = entry_param_ptr->url_ptr;

			req.argc = 2;
			req.argv = argv;
			ret = context->callback.app.startApp(&req, NULL, NULL);
		}
		break;
	case MMIBROWSER_ACCESS_MAINMENU:
		OD_TRACE_LOW("Error. MMIBROWSER_ACCESS_MAINMENU is not supported");
	default:
		{
			ret = context->callback.app.startApp(NULL, NULL, NULL);
		}
		break;
	}

	opera_release_browser_entry_param(entry_param_ptr);
	context->data.app.entry_arg = NULL;

	return (ret == OD_SUCCESS) ? MMIBROWSER_SUCC : MMIBROWSER_FAIL;
}

#endif /* OD_BUILD_OPERA_MOBILE */

#if defined(OPERA_MOBILE_SUPPORT) && defined(OPERA_MOBILE_DUMMY)
static MMI_RESULT_E OperaMobile_MMIMessageHandler(PWND app_ptr, uint16 msg_id, DPARAM param)
{
	return MMI_RESULT_FALSE;
}
MMI_APPLICATION_T g_opera_mobile_app = { OperaMobile_MMIMessageHandler, CT_APPLICATION, NULL };
#endif /* defined(OPERA_MOBILE_SUPPORT) && defined(OPERA_MOBILE_DUMMY) */
