/*
 * Copyright 1998-2008 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Native Interface: JBNI (FastBCC version)
 *
 * This interface provides definitions of types, macros and functions
 * that are used in non-KNI native methods.
 *
 * Also see kni.h for Sun's KNI portable native method interface.
 *
 * Also see the equivalent FastDAC version of this header.
 *
 * <b>Warning:</b> The functions defined here may only be called from the Jbed
 * task, i.e., the task that calls Jbed_run().  The only exceptions to this are
 * Jbni_convertUcs2ToAscii(), Jbni_convertUcs2ToUtf8(),
 * Jbni_convertUtf8ToUcs2(), Jbni_convertAsciiToUcs2(), Jbni_copyUnaligned64()
 * and Jbni_convertJvmDouble(), which may be called from any context.
 * Additionally, functions that might throw exceptions (directly or
 * indirectly), may only be called from JBNI native methods.
 *
 * <b>Warning:</b> Before calling any function that may invoke the garbage
 * collector from native code, each Java reference that has been passed down to
 * native <b>must</b> be registered correctly as a temporary root, to make the
 * reference visible to the garbage collector.  See #START_TEMPORARY_ROOTS,
 * #DECLARE_TEMPORARY_ROOT, etc.  This obviously holds for Jbni_runGc(), but
 * also for other calls that may allocate exceptions or other data, e.g.
 * Jbni_newCharArray(), Jbni_newString(), Jbni_raiseExceptionString(), etc.
 */

#ifndef __JBED_JBNI_H__
#define __JBED_JBNI_H__

/* Get C99 types */
#include <jbed_types.h>

/* Iassert support */
#include <iassert.h>

#include <jbed_vm_common.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* Include APIs that tell the GC that an reference field in an object or static
 * reference fields wre being written.
 * VMs that to not need write barriers use the base implementation which just
 * does the write but does no GC notification
 */
#include <jbed_write_barrier.h>

/*
 * Values for jboolean.
 */

/**
 * False value for #jboolean.
 */

#define JBNI_FALSE 0

/**
 * True value for #jboolean.
 */

#define JBNI_TRUE  1


/*
 * Include the core generated headers from the VM.
 */

#include <jbedcoreinstance.h>

/**
 * Special isolate identifier that refers to all isolates.
 */
#define JBED_ISOLATE_ALL STATIC_com_jbed_runtime_Constants_JBED_ISOLATE_ALL

#include <jbedupcalls.h>    /* prototypes for jbni_intern_newstring, etc. */

/* Object allocation */

/**
 * Allocate a new Java object of the specified type.
 * Warning: This method may cause a GC, and move Java objects.
 * Warning: The constructor of the new object is NOT called.
 * @param cls The class type to allocate. One of the <code>CLASS_*</code>
 * types defined in <code>javaClass.h</code>.
 *
 * \hideinitializer
 */
extern INSTANCE JBNI_IMPORT Jbni_newInstance(CLASS_INSTANCE cls);

/* Array allocation */

/**
 * Create a new Java <code>boolean</code> array of the specified length.
 * Warning: This method may cause a GC, and move Java objects.
 * @param len  Desired size (in elements) of the array.
 * @return a pointer to the new array object, or NULL on failure
 *         (for example lack of memory)
 * \hideinitializer
 */
extern BOOLEANARRAY JBNI_IMPORT Jbni_newBooleanArray(jint len);

/**
 * Create a new Java <code>byte</code> array of the specified length.
 * Warning: This method may cause a GC, and move Java objects.
 * @param len  Desired size (in elements) of the array.
 * @return a pointer to the new array object, or NULL on failure
 *         (for example lack of memory)
 * \hideinitializer
 */
extern BYTEARRAY JBNI_IMPORT Jbni_newByteArray(jint len);

/**
 * Create a new Java <code>char</code> array of the specified length.
 * Warning: This method may cause a GC, and move Java objects.
 * @param len  Desired size (in elements) of the array.
 * @return a pointer to the new array object, or NULL on failure
 *         (for example lack of memory)
 * \hideinitializer
 */
extern CHARARRAY JBNI_IMPORT Jbni_newCharArray(jint len);

/**
 * Create a new Java <code>short</code> array of the specified length.
 * Warning: This method may cause a GC, and move Java objects.
 * @param len  Desired size (in elements) of the array.
 * @return a pointer to the new array object, or NULL on failure
 *         (for example lack of memory)
 * \hideinitializer
 */
extern SHORTARRAY JBNI_IMPORT Jbni_newShortArray(jint len);

/**
 * Create a new Java <code>int</code> array of the specified length.
 * Warning: This method may cause a GC, and move Java objects.
 * @param len  Desired size (in elements) of the array.
 * @return a pointer to the new array object, or NULL on failure
 *         (for example lack of memory)
 * \hideinitializer
 */
extern INTARRAY JBNI_IMPORT Jbni_newIntArray(jint len);

/**
 * Create a new Java <code>long</code> array of the specified length.
 * Warning: This method may cause a GC, and move Java objects.
 * @param len  Desired size (in elements) of the array.
 * @return a pointer to the new array object, or NULL on failure
 *         (for example lack of memory)
 * \hideinitializer
 */
