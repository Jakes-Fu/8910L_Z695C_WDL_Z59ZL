/*
 * Copyright 2004-2009 Esmertec AG. All Rights Reserved.
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
 * Supported since rvct 2.1
 */
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 210000)
#define JBED_NORETURN  __declspec(noreturn)
#else
#define JBED_NORETURN  /* nothing */
#endif

/**
 * Format string for printf that can be used to print a pointer with a
 * consistent format of 0x0124abcd.
 */
#define JBED_FMTPTR "%p"

/**
 * Format modifier for printf that can be used to print a 64-bit integers.
 */
#define JBED_FMT64  "ll"

/*
 * Set DOUBLE_WORDS_FLIPPED acording to the -fpu=softxyz value passed to the
 * C compiler. Note that (despite the documentation) the ADS1.2 toolkit does
 * not flip double values in big endian mode.
 */
#if defined(__TARGET_FPU_SOFTFPA) && !defined(JBED_BIG_ENDIAN)
#define DOUBLE_WORDS_FLIPPED
#endif /* ADS_TOOLKIT && !JBED_BIG_ENDIAN */

#endif /* __TOOLSET_DEF_H__ */
