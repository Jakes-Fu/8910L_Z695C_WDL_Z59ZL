/* Sample KD/kdplatform.h for OpenKODE Core 1.0  */
#ifndef __kdplatform_h_
#define __kdplatform_h_

#define KD_API
#define KD_APIENTRY

typedef int KDint32;
typedef unsigned long int KDuint32;
#ifdef WIN32
typedef __int64 KDint64;
typedef unsigned __int64 KDuint64;
#else
typedef long long KDint64;
typedef unsigned long long KDuint64;
#endif
typedef short KDint16;
typedef unsigned short KDuint16;
typedef unsigned long KDuintptr;
typedef unsigned long KDsize;
typedef long KDssize;
#define KDINT_MIN (-0x7fffffff-1)
#define KDINT_MAX 0x7fffffff
#define KDUINT_MAX 0xffffffffU
#define KDINT64_MIN (-0x7fffffffffffffffLL-1)
#define KDINT64_MAX 0x7fffffffffffffffLL
#define KDUINT64_MAX 0xffffffffffffffffULL
#define KD_NORETURN
#define KD_INFINITY (1.0F/0.0F)
/*
#define KD_WINDOW_SUPPORTED
*/
#ifdef KD_NDEBUG
#define kdAssert(c)
#else
#define kdAssert(c) ((void)( (c) ? 0 : (kdHandleAssertion(#c, __FILE__, __LINE__), 0)))
#endif


/*
 If platform has got a function like printf,
 this functions isn't neccessary and define
 KD_PRINTF as the function's name.
 
 for example, on linux platform,
 #define	KD_PRINTF printf
*/
KD_API void kdPrint(KDuint32 mode, const char * fmt, ...);
extern KDuint32 kdTracemode;

#define	MAX_MSG_LEN 256

#define	KD_TRACE_LOW	KD_PRINTF
#define KD_TRACE_ERROR  KD_PRINTF
#define KD_TRACE_HIGH   KD_PRINTF

#define	KD_SPRINTF		sprintf

#ifdef WIN32
  #define	KD_PRINTF		printf
  #define __ALIGN(_b)
#else
#define	KD_PRINTF(...)		kdPrint(kdTracemode, __VA_ARGS__)
  /*
   * compiler specific keywords
   */
  #define __ALIGN(_b)    __align(_b) /* unit: byte */

#endif

#endif /* __kdplatform_h_ */

