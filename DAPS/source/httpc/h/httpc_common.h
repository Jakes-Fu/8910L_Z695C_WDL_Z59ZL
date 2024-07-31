/*
 * (C) Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __COMMON_H_
#define __COMMON_H_ 1

#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
/* ------------------------------------------------------------------------
** Constants
** ------------------------------------------------------------------------ */

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#define TRUE   1   /* Boolean true value. */
#define FALSE  0   /* Boolean false value. */

#ifndef NULL
#define NULL  0
#endif

/* -----------------------------------------------------------------------
** Standard Types
** ----------------------------------------------------------------------- */

typedef unsigned int    size_t;

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#define HTTPC_LOG  SCI_TRACE_LOW 
		//(fmt, ...)                                   \
      //  do {                                                  \
           // httpc_log(__FILE__, __LINE__, #fmt, __VA_ARGS__); 
		   
       // } while (0)

//void httpc_log(const char *file, int line, const char *format, ... );

#ifdef  WIN32
// FIXME: why a COMPILER check instead of OS? also, these should be HAVE checks
#ifdef __cplusplus
extern "C" {
#endif

int snprintf(char* buffer, size_t count, const char* format, ...);

double wtf_vsnprintf(char* buffer, size_t count, const char* format, va_list args);
#ifdef __cplusplus
}
#endif

#define vsnprintf(buffer, count, format, args) wtf_vsnprintf(buffer, count, format, args)

int strncasecmp(const char* s1, const char* s2, size_t len);
int strcasecmp(const char* s1, const char* s2);

#endif


#endif  /* __COMMON_H_ */

