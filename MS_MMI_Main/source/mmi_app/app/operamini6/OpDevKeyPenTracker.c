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

static KeyPenTrackerEventType KeyPenTracker_translateFromKeyEvent(OD_KEYEVENT keyEvent);
static KeyPenTrackerEventType KeyPenTracker_translateFromPenEvent(OD_PENEVENT penEvent);
static void KeyPenTracker_doResetStatus(OD_Context* context, KeyPenTracker* tracker, OD_BOOL synchronous);
static OD_STATUS KeyPenTracker_callback(OD_Context* context, KeyPenTracker* tracker, KeyPenTrackerEvent* event);

OD_STATUS KeyPenTracker_init(OD_Context* context, KeyPenTracker* tracker, KeyPenTrackerType type, KeyPenTrackerCallback callback, OD_BOOL callbackReenterable)
{
	OD_ASSERT(tracker);
	OD_ASSERT(type == KEYPENTRACKER_TYPE_KEY || type == KEYPENTRACKER_TYPE_PEN);
	OD_ASSERT(callback);

	tracker->type = type;
	tracker->callback = callback;
	tracker->callbackReenterable = callbackReenterable;

	tracker->pressed = OD_FALSE;
	tracker->needReset = OD_FALSE;
	return OD_SUCCESS;
}

void KeyPenTracker_destroy(OD_Context* context, KeyPenTracker* tracker)
{
	od_memset(tracker, 0, sizeof(KeyPenTracker));
}

static void KeyPenTracker_asyncCallback(OD_AsyncCall* call)
{
	OD_Context* context = (OD_Context*)call->req.param;
	if (context && IsValidContext(context))
	{
		if (context->data.keyTracker.needReset)
		{
			KeyPenTracker_doResetStatus(context, &context->data.keyTracker, OD_TRUE);
		}
		if (context->data.penTracker.needReset)
		{
			KeyPenTracker_doResetStatus(context, &context->data.penTracker, OD_TRUE);
		}
	}
}

void KeyPenTracker_doResetStatus(OD_Context* context, KeyPenTracker* tracker, OD_BOOL synchronous)
{
	if (tracker->needReset && tracker->pressed)
	{
		if (!synchronous)
		{
			OD_AsyncCall call = {0};
			call.req.call = KeyPenTracker_asyncCallback;
			call.req.param = context;
			call.req.receiver = OD_GetCurrentTaskID();

			OD_PostCall(&call);
		}
		else
		{
			/* Send a missing up event. */
			KeyPenTrackerEvent event = tracker->lastDownEvent;
			if (tracker->type == KEYPENTRACKER_TYPE_KEY)
			{
				event.key.type = OD_KEYEVENT_UP;
			}
			else
			{
				event.pen.type = OD_PENEVENT_UP;
			}
			tracker->needReset = OD_FALSE;
			KeyPenTracker_handleEvent(context, tracker, &event);
		}
	}
	else
	{
		tracker->needReset = OD_FALSE;
	}
}

void KeyPenTracker_resetStatus(OD_Context* context, KeyPenTracker* tracker)
{
	tracker->needReset = OD_TRUE;
	/* If the callback can be reentered, we do reset synchronously, otherwise asynchronously. Because we don't 
	know when this function is called, maybe it run inside of the callback. */
	KeyPenTracker_doResetStatus(context, tracker, tracker->callbackReenterable);
}

OD_STATUS KeyPenTracker_callback(OD_Context* context, KeyPenTracker* tracker, KeyPenTrackerEvent* event)
{
	OD_STATUS ret = OD_SUCCESS;
	OD_ASSERT(tracker->callback);
	ret = tracker->callback(context, event);
	return ret;
}

OD_STATUS KeyPenTracker_handleEvent(OD_Context* context, KeyPenTracker* tracker, KeyPenTrackerEvent* event)
{
	OD_STATUS ret = OD_ERROR;
	KeyPenTrackerEventType kptEvent = tracker->type == KEYPENTRACKER_TYPE_KEY
		? KeyPenTracker_translateFromKeyEvent(event->key.type)
		: KeyPenTracker_translateFromPenEvent(event->pen.type);

	if (tracker->needReset)
	{
		if (kptEvent == KEYPENTRACKER_EVENT_DOWN)
		{
			KeyPenTracker_doResetStatus(context, tracker, OD_TRUE);
		}
		else
		{
			/* Shouldn't receive UP/MOVE event here. */
			return OD_ERROR;
		}
	}

	if (kptEvent == KEYPENTRACKER_EVENT_DOWN)
	{
		/* Some platforms(eg. Windows) send many KEY_DOWN event instead of KEY_REPEAT. 
		* We need to ignore the following KEY_DOWNs.
		*/
		if (!tracker->pressed)
		{
			tracker->pressed = OD_TRUE;
			tracker->lastDownEvent = *event;
			ret = KeyPenTracker_callback(context, tracker, event);
		}
	}
	else if (kptEvent == KEYPENTRACKER_EVENT_UP)
	{
		/* Ignore the single UP event. If pressed, reset to unpressed state. */
		if (tracker->pressed)
		{
			tracker->pressed = OD_FALSE;
			ret = KeyPenTracker_callback(context, tracker, event);
		}
	}
	else if (kptEvent == KEYPENTRACKER_EVENT_MOVE)
	{
		/* Ignore the single MOVE event.*/
		if (tracker->pressed)
		{
			ret = KeyPenTracker_callback(context, tracker, event);
		}
	}
	else
	{
		OD_TRACE_LOW("Error. Keypen. Invalid event in KeyPenTracker_handleEvent");
	}
	return ret;
}

KeyPenTrackerEventType KeyPenTracker_translateFromKeyEvent(OD_KEYEVENT keyEvent)
{
	switch(keyEvent)
	{
	case OD_KEYEVENT_UP:
		return KEYPENTRACKER_EVENT_UP;
	case OD_KEYEVENT_DOWN:
		return KEYPENTRACKER_EVENT_DOWN;
	}
	return KEYPENTRACKER_EVENT_INVALID;
}

KeyPenTrackerEventType KeyPenTracker_translateFromPenEvent(OD_PENEVENT penEvent)
{
	switch(penEvent)
	{
	case OD_PENEVENT_UP:
		return KEYPENTRACKER_EVENT_UP;
	case OD_PENEVENT_DOWN:
		return KEYPENTRACKER_EVENT_DOWN;
	case OD_PENEVENT_MOVE:
		return KEYPENTRACKER_EVENT_MOVE;
	}
	return KEYPENTRACKER_EVENT_INVALID;
}
