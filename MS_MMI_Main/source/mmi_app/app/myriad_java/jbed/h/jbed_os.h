/*
 * Copyright 1998-2008 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Low-Level Porting Functions (FastBCC legacy)
 *
 * Basic support functions for the Jbed VM.  The functions declared here must
 * be implemented in a Jbed port.  They provide low-level functionality such as
 * memory management, multithreading, timing, etc.
 *
 * <b>Definition</b>: The "Jbed task" is the OS task in the Jbed port that calls
 * Jbed_run() (i.e. Jbed_initialize() and Jbed_iterate()).  We use the word
 * "task" for an OS-level thread and "thread" for a Java-level thread.
 *
 * The associated JbedLinker map file is <code>OPL_os_Map.txt</code>.
 * <!-- #interface list begin -->
 * \section jbed_os_interface Interface
 * - OPL_os_getHeapBlock()
 * - OPL_os_freeHeapBlock()
 * - OPL_os_getStackSpace()
 * - OPL_os_initTimer()
 * - OPL_os_startTimer()
 * - OPL_os_resetTimer()
 * - OPL_os_getElapsedMicros()
 * - JbniS_java_lang_System_getProperty0()
 * - OPL_os_fatalError()
 * <!-- #interface list end -->
 */

#ifndef __JBED_OPL_OS_H__
#define __JBED_OPL_OS_H__

#include <jbed_jbni.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Return amount of stack space usable by the VM.
 *
 * Since Jbed runs a Java thread on the original stack of the native calling
 * code, it needs to know how much space can be used in order to do stack
 * checks.
 *
 * The VM calls this during Jbed_initialize() to determine where to place
 * the limit on stack usage for the initial Java thread.
 *
 * <b>Note</b>: This is not the only stack in the Jbed system.  The VM will
 * allocate Java thread stacks in the Java heap and will switch between these
 * stacks internally.
 *
 * @return The number of bytes of stack space the VM is allowed to use.
 */

jint JBNI_EXPORT OPL_os_getStackSpace(void);

/**
 * Return a block of memory for the Java heap.
 *
 * The VM manages the Java heap using a garbage collector.  This function is
 * normally (see below) called once per VM startup to obtain a large block of
 * memory for the Java heap.
 *
 * The size of the heap required depends largely on the profile which the VM
 * has to execute (e.g. MIDP2), as well as the requirements of the applications
 * that run.
 *
 * In JBED_CHUNKED_HEAP configurations, the VM will attempt to get new heap
 * blocks when an allocation cannot be made on the existing heap. In those
 * situations, the size parameter specifies the minimal size of the block that
 * will guarantee the pending allocation to succeed. The implementation is free
 * to return a larger block and adjust the size parameter accordingly. A
 * smaller block may also be returned, but the VM will still throw an
 * OutOfMemoryError for the pending allocation.  If this function is unable to
 * return a memory block (or the requested size cannot be met and the
 * implementation chooses not to return a smaller block), it must set
 * <code>base</code> to NULL.
 *
 * The <code>size</code> parameter will be 0 during allocation of the initial
 * heap block, or in configurations without JBED_CHUNKED_HEAP. In this case,
 * the implementation is free to return whatever size is available. Returning
 * larger blocks results in better VM performance and less heap fragmentation.
 * The use of JBED_CHUNKED_HEAP should be avoided if possible as the VM will
 * perform better with a single heap block.
 *
 * For JBED_GENERATIONAL_GC builds, an second heap block will be requested at
 * VM startup for the young generation. The <code>size</code> parameter for
 * that request will be -1. The implementation is free to choose an appropriate
 * size. The young generation chunk will not be released until VM shutdown.
 *
 * The returned block need not be aligned in any special way.  The VM will
 * align the contents of the block itself.
 *
 * <em>NOTE:</em> It is not generally safe to call back into the VM from this function.
 * This is called very early during VM initialization, when the data structures
 * for calls into the VM have not yet been set up.
 *
 * @param base returns the starting address of the block.
 * @param size specifies the required size of the memory block (0 for first
 * block) and is adjusted to return the actual size of the allocated block in
 * bytes.
 */

