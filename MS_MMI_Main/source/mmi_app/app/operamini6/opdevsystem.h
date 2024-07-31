/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_SYSTEM_H
#define OPDEV_SYSTEM_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Touch screen support flags.
 *
 * @see OD_GetSystemProperty
 */
typedef enum _OD_TOUCH_SUPPORT_TYPE
{
	/** No touch screen support. */
	OD_TOUCH_SUPPORT_NONE       = 0,

	/** Press events reported. */
	OD_TOUCH_SUPPORT_PRESS      = (1 << 0),

	/** Release events reported. */
	OD_TOUCH_SUPPORT_RELEASE    = (1 << 1),

	/** Drag events reported. */
	OD_TOUCH_SUPPORT_MOVE       = (1 << 2),

	/** Multitouch supported. */
	OD_TOUCH_SUPPORT_MULTITOUCH = (1 << 3),
} OD_TOUCH_SUPPORT_TYPE;

/**
 * Keypad support flags.
 *
 * @see OD_GetSystemProperty
 */
typedef enum _OD_KEYPAD_SUPPORT_TYPE
{
	/** No keypad present. */
	OD_KEYPAD_SUPPORT_NONE      = 0,

	/** Normal ITU.T 12-keys pad. */
	OD_KEYPAD_SUPPORT_NORMAL    = (1 << 0),

	/** Function keys plus four way rocker. */
	OD_KEYPAD_SUPPORT_4WAY_ONLY = (1 << 1),

	/** Full QWERTY keypad. */
	OD_KEYPAD_SUPPORT_QWERTY    = (1 << 2),
} OD_KEYPAD_SUPPORT_TYPE;

/**
 * Physical key support flags
 *
 * @see OD_GetSystemProperty
 */
typedef enum _OD_PHYSICALKEY_SUPPORT_TYPE
{
	OD_PHYSICALKEY_SUPPORT_BACKSPACE		= 1,

	OD_PHYSICALKEY_SUPPORT_DELETE			= (1 << 1),

	OD_PHYSICALKEY_SUPPORT_BACK				= (1 << 2),

	OD_PHYSICALKEY_SUPPORT_MENU				= (1 << 3),

	OD_PHYSICALKEY_SUPPORT_SOFT_KEYS		= (1 << 4),

	OD_PHYSICALKEY_SUPPORT_DIRECTION_KEYS	= (1 << 5),

	OD_PHYSICALKEY_SUPPORT_FORWARD			= (1 << 6)

} OD_PHYSICALKEY_SUPPORT_TYPE;

/**
* Native input support flags
*
* @see OD_GetSystemProperty
*/
typedef enum _OD_NATIVE_INPUT_SUPPORT_TYPE
{
	/* support full-screen input */
	OD_NATIVE_INPUT_SUPPORT_FULLSCREEN = 1,

	/* support a native box overlap on bream box. */
	OD_NATIVE_INPUT_SUPPORT_INLINE_BOX_OVERLAP = (1 << 1),

	/* support raw IME, editor will communicate with input method engine. */
	OD_NATIVE_INPUT_SUPPORT_RAW_IME = (1 << 2)

} OD_NATIVE_INPUT_SUPPORT_TYPE;

/**
 * Available properties that can be queried by #OD_GetSystemProperty.
 */
