/*=====================================================================================
    FILE: eme_imap_parser.c
    MODULE NAME : IMAP

  
    GENERAL DESCRIPTION:
    Implement of IMAP Stack parser

    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
======================================================================================= 
    Revision History

    Modification                   Tracking
    Date              Author       Number              Description of changes
    --------          -----        ---------------     ----------------------------------
    05/15/2007        Hubin                            Change copyright
=====================================================================================*/
//pclint
/*lint -e574 */

#include "eme_file_api.h"
#include "eme_imap_parser.h"
#include "eme_imap_api.h"

static WE_INT32 EME_IMAP_FindMaxUID(St_IMAPInfo* pstIMAPInfo, WE_CHAR* pcUIDFilePath, WE_UINT32 uiNumberLimit);
static WE_BOOL EME_IMAP_UIDIsExist(St_UIDItem* pstUIDItem,WE_UINT32 uiUID);

WE_INT32 EME_IMAP_SaveResponseData(struct St_IMAPHandle* pstIMAP, WE_CHAR* pcResponse)
{
   WE_CHAR acNumOfMails[EME_IMAP_STRING_LENGTH] = {0};
   St_MailInfo stMailInfo;
   St_IMAPInfo* pstIMAPInfo = NULL;
   WE_CHAR* pcMailboxUtf7 = NULL;
   WE_CHAR* pcMailbox = NULL;
   WE_CHAR* pcUID  = NULL;
   WE_CHAR* pcSize = NULL;
   WE_INT32 iRet = EME_SUCCESS;
   WE_UINT32 uiIndex = 0;
   WE_VOID* pvMemMgr = NULL;
   St_IMAPMailboxGeted* pstIMAPMailbox = NULL;
   WE_UINT32   nIndex = 0;

   if (NULL == pcResponse)
      return EME_SUCCESS;

   pstIMAPInfo = &pstIMAP->stIMAPInfo;
   pvMemMgr = pstIMAP->stIMAPMgr.pvMemMgr;
   pstIMAPMailbox = &pstIMAP->stIMAPActRet.unRetData.stMailboxInfo;
   
   do 
   {
      if (pcResponse == EME_UTILS_StrStrNC(pcResponse, "* ")
         || NULL != pstIMAPInfo->stUIDString.pcLongString
         || NULL != pstIMAPInfo->stBodystructure.pcLongString)
      {
         /*--------------------------------------------------------------------- 
         mostly parser uid search response, first add all uid search response to 
         form a long string for finding max uid on server
          ---------------------------------------------------------------------*/
         if ((NULL != EME_UTILS_StrStrNC(pcResponse, "SEARCH")
             || NULL != pstIMAPInfo->stUIDString.pcLongString)
             && E_EME_IMAP_OPTYPE_SEARCH_UID == pstIMAPInfo->eOperationType)
         {
            iRet = EME_IMAP_AddToLongString(&pstIMAPInfo->stUIDString, pcResponse, FALSE);
            EME_UTILS_BREAKIF(iRet, "IMAP add long string error");
         }

         /*--------------------------------------------------------------------- 
         mostly parser LIST and LSUB response
          ---------------------------------------------------------------------*/
         if ((NULL != EME_UTILS_StrStrNC(pcResponse, "LIST")
            || NULL != EME_UTILS_StrStrNC(pcResponse, "LSUB"))
            && (E_EME_IMAP_OPTYPE_LIST == pstIMAPInfo->eOperationType
                || E_EME_IMAP_OPTYPE_LSUB == pstIMAPInfo->eOperationType))
         {
            WE_CHAR acTemp[EME_IMAP_MAX_STRING_LENGTH] = {0};
            WE_UINT32 uiIndex = 0;
            St_UIDItem stUIDItem;

            pstIMAPMailbox->bSelectable = TRUE;
            /* check if the mailbox selectable */
            iRet = EME_IMAP_StrGetSubString(pcResponse, '(', ')', acTemp, &uiIndex);
            EME_UTILS_BREAKIF(iRet, "IMAP parser list or lusb error");

            if (EME_UTILS_StrStrNC(acTemp, "Noselect") != NULL)
            {
               pstIMAPMailbox->bSelectable = FALSE;
            }
            EME_UTILS_MEMSET(acTemp, 0, EME_IMAP_MAX_STRING_LENGTH);
            
            /* get mailbox delimiter */
            iRet = EME_IMAP_StrGetSubString(pcResponse, '"', '"', acTemp, &uiIndex);
            EME_UTILS_BREAKIF(iRet, "IMAP parser list or lusb error");
            if (EME_UTILS_STRLEN(acTemp) <= 3 && EME_UTILS_STRLEN(acTemp) > 0)
            {
               EME_UTILS_STRCPY(pstIMAPMailbox->acDelimiter, acTemp);
            }
            EME_UTILS_MEMSET(acTemp, 0, EME_IMAP_MAX_STRING_LENGTH);

            /* get mailbox name */
            iRet = EME_IMAP_StrGetSubString(pcResponse, ' ', '\r', acTemp, &uiIndex);
            EME_UTILS_BREAKIF(iRet, "IMAP parser list or lusb error");
            if (0 != EME_UTILS_STRLEN(acTemp))
            {
               if ('"' == *acTemp)
               {
                  /* strip the double quotation marks if have */
                  pcMailboxUtf7 = acTemp+1;
                  *(acTemp+EME_UTILS_STRLEN(acTemp)-1) = 0;
               }
               else
               {
                  pcMailboxUtf7 = acTemp;
               }
            }

            pcMailbox = EME_UTILS_MALLOC(pvMemMgr, EME_UTILS_STRLEN(pcMailboxUtf7)*3+3);
            EME_UTILS_BREAKNULL(pcMailbox, iRet, EME_FAILED, "IMAP parser list or lusb error");
            EME_UTILS_MEMSET(pcMailbox, 0, EME_UTILS_STRLEN(pcMailboxUtf7)*3+3);
            
            iRet = EME_IMAP_Utf7ToUtf8(pcMailboxUtf7, pcMailbox, pvMemMgr);
            EME_UTILS_BREAKIF(iRet, "EME_IMAP_SaveResponseData EME_IMAP_Utf7ToUtf8 failed");

            /* set other results */
            pstIMAP->stIMAPActRet.iResult = EME_SUCCESS;
            pstIMAP->stIMAPActRet.eEvtCode = E_EME_IMAP_MAILBOX_NOTIFY;
            pstIMAPMailbox->pcMailboxName = pcMailbox;

            /* update uid in buffer */
            if (NULL != pcMailbox)
            {
               stUIDItem.iBeValide = TRUE;
               stUIDItem.pcMailboxName = pcMailbox;
               stUIDItem.uiMaxUID = 0;
               iRet = EME_IMAP_AddToUIDBuffer(&pstIMAP->stIMAPInfo.stUIDBuf, &stUIDItem);
               EME_UTILS_BREAKIF(iRet, "IMAP add to uid buffer failed");
            }

            /* notify result */
            if (NULL != pstIMAP->stIMAPCallback.pfnNotify
               && NULL != pcMailbox)
            {
               pstIMAP->stIMAPCallback.pfnNotify(pstIMAP->stIMAPCallback.pvClientData,
                                                 &pstIMAP->stIMAPActRet);
            }
            EME_UTILS_FREEIF(pvMemMgr, pcMailbox);
            EME_UTILS_MEMSET(&pstIMAP->stIMAPActRet, 0, sizeof(St_IMAPActRet));
         }
        

         /*--------------------------------------------------------------------- 
         mostly parser fetch body structure response, get mail's body structure
          ---------------------------------------------------------------------*/
         if ((NULL != EME_UTILS_StrStrNC(pcResponse, "BODYSTRUCTURE ")
             || NULL != pstIMAPInfo->stBodystructure.pcLongString)
             && E_EME_IMAP_OPTYPE_FETCH_BODYSTRUCTURE == pstIMAPInfo->eOperationType)
         {
            iRet = EME_IMAP_AddToLongString(&pstIMAPInfo->stBodystructure, pcResponse, FALSE);
            EME_UTILS_BREAKIF(iRet, "IMAP add long string error");
         }

         /*--------------------------------------------------------------------- 
            mostly parser select response, find mail number of the selected mailbox
          ---------------------------------------------------------------------*/
         if (NULL != EME_UTILS_StrStrNC(pcResponse, "EXISTS")
             && E_EME_IMAP_OPTYPE_SELECT_MAILBOX == pstIMAPInfo->eOperationType)
         {
            uiIndex = 0;
            if (SUCCESS == EME_IMAP_StrGetSubString(pcResponse,' ', ' ', acNumOfMails, &uiIndex))
            {
               pstIMAPInfo->NumOfMails = EME_UTILS_STRTOUL(acNumOfMails, NULL, 10);
            }
         }

         /*--------------------------------------------------------------------- 
            mostly parser fetch size response, find size of new mails
          ---------------------------------------------------------------------*/
         pcUID  = EME_UTILS_StrStrNC(pcResponse, "UID");
         pcSize = EME_UTILS_StrStrNC(pcResponse, "RFC822.SIZE");
         if (NULL != pcUID && NULL != pcSize
             && E_EME_IMAP_OPTYPE_FETCH_SIZE == pstIMAPInfo->eOperationType)
         {
            stMailInfo.uiUID = EME_UTILS_STRTOUL(pcUID+EME_UTILS_STRLEN("UID"), NULL, 10);
            stMailInfo.uiSize = EME_UTILS_STRTOUL(pcSize+EME_UTILS_STRLEN("RFC822.SIZE"), NULL, 10);

            for (nIndex = 0; nIndex < (pstIMAPInfo->stUIDBuf.uiItemNum);nIndex++)
            {
               if (FALSE == EME_IMAP_UIDIsExist(pstIMAPInfo->stUIDBuf.ppstUIDItem[nIndex],stMailInfo.uiUID))
               {
                  iRet = EME_IMAP_AddToMailInfoArray(&pstIMAPInfo->stMailInfoArray, stMailInfo,
                     pstIMAP->stIMAPAccountInfo.uiNumberLimit,
                     pstIMAP->stIMAPAccountInfo.iLast);
                  EME_UTILS_BREAKIF(iRet, "IMAP EME_AddToMailInfoArray failed");
               }
            }
         }
      }

      /*--------------------------------------------------------------------- 
        if response is end of bodystructure or uid string, parser them to get data
       ---------------------------------------------------------------------*/
      /* if uid search response end, find the max uid on server */
      if (NULL != pstIMAPInfo->stUIDString.pcLongString
          && NULL != EME_UTILS_STRCHR(pstIMAPInfo->stUIDString.pcLongString, '\n')
          && E_EME_IMAP_OPTYPE_SEARCH_UID == pstIMAPInfo->eOperationType)
      {
         iRet = EME_IMAP_FindMaxUID(pstIMAPInfo, 
                                    pstIMAPInfo->pcUIDFilePath, 
                                    pstIMAP->stIMAPAccountInfo.uiNumberLimit);
         EME_UTILS_BREAKIF(iRet, "IMAP EME_IMAP_FindMaxUID error");
         
         /* free the uid string */
         {
            WE_CHAR* pcTemp = EME_IMAP_GetLongString(&pstIMAPInfo->stUIDString);
            EME_UTILS_FREEIF(pstIMAPInfo->stUIDString.pMemHdr, pcTemp);
         }
      }

      /* if uid search response end, get the bodystructure data */
      if(NULL != pstIMAPInfo->stBodystructure.pcLongString)
      {
         WE_INT32 iSize = EME_UTILS_STRLEN(pstIMAPInfo->stBodystructure.pcLongString);
         WE_CHAR * ptemp =  EME_UTILS_STRCHR(pstIMAPInfo->stBodystructure.pcLongString, '\n');
         if (NULL != pstIMAPInfo->stBodystructure.pcLongString
             && NULL != ptemp
             && iSize > (WE_INT32)EME_UTILS_STRLEN(" BODYSTRUCTURE")
               && E_EME_IMAP_OPTYPE_FETCH_BODYSTRUCTURE == pstIMAPInfo->eOperationType)
            {
               /* useful bodystructure is between BODYSTRUCTURE and RFC822.HEADER */
               WE_CHAR* pcStart = EME_UTILS_STRSTR(pstIMAPInfo->stBodystructure.pcLongString, "BODYSTRUCTURE ");

	        
               if (NULL != pcStart)
               {
                  pcStart += EME_UTILS_STRLEN(" BODYSTRUCTURE"); /* let it point after " BODYSTRUCTURE" */
                  iRet = EME_IMAP_AddToLongString(&pstIMAPInfo->stBodystructure, pcStart, TRUE);
                  EME_UTILS_BREAKIF(iRet, "IMAP add long string error");
        
                  /* pstIMAPInfo->stBodystructure will contain nothing after EME_GetLongString */
                  {
                     WE_CHAR* pcTemp = EME_IMAP_GetLongString(&pstIMAPInfo->stBodystructure);
                     EME_UTILS_FREEIF(pstIMAPInfo->stBodystructure.pMemHdr, pcTemp);
                  }

               }
            }
      }
   } while(0);
  
   return iRet;
}


