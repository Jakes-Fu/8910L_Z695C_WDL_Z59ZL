/*
 * Copyright 1998-2004 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * This file provides the default OS-family specific
 * C runtime library environment for building Insignia's
 * EVM. A version of <crtl_md.h> is included via
 * <crtl.h> in all source files within the build.
 *
 * See <crtl.h> for more information.
 *
 * This default version simply assumes that an ANSI C
 * runtime library is available and should be used, we
 * thus simply need to include the appropriate ANSI
 * headers and <crtl.h> will do the rest.
 *
 * Target platforms which require differing runtime
 * environments, or additional setup should supply their
 * own version(s) of <crtl_md.h>, <os_crtl_md.h> or
 * <targ_crtl_md.h> in a target specific include
 * directory that will be found before this file.
 *
 */

/* OS-family specific Includes */

/*
 * Ensure we have prototypes for all ANSI stdio, string, character
 * class, & memory manipulation APIs.
 */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdio.h>
#include <ctype.h>

/* OS-family specific Macro Definitions */

/*
 * Include next, more specific CRTL header
 */
#include <os_crtl_core_md.h>

/* OS-family specific Function Prototypes */
