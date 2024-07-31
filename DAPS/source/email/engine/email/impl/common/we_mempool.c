/*=====================================================================================
FILE NAME   : cmmn_mem.c
MODULE NAME : CMMN

  
    GENERAL DESCRIPTION
    This file define function operating memory.
    
      TechFaith Software Confidential Proprietary
      Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
      =======================================================================================
      Revision History
      
        Modification                   Tracking
        Date              Author       Number       Description of changes
        ----------   --------------   ---------   --------------------------------------
        2006-06-08     BaoCaihua                        create cmmn_mem.c
        
=====================================================================================*/
//pclint
/*lint -e770 */
/*lint -e773 */
/*lint -e631 */
/*lint -e452 */
/*lint -e616 */
/*lint -e766 */
/*lint -e618 */


/*=====================================================================================
Include File Section
=====================================================================================*/
#include "cafhelp.h"
#include "sfs.h"

#include "we_def.h"
#include "tf3rd_for_hz.h"
#include "we_mempool.h"
/*******************************************************************************
*	Macro Define Section
*******************************************************************************/
extern PUBLIC void *email_alloc(uint32 size, const char *file, uint32 line) ;
extern PUBLIC void email_free(void *p);

#define M_MEM_MALLOC(size)    email_alloc(size,__FILE__,__LINE__)
#define M_MEM_FREE      email_free
#define M_MEM_MEMSET    memset
#define M_MEM_MEMCPY    memcpy
#define M_MEM_MEMCMP    memcmp
#define M_MEM_STRCMP    strcmp
#define M_MEM_STRNCMP   strncmp

/*in struct of St_Chunk, it's members uiPrevSize and uiSize take space*/
#define FLAG_EXT    "#$@^&*%~#$@^&*%~#$@^&*%~#$@^&*%~#$@^&*%~#$@^&*%~#$@^&*%~#$@^&*%~#$@^&*%~#$@^&*%~#$@^&*%~#$@^&*%~#$@^&*%~#$@^&*%~#$@^&*%~#$@^&*%~"

/*#define CMP(b)   ((b[0] =='#') && (b[1] == '$') && (b[2] == '@') && (b[3] == '^') && (b[4] == '&') && (b[5] == '*') && (b[6] == '%') && (b[7] == '~') \
&& (b[8] =='#') && (b[9] == '$') && (b[10] == '@') && (b[11] == '^') && (b[12] == '&') && (b[13] == '*') && (b[14] == '%') && (b[15] == '~') \
&& (b[16] =='#') && (b[17] == '$') && (b[18] == '@') && (b[19] == '^') && (b[20] == '&') && (b[21] == '*') && (b[22] == '%') && (b[23] == '~') \
&& (b[24] =='#') && (b[25] == '$') && (b[26] == '@') && (b[27] == '^') && (b[28] == '&') && (b[29] == '*') && (b[30] == '%') && (b[31] == '~'))
*/
#define CMP(b) M_MEM_STRNCMP(b, FLAG_EXT, 128)
#define M_MEM_MGR_HEAD 0

#define M_MEM_CHUNK_HEADER_SIZE (2 * sizeof(WE_UINT32))

/*Every time,least of malloc size*/
#define M_MEM_MIN_CHUNK_SIZE sizeof(St_Chunk)

/*use this variable ensure used spaces that is 4 bytes multiples*/
#define  M_MEM_MALLOC_ALIGN_MASK 3
#define  M_MEM_MAX_BLOCK 10
#define  M_MEM_MALLOC_SIZE 512000
#define  M_MEM_YULIU_SIZE 512000
/*
* We use segregated free lists, with separate lists for
* chunks of different sizes. Currently, we use sizes that are
* powers of two.
* In list number n is kept all the free chunks whose size is
* strictly less than maxsizes[n].
*/
#define  M_MEM_NUM_FREE_LISTS 10

/*the least of module initialization of memory spaces*/
#define  M_MEM_MIN_BLOCK_SIZE (M_MEM_NUM_FREE_LISTS+2) * M_MEM_MIN_CHUNK_SIZE

