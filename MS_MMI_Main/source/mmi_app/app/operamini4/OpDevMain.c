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


#include "mmi_appmsg.h"
#include "mmi_common.h"
#include "mmi_default.h"
#include "mmi_theme.h"
#include "mmk_type.h"
#include "mmipub.h"
#include "mmiudisk_export.h"
#include "mmidisplay_data.h"
#include "window_parse.h"
#include "opera_api_adapt_platform.h"

LOCAL OD_BOOL oprea_show=TRUE;
OD_BOOL oprea_search_delete = FALSE;
void OD_SPRD_SetOperaDisplay(OD_BOOL isShow)
{
    oprea_show = isShow;
}

OD_BOOL OD_SPRD_GetOperaDisplay(void)
{
   return oprea_show;
}

OD_STATUS OD_InitApp(OD_Context *context)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);
	context->data.app.entry_arg = NULL;
	return OD_SUCCESS;
}

OD_STATUS OD_DeinitApp(OD_Context *context)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);
	if (context->data.app.entry_arg)
	{
		opera_release_browser_entry_param(context->data.app.entry_arg);
		context->data.app.entry_arg = NULL;
	}
	return OD_SUCCESS;
}

static OD_MMI_TASK MMIBROWSER_ERROR_E _OD_HandleEntryParam(OD_Context* context)
{
	MMIBROWSER_ENTRY_PARAM *entry_param = NULL;

	OD_CHECK_ARGS(context, MMIBROWSER_ERROR_INPUT);

	entry_param = (MMIBROWSER_ENTRY_PARAM *)context->data.app.entry_arg;
	switch (context->data.app.win_id)
	{
#ifdef OD_BUILD_OPERA_MINI
	case OPERAMINI_MAIN_WIN_ID:
		{
			return operamini_start(entry_param);
		}
#endif
#ifdef OD_BUILD_OPERA_MOBILE
	case OPERAMOBILE_MAIN_WIN_ID:
		{
			return operamobile_start(context, entry_param);;
		}
#endif
	default:
		return MMIBROWSER_FAIL;
	}

	return MMIBROWSER_FAIL;
}

static OD_MMI_TASK MMIBROWSER_ERROR_E OD_AppEntry(OD_Context* context, uint32* win_table,
										   const MMIBROWSER_ENTRY_PARAM* entry_param_ptr)
{
#ifdef _DEBUG
	OD_TRACE_LOW("OD_GetUTCTime():%u", OD_GetUTCTime(context));
	OD_TRACE_LOW("SCI_GetCurrentTime():%u", SCI_GetCurrentTime());
	OD_TRACE_LOW("TM_GetTotalSeconds():%u", TM_GetTotalSeconds());
	OD_TRACE_LOW("OD_GetTickCount:%u", OD_GetTickCount());
	OD_TestJmp(100, 100);
#endif

	OD_CHECK_ARGS(context, MMIBROWSER_ERROR_INPUT);
	OD_CHECK_ARGS(win_table, MMIBROWSER_ERROR_INPUT);
	OD_CHECK_ARGS(entry_param_ptr, MMIBROWSER_ERROR_INPUT);

	if (MMIAPIUDISK_UdiskIsRun())
	{
		MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMMON_UDISK_USING, TXT_NULL,
			IMAGE_PUBWIN_WARNING, PNULL, PNULL,
			MMIPUB_SOFTKEY_ONE, PNULL);

		return MMIBROWSER_FAIL;
	}

	if (MMIAPISET_GetFlyMode())
	{
		MMIAPISET_AlertFlyMode();

		return MMIBROWSER_FAIL;
	}

	if (OD_GetStorageDriveLetter(context) < 0)
	{
		/*
		* Opera mobile and mini both need a place to put files. See HULA-204.
		* There is no string ID like TXT_NO_UDISK_AND_SDCARD, only TXT_NO_SD_CARD_ALERT
		*  or TXT_UDISK_NO_EXIST, and user can insert SDCARD, but not UDISK. Then alert TXT_NO_SD_CARD_ALERT.
		*/
		MMIPUB_OpenAlertWinByTextId(PNULL, TXT_NO_SD_CARD_ALERT, TXT_NULL,
			IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);

		return MMIBROWSER_FAIL;
	}

	// Continue network selection if it's suspended last time.
	// Note: Since we may have two windows opened simultaneously(the main
	// window and one of the network selection window), the priority here
	// to check the windows is reversed as what they're opened, to
	// make sure open the most recent window.
	if (MMK_IsOpenWin(OPERA_APN_SELECTION_WIN_ID))
	{
		if (OPERA_APN_SELECTION_WIN_ID != MMK_GetFocusWinId())
		{
			MMK_WinGrabFocus(OPERA_APN_SELECTION_WIN_ID);
		}

		return _OD_HandleEntryParam(context);
	}
	else if (MMK_IsOpenWin(OPERA_SIM_SELECTION_WIN_ID))
	{
		if (OPERA_SIM_SELECTION_WIN_ID != MMK_GetFocusWinId())
		{
			MMK_WinGrabFocus(OPERA_SIM_SELECTION_WIN_ID);
		}

		return _OD_HandleEntryParam(context);
	}
	else if (MMK_IsOpenWin(context->data.app.win_id))
	{
		if (context->data.app.win_id != MMK_GetFocusWinId())
		{
			MMK_WinGrabFocus(context->data.app.win_id);
		}

		return _OD_HandleEntryParam(context);
	}

	MMK_CreateWin(win_table, context);

	return MMIBROWSER_SUCC;
}


