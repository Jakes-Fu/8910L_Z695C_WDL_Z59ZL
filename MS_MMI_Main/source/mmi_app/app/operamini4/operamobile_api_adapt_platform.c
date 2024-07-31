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

#include "opera_api_adapt_platform.h"

static OD_Context *_OD_GetContext()
{
	if (MMK_IsOpenWin((MMI_WIN_ID_T)OPERAMOBILE_MAIN_WIN_ID))
	{
		return (OD_Context*)MMK_GetWinAddDataPtr((MMI_HANDLE_T)OPERAMOBILE_MAIN_WIN_ID);
	}

	return NULL;
}

#ifdef OD_TWEAK_OPERAMOBILE_IMPLEMENT_MMIBROWSER_API

void MMIBROWSER_InitModule(void)
{
	// Do nth.
}

static void _MMIBROWSER_OperaMobile_NetworkConnectionChanged(MMICONNECTION_NOTIFY_EVENT_E notify_event, uint32 param, MN_DUAL_SYS_E dual_sys)
{
	OD_Context* context = _OD_GetContext();
	uint8 index = (uint8)param;

	OD_TRACE_LOW("_MMIBROWSER_OperaMobile_NetworkConnectionChanged, index=%d, dual_sys=%d", index, dual_sys);
	if (!context)
	{
		return;
	}

	switch(notify_event)
	{
	case MMICONNECTION_NOTIFY_EVENT_DEL_ITEM:
		if (dual_sys >= 0 && dual_sys < MMI_DUAL_SYS_MAX)
		{
			if(context->data.net.profile_index[dual_sys] == index)
			{
				context->data.net.profile_index[dual_sys] = 0;
				// Reset connection here, to be done in PNGLAI-332
			}
			else if(context->data.net.profile_index[dual_sys] > index)
			{
				context->data.net.profile_index[dual_sys]--;
			}
		}
		break;
	default:
		break;
	}
}

static void _MMIBROWSER_OperaMobile_SIMPlugEventHandle(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param)
{
	OD_Context* context = _OD_GetContext();

	OD_TRACE_LOW("_MMIBROWSER_OperaMobile_SIMPlugEventHandle dual_sys=%d, notify_event=%d", dual_sys, notify_event);
	if (!context)
	{
		return;
	}

	switch (notify_event)
	{
	case MMIPHONE_NOTIFY_SIM_PLUG_IN:
		// do nth
		break;
	case MMIPHONE_NOTIFY_SIM_PLUG_OUT:
		if (context->data.net.selected_sim == dual_sys
			&& context->data.net.selected_sim != MMI_DUAL_SYS_MAX)
		{
			MMK_CloseMiddleWin(OPERAMOBILE_MAX_WIN_ID, OPERAMOBILE_MAX_WIN_ID);
		}
		break;
	default:
		break;
	}
}

void MMIBROWSER_InitApp(void)
{
	MMICONNECTION_NOTIFY_INFO_T notify_info = {0};   
	MMIPHONE_SIM_PLUG_NOTIFY_INFO_T sim_plug_notify_info = {0};

	notify_info.module_id = MMI_MODULE_OPERA_MOBILE;
	notify_info.notify_func = _MMIBROWSER_OperaMobile_NetworkConnectionChanged;
	MMICONNECTION_RegisterNotifyInfo(&notify_info);

	sim_plug_notify_info.module_id = MMI_MODULE_OPERA_MOBILE;
	sim_plug_notify_info.notify_func = _MMIBROWSER_OperaMobile_SIMPlugEventHandle;
	MMIPHONE_RegisterSIMPlugNotifyInfo(&sim_plug_notify_info);
}

void MMIBROWSER_ResetFactorySetting(void)
{
	// TODO, PNGLAI-332
}

void MMIAPIBROWSER_CleanCache(uint8 * url_ptr, uint16 url_len)
{
	// TODO, PNGLAI-332
}

/* Copied from SDK. */
BOOLEAN MMIBROWSER_SetBrowserActive(void)
{
	MMIBROWSER_ENTRY_PARAM entry_param = {0};

	entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
	entry_param.dual_sys = MN_DUAL_SYS_MAX;
	entry_param.url_ptr = NULL;
	entry_param.user_agent_ptr = NULL;
	MMIBROWSER_Entry(&entry_param);

	return TRUE;
}

char* MMIBROWSER_GetVersion(void)
{
	// To be done in PNGLAI-332
	return "0";
}

void MMIBROWSER_DeleteCompletedDownloadTask(uint32 task_index)
{
	// TODO, PNGLAI-332
}

void MMIAPIBROWSER_CheckTimerDisable(void)
{
	// TODO, PNGLAI-332
}

void MMIAPIBROWSER_SetNetSettingIndex(MN_DUAL_SYS_E dual_sys, uint8 index)
{
	OD_Context* context = _OD_GetContext();

	if (context && dual_sys >= 0 && dual_sys < MMI_DUAL_SYS_MAX)
	{
		context->data.net.selected_sim = dual_sys;
		context->data.net.profile_index[dual_sys] = index;
	}
}

#endif /* OD_TWEAK_OPERAMOBILE_IMPLEMENT_MMIBROWSER_API */
