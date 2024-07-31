/*=====================================================================================
FILE NAME   : eme_fsm_imap.c
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
    2007-03-02     Jiang Dingqing                  create eme_fsm_imap.c
          
=====================================================================================*/

/*===================================================================
* 
*                     Include Files
* 
====================================================================*/
/*Include System head file*/
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
#include "eme_imap_api.h"
#include "eme_tm_utils_api.h"
#include "email_store_api.h"
#include "eme_fsm_imap_api.h"
#include "ems_store_priv.h"
/*Include Private head file*/

/*The header file of dialling*/
//#include "oem_ppp_api.h"

/*=============================================================================
* 
*     DEFINITIONS AND CONSTANTS(It is not recommended)
* It is recommended that this section is defined to private head file.
* 
=============================================================================*/
typedef struct _St_IMAPActResult
{
   WE_UINT32 uiUID;       /*UID*/
   WE_UINT32 uiMailSize;  /*Body size*/
   WE_CHAR   *pcFileName; /*file name*/
   WE_INT32  iResult;     /*Result*/
}St_IMAPActResult;

typedef struct _St_CBIMAPDecodeData
{
   void      *pvUserData;
   void      *pvTMHandle;
   void      *pvTranEvent;
   void      *pvStore;
   void      *pvCodec;
   St_IMAPActResult stActRsult;
   WE_UINT32 uiMsgId;
   WE_INT32  iDecodingType;
   WE_BOOL   bSpec;
   WE_CHAR   cRes[3];
}St_CBIMAPDecodeData;
/*=============================================================================
*
*   Static Function Prototype Declare Section
*
=============================================================================*/
/* sig handle functions */
static WE_INT32 EME_FSMIMAP_TransStart(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMIMAP_Open(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMIMAP_SelectMailBox(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMIMAP_GetNewMailInfo(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMIMAP_FetchNew(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMIMAP_FetchSpec(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMIMAP_HandleMail(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMIMAP_HandleMailBox(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMIMAP_Close(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMIMAP_TransEnd(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);

static WE_INT32 EME_FSMIMAP_Cancel(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);


/* stack callback functions */
static WE_INT32 EME_CBIMAP_Open(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet);
static WE_INT32 EME_CBIMAP_Close(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet);
static WE_INT32 EME_CBIMAP_SelectMailBox(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet);
static WE_INT32 EME_CBIMAP_GetNewMailInfo(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet);
static WE_INT32 EME_CBIMAP_FetchNewMails(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet);
static WE_INT32 EME_CBIMAP_FetchNewHdrs(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet);
static WE_INT32 EME_CBIMAP_FetchSpecMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet);
static WE_INT32 EME_CBIMAP_FetchSpecPart(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet);
static WE_INT32 EME_CBIMAP_HandleMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet);
static WE_INT32 EME_CBIMAP_HandleMailBox(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet);

static void EME_CBIMAP_DecodingMsgCB(IEMSGHandle hEmailHandle, void *pvUserData, int iResult);

static void  EME_CBIMAP_pfnFree(St_TMHandle *pstTMHandle, void* pvTransEvt);
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
 *    FUNCTION    EME_HandleIMAPSig   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. dispatch signals to proper FSM.
 *              3. delete the signal after it is handled.
*    ================================================================*/
WE_INT32 EME_HandleIMAPSig(St_TMHandle *pstTmHandle, WE_UINT32 uiEvtCode, WE_UINT16 usWParam, WE_UINT32 ulDwParam)
{   
   WE_INT32    iRet = EME_SUCCESS;

   if (pstTmHandle == NULL)
   {
      return EME_FAILED;
   }

   switch(uiEvtCode)
   {
   case EME_SIG_IMAP_TRANS_START:
      iRet = EME_FSMIMAP_TransStart(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;

   case EME_SIG_IMAP_OPEN:
      iRet = EME_FSMIMAP_Open(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;

   case EME_SIG_IMAP_SELECT_MAILBOX:
      iRet = EME_FSMIMAP_SelectMailBox(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;

   case EME_SIG_IMAP_GET_NEWMAILINFO:
      iRet = EME_FSMIMAP_GetNewMailInfo(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;

   case EME_SIG_IMAP_FETCH_NEWMAILS:
   case EME_SIG_IMAP_FETCH_NEWHDRS:
      iRet = EME_FSMIMAP_FetchNew(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;

   case EME_SIG_IMAP_FETCH_SPEC_MAIL:
   case EME_SIG_IMAP_FETCH_SPEC_PART:
      iRet = EME_FSMIMAP_FetchSpec(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;

   case EME_SIG_IMAP_HANDLE_MAIL:
      iRet = EME_FSMIMAP_HandleMail(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;

   case EME_SIG_IMAP_HANDLE_MAILBOX:
      iRet = EME_FSMIMAP_HandleMailBox(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;

   case EME_SIG_IMAP_CLOSE:
      iRet = EME_FSMIMAP_Close(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;

   case EME_SIG_IMAP_TRANS_END:
      iRet = EME_FSMIMAP_TransEnd(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;

   case EME_SIG_IMAP_CANCEL:
      iRet = EME_FSMIMAP_Cancel(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
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
 *              2. if the imap state is login, trans fsm to imap operation fsm,
                   else select network,trans fsm to IMAP_OPEN
 *              3. if there is something wrong,trans fsm to TRANS_END with failed result
*    =========================================================================*/
static WE_INT32 EME_FSMIMAP_TransStart(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   WE_UINT32      uiSigId = 0;

   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;   
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMIMAP_TransStart, trans evt error");

      pstTmHandle->iCanBeCancel = EME_CANCEL_ENABLED;

      if(!EME_DataAccess_IsEnoughSpace(EME_OEMTMLib_GetIFileMgr(pstTmHandle), EMS_LEAST_EML_SPACE))
      {
         iRet = EME_NOSTORAGE;
         EME_UTILS_BREAKIF(iRet, "no storeage for new mail");
      }
      
      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_IMAP_TRANS_START;
      
      if (pstTmHandle->iIMAPState == EME_STACK_STATE_LOGIN)
      {
         switch(pstTransEvt->uiTransType)
         {
         case EME_TRANS_IMAP4_FETCH_NEW_MAILS_HEADER:
         case EME_TRANS_IMAP4_FETCH_NEW_MAILS:
         case EME_TRANS_IMAP4_FETCH_SPEC_MAIL:
         case EME_TRANS_IMAP4_FETCH_SPEC_MAIL_PART:
         case EME_TRANS_IMAP4_MARK_DEL_ON_SERVER:
         case EME_TRANS_IMAP4_UNMARK_DEL_ON_SERVER:
         case EME_TRANS_IMAP4_MARK_SEEN_ON_SERVER:
         case EME_TRANS_IMAP4_UNMARK_SEEN_ON_SERVER:
         case EME_TRANS_IMAP4_COPY_MAIL_TO_MAILBOX:
            uiSigId = EME_SIG_IMAP_SELECT_MAILBOX;
            break;
            
         case EME_TRANS_IMAP4_UPLOAD_MAIL_TO_SERVER:
            uiSigId = EME_SIG_IMAP_HANDLE_MAIL;
            break;
            
         case EME_TRANS_IMAP4_CREAT_MAILBOX:
         case EME_TRANS_IMAP4_RENAME_MAILBOX:
         case EME_TRANS_IMAP4_DELETE_MAILBOX:
         case EME_TRANS_IMAP4_SUBSCRIBE_MAILBOX:
         case EME_TRANS_IMAP4_UNSUBSCRIBE_MAILBOX:
         case EME_TRANS_IMAP4_LIST_SUBSCRIBED_MAILBOXES:
         case EME_TRANS_IMAP4_LIST_ALL_MAILBOXES:
         case EME_TRANS_IMAP4_EXPUNGE_DEL_MAIL_IN_MAILBOX:
            uiSigId = EME_SIG_IMAP_HANDLE_MAILBOX;
            break;
            
         default:
            iRet = EME_FAILED;
            break;
         }
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_Open, unknow trans type");

         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_IMAP, uiSigId, (WE_VOID*)pstTransEvt);
      }
      else
      {
//         iRet = EME_GetIntOptValue(pstTmHandle->pstAccountOpts, EME_OPTID_NETWORK_UMTS_PDP, &iPDP);
//         if (iRet != SUCCESS)
//         {
//            iPDP = EME_NETWORK_DEFAULT;
//         }

//         iRet = OEMTMLib_SeletNetWork(pstTmHandle, iPDP);
//         if (iRet == AEE_NET_EOPNOTSUPP)
//         {
//            iRet = EME_SUCCESS;
//         }
//         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_TransStart, select network failed");
         
         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_OPEN, (WE_VOID*)pstTransEvt);
      }
      
      /* register free function */
      pstTransEvt->pfnFreeSigData = NULL;

   } while(0);

   if (iRet != EME_SUCCESS)
   {
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = iRet;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_SUCCESS, pstTransEvt);
   }

   return iRet;
}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMIMAP_Open   
 *
 *    Pseudo Code:    
 *              1. check input parameters
                2. get account cfg info
 *              3. connect to imap server
 *              4. if there is something wrong,trans fsm to TRANS_END
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMIMAP_Open(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   IMAPHandle     pvIMAP = NULL;
   WE_CHAR        *pcTmp = NULL;
   St_IMAPOpt     stIMAPInfo[EME_IMAP_OPEN_OPT_NUM];
   WE_INT32       iPort = 0;
   WE_UINT32       uiSizeLimit = 0;
   WE_INT32       iIsNeedSSL = FALSE;
   WE_UINT32      uiNumberLimit = 0;
   WE_INT32 iLast = 0;

   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);
   
   do 
   {
      EME_UTILS_MEMSET(stIMAPInfo, 0, EME_IMAP_OPEN_OPT_NUM * sizeof(St_IMAPOpt));

      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;   
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMIMAP_Open, trans evt error");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_IMAP_OPEN;

      /* get imap handle */
      pvIMAP = pstTmHandle->pvIMAP;
      EME_UTILS_BREAKNULL(pvIMAP, iRet, EME_FAILED, "EME_FSMIMAP_Open, imap handle error");

      /*get account cfg info*/
      iRet = EME_GetStrOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_PASSWORD, &pcTmp);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_Open, get password failed");
      EME_UTILS_BREAKNULL(pcTmp, iRet, EME_FAILED, "EME_FSMIMAP_Open, get password failed");
      stIMAPInfo[0].iId = EME_IMAP_PASSWORD;
      stIMAPInfo[0].pvVal = (WE_VOID*)pcTmp;
      pcTmp = NULL;

      iRet = EME_GetStrOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_USERNAME, &pcTmp);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_Open, get username failed");
      EME_UTILS_BREAKNULL(pcTmp, iRet, EME_FAILED, "EME_FSMIMAP_Open, get username failed");
      stIMAPInfo[1].iId = EME_IMAP_USERNAME;
      stIMAPInfo[1].pvVal = (WE_VOID*)pcTmp;
      pcTmp = NULL;

      iRet = EME_GetStrOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_SERVER_ADDR, &pcTmp);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_Open, get ser address failed");
      EME_UTILS_BREAKNULL(pcTmp, iRet, EME_FAILED, "EME_FSMIMAP_Open, get ser address failed");
      stIMAPInfo[2].iId = EME_IMAP_SERVER_ADDR;
      stIMAPInfo[2].pvVal = (WE_VOID*)pcTmp;
      pcTmp = NULL;
      
      iRet = EME_GetStrOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_UID_FILE_IMAP, &pcTmp);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_Open, get ser address failed");
      EME_UTILS_BREAKNULL(pcTmp, iRet, EME_FAILED, "EME_FSMIMAP_Open, get ser address failed");
      stIMAPInfo[3].iId = EME_IMAP_UID_FILE_PATH;
      stIMAPInfo[3].pvVal = (WE_VOID*)pcTmp;
      pcTmp = NULL;

      iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_SERVER_PORT, &iPort);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_Open, get port failed");
      stIMAPInfo[4].iId = EME_IMAP_SERVER_PORT;
      stIMAPInfo[4].pvVal = (WE_VOID*)iPort;
      
      iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_SIZE_LIMIT, (WE_INT32*)&uiSizeLimit);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_Open, get size limit failed");
      stIMAPInfo[5].iId = EME_IMAP_SIZE_LIMIT;
      stIMAPInfo[5].pvVal = (WE_VOID*)uiSizeLimit;

      iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_IMAP_NEED_SSL, &iIsNeedSSL);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_Open, get size limit failed");
      stIMAPInfo[6].iId = EME_IMAP_IS_NEED_SSL;
      stIMAPInfo[6].pvVal = (WE_VOID*)iIsNeedSSL;

      iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_NUM_LIMIT, (WE_INT32*)&uiNumberLimit);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_Open, get number limit failed");
      stIMAPInfo[7].iId = EME_IMAP_NUMBER_LIMIT;
      stIMAPInfo[7].pvVal = (WE_VOID*)uiNumberLimit;

       iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_LATEST, (WE_INT32*)&iLast);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_Open, get number limit failed");
      stIMAPInfo[8].iId = EME_IMAP_RECEIVE_LAST;
      stIMAPInfo[8].pvVal = (WE_VOID*)iLast;

      stIMAPInfo[9].iId = EME_IMAP_OPTEND;
         
      /* record imap callback data */
      pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;

      /* imap open */
      iRet = EME_IMAP_OpenIMAP(pvIMAP, stIMAPInfo);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_Open, imap open failed");
      
      /* register free function */
      pstTransEvt->pfnFreeSigData = NULL;

   } while(0);

   if (iRet != EME_SUCCESS)
   {
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_FAILED, pstTransEvt);
   }   

   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMIMAP_SelectMailBox   
 *
 *    Pseudo Code:    
 *              1. check input parameters
                2. get box name from store by MsgId or from TransOpts
 *              3. select mail box by box name
 *              4. if there is something wrong,trans fsm to TRANS_END
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMIMAP_SelectMailBox(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   IMAPHandle     pvIMAP = NULL;
   WE_CHAR        *pcMailboxName = NULL;
   WE_VOID        *pvStore = NULL;
   WE_VOID        *pvFileMgr = NULL;
   EmailOpt       *pstHeaderOpts = NULL;
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
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMIMAP_SelectMailBox, trans evt error");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_IMAP_SELECT_MAILBOX;

      /* get imap handle */
      pvIMAP = pstTmHandle->pvIMAP;
      EME_UTILS_BREAKNULL(pvIMAP, iRet, EME_FAILED, "EME_FSMIMAP_SelectMailBox, imap handle error");

      /* record imap callback data */
      pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;

      if (EME_TRANS_IMAP4_FETCH_SPEC_MAIL == pstTransEvt->uiTransType ||
          EME_TRANS_IMAP4_FETCH_SPEC_MAIL_PART == pstTransEvt->uiTransType ||
          EME_TRANS_IMAP4_MARK_DEL_ON_SERVER == pstTransEvt->uiTransType ||
          EME_TRANS_IMAP4_UNMARK_DEL_ON_SERVER == pstTransEvt->uiTransType ||
          EME_TRANS_IMAP4_MARK_SEEN_ON_SERVER == pstTransEvt->uiTransType ||
          EME_TRANS_IMAP4_UNMARK_SEEN_ON_SERVER == pstTransEvt->uiTransType ||
          EME_TRANS_IMAP4_COPY_MAIL_TO_MAILBOX == pstTransEvt->uiTransType)
      {
         /* get file mgr */
//         pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
//         EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec,get file mgr failed");      
         
         /* get message id */
         iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, get message uidl failed");
         EME_UTILS_BREAKNULL(pcMsgId, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec, get message uidl failed");

         uiMsgId = EME_UTILS_ATOX(pcMsgId);
         
         iRet = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, init store handle failed");
         EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec, init store handle failed");
         
         iRet = EmailStore_GetMsgHeaders(pvStore, uiMsgId, &pstHeaderOpts);
         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, get message header failed");
         EME_UTILS_BREAKNULL(pstHeaderOpts, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec, get message header failed");
         
         iRet = EME_GetStrOptValue((EMEOpt*)pstHeaderOpts, EMAIL_OPT_BOXNAME, &pcMailboxName);
         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_SelectMailBox, get boxname failed");
         EME_UTILS_BREAKNULL(pcMailboxName, iRet, EME_FAILED, "EME_FSMIMAP_SelectMailBox, mailbox name failed");
      }
      else   /*TransType is fetch all msg or all msg header*/
      {
         iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_LOGIN_MAILBOXNAME, &pcMailboxName);
         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_SelectMailBox, get password failed");
         EME_UTILS_BREAKNULL(pcMailboxName, iRet, EME_FAILED, "EME_FSMIMAP_SelectMailBox, mailbox name failed");
      }

      /* select mail box */
      iRet = EME_IMAP_SelectMailbox(pvIMAP, pcMailboxName);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_SelectMailBox, select mail box failed");
      
      /* register free function */
      pstTransEvt->pfnFreeSigData = NULL;

   } while(0);

   EmailStore_DeleteHandle(pvStore);

   if (iRet != EME_SUCCESS)
   {
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_FAILED, pstTransEvt);
   }   

   return iRet;
}

static WE_INT32 EME_FSMIMAP_GetNewMailInfo(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   return EME_SUCCESS;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMIMAP_FetchNew   
 *
 *    Pseudo Code:    
 *              1. check input parameters
                2. fetch all new mails or all new mails header
 *                    if the result of fetch is no new mail
                          trans fsm to IMAP_CLOSE fsm
 *              4. if there is something wrong,trans fsm to TRANS_END with failed result
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMIMAP_FetchNew(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   IMAPHandle     pvIMAP = NULL;
   
   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);
   
   do 
   {
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;   
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMIMAP_FetchNew, trans evt error");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_IMAP_FETCH_NEWMAILS;

      /* get imap handle */
      pvIMAP = pstTmHandle->pvIMAP;
      EME_UTILS_BREAKNULL(pvIMAP, iRet, EME_FAILED, "EME_FSMIMAP_FetchNew, imap handle error");
         
      /* record imap callback data */
      pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;

      /* fetch new mail */
      if (pstTransEvt->uiTransType == EME_TRANS_IMAP4_FETCH_NEW_MAILS)
      {
         iRet = EME_IMAP_FetchMail(pvIMAP);
      }
      else
      {
         iRet = EME_IMAP_FetchHeader(pvIMAP);
      }
      pstTmHandle->iCanBeCancel = EME_CANCEL_ENABLED;

      if (iRet == EME_NO_NEWMAIL || iRet == EME_RECEIVED_END)
      {
         /* all new mail has been downloaded, so cancel disabled */
         pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;

         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_CLOSE, pstTransEvt);
         if(EME_NO_NEWMAIL == iRet)
            pstTransEvt->iErrCode = EME_NO_NEWMAIL;
         else
            pstTransEvt->iErrCode = EME_SUCCESS;
         iRet = EME_SUCCESS;
      }
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchNew, fetch new failed");    
      
      /* register free function */
      pstTransEvt->pfnFreeSigData = NULL;

   } while(0);

   if (iRet != EME_SUCCESS)
   {
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, iRet, pstTransEvt);
   }   

   return iRet;
}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMIMAP_FetchSpec   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. get spec uid by msg id from storage,
                   then fetch all parts or one part 
 *              3. if there is something wrong,trans fsm to TRANS_END
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMIMAP_FetchSpec(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   IMAPHandle     pvIMAP = NULL;
   WE_UINT32      uiUid = 0;
   WE_CHAR        *pcMsgUidl = NULL;
   WE_CHAR        *pcPartId = NULL;
   WE_INT32       iSizeLimit = 0;
   WE_CHAR        *pcMsgId = NULL;
   WE_VOID        *pvStore = NULL;
   WE_VOID        *pvFileMgr = NULL;
   EmailOpt       *pstHeaderOpts = NULL;
   EmailOpt       *pstMsgPartOpts = NULL;
   WE_UINT32      uiMailSize = 0;
   WE_CHAR        *pcSectionID = NULL;
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
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec, trans evt error");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_IMAP_FETCH_SPEC_MAIL;
 
      /* get imap handle */
      pvIMAP = pstTmHandle->pvIMAP;
      EME_UTILS_BREAKNULL(pvIMAP, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec, imap handle error");
      

      /* get file mgr */
//      pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
//      EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec,get file mgr failed");      

      /* get message id */
      iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, get message uidl failed");
      EME_UTILS_BREAKNULL(pcMsgId, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec, get message uidl failed");

      uiMsgId = EME_UTILS_ATOX(pcMsgId);

      iRet = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, init store handle failed");
      EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec, init store handle failed");
      
      iRet = EmailStore_GetMsgHeaders(pvStore, uiMsgId, &pstHeaderOpts);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, get message header failed");
      EME_UTILS_BREAKNULL(pstHeaderOpts, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec, get message header failed");
      
      iRet = EME_GetStrOptValue((EMEOpt*)pstHeaderOpts, EMAIL_OPT_UIDL, &pcMsgUidl);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, get UIDL failed");
      EME_UTILS_BREAKNULL(pcMsgUidl, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec, get UIDL failed");
      uiUid = EME_UTILS_ATOI((const WE_UCHAR*)pcMsgUidl);
      
      iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_SIZE_LIMIT, &iSizeLimit);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, get size limit failed");
      
      /* record imap callback data */
      pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;

      /* fetch new mail */
      if (pstTransEvt->uiTransType == EME_TRANS_IMAP4_FETCH_SPEC_MAIL)
      {
         iRet = EME_GetIntOptValue((EMEOpt*)pstHeaderOpts, EMAIL_OPT_SIZE, (WE_INT32*)&uiMailSize);
         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, get mail size failed");

         if(EME_CheckFitFreeSpaceEx(EME_OEMTMLib_GetIFileMgr(pstTmHandle), uiMsgId, uiMailSize))
         {
            iRet = EME_IMAP_FetchSpecificMail(pvIMAP, uiUid, uiMailSize);
         }
         else
         {
            iRet = EME_NOSTORAGE;
         }
      }
      else
      {
         iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_PART_ID, &pcPartId);
         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, get uidl failed");
         EME_UTILS_BREAKNULL(pcPartId, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec, get part id failed");

         iRet = EmailStore_GetMsgPart(pvStore, uiMsgId, EME_UTILS_ATOI((const WE_UCHAR*)pcPartId), &pstMsgPartOpts);
         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, get message part failed");
         EME_UTILS_BREAKNULL(pstMsgPartOpts, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec, get message part failed");
         /*get data size*/
         iRet = EME_GetIntOptValue((EMEOpt*)pstMsgPartOpts, EMAILPART_OPT_DATA_SIZE, (WE_INT32*)&uiMailSize);
         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, get msg part size failed");
         /*get section id*/
         iRet = EME_GetStrOptValue((EMEOpt*)pstMsgPartOpts, EMAILPART_OPT_SECTION_ID, &pcSectionID);
         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, get msg section id failed");

         iRet = EME_IMAP_FetchSpecificPart(pvIMAP, uiUid, pcSectionID, uiMailSize);
      }
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, fetch spec failed");      
      
      /* register free function */
      pstTransEvt->pfnFreeSigData = NULL;

   } while(0);

   EmailStore_DeleteHandle(pvStore);

   if (iRet != EME_SUCCESS)
   {
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = iRet;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_SUCCESS, pstTransEvt);
   }   

   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMIMAP_HandleMail   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. get relevant info about handle mail 
                   do handle mail operation
 *              3. if there is something wrong,trans fsm to TRANS_END
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMIMAP_HandleMail(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   IMAPHandle     pvIMAP = NULL;
   St_IMAPOpt     stOpts[EME_IMAP_HANDLMAIL_OPT_NUM];
   WE_CHAR        *pcMailBoxName = NULL;
   WE_UINT32      uiUID = 0;
   WE_UINT32      uiType = 0;
   WE_CHAR        *pcMsgId = NULL;
//    EmailOpt       stStoreOpts[EME_STORE_INIT_OPT_NUM];
//    WE_CHAR        acAccountId[EME_MAX_UINT32_STR_LEN];
//    WE_CHAR        acDefBoxId[EME_MAX_UINT32_STR_LEN];
   WE_VOID        *pvStore = NULL;
   WE_VOID        *pvFileMgr = NULL;
   EmailOpt       *pstHeaderOpts = NULL;
   WE_INT32       iIsUploadMail = FALSE;
   EmailOpt       *pstCodecOpt = NULL;
   EmailOpt       *pstMsgOpts = NULL;
   WE_CHAR        *pcUIDL = NULL;
   WE_VOID        *pvCodec = NULL;

   WE_UINT32      uiMsgId = 0;

   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);
   
   do 
   {
      EME_UTILS_MEMSET(stOpts, 0, sizeof(St_IMAPOpt) * EME_IMAP_HANDLMAIL_OPT_NUM);
//       EME_UTILS_MEMSET(stStoreOpts, 0, sizeof(EmailOpt) * EME_STORE_INIT_OPT_NUM);
//       EME_UTILS_MEMSET(acAccountId, 0, sizeof(WE_CHAR) * EME_MAX_UINT32_STR_LEN);
//       EME_UTILS_MEMSET(acDefBoxId, 0, sizeof(WE_CHAR) * EME_MAX_UINT32_STR_LEN);    

      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;   
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMIMAP_HandleMail, trans evt error");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_IMAP_HANDLE_MAIL;
 
      /* get imap handle */
      pvIMAP = pstTmHandle->pvIMAP;
      EME_UTILS_BREAKNULL(pvIMAP, iRet, EME_FAILED, "EME_FSMIMAP_HandleMail, imap handle error");
      
      /* get file mgr */
//      pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
//      EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec,get file mgr failed");      
     
      /* get message id */
      iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, get message uidl failed");
      EME_UTILS_BREAKNULL(pcMsgId, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec, get message uidl failed");

      uiMsgId = EME_UTILS_ATOX(pcMsgId);

      iRet = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, init store handle failed");
      EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec, init store handle failed");
      
      iRet = EmailStore_GetMsgHeaders(pvStore, uiMsgId, &pstHeaderOpts);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, get message header failed");
      EME_UTILS_BREAKNULL(pstHeaderOpts, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec, get message header failed");

      /* fetch new mail */
      switch(pstTransEvt->uiTransType)
      {
      case EME_TRANS_IMAP4_MARK_DEL_ON_SERVER:
         uiType = E_EME_IMAP_MARK_DELETED;
         break;
      case EME_TRANS_IMAP4_UNMARK_DEL_ON_SERVER:
         uiType = E_EME_IMAP_UNMARK_DELETED;
         break;
      case EME_TRANS_IMAP4_MARK_SEEN_ON_SERVER:
         uiType = E_EME_IMAP_MARK_SEEN;
         break;
      case EME_TRANS_IMAP4_UNMARK_SEEN_ON_SERVER:
         uiType = E_EME_IMAP_UNMARK_SEEN;
         break;
      case EME_TRANS_IMAP4_COPY_MAIL_TO_MAILBOX:
         uiType = E_EME_IMAP_COPY;
         break;
      case EME_TRANS_IMAP4_UPLOAD_MAIL_TO_SERVER:
         uiType = E_EME_IMAP_APPEND;
         break;

      default:
         iRet = EME_FAILED;
         break;
      }
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_HandleMail, error trans type");      

      stOpts[0].iId = EME_IMAP_HANDLE_MAIL_TYPE;
      stOpts[0].pvVal = (WE_VOID*)uiType;

      /* if transaction is not upload mail */
      if (EME_TRANS_IMAP4_UPLOAD_MAIL_TO_SERVER != pstTransEvt->uiTransType) 
      {
         iRet = EME_GetStrOptValue((EMEOpt*)pstHeaderOpts, EMAIL_OPT_UIDL, &pcUIDL);
         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, get UIDL failed");
         EME_UTILS_BREAKNULL(pcUIDL, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec, get UIDL failed");
         uiUID = EME_UTILS_ATOI((const WE_UCHAR*)pcUIDL);
      }

      switch(pstTransEvt->uiTransType)
      {
      case EME_TRANS_IMAP4_MARK_DEL_ON_SERVER:  /*need uid*/
      case EME_TRANS_IMAP4_UNMARK_DEL_ON_SERVER:
      case EME_TRANS_IMAP4_MARK_SEEN_ON_SERVER:
      case EME_TRANS_IMAP4_UNMARK_SEEN_ON_SERVER:
         stOpts[1].iId = EME_IMAP_UID;
         stOpts[1].pvVal = (WE_VOID*)uiUID;
         stOpts[2].iId = EME_IMAP_OPTEND;
         break;

      case EME_TRANS_IMAP4_COPY_MAIL_TO_MAILBOX:  /*need uid & boxname*/

         iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_HANDLE_MAILBOXNAME, &pcMailBoxName);
         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_HandleMail, get mail box name failed");
         EME_UTILS_BREAKNULL(pcMailBoxName, iRet, EME_FAILED, "EME_FSMIMAP_HandleMail, get mail box name failed");

         stOpts[1].iId = EME_IMAP_UID;
         stOpts[1].pvVal = (WE_VOID*)uiUID;
         stOpts[2].iId = EME_IMAP_MAILBOX_NAME;
         stOpts[2].pvVal = (WE_VOID*)pcMailBoxName;
         stOpts[3].iId = EME_IMAP_OPTEND;
         break;
      case EME_TRANS_IMAP4_UPLOAD_MAIL_TO_SERVER:  /*need encode buffer & buffer size & boxname*/
         iIsUploadMail = TRUE;
         break;
      default:
         iRet = EME_FAILED;
         break;
      }
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_HandleMail, handle mail failed");

      if (TRUE == iIsUploadMail)
      {
         WE_CHAR* pcMailBuffer = NULL;
         WE_UINT32 uiPartNum = 0;
         WE_UINT32 i = 0;
         /* init codec handle */
         iRet = EMECodecLib_InitHandle(&pvCodec, pvMemHdr, pvFileMgr, OEMTMLib_GetHCSC(pstTmHandle));
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, create codec handle failed");
         EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_FSMSMTP_Send, create codec handle failed");
         
         /* get part count */
         iRet = EmailStore_GetMsgPartCount(pvStore, uiMsgId, &uiPartNum);
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, get message part num failed");
         
         /* add message header to codec */
         iRet = EMECodecLib_AddHeader(pvCodec, pstHeaderOpts);
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, add message header failed");
         
         /* get and add message parts */
         for (i = 0; i < uiPartNum; i++)
         {
            pstMsgOpts = NULL;
            
            iRet = EmailStore_GetMsgPart(pvStore, uiMsgId, i, &pstMsgOpts);
            EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, get message part failed");
            EME_UTILS_BREAKNULL(pstMsgOpts, iRet, EME_FAILED, "EME_FSMSMTP_Send, get message part failed");
            
            iRet = EMECodecLib_AddPart(pvCodec, pstMsgOpts);
            EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, add message part failed");
         }
         EME_UTILS_BREAKIF(iRet, "get or add message part failed");
         /*encode date*/
         iRet = EMECodecLib_Encoding(pvCodec, &pstCodecOpt);
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, encoding message failed");
         EME_UTILS_BREAKNULL(pstCodecOpt, iRet, EME_FAILED, "EME_FSMSMTP_Send, encoding message failed");
         
         iRet = EME_GetStrOptValue((EMEOpt*)pstCodecOpt, EMAILMESSAGE_OPT_BUFF, &pcMailBuffer);
         EME_UTILS_BREAKIF(iRet, "EME_FSMSMTP_Send, encoding message failed");
         EME_UTILS_BREAKNULL(pcMailBuffer, iRet, EME_FAILED, "EME_FSMSMTP_Send, encoding message failed");

         stOpts[1].iId = EME_IMAP_MAIL_BUFFER;
         stOpts[1].pvVal = pcMailBuffer;
         
         stOpts[2].iId = EME_IMAP_MAIL_BUFFER_SIZE;
         stOpts[2].pvVal = (WE_VOID*)EME_UTILS_STRLEN((WE_CHAR*)pcMailBuffer);

         iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_HANDLE_MAILBOXNAME, &pcMailBoxName);
         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_HandleMail, get mail box name failed");
         EME_UTILS_BREAKNULL(pcMailBoxName, iRet, EME_FAILED, "EME_FSMIMAP_HandleMail, get mail box name failed");
         
         stOpts[3].iId = EME_IMAP_MAILBOX_NAME;
         stOpts[3].pvVal = (WE_VOID*)pcMailBoxName;
         stOpts[4].iId = EME_IMAP_OPTEND;
      }

      /* record imap callback data */
      pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;

      iRet = EME_IMAP_HandleMail(pvIMAP, stOpts);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_HandleMail, handle mail failed");

      /* register free function */
      pstTransEvt->pfnFreeSigData = NULL;

   } while(0);
   
   EmailStore_DeleteHandle(pvStore);
   EMECodecLib_DeleteHandle(&pvCodec);

   if (iRet != EME_SUCCESS)
   {
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_FAILED, pstTransEvt);
   }   

   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMIMAP_HandleMailBox   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. get relevant info about handle mail box
                   do handle mail box operation
 *              3. if there is something wrong,trans fsm to TRANS_END
 *  
 * ===================================================================================================*/
static WE_INT32 EME_FSMIMAP_HandleMailBox(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   IMAPHandle     pvIMAP = NULL;
   St_IMAPOpt     stOpts[EME_IMAP_HANDLMAIL_OPT_NUM];
   WE_CHAR        *pcMailBoxName = NULL;
   WE_CHAR        *pcMailBoxRename = NULL;
   WE_UINT32      uiType = 0;


   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);
   
   do 
   {
      EME_UTILS_MEMSET(stOpts, 0, sizeof(St_IMAPOpt) * EME_IMAP_HANDLMAIL_OPT_NUM);

      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;   
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMIMAP_HandleMailBox, trans evt error");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_IMAP_HANDLE_MAILBOX;
 
      /* get imap handle */
      pvIMAP = pstTmHandle->pvIMAP;
      EME_UTILS_BREAKNULL(pvIMAP, iRet, EME_FAILED, "EME_FSMIMAP_HandleMailBox, imap handle error");
      
      /* handle mail box type */
      switch(pstTransEvt->uiTransType)
      {
      case EME_TRANS_IMAP4_EXPUNGE_DEL_MAIL_IN_MAILBOX:
         uiType = E_EME_IMAP_EXPUNGMSG;
         break;
      case EME_TRANS_IMAP4_CREAT_MAILBOX:
         uiType = E_EME_IMAP_CREATE;
         break;
      case EME_TRANS_IMAP4_RENAME_MAILBOX:
         uiType = E_EME_IMAP_RENAME;
         break;
      case EME_TRANS_IMAP4_DELETE_MAILBOX:
         uiType = E_EME_IMAP_DELETE;
         break;
      case EME_TRANS_IMAP4_SUBSCRIBE_MAILBOX:
         uiType = E_EME_IMAP_SUBSCRIBE;
         break;
      case EME_TRANS_IMAP4_UNSUBSCRIBE_MAILBOX:
         uiType = E_EME_IMAP_UNSUBSCRIBE;
         break;
      case EME_TRANS_IMAP4_LIST_SUBSCRIBED_MAILBOXES:
         uiType = E_EME_IMAP_LIST_SUBSCRIBE_MAILBOXS;
         break;
      case EME_TRANS_IMAP4_LIST_ALL_MAILBOXES:
         uiType = E_EME_IMAP_LIST_MAILBOXS;
         break;

      default:
         iRet = EME_FAILED;
         break;
      }
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_HandleMailBox, error trans type");      

      stOpts[0].iId = EME_IMAP_HANDLE_MAILBOX_TYPE;
      stOpts[0].pvVal = (WE_VOID*)uiType;

      switch(pstTransEvt->uiTransType)
      {
      case EME_TRANS_IMAP4_EXPUNGE_DEL_MAIL_IN_MAILBOX:   /*need boxname*/
      case EME_TRANS_IMAP4_CREAT_MAILBOX:
      case EME_TRANS_IMAP4_DELETE_MAILBOX:
      case EME_TRANS_IMAP4_SUBSCRIBE_MAILBOX:
      case EME_TRANS_IMAP4_UNSUBSCRIBE_MAILBOX:
         iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_HANDLE_MAILBOXNAME, &pcMailBoxName);
         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_HandleMailBox, get mail box name failed");
         EME_UTILS_BREAKNULL(pcMailBoxName, iRet, EME_FAILED, "EME_FSMIMAP_HandleMailBox, get mail box name failed");
         
         stOpts[1].iId = EME_IMAP_MAILBOX_NAME;
         stOpts[1].pvVal = (WE_VOID*)pcMailBoxName;
         stOpts[2].iId = EME_IMAP_OPTEND;
         break;

      case EME_TRANS_IMAP4_RENAME_MAILBOX:      /*need original boxname & new boxname*/
         iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_LOGIN_MAILBOXNAME, &pcMailBoxName);
         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_HandleMailBox, get mail box name failed");
         EME_UTILS_BREAKNULL(pcMailBoxName, iRet, EME_FAILED, "EME_FSMIMAP_HandleMailBox, get mail box name failed");

         iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_HANDLE_MAILBOXNAME, &pcMailBoxRename);
         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_HandleMailBox, get mail box name failed");
         EME_UTILS_BREAKNULL(pcMailBoxName, iRet, EME_FAILED, "EME_FSMIMAP_HandleMailBox, get mail box name failed");
         
         stOpts[1].iId = EME_IMAP_MAILBOX_NAME;
         stOpts[1].pvVal = (WE_VOID*)pcMailBoxName;

         stOpts[2].iId = EME_IMAP_MAILBOX_RENAME;
         stOpts[2].pvVal = (WE_VOID*)pcMailBoxRename;
         
         stOpts[3].iId = EME_IMAP_OPTEND;
         break;

      case EME_TRANS_IMAP4_LIST_SUBSCRIBED_MAILBOXES:  /*need nothing*/
      case EME_TRANS_IMAP4_LIST_ALL_MAILBOXES:
         stOpts[1].iId = EME_IMAP_OPTEND;
         break;

      default:
         iRet = EME_FAILED;
         break;
      }
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_HandleMailBox, error trans type");      


      /* record imap callback data */
      pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;

      iRet = EME_IMAP_HandleMailbox(pvIMAP, stOpts);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_HandleMailBox, handle mailbox failed");

      /* register free function */
      pstTransEvt->pfnFreeSigData = NULL;

   } while(0);

   if (iRet != EME_SUCCESS)
   {
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_FAILED, pstTransEvt);
   }   

   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMIMAP_Close   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if imap need close ,
                       quit imap server,
                   else trans fsm to TRANS_END with success result
 *              3. if there is something wrong,trans fsm to TRANS_END with success result
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMIMAP_Close(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_VOID     *pvMemHdr = NULL;
   St_TransEvt *pstTransEvt = NULL;
   IMAPHandle  pvIMAP = NULL;

   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMIMAP_Close, transEvt is null");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_IMAP_CLOSE;

      /* get imap handle */
      pvIMAP = pstTmHandle->pvIMAP;
      EME_UTILS_BREAKNULL(pvIMAP, iRet, EME_FAILED, "EME_FSMIMAP_Close, imap handle is null");
      
      if (EME_IMAPCloseNeed(pstTmHandle, pstTransEvt))
      {
         /* record imap callback data */
         pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;

         iRet = EME_IMAP_CloseIMAP(pvIMAP);
         EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_Close, failed");
      }
      else
      {
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_SUCCESS, pstTransEvt);
      }

   }while(0);


   if (iRet != EME_SUCCESS)
   {  
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_FAILED, pstTransEvt);
   }

   return iRet;   
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMPOP_TransEnd   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if trans result is not success,cancel imap operation
                3. delete the transaction from trans queue ,
                4. reply this result of trans to app
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMIMAP_TransEnd(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   WE_INT32       iTransResult = EME_SUCCESS;
   WE_UINT32      uiTransID = 0;
   WE_UINT32      uiTransType = 0;
   WE_UINT32      uiClsId = 0;
   
   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }
   
   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);
   
   do 
   {
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMIMAP_TransEnd, transEvt is null");
      
      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_IMAP_TRANS_END;
      
      /* get trans result */
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
         EME_IMAP_Cancel(pstTmHandle->pvIMAP);
         pstTmHandle->iIMAPState = EME_STACK_STATE_LOGOUT;
      }
      else
      {
         iTransResult = pstTransEvt->iErrCode;
         EME_UTILS_LOG_ERROR(("TTTTTTTTTTTTTTTTTTTTransaction result:%x", iTransResult));
      }
      
      uiTransID = pstTransEvt->uiTransID;
      uiTransType = pstTransEvt->uiTransType;
