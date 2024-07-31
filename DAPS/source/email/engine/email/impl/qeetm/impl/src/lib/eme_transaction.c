/*=====================================================================================
FILE NAME   : eme_transaction.c
MODULE NAME : Email engine

GENERAL DESCRIPTION
Provide some function for transaction.

TechFaith Software Confidential Proprietary
Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
Revision History

Modification                   Tracking
Date              Author       Number       Description of changes
----------   --------------   ---------   --------------------------------------
2007-03-02     Jiang Dingqing                  create eme_transaction.c

=====================================================================================*/
//pclint
/*lint -e760 */

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
#include "eme_sig_api.h"
#include "eme_tm_def.h"
#include "eme_tm_utils_api.h"
#include "oem_tmlib_api.h"
#include "eme_transaction_api.h"
#include "oem_asyncproc_api.h"
#include "eme_dataaccess_priv.h"
// public files for all libs

/*Include Private head file*/
#include "email_store_api.h"
extern WE_INT32 EME_UpdateOpt(EmailOpt *pstAllOpts, WE_INT32 iId, WE_VOID* pvVal, WE_VOID** ppvOrigVal);

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


/*=============================================================================
*
*   Global Variable Declare Section
*
=============================================================================*/
/*signal free list max length*/
#define EME_MAX_TRANSACTION_FREE_LIST_LENGTH  0
#define EME_MAX_TRANSACTION_IN_PROGRESS       1

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
static WE_INT32 EME_InProgressTransactionFlush(St_TMHandle *pstTMHandle, TransID uiTransID);
static WE_INT32 EME_PendingTransactionFlush(St_TMHandle *pstTMHandle, TransID uiTransID);
static WE_INT32 EME_TransactionCancelReply(St_TMHandle *pstTMHandle,
                                           WE_UINT32 uiTransID,
                                           WE_UINT32 uiTransType,
                                           WE_UINT32 uiMsgId,
                                           WE_UINT32 uiClsId);

/*============================================================================ 
*    FUNCTION        EME_TransactionInit()
* 
*    DESCRIPTION     Get a signal from signal queue and process it,
If signal free list is not full, this signal will add to it;
* 
*    DEPENDENCIES    None
* 
*    PARAMETERS      None
* 
*    RETURN VALUE    WE_INT32:   EME_SUCCESS if process signal successful
*                              EME_FAILED if process signal failed   
*     
*    SIDE EFFECTS    None   
* 
*===========================================================================*/
WE_INT32 EME_TransactionInit(St_TMHandle *pstTMHandle)
{
   St_TMHandle* pMe = pstTMHandle;

   if(NULL == pMe)
   {
      return EME_FAILED;
   }

   do 
   {
      /************************************************************************/
      /* init pending queue,free list and trans queue                         */                                            
      /************************************************************************/
      //pMe->stTransQueueFirst = pMe->m_trans_queue_last = NULL;
      EME_INIT_LIST_HEAD(&pMe->stTransQueueFirst);
      EME_INIT_LIST_HEAD(&pMe->stTransPendingQueueFirst);
      EME_INIT_LIST_HEAD(&pMe->stTransFreeList);
      pMe->iTransQueueLength = 0;
      pMe->iTransFreeListLength = 0;

   } while(0);

   return EME_SUCCESS;
}


/*============================================================================ 
*    FUNCTION        EME_TransactionTerminate()
* 
*    DESCRIPTION     Get a signal from signal queue and process it,
If signal free list is not full, this signal will add to it;
* 
*    DEPENDENCIES    None
* 
*    PARAMETERS      None
* 
*    RETURN VALUE    WE_INT32:   EME_SUCCESS if process signal successful
*                              EME_FAILED if process signal failed   
*     
*    SIDE EFFECTS    None   
* 
*===========================================================================*/
WE_INT32 EME_TransactionTerminate(St_TMHandle *pstTMHandle)
{
   St_TMHandle    *pMe = pstTMHandle;
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iResult = EME_SUCCESS;
   void           *pvMemHdr = NULL;

   if(NULL == pMe)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pMe);
   do 
   {
      iResult = EME_TransactionFlush(pMe, EME_ALL_TRANS);
      EME_UTILS_BREAKIF(iResult, "EME_TransactionTerminate, flush trans queue failed");

      /************************************************************************/
      /*delete all in progress transactions in trans queue ,                  */                                                  
      /************************************************************************/
      while(!EME_list_empty(&pMe->stTransQueueFirst))
      {
         pstTransEvt = list_entry(pMe->stTransQueueFirst.next, St_TransEvt, stTransList);
         EME_list_del(&pstTransEvt->stTransList);
         EME_UTILS_FREEIF(pvMemHdr, pstTransEvt);
      }

      /************************************************************************/
      /*  delete all pending transactions in pending queue,                   */                                                
      /************************************************************************/
      while(!EME_list_empty(&pMe->stTransPendingQueueFirst))
      {
         pstTransEvt = list_entry(pMe->stTransPendingQueueFirst.next, St_TransEvt, stTransList);
         EME_list_del(&pstTransEvt->stTransList);
         EME_UTILS_FREEIF(pvMemHdr, pstTransEvt);
      }

      /************************************************************************/
      /* delete all transaction nodes in free list                            */                                         
      /************************************************************************/
      while(!EME_list_empty(&pMe->stTransFreeList))
      {
         pstTransEvt = list_entry(pMe->stTransFreeList.next, St_TransEvt, stTransList);
         EME_list_del(&pstTransEvt->stTransList);
         EME_UTILS_FREEIF(pvMemHdr, pstTransEvt);
      }

   } while(0);

   return iResult;
}


