/*=====================================================================================
FILE NAME   : eme_tm.c
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Provide some function for managing transaction.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                  create eme_tm.c
          
=====================================================================================*/


/*===================================================================
 * 
 *                     Include Files
 * 
====================================================================*/
/*Include System head file*/

/*Include Program Global head file*/
#include "eme_utils_api.h"
#include "eme_error.h"
/*Include Module head file*/
#include "oem_tmlib_api.h"
#include "eme_tm_utils_api.h"
#include "eme_transaction_api.h"
#include "eme_sig_api.h"
#include "eme_tm_api.h"
#include "eme_fsm_smtp_api.h"
#include "eme_fsm_imap_api.h"
#include "eme_fsm_pop_api.h"
#include "eme_smtp_api.h"
#include "eme_imap_api.h"
#include "eme_pop_api.h"
#include "eme_emn_api.h"
#include "oem_asyncproc_api.h"
/*Include Private head file*/

WE_UINT32    g_email_netid = 0;

/*=============================================================================
* 
*     DEFINITIONS AND CONSTANTS(It is not recommended)
* It is recommended that this section is defined to private head file.
* 
=============================================================================*/

/*=============================================================================
*
*   Static Function Prototype Declare Section
*
=============================================================================*/
static WE_INT32 EMETM_CheckNeedOpts(EMEOpt* pstAccountOpts,
                                    EMEOpt* pstTransOpts,
                                    WE_UINT32 uiTransType);

/*=============================================================================
*
*   Global Variable Declare Section
*
=============================================================================*/

/*=============================================================================
*
*   File Static Variable Define Section
*
=============================================================================*/



/*===================================================================
* 
*                    Functions
* 
* ==================================================================*/
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EMETM_CreateSession   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. copy new account info to tm handle
 *  
 * ===================================================================================================*/
WE_INT32 EMETM_CreateSession(ITMHandle hTMHandle, EMEOpt *pstAccountOpts)
{
   WE_INT32    iRet = EME_SUCCESS;
   St_TMHandle *pstTmHandle = (St_TMHandle*)hTMHandle;
   WE_VOID     *pvMemHdr = NULL;
   EMEOpt      *pstOptsTmp = NULL;     


   if (pstTmHandle == NULL || pstAccountOpts == NULL)
   {
      return EME_FAILED;
   }


   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      pstOptsTmp = pstTmHandle->pstAccountOpts;
      /*copy new account info to tm handle*/
      pstTmHandle->pstAccountOpts = EME_CopyAllOpts(pvMemHdr, pstAccountOpts);
      EME_UTILS_BREAKNULL(pstTmHandle->pstAccountOpts, iRet, EME_FAILED, "EMETM_CreateSession, copy account opt failed");      

      if (pstOptsTmp != NULL)
      {
         /*free old account info*/
         EME_FreeAllOptValue(pvMemHdr,pstOptsTmp);
         EME_UTILS_FREEIF(pvMemHdr, pstOptsTmp);
      }

   } while(0);

   if (iRet != EME_SUCCESS)
   {
      pstTmHandle->pstAccountOpts = pstOptsTmp;
   }

   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EMETM_CloseSession   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. free account info in tm handle
 *  
 * ===================================================================================================*/
WE_INT32 EMETM_CloseSession(ITMHandle hTMHandle)
{
   WE_INT32    iRet = EME_SUCCESS;
   St_TMHandle *pstTmHandle = (St_TMHandle*)hTMHandle;
   WE_VOID     *pvMemHdr = NULL;

   if (pstTmHandle == NULL)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   EME_FreeAllOptValue(pvMemHdr, pstTmHandle->pstAccountOpts);
   EME_UTILS_FREEIF(pvMemHdr, pstTmHandle->pstAccountOpts);

   return iRet;

}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EMETM_InitHandle   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. init pending queue,free list and trans queue 
 *              3. init smtp ,pop, imap emn handle and set callback
 * ===================================================================================================*/
