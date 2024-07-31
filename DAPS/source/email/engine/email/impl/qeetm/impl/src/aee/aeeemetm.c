/*=====================================================================================
    FILE NAME   : aeeemetm.c
    MODULE NAME : Email engine


    GENERAL DESCRIPTION
    	Provide email engine functions, include send with SMTP, receive with POP, 
      receive with IMAP, decode EMN.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History

    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                  create aeeemetm.c

=====================================================================================*/


/*===============================================================================
INCLUDES AND VARIABLE DEFINITIONS
=============================================================================== */
#include "aee_emetm_api.h"
#include "we_csc.h" 
#include "eme_utils_api.h"
#include "oem_asyncproc_api.h"
#include "eme_transaction_api.h"
/*-------------------------------------------------------------------
Applet structure. All variables in here are reference via "pMe->"
-------------------------------------------------------------------*/
// create an applet structure that's passed around. All variables in
// here will be able to be referenced as static.

/*-------------------------------------------------------------------
Function Prototypes
-------------------------------------------------------------------*/

/*===============================================================================
FUNCTION DEFINITIONS
=============================================================================== */



/*============================================================================ 
 *    FUNCTION        EMETM_New()
 * 
 *    DESCRIPTION     Initializes the EXAConform Services 	
 * 
 *    DEPENDENCIES    Must be called prior to any other EXAConform
 *                    function calls  
 * 
 *    PARAMETERS      None
 * 
 *    RETURN VALUE    boolean:  TRUE if initialization was successful
 *                               FALSE if initialization failed   
 *  	
 *    SIDE EFFECTS    None	
 * 
 *===========================================================================*/
typedef struct _St_DequeueCallbackData
{
   EMETM         *pEMETM;
   TransID       uiTransId;
}St_DequeueCallbackData;

St_EME_MemHandle stMemHdr = {0};

WE_INT AEEEMETM_Init(IEMETMHandle * po)
{
   WE_INT       iResult = SUCCESS;
   EMETM       *pMe =  NULL;
   
   
   //check input parameters
   if(NULL == po)
   {
      return EME_BADPARM;/*EBADPARM   :If invalid parameters are provided*/
   }
   
   do 
   {
		pMe = EME_UTILS_MALLOC(&stMemHdr, sizeof(EMETM));
      EME_UTILS_BREAKNULL(pMe, iResult, EME_NOMEMORY, "AEEEMETM_Init: failed to malloc!");
		EME_UTILS_MEMSET(pMe, 0x00, sizeof(EMETM));
		
      pMe->pMemHdr = &stMemHdr;
      
      //init  trans callback
      EME_UTILS_MEMSET(&pMe->m_TransCallback, 0 , sizeof(AEECallback));
      CALLBACK_Init(&pMe->m_TransCallback,EMETM_HandleTransactions, pMe->pvTmHandle);
      //init  signal callback
      EME_UTILS_MEMSET(&pMe->m_SigCallback, 0 , sizeof(AEECallback));
      CALLBACK_Init(&pMe->m_SigCallback,  EMETM_SignalProcess, (void*)pMe->pvTmHandle);

        //for pass check
        pMe->piFileMgr = -1;
 
//	   iResult = ISHELL_CreateInstance(pMe->piShell, AEECLSID_NET, (void*)&pMe->piNetMgr);
//      EME_UTILS_BREAKIF(iResult, "EMETM_Init creat net mgr failed");
//      EME_UTILS_BREAKNULL(pMe->piNetMgr, iResult, EFAILED, "EMETM_Init creat net mgr failed");
      
      iResult = WE_CSC_Create(&pMe->pvCSC);
      EME_UTILS_BREAKIF(iResult, "EMETM_Init create csc handle failed");
      EME_UTILS_BREAKNULL(pMe->pvCSC, iResult, EME_FAILED, "EMETM_Init creat net mgr failed");

      /************************************************************************/
      /* init TM handle                                                       */                                                                 
      /************************************************************************/
      iResult = EMETM_InitHandle(&pMe->pvTmHandle,pMe->pMemHdr, pMe);
      EME_UTILS_BREAKIF(iResult, "EMETM_Init, init core failed");
      EME_UTILS_BREAKNULL(pMe->pvTmHandle, iResult, EME_FAILED, "EMETM_Init, init core failed");
		*po = pMe;
      
   } while(0);
   
   
   if(EME_SUCCESS != iResult && NULL != pMe)
   {
/*      EME_UTILS_RELEASEIF(pMe->piNetMgr);*/
		if (pMe->pvCSC)
		{
			WE_CSC_Delete(&pMe->pvCSC);
		}
      CALLBACK_Cancel(&pMe->m_TransCallback);
      CALLBACK_Cancel(&pMe->m_SigCallback);
		
      if (pMe->pvTmHandle)
		{
         EMETM_DeleteHandle(&pMe->pvTmHandle);
      }
		EME_UTILS_FREEIF(pMe->pMemHdr, pMe);
   }

   return iResult;
}
/* This is where the real code goes. In this example the function
 * EXAConform_INIT will let your boss know it is coming and
 * EXAConform_Check which will contact your boss and get a value of
 * EXACmdStsType returned. simply describe effect of public function */

