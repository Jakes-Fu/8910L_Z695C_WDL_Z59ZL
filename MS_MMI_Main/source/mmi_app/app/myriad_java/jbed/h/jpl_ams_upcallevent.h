/*
 * Copyright 2006-2009 Myriad Group AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: AMS upcall handler events.
 * @defgroup jpl_ams_upcallevent Jbed Porting Layer: AMS upcall handler events
 *
 * This is Java porting layer (JPL) interface used to send request to the AMS
 * when the VM is running. The events are made of:
 * - The event id, which is #JBED_AMS_UPCALL_EVENT.
 * - The event type, which represents the purpose of request.
 * - The event parameters, that vary with the different event types.
 *
 * If needed, the port could broadcast the AMS request event with its
 * corresponding parameters to the VM according to the event types as below:
 *
 *
 * #JBED_AMS_EVENT_RUN_MIDLET - Event type that requests to run MIDlet
 *
 * @param storageName name of the MIDlet suite storage (string)
 * @param midletName name of the MIDlet (string)
 * @param midletNum number of the MIDlet (integer), used only when #midletName
 *                  is not specified
 *
 * @code
 * // Request AMS to run the first MIDlet of the MIDlet suite whose storage name is "s0_".
 * Jbed_upcall_queue * upcallQueue = Jbed_ams_event_getUpcallQueue();
 * if (upcallQueue != NULL)
 * {
 *     if (!Jbed_upcall_sendComplex(upcallQueue, Jbed_ams_event_getIsolateId(),
 *         JBED_AMS_UPCALL_EVENT, JBED_AMS_EVENT_RUN_MIDLET, "aai", "s0_", "", 1))
 *     {
 *        Trace("Queue full");
 *     }
 * }
 * else
 * {
 *     Trace("Queue not allocated");
 * }
 * @endcode
 *
 *
 * #JBED_AMS_EVENT_INSTALL - Event type that requests to install MIDlet
 *
 * @param installUrl url to install MIDlet (string)
 * @param localInstallUrl url to install MIDlet locally (string), only used when
 *                        #installUrl is not specified
 *
 * Optional paramertes for HTTP authentication:
 *
 * @param username the username used to authenticate (string)
 * @param password the password used to authenticate (string)
 *
 * @code
 * // Request AMS to install the MIDlet from http://midlets.com/games.jad.
 * Jbed_upcall_queue * upcallQueue = Jbed_ams_event_getUpcallQueue();
 * if (upcallQueue != NULL)
 * {
 *     if (!Jbed_upcall_sendComplex(upcallQueue, Jbed_ams_event_getIsolateId(),
 *         JBED_AMS_UPCALL_EVENT, JBED_AMS_EVENT_INSTALL, "aa", "http://midlets.com/games.jad", ""))
 *     {
 *        Trace("Queue full");
 *     }
 * }
 * else
 * {
 *     Trace("Queue not allocated");
 * }
 * @endcode
 *
 *
 * #JBED_AMS_EVENT_SHOW_AMS - Event type that requests to show AMS
 *
 * @code
 * // Request to show AMS.
 * if (!Jbed_ams_event_sendEvent(JBED_AMS_EVENT_SHOW_AMS))
 * {
 *    Trace("Queue full or not allocated");
 * }
 * @endcode
 *
 *
 * #JBED_AMS_EVENT_SHOW_TASK - Event type that requests to show task list
 *
 * @code
 * // Request to show AMS.
 * if (!Jbed_ams_event_sendEvent(JBED_AMS_EVENT_SHOW_TASK))
 * {
 *    Trace("Queue full or not allocated");
 * }
 * @endcode
 *
 *
 * #JBED_AMS_EVENT_VM_STATE - Event type that requests to switch the VM state
 *
 * @param vmState the desired VM state (integer). Must be one of #JBED_MAIN_DOWN,
 *                #JBED_MAIN_IDLE, #JBED_MAIN_ACTIVE and #JBED_MAIN_ACTIVE_FOREGROUND.
 *
 * @code
 * // Request to switch the VM state to JBED_MAIN_ACTIVE_FOREGROUND.
 * Jbed_upcall_queue * upcallQueue = Jbed_ams_event_getUpcallQueue();
 * if (upcallQueue != NULL)
 * {
 *     if (!Jbed_upcall_sendComplex(upcallQueue, Jbed_ams_event_getIsolateId(),
 *         JBED_AMS_UPCALL_EVENT, JBED_AMS_EVENT_VM_STATE, "i", JBED_MAIN_ACTIVE_FOREGROUND))
 *     {
 *        Trace("Queue full");
 *     }
 * }
 * else
 * {
 *     Trace("Queue not allocated");
 * }
 * @endcode
 *
 * #JBED_AMS_EVENT_JAR_PROGRESS - Event type that requests to display progress
 * of jar download by WAP browser
 * Note: this event is useful only when <code>AmsToolkit.wapDownloadJarFile()</code>
 * returns true.
 *
 * @param progress an Integer object showing the progress in percent
 *
 * @code
 * // Request to display progress of jar download by WAP browser.
 * Jbed_upcall_queue * upcallQueue = Jbed_ams_event_getUpcallQueue();
 * if (upcallQueue != NULL)
 * {
 *     if (!Jbed_upcall_sendComplex(upcallQueue, Jbed_ams_event_getIsolateId(),
 *         JBED_AMS_UPCALL_EVENT, JBED_AMS_EVENT_JAR_PROGRESS, "i", progress))
 *     {
 *        Trace("Queue full");
 *     }
 * }
 * else
 * {
 *     Trace("Queue not allocated");
 * }
 * @endcode
 *
 * #JBED_AMS_EVENT_JAR_DOWNLOAD_COMPLETE - Event type that notifies that jar
 * download is completed by WAP browser
 * Note: this event is useful only when <code>AmsToolkit.wapDownloadJarFile()</code>
 * returns true.
 *
 * @param result an Integer object with the InvalidJadException code for the
 * result of the download. If the download succeeds, the result is
 * <code>InvalidJadException.NO_ERROR</code>.
 *
 * @code
 * // notifies that jar download is completed by WAP browser.
 * Jbed_upcall_queue * upcallQueue = Jbed_ams_event_getUpcallQueue();
 * if (upcallQueue != NULL)
 * {
 *     if (!Jbed_upcall_sendComplex(upcallQueue, Jbed_ams_event_getIsolateId(),
 *         JBED_AMS_UPCALL_EVENT, JBED_AMS_EVENT_JAR_DOWNLOAD_COMPLETE, "i", result))
 *     {
 *        Trace("Queue full");
 *     }
 * }
 * else
 * {
 *     Trace("Queue not allocated");
 * }
 * @endcode
 *
 * If the "Queue full" message happens, increase the #JBED_AMS_NUM_EVENTS as necessary.
 *
 * @{
 */

