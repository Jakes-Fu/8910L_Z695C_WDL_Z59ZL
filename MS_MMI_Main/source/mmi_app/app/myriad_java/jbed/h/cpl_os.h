/*
 * Copyright 1998-2008 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Operating system support (VM-independent)
 *
 * Basic support functions from the underlying operating system that are
 * needed by the VM.
 *
 * <!-- #interface list begin -->
 * \section cpl_os_interface Interface
 * - CPL_os_logChar()
 * - CPL_os_flushCache()
 * - CPL_os_getMillisSince1970()
 * - CPL_os_getMillisElapsed()
 * <!-- #interface list end -->
 */

#ifndef __CPL_OS_H__
#define __CPL_OS_H__

#include <jbed_global.h>
#include <jbed_jbni.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* Note that unlike most CPL functions, these require JBNI_EXPORT as they
 * are called directly from Java (on FBCC).
 */

/**
 * Print a character to the debugging log.
 *
 * The output of the <code>out</code> and <code>err</code> streams in
 * <code>java.lang.System</code> is redirected to this function.
 * <p>
 * It is possible to receive 16-bit unicode characters through this function,
 * but most debugging information only uses 7-bit ASCII.  If a port is not able
 * to handle unicode characters, it is suggested that they are replaced by
 * a suitable alternative (for example '?').
 *
 * A port is also free to completely discard all of this output, but there are
 * many classes in Jbed have a compile-time switch to print debugging
 * information, and this can be a very useful debugging technique.
 *
 * @param chr the character to print
 */
void JBNI_EXPORT CPL_os_logChar(jint chr);

#ifdef JBED_CDCA    /* { */

/**
 * Read one or more bytes from standard input (if available).
 *
 * This is used by CDC to implement <code>java.lang.System.in</code>.
 * <p>
 * If Unicode characters are available from the device, they should be
 * passed in the VM default byte encoding.
 * <p>
 * Blocking is handled using the AsyncIO mechanism, so the port
 * must use ASYNC_* calls to arrange for blocking, signalling etc.
 * Note that if there are bytes to return, it should not ask for
 * blocking, because the bytes will be effectively discarded.
 *
 * @param buffer    address of a buffer to be filled
 * @param maxlen    maximum length to fill
 * @return  the positive number of bytes actually filled
 *          OR zero meaning end-of-file detected
 *          OR -1 to indicate ASYNC_callAgain... has been used and the caller
 *              should come back when signalled
 */
int CPL_os_readInputBytes(char *buffer, int maxlen);

#endif  /* } JBED_CDCA */

/**
 * Flush processor's data and instruction cache for a given range.
 *
 * This function must make the processor's instruction and data caches
 * consistent.  If there are no such caches (e.g. on the ARM7 TDMI), the
 * implementation can be left empty.
 *
 * It is called before executing code that Jbed has written to memory.
 *
 * @param start  Start address of region to flush
 * @param length Length in bytes of the region to flush
 */
void JBNI_EXPORT CPL_os_flushCache(const char* start, int length);

/**
 * Return the number of milliseconds since 1st January 1970 UTC.
 *
 * The value returned should be relative to coordinated universal time (UTC),
 * not local time.  However it is acceptable to return the time since
 * 1st of January 1970 in the current time zone (local time) if the target
 * does not support time zones, and does not have access to an absolute time
 * source.  If local time is used, there may be issues with applications that
 * communicate with external entities that have a more accurate notion of
 * time relative to UTC.
 *
 * This function is called by <code>java.lang.System.currentTimeMillis()</code>,
 * which many animated applications use to control their frame rate.
 * Therefore this function should be implemented efficiently.
 *
 * The returned time value should never decrease during a run of the VM, since
 * this might cause problems with applications running on the VM.
 *
 * This function is also used in the implementation of java.util.Timer, and
 * abrupt changes in the time returned may cause problems like too long or too
 * short delays.
 *
 * @see CPL_os_getMillisElapsed
 *
 * @return Time in <b>milliseconds</b> (1.0e-3 seconds) since
 *         00:00 January 1, 1970 UTC.
 */
int64_t JBNI_EXPORT CPL_os_getMillisSince1970(void);

/**
 * Return a relative time in milliseconds since an arbitrary (but fixed) point.
 *
 * Often a platform can more efficiently return a number of milliseconds
 * since system startup, or some other convenient starting point.
 *
 * This is the time base used within the VM for a number of things, including
 * the Java thread scheduler, therefore it should be implemented efficiently.
 * The returned value must never be smaller than a value that was returned
 * previously during the same VM run, otherwise Java threads may block.
 *
 * @see CPL_os_getMillisSince1970
 *
 * @return Elapsed time in <b>milliseconds</b> (1.0e-3 seconds).
 */
int64_t JBNI_EXPORT CPL_os_getMillisElapsed(void);


#ifdef JVM_FDAC

/**
 * Return a relative time in microseconds since an arbitrary (but fixed) point.
 * This is used only for profiling, where some callers benefit from more
 * precision than milliseconds. It is acceptible for a port to return
 * (CPL_os_getMillisElapsed() * 1000)
 *
 * @see CPL_os_getMillisElapsed
 *
 * @return Elapsed time in <b>microseconds</b> (1.0e-6 seconds).
 */
int64_t JBNI_EXPORT CPL_os_getMicrosElapsed(void);

#endif /* JVM_FDAC */

/**
 * CPL_os_setHeapSize: -heapsize option needs this function to set heap size.
 * @arg size: set the heap size to this value
 * @return: none
 */
void CPL_os_setHeapSize(int size);

#ifdef JBED_CHUNKED_HEAP

/**
 * CPL_os_setHeapIncrementSize: -heapinc option needs this function to set
 * heap increment size for chunked heap builds.
 * @arg size: set the minimum chunk size for additional chunks
 * @return: none
 */
void CPL_os_setHeapIncrementSize(int size);

/**
 * CPL_os_setHeapFirstChunkSize: -heapfirst option needs this function to set
 * the initial heap size for chunked heap builds.
 * @arg size: size of the first chunk 
 * @return: none
 */
void CPL_os_setHeapFirstChunkSize(int size);

#endif /* JBED_CHUNKED_HEAP */


#ifdef JVM_FDAC
/**
 * Freeze/exit VM after a fatal internal error.
 *
 * Called when the VM internal state (potentially) has been corrupted, e.g.
 * after an assertion failure.
 *
 */
void CPL_os_fatalError(void);
#endif

#if defined(__cplusplus)
}
#endif

#endif /* __CPL_OS_H__ */
