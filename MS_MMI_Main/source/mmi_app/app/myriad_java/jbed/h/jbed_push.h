/*
 * Copyright 2005-2009 Myriad Group AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: PushRegistry support (VM-independent)
 *
 * This interface defines the calls <em>into</em> the push the registry
 * The push registry provides a C based VM-lifetime-independent database
 * of network, SMS ports, CBS channels and MMS application ID opened for
 * push and manages connection to those.
 * It also supports ALARM push support to wake up a MIDLet at a given time.
 * <!-- #interface list begin -->
 * \section pushregistry_interface Interface (global)
 *    - Jbed_push_handle_boot()
 *    - Jbed_push_handle_vm_start()
 *    - Jbed_push_handle_vm_exit_or_reset()
 *    - Jbed_push_handle_shutdown()
 *    - Jbed_push_datagram_get_open_port()
 *    - Jbed_push_datagram_recv()
 *    - Jbed_push_socket_get_open_port()
 *    - Jbed_push_socket_accept()
 *    - Jbed_push_socket_get_name()
 *    - Jbed_push_hand_back()
 *    - Jbed_push_get_next_alarm_time()
 * \section pushregistry_interface_trigger Functions to handle pending push events
 *    - Jbed_push_io_activity()
 *    - Jbed_push_check_next_push()
 * <!-- #interface list end -->
 *
 * PushRegistry support is mandatory for MIDP 2.0 builds of Jbed though no
 * protocol or alarm support is mandated.
 * For MIDP 2.0 builds of Jbed, define the symbol <b>MIDP2_PUSH_SUPPORT</b>
 * if any PushRegistry protocol or alarms are implemented.
 */

#ifndef JBED_PUSH_H
#define JBED_PUSH_H

#include <pushregistry.h>