extern LONGARRAY JBNI_IMPORT Jbni_newLongArray(jint len);

/**
 * Create a new Java <code>Object[]</code> array of the specified length.
 * Warning: This method may cause a GC, and move Java objects.
 * @param len  Desired size (in elements) of the array.
 * @return a pointer to the new array object, or NULL on failure
 *         (for example lack of memory)
 */
extern REFARRAY JBNI_IMPORT Jbni_newObjectArray(jint len);

/**
 * Create a new Java <code>String[]</code> array of the specified length.
 * Warning: This method may cause a GC, and move Java objects.
 * @param len  Desired size (in elements) of the array.
 * @return a pointer to the new array object, or NULL on failure
 *         (for example lack of memory)
 */
extern STRINGARRAY JBNI_IMPORT Jbni_newStringArray(jint len);

/**
 * @name Garbage collection modes
 * These constants can be passed to Jbni_runGc().
 */
/*@{*/
/**
 * Free unused Java objects.
 * \hideinitializer
 */
#define JBNI_GC_COLLECT     STATIC_com_jbed_runtime_Jbni_GC_COLLECT
/**
 * Free unused Java objects and reduce heap fragmentation.
 * \hideinitializer
 */
#define JBNI_GC_COMPACT     STATIC_com_jbed_runtime_Jbni_GC_COMPACT
/**
 * Free unused Java objects, reduce heap fragmentation and, in systems where
 * the Java heap consists of multiple blocks obtained from native code, release
 * all heap blocks that are not in use.
 * \hideinitializer
 */
#define JBNI_GC_RELEASE     STATIC_com_jbed_runtime_Jbni_GC_RELEASE
/*@}*/

/**
 * Collects and compacts the Java heap and tries to return unused memory
 * blocks, depending on the mode.
 *
 * This function must only be called from the Jbed task, i.e., the task that
 * calls Jbed_run().
 *
 * @param mode One of JBNI_GC_COLLECT, JBNI_GC_COMPACT and JBNI_GC_RELEASE.
 */
extern void JBNI_IMPORT Jbni_runGc(jint mode);

/**
 * @name Heap block attributes
 * These constants can be passed to Jbni_addHeapBlock() and
 * Jbni_moveHeapBlock().
 */
/*@{*/
/**
 * The specified memory block may be used as a regular Java heap block.
 * \hideinitializer
 */
#define JBNI_HEAP_NORMAL    STATIC_com_jbed_runtime_Jbni_HEAP_NORMAL
/**
 * The specified memory block may not be used for locked Java objects.
 * \hideinitializer
 */
#define JBNI_HEAP_NOLOCK    STATIC_com_jbed_runtime_Jbni_HEAP_NOLOCK
/*@}*/

/**
 * @name Heap management functions
 * For builds that define <code>JBED_CHUNKED_HEAP</code>, the following
 * functions may be used for chunk management by native code. They do not
 * replace OPL_os_getHeapBlock() and OPL_os_freeHeapBlock(). Instead, they can
 * be used together with those calls to give the native code more control over
 * heap chunks.
 *
 * While Jbni_addHeapBlock() can be used in any build, the VM only manages
 * placement of locked objects if <code>JBED_STRICT_LOCKING</code> is defined.
 * On builds that do not use strict locking, attempts to release or move a
 * chunk will often fail due to locked objects within the chunk in question.
 */
/*@{*/
/**
 * Allow additional memory to be used as Java heap.
 * This function may only be used for specifying additional heap blocks after
 * the VM has acquired an initial one. The initial heap block must be returned
 * through OPL_os_getHeapBlock().
 *
 * Whenever the VM releases a heap block, it will call OPL_os_freeHeapBlock()
 * to notify the system, regardless of whether the block had been passed to the
 * VM by OPL_os_getHeapBlock() or Jbni_addHeapBlock().
 *
 * @param addr A pointer to the memory block to use.
 * @param size The size of the memory block.
 * @param mode JBNI_HEAP_NORMAL or JBNI_HEAP_NOLOCK.
 * @return JBNI_TRUE if the memory block was added to the Java heap, otherwise
 * JBNI_FALSE.
 * \hideinitializer
 */
#define Jbni_addHeapBlock(addr, size, mode) \
    Jbni_intern_addHeapBlock((jint)(addr), size, mode)

/**
 * Release a memory block which is currently used as Java heap.
 * Not all heap blocks can be released successfully. If a heap block contains
 * locked objects, it cannot be released. To ensure that a specific heap block
 * isn't used for locked objects, it must be added with Jbni_addHeapBlock(),
 * using JBNI_HEAP_NOLOCK.
 *
 * @param addr The pointer to the memory block, as passed in to
 * Jbni_addHeapBlock() or returned from OPL_os_getHeapBlock.
 * @return JBNI_TRUE if the memory block was released, otherwise JBNI_FALSE.
 * \hideinitializer
 */
#define Jbni_releaseHeapBlock(addr) \
    Jbni_intern_releaseHeapBlock((jint)(addr))

