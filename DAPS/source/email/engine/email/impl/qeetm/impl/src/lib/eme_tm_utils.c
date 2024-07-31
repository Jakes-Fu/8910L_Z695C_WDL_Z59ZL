/*=====================================================================================
FILE NAME   : eme_tm_utils.c
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Provide some function for managing sigs.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                  create eme_tm_utils.c
          
=====================================================================================*/

/*===================================================================
* 
*                     Include Files
* 
====================================================================*/
/*Include System head file*/

/*Include Program Global head file*/
#include "eme_utils_api.h"
#include "we_def.h"
#include "eme_error.h"
/*Include Module head file*/
#include "eme_tm_api.h"
#include "eme_tm_def.h"
#include "eme_tm_utils_api.h"
#include "eme_constant_api.h"
#include "eme_transaction_api.h"
#include "eme_smtp_api.h"
#include "eme_pop_api.h"
#include "eme_imap_api.h"
/*Include Private head file*/


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
EMEOpt* EME_CopyAllOpts(WE_VOID* pvMemHdr, EMEOpt *pstOrigOpts)
{
   WE_INT32    iOptNum = 0;
   EMEOpt      *pstNewOpts = NULL;
   EMEOpt      *pstOneOpt = NULL;
   EMEOpt      *pstOptCopy = NULL;
   WE_INT32    iRet = EME_SUCCESS;

   if (pstOrigOpts == NULL)
   {
      return NULL;
   }

   do 
   {
      /* calculate opts numbers */
      pstOneOpt = pstOrigOpts;
      while (pstOneOpt->iId != EME_OPTID_END)
      {
         iOptNum++;
         pstOneOpt++;
      }
      

      pstNewOpts = (EMEOpt*)EME_UTILS_MALLOC(pvMemHdr, (iOptNum + 1) * sizeof(EMEOpt));
      EME_UTILS_BREAKNULL(pstNewOpts, iRet, EME_NOMEMORY, "EME_CopyAllOpts, no memory");
      EME_UTILS_MEMSET(pstNewOpts, 0, (iOptNum + 1) * sizeof(EMEOpt));

      /* copy opts */
      pstOneOpt = pstOrigOpts;
      pstOptCopy = pstNewOpts;
      for (; iOptNum > 0; iOptNum--, pstOneOpt++, pstOptCopy++)
      {
         pstOptCopy->iId = pstOneOpt->iId;

         /* str opts */
         if (pstOneOpt->iId & EME_OPTID_STR_START)
         {
            if (pstOneOpt->pVal == NULL)
            {
               iRet = EME_FAILED;
               EME_UTILS_BREAK(iRet, "EME_CopyAllOpts, invalid opt val");
            }
	     EME_UTILS_LOG_INFO(("EME_CopyAllOpts: pstOneOpt->iId = %x val=%s", pstOneOpt->iId, pstOneOpt->pVal));

            pstOptCopy->pVal = (WE_VOID*)EME_UTILS_STRDUP(pvMemHdr, (WE_CHAR*)pstOneOpt->pVal);
            EME_UTILS_BREAKNULL(pstOptCopy->pVal, iRet, EME_NOMEMORY, "EME_CopyAllOpts, no memory");
         }
         /* int opts */
         else
         {
            EME_UTILS_LOG_INFO(("EME_CopyAllOpts: pstOneOpt->iId = %x val=%d", pstOneOpt->iId, pstOneOpt->pVal));
         
            pstOptCopy->pVal = pstOneOpt->pVal;
         }
      }
      pstOptCopy->iId = EME_OPTID_END;

      EME_UTILS_BREAKIF(iRet, "EME_CopyAllOpts, copy opt failed");

      

   } while(0);

   if (iRet != EME_SUCCESS)
   {
      EME_FreeAllOptValue(pvMemHdr, pstNewOpts);
      EME_UTILS_FREEIF(pvMemHdr, pstNewOpts);
   }

   return pstNewOpts;
}

