/*=====================================================================================
FILE NAME   : oem_tmlib.c
MODULE NAME : Email engine

GENERAL DESCRIPTION
Provide some utils for tm use.

TechFaith Software Confidential Proprietary
Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
Revision History

Modification                   Tracking
Date              Author       Number       Description of changes
----------   --------------   ---------   --------------------------------------
2007-03-02     Jiang Dingqing                  create oem_tmlib.c

=====================================================================================*/


/*===================================================================
* 
*                     Include Files
* 
====================================================================*/
/*Include System head file*/
#include "brweng_tf2hz_email.h"

#include "we_def.h"
/*Include Program Global head file*/
#include "eme_utils_api.h"
#include "eme_tm_utils_api.h"
#include "eme_error.h"
#include "aee_emetm_api.h"
#include "eme_sig_api.h"
#include "eme_transaction_api.h"
/*Include Module head file*/
/*Include Private head file*/
#include "oem_tmlib_api.h"


/*=============================================================================
* 
*     DEFINITIONS AND CONSTANTS(It is not recommended)
* It is recommended that this section is defined to private head file.
* 
=============================================================================*/
#define EME_NET_LINERTIME     0x01

#define EME_NOTIFY_COUNT      0x10
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

/*=============================================================================
*
*   File Static Variable Define Section
*
=============================================================================*/
WE_INT32 OEMTMLib_EmailTriggerDispatcher(St_TMHandle *pstTmHandle)
{
   St_TMHandle* pTMHandle = pstTmHandle;
   WE_INT32 iResult = EME_SUCCESS;
   EME_TM_STATE tuiState = EME_TM_STATE_FREE;

   EMETM* pEMETM = NULL;

   EME_UTILS_LOG_INFO(("OEMTMLib_EmailTriggerDispatcher start"));
   
   if(NULL == pTMHandle)
   {
      EME_UTILS_LOG_INFO(("OEMTMLib_EmailTriggerDispatcher bad parameter "));
      return EME_BADPARM;
   }

   do 
   {
      pEMETM = pTMHandle->pvParent;
      EME_UTILS_BREAKNULL(pEMETM, iResult, EME_FAILED, "OEMTMLib_EmailTriggerDispatcher, QMETM = NULL");
      /*if the callback was still in the queue ,,with state pending,no callback can be registered in the queue..*/
      if(!CALLBACK_IsQueued(&pEMETM->m_SigCallback))
      {
         if(EME_SUCCESS == (iResult = EME_SeekExistingSignal(pTMHandle)))
         {            
            EME_UTILS_LOG_INFO(("OEMTMLib_EmailTriggerDispatcher add SIG callback "));
            /*NO cancel function in callback,, the callback is free,,reset the callback state*/
            EME_UTILS_MEMSET(&pEMETM->m_SigCallback,0 ,sizeof(AEECallback));
            CALLBACK_Init(&pEMETM->m_SigCallback,EMETM_SignalProcess,pTMHandle);

            ISHELL_EmailResume(&pEMETM->m_SigCallback);

            return iResult;
         }  
      }

      tuiState = OEMTMLib_GetEmlTMState(pstTmHandle);
      if(EME_TM_STATE_BUSY == tuiState)
      {
         EME_UTILS_LOG_INFO(("OEMTMLib_EmailTriggerDispatcher: tm is busy"));
         return EME_SUCCESS;
      }

      if(!CALLBACK_IsQueued(&pEMETM->m_TransCallback))
      {        
         if(EME_SUCCESS == (iResult = EME_FindPendingTrans(pTMHandle)))
         {
             OEMTMLib_SetEmlTMState(pstTmHandle, EME_TM_STATE_BUSY);
            EME_UTILS_LOG_INFO(("OEMTMLib_EmailTriggerDispatcher add trans callback "));
            EME_UTILS_MEMSET(&pEMETM->m_TransCallback,0 ,sizeof(AEECallback));
            CALLBACK_Init(&pEMETM->m_TransCallback,(PFNNOTIFY)EMETM_HandleTransactions,pTMHandle);
            ISHELL_EmailResume(&pEMETM->m_TransCallback);
         }
      }
   } while(0);

   EME_UTILS_LOG_INFO(("OEMTMLib_EmailTriggerDispatcher end iResult = %d ", iResult));
   return iResult;
}



