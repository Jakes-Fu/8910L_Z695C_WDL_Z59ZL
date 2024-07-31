/*
 * Copyright 1998-2005 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Global definitions (FastBCC version)
 *
 * Define types and constants for porting layer that allow writing
 * portable C code for the Jbed virtual machine.  This provides:
 * - Basic C99 types (int32_t, etc.)
 * - C Runtime library functions (CRTL_malloc, etc.)
 * - Iassert assertion support
 * - Trace() support
 * - Operations for 64-bit integers
 *
 * It also includes:
 * - Toolset-specific definitions
 * - General/base definitions
 * - Processor-specific definitions
 * - OS-specific definitions
 * - Target-specific definitions
 */

#ifndef __JBED_GLOBAL_H__
#define __JBED_GLOBAL_H__

#include <jbed_types.h>     /* provide basic c99 types */

#include <toolset_def.h>    /* toolset-specific definitions */
#include <base_def.h>       /* general/base definitions */
#include <arch_def.h>       /* processor-specific definitions */
#include <os_def.h>         /* os-specific definitions */
#include <targ_def.h>       /* target-specific definitions */

#include <jbed_long.h>      /* provide operations for 64-bit integers */
#include <uapp_unit.h>      /* provide marco define for uapp unit test */

#include <crtl.h>           /* c runtime library functions */
#include <iassert.h>        /* iassert assertion support */
#include <jbed_trace.h>     /* trace() support */

#endif  /* __JBED_GLOBAL_H__ */
