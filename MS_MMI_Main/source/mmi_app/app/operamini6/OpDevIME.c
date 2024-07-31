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

#include "mmidisplay_data.h"
#include "window_parse.h"
#include "mmk_type.h"
#include "mmi_appmsg.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_default.h"
#include "mmi_theme.h"

#ifdef MMI_PDA_SUPPORT
#include "guiform.h"
#endif /* MMI_PDA_SUPPORT */

#ifdef PLATFORM_SC6600L
typedef enum
{
	GUIIM_TYPE_NONE,
	GUIIM_TYPE_SMART,
	GUIIM_TYPE_STROKE,
	GUIIM_TYPE_MULTITAP,
	GUIIM_TYPE_DIGITAL,
	GUIIM_TYPE_ABC,
	GUIIM_TYPE_ENGLISH,
	GUIIM_TYPE_SYMBOL
} GUIIM_TYPE_T;

typedef enum
{
	GUIEDIT_STYLE_SINGLE,
	GUIEDIT_STYLE_MULTI
} GUIEDIT_STYLE_E;

#define GUIEDIT_SetMaxLen GUIEDITBOX_SetEditBoxMaxLen
//FIXME how to set input mode in 6600.
#define GUIEDIT_SetIm(a,b,c)
#define GUIEDIT_SetString GUIEDITBOX_SetTextEditBoxStringInfo
#define GUIEDIT_GetString GUIEDITBOX_GetTextEditBoxInfo
static void GUIEDIT_SetStyle(MMI_CTRL_ID_T ctrl, GUIEDIT_STYLE_E style)
{
	OD_CHECK_ARGS_RETURN_VOID(ctrl);

	/* By default, the editor is multiline. So for multiline style, we do nothing.
	Besides, seems no way to set multiline style in 6600. */
	if(style == GUIEDIT_STYLE_SINGLE)
	{
		GUIEDITBOX_SetEditBoxToSingle(ctrl, SINGLE_ALIGN_CENTER);
	}
}

#endif

static GUIIM_TYPE_T _OD_TransferInputModeToPlatform(OD_InputType odtype);

#define OPERA_EDIT_MAX_LENGTH 1024
static MMI_RESULT_E HandleOperaEditWinMsg(
	MMI_WIN_ID_T win_id,
	MMI_MESSAGE_ID_E msg_id,
	DPARAM param
	);

static WINDOW_TABLE(OPERA_EDIT_WINDOW_TABLE) =
{
	WIN_FUNC((uint32)HandleOperaEditWinMsg),
	WIN_ID(OPERA_EDITOR_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
	WIN_STATUSBAR,
#endif /* MMI_ONLY_IDLE_DISP_STATUSBAR */
	WIN_TITLE(TXT_EDIT),
#ifdef MMI_PDA_SUPPORT
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, OPERA_EDITOR_FORM_ID),
	CHILD_EDIT_TEXT_CTRL(TRUE, OPERA_EDIT_MAX_LENGTH, OPERA_EDITOR_CTRL_ID, OPERA_EDITOR_FORM_ID),
	CHILD_SOFTKEY_CTRL(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN, MMICOMMON_SOFTKEY_CTRL_ID, OPERA_EDITOR_FORM_ID),
#else /* MMI_PDA_SUPPORT */
	WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
#ifdef PLATFORM_SC6600L
	CREATE_TEXTEDITBOX_CTRL(MMI_EDITBOX_FULLSCREEN_CLIENT_LEFT, MMI_EDITBOX_FULLSCREEN_CLIENT_TOP,
	MMI_EDITBOX_FULLSCREEN_CLIENT_RIGHT, MMI_EDITBOX_FULLSCREEN_CLIENT_BOTTOM,
	OPERA_EDIT_MAX_LENGTH, OPERA_EDIT_MAX_LENGTH,
	IM_DEFAULT_ALL_INPUT_MODE_SET,
	IM_ENGLISH_MODE, OPERA_EDITOR_CTRL_ID),
#else /* PLATFORM_SC6600L */
	CREATE_EDIT_TEXT_CTRL(OPERA_EDIT_MAX_LENGTH, OPERA_EDITOR_CTRL_ID),
#endif /* PLATFORM_SC6600L */
#endif /* MMI_PDA_SUPPORT */
	END_WIN
};