WE_INT32 EME_GetStrOptValue(EMEOpt *pstOrigOpts, WE_INT32 iOptId, WE_CHAR** ppcVal)
{
   EMEOpt      *pstOneOpt = NULL;

   if (pstOrigOpts == NULL || 
      ppcVal == NULL || 
      (!(iOptId & EME_OPTID_STR_START) && 
      !(iOptId & EMAILOPT_STR_START)))
   {
      return EME_FAILED;
   }

   pstOneOpt = pstOrigOpts;
   while (pstOneOpt->iId != EME_OPTID_END)
   {
      if (pstOneOpt->iId == iOptId)
      {
         *ppcVal = pstOneOpt->pVal;
         return EME_SUCCESS;
      }
      pstOneOpt++;
   }

   return EME_FAILED;
}


WE_INT32 EME_GetIntOptValue(EMEOpt *pstOrigOpts, WE_INT32 iOptId, WE_INT32* piVal)
{
   EMEOpt      *pstOneOpt = NULL;

   if (pstOrigOpts == NULL || 
      piVal == NULL || 
      (!(iOptId & EME_OPTID_INT_START) &&
      !(iOptId & EMAILOPT_INT_START)))
   {
      return EME_FAILED;
   }

   pstOneOpt = pstOrigOpts;
   while (pstOneOpt->iId != EME_OPTID_END)
   {
      if (pstOneOpt->iId == iOptId)
      {
         *piVal = (WE_INT32)((WE_INT32*)pstOneOpt->pVal);
         return EME_SUCCESS;
      }
      pstOneOpt++;
   }

   return EME_FAILED;
}


WE_INT32 EME_ChangeIntOptValue(EMEOpt *pstOrigOpts, WE_INT32 iOptId, WE_INT32 iVal)
{
   EMEOpt      *pstOneOpt = NULL;
   
   if (pstOrigOpts == NULL || 
       0 == iVal || 
      (!(iOptId & EME_OPTID_INT_START) &&
      !(iOptId & EMAILOPT_INT_START)))
   {
      return EME_FAILED;
   }
   
   pstOneOpt = pstOrigOpts;
   while (pstOneOpt->iId != EME_OPTID_END)
   {
      if (pstOneOpt->iId == iOptId)
      {
         pstOneOpt->pVal = (WE_VOID*)iVal;
         return EME_SUCCESS;
      }
      pstOneOpt++;
   }
   
   return EME_FAILED;
}

WE_INT32 EME_ChangeStrOptValue(EMEOpt *pstOrigOpts, WE_INT32 iOptId, WE_CHAR* pcVal)
{
   EMEOpt      *pstOneOpt = NULL;
   
   if (pstOrigOpts == NULL || 
       NULL == pcVal || 
      (!(iOptId & EME_OPTID_STR_START) &&
      !(iOptId & EMAILOPT_STR_START)))
   {
      return EME_FAILED;
   }
   
   pstOneOpt = pstOrigOpts;
   while (pstOneOpt->iId != EME_OPTID_END)
   {
      if (pstOneOpt->iId == iOptId)
      {
         EME_UTILS_FREEIF(NULL, pstOneOpt->pVal);
         pstOneOpt->pVal = EME_UTILS_STRDUP(NULL, pcVal);
         return EME_SUCCESS;
      }
      pstOneOpt++;
   }
   
   return EME_FAILED;
}

WE_INT32 EME_FreeAllOptValue(WE_VOID* pvMemHdr, EMEOpt *pstFreeOpts)
{
   EMEOpt      *pstOneOpt = NULL;
   WE_INT32    iRet = EME_SUCCESS;

   if (pstFreeOpts == NULL)
   {
      return EME_SUCCESS;
   }

   pstOneOpt = pstFreeOpts;
   while (pstOneOpt->iId != EME_OPTID_END)
   {
      if (pstOneOpt->iId & EME_OPTID_STR_START)
      {
         EME_UTILS_FREEIF(pvMemHdr, pstOneOpt->pVal);        
      }
      pstOneOpt++;
   }

   return iRet;
}