typedef enum _OD_SYSTEM_PROPERTY
{
	/**
	 * Touchscreen support.
	 *
	 * Querying this returns a combination of flags from the
	 * OD_TOUCH_SUPPORT_TYPE enumeration.
	 *
	 * @see OD_TOUCH_SUPPORT_TYPE
	 */
	OD_TOUCH_SUPPORT,

	/**
	 * Keypad support.
	 *
	 * Querying this returns a combination of flags from the
	 * OD_KEYPAD_SUPPORT_TYPE enumeration.
	 *
	 * @see OD_KEYPAD_SUPPORT_TYPE
	 */
	OD_KEYPAD_SUPPORT,

	/**
	 * Physical key support.
	 *
	 * Querying this returns a combination of flags from the 
	 * OD_PHYSICALKEY_SUPPORT_TYPE enumeration.
	 *
	 * @see OD_PHYSICALKEY_SUPPORT_TYPE
	 */
	OD_PHYSICALKEY_SUPPORT,

	/**
	 * Native input support.
	 *
	 * Querying this returns a combination of flags from the 
	 * OD_NATIVE_INPUT_SUPPORT_TYPE enumeration.
	 *
	 * @see OD_NATIVE_INPUT_SUPPORT_TYPE
	 */
	OD_NATIVE_INPUT_SUPPORT,

	/** Clipboard present. */
	OD_CLIPBOARD_SUPPORT,

	/** Vibration available. */
	OD_VIBRATION_SUPPORT,

	/** MMS support. */
	OD_MMS_SUPPORT,

	/** Bluetooth present. */
	OD_BLUETOOTH_SUPPORT,

	/** G-Sensor for screen rotation present. */
	OD_SCREEN_ROTATION_SUPPORT,

	/** GPS/AGPS support present. */
	OD_GEOLOCATION_SUPPORT,

	/** MAILTO support present. */
	OD_MAILTO_SUPPORT
} OD_SYSTEM_PROPERTY_TYPE;

/**
 * Container for System context data.
 */
typedef struct _OD_SystemContext
{
	/** Touch screen support. */
	OD_TOUCH_SUPPORT_TYPE touch_type;

	/** Keypad screen support. */
	OD_KEYPAD_SUPPORT_TYPE keypad_type;
} OD_SystemContext;

/**
 * Container for the System callbacks.
 */
typedef struct _OD_SystemCallbackContext
{
	/**
	 * Callback for the IMEI retrieval function.
	 *
	 * @param[in] status the operation result.
	 * @param[in] imei a pointer to the IMEI string.
	 * @param[in] length the length of the IMEI string data.
	 * 
	 * @return an appropriate status code from the OD_Error
	 * enumeration.
	 */
	OD_STATUS (*getImei)(OD_STATUS status, char *imei, int length);

	/**
	 * Callback for the IMSI retrieval function.
	 *
	 * @param[in] status the operation result.
	 * @param[in] imsi a pointer to the IMSI string.
	 * @param[in] length the length of the IMSI string data.
	 * 
	 * @return an appropriate status code from the OD_Error
	 * enumeration.
	 */
	OD_STATUS (*getImsi)(OD_STATUS status, char *imsi, int length);
} OD_SystemCallbacks;

/**
 * Initialises the system support code.
 *
 * @param[in] context the context.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointer is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_SystemInit(OD_Context *context);

/**
 * Deinitialises the system support code.
 *
 * @param[in] context the context.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointer is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_SystemDeinit(OD_Context *context);

/************************************************************************/
/* Clipboard                                                            */
/************************************************************************/

/**
* Only Unicode text is supported.
*/
typedef enum
{
	/* Unicode text */
    OD_CLIPBOARD_FORMAT_TEXT_UNICODE,

    OD_CLIPBOARD_FORMAT_MAX
} OD_CLIPBOARD_FORMAT;

/**
 * Sets the system clipboard contents to the given data in given format.
 *
 * @param[in] context the context.
 * @param[in] format the format of data.
 * @param[in] data the data to set.
 * @param[in] len the data length, in bytes.
 *
 * @note for text, data don't need to have tailing '\0', len shouldn't include length of training '\0'.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointer is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_ClipboardSetData(OD_Context *context, OD_CLIPBOARD_FORMAT format, void* data, int len);

/**
 * Retrieves the system clipboard contents in given format.
 *
 * @warn NO checks are done on the input pointers.  An assertion will
 *       trigger in debug mode but it <b>WILL</b> crash in release mode
 *       if a NULL pointer is being passed.
 *
 * @param[in] context the context.
 * @param[in] format the foramt of the requested data.
 * @param[out] data a pointer to the requested data.
 * @param[out] len a pointer to the data length, in bytes.
 *
 * @note for text, *data don't have tailing '\0', *len don't include the length of tailing '\0'.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointer is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_ClipboardGetData(OD_Context *context, OD_CLIPBOARD_FORMAT format, void** data, int* len); 

/**
* See if there is data in clipboard in given format.
*
* @param[in] context the context.
* @param[in] format the format of the requested data.
* @param[out] hasData a pointer to the value indicating if there is data.
*
* @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
*         any pointer is NULL, or the appropriate error code from the
*         OD_Error enumeration.
*/
OD_MMI_TASK OD_STATUS OD_ClipboardHasData(OD_Context *context, OD_CLIPBOARD_FORMAT format, OD_BOOL* hasData);


