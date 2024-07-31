/*[
 * Copyright 2001-2005 Esmertec AG. All Rights Reserved.
 * $Id$
]*/

/*
 * @(#)configuration.h	1.4 01/07/23
 *  Copyright (c) 2001 Sun Microsystems, Inc., 901 San Antonio Road,
 *  Palo Alto, CA 94303, U.S.A.  All Rights Reserved.
 *
 *  Sun Microsystems, Inc. has intellectual property rights relating
 *  to the technology embodied in this software.  In particular, and
 *  without limitation, these intellectual property rights may include
 *  one or more U.S. patents, foreign patents, or pending
 *  applications.  Sun, Sun Microsystems, the Sun logo, Java, KJava,
 *  and all Sun-based and Java-based marks are trademarks or
 *  registered trademarks of Sun Microsystems, Inc.  in the United
 *  States and other countries.
 *
 *  This software is distributed under licenses restricting its use,
 *  copying, distribution, and decompilation.  No part of this
 *  software may be reproduced in any form by any means without prior
 *  written authorization of Sun and its licensors, if any.
 *
 *  FEDERAL ACQUISITIONS:  Commercial Software -- Government Users
 *  Subject to Standard License Terms and Conditions
 */

/*=========================================================================
 * SYSTEM:    MIDP
 * SUBSYSTEM: Configuration property file support.
 * FILE:      configprop.h
 * OVERVIEW:  Function prototypes for accessing runtime system properties
 *
 * OVERVIEW:  This file defines the function that allows a native
 *            handler of configuration properties from the environment variables,
 *            command line arguments and preconfigured default property files
 *            to be unified in a common set of interfaces.
 *
 * USAGE:     Implementation proerties in general are not visible to the
 *            to Java application code. Properties are accessible
 *            to native code as well as Java implementation code.
 *
 * AUTHOR:    Gary Adams
 *=======================================================================*/

/*=========================================================================
 * Function prototypes for the configprop of native resources
 *=======================================================================*/

#ifndef CONFIGPROP_H
#define CONFIGPROP_H

/* Initialize the configuration subsystem */
extern void initializeConfig(char *);

#endif /* CONFIGPROP_H */