/*Pointer plus header size*/
#define   CHUNK2MEM(p)    ((void*)(( WE_UCHAR *)(p) + M_MEM_CHUNK_HEADER_SIZE))

/*Pointer minus header size*/
#define MEM2CHUNK(mem)  ((P_St_Chunk)((WE_UCHAR *)(mem) - M_MEM_CHUNK_HEADER_SIZE))

/*Get chunk's size*/
#define CHUNKSIZE(p)        (((p)->uiSize & ~0x01) >> 1)



/*Set chunk's uiPrevSize*/
#define SET_HD1(p, v)   ((p)->uiPrevSize = (WE_UINT32)(v))

/*Set chunk's uiSize*/
#define SET_HD2(p, v)   ((p)->uiSize = (WE_UINT32)(v))

/*Get a pointer of previous chunk*/
#define PREVCHUNK(p)    ((St_Chunk *)(((WE_UCHAR *)(p)) - p->uiPrevSize))

/*Get a pointer of next chunk*/
#define NEXTCHUNK(p)    ((St_Chunk *)(((WE_UCHAR *)(p)) + CHUNKSIZE (p)))

/*Check chunk if freed or not*/
#define CHUNK_ISFREE(p) ((((St_Chunk *)(p))->uiSize & 0X01) == 0)

/*Check chunk if freed or not*/
#define CHUNK_INUSE(p) ((((St_Chunk *)(p))->uiSize & 0X01) == 1)

/*remove a struct from free list*/
#define REMOVE_FROM_LIST(p) \
   (p)->pstFwd->pstBck = (p)->pstBck; \
   (p)->pstBck->pstFwd = (p)->pstFwd;

/*add a struct to free list*/
#define ADD_TO_LIST(l, p) \
   (p)->pstFwd = (l)->pstFwd; \
   (p)->pstBck = l; \
   (l)->pstFwd->pstBck = p; \
   (l)->pstFwd = p;

/*add chunk header and make it become 4 multipes */
#define REQUEST2SIZE(req) \
   (((WE_UINT32)((req) + M_MEM_CHUNK_HEADER_SIZE + M_MEM_MALLOC_ALIGN_MASK) < \
   (WE_UINT32)( M_MEM_MIN_CHUNK_SIZE + M_MEM_MALLOC_ALIGN_MASK)) ? M_MEM_MIN_CHUNK_SIZE : \
   (WE_UINT32)(((req) + M_MEM_CHUNK_HEADER_SIZE + \
   M_MEM_MALLOC_ALIGN_MASK) & ~( M_MEM_MALLOC_ALIGN_MASK)))

/*******************************For Debug**************************************/
/*print error information*/
#define PRINT_ERROR(flag,p) \
   if(flag != 0) \
   { \
    flag = TRUE;\
   } 

#define  GET_MEM_PEAK_VALUE(hMem)   ((St_MemHandle*)hMem)->iPeakSize
#define  GET_TOTAL_MEM_SIZE(hMem)   ((St_MemHandle*)hMem)->iTotalSize
#define  GET_MAX_MEM_SIZE(hMem)     ((St_MemHandle*)hMem)->iMaxSize
#define  GET_MALLOC_MEM_COUNT(hMem) ((St_MemHandle*)hMem)->iMallocCount
#define  GET_FREE_MEM_COUNT(hMem)   ((St_MemHandle*)hMem)->iFreeCount
#define  GET_FREE_MEM_TOTAL_SIZE(hMem)   ((St_MemHandle*)hMem)->iFreeTotalSize
/*******************************************************************************
*	Type Define Section
*******************************************************************************/

/*Declare MEM St_Chunk struct*/
typedef struct tagSt_Chunk
{
   WE_UINT32        uiPrevSize;     /* Size of previous chunk. */
   WE_UINT32        uiSize;    /* Size in bytes, including overhead, inuse bit is also stored here. */
   struct tagSt_Chunk  *pstFwd;     /* Double links -- used only if free. */
   struct tagSt_Chunk  *pstBck;     /* Double links -- used only if free. */   
}St_Chunk, *P_St_Chunk;


