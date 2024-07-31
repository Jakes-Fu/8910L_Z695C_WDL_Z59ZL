/*
 * Copyright 2000-2006 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Low-Level VM Upcalls (FastBCC legacy)
 *
 * Constants and prototypes exported from the Jbed VM to the native world.
 *
 * The functions declared here are implemented in the Jbed VM (in Java) and are
 * called from the Jbed porting layer.
 *
 * The associated JbedLinker map file is <code>OPL_os_Map.txt</code>.
 */

#ifndef __JBED_OPL_H__
#define __JBED_OPL_H__

#include <jbed_jbni.h>
#include <jbed_main.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @name Return Values for Jbed_iterate()
 *
 * Return code values for Jbed_iterate().
 */

/*@{*/

/**
 * Privileged VM code called <code>java.lang.System.exit()</code>.
 */
#define JBED_SYSTEM_EXIT  ((jint)-3)

/**
 * An internal error occurred in the Jbed scheduler (should never happen).
 */
#define JBED_ITERATE_EXCEPTION  ((jint)-2)

/**
 * The time limit specified in the Jbed_iterate() call was exceeded.
 */
#define JBED_ITERATE_TIMEOUT  ((jint)-1)

/**
 * The smallest idle time value in ms.
 */
#define JBED_MIN_IDLE   ((jint)0)

/**
 * The largest idle time value in ms (signifies that the VM is completely idle).
 */
#define JBED_MAX_IDLE   ((jint)0x7fffffff)

/*@}*/

/*
 * --- Initialization and shutdown ---
 */

/**
 * @name Functions for VM Life-cycle Control
 *
 * Low-level functions to control the VM life-cycle (initialization, running,
 * shutdown).
 *
 * Since Jbed release 4.1, the VM life-cycle handling has been implemented in a
 * standard porting module and these functions should not be called directly
 * any more.
 *
 * @see jbed_main.h
 */

/*@{*/

/**
 * Initialize the lower layers of the VM (memory management and thread
 * scheduler).
 *
 * Since Jbed release 4.1, porting code should call Jbed_run() instead.
 *
 * Initialize (clear) the heap and create a Java thread to execute the static
 * initializers and main class defined in the Jbed project file.  The created
 * thread will be executed during subsequent calls to Jbed_iterate().
 *
 * @param argc is the number of "command line" arguments.
 * @param argv is a standard C-style (<code>char *argv[]</code>) argument list.
 * Each <code>char *argv[n]</code> entry is expected to reference a null terminated
 * UTF8 encoded string.
 * @return The ID of the initially created Isolate when running an MVM build.
 *
 * The "command line" arguments are converted from ASCII to Java strings, and
 * passed to the <code>main(String args[])</code> static method of the main
 * class defined in the Jbed project file.
 */

extern jint JBNI_IMPORT Jbed_initialize(jint argc, char **argv); /* Main.initialize */

/**
 * Shut down the VM.
 *
 * Since Jbed release 4.1, this will be called by Jbed_run(), and should not be
 * called directly.
 *
 * Call the finalizers of all objects in the heap (regardless whether they are
 * reachable or not).
 */

extern void JBNI_IMPORT Jbed_shutdown(void);  /* Main.shutdown */

/**
 * Java scheduler entry point.
 *
 * Since Jbed release 4.1, porting code should call Jbed_run() instead.
 *
 * This is the entry point to the Jbed thread scheduler, which will select the
 * next Java thread to run and start executing it.  Execution of Java threads
 * continues until the specified timeout is reached, or there are no ready
 * threads available.
 *
 * @param millis specifies the number of milliseconds Java threads are allowed
 * to run before this function will return.  The time base is defined by
 * OPL_os_getElapsedMicros().  The timeout will only occur after
 * Jbed_timerEntry() is called next (unless the VM is configured for
 * non-preemptive scheduling).
 *
 * @return result code, see below.
 *
 * JBED_SYSTEM_EXIT indicates that java.lang.System.exit(n) was called by an
 * authorized thread.  Jbed_getExitCode() can be called to get the exit code n.
 *
 * JBED_ITERATE_EXCEPTION indicates that the Jbed scheduler threw an exception.
 * This should never happen and the native world must terminate the VM and not
 * call back into Jbed_iterate().
 *
 * JBED_ITERATE_TIMEOUT indicates that Jbed_iterate() timed out, but at least
 * one Java thread is still ready to run.  The recommended response is to call
 * Jbed_iterate() again immediately (this case can often be treated the same as
 * a 0 return value).
 *
 * A return value in the range JBED_MIN_IDLE to JBED_MAX_IDLE-1 indicates how
 * many milliseconds it will be before at least one Java thread is ready to run
 * again.  It is recommended that the native task sleep for this long and only
 * then call Jbed_iterate() again, to save power.  The task should not sleep
 * longer than this, since that will cause delays for Java threads.
 *
 * JBED_MAX_IDLE indicates that all Java threads are idle.  The native task can
 * sleep indefinitely before calling Jbed_iterate() (but see below).
 *
 * For example, if 1500 is returned, the Jbed native task can sleep for 1.5
 * seconds before calling Jbed_iterate() again.  It is not an error to call
 * Jbed_iterate() earlier than this, since the scheduler will just recompute
 * the new idle time and return it.
 *
 * <b>IMPORTANT</b>: The state of a Java thread can be changed from the native
 * world with Jbed_signal() or one of the event delivery upcalls
 * (Jbed_doKeyDown(), Jbed_doKeyUp(), etc.).  If this is done, the Jbed native
 * task must stop sleeping and call Jbed_iterate() again as soon as possible to
 * guarantee prompt response to the signal or event.
 */