/*============================================================================ 
*    FUNCTION        EME_FindPendingTrans()
* 
*    DESCRIPTION     Get a signal from signal queue and process it,
If signal free list is not full, this signal will add to it;
* 
*    DEPENDENCIES    None
* 
*    PARAMETERS      None
* 
*    RETURN VALUE    WE_INT32:   EME_SUCCESS if process signal successful
*                              EME_FAILED if process signal failed   
*     
*    SIDE EFFECTS    None   
* 
*===========================================================================*/
WE_INT32 EME_FindPendingTrans(St_TMHandle *pstTMHandle)
{
   WE_INT32          iResult = EME_SUCCESS;
   St_TMHandle     *pMe = pstTMHandle;

   if(NULL == pMe)
   {
      return EME_FAILED;
   }

   if(!EME_list_empty(&pMe->stTransPendingQueueFirst))
   {
      iResult = EME_SUCCESS;
   }
   else
   {
      iResult = EME_FAILED;
   }

   return iResult;
}


St_TransEvt* EME_NextTransEvt(St_TMHandle *pstTMHandle)
{
   St_TransEvt *pstTransEvt = NULL;

   if (pstTMHandle == NULL)
   {
      return NULL;
   }

   if(!EME_list_empty(&pstTMHandle->stTransPendingQueueFirst))
   {
      pstTransEvt = list_entry(pstTMHandle->stTransPendingQueueFirst.next, St_TransEvt, stTransList);
   }

   return pstTransEvt;
}
/*============================================================================ 
*    FUNCTION        EME_HandleTransactions()
* 
*    DESCRIPTION     Get a signal from signal queue and process it,
If signal free list is not full, this signal will add to it;
* 
*    DEPENDENCIES    None
* 
*    PARAMETERS      None
* 
*    RETURN VALUE    WE_INT32:   EME_SUCCESS if process signal successful
*                              EME_FAILED if process signal failed   
*     
*    SIDE EFFECTS    None   
* 
*===========================================================================*/
WE_INT32 EME_HandleTransactions(St_TMHandle *pstTMHandle)
{
   WE_INT32       iResult = EME_SUCCESS;
   St_TMHandle    *pMe = pstTMHandle;
   St_TransEvt    *pstTransEvt = NULL;
   WE_VOID        *pvMemHdr = NULL;

   if( NULL == pMe )
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pMe);
   do 
   {
      /************************************************************************/
      /* transaction in progress have a limit ,don't                          */                                       
      /************************************************************************/
      if(pMe->iTransQueueLength < EME_MAX_TRANSACTION_IN_PROGRESS)
      {   
         /************************************************************************/
         /* handling pending transactions,  move it to trans in progress queue,  */
         /*                                                                      */
         /************************************************************************/
         if(!EME_list_empty(&pMe->stTransPendingQueueFirst))
         {
            pstTransEvt = list_entry(pMe->stTransPendingQueueFirst.next, St_TransEvt, stTransList);
            EME_UTILS_BREAKNULL(pstTransEvt, iResult, EME_FAILED, "EME_HandleTransactions, transEvt is null");

            EME_list_move(&pstTransEvt->stTransList,&pMe->stTransQueueFirst);

            //increase trans queue length
            (pMe->iTransQueueLength)++; 
            EME_UTILS_LOG_INFO(("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"));
            EME_UTILS_LOG_INFO(("@@@@@@@@@@@@@@@@@@@@@@@@@trans queue in progress length = "));
            EME_UTILS_LOG_INFO(("@@@@@@@@@@@@@@@@@@@@@@@@@%d,", pMe->iTransQueueLength));
            EME_UTILS_LOG_INFO(("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"));

            /************************************************************************/
            /* handle transactions in pending queue  ,dispatch to proper FSM in core ,*/
            /* depending on the transaction result, we decide which queue the trans to put.*/
            /************************************************************************/
            //successfully retrieve user data
            switch(pstTransEvt->uiTransType)
            {
            case EME_TRANS_SMTP_SEND_MAIL:
            case EME_TRANS_SMTP_SEND_RECEIPT:
            case EME_TRANS_SMTP_FORWARD_WHOLEMAIL:
            case EME_TRANS_SMTP_FORWARD_ATTACHMENT:
               EME_EVT_SENDTO_SELF_P(pstTMHandle,
                  EME_FSM_SMTP, 
                  EME_SIG_SMTP_TRANS_START,
                  (void*)pstTransEvt);
               break;

            case EME_TRANS_POP3_FETCH_NEW_MAILS_HEADER:
            case EME_TRANS_POP3_FETCH_NEW_MAILS:
            case EME_TRANS_POP3_FETCH_SPEC_MAIL:
//             case EME_TRANS_POP3_FETCH_SPEC_MAIL_HEADER:
            case EME_TRANS_POP3_DELETE_SPEC_MAIL:
            case EME_TRANS_POP3_DELETE_ALL_MAIL:
            case EME_TRANS_EPOP_FETCH_NEW_MAILS_HEADER:
            case EME_TRANS_EPOP_FETCH_NEW_MAILS:
            case EME_TRANS_EPOP_FETCH_SPEC_MAIL:
            case EME_TRANS_EPOP_FETCH_SPEC_MAIL_PART:
            case EME_TRANS_EPOP_SET_PUSH_MAIL:
            case EME_TRANS_EPOP_GET_PUSH_MAIL_FILTER:
               EME_EVT_SENDTO_SELF_P(pstTMHandle,
                  EME_FSM_POP, 
                  EME_SIG_POP_TRANS_START,
                  (void*)pstTransEvt);
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
               EME_EVT_SENDTO_SELF_P(pstTMHandle,
                  EME_FSM_IMAP, 
                  EME_SIG_IMAP_TRANS_START,
                  (void*)pstTransEvt);
               break;

            default:
               EME_UTILS_LOG_INFO(("EME_HandleTransactions, invalid trans type"));
               EME_list_del(&pstTransEvt->stTransList);
               EME_FreeTransEvt(pvMemHdr, pstTransEvt);
               EME_UTILS_FREEIF(pvMemHdr, pstTransEvt);
               iResult = EME_FAILED;
               break;
            }
         }
      }
   } while(0);

   if(EME_SUCCESS != iResult)
   {

   }

   /************************************************************************/
   /* avoid trigger many times                                             */                    
   /************************************************************************/
   //OEMTMLib_EmailTriggerDispatcher(pstTMHandle);

   return iResult;
}

