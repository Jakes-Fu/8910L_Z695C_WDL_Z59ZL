/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef FUCUS_OPDEV_SCHEME_H
#define FUCUS_OPDEV_SCHEME_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Container for URI Scheme handler context data.
 */
typedef struct _OD_SchemeContext
{
	/**
	 * Initialization flag, OD_TRUE if initialised,
	 * OD_FALSE otherwise.
	 */
	OD_BOOL initialized;
} OD_SchemeContext;

/**
 * Initialise the URI scheme handler.
 *
 * @param[in] context the context.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         context is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_SchemeInit(OD_Context *context);

/**
 * Deinitialise the URI scheme handler.
 *
 * @param[in] context the context.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         context is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_SchemeDeinit(OD_Context *context);

/**
 * Tries to handle a given URI with the registered scheme handlers.
 *
 * In general, the API is expected to popup a dialog or a message box
 * to ask if user would like to continue before the URI is handled
 * when the URL is for calling out, sending a message, or sending a
 * mail("tel:", "sms:"/"mms:" or "mailto:"). It's an alternative way
 * to open the composing UI for user then user can confirm or cancel.
 * If there's no API to confirm for the cases mentioned above, it
 * depends the requirement of platform/customer/device to return error
 * or handling the URI without confirming.
 *
 * @param[in] context the context.
 * @param[in] uri the URI to open, in URL encoding.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointer is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_MMI_TASK OD_STATUS OD_SchemeHandle(OD_Context *context, const char *uri);

#ifdef __cplusplus
};
#endif

#endif /* FUCUS_OPDEV_SCHEME_H */
