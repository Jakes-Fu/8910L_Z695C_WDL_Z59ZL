/*==============================================================================
    FILE NAME   : email_store.c
    MODULE NAME : CoreStore


    GENERAL DESCRIPTION:
        Contain the implementation of the core store.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-10-08   Wenkai Xu        01         Create

==============================================================================*/

//pclint
/*lint -e527 */

/*==============================================================================
 * 
 *    Include Files
 * 
 *============================================================================*/
#include "email_store_priv.h"
#include "eme_utils_api.h"
#include "ems_store_priv.h"


/*==============================================================================
 * 
 *    DEFINITIONS AND CONSTANTS(It is not recommended)
 *    It is recommended that this section is defined to private head file.
 * 
 *============================================================================*/


/*==============================================================================
 *
 *    Static Function Prototype Declare Section
 *
 *============================================================================*/



/*==============================================================================
 * 
 *    Functions
 * 
 *============================================================================*/

/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EmailStore_GetMessage
 *
 *    COMMRNT:    Get the whole message from db.
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Create a new multipartmsg handler
 *          3. Get the message data
 *          4. Get the message index info
 * 
 *----------------------------------------------------------------------------*/
static WE_INT32 EmailStore_GetMessage(StoreHandle *me, 
                                      WE_UINT32 dwMsgId)
{
   WE_INT32 nErr = EME_FAILED;
   
   if (NULL == me)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      /* Create a new multipartmsg handler */
      me->hMsg = HMULTIPARTMSG_New(me->pvMemMgr);
      EME_UTILS_BREAKNULL(me->hMsg, nErr, EME_NOMEMORY, 
         "EmailStore_GetMessage, create multipartmsg handler failed");
      
      me->dwMsgId = dwMsgId;
      
      nErr = EME_MsgStore_GetMsg(me->pvMemMgr, 
                                 me->hFileMgr, 
                                 me->dwMsgId, 
                                 me->hMsg);
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_GetMessage, Get msgopt failed");
      
      nErr = EME_MsgIndex_GetMsgIndexInfo(me->hFileMgr, 
                                          me->dwMsgId, 
                                          me->hMsg);
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_GetMessage, Get message index info failed");
      
      /* Generate the message folder path */

      EME_MsgStore_GenerateMsgPath(dwMsgId, me->pcMsgPath, NULL);
      
      nErr = EME_SUCCESS;
   } while(0);
   
   if (EME_SUCCESS != nErr)
   {
      me->dwMsgId = 0;
      if (NULL != me->hMsg)
      {
         HMULTIPARTMSG_Delete(me->hMsg);
         me->hMsg = NULL;
      }
   }
   
   return nErr;
}


