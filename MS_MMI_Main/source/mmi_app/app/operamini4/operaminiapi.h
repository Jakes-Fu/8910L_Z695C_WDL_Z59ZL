/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_OPERA_MINI_API_H
#define OPDEV_OPERA_MINI_API_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _OperaMiniRunNetOptions
{
    /* Use WIFI or specified sim card to connect network, OD_SIM_INDEX_WIFI means WIFI,
    can be -1 to mean not specified */
    int sim_index;

    /* Use specified profile to connect network, can be -1 to mean not specified. */
    int profile_index;

    /* Indicate if let user to use WIFI. */
    OD_BOOL wifi_enabled;

} OperaMiniRunNetOptions;

typedef struct _OperaMiniRunOptions
{
    /* Open specified URL, can be NULL to mean not specified. Should be in UTF-8 encoding. */
    const char * url;
    
    OperaMiniRunNetOptions net_options;

} OperaMiniRunOptions;

/**
* Globally initialize opera mini application. It should be called once when MMI task initialize.
* @return OD_SUCCESS when opera is initialized successfully
*         OD_ERROR when fail.
*/
OD_STATUS OperaMini_InitApp(void);

/**
 * Run opera mini.
 * It is assumed OD_Context has been initialized properly at this point.
 * @param param Parameters to launch opera mini.
 * @return OD_SUCCESS when opera is launched successfully
 *         OD_ERROR when fail.
 */
OD_STATUS OperaMini_Run(const OperaMiniRunOptions* const param);

/**
* Bring opera mini to front.
* @return OD_SUCCESS when opera is brought to front successfully
*         OD_ERROR when fail.
*/
OD_STATUS OperaMini_BringToFront(void);

/**
* Get opera mini version string, in utf-8 encoding.
* @return pointer to version string.
*/
char* OperaMini_GetVersionString(void);

/**
* Check if opera mini is running.
* @return OD_TRUE when it is running, otherwise OD_FALSE.
*/
OD_BOOL OperaMini_IsRunning(void);

/**
* Check if opera mini is in front.
* @return OD_TRUE when it is in front, otherwise OD_FALSE.
*/
OD_BOOL OperaMini_IsInFront(void);

/**
* Reset Opera, delete all config/data.
* @return OD_SUCCESS when successfully
*         OD_ERROR when fail.
*/
OD_STATUS OperaMini_FactoryReset(void);

/**
* Reset network connection, should be called when the current connection config is not available.
*/
void OperaMini_ResetConnection(void);

/**
* Exit opera mini, can be called when opera is not running.
* @return OD_SUCCESS when exit opera successfully or opera is not running
*         OD_ERROR when fail.
*/
OD_STATUS OperaMini_Exit(void);

#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /* OPDEV_OPERA_MINI_API_H */
