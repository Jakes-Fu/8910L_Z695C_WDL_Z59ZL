/*=====================================================================================
FILE NAME   : eme_fsm_smtp.c
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Dispatch signals to proper FSM according to signal id.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                  create eme_fsm_smtp.c
          
=====================================================================================*/
//pclint
/*lint -e666 */

/*===================================================================
 * 
 *                     Include Files
 * 
====================================================================*/
/*Include System head file*/
#include "eme_file_api.h"

#include "we_def.h"
/*Include Program Global head file*/
#include "eme_error.h"
#include "eme_utils_api.h"
#include "email_store_api.h"
#include "eme_codec_api.h"
/*Include Module head file*/
#include "eme_tm_def.h"
#include "eme_sig_api.h"
#include "eme_transaction_api.h"
#include "oem_tmlib_api.h"
#include "eme_smtp_api.h"
#include "eme_tm_utils_api.h"
#include "email_store_api.h"
#include "eme_fsm_smtp_api.h"
#include "EMS_Store_Priv.h"
/*Include Private head file*/

/*The header file of dialing*/
//#include "oem_ppp_api.h"


/*=============================================================================
* 
*     DEFINITIONS AND CONSTANTS(It is not recommended)
* It is recommended that this section is defined to private head file.
* 
=============================================================================*/
#define EME_SMTP_MAX_DATE_LEN 20

typedef struct _St_CBEncodeData
{
   void      *pvUserData;
   void      *pvTMHandle;
   void      *pvTranEvent;
   void      *pvCodec;
   WE_UINT32 uiSizeLimit;
   WE_INT32  iEncodingType;
}St_CBEncodeData;

