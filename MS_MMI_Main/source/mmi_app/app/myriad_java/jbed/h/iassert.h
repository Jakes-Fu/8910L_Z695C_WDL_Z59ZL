/*
 * Copyright 2004-2005 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Assertion facility (FastBCC version)
 */

/*
 * TODO: Use another function name (other than assert()) that allows it to be
 * replaced by platform porting code i.e. Include in one of the CPL's
 */

#ifndef	IASSERT_INCLUDED
#define	IASSERT_INCLUDED

#include <assert.h>

/*===========================================================================*/
/*                      INTERFACE MACROS                                     */
/*===========================================================================*/

#ifdef IASSERT_SUPPRESS_ALWAYS
#define Iassert_always(a)      ((void)0)
#else
#define Iassert_always(a)      assert(a)
#endif

#if defined(IASSERTS)
#define Iassert(a)      Iassert_always(a)
#else
#define Iassert(a)      ((void)0)
#endif

#endif  /* IASSERT_INCLUDED */