WE_INT32 EMETM_InitHandle(ITMHandle *phTMHandle, WE_VOID* pMemHdr, WE_VOID *pvParent)
{   
   St_TMHandle *pHandle = NULL; 
   WE_INT32    iRet = EME_SUCCESS;
   WE_VOID     *pvFileMgr = NULL;
   WE_VOID     *pvIShell = NULL;
   WE_VOID     *pvHCSC = NULL;
   WE_VOID     *pvINetMgr = NULL;

   if(NULL == phTMHandle)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      pHandle = (St_TMHandle*)EME_UTILS_MALLOC(pMemHdr , sizeof(St_TMHandle));
      EME_UTILS_BREAKNULL(pHandle, iRet, EME_FAILED, "MMSETM_InitHandle, no memeory");
      EME_UTILS_MEMSET(pHandle, 0, sizeof(St_TMHandle));
      /************************************************************************/
      /* init pending queue,free list and trans queue                         */                                            
      /************************************************************************/
      EME_INIT_LIST_HEAD(&pHandle->stTransQueueFirst);
      EME_INIT_LIST_HEAD(&pHandle->stTransPendingQueueFirst);
      EME_INIT_LIST_HEAD(&pHandle->stTransFreeList);
      EME_INIT_LIST_HEAD(&pHandle->stSignalFreeList);
      EME_INIT_LIST_HEAD(&pHandle->stSignalQueueFirst);
      pHandle->iTransQueueLength = 0;
      pHandle->iTransFreeListLength = 0;
      pHandle->iSignalFreeListLength = 0;
      pHandle->pvParent = pvParent;
      
      //pvFileMgr = EME_OEMTMLib_GetIFileMgr(pHandle);
      //EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EMETM_InitHandle, get filemgr failed");
   
      //pvINetMgr = OEMTMLib_GetINetMgr(pHandle);
      //EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EMETM_InitHandle, get netmgr failed");

      //pvIShell = EME_OEMTMLib_GetIShell(pHandle);
      //EME_UTILS_BREAKNULL(pvIShell, iRet, EME_FAILED, "EMETM_InitHandle, get netmgr failed");

      pvHCSC = OEMTMLib_GetHCSC(pHandle);
      EME_UTILS_BREAKNULL(pvHCSC, iRet, EME_FAILED, "EMETM_InitHandle, get csc handle failed");

      iRet = OEM_AsyncProc_int(&pHandle->pvAsyncMgr, pHandle, pMemHdr);
      EME_UTILS_BREAKIF(iRet, "EMETM_Init, init core failed");
      EME_UTILS_BREAKNULL(pHandle->pvAsyncMgr, iRet, EME_FAILED, "EMETM_InitHandle, init async failed");
      /* init smtp handle and set callback */
      iRet = EME_SMTP_InitHandle(&pHandle->pvSMPT, pMemHdr, pvIShell, pHandle->pvAsyncMgr, pvFileMgr, pvINetMgr);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendStart, creat smtp handle failed");
      EME_UTILS_BREAKNULL(pHandle->pvSMPT, iRet, EME_FAILED, "EMETM_InitHandle, creat smtp handle failed");

      iRet = EME_SMTP_SetCallBack(pHandle->pvSMPT, EME_SMTPActionCB, pHandle);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendStart, set smtp callback failed");
     

      /* init pop handle and set callback */
      iRet = EME_POP_InitHandle(&pHandle->pvPOP, pMemHdr, pvIShell, pHandle->pvAsyncMgr, pvFileMgr, pvINetMgr);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendStart, creat pop handle failed");
      EME_UTILS_BREAKNULL(pHandle->pvPOP, iRet, EME_FAILED, "EMETM_InitHandle, creat pop handle failed");

      iRet = EME_POP_RegCallBack(pHandle->pvPOP, EME_POPActionCB, pHandle);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendStart, set pop callback failed");

      
      /* init imap handle and set callback */
      iRet = EME_IMAP_InitHandle(&pHandle->pvIMAP, pMemHdr, pvIShell, pHandle->pvAsyncMgr, pvFileMgr, pvINetMgr);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendStart, creat imap handle failed");
      EME_UTILS_BREAKNULL(pHandle->pvPOP, iRet, EME_FAILED, "EMETM_InitHandle, creat imap handle failed");

      iRet = EME_IMAP_SetCallback(pHandle->pvIMAP, EME_IMAPActionCB, pHandle);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendStart, set pop callback failed");

//      iRet = EME_EMN_InitHandle(&pHandle->pvEMN, pvIShell, pMemHdr, pvHCSC);
//      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendStart, creat emn handle failed");
//      EME_UTILS_BREAKNULL(pHandle->pvEMN, iRet, EME_FAILED, "EMETM_InitHandle, creat emn handle failed");

      /* record smtp init state */
      pHandle->iSMPTState = EME_STACK_STATE_LOGOUT;
      pHandle->iPOPState = EME_STACK_STATE_LOGOUT;
      pHandle->iIMAPState = EME_STACK_STATE_LOGOUT;

      *phTMHandle = pHandle;

   } while(0);
   
   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EMETM_DeleteHandle   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. free account info in tm handle 
 *              3. terminate transaction queue and signal queue
                4. delet smtp,pop,imap,emn handle
                5. free tm handle
 * ===================================================================================================*/
