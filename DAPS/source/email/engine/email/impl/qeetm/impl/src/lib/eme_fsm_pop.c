/*=====================================================================================
FILE NAME   : eme_fsm_pop.c
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
    2007-03-02     Jiang Dingqing                  create eme_fsm_pop.c
          
=====================================================================================*/

//pclint
/*lint -e506 */

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
#include "eme_pop_api.h"
#include "eme_tm_utils_api.h"

#include "eme_fsm_pop_api.h"

#include "eme_file_api.h"

#include "EMS_Store_Priv.h"

extern WE_INT32 EME_UpdateOpt(EmailOpt *pstAllOpts, WE_INT32 iId, WE_VOID* pvVal, WE_VOID** ppvOrigVal);

/*Include Private head file*/

/*The header file of dialing*/
//#include "oem_ppp_api.h"


/*=============================================================================
* 
*     DEFINITIONS AND CONSTANTS(It is not recommended)
* It is recommended that this section is defined to private head file.
* 
=============================================================================*/
typedef struct _St_POPActResult
{
   WE_CHAR *pcFetchedID;
   WE_CHAR *pcDataBuffer;
   WE_INT32 iFetchSize;
}St_PopActResult;

typedef struct _St_CBDecodeData
{
   void      *pvUserData;
   void      *pvTMHandle;
   void      *pvTranEvent;
   void      *pvStore;
   void      *pvCodec;
   St_PopActResult stActResult;
   WE_UINT32 uiMsgId;
   WE_INT32  iDecodingType;
   WE_BOOL   bSpec;
   WE_CHAR   cRes[3];
}St_CBDecodeData;


/*=============================================================================
*
*   Static Function Prototype Declare Section
*
=============================================================================*/
/* sig handle functions */
static WE_INT32 EME_FSMPOP_TransStart(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMPOP_Connection(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMPOP_FetchNew(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMPOP_UnkeepMail(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMPOP_FetchSpec(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMPOP_DeleteAllMail(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMPOP_DeleteSpecMail(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMPOP_Quit(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMPOP_TransEnd(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);

static WE_INT32 EME_FSMEPOP_Prof(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMEPOP_FetchNewMail(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMEPOP_FetchNewHeaders(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMEPOP_FetchSpecMail(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);
static WE_INT32 EME_FSMEPOP_FetchSpecPart(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);

//static WE_INT32 EME_FSMPOP_Cancel(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);

/* stack callback functions */
static WE_INT32 EME_CBPOP_ConnectionInit(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet);
static WE_INT32 EME_CBPOP_Login(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet);
static WE_INT32 EME_CBPOP_GetInfo(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet);
static WE_INT32 EME_CBPOP_FetchNewMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet);
static WE_INT32 EME_CBPOP_FetchNewHdr(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet);
static WE_INT32 EME_CBPOP_FetchSpecMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet);
static WE_INT32 EME_CBPOP_FetchSpecHdr(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet);
static WE_INT32 EME_CBPOP_DeleteAllMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet);
static WE_INT32 EME_CBPOP_DeleteSpecMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet);
static WE_INT32 EME_CBPOP_Quit(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet);

static void EME_CBPOP_DecodingMsgCB(IEMSGHandle hEmailHandle, void *pvUserData, int iResult);
static void EME_CBEPOP_DecodingMsgCB(IEMSGHandle hEmailHandle, void *pvUserData, int iResult);

static void  EME_CBPOP_pfnFree(St_TMHandle *pstTMHandle, void* pvTransEvt);
static WE_INT32 EME_CBEPOP_Prof(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet);
static WE_INT32 EME_CBEPOP_FetchNewMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet);
static WE_INT32 EME_CBEPOP_FetchNewHeaders(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet);
static WE_INT32 EME_CBEPOP_FetchSpecMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet);
static WE_INT32 EME_CBEPOP_FetchSpecPart(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet);
static WE_INT32 EME_CBEPOP_SetPushMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet);
static WE_INT32 EME_CBEPOP_GetPushMailFilter(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet);
/* Free functions */
//static WE_INT32 EME_FREEPOP_MailBuf(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt);
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
 *    FUNCTION    EME_HandlePOPSig   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. dispatch signals to proper FSM.
 *              3. delete the signal after it is handled.
 *  
*    ================================================================*/
WE_INT32 EME_HandlePOPSig(St_TMHandle *pstTmHandle, WE_UINT32 uiEvtCode, WE_UINT16 usWParam, WE_UINT32 ulDwParam)
{
   WE_INT32    iRet = EME_SUCCESS;

   if (pstTmHandle == NULL)
   {
      return EME_FAILED;
   }

   switch(uiEvtCode)
   {
   case EME_SIG_POP_TRANS_START:
      iRet = EME_FSMPOP_TransStart(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;
   case EME_SIG_POP_CONNECTION:
      iRet = EME_FSMPOP_Connection(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;
//    case EME_SIG_POP_LOGIN:
//       iRet = EME_FSMPOP_Login(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
//       break;
//    case EME_SIG_POP_GETINFO:    
//       iRet = EME_FSMPOP_Getinfo(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
//       break;
   case EME_SIG_POP_FETCH_NEWMAILS:
   case EME_SIG_POP_FETCH_NEWHDRS:
      iRet = EME_FSMPOP_FetchNew(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;

   case EME_SIG_POP_UNKEEP_MAIL:
      iRet = EME_FSMPOP_UnkeepMail(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;

   case EME_SIG_POP_FETCH_SPECMAIL:    
   case EME_SIG_POP_FETCH_SPECHDR:
      iRet = EME_FSMPOP_FetchSpec(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;
      
   case EME_SIG_POP_DELETE_ALLMAIL:  
      iRet = EME_FSMPOP_DeleteAllMail(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;
   case EME_SIG_POP_DELETE_SPECMAIL:   
      iRet = EME_FSMPOP_DeleteSpecMail(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;
   case EME_SIG_POP_QUIT:
      iRet = EME_FSMPOP_Quit(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;
   case EME_SIG_POP_TRANS_END:
      iRet = EME_FSMPOP_TransEnd(pstTmHandle, (St_EMEFsmSigData*)ulDwParam);
      break;
	case EME_SIG_EPOP_FETCH_PROF:
		iRet = EME_FSMEPOP_Prof(pstTmHandle, (St_EMEFsmSigData *)ulDwParam);
	   break;
	case EME_SIG_EPOP_FETCH_NEWHDRS:
		iRet = EME_FSMEPOP_FetchNewHeaders(pstTmHandle, (St_EMEFsmSigData *)ulDwParam);
		break;
	case EME_SIG_EPOP_FETCH_NEWMAILS:
		iRet = EME_FSMEPOP_FetchNewMail(pstTmHandle, (St_EMEFsmSigData *)ulDwParam);
		break;
	case EME_SIG_EPOP_FETCH_SPECMAIL:
		iRet = EME_FSMEPOP_FetchSpecMail(pstTmHandle, (St_EMEFsmSigData *)ulDwParam);
		break;
	case EME_SIG_EPOP_FETCH_SPECPART:
		iRet = EME_FSMEPOP_FetchSpecPart(pstTmHandle, (St_EMEFsmSigData *)ulDwParam);
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
 *              2. if the pop state is login, trans fsm to pop operation fsm,
                   else select network,trans fsm to POP_CONNECTION
 *              3. if there is something wrong,trans fsm to TRANS_END
 *  
*    =========================================================================*/
static WE_INT32 EME_FSMPOP_TransStart(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   WE_UINT32      uiSigId = 0;
  // WE_INT32       iPDP = 0;


   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;   
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMPOP_TransStart, trans evt error");

      pstTmHandle->iCanBeCancel = EME_CANCEL_ENABLED;

      if(!EME_DataAccess_IsEnoughSpace(EME_OEMTMLib_GetIFileMgr(pstTmHandle), EMS_LEAST_EML_SPACE))
      {
         iRet = EME_NOSTORAGE;
         EME_UTILS_BREAKIF(iRet, "no storeage for new mail");
      }

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_POP_TRANS_START;
      
      if (pstTmHandle->iPOPState == EME_STACK_STATE_LOGIN)
      {
         switch(pstTransEvt->uiTransType)
         {
         case EME_TRANS_POP3_FETCH_NEW_MAILS_HEADER:
            uiSigId = EME_SIG_POP_FETCH_NEWHDRS;
            break;
         case EME_TRANS_POP3_FETCH_NEW_MAILS:
            uiSigId = EME_SIG_POP_FETCH_NEWMAILS;
            break;
         case EME_TRANS_POP3_FETCH_SPEC_MAIL:
            uiSigId = EME_SIG_POP_FETCH_SPECMAIL;
            break;
//          case EME_TRANS_POP3_FETCH_SPEC_MAIL_HEADER:
//             uiSigId = EME_SIG_POP_FETCH_SPECHDR;
//             break;
         case EME_TRANS_POP3_DELETE_SPEC_MAIL:
            uiSigId = EME_SIG_POP_DELETE_SPECMAIL;
            break;
         case EME_TRANS_POP3_DELETE_ALL_MAIL:
            uiSigId = EME_SIG_POP_DELETE_ALLMAIL;
            break;
         default:
            iRet = EME_FAILED;
            break;
         }
         EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_TransStart, unknow trans type");

         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, uiSigId, (WE_VOID*)pstTransEvt);
      }
      else
      {
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
//         EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_TransStart, select network failed");
         
         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_CONNECTION, (WE_VOID*)pstTransEvt);
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
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, iRet, pstTransEvt);
   }

   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMPOP_Connection   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. connec to pop server
 *              3. if there is something wrong,trans fsm to TRANS_END
 *  
*    ===================================================================================================*/

static WE_INT32 EME_FSMPOP_Connection(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   IPOPHANDLER    pvPOP = NULL;
   St_POP_InitConn stConfigInfo;
   WE_INT32       iPort = 0;

   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }
   
   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);
   
   do 
   {
      EME_UTILS_LOG_INFO(("EME_FSMPOP_Connection : START!!"));
      EME_UTILS_MEMSET(&stConfigInfo, 0, sizeof(St_POP_InitConn));
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;   
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMPOP_Connection, trans evt error");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_POP_CONNECTION;

      /* get pop handle */
      pvPOP = pstTmHandle->pvPOP;
      EME_UTILS_BREAKNULL(pvPOP, iRet, EME_FAILED, "EME_FSMPOP_Connection, pop handle error");

      /*get account cfg info*/
      iRet = EME_GetStrOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_PASSWORD, &stConfigInfo.pcPassWord);
      EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_Connection, get password failed");
      EME_UTILS_BREAKNULL(stConfigInfo.pcPassWord, iRet, EME_FAILED, "EME_FSMPOP_Connection, get password failed");
      
      iRet = EME_GetStrOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_USERNAME, &stConfigInfo.pcUserName);
      EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_Connection, get username failed");
      EME_UTILS_BREAKNULL(stConfigInfo.pcUserName, iRet, EME_FAILED, "EME_FSMPOP_Connection, get username failed");

      iRet = EME_GetStrOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_SERVER_ADDR, &stConfigInfo.pcServerAdd);
      EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_Connection, get ser address failed");
      EME_UTILS_BREAKNULL(stConfigInfo.pcServerAdd, iRet, EME_FAILED, "EME_FSMPOP_Connection, get ser address failed");

      iRet = EME_GetStrOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_UIDL_FILE_POP, &stConfigInfo.pcUidlPath);
      EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_Connection, get uidl path failed");
      EME_UTILS_BREAKNULL(stConfigInfo.pcUidlPath, iRet, EME_FAILED, "EME_FSMPOP_Connection, get ser address failed");

      iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_SERVER_PORT, &iPort);
      EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_Connection, get port failed");
      stConfigInfo.usPort = (WE_UINT16)iPort;

      iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_POP_NEED_SSL, &stConfigInfo.iIsNeedSSL);
      EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_Connection, get ssl opt failed");

      iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_NUM_LIMIT, &stConfigInfo.iNumLimit);
      EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_Connection, get number limit failed");

      iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_SIZE_LIMIT, &stConfigInfo.iSizeLimit);
      EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_Connection, get size limit failed");

      EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_LATEST, &stConfigInfo.iLatest);

      /* get using APOP or not */
      stConfigInfo.bAuth = FALSE;

      /* record pop callback data */
      pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;

      /* pop Connection */
      iRet = EME_POP_InitConnection(pvPOP, &stConfigInfo);
      EME_UTILS_BREAKIF(iRet, "EME_CBPOP_Connection, pop init connection failed");
      
      /* register free function */
      pstTransEvt->pfnFreeSigData = NULL;

   } while(0);

   if (iRet != EME_SUCCESS)
   {
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, iRet, pstTransEvt);
   }   

   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMPOP_FetchNew   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. fech all new mail or fech all new header base on TransType
 *              3. if there is something wrong,trans fsm to TRANS_END
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMPOP_FetchNew(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   WE_INT32    iRet = EME_SUCCESS;
   IPOPHANDLER pvPOP = NULL;
   St_TransEvt *pstTransEvt = NULL;
   WE_VOID     *pvMemHdr = NULL;
   
   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMPOP_FetchNew, transEvt is null");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_POP_FETCH_NEWMAILS;

      /* get pop handle */
      pvPOP = pstTmHandle->pvPOP;
      EME_UTILS_BREAKNULL(pvPOP, iRet, EME_FAILED, "EME_FSMPOP_FetchNew, pop handle is null");

      /* record pop callback data */
      pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;

      if (pstTransEvt->uiTransType == EME_TRANS_POP3_FETCH_NEW_MAILS)
      {
         iRet = EME_POP_FetchAllMails(pvPOP);
      }
      else
      {
         iRet = EME_POP_FetchAllMailHeaders(pvPOP);
      }
      EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_FetchNew, fetch all failed");

       pstTmHandle->iCanBeCancel = EME_CANCEL_ENABLED;

   }while(0);


   if (iRet != EME_SUCCESS)
   {  
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, iRet, pstTransEvt);
   }

   return iRet;

}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMPOP_UnkeepMail   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. delete Sepcified mail by uidl  
 *              3. if there is something wrong,trans fsm to TRANS_END
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMPOP_UnkeepMail(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   WE_INT32    iRet = EME_SUCCESS;
   IPOPHANDLER pvPOP = NULL;
   St_TransEvt *pstTransEvt = NULL;
   
   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMPOP_DeleteSpecMsg, transEvt is null");
      
      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_POP_UNKEEP_MAIL;
      
      /* get pop handle */
      pvPOP = pstTmHandle->pvPOP;
      EME_UTILS_BREAKNULL(pvPOP, iRet, EME_FAILED, "EME_FSMPOP_DeleteSpecMsg, pop handle is null");
      
      /* record pop callback data */
      pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;
      
      iRet = EME_POP_DeleteSepcifiedMail(pvPOP, pstTransEvt->pcUIDL);
      EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_DeleteSpecMsg, fetch specified failed");

   } while(0);

   if (iRet != EME_SUCCESS)
   {  
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, iRet, pstTransEvt);
   }
   
   return iRet;
}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMPOP_FetchSpec   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. get spec uidl by msg id from storage,
                   then fetch spec mail or mail head by transtype
 *              3. if there is something wrong,trans fsm to TRANS_END
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMPOP_FetchSpec(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   WE_INT32    iRet = EME_SUCCESS;
   IPOPHANDLER pvPOP = NULL;
   St_TransEvt *pstTransEvt = NULL;
   WE_VOID     *pvMemHdr = NULL;
   WE_CHAR     *pcMsgUidl = NULL;
   WE_CHAR     *pcMsgId =NULL;
   WE_VOID     *pvStore = NULL;
   WE_VOID     *pvFileMgr = NULL;
   EmailOpt    *pstHeaderOpts = NULL;

   WE_UINT32   uiMsgId = 0;

   
   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMPOP_FetchSpec, transEvt is null");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_POP_FETCH_SPECMAIL;

      /* get pop handle */
      pvPOP = pstTmHandle->pvPOP;
      EME_UTILS_BREAKNULL(pvPOP, iRet, EME_FAILED, "EME_FSMPOP_FetchSpec, pop handle is null");
  
      /* get file mgr */
//      pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
//      EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_CBPOP_FetchSpec,get file mgr failed");      

      /* get message id */
      iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
      EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_FetchSpec, get message uidl failed");
      EME_UTILS_BREAKNULL(pcMsgId, iRet, EME_FAILED, "EME_FSMPOP_FetchSpec, get message uidl failed");

      uiMsgId = EME_UTILS_ATOX(pcMsgId);

      /*here can be deleted ,for get msg does not need account id or defboxid.by echo*/
      /* get account id and default folder */

      iRet = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
      EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpec, init store handle failed");
      EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_CBPOP_FetchSpec, init store handle failed");
      
      iRet = EmailStore_GetMsgHeaders(pvStore, uiMsgId, &pstHeaderOpts);
      EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpec, get message header failed");
      EME_UTILS_BREAKNULL(pstHeaderOpts, iRet, EME_FAILED, "EME_CBPOP_FetchSpec, get message header failed");
      
      iRet = EME_GetStrOptValue((EMEOpt*)pstHeaderOpts, EMAIL_OPT_UIDL, &pcMsgUidl);
      EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpec, get UIDL failed");
      EME_UTILS_BREAKNULL(pcMsgUidl, iRet, EME_FAILED, "EME_CBPOP_FetchSpec, get UIDL failed");
            
      /* record pop callback data */
      pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;

      if (pstTransEvt->uiTransType == EME_TRANS_POP3_FETCH_SPEC_MAIL)
      {
         iRet = EME_POP_FetchSpecifiedMail(pvPOP, pcMsgUidl);
      }
      else
      {
         iRet = EME_POP_FetchSpecifiedMailHeader(pvPOP, pcMsgUidl);
      }
      EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_FetchSpec, fetch specified failed");

   }while(0);


   EmailStore_DeleteHandle(pvStore);

   if (iRet != EME_SUCCESS)
   {  
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, iRet, pstTransEvt);
   }

   return iRet;
}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMPOP_DeleteAllMail   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. dele all mails
 *              3. if there is something wrong,trans fsm to TRANS_END
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMPOP_DeleteAllMail(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   WE_INT32    iRet = EME_SUCCESS;
   IPOPHANDLER pvPOP = NULL;
   St_TransEvt *pstTransEvt = NULL;
   WE_VOID     *pvMemHdr = NULL;
   
   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMPOP_DeleteAllMail, transEvt is null");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_POP_DELETE_ALLMAIL;

      /* get pop handle */
      pvPOP = pstTmHandle->pvPOP;
      EME_UTILS_BREAKNULL(pvPOP, iRet, EME_FAILED, "EME_FSMPOP_DeleteAllMail, pop handle is null");
      
      /* record pop callback data */
      pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;

      iRet = EME_POP_DeleteAllMails(pvPOP);
      EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_DeleteAllMail, fetch specified failed");

   }while(0);


   if (iRet != EME_SUCCESS)
   {  
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, iRet, pstTransEvt);
   }

   return iRet;
}
 /* --------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMPOP_DeleteSpecMail   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. get spec uidl by msg id from storage, then dele spec mail with uidl
 *              3. if there is something wrong,trans fsm to TRANS_END
 *  
 * ===================================================================================================*/
