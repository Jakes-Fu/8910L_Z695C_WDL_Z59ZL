/*=====================================================================================
FILE NAME   : eme_codec_decode_bodystruct.c
MODULE NAME : email codec

  
    GENERAL DESCRIPTION
    This File will support a way to decode a email type:body structure.
    These functions don't be changed by project changed.
    
      TechFaith Software Confidential Proprietary
      Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
      =======================================================================================
      Revision History
      
        Modification                   Tracking
        Date              Author       Number       Description of changes
        ----------   --------------   ---------   --------------------------------------
         20070306       wxs             01                  create
         20070310       wxs             02                  modify
        
=====================================================================================*/

/************************************************************************/
/*                     include files                                   */
/************************************************************************/
#include "eme_codec_decode_bodystruct.h"
#include "eme_codec_mailmessage.h"
#include "eme_encode_base64.h"
#include "eme_codec_utils_api.h"


/* ========private function declare========= */
static WE_INT32  EME_BodyStruct_AppendToList(stEMSGHandle *pstEMsgHandle,
                                             void* pvEBodyStruct,
                                             St_BodyStructPart* part);

static WE_INT32  EME_BodyStruct_ClearPartList(stEMSGHandle *pstEMsgHandle,void* pvEBodyStruct);

static WE_INT32  EME_BodyStruct_ParseElement(stEMSGHandle *pstEMsgHandle,void* pvEBodyStruct);

static WE_UINT32 EME_BodyStruct_GetPartSize(WE_CHAR *pStart);
static WE_INT32  EME_BodyStruct_CalcuSection(void* pvEBodyStruct,
                                             WE_CHAR* section, 
                                             WE_UINT16 bHtml);

static void*     EME_BodyStruct_GetElement(stEMSGHandle *pstEMsgHandle,
                                           WE_CHAR* pFind,
                                           WE_INT32 *piResut);

static void*     EME_BodyStruct_GetPartCharset(stEMSGHandle *pstEMsgHandle, 
                                               WE_CHAR* pStr,
                                               WE_INT32 *piResut);

static WE_CHAR*  EME_BodyStruct_GetTransferType(stEMSGHandle *pstEMsgHandle,WE_CHAR* pStr);


/*===================================================================
* 
*                    Functions
* 
* ==================================================================*/

