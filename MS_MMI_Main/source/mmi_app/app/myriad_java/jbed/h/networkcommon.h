/*
 * Copyright 2004-2007 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Unification: Networking
 *
 */

#ifndef _NETWORK_COMMON_H_
#define _NETWORK_COMMON_H_

/*------- Includes ---------------------------------------------------------*/
#include <cpl_net.h>
#include <jpl_net.h>
#include <javastatic.h>
#include <javainstance.h>

/*------- Constants --------------------------------------------------------*/
#define ILLEGALARGUMENT_EXCEPTION STATIC_com_sun_cldc_io_ConnectionBase_ILLEGALARGUMENT_EXCEPTION
#define NULLPOINTER_EXCEPTION STATIC_com_sun_cldc_io_ConnectionBase_NULLPOINTER_EXCEPTION
#define INDEXOUTOFBOUNDS_EXCEPTION STATIC_com_sun_cldc_io_ConnectionBase_INDEXOUTOFBOUNDS_EXCEPTION
#define IO_EXCEPTION STATIC_com_sun_cldc_io_ConnectionBase_IO_EXCEPTION
#define INTERRUPTEDIO_EXCEPTION STATIC_com_sun_cldc_io_ConnectionBase_INTERRUPTEDIO_EXCEPTION
#define CONNECTIONNOTFOUND_EXCEPTION STATIC_com_sun_cldc_io_ConnectionBase_CONNECTIONNOTFOUND_EXCEPTION
#define CLASSNOTFOUND_EXCEPTION  STATIC_com_sun_cldc_io_ConnectionBase_CLASSNOTFOUND_EXCEPTION
#define INTERRUPTED_EXCEPTION STATIC_com_sun_cldc_io_ConnectionBase_INTERRUPTED_EXCEPTION
#define CERTIFICATE_EXCEPTION STATIC_com_sun_cldc_io_ConnectionBase_CERTIFICATE_EXCEPTION


/*------- Types ------------------------------------------------------------*/

/*------- Macros -----------------------------------------------------------*/
#define NETWORKCLASS_HANDLE(s) ((s)->handle)
#define NETWORKCLASS_HANDLEVALID(s) ((s)->isHandleValid)

#define SET_NETWORKCLASS_HANDLE(s,h) ((s)->handle = (h))
#define SET_NETWORKCLASS_HANDLEVALID(s,h) ((s)->isHandleValid = (h))

#define SOCKCLASS_HANDLE(s) ((s)->super.handle)
#define SOCKCLASS_HANDLEVALID(s) ((s)->super.isHandleValid)

#define SET_SOCKCLASS_HANDLE(s,h) ((s)->super.handle = (h))
#define SET_SOCKCLASS_HANDLEVALID(s,h) ((s)->super.isHandleValid = (h))

#endif /* _NETWORK_COMMON_H_ */
