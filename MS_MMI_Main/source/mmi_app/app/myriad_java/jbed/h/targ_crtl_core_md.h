#ifndef _TARG_CRTL_CORE_MD_H_ /*perl robot add*/
#define _TARG_CRTL_CORE_MD_H_ /*perl robot add*/
/*[
********************************************************************************

  Project:      Jeode

  Name:       targ_crtl_md.h

  Version:      $Id$

  Created:      17 August 2001

  Derived from:   Original

  Coding Standards: 3.0

  Purpose:

    This file provides a stub target (i.e. OS and CPU) specific include
    file for the functionality normally provided by the standard C
    runtime library.  This file is included via <os_crtl_md.h> in all
    source files within the build.  It may be overridden for a particular
    target by that target defining a file of the same name.

    See <crtl.h> for more information.

  Copyright 1998-2012 Myriad Group AG. All Rights Reserved.

********************************************************************************
]*/

typedef unsigned short wchar;

/* Target specific Macro Definitions */

/* Target specific Function Prototypes */

#undef CRTL_malloc

#undef CRTL_free
extern void CRTL_freeImpl(void *ptr);
#define CRTL_free( _a )  \
        do { \
            CRTL_freeImpl( _a );  \
            _a = NULL; \
        } while(0)

#undef CRTL_calloc

#undef CRTL_memcpy

#undef CRTL_memset

/* Implemented in strcasecmp.c as extern */
#undef CRTL_strcasecmp

#undef CRTL_wstrlen
int CRTL_wstrlen(const wchar* str );

#undef CRTL_wstrncmp
int CRTL_wstrncmp(const wchar* string1,const wchar* string2,size_t count);

#undef CRTL_wstrncpy
wchar* CRTL_wstrncpy(wchar* dst,const wchar* src,int count);

#endif