WE_VOID* EME_OEMTMLib_GetMemHdr(St_TMHandle *pstTmHandle)
{
   St_TMHandle* pstHandle = NULL;
   EMETM* pEMETM = NULL;

   /*check input parameters*/
   if(NULL == (pstHandle = (St_TMHandle*)pstTmHandle) ||
      NULL == (pEMETM = ((EMETM*)pstHandle->pvParent)))
   {
      return NULL;
   }

   return (void*)pEMETM->pMemHdr;
}

/*
WE_INT32 OEMTMLib_SeletNetWork(St_TMHandle *pstTmHandle, WE_INT32 iPDP)
{
INetMgr  *piNetMgr = NULL;
WE_INT32 iRet = EME_SUCCESS;
WE_INT   iNetWork = 0;
WE_INT16 iTmp = 0;
WE_CHAR  acCard[5] = {"card"};
WE_CHAR  acPPP[10] = {0};

if (pstTmHandle == NULL)
{
return EME_FAILED;
}

do 
{
piNetMgr = OEMTMLib_GetINetMgr(pstTmHandle);
EME_UTILS_BREAKNULL(piNetMgr, iRet, EME_FAILED, "OEMTMLib_SeletNetWork, get net mgr failed");

if (iPDP != EME_NETWORK_DEFAULT)
{
iTmp = (WE_INT16)iPDP;
iNetWork = AEE_NETWORK_UMTS(iTmp);
}
else
{
iNetWork = AEE_NETWORK_DEFAULT;
}
iRet = INETMGR_SelectNetwork(piNetMgr, iNetWork);
EME_UTILS_LOG_ERROR(("OEMTMLib_SeletNetWork, select network ret: %x", iRet));

EME_UTILS_MEMCPY((void*)acPPP, (const void *)acCard, 5);
EME_UTILS_MEMCPY((void*)(acPPP+5), (const void *)acCard, 5);
iRet = INETMGR_SetOpt(piNetMgr, INET_OPT_PPP_AUTH, acPPP, 10);
EME_UTILS_LOG_ERROR(("OEMTMLib_SeletNetWork, set ppp ret: %x", iRet));

INETMGR_SetLinger(piNetMgr, EME_NET_LINERTIME);

iRet = EME_SUCCESS;
} while(0);

return iRet;
}
*/

WE_VOID* OEMTMLib_GetINetMgr(St_TMHandle *pstTmHandle)
{
   St_TMHandle* pstHandle = NULL;
   EMETM* pEMETM = NULL;

   /*check input parameters*/
   if(NULL == (pstHandle = (St_TMHandle*)pstTmHandle) ||
      NULL == (pEMETM = ((EMETM*)pstHandle->pvParent)))
   {
      return NULL;
   }

   return (void*)pEMETM->piNetMgr;
}

WE_VOID* EME_OEMTMLib_GetIFileMgr(St_TMHandle *pstTmHandle)
{
   St_TMHandle* pstHandle = NULL;
   EMETM* pEMETM = NULL;

   /*check input parameters*/
   if(NULL == (pstHandle = (St_TMHandle*)pstTmHandle) ||
      NULL == (pEMETM = ((EMETM*)pstHandle->pvParent)))
   {
      return NULL;
   }

   return (void*)pEMETM->piFileMgr;
}

WE_VOID* EME_OEMTMLib_GetIShell(St_TMHandle *pstTmHandle)
{
   St_TMHandle* pstHandle = NULL;
   EMETM* pEMETM = NULL;

   /*check input parameters*/
   if(NULL == (pstHandle = (St_TMHandle*)pstTmHandle) ||
      NULL == (pEMETM = ((EMETM*)pstHandle->pvParent)))
   {
      return NULL;
   }

   return (void*)pEMETM->piShell;
}