extern jint JBNI_IMPORT Jbed_iterate(jint millis);  /* Scheduling.xIterate */

/**
 * Retrieve exit code.
 *
 * Retrieve the <code>java.lang.System.exit()</code> code when Jbed_iterate()
 * returns JBED_SYSTEM_EXIT.
 *
 * @deprecated The exit code is passed to lifecycle transition callbacks when
 * transitioning to <code>JBED_MAIN_DOWN</code>. This method is unable to fetch
 * exit codes of many isolates in MVM builds, so it is deprecated.
 */

extern jint JBNI_IMPORT Jbed_getExitCode(void);  /* System.getExitCode */

/*@}*/

/*
 * --- Scheduling ---
 */

/**
 * @name Functions for Scheduling
 *
 * Functions related to scheduling of Java threads.
 */

/*@{*/

/**
 * Scheduling timer entry point.
 *
 * Entry point for a preemptive timer that allows the Jbed scheduler to preempt
 * long-running Java threads and react to events.  The timer is controlled by
 * OPL_os_initTimer(), OPL_os_startTimer(), OPL_os_resetTimer().
 *
 * The implementation of this function just sets a flag that is checked
 * periodically by the compiled code at well-defined "preemption points".
 */

extern void JBNI_IMPORT Jbed_timerEntry(void);  /* Scheduling.timerHandler */

/*@}*/

/*
 * --- Signalling ---
 */

/**
 * @name Functions for Signalling
 *
 * Functions to send asynchronous signals to the VM.
 *
 * These functions may be called from any native task (or even an interrupt
 * handler).  In the current implementation they set a flag that will be
 * checked the next time the Jbed scheduler runs.  In the worst case this will
 * be when the Jbed native task is scheduled again after the next call to
 * Jbed_timerEntry().
 */

/*@{*/

/**
 * Send a signal to the VM.
 *
 * Send a signal to a <code>com.jbed.runtime.Signal</code> object.
 *
 * If called when no thread is waiting on the signal, the signal is stored.
 * The next thread that attempts to wait on it will return immediately.
 *
 * Signals are not counted.  If a signal is sent twice, without being handled
 * in between, one of the signals will be lost.
 *
 * @param signalPtr is the address of a <code>com.jbed.runtime.Signal</code>
 * object.  This must be a valid signal pointer (not NULL or a dangling
 * reference).  Consider that the target object must be locked on the Java heap
 * (non-movable) and must be referenced from Java so that it is not
 * garbage-collected.
 */

extern void JBNI_IMPORT Jbed_signal(
        struct INSTANCE_com_jbed_runtime_Signal_s *signalPtr); /* Signal.send */

/**
 * Send a signal to the VM.
 *
 * This is identical to Jbed_signal(), except that it takes a single
 * <code>jint</code> argument, which can be retrieved from the signal object
 * with the <code>getArg()</code> method.
 *
 * @param signalPtr like in Jbed_signal().
 * @param arg arbitrary argument.
 *
 * <b>Note</b>: The argument value is <b>not</b> queued.  Multiple calls to
 * Jbed_signalArg() will only remember the most recent argument.
 */
extern void JBNI_IMPORT Jbed_signalArg(
        struct INSTANCE_com_jbed_runtime_Signal_s *signalPtr,
        jint arg); /* Signal.sendArg */

/**
 * Send a high priority signal to the VM.
 *
 * On MVM builds, the <code>Isolate</code> responsible for handling this signal
 * will be brought into the foreground to reduce latency.
 *
 * @see Jbed_signal()
 *
 * @param signalPtr like in Jbed_signal().
 *
 */
extern void JBNI_IMPORT Jbed_signalFast(
        struct INSTANCE_com_jbed_runtime_Signal_s *signalPtr); /* Signal.sendFast */

/**
 * Send a high priority signal to the VM, with argument.
 *
 * On MVM builds, the <code>Isolate</code> responsible for handling this signal
 * will be brought into the foreground to reduce latency.
 *
 * @see Jbed_signalArg()
 *
 * @param signalPtr like in Jbed_signal().
 * @param arg arbitrary argument.
 *
 * <b>Note</b>: The argument value is <b>not</b> queued.  Multiple calls to
 * Jbed_signalArgFast() will only remember the most recent argument.
 */
extern void JBNI_IMPORT Jbed_signalArgFast(
        struct INSTANCE_com_jbed_runtime_Signal_s *signalPtr,
        jint arg); /* Signal.sendArgFast */

/*@}*/