static MMI_RESULT_E OD_SPRD_MainWinProc(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	OD_Context* context = NULL;

	OD_CHECK_ARGS(win_id, MMI_RESULT_FALSE);

	context = (OD_Context*)MMK_GetWinAddDataPtr((MMI_HANDLE_T)win_id);
	OD_TRACE_LOW("OD_SPRD_MainWinProc event: 0x%04x, win_id=%d", msg_id, win_id);
	if(!context || !IsValidContext(context))
	{
		result = MMI_RESULT_FALSE;
		return result;
	}
	
	if(oprea_search_delete)
	{
		oprea_search_delete = FALSE;
		OD_TRACE_LOW("OD_SPRD_MainWinProc opera search delete");
		if(MSG_OPEN_WINDOW != msg_id)
		{
			MMK_PostMsg(context->data.app.win_id, MSG_KEYDOWN_OK, PNULL,0);
			SCI_Sleep(2);
			MMK_PostMsg(context->data.app.win_id, MSG_KEYUP_OK, PNULL,0);
			SCI_Sleep(2);

			MMK_PostMsg(context->data.app.win_id, MSG_KEYDOWN_CANCEL, PNULL,0);
			SCI_Sleep(2);
			MMK_PostMsg(context->data.app.win_id, MSG_KEYUP_CANCEL, PNULL,0);
			SCI_Sleep(2);
		}
	}
	//TODO: handle message.
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW :
        {
            result = _OD_HandleEntryParam(context) == MMIBROWSER_SUCC ? MMI_RESULT_TRUE : MMI_RESULT_FALSE;
        }
    case MSG_GET_FOCUS :
        {
            OD_ShowOpera(context);
            /* Don't need to check screen rotation here, instead, sdk will do it.*/
            context->callback.app.enterScreenCallback();
        }
        break;
    case MSG_CLOSE_WINDOW :
        {
            context->callback.app.uiExitCallback();
        }
		break;
	case MSG_LOSE_FOCUS :
		{
			context->callback.app.exitScreenCallback();
			OD_HideOpera(context);
		}
		break;
    case MSG_FULL_PAINT :
#if defined(OD_MULTILAYER_DISABLED)
        /* On non-6600 SDK, we paint everything on buffer of main layer. MSG_FULL_PAINT only require
        main layer content updated, shouldn't flush LCD(See HULA-722, HULA-392)
        On 6600 SDK, we paint directly on LCD buffer because no layers on 6600. We flush LCD to ensure
        opera's UI is on LCD buffer.
        */
        {
            /* The lcd buffer is dirty, need to repaint it with opera's data */
            int w = 0, h = 0;
            if (OD_SUCCESS == OD_GetLayerDimension(OD_LAYER_OPERA, &w, &h))
            {
                OD_Paint(context, 0, 0, w, h);
            }
        }
#endif /* defined(OD_MULTILAYER_DISABLED) */
		break;
#ifndef PLATFORM_SC6600L
		/* 6600 has no screen rotation support */
	case MSG_LCD_SWITCH :
		{
			context->callback.app.screenRotatedCallback();
		}
		break;
#endif
		//Touch event
	case MSG_TP_PRESS_DOWN:
	case MSG_TP_PRESS_UP:
	case MSG_TP_PRESS_MOVE:
		{
			int x, y, w, h;
			OD_PENEVENT pen_event;
			OD_GetViewportArea(context, &x, &y, &w, &h);
			x = MMK_GET_TP_X(param) - x;
			y = MMK_GET_TP_Y(param) - y;
			pen_event = OD_TranslatePenEvent(msg_id);
			if(x >= 0 && y >= 0)
			{
				context->callback.app.handlePenEvent(pen_event, x, y);
			}
			else
			{
				//FIXME, it should click on status bar area, we need to do something.
				result = MMI_RESULT_FALSE;
			}
		}
		break;
		//Special key event
	case MSG_KEYUP_RED:
		{
			context->callback.app.stopApp(OD_ExitUI);
#ifdef OD_7701_SUPPORT
			/* We should return mainmenu and we return TRUE here. */
			if(!MMK_IsOpenWin(MAIN_MAINMENU_WIN_ID))
			{
				MMIAPIMENU_CreatMainMenu();
				OD_TRACE_LOW("OD_SPRD_MainWinProc MMIAPIMENU_CreatMainMenu");
			}
            MMK_ReturnMainmenuWin();
			OD_TRACE_LOW("OD_SPRD_MainWinProc MMK_ReturnMainmenuWin");
			result = MMI_RESULT_TRUE;
#else
			/* HULA-470: We can't return to idle screen if we return TRUE here. */
			result = MMI_RESULT_FALSE;
#endif / * OD_7701_SUPPORT */
		}
		break;
    default:
        {
            uint32 msg_high = msg_id & 0xff00U;
            if( msg_high == KEY_PRESSED || msg_high == KEY_RELEASED 
                || msg_high == KEY_LONG_PRESSED || msg_high == KEY_LONG_RELEASED
                || msg_high == KEY_REPEATED)
            {
                OD_KEY key = OD_KEY_INVALID;
                key = OD_TranslateKeyCode(msg_id & 0xff);
                if (key != OD_KEY_INVALID)
                {
                    //Handle key event
                    if (msg_high == KEY_PRESSED)
                    {
                        context->callback.app.handleKeyEvent(OD_KEYEVENT_DOWN, key);
                    }
                    else if(msg_high == KEY_RELEASED || msg_high == KEY_LONG_RELEASED)
                    {
                        /* See PNGLAI-39. We need to handle KEY_LONG_RELEASED. */
                        context->callback.app.handleKeyEvent(OD_KEYEVENT_UP, key);
                    }
                    else
                    {
                        OD_TRACE_LOW("Unhandled key event: 0x%04x, but return TRUE", msg_id);
                    }
                }
                else
                {
                    OD_TRACE_LOW("Unhandled key event: 0x%04x", msg_id);
                    result = MMI_RESULT_FALSE;
                }
            }
            else
            {
                OD_TRACE_LOW("Unhandled event: 0x%04x", msg_id);
                result = MMI_RESULT_FALSE;
            }
        }
		break;
	}

	return result;
}