WE_VOID* OEMTMLib_GetHCSC(St_TMHandle *pstTmHandle)
{
   St_TMHandle* pstHandle = NULL;
   EMETM* pEMETM = NULL;

   /*check input parameters*/
   if(NULL == (pstHandle = (St_TMHandle*)pstTmHandle) ||
      NULL == (pEMETM = ((EMETM*)pstHandle->pvParent)))
   {
      return NULL;
   }

   return (void*)pEMETM->pvCSC;
}


WE_INT32 OEMTMLib_TransReply(St_TMHandle *pstTMHandle, 
                             WE_UINT32 uiTransID,
                             WE_UINT32 uiTransType,
                             WE_UINT32 uiMsgId,
                             WE_UINT32 uiClsId,
                             WE_INT32 iResult,
                             WE_CHAR  *pcRespDesc)
{
   WE_INT32    iRet = EME_SUCCESS;
   St_EMETransResult *pstTransResult = NULL;
   if (pstTMHandle == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      pstTransResult = EME_UTILS_MALLOC(NULL, sizeof(St_EMETransResult));
      EME_UTILS_BREAKNULL(pstTransResult, iRet, EME_NOMEMORY, "no memory for transaction result report");
      EME_UTILS_MEMSET(pstTransResult, 0x00, sizeof(St_EMETransResult));

      pstTransResult->iResult = iResult;
      pstTransResult->uiTransId = uiTransID;
      pstTransResult->uiTransType = uiTransType;
	  pstTransResult->uiMsgId = uiMsgId;
      if(NULL != pcRespDesc && EME_UTILS_STRLEN(pcRespDesc) > 0)
         EME_UTILS_MEMCPY(pstTransResult->acRespDesc, pcRespDesc, EME_UTILS_STRLEN(pcRespDesc));

      EME_UTILS_LOG_INFO(("OEMTMLib_TransReply :pstTMHandle->stTransRet.uiTransType = %d", pstTransResult->uiTransType));
      EME_UTILS_LOG_INFO(("OEMTMLib_TransReply :pstTMHandle->stTransRet.iResult = %d", pstTransResult->iResult));
      TS_HZ_EMAIL_EVENT_CALLBACK(BRWENG_EMAIL_MMI_APP, EVT_EME_TRANS_RESULT, (void*)pstTransResult, NULL);

   } while(0);


   return iRet;
}

#define OEMTMLIB_NOTIFYSIZE_EACH_TICKCOUNT   (500)

