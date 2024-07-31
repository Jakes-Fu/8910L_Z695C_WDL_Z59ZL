/*
 * Copyright 1998-2009 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Native Interface: KNI (FastBCC version)
 *
 * See the <a
 * href="../../../manuals/develop/kni_spec1.0/index.html">
 * KNI specification</a> in the Jbed documentation.
 *
 * See the <a
 * href="../../../manuals/develop/kni.html">Jbed KNI native
 * interface document</a>.
 */

/* Note: this header can be shipped externally and should not
contain important IP */

#ifndef _JBED_KNI_H_
#define _JBED_KNI_H_

/* Get C99 types */
#include <jbed_types.h>

/* assert support */
#include <assert.h>

/* Calling conventions */
#include <jbed_vm_common.h>

/* Write barrier helpers. */
#include <jbed_write_barrier.h>

#define JBED_JAVA_refTyp 2
#define JBED_JAVA_longTyp 11
#define JBED_JAVA_intTyp 10
#define JBED_JAVA_shortTyp 9
#define JBED_JAVA_byteTyp 8
#define JBED_JAVA_doubleTyp 7
#define JBED_JAVA_floatTyp 6
#define JBED_JAVA_charTyp 5
#define JBED_JAVA_boolTyp 4

#define JBEDKNI_instanceTagOffset 0
#define JBEDKNI_stringValueOffset 12
#define JBEDKNI_stringAddrOffset 16
#define JBEDKNI_stringCountOffset 20
#define JBEDKNI_classBaseOffset 20
#define JBEDKNI_arrayLengthOffset 12
#define JBEDKNI_arrayDataOffset 16

#define JBEDKNI_INSTANCE void*
#define JBEDKNI_STRING void*
#define JBEDKNI_CLASS void*
#define JBEDKNI_ARRAY void*

#ifndef __JBED_JBNI_H__
extern jint *Jbed_nativeCallStateLimit;
extern jint *Jbed_nativeCallStateAdr;
#endif // __JBED_JBNI_H__

/* Include the KNI Upcalls prototypes */
#include <jbedexpupcalls.h>

/* Constants */
#define KNI_FALSE 0
#define KNI_TRUE 1
#define KNI_OK 0
#define KNI_ERR -1
#define KNI_VERSION 0x00010000 /* KNI version 1.0 */

#ifdef __cplusplus
#define KNIEXPORT extern "C"
#else
#define KNIEXPORT extern
#endif

#define KNI_RETURNTYPE_VOID void JBNI_EXPORT
#define KNI_RETURNTYPE_BOOLEAN int32_t JBNI_EXPORT
#define KNI_RETURNTYPE_BYTE int32_t JBNI_EXPORT
#define KNI_RETURNTYPE_CHAR int32_t JBNI_EXPORT
#define KNI_RETURNTYPE_SHORT int32_t JBNI_EXPORT
#define KNI_RETURNTYPE_INT int32_t JBNI_EXPORT
#define KNI_RETURNTYPE_LONG int64_t JBNI_EXPORT
#define KNI_RETURNTYPE_FLOAT float JBNI_EXPORT
#define KNI_RETURNTYPE_DOUBLE double JBNI_EXPORT
#define KNI_RETURNTYPE_OBJECT JBEDKNI_INSTANCE JBNI_EXPORT

/* should be an opaque pointer according to spec */
typedef void *jfieldID;
typedef jint jsize;

/* Jbed Types */
typedef union { int32_t lo; JBEDKNI_INSTANCE obj; } *_jbedparamlist;

typedef JBEDKNI_INSTANCE *jobject;      /* object handle */
typedef jobject jclass;
typedef jobject jstring;
typedef jobject jthrowable;
typedef jobject jarray;
typedef jarray jobjectArray;
typedef jarray jbooleanArray;
typedef jarray jcharArray;
typedef jarray jshortArray;
typedef jarray jbyteArray;
typedef jarray jintArray;
typedef jarray jlongArray;
typedef jarray jfloatArray;
typedef jarray jdoubleArray;