//      iRet = EME_GetIntOptValue(pstTransEvt->pstTransOpts, EME_OPTID_REPLY_CLSID, (WE_INT32*)&uiClsId);
//      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_TransEnd, get cls id failed");
      
      /*delete the transaction from trans queue*/
      EME_DeleteTransaction(pstTmHandle, pstTransEvt);
      
      EME_UTILS_LOG_INFO(("EME_FSMIMAP_TransEnd: pstTmHandle.iIMAPState = %d", pstTmHandle->iIMAPState));
      
      EME_RemoveDir(EME_OEMTMLib_GetIFileMgr(pstTmHandle), EMEFS_TEMP_ROOT_DIR);
      
      /*
      if the state is EME_STACK_STATE_LOGOUT,need to disconnect the dail-------------just for plant of MTK
      */  
      /*	if(EME_STACK_STATE_LOGOUT == pstTmHandle->iIMAPState)
      {
      WE_INT32   nDialId = 0;
      EME_GetDialId(&nDialId);
      EME_UTILS_LOG_INFO(("EME_FSMIMAP_TransEnd: EME_GetDialId nDailid = %d", nDialId)); 
      EME_DisconnectPPP(nDialId);
      EME_UTILS_LOG_INFO(("EME_FSMIMAP_TransEnd: EME_DisconnectPPP END!"));
      EME_FreePPPData();
      EME_UTILS_LOG_INFO(("EME_FSMIMAP_TransEnd: EME_FreePPPData END!"));	 	  
      }
      */
      
      /*reply this result of trans to app*/
      iRet = OEMTMLib_TransReply(pstTmHandle, uiTransID, uiTransType, 0, uiClsId, iTransResult, pstTmHandle->acRespDesc);
      
   } while(0);
   
   return iRet;
}

