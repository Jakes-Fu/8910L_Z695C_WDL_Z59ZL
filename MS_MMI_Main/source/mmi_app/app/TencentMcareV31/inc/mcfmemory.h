/******************************************************************************/
// File Name:       McfMemory.h                                           
// Author:          wilsonzhu wiltan                                 
// DATE:            10/01/2011                                  
// Modifed:         haijunqiu 09/26/2011
// Copyright:       2011 Tencent, Incoporated. All Rights Reserved.
// Description:     用来定义内存相关操作接口                   
/******************************************************************************/

#ifndef _MCFMEMORY_H_
#define _MCFMEMORY_H_

#ifdef __cplusplus
    extern  "C"
    {
#endif
/******************************************************************************/
//                      include files
/******************************************************************************/
 #include "mcfcommon.h"

/******************************************************************************/
//                      Macro Declaration
/******************************************************************************/

/******************************************************************************/
//                      Type and constant
/******************************************************************************/

/*****************************************************************************/
//  Function:McfMem_Init
//  Description : Init the memeroy ,it's used in debug mode                     
//  param:
//		IN ----  MVOID
//	       OUT----MVOID
//  return:	       
//  Note: 
/*****************************************************************************/ 
 
MBYTE* mcf_med_ext_smalloc(MUINT nSize);
MVOID mcf_med_ext_sfree(MBYTE* pMem);

 
MINT McfMem_Init(MVOID);

MINT McfMem_Deinit(MVOID);

//主存申请
MVOID* McfMem_Malloc(MINT nMemStyle, MUINT nSize);

//主存释放
MVOID McfMem_Free(MINT nMemStyle, MVOID* pMem);

MUINT McfMem_PoolCreate(MVOID* pMem, MUINT nSize);

MINT McfMem_PoolDelete(MUINT nPoolId);

MVOID* McfMem_PoolMalloc(MUINT nPoolId, MUINT nSize);

MVOID McfMem_PoolFree(MUINT nPoolId, MVOID* pMem);

MVOID* McfMem_PoolMallocDebug(MUINT nPoolId, MUINT nSize, MCHAR* pFileName, MINT nLineNo);

MVOID McfMem_PoolFreeDebug(MUINT nPoolId, MVOID* pMem, MCHAR* pFileName, MINT nLineNo);

MUINT McfMem_PoolGetTotalLeftSize(MUINT nPoolId);

MUINT McfMem_PoolGetMaxAllocSize(MUINT nPoolId);

MVOID* McfMem_ReserveMalloc(MUINT nSize);

MVOID McfMem_ReserveFree(MVOID* pMem);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif //_MCFMEMORY_H_