#if defined(__cplusplus)
extern "C" {
#endif

/* Jbed Variables */

extern _jbedparamlist Jbed_ParamPtr;

#if defined(__cplusplus)
}
#endif

#if defined(JBED_X86)
#define Jbed_ParamOffset 2
#else
#define Jbed_ParamOffset 0
#endif

/* Common Macros */
/* Note: may also be used in kni_ext.h. */
#define JBEDKNI_STRVAL(s) (*(int32_t*)((int32_t)STR(s) + JBEDKNI_stringValueOffset))
#define JBEDKNI_STRADDR(s) (*(int32_t*)((int32_t)STR(s) + JBEDKNI_stringAddrOffset))
#define STRING_ADDR(s) (JBEDKNI_STRVAL(s) + JBEDKNI_STRADDR(s))
#define ARRAY_ADDR(darray) ((int32_t)ARR(darray) + JBEDKNI_arrayDataOffset)

/* Macros */

#ifdef IASSERTS

/* Null or valid object. */
#define NOBJ(x) ((JBEDKNI_INSTANCE)JbedKNI_VerifyObject(x,'o'))
/* Null or Class object. */
#define NCLS(x) ((JBEDKNI_CLASS)JbedKNI_VerifyObject(x,'c'))
/* Null or string object. */
#define NSTR(x) ((JBEDKNI_STRING)JbedKNI_VerifyObject(x,'s'))
/* Null or array object. */
#define NARR(x) ((JBEDKNI_ARRAY)JbedKNI_VerifyObject(x,'a'))
/* Valid object. */
#define OBJ(x) ((JBEDKNI_INSTANCE)JbedKNI_VerifyObject(x,'O'))
/* Class object. */
#define CLS(x) ((JBEDKNI_CLASS)JbedKNI_VerifyObject(x,'C'))
/* String object. */
#define STR(x) ((JBEDKNI_STRING)JbedKNI_VerifyObject(x,'S'))
/* Array Object. */
#define ARR(x) ((JBEDKNI_ARRAY)JbedKNI_VerifyObject(x,'A'))

#define OBJECT_FIELD(object, offset, type, typechar) (*(type*)JbedKNI_VerifyField(object, (jint)offset, typechar, NULL))
#define OBJECT_FIELD_SETOBJ(object, offset, type, typechar, val) JbedKNI_VerifyFieldSetObj(object,(jint)offset,typechar,val)
#define STATIC_FIELD(address, type, typechar) (*(type*)JbedKNI_VerifyField(0L, (jint)address, typechar, NULL))
#define STATIC_FIELD_SETOBJ(address, type, typechar, val) JbedKNI_VerifyFieldSetObj(0L,(jint)address,typechar,val)

#define GET_STRING(str, offset, length, dest) JbedKNI_GetStringRange(STR(str),offset,length,dest)

#define SET_ELEMENT(array, index, value, type, typechar) do { (*((type*)JbedKNI_VerifyArray(array,index,typechar,NULL))) = (value); } while(0)
#define SET_ELEMENTOBJ(array, index, value, type, typechar) JbedKNI_VerifyArraySetObj(array,index,typechar,value)

#define GET_ELEMENT(array, index, type, typechar) (*((type*)JbedKNI_VerifyArray(array,index,typechar, NULL)))

#define SET_ELEMENTS(array, offset, length, source) JbedKNI_VerifyArrayCopy(array,offset,length, (int32_t) source,1)
#define GET_ELEMENTS(array, offset, length, dest) JbedKNI_VerifyArrayCopy(array,offset,length, (int32_t) dest,0)

#else

#define NOBJ(x) ((JBEDKNI_INSTANCE)x)
#define NCLS(x) ((JBEDKNI_CLASS)x)
#define NSTR(x) ((JBEDKNI_STRING)x)
#define NARR(x) ((JBEDKNI_ARRAY)x)
#define OBJ(x) ((JBEDKNI_INSTANCE)x)
#define CLS(x) ((JBEDKNI_CLASS)x)
#define STR(x) ((JBEDKNI_STRING)x)
#define ARR(x) ((JBEDKNI_ARRAY)x)