static WE_INT32 EME_FSMIMAP_Cancel(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   return EME_SUCCESS;
}

/*************************************************************************************************************/
/* Callback Functions */
/*************************************************************************************************************/
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_IMAPActionCB   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. deal with the evt cb from imap stack 
 *              3. if there is something wrong,trans fsm to TRANS_END
 *  
 * ===================================================================================================*/
WE_VOID EME_IMAPActionCB(WE_VOID* pvTmHandle,  St_IMAPActRet* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;
   St_TransEvt *pstTransEvt = NULL;
   St_TMHandle *pstTmHandle = (St_TMHandle*)pvTmHandle;
   WE_UINT32   uiTotalSize = 0;
   WE_UINT32   uiTransferedSize = 0;
   WE_UINT32   uiCurrentNumber = 0;
   WE_UINT32   uiTotalNumber = 0;
   WE_CHAR     *pcMailboxName = NULL;
   WE_CHAR     *pcDelimiter = NULL;

   if (pstTmHandle == NULL)
   {
      return;
   }
   
   do 
   {
      pstTransEvt = (St_TransEvt*)pstTmHandle->pvCbkUserData;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_IMAPActionCB transevt is null");

      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_IMAPActionCB actret is null");
      
      /*copy respone decription*/
      EME_UTILS_MEMSET(pstTmHandle->acRespDesc, 0, 512);
      if(pstActRet->iResult != EME_SUCCESS && EME_UTILS_STRLEN(pstActRet->acRespDesc) > 0)
         EME_UTILS_MEMCPY(pstTmHandle->acRespDesc, pstActRet->acRespDesc, EME_UTILS_STRLEN(pstActRet->acRespDesc));
      EME_UTILS_MEMSET(pstActRet->acRespDesc, 0, 512);

      switch(pstActRet->eEvtCode)
      {   
      case E_EME_IMAP_OPEN_IMAP_END:
         iRet = EME_CBIMAP_Open(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case E_EME_IMAP_CLOSE_IMAP_END:
         iRet = EME_CBIMAP_Close(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case E_EME_IMAP_SELECT_MAILBOX_END:
         iRet = EME_CBIMAP_SelectMailBox(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case E_EME_IMAP_GET_INFOMATION_OF_NEW_MAILS_END:
         iRet = EME_CBIMAP_GetNewMailInfo(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case E_EME_IMAP_FETCH_HEADER_END:
         iRet = EME_CBIMAP_FetchNewHdrs(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case E_EME_IMAP_FETCH_MAIL_END:
         iRet = EME_CBIMAP_FetchNewMails(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case E_EME_IMAP_FETCH_SPECIFIC_MAIL_END:
         iRet = EME_CBIMAP_FetchSpecMail(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case E_EME_IMAP_FETCH_SPECIFIC_PART_END:
         iRet = EME_CBIMAP_FetchSpecPart(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case E_EME_IMAP_HANDLE_MAIL_END:
         iRet = EME_CBIMAP_HandleMail(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case E_EME_IMAP_HANDLE_MAILBOX_END:
         iRet = EME_CBIMAP_HandleMailBox(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case E_EME_IMAP_TRANSFER_SIZE_NOTIFY:
         uiTransferedSize = pstActRet->unRetData.stTransferNotify.uiTransferedSize;
         uiTotalSize = pstActRet->unRetData.stTransferNotify.uiTotalSize;
         iRet = OEMTMLib_NotifySize(pstTmHandle, pstTransEvt, uiTransferedSize, uiTotalSize);
         break;
      case E_EME_IMAP_MAILBOX_NOTIFY:
         pcMailboxName = pstActRet->unRetData.stMailboxInfo.pcMailboxName;
         pcDelimiter = pstActRet->unRetData.stMailboxInfo.acDelimiter;
         iRet = OEMTMLib_NotifyMailbox(pstTmHandle, pstTransEvt, pcMailboxName, pcDelimiter);
         break;
      case E_EME_IMAP_NUMBER_NOTIFY:
         uiCurrentNumber = pstActRet->unRetData.stNumberNotify.uiCurrentNumber;
         uiTotalNumber = pstActRet->unRetData.stNumberNotify.uiTotalNumber;
         iRet = OEMTMLib_NotifyNumber(pstTmHandle, pstTransEvt, uiCurrentNumber, uiTotalNumber);
         break;
      default:
         iRet = pstActRet->iResult;
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
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, iRet, pstTransEvt);
   }
   
   return;      
}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBIMAP_Open   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of login imap server from stack is success,
                   trans fsm to imap operation with specified transtype
 *              3. else trans fsm to TRANS_END with result of failed
 *   ===================================================================================================*/
static WE_INT32 EME_CBIMAP_Open(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_UINT32   uiSigId = 0;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBIMAP_Open actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS)
      {
         pstTmHandle->iIMAPState = EME_STACK_STATE_LOGIN;

         switch(pstTransEvt->uiTransType)
         {
         case EME_TRANS_IMAP4_FETCH_NEW_MAILS_HEADER:
         case EME_TRANS_IMAP4_FETCH_NEW_MAILS:
         case EME_TRANS_IMAP4_FETCH_SPEC_MAIL:
         case EME_TRANS_IMAP4_FETCH_SPEC_MAIL_PART:
         case EME_TRANS_IMAP4_MARK_DEL_ON_SERVER:
         case EME_TRANS_IMAP4_UNMARK_DEL_ON_SERVER:
         case EME_TRANS_IMAP4_MARK_SEEN_ON_SERVER:
         case EME_TRANS_IMAP4_UNMARK_SEEN_ON_SERVER:
         case EME_TRANS_IMAP4_COPY_MAIL_TO_MAILBOX:
            uiSigId = EME_SIG_IMAP_SELECT_MAILBOX;
            break;

         case EME_TRANS_IMAP4_UPLOAD_MAIL_TO_SERVER:
            uiSigId = EME_SIG_IMAP_HANDLE_MAIL;
            break;

         case EME_TRANS_IMAP4_CREAT_MAILBOX:
         case EME_TRANS_IMAP4_RENAME_MAILBOX:
         case EME_TRANS_IMAP4_DELETE_MAILBOX:
         case EME_TRANS_IMAP4_SUBSCRIBE_MAILBOX:
         case EME_TRANS_IMAP4_UNSUBSCRIBE_MAILBOX:
         case EME_TRANS_IMAP4_LIST_SUBSCRIBED_MAILBOXES:
         case EME_TRANS_IMAP4_LIST_ALL_MAILBOXES:
         case EME_TRANS_IMAP4_EXPUNGE_DEL_MAIL_IN_MAILBOX:
            uiSigId = EME_SIG_IMAP_HANDLE_MAILBOX;
            break;

         default:
            iRet = EME_FAILED;
            break;
         }
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_Open, unknow trans type");
      
         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_IMAP, uiSigId, pstTransEvt);
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = pstActRet->iResult;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, pstActRet->iResult, pstTransEvt);
      }

   } while(0);

   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBIMAP_Close   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of quit imap server from stack is success,
                       register the imap state is logout
                       trans fsm to TRANS_END with result of success
 *              3. else trans fsm to TRANS_END with result of failed
 *   ===================================================================================================*/
static WE_INT32 EME_CBIMAP_Close(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBIMAP_Close, actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS)
      {
         pstTmHandle->iIMAPState = EME_STACK_STATE_LOGOUT;

         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_SUCCESS, pstTransEvt);
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_FAILED, pstTransEvt);
      }

   } while(0);

   return iRet;
}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBIMAP_SelectMailBox   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of select mail box from stack is success, 
                      if the transtype is fetch all new mails or all new mails header,
                         get info of new mails. 
                         if there is not new mail, trans fsm to IMAP_CLOSE with no new mail result.
                      else trans fsm to relevant  operation fsm   
 *              3. if the result cb of select mail box from stack is not success,
                     trans fsm to TRANS_END
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBIMAP_SelectMailBox(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_BOOL     bGetNewMailInofNeeded = FALSE;
   WE_UINT32   uiSigId = 0;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBIMAP_SelectMailBox actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS)
      {
         switch(pstTransEvt->uiTransType)
         {
         case EME_TRANS_IMAP4_FETCH_NEW_MAILS_HEADER:
         case EME_TRANS_IMAP4_FETCH_NEW_MAILS:
            bGetNewMailInofNeeded = TRUE;
            break;

         case EME_TRANS_IMAP4_FETCH_SPEC_MAIL:
            uiSigId = EME_SIG_IMAP_FETCH_SPEC_MAIL;
            break;

         case EME_TRANS_IMAP4_FETCH_SPEC_MAIL_PART:
            uiSigId = EME_SIG_IMAP_FETCH_SPEC_PART;
            break; 

         case EME_TRANS_IMAP4_MARK_DEL_ON_SERVER:
         case EME_TRANS_IMAP4_UNMARK_DEL_ON_SERVER:
         case EME_TRANS_IMAP4_MARK_SEEN_ON_SERVER:
         case EME_TRANS_IMAP4_UNMARK_SEEN_ON_SERVER:
         case EME_TRANS_IMAP4_COPY_MAIL_TO_MAILBOX:
            uiSigId = EME_SIG_IMAP_HANDLE_MAIL;
            break;
         case EME_TRANS_IMAP4_CREAT_MAILBOX:
         case EME_TRANS_IMAP4_RENAME_MAILBOX:
         case EME_TRANS_IMAP4_DELETE_MAILBOX:
         case EME_TRANS_IMAP4_SUBSCRIBE_MAILBOX:
         case EME_TRANS_IMAP4_UNSUBSCRIBE_MAILBOX:
         case EME_TRANS_IMAP4_LIST_SUBSCRIBED_MAILBOXES:
         case EME_TRANS_IMAP4_LIST_ALL_MAILBOXES:
         case EME_TRANS_IMAP4_EXPUNGE_DEL_MAIL_IN_MAILBOX:
         case EME_TRANS_IMAP4_UPLOAD_MAIL_TO_SERVER:

         default:
            iRet = EME_FAILED;
            break;
         }
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_SelectMailBox, unknow trans type");
 
         
         if (bGetNewMailInofNeeded)  /*if the transtype is fetch all new mails or all new mails header*/
         {
            /* record imap callback data */
            pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;
         
            iRet = EME_IMAP_GetInfoOfNewMails(pstTmHandle->pvIMAP);
            if (iRet == EME_NO_NEWMAIL)
            {
               EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_CLOSE, EME_NO_NEWMAIL, pstTransEvt);
               pstTransEvt->iErrCode = EME_NO_NEWMAIL;
               iRet = EME_SUCCESS;
            }
            else
            {
               EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_SelectMailBox, get mail info failed");
            }
         }
         else
         {
            EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_IMAP, uiSigId, pstTransEvt);            
         }
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_FAILED, pstTransEvt);
      }

   } while(0);

   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBIMAP_GetNewMailInfo   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of select mail box from stack is success, 
                      trans fsm to relevant fsm
 *              3. if the result cb of select mail box from stack is no new mail,
                      trans fsm to IMAP_CLOSE with no new mail result.
 *              4. if the result cb of select mail box from stack is not success,
                     trans fsm to TRANS_END with failed result
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBIMAP_GetNewMailInfo(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_UINT32   uiSigId = 0;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBIMAP_GetNewMailInfo actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS || pstActRet->iResult == EME_NO_NEWMAIL)
      {
         switch(pstTransEvt->uiTransType)
         {
         case EME_TRANS_IMAP4_FETCH_NEW_MAILS_HEADER:
            uiSigId = EME_SIG_IMAP_FETCH_NEWHDRS;
            break;

         case EME_TRANS_IMAP4_FETCH_NEW_MAILS:
            uiSigId = EME_SIG_IMAP_FETCH_NEWMAILS;
            break;

         case EME_TRANS_IMAP4_FETCH_SPEC_MAIL:
         case EME_TRANS_IMAP4_FETCH_SPEC_MAIL_PART:
         case EME_TRANS_IMAP4_MARK_DEL_ON_SERVER:
         case EME_TRANS_IMAP4_UNMARK_DEL_ON_SERVER:
         case EME_TRANS_IMAP4_MARK_SEEN_ON_SERVER:
         case EME_TRANS_IMAP4_UNMARK_SEEN_ON_SERVER:
         case EME_TRANS_IMAP4_COPY_MAIL_TO_MAILBOX:
         case EME_TRANS_IMAP4_UPLOAD_MAIL_TO_SERVER:
         case EME_TRANS_IMAP4_CREAT_MAILBOX:
         case EME_TRANS_IMAP4_RENAME_MAILBOX:
         case EME_TRANS_IMAP4_DELETE_MAILBOX:
         case EME_TRANS_IMAP4_SUBSCRIBE_MAILBOX:
         case EME_TRANS_IMAP4_UNSUBSCRIBE_MAILBOX:
         case EME_TRANS_IMAP4_LIST_SUBSCRIBED_MAILBOXES:
         case EME_TRANS_IMAP4_LIST_ALL_MAILBOXES:
         case EME_TRANS_IMAP4_EXPUNGE_DEL_MAIL_IN_MAILBOX:
         default:
            iRet = EME_FAILED;
            break;
         }
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_GetNewMailInfo, unknow trans type");
      
         if (pstActRet->iResult == EME_NO_NEWMAIL)
         {
            pstTransEvt->iErrCode = EME_NO_NEWMAIL;
            EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_CLOSE, EME_NO_NEWMAIL, pstTransEvt);
         }
         else
         {
            EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_IMAP, uiSigId, pstTransEvt);
         }
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_FAILED, pstTransEvt);
      }

   } while(0);

   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBIMAP_FetchNewMails   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of fetch one mail from stack is success, 
                      decode email date and store it ,
                      update uid in uid file,
                      notify app NewMail,
                      trans fsm to IMAP_FETCH_NEWMAILS to fetch another new mail
 *              3. if the result cb of fetch one mail from stack is failed,
                      trans fsm to TRANS_END with failed result
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBIMAP_FetchNewMails(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_VOID     *pvMemHdr = NULL;
   WE_VOID     *pvCodec = NULL;
   WE_VOID     *pvStore = NULL;
   WE_VOID     *pvFileMgr = NULL;
   
   EmailOpt    stStoreOpts[EME_STORE_INIT_OPT_NUM];
   EmailOpt    stCodecOpts[EME_CODEC_DECODE_OPT_NUM];
   
   WE_INT32    iAccountId = 0;
   WE_INT32    iDefBoxId = 0;
   WE_CHAR     *pcMailboxName = NULL;
   WE_INT32    iIsHeaderOnly = FALSE;

   WE_UINT32   uiMsgId = 0;
   WE_CHAR     *pcPath = NULL;
   St_CBIMAPDecodeData *pstCBData = NULL;
   
   EME_UTILS_LOG_INFO(("EME_CBIMAP_FetchNewMails---: Start"));
   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_CBIMAP_FetchNewMails--: Bad parameter and return"));
      return EME_FAILED;
   }

   do 
   {
      pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBIMAP_FetchNewMails--:  actret is null");

      if ((pstActRet->iResult == EME_SUCCESS && pstActRet->unRetData.stMail.pvWholeMail != NULL) ||
          (pstActRet->iResult == EME_SIZE_LIMIT_EXCEED && pstActRet->unRetData.stHeader.pvHeader != NULL))
      {
         EME_UTILS_MEMSET(stCodecOpts, 0, sizeof(EmailOpt) * EME_CODEC_DECODE_OPT_NUM);
         EME_UTILS_MEMSET(stStoreOpts, 0, sizeof(EmailOpt) * EME_STORE_INIT_OPT_NUM);

         if (pstActRet->iResult == EME_SIZE_LIMIT_EXCEED)
         {
            iIsHeaderOnly = TRUE;
         }

         /* get file mgr */
//         pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
//         EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_CBIMAP_FetchNewMails--: get file mgr failed");      

         /* get account id and default folder and login mailbox*/
         iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_ACCOUNT_ID, &iAccountId);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchNewMails--:  get account id failed");

         iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_DEFAULT_FOLDER_ID, &iDefBoxId);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchNewMails--:  get default folder id  failed");

         iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_LOGIN_MAILBOXNAME, &pcMailboxName);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchNewMails--:  get login mailbox failed");
         EME_UTILS_BREAKNULL(pcMailboxName, iRet, EME_FAILED, "EME_CBIMAP_FetchNewMails--:  mailbox name failed");

         /* init store and codec handle */
         stStoreOpts[0].iId = EMAILMESSAGE_OPT_ACTID;
         stStoreOpts[0].pVal = (WE_VOID*)iAccountId;
         stStoreOpts[1].iId = EMAIL_OPT_BOXNAME;
         stStoreOpts[1].pVal = (WE_VOID*)pcMailboxName;
         stStoreOpts[2].iId = EMIALMESSAGE_OPT_REC_SERTYPE;
         stStoreOpts[2].pVal = (WE_VOID*)EMAIL_REC_SER_TYPE_IMAP4;
         stStoreOpts[3].iId = EMAILOPT_END;
         iRet = EmailStore_InitHandle(&pvStore, stStoreOpts, pvFileMgr,pvMemHdr);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchNewMails--:  init store handle failed");
         EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_CBIMAP_FetchNewMails--:  init store handle failed");

         iRet = EmailStore_NewMessage(pvStore, &uiMsgId, &pcPath);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchNewMails--: New message failed");
         EME_UTILS_BREAKNULL(pcPath, iRet, EME_FAILED, "EME_CBIMAP_FetchNewMails--: path is null");
         
         iRet = EMECodecLib_InitHandleEx(&pvCodec, pvMemHdr, pvFileMgr, OEMTMLib_GetHCSC(pstTmHandle), pstTmHandle->pvAsyncMgr);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchNewMails--:  init store handle failed");
         EME_UTILS_BREAKNULL(pvCodec, iRet, EME_FAILED, "EME_CBIMAP_FetchNewMails--:  init codec handle failed");
         
         pstCBData = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_CBIMAPDecodeData));
         EME_UTILS_BREAKNULL(pstCBData, iRet, EME_NOMEMORY, "EME_CBIMAP_FetchNewMails--: no memory for cb data");
         EME_UTILS_MEMSET(pstCBData, 0, sizeof(St_CBIMAPDecodeData));

         pstCBData->iDecodingType = EMAIL_DECODETYPE_WHOLEMSG;
         pstCBData->pvTMHandle    = pstTmHandle;
         pstCBData->pvTranEvent   = pstTransEvt;
         pstCBData->pvUserData    = NULL;
         pstCBData->pvStore       = pvStore;
         pstCBData->pvCodec       = pvCodec;
         pstCBData->uiMsgId       = uiMsgId;
         pstCBData->bSpec         = FALSE;


         /* decode mail */
         if (iIsHeaderOnly)
         {
            pstCBData->stActRsult.iResult    = pstActRet->iResult;
            pstCBData->stActRsult.uiMailSize = pstActRet->unRetData.stHeader.uiMailSize;
            pstCBData->stActRsult.uiUID      = pstActRet->unRetData.stHeader.uiUID;
            /* decode mail header */
            stCodecOpts[0].iId = EMAILMESSAGE_OPT_SRC_TYPE;
            stCodecOpts[0].pVal = (WE_VOID*)EMAIL_ENCODE_BYFILE;
            stCodecOpts[1].iId = EMIALMESSAGE_OPT_DECODE_TYPE;
            stCodecOpts[1].pVal = (WE_VOID*)EMAIL_DECODETYPE_HEADER;
            stCodecOpts[2].iId = EMAILMESSAGE_OPT_BUFF;
            stCodecOpts[2].pVal = (WE_VOID*)pstActRet->unRetData.stHeader.pvHeader;
            stCodecOpts[3].iId = EMAILOPT_END;
            iRet = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, NULL, NULL);
            EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchNewMails--:  decoding failed");

            pstCBData->iDecodingType = EMAIL_DECODETYPE_BODYSTRUCT;
            /* decode mail bodystructure */
            stCodecOpts[0].iId = EMAILMESSAGE_OPT_SRC_TYPE;
            stCodecOpts[0].pVal = (WE_VOID*)EMAIL_ENCODE_BYFILE;
            stCodecOpts[1].iId = EMIALMESSAGE_OPT_DECODE_TYPE;
            stCodecOpts[1].pVal = (WE_VOID*)EMAIL_DECODETYPE_BODYSTRUCT;
            stCodecOpts[2].iId = EMAILMESSAGE_OPT_BUFF;
            stCodecOpts[2].pVal = (WE_VOID*)pstActRet->unRetData.stHeader.pvBodystructure;
            stCodecOpts[3].iId = EMAILOPT_END;
            iRet = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, NULL, NULL);
            EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchNewMails--:  decoding failed");

            EME_CBIMAP_DecodingMsgCB(pvCodec, pstCBData, EME_SUCCESS);
         }
         else
         {
            pstCBData->stActRsult.iResult    = pstActRet->iResult;
            pstCBData->stActRsult.uiMailSize = pstActRet->unRetData.stMail.uiMailSize;
            pstCBData->stActRsult.uiUID      = pstActRet->unRetData.stMail.uiUID;
            pstCBData->stActRsult.pcFileName = EME_UTILS_STRDUP(pvMemHdr, (WE_CHAR*)pstActRet->unRetData.stMail.pvWholeMail);
            EME_UTILS_BREAKNULL(pstCBData->stActRsult.pcFileName, iRet, EME_NOMEMORY, "no memory for file name");

            stCodecOpts[0].iId = EMAILMESSAGE_OPT_SRC_TYPE;
            stCodecOpts[0].pVal = (WE_VOID*)EMAIL_ENCODE_BYFILE;
            stCodecOpts[1].iId = EMIALMESSAGE_OPT_DECODE_TYPE;
            stCodecOpts[1].pVal = (WE_VOID*)EMAIL_DECODETYPE_WHOLEMSG;
            stCodecOpts[2].iId = EMAILMESSAGE_OPT_BUFF;
            stCodecOpts[2].pVal = (WE_VOID*)pstCBData->stActRsult.pcFileName;
            stCodecOpts[3].iId = EMAILOPT_END;

            iRet = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, (void*)pstCBData, (PFNDECODINGCB)EME_CBIMAP_DecodingMsgCB);
            EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchNewMails--:  decoding failed");
            
            pstTransEvt->pfnFreeSigData = (PFNFREESIGDATA)EME_CBIMAP_pfnFree;
            pstTransEvt->pvTransData    = pstCBData;
            
