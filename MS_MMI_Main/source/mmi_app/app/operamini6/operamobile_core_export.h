/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef FUCUS_OPERAMOBILE_CORE_EXPORT_H
#define FUCUS_OPERAMOBILE_CORE_EXPORT_H

#ifdef __cplusplus
extern "C" {
#endif

struct _OD_Context;

struct _OD_Context* OperaMobile_AllocODContext(unsigned int size);

OD_STATUS OperaMobile_InitODContext(void);

struct _OD_Context* OperaMobile_GetODContext(void);

#ifdef __cplusplus
};
#endif

#endif /* FUCUS_OPERAMOBILE_CORE_EXPORT_H */