/**
 * Relocate all data from a block of memory currently used as Java heap into a
 * new memory block.
 * Not all data can be relocated, see Jbni_releaseHeapBlock(). The size of the
 * destination block must be equal or larger than the size of the source block.
 *
 * @param src A pointer to the memory block to be released.
 * @param dst A pointer to the new memory block to use instead.
 * @param size The size of the new memory block.
 * @param mode JBNI_HEAP_NORMAL or JBNI_HEAP_NOLOCK.
 * @return JBNI_TRUE if the relocation was successful, otherwise JBNI_FALSE. If
 * JBNI_FALSE is returned, the destination block hasn't been modified and isn't
 * part of the Java heap.
 * \hideinitializer
 */
#define Jbni_moveHeapBlock(src, dst, size, mode) \
    Jbni_intern_moveHeapBlock((jint)(src), (jint)(dst), size, mode)
/*@}*/

/**
 * @name Macros for quick inline operations.  These will not invoke the garbage
 * collector.
 */
/*@{*/
/**
 * Return a reference to the class instance of an object.
 * @param obj The INSTANCE to get a reference from.
 * @return reference to the class instance of the object.
 * \hideinitializer
 */
#define Jbni_getClassFromObject(obj) (*((CLASS_INSTANCE*)((INSTANCE)obj)->tag))

/**
 * Return a pointer to the first unicode character of a Java String. String
 * data <em>must not</em> be modified.
 * @param str is a non-null pointer to a STRING_INSTANCE
 * @return pointer to the first unicode character of the Java String.
 * \hideinitializer
 */
#define Jbni_getStringData(str) ((jchar *)(((char *)str->value)+(str->addr)))

/**
 * Return the length of a Java String.
 * @param str is a non-null pointer to a STRING_INSTANCE
 * @return the number of unicode characters in the Java String
 * \hideinitializer
 */
#define Jbni_getStringLength(str) (str->count)
/*@}*/

/**
 * @name 64-bit field and array access macros
 */
/*@{*/
/*
 * Simple structure for copying unaligned 64 bit quantities
 */
typedef struct { jint first; jint second; } jbni_64bits;

/**
 * Copy a 64-bit value jdouble from the source to the destination,
 * where either the source or the destination may not be aligned to 64-bits.
 * In Jbed, jdouble static or instance fields are not aligned to 64-bits.
 * This macro also ensures that the format of the double matches that of the C
 * compiler and run-time libraries.
 * Do not use (use Jbni_copyUnaligned64 instead) if neither src nor dst are
 * jdouble values as used by the VM. Do not use if both src and dst are VM
 * format jdoubles.
 * @param dst   Destination jdouble value
 * @param src   Source value for copy
 * \hideinitializer
 * \def Jbni_convertJvmDouble(dst, src)
 */

#ifdef DOUBLE_WORDS_FLIPPED
/** \hideinitializer */
#define Jbni_convertJvmDouble(dst, src) \
    do { \
        jbni_64bits *pSrc64 = (jbni_64bits*)&(src); \
        jbni_64bits *pDst64 = (jbni_64bits*)&(dst); \
        pDst64->second = pSrc64->first; \
        pDst64->first = pSrc64->second; \
    } while(0)
#else /* DOUBLE_WORDS_FLIPPED */
/** \hideinitializer */
#define Jbni_convertJvmDouble(dst, src) Jbni_copyUnaligned64(dst, src)
#endif /* DOUBLE_WORDS_FLIPPED */


/**
 * Copy a 64-bit value (a jlong) from the source to the destination,
 * where either the source or the destination may not be aligned to 64-bits.
 * In Jbed jlong static or instance fields are not aligned to 64-bits.
 * @param dst   Destination jlong/int64_t value
 * @param src   Source value for copy
 * \hideinitializer
 */
#define Jbni_copyUnaligned64(dst, src)  \
    do { *(jbni_64bits*)&(dst) = *(jbni_64bits*)&(src); } while(0)

/**
 * Load an array element from a Jbed LONGARRAY.
 * Note: No bounds checking is performed.
 * @param dst   Destination jlong/int64_t value
 * @param array Reference to a LONGARRAY
 * @param index Index of array element to load
 * \hideinitializer
 */
#define Jbni_arrayLoadLong(dst, array, index) \
    Jbni_copyUnaligned64(dst, (array)->data[index])

/**
 * Store a jlong/int64_t into a Java array of 64-bit longs (LONGARRAY).
 * Note: No bounds checking is performed.
 * @param src   Source jlong/int64_t value to store
 * @param array Reference to a LONGARRAY
 * @param index Index of array element to store
 * \hideinitializer
 */
#define Jbni_arrayStoreLong(src, array, index) \
    Jbni_copyUnaligned64((array)->data[index], src)

/**
 * Load an array element from an array of Java doubles.
 * The jdoubles in arrays are not guaranteed to be aligned to 64-bits.
 * Note: No bounds checking is performed.
 * @param dst   Destination jdouble value
 * @param array Reference to array
 * @param index Index of array element to load
 * \hideinitializer
 */