WE_INT32 EME_FreeTransEvt(WE_VOID* pvMemHdr, St_TransEvt *pstFreeTransEvt)
{
   if (pstFreeTransEvt == NULL)
   {
      return EME_SUCCESS;
   }
   
   EME_FreeAllOptValue(pvMemHdr, pstFreeTransEvt->pstAccountOpts);
   EME_UTILS_FREEIF(pvMemHdr, pstFreeTransEvt->pstAccountOpts);

   EME_FreeAllOptValue(pvMemHdr, pstFreeTransEvt->pstTransOpts);
   EME_UTILS_FREEIF(pvMemHdr, pstFreeTransEvt->pstTransOpts);

   EME_UTILS_FREEIF(pvMemHdr, pstFreeTransEvt->pcUIDL);
   return EME_SUCCESS;
}


 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_POPCloseNeed   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. get new transaction
                3. check if the transtype or server or user name or need ssl 
                   or network type of new transaction is different from old one ,
                   return false,
                   else return true
 *  
 * ===================================================================================================*/
WE_BOOL EME_SMTPCloseNeed(St_TMHandle *pstTmHandle, St_TransEvt *pstOldTransEvt)
{
   St_TransEvt *pstNewTransEvt = NULL;
   WE_CHAR     *pcOldUser = NULL;
   WE_CHAR     *pcNewUser = NULL;
   WE_CHAR     *pcOldServer = NULL;
   WE_CHAR     *pcNewServer = NULL;
   WE_INT32    iOldNeedSSL = FALSE;
   WE_INT32    iNewNeedSSL = FALSE;
   WE_INT32    iNewPDP = 0;
   WE_INT32    iOldPDP = 0;
   WE_INT32    iOldAuth = 0;
   WE_INT32    iNewAuth = 0;
   WE_INT32    iRet = EME_SUCCESS;


   if (pstTmHandle == NULL || pstOldTransEvt == NULL)
   {
      return TRUE;
   }

   do 
   {
      pstNewTransEvt = EME_NextTransEvt(pstTmHandle);
      EME_UTILS_BREAKNULL(pstNewTransEvt, iRet, EME_FAILED, "EME_SMTPCloseNeed, no more transactions");

      switch (pstNewTransEvt->uiTransType)
      {
      case EME_TRANS_SMTP_SEND_MAIL:      
         break;

      default:
         iRet = EME_FAILED;
         break;
      }
      EME_UTILS_BREAKIF(iRet, "EME_SMTPCloseNeed, stack changed");

      iRet = EME_GetStrOptValue(pstOldTransEvt->pstAccountOpts, EME_OPTID_SEND_SERVER_ADDR, &pcOldServer);
      EME_UTILS_BREAKIF(iRet, "EME_SMTPCloseNeed, no server address");
      EME_UTILS_BREAKNULL(pcOldServer, iRet, EME_FAILED, "EME_SMTPCloseNeed, no server address");

      iRet = EME_GetStrOptValue(pstNewTransEvt->pstAccountOpts, EME_OPTID_SEND_SERVER_ADDR, &pcNewServer);
      EME_UTILS_BREAKIF(iRet, "EME_SMTPCloseNeed, no server address");
      EME_UTILS_BREAKNULL(pcNewServer, iRet, EME_FAILED, "EME_SMTPCloseNeed, no server address");

      iRet = EME_GetStrOptValue(pstOldTransEvt->pstAccountOpts, EME_OPTID_SEND_USERNAME, &pcOldUser);
      EME_UTILS_BREAKIF(iRet, "EME_SMTPCloseNeed, no server address");
      EME_UTILS_BREAKNULL(pcOldUser, iRet, EME_FAILED, "EME_SMTPCloseNeed, no server address");

      iRet = EME_GetStrOptValue(pstNewTransEvt->pstAccountOpts, EME_OPTID_SEND_USERNAME, &pcNewUser);
      EME_UTILS_BREAKIF(iRet, "EME_SMTPCloseNeed, no server address");
      EME_UTILS_BREAKNULL(pcNewUser, iRet, EME_FAILED, "EME_SMTPCloseNeed, no server address");

      iRet = EME_GetIntOptValue(pstOldTransEvt->pstAccountOpts, EME_OPTID_SMTP_NEED_SSL, &iOldNeedSSL);
      EME_UTILS_BREAKIF(iRet, "EME_SMTPCloseNeed, no ssl");
      
      iRet = EME_GetIntOptValue(pstNewTransEvt->pstAccountOpts, EME_OPTID_SMTP_NEED_SSL, &iNewNeedSSL);
      EME_UTILS_BREAKIF(iRet, "EME_SMTPCloseNeed, no ssl");

      EME_GetIntOptValue(pstOldTransEvt->pstAccountOpts, EME_OPTID_NETWORK_UMTS_PDP, &iOldPDP);
      //EME_UTILS_BREAKIF(iRet, "EME_SMTPCloseNeed, no ssl");
      EME_GetIntOptValue(pstNewTransEvt->pstAccountOpts, EME_OPTID_NETWORK_UMTS_PDP, &iNewPDP);
      //EME_UTILS_BREAKIF(iRet, "EME_SMTPCloseNeed, no ssl");

      iRet = EME_GetIntOptValue(pstOldTransEvt->pstAccountOpts, EME_OPTID_SMTP_AUTHENTICATE, &iOldAuth);
      EME_UTILS_BREAKIF(iRet, "EME_SMTPCloseNeed, no ssl");
      
      iRet = EME_GetIntOptValue(pstNewTransEvt->pstAccountOpts, EME_OPTID_SMTP_AUTHENTICATE, &iNewAuth);
      EME_UTILS_BREAKIF(iRet, "EME_SMTPCloseNeed, no ssl");
      

      if (EME_UTILS_STRCMPNC(pcNewServer, pcOldServer) || 
          EME_UTILS_STRCMPNC(pcNewUser, pcOldUser) ||
          iOldNeedSSL != iNewNeedSSL ||
          iOldPDP != iNewPDP ||
          iOldAuth != iNewPDP) 
      {
         iRet = EME_FAILED;
         EME_UTILS_LOG_INFO(("EME_SMTPCloseNeed, server or user changed"));
      }

   } while(0);

   if (iRet != EME_SUCCESS)
   {
      return TRUE;
   }
   return FALSE;
}


 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_POPCloseNeed   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. get new transaction
                3. check if the transtype or server or user name or need ssl 
                   or network type of new transaction is different from old one ,
                   return false,
                   else return true
 *  
 * ===================================================================================================*/