/*=============================================================================
*
*   Static Function Prototype Declare Section
*
=============================================================================*/
static WE_INT32 EME_FSMSMTP_TransStart(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMSMTP_Open(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMSMTP_Send(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMSMTP_Forward(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMSMTP_SendReceipt(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMSMTP_Close(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMSMTP_TransEnd(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);

static WE_INT32 EME_FSMSMTP_Cancel(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
/* callback functions */
static WE_INT32 EME_CBSMTP_Open(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_SMTPActRet* pstActRet);
static WE_INT32 EME_CBSMTP_Sending(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_SMTPActRet* pstActRet);
static WE_INT32 EME_CBSMTP_Send(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_SMTPActRet* pstActRet);
static WE_INT32 EME_CBSMTP_Close(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_SMTPActRet* pstActRet);

static WE_INT32 EME_CBSMTP_Forwarding(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_SMTPActRet* pstActRet);
static WE_INT32 EME_CBSMTP_Forward(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_SMTPActRet* pstActRet);


static  void EME_FSMSMTP_EncodingMsgCB(IEMSGHandle hEmailHandle, EmailOpt *ppstOpts, void *pvUserData, int iResult);

static void  EME_FSMSMTP_pfnFree(St_TMHandle *pstTMHandle, void* pvTransEvt);
/* free functions */
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
 /* --------------------------------------------------------------------
 * 
 *    FUNCTION    EME_HandleSMTPSig   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. dispatch signals to proper FSM.
 *              3. delete the signal after it is handled.
*    ================================================================*/
WE_INT32 EME_HandleSMTPSig(St_TMHandle *pstTmHandle, WE_UINT32 uiEvtCode, WE_UINT16 usWParam, WE_UINT32 ulDwParam)
{
	WE_INT32    iRet = EME_SUCCESS;

   if (pstTmHandle == NULL)
   {
      EME_UTILS_LOG_INFO(("EME_HandleSMTPSig: BADPARAM!"));
      return EME_FAILED;
   }
   
   EME_UTILS_LOG_INFO(("EME_HandleSMTPSig: uiEvtCode = %d!", uiEvtCode));
   EME_UTILS_LOG_INFO(("EME_HandleSMTPSig: uiEvtCode = %d!", ulDwParam));	
   switch(uiEvtCode)
   {
   case EME_SIG_SMTP_TRANS_START:
      iRet = EME_FSMSMTP_TransStart(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;
   case EME_SIG_SMTP_OPEN:
      iRet = EME_FSMSMTP_Open(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;
   case EME_SIG_SMTP_SEND:
      iRet = EME_FSMSMTP_Send(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;
   case EME_SIG_SMTP_SEND_RECEIPT:
      iRet = EME_FSMSMTP_SendReceipt(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;
   case EME_SIG_SMTP_FORWARD:
      iRet = EME_FSMSMTP_Forward(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;
   case EME_SIG_SMTP_CLOSE:
      iRet = EME_FSMSMTP_Close(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;
   case EME_SIG_SMTP_CANCEL:
      iRet = EME_FSMSMTP_Cancel(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;
   case EME_SIG_SMTP_TRANS_END:
      iRet = EME_FSMSMTP_TransEnd(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;
   default:
      break;
   }

   EME_SignalDelete(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);

   return iRet;
}

 /* --------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMPOP_TransStart   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the smtp state is login, trans fsm to smtp operation fsm,
                   else select network,trans fsm to SMTP_OPEN
 *              3. if there is something wrong,trans fsm to TRANS_END
 *  
*    =========================================================================*/
static WE_INT32 EME_FSMSMTP_TransStart(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   WE_UINT32      uiSigId = 0;
//   WE_INT32       iPDP = 0;

   if (pstTmHandle == NULL || pstSig == NULL)
   {
     EME_UTILS_LOG_INFO(("EME_FSMSMTP_TransStart : BADPARAM!"));
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;	
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMSMTP_SendStart, trans evt error");

      pstTmHandle->iCanBeCancel = EME_CANCEL_ENABLED;

//      if(!EME_DataAccess_IsEnoughSpace(EME_OEMTMLib_GetIFileMgr(pstTmHandle), EMS_LEAST_EML_SPACE))
//      {
//         iRet = EME_NOSTORAGE;
//         EME_UTILS_BREAKIF(iRet, "no storeage for new mail");
//      }
      
      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_SMTP_TRANS_START;

	 EME_UTILS_LOG_INFO(("EME_FSMSMTP_TransStart : pstTmHandle->iSMPTState  = %d!", pstTmHandle->iSMPTState ));  

      if (pstTmHandle->iSMPTState == EME_STACK_STATE_LOGIN)
      {
          EME_UTILS_LOG_INFO(("EME_FSMSMTP_TransStart : pstTransEvt->uiTransType = %d!", pstTransEvt->uiTransType));
         switch(pstTransEvt->uiTransType)
         {
         case EME_TRANS_SMTP_SEND_MAIL:
            uiSigId = EME_SIG_SMTP_SEND;
         	break;
         case EME_TRANS_SMTP_SEND_RECEIPT:
            uiSigId = EME_SIG_SMTP_SEND_RECEIPT;
            break;
         case EME_TRANS_SMTP_FORWARD_WHOLEMAIL:
         case EME_TRANS_SMTP_FORWARD_ATTACHMENT:
            uiSigId = EME_SIG_SMTP_FORWARD;
            break;
         default:
            iRet = EME_FAILED;
            break;
         }
         EME_UTILS_LOG_INFO(("EME_FSMSMTP_TransStart : uiSigId = %d, iret = %d\n!", uiSigId, iRet));		 
         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_SMTP, uiSigId, (WE_VOID*)pstTransEvt);
      }
      else
      {  /*selet network*/
//         iRet = EME_GetIntOptValue(pstTmHandle->pstAccountOpts, EME_OPTID_NETWORK_UMTS_PDP, &iPDP);
//         if (iRet != SUCCESS)
//         {
//            iPDP = EME_NETWORK_DEFAULT;
//         }
//
//         iRet = OEMTMLib_SeletNetWork(pstTmHandle, iPDP);
//         if (iRet == AEE_NET_EOPNOTSUPP)
//         {
//            iRet = EME_SUCCESS;
//         }
//         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_TransStart, select network failed");
           EME_UTILS_LOG_INFO(("EME_FSMSMTP_TransStart : !! EME_STACK_STATE_LOGIN"));
         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_OPEN, (WE_VOID*)pstTransEvt);
      }
      
      /* register free function */
      pstTransEvt->pfnFreeSigData = NULL;

   } while(0);

   if (iRet != EME_SUCCESS)
   {
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_TRANS_END, iRet, pstTransEvt);
   }
  EME_UTILS_LOG_INFO(("EME_FSMSMTP_TransStart: return = %d", iRet));
   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMSMTP_Open   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. connec to pop server
 *              3. if there is something wrong,trans fsm to TRANS_END
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMSMTP_Open(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   ISMTPHandle    pvSMTP = NULL;
   St_SMTPConfigInfo stConfigInfo;
   WE_INT32       iPort = 0;

   if (pstTmHandle == NULL || pstSig == NULL)
   {
      EME_UTILS_LOG_INFO(("EME_FSMSMTP_Open:  BADPARAM!"));
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);
   
   do 
   {
      EME_UTILS_MEMSET(&stConfigInfo, 0, sizeof(St_SMTPConfigInfo));

      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;	
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMSMTP_Open, trans evt error");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_SMTP_OPEN;

      /* get transData */
      pvSMTP = pstTmHandle->pvSMPT;
      EME_UTILS_BREAKNULL(pvSMTP, iRet, EME_FAILED, "EME_FSMSMTP_Open, smtp handle error");
      /*get account info that smtp need*/
      iRet = EME_GetStrOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_SEND_PASSWORD, &stConfigInfo.pcPassword);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Open, get password failed");
      EME_UTILS_BREAKNULL(stConfigInfo.pcPassword, iRet, EME_FAILED, "EME_FSMSMTP_Open, get password failed");
      
      iRet = EME_GetStrOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_SEND_USERNAME, &stConfigInfo.pcUserName);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Open, get username failed");
      EME_UTILS_BREAKNULL(stConfigInfo.pcUserName, iRet, EME_FAILED, "EME_FSMSMTP_Open, get username failed");

      iRet = EME_GetStrOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_SEND_SERVER_ADDR, &stConfigInfo.pcSmtpSerName);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Open, get ser address failed");
      EME_UTILS_BREAKNULL(stConfigInfo.pcSmtpSerName, iRet, EME_FAILED, "EME_FSMSMTP_Open, get ser address failed");

      iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_SEND_SERVER_PORT, &iPort);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Open, get port failed");
      stConfigInfo.usPort = (WE_UINT16)iPort;

      iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_SMTP_NEED_SSL, &stConfigInfo.iIsNeedSSL);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Open, get ssl opt failed");

      iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_SMTP_AUTHENTICATE, &stConfigInfo.iIsAuth);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Open, get ssl opt failed");      

      /* record smtp callback data */
      pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;

      /* smtp open */
      iRet = EME_SMTP_Open(pvSMTP, &stConfigInfo);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Open, smtp open failed");
      
      /* register free function */
      pstTransEvt->pfnFreeSigData = NULL;

   } while(0);

   if (iRet != EME_SUCCESS)
   {
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_TRANS_END, iRet, pstTransEvt);
   }

  EME_UTILS_LOG_INFO(("EME_FSMSMTP_Open: end return = %d", iRet));
   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMSMTP_Send   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. get email from store by msg id and encode it
                3. check the email encode data size whether longer than the max length
                4. send the encode email data
 *              5. if there is something wrong,trans fsm to TRANS_END
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMSMTP_Send(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   WE_VOID        *pvFileMgr = NULL;
   WE_CHAR        *pcMsgId = NULL;
   St_RawMailInfo *pstMailInfo = NULL;
   HEmailStore    pvStore = NULL;
   WE_UINT32      uiPartNum = 0;
   IEMSGHandle    pvCodec = NULL;
   EmailOpt       *pstMsgOpts = NULL;
   WE_UINT32       i = 0;
   WE_UINT32      uiSizeLimit = 0;
   WE_UINT32      uiMessageSize = 0;
   WE_CHAR        *pcTmp = NULL;
   WE_INT32       iSrcType = 0;

   WE_UINT32      uiMsgId = 0;
   WE_CHAR        *pcPath = NULL;
//   EmailOpt* pstCodecOpt = NULL;

   St_CBEncodeData  *pstCBData = NULL;
   
   EME_UTILS_LOG_INFO(("EME_FSMSMTP_Send---: Start"));
   if (pstTmHandle == NULL || pstSig == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_FSMSMTP_Send--: Bad parameter and return"));
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      pstMailInfo = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_RawMailInfo));
      EME_UTILS_BREAKNULL(pstMailInfo, iRet, EME_NOMEMORY, "no memory for raw mail info");
      EME_UTILS_MEMSET(pstMailInfo, 0, sizeof(St_RawMailInfo));

      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMSMTP_Send, transEvt is null");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_SMTP_SEND;

//      pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
//      EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_FSMSMTP_Send, file mgr is null");

      /* get message id */
      iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, get message id failed");
      EME_UTILS_BREAKNULL(pcMsgId, iRet, EME_FAILED, "EME_FSMSMTP_Send, get message id failed");
      
      uiMsgId = EME_UTILS_ATOX(pcMsgId);
      
      iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_SIZE_LIMIT, (WE_INT32*)&uiSizeLimit);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, get message id failed");

      /* init store handle */
      iRet = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, create store handle failed");
      EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_FSMSMTP_Send, create store handle failed");

      /* init codec handle */
      iRet = EMECodecLib_InitHandleEx(&pvCodec, pvMemHdr, pvFileMgr, OEMTMLib_GetHCSC(pstTmHandle), pstTmHandle->pvAsyncMgr);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, create codec handle failed");
      EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_FSMSMTP_Send, create codec handle failed");

      /* get part count */
      iRet = EmailStore_GetMsgPartCount(pvStore, uiMsgId, &uiPartNum);
     EME_UTILS_LOG_INFO(("EME_FSMSMTP_Send: EmailStore_GetMsgPartCount ==partnum = %d, nret = %d\n",uiPartNum, iRet));
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, get message part num failed");

      /* get message header */
      iRet = EmailStore_GetMsgHeaders(pvStore, uiMsgId, &pstMsgOpts);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, get message header failed");
      EME_UTILS_BREAKNULL(pstMsgOpts, iRet, EME_FAILED, "EME_FSMSMTP_Send, get message header failed");

      /* get from */
      iRet = EME_GetStrOptValue((EMEOpt*)pstMsgOpts, EMAIL_OPT_FROM, &pcTmp);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, get from failed");
      EME_UTILS_BREAKNULL(pcTmp, iRet, EME_FAILED, "EME_FSMSMTP_Send, get from failed");
      pstMailInfo->pcFrom = EME_UTILS_STRDUP(pvMemHdr, pcTmp);
     EME_UTILS_LOG_INFO(("EME_FSMSMTP_Send:pstMailInfo->pcFrom  = %s\n", pstMailInfo->pcFrom ));
      EME_UTILS_BREAKNULL(pstMailInfo->pcFrom, iRet, EME_NOMEMORY, "strdup from address failed");
      pcTmp = NULL;
   
      /* get "to", "cc", "bcc", either one will work */
      EME_GetStrOptValue((EMEOpt*)pstMsgOpts, EMAIL_OPT_TO, &pcTmp);
      EME_UTILS_LOG_INFO(("EME_FSMSMTP_Send:EME_GetStrOptValue pcTO  = %s\n", pcTmp ));	  
      if(pcTmp != NULL)
      {
         pstMailInfo->pcTo = EME_UTILS_STRDUP(pvMemHdr, pcTmp);
         EME_UTILS_LOG_INFO(("EME_FSMSMTP_Send:  pstMailInfo->pcTo  = %s\n", pstMailInfo->pcTo ));	  		 
         EME_UTILS_BREAKNULL(pstMailInfo->pcTo, iRet, EME_NOMEMORY, "strdup to address failed");
         pcTmp = NULL;
      }
      EME_GetStrOptValue((EMEOpt*)pstMsgOpts, EMAIL_OPT_CC, &pcTmp);
      if(NULL != pcTmp)
      {
         pstMailInfo->pcCc = EME_UTILS_STRDUP(pvMemHdr, pcTmp);
         EME_UTILS_BREAKNULL(pstMailInfo->pcCc, iRet, EME_NOMEMORY, "strdup cc address failed");
         pcTmp = NULL;
      }
      EME_GetStrOptValue((EMEOpt*)pstMsgOpts, EMAIL_OPT_BCC, &pcTmp);
      if(NULL != pcTmp)
      {
         pstMailInfo->pcBcc = EME_UTILS_STRDUP(pvMemHdr, pcTmp);
         EME_UTILS_BREAKNULL(pstMailInfo->pcBcc, iRet, EME_NOMEMORY, "strdup bcc address failed");
         pcTmp = NULL;
      }
      pstMailInfo->uiSrcType = pstTransEvt->uiTransType;
      
      if (NULL == pstMailInfo->pcTo &&
          NULL == pstMailInfo->pcCc &&
          NULL == pstMailInfo->pcBcc)
      {
          EME_UTILS_LOG_INFO(("EME_FSMSMTP_Send:  To,bcc, cc all are null!"));
         iRet = EME_FAILED;
         break;
      }

      /* add message header to codec */
      iRet = EMECodecLib_AddHeader(pvCodec, pstMsgOpts);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, add message header failed");

      /* get and add message parts */
      for (i = 0; i < uiPartNum; i++)
      {
         pstMsgOpts = NULL;

         iRet = EmailStore_GetMsgPart(pvStore, uiMsgId, i, &pstMsgOpts);
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, get message part failed");
         EME_UTILS_BREAKNULL(pstMsgOpts, iRet, EME_FAILED, "EME_FSMSMTP_Send, get message part failed");

         iRet = EME_GetIntOptValue((EMEOpt*)pstMsgOpts, EMAILPART_OPT_SRC_TYPE, &iSrcType);
          EME_UTILS_LOG_INFO(("EME_FSMSMTP_Send: EME_GetIntOptValue iSrcType =%d", iSrcType));
         EME_UTILS_LOG_INFO(("EME_FSMSMTP_Send: EME_GetIntOptValue return =%d", iRet));
         iRet += EME_GetStrOptValue((EMEOpt*)pstMsgOpts, EMAILPART_OPT_DATA_BUFF, &pcTmp);
         EME_UTILS_LOG_INFO(("EME_FSMSMTP_Send: EMAILPART_OPT_DATA_BUFF pcTmp =%s\n", pcTmp));
#if 0
         if (iRet == EME_SUCCESS && pcTmp != NULL && iSrcType == EMAILPART_SRCTYPE_BUFF)
         {
            uiMessageSize += EME_UTILS_BASE64LEN(EME_UTILS_STRLEN(pcTmp));
            if (uiMessageSize > (uiSizeLimit * 4 / 3) *1024)
            {
               iRet = EME_SIZE_LIMIT_EXCEED;
               EME_UTILS_BREAK(iRet, "EME_FSMSMTP_Send, size limited");
            }
         }
         else if (iRet == EME_SUCCESS && pcTmp != NULL && iSrcType == EMAILPART_SRCTYPE_FILE)
         {
            FILE_HANDLE  hFile = NULL;
             EME_UTILS_LOG_INFO(("EME_FSMSMTP_Send : fpath = %s\n", pcTmp));
            hFile = EME_FileOpen(pvFileMgr, pcTmp, EME_OFM_READ);
            if (hFile != NULL)
            {
               uiMessageSize += EME_UTILS_BASE64LEN(EME_FileLength(hFile));
               if (uiMessageSize > (uiSizeLimit * 4 / 3) * 1024)
               {
                  iRet = EME_SIZE_LIMIT_EXCEED;
                  EME_FileClose(hFile);
                  EME_UTILS_BREAK(iRet, "EME_FSMSMTP_Send, size limited");
               }
            }
            EME_FileClose(hFile);
         }
#endif
         iRet = EMECodecLib_AddPartEx(pvCodec, pstMsgOpts);
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, add message part failed");
      }
      EME_UTILS_BREAKIF(iRet, "get or add message part failed");
/*
      iRet = EmailStore_GetMsgPath(pvStore, uiMsgId, &pcPath);
      EME_UTILS_LOG_INFO(("EME_FSMSMTP_Send: EmailStore_GetMsgPath uiMsgId= %d\n", uiMsgId));	  
      EME_UTILS_LOG_INFO(("EME_FSMSMTP_Send: EmailStore_GetMsgPath pcPath= %s\n", pcPath));
      EME_UTILS_BREAKNULL(pcPath, iRet, EME_FAILED, "path is null");
*/
      pcPath = EMEFS_TEMP_ROOT_DIR;

      pstCBData = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_CBEncodeData));
      EME_UTILS_BREAKNULL(pstCBData, iRet, EME_NOMEMORY, "no memory for encode data");
      EME_UTILS_MEMSET(pstCBData, 0, sizeof(St_CBEncodeData));

      pstCBData->pvTMHandle  = pstTmHandle;
      pstCBData->pvTranEvent = pstTransEvt;
      pstCBData->pvUserData  = (void*)pstMailInfo;
      pstCBData->uiSizeLimit = uiSizeLimit;
      pstCBData->pvCodec     = pvCodec;
      
      /*encode email*/
      iRet = EMECodecLib_EncodingEx(pvCodec, pcPath, pstCBData, (PFNENCODINGCB)EME_FSMSMTP_EncodingMsgCB, NULL);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, encoding message failed");
      pstTransEvt->pfnFreeSigData = (PFNFREESIGDATA)EME_FSMSMTP_pfnFree;
      pstTransEvt->pvTransData    = pstCBData;
    
//      iRet = EMECodecLib_EncodingEx(pvCodec, pcPath, pstCBData, NULL, &pstCodecOpt);
//      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, encoding message failed");
//      (void)EME_FSMSMTP_EncodingMsgCB(pvCodec, pstCodecOpt, pstCBData, EME_SUCCESS);
      
      EmailStore_DeleteHandle(pvStore);
      pvStore = NULL;

   } while(0);

   
   if (iRet != EME_SUCCESS)
   {  
      EME_UTILS_FREEIF(pvMemHdr, pstCBData);
      EMECodecLib_DeleteHandle(&pvCodec);
      EmailStore_DeleteHandle(pvStore);
      pvStore = NULL;

      if(NULL != pstMailInfo)
      {
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcBcc);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcCc);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcFrom);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcRawMail);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcTo);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcUID);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo);
      }

      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_TRANS_END, iRet, pstTransEvt);
   }

   EME_UTILS_LOG_INFO(("EME_FSMSMTP_Send---: End, Result = %d", iRet));
   return iRet;
}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMSMTP_SendReceipt   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. get email header from store by msg id & encode header ,then encode text part
                3. send the encode email data
 *              4. if there is something wrong,trans fsm to TRANS_END
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMSMTP_SendReceipt(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   ISMTPHandle    pvSMTP = NULL;
   WE_VOID        *pvFileMgr = NULL;
   WE_CHAR        *pcMsgId = NULL;
   St_RawMailInfo *pstMailInfo = NULL;
   HEmailStore    pvStore = NULL;
   WE_CHAR        *pcText = NULL;
   WE_CHAR        *pcSubject = NULL;
   WE_CHAR        *pcDisplayAddr = NULL;
   IEMSGHandle    pvCodec = NULL;
   EmailOpt       *pstMsgOpts = NULL;
   EmailOpt       stReceiptOpts[EME_SMTP_SEND_RECEIPT_OPT_NUM];
//   EmailOpt       *pstCodecOpt = NULL;
   WE_UINT32      uiSizeLimit = 0;
   WE_UINT32      uiSize = 0;
   WE_INT32       iTimeZone = 0;
   WE_UINT32      uiLocalSeconds = 0;
   TimeType       stJulianDate;
   WE_UINT32      uiMsgId = 0;

   WE_CHAR         *pcPath = NULL;
   St_CBEncodeData *pstCBData = NULL;
   WE_CHAR          *pcTmp = NULL;

   EME_UTILS_LOG_INFO(("EME_FSMSMTP_SendReceipt---: Start"));
   if (pstTmHandle == NULL || pstSig == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_FSMSMTP_SendReceipt--: Bad parameter and return"));
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      EME_UTILS_MEMSET(&stJulianDate, 0, sizeof(stJulianDate));
      EME_UTILS_MEMSET(&stReceiptOpts, 0, EME_SMTP_SEND_RECEIPT_OPT_NUM * sizeof(EmailOpt));
      
      pstMailInfo = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_RawMailInfo));
      EME_UTILS_BREAKNULL(pstMailInfo, iRet, EME_NOMEMORY, "EME_FSMSMTP_SendReceipt--: no memory for raw mail info");
      EME_UTILS_MEMSET(pstMailInfo, 0, sizeof(St_RawMailInfo));


      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMSMTP_SendReceipt--:  transEvt is null");
      pstMailInfo->uiTranType = pstTransEvt->uiTransType;
      pstMailInfo->pcUID = NULL;
      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_SMTP_SEND_RECEIPT;
      
      /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
      /*                          prepare data                              */
      /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
      /* get smtp handle */
      pvSMTP = pstTmHandle->pvSMPT;
      EME_UTILS_BREAKNULL(pvSMTP, iRet, EME_FAILED, "EME_FSMSMTP_SendReceipt--:  smtp handle is null");