static WE_INT32 EME_FSMPOP_DeleteSpecMail(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   WE_INT32    iRet = EME_SUCCESS;
   IPOPHANDLER pvPOP = NULL;
   St_TransEvt *pstTransEvt = NULL;
   WE_VOID     *pvMemHdr = NULL;
   WE_CHAR     *pcMsgUidl = NULL;
   WE_VOID     *pvStore = NULL;
   WE_VOID     *pvFileMgr = NULL;
   EmailOpt    *pstHeaderOpts = NULL;
   WE_CHAR     *pcMsgId =NULL;
   WE_UINT32   uiMsgId = 0;
   
   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {  
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMPOP_DeleteSpecMsg, transEvt is null");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_POP_DELETE_SPECMAIL;

      /* get pop handle */
      pvPOP = pstTmHandle->pvPOP;
      EME_UTILS_BREAKNULL(pvPOP, iRet, EME_FAILED, "EME_FSMPOP_DeleteSpecMsg, pop handle is null");
      
      /* get file mgr */
//      pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
//      EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_CBPOP_FetchSpec,get file mgr failed");      

      /* get message id */
      iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
      EME_UTILS_BREAKIF(iRet, "EME_FSMIMAP_FetchSpec, get message uidl failed");
      EME_UTILS_BREAKNULL(pcMsgId, iRet, EME_FAILED, "EME_FSMIMAP_FetchSpec, get message uidl failed");

      uiMsgId = EME_UTILS_ATOX(pcMsgId);
      
      /*here can be deleted ,for get msg does not need account id or defboxid.by echo*/
      /* get account id and default folder */
      iRet = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
      EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpec, init store handle failed");
      EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_CBPOP_FetchSpec, init store handle failed");
      
      iRet = EmailStore_GetMsgHeaders(pvStore, uiMsgId, &pstHeaderOpts);
      EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpec, get message header failed");
      EME_UTILS_BREAKNULL(pstHeaderOpts, iRet, EME_FAILED, "EME_CBPOP_FetchSpec, get message header failed");
      
      iRet = EME_GetStrOptValue((EMEOpt*)pstHeaderOpts, EMAIL_OPT_UIDL, &pcMsgUidl);
      EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpec, get UIDL failed");
      EME_UTILS_BREAKNULL(pcMsgUidl, iRet, EME_FAILED, "EME_CBPOP_FetchSpec, get UIDL failed");

      /* record pop callback data */
      pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;

      iRet = EME_POP_DeleteSepcifiedMail(pvPOP, pcMsgUidl);

      EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_DeleteSpecMsg, fetch specified failed");

   }while(0);

   EmailStore_DeleteHandle(pvStore);

   if (iRet != EME_SUCCESS)
   {  
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, iRet, pstTransEvt);
   }

   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMPOP_Quit   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if pop need close ,quit pop server,else trans fsm to TRANS_END
 *              3. if there is something wrong,trans fsm to TRANS_END
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMPOP_Quit(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_VOID     *pvMemHdr = NULL;
   St_TransEvt *pstTransEvt = NULL;
   IPOPHANDLER *pvPOP = NULL;

   EME_UTILS_LOG_INFO(("EME_FSMPOP_Quit : START!"));
   if (pstTmHandle == NULL || pstSig == NULL)
   {
      EME_UTILS_LOG_INFO(("EME_FSMPOP_Quit : BADPARAM!"));
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMPOP_Quit, transEvt is null");

      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_POP_QUIT;

      /* get pop handle */
      pvPOP = pstTmHandle->pvPOP;
      EME_UTILS_BREAKNULL(pvPOP, iRet, EME_FAILED, "EME_FSMPOP_Quit, pop handle is null");
      
      if (TRUE/*EME_POPCloseNeed(pstTmHandle, pstTransEvt)*/)
      {
         /* record pop callback data */
         pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;
         
         iRet = EME_POP_Quit(pvPOP);
         EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_Quit, failed");
      }
      else
      {
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, EME_SUCCESS, pstTransEvt);
      }

   }while(0);


   if (iRet != EME_SUCCESS)
   {  
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, iRet, pstTransEvt);
   }

   return iRet;
}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_FSMPOP_TransEnd   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if trans result is failed,cancel pop operation
                3. delete the transaction from trans queue ,
                4. reply this result of trans to app
 *  
*    ===================================================================================================*/
static WE_INT32 EME_FSMPOP_TransEnd(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   St_TransEvt    *pstTransEvt = NULL;
   WE_INT32       iRet = EME_SUCCESS;
   WE_VOID        *pvMemHdr = NULL;
   WE_INT32       iTransResult = EME_SUCCESS;
   WE_UINT32      uiTransID = 0;
   WE_UINT32      uiTransType = 0;
   WE_UINT32      uiClsId = 0;
   WE_INT32       iSetObj = 0;
   
   WE_VOID     *pvStore = NULL;
   WE_VOID     *pvFileMgr = NULL;
   WE_UINT32   uiMsgId = 0;
   WE_CHAR		*pcPartID = NULL;
   EmailOpt    *pstMsgPartOpts = NULL;
   WE_CHAR     *pcMsgId = NULL;
   WE_CHAR     *pcFileName = NULL;
   
   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }
   
   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);
   
   do 
   {
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMPOP_TransEnd, transEvt is null");
      
      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_POP_TRANS_END;
      
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
         EME_POP_CancelOperation(pstTmHandle->pvPOP);
         pstTmHandle->iPOPState = EME_STACK_STATE_LOGOUT;
      }
      
      uiTransID = pstTransEvt->uiTransID;
      uiTransType = pstTransEvt->uiTransType;
//      iRet = EME_GetIntOptValue(pstTransEvt->pstTransOpts, EME_OPTID_REPLY_CLSID, (WE_INT32*)&uiClsId);
//      EME_UTILS_BREAKIF(iRet, "EME_FSMPOP_TransEnd, get cls id failed");
      
      if(EME_TRANS_EPOP_SET_PUSH_MAIL == uiTransType)
      {
         iRet = EME_GetIntOptValue(pstTransEvt->pstTransOpts, EME_OPTID_EPOP_SET_OBJ, (WE_INT32*)&iSetObj);
      }
      else if(EME_TRANS_EPOP_FETCH_SPEC_MAIL_PART == uiTransType)
      {
         do 
         {
            /* get file mgr */
            pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
            EME_UTILS_BREAKNULL(pvFileMgr, iRet , EME_FAILED, "EME_CBEPOP_FetchSpecPart,get file mgr failed");      
            
            iRet = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
            EME_UTILS_BREAKIF(iRet , "EME_CBEPOP_FetchSpecPart, init store handle failed");
            EME_UTILS_BREAKNULL(pvStore, iRet , EME_FAILED, "EME_CBEPOP_FetchSpecPart, init store handle failed");
            
            /* get message id and part id */
            iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
            EME_UTILS_BREAKIF(iRet , "EME_CBEPOP_FetchSpecPart, get message id failed");
            
            uiMsgId = EME_UTILS_ATOX(pcMsgId);
            
            iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_PART_ID, &pcPartID);
            EME_UTILS_BREAKIF(iRet , "EME_CBEPOP_FetchSpecPart, get message part id failed");
            EME_UTILS_BREAKNULL(pcPartID, iRet , EME_FAILED, "EME_CBEPOP_FetchSpecPart, get message id failed");
            
            /* get message part opts */
            iRet = EmailStore_GetMsgPart(pvStore, uiMsgId, EME_UTILS_ATOI(pcPartID), &pstMsgPartOpts);
            EME_UTILS_BREAKIF(iRet , "EME_CBEPOP_FetchSpecPart, get message part failed");
            EME_UTILS_BREAKNULL(pstMsgPartOpts, iRet , EME_FAILED, "EME_CBEPOP_FetchSpecPart, get message part failed");
            
            EME_GetStrOptValue((EMEOpt*)pstMsgPartOpts, EMAILPARAM_OPT_FILE_NAME, &pcFileName);
            if(NULL!=pcFileName)
            {
               pstTmHandle->pcFilter = EME_UTILS_STRDUP(pvMemHdr, pcFileName);
            }
            
         }while(0);
         
         EmailStore_DeleteHandle(pvStore);
         pvStore = NULL;
         
      }
      
      EME_DeleteTransaction(pstTmHandle, pstTransEvt);
      
      EME_UTILS_LOG_INFO(("EME_FSMIMAP_TransEnd: pstTmHandle->iPOPState = %d",  pstTmHandle->iPOPState));
      
      /*Remove temp folder if exist*/
      EME_RemoveDir(EME_OEMTMLib_GetIFileMgr(pstTmHandle), EMEFS_TEMP_ROOT_DIR);
      
      /*
      if the state is EME_STACK_STATE_LOGOUT,need to disconnect the dail-------------just for plant of MTK
      */  
      /*	if(EME_STACK_STATE_LOGOUT == pstTmHandle->iPOPState)
      {
      WE_INT32   nDialId = 0;
      EME_GetDialId(&nDialId);
      EME_UTILS_LOG_INFO(("EME_FSMPOP_TransEnd: EME_GetDialId nDailid = %d", nDialId)); 
      EME_DisconnectPPP(nDialId);
      EME_UTILS_LOG_INFO(("EME_FSMPOP_TransEnd: EME_DisconnectPPP END!"));
      EME_FreePPPData();
      EME_UTILS_LOG_INFO(("EME_FSMPOP_TransEnd: EME_FreePPPData END!"));	 
      }
      */
      if(EME_TRANS_EPOP_SET_PUSH_MAIL == uiTransType)
      {
         iRet = OEMTMLib_TransReply(pstTmHandle, uiTransID, uiTransType, iSetObj, uiClsId, iTransResult, pstTmHandle->acRespDesc);
      }
      else if(EME_TRANS_EPOP_GET_PUSH_MAIL_FILTER== uiTransType)
      {
         iRet = OEMTMLib_TransReply(pstTmHandle, uiTransID, uiTransType, (WE_UINT32)pstTmHandle->pcFilter, uiClsId, iTransResult, pstTmHandle->acRespDesc);
         pstTmHandle->pcFilter = NULL;
      }
      else if(EME_TRANS_EPOP_FETCH_SPEC_MAIL_PART == uiTransType)
      {
         iRet = OEMTMLib_TransReply(pstTmHandle, uiTransID, uiTransType, uiMsgId, uiClsId, iTransResult, pstTmHandle->pcFilter);
         EME_UTILS_FREEIF(pvMemHdr, pstTmHandle->pcFilter);
         pstTmHandle->pcFilter = NULL;
      }
      else
      {
         iRet = OEMTMLib_TransReply(pstTmHandle, uiTransID, uiTransType, 0, uiClsId, iTransResult, pstTmHandle->acRespDesc);
      }
   } while(0);
   
   return iRet;
}

//static WE_INT32 EME_FSMPOP_Cancel(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig);


/*************************************************************************************************************/
/* Callback Functions */
/*************************************************************************************************************/
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_POPActionCB   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. deal with the evt cb from pop stack 
 *              3. if there is something wrong,trans fsm to TRANS_END
 *  
 * ===================================================================================================*/
WE_VOID EME_POPActionCB(WE_VOID* pvTmHandle,  St_POP_Ret* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;
   St_TransEvt *pstTransEvt = NULL;
   St_TMHandle *pstTmHandle = (St_TMHandle*)pvTmHandle;
   WE_UINT32   uiCurrSize = 0;
   WE_UINT32   uiTotalSize = 0;
   WE_UINT32   uiCurrNum = 0;
   WE_UINT32   uiTotalNum = 0;

   if (pstTmHandle == NULL)
   {
      return;
   }
   
   do 
   {
      pstTransEvt = (St_TransEvt*)pstTmHandle->pvCbkUserData;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_POPActionCB transevt is null");

      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_POPActionCB actret is null");

      /*copy respone description*/
      EME_UTILS_MEMSET(pstTmHandle->acRespDesc, 0, 512);
      if(pstActRet->iResult != EME_SUCCESS && EME_UTILS_STRLEN(pstActRet->acRespDesc) > 0)
         EME_UTILS_MEMCPY(pstTmHandle->acRespDesc, pstActRet->acRespDesc, EME_UTILS_STRLEN(pstActRet->acRespDesc));
      EME_UTILS_MEMSET(pstActRet->acRespDesc, 0, 512);

      switch(pstActRet->iEvCode)
      {
      case POP_EVT_INIT:
         iRet = EME_CBPOP_ConnectionInit(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case POP_EVT_LOGIN:
         iRet = EME_CBPOP_Login(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case POP_EVT_GETINFO:
         iRet = EME_CBPOP_GetInfo(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case POP_EVT_FETACHALL:
         iRet = EME_CBPOP_FetchNewMail(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case POP_EVT_FETCHHEADERS:
         iRet = EME_CBPOP_FetchNewHdr(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case POP_EVT_FETCHASP:
         iRet = EME_CBPOP_FetchSpecMail(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case POP_EVT_FETCHSPHEADER:
         iRet = EME_CBPOP_FetchSpecHdr(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case POP_EVT_DELETEALL:
         iRet = EME_CBPOP_DeleteAllMail(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case POP_EVT_DELETESP:
         if (pstTransEvt->iCurrSignalID == EME_SIG_POP_UNKEEP_MAIL && 
             pstTransEvt->uiTransType == EME_TRANS_POP3_FETCH_NEW_MAILS)
         {
            EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_FETCH_NEWMAILS, pstTransEvt);
         }
         else if (pstTransEvt->iCurrSignalID == EME_SIG_POP_UNKEEP_MAIL && 
				  pstTransEvt->uiTransType == EME_TRANS_EPOP_FETCH_NEW_MAILS)
         {
            EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_EPOP_FETCH_NEWMAILS, pstTransEvt);
         }
         else if (pstTransEvt->iCurrSignalID == EME_SIG_POP_UNKEEP_MAIL && 
                  (pstTransEvt->uiTransType == EME_TRANS_POP3_FETCH_SPEC_MAIL
				   || pstTransEvt->uiTransType == EME_TRANS_EPOP_FETCH_SPEC_MAIL))
         {
            EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_QUIT, pstTransEvt);
         }
         else
         {
            iRet = EME_CBPOP_DeleteSpecMail(pstTmHandle, pstTransEvt, pstActRet);
         }
         break;
      case POP_EVT_QUIT:
         iRet = EME_CBPOP_Quit(pstTmHandle, pstTransEvt, pstActRet);
         break;
      case POP_EVT_CANCEL:
         break;
      case POP_EVT_DISCONNECT:
         pstTmHandle->iPOPState = EME_STACK_STATE_LOGOUT;
         iRet = EME_CONNECT_ERROR;
         break;         
      case POP_EVT_SIZE_NOTIFY:
         /*notify size to app*/
         uiCurrSize = pstActRet->detail.stSizeNotifyRet.uiCurrSize;
         uiTotalSize = pstActRet->detail.stSizeNotifyRet.uiTotalSize;
         if (uiCurrSize > uiTotalSize)
         {
            uiCurrSize = uiTotalSize;
         }
         iRet = OEMTMLib_NotifySize(pstTmHandle, pstTransEvt, uiCurrSize, uiTotalSize);
         break;
      case POP_EVT_NUMBER_NOTIFY:
         /*notify current num & total num of email to app*/
         uiCurrNum = pstActRet->detail.stNumNotifyRet.uiCurrNum;
         uiTotalNum = pstActRet->detail.stNumNotifyRet.uiTotalNum;
         iRet = OEMTMLib_NotifyNumber(pstTmHandle, pstTransEvt, uiCurrNum, uiTotalNum);
         break;
    case EPOP_EVT_FETCHPROF:
        iRet = EME_CBEPOP_Prof(pstTmHandle, pstTransEvt, pstActRet);
        break;
    case EPOP_EVT_FETCHALL:
        iRet = EME_CBEPOP_FetchNewMail(pstTmHandle, pstTransEvt, pstActRet);
        break;
    case EPOP_EVT_FETCHSPMAIL:
        iRet = EME_CBEPOP_FetchSpecMail(pstTmHandle, pstTransEvt, pstActRet);
        break;
    case EPOP_EVT_FETCHSPPART:
        iRet = EME_CBEPOP_FetchSpecPart(pstTmHandle, pstTransEvt, pstActRet);
        break;
    case EPOP_EVT_FETCHHEADERS:
        iRet = EME_CBEPOP_FetchNewHeaders(pstTmHandle, pstTransEvt, pstActRet);
        break;
    case EPOP_EVT_SET:
    case EPOP_EVT_GET_FILTER:
        iRet = EME_CBEPOP_GetPushMailFilter(pstTmHandle, pstTransEvt, pstActRet);
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
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, iRet, pstTransEvt);
   }
   return;
}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBPOP_ConnectionInit   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of connect to pop server from stack is success, login pop server
 *              3. else trans fsm to TRANS_END
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBPOP_ConnectionInit(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBPOP_ConnectionInit actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS)
      {
         /* record pop callback data */
         pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;

         iRet = EME_POP_Login(pstTmHandle->pvPOP);
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, pstActRet->iResult, pstTransEvt);
      }

   } while(0);

   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBPOP_Login   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of login pop server from stack is success, get mail info
 *              3. else trans fsm to TRANS_END
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBPOP_Login(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;
   St_EPOP_PushMailSet* pstSet = NULL;
   WE_VOID     *pvMemHdr = NULL;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBPOP_Login actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS)
      {
         /* record pop callback data */
         pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;

        switch(pstTransEvt->uiTransType)
        {
             case EME_TRANS_EPOP_SET_PUSH_MAIL:
             {
                pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);
                pstSet = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_EPOP_PushMailSet));
                if(NULL == pstSet)
                {
                    iRet = EME_FAILED;
                    break;
                }
                /* get set obj, option */
                iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_EPOP_SET_FILTER, &(pstSet->pcFilter));
                if(NULL == pstSet->pcFilter)
                {
                    iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_EPOP_SET_STRING, &pstSet->pcValue);
                }
                iRet = EME_EPOP_Set(pstTmHandle->pvPOP, pstSet);
                break;
             }

             case EME_TRANS_EPOP_GET_PUSH_MAIL_FILTER:
             {
                iRet = EME_EPOP_GetFilter(pstTmHandle->pvPOP);
                break;
             }

            case EME_TRANS_EPOP_FETCH_SPEC_MAIL_PART:
//                EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_EPOP_FETCH_SPECPART, (WE_VOID*)pstTransEvt);
//                break;
            default:
                iRet = EME_POP_GetMailInfo(pstTmHandle->pvPOP);
                break;
        }

      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, pstActRet->iResult, pstTransEvt);
      }

   } while(0);

   return iRet;
}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBPOP_GetInfo   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of get mail info from stack is success, 
                   trans fsm to pop operation fsm,
                   else if the result is EME_NO_NEWMAIL,
                   trans fsm to TRANS_END
 *              3. else trans fsm to TRANS_END
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBPOP_GetInfo(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_UINT32   uiSigId = 0;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBPOP_GetInfo actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS)
      {
         /* set pop state */
         pstTmHandle->iPOPState = EME_STACK_STATE_LOGIN;
//          pstTmHandle->iPOPTotalMsg = pstActRet->detail.stGetInfoRet.iTotalNum;
//          pstTmHandle->iPOPTotalSize = pstActRet->detail.stGetInfoRet.iTotalSize;

         switch(pstTransEvt->uiTransType)
         {
         case EME_TRANS_POP3_FETCH_NEW_MAILS_HEADER:
            uiSigId = EME_SIG_POP_FETCH_NEWHDRS;
            break;
         case EME_TRANS_POP3_FETCH_NEW_MAILS:
            uiSigId = EME_SIG_POP_FETCH_NEWMAILS;
            break;
         case EME_TRANS_POP3_FETCH_SPEC_MAIL:
            uiSigId = EME_SIG_POP_FETCH_SPECMAIL;
            break;
//          case EME_TRANS_POP3_FETCH_SPEC_MAIL_HEADER:
//             uiSigId = EME_SIG_POP_FETCH_SPECHDR;
//             break;
         case EME_TRANS_POP3_DELETE_SPEC_MAIL:
            uiSigId = EME_SIG_POP_DELETE_SPECMAIL;
            break;
         case EME_TRANS_POP3_DELETE_ALL_MAIL:
            uiSigId = EME_SIG_POP_DELETE_ALLMAIL;
            break;
        case EME_TRANS_EPOP_FETCH_NEW_MAILS_HEADER:
            uiSigId = EME_SIG_EPOP_FETCH_NEWHDRS;
            break;
        case EME_TRANS_EPOP_FETCH_NEW_MAILS:
            uiSigId = EME_SIG_EPOP_FETCH_NEWMAILS;
            break;
        case EME_TRANS_EPOP_FETCH_SPEC_MAIL:
//            uiSigId = EME_SIG_EPOP_FETCH_PROF;
            uiSigId = EME_SIG_EPOP_FETCH_SPECMAIL;
            break;
        case EME_TRANS_EPOP_FETCH_SPEC_MAIL_PART:
            uiSigId = EME_SIG_EPOP_FETCH_SPECPART;
            break;
         default:
            iRet = EME_FAILED;
            break;
         }
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_GetInfo, unknow trans type");

         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, uiSigId, (WE_VOID*)pstTransEvt);
      }
      else if (pstActRet->iResult == EME_NO_NEWMAIL)
      {
         EME_UTILS_LOG_INFO(("EME_CBPOP_GetInfo, there is no new email"));
         if (pstTransEvt->uiTransType == EME_TRANS_POP3_FETCH_SPEC_MAIL)
         {
            EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, EME_MAIL_NOTEXSITED , pstTransEvt);
         }
         else
         {
            if(!pstActRet->bHasNewMail)
            {
               pstTransEvt->iErrCode = EME_NO_NEWMAIL;
            }
            EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_QUIT, pstTransEvt);
         }
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, pstActRet->iResult, pstTransEvt);
      }

   } while(0);

   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBPOP_FetchNewMail   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of fetch one new email from stack is success, 
                   decode the date & store it,
                      if the account cfg is keep mail, trans fsm to fetch an other new mail .
                      else trans fsm to delete this spec mail with uidl.
                3. else if the result cb of fetch one new email from stack is no new email,
                   trans fsm to POP_QUIT
 *              4. else trans fsm to TRANS_END
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBPOP_FetchNewMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_VOID     *pvMemHdr = NULL;
   WE_VOID     *pvCodec = NULL;
   WE_VOID     *pvFileMgr = NULL;
   WE_CHAR     *pcPath = NULL;
   WE_UINT32   uiMsgId = 0;

   WE_INT32    iAccountId = 0;
   WE_INT32    iDefBoxId = 0;

   EmailOpt    stCodecOpts[EME_CODEC_DECODE_OPT_NUM];
   EmailOpt    stStoreOpts [EME_STORE_INIT_OPT_NUM];
   St_CBDecodeData   *pstCBData = NULL;
   HEmailStore       pvStore = NULL;

   EME_UTILS_LOG_INFO(("EME_CBPOP_FetchNewMail---: Start"));
   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_CBPOP_FetchNewMail--: Bad parameter and return"));
      return EME_FAILED;
   }

   do 
   {
      pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBPOP_FetchNewMail--:  actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS && 
         pstActRet->detail.stFetchAllRet.pcDataBuffer != NULL &&
         pstActRet->detail.stFetchAllRet.pcFetchedID != NULL)
      {
         EME_UTILS_MEMSET(stCodecOpts, 0, sizeof(EmailOpt) * EME_CODEC_DECODE_OPT_NUM);  
         EME_UTILS_MEMSET(&stStoreOpts, 0, EME_STORE_INIT_OPT_NUM * sizeof(EmailOpt));
         
         /* get account id and default folder and keep mail opt */
         iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_ACCOUNT_ID, &iAccountId);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewMail--: , get account id failed");
         
         iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_DEFAULT_FOLDER_ID, &iDefBoxId);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewMail--: , get default folder id  failed");

         /* get file mgr */