/*Declare MEM St_MemBlockHandle struct*/
typedef struct tagSt_MemBlockHandle
{
   WE_UCHAR  *pucBasePtr;           /*Base address for a module*/
   St_Chunk  *pstFirstChunk;        /*Address for the first control struct*/
   St_Chunk  *pstLastChunk;         /*Address for the last control struct*/
   St_Chunk  *apstFreeLists[M_MEM_NUM_FREE_LISTS];  /*Point to 10 free control struct*/
   WE_UINT32  uiAllocatedBytes;     /*Total bytes of malloc spaces*/
   WE_UINT32 uiBlockSize;           /*block initlize size*/
}St_MemBlockHandle, *P_St_MemBlockHandle;

typedef struct tagSt_MemHandle
{
   St_MemBlockHandle    **MemInternal;
   WE_UINT16  uiMaxNo;
   WE_UINT16  usMaxBlock;
   WE_UINT32  uiAllocatedBytes;
   WE_UINT32  uiAllocateAll;
   WE_UINT32  uiTotalMemory;
   WE_UINT32  uiLastMemory;
   WE_INT32   bDel;
}St_MemHandle, *P_St_MemHandle;

#define M_MEM_INIT_SIZE ((M_MEM_NUM_FREE_LISTS + 2) * M_MEM_MIN_CHUNK_SIZE)

static WE_HANDLE WEMemPool_BlockInit(WE_UINT32 iSize);
static WE_HANDLE WEMemPool_AllocateMemory(St_MemBlockHandle* pstMem, WE_UINT32 uiRemSize, WE_UINT32 *puiLen);

const static WE_UINT32 s_MemPool_auiMaxSizes[M_MEM_NUM_FREE_LISTS] =
{
   16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 0x8FFFFFFF  
};

/*******************************************************************************
*	Prototype Declare Section
*******************************************************************************/
static WE_INT16 WEMemPool_ListIdx (WE_UINT32 uiChunkSize)
{
   WE_UINT16 l = 0;
   for(l = 0; s_MemPool_auiMaxSizes[l] < (uiChunkSize + 1); l++);
   return l;
}

WE_HANDLE WEMemPool_CreateMemPool(WE_UINT32 iSize)
{   
   St_MemHandle  *pstMemHandle = NULL;
   
   pstMemHandle = (St_MemHandle*)M_MEM_MALLOC(sizeof(St_MemHandle));
   
   if (NULL == pstMemHandle)
   {
      return NULL;
   }
   M_MEM_MEMSET(pstMemHandle, 0x00, sizeof(St_MemHandle)); 
   
   pstMemHandle->uiMaxNo = 0;
   pstMemHandle->usMaxBlock = M_MEM_MAX_BLOCK;
   pstMemHandle->MemInternal = M_MEM_MALLOC(sizeof(St_MemBlockHandle *) * M_MEM_MAX_BLOCK);
   pstMemHandle->bDel = FALSE;
   if (NULL == pstMemHandle->MemInternal)
   {
      M_MEM_FREE(pstMemHandle);
      return NULL;
   }
   M_MEM_MEMSET(pstMemHandle->MemInternal, 0x00, sizeof(St_MemBlockHandle*) * M_MEM_MAX_BLOCK);
   pstMemHandle->MemInternal[0] = (St_MemBlockHandle*)WEMemPool_BlockInit(iSize);
   if (NULL == pstMemHandle->MemInternal[0])
   {
      M_MEM_FREE(pstMemHandle);
      return NULL;
   }
   pstMemHandle->uiAllocatedBytes = pstMemHandle->MemInternal[0]->uiAllocatedBytes;
   pstMemHandle->uiAllocateAll += iSize;
   
   return pstMemHandle;
}