/************************************************************************/
/* Time                                                                 */
/************************************************************************/

/**
 * Returns the current system tick counter from a specific point in time.
 *
 * The point in time taken as reference may either be the UNIX Epoch
 * timestamp (19700101) or the system boot time.
 *
 * @warn NO checks are is done on the input context.  An assertion will
 *       trigger in debug mode if a NULL pointer is being passed.
 *
 * @param[in] context the context.
 *
 * @return the time difference from the reference timestamp, in
 *         milliseconds.
 */
OD_ALL_TASK unsigned int OD_GetTickCount(void);

/**
 * Returns the current time from the UNIX Epoch (as in UTC timezone).
 *
 * @warn NO checks are is done on the input context.  An assertion will
 *       trigger in debug mode if a NULL pointer is being passed.
 *
 * @param[in] context the context.
 *
 * @return the time difference from the UNIX Epoch, in seconds.
 */
OD_ALL_TASK unsigned int OD_GetUTCTime(OD_Context *context);

/**
 * Gets the current time of the day in local timezone.
 *
 * @warn NO checks are done on the input pointers.  An assertion will
 *       trigger in debug mode but it <b>WILL</b> crash in release mode
 *       if a NULL pointer is being passed.
 *
 * @param[in] context the context.
 * @param[out] h a pointer to an integer that will contain the hours
 *             value (from 0 to 23).
 * @param[out] m a pointer to an integer that will contain the minutes
 *             value (from 0 to 59).
 * @param[out] s a pointer to an integer that will contain the seconds
 *             value (from 0 to 23).
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointer is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_GetLocalTime(OD_Context *context, int *h, int *m, int *s);

/**
 * Returns the timezone difference from UTC in seconds.
 *
 * @param[in] context the context.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         context is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_MMI_TASK int OD_GetTimeZone(OD_Context *context);

/**
 * Queries the system for subsystem support properties.
 *
 * @see OD_TOUCH_SUPPORT_TYPE
 * @see OD_KEYPAD_SUPPORT_TYPE
 * @see OD_SYSTEM_PROPERTY_TYPE
 *
 * @param[in] context the context.
 * @param[in] property the property to query.
 * @param[out] value a pointer to the property value container.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointer is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_GetSystemProperty(OD_Context *context, OD_SYSTEM_PROPERTY_TYPE property, int *value);

/**
 * Returns the RFC-5646 code of the language the device is currently
 * set in.
 *
 * @param[in] context the context.
 * @param[in] buf the buffer that will contain the language code.
 * @param[in] buf_size the maximum length for the string.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointer is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_GetLangCountryCode(OD_Context *context, char *buf, int buf_size);

/**
* Returns the platform name, like "Nokia/N95".
*
* @param[in] context the context.
* @param[in] buf the buffer that will contain the platform name.
* @param[in] buf_size the maximum length for the string, suggested larger than 32.
*
* @return OD_SUCCESS if the operation succeeded, or the appropriate 
*         error code from the OD_Error enumeration.
*/
OD_ALL_TASK OD_STATUS OD_GetPlatformName(OD_Context *context, char *buf, int buf_size);

/**
 * The IMEI container.
 */
typedef struct 
{
	/** IMEI data, 15 bytes + 2 for SVN. */
	char data[30];
} OD_IMEI;

/**
 * Fetches the IMEI for the device.
 *
 * If imei is set to NULL, it means the IMEI retrieval will happen
 * asynchronously, and for that to succed, the getImei function pointer in
 * the system callbacks structure must be set accordingly.
 *
 * @param[in] context the context.
 * @param[in] imei a pointer to the OD_IMEI container or NULL for
 *            asynchronous retrieval.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         context is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_GetIMEI(OD_Context *context, OD_IMEI *imei);

/**
 * The IMSI container.
 */
