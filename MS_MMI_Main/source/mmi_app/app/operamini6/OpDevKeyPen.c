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

OD_KEY OD_TranslateKeyCode(int plaform_key)
{
	switch(plaform_key)
	{
	case KEY_0:
		return OD_KEY_0;
	case KEY_1:
		return OD_KEY_1;
	case KEY_2:
		return OD_KEY_2;
	case KEY_3:
		return OD_KEY_3;
	case KEY_4:
		return OD_KEY_4;
	case KEY_5:
		return OD_KEY_5;
	case KEY_6:
		return OD_KEY_6;
	case KEY_7:
		return OD_KEY_7;
	case KEY_8:
		return OD_KEY_8;
	case KEY_9:
		return OD_KEY_9;
	case KEY_OK:		/* Spreadtrum LSK */
		return OD_KEY_LSK;
	case KEY_WEB:		/* Spreadtrum CSK */
		return OD_KEY_CSK;
	case KEY_CANCEL:	/* Spreadtrum RSK */
#ifdef MMI_PDA_SUPPORT
		/*
		* 6804 device have hardware back key, but in code it is KEY_CANCEL 
		* And customer refuse to define it as KEY_BACK.
		*/
		return OD_KEY_BACK;
#else /* def MMI_PDA_SUPPORT */
		return OD_KEY_RSK;
#endif /* ndef MMI_PDA_SUPPORT */
	case KEY_ENTER:		/* Qwerty enter key. */
		return OD_KEY_ENTER;
	case KEY_UP:
		return OD_KEY_UP_ARROW;
	case KEY_DOWN:
		return OD_KEY_DOWN_ARROW;
	case KEY_LEFT:
		return OD_KEY_LEFT_ARROW;
	case KEY_RIGHT:
		return OD_KEY_RIGHT_ARROW;
	case KEY_STAR:
		return OD_KEY_STAR;
	case KEY_HASH:
		return OD_KEY_POUND;
	case KEY_DEL:
		return OD_KEY_CLEAR;
	case KEY_BACKWARD:
		return OD_KEY_BACK;
#ifdef KEY_MENU	/* Compatible with 6600 and old SDK. */
	case KEY_MENU:
		return OD_KEY_MENU;
#endif /* KEY_MENU */
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD /* Support for qwerty keypad */
	case KEY_M:
		return OD_KEY_0;
	case KEY_R:
		return OD_KEY_1;
	case KEY_T:
		return OD_KEY_2;
	case KEY_Y:
		return OD_KEY_3;
	case KEY_F:
		return OD_KEY_4;
	case KEY_G:
		return OD_KEY_5;
	case KEY_H:
		return OD_KEY_6;
	case KEY_V:
		return OD_KEY_7;
	case KEY_B:
		return OD_KEY_8;
	case KEY_N:
		return OD_KEY_9;
	case KEY_U:
		return OD_KEY_STAR;
	case KEY_J:
		return OD_KEY_POUND;
#endif
	}
	return OD_KEY_INVALID;  //an invalid key
}

OD_PENEVENT OD_TranslatePenEvent(int platform_pen_evt)
{
	OD_PENEVENT pen_event = OD_PENEVENT_NONE;
	switch(platform_pen_evt)
	{
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_DOWN:
		pen_event = OD_PENEVENT_DOWN;
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_UP:
		pen_event = OD_PENEVENT_UP;
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_MOVE:
		pen_event = OD_PENEVENT_MOVE;
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	}
	return pen_event;
}


/*Edit by script, ignore 3 case. Thu Apr 26 19:01:02 2012*/ //IGNORE9527
