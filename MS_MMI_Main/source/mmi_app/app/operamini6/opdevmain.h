/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/

#ifndef OPDEV_MAIN_H
#define OPDEV_MAIN_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _OD_SocketEvent;
typedef void (*OD_START_CALLBACK)(void*, int);
typedef void (*OD_STOP_CALLBACK)(OD_Context*);
typedef struct _OD_AppCallbackContext
{
	OD_STATUS (*startApp)(void*, OD_START_CALLBACK, void*);
	OD_STATUS (*stopApp)(OD_STOP_CALLBACK);
	OD_STATUS (*enterScreenCallback)(void);
	OD_STATUS (*exitScreenCallback)(void);
	OD_STATUS (*handlePenEvent)(OD_PENEVENT penEvent, int x, int y);
	OD_STATUS (*handleKeyEvent)(OD_KEYEVENT keyEvent, OD_KEY key);
	OD_STATUS (*handleSocketEvent)(struct _OD_SocketEvent *socketEvent);

	/* 
	 * When exit opera, exit ui first, then exit the core
	 * in this uiExitCallback.
	 */
	OD_STATUS (*uiExitCallback)(void);

	/* 
	 * Called after screen rotated, use OD_GetLayerDimension() 
	 * to get current screen size.
	 */
	OD_STATUS (*screenRotatedCallback)(void);
} OD_AppCallbacks;

OD_MMI_TASK void OD_ExitUI(OD_Context *context);

OD_MMI_TASK OD_STATUS OD_InitApp(OD_Context *context);

OD_MMI_TASK OD_STATUS OD_DeinitApp(OD_Context *context);

typedef struct _OD_AppContext
{
	unsigned int module_id;
	unsigned int win_id;
	unsigned int group_id;		/* group ID in platform which have screen-group mechanism. */

	unsigned int bream_timer_id;	/* timer ID for bream task used in certain platform. */

	void *entry_arg;            /* Argument to main (entry) */
} OD_AppContext;

#ifdef __cplusplus
};
#endif

#endif /* OPDEV_MAIN_H */
