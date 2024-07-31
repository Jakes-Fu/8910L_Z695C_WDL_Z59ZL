/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_RESOURCE_H
#define OPDEV_RESOURCE_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _OD_FileBlobType {
	const char *file_name;
	const unsigned int file_size;
	const unsigned char *file_data;
	const int file_crc;
} OD_FileBlobType;

OD_MMI_TASK void OD_MobileResInit(OD_Context *context);

OD_MMI_TASK void OD_MiniResInit(OD_Context *context);

OD_MMI_TASK int OD_GetBreamStringCount(OD_Context *context);

OD_MMI_TASK OD_STATUS OD_GetBreamString(OD_Context *context, int key, const OD_WCHAR** str);

OD_CORE_TASK OD_STATUS OD_GetOperaString(OD_Context *context, int key, const OD_WCHAR** str);

/**
* Get the file blob stream of all the static files.
* A dummy element {NULL, 0, NULL, 0} indicates the end of the stream.
*
* @note Static files are preloaded on ROM or already loaded in RAM.
*
* @param context the opera context
* @return The file blob stream ending with a dummy element, the stream could be NULL.
*/
OD_MMI_TASK const OD_FileBlobType* OD_GetStaticFiles(OD_Context *context);

/**
* Get the file blob stream of all the deployed files.
* A dummy element {NULL, 0, NULL, 0} indicates the end of the stream.
*
* @note Deploy files are typically preloaded in resource module, and will be deployed
* to specified folders when app is running for the first time or certain file is missing.
*
* @param context the opera context
* @return The file blob stream ending with a dummy element, the stream could be NULL.
*/
OD_MMI_TASK const OD_FileBlobType* OD_GetDeployFiles(OD_Context *context);

#ifdef __cplusplus
};
#endif

#endif /* OPDEV_RESOURCE_H */