#define MOVE_MEM(source, dest, length) JbedKNI_Move((int32_t)source, (int32_t)dest, length)

#define OBJECT_FIELD(object, offset, type, typechar) (*(type*)((int32_t)(object) + (int32_t)(offset)))
#define OBJECT_FIELD_SETOBJ(object,offset,type,typechar,val) (OBJECT_FIELD(object,offset,type,typechar) = val)

#ifdef JBED_CONCURRENT_ISOLATES
extern jint Jbed_relativeStaticsBase;
#define STATIC_FIELD(address, type, typechar) (*(type*)(((jint)address) < 0 ? (-((jint)address)) : ((jint)address + Jbed_relativeStaticsBase)))
#else
#define STATIC_FIELD(address, type, typechar) (*(type*)(address))
#endif
#define STATIC_FIELD_SETOBJ(address, type, typechar, val) (STATIC_FIELD(address,type,typechar) = val)

#define GET_STRING(str, offset, length, dest) MOVE_MEM(STRING_ADDR(str) + (offset<<1), dest, length<<1)

#define SET_ELEMENT(array, index, value, type, typechar) do { ((type*)ARRAY_ADDR(array))[index] = (value); } while(0)
#define SET_ELEMENTOBJ(array, index, value, type, typechar) SET_ELEMENT(array,index,value,type,typechar)
#define GET_ELEMENT(array, index, type, typechar) (((type*)ARRAY_ADDR(array))[index])

#define SET_ELEMENTS(array, offset, length, source) MOVE_MEM(source, (int32_t)ARRAY_ADDR(array) + (offset), length)
#define GET_ELEMENTS(array, offset, length, dest) MOVE_MEM((int32_t)ARRAY_ADDR(array) + (offset), dest, length)

#endif

#define OBJECT_CLASS(object) CLS(*(  (JBEDKNI_INSTANCE *)(*((int32_t *) ((int32_t) OBJ(object) + JBEDKNI_instanceTagOffset)))  ))
#define CLASS_SUPERCLASS(cls) NCLS(  (JBEDKNI_INSTANCE)(*((int32_t *)((int32_t) CLS(cls) + JBEDKNI_classBaseOffset)) )  )

#define IS_INSTANCE(object, cls) JbedKNI_IsInstance(CLS(cls), OBJ(object))
#define IS_ASSIGNABLE(class1, class2) JbedKNI_IsAssignableFrom(CLS(class2), CLS(class1))

#define FIELD_ID(cls, name, sig) (jfieldID)JbedKNI_GetFieldAddress(NCLS(cls), (int32_t) name, (int32_t) sig, 0)
#define STATIC_ID(cls, name, sig) (jfieldID)JbedKNI_GetFieldAddress(NCLS(cls), (int32_t) name, (int32_t) sig, 8)

#define STRING_LENGTH(str) (((void *)(str))==NULL?-1:*((int32_t *)((int32_t) STR(str) + JBEDKNI_stringCountOffset) ))
#define ARRAY_LENGTH(arr) (((void *)(arr))==NULL?-1:*((int32_t *)((int32_t) ARR(arr) + JBEDKNI_arrayLengthOffset)))
#define PARAMETER(index) (Jbed_ParamPtr[(index)+Jbed_ParamOffset].obj)
#define CHAR_STRING(chars, length) JbedKNI_NewCharString((int32_t)chars, length)
#define UTF_STRING(chars) JbedKNI_NewUtfString((int32_t) chars, 0)
#define THIS_CLASS(name) JbedKNI_LookupClass((int32_t)name)

#define THROW_NEW(name, message) JbedKNI_ThrowNew((int32_t)name, UTF_STRING(message))


/* KNI Functions */
#define KNI_StartHandles(n) \
    { jint *_tmp_roots_ = Jbed_nativeCallStateAdr

#define KNI_EndHandles() \
    Jbed_nativeCallStateAdr = _tmp_roots_; }