#define Jbni_arrayLoadDouble(dst, array, index) \
    Jbni_convertJvmDouble(dst, (array)->data[index])

/**
 * Store a jdouble into an array of Java doubles.
 * The jdoubles in arrays are not guaranteed to be aligned to 64-bits.
 * Note: No bounds checking is performed.
 * @param src   Source value to store
 * @param array Reference to array
 * @param index Index of array element to store
 * \hideinitializer
 */
#define Jbni_arrayStoreDouble(src, array, index) \
    Jbni_convertJvmDouble((array)->data[index], src)
/*@}*/

/**
 * @name String allocation
 */
/*@{*/
/**
 * Create a new Java String given a zero terminated utf8 encoded string.
 * Warning: This method may cause a GC, and move Java objects.
 * Badly formatted UTF8 strings are silently truncated.
 * @param utf8Str    Pointer to a utf8 string (must point to non-movable data)
 * @return a pointer to the new Java String, or NULL on failure
 *         (for example lack of memory)
 * \hideinitializer
 */
#define Jbni_newString(utf8Str) Jbni_intern_newString((jint)utf8Str)

/**
 * Create a new Java String given a zero terminated utf8 encoded string.
 * Warning: This method may cause a GC, and move Java objects.
 * Badly formatted UTF8 strings are silently truncated.
 * @param utf8Str    Pointer to a utf8 string (must point to non-movable data)
 * @return a pointer to the new Java String.
 *  On failure, and exception is thrown and this method does not return.
 * \hideinitializer
 */
#define Jbni_newStringExc(utf8Str) Jbni_intern_newStringExc((jint)utf8Str)

/**
 * Create a new Java String given a utf8 encoded string.
 * Warning: This method may cause a GC, and move Java objects.
 * Badly formatted UTF8 strings are silently truncated.
 * @param utf8Str    Pointer to a utf8 string (must point to non-movable data)
 * @param utf8Bytes  Number of utf8 bytes in utf8str
 * @return a pointer to the new Java String, or NULL on failure
 *         (for example lack of memory)
 * \hideinitializer
 */
#define Jbni_newStringLen(utf8Str, utf8Bytes) \
            Jbni_intern_newStringLen((jint)utf8Str, utf8Bytes)

/**
 * Create a new Java String given a utf8 encoded string.
 * Warning: This method may cause a GC, and move Java objects.
 * Badly formatted UTF8 strings are silently truncated.
 * @param utf8Str    Pointer to a utf8 string (must point to non-movable data)
 * @param utf8Bytes  Number of utf8 bytes in utf8str
 * @return a pointer to the new Java String.
 *  On failure, and exception is thrown and this method does not return.
 * \hideinitializer
 */
#define Jbni_newStringLenExc(utf8Str, utf8Bytes) \
            Jbni_intern_newStringLenExc((jint)utf8Str, utf8Bytes)

/**
 * Create a new Java String with a specified number of characters, without
 * filling in any content.
 * Warning: This method may cause a GC, and move Java objects.
 * @param len  Number of characters to allocate for the string.
 * @return a pointer to the new Java String, or NULL on failure
 *         (for example lack of memory)
 * \hideinitializer
 */
#define Jbni_newEmptyStringLen(len) \
            Jbni_intern_newEmptyStringLen((jint)(len))

/**
 * Create a new Java String with a specified number of characters, without
 * filling in any content.
 * Warning: This method may cause a GC, and move Java objects.
 * @param len  Number of characters to allocate for the string.
 * @return a pointer to the new Java String.
 *  On failure, and exception is thrown and this method does not return.
 * \hideinitializer
 */
#define Jbni_newEmptyStringLenExc(len) \
            Jbni_intern_newEmptyStringLenExc((jint)(len))

/**
 * Create a new Java String given an array of unicode characters.
 * Warning: This method may cause a GC, and move Java objects.
 * @param src   Source unicode data (must point to non-movable data)
 * @param len   Number of source unicode characters
 * @return a pointer to the new Java String, or NULL on failure
 *         (for example lack of memory)
 * \hideinitializer
 */
#define Jbni_newStringUcs2(src, len) Jbni_intern_newStringUcs2((jint)src, len)

/**
 * Create a new Java String given an array of unicode characters.
 * Warning: This method may cause a GC, and move Java objects.
 * @param src   Source unicode data (must point to non-movable data)
 * @param len   Number of source unicode characters
 * @return a pointer to the new Java String.
 *  On failure, and exception is thrown and this method does not return.
 * \hideinitializer
 */
#define Jbni_newStringUcs2Exc(src, len) Jbni_intern_newStringUcs2Exc((jint)src, len)
/*@}*/

/**
 * @name Character conversion
 */
/*@{*/
/**
 * Convert an array of Unicode characters to ASCII bytes.
 * The conversion replaces \\u0000 characters and those over 255 with a '?'.
 * The resulting data is always terminated with a zero byte, even
 * if that means truncating the string.
 *
 * Pass dstBytes=0 and dst=NULL to get the number of bytes necessary for
 * encoding including the zero terminating byte.
 *
 * @param src      Source array of Unicode characters
 * @param srcChars Number of source characters to encode (characters not bytes)
 * @param dst      Destination for encoded data
 * @param dstBytes Maximum number of bytes to write to dst (inc terminator)
 * @return Number of bytes required for the encoding or -1 if the string had
 *         to be truncated.
 */
