/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_MEMORY_H
#define OPDEV_MEMORY_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _OD_MemoryContext
{
	char* base;
	/** Pool size. */
	int size;
	/** Used size. */
	int used;
} OD_MemoryContext;

OD_ALL_TASK OD_STATUS OD_MemoryCreatePool(OD_Context *context, char** mempool, int* pool_size, int default_size, int min_size);

OD_ALL_TASK OD_STATUS OD_MemoryDestroyPool(OD_Context *context);

OD_ALL_TASK int OD_MemoryGetPoolSize(OD_Context *context);

OD_ALL_TASK void* OD_MemoryGetPoolBase(OD_Context *context);

/* OD_Malloc/OD_Free, added for HULA-797: support discontinuous memory allocation.
We need to allocate memory block by block, but not only allocate a big one. */

#ifndef OD_MEMORY_ALIGN
#define OD_MEMORY_ALIGN (8)
#endif

OD_MMI_TASK OD_STATUS OD_MemoryBlocksInit(OD_Context* context, int* pool_size, int default_size, int min_size);

OD_MMI_TASK OD_STATUS OD_MemoryBlocksDeinit(OD_Context* context);

/**
 * Allocates memory.
 *
 * @note It's designed to be backend of memory management module, like lea-malloc.
 * @note It may allocate memory from a memory pool. In this case, the memory will
 *       not be actually released until the the memory pool is destroyed, even
 *       if you explicitly call OD_Free to free the buffer. It means that the memory
 *       freed by OD_Free may not be reused any more before application exits.
 * @note Please pay attention if you can use OD_Malloc/OD_Free for some temporary
 *       memory usage scenarios, esp. in OpDev. Strongly recommended that you don't
 *       use OD_Malloc/OD_Free directly in these cases, but use platform SDK APIs
 *       or introduce other functions which can be implemented either based on SDK
 *       APIs or simply as a wrapper of OD_Malloc/OD_Free.
 *
 * @param[in] context
 * @param[in] size the required memory's size
 * @return a pointer pointing to the allocated memory. NULL if allocating failed.
 */
OD_MMI_TASK void* OD_Malloc(OD_Context* context, int size);

/**
 * Frees memory allocated from OD_Malloc.
 *
 * @param[in] context
 * @param[in] ptr a pointer pointing to the memory to be freed
 */
OD_MMI_TASK void OD_Free(OD_Context* context, void* ptr);

#ifdef __cplusplus
};
#endif


#endif /* OPDEV_MEMORY_H */
