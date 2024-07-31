/*=====================================================================================
    FILE: oem_asyncproc.c
    MODULE NAME : IMAP

  
    GENERAL DESCRIPTION:


    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
======================================================================================= 
    Revision History

    Modification                   Tracking
    Date              Author       Number              Description of changes
    --------          -----        ---------------     ----------------------------------
    10/23/2007        Jiangdingqing                            Change copyright
=====================================================================================*/
#include "we_def.h"
#include "eme_error.h"
#include "eme_utils_api.h"
#include "oem_asyncproc_api.h"
#include "eme_sig_api.h"



#define OEM_ASYNCPROC_ALLPROC    0xDDDDDDDD



typedef struct tag_St_AsyncProcNode
{
   WE_VOID     *pvUser;
	PFNASYNC   pfn;  
   WE_UINT32   uiCanceled;

   struct tag_St_AsyncProcNode *pstNext;

} St_AsyncProcNode;

typedef struct tag_St_AsyncProcMgr
{
   WE_VOID           *pvMemMgr;
   WE_VOID           *pvTmHandle;

   St_AsyncProcNode  *pstProcs;
} St_AsyncProcMgr;






WE_INT32 OEM_AsyncProc_int(HAsyncProcMgr *phAsyncMgr, WE_VOID* pvTmHandle, void* pvMemMgr)
{
   St_AsyncProcMgr  *pstAsyncMgr = NULL;
   WE_INT32          iRet = EME_SUCCESS;
   
   
   if (phAsyncMgr == NULL)
   {
      return EME_BADPARM;
   }

   do 
   {
      pstAsyncMgr = EME_UTILS_MALLOC(pvMemMgr, sizeof(St_AsyncProcMgr));
      EME_UTILS_BREAKNULL(pstAsyncMgr, iRet, EME_NOMEMORY, "OEM_AsyncProc_int, init failed");
      EME_UTILS_MEMSET(pstAsyncMgr, 0, sizeof(St_AsyncProcMgr));

      pstAsyncMgr->pvMemMgr = pvMemMgr;
      pstAsyncMgr->pvTmHandle = pvTmHandle;

      *phAsyncMgr = (HAsyncProcMgr *)pstAsyncMgr;

   } while(0);  

   return iRet;
}

WE_INT32 OEM_AsyncProc_AddProc(HAsyncProcMgr hAsyncMgr, PFNASYNC pfn, WE_VOID *pvUser)
{
   St_AsyncProcMgr   *pstAsyncMgr = (St_AsyncProcMgr*)hAsyncMgr;
   WE_INT32          iRet = EME_SUCCESS;
   St_AsyncProcNode  *pstNew = NULL;
   St_AsyncProcNode  *pstTmp = NULL;
   St_AsyncProcNode  *pstLast = NULL;
  
   if (pstAsyncMgr == NULL || pfn == NULL || pvUser == NULL)
   {
      return EME_BADPARM;
   }

   EME_UTILS_LOG_ERROR(("OEM_AsyncProc_AddProc: functon: %x  userdata: %x", pfn, pvUser));

   do 
   {
      pstNew = EME_UTILS_MALLOC(pstAsyncMgr->pvMemMgr, sizeof(St_AsyncProcNode));
      EME_UTILS_BREAKNULL(pstNew, iRet,  EME_NOMEMORY, "OEM_AsyncProc_AddProc, add proc failed");
      EME_UTILS_MEMSET(pstNew, 0, sizeof(St_AsyncProcNode));

      pstNew->pfn = pfn;
      pstNew->pvUser = pvUser;
      pstNew->uiCanceled = 0;

      if (pstAsyncMgr->pstProcs == NULL)
      {
         pstAsyncMgr->pstProcs = pstNew;
         pstAsyncMgr->pstProcs->pstNext = NULL;
      }
      else
      {
         pstTmp = pstAsyncMgr->pstProcs;
         while (pstTmp != NULL)
         {
            pstLast = pstTmp;
            pstTmp = pstTmp->pstNext;
         }

         pstLast->pstNext = pstNew;
         pstNew->pstNext = NULL;
      }
   	

      EME_EVT_SENDTO_SELF_P(pstAsyncMgr->pvTmHandle, EME_FSM_ASYNC, EME_SIG_ASYNC_RESUME, pstAsyncMgr);
      //ISHELL_SetTimer((IShell*)pstAsyncMgr->pvOSAsyncMgr, 0, OEM_AsyncProc_ProcHandle, pstAsyncMgr);
      //OEM_PostEvent_toApp(0x07);

   } while(0);

   return iRet;
}