//      pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
//      EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_FSMSMTP_SendReceipt--:  file mgr is null");

      /* get message id */
      iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendReceipt--:  get message id failed");
      EME_UTILS_BREAKNULL(pcMsgId, iRet, EME_FAILED, "EME_FSMSMTP_SendReceipt--:  get message id failed");

      uiMsgId = EME_UTILS_ATOX(pcMsgId);

      /* init store handle */
      iRet = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendReceipt--:  create store handle failed");
      EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_FSMSMTP_SendReceipt--:  create store handle failed");

      /* init codec handle */
      iRet = EMECodecLib_InitHandleEx(&pvCodec, pvMemHdr, pvFileMgr, OEMTMLib_GetHCSC(pstTmHandle), pstTmHandle->pvAsyncMgr);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendReceipt--:  create codec handle failed");
      EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_FSMSMTP_SendReceipt--:  create codec handle failed");

      /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
      /*                          add header                                */
      /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
      /* get message header */
      iRet = EmailStore_GetMsgHeaders(pvStore, uiMsgId, &pstMsgOpts);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendReceipt--:  get message header failed");
      EME_UTILS_BREAKNULL(pstMsgOpts, iRet, EME_FAILED, "EME_FSMSMTP_SendReceipt--:  get message header failed");

      /* get "from" address for setting "to" field. mail's "from" field is the receipt's "to" field */
      pcTmp = NULL;
      iRet = EME_GetStrOptValue((EMEOpt*)pstMsgOpts, EMAIL_OPT_FROM, (WE_CHAR**)&pcTmp);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendReceipt--:  get from failed");
      EME_UTILS_BREAKNULL(pcTmp, iRet, EME_FAILED, "EME_FSMSMTP_SendReceipt--:  get from failed");
      pstMailInfo->pcTo = EME_UTILS_STRDUP(pvMemHdr, (char*)pcTmp);
      EME_UTILS_BREAKNULL(pstMailInfo->pcTo, iRet, EME_NOMEMORY, "EME_FSMSMTP_SendReceipt--:  no memory for to address");

      iRet = EME_GetStrOptValue((EMEOpt*)pstMsgOpts, EMAIL_OPT_SUBJECT, &pcSubject);
      if (pcSubject == NULL)
      {
         pcSubject = "";
      }
