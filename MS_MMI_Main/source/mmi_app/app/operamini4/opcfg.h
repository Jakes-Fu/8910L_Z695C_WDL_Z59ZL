/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_OPCFG_H
#define OPDEV_OPCFG_H

/**
* Don't MODIFY following line
* By default we build opera mobile, but when build mini, we will replace following line with 
* "#define OD_BUILD_OPERA_MINI" using script.
*/
#define OD_BUILD_OPERA_MINI

#if (defined(OD_BUILD_OPERA_MINI) && defined(OD_BUILD_OPERA_MOBILE))
	|| (!defined(OD_BUILD_OPERA_MINI) && !defined(OD_BUILD_OPERA_MOBILE))
#error "You must define and only define one between OD_BUILD_OPERA_MINI and OD_BUILD_OPERA_MINI "
#endif

/* Include opera dev id header for OpDev */
#ifdef OD_BUILD_OPERA_MINI
#ifndef OD_API_RENAME_PREFIX
#define OD_API_RENAME_PREFIX _OperaMini_
#endif
#include "opera_dev_id_opdev.h"
#endif

#if defined(OD_BUILD_OPERA_MOBILE) && defined(WIN32)
#define OD_SIM_DYNLINK_SUPPORT
#endif

#ifndef OD_HAVE_ENTRY
/************************************************************************/
/* App                                                                  */
/************************************************************************/
#ifdef OD_BUILD_OPERA_MINI
#define OperaMini_Entry		MMIBROWSER_Entry
#endif
#ifdef OD_BUILD_OPERA_MOBILE
#define OperaMobile_Entry	MMIBROWSER_Entry
#endif
#endif /* OD_HAVE_ENTRY */

/************************************************************************/
/* GDI                                                                  */
/************************************************************************/
#define OD_STATUSBAR_LAYER_HEIGHT	MMI_STATUSBAR_HEIGHT
#define OD_TEXT_LAYER_HEIGHT		128

#if !defined(UI_MULTILAYER_SUPPORT)
#define OD_MULTILAYER_DISABLED
#endif

/************************************************************************/
/* Memory                                                               */
/************************************************************************/
#ifdef BZP_SUPPORT
#define OD_MEMORY_POOL_SIZE_MINI	(1024 * 1024)
#else
#define OD_MEMORY_POOL_SIZE_MINI	(1408 * 1024)
#endif
#define OD_MEMORY_POOL_SIZE_MOBILE	(1408 * 1024)
#define OD_MEMORY_POOL_DEFAULT_STEP	(64 * 1024)

#if defined(_DEBUG) && defined(WIN32)
/* In simulator debug mode, we need much memory which the platform can't
provide, then we use a static array as memory pool. */
#define OD_TWEAK_MEMORY_STATIC_DEBUG_POOL
#define OD_TWEAK_ALLOCATE_MEMORY_ONCE
#endif /* _DEBUG && WIN32 */

#ifdef OD_TWEAK_MEMORY_STATIC_DEBUG_POOL
#define OD_MEMORY_DEBUG_POOL_SIZE	(80 * 1024 * 1024)
#endif /* OD_TWEAK_MEMORY_STATIC_DEBUG_POOL */


/************************************************************************/
/* File                                                                 */
/************************************************************************/
#define OD_FILE_DIRECTORY_SEPARATOR     L'\\'
#define OD_FILE_NAME_MAX_LENGTH         SFS_MAX_PATH
#define OD_FILE_CTL_STR_MAX_LENGTH      128

#define OD_FILE_PREDEFINED_PATH_PICTURE     L"?:\\Image"
#define OD_FILE_PREDEFINED_PATH_VIDEO       L"?:\\Video"
#define OD_FILE_PREDEFINED_PATH_SOUND       L"?:\\Sound"
#define OD_FILE_PREDEFINED_PATH_MUSIC       L"?:\\Music"
#define OD_FILE_PREDEFINED_PATH_DOCUMENT    L"?:\\Doc"
#define OD_FILE_PREDEFINED_PATH_OTHER       L"?:\\Other"

// Folders for prestorage and storage files, they should be different.
// Prestorage: for read-only resources, such as locales. This folder must exist.
// Storage: for read-write resources, such as settings, saved pages, etc.
// They must be end with path separator "\\"
#define OD_FILE_CUSTOM_PRESTORAGE_FOLDER	L"E:\\OperaMini\\"
#define OD_FILE_CUSTOM_STORAGE_FOLDER		L"E:\\OperaMini\\"


// We use Timer id to identify the timer, so the name is not significant
#define OD_TIMER_NAME					"Opera_Timer"

#include "optweak.h"

/************************************************************************/
/* Custom config                                                        */
/************************************************************************/
#ifdef OPDEV_CUSTOM_CFG
#include OPDEV_CUSTOM_CFG
#endif /* OPDEV_CUSTOM_CFG */


#endif /* OPDEV_OPCFG_H */
