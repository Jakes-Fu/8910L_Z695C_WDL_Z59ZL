/*[
********************************************************************************

	Project:			Jeode

	Name:				os_crtl_md.h

	Version:			$Id$

	Created:			17 August 2001

	Derived from:		Original

	Coding Standards:	3.0

	Purpose:

		This file provides a stub OS-specific include file for the
 		functionality normally provided by the standard C runtime library.
		This file is included via <crtl_md.h> in all source files within
		the build.  It may be overridden for a particular OS by that OS
		defining a file of the same name.

		See <crtl.h> for more information.
		
	(c) Copyright Insignia Solutions plc, 1998 - 2001

********************************************************************************
]*/

/* OS specific Includes */

/* OS specific Macro Definitions */

/* Include the next, more specific CRTL header */
#include <targ_crtl_core_md.h>

/* OS specific Function Prototypes */