/*============================================================================ 
*    FUNCTION        EME_SignalProcess()
* 
*    DESCRIPTION     Get a signal from signal queue and process it,
If signal free list is not full, this signal will add to it;
* 
*    DEPENDENCIES    None
* 
*    PARAMETERS      None
* 
*    RETURN VALUE    WE_INT32:   EME_SUCCESS if process signal successful
*                              EME_FAILED if process signal failed   
*     
*    SIDE EFFECTS    None   
* 
*===========================================================================*/
WE_INT32 EME_DeleteTransaction(St_TMHandle *pstTMHandle, St_TransEvt* pstTransData)
{
   WE_INT32       iResult = EME_SUCCESS;
   St_TMHandle    *pMe = pstTMHandle;
   St_TransEvt    *pstTemp = NULL;// the trans to seek which is identical to pstTransData 
   void           *pvMemHdr = NULL;
   WE_VOID     *pvStore = NULL;

   EME_UTILS_LOG_INFO(("EME_DequeueTransaction  EME_DeleteTransaction START!!"));

   if(NULL == pMe ||
      NULL == pstTransData)
   {
      EME_UTILS_LOG_INFO(("EME_DequeueTransaction  EME_DeleteTransaction BADPARM!!"));

      return EME_FAILED;
   }
   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTMHandle);

   do 
   {
      /************************************************************************/
      /* remove the transaction from in progress queue                        */                                              
      /************************************************************************/
      EME_UTILS_LOG_INFO(("EME_DeleteTransaction stTransQueueFirst  is %p", pMe->stTransQueueFirst));
      EME_UTILS_LOG_INFO(("EME_DeleteTransaction stTransPendingQueueFirst  is %p", pMe->stTransPendingQueueFirst));
      list_for_each_entry(St_TransEvt, pstTemp, &pMe->stTransQueueFirst, stTransList)
      {
         EME_UTILS_LOG_INFO(("EME_DeleteTransaction  have trans in progress queue"));
         EME_UTILS_LOG_INFO(("EME_DeleteTransaction stTransQueueFirst  is %p", pMe->stTransQueueFirst));
         EME_UTILS_LOG_INFO(("EME_DeleteTransaction stTransPendingQueueFirst  is %p", pMe->stTransPendingQueueFirst));

         if(pstTemp->uiTransID == pstTransData->uiTransID && pstTemp == pstTransData)
         {
            /************************************************************************/
            /* handle transaction canceled or failed ,move it to the end of pending queue,  */
            /* and change its state to trans pending                                */
            /************************************************************************/
            if(EME_TRANS_STATUS_CANCELED == pstTransData->uiTransState)
            {
               pstTransData->uiTransState = EME_TRANS_STATUS_PENDING;
               EME_UTILS_LOG_INFO(("EME_DeleteTransaction  pMe->stTransPendingQueueFirst1 is %p",pMe->stTransPendingQueueFirst));
               //empty pending queue  
               EME_list_move(&pstTransData->stTransList, &pMe->stTransPendingQueueFirst);
               EME_UTILS_LOG_INFO(("EME_DeleteTransaction  pMe->stTransPendingQueueFirst2 is %p",pMe->stTransPendingQueueFirst));
            }
            /************************************************************************/
            /*handle transaction in  state other than canceled                     */
            /*add it to the start of the free trans list queue, and change its state to */
            /*E_TM_TRANSSTATETYPE_MAX                                               */
            /************************************************************************/
            else 
            {
               pstTransData->uiTransState = EME_TRANS_STATUS_MAX;
               EME_UTILS_LOG_INFO(("EME_DeleteTransaction  iTransFreeListLength is %d", pMe->iTransFreeListLength));
               // free node exists 
               if(pMe->iTransFreeListLength < EME_MAX_TRANSACTION_FREE_LIST_LENGTH)
               {
                  EME_UTILS_LOG_INFO(("EME_DeleteTransaction  iTransFreeListLength1"));
                  EME_list_move(&pstTransData->stTransList, &pMe->stTransFreeList);
                  ++(pMe->iTransFreeListLength);
                  EME_UTILS_LOG_INFO(("EME_DeleteTransaction stTransQueueFirst1  is %p", pMe->stTransQueueFirst));
                  EME_UTILS_LOG_INFO(("EME_DeleteTransaction stTransPendingQueueFirst1  is %p", pMe->stTransPendingQueueFirst));
               }
               else
               {
                  EME_UTILS_LOG_INFO(("EME_DeleteTransaction  pstTransData->uiTransType is %x", pstTransData->uiTransType));
                  EME_list_del(&pstTransData->stTransList);
                  EME_UTILS_LOG_INFO(("EME_DeleteTransaction stTransQueueFirst2  is %p", pMe->stTransQueueFirst));
                  EME_UTILS_LOG_INFO(("EME_DeleteTransaction stTransPendingQueueFirst2  is %p", pMe->stTransPendingQueueFirst));
                  // no free node, delete the pending node directly 
                  //clear memories in transaction node
                  //FREE p memory in signal
                  if(NULL != pstTransData->pfnFreeSigData)
                  {
                     (pstTransData->pfnFreeSigData)(pstTMHandle, pstTransData);
                  }
                  EME_UTILS_LOG_INFO(("EME_DeleteTransaction stTransQueueFirst3  is %p", pMe->stTransQueueFirst));
                  EME_UTILS_LOG_INFO(("EME_DeleteTransaction stTransPendingQueueFirst3  is %p", pMe->stTransPendingQueueFirst));

#if 1
                do {
                    if(EME_TRANS_EPOP_FETCH_SPEC_MAIL_PART ==pstTransData->uiTransType)
                    {
                        WE_INT32    iRet = EME_SUCCESS;
                        WE_VOID     *pvFileMgr = NULL;
                        WE_UINT32   uiMsgId = 0;
                        WE_CHAR     *pcMsgId = NULL;
                        EmailOpt    *pstMsgPartOpts = NULL;
                        WE_CHAR		*pcPartID = NULL;
                        WE_INT32 iAttachStatus = 0;
                        EME_UTILS_LOG_INFO(("EME_DeleteTransaction stTransQueueFirst4  is %p", pMe->stTransQueueFirst));
                        EME_UTILS_LOG_INFO(("EME_DeleteTransaction stTransPendingQueueFirst4  is %p", pMe->stTransPendingQueueFirst));

                        /* get file mgr */
                        pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTMHandle);
                        EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_DequeueTransaction EME_FSMEPOP_FetchSpecPart,get file mgr failed");      

                        /* get message id */
                        iRet = EME_GetStrOptValue(pstTransData->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
                        EME_UTILS_BREAKIF(iRet, "EME_DequeueTransaction EME_FSMEPOP_FetchSpecPart, get message uidl failed");
                        uiMsgId = EME_UTILS_ATOX(pcMsgId);

                        iRet = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
                        EME_UTILS_BREAKIF(iRet, "EME_DequeueTransaction EME_FSMEPOP_FetchSpecPart, init store handle failed");
                        EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_DequeueTransactionEME_FSMEPOP_FetchSpecPart, init store handle failed");

                        iRet = EME_GetStrOptValue(pstTransData->pstTransOpts, EME_OPTID_PART_ID, &pcPartID);
                        EME_UTILS_BREAKIF(iRet, "EME_DequeueTransaction EME_FSMEPOP_FetchSpecPart, get part id failed");
                        EME_UTILS_BREAKNULL(pcPartID, iRet, EME_FAILED, "EME_DequeueTransaction EME_FSMEPOP_FetchSpecPart, get Content id failed");

                        iRet = EmailStore_GetMsgPart(pvStore, uiMsgId, EME_UTILS_ATOI(pcPartID), &pstMsgPartOpts);
                        EME_UTILS_BREAKIF(iRet, "EME_DequeueTransaction EME_FSMEPOP_FetchSpecPart, get message part failed");
                        EME_UTILS_BREAKNULL(pstMsgPartOpts, iRet, EME_FAILED, "EME_DequeueTransaction EME_FSMEPOP_FetchSpecPart, get message part failed");

                        EME_GetIntOptValue((EMEOpt*)pstMsgPartOpts, EMAILPART_OPT_X_ATTACHMENT, &iAttachStatus);

                        if(2==iAttachStatus)    //downloading
                        {
                            // 0: not downloading, ATTACHMENT_STATUS_NOT_DOWN
                            EME_UpdateOpt(pstMsgPartOpts, EMAILPART_OPT_X_ATTACHMENT, (WE_VOID*)0, NULL);
                            EmailStore_UpdateMsgPart(pvStore, uiMsgId, EME_UTILS_ATOI(pcPartID), pstMsgPartOpts);
                        }
                    }
                }while(0);

                EmailStore_DeleteHandle(pvStore);
#endif
                EME_UTILS_LOG_INFO(("EME_DeleteTransaction stTransQueueFirst5  is %p", pMe->stTransQueueFirst));
                EME_UTILS_LOG_INFO(("EME_DeleteTransaction stTransPendingQueueFirst5  is %p", pMe->stTransPendingQueueFirst));
                  EME_FreeTransEvt(pvMemHdr, pstTransData);
                  EME_UTILS_FREEIF(pvMemHdr, pstTransData);
               }
            }
            //find the proper transaction,decrease trans queue length
            pMe->iTransQueueLength--;

            break;
         }
      }

      /************************************************************************/
      /* when a transaction finished,we should clear the signals of this transaction */
      /* or will lead to memory leak when the applet invoke enqueue trans function exits. */
      /************************************************************************/
      EME_UTILS_LOG_INFO(("EME_DequeueTransaction  EME_SignalTerminate START"));
      EME_UTILS_LOG_INFO(("EME_DeleteTransaction stTransQueueFirst6  is %p", pMe->stTransQueueFirst));
      EME_UTILS_LOG_INFO(("EME_DeleteTransaction stTransPendingQueueFirst6  is %p", pMe->stTransPendingQueueFirst));

      OEM_AsyncProc_Reset(pMe->pvAsyncMgr);
      OEMTMLib_SetEmlTMState(pMe, EME_TM_STATE_FREE);
      EME_SignalTerminate(pMe);   

      EME_UTILS_LOG_INFO(("EME_DequeueTransaction  EME_SignalTerminate END"));
   } while(0);

   /************************************************************************/
   /*when a transaction in progress is finished, we need to trigger again  */                                                               
   /************************************************************************/

   EME_UTILS_LOG_INFO(("EME_DequeueTransaction  OEMTMLib_EmailTriggerDispatcher START"));
   OEMTMLib_EmailTriggerDispatcher(pMe);

   EME_UTILS_LOG_INFO(("EME_DequeueTransaction  OEMTMLib_EmailTriggerDispatcher END"));
   if(EME_SUCCESS != iResult)//some error occurs, flush the transaction queue 
   {

   }

   return iResult;
}

