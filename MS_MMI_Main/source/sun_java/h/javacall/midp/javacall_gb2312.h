/*
 *
 * Copyright  1990-2008 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */

#ifndef __JAVACALL_GB2312_H_
#define __JAVACALL_GB2312_H_


#ifdef __cplusplus
extern "C" {
#endif
#include "javacall_defs.h"

/**
 * @file javacall_gb2312.h
 * @ingroup gb2312
 * @brief Javacall interfaces for conversion between gb2312 and unicode
 */

/**The Following 4 functions are for trasforamtion between GB2312 and Unicode(UTF-16)*/

/**
 * Performs platform-specific transformation of Unicode to GB2312.
 *
 * @retval >0 the number of byte have been transformed. 
 * @retval -1 failure of transformation
 */
int javacall_utf16_To_gb2312(const unsigned short *ustr, int ulen, unsigned char *sstr, int slen);

/**
 * Performs platform-specific transformation of GB2312 to Unicode.
 *
 * @retval >0 the number of Unicode have been transformed. 
 * @retval -1 failure of transformation
 */
int javacall_gb2312_To_utf16(const unsigned char *sstr, int slen, unsigned short *ustr, int ulen);

/**
 * Performs getting the number of Unicode if have been tranformed.
 *
 * @retval int the number of Unicode have been transformed. 
 */
int javacall_sizeof_gb2312_in_utf16(const unsigned char *b, int offset, int len);

/**
 * Performs getting the number of Bytes with GB2312 format if have been tranformed.
 *
 * @retval int the number of Bytes with GB2312 foamat if have been transformed. 
 */
int javacall_sizeof_utf16_in_gb2312(const unsigned short *c, int offset, int len);

#ifdef __cplusplus
}
#endif

#endif
