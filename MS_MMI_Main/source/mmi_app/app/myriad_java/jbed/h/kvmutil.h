/*[
 * Copyright 2004-2008 Esmertec AG. All Rights Reserved.
 * $Id$
********************************************************************************
]*/

/*
 * @(#)kvmUtil.h	1.28 01/03/20
 * Copyright (c) 1999,2001 Sun Microsystems, Inc. All Rights Reserved.
 *
 * This software is the confidential and proprietary information of Sun
 * Microsystems, Inc. ("Confidential Information").  You shall not
 * disclose such Confidential Information and shall use it only in
 * accordance with the terms of the license agreement you entered into
 * with Sun.
 *
 * SUN MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
 * SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, OR NON-INFRINGEMENT. SUN SHALL NOT BE LIABLE FOR ANY DAMAGES
 * SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
 * THIS SOFTWARE OR ITS DERIVATIVES.
 */

#ifndef _KVM_UTIL_H_
#define _KVM_UTIL_H_
#include <stdlib.h>

/*
 * Porting layer indirection of mallocs, frees, etc. for the native piece of
 * the JAM
 */
#define jam_malloc CRTL_malloc
#define jam_free CRTL_free

char * jam_strdup (const char *);

#endif /* _JAM_H_ */