#ifndef _JPL_AMS_UPCALLEVENT_H_
#define _JPL_AMS_UPCALLEVENT_H_

#include <jbed_jbni.h>
#include <javainstance.h>
#include <javastatic.h>
#include <javanative.h>
#include <jbed_upcall.h>

/**
 * @name Convenience constants for the AMS upcall handler events.
 * @{
 */

/**
 * Convenience short form for com::jbed::ams::AmsCallHandler.JBED_AMS_UPCALL_EVENT
 */
#define JBED_AMS_UPCALL_EVENT STATIC_com_jbed_ams_AmsCallHandler_JBED_AMS_UPCALL_EVENT

/**
 * Convenience short form for com::jbed::ams::AmsCallHandler.JBED_AMS_EVENT_RUN_MIDLET
 */
#define JBED_AMS_EVENT_RUN_MIDLET STATIC_com_jbed_ams_AmsCallHandler_JBED_AMS_EVENT_RUN_MIDLET

/**
 * Convenience short form for com::jbed::ams::AmsCallHandler.JBED_AMS_EVENT_INSTALL
 */
#define JBED_AMS_EVENT_INSTALL STATIC_com_jbed_ams_AmsCallHandler_JBED_AMS_EVENT_INSTALL

/**
* Convenience short form for com::jbed::ams::AmsCallHandler.JBED_AMS_EVENT_REMOVE
*/
#define JBED_AMS_EVENT_REMOVE STATIC_com_jbed_ams_AmsCallHandler_JBED_AMS_EVENT_REMOVE

