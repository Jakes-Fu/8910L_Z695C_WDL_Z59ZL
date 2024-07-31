/*[
 *      Project:			Jeode
 *		Name:				publicsymbol.h
 *		Created On:			Aug 2001
 *		Version:			$Id$
 *		Coding Standards:	3.0
 *		Purpose:			Define default PUBLICSYMBOL
 *		(c) Copyright Insignia Solutions plc, 2001
]*/

#ifndef	_PUBLICSYMBOL_H
#define	_PUBLICSYMBOL_H

/*
 * This file should only define PUBLICSYMBOL and nothing else.
 * It exists because some dancing bear inserted prototypes
 * occur right at the start of the file before any includes
 * but these may need PUBLICSYMBOL to be defined. In such
 * cases the dancing bear includes (indirectly) this file.
 *
 * This file is also included indirectly by insignia.h.
 */

/*
 * Visual C needs a hint to define a symbol as public
 */
#define PUBLICSYMBOL	__declspec(dllexport)

#endif	/* _PUBLICSYMBOL_H */

