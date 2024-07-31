/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef FUCUS_OPERAMINI_CORE_EXPORT_H
#define FUCUS_OPERAMINI_CORE_EXPORT_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _OD_Context;

struct _OD_Context* OperaMini_AllocODContext(unsigned int size);

OD_STATUS OperaMini_InitODContext(void);

struct _OD_Context* OperaMini_GetODContext(void);

struct _OD_FileBlobType;
const struct _OD_FileBlobType* OperaMini_GetCustomDeployFiles(void);

/* Functions used by OperaMiniApi.h */
void MOpApp_shutdown(void *userData);

OD_STATUS MOpApp_openPage(const char * url);

OD_STATUS MOpApp_start(void*, OD_START_CALLBACK, void*);

OD_STATUS MOpApp_resume(void);

OD_BOOL MOpApp_isRunning(void);

OD_BOOL MOpApp_isActive(void);

char* MOpApp_getVersion(void);

OD_STATUS MOpApp_factoryReset(void);

void MOpApp_resetConnection(void);

#ifdef __cplusplus
};
#endif

#endif /* FUCUS_OPERAMINI_CORE_EXPORT_H */
