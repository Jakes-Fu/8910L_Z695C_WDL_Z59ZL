/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_OPDEV_INTERNAL_H
#define OPDEV_OPDEV_INTERNAL_H

/* Always include "OpType.h" at the beggining of the file. */
#include "optype.h"

/* INTERNAL header for renaming Opera and OpDev APIs */
#include "operafuncrename.h"

/* INTERNAL Opera API headers to be used for exported PUBLIC APIs */
#include "operaminiapi.h"

/* INTERNAL OpDev classified headers */
#include "opdevbookmarkimport.h"
#include "opdevdebug.h"
#include "opdevdownload.h"
#include "opdevfile.h"
#include "opdevfont.h"
#include "opdevgdi.h"
#include "opdevime.h"
#include "opdevkeypen.h"
#include "opdevkeypentracker.h"
#include "opdevmain.h"
#include "opdevmedia.h"
#include "opdevmemory.h"
#include "opdevnetwork.h"
#include "opdevresource.h"
#include "opdevscheme.h"
#include "opdevsocket.h"
#include "opdevsystem.h"
#include "opdevthread.h"
#include "opdevutil.h"

struct _OD_Context
{
	struct _OD_ContextData
	{
		OD_MemoryContext memory;
		OD_GdiContext gdi;
		OD_NetContext net;
		OD_EditorContext ime;
		OD_ThreadContext thread;
		OD_AppContext app;
		OD_SchemeContext scheme;
		OD_FileContext file;
		OD_SystemContext system;
		KeyPenTracker keyTracker;
		KeyPenTracker penTracker;
	} data;

	struct _OD_ContextCallback
	{
		OD_AppCallbacks app;
		OD_NetCallbacks net;
		OD_EditorCallbacks ime;
		OD_SystemCallbacks system;
		OD_DownloadCallbacks download;
	} callback;

	OD_BOOL initialized;
};
typedef struct _OD_Context OD_Context;

#define IsValidContext(context)		((context) != NULL && (context)->initialized)

#endif /* OPDEV_OPDEV_INTERNAL_H */