void OD_ExitUI(OD_Context *context)
{
	OD_CHECK_ARGS_RETURN_VOID(context);
	/* HULA-1068, Windows related to network should be closed also. */
	OD_NetCloseWin();
	if (MMK_IsOpenWin((MMI_HANDLE_T)context->data.app.win_id))
	{
		MMK_CloseWin((MMI_HANDLE_T)context->data.app.win_id);
	}
	OD_SPRD_SetOperaDisplay(FALSE);

}

void OD_ExitUI_ID(void)
{
    /* HULA-1068, Windows related to network should be closed also. */
    OD_NetCloseWin();
    if (MMK_IsOpenWin(OPERAMINI_MAIN_WIN_ID))
    {
        MMK_CloseWin(OPERAMINI_MAIN_WIN_ID);
    }
    OD_SPRD_SetOperaDisplay(FALSE);
}

/************************************************************************/
/* Message Handler                                                      */
/* - from SOCKET_SIG_BEGIN to SOCKET_SIG_END                            */
/* - from MSG_MMI_OPERA_START to MSG_MMI_OPERA_END                      */
/************************************************************************/
static MMI_RESULT_E OD_MMIMessageHandler(OD_Context* context, PWND app_ptr, uint16 msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_FALSE;

	/**
	 * Spreadtrum said one platform maybe use different SDK, the new one and the old one.
	 * There are two kind of MMI APIs for the two SDK, and we can not decide which one should be used.
	 * So we need spreadtrum to define the macro #OD_8501_MAIN in the makefile "app.mk" to help us distinguish it .
	 */
#ifdef OD_8501_MAIN
	xSignalHeaderRec *event = (xSignalHeaderRec*)(param);
#else
	xSignalHeaderRec *event = (xSignalHeaderRec*)((uint32)param - od_offsetof(MmiSignalS, sig));
#endif /* OD_8501_MAIN */

	OD_TRACE_LOW("OD_MMIMessageHandler: app_ptr=%d, msg_id=0x%x", app_ptr, msg_id);

	OD_CHECK_ARGS(context, MMI_RESULT_FALSE);

	if (msg_id >= SOCKET_SIG_BEGIN && msg_id <= SOCKET_SIG_END)
	{
		if (OD_SocketEventDispatcher(context, (OD_SYS_Event*)event))
		{
			result = MMI_RESULT_TRUE;
		}
	}
	else if (msg_id >= MSG_MMI_OPERA_START && msg_id <= MSG_MMI_OPERA_END)
	{
		OD_TaskMessageHandler(context, (OD_TaskMsg*)event);

		result = MMI_RESULT_TRUE;
	}

	return result;
}
