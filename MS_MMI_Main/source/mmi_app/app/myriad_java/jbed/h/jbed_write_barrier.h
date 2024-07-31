/*
 * Copyright 2009 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Write Barrier GC notification
 *
 * Define macro APIs that notify GC when a reference field in an instance
 * or a static reference field are modified by C code.
 */

#ifndef __JBED_WRITE_BARRIER_H__
#define __JBED_WRITE_BARRIER_H__

/* An empty definition for non-GGC builds. */
#define WRITE_BARRIER_ADD(ptr)

#ifdef JBED_GENERATIONAL_GC

/* For access to java static fields */
#include <jbed_static_com_jbed_runtime_rememberedset.h>
#include <jbedcoreinstance.h>
/* prototypes for Jbni_flushSSB. */
#include <jbedupcalls.h>

/* To remove the dependency from javaStatic.h, the followings are defined. */
extern jint JbniF_com_jbed_runtime_RememberedSet_sequentialStorePtr;
#define STATIC_com_jbed_runtime_RememberedSet_sequentialStorePtr (JbniF_com_jbed_runtime_RememberedSet_sequentialStorePtr)

#undef WRITE_BARRIER_ADD
/**
 * Add a pointer to the sequential store buffer.
 * @param ptr - the non-null reference of the object that will be added
 */
#define WRITE_BARRIER_ADD(ptr)                                           \
    do                                                                   \
    {                                                                    \
        *(uint32_t*)(STATIC_com_jbed_runtime_RememberedSet_sequentialStorePtr)\
            =  (uint32_t)ptr;                                            \
        STATIC_com_jbed_runtime_RememberedSet_sequentialStorePtr += 4;   \
        if(STATIC_com_jbed_runtime_RememberedSet_sequentialStorePtr <<   \
            STATIC_com_jbed_runtime_RememberedSet_SSB_SHIFT == 0)        \
        {                                                                \
            Jbni_flushSSB();                                             \
        }                                                                \
    } while (0)
#endif

/**
 * Write to a reference field within an object and notify GC of the write.
 * @param objRef - the non-null reference of the object that is being modified
 * @param fieldPtr - the address of the field being written
 * @param value - the value being written into a field in objRef
 */
#define Jbni_writeBarrierAddr(objRef,fieldPtr,value)                     \
    do                                                                   \
    {                                                                    \
        void* __v__ = (value);                                           \
        *(uint32_t*)(fieldPtr) = (uint32_t)__v__;                        \
        WRITE_BARRIER_ADD(objRef);                                       \
    } while (0)

/**
 * Write into an element of reference-type array object and notify GC of
 * the write.
 * @param objRef - the non-null reference of the object that is being modified
 * @param field  - the 'C' name of the data array in the object
 * @param idx    - the index of the array being written
 * @param value  - the value being written into field[idx] in objRef
 */
#define Jbni_writeBarrierArray(objRef,field,idx,value)                   \
    do                                                                   \
    {                                                                    \
        uint32_t __idx__ = (idx);                                        \
        Jbni_writeBarrierAddr(objRef, &((objRef)->field[__idx__]), value);  \
    } while (0)

/**
 * Write to a reference field in an object and notify GC of the write.
 * @param objRef - the non-null reference of the object that is being modified
 * @param field - the name of the field being written
 * @param value - the value being written into a field in objRef
 */
#define Jbni_writeBarrierObj(objRef,field,value)                         \
    Jbni_writeBarrierAddr(objRef, &((objRef)->field), value)


/**
 * Write to a static reference field of rommed class and notify GC of the write
 * @param fieldPtr - the address of the static cell being modified
 * @param value - the value being written into the static field
 */
#define Jbni_writeBarrierRommedStatic(fieldPtr,value)                    \
    do                                                                   \
    {                                                                    \
        void* __v__ = (value);                                           \
        *(void**)(fieldPtr) = __v__;                                     \
        /* No write barriers in for rommed static fields */              \
    } while (0)


/**
 * Write to a static reference field of loaded class and notify GC of the write
 * @param clazz - the CLASS of the static field.
 * @param fieldPtr - the address of the static cell being modified
 * @param value - the value being written into the static field
 */
#define Jbni_writeBarrierStatic(clazz,fieldPtr,value)                    \
    do                                                                   \
    {                                                                    \
        void* __v__ = (value);                                           \
        *(void**)(fieldPtr) = __v__;                                     \
        /* No write barriers in for static fields */                     \
    } while (0)


/**
 * Note that a write to a block of array cells containing references has
 * occurred (or is about to occur). This does NOT do the write or any checks.
 *
 * @param dstArr - the reference of the destination array.
 * @param dstOffs - the lowest index into the destination array.
 * @param srcArr - the reference of the source array.
 * @param srcOffs - the lowest index into the source array.
 * @param nElems - the number of elements copied.
 */
#define Jbni_writeBarrierNoteArrayCopy(dstArr,dstOffs,srcArr,srcOffs,nElems) \
    do                                                                   \
    {                                                                    \
        UNUSED(dstArr);                                                  \
        UNUSED(srcOffs);                                                 \
        UNUSED(srcArr);                                                  \
        UNUSED(srcOffs);                                                 \
        WRITE_BARRIER_ADD(dstArr);                                       \
    } while (0)


#endif /* __JBED_WRITE_BARRIER_H__ */
