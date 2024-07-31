/*
 * Copyright 2004-2008 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Asynchronous I/O support (FastBCC version)
 *
 * These macros allow native code to interact with the VM for asynchronous data
 * transfers.
 *
 * Class <code>com.jbed.runtime.AsyncIO</code> provides the Java counterpart
 * for implementing asynchronous transfers. The Java side uses the following
 * pattern to perform an I/O operation:
 * \code
 * int retVal = 0;
 * while(AsyncIO.loop()) {
 *     // call the native method
 *     retVal = lookupHost(hostName, ipAddr);
 * }
 * \endcode
 *
 * A native function is free to either implement synchronous or asynchronous
 * I/O, without the need to change the Java side. In the case of synchronous
 * I/O, none of the macros defined in this file are needed. The framework
 * guarantees that the native function is called exactly once.
 *
 * For asynchronous I/O, the native function is called multiple times. In the
 * first call, the native function will initiate the I/O operation and call
 * ASYNC_callAgainWhenSignalled() to request that the Java side calls again
 * once the transfer is completed.
 *
 * This is an example of a native function with asynchronous I/O:
 * \code
 * int JBEDEXPORT lookupHost(jobject name, jobject ipaddress)
 * {
 *     int result = -1; // assume failure
 *     int handle = 0;
 *     char buffer[128]; // small enough to fit on Java native stack
 *
 *     if (ASYNC_firstCall()) {
 *         Jbni_getStringAscii(name, buffer, sizeof(buffer));
 *         handle = sendLookupRequest(buffer);
 *         if (handle != error) {
 *             *(int *)(ASYNC_getCurrentState()) = handle;
 *             // platform-specific
 *             ASYNC_signalWhenLookupResponseArrives(handle);
 *             ASYNC_callAgainWhenSignalled();
 *         }
 *     } else { // call on completion
 *         handle = *(int *)(ASYNC_getCurrentState());
 *         result = getLookupResponse(handle);
 *         if (result >= 0) { // success?
 *             // store response in ipaddress
 *             ...
 *         }
 *     }
 *     return result; // result >= 0 on success, < 0 on error
 * }
 * \endcode
 *
 * Note that the function ASYNC_signalWhenLookupResponseArrives() used above is
 * platform specific. It registers the asynchronous I/O operation with the OS
 * event dispatcher, causing the Java thread which waits for the completion of
 * the asynchronous I/O to be notified (with ASYNC_notify()). A platform port will offer several of
 * these functions to cause notification after various asynchronous events. By
 * convention, the names of these functions start with ASYNC_signalWhen...().
 *
 * This is an example of what ASYNC_signalWhenLookupResponseArrives() could
 * look like:
 * \code
 * void ASYNC_signalWhenLookupResponseArrives(int handle)
 * {
 *     // add to list of expected events
 *     addDnsEventListener(handle, ASYNC_getNotifier());
 * }
 * \endcode
 *
 * This is an example fragment of an OS event dispatcher which notifies the
 * Java thread:
 * \code
 * ...
 * event = getEvent();
 * switch(event->type) {
 *     ...
 *     case DNS_RESPONSE:
 *         // find and remove an event listener for the event's handle
 *         notifier = findDnsEventListener(event->handle);
 *         if (notifier != NULL) {
 *             ASYNC_notify(notifier);
 *         }
 *         break;
 *     ...
 * \endcode
 */

#ifndef __JBED_OPL_ASYNC_H__
#define __JBED_OPL_ASYNC_H__

#include <jbed_types.h>
#include <jbed_vm_common.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @name Size of the async state.
 */

/*@{*/
#ifndef ASYNC_STATE_SIZE
#define ASYNC_STATE_SIZE 128
#endif
/*@}*/

struct ASYNC_byteArrayStruct
{
    int8_t b1[16];
    int8_t data[1];
    char b2[3];
};

typedef struct ASYNC_byteArrayStruct *ASYNC_BYTEARRAY;

struct ASYNC_INSTANCE_Signal_s
{
    int8_t b1[36];
    int32_t state;
    int8_t b2[12];
    ASYNC_BYTEARRAY asyncBuffer;
    int32_t i;
};


#include <jbedexpupcalls.h>

/**
 * Jbed notification type.
 */

/**
 * The Java notifier type.
 */