void WEMemPool_DeleteMemPool(WE_HANDLE hMem)
{
   St_MemHandle *pstMemHandle = NULL;
   St_MemBlockHandle * pstMem = NULL;
   WE_UINT32 i = 0;
   pstMemHandle = (St_MemHandle *)hMem;

   if (NULL == pstMemHandle)
   {
      return;
   }

   for(i = 0; i <= pstMemHandle->uiMaxNo; i++)
   {
      pstMem = pstMemHandle->MemInternal[i];
      if (pstMem->pucBasePtr)
      {
         M_MEM_FREE(pstMem->pucBasePtr);
         pstMem->pucBasePtr = NULL;
      }
      M_MEM_FREE(pstMem); 
   }
   M_MEM_FREE(pstMemHandle->MemInternal);
   M_MEM_FREE(pstMemHandle);
}

static WE_HANDLE WEMemPool_BlockInit(WE_UINT32 iSize)
{
   St_MemBlockHandle *pstMem = NULL;
   WE_UINT32 uiSize = 0;
   WE_INT16       i = 0;
   St_Chunk *pstTemp = NULL;
   
   if((iSize < M_MEM_MIN_BLOCK_SIZE) || (iSize > 0x7FFFFFFF))
   {   
      return NULL;
   }
   pstMem = (St_MemBlockHandle *)M_MEM_MALLOC(sizeof(St_MemBlockHandle));
   if (NULL == pstMem)
   {
      return NULL;
   }
   M_MEM_MEMSET(pstMem, 0X00, sizeof(St_MemBlockHandle));
   
   pstMem->pucBasePtr = (WE_HANDLE)M_MEM_MALLOC(iSize);
   if (NULL == pstMem->pucBasePtr)
   {
      M_MEM_FREE(pstMem);
      return NULL;
   }

   M_MEM_MEMSET(pstMem->pucBasePtr, 0x00, iSize);
   iSize &= ~M_MEM_MALLOC_ALIGN_MASK;
   /*make M_MEM_FREE chunk manage list*/
   for(i = 0; i < M_MEM_NUM_FREE_LISTS; i++)
   {
      pstTemp = (St_Chunk *)(pstMem->pucBasePtr + i * M_MEM_MIN_CHUNK_SIZE);
      SET_HD1(pstTemp, 0);
      SET_HD2(pstTemp, (M_MEM_MIN_CHUNK_SIZE << 1) | 0x01);
      pstTemp->pstFwd = pstTemp;
      pstTemp->pstBck = pstTemp;
      pstMem->apstFreeLists[i] = pstTemp;
   }
   /*set first and last chunk */
   pstMem->pstFirstChunk = (St_Chunk *)(pstMem->pucBasePtr + M_MEM_NUM_FREE_LISTS * M_MEM_MIN_CHUNK_SIZE);
   SET_HD1(pstMem->pstFirstChunk, M_MEM_MIN_CHUNK_SIZE);
   uiSize = iSize - ((M_MEM_NUM_FREE_LISTS + 1) * M_MEM_MIN_CHUNK_SIZE);
   SET_HD2(pstMem->pstFirstChunk, uiSize << 1);
   ADD_TO_LIST(pstMem->apstFreeLists[WEMemPool_ListIdx(uiSize)], pstMem->pstFirstChunk);
   pstMem->pstLastChunk = (St_Chunk *)(((WE_UCHAR *)(pstMem->pucBasePtr)) + iSize - M_MEM_MIN_CHUNK_SIZE);
   SET_HD1(pstMem->pstLastChunk, uiSize);
   SET_HD2(pstMem->pstLastChunk, (M_MEM_MIN_CHUNK_SIZE << 1) | 0x01);
   
   pstMem->uiAllocatedBytes = (M_MEM_NUM_FREE_LISTS + 1) * M_MEM_MIN_CHUNK_SIZE;
   pstMem->uiBlockSize = iSize;
   return (void*)pstMem;
}

