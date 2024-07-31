/*[
 *      Name:           targ_def.h
 *      Project:        Spreadtrum
 *      Derived From:
 *      Created On:     May 07, 2008
 *      Version:        $Id$
 *      Purpose:        To contain TARGET specific definitions for
 *                      the Spreadtrum product.
 *      NOTE:g
 *                      This file should contain only target specific
 *                      definitions as opposed to OS specific or
 *                      architecture specific definitions which can be
 *                      found in the relevant os_def.h or arch_def.h
 *                      file respectively.
 *
 *      Copyright 2005-2011 Myriad Group AG. All Rights Reserved.
]*/

#ifndef _TARG_DEF_H
#define _TARG_DEF_H

#define CFG_HOME "D:/Java"

#ifdef MAX_FILENAME_LEN
    #undef MAX_FILENAME_LEN
#endif

#define MAX_FILENAME_LEN (256)

#ifdef DISPLAY_WIDTH
    #undef  DISPLAY_WIDTH
#endif

#define DISPLAY_WIDTH 240  /*define the screen width*/

#ifdef DISPLAY_HEIGHT
    #undef DISPLAY_HEIGHT
#endif

#define DISPLAY_HEIGHT 320 /*define the screen height*/

/*----not need to define HAVE_BOOLEAN,to void build errs
#ifdef JBED_X86
#define HAVE_BOOLEAN
#endif
------------*/

#endif  /* _TARG_DEF_H */