typedef struct ASYNC_INSTANCE_Signal_s *ASYNC_javaNotifier;

typedef struct ASYNC_notifier_s ASYNC_notifier_t;
typedef ASYNC_notifier_t *ASYNC_notifier;
typedef void ASYNC_notifierFunc(ASYNC_notifier);

/**
 * The notifier struct.
 * The first three fields match the state, data and asyncTimeout fields
 * from the Java signal type. This allows us to point an ASYNC_Notifier
 * at the relevant part of the Java structure. The async state buffer
 * is different for the two structures so has to be special cased in
 * ASYNC_getStateFromNotifier().
 * A Java notifier can be identified by the notifierFunc being NULL.
 */
struct ASYNC_notifier_s
{
    int32_t asyncIOState;
    ASYNC_notifierFunc *notifierFunc;
    ASYNC_notifierFunc *invalidateFunc;
    int32_t timeout;
    char stateBuffer[ASYNC_STATE_SIZE];
    int32_t notified;
};

/**
 * The current notifier, or NULL to use the notifier out of the current
 * thread.
 * \hideinitializer
 */

extern int32_t JbniF_com_jbed_runtime_AsyncIO_ASYNC_currentNotifier;
#define ASYNC_currentNotifier (JbniF_com_jbed_runtime_AsyncIO_ASYNC_currentNotifier)

/**
 * Set the function which will invalidate the notifier.
 *
 * The corresponding Java object of a notifier might get removed on the Java
 * heap. To prevent native code from calling into such objects notifiers are
 * invalidated when leaving the AsyncIO loop or when its isolate is killed.
 * IO operations should register a callback function here to get informed about
 * the invalidation and they must not access the notifier anymore after the
 * registered callback was called.
 * \hideinitializer
 */
#define ASYNC_setInvalidateFunc(func) \
    ASYNC_getNotifier()->invalidateFunc = func

/**
 * @name Asynchronous I/O states
 *
 * These values indicate the state of an asynchronous I/O operation. Typically,
 * there's no need for using these definitions directly. Native methods should
 * use ASYNC_firstCall() and ASYNC_timeout() to extract relevant state
 * information.
 */

/*@{*/
/** \hideinitializer */
#define ASYNC_IO_IDLE     0
/** \hideinitializer */
#define ASYNC_IO_BUSY     1
/** \hideinitializer */
#define ASYNC_IO_DONE0    2
/** \hideinitializer */
#define ASYNC_IO_DONE     3
/** \hideinitializer */
#define ASYNC_IO_TIMEOUT  4
/** \hideinitializer */
#define ASYNC_IO_LOOP     5
/*@}*/

/**
 * @name Macros for native I/O functions
 */
/*@{*/

/**
 * Arrange to call the native method again.
 * \hideinitializer
 */
#define ASYNC_callAgain() \
    (ASYNC_getNotifier()->asyncIOState = ASYNC_IO_LOOP)

/**
 * Cause the Java thread to wait until it is notified that the I/O operation
 * has been completed. The native method will be called again as soon as the
 * Java thread has been notified. This must always be used in conjunction with
 * one of the platform-specific ASYNC_signalWhen...() functions.
 * \hideinitializer
 */
#define ASYNC_callAgainWhenSignalled() \
    (ASYNC_getNotifier()->asyncIOState = ASYNC_IO_BUSY)

/**
 * Like ASYNC_callAgainWhenSignalled(), but wake up the Java thread if the
 * native I/O operation does not complete within the given time. In the
 * following call to the native method, ASYNC_timeout() will be true.
 *
 * @param timeout The timeout in milliseconds after which the asynchronous I/O
 * operation is to be aborted.
 * \hideinitializer
 */
#define ASYNC_callAgainWhenSignalledOrTimeout(t) \
    do { \
        ASYNC_notifier s = ASYNC_getNotifier(); \
        s->timeout = (t); \
        s->asyncIOState = ASYNC_IO_BUSY; \
    } while(0)

/**
 * Evaluates to true in the first call to the native method.  Truly
 * asynchronous native methods query this to find out whether to initiate an
 * I/O (on the first call) or transfer resulting data back to Java (in
 * consecutive calls).
 *
 * @return TRUE if this is the first call to the native method for the current
 * asynchronous I/O operation, FALSE otherwise.
 * \hideinitializer
 */