WE_BOOL EME_POPCloseNeed(St_TMHandle *pstTmHandle, St_TransEvt *pstOldTransEvt)
{
   St_TransEvt *pstNewTransEvt = NULL;
   WE_CHAR     *pcOldUser = NULL;
   WE_CHAR     *pcNewUser = NULL;
   WE_CHAR     *pcOldServer = NULL;
   WE_CHAR     *pcNewServer = NULL;
   WE_INT32    iOldNeedSSL = FALSE;
   WE_INT32    iNewNeedSSL = FALSE;
   WE_INT32    iNewPDP = 0;
   WE_INT32    iOldPDP = 0;
   WE_INT32    iRet = EME_SUCCESS;

   if (pstTmHandle == NULL || pstOldTransEvt == NULL)
   {
      return TRUE;
   }

   do 
   {
      pstNewTransEvt = EME_NextTransEvt(pstTmHandle);
      EME_UTILS_BREAKNULL(pstNewTransEvt, iRet, EME_FAILED, "EME_POPCloseNeed, no more transactions");

      switch (pstNewTransEvt->uiTransType)
      {
      case EME_TRANS_POP3_FETCH_NEW_MAILS_HEADER:
      case EME_TRANS_POP3_FETCH_NEW_MAILS:
      case EME_TRANS_POP3_FETCH_SPEC_MAIL:
//       case EME_TRANS_POP3_FETCH_SPEC_MAIL_HEADER:
      case EME_TRANS_POP3_DELETE_SPEC_MAIL:
      case EME_TRANS_POP3_DELETE_ALL_MAIL:
         break;

      default:
         iRet = EME_FAILED;
         break;
      }
      EME_UTILS_BREAKIF(iRet, "EME_POPCloseNeed, stack changed");


      iRet = EME_GetStrOptValue(pstOldTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_SERVER_ADDR, &pcOldServer); 
     EME_UTILS_LOG_INFO(("EME_POPCloseNeed  : pcOldServer = %s\n!", pcOldServer));
      EME_UTILS_BREAKIF(iRet, "EME_POPCloseNeed, no server address");
      EME_UTILS_BREAKNULL(pcOldServer, iRet, EME_FAILED, "EME_SMTPCloseNeed, no server address");

      iRet = EME_GetStrOptValue(pstNewTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_SERVER_ADDR, &pcNewServer);
      EME_UTILS_LOG_INFO(("EME_POPCloseNeed  : pcOldServer = %s\n!", pcNewServer));
      EME_UTILS_BREAKIF(iRet, "EME_POPCloseNeed, no server address");
      EME_UTILS_BREAKNULL(pcNewServer, iRet, EME_FAILED, "EME_SMTPCloseNeed, no server address");

      iRet = EME_GetStrOptValue(pstOldTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_USERNAME, &pcOldUser);
      EME_UTILS_LOG_INFO(("EME_POPCloseNeed  : pcOldUser = %s\n!", pcOldUser));
      EME_UTILS_BREAKIF(iRet, "EME_POPCloseNeed, no server address");
      EME_UTILS_BREAKNULL(pcOldUser, iRet, EME_FAILED, "EME_SMTPCloseNeed, no server address");

      iRet = EME_GetStrOptValue(pstNewTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_USERNAME, &pcNewUser);
     EME_UTILS_LOG_INFO(("EME_POPCloseNeed  : pcNewUser = %s\n!", pcNewUser));
      EME_UTILS_BREAKIF(iRet, "EME_POPCloseNeed, no server address");
      EME_UTILS_BREAKNULL(pcNewUser, iRet, EME_FAILED, "EME_SMTPCloseNeed, no server address");

      iRet = EME_GetIntOptValue(pstOldTransEvt->pstAccountOpts, EME_OPTID_POP_NEED_SSL, &iOldNeedSSL);
      EME_UTILS_LOG_INFO(("EME_POPCloseNeed  : iOldNeedSSL = %s\n!", iOldNeedSSL));
      EME_UTILS_LOG_INFO(("EME_POPCloseNeed  : iret = %s\n!", iRet));
      EME_UTILS_BREAKIF(iRet, "EME_POPCloseNeed, no ssl");
      
      iRet = EME_GetIntOptValue(pstNewTransEvt->pstAccountOpts, EME_OPTID_POP_NEED_SSL, &iNewNeedSSL);
      EME_UTILS_BREAKIF(iRet, "EME_POPCloseNeed, no ssl");

      EME_GetIntOptValue(pstOldTransEvt->pstAccountOpts, EME_OPTID_NETWORK_UMTS_PDP, &iOldPDP);
      //EME_UTILS_BREAKIF(iRet, "EME_POPCloseNeed, no ssl");
      EME_GetIntOptValue(pstNewTransEvt->pstAccountOpts, EME_OPTID_NETWORK_UMTS_PDP, &iNewPDP);
      //EME_UTILS_BREAKIF(iRet, "EME_POPCloseNeed, no ssl");

      if (EME_UTILS_STRCMPNC(pcNewServer, pcOldServer) || 
          EME_UTILS_STRCMPNC(pcNewUser, pcOldUser) ||
          iNewNeedSSL != iOldNeedSSL ||
          iOldPDP != iNewPDP)
      {
         iRet = EME_FAILED;
         EME_UTILS_LOG_INFO(("EME_POPCloseNeed, server or user changed"));
      }

   } while(0);

   if (iRet != EME_SUCCESS)
   {
      return TRUE;
   }
   return FALSE;
}

 /* -----------------------------------------------------------------------------------------------------
 * 
 *    FUNCTION    EME_IMAPCloseNeed   
 *
 *    Pseudo Code:    
 *              1. check input parameters
 *              2. get new transaction
                3. check if the transtype or server or user name or need ssl 
                   or network type of new transaction is different from old one ,
                   return false,
                   else return true
 *  
 * ===================================================================================================*/
