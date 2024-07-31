/*
 * Copyright 2004-2007 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * JbedME Porting Layer: Jbed Legacy definitions.
 *
 * Provide mapping from old legacy interfaces to new JBNI interfaces.
 */
#ifndef JBED_LEGACY_H
#define JBED_LEGACY_H

/* Old jeodek types. */
typedef uint8_t IU8;
typedef uint16_t IU16;
typedef uint32_t IU32;
typedef uint64_t IU64;
typedef int8_t IS8;
typedef int16_t IS16;
typedef int32_t IS32;
typedef int64_t IS64;

typedef IU32 IUM8;		/* Minimum  8 bits unsigned */
typedef IS32 ISM8;		/* Minimum  8 bits   signed */
typedef IU32 IUM16;		/* Minimum 16 bits unsigned */
typedef IS32 ISM16;		/* Minimum 16 bits   signed */
typedef IU32 IUM32;		/* Minimum 32 bits unsigned */
typedef IS32 ISM32;		/* Minimum 32 bits   signed */
typedef IU64 IUM64;		/* Minimum 64 bits unsigned */
typedef IS64 ISM64;		/* Minimum 64 bits   signed */

typedef bool_t IBOOL;

typedef int ISH;
typedef unsigned int IUH;
typedef void* IHP;
typedef uintptr_t IHPE;

/** Unused structure to perform compile-time size & sign checks.
 *
 * Any compilation errors produced by this structure indicate that there are
 * serious problems elsewhere with the size or sign of the underlying types
 * used by the Jbed sources. Do NOT delete or modify this structure if it does
 * not compile -- the types it is checking must be fixed.
 */
struct __isl_unused_jbed_legacy_h_assertion___ {
    char check_sizeof_IU8[ sizeof(IU8) == 1 ? 1 : -1];
    char check_sizeof_IS8[ sizeof(IS8) == 1 ? 1 : -1];
    char check_sizeof_IU16[ sizeof(IU16) == 2 ? 1 : -1];
    char check_sizeof_IS16[ sizeof(IS16) == 2 ? 1 : -1];
    char check_sizeof_IU32[ sizeof(IU32) == 4 ? 1 : -1];
    char check_sizeof_IS32[ sizeof(IS32) == 4 ? 1 : -1];
    char check_sizeof_IU64[ sizeof(IU64) == 8 ? 1 : -1];
    char check_sizeof_IS64[ sizeof(IS64) == 8 ? 1 : -1];
    char check_sign_IU8[ ((IU8)-1) > 0 ? 1 : -1];
    char check_sign_IS8[ ((IS8)-1) < 0 ? 1 : -1];
    char check_sign_IU16[ ((IU16)-1) > 0 ? 1 : -1];
    char check_sign_IS16[ ((IS16)-1) < 0 ? 1 : -1];
    char check_sign_IU32[ ((IU32)-1) > 0 ? 1 : -1];
    char check_sign_IS32[ ((IS32)-1) < 0 ? 1 : -1];
};

#endif /* JBED_LEGACY_H */
