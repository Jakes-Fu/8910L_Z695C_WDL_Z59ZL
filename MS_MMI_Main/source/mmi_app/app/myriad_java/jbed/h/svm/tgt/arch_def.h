/*
 * Copyright 1999-2012 Myriad Group AG. All Rights Reserved.
 * $Id$Id: //depot/dev/archs/arm/inc/arch_def.h#2 $
 */

/* This file contains the definition of the architecture
 * specific definitions for the arm architecture.
 */

#ifndef _ARCH_DEF_H
#define _ARCH_DEF_H

/*
 * Given a return address, adjust so that the pointer is actually within the
 * call instruction.
 */
#define	TARG_CALL_INSTRUCTION_FROM_RETURN_ADDR(retAddr)						\
												((void *)(((IU8*)retAddr) - 4))

#define ALIGNPOLICY_CODE 0

#define ARCH_PROPERTY_NAME	"arm"

#define PCF_ARCH_FRAGS		/* Have precompiled fragments */

/* offset added to native addresses used to encode IL_loadWordTrap()
 * Must be a multiple of instruction unit
 */
#define ARCH_TRAP_BYTE_OFFSET 4

#if !defined(JBED_ABI_FPU_REGS) && defined(__ARM_PCS_VFP)
/* A gcc compiler is using hardware FPU registers to pass parameters
 * and upto 16 single precision args can be passed in registers.
 */
#define JBED_ABI_FPU_REGS (16)
#endif /* !JBED_ABI_FPU_REGS && __ARM_PCS_VFP */

#endif /* _ARCH_DEF_H */
