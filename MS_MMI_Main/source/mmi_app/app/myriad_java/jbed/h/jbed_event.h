/*
 * Copyright 2003-2005 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Event system (FastBCC legacy)
 *
 * Porting interface for Jbed events.
 *
 * These methods must be implemented to start and stop delivery of events to
 * the VM.
 *
 * The OPL_event_start() method passes down the event queue to use
 * for delivery of events to the VM, and indicates that event delivery can
 * start now.
 *
 * The OPL_event_stop() indicates that no more events should be
 * delivered to the VM.
 *
 * After OPL_event_start() has been called, and before
 * OPL_event_stop() is called, events can be delivered to the VM
 * using the <code>Jbed_do*</code> event methods defined in jbed_opl.h.
 *
 * <!-- #interface list begin -->
 * \section jbed_event_interface Interface
 * - OPL_event_start()
 * - OPL_event_stop()
 * <!-- #interface list end -->
 */

#ifndef _JBED_EVENT_H_
#define _JBED_EVENT_H_

#include <jbed_jbni.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* Include definitions of event delivery upcalls */
#include <jbed_opl.h>

/**
 * Start delivery of events. The passed reference to the event queue should
 * be used to deliver events.
 *
 * @param queue The queue to use for delivery of events.
 */
void JBNI_EXPORT OPL_event_start(struct INSTANCE_com_jbed_runtime_SignalQueue_s *queue);

/**
 * Stop delivery of events. No more events should be delivered.
 */
void JBNI_EXPORT OPL_event_stop(void);

#if defined(__cplusplus)
}
#endif

#endif /* _JBED_EVENT_H_ */