#ifdef IASSERTS
#define INTERN_VERIFY_ROOTS (_tmp_roots_ = _tmp_roots_),
#else /* IASSERTS */
#define INTERN_VERIFY_ROOTS
#endif /* IASSERTS */

#ifdef IASSERT_SUPPRESS_ALWAYS
#define Jbed_Iassert_always(a)      ((void)0)
#else
#define Jbed_Iassert_always(a)      assert(a)
#endif

#define KNI_DeclareHandle(handle) \
    JBEDKNI_INSTANCE kni0_##handle = (INTERN_VERIFY_ROOTS \
    kni0_##handle = NULL, \
    Jbed_Iassert_always((Jbed_nativeCallStateAdr + 1) < Jbed_nativeCallStateLimit), \
    *(Jbed_nativeCallStateAdr++) = (jint)&kni0_##handle, \
    kni0_##handle); \
    const jobject handle = (jobject)&kni0_##handle


#define KNI_ReleaseHandle(handle) (*(handle) = NULL)
#define KNI_IsNullHandle(handle) (*(handle) == NULL)
#define KNI_IsSameObject(handle1, handle2) (*(handle1) == *(handle2))

#define KNI_GetVersion() (KNI_VERSION)

#define KNI_FindClass(name, classHandle) (*(classHandle) = (JBEDKNI_INSTANCE)THIS_CLASS(name))
#define KNI_GetObjectClass(objectHandle, classHandle) (*(classHandle) = (JBEDKNI_INSTANCE)OBJECT_CLASS(*(objectHandle)))
#define KNI_GetSuperClass(classHandle, superclassHandle) (*(superclassHandle) = (JBEDKNI_INSTANCE)CLASS_SUPERCLASS(*(classHandle)))
#define KNI_IsInstanceOf(objectHandle, classHandle) IS_INSTANCE(*(objectHandle), *(classHandle))
#define KNI_IsAssignableFrom(classHandle1, classHandle2) IS_ASSIGNABLE(*(classHandle1), *(classHandle2))

#define KNI_ThrowNew(name, message) THROW_NEW(name, message)
#define KNI_FatalError(message) JbedKNI_FatalError(message)

#define KNI_GetFieldID(classHandle, name, signature) FIELD_ID(*(classHandle), name, signature)
#define KNI_GetStaticFieldID(classHandle, name, signature) STATIC_ID(*(classHandle), name, signature)

/* Get instance fields */
#define KNI_GetBooleanField(objectHandle, fieldID) \
    OBJECT_FIELD(*(objectHandle), fieldID, jboolean, JBED_JAVA_boolTyp)
#define KNI_GetByteField(objectHandle, fieldID) \
    OBJECT_FIELD(*(objectHandle), fieldID, jbyte, JBED_JAVA_byteTyp)
#define KNI_GetCharField(objectHandle, fieldID) \
    OBJECT_FIELD(*(objectHandle), fieldID, jchar, JBED_JAVA_charTyp)
#define KNI_GetShortField(objectHandle, fieldID) \
    OBJECT_FIELD(*(objectHandle), fieldID, jshort, JBED_JAVA_shortTyp)
#define KNI_GetIntField(objectHandle, fieldID) \
    OBJECT_FIELD(*(objectHandle), fieldID, jint, JBED_JAVA_intTyp)
#define KNI_GetLongField(objectHandle, fieldID) \
    OBJECT_FIELD(*(objectHandle), fieldID, jlong, JBED_JAVA_longTyp)
#define KNI_GetFloatField(objectHandle, fieldID) \
    OBJECT_FIELD(*(objectHandle), fieldID, jfloat, JBED_JAVA_floatTyp)
#define KNI_GetDoubleField(objectHandle, fieldID) \
    OBJECT_FIELD(*(objectHandle), fieldID, jdouble, JBED_JAVA_doubleTyp)
#define KNI_GetObjectField(objectHandle, fieldID, toHandle) \
    do { *toHandle = OBJECT_FIELD(*(objectHandle), fieldID, JBEDKNI_INSTANCE, JBED_JAVA_refTyp); } while (0)