//       EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendReceipt, get subject failed");
//       EME_UTILS_BREAKNULL(pcSubject, iRet, EME_FAILED, "EME_FSMSMTP_SendReceipt, get subject failed");

      /* get send address for setting "from" field. User's send address is receipt's from field */
      pcTmp = NULL;
      iRet = EME_GetStrOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_SEND_EMAIL_ADDR, (WE_CHAR**)&pcTmp);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendReceipt--:  get receive server addr failed");
      EME_UTILS_BREAKNULL(pcTmp, iRet, EME_FAILED, "EME_FSMSMTP_SendReceipt--:  get receive server addr failed");
      pstMailInfo->pcFrom = EME_UTILS_STRDUP(pvMemHdr, (char*)pcTmp);
      EME_UTILS_BREAKNULL(pstMailInfo->pcFrom, iRet, EME_NOMEMORY, "EME_FSMSMTP_SendReceipt--: no memory for from address");

      uiLocalSeconds = EME_UTILS_GETLOCALSECONDS ;//EME_UTILS_GETTIMEMS;
      EME_UTILS_LOG_INFO(("EME_FSMSMTP_SendReceipt: uiLocalSeconds = %d", uiLocalSeconds));
      iTimeZone = EME_UTILS_GETLOCALOFFSET;
      EME_UTILS_LOG_INFO(("EME_FSMSMTP_SendReceipt: iTimeZone = %d", iTimeZone));
      stReceiptOpts[0].iId = EMAIL_OPT_FROM;
      stReceiptOpts[0].pVal = (WE_VOID*)pstMailInfo->pcFrom;
      stReceiptOpts[1].iId = EMAIL_OPT_TO;
      stReceiptOpts[1].pVal = (WE_VOID*)pstMailInfo->pcTo;
      stReceiptOpts[2].iId = EMAIL_OPT_DATE;
      stReceiptOpts[2].pVal = (WE_VOID*)uiLocalSeconds;
      stReceiptOpts[3].iId = EMAIL_OPT_TIME_ZONE;
      stReceiptOpts[3].pVal = (WE_VOID*)iTimeZone;
      stReceiptOpts[4].iId = EMAIL_OPT_SUBJECT;
      stReceiptOpts[4].pVal = (WE_VOID*)"Read receipt";
      stReceiptOpts[5].iId = EMAIL_OPT_REPLY_TO;
      stReceiptOpts[5].pVal = (WE_VOID*)pstMailInfo->pcFrom;
      stReceiptOpts[6].iId = EMAILOPT_END;

      /* add message header to codec */
      iRet = EMECodecLib_AddHeader(pvCodec, stReceiptOpts);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendReceipt--:  add message header failed");

      /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
      /*                          add text                                  */
      /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
      /* get and add message parts */
      uiSize = EME_UTILS_STRLEN(EME_FSMSMTP_RECEIPT_TEXT) + 
               EME_UTILS_STRLEN(pstMailInfo->pcFrom) + 
               EME_UTILS_STRLEN(pcSubject) + 
               EME_SMTP_MAX_DATE_LEN +
               1;
      pcText = EME_UTILS_MALLOC(pvMemHdr, uiSize);
      EME_UTILS_BREAKNULL(pcText, iRet, EME_FAILED, "EME_FSMSMTP_SendReceipt--:  malloc text failed");
      EME_UTILS_MEMSET(pcText, 0, uiSize);

      pcDisplayAddr = pstMailInfo->pcFrom;
//      EME_UTILS_GETJULIANDATE(uiLocalSeconds, &stJulianDate);
      TF3RD_CURRENTTIME((unsigned short*)&stJulianDate.nYear, (unsigned char*) &stJulianDate.nMonth, (unsigned char*)&stJulianDate.nDay,
        (unsigned char*)&stJulianDate.nHour, (unsigned char*)&stJulianDate.nMin, (unsigned char*)&stJulianDate.nSec);
      EME_UTILS_SNPRINTF(pcText, 
                        uiSize, 
                        EME_FSMSMTP_RECEIPT_TEXT, 
                        pcDisplayAddr,
                        pcSubject, 
                        stJulianDate.nYear,
                        stJulianDate.nMonth, 
                        stJulianDate.nDay,
                        stJulianDate.nHour,
                        stJulianDate.nMin, 
                        stJulianDate.nSec);

      stReceiptOpts[0].iId = EMAILPART_OPT_PART_TYPE;
      stReceiptOpts[0].pVal = (void*)EMAILPART_PARTTYPE_TEXTPLAIN;
      stReceiptOpts[1].iId = EMAILPART_OPT_DATA_BUFF;
      stReceiptOpts[1].pVal = (void*)pcText;
      stReceiptOpts[2].iId = EMAILPART_OPT_DATA_SIZE;
      stReceiptOpts[2].pVal = (void*)EME_UTILS_STRLEN(pcText);
      stReceiptOpts[3].iId = EMAILPART_OPT_SRC_TYPE;
      stReceiptOpts[3].pVal = (void*)EMAILPART_SRCTYPE_BUFF;
      stReceiptOpts[4].iId = EMAILOPT_END;

      iRet = EMECodecLib_AddPart(pvCodec, stReceiptOpts);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendReceipt--:  add message part failed");
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendReceipt--: get or add message part failed");

/*
      iRet = EmailStore_GetMsgPath(pvStore, uiMsgId, (WE_CHAR**)&pcPath);
      EME_UTILS_BREAKNULL(pcPath, iRet, EME_FAILED, "EME_FSMSMTP_SendReceipt--: path is null");
*/
      pcPath = EMEFS_TEMP_ROOT_DIR;

      iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_SIZE_LIMIT, (WE_INT32*)&uiSizeLimit);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendReceipt--:  get size limit failed");

      pstCBData = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_CBEncodeData));
      EME_UTILS_BREAKNULL(pstCBData, iRet, EME_NOMEMORY, "EME_FSMSMTP_SendReceipt--: no memory for encode data");
      EME_UTILS_MEMSET(pstCBData, 0, sizeof(St_CBEncodeData));

      pstCBData->pvTMHandle  = pstTmHandle;
      pstCBData->pvTranEvent = pstTransEvt;
      pstCBData->pvUserData  = (void*)pstMailInfo;
      pstCBData->uiSizeLimit = uiSizeLimit;
      pstCBData->pvCodec     = pvCodec;
      
      /*encode email*/
      iRet = EMECodecLib_EncodingEx(pvCodec, pcPath, pstCBData, (PFNENCODINGCB )EME_FSMSMTP_EncodingMsgCB, NULL);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_SendReceipt--:  encoding message failed");
      pstTransEvt->pfnFreeSigData = (PFNFREESIGDATA)EME_FSMSMTP_pfnFree;
      pstTransEvt->pvTransData    = pstCBData;