static void* WEMemPool_AllocateMemory(St_MemBlockHandle* pstMem,
                                      WE_UINT32 uiNewSize,
                                      WE_UINT32 *puiLen)
{
   St_Chunk *pstTemp = NULL;
   St_Chunk *pstFound = NULL;
   St_Chunk *pstFreeList = NULL;
   St_Chunk *q = NULL;
   St_Chunk *pstNext = NULL; 
   St_Chunk *l = NULL;
   WE_UINT32 uiRemSize = 0;
   WE_UINT32 uiTempSize = 0;
   WE_UINT32 uiSz = 0xFFFFFFFF;
   WE_UINT16 i = 0;
   WE_BOOL bFound = FALSE;
   if (NULL == pstMem || NULL == puiLen)
   {
      return NULL;
   }
   if(NULL == pstMem->pucBasePtr)
   {
      return NULL;
   }
   /*add the header*/
   for(i = WEMemPool_ListIdx(uiNewSize); i < M_MEM_NUM_FREE_LISTS; i++)
   {
      pstFreeList = pstMem->apstFreeLists[i];
      for(pstTemp = pstFreeList->pstFwd; (pstTemp != pstFreeList); pstTemp = pstTemp->pstFwd)
      {
         uiTempSize= CHUNKSIZE(pstTemp);
         if(uiTempSize == uiNewSize)
         {
            pstFound = pstTemp;
            uiSz = uiTempSize;
            bFound = TRUE;
            break;
         }
         else if(uiTempSize > uiNewSize)
         {
            /*make searching size is the most near*/
            if(uiTempSize < uiSz)
            {
               pstFound = pstTemp;
               uiSz = uiTempSize;
            }
         }
      }
      
      if(pstFound != NULL)
      {
         bFound = TRUE;
         break;
      }
   }
   
   if(!bFound)
   {
      return NULL;
   }
   
   /*remove chunk from free list*/
   REMOVE_FROM_LIST(pstFound);
   uiRemSize = uiSz - uiNewSize;
   
   /*if remain size is limited then alloc all apply size*/
   if(uiRemSize > M_MEM_MIN_CHUNK_SIZE)
   {
      uiSz = uiNewSize;
      /*make the remain chunk*/
      q = (St_Chunk *)((WE_UCHAR*)pstFound + uiSz);
      SET_HD2(q,uiRemSize << 1);
      SET_HD1(q,uiNewSize);
      /*modified next chunk's uiPrevSize*/		
      pstNext = NEXTCHUNK(q);
      pstNext->uiPrevSize = uiRemSize;
      l = pstMem->apstFreeLists[WEMemPool_ListIdx(uiRemSize)];
      ADD_TO_LIST(l, q);
   }
   SET_HD2(pstFound, (uiSz << 1) | 0x01);
   
   pstMem->uiAllocatedBytes += CHUNKSIZE(pstFound);
   *puiLen = CHUNKSIZE(pstFound);
   
   return CHUNK2MEM(pstFound);
}