/* Set instance fields */
#define KNI_SetBooleanField(objectHandle, fieldID, value) \
    do { OBJECT_FIELD(*(objectHandle), fieldID, jboolean, JBED_JAVA_boolTyp) = (value); } while (0)
#define KNI_SetByteField(objectHandle, fieldID, value) \
    do { OBJECT_FIELD(*(objectHandle), fieldID, jbyte, JBED_JAVA_byteTyp) = (value); } while (0)
#define KNI_SetCharField(objectHandle, fieldID, value) \
    do { OBJECT_FIELD(*(objectHandle), fieldID, jchar, JBED_JAVA_charTyp) = (value); } while (0)
#define KNI_SetShortField(objectHandle, fieldID, value) \
    do { OBJECT_FIELD(*(objectHandle), fieldID, jshort, JBED_JAVA_shortTyp) = (value); } while (0)
#define KNI_SetIntField(objectHandle, fieldID, value) \
    do { OBJECT_FIELD(*(objectHandle), fieldID, jint, JBED_JAVA_intTyp) = (value); } while (0)
#define KNI_SetLongField(objectHandle, fieldID, value) \
    do { OBJECT_FIELD(*(objectHandle), fieldID, jlong, JBED_JAVA_longTyp) = (value); } while (0)
#define KNI_SetFloatField(objectHandle, fieldID, value) \
    do { OBJECT_FIELD(*(objectHandle), fieldID, jfloat, JBED_JAVA_floatTyp) = (value); } while (0)
#define KNI_SetDoubleField(objectHandle, fieldID, value) \
    do { OBJECT_FIELD(*(objectHandle), fieldID, jdouble, JBED_JAVA_doubleTyp) = (value); } while (0)
#define KNI_SetObjectField(objectHandle, fieldID, fromHandle) \
    do                                                        \
    {                                                         \
        OBJECT_FIELD_SETOBJ(*(objectHandle), fieldID, JBEDKNI_INSTANCE, JBED_JAVA_refTyp, *(fromHandle)); \
        WRITE_BARRIER_ADD(objectHandle);                      \
    } while (0)
/* Get static fields */
#define KNI_GetStaticBooleanField(classHandle, fieldID) \
    STATIC_FIELD(fieldID, jboolean,JBED_JAVA_boolTyp)
#define KNI_GetStaticByteField(classHandle, fieldID) \
    STATIC_FIELD(fieldID, jbyte,JBED_JAVA_byteTyp)
#define KNI_GetStaticCharField(classHandle, fieldID) \
    STATIC_FIELD(fieldID, jchar,JBED_JAVA_charTyp)
#define KNI_GetStaticShortField(classHandle, fieldID) \
    STATIC_FIELD(fieldID, jshort,JBED_JAVA_shortTyp)
#define KNI_GetStaticIntField(classHandle, fieldID) \
    STATIC_FIELD(fieldID, jint,JBED_JAVA_intTyp)
#define KNI_GetStaticLongField(classHandle, fieldID) \
    STATIC_FIELD(fieldID, jlong,JBED_JAVA_longTyp)
#define KNI_GetStaticFloatField(classHandle, fieldID) \
    STATIC_FIELD(fieldID, jfloat,JBED_JAVA_floatTyp)
#define KNI_GetStaticDoubleField(classHandle, fieldID) \
    STATIC_FIELD(fieldID, jdouble,JBED_JAVA_doubleTyp)
#define KNI_GetStaticObjectField(classHandle, fieldID, toHandle) \
    do { *(toHandle) = STATIC_FIELD(fieldID, JBEDKNI_INSTANCE,JBED_JAVA_refTyp); } while(0)

/* Set static fields */
#define KNI_SetStaticBooleanField(classHandle, fieldID, value) \
    do { STATIC_FIELD(fieldID, jboolean, JBED_JAVA_boolTyp) = (value); } while (0)
#define KNI_SetStaticByteField(classHandle, fieldID, value) \
    do { STATIC_FIELD(fieldID, jbyte, JBED_JAVA_byteTyp) = (value); } while (0)
