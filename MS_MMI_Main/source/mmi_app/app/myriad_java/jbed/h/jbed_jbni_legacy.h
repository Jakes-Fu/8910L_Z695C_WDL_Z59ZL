/*
 * Copyright 2004-2005 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Legacy definitions (FastBCC legacy, deprecated)
 *
 * Provide mapping from old legacy interfaces to new JBNI interfaces.
 */

#ifndef JBED_JBNI_LEGACY_H
#define JBED_JBNI_LEGACY_H

#ifndef __JBED_JBNI_H__
#error Have to include jbed_jbni.h before jbed_jbni_legacy.h
#endif

/* Old jbed types */
typedef unsigned char JPTR jutf8str;
typedef jchar JPTR jucs2str;
#define _ptrDecl *
typedef uint32_t juint32;

/* Old jbed pointer types */
typedef void JPTR JbedPtr;      /* deprecated */
typedef jbyte JPTR JbedPtrByte; /* deprecated */
typedef jint JPTR JbedPtrInt;   /* deprecated */
typedef jlong JPTR JbedPtrLong; /* deprecated */
typedef JbedPtr JPTR JbedPtrPtr;        /* deprecated */
typedef juint32 JPTR JbedPtrUint32;     /* deprecated */
typedef INSTANCE _jobject;

/**
 * Return the length of a Java string.
 *
 * @param str is the String object.
 * @return the length in number of chars of the string.
 */
#define OPL_jbni_getStringLength(str) Jbni_getStringLength(str)

/**
 * Return the UCS2 contents of a Java string.
 *
 * Note that the returned pointer is within the Java heap and may be freed by
 * the garbage collector or moved by the heap compactor.  Therefore, it must
 * not be retained and/or accessed outside the native function where the
 * originating \a str pointer is obtained.
 *
 * @param str is the String object.
 * @return a pointer to the UCS2 string data.
 */
#define OPL_jbni_getStringUcs2(str) (jucs2str)(Jbni_getStringData((STRING_INSTANCE)str))

/**
 * Temporary definitions of old macros names.
 */
#define ASSERTING_NO_ALLOCATION     JBNI_ASSERTING_NO_ALLOCATION
#define END_ASSERTING_NO_ALLOCATION JBNI_END_ASSERTING_NO_ALLOCATION

#define unhand(x) (*(x))


#endif /* JBED_JBNI_LEGACY_H */

/* vim:set sw=4 expandtab ts=8: */