//         pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
//         EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_CBPOP_FetchNewMail--: ,get file mgr failed");
         
         /* init store and codec handle */
         stStoreOpts[0].iId = EMAILMESSAGE_OPT_ACTID;
         stStoreOpts[0].pVal = (WE_VOID*)iAccountId;
         stStoreOpts[1].iId = EMAILMESSAGE_OPT_DEF_INBOXID;
         stStoreOpts[1].pVal = (WE_VOID*)iDefBoxId;
         stStoreOpts[2].iId = EMAILOPT_END;

         iRet = EmailStore_InitHandle(&pvStore, stStoreOpts, pvFileMgr,pvMemHdr);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewMail--: , init store handle failed");
         EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_CBPOP_FetchNewMail--: , init store handle failed");

         iRet = EmailStore_NewMessage(pvStore, &uiMsgId, &pcPath);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewMail--: Create new message failed");
         EME_UTILS_BREAKNULL(pcPath, iRet, EME_NOMEMORY, "EME_CBPOP_FetchNewMail--: Create path failed");

         iRet = EMECodecLib_InitHandleEx(&pvCodec, pvMemHdr, pvFileMgr, OEMTMLib_GetHCSC(pstTmHandle), pstTmHandle->pvAsyncMgr);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewMail--: , init store handle failed");
         EME_UTILS_BREAKNULL(pvCodec, iRet, EME_FAILED, "EME_CBPOP_FetchNewMail--:  init codec handle failed");

         pstCBData = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_CBDecodeData));
         EME_UTILS_BREAKNULL(pstCBData, iRet, EME_NOMEMORY, "no memory for cb data");
         EME_UTILS_MEMSET(pstCBData, 0, sizeof(St_CBDecodeData));

         pstCBData->iDecodingType = EMAIL_DECODETYPE_WHOLEMSG;
         pstCBData->pvTMHandle    = pstTmHandle;
         pstCBData->pvTranEvent   = pstTransEvt;
         pstCBData->pvUserData    = NULL;
         pstCBData->pvStore       = pvStore;
         pstCBData->pvCodec       = pvCodec;
         pstCBData->uiMsgId       = uiMsgId;
         pstCBData->bSpec         = FALSE;

         /*recode the act result*/
         if(NULL != pstActRet->detail.stFetchAllRet.pcFetchedID)
         {
            pstCBData->stActResult.pcFetchedID   = EME_UTILS_STRDUP(pvMemHdr, pstActRet->detail.stFetchAllRet.pcFetchedID);
            EME_UTILS_BREAKNULL(pstCBData->stActResult.pcFetchedID, iRet, EME_NOMEMORY, "no memory for fetch id");
         }
         pstCBData->stActResult.pcDataBuffer  = EME_UTILS_STRDUP(pvMemHdr, pstActRet->detail.stFetchAllRet.pcDataBuffer);
         EME_UTILS_BREAKNULL(pstCBData->stActResult.pcDataBuffer, iRet, EME_NOMEMORY, "no memory for data buffer");
         pstCBData->stActResult.iFetchSize = pstActRet->detail.stFetchAllRet.iFetchedSize;
         
         /* decode mail */
         stCodecOpts[0].iId = EMAILMESSAGE_OPT_SRC_TYPE;
         stCodecOpts[0].pVal = (WE_VOID*)EMAIL_ENCODE_BYFILE;
         stCodecOpts[1].iId = EMIALMESSAGE_OPT_DECODE_TYPE;
         stCodecOpts[1].pVal = (WE_VOID*)EMAIL_DECODETYPE_WHOLEMSG;
         stCodecOpts[2].iId = EMAILMESSAGE_OPT_BUFF;
         stCodecOpts[2].pVal = (WE_VOID*)pstCBData->stActResult.pcDataBuffer;
         stCodecOpts[3].iId = EMAILOPT_END;

         iRet = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, (void*)pstCBData, (PFNDECODINGCB)EME_CBPOP_DecodingMsgCB);
         //EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewMail--: decoding message failed");
         /*register the free function*/
         if (iRet == EME_SUCCESS)
         {
            pstTransEvt->pfnFreeSigData = EME_CBPOP_pfnFree;
            pstTransEvt->pvTransData = pstCBData;         
         }
         else
         {
            /* update uidl */
            iRet =  EME_POP_TAG_MAIL(pstTmHandle->pvPOP, pstActRet->detail.stFetchAllRet.pcFetchedID, TRUE);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewHdr, update uidl failed");
            
            pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;
            
            EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_FETCH_NEWMAILS, pstTransEvt);

            if(NULL != pstCBData)
            {
               EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActResult.pcDataBuffer);
               EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActResult.pcFetchedID);
               EME_UTILS_FREEIF(pvMemHdr, pstCBData);
            }
            (void)EmailStore_DeleteMsg(pvStore, uiMsgId);
            EMECodecLib_DeleteHandle(&pvCodec);
            EmailStore_DeleteHandle(pvStore);
            pvStore = NULL;
         }

//         iRet = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, NULL, NULL);
//         EME_UTILS_BREAKIF(iRet, "Decode email failed");
//         EME_CBPOP_DecodingMsgCB(pvCodec, pstCBData, EME_SUCCESS);
      }
      else if (pstActRet->iResult == EME_NO_NEWMAIL)
      {
         pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;
         if(!pstActRet->bHasNewMail)
         {
            pstTransEvt->iErrCode = EME_NO_NEWMAIL;
         }
         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_QUIT, pstTransEvt);
      }
      else
      {
//          if (pstTransEvt != NULL)
//          {
//             pstTransEvt->iErrCode = EME_FAILED;
//          }
         //EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, pstActRet->iResult, pstTransEvt);
         /*change the fresh status of this email in pop uidl list*/

        /* update uidl */
        if(pstActRet->detail.stFetchAllRet.pcFetchedID != NULL)
        {
            iRet =  EME_POP_TAG_MAIL(pstTmHandle->pvPOP, pstActRet->detail.stFetchAllRet.pcFetchedID, TRUE);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewHdr, update uidl failed");
        }
        EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_FETCH_NEWMAILS, pstTransEvt);
      }

   } while(0);


   if(iRet != EME_SUCCESS)
   {
      if(NULL != pstCBData)
      {
         EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActResult.pcDataBuffer);
         EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActResult.pcFetchedID);
         EME_UTILS_FREEIF(pvMemHdr, pstCBData);
      }
      (void)EmailStore_DeleteMsg(pvStore, uiMsgId);
      EMECodecLib_DeleteHandle(&pvCodec);
      EmailStore_DeleteHandle(pvStore);
      pvStore = NULL;
   }
   
   EME_UTILS_LOG_INFO(("EME_CBPOP_FetchNewMail---: End, Result = %d", iRet));
   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBPOP_FetchNewHdr   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of fetch one new email header from stack is success, 
                   decode the date & store it,
                   trans fsm to fetch an other new mail header .
                3. else if the result cb of fetch one new email header from stack is no new email,
                   trans fsm to POP_QUIT
 *              4. else trans fsm to TRANS_END
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBPOP_FetchNewHdr(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_VOID     *pvMemHdr = NULL;
   WE_VOID     *pvCodec = NULL;
   WE_VOID     *pvFileMgr = NULL;
   EmailOpt    stStoreOpts[EME_STORE_INIT_OPT_NUM];
   EmailOpt    stCodecOpts[EME_CODEC_DECODE_OPT_NUM];

   WE_INT32    iAccountId = 0;
   WE_INT32    iDefBoxId = 0;
   WE_UINT32   uiMsgId = 0;
   WE_CHAR     *pcPath = NULL;
   WE_VOID     *pvStore = NULL;

   St_CBDecodeData* pstCBData = NULL;

   EME_UTILS_LOG_INFO(("EME_CBPOP_FetchNewHdr---: Start"));
   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_CBPOP_FetchNewHdr--: Bad parameter and return"));
      return EME_FAILED;
   }

   do 
   {
      pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBPOP_FetchNewHdr--:  actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS && 
         pstActRet->detail.stAllHeaderRet.pcDataBuffer != NULL &&
         pstActRet->detail.stAllHeaderRet.pcFetchedID != NULL)
      {
         EME_UTILS_MEMSET(stCodecOpts, 0, sizeof(EmailOpt) * EME_CODEC_DECODE_OPT_NUM);
         EME_UTILS_MEMSET(stStoreOpts, 0, sizeof(EmailOpt) * EME_STORE_INIT_OPT_NUM);   
   
         /* get file mgr */
 //        pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
 //        EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_CBPOP_FetchNewHdr--: ,get file mgr failed");      

         /* get account id and default folder and keep mail opt */
         iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_ACCOUNT_ID, &iAccountId);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewHdr--: , get account id failed");

         iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_DEFAULT_FOLDER_ID, &iDefBoxId);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewHdr--: , get default folder id  failed");

         /* init store and codec handle */
         stStoreOpts[0].iId = EMAILMESSAGE_OPT_ACTID;
         stStoreOpts[0].pVal = (WE_VOID*)iAccountId;
         stStoreOpts[1].iId = EMAILMESSAGE_OPT_DEF_INBOXID;
         stStoreOpts[1].pVal = (WE_VOID*)iDefBoxId;
         stStoreOpts[2].iId = EMAILOPT_END;
         iRet = EmailStore_InitHandle(&pvStore, stStoreOpts, pvFileMgr,pvMemHdr);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewHdr--: , init store handle failed");
         EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_CBPOP_FetchNewHdr--: init store handle failed");

         iRet = EmailStore_NewMessage(pvStore, &uiMsgId, &pcPath);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewHdr--: New message failed");
         EME_UTILS_BREAKNULL(pcPath, iRet, EME_FAILED, "create path failed");

         iRet = EMECodecLib_InitHandleEx(&pvCodec, pvMemHdr, pvFileMgr, OEMTMLib_GetHCSC(pstTmHandle), pstTmHandle->pvAsyncMgr);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewHdr--:  init store handle failed");
         EME_UTILS_BREAKNULL(pvCodec, iRet, EME_FAILED, "EME_CBPOP_FetchNewHdr--:  init codec handle failed");

         pstCBData = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_CBDecodeData));
         EME_UTILS_BREAKNULL(pstCBData, iRet, EME_NOMEMORY, "EME_CBPOP_FetchNewHdr--: no memory for cb data");
         EME_UTILS_MEMSET(pstCBData, 0, sizeof(St_CBDecodeData));

         pstCBData->iDecodingType = EMAIL_DECODETYPE_HEADER;
         pstCBData->pvTMHandle    = pstTmHandle;
         pstCBData->pvTranEvent   = pstTransEvt;
         pstCBData->pvUserData    = NULL;
         pstCBData->pvStore       = pvStore;
         pstCBData->pvCodec       = pvCodec;
         pstCBData->uiMsgId       = uiMsgId;
         pstCBData->bSpec         = FALSE;
         
         /*recode the act result*/
         if(NULL != pstActRet->detail.stAllHeaderRet.pcFetchedID)
         {
            pstCBData->stActResult.pcFetchedID   = EME_UTILS_STRDUP(pvMemHdr, pstActRet->detail.stAllHeaderRet.pcFetchedID);
            EME_UTILS_BREAKNULL(pstCBData->stActResult.pcFetchedID, iRet, EME_NOMEMORY, "no memory for fetch id");
         }
         pstCBData->stActResult.pcDataBuffer  = EME_UTILS_STRDUP(pvMemHdr, pstActRet->detail.stAllHeaderRet.pcDataBuffer);
         EME_UTILS_BREAKNULL(pstCBData->stActResult.pcDataBuffer, iRet, EME_NOMEMORY, "no memory for data buffer");
         pstCBData->stActResult.iFetchSize = pstActRet->detail.stAllHeaderRet.iFetchedSize;
         
         /* decode mail */
         stCodecOpts[0].iId = EMAILMESSAGE_OPT_SRC_TYPE;
         stCodecOpts[0].pVal = (WE_VOID*)EMAIL_ENCODE_BYFILE;
         stCodecOpts[1].iId = EMIALMESSAGE_OPT_DECODE_TYPE;
         stCodecOpts[1].pVal = (WE_VOID*)EMAIL_DECODETYPE_HEADER;
         stCodecOpts[2].iId = EMAILMESSAGE_OPT_BUFF;
         stCodecOpts[2].pVal = (WE_VOID*)pstCBData->stActResult.pcDataBuffer;
         stCodecOpts[3].iId = EMAILOPT_END;

         iRet = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, (void*)pstCBData, (PFNDECODINGCB)EME_CBPOP_DecodingMsgCB);
         //EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewHdr--:  decoding failed");
         if (iRet == EME_SUCCESS)
         {
            pstTransEvt->pfnFreeSigData = EME_CBPOP_pfnFree;
            pstTransEvt->pvTransData = pstCBData;         
         }
         else
         {
            /* update uidl */
            iRet =  EME_POP_TAG_MAIL(pstTmHandle->pvPOP, pstActRet->detail.stAllHeaderRet.pcFetchedID, TRUE);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewHdr, update uidl failed");
            
            pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;
            
            EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_FETCH_NEWHDRS, pstTransEvt);

            if(NULL != pstCBData)
            {
               EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActResult.pcDataBuffer);
               EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActResult.pcFetchedID);
               EME_UTILS_FREEIF(pvMemHdr, pstCBData);
            }
            (void)EmailStore_DeleteMsg(pvStore, uiMsgId);
            EMECodecLib_DeleteHandle(&pvCodec);
            EmailStore_DeleteHandle(pvStore);
            pvStore = NULL;
         }
      }
      else if (pstActRet->iResult == EME_NO_NEWMAIL)
      {
         pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;
         if(!pstActRet->bHasNewMail)
         {
            pstTransEvt->iErrCode = EME_NO_NEWMAIL;
         }
         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_QUIT, pstTransEvt);
      }
      else
      {
//          if (pstTransEvt != NULL)
//          {
//             pstTransEvt->iErrCode = EME_FAILED;
//          }
//          EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, pstActRet->iResult, pstTransEvt);

         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_FETCH_NEWHDRS, pstTransEvt);
      }

   } while(0);


   if(EME_SUCCESS != iRet)
   {
      if(NULL != pstCBData)
      {
         EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActResult.pcDataBuffer);
         EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActResult.pcFetchedID);
         EME_UTILS_FREEIF(pvMemHdr, pstCBData);
      }
      (void)EmailStore_DeleteMsg(pvStore, uiMsgId);
      EMECodecLib_DeleteHandle(&pvCodec);
      EmailStore_DeleteHandle(pvStore);
      pvStore = NULL;
   }
   EME_UTILS_LOG_INFO(("EME_CBPOP_FetchNewHdr---: End, Result = %d", iRet));
   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBPOP_FetchSpecMail   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of fetch all parts of one email from stack is success, 
                   decode the date & store it,
                      if the account cfg is keep mail, trans fsm to POP_QUIT .
                      else trans fsm to delete this spec mail with uidl.
                3. else if the result cb of fetch all parts of one email from stack is no new email,
                   trans fsm to POP_QUIT
 *              4. else trans fsm to TRANS_END
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBPOP_FetchSpecMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_VOID     *pvMemHdr = NULL;
   WE_VOID     *pvFileMgr = NULL;
   WE_VOID     *pvCodec = NULL;
   WE_VOID     *pvStore = NULL;
   WE_CHAR     *pcMsgId = NULL;
   WE_UINT32   uiMsgId = 0;
   WE_CHAR     *pcPath = NULL;
   EmailOpt    stCodecOpts[EME_CODEC_DECODE_OPT_NUM];

   St_CBDecodeData* pstCBData = NULL;

   EME_UTILS_LOG_INFO(("EME_CBPOP_FetchSpecMail---: Start"));
   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_CBPOP_FetchSpecMail--: Bad parameter and return"));
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBPOP_FetchSpecMail--:  actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS && 
         pstActRet->detail.stFetchSpRet.pcDataBuffer != NULL &&
         pstActRet->detail.stFetchSpRet.pcFetchedID != NULL)
      {
         EME_UTILS_MEMSET(stCodecOpts, 0, sizeof(EmailOpt) * EME_CODEC_DECODE_OPT_NUM);
         /* get file mgr */
//         pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
//         EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_CBPOP_FetchSpecMail--: get file mgr failed");
  
         iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpecMail, get message id failed");
         EME_UTILS_BREAKNULL(pcMsgId, iRet, EME_FAILED, "EME_CBPOP_FetchSpecMail--:  get message id failed");

         uiMsgId = EME_UTILS_ATOX(pcMsgId);

         iRet = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpecMail--:  init store handle failed");
         EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_CBPOP_FetchSpecMail--:  init store handle failed");

         iRet = EmailStore_GetMsgPath(pvStore, uiMsgId, &pcPath);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpecMail--: Get message path failed");
         EME_UTILS_BREAKNULL(pcPath, iRet, EME_FAILED, "EME_CBPOP_FetchSpecMail--: get message path failed");

         iRet = EMECodecLib_InitHandleEx(&pvCodec, pvMemHdr, pvFileMgr, OEMTMLib_GetHCSC(pstTmHandle), pstTmHandle->pvAsyncMgr);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpecMail--:  init store handle failed");
         EME_UTILS_BREAKNULL(pvCodec, iRet, EME_FAILED, "EME_CBPOP_FetchSpecMail--:  init codec handle failed");

         pstCBData = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_CBDecodeData));
         EME_UTILS_BREAKNULL(pstCBData, iRet, EME_NOMEMORY, "EME_CBPOP_FetchSpecMail--: no memory for cb data");
         EME_UTILS_MEMSET(pstCBData, 0, sizeof(St_CBDecodeData));

         pstCBData->iDecodingType = EMAIL_DECODETYPE_WHOLEMSG;
         pstCBData->pvTMHandle    = pstTmHandle;
         pstCBData->pvTranEvent   = pstTransEvt;
         pstCBData->pvUserData    = NULL;
         pstCBData->pvStore       = pvStore;
         pstCBData->pvCodec       = pvCodec;
         pstCBData->uiMsgId       = uiMsgId;
         pstCBData->bSpec         = TRUE;

         /*recode the act result*/
         if(NULL != pstActRet->detail.stFetchSpRet.pcFetchedID)
         {
            pstCBData->stActResult.pcFetchedID   = EME_UTILS_STRDUP(pvMemHdr, pstActRet->detail.stFetchSpRet.pcFetchedID);
            EME_UTILS_BREAKNULL(pstCBData->stActResult.pcFetchedID, iRet, EME_NOMEMORY, "no memory for fetch id");
         }
         pstCBData->stActResult.pcDataBuffer  = EME_UTILS_STRDUP(pvMemHdr, pstActRet->detail.stFetchSpRet.pcDataBuffer);
         EME_UTILS_BREAKNULL(pstCBData->stActResult.pcDataBuffer, iRet, EME_NOMEMORY, "no memory for data buffer");
         pstCBData->stActResult.iFetchSize = pstActRet->detail.stFetchSpRet.iFetchedSize;

         /* decode mail */
         stCodecOpts[0].iId = EMAILMESSAGE_OPT_SRC_TYPE;
         stCodecOpts[0].pVal = (WE_VOID*)EMAIL_ENCODE_BYFILE;
         stCodecOpts[1].iId = EMIALMESSAGE_OPT_DECODE_TYPE;
         stCodecOpts[1].pVal = (WE_VOID*)EMAIL_DECODETYPE_WHOLEMSG;
         stCodecOpts[2].iId = EMAILMESSAGE_OPT_BUFF;
         stCodecOpts[2].pVal = (WE_VOID*)pstCBData->stActResult.pcDataBuffer;