/*============================================================================ 
*    FUNCTION        EME_SignalProcess()
* 
*    DESCRIPTION     Get a signal from signal queue and process it,
If signal free list is not full, this signal will add to it;
* 
*    DEPENDENCIES    None
* 
*    PARAMETERS      None
* 
*    RETURN VALUE    WE_INT32:   EME_SUCCESS if process signal successful
*                              EME_FAILED if process signal failed   
*     
*    SIDE EFFECTS    None   
* 
*===========================================================================*/

WE_INT32 EME_TransactionFlush(St_TMHandle *pstTMHandle, TransID uiTransID)
{
   WE_INT32 iResult = EME_SUCCESS;

   if(NULL == pstTMHandle)
   {
      return EME_FAILED;
   }

   EME_UTILS_LOG_INFO(("EME_TransactionFlush  START"));

   do 
   {
      iResult = EME_PendingTransactionFlush(pstTMHandle, uiTransID);
      iResult += EME_InProgressTransactionFlush(pstTMHandle, uiTransID);
   } while(0);

   EME_UTILS_LOG_INFO(("EME_TransactionFlush  END, the result is %d", iResult));

   return iResult;
}

/*============================================================================ 
*    FUNCTION        EME_SignalProcess()
* 
*    DESCRIPTION     Get a signal from signal queue and process it,
If signal free list is not full, this signal will add to it;
* 
*    DEPENDENCIES    None
* 
*    PARAMETERS      None
* 
*    RETURN VALUE    WE_INT32:   EME_SUCCESS if process signal successful
*                              EME_FAILED if process signal failed   
*     
*    SIDE EFFECTS    None   
* 
*===========================================================================*/
static WE_INT32 EME_InProgressTransactionFlush(St_TMHandle *pstTMHandle, TransID uiTransID)
{
   WE_INT32    iResult = EME_SUCCESS;
   St_TransEvt *pstEvt, *tmp = NULL;// current trans node 
   WE_VOID     *pvMemHdr = NULL;

   if(NULL == pstTMHandle)
   {
      return EME_FAILED;
   }

   EME_UTILS_LOG_INFO(("EME_InProgressTransactionFlush  START"));
   do 
   {
      pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTMHandle);

      /************************************************************************/
      /* If value of transID equals QMETM_ALL_TRANS, then all the             */
      /* transactions associated with given profile ID are canceled.          */                                                        
      /************************************************************************/
      list_for_each_entry_safe(St_TransEvt, pstEvt, tmp, &pstTMHandle->stTransQueueFirst, stTransList)
      {
         if(pstEvt != NULL &&
            (pstEvt->uiTransID == uiTransID ||
            uiTransID == EME_ALL_TRANS))
         {
            iResult = EME_DeleteTransaction(pstTMHandle, pstEvt);
            if(SUCCESS == iResult)
            {
               EME_UTILS_LOG_INFO(("EME_DeleteTransaction  ERROR, the result is %d",  iResult));	
            }

            //iResult = EME_TransactionCancelReply(pstTMHandle, pstEvt);
         }
      }
      /************************************************************************/
      /* cancel all transactions in progress                                  */                               
      /************************************************************************/

   } while(0);

   return iResult;
}


