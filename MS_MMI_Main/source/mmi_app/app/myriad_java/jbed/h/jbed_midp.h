/*
 * Copyright 2003-2004 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: MIDP Specifics (FastBCC legacy)
 *
 * This file defines some MIDP-specific constants.
 */

#ifndef _JBED_MIDP_H_
#define _JBED_MIDP_H_

#include <jbed_jbni.h>

/**
 * @name Return Values for VM
 *
 * These are special exit codes returned by Jbed_getExitCode() when using the
 * MIDPToolkit together with the AMS, Jam or TCKRunnerMIDP.
 */

/*@{*/

/** Restart the VM. */
#define JBED_RESTART                    ((jint)1)

/** Restart the VM (a MIDlet has finished executing). */
#define JBED_EXIT_MIDLET                ((jint)2)

/** Exit the VM. */
#define JBED_EXIT_VM                    ((jint)3)

/** Exit the VM (an install has failed). */
#define JBED_EXIT_VM_INSTALL_FAILED     ((jint)4)

/** Exit the VM (an install has succeeded). */
#define JBED_EXIT_VM_INSTALL_SUCCEEDED  ((jint)5)

/** Not used (this is obsolete). */
#define JBED_MIDP_SUSPENDED             ((jint)6)

/*@}*/

#endif /* _JBED_MIDP_H_ */
