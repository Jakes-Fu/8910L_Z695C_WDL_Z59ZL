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

/*******************************************************************************
*	Multi-Include-Prevent Section
*******************************************************************************/
#ifndef MEM_INTERANL_H
#define MEM_INTERANL_H

/*******************************************************************************
*	Include File Section
*******************************************************************************/
/* this headfile contain all type define*/
#include "we_def.h"
#include "tf3rd_for_HZ.h"
#ifdef CMMN_MEM_MEM_DEBUG
#include "aee.h"
#endif
/*******************************************************************************
*	Macro Define Section
*******************************************************************************/

//#define CMMN_MEM_LOW_DEBUG
//#define CMMN_MEM_MEM_DEBUG
#ifdef CMMN_MEM_MEM_DEBUG 
#define  CMMN_MEM_LOW_DEBUG
#endif
#define CMMN_MEM_MALLOC TF3RD_MALLOCMEMORY
#define CMMN_MEM_FREE TF3RD_MEMFREE
#define CMMN_MEM_MEMSET TF3RD_MEMSET
#define CMMN_MEM_MEMCPY TF3RD_MEMCPY
#define CMMN_MEM_MEMMOVE TF3RD_MEMMOVE
#define CMMN_MEM_DBGPRINTF TF3RD_DBGPRINTF
#define CMMN_MEM_STRNCPY TF3RD_STRNCPY
#define CMMN_MEM_STRCPY TF3RD_STRCPY
#define CMMN_MEM_STRLEN TF3RD_STRLEN
#define CMMN_MEM_STRCAT TF3RD_STRCAT
#define CMMN_MEM_STRLEN TF3RD_STRLEN
#define CMMN_MEM_SIZEOF sizeof
/*in struct of St_Chunk, it's members uiPrevSize and uiSize take space*/
#define FLAG_EXT    "#$@^&*%~#$@^&*%~#$@^&*%~#$@^&*%~"
#define CMP(b)   ((b[0] =='#') && (b[1] == '$') && (b[2] == '@') && (b[3] == '^') && (b[4] == '&') && (b[5] == '*') && (b[6] == '%') && (b[7] == '~') \
&& (b[8] =='#') && (b[9] == '$') && (b[10] == '@') && (b[11] == '^') && (b[12] == '&') && (b[13] == '*') && (b[14] == '%') && (b[15] == '~') \
&& (b[16] =='#') && (b[17] == '$') && (b[18] == '@') && (b[19] == '^') && (b[20] == '&') && (b[21] == '*') && (b[22] == '%') && (b[23] == '~') \
&& (b[24] =='#') && (b[25] == '$') && (b[26] == '@') && (b[27] == '^') && (b[28] == '&') && (b[29] == '*') && (b[30] == '%') && (b[31] == '~'))

#ifdef CMMN_MEM_MEM_DEBUG
#define M_MEM_CHUNK_HEADER_SIZE  (2 * sizeof(WE_UINT32)+ STRLEN(FLAG_EXT)+140)
#else
#define M_MEM_CHUNK_HEADER_SIZE   (2 * sizeof(WE_UINT32))
#endif
/*Every time,least of malloc size*/
#define M_MEM_MIN_CHUNK_SIZE sizeof(St_Chunk)


/*use this variable ensure used spaces that is 4 bytes multiples*/
#define  M_MEM_MALLOC_ALIGN_MASK 3
#define  M_MEM_MAX_BLOCK 10
#define  M_MEM_MALLOC_SIZE 512000
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

#define SET_FLAG(p)   (CMMN_MEM_MEMCPY((p)->acFlag,FLAG_EXT, CMMN_MEM_STRLEN(FLAG_EXT)))

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
#define PRINT_ERROR(Flag,p) \
    if(Flag == TRUE) \
    { \
        bFlag = TRUE;\
        CMMN_MEM_DBGPRINTF("\n$$$$$$$$$$$$$$$ERROR$$$$$$$$$$$$$$$$$$$$$\n");   \
        CMMN_MEM_DBGPRINTF("Error!Address is %x\n",p); \
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
#ifdef CMMN_MEM_MEM_DEBUG
    WE_CHAR          acFlag[32];
    WE_CHAR			 aFileName[128];
    WE_UINT32		 uiFileLine;
    WE_UINT32	     uiMallocIndex;
    WE_UINT32        uiReqSize;
#endif
        
    WE_UINT32        uiPrevSize;     /* Size of previous chunk. */
   	WE_UINT32        uiSize;    /* Size in bytes, including overhead, inuse bit is also stored here. */
	struct tagSt_Chunk  *pstFwd;     /* Double links -- used only if free. */
    struct tagSt_Chunk  *pstBck;     /* Double links -- used only if free. */   
}St_Chunk, *P_St_Chunk;


/*Declare MEM St_MemBlockHandle struct*/
typedef struct tagSt_MemBlockHandle{
    WE_UCHAR  *pucBasePtr;           /*Base address for a module*/
    St_Chunk  *pstFirstChunk;        /*Address for the first control struct*/
	St_Chunk  *pstLastChunk;         /*Address for the last control struct*/
	St_Chunk  *apstFreeLists[M_MEM_NUM_FREE_LISTS];  /*Point to 10 free control struct*/
	WE_UINT32  uiAllocatedBytes;     /*Total bytes of malloc spaces*/
	WE_UINT32 uiBlockSize;           /*block initlize size*/
}St_MemBlockHandle, *P_St_MemBlockHandle;

