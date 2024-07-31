/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_OPERA_MOBILE_API_H
#define OPDEV_OPERA_MOBILE_API_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct  _OD_APIRequest
{
	int argc;
	const char** argv;
} OD_APIRequest;

#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /* OPDEV_OPERA_MOBILE_API_H */
