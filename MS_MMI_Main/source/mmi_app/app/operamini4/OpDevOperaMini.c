/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/
#if !(defined(WIN32) && defined(OD_NO_SIMULATOR))
#include "opcfg.h"
#include "opdevinternal.h"
#include "opdevstacktest.h"
#include "opimport.h"


/************************************************************************/
/* PI                                                                   */
/************************************************************************/
#include "OpDevBookmarkImport.c"
#include "OpDevDebug.c"
#include "OpDevDownload.c"
#include "OpDevFile.c"
#include "OpDevFont.c"
#include "OpDevGDI.c"
#include "OpDevIME.c"
#include "OpDevKeyPen.c"
#include "OpDevKeyPenTracker.c"
#include "OpDevMemory.c"
#include "OpDevNetwork.c"
#include "OpDevResource.c"
#include "OpDevScheme.c"
#include "OpDevSocket.c"
#include "OpDevSystem.c"
#include "OpDevThread.c"
#include "OpDevUtil.c"
#include "OpDevStackTest.c"

#ifdef OD_SIM_DYNLINK_SUPPORT
#include "OpDevSim.c"
#endif /* OD_SIM_DYNLINK_SUPPORT */


/************************************************************************/
/* Application                                                          */
/************************************************************************/
#include "OpDevMain.c"
#include "OperaMini.c"
#include "OperaMobile.c"


/************************************************************************/
/* API                                                                  */
/************************************************************************/
#include "opera_api_adapt_platform.c"

#ifdef OD_BUILD_OPERA_MINI
#include "OperaMiniApi.c"
#include "operamini_api_adapt_platform.c"
#endif

#ifdef OD_BUILD_OPERA_MOBILE
#include "operamobile_api_adapt_platform.c"
#endif

#endif /* !(defined(WIN32) && defined(OD_NO_SIMULATOR)) */
