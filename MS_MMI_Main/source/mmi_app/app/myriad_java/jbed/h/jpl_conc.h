/*
 * Copyright 1998-2006 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Concurrency support (FBCC only)
 */

#ifndef __JBED_JPL_CONC_H__
#define __JBED_JPL_CONC_H__

#include <jbed_jbni.h>

#include <javastatic.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Constant to indicate that the user wants
 * to see the AMS.
 */
#define JPL_CONC_REQUEST_AMS STATIC_com_jbed_ams_AmsCallHandler_JBED_AMS_EVENT_SHOW_AMS

/**
 * Constant to indicate that the user wants
 * to see the Application List (aka TaskManager).
 */
#define JPL_CONC_REQUEST_APPLICATION_LIST STATIC_com_jbed_ams_AmsCallHandler_JBED_AMS_EVENT_SHOW_TASK

/**
 * Sets a user request.
 * The actions to honor the request will be executed
 * the next time the root VM is activated.
 * @param request on of:
 * #JPL_CONC_REQUEST_AMS
 * #JPL_CONC_REQUEST_APPLICATION_LIST
 */
void CPL_conc_setAppUserRequest(int32_t request);

#if defined(__cplusplus)
}
#endif

#endif /* __JBED_JPL_CONC_H__ */
