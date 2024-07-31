/*
 * Copyright 2004-2005 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Toolset specific definitions
 *
 * Define any macros and types that are toolset specific, to allow writing
 * portable C code for the Jbed virtual machine.
 */

#ifndef __TOOLSET_DEF_H__
#define __TOOLSET_DEF_H__

/**
 * Macro that requests the C compiler to consider a function for inlining.
 * Note that generally C compilers consider this a hint, and may choose
 * no to inline.
 */
#define JBED_INLINE __inline

/**
 * Macro that tells the compiler a function will never return.
 */
#define JBED_NORETURN __declspec(noreturn)

/**
 * Format string for printf that can be used to print a pointer with a
 * consistent format of 0x0124abcd.
 */
#define JBED_FMTPTR "0x%p"

/**
 * Format modifier for printf that can be used to print a 64-bit integers.
 */
#define JBED_FMT64  "I64"

#endif /* __TOOLSET_DEF_H__ */
