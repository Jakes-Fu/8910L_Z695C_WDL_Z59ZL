/*
 * Copyright 1998-2007 Esmertec AG. All Rights Reserved.
 * $Id$
 */

#ifndef __UAPP_UNIT_H__
#define __UAPP_UNIT_H__

#if defined (UAPP_WAP_UNIT) || defined (UAPP_MMS_UNIT) || defined (UAPP_MMS13_UNIT) || defined (UAPP_DRM_UNIT) || defined(MMS13_VIVIAN_TEST)
#define UAPP_LOCAL
#else
#define UAPP_LOCAL static
#endif/*UAPP_WAP_UNIT*/

#if defined JBED_CPP_UNIT
#define UAPP_PRIVATE public
#define UAPP_PROTECT public
#else
#define UAPP_PRIVATE private
#define UAPP_PROTECT protected
#endif /* JBED_CPP_UNIT */

/** It is used for performance analysis. */

#define TEST_ELAPSE_TIME 0

#endif  /* __UAPP_UNIT_H__ */
