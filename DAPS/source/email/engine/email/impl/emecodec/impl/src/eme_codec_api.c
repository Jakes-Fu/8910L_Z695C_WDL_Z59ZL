/*=====================================================================================
FILE NAME   : eme_codec_api.c
MODULE NAME : email codec

  
    GENERAL DESCRIPTION
      This File will support a list interface funcion  for email engine encoding or
      decoding email.These functions don't be changed by project changed.
    
      TechFaith Software Confidential Proprietary
      Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
      =======================================================================================
      Revision History
      
        Modification                   Tracking
        Date              Author       Number       Description of changes
        ----------   --------------   ---------   --------------------------------------
         20070306       wxs             01                  create
         20070308       wxs             02                  modify
        
=====================================================================================*/

/*===================================================================
* 
*                     Include Files
* 
====================================================================*/
/*Include System head file*/
#include "eme_file_api.h"
#include "eme_utils_api.h"
//#include "AEEFile.h" 
/*Include Module head file*/
#include "eme_codec_mailmessage.h"
#include "eme_codec_api.h"
#include "eme_codec_def.h"
#include "eme_codec_utils_api.h"
#include "oem_asyncproc_api.h"
/*Include gzip head file*/
#include "common.h"

static WE_INT32 EME_ReadFilePart(WE_VOID* pvFileMgr, WE_VOID* pvMemMgr, EmailOpt *pstPartOpts);
static WE_INT32 EME_ReadFileParteEx(WE_VOID* pvFileMgr, WE_VOID* pvMemMgr, EmailOpt *pstPartOpts);

/************************************************************************/
/*                         function implement                         */
/************************************************************************/

/* --------------------------------------------------------------------
* 
*    FUNCTION        EMECodecLib_InitHandle()
*
*    Pseudo Code:    
*              1. check input parameters
*              2. init Msg handle operation
*              3. malloc memory for Msg handle
*              4. error handling
*    
*    ================================================================*/