typedef struct tagSt_MemHandle{
    St_MemBlockHandle    **MemInternal;
    WE_UINT16         uiMaxNo;
	WE_UINT16		  usMaxBlock;
    WE_UINT32  uiAllocatedBytes;
    WE_UINT32   uiAllocateAll;
    WE_UINT32  uiTotalMemory;
    WE_UINT32  uiLastMemory;
#ifdef CMMN_MEM_MEM_DEBUG
    WE_UINT32 iPeakSize;
    WE_UINT32 iMaxSize;
    WE_UINT32 iTotalSize;
    WE_UINT32 iMallocCount;
    WE_UINT32 iFreeCount;
    WE_UINT32 iFreeTotalSize;
    WE_HANDLE hFileHandle;
    IFileMgr  *fileMgr;
#endif
}St_MemHandle, *P_St_MemHandle;

void BrwMem_GetMemoryInfo(WE_HANDLE hMem);
WE_HANDLE BrwMem_HandleInit
(
	WE_UINT32 iSize
#ifdef CMMN_MEM_MEM_DEBUG
    ,
    IShell *shell
#endif
	
	);
WE_HANDLE BrwMem_Alloc 
(
 WE_HANDLE hMem,
 WE_UINT32 uiSize
#ifdef CMMN_MEM_MEM_DEBUG
,
 WE_CHAR   *pcFileName,
 WE_UINT32  iLine
#endif 
 );
WE_INT32 BrwMem_Free
(
 WE_HANDLE hMem,
 WE_HANDLE hFree
#ifdef CMMN_MEM_MEM_DEBUG
 ,
 WE_CHAR *pcFile,
 WE_INT32 iLine
#endif
 );
 void BrwMem_FreeAll 
(
 WE_HANDLE hMem
 );
void *BRW_Realloc
(
	WE_HANDLE hMem,
    void *src,
    WE_INT32 iLen
#ifdef CMMN_MEM_MEM_DEBUG
    ,
    WE_CHAR   *pcFileName,
    WE_INT32  iLine
#endif  
    );
void BrwMem_CancelTotalMemorySize(WE_HANDLE hMem);
WE_INT32 BrwMem_SetTotalMemorySize(WE_HANDLE hMem, WE_UINT32 iSize);
WE_UINT32 BrwMem_GetMemoryBlockSize(WE_HANDLE hMem);
void BrwMem_Crash(void);
void  BrwMem_Memmove(WE_CHAR *pcDst, WE_CHAR *pcSrc, WE_INT32 iLen, WE_CHAR *pcFile, WE_INT32 iLine);
void*  BrwMem_Memcpy(WE_HANDLE hDst, WE_HANDLE hSrc, WE_INT32 iLen, WE_CHAR *pcFile, WE_INT32 iLine);
void  BrwMem_Memset(WE_HANDLE hSrc, WE_INT32 iInit, WE_INT32 iLen, WE_CHAR *pcFile, WE_INT32 iLine);
void  BrwMem_Strcat(WE_CHAR *pcDst, WE_CHAR *pcSrc, WE_CHAR *pcFile, WE_INT32 iLine);
void  BrwMem_Strcpy(WE_CHAR *pcDst, WE_CHAR *pcSrc, WE_CHAR *pcFile, WE_INT32 iLine);
void  BrwMem_Strncpy(WE_CHAR *pcDst, WE_CHAR *pcSrc, WE_INT32 iLen, WE_CHAR *pcFile, WE_INT32 iLine);

WE_CHAR* Brw_StrnDup(WE_HANDLE hMem,
                     const WE_CHAR *pcStrSource,
                     WE_INT32 iLen);
WE_CHAR* Brw_Str3Cat(WE_HANDLE hMem,
                     const WE_CHAR *pcStrSource1, 
                     const WE_CHAR *pcStrSource2, 
                     const WE_CHAR *pcStrSource3);

WE_CHAR* Brw_StrDup(WE_HANDLE hMem,
                    const WE_CHAR *pcStrSource);
WE_CHAR* Brw_StrCat
(
 WE_HANDLE  hMem,
 const WE_CHAR *pcStrSource1,
 const WE_CHAR *pcStrSource2
 );


#ifdef CMMN_MEM_LOW_DEBUG
void BrwMem_CheckAll(WE_HANDLE hMem);
#endif
#ifdef CMMN_MEM_MEM_DEBUG
#define CMMN_MEM_BRW_MALLOC(hMem, Param,DebugInfo)       BrwMem_Alloc(hMem, Param,__FILE__,__LINE__)
#define CMMN_MEM_BRW_FREE(hMem, param, debug) \
{\
    if(param != NULL)\
{\
    BrwMem_Free(hMem,param,__FILE__,__LINE__);\
    param = NULL;\
}\
}
#endif

#define CMMN_MEM_BRW_MALLOC(hMem, param, debug) BrwMem_Alloc(hMem, param)
#define CMMN_MEM_BRW_FREE(hMem, param, debug) \
{\
    if(param != NULL)\
{\
    BrwMem_Free(hMem,param);\
    param = NULL;\
}\
}
/*-------------------------------END------------------------------------*/

#endif