#define ASYNC_firstCall() \
    (ASYNC_getNotifier()->asyncIOState == ASYNC_IO_DONE0)

/**
 * Check if a call that could require asynchronous completion has asked to
 * be called again later.
 *
 * @return TRUE if any of the ASYNC_callAgain... functions have been called.
 */
bool_t ASYNC_pendingCall(void);

/**
 * Evaluates to true if the Java thread has been woken up by a timeout (as
 * opposed to notification on completion). This can only happen if a timeout
 * has been set up previously with ASYNC_callAgainWhenSignalledOrTimeout().
 *
 * @return TRUE if the asynchronous I/O is to be aborted due to a timeout,
 * otherwise FALSE.
 * \hideinitializer
 */
#define ASYNC_timeout() \
    (ASYNC_getNotifier()->asyncIOState == ASYNC_IO_TIMEOUT)

/**
 * Get the async call state buffer for the current I/O operation.
 *
 * Can be called from an async IO native call. The current AsyncIO
 * notifier is fetched from the current thread.
 *
 * @return A pointer to the async state buffer for the current I/O operation.
 * \hideinitializer
 */
#define ASYNC_getCurrentState() \
    (ASYNC_getStateFromNotifier(ASYNC_getNotifier()))

/**
 * Get the async call state buffer for a particular I/O operation.
 *
 * Can be called from any code with a reference to an async IO notifier.
 *
 * @return A pointer to the async state buffer for the I/O operation
 * of the passed in notifier.
 */
void *ASYNC_getStateFromNotifier(ASYNC_notifier notifier);

/*@}*/


/**
 * @name Macros for platform specific signalling functions
 */
/*@{*/
/**
 * Gets the asynchronous I/O notifier for the current Java thread. This is
 * typically used within the platform-specific ASYNC_signalWhen...() functions
 * in order to register the awaited event together with the appropriate
 * notifier.
 *
 * This macro may not be used in the OS event dispatcher, since at that point,
 * the current Java thread is most likely not the thread that needs to be
 * notified.
 *
 * N.B. The default behaviour when returning from an asynchronous native method,
 * is that the Java thread continues normally, without waiting. If either
 * ASYNC_callAgainWhenSignaled or ASYNC_callAgainWhenSignalledOrTimeout are
 * called in the native, on returning the Java thread will wait until the
 * notifier returned here, is signaled with ASYNC_notify.
 *
 * @return The asynchronous notifier associated with the current Java thread as
 * a void *.
 * \hideinitializer
 */
#define ASYNC_getNotifier() \
    ((ASYNC_currentNotifier != 0) ? \
            (ASYNC_notifier)ASYNC_currentNotifier : \
            (ASYNC_notifier)(&(JbedExp_getCurrentSignal()->state)))

/**
 * Wakes up the waiting Java thread associated with the given notifier. This
 * macro is to be used in the OS event dispatcher.
 *
 * @param notifier A notifier previously obtained through ASYNC_getNotifier().
 */
void ASYNC_notify(ASYNC_notifier notifier);
/*@}*/

/**
 * Wakes up the waiting Java thread associated with the given notifier. This
 * should be used for high priority events which require low latency only.
 * The VM may boost the receiver of this event to reduce latency.
 *
 * @param notifier A notifier previously obtained through ASYNC_getNotifier().
 */
void ASYNC_notifyFast(ASYNC_notifier notifier);
/*@}*/

typedef struct ASYNC_callbackNotifier_s ASYNC_callbackNotifier_t;
typedef ASYNC_callbackNotifier_t *ASYNC_callbackNotifier;

