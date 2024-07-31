#ifndef WE_MEMPOOL_H
#define WE_MEMPOOL_H

#include "we_def.h"

WE_HANDLE WEMemPool_CreateMemPool(WE_UINT32 iSize);
void      WEMemPool_DeleteMemPool(WE_HANDLE hMem);
WE_BOOL   WEMemPool_Free(WE_HANDLE hMem,WE_HANDLE hFree);
void*     WEMemPool_Alloc(WE_HANDLE hMem,WE_UINT32 uiSize);
void*     WEMemPool_Realloc(WE_HANDLE hMem,void *src,WE_INT32 iLen);

#endif