WE_INT32 EME_BodyStruct_Create(stEMSGHandle *pstEMsgHandle,void** pvBodyStruct)
{  
   WE_INT32         iResult      = EME_SUCCESS; /*must be init to success*/ 
   void             *pvMemMgr    = NULL;
   St_BodyStruct    *pRet        = NULL;
   if (NULL == pstEMsgHandle || NULL == pvBodyStruct)
   {
      EME_UTILS_LOG_ERROR(("EME_BodyStruct_Create(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
     pRet = EME_UTILS_MALLOC(pvMemMgr,sizeof(St_BodyStruct));
     EME_UTILS_BREAKNULL(pRet,
        iResult,
        EME_NOMEMORY,
        "memory malloc failed in EME_BodyStruct_Create()");
    EME_UTILS_MEMSET(pRet,0x00,sizeof(St_BodyStruct));
    *pvBodyStruct = pRet;
   } while(0);
   
   return iResult;
}

WE_INT32 EME_BodyStruct_Destroy(stEMSGHandle *pstEMsgHandle,void** pvBodyStruct)
{
   WE_INT32          iResult           = EME_FAILED; 
   void              *pvMemMgr         = NULL;
   St_BodyStruct     *pstBodyStructMsg = NULL;
   
   if (NULL == pstEMsgHandle || NULL == pvBodyStruct)
   {
      EME_UTILS_LOG_ERROR(("@@@EME_BodyStruct_Destroy(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
      if (NULL == *pvBodyStruct)
      {
         iResult = EME_SUCCESS;
         EME_UTILS_LOG_INFO(("BodyStruct is NULL,don't need free memory!"));
         break;
      }
      pstBodyStructMsg = (St_BodyStruct *)(*pvBodyStruct);
      
      iResult = EME_BodyStruct_ClearPartList(pstEMsgHandle,pstBodyStructMsg);
      EME_UTILS_BREAKIF(iResult,"EME_BodyStruct_Destroy EME_BodyStruct_ClearPartList,failed!!!!!!!!!!!");

//       EME_UTILS_FREEIF(pvMemMgr,pstBodyStructMsg->_mixedBndary);
//       EME_UTILS_FREEIF(pvMemMgr,pstBodyStructMsg->_relatedBndary);
     
      EME_UTILS_FREEIF(pvMemMgr,pstBodyStructMsg);

      *pvBodyStruct = NULL;
   } while(0);
   
   
   return iResult;
}


WE_INT32 EME_BodyStruct_Parse(stEMSGHandle *pstEMsgHandle,
                              void* pvBodyStruct, 
                              WE_CHAR* pcBodystructBuff)
{
   WE_INT32       iResult          = EME_SUCCESS; /*must init to success*/
   St_BodyStruct *pstBodyStructMsg = (St_BodyStruct*)pvBodyStruct;
   
   if ( NULL == pstBodyStructMsg || NULL == pcBodystructBuff )
   {
      EME_UTILS_LOG_ERROR(("EME_BodyStruct_Parse(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   do 
   {
      pstBodyStructMsg->pcPosition = pcBodystructBuff;
      pstBodyStructMsg->sMarkCounter = -1;
      
      while ( EME_UTILS_STRLEN(pstBodyStructMsg->pcPosition) > 0 )
      {
         if ( *(pstBodyStructMsg->pcPosition) == '(' )
         {
            pstBodyStructMsg->sMarkCounter++;
            
            if ( pstBodyStructMsg->sMarkCounter == 1)
            {
               pstBodyStructMsg->usMainPtNum++;
               pstBodyStructMsg->usSecPtNum = 0;
            }
            if (pstBodyStructMsg->sMarkCounter == 2)
            {
               pstBodyStructMsg->usSecPtNum++;
            }
            if ( *(pstBodyStructMsg->pcPosition + 1) == '\"' )
            {
               /* 0 == pstBodyStructMsg->sMarkCounter means mail has only one level */
               if (0 == pstBodyStructMsg->sMarkCounter)
               {
                  pstBodyStructMsg->usMainPtNum++;
               }
               /* do field parse here */
               iResult = EME_BodyStruct_ParseElement(pstEMsgHandle,pstBodyStructMsg);
               EME_UTILS_BREAKIF(iResult,"EME_BodyStruct_Parse EME_BodyStruct_ParseElement,failed!!!");
            }
         }
         EME_UTILS_BREAKIF(iResult,"EME_BodyStruct_Parse EME_BodyStruct_ParseElement(),failed!!!!!!!!!!!!");
         if ( *(pstBodyStructMsg->pcPosition) == ')' )
         {
            pstBodyStructMsg->sMarkCounter--;
         }
         pstBodyStructMsg->pcPosition++;
      }
      
   } while(0);
   
   return iResult;
}


static WE_INT32 EME_BodyStruct_ParseElement(stEMSGHandle *pstEMsgHandle,void* pvEBodyStruct)
{
   WE_INT32 iResult = EME_SUCCESS; /*must init to success*/
   void *pvMemMgr = NULL;
   St_BodyStruct* pstBodyStructMsg = NULL;
   WE_CHAR* pFind = NULL;
   pstBodyStructMsg = (St_BodyStruct*)pvEBodyStruct;
   if (NULL == pstEMsgHandle || NULL == pstBodyStructMsg || NULL == pstBodyStructMsg->pcPosition )
   {
      EME_UTILS_LOG_ERROR(("EME_BodyStruct_ParseElement(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
      St_BodyStructPart* pBodyStructPart = EME_UTILS_MALLOC(pvMemMgr,sizeof(St_BodyStructPart));
      EME_UTILS_BREAKNULL(pBodyStructPart,
         iResult,
         EME_NOMEMORY,
         "memory malloc failed!!!!!!!");
      EME_UTILS_MEMSET(pBodyStructPart,0x00,sizeof(St_BodyStructPart));
      
      if ( (pFind = EME_UTILS_StrStrNC(pstBodyStructMsg->pcPosition, "\"text\""))  == pstBodyStructMsg->pcPosition + 1 )
      {
         /* record text hierarchical level */
         pstBodyStructMsg->sTextHierarch = pstBodyStructMsg->sMarkCounter;
         pBodyStructPart->pcCharset = EME_BodyStruct_GetPartCharset(pstEMsgHandle,pFind,&iResult);
         EME_UTILS_BREAKIF(iResult,"EME_BodyStruct_GetPartCharset,failed!!!!");
         
         pBodyStructPart->uiBSPartSize = EME_BodyStruct_GetPartSize(pFind);
         pBodyStructPart->pcTransEncoding = EME_BodyStruct_GetTransferType(pstEMsgHandle,pFind);
         
         pFind += 7; /* go behind "\"text\" " */
         if (EME_UTILS_StrStrNC(pFind, "\"plain\"") == pFind)
         {
            pBodyStructPart->eBodyStructType = E_BODYS_TEXTPLAIN;
            iResult = EME_BodyStruct_CalcuSection(pvEBodyStruct, pBodyStructPart->acSection, 0);
            EME_UTILS_BREAKIF(iResult,"EME_BodyStruct_CalcuSection,failed!!!!!");
         }
         else if (EME_UTILS_StrStrNC(pFind, "\"html\"") == pFind)
         {
            pBodyStructPart->eBodyStructType = E_BODYS_TEXTHTML;
            iResult = EME_BodyStruct_CalcuSection(pvEBodyStruct, pBodyStructPart->acSection, 1);
            EME_UTILS_BREAKIF(iResult,"EME_BodyStruct_CalcuSection,failed!!!!!");
         }
         EME_BodyStruct_AppendToList(pstEMsgHandle,pvEBodyStruct, pBodyStructPart);
      }
      
      else if ( (pFind = EME_UTILS_StrStrNC(pstBodyStructMsg->pcPosition, "\"name\"")) == pstBodyStructMsg->pcPosition + 1 )
      {
         WE_CHAR* pEnd = NULL;
         WE_CHAR* pName = NULL;
         
         pBodyStructPart->uiBSPartSize = EME_BodyStruct_GetPartSize(pFind);
         pBodyStructPart->pcTransEncoding = EME_BodyStruct_GetTransferType(pstEMsgHandle, pFind);
         pFind += 7; /* go behind "\"name\" " */
         
         /* get name in unicode */
         pName = EME_BodyStruct_GetElement(pstEMsgHandle,pFind,&iResult);
         EME_UTILS_BREAKIF(iResult,"EME_BodyStruct_GetElement,failed!!!!");
         
         pBodyStructPart->pcAttachName = EME_UTILS_STRDUP(pvMemMgr, pName);
         EME_UTILS_FREEIF(pvMemMgr,pName);
         
         pEnd = EME_UTILS_STRCHR(pFind+1, '\"');
         if (NULL != pEnd && pEnd + 1 == EME_UTILS_STRCHR(pEnd + 1, ')') )
         {
            pFind = pEnd + 3; /* go behind "\") "*/
            pBodyStructPart->pcContentId = EME_BodyStruct_GetElement(pstEMsgHandle,pFind,&iResult);
            EME_UTILS_BREAKIF(iResult,"EME_BodyStruct_GetElement,failed!!!!");
         }
         
         /* if the att level == text hierarch level, it is related att */
         if ( pstBodyStructMsg->sTextHierarch == pstBodyStructMsg->sMarkCounter)
         {
            pBodyStructPart->eBodyStructType = E_BODYS_RELATEDATTACH;
            if ( pstBodyStructMsg->sMarkCounter >= 3 )
               EME_UTILS_SPRINTF(pBodyStructPart->acSection, "%d.%d", pstBodyStructMsg->usMainPtNum, pstBodyStructMsg->usSecPtNum);
            else
               EME_UTILS_SPRINTF(pBodyStructPart->acSection, "%d", pstBodyStructMsg->usMainPtNum);
         }
         else
         {
            pBodyStructPart->eBodyStructType = E_BODYS_ATTACHMENT;
            EME_UTILS_SPRINTF(pBodyStructPart->acSection, "%d", pstBodyStructMsg->usMainPtNum);
         }
         iResult = EME_BodyStruct_AppendToList(pstEMsgHandle,pvEBodyStruct, pBodyStructPart);
         EME_UTILS_BREAKIF(iResult,"EME_BodyStruct_AppendToList,EME_bodyStruct_appendTolist(),failed!!!!");
      }
      else
      {
         EME_UTILS_FREEIF(pvMemMgr,pBodyStructPart);
      }

   } while(0);
   
   return iResult;
}

static WE_INT32 EME_BodyStruct_CalcuSection(void* pvEBodyStruct, WE_CHAR* section, WE_UINT16 bHtml)
{   
   St_BodyStruct* pstBodyStructMsg = (St_BodyStruct*)pvEBodyStruct;
   if (NULL == pstBodyStructMsg )
   {
      EME_UTILS_LOG_ERROR(("EME_BodyStruct_CalcuSection(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
  
   if (pstBodyStructMsg->sTextHierarch >= 3)
   {
      EME_UTILS_SPRINTF(section, "%d.%d.%d",
         pstBodyStructMsg->usMainPtNum, pstBodyStructMsg->usSecPtNum, bHtml + 1);
   }
   else if (pstBodyStructMsg->sTextHierarch == 2)
   {
      EME_UTILS_SPRINTF(section, "%d.%d", pstBodyStructMsg->usMainPtNum, pstBodyStructMsg->usSecPtNum);
   }
   else
   {
      EME_UTILS_SPRINTF(section, "%d", pstBodyStructMsg->usMainPtNum);
   }
   
   return EME_SUCCESS;
}

static void* EME_BodyStruct_GetElement(stEMSGHandle *pstEMsgHandle,
                                       WE_CHAR* pFind,
                                       WE_INT32 *piResut)
{
   WE_INT32 iResult = EME_SUCCESS; /*must init to success*/
   void *pvMemMgr = NULL;
   WE_CHAR* pEnd = NULL;
   void* pRet = NULL;
   
   if (NULL == pstEMsgHandle || NULL == pFind || NULL == piResut)
   {
      EME_UTILS_LOG_ERROR(("EME_BodyStruct_GetElement(),input parameters is invalid!!!!"));
      return NULL;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;

   if ( EME_UTILS_STRCHR(pFind, '\"') == pFind )
   {
      pEnd = EME_UTILS_STRCHR(pFind+1, '\"');
      if (pEnd)
      {
         pRet = EME_UTILS_MALLOC(pvMemMgr,pEnd - pFind + 1);
         if (NULL == pRet)
         {
            EME_UTILS_LOG_ERROR(("EME_BodyStruct_GetElement,memory malloc failed!!!"));
            iResult = EME_NOMEMORY;
            *piResut = iResult;
            return NULL;
         }

         EME_UTILS_MEMSET(pRet,0x00,pEnd - pFind + 1);
         iResult = EME_MailMsg_ParseLine(pstEMsgHandle,pRet, pFind + 1, pEnd - 1);
      }
   }
   *piResut = iResult;
   return pRet;
}


static void* EME_BodyStruct_GetPartCharset(stEMSGHandle *pstEMsgHandle,
                                           WE_CHAR* pStr,
                                           WE_INT32 *piResut)
{
   WE_INT32 iResult = EME_SUCCESS; /*must init to success*/
   void *pvMemMgr = NULL;
   WE_CHAR* pStart = NULL;
   WE_CHAR* pRet = NULL;
   WE_CHAR* pCharSet = NULL;
   WE_CHAR* pEnd = NULL;
   if (NULL == pstEMsgHandle || NULL == pStr || NULL == piResut)
   {
      EME_UTILS_LOG_ERROR(("EME_BodyStruct_GetPartCharset(),input parameters is invalid!!!!"));
      return NULL;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;

   pStart = EME_UTILS_STRCHR(pStr, '(');
   if ( (pCharSet = EME_UTILS_StrStrNC(pStr, "\"charset\"")) == pStart + 1)
   {
      pCharSet += 10; /* go behind "\"charset\" " */
      if (*pCharSet != '\"')
      {
         return NULL;
      }

      pCharSet++;
      if ( (pEnd = EME_UTILS_STRCHR(pCharSet, '\"')) != NULL)
      {
         if (*(pEnd + 1) != ')')
         {
            return NULL;
         }
         pRet = EME_UTILS_MALLOC(pvMemMgr,pEnd - pCharSet + 1);
         if (NULL == pRet)
         {
            EME_UTILS_LOG_ERROR(("EME_BodyStruct_GetPartCharset,memory malloc failed!!!"));
            iResult = EME_NOMEMORY;
         }
         EME_UTILS_MEMSET(pRet,0x00,pEnd - pCharSet + 1);
         EME_UTILS_MEMMOVE(pRet, pCharSet, pEnd - pCharSet);
      }
   }
   
   *piResut = iResult;
   return pRet;
}


static WE_UINT32 EME_BodyStruct_GetPartSize(WE_CHAR* pStr)
{
   WE_CHAR* pStart = EME_UTILS_STRCHR(pStr, ')');
   WE_CHAR* pFind = NULL;
   WE_UINT16 iIndex = 0;
   
   if (pStart == NULL)
   {
      return 0;
   }
   
   pFind = pStart;
   /* 4 means: go 4 spaces-' ', and get the size */
   for (iIndex = 0; iIndex < 4;)
   {
      if ( (pFind = EME_UTILS_STRCHR(pFind, ' ')) != NULL )
      {
         iIndex++;
         pFind++;
         /* if find "..." go behind end of it */
         if ( *pFind == '\"')
         {
            pFind = EME_UTILS_STRCHR(pFind+1, '\"');
         }
      }
      if (NULL == pFind || EME_UTILS_STRLEN(pFind) == 0)
      {
         return 0;
      }
   }

   return TF3RD_STRTOUL(pFind, NULL, 10);
}


static WE_CHAR* EME_BodyStruct_GetTransferType(stEMSGHandle *pstEMsgHandle, WE_CHAR* pStr)
{
   WE_CHAR* pStart = NULL;
   WE_CHAR* pFind = NULL;
   WE_CHAR* pType = NULL;
   WE_CHAR* pRet = NULL;
   WE_INT32 iIndex = 0;
   void *pvMemMgr = NULL;
   if (NULL == pstEMsgHandle || NULL == pStr)
   {
      EME_UTILS_LOG_ERROR(("EME_BodyStruct_GetTransferType(),input parameters is invalid!!!!"));
      return NULL;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
      pStart = EME_UTILS_STRCHR(pStr, ')');
      if (NULL == pStart)
      {
         EME_UTILS_LOG_ERROR(("EME_BodyStruct_GetTransferType(),is not exist!!!!"));
         break;
      }
      pFind = pStart;
      /* 4 means: go 3 spaces-' ', and get the transfer type */
      for (iIndex = 0; iIndex < 3;)
      {
         if ( (pFind = EME_UTILS_STRCHR(pFind, ' ')) != NULL )
         {
            iIndex++;
            pFind++;
            /* if find "..." go behind end of it */
            if ( *pFind == '\"')
            {
               if (iIndex == 3)
               {
                  pType = pFind+1;
               }
               pFind = EME_UTILS_STRCHR(pFind+1, '\"');
            }
         }
         
         if (NULL == pFind || EME_UTILS_STRLEN(pFind) == 0)
         {
            return NULL;
         }
         
      }
      
      if (NULL != pType)
      {
         pRet = EME_UTILS_MALLOC(pvMemMgr,pFind - pType + 1);
         if (NULL == pRet)
         {
            EME_UTILS_LOG_ERROR(("no memory to allocate !!!!!!!!"));
            break;
         }
         EME_UTILS_MEMSET(pRet,0x00,pFind - pType + 1);
         EME_UTILS_MEMMOVE(pRet, pType, pFind - pType);
         
      }
   } while(0);
   
   return pRet;
}


WE_UINT32 EME_BodyStruct_GetBdNum(void* pvBodyStruct)
{
   St_BodyStruct* pstBodyStructMsg = (St_BodyStruct*)pvBodyStruct;
   
   if (NULL == pstBodyStructMsg)
   {
      EME_UTILS_LOG_ERROR(("EME_BodyStruct_GetBdNum(),input parameters is invalid!!!!"));
      return 0;
   }
   
   return pstBodyStructMsg->uiBSPartNum;
}


void* EME_BodyStruct_GetBdPart(void* pvBodyStruct, WE_UINT32 id)
{
   St_BodyStruct* pstBodyStructMsg = (St_BodyStruct*)pvBodyStruct;
   if (NULL == pstBodyStructMsg || id > pstBodyStructMsg->uiBSPartNum )
   {
      EME_UTILS_LOG_ERROR(("EME_BodyStruct_GetBdPart(),input parameters is invalid!!!!"));
      return NULL;
   }
   
   return pstBodyStructMsg->ppstBSPartList[id];
}

/* part list manager */
static WE_INT32 EME_BodyStruct_AppendToList(stEMSGHandle *pstEMsgHandle,
                                            void* pvEBodyStruct,
                                            St_BodyStructPart* part)
{
   WE_INT32 iResult = EME_SUCCESS;   /*must be init to success*/
   void *pvMemMgr   = NULL;
   St_BodyStruct* pstBodyStructMsg = (St_BodyStruct *)pvEBodyStruct;
   
   if (NULL == pstEMsgHandle || NULL == pstBodyStructMsg || NULL == part)
   {
      EME_UTILS_LOG_ERROR(("EME_BodyStruct_AppendToList(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
      if (pstBodyStructMsg->uiBSPartNum % 10 == 0)
      {
         pstBodyStructMsg->ppstBSPartList = EME_UTILS_REALLOC(pvMemMgr,pstBodyStructMsg->ppstBSPartList, sizeof(St_BodyStructPart*) * (pstBodyStructMsg->uiBSPartNum + 10));
         EME_UTILS_BREAKNULL(pstBodyStructMsg->ppstBSPartList,
            iResult,
            EME_NOMEMORY,
            "EME_BodyStruct_AppendToList(),memory allocate failed!!!");
      }
      
      pstBodyStructMsg->ppstBSPartList[pstBodyStructMsg->uiBSPartNum++] = part;
      
   } while(0);
   
   return iResult;
   
}

static WE_INT32 EME_BodyStruct_ClearPartList(stEMSGHandle *pstEMsgHandle,void* pvEBodyStruct)
{
   void *pvMemMgr  = NULL;
   St_BodyStruct* pstBodyStructMsg = (St_BodyStruct*)pvEBodyStruct;
   WE_UINT32 iIndex = 0;
   
   if (NULL == pstEMsgHandle || NULL == pstBodyStructMsg)
   {
      EME_UTILS_LOG_ERROR(("EME_BodyStruct_ClearPartList(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;

   for (iIndex = 0; iIndex < pstBodyStructMsg->uiBSPartNum; iIndex++)
   {
      if (NULL != pstBodyStructMsg->ppstBSPartList[iIndex])
      {
         EME_UTILS_FREEIF(pvMemMgr,pstBodyStructMsg->ppstBSPartList[iIndex]->pcAttachName);
         EME_UTILS_FREEIF(pvMemMgr,pstBodyStructMsg->ppstBSPartList[iIndex]->pcContentId);
         EME_UTILS_FREEIF(pvMemMgr,pstBodyStructMsg->ppstBSPartList[iIndex]->pcCharset);
         EME_UTILS_FREEIF(pvMemMgr,pstBodyStructMsg->ppstBSPartList[iIndex]->pcTransEncoding);
         EME_UTILS_FREEIF(pvMemMgr,pstBodyStructMsg->ppstBSPartList[iIndex]);
      }
   }

   EME_UTILS_FREEIF(pvMemMgr,pstBodyStructMsg->ppstBSPartList);
   return EME_SUCCESS;
}