//         stCodecOpts[2].pVal = (WE_VOID*)"D:/TechSoft/Win-1252.eml";
         stCodecOpts[3].iId = EMAILOPT_END;

         iRet = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, (void*)pstCBData, (PFNDECODINGCB)EME_CBPOP_DecodingMsgCB);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpecMail--:  decoding failed");

         pstTransEvt->pfnFreeSigData = EME_CBPOP_pfnFree;
         pstTransEvt->pvTransData = pstCBData;
         
//         iRet = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, NULL, NULL);
//         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpecMail, decoding failed");
//         EME_CBPOP_DecodingMsgCB(pvCodec, pstCBData, EME_SUCCESS);
      }
      else if (pstActRet->iResult == EME_MAIL_NOTEXSITED)
      {
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, EME_MAIL_NOTEXSITED, pstTransEvt);
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, pstActRet->iResult, pstTransEvt);
      }

   } while(0);
   
   if (iRet != EME_SUCCESS)
   {
      if(NULL != pstCBData)
      {
         EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActResult.pcDataBuffer);
         EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActResult.pcFetchedID);
         EME_UTILS_FREEIF(pvMemHdr, pstCBData);
      }
      EMECodecLib_DeleteHandle(&pvCodec);
      EmailStore_DeleteHandle(pvStore);
      pvStore = NULL;
   }
   EME_UTILS_LOG_INFO(("EME_CBPOP_FetchSpecMail---: End, Result = %d", iRet));

   return iRet;
}

static WE_INT32 EME_CBPOP_FetchSpecHdr(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_VOID     *pvMemHdr = NULL;
   WE_VOID     *pvFileMgr = NULL;
   WE_VOID     *pvCodec = NULL;
   WE_VOID     *pvStore = NULL;
   EmailOpt    *pstHeaderOpts = NULL;
   EmailOpt    stStoreOpts[EME_STORE_INIT_OPT_NUM];
   EmailOpt    stCodecOpts[EME_CODEC_DECODE_OPT_NUM];

   WE_INT32    iAccountId = 0;
   WE_INT32    iDefBoxId = 0;

   WE_CHAR     *pcPath = NULL;
   WE_UINT32   uiMsgId = 0;
   St_CBDecodeData* pstCBData = NULL;

   EME_UTILS_LOG_INFO(("EME_CBPOP_FetchSpecHdr---: Start"));
   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_CBPOP_FetchSpecHdr--: Bad parameter and return"));
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBPOP_FetchSpecHdr--:  actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS && 
         pstActRet->detail.stSpHeaderRet.pcDataBuffer != NULL &&
         pstActRet->detail.stSpHeaderRet.pcFetchedID != NULL)
      {
         EME_UTILS_MEMSET(stCodecOpts, 0, sizeof(EmailOpt) * EME_CODEC_DECODE_OPT_NUM);
         EME_UTILS_MEMSET(stStoreOpts, 0, sizeof(EmailOpt) * EME_STORE_INIT_OPT_NUM);
         
         /* get file mgr */
//         pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
//         EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_CBPOP_FetchSpecHdr--: ,get file mgr failed");
  
         /* get account id and default box id */
         iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_ACCOUNT_ID, &iAccountId);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpecHdr--:  get account id failed");

         iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_DEFAULT_FOLDER_ID, &iDefBoxId);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpecHdr--:  get default folder id  failed");

         /* init store and codec handle */
         stStoreOpts[0].iId = EMAILMESSAGE_OPT_ACTID;
         stStoreOpts[0].pVal = (WE_VOID*)iAccountId;
         stStoreOpts[1].iId = EMAILMESSAGE_OPT_DEF_INBOXID;
         stStoreOpts[1].pVal = (WE_VOID*)iDefBoxId;
         stStoreOpts[2].iId = EMAILOPT_END;     
         iRet = EmailStore_InitHandle(&pvStore, stStoreOpts, pvFileMgr,pvMemHdr);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpecHdr--:  init store handle failed");
         EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_CBPOP_FetchSpecHdr--:  init store handle failed");

         iRet = EmailStore_NewMessage(pvStore, &uiMsgId, &pcPath);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpecHdr--: New message failed");
         EME_UTILS_BREAKNULL(pcPath, iRet, EME_FAILED, "EME_CBPOP_FetchSpecHdr--: path is null");

         pstCBData = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_CBDecodeData));
         EME_UTILS_BREAKNULL(pstCBData, iRet, EME_NOMEMORY, "EME_CBPOP_FetchSpecHdr--: no memory for cb data");
         EME_UTILS_MEMSET(pstCBData, 0, sizeof(St_CBDecodeData));

         pstCBData->iDecodingType = EMAIL_DECODETYPE_HEADER;
         pstCBData->pvTMHandle    = pstTmHandle;
         pstCBData->pvTranEvent   = pstTransEvt;
         pstCBData->pvUserData    = NULL;
         pstCBData->pvStore       = pvStore;
         pstCBData->pvCodec       = pvCodec;
         pstCBData->uiMsgId       = uiMsgId;
         pstCBData->bSpec         = TRUE;

         /*recode the act result*/
         if(NULL != pstActRet->detail.stSpHeaderRet.pcFetchedID)
         {
            pstCBData->stActResult.pcFetchedID   = EME_UTILS_STRDUP(pvMemHdr, pstActRet->detail.stSpHeaderRet.pcFetchedID);
            EME_UTILS_BREAKNULL(pstCBData->stActResult.pcFetchedID, iRet, EME_NOMEMORY, "no memory for fetch id");
         }
         pstCBData->stActResult.pcDataBuffer  = EME_UTILS_STRDUP(pvMemHdr, pstActRet->detail.stSpHeaderRet.pcDataBuffer);
         EME_UTILS_BREAKNULL(pstCBData->stActResult.pcDataBuffer, iRet, EME_NOMEMORY, "no memory for data buffer");
         pstCBData->stActResult.iFetchSize = pstActRet->detail.stSpHeaderRet.iFetchedSize;

         iRet = EMECodecLib_InitHandleEx(&pvCodec, pvMemHdr, pvFileMgr, OEMTMLib_GetHCSC(pstTmHandle), pstTmHandle->pvAsyncMgr);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpecHdr--:  init store handle failed");
         EME_UTILS_BREAKNULL(pvCodec, iRet, EME_FAILED, "EME_CBPOP_FetchSpecHdr--:  init codec handle failed");

         /* decode mail */
         /* decode mail */
         stCodecOpts[0].iId = EMAILMESSAGE_OPT_SRC_TYPE;
         stCodecOpts[0].pVal = (WE_VOID*)EMAIL_ENCODE_BYBUFF;
         stCodecOpts[1].iId = EMIALMESSAGE_OPT_DECODE_TYPE;
         stCodecOpts[1].pVal = (WE_VOID*)EMAIL_DECODETYPE_HEADER;
         stCodecOpts[2].iId = EMAILMESSAGE_OPT_BUFF;
         stCodecOpts[2].pVal = (WE_VOID*)pstCBData->stActResult.pcDataBuffer;
         stCodecOpts[3].iId = EMAILOPT_END;

         iRet = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, (void*)pstCBData, (PFNDECODINGCB)EME_CBPOP_DecodingMsgCB);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpecHdr--:  decoding failed");

         pstTransEvt->pfnFreeSigData = EME_CBPOP_pfnFree;
         pstTransEvt->pvTransData = pstCBData;

//         iRet = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, NULL, NULL);
//         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpecHdr, decoding failed");
//         EME_CBPOP_DecodingMsgCB(pvCodec, pstCBData, EME_SUCCESS);
      }
      else if (pstActRet->iResult == EME_MAIL_NOTEXSITED)
      {
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, EME_MAIL_NOTEXSITED, pstTransEvt);
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, pstActRet->iResult, pstTransEvt);
      }

   } while(0);
   
   if (iRet != EME_SUCCESS)
   {
      if(NULL != pstCBData)
      {
         EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActResult.pcDataBuffer);
         EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActResult.pcFetchedID);
         EME_UTILS_FREEIF(pvMemHdr, pstCBData);
      }
      (void)EmailStore_DeleteMsg(pvStore, uiMsgId);
   }

//    EmailStore_DeleteHandle(pvStore);
//    pvStore = NULL;
//    EMECodecLib_DeleteHandle(&pvCodec);
   EME_UTILS_FREEIF(pvMemHdr, pstHeaderOpts);

   EME_UTILS_LOG_INFO(("EME_CBPOP_FetchSpecHdr--: End, Result = %d", iRet));
   return iRet;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBPOP_DeleteAllMail   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of delete one email from stack is success, 
                   trans fsm to POP_DELETE_ALLMAIL to go on to delete one email.
                3. else if the result cb of fetch one new email from stack is no new email,
                   trans fsm to POP_QUIT
 *              4. else trans fsm to TRANS_END
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBPOP_DeleteAllMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBPOP_DeleteAllMail actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS)
      {
         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_DELETE_ALLMAIL, pstTransEvt);
      }
      else if (pstActRet->iResult == EME_NO_NEWMAIL)
      {
         if(!pstActRet->bHasNewMail)
         {
            pstTransEvt->iErrCode = EME_NO_NEWMAIL;
         }
         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_QUIT, pstTransEvt);
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, pstActRet->iResult, pstTransEvt);
      }

   } while(0);
   

   return iRet;
}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBPOP_DeleteSpecMail   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of delete one email from stack is success, 
                   trans fsm to POP_QUIT with succss result.
                3. else if the result cb of fetch one new email from stack is no new email,
                   trans fsm to POP_QUIT with MAIL_NOTEXSITED result
 *              4. else trans fsm to TRANS_END
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBPOP_DeleteSpecMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBPOP_DeleteSpecMail actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS)
      {
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_QUIT, EME_SUCCESS, pstTransEvt);
      }
      else if (pstActRet->iResult == EME_MAIL_NOTEXSITED)
      {
         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_QUIT, pstTransEvt);
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, pstActRet->iResult, pstTransEvt);
      }

   } while(0);
   

   return iRet;
}
 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_CBPOP_Quit   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. if the result cb of quit from pop server from stack is success, 
                   trans fsm to TRANS_END with succss result.
 *              3. else trans fsm to TRANS_END with failed result.
 *  
 * ===================================================================================================*/
static WE_INT32 EME_CBPOP_Quit(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet)
{
   WE_INT32    iRet = EME_SUCCESS;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      EME_UTILS_LOG_INFO(("EME_CBPOP_Quit--: QUIT START"));
      pstTmHandle->iPOPState = EME_STACK_STATE_LOGOUT;
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBPOP_Quit, actret is null");
//       if (pstActRet->iResult == EME_SUCCESS)
//       {
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, EME_SUCCESS, pstTransEvt);
//      }
//       else
//       {
//          if (pstTransEvt != NULL)
//          {
//             pstTransEvt->iErrCode = EME_FAILED;
//          }
//          //EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, pstActRet->iResult, pstTransEvt);
//          /* QUIT failed maybe not a failed */
//          EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, EME_SUCCESS, pstTransEvt);
//       }

   } while(0);

   EME_UTILS_LOG_INFO(("EME_CBPOP_Quit--: QUIT END %d", iRet));
   return iRet;
}




/*************************************************************************************************************/
/* Free Functions */
/*************************************************************************************************************/
/*static WE_INT32 EME_FREEPOP_MailBuf(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt)
{
   WE_VOID*    pvMemHdr = NULL;
   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   EME_UTILS_FREEIF(pvMemHdr, pstTransEvt->pvTransData);

   return EME_SUCCESS;
}*/

static void EME_CBPOP_DecodingMsgCB(IEMSGHandle hEmailHandle, void *pvUserData, int iResult)
{
   St_CBDecodeData *pstCBData = (St_CBDecodeData*)pvUserData;
   WE_INT32        iRet = EME_SUCCESS;
   void            *pvCodec = hEmailHandle;
   WE_UINT32       uiPartsNum = 0;
   EmailOpt        *pstTmpOpts = NULL;
   EmailOpt        *pstOneOpt = NULL;
   WE_INT32        iOptNum = 0;
   EmailOpt        *pstHeaderOpts = NULL;
   St_TMHandle     *pstTmHandle = NULL;
   WE_BOOL         bSizeLimited = FALSE;
   WE_CHAR         *pcMsgId = NULL;
   WE_CHAR         acMsgId[9] = {0};
   EmailOpt        *pstBodyOpts = NULL;
   St_TransEvt     *pstTransEvt = NULL;
   St_PopActResult *pstActRet = NULL; 
   WE_INT32        iIsHeaderOnly = FALSE;

   WE_INT32        iIsKeepMail = TRUE;
   WE_VOID         *pvStore = NULL;

   WE_VOID         *pvMemHdr = NULL;

   WE_UINT32       i = 0;
   WE_UINT32       uiStatus = 0;

   EME_UTILS_LOG_INFO(("EME_CBPOP_DecodingMsgCB---: Start"));
   if(NULL == pvCodec || NULL == pstCBData)
   {
      EME_UTILS_LOG_ERROR(("EME_CBPOP_DecodingMsgCB--: Bad parameter and return"));
      EMECodecLib_DeleteHandle(&pvCodec);
      EME_UTILS_FREEIF(EME_UTILS_MEMMGR_FOR_FREE, pvUserData);
      return;
   }

   do 
   {
      pstTransEvt = (St_TransEvt*)pstCBData->pvTranEvent;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB--: TranEvent is null");
      pstTransEvt->pfnFreeSigData = NULL;
      pstTransEvt->pvTransData = NULL;

      pstTmHandle = (St_TMHandle*)pstCBData->pvTMHandle;
      EME_UTILS_BREAKNULL(pstTmHandle, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB--: TM handle is null");

      pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

      /* get file mgr */
//      pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
//      EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB--: get file mgr failed");

      pstActRet = (St_PopActResult*)&pstCBData->stActResult;
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB--: Act Ret is null");

      pvStore = pstCBData->pvStore;
      EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB--: Store handle is null");

      if(pstCBData->iDecodingType == EMAIL_DECODETYPE_WHOLEMSG)
      {
         EME_UTILS_LOG_INFO(("EME_CBPOP_DecodingMsgCB--: Spec mail"));
         if(pstCBData->bSpec)
         {
            EME_UTILS_BREAKIF(iResult, "EME_CBPOP_DecodingMsgCB--: Decoding Spec email failed");
            /* get message id */
            iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: get message uidl failed");
            EME_UTILS_BREAKNULL(pcMsgId, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB--: get message uidl failed");

            iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_POP_KEEP_MAIL, &iIsKeepMail);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: get keep mail failed");

            /* get mail's  parts and store the mail */
            iRet = EMECodecLib_GetNumOfParts(pvCodec, &uiPartsNum);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: get part's num failed");

            for (i = 0; i < uiPartsNum; i++)
            {
               iRet = EMECodecLib_GetPart(pvCodec, i, &pstBodyOpts);
               EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: get part opts failed");
               EME_UTILS_BREAKNULL(pstBodyOpts, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB--: get part opts failed");

               iRet = EmailStore_AddMsgPart(pvStore, pstCBData->uiMsgId, pstBodyOpts);
               EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: store part failed");
               pstBodyOpts = NULL;
            }
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: store part failed");

            /*update email down status*/
            iRet = EmailStore_SetMsgDownStatus(pvStore, pstCBData->uiMsgId, FALSE);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: set msg down state failed");
            /*update fresh status of email in pop uidl list*/  
            //          iRet =  EME_POP_TAG_MAIL(pstTmHandle->pvPOP, pstActRet->pcFetchedID, TRUE);
            //          EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchSpecMail, update uidl failed");

				iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_POP_KEEP_MAIL, &iIsKeepMail);
				EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: get keep mail failed");

            if (TRUE != iIsKeepMail)
            {
               EME_UTILS_FREEIF(pvMemHdr, pstTransEvt->pcUIDL);
               pstTransEvt->pcUIDL = EME_UTILS_STRDUP(pvMemHdr, pstActRet->pcFetchedID);
               EME_UTILS_BREAKNULL(pstTransEvt->pcUIDL, iRet, EME_NOMEMORY, "EME_CBPOP_DecodingMsgCB--: get uidl failed");
            }

            if (iRet == EME_SUCCESS)
            {
               /* the specified mail has been downloaded, so cancel is disabled */
               pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;

               EME_POP_UpdateUidlLocal(pstTmHandle->pvPOP);

               if (TRUE == iIsKeepMail)
               {
                  OEMTMLib_NotifyNewMail(pstTmHandle, pstTransEvt, pcMsgId, pstActRet->pcFetchedID);
                  EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_QUIT, pstTransEvt);
               }
               else
               {
                  OEMTMLib_NotifyNewMail(pstTmHandle, pstTransEvt, pcMsgId, pstActRet->pcFetchedID);
                  EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_UNKEEP_MAIL, pstTransEvt);
               }
            }
         }
         else
         {
            EME_UTILS_LOG_INFO(("EME_CBPOP_DecodingMsgCB--: All mails"));
            do 
            {
               if(EME_SUCCESS == iResult)
               {
                  EME_UTILS_MEMSET(&acMsgId, 0, 9);
                  EME_UTILS_SPRINTF(acMsgId, "%x", pstCBData->uiMsgId);
                  pcMsgId = acMsgId;

                  iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_POP_KEEP_MAIL, &iIsKeepMail);
                  EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: get keep mail failed");

                  /* get mail's header, parts and store the mail */
                  iRet = EMECodecLib_GetNumOfParts(pvCodec, &uiPartsNum);
                  EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: get part's num failed");

                  /*find the attachment part*/
                  for (i = 0; i < uiPartsNum; i++)
                  {
                     WE_INT32  iPartType = 0;
                     iRet = EMECodecLib_GetPart(pvCodec, i, &pstBodyOpts);
                     EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: get part opts failed");
                     EME_UTILS_BREAKNULL(pstBodyOpts, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB--: get part opts failed");

                     iRet = EME_GetIntOptValue((EMEOpt *)pstBodyOpts, EMAILPART_OPT_PART_TYPE, &iPartType);
                     if(iRet == EME_SUCCESS && iPartType == EMAILPART_PARTTYPE_ATTACHMENT)
                     {
                        uiStatus |= EMAIL_STATUS_WITH_ATTACH;
                        break;
                     }
                     pstBodyOpts = NULL;
                  }
                  EME_UTILS_BREAKIF(iRet, "find attachment count failed");
                  /*reset i value*/
                  pstBodyOpts = NULL;
                  i = 0;

                  iRet = EMECodecLib_GetHeader(pvCodec, &pstTmpOpts);
                  EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: get header opts failed");
                  EME_UTILS_BREAKNULL(pstTmpOpts, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB--: get header opts failed");

                  pstOneOpt = pstTmpOpts;
                  while (pstOneOpt->iId != EMAILOPT_END)
                  {
                     iOptNum++;
                     pstOneOpt++;
                  }

                  /* malloc opts, 1 for uidl and 1 for EMAILOPT_END and 1 for EMAIL_OPT_BEDOWN */
                  pstHeaderOpts = (EmailOpt*)EME_UTILS_MALLOC(pvMemHdr, (iOptNum + 4) * sizeof(EmailOpt));
                  EME_UTILS_BREAKNULL(pstHeaderOpts, iRet, EME_NOMEMORY, "EME_CBPOP_DecodingMsgCB--: no memory");
                  EME_UTILS_MEMSET(pstHeaderOpts, 0, (iOptNum + 3) * sizeof(EmailOpt));

                  /* if the size exceed, we only download the header */
                  EME_POP_GetSizeLimit(pstTmHandle->pvPOP, &bSizeLimited);
                  if (bSizeLimited)
                  {
                     iIsHeaderOnly = TRUE;
                  }
                  else
                  {
                     uiStatus |= EMAIL_STATUS_WITH_BODY;
                  }

                  EME_UTILS_MEMCPY(pstHeaderOpts, pstTmpOpts, iOptNum * sizeof(EmailOpt));
                  (pstHeaderOpts + iOptNum)->iId = EMAIL_OPT_UIDL;
                  (pstHeaderOpts + iOptNum)->pVal = (WE_VOID*)pstActRet->pcFetchedID;
                  (pstHeaderOpts + iOptNum + 1)->iId = EMAIL_OPT_BEDOWN;
                  (pstHeaderOpts + iOptNum + 1)->pVal = (WE_VOID*)iIsHeaderOnly;
                  (pstHeaderOpts + iOptNum + 2)->iId = EMAIL_OPT_STATUS;
                  (pstHeaderOpts + iOptNum + 2)->pVal = (WE_VOID*)uiStatus;
                  (pstHeaderOpts + iOptNum + 3)->iId = EMAILOPT_END;
                  /*add email header*/
                  iRet = EmailStore_AddMsgHeaders(pvStore, pstCBData->uiMsgId, pstHeaderOpts);
                  EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: store header failed");

                  /*add email body*/
                  for (i = 0; i < uiPartsNum; i++)
                  {  
                     iRet = EMECodecLib_GetPart(pvCodec, i, &pstBodyOpts);
                     EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: get part opts failed");
                     EME_UTILS_BREAKNULL(pstBodyOpts, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB--: get part opts failed");

                     iRet = EmailStore_AddMsgPart(pvStore, pstCBData->uiMsgId, pstBodyOpts);
                     EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: store part failed");
                     pstBodyOpts = NULL;
                  }
                  EME_UTILS_BREAKIF(iRet, "add email body failed");
               }
               else
               {
                  (void)EmailStore_DeleteMsg(pvStore, pstCBData->uiMsgId);
               }
               iResult = EME_SUCCESS;
               
               /*change the fresh status of this email in pop uidl list*/
               iRet =  EME_POP_TAG_MAIL(pstTmHandle->pvPOP, pstActRet->pcFetchedID, TRUE);
               EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: update uidl failed");

               /* this new mail has been downloaded, so cancel is disabled */
               pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;
               EME_POP_UpdateUidlLocal(pstTmHandle->pvPOP);

               if (TRUE != iIsKeepMail)
               {
                  EME_UTILS_FREEIF(pvMemHdr, pstTransEvt->pcUIDL);
                  pstTransEvt->pcUIDL = EME_UTILS_STRDUP(pvMemHdr, pstActRet->pcFetchedID);
                  EME_UTILS_BREAKNULL(pstTransEvt->pcUIDL, iRet, EME_NOMEMORY, "EME_CBPOP_DecodingMsgCB--: get uidl failed");
               }
               OEMTMLib_NotifyNewMail(pstTmHandle, pstTransEvt, pcMsgId, pstActRet->pcFetchedID);
            } while(0);

            if(iRet != EME_SUCCESS)
            {
               (void)EmailStore_DeleteMsg(pvStore, pstCBData->uiMsgId);
            }

            iRet = (iRet == EME_NOSTORAGE) ? EME_NOSTORAGE : EME_SUCCESS;
            EME_UTILS_BREAKIF(iRet, "no storage!!!");
            
            /*get the next new mail*/
            if(iIsKeepMail)
            {
               EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_FETCH_NEWMAILS, pstTransEvt);
            }
            else
            {
               EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_UNKEEP_MAIL, pstTransEvt);
            }
         }
      }
      else if(pstCBData->iDecodingType == EMAIL_DECODETYPE_HEADER)
      {
         EME_UTILS_LOG_INFO(("EME_CBPOP_DecodingMsgCB--: mail header"));
         do 
         {
            if (iResult == EME_SUCCESS)
            {
               EME_UTILS_MEMSET(&acMsgId, 0, 9);
               EME_UTILS_SPRINTF(acMsgId, "%x", pstCBData->uiMsgId);
               pcMsgId = acMsgId;

               iRet = EMECodecLib_GetHeader(pvCodec, &pstTmpOpts);
               EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: get header opts failed");
               EME_UTILS_BREAKNULL(pstTmpOpts, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB--: get header opts failed");

               iIsHeaderOnly = TRUE;

               pstOneOpt = pstTmpOpts;
               while (pstOneOpt->iId != EMAILOPT_END)
               {
                  iOptNum++;
                  pstOneOpt++;
               }

               /* malloc opts, 1 for uidl and 1 for EMAILOPT_END */
               pstHeaderOpts = (EmailOpt*)EME_UTILS_MALLOC(pvMemHdr, (iOptNum + 4) * sizeof(EmailOpt));
               EME_UTILS_BREAKNULL(pstHeaderOpts, iRet, EME_NOMEMORY, "EME_CBPOP_DecodingMsgCB--: no memory");
               EME_UTILS_MEMSET(pstHeaderOpts, 0, (iOptNum + 4) * sizeof(EmailOpt));

               EME_UTILS_MEMCPY(pstHeaderOpts, pstTmpOpts, iOptNum * sizeof(EmailOpt));
               (pstHeaderOpts + iOptNum)->iId = EMAIL_OPT_UIDL;
               (pstHeaderOpts + iOptNum)->pVal = (WE_VOID*)pstActRet->pcFetchedID;
               (pstHeaderOpts + iOptNum + 1)->iId = EMAIL_OPT_BEDOWN;
               (pstHeaderOpts + iOptNum + 1)->pVal = (WE_VOID*)iIsHeaderOnly;
               (pstHeaderOpts + iOptNum + 2)->iId = EMAIL_OPT_STATUS;
               (pstHeaderOpts + iOptNum + 2)->pVal = (WE_VOID*)uiStatus;
               (pstHeaderOpts + iOptNum + 3)->iId = EMAILOPT_END;

               iRet = EmailStore_AddMsgHeaders(pvStore, pstCBData->uiMsgId, pstHeaderOpts);
               EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: store header failed");
            }

            if(pstCBData->bSpec)
            {
               EME_UTILS_BREAKIF(iResult, "EME_CBPOP_DecodingMsgCB--: Decoding spec maile header failed");
               /* update uidl */
               iRet =  EME_POP_TAG_MAIL(pstTmHandle->pvPOP, pstActRet->pcFetchedID, TRUE);
               EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: update uidl failed");

               if (iRet == EME_SUCCESS)
               {
                  OEMTMLib_NotifyNewMail(pstTmHandle, pstTransEvt, pcMsgId, pstActRet->pcFetchedID);
                  EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_QUIT, pstTransEvt);
               }
            }
            else
            {
               /* update uidl */
               if(iResult != EME_SUCCESS)
               {
                  (void)EmailStore_DeleteMsg(pvStore, pstCBData->uiMsgId);
               }
               iResult = EME_SUCCESS;

               iRet =  EME_POP_TAG_MAIL(pstTmHandle->pvPOP, pstActRet->pcFetchedID, TRUE);
               EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB--: update uidl failed");
               pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;
               EME_POP_UpdateUidlLocal(pstTmHandle->pvPOP);
               OEMTMLib_NotifyNewMail(pstTmHandle, pstTransEvt, pcMsgId, pstActRet->pcFetchedID);
            }
         } while(0);

         /*error handle decoding the all mail header*/
         if(!pstCBData->bSpec)
         {
            if(iRet != EME_SUCCESS)
            {
               (void)EmailStore_DeleteMsg(pvStore, pstCBData->uiMsgId);
            }

            iRet = (EME_NOSTORAGE == iRet) ? EME_NOSTORAGE : EME_SUCCESS;
            EME_UTILS_BREAKIF(iRet, "no storage!!!");
            
            /*fetch the next new mail header*/
            EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_FETCH_NEWHDRS, pstTransEvt);
         }
      }
   } while(0);

   if(iResult != EME_SUCCESS || iRet != EME_SUCCESS)
   {
      if(pstCBData->iDecodingType == EMAIL_DECODETYPE_WHOLEMSG && !pstCBData->bSpec ||
         pstCBData->iDecodingType == EMAIL_DECODETYPE_HEADER)
      {
         /*delete message*/
         (void)EmailStore_DeleteMsg(pvStore, pstCBData->uiMsgId);
      }
      else
      {
         /*delete the unused objects*/
         EmailStore_WindUp(pvStore, pstCBData->uiMsgId);
      }

      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      
      if(iResult != EME_SUCCESS)
      {
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, iResult, pstTransEvt);
      }
      else
      {
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, iRet, pstTransEvt);
      }
   }

   EME_UTILS_FREEIF(pvMemHdr, pstHeaderOpts);
   
   if(NULL != pstCBData)
   {
      EME_DeleteFile(NULL, pstCBData->stActResult.pcDataBuffer);
      EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActResult.pcFetchedID);
      EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActResult.pcDataBuffer);
      EME_UTILS_FREEIF(pvMemHdr, pstCBData);
   }

   EmailStore_DeleteHandle(pvStore);
   pvStore = NULL;
   EMECodecLib_DeleteHandle(&pvCodec);

   EME_UTILS_LOG_INFO(("EME_CBPOP_DecodingMsgCB---: End, Result = %d", iResult));
}