WE_INT32 OEM_AsyncProc_CancelProc(HAsyncProcMgr hAsyncMgr, PFNASYNC pfn, WE_VOID *pvUser)
{
   St_AsyncProcMgr   *pstAsyncMgr = (St_AsyncProcMgr*)hAsyncMgr;
   WE_INT32          iRet = EME_SUCCESS;
   St_AsyncProcNode  *pstTmp = NULL;
  
   if (pstAsyncMgr == NULL)
   {
      return EME_BADPARM;
   }

   pstTmp = pstAsyncMgr->pstProcs;
   while (pstTmp != NULL)
   {
      if (pstTmp->pfn == pfn || (pvUser != NULL && pstTmp->pvUser == pvUser) || (WE_UINT32)pfn == OEM_ASYNCPROC_ALLPROC)
      {
         EME_UTILS_LOG_ERROR(("OEM_AsyncProc_CancelProc: functon: %x  userdata: %x", pstTmp->pfn, pstTmp->pvUser));
         pstTmp->uiCanceled = 1;
      }
      pstTmp = pstTmp->pstNext;
   }

   return iRet;
}

WE_INT32 OEM_AsyncProc_Delete(HAsyncProcMgr *phAsyncMgr)
{

   St_AsyncProcMgr   *pstAsyncMgr = NULL;

   if (phAsyncMgr == NULL || *phAsyncMgr == NULL)
   {
      return EME_BADPARM;
   }

   pstAsyncMgr = *phAsyncMgr;

   OEM_AsyncProc_CancelProc(*phAsyncMgr, (PFNASYNC)OEM_ASYNCPROC_ALLPROC, NULL);
   
   EME_UTILS_FREEIF(pstAsyncMgr->pvMemMgr, pstAsyncMgr);

   return EME_SUCCESS;
}



WE_INT32 OEM_AsyncProc_ProcHandle(HAsyncProcMgr hAsyncMgr)
{
   St_AsyncProcMgr   *pstAsyncMgr = (St_AsyncProcMgr*)hAsyncMgr;
   St_AsyncProcNode  *pstProNode = NULL;
   PFNASYNC         pfnProc = NULL;
   WE_VOID           *pvUser = NULL;
   WE_UINT32         uiCanceled = 0;

   if (pstAsyncMgr == NULL)
   {
      return EME_BADPARM;
   }

   pstProNode = pstAsyncMgr->pstProcs;

   if (pstProNode != NULL)
   {
      pvUser = pstProNode->pvUser;
      pfnProc = pstProNode->pfn;
      uiCanceled = pstProNode->uiCanceled;

      pstAsyncMgr->pstProcs = pstProNode->pstNext;
      EME_UTILS_FREEIF(pstAsyncMgr->pvMemMgr, pstProNode);

      if (!uiCanceled)
      {
         EME_UTILS_LOG_ERROR(("OEM_AsyncProc_ProcHandle: functon: %x  userdata: %x", pfnProc, pvUser));
         pfnProc(pvUser);
      }
      else
      {
         EME_UTILS_LOG_ERROR(("OEM_AsyncProc_ProcHandle: canceled: %x  userdata: %x", pfnProc, pvUser));
      }
   }
   
   return EME_SUCCESS;
}


WE_VOID OEM_AsyncProc_Reset(HAsyncProcMgr hAsyncMgr)
{
   St_AsyncProcMgr   *pstAsyncMgr = (St_AsyncProcMgr*)hAsyncMgr;
   St_AsyncProcNode  *pstProNode = NULL;
   if(NULL == pstAsyncMgr)
      return;

   pstProNode = pstAsyncMgr->pstProcs;
   while(pstProNode)
   {
      pstAsyncMgr->pstProcs = pstProNode->pstNext;
      EME_UTILS_FREEIF(pstAsyncMgr->pvMemMgr, pstProNode);
      pstProNode = pstAsyncMgr->pstProcs;
   }

   return;
}