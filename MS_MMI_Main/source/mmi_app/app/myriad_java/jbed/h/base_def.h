/*
 * Copyright 1998-2012 Myriad Group AG. All Rights Reserved.
 * $Id$
 */


/**
 * @file
 *
 * This file contains base definitions for writing portable code in Jbed sources
 */

#ifndef _BASE_DEF_H_
#define _BASE_DEF_H_

/* For non production builds, redefine JBED_INLINE so inlining never happens.
 * Only redefine JBED_INLINE if it is already defined. If base_def.h has been
 * included in the right order (after toolset_def.h) then it will always be
 * defined. This eliminates a spurious redefinition when base_def.h is
 * included from other old headers such as insignia.h.
 */
#if defined JBED_INLINE && !defined PROD
#undef  JBED_INLINE
#define JBED_INLINE /* Disable inlining in debug builds */
#endif

/* You should use this macro wherever you have an argument to a
 * function which is unused.
 */
#ifndef UNUSED
#define UNUSED(x)       { (void)x; }
#endif /* UNUSED */

/* String literal concatenation and quoting */

#ifdef  ANSI
#define STRINGIFY(x)    #x
#define CAT(x, y)       x ## y
#else   /* !ANSI */
#define STRINGIFY(x)    "x"
#define CAT(x, y)       x y
#endif  /* ANSI */

#define QUOTED(x)       STRINGIFY(x)


/* True and false.  Compatible with C99 and C++ true/false. */
#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

/* Standard Storage Classes */
#define GLOBAL                  /* Function referenced outside current file */
#define LOCAL   static          /* Local to the source file */
#define SAVED   static          /* For local static variables */

#include <publicsymbol.h>       /* publicsymbol - to be exported */

/* Other 'useful' macros. */
#define TABLE_SIZE(table)               (sizeof(table) / sizeof(table[0]))
#ifndef STRUCT_OFFSET
#define STRUCT_OFFSET(stname, field)    ((unsigned)&(((stname *)NULL)->field))
#endif /* STRUCT_OFFSET */
#define EQUAL_BOOLEANS(b1, b2)          ((!(b1)) == (!(b2)))

#endif /* _BASE_DEF_H_ */
