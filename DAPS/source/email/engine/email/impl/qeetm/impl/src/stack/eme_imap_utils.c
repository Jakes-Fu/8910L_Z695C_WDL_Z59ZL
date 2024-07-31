/*=====================================================================================
    FILE: eme_imap_uitls.c
    MODULE NAME : IMAP

  
    GENERAL DESCRIPTION:
    Implement of IMAP Stack utils

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
#include "eme_imap_utils.h"
#include "eme_encode_base64.h"

/* used to find if pcStr1 is the same as pcStr2, ignore case.
   note: it is ignore case, so if parameter is STRING and String, it will 
         return true */
static WE_BOOL EME_IMAP_IsStrEqualIC(WE_CHAR *pcStr1, WE_CHAR *pcStr2);

static WE_BOOL EME_IMAP_SearhUID(St_UIDItem*pstUIDItem,WE_UINT32 uiUID)
{
	int index = 0;
	int iRet  = EME_SUCCESS;
	do 
	{

		for(index= 0;index< (pstUIDItem->nCurrNum);index++)
		{
			if (uiUID == pstUIDItem->nCurrList[index].uiUID)
			{
				return TRUE;
			}
		}
		if (0 == pstUIDItem->nCurrNum % EME_UIDFILE_ENLARGE_NUMBER)
		{
			iRet = EME_IMAP_ReallocMemory((void**)&pstUIDItem->nCurrList,
				(pstUIDItem->nCurrNum+10)* sizeof(St_MailInfo),
				NULL);
			EME_UTILS_BREAKIF(iRet, "pstUIDItem->nCurrList imap no memory error");
			EME_UTILS_BREAKNULL(pstUIDItem->nCurrList, iRet, EME_NOMEMORY, "pstUIDItem->nCurrList malloc error");
		}
		pstUIDItem->nCurrList[pstUIDItem->nCurrNum].uiUID = uiUID;
		pstUIDItem->nCurrNum ++;
	} while (0);
	if (EME_SUCCESS == iRet)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

WE_INT32 EME_IMAP_AddToUIDBuffer(St_UIDBuf* pstUIDBuf, St_UIDItem* pstUIDItem)
{
   WE_INT32 iRet = EME_SUCCESS;
   WE_UINT32 uiIndex = 0;
   St_UIDItem* pstUIDItemTemp = NULL;
   
   if (NULL == pstUIDBuf || NULL == pstUIDItem || NULL == pstUIDItem->pcMailboxName)
      return EME_INVALID_DATA;
   
   do
   {
      /* if no memory left for add new item, realloc */
      if (0 == pstUIDBuf->uiItemNum % EME_UIDFILE_ENLARGE_NUMBER)
      {
         iRet = EME_IMAP_ReallocMemory((void**)&pstUIDBuf->ppstUIDItem,
            (pstUIDBuf->uiItemNum+EME_UIDFILE_ENLARGE_NUMBER)* sizeof(WE_UINT32),
            pstUIDBuf->pMemHdr);
         EME_UTILS_BREAKIF(iRet, "imap no memory error");
         EME_UTILS_BREAKNULL(pstUIDBuf->ppstUIDItem, iRet, EME_NOMEMORY, "pstUIDBuf malloc error");
      }
      
      /* find the pstUIDItem in pstUIDBuf */
      while (uiIndex < pstUIDBuf->uiItemNum)
      {
         if (TRUE != EME_IMAP_IsStrEqualIC(pstUIDBuf->ppstUIDItem[uiIndex]->pcMailboxName, 
            pstUIDItem->pcMailboxName))
         {
            /* check next item */
            uiIndex++;
            continue;
         }
         else /* the specfic item found, change uid and return success */
         {
            /* set max uid in the mailbox */
            pstUIDBuf->ppstUIDItem[uiIndex]->uiMaxUID = 
               EME_MAX(pstUIDBuf->ppstUIDItem[uiIndex]->uiMaxUID, pstUIDItem->uiMaxUID);
            pstUIDBuf->ppstUIDItem[uiIndex]->iBeValide = TRUE;

            if (0 == pstUIDItem->uiMaxUID)
            {
               return EME_SUCCESS;
            }
            if (TRUE == EME_IMAP_SearhUID(pstUIDBuf->ppstUIDItem[uiIndex],pstUIDItem->uiMaxUID))
            {
               return EME_SUCCESS;
            }
            else
            {
               return EME_FAILED;
            }
         }
      }
      
      /************************************************************************ 
      pstUIDItem not found in pstUIDBuf, add pstUIDItem as a new item 
      *************************************************************************/
      /* construct a pstUIDItemTemp, and initialize it with input pstUIDItem */
      pstUIDItemTemp = EME_UTILS_MALLOC(pstUIDBuf->pMemHdr, sizeof(St_UIDItem));
      EME_UTILS_BREAKNULL(pstUIDItemTemp, iRet, EME_NOMEMORY, "pstUIDBuf malloc error");
      EME_UTILS_MEMSET(pstUIDItemTemp, 0, sizeof(St_UIDItem));
      
      pstUIDItemTemp->pcMailboxName = EME_UTILS_STRDUP(pstUIDBuf->pMemHdr, pstUIDItem->pcMailboxName);
      EME_UTILS_BREAKNULL(pstUIDItemTemp->pcMailboxName, iRet, EME_NOMEMORY, "pstUIDBuf malloc error");
      pstUIDItemTemp->uiMaxUID = pstUIDItem->uiMaxUID;
      pstUIDItemTemp->iBeValide = TRUE;
      
      ////////Freddy.wu 2010.02.25
      pstUIDItemTemp->nCurrList = EME_UTILS_MALLOC(pstUIDBuf->pMemHdr, EME_UIDFILE_ENLARGE_NUMBER * sizeof(St_MailInfo));
      EME_UTILS_BREAKNULL(pstUIDItemTemp->nCurrList, iRet, EME_NOMEMORY, "pstUIDItemTemp->nCurrList malloc error");
      pstUIDItemTemp->nCurrList[pstUIDItemTemp->nCurrNum].uiUID = pstUIDItem->uiMaxUID;
      if (0 != pstUIDItem->uiMaxUID)
      {
         pstUIDItemTemp->nCurrNum ++;
      }
      
      /* add pstUIDItemTemp to pstUIDBuf->ppstUIDItem as a new item */
      pstUIDBuf->ppstUIDItem[pstUIDBuf->uiItemNum] = pstUIDItemTemp;
      pstUIDBuf->uiItemNum++;
   } while(0);
   
   if (EME_SUCCESS != iRet)
   {
      if (NULL != pstUIDItemTemp)
      {
         EME_UTILS_FREEIF(pstUIDBuf->pMemHdr, pstUIDItemTemp->pcMailboxName);
         EME_UTILS_FREEIF(pstUIDBuf->pMemHdr,pstUIDItemTemp->nCurrList);
         EME_UTILS_FREEIF(pstUIDBuf->pMemHdr, pstUIDItemTemp);
      }
      return iRet;
   }
   return EME_SUCCESS;
}



WE_INT32 EME_IMAP_ReadUIDFileToBuffer(St_UIDBuf* pstUIDBuf, WE_CHAR* pcUIDFilePath)
{
   WE_UINT32 uiSize = 0; /* file size */
   St_UIDItem pstUIDItem;
   WE_INT32 iRet = EME_SUCCESS;
   WE_CHAR* pcBuf = NULL;
   FILE_HANDLE hFile = NULL;
   WE_UINT32 uiBytes = 0;
   WE_CHAR* pLine = NULL;
   WE_BOOL  bExist = FALSE;
   int   index = 0;
   
   if (NULL == pstUIDBuf|| NULL == pcUIDFilePath)
      return EME_INVALID_DATA;
   if (0 == EME_UTILS_STRLEN(pcUIDFilePath))
      return EME_INVALID_DATA;
   
   do 
   {
   /***********************************************************************
   first, read formated data from file to memory at a time. Format in file
   is like this: inbox\n
   56\n
   draft\n
   2\n
   then, get mailboxs and uids infomation and put them into St_UIDBuf
      ************************************************************************/
      bExist = EME_FileExists(pstUIDBuf->pFileMgr, pcUIDFilePath);
      if(!bExist)
      {
         hFile = EME_FileOpen(pstUIDBuf->pFileMgr, pcUIDFilePath, EME_OFM_CREATE);
      }
      else
      {
         hFile = EME_FileOpen(pstUIDBuf->pFileMgr, pcUIDFilePath, EME_OFM_READWRITE);
      }
      EME_UTILS_BREAKNULL(hFile, iRet, EME_FILE_ERROR, "file open error");

      /* calculate buffer size needed for containing all mailbox name and uid strings */
      uiSize = EME_FileLength(hFile);
      
      /* use uid file first time */
      if (0 == uiSize)
         break;
      
      uiSize += 1; /* +1 means '\0' */
      
      /* allocate buffer for containing all mailbox name and uid strings */
      pcBuf = EME_UTILS_MALLOC(pstUIDBuf->pMemHdr, uiSize);
      EME_UTILS_BREAKNULL(pcBuf, iRet, EME_FAILED, "mailbox name is NULL, error");
      EME_UTILS_MEMSET(pcBuf, 0, uiSize);
      /* read all mailboxs and uids infomation from file at a time*/
      uiBytes = EME_FileRead(hFile, pcBuf, uiSize-1);
      if (uiSize-1 != uiBytes)
      {
         iRet = EME_FILE_ERROR;
         EME_UTILS_BREAK(EME_FILE_ERROR, "file read error");
      }
      /* check whether this file is right uid file */
      if (EME_UTILS_STRSTR(pcBuf, EME_UIDFILE_FLAG) != pcBuf)
      {
         EME_FileTruncate(hFile, 0);
         break;
//         iRet = EME_FILE_ERROR;
//         EME_UTILS_BREAK(EME_FILE_ERROR, "file corrupted, error");
      }
      
      /* find first line */
      pLine = EME_UTILS_STRCHR(pcBuf, '\n');
      pLine = (pLine == NULL) ? NULL : (pLine+1);
      
      /* if there exists mailboxs and uids, put them into St_UIDBuf  */
      while (NULL != pLine && *pLine)
      {
         /* every loop will add a uid item to pstUIDBuf */
         pstUIDItem.pcMailboxName = pLine; /* point to mailbox name in pcBuf */
         
         /* find next line, next line should be uid string */
         pLine = EME_UTILS_STRCHR(pLine, '\n');
         *pLine = '\0';/* set '\n' zero to let pstUIDItem->pcMailboxName form a string */
         pLine++;
         
         pstUIDItem.nCurrNum = EME_UTILS_STRTOUL(pLine, NULL, 10);
         pLine = EME_UTILS_STRCHR(pLine, '\n');
         pLine++;
         for (index = 0 ; index < (pstUIDItem.nCurrNum) && NULL != pLine; index++)
         {
            pstUIDItem.uiMaxUID = EME_UTILS_STRTOUL(pLine, NULL, 10);
            pstUIDItem.iBeValide = TRUE;
            
            /* add UIDItem to pstUIDBuf */
            iRet = EME_IMAP_AddToUIDBuffer(pstUIDBuf, &pstUIDItem);
            EME_UTILS_BREAKIF(iRet, "EME_AddToUIDBuffer error");
            
            /* find next line, next line should be mailbox name string */
            pLine = EME_UTILS_STRCHR(pLine, '\n');
            pLine = (pLine == NULL) ? pLine : (pLine+1);
         }
      }
      /* EME_UTILS_BREAKIF can not break to end of function, so add below */
      //  if (NULL != *pLine) //?
      if (NULL != pLine && '\0' != *pLine)
      {
         EME_UTILS_BREAKIF(iRet, "EME_AddToUIDBuffer error");
      }
      
   } while(0);
   
   /* whether error occured or not, below will execute */
   EME_UTILS_FREEIF(pstUIDBuf->pMemHdr, pcBuf);
   EME_FileClose(hFile);
   
   if (EME_SUCCESS != iRet)
   {
      EME_IMAP_FreeUIDBuf(pstUIDBuf);
      return iRet;
   }
   return EME_SUCCESS;
}


/* used when list mailboxs */
WE_INT32 EME_IMAP_InvalideAll(St_UIDBuf* pstUIDBuf)
{
   WE_UINT32 uiIndex = 0;
   
   if (NULL == pstUIDBuf)
      return EME_INVALID_DATA;
   
   while (uiIndex < pstUIDBuf->uiItemNum)
   {
      pstUIDBuf->ppstUIDItem[uiIndex]->iBeValide = FALSE;
      uiIndex++;
   }
   
   return EME_SUCCESS;
}

/* used when delete mailbox */
WE_INT32 EME_IMAP_InvalideMailbox(St_UIDBuf* pstUIDBuf, WE_CHAR* pcMailboxName)
{
   WE_UINT32 uiIndex = 0;
   
   if (NULL == pstUIDBuf || NULL == pcMailboxName)
      return EME_INVALID_DATA;
   
   /* find the pcMailboxName in pstUIDBuf */
   while (uiIndex < pstUIDBuf->uiItemNum)
   {
      if (TRUE != EME_IMAP_IsStrEqualIC(pstUIDBuf->ppstUIDItem[uiIndex]->pcMailboxName, 
         pcMailboxName))
      {
         /* check next item */
         uiIndex++;
         continue;
      }
      else /* the specfic item found, change uid and return success */
      {
         /* set invalide */
         pstUIDBuf->ppstUIDItem[uiIndex]->iBeValide = FALSE;
         return EME_SUCCESS;
      }
   }
   
   return EME_FAILED;
}

/* used when rename mailbox */
WE_INT32 EME_IMAP_RenameMailbox(St_UIDBuf* pstUIDBuf, WE_CHAR* pcOrginName, WE_CHAR* pcDestName)
{
   WE_UINT32 uiIndex = 0;
   
   if (NULL == pstUIDBuf || NULL == pcOrginName)
      return EME_INVALID_DATA;
   
   /* find the pcMailboxName in pstUIDBuf */
   while (uiIndex < pstUIDBuf->uiItemNum)
   {
      if (TRUE != EME_IMAP_IsStrEqualIC(pstUIDBuf->ppstUIDItem[uiIndex]->pcMailboxName, 
         pcOrginName))
      {
         /* check next item */
         uiIndex++;
         continue;
      }
      else /* the specfic item found, change uid and return success */
      {
         /* rename mailbox */
         EME_UTILS_FREEIF(pstUIDBuf->pMemHdr, pstUIDBuf->ppstUIDItem[uiIndex]->pcMailboxName);
         pstUIDBuf->ppstUIDItem[uiIndex]->pcMailboxName = EME_UTILS_STRDUP(pstUIDBuf->pMemHdr, pcDestName);
         return EME_SUCCESS;
      }
   }
   
   return EME_FAILED;
}


WE_INT32 EME_IMAP_WriteUIDBufferToFile(St_UIDBuf* pstUIDBuf, WE_CHAR* pcUIDFilePath)
{
   WE_UINT32 uiSize = 0; /* file size */
   WE_INT32 uiNum = 0;
   WE_INT32 iRet = EME_SUCCESS;
   WE_CHAR* pcBuf = NULL; 
   FILE_HANDLE hFile = NULL;
   WE_UINT32 uiBytes = 0;
   WE_UINT32  uiNumTemp = 0;

   if (NULL == pstUIDBuf|| NULL == pcUIDFilePath)
      return EME_INVALID_DATA;
   if (0 == EME_UTILS_STRLEN(pcUIDFilePath))
      return EME_INVALID_DATA;
   
   uiNum = pstUIDBuf->uiItemNum;

   do 
   {
      /***********************************************************************
         first, write to memory according format in file, then write to file at
         a time. Format in file is like this: inbox\n
                                              56\n
                                              draft\n
                                              2\n
      ************************************************************************/
      /* calculate buffer size needed for containing all mailbox name and uid strings */
      while (uiNum--)
      {
         EME_UTILS_BREAKNULL(pstUIDBuf->ppstUIDItem[uiNum]->pcMailboxName, 
                             iRet, EME_FAILED, "mailbox name is NULL, error");
         
         if (TRUE == pstUIDBuf->ppstUIDItem[uiNum]->iBeValide)
         {
            /* +1 means '\n' */
            uiSize += EME_UTILS_STRLEN(pstUIDBuf->ppstUIDItem[uiNum]->pcMailboxName)+1; 
            /* +1 means the max WE_INT32 string length and '\n' */
            uiSize += pstUIDBuf->ppstUIDItem[uiNum]->nCurrNum * 11;
            uiSize += 12;
         }
      }
      /* EME_UTILS_BREAKNULL can not break to end of function, so add below */
      if (uiNum >= 0)
      {
         EME_UTILS_BREAK(EME_FAILED, "mailbox name is NULL, error");
      }
      
      uiSize += (1 + EME_UTILS_STRLEN(EME_UIDFILE_FLAG)); /* +1 means '\0'*/

      /* allocate buffer for containing all mailbox name and uid strings */
      pcBuf = EME_UTILS_MALLOC(pstUIDBuf->pMemHdr, uiSize);
      EME_UTILS_BREAKNULL(pcBuf, iRet, EME_FAILED, "mailbox name is NULL, error");
      EME_UTILS_MEMSET(pcBuf, 0, uiSize);
      /* add flag to judge whether file is corrupted */
      EME_UTILS_STRCAT(pcBuf, "TFMAIL_UID_FILE\n");
      /* add mailboxs and uids infomation to buffer */
      uiNum = pstUIDBuf->uiItemNum;
//      while (uiNum--)
//      {
//         if (TRUE == pstUIDBuf->ppstUIDItem[uiNum]->iBeValide)
//         {
//            EME_UTILS_SPRINTF(pcBuf+EME_UTILS_STRLEN(pcBuf), "%s\n%d\n", 
//            pstUIDBuf->ppstUIDItem[uiNum]->pcMailboxName, pstUIDBuf->ppstUIDItem[uiNum]->uiMaxUID);
//         }
//      }

      while (uiNum--)
	  {
		  if (TRUE == pstUIDBuf->ppstUIDItem[uiNum]->iBeValide)
		  {
			  EME_UTILS_SPRINTF(pcBuf+EME_UTILS_STRLEN(pcBuf), "%s\n%d\n", 
				  pstUIDBuf->ppstUIDItem[uiNum]->pcMailboxName, pstUIDBuf->ppstUIDItem[uiNum]->nCurrNum);
			  uiNumTemp = pstUIDBuf->ppstUIDItem[uiNum]->nCurrNum;
			  while(uiNumTemp--)
			  {
				  EME_UTILS_SPRINTF(pcBuf+EME_UTILS_STRLEN(pcBuf), "%d\n", 
					  pstUIDBuf->ppstUIDItem[uiNum]->nCurrList[uiNumTemp].uiUID);
			  }
		  }
	  }
      
      /* write all mailboxs and uids infomation to file at a time*/
      hFile = EME_FileOpen(pstUIDBuf->pFileMgr, pcUIDFilePath, EME_OFM_READWRITE);
      EME_UTILS_BREAKNULL(hFile, iRet, EME_FILE_ERROR, "file open error");
      
      EME_FileTruncate(hFile, 0);
      uiBytes = EME_FileWrite(hFile, pcBuf, EME_UTILS_STRLEN(pcBuf));
      if ((WE_UINT32)EME_UTILS_STRLEN(pcBuf) != uiBytes)
      {
         iRet = EME_FILE_ERROR;
         EME_UTILS_BREAK(EME_FILE_ERROR, "file write error");
      }

   } while(0);
   
   /* whether error occured or not, below will execute */
   EME_UTILS_FREEIF(pstUIDBuf->pMemHdr, pcBuf);
   EME_FileClose(hFile);

   if (EME_SUCCESS != iRet)
   {
      if (NULL != pcBuf)
         EME_UTILS_FREEIF(pstUIDBuf->pMemHdr, pcBuf);
      return iRet;
   }
   return EME_SUCCESS;
}

WE_INT32 EME_IMAP_GetMaxUID(St_UIDBuf* pstUIDBuf, WE_CHAR* pcMailboxName, WE_UINT32* uiMaxUID)
{
   WE_UINT32 uiIndex = 0;

   if (NULL == pstUIDBuf || NULL == pcMailboxName || NULL == uiMaxUID)
      return EME_INVALID_DATA;

   /* find the pcMailboxName in pstUIDBuf */
   while (uiIndex < pstUIDBuf->uiItemNum)
   {
      if (TRUE != EME_IMAP_IsStrEqualIC(pstUIDBuf->ppstUIDItem[uiIndex]->pcMailboxName, 
                                   pcMailboxName))
      {
         /* check next item */
         uiIndex++;
         continue;
      }
      else /* the specfic item found, change uid and return success */
      {
         /* get max uid */
         *uiMaxUID = pstUIDBuf->ppstUIDItem[uiIndex]->uiMaxUID;
         return EME_SUCCESS;
      }
   }

   return EME_FAILED;
}

WE_INT32 EME_IMAP_FreeUIDBuf(St_UIDBuf* pstUIDBuf)
{
   if (NULL == pstUIDBuf)
      return EME_SUCCESS;
   
   /* free all UIDItem */
   while (pstUIDBuf->uiItemNum--)
   {
      /* free all mailboxs's name */
      EME_UTILS_FREEIF(pstUIDBuf->pMemHdr, pstUIDBuf->ppstUIDItem[pstUIDBuf->uiItemNum]->pcMailboxName);
      /*Free all UID list*/
      EME_UTILS_FREEIF(pstUIDBuf->pMemHdr, pstUIDBuf->ppstUIDItem[pstUIDBuf->uiItemNum]->nCurrList);
      /* free all UIDItems's buffer */
      EME_UTILS_FREEIF(pstUIDBuf->pMemHdr, pstUIDBuf->ppstUIDItem[pstUIDBuf->uiItemNum]);
   }

   /* free buffer which hold pointers of list */
   EME_UTILS_FREEIF(pstUIDBuf->pMemHdr, pstUIDBuf->ppstUIDItem);
   EME_UTILS_MEMSET(pstUIDBuf, 0, sizeof(St_UIDBuf));
   return EME_SUCCESS;
}

static WE_BOOL EME_IMAP_IsStrEqualIC(WE_CHAR *pcStr1, WE_CHAR *pcStr2)
{
   WE_UINT32 i = 0;
   
   if (NULL == pcStr1 || NULL == pcStr2)
      return FALSE;

   if ( EME_UTILS_STRLEN(pcStr1) != EME_UTILS_STRLEN(pcStr2) )
      return FALSE;
   
   /* here, two str has same length */
   for (i = 0; i < (WE_UINT32)EME_UTILS_STRLEN(pcStr1); i++)
   {
      if (pcStr1[i] == pcStr2[i])			
         continue;			
      else
      {
         /* check whether the different between two strings is only due to case */
         if ( pcStr1[i] < 65 || pcStr1[i] > 122 || (pcStr1[i] > 90 && pcStr1[i] < 97) || /* isn't alpha */
            (pcStr1[i] >= 65 && pcStr1[i] <= 90 && pcStr1[i] + 32 != pcStr2[i]) || /* is upper alpha */
            (pcStr1[i] >= 97 && pcStr1[i] <= 122 && pcStr1[i] != pcStr2[i] + 32) ) /* is lower alpha */
            return FALSE;
         else
            continue;
      }			
   }
   return TRUE;
}


WE_INT32 EME_IMAP_ReallocMemory(WE_VOID **ppvOrginPointer, WE_UINT32 uiNewSize, WE_VOID* pMemMgr)
{
   WE_INT32 iRet = EME_SUCCESS;
   WE_VOID *pvTemp = NULL;
   if (NULL == ppvOrginPointer)
      return EME_INVALID_DATA;
   
   do 
   {
      pvTemp = EME_UTILS_REALLOC(pMemMgr, *ppvOrginPointer, uiNewSize);
      EME_UTILS_BREAKNULL(pvTemp, iRet, EME_NOMEMORY, "enlarge memory malloc error");
      
      *ppvOrginPointer = pvTemp;
   } while(0);
   
   return iRet;
}


WE_INT32 EME_IMAP_StringCat(WE_CHAR **ppcOrginString, WE_CHAR* pcAddedString, WE_VOID* pMemMgr)
{
   WE_INT32 iRet = EME_SUCCESS;
   WE_UINT32 uiTotalSize = 0;
   if (NULL == ppcOrginString)
      return EME_INVALID_DATA;

   if (NULL == pcAddedString)
      return EME_SUCCESS;
   
   do 
   {
      uiTotalSize = EME_UTILS_STRLEN(*ppcOrginString) + EME_UTILS_STRLEN(pcAddedString)+1;
      iRet = EME_IMAP_ReallocMemory((WE_VOID*)ppcOrginString, uiTotalSize, pMemMgr);
      EME_UTILS_BREAKIF(iRet, "imap no memory");
      EME_UTILS_STRCAT(*ppcOrginString, pcAddedString);
   } while(0);

   return EME_SUCCESS;
}


WE_INT32 EME_IMAP_StrGetSubString(WE_CHAR* pcInString, 
                                  WE_CHAR  cLeftChar,
                                  WE_CHAR  cRightChar,
                                  WE_CHAR* pcOutString,
                                  WE_UINT32* puiIndex)
{
   char* pcTemp = NULL;
   char* pcStopPos = NULL;
   
   /* Could not token by itself. */
   if (pcInString == pcOutString || NULL == pcInString || NULL == pcOutString)
      return EME_INVALID_DATA;
   
   /* check the index. */
   if (*puiIndex >= (WE_UINT32)EME_UTILS_STRLEN(pcInString))
      return EME_INVALID_DATA;

   /* pcInString + (*puiIndex) is the actual string that contain substring 
      pcTemp will point to location of cLeftChar in string after STRCHR */
   pcTemp = EME_UTILS_STRCHR(pcInString + (*puiIndex), cLeftChar);
   if (NULL == pcTemp)
      return EME_FAILED;

   /* pcStopPos will point to location of cRightChar in string after STRCHR */
   pcStopPos = EME_UTILS_STRCHR(++pcTemp, cRightChar);
   if (NULL == pcStopPos)
      return EME_FAILED;
   
   /* copy substring between cLeftChar and cRightChar to pcOutString */
   while (pcTemp != pcStopPos)
   {
      *pcOutString++ = *pcTemp++;
   }
   
   /* change *puiIndex as return param */
   *puiIndex = pcStopPos - pcInString + 1;    
   
   *pcOutString = '\0';
   return EME_SUCCESS;
}


WE_INT32 EME_IMAP_AddToMailInfoArray(St_MailInfoArray* pstMailInfoArray, St_MailInfo stMailInfo,WE_UINT32 uiNumberLimit, WE_INT32 iReceLast)
{
   WE_INT32 iRet = EME_SUCCESS;
   WE_UINT32 uiIndex  = 0; 
   WE_UINT32 uiMinIndex = 0;
   WE_UINT32 i =0;
   St_MailInfo stMailInfoTemp = {0};
 
   
   if (NULL == pstMailInfoArray)
      return EME_INVALID_DATA;
   
   do 
   {
      if (pstMailInfoArray->uiLength >= pstMailInfoArray->uiMaxLength)
      {         
         iRet = EME_IMAP_ReallocMemory((WE_VOID**)&pstMailInfoArray->pastMailInfo,
                        (pstMailInfoArray->uiLength+EME_MAILINFO_ENLARGE_LENGTH) * sizeof(St_MailInfo),
                        pstMailInfoArray->pMemHdr);
         EME_UTILS_BREAKIF(iRet, "mail info array malloc error");
         pstMailInfoArray->uiMaxLength += EME_MAILINFO_ENLARGE_LENGTH;
      }

	  if (pstMailInfoArray->uiLength == uiNumberLimit)
	  {
		  for (uiIndex = 1;uiIndex < pstMailInfoArray->uiLength;uiIndex++)
		  {
			  if(pstMailInfoArray->pastMailInfo[uiIndex].uiUID < pstMailInfoArray->pastMailInfo[uiMinIndex].uiUID)
			  {
				  uiMinIndex = uiIndex;
			  }

		  }
        
		  if (stMailInfo.uiUID > pstMailInfoArray->pastMailInfo[uiMinIndex].uiUID )
		  {
			  pstMailInfoArray->pastMailInfo[uiMinIndex].uiSize = stMailInfo.uiSize;
			  pstMailInfoArray->pastMailInfo[uiMinIndex].uiUID = stMailInfo.uiUID;
		  }
	  }
	  else
	  {
		  pstMailInfoArray->pastMailInfo[pstMailInfoArray->uiLength].uiSize = stMailInfo.uiSize;
		  pstMailInfoArray->pastMailInfo[pstMailInfoArray->uiLength].uiUID = stMailInfo.uiUID;
		  pstMailInfoArray->uiLength = pstMailInfoArray->uiLength + 1;
	  }
     
     if(iReceLast)
     {
        for (uiIndex = 0;uiIndex < pstMailInfoArray->uiLength;uiIndex++)
        {
           for (i = 1;i < (pstMailInfoArray->uiLength - uiIndex);i++)
           {
              if(pstMailInfoArray->pastMailInfo[uiIndex+i].uiUID > pstMailInfoArray->pastMailInfo[uiIndex].uiUID)
              {
                 stMailInfoTemp.uiSize = pstMailInfoArray->pastMailInfo[uiIndex].uiSize;
                 stMailInfoTemp.uiUID  = pstMailInfoArray->pastMailInfo[uiIndex].uiUID;
                 
                 pstMailInfoArray->pastMailInfo[uiIndex].uiSize = pstMailInfoArray->pastMailInfo[uiIndex+i].uiSize;
                 pstMailInfoArray->pastMailInfo[uiIndex].uiUID = pstMailInfoArray->pastMailInfo[uiIndex+i].uiUID;
                 
                 pstMailInfoArray->pastMailInfo[uiIndex+i].uiSize = stMailInfoTemp.uiSize;
                 pstMailInfoArray->pastMailInfo[uiIndex+i].uiUID = stMailInfoTemp.uiUID;
              }
              
           }
           
        }
     }
     else
     {
        for (uiIndex = 0;uiIndex < pstMailInfoArray->uiLength;uiIndex++)
        {
           for (i = 1;i < (pstMailInfoArray->uiLength - uiIndex);i++)
           {
              if(pstMailInfoArray->pastMailInfo[uiIndex+i].uiUID < pstMailInfoArray->pastMailInfo[uiIndex].uiUID)
              {
                 stMailInfoTemp.uiSize = pstMailInfoArray->pastMailInfo[uiIndex].uiSize;
                 stMailInfoTemp.uiUID  = pstMailInfoArray->pastMailInfo[uiIndex].uiUID;
                 
                 pstMailInfoArray->pastMailInfo[uiIndex].uiSize = pstMailInfoArray->pastMailInfo[uiIndex+i].uiSize;
                 pstMailInfoArray->pastMailInfo[uiIndex].uiUID = pstMailInfoArray->pastMailInfo[uiIndex+i].uiUID;
                 
                 pstMailInfoArray->pastMailInfo[uiIndex+i].uiSize = stMailInfoTemp.uiSize;
                 pstMailInfoArray->pastMailInfo[uiIndex+i].uiUID = stMailInfoTemp.uiUID;
              }
              
           }
           
        }
     }


   } while(0);
   
   if (EME_SUCCESS != iRet)
   {
      return iRet;
   }
   
   return EME_SUCCESS;
}

WE_INT32 EME_IMAP_GetUIDFromArray(St_MailInfoArray* pstMailInfoArray, WE_UINT32* puiUID)
{
   if (NULL == pstMailInfoArray || NULL == puiUID)
      return EME_INVALID_DATA;

   if(pstMailInfoArray->uiLength == 0)
      return EME_NO_NEWMAIL;

   if (pstMailInfoArray->uiIndex >= pstMailInfoArray->uiLength)
      return EME_RECEIVED_END;
   
   *puiUID = pstMailInfoArray->pastMailInfo[pstMailInfoArray->uiIndex++].uiUID;

   return EME_SUCCESS;
}


WE_INT32 EME_IMAP_GetSizeFromArray(St_MailInfoArray* pstMailInfoArray, WE_UINT32 uiUID, WE_UINT32* puiSize)
{
   WE_UINT32 uiTempIndex = 0;
   if (NULL == pstMailInfoArray || NULL == puiSize)
      return EME_INVALID_DATA;

   while (uiTempIndex < pstMailInfoArray->uiLength)
   {
      if (uiUID == pstMailInfoArray->pastMailInfo[uiTempIndex].uiUID)
      {
         *puiSize = pstMailInfoArray->pastMailInfo[uiTempIndex].uiSize;
         return EME_SUCCESS;
      }
      uiTempIndex++;
   }

   return EME_FAILED;
}


WE_INT32 EME_IMAP_ReleaseMailInfoArray(St_MailInfoArray* pstMailInfoArray)
{
   if (NULL == pstMailInfoArray)
      return EME_INVALID_DATA;

   EME_UTILS_FREEIF(pstMailInfoArray->pMemHdr, pstMailInfoArray->pastMailInfo);
   EME_UTILS_MEMSET(pstMailInfoArray, 0, sizeof(St_MailInfoArray));

   return EME_SUCCESS;
}

WE_INT32 EME_IMAP_AddToLongString(St_LongString* pstLongString, WE_CHAR* pcString, WE_BOOL bFile)
{
   WE_INT32 iRet = EME_SUCCESS;
   WE_UINT32 uiEnlargeSize = 0;

   if (NULL == pstLongString || NULL == pcString)
      return EME_INVALID_DATA;

   do
   {
      if(bFile)
      {
         if(NULL == pstLongString->pcFileName)
         {
            pstLongString->pvCatchBuf = EME_UTILS_MALLOC(pstLongString->pMemHdr, EME_IMAP_BUFFER_CATCH_SIZE + 1);
            EME_UTILS_BREAKNULL(pstLongString->pvCatchBuf, iRet, EME_NOMEMORY, "no memory for catch buff");
            EME_UTILS_MEMSET(pstLongString->pvCatchBuf, 0, EME_IMAP_BUFFER_CATCH_SIZE + 1);

            pstLongString->pcFileName = EME_UTILS_MALLOC(pstLongString->pMemHdr,
               EME_UTILS_STRLEN(EME_IMAP_TEMP_MAIL_FILE_DIR) +
               EME_IMAP_MAX_FILE_NAME_LEN);
            EME_UTILS_BREAKNULL(pstLongString->pcFileName, iRet, EME_NOMEMORY,"no memory for temp file");
            EME_UTILS_MEMSET(pstLongString->pcFileName, 0, 
               EME_UTILS_STRLEN(EME_IMAP_TEMP_MAIL_FILE_DIR) + 
               EME_IMAP_MAX_FILE_NAME_LEN);

            EME_UTILS_SPRINTF(pstLongString->pcFileName, "%s\\eml%x.hdr",
               EME_IMAP_TEMP_MAIL_FILE_DIR,
               EME_UTILS_GETTIMEMS);

            pstLongString->pvTmpFile = EME_FileOpen(pstLongString->pvFileMgr, pstLongString->pcFileName, EME_OFM_CREATE);
            EME_UTILS_BREAKNULL(pstLongString->pvTmpFile, iRet, EME_FAILED, "create tmp file failed");
            pstLongString->uiHadUsedSize = 0;
         }

         if(pstLongString->uiHadUsedSize + EME_UTILS_STRLEN(pcString) > EME_IMAP_BUFFER_CATCH_SIZE)
         {
            EME_FileWrite(pstLongString->pvTmpFile, pstLongString->pvCatchBuf, pstLongString->uiHadUsedSize);
            EME_UTILS_MEMSET(pstLongString->pvCatchBuf, 0, EME_IMAP_BUFFER_CATCH_SIZE);
            pstLongString->uiHadUsedSize = 0;

            EME_UTILS_MEMCPY(pstLongString->pvCatchBuf, pcString, EME_UTILS_STRLEN(pcString));
            pstLongString->uiHadUsedSize += EME_UTILS_STRLEN(pcString);
         }
         else
         {
            EME_UTILS_MEMCPY((WE_CHAR*)pstLongString->pvCatchBuf + pstLongString->uiHadUsedSize,
               pcString,
               EME_UTILS_STRLEN(pcString));
            pstLongString->uiHadUsedSize += EME_UTILS_STRLEN(pcString);
         }
      }
      else
      {
         /* if no memory left for add new string, realloc */
         if (NULL == pstLongString->pcLongString
            || pstLongString->uiMaxLength
            < (pstLongString->uiLength + EME_UTILS_STRLEN(pcString) + 1))
         {
         /* if the add string is too long for left memory and EME_ENLARGE_SIZE to
            contain, allocate length of pcSting+1,else allocate EME_ENLARGE_SIZE */
            if (pstLongString->uiMaxLength + EME_ENLARGE_SIZE
               <= EME_UTILS_STRLEN(pcString) + pstLongString->uiLength)
            {
               uiEnlargeSize = EME_UTILS_STRLEN(pcString)+1;
               
            }
            else
            {
               uiEnlargeSize = EME_ENLARGE_SIZE;
            }
            
            iRet = EME_IMAP_ReallocMemory((WE_VOID**)&pstLongString->pcLongString,
               (pstLongString->uiMaxLength+uiEnlargeSize) * sizeof(WE_CHAR),
               pstLongString->pMemHdr);
            EME_UTILS_BREAKIF(iRet, "pstUIDBuf malloc error");
            EME_UTILS_BREAKNULL(pstLongString->pcLongString, iRet, EME_NOMEMORY, "pstUIDBuf malloc error");
            pstLongString->uiMaxLength += uiEnlargeSize; 
         }
         
         EME_UTILS_STRCAT(pstLongString->pcLongString, pcString);
         pstLongString->uiLength += EME_UTILS_STRLEN(pcString);
      }

      

   } while(0);
   
   if (EME_SUCCESS != iRet)
   {
      EME_FileClose(pstLongString->pvTmpFile);
      pstLongString->pvTmpFile = NULL;

      EME_DeleteFile(pstLongString->pvFileMgr, pstLongString->pcFileName);
      EME_UTILS_FREEIF(pstLongString->pMemHdr, pstLongString->pcFileName);

      return iRet;
   }

   return EME_SUCCESS;
}


WE_CHAR* EME_IMAP_GetLongString(St_LongString* pstLongString)
{
   WE_CHAR* pcLongString = NULL;
   if (NULL == pstLongString)
      return NULL;

   pcLongString = pstLongString->pcLongString;

   /* reinit it for adding other string */
   pstLongString->pcLongString = NULL;
   pstLongString->uiLength = 0;
   pstLongString->uiMaxLength = 0;

   return pcLongString;
}


WE_INT32 EME_IMAP_Utf8ToUtf7(WE_CHAR* pcUtf8, WE_CHAR* pcUtf7, WE_VOID* pvMemMgr)
{
   WE_UINT32 iRet = EME_SUCCESS;
   WE_UINT16 *usUcs2 = NULL;
   if (NULL == pcUtf7 || NULL == pcUtf8)
      return EME_INVALID_DATA;
   
   do 
   {
      usUcs2 = EME_UTILS_MALLOC(pvMemMgr, EME_UTILS_STRLEN(pcUtf8)*2+2);
      EME_UTILS_BREAKNULL(usUcs2, iRet, EME_NOMEMORY, "IMAP no memory");
      EME_UTILS_MEMSET(usUcs2, 0, EME_UTILS_STRLEN(pcUtf8)*2+2);
      EME_Utf8ToUcs2((WE_UCHAR*)pcUtf8, usUcs2);
      EME_Ucs2ToUtf7(usUcs2, pcUtf7, 1);
   } while(0);

   EME_UTILS_FREEIF(pvMemMgr, usUcs2);
   return iRet;
}


WE_INT32 EME_IMAP_Utf7ToUtf8(WE_CHAR* pcUtf7, WE_CHAR* pcUtf8, WE_VOID* pvMemMgr)
{
   WE_UINT32 iRet = EME_SUCCESS;
   WE_UINT16 *usUcs2 = NULL;
   if (NULL == pcUtf7 || NULL == pcUtf8)
      return EME_INVALID_DATA;
   
   do 
   {
      usUcs2 = EME_UTILS_MALLOC(pvMemMgr, EME_UTILS_STRLEN(pcUtf7)*2+2);
      EME_UTILS_BREAKNULL(usUcs2, iRet, EME_NOMEMORY, "IMAP no memory");
      EME_UTILS_MEMSET(usUcs2, 0, EME_UTILS_STRLEN(pcUtf7)*2+2);
      EME_Utf7ToUcs2(pcUtf7, usUcs2, 1);
      EME_Ucs2ToUtf8(usUcs2, (WE_UCHAR*)pcUtf8);
   } while(0);
   
   EME_UTILS_FREEIF(pvMemMgr, usUcs2);
   return iRet;
}