/**
 * Convenience short form for com::jbed::ams::AmsCallHandler.JBED_AMS_EVENT_SHOW_AMS
 */
#define JBED_AMS_EVENT_SHOW_AMS STATIC_com_jbed_ams_AmsCallHandler_JBED_AMS_EVENT_SHOW_AMS

/**
 * Convenience short form for com::jbed::ams::AmsCallHandler.JBED_AMS_EVENT_SHOW_TASK
 */
#define JBED_AMS_EVENT_SHOW_TASK STATIC_com_jbed_ams_AmsCallHandler_JBED_AMS_EVENT_SHOW_TASK

/**
 * Convenience short form for com::jbed::ams::AmsCallHandler.JBED_AMS_EVENT_VM_STATE
 */
#define JBED_AMS_EVENT_VM_STATE STATIC_com_jbed_ams_AmsCallHandler_JBED_AMS_EVENT_VM_STATE

/**
 * Convenience short form for com::jbed::ams::AmsCallHandler.JBED_AMS_EVENT_JAR_PROGRESS
 */
#define JBED_AMS_EVENT_JAR_PROGRESS STATIC_com_jbed_ams_AmsCallHandler_JBED_AMS_EVENT_JAR_PROGRESS

/**
 * Convenience short form for com::jbed::ams::AmsCallHandler.JBED_AMS_EVENT_JAR_DOWNLOAD_COMPLETE
 */
#define JBED_AMS_EVENT_JAR_DOWNLOAD_COMPLETE STATIC_com_jbed_ams_AmsCallHandler_JBED_AMS_EVENT_JAR_DOWNLOAD_COMPLETE

/**
 * Convenience short form for com::jbed::ams::AmsCallHandler.JBED_AMS_EVENT_LIST_RUNNING_MIDLETS
 */
#define JBED_AMS_EVENT_LIST_RUNNING_MIDLETS STATIC_com_jbed_ams_AmsCallHandler_JBED_AMS_EVENT_LIST_RUNNING_MIDLETS

/**
 * Convenience short form for com::jbed::ams::AmsCallHandler.JBED_AMS_EVENT_KILL_MIDLET
 */
#define JBED_AMS_EVENT_KILL_MIDLET STATIC_com_jbed_ams_AmsCallHandler_JBED_AMS_EVENT_KILL_MIDLET

/**
 * Convenience short form for com::jbed::ams::AmsCallHandler.JBED_AMS_EVENT_ACTIVE_MIDLET
 */
#define JBED_AMS_EVENT_ACTIVE_MIDLET STATIC_com_jbed_ams_AmsCallHandler_JBED_AMS_EVENT_ACTIVE_MIDLET

/**
 * Convenience short form for com::jbed::ams::AmsCallHandler.JBED_AMS_EVENT_ACTIVITY_MENU
 */
#define JBED_AMS_EVENT_ACTIVITY_MENU STATIC_com_jbed_ams_AmsCallHandler_JBED_AMS_EVENT_ACTIVITY_MENU

/**
 * Convenience short form for com::jbed::ams::AmsCallHandler.JBED_AMS_EVENT_BATTERY_STATUS_CHANGED
 */
#define JBED_AMS_EVENT_BATTERY_STATUS_CHANGED STATIC_com_jbed_ams_AmsCallHandler_JBED_AMS_EVENT_BATTERY_STATUS_CHANGED

/**
 * Maximum number of characters that a URL may have.
 * A port can override this value at the OS level in os_def.h or at the project
 * level in targ_def.h.
 */
#ifndef JBED_MAX_URL_LEN
#define JBED_MAX_URL_LEN 1024
#endif