WE_INT32 EME_IMAP_SaveMailData(struct St_IMAPHandle* pstIMAP, WE_CHAR* pcResponse)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_IMAPInfo* pstIMAPInfo = NULL;
   if (NULL == pcResponse || '\0' == *pcResponse)
      return EME_SUCCESS;
   
   pstIMAPInfo = &pstIMAP->stIMAPInfo;

   do 
   {
      /*----------------------------------------------------------------------- 
         if the response is not uid string and not bodystructure and not other 
         untagged response(which prefixed with "*"), it is the mail data. 
       -----------------------------------------------------------------------*/
      if (NULL == pstIMAPInfo->stUIDString.pcLongString /* ensure data is not uid string */
           && NULL == pstIMAPInfo->stBodystructure.pcLongString /* ensure data is not bodystructure string */
           && pcResponse != EME_UTILS_StrStrNC(pcResponse, "*")) /* ensure data is not untagged response */
      {
         St_IMAPTransferNotify* pstIMAPNotify;
         EME_UTILS_MEMSET(&pstIMAP->stIMAPActRet, 0, sizeof(St_IMAPActRet));
         pstIMAPNotify = &pstIMAP->stIMAPActRet.unRetData.stTransferNotify;

         /* add response to right place */
         switch(pstIMAPInfo->eOperationType)
         {
         case E_EME_IMAP_OPTYPE_FETCH_HEADER:
            {
               if(EME_UTILS_STRCMP(pcResponse, ")\r\n") == 0)
                  break;

               iRet = EME_IMAP_AddToLongString(&pstIMAPInfo->stHeader, pcResponse, TRUE);
               EME_UTILS_BREAKIF(iRet, "IMAP add long string error");
               break;
            }
         case E_EME_IMAP_OPTYPE_FETCH_MAIL:
            {
                 /* if have not enough memory for containing mail, realloc(in function EME_IMAP_StringCat) 
                  else strcat directly*/
               if(EME_UTILS_STRCMP(pcResponse, ")\r\n") == 0)
                  break;

               if(pstIMAPInfo->uiUsedBufSize + EME_UTILS_STRLEN(pcResponse) > EME_IMAP_BUFFER_CATCH_SIZE)
               {
                  EME_FileWrite(pstIMAPInfo->pvTmpFile, pstIMAPInfo->pvCatchBuf, pstIMAPInfo->uiUsedBufSize);
                  EME_UTILS_MEMSET(pstIMAPInfo->pvCatchBuf, 0, EME_IMAP_BUFFER_CATCH_SIZE + 1);
                  pstIMAPInfo->uiUsedBufSize = 0;

                  EME_UTILS_MEMCPY((WE_CHAR*)pstIMAPInfo->pvCatchBuf,
                     pcResponse,
                     EME_UTILS_STRLEN(pcResponse));
                  pstIMAPInfo->uiUsedBufSize += EME_UTILS_STRLEN(pcResponse);
                  
               }
               else
               {
                  EME_UTILS_MEMCPY((WE_CHAR*)pstIMAPInfo->pvCatchBuf + pstIMAPInfo->uiUsedBufSize,
                     pcResponse,
                     EME_UTILS_STRLEN(pcResponse));
                  pstIMAPInfo->uiUsedBufSize += EME_UTILS_STRLEN(pcResponse);
               }
               break;
            }
         case E_EME_IMAP_OPTYPE_FETCH_SPECIFIC_MAIL:
            {
               if(EME_UTILS_STRCMP(pcResponse, ")\r\n") == 0)
                  break;

               if(pstIMAPInfo->uiUsedBufSize + EME_UTILS_STRLEN(pcResponse) > EME_IMAP_BUFFER_CATCH_SIZE)
               {
                  EME_FileWrite(pstIMAPInfo->pvTmpFile, pstIMAPInfo->pvCatchBuf, pstIMAPInfo->uiUsedBufSize);
                  EME_UTILS_MEMSET(pstIMAPInfo->pvCatchBuf, 0, EME_IMAP_BUFFER_CATCH_SIZE + 1);
                  pstIMAPInfo->uiUsedBufSize = 0;

                  EME_UTILS_MEMCPY((WE_CHAR*)pstIMAPInfo->pvCatchBuf,
                     pcResponse,
                     EME_UTILS_STRLEN(pcResponse));
                  pstIMAPInfo->uiUsedBufSize += EME_UTILS_STRLEN(pcResponse);
                  
               }
               else
               {
                  EME_UTILS_MEMCPY((WE_CHAR*)pstIMAPInfo->pvCatchBuf + pstIMAPInfo->uiUsedBufSize,
                     pcResponse,
                     EME_UTILS_STRLEN(pcResponse));
                  pstIMAPInfo->uiUsedBufSize += EME_UTILS_STRLEN(pcResponse);
               }
               break;
            }
         case E_EME_IMAP_OPTYPE_FETCH_MAILPART:
            {
               if(EME_UTILS_STRCMP(pcResponse, ")\r\n") == 0)
                  break;

               if(pstIMAPInfo->uiUsedBufSize + EME_UTILS_STRLEN(pcResponse) > EME_IMAP_BUFFER_CATCH_SIZE)
               {
                  EME_FileWrite(pstIMAPInfo->pvTmpFile, pstIMAPInfo->pvCatchBuf, pstIMAPInfo->uiUsedBufSize);
                  EME_UTILS_MEMSET(pstIMAPInfo->pvCatchBuf, 0, EME_IMAP_BUFFER_CATCH_SIZE + 1);
                  pstIMAPInfo->uiUsedBufSize = 0;

                  EME_UTILS_MEMCPY((WE_CHAR*)pstIMAPInfo->pvCatchBuf,
                     pcResponse,
                     EME_UTILS_STRLEN(pcResponse));
                  pstIMAPInfo->uiUsedBufSize += EME_UTILS_STRLEN(pcResponse);
                  
               }
               else
               {
                  EME_UTILS_MEMCPY((WE_CHAR*)pstIMAPInfo->pvCatchBuf + pstIMAPInfo->uiUsedBufSize,
                     pcResponse,
                     EME_UTILS_STRLEN(pcResponse));
                  pstIMAPInfo->uiUsedBufSize += EME_UTILS_STRLEN(pcResponse);
               }
               break;
            }
         default:
             break;
         }
         EME_UTILS_BREAKIF(iRet, "imap fetch data error");

         pstIMAPInfo->uiCurrentSize += EME_UTILS_STRLEN(pcResponse);
          /* notify transfer data if not downloading header */
         if (E_EME_IMAP_OPTYPE_FETCH_HEADER != pstIMAPInfo->eOperationType)
         {
            pstIMAPNotify->uiTotalSize = pstIMAPInfo->uiTotalSize;
            pstIMAPNotify->uiTransferedSize = pstIMAPInfo->uiCurrentSize;
            pstIMAP->stIMAPActRet.eEvtCode = E_EME_IMAP_TRANSFER_SIZE_NOTIFY;
            
            if (pstIMAPNotify->uiTransferedSize > pstIMAPNotify->uiTotalSize)
            {
               pstIMAPNotify->uiTransferedSize = pstIMAPNotify->uiTotalSize;
            }

            if (NULL != pstIMAP->stIMAPCallback.pfnNotify)
            {
               pstIMAP->stIMAPCallback.pfnNotify(pstIMAP->stIMAPCallback.pvClientData,
                  &pstIMAP->stIMAPActRet);
            }
            EME_UTILS_MEMSET(&pstIMAP->stIMAPActRet, 0, sizeof(St_IMAPActRet));
         }
      }

   } while(0);

   if (EME_SUCCESS != iRet)
   {
      return EME_FAILED;
   }

   return EME_SUCCESS;
}