//      iRet = EMECodecLib_EncodingEx(pvCodec, NULL, pstCBData, NULL, &pstCodecOpt);
//      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, encoding message failed");
//      (void)EME_FSMSMTP_EncodingMsgCB(pvCodec, pstCodecOpt, pstCBData, EME_SUCCESS);

      pcPath = NULL;
   } while(0);
   
   EME_UTILS_FREEIF(pvMemHdr, pcText);
   EmailStore_DeleteHandle(pvStore);
   pvStore = NULL;
   
   if (iRet != EME_SUCCESS)
   {  
      EME_UTILS_FREEIF(pvMemHdr, pstCBData);
      EMECodecLib_DeleteHandle(&pvCodec);
      EmailStore_DeleteHandle(pvStore);
      pvStore = NULL;

      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_TRANS_END, iRet, pstTransEvt);
   }

   EME_UTILS_LOG_INFO(("EME_FSMSMTP_SendReceipt--: End, Result = %d", iRet));
   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMSMTP_Close   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if smtp need close ,quit smtp server,else trans fsm to TRANS_END with result of success
 *              3. if there is something wrong,trans fsm to TRANS_END with result of failed
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMSMTP_Close(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_VOID     *pvMemHdr = NULL;
   St_TransEvt *pstTransEvt = NULL;
   ISMTPHandle *pvSMTP = NULL;

   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMSMTP_Close, transEvt is null");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_SMTP_CLOSE;

      /* get smtp handle */
      pvSMTP = pstTmHandle->pvSMPT;
      EME_UTILS_BREAKNULL(pvSMTP, iRet, EME_FAILED, "EME_FSMSMTP_Close, smtp handle is null");
      
      if (EME_SMTPCloseNeed(pstTmHandle, pstTransEvt))
      {
         /* record smtp callback data */
         pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;

         iRet = EME_SMTP_Close(pvSMTP);
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Close, failed");
      }
      else
      {
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_TRANS_END, EME_SUCCESS, pstTransEvt);
      }

   }while(0);

   if (iRet != EME_SUCCESS)
   {  
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_TRANS_END, iRet, pstTransEvt);
   }
   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMPOP_TransEnd   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if trans result is failed,cancel smtp operation
                3. delete the transaction from trans queue ,
                4. reply this result of trans to app
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMSMTP_TransEnd(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   WE_INT32       iTransResult = EME_SUCCESS;
   WE_UINT32      uiTransID = 0;
   WE_UINT32      uiTransType = 0;
   WE_UINT32      uiClsId = 0;
   WE_CHAR        *pcMsgId = NULL;
   WE_UINT32      uiMsgId = 0;

   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMSMTP_TransEnd, transEvt is null");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_SMTP_TRANS_END;

      /* get trans result */
      if(pstSig->iParam != EME_SUCCESS)
      {
         iTransResult = pstSig->iParam;
      }
      else
      {
         iTransResult = pstTransEvt->iErrCode;
      }

      if (iTransResult != EME_SUCCESS)
      {
         EME_UTILS_LOG_ERROR(("TTTTTTTTTTTTTTTTTTTTransaction result:%x", iTransResult));
         EME_SMTP_Cancel(pstTmHandle->pvSMPT);
         pstTmHandle->iSMPTState = EME_STACK_STATE_LOGOUT;
      }

      uiTransID = pstTransEvt->uiTransID;
      uiTransType = pstTransEvt->uiTransType;
//      iRet = EME_GetIntOptValue(pstTransEvt->pstTransOpts, EME_OPTID_REPLY_CLSID, (WE_INT32*)&uiClsId);
//      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_TransEnd, get cls id failed");

      iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
      if(iRet == EME_SUCCESS)
      {
         uiMsgId = EME_UTILS_ATOX(pcMsgId);
      }
      iRet = EME_SUCCESS;


      EME_DeleteTransaction(pstTmHandle, pstTransEvt);

     
      EME_UTILS_LOG_INFO(("EME_FSMSMTP_TransEnd: pstTmHandle->iSMPTState = %d",  pstTmHandle->iSMPTState));

      /*Remove temp folder if exist*/
      EME_RemoveDir(EME_OEMTMLib_GetIFileMgr(pstTmHandle), EMEFS_TEMP_ROOT_DIR);

     /*
	if the state is EME_STACK_STATE_LOGOUT,need to disconnect the dail-------------just for plant of MTK
	*/  
/*	if(EME_STACK_STATE_LOGOUT == pstTmHandle->iSMPTState)
      {
             WE_INT32   nDialId = 0;
             EME_GetDialId(&nDialId);
	      EME_UTILS_LOG_INFO(("EME_FSMSMTP_TransEnd: EME_GetDialId nDailid = %d", nDialId)); 
             EME_DisconnectPPP(nDialId);
	      EME_UTILS_LOG_INFO(("EME_FSMSMTP_TransEnd: EME_DisconnectPPP END!"));
	      EME_FreePPPData();
             EME_UTILS_LOG_INFO(("EME_FSMPOP_TransEnd: EME_FreePPPData END!"));	 
      }*/
	  

      iRet = OEMTMLib_TransReply(pstTmHandle, uiTransID, uiTransType, uiMsgId, uiClsId, iTransResult, pstTmHandle->acRespDesc);

      //pstTmHandle->iCanBeCancel = EME_CANCEL_ENABLED;

   } while(0);

   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMSMTP_Cancel   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. cancel smtp operation & update smtp status
                3. delete the transaction from trans queue ,
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMSMTP_Cancel(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   ISMTPHandle    pvSMTP = NULL;

   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMSMTP_Cancel, transEvt is null");

      pvSMTP = pstTransEvt->pvTransData;
      EME_UTILS_BREAKNULL(pvSMTP, iRet, EME_FAILED, "EME_FSMSMTP_Cancel, smtp handle is null");

      EME_SMTP_Cancel(pvSMTP);
   	
      /* set smtp state */
      pstTmHandle->iSMPTState = EME_STACK_STATE_LOGOUT;

      if (NULL != pstTransEvt->pfnFreeSigData)
      {
         pstTransEvt->pfnFreeSigData(pstTmHandle, pstTransEvt);
      }
      
      /************************ reply to app the result ****************************/
      /************************ need to be added ****************************/
      /************************ reply to app the result ****************************/   
      
      EME_DeleteTransaction(pstTmHandle, pstTransEvt);

   } while(0);


   return iRet;
}




/*************************************************************************************************************/
/* Callback Functions */
/*************************************************************************************************************/
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_SMTPActionCB   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. deal with the evt cb from pop stack 
 *              3. if there is something wrong,trans fsm to TRANS_END
 *  
 * ===================================================================================================*/
