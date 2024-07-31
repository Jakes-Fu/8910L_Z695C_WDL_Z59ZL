// debug.h
#ifndef __WAP_DEBUG_H__
#define __WAP_DEBUG_H__


#define warning(a, b)


#ifdef	_WIN32
#define	__func__	"function unknown"
#endif

#ifdef WSP_ASSERT_MODE
	#define WSP_DEBUG_PRINTF SCI_TRACE_HIGH /*assert verified*/
#else
	#define WSP_DEBUG_PRINTF SCI_TRACE_LOW
#endif

#define NO_GWASSERT
#define gw_assert_place(expr, file, lineno, func)

#define gw_malloc     SCI_ALLOCA
#define gw_free       SCI_FREE

#endif