WE_INT32 OEMTMLib_NotifySize(St_TMHandle *pstTMHandle, 
                             St_TransEvt* pstUserData, 
                             WE_UINT32 uiTransferSize, 
                             WE_UINT32 uiTotalSize)
{
   WE_INT32          iRet = EME_SUCCESS;
   WE_UINT32         uiClsId = 0;
   St_EMESizeNotify  *pstSizeNotify = NULL;
   WE_BOOL           bNeedSend = FALSE;
   WE_UINT32         uiRal     = 0;

   if(NULL == pstTMHandle|| NULL == pstUserData || 0 == uiTotalSize)
   {
      return EME_FAILED;
   }

   if(pstUserData->uiTickCount == 0 && !pstUserData->bFirstPackage)
   {
      pstUserData->bFirstPackage = TRUE;
      pstUserData->uiTickCount = TF3RD_TICKCOUNT();
      bNeedSend = TRUE;
   }
   else if(uiTransferSize == uiTotalSize && !pstUserData->bLastPackage)
   {
      pstUserData->bLastPackage = TRUE;
      pstUserData->uiTickCount = TF3RD_TICKCOUNT();
      bNeedSend = TRUE;
   }
   else
   {
      WE_UINT32 uiTickCount = TF3RD_TICKCOUNT();

      if(uiTickCount - pstUserData->uiTickCount >= OEMTMLIB_NOTIFYSIZE_EACH_TICKCOUNT)
      {
         pstUserData->uiTickCount = uiTickCount;
         bNeedSend = TRUE;
      }
   }

   if(!bNeedSend)
   {
      return EME_SUCCESS;
   }

   do
   {
      /*
      pstTMHandle->iNotifyCount++;
      if (pstTMHandle->iNotifyCount < EME_NOTIFY_COUNT && uiTransferSize != uiTotalSize)
      {
      break;
      }
      pstTMHandle->iNotifyCount = 0;	
      */

      pstSizeNotify = EME_UTILS_MALLOC(NULL, sizeof(St_EMESizeNotify));
      EME_UTILS_BREAKNULL(pstSizeNotify, iRet, EME_NOMEMORY, "no memory for size notify");
      EME_UTILS_MEMSET(pstSizeNotify, 0, sizeof(St_EMESizeNotify));

      pstSizeNotify->uiTransId = pstUserData->uiTransID;
      pstSizeNotify->uiTotalSize = uiTotalSize;
      pstSizeNotify->uiTransferedSize = uiTransferSize;
      pstSizeNotify->uiTransType = pstUserData->uiTransType;

//      iRet = EME_GetIntOptValue(pstUserData->pstTransOpts, EME_OPTID_REPLY_CLSID, (WE_INT32*)&uiClsId);
//      EME_UTILS_BREAKIF(iRet, "OEMTMLib_NotifySize, get cls id failed");

      EME_UTILS_LOG_INFO(("OEMTMLib_NotifySize :pstTMHandle->stSizeNotify.uiTransType = %d", pstSizeNotify->uiTransType));
      EME_UTILS_LOG_INFO(("OEMTMLib_NotifySize :pstTMHandle->stSizeNotify.uiTotalSize= %d", pstSizeNotify->uiTotalSize));
      EME_UTILS_LOG_INFO(("OEMTMLib_NotifySize :pstTMHandle->stSizeNotify.uiTransferedSize = %d", pstSizeNotify->uiTransferedSize));

      TS_HZ_EMAIL_EVENT_CALLBACK(BRWENG_EMAIL_MMI_APP, EVT_EME_MESSAGE_SIZE_NOTIFY, (void*)pstSizeNotify, NULL);

   } while(0);

   return iRet;
}


WE_INT32 OEMTMLib_NotifyNumber(St_TMHandle *pstTMHandle, 
                               St_TransEvt* pstUserData, 
                               WE_UINT32 uiCurrMsg, 
                               WE_UINT32 uiTotalNum)
{
   WE_INT32          iRet = EME_SUCCESS;  
   WE_UINT32         uiClsId= 0;
   St_EMENumNotify   *pstNumNotify = NULL;

   if(NULL == pstTMHandle|| NULL == pstUserData )
   {
      return EME_FAILED;
   }

   do 
   {
      pstNumNotify = EME_UTILS_MALLOC(NULL, sizeof(St_EMENumNotify));
      EME_UTILS_BREAKNULL(pstNumNotify, iRet, EME_NOMEMORY, "no memory for number notify");
      EME_UTILS_MEMSET(pstNumNotify, 0, sizeof(St_EMENumNotify));

      pstNumNotify->uiTransId = pstUserData->uiTransID;
      pstNumNotify->uiTotalMsg = uiTotalNum;
      pstNumNotify->uiCurrMsg = uiCurrMsg;
      pstNumNotify->uiTransType = pstUserData->uiTransType;

//      iRet = EME_GetIntOptValue(pstUserData->pstTransOpts, EME_OPTID_REPLY_CLSID, (WE_INT32*)&uiClsId);
//      EME_UTILS_BREAKIF(iRet, "OEMTMLib_NotifyNumber, get cls id failed");

      EME_UTILS_LOG_INFO(("OEMTMLib_NotifyNumber :pstTMHandle->stNumNotify.uiTransType = %d", pstNumNotify->uiTransType));
      EME_UTILS_LOG_INFO(("OEMTMLib_NotifyNumber :pstTMHandle->stNumNotify.uiCurrMsg= %d", pstNumNotify->uiCurrMsg));
      EME_UTILS_LOG_INFO(("OEMTMLib_NotifyNumber :pstTMHandle->stNumNotify.uiTotalMsg = %d", pstNumNotify->uiTotalMsg));
      TS_HZ_EMAIL_EVENT_CALLBACK(BRWENG_EMAIL_MMI_APP, EVT_EME_MESSAGE_NUMBER_NOTIFY, (void*)pstNumNotify, NULL);

   } while(0);

   return iRet;
}


