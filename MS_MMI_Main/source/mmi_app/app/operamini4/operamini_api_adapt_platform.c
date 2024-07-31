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
#include "operaminiapi.h"

static MMIBROWSER_ERROR_E operamini_start(MMIBROWSER_ENTRY_PARAM * entry_param_ptr)
{
	if (!entry_param_ptr)
	{
		return MMIBROWSER_FAIL;
	}

	switch (entry_param_ptr->type)
	{
	case MMIBROWSER_ACCESS_MAINMENU:
	case MMIBROWSER_HOMEPAGE:
	case MMIBROWSER_ACCESS_URL:
		{
			OperaMiniRunOptions op = {0};
			op.url = entry_param_ptr->url_ptr;      // CHECK: Does this work for MMIBROWSER_HOMEPAGE?

			// TODO: Find a convention between caller and this api to set sim_index for wifi usage.
			if (entry_param_ptr->is_use_wifi)
			{
				op.net_options.sim_index = OD_SIM_INDEX_WIFI;
			}
			else if (entry_param_ptr->dual_sys >= 0 && entry_param_ptr->dual_sys < MMI_DUAL_SYS_MAX)
			{
				op.net_options.sim_index = entry_param_ptr->dual_sys;
			}
			else
			{
				op.net_options.sim_index = -1;
			}

#if defined(WIFI_SUPPORT) && !defined(WIFI_SUPPORT_UNISOC_RX)
			op.net_options.wifi_enabled = !entry_param_ptr->is_connection_customized;
#else
			op.net_options.wifi_enabled = OD_FALSE;
#endif
			/* According to spreadtrum, entry_param_ptr->connection_index is only valid in NetFront.
			So ignore it*/
			op.net_options.profile_index = -1;

			// CHECK: Does it work to call this directly? maybe need use OD_PostCall, but if so when/where to free the memory?
			OperaMini_Run(&op);
		}
		break;
	default:
		break;
	}
	return MMIBROWSER_SUCC;
}

#ifdef OD_TWEAK_IMPLEMENT_OPERAMINI_API

void OperaMini_InitModule(void);
void OperaMini_InitApplication(void);
void OperaMini_UninitApplication(void);
MMIBROWSER_ERROR_E OperaMini_ExitBrowser(void);
void OperaMini_SDCardPlugOutNotify(void);
void OperaMini_StorageFormatNotify(void);
void OperaMini_CleanCache(uint8 * url_ptr, uint16 url_len);
void OperaMini_DeleteCompletedDownloadTask(uint32 task_index);
void OperaMini_CheckTimerDisable(void);
void OperaMini_SetNetAccountBySim(MN_DUAL_SYS_E dual_sys, uint8 index);
uint8 OperaMini_GetNetAccountBySim(MN_DUAL_SYS_E dual_sys);

const MMIBROWSER_PROCESS_T g_opera_mini_proc = {
	OperaMini_InitModule, //InitModule,
	OperaMini_InitApplication, //InitApp
	OperaMini_UninitApplication, //UninitApp,
	OperaMini_Entry, //Entry,
	OperaMini_ExitBrowser, //Exit
	PNULL, //HandleAppMsg
	OperaMini_GetVersionString, //GetBrowserVersion
	PNULL, //SimPlugNotify
	OperaMini_SDCardPlugOutNotify, //SDCardPlugOutNotify
	OperaMini_StorageFormatNotify, //StorageFormatNotify
	OperaMini_SetNetAccountBySim, //SetNetAccountBySim
	OperaMini_GetNetAccountBySim, //GetNetSettingIndex,
	OperaMini_IsRunning, //IsRunning,
	OperaMini_IsInFront, //IsActive,
	OperaMini_FactoryReset, //ResetDefaultSettings,
	PNULL, //CleanUserInfo
	PNULL, //RegMimeHandleFunc
	OperaMini_CleanCache, //CleanCache
	OperaMini_DeleteCompletedDownloadTask, //DeleteCompletedDownloadTask
	OperaMini_CheckTimerDisable, //CheckTimerDisable
	PNULL, //SetDefaultBookmark
#ifndef PLATFORM_SC6600L
	//begin:for AT TEST
	PNULL, //Atest_BROWSERInsertURL
	PNULL, //Atest_BROWSERBookmarkTitleInput
	PNULL //Atest_BROWSERBookmarkURLInput
	//end:for AT TEST
#endif /* PLATFORM_SC6600L */
};

static OD_Context *_OD_GetContext()
{
    if (MMK_IsOpenWin((MMI_WIN_ID_T)OPERAMINI_MAIN_WIN_ID))
    {
        return (OD_Context*)MMK_GetWinAddDataPtr((MMI_HANDLE_T)OPERAMINI_MAIN_WIN_ID);
    }
    return NULL;
}

/* Implementation of MMIBrowser APIs. */

void OperaMini_InitModule(void)
{
    // Do nth.
}

MMIBROWSER_ERROR_E OperaMini_ExitBrowser(void)
{
	return OperaMini_Exit() == OD_SUCCESS ? MMIBROWSER_SUCC : MMIBROWSER_FAIL;
}