WE_VOID EME_SMTPActionCB(WE_VOID* pvTmHandle,  St_SMTPActRet* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;
   St_TransEvt *pstTransEvt = NULL;
   St_TMHandle *pstTmHandle = (St_TMHandle*)pvTmHandle;

   if (pstTmHandle == NULL)
   {
      return;
   }
   
   do 
   {
      pstTransEvt = (St_TransEvt*)pstTmHandle->pvCbkUserData;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_SMTPActionCB transevt is null");

      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_SMTPActionCB actret is null");

      /*copy response description*/
      EME_UTILS_MEMSET(pstTmHandle->acRespDesc, 0, 512);
      if(pstActRet->iRet != EME_SUCCESS && EME_UTILS_STRLEN(pstActRet->acRespDesc) > 0)
         EME_UTILS_MEMCPY(pstTmHandle->acRespDesc, pstActRet->acRespDesc, EME_UTILS_STRLEN(pstActRet->acRespDesc));
      EME_UTILS_MEMSET(pstActRet->acRespDesc, 0, 512);

      switch(pstActRet->uiActType)
      {
      case SMTP_ACTTYPE_OPEN:
         iRet = EME_CBSMTP_Open(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case SMTP_ACTTYPE_SENDING:
         iRet = EME_CBSMTP_Sending(pstTmHandle, pstTransEvt, pstActRet);
         break;      
      case SMTP_ACTTYPE_SENDOVER:
         iRet = EME_CBSMTP_Send(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case SMTP_ACTTYPE_SENDREADY:
         iRet = pstActRet->iRet;
         break;
      case SMTP_ACTTYPE_FORWARDING:
         iRet = EME_CBSMTP_Forwarding(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case SMTP_ACTTYPE_FORWARDOVER:
         iRet = EME_CBSMTP_Forward(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case SMTP_ACTTYPE_FORWARDREADY:
         iRet = pstActRet->iRet;
         break;
      case SMTP_ACTTYPE_CLOSE:
         iRet = EME_CBSMTP_Close(pstTmHandle, pstTransEvt, pstActRet);
         break;
      
      case SMTP_ACTTYPE_NOTIFY:
         if (pstActRet->iRet == EME_SOCKET_SHUTDOWN)
         {
            pstTmHandle->iSMPTState = EME_STACK_STATE_LOGOUT;
            iRet = EME_FAILED;
         }
         break;
//       case SMTP_ACTTYPE_CANCEL:
//          iRet = EME_CBSMTP_Cancel(pstTmHandle, pstTransEvt, pstActRet);
//          break;
      
      default:
         iRet = pstActRet->iRet;
         break;
      }

   } while(0);

   /* iRet is the cb function operation result, isn't the STACK action result.
      The STACK action result is handed in cb function */
   if (iRet != EME_SUCCESS)
   {
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_TRANS_END, iRet, pstTransEvt);
   }
   
   return;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBSMTP_Open   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of connect to smtp server from stack is success, 
                     trans fsm to send or send receipt fsm
 *              3. else trans fsm to TRANS_END with result of failed
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBSMTP_Open(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_SMTPActRet* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_UINT32   uiSigId = 0;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBSMTP_Open actret is null");
      
      if (pstActRet->iRet == EME_SUCCESS)
      {
         /* set smtp state */
         pstTmHandle->iSMPTState = EME_STACK_STATE_LOGIN;

         switch(pstTransEvt->uiTransType)
         {
         case EME_TRANS_SMTP_SEND_MAIL:
            uiSigId = EME_SIG_SMTP_SEND;
            break;
         case EME_TRANS_SMTP_SEND_RECEIPT:
            uiSigId = EME_SIG_SMTP_SEND_RECEIPT;
            break;
         case EME_TRANS_SMTP_FORWARD_ATTACHMENT:
         case EME_TRANS_SMTP_FORWARD_WHOLEMAIL:
            uiSigId = EME_SIG_SMTP_FORWARD;
            break;
         default:
            iRet = EME_FAILED;
            break;
         }
         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_SMTP, uiSigId, (WE_VOID*)pstTransEvt);
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_TRANS_END, pstActRet->iRet, pstTransEvt);
      }

   } while(0);

   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBSMTP_Sending   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. notify the size have sent to app
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBSMTP_Sending(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_SMTPActRet* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBSMTP_Sending actret is null");

      if (pstActRet->iRet == EME_SUCCESS)
      {
         if(pstActRet->stDataSent.uiSentData <= pstActRet->stDataSent.uiTotalData)
         {
            iRet = OEMTMLib_NotifySize(pstTmHandle, 
               pstTransEvt, 
               pstActRet->stDataSent.uiSentData, 
               pstActRet->stDataSent.uiTotalData);
         }
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_TRANS_END, pstActRet->iRet, pstTransEvt);
      }

   } while(0);

   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBSMTP_Sending   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. notify the size have sent to app
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBSMTP_Forwarding(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_SMTPActRet* pstActRet)
{
   return EME_CBSMTP_Sending(pstTmHandle, pstTransEvt, pstActRet);
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBSMTP_Send   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of send email from stack is success, 
                     trans fsm to SMTP_CLOSE fsm
 *              3. else trans fsm to TRANS_END with result of failed
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBSMTP_Send(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_SMTPActRet* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBSMTP_Send actret is null");
      
      if (pstActRet->iRet == EME_SUCCESS)
      {
         pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;
         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_CLOSE, pstTransEvt);
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_TRANS_END, pstActRet->iRet, pstTransEvt);
      }

   } while(0);

   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBSMTP_Send   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of send email from stack is success, 
                     trans fsm to SMTP_CLOSE fsm
 *              3. else trans fsm to TRANS_END with result of failed
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBSMTP_Forward(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_SMTPActRet* pstActRet)
{
   return EME_CBSMTP_Send(pstTmHandle, pstTransEvt, pstActRet);
}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBSMTP_Close   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of quit from smtp server from stack is success, 
                     update smtp state to logout
                     trans fsm to TRANS_END with result of success
 *              3. else trans fsm to TRANS_END with result of failed
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBSMTP_Close(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_SMTPActRet* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBSMTP_Close actret is null");
      
      if (pstActRet->iRet == EME_SUCCESS)
      {
         pstTmHandle->iSMPTState = EME_STACK_STATE_LOGOUT;

         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_TRANS_END, EME_SUCCESS, pstTransEvt);
      }
      else
      {
         /* gmail smtp 'quit' always failed, so change here temporarily */
         EME_SMTP_Cancel(pstTmHandle->pvSMPT);
         pstTmHandle->iSMPTState = EME_STACK_STATE_LOGOUT;

         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_TRANS_END, EME_SUCCESS, pstTransEvt);
         
//          if (pstTransEvt != NULL)
//          {
//             pstTransEvt->iErrCode = EME_FAILED;
//          }
//          EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_TRANS_END, EME_FAILED, pstTransEvt);
      }

   } while(0);

   return iRet;
}


/*************************************************************************************************************/
/* Free Functions */
/*************************************************************************************************************/
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMSMTP_Send   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. get email from store by msg id and encode it
                3. check the email encode data size whether longer than the max length
                4. send the encode email data
 *              5. if there is something wrong,trans fsm to TRANS_END
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMSMTP_Forward(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   ISMTPHandle    pvSMTP = NULL;
   WE_VOID        *pvFileMgr = NULL;
   WE_CHAR        *pcMsgId = NULL;
   St_RawMailInfo *pstMailInfo = NULL;
   HEmailStore    pvStore = NULL;
   WE_UINT32      uiPartNum = 0;
   IEMSGHandle    pvCodec = NULL;
   EmailOpt       *pstMsgOpts = NULL;
   WE_UINT32       i = 0;
//   EmailOpt       *pstCodecOpt = NULL;
   WE_UINT32      uiSizeLimit = 0;
   WE_UINT32      uiMessageSize = 0;
   WE_CHAR        *pcTmp = NULL;
   WE_INT32       iSrcType = 0;

   WE_UINT32      uiMsgId = 0;
   WE_CHAR        *pcPath = NULL;
   St_CBEncodeData *pstCBData =NULL;

   EME_UTILS_LOG_INFO(("EME_FSMSMTP_Forward---: Start"));
   if (pstTmHandle == NULL || pstSig == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_FSMSMTP_Forward--: Bad parameter and return"));
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      /* check smtp state */
      if (pstTmHandle->iSMPTState != EME_STACK_STATE_LOGIN)
      {
         iRet = EME_FAILED;
         break;
      }

      pstMailInfo = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_RawMailInfo));
      EME_UTILS_BREAKNULL(pstMailInfo, iRet, EME_NOMEMORY, "EME_FSMSMTP_Forward--: no memory for raw mail info");
      EME_UTILS_MEMSET(pstMailInfo, 0, sizeof(St_RawMailInfo));

      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMSMTP_Forward--: transEvt is null");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_SMTP_SEND;

      /* get smtp handle */
      pvSMTP = pstTmHandle->pvSMPT;
      EME_UTILS_BREAKNULL(pvSMTP, iRet, EME_FAILED, "EME_FSMSMTP_Forward--:  smtp handle is null");