static WINDOW_TABLE(OPERA_PASSWD_EDIT_WINDOW_TABLE) =
{
	WIN_FUNC((uint32)HandleOperaEditWinMsg),
	WIN_ID(OPERA_PASSWD_EDITOR_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
	WIN_STATUSBAR,
#endif /* MMI_ONLY_IDLE_DISP_STATUSBAR */
	WIN_TITLE(TXT_EDIT),
#ifdef MMI_PDA_SUPPORT
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, OPERA_PASSWD_EDITOR_FORM_ID),
	CHILD_EDIT_PASSWORD_CTRL(TRUE, OPERA_EDIT_MAX_LENGTH, OPERA_PASSWD_EDITOR_CTRL_ID, OPERA_PASSWD_EDITOR_FORM_ID),
	CHILD_SOFTKEY_CTRL(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN, MMICOMMON_SOFTKEY_CTRL_ID, OPERA_PASSWD_EDITOR_FORM_ID),
#else /* MMI_PDA_SUPPORT */
	WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
	/*password edit in SC6600l just support digital input, so we use text editor here.*/
#ifdef PLATFORM_SC6600L
	CREATE_TEXTEDITBOX_CTRL(MMI_EDITBOX_FULLSCREEN_CLIENT_LEFT, MMI_EDITBOX_FULLSCREEN_CLIENT_TOP,
	MMI_EDITBOX_FULLSCREEN_CLIENT_RIGHT, MMI_EDITBOX_FULLSCREEN_CLIENT_BOTTOM,
	OPERA_EDIT_MAX_LENGTH, OPERA_EDIT_MAX_LENGTH,
	IM_DEFAULT_ALL_INPUT_MODE_SET,
	IM_ENGLISH_MODE, OPERA_PASSWD_EDITOR_CTRL_ID),
#else /* PLATFORM_SC6600L */
	CREATE_EDIT_PASSWORD_CTRL(OPERA_EDIT_MAX_LENGTH, OPERA_PASSWD_EDITOR_CTRL_ID),
#endif /* PLATFORM_SC6600L */
#endif /* MMI_PDA_SUPPORT */
	END_WIN
};

static MMI_RESULT_E HandleOperaEditWinMsg(
	MMI_WIN_ID_T win_id,
	MMI_MESSAGE_ID_E msg_id,
	DPARAM param
	)
{
	MMI_RESULT_E    result = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T   ctrl_id = OPERA_EDITOR_CTRL_ID;

	OD_CHECK_ARGS(win_id, MMI_RESULT_FALSE);

	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
			OD_Context *context = (OD_Context *)MMK_GetWinAddDataPtr((MMI_HANDLE_T)win_id);
			GUIIM_TYPE_T initial_im = GUIIM_TYPE_NONE;
			GUIIM_TYPE_T allow_im = GUIIM_TYPE_SMART | GUIIM_TYPE_STROKE | GUIIM_TYPE_MULTITAP
				| GUIIM_TYPE_DIGITAL | GUIIM_TYPE_ABC | GUIIM_TYPE_ENGLISH;
			GUIEDIT_STYLE_E style;

			OD_ASSERT(context);

			/* reset password editor widget theme to normal for good user experience*/
			if(context->data.ime.input_type & OD_INPUT_TYPE_PASSWORD)
			{
				/* SC6600L's sdk doesn't support theme setting functions*/
#ifdef PLATFORM_SC6600L
				ctrl_id = OPERA_PASSWD_EDITOR_CTRL_ID;
#else
				MMITHEME_EDIT_T theme = {0};
				IGUICTRL_T *ctrl_ptr = NULL;

				ctrl_id = OPERA_PASSWD_EDITOR_CTRL_ID;
				ctrl_ptr = MMK_GetCtrlPtr(ctrl_id);
				MMITHEME_GetEditTheme(&theme);
				IGUICTRL_SetBg(ctrl_ptr, &(theme.bg));
				IGUICTRL_SetFont(ctrl_ptr, &(theme.font));
				IGUICTRL_SetBorder(ctrl_ptr, &(theme.border));
				GUIEDIT_SetPasswordStyle(ctrl_id, GUIEDIT_PASSWORD_STYLE_ALL);
#endif
			}

			if(context->data.ime.max_length >= 0)
			{
				GUIEDIT_SetMaxLen(ctrl_id, context->data.ime.max_length);
			}

			initial_im = _OD_TransferInputModeToPlatform(context->data.ime.input_type);
			/*
			* HULA-196: Fullscreen edit input method is not remembered.
			* Setting initial input method to GUIIM_TYPE_NONE will lose the remembered one.
			*
			* HULA-460: password editor need set allowed inputmethods, otherwise it only accepts
			* digital input.
			*/
			if (initial_im != GUIIM_TYPE_NONE || context->data.ime.input_type & OD_INPUT_TYPE_PASSWORD)
			{
				GUIEDIT_SetIm(ctrl_id, allow_im, initial_im);
			}

			if(context->data.ime.input_type & OD_INPUT_TYPE_MULTILINE)
			{
				style = GUIEDIT_STYLE_MULTI;
			}
			else
			{
				style = GUIEDIT_STYLE_SINGLE;
			}
			GUIEDIT_SetStyle(ctrl_id, style);

			if(context->data.ime.initial_text && context->data.ime.initial_text_length >0)
			{
				GUIEDIT_SetString(ctrl_id, (wchar*)context->data.ime.initial_text, (uint16)context->data.ime.initial_text_length);
			}
			GUIEDIT_SetSoftkey(ctrl_id,0,1,TXT_NULL,STXT_SOFTKEY_OK_MK,PNULL);
			GUIEDIT_SetSoftkey(ctrl_id, 2, 1, STXT_SOFTKEY_CANCEL_MK, STXT_SOFTKEY_DELETE_MK, PNULL);

			MMK_SetAtvCtrl(win_id, ctrl_id);
			break;
		}
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT
		/* Refer to EngVoiceParamWinHandleMsg in mmieng_win.c. */
		if (NULL != param)
		{
			uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;

			switch (src_id)
			{
			case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
				MMK_SendMsg(win_id, MSG_APP_OK, NULL);
				break;
			case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
				break;
			case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
				MMK_SendMsg(win_id, MSG_APP_CANCEL, NULL);
				break;
			default:
				break;
			}
		}
		break;
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
	case MSG_CTL_OK:
	case MSG_APP_OK:
		{
			OD_Context *context = (OD_Context *)MMK_GetWinAddDataPtr((MMI_HANDLE_T)win_id);
			MMI_STRING_T    new_str = {0};

			OD_ASSERT(context);
			if(context->data.ime.input_type & OD_INPUT_TYPE_PASSWORD)
			{
				ctrl_id = OPERA_PASSWD_EDITOR_CTRL_ID;
			}
			//get string info
			GUIEDIT_GetString(ctrl_id, &new_str);
			if(context->callback.ime.done_cb)
				context->callback.ime.done_cb(context, new_str.wstr_ptr, new_str.wstr_len);
			MMK_CloseWin(win_id);
			break;
		}

	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		{
			OD_Context *context = (OD_Context *)MMK_GetWinAddDataPtr((MMI_HANDLE_T)win_id);
			OD_ASSERT(context);

			if(context->callback.ime.cancel_cb)
				context->callback.ime.cancel_cb(context);
			MMK_CloseWin(win_id);
			break;
		}

	default:
		result = MMI_RESULT_FALSE;
		break;
	}

	return result;
}




