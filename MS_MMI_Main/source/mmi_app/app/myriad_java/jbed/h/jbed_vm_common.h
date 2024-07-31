/*
 * Copyright 2008 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * FastBCC VM defines that depend on the toolset,
 * common between JBNI, KNI and ASYNC.
 */

#ifndef __JBED_VM_COMMON_H__
#define __JBED_VM_COMMON_H__

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * Definitions that depend on the toolset.
 *
 * All exported documentation comments are in the ARM section.
 *
 */

/* Note: this header can be shipped externally and should not
   contain important IP */

#if defined(JBED_ARM)
/* { ARM */

/*
 * Architecture: ARM.
 * Compilers: SDT, ADS, RVCT, GCC-ARM, IAR ARM compiler.
 */

/**
 * Calling convention for functions exported to Java.
 *
 * <code>JBNI_EXPORT</code> is a calling convention alias for functions exported
 * from the native world to Java, e.g., OPL_os_getStackSpace().
 */

#define JBNI_EXPORT

/**
 * Calling convention for functions imported from Java.
 *
 * <code>JBNI_IMPORT</code> is a calling convention alias for functions imported
 * into the native world from Java, e.g., Jbed_iterate().
 */

#define JBNI_IMPORT

/**
 * Alias for "*", to declare pointers.
 *
 * JPTR is an alias for "*" to declare pointers in Jbed headers.  It must be
 * used when interfacing with the VM to ensure that native pointers are in the
 * same format as those produced by the Jbed compiler.
 */

#define JPTR *

/* } ARM */
#elif defined(JBED_X86)
/* { WIN32 */

/*
 * Architecture: Win32.
 * Compilers: MS, Borland.
 * Also see comments in ARM section above.
 */

#if defined(__GNUC__)
#define JBNI_EXPORT __attribute__ ((stdcall)) /* Jbed x86 uses standard calling convention */
#define JBNI_IMPORT __attribute__ ((stdcall))

#elif defined (UNDER_CE) && defined(_MSC_VER)

#define JBNI_EXPORT /* For the JBED WIN CE build __stdcall is default convention */
#define JBNI_IMPORT
#else
#define JBNI_EXPORT __stdcall    /* Jbed x86 uses standard calling convention */
#define JBNI_IMPORT __stdcall
#endif
#define JPTR *

/* } WIN32 */
#else
#error "unknown toolset"
#endif

/** A Java byte. */
typedef int8_t jbyte;

/** A Java boolean. */
typedef uint8_t jboolean;

/** A Java short (16-bit). */
typedef int16_t jshort;

/** A Java char (16-bit). */
typedef uint16_t jchar;

/** A Java int (32-bit). */
typedef int32_t jint;

/** A Java long (64-bit). */
typedef int64_t jlong;

/** A Java float (32-bit). */
typedef float jfloat;
/** A Java double (64-bit). */
typedef double jdouble;

/** Unused structure to perform compile-time size check. */
struct __unused_jbed_jbni_h_assertion___ {
    char check_jbyte[sizeof(jbyte) == 1 ? 1 : -1];
    char check_jboolean[sizeof(jboolean) == 1 ? 1 : -1];
    char check_jshort[sizeof(jshort) == 2 ? 1 : -1];
    char check_jchar[sizeof(jchar) == 2 ? 1 : -1];
    char check_jint[sizeof(jint) == 4 ? 1 : -1];
    char check_jlong[sizeof(jlong) == 8 ? 1 : -1];
};


#if defined(__cplusplus)
}
#endif

#endif /* __JBED_VM_COMMON_H__ */