WE_BOOL EME_IMAPCloseNeed(St_TMHandle *pstTmHandle, St_TransEvt *pstOldTransEvt)
{
   St_TransEvt *pstNewTransEvt = NULL;
   WE_CHAR     *pcOldUser = NULL;
   WE_CHAR     *pcNewUser = NULL;
   WE_CHAR     *pcOldServer = NULL;
   WE_CHAR     *pcNewServer = NULL;
   WE_INT32    iOldNeedSSL = FALSE;
   WE_INT32    iNewNeedSSL = FALSE;
   WE_INT32    iNewPDP = 0;
   WE_INT32    iOldPDP = 0;
   WE_INT32    iRet = EME_SUCCESS;

   if (pstTmHandle == NULL || pstOldTransEvt == NULL)
   {
      return TRUE;
   }

   do 
   {
      pstNewTransEvt = EME_NextTransEvt(pstTmHandle);
      EME_UTILS_BREAKNULL(pstNewTransEvt, iRet, EME_FAILED, "EME_IMAPCloseNeed, no more transactions");

      switch (pstNewTransEvt->uiTransType)
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
      case EME_TRANS_IMAP4_UPLOAD_MAIL_TO_SERVER:
      case EME_TRANS_IMAP4_EXPUNGE_DEL_MAIL_IN_MAILBOX:
      case EME_TRANS_IMAP4_CREAT_MAILBOX:
      case EME_TRANS_IMAP4_RENAME_MAILBOX:
      case EME_TRANS_IMAP4_DELETE_MAILBOX:
      case EME_TRANS_IMAP4_SUBSCRIBE_MAILBOX:
      case EME_TRANS_IMAP4_UNSUBSCRIBE_MAILBOX:
      case EME_TRANS_IMAP4_LIST_SUBSCRIBED_MAILBOXES:
      case EME_TRANS_IMAP4_LIST_ALL_MAILBOXES:
         break;

      default:
         iRet = EME_FAILED;   /*transtype is not imap4*/
         break;
      }
      EME_UTILS_BREAKIF(iRet, "EME_IMAPCloseNeed, stack changed");

      iRet = EME_GetStrOptValue(pstOldTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_SERVER_ADDR, &pcOldServer);
      EME_UTILS_BREAKIF(iRet, "EME_IMAPCloseNeed, no server address");
      EME_UTILS_BREAKNULL(pcOldServer, iRet, EME_FAILED, "EME_SMTPCloseNeed, no server address");

      iRet = EME_GetStrOptValue(pstNewTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_SERVER_ADDR, &pcNewServer);
      EME_UTILS_BREAKIF(iRet, "EME_IMAPCloseNeed, no server address");
      EME_UTILS_BREAKNULL(pcNewServer, iRet, EME_FAILED, "EME_SMTPCloseNeed, no server address");

      iRet = EME_GetStrOptValue(pstOldTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_USERNAME, &pcOldUser);
      EME_UTILS_BREAKIF(iRet, "EME_IMAPCloseNeed, no server address");
      EME_UTILS_BREAKNULL(pcOldUser, iRet, EME_FAILED, "EME_SMTPCloseNeed, no server address");

      iRet = EME_GetStrOptValue(pstNewTransEvt->pstAccountOpts, EME_OPTID_RECEIVE_USERNAME, &pcNewUser);
      EME_UTILS_BREAKIF(iRet, "EME_IMAPCloseNeed, no server address");
      EME_UTILS_BREAKNULL(pcNewUser, iRet, EME_FAILED, "EME_SMTPCloseNeed, no server address");

      iRet = EME_GetIntOptValue(pstOldTransEvt->pstAccountOpts, EME_OPTID_IMAP_NEED_SSL, &iOldNeedSSL);
      EME_UTILS_BREAKIF(iRet, "EME_IMAPCloseNeed, no ssl");

      iRet = EME_GetIntOptValue(pstNewTransEvt->pstAccountOpts, EME_OPTID_IMAP_NEED_SSL, &iNewNeedSSL);
      EME_UTILS_BREAKIF(iRet, "EME_IMAPCloseNeed, no ssl");

      EME_GetIntOptValue(pstOldTransEvt->pstAccountOpts, EME_OPTID_NETWORK_UMTS_PDP, &iOldPDP);
      //EME_UTILS_BREAKIF(iRet, "EME_SMTPCloseNeed, no ssl");
      EME_GetIntOptValue(pstNewTransEvt->pstAccountOpts, EME_OPTID_NETWORK_UMTS_PDP, &iNewPDP);
      //EME_UTILS_BREAKIF(iRet, "EME_SMTPCloseNeed, no ssl");

      if (EME_UTILS_STRCMPNC(pcNewServer, pcOldServer) ||    /*server or user name or need ssl or network type of new transaction*/ 
          EME_UTILS_STRCMPNC(pcNewUser, pcOldUser) ||        /*is different from old one*/
          iOldNeedSSL != iNewNeedSSL ||
          iOldPDP != iNewPDP)
      {
         iRet = EME_FAILED;
         EME_UTILS_LOG_INFO(("EME_IMAPCloseNeed, server or user changed"));
      }

   } while(0);

   if (iRet != EME_SUCCESS)
   {
      return TRUE;
   }
   return FALSE;
}