typedef struct
{
	/** IMSI data, 15 bytes or 18 bytes for PURE IMSI. */
	char data[30];
} OD_IMSI;

/**
 * Fetches the IMSI for the device.
 *
 * If imsi is set to NULL, it means the IMSI retrieval will happen
 * asynchronously, and for that to succed, the getImsi function pointer in
 * the system callbacks structure must be set accordingly.
 *
 * @param[in] context the context.
 * @param[in] imsi a pointer to the OD_IMSI container or NULL for
 *            asynchronous retrieval.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         context is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_GetIMSI(OD_Context *context, OD_IMSI *imsi);

/************************************************************************/
/* Timer                                                                */
/************************************************************************/

/**
* Create a timer. The timer can start/stop many times.
*
* @param[in] task the task id who requests the timer.
* @param[out] timer will be assigned as the pointer to created timer if succeed.
* @param[in] callback pointer to the callback function when the timer timeout.
* @param[in] delay the delay time of timeout, in milliseconds.
* @param[in] userData will be passed as parameter into timer callback.
*
* @return OD_SUCCESS if succeed, or the appropriate error code from 
* the OD_Error enumeration.
*/
OD_ALL_TASK OD_STATUS OD_TimerCreate(OD_Context *context, OD_TaskID task, OD_TimerID* timer, OD_TimerArg userdata);

/**
 * Destroys a timer.
 *
 * If a timer is not needed anymore it must be destroyed via
 * OD_TimerDestroy.  This is an important step as on certain platforms the
 * number of available timers is very limited.  If OD_TimerDestroy is
 * called on a timer that's currently scheduled, it will be stopped first
 * but the callback will NOT be invoked.
 *
 * @see OD_TimerCreate
 * @see OD_TimerStop
 *
 * @param[in] context the context.
 * @param[in] task the task requesting the timer.
 * @param[in] timer the timer ID to destroy.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         context is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_TimerDestroy(OD_Context *context, OD_TaskID task, OD_TimerID timer);

/**
 * Schedules a timer.
 *
 * This schedules the given timer ID to trigger after the given amount of
 * milliseconds.  It is allowed to reschedule a previously created timer,
 * even with a different delay.
 *
 * @param[in] context the context.
 * @param[in] task the task requesting the timer.
 * @param[in] timer the timer ID for the timer.
 * @param[in] callback the callback to call when the timer triggers.
 * @param[in] delay the amount of time to wait, in milliseconds.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         context is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_TimerStart(OD_Context *context, OD_TaskID task, OD_TimerID timer, OD_TIMERFUN callback, int delay, OD_TimerArg userdata);

/**
 * Stops a scheduled timer.
 *
 * If a scheduled timer is stopped the associated callback will NOT be
 * invoked.
 *
 * @param[in] context the context.
 * @param[in] task the task requesting the timer.
 * @param[in] timer the timer ID for the timer.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         context is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_TimerStop(OD_Context *context, OD_TaskID task, OD_TimerID timer);

/**
 * Checks whether a given timer has been scheduled.
 *
 * @param[in] context the context.
 * @param[in] task the task requesting the timer.
 * @param[in] timer the timer ID for the timer.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         context is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_BOOL OD_IsTimerStarted(OD_Context *context, OD_TaskID task, OD_TimerID timer);


/*call platform to open document;
 *@param[in] path, the path to mimetype document
 *@param[in] mime, mime type.
 *@return OD_SUCCESS if operation is successful, OD_ERROR if mime type is not suport,
 *        OD_NULL_POINTER either mime or path is null
 *        OD_NO_MEMORY if there's no enough memory for opening the document.
 */
OD_MMI_TASK OD_STATUS OD_PlatformOpenDocument(OD_Context *context, const OD_FileName *path, const char *mimetype);

/*
 * Query if mime type document can be opened by platform.
 * @return OD_TRUE if support, else return OD_FALSE
 */
OD_MMI_TASK OD_BOOL OD_PlatformCanOpenDocument(OD_Context *context, const char * mimeType);

#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /* OPDEV_SYSTEM_H */