jint Jbni_convertUcs2ToAscii(
    const jchar* src, jint srcChars, char* dst, jint dstBytes);

/**
 * Convert an array of Unicode characters to UTF8 encoded bytes.
 *
 * The exact form of UTF8 encoding matches the JVM Spec in that \\u0000
 * characters are encoded using the two byte UTF8 form, and the result
 * is terminated with a zero byte.
 *
 * The resulting data is always terminated with a zero byte, even
 * if that means truncating the string.  When truncation occurs, it will
 * always be at a clean UTF8 boundary.
 *
 * Pass dstBytes=0 and dst=NULL to get the number of bytes necessary for
 * encoding including the zero terminating byte.
 *
 * @param src      Source array of Unicode characters
 * @param srcChars Number of source characters to encode (characters not bytes)
 * @param dst      Destination for encoded data
 * @param dstBytes Maximum number of bytes to write to dst (inc terminator)
 * @return Number of bytes required for the encoding or -1 if the string had
 *         to be truncated.
 */
jint Jbni_convertUcs2ToUtf8(
    const jchar* src, jint srcChars, char* dst, jint dstBytes);

/**
 * Convert UTF8 encoded bytes into an array of Unicode characters.
 *
 * Truncation occurs when the destination array is not big enough,
 * if the source UTF8 data is badly formatted, or if a nul byte is
 * found before srcBytes have been consumed.
 *
 * Unlike some of the other conversion routines, the destination array is
 * not terminated with a \\u0000 character.
 *
 * Pass dst=NULL to get the number of Unicode characters
 * that the UTF8 data decodes to.
 *
 * @param src       Source array of UTF8 encoded bytes
 * @param srcBytes  Number of bytes to decode (-1 means up to first nul byte)
 * @param dst       Destination for decoded characters
 * @param dstBytes  Size of destination array in bytes (not characters)
 *
 * @return Number of characters that the source data decodes to, or
 *  -1 if the destination array is not big enough.
 */
jint Jbni_convertUtf8ToUcs2(
    const char* src, jint srcBytes, jchar* dst, jint dstBytes);

/**
 * Convert ASCII bytes into an array of Unicode characters.
 *
 * Truncation occurs when the destination array is not big enough,
 * or if a nul byte is found before srcBytes have been consumed.
 *
 * Unlike some of the other conversion routines, the destination array is
 * not terminated with a \\u0000 character.
 *
 * Pass dst=NULL to get the number of Unicode characters
 * that the ASCII data decodes to.
 *
 * @param src       Source array of ASCII encoded bytes
 * @param srcBytes  Number of bytes to decode (-1 means up to first nul byte)
 * @param dst       Destination for decoded characters
 * @param dstBytes  Size of destination array in bytes (not characters)
 *
 * @return Number of characters that the source data decodes to, or
 *  -1 if the destination array is not big enough.
 */
jint Jbni_convertAsciiToUcs2(
    const char* src, jint srcBytes, jchar* dst, jint dstBytes);
/*@}*/

/**
 * @name Fetching String data
 */
/*@{*/
/**
 * Extract the data from a Java String, and encode the data as UTF8.
 * The resulting data is always terminated with a zero byte, even
 * if that means truncating the string.  When truncation occurs, it will
 * always be at a clean UTF8 boundary.
 *
 * Pass numBytes=0 and dst=NULL to get the number of bytes necessary for
 * encoding.
 *
 * @param str      String to extract data from
 * @param dst      Destination for encoded data
 * @param dstBytes Maximum number of bytes to write to dst (inc terminator)
 * @return Number of bytes required for the string or -1 if the string had
 *         to be truncated.
 * \hideinitializer
 */
#define Jbni_getStringUtf8(str, dst, dstBytes) \
    Jbni_intern_getStringUtf8(str, (jint)dst, dstBytes)

/**
 * Extract the data from a Java String, converting to ASCII bytes.
 * The conversion replaces zero characters and those over 255 with a '?'.
 * The resulting data is always terminated with a zero byte, even
 * if that means truncating the string.
 *
 * Pass numBytes=0 and dst=NULL to get the number of bytes necessary for
 * encoding.
 *
 * @param str    String to extract data from
 * @param dst    Destination for encoded data
 * @param len    Maximum number of bytes to write to dst (inc terminator)
 * @return Number of bytes required for the string or -1 if the string had
 *         to be truncated.
 * \hideinitializer
 */
#define Jbni_getStringAscii(str, dst, len) \
    Jbni_intern_getStringAscii(str, (jint)dst, len)