static WE_INT32 
EME_FSMEPOP_Prof(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
	WE_INT32 iResult = EME_SUCCESS;
	IPOPHANDLER pvPOP = NULL;
	St_TransEvt *pstTransEvt = NULL;
	WE_VOID     *pvMemHdr = NULL;
	WE_CHAR     *pcConverCharset = NULL;
	WE_CHAR     *pcUseCompress = NULL;

	if (NULL == pstTmHandle || NULL == pstSig){

		return EME_FAILED;
	}

	pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

	do {

		pstTransEvt = (St_TransEvt *)pstSig->pvParam;
		EME_UTILS_BREAKNULL(pstTransEvt, iResult, EME_FAILED, "EME_FSMEPOP_Prof, trans evt error");

		pstTransEvt->iCurrSignalID = EME_SIG_EPOP_FETCH_PROF;
		
		pvPOP = pstTmHandle->pvPOP;
		EME_UTILS_BREAKNULL(pvPOP, iResult, EME_FAILED, "EME_FSMEPOP_Prof, pop handle error");

		iResult = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_EPOP_CONVERTCHARSET, &pcConverCharset);
		//EME_UTILS_BREAKIF(iResult, "EME_FSMEPOP_Prof, get charset failed");

		if (NULL == pcConverCharset || 0 == EME_UTILS_STRLEN(pcConverCharset)){

			pcConverCharset = "original";
		}
		
		iResult = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_EPOP_USECOMPRESS, &pcUseCompress);
		//EME_UTILS_BREAKIF(iResult, "EME_FSMEPOP_Prof, get compress failed");

		if (NULL == pcUseCompress || 0 == EME_UTILS_STRLEN(pcUseCompress)){

			pcUseCompress = "original";
		}

		iResult = EME_EPOP_Prof(pvPOP, pcConverCharset, pcUseCompress);
		EME_UTILS_BREAKIF(iResult, "EME_FSMEPOP_Prof, EME_EPOP_Prof failed");

		pstTransEvt->pfnFreeSigData = NULL;
		
	} while(0);

	if (EME_SUCCESS != iResult){

		if (NULL != pstTransEvt){

			pstTransEvt->iErrCode = EME_FAILED;
		}

		EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, iResult, pstTransEvt);
	}

	return iResult;
}

static WE_INT32 
EME_FSMEPOP_FetchNewMail(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   WE_INT32 iRet = EME_SUCCESS;
   IPOPHANDLER pvPOP = NULL;
   St_TransEvt *pstTransEvt = NULL;
   WE_VOID     *pvMemHdr = NULL;
   
   if (pstTmHandle == NULL || pstSig == NULL){
      
      return EME_FAILED;
   }
   
   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);
   
   do {
      
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMEPOP_FetchNewHeaders, transEvt is null");		
      
      pstTransEvt->iCurrSignalID = EME_SIG_EPOP_FETCH_NEWMAILS;
      
      if(!EME_DataAccess_IsEnoughSpace(EME_OEMTMLib_GetIFileMgr(pstTmHandle), EMS_LEAST_EML_SPACE))
      {
         iRet = EME_NOSTORAGE;
         EME_UTILS_BREAKIF(iRet, "no storeage for new mail");
      }
      
      pvPOP = pstTmHandle->pvPOP;
      EME_UTILS_BREAKNULL(pvPOP, iRet, EME_FAILED, "EME_FSMEPOP_FetchNewHeaders, pop handle is null");
      
      pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;
      
      iRet = EME_EPOP_FetchAllMails(pvPOP);
      EME_UTILS_BREAKIF(iRet, "EME_FSMEPOP_FetchNewHeaders, fetch all failed");
      
      pstTmHandle->iCanBeCancel = EME_CANCEL_ENABLED;
      
   }while(0);
   
   
   if (iRet != EME_SUCCESS){
      
      if (pstTransEvt != NULL){
         
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, iRet, pstTransEvt);
   }
   
   return iRet;
}

static WE_INT32 
EME_FSMEPOP_FetchNewHeaders(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
	WE_INT32 iRet = EME_SUCCESS;
	IPOPHANDLER pvPOP = NULL;
	St_TransEvt *pstTransEvt = NULL;
	WE_VOID     *pvMemHdr = NULL;
	
	if (pstTmHandle == NULL || pstSig == NULL){
		
		return EME_FAILED;
	}

	pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);
	
	do {

		pstTransEvt = (St_TransEvt*)pstSig->pvParam;
		EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMEPOP_FetchNewHeaders, transEvt is null");
		
		pstTransEvt->iCurrSignalID = EME_SIG_EPOP_FETCH_NEWHDRS;
		
      if(!EME_DataAccess_IsEnoughSpace(EME_OEMTMLib_GetIFileMgr(pstTmHandle), EMS_LEAST_EML_SPACE))
      {
         iRet = EME_NOSTORAGE;
         EME_UTILS_BREAKIF(iRet, "no storeage for new mail");
      }

		pvPOP = pstTmHandle->pvPOP;
		EME_UTILS_BREAKNULL(pvPOP, iRet, EME_FAILED, "EME_FSMEPOP_FetchNewHeaders, pop handle is null");
		
		pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;
		
		iRet = EME_EPOP_FetchAllMailHeaders(pvPOP);
		EME_UTILS_BREAKIF(iRet, "EME_FSMEPOP_FetchNewHeaders, fetch all failed");
		
		pstTmHandle->iCanBeCancel = EME_CANCEL_ENABLED;
		
	}while(0);
	
	
	if (iRet != EME_SUCCESS){
		
		if (pstTransEvt != NULL){

			pstTransEvt->iErrCode = EME_FAILED;
		}
		EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, iRet, pstTransEvt);
	}
	
	return iRet;
}

static WE_INT32 
EME_FSMEPOP_FetchSpecMail(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   WE_INT32    iRet = EME_SUCCESS;
   IPOPHANDLER pvPOP = NULL;
   St_TransEvt *pstTransEvt = NULL;
   WE_VOID     *pvMemHdr = NULL;
   WE_CHAR     *pcMsgUidl = NULL;
   WE_VOID     *pvStore = NULL;
   WE_VOID     *pvFileMgr = NULL;
   EmailOpt    *pstHeaderOpts = NULL;
   WE_UINT32   uiMsgId = 0;
   WE_CHAR     *pcMsgId = NULL;
   
   
   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }
   
   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);
   
   do {
      
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMEPOP_FetchSpecMail, transEvt is null");
      
      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_EPOP_FETCH_SPECMAIL;
      
      if(!EME_DataAccess_IsEnoughSpace(EME_OEMTMLib_GetIFileMgr(pstTmHandle), EMS_LEAST_EML_SPACE))
      {
         iRet = EME_NOSTORAGE;
         EME_UTILS_BREAKIF(iRet, "no storeage for new mail");
      }
      
      /* get pop handle */
      pvPOP = pstTmHandle->pvPOP;
      EME_UTILS_BREAKNULL(pvPOP, iRet, EME_FAILED, "EME_FSMEPOP_FetchSpecMail, pop handle is null");
      
      /* get file mgr */
      pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
      EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_FSMEPOP_FetchSpecMail,get file mgr failed");      
      
      /* get message id */
      iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
      EME_UTILS_BREAKIF(iRet, "EME_FSMEPOP_FetchSpecMail, get message uidl failed");
      
      uiMsgId = EME_UTILS_ATOX(pcMsgId);
      
      iRet = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
      EME_UTILS_BREAKIF(iRet, "EME_FSMEPOP_FetchSpecMail, init store handle failed");
      EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_CBPOP_FetchSpec, init store handle failed");
      
      iRet = EmailStore_GetMsgHeaders(pvStore, uiMsgId, &pstHeaderOpts);
      EME_UTILS_BREAKIF(iRet, "EME_FSMEPOP_FetchSpecMail, get message header failed");
      EME_UTILS_BREAKNULL(pstHeaderOpts, iRet, EME_FAILED, "EME_CBPOP_FetchSpec, get message header failed");
      
      iRet = EME_GetStrOptValue((EMEOpt*)pstHeaderOpts, EMAIL_OPT_UIDL, &pcMsgUidl);
      EME_UTILS_BREAKIF(iRet, "EME_FSMEPOP_FetchSpecMail, get UIDL failed");
      EME_UTILS_BREAKNULL(pcMsgUidl, iRet, EME_FAILED, "EME_CBPOP_FetchSpec, get UIDL failed");
      
      /* record pop callback data */
      pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;
      
      iRet = EME_EPOP_FetchSepcialMail(pvPOP, pcMsgUidl); 
      EME_UTILS_BREAKIF(iRet, "EME_FSMEPOP_FetchSpecMail, fetch specified failed");
      
   }while(0);
   
   
   EmailStore_DeleteHandle(pvStore);
   
   if (iRet != EME_SUCCESS){
      
      if (pstTransEvt != NULL){
         
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, iRet, pstTransEvt);
   }
   
   return iRet;
}

static WE_INT32 
EME_FSMEPOP_FetchSpecPart(St_TMHandle *pstTmHandle, St_EMEFsmSigData *pstSig)
{
   WE_INT32    iRet = EME_SUCCESS;
   IPOPHANDLER pvPOP = NULL;
   St_TransEvt *pstTransEvt = NULL;
   WE_VOID     *pvMemHdr = NULL;
   WE_CHAR     *pcMsgUidl = NULL;
   WE_VOID     *pvStore = NULL;
   WE_VOID     *pvFileMgr = NULL;
   EmailOpt    *pstHeaderOpts = NULL;
   WE_UINT32   uiMsgId = 0;
   WE_CHAR		*pcContentID = NULL;
   WE_CHAR		*pcConvertType = NULL;
   WE_CHAR		*pcPartID = NULL;
   EmailOpt    *pstMsgPartOpts = NULL;
   WE_CHAR     *pcMsgId = NULL;
   WE_INT32   iAttachSize = 0;
   
   
   if (pstTmHandle == NULL || pstSig == NULL)
   {
      return EME_FAILED;
   }
   
   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);
   
   do {
      
      /* get transEvt */
      pstTransEvt = (St_TransEvt*)pstSig->pvParam;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_FSMEPOP_FetchSpecPart, transEvt is null");
      
      /* record current signal */
      pstTransEvt->iCurrSignalID = EME_SIG_EPOP_FETCH_SPECMAIL;
      
      if(EME_DataAccess_IsEnoughSpace(EME_OEMTMLib_GetIFileMgr(pstTmHandle), EMS_LEAST_EML_SPACE))
      {
         iRet = EME_NOSTORAGE;
         EME_UTILS_BREAKIF(iRet, "no storeage for new mail");
      }
      
      /* get pop handle */
      pvPOP = pstTmHandle->pvPOP;
      EME_UTILS_BREAKNULL(pvPOP, iRet, EME_FAILED, "EME_FSMEPOP_FetchSpecPart, pop handle is null");
      
      /* get file mgr */
      pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
      EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_FSMEPOP_FetchSpecPart,get file mgr failed");      
      
      /* get message id */
      iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
      EME_UTILS_BREAKIF(iRet, "EME_FSMEPOP_FetchSpecPart, get message uidl failed");
      
      uiMsgId = EME_UTILS_ATOX(pcMsgId);
      
      iRet = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
      EME_UTILS_BREAKIF(iRet, "EME_FSMEPOP_FetchSpecPart, init store handle failed");
      EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_FSMEPOP_FetchSpecPart, init store handle failed");
      
      iRet = EmailStore_GetMsgHeaders(pvStore, uiMsgId, &pstHeaderOpts);
      EME_UTILS_BREAKIF(iRet, "EME_FSMEPOP_FetchSpecPart, get message header failed");
      EME_UTILS_BREAKNULL(pstHeaderOpts, iRet, EME_FAILED, "EME_FSMEPOP_FetchSpecPart, get message header failed");
      
      iRet = EME_GetStrOptValue((EMEOpt*)pstHeaderOpts, EMAIL_OPT_UIDL, &pcMsgUidl);
      EME_UTILS_BREAKIF(iRet, "EME_FSMEPOP_FetchSpecPart, get UIDL failed");
      EME_UTILS_BREAKNULL(pcMsgUidl, iRet, EME_FAILED, "EME_FSMEPOP_FetchSpecPart, get UIDL failed");
      
      iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_PART_ID, &pcPartID);
      EME_UTILS_BREAKIF(iRet, "EME_FSMEPOP_FetchSpecPart, get part id failed");
      EME_UTILS_BREAKNULL(pcPartID, iRet, EME_FAILED, "EME_FSMEPOP_FetchSpecPart, get Content id failed");
      
      
      iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_EPOP_CONVERTTYPE, &pcConvertType);
      EME_UTILS_BREAKIF(iRet, "EME_FSMEPOP_FetchSpecPart, get Content id failed");
      EME_UTILS_BREAKNULL(pcConvertType, iRet, EME_FAILED, "EME_FSMEPOP_FetchSpecPart, get Content id failed");
      
      /* get message part opts for getting content ID */
      iRet = EmailStore_GetMsgPart(pvStore, uiMsgId, EME_UTILS_ATOI(pcPartID), &pstMsgPartOpts);
      EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchSpecMail, get message part failed");
      EME_UTILS_BREAKNULL(pstMsgPartOpts, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecMail, get message part failed");
      
      EME_GetStrOptValue((EMEOpt*)pstMsgPartOpts, EMAILPART_OPT_CONTENT_ID, &pcContentID);
      EME_UTILS_BREAKNULL(pcContentID, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecMail, get pcContentID failed");
      
      EME_GetIntOptValue((EMEOpt*)pstMsgPartOpts, EMAILPART_OPT_X_ATTACHMENT_SIZE, &iAttachSize);
      
      /* record pop callback data */
      pstTmHandle->pvCbkUserData = (WE_VOID*)pstTransEvt;
      
      iRet = EME_EPOP_FetchSepcialMailPart(pvPOP, pcMsgUidl, pcContentID, pcConvertType, iAttachSize); 
      EME_UTILS_BREAKIF(iRet, "EME_FSMEPOP_FetchSpecMail, fetch specified failed");
      
#if 0   //set in dispatch.cpp
      // 3: downloading, ATTACHMENT_STATUS_DOWN_LOADING
      EME_UpdateOpt(pstMsgPartOpts, EMAILPART_OPT_X_ATTACHMENT, (WE_VOID*)2, NULL);
      EmailStore_UpdateMsgPart(pvStore, uiMsgId, EME_UTILS_ATOI(pcPartID), pstMsgPartOpts);
#endif
      
   }while(0);
   
   
   EmailStore_DeleteHandle(pvStore);
   
   if (iRet != EME_SUCCESS){
      
      if (pstTransEvt != NULL){
         
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, iRet, pstTransEvt);
   }
   
   return iRet;
}

