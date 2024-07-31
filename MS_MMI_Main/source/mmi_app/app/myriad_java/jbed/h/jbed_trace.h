/*
 * Copyright 2001-2004 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Tracing facility (FastBCC version)
 *
 * Basic tracing macros for Jbed.
 */

#ifndef	TRACE_INCLUDED
#define	TRACE_INCLUDED

#ifdef TRACE_ON
#define TRACE_VAL 1
#else
#define TRACE_VAL 0
#endif

void JbedLibTrace(const char * fmt,...);

/*
 * For -nodb builds Trace is not mangled by db2.pl.
 * Map to printf by default (not too useful since this will be very verbose).
 */
/*detail in jbed_os.c*/
#define	Trace


#endif	/* TRACE_INCLUDED */