WE_INT32 OEMTMLib_NotifyNewMail(St_TMHandle *pstTMHandle,
                                St_TransEvt* pstUserData,
                                WE_CHAR *pcMsgId,
                                WE_CHAR *pcUidl)
{
#if 0
   WE_INT32          iRet = EME_SUCCESS;
   WE_UINT32         uiClsId = 0;
   St_EMEMsg         *pstNewMsg = NULL;

   if(NULL == pstTMHandle|| NULL == pstUserData )
   {
      return EME_FAILED;
   }

    if(NULL==pcMsgId)
    {
        EME_UTILS_LOG_INFO(("OEMTMLib_NotifyNewMail :pstTMHandle->stMsgNotify.pcMsgId = NULL\n"));
        return EME_FAILED;
    }
    else if(EME_UTILS_STRLEN(pcMsgId)==0)
    {
        EME_UTILS_LOG_INFO(("OEMTMLib_NotifyNewMail :pstTMHandle->stMsgNotify.pcMsgId len = 0\n"));
        return EME_FAILED;
    }

   do 
   {
      pstNewMsg = EME_UTILS_MALLOC(NULL, sizeof(St_EMEMsg));
      EME_UTILS_BREAKNULL(pstNewMsg, iRet, EME_NOMEMORY, "no memory for new message notify");
      EME_UTILS_MEMSET(pstNewMsg, 0, sizeof(St_EMEMsg));

      pstNewMsg->uiTransType = pstUserData->uiTransType;
      pstNewMsg->uiTransId = pstUserData->uiTransID;
      pstNewMsg->pcMsgId = EME_UTILS_STRDUP(NULL, pcMsgId);
      pstNewMsg->pcUidl =  EME_UTILS_STRDUP(NULL, pcUidl);

      iRet = EME_GetIntOptValue(pstUserData->pstTransOpts, EME_OPTID_REPLY_CLSID, (WE_INT32*)&uiClsId);
      EME_UTILS_BREAKIF(iRet, "OEMTMLib_NotifyNewMail, get cls id failed");

      EME_UTILS_LOG_INFO(("OEMTMLib_NotifyNewMail :pstTMHandle->stNumNotify.uiTransType = %d", pstNewMsg->uiTransType));
      EME_UTILS_LOG_INFO(("OEMTMLib_NotifyNewMail :pstTMHandle->stMsgNotify.pcUidl= %s\n", pstNewMsg->pcUidl));
      EME_UTILS_LOG_INFO(("OEMTMLib_NotifyNewMail :pstTMHandle->stMsgNotify.pcMsgId = %s\n", pstNewMsg->pcMsgId));
      TS_HZ_EMAIL_EVENT_CALLBACK(BRWENG_EMAIL_MMI_APP, EVT_EME_NEW_MESSAGE, (void*)pstNewMsg, NULL);

   } while(0);

   return iRet;
#endif
   return EME_SUCCESS;
}

