/*
 *
 * Copyright  1990-2009 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */

#include "mmi_sun_java_trc.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "os_api.h"

#include "javacall_memory.h"
//#include "javacall_platform_defs.h"
//#include "javacall_defs.h"
#include "block_mem.h"
#include "mmijava_main.h"

#define JAVA_RESERVED_HEAP_SIZE 851968

//javacall_bool isJavaMemIni = JAVACALL_FALSE;

/** 
 * Allocates large memory heap
 * VM will use this memory heap for internal memory allocation/deallocation
 * Will be called ONCE during VM startup!
 * 
 * @param    size required heap size in bytes
 * @param    outSize actual size of memory allocated
 * @return	  a pointer to the newly allocated memory, or <tt>0</tt> if not available
 */
void* javacall_memory_heap_allocate(long size, /*OUT*/ long* outSize) {
    void * pMemory = NULL;
    long lSize = 0;
    
    //SCI_TRACE_LOW:"javacall_memory_heap_allocate>>>>>>>>>>>"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JAVACALL_MEMORY_53_112_2_18_3_26_38_10,(uint8*)"");

    pMemory = MMI_BL_MALLOC(BLOCK_MEM_POOL_JAVA);/*lint !e64 */
    if(pMemory != NULL)
    {
        lSize = MMIJAVA_GetAppHeapMemSize();
    }
    
    //SCI_TRACE_LOW:"javacall_memory_heap_allocate<<<<<<the size is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JAVACALL_MEMORY_61_112_2_18_3_26_38_11,(uint8*)"d",lSize);
    *outSize = lSize;
    return pMemory;
    
}
    
/** 
 * Free large memory heap
 * VM will call this function once when VM is shutdown to free large memory heap 
 * Will be called ONCE during VM shutdown!
 * 
 * @param    heap memory pointer to free
 */
void javacall_memory_heap_deallocate(void* heap) { 
    MMI_BL_FREE( BLOCK_MEM_POOL_JAVA );/*lint !e64 */
//    isJavaMemIni = JAVACALL_FALSE;
}
    
long javacall_memory_get_app_heap_size() {
    long size = (MMIJAVA_GetAppHeapMemSize() - JAVA_RESERVED_HEAP_SIZE);

    //SCI_TRACE_LOW:"javacall_memory_get_app_heap_size<<<<<<jvm&app heap size is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JAVACALL_MEMORY_81_112_2_18_3_26_38_12,(uint8*)"d",size);
    return size;
}

/** 
 * Allocates memory of the given size from the private JAVACALL memory
 * pool.
 * 
 * @param    size Number of byte to allocate
 * @return	  a pointer to the newly allocated memory
 */
#ifndef TRACK_MEMORY_ALLOC
void* /*OPTIONAL*/ javacall_malloc(unsigned int size){
    return (void*)SCI_ALLOCA(size);
}
#endif
/**
 * Reallocates memory of the given size from the private JAVACALL memory
 * pool.
 *
 * @param size Number of byte to allocate
 * @param ptr	Pointer to previously allocated memory
 * @return a pointer to the reallocated memory or null if memory could not be reallocated
 */
void* /*OPTIONAL*/  javacall_realloc(void* ptr, unsigned int size) {
    return realloc(ptr, size);
}

    
/**
 * Frees memory at the given pointer in the private JAVACALL memory pool.
 * 
 * @param    ptr	Pointer to allocated memory
 */
void  /*OPTIONAL*/ javacall_free(void* ptr) {
	SCI_FREE(ptr);
}
    
#ifdef __cplusplus
}
#endif

 