/**
 * A callback function called to advance the state of a callback notifier.
 *
 * The callback is called from a safe context (no restrictions on what OS APIs
 * the code can call), as soon as possible after ASYNC_notify() is called on
 * the notifier that this callback is associated with.
 *
 * The callback should do whatever is required by the asynchronous operation
 * that is in progress. Usually this will mean calling back to the porting
 * layer to retrieve the result of the operation or start the next step.
 *
 * The callback returns JBNI_TRUE if it does not expect to be called again (the
 * operation is complete). The notifier will be removed from the system's list
 * of callback notifiers and the callback will not be called again. In this
 * case, the system will not access the callback notifier again, allowing the
 * callback to free the memory before returning.
 *
 * The callback should never return JBNI_TRUE if one of the ASYNC_callAgain...
 * functions has been called.
 *
 * If the callback returns JBNI_FALSE then the notifier with remain on the
 * system's list. One of the ASYNC_callAgain... function must have been called
 * prior to returning.
 *
 * If the callback needs to start a new asynchronous operation it may reuse the
 * same notifier but it must call ASYNC_initCallbackNotifier() again to reset
 * the asyncIO state machine. If this is not done then ASYNC_firstCall() will
 * not return the expected value at the start of the new operation.
 *
 * It is safe for the callback to call ASYNC_initCallbackNotifier() to
 * reinitialise the notifier but then to decide it does not need to be
 * called again. In this case is it not necessary to call
 * ASYNC_deregisterCallbackNotifier(), the callback simply needs to
 * return JBNI_TRUE.
 */
typedef bool_t ASYNC_callback(ASYNC_callbackNotifier, void *);

/**
 * A callback notifier - a standard notifier plus the additional state
 * required to handle callbacks.
 */
struct ASYNC_callbackNotifier_s
{
    /* Link to next notifier in the list */
    ASYNC_callbackNotifier next;
    /* The handle to pass to the callback function */
    void *callbackHandle;
    /* The callback function to call after notification */
    ASYNC_callback *callback;
    /* The relative time (as returned by CPL_os_getRelativeMillis()) when the
     * notifier will enter timeout state. Or 0 if it never times out.
     */
    int64_t timeoutTime;
    /* The standard notifier to be passed to normal ASYNC functions. */
    ASYNC_notifier_t notifier;
};

/**
 * Initialise a callback notifier.
 * The notifier is initialised with the supplied callback function and added to
 * the system's internal list of callback notifiers.
 * If the notifier is already in the list then it is simply reset, ready to
 * start a new asynchronous operation.
 *
 * @param np  the notifier
 * @param callback  the callback function
 * @param handle  the handle to pass to the callback
 * @return a pointer to the standard notifier within the callback notifier.
 *         This is the notifier that should be used with subsequent async io
 *         operations.
 */
ASYNC_notifier ASYNC_initCallbackNotifier(
        ASYNC_callbackNotifier np,
        ASYNC_callback *callback,
        void *handle);

/**
 * Deregister a callback notifier.
 * Used to inform the system that a callback notifier will not be used. Must
 * only be called when no asynchronous operations are using the notifier.
 */
void ASYNC_deregisterCallbackNotifier(ASYNC_callbackNotifier np);

/**
 * Handle callback notifiers, calling callbacks if required.
 * Usually called from the VM's main loop but may be called from any safe
 * place. i.e. any foreground context. If callback notifiers are used whilst
 * the VM is not running then port code must ensure that this function is
 * called regularly.
 */
void ASYNC_handleCallbacks(void);

/**
 * Compute the minimum delay when ASYNC_handleCallbacks() should be called
 * again.
 * @return The minimum time in ms before it should be called again or 0 to
 *         be called again soon or INT_MAX if it doesn't need to be called
 *         until an asynchronous event occurs.
 */
int32_t ASYNC_nextUpdateDelay(void);

/**
 * Set the current notifier (in special situations). Use as:
 *
 * \code
 * ASYNC_SET_CURRENT_NOTIFIER(notifier)
 * {
 *     async operations
 * }
 * ASYNC_END_CURRENT_NOTIFIER
 * \endcode
 *
 * Any calls to ASYNC_getNotifier() within the block or any functions called
 * from there will return the specified notifier.
 * \hideinitializer
 */
#define ASYNC_SET_CURRENT_NOTIFIER(np) \
{ \
    jint ASYNC__oldNotifier = ASYNC_currentNotifier; \
    ASYNC_currentNotifier = (jint)(np);

/**
 * End a region where the notifier was set explicitly.
 * See #ASYNC_SET_CURRENT_NOTIFIER.
 * \hideinitializer
 */
#define ASYNC_END_CURRENT_NOTIFIER \
    ASYNC_currentNotifier = ASYNC__oldNotifier; \
}

#if defined(__cplusplus)
}
#endif

#endif /* __JBED_OPL_ASYNC_H__ */