//      pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
//      EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_FSMSMTP_Forward--:  file mgr is null");

      /* get message id */
      iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Forward--:  get message id failed");
      EME_UTILS_BREAKNULL(pcMsgId, iRet, EME_FAILED, "EME_FSMSMTP_Forward--:  get message id failed");

      uiMsgId = EME_UTILS_ATOX(pcMsgId);

      iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_UID, &pcTmp);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Forward--:  get message id failed");
      EME_UTILS_BREAKNULL(pcTmp, iRet, EME_FAILED, "EME_FSMSMTP_Forward--:  get message id failed");
      pstMailInfo->pcUID = EME_UTILS_STRDUP(pvMemHdr, pcTmp);
      EME_UTILS_BREAKNULL(pstMailInfo->pcUID, iRet, EME_NOMEMORY, "no memory for uid");
      
      iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_SIZE_LIMIT, (WE_INT32*)&uiSizeLimit);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Forward--: get message id failed");

      /* init store handle */
      iRet = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Forward--: create store handle failed");
      EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_FSMSMTP_Send, create store handle failed");

      /* get message header */
      iRet = EmailStore_GetMsgHeaders(pvStore, uiMsgId, &pstMsgOpts);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Forward--: get message header failed");
      EME_UTILS_BREAKNULL(pstMsgOpts, iRet, EME_FAILED, "EME_FSMSMTP_Forward--: get message header failed");
      
      /* get from */
      pcTmp = NULL;
      iRet = EME_GetStrOptValue((EMEOpt*)pstMsgOpts, EMAIL_OPT_FROM, &pcTmp);
      EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Forward--: get from failed");
      EME_UTILS_BREAKNULL(pcTmp, iRet, EME_FAILED, "EME_FSMSMTP_Forward--: get from failed");
      pstMailInfo->pcFrom = EME_UTILS_STRDUP(pvMemHdr, pcTmp);
      EME_UTILS_BREAKNULL(pstMailInfo->pcFrom, iRet, EME_NOMEMORY, "EME_FSMSMTP_Forward--: no memory for from");

      /* get "to", "cc", "bcc", either one will work */
      pcTmp = NULL;
      EME_GetStrOptValue((EMEOpt*)pstMsgOpts, EMAIL_OPT_TO, &pcTmp);
      if(NULL != pcTmp)
      {
         pstMailInfo->pcTo = EME_UTILS_STRDUP(pvMemHdr, pcTmp);
         EME_UTILS_BREAKNULL(pstMailInfo->pcTo, iRet, EME_NOMEMORY, "EME_FSMSMTP_Forward--: no memory for to ");
      }
      pcTmp = NULL;
      EME_GetStrOptValue((EMEOpt*)pstMsgOpts, EMAIL_OPT_CC, &pcTmp);
      if(NULL != pcTmp)
      {
         pstMailInfo->pcCc = EME_UTILS_STRDUP(pvMemHdr, pcTmp);
         EME_UTILS_BREAKNULL(pstMailInfo->pcCc, iRet, EME_NOMEMORY, "EME_FSMSMTP_Forward--: no memory for cc ");
      }
      pcTmp = NULL;
      EME_GetStrOptValue((EMEOpt*)pstMsgOpts, EMAIL_OPT_BCC, &pcTmp);
      if(NULL != pcTmp)
      {
         pstMailInfo->pcBcc = EME_UTILS_STRDUP(pvMemHdr, pcTmp);
         EME_UTILS_BREAKNULL(pstMailInfo->pcBcc, iRet, EME_NOMEMORY, "EME_FSMSMTP_Forward--: no memory for bcc ");
      }
      if (NULL == pstMailInfo->pcTo &&
         NULL == pstMailInfo->pcCc &&
         NULL == pstMailInfo->pcBcc)
      {
         iRet = EME_FAILED;
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Forward--: to, cc, or bcc is null");
      }
      /*set transaction type*/
      pstMailInfo->uiTranType = pstTransEvt->uiTransType;

      if(pstTransEvt->uiTransType == EME_TRANS_SMTP_FORWARD_WHOLEMAIL)
      {
         pstMailInfo->pcRawMail = NULL;
         pstMailInfo->uiSrcType = SMTP_RAWMAIL_SRCTYPE_NONE;

         iRet = EME_SMTP_Forward(pvSMTP, pstMailInfo);
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Forward--:  send message failed");

         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcBcc);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcCc);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcFrom);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcRawMail);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcTo);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcUID);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo);
      }
      else if(pstTransEvt->uiTransType == EME_TRANS_SMTP_FORWARD_ATTACHMENT)
      {
         /* init codec handle */
         iRet = EMECodecLib_InitHandleEx(&pvCodec, pvMemHdr, pvFileMgr, OEMTMLib_GetHCSC(pstTmHandle), pstTmHandle->pvAsyncMgr);
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Forward--: create codec handle failed");
         EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_FSMSMTP_Send, create codec handle failed");
         
         /* get part count */
         iRet = EmailStore_GetMsgPartCount(pvStore, uiMsgId, &uiPartNum);
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Forward--: get message part num failed");
         
         /* add message header to codec */
         iRet = EMECodecLib_AddHeader(pvCodec, pstMsgOpts);
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Forward--: add message header failed");
         
         /* get and add message parts */
         for (i = 0; i < uiPartNum; i++)
         {
            pstMsgOpts = NULL;
            
            iRet = EmailStore_GetMsgPart(pvStore, uiMsgId, i, &pstMsgOpts);
            EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Forward--: get message part failed");
            EME_UTILS_BREAKNULL(pstMsgOpts, iRet, EME_FAILED, "EME_FSMSMTP_Forward--: get message part failed");
            
            iRet = EME_GetStrOptValue((EMEOpt*)pstMsgOpts, EMAILPART_OPT_CONTENT_ID, &pcTmp);
            if (iRet == EME_SUCCESS && pcTmp != NULL)
            {
                EME_UTILS_LOG_INFO(("EME_FSMSMTP_Forward---: skip server attachment %s", pcTmp));
                continue;
            }

            iRet = EME_GetIntOptValue((EMEOpt*)pstMsgOpts, EMAILPART_OPT_SRC_TYPE, &iSrcType);
            iRet += EME_GetStrOptValue((EMEOpt*)pstMsgOpts, EMAILPART_OPT_DATA_BUFF, &pcTmp);
            if (iRet == EME_SUCCESS && pcTmp != NULL && iSrcType == EMAILPART_SRCTYPE_BUFF)
            {
               uiMessageSize += EME_UTILS_BASE64LEN(EME_UTILS_STRLEN(pcTmp));
               if (uiMessageSize > (uiSizeLimit * 4 / 3) *1024)
               {
                  iRet = EME_SIZE_LIMIT_EXCEED;
                  EME_UTILS_BREAK(iRet, "EME_FSMSMTP_Forward--: size limited");
               }
            }
            else if (iRet == EME_SUCCESS && pcTmp != NULL && iSrcType == EMAILPART_SRCTYPE_FILE)
            {
               FILE_HANDLE  hFile = NULL;
               
               hFile = EME_FileOpen(pvFileMgr, pcTmp, EME_OFM_READ);
               if (hFile != NULL)
               {
                  uiMessageSize += EME_UTILS_BASE64LEN(EME_FileLength(hFile));
                  if (uiMessageSize > (uiSizeLimit * 4 / 3) *1024)
                  {
                     iRet = EME_SIZE_LIMIT_EXCEED;
                     EME_FileClose(hFile);
                     EME_UTILS_BREAK(iRet, "EME_FSMSMTP_Forward--: size limited");
                  }
               }
               EME_FileClose(hFile);
            }
            
            //iRet = EMECodecLib_AddPart(pvCodec, pstMsgOpts);
            iRet = EMECodecLib_AddPartEx(pvCodec, pstMsgOpts);
            EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Forward--: add message part failed");
         }
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Forward--: get or add message part failed");

         iRet = EmailStore_GetMsgPath(pvStore, uiMsgId, &pcPath);
         EME_UTILS_BREAKNULL(pcPath, iRet, EME_FAILED, "path is null");

         pstCBData = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_CBEncodeData));
         EME_UTILS_BREAKNULL(pstCBData, iRet, EME_NOMEMORY, "EME_FSMSMTP_Forward--: no memory for encode data");
         EME_UTILS_MEMSET(pstCBData, 0, sizeof(St_CBEncodeData));
         
         pstCBData->pvTMHandle  = pstTmHandle;
         pstCBData->pvTranEvent = pstTransEvt;
         pstCBData->pvUserData  = (void*)pstMailInfo;
         pstCBData->uiSizeLimit = uiSizeLimit;
         pstCBData->pvCodec     = pvCodec;

         /*encode email*/
         iRet = EMECodecLib_EncodingEx(pvCodec, pcPath, pstCBData, (PFNENCODINGCB)EME_FSMSMTP_EncodingMsgCB, NULL);
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Forward--: encoding message failed");
         pstTransEvt->pfnFreeSigData = (PFNFREESIGDATA)EME_FSMSMTP_pfnFree;
         pstTransEvt->pvTransData    = pstCBData;
         
//         iRet = EMECodecLib_EncodingEx(pvCodec, NULL, pstCBData, NULL, &pstCodecOpt);
//         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, encoding message failed");
//         (void)EME_FSMSMTP_EncodingMsgCB(pvCodec, pstCodecOpt, pstCBData, EME_SUCCESS);
      }
      else
      {
         iRet = EME_FAILED;
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Forward--: Don't supported type");
      }

      /* record smtp callback data */
      pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;

      /* register free function */
      pstTransEvt->pfnFreeSigData = NULL;

   } while(0);

   EmailStore_DeleteHandle(pvStore);
   pvStore = NULL;

   if (iRet != EME_SUCCESS)
   {
      EME_UTILS_FREEIF(pvMemHdr, pstCBData);
      EMECodecLib_DeleteHandle(&pvCodec);
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }

      if(pstMailInfo != NULL)
      {
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcBcc);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcCc);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcFrom);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcRawMail);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcTo);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcUID);
         EME_UTILS_FREEIF(pvMemHdr, pstMailInfo);
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_TRANS_END, iRet, pstTransEvt);
   }

   EME_UTILS_LOG_INFO(("EME_FSMSMTP_Forward---: End, Result = %d", iRet));
   return iRet;
}