/**
 * Send a complex upcall event to the VM.
 *
 * @param isolateId The ID of the isolate to deliver to, or #JBED_ISOLATE_ALL to broadcast.
 * @param id The ID of the event to send.
 * @param payload An arbitrary integer argument.
 * @param objPayload An optional arbitrary Object argument.
 *
 * On MVM builds, it is possible to deliver the event to a specific Isolate,
 * or to broadcast to all.
 *
 * WARNING: This method may only be called from within the same native
 * thread the VM is running in.
 */
extern void JBNI_IMPORT Jbed_sendComplexEvent(
        jint isolateId,
        jint id,
        jint payload,
        INSTANCE objPayload); /* GlobalCallHandler.deliverNativeEvent */

/*
 * --- Event delivery ---
 */
#ifdef MIDP

/**
 * @name Functions for Event Delivery
 *
 * Functions to asynchronously add events to the Jbed event queue.
 *
 * The following functions add events to the Jbed event queue.  They may be
 * called from any native task (or even an interrupt handler).  The events in
 * the queue will be handled by a Jbed event thread the next time
 * Jbed_iterate() runs (subject to Java thread scheduling).
 *
 * <b>IMPORTANT</b>: These functions are not synchronized by the VM.  The
 * caller must ensure that calls to these functions are mutually synchronized,
 * e.g. by always calling them from the same native task, or by using a
 * semaphore or some other concurrency control mechanism provided by the OS.
 */

/*@{*/

/**
 * Deliver a key down event to the Jbed event system.
 *
 * @param queueHandle A handle to the queue to send the event to.
 * @param code is the key code.
 */

extern void JBNI_IMPORT Jbed_doKeyDown(
        struct INSTANCE_com_jbed_runtime_SignalQueue_s *queueHandle,
        jint code);  /* OPLEventSystem.doKeyDown */

/**
 * Deliver a key up event to the Jbed event system.
 *
 * @param queueHandle A handle to the queue to send the event to.
 * @param code is the key code.
 */

extern void JBNI_IMPORT Jbed_doKeyUp(
        struct INSTANCE_com_jbed_runtime_SignalQueue_s *queueHandle,
        jint code);  /* OPLEventSystem.doKeyUp */

/**
 * Deliver a pointer button down event to the Jbed event system.
 *
 * @param queueHandle A handle to the queue to send the event to.
 * @param button is the button code.
 * @param x is the x coordinate of the pointer.
 * @param y is the y coordinate of the pointer.
 */

extern void JBNI_IMPORT Jbed_doPointerButtonDown(
        struct INSTANCE_com_jbed_runtime_SignalQueue_s *queueHandle,
        jint button,jint x,jint y);  /* OPLEventSystem.doPointerButtonDown */

/**
 * Deliver a pointer button up event to the Jbed event system.
 *
 * @param queueHandle A handle to the queue to send the event to.
 * @param button is the button code.
 * @param x is the x coordinate of the pointer.
 * @param y is the y coordinate of the pointer.
 */

extern void JBNI_IMPORT Jbed_doPointerButtonUp(
        struct INSTANCE_com_jbed_runtime_SignalQueue_s *queueHandle,
        jint button, jint x, jint y);  /* OPLEventSystem.doPointerButtonUp */

/**
 * Deliver a pointer movement event to the Jbed event system.
 *
 * @param queueHandle A handle to the queue to send the event to.
 * @param x is the x coordinate of the pointer.
 * @param y is the y coordinate of the pointer.
 */

extern void JBNI_IMPORT Jbed_doPointerMove(
        struct INSTANCE_com_jbed_runtime_SignalQueue_s *queueHandle,
        jint x, jint y);  /* OPLEventSystem.doPointerMove */

/**
 * Deliver a user-defined event to the Jbed event system.
 *
 * @param queueHandle A handle to the queue to send the event to.
 * @param id is the <code>UserEventHandler</code> id.
 * @param event is the event code.
 */

extern void JBNI_IMPORT Jbed_doUserEvent(
        struct INSTANCE_com_jbed_runtime_SignalQueue_s *queueHandle,
        jint id, jint event);  /* OPLEventSystem.doUserDefinedEvent */

/**
 * Deliver a media event to the Jbed event system.
 *
 * @param queueHandle A handle to the queue to send the event to.
 * @see jbed_media.h
 */

extern void JBNI_IMPORT Jbed_doAudioEvent(
        struct INSTANCE_com_jbed_runtime_SignalQueue_s *queueHandle,
        jint, jint, jint);  /* OPLEventSystem.doMediaEvent */

/**
 * Deliver a raw event to the Jbed event system. The event
 * is already encoded.
 *
 * @param queueHandle A handle to the queue to send the event to.
 * @param event The already encoded event to deliver.
 */

extern void JBNI_IMPORT Jbed_doRawEvent(
        struct INSTANCE_com_jbed_runtime_SignalQueue_s *queueHandle,
        jint event);  /* OPLEventSystem.doRawEvent */

/*@}*/
#endif

#if defined(__cplusplus)
}
#endif

#endif /* __JBED_OPL_H__ */