WE_BOOL WEMemPool_Free
(
 WE_HANDLE hMem,
 WE_HANDLE hFree
 )
{
   St_MemHandle *pstMemHandle = NULL;
   St_MemBlockHandle *pstMem = NULL;
   St_Chunk *pstFirst = NULL;
   St_Chunk *pstLast = NULL;
   St_Chunk *pstTemp = NULL;
   St_Chunk *pstNext = NULL;
   St_Chunk *pstPrev = NULL;
   WE_UINT32 uiFreeChunkSize = 0;
   WE_UINT32 uiTempSize = 0;
   St_Chunk *l = NULL;
   WE_UINT32 i = 0;
   WE_INT32 bFlag = FALSE;

   pstMemHandle = (St_MemHandle*)hMem;
   if (NULL == pstMemHandle)
   {
      return FALSE;
   }
   
   if(NULL == hFree)
   {
      return TRUE;
   }

   if(pstMemHandle->bDel)
   {
      return TRUE;
   }
   
   pstTemp = MEM2CHUNK(hFree);
   for(i = 0; i <= pstMemHandle->uiMaxNo; i++)
   {
      pstMem = pstMemHandle->MemInternal[i];
      if (NULL == pstMem)
      {
         return FALSE;
      }
      pstFirst = pstMem->pstFirstChunk;
      pstLast = pstMem->pstLastChunk;
      if ((pstTemp >= pstFirst) && (pstTemp < pstLast))
      {
         bFlag = TRUE;
         break;
      }
   }

   if (!bFlag)
   {
      return TRUE;
   }

   if(CHUNK_ISFREE(pstTemp))
   {
      return TRUE;
   }
   
   uiFreeChunkSize = CHUNKSIZE(pstTemp);
   uiTempSize = uiFreeChunkSize;
   pstPrev = PREVCHUNK(pstTemp);
   pstNext = NEXTCHUNK(pstTemp);
   
   /*if prev chunk is freed, then united this chunk and prev chunk*/
   if(CHUNK_ISFREE(pstPrev))
   {
      uiTempSize += CHUNKSIZE(pstPrev);
      pstTemp = pstPrev;
      REMOVE_FROM_LIST(pstPrev);
   }
   
   /*if next chunk is freed, then united this chunk and next chunk*/
   if(CHUNK_ISFREE(pstNext))
   {
      uiTempSize += CHUNKSIZE(pstNext);
      REMOVE_FROM_LIST(pstNext);
   }
   
   SET_HD2(pstTemp,uiTempSize << 1);
   pstNext = NEXTCHUNK(pstTemp);
   pstNext->uiPrevSize = uiTempSize;
   l = pstMem->apstFreeLists[WEMemPool_ListIdx(uiTempSize)];

   ADD_TO_LIST(l,pstTemp);
   pstMem->uiAllocatedBytes -= uiFreeChunkSize;
   pstMemHandle->uiAllocatedBytes -= uiFreeChunkSize;

   return TRUE;
}

void* WEMemPool_Alloc 
(
 WE_HANDLE hMem,
 WE_UINT32 uiSize
 )
{
   St_MemHandle *pstMemHandle = NULL;
   St_MemBlockHandle *pstMem = NULL;
   St_MemBlockHandle **ppstTempBlockArray = NULL;
   WE_HANDLE *pstFound = NULL;
   WE_UINT32 j = 0;
   WE_UINT32 uiInit = 0;
   WE_UINT32 uiNewSize = 0;
   WE_UINT32 uiLen = 0;
   WE_UINT32 uiRes = 0;

   pstMemHandle = (St_MemHandle *)hMem;

   if (NULL == pstMemHandle)
   {
      return NULL;
   }
   
   uiNewSize = REQUEST2SIZE(uiSize);
   for(j = 0; j <= pstMemHandle->uiMaxNo; j++) 
   {
      pstMem = pstMemHandle->MemInternal[j];
      pstFound = WEMemPool_AllocateMemory(pstMem, uiNewSize, &uiLen);
      if (NULL != pstFound)
      {
         pstMemHandle->uiAllocatedBytes += uiLen;
         return pstFound;
      }
   }

   if (pstMemHandle->uiMaxNo >= pstMemHandle->usMaxBlock - 1)
   {
      ppstTempBlockArray = M_MEM_MALLOC((pstMemHandle->usMaxBlock+M_MEM_MAX_BLOCK) * sizeof(St_MemBlockHandle *));
      if(NULL==ppstTempBlockArray)
      {
         return NULL;
      }
      pstMemHandle->usMaxBlock += M_MEM_MAX_BLOCK;
      M_MEM_MEMSET(ppstTempBlockArray, 0x00, pstMemHandle->usMaxBlock * sizeof(St_MemBlockHandle *));
      M_MEM_MEMCPY(ppstTempBlockArray, pstMemHandle->MemInternal, (pstMemHandle->uiMaxNo+1) *sizeof(St_MemBlockHandle *));
      M_MEM_FREE(pstMemHandle->MemInternal);
      pstMemHandle->MemInternal = ppstTempBlockArray;
   }
   
   if (0 == pstMemHandle->uiTotalMemory) /*user have not set the biggest memory size*/
   {
      if ((uiNewSize + M_MEM_INIT_SIZE) > M_MEM_MALLOC_SIZE) 
      {
         uiInit = uiNewSize + M_MEM_INIT_SIZE;
      }
      else
      {
         uiInit = M_MEM_MALLOC_SIZE;
      }
   }
   else
   {
      uiRes = pstMemHandle->uiTotalMemory - pstMemHandle->uiAllocateAll;
      if ((uiNewSize + M_MEM_INIT_SIZE) > uiRes)
      {
         return NULL;
      }
      if(uiRes <= M_MEM_YULIU_SIZE)
      {
         pstMemHandle->uiLastMemory = TRUE;

         uiInit = uiRes;
      }
      /*the reserve memory already not enough initialize another block*/
      else if (uiRes - M_MEM_YULIU_SIZE <= M_MEM_INIT_SIZE)
      {
         pstMemHandle->uiLastMemory = TRUE;
         uiInit = uiRes;
      }
      else if ((uiNewSize + M_MEM_INIT_SIZE) >= M_MEM_MALLOC_SIZE)
      {
         uiInit = uiNewSize + M_MEM_INIT_SIZE;
      }
      else 
      {
         uiInit = M_MEM_MALLOC_SIZE;
      }
   }
   
   pstMemHandle->MemInternal[pstMemHandle->uiMaxNo + 1] = (St_MemBlockHandle*)WEMemPool_BlockInit(uiInit);
   if (NULL == pstMemHandle->MemInternal[pstMemHandle->uiMaxNo + 1])
   {
      return NULL;
   }
   
   pstMemHandle->uiAllocateAll += uiInit;
   pstMemHandle->uiMaxNo++;
   pstMem = pstMemHandle->MemInternal[pstMemHandle->uiMaxNo];
   pstMemHandle->uiAllocatedBytes += pstMem->uiAllocatedBytes;
   pstFound = WEMemPool_AllocateMemory(pstMem, uiNewSize, &uiLen);
   if (NULL != pstFound)
   { 
      pstMemHandle->uiAllocatedBytes += uiLen;
      return pstFound;
   }
   
   return NULL;
}


