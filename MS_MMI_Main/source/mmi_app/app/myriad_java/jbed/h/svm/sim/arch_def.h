/*[
 *      Name:           arch_def.h
 *		Project:		Jeode
 *      Derived from:   original
 *      Created on:     25/11/96
 *      Version:         $Id$
 *      Purpose:        This file contains the definition of the architecture
 *						specific definitions for the x86 architecture, needed
 *						for the EVM.
 *
 *      Copyright 1999-2004 Esmertec AG. All Rights Reserved.
]*/

#ifndef _ARCH_DEF_H
#define _ARCH_DEF_H

/* Given a return address, adjust so that the pointer is actually within the
 * call instruction.
 */
#define	TARG_CALL_INSTRUCTION_FROM_RETURN_ADDR(retAddr)			\
	((void *)(((IU8*)retAddr) - 1))

#define ALIGNPOLICY_CODE 0

#define ARCH_PROPERTY_NAME	"x86"

#endif /* _ARCH_DEF_H */