//            iRet = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, NULL, NULL);
//            EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchNewMails, decoding failed");
//            EME_CBIMAP_DecodingMsgCB(pvCodec, pstCBData, EME_SUCCESS);
         }
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_FAILED, pstTransEvt);
      }

   } while(0);

   if(iRet != EME_SUCCESS)
   {
      if(NULL != pstCBData)
      {
         EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActRsult.pcFileName);
         EME_UTILS_FREEIF(pvMemHdr, pstCBData);
      }
      EmailStore_DeleteMsg(pvStore, uiMsgId);
      EmailStore_DeleteHandle(pvStore);
      pvStore = NULL;
      EMECodecLib_DeleteHandle(&pvCodec);
   }

   EME_UTILS_LOG_INFO(("EME_CBIMAP_FetchNewMails---: End, Result = %d", iRet));
   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBIMAP_FetchNewHdrs   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of fetch one mail header from stack is success, 
                      decode email date and store it,
                      update uid in uid file,
                      notify app NewMail header downed,
                      trans fsm to IMAP_FETCH_NEWMAILS to fetch another new mail header
 *              3. if the result cb of fetch one mail header from stack is failed,
                      trans fsm to TRANS_END with failed result
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBIMAP_FetchNewHdrs(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_VOID     *pvMemHdr = NULL;
   WE_VOID     *pvCodec = NULL;
   WE_VOID     *pvStore = NULL;
   WE_VOID     *pvFileMgr = NULL;
   EmailOpt    stStoreOpts[EME_STORE_INIT_OPT_NUM];
   EmailOpt    stCodecOpts[EME_CODEC_DECODE_OPT_NUM];

   WE_INT32    iAccountId = 0;
   WE_INT32    iDefBoxId = 0;

   WE_CHAR     acUid[EME_MAX_UINT32_STR_LEN];
   WE_CHAR     *pcMailboxName = NULL;

   WE_CHAR     *pcPath = NULL;
   WE_UINT32   uiMsgId = 0;
   St_CBIMAPDecodeData* pstCBData = NULL;

   EME_UTILS_LOG_INFO(("EME_CBIMAP_FetchNewHdrs---: Start"));
   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_CBIMAP_FetchNewHdrs--: Bad parameter and return"));
      return EME_FAILED;
   }

   do 
   {
      pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBIMAP_FetchNewHdrs--:  actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS && 
          pstActRet->unRetData.stHeader.pvHeader != NULL &&
          pstActRet->unRetData.stHeader.pvBodystructure != NULL)
      {
         EME_UTILS_MEMSET(stCodecOpts, 0, sizeof(EmailOpt) * EME_CODEC_DECODE_OPT_NUM);
         EME_UTILS_MEMSET(stStoreOpts, 0, sizeof(EmailOpt) * EME_STORE_INIT_OPT_NUM); 
         EME_UTILS_MEMSET(acUid, 0, sizeof(WE_CHAR) * EME_MAX_UINT32_STR_LEN);       

         EME_UTILS_SPRINTF(acUid, "%u", pstActRet->unRetData.stHeader.uiUID);
         
         /* get file mgr */
//         pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
//         EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_CBIMAP_FetchNewHdrs--: get file mgr failed");      

         /* get account id and default folder and login mailbox */
         iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_ACCOUNT_ID, &iAccountId);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchNewHdrs--:  get account id failed");

         iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_DEFAULT_FOLDER_ID, &iDefBoxId);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchNewHdrs--:  get default folder id  failed");

         iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_LOGIN_MAILBOXNAME, &pcMailboxName);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchNewHdrs--:  get login mailbox failed");
         EME_UTILS_BREAKNULL(pcMailboxName, iRet, EME_FAILED, "EME_CBIMAP_FetchNewHdrs--:  mailbox name failed");

         /* init store and codec handle */
         stStoreOpts[0].iId = EMAILMESSAGE_OPT_ACTID;
         stStoreOpts[0].pVal = (WE_VOID*)iAccountId;
         stStoreOpts[1].iId = EMAIL_OPT_BOXNAME;
         stStoreOpts[1].pVal = (WE_VOID*)pcMailboxName;
         stStoreOpts[2].iId = EMIALMESSAGE_OPT_REC_SERTYPE;
         stStoreOpts[2].pVal = (WE_VOID*)EMAIL_REC_SER_TYPE_IMAP4;
         stStoreOpts[3].iId = EMAILOPT_END;
         iRet = EmailStore_InitHandle(&pvStore, stStoreOpts, pvFileMgr,pvMemHdr);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchNewHdrs--:  init store handle failed");
         EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_CBIMAP_FetchNewHdrs--:  init store handle failed");

         iRet = EmailStore_NewMessage(pvStore, &uiMsgId, &pcPath);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchNewHdrs--: New message failed");
         EME_UTILS_BREAKNULL(pcPath, iRet, EME_FAILED, "EME_CBIMAP_FetchNewHdrs--: path is null");

         iRet = EMECodecLib_InitHandleEx(&pvCodec, pvMemHdr, pvFileMgr, OEMTMLib_GetHCSC(pstTmHandle), pstTmHandle->pvAsyncMgr);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchNewHdrs--:  init store handle failed");
         EME_UTILS_BREAKNULL(pvCodec, iRet, EME_FAILED, "EME_CBIMAP_FetchNewHdrs--:  init codec handle failed");
         
         pstCBData = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_CBIMAPDecodeData));
         EME_UTILS_BREAKNULL(pstCBData, iRet, EME_NOMEMORY, "EME_CBIMAP_FetchNewHdrs--: no memory for cb data");
         EME_UTILS_MEMSET(pstCBData, 0, sizeof(St_CBIMAPDecodeData));

         pstCBData->iDecodingType = EMAIL_DECODETYPE_HEADER;
         pstCBData->pvTMHandle    = pstTmHandle;
         pstCBData->pvTranEvent   = pstTransEvt;
         pstCBData->pvUserData    = NULL;
         pstCBData->pvStore       = pvStore;
         pstCBData->pvCodec       = pvCodec;
         pstCBData->uiMsgId       = uiMsgId;
         pstCBData->bSpec         = FALSE;

         pstCBData->stActRsult.iResult    = pstActRet->iResult;
         pstCBData->stActRsult.uiMailSize = pstActRet->unRetData.stHeader.uiMailSize;
         pstCBData->stActRsult.uiUID      = pstActRet->unRetData.stHeader.uiUID;

         /* decode mail header */
         stCodecOpts[0].iId = EMAILMESSAGE_OPT_SRC_TYPE;
         stCodecOpts[0].pVal = (WE_VOID*)EMAIL_ENCODE_BYFILE;
         stCodecOpts[1].iId = EMIALMESSAGE_OPT_DECODE_TYPE;
         stCodecOpts[1].pVal = (WE_VOID*)EMAIL_DECODETYPE_HEADER;
         stCodecOpts[2].iId = EMAILMESSAGE_OPT_BUFF;
         stCodecOpts[2].pVal = (WE_VOID*)pstActRet->unRetData.stHeader.pvHeader;
         stCodecOpts[3].iId = EMAILOPT_END;
         iRet = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, NULL, NULL);
         EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActRsult.pcFileName);

         pstCBData->iDecodingType = EMAIL_DECODETYPE_BODYSTRUCT;
         /* decode mail bodystructure */
         stCodecOpts[0].iId = EMAILMESSAGE_OPT_SRC_TYPE;
         stCodecOpts[0].pVal = (WE_VOID*)EMAIL_ENCODE_BYFILE;
         stCodecOpts[1].iId = EMIALMESSAGE_OPT_DECODE_TYPE;
         stCodecOpts[1].pVal = (WE_VOID*)EMAIL_DECODETYPE_BODYSTRUCT;
         stCodecOpts[2].iId = EMAILMESSAGE_OPT_BUFF;
         stCodecOpts[2].pVal = (WE_VOID*)pstActRet->unRetData.stHeader.pvBodystructure;
         stCodecOpts[3].iId = EMAILOPT_END;
         iRet = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, NULL, NULL);

         if(iRet == EME_SUCCESS)
         {
            EME_UTILS_LOG_INFO(("EME_CBIMAP_FetchNewHdrs---: iRet is EME_SUCCESS"));
            EME_CBIMAP_DecodingMsgCB(pvCodec, pstCBData, EME_SUCCESS);
         }
         else
         {
            EME_UTILS_LOG_INFO(("EME_CBIMAP_FetchNewHdrs---: iRet is OTHER"));
             /*fetch other new mail header*/
            EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_FETCH_NEWMAILS, pstTransEvt);
         }
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_FAILED, pstTransEvt);
      }

   } while(0);


   if(EME_SUCCESS != iRet)
   {
      (void)EmailStore_DeleteMsg(pvStore, uiMsgId);
      EmailStore_DeleteHandle(pvStore);
      pvStore = NULL;
      EMECodecLib_DeleteHandle(&pvCodec);
      EME_UTILS_FREEIF(pvMemHdr, pstCBData);
   }

   EME_UTILS_LOG_INFO(("EME_CBIMAP_FetchNewHdrs--:  End, Result = %d", iRet));
   return iRet;
}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBIMAP_FetchSpecMail   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of fetch all parts of one new email from stack is success, 
                      decode the date & update all parts,
                      update msg downstatus,
                      notify app new email downed
                      trans fsm to IMAP_CLOSE
                3. else if the result cb of fetch all parts of one new email from stack is NOTEXSITED,
                   trans fsm to IMAP_CLOSE with the result of NOTEXSITED
 *              4. else trans fsm to TRANS_END with the result of failed
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBIMAP_FetchSpecMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_VOID     *pvMemHdr = NULL;
   WE_VOID     *pvCodec = NULL;
   WE_VOID     *pvStore = NULL;
   WE_VOID     *pvFileMgr = NULL;
   WE_CHAR     *pcMsgId = NULL;
   EmailOpt    stCodecOpts[EME_CODEC_DECODE_OPT_NUM];
   WE_CHAR     acUid[EME_MAX_UINT32_STR_LEN];

   WE_CHAR      *pcPath = NULL;
   WE_UINT32    uiMsgId = 0;
   St_CBIMAPDecodeData *pstCBData = NULL;

   EME_UTILS_LOG_INFO(("EME_CBIMAP_FetchSpecMail---: Start"));
   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_CBIMAP_FetchSpecMail--: Bad parameter and return"));
      return EME_FAILED;
   }

   do 
   {
      pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecMail--:  actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS && 
          pstActRet->unRetData.stMail.pvWholeMail != NULL)
      {
         EME_UTILS_MEMSET(stCodecOpts, 0, sizeof(EmailOpt) * EME_CODEC_DECODE_OPT_NUM);
         EME_UTILS_MEMSET(acUid, 0, sizeof(WE_CHAR) * EME_MAX_UINT32_STR_LEN);       

         /* get file mgr */
 //        pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
 //        EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecMail--: ,get file mgr failed");      

         iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchSpecMail--: , get message id failed");
         EME_UTILS_BREAKNULL(pcMsgId, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecMail--: , get message id failed");
         uiMsgId = EME_UTILS_ATOX(pcMsgId);

         iRet = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchSpecMail--:  init store handle failed");
         EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecMail--:  init store handle failed");

         iRet = EmailStore_GetMsgPath(pvStore, uiMsgId, &pcPath);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchSpecMail--: Get message path failed");
         EME_UTILS_BREAKNULL(pcPath, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecMail--: get message path failed");

         iRet = EMECodecLib_InitHandleEx(&pvCodec, pvMemHdr, pvFileMgr, OEMTMLib_GetHCSC(pstTmHandle), pstTmHandle->pvAsyncMgr);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchSpecMail--: init store handle failed");
         EME_UTILS_BREAKNULL(pvCodec, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecMail--: init codec handle failed");
         
         pstCBData = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_CBIMAPDecodeData));
         EME_UTILS_BREAKNULL(pstCBData, iRet, EME_NOMEMORY, "EME_CBIMAP_FetchSpecMail--: no memory for cb data");
         EME_UTILS_MEMSET(pstCBData, 0, sizeof(St_CBIMAPDecodeData));

         pstCBData->iDecodingType = EMAIL_DECODETYPE_WHOLEMSG;
         pstCBData->pvTMHandle    = pstTmHandle;
         pstCBData->pvTranEvent   = pstTransEvt;
         pstCBData->pvUserData    = NULL;
         pstCBData->pvStore       = pvStore;
         pstCBData->pvCodec       = pvCodec;
         pstCBData->uiMsgId       = uiMsgId;
         pstCBData->bSpec         = TRUE;

         pstCBData->stActRsult.pcFileName = EME_UTILS_STRDUP(pvMemHdr, (WE_CHAR*)pstActRet->unRetData.stMail.pvWholeMail);
         EME_UTILS_BREAKNULL(pstCBData->stActRsult.pcFileName, iRet, EME_NOMEMORY, "no memory for file name");
         pstCBData->stActRsult.uiUID      = pstActRet->unRetData.stMail.uiUID;
         pstCBData->stActRsult.uiMailSize = pstActRet->unRetData.stMail.uiMailSize;
         pstCBData->stActRsult.iResult    = pstActRet->iResult;

         /* decode mail */
         stCodecOpts[0].iId = EMAILMESSAGE_OPT_SRC_TYPE;
         stCodecOpts[0].pVal = (WE_VOID*)EMAIL_ENCODE_BYFILE;
         stCodecOpts[1].iId = EMIALMESSAGE_OPT_DECODE_TYPE;
         stCodecOpts[1].pVal = (WE_VOID*)EMAIL_DECODETYPE_WHOLEMSG;
         stCodecOpts[2].iId = EMAILMESSAGE_OPT_BUFF;
         stCodecOpts[2].pVal = (WE_VOID*)pstCBData->stActRsult.pcFileName;
         stCodecOpts[3].iId = EMAILOPT_END;

         iRet = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, (void*)pstCBData, (PFNDECODINGCB)EME_CBIMAP_DecodingMsgCB);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchSpecMail--:  decoding failed");
         pstTransEvt->pfnFreeSigData = (PFNFREESIGDATA)EME_CBIMAP_pfnFree;
         pstTransEvt->pvTransData    = pstCBData;
         
//            iRet = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, NULL, NULL);
//            EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchNewMails, decoding failed");
//            EME_CBIMAP_DecodingMsgCB(pvCodec, pstCBData, EME_SUCCESS);
      }
      else if (pstActRet->iResult == EME_MAIL_NOTEXSITED)
      {
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_CLOSE, EME_MAIL_NOTEXSITED, pstTransEvt);
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_FAILED, pstTransEvt);
      }

   } while(0);


   if(iRet != EME_SUCCESS)
   {
      if(NULL != pstCBData)
      {
         EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActRsult.pcFileName);
         EME_UTILS_FREEIF(pvMemHdr, pstCBData);
      }
      EmailStore_DeleteHandle(pvStore);
      pvStore = NULL;
      EMECodecLib_DeleteHandle(&pvCodec);
   }
   EME_UTILS_LOG_INFO(("EME_CBIMAP_FetchSpecMail---: End, Result = %d", iRet));
   return iRet;
}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBIMAP_FetchSpecPart   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of fetch one parts of one new email from stack is success, 
                      decode the date & update one parts,
                      notify app new email downed
                      trans fsm to IMAP_CLOSE
                3. else if the result cb of fetch all parts of one new email from stack is NOTEXSITED,
                   trans fsm to IMAP_CLOSE with the result of NOTEXSITED
 *              4. else trans fsm to TRANS_END with the result of failed
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBIMAP_FetchSpecPart(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_VOID     *pvMemHdr = NULL;
   WE_VOID     *pvCodec = NULL;
   WE_VOID     *pvStore = NULL;
   WE_VOID     *pvFileMgr = NULL;
   WE_CHAR     *pcMsgId = NULL;
   EmailOpt    stCodecOpts[EME_CODEC_DECODE_OPT_NUM];
   WE_CHAR     acUid[EME_MAX_UINT32_STR_LEN];
   WE_CHAR     *pcCharSet = NULL;
   WE_CHAR     *pcEncoding = NULL;
   WE_CHAR     *pcPartID = NULL;
   WE_CHAR     *pcMimeType = NULL;
   WE_CHAR     *pcFileName = NULL;
   EmailOpt    *pstMsgPartOpts = NULL;

   WE_UINT32   uiMsgId = 0;
   WE_CHAR     *pcPath = NULL;
   St_CBIMAPDecodeData* pstCBData = NULL;

   EME_UTILS_LOG_INFO(("EME_CBIMAP_FetchSpecPart---: Start"));
   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_CBIMAP_FetchSpecPart--: Bad parameter and return"));
      return EME_FAILED;
   }

   do 
   {
      pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecPart--:  actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS && 
          pstActRet->unRetData.stMailPart.pvMailPart != NULL)
      {
         EME_UTILS_MEMSET(stCodecOpts, 0, sizeof(EmailOpt) * EME_CODEC_DECODE_OPT_NUM);    
         EME_UTILS_MEMSET(acUid, 0, sizeof(WE_CHAR) * EME_MAX_UINT32_STR_LEN);       

         /* get file mgr */
//         pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
//         EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecPart--: get file mgr failed");      

         iRet = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchSpecPart--:  init store handle failed");
         EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecPart--:  init store handle failed");

         iRet = EMECodecLib_InitHandleEx(&pvCodec, pvMemHdr, pvFileMgr, OEMTMLib_GetHCSC(pstTmHandle), pstTmHandle->pvAsyncMgr);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchSpecPart--:  init store handle failed");
         EME_UTILS_BREAKNULL(pvCodec, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecPart--:  init codec handle failed");
         
         /* get message id and part id */
         iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchSpecPart--:  get message id failed");
         EME_UTILS_BREAKNULL(pcMsgId, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecPart--:  get message id failed");
         uiMsgId = EME_UTILS_ATOX(pcMsgId);

         iRet = EmailStore_GetMsgPath(pvStore, uiMsgId, &pcPath);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchSpecPart--: Get message path failed");
         EME_UTILS_BREAKNULL(pcPath, iRet, EME_FAILED, "get message path failed");

         iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_PART_ID, &pcPartID);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchSpecPart--:  get message part id failed");
         EME_UTILS_BREAKNULL(pcPartID, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecPart--:  get message id failed");

         /* get message part opts for getting encoding and charset */
         iRet = EmailStore_GetMsgPart(pvStore, uiMsgId, EME_UTILS_ATOI((const WE_UCHAR*)pcPartID), &pstMsgPartOpts);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchSpecPart--:  get message part failed");
         EME_UTILS_BREAKNULL(pstMsgPartOpts, iRet, EME_FAILED, "EMEEME_CBIMAP_FetchSpecPart--: CBIMAP_FetchSpecMail, get message part failed");

         /* get charset of the message part, it may be NULL if it is attachment */
         EME_GetStrOptValue((EMEOpt*)pstMsgPartOpts, EMAILPARAM_OPT_CHARSET, &pcCharSet);

         /* get encoding of the message part, it may be NULL if it is attachment */
         EME_GetStrOptValue((EMEOpt*)pstMsgPartOpts, EMAILPART_OPT_TRANS_ENCODING, &pcEncoding);

	   /* get MimeType of the message part (modify in 2008.5.30) */
         EME_GetStrOptValue((EMEOpt*)pstMsgPartOpts, EMAILPART_OPT_MIME_TYPE, &pcMimeType);

	   /* get fileName of the message part, it may be NULL if it is not  attachment(modify in 2008.5.30) */
         EME_GetStrOptValue((EMEOpt*)pstMsgPartOpts, EMAILPARAM_OPT_FILE_NAME, &pcFileName);
         pstCBData = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_CBIMAPDecodeData));
         EME_UTILS_BREAKNULL(pstCBData, iRet, EME_NOMEMORY, "EME_CBIMAP_FetchSpecPart--: no memory for cb data");
         EME_UTILS_MEMSET(pstCBData, 0, sizeof(St_CBIMAPDecodeData));

         pstCBData->iDecodingType = EMAIL_DECODETYPE_MSGPART;
         pstCBData->pvTMHandle    = pstTmHandle;
         pstCBData->pvTranEvent   = pstTransEvt;
         pstCBData->pvUserData    = NULL;
         pstCBData->pvStore       = pvStore;
         pstCBData->pvCodec       = pvCodec;
         pstCBData->uiMsgId       = uiMsgId;
         pstCBData->bSpec         = TRUE;

         pstCBData->stActRsult.pcFileName = EME_UTILS_STRDUP(pvMemHdr, (WE_CHAR*)pstActRet->unRetData.stMailPart.pvMailPart);
         EME_UTILS_BREAKNULL(pstCBData->stActRsult.pcFileName, iRet, EME_NOMEMORY, "no memory for file name");

         pstCBData->stActRsult.iResult       = pstActRet->iResult;
         pstCBData->stActRsult.uiMailSize    = pstActRet->unRetData.stMailPart.uiPartSize;
         pstCBData->stActRsult.uiUID         = pstActRet->unRetData.stMailPart.uiUID;

         /* decode mail */
         stCodecOpts[0].iId = EMAILMESSAGE_OPT_SRC_TYPE;
         stCodecOpts[0].pVal = (WE_VOID*)EMAIL_ENCODE_BYFILE;
         stCodecOpts[1].iId = EMIALMESSAGE_OPT_DECODE_TYPE;
         stCodecOpts[1].pVal = (WE_VOID*)EMAIL_DECODETYPE_MSGPART;
         stCodecOpts[2].iId = EMAILMESSAGE_OPT_BUFF;
         stCodecOpts[2].pVal = (WE_VOID*)pstCBData->stActRsult.pcFileName;
         stCodecOpts[3].iId = EMAILPARAM_OPT_CHARSET;
         stCodecOpts[3].pVal = (WE_VOID*)pcCharSet;
         stCodecOpts[4].iId = EMAILPART_OPT_TRANS_ENCODING;
         stCodecOpts[4].pVal = (WE_VOID*)pcEncoding;
         stCodecOpts[5].iId = EMAILPART_OPT_MIME_TYPE;
         stCodecOpts[5].pVal = (WE_VOID*)pcMimeType;     //add it in 2008.5.30
         stCodecOpts[6].iId = EMAILPARAM_OPT_FILE_NAME;
         stCodecOpts[6].pVal = (WE_VOID*)pcFileName;     //add it in 2008.5.30
         stCodecOpts[7].iId = EMAILOPT_END;
         
         iRet = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, (void*)pstCBData, (PFNDECODINGCB)EME_CBIMAP_DecodingMsgCB);
         EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchSpecPart--:  decoding failed");
         pstTransEvt->pfnFreeSigData = (PFNFREESIGDATA)EME_CBIMAP_pfnFree;;
         pstTransEvt->pvTransData    = pstCBData;
      }
      else if (pstActRet->iResult == EME_MAIL_NOTEXSITED)
      {
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_CLOSE, EME_MAIL_NOTEXSITED, pstTransEvt);
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_FAILED, pstTransEvt);
      }

   } while(0);

   if(iRet != EME_SUCCESS)
   {
      if(NULL != pstCBData)
      {
         EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActRsult.pcFileName);
         EME_UTILS_FREEIF(pvMemHdr, pstCBData);
      }
      EmailStore_DeleteHandle(pvStore);
      pvStore = NULL;
      EMECodecLib_DeleteHandle(&pvCodec);
   }

   EME_UTILS_LOG_INFO(("EME_CBIMAP_FetchSpecPart---: End, Result = %d", iRet));
   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBIMAP_HandleMail   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of handle email from stack is success, 
                      trans fsm to IMAP_CLOSE
 *              3. else trans fsm to TRANS_END with the result of failed
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBIMAP_HandleMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBIMAP_HandleMail actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS)
      {
         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_CLOSE, pstTransEvt);
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_FAILED, pstTransEvt);
      }

   } while(0);
   
   return iRet;
}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBIMAP_HandleMailBox   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of handle email box from stack is success, 
                      trans fsm to IMAP_CLOSE
 *              3. else trans fsm to TRANS_END with the result of failed
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBIMAP_HandleMailBox(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_IMAPActRet* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBIMAP_HandleMailBox actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS)
      {
         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_CLOSE, pstTransEvt);
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, EME_FAILED, pstTransEvt);
      }

   } while(0);
   
   return iRet;
}

