/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/

#ifndef _FUCUS_KEYPEN_TRACKER_H_
#define _FUCUS_KEYPEN_TRACKER_H_

#include "optype.h"
#include "opdevkeypen.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* KeyPenTracker: a object to make key/pen event in pair.
* It will
* (1) In KeyPenTracker_handleEvent: Ignore UP/MOVE event if we haven't receive DOWN event.
* (2) In KeyPenTracker_resetStatus: Send UP event to callback if we are in DOWN state.
* Usage:
* (1) Replace real key/pen event handler with KeyPenTracker_handleEvent,
*   set the original handler as callback of KeyPenTracker.
* (2) If now is in key/pen DOWN state, but we won't receive UP event, use KeyPenTracker_resetStatus
*
* Notes:
* To make things simple, limit all functions in a task(MMI).
*/

typedef enum _KeyPenTrackerType
{
	KEYPENTRACKER_TYPE_KEY,
	KEYPENTRACKER_TYPE_PEN,
} KeyPenTrackerType;

typedef enum _KeyPenTrackerEventType
{
	KEYPENTRACKER_EVENT_DOWN,
	KEYPENTRACKER_EVENT_UP,
	KEYPENTRACKER_EVENT_MOVE,
	KEYPENTRACKER_EVENT_INVALID,
} KeyPenTrackerEventType;

typedef union _KeyPenTrackerEvent
{
	struct
	{
		OD_PENEVENT type;
		int x;
		int y;
	} pen;
	struct
	{
		OD_KEYEVENT type;
		OD_KEY keyCode;
	} key;
} KeyPenTrackerEvent;

typedef OD_STATUS (*KeyPenTrackerCallback)(OD_Context* context, KeyPenTrackerEvent* event);

typedef struct _KeyPenTracker
{
	KeyPenTrackerType type; /* Type of tracker, key or pen */
	OD_BOOL pressed;	/* If in pressed state */
	OD_BOOL needReset;	/* If state need to be reset */
	KeyPenTrackerEvent lastDownEvent;	/* The last down event */
	OD_BOOL callbackReenterable;	/* If callback can be reentered */
	KeyPenTrackerCallback callback; /* The callback being called when paired key/pen event coming */
} KeyPenTracker;

/*
* Init KeyPenTracker.
* @param context: the context
* @param tracker: the tracker
* @param type: the type of the tracker, track pen or key event
* @param callback: the real key/pen handler
* @param callback: if the callback can be reentered.
*
* @return OD_SUCCESS if ok, otherwise OD_ERROR
*/
OD_MMI_TASK OD_STATUS KeyPenTracker_init(OD_Context* context, KeyPenTracker* tracker, KeyPenTrackerType type, KeyPenTrackerCallback callback, OD_BOOL callbackReenterable);

OD_MMI_TASK void KeyPenTracker_destroy(OD_Context* context, KeyPenTracker* tracker);

/*
* Reset key/pen status. Set key/pen to unpressed state.
* @param context, be used to check if tracker is safe to access in KeyPenTracker_asyncCallback.
* @param tracker: the tracker
*/
OD_MMI_TASK void KeyPenTracker_resetStatus(OD_Context* context, KeyPenTracker* tracker);

/*
* Handle key/pen event.
* @param context: the context
* @param tracker: the tracker
* @param event: a key or pen event, the type of the event is depended on the type of tracker.
*
* @return OD_SUCCESS if ok, otherwise OD_ERROR
*/
OD_MMI_TASK OD_STATUS KeyPenTracker_handleEvent(OD_Context* context, KeyPenTracker* tracker, KeyPenTrackerEvent* event);

#ifdef __cplusplus
};
#endif

#endif /* _FUCUS_KEYPEN_TRACKER_H_ */
