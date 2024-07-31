/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_KEYPEN_H
#define OPDEV_KEYPEN_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Pen events.
 */
typedef enum _OD_PENEVENT
{
	/** Pen down. */
	OD_PENEVENT_DOWN = 1,

	/** Pen up. */
	OD_PENEVENT_UP,

	/** Pen down, pause, pen up. */
	OD_PENEVENT_LONG_TAP,

	/** Continuous pen down. */
	OD_PENEVENT_REPEAT,

	/** Pen drag. */
	OD_PENEVENT_MOVE,

	/** Pen event aborted. */
	OD_PENEVENT_ABORT,

	/** Pen down, pen up, pen down, pen up. */
	OD_PENEVENT_DOUBLE_CLICK,

	/** No pen event. */
	OD_PENEVENT_NONE,

	/** Invalid pen event. */
	OD_PENEVENT_INVALID,

	/** Sentinel. */
	OD_PENEVENT_TYPE_MAX
} OD_PENEVENT;

/**
 * Pen events.
 */
typedef enum _OD_KEYEVENT
{
	/** Key down. */
	OD_KEYEVENT_DOWN = 1,

	/** Key up. */
	OD_KEYEVENT_UP,

	/* Key long press. */
	OD_KEYEVENT_LONG_PRESS,

	/** Sentinel. */
	OD_KEYEVENT_MAX
} OD_KEYEVENT;

/**
 * Key mapping.
 *
 * The key code of printable ASCII characters can be reused directly. For instance, all letters.
 * The other keys will be defined in Unicode private use area. For instance, OD_KEY_LSK.
 */
typedef enum tag_OD_KEY
{
	/**
	 * ASCII keys (0x0~0x7F)
	 */

	/** Clear/Backsapce. */
	OD_KEY_CLEAR = '\b', /* not printable, but just reuse the key code */

	/** Enter. */
	OD_KEY_ENTER = '\n',

	/** Digit 0~9. */
	OD_KEY_0 = '0',
	OD_KEY_1,
	OD_KEY_2,
	OD_KEY_3,
	OD_KEY_4,
	OD_KEY_5,
	OD_KEY_6,
	OD_KEY_7,
	OD_KEY_8,
	OD_KEY_9,

	/**
	 * non-ASCII keys (unicode private use area: 0xE000~0xF8FF)
	 */
	OD_KEY_PRIVATE_BEGIN = 0xE000,

	/** Left softkey. */
	OD_KEY_LSK,

	/** Right softkey. */
	OD_KEY_RSK,

	/** Centre softkey. */
	OD_KEY_CSK, /* aka. OK */

	/** Rocker up. */
	OD_KEY_UP_ARROW,

	/** Rocker down. */
	OD_KEY_DOWN_ARROW,

	/** Rocker left. */
	OD_KEY_LEFT_ARROW,

	/** Rocker right. */
	OD_KEY_RIGHT_ARROW,

	/** Keypad \* */
	OD_KEY_STAR,

	/** Keypad \# */
	OD_KEY_POUND,

	/** Back. */
	OD_KEY_BACK, /* different from Clear/Backspace */

	/** Forward. */
	OD_KEY_FORWARD,

	/** Menu. */
	OD_KEY_MENU,

	/** Delete. */
	OD_KEY_DEL, /* delete key, different from Back or Clear/Backspace */

	/** End/Hang up. */
	OD_KEY_END,

	/** Search. */
	OD_KEY_SEARCH,

	/** Cut. */
	OD_KEY_CUT,

	/** Paste. */
	OD_KEY_COPY,

	/** Paste. */
	OD_KEY_PASTE,

	/** Volume up. */
	OD_KEY_VOLUME_UP,

	/** Volume down. */
	OD_KEY_VOLUME_DOWN,

	/** Invalid key. */
	OD_KEY_INVALID,

	/** Sentinel. */
	OD_KEY_MAX = (int)0x7FFFFFFF
} OD_KEY;


OD_ALL_TASK OD_KEY OD_TranslateKeyCode(int platform_key);

OD_ALL_TASK OD_PENEVENT OD_TranslatePenEvent(int platform_pen_evt);


#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /* OPDEV_KEYPEN_H */