static void EME_CBIMAP_DecodingMsgCB(IEMSGHandle hEmailHandle, void *pvUserData, int iResult)
{
   WE_VOID   *pvCodec = hEmailHandle;
   WE_UINT32 uiPartsNum = 0;
   St_CBIMAPDecodeData *pstCBData = (St_CBIMAPDecodeData*)pvUserData;
   EmailOpt  *pstTmpOpts = NULL;
   WE_INT32  iOptNum = 0;
   EmailOpt  *pstOneOpt = NULL;
   WE_INT32  iRet = EME_SUCCESS;
   EmailOpt  *pstHeaderOpts = NULL;
   WE_VOID   *pvMemHdr = NULL;
   EmailOpt  *pstBodyOpts = NULL;
   
   WE_CHAR     acUid[EME_MAX_UINT32_STR_LEN];
   WE_CHAR     *pcMailboxName = NULL;

   WE_INT32    iIsHeaderOnly = TRUE;

   St_IMAPActResult  *pstActRet = NULL ;
   WE_UINT32      uiMailSize = 0;
   WE_VOID        *pvStore = NULL;
   St_TMHandle     *pstTmHandle = NULL;
   St_TransEvt     *pstTransEvt = NULL;

   WE_UINT32  i = 0;
   WE_CHAR   acMsgId [9] = {0};
   WE_CHAR   *pcMsgId = NULL;

   WE_UINT32  uiStatus = 0;

   EME_UTILS_LOG_INFO(("EME_CBIMAP_DecodingMsgCB---: Start"));
   if(NULL == pvCodec || NULL == pstCBData)
   {
      EME_UTILS_LOG_ERROR(("EME_CBIMAP_DecodingMsgCB--: Bad parameter and return"));
      return;
   }
   
   do 
   {
      pstTransEvt = (St_TransEvt*)pstCBData->pvTranEvent;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_CBIMAP_DecodingMsgCB--: TranEvent is null");
      pstTransEvt->pfnFreeSigData = NULL;
      pstTransEvt->pvTransData    = NULL;

      pstTmHandle = (St_TMHandle*)pstCBData->pvTMHandle;
      EME_UTILS_BREAKNULL(pstTmHandle, iRet, EME_FAILED, "EME_CBIMAP_DecodingMsgCB--: TM handle is null");
      pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);
      
      pstActRet = (St_IMAPActResult*)&pstCBData->stActRsult;
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBIMAP_DecodingMsgCB--: Act Ret is null");
      
      pvStore = pstCBData->pvStore;
      EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_CBIMAP_DecodingMsgCB--: Store handle is null");
      
      EME_UTILS_MEMSET(acUid, 0, sizeof(WE_CHAR) * EME_MAX_UINT32_STR_LEN);
      
      if(pstCBData->iDecodingType == EMAIL_DECODETYPE_WHOLEMSG)
      {
         if(pstCBData->bSpec)
         {
            /*spec mail*/
            /* get mail's header, parts and store the mail */
            EME_UTILS_BREAKIF(iResult, "EME_CBIMAP_DecodingMsgCB--: Decoding spec mail failed");
            
            iRet = EMECodecLib_GetNumOfParts(pvCodec, &uiPartsNum);
            EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--: get part's num failed");
            
            for (i = 0; i < uiPartsNum; i++)
            {
               iRet = EMECodecLib_GetPart(pvCodec, i, &pstBodyOpts);
               EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--: get part opts failed");
               EME_UTILS_BREAKNULL(pstBodyOpts, iRet, EME_FAILED, "EME_CBIMAP_DecodingMsgCB--: get part opts failed");
               
               iRet = EmailStore_UpdateMsgPart(pvStore, pstCBData->uiMsgId, i,pstBodyOpts);
               EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--:  store part failed");
            }
            EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--:  store part failed");
            /*update msg downstatus*/
            iRet = EmailStore_SetMsgDownStatus(pvStore, pstCBData->uiMsgId, FALSE);
            EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--: set msg down state failed");
            
            if (iRet == EME_SUCCESS)
            {
               /* The specified mail has been downloaded, so cancel was disabled */
               pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;
               
               EME_UTILS_SPRINTF(acUid, "%u", pstActRet->uiUID);
               EME_UTILS_MEMSET(&acMsgId, 0, 9);
               EME_UTILS_SPRINTF(acMsgId, "%x", pstCBData->uiMsgId);
               OEMTMLib_NotifyNewMail(pstTmHandle, pstTransEvt, acMsgId, acUid);
               EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_CLOSE, pstTransEvt);
            }
         }
         else
         {
            do 
            {
               if(EME_SUCCESS == iResult)
               {
                  WE_INT32 iPartType = 0;

                  iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_LOGIN_MAILBOXNAME, &pcMailboxName);
                  EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--: get login mailbox failed");
                  EME_UTILS_BREAKNULL(pcMailboxName, iRet, EME_FAILED, "EME_CBIMAP_DecodingMsgCB--: mailbox name failed");

                  if (pstActRet->iResult == EME_SIZE_LIMIT_EXCEED)
                  {
                     iIsHeaderOnly = TRUE;
                     uiMailSize = pstActRet->uiMailSize;
                     EME_UTILS_SPRINTF(acUid, "%u", pstActRet->uiUID);
                  }
                  else
                  {
                     uiMailSize = pstActRet->uiMailSize;
                     EME_UTILS_SPRINTF(acUid, "%u", pstActRet->uiUID);
                     uiStatus |= EMAIL_STATUS_WITH_BODY;
                  }

                  /* get mail's header, parts and store the mail */
                  iRet = EMECodecLib_GetNumOfParts(pvCodec, &uiPartsNum);
                  EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--:  get part's num failed");

                  for (i = 0; i < uiPartsNum; i++)
                  {
                     iRet = EMECodecLib_GetPart(pvCodec, i, &pstBodyOpts);
                     EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--:  get part opts failed");
                     EME_UTILS_BREAKNULL(pstBodyOpts, iRet, EME_FAILED, "EME_CBIMAP_DecodingMsgCB--:  get part opts failed");

                     iRet = EME_GetIntOptValue((EMEOpt *)pstBodyOpts, EMAILPART_OPT_PART_TYPE, &iPartType);
                     if(iRet == EME_SUCCESS && iPartType == EMAILPART_PARTTYPE_ATTACHMENT)
                     {
                        uiStatus |= EMAIL_STATUS_WITH_ATTACH;
                        break;
                     }
                     pstBodyOpts = NULL;
                  }
                  EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--:get attachment count failed");
                  /*reset body opts and i values*/
                  pstBodyOpts = NULL;
                  i = 0;

                  iRet = EMECodecLib_GetHeader(pvCodec, &pstTmpOpts);
                  EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--:  get header opts failed");
                  EME_UTILS_BREAKNULL(pstTmpOpts, iRet, EME_FAILED, "EME_CBIMAP_DecodingMsgCB--:  get header opts failed");

                  pstOneOpt = pstTmpOpts;
                  while (pstOneOpt->iId != EMAILOPT_END)
                  {
                     iOptNum++;
                     pstOneOpt++;
                  }

                  /* malloc opts, 1 for uidl and 1 for mail size and and 1 for mailbox and 1 for EMAILOPT_END */
                  pstHeaderOpts = (EmailOpt*)EME_UTILS_MALLOC(pvMemHdr, (iOptNum + 6) * sizeof(EmailOpt));
                  EME_UTILS_BREAKNULL(pstHeaderOpts, iRet, EME_NOMEMORY, "EME_CBIMAP_DecodingMsgCB--:  no memory");
                  EME_UTILS_MEMSET(pstHeaderOpts, 0, (iOptNum + 6) * sizeof(EmailOpt));

                  EME_UTILS_MEMCPY(pstHeaderOpts, pstTmpOpts, iOptNum * sizeof(EmailOpt));
                  (pstHeaderOpts + iOptNum)->iId      = EMAIL_OPT_UIDL;
                  (pstHeaderOpts + iOptNum)->pVal     = (WE_VOID*)acUid;
                  (pstHeaderOpts + iOptNum + 1)->iId  = EMAIL_OPT_SIZE;
                  (pstHeaderOpts + iOptNum + 1)->pVal = (WE_VOID*)uiMailSize;
                  (pstHeaderOpts + iOptNum + 2)->iId  = EMAIL_OPT_BOXNAME;
                  (pstHeaderOpts + iOptNum + 2)->pVal = (WE_VOID*)pcMailboxName;
                  (pstHeaderOpts + iOptNum + 3)->iId  = EMAIL_OPT_BEDOWN;
                  (pstHeaderOpts + iOptNum + 3)->pVal = (WE_VOID*)iIsHeaderOnly;
                  (pstHeaderOpts + iOptNum + 4)->iId  = EMAIL_OPT_STATUS;
                  (pstHeaderOpts + iOptNum + 4)->pVal = (WE_VOID*)uiStatus;
                  (pstHeaderOpts + iOptNum + 5)->iId  = EMAILOPT_END;
                  /*add msg header to file*/
                  iRet = EmailStore_AddMsgHeaders(pvStore,  pstCBData->uiMsgId, pstHeaderOpts);
                  EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--:  store header failed");

                  for (i = 0; i < uiPartsNum; i++)
                  {
                     iRet = EMECodecLib_GetPart(pvCodec, i, &pstBodyOpts);
                     EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--:  get part opts failed");
                     EME_UTILS_BREAKNULL(pstBodyOpts, iRet, EME_FAILED, "EME_CBIMAP_DecodingMsgCB--:  get part opts failed");

                     /*add msg part to file*/
                     iRet = EmailStore_AddMsgPart(pvStore, pstCBData->uiMsgId, pstBodyOpts);
                     EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--:  store part failed");
                  }
                  EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--:  store part failed");
               }
               else
               {
                  (void)EmailStore_DeleteMsg(pvStore, pstCBData->uiMsgId);
               }

               /*Reset the result as SUCCESS to continue the next operate*/
               iResult = (EME_NOSTORAGE == iResult) ? EME_NOSTORAGE : EME_SUCCESS;
               EME_UTILS_BREAKIF(iResult, "no storage!!!");
            
               /*register the max uid in uid file*/
               iRet =  EME_IMAP_SaveUID(pstTmHandle->pvIMAP);
               EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--:  update uidl failed");

               /* This mail has been downloaded, so cancel was disabled */
               pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;

               /*update msg downstatus*/
               iRet = EmailStore_SetMsgDownStatus(pvStore, pstCBData->uiMsgId, FALSE);
               if (iRet == EME_SUCCESS)
               {
                  EME_UTILS_MEMSET(&acMsgId, 0, 9);
                  EME_UTILS_SPRINTF(acMsgId, "%x", pstCBData->uiMsgId);
                  pcMsgId = acMsgId;

                  /*notify app there is a new mail downed*/
                  OEMTMLib_NotifyNewMail(pstTmHandle, pstTransEvt, pcMsgId, acUid);
               }
            } while(0);

            if(iRet != EME_SUCCESS)
            {
               (void)EmailStore_DeleteMsg(pvStore, pstCBData->uiMsgId);
            }

            /*Reset the result as SUCCESS to continue the next operate*/
            iResult = (EME_NOSTORAGE == iResult) ? EME_NOSTORAGE : EME_SUCCESS;
            EME_UTILS_BREAKIF(iResult, "no storage!!!");

            /*Reset the result as SUCCESS to continue the next operate*/
            iRet = (EME_NOSTORAGE == iRet) ? EME_NOSTORAGE : EME_SUCCESS;
            EME_UTILS_BREAKIF(iRet, "no storage!!!");

            /*fetch other new mail header*/
            EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_FETCH_NEWMAILS, pstTransEvt);
         }//end if(EMAIL_DECODETYPE_WHOLEMSG)
      }
      else if(pstCBData->iDecodingType == EMAIL_DECODETYPE_HEADER || 
         pstCBData->iDecodingType == EMAIL_DECODETYPE_BODYSTRUCT
         )
       {
          do 
          {
             if(iResult == EME_SUCCESS)
             {
                iIsHeaderOnly = TRUE;

                iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_LOGIN_MAILBOXNAME, &pcMailboxName);
                EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--:  get login mailbox failed");
                EME_UTILS_BREAKNULL(pcMailboxName, iRet, EME_FAILED, "EME_CBIMAP_DecodingMsgCB--:  mailbox name failed");

                if (pstActRet->iResult == EME_SIZE_LIMIT_EXCEED)
                {
                   uiMailSize = pstActRet->uiMailSize;
                   EME_UTILS_SPRINTF(acUid, "%u", pstActRet->uiUID);
                }
                else
                {
                   uiMailSize = pstActRet->uiMailSize;
                   EME_UTILS_SPRINTF(acUid, "%u", pstActRet->uiUID);
                }

                /* get mail's body structure parts */
                iRet = EMECodecLib_GetNumOfParts(pvCodec, &uiPartsNum);
                EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--:  get part's num failed");

                iRet = EMECodecLib_GetHeader(pvCodec, &pstTmpOpts);
                EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--:  get header opts failed");
                EME_UTILS_BREAKNULL(pstTmpOpts, iRet, EME_FAILED, "EME_CBIMAP_DecodingMsgCB--:  get header opts failed");

                pstOneOpt = pstTmpOpts;
                while (pstOneOpt->iId != EMAILOPT_END)
                {
                   iOptNum++;
                   pstOneOpt++;
                }

                /* malloc opts, 1 for uidl and 1 for mail size and and 1 for mailbox and 1 for EMAILOPT_END */
                pstHeaderOpts = (EmailOpt*)EME_UTILS_MALLOC(pvMemHdr, (iOptNum + 6) * sizeof(EmailOpt));
                EME_UTILS_BREAKNULL(pstHeaderOpts, iRet, EME_NOMEMORY, "EME_CBIMAP_DecodingMsgCB--:  no memory");
                EME_UTILS_MEMSET(pstHeaderOpts, 0, (iOptNum + 6) * sizeof(EmailOpt));

                EME_UTILS_MEMCPY(pstHeaderOpts, pstTmpOpts, iOptNum * sizeof(EmailOpt));
                (pstHeaderOpts + iOptNum)->iId        = EMAIL_OPT_UIDL;
                (pstHeaderOpts + iOptNum)->pVal       = (WE_VOID*)acUid;
                (pstHeaderOpts + iOptNum + 1)->iId    = EMAIL_OPT_SIZE;
                (pstHeaderOpts + iOptNum + 1)->pVal   = (WE_VOID*)uiMailSize;
                (pstHeaderOpts + iOptNum + 2)->iId    = EMAIL_OPT_BOXNAME;
                (pstHeaderOpts + iOptNum + 2)->pVal   = (WE_VOID*)pcMailboxName;
                (pstHeaderOpts + iOptNum + 3)->iId    = EMAIL_OPT_BEDOWN;
                (pstHeaderOpts + iOptNum + 3)->pVal   = (WE_VOID*)iIsHeaderOnly;
                (pstHeaderOpts + iOptNum + 4)->iId    = EMAIL_OPT_STATUS;
                (pstHeaderOpts + iOptNum + 4)->pVal   = (WE_VOID*)uiStatus;
                (pstHeaderOpts + iOptNum + 5)->iId    = EMAILOPT_END;
                /*add msg header to file*/
                iRet = EmailStore_AddMsgHeaders(pvStore,  pstCBData->uiMsgId, pstHeaderOpts);
                EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--: , store header failed");

                for (i = 0; i < uiPartsNum; i++)
                {
                   iRet = EMECodecLib_GetPart(pvCodec, i, &pstBodyOpts);
                   EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--: , get part opts failed");
                   EME_UTILS_BREAKNULL(pstBodyOpts, iRet, EME_FAILED, "EME_CBIMAP_DecodingMsgCB--: , get part opts failed");

                   /*add msg part to file*/
                   iRet = EmailStore_AddMsgPart(pvStore, pstCBData->uiMsgId, pstBodyOpts);
                   EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--: , store part failed");
                }
                EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--: , store part failed");
             }
             else
             {
                (void)EmailStore_DeleteMsg(pvStore, pstCBData->uiMsgId);
             }
             
             iResult =(iResult == EME_NOSTORAGE) ? EME_NOSTORAGE : EME_SUCCESS;
             EME_UTILS_BREAKIF(iResult, "no storage");
             
             /*register the max uid in uid file*/
             iRet =  EME_IMAP_SaveUID(pstTmHandle->pvIMAP);
             EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--: , update uidl failed");

             if (iRet == EME_SUCCESS)
             {
                /* This mail has been downloaded, so cancel was disabled */
                pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;

                EME_UTILS_MEMSET(&acMsgId, 0, 9);
                EME_UTILS_SPRINTF(acMsgId, "%x", pstCBData->uiMsgId);
                pcMsgId = acMsgId;

                /*notify app there is a new mail downed*/
                OEMTMLib_NotifyNewMail(pstTmHandle, pstTransEvt, pcMsgId, acUid);
             }
          } while(0);

          if(iRet != EME_SUCCESS)
          {
            (void)EmailStore_DeleteMsg(pvStore, pstCBData->uiMsgId);
          }

          iRet = (EME_NOSTORAGE == iRet) ? EME_NOSTORAGE : EME_SUCCESS;
          EME_UTILS_BREAKIF(iRet, "no storage!!!");

          iResult = (EME_NOSTORAGE == iResult) ? EME_NOSTORAGE : EME_SUCCESS;
          EME_UTILS_BREAKIF(iResult, "no storage!!!");
            
          /*featch another new mail header*/
          EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_FETCH_NEWHDRS, pstTransEvt);
       }
       else if(pstCBData->iDecodingType == EMAIL_DECODETYPE_MSGPART)
       {
          if(iResult == EME_SUCCESS)
          {
             WE_CHAR* pcPartDataOfCodec = NULL;
             WE_INT32 iBufferSize = 0;
             EmailOpt    pstMsgPartOptsForUpdate[EME_IMAP_UPDATE_PART_NUM];
             WE_CHAR* pcPartID = NULL;
             WE_INT32     iSrcType  = 0;//Add in 5.30.2008
             WE_INT32     iPartType = 0;//Add in 5.30.2008
             WE_CHAR       *pcAttName = NULL;

             EME_UTILS_BREAKIF(iResult, "Decoding part failed");
             EME_UTILS_MEMSET(&pstMsgPartOptsForUpdate, 0, sizeof(EmailOpt) * EME_IMAP_UPDATE_PART_NUM);

             iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_PART_ID, &pcPartID);
             EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--: , get message part id failed");
             EME_UTILS_BREAKNULL(pcPartID, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecMail, get message id failed");

             /* get mail's header, parts and store the mail */
             iRet = EMECodecLib_GetNumOfParts(pvCodec, &uiPartsNum);
             EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--: , get part's num failed");

             for (i = 0; i < uiPartsNum; i++)
             {
                iRet = EMECodecLib_GetPart(pvCodec, i, &pstBodyOpts);
                EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--: , get part opts failed");
                EME_UTILS_BREAKNULL(pstBodyOpts, iRet, EME_FAILED, "EME_CBIMAP_DecodingMsgCB--: , get part opts failed");

                /* get part data buffer from codec */
                iRet = EME_GetStrOptValue((EMEOpt*)pstBodyOpts, EMAILPART_OPT_DATA_BUFF, &pcPartDataOfCodec);
                EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--: , get message part from codec failed");
                EME_UTILS_BREAKNULL(pcPartDataOfCodec, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecMail, get part data buffer failed");

                /* get part data size from codec */
                iRet = EME_GetIntOptValue((EMEOpt*)pstBodyOpts, EMAILPART_OPT_DATA_SIZE, &iBufferSize);
                EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--: , get message part size from codec failed");
                iRet = EME_GetIntOptValue((EMEOpt*)pstBodyOpts, EMAILPART_OPT_SRC_TYPE,&iSrcType);
                EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--: , get message part srctype from codec failed");
                
                /* get part Part Type from Codec (2008.5.30) */
                iRet = EME_GetIntOptValue((EMEOpt*)pstBodyOpts, EMAILPART_OPT_PART_TYPE,&iPartType);
                EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--: , get message part type from codec failed");

                if (EMAILPART_PARTTYPE_ATTACHMENT == iPartType)
                {
                   /* get part Part file name from Codec (2008.5.30) */
                   iRet = EME_GetStrOptValue((EMEOpt*)pstBodyOpts, EMAILPARAM_OPT_FILE_NAME, &pcAttName);
                   EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--: , get message part attachment name from codec failed");
                }

                pstOneOpt = pstBodyOpts;
                while (pstOneOpt->iId != EMAILOPT_END)
                {
                   iOptNum++;
                   pstOneOpt++;
                }

                /* malloc opts, 1 for data buffer and 1 for EMAILOPT_END */
                pstMsgPartOptsForUpdate[0].iId = EMAILPART_OPT_DATA_BUFF;
                pstMsgPartOptsForUpdate[0].pVal = (WE_VOID*)pcPartDataOfCodec;
                pstMsgPartOptsForUpdate[1].iId = EMAILPART_OPT_DATA_SIZE;
                pstMsgPartOptsForUpdate[1].pVal = (WE_VOID*)iBufferSize;
                pstMsgPartOptsForUpdate[2].iId = EMAILPART_OPT_SRC_TYPE;
                pstMsgPartOptsForUpdate[2].pVal = (WE_VOID*)iSrcType;
                pstMsgPartOptsForUpdate[3].iId = EMAILPART_OPT_PART_TYPE;
                pstMsgPartOptsForUpdate[3].pVal = (WE_VOID*)iPartType;
                pstMsgPartOptsForUpdate[4].iId = EMAILPARAM_OPT_FILE_NAME;
                pstMsgPartOptsForUpdate[4].pVal = (WE_VOID*)pcAttName;
                pstMsgPartOptsForUpdate[5].iId = EMAILOPT_END;

                iRet = EmailStore_UpdateMsgPart(pvStore, pstCBData->uiMsgId, EME_UTILS_ATOI((const WE_UCHAR*)pcPartID), pstMsgPartOptsForUpdate);
                EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_DecodingMsgCB--:  store part failed");
             }

             if (iRet == EME_SUCCESS)
             {
                /* The specified part has been downloaded, so cancel was disabled */
                pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;

                EME_UTILS_SPRINTF(acUid, "%u", pstActRet->uiUID);
                //OEMTMLib_NotifyNewMail(pstTmHandle, pstTransEvt, pcMsgId, acUid);
                EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_CLOSE, pstTransEvt);
             }
          }
          else
          {
            EmailStore_WindUp(pvStore, pstCBData->uiMsgId);
          }
          iResult = EME_SUCCESS;
       }
   } while(0);
   
   if(EME_SUCCESS != iRet || EME_SUCCESS != iResult)
   {
      /*delete the mail message if failed, except get mail part or spceify mail*/
      if(pstCBData->iDecodingType == EMAIL_DECODETYPE_WHOLEMSG && !pstCBData->bSpec ||
         pstCBData->iDecodingType == EMAIL_DECODETYPE_BODYSTRUCT ||
         pstCBData->iDecodingType == EMAIL_DECODETYPE_HEADER)
      {
         EmailStore_DeleteMsg(pvStore, pstCBData->uiMsgId);
      }
      else
      {
         EmailStore_WindUp(pvStore, pstCBData->uiMsgId);
      }

      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }

      if(EME_SUCCESS != iResult)
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, iResult, pstTransEvt);
      else
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_IMAP, EME_SIG_IMAP_TRANS_END, iRet, pstTransEvt);
   }

   EmailStore_DeleteHandle(pvStore);
   pvStore = NULL;
   EMECodecLib_DeleteHandle(&pvCodec);
   if(NULL != pstCBData)
   {
      EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActRsult.pcFileName);
      EME_UTILS_FREEIF(pvMemHdr, pstCBData);
   }
   EME_UTILS_FREEIF(pvMemHdr, pstHeaderOpts);

   EME_UTILS_LOG_INFO(("EME_CBIMAP_DecodingMsgCB---: End, Result = %d", SUCCESS));
}