void* WEMemPool_Realloc
(
	WE_HANDLE hMem,
   void *src,
   WE_INT32 iLen
   )
{
   St_Chunk    *pstFound = NULL;
   St_Chunk    *pstNext = NULL;
   St_Chunk    *q = NULL;
   St_Chunk    *l = NULL;
   WE_UINT32    uiNext = 0;
   WE_UINT32    uiStr = 0;
   void     *pszTemp =NULL;
   WE_UINT32    uiNewSize = 0;
   WE_UINT32   uiRemSize = 0;
   St_MemBlockHandle *pstMem = NULL;
   WE_UINT32        i = 0;
   St_MemHandle *pstMemHandle = NULL;
   St_Chunk *pstFirst = NULL;
   St_Chunk *pstLast = NULL;
   WE_INT32    bFlag = FALSE;
   
   pstMemHandle = (St_MemHandle *)hMem;
   if(NULL == src)
   {
      pszTemp = WEMemPool_Alloc(hMem, iLen);
      if(NULL == pszTemp)
      {
         return NULL;
      }
      return pszTemp;
   }
   if(0 >= iLen)
   {
      WEMemPool_Free(hMem, src);
      return NULL;
   }
   pstFound = MEM2CHUNK(src);
   for(i = 0; i <= pstMemHandle->uiMaxNo; i++)
   {
      pstMem = pstMemHandle->MemInternal[i];
      if (NULL == pstMem)
      {
         return NULL;
      }
      pstFirst = pstMem->pstFirstChunk;
      pstLast = pstMem->pstLastChunk;
      if ((pstFound >= pstFirst) && (pstFound < pstLast))
      {
         bFlag = TRUE;
         break;
      }
   }

   if (bFlag == FALSE)
   {
      return NULL;
   }
   uiNewSize = REQUEST2SIZE(iLen);
   uiStr = CHUNKSIZE(pstFound);

   /*the last biggest chunk*/
   if (((WE_UCHAR*)pstFound + uiNewSize) >= (WE_UCHAR*)pstLast)
   {
      pszTemp = WEMemPool_Alloc(hMem, uiNewSize);
      if (NULL == pszTemp)
      {
         return NULL;
      }
      memmove(pszTemp, src, uiStr - M_MEM_CHUNK_HEADER_SIZE);
      WEMemPool_Free(hMem, src);
      return pszTemp;
   }
   
   pstNext = NEXTCHUNK(pstFound);
   
   uiNext = CHUNKSIZE(pstNext);
   
   if (uiStr == uiNewSize)
   {
      return src;
   }
   
   /*the realloc size is small than the previously memory.
   M_MEM_FREE the bigger memory, return the previously memory*/
   if (uiStr > uiNewSize)
   {
      uiRemSize = uiStr - uiNewSize;
      if (uiRemSize <= M_MEM_MIN_CHUNK_SIZE)
      {
         return src;
      }
      SET_HD2(pstFound, (uiNewSize << 1) | 0x01);
      q = (St_Chunk *)((WE_UCHAR*)pstFound + uiNewSize);
      if (CHUNK_ISFREE(pstNext))
      {
         uiRemSize += CHUNKSIZE(pstNext);
         REMOVE_FROM_LIST(pstNext);
         pstNext = NEXTCHUNK(pstNext);
      }
      SET_HD2(q,uiRemSize << 1);
      SET_HD1(q,uiNewSize);
      SET_HD1(pstNext, uiRemSize);
      l = pstMem->apstFreeLists[WEMemPool_ListIdx(uiRemSize)];
      ADD_TO_LIST(l,q);
      pstMem->uiAllocatedBytes -= (uiStr - uiNewSize);
      pstMemHandle->uiAllocatedBytes -= (uiStr - uiNewSize);
      return src;
   }
   else
   {
      /*first the next chuck is used, malloc another one for realloc*/
      if (!CHUNK_ISFREE(pstNext))
      {
         pszTemp = WEMemPool_Alloc(hMem, uiNewSize);
         if (NULL == pszTemp)
         {
            return NULL;
         }
         memmove(pszTemp, src, uiStr - M_MEM_CHUNK_HEADER_SIZE);
         WEMemPool_Free(hMem, src);
         return pszTemp;
      }
      else 
      {
         /*next chuck is free but still smaller than realloc size, malloc another one*/
         if (uiNext + uiStr < uiNewSize)
         {
            pszTemp = WEMemPool_Alloc(hMem, uiNewSize);
            if (NULL == pszTemp)
            {
               return NULL;
            }
            memmove(pszTemp, src, uiStr - M_MEM_CHUNK_HEADER_SIZE);
            WEMemPool_Free(hMem, src);
            return pszTemp;
         }
         /*next chuck is free, and the size remain will smaller than min chunk*/
         else if ((uiNext + uiStr - uiNewSize) <= M_MEM_MIN_CHUNK_SIZE)
         {
            uiNewSize = uiNext + uiStr;
            REMOVE_FROM_LIST(pstNext);
            SET_HD2(pstFound, (uiNewSize << 1) | 0x01);
            pstNext = NEXTCHUNK(pstNext);
            SET_HD1(pstNext, uiNewSize);
            pstMem->uiAllocatedBytes += uiNext;
            pstMemHandle->uiAllocatedBytes += uiNext;
            return src;
         }
         /*next chuck is free, and bigger enough for another chunk*/
         else
         {
            REMOVE_FROM_LIST(pstNext);
            pstNext = NEXTCHUNK(pstNext);
            uiRemSize = uiNext - (uiNewSize - uiStr);
            SET_HD2(pstFound, (uiNewSize << 1) | 0x01);
            q = (St_Chunk *)((WE_UCHAR*)pstFound + uiNewSize);
            SET_HD2(q, uiRemSize << 1);
            SET_HD1(q, uiNewSize);
            SET_HD1(pstNext, uiRemSize);
            l = pstMem->apstFreeLists[WEMemPool_ListIdx(uiRemSize)];
            ADD_TO_LIST(l,q);
            pstMem->uiAllocatedBytes += (uiNewSize - uiStr);
            pstMemHandle->uiAllocatedBytes += (uiNewSize - uiStr);
            return src;
         }
      }
   }
}