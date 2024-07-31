/*
 * Copyright 2005-2009 Myriad Group AG. All Rights Reserved.
 * $Id$
 */

#ifndef _MIDP_EVENTS_H_
#define _MIDP_EVENTS_H_

/**
 * @file
 * Interface for passing events into the midp system.
 * Events are encoded as integers, with the bottom four bits
 * defining the major event id. The remaining bits contain
 * additional information pertaining to the event.
 * The actual mechanism for delivering events is defined
 * by the cldc code.
 * @see events.h
 * @see com.sun.midp.io.EventHandler
 */

#ifdef JVM_FDAC
#include <vmcommon/events.h>
#include <jbed_static_com_sun_midp_lcdui_eventhandler.h>
#define mmEOMEvent STATIC_com_sun_midp_lcdui_EventHandler_MM_END_OF_MEDIA
#else
#define appStopKVMEvent -1
#define mmEOMEvent 0
#endif

#include <jbed_static_com_jbed_event_eventcode.h>
#include <javastatic.h>

/**
 * The major codes for the events which can be delivered to the system.
 * These are convenience aliases for values defined in java.
 */
enum KVMEventTypes {
    invalidKVMEvent         = STATIC_com_jbed_event_EventCode_EVT_NONE,
    keyDownKVMEvent         = STATIC_com_jbed_event_EventCode_EVT_KEYDOWN,
    keyUpKVMEvent           = STATIC_com_jbed_event_EventCode_EVT_KEYUP,
    keyTypedKVMEvent        = STATIC_com_jbed_event_EventCode_EVT_KEYTYPED,
    penDownKVMEvent         = STATIC_com_jbed_event_EventCode_EVT_BTNDOWN,
    penUpKVMEvent           = STATIC_com_jbed_event_EventCode_EVT_BTNUP,
    penMoveKVMEvent         = STATIC_com_jbed_event_EventCode_EVT_PTRMOVE,
    activateKVMEvent        = STATIC_com_jbed_event_EventCode_EVT_ACTIVATE,
    mediaKVMEvent           = STATIC_com_jbed_event_EventCode_EVT_MEDIA,
    killCurrentMIDletEvent  = STATIC_com_jbed_event_EventCode_EVT_KILL_CURRENT_MIDLET,
#ifdef JVM_FDAC
    netOnEvent          = STATIC_com_jbed_event_EventCode_EVT_NET_ON,
    netOffEvent         = STATIC_com_jbed_event_EventCode_EVT_NET_OFF,
#endif /* JVM_FDAC */
    incomingKVMEvent    =
        STATIC_com_jbed_event_EventCode_EVT_OTHER_CALL_INCOMING,
    startCallKVMEvent =
        STATIC_com_jbed_event_EventCode_EVT_OTHER_CALL_START,
    endCallKVMEvent =
        STATIC_com_jbed_event_EventCode_EVT_OTHER_CALL_END
};

/* FIXME: Macros ifdefed for now, reconsider unifying */
#ifdef JVM_FDAC

/**
 * Convenience macro for delivering events.
 * @param evt event encoded as an integer.
 */
#define StoreMIDPEvent(evt) StoreKVMEvent(evt)

#endif /* JVM_FDAC */

/**
 * Encode a keydown or keyup event.
 * @param evt keyDownKVMEvent or keyUpKVMEvent or keyTypedKVMEvent
 * @param key keycode of the key
 */
#define encodeMIDPKeyEvent(evt, key) \
   ( ((key) << 4) | (evt))

/**
 * Encode a pen event.
 * @param evt penDownKVMEvent or penUpKVMEvent or penMoveKVMEvent
 * @param x x co-ordinate of pen
 * @param y y co-ordinate of pen
 */
#define encodeMIDPPenEvent(evt, x, y) \
   ( ((x) << 19) | ((y) << 6) | (evt))

/**
 * Encode an activate event
 * @param active  true or false to activate or deactive the VM
 */
#define EncodeMIDPActivateEvent(active) \
   ( ((active) << 4) | activateKVMEvent)

/**
 * Encode a media event
 * @param pid
 * @param minor
 * @param mtime
 */
#define EncodeMIDPMediaEvent(pid, minor, mtime) \
   ( ((pid) << 28) | ((minor) << 24) | ((mtime) << 4) | mediaKVMEvent)

#endif /* _MIDP_EVENTS_H_ */