OD_STATUS OD_EnterFullScreenEditor(OD_Context *context)
{
	MMI_HANDLE_T handle = MMI_INVALID_ID;
	uint32* editWinTable = (uint32 *)OPERA_EDIT_WINDOW_TABLE;
	MMI_WIN_ID_T editWinID = OPERA_EDITOR_WIN_ID;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(context->callback.ime.done_cb);
	OD_CHECK_ARGS_RETURN_STATUS(context->callback.ime.cancel_cb);

	if(context->data.ime.input_type & OD_INPUT_TYPE_PASSWORD)
	{
		editWinTable = (uint32 *)OPERA_PASSWD_EDIT_WINDOW_TABLE;
		editWinID = OPERA_PASSWD_EDITOR_WIN_ID;
	}

	if(MMK_IsOpenWin(editWinID))
		MMK_CloseWin(editWinID);

	handle = MMK_CreateWin(editWinTable, (ADD_DATA)context);
	return (MMI_INVALID_ID != handle) ? OD_SUCCESS : OD_ERROR;

}

OD_STATUS OD_CloseFullScreenEditor(OD_Context *context)
{
	MMI_WIN_ID_T editWinID = OPERA_EDITOR_WIN_ID;
	OD_CHECK_ARGS_RETURN_STATUS(context);

	if(context->data.ime.input_type & OD_INPUT_TYPE_PASSWORD)
		editWinID=OPERA_PASSWD_EDITOR_WIN_ID;

	if(MMK_IsOpenWin(editWinID))
	{
		MMK_CloseWin(editWinID);
	}

	return OD_SUCCESS;
}

static GUIIM_TYPE_T _OD_TransferInputModeToPlatform(OD_InputType odtype)
{
	GUIIM_TYPE_T type = GUIIM_TYPE_NONE;
	if(odtype & OD_INPUT_TYPE_DIGIT
		|| odtype & OD_INPUT_TYPE_PHONENUMBER)
	{
		type = GUIIM_TYPE_DIGITAL;
	}
	else if(odtype & OD_INPUT_TYPE_URL
		|| odtype & OD_INPUT_TYPE_EMAIL
		|| odtype & OD_INPUT_TYPE_USERNAME
		|| odtype & OD_INPUT_TYPE_LATIN
		|| odtype & OD_INPUT_TYPE_ALPHANUMERIC)
	{
		/* We should set type to GUIIM_TYPE_ENGLISH/ABC, but actually they are outdated and setting input method
		to them will cause HULA-169. Then leave type to GUIIM_TYPE_NONE. */
	}
	return type;
}

/* Dummy functions. */
void OD_ImeEnterScreen(OD_Context* context){ OD_ASSERT(FALSE); }
void OD_ImeExitScreen(OD_Context* context){ OD_ASSERT(FALSE); }

void OD_ImeConnect(OD_Context* context, OD_InputType input_type, const OD_WCHAR* lsk_label, const OD_WCHAR* rsk_label){ OD_ASSERT(FALSE); }
void OD_ImeDisconnect(OD_Context* context){ OD_ASSERT(FALSE); }

void OD_ImeBeforeEditorOrCaretChanged(void){ OD_ASSERT(FALSE); }
int OD_ImeGetImuiHeight(void){ OD_ASSERT(FALSE); return 0; }
void OD_DrawImePart(int x,int y,int w,int h){ OD_ASSERT(FALSE); }
void OD_ImeSetSoftKeyLabel(OD_KEY type, const unsigned short* label){ OD_ASSERT(FALSE); }


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:52 2012*/ //IGNORE9527
