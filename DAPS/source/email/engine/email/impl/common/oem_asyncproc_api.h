/*=====================================================================================
    FILE: oem_asyncproc_api.h
    MODULE NAME : IMAP

  
    GENERAL DESCRIPTION:
    

    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
======================================================================================= 
    Revision History

    Modification                   Tracking
    Date              Author       Number              Description of changes
    --------          -----        ---------------     ----------------------------------
    10/23/2007        Jiangdingqing                      Change copyright
=====================================================================================*/
#ifndef _OEM_ASYNCPROC_API_H
#define _OEM_ASYNCPROC_API_H


#include "we_def.h"



typedef WE_VOID* HAsyncProcMgr;

typedef void(*PFNASYNC)(void * pUser);

WE_INT32 OEM_AsyncProc_int(HAsyncProcMgr *phAsyncMgr, WE_VOID* pvOSAsyncMgr, WE_VOID* pvMemMgr);
WE_INT32 OEM_AsyncProc_AddProc(HAsyncProcMgr hAsyncMgr, PFNASYNC pfn, WE_VOID *pvUser);
WE_INT32 OEM_AsyncProc_CancelProc(HAsyncProcMgr hAsyncMgr, PFNASYNC pfn, WE_VOID *pvUser);
WE_INT32 OEM_AsyncProc_Delete(HAsyncProcMgr *phAsyncMgr);
WE_INT32 OEM_AsyncProc_ProcHandle(HAsyncProcMgr hAsyncMgr);
WE_VOID  OEM_AsyncProc_Reset(HAsyncProcMgr hAsyncMgr);



#endif