WE_INT32 EME_StackClose(St_TMHandle *pstTmHandle, WE_UINT32 uiTransType)
{
   WE_INT32 iRet = EME_SUCCESS;

   EME_UTILS_LOG_INFO(("EME_DequeueTransaction  EME_StackClose START!!"));
   EME_UTILS_LOG_INFO(("EME_DequeueTransaction  EME_StackClose START!!"));
   
   if (pstTmHandle == NULL)
   {
   
     EME_UTILS_LOG_INFO(("EME_DequeueTransaction  EME_StackClose BADPARAM!!"));
     EME_UTILS_LOG_INFO(("EME_DequeueTransaction  EME_StackClose BADPARAM!!"));
 
      return EME_FAILED;
   }

    EME_UTILS_LOG_INFO(("EME_DequeueTransaction  EME_StackClose uiTransType = %d!!", uiTransType));
    EME_UTILS_LOG_INFO(("EME_DequeueTransaction  EME_StackClose uiTransType = %d!!",uiTransType));
 
   switch(uiTransType)
   {
   case EME_TRANS_SMTP_SEND_MAIL:
      iRet = EME_SMTP_Cancel(pstTmHandle->pvSMPT);
      pstTmHandle->iSMPTState = EME_STACK_STATE_LOGOUT;
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
      EME_POP_CancelOperation(pstTmHandle->pvPOP);
      pstTmHandle->iPOPState = EME_STACK_STATE_LOGOUT;
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
      EME_IMAP_Cancel(pstTmHandle->pvIMAP);
      pstTmHandle->iIMAPState = EME_STACK_STATE_LOGOUT;
      break;

   default:
      iRet = EME_FAILED;
      break;         
   }

    EME_UTILS_LOG_INFO(("EME_DequeueTransaction  EME_StackClose END iret  = %d!!", iRet));
    EME_UTILS_LOG_INFO(("EME_DequeueTransaction  EME_StackClose END iret = %d!!", iRet));
 
   return iRet;
}