static WE_INT32
EME_CBEPOP_Prof(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet)
{
	WE_INT32 iResult = EME_SUCCESS;
	WE_UINT32   uiSigId = 0;

	if (NULL == pstTmHandle || NULL == pstTransEvt){

		return EME_FAILED;
	}

	do{

		EME_UTILS_BREAKNULL(pstActRet, iResult, EME_FAILED, "EME_CBEPOP_Prof actret is null");

		if (EME_SUCCESS == pstActRet->iResult){

			switch(pstTransEvt->uiTransType)
			{
			case EME_TRANS_EPOP_FETCH_NEW_MAILS:
				uiSigId = EME_SIG_EPOP_FETCH_NEWMAILS;
				break;
			case EME_TRANS_EPOP_FETCH_SPEC_MAIL:
				uiSigId = EME_SIG_EPOP_FETCH_SPECMAIL;
				break;
			case EME_TRANS_EPOP_FETCH_SPEC_MAIL_PART:
				uiSigId = EME_SIG_EPOP_FETCH_SPECPART;
			    break;
			default:
				iResult = EME_FAILED;
			    break;
			}
			EME_UTILS_BREAKIF(iResult, "EME_CBEPOP_Prof, unknow trans type");

			EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, uiSigId, (WE_VOID*)pstTransEvt);
		}
		else{

			if (NULL != pstTransEvt){

				pstTransEvt->iErrCode = EME_FAILED;
			}
			EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, pstActRet->iResult, pstTransEvt);
		}

	}while (0);

	return iResult;
}

static WE_INT32 
EME_CBEPOP_FetchNewMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet)
{
	WE_INT32 iResult = EME_SUCCESS;
   WE_VOID     *pvMemHdr = NULL;
   WE_VOID     *pvCodec = NULL;
   WE_VOID     *pvFileMgr = NULL;
   WE_CHAR     *pcPath = NULL;
   WE_UINT32   uiMsgId = 0;
   WE_UINT32   uiAccountId = 0;
   WE_UINT32   uiDefBoxId = 0;
   EmailOpt    stCodecOpts[EME_CODEC_DECODE_OPT_NUM];
   EmailOpt    stStoreOpts [EME_STORE_INIT_OPT_NUM];
   St_CBDecodeData   *pstCBData = NULL;
   HEmailStore       pvStore = NULL;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

      EME_UTILS_BREAKNULL(pstActRet, iResult, EME_FAILED, "EME_CBEPOP_FetchNewMail actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS && 
         pstActRet->detail.stFetchAllRet.pcDataBuffer != NULL &&
         pstActRet->detail.stFetchAllRet.pcFetchedID != NULL)
      {
         EME_UTILS_MEMSET(stCodecOpts, 0, sizeof(EmailOpt) * EME_CODEC_DECODE_OPT_NUM);  
         EME_UTILS_MEMSET(&stStoreOpts, 0, EME_STORE_INIT_OPT_NUM * sizeof(EmailOpt));
         
         /* get account id and default folder and keep mail opt */
         iResult = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_ACCOUNT_ID, (WE_INT32*)&uiAccountId);
         EME_UTILS_BREAKIF(iResult, "EME_CBEPOP_FetchNewMail, get account id failed");
         
         iResult = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_DEFAULT_FOLDER_ID, (WE_INT32*)&uiDefBoxId);
         EME_UTILS_BREAKIF(iResult, "EME_CBEPOP_FetchNewMail, get default folder id  failed");

         /* get file mgr */
         pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
         EME_UTILS_BREAKNULL(pvFileMgr, iResult, EME_FAILED, "EME_CBEPOP_FetchNewMail,get file mgr failed");
         
         /* init store and codec handle */
         stStoreOpts[0].iId = EMAILMESSAGE_OPT_ACTID;
         stStoreOpts[0].pVal = (WE_VOID*)uiAccountId;
         stStoreOpts[1].iId = EMAILMESSAGE_OPT_DEF_INBOXID;
         stStoreOpts[1].pVal = (WE_VOID*)uiDefBoxId;
         stStoreOpts[2].iId = EMAILOPT_END;

         iResult = EmailStore_InitHandle(&pvStore, stStoreOpts, pvFileMgr,pvMemHdr);
         EME_UTILS_BREAKIF(iResult, "EME_CBPOP_FetchNewHdr, init store handle failed");
         EME_UTILS_BREAKNULL(pvStore, iResult, EME_FAILED, "EME_CBPOP_FetchNewHdr, init store handle failed");

         iResult = EmailStore_NewMessage(pvStore, &uiMsgId, &pcPath);
         EME_UTILS_BREAKIF(iResult, "Create new message failed");
         EME_UTILS_BREAKNULL(pcPath, iResult, EME_NOMEMORY, "Create path failed");

         iResult = EMECodecLib_InitHandleEx(&pvCodec, pvMemHdr, pvFileMgr, OEMTMLib_GetHCSC(pstTmHandle), pstTmHandle->pvAsyncMgr);
         EME_UTILS_BREAKIF(iResult, "EME_CBEPOP_FetchNewMail, init store handle failed");
         EME_UTILS_BREAKNULL(pvCodec, iResult, EME_FAILED, "EME_CBPOP_FetchSpec, init codec handle failed");

         pstCBData = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_CBDecodeData));
         EME_UTILS_BREAKNULL(pstCBData, iResult, EME_NOMEMORY, "no memory for cb data");
         EME_UTILS_MEMSET(pstCBData, 0, sizeof(St_CBDecodeData));

         pstCBData->iDecodingType = EMAIL_DECODETYPE_WHOLEMSG;
         pstCBData->pvTMHandle    = pstTmHandle;
         pstCBData->pvTranEvent   = pstTransEvt;
         pstCBData->pvUserData    = pstActRet;
         pstCBData->pvStore       = pvStore;
         pstCBData->pvCodec       = pvCodec;
         pstCBData->uiMsgId       = uiMsgId;
         pstCBData->bSpec         = FALSE;

         /*recode the act result*/
         pstCBData->stActResult.pcDataBuffer  = EME_UTILS_STRDUP(pvMemHdr, pstActRet->detail.stFetchAllRet.pcDataBuffer);
           
         /* decode mail */
         stCodecOpts[0].iId = EMAILMESSAGE_OPT_SRC_TYPE;
         stCodecOpts[0].pVal = (WE_VOID*)EMAIL_ENCODE_BYFILE;
         stCodecOpts[1].iId = EMIALMESSAGE_OPT_DECODE_TYPE;
         stCodecOpts[1].pVal = (WE_VOID*)EMAIL_DECODETYPE_WHOLEMSG;
         stCodecOpts[2].iId = EMAILMESSAGE_OPT_BUFF;
         stCodecOpts[2].pVal = (WE_VOID*)pstActRet->detail.stFetchAllRet.pcDataBuffer;
         stCodecOpts[3].iId = EMAILOPT_END;

//         iResult = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, (void*)pstCBData, (PFNDECODINGCB)EME_CBPOP_DecodingMsgCB);
//         EME_UTILS_BREAKIF(iResult, "EME_CBEPOP_FetchNewMail--: decoding message failed");
//         /*register the free function*/
//         pstTransEvt->pfnFreeSigData = EME_CBPOP_pfnFree;
//         pstTransEvt->pvTransData = pstCBData;

#if 0
         iResult = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, NULL, NULL);
         EME_UTILS_BREAKIF(iResult, "Decode email failed");
         EME_CBEPOP_DecodingMsgCB(pvCodec, pstCBData, EME_SUCCESS);
#else
         iResult = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, pstCBData, EME_CBEPOP_DecodingMsgCB);
         EME_UTILS_BREAKIF(iResult, "Decode email failed");
#endif
      }
      else if (pstActRet->iResult == EME_NO_NEWMAIL)
      {
         pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;
         if(!pstActRet->bHasNewMail)
         {
            pstTransEvt->iErrCode = EME_NO_NEWMAIL;
         }
         EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_QUIT, pstTransEvt);
      }
      else
      {
        if(pstActRet->iResult==EME_SERVER_RESPONSE_ERR)
        {
            /* update uidl */
            if(pstActRet->detail.stFetchAllRet.pcFetchedID != NULL)
            {
                EME_POP_TAG_MAIL(pstTmHandle->pvPOP, pstActRet->detail.stFetchAllRet.pcFetchedID, TRUE);
            }
            EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_EPOP_FETCH_NEWMAILS, pstTransEvt);
        }
        else
        {
            if (pstTransEvt != NULL)
            {
                pstTransEvt->iErrCode = EME_FAILED;
            }
            EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, pstActRet->iResult, pstTransEvt);
        }
       }

   } while(0);


   if(iResult != EME_SUCCESS)
   {
      EME_UTILS_FREEIF(pvMemHdr, pstCBData);
      (void)EmailStore_DeleteMsg(pvStore, uiMsgId);
      EMECodecLib_DeleteHandle(&pvCodec);
      EmailStore_DeleteHandle(pvStore);
      pvStore = NULL;
   }
	return iResult;
}

static WE_INT32 
EME_CBEPOP_FetchNewHeaders(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet)
{
	WE_INT32    iRet = EME_SUCCESS;
	WE_VOID     *pvMemHdr = NULL;
	WE_VOID     *pvCodec = NULL;
	WE_VOID     *pvFileMgr = NULL;
	EmailOpt    stStoreOpts[EME_STORE_INIT_OPT_NUM];
	EmailOpt    stCodecOpts[EME_CODEC_DECODE_OPT_NUM];
	WE_UINT32   uiAccountId = 0;
	WE_UINT32   uiDefBoxId = 0;
	WE_UINT32   uiMsgId = 0;
	WE_CHAR     *pcPath = NULL;
	WE_VOID     *pvStore = NULL;
	St_CBDecodeData* pstCBData = NULL;
	
	
	if (pstTmHandle == NULL || pstTransEvt == NULL)
	{
		return EME_FAILED;
	}
	
	do 
	{
		pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);
		
		EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBPOP_FetchNewHdr actret is null");
		
		if (pstActRet->iResult == EME_SUCCESS && 
			pstActRet->detail.stAllHeaderRet.pcDataBuffer != NULL &&
			pstActRet->detail.stAllHeaderRet.pcFetchedID != NULL)
		{
			EME_UTILS_MEMSET(stCodecOpts, 0, sizeof(EmailOpt) * EME_CODEC_DECODE_OPT_NUM);
			EME_UTILS_MEMSET(stStoreOpts, 0, sizeof(EmailOpt) * EME_STORE_INIT_OPT_NUM);   
			
			/* get file mgr */
			pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
			EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_CBPOP_FetchNewHdr,get file mgr failed");      
			
			/* get account id and default folder and keep mail opt */
			iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_ACCOUNT_ID, (WE_INT32*)&uiAccountId);
			EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewHdr, get account id failed");
			
			iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_DEFAULT_FOLDER_ID, (WE_INT32*)&uiDefBoxId);
			EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewHdr, get default folder id  failed");
			
			/* init store and codec handle */
			stStoreOpts[0].iId = EMAILMESSAGE_OPT_ACTID;
			stStoreOpts[0].pVal = (WE_VOID*)uiAccountId;
			stStoreOpts[1].iId = EMAILMESSAGE_OPT_DEF_INBOXID;
			stStoreOpts[1].pVal = (WE_VOID*)uiDefBoxId;
			stStoreOpts[2].iId = EMAILOPT_END;
			iRet = EmailStore_InitHandle(&pvStore, stStoreOpts, pvFileMgr,pvMemHdr);
			EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewHdr, init store handle failed");
			EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_CBPOP_FetchNewHdr, init store handle failed");
			
			iRet = EmailStore_NewMessage(pvStore, &uiMsgId, &pcPath);
			EME_UTILS_BREAKIF(iRet, "New message failed");
			EME_UTILS_BREAKNULL(pcPath, iRet, EME_FAILED, "create path failed");
			
			iRet = EMECodecLib_InitHandleEx(&pvCodec, pvMemHdr, pvFileMgr, OEMTMLib_GetHCSC(pstTmHandle), pstTmHandle->pvAsyncMgr);
			EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewHdr, init store handle failed");
			EME_UTILS_BREAKNULL(pvCodec, iRet, EME_FAILED, "EME_CBPOP_FetchNewHdr, init codec handle failed");
			
			pstCBData = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_CBDecodeData));
			EME_UTILS_BREAKNULL(pstCBData, iRet, EME_NOMEMORY, "no memory for cb data");
			EME_UTILS_MEMSET(pstCBData, 0, sizeof(St_CBDecodeData));
			
			pstCBData->iDecodingType = EMAIL_DECODETYPE_HEADER;
			pstCBData->pvTMHandle    = pstTmHandle;
			pstCBData->pvTranEvent   = pstTransEvt;
			pstCBData->pvUserData    = pstActRet;
			pstCBData->pvStore       = pvStore;
			pstCBData->pvCodec       = pvCodec;
			pstCBData->uiMsgId       = uiMsgId;
			pstCBData->bSpec         = FALSE;

                     /*recode the act result*/
                     pstCBData->stActResult.pcDataBuffer  = EME_UTILS_STRDUP(pvMemHdr, pstActRet->detail.stAllHeaderRet.pcDataBuffer);
			
			/* decode mail */
			stCodecOpts[0].iId = EMAILMESSAGE_OPT_SRC_TYPE;
			stCodecOpts[0].pVal = (WE_VOID*)EMAIL_ENCODE_BYFILE;
			stCodecOpts[1].iId = EMIALMESSAGE_OPT_DECODE_TYPE;
			stCodecOpts[1].pVal = (WE_VOID*)EMAIL_DECODETYPE_HEADER;
			stCodecOpts[2].iId = EMAILMESSAGE_OPT_BUFF;
			stCodecOpts[2].pVal = (WE_VOID*)pstActRet->detail.stAllHeaderRet.pcDataBuffer;
			stCodecOpts[3].iId = EMAILOPT_END;
			
			iRet = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, NULL, NULL);
			EME_UTILS_BREAKIF(iRet, "EME_CBPOP_FetchNewHdr, decoding failed");
			EME_CBEPOP_DecodingMsgCB(pvCodec, pstCBData, EME_SUCCESS);
		}
		else if (pstActRet->iResult == EME_NO_NEWMAIL)
		{
			pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;
         if(!pstActRet->bHasNewMail)                      
         {
            pstTransEvt->iErrCode = EME_NO_NEWMAIL;
         }
			EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_QUIT, pstTransEvt);
		}
		else
		{
			if (pstTransEvt != NULL)
			{
				pstTransEvt->iErrCode = EME_FAILED;
			}
			EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, pstActRet->iResult, pstTransEvt);
		}
		
	} while(0);
	
	
	if(EME_SUCCESS != iRet)
	{
		EME_UTILS_FREEIF(pvMemHdr, pstCBData);
		(void)EmailStore_DeleteMsg(pvStore, uiMsgId);
		EMECodecLib_DeleteHandle(&pvCodec);
		EmailStore_DeleteHandle(pvStore);
		pvStore = NULL;
	}
	
	
	return iRet;
}

static WE_INT32 
EME_CBEPOP_FetchSpecMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet)
{
   WE_INT32    iResult = EME_SUCCESS;
   WE_VOID     *pvMemHdr = NULL;
   WE_VOID     *pvFileMgr = NULL;
   WE_VOID     *pvCodec = NULL;
   WE_VOID     *pvStore = NULL;
   WE_UINT32   uiMsgId = 0;
   WE_CHAR     *pcPath = NULL;
   EmailOpt    stCodecOpts[EME_CODEC_DECODE_OPT_NUM];
   St_CBDecodeData* pstCBData = NULL;
   WE_CHAR     *pcMsgId = NULL;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

   do 
   {
      EME_UTILS_BREAKNULL(pstActRet, iResult, EME_FAILED, "EME_CBPOP_FetchSpecMail actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS && 
         pstActRet->detail.stFetchSpRet.pcDataBuffer != NULL &&
         pstActRet->detail.stFetchSpRet.pcFetchedID != NULL)
      {
         EME_UTILS_MEMSET(stCodecOpts, 0, sizeof(EmailOpt) * EME_CODEC_DECODE_OPT_NUM);
         /* get file mgr */
         pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
         EME_UTILS_BREAKNULL(pvFileMgr, iResult, EME_FAILED, "EME_CBPOP_FetchSpecMail,get file mgr failed");

         iResult = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
         EME_UTILS_BREAKIF(iResult, "EME_CBPOP_FetchSpecMail, get message id failed");

         uiMsgId = EME_UTILS_ATOX(pcMsgId);

         iResult = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
         EME_UTILS_BREAKIF(iResult, "EME_CBPOP_FetchSpecMail, init store handle failed");
         EME_UTILS_BREAKNULL(pvStore, iResult, EME_FAILED, "EME_CBPOP_FetchSpecMail, init store handle failed");     

         iResult = EmailStore_GetMsgPath(pvStore, uiMsgId, &pcPath);
         EME_UTILS_BREAKIF(iResult, "Get message path failed");
         EME_UTILS_BREAKNULL(pcPath, iResult, EME_FAILED, "get message path failed");

         iResult = EMECodecLib_InitHandleEx(&pvCodec, pvMemHdr, pvFileMgr, OEMTMLib_GetHCSC(pstTmHandle), pstTmHandle->pvAsyncMgr);
         EME_UTILS_BREAKIF(iResult, "EME_CBPOP_FetchSpecMail, init store handle failed");
         EME_UTILS_BREAKNULL(pvCodec, iResult, EME_FAILED, "EME_CBPOP_FetchSpecMail, init codec handle failed");

         pstCBData = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_CBDecodeData));
         EME_UTILS_BREAKNULL(pstCBData, iResult, EME_NOMEMORY, "no memory for cb data");
         EME_UTILS_MEMSET(pstCBData, 0, sizeof(St_CBDecodeData));

         pstCBData->iDecodingType = EMAIL_DECODETYPE_WHOLEMSG;
         pstCBData->pvTMHandle    = pstTmHandle;
         pstCBData->pvTranEvent   = pstTransEvt;
         pstCBData->pvUserData    = pstActRet;
         pstCBData->pvStore       = pvStore;
         pstCBData->pvCodec       = pvCodec;
         pstCBData->uiMsgId       = uiMsgId;
         pstCBData->bSpec         = TRUE;

         /*recode the act result*/
         pstCBData->stActResult.pcDataBuffer  = EME_UTILS_STRDUP(pvMemHdr, pstActRet->detail.stFetchSpRet.pcDataBuffer);

         /* decode mail */
         stCodecOpts[0].iId = EMAILMESSAGE_OPT_SRC_TYPE;
         stCodecOpts[0].pVal = (WE_VOID*)EMAIL_ENCODE_BYFILE;
         stCodecOpts[1].iId = EMIALMESSAGE_OPT_DECODE_TYPE;
         stCodecOpts[1].pVal = (WE_VOID*)EMAIL_DECODETYPE_WHOLEMSG;
         stCodecOpts[2].iId = EMAILMESSAGE_OPT_BUFF;
         stCodecOpts[2].pVal = (WE_VOID*)pstActRet->detail.stFetchSpRet.pcDataBuffer;
         stCodecOpts[3].iId = EMAILOPT_END;

//         iResult = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, (void*)pstCBData, (PFNDECODINGCB)EME_CBPOP_DecodingMsgCB);
//         EME_UTILS_BREAKIF(iResult, "EME_CBPOP_FetchSpecMail, decoding failed");
//
//         pstTransEvt->pfnFreeSigData = EME_CBPOP_pfnFree;
//         pstTransEvt->pvTransData = pstCBData;
         
         iResult = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, NULL, NULL);
         EME_UTILS_BREAKIF(iResult, "EME_CBPOP_FetchSpecMail, decoding failed");
         EME_CBEPOP_DecodingMsgCB(pvCodec, pstCBData, EME_SUCCESS);
      }
      else if (pstActRet->iResult == EME_MAIL_NOTEXSITED)
      {
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, EME_MAIL_NOTEXSITED, pstTransEvt);
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, pstActRet->iResult, pstTransEvt);
      }

   } while(0);
   if (iResult != EME_SUCCESS)
   {
      EME_UTILS_FREEIF(pvMemHdr, pstCBData);
      EMECodecLib_DeleteHandle(&pvCodec);
      EmailStore_DeleteHandle(pvStore);
      pvStore = NULL;
   }

	return iResult;
}

