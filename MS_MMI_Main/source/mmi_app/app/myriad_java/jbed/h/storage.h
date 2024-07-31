/*[
 * Copyright 2001-2007 Esmertec AG. All Rights Reserved.
 * $Id$
]*/

/*
 * @(#)storage.h	1.7 01/07/25
 *
 * Copyright (c) 1998-2001 Sun Microsystems, Inc. All Rights Reserved.
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
 *
 *
 * This is the native storage interface for the MIDP RI. It hides any
 * directory hierarchies by making the file system look like a flat file
 * system. The internal Java classes will prefix all file names with a storage
 * root string without using an directory separators. RMS will go further and
 * add a suite unique string to the storage root string, also without using
 * file separators.
 *
 * See also "Filenames" and "I/O Modes" below.
 */
#ifndef _STORAGE_H
#define _STORAGE_H

/*
 * Filenames
 *
 * The storage implementation must support 255 8 bit character file names,
 * that include the characters A-Z, a-z, 0-9, _, %, #, .
 * The filenames will differ in more than just case, so case insensitive
 * systems like Win32 are not a problem.
 */
#define MAX_FILENAME_LENGTH 255 /* does not include the zero terminator */

/*
 * Initialize and finalize the storage subsystem.
 */
extern int  initializeStorage(char *);
extern void finalizeStorage(void);

/*
 * Returns the file separator as a string.
 *
 * This function only called by the main native code to
 * to manage files outside of the RI simulated storage.
 */
extern const char* getFileSeparator(void);

/*
 * Returns the class path separator as a string.
 *
 * This function only called by the main native code to build
 * a class path.
 */
extern const char* getPathSeparator(void);

/*
 * Returns root string for that all files should begin with, including
 * a trailing file separator if needed. By including the any trailing file
 * separators the Java API does not need to know about file separators
 * or subdirectories.
 *
 * The string can be zero characters (just the zero terminator), but not
 * NULL.
 */
extern char* getStorageRoot(void);

#ifdef	MIDP2
/*
 * Returns config root string for that all files should begin with, including
 * a trailing file separator if needed. By including the any trailing file
 * separators the Java API does not need to know about file separators
 * or subdirectories.
 *
 * The string can be zero characters (just the zero terminator), but not
 * NULL.
 */
extern char* getConfigRoot(void);
#endif

#endif /* _STORAGE_H */