/**
 * Extract data from a substring of a Java String, and encode the data as UTF8.
 * The resulting data is always terminated with a zero byte, even
 * if that means truncating the string.  When truncation occurs, it will
 * always be at a clean UTF8 boundary.
 *
 * Pass numBytes=0 and dst=NULL to get the number of bytes necessary for
 * encoding.
 *
 * @param str      String to extract data from
 * @param off      Starting offset from the start of the string
 * @param len      Number of String characters to encode
 * @param dst      Destination for encoded data
 * @param dstBytes Maximum number of bytes to write to dst (inc terminator)
 * @return Number of bytes required for the string or -1 if the string had
 *         to be truncated.
 * \hideinitializer
 */
#define Jbni_getStringCharsUtf8(str, off, len, dst, dstBytes) \
    Jbni_intern_getStringCharsUtf8(str, off, len, (jint)dst, dstBytes)

/**
 * Extract data from a substring of a Java String, converting to ASCII bytes.
 * The conversion replaces zero characters and those over 255 with a '?'.
 * The resulting data is always terminated with a zero byte, even
 * if that means truncating the string.
 *
 * Pass numBytes=0 and dst=NULL to get the number of bytes necessary for
 * encoding.
 *
 * @param str      String to extract data from
 * @param off      Starting offset from the start of the string
 * @param len      Number of String characters to encode
 * @param dst      Destination for encoded data
 * @param dstBytes Maximum number of bytes to write to dst (inc terminator)
 * @return Number of bytes required for the string or -1 if the string had
 *         to be truncated.
 * \hideinitializer
 */
#define Jbni_getStringCharsAscii(str, off, len, dst, dstBytes) \
    Jbni_intern_getStringCharsAscii(str, off, len, (jint)dst, dstBytes)
/*@}*/

/**
 * @name Exception throwing
 */
/*@{*/
/**
 * Raise an exception and cause an abrupt exit from the native method.
 * This function does not return.
 * @param cls Name of exception class to raise.
 * \hideinitializer
 */
#define Jbni_raiseException(cls) Jbni_intern_raiseException((jint)cls)


/**
 * Raise an exception with a message string and cause an abrupt exit
 * from the native method.
 * This function does not return.
 * @param cls  Name of exception class to raise.
 * @param msg  Exception message string instance.
 * \hideinitializer
 */
#define Jbni_raiseExceptionString(cls,msg) Jbni_intern_raiseExceptionString((jint)cls,msg)


/**
 * Raise an exception with a C message string and cause an abrupt exit
 * from the native method.
 * This function does not return.
 * @param cls  Name of exception class to raise.
 * @param msg  Nul terminated exception message C string.
 * \hideinitializer
 */
#define Jbni_raiseExceptionCString(cls,msg) \
            Jbni_intern_raiseExceptionCString((jint)cls,(jint)msg)


/* Wrappers for some well-known exceptions */

/** Wrapper for NullPointer exception \hideinitializer */
#define Jbni_raiseNullPointerException() \
    Jbni_intern_raiseCommonException( \
            STATIC_com_jbed_runtime_Jbni_exNullPointerException)

/** Wrapper for IndexOutOfBounds exception \hideinitializer */
#define Jbni_raiseIndexOutOfBoundsException() \
    Jbni_intern_raiseCommonException( \
            STATIC_com_jbed_runtime_Jbni_exIndexOutOfBoundsException)

/** Wrapper for ArrayIndexOutOfBounds exception \hideinitializer */
#define Jbni_raiseArrayIndexOutOfBoundsException() \
    Jbni_intern_raiseCommonException( \
            STATIC_com_jbed_runtime_Jbni_exArrayIndexOutOfBoundsException)

/** Wrapper for StringIndexOutOfBounds exception \hideinitializer */
#define Jbni_raiseStringIndexOutOfBoundsException() \
    Jbni_intern_raiseCommonException( \
            STATIC_com_jbed_runtime_Jbni_exStringIndexOutOfBoundsException)

/** Wrapper for IllegalArgument exception \hideinitializer */
#define Jbni_raiseIllegalArgumentException() \
    Jbni_intern_raiseCommonException( \
            STATIC_com_jbed_runtime_Jbni_exIllegalArgumentException)

/** Wrapper for OutOfMemory exception \hideinitializer */
#define Jbni_raiseOutOfMemoryError() \
    Jbni_intern_raiseCommonException( \
            STATIC_com_jbed_runtime_Jbni_exOutOfMemoryError)

/** Wrapper for IO exception \hideinitializer */
#define Jbni_raiseIOException() \
    Jbni_intern_raiseCommonException( \
            STATIC_com_jbed_runtime_Jbni_exIOException)
/*@}*/

/**
 * @name Class Names
 */
/*@{*/
/**
 * Get the name of a Java Class, encoded as UTF8.
 * The resulting data is always terminated with a zero byte.
 *
 * Pass numBytes=0 and dst=NULL to get the number of bytes necessary for
 * encoding.
 *
 * @param cls      The Class object to get the name of.
 * @param dst      Destination for the name data.
 * @param dstBytes Maximum number of bytes to write to dst (inc terminator)
 * @return Number of bytes required for the name.
 * \hideinitializer
 */
#define Jbni_getClassNameUtf8(cls, dst, dstBytes) \
    Jbni_intern_getClassNameUtf8(cls, (jint)dst, dstBytes)