static WE_INT32 
EME_CBEPOP_FetchSpecPart(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet)
{
	WE_INT32 iResult = EME_SUCCESS;
   WE_VOID     *pvMemHdr = NULL;
   WE_VOID     *pvCodec = NULL;
   WE_VOID     *pvStore = NULL;
   WE_VOID     *pvFileMgr = NULL;
   EmailOpt    stCodecOpts[EME_CODEC_DECODE_OPT_NUM];
   WE_CHAR     acUid[EME_MAX_UINT32_STR_LEN];
   WE_CHAR     *pcCharSet = NULL;
   WE_CHAR     *pcEncoding = NULL;
   WE_CHAR     *pcFileName = NULL;
   WE_CHAR     *pcPartID = NULL;
   EmailOpt    *pstMsgPartOpts = NULL;

   WE_UINT32   uiMsgId = 0;
   WE_CHAR     *pcPath = NULL;
   St_CBDecodeData* pstCBData = NULL;
   WE_CHAR     *pcMsgId = NULL;
   WE_CHAR* pcConvertType = NULL;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

      EME_UTILS_BREAKNULL(pstActRet, iResult, EME_FAILED, "EME_CBIMAP_FetchSpecMail actret is null");
      
      if (pstActRet->iResult == EME_SUCCESS && 
          pstActRet->detail.stFetchSpRet.pcDataBuffer != NULL)
      {
         EME_UTILS_MEMSET(stCodecOpts, 0, sizeof(EmailOpt) * EME_CODEC_DECODE_OPT_NUM);  
         EME_UTILS_MEMSET(acUid, 0, sizeof(WE_CHAR) * EME_MAX_UINT32_STR_LEN);       

         /* get file mgr */
         pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
         EME_UTILS_BREAKNULL(pvFileMgr, iResult, EME_FAILED, "EME_CBIMAP_FetchSpecMail,get file mgr failed");      

         iResult = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
         EME_UTILS_BREAKIF(iResult, "EME_CBIMAP_FetchSpecMail, init store handle failed");
         EME_UTILS_BREAKNULL(pvStore, iResult, EME_FAILED, "EME_CBIMAP_FetchSpecMail, init store handle failed");

         iResult = EMECodecLib_InitHandleEx(&pvCodec, pvMemHdr, pvFileMgr, OEMTMLib_GetHCSC(pstTmHandle), pstTmHandle->pvAsyncMgr);
         EME_UTILS_BREAKIF(iResult, "EME_CBIMAP_FetchSpecMail, init store handle failed");
         EME_UTILS_BREAKNULL(pvCodec, iResult, EME_FAILED, "EME_CBIMAP_FetchSpecMail, init codec handle failed");
         
         /* get message id and part id */
         iResult = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
         EME_UTILS_BREAKIF(iResult, "EME_CBIMAP_FetchSpecMail, get message id failed");

         uiMsgId = EME_UTILS_ATOX(pcMsgId);

         iResult = EmailStore_GetMsgPath(pvStore, uiMsgId, &pcPath);
         EME_UTILS_BREAKIF(iResult, "Get message path failed");
         EME_UTILS_BREAKNULL(pcPath, iResult, EME_FAILED, "get message path failed");

         iResult = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_PART_ID, &pcPartID);
         EME_UTILS_BREAKIF(iResult, "EME_CBIMAP_FetchSpecMail, get message part id failed");
         EME_UTILS_BREAKNULL(pcPartID, iResult, EME_FAILED, "EME_CBIMAP_FetchSpecMail, get message id failed");

         /* get message part opts for getting encoding and charset */
         iResult = EmailStore_GetMsgPart(pvStore, uiMsgId, EME_UTILS_ATOI(pcPartID), &pstMsgPartOpts);
         EME_UTILS_BREAKIF(iResult, "EME_CBIMAP_FetchSpecMail, get message part failed");
         EME_UTILS_BREAKNULL(pstMsgPartOpts, iResult, EME_FAILED, "EME_CBIMAP_FetchSpecMail, get message part failed");

         /* get charset of the message part, it may be NULL if it is attachment */
         EME_GetStrOptValue((EMEOpt*)pstMsgPartOpts, EMAILPARAM_OPT_CHARSET, &pcCharSet);

         /* get encoding of the message part, it may be NULL if it is attachment */
         EME_GetStrOptValue((EMEOpt*)pstMsgPartOpts, EMAILPART_OPT_TRANS_ENCODING, &pcEncoding);

         EME_GetStrOptValue((EMEOpt*)pstMsgPartOpts, EMAILPARAM_OPT_FILE_NAME, &pcFileName);
		 
         pstCBData = EME_UTILS_MALLOC(pvMemHdr, sizeof(St_CBDecodeData));
         EME_UTILS_BREAKNULL(pstCBData, iResult, EME_NOMEMORY, "no memory for cb data");
         EME_UTILS_MEMSET(pstCBData, 0, sizeof(St_CBDecodeData));

         pstCBData->iDecodingType = EMAIL_DECODETYPE_MSGPART;
         pstCBData->pvTMHandle    = pstTmHandle;
         pstCBData->pvTranEvent   = pstTransEvt;
         pstCBData->pvUserData    = pstActRet;
         pstCBData->pvStore       = pvStore;
         pstCBData->pvCodec       = pvCodec;
         pstCBData->uiMsgId       = uiMsgId;
         pstCBData->bSpec         = TRUE;

         /*recode the act result*/
         pstCBData->stActResult.pcDataBuffer  = EME_UTILS_STRDUP(pvMemHdr, pstActRet->detail.stFetchSpRet.pcDataBuffer);

         iResult = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_EPOP_CONVERTTYPE, &pcConvertType);
         EME_UTILS_BREAKIF(iResult, "EME_FSMEPOP_FetchSpecPart, get Content id failed");
         EME_UTILS_BREAKNULL(pcConvertType, iResult, EME_FAILED, "EME_FSMEPOP_FetchSpecPart, get Content id failed");

         /* decode mail */
         if('+'==*pcConvertType)
         {
             stCodecOpts[0].iId = EMAILMESSAGE_OPT_SRC_TYPE;
             stCodecOpts[0].pVal = (WE_VOID*)EMAIL_ENCODE_BYFILE;
             stCodecOpts[1].iId = EMIALMESSAGE_OPT_DECODE_TYPE;
             stCodecOpts[1].pVal = (WE_VOID*)EMAIL_DECODETYPE_MSGPART_LIST;
             stCodecOpts[2].iId = EMAILMESSAGE_OPT_BUFF;
             stCodecOpts[2].pVal = (WE_VOID*)pstActRet->detail.stFetchSpRet.pcDataBuffer;
             stCodecOpts[3].iId = EMAILOPT_END;
         }
         else
         {

             stCodecOpts[0].iId = EMAILMESSAGE_OPT_SRC_TYPE;
             stCodecOpts[0].pVal = (WE_VOID*)EMAIL_ENCODE_BYFILE;
             stCodecOpts[1].iId = EMIALMESSAGE_OPT_DECODE_TYPE;
             stCodecOpts[1].pVal = (WE_VOID*)EMAIL_DECODETYPE_MSGPART;
             stCodecOpts[2].iId = EMAILMESSAGE_OPT_BUFF;
             stCodecOpts[2].pVal = (WE_VOID*)pstActRet->detail.stFetchSpRet.pcDataBuffer;
             stCodecOpts[3].iId = EMAILPARAM_OPT_CHARSET;
             stCodecOpts[3].pVal = (WE_VOID*)pcCharSet;
             stCodecOpts[4].iId = EMAILPART_OPT_TRANS_ENCODING;
             stCodecOpts[4].pVal = (WE_VOID*)pcEncoding;
             stCodecOpts[5].iId = EMAILPARAM_OPT_FILE_NAME;
             stCodecOpts[5].pVal = (WE_VOID*)pcFileName;
             stCodecOpts[6].iId = EMAILOPT_END;
         }
//          iResult = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, (void*)pstCBData, (PFNDECODINGCB)EME_CBPOP_DecodingMsgCB);
//          EME_UTILS_BREAKIF(iResult, "EME_CBIMAP_FetchSpecMail, decoding failed");
//          pstTransEvt->pfnFreeSigData = (PFNFREESIGDATA)EME_CBPOP_pfnFree;;
//          pstTransEvt->pvTransData    = pstCBData;

#if 0
         iResult = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, NULL, NULL);
         EME_UTILS_BREAKIF(iResult, "EME_CBIMAP_FetchSpecMail, decoding failed");

         EME_CBEPOP_DecodingMsgCB(pvCodec, pstCBData, SUCCESS);
#else
         iResult = EMECodecLib_DecodingEx(pvCodec, pcPath, stCodecOpts, pstCBData, EME_CBEPOP_DecodingMsgCB);
         EME_UTILS_BREAKIF(iResult, "Decode email failed");
#endif

         
      }
      else if (pstActRet->iResult == EME_MAIL_NOTEXSITED)
      {
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, EME_MAIL_NOTEXSITED, pstTransEvt);
      }
      else
      {

         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;

            /* get file mgr */
            pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
            EME_UTILS_BREAKNULL(pvFileMgr, iResult, EME_FAILED, "EME_CBEPOP_FetchSpecPart,get file mgr failed");      

            iResult = EmailStore_InitHandle(&pvStore, NULL, pvFileMgr,pvMemHdr);
            EME_UTILS_BREAKIF(iResult, "EME_CBEPOP_FetchSpecPart, init store handle failed");
            EME_UTILS_BREAKNULL(pvStore, iResult, EME_FAILED, "EME_CBEPOP_FetchSpecPart, init store handle failed");

            /* get message id and part id */
            iResult = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
            EME_UTILS_BREAKIF(iResult, "EME_CBEPOP_FetchSpecPart, get message id failed");

            uiMsgId = EME_UTILS_ATOX(pcMsgId);

            iResult = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_PART_ID, &pcPartID);
            EME_UTILS_BREAKIF(iResult, "EME_CBEPOP_FetchSpecPart, get message part id failed");
            EME_UTILS_BREAKNULL(pcPartID, iResult, EME_FAILED, "EME_CBEPOP_FetchSpecPart, get message id failed");

            /* get message part opts */
            iResult = EmailStore_GetMsgPart(pvStore, uiMsgId, EME_UTILS_ATOI(pcPartID), &pstMsgPartOpts);
            EME_UTILS_BREAKIF(iResult, "EME_CBEPOP_FetchSpecPart, get message part failed");
            EME_UTILS_BREAKNULL(pstMsgPartOpts, iResult, EME_FAILED, "EME_CBEPOP_FetchSpecPart, get message part failed");

            // FALSE: not download
            EME_UpdateOpt(pstMsgPartOpts, EMAILPART_OPT_X_ATTACHMENT, (WE_VOID*)FALSE, NULL);
            EmailStore_UpdateMsgPart(pvStore, uiMsgId, EME_UTILS_ATOI(pcPartID), pstMsgPartOpts);

            EmailStore_DeleteHandle(pvStore);

         }

         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, EME_FAILED, pstTransEvt);
      }

   } while(0);


	return iResult;

}

static WE_INT32 
EME_CBEPOP_SetPushMail(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet)
{
	WE_INT32 iResult = EME_SUCCESS;
   WE_VOID     *pvMemHdr = NULL;
   WE_VOID     *pvCodec = NULL;
   WE_VOID     *pvStore = NULL;
   WE_VOID     *pvFileMgr = NULL;
   WE_CHAR     *pcCharSet = NULL;
   WE_CHAR     *pcEncoding = NULL;
   WE_CHAR     *pcFileName = NULL;
   WE_CHAR     *pcPartID = NULL;
   EmailOpt    *pstMsgPartOpts = NULL;

   WE_UINT32   uiMsgId = 0;
   WE_CHAR     *pcPath = NULL;
   St_CBDecodeData* pstCBData = NULL;
   WE_CHAR     *pcMsgId = NULL;

   if (pstTmHandle == NULL || pstTransEvt == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      if (pstActRet->iResult == EME_SUCCESS)
      {
        EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_QUIT, pstTransEvt);
      }
      else
      {
         if (pstTransEvt != NULL)
         {
            pstTransEvt->iErrCode = EME_FAILED;
         }
         EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, EME_FAILED, pstTransEvt);
      }

   } while(0);


	return iResult;

}

static WE_INT32 EME_CBEPOP_GetPushMailFilter(St_TMHandle* pstTmHandle, St_TransEvt* pstTransEvt, St_POP_Ret* pstActRet)
{
    WE_INT32 iResult = EME_SUCCESS;
    WE_VOID         *pvMemHdr = NULL;

    if (pstTmHandle == NULL || pstTransEvt == NULL)
    {
        return EME_FAILED;
    }

    pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);
    do 
    {
        if (pstActRet->iResult == EME_SUCCESS)
        {
            if(NULL!=pstActRet->detail.stFetchAllRet.pcDataBuffer)
            {
                pstTmHandle->pcFilter = EME_UTILS_STRDUP(pvMemHdr, pstActRet->detail.stFetchAllRet.pcDataBuffer);
            }

            EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_QUIT, pstTransEvt);
        }
        else
        {
            pstTransEvt->iErrCode = EME_FAILED;
            EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, EME_FAILED, pstTransEvt);
        }

    } while(0);


    return iResult;

}

static void  EME_CBPOP_pfnFree(St_TMHandle *pstTMHandle, void* pvTransEvt)
{
   St_CBDecodeData *pstCBData = NULL;
   St_TransEvt     *pstTransEvt = (St_TransEvt*)pvTransEvt;
   WE_INT32        iRet = EME_SUCCESS;
   void            *pvMemHdr = NULL;

   EME_UTILS_LOG_INFO(("EME_CBPOP_pfnFree---: Start"));
   if(NULL == pstTMHandle || NULL == pvTransEvt)
   {
      EME_UTILS_LOG_ERROR(("EME_CBPOP_pfnFree--: Bad parameter and return"));
      return;
   }

   do 
   {
   	pstCBData = (St_CBDecodeData*)pstTransEvt->pvTransData;
      EME_UTILS_BREAKNULL(pstCBData, iRet, EME_FAILED, "pstCBData is null");
      pvMemHdr = EME_OEMTMLib_GetMemHdr(pstCBData->pvTMHandle);

      if(pstCBData->iDecodingType == EMAIL_DECODETYPE_WHOLEMSG && !pstCBData->bSpec ||
         pstCBData->iDecodingType == EMAIL_DECODETYPE_HEADER)
      {
         EmailStore_DeleteMsg(pstCBData->pvStore, pstCBData->uiMsgId);
      }
      else
      {
         /*delete the unused objects*/
         EmailStore_WindUp(pstCBData->pvStore, pstCBData->uiMsgId);
      }

      EmailStore_DeleteHandle(pstCBData->pvStore);
      EMECodecLib_DeleteHandle(&pstCBData->pvCodec);
      EME_DeleteFile(NULL, pstCBData->stActResult.pcDataBuffer);
      EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActResult.pcDataBuffer);
      EME_UTILS_FREEIF(pvMemHdr, pstCBData->stActResult.pcFetchedID);
      EME_UTILS_FREEIF(pvMemHdr, pstCBData);
   } while(0);

   EME_UTILS_LOG_INFO(("EME_CBPOP_pfnFree---: End, Result = %d", SUCCESS));
}