static MsgOpt* EmailStore_GetOpt(MsgOpt* pstMsgOpt, WE_INT32 nId)
{
   MsgOpt *pstMsgOptOut= NULL;
   
   if(NULL == pstMsgOpt)
   {       
      return NULL;
   }
   
   pstMsgOptOut = pstMsgOpt;
   /* Search the email opt by opt id */
   while(pstMsgOptOut->nId != MSGOPT_END)
   {
      if(pstMsgOptOut->nId == nId)
      {
         break;
      }
      
      ++pstMsgOptOut;
   }
   
   if(pstMsgOptOut->nId != MSGOPT_END)
   {
      return pstMsgOptOut;
   }
   
   return NULL;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EmailStore_InitHandle
 *
 *    COMMRNT:    Create and init a new EmailStore handler
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Allocate the memory for the handler
 *          3. Init the element in the handler
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EmailStore_InitHandle(HEmailStore *phStore, 
                               EmailOpt *pstOpt, 
                               WE_VOID *pvFileMgr, 
                               WE_VOID *pvMemMgr)
{
   MsgOpt *pstMsgOpt = (MsgOpt *)pstOpt;
   WE_INT32 nErr = EME_FAILED;
   StoreHandle *me = NULL;
   MsgOpt *pstMsgOptTmp = NULL;
   
   if (NULL == phStore)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      me = (StoreHandle *)EME_UTILS_MALLOC(pvMemMgr, sizeof(StoreHandle));
      EME_UTILS_BREAKNULL(me, nErr, EME_NOMEMORY, 
         "EmailStore_InitHandle, no memory for new handler");
      EME_UTILS_MEMSET(me, 0x00, sizeof(StoreHandle));
      
      me->hFileMgr = (FILEMGR_HANDLE)pvFileMgr;
      me->pvMemMgr = pvMemMgr;
      
      if (pstMsgOpt != NULL)
      {
         /* Get accounts id */
         pstMsgOptTmp = EmailStore_GetOpt(pstMsgOpt, EMAILMESSAGE_OPT_ACTID);
         if (NULL == pstMsgOptTmp || 0 == (WE_UINT32)pstMsgOptTmp->pVal)
         {
            EME_UTILS_BREAK(nErr, 
               "EmailStore_InitHandle, no account id");
         }
         
         me->dwActId = (WE_UINT32)pstMsgOptTmp->pVal;
         pstMsgOptTmp = NULL;
         
         /* Get default inbox id */ 
         pstMsgOptTmp = EmailStore_GetOpt(pstMsgOpt, EMAILMESSAGE_OPT_DEF_INBOXID);
         if (NULL == pstMsgOptTmp || 0 == (WE_UINT32)pstMsgOptTmp->pVal)
         {
           // BoxInfo stBoxInfo = {0};
            WE_UINT16 *wszBoxName = NULL;
            
            pstMsgOptTmp = EmailStore_GetOpt(pstMsgOpt, EMAIL_OPT_BOXNAME);
            if (NULL == pstMsgOptTmp || NULL == pstMsgOptTmp->pVal)
            {
               nErr = EME_FAILED;
               EME_UTILS_BREAK(nErr, 
                  "EmailStore_InitHandle, no box id");
            }
            
            wszBoxName = EME_UTILS_UTF8TOWSTR(pvMemMgr, (WE_CHAR *)pstMsgOptTmp->pVal);
            EME_UTILS_BREAKNULL(wszBoxName, nErr, EME_FAILED, "utf8 to wstring failed");
            nErr = EME_MsgIndex_BoxNameToId(me->hFileMgr, 
                                            wszBoxName, 
                                            me->dwActId, 
                                            &me->dwBoxId);
            EME_UTILS_FREEIF(pvMemMgr, wszBoxName);
            EME_UTILS_BREAKIF(nErr, 
               "EmailStore_InitHandle, no box id");
         }
         else 
         {
            me->dwBoxId = (WE_UINT32)pstMsgOptTmp->pVal;
            pstMsgOptTmp = NULL;
         }
      }
      
      nErr = EME_SUCCESS;
   } while(0);
   
   if (EME_SUCCESS == nErr)
   {
      *phStore = (HEmailStore)me;
   }
   else 
   {
      EME_UTILS_FREEIF(pvMemMgr, me);
   }
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EmailStore_DeleteHandle
 *
 *    COMMRNT:    Destroy the handler create by EmailStore_InitHandle.
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Destruct the handler
 *          3. Free the memory of the handler
 * 
 *----------------------------------------------------------------------------*/
WE_VOID EmailStore_DeleteHandle(HEmailStore hStore)
{
   StoreHandle *me = (StoreHandle *)hStore;
   if (NULL == me)
   {
      return;
   }
   
   EME_UTILS_FREEIF(me->pvMemMgr, me->pstHeaderOpt);
   EME_UTILS_FREEIF(me->pvMemMgr, me->pstPartOpt);
   
   if (NULL != me->hMsg)
   {
      HMULTIPARTMSG_Delete(me->hMsg);
      me->hMsg = NULL;
   }
   
   EME_UTILS_FREEIF(me->pvMemMgr, me);
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EmailStore_NewMessage
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Check the storage
 *          3. Check if there is already a message in processing
 *          4. Create a new multipartmsg handler
 *          5. Get one available message id and create the message record
 *          6. Generate the message folder path for later use
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EmailStore_NewMessage(HEmailStore hStore, 
                               WE_UINT32 *pdwMsgId, 
                               WE_CHAR **ppcPath)
{
   WE_INT32 nErr = EME_FAILED;
   StoreHandle *me = (StoreHandle *)hStore;
   MsgOpt aMsgOpt[4] = {0};
   
   if (NULL == me)
   {
      return EME_BADPARM;
   }
   
   if (NULL != me->hMsg)
   {
      EME_UTILS_LOG_ERROR(("EmailStore_NewMessage, already have a message in processing"));
      return EME_ALREADY;
   }
   
   if (!EME_DataAccess_IsEnoughSpace(me->hFileMgr, EMS_LEAST_EML_SPACE))
   {
      EME_UTILS_LOG_ERROR(("EmailStore_NewMessage, no storage for new message"));
      return EME_NOSTORAGE;
   }
   
   do 
   {
      nErr = EME_MsgStore_NewMsg(me->hFileMgr, me->dwActId, &me->dwMsgId);
      EME_UTILS_BREAKIF(nErr, "new msg failed!!");

      EME_MsgStore_GenerateMsgPath(me->dwMsgId, me->pcMsgPath, NULL);

      /* Create a new multipartmsg handler */
      me->hMsg = HMULTIPARTMSG_New(me->pvMemMgr);
      EME_UTILS_BREAKNULL(me->hMsg, nErr, EME_NOMEMORY, 
         "EmailStore_NewMessage, create multipartmsg handler failed");
      
      /* Add needed msgopt into the multipartmsg handler */
      aMsgOpt[0].nId = EME_MSGOPT_ACTID;
      aMsgOpt[0].pVal = (WE_VOID *)me->dwActId;
      
      aMsgOpt[1].nId = EME_MSGOPT_BOXID;
      aMsgOpt[1].pVal = (WE_VOID *)me->dwBoxId;
      
      aMsgOpt[2].nId = EME_MSGOPT_MSGID;
      aMsgOpt[2].pVal = (WE_VOID *)me->dwMsgId;
      
      aMsgOpt[3].nId = MSGOPT_END;
      aMsgOpt[3].pVal = NULL;
      
      nErr = HMULTIPARTMSG_AddOpt(me->hMsg, aMsgOpt);
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_NewMessage, add msgopt failed");
      
      if (NULL != pdwMsgId)
      {
         *pdwMsgId = me->dwMsgId;
      }
      
      if (NULL != ppcPath)
      {
         *ppcPath = me->pcMsgPath;
      }
      
      nErr = EME_SUCCESS;
   } while(0);
   
   if (EME_SUCCESS != nErr)
   {
      if (NULL != me->hMsg)
      {
         HMULTIPARTMSG_Delete(me->hMsg);
         me->hMsg = NULL;
      }
      
      EME_UTILS_MEMSET(me->pcMsgPath, 0x00, sizeof(me->pcMsgPath));
      me->dwMsgId = 0;
   }
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EmailStore_AddMsgHeaders
 *
 *    COMMRNT:    Add message header into the db.
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Check the Message id and the multipartmsg handler
 *          3. Add the MsgOpt into the multiartmsg handler
 *          4. Save the message header into the db
 *          5. Index the message
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EmailStore_AddMsgHeaders(HEmailStore hStore, 
                                  WE_UINT32 dwMsgId, 
                                  EmailOpt *pstOpt)
{
   MsgOpt *pstMsgOpt = (MsgOpt *)pstOpt;
   WE_INT32 nErr = EME_FAILED;
   StoreHandle *me = (StoreHandle *)hStore;
   
   if (NULL == me || NULL == me->hMsg)
   {
      return EME_BADPARM;
   }
   
   if (NULL != me->hMsg && dwMsgId != me->dwMsgId)
   {
      EME_UTILS_LOG_ERROR(
         ("EmailStore_AddMsgHeaders, one message has already in processing"));
      return EME_ALREADY;
   }
   
   do 
   {
      if (NULL == me->hMsg)
      {
         nErr = EmailStore_GetMessage(me, dwMsgId);
         EME_UTILS_BREAKIF(nErr, 
            "EmailStore_DeleteMsgPart, get multipartmsg failed");
      }
      
      nErr = HMULTIPARTMSG_AddOpt(me->hMsg, pstMsgOpt);
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_AddMsgHeaders, add msgopt failed");
      
      nErr = EME_MsgStore_SaveMsgHeader(me->pvMemMgr, 
                                        me->hFileMgr, 
                                        me->hMsg, 
                                        me->dwMsgId);
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_AddMsgHeaders, save message header failed");
      
      nErr = EME_MsgIndex_IndexMsg(me->pvMemMgr, me->hFileMgr, me->hMsg, TRUE);
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_AddMsgHeaders, index message failed");
      
      nErr = EME_SUCCESS;
   } while(0);
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EmailStore_AddMsgPart
 *
 *    COMMRNT:    Add one message part into the db.
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Create a new msgpart handler
 *          3. Add msgopt into msgpart then add msgpart into the multipartmsg
 *          4. Save the message part
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EmailStore_AddMsgPart(HEmailStore hStore, 
                               WE_UINT32 dwMsgId, 
                               EmailOpt *pstOpt)
{
   MsgOpt *pstMsgOpt = (MsgOpt *)pstOpt;
   WE_INT32 nErr = EME_FAILED;
   StoreHandle *me = (StoreHandle *)hStore;
   HMsgPart hMsgPart = NULL;
   
   if (NULL == me || NULL == pstMsgOpt)
   {
      return EME_BADPARM;
   }
   
   if (NULL != me->hMsg && dwMsgId != me->dwMsgId)
   {
      EME_UTILS_LOG_ERROR(
         ("EmailStore_AddMsgPart, one message has already in processing"));
      return EME_ALREADY;
   }
   
   do 
   {
      if (NULL == me->hMsg)
      {
         nErr = EmailStore_GetMessage(me, dwMsgId);
         EME_UTILS_BREAKIF(nErr, 
            "EmailStore_DeleteMsgPart, get multipartmsg failed");
      }
      
      hMsgPart = HMSGPART_New(me->pvMemMgr);
      EME_UTILS_BREAKNULL(hMsgPart, nErr, EME_NOMEMORY, 
         "EmailStore_AddMsgPart, create new msgpart handler failed");
      
      nErr = HMSGPART_AddOpt(hMsgPart, pstMsgOpt);
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_AddMsgPart, add msgopt failed");
      
      nErr = HMULTIPARTMSG_AddMessagePart(me->hMsg, hMsgPart, -1);
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_AddMsgPart, add msgpart failed");
      
      nErr = EME_MsgStore_SaveMsgPart(me->pvMemMgr, 
                                      me->hFileMgr, 
                                      hMsgPart, 
                                      me->dwMsgId);
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_AddMsgPart, save msgpart failed");
      
      nErr = EME_SUCCESS;
   } while(0);
   
   if (NULL != hMsgPart)
   {
      HMSGPART_Delete(hMsgPart);
      hMsgPart = NULL;
   }
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EmailStore_UpdateMsgPart
 *
 *    COMMRNT:    Update one message part in the db.
 *
 *    Pseudo Code:    
 *          1. Check input parameters.
 *          2. If there is no message in the handler, 
 *             get the whole message by message id.
 *          3. Create a new message part and add the part opts.
 *          4. delete the old message part and add the new message part.
 *          5. Resave the message to db.
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EmailStore_UpdateMsgPart(HEmailStore hStore, 
                                  WE_UINT32 dwMsgId, 
                                  WE_UINT32 dwPartId, 
                                  EmailOpt *pstOpt)
{
   MsgOpt *pstMsgOpt = (MsgOpt *)pstOpt;
   WE_INT32 nErr = EME_FAILED;
   StoreHandle *me = (StoreHandle *)hStore;
   HMsgPart hMsgPart = NULL;
   
   if (NULL == me || NULL == pstMsgOpt)
   {
      return EME_BADPARM;
   }
   
   if (NULL != me->hMsg && dwMsgId != me->dwMsgId)
   {
      EME_UTILS_LOG_ERROR(("EmailStore_UpdateMsgPart, one message has already in processing"));
      return EME_ALREADY;
   }
   
   do 
   {
      if (NULL == me->hMsg)
      {
         nErr = EmailStore_GetMessage(me, dwMsgId);
         EME_UTILS_BREAKIF(nErr, 
            "EmailStore_UpdateMsgPart, get multipartmsg failed");
      }
      
      hMsgPart = HMSGPART_New(me->pvMemMgr);
      EME_UTILS_BREAKNULL(hMsgPart, nErr, EME_NOMEMORY, 
         "EmailStore_UpdateMsgPart, create new message part failed");
      
      nErr = HMSGPART_AddOpt(hMsgPart, pstMsgOpt);
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_UpdateMsgPart, add opts to new message part failed");
      
      nErr = HMULTIPARTMSG_RemoveMessagePart(me->hMsg, 
                                             EME_MSGPART_ID_POSITION, 
                                             (WE_VOID *)dwPartId);
      if(nErr != EME_SUCCESS)
      {
         EME_UTILS_LOG_ERROR(("remove msg part failed, it may be not exist !! part id = %d", dwPartId));
      }
      
      nErr = HMULTIPARTMSG_AddMessagePart(me->hMsg, hMsgPart, dwPartId);
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_UpdateMsgPart, add new message part failed");
      
      nErr = EME_MsgStore_SaveMsg(me->pvMemMgr, 
                                  me->hFileMgr, 
                                  me->hMsg, 
                                  FALSE, 
                                  FALSE);
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_UpdateMsgPart, update message to db failed");
      
      nErr = EME_SUCCESS;
   } while(0);
   
   if (NULL != hMsgPart)
   {
      HMSGPART_Delete(hMsgPart);
      hMsgPart = NULL;
   }
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EmailStore_WindUp
 *
 *    COMMRNT:    Delete the file in the message folder which is not referenced 
 *                by the message.
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. If there is no message in the handler, 
 *             get the whole message by message id.
 *          3. Do clean.
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EmailStore_WindUp(HEmailStore hStore, 
                           WE_UINT32 dwMsgId)
{
   WE_INT32 nErr = EME_FAILED;
   StoreHandle *me = (StoreHandle *)hStore;
   
   if (NULL == me)
   {
      return EME_BADPARM;
   }
   
   if (NULL != me->hMsg && dwMsgId != me->dwMsgId)
   {
      EME_UTILS_LOG_ERROR(("EmailStore_WindUp, one message has already in processing"));
      return EME_ALREADY;
   }
   
   do 
   {
      if (NULL == me->hMsg)
      {
         nErr = EmailStore_GetMessage(me, dwMsgId);
         EME_UTILS_BREAKIF(nErr, 
            "EmailStore_WindUp, get multipartmsg failed");
      }
      
      nErr = EME_MsgStore_WindUp(me->pvMemMgr, 
                                 me->hFileMgr, 
                                 me->hMsg);
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_WindUp, update message to db failed");
      
      nErr = EME_SUCCESS;
   } while(0);
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EmailStore_DeleteMsgPart
 *
 *    COMMRNT:    Delete one message part in db.
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. If there is no message in the handler, 
 *             get the whole message by message id.
 *          3. Delete the message part and resave the message to db.
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EmailStore_DeleteMsgPart(HEmailStore hStore, 
                                  WE_UINT32 dwMsgId, 
                                  WE_UINT32 dwPartId)
{
   WE_INT32 nErr = EME_FAILED;
   StoreHandle *me = (StoreHandle *)hStore;
   
   if (NULL == me)
   {
      return EME_BADPARM;
   }
   
   if (NULL != me->hMsg && dwMsgId != me->dwMsgId)
   {
      EME_UTILS_LOG_ERROR(("EmailStore_DeleteMsgPart, one message has already in processing"));
      return EME_ALREADY;
   }
   
   do 
   {
      if (NULL == me->hMsg)
      {
         nErr = EmailStore_GetMessage(me, dwMsgId);
         EME_UTILS_BREAKIF(nErr, 
            "EmailStore_DeleteMsgPart, get multipartmsg failed");
      }
      
      nErr = HMULTIPARTMSG_RemoveMessagePart(me->hMsg, 
                                             EME_MSGPART_ID_POSITION, 
                                             (WE_VOID *)dwPartId);
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_DeleteMsgPart, remove message part failed");
      
      nErr = EME_MsgStore_SaveMsg(me->pvMemMgr, 
                                  me->hFileMgr, 
                                  me->hMsg, 
                                  FALSE, 
                                  FALSE);
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_DeleteMsgPart, update message to db failed");
      
      nErr = EME_SUCCESS;
   } while(0);
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EmailStore_GetMsgHeaders
 *
 *    COMMRNT:    Get the header options from db.
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. If there is no message in the handler, 
 *             get the whole message by message id.
 *          3. Get the option count and allocate the msgopt array.
 *          4. Get all msgopt from multipartmsg handler
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EmailStore_GetMsgHeaders(HEmailStore hStore, 
                                  WE_UINT32 dwMsgId, 
                                  EmailOpt **ppstOpt)
{
   WE_INT32 nErr = EME_FAILED;
   StoreHandle *me = (StoreHandle *)hStore;
   WE_UINT32 dwOptNum = 0;
   WE_UINT32 dwOptPos = 0;
   
   if (NULL == me || NULL == ppstOpt)
   {
      return EME_BADPARM;
   }
   
   if (NULL != me->hMsg && dwMsgId != me->dwMsgId)
   {
      EME_UTILS_LOG_ERROR(("EmailStore_GetMsgHeaders, one message has already in processing"));
      return EME_ALREADY;
   }
   
   do 
   {
      if (NULL == me->hMsg)
      {
         nErr = EmailStore_GetMessage(me, dwMsgId);
         EME_UTILS_BREAKIF(nErr, 
            "EmailStore_GetMsgHeaders, get multipartmsg failed");
      }
      
      EME_UTILS_FREEIF(me->pvMemMgr, me->pstHeaderOpt);
      dwOptNum = HMULTIPARTMSG_GetOptCount(me->hMsg);
      me->pstHeaderOpt = (MsgOpt *)EME_UTILS_MALLOC(me->pvMemMgr, sizeof(MsgOpt) * (dwOptNum + 1));
      EME_UTILS_BREAKNULL(me->pstHeaderOpt, nErr, EME_NOMEMORY, 
         "EmailStore_GetMsgHeaders, no memory for msgopt array");
      EME_UTILS_MEMSET(me->pstHeaderOpt, 0x00, sizeof(MsgOpt) * (dwOptNum + 1));
      me->pstHeaderOpt[dwOptNum].nId = MSGOPT_END;
      me->pstHeaderOpt[dwOptNum].pVal = NULL;
      
      for (dwOptPos = 0; dwOptPos < dwOptNum; ++dwOptPos)
      {
         nErr = HMULTIPARTMSG_GetOpt(me->hMsg, 
                                     MSGOPT_ANY, 
                                     dwOptPos, 
                                     &me->pstHeaderOpt[dwOptPos]);
         EME_UTILS_BREAKIF(nErr, 
            "EmailStore_GetMsgHeaders, get one msgopt failed");
      }
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_GetMsgHeaders, get msgopts failed");
      
      *ppstOpt = (EmailOpt *)me->pstHeaderOpt;
      
      nErr = EME_SUCCESS;
   } while(0);
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EmailStore_GetMsgPartCount
 *
 *    COMMRNT:    Get the part count of one message in the db.
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. If there is no message in the handler, 
 *             get the whole message by message id.
 *          3. Get and return the part count.
 *
 *----------------------------------------------------------------------------*/
WE_INT32 EmailStore_GetMsgPartCount(HEmailStore hStore, 
                                    WE_UINT32 dwMsgId, 
                                    WE_UINT32 *pdwPartNum)
{
   WE_INT32 nErr = EME_FAILED;
   StoreHandle *me = (StoreHandle *)hStore;
   
   if (NULL == me || NULL == pdwPartNum)
   {
      return EME_BADPARM;
   }
   
   if (NULL != me->hMsg && dwMsgId != me->dwMsgId)
   {
      EME_UTILS_LOG_ERROR(("EmailStore_GetMsgPartCount, one message has already in processing"));
      return EME_ALREADY;
   }
   
   do 
   {
      if (NULL == me->hMsg)
      {
         nErr = EmailStore_GetMessage(me, dwMsgId);
         EME_UTILS_BREAKIF(nErr, 
            "EmailStore_GetMsgPartCount, get multipartmsg failed");
      }
      
      *pdwPartNum = HMULTIPARTMSG_GetPartCount(me->hMsg);
      
      nErr = EME_SUCCESS;
   } while(0);
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EmailStore_GetMsgPart
 *
 *    COMMRNT:    Get one message part form the db.
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. If there is no message in the handler, 
 *             get the whole message by message id.
 *          3. Get the msgpart from the multipartmsg handler indicate bu part id
 *          4. Get the option count and allocate the msgopt array.
 *          5. Get all msgopt from msgpart handler.
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EmailStore_GetMsgPart(HEmailStore hStore, 
                               WE_UINT32 dwMsgId, 
                               WE_UINT32 dwPartId, 
                               EmailOpt **ppstOpt)
{
   WE_INT32 nErr = EME_FAILED;
   StoreHandle *me = (StoreHandle *)hStore;
   HMsgPart hMsgPart = NULL;
   WE_UINT32 dwOptNum = 0;
   WE_UINT32 dwOptPos = 0;
   
   if (NULL == me || NULL == ppstOpt)
   {
      return EME_BADPARM;
   }
   
   if (NULL != me->hMsg && dwMsgId != me->dwMsgId)
   {
      EME_UTILS_LOG_ERROR(("EmailStore_GetMsgPart, one message has already in processing"));
      return EME_ALREADY;
   }
   
   do 
   {
      if (NULL == me->hMsg)
      {
         nErr = EmailStore_GetMessage(me, dwMsgId);
         EME_UTILS_BREAKIF(nErr, 
            "EmailStore_GetMsgPart, get multipartmsg failed");
      }
      
      nErr = HMULTIPARTMSG_GetMessagePart(me->hMsg, 
                                          EME_MSGPART_ID_POSITION, 
                                          (WE_VOID *)dwPartId, 
                                          &hMsgPart);
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_GetMsgPart, get msgpart failed");
      
      EME_UTILS_FREEIF(me->pvMemMgr, me->pstPartOpt);
      dwOptNum = HMSGPART_GetOptCount(hMsgPart);
      me->pstPartOpt = (MsgOpt *)EME_UTILS_MALLOC(me->pvMemMgr, sizeof(MsgOpt) * (dwOptNum + 1));
      EME_UTILS_BREAKNULL(me->pstPartOpt, nErr, EME_NOMEMORY, 
         "EmailStore_GetMsgPart, no memory for msgopt array");
      EME_UTILS_MEMSET(me->pstPartOpt, 0x00, sizeof(MsgOpt) * (dwOptNum + 1));
      me->pstPartOpt[dwOptNum].nId = MSGOPT_END;
      me->pstPartOpt[dwOptNum].pVal = NULL;
      
      for (dwOptPos = 0; dwOptPos < dwOptNum; ++dwOptPos)
      {
         nErr = HMSGPART_GetOpt(hMsgPart, 
                                MSGOPT_ANY, 
                                dwOptPos, 
                                &me->pstPartOpt[dwOptPos]);
         EME_UTILS_BREAKIF(nErr, 
            "EmailStore_GetMsgPart, get one msgopt failed");
      }
      EME_UTILS_BREAKIF(nErr, 
         "EmailStore_GetMsgPart, get msgopts failed");
      
      *ppstOpt = (EmailOpt *)me->pstPartOpt;
      
      nErr = EME_SUCCESS;
   } while(0);
   
   if (NULL != hMsgPart)
   {
      HMSGPART_Delete(hMsgPart);
      hMsgPart = NULL;
   }
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EmailStore_SetMsgDownStatus
 *
 *    COMMRNT:    Update the bDown option in the db.
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. update the bDown option in the multipartmsg handler.
 *          3. update the msgextinfo in the db.
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EmailStore_SetMsgDownStatus(HEmailStore hStore, 
                                     WE_UINT32 dwMsgId, 
                                     WE_BOOL bDown)
{
   WE_INT32 nErr = EME_FAILED;
   StoreHandle *me = (StoreHandle *)hStore;
   MsgOpt astMsgOpt[2] = {0};
   WE_CHAR    acMsgFile[64] = {0};
   WE_CHAR    *pcMsgFileBuff = NULL;
   WE_UINT32  uiMsgFileSize = 0;
   
   if (NULL == me)
   {
      return EME_BADPARM;
   }
   
   if (NULL != me->hMsg && dwMsgId != me->dwMsgId)
   {
      EME_UTILS_LOG_ERROR(("EmailStore_SetMsgDownStatus, one message has already in processing"));
      return EME_ALREADY;
   }
   
   do 
   {
      EME_MsgStore_GenerateMsgPath(dwMsgId, NULL, acMsgFile);

      if (NULL == me->hMsg)
      {
         nErr = EmailStore_GetMessage(me, dwMsgId);
         EME_UTILS_BREAKIF(nErr, 
            "EmailStore_SetMsgDownStatus, get multipartmsg failed");
      }
      
      astMsgOpt[1].nId = EMAILOPT_END;
      nErr = HMULTIPARTMSG_GetOpt(me->hMsg, EME_MSGOPT_BEDOWN, 0, &astMsgOpt[0]);
      if (EME_SUCCESS == nErr)
      {
         astMsgOpt[0].pVal = (WE_VOID *)bDown;
         nErr = HMULTIPARTMSG_UpdateOpt(me->hMsg, 0, &astMsgOpt[0]);
         EME_UTILS_BREAKIF(nErr, 
         "EmailStore_SetMsgDownStatus, update bDown option failed");
      }
      else
      {
         astMsgOpt[0].nId = EME_MSGOPT_BEDOWN;
         astMsgOpt[0].pVal = (WE_VOID *)bDown;
         nErr = HMULTIPARTMSG_AddOpt(me->hMsg, astMsgOpt);
         EME_UTILS_BREAKIF(nErr, 
         "EmailStore_SetMsgDownStatus, update bDown option failed");
      }
      
      nErr = IWEMSG_EncodeMsg(me->hMsg, (WE_UCHAR**)&pcMsgFileBuff, &uiMsgFileSize);
      EME_UTILS_BREAKIF(nErr, "encode msg failed!!");

      nErr = EME_DataAccess_UpdateFileByBuffer(me->hFileMgr, acMsgFile, pcMsgFileBuff, uiMsgFileSize);
      EME_UTILS_BREAKIF(nErr, "update failed!!");

      nErr = EME_MsgIndex_IndexMsg(me->pvMemMgr, me->hFileMgr, me->hMsg, FALSE);
      EME_UTILS_BREAKIF(nErr, "update msg down faild!!");

   } while(0);

   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EmailStore_GetMsgPath
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EmailStore_GetMsgPath(HEmailStore hStore, 
                               WE_UINT32 dwMsgId, 
                               WE_CHAR **ppcPath)
{
   WE_INT32 nErr = EME_FAILED;
   StoreHandle *me = (StoreHandle *)hStore;
   
   if (NULL == me || NULL == ppcPath)
   {
      return EME_BADPARM;
   }
   
   if (NULL != me->hMsg && dwMsgId != me->dwMsgId)
   {
      EME_UTILS_LOG_ERROR(
         ("EmailStore_GetMsgPath, one message has already in processing"));
      return EME_ALREADY;
   }
   
   do 
   {
      if (NULL == me->hMsg)
      {
         nErr = EmailStore_GetMessage(me, dwMsgId);
         EME_UTILS_BREAKIF(nErr, 
            "EmailStore_SetMsgDownStatus, get multipartmsg failed");
      }
      
      *ppcPath = me->pcMsgPath;
      
      nErr = EME_SUCCESS;
   } while(0);
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EmailStore_DeleteMsg
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EmailStore_DeleteMsg(HEmailStore hStore, WE_UINT32 dwMsgId)
{
   WE_INT32 nErr = EME_FAILED;
   StoreHandle *me = (StoreHandle *)hStore;

   if (NULL == me)
   {
      return EME_BADPARM;
   }
   
   /* Must remove index first ignore the errors */
   nErr = EME_MsgIndex_RemoveMsg(me->pvMemMgr, me->hFileMgr, dwMsgId);
   nErr = EME_MsgStore_RemoveMsg(me->hFileMgr, dwMsgId);
   
   return nErr;
}