static void _OperaMini_NetworkConnectionChanged(MMICONNECTION_NOTIFY_EVENT_E notify_event, uint32 param, MN_DUAL_SYS_E dual_sys)
{
    OD_Context* context = _OD_GetContext();
    uint8 index = (uint8)param;

    OD_TRACE_LOW("MMIBROWSER_Setting_ConnectionCallback,index=%d,dual_sys=%d",index, dual_sys);
    if (!(context && IsValidContext(context)))
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
                OperaMini_ResetConnection();
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

static void _OperaMini_SIMPlugEventHandle(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param)
{
    OD_Context* context = _OD_GetContext();

    OD_TRACE_LOW("[MMIBROWSER]: HandleSIMPlugNotifyBrowserFunc dual_sys=%d, notify_event=%d", dual_sys, notify_event);
    if (!(context && IsValidContext(context)))
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
            MMK_CloseMiddleWin(OPERAMINI_WIN_ID_START, OPERAMINI_MAX_WIN_ID);
        }
        break;
    default:
        break;
    }
}

void OperaMini_InitApplication(void)
{
    MMICONNECTION_NOTIFY_INFO_T notify_info = {0};   
    MMIPHONE_SIM_PLUG_NOTIFY_INFO_T sim_plug_notify_info = {0};

    notify_info.module_id = MMI_MODULE_OPERA_MINI;
    notify_info.notify_func = _OperaMini_NetworkConnectionChanged;
    MMICONNECTION_RegisterNotifyInfo(&notify_info);

    sim_plug_notify_info.module_id = MMI_MODULE_OPERA_MINI;
    sim_plug_notify_info.notify_func = _OperaMini_SIMPlugEventHandle;
#if defined(OD_6530_12A) || defined(OD_6530_12B) || defined(OD_6530_12C) || defined(OD_8501_MAIN)
#if !defined(OD_6530_12C) && !defined(OD_8501_MAIN)
	MMIAPIPHONE_RegisterSIMPlugNotifyInfo(&sim_plug_notify_info);
#elif defined(SIM_PLUG_IN_SUPPORT)
    MMIAPIPHONE_RegisterSIMPlugNotifyInfo(&sim_plug_notify_info);
#endif
#else
    MMIPHONE_RegisterSIMPlugNotifyInfo(&sim_plug_notify_info);
#endif /* OD_6530_12A || OD_6530_12B || OD_6530_12C || OD_8501_MAIN */
    OperaMini_InitApp();
}

void OperaMini_UninitApplication(void)
{
	OD_Context* context = _OD_GetContext();

	if (context && IsValidContext(context))
	{
#if defined(OD_6530_12A) || defined(OD_6530_12B) || defined(OD_6530_12C) || defined(OD_8501_MAIN)
		MMIAPICONNECTION_UnregisterNotifyInfo(context->data.app.module_id);
#if !defined(OD_6530_12C) && !defined(OD_8501_MAIN)
		MMIAPIPHONE_UnregisterSIMPlugNotifyInfo(context->data.app.module_id);
#elif defined(SIM_PLUG_IN_SUPPORT)
		MMIAPIPHONE_UnregisterSIMPlugNotifyInfo(context->data.app.module_id);
#endif
#else
		MMICONNECTION_UnregisterNotifyInfo(context->data.app.module_id);
		MMIPHONE_UnregisterSIMPlugNotifyInfo(context->data.app.module_id);
#endif /* OD_6530_12A || OD_6530_12B || OD_6530_12C || OD_8501_MAIN */
	}
}

void OperaMini_SDCardPlugOutNotify(void)
{
	if (OperaMini_IsRunning())
	{
		OperaMini_Exit();
	}
	OD_ASSERT(!OperaMini_IsRunning());
}

void OperaMini_StorageFormatNotify(void)
{
	/* Simply exit opera to avoid any mediate status. */
	if (OperaMini_IsRunning())
	{
		OperaMini_Exit();
	}
	OD_ASSERT(!OperaMini_IsRunning());
}

void OperaMini_CleanCache(uint8 * url_ptr, uint16 url_len)
{
    // Do nth. Mini's cache is on Mini Server.
}

/* Copied from SDK. */
void OperaMini_DeleteCompletedDownloadTask(uint32 task_index)
{
    // Do nth. Is it meaning to just let Opera to delete one Nth download item, while no any api to get the list of download items?
}

void OperaMini_CheckTimerDisable(void)
{
    // TODO, if timer stopped at this point, when/where to start it again?
}

void OperaMini_SetNetAccountBySim(MN_DUAL_SYS_E dual_sys, uint8 index)
{
    OD_Context* context = _OD_GetContext();

    if (context && IsValidContext(context) && dual_sys >= 0 && dual_sys < MMI_DUAL_SYS_MAX)
    {
        context->data.net.selected_sim = dual_sys;
        context->data.net.profile_index[dual_sys] = index;
    }
}

uint8 OperaMini_GetNetAccountBySim(MN_DUAL_SYS_E dual_sys)
{
	OD_Context* context = _OD_GetContext();

	if (context && IsValidContext(context) && dual_sys >= 0 && dual_sys < MMI_DUAL_SYS_MAX)
	{
		return context->data.net.profile_index[dual_sys];
	}

	// TODO: check SDK reference for which to return for error.
	return 0xFF;
}
#endif /* OD_TWEAK_IMPLEMENT_OPERAMINI_API */