int EMECodecLib_InitHandle(IEMSGHandle *phEmailHandle,void* pvMemoryMgr,void *pIFileMgr,void* pICSC)
{
   WE_INT32       iResult = EME_SUCCESS;  /* must be init to SUCCESS */
   stEMSGHandle   *pstEmailHandle = NULL;
   
   /*check input parameters*/
   if (NULL == phEmailHandle || pICSC == NULL)
   {
      EME_UTILS_LOG_ERROR(("EMECodecLib_InitHandle(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   
   do 
   { 
      pstEmailHandle = EME_UTILS_MALLOC(pvMemoryMgr,sizeof(stEMSGHandle));
      EME_UTILS_BREAKNULL(pstEmailHandle,
                          iResult,
                          EME_NOMEMORY,
                          "memory malloc failed!");
      EME_UTILS_MEMSET(pstEmailHandle,0x00,sizeof(stEMSGHandle));

      pstEmailHandle->pvMemMgr = pvMemoryMgr;
      pstEmailHandle->pIFileMgr = pIFileMgr;
      pstEmailHandle->pICSC = pICSC;

      iResult = EME_CreateMailMsg(pstEmailHandle,&pstEmailHandle->pvEmailMsg);
      EME_UTILS_BREAKIF(iResult,"EMECodecLib_InitHandle,EME_CreateMailMsg,failed!");
      EME_UTILS_BREAKNULL(pstEmailHandle->pvEmailMsg,
                          iResult,
                          EME_FAILED,
                          "EMECodecLib_InitHandle,EME_CreateMailMsg,failed!!!!!");

      *phEmailHandle = pstEmailHandle;
      
   } while(0);
   
   if (EME_SUCCESS != iResult)
   {
      EMECodecLib_DeleteHandle((IEMSGHandle *)&pstEmailHandle);
   }
   
   return iResult;
}

/* --------------------------------------------------------------------
* 
*    FUNCTION        EMECodecLib_DeleteHandle()
*
*    Pseudo Code:    
*              1. check input parameters
*              2. delete handle operation:
*                (1). delete body part
*                (2). delete message header part
*                (3). delete message part opt arrays
*  		        (4). delete message  buffer
*                (5). delete message header opt array 
*
*
*
 *    ================================================================*/
int EMECodecLib_DeleteHandle(IEMSGHandle *phEmailHandle)
{
   WE_INT32       iResult = EME_SUCCESS;  /* must be init to SUCCESS */
   void           *pvMemMgr = NULL;
   stEMSGHandle   *pstEmailHandle = NULL;
  
   /*check input parameters*/
   if (NULL == phEmailHandle || *phEmailHandle == NULL)
   {
      EME_UTILS_LOG_ERROR(("handle is NULL,don't need free memory!"));
      return EME_SUCCESS;
   }

   pstEmailHandle = *phEmailHandle;

   if(NULL == pstEmailHandle)
   {
      return EME_SUCCESS;
   }

   pvMemMgr = pstEmailHandle->pvMemMgr;

   do 
   {
      pstEmailHandle = (stEMSGHandle *)(*phEmailHandle);

      if (NULL != pstEmailHandle->pvEmailMsg)
      {
         iResult = EME_MailMsg_ClearMsgParts(pstEmailHandle,pstEmailHandle->pvEmailMsg);
         EME_UTILS_BREAKIF(iResult,"EMECodecLib_DeleteHandle,EME_MailMsg_ClearMsgParts failed!!");
         
         iResult = EME_DestroyMailMsg(pstEmailHandle,&pstEmailHandle->pvEmailMsg);
         EME_UTILS_BREAKIF(iResult,"EMECodecLib_DeleteHandle,EME_DestroyMailMsg failed!!");
      }
      if (NULL != pstEmailHandle->pstEncodedOpts )
      {
         //iResult = EME_FreeAllOptsValue(pvMemMgr,pstEmailHandle->pstEncodedOpts);
         EME_UTILS_FREEIF(pvMemMgr,pstEmailHandle->pstEncodedOpts);
      }
      if (NULL != pstEmailHandle->pstHeaderOpts)
      {
         iResult = EME_FreeAllOptsValue(pvMemMgr,pstEmailHandle->pstHeaderOpts);
         EME_UTILS_FREEIF(pvMemMgr,pstEmailHandle->pstHeaderOpts);
      }

      if (pstEmailHandle->pstPartList != NULL)
      {
         EME_FreeMsgPartList(pvMemMgr, pstEmailHandle->pstPartList);
         pstEmailHandle->pstPartList = NULL;
      }
      
      if(NULL != pstEmailHandle->pICBMgr && pstEmailHandle->pfnRegister != NULL)
      {
         OEM_AsyncProc_CancelProc(pstEmailHandle->pICBMgr, 
            pstEmailHandle->pfnRegister,
            pstEmailHandle->pvRegisterData);
      }
      EME_UTILS_FREEIF(pvMemMgr,pstEmailHandle);

      *phEmailHandle = NULL;
      
   } while(0);
   
   return iResult;
}


/* --------------------------------------------------------------------
* 
*    FUNCTION        EMECodecLib_AddHeader()
*
*    Pseudo Code:    
*              1. check input parameters
*              2. add message header to message handle 	
* 
 *    ================================================================*/
int EMECodecLib_AddHeader(IEMSGHandle hEmailHandle, EmailOpt *pstHeaderOpts)
{
   WE_INT32     iResult         =  EME_SUCCESS;  /* must be init to SUCCESS */
   stEMSGHandle *pstEmailHandle = (stEMSGHandle *)hEmailHandle;
   void         *pvMemMgr       = NULL;

   /*check input parameters*/
   if (NULL == pstEmailHandle || NULL == pstHeaderOpts)
   {
      EME_UTILS_LOG_ERROR(("@@@@@@@EMECodecLib_AddHeader(),input parameters is invalid!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEmailHandle->pvMemMgr;

   do 
   {
      /**************************** ??? check header opts *********************************/
      
      if (pstEmailHandle->pstHeaderOpts != NULL)
      {
         EME_UTILS_LOG_ERROR(("@@@@@@@EMECodecLib_AddHeader(),header already added"));
         iResult = EME_FAILED;
         break;
      }

      pstEmailHandle->pstHeaderOpts = EME_CopyOpts(pvMemMgr, pstHeaderOpts);
      EME_UTILS_BREAKNULL(pstEmailHandle->pstHeaderOpts, iResult, EME_FAILED, "Copy opts failed");

   } while(0);
   
   return iResult;
}


/* --------------------------------------------------------------------
* 
*    FUNCTION        EMECodecLib_AddPart()
*
*    Pseudo Code:    
*              1. check input parameters
*              2. add message body to message handle
*    
*  		
* 
*    ================================================================*/
int EMECodecLib_AddPart(IEMSGHandle hEmailHandle, EmailOpt *pstOpts)
{
   WE_INT32       iResult = EME_SUCCESS;  /* must be init to SUCCESS */
   void           *pvMemMgr = NULL;
   stEMSGHandle   *pstEmailHandle = (stEMSGHandle *)hEmailHandle;
   St_MsgPartList *pstNewPart = NULL;
   St_MsgPartList *pstTmp = NULL;
   St_MsgPartList *pstLast = NULL;

   if (NULL == pstEmailHandle || NULL == pstOpts)
   {
      EME_UTILS_LOG_ERROR(("@@@@@@@EMECodecLib_AddPart(),input parameters is invalid!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEmailHandle->pvMemMgr;

   do 
   {
      /**************************** ??? check part opts *********************************/
      

      pstNewPart = EME_UTILS_MALLOC(pvMemMgr, sizeof(St_MsgPartList));
      EME_UTILS_BREAKNULL(pstNewPart, iResult, EME_NOMEMORY, "no memory");
      EME_UTILS_MEMSET(pstNewPart, 0, sizeof(St_MsgPartList));

      pstNewPart->pstPartOpts = EME_CopyOpts(pvMemMgr, pstOpts);
      EME_UTILS_BREAKNULL(pstNewPart->pstPartOpts, iResult, EME_NOMEMORY, "copy opts failed");
      
      iResult = EME_ReadFilePart(pstEmailHandle->pIFileMgr, pvMemMgr, pstNewPart->pstPartOpts);
      EME_UTILS_BREAKIF(iResult, "read from file failed");

      if (pstEmailHandle->pstPartList == NULL)
      {
         pstEmailHandle->pstPartList = pstNewPart;
      }
      else
      {
         pstTmp = pstEmailHandle->pstPartList;
         pstLast = pstEmailHandle->pstPartList;
         while (pstTmp != NULL)
         {
            pstLast = pstTmp;
            pstTmp = pstTmp->pstNext;
         }
         
         pstLast->pstNext = pstNewPart;
      }

      pstEmailHandle->uiPartsNum++;
      
   } while(0);

   if (iResult != EME_SUCCESS)
   {
      EME_UTILS_FREEIF(pvMemMgr, pstNewPart);
   }
   
   return iResult;
}

/* --------------------------------------------------------------------
* 
*    FUNCTION        EMECodecLib_Encoding()
*
*    Pseudo Code:    
*              1. check input parameters
*              2. Encoding operation
*              3. get message type
*              4. error handling
*    
*  		
* 
 *    ================================================================*/
int EMECodecLib_Encoding(IEMSGHandle hEmailHandle, EmailOpt **ppstOpts)
{
   WE_INT32       iResult =  EME_SUCCESS;  /* must be init to SUCCESS */
   stEMSGHandle   *pstEmailHandle = (stEMSGHandle *)hEmailHandle;
   void           *pvMemMgr = NULL;
   WE_UINT32      uiIndex = 0;
   EmailOpt       *pstOneOpt = NULL;

   /*check input parameters*/
   if (NULL == pstEmailHandle || NULL == ppstOpts)
   {
      EME_UTILS_LOG_ERROR(("EMECodecLib_Encoding(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEmailHandle->pvMemMgr;

   do 
   {
      if (pstEmailHandle->pstEncodedOpts != NULL)
      {
         EME_UTILS_FREEIF(pvMemMgr, pstEmailHandle->pstEncodedOpts);
      }

      iResult = EME_SetHeader(pstEmailHandle);
      EME_UTILS_BREAKIF(iResult, "EMECodecLib_Encoding,EME_SetHeader(),failed!!!!!!!!!!!");
            
      for (uiIndex = 0; uiIndex < pstEmailHandle->uiPartsNum; uiIndex++)
      {
         iResult = EME_SetOneBodyPart(pstEmailHandle, uiIndex);
         EME_UTILS_BREAKIF(iResult, "EMECodecLib_Encoding, EME_SetOneBodyPart failed!!!!!!!!!!!");
      }
      EME_UTILS_BREAKIF(iResult,"EMECodecLib_Encoding, EME_SetOneBodyPart() failed!!!!!!!!!!!");

      iResult = EME_EncodingMailMsg(pstEmailHandle,pstEmailHandle->pvEmailMsg);
      EME_UTILS_BREAKIF(iResult, "EMECodecLib_Encoding,EME_EncodingMailMsg failed!!!!!!!!!!!");
      
      pstEmailHandle->pstEncodedOpts = EME_UTILS_MALLOC(pvMemMgr, sizeof(EmailOpt) * EMAIL_MAX_ENCODED_OPT);
      EME_UTILS_BREAKNULL(pstEmailHandle->pstEncodedOpts,
                          iResult,
                          EME_NOMEMORY,
                          "EMECodecLib_Encoding,EME_EncodingMailMsg failed!!!!!!!!!!!");
      EME_UTILS_MEMSET(pstEmailHandle->pstEncodedOpts, 0, sizeof(EmailOpt) * EMAIL_MAX_ENCODED_OPT);

      pstOneOpt = pstEmailHandle->pstEncodedOpts;

      pstOneOpt->iId = EMAILMESSAGE_OPT_SRC_TYPE;
      pstOneOpt->pVal = (WE_VOID*)EMAIL_ENCODE_BYBUFF;
      pstOneOpt++;
      
      pstOneOpt->iId  = EMAILMESSAGE_OPT_BUFF;
      pstOneOpt->pVal = EME_MailMsg_GetEncodeBuf(pstEmailHandle->pvEmailMsg);
      pstOneOpt++;
      
      pstOneOpt->iId  = EMAILOPT_END;
      
      *ppstOpts = pstEmailHandle->pstEncodedOpts;

   } while(0);
   
   return iResult;
}

/* --------------------------------------------------------------------
* 
*    FUNCTION        EMECodecLib_Decoding()
*
*    Pseudo Code:    
*              1. check input parameters
*              2. Decoding operation
*              3. get message type value from email opts.
*              4. error handling 
*    
*  		
* 
 *    ================================================================*/
int EMECodecLib_Decoding(IEMSGHandle hEmailHandle, EmailOpt *pstOpts)
{
   WE_INT32      iResult          =  EME_FAILED;  
   stEMSGHandle  *pstEmailHandle  = (stEMSGHandle *)hEmailHandle;
   EmailOpt      stMsgPartOpt;
   EME_UTILS_MEMSET(&stMsgPartOpt,0x00,sizeof(EmailOpt));

   /*check input parameters*/
   if (NULL == pstEmailHandle || NULL == pstOpts)
   {
      EME_UTILS_LOG_ERROR(("EMECodecLib_Encoding(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   
   do 
   {
      switch((WE_INT32)pstOpts[1].pVal)
      {
      case EMAIL_DECODETYPE_HEADER:
         iResult = EME_DecodeMsg_JustHd(pstEmailHandle, (WE_CHAR *)pstOpts[2].pVal,TRUE);
         break;

      case EMAIL_DECODETYPE_BODYSTRUCT:
         iResult = EME_DecodeBodyStruct(pstEmailHandle, (WE_CHAR *)pstOpts[2].pVal);
         break;

      case EMAIL_DECODETYPE_WHOLEMSG:
         iResult = EME_DecodeEmailMsg(pstEmailHandle, (WE_CHAR *)pstOpts[2].pVal);
         break;

      case EMAIL_DECODETYPE_MSGPART:
         iResult = EME_DecodeMsg_JustPart(pstEmailHandle,pstOpts,(WE_CHAR *)pstOpts[2].pVal);
         break;

      default:
         iResult = EME_FAILED;
         break;
      }
   } while(0);
   
   return iResult;
}

/* --------------------------------------------------------------------
* 
*    FUNCTION        EMECodecLib_GetHeader()
*
*    Pseudo Code:    
*              1. check input parameters
*              2. get message header from  message handle
*              
*  
*    
*  		
* 
 *    ================================================================*/
int EMECodecLib_GetHeader(IEMSGHandle hEmailHandle, EmailOpt **ppstOpts)
{
   stEMSGHandle  *pstEmailHandle = (stEMSGHandle *)hEmailHandle;
   
   /*check input parameters*/
   if (NULL == pstEmailHandle || NULL == ppstOpts)
   {
      EME_UTILS_LOG_ERROR(("EMECodecLib_GetHeader(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   
   /*get message header from  message handle*/
   *ppstOpts = pstEmailHandle->pstHeaderOpts;
   
   return EME_SUCCESS;
}


/* --------------------------------------------------------------------
* 
*    FUNCTION        EMECodecLib_GetNumOfParts()
*
*    Pseudo Code:    
*              1. check input parameters
*              2. get the number of body part  from  message handle
*  		
* 
 *    ================================================================*/
int EMECodecLib_GetNumOfParts(IEMSGHandle hEmailHandle, WE_UINT32 *partNum)
{
   stEMSGHandle  *pstEmailHandle = (stEMSGHandle *)hEmailHandle;
   /*check input parameters*/
   if (NULL == pstEmailHandle || NULL == partNum)
   {
      EME_UTILS_LOG_ERROR(("EMECodecLib_GetNumOfParts(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   /*get the number of body part  from  message handle*/
   *partNum = pstEmailHandle->uiPartsNum;
   
   return EME_SUCCESS;
}


/* --------------------------------------------------------------------
* 
*    FUNCTION        EMECodecLib_GetPart()
*
*    Pseudo Code:    
*              1. check input parameters
*              2. get message body from  message handle
*  
*    
*  		
* 
 *    ================================================================*/
int EMECodecLib_GetPart(IEMSGHandle hEmailHandle, WE_UINT32 uiPartIndex, EmailOpt **ppstOpts)
{
   WE_INT32 iResult =  EME_SUCCESS;  /* must be init to SUCCESS */
   stEMSGHandle   *pstEmailHandle = (stEMSGHandle *)hEmailHandle;
   St_MsgPartList *pMsgPartTemp = NULL;
   WE_UINT32      uiIndex = 0;
   
   /*check input parameters*/
   if (NULL == pstEmailHandle || NULL == pstEmailHandle->pstPartList || ppstOpts == NULL)
   {
      EME_UTILS_LOG_ERROR(("EMECodecLib_GetPart(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   
   do 
   {
      pMsgPartTemp = pstEmailHandle->pstPartList;
      while (uiIndex < uiPartIndex)
      {
         if (NULL == pMsgPartTemp)
         {  
            iResult = EME_FAILED;
            break;
         }
         pMsgPartTemp = pMsgPartTemp->pstNext;
         uiIndex++;
      }
      EME_UTILS_BREAKIF(iResult,"@@@@@@@@@have no body part in message !!!!!!!!!!");
      EME_UTILS_BREAKNULL(pMsgPartTemp,
                          iResult,
                          EME_FAILED,
                          "the part you which you want is not exist !!!!!!!!!!! ");
      
      /*get message body from  message handle*/
      *ppstOpts = pMsgPartTemp->pstPartOpts;
      
   } while(0);
   
   return iResult;
}


static WE_INT32 EME_ReadFilePart(WE_VOID* pvFileMgr, WE_VOID* pvMemMgr, EmailOpt *pstPartOpts)
{  
   WE_UINT32   uiSize = 0;
   FILE_HANDLE hFile = NULL;
   EmailOpt    stOpt = {0};
   WE_INT32    iRet = EME_SUCCESS;
   WE_CHAR     *pcData = NULL;
   WE_CHAR     *pcOrigData = NULL;

   if (pstPartOpts == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      iRet = EME_GetOptsByIndex(pstPartOpts, EMAILPART_OPT_SRC_TYPE, 0, &stOpt);
      EME_UTILS_BREAKIF(iRet, "get src type failed");
      if ((WE_UINT32)stOpt.pVal != EMAILPART_SRCTYPE_FILE)
      {
         return EME_SUCCESS;
      }

      EME_UTILS_MEMSET(&stOpt, 0, sizeof(EmailOpt));
      iRet = EME_GetOptsByIndex(pstPartOpts, EMAILPART_OPT_DATA_BUFF, 0, &stOpt);
      EME_UTILS_BREAKIF(iRet, "get data buf failed");
      EME_UTILS_BREAKNULL(stOpt.pVal, iRet, EME_FAILED, "get data buf failed");

      hFile = EME_FileOpen(pvFileMgr, (WE_CHAR*)stOpt.pVal, EME_OFM_READ);
      EME_UTILS_BREAKNULL(hFile, iRet, EME_FAILED, "open file failed");
   
      uiSize = EME_FileLength(hFile);

      pcData = EME_UTILS_MALLOC(pvMemMgr, uiSize + 1);
      EME_UTILS_BREAKNULL(pcData, iRet, EME_FAILED, "no memory");
      EME_UTILS_MEMSET(pcData, 0, uiSize + 1);

      EME_FileRead(hFile, pcData, uiSize);

      EME_FileClose(hFile);
      hFile = NULL;

      iRet = EME_UpdateOpt(pstPartOpts, EMAILPART_OPT_DATA_BUFF, (WE_VOID*)pcData, (WE_VOID*)&pcOrigData);
      EME_UTILS_BREAKIF(iRet, "update data buf failed");
      EME_UTILS_FREEIF(pvMemMgr, pcOrigData);

      iRet = EME_UpdateOpt(pstPartOpts, EMAILPART_OPT_SRC_TYPE, (WE_VOID*)EMAILPART_SRCTYPE_BUFF, NULL);
      EME_UTILS_BREAKIF(iRet, "update data buf failed");

      iRet = EME_UpdateOpt(pstPartOpts, EMAILPART_OPT_DATA_SIZE, (WE_VOID*)uiSize, NULL);
      EME_UTILS_BREAKIF(iRet, "update data buf failed");


   } while(0);

   if (iRet != EME_SUCCESS)
   {
      EME_UTILS_FREEIF(pvMemMgr, pcData);
      EME_FileClose(hFile);
      hFile = NULL;
   }

   return iRet;
}

/***********************************************************************************************
                                    Encoding & Decoding of Synchronism.
***********************************************************************************************/

int EMECodecLib_InitHandleEx(IEMSGHandle *phEmailHandle,void* pvMemoryMgr,void *pIFileMgr,void* pICSC, void *pICBMgr)
{
   WE_INT32       iResult = EME_SUCCESS;  /* must be init to SUCCESS */
   stEMSGHandle   *pstEmailHandle = NULL;
   
   EME_UTILS_LOG_INFO(("EMECodecLib_InitHandleEx--: Start"));
   /*check input parameters*/
   if (NULL == phEmailHandle ||NULL ==  pICSC || NULL == pICBMgr)
   {
      EME_UTILS_LOG_ERROR(("EMECodecLib_InitHandleEx--: input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   
   do 
   { 
      pstEmailHandle = EME_UTILS_MALLOC(pvMemoryMgr,sizeof(stEMSGHandle));
      EME_UTILS_BREAKNULL(pstEmailHandle,
                          iResult,
                          EME_NOMEMORY,
                          "EMECodecLib_InitHandleEx--: memory malloc failed!");
      EME_UTILS_MEMSET(pstEmailHandle,0x00,sizeof(stEMSGHandle));

      pstEmailHandle->pvMemMgr = pvMemoryMgr;
      pstEmailHandle->pIFileMgr = pIFileMgr;
      pstEmailHandle->pICSC = pICSC;
      pstEmailHandle->pICBMgr = pICBMgr;

      iResult = EME_CreateMailMsg(pstEmailHandle,&pstEmailHandle->pvEmailMsg);
      EME_UTILS_BREAKIF(iResult,"EMECodecLib_InitHandleEx--: ,EME_CreateMailMsg,failed!");
      EME_UTILS_BREAKNULL(pstEmailHandle->pvEmailMsg,
                          iResult,
                          EME_FAILED,
                          "EMECodecLib_InitHandleEx--: ,EME_CreateMailMsg,failed!!!!!");

      *phEmailHandle = pstEmailHandle;
      
   } while(0);
   
   if (EME_SUCCESS != iResult)
   {
      EMECodecLib_DeleteHandle((IEMSGHandle *)&pstEmailHandle);
   }
   EME_UTILS_LOG_INFO(("EMECodecLib_InitHandleEx--: End and Result = %d", iResult));
   return iResult;
}
/*********************** Encoding email message *******************************/
static WE_INT32 EME_ReadFileParteEx(WE_VOID* pvFileMgr, WE_VOID* pvMemMgr, EmailOpt *pstPartOpts)
{  
   WE_UINT32   uiSize = 0;
   FILE_HANDLE hFile = NULL;
   EmailOpt    stOpt = {0};
   WE_INT32    iRet = EME_SUCCESS;
   WE_CHAR     *pcData = NULL;
   WE_CHAR     *pcOrigData = NULL;

   EME_UTILS_LOG_INFO(("EME_ReadFileParteEx--: Start"));
   if (pstPartOpts == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_ReadFileParteEx--: input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   do 
   {
      iRet = EME_GetOptsByIndex(pstPartOpts, EMAILPART_OPT_SRC_TYPE, 0, &stOpt);
      EME_UTILS_BREAKIF(iRet, "EME_ReadFileParteEx--: get src type failed");
      if ((WE_UINT32)stOpt.pVal != EMAILPART_SRCTYPE_FILE)
      {
         return EME_SUCCESS;
      }

      iRet = EME_GetOptsByIndex(pstPartOpts, EMAILPART_OPT_PART_TYPE, 0, &stOpt);
      EME_UTILS_BREAKIF(iRet, "EME_ReadFileParteEx--: get part type failed");
      if((WE_UINT32)stOpt.pVal != EMAILPART_PARTTYPE_TEXTPLAIN &&
         (WE_UINT32)stOpt.pVal != EMAILPART_PARTTYPE_TEXTHTML)
      {
         return EME_SUCCESS;
      }

      EME_UTILS_MEMSET(&stOpt, 0, sizeof(EmailOpt));
      iRet = EME_GetOptsByIndex(pstPartOpts, EMAILPART_OPT_DATA_BUFF, 0, &stOpt);
      EME_UTILS_BREAKIF(iRet, "EME_ReadFileParteEx--: get data buf failed");
      EME_UTILS_BREAKNULL(stOpt.pVal, iRet, EME_FAILED, "get data buf failed");

      hFile = EME_FileOpen(pvFileMgr, (WE_CHAR*)stOpt.pVal, EME_OFM_READ);
      EME_UTILS_BREAKNULL(hFile, iRet, EME_FAILED, "EME_ReadFileParteEx--: open file failed");
   
      uiSize = EME_FileLength(hFile);

      pcData = EME_UTILS_MALLOC(pvMemMgr, uiSize + 1);
      EME_UTILS_BREAKNULL(pcData, iRet, EME_FAILED, "EME_ReadFileParteEx--: no memory");
      EME_UTILS_MEMSET(pcData, 0, uiSize + 1);

      EME_FileRead(hFile, pcData, uiSize);

      EME_FileClose(hFile);
      hFile = NULL;

      iRet = EME_UpdateOpt(pstPartOpts, EMAILPART_OPT_DATA_BUFF, (WE_VOID*)pcData, (WE_VOID*)&pcOrigData);
      EME_UTILS_BREAKIF(iRet, "EME_ReadFileParteEx--: update data buf failed");
      EME_UTILS_FREEIF(pvMemMgr, pcOrigData);

      iRet = EME_UpdateOpt(pstPartOpts, EMAILPART_OPT_SRC_TYPE, (WE_VOID*)EMAILPART_SRCTYPE_BUFF, NULL);
      EME_UTILS_BREAKIF(iRet, "EME_ReadFileParteEx--: update data buf failed");

      EME_UpdateOpt(pstPartOpts, EMAILPART_OPT_DATA_SIZE, (WE_VOID*)uiSize, NULL);

   } while(0);

   if (iRet != EME_SUCCESS)
   {
      EME_UTILS_FREEIF(pvMemMgr, pcData);
      EME_FileClose(hFile);
      hFile = NULL;
   }

   EME_UTILS_LOG_INFO(("EME_ReadFileParteEx--: End and Result = %d", iRet));
   return iRet;
}

int EMECodecLib_AddPartEx(IEMSGHandle hEmailHandle, EmailOpt *pstOpts)
{
   WE_INT32       iResult = EME_SUCCESS;  /* must be init to SUCCESS */
   void           *pvMemMgr = NULL;
   stEMSGHandle   *pstEmailHandle = (stEMSGHandle *)hEmailHandle;
   St_MsgPartList *pstNewPart = NULL;
   St_MsgPartList *pstTmp = NULL;
   St_MsgPartList *pstLast = NULL;

   EME_UTILS_LOG_INFO(("EMECodecLib_AddPartEx--: Start"));
   if (NULL == pstEmailHandle || NULL == pstOpts)
   {
      EME_UTILS_LOG_ERROR(("EMECodecLib_AddPartEx--: input parameters is invalid!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEmailHandle->pvMemMgr;

   do 
   {
      pstNewPart = EME_UTILS_MALLOC(pvMemMgr, sizeof(St_MsgPartList));
      EME_UTILS_BREAKNULL(pstNewPart, iResult, EME_NOMEMORY, "EMECodecLib_AddPartEx--: no memory");
      EME_UTILS_MEMSET(pstNewPart, 0, sizeof(St_MsgPartList));

      pstNewPart->pstPartOpts = EME_CopyOpts(pvMemMgr, pstOpts);
      EME_UTILS_BREAKNULL(pstNewPart->pstPartOpts, iResult, EME_NOMEMORY, "EMECodecLib_AddPartEx--: copy opts failed");
      
      iResult = EME_ReadFileParteEx(pstEmailHandle->pIFileMgr, pvMemMgr, pstNewPart->pstPartOpts);
      EME_UTILS_BREAKIF(iResult, "EMECodecLib_AddPartEx--: read from file failed");

      if (pstEmailHandle->pstPartList == NULL)
      {
         pstEmailHandle->pstPartList = pstNewPart;
      }
      else
      {
         pstTmp = pstEmailHandle->pstPartList;
         pstLast = pstEmailHandle->pstPartList;
         while (pstTmp != NULL)
         {
            pstLast = pstTmp;
            pstTmp = pstTmp->pstNext;
         }
         
         pstLast->pstNext = pstNewPart;
      }

      pstEmailHandle->uiPartsNum++;
      
   } while(0);

   if (iResult != EME_SUCCESS)
   {
      EME_UTILS_FREEIF(pvMemMgr, pstNewPart);
   }
   
   EME_UTILS_LOG_INFO(("EMECodecLib_AddPartEx--: End and Result = %d", iResult));
   return iResult;
}

int EMECodecLib_EncodingEx(IEMSGHandle hEmailHandle, WE_CHAR *pcDir, void *pvUserData, PFNENCODINGCB pfn, EmailOpt **ppstOpts)
{
   WE_INT32       iResult =  EME_SUCCESS;  /* must be init to SUCCESS */
   stEMSGHandle   *pstEmailHandle = (stEMSGHandle *)hEmailHandle;
   void           *pvMemMgr = NULL;
   WE_UINT32      uiIndex = 0;

   EME_UTILS_LOG_INFO(("-------------------------------Start Encoding %d", EME_UTILS_GETTIMEMS));
   EME_UTILS_LOG_INFO(("-------------------------------Start pcDir %s\n", pcDir));
   EME_UTILS_LOG_INFO(("EMECodecLib_EncodingEx--: Start"));
   /*check input parameters*/
   if (NULL == pstEmailHandle || pcDir == NULL)
   {
      EME_UTILS_LOG_ERROR(("EMECodecLib_EncodingEx--: input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   if(NULL == pfn && ppstOpts == NULL)
   {
       EME_UTILS_LOG_ERROR(("EMECodecLib_EncodingEx--: pfn and ppstiots all is null!!!!"));
      return EME_INVALID_DATA;
   }

   pvMemMgr = pstEmailHandle->pvMemMgr;

   do 
   {
      if (pstEmailHandle->pstEncodedOpts != NULL)
      {
         EME_UTILS_FREEIF(pvMemMgr, pstEmailHandle->pstEncodedOpts);
      }

      pstEmailHandle->pfnEncoding = pfn;
      pstEmailHandle->pvUserData = pvUserData;
      pstEmailHandle->pfnDecoding = NULL;
      
      iResult = EME_SetHeader(pstEmailHandle);
      EME_UTILS_BREAKIF(iResult, "EMECodecLib_EncodingEx--: EME_SetHeader(),failed!!!!!!!!!!!");
      EME_UTILS_LOG_INFO(("EMECodecLib_EncodingEx: pstEmailHandle->uiPartsNum = %d\n", pstEmailHandle->uiPartsNum));
		
      for (uiIndex = 0; uiIndex < pstEmailHandle->uiPartsNum; uiIndex++)
      {
         iResult = EME_SetOneBodyPartEx(pstEmailHandle, uiIndex);
         EME_UTILS_BREAKIF(iResult, "EMECodecLib_EncodingEx--: EME_SetOneBodyPart failed!!!!!!!!!!!");
      }
      EME_UTILS_BREAKIF(iResult,"EMECodecLib_EncodingEx--: EME_SetOneBodyPart() failed!!!!!!!!!!!");

      iResult = EME_EncodingMailMsgEx(pstEmailHandle,pstEmailHandle->pvEmailMsg, pcDir);
      EME_UTILS_BREAKIF(iResult, "EMECodecLib_EncodingEx--: ME_EncodingMailMsg failed!!!!!!!!!!!");

      if(pfn == NULL)
      {
         *ppstOpts = pstEmailHandle->pstEncodedOpts;
      }

   } while(0);
   EME_UTILS_LOG_INFO(("EMECodecLib_EncodingEx--: End and Result = %d", iResult));
   return iResult;
}

/*********************** Decoding email message *******************************/
int EMECodecLib_DecodingEx(IEMSGHandle hEmailHandle, WE_CHAR *pcDir, EmailOpt *pstOpts, void *pvUserData, PFNDECODINGCB pfn)
{
   WE_INT32      iResult          =  EME_FAILED;  
   stEMSGHandle  *pstEmailHandle  = (stEMSGHandle *)hEmailHandle;

   EmailOpt      stMsgPartOpt;

   WE_INT32     iDecodingType = 0;
   WE_INT32     iSrcType = 0;

   EME_UTILS_LOG_INFO(("EMECodecLib_DecodingEx--: Start"));
   /*check input parameters*/
   if (NULL == pstEmailHandle || NULL == pstOpts || NULL == pcDir)
   {
      EME_UTILS_LOG_ERROR(("EMECodecLib_DecodingEx--: input parameters is invalid!!!!"));
      return EME_INVALID_DATA;
   }

   EME_UTILS_LOG_INFO(("EME Performance: start decoding at time %d", EME_UTILS_GETTIMEMS));
   do 
   {
      pstEmailHandle->pfnDecoding = pfn;
      pstEmailHandle->pvUserData = pvUserData;

      EME_UTILS_MEMSET(&stMsgPartOpt, 0, sizeof(EmailOpt));
      iResult = EME_GetOptsByIndex(pstOpts, EMAILMESSAGE_OPT_SRC_TYPE, 0, &stMsgPartOpt);
      EME_UTILS_BREAKIF(iResult, "EMECodecLib_DecodingEx--: get src type failed");
      iSrcType = (WE_INT32)stMsgPartOpt.pVal;
      if(iSrcType != EMAIL_ENCODE_BYFILE)
      {
         EME_UTILS_LOG_ERROR(("EMECodecLib_DecodingEx--: Src Type Error"));
         return EME_FAILED;
      }

      EME_UTILS_MEMSET(&stMsgPartOpt,0x00, sizeof(EmailOpt));
      iResult = EME_GetOptsByIndex(pstOpts, EMIALMESSAGE_OPT_DECODE_TYPE, 0, &stMsgPartOpt);
      EME_UTILS_BREAKIF(iResult, "EMECodecLib_DecodingEx--: get decoding type failed");
      iDecodingType = (WE_INT32)stMsgPartOpt.pVal;

      EME_UTILS_MEMSET(&stMsgPartOpt, 0, sizeof(EmailOpt));
      iResult = EME_GetOptsByIndex(pstOpts, EMAILMESSAGE_OPT_BUFF,  0, &stMsgPartOpt);
      EME_UTILS_BREAKIF(iResult, "EMECodecLib_DecodingEx--: get buffer failed");
      
      switch(iDecodingType)
      {
      case EMAIL_DECODETYPE_HEADER:
         EME_UTILS_LOG_INFO(("EMECodecLib_DecodingEx--: EMAIL_DECODETYPE_HEADER"));
         iResult = EME_DecodeMsg_JustHdEx(pstEmailHandle, (WE_CHAR*)stMsgPartOpt.pVal, pcDir, TRUE);
         break;
         
      case EMAIL_DECODETYPE_BODYSTRUCT:
         EME_UTILS_LOG_INFO(("EMECodecLib_DecodingEx--: EMAIL_DECODETYPE_BODYSTRUCT"));
         iResult = EME_DecodeBodyStructEx(pstEmailHandle, (WE_CHAR *)stMsgPartOpt.pVal, pcDir);
         break;
         
      case EMAIL_DECODETYPE_WHOLEMSG:
         EME_UTILS_LOG_INFO(("EMECodecLib_DecodingEx--: EMAIL_DECODETYPE_WHOLEMSG"));
         iResult = EME_DecodeEmailMsgEx(pstEmailHandle, (WE_CHAR *)stMsgPartOpt.pVal, pcDir);
         break;
         
      case EMAIL_DECODETYPE_MSGPART:
         EME_UTILS_LOG_INFO(("EMECodecLib_DecodingEx--: EMAIL_DECODETYPE_MSGPART"));
         iResult = EME_DecodeMsg_JustPartEx(pstEmailHandle, pstOpts, (WE_CHAR*)stMsgPartOpt.pVal, pcDir);
         break;
         
      default:
         iResult = EME_FAILED;
         break;
      }
   } while(0);
   EME_UTILS_LOG_INFO(("EMECodecLib_DecodingEx--: End and Result = %d", iResult));
   return iResult;
}
#if 0
int EMECodecLib_UnZip(IEMSGHandle hEmailHandle, void *pvInFileName, void *pvOutFileName)
{
#define EME_ZIP_CROW_SIZE   24 * 1024
	WE_CHAR        *pcInStream = NULL;
	WE_CHAR        *pcOutStream = NULL;
	int            iRet = EME_SUCCESS;
	stEMSGHandle   *pstEmailHandle  = (stEMSGHandle *)hEmailHandle;
	
	void  *pvInFile = NULL;        
	void  *pvOutFile = NULL;
	WE_UINT32 uiInFileSize = 0;
	WE_UINT32 uiHadRead = 0;
	WE_UINT32 uiTotalRead = 0;
	WE_HANDLE hZipHdl = NULL;
	WE_UINT32 uiReadLen = 0;
	
   EME_UTILS_LOG_INFO(("EMECodecLib_UnZip--: Start"));
   /*check input parameter*/
	if(NULL == hEmailHandle || NULL == pvInFileName || pvOutFileName == NULL)
	{
      EME_UTILS_LOG_ERROR(("EMECodecLib_UnZip--: input parameter error and return"));
		return EME_FAILED;
	}
	
	do 
	{
		EME_DeleteFile(pstEmailHandle->pIFileMgr, pvOutFileName);
		pvInFile = EME_FileOpen(pstEmailHandle->pIFileMgr, pvInFileName, EME_OFM_READ);
		EME_UTILS_BREAKNULL(pvInFile, iRet, EME_FAILED, "EMECodecLib_UnZip--: open file failed");
		pvOutFile = EME_FileOpen(pstEmailHandle->pIFileMgr, pvOutFileName, EME_OFM_CREATE);
		EME_UTILS_BREAKNULL(pvOutFile, iRet, EME_FAILED, "EMECodecLib_UnZip--: create file failed");
		
		pcInStream = EME_UTILS_MALLOC(pstEmailHandle->pvMemMgr, EME_ZIP_CROW_SIZE + 1);
		EME_UTILS_BREAKNULL(pcInStream, iRet, EME_NOMEMORY, "EMECodecLib_UnZip--: no memory");
		pcOutStream = EME_UTILS_MALLOC(pstEmailHandle->pvMemMgr, EME_ZIP_CROW_SIZE + 1);
		EME_UTILS_BREAKNULL(pcOutStream, iRet, EME_NOMEMORY, "EMECodecLib_UnZip--: no memory");
		EME_UTILS_MEMSET(pcInStream, 0, EME_ZIP_CROW_SIZE + 1);
		EME_UTILS_MEMSET(pcOutStream, 0, EME_ZIP_CROW_SIZE + 1);
		
		uiInFileSize = EME_FileLength(pvInFile);
		uiHadRead = EME_FileRead(pvInFile, pcInStream, EME_ZIP_CROW_SIZE);
		iRet = (uiHadRead > 0) ? EME_SUCCESS : EME_FAILED;
		EME_UTILS_BREAKIF(iRet, "EMECodecLib_UnZip--: read file size failed");
		uiTotalRead += uiHadRead;
		
		do
		{
			WE_BOOL bIsEnd = FALSE;
			if(hZipHdl == NULL)
			{
				/*init the zip handle if it's not created*/
				hZipHdl = Unzip_StreamSet((const WE_UINT8 *)pcInStream,
					(WE_UINT32)uiHadRead,
					TRUE);
				EME_UTILS_BREAKNULL(hZipHdl, iRet, EME_FAILED, "EMECodecLib_UnZip--: stream set failed");
			}
			else
			{
				/*update the zip handle for new buffer*/
				iRet = Unzip_StreamUpdate(hZipHdl, (const WE_UINT8*)pcInStream, uiHadRead);
				iRet = (iRet == ZIP_OK) ? EME_SUCCESS : EME_FAILED;
				EME_UTILS_BREAKIF(iRet, "EMECodecLib_UnZip--: update stream failed");
			}
			
			while(!bIsEnd)
			{
				/*read data from stream*/
				uiReadLen = EME_ZIP_CROW_SIZE;
				iRet = Unzip_StreamRead(hZipHdl, (WE_UINT8*)pcOutStream, &uiReadLen);
            if(uiReadLen == 0)
            {
               bIsEnd = TRUE;
               break;
            }

				EME_FileWrite(pvOutFile, pcOutStream, uiReadLen);
				EME_UTILS_MEMSET(pcOutStream, 0, EME_ZIP_CROW_SIZE + 1);
            bIsEnd = (uiReadLen < EME_ZIP_CROW_SIZE) ? TRUE : FALSE;
			}
			
			/*if it's not the end of the file then read one buffer from file*/
			if(uiTotalRead < uiInFileSize)
			{
				EME_UTILS_MEMSET(pcInStream, 0, EME_ZIP_CROW_SIZE + 1);
				uiHadRead = EME_FileRead(pvInFile, pcInStream, EME_ZIP_CROW_SIZE);
				iRet = (uiHadRead > 0) ? EME_SUCCESS : EME_FAILED;
				EME_UTILS_BREAKIF(iRet, "EMECodecLib_UnZip--: read file failed");
				uiTotalRead += uiHadRead;
			}
			else
         {
            iRet = (iRet == ZIP_STREAM_END) ? EME_SUCCESS : EME_FAILED;
            break;
         }
		} while(1);
		EME_UTILS_BREAKIF(iRet, "EMECodecLib_UnZip--: unzip failed");
	} while(0);

	Unzip_StreamClose(hZipHdl);
	hZipHdl = NULL;
	EME_FileClose(pvOutFile);
	EME_FileClose(pvInFile);
	EME_UTILS_FREEIF(pstEmailHandle->pvMemMgr, pcInStream);
	EME_UTILS_FREEIF(pstEmailHandle->pvMemMgr, pcOutStream);
	if(iRet != EME_SUCCESS)
	{
		EME_DeleteFile(pstEmailHandle->pIFileMgr, pvOutFileName);
	}
	EME_UTILS_LOG_INFO(("EMECodecLib_UnZip--: End and Result = %d", iRet));
	return iRet;
}
#endif