static WE_VOID AEEEMETM_DeleteCB(WE_VOID *pvData)
{
   EMETM*  me = NULL;
   //  WE_UINT32 uiCount = 0;
   
   //check input parameters
   
   if(pvData == NULL)
   {
      return;
   }
   
   do
   {
      me = (EMETM*)pvData;
      
      /************************************************************************/
      /* delete TM lib data                                                   */              
      /************************************************************************/
      if (me->pvTmHandle)
      {
         EMETM_DeleteHandle(&me->pvTmHandle);
      }
      
      
      /************************************************************************/
      /* delete brew platform related data                                    */                             
      /************************************************************************/	
      if (me->pvCSC)
      {
         WE_CSC_Delete(&me->pvCSC);
      }
      
      CALLBACK_Cancel(&me->m_TransCallback);
      CALLBACK_Cancel(&me->m_SigCallback);
      CALLBACK_Cancel(&me->m_DequeueCallback);
      EME_UTILS_FREEIF(me->pMemHdr, me);
   }
   while(0);
   
   return;
}

WE_INT AEEEMETM_Delete(IEMETMHandle *po)
{
   EMETM*  me = NULL;
 //  WE_UINT32 uiCount = 0;
   
   //check input parameters
   
   if(po == NULL || *po == NULL)
   {
      return EME_BADPARM;
   }
   
   me = (EMETM*)*po;

   if(!CALLBACK_IsQueued(&me->m_DeleteCallback))
   {
      AEEEMETM_DeleteCB(me);
//       CALLBACK_Init(&me->m_DeleteCallback, AEEEMETM_DeleteCB, me);
//       IShell_EmailResume(&me->m_DeleteCallback);
   }
   *po = NULL;

   return EME_SUCCESS;
}


WE_INT AEEEMETM_CreateSession(IEMETMHandle po, AEEEMEOpt *pstAccountOpts)
{
   EMETM *pMe = (EMETM*)po;

   if (pMe == NULL || pstAccountOpts == NULL)
   {
      return EME_BADPARM;
   }

   return EMETM_CreateSession(pMe->pvTmHandle, (EMEOpt*)pstAccountOpts);
}



WE_INT AEEEMETM_CloseSession(IEMETMHandle po)
{
   EMETM *pMe = (EMETM*)po;

   if (pMe == NULL)
   {
      return EME_BADPARM;
   }

   return EMETM_CloseSession(pMe->pvTmHandle);
}




