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

#include "operaminiapi.h"

OD_STATUS OperaMini_InitApp(void)
{
    // Nth to do.
    return OD_SUCCESS;
}

OD_STATUS OperaMini_Run(const OperaMiniRunOptions * const param)
{
    OD_Context* context = (OD_Context*)MMK_GetWinAddDataPtr((MMI_HANDLE_T)OPERAMINI_MAIN_WIN_ID);

    if (!context)
    {
        return OD_ERROR;
    }

	if (OperaMini_IsRunning())
	{
		// Opera window has been bring to front, if param->url is invalid, just do nothing.
		if (param && param->url)
		{
			return MOpApp_openPage(param->url);
		}
		return OD_SUCCESS;
	}
	else
	{
		return MOpApp_start((void*)param, NULL, NULL);
	}
}

OD_STATUS OperaMini_Exit(void)
{
    OD_Context *context = MMK_IsOpenWin(OPERAMINI_MAIN_WIN_ID)
        ? (OD_Context*)MMK_GetWinAddDataPtr((MMI_HANDLE_T)OPERAMINI_MAIN_WIN_ID)
        : NULL;
    if (!context || !IsValidContext(context))
    {
        /* Return OD_SUCCESS when opera not running */
        return OD_SUCCESS;
    }
    return context->callback.app.stopApp(NULL);
}

OD_STATUS OperaMini_BringToFront(void)
{
    OD_Context* context = NULL;
    if (!OperaMini_IsRunning())
    {
        return OD_ERROR;
    }

    context = (OD_Context*)MMK_GetWinAddDataPtr((MMI_HANDLE_T)OPERAMINI_MAIN_WIN_ID);;
    if (!context)
    {
        return OD_ERROR;
    }
    if (MMK_IsOpenWin(context->data.app.win_id))
    {
        if (context->data.app.win_id != MMK_GetFocusWinId())
        {
            MMK_WinGrabFocus(context->data.app.win_id);
        }
    }

    if (!OperaMini_IsInFront())
    {
        return MOpApp_resume();
    }
    return OD_SUCCESS;
}

char* OperaMini_GetVersionString(void)
{
    return MOpApp_getVersion();
}

OD_BOOL OperaMini_IsRunning(void)
{
    return (OD_BOOL)MOpApp_isRunning();
}

OD_BOOL OperaMini_IsInFront(void)
{
    return (OD_BOOL)MOpApp_isActive();
}

OD_STATUS OperaMini_FactoryReset(void)
{
    return MOpApp_factoryReset();
}

void OperaMini_ResetConnection(void)
{
    MOpApp_resetConnection();
}