/*============================================================================ 
*    FUNCTION        EME_PendingTransactionFlush()
* 
*    DESCRIPTION     Get a signal from signal queue and process it,
If signal free list is not full, this signal will add to it;
* 
*    DEPENDENCIES    None
* 
*    PARAMETERS      None
* 
*    RETURN VALUE    WE_INT32:   EME_SUCCESS if process signal successful
*                              EME_FAILED if process signal failed   
*     
*    SIDE EFFECTS    None   
* 
*===========================================================================*/
static WE_INT32 EME_PendingTransactionFlush(St_TMHandle *pstTMHandle, TransID uiTransID)
{
   WE_INT32    iResult = EME_SUCCESS;
   St_TransEvt * pstEvt, *tmp = NULL;// current trans node 
   void        * pvMemHdr = NULL;   

   if(NULL == pstTMHandle)
   {
      return EME_FAILED;
   }

   EME_UTILS_LOG_INFO(("EME_PendingTransactionFlush  START"));

   do 
   {
      /************************************************************************/
      /* If value of transID equals QMETM_ALL_TRANS, then all the             */
      /* transactions associated with given profile ID are canceled.          */                                                       
      /************************************************************************/

      pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTMHandle);

      list_for_each_entry_safe(St_TransEvt, pstEvt, tmp, &pstTMHandle->stTransPendingQueueFirst, stTransList)
      {
         if(pstEvt != NULL && 
            (pstEvt->uiTransID == uiTransID ||
            uiTransID == EME_ALL_TRANS))
         {
            EME_list_del(&pstEvt->stTransList);
            EME_FreeTransEvt(pvMemHdr, pstEvt);
            EME_UTILS_FREEIF(pvMemHdr, pstEvt);
         }
      }

   } while(0);

   EME_UTILS_LOG_INFO(("EME_PendingTransactionFlush  END"));	
   return iResult;
}
/*============================================================================ 
*    FUNCTION        EME_DequeueTransaction()
* 
*    DESCRIPTION     Get a signal from signal queue and process it,
If signal free list is not full, this signal will add to it;
* 
*    DEPENDENCIES    None
* 
*    PARAMETERS      None
* 
*    RETURN VALUE    WE_INT32:   EME_SUCCESS if process signal successful
*                              EME_FAILED if process signal failed   
*     
*    SIDE EFFECTS    None   
* 
*===========================================================================*/
WE_INT32 EME_DequeueTransaction(St_TMHandle *pstHandle, TransID uiTransID)
{
   WE_INT32    iResult = EME_SUCCESS;
   St_TransEvt *pstEvt, *tmp = NULL;// current trans node 
   WE_VOID     *pvMemHdr = NULL;
   WE_UINT32     uiTransType = 0;
   WE_UINT32     uiClsId = 0;
   WE_CHAR     *pcMsgId = NULL;
   WE_UINT32   uiMsgId = 0;

   EME_UTILS_LOG_INFO(("EME_DequeueTransaction START!!"));
   if(NULL == pstHandle)
   {
      EME_UTILS_LOG_INFO(("EME_DequeueTransaction BADPARAM!!"));
      return EME_FAILED;
   }

   do 
   {
      pvMemHdr = EME_OEMTMLib_GetMemHdr(pstHandle);
      /************************************************************************/
      /* If value of transID equals EMETM_ALL_TRANS, then all the             */
      /* transactions associated with given profile ID are canceled.          */                                                        
      /************************************************************************/

      /************************************************************************/
      /* cancel all transactions in progress                                  */                               
      /************************************************************************/
      EME_UTILS_LOG_INFO(("EME_DequeueTransaction   pstHandle is %p,TransID is %d", pstHandle, uiTransID));
      EME_UTILS_LOG_INFO(("EME_DequeueTransaction stTransQueueFirst  is %p", pstHandle->stTransQueueFirst));
      EME_UTILS_LOG_INFO(("EME_DequeueTransaction stTransPendingQueueFirst  is %p", pstHandle->stTransPendingQueueFirst));
      list_for_each_entry_safe(St_TransEvt, pstEvt, tmp, &pstHandle->stTransQueueFirst, stTransList)
      {
         EME_UTILS_LOG_INFO(("EME_DequeueTransaction   cancel all transactions in progress!!"));
         EME_UTILS_LOG_INFO(("EME_DequeueTransaction stTransQueueFirst1  is %p", pstHandle->stTransQueueFirst));
         EME_UTILS_LOG_INFO(("EME_DequeueTransaction stTransPendingQueueFirst1  is %p", pstHandle->stTransPendingQueueFirst));
         EME_UTILS_LOG_INFO(("EME_DequeueTransaction stTransQueueFirst  pstEvt is %p", pstEvt));
         if(pstEvt != NULL && 
            /*pstHandle->iCanBeCancel == EME_CANCEL_ENABLED &&*/
            (pstEvt->uiTransID == uiTransID ||
            uiTransID == EME_ALL_TRANS ||
            uiTransID == EME_PROGRESS_INDI_MASK))
         {
            EME_UTILS_LOG_INFO(("EME_DequeueTransaction  pstEvt->uiTransID = %d!!", pstEvt->uiTransID));

            uiTransID = pstEvt->uiTransID;
            uiTransType = pstEvt->uiTransType;

//            iResult = EME_GetIntOptValue(pstEvt->pstTransOpts, EME_OPTID_REPLY_CLSID, (WE_INT32*)&uiClsId);
//            EME_UTILS_BREAKIF(iResult,"EME_DequeueTransaction, get clsid failed");

            EME_UTILS_LOG_INFO(("EME_DequeueTransaction  EME_GetIntOptValue iret = %d!!", iResult));

            EME_StackClose(pstHandle, uiTransType);

            EME_UTILS_LOG_INFO(("EME_DequeueTransaction  EME_StackClose END!!"));

            iResult = EME_GetStrOptValue(pstEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
            if(iResult == EME_SUCCESS)
            {
               uiMsgId = EME_UTILS_ATOX(pcMsgId);
            }

            EME_UTILS_LOG_INFO(("EME_DequeueTransaction stTransPendingQueueFirst10 is %p", pstHandle->stTransPendingQueueFirst));
            EME_UTILS_LOG_INFO(("EME_DequeueTransaction stTransQueueFirst10 is %p", pstHandle->stTransQueueFirst));
            iResult = EME_DeleteTransaction(pstHandle, pstEvt);
            EME_UTILS_LOG_INFO(("EME_DequeueTransaction stTransPendingQueueFirst11  is %p", pstHandle->stTransPendingQueueFirst));
            EME_UTILS_LOG_INFO(("EME_DequeueTransaction stTransQueueFirst11  is %p", pstHandle->stTransQueueFirst));

            EME_UTILS_LOG_INFO(("EME_DequeueTransaction  EME_DeleteTransaction iret = %d!!", iResult));

            /* cancel reply */
            if (iResult == EME_SUCCESS)
            {
               EME_TransactionCancelReply(pstHandle, uiTransID, uiTransType, uiMsgId, uiClsId);
            }
            EME_UTILS_LOG_INFO(("EME_DequeueTransaction stTransPendingQueueFirst12  is %p", pstHandle->stTransPendingQueueFirst));
            EME_UTILS_LOG_INFO(("EME_DequeueTransaction stTransQueueFirst12  is %p", pstHandle->stTransQueueFirst));

         }
      }
      /************************************************************************/
      /* cancel all transactions in pending                                  */                               
      /************************************************************************/
      EME_UTILS_LOG_INFO(("EME_DequeueTransaction  pendding pstHandle is %p,TransID is %d", pstHandle, uiTransID));
      EME_UTILS_LOG_INFO(("EME_DequeueTransaction stTransQueueFirst2  is %p", pstHandle->stTransQueueFirst));
      EME_UTILS_LOG_INFO(("EME_DequeueTransaction stTransPendingQueueFirst2  is %p", pstHandle->stTransPendingQueueFirst));
      list_for_each_entry_safe(St_TransEvt, pstEvt, tmp, &pstHandle->stTransPendingQueueFirst, stTransList)
      {
          EME_UTILS_LOG_INFO(("EME_DequeueTransaction  have trans in progress stTransPendingQueueFirst"));
          EME_UTILS_LOG_INFO(("EME_DequeueTransaction stTransPendingQueueFirst  pstEvt is %p", pstEvt));
          EME_UTILS_LOG_INFO(("EME_DequeueTransaction stTransQueueFirst21  is %p", pstHandle->stTransQueueFirst));
          EME_UTILS_LOG_INFO(("EME_DequeueTransaction stTransPendingQueueFirst21  is %p", pstHandle->stTransPendingQueueFirst));
         if(pstEvt != NULL && 
            (pstEvt->uiTransID == uiTransID ||
            uiTransID == EME_ALL_TRANS))
         {

            uiTransID = pstEvt->uiTransID;
            uiTransType = pstEvt->uiTransType;

//            iResult = EME_GetIntOptValue(pstEvt->pstTransOpts, EME_OPTID_REPLY_CLSID, (WE_INT32*)&uiClsId);
//            EME_UTILS_BREAKIF(iResult,"EME_DequeueTransaction, get clsid failed");

            iResult = EME_GetStrOptValue(pstEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
            if(iResult == EME_SUCCESS)
            {
               uiMsgId = EME_UTILS_ATOX(pcMsgId);
            }
            iResult = EME_SUCCESS;

            //EME_StackClose(pstHandle, uiTransType);

            EME_list_del(&pstEvt->stTransList);
            EME_FreeTransEvt(pvMemHdr, pstEvt);
            EME_UTILS_FREEIF(pvMemHdr, pstEvt);

            /* cancel reply */
            if (iResult == EME_SUCCESS)
            {
               EME_TransactionCancelReply(pstHandle, uiTransID, uiTransType, uiMsgId, uiClsId);
            }
         }

      }      
   } while(0);

   return iResult;
}

/*============================================================================ 
*    FUNCTION        EME_SignalProcess()
* 
*    DESCRIPTION     Get a signal from signal queue and process it,
If signal free list is not full, this signal will add to it;
* 
*    DEPENDENCIES    None
* 
*    PARAMETERS      None
* 
*    RETURN VALUE    WE_INT32:   EME_SUCCESS if process signal successful
*                              EME_FAILED if process signal failed   
*     
*    SIDE EFFECTS    None   
* 
*===========================================================================*/
WE_INT32 EME_AddNewTransaction(St_TMHandle *pstTMHandle,St_TransEvt* pstTransData)
{
   WE_INT32          iResult = EME_SUCCESS;
   St_TMHandle     *pMe = (St_TMHandle*)pstTMHandle;
   St_TransEvt     *pstTransEvt = pstTransData;

   if(NULL == pstTMHandle || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do
   {
      EME_UTILS_LOG_INFO(("EME_AddNewTransaction Start-- "));
      switch(pstTransData->uiTransState)
      {
      case EME_TRANS_STATUS_PENDING:
         EME_list_add_tail(&pstTransEvt->stTransList, &pMe->stTransPendingQueueFirst);
         break;
      case EME_TRANS_STATUS_CANCELED:
         {
            St_TransEvt * pstEvt = NULL;

            list_for_each_entry(St_TransEvt, pstEvt, &pMe->stTransPendingQueueFirst, stTransList)
            {
               if(pstEvt->uiTransID == pstTransData->uiTransID)
               {
                  iResult = EME_TransactionFlush(pstTMHandle, pstEvt->uiTransID);
                  break;
               }
            }
            list_for_each_entry(St_TransEvt, pstEvt, &pMe->stTransQueueFirst, stTransList)
            {
               if(pstEvt->uiTransID == pstTransData->uiTransID)
               {
                  iResult = EME_TransactionFlush(pstTMHandle, pstEvt->uiTransID);
                  break;
               }   
            }   
         }
         break;

      default:
         iResult = EME_FAILED;
         break;
      }

      /************************************************************************/
      /* if the app need to cancel the transaction                            */
      /************************************************************************/
      if(EME_SUCCESS == iResult)
      {
         OEMTMLib_EmailTriggerDispatcher(pstTMHandle);
      }

   } while(0);
   EME_UTILS_LOG_INFO(("EME_AddNewTransaction End-- iRet = %d", iResult));
   return iResult;
}


static WE_INT32 EME_TransactionCancelReply(St_TMHandle *pstTMHandle,
                                           WE_UINT32 uiTransID,
                                           WE_UINT32 uiTransType,
                                           WE_UINT32 uiMsgId,
                                           WE_UINT32 uiClsId)
{
   WE_INT32      iRet = EME_SUCCESS;
   St_TMHandle * pMe = (St_TMHandle*)pstTMHandle;

   if(NULL == pMe)
   {
      return EME_FAILED;
   }
   do 
   {  
      /*Remove temp folder if exist*/
      EME_RemoveDir(EME_OEMTMLib_GetIFileMgr(pstTMHandle), EMEFS_TEMP_ROOT_DIR);

      iRet = OEMTMLib_TransReply(pstTMHandle, uiTransID, uiTransType, uiMsgId, uiClsId, EME_USERCANCEL, NULL);

   } while(0);


   return iRet;
}

