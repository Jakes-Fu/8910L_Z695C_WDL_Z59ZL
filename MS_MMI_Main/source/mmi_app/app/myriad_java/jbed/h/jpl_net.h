/*
 * Copyright 2005-2008 Esmertec AG. All Rights Reserved.
 * $Id$
 */

#ifndef _JPL_NET_H_
#define _JPL_NET_H_

/**
 * FIXME: The functions below are not really JPL since they are not required
 * to be implemented by port.  It would be more appropriate to use "Jbed_"
 * prefix for them.
 */

/**
 * Initialise the network subsystem.
 * Must be called within an asynchronous context.
 * Multiple simultaneous calls will be queued with ASYNC_callAgain().
 * @returns CPL_NET_SUCCESS when the network subsystem is initialised.
 *          CPL_NET_ERROR if initialisation failed.
 *          CPL_NET_WOULDBLOCK if initialisation is pending.
 */
extern int JPL_net_startup(void);

/**
 * If the network has been initialised, call CPL_net_shutdown()
 */
extern void JPL_net_shutdown(void);

/**
 * The network has been shut down by the platform.
 */
extern void Jbed_net_notifyNetworkDown(void);

#endif /* _JPL_NET_H_ */
