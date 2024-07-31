/*==============================================================================
    FILE NAME   : EMS_Store.c
    MODULE NAME : AppStore


    GENERAL DESCRIPTION:
        

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-10-16   Wenkai Xu        01         Create

==============================================================================*/

//pclit
/*lint -e64 */

/*==============================================================================
 * 
 *    Include Files
 * 
 *============================================================================*/
#include "EMS_Store_Priv.h"
#include "eme_utils_api.h"
#include "brweng_tf2hz_email.h"
/*==============================================================================
 * 
 *    DEFINITIONS AND CONSTANTS(It is not recommended)
 *    It is recommended that this section is defined to private head file.
 * 
 *============================================================================*/



/*==============================================================================
 * 
 *    Functions
 * 
 *============================================================================*/
/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HEMSSTORE_New
 *
 *    COMMRNT:    Create a new HEMSStore handler
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
HEMSStore HEMSSTORE_New(void *pFileMgr, void *pvMemMgr)
{
   WE_INT32 nErr = EME_FAILED;
   EMSStore *me = NULL;
   
   do 
   {
      EME_DataAccess_InitActCashe(pFileMgr);
      EME_DataAccess_InitPathMgr();

      me = (EMSStore *)EME_UTILS_MALLOC(pvMemMgr, sizeof(EMSStore));
      EME_UTILS_BREAKNULL(me, nErr, EME_NOMEMORY, 
         "HEMSSTORE_New, no memory for new handler");
      EME_UTILS_MEMSET(me, 0x00, sizeof(EMSStore));
      
      me->hFileMgr = (FILEMGR_HANDLE)pFileMgr;
      me->pvMemMgr = (WE_VOID *)pvMemMgr;

      nErr = EME_DataAccess_Init(me->hFileMgr);
      EME_UTILS_BREAKIF(nErr, "HEMSSTORE_New, init file system failed");

      nErr = EME_SUCCESS;
   } while(0);
   
   if (EME_SUCCESS != nErr)
   {
      EME_DataAccess_DestroyActCashe(pFileMgr);
      EME_DataAccess_DestroyPathMgr();
      EME_UTILS_FREEIF(pvMemMgr, me);
      return NULL;
   }
   
   return (HEMSStore)me;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HEMSSTORE_Delete
 *
 *    COMMRNT:    Delete the handler create by HEMSSTORE_New
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
void HEMSSTORE_Delete(HEMSStore hStore)
{
   EMSStore *me = (EMSStore *)hStore;
   if (NULL == me)
   {
      return;
   }

   switch(me->asynType)
   {
   case EMS_ASYN_TYPE_SEARCH_MSG:
      EME_UTILS_FREEIF(NULL, me->u.msgSearch.pcFilter);
      EME_UTILS_FREEIF(NULL, me->u.msgSearch.pstMsgInfo);
      break;
   case EMS_ASYN_TYPE_MULTI_ACT_DEL:
      EME_UTILS_FREEIF(NULL, me->u.multiActDel.puiActList);
      break;
   case EMS_ASYN_TYPE_MULTI_MSG_MOVE:
      EME_UTILS_FREEIF(NULL, me->u.multiMsgMove.puiMsgList);
      break;
   case EMS_ASYN_TYPE_MULTI_MSG_DEL:
      EME_UTILS_FREEIF(NULL, me->u.multiMsgDel.puiMsgList);
      break;
   case EMS_ASYN_TYPE_STATE_UPDATE:
      EME_UTILS_FREEIF(NULL, me->u.stateUpdate.pstMsgState);
      break;
   case EMS_ASYN_TYPE_FACTORY_RESET:
      break;
   case EMS_ASYN_TYPE_RECOVER_MSG:
      EME_UTILS_FREEIF(NULL, me->u.recoverMsg.puiMsgList);
      break;
   default:
      break;
   }

   EME_DataAccess_DestroyActCashe(me->hFileMgr);
   EME_DataAccess_DestroyPathMgr();

   EME_UTILS_FREEIF(me->pvMemMgr, me);
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HEMSSTORE_StoreMsg
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
int HEMSSTORE_StoreMsg(HEMSStore hStore, HEMSMsg hMsg)
{
   int nErr = EME_FAILED;
   EMSStore *me = (EMSStore *)hStore;
   MsgOpt stMsgOpt = {0};
   EMS_MSGID dwMsgId = 0;
   
   if (NULL == me || NULL == hMsg)
   {
      return EME_BADPARM;
   }
   
   if (!EME_DataAccess_IsEnoughSpace(me->hFileMgr, EMS_LEAST_EML_SPACE))
   {
      EME_UTILS_LOG_ERROR(("HEMSSTORE_StoreMsg, no storage for new message"));
      return EME_NOSTORAGE;
   }
   
   do 
   {
      /* Save message to store */
      nErr =  EME_MsgStore_SaveMsg(me->pvMemMgr, me->hFileMgr, hMsg, TRUE, FALSE);
      if (EME_SUCCESS != nErr && 
          EME_SUCCESS == HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_MSGID, 0, &stMsgOpt))
      {
         dwMsgId = (EMS_MSGID)stMsgOpt.pVal;
         EME_UTILS_BREAK(nErr, "HEMSSTORE_StoreMsg, save message failed");
      }
      
      nErr = HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_MSGID, 0, &stMsgOpt);
      EME_UTILS_BREAKIF(nErr, "HEMSSTORE_StoreMsg, get saved message id failed");
      dwMsgId = (EMS_MSGID)stMsgOpt.pVal;
      
      /* Index the message */
      nErr = EME_MsgIndex_IndexMsg(me->pvMemMgr, me->hFileMgr, hMsg, TRUE);
      EME_UTILS_BREAKIF(nErr, "HEMSSTORE_StoreMsg, index message failed");
      
      nErr = EME_SUCCESS;
   } while(0);
   
   if (EME_SUCCESS != nErr)
   {
      if (0 != dwMsgId)
      {
         (void) HEMSSTORE_RemoveMsg(hStore, dwMsgId);
      }
   }
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HEMSSTORE_GetMsg
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
int HEMSSTORE_GetMsg(HEMSStore hStore, EMS_MSGID uiMsgId, HEMSMsg *phMsg)
{
   int nErr = EME_FAILED;
   EMSStore *me = (EMSStore *)hStore;
   
   if (NULL == me || NULL == phMsg)
   {
      return EME_BADPARM;
   }
   
   *phMsg = NULL;
   
   do 
   {
      *phMsg = HEMSMSG_New(me->pvMemMgr);
      EME_UTILS_BREAKNULL(*phMsg, nErr, EME_FAILED, "HEMSSTORE_GetMsg, new HEMSMsg failed");
      
      nErr = EME_MsgStore_GetMsg(me->pvMemMgr, me->hFileMgr, uiMsgId, *phMsg);
      EME_UTILS_BREAKIF(nErr, "HEMSSTORE_GetMsg, get message from store failed");
      
      nErr = EME_MsgIndex_GetMsgIndexInfo(me->hFileMgr, uiMsgId, *phMsg);
      EME_UTILS_BREAKIF(nErr, "HEMSSTORE_GetMsg, get message index info failed");
      
      nErr = EME_SUCCESS;
   } while(0);
   
   if (SUCCESS != nErr)
   {
      if (NULL != *phMsg)
      {
         HEMSMSG_Delete(*phMsg);
         *phMsg = NULL;
      }
   }
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HEMSSTORE_UpdateMsg
 *
 *    COMMRNT:    Update the message 
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
int HEMSSTORE_UpdateMsg(HEMSStore hStore, HEMSMsg hMsg)
{
   int nErr = EME_FAILED;
   EMSStore *me = (EMSStore *)hStore;
   MsgOpt stMsgOpt = {0};
   EMS_MSGID dwMsgId = 0;
   
   if (NULL == me || NULL == hMsg)
   {
      return EME_BADPARM;
   }
   
//   if (!EME_DataAccess_IsEnoughSpace(me->hFileMgr, EMS_LEAST_EML_SPACE))
//   {
//      EME_UTILS_LOG_ERROR(("HEMSSTORE_StoreMsg, no storage for new message"));
//      return EME_NOSTORAGE;
//   }
   
   do 
   {
      /* Save message to store */
      nErr =  EME_MsgStore_SaveMsg(me->pvMemMgr, me->hFileMgr, hMsg, FALSE, TRUE);
      EME_UTILS_BREAKIF(nErr, "HEMSSTORE_UpdateMsg, save message failed");
      
      nErr = HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_MSGID, 0, &stMsgOpt);
      EME_UTILS_BREAKIF(nErr, "HEMSSTORE_UpdateMsg, get saved message id failed");
      dwMsgId = (EMS_MSGID)stMsgOpt.pVal;
      
      /* Index the message */
      nErr = EME_MsgIndex_IndexMsg(me->pvMemMgr, me->hFileMgr, hMsg, FALSE);
      EME_UTILS_BREAKIF(nErr, "HEMSSTORE_UpdateMsg, index message failed");
      
      nErr = EME_SUCCESS;
   } while(0);
     
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HEMSSTORE_RemoveMsg
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
int HEMSSTORE_RemoveMsg(HEMSStore hStore, EMS_MSGID uiMsgId)
{
   int nErr = EME_FAILED;
   EMSStore *me = (EMSStore *)hStore;
   
   /* Must remove index first ignore the errors */
   nErr = EME_MsgIndex_RemoveMsg(me->pvMemMgr, me->hFileMgr, uiMsgId);

   nErr = EME_MsgStore_RemoveMsg(me->hFileMgr, uiMsgId);
   
   return nErr;
}

static void HEMSSTORE_RemoveMultiMsgCB(void *pvData)
{
   EMSStore *me = (EMSStore*)pvData;

//   EME_MsgIndex_RemoveMsgEx(me->pvMemMgr, me->hFileMgr, me->u.multiMsgDel.puiMsgList, me->u.multiMsgDel.uiMsgCount);

   if(me->u.multiMsgDel.uiMsgIndex < me->u.multiMsgDel.uiMsgCount)
   {
      EME_UTILS_LOG_INFO(("HEMSSTORE_DeleteMsgCB--: delete msg id = 0x%08x", me->u.multiMsgDel.puiMsgList[me->u.multiMsgDel.uiMsgIndex]));
      EME_MsgIndex_RemoveMsg(me->pvMemMgr, me->hFileMgr, me->u.multiMsgDel.puiMsgList[me->u.multiMsgDel.uiMsgIndex]);
      EME_MsgStore_RemoveMsg(me->hFileMgr, me->u.multiMsgDel.puiMsgList[me->u.multiMsgDel.uiMsgIndex]);
      me->u.multiMsgDel.uiMsgIndex++;

      CALLBACK_Init(&me->u.multiMsgDel.delmsgCallback, HEMSSTORE_RemoveMultiMsgCB, me);
      ISHELL_EmailResume(&me->u.multiMsgDel.delmsgCallback);
   }
   else if(me->u.multiMsgDel.uiMsgCount > 0)
   {
      StEMSDelAllNotify *pstDelNotify = EME_UTILS_MALLOC(NULL, sizeof(StEMSDelAllNotify));
      
      if(NULL == pstDelNotify)
      {
         EME_UTILS_LOG_ERROR(("no memory for deletemsg cb"));
         return;
      }
      
      EME_UTILS_MEMSET(pstDelNotify, 0x00, sizeof(StEMSDelAllNotify));
      pstDelNotify->uiCurr = me->u.multiMsgDel.uiMsgIndex;
      pstDelNotify->uiTotal = me->u.multiMsgDel.uiMsgCount;
      pstDelNotify->iRet = EME_SUCCESS;
      
      EME_UTILS_FREEIF(NULL, me->u.multiMsgDel.puiMsgList);
      me->u.multiMsgDel.uiMsgIndex = me->u.multiMsgDel.uiMsgCount = 0;
      
      TS_HZ_EMAIL_EVENT_CALLBACK(BRWENG_EMAIL_MMI_APP, EVT_EMS_DELETEALL_RESULT, (void*)pstDelNotify, NULL);
   }
}

int HEMSSTORE_RemoveMultiMsg(HEMSStore hStore, EMS_MSGID *puiMsglist, unsigned int uiNum)
{
   EMSStore *me = (EMSStore*)hStore;
   if(NULL == hStore || NULL == puiMsglist || 0 == uiNum)
   {
      return EME_BADPARM;
   }
   
   if(CALLBACK_IsQueued(&me->u.multiMsgDel.delmsgCallback))
   {
      return EME_FAILED;
   }
   
   EME_UTILS_FREEIF(NULL, me->u.multiMsgDel.puiMsgList);
   me->u.multiMsgDel.puiMsgList = EME_UTILS_MALLOC(NULL, uiNum * sizeof(EMS_MSGID));
   if(NULL == me->u.multiMsgDel.puiMsgList)
   {
      return EME_NOMEMORY;
   }
   EME_UTILS_MEMSET(me->u.multiMsgDel.puiMsgList, 0x00, uiNum * sizeof(EMS_MSGID));
   EME_UTILS_MEMCPY(me->u.multiMsgDel.puiMsgList, puiMsglist, uiNum * sizeof(EMS_MSGID));
   me->u.multiMsgDel.uiMsgCount = uiNum;
   me->u.multiMsgDel.uiMsgIndex = 0;
   
   me->asynType = EMS_ASYN_TYPE_MULTI_MSG_DEL;
   
   CALLBACK_Init(&me->u.multiMsgDel.delmsgCallback, HEMSSTORE_RemoveMultiMsgCB, me);
   ISHELL_EmailResume(&me->u.multiMsgDel.delmsgCallback);
   
   return EME_SUCCESS;
}

void HEMSSTORE_CancelRemoveMultiMsg(HEMSStore hStore)
{
   EMSStore *me = (EMSStore*)hStore;
   StEMSDelAllNotify *pstDelNotify = EME_UTILS_MALLOC(NULL, sizeof(StEMSDelAllNotify));

   if(me == NULL)
   {
      return;
   }
   
   CALLBACK_Cancel(&me->u.multiMsgDel.delmsgCallback);

   if(NULL == pstDelNotify)
   {
      EME_UTILS_LOG_ERROR(("no memory for deletemsg cb"));
      return;
   }
   
   EME_UTILS_MEMSET(pstDelNotify, 0x00, sizeof(StEMSDelAllNotify));
   pstDelNotify->uiCurr = me->u.multiMsgDel.uiMsgIndex;
   pstDelNotify->uiTotal = me->u.multiMsgDel.uiMsgCount;
   pstDelNotify->iRet = EME_SUCCESS;
   
   EME_UTILS_FREEIF(NULL, me->u.multiMsgDel.puiMsgList);
   me->u.multiMsgDel.uiMsgCount = 0;
   
   TS_HZ_EMAIL_EVENT_CALLBACK(BRWENG_EMAIL_MMI_APP, EVT_EMS_DELETEALL_RESULT, (void*)pstDelNotify, NULL);
}

/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HEMSSTORE_CreateAccount
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
int HEMSSTORE_CreateAccount(HEMSStore        hStore, 
                            unsigned short   *pwszActName, 
                            EMS_ACTID        *puiActId)
{
   EMSStore *me = (EMSStore *)hStore;
   if (NULL == me || NULL == pwszActName || NULL == puiActId)
   {
      return EME_BADPARM;
   }
   
   return EME_ActIndex_CreateAccount(me->hFileMgr, pwszActName, (WE_UINT32*)puiActId);
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HEMSSTORE_GetActInfo
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
int HEMSSTORE_GetActInfo(HEMSStore        hStore, 
                         unsigned int     uiStartPos, 
                         unsigned int     *puiNum, 
                         EMS_ActInfo      *aActInfo)
{
   int nErr = EME_FAILED;
   EMSStore *me = (EMSStore *)hStore;
   
   if (NULL == me || NULL == puiNum)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      if (NULL == aActInfo)
      {
         nErr = EME_ActIndex_GetAccountNum(me->hFileMgr, (WE_UINT32 *)puiNum);
      } 
      else
      {
         nErr = EME_ActIndex_GetActInfo(me->hFileMgr, *puiNum, aActInfo);
      }
      EME_UTILS_BREAKIF(nErr, "HEMSSTORE_GetActInfo, read account info failed");
      
      nErr = EME_SUCCESS;
   } while(0);
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HEMSSTORE_DeleteAccount
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
int HEMSSTORE_DeleteAccount(HEMSStore hStore, EMS_ACTID uiActId)
{
   EMSStore *me = (EMSStore *)hStore;
   
   if (NULL == me)
   {
      return EME_BADPARM;
   }
   
   return EME_ActIndex_DeleteAccount(me->hFileMgr, uiActId);
}

static void HEMSSTORE_DeleteMultiAccountCB(HEMSStore hStore)
{
   EMSStore *me = (EMSStore *)hStore;
   
   if(NULL == me)
      return;
   
   while(me->u.multiActDel.uiActIndex < me->u.multiActDel.uiActCount)
   {
      EME_ActIndex_DeleteAccount(me->hFileMgr, me->u.multiActDel.puiActList[me->u.multiActDel.uiActIndex]);
      me->u.multiActDel.uiActIndex++;
   }
   
   EME_UTILS_FREEIF(me->pvMemMgr, me->u.multiActDel.puiActList);

   TS_HZ_EMAIL_EVENT_CALLBACK(BRWENG_EMAIL_MMI_APP, EVT_EMS_DELETE_ACCOUNT_RESULT, NULL, NULL);
}

int HEMSSTORE_DeleteMultiAccount(HEMSStore hStore, EMS_ACTID *puiActId, WE_UINT32 uiNum)
{
   int      iRet = EME_SUCCESS;
   EMSStore *me = (EMSStore *)hStore;
   
   if (NULL == me || NULL == puiActId || 0 == uiNum)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      EME_UTILS_FREEIF(me->pvMemMgr, me->u.multiActDel.puiActList);
      me->u.multiActDel.puiActList = EME_UTILS_MALLOC(me->pvMemMgr, sizeof(EMS_ACTID) * uiNum);
      EME_UTILS_BREAKNULL(me->u.multiActDel.puiActList, iRet, EME_NOMEMORY, "no memory!!");
      EME_UTILS_MEMSET(me->u.multiActDel.puiActList, 0x00, sizeof(EMS_ACTID) * uiNum);
      
      EME_UTILS_MEMCPY(me->u.multiActDel.puiActList, puiActId, sizeof(EMS_ACTID) * uiNum);
      me->u.multiActDel.uiActCount = uiNum;
      me->u.multiActDel.uiActIndex = 0;
      
      me->asynType = EMS_ASYN_TYPE_MULTI_ACT_DEL;

      CALLBACK_Init(&me->u.multiActDel.delactCallback, HEMSSTORE_DeleteMultiAccountCB, me);
      ISHELL_EmailResume(&me->u.multiActDel.delactCallback);

   } while (0);
   
   return EME_SUCCESS;
}

/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HEMSSTORE_SetActCfgItem
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
int HEMSSTORE_SetActCfgItem(HEMSStore        hStore, 
                            EMS_ACTID        uiActId,
                            EMS_PRFITEM_ID   uiItem,
                            EMS_PRFITEM_VAL  *pItemVal)
{
   EMSStore *me = (EMSStore *)hStore;
   int      nErr = EME_SUCCESS;
   WE_UINT16 *pwcItemValue = NULL;

   if (NULL == me || NULL == pItemVal)
   {
      return EME_BADPARM;
   }
   
   if(0 == uiActId)
   {
      nErr = EME_ActIndex_SetActGlobalCfgItem(me->hFileMgr, uiItem, pItemVal->pData);
   }
   else
   {
      nErr = EME_ActIndex_SetActCfgItem(me->hFileMgr, uiActId, uiItem, pItemVal->pData);
   }

   return nErr;
}

int HEMSSTORE_SetActCfgItemEx(HEMSStore        hStore, 
                            EMS_ACTID        uiActId,
                            EMSMsgOpt       *pstOpt)
{
   EMSStore *me = (EMSStore *)hStore;
   int      nErr = EME_SUCCESS;
   WE_UINT16 *pwcItemValue = NULL;

   if (NULL == me || NULL == pstOpt)
   {
      return EME_BADPARM;
   }
   
   if(0 == uiActId)
   {
      nErr = EME_ActIndex_SetActGlobalCfgItemEx(me->hFileMgr, (WEMsgOpt*)pstOpt);
   }
   else
   {
      nErr = EME_ActIndex_SetActCfgItemEx(me->hFileMgr, uiActId, (WEMsgOpt*)pstOpt);
   }

   return nErr;
}
/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HEMSSTORE_GetActCfgItem
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
int HEMSSTORE_GetActCfgItem(HEMSStore        hStore, 
                            EMS_ACTID        uiActId, 
                            EMS_PRFITEM_ID   uiItem, 
                            EMS_PRFITEM_VAL  *pItemVal)
{
   int nErr = EME_SUCCESS;
   EMSStore *me = (EMSStore *)hStore;
   WE_VOID  *pItemValue = NULL;
   
   if (NULL == me || NULL == pItemVal)
   {
      return EME_BADPARM;
   }

   do 
   {
      if(0 == uiActId)
      {
         nErr = EME_ActIndex_GetActGlobalCfgItem(me->hFileMgr, uiItem, &pItemValue);
      }
      else
      {
         nErr = EME_ActIndex_GetActCfgItem(me->hFileMgr, uiActId, uiItem, &pItemValue);
      }
      EME_UTILS_BREAKIF(nErr, "Get Act Cfg Item failed!!!");

      if(uiItem >= WEMSG_OPT_32BIT && uiItem <= WEMSG_OPT_32BIT_LAST)
      {
         pItemVal->pData = pItemValue;
         pItemVal->size  = sizeof(WE_UINT32);
         break;
      }

      switch(uiItem)
      {
      case EMS_PRFITEM_ACCOUNT_NAME :
         EME_UTILS_MEMCPY(me->stActCfg.wszActName, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
         pItemVal->pData = me->stActCfg.wszActName;
         pItemVal->size = EME_UTILS_WSTRLEN((WE_UINT16*)pItemVal->pData) * sizeof(WE_UINT16);
         break;
      case EMS_PRFITEM_USER_NAME:
         EME_UTILS_MEMCPY(me->stActCfg.stUsrInfo.wszName, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
         pItemVal->pData = me->stActCfg.stUsrInfo.wszName;
         pItemVal->size = EME_UTILS_WSTRLEN((WE_UINT16*)pItemVal->pData) * sizeof(WE_UINT16);
         break;
      case EMS_PRFITEM_ACCOUNT_EMAIL :
         EME_UTILS_MEMCPY(me->stActCfg.stUsrInfo.wszEmail, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
         pItemVal->pData = (void *)me->stActCfg.stUsrInfo.wszEmail;
         pItemVal->size = EME_UTILS_WSTRLEN((WE_UINT16*)pItemVal->pData) * sizeof(WE_UINT16);
         break;
      case EMS_PRFITEM_REPLY_TO_EMAIL :
         EME_UTILS_MEMCPY(me->stActCfg.stUsrInfo.wszReply2, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
         pItemVal->pData = (void *)me->stActCfg.stUsrInfo.wszReply2;
         pItemVal->size = EME_UTILS_WSTRLEN((WE_UINT16*)pItemVal->pData) * sizeof(WE_UINT16);
         break;
      case EMS_PRFITEM_EMAIL_USER_ID :
         EME_UTILS_MEMCPY(me->stActCfg.stUsrInfo.wszUserId, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
         pItemVal->pData = (void *)me->stActCfg.stUsrInfo.wszUserId;
         pItemVal->size = EME_UTILS_WSTRLEN((WE_UINT16*)pItemVal->pData) * sizeof(WE_UINT16);
         break;
      case EMS_PRFITEM_EMAIL_PASSWORD :
         EME_UTILS_MEMCPY(me->stActCfg.stUsrInfo.wszPwd, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
         pItemVal->pData = (void *)me->stActCfg.stUsrInfo.wszPwd;
         pItemVal->size = EME_UTILS_WSTRLEN((WE_UINT16*)pItemVal->pData) * sizeof(WE_UINT16);
         break;
      case EMS_PRFITEM_SMTP_USER_ID :
         EME_UTILS_MEMCPY(me->stActCfg.stUsrInfo.wszSmtpid, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
         pItemVal->pData = (void *)me->stActCfg.stUsrInfo.wszSmtpid;
         pItemVal->size = EME_UTILS_WSTRLEN((WE_UINT16*)pItemVal->pData) * sizeof(WE_UINT16);
         break;
      case EMS_PRFITEM_SMTP_PASSWORD :
         EME_UTILS_MEMCPY(me->stActCfg.stUsrInfo.wszSmtppwd, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
         pItemVal->pData = (void *)me->stActCfg.stUsrInfo.wszSmtppwd;
         pItemVal->size = EME_UTILS_WSTRLEN((WE_UINT16*)pItemVal->pData) * sizeof(WE_UINT16);
         break;
      case EMS_PRFITEM_APN_TYPE:
         pItemVal->pData = (void *)me->stActCfg.stUsrInfo.acRes[0];
         break;
      case EMS_PRFITEM_POP3_SERVER :
         EME_UTILS_MEMCPY(me->stActCfg.stConnectInfo.wszPop3Server, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
         pItemVal->pData = (void *)me->stActCfg.stConnectInfo.wszPop3Server;
         pItemVal->size = EME_UTILS_WSTRLEN((WE_UINT16*)pItemVal->pData) * sizeof(WE_UINT16);
         break;
      case EMS_PRFITEM_POP3_PORT :
         EME_UTILS_MEMCPY(me->stActCfg.stConnectInfo.wszPop3Port, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
         pItemVal->pData = (void *)me->stActCfg.stConnectInfo.wszPop3Port;
         pItemVal->size = EME_UTILS_WSTRLEN((WE_UINT16*)pItemVal->pData) * sizeof(WE_UINT16);
         break;
      case EMS_PRFITEM_IMAP4_SERVER :
         EME_UTILS_MEMCPY(me->stActCfg.stConnectInfo.wszImap4Server, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
         pItemVal->pData = (void *)me->stActCfg.stConnectInfo.wszImap4Server;
         pItemVal->size = EME_UTILS_WSTRLEN((WE_UINT16*)pItemVal->pData) * sizeof(WE_UINT16);
         break;
      case EMS_PRFITEM_IMAP4_SERVER_PORT :
         EME_UTILS_MEMCPY(me->stActCfg.stConnectInfo.wszImap4Port, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
         pItemVal->pData = (void *)me->stActCfg.stConnectInfo.wszImap4Port;
         pItemVal->size = EME_UTILS_WSTRLEN((WE_UINT16*)pItemVal->pData) * sizeof(WE_UINT16);
         break;
      case EMS_PRFITEM_SMTP_SERVER :
         EME_UTILS_MEMCPY(me->stActCfg.stConnectInfo.wszSmtpServer, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
         pItemVal->pData = (void *)me->stActCfg.stConnectInfo.wszSmtpServer;
         pItemVal->size = EME_UTILS_WSTRLEN((WE_UINT16*)pItemVal->pData) * sizeof(WE_UINT16);
         break;
      case EMS_PRFITEM_SMTP_SERVER_PORT :
         EME_UTILS_MEMCPY(me->stActCfg.stConnectInfo.wszSmtpPort, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
         pItemVal->pData = (void *)me->stActCfg.stConnectInfo.wszSmtpPort;
         pItemVal->size = EME_UTILS_WSTRLEN((WE_UINT16*)pItemVal->pData) * sizeof(WE_UINT16);
         break;
      case EMS_PRFITEM_MAX_MSG_SIZE :
         EME_UTILS_MEMCPY(me->stActCfg.stMessageInfo.wszMsgsize, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
         pItemVal->pData = (void *)me->stActCfg.stMessageInfo.wszMsgsize;
         pItemVal->size = EME_UTILS_WSTRLEN((WE_UINT16*)pItemVal->pData) * sizeof(WE_UINT16);
         break;
//       case EMS_PRFITEM_NUMBER_LIMIT :
//          EME_UTILS_MEMCPY(me->stActCfg.stMessageInfo.wszNumLimit, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
//          pItemVal->pData = (void *)me->stActCfg.stMessageInfo.wszNumLimit;
//          pItemVal->size = EME_UTILS_WSTRLEN((WE_UINT16*)pItemVal->pData) * sizeof(WE_UINT16);
//          break;
      case EMS_PRFITEM_EMAIL_CREATE_SIGNATURE :
         EME_UTILS_STRCPY(me->stActCfg.aszCreateSig, pItemValue);
         pItemVal->pData = (void *)me->stActCfg.aszCreateSig;
         pItemVal->size = EME_UTILS_STRLEN((WE_UCHAR*)pItemVal->pData);
         break;
      case EMS_PRFITEM_EMAIL_UIDLFILEDIR :
         EME_UTILS_STRCPY(me->stActCfg.pcUidlFileDir, pItemValue);
         pItemVal->pData = me->stActCfg.pcUidlFileDir;
         pItemVal->size = EME_UTILS_STRLEN((WE_UCHAR*)pItemVal->pData);
         break;
      case EMS_PRFITEM_EMAIL_UIDFILEDIR:
         EME_UTILS_STRCPY(me->stActCfg.pcUidFileDir, pItemValue);
         pItemVal->pData = me->stActCfg.pcUidFileDir;
         pItemVal->size = EME_UTILS_STRLEN((WE_UCHAR*)pItemVal->pData);
         break;
      case EMS_PRFITEM_SENDER_NAME:
         EME_UTILS_MEMCPY(me->stActCfg.stMessageInfo.wszSenderName, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
         pItemVal->pData = me->stActCfg.stMessageInfo.wszSenderName;
         pItemVal->size = EME_UTILS_WSTRLEN((WE_UCHAR*)pItemVal->pData) * sizeof(WE_UINT16);
         break;
      case EMS_PRFITEM_SET_PASSWORD_PROTECTION:
         EME_UTILS_MEMCPY(me->stGloabalSetting.wszProtectPassward, pItemValue, (EME_UTILS_WSTRLEN(pItemValue) + 1) * sizeof(WE_UINT16));
         pItemVal->pData = me->stGloabalSetting.wszProtectPassward;
         pItemVal->size = EME_UTILS_WSTRLEN((WE_UCHAR*)pItemVal->pData) * sizeof(WE_UINT16);
         break;
      default:
         EME_UTILS_LOG_INFO(("HEMSSTORE_GetActCfgItem--: bad item 0x%x", uiItem));
         nErr = EME_BADPARM;
         break;
      }

      if(uiItem > WEMSG_OPT_SZ && uiItem < WEMSG_OPT_BINARY_LAST)
      {
         EME_UTILS_FREEIF(NULL, pItemValue);
      }

   } while(0);

   return nErr;

}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HEMSSTORE_CreateBox
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
int HEMSSTORE_CreateBox(HEMSStore      hStore, 
                        unsigned short *pwszBoxName, 
                        EMS_BOXID      *puiBoxId, 
                        EMS_ACTID      uiActId, 
                        unsigned char  ucState)
{
   EMSStore *me = (EMSStore *)hStore;
   
   return EME_BoxIndex_CreateBox(me->hFileMgr, uiActId, pwszBoxName, ucState, (WE_UINT32*)puiBoxId);
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HEMSSTORE_DeleteBox
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
int HEMSSTORE_DeleteBox(HEMSStore      hStore, 
                        EMS_BOXID      uiBoxId, 
                        EMS_ACTID      uiActId)
{
   EMSStore *me = (EMSStore *)hStore;

   if (NULL == me)
   {
      return EME_BADPARM;
   }
   
   return EME_BoxIndex_DeleteBox(me->hFileMgr, uiActId, uiBoxId);
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HEMSSTORE_GetBoxInfo
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
int HEMSSTORE_GetBoxInfo(HEMSStore     hStore, 
                         EMS_ACTID     uiActId, 
                         unsigned int  uiStartPos, 
                         unsigned int  *puiNum, 
                         EMS_BoxInfo   *aBoxInfo)
{
   int nErr = EME_FAILED;
   EMSStore *me = (EMSStore *)hStore;

   if (NULL == me || NULL == puiNum)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      if (NULL == aBoxInfo)
      {
         nErr = EME_BoxIndex_GetBoxNum(me->hFileMgr, uiActId, (WE_UINT32 *)puiNum);
      } 
      else
      {
         nErr = EME_BoxIndex_GetBoxInfo(me->hFileMgr, uiActId,*puiNum, aBoxInfo);
      }
      EME_UTILS_BREAKIF(nErr, "HEMSSTORE_GetBoxInfo, read box info failed");
      
      nErr = EME_SUCCESS;
   } while(0);
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HEMSSTORE_MoveMsg
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
int HEMSSTORE_MoveMsg(HEMSStore  hStore, 
                      EMS_ACTID  uiActId, 
                      EMS_BOXID  uiSrcBoxId, 
                      EMS_BOXID  uiDestBoxId, 
                      EMS_MSGID  uiMsgId)
{
   EMSStore *me = (EMSStore *)hStore;
   if (NULL == me)
   {
      return EME_BADPARM;
   }
   
   return EME_MsgIndex_MoveMsg(me->pvMemMgr, me->hFileMgr, uiActId, uiSrcBoxId, uiDestBoxId, uiMsgId);
}

void HEMSSTORE_MoveMultiMsgCB(WE_VOID *pvData)
{
   EMSStore *me = (EMSStore *)pvData;
   
   EME_MsgIndex_MoveMsgEx(me->pvMemMgr, me->hFileMgr, 
      me->u.multiMsgMove.uiActId,
      me->u.multiMsgMove.uiSrcBoxId,
      me->u.multiMsgMove.uiDesBoxId,
      me->u.multiMsgMove.puiMsgList,
      me->u.multiMsgMove.uiMsgCount);

//    while(me->u.multiMsgMove.uiMsgIndex < me->u.multiMsgMove.uiMsgCount)
//    {
//       HEMSSTORE_MoveMsg(me, 
//          me->u.multiMsgMove.uiActId,
//          me->u.multiMsgMove.uiSrcBoxId,
//          me->u.multiMsgMove.uiDesBoxId,
//          me->u.multiMsgMove.puiMsgList[me->u.multiMsgMove.uiMsgIndex]);
//       
//       me->u.multiMsgMove.uiMsgIndex++;
//    }
   
   EME_UTILS_FREEIF(NULL, me->u.multiMsgMove.puiMsgList);
   
   TS_HZ_EMAIL_EVENT_CALLBACK(BRWENG_EMAIL_MMI_APP, EVT_EMS_MOVE_RESULT, NULL, NULL);
   
   return;
}

int HEMSSTORE_MoveMultiMsg(HEMSStore     hStore, 
                           EMS_ACTID     uiActId,
                           EMS_BOXID     uiSrcBoxId,
                           EMS_BOXID     uiDesBoxId,
                           EMS_MSGID     *puiMsglist,
                           unsigned int  uiNum)
{
   EMSStore *me = (EMSStore *)hStore;
   
   if(NULL == puiMsglist || 0 == uiNum)
   {
      EME_UTILS_LOG_ERROR(("bad parameter!!"));
      return EME_BADPARM;
   }
   
   if(CALLBACK_IsQueued(&me->u.multiMsgMove.movemsgCallback))
   {
      EME_UTILS_LOG_ERROR(("callback is queued!!"));
      return EME_FAILED;
   }
   
   EME_UTILS_FREEIF(NULL, me->u.multiMsgMove.puiMsgList);
   me->u.multiMsgMove.puiMsgList = EME_UTILS_MALLOC(NULL, uiNum * sizeof(EMS_MSGID));
   if(NULL == me->u.multiMsgMove.puiMsgList)
   {
      EME_UTILS_LOG_ERROR(("no memory!!!"));
      return EME_NOMEMORY;
   }
   
   EME_UTILS_MEMCPY(me->u.multiMsgMove.puiMsgList, puiMsglist, uiNum * sizeof(EMS_MSGID));
   me->u.multiMsgMove.uiMsgCount = uiNum;
   me->u.multiMsgMove.uiActId    = uiActId;
   me->u.multiMsgMove.uiSrcBoxId = uiSrcBoxId;
   me->u.multiMsgMove.uiDesBoxId = uiDesBoxId;
   me->u.multiMsgMove.uiMsgIndex = 0;
   
   me->asynType = EMS_ASYN_TYPE_MULTI_MSG_MOVE;
   CALLBACK_Init(&me->u.multiMsgMove.movemsgCallback, HEMSSTORE_MoveMultiMsgCB, hStore);
   ISHELL_EmailResume(&me->u.multiMsgMove.movemsgCallback);
   
   return EME_SUCCESS;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HEMSSTORE_GetMsgInfo
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
int HEMSSTORE_GetMsgInfo(HEMSStore     hStore, 
                         EMS_ACTID     uiActId, 
                         EMS_BOXID     uiBoxId, 
                         unsigned int  uiStartPos, 
                         unsigned int  *puiNum, 
                         EMS_MsgInfo   *aMsgInfo)
{
   int nErr = EME_FAILED;
   EMSStore *me = (EMSStore *)hStore;

   if (NULL == me || NULL == puiNum)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      if (NULL == aMsgInfo)
      {
         nErr = EME_MsgIndex_GetMsgInfoNum(me->hFileMgr, uiActId, uiBoxId, (WE_UINT32 *)puiNum);
      } 
      else
      {
         nErr = EME_MsgIndex_GetMsgInfo(me->hFileMgr, uiActId, uiBoxId, *puiNum, aMsgInfo);
      }
      EME_UTILS_BREAKIF(nErr, "HEMSSTORE_GetMsgInfo, read message info failed");
      
      nErr = EME_SUCCESS;
   } while(0);
   
   return nErr;
}

int HEMSSTORE_GetTotalNum(HEMSStore     hStore, 
                          EMS_ACTID     uiActId,
                          unsigned int  *puiTotalNum)
{
   int nErr = EME_FAILED;
   EMSStore *me = (EMSStore *)hStore;
   
   if (NULL == me || NULL == puiTotalNum)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      nErr = EME_MsgIndex_GetMsgInfoNum(me->hFileMgr, uiActId, EMS_BOX_ID_ALL, (WE_UINT32 *)puiTotalNum);
      EME_UTILS_BREAKIF(nErr, "HEMSSTORE_GetTotalNum, read message info failed");
      
   } while(0);
   
   return nErr;
}

static void HEMSSTORE_SearchMsgInfoCB(WE_VOID *pvData)
{
   EMSStore *me = (EMSStore*)pvData;
   int      nErr = EME_SUCCESS;
   
   nErr = EME_MsgIndex_SearchMsgInfo(me->hFileMgr, 
      me->u.msgSearch.uiActId, 
      me->u.msgSearch.uiBoxId, 
      me->u.msgSearch.pcFilter, 
      NULL, 
      &me->u.msgSearch.uiMsgNum);
   if(nErr != EME_SUCCESS)
   {
      EME_UTILS_LOG_ERROR(("get msg info number failed!!"));
      return;
   }
   
   me->u.msgSearch.pstMsgInfo = EME_UTILS_MALLOC(NULL, me->u.msgSearch.uiMsgNum * sizeof(EMS_MsgInfo));
   if(NULL == me->u.msgSearch.pstMsgInfo)
   {
      EME_UTILS_LOG_ERROR(("no memory!!"));
      return;
   } 
   EME_UTILS_MEMSET(me->u.msgSearch.pstMsgInfo, 0x00, me->u.msgSearch.uiMsgNum * sizeof(EMS_MsgInfo));
   
   nErr = EME_MsgIndex_SearchMsgInfo(me->hFileMgr, 
      me->u.msgSearch.uiActId, 
      me->u.msgSearch.uiBoxId, 
      me->u.msgSearch.pcFilter, 
      me->u.msgSearch.pstMsgInfo, 
      &me->u.msgSearch.uiMsgNum);
   
   EME_UTILS_FREEIF(NULL, me->u.msgSearch.pcFilter);
   
   if(nErr != EME_SUCCESS)
   {
      EME_UTILS_FREEIF(NULL, me->u.msgSearch.pstMsgInfo);
      me->u.msgSearch.uiMsgNum = 0;
   }
   
   TS_HZ_EMAIL_EVENT_CALLBACK(BRWENG_EMAIL_MMI_APP, EVT_EMS_SEARCH_RESULT, NULL, NULL);
   
   return;
}

int HEMSSTORE_SearchMsgInfo(HEMSStore hStore, EMS_ACTID uiActId, unsigned short *pwcFilte, EMS_BOXID uiBoxId)
{
   EMSStore *me = (EMSStore*)hStore;
   
   if(NULL == pwcFilte || EME_UTILS_WSTRLEN(pwcFilte) == 0)
   {
      EME_UTILS_LOG_ERROR(("bad parameter!!!"));
      return EME_FAILED;
   }
   
   if(CALLBACK_IsQueued(&me->u.msgSearch.searchCallback))
   {
      EME_UTILS_LOG_ERROR(("search callback is queued!!"));
      return EME_FAILED;
   }
   
   EME_UTILS_FREEIF(NULL, me->u.msgSearch.pcFilter);
   EME_UTILS_FREEIF(NULL, me->u.msgSearch.pstMsgInfo);
   
   me->u.msgSearch.pcFilter = EME_UTILS_WSTRTOUTF8(NULL, pwcFilte);
   if(NULL == me->u.msgSearch.pcFilter)
   {
      EME_UTILS_LOG_ERROR(("wstr to utf8 failed!!"));
      return EME_NOMEMORY;
   }
   me->u.msgSearch.uiActId  = uiActId;
   me->u.msgSearch.uiBoxId  = uiBoxId;
   me->u.msgSearch.uiMsgNum = 0;
   
   me->asynType = EMS_ASYN_TYPE_SEARCH_MSG;
   
   CALLBACK_Init(&me->u.msgSearch.searchCallback, HEMSSTORE_SearchMsgInfoCB, hStore);
   ISHELL_EmailResume(&me->u.msgSearch.searchCallback);
   
   return EME_SUCCESS;
}

EMS_MSGID HEMSSTORE_CheckMsgId(HEMSStore hStore, EMS_ACTID uiActId, EMS_MSGID uiMsgId)
{
   EMSStore *me = (EMSStore*)hStore;

   return EME_MsgStore_DetermMsgId(me->hFileMgr, uiActId, uiMsgId);
}

int HEMSSTORE_GetServerConf(HEMSStore hStore, EMS_MSGID *puiNum, EMS_ServerConf *pstConInfo)
{
   EMSStore *me = (EMSStore*)hStore;

   if(NULL == me)
      return EME_FAILED;

   return EME_SerConf_GetData(me->hFileMgr, puiNum, pstConInfo);
}

unsigned char HEMSSTORE_CheckAccountIsOK(HEMSStore hStore, EMS_ACTID dwActId)
{
   EMSStore *me = (EMSStore*)hStore;
   
   if(NULL == me)
      return FALSE;

   return EME_ActIndex_CheckActountOK(me->hFileMgr, dwActId);
}

int HEMSSTORE_SetActiveActId(HEMSStore hStore, EMS_ACTID dwActId)
{
   EMSStore *me = (EMSStore*)hStore;
   
   if(NULL == me)
   {
      return EME_FAILED;
   }
   
   return EME_ActIndex_SetActiveActId(me->hFileMgr, dwActId);
}

int HEMSSTORE_GetActiveActId(HEMSStore hStore, EMS_ACTID *pdwActId)
{
   EMSStore *me = (EMSStore*)hStore;
   
   if(NULL == me || NULL == pdwActId)
   {
      return EME_FAILED;
   }
   
   return EME_ActIndex_GetActiveActId(me->hFileMgr, pdwActId);
}

static void HEMSSTORE_FactoryResetCB(void *pvData)
{
   EMSStore *me = (EMSStore*)pvData;
   
   EME_ActIndex_FactoryReset(me->hFileMgr);

   TS_HZ_EMAIL_EVENT_CALLBACK(BRWENG_EMAIL_MMI_APP, EVT_EMS_FACTORY_RESET_RESULT, NULL, NULL);
}

void HEMSSTORE_FactoryReset(HEMSStore hStore)
{
   EMSStore *me = (EMSStore*)hStore;

   if(NULL == me)
      return;

   me->asynType = EMS_ASYN_TYPE_FACTORY_RESET;

   CALLBACK_Init(&me->u.factoryReset.factoryCallback, HEMSSTORE_FactoryResetCB, hStore);
   ISHELL_EmailResume(&me->u.factoryReset.factoryCallback);
}

static EMSStore *gs_hStore = NULL;

void HEMSSTORE_SyncMsgIndexInfoCB(void *pvData)
{
   int         nErr = EME_FAILED;
   EMSStore *me = (EMSStore*)pvData;
   EMS_ActInfo *pstActInfo = 0;
   WE_UINT32   uiNum = 0;
   WE_UINT32   uiIndex = 0;

   do 
   {
      nErr = EME_ActIndex_GetAccountNum(me->hFileMgr, (WE_UINT32 *)&uiNum);
      EME_UTILS_BREAKIF(nErr, "get account number failed!!");
      if(0 == uiNum)
      {
         break;
      }
      
      pstActInfo = EME_UTILS_MALLOC(NULL, sizeof(EMS_ActInfo) * uiNum);
      EME_UTILS_BREAKNULL(pstActInfo, nErr, EME_NOMEMORY, "no memory!!");
      
      EME_UTILS_MEMSET(pstActInfo, 0x00, sizeof(EMS_ActInfo) * uiNum);
      
      nErr = EME_ActIndex_GetActInfo(me->hFileMgr, uiNum, pstActInfo);
      EME_UTILS_BREAKIF(nErr, "get act info failed");
      
      while(uiIndex < uiNum)
      {
         EME_MsgIndex_SyncMsgIndexInfo(me->hFileMgr, pstActInfo[uiIndex].uiActId);
         uiIndex++;
      }

   } while (0);

   EME_UTILS_FREEIF(NULL, pstActInfo);
   HEMSSTORE_Delete(gs_hStore);
   gs_hStore = NULL;
   
   return;
}

void HEMSSTORE_SyncMsgIndexInfo(void)
{
   int         nErr = EME_FAILED;

   do 
   {
      if(NULL == gs_hStore)
      {
         gs_hStore = HEMSSTORE_New(NULL, NULL);
         EME_UTILS_BREAKNULL(gs_hStore, nErr, EME_FAILED, "create hstore handle failed!!!");
      }
      else
      {
         EME_UTILS_LOG_ERROR(("gs_hStore is not null!!!"));
         break;
      }

      CALLBACK_Init(&gs_hStore->u.syncMsgInfo.syncmsginfoCallback, HEMSSTORE_SyncMsgIndexInfoCB, gs_hStore);
      ISHELL_EmailResume(&gs_hStore->u.syncMsgInfo.syncmsginfoCallback);
   } while(0);

   return;
}

int HEMSSTORE_UpdateMsgHeader(HEMSStore hStore, HEMSMsg hMsg)
{
   int nErr = EME_FAILED;
   EMSStore *me = (EMSStore *)hStore;
   MsgOpt stMsgOpt = {0};
   EMS_MSGID dwMsgId = 0;
   
   if (NULL == me || NULL == hMsg)
   {
      return EME_BADPARM;
   }
   
   //   if (!EME_DataAccess_IsEnoughSpace(me->hFileMgr, EMS_LEAST_EML_SPACE))
   //   {
   //      EME_UTILS_LOG_ERROR(("HEMSSTORE_StoreMsg, no storage for new message"));
   //      return EME_NOSTORAGE;
   //   }
   
   do 
   {
      nErr = HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_MSGID, 0, &stMsgOpt);
      EME_UTILS_BREAKIF(nErr, "HEMSSTORE_UpdateMsg, get saved message id failed");
      dwMsgId = (EMS_MSGID)stMsgOpt.pVal;
      
      /* Save message to store */
      nErr =  EME_MsgStore_SaveMsgHeader(me->pvMemMgr, me->hFileMgr, hMsg, dwMsgId);
      EME_UTILS_BREAKIF(nErr, "HEMSSTORE_UpdateMsg, save message header failed");
      
      /* Index the message */
      nErr = EME_MsgIndex_IndexMsg(me->pvMemMgr, me->hFileMgr, hMsg, FALSE);
      EME_UTILS_BREAKIF(nErr, "HEMSSTORE_UpdateMsg, index message failed");
      
      nErr = EME_SUCCESS;
   } while(0);
   
   return nErr;
}

static void HESSTORE_UpdateMsgStateCB(void *pvData)
{
   EMSStore *me = (EMSStore *)pvData;
   MsgOpt stMsgOpt[2] = {0};
   EMS_MSGID dwMsgId = 0;
   unsigned int uiIndex = 0;
   HEMSMsg      hMsg = NULL;
   
   while(uiIndex < me->u.stateUpdate.uiMsgStateCount)
   {
      HEMSSTORE_GetMsg(me, me->u.stateUpdate.pstMsgState[uiIndex].msgid, &hMsg);
      if(NULL != hMsg)
      {
         stMsgOpt[0].nId = EMS_MSGOPT_STATUS;
         stMsgOpt[0].pVal = (void*)me->u.stateUpdate.pstMsgState[uiIndex].state;
         stMsgOpt[1].nId = WEMSG_OPT_END;
         
         HMULTIPARTMSG_UpdateOpt(hMsg, 0, stMsgOpt);
         EME_MsgStore_SaveMsgHeader(me->pvMemMgr, me->hFileMgr, hMsg, me->u.stateUpdate.pstMsgState[uiIndex].msgid);
         EME_MsgIndex_IndexMsg(me->pvMemMgr, me->hFileMgr, hMsg, FALSE);
         
         HMULTIPARTMSG_Delete(hMsg);
         hMsg = NULL;
      }
      uiIndex++;
   }
   
   EME_UTILS_FREEIF(NULL, me->u.stateUpdate.pstMsgState);
   me->u.stateUpdate.uiMsgStateCount = 0;
   
   TS_HZ_EMAIL_EVENT_CALLBACK(BRWENG_EMAIL_MMI_APP, EVT_EMS_UPDATE_STATE_RESULT, NULL, NULL);
}

int HEMSSTORE_UpdateMsgState(HEMSStore hStore, StEMSUpdateState *pstMsgState, unsigned int uiNum)
{
   EMSStore *me = (EMSStore *)hStore;
   
   if(NULL == hStore || NULL == pstMsgState || 0 == uiNum)
   {
      return EME_FAILED;
   }
   
   if(CALLBACK_IsQueued(&me->u.stateUpdate.updateCallback))
   {
      return EME_FAILED;
   }
   
   EME_UTILS_FREEIF(NULL, me->u.stateUpdate.pstMsgState);
   me->u.stateUpdate.pstMsgState = EME_UTILS_MALLOC(NULL, uiNum * sizeof(StEMSUpdateState));
   if(NULL == me->u.stateUpdate.pstMsgState)
   {
      return EME_NOMEMORY;
   }
   EME_UTILS_MEMSET(me->u.stateUpdate.pstMsgState, 0x00, uiNum * sizeof(StEMSUpdateState));
   EME_UTILS_MEMCPY(me->u.stateUpdate.pstMsgState, pstMsgState, uiNum * sizeof(StEMSUpdateState));
   me->u.stateUpdate.uiMsgStateCount = uiNum;
   CALLBACK_Init(&me->u.stateUpdate.updateCallback, HESSTORE_UpdateMsgStateCB, me);
   ISHELL_EmailResume(&me->u.stateUpdate.updateCallback);
   
   return EME_SUCCESS;
}

void HEMSSTORE_GetAsynData(HEMSStore hStore, EMS_ASYNTYPE asyn, WE_VOID** ppData, WE_UINT32 *pdwNum)
{
   EMSStore *me = (EMSStore*)hStore;

   switch(asyn)
   {
   case EMS_ASYN_TYPE_SEARCH_MSG:
      if(NULL != ppData &&pdwNum)
      {
         *ppData = me->u.msgSearch.pstMsgInfo;
         *pdwNum = me->u.msgSearch.uiMsgNum;
      }
      break;
   default:
      break;
   }

   return;
}

static void HEMSSTORE_RecoverMsgCB(WE_VOID *pvData)
{
   EMSStore *me = (EMSStore*)pvData;

   if(EME_MsgIndex_RecoverMsg(me->pvMemMgr, me->hFileMgr, 
      me->u.recoverMsg.uiActId, 
      me->u.recoverMsg.puiMsgList, 
      me->u.recoverMsg.uiMsgCount) != EME_SUCCESS)
   {
      EME_UTILS_LOG_ERROR(("recover msg failed!!!"));
   }

   EME_UTILS_FREEIF(NULL, me->u.recoverMsg.puiMsgList);
   me->u.recoverMsg.uiMsgCount = 0;
   me->u.recoverMsg.uiActId = 0;
   
   TS_HZ_EMAIL_EVENT_CALLBACK(BRWENG_EMAIL_MMI_APP, EVT_EMS_RECOVER_MSG_RESULT, NULL, NULL);
   
   return;
}

int HEMSSTORE_RecoverMsg(HEMSStore hStore, EMS_ACTID uiActId, EMS_MSGID *puiMsgList, unsigned int uiMsgNum)
{
   EMSStore *me = (EMSStore*)hStore;

   if(NULL == puiMsgList || 0 == uiMsgNum || 0 == uiActId)
   {
      return EME_BADPARM;
   }

   if(CALLBACK_IsQueued(&me->u.recoverMsg.recoverCallback))
   {
      return EME_FAILED;
   }

   EME_UTILS_FREEIF(NULL, me->u.recoverMsg.puiMsgList);
   me->u.recoverMsg.puiMsgList = EME_UTILS_MALLOC(NULL, uiMsgNum * sizeof(EMS_MSGID));
   if(NULL == me->u.recoverMsg.puiMsgList)
   {
      EME_UTILS_LOG_ERROR(("no memory!!!"));
      return EME_NOMEMORY;
   }
   EME_UTILS_MEMSET(me->u.recoverMsg.puiMsgList, 0x00, uiMsgNum * sizeof(EMS_MSGID));

   EME_UTILS_MEMCPY(me->u.recoverMsg.puiMsgList, puiMsgList, uiMsgNum * sizeof(EMS_MSGID));
   me->u.recoverMsg.uiMsgCount = uiMsgNum;
   me->u.recoverMsg.uiMsgIndex = 0;
   me->u.recoverMsg.uiActId = uiActId;
   me->asynType = EMS_ASYN_TYPE_RECOVER_MSG;

   CALLBACK_Init(&me->u.recoverMsg.recoverCallback, HEMSSTORE_RecoverMsgCB, hStore);
   ISHELL_EmailResume(&me->u.recoverMsg.recoverCallback);

   return EME_SUCCESS;
}

static void HEMSSTORE_CleanMsgCB(WE_VOID *pvData)
{
   EMSStore *me = (EMSStore*)pvData;
   
   EME_MsgIndex_CleanMsg(me->pvMemMgr, 
      me->hFileMgr, 
      me->u.cleanMsg.uiActId, 
      me->u.cleanMsg.uiBoxId,
      me->u.cleanMsg.uiSaveplace);

   me->u.cleanMsg.uiActId = 0;
   me->u.cleanMsg.uiSaveplace = 0;
   
   TS_HZ_EMAIL_EVENT_CALLBACK(BRWENG_EMAIL_MMI_APP, EVT_EMS_CLEAN_MSG_RESULT, NULL, NULL);
   
   return;
}

int HEMSSTORE_CleanMsg(HEMSStore hStore, EMS_ACTID uiActId, EMS_BOXID uiBoxId, unsigned int uiSaveplace)
{
   EMSStore *me = (EMSStore*)hStore;
   
   if(0 == uiActId)
   {
      return EME_BADPARM;
   }
   
   if(CALLBACK_IsQueued(&me->u.cleanMsg.cleanCallback))
   {
      return EME_FAILED;
   }
   
   me->u.cleanMsg.uiActId = uiActId;
   me->u.cleanMsg.uiBoxId = uiBoxId;
   me->u.cleanMsg.uiSaveplace = uiSaveplace;
   me->asynType = EMS_ASYN_TYPE_CLEAN_MSG;
   
   CALLBACK_Init(&me->u.cleanMsg.cleanCallback, HEMSSTORE_CleanMsgCB, hStore);
   ISHELL_EmailResume(&me->u.cleanMsg.cleanCallback);
   
   return EME_SUCCESS;
}

void HEMSSTORE_ClearPathMgr(void)
{
    EME_DataAccess_ClearPathMgr();
}