static void  EME_CBIMAP_pfnFree(St_TMHandle *pstTMHandle, void* pvTransEvt)
{
   St_CBIMAPDecodeData *pstCBData = NULL;
   St_TransEvt     *pstTransEvt = (St_TransEvt*)pvTransEvt;
   WE_INT32        iRet = EME_SUCCESS;
   void            *pvMemHdr = NULL;

   EME_UTILS_LOG_INFO(("EME_CBIMAP_pfnFree---: Start"));
   if(NULL == pstTMHandle || NULL == pvTransEvt)
   {
      EME_UTILS_LOG_ERROR(("EME_CBIMAP_pfnFree--: Bad parameter and return"));
      return;
   }

   do 
   {
   	pstCBData = (St_CBIMAPDecodeData*)pstTransEvt->pvTransData;
      EME_UTILS_BREAKNULL(pstCBData, iRet, EME_FAILED, "EME_CBIMAP_pfnFree--: pstCBData is null");
      pvMemHdr = EME_OEMTMLib_GetMemHdr(pstCBData->pvTMHandle);

      if(pstCBData->iDecodingType == EMAIL_DECODETYPE_WHOLEMSG && !pstCBData->bSpec ||
         pstCBData->iDecodingType == EMAIL_DECODETYPE_BODYSTRUCT ||
         pstCBData->iDecodingType == EMAIL_DECODETYPE_HEADER)
      {
         EmailStore_DeleteMsg(pstCBData->pvStore, pstCBData->uiMsgId);
      }
      else
      {
         EmailStore_WindUp(pstCBData->pvStore, pstCBData->uiMsgId);
      }

      EmailStore_DeleteHandle(pstCBData->pvStore);
      EMECodecLib_DeleteHandle(&pstCBData->pvCodec);
      EME_UTILS_FREEIF(pvMemHdr, pstCBData);
   } while(0);

   EME_UTILS_LOG_INFO(("EME_CBIMAP_pfnFree---: End, Result = %d", SUCCESS));
}