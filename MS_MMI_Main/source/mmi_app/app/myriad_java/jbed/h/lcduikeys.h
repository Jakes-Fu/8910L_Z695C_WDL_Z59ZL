/*
 * Copyright 2007-2008 Esmertec AG. All Rights Reserved.
 * $Id$
 */

#ifndef LCDUI_KEYS
#define LCDUI_KEYS

/* Changes to this enum must be kept in step with the Java definitions */
typedef enum {
    LCDUI_KEY_0        = 48,
    LCDUI_KEY_1        = 49,
    LCDUI_KEY_2        = 50,
    LCDUI_KEY_3        = 51,
    LCDUI_KEY_4        = 52,
    LCDUI_KEY_5        = 53,
    LCDUI_KEY_6        = 54,
    LCDUI_KEY_7        = 55,
    LCDUI_KEY_8        = 56,
    LCDUI_KEY_9        = 57,
    LCDUI_KEY_ASTERISK = 42,
    LCDUI_KEY_POUND    = 35,

#ifdef SPRINT_SUPPORT
    LCDUI_KEY_FAST_FORWARD = -86,
    LCDUI_KEY_MUTE = -87,
    LCDUI_KEY_PAUSE = -88,
    LCDUI_KEY_PLAY = -89,
    LCDUI_KEY_PLAY_PAUSE = -90,
    LCDUI_KEY_REWIND = -91,
    LCDUI_KEY_STOP = -92,
    LCDUI_KEY_TRACK_FORWARD = -93,
    LCDUI_KEY_TRACK_BACK = -94,
#endif

    LCDUI_KEY_UP       = -1,
    LCDUI_KEY_DOWN     = -2,
    LCDUI_KEY_LEFT     = -3,
    LCDUI_KEY_RIGHT    = -4,
    LCDUI_KEY_SELECT   = -5,
    LCDUI_KEY_CLEAR    = -8,

    LCDUI_KEY_SOFT1    = -6,
    LCDUI_KEY_SOFT2    = -7,

    /* these may not be available to java */
    LCDUI_KEY_SEND     = -10,
    LCDUI_KEY_END      = -11,
    LCDUI_KEY_POWER    = -12,
#ifdef SPRINT_SUPPORT
    /**
     * Sprint 3.1 requires the keycode for key_back is -8,
     * this value is duplicate with key_clear. */
    LCDUI_KEY_BACK     = LCDUI_KEY_CLEAR,
#else
    LCDUI_KEY_BACK     = -82,
#endif

#ifdef SPRINT_SUPPORT
    LCDUI_KEY_CAMERA   = -103,
    LCDUI_KEY_PTT = -95,
    LCDUI_KEY_SIDE_DOWN = -105,
    LCDUI_KEY_SIDE_UP   = -104,
    LCDUI_KEY_EXIT = -96,
    LCDUI_KEY_DELETE = -97,
    LCDUI_KEY_TOGGLE = -98,
    LCDUI_KEY_FLASH = -99,
    LCDUI_KEY_ZOOM_IN = -100,
    LCDUI_KEY_ZOOM_OUT = -101,
    LCDUI_KEY_RECORD = -102,
#endif

#ifdef FULL_KEYPAD_SUPPORT
    LCDUI_KEY_SYM = -20,
    LCDUI_KEY_CAPS = -21,
    LCDUI_KEY_FUNCTION = -22,
#endif

    LCDUI_KEY_INVALID  = 0
} LCDUIKeyType;

#endif

