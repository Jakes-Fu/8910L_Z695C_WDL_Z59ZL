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


OD_BOOL OD_BookmarkImportIsAvailable(OD_Context *context)
{
	(context);

	return OD_FALSE;
}

void OD_BookmarkImportEnumBegin(OD_Context *context)
{
	(context);

	OD_ASSERT(!"Shouldn't be called!");
}

void OD_BookmarkImportEnumEnd(OD_Context *context)
{
	(context);

	OD_ASSERT(!"Shouldn't be called!");
}

OD_STATUS OD_BookmarkImportGetNextDataSize(OD_Context *context, OD_BookmarkImportEntryDataSize *entry_size)
{
	(context);
	(entry_size);

	OD_ASSERT(!"Shouldn't be called!");

	return OD_OUT_OF_RANGE;
}

OD_STATUS OD_BookmarkImportGetNext(OD_Context *context, OD_BookmarkImportEntry *entry)
{
	(context);
	(entry);

	OD_ASSERT(!"Shouldn't be called!");

	return OD_OUT_OF_RANGE;
}