WE_INT AEEEMETM_EnqueueTrans(IEMETMHandle po, AEEEMEOpt* pstTransOpts, TransID *puiTransId)
{
   EMETM *pMe = (EMETM*)po;

   EME_UTILS_LOG_INFO(("AEEEMETM_EnqueueTrans Start--"));
   if (pMe == NULL || pstTransOpts == NULL || puiTransId == NULL)
   {
      EME_UTILS_LOG_INFO(("AEEEMETM_EnqueueTrans Bad parameter--"));
      return EME_BADPARM;
   }

   return EMETM_EnqueueTrans(pMe->pvTmHandle, (EMEOpt*)pstTransOpts, puiTransId);
}


/*Add by jade for dequeue transaction*/
static void AEEEMETM_DequeueTransCallback(void* pvData)
{
   St_DequeueCallbackData* pstDequeueData = NULL;

   EME_UTILS_LOG_INFO(("\r\n IQMETM_DequeueTransCallback, begin"));
   if(NULL == pvData)
      return;

   pstDequeueData = (St_DequeueCallbackData*)pvData;

   EME_DequeueTransaction(pstDequeueData->pEMETM->pvTmHandle, pstDequeueData->uiTransId);
   EME_UTILS_FREEIF(pstDequeueData->pEMETM->pMemHdr, pstDequeueData);
   EME_UTILS_LOG_INFO(("\r\n IQMETM_DequeueTransCallback, end"));
}

WE_INT AEEEMETM_DequeueTrans(IEMETMHandle po, TransID uiTransId)
{
   EMETM *pMe = (EMETM*)po;
   WE_INT iResult = EME_SUCCESS;

   St_DequeueCallbackData *pstDequeueData = NULL;
   EME_UTILS_LOG_INFO(("AEEEMETM_DequeueTrans  START uiTransId = 0x08%x !!",uiTransId));
  
   if (pMe == NULL || 0 == uiTransId )
   {
     EME_UTILS_LOG_INFO(("AEEEMETM_DequeueTrans  BADPARAM!!"));
	 
      return EME_BADPARM;
   }

   if(CALLBACK_IsQueued(&pMe->m_DequeueCallback))
   {
      return EME_FAILED;
   }

   do 
   {
      pstDequeueData = EME_UTILS_MALLOC(pMe->pMemHdr, sizeof(St_DequeueCallbackData));
      EME_UTILS_BREAKNULL(pstDequeueData, iResult, EME_NOMEMORY, "no memory for dequeue data");
      EME_UTILS_MEMSET(pstDequeueData, 0, sizeof(St_DequeueCallbackData));

      pstDequeueData->pEMETM = pMe;
      pstDequeueData->uiTransId = uiTransId;

      CALLBACK_Init(&pMe->m_DequeueCallback, AEEEMETM_DequeueTransCallback, (void*)pstDequeueData);
      IShell_EmailResume(&pMe->m_DequeueCallback);

   } while(0);

   return iResult;
}

WE_INT AEEEMETM_DecodeEMN(IEMETMHandle po, AEEEMEOpt* pstDecodeOpts, AEEEMEEmnRet** ppRet)
{
   EMETM *pMe = (EMETM*)po;
   
   if (pMe == NULL || pstDecodeOpts == NULL || NULL == ppRet)
   {
      return EME_BADPARM;
   }
   
   return EMETM_DecodeEMN(pMe->pvTmHandle, (EMEOpt*)pstDecodeOpts, (St_EMEEmnRet**)ppRet);
}


WE_INT AEEEMETM_GetTransNumber(IEMETMHandle po, TransID transId, unsigned int *puiNum)
{
   EMETM *pMe = (EMETM*)po;

   if(NULL == po ||  NULL == puiNum)
      return EME_BADPARM;

   return EMETM_GetTransNumber(pMe->pvTmHandle, transId, puiNum);
}

WE_VOID AEEEMETM_FreeNotifyMemory(WE_VOID* pvMem)
{
   EME_UTILS_FREEIF(NULL, pvMem);
}