#define KNI_SetStaticCharField(classHandle, fieldID, value) \
    do { STATIC_FIELD(fieldID, jchar, JBED_JAVA_charTyp) = (value); } while (0)
#define KNI_SetStaticShortField(classHandle, fieldID, value) \
    do { STATIC_FIELD(fieldID, jshort, JBED_JAVA_shortTyp) = (value); } while (0)
#define KNI_SetStaticIntField(classHandle, fieldID, value) \
    do { STATIC_FIELD(fieldID, jint, JBED_JAVA_intTyp) = (value); } while (0)
#define KNI_SetStaticLongField(classHandle, fieldID, value) \
    do { STATIC_FIELD(fieldID, jlong, JBED_JAVA_longTyp) = (value); } while (0)
#define KNI_SetStaticFloatField(classHandle, fieldID, value) \
    do { STATIC_FIELD(fieldID, jfloat, JBED_JAVA_floatTyp) = (value); } while (0)
#define KNI_SetStaticDoubleField(classHandle, fieldID, value) \
    do { STATIC_FIELD(fieldID, jdouble, JBED_JAVA_doubleTyp) = (value); } while(0)
#define KNI_SetStaticObjectField(classHandle, fieldID, fromHandle) \
    STATIC_FIELD_SETOBJ(fieldID, JBEDKNI_INSTANCE, JBED_JAVA_refTyp, *(fromHandle))

#define KNI_GetStringLength(stringHandle) STRING_LENGTH(*stringHandle)
#define KNI_GetStringRegion(stringHandle, offset, n, jcharbuf) GET_STRING(*(stringHandle), offset, n, (int32_t)jcharbuf)

#define KNI_NewString(uchars, length, stringHandle) do { *(stringHandle) = (JBEDKNI_INSTANCE)CHAR_STRING(uchars, length); } while(0)
#define KNI_NewStringUTF(utf8chars, stringHandle) do { *(stringHandle) = (JBEDKNI_INSTANCE)UTF_STRING(utf8chars); } while(0)

#define KNI_GetArrayLength(arrayHandle) ARRAY_LENGTH(*(arrayHandle))

#define KNI_GetBooleanArrayElement(arrayHandle, index) GET_ELEMENT(*(arrayHandle), index, jboolean, JBED_JAVA_boolTyp)
#define KNI_GetByteArrayElement(arrayHandle, index) GET_ELEMENT(*(arrayHandle), index, jbyte, JBED_JAVA_byteTyp)
#define KNI_GetCharArrayElement(arrayHandle, index) GET_ELEMENT(*(arrayHandle), index, jchar, JBED_JAVA_charTyp)
#define KNI_GetShortArrayElement(arrayHandle, index) GET_ELEMENT(*(arrayHandle), index, jshort, JBED_JAVA_shortTyp)
#define KNI_GetIntArrayElement(arrayHandle, index) GET_ELEMENT(*(arrayHandle), index, jint, JBED_JAVA_intTyp)
#define KNI_GetLongArrayElement(arrayHandle, index) GET_ELEMENT(*(arrayHandle), index, jlong, JBED_JAVA_longTyp)
#define KNI_GetFloatArrayElement(arrayHandle, index) GET_ELEMENT(*(arrayHandle), index, jfloat, JBED_JAVA_floatTyp)
#define KNI_GetDoubleArrayElement(arrayHandle, index) GET_ELEMENT(*(arrayHandle), index, jdouble, JBED_JAVA_doubleTyp)
#define KNI_GetObjectArrayElement(arrayHandle, index, toHandle) do { *(toHandle) = GET_ELEMENT(*(arrayHandle), index, JBEDKNI_INSTANCE, JBED_JAVA_refTyp); } while(0)
#define KNI_GetRawArrayRegion(arrayHandle, offset, n, dstBuffer) GET_ELEMENTS(*(arrayHandle), offset, n, dstBuffer)