WE_INT32 EMETM_DeleteHandle(ITMHandle *phTMHandle)
{
   void           *pvMemHdr = NULL;
   St_TMHandle    *pstTmHandle = NULL;

   /*check input parameters*/
   if(NULL == phTMHandle || NULL == *phTMHandle)
   {
      return EME_FAILED;
   }
   
   pstTmHandle = (St_TMHandle*)*phTMHandle;

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do
   {

      /* Tm Handle was deleted, all transaction should be canceled */
      pstTmHandle->iCanBeCancel = EME_CANCEL_ENABLED;

      EME_FreeAllOptValue(pvMemHdr, pstTmHandle->pstAccountOpts);
      EME_UTILS_FREEIF(pvMemHdr, pstTmHandle->pstAccountOpts);

      EME_TransactionTerminate(*phTMHandle);
      EME_SignalTerminate(*phTMHandle);

      
      EME_SMTP_DeleteHandle(&pstTmHandle->pvSMPT);
      EME_POP_DeleteHandle(&pstTmHandle->pvPOP);
      EME_IMAP_DeleteHandle(&pstTmHandle->pvIMAP);
      //EME_EMN_DeleteHandle(&pstTmHandle->pvEMN);
      OEM_AsyncProc_Delete(&pstTmHandle->pvAsyncMgr);

      pstTmHandle->iSMPTState = EME_STACK_STATE_LOGOUT;
      pstTmHandle->iIMAPState = EME_STACK_STATE_LOGOUT;
      pstTmHandle->iPOPState = EME_STACK_STATE_LOGOUT;

      EME_UTILS_FREEIF(pvMemHdr, *phTMHandle);

      *phTMHandle = NULL;
        
   } while(0);
   
   return EME_SUCCESS;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EMETM_EnqueueTrans   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. copy trans info & account info 
                3. add new transaction to transaction queue
 * ===================================================================================================*/
WE_INT32 EMETM_EnqueueTrans(ITMHandle hTMHandle,
                          EMEOpt* pstTransOpts,
                          TransID *puiTransId)
{
   St_TransEvt *pstTransEvt = NULL;
   void        *pvMemHdr = NULL;
   WE_INT32    iRet = EME_SUCCESS;
   St_TMHandle *pstTmHandle = (St_TMHandle*)hTMHandle;
   WE_UINT32   uiClsId = 0;
   WE_UINT32   uiTransType = 0;


   EME_UTILS_LOG_INFO(("EMETM_EnqueueTrans Start--"));
		    
		    
   /*check input parameters*/
   if(NULL == pstTmHandle ||
      NULL == pstTransOpts ||
      NULL == puiTransId)
   {
      return EME_FAILED;
   }
   
   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);
   do 
   {
      iRet = EME_GetIntOptValue(pstTransOpts, EME_OPTID_TRANS_TYPE, (WE_INT32*)&uiTransType);
      EME_UTILS_LOG_INFO(("EMETM_EnqueueTrans: uiTransType = %d", uiTransType));
      EME_UTILS_BREAKIF(iRet, "EMETM_EnqueueTrans, get trans type failed");

      iRet = EMETM_CheckNeedOpts(pstTmHandle->pstAccountOpts, pstTransOpts, uiTransType);
      EME_UTILS_BREAKIF(iRet, "EMETM_EnqueueTrans, in put opts error");

      /* generate transaction details and status */
      pstTransEvt = (St_TransEvt*)EME_UTILS_MALLOC(pvMemHdr, sizeof(St_TransEvt));
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "MMSETM_EnqueueTrans, no memory!");
      EME_UTILS_MEMSET(pstTransEvt, 0, sizeof(St_TransEvt));
      
      /* no error occurs in signal operation */
      pstTransEvt->iCurrSignalID = EME_SIG_NULL;
      pstTransEvt->uiParamValidate = EME_EVT_PARAM_VALID_ID;
      pstTransEvt->iErrCode = EME_SUCCESS;
     
      /*Copy Trans Opts*/
      pstTransEvt->pstTransOpts = EME_CopyAllOpts(pvMemHdr, pstTransOpts);
      EME_UTILS_BREAKNULL(pstTransEvt->pstTransOpts, iRet, EME_FAILED, "EMETM_EnqueueTrans, copy opts failed");

      /*Copy Account Opts*/
      pstTransEvt->pstAccountOpts = EME_CopyAllOpts(pvMemHdr, pstTmHandle->pstAccountOpts);
      EME_UTILS_BREAKNULL(pstTransEvt->pstAccountOpts, iRet, EME_FAILED, "EMETM_EnqueueTrans, copy opts failed");

      /* init transaction params */
      pstTransEvt->uiTransID = EME_UTILS_GETTIMEMS;
      pstTransEvt->uiTransState = EME_TRANS_STATUS_PENDING;

      pstTransEvt->uiTransType = uiTransType;

//      iRet = EME_GetIntOptValue(pstTransOpts, EME_OPTID_REPLY_CLSID, (WE_INT32*)&uiClsId);
//      EME_UTILS_BREAKIF(iRet, "EMETM_EnqueueTrans, get cls id failed");

      /*add new transaction to transaction queue*/
      iRet = EME_AddNewTransaction((St_TMHandle*)hTMHandle, pstTransEvt);
      EME_UTILS_BREAKIF(iRet, "EMETM_EnqueueTrans, add transaction failed");

      *puiTransId = pstTransEvt->uiTransID;
      
   } while(0);
   
   if(iRet != EME_SUCCESS)
   {
      /*exception handling*/
      if (pstTransEvt != NULL)
      {
         EME_FreeAllOptValue(pvMemHdr, pstTransEvt->pstTransOpts);
         EME_UTILS_FREEIF(pvMemHdr, pstTransEvt->pstTransOpts);
         EME_UTILS_FREEIF(pvMemHdr, pstTransEvt);
      }
   }

   EME_UTILS_LOG_INFO(("EMETM_EnqueueTrans End-- iRet = %d", iRet));
   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EMETM_DequeueTrans   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. dequeue transaction from transaction queue
 * ===================================================================================================*/
