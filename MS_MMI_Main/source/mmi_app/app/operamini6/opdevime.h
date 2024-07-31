/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/

#ifndef OPDEV_IME_H
#define OPDEV_IME_H

#include "optype.h"
#include "opdevkeypen.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Opera requested input type */
typedef enum
{
	OD_INPUT_TYPE_SINGLELINE =   0,
	OD_INPUT_TYPE_MULTILINE =    (1 << 0),
	OD_INPUT_TYPE_PASSWORD =     (1 << 1),
	OD_INPUT_TYPE_DIGIT =        (1 << 2),
	OD_INPUT_TYPE_EMAIL =        (1 << 3),
	OD_INPUT_TYPE_PHONENUMBER =  (1 << 4),
	OD_INPUT_TYPE_URL =          (1 << 5),
	OD_INPUT_TYPE_USERNAME =     (1 << 6),
	OD_INPUT_TYPE_UNEDITABLE =   (1 << 7),
	OD_INPUT_TYPE_SENSITIVE =    (1 << 8),
	OD_INPUT_TYPE_NON_PREDICTIVE = (1 << 9),
	OD_INPUT_TYPE_INITIAL_CAPS_WORD = (1 << 10),
	OD_INPUT_TYPE_CAPS_SENTENCE = (1 << 11),
	OD_INPUT_TYPE_MIXEDCASE =    (1 << 12),
	OD_INPUT_TYPE_LATIN =        (1 << 13),
	OD_INPUT_TYPE_ALPHA =        (1 << 14),
	OD_INPUT_TYPE_ALPHANUMERIC = (1 << 15),
} OD_InputType;

/* In editor done callback, the text must be copied before it return. */
typedef void (OD_MMI_TASK *OD_EditorDoneCallback)(OD_Context *context, const OD_WCHAR* text, int len);
typedef void (OD_MMI_TASK *OD_EditorCancelCallback)(OD_Context *context);

/* The passed-in parameters when open a editor window. */
typedef struct _OD_EditorContext
{
	/* input type, single/multiline, URL or EMAIL, password or not, etc. */
	OD_InputType input_type;
	/* max input length, < 0 means no limit. */
	int max_length;
	/* initial text showing in editor, NULL means no initial text.
	the memory of initial text won't be released until opera done/cancel callback be called. */
	const OD_WCHAR * initial_text;
	/* the length of initial text, number of WCHARs. */
	int initial_text_length;
} OD_EditorContext;

/* raw IME messages definition, will be used for certain platforms */
typedef enum _OD_IME_MESSAGE
{
	OD_IME_MSG_CLEAR_HIGHLIGHT,
	OD_IME_MSG_CLOSE_CONTEXT_MENU,
	OD_IME_MSG_CONTEXT_MENU_EXIST,
	OD_IME_MSG_DELETE_CHARACTERS,
	OD_IME_MSG_DRAW_OPERA,
	OD_IME_MSG_GET_CANDIDATE_WORDS,
	OD_IME_MSG_GET_CARET_RECT,
	OD_IME_MSG_GET_INPUT_BOX_RECT,
	OD_IME_MSG_GET_MULTITAP_STATE,
	OD_IME_MSG_GET_STRING_BEFORE_CANDIDATE,
	OD_IME_MSG_GET_STRING_BEFORE_CURSOR,
	OD_IME_MSG_GET_TEXT_LENGTH,
	OD_IME_MSG_GET_TEXT_PTR_BEFORE_CANDIDATE,
	OD_IME_MSG_GET_TEXT_PTR_BEFORE_CURSOR,
	OD_IME_MSG_HANDLE_SOFT_KEY,
	OD_IME_MSG_HAS_HIGHLIGHT,
	OD_IME_MSG_HIGHLIGHT_TEXT,
	OD_IME_MSG_INSERT_MULTITAP_STRING,
	OD_IME_MSG_INSERT_STRING,
	OD_IME_MSG_MOVE_CURSOR,
	OD_IME_MSG_MOVE_CURSOR_TO_HIGHLIGHT_START,
	OD_IME_MSG_RELAYOUT,
	OD_IME_MSG_SET_MULTITAP_STATE,
	OD_IME_MSG_STOP_CURSOR_BLINK,
	OD_IME_MSG_START_CURSOR_BLINK
} OD_IME_MESSAGE;

/* The passed-in parameters when open a editor window. */
typedef struct _OD_EditorCallbacks
{
	/* The callback called when user committed editing in editor window. */
	OD_EditorDoneCallback done_cb;
	/* The callback called when user canceled editing in editor window. */
	OD_EditorCancelCallback cancel_cb;
	/* Handle the message sent from platform IME. Only used in some platforms. */
	int (*handleImeMessage)(OD_IME_MESSAGE msg, int param0, int param1);
} OD_EditorCallbacks;

/*
* Enter full screen editor
* @param context the opera context, context->data.ime and context->callback.ime should be prepared
* @return OD_SUCCESS if succeed, otherwise OD_ERROR
*/
OD_MMI_TASK OD_STATUS OD_EnterFullScreenEditor(OD_Context *context);

/*
* Force leave full screen editor from opera
* @param context the opera context
* @return OD_SUCCESS if succeed, otherwise OD_ERROR
*/
OD_MMI_TASK OD_STATUS OD_CloseFullScreenEditor(OD_Context *context);

typedef enum
{
	OD_DOWN,
	OD_UP,
	OD_LEFT,
	OD_RIGHT
} OD_Direction;

typedef struct
{
	int x, y, w, h;
} OD_Rect;

#define OD_IME_MAX_SOFT_KEY_TEXT_LENGTH 20

OD_MMI_TASK void OD_ImeEnterScreen(OD_Context* context);
OD_MMI_TASK void OD_ImeExitScreen(OD_Context* context);

OD_MMI_TASK void OD_ImeConnect(OD_Context* context, OD_InputType input_type, const OD_WCHAR* lsk_label, const OD_WCHAR* rsk_label);
OD_MMI_TASK void OD_ImeDisconnect(OD_Context* context);

OD_MMI_TASK void OD_ImeBeforeEditorOrCaretChanged(void);
OD_MMI_TASK int OD_ImeGetImuiHeight(void);
OD_MMI_TASK void OD_DrawImePart(int x,int y,int w,int h);
OD_MMI_TASK void OD_ImeSetSoftKeyLabel(OD_KEY type, const unsigned short* label);

#ifdef __cplusplus
};
#endif

#endif /* OPDEV_IME_H */