static  void EME_FSMSMTP_EncodingMsgCB(IEMSGHandle hEmailHandle, EmailOpt *pstOpts, void *pvUserData, int iResult)
{
   WE_INT32        iRet = EME_SUCCESS;
   EmailOpt        *pstCodecOpt = NULL;
   St_RawMailInfo  *pstMailInfo = NULL;
   St_TMHandle     *pstTmHandle = NULL;
   St_CBEncodeData *pstCBData = (St_CBEncodeData*)pvUserData;
   St_TransEvt     *pstTransEvt = NULL;
   WE_UINT32       uiMessageSize = 0;
   WE_UINT32       uiSizeLimit = 0;
   WE_CHAR         *pcTmp = NULL;
   
   void            *pvMemMgr = NULL;

   EME_UTILS_LOG_INFO(("EME_FSMSMTP_EncodingMsgCB---: Start"));
   do 
   {
      EME_UTILS_BREAKNULL(hEmailHandle, iRet, EME_INVALID_DATA, "EME_FSMSMTP_EncodingMsgCB--,email msg is null");
      EME_UTILS_BREAKNULL(pstCBData, iRet, EME_INVALID_DATA, "EME_FSMSMTP_EncodingMsgCB--, user data is null");

      pstTransEvt = (St_TransEvt*)pstCBData->pvTranEvent;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMSMTP_EncodingMsgCB--,tranevent is null");
      pstTransEvt->pfnFreeSigData = NULL;
      pstTransEvt->pvTransData    = NULL;

      pstTmHandle = (St_TMHandle*)pstCBData->pvTMHandle;
      EME_UTILS_BREAKNULL(pstTmHandle, iRet, EME_FAILED, "EME_FSMSMTP_EncodingMsgCB--,tm handle is null");
      
      pvMemMgr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

      uiSizeLimit = (WE_UINT32)pstCBData->uiSizeLimit;

      if(EME_SUCCESS == iResult)
      {
         pstCodecOpt = pstOpts;
         pstMailInfo = (St_RawMailInfo*)pstCBData->pvUserData;
         EME_UTILS_BREAKNULL(pstMailInfo, iRet, EME_FAILED, "EME_FSMSMTP_EncodingMsgCB--,raw mail info is null");
         
         iRet = EME_GetStrOptValue((EMEOpt*)pstCodecOpt, EMAILMESSAGE_OPT_BUFF, &pcTmp);
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_EncodingMsgCB--, get buffer opt failed");
         EME_UTILS_BREAKNULL(pcTmp, iRet, EME_FAILED, "EME_FSMSMTP_Send, encoding message failed");

         pstMailInfo->pcRawMail = EME_UTILS_STRDUP(pvMemMgr, pcTmp);
         EME_UTILS_BREAKNULL(pstMailInfo->pcRawMail, iRet, EME_NOMEMORY, "no memory for raw mail");

         /*check if the size of email is longer than the max length*/
         iRet = EME_GetIntOptValue((EMEOpt*)pstCodecOpt, EMIALMESSAGE_OPT_BUFFER_SIZE, (WE_INT32*)&uiMessageSize);
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_EncodingMsgCB--, get message buffer size failed");
         
         if (uiMessageSize > (uiSizeLimit * 4 / 3 ) * 1024)
         {
            iRet = EME_SIZE_LIMIT_EXCEED;
            EME_UTILS_BREAK(iRet, "EME_FSMSMTP_EncodingMsgCB--, size limited");
         }
         
         pstMailInfo->uiSrcType = SMTP_RAWMAIL_SRCTYPE_FILE;

         if(pstTransEvt->uiTransType == EME_TRANS_SMTP_SEND_MAIL)
         {
            /* check smtp state */
            if (pstTmHandle->iSMPTState != EME_STACK_STATE_LOGIN)
            {
               iRet = EME_FAILED;
               break;
            }
            
            /* record smtp callback data */
            pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;
            
            iRet = EME_SMTP_Send(pstTmHandle->pvSMPT, pstMailInfo);
            EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_EncodingMsgCB--, send message failed");
         }
         else if(pstTransEvt->uiTransType == EME_TRANS_SMTP_FORWARD_ATTACHMENT)
         {
            iRet = EME_SMTP_Forward(pstTmHandle->pvSMPT, pstMailInfo);
            EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_EncodingMsgCB--, Forward message failed");
         }
         else if(pstTransEvt->uiTransType == EME_TRANS_SMTP_SEND_RECEIPT)
         {
            /* check smtp state */
            if (pstTmHandle->iSMPTState != EME_STACK_STATE_LOGIN)
            {
               iRet = EME_FAILED;
               break;
            }
            
            /* record smtp callback data */
            pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;
            
            iRet = EME_SMTP_Send(pstTmHandle->pvSMPT, pstMailInfo);
            EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_EncodingMsgCB--, send message failed");
            /* register free function */
            pstTransEvt->pfnFreeSigData = NULL;
         }
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = iResult;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_TRANS_END, EME_SUCCESS, pstTransEvt);
      }
   } while(0);

   EME_UTILS_FREEIF(pvMemMgr, pstCBData);
   if(NULL != pstMailInfo)
   {
      EME_UTILS_FREEIF(pvMemMgr, pstMailInfo->pcBcc);
      EME_UTILS_FREEIF(pvMemMgr, pstMailInfo->pcCc);
      EME_UTILS_FREEIF(pvMemMgr, pstMailInfo->pcFrom);
      EME_UTILS_FREEIF(pvMemMgr, pstMailInfo->pcRawMail);
      EME_UTILS_FREEIF(pvMemMgr, pstMailInfo->pcTo);
      EME_UTILS_FREEIF(pvMemMgr, pstMailInfo->pcUID);
      EME_UTILS_FREEIF(pvMemMgr, pstMailInfo);
   }

   EMECodecLib_DeleteHandle(&hEmailHandle);
   
   if(iRet != EME_SUCCESS )
   {
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = iRet;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_SMTP, EME_SIG_SMTP_TRANS_END, iRet, pstTransEvt);
   }
   EME_UTILS_LOG_INFO(("EME_FSMSMTP_EncodingMsgCB---: End, Result = %d", iResult));
}

static void  EME_FSMSMTP_pfnFree(St_TMHandle *pstTMHandle, void* pvTransEvt)
{
   St_CBEncodeData *pstCBData = NULL;
   St_TransEvt     *pstTransEvt = (St_TransEvt*)pvTransEvt;
   WE_INT32        iRet = EME_SUCCESS;
   void            *pvMemHdr = NULL;
   St_RawMailInfo* pstMailInfo = NULL;

   EME_UTILS_LOG_INFO(("EME_FSMSMTP_pfnFree---: Start"));
   if(NULL == pstTMHandle || NULL == pvTransEvt)
   {
      EME_UTILS_LOG_ERROR(("EME_FSMSMTP_pfnFree--: Bad parameter and return"));
      return;
   }

   do 
   {
   	pstCBData = (St_CBEncodeData*)pstTransEvt->pvTransData;
      EME_UTILS_BREAKNULL(pstCBData, iRet, EME_FAILED, "EME_FSMSMTP_pfnFree--: pstCBData is null");
      pvMemHdr = EME_OEMTMLib_GetMemHdr(pstCBData->pvTMHandle);

      pstMailInfo = (St_RawMailInfo*)pstCBData->pvUserData;
      
      EMECodecLib_DeleteHandle(&pstCBData->pvCodec);
      EME_UTILS_FREEIF(pvMemHdr, pstCBData);
   } while(0);

   if(NULL != pstMailInfo)
   {
      EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcBcc);
      EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcCc);
      EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcFrom);
      EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcRawMail);
      EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcTo);
      EME_UTILS_FREEIF(pvMemHdr, pstMailInfo->pcUID);
      EME_UTILS_FREEIF(pvMemHdr, pstMailInfo);
   }

   EME_UTILS_LOG_INFO(("EME_FSMSMTP_pfnFree---: End, Result = %d", SUCCESS));
}
