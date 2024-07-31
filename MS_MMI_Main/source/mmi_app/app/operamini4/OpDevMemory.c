/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/

#include "opcfg.h"
#include "opdevinternal.h"
#include "opimport.h"

#ifdef OD_TWEAK_MEMORY_STATIC_DEBUG_POOL
static char od_debug_memory_pool[OD_MEMORY_DEBUG_POOL_SIZE];
#endif /* OD_TWEAK_MEMORY_STATIC_DEBUG_POOL */

OD_STATUS OD_MemoryCreatePool(OD_Context *context, char** mempool, int* pool_size, int default_size, int minsize)
{
	char* buffer = NULL;
	int pool_step = 0;

	OD_TRACE_LOW("op:OD_MemoryCreatePool minsize=%d default_size=%d", minsize, default_size);
	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(mempool);
	OD_CHECK_ARGS_RETURN_STATUS(pool_size);
	OD_CHECK_ARGS_RETURN_STATUS(default_size > minsize);

	/* ASSERT OD_MemoryCreatePool can't be called twice. */
	OD_ASSERT(!context->data.memory.base);

	pool_step = (default_size - minsize) >> 3;
	if (pool_step < OD_MEMORY_POOL_DEFAULT_STEP)
		pool_step = OD_MEMORY_POOL_DEFAULT_STEP;

#ifdef OD_TWEAK_MEMORY_STATIC_DEBUG_POOL
	*pool_size = OD_MEMORY_DEBUG_POOL_SIZE;
#else
	*pool_size = default_size;
#endif /* OD_TWEAK_MEMORY_STATIC_DEBUG_POOL */

	do
	{
		OD_TRACE_LOW("Allocating Memory... %d KBytes", (*pool_size) >> 10);
#ifdef OD_TWEAK_MEMORY_STATIC_DEBUG_POOL
		buffer = od_debug_memory_pool;
#else
		buffer = (char*)SCI_ALLOCA(*pool_size);
#endif /* OD_TWEAK_MEMORY_STATIC_DEBUG_POOL */

		if (buffer)
		{
			break;
		}
		else
		{
			*pool_size -= pool_step;
		}
	} while (*pool_size >= minsize);

	OD_TRACE_LOW("Allocated Memory: buffer = 0x%x, size = %d", buffer, buffer ? *pool_size : 0);
	if (buffer)
	{
		*mempool = buffer;

		context->data.memory.base = buffer;
		context->data.memory.size = *pool_size;

		return OD_SUCCESS;
	}
	else
	{
		*mempool = NULL;
		*pool_size = 0;

		context->data.memory.base = NULL;
		context->data.memory.size = 0;

		return OD_NO_MEMORY;
	}
}

OD_STATUS OD_MemoryDestroyPool(OD_Context *context)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);
#ifndef OD_TWEAK_MEMORY_STATIC_DEBUG_POOL
	if (context->data.memory.base)
	{
		SCI_FREE(context->data.memory.base);
	}
#endif /* OD_TWEAK_MEMORY_STATIC_DEBUG_POOL */
	context->data.memory.base = NULL;
	context->data.memory.size = 0;

	return OD_SUCCESS;
}

void* OD_MemoryGetPoolBase(OD_Context *context)
{
	OD_CHECK_ARGS(context, NULL);

	return context->data.memory.base;
}

int OD_MemoryGetPoolSize(OD_Context *context)
{
	OD_CHECK_ARGS(context, -1);

	return context->data.memory.size;
}

OD_STATUS OD_MemoryBlocksInit(OD_Context* context, int* pool_size, int default_size, int min_size)
{
	OD_STATUS ret = OD_SUCCESS;
#ifdef OD_TWEAK_ALLOCATE_MEMORY_ONCE
	char* pool;
	ret = OD_MemoryCreatePool(context, &pool, pool_size, default_size, min_size);
#else
	*pool_size = default_size;
	context->data.memory.base = NULL;
	/* Here context->data.memory.size means the memory size we can use. */
	context->data.memory.size = default_size;
	context->data.memory.used = 0;
#endif
	OD_TRACE_LOW("OD_MemoryBlocksInit: pool_size = %d, default_size = %d, min_size = %d", *pool_size, default_size, min_size);
	return ret;
}

OD_STATUS OD_MemoryBlocksDeinit(OD_Context* context)
{
#ifdef OD_TWEAK_ALLOCATE_MEMORY_ONCE
	return OD_MemoryDestroyPool(context);
#else
	return OD_SUCCESS;
#endif
}

void* OD_Malloc(OD_Context* context, int size)
{
	char* ptr = NULL;
#ifdef OD_BUILD_OPERA_MOBILE
	OD_ASSERT(!"Opera mobile shouldn't use OD_Malloc now");
	return NULL;
#endif

	OD_ASSERT(size > 0);
#ifdef OD_TWEAK_ALLOCATE_MEMORY_ONCE
	OD_ASSERT(context->data.memory.base);

	ptr = context->data.memory.base + context->data.memory.used;

	if ((unsigned int)ptr % OD_MEMORY_ALIGN != 0)
	{
		ptr += OD_MEMORY_ALIGN - (unsigned int)ptr % OD_MEMORY_ALIGN;
	}

	if (ptr + size > context->data.memory.base + context->data.memory.size)
	{
		ptr = NULL;
	}
	else
	{
		context->data.memory.used = ptr + size - context->data.memory.base;
	}
#else
	if (context->data.memory.used + size > context->data.memory.size)
	{
		ptr = NULL;
	}
	else
	{
		ptr = SCI_ALLOCA(size);
		if (ptr != NULL)
		{
			context->data.memory.used += size;
		}
	}
#endif
	OD_TRACE_LOW("OD_Malloc: size = %d, ptr = 0x%x, already used = %d", size, ptr, context->data.memory.used);

	return ptr;
}

void OD_Free(OD_Context* context, void* ptr)
{
	/* Actually opera lea_malloc memory management don't free memory to system until exit opera.
	So don't update context->data.memory.used here */
	OD_ASSERT(ptr);
#ifdef OD_TWEAK_ALLOCATE_MEMORY_ONCE
	/* do nothing */
	(ptr);
#else
	if (ptr != NULL)
	{
		SCI_FREE(ptr);
	}
#endif
}
