/*[
 *      Name:           family_def.h
 *		Project:		Jeode
 *      Derived from:   Original
 *      Created on:     24/3/98
 *      Version:         $Id$
 *      Purpose:        This file contains the generic OS specific
 *						definitions for the Unix OS, needed for the
 *						EVM.
 *
 *		Copyright 2004 Esmertec AG Switzerland. All Rights Reserved.
 *		(c) Copyright Insignia Solutions plc, 1998 - 2002
]*/

#ifndef FAMILY_DEF_H
#define FAMILY_DEF_H

#define DEFAULT_LINE_SEP		"\n"
#define DEFAULT_PATH_SEP		":"
#define DEFAULT_FILE_SEP		"/"
#define PATH_SEPARATOR			':'
#define FILE_SEPARATOR			'/'
#define IS_FILE_SEPARATOR(c)	((c) == '/')

#define IS_PATH_SEPARATOR(p)	(*(p) == PATH_SEPARATOR)

#endif /* FAMILY_DEF_H */