/**
 * Size of an event in words. The largest complex event is made of 2 strings that are all
 * maximum #JBED_MAX_URL_LEN characters long, and 1 integer.
 * See Jbed_upcall_createQueue() on how to calculate the size of a complex event.
 */
#define JBED_AMS_EVENT_SIZE (JBED_UPCALL_MINEVENTSIZE + 2*JBED_UPCALL_OBJECTHEADERSIZE + 2*JBED_UPCALL_STRINGSIZE(JBED_MAX_URL_LEN) + 1)

/**
 * The priority of the AMS event queue (bigger number means higher priority).
 * A port can override this value at the OS level in os_def.h or at the project
 * level in targ_def.h.
 */
#ifndef JBED_AMS_EVENT_QUEUE_PRIORITY
#define JBED_AMS_EVENT_QUEUE_PRIORITY 0
#endif

/**
 * Maximum number that the upcall handler event queue needs to be able to hold.
 * A port can override this value at the OS level in os_def.h or at the project
 * level in targ_def.h.
 */
#ifndef JBED_AMS_NUM_EVENTS
#define JBED_AMS_NUM_EVENTS 1
#endif

/**
 * Required size of the upcall handler event queue in words.
 * See Jbed_upcall_createQueue() on how to calculate the size of a queue.
 */
#define JBED_AMS_EVENT_QUEUE_SIZE (JBED_UPCALL_SPAREQUEUESPACE + (JBED_AMS_NUM_EVENTS * JBED_AMS_EVENT_SIZE))

/* @} */

/**
 * @name Convenience short form macros for the native methods.
 * @{
 */

/**
 * Convenience short form for com::jbed::ams::AmsCallHandler.nativeAmsListenerUp()
 */
#define JPL_ams_event_nativeAmsListenerUp JbniS_com_jbed_ams_AmsCallHandler_nativeAmsListenerUp

/**
 * Convenience short form for com::jbed::ams::AmsCallHandler.nativeAmsListenerDown()
 */
#define JPL_ams_event_nativeAmsListenerDown JbniS_com_jbed_ams_AmsCallHandler_nativeAmsListenerDown

/* @} */


#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @name Convenience functions to send AMS upcall event.
 * @{
 */

/**
 * Get the AMS isolate ID.
 *
 * @return the AMS isolate ID.
 */
int32_t Jbed_ams_event_getIsolateId(void);

/**
 * Get the AMS event upcall queue.
 *
 * @return upcall queue for AMS event
 */
Jbed_upcall_queue * Jbed_ams_event_getUpcallQueue(void);

/**
 * Send upcall event to AMS.
 *
 * @param eventType the AMS event type
 * @return JBNI_TRUE if succeed or JBNI_FALSE if the queue is full or not allocated.
 */
bool_t Jbed_ams_event_sendEvent(int32_t eventType);

/**
 * Request a VM to install from local file.
 *
 * Should be called by porting code when clicking a jad/jar file in local file system.
 * This function will triger event:JBED_AMS_EVENT_INSTALL
 *
 * @param fileUrl url encoded with utf8 for install.
 */
void Jbed_ams_event_requestInstall(const char* fileUrl);
/**
 * Request a VM to install from local file.
 *
 * Should be called by porting code when clicking a jad/jar file in local file system.
 * This function will triger event:JBED_AMS_EVENT_INSTALL
 *
 * @param fileUrl url encoded with utf8 for install.
 * @param originalUrl original url encoded with utf8 for install.
 */
void Jbed_ams_event_requestLocalInstall(const char* fileUrl, const char* originalUrl);

/**
 * Request a VM to remove midlet.
 *
 * Should be called by porting code when removing midlet.
 * This function will triger event:JBED_AMS_EVENT_REMOVE
 *
 * @param storageName encoded with ASCII for install.
 */
void Jbed_ams_event_requestRemove(const char* storageName);

/* @} */

/* @} */

#if defined(__cplusplus)
}
#endif

#endif /* _JPL_AMS_UPCALLEVENT_H_ */