/*@}*/

/**
 * @name Returning from a native method
 */
/*@{*/
/** Return from a void native method \hideinitializer */
#define Jbni_return return

/** Return the given value from a native method returning a jboolean
 * \hideinitializer
 */
#define Jbni_returnBoolean(v) return (v)

/** Return the given value from a native method returning a jbyte
 * \hideinitializer
 */
#define Jbni_returnByte(v) return (v)

/** Return the given value from a native method returning a jshort
 * \hideinitializer
 */
#define Jbni_returnShort(v) return (v)

/** Return the given value from a native method returning a jchar
 * \hideinitializer
 */
#define Jbni_returnChar(v) return (v)

/** Return the given value from a native method returning a jint
 * \hideinitializer
 */
#define Jbni_returnInt(v) return (v)

/** Return the given value from a native method returning a jlong
 * \hideinitializer
 */
#define Jbni_returnLong(v) return (v)

/** Return the given value from a native method returning a jfloat
 * \hideinitializer
 */
#define Jbni_returnFloat(v) return (v)

/** Return the given value from a native method returning a jdouble
 * \hideinitializer
 */
#define Jbni_returnDouble(v) \
   do { \
      jdouble tmp; \
      Jbni_convertJvmDouble(tmp, v); \
      return (tmp); \
   } while(0)

/** Return the given value from a native method returning a reference
 * \hideinitializer
 */
#define Jbni_returnRef(v) return (v)
/*@}*/

/**
 * @name Temporary roots and allocation assertion
 *
 * Protect Java references in the face of possible GC cycles.
 *
 * Any allocation from the Java heap may cause the garbage collector to run.
 * During garbage collection, Java heap objects may be moved within the heap.
 * Therefore, references passed down to C potentially become invalid during a
 * GC run.
 *
 * The following macros provide mechanisms to either assert that no Java
 * allocations are made within a critical block, or to register a local
 * reference with the GC such that the reference gets updated when the
 * corresponding heap object is moved. References that have been registered
 * with the GC are called temporary roots.
 */
/*@{*/
/** \def START_TEMPORARY_ROOTS
 * Start a block containing temporary roots.
 * This must be matched by #END_TEMPORARY_ROOTS at the same block level.
 * Nesting of temporary root blocks is allowed.
 *
 * Do not return or otherwise jump out of a temporary root block.
 * #END_TEMPORARY_ROOTS must be executed to invalidate the temporary root
 * registrations. Raising an exception withing a temporary root block is fine
 * though, since this will also invalidate temporary roots before returning to
 * Java to handle the exception.
 */
/** \def END_TEMPORARY_ROOTS
 * End a block containing temporary roots.
 * This invalidates all temporary roots declared since the last
 * #START_TEMPORARY_ROOTS.
 */
/** \def INDICATE_DYNAMICALLY_INSIDE_TEMPORARY_ROOTS
 * Allow declaring temporary roots outside a temporary root block.
 * Legacy code uses this to allow declaration of temporary roots even when
 * the current function doesn't contain a temporary root block. This is used
 * in cases where the caller of a function already started a temporary root
 * block and a temporary root needs to be kept alive after returning from the
 * current function.
 *
 * \deprecated Do not use this in new porting code. Instead, declare the
 * temporary root in the caller and assign NULL. The callee can then update
 * the reference in the caller's stack frame.
 */
/** \def IS_TEMPORARY_ROOT(var, value)
 * @param var The name of a local reference variable
 * @param value The reference to assign to variable var
 *
 * Assign a value to a local reference variable and register it with the GC.
 * This macro produces code identical to <code>var = value</code>,
 * except that variable var is registered with the GC and updated when the
 * referenced object moves.
 */
/** \def IS_TEMPORARY_ROOT_FROM_BASE(var, value, base)
 * @param var The name of a local pointer variable
 * @param value A pointer into the object referenced by base
 * @param base A reference to a Java object
 *
 * Assign an internal pointer to a local pointer variable and register it with
 * the GC. This macro produces code identical to <code>var = value</code>,
 * except that var is registered with the GC as a pointer into object base.
 * If object base is moved, the GC updates var to point at the same offset
 * within the base object as before the move.
 *
 * \deprecated Do not use this in new porting code. Instead, register a local
 * reference variable pointing to base and add the offset when accessing data
 * within the base object.
 */
/** \def DECLARE_TEMPORARY_ROOT(type, var, value)
 * @param type The reference type to use for declaring variable var
 * @param var The name of the new local referece variable
 * @param value The reference to assign to variable var
 *
 * Identical to #IS_TEMPORARY_ROOT, except that it also declares the local
 * reference variable. The macro produces code similar to <code>type var
 * = value</code>. A value of NULL is allowed and often useful if the reference
 * to assign is only obtained later. Once the temporary root has been declared,
 * regular assignments are allowed to modify the reference stored in var, as
 * long as var always contains either a valid reference to a Java heap object
 * or NULL.
 */