WE_INT32 OEMTMLib_NotifyMailbox(St_TMHandle *pstTMHandle,
                                St_TransEvt* pstUserData,
                                WE_CHAR *pcMailboxName, 
                                WE_CHAR *pcDelimiter)
{
   WE_INT32          iRet = EME_SUCCESS;
   WE_UINT32         uiClsId = 0;
   St_EMEMailbox     *pstBoxNtf = NULL;

   if(NULL == pstTMHandle|| NULL == pstUserData )
   {
      return EME_FAILED;
   }

   do 
   {
      pstBoxNtf = EME_UTILS_MALLOC(NULL, sizeof(St_EMEMailbox));
      EME_UTILS_BREAKNULL(pstBoxNtf, iRet, EME_NOMEMORY, "no memory for box notify");
      EME_UTILS_MEMSET(pstBoxNtf, 0x00, sizeof(St_EMEMailbox));

      pstBoxNtf->uiTransId = pstUserData->uiTransID;
      pstBoxNtf->uiTransType = pstUserData->uiTransType;

      EME_UTILS_FREEIF(NULL, pstBoxNtf->pcMailboxName);
      EME_UTILS_FREEIF(NULL, pstBoxNtf->pcDelimiter);
      pstBoxNtf->pcMailboxName = EME_UTILS_STRDUP(NULL, pcMailboxName);
      pstBoxNtf->pcDelimiter = EME_UTILS_STRDUP(NULL, pcDelimiter);

//      iRet = EME_GetIntOptValue(pstUserData->pstTransOpts, EME_OPTID_REPLY_CLSID, (WE_INT32*)&uiClsId);
//      EME_UTILS_BREAKIF(iRet, "OEMTMLib_NotifyMailbox, get cls id failed");

      EME_UTILS_LOG_INFO(("OEMTMLib_NotifyMailbox :stNotifyData.uiTransType = %d", pstBoxNtf->uiTransType));
      EME_UTILS_LOG_INFO(("OEMTMLib_NotifyMailbox :stNotifyData.pcMailboxName= %s\n", pstBoxNtf->pcMailboxName));
      TS_HZ_EMAIL_EVENT_CALLBACK(BRWENG_EMAIL_MMI_APP, EVT_EME_MAILBOX_GETED, (void*)pstBoxNtf, NULL);

   } while(0);

   return iRet;
}


WE_INT32 OEMTMLib_NotifyInfo(St_TMHandle *pstTMHandle, 
                             St_TransEvt* pstUserData, 
                             WE_CHAR *pcInfo)
{
   WE_INT32          iRet = EME_SUCCESS;
   WE_UINT32         uiClsId = 0;
   WE_CHAR           *pcResp = NULL;

   if(NULL == pstTMHandle|| pcInfo == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      /*
      pstTMHandle->iNotifyCount++;
      if (pstTMHandle->iNotifyCount < EME_NOTIFY_COUNT && uiTransferSize != uiTotalSize)
      {
      break;
      }
      pstTMHandle->iNotifyCount = 0;	
      */
//      EME_UTILS_MEMSET(&pstTMHandle->acRespDesc, 0, 512);
//
//       if (EME_UTILS_STRLEN(pcInfo) < 512)
//       {
//          EME_UTILS_STRCPY(pstTMHandle->acRespDesc, pcInfo);
//       }

      pcResp = EME_UTILS_STRDUP(NULL, pcInfo);
      EME_UTILS_LOG_INFO(("OEMTMLib_NotifyInfo :pstTMHandle->acRespDesc = %s", pcResp));
      TS_HZ_EMAIL_EVENT_CALLBACK(BRWENG_EMAIL_MMI_APP, EVT_EME_INFO_NOTIFY, (void*)pcResp, NULL);

   } while(0);

   return iRet;
}


WE_VOID OEMTMLib_SetEmlTMState(St_TMHandle *pstTMHandle, EME_TM_STATE tuiState)
{
   EMETM* pEMETM = NULL;

   if(NULL == pstTMHandle)
      return;

   pEMETM = pstTMHandle->pvParent;
   pEMETM->tuiEmlTMState = tuiState;
}

EME_TM_STATE OEMTMLib_GetEmlTMState(St_TMHandle *pstTMHandle)
{
   EMETM* pEMETM = NULL;

   if(NULL == pstTMHandle)
      return 0;

   pEMETM = pstTMHandle->pvParent;
   return pEMETM->tuiEmlTMState;
}
