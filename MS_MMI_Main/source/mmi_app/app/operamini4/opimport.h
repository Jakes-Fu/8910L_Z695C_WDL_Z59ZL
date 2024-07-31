/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_OPIMPORT_H
#define OPDEV_OPIMPORT_H

#include "os_api.h"
#include "socket_types.h"
#include "socket_api.h"
#include "in_message.h"

#ifdef OPERA_MINI_VER42_DEBUG
#ifndef MOP_DEBUG
#define MOP_DEBUG
#endif
#ifndef DEBUG
#define DEBUG
#endif
#ifndef _DEBUG
#define _DEBUG
#endif
#endif //OPERA_MINI_VER42_DEBUG
#include "opera_id.h"

#include "operamini_core_export.h"
#include "operamobile_core_export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* Standard C functions, it depend on platform,
* so put it here, not in OpDevUtil.h
*/
#define od_snprintf _snprintf
#define od_vsnprintf _vsnprintf
#define od_memchr memchr
#define od_memcmp memcmp
#define od_memcpy memcpy
#define od_memmove memmove
#define od_memset memset
#define od_strlen strlen
#define od_strcpy strcpy
#define od_strncpy strncpy
#define od_strcat strcat
#define od_strncat strncat
#define od_strcmp strcmp
#define od_strncmp strncmp
#define od_strchr strchr
#define od_strrchr strrchr
#define od_strstr strstr

#define OD_6530_12A
#define OD_6530_SUPPORT
#define OD_7701_SUPPORT

#ifdef OD_7701_SUPPORT
#define OD_HAVE_SCHEME_API
#endif

#if defined(OD_6530_12A) || defined(OD_6530_12B) || defined(OD_6530_12C) || defined(OD_8501_MAIN)
#include "mmibrowser_export.h"
#include "mmiconnection_export.h"
#include "mmipdp_export.h"
#include "mmiphone_export.h"
#include "mmiset_export.h"
#include "mmifmm_export.h"
#include "mmiwclk_export.h"

#ifdef WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif /* WIFI_SUPPORT */

#else
#include "mmibrowser_api.h"
#include "mmiconnection.h"
#include "mmipdp_api.h"
#include "mmiphone.h"
#include "mmiset.h"
#include "mmi_filemgr.h"
#include "mmiwclk_main.h"

#ifdef WIFI_SUPPORT
#include "mmiwifi_api.h"
#endif /* WIFI_SUPPORT */

#endif /* OD_6530_12A || OD_6530_12B || OD_6530_12C || OD_8501_MAIN */


#ifdef __cplusplus
};
#endif


#endif /* OPDEV_OPIMPORT_H */
