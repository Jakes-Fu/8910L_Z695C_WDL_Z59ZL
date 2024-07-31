/*
 *
 * Copyright  1990-2009 Sun Microsystems, Inc. All Rights Reserved.
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

#ifndef __JAVACALL_LOGGING_H_
#define __JAVACALL_LOGGING_H_

/**
 * @file javacall_logging.h
 * @ingroup MandatoryLogging
 * @brief Javacall interfaces for logging
 */

#include "javacall_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup MandatoryLogging Logging API 
 *  @ingroup JTWI
 *
 *  @{
 */    

#ifdef _MSC_VER
#define LOG_STR(str) "%s:%d: " str, javacall_log_get_file(__FILE__), __LINE__
#define LOG_STR_ENTER(str) "%s:%d: Enter ... " str, javacall_log_get_file(__FILE__), __LINE__
#define LOG_STR_LEAVE(str) "%s:%d: Leave ... " str, javacall_log_get_file(__FILE__), __LINE__
#else
#define LOG_STR(str) "%s:%d, %s(): " str, javacall_log_get_file(__FILE__), __LINE__, __func__
#define LOG_STR_ENTER(str) "%s:%d, %s(): Enter ... " str, javacall_log_get_file(__FILE__), __LINE__, __func__
#define LOG_STR_LEAVE(str) "%s:%d, %s(): Leave ... " str, javacall_log_get_file(__FILE__), __LINE__, __func__
#endif
/**
 * Prints out a string to a system specific output strream
 *
 * @param s a NULL terminated character buffer to be printed
 */
void javacall_prints(const char* s);
void javacall_flushs(void);
void javacall_print(const char *s);
void javacall_printf(const char *format, ...);
void javacall_printbinary(char* buf, int size);
char* javacall_wstr2str(const short* src);
char* javacall_nwstr2str(const short* src, int len);
char* javacall_log_get_file(const char* file);

#ifdef JAVA_LOGGING_FILE_ENABLE
void javacall_logging_file_start();
void javacall_logging_file_end();
#endif
typedef enum {
    javacall_channel_default = 2608,
    javacall_channel_system_out,
    javacall_channel_jvm_internal,
    javacall_channel_trace_in,
    javacall_channel_trace_out
} midp_log_channels;


/** @} */


#ifdef __cplusplus
}
#endif

#endif 