WE_INT32 EMETM_DequeueTrans(ITMHandle hTMHandle, TransID uiTransId)
{
   int iRet = EME_FAILED;
   
     EME_UTILS_LOG_INFO(("EMETM_DequeueTrans  START!!"));
	
   /*check input parameters*/
   if(NULL == hTMHandle)
   {
     EME_UTILS_LOG_INFO(("EMETM_DequeueTrans  BADPARAM!!"));
	
      return EME_FAILED;
   }
   
   do
   {
      //iRet = EME_TransactionFlush(hTMHandle, uiTransId);
      iRet = EME_DequeueTransaction(hTMHandle, uiTransId);
      EME_UTILS_BREAKIF(iRet, "EMETM_DequeueTrans, Dequeue transaction failed!");
      EME_UTILS_LOG_INFO(("EMETM_DequeueTrans  END return = %d!!", iRet));
	  
   } while(0);
   
   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EMETM_DecodeEMN   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. decode emn
 * ===================================================================================================*/
WE_INT32 EMETM_DecodeEMN(ITMHandle hTMHandle, EMEOpt *pOpts, St_EMEEmnRet** ppRet)
{

	return EME_BADPARM;
#if 0
   St_TMHandle *pstTmHandle = (St_TMHandle*)hTMHandle;
   int iRet = EME_FAILED;
   WE_CHAR*    pcBody = NULL;
   WE_INT32    iContentType = 0;
   WE_INT32    iCharsetType = 0;
   WE_UINT32   uiDataLen = 0;

   if (NULL == hTMHandle || NULL == pOpts || NULL == ppRet)
   {
      return EME_FAILED;
   }

   do
   {
      /*get emn body data*/
      iRet = EME_GetStrOptValue(pOpts, EME_OPTID_EMN_BODYDATA,&pcBody);
      EME_UTILS_BREAKIF(iRet,"EMETM_DecodeEMN, get emn body data failed");
      EME_UTILS_BREAKNULL(pcBody, iRet, EME_FAILED, "EMETM_DecodeEMN, get emn body data failed");
      /*get content type*/
      iRet = EME_GetIntOptValue(pOpts, EME_OPTID_EMN_CONTENTTYPE,&iContentType);
      EME_UTILS_BREAKIF(iRet,"EMETM_DecodeEMN, get emn content type failed");
      /*get body data length*/
      iRet = EME_GetIntOptValue(pOpts, EME_OPTID_EMN_BODYDATALEN,(WE_INT32*)&uiDataLen);
      EME_UTILS_BREAKIF(iRet,"EMETM_DecodeEMN, get emn body data len failed");
      /*get charset type*/
      iRet = EME_GetIntOptValue(pOpts, EME_OPTID_EMN_CHARSETTYPE,&iCharsetType);
      EME_UTILS_BREAKIF(iRet,"EMETM_DecodeEMN, get emn charset type failed");

      iRet = EME_EMN_DecodeEMN(pstTmHandle->pvEMN, 
                               pcBody, 
                               iContentType,
                               iCharsetType,
                               uiDataLen,
                               (St_EmnRet**)ppRet);
      EME_UTILS_BREAKIF(iRet, "EMETM_DecodeEMN failed!");
   } while(0);
   
   return iRet;
   
#endif
}



WE_INT32 EMETM_SignalProcess(ITMHandle hTMHandle)
{
   if (hTMHandle == NULL)
   {
      return EME_FAILED;
   }

   return EME_SignalProcess((St_TMHandle*)hTMHandle);
}

WE_INT32 EMETM_HandleTransactions(ITMHandle hTMHandle)
{
   if (hTMHandle == NULL)
   {
      return EME_FAILED;
   }

   return EME_HandleTransactions((St_TMHandle*)hTMHandle);
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EMETM_CheckNeedOpts   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. check need options by different trans type
 * ===================================================================================================*/
static WE_INT32 EMETM_CheckNeedOpts(EMEOpt* pstAccountOpts,
                                    EMEOpt* pstTransOpts,
                                    WE_UINT32 uiTransType)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_CHAR     *pcStrVal = NULL;
   WE_INT32    iIntVal = 0;

   if (pstAccountOpts == NULL || pstTransOpts == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      /* check opts of account infomation */
   	switch(uiTransType)
      {        
      case EME_TRANS_SMTP_SEND_MAIL:
      case EME_TRANS_SMTP_SEND_RECEIPT:
      case EME_TRANS_SMTP_FORWARD_WHOLEMAIL:
      case EME_TRANS_SMTP_FORWARD_ATTACHMENT:        
         pcStrVal = NULL;
         iRet = EME_GetStrOptValue(pstAccountOpts, EME_OPTID_SEND_SERVER_ADDR,&pcStrVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get send server address failed");
         EME_UTILS_BREAKNULL(pcStrVal, iRet, EME_FAILED, "EMETM_CheckNeedOpts, get send server address failed");
         
         if (uiTransType == EME_TRANS_SMTP_SEND_RECEIPT)
         {
            pcStrVal = NULL;
            iRet = EME_GetStrOptValue(pstAccountOpts, EME_OPTID_SEND_EMAIL_ADDR,&pcStrVal);
            EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get send mail address failed");
            EME_UTILS_BREAKNULL(pcStrVal, iRet, EME_FAILED, "EMETM_CheckNeedOpts, get send mail address failed");
         }
         
         pcStrVal = NULL;
         iRet = EME_GetStrOptValue(pstAccountOpts, EME_OPTID_SEND_USERNAME,&pcStrVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get send user name failed");
         EME_UTILS_BREAKNULL(pcStrVal, iRet, EME_FAILED, "EMETM_CheckNeedOpts, get send user name failed");
         
         pcStrVal = NULL;
         iRet = EME_GetStrOptValue(pstAccountOpts, EME_OPTID_SEND_PASSWORD,&pcStrVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get send password failed");
         EME_UTILS_BREAKNULL(pcStrVal, iRet, EME_FAILED, "EMETM_CheckNeedOpts, get send password failed");
         
         iRet = EME_GetIntOptValue(pstAccountOpts, EME_OPTID_SMTP_NEED_SSL,&iIntVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get SMTP need ssl failed");
         
         iRet = EME_GetIntOptValue(pstAccountOpts, EME_OPTID_SIZE_LIMIT,&iIntVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get size limit failed");
         
         iRet = EME_GetIntOptValue(pstAccountOpts, EME_OPTID_SEND_SERVER_PORT,&iIntVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get send server port failed");      
         break;

      case EME_TRANS_POP3_FETCH_NEW_MAILS_HEADER:
      case EME_TRANS_POP3_FETCH_NEW_MAILS:
      case EME_TRANS_POP3_FETCH_SPEC_MAIL:
      case EME_TRANS_POP3_DELETE_SPEC_MAIL:
      case EME_TRANS_POP3_DELETE_ALL_MAIL:
      case EME_TRANS_EPOP_FETCH_NEW_MAILS_HEADER:
      case EME_TRANS_EPOP_FETCH_NEW_MAILS:
      case EME_TRANS_EPOP_FETCH_SPEC_MAIL:
      case EME_TRANS_EPOP_FETCH_SPEC_MAIL_PART:
      case EME_TRANS_EPOP_SET_PUSH_MAIL:
      case EME_TRANS_EPOP_GET_PUSH_MAIL_FILTER:
         pcStrVal = NULL;
         iRet  = EME_GetStrOptValue(pstAccountOpts, EME_OPTID_RECEIVE_SERVER_ADDR,&pcStrVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get recv server address failed");
         EME_UTILS_BREAKNULL(pcStrVal, iRet, EME_FAILED, "EMETM_CheckNeedOpts, get recv server address failed"); 
         
         pcStrVal = NULL;
         iRet  = EME_GetStrOptValue(pstAccountOpts, EME_OPTID_RECEIVE_USERNAME,&pcStrVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get recv user name failed");
         EME_UTILS_BREAKNULL(pcStrVal, iRet, EME_FAILED, "EMETM_CheckNeedOpts, get recv user name failed"); 
         
         pcStrVal = NULL;
         iRet = EME_GetStrOptValue(pstAccountOpts, EME_OPTID_RECEIVE_PASSWORD,&pcStrVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get recv password failed");
         EME_UTILS_BREAKNULL(pcStrVal, iRet, EME_FAILED, "EMETM_CheckNeedOpts, get recv password failed");     
         
         pcStrVal = NULL;
         iRet = EME_GetStrOptValue(pstAccountOpts, EME_OPTID_UIDL_FILE_POP,&pcStrVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get pop uidl file failed");
         EME_UTILS_BREAKNULL(pcStrVal, iRet, EME_FAILED, "EMETM_CheckNeedOpts, get pop uidl file failed");
         
         iRet = EME_GetIntOptValue(pstAccountOpts, EME_OPTID_ACCOUNT_ID, &iIntVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get account id failed");
         
         iRet = EME_GetIntOptValue(pstAccountOpts, EME_OPTID_DEFAULT_FOLDER_ID,&iIntVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get def folder id failed");
         
         iRet = EME_GetIntOptValue(pstAccountOpts, EME_OPTID_POP_KEEP_MAIL,&iIntVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get pop keep mail failed");
         
         iRet = EME_GetIntOptValue(pstAccountOpts, EME_OPTID_POP_NEED_SSL,&iIntVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get SMTP need ssl failed");
         
         iRet = EME_GetIntOptValue(pstAccountOpts, EME_OPTID_SIZE_LIMIT,&iIntVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get size limit failed");
         
         iRet = EME_GetIntOptValue(pstAccountOpts, EME_OPTID_NUM_LIMIT,&iIntVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get num limit failed");
         
         iRet = EME_GetIntOptValue(pstAccountOpts, EME_OPTID_RECEIVE_SERVER_PORT,&iIntVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get recv server port failed");
         break;
         
      case EME_TRANS_IMAP4_FETCH_NEW_MAILS_HEADER:
      case EME_TRANS_IMAP4_FETCH_NEW_MAILS:
      case EME_TRANS_IMAP4_FETCH_SPEC_MAIL:
      case EME_TRANS_IMAP4_FETCH_SPEC_MAIL_PART:
      case EME_TRANS_IMAP4_MARK_DEL_ON_SERVER:
      case EME_TRANS_IMAP4_UNMARK_DEL_ON_SERVER:
      case EME_TRANS_IMAP4_MARK_SEEN_ON_SERVER:
      case EME_TRANS_IMAP4_UNMARK_SEEN_ON_SERVER:
      case EME_TRANS_IMAP4_COPY_MAIL_TO_MAILBOX:
      case EME_TRANS_IMAP4_UPLOAD_MAIL_TO_SERVER:
      case EME_TRANS_IMAP4_EXPUNGE_DEL_MAIL_IN_MAILBOX:
      case EME_TRANS_IMAP4_CREAT_MAILBOX:
      case EME_TRANS_IMAP4_RENAME_MAILBOX:
      case EME_TRANS_IMAP4_DELETE_MAILBOX:
      case EME_TRANS_IMAP4_SUBSCRIBE_MAILBOX:
      case EME_TRANS_IMAP4_UNSUBSCRIBE_MAILBOX:
      case EME_TRANS_IMAP4_LIST_SUBSCRIBED_MAILBOXES:
      case EME_TRANS_IMAP4_LIST_ALL_MAILBOXES:
         pcStrVal = NULL;
         iRet  = EME_GetStrOptValue(pstAccountOpts, EME_OPTID_RECEIVE_SERVER_ADDR,&pcStrVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get recv server address failed");
         EME_UTILS_BREAKNULL(pcStrVal, iRet, EME_FAILED, "EMETM_CheckNeedOpts, get recv server address failed"); 
         
         pcStrVal = NULL;
         iRet  = EME_GetStrOptValue(pstAccountOpts, EME_OPTID_RECEIVE_USERNAME,&pcStrVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get recv user name failed");
         EME_UTILS_BREAKNULL(pcStrVal, iRet, EME_FAILED, "EMETM_CheckNeedOpts, get recv user name failed"); 
         
         pcStrVal = NULL;
         iRet = EME_GetStrOptValue(pstAccountOpts, EME_OPTID_RECEIVE_PASSWORD,&pcStrVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get recv password failed");
         EME_UTILS_BREAKNULL(pcStrVal, iRet, EME_FAILED, "EMETM_CheckNeedOpts, get recv password failed");     
         
         pcStrVal = NULL;
         iRet = EME_GetStrOptValue(pstAccountOpts, EME_OPTID_UID_FILE_IMAP,&pcStrVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get uid file failed");
         EME_UTILS_BREAKNULL(pcStrVal, iRet, EME_FAILED, "EMETM_CheckNeedOpts, get uid file failed");
         
         iRet = EME_GetIntOptValue(pstAccountOpts, EME_OPTID_ACCOUNT_ID, &iIntVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get account id failed");
         
         iRet = EME_GetIntOptValue(pstAccountOpts, EME_OPTID_DEFAULT_FOLDER_ID,&iIntVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get def folder id failed");
         
         iRet = EME_GetIntOptValue(pstAccountOpts, EME_OPTID_IMAP_NEED_SSL,&iIntVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get SMTP need ssl failed");
         
         iRet = EME_GetIntOptValue(pstAccountOpts, EME_OPTID_SIZE_LIMIT,&iIntVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get size limit failed");
         
         iRet = EME_GetIntOptValue(pstAccountOpts, EME_OPTID_NUM_LIMIT,&iIntVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get num limit failed");
         
         iRet = EME_GetIntOptValue(pstAccountOpts, EME_OPTID_RECEIVE_SERVER_PORT,&iIntVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get recv server port failed");
         break;

      default:
         iRet = EME_FAILED;
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get no such transation failed");         
         break;    
      }
      EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, check account infomation failed");
      EME_UTILS_BREAKNULL(pcStrVal, iRet, EME_FAILED, "EMETM_CheckNeedOpts, check account infomation failed");
      

      /* check opts of every transation detail */
      iRet = EME_GetIntOptValue(pstTransOpts, EME_OPTID_TRANS_TYPE,&iIntVal);
      EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get recv server port failed");
      
//      iRet = EME_GetIntOptValue(pstTransOpts, EME_OPTID_REPLY_CLSID,&iIntVal);
//      EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get recv server port failed");

      iRet = EME_GetIntOptValue(pstTransOpts, EME_OPTID_CONN_NETID, (WE_INT32*)&g_email_netid);
      EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get connect id failed");

      if (EME_TRANS_SMTP_SEND_MAIL == uiTransType ||
          EME_TRANS_SMTP_SEND_RECEIPT == uiTransType ||
          EME_TRANS_POP3_FETCH_SPEC_MAIL == uiTransType ||
          EME_TRANS_POP3_DELETE_SPEC_MAIL == uiTransType ||
          EME_TRANS_IMAP4_FETCH_SPEC_MAIL == uiTransType ||
          EME_TRANS_IMAP4_FETCH_SPEC_MAIL_PART == uiTransType ||
          EME_TRANS_IMAP4_MARK_DEL_ON_SERVER == uiTransType ||
          EME_TRANS_IMAP4_UNMARK_DEL_ON_SERVER == uiTransType ||
          EME_TRANS_IMAP4_MARK_SEEN_ON_SERVER == uiTransType ||
          EME_TRANS_IMAP4_UNMARK_SEEN_ON_SERVER == uiTransType ||
          EME_TRANS_IMAP4_COPY_MAIL_TO_MAILBOX == uiTransType ||
          EME_TRANS_IMAP4_UPLOAD_MAIL_TO_SERVER == uiTransType)
      {
         pcStrVal = NULL;
         iRet  = EME_GetStrOptValue(pstTransOpts, EME_OPTID_MESSAGE_ID,&pcStrVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get message id failed");         
         EME_UTILS_BREAKNULL(pcStrVal, iRet, EME_FAILED, "EMETM_CheckNeedOpts, get message id failed");
      }

      if (EME_TRANS_IMAP4_FETCH_SPEC_MAIL_PART == uiTransType)
      {
         pcStrVal = NULL;
         iRet  = EME_GetStrOptValue(pstTransOpts, EME_OPTID_PART_ID,&pcStrVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get part id failed");         
         EME_UTILS_BREAKNULL(pcStrVal, iRet, EME_FAILED, "EMETM_CheckNeedOpts, get part id failed");
      }

      if (EME_TRANS_IMAP4_FETCH_NEW_MAILS_HEADER == uiTransType ||
          EME_TRANS_IMAP4_FETCH_NEW_MAILS == uiTransType ||
          EME_TRANS_IMAP4_RENAME_MAILBOX == uiTransType)
      {
         pcStrVal = NULL;
         iRet  = EME_GetStrOptValue(pstTransOpts, EME_OPTID_LOGIN_MAILBOXNAME,&pcStrVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get login mailbox failed");         
         EME_UTILS_BREAKNULL(pcStrVal, iRet, EME_FAILED, "EMETM_CheckNeedOpts, get login mailbox failed");
      }
         

      if (EME_TRANS_IMAP4_UPLOAD_MAIL_TO_SERVER == uiTransType ||
          EME_TRANS_IMAP4_EXPUNGE_DEL_MAIL_IN_MAILBOX == uiTransType ||
          EME_TRANS_IMAP4_CREAT_MAILBOX == uiTransType ||
          EME_TRANS_IMAP4_RENAME_MAILBOX == uiTransType ||
          EME_TRANS_IMAP4_DELETE_MAILBOX == uiTransType ||
          EME_TRANS_IMAP4_SUBSCRIBE_MAILBOX == uiTransType ||
          EME_TRANS_IMAP4_UNSUBSCRIBE_MAILBOX == uiTransType)
      {
         pcStrVal = NULL;
         iRet  = EME_GetStrOptValue(pstTransOpts, EME_OPTID_HANDLE_MAILBOXNAME,&pcStrVal);
         EME_UTILS_BREAKIF(iRet,"EMETM_CheckNeedOpts, get handle mailbox failed");         
         EME_UTILS_BREAKNULL(pcStrVal, iRet, EME_FAILED, "EMETM_CheckNeedOpts, get login mailbox failed");
      }
   } while(0);

   return iRet;
}


WE_INT32 EMETM_GetTransNumber(ITMHandle hTMHandle,TransID transID, unsigned int *puiNum)
{
   St_TMHandle         *pMe = (St_TMHandle*)hTMHandle;
   St_TransEvt         *pstTransTmp = NULL;
   unsigned int         uiTransNum = 0;
   int                 iRet = EME_SUCCESS;
   void                *pvMemHdr = NULL;

   /*check input parameters*/
   if(NULL == hTMHandle || NULL == puiNum)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(hTMHandle);

   do
   {
      list_for_each_entry(St_TransEvt, pstTransTmp, &pMe->stTransQueueFirst,stTransList)
      {
         if(EME_ALL_TRANS == transID || EME_PROGRESS_INDI_MASK == transID)
         {
            uiTransNum++;
         }
         else if(transID == pstTransTmp->uiTransID)
         {
            uiTransNum++;
         }
      }

      if(transID == EME_PROGRESS_INDI_MASK)
         break;

      list_for_each_entry(St_TransEvt, pstTransTmp, &pMe->stTransPendingQueueFirst,stTransList)
      {
         if(EME_ALL_TRANS == transID)
         {
            uiTransNum++;
         }
         else
         {
            if (transID == pstTransTmp->uiTransID) 
            {
               uiTransNum++;
            }
         }
      }
   }while(0);

   *puiNum = uiTransNum;

   return iRet;
}