static WE_INT32 EME_IMAP_FindMaxUID(St_IMAPInfo* pstIMAPInfo, WE_CHAR* pcUIDFilePath, WE_UINT32 uiNumberLimit)
{
   WE_INT32 iRet = EME_SUCCESS;
   WE_INT32 iResult = EME_SUCCESS;
   WE_UINT32 uiIndex = 0;
   WE_CHAR  acTempUIDString[EME_IMAP_STRING_LENGTH] = {0};
   if (NULL == pstIMAPInfo)
      return EME_INVALID_DATA;
   
   do
   {
      /*----------------------------------------------------------------------- 
                           get max uid recorded 
       -----------------------------------------------------------------------*/
      iRet = EME_IMAP_ReadUIDFileToBuffer(&pstIMAPInfo->stUIDBuf, pcUIDFilePath);
      EME_UTILS_BREAKIF(iRet, "IMAP read uid file error");

      iResult = EME_IMAP_GetMaxUID(&pstIMAPInfo->stUIDBuf, 
                              pstIMAPInfo->pcMailboxName, 
                              &pstIMAPInfo->uiMaxSavedUID);

      if (EME_INVALID_DATA == iResult)
      {
         iRet = EME_INVALID_DATA;
         break;
      }
      else if (EME_FAILED == iResult)
      {
         /* if mailbox not recorded, record mailbox and the max uid as 0
            (0 means has not download any mail yet)¡¡*/
         St_UIDItem pstUIDItem;
         pstUIDItem.uiMaxUID = pstIMAPInfo->uiMaxSavedUID = 0;
         pstUIDItem.pcMailboxName = pstIMAPInfo->pcMailboxName;
         
         iRet = EME_IMAP_AddToUIDBuffer(&pstIMAPInfo->stUIDBuf, &pstUIDItem);
         EME_UTILS_BREAKIF(iRet, "imap write uid to buffer failed");
      }

      pstIMAPInfo->uiMaxSavedUID = 0;
      
      /*----------------------------------------------------------------------- 
                           find the max uid on server 
       -----------------------------------------------------------------------*/
      uiIndex = 7; /* 7 is strlen("* SEARCH")-1, let it point to fisrt uid */
      /* every loop will get a uid, but not save them at now */
      while (EME_SUCCESS == EME_IMAP_StrGetSubString(pstIMAPInfo->stUIDString.pcLongString, 
                                                ' ', 
                                                ' ',
                                                acTempUIDString,
                                                &uiIndex))
      {
         /* save it only temporary, it will replace by the last uid blow(the largest one) */
         pstIMAPInfo->uiMaxDownloadUID = EME_UTILS_STRTOUL(acTempUIDString, NULL, 10);
         uiIndex--; /* back to left of ' ' for avoiding escape uid, 
                       see description of EME_StrGetSubString */

         if (pstIMAPInfo->uiMaxDownloadUID > pstIMAPInfo->uiMaxSavedUID)
         {
//            uiNumberLimit--;
//            if (0 == uiNumberLimit)
//               break;            
         }
         else
         {
            pstIMAPInfo->NumOfMails--;
         }
      }
      if (0 == uiNumberLimit)
         break;

      /* get the max uid on server and save it */
      if (EME_SUCCESS == EME_IMAP_StrGetSubString(pstIMAPInfo->stUIDString.pcLongString, 
                                             ' ', 
                                             '\r',
                                             acTempUIDString,
                                             &uiIndex))
      {            
         pstIMAPInfo->uiMaxDownloadUID = EME_UTILS_STRTOUL(acTempUIDString, NULL, 10);
      }
      
   } while(0);

   return iRet;
}


WE_INT32 EME_IMAP_StripLastLine(WE_CHAR* pcString)
{
   WE_UINT32 uiLength = 0;
   WE_CHAR* pcLast = NULL;
   if (NULL == pcString || '\0' == *pcString)
      return EME_INVALID_DATA;

   uiLength = EME_UTILS_STRLEN(pcString);
   pcLast = pcString+uiLength-1;

   /* if the last is not a line, treat it as success */
   if ('\n' != *pcLast)
      return EME_SUCCESS;
   
   while ('\n' != *(--pcLast) && pcLast >= pcString)
      /*do nothing*/;
   
   if (pcString > pcLast)
      return EME_FAILED;
   
   pcLast++;
   *pcLast = 0;

   return EME_SUCCESS;
}

static WE_BOOL EME_IMAP_UIDIsExist(St_UIDItem* pstUIDItem,WE_UINT32 uiUID)
{
	int index = 0;
	int iRet  = EME_SUCCESS;

	for(index= 0;index< (pstUIDItem->nCurrNum);index++)
	{
		if (uiUID == pstUIDItem->nCurrList[index].uiUID)
		{
			return TRUE;
		}
	}
	return FALSE;
}