void JBNI_EXPORT OPL_os_getHeapBlock(void **base, jint *size);

/**
 * Free a Java heap block.
 *
 * Whenever the VM releases a heap block, it will call this function to notify
 * the native side.
 *
 * @param base is the starting address of the block.
 */

void JBNI_EXPORT OPL_os_freeHeapBlock(void *base);

/**
 * Switch a debugging indicator on or off.
 *
 * When debugging some low-level code without a debugger, it is sometimes
 * useful to have a simple on/off debugging indicator, e.g. a LED.
 *
 * This function is not normally used in Jbed, so its implementation may be
 * empty.
 *
 * @param value is the indicator value (on or off).
 */

void JBNI_EXPORT OPL_os_showLED(jboolean value);

/**
 * Report a fatal VM error.
 *
 * This function reports a serious VM error, usually indicating an
 * implementation error.  The debug version of the VM includes additional
 * checks.
 *
 * This function should not return.  In a debugging environment it should break
 * to the debugger, or loop idly so that a debugger can be attached.  In a
 * production environment it should reset the target.  If this function returns
 * the VM will execute an endless tight loop.
 *
 * @param inf is a String object describing the error.
 * @param extra may contain some additional information on the error.
 */

void JBNI_EXPORT OPL_os_fatalError(STRING_INSTANCE inf, INSTANCE extra);

/**
 * Get a property value.
 *
 * This method is called by <code>java.lang.System.getProperty()</code> to
 * retrieve a property value.
 *
 * <b>Note</b>: Constant properties can also be defined in the JbedLinker
 * project file.  If a property is defined in both place, the value returned by
 * this function will be used.
 *
 * @param key is the name of the property.
 * @return the value of the property, or NULL if not found.
 */

STRING_INSTANCE JBNI_EXPORT
JbniS_java_lang_System_getProperty0(STRING_INSTANCE key);

/**
 * Return elapsed time as a 64-bit microsecond value.
 *
 * Return the elapsed time since system startup (or some other convenient
 * starting point).  This is the time base for the Java thread scheduler,
 * therefore this function should be implemented efficiently.  The returned
 * value must never be smaller than a value that was returned previously during
 * the same VM run, otherwise Java threads will block.
 *
 * @return Elapsed time in <b>microseconds</b> (1.0e-6 seconds).
 */

jlong JBNI_EXPORT OPL_os_getElapsedMicros(void);

/**
 * Initialize the scheduling timer.
 *
 * This is called once per VM startup.  It should initialize (but not start) a
 * one-shot timer with the specified period.  The timer is started by
 * OPL_os_startTimer() and restarted by OPL_os_resetTimer().  The timer period
 * can be configured in the JbedLinker project file and is the same in all
 * calls of OPL_os_initTimer(), so an implementation may configure a timer once
 * and just reenable it, if that is simpler or more efficient in the OS being
 * ported to.
 *
 * The timer should be set up so that it calls Jbed_timerEntry() when it
 * expires.
 *
 * @param usecs is the timer period in <b>microseconds</b> (1.0e-6 seconds).
 */

void JBNI_EXPORT OPL_os_initTimer(jint usecs);

/**
 * Start the scheduling timer.
 *
 * This is called once per VM startup to start the one-shot timer that was
 * initialized by OPL_os_initTimer().
 */

void JBNI_EXPORT OPL_os_startTimer(void);

/**
 * Restart the scheduling timer.
 *
 * This is called to restart the scheduling timer after it has expired once.
 * In most systems the implementation will be identical to OPL_os_startTimer().
 * OPL_os_resetTimer() is called often, so it should be implemented
 * efficiently.
 */

void JBNI_EXPORT OPL_os_resetTimer(void);

#if defined(__cplusplus)
}
#endif

#endif /* __JBED_OPL_OS_H__ */