#define KNI_SetBooleanArrayElement(arrayHandle, index, value) SET_ELEMENT(*(arrayHandle), index, value, jboolean, JBED_JAVA_boolTyp)
#define KNI_SetByteArrayElement(arrayHandle, index, value) SET_ELEMENT(*(arrayHandle), index, value, jbyte, JBED_JAVA_byteTyp)
#define KNI_SetCharArrayElement(arrayHandle, index, value) SET_ELEMENT(*(arrayHandle), index, value, jchar, JBED_JAVA_charTyp)
#define KNI_SetShortArrayElement(arrayHandle, index, value) SET_ELEMENT(*(arrayHandle), index, value, jshort, JBED_JAVA_shortTyp)
#define KNI_SetIntArrayElement(arrayHandle, index, value) SET_ELEMENT(*(arrayHandle), index, value, jint, JBED_JAVA_intTyp)
#define KNI_SetLongArrayElement(arrayHandle, index, value) SET_ELEMENT(*(arrayHandle), index, value, jlong, JBED_JAVA_longTyp)
#define KNI_SetFloatArrayElement(arrayHandle, index, value) SET_ELEMENT(*(arrayHandle), index, value, jfloat, JBED_JAVA_floatTyp)
#define KNI_SetDoubleArrayElement(arrayHandle, index, value) SET_ELEMENT(*(arrayHandle), index, value, jdouble, JBED_JAVA_doubleTyp)
#define KNI_SetObjectArrayElement(arrayHandle, index, fromHandle) \
    do                                                            \
    {                                                             \
        SET_ELEMENTOBJ(*(arrayHandle), index, *(fromHandle), JBEDKNI_INSTANCE, JBED_JAVA_refTyp); \
        WRITE_BARRIER_ADD(arrayHandle);                           \
    } while (0)
#define KNI_SetRawArrayRegion(arrayHandle, offset, n, srcBuffer) SET_ELEMENTS(*(arrayHandle), offset, n, srcBuffer)

#define KNI_GetParameterAsBoolean(index) (jboolean)(int32_t)PARAMETER(index)
#define KNI_GetParameterAsByte(index) (jbyte)(int32_t)PARAMETER(index)
#define KNI_GetParameterAsChar(index) (jchar)(int32_t)PARAMETER(index)
#define KNI_GetParameterAsShort(index) (jshort)(int32_t)PARAMETER(index)
#define KNI_GetParameterAsInt(index) (jint)PARAMETER(index)
#define KNI_GetParameterAsLong(index) *((jlong*)&PARAMETER(index))
#define KNI_GetParameterAsFloat(index) *((jfloat*)&PARAMETER(index))
#define KNI_GetParameterAsDouble(index) *((jdouble*)&PARAMETER(index))
#define KNI_GetParameterAsObject(index, toHandle) do { *(toHandle) = NOBJ((JBEDKNI_INSTANCE)PARAMETER(index)); } while(0)
#define KNI_GetThisPointer(toHandle) do { *(toHandle) = OBJ((JBEDKNI_INSTANCE)PARAMETER(0)); } while(0)
#define KNI_GetClassPointer(toHandle) do { *(toHandle) = (JBEDKNI_INSTANCE)CLS((JBEDKNI_INSTANCE)PARAMETER(0)); } while(0)

#define KNI_ReturnVoid() do { return; } while (0)
#define KNI_ReturnBoolean(value) do { return (value); } while (0)
#define KNI_ReturnByte(value) do { return (value); } while (0)
#define KNI_ReturnChar(value) do { return (value); } while (0)
#define KNI_ReturnShort(value) do { return (value); } while (0)
#define KNI_ReturnInt(value) do { return (value); } while (0)
#define KNI_ReturnLong(value) do { return (value); } while (0)
#define KNI_ReturnFloat(value) do { return (value); } while (0)
#define KNI_ReturnDouble(value) do { return (value); } while (0)

#define KNI_EndHandlesAndReturnObject(objectHandle) \
    Jbed_nativeCallStateAdr = _tmp_roots_; \
    return NOBJ(*(objectHandle)); }
/* Also see KNI_EndHandles */

/* Include project-specific KNI extensions at end */
#include <kni_ext.h>

#endif /* _JBED_KNI_H_ */