static void 
EME_CBEPOP_DecodingMsgCB(IEMSGHandle hEmailHandle, void *pvUserData, int iResult)
{
	St_CBDecodeData *pstCBData = (St_CBDecodeData*)pvUserData;
   WE_INT32        iRet = EME_SUCCESS;
   void            *pvCodec = hEmailHandle;
   WE_UINT32       uiPartsNum = 0;
   EmailOpt        *pstTmpOpts = NULL;
   EmailOpt        *pstOneOpt = NULL;
   WE_INT32        iOptNum = 0;
   EmailOpt        *pstHeaderOpts = NULL;
   St_TMHandle     *pstTmHandle = NULL;
   WE_BOOL         bSizeLimited = FALSE;
   EmailOpt        *pstBodyOpts = NULL;
   St_TransEvt     *pstTransEvt = NULL;
   St_POP_Ret      *pstActRet = NULL; 
   WE_INT32        iIsHeaderOnly = FALSE;
   WE_INT32        iIsKeepMail = TRUE;
   WE_VOID         *pvStore = NULL;
   WE_VOID         *pvMemHdr = NULL;
   WE_VOID         *pvFileMgr = NULL;
   WE_UINT32       i = 0;
   EmailOpt    *pstMsgPartOpts = NULL;
   WE_CHAR* pcFileName = NULL;
   WE_CHAR  *pcMsgId = NULL;
   WE_CHAR* pcContentID = NULL;
   WE_UINT32       uiStatus = 0;
   WE_CHAR         acMsgId[10] = {0};

   if(NULL == pvCodec || NULL == pstCBData)
   {
      /*EMECodecLib_DeleteHandle(&pvCodec);*/
      EME_UTILS_FREEIF(EME_UTILS_MEMMGR_FOR_FREE, pvUserData);
      return;
   }

   do 
   {
      pstTransEvt = (St_TransEvt*)pstCBData->pvTranEvent;
      EME_UTILS_BREAKNULL(pstTransEvt, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB--: TranEvent is null");
      pstTransEvt->pfnFreeSigData = NULL;
      pstTransEvt->pvTransData = NULL;

      pstTmHandle = (St_TMHandle*)pstCBData->pvTMHandle;
      EME_UTILS_BREAKNULL(pstTmHandle, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB, TM handle is null");

      pvMemHdr = EME_OEMTMLib_GetMemHdr(pstTmHandle);

      /* get file mgr */
      pvFileMgr = EME_OEMTMLib_GetIFileMgr(pstTmHandle);
      EME_UTILS_BREAKNULL(pvFileMgr, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB,get file mgr failed");

      pstActRet = (St_POP_Ret*)pstCBData->pvUserData;
      EME_UTILS_BREAKNULL(pstActRet, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB, Act Ret is null");

      pvStore = pstCBData->pvStore;
      EME_UTILS_BREAKNULL(pvStore, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB, Store handle is null");

   	if(pstCBData->iDecodingType == EMAIL_DECODETYPE_WHOLEMSG)
      {
         if(pstCBData->bSpec)
         {
            EME_UTILS_BREAKIF(iResult, "EME_CBPOP_DecodingMsgCB, Decoding Spec email failed");
            /* get message id */
            iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_MESSAGE_ID, &pcMsgId);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, get message uidl failed");
            
//            uiMsgId = EME_UTILS_ATOX(pcMsgId);

            /* get mail's  parts and store the mail */
            iRet = EMECodecLib_GetNumOfParts(pvCodec, &uiPartsNum);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, get part's num failed");
            
            for (i = 0; i < uiPartsNum; i++)
            {
               iRet = EMECodecLib_GetPart(pvCodec, i, &pstBodyOpts);
               EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, get part opts failed");
               EME_UTILS_BREAKNULL(pstBodyOpts, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB, get part opts failed");
               
               iRet = EmailStore_AddMsgPart(pvStore, pstCBData->uiMsgId, pstBodyOpts);
               EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, store part failed");
            }
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, store part failed");
            
            /*update email down status*/
            iRet = EmailStore_SetMsgDownStatus(pvStore, pstCBData->uiMsgId, FALSE);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, set msg down state failed");
            /*update fresh status of email in pop uidl list*/  
            //          iRet =  EME_POP_TAG_MAIL(pstTmHandle->pvPOP, pstActRet->detail.stFetchSpRet.pcFetchedID, TRUE);
            //          EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, update uidl failed");
            
            if (TRUE != iIsKeepMail)
            {
               EME_UTILS_FREEIF(pvMemHdr, pstTransEvt->pcUIDL);
               pstTransEvt->pcUIDL = EME_UTILS_STRDUP(pvMemHdr, pstActRet->detail.stFetchAllRet.pcFetchedID);
               EME_UTILS_BREAKNULL(pstTransEvt->pcUIDL, iRet, EME_NOMEMORY, "EME_CBPOP_DecodingMsgCB, get uidl failed");
            }
            
            if (iRet == EME_SUCCESS)
            {
               /* the specified mail has been downloaded, so cancel is disabled */
               pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;

               EME_POP_UpdateUidlLocal(pstTmHandle->pvPOP);
               
               EME_UTILS_MEMSET(&acMsgId, 0, 10);
               EME_UTILS_SPRINTF(acMsgId, "%x", pstCBData->uiMsgId);
               if (TRUE == iIsKeepMail)
               {
                  OEMTMLib_NotifyNewMail(pstTmHandle, pstTransEvt, acMsgId, pstActRet->detail.stFetchSpRet.pcFetchedID);
                  EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_QUIT, pstTransEvt);
               }
               else
               {
                  OEMTMLib_NotifyNewMail(pstTmHandle, pstTransEvt, acMsgId, pstActRet->detail.stFetchSpRet.pcFetchedID);
                  EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_UNKEEP_MAIL, pstTransEvt);
               }
            }
         }
         else
         {
            if(EME_SUCCESS == iResult)
            {
              EME_UTILS_MEMSET(&acMsgId, 0, 10);
              EME_UTILS_SPRINTF(acMsgId, "%d", pstCBData->uiMsgId);
              pcMsgId = acMsgId;
               
               iRet = EME_GetIntOptValue(pstTransEvt->pstAccountOpts, EME_OPTID_POP_KEEP_MAIL, &iIsKeepMail);
               EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, get keep mail failed");
               
               /* get mail's header, parts and store the mail */
               iRet = EMECodecLib_GetNumOfParts(pvCodec, &uiPartsNum);
               EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, get part's num failed");

                /*find the attachment part*/
                for (i = 0; i < uiPartsNum; i++)
                {
                    WE_INT32  iPartType = 0;
                    iRet = EMECodecLib_GetPart(pvCodec, i, &pstBodyOpts);
                    EME_UTILS_BREAKIF(iRet, "EME_CBEPOP_DecodingMsgCB--: get part opts failed");
                    EME_UTILS_BREAKNULL(pstBodyOpts, iRet, EME_FAILED, "EME_CBEPOP_DecodingMsgCB--: get part opts failed");

                    iRet = EME_GetIntOptValue((EMEOpt *)pstBodyOpts, EMAILPART_OPT_PART_TYPE, &iPartType);
                    if(iRet == EME_SUCCESS && iPartType == EMAILPART_PARTTYPE_ATTACHMENT)
                    {
                        uiStatus |= EMAIL_STATUS_WITH_ATTACH;
                        break;
                    }
                    pstBodyOpts = NULL;
                }
                EME_UTILS_BREAKIF(iRet, "EME_CBEPOP_DecodingMsgCB, find attachment count failed");
               
               iRet = EMECodecLib_GetHeader(pvCodec, &pstTmpOpts);
               EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, get header opts failed");
               EME_UTILS_BREAKNULL(pstTmpOpts, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB, get header opts failed");
               
               pstOneOpt = pstTmpOpts;
               while (pstOneOpt->iId != EMAILOPT_END)
               {
                  iOptNum++;
                  pstOneOpt++;
               }
               
               /* malloc opts, 1 for uidl and 1 for EMAILOPT_END and 1 for EMAIL_OPT_BEDOWN, 1 for EMAIL_OPT_STATUS */
               pstHeaderOpts = (EmailOpt*)EME_UTILS_MALLOC(pvMemHdr, (iOptNum + 4) * sizeof(EmailOpt));
               EME_UTILS_BREAKNULL(pstHeaderOpts, iRet, EME_NOMEMORY, "EME_CBPOP_DecodingMsgCB, no memory");
               EME_UTILS_MEMSET(pstHeaderOpts, 0, (iOptNum + 4) * sizeof(EmailOpt));
               
               /* if the size exceed, we only download the header */
               EME_POP_GetSizeLimit(pstTmHandle->pvPOP, &bSizeLimited);
               if (bSizeLimited)
               {
                  iIsHeaderOnly = TRUE;
               }
              else
              {
                 uiStatus |= EMAIL_STATUS_WITH_BODY;
              }
               
               EME_UTILS_MEMCPY(pstHeaderOpts, pstTmpOpts, iOptNum * sizeof(EmailOpt));
               (pstHeaderOpts + iOptNum)->iId = EMAIL_OPT_UIDL;
               (pstHeaderOpts + iOptNum)->pVal = (WE_VOID*)pstActRet->detail.stFetchAllRet.pcFetchedID;
               (pstHeaderOpts + iOptNum + 1)->iId = EMAIL_OPT_BEDOWN;
               (pstHeaderOpts + iOptNum + 1)->pVal = (WE_VOID*)iIsHeaderOnly;
               (pstHeaderOpts + iOptNum + 2)->iId = EMAIL_OPT_STATUS;
               (pstHeaderOpts + iOptNum + 2)->pVal = (WE_VOID*)uiStatus;
               (pstHeaderOpts + iOptNum + 3)->iId = EMAILOPT_END;

               /*add email header*/
               iRet = EmailStore_AddMsgHeaders(pvStore, pstCBData->uiMsgId, pstHeaderOpts);
               EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, store header failed");
               
               /*add email body*/
               for (i = 0; i < uiPartsNum; i++)
               {
                  iRet = EMECodecLib_GetPart(pvCodec, i, &pstBodyOpts);
                  EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, get part opts failed");
                  EME_UTILS_BREAKNULL(pstBodyOpts, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB, get part opts failed");
                  
                  iRet = EmailStore_AddMsgPart(pvStore, pstCBData->uiMsgId, pstBodyOpts);
                  EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, store part failed");
               }
            }
            else
            {
               (void)EmailStore_DeleteMsg(pvStore, pstCBData->uiMsgId);
            }

            iResult = EME_SUCCESS;
            
            /*change the fresh status of this email in pop uidl list*/
            iRet =  EME_POP_TAG_MAIL(pstTmHandle->pvPOP, pstActRet->detail.stFetchAllRet.pcFetchedID, TRUE);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, update uidl failed");
            
            if (TRUE != iIsKeepMail)
            {
               EME_UTILS_FREEIF(pvMemHdr, pstTransEvt->pcUIDL);
               pstTransEvt->pcUIDL = EME_UTILS_STRDUP(pvMemHdr, pstActRet->detail.stFetchAllRet.pcFetchedID);
               EME_UTILS_BREAKNULL(pstTransEvt->pcUIDL, iRet, EME_NOMEMORY, "EME_CBPOP_DecodingMsgCB, get uidl failed");
            }
            
            if (iRet == EME_SUCCESS)
            {
               /* this new mail has been downloaded, so cancel is disabled */
//               pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;
               
               if (TRUE == iIsKeepMail)
               {
                  OEMTMLib_NotifyNewMail(pstTmHandle, pstTransEvt, pcMsgId, pstActRet->detail.stFetchAllRet.pcFetchedID);
                  EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_EPOP_FETCH_NEWMAILS, pstTransEvt);
                  //EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_EPOP_FETCH_PROF, pstTransEvt);
               }
               else
               {
                  OEMTMLib_NotifyNewMail(pstTmHandle, pstTransEvt, pcMsgId, pstActRet->detail.stFetchAllRet.pcFetchedID);
                  EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_UNKEEP_MAIL, pstTransEvt);
               }
            }
         }
      }
      else if(pstCBData->iDecodingType == EMAIL_DECODETYPE_HEADER)
      {
         if (iResult == EME_SUCCESS)
         {            
            iRet = EMECodecLib_GetHeader(pvCodec, &pstTmpOpts);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, get header opts failed");
            EME_UTILS_BREAKNULL(pstTmpOpts, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB, get header opts failed");
            
            iIsHeaderOnly = TRUE;

            pstOneOpt = pstTmpOpts;
            while (pstOneOpt->iId != EMAILOPT_END)
            {
               iOptNum++;
               pstOneOpt++;
            }
            
            /* malloc opts, 1 for uidl and 1 for EMAILOPT_END and 1 for EMAIL_OPT_BEDOWN, 1 for EMAIL_OPT_STATUS */
            pstHeaderOpts = (EmailOpt*)EME_UTILS_MALLOC(pvMemHdr, (iOptNum + 4) * sizeof(EmailOpt));
            EME_UTILS_BREAKNULL(pstHeaderOpts, iRet, EME_NOMEMORY, "EME_CBPOP_DecodingMsgCB, no memory");
            EME_UTILS_MEMSET(pstHeaderOpts, 0, (iOptNum + 4) * sizeof(EmailOpt));
            
            EME_UTILS_MEMCPY(pstHeaderOpts, pstTmpOpts, iOptNum * sizeof(EmailOpt));
            (pstHeaderOpts + iOptNum)->iId = EMAIL_OPT_UIDL;
            (pstHeaderOpts + iOptNum)->pVal = (WE_VOID*)pstActRet->detail.stAllHeaderRet.pcFetchedID;
            (pstHeaderOpts + iOptNum + 1)->iId = EMAIL_OPT_BEDOWN;
            (pstHeaderOpts + iOptNum + 1)->pVal = (WE_VOID*)iIsHeaderOnly;
            (pstHeaderOpts + iOptNum + 2)->iId = EMAIL_OPT_STATUS;
            (pstHeaderOpts + iOptNum + 2)->pVal = (WE_VOID*)uiStatus;
            (pstHeaderOpts + iOptNum + 3)->iId = EMAILOPT_END;

            
            iRet = EmailStore_AddMsgHeaders(pvStore, pstCBData->uiMsgId, pstHeaderOpts);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, store header failed");
         }

         if(pstCBData->bSpec)
         {
            EME_UTILS_BREAKIF(iResult, "Decoding spec maile header failed");
            /* update uidl */
            iRet =  EME_POP_TAG_MAIL(pstTmHandle->pvPOP, pstActRet->detail.stSpHeaderRet.pcFetchedID, TRUE);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, update uidl failed");
            
            if (iRet == EME_SUCCESS)
            {
               OEMTMLib_NotifyNewMail(pstTmHandle, pstTransEvt, pcMsgId, pstActRet->detail.stSpHeaderRet.pcFetchedID);
               EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_QUIT, pstTransEvt);
            }
         }
         else
         {
            /* update uidl */
            if(iResult != EME_SUCCESS)
            {
               (void)EmailStore_DeleteMsg(pvStore, pstCBData->uiMsgId);
            }
            iResult = EME_SUCCESS;

            iRet =  EME_POP_TAG_MAIL(pstTmHandle->pvPOP, pstActRet->detail.stAllHeaderRet.pcFetchedID, TRUE);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, update uidl failed");
            if (iRet == EME_SUCCESS)
            {
               pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;
               
               OEMTMLib_NotifyNewMail(pstTmHandle, pstTransEvt, pcMsgId, pstActRet->detail.stAllHeaderRet.pcFetchedID);
               EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_EPOP_FETCH_NEWHDRS, pstTransEvt);
            }
         }
      }
      else if(pstCBData->iDecodingType == EMAIL_DECODETYPE_MSGPART)
      {
         /*store message part*/
         WE_CHAR* pcPartDataOfCodec = NULL;
         WE_INT32 iBufferSize = 0;
         EmailOpt pstMsgPartOptsForUpdate[EME_POP_UPDATE_PART_NUM];
         WE_CHAR* pcPartID = NULL;
         WE_CHAR* pcConvertType = NULL;
         WE_CHAR acFileName[256] = {0};
         
         EME_UTILS_BREAKIF(iResult, "Decoding part failed");
         EME_UTILS_MEMSET(&pstMsgPartOptsForUpdate, 0, sizeof(EmailOpt) * EME_IMAP_UPDATE_PART_NUM);
         
         iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_PART_ID, &pcPartID);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, get message part id failed");
         EME_UTILS_BREAKNULL(pcPartID, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB, get message id failed");
         
         /* get mail's header, parts and store the mail */
         iRet = EMECodecLib_GetNumOfParts(pvCodec, &uiPartsNum);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, get part's num failed");

         iRet = EME_GetStrOptValue(pstTransEvt->pstTransOpts, EME_OPTID_EPOP_CONVERTTYPE, &pcConvertType);
         EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, get cvt type failed");
         EME_UTILS_BREAKNULL(pcConvertType, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB, get cvt type failed");

         /* decode mail */
         if('+' != *pcConvertType)         
         {
            iRet = EMECodecLib_GetPart(pvCodec, i, &pstBodyOpts);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, get part opts failed");
            EME_UTILS_BREAKNULL(pstBodyOpts, iRet, EME_FAILED, "EME_CBIMAP_FetchNewMails, get part opts failed");
            
            /* get part data buffer from codec */
            iRet = EME_GetStrOptValue((EMEOpt*)pstBodyOpts, EMAILPART_OPT_DATA_BUFF, &pcPartDataOfCodec);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, get message part from codec failed");
            EME_UTILS_BREAKNULL(pcPartDataOfCodec, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB, get part data buffer failed");
            
            /* get part data size from codec */
            iRet = EME_GetIntOptValue((EMEOpt*)pstBodyOpts, EMAILPART_OPT_DATA_SIZE, &iBufferSize);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, get message part size from codec failed");
            
            pstOneOpt = pstBodyOpts;
            while (pstOneOpt->iId != EMAILOPT_END)
            {
               iOptNum++;
               pstOneOpt++;
            }

            iRet = EmailStore_GetMsgPart(pvStore, pstCBData->uiMsgId, EME_UTILS_ATOI(pcPartID), &pstMsgPartOpts);
            EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchSpecMail, get message part failed");
            EME_UTILS_BREAKNULL(pstMsgPartOpts, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecMail, get message part failed");

            /* del old empty file */
            EME_GetStrOptValue((EMEOpt*)pstMsgPartOpts, EMAILPART_OPT_DATA_BUFF, &pcFileName);
            if(pcFileName)
            {
                EME_DeleteFile(NULL, pcFileName);
                pcFileName = NULL;
            }

            EME_GetStrOptValue((EMEOpt*)pstMsgPartOpts, EMAILPARAM_OPT_FILE_NAME, &pcFileName);
            EME_GetStrOptValue((EMEOpt*)pstMsgPartOpts, EMAILPART_OPT_CONTENT_ID, &pcContentID);

            if( NULL != pcConvertType )
            {
                WE_CHAR* pcTmp = NULL;
                WE_INT32 iLen = 0;
                if( 0!= EME_UTILS_STRCMPNC(pcConvertType, "original"))
                {
                    if(('+' == *pcConvertType)||('-' == *pcConvertType))
                    {
                        pcConvertType++;
                    }
                    pcTmp = EME_UTILS_STRRCHR(pcPartDataOfCodec, '.');
                    if(NULL!=pcTmp)
                    {
                        iLen = pcTmp - pcPartDataOfCodec + 1;

                        EME_UTILS_SNPRINTF(acFileName, iLen, "%s", pcPartDataOfCodec);
                        EME_UTILS_STRCAT(acFileName, pcConvertType);
                    }
                    else
                    {
                        EME_UTILS_SPRINTF(acFileName, "%s.%s", pcPartDataOfCodec, pcConvertType);
                    }
                    if(EME_SUCCESS==EME_Rename(pcPartDataOfCodec, acFileName))
                    {
                        pcPartDataOfCodec = acFileName;
                    }
                }
            }
            
            /* malloc opts, 1 for data buffer and 1 for EMAILOPT_END */
            pstMsgPartOptsForUpdate[0].iId  = EMAILPART_OPT_DATA_BUFF;
            pstMsgPartOptsForUpdate[0].pVal = (WE_VOID*)pcPartDataOfCodec;
            pstMsgPartOptsForUpdate[1].iId  = EMAILPART_OPT_DATA_SIZE;
            pstMsgPartOptsForUpdate[1].pVal = (WE_VOID*)iBufferSize;
            pstMsgPartOptsForUpdate[2].iId  = EMAILPART_OPT_X_ATTACHMENT;
            pstMsgPartOptsForUpdate[2].pVal = (WE_VOID*)TRUE;
            pstMsgPartOptsForUpdate[3].iId  = EMAILPART_OPT_SRC_TYPE;
            pstMsgPartOptsForUpdate[3].pVal = (WE_VOID*)EMAILPART_SRCTYPE_FILE;
            pstMsgPartOptsForUpdate[4].iId  = EMAILPART_OPT_PART_TYPE;
            pstMsgPartOptsForUpdate[4].pVal = (WE_VOID*)EMAILPART_PARTTYPE_ATTACHMENT;
            pstMsgPartOptsForUpdate[5].iId  = EMAILPARAM_OPT_FILE_NAME;
            pstMsgPartOptsForUpdate[5].pVal = (WE_VOID*)pcFileName;
            pstMsgPartOptsForUpdate[6].iId  = EMAILPART_OPT_CONTENT_ID;
            pstMsgPartOptsForUpdate[6].pVal = (WE_VOID*)pcContentID;
            pstMsgPartOptsForUpdate[7].iId  = EMAILOPT_END;


            iRet = EmailStore_UpdateMsgPart(pvStore, pstCBData->uiMsgId, EME_UTILS_ATOI(pcPartID), pstMsgPartOptsForUpdate);
            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, store part failed");
         }
         else
         {
             /* add all part */
            for (i = 0; i < uiPartsNum; i++)
            {
                iRet = EMECodecLib_GetPart(pvCodec, i, &pstBodyOpts);
                EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, get part opts failed");
                EME_UTILS_BREAKNULL(pstBodyOpts, iRet, EME_FAILED, "EME_CBPOP_DecodingMsgCB, get part opts failed");

                iRet = EmailStore_AddMsgPart(pvStore, pstCBData->uiMsgId, pstBodyOpts);
                EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, store part failed");
            }

            /* update part status */
            iRet = EmailStore_GetMsgPart(pvStore, pstCBData->uiMsgId, EME_UTILS_ATOI(pcPartID), &pstMsgPartOpts);
            EME_UTILS_BREAKIF(iRet, "EME_CBIMAP_FetchSpecMail, get message part failed");
            EME_UTILS_BREAKNULL(pstMsgPartOpts, iRet, EME_FAILED, "EME_CBIMAP_FetchSpecMail, get message part failed");

            /* down sub file list, ATTACHMENT_STATUS_IS_DOWN_LIST */
            EME_UpdateOpt(pstMsgPartOpts, EMAILPART_OPT_X_ATTACHMENT, (WE_VOID*)3, NULL);
            EmailStore_UpdateMsgPart(pvStore, pstCBData->uiMsgId, EME_UTILS_ATOI(pcPartID), pstMsgPartOpts);

            EME_UTILS_BREAKIF(iRet, "EME_CBPOP_DecodingMsgCB, store part failed");
         }

         if (iRet == EME_SUCCESS)
         {
            /* The specified part has been downloaded, so cancel was disabled */
            pstTmHandle->iCanBeCancel = EME_CANCEL_DISABLED;
            
            //EME_UTILS_SPRINTF(acUid, "%u", pstActRet->unRetData.stMailPart.uiUID);
            //OEMTMLib_NotifyNewMail(pstTmHandle, pstTransEvt, pcMsgId, acUid);
            EME_EVT_SENDTO_SELF_P(pstTmHandle, EME_FSM_POP, EME_SIG_POP_QUIT, pstTransEvt);
         }
      }
      else
      {
         iResult = EME_FAILED;
      }
   } while(0);

   if(iResult != EME_SUCCESS || iRet != EME_SUCCESS)
   {
      (void)EmailStore_DeleteMsg(pvStore, pstCBData->uiMsgId);
      if (pstTransEvt != NULL)
      {
         pstTransEvt->iErrCode = EME_FAILED;
      }
      EME_EVT_SENDTO_SELF_IP(pstTmHandle, EME_FSM_POP, EME_SIG_POP_TRANS_END, iResult, pstTransEvt);
   }

   EME_UTILS_FREEIF(pvMemHdr, pstHeaderOpts);
   if(NULL != pstCBData->stActResult.pcDataBuffer)
   {
         EME_DeleteFile(NULL, pstCBData->stActResult.pcDataBuffer);
         EME_UTILS_FREE(pvMemHdr, pstCBData->stActResult.pcDataBuffer);
   }
   EME_UTILS_FREEIF(pvMemHdr, pvUserData);
   EmailStore_DeleteHandle(pvStore);
   pvStore = NULL;
   EMECodecLib_DeleteHandle(&pvCodec);
}
