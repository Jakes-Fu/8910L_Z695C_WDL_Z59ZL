/*
 * Copyright 2006 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Application ID's (VM-independent)
 */

#ifndef __JBED_APPS_H__
#define __JBED_APPS_H__

#include <jbed_global.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @defgroup appid Application-identifiers
 * Ports may override the following values in their OS- or
 * target-specific definitions, for example in os_def.h or targ_def.h.
 * Modifying the application-identifiers in a binary distribution must
 * not necessarily work, since some source files may have been compiled
 * by using the definitions of this file.
 */
/*@{*/
#ifndef JBED_APP_JVM
/** The Java Virtual Machine, either FBCC or FDAC */
#define JBED_APP_JVM     1
#endif
#ifndef JBED_APP_WAP
/** The u\@WAP application */
#define JBED_APP_WAP     2
#endif
#ifndef JBED_APP_MMS
/** The u\@MMS application */
#define JBED_APP_MMS     4
#endif
#ifndef JBED_APP_DRM
/** The u\@DRM application */
#define JBED_APP_DRM     8
#endif
/*@}*/

#if defined(__cplusplus)
}
#endif

#endif /* __JBED_APPS_H__ */