/** \def DECLARE_TEMPORARY_ROOT_FROM_BASE(type, var, value, base)
 * @param type The pointer type to use for declaring variable var
 * @param var The name of the new local pointer variable
 * @param value A pointer into the object referenced by base
 * @param base A reference to a Java object
 *
 * Identical to #IS_TEMPORARY_ROOT_FROM_BASE, except that it also declares the
 * local pointer variable. The macro produces code similar to <code>type var
 * = value</code>.
 *
 * \deprecated Do not use this in new porting code. Instead, declare a local
 * reference variable pointing to base and add the offset when accessing data
 * within the base object.
 */

#ifndef _JBED_KNI_H_
extern jint *Jbed_nativeCallStateLimit;
extern jint *Jbed_nativeCallStateAdr;
#endif

/** \hideinitializer */
#define START_TEMPORARY_ROOTS { \
    jint *_tmp_roots_ = Jbed_nativeCallStateAdr;
/** \hideinitializer */
#define END_TEMPORARY_ROOTS \
    Jbed_nativeCallStateAdr = _tmp_roots_; }

/** \internal \hideinitializer */
#define INTERN_VERIFY_TEMPORARY_ROOT_SPACE(size) \
    Iassert_always((Jbed_nativeCallStateAdr + size) < Jbed_nativeCallStateLimit),

#ifdef IASSERTS
/** \internal \hideinitializer */
#define INTERN_VERIFY_INSIDE_TEMPORARY_ROOTS (_tmp_roots_ = _tmp_roots_),
#define INDICATE_DYNAMICALLY_INSIDE_TEMPORARY_ROOTS jint *_tmp_roots_ = 0;
#else /* IASSERTS */
/** \internal */
#define INTERN_VERIFY_INSIDE_TEMPORARY_ROOTS
#define INDICATE_DYNAMICALLY_INSIDE_TEMPORARY_ROOTS
#endif /* IASSERTS */

/** \hideinitializer */
#define IS_TEMPORARY_ROOT0(var, value) \
    var = (INTERN_VERIFY_INSIDE_TEMPORARY_ROOTS \
    var = value, \
    INTERN_VERIFY_TEMPORARY_ROOT_SPACE(1) \
    *(Jbed_nativeCallStateAdr++) = (jint)&var, \
    var)

/** \hideinitializer */
#define IS_TEMPORARY_ROOT(var, value) \
    (IS_TEMPORARY_ROOT0(var, value))

/** \hideinitializer */
#define IS_TEMPORARY_ROOT_FROM_BASE0(var, value, base) \
    var = (INTERN_VERIFY_INSIDE_TEMPORARY_ROOTS \
    var = value, \
    INTERN_VERIFY_TEMPORARY_ROOT_SPACE(3) \
    *(Jbed_nativeCallStateAdr++) = ~0, \
    *(Jbed_nativeCallStateAdr++) = (jint)&var, \
    *(Jbed_nativeCallStateAdr++) = (jint)base, \
    var)

/** \hideinitializer */
#define IS_TEMPORARY_ROOT_FROM_BASE(var, value, base) \
    (IS_TEMPORARY_ROOT_FROM_BASE0(var, value, base))

/** \hideinitializer */
#define DECLARE_TEMPORARY_ROOT(type, var, value) \
    type IS_TEMPORARY_ROOT0(var, value)
/** \hideinitializer */
#define DECLARE_TEMPORARY_ROOT_FROM_BASE(type, var, value, base) \
    type IS_TEMPORARY_ROOT_FROM_BASE0(var, value, base)

/**
 * \def JBNI_ASSERTING_NO_ALLOCATION
 * Start a block of code that contains no allocations from the Java heap.
 * Debug builds assert that no allocations from the Java heap are made
 * between #JBNI_ASSERTING_NO_ALLOCATION and #JBNI_END_ASSERTING_NO_ALLOCATION.
 * This macro starts a new C block, so JBNI_END_ASSERTING_NO_ALLOCATION must
 * appear on the same block level. It is safe to assume that no garbage
 * collection takes place while inside such a block, hence the start address of
 * heap objects is guaranteed not to change.
 *
 * Nesting of #JBNI_ASSERTING_NO_ALLOCATION blocks is allowed.
 */
/**
 * \def JBNI_END_ASSERTING_NO_ALLOCATION
 * End a block of code that contains no allocations from the Java heap.
 * End a block started by #JBNI_ASSERTING_NO_ALLOCATION. Must be on the same
 * block level as #JBNI_ASSERTING_NO_ALLOCATION.
 */
#ifdef IASSERTS
extern jint Jbed_noAllocation;
/** \hideinitializer */
#define JBNI_ASSERTING_NO_ALLOCATION \
    { Jbed_noAllocation++; {
/** \hideinitializer */
#define JBNI_END_ASSERTING_NO_ALLOCATION } \
    Jbed_noAllocation--; }
#else
/** \hideinitializer */
#define JBNI_ASSERTING_NO_ALLOCATION {
/** \hideinitializer */
#define JBNI_END_ASSERTING_NO_ALLOCATION }
#endif
/*@}*/

#if defined(__cplusplus)
}
#endif

#endif  /* __JBED_JBNI_H__ */