#if defined (__cplusplus)
extern "C" {
#endif

/**
 * @name Functions that need to be called during VM life cycle.
 *
 * These hooks must be invoked at the indicated points in VM life cycle.
 * This allows the push registry to do the appropriate operations for
 * the state change.
 */
/*@{*/
/**
 * Start push registry
 *
 * Setup (and polulate) the persistent data structures in the
 * push registry.
 *
 * This function must be called with do_listen=JBNI_TRUE once at the
 * start of the (main) VM task/process/program (i.e. after a reboot of
 * the phone). It will allocate memory and use the APIs in cpl_push.h
 * (this usually implies file system access).
 *
 * After reading in the remembered push entries this will open the
 * requested connections for listening and the system will start
 * reacting to push events. For debugging purposes setting do_listen to
 * false will disable this functionality (thus disabling the actual
 * push entries) until Jbed_push_handle_boot
 *
 * @param do_listen if JBNI_FALSE, only read in data structures
 */
void Jbed_push_handle_boot(unsigned int do_listen);

/**
 * Inform the push registry that a VM instance is being started
 *
 * In this call any registered ports will be opened if not opened
 * already. A call to this function must be made at the start of very
 * run of a the VM. In particular when restarting the VM after a VM
 * reset.
 *
 * This function expects Jbed_push_handle_boot to have been called before.
 */
void Jbed_push_handle_vm_start(void);

/**
 * Inform the push registry that a VM instance has exited (incl for reset)
 *
 * This funtion must be called after a run of the VM has completed.
 * In particular this includes running after a VM has exited for reset.
 */
void Jbed_push_handle_vm_exit_or_reset(void);

/**
 * Terminate the push subsystem.
 *
 * This will free resources and close any open ports that the push subsystem
 * was listenening on.
 *
 * In a system with a dedicated VM task or application layer task
 * that never exits, this function will never be called.
 */
void Jbed_push_handle_shutdown(void);

/**
 * Hand back an open connection to the push registry.
 *
 * This should be called when a midlet closes a connection previously
 * retrieved using
 *   - Jbed_push_sms_get_open_port()
 *   - Jbed_push_mms_get_open_port()
 *
 * @return JBNI_TRUE on success, JBNI_FALSE on failure
 */
unsigned int Jbed_push_hand_back(PushEntry *p);
/*@}*/

/**
 * @name Managing inbound push events
 */
/*@{*/
#define JBED_PUSH_START_VM     (-1) /**< Push event pending, start a VM instance */
#define JBED_PUSH_SLEEP_INFINITE (-2) /**< No pending push, sleep until next */

/**
 * Notify push registry of activity
 *
 * If any event (i.e. an inbound SMS message) occurred that could
 * cause a push midlet to wakeup this function must be called.
 */
void Jbed_push_io_activity(void);

/**
 * Check for pending push events.
 *
 * This function must be called when the VM is NOT running to handle incoming
 * push events. It must be called
 *    - Once after the VM has stopped running
 *    - After an inbound event, e.g. SMS message, which might trigger push.
 *
 * It is safe to call this more often that required.
 *
 * If JBED_PUSH_START_VM is returned the VM must be started soon.
 * For JBED_PUSH_SLEEP_INFINITE, sleep until next inbound event.
 *
 * @return JBED_PUSH_START_VM or JBED_PUSH_SLEEP_INFINITE.
 */
int32_t Jbed_push_check_next_push(void);

#ifdef SUPPORTS_PUSH_ALARM
/**
 * Get the time interval to next alarm push, in milli-seconds
 *
 * This function is called to get the interval from now to next push alarm.
 *
 * @return the interval from now to next push alarm, in milli-seconds, or
 * INT64_MAX to indicate there is no alarm push registered, or 0 to indicate an
 * already delayed or just on time alarm event.
 */
int64_t Jbed_push_get_next_alarm_time(void);
#endif // SUPPORTS_PUSH_ALARM

/*@}*/


/**
 * @defgroup midletinfoflags MIDlet information mask flags
 *
 * These flags indicate specific properties of the midlet requesting
 * particular info
 *
 * These flags currently only exposed as the lower 16 bits of the
 * midlet info word in cpl_wma.h.
 *
 * @internal
 * They are also stored in the push registry.
 */

/**
 * @name Generic MIDlet property flags
 * @ingroup midletinfoflags
 */
/*@{*/
/** MIDlet suite has authenticated signature \hideinitializer */
#define JBED_MIDLET_INFO_AUTHENTICATED STATIC_com_jbed_ams_MIDletSuite_MIDLET_INFO_AUTHENTICATED
/*@}*/

/**
 * Maximum number of the push events that can be cached before VM is started.
 */
#ifndef JBED_PUSH_EVENT_CACHE_SIZE
#define JBED_PUSH_EVENT_CACHE_SIZE 20
#endif

/**
 * Maximum number that the push upcall queue needs to be able to hold.
 */
#ifndef JBED_PUSH_EVENT_NUM
#define JBED_PUSH_EVENT_NUM 20
#endif

/**
 * The priority of the push event queue. Bigger number means higher priority.
 */
#ifndef JBED_PUSH_EVENT_PRIORITY
#define JBED_PUSH_EVENT_PRIORITY 0
#endif

/* Required size of the upcall handler event queue in words.*/
#define JBED_PUSH_EVENT_QUEUE_SIZE (JBED_UPCALL_SPAREQUEUESPACE + (JBED_PUSH_EVENT_NUM * JBED_UPCALL_MINEVENTSIZE))

/**
  * @name Convenience constants
  * @{
  */

/**
  * Convenience short form for com::jbed::ams::PushRegistryCallHandler.JBED_PUSH_SEND_PUSHEVENT
  */
#define JBED_PUSH_SEND_PUSHEVENT STATIC_com_jbed_ams_PushRegistryCallHandler_JBED_PUSH_SEND_PUSHEVENT
/* @} */

/**
 * @name Convenience functions to manage the event queue.
 * @{
 */

/**
  * Opens the queue for sending events.
  *
  * @param size The size of the queue to be created.
  * @param priority The priority of the queue.
  */
void Jbed_push_openQueue(int32_t size, int32_t priority);

/**
  * When a new push event arrives, if the VM is running, send an upcall event to VM;
  * if the VM is not running, only set some necessary flags. <code>Jbed_push_check_next_push</code>.
  *
  * @param pe a pointer to a PushEntry.
  */
void Jbed_push_sendPushEvent(PushEntry *pe);

/**
  * Same as Jbed_push_sendPushEvent() except the return value
  *
  * @param pe a pointer to a PushEntry.
  * @return TRUE if send an upcall on success while vm is running, otherwise FALSE
  */
bool_t Jbed_push_sendPushEventEx(PushEntry *pe);
/** Closes the push event queue. */
void Jbed_push_closeQueue(void);

/* @} */

#if defined(__cplusplus)
}
#endif

#endif /* JBED_PUSH_H */
