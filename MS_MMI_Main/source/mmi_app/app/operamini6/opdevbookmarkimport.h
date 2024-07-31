/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/
#ifndef OPDEV_BOOKMARKIMPORT_H
#define OPDEV_BOOKMARKIMPORT_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* All strings for imported bookmarks are encoded in UTF-8 and zero-terminated
* unless otherwise specified.
* The required memory size includes the terminated '\0' character for a string.
*/

/*
* The size of bookmark import entry's data members.
*/
typedef struct _OD_BookmarkImportEntryDataSize
{
	unsigned int title_size;		/* [OUT] */
	unsigned int url_size;			/* [OUT] */
	unsigned int icon_data_size;	/* [OUT] */
} OD_BookmarkImportEntryDataSize;

/*
* Bookmark import entry.
*/
typedef struct _OD_BookmarkImportEntry
{
	/* the title of the bookmark, must not be NULL */
	char *title;					/* [IN/OUT] */
	unsigned int title_size;		/* [IN] */

	/* the url of the bookmark, must not be NULL */
	char *url;						/* [IN/OUT] */
	unsigned int url_size;			/* [IN] */

	/* the favicon data of the bookmark, could be NULL */
	/* PNG or JPEG format is supported, the width and height should be the same */
	void *icon_data;				/* [IN/OUT] */
	unsigned int icon_data_size;	/* [IN] */
} OD_BookmarkImportEntry;

/**
* Check whether any system bookmarks are available.
*
* @note If the function returns true, then please use a loop to enumerate all
* the available bookmarks for import.
* \code
* if (OD_BookmarkImportIsAvailable(g_od_context))
* {
* 	OD_BookmarkImportEnumBegin(g_od_context);
*
* 	for (;;)
* 	{
* 		OD_STATUS od_status;
* 		OD_BookmarkImportEntryDataSize entry_size;
*
* 		od_status = OD_BookmarkImportGetNextDataSize(g_od_context, &entry_size);
* 		if (od_status == OD_OUT_OF_RANGE)
* 		{
* 			break;
* 		}
* 		if (od_status == OD_SUCCESS)
* 		{
* 			OD_BookmarkImportEntry entry;
* 			// @TODO: prepare sufficient buffer to get entry data
* 			if (OD_BookmarkImportGetNext(g_od_context, &entry) == OD_SUCCESS)
* 			{
* 				// @TODO: import this bookmark entry
* 			}
* 			// @TODO: release buffer of entry if necessary
* 		}
* 	}
*
* 	OD_BookmarkImportEnumEnd(g_od_context);
* }
* \endcode
*
* @return OD_TRUE if any system bookmarks are available else OD_FALSE.
*/
OD_MMI_TASK OD_BOOL OD_BookmarkImportIsAvailable(OD_Context *context);

/**
* Begin enumeration of system bookmarks.
*/
OD_MMI_TASK void OD_BookmarkImportEnumBegin(OD_Context *context);

/**
* End enumeration of system bookmarks.
*/
OD_MMI_TASK void OD_BookmarkImportEnumEnd(OD_Context *context);

/**
* Get the data size of the next bookmark entry.
*
* @note Every time before reading the next bookmark, you must call this function
* at first to get the data size(title/url/icon) of the next bookmark.
* Then prepare sufficient memory and get the real data of the next bookmark.
*
* @param[out]  entry_size bookmark entry size to be filled out
*
* @return OD_SUCCESS if a bookmark was available and #entry_size was filled out.
* @return OD_NULL_POINTER if pointer #entry_size is NULL.
* @return OD_OUT_OF_RANGE if there is no more bookmark for enumeration.
* @return OD_NO_MEMORY if OOM.
* @return any other OD_Error error
*/
OD_MMI_TASK OD_STATUS OD_BookmarkImportGetNextDataSize(OD_Context *context, OD_BookmarkImportEntryDataSize *entry_size);

/**
* Read the next bookmark with sufficient memory buffer.
*
* @note You must call OD_BookmarkImportGetNextDataSize just before this function
* is called, then prepare required memory buffer.
* After the function is called, the next bookmark beyond the one returned will
* be ready for enumeration.
*
* @param[out]  entry bookmark entry data to be filled out
*
* @return OD_SUCCESS if a bookmark was available and #entry was filled out.
* @return OD_NULL_POINTER if pointer #entry is NULL.
* @return OD_INSUFFICIENT_BUFFER the given buffer size is not sufficient.
* @return OD_OUT_OF_RANGE if there is no more bookmark for enumeration.
* @return OD_NO_MEMORY if OOM.
* @return any other OD_Error error
*/
OD_MMI_TASK OD_STATUS OD_BookmarkImportGetNext(OD_Context *context, OD_BookmarkImportEntry *entry);

#ifdef __cplusplus
};
#endif

#endif /* OPDEV_BOOKMARKIMPORT_H */
